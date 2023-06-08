//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリの比較ダイアログ										//
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
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TDiffDirDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
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

private:	// ユーザー宣言
	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
	__fastcall TDiffDirDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TDiffDirDlg *DiffDirDlg;
//---------------------------------------------------------------------------
#endif
