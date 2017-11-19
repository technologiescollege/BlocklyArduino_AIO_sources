//---------------------------------------------------------------------------

#include <vcl.h>    
#include <tlhelp32.h>
#pragma hdrstop

#include "DetectBrowser.h" 
#include "blockly_arduino_COMM.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important : les méthodes et les propriétés des objets de la CVL peuvent uniquement
//   être utilisées dans une méthode appelée en utilisant Synchronize, comme suit :
//
//      Synchronize(&UpdateCaption);
//
//   où UpdateCaption serait de la forme :
//
//      void __fastcall BrowserDetect::UpdateCaption()
//      {
//        Form1->Caption = "Mis à jour dans un thread";
//      }
//---------------------------------------------------------------------------

__fastcall BrowserDetect::BrowserDetect(bool CreateSuspended)
	: TThread(CreateSuspended)
{
}
//---------------------------------------------------------------------------
void BrowserDetect::SetName()
{
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = "DetectBrowserClose";
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
void __fastcall BrowserDetect::Execute()
{
SetName();
FreeOnTerminate = true;
while (!Terminated)
   {
   if(Terminated) break;
   Synchronize(&Scan_Browser);
   Sleep(250);
   }
Application->Terminate();
}
//---------------------------------------------------------------------------
bool BrowserDetect::isRunning(int pid)
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
void __fastcall BrowserDetect::Scan_Browser()
{
if (!isRunning(InterfaceS2A->PID_nwjs)) {
	InterfaceS2A->StopProcess();
	InterfaceS2A->Close();
	}
}
