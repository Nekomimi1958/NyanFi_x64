//----------------------------------------------------------------------//
// NyanFi																//
//  登録ディレクトリ/特殊フォルダ一覧/参照								//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MainFrm.h"
#include "FileInfDlg.h"
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
	SpDirList	 = new TStringList();
	SpDirBuff	 = new TStringList();

	IsSpecial = false;
	ToFilter  = false;
	IsAddMode = IsSelect = false;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormShow(TObject *Sender)
{
	Caption = IsSpecial? "特殊フォルダ一覧" : "登録ディレクトリ";
	CmdStr	= EmptyStr;

	OpeToolBar->Visible 	 = IsSpecial;
	ShowIconAction->Visible  = IsSpecial;
	AddNyanFiAction->Visible = IsSpecial;

	if (IsSpecial) {
		setup_ToolBar(OpeToolBar);
		FilterEdit->Width = IniFile->ReadIntGen(_T("SpecialDirFilterWidth"),	200);

		set_MigemoAction(MigemoAction, _T("SpecialDirMigemo"));
		AndOrAction->Checked	= IniFile->ReadBoolGen(_T("SpecialDirFltAndOr"));
		ShowIconAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirShowIcon"), true);
	}

	IniFile->LoadPosInfo(this, DialogCenter, (IsSpecial? (IsSelect? "SelSpecialDir" : "SpecialDirList") : ""));

	//ヘッダ
	InitializeListHeader(RegDirHeader, _T("キー|名前|場所"));
	THeaderSections *sp = RegDirHeader->Sections;
	sp->Items[0]->MaxWidth	 = IsSpecial? 0 : 1000;
	sp->Items[1]->FixedWidth = IsSpecial;

	TListBox *lp = RegDirListBox;
	lp->Clear();
	set_StdListBox(lp, LBTAG_OPT_FOCS);
	set_UsrScrPanel(ListScrPanel);

	//特殊フォルダ
	if (IsSpecial) {
		UseEnvVarAction->Visible = true;
		UseEnvVarAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirUseEnv"));
		if (UseEnvVarAction->Checked) lp->Tag |= LBTAG_OPT_SDIR;
		AddNyanFiAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirAddNyanFi"));

		//環境変数変換リストを作成
		EnvVarList->Clear();
		EnvVarList->Add(cv_env_var("%APPDATA%")		 	  + "\t%APPDATA%");
		EnvVarList->Add(cv_env_var("%LOCALAPPDATA%")	  + "\t%LOCALAPPDATA%");
		EnvVarList->Add(cv_env_var("%PUBLIC%")			  + "\t%PUBLIC%");
		EnvVarList->Add(cv_env_var("%PROGRAMDATA%")		  + "\t%PROGRAMDATA%");
		EnvVarList->Add(cv_env_var("%USERPROFILE%")		  + "\t%USERPROFILE%");
		EnvVarList->Add(cv_env_var("%WINDIR%")			  + "\t%WINDIR%");
		EnvVarList->Add(cv_env_var("%PROGRAMFILES(X86)%") + "\t%PROGRAMFILES(X86)%");
		EnvVarList->Add(cv_env_var("%PROGRAMFILES%")	  + "\t%PROGRAMFILES%");
		EnvVarList->Add(cv_env_var("%PROGRAMW6432%")	  + "\t%PROGRAMW6432%");

		OptPanel->Visible	= false;
		BlankPanel->Visible = false;

		UpdateSpDirList(true);

		(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)lp)->SetFocus();
	}
	//登録ディレクトリ
	else {
		UseEnvVarAction->Visible = false;

		set_BtnMarkDark(HideOptBtn, UBMK_BDOWN);
		set_BtnMarkDark(ShowOptBtn, UBMK_BUP);
		HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
		ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);

		MoveTopCheckBox->Checked = IniFile->ReadBoolGen(_T("RegDirDlgMoveTop"));
		OptPanel->Visible		 = IniFile->ReadBoolGen(_T("RegDirDlgShowOpt"),	true);
		BlankPanel->Visible 	 = !OptPanel->Visible;

		sp->Items[0]->Width = IniFile->ReadInteger("RegDirGrid", "ColWidth0",	40);
		sp->Items[1]->Width = IniFile->ReadInteger("RegDirGrid", "ColWidth1",	200);
		adjust_HeaderSecWidth(RegDirHeader, 2);

		lp->Items->Assign(RegDirList);

		//タグ検索の結果リスト
		if (CurStt->is_Find && CurStt->find_TAG) {
			lp->Items->Add(EmptyStr);
			lp->ItemIndex = lp->Count - 1;
			KeyEdit->Text  = EmptyStr;
			DescEdit->Text = EmptyStr;
			UnicodeString tags = CurStt->find_Keywd;
			if (!CurStt->find_And) tags = ReplaceStr(tags, ";", "|");
			DirEdit->Text = "#:" + tags;
		}
		//その他
		else {
			int idx = IndexOfDir(CurPathName);
			if (idx!=-1) {
				lp->ItemIndex = idx;
				IsAddMode = false;
			}
			else {
				lp->Items->Add(EmptyStr);
				lp->ItemIndex = lp->Count - 1;
				KeyEdit->Text  = EmptyStr;
				DescEdit->Text = EmptyStr;
			}
			DirEdit->Text = CurPathName;
		}

		SetOptBtn();
		lp->SetFocus();
	}

	ListScrPanel->UpdateKnob();
	lp->DragMode = IsSpecial? dmManual : dmAutomatic;
	UserModule->InitializeListBox(lp, ListScrPanel);
	SelIndex = -1;

	SetDarkWinTheme(this);
	SetDarkWinTheme(FilterEdit);
	FilterEdit->Color = get_WinColor(!ToFilter);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	if (IsSpecial) {
		IniFile->WriteIntGen( _T("SpecialDirFilterWidth"),	FilterEdit->Width);
		IniFile->WriteBoolGen(_T("SpecialDirMigemo"),		MigemoAction);
		IniFile->WriteBoolGen(_T("SpecialDirFltAndOr"),		AndOrAction);
		IniFile->WriteBoolGen(_T("SpecialDirShowIcon"),		ShowIconAction);
		IniFile->WriteBoolGen(_T("SpecialDirUseEnv"),		UseEnvVarAction);
		IniFile->WriteBoolGen(_T("SpecialDirAddNyanFi"),	AddNyanFiAction);

		SpDirList->Clear();
		SpDirBuff->Clear();
	}
	else {
		TListBox *lp = RegDirListBox;
		int i = 0;
		while (i<lp->Count) {
			UnicodeString lbuf = lp->Items->Strings[i];
			if (lbuf.IsEmpty()) {
				lp->Items->Delete(i);
			}
			else {
				if (is_separator(get_csv_item(lbuf, 1))) lp->Items->Strings[i] = ",-,";
				i++;
			}
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

	IniFile->SavePosInfo(this, (IsSpecial? (IsSelect? "SelSpecialDir" : "SpecialDirList") : ""));

	if (IsSelect) Action = caFree;

	IsSpecial = false;
	ToFilter  = false;
	IsAddMode = IsSelect = false;
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
	delete EnvVarList;
	delete SpDirList;
	delete SpDirBuff;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormResize(TObject *Sender)
{
	if (!IsSpecial) SetOptBtn();

	RegDirListBox->Invalidate();
}

//---------------------------------------------------------------------------
//特殊フォルダ一覧の更新
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::UpdateSpDirList(bool reload)
{
	if (!IsSpecial) return;

	//一覧を取得(ワークリスト形式)
	if (reload) {
		SpDirList->Clear();
		usr_SH->get_SpecialFolderList(SpDirList);

		//NyanFi 固有
		if (AddNyanFiAction->Checked) {
			SpDirList->Add("\t-");		//セパレータ
			SpDirList->Add(ExePath   + "\t実行パス");
			SpDirList->Add(TempPathA + "\t一時ディレクトリ");
			if (!DownloadPath.IsEmpty())	SpDirList->Add(DownloadPath + "\tダウンロード");
			if (usr_Migemo->Available)		SpDirList->Add(to_absolute_name(MigemoPath) + "\tMigemo");
			if (!SpiDir.IsEmpty())			SpDirList->Add(to_absolute_name(SpiDir) + "\tSusie プラグイン");
			if (!CmdGitExe.IsEmpty())		SpDirList->AddObject(CmdGitExe + "\tgit.exe",	  		  (TObject*)1);

			//エディタ
			SpDirList->Add("\t-");
			if (!TextEditor.IsEmpty())		SpDirList->AddObject(TextEditor + "\tテキストエディタ",  (TObject*)1);
			if (!ImageEditor.IsEmpty())		SpDirList->AddObject(ImageEditor + "\tイメージエディタ",  (TObject*)1);
			if (!BinaryEditor.IsEmpty())	SpDirList->AddObject(BinaryEditor + "\tバイナリエディタ", (TObject*)1);
			//その他のエディタ
			std::unique_ptr<TStringList> x_lst(new TStringList());
			for (int i=0; i<EtcEditorList->Count; i++) {
				UnicodeString xnam = exclude_quot(EtcEditorList->ValueFromIndex[i]);
				if (!starts_Dollar(xnam) || contains_PathDlmtr(xnam)) {
					xnam = get_actual_path(xnam);
					if (x_lst->IndexOf(xnam)==-1) x_lst->Add(get_base_name(xnam) + "\t" + xnam);
				}
			}
			x_lst->Sort();
			for (int i=0; i<x_lst->Count; i++) {
				UnicodeString lbuf = x_lst->Strings[i];
				SpDirList->AddObject(get_post_tab(lbuf) + ((i==0)? "\t|" : "\t") + get_pre_tab(lbuf), (TObject*)1);
			}

			//外部ツール
			SpDirList->Add("\t-");
			x_lst->Clear();
			for (int i=0; i<ExtToolList->Count; i++) {
				TStringDynArray itm_buf = get_csv_array(ExtToolList->Strings[i], 2, true);
				UnicodeString xnam = itm_buf[1];	if (xnam.IsEmpty()) continue;
				xnam = get_actual_path(xnam);
				if (ExtractFileDrive(xnam).IsEmpty()) xnam = get_actual_name(xnam);
				if (x_lst->IndexOf(xnam)==-1) x_lst->Add(get_base_name(xnam) + "\t" + xnam);
			}
			x_lst->Sort();
			for (int i=0; i<x_lst->Count; i++) {
				UnicodeString lbuf = x_lst->Strings[i];
				SpDirList->AddObject(get_post_tab(lbuf) + "\t" + get_pre_tab(lbuf), (TObject*)1);
			}
		}
	}

	if (!FilterEdit->Text.IsEmpty()
		&& (!MigemoAction->Checked || (MigemoAction->Checked && FilterEdit->Text.Length()>=IncSeaMigemoMin)))
	{
		filter_List(SpDirList, SpDirBuff, FilterEdit->Text, MigemoAction->Checked, AndOrAction->Checked);
	}
	else {
		SpDirBuff->Assign(SpDirList);
	}

	//名前の最大幅を取得
	TListBox *lp = RegDirListBox;
	int n_wd = 0;
	for (int i=0; i<SpDirBuff->Count; i++) {
		UnicodeString dnam = get_post_tab(SpDirBuff->Strings[i]);
		if (!dnam.IsEmpty()) {
			int wd = lp->Canvas->TextWidth(dnam);
			if ((int)SpDirBuff->Objects[i]==1 && ShowIconAction->Checked) wd += get_IcoWidth();
			n_wd = std::max(n_wd, wd);
		}
	}

	THeaderSections *sp = RegDirHeader->Sections;
	sp->Items[0]->Width = 0;
	sp->Items[1]->Width = n_wd + ScaledInt(24);
	adjust_HeaderSecWidth(RegDirHeader, 2);

	lp->Items->Assign(SpDirBuff);
	lp->ItemIndex = IndexOfDir(CurPathName);
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
			UnicodeString pnam = get_pre_tab(lp->Items->Strings[i]);
			if ((int)lp->Items->Objects[i]==1) pnam = ExtractFilePath(pnam);
			if (SameText(pnam, dnam)) idx = i;
		}
		//一致するものがなければ、一番近い親を探す
		if (idx==-1) {
			UnicodeString max_nam;
			for (int i=0; i<lp->Count; i++) {
				UnicodeString pnam = get_pre_tab(lp->Items->Strings[i]);
				if ((int)lp->Items->Objects[i]==1) pnam = ExtractFilePath(pnam);
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
	bool nam_sw,	//true = 特殊フォルダ名を付加		(default = false)
	bool exe_sw)	//true = 実行ファイル名として取得	(default = false)
{
	UnicodeString dnam;
	TListBox *lp = RegDirListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		UnicodeString lbuf = lp->Items->Strings[idx];
		if (IsSpecial) {
			TStringDynArray itm_buf = split_strings_tab(lbuf);
			if (itm_buf.Length==2 && !itm_buf[0].IsEmpty()) {
				dnam = itm_buf[0];
				if ((int)lp->Items->Objects[idx]==1 && !exe_sw) {
					dnam = ExtractFilePath(dnam);
				}
				if (dsp_sw && UseEnvVarAction->Checked) {
					if (contained_wd_i(_T("TEMP|TMP"), itm_buf[1]))
						dnam = "%" + itm_buf[1] + "%";
					else
						dnam = ExcludeTrailingPathDelimiter(replace_str_by_list(dnam, EnvVarList));
				}
				if (nam_sw) dnam += ("\t" + itm_buf[1]);
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
	bool to_call = false;
	if (!IsSpecial) {
		for (int i=0; i<lp->Count && !to_call; i++) {
			to_call = SameText(KeyStr, get_tkn(lp->Items->Strings[i], ','));
		}
	}

	bool handled = true;
	if (to_call) {
		handled = false;
	}
	else if (equal_UP(KeyStr) && lp->ItemIndex==0) {
		lp->ItemIndex = lp->Count - 1;
	}
	else if (equal_DOWN(KeyStr) && lp->ItemIndex==lp->Count-1) {
		lp->ItemIndex = 0;
	}
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
		show_PopupMenu(lp);
	}
	else if (IsSpecial && ExeCmdListBox(lp, cmd_F)) {
		;
	}
	else if (!IsSpecial && UserModule->ListBoxOpeItem(KeyStr)) {
		;
	}
	else if (StartsText("ContextMenu", cmd_F)) {
		show_PopupMenu(lp);
	}
	else if (IsSpecial && StartsText("IncSearch", cmd_F)) {
		FilterEdit->SetFocus();
	}
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
	//アクセラレータで移動
	else if (IsSpecial) {
		int s_idx = USAME_TI(KeyStr, "Alt+U")? 1 :
					USAME_TI(KeyStr, "Alt+V")? 2 :
					USAME_TI(KeyStr, "Alt+N")? 3 :
					USAME_TI(KeyStr, "Alt+E")? 4 :
					USAME_TI(KeyStr, "Alt+X")? 5 : 0;
		if (s_idx>0) {
			int s_cnt = 0;
			for (int i=0; i<lp->Count; i++) {
				if (StartsStr("\t", lp->Items->Strings[i])) {
					s_cnt++;
					if (s_cnt==s_idx) ListBoxSetIndex(lp, i + 1, true);
				}
			}
		}
		else {
			handled = false;
		}
	}
	else {
		handled = false;
	}

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
		jdir = GetCurDirItem(false, IsSelect, true);
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

		if (f_cnt==0)
			beep_Warn();
		else if (f_cnt==1)
			found = true;
	}

	//確定
	if (found) {
		if (IsSelect) {
			CmdStr = jdir;
			ModalResult = mrOk;
		}
		else {
			//実行ファイル
			if (SelIndex!=-1 && (int)lp->Items->Objects[SelIndex]==1) {
				CmdStr.sprintf(_T("JumpTo_\"%s\""), jdir.c_str());
				ModalResult = mrOk;
			}
			//特殊フォルダ
			else if (StartsStr("shell:", jdir)) {
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
				else {
					msgbox_ERR(USTR_CantAccessDir);
				}
			}
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
	if (IsSpecial || IsAddMode) return;

	int idx = RegDirListBox->ItemIndex;
	if (idx==-1) return;
	UnicodeString lbuf = RegDirListBox->Items->Strings[idx];
	if (!lbuf.IsEmpty()) {
		TStringDynArray itm_buf = get_csv_array(RegDirListBox->Items->Strings[idx], 3, true);
		if (is_separator(itm_buf[1])) itm_buf[0] = itm_buf[2] = EmptyStr;
		KeyEdit->Text  = itm_buf[0];
		DescEdit->Text = itm_buf[1];
		DirEdit->Text  = itm_buf[2];
	}
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
	bool is_irreg  = IsIrregularFont(cv->Font);
	TColor adj_col = AdjustColor(col_Folder, ADJCOL_LIGHT);

	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (!lbuf.IsEmpty()) {
		//特殊フォルダ
		if (IsSpecial) {
			TStringDynArray itm_buf = split_strings_tab(lbuf);
			if (itm_buf.Length==2) {
				int xp = Rect.Left + Scaled4;
				int yp = Rect.Top  + get_TopMargin2(cv);
				//セパレータ
				if (itm_buf[0].IsEmpty()) {
					draw_Separator(cv, Rect);
					cv->Font->Color = col_Folder;
					int s_cnt = 0;
					for (int i=0; i<=Index; i++) if (StartsStr("\t", lp->Items->Strings[i])) s_cnt++;
					UnicodeString snam;
					if (lp->Focused()) {
						snam = (s_cnt==1)? "<All Users (&U)> " : (s_cnt==2)? "<仮想フォルダ (&V)>" :
							   (s_cnt==3)? "<NyanFi (&N)> "    : (s_cnt==4)? "<エディタ (&E)>" :
							   (s_cnt==5)? "<外部ツール (&X)>" : "";
					}
					else {
						snam = (s_cnt==1)? "<All Users> " : (s_cnt==2)? "<仮想フォルダ>" :
							   (s_cnt==3)? "<NyanFi>"     : (s_cnt==4)? "<エディタ>" :
							   (s_cnt==5)? "<外部ツール>" : "";
					}
					if (!snam.IsEmpty()) {
						TRect rc = Rect; rc.Left = xp; rc.Top  = yp;
						::DrawText(cv->Handle, snam.c_str(), -1, &rc, DT_LEFT);
					}
				}
				//項目
				else {
					UnicodeString dnam = itm_buf[0];
					bool is_exe = ((int)lp->Items->Objects[Index]==1);
					//アイコン
					if (is_exe && ShowIconAction->Checked) {
						draw_SmallIconF(dnam, cv, xp, std::max(yp + (cv->TextHeight("Q") - SIcoSize)/2, 0));
						xp += get_IcoWidth();
					}
					//名前
					THeaderSections *sp = RegDirHeader->Sections;
					cv->Font->Color = col_fgList;
					UnicodeString inam = itm_buf[1];
					bool brk = remove_top_s(inam, '|');
					cv->TextOut(xp, yp, inam);
					xp = sp->Items[1]->Width + 1;
					//場所
					if (is_exe) dnam = ExtractFilePath(dnam);
					if (UseEnvVarAction->Checked) {
						if (contained_wd_i(_T("TEMP|TMP"), itm_buf[1]))
							dnam = "%" + itm_buf[1] + "%";
						else
							dnam = ExcludeTrailingPathDelimiter(replace_str_by_list(dnam, EnvVarList));
						if (remove_top_s(dnam, '%')) {
							UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
							cv->Font->Color = adj_col;
							cv->TextOut(xp, yp, envstr);
							xp += get_TextWidth(cv, envstr, is_irreg) + 2;
						}
					}
					cv->Font->Color = (StartsStr("shell:", dnam))? adj_col : col_Folder;
					PathNameOut(dnam, cv, xp, yp, sp->Items[2]->Width);
					//区切り線
					if (brk) draw_separateLine(cv, Rect);
				}
			}
		}
		//登録ディレクトリ
		else {
			TStringDynArray itm_buf = get_csv_array(lbuf, 3, true);
			//セパレータ
			if (is_separator(itm_buf[1])) {
				draw_Separator(cv, Rect);
			}
			//項目
			else {
				THeaderSections *sp = RegDirHeader->Sections;
				//キー
				cv->Font->Color = col_fgList;
				cv->Font->Style = cv->Font->Style << fsBold;
				int s_wd = cv->TextWidth(itm_buf[0]);
				int c_wd = sp->Items[0]->Width - 4;
				int xp = Rect.Left + Scaled2;
				int yp = Rect.Top  + get_TopMargin2(cv);
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
					cv->Font->Color = adj_col;
					usr_TAG->DrawTags(dnam, cv, xp, yp, RevTagCololr? col_bgList : clNone);
				}
				else {
					if (remove_top_s(dnam, '%')) {
						UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
						cv->Font->Color = adj_col;
						cv->TextOut(xp, yp, envstr);
						xp += get_TextWidth(cv, envstr, is_irreg) + 2;
					}
					cv->Font->Color = (StartsStr("shell:", dnam))? adj_col : col_Folder;
					PathNameOut(dnam, cv, xp, yp, sp->Items[2]->Width);
				}
			}
		}
	}

	//カーソル
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
//項目の追加・変更
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::ChangeItemActionExecute(
	int chg_mod)	//変更モード 0:追加/ 1:変更/ 2:挿入
{
	if (chg_mod==0 && IndexOfDir(DirEdit->Text)!=-1) {
		msgbox_WARN(LoadUsrMsg(USTR_Registered));
		return;
	}

	if (is_separator(DescEdit->Text)
		|| StartsText("shell:", DirEdit->Text) || StartsText("#:", DirEdit->Text)
		|| dir_exists(get_actual_path(DirEdit->Text)))
	{
		UnicodeString lbuf;
		if (is_separator(DescEdit->Text)) {
			lbuf = ",-,";
		}
		else {
			UnicodeString dnam = DirEdit->Text;
			if (!StartsText("shell:", dnam) && !StartsText("#:", dnam))
				dnam = IncludeTrailingPathDelimiter(dnam);
			lbuf = KeyEdit->Text.SubString(1, 1).UpperCase();
			lbuf.cat_sprintf(_T(",%s,%s"),
				make_csv_str(DescEdit->Text).c_str(), make_csv_str(dnam).c_str());
		}

		TListBox *lp = RegDirListBox;
		switch (chg_mod) {
		case 0:	//追加
			if (lp->Items->Strings[lp->Count - 1].IsEmpty())
				lp->Items->Strings[lp->Count - 1] = lbuf;
			else
				lp->Items->Add(lbuf);
			lp->ItemIndex = lp->Count - 1;
			break;
		case 1:	//変更
			if (lp->ItemIndex==-1) return;
			lp->Items->Strings[lp->ItemIndex] = lbuf;
			break;
		case 2:	//挿入
			if (lp->ItemIndex==-1) return;
			lp->Items->Insert(lp->ItemIndex, lbuf);
			break;
		}

		int i = 0;
		while (i<lp->Count) {
			if (lp->Items->Strings[i].IsEmpty()) lp->Items->Delete(i); else i++;
		}

		lp->SetFocus();
		ListScrPanel->UpdateKnob();
		IsAddMode = false;
	}
	else msgbox_WARN(LoadUsrMsg(USTR_DirNotFound));
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionExecute(TObject *Sender)
{
	ChangeItemActionExecute(0);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (!IsSpecial && !DescEdit->Text.IsEmpty());
	if (!IsSpecial) Caption = IsAddMode? "登録ディレクトリ(追加)" : "登録ディレクトリ";
}
//---------------------------------------------------------------------------
//変更/挿入
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::EditItemActionExecute(TObject *Sender)
{
	ChangeItemActionExecute(IsAddMode? 2 : 1);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::EditItemActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	TListBox *lp = RegDirListBox;
	if (IsAddMode) {
		ap->Caption = "挿入";
		ap->Enabled = (!IsSpecial && lp->ItemIndex!=-1 && !DescEdit->Text.IsEmpty());
	}
	else {
		ap->Caption = "変更";
		ap->Enabled = (!IsSpecial && lp->ItemIndex!=-1 && !lp->Items->Strings[lp->ItemIndex].IsEmpty());
	}
}
//---------------------------------------------------------------------------
//削除
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::DelItemActionExecute(TObject *Sender)
{
	TListBox *lp = RegDirListBox;
	delete_ListItem(lp);
	if (lp->Tag & LBTAG_OPT_FOCS) lp->SetFocus();
	ListScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::DelItemActionUpdate(TObject *Sender)
{
	TListBox *lp = RegDirListBox;
	((TAction*)Sender)->Enabled =
		(!IsSpecial && lp->ItemIndex!=-1 && !lp->Items->Strings[lp->ItemIndex].IsEmpty());
}

//---------------------------------------------------------------------------
//ディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = DirEdit->Text;
	if (UserModule->SelectDirEx(_T("登録ディレクトリ"), dnam)) DirEdit->Text = to_path_name(dnam);
}
//---------------------------------------------------------------------------
//特殊フォルダの参照
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RefSpBtnClick(TObject *Sender)
{
	TRegDirDlg *SelSpDirDlg = new TRegDirDlg(this);		//閉じたときに破棄
	SelSpDirDlg->IsSpecial	= true;
	SelSpDirDlg->IsSelect	= true;
	if (SelSpDirDlg->ShowModal()==mrOk) {
		DirEdit->Text  = get_pre_tab(SelSpDirDlg->CmdStr);
		DescEdit->Text = get_post_tab(SelSpDirDlg->CmdStr);
		KeyEdit->SetFocus();
	}
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
void __fastcall TRegDirDlg::ToggleActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	UpdateSpDirList();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddNyanFiActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	UpdateSpDirList(true);
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
//アプリケーション情報
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AppInfoActionExecute(TObject *Sender)
{
	FileInfoDlg->ShowModalEx(GetCurDirItem(false, false, true));
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AppInfoActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	TListBox *lp = RegDirListBox;
	ap->Visible = (lp->ItemIndex!=-1 && (int)lp->Items->Objects[lp->ItemIndex]==1);
	ap->Enabled = ap->Visible && file_exists(GetCurDirItem(false, false, true));
}

//---------------------------------------------------------------------------
//一覧をワークリストとして保存
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SaveAsWorkActionExecute(TObject *Sender)
{
	UserModule->PrepareSaveDlg(LoadUsrMsg(USTR_SaveAs, _T("ワークリスト")).c_str(), 
		F_FILTER_NWL, _T("SpecialDir.nwl"), WorkListPath);

	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) {
		TListBox *lp = RegDirListBox;
		std::unique_ptr<TStringList> lst(new TStringList());
		int s_cnt = 0;
		for (int i=0; i<lp->Count; i++) {
			UnicodeString lbuf = ReplaceStr(lp->Items->Strings[i], "\t|", "\t");
			if (get_pre_tab(lbuf).IsEmpty()) s_cnt++;
			if (s_cnt<2) lst->Add(lbuf);
		}

		if (!saveto_TextUTF8(fnam, lst.get())) msgbox_ERR(LoadUsrMsg(USTR_FaildSave, ExtractFileName(fnam)));
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
//設定パネルの開閉
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
	if (!OptPanel->Visible) RegDirListBox->SetFocus();
}

//---------------------------------------------------------------------------
//フィルタ
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterBtnClick(TObject *Sender)
{
	FilterEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = get_WinColor();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterEditChange(TObject *Sender)
{
	UpdateSpDirList();
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	TListBox *lp = RegDirListBox;

	if (equal_ENTER(KeyStr)) {
		lp->SetFocus();
		if (lp->Count==1) {
			WideChar key = VK_RETURN;
			RegDirListBoxKeyPress(lp, key);
			return;
		}
	}
	else if (contained_wd_i(KeysStr_ToList, KeyStr)) {
		lp->SetFocus();
	}
	else if (MovListBoxFromFilter(lp, KeyStr)) {
		;
	}
	else if (SameText(KeyStr, KeyStr_Migemo)) {
		MigemoAction->Checked = !MigemoAction->Checked;
	}
	else if (equal_ESC(KeyStr)) {
		ModalResult = mrCancel;
	}
	else return;

	lp->Invalidate();
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!IsSpecial && USAME_TI(get_KeyStr(Key, Shift), "Alt+O")) {
		ChgOptBtnClick(NULL);
	}
	else {
		SpecialKeyProc(this, Key, Shift,
			IsSpecial? _T(HELPTOPIC_FL) _T("#SpecialDirList") : _T("hid00054.htm"));
	}
}
//---------------------------------------------------------------------------

