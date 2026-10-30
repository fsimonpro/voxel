#ifndef VOXELCORE_H
#define VOXELCORE_H

//moteur de rendu voxel 100 % natif (pas de /clr) : compile en code machine optimise, sans FreeImage ni .NET.

struct VoxelView
{
	int posx;
	int posy;
	int posz;
	float angle;
};

struct VoxelParams
{
	const unsigned int * texture; //une case par texel : hauteur dans l'octet de poids fort, couleur 0x00RRGGBB dessous
	int mapwidth;
	int mapheight;
	int maxheight;       //hauteur maximale presente dans la texture (0..255)
	VoxelView view;
	float fov;
	int flipdistance;
	int resolution;      //image de sortie resolution x resolution
};

//remplit out (resolution * resolution pixels 0xAARRGGBB), un lancer de rayon par colonne, colonnes reparties sur tous les coeurs
void RenderVoxel(const VoxelParams & params, unsigned int * out);

#endif
