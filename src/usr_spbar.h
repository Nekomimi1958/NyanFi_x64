//----------------------------------------------------------------------//
// ステータスバー上のプログレスバー										//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrSttPrgBarH
#define UsrSttPrgBarH

//---------------------------------------------------------------------------
#include <shlobj.h>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class SttProgressBar
{
private:
	TStatusBar *SttBar;
	int PrgIndex;

public:
	TProgressBar *PrgBar;
	int MsgIndex;

	SttProgressBar(TStatusBar *sp, int idx);
	~SttProgressBar();

	void __fastcall ResetPos();
	void __fastcall Begin(const _TCHAR *s = NULL);
	void __fastcall End(UnicodeString s = EmptyStr, int wait = 100);
	void __fastcall SetPosR(float r);
};

//---------------------------------------------------------------------------
#endif
