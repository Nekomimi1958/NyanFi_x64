//----------------------------------------------------------------------//
// ステータスバー上のプログレスバー										//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_spbar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
SttProgressBar::SttProgressBar(TStatusBar *sp, int idx)
{
	PrgBox	 = NULL;
	PrgIndex = idx;
	MsgIndex = -1;
	SttBar	 = sp;

	BarRatio = 0.0;
	BgColor  = TColor(RGB(0xe6, 0xe6, 0xe6));
	BarColor = TColor(RGB(0x25, 0xb0, 0x06));

	if (SttBar && PrgIndex<SttBar->Panels->Count) {
		PrgBox = new TPaintBox(SttBar);
		PrgBox->Parent  = SttBar;
		PrgBox->OnPaint = PrgBoxPaint;
		PrgBox->Visible = false;
		ResetPos();
	}
}
//---------------------------------------------------------------------------
SttProgressBar::~SttProgressBar()
{
}

//---------------------------------------------------------------------------
void __fastcall SttProgressBar::PrgBoxPaint(TObject *Sender)
{
	TPaintBox *pp = (TPaintBox*)Sender;
	TCanvas *cv = pp->Canvas;
	TRect    rc = pp->ClientRect;
	cv->Brush->Color = BgColor;
	cv->FillRect(rc);
	InflateRect(rc, -1, -1);
	rc.Right = rc.Left + (int)(rc.Width() * BarRatio);
	cv->Brush->Color = BarColor;
	cv->FillRect(rc);
}

//---------------------------------------------------------------------------
//プログレスバー位置のリセット
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::ResetPos()
{
	if (SttBar && PrgBox && PrgIndex<SttBar->Panels->Count) {
		PrgBox->Top = 3;
		int xp = 3;
		for (int i=0; i<PrgIndex; i++) xp += SttBar->Panels->Items[i]->Width;
		PrgBox->Left   = xp;
		PrgBox->Height = SttBar->ClientHeight - 4;
		PrgBox->Width  = SttBar->Panels->Items[PrgIndex]->Width - 8;
	}
}

//---------------------------------------------------------------------------
//プログレスバー表示開始
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::Begin(const _TCHAR *s)
{
	Screen->Cursor = crHourGlass;

	if (PrgBox) {
		ResetPos();
		UnicodeString msg = s? s : null_TCHAR;
		if (MsgIndex>=0 && MsgIndex<SttBar->Panels->Count) SttBar->Panels->Items[MsgIndex]->Text = msg;
		SttBar->Panels->Items[PrgIndex]->Text = EmptyStr;
		SttBar->Repaint();
		BarRatio = 0.0;
		PrgBox->Visible = true;
		PrgBox->Repaint();
	}
}

//---------------------------------------------------------------------------
//プログレスバー表示終了
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::End(UnicodeString s, int wait)
{
	Screen->Cursor = crDefault;

	if (PrgBox) {
		BarRatio = 1.0;
		PrgBox->Repaint();
		Sleep(wait);
		BarRatio = 0.0;
		PrgBox->Visible = false;
		if (MsgIndex>=0 && MsgIndex<SttBar->Panels->Count) SttBar->Panels->Items[MsgIndex]->Text = s;
	}
}
//---------------------------------------------------------------------------
//バー設定
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::SetPosI(int idx, int count)
{
	if (PrgBox) {
		BarRatio = std::min(((count>0)? 1.0 * idx/count : 0.0), 1.0);
		PrgBox->Repaint();
	}
}
//---------------------------------------------------------------------------
