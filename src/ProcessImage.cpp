
#include "stdafx.h"

#include "ProcessImage.h"

#include "FreeImage.h"
#include "VoxelCore.h"

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

string g_heightpath = "Samples/height.tga";
string g_albedopath = "Samples/albedo.tga";

FIBITMAP* g_curpictures;

//heightmap + albedo fusionnes : un texel = hauteur (octet de poids fort) + couleur ; l'albedo est reechantillonne en uv sur la taille de la heightmap
vector<unsigned int> g_texture;
int g_mapwidth = 0;
int g_mapheight = 0;
int g_maxheight = 0;

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



signed int SCREENRESOLUTION = 512;

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


//charge une map et la convertit au format attendu par ComputeVoxel (8 bits pour la hauteur, 24 bits pour l'albedo)
FIBITMAP* LoadMap(const string& path, bool greyscale)
{
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path.c_str(),0);
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(path.c_str());
	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
		return NULL;
	FIBITMAP* img = FreeImage_Load(fif,path.c_str(),0);
	if (!img)
		return NULL;
	FIBITMAP* converted = greyscale ? FreeImage_ConvertToGreyscale(img) : FreeImage_ConvertTo24Bits(img);
	FreeImage_Unload(img);
	return converted;
}

//fusionne les deux images dans g_texture (voir plus haut) ; l'albedo est lu en coordonnees uv, les tailles peuvent differer
static void BuildTexture(FIBITMAP* height, FIBITMAP* albedo, vector<unsigned int>& texture, int& width, int& heightpx, int& maxheight)
{
	width = FreeImage_GetWidth(height);
	heightpx = FreeImage_GetHeight(height);
	const int albedowidth = FreeImage_GetWidth(albedo);
	const int albedoheight = FreeImage_GetHeight(albedo);
	maxheight = 0;
	texture.assign((size_t)width * heightpx, 0);
	for (int y = 0; y < heightpx; y++)
	{
		const BYTE* heightrow = FreeImage_GetScanLine(height, y);
		const BYTE* albedorow = FreeImage_GetScanLine(albedo, (int)((long long)y * albedoheight / heightpx));
		unsigned int* dest = &texture[(size_t)y * width];
		for (int x = 0; x < width; x++)
		{
			const BYTE* pixel = albedorow + ((long long)x * albedowidth / width) * 3;
			unsigned int rgb = pixel[FI_RGBA_BLUE] | pixel[FI_RGBA_GREEN] << 8 | pixel[FI_RGBA_RED] << 16;
			dest[x] = rgb | (unsigned int)heightrow[x] << 24;
			if (heightrow[x] > maxheight) maxheight = heightrow[x];
		}
	}
}

bool ProcessImage::SetMapPaths(string heightpath, string albedopath)
{
	FIBITMAP* newheight = LoadMap(heightpath,true);
	FIBITMAP* newalbedo = LoadMap(albedopath,false);
	bool ok = newheight && newalbedo;
	if (ok)
	{
		BuildTexture(newheight, newalbedo, g_texture, g_mapwidth, g_mapheight, g_maxheight);
		g_heightpath = heightpath;
		g_albedopath = albedopath;
	}
	//les images FreeImage ne servent plus une fois la texture construite
	if (newheight) FreeImage_Unload(newheight);
	if (newalbedo) FreeImage_Unload(newalbedo);
	return ok;
}

string ProcessImage::GetHeightmapPath()
{
	return g_heightpath;
}

string ProcessImage::GetAlbedoPath()
{
	return g_albedopath;
}

void ProcessImage::SetImagesPaths(Paths paths)
{
	g_curpath = paths;
	if (g_curpath.size() == 2)
		SetMapPaths(g_curpath[0], g_curpath[1]);
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


void ProcessImage::MoveForward(int dir)
{
	//la camera regarde vers pos - dist*(cos,sin) (cf ComputeVoxel)
	g_curview.posx -= (int)floor(dir * STEPMOVE * cos(g_curview.angle) + 0.5f);
	g_curview.posy -= (int)floor(dir * STEPMOVE * sin(g_curview.angle) + 0.5f);
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

//le rendu lui-meme est dans VoxelCore.cpp (code natif, multi-thread) ; ici on ne fait que lui passer la texture et la vue
double ProcessImage::ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform)
{
	if (g_texture.empty())
		SetMapPaths(g_heightpath, g_albedopath); //images par defaut

	VoxelParams params;
	params.texture = g_texture.empty() ? NULL : &g_texture[0];
	params.mapwidth = g_mapwidth;
	params.mapheight = g_mapheight;
	params.maxheight = g_maxheight;
	params.view.posx = g_curview.posx;
	params.view.posy = g_curview.posy;
	params.view.posz = g_curview.posz;
	params.view.angle = g_curview.angle;
	params.fov = FOV;
	params.flipdistance = (int)FLIPDISTANCE;
	params.resolution = SCREENRESOLUTION;
	RenderVoxel(params, Outputforform);

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

