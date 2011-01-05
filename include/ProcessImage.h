
#pragma once

#include <vector>

using namespace std;

typedef vector<string> Paths;

struct FIBITMAP;

#define DEFAULTOUTPUTNAME "output.bmp"
#define DEFAULTOUTPUTPATH "c:\\"

namespace ProcessImage
{
	void SetImagesPaths(Paths paths);

	void SetExportDir(string dir);

	FIBITMAP* BuildImages(FIBITMAP* input);

	void SetName(string name);

	string ComputeOutput(); //calcul le fibitmap qu'il faut

	string GetPurePathOnly(string path);

	string GetCurrentImportPath();

	void ComputeVoxel();

}