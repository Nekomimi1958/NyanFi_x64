//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリ履歴/ スタック											//
//----------------------------------------------------------------------//
#ifndef HistDlgH
#define HistDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TDirHistoryDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddDirsAction;
	TAction *ClearAllAction;
	TAction *ClearFltAction;
	TAction *CopyAllAction;
	TAction *PropertyAction;
	TActionList *ActionList1;
	TLabel *HintLabel1;
	TLabel *HintLabel2;
	TListBox *DirHistListBox;
	TMenuItem *AddDirsItem;
	TMenuItem *ClearAllItem;
	TMenuItem *ClearFltItem;
	TMenuItem *CopyAllItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *PropertyItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TPaintBox *InpPaintBox;
	TPanel *HintPanel;
	TPanel *InpPanel;
	TPanel *ListPanel;
	TPopupMenu *HistPopupMenu;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DirHistListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DirHistListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall DirHistListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DirHistListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall DirHistListBoxDblClick(TObject *Sender);
	void __fastcall InpPaintBoxPaint(TObject *Sender);
	void __fastcall AddDirsActionExecute(TObject *Sender);
	void __fastcall AddDirsActionUpdate(TObject *Sender);
	void __fastcall ClearAllActionExecute(TObject *Sender);
	void __fastcall ClearAllActionUpdate(TObject *Sender);
	void __fastcall ClearFltActionExecute(TObject *Sender);
	void __fastcall ClearFltActionUpdate(TObject *Sender);
	void __fastcall CopyAllActionExecute(TObject *Sender);
	void __fastcall CopyAllActionUpdate(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TStringList *ListBuff;

	UnicodeString IncSeaWord;
	bool IsMigemo;

	bool FindBusy;
	bool FindAborted;

	void __fastcall UpdateListBox(int idx = 0);
	void __fastcall AddDirsCore(UnicodeString pnam, TStrings *lst, TStringDynArray ex_lst);

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	bool IsDirStack;		//ディレクトリ・スタック
	bool IsAllDirHist;		//全体履歴
	bool IsFindDirHist;		//全体履歴(インクリメンタルサーチ)
	bool IdRecentDir;		//最近使ったディレクトリ

	int  RefListTag;
	UnicodeString WorkListName;

	__fastcall TDirHistoryDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDirHistoryDlg *DirHistoryDlg;
//---------------------------------------------------------------------------
#endif
