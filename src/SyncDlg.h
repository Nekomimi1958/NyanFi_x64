//----------------------------------------------------------------------//
// NyanFi																//
//  �����R�s�[�̐ݒ�													//
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
#include <Vcl.Mask.hpp>

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
	void __fastcall RegListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DirListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RegListBoxClick(TObject *Sender);
	void __fastcall SyncListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegListBoxClickCheck(TObject *Sender);
	void __fastcall AddRegActionExecute(TObject *Sender);
	void __fastcall AddRegActionUpdate(TObject *Sender);
	void __fastcall ChgRegActionExecute(TObject *Sender);
	void __fastcall ChgRegActionUpdate(TObject *Sender);
	void __fastcall DelRegActionExecute(TObject *Sender);
	void __fastcall AddDirActionExecute(TObject *Sender);
	void __fastcall DelDirActionExecute(TObject *Sender);
	void __fastcall DelDirActionUpdate(TObject *Sender);
	void __fastcall ClrDirActionExecute(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);

private:	// ���[�U�[�錾
	UnicodeString InitDir;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

	UnicodeString __fastcall MakeRegItem(int idx = -1);

public:		// ���[�U�[�錾
	__fastcall TRegSyncDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TRegSyncDlg *RegSyncDlg;
//---------------------------------------------------------------------------
#endif
