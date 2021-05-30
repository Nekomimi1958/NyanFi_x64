//----------------------------------------------------------------------//
// NyanFi																//
//  設定項目の編集ダイアログ											//
//----------------------------------------------------------------------//
#pragma hdrstop
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
	set_ListBoxItemHi(ItemListBox);
	UserModule->InitializeListBox(ItemListBox);
	SetDarkWinTheme(this, true);

	ItemEdit->Text = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	RetStr = EmptyStr;
	if (ModalResult==mrOk) {
		UnicodeString tit = ItemEdit->EditLabel->Caption;
		if (remove_top_s(tit, "パスマスク")) {
			UnicodeString dsc = get_tkn(get_tkn_r(tit, ":"), ":");
			if (!dsc.IsEmpty()) RetStr.cat_sprintf(_T(":%s:"), dsc.c_str());
		}

		for (int i=0; i<ItemListBox->Count; i++) {
			if (i>0) RetStr += ItemDelimiter;
			RetStr += ItemListBox->Items->Strings[i];
		}
		if (USAME_TS(ItemDelimiter, ".")) RetStr.Insert(".", 1);
	}
}

//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::ItemListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(Rect.Left + ScaledIntX(2), Rect.Top + get_TopMargin(cv), lp->Items->Strings[Index]);
}

//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::MakeList(UnicodeString s, UnicodeString delimiter, TStringList *lst)
{
	TStringDynArray i_lst = SplitString(s, ItemDelimiter);
	for (int i=0; i<i_lst.Length; i++) if (!i_lst[i].IsEmpty()) lst->Add(i_lst[i]);
}

//---------------------------------------------------------------------------
void __fastcall TEditItemDlg::AssignItems(TControl *cp)
{
	UnicodeString tit, lbuf;
	if (class_is_CustomEdit(cp)) {
		if (class_is_LabeledEdit(cp)) tit = ((TLabeledEdit *)cp)->EditLabel->Caption;
		lbuf = ((TCustomEdit *)cp)->Text;
	}
	else if (class_is_ComboBox(cp)) {
		//一番近いラベルを見つける
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
					tit = lp->Caption;
					min_s = ss;
				}
			}
		}
		lbuf = ((TComboBox *)cp)->Text;
	}

	if (tit.IsEmpty() && Screen->ActiveForm)
		tit = (Screen->ActiveForm!=Application->MainForm)? Screen->ActiveForm->Caption : EmptyStr;

	UnicodeString dsc = split_dsc(lbuf);
	if (StartsStr("パスマスク", tit) && !dsc.IsEmpty()) tit.cat_sprintf(_T(" :%s:"), dsc.c_str());
	ItemEdit->EditLabel->Caption = tit;

	ItemDelimiter = StartsStr('.', cp->Hint)? "." : ";";
	std::unique_ptr<TStringList> lst(new TStringList());
	MakeList(lbuf, ItemDelimiter, lst.get());
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
			msgbox_WARN(LoadUsrMsg(USTR_Registered));
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

