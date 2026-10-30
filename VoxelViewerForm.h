#pragma once

#include "ProcessImage.h"
#include <string>
#include <iostream>

#include <sys/types.h>  
#include <sys/timeb.h>
#include <time.h>




using namespace std;
using namespace System;

void MarshalString ( System::String ^ s, std::string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

namespace VoxelRendering {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Description résumée de VoxelViewerForm
	///
	/// AVERTISSEMENT : si vous modifiez le nom de cette classe, vous devrez modifier la
	///          propriété 'Nom du fichier de ressources' de l'outil de compilation de ressource managée
	///          pour tous les fichiers .resx dont dépend cette classe. Dans le cas contraire,
	///          les concepteurs ne pourront pas interagir correctement avec les ressources
	///          localisées associées à ce formulaire.
	/// </summary>
	public ref class VoxelViewerForm : public System::Windows::Forms::Form
	{
	public:
		VoxelViewerForm(void)
		{
			InitializeComponent();
			//
			//TODO : ajoutez ici le code du constructeur
			//
			//on fait un premier rendu auto
			m_bitmapdisplay = gcnew Bitmap(ProcessImage::GetResolution(), ProcessImage::GetResolution());
			this->pictureBoxDest->Image = m_bitmapdisplay;
			SetupExtraUI();
			GenerateVoxelInForm();
			//le focus initial ne doit pas etre dans un champ de saisie sinon les fleches y sont captees
			this->ActiveControl = checkRealTime;
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~VoxelViewerForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::TextBox^  inputfiletext;
	protected: 

	private: System::Windows::Forms::Label^  label1;


	private: System::Windows::Forms::Button^  InputFileButton;


	private: System::Windows::Forms::TextBox^  OutputNameText;


	private: System::Windows::Forms::Label^  label3;


	private: System::Windows::Forms::Button^  ButtonGenerate;







	private: System::Windows::Forms::PictureBox^  pictureBoxDest;
	private: System::Windows::Forms::Label^  InfoDebug;
	private: System::Windows::Forms::HScrollBar^  hScrollBar1;
	private: System::Windows::Forms::HScrollBar^  hScrollBarResolution;
	private: System::Windows::Forms::HScrollBar^  hScrollBarFlipDistance;


	private: System::Windows::Forms::Label^  labelfov;
	private: System::Windows::Forms::Label^  labelRes;
	private: System::Windows::Forms::Label^  labelflipdistance;
	private: System::Windows::Forms::Label^  labelposx;
	private: System::Windows::Forms::Label^  labelposy;
	private: System::Windows::Forms::Label^  labelposz;
	private: System::Windows::Forms::Label^  labelangle;



	private: Bitmap^ m_bitmapdisplay;
	private: System::Windows::Forms::CheckBox^  checkRealTime;
	private: System::Windows::Forms::NumericUpDown^  numX;
	private: System::Windows::Forms::NumericUpDown^  numY;
	private: System::Windows::Forms::NumericUpDown^  numZ;
	private: System::Windows::Forms::NumericUpDown^  numAngle;
	private: bool m_updatingui;
	private: System::Windows::Forms::Timer^  keyTimer;
	private: System::Windows::Forms::Button^  buttonSavePng;
	private: String^ m_rootdir; //dossier contenant Samples et Output
	private: System::Windows::Forms::Label^  labelAlbedo;
	private: System::Windows::Forms::TextBox^  albedoText;
	private: System::Windows::Forms::Button^  buttonAlbedo;






	private:
		/// <summary>
		/// Variable nécessaire au concepteur.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
		/// le contenu de cette méthode avec l'éditeur de code.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->inputfiletext = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->InputFileButton = (gcnew System::Windows::Forms::Button());
			this->OutputNameText = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->ButtonGenerate = (gcnew System::Windows::Forms::Button());
			this->pictureBoxDest = (gcnew System::Windows::Forms::PictureBox());
			this->InfoDebug = (gcnew System::Windows::Forms::Label());
			this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBarResolution = (gcnew System::Windows::Forms::HScrollBar());
			this->hScrollBarFlipDistance = (gcnew System::Windows::Forms::HScrollBar());
			this->labelfov = (gcnew System::Windows::Forms::Label());
			this->labelRes = (gcnew System::Windows::Forms::Label());
			this->labelflipdistance = (gcnew System::Windows::Forms::Label());
			this->labelposx = (gcnew System::Windows::Forms::Label());
			this->labelposy = (gcnew System::Windows::Forms::Label());
			this->labelposz = (gcnew System::Windows::Forms::Label());
			this->labelangle = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDest))->BeginInit();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			// 
			// inputfiletext
			// 
			this->inputfiletext->Location = System::Drawing::Point(49, 33);
			this->inputfiletext->Name = L"inputfiletext";
			this->inputfiletext->Size = System::Drawing::Size(319, 20);
			this->inputfiletext->TabIndex = 0;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(46, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(68, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Heightmap";
			this->label1->Click += gcnew System::EventHandler(this, &VoxelViewerForm::label1_Click);
			// 
			// InputFileButton
			// 
			this->InputFileButton->Location = System::Drawing::Point(400, 32);
			this->InputFileButton->Name = L"InputFileButton";
			this->InputFileButton->Size = System::Drawing::Size(32, 20);
			this->InputFileButton->TabIndex = 4;
			this->InputFileButton->Text = L"...";
			this->InputFileButton->UseVisualStyleBackColor = true;
			this->InputFileButton->Click += gcnew System::EventHandler(this, &VoxelViewerForm::InputFileButton_Click);
			// 
			// OutputNameText
			// 
			this->OutputNameText->Location = System::Drawing::Point(49, 98);
			this->OutputNameText->Name = L"OutputNameText";
			this->OutputNameText->Size = System::Drawing::Size(154, 20);
			this->OutputNameText->TabIndex = 9;
			this->OutputNameText->Text = L"output";
			this->OutputNameText->TextChanged += gcnew System::EventHandler(this, &VoxelViewerForm::OutputNameText_TextChanged);
			this->OutputNameText->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &VoxelViewerForm::OutputNameText_KeyDown);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(46, 82);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(89, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Output prefix";
			// 
			// ButtonGenerate
			// 
			this->ButtonGenerate->Location = System::Drawing::Point(252, 90);
			this->ButtonGenerate->Name = L"ButtonGenerate";
			this->ButtonGenerate->Size = System::Drawing::Size(116, 34);
			this->ButtonGenerate->TabIndex = 13;
			this->ButtonGenerate->Text = L"Generate";
			this->ButtonGenerate->UseVisualStyleBackColor = true;
			this->ButtonGenerate->Click += gcnew System::EventHandler(this, &VoxelViewerForm::ButtonGenerate_Click);
			// 
			// pictureBoxDest
			// 
			this->pictureBoxDest->BackgroundImageLayout = System::Windows::Forms::ImageLayout::None;
			this->pictureBoxDest->Location = System::Drawing::Point(49, 130);
			this->pictureBoxDest->Name = L"pictureBoxDest";
			this->pictureBoxDest->Size = System::Drawing::Size(500, 500);
			this->pictureBoxDest->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBoxDest->TabIndex = 22;
			this->pictureBoxDest->TabStop = false;
			this->pictureBoxDest->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &VoxelViewerForm::pictureBoxDest_MouseMove);
			// 
			// InfoDebug
			// 
			this->InfoDebug->AutoSize = true;
			this->InfoDebug->Location = System::Drawing::Point(417, 90);
			this->InfoDebug->Name = L"InfoDebug";
			this->InfoDebug->Size = System::Drawing::Size(89, 13);
			this->InfoDebug->TabIndex = 23;
			this->InfoDebug->Text = L"Output File Name";
			// 
			// hScrollBar1
			// 
			this->hScrollBar1->Location = System::Drawing::Point(581, 271);
			this->hScrollBar1->Maximum = 200;
			this->hScrollBar1->Minimum = 10;
			this->hScrollBar1->Name = L"hScrollBar1";
			this->hScrollBar1->Size = System::Drawing::Size(469, 19);
			this->hScrollBar1->TabIndex = 24;
			this->hScrollBar1->Value = 70;
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &VoxelViewerForm::hScrollBar1_Scroll);
			// 
			// hScrollBarResolution
			// 
			this->hScrollBarResolution->LargeChange = 50;
			this->hScrollBarResolution->Location = System::Drawing::Point(581, 333);
			this->hScrollBarResolution->Maximum = 1073; //1024 + LargeChange - 1 : valeur max atteignable = 1024
			this->hScrollBarResolution->Minimum = 50;
			this->hScrollBarResolution->Name = L"hScrollBarResolution";
			this->hScrollBarResolution->Size = System::Drawing::Size(469, 19);
			this->hScrollBarResolution->SmallChange = 10;
			this->hScrollBarResolution->TabIndex = 25;
			this->hScrollBarResolution->Value = 512;
			this->hScrollBarResolution->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &VoxelViewerForm::hScrollBarResolution_Scroll);
			// 
			// hScrollBarFlipDistance
			// 
			this->hScrollBarFlipDistance->LargeChange = 30;
			this->hScrollBarFlipDistance->Location = System::Drawing::Point(581, 398);
			this->hScrollBarFlipDistance->Maximum = 400;
			this->hScrollBarFlipDistance->Minimum = 50;
			this->hScrollBarFlipDistance->Name = L"hScrollBarFlipDistance";
			this->hScrollBarFlipDistance->Size = System::Drawing::Size(469, 19);
			this->hScrollBarFlipDistance->SmallChange = 10;
			this->hScrollBarFlipDistance->TabIndex = 26;
			this->hScrollBarFlipDistance->Value = 200;
			this->hScrollBarFlipDistance->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &VoxelViewerForm::hScrollBarFlipDistance_Scroll);
			// 
			// labelfov
			// 
			this->labelfov->AutoSize = true;
			this->labelfov->Location = System::Drawing::Point(802, 236);
			this->labelfov->Name = L"labelfov";
			this->labelfov->Size = System::Drawing::Size(28, 13);
			this->labelfov->TabIndex = 27;
			this->labelfov->Text = L"FOV";
			// 
			// labelRes
			// 
			this->labelRes->AutoSize = true;
			this->labelRes->Location = System::Drawing::Point(787, 307);
			this->labelRes->Name = L"labelRes";
			this->labelRes->Size = System::Drawing::Size(57, 13);
			this->labelRes->TabIndex = 28;
			this->labelRes->Text = L"Resolution";
			this->labelRes->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelflipdistance
			// 
			this->labelflipdistance->AutoSize = true;
			this->labelflipdistance->Location = System::Drawing::Point(787, 368);
			this->labelflipdistance->Name = L"labelflipdistance";
			this->labelflipdistance->Size = System::Drawing::Size(65, 13);
			this->labelflipdistance->TabIndex = 29;
			this->labelflipdistance->Text = L"Perspective";
			this->labelflipdistance->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// labelposx
			// 
			this->labelposx->AutoSize = true;
			this->labelposx->Location = System::Drawing::Point(664, 32);
			this->labelposx->Name = L"labelposx";
			this->labelposx->Size = System::Drawing::Size(28, 13);
			this->labelposx->TabIndex = 30;
			this->labelposx->Text = L"FOV";
			// 
			// labelposy
			// 
			this->labelposy->AutoSize = true;
			this->labelposy->Location = System::Drawing::Point(664, 55);
			this->labelposy->Name = L"labelposy";
			this->labelposy->Size = System::Drawing::Size(28, 13);
			this->labelposy->TabIndex = 31;
			this->labelposy->Text = L"FOV";
			// 
			// labelposz
			// 
			this->labelposz->AutoSize = true;
			this->labelposz->Location = System::Drawing::Point(664, 82);
			this->labelposz->Name = L"labelposz";
			this->labelposz->Size = System::Drawing::Size(28, 13);
			this->labelposz->TabIndex = 32;
			this->labelposz->Text = L"FOV";
			// 
			// labelangle
			// 
			this->labelangle->AutoSize = true;
			this->labelangle->Location = System::Drawing::Point(664, 105);
			this->labelangle->Name = L"labelangle";
			this->labelangle->Size = System::Drawing::Size(28, 13);
			this->labelangle->TabIndex = 33;
			this->labelangle->Text = L"FOV";
			// 
			// VoxelViewerForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1059, 641);
			this->Controls->Add(this->labelangle);
			this->Controls->Add(this->labelposz);
			this->Controls->Add(this->labelposy);
			this->Controls->Add(this->labelposx);
			this->Controls->Add(this->labelflipdistance);
			this->Controls->Add(this->labelRes);
			this->Controls->Add(this->labelfov);
			this->Controls->Add(this->hScrollBarFlipDistance);
			this->Controls->Add(this->hScrollBarResolution);
			this->Controls->Add(this->hScrollBar1);
			this->Controls->Add(this->InfoDebug);
			this->Controls->Add(this->pictureBoxDest);
			this->Controls->Add(this->ButtonGenerate);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->OutputNameText);
			this->Controls->Add(this->InputFileButton);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->inputfiletext);
			this->Name = L"VoxelViewerForm";
			this->Text = L"Voxel Rendering";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDest))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
private: System::Windows::Forms::NumericUpDown^ MakeNumeric(int decimals, double inc, double mn, double mx)
		 {
			 NumericUpDown^ n = gcnew NumericUpDown();
			 n->DecimalPlaces = decimals;
			 n->Increment = System::Decimal(inc);
			 n->Minimum = System::Decimal(mn);
			 n->Maximum = System::Decimal(mx);
			 n->ValueChanged += gcnew System::EventHandler(this, &VoxelViewerForm::ViewValue_Changed);
			 return n;
		 }

private: System::Void SetupExtraUI()
		 {
			 m_updatingui = false;

			 //deuxieme champ : l'albedo (le premier champ, inputfiletext, est la heightmap)
			 labelAlbedo = gcnew Label();
			 labelAlbedo->Text = L"Albedo";
			 labelAlbedo->AutoSize = true;
			 albedoText = gcnew TextBox();
			 buttonAlbedo = gcnew Button();
			 buttonAlbedo->Text = L"...";
			 buttonAlbedo->Click += gcnew System::EventHandler(this, &VoxelViewerForm::buttonAlbedo_Click);
			 this->Controls->Add(labelAlbedo);
			 this->Controls->Add(albedoText);
			 this->Controls->Add(buttonAlbedo);
			 //racine du projet : le dossier courant, celui de l'exe ou son parent, selon ou se trouve Samples
			 m_rootdir = Environment::CurrentDirectory;
			 cli::array<String^>^ candidates = { Environment::CurrentDirectory, Application::StartupPath, Path::GetDirectoryName(Application::StartupPath) };
			 for each (String^ candidate in candidates)
			 {
				 if (!String::IsNullOrEmpty(candidate) && Directory::Exists(Path::Combine(candidate, L"Samples")))
				 {
					 m_rootdir = candidate;
					 break;
				 }
			 }
			 inputfiletext->Text = Path::GetFullPath(Path::Combine(m_rootdir, gcnew String(ProcessImage::GetHeightmapPath().c_str())));
			 albedoText->Text = Path::GetFullPath(Path::Combine(m_rootdir, gcnew String(ProcessImage::GetAlbedoPath().c_str())));
			 {
				 string heightpath,albedopath;
				 MarshalString(inputfiletext->Text,heightpath);
				 MarshalString(albedoText->Text,albedopath);
				 ProcessImage::SetMapPaths(heightpath,albedopath);
			 }

			 buttonSavePng = gcnew Button();
			 buttonSavePng->Text = L"Save PNG";
			 buttonSavePng->Click += gcnew System::EventHandler(this, &VoxelViewerForm::buttonSavePng_Click);
			 this->Controls->Add(buttonSavePng);

			 keyTimer = gcnew System::Windows::Forms::Timer();
			 keyTimer->Interval = 30;
			 keyTimer->Tick += gcnew System::EventHandler(this, &VoxelViewerForm::KeyTimer_Tick);
			 keyTimer->Start();

			 checkRealTime = gcnew CheckBox();
			 checkRealTime->Text = L"Generate real time";
			 checkRealTime->AutoSize = true;
			 checkRealTime->Checked = true;
			 checkRealTime->CheckedChanged += gcnew System::EventHandler(this, &VoxelViewerForm::checkRealTime_CheckedChanged);

			 numX = MakeNumeric(0, 10, -100000, 100000);
			 numY = MakeNumeric(0, 10, -100000, 100000);
			 numZ = MakeNumeric(0, 4, -10000, 10000); //hauteur = -posz
			 numAngle = MakeNumeric(2, 0.1, -100, 100);

			 this->Controls->Add(checkRealTime);
			 this->Controls->Add(numX);
			 this->Controls->Add(numY);
			 this->Controls->Add(numZ);
			 this->Controls->Add(numAngle);

			 //les labels deviennent de simples legendes
			 labelposx->Text = L"X";
			 labelposy->Text = L"Y";
			 labelposz->Text = L"Hauteur";
			 labelangle->Text = L"Angle";
			 labelfov->Text = String::Format( "FOV {0}", hScrollBar1->Value );
			 labelRes->Text = String::Format( "Resolution {0}", hScrollBarResolution->Value );
			 labelflipdistance->Text = String::Format( "Perspective {0}", hScrollBarFlipDistance->Value );

			 labelfov->AutoSize = false;
			 labelRes->AutoSize = false;
			 labelflipdistance->AutoSize = false;
			 labelfov->TextAlign = ContentAlignment::TopCenter;
			 labelRes->TextAlign = ContentAlignment::TopCenter;
			 labelflipdistance->TextAlign = ContentAlignment::TopCenter;

			 pictureBoxDest->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &VoxelViewerForm::pictureBoxDest_MouseDown);
			 this->MinimumSize = System::Drawing::Size(640, 480);
			 this->Resize += gcnew System::EventHandler(this, &VoxelViewerForm::VoxelViewerForm_Resize);
			 ApplyLayout();
		 }

private: System::Void ApplyLayout()
		 {
			 const int m = 12;
			 const int top = 140;
			 int cw = this->ClientSize.Width;
			 int ch = this->ClientSize.Height;

			 //zone image : carree, a gauche
			 int side = Math::Min((int)(cw * 0.55) - m, ch - top - m);
			 if (side < 150) side = 150;
			 pictureBoxDest->SetBounds(m, top, side, side);

			 label1->Location = System::Drawing::Point(m, 4);
			 inputfiletext->SetBounds(m, 18, Math::Max(100, side - 40), 20);
			 InputFileButton->SetBounds(m + side - 32, 17, 32, 22);

			 labelAlbedo->Location = System::Drawing::Point(m, 44);
			 albedoText->SetBounds(m, 58, Math::Max(100, side - 40), 20);
			 buttonAlbedo->SetBounds(m + side - 32, 57, 32, 22);

			 label3->Location = System::Drawing::Point(m, 84);
			 OutputNameText->SetBounds(m, 100, 154, 20);
			 ButtonGenerate->SetBounds(m + 170, 92, 116, 34);
			 buttonSavePng->SetBounds(m + 296, 92, 100, 34);
			 InfoDebug->Location = System::Drawing::Point(m + 410, 102);

			 //panneau de droite
			 int px = m + side + 20;
			 int pw = Math::Max(160, cw - px - m);

			 checkRealTime->Location = System::Drawing::Point(px, 24);

			 int cell = pw / 4;
			 cli::array<Label^>^ labs = { labelposx, labelposy, labelposz, labelangle };
			 cli::array<NumericUpDown^>^ nums = { numX, numY, numZ, numAngle };
			 for (int i = 0; i < 4; i++)
			 {
				 labs[i]->Location = System::Drawing::Point(px + i * cell, 62);
				 nums[i]->SetBounds(px + i * cell, 78, cell - 8, 20);
			 }

			 labelfov->SetBounds(px, 140, pw, 16);
			 hScrollBar1->SetBounds(px, 160, pw, 19);
			 labelRes->SetBounds(px, 200, pw, 16);
			 hScrollBarResolution->SetBounds(px, 220, pw, 19);
			 labelflipdistance->SetBounds(px, 260, pw, 16);
			 hScrollBarFlipDistance->SetBounds(px, 280, pw, 19);
		 }

private: System::Void VoxelViewerForm_Resize(System::Object^  sender, System::EventArgs^  e)
		 {
			 ApplyLayout();
		 }

//rend si le mode temps reel est actif
private: System::Void RenderIfRealTime()
		 {
			 if (checkRealTime->Checked)
				 GenerateVoxelInForm();
		 }

//sauvegarde l'image affichee en PNG : <nom>_aaaaMMdd_HHmmss.png dans le dossier Output (jamais d'ecrasement)
private: System::Void buttonSavePng_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 String^ basename = Path::GetFileNameWithoutExtension(OutputNameText->Text);
			 if (String::IsNullOrEmpty(basename))
				 basename = L"output";
			 String^ filename = String::Format("{0}_{1}.png", basename, DateTime::Now.ToString("yyyyMMdd_HHmmss"));
			 String^ outputdir = Path::Combine(m_rootdir, L"Output");
			 String^ fullpath = Path::Combine(outputdir, filename);
			 try
			 {
				 Directory::CreateDirectory(outputdir);
				 m_bitmapdisplay->Save(fullpath, System::Drawing::Imaging::ImageFormat::Png);
				 InfoDebug->Text = String::Format("sauvegarde : Output\{0}", filename);
			 }
			 catch (Exception^ ex)
			 {
				 MessageBox::Show(ex->Message, L"Save PNG");
			 }
		 }

private: System::Void checkRealTime_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
		 {
			 ButtonGenerate->Visible = !checkRealTime->Checked;
			 if (checkRealTime->Checked)
				 GenerateVoxelInForm();
		 }

private: System::Void ViewValue_Changed(System::Object^  sender, System::EventArgs^  e)
		 {
			 if (m_updatingui)
				 return;
			 ProcessImage::SetView( (int)numX->Value, (int)numY->Value, -(int)numZ->Value, (float)numAngle->Value );
			 RenderIfRealTime();
		 }

private: System::Void SyncViewControls()
		 {
			 const Curview* curview = ProcessImage::GetCurView();
			 m_updatingui = true;
			 numX->Value = Math::Max(numX->Minimum, Math::Min(numX->Maximum, System::Decimal(curview->posx)));
			 numY->Value = Math::Max(numY->Minimum, Math::Min(numY->Maximum, System::Decimal(curview->posy)));
			 numZ->Value = Math::Max(numZ->Minimum, Math::Min(numZ->Maximum, System::Decimal(-curview->posz)));
			 numAngle->Value = Math::Max(numAngle->Minimum, Math::Min(numAngle->Maximum, System::Decimal(curview->angle)));
			 m_updatingui = false;
		 }

private: System::Void ButtonGenerate_Click(System::Object^  sender, System::EventArgs^  e) 
		 {
			 //on récupére les valeurs
/*
			int newlum = System::Int32::Parse((this->lumintext->Text));
			ProcessImage::SetLuminosity(newlum);
			int newcontrast = System::Int32::Parse((this->contrasttext->Text));
			ProcessImage::SetContraste(newcontrast);
			int newsat = System::Int32::Parse((this->saturationtext->Text));
			ProcessImage::SetSaturation(newsat);
			string outputpath = ProcessImage::ComputeOutput();*/

			//if (outputpath != 0)
			GenerateVoxelInForm();
	

		 }

private: System::Void OutputNameText_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			string s;
			MarshalString(this->OutputNameText->Text,s);
			ProcessImage::SetName(s);
		 }

private: System::Void GenerateVoxelInForm()
{
	//Timer^ mytimer = gcnew Timer;
	struct _timeb starttime,endtime,middletime;

	_ftime_s(&starttime);
	unsigned int * curdisplay;
	unsigned int curres = ProcessImage::GetResolution();

	Rectangle rect = Rectangle(0,0,m_bitmapdisplay->Width,m_bitmapdisplay->Height);

	//on lock les bits et on passe directement en paramétre le pointeur au compute voxel
	System::Drawing::Imaging::BitmapData^ bmpData = m_bitmapdisplay->LockBits( rect, System::Drawing::Imaging::ImageLockMode::ReadWrite, m_bitmapdisplay->PixelFormat );
	IntPtr ptr = bmpData->Scan0;
	unsigned int * realptr = (unsigned int*)ptr.ToPointer();

	double length = ProcessImage::ComputeVoxel(curres,curres,realptr);

	int x,y;

	unsigned int curcol;

	m_bitmapdisplay->UnlockBits(bmpData);

	this->pictureBoxDest->Image = m_bitmapdisplay;


	const Curview* curview = ProcessImage::GetCurView();

	SyncViewControls();

	_ftime_s(&endtime);
	double res = (endtime.time - starttime.time) * 1000 +  (endtime.millitm - starttime.millitm);

	InfoDebug->Text = String::Format( "temps rendu : {0} ms ", res );


	
}

[System::Runtime::InteropServices::DllImport("user32.dll")]
static short GetAsyncKeyState(int vKey);

//vrai si on est en train d'editer un champ (les fleches y servent deja)
private: bool IsEditingField()
{
	Control^ active = this->ActiveControl;
	return dynamic_cast<NumericUpDown^>(active) != nullptr || dynamic_cast<TextBox^>(active) != nullptr;
}

private: static bool IsKeyDown(Keys key)
{
	return (GetAsyncKeyState((int)key) & 0x8000) != 0;
}

//les deplacements sont lus par le timer (pas de delai de repetition clavier) ;
//ici on empeche seulement les fleches de deplacer le focus
protected: virtual bool ProcessCmdKey(Message% msg, Keys keyData) override
{
	if (!IsEditingField())
	{
		switch (keyData)
		{
		case Keys::Up: case Keys::Down: case Keys::Left: case Keys::Right:
		case Keys::PageUp: case Keys::PageDown:
			return true;
		default: break;
		}
	}
	return Form::ProcessCmdKey(msg, keyData);
}

private: System::Void KeyTimer_Tick(System::Object^  sender, System::EventArgs^  e)
{
	if (Form::ActiveForm != this || IsEditingField())
		return;
	bool moved = false;
	if (IsKeyDown(Keys::Up))       { ProcessImage::MoveForward(1);  moved = true; }
	if (IsKeyDown(Keys::Down))     { ProcessImage::MoveForward(-1); moved = true; }
	if (IsKeyDown(Keys::Left))     { ProcessImage::Turn(false);     moved = true; }
	if (IsKeyDown(Keys::Right))    { ProcessImage::Turn(true);      moved = true; }
	if (IsKeyDown(Keys::PageUp))   { ProcessImage::Move(0,0,-1);    moved = true; }
	if (IsKeyDown(Keys::PageDown)) { ProcessImage::Move(0,0,1);     moved = true; }
	if (moved)
		GenerateVoxelInForm();
}

private: System::Void OutputNameText_KeyDown( Object^ /*sender*/, System::Windows::Forms::KeyEventArgs^ e)
{
	 Keys mykey = e->KeyCode;
	 if (mykey == Keys::D)
		 ProcessImage::Turn(false);
	 else if (mykey == Keys::S)
		 ProcessImage::Turn(true);
	 else if (mykey == Keys::T)
		 ProcessImage::Move(0,1);
	 else if (mykey == Keys::G)
		 ProcessImage::Move(0,-1);
	 else if (mykey == Keys::H)
		 ProcessImage::Move(1,0);
	 else if (mykey == Keys::F)
		 ProcessImage::Move(-1,0);
	 else if (mykey == Keys::A)
		 ProcessImage::Move(0,0,1);
	 else if (mykey == Keys::Q)
		 ProcessImage::Move(0,0,-1);
	
	 GenerateVoxelInForm();
}

//charge la heightmap et l'albedo des deux champs ; false (et message) si echec
private: bool ApplyMapFields()
{
	string heightpath,albedopath;
	MarshalString(inputfiletext->Text,heightpath);
	MarshalString(albedoText->Text,albedopath);
	if (!ProcessImage::SetMapPaths(heightpath,albedopath))
	{
		MessageBox::Show(L"Impossible de charger les images : verifie que les fichiers existent et sont lisibles.", L"Heightmap / Albedo");
		return false;
	}
	GenerateVoxelInForm();
	return true;
}

private: System::Void BrowseMap(TextBox^ target)
{
	OpenFileDialog^ dlg = gcnew OpenFileDialog();
	dlg->Filter = "Images Files|*.tga;*.bmp;*.png;*.gif|All files|*.*";
	dlg->RestoreDirectory = true;
	if (dlg->ShowDialog() != System::Windows::Forms::DialogResult::OK)
		return;
	String^ previous = target->Text;
	target->Text = dlg->FileName;
	if (!ApplyMapFields())
		target->Text = previous;
}

private: System::Void InputFileButton_Click(System::Object^  sender, System::EventArgs^  e) {
			 BrowseMap(inputfiletext);
		 }

private: System::Void buttonAlbedo_Click(System::Object^  sender, System::EventArgs^  e) {
			 BrowseMap(albedoText);
		 }

private: System::Void saturationtext_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //this->saturationtext->Text;
		 }
private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) 
		 {
			 ProcessImage::SetFov(e->NewValue);
			 RenderIfRealTime();		
			 labelfov->Text = String::Format( "FOV {0}", e->NewValue );

		 }
private: System::Void hScrollBarResolution_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) 
		 {
			 ProcessImage::SetResolution(e->NewValue);
			 //on met à jour le bitmap associé
			 this->m_bitmapdisplay = gcnew Bitmap(ProcessImage::GetResolution(), ProcessImage::GetResolution());
			this->pictureBoxDest->Image = m_bitmapdisplay;
			 RenderIfRealTime();
			 labelRes->Text = String::Format( "Resolution {0}", e->NewValue );
		 }
private: System::Void hScrollBarFlipDistance_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) 
		 {
			 ProcessImage::SetFlipDistance(e->NewValue);
			 RenderIfRealTime();		
			 labelflipdistance->Text = String::Format( "Perspective {0}", e->NewValue );		 
		 }
private: System::Void pictureBoxDest_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
		 {
			 //un clic sur l'image rend le focus aux touches de deplacement
			 this->ActiveControl = checkRealTime;
		 }
private: System::Void pictureBoxDest_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) 
		 {

		 }
};
}


