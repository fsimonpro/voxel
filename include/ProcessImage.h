
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

	//charge la heightmap et l'albedo (meme taille obligatoire) ; false si echec, l'etat precedent est conserve
	bool SetMapPaths(string heightpath, string albedopath);

	string GetHeightmapPath();

	string GetAlbedoPath();

	void SetExportDir(string dir);

	void SetName(string name);

	string ComputeOutput(); //calcul le fibitmap qu'il faut

	string GetPurePathOnly(string path);

	string GetCurrentImportPath();

	double ComputeVoxel(int screenwidth, int screenheight , unsigned int * Outputforform);

	void Turn(bool left);

	void Move(int dirx,int diry,int dirz = 0);

	void MoveForward(int dir); //dir > 0 : avance dans la direction de la vue

	void SetView(int x,int y,int z,float angle);

	void SetFov(int nfov);

	void SetResolution(unsigned int res);

	unsigned int  GetResolution();

	void SetFlipDistance(unsigned int n);

	const Curview* GetCurView();

}

#endif