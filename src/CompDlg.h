//----------------------------------------------------------------------//
// NyanFi																//
//  �����t�@�C���̔�r�_�C�A���O										//
//----------------------------------------------------------------------//
#ifndef CompDlgH
#define CompDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TFileCompDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *CmpArcCheckBox;
	TCheckBox *CmpDirCheckBox;
	TCheckBox *ReverseCheckBox;
	TCheckBox *SelMaskCheckBox;
	TCheckBox *SelOppCheckBox;
	TComboBox *AlgComboBox;
	TPanel *HashPanel;
	TPanel *IdPanel;
	TPanel *SizePanel;
	TPanel *TimePanel;
	TRadioGroup *HashRadioGroup;
	TRadioGroup *IdRadioGroup;
	TRadioGroup *SizeRadioGroup;
	TRadioGroup *TimeRadioGroup;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OptRadioGroupClick(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ���[�U�[�錾
	bool AllDirHasSize;		//���ׂĂ̑Ώۃf�B���N�g�����T�C�Y�擾�ς�

	__fastcall TFileCompDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFileCompDlg *FileCompDlg;
//---------------------------------------------------------------------------
#endif
