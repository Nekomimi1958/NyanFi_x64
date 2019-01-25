//----------------------------------------------------------------------//
// ステータスバー上のプログレスバー										//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "usr_str.h"
#include "usr_spbar.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
SttProgressBar::SttProgressBar(TStatusBar *sp, int idx)
{
	PrgBar	 = NULL;
	PrgIndex = idx;
	MsgIndex = -1;
	SttBar	 = sp;

	if (SttBar && PrgIndex<SttBar->Panels->Count) {
		//プログレスバーを作成
		PrgBar = new TProgressBar(SttBar);
		PrgBar->DoubleBuffered = true;
		PrgBar->Parent  = SttBar;
		PrgBar->Top     = 2;
		int xp = 2;
		for (int i=0; i<PrgIndex; i++) xp += SttBar->Panels->Items[i]->Width;
		PrgBar->Left	= xp;
		PrgBar->Height	= SttBar->ClientHeight -2;
		PrgBar->Width	= SttBar->Panels->Items[PrgIndex]->Width -2;
		PrgBar->Min 	= 0;
		PrgBar->Max 	= 100;
		PrgBar->Smooth  = true;
		PrgBar->Visible = false;
	}
}
//---------------------------------------------------------------------------
SttProgressBar::~SttProgressBar()
{
}

//---------------------------------------------------------------------------
//プログレスバー位置のリセット
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::ResetPos()
{
	if (SttBar && PrgIndex<SttBar->Panels->Count) {
		PrgBar->Top     = 2;
		int xp = 2;
		for (int i=0; i<PrgIndex; i++) xp += SttBar->Panels->Items[i]->Width;
		PrgBar->Left	= xp;
		PrgBar->Height	= SttBar->ClientHeight -2;
		PrgBar->Width	= SttBar->Panels->Items[PrgIndex]->Width -2;
	}
}

//---------------------------------------------------------------------------
//プログレスバー表示開始
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::Begin(const _TCHAR *s)
{
	Screen->Cursor = crHourGlass;

	if (PrgBar) {
		UnicodeString msg = s? s : null_TCHAR;
		if (MsgIndex>=0 && MsgIndex<SttBar->Panels->Count) SttBar->Panels->Items[MsgIndex]->Text = msg;
		SttBar->Panels->Items[PrgIndex]->Text = EmptyStr;
		SttBar->Repaint();
		PrgBar->Position = 0;
		PrgBar->Visible  = true;
		PrgBar->Repaint();
	}
}

//---------------------------------------------------------------------------
//プログレスバー表示終了
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::End(UnicodeString s, int wait)
{
	Screen->Cursor = crDefault;

	if (PrgBar) {
		PrgBar->Position = PrgBar->Max;
		Sleep(wait);
		PrgBar->Visible = false;
		if (MsgIndex>=0 && MsgIndex<SttBar->Panels->Count) SttBar->Panels->Items[MsgIndex]->Text = s;
	}
}
//---------------------------------------------------------------------------
//バー設定
//  r = 0.0～1.0
//---------------------------------------------------------------------------
void __fastcall SttProgressBar::SetPosR(float r)
{
	if (PrgBar) {
		if (r>1.0) r = 1.0;
		int p = floor(PrgBar->Max * r);
		if (PrgBar->Position!=p) PrgBar->Position = p;
	}
}
//---------------------------------------------------------------------------
