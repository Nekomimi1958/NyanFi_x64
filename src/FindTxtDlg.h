//----------------------------------------------------------------------//
// NyanFi																//
//  文字列検索(テキストビュアー)										//
//----------------------------------------------------------------------//
#ifndef FindTxtDlgH
#define FindTxtDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include "ExTViewer.h"
#include "TxtViewer.h"

//---------------------------------------------------------------------------
class TFindTextDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *FindNextAction;
	TActionList *ActionList1;
	TButton *CalcelBtn;
	TButton *FindBtn;
	TCheckBox *BytesCheckBox;
	TCheckBox *CaseCheckBox;
	TCheckBox *CloseCheckBox;
	TCheckBox *HighlightCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *RegExCheckBox;
	TComboBox *CodePageComboBox;
	TComboBox *FindComboBox;
	TLabel *KwdLabel;
	TRadioGroup *UpDownGroup;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SubOptClick(TObject *Sender);
	void __fastcall FindComboBoxChange(TObject *Sender);
	void __fastcall FindComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FindOptChangedClick(TObject *Sender);
	void __fastcall FindNextActionUpdate(TObject *Sender);
	void __fastcall FindNextActionExecute(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegExCheckBoxClick(TObject *Sender);
	void __fastcall MigemoCheckBoxClick(TObject *Sender);

private:	// ユーザー宣言
	TTxtViewer *Viewer;
	bool DlgInitialized;

public:		// ユーザー宣言
	bool fromTV;		//テキストビュアーから

	TExTxtViewer *ExTViewer;

	UnicodeString RegExPtn;

	__fastcall TFindTextDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindTextDlg *FindTextDlg;
//---------------------------------------------------------------------------
#endif
