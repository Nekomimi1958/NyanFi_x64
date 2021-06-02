//----------------------------------------------------------------------//
// ステータスバー上のプログレスバー										//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrSttPrgBarH
#define UsrSttPrgBarH

//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class SttProgressBar
{
private:
	TStatusBar *SttBar;
	int PrgIndex;
	double BarRatio;

	void __fastcall PrgBoxPaint(TObject *Sender);
	void __fastcall ResetPos();

public:
	TPaintBox *PrgBox;
	int MsgIndex;

	TColor BgColor;
	TColor BarColor;

	SttProgressBar(TStatusBar *sp, int idx);
	~SttProgressBar();

	void __fastcall Begin(const _TCHAR *s = NULL);
	void __fastcall End(UnicodeString s = EmptyStr, int wait = 100);
	void __fastcall SetPosI(int idx, int count);
};

//---------------------------------------------------------------------------
#endif
