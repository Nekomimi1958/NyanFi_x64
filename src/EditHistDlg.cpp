//----------------------------------------------------------------------//
// NyanFi																//
//  �ŋߕҏW/�{������/�g�����t�@�C���ꗗ								//
//  �x�}�[�N�ꗗ/ ���|�W�g���ꗗ/ �_�C���N�g�^�O�W�����v				//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "usr_excmd.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpExDlg.h"
#include "FileInfDlg.h"
#include "MainFrm.h"
#include "EditHistDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditHistoryDlg *EditHistoryDlg;

//---------------------------------------------------------------------------
__fastcall TEditHistoryDlg::TEditHistoryDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	GridScrPanel = new UsrScrollPanel(GridPanel, EditHistGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);

	isView = isRecent = isMark = isRepo = isTags = isTagPtn = false;
	ToFilter = false;

	HistoryList = NULL;
	HistBufList = new TStringList();
	TagJumpList = new TStringList();
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormShow(TObject *Sender)
{
	DlgInitialized = false;

	TStringGrid *gp = EditHistGrid;
	InitializeListGrid(gp);

	IniFile->LoadPosInfo(this, DialogCenter,
		isRecent? "RecentListDlg" : isMark? "MarkListDlg" : isRepo? "RepoListDlg" : isTags? "TagJumpDlg" : "");

	setup_ToolBar(OpeToolBar);
	FilterEdit->Width = IniFile->ReadIntGen(_T("EditHistFilterWidth"),	200);
	FilterEdit->Color = ToFilter? scl_Window : col_Invalid;

	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);
	StatusBar1->Panels->Items[0]->Width = StatusBar1->ClientWidth;
	StatusBar1->Visible = false;

	bool has_mode = (!isRecent && !isMark && !isRepo);
	OptMode0Action->Enabled = has_mode;
	OptMode0Action->Visible = has_mode;
	OptMode1Action->Enabled = has_mode;
	OptMode1Action->Visible = has_mode;
	OptMode2Action->Enabled = has_mode;
	OptMode2Action->Visible = has_mode;

	OpenAction->Enabled = isMark;
	OpenAction->Visible = isMark;
	MemoAction->Enabled = isMark;
	MemoAction->Visible = isMark;

	if (isRecent) {
		gp->Name = "RecentListGrid";
		gp->ColCount = 5;
		IniFile->LoadGridColWidth(gp, 5, 24,200,50,120,200);
		ListSortMode   = IniFile->ReadIntGen(_T("RecentListSortMode"));
		ListSortOdr[0] = IniFile->ReadIntGen(_T("RecentListSortOdr0"),  1);
		ListSortOdr[1] = IniFile->ReadIntGen(_T("RecentListSortOdr1"),  1);
		ListSortOdr[2] = IniFile->ReadIntGen(_T("RecentListSortOdr2"), -1);
		ListSortOdr[3] = IniFile->ReadIntGen(_T("RecentListSortOdr3"),  1);
		ShowUsedTimeAction->Checked  = IniFile->ReadBoolGen(_T("RecentListUsedTime"), true);
		ShowStatusBarAction->Checked = IniFile->ReadBoolGen(_T("RecentListSttBar"),	  true);
	}
	else if (isMark) {
		gp->Name = "MarkListGrid";
		gp->ColCount = 7;
		IniFile->LoadGridColWidth(gp, 7, 24,200,50,120,200,100,100);
		ListSortMode   = IniFile->ReadIntGen(_T("MarkListSortMode"));
		ListSortOdr[0] = IniFile->ReadIntGen(_T("MarkListSortOdr0"),  1);
		ListSortOdr[1] = IniFile->ReadIntGen(_T("MarkListSortOdr1"),  1);
		ListSortOdr[2] = IniFile->ReadIntGen(_T("MarkListSortOdr2"), -1);
		ListSortOdr[3] = IniFile->ReadIntGen(_T("MarkListSortOdr3"),  1);
		ListSortOdr[4] = IniFile->ReadIntGen(_T("MarkListSortOdr4"),  1);
		ListSortOdr[5] = IniFile->ReadIntGen(_T("MarkListSortOdr5"), -1);
		ShowStatusBarAction->Checked = IniFile->ReadBoolGen(_T("MarkListSttBar"),	true);
	}
	else if (isRepo) {
		gp->Name = "RepoListGrid";
		gp->ColCount = 6;
		IniFile->LoadGridColWidth(gp, 6, 24,100,120,200,100,80);
		ListSortMode   = IniFile->ReadIntGen(_T("RepoListSortMode"));
		ListSortOdr[0] = IniFile->ReadIntGen(_T("RepoListSortOdr0"),  1);
		ListSortOdr[1] = IniFile->ReadIntGen(_T("RepoListSortOdr1"),  1);
		ListSortOdr[2] = IniFile->ReadIntGen(_T("RepoListSortOdr2"), -1);
		ListSortOdr[3] = IniFile->ReadIntGen(_T("RepoListSortOdr3"),  1);
		ListSortOdr[4] = IniFile->ReadIntGen(_T("RepoListSortOdr4"),  1);
		ShowStatusBarAction->Checked = IniFile->ReadBoolGen(_T("RepoListSttBar"),	true);
	}
	else if (isTags) {
		gp->Name = "TagJumpGrid";
		gp->ColCount = 6;
		IniFile->LoadGridColWidth(gp, 6, 24,200,120,200,100,100);
	}
	else {
		gp->ColCount = 4;
		IniFile->LoadGridColWidth(gp, 4, 24,200,120,200);
		ShowStatusBarAction->Checked = IniFile->ReadBoolGen(_T("EditHistSttBar"),	 true);
	}
	gp->Name = "EditHistGrid";

	StatusBar1->Visible = ShowStatusBarAction->Checked;
	OpeToolBar->Visible = (!isRecent && !isTags);
	if (OpeToolBar->Visible && StatusBar1->Visible) {
		OpeToolBar->Align = alTop;
		OpeToolBar->Align = alBottom;
	}

	HelpContext = isTags? 83 : isMark? 70 : 58;

	EditHistHeader->Style = (isRecent || isMark || isRepo)? hsButtons : hsFlat;
	OpenAction->Checked   = IniFile->ReadBoolGen(_T("MarkListCnfOpen"));

	int opt_mode = IniFile->ReadIntGen(_T("EditHistOptMode"));
	if		(opt_mode==2)	OptMode2Action->Checked = true;
	else if (opt_mode==1)	OptMode1Action->Checked = true;
	else					OptMode0Action->Checked = true;

	UnicodeString tit;
	EditHistHeader->Sections->Clear();
	if (isRecent) {
		tit = "�ŋߎg�����t�@�C���ꗗ";
		for (int i=0; i<gp->ColCount+1; i++) EditHistHeader->Sections->Add();
		InitializeListHeader(EditHistHeader, _T(" |���O|���|�X�V����|�ꏊ"));
		EditHistGrid->PopupMenu = RecentPopupMenu;
	}
	else if (isMark) {
		tit = "�x�}�[�N�ꗗ";
		for (int i=0; i<gp->ColCount+1; i++) EditHistHeader->Sections->Add();
		InitializeListHeader(EditHistHeader, _T(" |���O|���|�X�V����|�ꏊ|����|�ݒ����"));
		EditHistGrid->PopupMenu = MarkPopupMenu;
	}
	else if (isRepo) {
		tit = "���|�W�g���ꗗ";
		for (int i=0; i<gp->ColCount+1; i++) EditHistHeader->Sections->Add();
		InitializeListHeader(EditHistHeader, _T(" |���O|.git/index ����|�ꏊ|�R�~�b�g|���"));
		EditHistGrid->PopupMenu = RepoPopupMenu;
	}
	else if (isTags) {
		tit.sprintf(_T("�_�C���N�g�^�O�W�����v [%s] - %s"), TagName.c_str(), (isView? _T("�{��") : _T("�ҏW")));
		for (int i=0; i<gp->ColCount+1; i++) EditHistHeader->Sections->Add();
		UnicodeString inam;
		if (TagJumpList->Count>0) {
			UnicodeString nptn = get_post_tab(get_post_tab(TagJumpList->Strings[0]));
			isTagPtn = (StartsStr('/', nptn) || StartsStr('?', nptn));
			inam = isTagPtn? "�p�^�[��" : "�s�ԍ�";
		}
		UnicodeString hdr;
		hdr.sprintf(_T(" |���O|�X�V����|�ꏊ|%s|���l"), inam.c_str());
		InitializeListHeader(EditHistHeader, hdr.c_str());
		EditHistGrid->PopupMenu = NULL;
	}
	else {
		tit.sprintf(_T("�ŋ�%s�����t�@�C���ꗗ"), isView? _T("�{��") : _T("�ҏW"));
		for (int i=0; i<gp->ColCount+1; i++) EditHistHeader->Sections->Add();
		InitializeListHeader(EditHistHeader, _T(" |���O|�X�V����|�ꏊ"));
		EditHistGrid->PopupMenu = HistPopupMenu;
	}

	for (int i=0; i<EditHistHeader->Sections->Count; i++)
		EditHistHeader->Sections->Items[i]->Style = hsOwnerDraw;

	set_HeaderFromGrid(gp, EditHistHeader);
	set_UsrScrPanel(GridScrPanel);

	Caption = tit;

	set_MigemoAction(MigemoAction, _T("EditHistMigemo"));

	HistoryList = (isRecent || isMark || isRepo || isTags)? NULL : isView? TextViewHistory : TextEditHistory;
	//���݂��Ȃ��t�@�C���̗������폜
	if (HistoryList) {
		int i = 0;
		while (i<HistoryList->Count)
			if (!file_exists(get_csv_item(HistoryList->Strings[i], 0))) HistoryList->Delete(i); else i++;
	}
	//���݂��Ȃ��x�}�[�N���ڂ��폜
	else if (isMark) {
		IniFile->CheckMarkItems();
	}

	DlgInitialized = true;
	UpdateActions();

	UpdateList();
	gp->Row = isMark? std::min(IniFile->ReadIntGen(_T("MarkListLastIdx")), gp->RowCount - 1) : 0;
	GridScrPanel->UpdateKnob();

	EditFileName = EmptyStr;
	CmdStr		 = EmptyStr;
	TagJumpInf	 = EmptyStr;

	((ToFilter && OpeToolBar->Visible)? (TWinControl*)FilterEdit : (TWinControl*)gp)->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	TStringGrid *gp = EditHistGrid;

	IniFile->SavePosInfo(this,
		isRecent? "RecentListDlg" : isMark? "MarkListDlg" : isRepo? "RepoListDlg" : isTags? "TagJumpDlg" : "");

	if		(isRecent) gp->Name = "RecentListGrid";
	else if (isMark)   gp->Name = "MarkListGrid";
	else if (isRepo)   gp->Name = "RepoListGrid";
	else if (isTags)   gp->Name = "TagJumpGrid";
	IniFile->SaveGridColWidth(gp);
	gp->Name = "EditHistGrid";

	if (isMark) IniFile->WriteIntGen(_T("MarkListLastIdx"),	gp->Row);

	IniFile->WriteBoolGen(_T("EditHistMigemo"),		MigemoAction);
	IniFile->WriteBoolGen(_T("MarkListCnfOpen"),	OpenAction);

	IniFile->WriteIntGen(_T("EditHistOptMode"),
							(OptMode2Action->Checked? 2 : OptMode1Action->Checked? 1 : 0));

	IniFile->WriteIntGen(_T("EditHistFilterWidth"),	FilterEdit->Width);

	if (isRecent) {
		IniFile->WriteIntGen(_T("RecentListSortMode"),	ListSortMode);
		IniFile->WriteIntGen(_T("RecentListSortOdr0"),	ListSortOdr[0]);
		IniFile->WriteIntGen(_T("RecentListSortOdr1"),	ListSortOdr[1]);
		IniFile->WriteIntGen(_T("RecentListSortOdr2"),	ListSortOdr[2]);
		IniFile->WriteIntGen(_T("RecentListSortOdr3"),	ListSortOdr[3]);
		IniFile->WriteBoolGen(_T("RecentListUsedTime"),	ShowUsedTimeAction);
		IniFile->WriteBoolGen(_T("RecentListSttBar"),	ShowStatusBarAction);
	}
	else if (isMark) {
		IniFile->WriteIntGen(_T("MarkListSortMode"),	ListSortMode);
		IniFile->WriteIntGen(_T("MarkListSortOdr0"),	ListSortOdr[0]);
		IniFile->WriteIntGen(_T("MarkListSortOdr1"),	ListSortOdr[1]);
		IniFile->WriteIntGen(_T("MarkListSortOdr2"),	ListSortOdr[2]);
		IniFile->WriteIntGen(_T("MarkListSortOdr3"),	ListSortOdr[3]);
		IniFile->WriteIntGen(_T("MarkListSortOdr4"),	ListSortOdr[4]);
		IniFile->WriteIntGen(_T("MarkListSortOdr5"),	ListSortOdr[5]);
		IniFile->WriteBoolGen(_T("MarkListSttBar"),		ShowStatusBarAction);
	}
	else if (isRepo) {
		IniFile->WriteIntGen(_T("RepoListSortMode"),	ListSortMode);
		IniFile->WriteIntGen(_T("RepoListSortOdr0"),	ListSortOdr[0]);
		IniFile->WriteIntGen(_T("RepoListSortOdr1"),	ListSortOdr[1]);
		IniFile->WriteIntGen(_T("RepoListSortOdr2"),	ListSortOdr[2]);
		IniFile->WriteIntGen(_T("RepoListSortOdr3"),	ListSortOdr[3]);
		IniFile->WriteIntGen(_T("RepoListSortOdr4"),	ListSortOdr[4]);
		IniFile->WriteBoolGen(_T("RepoListSttBar"),		ShowStatusBarAction);
	}
	else if (!isTags) {
		IniFile->WriteBoolGen(_T("EditHistSttBar"),		ShowStatusBarAction);
	}

	clear_FileList(HistBufList);
	TagJumpList->Clear();
	isView = isRecent = isMark = isRepo = isTags = isTagPtn = false;
	ToFilter = false;
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormDestroy(TObject *Sender)
{
	clear_FileList(HistBufList);
	delete HistBufList;

	delete TagJumpList;

	delete GridScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormResize(TObject *Sender)
{
	StatusBar1->Panels->Items[0]->Width = StatusBar1->ClientWidth;
}

//---------------------------------------------------------------------------
//�w�b�_�̕`��
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistHeaderDrawSection(THeaderControl *HeaderControl,
		THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	int mk_mode = 0;
	if ((isRecent || isMark || isRepo) && Section->Index>0) {
		int idx = Section->Index - 1;
		if (idx==ListSortMode) mk_mode = ListSortOdr[idx];
	}

	draw_SortHeader(HeaderControl, Section, Rect, mk_mode);
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(EditHistGrid, EditHistHeader);
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	set_GridFromHeader(EditHistHeader, EditHistGrid);
	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̕`��
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	cv->Brush->Color = col_bgSttBar;
	TRect rc = Rect;
	cv->FillRect(rc);

	UnicodeString lbuf = Panel->Text;
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(rc.Left + 2, rc.Top, split_pre_tab(lbuf));
	if (!lbuf.IsEmpty()) cv->TextOut(rc.Right - cv->TextWidth(lbuf) - ScaledInt(20), rc.Top, lbuf);
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::SetSttBar()
{
	if (!StatusBar1->Visible || !Enabled) return;

	file_rec *cfp = get_CurFileRec();
	if (cfp) {
		if (cfp->inf_list->Count==0) GetFileInfList(cfp);
		UnicodeString stt_str;
		NyanFiForm->SetSttBarInf(cfp, &stt_str);
		StatusBar1->Panels->Items[0]->Text = stt_str;
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ShowStatusBarActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	StatusBar1->Visible = !StatusBar1->Visible;

	if (OpeToolBar->Visible && StatusBar1->Visible) {
		OpeToolBar->Align = alTop;
		OpeToolBar->Align = alBottom;
	}

	ShowStatusBarAction->Checked = StatusBar1->Visible;
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TEditHistoryDlg::get_CurFileName()
{
	TStringGrid *gp = EditHistGrid;
	int idx = gp->Row;
	return (idx>=0 && idx<HistBufList->Count)? HistBufList->Strings[idx] : EmptyStr;
}
//---------------------------------------------------------------------------
file_rec * __fastcall TEditHistoryDlg::get_CurFileRec()
{
	TStringGrid *gp = EditHistGrid;
	int idx = gp->Row;
	return (idx>=0 && idx<HistBufList->Count)? (file_rec *) HistBufList->Objects[idx] : NULL;
}

//---------------------------------------------------------------------------
//�ꗗ���e�̍X�V
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::UpdateList()
{
	if (!DlgInitialized) return;

	cursor_HourGlass();

	UnicodeString cur_fnam = get_CurFileName();

	clear_FileList(HistBufList);

	UnicodeString ptn = OpeToolBar->Visible? usr_Migemo->GetRegExPtn(MigemoAction->Checked, FilterEdit->Text) : EmptyStr;
	TRegExOptions opt; opt << roIgnoreCase;

	//�ŋߎg�����t�@�C��
	if (isRecent) {
		EditHistHeader->Sections->Items[3]->Text = ShowUsedTimeAction->Checked? "�g�p����" : "�X�V����";

		UnicodeString dnam = usr_SH->KnownGuidToPath(FOLDERID_Recent);
		if (!dnam.IsEmpty()) {
			std::unique_ptr<TStringList> lnk_lst(new TStringList());
			get_files(dnam, _T("*.lnk"), lnk_lst.get());
			for (int i=0; i<lnk_lst->Count; i++) {
				UnicodeString fnam = lnk_lst->Strings[i];
				UnicodeString lnam = usr_SH->get_LnkName(fnam);
				if (lnam.IsEmpty()) continue;
				if (StartsStr("::{", lnam)) continue;

				file_rec *fp = cre_new_file_rec();
				fp->f_name = lnam;
				fp->r_name = fnam;
				fp->p_name = ExtractFilePath(lnam);
				fp->n_name = ExtractFileName(lnam);
				fp->b_name = get_base_name(lnam);
				fp->f_ext  = get_extension(lnam);

				if (!NoCheckRecentUnc || !StartsStr("\\\\", fp->p_name)) {
					if (is_InvalidUnc(fp->p_name) || !NyanFiForm->CheckUncPath(fp->p_name)
						|| !file_exists(lnam) || dir_exists(lnam))
					{
						del_file_rec(fp);
						fp = NULL;
					}
					else {
						fp->f_time	 = get_file_age(ShowUsedTimeAction->Checked? fnam : lnam);
						fp->f_attr	 = file_GetAttr(lnam);
						fp->is_sym	 = (fp->f_attr & faSymLink);
						fp->attr_str = get_file_attr_str(fp->f_attr);
						fp->f_size	 = get_file_size(lnam);
					}
				}
				else {		//NoCheckRecentUnc && StartsStr("\\\\", fp->p_name)
					if (fp->f_ext.IsEmpty()) {
						del_file_rec(fp);
						fp = NULL;
					}
				}

				if (fp) HistBufList->AddObject(fp->f_name, (TObject*)fp);
			}
		}

		SortItemList();
	}
	//�x�}�[�N
	else if (isMark) {
		std::unique_ptr<TStringList> del_lst(new TStringList());	//�s�ݍ��ڃ��X�g

		for (int i=0; i<IniFile->MarkIdxList->Count; i++) {
			UnicodeString dnam = IniFile->MarkIdxList->Strings[i];
			if (!is_drive_accessible(dnam)) continue;
			bool is_arc = !ends_PathDlmtr(dnam);	//�A�[�J�C�u
			TStringList *klist = (TStringList*)(IniFile->MarkIdxList->Objects[i]);
			for (int k=0; k<klist->Count; k++) {
				TStringDynArray sbuf = split_strings_tab(klist->Strings[k]);
				if (sbuf.Length==0) continue;
				UnicodeString fnam = sbuf[0];
				UnicodeString memo = (sbuf.Length>1)? sbuf[1] : EmptyStr;	//����
				UnicodeString stim = (sbuf.Length>2)? sbuf[2] : EmptyStr;	//�ݒ����

				//�i�荞��
				if (!ptn.IsEmpty()) {
					UnicodeString s = fnam;
					if (!memo.IsEmpty()) s.cat_sprintf(_T("\t%s"), memo.c_str());
					if (!TRegEx::IsMatch(s, ptn, opt)) continue;
				}

				file_rec *fp = NULL;
				//�A�[�J�C�u
				if (is_arc) {
					arc_find_inf inf;
					if (usr_ARC->GetFileInf(dnam, fnam, &inf)) {
						fp = cre_new_file_rec();
						fp->is_virtual = true;
						fp->arc_name   = dnam;
						fp->f_name.sprintf(_T("%s/%s"), fp->arc_name.c_str(), fnam.c_str());
						fp->is_dir	   = inf.is_dir;
						if (fp->is_dir) {
							fp->b_name = ExtractFileName(fnam);
						}
						else {
							fp->f_ext  = get_extension(fp->f_name);
							fp->b_name = get_base_name(fnam);
						}
						fp->p_name.sprintf(_T("%s/%s"), fp->arc_name.c_str(), ExtractFilePath(fnam).c_str());
						fp->f_time = inf.f_time;
					}
					else {
						del_lst->Add(dnam + "/" + fnam);
					}
				}
				//�ʏ�
				else {
					UnicodeString pnam = dnam + fnam;
					if (dir_exists(pnam)) pnam = IncludeTrailingPathDelimiter(pnam);
					fp = cre_new_file_rec(pnam);
				}

				if (fp) {
					fp->memo = memo.cat_sprintf(_T("\t%s"), stim.c_str());	//���� \t �ݒ����
					HistBufList->AddObject(fp->f_name, (TObject*)fp);
				}
			}
		}

		//�s�ݍ��ڂ��폜
		for (int i=0; i<del_lst->Count; i++) IniFile->FileMark(del_lst->Strings[i], 0);

		SortItemList();
	}
	//���|�W�g���ꗗ
	else if (isRepo) {
		int i = 0;
		while (i<GitInfList->Count) {
			UnicodeString dnam = IncludeTrailingPathDelimiter(GitInfList->Names[i]);
			if ((!NoCheckRecentUnc || !StartsStr("\\\\", dnam))
				&& (is_InvalidUnc(dnam) || !NyanFiForm->CheckUncPath(dnam) || !dir_exists(dnam + ".git")))
			{
				GitInfList->Delete(i);
			}
			else {
				bool ok = true;
				TStringDynArray ibuf = get_csv_array(GitInfList->ValueFromIndex[i], 3);
				if (!ptn.IsEmpty()) {
					UnicodeString s = dnam;
					for (int j=1; j<ibuf.Length; j++) s.cat_sprintf(_T(" %s"), ibuf[j].c_str());
					ok = TRegEx::IsMatch(s, ptn, opt);
				}

				if (ok) {
					file_rec *fp = cre_new_file_rec(dnam);
					UnicodeString cmt_s, stt_s;
					for (int j=0; j<ibuf.Length; j++) {
						if (j==0) {
							TDateTime dt;
							if (str_to_DateTime(ibuf[0], &dt)) fp->f_time = dt;
						}
						else {
							UnicodeString inam = get_tkn(ibuf[j], ": ");
							UnicodeString ival = get_tkn_r(ibuf[j], ": ");
							if		(SameText(inam, "Git-Commit")) cmt_s = ival;
							else if (SameText(inam, "Git-Status")) stt_s = ival;
						}
					}
					fp->memo.sprintf(_T("%s\t%s"), cmt_s.c_str(), stt_s.c_str());
					HistBufList->AddObject(fp->f_name, (TObject*)fp);
				}

				i++;
			}
		}
		SortItemList();
	}
	//�_�C���N�g�^�O�W�����v
	else if (isTags) {
		for (int i=0; i<TagJumpList->Count; i++) {
			UnicodeString lbuf = TagJumpList->Strings[i];
			UnicodeString fnam = get_post_tab(lbuf);
			UnicodeString nptn = get_post_tab(fnam);
			fnam = TagPath + get_pre_tab(fnam);
			file_rec *fp = cre_new_file_rec(fnam);
			if (fp) {
				fp->alias = get_tkn(lbuf, ";\"");
				fp->memo  = Trim(get_tkn_r(lbuf, ";\""));
				HistBufList->AddObject(fp->f_name, (TObject*)fp);
			}
		}
	}
	//����
	else if (HistoryList) {
		for (int i=0; i<HistoryList->Count; i++) {
			UnicodeString fnam = get_csv_item(HistoryList->Strings[i], 0); 
			if (OptMode0Action->Checked ||
			   (OptMode1Action->Checked && StartsText(CurPathName, fnam)) ||
			   (OptMode2Action->Checked && SameText(CurPathName, ExtractFilePath(fnam))))
			{
				if (!ptn.IsEmpty() && !TRegEx::IsMatch(ExtractFileName(fnam), ptn, opt)) continue;
				file_rec *fp = cre_new_file_rec(fnam);
				if (fp) HistBufList->AddObject(fp->f_name, (TObject*)fp);
			}
		}
	}

	UpdateGrid();
	SetSttBar();

	//�J�[�\���ʒu�𕜌�
	TStringGrid *gp = EditHistGrid;
	int idx = std::max(HistBufList->IndexOf(cur_fnam), 0);
	if (idx<gp->RowCount) gp->Row = idx;

	GridScrPanel->UpdateKnob();

	cursor_Default();
}

//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::UpdateGrid()
{
	TStringGrid *gp = EditHistGrid;
	gp->Canvas->Font->Assign(ListFont);

	if (HistBufList->Count>0) {
		//�g���q�̍ő啝���擾
		MaxFextWd = 0;
		if (!isRecent && !isMark) {
			bool is_irreg = IsIrregularFont(ListFont);
			for (int i=0; i<HistBufList->Count; i++) {
				file_rec *fp = (file_rec*)HistBufList->Objects[i];
				MaxFextWd = std::max(get_TextWidth(gp->Canvas, fp->f_ext, is_irreg), MaxFextWd);
			}
		}

		gp->RowCount = HistBufList->Count;

		for (int i=0; i<HistBufList->Count; i++) {
			file_rec *fp = (file_rec*)HistBufList->Objects[i];
			int col = 0;
			gp->Cells[col++][i] = (i<10)? UnicodeString().sprintf(_T("%u"), (i + 1)%10) : EmptyStr;
			//���O
			if (isRecent || isMark) {
				gp->Cells[col++][i] = fp->b_name;
				gp->Cells[col++][i] = fp->f_ext;
			}
			else if (isRepo) {
				gp->Cells[col++][i] = fp->b_name;
			}
			else {
				gp->Cells[col++][i] = fp->b_name + fp->f_ext;
			}
			//�X�V����
			gp->Cells[col++][i] = (isRecent && NoCheckRecentUnc && StartsStr("\\\\", fp->p_name))?
									EmptyStr : FormatDateTime(TimeStampFmt, fp->f_time);
			//�ꏊ
			UnicodeString lbuf = fp->p_name;
			if (isTags) {
				remove_top_text(lbuf, TagPath);
				if (lbuf.IsEmpty()) lbuf = ".\\";
			}
			else {
				if (UncToNetDrive) lbuf = UNC_to_NetDriveName(lbuf);
				if (DispRegName)   lbuf = get_RegDirName(lbuf);
			}
			gp->Cells[col++][i] = lbuf;

			//����,�ݒ����
			if (isMark) {
				gp->Cells[col++][i] = get_pre_tab(fp->memo);
				TDateTime dt;
				if (!str_to_DateTime(get_post_tab(fp->memo), &dt)) dt = fp->f_time;
				gp->Cells[col][i] = FormatDateTime(TimeStampFmt, dt);
			}
			//�R�~�b�g,���
			else if (isRepo) {
				gp->Cells[col++][i] = get_pre_tab(fp->memo);
				gp->Cells[col][i]	= get_post_tab(fp->memo);
			}
			//�s�ԍ�/�p�^�[��, ���l
			else if (isTags) {
				lbuf = get_post_tab(get_post_tab(fp->alias));
				gp->Cells[col++][i] = ReplaceStr(lbuf, "\t", " ");
				gp->Cells[col][i]	= ReplaceStr(fp->memo, "\t", " ");
			}
		}
	}
	else {
		gp->RowCount = 1;
		clear_GridRow(gp, 0);
	}
	gp->Invalidate();
}

//---------------------------------------------------------------------------
//�w��C���f�b�N�X�̗������ڂ��폜/�}�[�N����
//---------------------------------------------------------------------------
bool __fastcall TEditHistoryDlg::del_HistItem()
{
	try {
		if (!isRecent && !isMark && !HistoryList) Abort();

		int idx = EditHistGrid->Row;
		file_rec *fp = get_CurFileRec();  if (!fp) Abort();

		if (isRecent) {
			if (!FileExists(fp->r_name))  Abort();
			if (!delete_File(fp->r_name)) Abort();
		}
		else if (isMark) {
			IniFile->FileMark(fp->f_name, 0);
		}
		else {
			int i = 0;
			while (i<HistoryList->Count)
				if (SameText(fp->f_name, get_csv_item(HistoryList->Strings[i], 0))) HistoryList->Delete(i); else i++;
		}

		del_FileListItem(HistBufList, idx);
		UpdateGrid();
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�Z���̕`��
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	int idx = ARow;
	if (idx<HistBufList->Count) {
		file_rec *fp = (file_rec*)HistBufList->Objects[ARow];

		//�w�i
		cv->Brush->Color = fp->is_virtual? col_bgArc :
				(!isMark && (ACol==1 ||(isRecent && ACol==2)) && IniFile->IsMarked(fp->f_name))? col_bgMark :
				is_AltLnBgCol(ARow)? col_bgList2 : col_bgList;
		cv->FillRect(Rect);

		UnicodeString lbuf = gp->Cells[ACol][ARow];
		TColor col_fg = col_fgList;

		//�ŋߎg�����t�@�C���ꗗ/�x�}�[�N�ꗗ
		if (isRecent || isMark) {
			switch (ACol) {
			case 0: cv->Font->Style = cv->Font->Style << fsUnderline;	break;
			case 2: col_fg = get_ExtColor(fp->f_ext);					break;
			case 3: col_fg = get_TimeColor(fp->f_time, col_fgList);		break;
			case 4: col_fg = col_Folder;								break;
			case 6: {
					TDateTime dt;
					if (str_to_DateTime(lbuf, &dt)) col_fg = get_TimeColor(dt, col_fgList);
				}
				break;
			}
		}
		//���̑��̈ꗗ
		else {
			switch (ACol) {
			case 0: cv->Font->Style = cv->Font->Style << fsUnderline;	break;
			case 2: col_fg = get_TimeColor(fp->f_time, col_fgList);		break;
			case 3: col_fg = col_Folder;								break;
			case 5: if (isRepo && SameText(lbuf, "Clean")) col_fg = AdjustColor(col_fg, ADJCOL_FGLIST);	break;
			}
		}
		cv->Font->Color = col_fg;

		int xp = Rect.Left + Scaled4;
		int yp = Rect.Top + get_TopMargin2(cv);

		TColor col_x = get_ExtColor(fp->f_ext);
		TColor col_f = (fp->is_sym && SymColorToName)? col_SymLink : get_FileColor(fp, col_x);

		//�t�@�C�����̋��ʏ���
		if (ACol==1) {
			//�A�C�R��
			if (IconMode==1 || (isRepo && IconMode>0)) {
				draw_SmallIcon(fp, cv, xp, std::max(yp + (cv->TextHeight("Q") - SIcoSize)/2, 0));
				xp += get_IcoWidth();
			}
		}

		//�ŋߎg�����t�@�C���ꗗ/�x�}�[�N�ꗗ
		if (isRecent || isMark) {
			//�t�@�C����
			if (ACol==1) {
				cv->Font->Color = col_f;
				int mgn = Scaled4;
				//�f�B���N�g��
				UnicodeString bnam;
				if (fp->is_dir) {
					bnam = (fp->is_dir && IconMode==0)? (DirBraStr + fp->b_name + DirKetStr) : fp->b_name;
					bnam = minimize_str(bnam, cv, Rect.Right - xp - mgn, OmitEndOfName);
					cv->TextOut(xp, yp, bnam);
				}
				//�t�@�C��
				else {
					bnam = minimize_str(fp->b_name, cv, Rect.Right - xp - mgn, OmitEndOfName);
					cv->TextOut(xp, yp, bnam);
				}
			}
			//�g���q
			else if (ACol==2) {
				cv->Font->Color = col_x;
				if (!fp->is_dir) cv->TextOut(xp, yp, lbuf);
			}
			//�ꏊ
			else if (ACol==4) {
				PathNameOut(lbuf, cv, xp, yp, gp->ColWidths[ACol] - Scaled8);
			}
			else {
				cv->TextRect(Rect, xp, yp, lbuf);
			}
		}
		//���|�W�g���ꗗ
		else if	(isRepo) {
			if (ACol==1) {
				cv->Font->Color = col_f;
				UnicodeString bnam = minimize_str(fp->b_name, cv, Rect.Right - xp - Scaled4, OmitEndOfName);
				cv->TextOut(xp, yp, bnam);
			}
			//�ꏊ
			else if (ACol==3) {
				PathNameOut(lbuf, cv, xp, yp, gp->ColWidths[ACol] - Scaled8);
			}
			//�R�~�b�g
			else if (ACol==4) {
				UnicodeString s = get_in_paren(lbuf);
				if (!s.IsEmpty()) {
					lbuf = Trim(get_tkn_r(lbuf, ")"));
					TStringDynArray b_buf = SplitString(s, ",");
					for (int i=0; i<b_buf.Length; i++) {
						UnicodeString ss = Trim(b_buf[i]);
						if (remove_top_text(ss, "tag: ")) {
							draw_GitTag(cv, xp, yp, ss, Scaled4);
						}
						else {
							if (remove_top_text(ss, HEAD_Mark)) out_TextEx(cv, xp, yp, HEAD_Mark, col_GitHEAD);
							out_TextEx(cv, xp, yp, ss, col_bgList, col_GitBra, Scaled8);
						}
					}
				}
				TRect rc = Rect; rc.Left = xp;
				cv->TextRect(rc, xp, yp, minimize_str(lbuf, cv, rc.Width() - Scaled4, true));
			}
			else {
				cv->TextRect(Rect, xp, yp, lbuf);
			}
		}
		//���̑��̈ꗗ
		else {
			//�t�@�C����
			if (ACol==1) {
				int mgn = get_CharWidth(cv, 1);
				//�f�B���N�g�� or �g���q�񕪗�
				cv->Font->Color = col_f;
				UnicodeString bnam;
				if (NoSpaceFExt || fp->is_dir) {
					bnam = (fp->is_dir && IconMode==0)? (DirBraStr + fp->b_name + DirKetStr) : fp->b_name;
					bnam = minimize_str(bnam, cv, Rect.Right - xp - get_TextWidth(cv, fp->f_ext, is_irreg) - mgn, OmitEndOfName);
					cv->TextOut(xp, yp, bnam);
					xp += get_TextWidth(cv, bnam, is_irreg);
				}
				//�g���q����
				else {
					int x_fx = Rect.Right - MaxFextWd - mgn;
					bnam = minimize_str(fp->b_name, cv, x_fx - xp, OmitEndOfName);
					cv->TextOut(xp, yp, bnam);
					xp = x_fx;
				}

				//�g���q
				if (!fp->is_dir) {
					if (PriorFExtCol) cv->Font->Color = col_x;
					cv->TextOut(xp, yp, fp->f_ext);
				}
			}
			//�ꏊ
			else if (ACol==3) {
				PathNameOut(lbuf, cv, xp, yp, gp->ColWidths[ACol] - Scaled8);
			}
			else {
				if (isTagPtn && ACol==4) lbuf = minimize_str(lbuf, cv, Rect.Width() - 8, true);
				cv->TextRect(Rect, xp, yp, lbuf);
			}
		}
	}
	else {
		cv->Brush->Color = col_bgList;
		cv->FillRect(Rect);
	}

	//�J�[�\��
	draw_GridCursor(gp, Rect, ARow, State);
}

//---------------------------------------------------------------------------
bool __fastcall TEditHistoryDlg::set_FileName(int idx)
{
	if (idx>=0 && idx<HistBufList->Count) {
		file_rec *fp = (file_rec*)HistBufList->Objects[idx];
		EditFileName = fp->f_name;
		if (isTags) TagJumpInf = fp->alias;
	}
	else {
		EditFileName = EmptyStr;
	}

	return !EditFileName.IsEmpty();
}

//---------------------------------------------------------------------------
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TStringGrid *gp = (TStringGrid*)Sender;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	CmdStr = Key_to_CmdF(KeyStr);

	try {
		//�����L�[�ɂ��A�N�Z�X
		if (is_Num0to9(KeyStr)) {
			if (!set_FileName((KeyStr.ToIntDef(0) + 9)%10)) Abort(); 
			if (isRecent || isRepo) {
				CmdStr = EmptyStr;
				ModalResult = mrClose;	//�ړ�
			}
			else {
				if (isMark)
					CmdStr.sprintf(_T("%s"), OpenAction->Checked? _T("OpenStandard") : null_TCHAR);
				else
					CmdStr.sprintf(_T("%s"), isView? _T("TextViewer") : _T("FileEdit"));
				ModalResult = mrOk;
			}
		}
		else if (equal_ENTER(KeyStr)) {
			if (!set_FileName(gp->Row)) Abort();
			CmdStr.sprintf(_T("%s"), (isMark && OpenAction->Checked)? _T("OpenStandard") : null_TCHAR);
			ModalResult = mrClose;	//�ړ�
		}
		//�t�B���^��
		else if (StartsText("IncSearch", CmdStr) && OpeToolBar->Visible) {
			FilterEdit->SetFocus();
		}
		//����
		else if (USAME_TI(CmdStr, "ReturnList")) {
			ModalResult = mrCancel;
		}
		//�J�[�\���ړ�
		else if (GridCursorMove(CmdStr, gp)) {
			;
		}
		//���ڍ폜/�}�[�N����
		else if (equal_DEL(KeyStr) || USAME_TI(CmdStr, "Delete")) {
			if (!del_HistItem()) Abort();
		}
		//��������
		else if (isMark && USAME_TI(CmdStr, "Mark_IM")) {
			MemoAction->Execute();
		}
		//�R�}���h
		else if (isRepo && contained_wd_i(_T("GitViewer|TextViewer"), CmdStr)) {
			if (!set_FileName(gp->Row)) Abort();
			CmdStr = "GitViewer";
			ModalResult = mrOk;
		}
		else if (!isTags && !isRepo
			&& contained_wd_i(_T("FileEdit|TextViewer|ImageViewer|OpenByApp|OpenByWin|OpenByExp"), CmdStr))
		{
			if (!set_FileName(gp->Row)) Abort();
			ModalResult = mrOk;
		}
		//�t�@�C�����
		else if (!isTags && StartsText("ShowFileInfo", CmdStr)) {
			ShowFileInfoAction->Execute();
		}
		//�v���p�e�B
		else if (!isTags && USAME_TI(CmdStr, "PropertyDlg")) {
			ShowPropertyAction->Execute();
		}
		//�X�e�[�^�X�o�[
		else if (!isTags && USAME_TI(CmdStr, "ShowStatusBar")) {
			ShowStatusBarAction->Execute();
		}
		//�E�N���b�N���j���[
		else if (StartsText("ContextMenu", CmdStr)) {
			show_PopupMenu(gp);
		}
		//�������T�[�`
		else if (is_IniSeaKey(KeyStr)) {	//KeyStr �ɐ��K�\���p�^�[�����Ԃ�
			TRegExOptions opt; opt << roIgnoreCase;
			int idx0 = -1, idx1 = -1;
			for (int i=0; i<HistBufList->Count && idx1==-1; i++) {
				if (i<=gp->Row && idx0!=-1) continue;
				UnicodeString fnam = HistBufList->Strings[i];
				if (contains_Slash(fnam)) fnam = get_tkn_r(fnam, '/');
				fnam = ExtractFileName(fnam);
				if (TRegEx::IsMatch(fnam, KeyStr, opt)) ((i<=gp->Row)? idx0 : idx1) = i;
			}
			int f_idx = (idx1!=-1)? idx1 : idx0;
			if (f_idx!=-1) gp->Row = f_idx;
		}
		else {
			CmdStr = EmptyStr;
		}

		SetSttBar();
	}
	catch (EAbort &e) {
		CmdStr = EmptyStr;
		beep_Warn();
	}

	if (!is_DialogKey(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistGridClick(TObject *Sender)
{
	SetSttBar();
}

//---------------------------------------------------------------------------
//�}�E�X����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistGridDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//�\�[�g
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::SortItemList()
{
	if (isRecent || isMark) {
		switch (ListSortMode) {
		case 0: case 1: case 4:
			DscNameOrder = (ListSortOdr[ListSortMode] == -1);	break;
		case 3:
			DscPathOrder = (ListSortOdr[ListSortMode] == -1);	break;
		case 2: case 5:
			OldOrder	 = (ListSortOdr[ListSortMode] == 1);	break;
		}

		switch (ListSortMode) {
		case 0: HistBufList->CustomSort(SortComp_Name);		break;
		case 1: HistBufList->CustomSort(SortComp_Ext);		break;
		case 2: HistBufList->CustomSort(SortComp_Time);		break;
		case 3: HistBufList->CustomSort(SortComp_PathName);	break;
		case 4: HistBufList->CustomSort(SortComp_Memo);		break;
		case 5: HistBufList->CustomSort(SortComp_MarkTime);	break;
		}
	}
	else if (isRepo) {
		switch (ListSortMode) {
		case 0: case 3: case 4:
			DscNameOrder = (ListSortOdr[ListSortMode] == -1);	break;
		case 2:
			DscPathOrder = (ListSortOdr[ListSortMode] == -1);	break;
		case 1:
			OldOrder	 = (ListSortOdr[ListSortMode] == 1);	break;
		}

		switch (ListSortMode) {
		case 0: HistBufList->CustomSort(SortComp_Name);		break;
		case 1: HistBufList->CustomSort(SortComp_Time);		break;
		case 2: HistBufList->CustomSort(SortComp_PathName);	break;
		case 3: HistBufList->CustomSort(SortComp_Memo);		break;
		case 4: HistBufList->CustomSort(SortComp_GitStt);	break;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::EditHistHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	if ((!isRecent && !isMark && !isRepo) || Section->Index<1) return;

	int idx = Section->Index - 1;

	if (idx==ListSortMode)
		ListSortOdr[idx] = -ListSortOdr[idx];
	else
		ListSortMode = idx;

	EditHistHeader->Invalidate();

	UnicodeString fnam = get_CurFileName();
	SortItemList();
	UpdateGrid();

	TStringGrid *gp = EditHistGrid;
	idx = std::max(HistBufList->IndexOf(fnam), 0);
	if (idx<gp->RowCount) gp->Row = idx;
	gp->Invalidate();
	gp->SetFocus();
}

//---------------------------------------------------------------------------
//�����̍폜
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClrMemoItemClick(TObject *Sender)
{
	int idx = EditHistGrid->Row;
	if (idx<0 || idx>=HistBufList->Count) return;

	file_rec *cfp = (file_rec*)HistBufList->Objects[idx];
	UnicodeString fnam = HistBufList->Strings[idx];
	IniFile->FileMark(fnam, 1, EmptyStr);
	cfp->memo = IniFile->GetMarkMemo(fnam);
	UpdateList();
}
//---------------------------------------------------------------------------
//�x�}�[�N����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::DelMarkItemClick(TObject *Sender)
{
	if (!del_HistItem()) beep_Warn();
}
//---------------------------------------------------------------------------
//���ׂĂ̞x�}�[�N������
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClrAllMarkItemClick(TObject *Sender)
{
	if (msgbox_Sure(USTR_ClrAllMarkQ)) {
		IniFile->ClearAllMark();
		UpdateList();
	}
}

//---------------------------------------------------------------------------
//�t�B���^�ɂ��ꗗ�̍X�V
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterEditChange(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterBtnClick(TObject *Sender)
{
	FilterEdit->SetFocus();
}

//---------------------------------------------------------------------------
//�t�B���^���ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;

	if		(contained_wd_i(KeysStr_ToList, KeyStr))	EditHistGrid->SetFocus();
	else if (MovGridFromFilter(EditHistGrid, KeyStr))	;
	else if (SameText(KeyStr, KeyStr_Migemo))			MigemoAction->Checked = !MigemoAction->Checked;
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key)) {
		Key = 0;
	}
	else if (Key==VK_RETURN) {
		if (set_FileName(EditHistGrid->Row)) {
			CmdStr.sprintf(_T("%s"), (isMark && OpenAction->Checked)? _T("OpenStandard") : null_TCHAR);
			ModalResult = mrClose;	//�ړ�
		}
		Key = 0;
	}
}

//---------------------------------------------------------------------------
//���������ׂč폜
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClearAllItemClick(TObject *Sender)
{
	if (HistoryList && msgbox_Sure(USTR_DelHistoryQ)) {
		HistoryList->Clear();
		UpdateList();
	}
}
//---------------------------------------------------------------------------
//�����ɓ���Ȃ��p�X�̐ݒ�
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::NoHistItemClick(TObject *Sender)
{
	UnicodeString *p = isView? &NoViewHistPath : &NoEditHistPath;
	if (input_query_ex(_T("�����ɓ���Ȃ��p�X�̐ݒ� (������v)"), null_TCHAR, p, 0, false, LoadUsrMsg(USTR_HintMltSepSC))) {
		if (!(*p).IsEmpty()) {
			TStringDynArray path_lst = split_strings_semicolon(*p);
			int idx = 0;
			while (idx<HistoryList->Count) {
				UnicodeString pnam = ExtractFilePath(get_csv_item(HistoryList->Strings[idx], 0));
				for (int i=0; i<path_lst.Length && !pnam.IsEmpty(); i++) {
					if (path_lst[i].IsEmpty()) continue;
					if (ContainsText(pnam, cv_env_str(path_lst[i]))) pnam = EmptyStr;
				}

				if (pnam.IsEmpty()) HistoryList->Delete(idx); else idx++;
			}

			UpdateList();
		}
	}
}
//---------------------------------------------------------------------------
//�ŋߎg�������ׂĂ̍��ڂ��폜
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClrAllRecentActionExecute(TObject *Sender)
{
	if (msgbox_Sure(LoadUsrMsg(USTR_DeleteQ, _T("���ׂĂ̍���")))) {
		SHAddToRecentDocs(SHARD_PATHA, NULL);
		UpdateList();
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClrAllRecentActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (HistBufList->Count>0);
}
//---------------------------------------------------------------------------
//�����N�؂ꍀ�ڂ𐮗�
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ClrBrkRecentActionExecute(TObject *Sender)
{
	UnicodeString dnam = usr_SH->KnownGuidToPath(FOLDERID_Recent);
	if (!dnam.IsEmpty()) {
		std::unique_ptr<TStringList> lnk_lst(new TStringList());
		std::unique_ptr<TStringList> brk_lst(new TStringList());
		get_files(dnam, _T("*.lnk"), lnk_lst.get());
		for (int i=0; i<lnk_lst->Count; i++) {
			UnicodeString fnam = lnk_lst->Strings[i];
			UnicodeString lnam = usr_SH->get_LnkName(fnam);
			if (lnam.IsEmpty()) continue;
			if (StartsStr("::{", lnam)) continue;

			if (!NoCheckRecentUnc || !StartsStr("\\\\", lnam)) {
				if (is_InvalidUnc(ExtractFilePath(lnam)) || !file_exists(lnam)) brk_lst->Add(fnam);
			}
		}

		if (brk_lst->Count>0) {
			UnicodeString msg;
			if (msgbox_Sure(msg.sprintf(_T("%u�̃����N�؂ꍀ�ڂ��폜���܂���?"), brk_lst->Count),
				!(NyanFiForm->ExeCmdsBusy && XCMD_MsgOff)))
			{
				cursor_HourGlass();
				for (int i=0; i<brk_lst->Count; i++) {
					UnicodeString fnam = brk_lst->Strings[i];
					UnicodeString msg  = make_LogHdr(_T("DELETE"), fnam);
					if (!delete_File(brk_lst->Strings[i])) set_LogErrMsg(msg);
					AddLog(msg);
				}
				Sleep(250);
				cursor_Default();
			}
		}
		else if (!(NyanFiForm->ExeCmdsBusy && XCMD_MsgOff)) {
			msgbox_OK("�����N�؂�͂���܂���B");
		}
	}
}
//---------------------------------------------------------------------------
//�g�p������\��
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ShowUsedTimeActionExecute(TObject *Sender)
{
	ShowUsedTimeAction->Checked = !ShowUsedTimeAction->Checked;
	UpdateList();
}

//---------------------------------------------------------------------------
//�t�@�C�����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ShowFileInfoActionExecute(TObject *Sender)
{
	FileInfoDlg->ShowModalEx(get_CurFileName());
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ShowFileInfoActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !get_CurFileName().IsEmpty();
}
//---------------------------------------------------------------------------
//�v���p�e�B
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ShowPropertyActionExecute(TObject *Sender)
{
	TStringGrid *gp = EditHistGrid;
	TRect rc = gp->CellRect(1, gp->Row);
	Mouse->CursorPos = gp->ClientToScreen(Point(rc.Left + rc.Width()/2, rc.Top + rc.Height()));

	ShowPropertyDialog(get_CurFileName());
}

//---------------------------------------------------------------------------
//����
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::MemoActionExecute(TObject *Sender)
{
	int idx = EditHistGrid->Row;
	if (idx<0 || idx>=HistBufList->Count) return;

	file_rec *cfp = (file_rec*)HistBufList->Objects[idx];
	UnicodeString mstr = get_pre_tab(cfp->memo);
	UnicodeString fnam = HistBufList->Strings[idx];
	if (input_query_ex(ExtractFileName(fnam).c_str(), _T("����"), &mstr, 480)) {
		IniFile->FileMark(fnam, 1, mstr);
		cfp->memo = IniFile->GetMarkMemo(fnam);
		UpdateList();
	}
	CloseIME(Handle);
}

//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::OptModeActionExecute(TObject *Sender)
{
	((TAction *)Sender)->Checked = true;

	UpdateList();

	TStringGrid *gp = EditHistGrid;
	gp->Row = 0;
	gp->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::ToggleActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	if (ap==OpenAction) EditHistGrid->SetFocus();
}

//---------------------------------------------------------------------------
//�ŐV�̏��ɍX�V
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::UpdateGitInfActionExecute(TObject *Sender)
{
	StatusBar1->Panels->Items[0]->Text = "�ŐV�̏��ɍX�V��...";
	TStringGrid *gp = EditHistGrid;
	gp->Row = 0;
	Enabled = false;
	for (int i=0; i<HistBufList->Count; i++) {
		gp->Row = i;
		get_GitInf(HistBufList->Strings[i], NULL, true);
		gp->Invalidate();
	}
	Enabled = true;
	gp->Row = 0;
}
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::UpdateGitInfActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (HistBufList->Count>0);
}
//---------------------------------------------------------------------------
//Git�r���A�[���J��
//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::GitViewerActionExecute(TObject *Sender)
{
	if (set_FileName(EditHistGrid->Row)) {
		CmdStr = "GitViewer";
		ModalResult = mrClose;
	}
}

//---------------------------------------------------------------------------
void __fastcall TEditHistoryDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

