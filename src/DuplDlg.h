//----------------------------------------------------------------------//
// NyanFi																//
//  �d���t�@�C���̌����_�C�A���O										//
//----------------------------------------------------------------------//
#ifndef DuplDlgH
#define DuplDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TFindDuplDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *ExcSymCheckBox;
	TCheckBox *OppPathCheckBox;
	TCheckBox *ResLinkCheckBox;
	TCheckBox *SubDirCheckBox;
	TLabel *Label1;
	TLabeledEdit *MaxSizeEdit;
	TRadioGroup *AlgRadioGroup;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	__fastcall TFindDuplDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindDuplDlg *FindDuplDlg;
//---------------------------------------------------------------------------
#endif
