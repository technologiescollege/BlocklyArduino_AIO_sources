//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "splash.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplashUpdate *SplashUpdate;
//---------------------------------------------------------------------------
__fastcall TSplashUpdate::TSplashUpdate(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

