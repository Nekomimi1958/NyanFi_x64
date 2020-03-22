//----------------------------------------------------------------------//
// NyanFi																//
//  置換確認ダイアログ													//
//----------------------------------------------------------------------//
#ifndef AskRepH
#define AskRepH

//----------------------------------------------------------------------------
#include <Winapi.Windows.hpp>
#include <System.hpp>
#include <System.SysUtils.hpp>
#include <System.Classes.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include "usr_scrpanel.h"

//----------------------------------------------------------------------------
class TAskRepDlg : public TForm
{
__published:
	TButton *AllBtn;
	TButton *CanBtn;
	TButton *OKBtn;
	TButton *SkipBtn;
	TListBox *RepListBox;
	TPanel *ListPanel;
	TPanel *OpPanel;
	TStatusBar *StatusBar1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall RepListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RepListBoxEnter(TObject *Sender);

private:
	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this, true);
	}

public:
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	UnicodeString FileName;
	UnicodeString KeyWord;

	int FoundLine;
	int FoundPos;

	int CurIndex;
	int FileCount;
	int FileIndex;

	virtual __fastcall TAskRepDlg(TComponent* AOwner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//----------------------------------------------------------------------------
extern PACKAGE TAskRepDlg *AskRepDlg;
//----------------------------------------------------------------------------
#endif
