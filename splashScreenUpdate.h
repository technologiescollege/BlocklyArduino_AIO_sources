//---------------------------------------------------------------------------

#ifndef splashUpdateH
#define splashUpdateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TSplashUpdate : public TForm
{
__published:	// Composants g�r�s par l'EDI  
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
private:	// D�clarations de l'utilisateur
public:		// D�clarations de l'utilisateur
	__fastcall TSplashUpdate(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashUpdate *SplashUpdate;
//---------------------------------------------------------------------------
#endif
