
#pragma once

#include <vector>

using namespace std;

typedef vector<string> Paths;

struct FIBITMAP;

#define DEFAULTOUTPUTNAME "output.bmp"
#define DEFAULTOUTPUTPATH "c:\\"

#define SCREENSIZE 250

namespace ProcessImage
{
	void SetImagesPaths(Paths paths);

	void SetExportDir(string dir);

	FIBITMAP* BuildImages(FIBITMAP* input);

	void SetName(string name);

	string ComputeOutput(); //calcul le fibitmap qu'il faut

	string GetPurePathOnly(string path);

	string GetCurrentImportPath();

	void ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform);

	void Turn(bool left);

	void Move(int dirx,int diry,int dirz = 0);

}