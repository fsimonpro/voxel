#pragma once

#include "ProcessImage.h"
#include <string>
#include <iostream>

using namespace std;
using namespace System;

void MarshalString ( System::String ^ s, std::string& os ) {
   using namespace Runtime::InteropServices;
   const char* chars = 
      (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
   os = chars;
   Marshal::FreeHGlobal(IntPtr((void*)chars));
}

namespace SpriteSetMaker {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Description résumée de Form1
	///
	/// AVERTISSEMENT : si vous modifiez le nom de cette classe, vous devrez modifier la
	///          propriété 'Nom du fichier de ressources' de l'outil de compilation de ressource managée
	///          pour tous les fichiers .resx dont dépend cette classe. Dans le cas contraire,
	///          les concepteurs ne pourront pas interagir correctement avec les ressources
	///          localisées associées à ce formulaire.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO : ajoutez ici le code du constructeur
			//
			//on fait un premier rendu auto
			GenerateVoxelInForm();
		}

	protected:
		/// <summary>
		/// Nettoyage des ressources utilisées.
		/// </summary>
		~Form1()
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

	private: System::Windows::Forms::HScrollBar^  hScrollBar3;
	private: System::Windows::Forms::Label^  labelfov;
	private: System::Windows::Forms::Label^  labelRes;






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
			this->hScrollBar3 = (gcnew System::Windows::Forms::HScrollBar());
			this->labelfov = (gcnew System::Windows::Forms::Label());
			this->labelRes = (gcnew System::Windows::Forms::Label());
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
			this->label1->Text = L"Input Images";
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// InputFileButton
			// 
			this->InputFileButton->Location = System::Drawing::Point(400, 32);
			this->InputFileButton->Name = L"InputFileButton";
			this->InputFileButton->Size = System::Drawing::Size(32, 20);
			this->InputFileButton->TabIndex = 4;
			this->InputFileButton->Text = L"...";
			this->InputFileButton->UseVisualStyleBackColor = true;
			this->InputFileButton->Click += gcnew System::EventHandler(this, &Form1::InputFileButton_Click);
			// 
			// OutputNameText
			// 
			this->OutputNameText->Location = System::Drawing::Point(49, 98);
			this->OutputNameText->Name = L"OutputNameText";
			this->OutputNameText->Size = System::Drawing::Size(154, 20);
			this->OutputNameText->TabIndex = 9;
			this->OutputNameText->Text = L"output.bmp";
			this->OutputNameText->TextChanged += gcnew System::EventHandler(this, &Form1::OutputNameText_TextChanged);
			this->OutputNameText->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &Form1::OutputNameText_KeyDown);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(46, 82);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(89, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Output File Name";
			// 
			// ButtonGenerate
			// 
			this->ButtonGenerate->Location = System::Drawing::Point(252, 90);
			this->ButtonGenerate->Name = L"ButtonGenerate";
			this->ButtonGenerate->Size = System::Drawing::Size(116, 34);
			this->ButtonGenerate->TabIndex = 13;
			this->ButtonGenerate->Text = L"Generate";
			this->ButtonGenerate->UseVisualStyleBackColor = true;
			this->ButtonGenerate->Click += gcnew System::EventHandler(this, &Form1::ButtonGenerate_Click);
			// 
			// pictureBoxDest
			// 
			this->pictureBoxDest->Location = System::Drawing::Point(49, 130);
			this->pictureBoxDest->Name = L"pictureBoxDest";
			this->pictureBoxDest->Size = System::Drawing::Size(500, 500);
			this->pictureBoxDest->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBoxDest->TabIndex = 22;
			this->pictureBoxDest->TabStop = false;
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
			this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBar1_Scroll);
			// 
			// hScrollBarResolution
			// 
			this->hScrollBarResolution->LargeChange = 50;
			this->hScrollBarResolution->Location = System::Drawing::Point(581, 333);
			this->hScrollBarResolution->Maximum = 500;
			this->hScrollBarResolution->Minimum = 50;
			this->hScrollBarResolution->Name = L"hScrollBarResolution";
			this->hScrollBarResolution->Size = System::Drawing::Size(469, 19);
			this->hScrollBarResolution->SmallChange = 10;
			this->hScrollBarResolution->TabIndex = 25;
			this->hScrollBarResolution->Value = 250;
			this->hScrollBarResolution->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &Form1::hScrollBarResolution_Scroll);
			// 
			// hScrollBar3
			// 
			this->hScrollBar3->Location = System::Drawing::Point(581, 398);
			this->hScrollBar3->Name = L"hScrollBar3";
			this->hScrollBar3->Size = System::Drawing::Size(469, 19);
			this->hScrollBar3->TabIndex = 26;
			// 
			// labelfov
			// 
			this->labelfov->AutoSize = true;
			this->labelfov->Location = System::Drawing::Point(816, 238);
			this->labelfov->Name = L"labelfov";
			this->labelfov->Size = System::Drawing::Size(28, 13);
			this->labelfov->TabIndex = 27;
			this->labelfov->Text = L"FOV";
			// 
			// labelRes
			// 
			this->labelRes->AutoSize = true;
			this->labelRes->Location = System::Drawing::Point(800, 306);
			this->labelRes->Name = L"labelRes";
			this->labelRes->Size = System::Drawing::Size(57, 13);
			this->labelRes->TabIndex = 28;
			this->labelRes->Text = L"Resolution";
			this->labelRes->TextAlign = System::Drawing::ContentAlignment::TopCenter;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1059, 641);
			this->Controls->Add(this->labelRes);
			this->Controls->Add(this->labelfov);
			this->Controls->Add(this->hScrollBar3);
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
			this->Name = L"Form1";
			this->Text = L"Form1";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBoxDest))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
			 }
	private: System::Void label4_Click(System::Object^  sender, System::EventArgs^  e) {
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
	unsigned int * curdisplay;
	unsigned int curres = ProcessImage::GetResolution();
	curdisplay =static_cast<unsigned int *> (malloc(curres*curres*sizeof(unsigned int)));
	//memset(curdisplay,0xFF000000,SCREENRESOLUTION*SCREENRESOLUTION*sizeof(unsigned int));

	double length = ProcessImage::ComputeVoxel(curres,curres,curdisplay);

	this->InfoDebug->Text = (gcnew System::Double(length))->ToString();


	Bitmap^ image1 = gcnew Bitmap(curres, curres);
	int x,y;
	// Graphics flagGraphics = Graphics.FromImage(flag);
	unsigned int curcol;

	//todo penser à utiliser lockbits et unlockbits
	for ( x = 0; x < image1->Width; x++ )
	{
		for ( y = 0; y < image1->Height; y++ )
		{
			//Color pixelColor = image1->GetPixel( x, y );
			curcol = curdisplay[x + y*curres];
			Color newColor = Color::FromArgb(curcol);
			image1->SetPixel( x, image1->Height-y-1, newColor );
		}
	}

	this->pictureBoxDest->Image = image1;

	//this->pictureBoxDest->ImageLocation = "e:\\Voxel\\test.bmp";
	this->label3->Text = String::Format("Pixel format: {0}", image1->PixelFormat );
	
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

private: System::Void InputFileButton_Click(System::Object^  sender, System::EventArgs^  e) {
			  //Stream^ myStream;
			 Paths mypaths; 
			 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;

			  openFileDialog1->InitialDirectory = "c:\\";
			  openFileDialog1->Filter = "Images Files |*.tga;*.bmp; *.png; *.gif";
			  openFileDialog1->FilterIndex = 2;
			  openFileDialog1->RestoreDirectory = true;
			  openFileDialog1->Multiselect = true;
			  //pas trés pratique -> on va convertir tout en string

			 String^ display = gcnew System::String("");
			  if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK )
			  {
				  mypaths.resize(openFileDialog1->FileNames->Length);
				  for (int i =0;i<openFileDialog1->FileNames->Length;i++)
				  {
					  //MessageBox::Show(openFileDialog1->FileNames[i]);
					  string s;
					  MarshalString(openFileDialog1->FileNames[i],s);
					  mypaths[i] = s;
					  display = display + openFileDialog1->FileNames[i];
				  } 
				   //ProcessImage::SetImagesPaths(mypaths);
				   this->inputfiletext->Text = display;
			  }
			  else
			  {
				  return;
			  }

			  ProcessImage::SetImagesPaths(mypaths);

			  //on sort toujours l'image dans le même path
			    string newpath = ProcessImage::GetCurrentImportPath();
				ProcessImage::SetExportDir(newpath);

//				this->pictureBoxSrc->ImageLocation = display;

		 }



private: System::Void saturationtext_TextChanged(System::Object^  sender, System::EventArgs^  e) {
			 //this->saturationtext->Text;
		 }
private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) 
		 {
			 ProcessImage::SetFov(e->NewValue);
			 GenerateVoxelInForm();		
			 labelfov->Text = String::Format( "FOV {0}", e->NewValue );

		 }
private: System::Void hScrollBarResolution_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) 
		 {
			 ProcessImage::SetResolution(e->NewValue);
			 GenerateVoxelInForm();		
			 labelRes->Text = String::Format( "Resolution {0}", e->NewValue );
		 }
};
}


