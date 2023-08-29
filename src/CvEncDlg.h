//----------------------------------------------------------------------//
// NyanFi																//
//  �����R�[�h�̕ϊ�													//
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
__published:	// IDE �ŊǗ������R���|�[�l���g
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

private:	// ���[�U�[�錾
	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

public:		// ���[�U�[�錾
	UnicodeString TitleInf;

	__fastcall TCvTxtEncDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCvTxtEncDlg *CvTxtEncDlg;
//---------------------------------------------------------------------------
#endif
