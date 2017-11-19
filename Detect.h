//---------------------------------------------------------------------------

#ifndef DetectH
#define DetectH
//---------------------------------------------------------------------------
#include <Classes.hpp>


#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TestCarte : public TThread
{
typedef struct tagTHREADNAME_INFO
  {
    DWORD dwType;     // doit être 0x1000
    LPCSTR szName;    // pointeur sur le nom (dans l'espace d'adresse de l'utilisateur)
    DWORD dwThreadID; // ID de thread (-1=thread de l'appelant)
    DWORD dwFlags;    // réservé pour une future utilisation, doit être zéro
  } THREADNAME_INFO;
private:
	void SetName();
	int __fastcall Scan_COM();
	void __fastcall Scan_Keys();
	void __fastcall ListPortCOM();
	void __fastcall ChangeCOM(TObject *Sender);
protected:
	void __fastcall Execute();
	void __fastcall OnTerminate();
	bool isRunning(int pid);
public:
	__fastcall TestCarte(bool CreateSuspended);
	void __fastcall Scan_Arduino();
	int NbPortsCOMAvant;
};
//---------------------------------------------------------------------------
#endif
