//----------------------------------------------------------------------//
// NyanFi																//
//  アーカイブ作成ダイアログ											//
//----------------------------------------------------------------------//
#ifndef PackDlgH
#define PackDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TPackArcDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *IncDirCheckBox;
	TCheckBox *PerDirCheckBox;
	TCheckBox *SfxCheckBox;
	TCheckBox *SureSameCheckBox;
	TComboBox *ParamComboBox;
	TGroupBox *GroupBox1;
	TLabel *FextLabel;
	TLabel *Label1;
	TLabeledEdit *ArcNameEdit;
	TLabeledEdit *ExSwEdit;
	TLabeledEdit *PasswordEdit;
	TRadioGroup *FormatRadioGroup;
	TRadioGroup *SameRadioGroup;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormatRadioGroupClick(TObject *Sender);
	void __fastcall SfxCheckBoxClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PerDirCheckBoxClick(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	__fastcall TPackArcDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPackArcDlg *PackArcDlg;
//---------------------------------------------------------------------------
#endif
