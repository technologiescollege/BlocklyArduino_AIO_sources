//---------------------------------------------------------------------------

#ifndef splashScreenWelcomeH
#define splashScreenWelcomeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
//---------------------------------------------------------------------------
class TSplashScreen : public TForm
{
__published:	// Composants gérés par l'EDI
	TImage *Image1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TImage *Image2;
	TLabel *Label4;
    TShape *Shape1;
	void __fastcall Label4Click(TObject *Sender);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
	__fastcall TSplashScreen(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashScreen *SplashScreen;
//---------------------------------------------------------------------------
#endif
