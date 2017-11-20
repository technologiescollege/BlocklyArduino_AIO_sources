//---------------------------------------------------------------------------

#ifndef blockly_arduino_COMMH
#define blockly_arduino_COMMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Dialogs.hpp>
#include <FileCtrl.hpp>
#include <IniFiles.hpp>
#include <Graphics.hpp>
#include <string>
#include "GestionLangue.h"
//---------------------------------------------------------------------------

class TInterfaceS2A : public TForm
{
__published:	// Composants gérés par l'EDI
	TMenuItem *A_propos;
	TMenuItem *BT_supervision;
	TMenuItem *Wiki1;
	TMenuItem *PrparerArduino1;
	TMenuItem *Utilisation1;
	TMenuItem *N3Mmentodescommandes1;
	TMenuItem *Aide1;
	TMenuItem *Apropos1;
	TMenuItem *Documentation1;
	TMenuItem *Siteofficieldes2a1;
	TMenuItem *N4Principesdeprogrammation1;
	TMenuItem *Gestionnairedepriphriques1;
	TMenuItem *Outils1;
	TMenuItem *Langue1;
	TMenuItem *N4;
	TMenuItem *Init;
	TMenuItem *SiteofficielArduino1;
	TMenuItem *N6;
	TOpenDialog *OpenDialog;
	TMenuItem *Firmata;
	TMenuItem *Firmata_Uno;
	TMenuItem *Firmata_Mega;
	TMenuItem *Links;
	TMenuItem *InstallDriver;
	TMenuItem *w32;
	TMenuItem *w64;
	TMenuItem *Drivers;
	TMenuItem *Firmata_Leonardo;
	TMenuItem *Firmata_Micro;
	TMenuItem *Firmata_Nano;
	TMenuItem *BT_upload;
	TMenuItem *s2aio_window;
	TComboBox *Popup;
	TMenuItem *N8;
	TMenuItem *DuemilanoveDiecimila328P1;
	TMenuItem *SiteofficielderDuinoUploader;
	TMenuItem *MenuConnect;
	TMenuItem *supervision_on;
	TMenuItem *upload_on;
	TMenuItem *N3;
	TMenuItem *N9;
	TMenuItem *Localiserledossierdocumentation1;
	TMenuItem *N5;
	TMenuItem *Redmarrerlenavigateur1;
	TPopupMenu *PopupMenuTray;
	TMenuItem *Forum1;
	TTrayIcon *TrayIcon;
	TMenuItem *N1;
	TMenuItem *N11;
	TMenuItem *Firmata_RoMeo;
	TMenuItem *Redtecter;
	TMenuItem *MenuUnconnect;
	TMenuItem *forceUpdate;
	TMenuItem *updateMenu;
	TMenuItem *proxyUpdate;
	TMenuItem *N2;
	void __fastcall InitINI();
	void __fastcall A_proposClick(TObject *Sender);
	void __fastcall PrparerArduino1Click(TObject *Sender);
	void __fastcall Utilisation1Click(TObject *Sender);
	void __fastcall N3Mmentodescommandes1Click(TObject *Sender);
	void __fastcall Wiki1Click(TObject *Sender);
	void __fastcall pymataClick(TObject *Sender);
	void __fastcall rDuinoClick(TObject *Sender); 
	void __fastcall rDuino();
	void __fastcall Apropos1Click(TObject *Sender);
	void __fastcall Siteofficieldes2a1Click(TObject *Sender);
	void __fastcall N4Principesdeprogrammation1Click(TObject *Sender);
	void __fastcall Gestionnairedepriphriques1Click(TObject *Sender);
	void __fastcall SearchDocs(AnsiString FilePath, TStringList * Extensions, TStrings * ListeFichiers);
	void __fastcall DocsClick(TObject *Sender);
	void __fastcall InitClick(TObject *Sender);
	void __fastcall SiteofficielArduino1Click(TObject *Sender);
	void __fastcall Localiserledossierdocumentation1Click(TObject *Sender);
	void __fastcall Firmata_UnoClick(TObject *Sender);
	void __fastcall Firmata_MegaClick(TObject *Sender);
	void __fastcall w32Click(TObject *Sender);
	void __fastcall w64Click(TObject *Sender);
	void __fastcall DriversClick(TObject *Sender);
	void __fastcall Firmata_LeonardoClick(TObject *Sender);
	void __fastcall Firmata_MicroClick(TObject *Sender);
	void __fastcall Firmata_NanoClick(TObject *Sender);
	void __fastcall s2aio_windowClick(TObject *Sender);
	void __fastcall DuemilanoveDiecimila328P1Click(TObject *Sender);
	void __fastcall SiteofficielderDuinoUploaderClick(TObject *Sender);
	void __fastcall StopperrDuino1Click(TObject *Sender);
	void __fastcall Stopperlacommunication1Click(TObject *Sender);
	void __fastcall BrowserLaunchClick(TObject *Sender);                
	void __fastcall BrowserRelaunch(TObject *Sender);
	void __fastcall GetOSversion(TObject *Sender);
	void __fastcall Forum1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RedtecterClick(TObject *Sender);
	void __fastcall MenuUnconnectConnect(TObject *Sender);
	void __fastcall forceUpdateClick(TObject *Sender);
	void __fastcall proxyUpdateClick(TObject *Sender);
	//void __fastcall MenuUnconnectConnect2(TObject *Sender);
private:	// Déclarations de l'utilisateur
	TIniFile *INI_Version_old;
	TIniFile *INI_Version_new;
	TLabel * interLabel ;
	TButton * interButton ;
	TInterfaceS2A * interInterfaceS2A ;
	TGroupBox * interGroupeBox ;
	TComboBox * interComboBox ;
	TCheckBox * interCheckBox ;
	TRadioGroup * interRadioGroupe ;
	TMenuItem * interMenuItem ;
	GestionLangue * langue ;
	bool TestRunning(int pid);
	bool CompareVersion();
public:		// Déclarations de l'utilisateur
	void __fastcall Langue1Click(TObject *Sender);
	void __fastcall StopProcess();
	__fastcall TInterfaceS2A(TComponent* Owner);
	TIniFile *INI;
	TIniFile *UpdateLog;
	DWORD PID_rduino;
	DWORD PID_pymata;
	DWORD PID_nwjs;  
	DWORD PID_update;
	int port;
	String MenuConnect_NumCOM;
	//boolean relaunchNwjs;
	boolean auto_detect;
protected:
};
//---------------------------------------------------------------------------
extern PACKAGE TInterfaceS2A *InterfaceS2A;
//---------------------------------------------------------------------------
#endif
