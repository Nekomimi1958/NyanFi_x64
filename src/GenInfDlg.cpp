//----------------------------------------------------------------------//
// NyanFi																//
//  �ėp�ꗗ�_�C�A���O													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <System.DateUtils.hpp>
#include <RegularExpressions.hpp>
#include <Vcl.Clipbrd.hpp>
#include "usr_id3.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "FindTxtDlg.h"
#include "FileInfDlg.h"
#include "GenInfDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGeneralInfoDlg *GeneralInfoDlg;

//---------------------------------------------------------------------------
__fastcall TGeneralInfoDlg::TGeneralInfoDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, GenListBox, USCRPNL_FLAG_GL|USCRPNL_FLAG_P_WP|USCRPNL_FLAG_L_WP|USCRPNL_FLAG_HS);

	org_GenListWndProc	   = GenListBox->WindowProc;
	GenListBox->WindowProc = GenListWndProc;

	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	GenInfoList = new TStringList();
	GenInfoBuff = new TStringList();
	GenSelList	= new TStringList();

	isVarList	 = isLog = isGit = isDirs = isTree = isCmdHistory = isFileList = isPlayList = false;
	isTail		 = isReverse = isFTP = false;
	fromGitView	 = false;
	ToFilter	 = ToEnd = false;
	ErrOnly 	 = false;
	isFiltered	 = false;

	CodePage	 = 0;
	HasBOM		 = false;
	MaxNameWidth = 0;
	LastIndex	 = 0;
	LastTopIndex = 0;
	TailLine	 = 100;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormShow(TObject *Sender)
{
	setup_ToolBar(OpeToolBar, true);

	FilterEdit->Width = IniFile->ReadIntGen(_T("GenInfoFilterWidth"),	200);

	RetStr = EmptyStr;
	GenSelList->Clear();

	CancelKeySeq();

	FindWord  = RegExPtn = EmptyStr;
	Found	  = false;
	SortMode  = 0;
	ListWidth = 0;

	//���o���p�^�[���̐ݒ�
	if (UserHighlight->Recycle()) AddErr_Highlight();
	if (HdrLnStr.IsEmpty()) {
		HdrLnStr = UserHighlight->GetHeadlinePtn(FileName, HeadlineList);
		if (!HdrLnStr.IsEmpty()) HdrLnStr = "/" + HdrLnStr + "/";
	}

	if (isCmdHistory)
		GenInfoList->Assign(CommandHistory);
	else if (isPlayList)
		GenInfoList->Assign(PlayList);

	if (Caption.IsEmpty()) {
		UnicodeString cap_str = def_if_empty(yen_to_delimiter(FileName), LoadUsrMsg(USTR_List));
		if (isTail) {
			cap_str.cat_sprintf(_T(" (���� %u�s"), GenInfoList->Count);
			if (isReverse) cap_str += ":�t��";
			cap_str += ")";
		}
		else if (isTree) {
			cap_str += " (�c���[�\��)";
		}
		Caption = cap_str;
	}

	isDirs = USAME_TI(Caption, "�f�B���N�g�����ꗗ");

	IniFile->LoadPosInfo(this, DialogCenter);

	ShowLineNoAction->Checked  = IniFile->ReadBoolGen(_T("GenInfoShowLnNo"));
	KeepIndexAction->Checked   = IniFile->ReadBoolGen(_T("GenInfoKeepIndex"));
	FileName1stAction->Checked = IniFile->ReadBoolGen(_T("GenInfoFileName1st"));
	OmitComPathAction->Checked = IniFile->ReadBoolGen(_T("GenInfoOmitComPath"));

	set_MigemoAction(MigemoAction, _T("GenInfoMigemo"));

	AndOrAction->Checked	 = IniFile->ReadBoolGen(_T("GenInfoFltAndOr"));
	HighlightAction->Checked = IniFile->ReadBoolGen(_T("GenInfoHighlight"));

	if (isTail) {
		WatchAction->Checked  = IniFile->ReadBoolGen(_T("GenInfoTailWatch"),	false);
		NotifyAction->Checked = IniFile->ReadBoolGen(_T("GenInfoTailNotify"),	false);
	}

	//���O�t�@�C����?
	if (!FileName.IsEmpty()) isLog = str_match(ExePath + "tasklog*.txt", cv_VirToOrgName(FileName));

	//Git?
	isGit = (GenInfoList->Count>0 && StartsStr("$ git ", GenInfoList->Strings[0]));

	//�u���O=�l�v�`����?
	if (!isVarList && !isLog) {
		bool all_n_v = (GenInfoList->Count>0);
		for (int i=0; i<GenInfoList->Count && all_n_v; i++) {
			if (i==0 && isGit) continue;
			if (!GenInfoList->Strings[i].IsEmpty()) {
				UnicodeString nbuf = GenInfoList->Names[i];
				all_n_v = !nbuf.IsEmpty() && !ContainsStr(nbuf, " ") && !ContainsStr(nbuf, "\t");
			}
		}
		isVarList = all_n_v;
	}

	//���X�g�{�b�N�X�̏�����(���z)
	TListBox *lp = GenListBox;
	lp->Tag = LBTAG_GEN_LIST | LBTAG_OPT_ZOOM
				| (ShowLineNoAction->Checked? LBTAG_OPT_LNNO : 0)
				| ((isFileList || isTree)? LBTAG_TAB_FNAM : 0)	//�^�u�ȍ~�Ɏ��t�@�C����������
				| (isVarList? LBTAG_VAR_LIST : 0)
				| (isTree? LBTAG_OPT_TREE : 0);

	set_ListBoxItemHi(lp, GenListFont);
	lp->Color = isLog? col_bgLog : col_bgList;
	ListPanel->Color = lp->Color;
	lp->TabWidth = get_ViewTabWidth(get_extension(FileName));	//�^�u��(�`��̂��߂ɗ��p)
	set_UsrScrPanel(ListScrPanel);

	//�t�@�C����/�ϐ����̍ő啝���擾
	MaxNameWidth = 0;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	if (isPlayList && FileName1stAction->Checked) {
		for (int i=0; i<GenInfoList->Count; i++) {
			MaxNameWidth = std::max(get_TextWidth(cv, ExtractFileName(GenInfoList->Strings[i]), is_irreg), MaxNameWidth);
		}
		MaxNameWidth += get_TextWidth(cv, " ", is_irreg);
	}
	else if (isVarList) {
		for (int i=0; i<GenInfoList->Count; i++) {
			MaxNameWidth = std::max(get_TextWidth(cv, GenInfoList->Names[i], is_irreg), MaxNameWidth);
		}
		MaxNameWidth += get_TextWidth(cv, " = ", is_irreg);
	}

	//�p�X�̋��ʕ������擾
	ComPathName = EmptyStr;
	if (isPlayList && GenInfoList->Count>1) {
		ComPathName = ExtractFilePath(GenInfoList->Strings[0]);
		for (int i=1; i<GenInfoList->Count; i++) {
			UnicodeString pnam = ExtractFilePath(GenInfoList->Strings[i]);
			if (!SameText(pnam, ComPathName)) {
				TStringDynArray plst0 = split_path(ComPathName);
				TStringDynArray plst1 = split_path(pnam);
				ComPathName = EmptyStr;
				for (int j=0; (j<plst0.Length && j<plst1.Length); j++) {
					if (!SameText(plst0[j], plst1[j])) break;
					ComPathName.cat_sprintf(_T("%s\\"), plst0[j].c_str());
				}
			}
		}
	}

	FileSize = -1;
	UpdateList();

	//�J�[�\���ʒu��ݒ�
	int idx = -1;
	int top = -1;
	if (lp->Count>0) {
		if (isLog || isCmdHistory || ToEnd) {
			idx = lp->Count - 1;
		}
		else if (isPlayList) {
			idx = GenInfoBuff->IndexOf(PlayFile);
		}
		else if (KeepIndexAction->Checked) {
			idx = LastIndex;
			top = LastTopIndex;
		}

		if		(idx==-1)		 idx = 0;
		else if (idx>=lp->Count) idx = lp->Count - 1;
	}

	if (top!=-1) lp->TopIndex = top;
	lp->ItemIndex = idx;
	ListScrPanel->UpdateKnob();

	(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)lp)->SetFocus();

	//�X�e�[�^�X�o�[
	setup_StatusBar(StatusBar1);
	set_SttBarPanelWidth(StatusBar1, 0, 21 + IntToStr(GenInfoList->Count).Length() * 3);
												//"����: nnn  -  nnn    �I��: nnn"
	set_SttBarPanelWidth(StatusBar1, 1, "UTF-16(BE) BOM�t");
	set_SttBarPanelWidth(StatusBar1, 2, "CR/LF");
	SetStatusBar();

	SetDarkWinTheme(this);
	SetDarkWinTheme(FilterEdit);
	FilterEdit->Color = get_WinColor(!ToFilter);

	if (!ErrMsg.IsEmpty()) {
		msgbox_ERR(ErrMsg);
	}
	else {
		Timer1->Interval = 1000;	//***
		Timer1->Enabled  = isTail && WatchAction->Checked;
	}

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen( _T("GenInfoFilterWidth"),	FilterEdit->Width);
	IniFile->WriteBoolGen(_T("GenInfoShowLnNo"),	ShowLineNoAction);
	IniFile->WriteBoolGen(_T("GenInfoKeepIndex"),	KeepIndexAction);
	IniFile->WriteBoolGen(_T("GenInfoFileName1st"),	FileName1stAction);
	IniFile->WriteBoolGen(_T("GenInfoOmitComPath"),	OmitComPathAction);
	IniFile->WriteBoolGen(_T("GenInfoMigemo"),		MigemoAction);
	IniFile->WriteBoolGen(_T("GenInfoFltAndOr"),	AndOrAction);
	IniFile->WriteBoolGen(_T("GenInfoHighlight"),	HighlightAction);
	if (isTail) {
		IniFile->WriteBoolGen(_T("GenInfoTailWatch"),	WatchAction);
		IniFile->WriteBoolGen(_T("GenInfoTailNotify"),	NotifyAction);
	}

	LastIndex	 = (ModalResult==mrRetry)? GenListBox->ItemIndex : 0;
	LastTopIndex = (ModalResult==mrRetry)? GenListBox->TopIndex : 0;

	GenInfoList->Clear();
	GenInfoBuff->Clear();

	isVarList	 = isLog = isGit = isDirs = isTree = isCmdHistory = isFileList = isPlayList = false;
	isTail		 = isReverse = isFTP = false;
	fromGitView	 = false;
	ToFilter	 = ToEnd = false;
	ErrOnly 	 = false;
	isFiltered	 = false;

	FileName	 = EmptyStr;
	Caption 	 = EmptyStr;
	ErrMsg		 = EmptyStr;

	CodePage	 = 0;
	HasBOM		 = false;
	LineBreakStr = EmptyStr;
	HdrLnStr	 = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormDestroy(TObject *Sender)
{
	delete GenInfoList;
	delete GenInfoBuff;
	delete GenSelList;
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListWndProc(TMessage &msg)
{
	if (msg.Msg==WM_VSCROLL || msg.Msg==WM_NYANFI_USCROLL) {
		if (TvCursorVisible && ListBoxCsrToVisible(GenListBox)) {
			GenListBox->Invalidate();
			SetStatusBar();
		}
	}

	org_GenListWndProc(msg);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::WmFormShowed(TMessage &msg)
{
	GenListBox->Invalidate();
	Application->ProcessMessages();
}

//---------------------------------------------------------------------------
//�v���C���X�g�̍X�V
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::WmNyanFiPlayList(TMessage &msg)
{
	if (isPlayList) {
		if (msg.WParam==1) {
			GenInfoList->Assign(PlayList);
			UpdateList();
		}
		else {
			GenListBox->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
//ListTail �̊Ď�����
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::Timer1Timer(TObject *Sender)
{
	if (!FileName.IsEmpty()) {
		__int64   fsz = get_file_size(FileName);
		TDateTime fdt = get_file_age(FileName);
		if (FileSize!=fsz || FileTime!=fdt) {
			if (FileSize>0) {
				if (UpdateList(true) && NotifyAction->Checked) {
					play_sound(SoundWatch);
					NotifyPrimNyan("�Ď����̃t�@�C�����ω����܂���");
				}
			}
			FileSize = fsz;
			FileTime = fdt;
		}
	}
}
//---------------------------------------------------------------------------
//�ꗗ�̍X�V
// GenInfoBuff->Objects ���s�C���f�b�N�X�t���ɗ��p
//---------------------------------------------------------------------------
bool __fastcall TGeneralInfoDlg::UpdateList(bool reload)
{
	UnicodeString last_text = GenInfoBuff->Text;
	Found	  = false;
	SortMode  = 0;
	ListWidth = 0;

	ListScrPanel->HitLines->Clear();

	TListBox *lp = GenListBox;
	int idx = lp->ItemIndex;
	if (idx>=0 && idx<GenInfoBuff->Count && isFiltered) idx = (int)GenInfoBuff->Objects[idx];

	if (reload) {
		std::unique_ptr<TStringList> f_buf(new TStringList());
		if (load_text_tail(FileName, f_buf.get(), CodePage, TailLine, isReverse)!=0) {
			GenInfoList->Assign(f_buf.get());
		}
		else msgbox_ERR(USTR_FileNotOpen);
	}

	cursor_HourGlass();
	GenInfoBuff->Clear();
	//���O�̃G���[�ӏ��̂ݍi�荞��
	if (isLog && ErrOnly) {
		bool er_flag = false;
		for (int i=0; i<GenInfoList->Count; i++) {
			UnicodeString lbuf = GenInfoList->Strings[i];
			if (lbuf.Length()<4) continue;
			if (lbuf[2]=='>' && lbuf[3]=='E' && lbuf[4]==' ') {
				er_flag = true;
				GenInfoBuff->AddObject(lbuf, (TObject*)(NativeInt)i);
			}
			else if (er_flag) {
				if (StartsStr("    ", lbuf)) {
					GenInfoBuff->AddObject(lbuf, (TObject*)(NativeInt)i);
				}
				er_flag = false;
			}
		}
		idx = 0;
		isFiltered = true;
	}
	//�t�B���^
	else if (!FilterEdit->Text.IsEmpty()
		&& (!MigemoAction->Checked || (MigemoAction->Checked && FilterEdit->Text.Length()>=IncSeaMigemoMin)))
	{
		UnicodeString pnam = (isTree && GenInfoList->Count>0)? get_post_tab(GenInfoList->Strings[0]) : EmptyStr;

		filter_List(GenInfoList, GenInfoBuff, FilterEdit->Text,
					MigemoAction->Checked, AndOrAction->Checked, false, isFileList, isTree);

		if (isTree) {
			MakeTreeList(GenInfoBuff, pnam, true);
			idx = -1;
		}

		if (idx!=-1) idx = GenInfoBuff->IndexOfObject((TObject*)(NativeInt)idx);
		isFiltered = true;
	}
	//Git(�t�@�C������)
	else if (isGit && (!FileName.IsEmpty() || fromGitView)) {
		int lno = 0;
		for (int i=0; i<GenInfoList->Count; i++) {
			UnicodeString lbuf = GenInfoList->Strings[i];
			if (StartsStr("@@ ", lbuf)) {
				UnicodeString s = get_tkn(get_tkn_r(lbuf, "@@ "), " @@ ");
				s = get_tkn(get_tkn_r(s, ' '), ',');
				s.Delete(1, 1);
				lno = s.ToIntDef(1);
			}
			GenInfoBuff->AddObject(lbuf, (TObject*)(NativeInt)lno);
		}
		isFiltered = false;
	}
	//�ʏ�\��
	else {
		for (int i=0; i<GenInfoList->Count; i++) GenInfoBuff->AddObject(GenInfoList->Strings[i], (TObject*)(NativeInt)i);
		isFiltered = false;
	}
	cursor_Default();

	//���X�g�{�b�N�X�Ɋ��蓖��(���z)
	assign_FileListBox(lp, GenInfoBuff, 0);
	if (lp->Count>0) lp->ItemIndex = (idx!=-1)? idx : 0;
	ListScrPanel->UpdateKnob();

	SetStatusBar();

	return (reload && !SameStr(last_text, GenInfoBuff->Text));
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TGeneralInfoDlg::GetBuffText()
{
	if (isTree || isFileList) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Assign(GenInfoBuff);
		for (int i=0; i<fbuf->Count; i++) fbuf->Strings[i] = get_pre_tab(fbuf->Strings[i]);
		return fbuf->Text;
	}
	else {
		return GenInfoBuff->Text;
	}
}

//---------------------------------------------------------------------------
//�����񌟍�
//---------------------------------------------------------------------------
bool __fastcall TGeneralInfoDlg::FindText(
	bool down)		//true = ������/ false = �����
{
	if (FindWord.IsEmpty() && RegExPtn.IsEmpty()) return false;

	cursor_HourGlass();

	TListBox *lp = GenListBox;
	int idx0 = lp->ItemIndex;
	if (idx0==-1) idx0 = 0;
	int idx1 = -1;
	bool case_sw = FindTextDlg->CaseCheckBox->Checked;

	//���K�\��
	if ((FindTextDlg->MigemoCheckBox->Enabled && FindTextDlg->MigemoCheckBox->Checked)
		|| FindTextDlg->RegExCheckBox->Checked)
	{
		if (chk_RegExPtn(RegExPtn)) {
			TRegExOptions opt;
			if (!case_sw) opt << roIgnoreCase;
			for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1)) {
				if (TRegEx::IsMatch(lp->Items->Strings[i], RegExPtn, opt)) { idx1 = i; break; }
			}

			if (ListScrPanel->KeyWordChanged(RegExPtn, lp->Count, case_sw)) {
				for (int i=0; i<lp->Count; i++) {
					if (TRegEx::IsMatch(lp->Items->Strings[i], RegExPtn, opt))
						ListScrPanel->AddHitLine(i);
				}
				ListScrPanel->Repaint();
			}
		}
		else {
			ListScrPanel->KeyWordChanged(EmptyStr, 0);
		}
	}
	//�ʏ�
	else if (!FindWord.IsEmpty()) {
		for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1)) {
			if (case_sw? ContainsStr(lp->Items->Strings[i], FindWord) : ContainsText(lp->Items->Strings[i], FindWord)) {
				 idx1 = i; break;
			}
		}

		if (ListScrPanel->KeyWordChanged(FindWord, lp->Count, case_sw)) {
			for (int i=0; i<lp->Count; i++) {
				if (case_sw? ContainsStr(lp->Items->Strings[i], FindWord) : ContainsText(lp->Items->Strings[i], FindWord))
					ListScrPanel->AddHitLine(i);
			}
			ListScrPanel->Repaint();
		}
	}
	else {
		ListScrPanel->KeyWordChanged(EmptyStr, 0);
	}

	Found = (idx1!=-1);

	//���ʕ\��
	UnicodeString msg; msg.sprintf(_T("[%s] "), FindWord.c_str());
	if (Found) {
		lp->ItemIndex = idx1;
		lp->Repaint();
		msg += "����";
	}
	else {
		beep_Warn();
		msg += "������܂���";
	}
	SetStatusBar(msg);
	cursor_Default();

	return (Found);
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̐ݒ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SetStatusBar(UnicodeString msg)
{
	UnicodeString stt_str;
	TListBox *lp = GenListBox;
	stt_str.sprintf(_T("%s: %u"), (ErrOnly? _T("ERR") : _T("����")), lp->Count);

	if (isFiltered) 		stt_str.cat_sprintf(_T("/%u"),	GenInfoList->Count);
	if (lp->ItemIndex!=-1)	stt_str.cat_sprintf(_T("  -  %u"),lp->ItemIndex + 1);
	if (lp->SelCount>0)		stt_str.cat_sprintf(_T("    �I��: %u"), lp->SelCount);
	StatusBar1->Panels->Items[0]->Text = stt_str;
	StatusBar1->Panels->Items[1]->Text = get_NameOfCodePage(CodePage, false, HasBOM);
	StatusBar1->Panels->Items[2]->Text = LineBreakStr;

	UnicodeString lbuf = (lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr;
	UnicodeString fnam = (isTree || isFileList)? get_post_tab(lbuf) : EmptyStr;

	UnicodeString id3_str;
	if (isPlayList && test_Mp3Ext(get_extension(lbuf))) {
		std::unique_ptr<TStringList> lst(new TStringList());
		if (ID3_GetInf(lbuf, lst.get(), true)) {
			id3_str.cat_sprintf(_T("%s - %s / %s"),
				lst->Values["TP1"].c_str(), lst->Values["TT2"].c_str(), lst->Values["TAL"].c_str());
		}
	}

	int w3 = StatusBar1->ClientWidth - 16;
	for (int i=0; i<(StatusBar1->Panels->Count - 1); i++) w3 -= StatusBar1->Panels->Items[i]->Width;

	StatusBar1->Panels->Items[3]->Text =
			 !msg.IsEmpty()? msg :
		!FirstKey.IsEmpty()? "2�X�g���[�N�L�[: " + FirstKey :
			!fnam.IsEmpty()? get_MiniPathName(fnam, w3, StatusBar1->Font) :
		 !id3_str.IsEmpty()? id3_str :
		!FileName.IsEmpty()? get_FileInfStr(FileName, false) : EmptyStr;

	StatusBar1->Repaint();
}
//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̕`��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect) ;

	UnicodeString lbuf = Panel->Text;
	int yp = (Rect.Height() - abs(cv->Font->Height)) / 2;
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(Rect.Left + ScaledIntX(2), yp, split_pre_tab(lbuf));
	if (!lbuf.IsEmpty()) cv->TextOut(Rect.Right - cv->TextWidth(lbuf) - ScaledIntX(4), yp, lbuf);
}

//---------------------------------------------------------------------------
//�t�B���^
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditChange(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	TListBox *lp = GenListBox;

	if		(contained_wd_i(KeysStr_ToList, KeyStr) || equal_ENTER(KeyStr)) lp->SetFocus();
	else if (MovListBoxFromFilter(lp, KeyStr))		;
	else if (SameText(KeyStr, KeyStr_Migemo))		MigemoAction->Checked = !MigemoAction->Checked;
	else if	(equal_ESC(KeyStr))						ModalResult = mrCancel;
	else return;

	lp->Invalidate();
	SetStatusBar();
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = get_WinColor();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterBtnClick(TObject *Sender)
{
	FilterEdit->SetFocus();
}

//---------------------------------------------------------------------------
//�ꗗ�̕`�� (���z)
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	TRect rc = Rect;
	int yp = rc.Top + 1;
	int tw = FileName.IsEmpty()? 4 : lp->TabWidth;
	int lw = 0;
	UnicodeString lbuf = lp->Items->Strings[Index];

	//����������
	if (lbuf.Length()>1024) lbuf = lbuf.SubString(1, 1024) + "�c";	//***

	//�t�B���^/�����̃}�b�`��
	std::unique_ptr<TStringList> wlist(new TStringList());
	if (HighlightAction->Checked) {
		if (Found && Index==lp->ItemIndex) {
			get_MatchWordList(lbuf, RegExPtn,
				false, true, AndOrAction->Checked, false, wlist.get());
		}
		else if (!FilterEdit->Text.IsEmpty()) {
			get_MatchWordList(lbuf, FilterEdit->Text,
				MigemoAction->Checked, false, AndOrAction->Checked, false, wlist.get());
		}
	}

	//�s�ԍ�
	if (ShowLineNoAction->Checked) {
		if (isGit) {
			int n  = (int)lp->Items->Objects[Index];
			int n1 = (Index>1)? (int)lp->Items->Objects[Index - 1] : 0;
			LineNoOut(cv, rc, (n!=n1)? n : 0);
		}
		else {
			LineNoOut(cv, rc, (int)lp->Items->Objects[Index] + 1);
		}
	}

	//�w�i�F
	cv->Brush->Color = State.Contains(odSelected)? (lp->Focused()? col_selItem : col_oppItem) :
											isLog? col_bgLog :
			 (isPlayList && is_AltLnBgCol(Index))? col_bgList2 : col_bgList;
	cv->FillRect(rc);

	rc.Left += 4;
	int xp = rc.Left;
	int w0 = rc.Left;

	bool use_fgsel = is_SelFgCol(State);

	//���O
	if (isLog) {
		RuledLnTextOut(yen_to_delimiter(lbuf), cv, rc,
			(use_fgsel? col_fgSelItem : get_LogColor(lbuf)), tw, wlist.get());
	}
	//�ϐ��ꗗ
	else if (isVarList) {
		if (lbuf.Pos('=')) {
			UnicodeString namstr = split_tkn(lbuf, '=') + " = ";
			rc.Left = xp + MaxNameWidth - get_TextWidth(cv, namstr, is_irreg);
			RuledLnTextOut(namstr, cv, rc, use_fgsel? col_fgSelItem : col_fgInfNam, tw, wlist.get());
			rc.Left = xp + MaxNameWidth;
			lw = MaxNameWidth + get_TabTextWidth(lbuf, cv, tw);
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_fgList, tw, wlist.get());
		}
		else {
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_Headline, tw, wlist.get());
		}
	}
	//�R�}���h����
	else if (isCmdHistory) {
		UnicodeString cmd_inf = split_pre_tab(lbuf);
		int x0  = rc.Left;
		int y	= rc.Top + 1;
		int mgn = cv->TextWidth("W");
		//Time
		UnicodeString s = split_tkn(cmd_inf, ' ');
		bool is_brk = false;
		if (Index>0) {
			TDateTime idx_t, lst_t;
			if (str_to_DateTime(get_tkn(s, '.'), &idx_t)
				&& str_to_DateTime(get_tkn(lp->Items->Strings[Index - 1], '.'), &lst_t))
			{
				is_brk = (MilliSecondsBetween(idx_t, lst_t) > (30*1000));
			}
		}
		TColor fg = use_fgsel? col_fgSelItem : AdjustColor(col_fgList, ADJCOL_FGLIST);
		out_TextEx(cv, xp, y, s, fg, col_None, mgn, is_irreg);
		//ID
		UnicodeString id = split_tkn(cmd_inf, ' ');
		fg = use_fgsel? col_fgSelItem : col_fgList;
		out_TextEx(cv, xp, y, ReplaceStr(id, "-", " "), fg, col_None, mgn, is_irreg);
		//L/R
		s = split_tkn(cmd_inf, ' ');
		for (int i=1; i<=s.Length(); i++) {
			fg = use_fgsel? col_fgSelItem : (s[i]=='_')? AdjustColor(col_fgList, ADJCOL_FGLIST) : col_fgList;
			out_TextEx(cv, xp, y, s[i], fg, col_None, 0, is_irreg);
		}
		xp += (mgn * 2);
		//ScrMode �ύX
		if ((id=='-')) {
			fg = use_fgsel? col_fgSelItem : col_Comment;
			out_TextEx(cv, xp, y, cmd_inf, fg, col_None, mgn, is_irreg);
			rc.Left = xp;
			draw_Separator(cv, rc);
		}
		//�R�}���h
		else {
			//command
			UnicodeString cmd = get_CmdStr(cmd_inf);
			rc.Left = xp;
			RuledLnTextOut(cmd, cv, rc, use_fgsel? col_fgSelItem : col_Headline, tw, wlist.get());
			xp = rc.Left;
			//param
			UnicodeString prm = get_PrmStr(cmd_inf);
			if (!prm.IsEmpty()) {
				fg = use_fgsel? col_fgSelItem : col_Symbol;
				out_TextEx(cv, xp, y, "_", fg, col_None, 0, is_irreg);
				rc.Left = xp;
				RuledLnTextOut(prm, cv, rc, use_fgsel? col_fgSelItem : col_fgList, tw, wlist.get());
				xp = rc.Left + mgn * 2;
			}
			//�J�����g�p�X
			if (!lbuf.IsEmpty()) {
				int cmd_w = x0 + get_CharWidth(cv, 40);
				int tab_w = get_CharWidth(cv, 8);
				if (xp<cmd_w) xp = cmd_w; else xp += (tab_w - xp%tab_w);
				cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
				PathNameOut(lbuf, wlist.get(), cv, xp, yp);
			}
			//30�b�ȏ�󂢂��番����
			if (is_brk) draw_separateLine(cv, Rect);
		}
	}
	//�t�@�C�����X�g
	else if (isFileList) {
		UnicodeString fnam = get_post_tab(lbuf);
		bool is_dir = ends_PathDlmtr(fnam);
		fnam = ExcludeTrailingPathDelimiter(fnam);
		lbuf = get_pre_tab(lbuf);

		if (StartsText(fnam, lbuf)) {
			lbuf.Delete(1, fnam.Length());
		}
		else {
			fnam = ExtractFileName(fnam);
			if (StartsText(fnam, lbuf)) lbuf.Delete(1, fnam.Length()); else fnam = EmptyStr;
		}

		if (!fnam.IsEmpty()) {
			if (is_dir) {
				cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
				PathNameOut(fnam, wlist.get(), cv, xp, yp);
			}
			else {
				FileNameOut(cv, rc, fnam, use_fgsel, true, wlist.get());
			}
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_fgList, tw, wlist.get());
		}
		else {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
	}
	//�f�B���N�g�����ꗗ
	else if (isDirs) {
		cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
		PathNameOut(lbuf, wlist.get(), cv, xp, yp);
	}
	//�f�B���N�g���c���[�\��
	else if (isTree) {
		lbuf = get_pre_tab(lbuf);
		TColor fg = use_fgsel? col_fgSelItem : col_Folder;
		if (Index==0) {
			cv->Font->Color = fg;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
		else {
			RuledLnTextOut(lbuf, cv, rc, fg, tw, wlist.get());
		}
	}
	//�v���C���X�g
	else if (isPlayList) {
		//�Đ����}�[�N
		if (SameText(PlayFile, lbuf))
			out_Text(cv, rc.Left + 2, yp, PLAY_Mark.c_str(), is_ListPlaying()? col_Cursor : col_InvItem);
		rc.Left += get_TextWidth(cv, PLAY_Mark, is_irreg) + 4;

		if (OmitComPathAction->Checked) remove_top_text(lbuf, ComPathName);	//�p�X�̋��ʕ������ȗ�
		if (FileName1stAction->Checked) {
			xp = rc.Left;
			RuledLnTextOut(ExtractFileName(lbuf),
				cv, rc, use_fgsel? col_fgSelItem : get_ExtColor(get_extension(lbuf)), tw, wlist.get());
			rc.Left = xp + MaxNameWidth;
			FileNameOut(cv, rc, ExtractFilePath(lbuf), use_fgsel, true, wlist.get());
		}
		else {
			FileNameOut(cv, rc, lbuf, use_fgsel, true, wlist.get());
		}
	}
	//NyanFi ���
	else if (USAME_TS(Caption, "NyanFi ���")) {
		TColor fg =
			use_fgsel? col_fgSelItem :
			is_match_regex(lbuf, _T("^([^ :.]{3,}\\.(exe|dll|spi)|\\.|WIC)"))? col_Headline :
			StartsStr("         �G���[:", lbuf)? col_Error :
			StartsStr('[', lbuf)? col_Comment : col_fgList;

		if (EndsStr('\\', lbuf) && lbuf.Pos(':')>1) {
			cv->Font->Color = fg;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
		else {
			PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get());
		}
	}
	//Git
	else if (isGit) {
		TColor fg = use_fgsel? col_fgSelItem :
			 		(StartsStr("$ git ", lbuf) || StartsStr("@@ ", lbuf))? col_Headline :
		 	 		StartsStr("-", lbuf)? col_GitDel : StartsStr("+", lbuf)? col_GitIns : col_fgList;
		UnicodeString s = split_GitGraphStr(lbuf);
		if (!s.IsEmpty()) {
			UnicodeString s1 = (Index>1)? get_GitGraphStr(lp->Items->Strings[Index - 1]) : EmptyStr;
			UnicodeString s2 = (Index<(lp->Count - 1))? get_GitGraphStr(lp->Items->Strings[Index + 1]) : EmptyStr;
			draw_GitGraph(s, s1, s2, cv, rc, get_in_paren(lbuf).Pos("HEAD -> "));
		}
		if (!lbuf.IsEmpty()) RuledLnTextOut(lbuf, cv, rc, fg, tw, wlist.get());
	}
	//���o���s
	else if (!ptn_match_str(HdrLnStr, lbuf).IsEmpty()) {
		TColor fg = use_fgsel? col_fgSelItem : col_Headline;
		PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get());
	}
	//�R�����g or �ʏ�s
	else {
		TColor fg = use_fgsel? col_fgSelItem : col_fgList;
		PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get(), FileName, (SortMode==0 && !isFiltered));
	}

	//�X�N���[������ݒ�
	int lst_scr_wd = lp->ScrollWidth;
	ListWidth = std::max(ListWidth, ((lw>0)? lw : get_TabTextWidth(lbuf, cv, tw)) + get_CharWidth(cv, 4));
	lp->ScrollWidth = w0 + ListWidth;
	if (lst_scr_wd!=lp->ScrollWidth) ListScrPanel->UpdateKnob();

	//�J�[�\��
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<GenInfoBuff->Count)? GenInfoBuff->Strings[Index] : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDataObject(TWinControl *Control, int Index, TObject *&DataObject)
{
	DataObject = (Index>=0 && Index<GenInfoBuff->Count)? GenInfoBuff->Objects[Index] : NULL;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxExit(TObject *Sender)
{
	CancelKeySeq();
	((TListBox*)Sender)->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxClick(TObject *Sender)
{
	CancelKeySeq();
}

//---------------------------------------------------------------------------
//2�X�g���[�N����̏���
//---------------------------------------------------------------------------
UnicodeString __fastcall TGeneralInfoDlg::TwoStrokeSeq(WORD &Key, TShiftState Shift)
{
	UnicodeString key_str = get_KeyStr(Key);	if (key_str.IsEmpty()) return EmptyStr;
	key_str = get_ShiftStr(Shift) + key_str;

	//2�X�g���[�N��
	if (Wait2ndKey) {
		Wait2ndKey = false;
		key_str.sprintf(_T("%s~%s"), FirstKey.c_str(), get_KeyStr(Key).c_str());
		CancelKeySeq();
		if (Key_to_CmdV(key_str).IsEmpty()) Key = 0;
	}
	//1�X�g���[�N��
	else if (is_FirstKey("V", key_str)) {
		FirstKey   = key_str;
		Wait2ndKey = true;
		SetStatusBar();
		Key = 0;
	}

	return key_str;
}
//---------------------------------------------------------------------------
//2�X�g���[�N����𒆒f
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CancelKeySeq()
{
	FirstKey   = EmptyStr;
	Wait2ndKey = false;
	SetStatusBar();
}

//---------------------------------------------------------------------------
//�ꗗ�̃L�[����
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = TwoStrokeSeq(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	TListBox *lp = (TListBox*)Sender;

	//�R�}���h
	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V)) {
		if (StartsText("Zoom", cmd_F) || StartsText("Zoom", cmd_V)) ResetListWidth();
	}
	//�t�B���^����
	else if (StartsText("IncSearch", cmd_F)) {
		FilterEdit->SetFocus();
	}
	//�\�[�g
	else if (USAME_TI(cmd_V, "Sort_AO")) {
		SortAscAction->Execute();
	}
	else if (USAME_TI(cmd_V, "Sort_DO")) {
		SortDesAction->Execute();
	}
	else if (USAME_TI(cmd_V, "Sort")) {
		OrgOrderAction->Execute();
	}
	//����
	else if (USAME_TI(cmd_V, "FindText")) {
		FindTextDlg->ShowModal();
	}
	else if (contained_wd_i(_T("FindDown|FindUp"), cmd_V)) {
		FindText(USAME_TI(cmd_V, "FindDown"));
	}
	//�G���[����
	else if (contained_wd_i(_T("NextErr|PrevErr"), cmd_V)) {
		if (isLog) {
			bool down = USAME_TI(cmd_V, "NextErr");
			cursor_HourGlass();
			int idx0 = lp->ItemIndex;
			if (idx0==-1) idx0 = 0;
			int idx1 = -1;
			UnicodeString ptn = "^.>([ECW]|(     [45]\\d{2})) .*";
			for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1))
				if (TRegEx::IsMatch(lp->Items->Strings[i], ptn)) { idx1 = i; break; }
			if (idx1!=-1) {
				lp->ItemIndex = idx1;
				lp->Repaint();
			}
			else beep_Warn();
			cursor_Default();
		}
	}
	//�O��̃t�@�C���֐؂�ւ�
	else if (!isPlayList && !isFTP && !FileName.IsEmpty()
		&& contained_wd_i(_T("PrevFile|NextFile"), cmd_V))
	{
		RetStr = cmd_V;
		this->Perform(WM_SETREDRAW, 0, (NativeInt)0);	//��ʂ��������Ɏc��
		ModalResult = mrRetry;
	}
	//�{��
	else if (USAME_TI(cmd_F, "TextViewer")) {
		if (ViewFileAction->Enabled) ViewFileAction->Execute(); else ViewListAction->Execute();
	}
	//�ҏW
	else if (USAME_TI(cmd_F, "FileEdit")) {
		EditFileAction->Execute();
	}
	//�t�@�C�����
	else if (USAME_TI(cmd_F, "ListFileInfo") || StartsText("ShowFileInfo", cmd_F)) {
		ShowFileInfoAction->Execute();
	}
	//�v���p�e�B
	else if (USAME_TI(cmd_F, "PropertyDlg")) {
		PropertyAction->Execute();
	}
	//�L�[
	else if (equal_ENTER(KeyStr)) {
		TListBox *lp = GenListBox;
		if (lp->ItemIndex!=-1) {
			UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
			UnicodeString cmd;
			//�J�[�\���ʒu�Ɉړ�
			if (isFileList) {
				if (lp->SelCount>0) {
					for (int i=0; i<lp->Count; i++) {
						if (lp->Selected[i]) GenSelList->Add(get_post_tab(lp->Items->Strings[i]));
					}
				}
				RetStr = get_post_tab(fnam);
				ModalResult = mrOk;
			}
			else if (isDirs) {
				CmdRequestList->Add(cmd.sprintf(_T("ChangeDir_\"%s\""), IncludeTrailingPathDelimiter(fnam).c_str()));
				ModalResult = mrOk;
			}
			else if (isTree) {
				fnam = get_post_tab(fnam);
				if (fnam.Pos("/")) {
					if (EndsStr("/", fnam) || EndsStr("\\", fnam)) fnam += "*";
					CmdRequestList->Add(cmd.sprintf(_T("JumpTo_\"%s\""), fnam.c_str()));
				}
				else {
					CmdRequestList->Add(cmd.sprintf(_T("ChangeDir_\"%s\""), fnam.c_str()));
				}
				ModalResult = mrOk;
			}
			//�J�[�\���ʒu���Đ�
			else if (isPlayList) {
				int idx = PlayList->IndexOf(fnam);
				if (idx!=-1) {
					if (SameText(fnam, PlayFile)) {
						if (is_ListPlaying())
							mciSendString(_T("pause PLYLIST"), NULL, 0, NULL);
						else
							mciSendString(_T("resume PLYLIST"), NULL, 0, NULL);
						GenListBox->Invalidate();
					}
					else {
						cursor_HourGlass();
						PlayStbIdx = idx;
						play_PlayList(false, true);	//�V���b�t���}�~
						lp->Repaint();
						cursor_Default();
					}
				}
			}
		}
	}
	else if (SameText(KeyStr, KeyStr_Copy)) {
		CopyAction->Execute();
	}
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
		show_PopupMenu(lp);
	}
	//����
	else if (USAME_TI(cmd_V, "Close") || equal_ESC(KeyStr)) {
		ModalResult = mrCancel;
	}
	else {
		return;
	}

	if (!StartsText("Find", cmd_V)) SetStatusBar();
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//�C���N�������^���T�[�`�����
	if (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDblClick(TObject *Sender)
{
	CancelKeySeq();
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyActionExecute(TObject *Sender)
{
	ExeCmdListBox(GenListBox, _T("ClipCopy"));
	SetStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (GenListBox->SelCount>0);
}

//---------------------------------------------------------------------------
//�l���R�s�[
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyValActionExecute(TObject *Sender)
{
	TListBox *lp = GenListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) copy_to_Clipboard(get_tkn_r(lp->Items->Strings[idx], '='));
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyValActionUpdate(TObject *Sender)
{
	TAction *ap  = (TAction*)Sender;
	ap->Visible  = isVarList;
	TListBox *lp = GenListBox;
	ap->Enabled  = ap->Visible && (lp->ItemIndex!=-1) && lp->Items->Strings[lp->ItemIndex].Pos('=');
}

//---------------------------------------------------------------------------
//�R�}���h�����őI���s�̃R�}���h���e���擾
//---------------------------------------------------------------------------
UnicodeString __fastcall TGeneralInfoDlg::GetSelcetedCmd(TStringList *lst)
{
	std::unique_ptr<TStringList> cpy_buf(new TStringList());
	TListBox *lp = GenListBox;
	for (int i=0; i<lp->Count; i++) {
		if (lp->Selected[i]) {
			UnicodeString lbuf = get_pre_tab(lp->Items->Strings[i]);
			lbuf = get_tkn_r(lbuf, ' ');
			UnicodeString id = split_tkn(lbuf, ' ');
			if (!SameStr(id, "-")) {
				lbuf = get_tkn_r(lbuf, ' ');
				if (!lbuf.IsEmpty()) cpy_buf->Add(lbuf);
			}
			lp->Selected[i] = false;
		}
	}
	if (lst) lst->Assign(cpy_buf.get());
	return cpy_buf->Text;
}

//---------------------------------------------------------------------------
//�R�}���h���R�s�[
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyCmdActionExecute(TObject *Sender)
{
	copy_to_Clipboard(GetSelcetedCmd());
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyCmdActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isCmdHistory;
	ap->Enabled = ap->Visible && (GenListBox->SelCount>0);
}

//---------------------------------------------------------------------------
//�I���s���R�}���h�t�@�C���Ƃ��ĕۑ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SaveAsNbtActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> cb_buf(new TStringList());
	if (!GetSelcetedCmd(cb_buf.get()).IsEmpty()) {
		UserModule->PrepareSaveDlg(_T("�R�}���h�t�@�C���Ƃ��ĕۑ�"), F_FILTER_NBT, NULL, CmdFilePath);
		UnicodeString fnam = UserModule->SaveDlgExecute();
		if (!fnam.IsEmpty()) {
			CmdFilePath = ExtractFilePath(fnam);
			if (!saveto_TextUTF8(fnam, cb_buf.get())) msgbox_ERR(USTR_FaildSave);
		}
	}
}

//---------------------------------------------------------------------------
//�R�}���h�������N���A
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ClrCmdHistoryActionExecute(TObject *Sender)
{
	if (msgbox_Sure(LoadUsrMsg(USTR_DelHistoryQ, _T("�R�}���h")))) {
		CommandHistory->Clear();
		GenInfoList->Assign(CommandHistory);
		UpdateList();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ClrCmdHistoryActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isCmdHistory;
	ap->Enabled = ap->Visible && (GenListBox->Count>0);
}

//---------------------------------------------------------------------------
//���ׂđI��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SelAllActionExecute(TObject *Sender)
{
	ListBoxSelectAll(GenListBox);
}

//---------------------------------------------------------------------------
//����
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FindTextItemClick(TObject *Sender)
{
	FindTextDlg->ShowModal();
}

//---------------------------------------------------------------------------
//�ꗗ���t�@�C���ɕۑ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SaveAsActionExecute(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, USTR_List);
	UserModule->SaveTxtDlg->InitialDir	  = ExcludeTrailingPathDelimiter(SaveTxtPath);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveTxtPath  = ExtractFilePath(UserModule->SaveTxtDlg->FileName);
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Text = GetBuffText();
		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, USTR_List));
	}
}
//---------------------------------------------------------------------------
//�ꗗ���e�L�X�g�r���A�[�ŊJ��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewListActionExecute(TObject *Sender)
{
	Clipboard()->AsText = GetBuffText();
	CmdRequestList->Add("TextViewer_CB");
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewListActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = FileName.IsEmpty();
	ap->Enabled = (GenListBox->Count>0) && !fromGitView;
}
//---------------------------------------------------------------------------
//�t�@�C�����e�L�X�g�r���A�[�ŊJ��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewFileActionExecute(TObject *Sender)
{
	int idx = GenListBox->ItemIndex;
	if (idx==-1) idx = 0;
	int lno =  (int)GenInfoBuff->Objects[idx] + 1;
	UnicodeString cmd;
	CmdRequestList->Add(cmd.sprintf(_T("TextViewer_\"%s\"\t%u"), FileName.c_str(), lno));
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
//�t�@�C�����e�L�X�g�G�f�B�^�ŊJ��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::EditFileActionExecute(TObject *Sender)
{
	int idx = GenListBox->ItemIndex;
	if (idx==-1) idx = 0;
	int lno = (int)GenInfoBuff->Objects[idx] + 1;
	UnicodeString cmd;
	CmdRequestList->Add(cmd.sprintf(_T("FileEdit_\"%s\"\t%u"), FileName.c_str(), lno));
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OpenFileActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = !FileName.IsEmpty();
	ap->Enabled = Active && ap->Visible;
}

//---------------------------------------------------------------------------
//�t�@�C������\��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ShowFileInfoActionExecute(TObject *Sender)
{
	TListBox *lp = GenListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
		if (lp->Tag & LBTAG_TAB_FNAM) fnam = get_post_tab(fnam);
		if (isPlayList && PlayList->IndexOf(fnam)==-1) fnam = EmptyStr;
		if (!fnam.IsEmpty()) FileInfoDlg->ShowModalEx(fnam);
	}
}
//---------------------------------------------------------------------------
//�v���p�e�B
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PropertyActionExecute(TObject *Sender)
{
	TListBox *lp = GenListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
		if (lp->Tag & LBTAG_TAB_FNAM) fnam = get_post_tab(fnam);
		if (isPlayList && PlayList->IndexOf(fnam)==-1) fnam = EmptyStr;
		if (!fnam.IsEmpty()) {
			pos_ListBoxItem(lp);
			ShowPropertyDialog(fnam);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PropertyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isFileList || isPlayList || isTree;
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//�s�ԍ���\��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ShowLineNoActionExecute(TObject *Sender)
{
	ShowLineNoAction->Checked = !ShowLineNoAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
//�t�@�C�������ɕ\��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FileName1stActionExecute(TObject *Sender)
{
	FileName1stAction->Checked = !FileName1stAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FileName1stActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isPlayList;
}
//---------------------------------------------------------------------------
//�p�X�̋��ʕ������ȗ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OmitComPathActionExecute(TObject *Sender)
{
	OmitComPathAction->Checked = !OmitComPathAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OmitComPathActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isPlayList;
}

//---------------------------------------------------------------------------
//�J�[�\���ʒu���ێ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::KeepIndexActionExecute(TObject *Sender)
{
	KeepIndexAction->Checked = !KeepIndexAction->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::KeepIndexActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = !FileName.IsEmpty() && !isFTP;	//�O��؂�ւ��\��
}

//---------------------------------------------------------------------------
//�G���[�ӏ����i�荞�ݕ\��
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ErrOnlyActionExecute(TObject *Sender)
{
	ErrOnly = !ErrOnly;
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ErrOnlyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isLog;
	ap->Checked = ErrOnly;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::InfPopupMenuPopup(TObject *Sender)
{
	SortItem->Enabled		  = !isTree;
	DelDuplItem->Enabled	  = !isTree;
	RestoreListItem->Enabled  = !isTree;

	reduction_MenuLine(InfPopupMenu->Items);
}

//---------------------------------------------------------------------------
//�ꗗ�̃\�[�g
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortGenList(int mode)
{
	cursor_HourGlass();

	TListBox *lp = GenListBox;
	int id = (lp->ItemIndex!=-1)? (int)GenInfoBuff->Objects[lp->ItemIndex] : -1;

	GenInfoBuff->CustomSort((mode==1)? comp_AscendOrder : (mode==-1)? comp_DescendOrder : comp_ObjectsOrder);

	assign_FileListBox(lp, GenInfoBuff, 0);
	int idx = 0;
	if (id!=-1) idx = GenInfoBuff->IndexOfObject((TObject*)(NativeInt)id);
	lp->ItemIndex = idx;

	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortAscActionExecute(TObject *Sender)
{
	SortMode = 1;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortDesActionExecute(TObject *Sender)
{
	SortMode = -1;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OrgOrderActionExecute(TObject *Sender)
{
	SortMode = 0;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
//�ꗗ�̏d���s���폜
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::DelDuplActionExecute(TObject *Sender)
{
	cursor_HourGlass();
	//��U������
	GenInfoBuff->CustomSort(comp_AscendOrder);
	//�d���s�폜
	UnicodeString laststr = GenInfoBuff->Strings[0];
	int i = 1;
	int cnt = 0;
	while (i<GenInfoBuff->Count) {
		UnicodeString lbuf = GenInfoBuff->Strings[i];
		if (SameStr(laststr, lbuf)) {
			GenInfoBuff->Delete(i);
			cnt++;
		}
		else {
			laststr = lbuf;
			i++;
		}
	}
	assign_FileListBox(GenListBox, GenInfoBuff);
	cursor_Default();

	//���̕��тɒ���
	SortGenList(SortMode);
	GenListBox->ItemIndex = 0;

	//���ʕ\��
	UnicodeString msg;
	if (cnt>0)
		msg.sprintf(_T("%u�s�̏d���s���폜"), cnt);
	else
		msg += "�d���s�͂���܂���";
	SetStatusBar(msg);
}

//---------------------------------------------------------------------------
//�ꗗ�����̓��e�ɖ߂�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::RestoreListActionExecute(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ListActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (GenListBox->Count>0 && !isTree);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ToggleActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	Timer1->Enabled = WatchAction->Checked;
	NotifyAction->Enabled = WatchAction->Checked;

	if (ap==HighlightAction) GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::TailActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isTail;
	ap->Enabled = isTail;
}

//---------------------------------------------------------------------------
//�Đ�/�ꎞ��~
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PlayPauseActionExecute(TObject *Sender)
{
	if (is_ListPlaying())
		mciSendString(_T("pause PLYLIST"), NULL, 0, NULL);
	else
		mciSendString(_T("resume PLYLIST"), NULL, 0, NULL);
	GenListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PlayPauseActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isPlayList;
	ap->Enabled = isPlayList;

	if (isPlayList) {
		ap->Checked = is_ListPlaying();
		ap->Hint	= is_ListPlaying()? "�ꎞ��~" : "�Đ�";
	}
}
//---------------------------------------------------------------------------
//���̋Ȃ��Đ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PlayNextActionExecute(TObject *Sender)
{
	play_PlayList(false, true);
	GenListBox->Invalidate();
}
//---------------------------------------------------------------------------
//�O�̋Ȃ��Đ�
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PlayPrevActionExecute(TObject *Sender)
{
	play_PlayList(true, true);
	GenListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PlayActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isPlayList;
	ap->Enabled = isPlayList;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, HelpContext);
}
//---------------------------------------------------------------------------

