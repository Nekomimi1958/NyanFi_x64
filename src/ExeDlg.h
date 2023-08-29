//----------------------------------------------------------------------//
// NyanFi																//
//  コマンドラインの実行												//
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

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DelItemActionExecute(TObject *Sender);
	void __fastcall DelItemActionUpdate(TObject *Sender);
	void __fastcall ClrHistoryItemClick(TObject *Sender);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ユーザー宣言
	file_rec *FileRec;
	bool inp_CurName;		//カーソル位置のファイル名を入力
	bool inp_LastCmd;		//前回のコマンドを入力

	__fastcall TExeCmdDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TExeCmdDlg *ExeCmdDlg;
//---------------------------------------------------------------------------
#endif
