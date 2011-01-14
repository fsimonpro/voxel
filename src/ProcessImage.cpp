
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
#include <time.h>

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

void ComputeHSVFromRGB(BYTE r,BYTE g, BYTE b, double &h,double &s, double &v);
void ComputeRGBFromHSV(double h,double s, double v,BYTE &r,BYTE &g,BYTE &b);

double g_curangle = -1.3f;
unsigned g_curposx = 500;
unsigned g_curposy = 500;
unsigned g_curposz = 40;


#define FOV 0.7f
#define FOV_2 FOV/2.0f

//TODO séparé flipdistance et distance focale ?
#define FLIPDISTANCE 200
#define SCREENSIZE 300



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


void ProcessImage::SetImagesPaths(Paths paths)
{
	g_curpath = paths;
}

void ProcessImage::Turn(bool left)
{
	if (left)
		g_curangle -= 0.1;
	else
		g_curangle +=0.1;
}


#define STEPMOVE  20
void ProcessImage::Move(int dirx,int diry,int dirz)
{
	g_curposy += diry*STEPMOVE;
	g_curposx += dirx*STEPMOVE;
	g_curposz += dirz*STEPMOVE;
}


void ProcessImage::SetExportDir(string dir)
{
	g_psddir = dir;
	g_psddir += "\\";
}

//construit l'image finale à partir de g_pictures
FIBITMAP* ProcessImage::BuildImages(FIBITMAP* input)
{
	/*
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
*/
	return 0;
}





void ProcessImage::SetName(string name)
{
	g_psdname = name;
}

double ProcessImage::ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform)
{
	unsigned int xmain,ymain;
	int curdiffx,curdiffy;
	double pixelangle;
	signed int curheight;
	unsigned int LastDist[SCREENRESOLUTION][SCREENRESOLUTION]; //buffer type zbuffer
	unsigned int curdistsquare;
	unsigned int widthheightmap;
	unsigned int heightonscreen;
	double curdist;

	int curxoutput; 
	if (!g_curheightmap) 
		g_curheightmap = FreeImage_Load(FIF_TARGA,"height.tga");
	if (!g_curalbedo)
		g_curalbedo = FreeImage_Load(FIF_TARGA,"albedo.tga");

	time_t starttime = time(NULL);

	memset(&LastDist,0,SCREENRESOLUTION*SCREENRESOLUTION*sizeof(unsigned int));

	//FIBITMAP* output = FreeImage_Allocate(SCREENSIZE,SCREENSIZE,24);
	//premiére approche bourrine et naive on parcourt toute l'image des heights
	
	BYTE *bitsalbedo = (BYTE*)FreeImage_GetBits(g_curalbedo);
	unsigned pitchalbedo = FreeImage_GetPitch(g_curalbedo);

	BYTE *bitsheightmap = (BYTE*)FreeImage_GetBits(g_curheightmap);
	unsigned pitchheightmap = FreeImage_GetPitch(g_curheightmap);

	

	widthheightmap = FreeImage_GetWidth(g_curheightmap);
	for(ymain = 0; ymain < FreeImage_GetHeight(g_curheightmap) ; ymain++) 
	{
		BYTE *pixelalbedo = (BYTE*)bitsalbedo;
		BYTE *pixelheightmap = (BYTE*)bitsheightmap;
		curdiffy = g_curposy - ymain;

		for(xmain = 0; xmain < widthheightmap; xmain++) 
		{
			curdiffx = g_curposx - xmain;
			pixelangle = atan2((double)curdiffy,(double)curdiffx);
			
			if (ABS(pixelangle-g_curangle) < (FOV_2)) 
			{
				//pixel à l'intérieur !
				//on calcule le x sur l'image en sortie
				curxoutput = (((pixelangle-g_curangle) + (FOV_2)) * SCREENRESOLUTION) / FOV;
				assert( curxoutput >= 0 && curxoutput <= SCREENRESOLUTION && " probléme calcul de curxouput !");

				//on récupére la valeur
				curheight = *pixelheightmap;
				curdistsquare = (curdiffx)*(curdiffx) + (curdiffy)*(curdiffy);
				curdist = sqrt((double)curdistsquare);

				if (curdist > FLIPDISTANCE)
				{
					curheight = MAX(0,curheight + g_curposz);
					//on calcule la hauteur sur l'écran par thalés
					heightonscreen = (((SCREENSIZE/2 - curheight ) * (curdist - FLIPDISTANCE)) / (curdist)) + curheight; 

					//rajout brume bleu
					BYTE blue = MIN(255,(unsigned)pixelalbedo[FI_RGBA_BLUE] + MIN(128,(curdistsquare / 15400)));
					unsigned int color = blue  | pixelalbedo[FI_RGBA_GREEN]  << 8 | pixelalbedo[FI_RGBA_RED] << 16 | 0xFF << 24 ;

					if (heightonscreen > 4 && heightonscreen < SCREENRESOLUTION) //sécurité
					{
						for (unsigned int yout = heightonscreen;yout >= heightonscreen - 4 ;yout--)
						{
							//il faut calculer la distance par rapport à la position aussi
							if ((curdistsquare < LastDist[curxoutput][yout])  || LastDist[curxoutput][yout] == 0) 
							{
								LastDist[curxoutput][yout] = curdistsquare;
								Outputforform[curxoutput + yout * SCREENRESOLUTION] = color;
							}
							else if (LastDist[curxoutput][yout] >= curdistsquare) //sinon on peut sortir tt de suite
								break;
						}
					}	
				}

			}
			pixelalbedo+=3;
			pixelheightmap++;
		}

		bitsalbedo+= pitchalbedo;
		bitsheightmap += pitchheightmap;

	}

	//FreeImage_Save(FIF_BMP,output,"E:\\Voxel\\test.bmp");

	double totaltime = difftime(time(NULL),starttime);
	return totaltime;
	//cout << totaltime <<endl;

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
		result = BuildImages(img);

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

