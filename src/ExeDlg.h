//----------------------------------------------------------------------//
// NyanFi																//
//  コマンドライン実行													//
//----------------------------------------------------------------------//
#ifndef ExeDlgH
#define ExeDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>
#include "Global.h"

//---------------------------------------------------------------------------
class TExeCmdDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *DelItemAction;
	TAction *OkAction;
	TAction *RefCurFnamAction;
	TAction *RefCurPnamAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefOutBtn;
	TCheckBox *CopyStdOutCheckBox;
	TCheckBox *ListStdOutCheckBox;
	TCheckBox *LogStdOutCheckBox;
	TCheckBox *RunAsCheckBox;
	TCheckBox *SaveStdOutCheckBox;
	TCheckBox *UacDlgCheckBox;
	TComboBox *ExeComboBox;
	TEdit *SaveNameEdit;
	TGroupBox *OptGroupBox;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ClrHistoryItemClick(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall DelItemActionExecute(TObject *Sender);
	void __fastcall DelItemActionUpdate(TObject *Sender);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormCreate(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	file_rec *FileRec;
	bool inp_CurName;		//カーソル位置のファイル名を入力
	bool inp_LastCmd;		//前回のコマンドを入力

	__fastcall TExeCmdDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TExeCmdDlg *ExeCmdDlg;
//---------------------------------------------------------------------------
#endif
