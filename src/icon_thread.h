//----------------------------------------------------------------------//
// NyanFi																//
//  �A�C�R���擾�X���b�h												//
//----------------------------------------------------------------------//
#ifndef TGetIconThreadH
#define TGetIconThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class TGetIconThread : public TThread
{
private:
	void __fastcall IconNotify();
	void __fastcall Execute();

public:
	HWND CallbackWnd;

	__fastcall TGetIconThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
