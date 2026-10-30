#include "VoxelCore.h"

#include <math.h>
#include <vector>
#include <thread>
#include <atomic>
#include <algorithm>

namespace
{
	//tout est precalcule une fois par image : pas de division ni de flottant dans la boucle des rayons
	struct FrameTables
	{
		std::vector<int> factor;   //65536 * (dist - flip) / dist  (projection de Thales)
		std::vector<int> fog;      //brume bleue ajoutee en fonction de la distance
		int heightlut[256];        //hauteur texture -> hauteur ecran (avant projection)
	};

	const int COLUMNBLOCK = 8;

	void RenderColumn(const VoxelParams & p, const FrameTables & t, int realscreensize, int maxdist, int hmaxscreen, unsigned int * out, int col)
	{
		const int res = p.resolution;
		const int curxoutput = res - 1 - col; //meme miroir que l'ancienne projection
		const float rayangle = p.view.angle - p.fov * 0.5f + (curxoutput * p.fov) / res;
		const double cosa = cos(rayangle);
		const double sina = sin(rayangle);

		const long long startx = (long long)p.view.posx << 16;
		const long long starty = (long long)p.view.posy << 16;

		int maxy = -1; //plus haut pixel deja rempli de cette colonne
		int dist = 1;

		while (dist < maxdist && maxy < res - 1)
		{
			//pas d'un texel partout : un pas plus gros au loin creait des marches visibles sur les reliefs lointains
			const int step = 1;
			const int segmentend = maxdist;

			//point fixe 16.16 : on recalcule la position au debut du segment puis on additionne
			long long x = startx - (long long)floor(cosa * dist * 65536.0 + 0.5);
			long long y = starty - (long long)floor(sina * dist * 65536.0 + 0.5);
			const long long dx = -(long long)floor(cosa * step * 65536.0 + 0.5);
			const long long dy = -(long long)floor(sina * step * 65536.0 + 0.5);

			for (; dist < segmentend; dist += step, x += dx, y += dy)
			{
				const int xi = (int)(x >> 16);
				const int yi = (int)(y >> 16);
				if ((unsigned)xi >= (unsigned)p.mapwidth || (unsigned)yi >= (unsigned)p.mapheight)
					continue;

				const unsigned int texel = p.texture[yi * p.mapwidth + xi];
				const int curheight = t.heightlut[texel >> 24];

				int heightonscreen = curheight + (int)(((long long)(realscreensize - curheight) * t.factor[dist]) >> 16);
				if (heightonscreen > res - 1) heightonscreen = res - 1;
				if (heightonscreen <= maxy)
					continue; //masque par un point plus proche

				//rajout brume bleu
				int blue = (int)(texel & 0xFF) + t.fog[dist];
				if (blue > 255) blue = 255;
				const unsigned int color = (texel & 0x00FFFF00) | (unsigned int)blue | 0xFF000000;

				unsigned int * pixel = out + col + (res - 1 - (maxy + 1)) * res;
				for (int yout = maxy + 1; yout <= heightonscreen; yout++, pixel -= res)
					*pixel = color;
				maxy = heightonscreen;
				if (maxy >= res - 1)
					return;
			}

			//sortie anticipee : plus aucun point plus loin ne peut etre au dessus du plus haut pixel deja rempli
			if (dist >= p.flipdistance && maxy >= hmaxscreen)
				return;
		}
	}
}

void RenderVoxel(const VoxelParams & p, unsigned int * out)
{
	const int res = p.resolution;

	//ciel : degrade bleu, plus fonce en haut, clair vers l'horizon
	for (int row = 0; row < res; row++)
	{
		const float t = (float)row / (res - 1); //0 en haut, 1 en bas
		const unsigned int r = (unsigned int)(40 + t * (170 - 40));
		const unsigned int g = (unsigned int)(100 + t * (210 - 100));
		const unsigned int b = (unsigned int)(215 + t * (250 - 215));
		const unsigned int skycolor = b | g << 8 | r << 16 | 0xFF000000;
		std::fill(out + row * res, out + (row + 1) * res, skycolor);
	}

	if (!p.texture || p.mapwidth <= 0 || p.mapheight <= 0)
		return;

	const int realcurposz = (p.view.posz * res) / 100; //en fonction de la resolution de sortie !
	const int realscreensize = (450 * res) / (2 * 250); //on divise par 2 par anticipation

	//farclip : distance jusqu'au coin de la carte le plus eloigne de la camera (la camera peut etre hors carte)
	const double farx = (std::max)(fabs((double)p.view.posx), fabs((double)(p.view.posx - p.mapwidth)));
	const double fary = (std::max)(fabs((double)p.view.posy), fabs((double)(p.view.posy - p.mapheight)));
	const int maxdist = (int)sqrt(farx * farx + fary * fary) + 1;

	FrameTables t;
	t.factor.resize(maxdist + 1);
	t.fog.resize(maxdist + 1);
	for (int d = 1; d <= maxdist; d++)
	{
		t.factor[d] = 65536 - (int)(((long long)p.flipdistance << 16) / d);
		const long long distsquare = (long long)d * d;
		t.fog[d] = (int)(std::min)((long long)128, distsquare / 15400);
	}
	for (int h = 0; h < 256; h++)
		t.heightlut[h] = ((h * res) >> 8) + realcurposz;

	//majorant de la hauteur ecran de tout point plus lointain : au dela, on peut s'arreter. Le plus haut possible est
	//max(horizon, hauteur max de la carte) ; en dessous de cet horizon un point ne peut jamais depasser realscreensize
	const int hmaxtexture = t.heightlut[p.maxheight & 255];
	const int hmaxscreen = (std::max)(realscreensize, hmaxtexture);

	std::atomic<int> nextblock(0);
	const int blocks = (res + COLUMNBLOCK - 1) / COLUMNBLOCK;
	auto worker = [&]()
	{
		for (;;)
		{
			const int block = nextblock.fetch_add(1);
			if (block >= blocks)
				return;
			const int first = block * COLUMNBLOCK;
			const int last = (std::min)(first + COLUMNBLOCK, res);
			for (int col = first; col < last; col++)
				RenderColumn(p, t, realscreensize, maxdist, hmaxscreen, out, col);
		}
	};

	unsigned int threadcount = std::thread::hardware_concurrency();
	if (threadcount == 0) threadcount = 4;
	if (threadcount > 16) threadcount = 16;
	std::vector<std::thread> threads;
	for (unsigned int i = 1; i < threadcount; i++)
		threads.push_back(std::thread(worker));
	worker(); //le thread courant travaille aussi
	for (size_t i = 0; i < threads.size(); i++)
		threads[i].join();
}
