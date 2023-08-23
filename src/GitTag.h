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
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TSetGitTagDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CancelBtn;
	TButton *OkBtn;
	TCheckBox *AnnotateCheckBox;
	TLabel *Label1;
	TLabeledEdit *NameEdit;
	TMemo *AnnotateMemo;
	TPanel *CommitPanel;
	TPanel *MsgPanel;
	TPanel *TagPanel;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// ユーザー宣言

public:		// ユーザー宣言
	bool IsCommit;				//true=コミット / false=タグ
	UnicodeString CommitID;
	UnicodeString EditMsgFile;	//メッセージ編集用ファイル
	UnicodeString GitParam;		//[o] git実行用パラメータ

	__fastcall TSetGitTagDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetGitTagDlg *SetGitTagDlg;
//---------------------------------------------------------------------------
#endif
