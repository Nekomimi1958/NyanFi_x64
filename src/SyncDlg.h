//----------------------------------------------------------------------//
// NyanFi																//
//  �����R�s�[��̐ݒ�													//
//----------------------------------------------------------------------//
#ifndef SyncDlgH
#define SyncDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.CheckLst.hpp>

//---------------------------------------------------------------------------
class TRegSyncDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *AddDirAction;
	TAction *AddRegAction;
	TAction *ChgRegAction;
	TAction *ClrDirAction;
	TAction *DelDirAction;
	TAction *DelRegAction;
	TActionList *ActionList1;
	TButton *AddDirBtn;
	TButton *AddMenuBtn;
	TButton *CanButton;
	TButton *ChgMenuBtn;
	TButton *ClrDirBtn;
	TButton *DelDirBtn;
	TButton *DelMenuBtn;
	TButton *OkButton;
	TCheckBox *SyncDelCheckBox;
	TCheckBox *SyncOwrCheckBox;
	TCheckListBox *RegListBox;
	TLabeledEdit *NameEdit;
	TListBox *DirListBox;
	TPanel *OptPanel;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall RegListBoxClick(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall RegListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall AddRegActionExecute(TObject *Sender);
	void __fastcall AddRegActionUpdate(TObject *Sender);
	void __fastcall DelRegActionExecute(TObject *Sender);
	void __fastcall ChgRegActionExecute(TObject *Sender);
	void __fastcall ChgRegActionUpdate(TObject *Sender);
	void __fastcall AddDirActionExecute(TObject *Sender);
	void __fastcall DelDirActionExecute(TObject *Sender);
	void __fastcall DelDirActionUpdate(TObject *Sender);
	void __fastcall DirListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RegListBoxClickCheck(TObject *Sender);
	void __fastcall SyncListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ClrDirActionExecute(TObject *Sender);

private:	// ���[�U�[�錾
	UnicodeString InitDir;

	UnicodeString __fastcall MakeRegItem(int idx = -1);

public:		// ���[�U�[�錾
	__fastcall TRegSyncDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRegSyncDlg *RegSyncDlg;
//---------------------------------------------------------------------------
#endif
