//----------------------------------------------------------------------//
// NyanFi																//
//  文字コードの変換													//
//----------------------------------------------------------------------//
#ifndef CvEncDlgH
#define CvEncDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TCvTxtEncDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *BomCheckBox;
	TComboBox *OutCodeComboBox;
	TComboBox *OutLnBrkComboBox;
	TLabel *Label1;
	TLabel *Label2;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OutCodeComboBoxClick(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	UnicodeString TitleInf;

	__fastcall TCvTxtEncDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCvTxtEncDlg *CvTxtEncDlg;
//---------------------------------------------------------------------------
#endif
