//----------------------------------------------------------------------//
// NyanFi																//
//  Gitタグ設定															//
//----------------------------------------------------------------------//
#ifndef GitTagH
#define GitTagH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TSetGitTagDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CancelBtn;
	TButton *OkBtn;
	TCheckBox *AnnotateCheckBox;
	TLabeledEdit *NameEdit;
	TMemo *AnnotateMemo;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	UnicodeString CommitID;
	UnicodeString TagMsgFile;
	UnicodeString GitParam;

	__fastcall TSetGitTagDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetGitTagDlg *SetGitTagDlg;
//---------------------------------------------------------------------------
#endif
