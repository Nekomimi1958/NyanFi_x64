//----------------------------------------------------------------------//
// NyanFi																//
//  �U�蕪���_�C�A���O													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "DistrDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDistributionDlg *DistributionDlg = NULL;

//---------------------------------------------------------------------------
//�v���r���[�ꗗ�\�[�g�p��r�֐�
//---------------------------------------------------------------------------
//�t�@�C����
int __fastcall DistrCmp_Name(TStringList *List, int Index1, int Index2)
{
	UnicodeString name1 = ExtractFileName(ExcludeTrailingPathDelimiter(get_pre_tab(List->Strings[Index1])));
	UnicodeString name2 = ExtractFileName(ExcludeTrailingPathDelimiter(get_pre_tab(List->Strings[Index2])));
	return StrCmpLogicalW(name1.c_str(), name2.c_str());
}
//---------------------------------------------------------------------------
//�U�蕪����
int __fastcall DistrCmp_Dest(TStringList *List, int Index1, int Index2)
{
	UnicodeString dst1 = get_post_tab(List->Strings[Index1]);
	UnicodeString dst2 = get_post_tab(List->Strings[Index2]);
	return SameText(dst1, dst2)?
		DistrCmp_Name(List, Index1, Index2) : StrCmpLogicalW(dst1.c_str(), dst2.c_str());
}

//---------------------------------------------------------------------------
// TDistributionDlg �N���X
//---------------------------------------------------------------------------
__fastcall TDistributionDlg::TDistributionDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	UserModule->SetUsrPopupMenu(this);

	DistrDirEdit->Tag = EDTAG_DST_FMT;

	ItemList  = new TStringList();
	DistrList = new TStringList();

	set_ComboBoxText(SameNameComboBox,
		_T("�����㏑��\n")
		_T("�ŐV�Ȃ�㏑��\n")
		_T("�X�L�b�v\n")
		_T("�����I�ɖ��O��ύX\n"));

	SttPrgBar = new SttProgressBar(StatusBar1, 2);
	SttPrgBar->MsgIndex = 1;
	ImmediateExe = false;
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	UnicodeString tit = "�U�蕪�� - ";

	//�o�^�t�@�C��������΂����ǂݍ���
	DistrFile = to_absolute_name(IniFile->ReadStrGen(_T("DistrDlgFileName"),	DISTR_FILE));
	if (file_exists(DistrFile)) {
		LoadDistrFile();
		tit += ExtractFileName(DistrFile);
	}
	else {
		//�o�^�t�@�C�����Ȃ��ꍇ�A��d�N�����ł͌����_��INI�t�@�C������擾
		if (!IsPrimary) {
			std::unique_ptr<UsrIniFile> tmp_ini_file(new UsrIniFile(IniFile->FileName));
			DistrDefList->Clear();
			tmp_ini_file->LoadListItems("DistrDefList", DistrDefList, 200, false);
			RegEnabled = false;
		}
		else {
			RegEnabled = true;
		}
		tit += ExtractFileName(IniFile->FileName);
	}

	Caption = tit;
	ListListBox->Clear();

	MainPanel->Height			 = IniFile->ReadIntGen( _T("DistrDlgPanelHeight"),	260);
	ListListBox->Width			 = IniFile->ReadIntGen( _T("DistrDlgPrvLstWidth"),	150);
	SameNameComboBox->ItemIndex  = IniFile->ReadIntGen( _T("DistrDlgCopyMode"));
	CreDistrDirCheckBox->Checked = IniFile->ReadBoolGen(_T("DistrDlgCreDir"),		true);
	GroupCheckAction->Checked	 = IniFile->ReadBoolGen(_T("DistrDlgGroupChek"));
	PrvListAction->Checked		 = IniFile->ReadBoolGen(_T("DistrDlgPrvLstFile"));

	set_ListBoxItemHi(RegListBox);
	set_ListBoxItemHi(ListListBox);
	set_ListBoxItemHi(PrvListBox);

	PrvSplitter->Color	= col_Splitter;
	ListSplitter->Color = col_Splitter;

	SttPrgBar->BgColor  = col_bgPrgBar;
	SttPrgBar->BarColor = col_fgPrgBar;

	ListListBox->Visible  = false;
	ListSplitter->Visible = false;
	if (PrvListAction->Checked) {
		ListSplitter->Visible = true;
		ListSplitter->Align   = alLeft;
		ListListBox->Visible  = true;
		ListSplitter->Align   = alRight;
	}

	InitializeListHeader(PrvListHeader, _T("�t�@�C����|�U�蕪����"));
	PrvListHeader->Sections->Items[0]->Width = IniFile->ReadIntGen(_T("DistrDlgPrvHdrW0"), ClientWidth/2);
	PrvListHeader->Sections->Items[1]->Width = ClientWidth - PrvListHeader->Sections->Items[0]->Width;
	PrvSortMode = IniFile->ReadIntGen(_T("DistrDlgSortMode"));
	LastDistDir = IniFile->ReadStrGen(_T("DistrDlgLastDir"),	CurPathName);

	StatusBar1->Canvas->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);

	UserModule->InitializeListBox(RegListBox);
	AssignRegListBox();

	if (!ImmediateExe) IsMove = false;

	if (TitleEdit->Text.IsEmpty()) TitleEdit->SetFocus();

	SkipCount = 0;

	SetDarkWinTheme(this, true);
	Shape1->Pen->Color = IsDarkMode? dcl_BtnShadow : scl_BtnShadow;
	Shape3->Pen->Color = IsDarkMode? dcl_BtnShadow : scl_BtnShadow;
	Shape2->Pen->Color = IsDarkMode? dcl_BtnHighlight : scl_BtnHighlight;
	Shape4->Pen->Color = IsDarkMode? dcl_BtnHighlight : scl_BtnHighlight;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::WmFormShowed(TMessage &msg)
{
	Repaint();
	UpdatePreview();

	//�����Ɏ��s
	if (ImmediateExe) {
		ImmediateExe = false;
		ModalResult  = mrOk;
	}
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	DistrDefList->Assign(RegListBox->Items);

	DistrMaskEdit->Text = EmptyStr;
	DistrDirEdit->Text	= EmptyStr;

	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen( _T("DistrDlgPanelHeight"),	MainPanel->Height);
	IniFile->WriteIntGen( _T("DistrDlgPrvLstWidth"),	ListListBox->Width);
	IniFile->WriteIntGen( _T("DistrDlgCopyMode"),		SameNameComboBox);
	IniFile->WriteBoolGen(_T("DistrDlgCreDir"),			CreDistrDirCheckBox);
	IniFile->WriteBoolGen(_T("DistrDlgGroupChek"),		GroupCheckAction);
	IniFile->WriteBoolGen(_T("DistrDlgPrvLstFile"),		PrvListAction);
	IniFile->WriteIntGen( _T("DistrDlgPrvHdrW0"),		PrvListHeader->Sections->Items[0]->Width);
	IniFile->WriteIntGen( _T("DistrDlgSortMode"),		PrvSortMode);
	IniFile->WriteStrGen( _T("DistrDlgLastDir"),		LastDistDir);

	//�o�^�t�@�C��������΂�����X�V
	if (file_exists(DistrFile)) {
		SaveDistrFile();
	}
	//�o�^�t�@�C�����Ȃ��ꍇ�A���C�����ł�INI�t�@�C���ɕۑ�
	else if (IsPrimary) {
		SaveOptions();
		UpdateIniFile(IniFile);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormDestroy(TObject *Sender)
{
	delete ItemList;
	delete DistrList;

	delete SttPrgBar;
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormResize(TObject *Sender)
{
	PrvListHeader->Sections->Items[1]->Width = ClientWidth - PrvListHeader->Sections->Items[0]->Width;
	PrvListBox->Invalidate();
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̕`��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::StatusBar1DrawPanel(TStatusBar *StatusBar,
	TStatusPanel *Panel, const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = IsDarkMode? col_bgSttBar : scl_BtnFace;
	cv->FillRect(Rect);
	cv->Font->Color = IsDarkMode? col_fgSttBar : scl_BtnText;
	cv->TextOut(Rect.Left + 2, Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//�o�^���e�����X�g�{�b�N�X�Ɋ��蓖��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::AssignRegListBox()
{
	TCheckListBox *lp = RegListBox;
	lp->Tag = RegEnabled? 0 : LBTAG_OPT_INHI;
	lp->Items->Assign(DistrDefList);
	for (int i=0; i<lp->Count; i++) {
		//�^�C�g��,�L��,�}�X�N,�U�蕪����
		lp->Checked[i] = equal_1(get_csv_item(lp->Items->Strings[i], 1));
	}
	lp->ItemIndex = -1;
}

//---------------------------------------------------------------------------
//�U�蕪���o�^�t�@�C���̓ǂݍ���
//---------------------------------------------------------------------------
bool __fastcall TDistributionDlg::LoadDistrFile()
{
	std::unique_ptr<UsrIniFile> distr_file(new UsrIniFile(DistrFile));
	if (distr_file->SectionList->Count==1 && distr_file->RectionExists("DistrDefList")) {
		DistrDefList->Clear();
		distr_file->LoadListItems("DistrDefList", DistrDefList, 200, false);
		RegEnabled = true;
		IniFile->WriteStrGen(_T("DistrDlgFileName"),	to_relative_name(DistrFile));
		return true;
	}

	msgbox_ERR("�s���ȐU�蕪���o�^�t�@�C���ł��B");
	return false;
}
//---------------------------------------------------------------------------
//�U�蕪���o�^�t�@�C���̕ۑ�
//---------------------------------------------------------------------------
bool __fastcall TDistributionDlg::SaveDistrFile()
{
	std::unique_ptr<UsrIniFile> distr_file(new UsrIniFile(DistrFile));
	distr_file->SaveListItems("DistrDefList", DistrDefList, 200);
	if (distr_file->UpdateFile()) return true;

	msgbox_ERR(USTR_FaildSave);
	return false;
}


//---------------------------------------------------------------------------
//�v���r���[�̃w�b�_
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect, (Section->Index==PrvSortMode)?  1 : 0, true, IsDarkMode);
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	PrvSortMode = Section->Index;
	PrvListHeader->Invalidate();
	DistrList->CustomSort((PrvSortMode==1)? DistrCmp_Dest : DistrCmp_Name);
	PrvListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	PrvListHeader->Sections->Items[1]->Width = ClientWidth - PrvListHeader->Sections->Items[0]->Width;
	PrvListBox->Invalidate();
}

//---------------------------------------------------------------------------
//�v���r���[�\���̍X�V
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::UpdatePreview(bool upd)
{
	SkipCount = 0;

	StatusBar1->Panels->Items[0]->Text =
		UnicodeString().sprintf(_T("�o�^:%u/%u"), get_CheckListCount(RegListBox), RegListBox->Count);

	if (!upd) return;

	struct def_rec {
		UnicodeString mask;
		UnicodeString dnam;
		bool is_regex;
	};

	UnicodeString tmp;

	cursor_HourGlass();
	SttPrgBar->Begin(_T("������..."));

	//�K�p���[���̒��o
	TCheckListBox *cp = RegListBox;
	std::unique_ptr<TList> def_lst(new TList());
	for (int i=0; i<cp->Count; i++) {
		if (!cp->Checked[i]) continue;
		TStringDynArray itm_buf = get_csv_array(cp->Items->Strings[i], DISTRLS_CSVITMCNT, true);
		UnicodeString mask = itm_buf[2];
		UnicodeString dnam = itm_buf[3];
		//���X�g�t�@�C��
		if (remove_top_AT(mask)) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			if (load_text_ex(to_absolute_name(mask), fbuf.get())==0) continue;
			for (int j=0; j<fbuf->Count; j++) {
				TStringDynArray l_buf = split_strings_tab(fbuf->Strings[j]);
				if (l_buf.Length!=2) continue;
				UnicodeString l_mask = l_buf[0];
				UnicodeString l_dnam = l_buf[1];
				if (!l_mask.IsEmpty() && !l_dnam.IsEmpty()) {
					bool is_regex = is_regex_slash(l_mask);
					if (is_regex) {
						l_mask = exclude_top_end(l_mask);
						if (!chk_RegExPtn(l_mask)) continue;
					}
					def_rec *dp = new def_rec;
					dp->mask = l_mask;
					dp->dnam = l_dnam;
					dp->is_regex = is_regex;
					def_lst->Add((void*)dp);
				}
			}
		}
		//�ʏ퍀��
		else {
			bool is_regex = is_regex_slash(mask);
			if (is_regex) {
				mask = exclude_top_end(mask);
				if (!chk_RegExPtn(mask)) continue;
			}
			def_rec *dp = new def_rec;
			dp->mask = mask;
			dp->dnam = dnam;
			dp->is_regex = is_regex;
			def_lst->Add((void*)dp);
		}
	}

	//���X�g���N���A
	TListBox *lp = PrvListBox;
	lp->Count = 0;
	TStringList *lst = DistrList;
	lst->Clear();

	int m_cnt = 0;
	int f_cnt = 0;
	int d_cnt = 0;
	for (int i=0; i<ItemList->Count; i++) {
		SttPrgBar->SetPosI(i, ItemList->Count);

		UnicodeString fnam = ItemList->Strings[i];
		UnicodeString nnam = ExtractFileName(ExcludeTrailingPathDelimiter(fnam));

		TRegExOptions opt; opt << roIgnoreCase;
		for (int j=0; j<def_lst->Count; j++) {
			def_rec *dp = (def_rec*)def_lst->Items[j];
			UnicodeString dnam = dp->dnam;
			//���K�\��
			if (dp->is_regex) {
				TMatch mt = TRegEx::Match(nnam, dp->mask, opt);
				if (!mt.Success) continue;
				//\1,\2...��u��
				for (int k=1; k<mt.Groups.Count; k++) {
					if (mt.Groups.Item[k].Success) {
						UnicodeString v = mt.Groups.Item[k].Value;
						dnam = ReplaceStr(dnam, tmp.sprintf(_T("\\%u"), k), mt.Groups.Item[k].Value);
					}
				}
			}
			//�}�X�N�}�b�`
			else if (!str_match(dp->mask, nnam)) continue;

			dnam = format_FileName(dnam, ExcludeTrailingPathDelimiter(fnam));
			lst->Add(tmp.sprintf(_T("%s\t%s"), fnam.c_str(), dnam.c_str()));

			m_cnt++;
			if (!CreDistrDirCheckBox->Checked && !dir_exists(dnam.IsEmpty()? OppPath : to_absolute_name(dnam, OppPath)))
				SkipCount++;
			else if (ends_PathDlmtr(fnam))
				d_cnt++;
			else
				f_cnt++;
			break;
		}
	}

	for (int i=0; i<def_lst->Count; i++)
		delete (def_rec*)def_lst->Items[i];

	//���X�g�{�b�N�X�Ɋ��蓖��(���z)
	lp->Count = DistrList->Count;

	//��ԕ\��
	tmp.sprintf(_T("�}�b�`:%u/%u (%u Dirs/ %u Files"), m_cnt, ItemList->Count, d_cnt, f_cnt);
	if (SkipCount>0) tmp.cat_sprintf(_T("  Skip:%u"), SkipCount);
	tmp += ")";
	SttPrgBar->End(tmp);

	cursor_Default();
}

//---------------------------------------------------------------------------
//�U�蕪����`���X�g�̕`��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RegListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);

	int w_tit = 50;
	int w_msk = 100;
	for (int i=0; i<lp->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], DISTRLS_CSVITMCNT, true);
		w_tit = std::max(cv->TextWidth(itm_buf[0]), w_tit);
		if (!starts_AT(itm_buf[2])) w_msk = std::max(cv->TextWidth(itm_buf[2]), w_msk);
	}

	TStringDynArray cur_buf = get_csv_array(lp->Items->Strings[Index], DISTRLS_CSVITMCNT, true);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);
	cv->Font->Color = get_TextColor(State.Contains(odSelected));
	cv->TextOut(xp, yp, cur_buf[0]);	xp += w_tit + 8;
	cv->TextOut(xp, yp, cur_buf[2]);	xp += w_msk + 4;
	if (!starts_AT(cur_buf[2])) {
		out_Text(cv, xp, yp, _T("��"));	xp += cv->TextWidth("��") + 4;
		cv->TextOut(xp, yp, cur_buf[3]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RegListBoxClickCheck(TObject *Sender)
{
	//�`�F�b�N��Ԃ𔽉f
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		UpdateListItem(idx);
		//����^�C�g���𓯎��Ƀ`�F�b�N
		if (GroupCheckAction->Checked) {
			TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
			UnicodeString tit = itm_buf[0];
			for (int i=0; i<lp->Count; i++) {
				if (i==idx) continue;
				itm_buf = get_csv_array(lp->Items->Strings[i], DISTRLS_CSVITMCNT, true);
				if (SameStr(itm_buf[0], tit)) {
					lp->Checked[i] = lp->Checked[idx];
					UpdateListItem(i);
				}
			}
		}
	}

	UpdatePreview();
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RegListBoxClick(TObject *Sender)
{
	TCheckListBox *cp = RegListBox;
	int idx = cp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray itm_buf = get_csv_array(cp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
		TitleEdit->Text 	= itm_buf[0];
		DistrMaskEdit->Text = itm_buf[2];
		DistrDirEdit->Text	= itm_buf[3];

		//���X�g�t�@�C���̃v���r���[
		TListBox *lp = ListListBox;
		lp->Clear();
		if (lp->Visible) {
			UnicodeString lnam = DistrMaskEdit->Text;
			if (remove_top_AT(lnam)) {
				try {
					lnam = to_absolute_name(lnam);
					if (file_exists(lnam)) {
						std::unique_ptr<TStringList> fbuf(new TStringList());
						if (load_text_ex(lnam, fbuf.get())!=0) {
							lp->Items->Assign(fbuf.get());
							int ok_cnt = 0;
							int ng_cnt = 0;
							for (int i=0; i<fbuf->Count; i++) {
								UnicodeString lbuf = fbuf->Strings[i];
								if (lbuf.IsEmpty()) continue;
								if (split_strings_tab(lbuf).Length==2) ok_cnt++; else ng_cnt++;
							}
							if (ng_cnt>0)
								TextAbort(_T("���X�g�t�@�C���̏������Ԉ���Ă��܂��B\n")
										  _T("�}�X�N/�p�^�[���ƐU�蕪������^�u�ŋ�؂��ċL�q���Ă��������B"));
							else if (ok_cnt==0)
								TextAbort(_T("���X�g�t�@�C���ɗL���ȓo�^���e������܂���B"));
						}
						else UserAbort(USTR_FaildLoad);
					}
					else throw EAbort(LoadUsrMsg(USTR_NotFound, _T("���X�g�t�@�C��")));
				}
				catch (EAbort &e) {
					lp->EndDrag(false);
					msgbox_ERR(e.Message);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RegListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	if (cmd_F.IsEmpty()) cmd_F = get_CsrKeyCmd(KeyStr);

	int last_idx = lp->ItemIndex;
	bool handled = true;
	if		(ExeCmdListBox(lp, cmd_F))				;
	else if (USAME_TI(cmd_F, "FileEdit"))			EditListAction->Execute();
	else if (equal_DEL(KeyStr)) 					DelRegAction->Execute();
	else if (UserModule->ListBoxOpeItem(KeyStr))	;
	else if (ListListBox->Visible && is_ToRightOpe(KeyStr, cmd_F))
													ListListBox->SetFocus();
	else handled = false;

	if (last_idx!=lp->ItemIndex) RegListBoxClick(NULL);

	if (!is_DialogKey(Key) || handled) Key = 0;
}

//---------------------------------------------------------------------------
//���X�g�t�@�C���̃v���r���[��`��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ListListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);
	UnicodeString lbuf = lp->Items->Strings[Index];
	cv->Font->Color = get_TextColor(State.Contains(odSelected));
	if (!lbuf.IsEmpty() && (split_strings_tab(lbuf).Length!=2)) cv->Font->Color = col_Error;
	TabCrTextOut(lbuf, cv, xp, yp, cv->Font->Color);
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ListListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	if (cmd_F.IsEmpty()) cmd_F = get_CsrKeyCmd(KeyStr);

	bool handled = true;
	if		(ExeCmdListBox(lp, cmd_F))		;
	else if (USAME_TI(cmd_F, "FileEdit"))	EditListAction->Execute();
	else if (is_ToLeftOpe(KeyStr, cmd_F))	RegListBox->SetFocus();
	else handled = false;

	if (!is_DialogKey(Key) || handled) Key = 0;
}

//---------------------------------------------------------------------------
//�v���r���[�̕`��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	UnicodeString lbuf = lp->Items->Strings[Index];
	UnicodeString fnam = split_pre_tab(lbuf);
	fnam = ends_PathDlmtr(fnam)? IncludeTrailingPathDelimiter(ExtractFileName(ExcludeTrailingPathDelimiter(fnam)))
							   : ExtractFileName(fnam);
	UnicodeString dnam = lbuf;
	UnicodeString anam = dnam.IsEmpty()? OppPath : to_absolute_name(dnam, OppPath);

	THeaderSections *sp = PrvListHeader->Sections;
	int wd = sp->Items[0]->Width;
	cv->TextOut(xp, yp, minimize_str(fnam, cv, wd, OmitEndOfName));	xp += wd + 4;
	out_Text(cv, xp, yp, _T("��"));	xp += cv->TextWidth("��") + 4;

	cv->Font->Color = (!CreDistrDirCheckBox->Checked && !dir_exists(anam))? col_Error : get_TextColor();
	cv->TextOut(xp, yp, yen_to_delimiter(dnam));
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<DistrList->Count)? DistrList->Strings[Index] : EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListBoxDblClick(TObject *Sender)
{
	//�Y������o�^���ڂ�����
	TListBox *lp = (TListBox*)Sender;
	if (lp->ItemIndex!=-1) {
		UnicodeString nnam = ExtractFileName(ExcludeTrailingPathDelimiter(get_pre_tab(lp->Items->Strings[lp->ItemIndex])));
		TCheckListBox *cp = RegListBox;
		cp->ItemIndex = -1;
		for (int i=0; i<cp->Count && cp->ItemIndex==-1; i++) {
			if (!cp->Checked[i]) continue;
			UnicodeString mask = get_csv_item(cp->Items->Strings[i], 2);
			//���X�g�t�@�C��
			if (remove_top_AT(mask)) {
				UnicodeString lnam = to_absolute_name(mask);
				std::unique_ptr<TStringList> fbuf(new TStringList());
				if (load_text_ex(lnam, fbuf.get())!=0) {
					for (int j=0; j<fbuf->Count; j++) {
						TStringDynArray l_buf = split_strings_tab(fbuf->Strings[j]);
						if (l_buf.Length==2 && MatchRegMask(l_buf[0], nnam)) {
							cp->ItemIndex = i;
							RegListBoxClick(NULL);
							if (ListListBox->Visible) ListListBox->ItemIndex = j;
							break;
						}
					}
				}
			}
			//�ʏ퍀��
			else if (MatchRegMask(mask, nnam)) {
				cp->ItemIndex = i;
				RegListBoxClick(NULL);
			}
		}
	}
}
//---------------------------------------------------------------------------
//�t�@�C�������}�X�N�Ƀ}�b�`���邩?
//---------------------------------------------------------------------------
bool __fastcall TDistributionDlg::MatchRegMask(
	UnicodeString mask,		//�}�X�N
	UnicodeString nnam)		//�t�@�C����
{
	if (is_regex_slash(mask)) {
		mask = exclude_top_end(mask);
		if (!chk_RegExPtn(mask)) return false;
		TRegExOptions opt; opt << roIgnoreCase;
		return TRegEx::Match(nnam, mask, opt).Success;
	}

	return str_match(mask, nnam);
}

//---------------------------------------------------------------------------
//�`�F�b�N��Ԃ��f�[�^�ɔ��f
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::UpdateListItem(int idx)
{
	TCheckListBox *lp = RegListBox;
	TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
	itm_buf[1].sprintf(_T("%s"), lp->Checked[idx]? _T("1") : _T("0"));
	lp->Items->Strings[idx] = make_csv_rec_str(itm_buf);
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::CheckBtnClick(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int chk_cnt = get_CheckListCount(lp);
	for (int i=0; i<lp->Count; i++) {
		lp->Checked[i] = (chk_cnt==0);
		UpdateListItem(i);
	}
	UpdatePreview();
}

//---------------------------------------------------------------------------
//�o�^���ڕ�������쐬
//---------------------------------------------------------------------------
UnicodeString __fastcall TDistributionDlg::MakeRegItem(int idx)
{
	return (UnicodeString().cat_sprintf(_T("%s,\"%u\",%s,%s"),
				make_csv_str(TitleEdit->Text).c_str(),
				((idx!=-1 && RegListBox->Checked[idx])? 1 : 0),
				make_csv_str(DistrMaskEdit->Text).c_str(),
				make_csv_str(!starts_AT(DistrMaskEdit->Text)? DistrDirEdit->Text : EmptyStr).c_str()));
}

//---------------------------------------------------------------------------
//�o�^
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::AddRegActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	bool found = false;
	for (int i=0; i<lp->Count && !found; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], DISTRLS_CSVITMCNT, true);
		found = (SameText(DistrMaskEdit->Text, itm_buf[2]) && SameText(DistrDirEdit->Text, itm_buf[3]));
	}
	if (!found) {
		lp->ItemIndex = lp->Items->Add(MakeRegItem());
		UpdatePreview();
	}
	else {
		msgbox_WARN(LoadUsrMsg(USTR_Registered));
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::AddRegActionUpdate(TObject *Sender)
{
	UnicodeString mask = DistrMaskEdit->Text;
	bool regex_ng = (is_regex_slash(mask) && !chk_RegExPtn(exclude_top_end(mask)));
	bool is_lstfl = starts_AT(mask);
	bool lstfl_ng = is_lstfl? !file_exists(to_absolute_name(get_tkn_r(mask, '@'))) : false;

	set_ErrColor(DistrMaskEdit, regex_ng || lstfl_ng);

	DistrMaskEdit->Tag	  = EDTAG_RGEX_V|EDTAG_RGEX_E;
	DistrDirEdit->Enabled = !is_lstfl;
	RefDirButton->Enabled = !is_lstfl;

	((TAction*)Sender)->Enabled =
		(RegEnabled && !TitleEdit->Text.IsEmpty() && !mask.IsEmpty() && !regex_ng && !lstfl_ng && RegListBox->Count<200);	//***
}
//---------------------------------------------------------------------------
//�ύX
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ChgRegActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		lp->Items->Strings[idx] = MakeRegItem(idx);
		UpdatePreview(lp->Checked[idx]);
	}
}
//---------------------------------------------------------------------------
//�폜
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::DelRegActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		bool is_chk = lp->Checked[idx];
		delete_ListItem(RegListBox);
		if (is_chk) RegListBoxClick(NULL);
		UpdatePreview(is_chk);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ChgRegActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (RegEnabled && RegListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//���X�g�t�@�C���̕ҏW
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::EditListActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
		UnicodeString lnam = itm_buf[2];
		if (remove_top_AT(lnam)) {
			lnam = to_absolute_name(lnam);
			if (file_exists(lnam)) {
				open_by_TextEditor(lnam, ListListBox->Focused()? ListListBox->ItemIndex + 1 : 1);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::EditListActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	TCheckListBox *lp = RegListBox;
	int idx = lp->ItemIndex;
	ap->Enabled = (idx!=-1);
	if (ap->Enabled) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
		UnicodeString lnam = itm_buf[2];
		ap->Enabled = remove_top_AT(lnam)? file_exists(to_absolute_name(lnam)) : false;
	}
}
//---------------------------------------------------------------------------
//���X�g�t�@�C���̎Q��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RefListBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("���X�g�t�@�C���̎w��"), F_FILTER_TXT, NULL);

	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam, true)) {
		DistrMaskEdit->Text = "@" + fnam;
		DistrDirEdit->Text	= EmptyStr;
	}
}
//---------------------------------------------------------------------------
//�U�蕪����̎Q��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::RefDirButtonClick(TObject *Sender)
{
	if (UserModule->SelectDirEx(_T("�U�蕪����"), LastDistDir))
		DistrDirEdit->Text = ReplaceStr(ExcludeTrailingPathDelimiter(LastDistDir), "\\", "\\\\");
}

//---------------------------------------------------------------------------
//�o�^���ڂ̌���
//---------------------------------------------------------------------------
bool __fastcall TDistributionDlg::MatchRegItem(int idx)
{
	bool found = false;
	TCheckListBox *lp = RegListBox;
	UnicodeString kwd = FindEdit->Text;
	if (!kwd.IsEmpty() && idx>=0 && idx<lp->Count) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], DISTRLS_CSVITMCNT, true);
		found = (pos_i(kwd, itm_buf[0])>0 || pos_i(kwd, itm_buf[2])>0 || pos_i(kwd, itm_buf[3])>0);
	}
	return found;
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindEditChange(TObject *Sender)
{
	int idx = -1;
	if (!FindEdit->Text.IsEmpty()) for (int i=0; i<RegListBox->Count && idx==-1; i++) if (MatchRegItem(i)) idx = i;
	RegListBox->ItemIndex = idx;
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!Shift.Empty()) return;

	if		(Key==VK_UP)	{ FindUpAction->Execute();		Key = 0; }
	else if (Key==VK_DOWN)	{ FindDownAction->Execute();	Key = 0; }
}
//---------------------------------------------------------------------------
//�������Ɍ���
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindDownActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	if (!FindEdit->Text.IsEmpty() && lp->Count>0) {
		int i0 = (lp->ItemIndex!=-1)? lp->ItemIndex : 0;
		if (MatchRegItem(i0)) i0++;
		int idx = -1;
		for (int i=i0; i<lp->Count && idx==-1; i++) if (MatchRegItem(i)) idx = i;
		if (idx!=-1) lp->ItemIndex = idx; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindDownActionUpdate(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = -1;
	for (int i=lp->Count-1; i>=0 && idx==-1; i--) if (MatchRegItem(i)) idx = i;
	((TAction*)Sender)->Enabled = (idx!=-1 && idx>lp->ItemIndex);
}
//---------------------------------------------------------------------------
//������Ɍ���
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindUpActionExecute(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	if (!FindEdit->Text.IsEmpty() && lp->ItemIndex>0) {
		int i0 = lp->ItemIndex;
		if (MatchRegItem(i0)) i0--;
		int idx = -1;
		for (int i=i0; i>=0 && idx==-1; i--) if (MatchRegItem(i)) idx = i;
		if (idx!=-1) lp->ItemIndex = idx; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FindUpActionUpdate(TObject *Sender)
{
	TCheckListBox *lp = RegListBox;
	int idx = -1;
	for (int i=0; i<lp->Count && idx==-1; i++) if (MatchRegItem(i)) idx = i;
	((TAction*)Sender)->Enabled = (idx!=-1 && idx<lp->ItemIndex);
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::CreDistrDirCheckBoxClick(TObject *Sender)
{
	UpdatePreview();
}

//---------------------------------------------------------------------------
//�R�s�[�J�n
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ExeCopyActionExecute(TObject *Sender)
{
	if (msgbox_Sure(_T("�U�蕪��(�R�s�[)���J�n���܂���?"), SureCopy)) {
		IsMove		= false;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
//�ړ��J�n
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ExeMoveActionExecute(TObject *Sender)
{
	if (msgbox_Sure(_T("�U�蕪��(�ړ�)���J�n���܂���?"), SureMove)) {
		IsMove		= true;
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::ExeActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (PrvListBox->Count>0);
}

//---------------------------------------------------------------------------
//�U�蕪���o�^�t�@�C�����쐬
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::MakeFileActionExecute(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("�U�蕪���o�^�t�@�C���̍쐬"), F_FILTER_INI, _T(DISTR_FILE));
	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) {
		DistrFile = fnam;
		if (SaveDistrFile()) {
			IniFile->WriteStrGen(_T("DistrDlgFileName"),	to_relative_name(DistrFile));
			Caption = "�U�蕪�� - " + ExtractFileName(DistrFile);
			msgbox_OK(UnicodeString().sprintf(
				_T("�U�蕪���o�^�t�@�C�� %s ���쐬���܂����B"), ExtractFileName(DistrFile).c_str()));
		}
	}
}
//---------------------------------------------------------------------------
//�U�蕪���o�^�t�@�C����I��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::SelFileActionExecute(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("�U�蕪���o�^�t�@�C���̑I��"), F_FILTER_INI, _T("*.INI"));
	UnicodeString fnam = UserModule->OpenDlgExecute();
	if (!fnam.IsEmpty()) {
		DistrFile = fnam;
		if (LoadDistrFile()) {
			Caption = "�U�蕪�� - " + ExtractFileName(DistrFile);
			AssignRegListBox();
			UpdatePreview();
		}
	}
}

//---------------------------------------------------------------------------
//����^�C�g���𓯎��ɐݒ�
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::GroupCheckActionExecute(TObject *Sender)
{
	GroupCheckAction->Checked = !GroupCheckAction->Checked;
}

//---------------------------------------------------------------------------
//���X�g�t�@�C���̃v���r���[��\��
//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::PrvListActionExecute(TObject *Sender)
{
	PrvListAction->Checked = !PrvListAction->Checked;
	if (PrvListAction->Checked) {
		ListSplitter->Visible = true;
		ListSplitter->Align   = alLeft;
		ListListBox->Visible  = true;
		ListSplitter->Align   = alRight;
		RegListBoxClick(NULL);
	}
	else {
		ListSplitter->Visible = false;
		ListListBox->Visible  = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TDistributionDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

