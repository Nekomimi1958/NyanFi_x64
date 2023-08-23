//----------------------------------------------------------------------//
// NyanFi																//
//  重複ファイルの検索ダイアログ										//
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
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TFindDuplDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *ExcSymCheckBox;
	TCheckBox *OppPathCheckBox;
	TCheckBox *ResLinkCheckBox;
	TCheckBox *SubDirCheckBox;
	TComboBox *AlgComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabeledEdit *MaxSizeEdit;
	TPanel *BottomPanel;
	TPanel *OptPanel;
	TPanel *TopPanel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// ユーザー宣言
	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ユーザー宣言
	__fastcall TFindDuplDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFindDuplDlg *FindDuplDlg;
//---------------------------------------------------------------------------
#endif
