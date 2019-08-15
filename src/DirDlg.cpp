//----------------------------------------------------------------------//
// NyanFi																//
//  �o�^�f�B���N�g��/����t�H���_�ꗗ/�Q��								//
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
	Caption = IsSpecial? "����t�H���_�ꗗ" : "�o�^�f�B���N�g��";
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

	//�w�b�_
	InitializeListHeader(RegDirHeader, _T("�L�[|���O|�ꏊ"));
	THeaderSections *sp = RegDirHeader->Sections;
	sp->Items[0]->MaxWidth	 = IsSpecial? 0 : 1000;
	sp->Items[1]->FixedWidth = IsSpecial;

	TListBox *lp = RegDirListBox;
	lp->Clear();
	set_StdListBox(lp, LBTAG_OPT_FOCS);
	set_UsrScrPanel(ListScrPanel);

	//����t�H���_
	if (IsSpecial) {
		UseEnvVarAction->Visible = true;
		UseEnvVarAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirUseEnv"));
		if (UseEnvVarAction->Checked) lp->Tag |= LBTAG_OPT_SDIR;
		AddNyanFiAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirAddNyanFi"));

		//���ϐ��ϊ����X�g���쐬
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
	//�o�^�f�B���N�g��
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

		//�^�O�����̌��ʃ��X�g
		if (CurStt->is_Find && CurStt->find_TAG) {
			lp->Items->Add(EmptyStr);
			lp->ItemIndex = lp->Count - 1;
			KeyEdit->Text  = EmptyStr;
			DescEdit->Text = EmptyStr;
			UnicodeString tags = CurStt->find_Keywd;
			if (!CurStt->find_And) tags = ReplaceStr(tags, ";", "|");
			DirEdit->Text = "#:" + tags;
		}
		//���̑�
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

		//�g�p���ڂ��Z�p���[�^���l�����Đ擪�ֈړ�
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
//����t�H���_�ꗗ�̍X�V
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::UpdateSpDirList(bool reload)
{
	if (!IsSpecial) return;

	//�ꗗ���擾(���[�N���X�g�`��)
	if (reload) {
		SpDirList->Clear();
		usr_SH->get_SpecialFolderList(SpDirList);

		//NyanFi �ŗL
		if (AddNyanFiAction->Checked) {
			SpDirList->Add("\t-");		//�Z�p���[�^
			SpDirList->Add(ExePath   + "\t���s�p�X");
			SpDirList->Add(TempPathA + "\t�ꎞ�f�B���N�g��");
			if (!DownloadPath.IsEmpty())	SpDirList->Add(DownloadPath + "\t�_�E�����[�h");
			if (usr_Migemo->Available)		SpDirList->Add(to_absolute_name(MigemoPath) + "\tMigemo");
			if (!SpiDir.IsEmpty())			SpDirList->Add(to_absolute_name(SpiDir) + "\tSusie �v���O�C��");
			if (!CmdGitExe.IsEmpty())		SpDirList->AddObject(CmdGitExe + "\tgit.exe",	  		  (TObject*)1);

			//�G�f�B�^
			SpDirList->Add("\t-");
			if (!TextEditor.IsEmpty())		SpDirList->AddObject(TextEditor + "\t�e�L�X�g�G�f�B�^",  (TObject*)1);
			if (!ImageEditor.IsEmpty())		SpDirList->AddObject(ImageEditor + "\t�C���[�W�G�f�B�^",  (TObject*)1);
			if (!BinaryEditor.IsEmpty())	SpDirList->AddObject(BinaryEditor + "\t�o�C�i���G�f�B�^", (TObject*)1);
			//���̑��̃G�f�B�^
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

			//�O���c�[��
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

	//���O�̍ő啝���擾
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
		//��v������̂��Ȃ���΁A��ԋ߂��e��T��
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
//�J�[�\���ʒu���ڂ̃p�X���擾
//---------------------------------------------------------------------------
UnicodeString __fastcall TRegDirDlg::GetCurDirItem(
	bool dsp_sw,	//true = ����t�H���_�̕\�������擾	(default = false)
	bool nam_sw,	//true = ����t�H���_����t��		(default = false)
	bool exe_sw)	//true = ���s�t�@�C�����Ƃ��Ď擾	(default = false)
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
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	TListBox *lp = RegDirListBox;

	//�Ăяo���L�[���H
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
	//�v���p�e�B
	else if (USAME_TI(cmd_F, "PropertyDlg")) {
		UnicodeString dnam = GetCurDirItem();
		if (!dnam.IsEmpty() && !StartsText("shell:", dnam) && !StartsText("#:", dnam)) {
			pos_ListBoxItem(lp);
			ShowPropertyDialog(dnam);
		}
	}
	//�G�N�X�v���[���ŊJ��
	else if (USAME_TI(cmd_F, "OpenByExp")) {
		ClearKeyBuff(true);		//OnKeyPress ��}�~
		OpenByExpAction->Execute();
	}
	//�A�N�Z�����[�^�ňړ�
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
		//�Y���L�[�̓o�^�����J�E���g
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

	//�m��
	if (found) {
		if (IsSelect) {
			CmdStr = jdir;
			ModalResult = mrOk;
		}
		else {
			//���s�t�@�C��
			if (SelIndex!=-1 && (int)lp->Items->Objects[SelIndex]==1) {
				CmdStr.sprintf(_T("JumpTo_\"%s\""), jdir.c_str());
				ModalResult = mrOk;
			}
			//����t�H���_
			else if (StartsStr("shell:", jdir)) {
				CmdStr.sprintf(_T("OpenByExp_\"%s\""), jdir.c_str());
				ModalResult = mrOk;
			}
			//�^�O����
			else if (remove_top_s(jdir, "#:")) {
				NyanFiForm->RecoverFileList2();
				CmdStr.sprintf(_T("FindTag_%s"), jdir.c_str());
				ModalResult = mrOk;
			}
			//�ʏ�f�B���N�g��
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

	//����t�H���_�ꗗ�ł̃C���N�������^���T�[�`�����
	if (IsSpecial && (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN || is_KeyDown(VK_CONTROL))) Key = 0;
}
//---------------------------------------------------------------------------
//�}�E�X����
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
//���ڂ̕`��
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
		//����t�H���_
		if (IsSpecial) {
			TStringDynArray itm_buf = split_strings_tab(lbuf);
			if (itm_buf.Length==2) {
				int xp = Rect.Left + Scaled4;
				int yp = Rect.Top  + get_TopMargin2(cv);
				//�Z�p���[�^
				if (itm_buf[0].IsEmpty()) {
					draw_Separator(cv, Rect);
					cv->Font->Color = col_Folder;
					int s_cnt = 0;
					for (int i=0; i<=Index; i++) if (StartsStr("\t", lp->Items->Strings[i])) s_cnt++;
					UnicodeString snam;
					if (lp->Focused()) {
						snam = (s_cnt==1)? "<All Users (&U)> " : (s_cnt==2)? "<���z�t�H���_ (&V)>" :
							   (s_cnt==3)? "<NyanFi (&N)> "    : (s_cnt==4)? "<�G�f�B�^ (&E)>" :
							   (s_cnt==5)? "<�O���c�[�� (&X)>" : "";
					}
					else {
						snam = (s_cnt==1)? "<All Users> " : (s_cnt==2)? "<���z�t�H���_>" :
							   (s_cnt==3)? "<NyanFi>"     : (s_cnt==4)? "<�G�f�B�^>" :
							   (s_cnt==5)? "<�O���c�[��>" : "";
					}
					if (!snam.IsEmpty()) {
						TRect rc = Rect; rc.Left = xp; rc.Top  = yp;
						::DrawText(cv->Handle, snam.c_str(), -1, &rc, DT_LEFT);
					}
				}
				//����
				else {
					UnicodeString dnam = itm_buf[0];
					bool is_exe = ((int)lp->Items->Objects[Index]==1);
					//�A�C�R��
					if (is_exe && ShowIconAction->Checked) {
						draw_SmallIconF(dnam, cv, xp, std::max(yp + (cv->TextHeight("Q") - SIcoSize)/2, 0));
						xp += get_IcoWidth();
					}
					//���O
					THeaderSections *sp = RegDirHeader->Sections;
					cv->Font->Color = col_fgList;
					UnicodeString inam = itm_buf[1];
					bool brk = remove_top_s(inam, '|');
					cv->TextOut(xp, yp, inam);
					xp = sp->Items[1]->Width + 1;
					//�ꏊ
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
					//��؂��
					if (brk) draw_separateLine(cv, Rect);
				}
			}
		}
		//�o�^�f�B���N�g��
		else {
			TStringDynArray itm_buf = get_csv_array(lbuf, 3, true);
			//�Z�p���[�^
			if (is_separator(itm_buf[1])) {
				draw_Separator(cv, Rect);
			}
			//����
			else {
				THeaderSections *sp = RegDirHeader->Sections;
				//�L�[
				cv->Font->Color = col_fgList;
				cv->Font->Style = cv->Font->Style << fsBold;
				int s_wd = cv->TextWidth(itm_buf[0]);
				int c_wd = sp->Items[0]->Width - 4;
				int xp = Rect.Left + Scaled2;
				int yp = Rect.Top  + get_TopMargin2(cv);
				if (s_wd<c_wd) xp += (c_wd - s_wd)/2;	//�Z���^�����O
				cv->TextOut(xp, yp, itm_buf[0]);
				//���O
				xp = sp->Items[0]->Width + 1;
				cv->Font->Style = cv->Font->Style >> fsBold;
				cv->TextOut(xp, yp, itm_buf[1]);
				xp += sp->Items[1]->Width + 1;
				//�ꏊ
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

	//�J�[�\��
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
//���ڂ̒ǉ��E�ύX
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::ChangeItemActionExecute(
	int chg_mod)	//�ύX���[�h 0:�ǉ�/ 1:�ύX/ 2:�}��
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
		case 0:	//�ǉ�
			if (lp->Items->Strings[lp->Count - 1].IsEmpty())
				lp->Items->Strings[lp->Count - 1] = lbuf;
			else
				lp->Items->Add(lbuf);
			lp->ItemIndex = lp->Count - 1;
			break;
		case 1:	//�ύX
			if (lp->ItemIndex==-1) return;
			lp->Items->Strings[lp->ItemIndex] = lbuf;
			break;
		case 2:	//�}��
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
//�ǉ�
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionExecute(TObject *Sender)
{
	ChangeItemActionExecute(0);
}
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (!IsSpecial && !DescEdit->Text.IsEmpty());
	if (!IsSpecial) Caption = IsAddMode? "�o�^�f�B���N�g��(�ǉ�)" : "�o�^�f�B���N�g��";
}
//---------------------------------------------------------------------------
//�ύX/�}��
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
		ap->Caption = "�}��";
		ap->Enabled = (!IsSpecial && lp->ItemIndex!=-1 && !DescEdit->Text.IsEmpty());
	}
	else {
		ap->Caption = "�ύX";
		ap->Enabled = (!IsSpecial && lp->ItemIndex!=-1 && !lp->Items->Strings[lp->ItemIndex].IsEmpty());
	}
}
//---------------------------------------------------------------------------
//�폜
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
//�f�B���N�g���̎Q��
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = DirEdit->Text;
	if (UserModule->SelectDirEx(_T("�o�^�f�B���N�g��"), dnam)) DirEdit->Text = to_path_name(dnam);
}
//---------------------------------------------------------------------------
//����t�H���_�̎Q��
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RefSpBtnClick(TObject *Sender)
{
	TRegDirDlg *SelSpDirDlg = new TRegDirDlg(this);		//�����Ƃ��ɔj��
	SelSpDirDlg->IsSpecial	= true;
	SelSpDirDlg->IsSelect	= true;
	if (SelSpDirDlg->ShowModal()==mrOk) {
		DirEdit->Text  = get_pre_tab(SelSpDirDlg->CmdStr);
		DescEdit->Text = get_post_tab(SelSpDirDlg->CmdStr);
		KeyEdit->SetFocus();
	}
}

//---------------------------------------------------------------------------
//�p�X�����R�s�[
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
//���ϐ���p���ăp�X����\��
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
//�G�N�X�v���[���ŊJ��
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
//�f�B���N�g���̃v���p�e�B
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
//�A�v���P�[�V�������
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
//�ꗗ�����[�N���X�g�Ƃ��ĕۑ�
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SaveAsWorkActionExecute(TObject *Sender)
{
	UserModule->PrepareSaveDlg(LoadUsrMsg(USTR_SaveAs, _T("���[�N���X�g")).c_str(), 
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
//�ݒ�p�l���̊J��
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
//�t�B���^
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

