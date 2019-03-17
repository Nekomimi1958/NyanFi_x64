//----------------------------------------------------------------------//
// NyanFi																//
//  �o�^�f�B���N�g��/����t�H���_�ꗗ									//
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
	IsAddMode = false;
}

//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::FormShow(TObject *Sender)
{
	Caption = IsSpecial? "����t�H���_�ꗗ" : "�o�^�f�B���N�g��";
	CmdStr	= EmptyStr;

	IniFile->LoadPosInfo(this, DialogCenter, (IsSpecial? _T("SpecialDirList") : null_TCHAR));

	InitializeListHeader(RegDirHeader, _T("�L�[|���O|�ꏊ"));
	THeaderSections *sp = RegDirHeader->Sections;
	sp->Items[0]->MaxWidth = IsSpecial? 0 : 1000;

	TListBox *lp = RegDirListBox;
	lp->Clear();
	set_StdListBox(lp, LBTAG_OPT_FOCS);
	set_UsrScrPanel(ListScrPanel);

	//����t�H���_
	if (IsSpecial) {
		UseEnvVarAction->Checked = IniFile->ReadBoolGen(_T("SpecialDirUseEnv"));
		if (UseEnvVarAction->Checked) lp->Tag |= LBTAG_OPT_SDIR;

		//���ϐ��ϊ����X�g���쐬
		EnvVarList->Clear();
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%APPDATA%")) + "\t%APPDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%LOCALAPPDATA%")) + "\t%LOCALAPPDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PUBLIC%")) + "\t%PUBLIC%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMDATA%")) + "\t%PROGRAMDATA%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%USERPROFILE%")) + "\t%USERPROFILE%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%WINDIR%")) + "\t%WINDIR%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES(X86)%")) + "\t%PROGRAMFILES(X86)%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES%")) + "\t%PROGRAMFILES%");
		EnvVarList->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMW6432%")) + "\t%PROGRAMW6432%");

		OptPanel->Visible	= false;
		BlankPanel->Visible = false;

		//�ꗗ���擾(���[�N���X�g�`��)
		std::unique_ptr<TStringList> lst(new TStringList());
		usr_SH->get_SpecialFolderList(lst.get());

		//���O�̍ő啝���擾
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
	//�o�^�f�B���N�g��
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
		IniFile->WriteBoolGen(_T("SpecialDirUseEnv"),	UseEnvVarAction);
	}
	else {
		TListBox *lp = RegDirListBox;
		int i = 0;
		while (i<lp->Count) {
			UnicodeString lbuf = lp->Items->Strings[i];
			if (lbuf.IsEmpty())
				lp->Items->Delete(i);
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

	IniFile->SavePosInfo(this, (IsSpecial? _T("SpecialDirList"): null_TCHAR));

	IsSpecial = false;
	IsAddMode = false;
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
		//��v������̂��Ȃ���΁A��ԋ߂��e��T��
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
//�J�[�\���ʒu���ڂ̃p�X���擾
//---------------------------------------------------------------------------
UnicodeString __fastcall TRegDirDlg::GetCurDirItem(
	bool dsp_sw,	//true = ����t�H���_�̕\�������擾	(default = false)
	bool id_sw)		//true = CSIDL���擾				(default = false)
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
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::RegDirListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	TListBox *lp = RegDirListBox;

	//�Ăяo���L�[���H
	bool k_used = false;
	for (int i=0; i<lp->Count && !k_used; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], 3, true);
		k_used = SameText(KeyStr, get_tkn(lp->Items->Strings[i], ','));
	}

	bool handled = true;
	if (k_used)
		handled = false;
	else if (equal_UP(KeyStr) && lp->ItemIndex==0)
		lp->ItemIndex = lp->Count - 1;
	else if (equal_DOWN(KeyStr) && lp->ItemIndex==lp->Count-1)
		lp->ItemIndex = 0;
	else if (contained_wd_i(KeysStr_Popup, KeyStr))
		show_PopupMenu(lp);
	else if (IsSpecial && ExeCmdListBox(lp, cmd_F))
		;
	else if (!IsSpecial && UserModule->ListBoxOpeItem(KeyStr))
		;
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
	//�E�N���b�N���j���[
	else if (StartsText("ContextMenu", cmd_F)) {
		show_PopupMenu(lp);
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

		if		(f_cnt==0) beep_Warn();
		else if (f_cnt==1) found = true;
	}

	//�m��
	if (found) {
		//����t�H���_
		if (StartsStr("shell:", jdir)) {
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
			else msgbox_ERR(USTR_CantAccessDir);
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

	int idx = RegDirListBox->ItemIndex;  if (idx==-1) return;
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
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (!lbuf.IsEmpty()) {
		//����t�H���_
		if (IsSpecial) {
			TStringDynArray itm_buf = split_strings_tab(lbuf);
			if (itm_buf.Length==2) {
				int xp = Rect.Left + 4;
				int yp = Rect.Top + get_TopMargin2(cv);
				//�Z�p���[�^
				if (itm_buf[0].IsEmpty()) {
					draw_Separator(cv, Rect);
					cv->Font->Color = col_Folder;
					int tag = (int)lp->Items->Objects[Index];
					if (tag>0) cv->TextOut(xp, yp, (tag==2)? "<���z�t�H���_>" : "<All Users> ");
				}
				//����
				else {
					//���O
					THeaderSections *sp = RegDirHeader->Sections;
					cv->Font->Color = col_fgList;
					UnicodeString inam = itm_buf[1];
					bool brk = remove_top_s(inam, '|');
					cv->TextOut(xp, yp, inam);
					xp = sp->Items[1]->Width + 1;
					//�ꏊ
					UnicodeString dnam = itm_buf[0];
					if (UseEnvVarAction->Checked) {
						if (contained_wd_i(_T("TEMP|TMP"), itm_buf[1])) dnam = "%" + itm_buf[1] + "%";
						else dnam = ExcludeTrailingPathDelimiter(replace_str_by_list(dnam, EnvVarList));
						if (remove_top_s(dnam, '%')) {
							UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
							cv->Font->Color = AdjustColor(col_Folder, 72);
							cv->TextOut(xp, yp, envstr);
							xp += get_TextWidth(cv, envstr, is_irreg) + 2;
						}
					}
					cv->Font->Color = (StartsStr("shell:", dnam))? AdjustColor(col_Folder, 72) : col_Folder;
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
			if (is_separator(itm_buf[1]))
				draw_Separator(cv, Rect);
			//����
			else {
				THeaderSections *sp = RegDirHeader->Sections;
				//�L�[
				cv->Font->Color = col_fgList;
				cv->Font->Style = cv->Font->Style << fsBold;
				int s_wd = cv->TextWidth(itm_buf[0]);
				int c_wd = sp->Items[0]->Width - 4;
				int xp = Rect.Left + 2;
				int yp = Rect.Top + get_TopMargin2(cv);
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
					cv->Font->Color = AdjustColor(col_Folder, 72);
					usr_TAG->DrawTags(dnam, cv, xp, yp, RevTagCololr? col_bgList : clNone);
				}
				else {
					if (remove_top_s(dnam, '%')) {
						UnicodeString envstr = "%" + split_tkn(dnam, '%') + "%";
						cv->Font->Color = AdjustColor(col_Folder, 72);
						cv->TextOut(xp, yp, envstr);
						xp += get_TextWidth(cv, envstr, is_irreg) + 2;
					}
					cv->Font->Color = (StartsStr("shell:", dnam))? AdjustColor(col_Folder, 72) : col_Folder;
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
	else msgbox_WARN(USTR_DirNotFound);
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
void __fastcall TRegDirDlg::RefDirButtonClick(TObject *Sender)
{
	UnicodeString dnam = DirEdit->Text;
	if (UserModule->SelectDirEx(_T("�o�^�f�B���N�g��"), dnam)) DirEdit->Text = to_path_name(dnam);
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
void __fastcall TRegDirDlg::UseEnvVarActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = IsSpecial;
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
//�ꗗ�����[�N���X�g�Ƃ��ĕۑ�
//---------------------------------------------------------------------------
void __fastcall TRegDirDlg::SaveAsWorkActionExecute(TObject *Sender)
{
	UnicodeString fnam;
	UserModule->PrepareSaveDlg(LoadUsrMsg(USTR_SaveAs, _T("���[�N���X�g")).c_str(), 
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
void __fastcall TRegDirDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!IsSpecial && USAME_TI(get_KeyStr(Key, Shift), "Alt+O"))
		ChgOptBtnClick(NULL);
	else
		SpecialKeyProc(this, Key, Shift,
			IsSpecial? _T(HELPTOPIC_FL) _T("#SpecialDirList") : _T("hid00054.htm"));
}
//---------------------------------------------------------------------------

