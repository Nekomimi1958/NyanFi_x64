//----------------------------------------------------------------------//
// インスペクタ															//
//																		//
//----------------------------------------------------------------------//
#ifndef InspectFrmH
#define InspectFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ToolWin.hpp>
#include "usr_scrpanel.h"

//-----------------------------------------------
class TInspectForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *BigEndianAction;
	TAction *UnsignedAction;
	TActionList *ActionList1;
	THeaderControl *CodePageHeader;
	THeaderControl *InspectHeader;
	TMenuItem *CopyItem;
	TPanel *ClientPanel;
	TPanel *CodePanel;
	TPanel *GridPanel;
	TPanel *InspectPanel;
	TPopupMenu *PopupMenu1;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TShape *BorderShape3;
	TSplitter *GridSplitter;
	TStatusBar *InsStatusBar;
	TStringGrid *CodePageGrid;
	TStringGrid *InspectGrid;
	TToolBar *OptToolBar;
	TToolButton *BeButton;
	TToolButton *UsButton;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InsStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall InspectHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall InspectHeaderResize(TObject *Sender);
	void __fastcall InspectHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall CodePageHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall CodePageHeaderResize(TObject *Sender);
	void __fastcall CodePageHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall OptCheckBoxClick(TObject *Sender);
	void __fastcall InspectGridDrawCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall CopyItemClick(TObject *Sender);
	void __fastcall UnsignedActionExecute(TObject *Sender);
	void __fastcall BigEndianActionExecute(TObject *Sender);

private:	// ユーザー宣言
	TColor ColorRef;

	TWndMethod org_InsSttBarWndProc;
	void __fastcall InsSttBarWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(InsStatusBar, msg)) return;
		org_InsSttBarWndProc(msg);
	}

public:		// ユーザー宣言
	UsrScrollPanel *InspectScrPanel;	//シンプルスクロールバー
	UsrScrollPanel *CodeScrPanel;

	__int64 Address;	//アドレス
	TBytes  Bytes;		//バイトデータ

	__fastcall TInspectForm(TComponent* Owner);

	void __fastcall UpdateValue();
};
//---------------------------------------------------------------------------
extern PACKAGE TInspectForm *InspectForm;
//---------------------------------------------------------------------------
#endif
