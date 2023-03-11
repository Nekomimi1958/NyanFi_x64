// NyanFi																//
//  �V�K�t�@�C���̍쐬�_�C�A���O										//
//----------------------------------------------------------------------//
#ifndef NewDlgH
#define NewDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TNewFileDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *OkAction;
	TActionList *OK;
	TButton *CancelBtn;
	TButton *OkBtn;
	TButton *RefTplBtn;
	TComboBox *TplComboBox;
	TLabel *Label1;
	TLabeledEdit *NewExeCmdEdit;
	TLabeledEdit *NewNameEdit;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TplComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RefTplBtnClick(TObject *Sender);
	void __fastcall TplComboBoxClick(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	UnicodeString TplRefDir;

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ���[�U�[�錾
	__fastcall TNewFileDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TNewFileDlg *NewFileDlg;
//---------------------------------------------------------------------------
#endif
