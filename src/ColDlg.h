//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビュアーの配色												//
//----------------------------------------------------------------------//
#ifndef ColDlgH
#define ColDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.ActnList.hpp>
#include "usr_swatch.h"
#include "TxtViewer.h"

//---------------------------------------------------------------------------
class TColorDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *DisableColAction;
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *ApplyBtn;
	TButton *Button1;
	TButton *CancelBtn;
	TButton *ExportBtn;
	TButton *InportBtn;
	TButton *OkBtn;
	TButton *OptApplyBtn;
	TButton *RefColBtn;
	TImage *SpuitImage;
	TListBox *ColorListBox;
	TPanel *SpuitPanel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, NativeInt Data, bool &CallHelp);
	void __fastcall ColorListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RefColBtnClick(TObject *Sender);
	void __fastcall SpuitImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpuitImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall InportBtnClick(TObject *Sender);
	void __fastcall ExportBtnClick(TObject *Sender);
	void __fastcall OptApplyBtnClick(TObject *Sender);
	void __fastcall ApplyBtnClick(TObject *Sender);
	void __fastcall OkActionExecute(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall DisableColActionExecute(TObject *Sender);
	void __fastcall DisableColActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TStringList *ColBufList;
	UsrSwatchPanel *SwatchPanel;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this, true);
	}

public:		// ユーザー宣言
	TTxtViewer *ObjViewer;
	UnicodeString FileName;

	__fastcall TColorDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,		TMessage,		WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TColorDlg *ColorDlg;
//---------------------------------------------------------------------------
#endif
