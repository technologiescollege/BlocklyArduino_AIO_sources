//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "question_port_COM.h"   
#include "Detect.h"
#include "blockly_arduino_COMM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tport_COM *port_COM;
bool key_ok = false;
//---------------------------------------------------------------------------
__fastcall Tport_COM::Tport_COM(TComponent* Owner)
	: TForm(Owner)
{
ButtonOK->Left = (port_COM->Width)/2 - (ButtonOK->Width)/2;
if (MemoListe->Enabled) port_COM->ClientHeight = MemoListe->Top + 19 + MemoListe->Lines->Count*23,396;
	else port_COM->Height = 104;
}
//---------------------------------------------------------------------------
void __fastcall Tport_COM::ButtonOKClick(TObject *Sender)
{
if (key_ok == true) {
	InterfaceS2A->StopProcess();
	InterfaceS2A->port = StrToInt(Edit_COM->Text);
	port_COM->Visible = false;
	InterfaceS2A->INI->WriteInteger("port COM", "port", StrToInt(Edit_COM->Text));
	InterfaceS2A->MenuConnect->Visible = false;
	InterfaceS2A->MenuUnconnect->Visible = true;
	InterfaceS2A->MenuUnconnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " -> " + Edit_COM->Text;
	InterfaceS2A->rDuino();
	InterfaceS2A->BT_upload->Visible = false;
	InterfaceS2A->upload_on->Visible = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall Tport_COM::Edit_COMKeyPress(TObject *Sender, char &Key)
{
key_ok = false;
if (Key < '0' || Key >'9')
	{
	ShowMessage(InterfaceS2A->Popup->Items->Strings[3]);
	Key=NULL;
	}
	else key_ok = true;
}
//---------------------------------------------------------------------------

