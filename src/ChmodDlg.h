//----------------------------------------------------------------------//
// NyanFi																//
//  �p�[�~�b�V�����ݒ�													//
//----------------------------------------------------------------------//
#ifndef ChmodDlgH
#define ChmodDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TFtpChmodDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *R1CheckBox;
	TCheckBox *R2CheckBox;
	TCheckBox *R3CheckBox;
	TCheckBox *W1CheckBox;
	TCheckBox *W2CheckBox;
	TCheckBox *W3CheckBox;
	TCheckBox *X1CheckBox;
	TCheckBox *X2CheckBox;
	TCheckBox *X3CheckBox;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TLabel *Label1;
	TMaskEdit *OctalEdit;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PrmCheckBoxClick(TObject *Sender);
	void __fastcall OctalEditChange(TObject *Sender);

private:	// ���[�U�[�錾
	bool InhChange;

public:		// ���[�U�[�錾
	__fastcall TFtpChmodDlg(TComponent* Owner);
	bool __fastcall SetPermission(UnicodeString prm);
};
//---------------------------------------------------------------------------
extern PACKAGE TFtpChmodDlg *FtpChmodDlg;
//---------------------------------------------------------------------------
#endif
