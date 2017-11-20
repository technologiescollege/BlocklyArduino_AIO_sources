//---------------------------------------------------------------------------
#include <stdio.h>
#include <vcl.h>
#include <registry.hpp>
#pragma hdrstop
#include <tlhelp32.h>
#include <cstdio>
#include <windows.h>
#include <tlhelp32.h>

#include <string>

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include "DetectCard.h"
#include "blockly_arduino_COMM.h"
#include "popup_port_COM.h" 
#pragma package(smart_init)      
#pragma hdrstop

int NbPortsCOM;

TStringList* availableComPorts = new TStringList(); 
TStringList* ArduinoComPorts = new TStringList();
TRegistry *RegistryCOM = new TRegistry();
TRegistry *RegistryKeys = new TRegistry();
TRegistry *RegistryCard1 = new TRegistry();
TRegistry *RegistryCard2 = new TRegistry();
TRegistry *RegistryCard3 = new TRegistry();
TRegistry *RegistryCard4 = new TRegistry();

/*array of result for keys :
keys_array[0] = 1 si vide
keys_array[1] = 1 si "SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum"
keys_array[2] = 1 si "SYSTEM\\CurrentControlSet\\Services\\FTDIBUS\\Enum"
keys_array[3] = 1 si "SYSTEM\\CurrentControlSet\\Services\\CH341SER_A64\\Enum"
keys_array[4] = 1 si "SYSTEM\\CurrentControlSet\\Services\\silabser\\Enum"*/
int keys_array[5] = {0, 0, 0, 0, 0};

//---------------------------------------------------------------------------

__fastcall TestCarte::TestCarte(bool CreateSuspended)
	: TThread(CreateSuspended)
{
NbPortsCOMAvant = 0;
}
//---------------------------------------------------------------------------
bool TestCarte::isRunning(int pid)
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
void TestCarte::SetName()
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = "TestArduino";
	info.dwThreadID = -1;
	info.dwFlags = 0;
	__try
	{
		RaiseException( 0x406D1388, 0, sizeof(info)/sizeof(DWORD),(DWORD*)&info );
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TestCarte::Execute()
{
SetName();
FreeOnTerminate = true;
while (!Terminated)
   {
   if(Terminated) break;
   Synchronize(&Scan_Arduino);
   Sleep(250);
   }
OnTerminate();
}      
//---------------------------------------------------------------------------
void __fastcall TestCarte::OnTerminate()
{
RegistryCOM->CloseKey();
delete RegistryCOM;
RegistryKeys->CloseKey();
delete RegistryKeys;
RegistryCard1->CloseKey();
delete RegistryCard1;
RegistryCard2->CloseKey();
delete RegistryCard2;
RegistryCard3->CloseKey();
delete RegistryCard3;
RegistryCard4->CloseKey();
delete RegistryCard4;
}

//---------------------------------------------------------------------------
int __fastcall TestCarte::Scan_COM()
{
RegistryCOM->RootKey = HKEY_LOCAL_MACHINE;
RegistryCOM->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
RegistryCOM->GetValueNames(availableComPorts);
RegistryCOM->CloseKey();
return (availableComPorts->Count);
}         

//---------------------------------------------------------------------------
void __fastcall TestCarte::Scan_Keys()
{
RegistryKeys->RootKey = HKEY_LOCAL_MACHINE;
//defaut : pas de carte
keys_array[0] = 1;

RegistryKeys->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum");
if (RegistryKeys->ValueExists("0")) {
	keys_array[0] = 0;
	keys_array[1] = 1;
	}
RegistryKeys->CloseKey();
RegistryKeys->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\FTDIBUS\\Enum");
if (RegistryKeys->ValueExists("0")) {
	keys_array[0] = 0;
	keys_array[2] = 1;
	}
RegistryKeys->CloseKey();
RegistryKeys->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\CH341SER_A64\\Enum");
if (RegistryKeys->ValueExists("0")) {
	keys_array[0] = 0;
	keys_array[3] = 1;
	}
RegistryKeys->CloseKey();
RegistryKeys->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\silabser\\Enum");
if (RegistryKeys->ValueExists("0")) {
	keys_array[0] = 0;
	keys_array[4] = 1;
	}
RegistryKeys->CloseKey();
}


//---------------------------------------------------------------------------
 void __fastcall TestCarte::Scan_Arduino()
{
if (isRunning(InterfaceS2A->PID_rduino)) {
	InterfaceS2A->upload_on->Visible=true;
	InterfaceS2A->BT_upload->Visible=false;
	} else {
			InterfaceS2A->upload_on->Visible=false;
			InterfaceS2A->BT_upload->Visible=true;
			}
if (isRunning(InterfaceS2A->PID_pymata)) {
	InterfaceS2A->supervision_on->Visible=true;
	InterfaceS2A->BT_supervision->Visible=false;
	} else {
			InterfaceS2A->supervision_on->Visible=false;
			InterfaceS2A->BT_supervision->Visible=true;
			}

NbPortsCOM = Scan_COM();
if (NbPortsCOMAvant != NbPortsCOM)
	{
	NbPortsCOMAvant = NbPortsCOM;
	Scan_Keys();
	/*	port_COM2->MemoListe->Clear();
		port_COM2->MemoListe->Lines->Add(IntToStr(keys_array[0]));
		port_COM2->MemoListe->Lines->Add(IntToStr(keys_array[1]));
		port_COM2->MemoListe->Lines->Add(IntToStr(keys_array[2]));
		port_COM2->MemoListe->Lines->Add(IntToStr(keys_array[3]));
		port_COM2->MemoListe->Lines->Add(IntToStr(keys_array[4]));*/
	if (keys_array[0]==1) {
		InterfaceS2A->MenuConnect->Visible = false;
		InterfaceS2A->MenuUnconnect->Visible = true;
		InterfaceS2A->TrayIcon->BalloonHint = InterfaceS2A->Popup->Items->Strings[11];
		InterfaceS2A->TrayIcon->BalloonFlags = bfWarning;
		InterfaceS2A->TrayIcon->Visible = true;
		InterfaceS2A->TrayIcon->ShowBalloonHint();
		InterfaceS2A->port=0;
		InterfaceS2A->INI->WriteInteger("port COM", "port", 0);
		InterfaceS2A->MenuUnconnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " ?";
		InterfaceS2A->StopProcess();
		} else {
				InterfaceS2A->MenuConnect->Visible = true;
				InterfaceS2A->MenuUnconnect->Visible = false;
				if (keys_array[1]==1) {
					//valable pour cartes Arduino seules
					//test de la lecture de la valeur de la chaîne tronquée de 40 caractères
					//à partir du 13ème pour ne garder que le VID
					//ch340 port chinois, 2341 arduino, 2A03 leonardo, 0403 duemilanove
					//obligation de 2 tests pour minuscules compatibilité Windows XP
					RegistryCard1->RootKey = HKEY_LOCAL_MACHINE;
					RegistryCard1->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\usbser\\Enum");
					if (((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\VID_2341")||
						((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\Vid_2341")||
						((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\VID_2A03")||
						((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\Vid_2a03")||
						((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\VID_1A86")||
						((RegistryCard1->ReadString("0").Delete(13,40))=="USB\\Vid_1a86"))
						{   
						RegistryCOM->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
						RegistryCOM->GetValueNames(availableComPorts);
						for (int i(0); i<availableComPorts->Count; i++) {
							if (availableComPorts->Strings[i].Delete(15,3) != ("\\Device\\USBSER")) availableComPorts->Delete(i);
							}
						}
					RegistryCard1->CloseKey();
					}
				if (keys_array[2]==1) {
					//valable pour cartes Arduino avec puce FTDI   VID = 0403 , PID = 6001, 6010 ou 6011
					//test de la lecture de la valeur de la chaîne tronquée de 41 caractères
					//à partir du 13ème pour ne garder que le VID
					//obligation de 2 tests pour minuscules compatibilité Windows XP
					RegistryCard2->RootKey = HKEY_LOCAL_MACHINE;
					RegistryCard2->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\FTDIBUS\\Enum");
					if (((RegistryCard2->ReadString("0").Delete(13,41))=="USB\\VID_0403")||
						((RegistryCard2->ReadString("0").Delete(13,41))=="USB\\Vid_0403"))
						{
						RegistryCOM->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
						RegistryCOM->GetValueNames(availableComPorts);
						for (int i(0); i<availableComPorts->Count; i++) {
							if (availableComPorts->Strings[i].Delete(12,1) != ("\\Device\\VCP")) availableComPorts->Delete(i);
							}
						}
					RegistryCard2->CloseKey();
					}
				if (keys_array[3]==1) {
					//valable pour cartes Arduino seules
					//test de la lecture de la valeur de la chaîne tronquée de 40 caractères
					//à partir du 13ème pour ne garder que le VID
					//ch340 port chinois, 2341 arduino, 2A03 leonardo, 0403 duemilanove
					//obligation de 2 tests pour minuscules compatibilité Windows XP      
					RegistryCard3->RootKey = HKEY_LOCAL_MACHINE;
					RegistryCard3->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\CH341SER_A64\\Enum");
					if (((RegistryCard3->ReadString("0").Delete(13,46))=="USB\\VID_1A86")||
						((RegistryCard3->ReadString("0").Delete(13,46))=="USB\\Vid_1a86"))
						{
						RegistryCOM->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM"); 
						RegistryCOM->GetValueNames(availableComPorts);
						for (int i(0); i<availableComPorts->Count; i++) {
							if (availableComPorts->Strings[i].Delete(15,1) != ("\\Device\\Serial")) availableComPorts->Delete(i);
							}
						}
					RegistryCard3->CloseKey();
					}
				if (keys_array[4]==1) {
					//valable pour cartes Arduino seules
					//test de la lecture de la valeur de la chaîne tronquée de 40 caractères
					//à partir du 13ème pour ne garder que le VID
					//obligation de 2 tests pour minuscules compatibilité Windows XP        
					RegistryCard4->RootKey = HKEY_LOCAL_MACHINE;
					RegistryCard4->OpenKeyReadOnly("SYSTEM\\CurrentControlSet\\Services\\silabser\\Enum");
					if (((RegistryCard4->ReadString("0").Delete(13,40))=="USB\\VID_10C4")||
						((RegistryCard4->ReadString("0").Delete(13,40))=="USB\\Vid_10c4"))
						{
						RegistryCOM->OpenKeyReadOnly("HARDWARE\\DEVICEMAP\\SERIALCOMM");
						RegistryCOM->GetValueNames(availableComPorts);
						for (int i(0); i<availableComPorts->Count; i++) {
							if (availableComPorts->Strings[i].Delete(17,1) != ("\\Device\\Silabser")) availableComPorts->Delete(i);
							}
						}
					RegistryCard4->CloseKey();
					}
		//test
		//for (int i(0); i<availableComPorts->Count; i++) port_COM2->MemoListe->Lines->Add(availableComPorts->Strings[i]);
		if (availableComPorts->Count>1) ListPortCOM();
			else {
				//prends la première valeur comme entrée de clé et cherche sa valeur
				InterfaceS2A->port=StrToInt(RegistryCOM->ReadString(availableComPorts->Strings[0]).Delete(1,3));
				RegistryCOM->CloseKey();
				InterfaceS2A->MenuConnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " : " + IntToStr(InterfaceS2A->port);
				InterfaceS2A->INI->WriteInteger("port COM", "port", InterfaceS2A->port);
				InterfaceS2A->rDuino();
				InterfaceS2A->TrayIcon->BalloonHint = InterfaceS2A->Popup->Items->Strings[10] + IntToStr(InterfaceS2A->INI->ReadInteger("port COM", "port", InterfaceS2A->port));
				InterfaceS2A->TrayIcon->BalloonFlags = bfInfo;
				InterfaceS2A->TrayIcon->Visible = true;
				InterfaceS2A->TrayIcon->ShowBalloonHint();
				InterfaceS2A->PopupMenuTray->Items->Items[5]->Clear();
				}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TestCarte::ListPortCOM()
{
TMenuItem *NewItem = new TMenuItem(InterfaceS2A);
//port_COM->MemoListe->Clear();
for (int i(0); i < availableComPorts->Count; i++)
	{
	//port_COM->MemoListe->Lines->Add(RegistryCOM->ReadString(availableComPorts->Strings[i]).Delete(1,3));
	//port_COM->MemoListe->Height = 19 + port_COM->MemoListe->Lines->Count*23,396;
	//multi card COM port list
	NewItem = new TMenuItem(InterfaceS2A);
	NewItem->Name = "COM" + RegistryCOM->ReadString(availableComPorts->Strings[i]).Delete(1,3);
	NewItem->Caption = RegistryCOM->ReadString(availableComPorts->Strings[i]).Delete(1,3);
	NewItem->Tag = StrToInt(NewItem->Caption);
	NewItem->OnClick = ChangeCOM;     
	TNotifyEvent();
	InterfaceS2A->PopupMenuTray->Items->Items[5]->Add(NewItem);
	}
/*port_COM->ClientHeight = port_COM->MemoListe->Top + port_COM->MemoListe->Height;
port_COM->MemoListe->Enabled = true;
port_COM->MemoListe->Visible = true;*/
}  

//---------------------------------------------------------------------------
void __fastcall TestCarte::ChangeCOM(TObject *Sender)
{
InterfaceS2A->StopProcess();
TMenuItem *ClickedItem = static_cast<TMenuItem *>(Sender);
int NameMenuCOM=static_cast<TMenuItem*>(Sender)->Tag;
int index = InterfaceS2A->PopupMenuTray->Items->Items[5]->Count;
for (int i(0); i < index; i++) InterfaceS2A->PopupMenuTray->Items->Items[5]->Items[i]->Checked = false;
ClickedItem->Checked = true;
InterfaceS2A->port=NameMenuCOM;
InterfaceS2A->MenuConnect->Caption = InterfaceS2A->MenuConnect_NumCOM + " : " + IntToStr(InterfaceS2A->port);
InterfaceS2A->INI->WriteInteger("port COM", "port", InterfaceS2A->port);
InterfaceS2A->rDuino();
InterfaceS2A->TrayIcon->BalloonHint = InterfaceS2A->Popup->Items->Strings[9] + IntToStr(InterfaceS2A->INI->ReadInteger("port COM", "port", InterfaceS2A->port));
InterfaceS2A->TrayIcon->BalloonFlags = bfInfo;
InterfaceS2A->TrayIcon->Visible = true;
InterfaceS2A->TrayIcon->ShowBalloonHint();
}
