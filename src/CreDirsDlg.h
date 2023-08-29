//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリ一括作成ダイアログ										//
//----------------------------------------------------------------------//
#ifndef CreDirsDlgH
#define CreDirsDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TCreateDirsDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddDateAction;
	TAction *AddSerAction;
	TAction *AddStrAction;
	TAction *CreateAction;
	TAction *LoadListAction;
	TAction *SaveListAction;
	TAction *UndoAction;
	TActionList *ActionList1;
	TButton *AddDateBtn;
	TButton *AddSerBtn;
	TButton *AddStrBtn;
	TButton *CancelBtn;
	TButton *CreateBtn;
	TEdit *StrEdit;
	TEditPaste *EditPaste1;
	TGroupBox *DateGroupBox;
	TGroupBox *SirealGroupBox;
	TGroupBox *StrGroupBox;
	TLabel *Label1;
	TLabeledEdit *DtFmtEdit;
	TLabeledEdit *IncEdit;
	TLabeledEdit *StartEdit;
	TMaskEdit *DateMaskEdit;
	TMemo *ListMemo;
	TMenuItem *AddEmptyItem;
	TMenuItem *ClearListItem;
	TMenuItem *GetSubDirItem;
	TMenuItem *LoadListItem;
	TMenuItem *PasteItem;
	TMenuItem *RepCharItem;
	TMenuItem *SaveListItem;
	TMenuItem *Sep_p_1;
	TMenuItem *Sep_p_2;
	TMenuItem *UndoItem;
	TPanel *MainPanel;
	TPanel *Panel1;
	TPopupMenu *PopupMenu1;
	TRadioButton *PreDateRadioBtn;
	TRadioButton *PreSerRadioBtn;
	TRadioButton *PreStrRadioBtn;
	TRadioButton *PstDateRadioBtn;
	TRadioButton *PstSerRadioBtn;
	TRadioButton *PstStrRadioBtn;
	TStatusBar *StatusBar1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall AddSerActionExecute(TObject *Sender);
	void __fastcall AddSerActionUpdate(TObject *Sender);
	void __fastcall AddStrActionExecute(TObject *Sender);
	void __fastcall AddStrActionUpdate(TObject *Sender);
	void __fastcall AddDateActionExecute(TObject *Sender);
	void __fastcall AddDateActionUpdate(TObject *Sender);
	void __fastcall AddEmptyItemClick(TObject *Sender);
	void __fastcall GetSubDirItemClick(TObject *Sender);
	void __fastcall RepCharItemClick(TObject *Sender);
	void __fastcall UndoActionExecute(TObject *Sender);
	void __fastcall UndoActionUpdate(TObject *Sender);
	void __fastcall SaveListActionExecute(TObject *Sender);
	void __fastcall SaveListActionUpdate(TObject *Sender);
	void __fastcall LoadListActionExecute(TObject *Sender);
	void __fastcall ClearListItemClick(TObject *Sender);
	void __fastcall ListMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CreateActionExecute(TObject *Sender);
	void __fastcall CreateActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString UndoBuf;
	UnicodeString RefDir;

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && IsDarkMode && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

public:		// ユーザー宣言
	__fastcall TCreateDirsDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCreateDirsDlg *CreateDirsDlg;
//---------------------------------------------------------------------------
#endif
