//----------------------------------------------------------------------//
// NyanFi																//
//  ツールバーの設定													//
//----------------------------------------------------------------------//
#ifndef BtnDlgH
#define BtnDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TToolBtnDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddBtnAction;
	TAction *ChgBtnAction;
	TAction *EditFileAction;
	TAction *InsBtnAction;
	TActionList *ActionList1;
	TButton *AddItemBtn;
	TButton *CanButton;
	TButton *ChgItemBtn;
	TButton *DelItemBtn;
	TButton *DowItemBtn;
	TButton *EditBtn;
	TButton *InsItemBtn;
	TButton *OkButton;
	TButton *RefCmdsBtn;
	TButton *RefIconBtn;
	TButton *UpItemBtn;
	TComboBox *BtnCmdsComboBox;
	TImage *IcoImage;
	TLabel *CmdLabel;
	TLabeledEdit *CaptionEdit;
	TLabeledEdit *IconEdit;
	TListBox *BtnListBox;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall BtnListBoxClick(TObject *Sender);
	void __fastcall RefCmdsBtnClick(TObject *Sender);
	void __fastcall EditFileActionExecute(TObject *Sender);
	void __fastcall EditFileActionUpdate(TObject *Sender);
	void __fastcall AddBtnActionExecute(TObject *Sender);
	void __fastcall AddBtnActionUpdate(TObject *Sender);
	void __fastcall ChgBtnActionExecute(TObject *Sender);
	void __fastcall ChgBtnActionUpdate(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall RefIconBtnClick(TObject *Sender);
	void __fastcall IconEditChange(TObject *Sender);
	void __fastcall BtnListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall InsBtnActionExecute(TObject *Sender);
	void __fastcall BtnCmdsComboBoxChange(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, NativeInt Data, bool &CallHelp);

private:	// ユーザー宣言
	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this, true);
	}

	void __fastcall WmDropped(TMessage &msg);
	void __fastcall WmNyanFiFlIcon(TMessage &msg) { BtnListBox->Invalidate(); }

	UnicodeString __fastcall MakeCsvItem();

public:		// ユーザー宣言
	TStringList *ButtonList;
	int IniIndex;		//開いたときの項目選択を指定

	__fastcall TToolBtnDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,		TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,	WmDropped)
		VCL_MESSAGE_HANDLER(WM_NYANFI_FLICON,	TMessage,	WmNyanFiFlIcon)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TToolBtnDlg *ToolBtnDlg;
//---------------------------------------------------------------------------
#endif
