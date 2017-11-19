//---------------------------------------------------------------------------

#ifndef DetectBrowserH
#define DetectBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
//---------------------------------------------------------------------------
class BrowserDetect : public TThread
{
  typedef struct tagTHREADNAME_INFO
  {
    DWORD dwType;     // doit �tre 0x1000
    LPCSTR szName;    // pointeur sur le nom (dans l'espace d'adresse de l'utilisateur)
    DWORD dwThreadID; // ID de thread (-1=thread de l'appelant)
    DWORD dwFlags;    // r�serv� pour une future utilisation, doit �tre z�ro
  } THREADNAME_INFO;
private:
  void SetName();
protected:
	void __fastcall Execute(); 
	bool isRunning(int pid);
public:
	__fastcall BrowserDetect(bool CreateSuspended); 
	void __fastcall Scan_Browser();
};
//---------------------------------------------------------------------------
#endif
