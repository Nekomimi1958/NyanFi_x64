//----------------------------------------------------------------------//
// NyanFi																//
//  同期コピーの設定													//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "SyncDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegSyncDlg *RegSyncDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TRegSyncDlg::TRegSyncDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::FormShow(TObject *Sender)
{
	set_ListBoxItemHi(RegListBox);
	set_ListBoxItemHi(DirListBox);

	TCheckListBox *lp = RegListBox;
	lp->Items->Assign(SyncDirList);
	for (int i=0; i<lp->Count; i++) lp->Checked[i] = !equal_0(get_csv_item(lp->Items->Strings[i], 1));
	lp->ItemIndex = -1;
	RegListBoxClick(lp);
	UserModule->InitializeListBox(lp);

	InitDir = CurPathName;

	SetDarkWinTheme(this, true);
}
//---------------------------------------------------------------------------
//一覧の表示
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::RegListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + ScaledIntX(2);
	int yp = Rect.Top  + get_TopMargin(cv);

	int w_x = 50;
	for (int i=0; i<lp->Count; i++) w_x = std::max(cv->TextWidth(get_csv_item(lp->Items->Strings[i], 0)), w_x);
	TStringDynArray syn_lst = get_csv_array(lp->Items->Strings[Index], 50, true);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(xp, yp, syn_lst[0]);	xp += w_x + 16;

	if (ContainsText(syn_lst[2], "O")) out_Text(cv, xp, yp, _T("O"));
	xp += 18;
	if (ContainsText(syn_lst[2], "D")) out_Text(cv, xp, yp, _T("D"));
	xp += 24;

	UnicodeString lbuf;
	for (int i=3; i<syn_lst.Length && !syn_lst[i].IsEmpty(); i++) {
		if (!lbuf.IsEmpty()) lbuf += ", ";
		lbuf += syn_lst[i];
	}
	cv->TextOut(xp, yp, lbuf);
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::DirListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(Rect.Left + ScaledIntX(2), Rect.Top + get_TopMargin(cv),
				get_MiniPathName(lp->Items->Strings[Index], lp->ClientWidth, lp->Font));
}

//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::RegListBoxClick(TObject *Sender)
{
	NameEdit->Text = EmptyStr;
	DirListBox->Clear();

	TCheckListBox *lp = (TCheckListBox*)Sender;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray syn_lst = get_csv_array(lp->Items->Strings[idx], 50);
		if (syn_lst.Length>2) {
			NameEdit->Text = syn_lst[0];
			SyncOwrCheckBox->Checked = ContainsText(syn_lst[2], "O");
			SyncDelCheckBox->Checked = ContainsText(syn_lst[2], "D");
			for (int i=3; i<syn_lst.Length; i++) DirListBox->Items->Add(syn_lst[i]);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::SyncListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Shift.Empty() && Key==VK_DELETE) {
		TControl *cp = (TControl*)Sender;
		if		(cp==RegListBox) DelRegAction->Execute();
		else if (cp==DirListBox) DelDirAction->Execute();
		Key = 0;
	}
}

//---------------------------------------------------------------------------
//登録項目文字列を作成
//---------------------------------------------------------------------------
UnicodeString __fastcall TRegSyncDlg::MakeRegItem(int idx)
{
	UnicodeString ret_str = NameEdit->Text;
	if (ret_str.IsEmpty()) ret_str.sprintf(_T("登録%u"), RegListBox->Count + 1);
	ret_str = make_csv_str(ret_str);
	ret_str.cat_sprintf(_T("%s"), (idx!=-1 && RegListBox->Checked[idx])? _T(",\"1\"") : _T(",\"0\""));
	UnicodeString opt;
	if (SyncOwrCheckBox->Checked) opt += "O";
	if (SyncDelCheckBox->Checked) opt += "D";
	ret_str.cat_sprintf(_T(",\"%s\""), opt.c_str());

	for (int i=0; i<DirListBox->Count; i++)
		ret_str.cat_sprintf(_T(",\"%s\""), DirListBox->Items->Strings[i].c_str());
	return ret_str;
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::RegListBoxClickCheck(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray syn_lst = get_csv_array(lp->Items->Strings[idx], 50);
		if (syn_lst.Length>=2) {
			syn_lst[1].sprintf(_T("%s"), lp->Checked[idx]? _T("1") : _T("0"));
			lp->Items->Strings[idx] = make_csv_rec_str(syn_lst);
		}
	}
}

//---------------------------------------------------------------------------
//登録の追加
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::AddRegActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	lp->Items->Add(MakeRegItem());
	lp->ItemIndex = lp->Count - 1;
	RegListBoxClick(RegListBox);
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::AddRegActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (DirListBox->Count>=2);
}

//---------------------------------------------------------------------------
//登録の変更
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::ChgRegActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) lp->Items->Strings[idx] = MakeRegItem(idx);
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::ChgRegActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (RegListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//登録の削除
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::DelRegActionExecute(TObject *Sender)
{
	delete_ListItem(RegListBox);
	RegListBoxClick(RegListBox);
}

//---------------------------------------------------------------------------
//ディレクトリの追加
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::AddDirActionExecute(TObject *Sender)
{
	if (UserModule->SelectDirEx(_T("同期コピー先の追加"), InitDir)) {
		InitDir = to_path_name(InitDir);
		if (DirListBox->Items->IndexOf(InitDir)==-1)
			DirListBox->ItemIndex = DirListBox->Items->Add(InitDir);
		else
			msgbox_WARN(LoadUsrMsg(USTR_Registered));
	}
}
//---------------------------------------------------------------------------
//ディレクトリの削除
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::DelDirActionExecute(TObject *Sender)
{
	delete_ListItem(DirListBox);
}
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::DelDirActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (DirListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//ディレクトリのクリア
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::ClrDirActionExecute(TObject *Sender)
{
	RegListBox->ItemIndex = -1;
	NameEdit->Text = EmptyStr;
	DirListBox->Clear();
}

//---------------------------------------------------------------------------
//確定
//---------------------------------------------------------------------------
void __fastcall TRegSyncDlg::OkButtonClick(TObject *Sender)
{
	SyncDirList->Assign(RegListBox->Items);

	int i = 0;
	while (i<SyncDirList->Count) {
		TStringDynArray syn_lst = get_csv_array(SyncDirList->Strings[i], 50);	//***
		//不正データをはねる
		if (syn_lst.Length<5) {
			SyncDirList->Delete(i);
		}
		//正規化
		else {
			//"タイトル","有効:1/無効:0","オプション","dir1","dir2",...
			UnicodeString lbuf;
			for (int j=0; j<syn_lst.Length; j++) {
				if (j>0)  lbuf += ",";
				if (j>=3) syn_lst[j] = IncludeTrailingPathDelimiter(syn_lst[j]);
				lbuf += make_csv_str(syn_lst[j]);
			}
			SyncDirList->Strings[i] = lbuf;
			i++;
		}
	}
}
//---------------------------------------------------------------------------
