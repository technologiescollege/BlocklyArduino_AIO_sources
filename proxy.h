//---------------------------------------------------------------------------

#ifndef proxyH
#define proxyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormProxy : public TForm
{
__published:	// Composants gérés par l'EDI
	TLabel *LabelProxy;
	TLabel *LabelProxyPort;
	TLabel *LabelProxyUser;
	TLabel *LabelProxyMdp;
	TEdit *EditProxyPort;
	TEdit *EditProxyUser;
	TButton *ButtonProxyOK;
	TEdit *EditProxyMdP;
	void __fastcall ButtonProxyOKClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// Déclarations de l'utilisateur
	HWND hWndIPAddress;
public:		// Déclarations de l'utilisateur
	__fastcall TFormProxy(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProxy *FormProxy;
//---------------------------------------------------------------------------
#endif
