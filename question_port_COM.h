//---------------------------------------------------------------------------

#ifndef question_port_COMH
#define question_port_COMH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class Tport_COM : public TForm
{
__published:	// Composants gérés par l'EDI      
	TEdit *Edit_COM;
	TButton *ButtonOK;
	TMemo *MemoListe;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall Edit_COMKeyPress(TObject *Sender, char &Key);
private:	// Déclarations de l'utilisateur
public:		// Déclarations de l'utilisateur
	__fastcall Tport_COM(TComponent* Owner);
	TStringList* availableComPorts;
};
//---------------------------------------------------------------------------
extern PACKAGE Tport_COM *port_COM;
//---------------------------------------------------------------------------
#endif
