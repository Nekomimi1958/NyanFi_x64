//----------------------------------------------------------------------//
// NyanFi																//
//  設定項目の編集ダイアログ											//
//----------------------------------------------------------------------//
#ifndef EditItemH
#define EditItemH

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
class TEditItemDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddItemAction;
	TAction *ChgItemAction;
	TAction *SortItemAction;
	TActionList *ActionList1;
	TButton *AddItemBtn;
	TButton *CanButton;
	TButton *ChgItemBtn;
	TButton *DelItemBtn;
	TButton *DownItemBtn;
	TButton *OkButton;
	TButton *SortItemBtn;
	TButton *UpItemBtn;
	TLabeledEdit *ItemEdit;
	TListBox *ItemListBox;
	TPanel *Panel1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall ItemListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall AddItemActionUpdate(TObject *Sender);
	void __fastcall ChgItemActionExecute(TObject *Sender);
	void __fastcall ChgItemActionUpdate(TObject *Sender);
	void __fastcall SortItemActionExecute(TObject *Sender);
	void __fastcall SortItemActionUpdate(TObject *Sender);
	void __fastcall ItemListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ItemListBoxClick(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString ItemDelimiter;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this, true);
	}

public:		// ユーザー宣言
	UnicodeString RetStr;

	void __fastcall MakeList(UnicodeString s, UnicodeString delimiter, TStringList *lst);
	void __fastcall AssignItems(TControl *cp);
	void __fastcall AssignText(UnicodeString tit, UnicodeString s, UnicodeString delimiter = ";");

	__fastcall TEditItemDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TEditItemDlg *EditItemDlg;
//---------------------------------------------------------------------------
#endif
