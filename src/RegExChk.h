//----------------------------------------------------------------------//
// NyanFi																//
//  正規表現チェッカー													//
//----------------------------------------------------------------------//
#ifndef RegExChkH
#define RegExChkH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TRegExChecker : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CopyAction;
	TAction *CopyCAction;
	TAction *PasteAction;
	TAction *ReplaceAction;
	TAction *TestAction;
	TActionList *ActionList1;
	TButton *CCopyBtn;
	TButton *CopyBtn;
	TButton *HiddenCanBtn;
	TButton *MatchBtn;
	TButton *PasteBtn;
	TButton *RefFileBtn;
	TButton *ReplaceBtn;
	TCheckBox *CaseCheckBox;
	TCheckBox *UpdtCheckBox;
	TComboBox *PtnComboBox;
	TLabel *ResultLabel;
	TLabeledEdit *FileEdit;
	TLabeledEdit *ReplaceEdit;
	TListBox *ReferListBox;
	TListBox *ResListBox;
	TMemo *ObjMemo;
	TPanel *BevelPanel1;
	TPanel *MainPanel;
	TPanel *OpePanel;
	TPanel *ReferPanel;
	TPanel *ResPanel;
	TShape *Shape1;
	TShape *Shape2;
	TSplitter *Splitter1;
	TSplitter *Splitter2;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TestActionExecute(TObject *Sender);
	void __fastcall TestActionUpdate(TObject *Sender);
	void __fastcall ReplaceActionExecute(TObject *Sender);
	void __fastcall ReplaceActionUpdate(TObject *Sender);
	void __fastcall PasteActionExecute(TObject *Sender);
	void __fastcall PasteActionUpdate(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall ReferListBoxDblClick(TObject *Sender);
	void __fastcall RefFileBtnClick(TObject *Sender);
	void __fastcall ResListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);

private:	// ユーザー宣言
	int PtnSelStart;
	int PtnSelLength;
	bool with_Slash;

	TStringList *MatchWdList;
	int MatchCount;

public:		// ユーザー宣言
	TComboBox   *ObjComboBox;
	TCustomEdit *ObjCustomEdit;

	UnicodeString PatternStr;

	__fastcall TRegExChecker(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRegExChecker *RegExChecker;
//---------------------------------------------------------------------------
#endif
