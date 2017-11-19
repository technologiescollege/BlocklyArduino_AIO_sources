//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Welcome.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplashScreen *SplashScreen;
//---------------------------------------------------------------------------
__fastcall TSplashScreen::TSplashScreen(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplashScreen::Label4Click(TObject *Sender)
{
ShellExecute(0, 0, "http://astuce.technology/", 0, 0 , SW_SHOW );	
}
//---------------------------------------------------------------------------
