
#ifndef PROCESSIMAGE_H
#define  PROCESSIMAGE_H

#include <string>
#include <vector>

using namespace std;

typedef vector<string> Paths;

struct FIBITMAP;

#define DEFAULTOUTPUTNAME "output.bmp"
#define DEFAULTOUTPUTPATH "c:\\"

struct Curview
{
	signed int posx;
	signed int posy;
	signed int posz;
	float angle;
};

namespace ProcessImage
{
	void SetImagesPaths(Paths paths);

	void SetExportDir(string dir);

	void SetName(string name);

	string ComputeOutput(); //calcul le fibitmap qu'il faut

	string GetPurePathOnly(string path);

	string GetCurrentImportPath();

	double ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform);

	void Turn(bool left);

	void Move(int dirx,int diry,int dirz = 0);

	void SetFov(int nfov);

	void SetResolution(unsigned int res);

	unsigned int  GetResolution();

	void SetFlipDistance(unsigned int n);

	const Curview* GetCurView();

}

#endif