# Voxel – SpriteSetMaker

Ancien projet C++ (janvier 2011) : moteur de rendu **voxel** logiciel de type *Comanche* / *Voxel Space*. À partir d'une carte d'altitude (*heightmap*) et d'une carte de couleur (*albedo*), il calcule une vue en perspective du terrain, sans GPU, directement dans un bitmap affiché par l'interface.

> Projet historique, conservé tel quel. L'historique Git a été importé depuis un dépôt SVN (21 révisions, 5–22 janvier 2011).

## Fonctionnalités

- Chargement d'une heightmap et d'un albedo (`.tga`, `.gif`… via [FreeImage](http://freeimage.sourceforge.net/)), avec sélection dynamique des images d'entrée.
- Rendu voxel en temps réel : déplacement (X/Y/altitude, y compris altitude négative) et rotation de la caméra.
- Réglages par barres de défilement : **FOV**, **résolution** (rendu multi-résolution) et **distance de flip**.
- Affichage du temps de calcul par image ; écriture directe dans les `BitmapData` pour de meilleures performances.
- Export du résultat en image (`output.bmp` par défaut).

## Contenu du dépôt

| Chemin | Rôle |
|---|---|
| `Form1.h`, `Form1.resx` | Interface Windows Forms (C++/CLI) |
| `SpriteSetMaker.cpp`, `AssemblyInfo.cpp`, `stdafx.*` | Point d'entrée et fichiers du projet |
| `src/ProcessImage.cpp`, `include/ProcessImage.h` | Moteur de rendu voxel (`ComputeVoxel`, `Move`, `Turn`, `SetFov`…) |
| `include/FreeImage.h`, `lib/FreeImage.lib`, `FreeImage.dll` | Bibliothèque FreeImage 3.9.3 |
| `*.tga`, `*.gif` | Cartes d'exemple (`height*` = altitude, `albedo*` = couleur : canyon, mer, versions HD) |
| `voxelweb.rar` | Archive d'une version web du rendu voxel |
| `SpriteSetMaker.sln`, `.vcproj` | Solution Visual Studio (format 2008 ou antérieur) |

## Compilation

- Windows, Visual Studio avec C++/CLI (.NET Framework). Le projet a été créé avec l'assistant d'application *Windows Forms* de Visual C++ ; ouvrir `SpriteSetMaker.sln` et laisser Visual Studio convertir le projet si besoin.
- FreeImage est fournie dans `include/`, `lib/` et `FreeImage.dll` (la DLL doit se trouver à côté de l'exécutable).

## Utilisation

1. Lancer l'application et choisir les images d'entrée (heightmap + albedo) avec le bouton **...**.
2. Ajuster FOV, résolution et distance de flip.
3. Naviguer dans le terrain, puis **Generate** pour exporter l'image.

Le dossier d'export par défaut est `c:\` (voir `DEFAULTOUTPUTPATH` dans `include/ProcessImage.h`).
