//---------------------------------------------------------------------------
#define WIN32_LEAN_AND_MEAN
#include <vcl.h>
#include <stdio.h>
#include <iostream>  
#include <tlhelp32.h>
#include <string>
#include <fstream>     
#include <iostream>
#include <Windows.h>
#include <setupapi.h>
#include <ShellApi.h> //delete folder Blockly@rduino

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

using namespace std;  //pour les traductions
#include <tinyxml.h>

#include <IniFiles.hpp> //pour le fichier INI
#include <registry.hpp>

#include "blockly_arduino_COMM.h"
#include "popup_port_COM.h"    
#include "popup_proxy.h"
#include "splashScreenUpdate.h"
#include "splashScreenWelcome.h"
#include "DetectCard.h"
#include "DetectBrowser.h"

#pragma hdrstop
#pragma package(smart_init)
#pragma resource "*.dfm"

TInterfaceS2A *InterfaceS2A;

SHELLEXECUTEINFO ShExecRDuino;
SHELLEXECUTEINFO ShExecPymata;
SHELLEXECUTEINFO ShExecNwjs;
SHELLEXECUTEINFO ShExecUpdate;

//listes pour récupérer le contenu du fichier INI
int choix_langue = 0;
AnsiString locate_doc;
AnsiString locate_Arduino;
AnsiString locate_scripts;  
AnsiString URL_to_launch;
AnsiString arg="";
//listes pour les menus dynamiques
TStringList *Aides = new TStringList;
TStringList *Docs = new TStringList;
TStringList *Extensions = new TStringList;
//thread des autodtections
TestCarte *ThreadTestArduino = new TestCarte(CREATE_SUSPENDED);
BrowserDetect *ThreadDetectBrowserClose = new BrowserDetect(CREATE_SUSPENDED);

bool voirDOS = false;
bool NewVersion = false;

//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::GetOSversion(TObject *Sender)
{
OSVERSIONINFO osvi;
BOOL bIsWindowsXPLater;
BOOL bIsWindowsXP;

ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

GetVersionEx(&osvi);

if (osvi.dwMajorVersion > 5)
	{
	locate_scripts = "tools\\python";
	INI->WriteString("Windows_version", "version", " >>XP");
	}
	else if ((osvi.dwMajorVersion == 5) && (osvi.dwMinorVersion >= 1))
		{
		locate_scripts = "tools\\python_XP";
		INI->WriteString("Windows_version", "version", "=XP");
		}
		else {
            locate_scripts = " ";
			INI->WriteString("Windows_version", "version", "incompatible");
			ShowMessage(Popup->Items->Strings[8]);
		}
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::InitINI()
{
  //stop process
  TerminateProcess(ShExecRDuino.hProcess, 0);
  TerminateProcess(ShExecPymata.hProcess, 0);
  TerminateProcess(ShExecNwjs.hProcess, 0);
  MenuConnect->Visible = false;
  //écriture dans le fichier INI des variables utiles
  INI->WriteInteger("langue", "langue", 0); //dans le fichier labels.xml, à la balise <Langues>, cela correspond au n° du rang de la langue
  //dossiers à utiliser pour les documents
  INI->WriteString("locate Documentation", "locate_doc", ExtractFilePath(Application->ExeName) + "documentation\\"); //chemin par défaut pour initialiser
  INI->WriteInteger("port COM", "port", 0);      //port par défaut pour initialiser
  INI->WriteBool("voir_scripts", "fenetre_DOS", false);
  INI->WriteBool("AutoDetect", "auto detect Arduino", true);
  INI->WriteString("URL", "URL", "");
  INI->WriteString("executable + URL", "arg_URL", ""); 
  INI->WriteString("proxy", "IP", "");
  INI->WriteInteger("proxy", "port", NULL);
  INI->WriteString("proxy", "user", "");
  INI->WriteString("proxy", "password", "");
  GetOSversion(0);
  Sleep(500);
  BrowserRelaunch(0);
}
//---------------------------------------------------------------------------
__fastcall TInterfaceS2A::TInterfaceS2A(TComponent* Owner)
	: TForm(Owner)
{
  SplashScreen = new TSplashScreen(NULL); // Crée la fiche dynamiquement
  POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
  SplashScreen->Left = (Ecran.x)/2 - (SplashScreen->Width)/2;
  SplashScreen->Top = (Ecran.y)/2 - (SplashScreen->Height)/2;
  SplashScreen->Show(); // Affiche la fiche
  SplashScreen->Repaint();

  //appel à la procédure pour gérer les langues
  AnsiString fileLanguage = ExtractFilePath(Application->ExeName) + "tools\\labels.xml";
  langue = new GestionLangue;
  langue->Init(InterfaceS2A->Langue1,fileLanguage.c_str(),(ptrOnClick)&Langue1Click);

  UpdateLog = new TIniFile(ExtractFilePath(Application->ExeName)+ "tools\\update.log");
  INI = new TIniFile(ExtractFilePath(Application->ExeName)+ "blocklyArduino.ini");
  choix_langue=INI->ReadInteger("langue", "langue", 0); 
  //après l'initialisation des langues, le système pioche la langue précédemment sélectionnée
  langue->Change(choix_langue);

  MenuConnect->Visible = false;
  MenuUnconnect->Visible = true;
  MenuConnect_NumCOM = Popup->Items->Strings[10];

  //vérification de l'existence du fichier INI, sinon le recréé
  if (!FileExists(ExtractFilePath(Application->ExeName)+ "blocklyArduino.ini"))
	 InitINI();

  //lecture du fichier INI
  locate_doc=INI->ReadString("locate Documentation", "locate_doc", ExtractFilePath(Application->ExeName) + "documentation\\");
  //lit si voirDOS ==true dans INI puis change le check du menu
  voirDOS = INI->ReadBool("voir_scripts", "fenetre_DOS", false);
  if (voirDOS) s2aio_window->Checked = true;
	else s2aio_window->Checked = false;

  auto_detect = INI->ReadBool("AutoDetect", "auto detect Arduino", true);
  if (auto_detect == false) {
		Redtecter->Checked = false;
		ThreadTestArduino->Suspend();
		upload_on->Visible = false;
		supervision_on->Visible = false;
		MenuUnconnect->Visible = true;
		MenuConnect->Visible = false;
		INI->WriteInteger("port COM", "port", 0);
		MenuUnconnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " ?";
		} else {
			Redtecter->Checked = true;
			ThreadTestArduino->Resume();
			}

  // Appel à Search, avec pour liste des fichiers, les items des listes Biblio & Examples.
  Extensions->Add(".pdf");
  Extensions->Add(".odt");
  Extensions->Add(".lnk");
  Extensions->Add(".url");
  Extensions->Add(".html");
  Extensions->Add(".htm");
  Extensions->Add(".flv");
  Extensions->Add(".mp4");
  Extensions->Add(".avi");
  // Appel à SearchDocs, avec pour liste des fichiers les items de Documentation.
  SearchDocs(locate_doc, Extensions, Docs);
  delete Extensions;

  //vérification d'argument sur la ligne de commande
  arg=GetCommandLine();
  arg=arg.Delete(1,15);  //élimine les 14 premiers caractères : blocklyArduino_AIO_
  if(arg=="-i") InitINI();
  GetOSversion(0); 
  //initialisation quasi auto
  if ((!auto_detect)&&(INI->ReadInteger("port COM", "port", 0)!=0)) rDuino();

  INI_Version_new = new TIniFile(ExtractFilePath(Application->ExeName)+ "tools\\version.txt");
  INI_Version_old = new TIniFile(ExtractFilePath(Application->ExeName)+ "wwwBlockly@rduino\\core_BlocklyArduino\\version.txt");
  INI_Version_new->WriteString("release", "version", INI_Version_old->ReadString("release", "version", ""));
  INI_Version_new->WriteInteger("update", "date", INI_Version_old->ReadInteger("update", "date", 0));

  if (CompareVersion()==true) {
	updateMenu->Checked = true;
	UpdateLog->WriteBool("NewVersion", "detected", true);
	}
	else {
		updateMenu->Checked = false;
		UpdateLog->WriteBool("NewVersion", "detected", false);
		}
  //launch Light & open Blockly@rduino
  BrowserRelaunch(0);
  ThreadDetectBrowserClose->Resume();
  Sleep(5000);
  delete SplashScreen;
}

//-------------------------relance le navigateur---------------

void __fastcall TInterfaceS2A::BrowserLaunchClick(TObject *Sender)
{
BrowserRelaunch(0);
}

//-------------------------relance le navigateur---------------

void __fastcall TInterfaceS2A::BrowserRelaunch(TObject *Sender)
{
  ThreadDetectBrowserClose->Suspend(); //do not close Interface while restarting
  SendMessage(ShExecNwjs.hProcess, WM_CLOSE | WM_DESTROY, 0, 0);
  Sleep(1000);
  TerminateProcess(ShExecNwjs.hProcess, 1);
  choix_langue=INI->ReadInteger("langue", "langue", 0);
  //launch browser, put in Panel1 & open Blockly@rduino
  URL_to_launch = "wwwBlockly@rduino\\index.html?lang=";
  if (choix_langue == 0) URL_to_launch += "fr&AIO=on&webaccess=false&localcodebender=true";
	else if (choix_langue == 1) URL_to_launch += "en&AIO=on&webaccess=false&localcodebender=true";
		else if (choix_langue == 2) URL_to_launch += "es&AIO=on&webaccess=false&localcodebender=true";

  //test if user created specific URL in INI file
  AnsiString URL_to_use = INI->ReadString("URL", "URL", "");
  if (URL_to_use != "") URL_to_launch = URL_to_use;
  AnsiString LigneDeCommande = "tools\\light\\BlocklyArduino_off.exe ";
  INI->WriteString("executable + URL", "arg_URL", LigneDeCommande + URL_to_launch);
  ShExecNwjs.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecNwjs.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecNwjs.hwnd = NULL;
			ShExecNwjs.lpVerb = NULL;
			ShExecNwjs.lpFile = LigneDeCommande.c_str();
			ShExecNwjs.lpParameters = URL_to_launch.c_str();
			ShExecNwjs.nShow = SW_SHOW;
			ShExecNwjs.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
			ShExecNwjs.hInstApp = NULL;
  ShellExecuteEx(&ShExecNwjs);
  PID_nwjs = GetProcessId(ShExecNwjs.hProcess);
  Sleep(500);
  ThreadDetectBrowserClose->Resume();
}

//-------------------------recherche des fichiers de docs pour les lister dans les menus Aide & Documentation---------------

void __fastcall TInterfaceS2A::SearchDocs(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers)
{
  TSearchRec Infos_fichier;
  TMenuItem *NewItem = new TMenuItem(this);
  if (!FindFirst(FilePath+"*.*", faAnyFile, Infos_fichier))
  do
  {
	for (int i = 0 ; i < Extensions->Count ; i++)
	{
	  if (ExtractFileExt(Infos_fichier.Name).LowerCase() == Extensions->Strings[i].LowerCase())
	  {
		//ajout dans la liste d'une ligne avec le chemin+nom du fichier trouvé
		ListeFichiers->Add(ExpandFileName(Infos_fichier.Name));
		//nettoie le nom de fichier de son extension pour le nom seul du fichier, sans le chemin
		AnsiString CleanName = StringReplace(Infos_fichier.Name, Extensions->Strings[i].LowerCase(), "",TReplaceFlags() << rfReplaceAll);
		//rajout d'entrées dans le menu
		NewItem = new TMenuItem(this);
		NewItem->Caption = Infos_fichier.Name;
		NewItem->Name = CleanName;
		//stockage du chemin complet + nom de fichier pour la fonction ExempleClick
		NewItem->Hint = FilePath + Infos_fichier.Name.c_str();
		NewItem->OnClick=DocsClick;
		//rajout au bon endroit de l'entrée dans le menu Documentation
		//MainMenu1->Items->Items[RangMenu]->Add(NewItem);
		Documentation1->Add(NewItem);
	  }
	}
  }
  while(!FindNext(Infos_fichier));
  FindClose(Infos_fichier);
}
//---------------------associer chaque entrée dynamique dans Biblio à leur lancement-------------------
void __fastcall TInterfaceS2A::DocsClick(TObject *Sender)
{
//récupération des infos de l'entrée TMenu qui a déclenché cette fonction
AnsiString CheminNomFichier=static_cast<TMenuItem*>(Sender)->Hint;

ShellExecute(0, 0, CheminNomFichier.c_str(), 0, 0 , SW_HIDE );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::A_proposClick(TObject *Sender)
{
ShellExecute(0, 0, "https://github.com/technologiescollege/BlocklyArduino_AIO", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::Siteofficieldes2a1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://mryslab.blogspot.fr/", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::SiteofficielderDuinoUploaderClick(TObject *Sender)
{
ShellExecute(0, 0, "https://github.com/nbremond77", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::PrparerArduino1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\aide\\1.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::Utilisation1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\aide\\2.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::N4Principesdeprogrammation1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\aide\\3.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::N3Mmentodescommandes1Click(TObject *Sender)
{
ShellExecute(0, 0, ".\\aide\\4.pdf", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::Wiki1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://info.technologiescollege.fr/wiki", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------  

void __fastcall TInterfaceS2A::Forum1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://blockly.technologiescollege.fr/forum/", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::rDuino()
{
TerminateProcess(ShExecRDuino.hProcess, 0);
port = StrToInt(INI->ReadString("port COM", "port", port));
voirDOS = INI->ReadBool("voir_scripts", "fenetre_DOS", false);
locate_Arduino = ExtractFilePath(Application->ExeName) + "ArduinoTechnoEduc\\";
//AnsiString locate_Arduino_debug = StringReplace(locate_Arduino, "arduino.exe", "arduino_debug.exe", TReplaceFlags() << rfReplaceAll);
if (port==0)
	{
	ShowMessage(Popup->Items->Strings[5] + "\n" + Popup->Items->Strings[6] + "\n" + Popup->Items->Strings[7]);
	}
	else {
	ofstream fichier_s2a(".\\tools\\rDuino_uploader.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
			if(fichier_s2a)
			{
				fichier_s2a << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%" << locate_scripts.c_str() << "\ncls\npython.exe ..\\rDuinoUploader\\rDuino_uploader_server.py -D COM" << port << " -U arduino_debug.exe -C arduino.exe -T " << locate_Arduino.c_str();
				fichier_s2a.close();
			}
			else ShowMessage(Popup->Items->Strings[2]);

	AnsiString ligne_cmd = locate_scripts + "\\python.exe ";
	AnsiString ligne_cmd_arg = "tools\\rDuinoUploader\\rDuino_uploader_server.py -D COM" + IntToStr(port) + " -U arduino_debug.exe -C arduino.exe -T " + locate_Arduino;
	ShExecRDuino.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecRDuino.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecRDuino.hwnd = NULL;
			ShExecRDuino.lpVerb = NULL;
			ShExecRDuino.lpFile = ligne_cmd.c_str();
			ShExecRDuino.lpParameters = ligne_cmd_arg.c_str();
			ShExecRDuino.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
			if (voirDOS) ShExecRDuino.nShow = SW_SHOW;
			if (!voirDOS) ShExecRDuino.nShow = SW_HIDE;
			ShExecRDuino.hInstApp = NULL;
	ShellExecuteEx(&ShExecRDuino);
	PID_rduino = GetProcessId(ShExecRDuino.hProcess);
	}
}
//--------------------------------------------------------------------------- 

void __fastcall TInterfaceS2A::rDuinoClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
voirDOS = INI->ReadBool("voir_scripts", "fenetre_DOS", false);
locate_Arduino = ExtractFilePath(Application->ExeName) + "ArduinoTechnoEduc\\";
//AnsiString locate_Arduino_debug = StringReplace(locate_Arduino, "arduino.exe", "arduino_debug.exe", TReplaceFlags() << rfReplaceAll);
if (port==0)
	{
	ShowMessage(Popup->Items->Strings[5] + "\n" + Popup->Items->Strings[6] + "\n" + Popup->Items->Strings[7]);
	}
	else {
	ofstream fichier_s2a(".\\tools\\rDuino_uploader.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
			if(fichier_s2a)
			{
				fichier_s2a << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%" << locate_scripts.c_str() << "\ncls\npython.exe ..\\rDuinoUploader\\rDuino_uploader_server.py -D COM" << port << " -U arduino_debug.exe -C arduino.exe -T " << locate_Arduino.c_str();
				fichier_s2a.close();
			}
			else ShowMessage(Popup->Items->Strings[2]);

	AnsiString ligne_cmd = locate_scripts + "\\python.exe ";
	AnsiString ligne_cmd_arg = "tools\\rDuinoUploader\\rDuino_uploader_server.py -D COM" + IntToStr(port) + " -U arduino_debug.exe -C arduino.exe -T " + locate_Arduino;
	ShExecRDuino.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecRDuino.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecRDuino.hwnd = NULL;
			ShExecRDuino.lpVerb = NULL;
			ShExecRDuino.lpFile = ligne_cmd.c_str();
			ShExecRDuino.lpParameters = ligne_cmd_arg.c_str();
			ShExecRDuino.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
			if (voirDOS) ShExecRDuino.nShow = SW_SHOW;
			if (!voirDOS) ShExecRDuino.nShow = SW_HIDE;
			ShExecRDuino.hInstApp = NULL;
	ShellExecuteEx(&ShExecRDuino);
	PID_rduino = GetProcessId(ShExecRDuino.hProcess);
	}
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::StopProcess()
{
TerminateProcess(ShExecRDuino.hProcess, 0);
TerminateProcess(ShExecPymata.hProcess, 0);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Apropos1Click(TObject *Sender)
{
ShowMessage(Popup->Items->Strings[4]);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Gestionnairedepriphriques1Click(TObject *Sender)
{
ShellExecute(0, 0, "devmgmt.msc", 0, 0 , SW_HIDE );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Langue1Click(TObject *Sender)
{
INI->WriteInteger("langue", "langue", ((TMenuItem*)Sender)->Tag);
langue->Change(((TMenuItem*)Sender)->Tag);
choix_langue = ((TMenuItem*)Sender)->Tag;
BrowserRelaunch(0);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::InitClick(TObject *Sender)
{
DeleteFile("blocklyArduino.ini");
InitINI();
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::SiteofficielArduino1Click(TObject *Sender)
{
ShellExecute(0, 0, "http://arduino.cc/en/Main/Software", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Localiserledossierdocumentation1Click(TObject *Sender)
{
if (OpenDialog->Execute()==true) locate_doc=ExtractFilePath(OpenDialog->FileName);
//écriture dans le fichier INI
INI->WriteString("locate Documentation", "locate_doc", locate_doc);
}
//---------------------------------------------------------------------------
void __fastcall TInterfaceS2A::Firmata_UnoClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_uno.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega328p -carduino -P\\" << "\\" << ".\\COM" << port << " -b115200 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Uno.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_uno.bat", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Firmata_MegaClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_mega.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega2560 -cwiring -P\\" << "\\" << ".\\COM" << port << " -b115200 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Mega.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_mega.bat", 0, 0 , SW_SHOW );
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Firmata_LeonardoClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_leonardo.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega32u4 -cavr109 -P\\" << "\\" << ".\\COM" << port << " -b57600 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Leonardo.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_leonardo.bat", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Firmata_MicroClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_micro.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega32u4 -cavr109 -P\\" << "\\" << ".\\COM" << port << " -b57600 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Micro.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_micro.bat", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Firmata_NanoClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_nano.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega328p -carduino -P\\" << "\\" << ".\\COM" << port << " -b57600 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Nano.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_nano.bat", 0, 0 , SW_SHOW );
}
//--------------------------------------------------------------------------- 

void __fastcall TInterfaceS2A::DuemilanoveDiecimila328P1Click(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
ofstream fichier_s2(".\\tools\\flash_duemilanove.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
		if(fichier_s2)
		{
				fichier_s2 << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%tools\ncls\n..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\bin\\avrdude -C..\\ArduinoTechnoEduc\\hardware\\tools\\avr\\etc\\avrdude.conf -v -patmega328p -carduino -P\\" << "\\" << ".\\COM" << port << " -b57600 -D -V -Uflash:w:PyMata-aio-FirmataPlus.Duemilanove.ino.hex:i" << "\npause";
				fichier_s2.close();
		}
		else ShowMessage(Popup->Items->Strings[1]);
ShellExecute(0, 0, ".\\tools\\flash_duemilanove.bat", 0, 0 , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::w32Click(TObject *Sender)
{
ShellExecute(0, 0, "dpinst-x86.exe", 0, (ExtractFilePath(Application->ExeName) + "ArduinoTechnoEduc\\drivers").c_str() , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::w64Click(TObject *Sender)
{
ShellExecute(0, 0, "dpinst-amd64.exe", 0, (ExtractFilePath(Application->ExeName) + "ArduinoTechnoEduc\\drivers").c_str() , SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::DriversClick(TObject *Sender)
{
ShellExecute(0, 0, "explorer.exe", (ExtractFilePath(Application->ExeName) + "ArduinoTechnoEduc\\drivers").c_str(), 0, SW_SHOW );
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::pymataClick(TObject *Sender)
{
port = StrToInt(INI->ReadString("port COM", "port", port));
if (port == 0)
	{
	ShowMessage(Popup->Items->Strings[5] + "\n" + Popup->Items->Strings[6] + "\n" + Popup->Items->Strings[7]);
	}
else {
	ofstream fichier_s2a(".\\tools\\pymata_cmd.bat", ios::out | ios::trunc);  // ouverture en écriture avec effacement du fichier ouvert
			if(fichier_s2a)
			{
				fichier_s2a << "@echo off\nbreak ON\nrem fichiers BAT et fork créés par Sébastien CANET\nSET currentpath=%~dp1\ncd %currentpath%" << locate_scripts.c_str() << "\ncls\npython.exe .\\Lib\\site-packages\\pymata_aio\\pymata_iot.py -l 5 -c no_client -comport COM" << port;
				fichier_s2a.close();
			}
			else ShowMessage(Popup->Items->Strings[0]);
	//ShellExecute(0, 0, "pymata_cmd.bat", 0, 0 , SW_SHOW );
	AnsiString ligne_cmd = locate_scripts + "\\python.exe ";
	AnsiString ligne_cmd_arg = locate_scripts + "\\Lib\\site-packages\\pymata_aio\\pymata_iot.py -l 5 -c no_client -comport COM" + IntToStr(port);
	ShExecPymata.cbSize = sizeof(SHELLEXECUTEINFO);
			ShExecPymata.fMask = SEE_MASK_NOCLOSEPROCESS;
			ShExecPymata.hwnd = NULL;
			ShExecPymata.lpVerb = NULL;
			ShExecPymata.lpFile = ligne_cmd.c_str();
			ShExecPymata.lpParameters = ligne_cmd_arg.c_str();
			ShExecPymata.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
			if (voirDOS) ShExecPymata.nShow = SW_SHOW;
			if (!voirDOS) ShExecPymata.nShow = SW_HIDE;
			ShExecPymata.hInstApp = NULL;
	ShellExecuteEx(&ShExecPymata);
	PID_pymata = GetProcessId(ShExecPymata.hProcess);
	}
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::s2aio_windowClick(TObject *Sender)
{
if (voirDOS)
	{
	voirDOS = false;
	s2aio_window->Checked = false;
	}
	else {
		voirDOS = true;
		s2aio_window->Checked = true;
		}
//écriture dans le fichier INI
INI->WriteBool("voir_scripts", "fenetre_DOS", voirDOS);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::StopperrDuino1Click(TObject *Sender)
{
TerminateProcess(ShExecRDuino.hProcess, 0);
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::Stopperlacommunication1Click(TObject *Sender)
{
TerminateProcess(ShExecPymata.hProcess, 0);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::FormClose(TObject *Sender, TCloseAction &Action)
{
StopProcess();
ThreadTestArduino->Terminate();
ThreadDetectBrowserClose->Terminate();
WaitForSingleObject(ThreadTestArduino, INFINITE);
}
//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::RedtecterClick(TObject *Sender)
{
StopProcess();
if (auto_detect == false) {
	auto_detect = true;
	INI->WriteBool("AutoDetect", "auto detect Arduino", true);
	Redtecter->Checked = true;
	ThreadTestArduino->NbPortsCOMAvant = 0;
	TestCarte *ThreadTestArduino = new TestCarte(false);
	} else {
		ThreadTestArduino->Terminate();
		auto_detect = false;
		INI->WriteBool("AutoDetect", "auto detect Arduino", false);
		Redtecter->Checked = false;
		//port_COM->MemoListe->Clear();
		MenuConnect->Visible = false;
		MenuUnconnect->Visible = true;
		MenuUnconnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " ?";
	}
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::MenuUnconnectConnect(TObject *Sender)
{
if (auto_detect == false) {
	TerminateProcess(ShExecRDuino.hProcess, 0);
	POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
	port_COM->Left = (Ecran.x)/2 - (port_COM->Width)/2;
	port_COM->Top = (Ecran.y)/2 - (port_COM->Height)/2;
	/*port_COM->MemoListe->Clear();
	port_COM->MemoListe->Visible = false;
	port_COM->MemoListe->Enabled = false;*/
	port_COM->Height = 104;
	port_COM->Visible = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::forceUpdateClick(TObject *Sender)
{
SplashUpdate = new TSplashUpdate(NULL);
POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
SplashUpdate->Left = (Ecran.x)/2 - (SplashUpdate->Width)/2;
SplashUpdate->Top = (Ecran.y)/2 - (SplashUpdate->Height)/2;
SplashUpdate->Show();
SplashUpdate->Repaint();
DeleteFile(ExtractFilePath(Application->ExeName) + "gh-pages");
SplashUpdate->Label1->Caption = Popup->Items->Strings[12];
SplashUpdate->Label1->Visible = true;
SplashUpdate->Repaint();
Sleep(1000);
//stop everything
TerminateProcess(ShExecRDuino.hProcess, 0);
TerminateProcess(ShExecPymata.hProcess, 0);
SplashUpdate->Label2->Caption = Popup->Items->Strings[13];
SplashUpdate->Label2->Visible = true;
SplashUpdate->Repaint();
//get from Github
AnsiString ligne_cmd = ExtractFilePath(Application->ExeName) + "tools\\wget\\wget.exe";
AnsiString ligne_cmd_arg = "https://github.com/technologiescollege/Blockly-at-rduino/archive/gh-pages.zip -o tools\\log.txt --no-check-certificate ";
if (InterfaceS2A->INI->ReadString("proxy", "IP", NULL) == NULL)
	ligne_cmd_arg += "--no-proxy";
	else
	ligne_cmd_arg += "proxy=on http_proxy=http://"
		+ InterfaceS2A->INI->ReadString("proxy", "user", NULL) + ":"
		+ InterfaceS2A->INI->ReadString("proxy", "password", NULL) + "@"
		+ InterfaceS2A->INI->ReadString("proxy", "IP", NULL) + ":"
		+ InterfaceS2A->INI->ReadInteger("proxy", "port", 0);

ShExecUpdate.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecUpdate.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecUpdate.hwnd = NULL;
	ShExecUpdate.lpVerb = NULL;
	ShExecUpdate.lpFile = ligne_cmd.c_str();
	ShExecUpdate.lpParameters = ligne_cmd_arg.c_str();
	ShExecUpdate.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
	if (voirDOS) ShExecUpdate.nShow = SW_SHOW;
	if (!voirDOS) ShExecUpdate.nShow = SW_HIDE;
	ShExecUpdate.hInstApp = NULL;
ShellExecuteEx(&ShExecUpdate);
PID_update = GetProcessId(ShExecUpdate.hProcess);
bool isRunningPID = true;
while (isRunningPID) {
	Sleep(200);
	isRunningPID = TestRunning(PID_update);
	}
    
if (fopen("gh-pages.zip", "r") != NULL) {
   UpdateLog->WriteBool("Download", "finished", true);
   //move file after download
   MoveFile((ExtractFilePath(Application->ExeName) + "tools\\log.txt").c_str(), (ExtractFilePath(Application->ExeName) + "tools\\wget\\log.txt").c_str());
   //unzip file from Github
   RenameFile((ExtractFilePath(Application->ExeName) + "gh-pages").c_str(), (ExtractFilePath(Application->ExeName) + "gh-pages.zip").c_str());
   Sleep(500);
   UpdateLog->WriteBool("Ready to unzip", "file ready", true);
   SplashUpdate->Label3->Caption = Popup->Items->Strings[14];
   SplashUpdate->Label3->Visible = true;
   SplashUpdate->Repaint();
   //need to add path with "file_path_wtih_space"
   ligne_cmd = ExtractFilePath(Application->ExeName) + "tools\\7z.exe";
   ligne_cmd_arg = " x -y ";
   ligne_cmd_arg += '"';
   ligne_cmd_arg += ExtractFilePath(Application->ExeName) + "gh-pages.zip";
   ligne_cmd_arg += '"';
   ShExecUpdate.cbSize = sizeof(SHELLEXECUTEINFO);
     ShExecUpdate.fMask = SEE_MASK_NOCLOSEPROCESS;
     ShExecUpdate.hwnd = NULL;
	 ShExecUpdate.lpVerb = NULL;
	 ShExecUpdate.lpFile = ligne_cmd.c_str();
	 ShExecUpdate.lpParameters = ligne_cmd_arg.c_str();
	 ShExecUpdate.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
	 if (voirDOS) ShExecUpdate.nShow = SW_SHOW;
	 if (!voirDOS) ShExecUpdate.nShow = SW_HIDE;
	 ShExecUpdate.hInstApp = NULL;
   ShellExecuteEx(&ShExecUpdate);
   PID_update = GetProcessId(ShExecUpdate.hProcess);
   isRunningPID = true;
   while (isRunningPID) {
         Sleep(200);
         isRunningPID = TestRunning(PID_update);
         }
   UpdateLog->WriteBool("Unzip", "new Blockly@rduino", true);
   SplashUpdate->Label4->Caption = Popup->Items->Strings[15];
   SplashUpdate->Label4->Visible = true;
   SplashUpdate->Repaint();
   //delete old folder
   SHFILEOPSTRUCT stFileOp;
   ZeroMemory(&stFileOp, sizeof(stFileOp));
   stFileOp.wFunc = FO_DELETE;
   stFileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;
   stFileOp.hwnd = Application->Handle;
   stFileOp.pFrom = (ExtractFilePath(Application->ExeName) + "wwwBlockly@rduino\0").c_str();
   SHFileOperation(&stFileOp);
   UpdateLog->WriteBool("Delete", "no more wwwBlockly@rduino", true);
   //clean
   RenameFile(ExtractFilePath(Application->ExeName) + "Blockly-at-rduino-gh-pages\\", ExtractFilePath(Application->ExeName) + "wwwBlockly@rduino\\");
   DeleteFile((ExtractFilePath(Application->ExeName) + "gh-pages.zip").c_str());
   UpdateLog->WriteBool("Clean", "everything ok", true);
   SplashUpdate->Label5->Caption = Popup->Items->Strings[16];
   SplashUpdate->Label5->Visible = true;
   SplashUpdate->Repaint();
   Sleep(2000);
   updateMenu->Checked = false;
   UpdateLog->WriteBool("NewVersion", "detected", false);
   BrowserRelaunch(0);
    } else {
      ShowMessage(Popup->Items->Strings[21]);
      UpdateLog->WriteBool("Download", "finished", false);
      UpdateLog->WriteBool("Ready to unzip", "file ready", false);
      UpdateLog->WriteBool("Unzip", "new Blockly@rduino", false);
      UpdateLog->WriteBool("Delete", "no more wwwBlockly@rduino", false);
      UpdateLog->WriteBool("Clean", "everything ok", false);
      UpdateLog->WriteBool("NewVersion", "detected", true);
   }   
SplashUpdate->Close();
delete SplashUpdate;
}
//---------------------------------------------------------------------------
bool TInterfaceS2A::TestRunning(int pid)
 {
   HANDLE pss = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
   PROCESSENTRY32 pe = { 0 };
   pe.dwSize = sizeof(pe);
   if (Process32First(pss, &pe))   
   {  
	 while(Process32Next(pss, &pe))
     {  
	   // pe.szExeFile can also be useful
	   if (pe.th32ProcessID == pid)
		 return true;
	 }
   }
   CloseHandle(pss);
   return false;  
 }
//---------------------------------------------------------------------------
bool TInterfaceS2A::CompareVersion()
{
AnsiString ligne_cmd = ExtractFilePath(Application->ExeName) + "tools\\wget\\wget.exe";
AnsiString ligne_cmd_arg = "https://raw.githubusercontent.com/technologiescollege/Blockly-at-rduino/gh-pages/core_BlocklyArduino/version.txt -o tools\\updateAuto.log --no-check-certificate ";

if (InterfaceS2A->INI->ReadString("proxy", "IP", NULL) == NULL)
	ligne_cmd_arg += "--no-proxy";
	else
	ligne_cmd_arg += "proxy=on http_proxy=http://"
		+ InterfaceS2A->INI->ReadString("proxy", "user", NULL) + ":"
		+ InterfaceS2A->INI->ReadString("proxy", "password", NULL) + "@"
		+ InterfaceS2A->INI->ReadString("proxy", "IP", NULL) + ":"
		+ InterfaceS2A->INI->ReadInteger("proxy", "port", 0);

ShExecUpdate.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecUpdate.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecUpdate.hwnd = NULL;
	ShExecUpdate.lpVerb = NULL;
	ShExecUpdate.lpFile = ligne_cmd.c_str();
	ShExecUpdate.lpParameters = ligne_cmd_arg.c_str();
	ShExecUpdate.lpDirectory = ExtractFilePath(Application->ExeName).c_str();
	ShExecUpdate.nShow = SW_HIDE;
	ShExecUpdate.hInstApp = NULL;
ShellExecuteEx(&ShExecUpdate);
PID_update = GetProcessId(ShExecUpdate.hProcess);

bool isRunningPID = true;
while (isRunningPID) {
   	Sleep(200);
   	isRunningPID = TestRunning(PID_update);
   	}
UpdateLog->WriteBool("Compare", "everything ok", true);

INI_Version_new = new TIniFile(ExtractFilePath(Application->ExeName)+ "version.txt");
INI_Version_old = new TIniFile(ExtractFilePath(Application->ExeName)+ "tools\\version.txt");
int GitVersionDist;
int GitVersionLoc;
if (fopen("version.txt", "r") != NULL) {
   GitVersionDist = INI_Version_new->ReadInteger("update", "date", 0);
   GitVersionLoc = INI_Version_old->ReadInteger("update", "date", 0);
   DeleteFile("version.txt");
   UpdateLog->WriteInteger("fichierOld", "version", GitVersionLoc);
   if (GitVersionDist > GitVersionLoc) return true;
      else return false;
   }                    
   else return false;
}

//---------------------------------------------------------------------------

void __fastcall TInterfaceS2A::proxyUpdateClick(TObject *Sender)
{
FormProxy = new TFormProxy(NULL);
POINT Ecran = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
FormProxy->Left = (Ecran.x)/2 - (FormProxy->Width)/2;
FormProxy->Top = (Ecran.y)/2 - (FormProxy->Height)/2;
FormProxy->Visible = true;
FormProxy->LabelProxy->Caption = Popup->Items->Strings[17];
FormProxy->LabelProxyPort->Caption = Popup->Items->Strings[18];
FormProxy->LabelProxyUser->Caption = Popup->Items->Strings[19];
FormProxy->LabelProxyMdp->Caption = Popup->Items->Strings[20];
FormProxy->Show();
FormProxy->Repaint();
}
//---------------------------------------------------------------------------



