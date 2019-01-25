//----------------------------------------------------------------------//
// NyanFi																//
//  設定項目の編集ダイアログ											//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <System.StrUtils.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "EditItem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TEditItemDlg *EditItemDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TEditItemDlg::TEditItemDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::FormShow(TObject *Sender)
{
	TListBox *lp = ItemListBox;
	set_ListBoxItemHi(lp);
	UserModule->InitializeListBox(lp);

	ItemEdit->Text = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	RetStr = EmptyStr;
	if (ModalResult==mrOk) {
		for (int i=0; i<ItemListBox->Count; i++) {
			if (i>0) RetStr += ItemDelimiter;
			RetStr += ItemListBox->Items->Strings[i];
		}
		if (USAME_TS(ItemDelimiter, ".")) RetStr.Insert(".", 1);
	}
}

//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::MakeList(UnicodeString s, UnicodeString delimiter, TStringList *lst)
{
	TStringDynArray i_lst = SplitString(s, ItemDelimiter);
	for (int i=0; i<i_lst.Length; i++) if (!i_lst[i].IsEmpty()) lst->Add(i_lst[i]);
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::AssignItems(TCustomEdit *ep)
{
	ItemEdit->EditLabel->Caption = class_is_LabeledEdit(ep)? ((TLabeledEdit *)ep)->EditLabel->Caption : EmptyStr;
	if (ItemEdit->EditLabel->Caption.IsEmpty() && Screen->ActiveForm)
		ItemEdit->EditLabel->Caption = (Screen->ActiveForm!=Application->MainForm)? Screen->ActiveForm->Caption : EmptyStr;

	ItemDelimiter = StartsStr('.', ep->Hint)? "." : ";";
	std::unique_ptr<TStringList> lst(new TStringList());
	MakeList(ep->Text, ItemDelimiter, lst.get());
	ItemListBox->Items->Assign(lst.get());
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::AssignItems(TComboBox *cp)
{
	//一番近いラベルを見つける
	ItemEdit->EditLabel->Caption = EmptyStr;
	TWinControl *wp = cp->Parent;
	if (wp) {
		double min_s = 9999;
		for (int i=0; i<wp->ControlCount; i++) {
			if (!wp->Controls[i]->ClassNameIs("TLabel")) continue;
			TLabel *lp = (TLabel *)wp->Controls[i];
			if (!lp->Visible) continue;
			double dx = lp->Top - cp->Top;
			double dy = lp->Left - cp->Left;
			double ss = sqrt(dx*dx + dy*dy);
			if (ss<min_s) {
				ItemEdit->EditLabel->Caption = lp->Caption;
				min_s = ss;
			}
		}
	}

	if (ItemEdit->EditLabel->Caption.IsEmpty() && Screen->ActiveForm)
		ItemEdit->EditLabel->Caption = Screen->ActiveForm->Caption;

	ItemDelimiter = StartsStr('.', cp->Hint)? "." : ";";
	std::unique_ptr<TStringList> lst(new TStringList());
	MakeList(cp->Text, ItemDelimiter, lst.get());
	ItemListBox->Items->Assign(lst.get());
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::AddItemActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	MakeList(ItemEdit->Text, ItemDelimiter, lst.get());

	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (ItemListBox->Items->IndexOf(lbuf)!=-1) {
			msgbox_WARN(USTR_Registered);
			continue;
		}
		ItemListBox->Items->Add(lbuf);
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !ItemEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::ChgItemActionExecute(TObject *Sender)
{
	int idx = ItemListBox->ItemIndex;
	if (idx==-1) return;

	std::unique_ptr<TStringList> lst(new TStringList());
	MakeList(ItemEdit->Text, ItemDelimiter, lst.get());
	if (lst->Count>0) ItemListBox->Items->Strings[idx] = lst->Strings[0];
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::ChgItemActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (ItemListBox->ItemIndex!=-1) && !ItemEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//ソート
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::SortItemActionExecute(TObject *Sender)
{
	ItemListBox->Sorted = true;
	Application->ProcessMessages();
	ItemListBox->Sorted = false;
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::SortItemActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (ItemListBox->Count>0);
}

//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::ItemListBoxClick(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	if (lp->ItemIndex!=-1) ItemEdit->Text = lp->Items->Strings[lp->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::ItemListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}
//---------------------------------------------------------------------------

