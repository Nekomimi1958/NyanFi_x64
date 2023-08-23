//----------------------------------------------------------------------//
// NyanFi																//
//  同期コピーの設定													//
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
__published:	// IDE で管理されるコンポーネント
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

private:	// ユーザー宣言
	UnicodeString InitDir;

	UnicodeString __fastcall MakeRegItem(int idx = -1);

public:		// ユーザー宣言
	__fastcall TRegSyncDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRegSyncDlg *RegSyncDlg;
//---------------------------------------------------------------------------
#endif
