//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("blockly_arduino_COMM.cpp", InterfaceS2A);
USEFORM("popup_port_COM.cpp", port_COM);
USEFORM("splashScreenUpdate.cpp", SplashUpdate);
USEFORM("popup_proxy.cpp", FormProxy);
USEFORM("splashScreenWelcome.cpp", SplashScreen);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HANDLE hMutex;
	try
	{
		hMutex = CreateMutex (NULL,FALSE,"Blockly@rduino");
		if (GetLastError() == ERROR_ALREADY_EXISTS) return 0;
		Application->Initialize();
		Application->Title = "Blockly@rduino communication";
		Application->CreateForm(__classid(TInterfaceS2A), &InterfaceS2A);
         Application->CreateForm(__classid(Tport_COM), &port_COM);
         Application->ShowMainForm = false;
		Application->Run();  
		CloseHandle(hMutex);
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
