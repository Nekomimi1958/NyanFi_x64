//----------------------------------------------------------------------//
// NyanFi																//
//  Git�^�O�ݒ�															//
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
__published:	// IDE �ŊǗ������R���|�[�l���g
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

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	bool IsCommit;				//true=�R�~�b�g / false=�^�O
	UnicodeString CommitID;
	UnicodeString EditMsgFile;	//���b�Z�[�W�ҏW�p�t�@�C��
	UnicodeString GitParam;		//[o] git���s�p�p�����[�^

	__fastcall TSetGitTagDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSetGitTagDlg *SetGitTagDlg;
//---------------------------------------------------------------------------
#endif
