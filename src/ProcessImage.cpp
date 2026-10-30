
#include "stdafx.h"

#include "ProcessImage.h"

#include "FreeImage.h"

#include <iostream>
#include <fstream>

#include <algorithm>
#include <ostream>
#include <list>
#include <sstream>
#include <string>
#include <math.h>
#include <assert.h>

#define MAXHEIGHTBEFORECROP 0xFFFFFFFF

Paths g_curpath;

FIBITMAP* g_curpictures;
FIBITMAP* g_curheightmap;
FIBITMAP* g_curalbedo;

#define	MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#define	MAX(_a,_b) ((_a) < (_b) ? (_b) : (_a))
#define	BOUND(_val,_min,_max) (MAX(MIN((_val),(_max)),(_min)))
#define ABS(x) ((x) < 0 ? - (x) : (x))

string g_psddir = DEFAULTOUTPUTPATH;
string g_psdname = DEFAULTOUTPUTNAME;

/*
double g_curangle = -1.3f;
unsigned g_curposx = 500;
unsigned g_curposy = 500;
unsigned g_curposz = 16; //sur une base 100 !*/

Curview g_curview = {500,500,16,-1.3f};


float FOV = 0.4f;
float FOV_2 = FOV/2.0f;

//TODO séparé flipdistance et distance focale ?
unsigned int FLIPDISTANCE  = 200;
signed int VOXELSIZE = 4;
//#define SCREENSIZE 300



signed int SCREENRESOLUTION = 250;

string ConvertToString(int n)
{	ostringstream o;
	o<<n;
	return o.str();
}

string ConvertToString(double n)
{	ostringstream o;
	o<<n;
	return o.str();
}

bool stringpred(const std::string& lhs, const std::string& rhs)
{
	return std::lexicographical_compare(rhs.begin(),rhs.end(),lhs.begin(),lhs.end());
}


void ProcessImage::SetFov(int nfov)
{
	FOV = (nfov/100.0f);
	FOV_2 = FOV/2.0f;
}

void ProcessImage::SetResolution(unsigned int res)
{
	SCREENRESOLUTION = res;
}

void ProcessImage::SetFlipDistance(unsigned int n)
{
	FLIPDISTANCE = n;
}

unsigned int ProcessImage::GetResolution()
{
	return SCREENRESOLUTION;
}


void ProcessImage::SetImagesPaths(Paths paths)
{
	g_curpath = paths;
	if (g_curpath.size() == 2)
	{
		//premiére image hiehgtmap
		g_curheightmap = FreeImage_Load(FIF_TARGA,g_curpath[0].c_str());
		g_curalbedo = FreeImage_Load(FIF_TARGA,g_curpath[1].c_str());
	}

}

void ProcessImage::Turn(bool left)
{
	if (left)
		g_curview.angle -= 0.1f;
	else
		g_curview.angle +=0.1f;
}


#define STEPMOVE  20
#define STEPMOVEZ 8
void ProcessImage::Move(int dirx,int diry,int dirz)
{
	g_curview.posy  += diry*STEPMOVE;
	g_curview.posx  += dirx*STEPMOVE;
	g_curview.posz  += dirz*STEPMOVEZ;
}


void ProcessImage::SetView(int x,int y,int z,float angle)
{
	g_curview.posx = x;
	g_curview.posy = y;
	g_curview.posz = z;
	g_curview.angle = angle;
}


void ProcessImage::SetExportDir(string dir)
{
	g_psddir = dir;
	g_psddir += "\\";
}


//construit l'image finale à partir de g_pictures
/*FIBITMAP* ProcessImage::BuildImages(FIBITMAP* input)
{
	
WORD xmain,ymain;
	WORD height,width;
	BYTE r,g,b;

	width = FreeImage_GetWidth(input);
	height = FreeImage_GetHeight(input);
	unsigned bpp = FreeImage_GetBPP(input);


	if (input == NULL || bpp != 24)
	{
		
		return NULL;
	}

	FIBITMAP* result;

	//la alloue la mémoire pour le résultat
	result = FreeImage_Allocate(width,height,24);


	//boucle princpale
	//c'est ici qu'il faut fair les modifcations
	for(ymain = 0; ymain < height ; ymain++) 
	{
		BYTE *inputimagebits = FreeImage_GetScanLine(input, ymain);
		BYTE *ouputimagebits = FreeImage_GetScanLine(result, ymain);

		for(xmain = 0; xmain < width; xmain++) 
		{
			b = inputimagebits[0];g = inputimagebits[1];r = inputimagebits[2];
			
			AddLuminosity(b,g,r);
			AddContraste(b,g,r);

			//if (g_saturation != 0)
				AddSaturation(b,g,r);
			
			
			
			ouputimagebits[0] = b; //bleu
			ouputimagebits[1] = g; //vert
			ouputimagebits[2] = r; //rouge

			// jump to next pixel
			ouputimagebits += 3; //pas d'alpha 3 octets par pixels
			inputimagebits += 3;
		}

	}

	return result;

	return 0;
}*/





void ProcessImage::SetName(string name)
{
	g_psdname = name;
}

//à revoir
const Curview* ProcessImage::GetCurView()
{
	return &g_curview;
}

//projection inverse : pour chaque colonne de l'ecran on lance un rayon dans la heightmap
//et on remplit tous les pixels de la colonne (painter front-to-back), il n'y a donc plus de trous
double ProcessImage::ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform)
{
	const int res = SCREENRESOLUTION;
	signed int realcurposz = (g_curview.posz* res) /100;  //en fonction de la resolution de sortie !
	signed int realscreensize = (450 * res) / (2 * 250); //on divise par 2 par anticipation

	if (!g_curheightmap) 
		g_curheightmap = FreeImage_Load(FIF_TARGA,"height.tga");
	if (!g_curalbedo)
		g_curalbedo = FreeImage_Load(FIF_TARGA,"albedo.tga");

	assert( FreeImage_GetWidth(g_curheightmap) == FreeImage_GetWidth(g_curalbedo) && FreeImage_GetHeight(g_curheightmap) == FreeImage_GetHeight(g_curalbedo));
	memset(Outputforform,0xFF77B5FE,res*res*sizeof(unsigned int));

	BYTE *bitsalbedo = (BYTE*)FreeImage_GetBits(g_curalbedo);
	unsigned pitchalbedo = FreeImage_GetPitch(g_curalbedo);

	BYTE *bitsheightmap = (BYTE*)FreeImage_GetBits(g_curheightmap);
	unsigned pitchheightmap = FreeImage_GetPitch(g_curheightmap);

	const int widthheightmap = FreeImage_GetWidth(g_curheightmap);
	const int heightheightmap = FreeImage_GetHeight(g_curheightmap);
	const float maxdist = sqrt((float)(widthheightmap * widthheightmap + heightheightmap * heightheightmap));

	for (int col = 0; col < res; col++)
	{
		//meme mapping angle/colonne que l'ancienne projection (avec le miroir en sortie)
		int curxoutput = res - 1 - col;
		float rayangle = g_curview.angle - FOV_2 + (curxoutput * FOV) / res;
		float cosa = cos(rayangle);
		float sina = sin(rayangle);

		int maxy = -1; //plus haut pixel deja rempli de cette colonne

		for (float dist = (float)FLIPDISTANCE + 1; dist < maxdist && maxy < res - 1; dist += 1.0f)
		{
			float fx = g_curview.posx - dist * cosa;
			float fy = g_curview.posy - dist * sina;
			if (fx < 0 || fy < 0 || fx >= widthheightmap || fy >= heightheightmap)
				continue;
			int x = (int)fx;
			int y = (int)fy;

			BYTE *pixelheightmap = bitsheightmap + y * pitchheightmap + x;
			BYTE *pixelalbedo = bitsalbedo + y * pitchalbedo + x * 3;

			signed int curheight = ((*pixelheightmap * res) >> 8) + realcurposz;
			//on calcule la hauteur sur l'ecran par thales
			int heightonscreen = (int)((((realscreensize - curheight) * (dist - FLIPDISTANCE)) / dist) + curheight);
			heightonscreen = BOUND(heightonscreen, 0, res - 1);
			if (heightonscreen <= maxy)
				continue; //masque par un point plus proche

			unsigned int curdistsquare = (unsigned int)(dist * dist);
			//rajout brume bleu
			BYTE blue = MIN(255,(unsigned)pixelalbedo[FI_RGBA_BLUE] + MIN(128,(curdistsquare / 15400)));
			unsigned int color = blue  | pixelalbedo[FI_RGBA_GREEN]  << 8 | pixelalbedo[FI_RGBA_RED] << 16 | 0xFF << 24 ;

			for (int yout = maxy + 1; yout <= heightonscreen; yout++)
				Outputforform[col + (res - yout - 1) * res] = color;
			maxy = heightonscreen;
		}
	}

	return 0;

}





//charge l'image, appelle le process dessu et la sauvegarde
string ProcessImage::ComputeOutput()
{
	FIBITMAP* img;
	FIBITMAP* result;
	FREE_IMAGE_FORMAT format;
	
	FreeImage_Initialise();

	if (g_curpath.size() > 0)
	{
		format = FreeImage_GetFIFFromFilename(g_curpath[0].c_str());
		img = FreeImage_Load(format,g_curpath[0].c_str());
		//on construit le bmp en sortie
		//result = BuildImages(img);

		FreeImage_Unload(img);	
		string fullpath = g_psddir;
		fullpath+= g_psdname;
		BOOL exportok = FreeImage_Save(FIF_BMP,result,fullpath.c_str(),0);
		//if (!exportok)
		//	MessageBox(g_handle,"export pas ok !","ok",MB_OK);
		FreeImage_Unload(result);

		return fullpath;
	}

	return "";


}

string ProcessImage::GetPurePathOnly(string path)
{
	size_t pos = path.find_last_of("\\");
	if (pos != 0xFFFFFFFF)
	{
		return path.substr(0,pos+1);
	}
	else
	{
		return "";
	}
}

string ProcessImage::GetCurrentImportPath()
{
	if (g_curpath.size() > 0)
	{
		return GetPurePathOnly(g_curpath[0]);
	}

	return "c:\\";

}

