//----------------------------------------------------------------------//
// NyanFi																//
//  ソートダイアログ													//
//----------------------------------------------------------------------//
#ifndef SrtModDlgH
#define SrtModDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TSortModeDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *DscAttrCheckBox;
	TCheckBox *DscNameCheckBox;
	TCheckBox *NaturalCheckBox;
	TCheckBox *OldCheckBox;
	TCheckBox *SmallCheckBox;
	TCheckBox *SortBothCheckBox;
	TComboBox *DirSortModeComboBox;
	TGroupBox *DirSortModeGroupBox;
	TGroupBox *GroupBox1;
	TPanel *BtnPanel;
	TPanel *DirOptPanel;
	TPanel *ModePanel;
	TRadioGroup *SortModeRadioGroup;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SortModeRadioGroupClick(TObject *Sender);
	void __fastcall SortModeRadioGroupEnter(TObject *Sender);
	void __fastcall SortModeRadioGroupExit(TObject *Sender);
	void __fastcall SortBothCheckBoxClick(TObject *Sender);
	void __fastcall OptCheckBoxClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);

private:	// ユーザー宣言
	bool Changed;
	bool SelByKey;

public:		// ユーザー宣言
	bool InhOk;

	__fastcall TSortModeDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSortModeDlg *SortModeDlg;
//---------------------------------------------------------------------------
#endif
