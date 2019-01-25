//----------------------------------------------------------------------//
// NyanFi																//
//  �f�B���N�g���̔�r�_�C�A���O										//
//----------------------------------------------------------------------//
#ifndef DiffDlgH
#define DiffDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TDiffDirDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *StartAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *SubDirCheckBox;
	TComboBox *DiffExcDirComboBox;
	TComboBox *ExcMaskComboBox;
	TComboBox *IncMaskComboBox;
	TLabel *Label1;
	TLabeledEdit *DstDirEdit;
	TLabeledEdit *SrcDirEdit;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall StartActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	__fastcall TDiffDirDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDiffDirDlg *DiffDirDlg;
//---------------------------------------------------------------------------
#endif
