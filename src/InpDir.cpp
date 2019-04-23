//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリを開く/ Web検索											//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <Vcl.FileCtrl.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpDir.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInpDirDlg *InpDirDlg;

//---------------------------------------------------------------------------
__fastcall TInpDirDlg::TInpDirDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	InpDirComboBox->Tag = CBTAG_HISTORY;

	ItemList = new TStringList();
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormShow(TObject *Sender)
{
	isWebSea = USAME_TI(CommandStr, "WebSearch");

	ClientHeight = TopPanel->Height + BottomPanel->Height;
	Constraints->MaxHeight = Height;
	Constraints->MinHeight = Height;
	Constraints->MinWidth  = ScaledInt(360);

	IniFile->LoadPosInfo(this, DialogCenter);
	CmpByKeyCheckBox->Checked = IniFile->ReadBoolGen(_T("InpDirDlgCmpByKey"));	//→キーで補完
	CmpByKeyCheckBox->Visible = !isWebSea;

	InpDirComboBox->Text = EmptyStr;
	if (isWebSea) {
		InpDirComboBox->Items->Assign(WebSeaHistory);
		RefBtnPanel->Visible = false;
	}
	else {
		InpDirComboBox->Items->Assign(InputDirHistory);
		RefBtnPanel->Visible = true;
	}

	if (InitialPath.IsEmpty()) InitialPath = CurPathName;

	InhFilter  = false;
	CurSeaPath = EmptyStr;

	SubComboBox->AutoComplete	 = false;
	SubComboBox->Enabled		 = false;
	SubComboBox->Clear();

	InpDirComboBox->AutoComplete = false;
	InpDirComboBox->Text = EmptyStr;
	InpDirComboBox->SetFocus();

	SetList();
	Filter();
}

//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (ModalResult==mrOk) {
		if (USAME_TI(CommandStr, "WebSearch"))
			add_ComboBox_history(InpDirComboBox);
		else {
			UnicodeString dnam = to_absolute_name(cv_env_str(InpDirComboBox->Text), InitialPath);
			if (file_exists(dnam)) add_ComboBox_history(InpDirComboBox, dnam);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("InpDirDlgCmpByKey"),	CmpByKeyCheckBox);

	(USAME_TI(CommandStr, "WebSearch")? WebSeaHistory : InputDirHistory)->Assign(InpDirComboBox->Items);

	ItemList->Clear();
	InitialPath = EmptyStr;
	NoCheck = false;
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormDestroy(TObject *Sender)
{
	delete ItemList;
}

//---------------------------------------------------------------------------
//参照
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = dir_exists(InpDirComboBox->Text)? InpDirComboBox->Text : InitialPath;
	if (UserModule->SelectDirEx(null_TCHAR, dnam)) InpDirComboBox->Text = dnam;
}

//---------------------------------------------------------------------------
//入力文字列をパス名に変換
//---------------------------------------------------------------------------
UnicodeString __fastcall TInpDirDlg::GetInpWord(bool path_sw)
{
	UnicodeString wd = InpDirComboBox->Text;
	wd = to_absolute_name(cv_env_str(slash_to_yen(wd)), InitialPath);
	if (wd.IsEmpty()) wd = InitialPath;

	if (ExtractFileDrive(wd).IsEmpty() || USAME_TS(wd, "\\\\")) wd = EmptyStr;
	if (path_sw && !wd.IsEmpty()) wd = IncludeTrailingPathDelimiter(ExtractFilePath(wd));

	return wd;
}

//---------------------------------------------------------------------------
//候補リストを設定
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::SetList()
{
	if (isWebSea) return;

	ItemList->Clear();
	if (!check_if_unc(InpDirComboBox->Text)) return;

	UnicodeString pnam = GetInpWord(true);
	if (!SameText(CurSeaPath, pnam)) {
		cursor_HourGlass();
		TSearchRec sr;
		UnicodeString sea_str = pnam + "*";
		if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);
		if (FindFirst(sea_str, faAnyFile, sr)==0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				UnicodeString fnam = sr.Name;  if (ContainsStr("..", fnam)) continue;
				ItemList->Add(pnam + fnam);
			} while(FindNext(sr)==0);
			FindClose(sr);
			CurSeaPath = pnam;
		}
		else
			CurSeaPath = EmptyStr;
		cursor_Default();
	}
}
//---------------------------------------------------------------------------
//フィルタ
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::Filter()
{
	if (isWebSea) return;

	if (InhFilter) {
		InhFilter = false; return;
	}

	//キーワード取得
	UnicodeString kwd = GetInpWord();
	try {
		if (kwd.IsEmpty() || ItemList->Count==0) Abort();

		//絞り込み
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Assign(ItemList);
		int i=0;
		while (i<lst->Count) if (!StartsText(kwd, lst->Strings[i])) lst->Delete(i); else i++;
		if (lst->Count==0) Abort();

		//裏コンボボックスに割り当て
		cursor_Default();
		set_RedrawOff(SubComboBox);
		{
			SubComboBox->DroppedDown = false;
			SubComboBox->Items->Assign(lst.get());
			SubComboBox->DroppedDown = !CmpByKeyCheckBox->Checked && !InpDirComboBox->Text.IsEmpty();
			SubComboBox->ItemIndex	 = 0;
			SubComboBox->Enabled	 = true;
		}
		set_RedrawOn(SubComboBox);
		Screen->Cursor = crArrow;	//※ドロップダウン時にカーソルが消える現象への対策
	}
	catch (...) {
		SubComboBox->DroppedDown = false;
		SubComboBox->Clear();
		cursor_Default();
	}

	InpDirComboBox->SetFocus();
	if (!kwd.IsEmpty()) {
		InpDirComboBox->SelLength = 0;
		InpDirComboBox->SelStart  = kwd.Length();
	}
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::InpDirComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!Shift.Empty()) return;

	TComboBox *cp_inp  = InpDirComboBox;
	TComboBox *cp_down = SubComboBox->DroppedDown? SubComboBox :
					  InpDirComboBox->DroppedDown? InpDirComboBox : NULL;

	bool handled = true;
	if (Key==VK_RETURN) {
		if (SubComboBox->DroppedDown) {
			UnicodeString wd = SubComboBox->Text;
			SubComboBox->DroppedDown = false;
			cp_inp->SetFocus();
			cp_inp->Text	  = wd;
			cp_inp->SelLength = 0;
			cp_inp->SelStart  = wd.Length();
		}
		else {
			if (!cp_inp->Text.IsEmpty()) {
				add_ComboBox_history(cp_inp);
				ModalResult = mrOk;
			}
			else ModalResult = mrCancel;
		}
	}
	else if (Key==VK_RIGHT) {
		if (!isWebSea) {
			if (cp_inp->SelStart==cp_inp->Text.Length() && !SubComboBox->DroppedDown) {
				SubComboBox->DroppedDown = true;
				SubComboBox->Enabled	 = true;
			}
			else
				cp_inp->SelStart = cp_inp->SelStart + 1;
		}
	}
	else if (Key==VK_DOWN) {
		if (cp_down) {
			if (cp_down->ItemIndex < cp_down->Items->Count-1) cp_down->ItemIndex = cp_down->ItemIndex + 1;
		}
		else cp_inp->DroppedDown = true;
	}
	else if (Key==VK_UP && SubComboBox->DroppedDown) {
		if (cp_down && cp_down->ItemIndex>0) cp_down->ItemIndex = cp_down->ItemIndex - 1;
	}
	else if (Key==VK_ESCAPE) {
		if (cp_down)
			cp_down->DroppedDown = false;
		else
			ModalResult = mrCancel;
	}
	else if (Key==VK_DELETE || cp_inp->SelStart<cp_inp->Text.Length()) {
		InhFilter = true;
		handled   = false;
	}
	else handled = false;

	if (handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::InpDirComboBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::InpDirComboBoxChange(TObject *Sender)
{
	UnicodeString pnam = GetInpWord(true);
	if (pnam.IsEmpty()) {
		CurSeaPath = EmptyStr;
		ItemList->Clear();
	}
	else if (!SameText(CurSeaPath, pnam))
		SetList();

	Filter();
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::InpDirComboBoxSelect(TObject *Sender)
{
	SubComboBox->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::SubComboBoxClick(TObject *Sender)
{
	InpDirComboBox->SetFocus();
	UnicodeString wd = SubComboBox->Text;
	InpDirComboBox->Text	  = wd;
	InpDirComboBox->SelLength = 0;
	InpDirComboBox->SelStart  = wd.Length();
}
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::SubComboBoxCloseUp(TObject *Sender)
{
	SubComboBox->Enabled = false;
}

//---------------------------------------------------------------------------
//OK
//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::OkActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !InpDirComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
void __fastcall TInpDirDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#InputDir"));
}
//---------------------------------------------------------------------------

