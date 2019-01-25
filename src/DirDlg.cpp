//----------------------------------------------------------------------//
// NyanFi																//
//  登録ディレクトリ/特殊フォルダ一覧									//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MainFrm.h"
#include "DirDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegDirDlg *RegDirDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TRegDirDlg::TRegDirDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, RegDirListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	EnvVarList	 = new TStringList();

	IsSpecial = false;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormShow(TObject *Sender)
{
	Caption = IsSpecial? "特殊フォルダ一覧" : "登録ディレクトリ";
	CmdStr	= EmptyStr;

	IniFile->LoadPosInfo(this, DialogCenter, (IsSpecial? _T("SpecialDirList") : null_TCHAR));

	InitializeListHeader(RegDirHeader, _T("キー|名前|場所"));
	THeaderSections *sp = RegDirHeader->Sections;
	sp->Items[0]->MaxWidth = IsSpecial? 0 : 1000;

	TListBox *lp = RegDirListBox;
	lp->Clear();
	set_StdListBox(lp, LBTAG_OPT_FOCS);
	set_UsrScrPanel(ListScrPanel);

	//特殊フォルダ
	if (IsSpecial) {
		UseEnvVarAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirUseEnv"));
		if (UseEnvVarAction->Checked) lp->Tag |= LBTAG_OPT_SDIR;

		//環境変数変換リストを作成
		EnvVarList->Clear();
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%APPDATA%")) + "\t%APPDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%LOCALAPPDATA%")) + "\t%LOCALAPPDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PUBLIC%")) + "\t%PUBLIC%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMDATA%")) + "\t%PROGRAMDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%USERPROFILE%")) + "\t%USERPROFILE%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%WINDIR%")) + "\t%WINDIR%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES(X86)%")) + "\t%PROGRAMFILES(X86)%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES%")) + "\t%PROGRAMFILES%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMW6432%")) + "\t%PROGRAMW6432S%");

		OptPanel->Visible	= false;
		BlankPanel->Visible = false;

		//一覧を取得(ワークリスト形式)
		std::unique_ptr<TStringList> lst(new TStringList());
		usr_SH->get_SpecialFolderList(lst.get());

		//名前の最大幅を取得
		int n_wd = 0;
		for (int i=0; i<lst->Count; i++) {
			TStringDynArray itm_buf = split_strings_tab(lst->Strings[i]);
			if (itm_buf.Length==2) n_wd = std::max(n_wd, lp->Canvas->TextWidth(itm_buf[1]));
		}
		sp->Items[0]->Width = 0;
		sp->Items[1]->Width = n_wd + ScaledInt(24);
		adjust_HeaderSecWidth(RegDirHeader, 2);

		lp->Items->Assign(lst.get());
		lp->ItemIndex = IndexOfDir(CurPathName);
	}
	//登録ディレクトリ
	else {
		set_ButtonMark(HideOptBtn, UBMK_BDOWN);
		set_ButtonMark(ShowOptBtn, UBMK_BUP);
		HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
		ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);

		MoveTopCheckBox->Checked = IniFile->ReadBoolGen(_T("RegDirDlgMoveTop"));
		OptPanel->Visible		 = IniFile->ReadBoolGen(_T("RegDirDlgShowOpt"),  true);
		BlankPanel->Visible 	 = !OptPanel->Visible;

		sp->Items[0]->Width = IniFile->ReadInteger("RegDirGrid", "ColWidth0",	40);
		sp->Items[1]->Width = IniFile->ReadInteger("RegDirGrid", "ColWidth1",	200);
		adjust_HeaderSecWidth(RegDirHeader, 2);

		lp->Items->Assign(RegDirList);
		//タグ検索の結果リスト
		if (CurStt->is_Find && CurStt->find_TAG) {
			KeyEdit->Text  = EmptyStr;
			DescEdit->Text = EmptyStr;
			UnicodeString tags = CurStt->find_Keywd;
			if (!CurStt->find_And) tags = ReplaceStr(tags, ";", "|");
			DirEdit->Text = "#:" + tags;
		}
		//その他
		else {
			lp->ItemIndex = IndexOfDir(CurPathName);
			if (lp->ItemIndex==-1) {
				KeyEdit->Text  = EmptyStr;
				DescEdit->Text = EmptyStr;
			}
			DirEdit->Text = CurPathName;
		}

		SetOptBtn();
	}

	lp->SetFocus();
	ListScrPanel->UpdateKnob();

	UserModule->InitializeListBox(lp, ListScrPanel);
	SelIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	if (IsSpecial) {
		IniFile->WriteBoolGen(_T("SpecialDirUseEnv"),	UseEnvVarAction->Checked);
	}
	else {
		TListBox *lp = RegDirListBox;
		for (int i=0; i<lp->Count; i++) {
			if (is_separator(get_csv_item(lp->Items->Strings[i], 1))) lp->Items->Strings[i] = ",-,";
		}
		RegDirList->Assign(lp->Items);

		//使用項目をセパレータを考慮して先頭へ移動
		if (MoveTopCheckBox->Checked) move_top_RegDirItem(SelIndex);

		IniFile->WriteBoolGen(_T("RegDirDlgMoveTop"),	MoveTopCheckBox);
		IniFile->WriteBoolGen(_T("RegDirDlgShowOpt"),	OptPanel->Visible);

		THeaderSections *sp = RegDirHeader->Sections;
		IniFile->WriteInteger("RegDirGrid", "ColWidth0",	sp->Items[0]->Width);
		IniFile->WriteInteger("RegDirGrid", "ColWidth1",	sp->Items[1]->Width);
	}

	IniFile->SavePosInfo(this, (IsSpecial? _T("SpecialDirList"): null_TCHAR));

	IsSpecial = false;
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
	delete EnvVarList;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormResize(TObject *Sender)
{
	if (!IsSpecial) SetOptBtn();

	RegDirListBox->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirHeaderResize(TObject *Sender)
{
	adjust_HeaderSecWidth((THeaderControl *)Sender, 2);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section)
{
	adjust_HeaderSecWidth(HeaderControl, 2);
	RegDirListBox->Repaint();
}

//---------------------------------------------------------------------------
int __fastcall TRegDirDlg::IndexOfDir(UnicodeString dnam)
{
	int idx = -1;
	dnam = IncludeTrailingPathDelimiter(dnam);
	TListBox *lp = RegDirListBox;

	if (IsSpecial) {
		for (int i=0; i<lp->Count && idx==-1; i++) {
			if (SameText(get_pre_tab(lp->Items->Strings[i]), dnam)) idx = i;
		}
		//一致するものがなければ、一番近い親を探す
		if (idx==-1) {
			UnicodeString max_nam;
			for (int i=0; i<lp->Count; i++) {
				UnicodeString pnam = get_pre_tab(lp->Items->Strings[i]);
				if (StartsText(pnam, dnam)) {
					if (pnam.Length()>max_nam.Length()) {
						max_nam = pnam;
						idx = i;
					}
				}
			}
		}
	}
	else {
		for (int i=0; i<lp->Count && idx==-1; i++) {
			if (SameText(get_csv_item(lp->Items->Strings[i], 2), dnam)) idx = i;
		}
	}

	return idx;
}

//---------------------------------------------------------------------------
//カーソル位置項目のパスを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TRegDirDlg::GetCurDirItem(
	bool dsp_sw,	//true = 特殊フォルダの表示名を取得	(default = false)
	bool id_sw)		//true = CSIDLを取得				(default = false)
{
	UnicodeString dnam;
	TListBox *lp = RegDirListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString lbuf = lp->Items->Strings[lp->ItemIndex];
		if (IsSpecial) {
			TStringDynArray itm_buf = split_strings_tab(lbuf);
			if (itm_buf.Length==2 && !itm_buf[0].IsEmpty()) {
				dnam = itm_buf[0];
				if (dsp_sw && UseEnvVarAction->Checked) {
					if (contained_wd_i(_T("TEMP|TMP"), itm_buf[1])) dnam = "%" + itm_buf[1] + "%";
					else dnam = ExcludeTrailingPathDelimiter(replace_str_by_list(dnam, EnvVarList));
				}
			}
		}
		else {
			TStringDynArray itm_buf = get_csv_array(lbuf, 3, true);
			if (!is_separator(itm_buf[1])) dnam = itm_buf[2];
		}
	}

	return dnam;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	TListBox *lp = RegDirListBox;

	//呼び出しキーか？
	bool k_used = false;
	for (int i=0; i<lp->Count && !k_used; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], 3, true);
		k_used = SameText(KeyStr, get_tkn(lp->Items->Strings[i], ','));
	}

	bool handled = true;
	if (k_used)
		handled = false;
	else if (USAME_TI(KeyStr, "UP") && lp->ItemIndex==0)
		lp->ItemIndex = lp->Count - 1;
	else if (USAME_TI(KeyStr, "DOWN") && lp->ItemIndex==lp->Count-1)
		lp->ItemIndex = 0;
	else if (contained_wd_i(KeysStr_Popup, KeyStr))
		show_PopupMenu(lp);
	else if (IsSpecial && ExeCmdListBox(lp, cmd_F))
		;
	else if (!IsSpecial && UserModule->ListBoxOpeItem(KeyStr))
		;
	//プロパティ
	else if (USAME_TI(cmd_F, "PropertyDlg")) {
		UnicodeString dnam = GetCurDirItem();
		if (!dnam.IsEmpty() && !StartsText("shell:", dnam) && !StartsText("#:", dnam)) {
			pos_ListBoxItem(lp);
			ShowPropertyDialog(dnam);
		}
	}
	//エクスプローラで開く
	else if (USAME_TI(cmd_F, "OpenByExp")) {
		ClearKeyBuff(true);		//OnKeyPress を抑止
		OpenByExpAction->Execute();
	}
	else handled = false;

	if (handled) Key = 0;

	ListScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	UnicodeString jdir;
	bool found = false;
	SelIndex = -1;

	TListBox *lp = (TListBox*)Sender;
	if (Key==VK_RETURN) {
		jdir = GetCurDirItem();
		if (!jdir.IsEmpty()) {
			SelIndex = lp->ItemIndex;
			found = true;
		}
	}
	else if (!IsSpecial && !is_KeyDown(VK_CONTROL)) {
		//該当キーの登録数をカウント
		int f_cnt = 0;
		UnicodeString k = Key;
		for (int i=0; i<lp->Count; i++) {
			TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], 3, true);
			if (SameText(k, itm_buf[0])) {
				jdir = itm_buf[2];
				SelIndex = i;
				f_cnt++;
			}
		}

		if		(f_cnt==0) beep_Warn();
		else if (f_cnt==1) found = true;
	}

	//確定
	if (found) {
		//特殊フォルダ
		if (StartsStr("shell:", jdir)) {
			CmdStr.sprintf(_T("OpenByExp_\"%s\""), jdir.c_str());
			ModalResult = mrOk;
		}
		//タグ検索
		else if (remove_top_s(jdir, "#:")) {
			NyanFiForm->RecoverFileList2();
			CmdStr.sprintf(_T("FindTag_%s"), jdir.c_str());
			ModalResult = mrOk;
		}
		//通常ディレクトリ
		else {
			NyanFiForm->RecoverFileList2();
			UnicodeString dnam = get_actual_path(jdir);
			if (is_dir_accessible(dnam)) {
				NyanFiForm->UpdateCurPath(dnam);
				ModalResult = mrOk;
			}
			else msgbox_ERR(USTR_CantAccessDir);
		}
	}

	//特殊フォルダ一覧でのインクリメンタルサーチを回避
	if (IsSpecial && (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN || is_KeyDown(VK_CONTROL))) Key = 0;
}
//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxDblClick(TObject *Sender)
{
	WideChar key = VK_RETURN;
	RegDirListBoxKeyPress(Sender, key);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxClick(TObject *Sender)
{
	if (IsSpecial) return;

	int idx = RegDirListBox->ItemIndex;  if (idx==-1) return;
	TStringDynArray itm_buf = get_csv_array(RegDirListBox->Items->Strings[idx], 3, true);
	if (is_separator(itm_buf[1])) itm_buf[0] = itm_buf[2] = EmptyStr;
	KeyEdit->Text  = itm_buf[0];
	DescEdit->Text = itm_buf[1];
	DirEdit->Text  = itm_buf[2];
}

//---------------------------------------------------------------------------
//項目の描画
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(ListFont);
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	//特殊フォルダ
	if (IsSpecial) {
		TStringDynArray itm_buf = split_strings_tab(lp->Items->Strings[Index]);
		if (itm_buf.Length==2) {
			int xp = Rect.Left + 4;
			int yp = Rect.Top + get_TopMargin2(cv);
			//セパレータ
			if (itm_buf[0].IsEmpty()) {
				draw_Separator(cv, Rect);
				cv->Font->Color = col_Folder;
				int tag = (int)lp->Items->Objects[Index];
				if (tag>0) cv->TextOut(xp, yp, (tag==2)? "<仮想フォルダ>" : "<All Users> ");
			}
			//項目
			else {
				//名前
				THeaderSections *sp = RegDirHeader->Sections;
				cv->Font->Color = col_fgList;
				UnicodeString inam = itm_buf[1];
				bool brk = remove_top_s(inam, '|');
				cv->TextOut(xp, yp, inam);
				xp = sp->Items[1]->Width + 1;
				//場所
				UnicodeString dnam = itm_buf[0];
				if (UseEnvVarAction->Checked) {
					if (contained_wd_i(_T("TEMP|TMP"), itm_buf[1])) dnam = "%" + itm_buf[1] + "%";
					else dnam = ExcludeTrailingPathDelimiter(replace_str_by_list(dnam, EnvVarList));
					if (remove_top_s(dnam, '%')) {
						UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
						cv->Font->Color = AdjustColor(col_Folder, 64);
						cv->TextOut(xp, yp, envstr);
						xp += get_TextWidth(cv, envstr, is_irreg) + 2;
					}
				}
				cv->Font->Color = (StartsStr("shell:", dnam))? AdjustColor(col_Folder, 64) : col_Folder;
				PathNameOut(dnam, cv, xp, yp, sp->Items[2]->Width);
				//区切り線
				if (brk) draw_separateLine(cv, Rect);
			}
		}
	}
	//登録ディレクトリ
	else {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[Index], 3, true);
		//セパレータ
		if (is_separator(itm_buf[1]))
			draw_Separator(cv, Rect);
		//項目
		else {
			THeaderSections *sp = RegDirHeader->Sections;
			//キー
			cv->Font->Color = col_fgList;
			cv->Font->Style = cv->Font->Style << fsBold;
			int s_wd = cv->TextWidth(itm_buf[0]);
			int c_wd = sp->Items[0]->Width - 4;
			int xp = Rect.Left + 2;
			int yp = Rect.Top + get_TopMargin2(cv);
			if (s_wd<c_wd) xp += (c_wd - s_wd)/2;	//センタリング
			cv->TextOut(xp, yp, itm_buf[0]);
			//名前
			xp = sp->Items[0]->Width + 1;
			cv->Font->Style = cv->Font->Style >> fsBold;
			cv->TextOut(xp, yp, itm_buf[1]);
			xp += sp->Items[1]->Width + 1;
			//場所
			UnicodeString dnam = itm_buf[2];
			if (StartsStr("#:", dnam)) {
				cv->Font->Color = AdjustColor(col_Folder, 64);
				usr_TAG->DrawTags(dnam, cv, xp, yp, RevTagCololr? col_bgList : clNone);
			}
			else {
				if (remove_top_s(dnam, '%')) {
					UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
					cv->Font->Color = AdjustColor(col_Folder, 64);
					cv->TextOut(xp, yp, envstr);
					xp += get_TextWidth(cv, envstr, is_irreg) + 2;
				}
				cv->Font->Color = (StartsStr("shell:", dnam))? AdjustColor(col_Folder, 64) : col_Folder;
				PathNameOut(dnam, cv, xp, yp, sp->Items[2]->Width);
			}
		}
	}

	//カーソル
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
//項目の追加・変更
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::ChangeItemActionExecute(bool add)
{
	if (add && IndexOfDir(DirEdit->Text)!=-1) {
		msgbox_WARN(USTR_Registered);
		return;
	}

	if (is_separator(DescEdit->Text)
		|| StartsText("shell:", DirEdit->Text) || StartsText("#:", DirEdit->Text)
		|| dir_exists(get_actual_path(DirEdit->Text)))
	{
		UnicodeString lbuf;
		if (is_separator(DescEdit->Text))
			lbuf.USET_T(",-,");
		else {
			UnicodeString dnam = DirEdit->Text;
			if (!StartsText("shell:", dnam) && !StartsText("#:", dnam))
				dnam = IncludeTrailingPathDelimiter(dnam);
			lbuf = KeyEdit->Text.SubString(1, 1).UpperCase();
			lbuf.cat_sprintf(_T(",%s,%s"),
				make_csv_str(DescEdit->Text).c_str(), make_csv_str(dnam).c_str());
		}

		TListBox *lp = RegDirListBox;
		if (add) {
			lp->Items->Add(lbuf);
			lp->ItemIndex = lp->Count - 1;
		}
		else {
			if (lp->ItemIndex==-1) return;
			lp->Items->Strings[lp->ItemIndex] = lbuf;
		}

		lp->SetFocus();
		ListScrPanel->UpdateKnob();
	}
	else msgbox_WARN(USTR_DirNotFound);
}

//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::EditItemActionExecute(TObject *Sender)
{
	ChangeItemActionExecute(false);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::EditItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (!IsSpecial && RegDirListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionExecute(TObject *Sender)
{
	ChangeItemActionExecute(true);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (!IsSpecial && !DescEdit->Text.IsEmpty());
}

//---------------------------------------------------------------------------
//ディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RefDirButtonClick(TObject *Sender)
{
	UnicodeString dnam = DirEdit->Text;
	if (UserModule->SelectDirEx(_T("登録ディレクトリ"), dnam)) DirEdit->Text = to_path_name(dnam);
}

//---------------------------------------------------------------------------
//パス名をコピー
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::CopyPathActionExecute(TObject *Sender)
{
	copy_to_Clipboard(ExcludeTrailingPathDelimiter(GetCurDirItem(IsSpecial)));
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::CopyPathActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !GetCurDirItem().IsEmpty();
}

//---------------------------------------------------------------------------
//環境変数を用いてパス名を表示
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::UseEnvVarActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	if (ap->Checked)
		RegDirListBox->Tag |= LBTAG_OPT_SDIR;
	else
		RegDirListBox->Tag &= ~LBTAG_OPT_SDIR;

	RegDirListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::UseEnvVarActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = IsSpecial;
}

//---------------------------------------------------------------------------
//エクスプローラで開く
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::OpenByExpActionExecute(TObject *Sender)
{
	UnicodeString dnam = GetCurDirItem();
	if (::ShellExecute(NULL, _T("explore"), dnam.c_str(), NULL, NULL, SW_SHOWNORMAL) > (HINSTANCE)32)
		ModalResult = mrOk;
	else
		msgbox_ERR(USTR_FaildExec);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::OpenByExpActionUpdate(TObject *Sender)
{
	UnicodeString dnam = GetCurDirItem();
	((TAction*)Sender)->Enabled = (!dnam.IsEmpty() && !StartsText("#:", dnam));
}

//---------------------------------------------------------------------------
//ディレクトリのプロパティ
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::PropertyActionExecute(TObject *Sender)
{
	UnicodeString dnam = GetCurDirItem();
	if (!dnam.IsEmpty()) ShowPropertyDialog(dnam);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::PropertyActionUpdate(TObject *Sender)
{
	UnicodeString dnam = GetCurDirItem();
	((TAction*)Sender)->Enabled = (!dnam.IsEmpty() && !StartsText("shell:", dnam) && !StartsText("#:", dnam));
}

//---------------------------------------------------------------------------
//一覧をワークリストとして保存
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SaveAsWorkActionExecute(TObject *Sender)
{
	UnicodeString fnam;
	UserModule->PrepareSaveDlg(LoadUsrMsg(USTR_SaveAs, _T("ワークリスト")).c_str(), 
		F_FILTER_NWL, _T("SpecialDir.nwl"), WorkListPath);

	if (UserModule->SaveDlg->Execute()) {
		TListBox *lp = RegDirListBox;
		std::unique_ptr<TStringList> lst(new TStringList());
		for (int i=0; i<lp->Count; i++) {
			if ((int)lp->Items->Objects[i]==2) break;
			lst->Add(REPLACE_TS(lp->Items->Strings[i], "\t|", "\t"));
		}

		if (!saveto_TextUTF8(UserModule->SaveDlg->FileName, lst.get()))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, ExtractFileName(UserModule->SaveDlg->FileName)));
	}
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SaveAsWorkActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = IsSpecial;
	ap->Enabled = IsSpecial;
}

//---------------------------------------------------------------------------
//オプションの開閉
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SetOptBtn()
{
	if (ScrBarStyle==0)
		set_ControlRBCorner(RegDirListBox, BlankPanel);
	else
		ListScrPanel->SetRBCornerPanel(BlankPanel);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::ChgOptBtnClick(TObject *Sender)
{
	OptPanel->Visible	= !OptPanel->Visible;
	BlankPanel->Visible = !OptPanel->Visible;
	ListScrPanel->UpdateKnob();
	SetOptBtn();
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift,
		IsSpecial? _T(HELPTOPIC_FL) _T("#SpecialDirList") : _T("hid00054.htm"));
}
//---------------------------------------------------------------------------

