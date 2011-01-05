
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

#define MAXHEIGHTBEFORECROP 0xFFFFFFFF

Paths g_curpath;

FIBITMAP* g_curpictures;


#define	MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#define	MAX(_a,_b) ((_a) < (_b) ? (_b) : (_a))
#define	BOUND(_val,_min,_max) (MAX(MIN((_val),(_max)),(_min)))

string g_psddir = DEFAULTOUTPUTPATH;
string g_psdname = DEFAULTOUTPUTNAME;
int g_luminosity = 0;
double g_constraste = 1;
int g_saturation = 0;

void ComputeHSVFromRGB(BYTE r,BYTE g, BYTE b, double &h,double &s, double &v);
void ComputeRGBFromHSV(double h,double s, double v,BYTE &r,BYTE &g,BYTE &b);

string ConvertToString(int n)
{	ostringstream o;
	o<<n;
	return o.str();
}

void AddLuminosity(BYTE &b,BYTE &g,BYTE &r)
{
	b = BOUND(b+g_luminosity,0,255);
	g = BOUND(g+g_luminosity,0,255);
	r = BOUND(r+g_luminosity,0,255);
}

void AddContraste(BYTE &b,BYTE &g,BYTE &r)
{
	b = BOUND(b*g_constraste,0,255);
	g = BOUND(g*g_constraste,0,255);
	r = BOUND(r*g_constraste,0,255);
}

void AddSaturation(BYTE &b,BYTE &g,BYTE &r)
{
	int maxcol = MAX(b,MAX(g,r));
	int mincol = MIN(b,MIN(g,r));
	int delta = maxcol-mincol;

	if (delta == 0) return;

	double h,s,v;


	ComputeHSVFromRGB(r,g,b,h,s,v);
	s = BOUND(s + (double)g_saturation/255.0f,0.0f,1.0f);
	ComputeRGBFromHSV(h,s,v,r,g,b);

}

void ComputeHSVFromRGB(BYTE r,BYTE g, BYTE b, double &h,double &s, double &v)
{
	//on renvoit les valeurs entre 0 et 1
	double realr = (double)r /255.f;
	double realg = (double)g /255.f;
	double realb = (double)b /255.f;

	double maxcol = MAX(realb,MAX(realg,realr));
	double mincol = MIN(realb,MIN(realg,realr));
	double delta = maxcol-mincol;

	if (maxcol > 0)
		s = delta / maxcol;
	else
		s = 0;

	v = maxcol;

	if (delta == 0) 
		h =0;
	else
	{
		if (maxcol == realr)
			h = (realg-realb)/(6.0f*delta);
		else if (maxcol == realg)
			h = (realb-realr)/(6.0f*delta) + 1.0f/3.0f;
		else if (maxcol == realb)
			h = (realr-realg)/(6.0f*delta) + 2.0f/3.0f;
	}

	if (h < 0.0f)
		h+=1.0f;
	
	if (h > 1.0f)
		h-=1.0f;


}


void ComputeRGBFromHSV(double h,double s, double v,BYTE &r,BYTE &g,BYTE &b)
{
	double var_i,var_1,var_2,var_3,var_h,var_r,var_g,var_b;
	
	if ( s == 0 )                       //HSV from 0 to 1
	{
		r = v * 255;
		g = v * 255;
		b = v * 255;
	}
	else
	{
		var_h = h * 6;
		if ( var_h == 6 ) var_h = 0;      //H must be < 1
		
		var_i = int( var_h );             //Or ... var_i = floor( var_h )
		var_1 = v * ( 1 - s );
		var_2 = v * ( 1 - s * ( var_h - var_i ) );
		var_3 = v * ( 1 - s * ( 1 - ( var_h - var_i ) ) );

		if      ( var_i == 0 ) { var_r = v     ; var_g = var_3 ; var_b = var_1; }
		else if ( var_i == 1 ) { var_r = var_2 ; var_g = v     ; var_b = var_1; }
		else if ( var_i == 2 ) { var_r = var_1 ; var_g = v     ; var_b = var_3; }
		else if ( var_i == 3 ) { var_r = var_1 ; var_g = var_2 ; var_b = v;     }
		else if ( var_i == 4 ) { var_r = var_3 ; var_g = var_1 ; var_b = v;     }
		else                   { var_r = v     ; var_g = var_1 ; var_b = var_2; }

		r = var_r * 255;                //rgb results from 0 to 255
		g = var_g * 255;
		b = var_b * 255;
	}

}

bool stringpred(const std::string& lhs, const std::string& rhs)
{
	return std::lexicographical_compare(rhs.begin(),rhs.end(),lhs.begin(),lhs.end());
}


void ProcessImage::SetImagesPaths(Paths paths)
{
	g_curpath = paths;
}


void ProcessImage::SetExportDir(string dir)
{
	g_psddir = dir;
	g_psddir += "\\";
}

//construit l'image finale à partir de g_pictures
FIBITMAP* ProcessImage::BuildImages(FIBITMAP* input)
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

}





void ProcessImage::SetName(string name)
{
	g_psdname = name;
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

void ProcessImage::SetLuminosity(int l)
{
	g_luminosity = l;
}

void ProcessImage::SetContraste(int c)
{
	//on fonctionne en pourcentage
	g_constraste = ((double)c)/100.0f;
}

void ProcessImage::SetSaturation(int s)
{
	g_saturation = s;
}

