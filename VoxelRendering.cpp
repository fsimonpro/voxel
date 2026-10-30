// VoxelRendering.cpp : fichier projet principal.

#include "stdafx.h"
#include "Form1.h"

using namespace VoxelRendering;

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	// Activation des effets visuels de Windows XP avant la création de tout contrôle
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Créer la fenêtre principale et l'exécuter
	Application::Run(gcnew Form1());
	return 0;
}
