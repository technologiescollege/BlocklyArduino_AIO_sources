//---------------------------------------------------------------------------

#include <vcl.h> 
#include <string.h>
#pragma hdrstop

#include "proxy.h" 
#include "blockly_arduino_COMM.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormProxy *FormProxy;

AnsiString ProxyHTTP;
int ProxyPort;
AnsiString ProxyUser;
AnsiString ProxyMdP;

//---------------------------------------------------------------------------
__fastcall TFormProxy::TFormProxy(TComponent* Owner)
	: TForm(Owner)
{ 
//create IP fields
hWndIPAddress = CreateWindowEx( 0,
					WC_IPADDRESS,
					NULL,
					WS_CHILD | WS_VISIBLE | WS_TABSTOP,
					180, 10, 240, 24,
					Handle,
					NULL,
					HInstance,
					NULL );

ProxyHTTP = InterfaceS2A->INI->ReadString("proxy", "IP", NULL);
if (ProxyHTTP != "") {
	char * ip;
	ip = strtok (ProxyHTTP.c_str(),".");
	BYTE IPPart1 = StrToInt(ip);
	ip = strtok(NULL, ".");
	BYTE IPPart2 = StrToInt(ip);
	ip = strtok(NULL, ".");
	BYTE IPPart3 = StrToInt(ip);
	ip = strtok(NULL, ".");
	BYTE IPPart4 = StrToInt(ip);
	LPARAM lpAdr = MAKEIPADDRESS(IPPart1, IPPart2, IPPart3, IPPart4);
	//if IP in INI file, send it to IP fields
	SendMessage(hWndIPAddress, IPM_SETADDRESS, 0, lpAdr);
	} else SendMessage(hWndIPAddress, IPM_CLEARADDRESS, 0, 0);

ProxyPort = InterfaceS2A->INI->ReadInteger("proxy", "port", NULL);
if ((ProxyPort > 0) && (ProxyPort<65535)) EditProxyPort->Text = IntToStr(ProxyPort);
	else EditProxyPort->Text = "";
EditProxyUser->Text = InterfaceS2A->INI->ReadString("proxy", "user", "");
EditProxyMdP->Text = InterfaceS2A->INI->ReadString("proxy", "password", "");
}
//---------------------------------------------------------------------------
void __fastcall TFormProxy::ButtonProxyOKClick(TObject *Sender)
{
DWORD CurAddress;
if (SendMessage(hWndIPAddress, IPM_ISBLANK, 0, 0)) InterfaceS2A->INI->WriteString("proxy", "IP", "");
	else {
		SendMessage(hWndIPAddress, IPM_GETADDRESS, 0, (LPARAM)(LPDWORD)&CurAddress);
		ProxyHTTP = IntToStr(FIRST_IPADDRESS((LPARAM)CurAddress)) + "." + IntToStr(SECOND_IPADDRESS((LPARAM)CurAddress))
			+ "." + IntToStr(THIRD_IPADDRESS((LPARAM)CurAddress)) + "." + IntToStr(FOURTH_IPADDRESS((LPARAM)CurAddress));
		InterfaceS2A->INI->WriteString("proxy", "IP", ProxyHTTP);
	}
if (EditProxyPort->Text != "") ProxyPort = StrToInt(EditProxyPort->Text);
	else ProxyPort = 0;
if ((ProxyPort >= 0) && (ProxyPort <65535)) {
	InterfaceS2A->INI->WriteInteger("proxy", "port", ProxyPort);
	InterfaceS2A->INI->WriteString("proxy", "user", EditProxyUser->Text);
	InterfaceS2A->INI->WriteString("proxy", "password", EditProxyMdP->Text);
	delete FormProxy;
	} else ShowMessage(InterfaceS2A->Popup->Items->Strings[5]);
}

//---------------------------------------------------------------------------

void __fastcall TFormProxy::FormClose(TObject *Sender, TCloseAction &Action)
{
delete FormProxy;
}
//---------------------------------------------------------------------------

