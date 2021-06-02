//----------------------------------------------------------------------//
// NyanFi																//
//  �g���q�ʈꗗ														//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "FileInfDlg.h"
#include "FileExtDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFileExtensionDlg *FileExtensionDlg = NULL;

//---------------------------------------------------------------------------
//�g���q�ꗗ�\�[�g�p��r�֐�
//---------------------------------------------------------------------------
int ExtSortOdr[4];		//�g���q�ꗗ�̃\�[�g����
int FileSortOdr[2];		//�t�@�C���ꗗ�̃\�[�g����

//---------------------------------------------------------------------------
//�g���q
int __fastcall FextComp_Ext(TStringList *List, int Index1, int Index2)
{
	return (StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str()) * ExtSortOdr[0]);
}
//---------------------------------------------------------------------------
//�t�@�C����
int __fastcall FextComp_Count(TStringList *List, int Index1, int Index2)
{
	ext_inf_rec *ip0 = (ext_inf_rec*)List->Objects[Index1];
	ext_inf_rec *ip1 = (ext_inf_rec*)List->Objects[Index2];
	if (!ip0 || !ip1) return 0;

	return (ip0->f_count==ip1->f_count)?
		StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str()) * ExtSortOdr[0] :
		((ip1->f_count < ip0->f_count)? 1 : -1) * ExtSortOdr[1];
}
//---------------------------------------------------------------------------
//���v�T�C�Y
int __fastcall FextComp_Size(TStringList *List, int Index1, int Index2)
{
	ext_inf_rec *ip0 = (ext_inf_rec*)List->Objects[Index1];
	ext_inf_rec *ip1 = (ext_inf_rec*)List->Objects[Index2];
	if (!ip0 || !ip1) return 0;

	return (ip0->f_size==ip1->f_size)?
		StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str()) * ExtSortOdr[0] :
		((ip1->f_size < ip0->f_size)? 1 : -1) * ExtSortOdr[2];
}
//---------------------------------------------------------------------------
//���σT�C�Y
int __fastcall FextComp_Ave(TStringList *List, int Index1, int Index2)
{
	ext_inf_rec *ip0 = (ext_inf_rec*)List->Objects[Index1];
	ext_inf_rec *ip1 = (ext_inf_rec*)List->Objects[Index2];
	if (!ip0 || !ip1) return 0;

	return (ip0->av_size==ip1->av_size)?
		StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str()) * ExtSortOdr[0] :
		((ip1->av_size < ip0->av_size)? 1 : -1) * ExtSortOdr[3];
}
//---------------------------------------------------------------------------
//�t�@�C����
int __fastcall FextComp_Name(TStringList *List, int Index1, int Index2)
{
	UnicodeString fnam0 = ExtractFileName(List->Strings[Index1]);
	UnicodeString fnam1 = ExtractFileName(List->Strings[Index2]);
	return SameText(fnam0, fnam1)?
		StrCmpLogicalW(ExtractFileDir(List->Strings[Index1]).c_str(), ExtractFileDir(List->Strings[Index2]).c_str()) * FileSortOdr[1] :
		StrCmpLogicalW(fnam0.c_str(), fnam1.c_str()) * FileSortOdr[0];
}
//---------------------------------------------------------------------------
//�ꏊ
int __fastcall FextComp_Path(TStringList *List, int Index1, int Index2)
{
	UnicodeString dnam0 = ExtractFileDir(List->Strings[Index1]);
	UnicodeString dnam1 = ExtractFileDir(List->Strings[Index2]);
	return SameText(dnam0, dnam1)?
		StrCmpLogicalW(ExtractFileName(List->Strings[Index1]).c_str(), ExtractFileName(List->Strings[Index2]).c_str()) * FileSortOdr[0] :
		StrCmpLogicalW(dnam0.c_str(), dnam1.c_str()) * FileSortOdr[1];
}

//---------------------------------------------------------------------------
// TFileExtensionDlg �N���X
//---------------------------------------------------------------------------
__fastcall TFileExtensionDlg::TFileExtensionDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormCreate(TObject *Sender)
{
	InfoScrPanel = new UsrScrollPanel(ListPanel, InfoListBox,	USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	FileScrPanel = new UsrScrollPanel(FilePanel, FileListBox,	USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	FextInfoList = new TStringList();
	FileItemList = new TStringList();

	org_FextInfBarWndProc  = FextInfBar->WindowProc;
	FextInfBar->WindowProc = FextInfBarWndProc;

	org_FileInfBarWndProc  = FileInfBar->WindowProc;
	FileInfBar->WindowProc = FileInfBarWndProc;
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormShow(TObject *Sender)
{
	Caption = UnicodeString().sprintf(_T("�g���q�ʈꗗ - [%s]"), yen_to_delimiter(PathName).c_str());
	IniFile->LoadPosInfo(this, DialogCenter);

	ExtSortMode    = IniFile->ReadIntGen(_T("FileExtSortModeX"));
	ExtSortOdr[0]  = IniFile->ReadIntGen(_T("FileExtSortOdrX0"), 1);
	ExtSortOdr[1]  = IniFile->ReadIntGen(_T("FileExtSortOdrX1"), -1);
	ExtSortOdr[2]  = IniFile->ReadIntGen(_T("FileExtSortOdrX2"), -1);
	ExtSortOdr[3]  = IniFile->ReadIntGen(_T("FileExtSortOdrX3"), -1);

	FileSortMode   = IniFile->ReadIntGen(_T("FileExtSortModeF"));
	FileSortOdr[0] = IniFile->ReadIntGen(_T("FileExtSortOdrF0"), 1);
	FileSortOdr[1] = IniFile->ReadIntGen(_T("FileExtSortOdrF1"), 1);

	//�N���X�^�T�C�Y�擾
	ClusterSize = get_ClusterSize(PathName);

	//�g���q�ꗗ�̏�����
	set_StdListBox(InfoListBox);
	set_UsrScrPanel(InfoScrPanel);
	TCanvas *cv = InfoListBox->Canvas;

	//�w�b�_��������
	InitializeListHeader(FextInfHeader, _T("�g���q|�t�@�C����|���v�T�C�Y|���σT�C�Y"));
	FExtSctWd = get_CharWidth(cv, 14, ScaledInt((IconMode==1)? 20 : 0, this));	//" .XXXXXXXXXXXX" 
	FCntSctWd = get_CharWidth(cv, 10);	//" 9,999,999"
	SizeSctWd = get_CharWidth(cv, 13);	//" 1999.999 MB "
	PercSctWd = get_CharWidth(cv,  8);	//" 100.0% "
	set_HeaderSecWidth(FextInfHeader, 4, FExtSctWd, FCntSctWd, SizeSctWd + PercSctWd + 110, SizeSctWd + 8);
	THeaderSections *sp = FextInfHeader->Sections;
	for (int i=0; i<sp->Count; i++) FextInfBar->Panels->Items[i]->Width = sp->Items[i]->Width;

	setup_StatusBar(FextInfBar, ListFont);
	for (int i=0; i<FextInfBar->Panels->Count; i++) FextInfBar->Panels->Items[i]->Text = EmptyStr;

	//�t�@�C���ꗗ�̏�����
	FilePanel->Height = IniFile->ReadIntGen(_T("FileExtListHeight"), 150);
	InitializeListHeader(FileListHeader, _T("�t�@�C����|�ꏊ"));
	FileListHeader->Sections->Items[0]->Width = IniFile->ReadIntGen(_T("FileExtFileHdrW0"),	200);
	FileListHeader->Sections->Items[1]->Width = ClientWidth - FileListHeader->Sections->Items[0]->Width;

	FileListBox->Tag = LBTAG_OPT_LCPY;
	set_StdListBox(FileListBox);
	set_UsrScrPanel(FileScrPanel);

	setup_StatusBar(FileInfBar);
	FileInfBar->Panels->Items[0]->Text = EmptyStr;

	ListSplitter->Color = col_Splitter;

	FextMask  = EmptyStr;
	ErrMesage = EmptyStr;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::WmFormShowed(TMessage &msg)
{
	Repaint();

	//�������q���g�\���̐ݒ�
	HintLabel->Font->Assign(HintFont);
	HintLabel->Caption	  = LoadUsrMsg(USTR_SearchingESC);
	HintLabel->BoundsRect = HintPanel->ClientRect.CenteredRect(HintLabel->ClientRect);
	HintPanel->Color	  = col_bgHint;
	HintPanel->BoundsRect = InfoListBox->ClientRect.CenteredRect(HintPanel->ClientRect);
	HintPanel->Visible	  = true;
	HintPanel->Repaint();
	cursor_HourGlass();

	TotalFileCount = TotalSize = 0;
	AccDeniedCnt = 0;

	if (GetInfo(PathName)) {
		//���σT�C�Y�v�Z/ �g���q�̍ő�\�������擾
		TCanvas *cv = InfoListBox->Canvas;
		cv->Font->Assign(InfoListBox->Font);
		for (int i=0; i<FextInfoList->Count; i++) {
			ext_inf_rec *ip = (ext_inf_rec*)FextInfoList->Objects[i];
			ip->av_size = (ip->f_count>0)? ip->f_size/ip->f_count : 0;
		}
		SortListX();
		AverageSize = (TotalFileCount>0)? TotalSize/TotalFileCount : 0;

		//���v�\��
		FextInfBar->Panels->Items[0]->Text = FextInfoList->Count;
		FextInfBar->Panels->Items[1]->Text = get_size_str_B(TotalFileCount, 0);
		FextInfBar->Panels->Items[2]->Text = get_size_str_G(TotalSize,   10, SizeDecDigits);
		FextInfBar->Panels->Items[3]->Text = get_size_str_G(AverageSize, 10, SizeDecDigits);

		//���X�g�{�b�N�X�Ɋ��蓖��(���z)
		InfoListBox->Count = FextInfoList->Count;
		if (FextInfoList->Count>0) {
			InfoListBox->ItemIndex = 0;
			InfoListBoxClick(NULL);
		}
		InfoScrPanel->UpdateKnob();

	}
	HintPanel->Visible = false;

	if (AccDeniedCnt>0)
		msgbox_OK(ErrMesage.sprintf(_T("�x��: %u�̃f�B���N�g���ŃA�N�Z�X�����ۂ���܂����B"), AccDeniedCnt));

	InfoListBox->SetFocus();
	cursor_Default();
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("FileExtFileHdrW0"),	FileListHeader->Sections->Items[0]->Width);

	IniFile->WriteIntGen(_T("FileExtSortModeX"),	ExtSortMode);
	IniFile->WriteIntGen(_T("FileExtSortOdrX0"),	ExtSortOdr[0]);
	IniFile->WriteIntGen(_T("FileExtSortOdrX1"),	ExtSortOdr[1]);
	IniFile->WriteIntGen(_T("FileExtSortOdrX2"),	ExtSortOdr[2]);
	IniFile->WriteIntGen(_T("FileExtSortOdrX3"),	ExtSortOdr[3]);

	IniFile->WriteIntGen(_T("FileExtSortModeF"),	FileSortMode);
	IniFile->WriteIntGen(_T("FileExtSortOdrF0"),	FileSortOdr[0]);
	IniFile->WriteIntGen(_T("FileExtSortOdrF1"),	FileSortOdr[1]);

	IniFile->WriteIntGen(_T("FileExtListHeight"),	FilePanel->Height);

	InfoListBox->Count = 0;
	for (int i=0; i<FextInfoList->Count; i++) delete (ext_inf_rec*)FextInfoList->Objects[i];
	FextInfoList->Clear();

	FileListBox->Count = 0;
	FileItemList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormDestroy(TObject *Sender)
{
	for (int i=0; i<FextInfoList->Count; i++) delete (ext_inf_rec*)FextInfoList->Objects[i];
	delete FextInfoList;

	delete FileItemList;

	delete FileScrPanel;
	delete InfoScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormResize(TObject *Sender)
{
	set_HeaderSecWidth(FextInfHeader, 4, FExtSctWd, FCntSctWd, SizeSctWd + PercSctWd + 110, SizeSctWd + 8);
	FileListHeader->Sections->Items[1]->Width = ClientWidth - FileListHeader->Sections->Items[0]->Width;
	FileListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::ListSplitterMoved(TObject *Sender)
{
	InfoScrPanel->UpdateKnob();
	FileScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//�g���q�ꗗ���擾
//  �߂�l: false=�L�����Z�����ꂽ
//---------------------------------------------------------------------------
bool __fastcall TFileExtensionDlg::GetInfo(UnicodeString pnam)
{
	bool res = true;

	pnam = IncludeTrailingPathDelimiter(pnam);
	UnicodeString snam = pnam;  split_tkn(snam, PathName);

	TSearchRec sr;
	int err = FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr);
	if (err==0) {
		try {
			do {
				UnicodeString fext = ((sr.Attr & faDirectory)==0)? get_extension(sr.Name).LowerCase() : EmptyStr;
				//�T�u�f�B���N�g��������
				if (sr.Attr & faDirectory) {
					if (!ContainsStr("..", sr.Name) && !GetInfo(pnam + sr.Name)) Abort();
				}
				//���C�u����
				else if (snam.IsEmpty() && test_LibExt(fext)) {
					std::unique_ptr<TStringList> lib(new TStringList());
					get_LibraryList(pnam + sr.Name, lib.get());
					for (int i=0; i<lib->Count; i++) if (!GetInfo(lib->Strings[i])) Abort();
				}
				//�t�@�C��
				else {
					if (fext.IsEmpty()) fext = ".";
					int idx = FextInfoList->IndexOf(fext);
					//����
					if (idx!=-1) {
						ext_inf_rec *ip = (ext_inf_rec*)FextInfoList->Objects[idx];
						ip->f_count++;
						ip->f_size += sr.Size;
						if (ClusterSize>0) {
							__int64 clu_n = sr.Size/ClusterSize + ((sr.Size%ClusterSize>0)? 1 : 0);
							ip->oc_size += (clu_n * ClusterSize);
						}

						if (ip->f_count<=MAX_FLIST_CNT)
							ip->f_names.cat_sprintf(_T("%s%s\r\n"), snam.c_str(), sr.Name.c_str());
					}
					//�V�K
					else {
						ext_inf_rec *ip = new ext_inf_rec;
						ip->f_count = 1;
						ip->f_size  = sr.Size;
						if (ClusterSize>0) {
							__int64 clu_n = sr.Size/ClusterSize + ((sr.Size%ClusterSize>0)? 1 : 0);
							ip->oc_size = (clu_n * ClusterSize);
						}
						ip->f_names.sprintf(_T("%s%s\r\n"), snam.c_str(), sr.Name.c_str());

						FextInfoList->AddObject(fext, (TObject*)ip);
						//�g���q���\���X�V
						FextInfBar->Panels->Items[0]->Text = FextInfoList->Count;
					}
					TotalFileCount++;
					TotalSize += sr.Size;
				}
				//�t�@�C�����\���X�V
				if (TotalFileCount%256==255) {
					FextInfBar->Panels->Items[1]->Text = get_size_str_B(TotalFileCount, 0);
					FextInfBar->Repaint();
				}

				if (is_KeyPress_ESC()) Abort();
			} while(FindNext(sr)==0);
			res = true;
		}
		catch (...) {
			res = false;
		}

		FindClose(sr);
	}
	else {
		if (err==ERROR_ACCESS_DENIED) AccDeniedCnt++;
	}

	return res;
}

//---------------------------------------------------------------------------
//���ʈꗗ���擾
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::GetResult(TStringList *lst, int mode)
{
	//�g���q�ꗗ
	if (InfoListBox->Focused()) {
		int max_fext = 6;
		for (int i=0; i<FextInfoList->Count; i++) max_fext = std::max(max_fext, FextInfoList->Strings[i].Length());

		UnicodeString hr_str = make_RuledLine(5, max_fext - 1, 9, 11, 6, 13);
		UnicodeString lbuf;

		switch (mode) {
		case 1:	//CSV
			lst->Add("\"�g���q\",\"�t�@�C����\",\"���v�T�C�Y\",\"���σT�C�Y\"");
			break;
		case 2:	//TSV
			lst->Add("�g���q\t�t�@�C����\t���v�T�C�Y\t���σT�C�Y");
			break;
		default:
			lst->Add(PathName);
			lst->Add(lbuf.sprintf(_T("�g���q%*s�t�@�C����   �T�C�Y    �䗦      ����"), max_fext - 6, _T(" ")));
			lst->Add(hr_str);
		}
		//����
		for (int i=0; i<FextInfoList->Count; i++) {
			UnicodeString fext = FextInfoList->Strings[i];
			if (fext.IsEmpty()) fext = ".";
			ext_inf_rec *ip = (ext_inf_rec*)FextInfoList->Objects[i];
			switch (mode) {
			case 1:	//CSV
				lst->Add(lbuf.sprintf(_T("\"%s\",%u,%llu,%llu"), fext.c_str(), ip->f_count, ip->f_size, ip->av_size));
				break;
			case 2:	//TSV
				lst->Add(lbuf.sprintf(_T("%s\t%u\t%llu\t%llu"),  fext.c_str(), ip->f_count, ip->f_size, ip->av_size));
				break;
			default:
				lst->Add(lbuf.sprintf(_T("%-*s  %s  %s %5.1f%%  %s"),
						max_fext, fext.c_str(),
						get_size_str_B(ip->f_count, 7).c_str(),
						get_size_str_G(ip->f_size, 10, SizeDecDigits).c_str(),
						(TotalSize>0)? 100.0*ip->f_size/TotalSize : 0.0,
						get_size_str_G(ip->av_size, 10, SizeDecDigits).c_str()));
			}
		}
		//�v
		if (mode==0) {
			lst->Add(hr_str);
			lst->Add(lbuf.sprintf(_T("%-*u  %s  %s         %s"),
				max_fext, FextInfoList->Count,
				get_size_str_B(TotalFileCount, 7).c_str(),
				get_size_str_G(TotalSize,   10, SizeDecDigits).c_str(),
				get_size_str_G(AverageSize, 10, SizeDecDigits).c_str()));
			//�x��
			if (!ErrMesage.IsEmpty()) lst->Add(ErrMesage);
		}
	}
	//�t�@�C���ꗗ
	else if (FileListBox->Focused()) {
		lst->Assign(FileItemList);
		lst->Insert(0, PathName);
	}
}

//---------------------------------------------------------------------------
//�ꗗ�̃\�[�g
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortListX(int mode)
{
	if (mode!=-1) ExtSortMode = mode;

	switch (ExtSortMode) {
	case 1:  FextInfoList->CustomSort(FextComp_Count); break;
	case 2:  FextInfoList->CustomSort(FextComp_Size);  break;
	case 3:  FextInfoList->CustomSort(FextComp_Ave);   break;
	default: FextInfoList->CustomSort(FextComp_Ext);
	}

	FextInfHeader->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortListF(int mode)
{
	if (mode!=-1) FileSortMode = mode;

	switch (FileSortMode) {
	case 0:  FileItemList->CustomSort(FextComp_Name); break;
	default: FileItemList->CustomSort(FextComp_Path);
	}

	FileListHeader->Repaint();
}

//---------------------------------------------------------------------------
//�������T�[�`
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::IniSearchList(TListBox *lp, UnicodeString ptn)
{
	TStringList *lst = (lp->Tag==0)? FextInfoList : FileItemList;
	TRegExOptions opt; opt << roIgnoreCase;
	int idx = lp->ItemIndex;
	int idx0=-1, idx1=-1;
	for (int i=0; i<lst->Count && idx1==-1; i++) {
		if (i<=idx && idx0!=-1) continue;
		UnicodeString lbuf = (lp->Tag==0) ? get_tkn_r(lst->Strings[i], '.')
										  : ExtractFileName(lst->Strings[i]);
		if (TRegEx::IsMatch(lbuf, ptn, opt)) ((i<=idx)? idx0 : idx1) = i;
	}
	idx = (idx1!=-1)? idx1 : idx0;
	if (idx!=-1) {
		lp->ItemIndex = idx;
		lp->OnClick(lp);
	}
}

//---------------------------------------------------------------------------
//�J�[�\���ʒu�̃t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString __fastcall TFileExtensionDlg::GetCurFileItem(int idx)
{
	UnicodeString fnam;
	if (idx==-1) idx = FileListBox->ItemIndex;
	if (idx>=0 && idx<FileItemList->Count) {
		fnam = FileItemList->Strings[idx];
		if (!fnam.IsEmpty()) fnam = PathName + fnam;
	}
	return fnam;
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextInfBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
		const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect) ;

	UnicodeString lbuf = Panel->Text;
	int xp = ((Panel->Index==2)? Rect.Left + SizeSctWd - ScaledIntX(4) : Rect.Right) - cv->TextWidth(lbuf) - ScaledIntX(4);
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(xp, Rect.Top, lbuf);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileInfBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	draw_SttBarPanel(StatusBar, Panel, Rect);
}

//---------------------------------------------------------------------------
//�w�b�_�̕`��
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextHeaderDrawSection(THeaderControl *HeaderControl,
		THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	int mk_mode = 0;

	if (HeaderControl->Tag==0) {
		if (Section->Index==ExtSortMode)  mk_mode = ExtSortOdr[Section->Index];
	}
	else {
		if (Section->Index==FileSortMode) mk_mode = FileSortOdr[Section->Index];
	}

	draw_SortHeader(HeaderControl, Section, Rect, mk_mode);
}

//---------------------------------------------------------------------------
//�g���q�ꗗ: �\�[�g�ύX
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextInfHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	int idx = Section->Index;
	if (idx==ExtSortMode)  ExtSortOdr[idx] = -ExtSortOdr[idx];
	SortListX(idx);
	InfoListBox->Invalidate();
	InfoListBoxClick(NULL);
}

//---------------------------------------------------------------------------
//�g���q�ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::InfoListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	if (Index>=FextInfoList->Count) return;

	ext_inf_rec *ip = (ext_inf_rec*)FextInfoList->Objects[Index];

	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	TColor bgcol = is_AltLnBgCol(Index)? col_bgList2 : col_bgList;
	cv->Brush->Color = bgcol;
	cv->FillRect(Rect);

	int xp = Rect.Left + ScaledIntX(4);
	int yp = Rect.Top  + get_TopMargin(cv);

	UnicodeString fext = FextInfoList->Strings[Index];
	//�A�C�R��
	if (IconMode==1) {
		HICON hIcon = get_fext_SmallIcon(fext);
		if (hIcon) ::DrawIconEx(cv->Handle, xp, yp, hIcon, ScaledIntX(16), ScaledIntX(16), 0, NULL, DI_NORMAL);
	}

	//�g���q
	cv->Font->Color = get_ExtColor(fext);
	int i_w = ScaledIntX(IconMode==1? 20 : 0);
	cv->TextOut(xp + i_w, yp, minimize_str(fext, cv, FExtSctWd - i_w, true));
	xp += FExtSctWd - 8;

	//�t�@�C����
	UnicodeString lbuf = get_size_str_B(ip->f_count, 7);
	cv->Font->Color = col_fgList;
	cv->TextOut(xp + FCntSctWd - get_TextWidth(cv, lbuf, is_irreg), yp, lbuf);
	xp += FCntSctWd;

	//���v�T�C�Y
	lbuf = get_size_str_G(ip->f_size, ((SizeFormatMode>0)? 7 : 10), SizeDecDigits, SizeFormatMode);

	float r = (TotalSize>0)? (1.0 * ip->f_size/TotalSize) : 0.0;
	cv->Font->Color = get_SizeColor(ip->f_size, col_fgList);
	cv->TextOut(xp + SizeSctWd - get_TextWidth(cv, lbuf, is_irreg), yp, lbuf);
	cv->Font->Color = col_fgList;
	lbuf.sprintf(_T(" %4.1f%%"), r * 100.0);
	cv->TextOut(xp + SizeSctWd + PercSctWd - get_TextWidth(cv, lbuf, is_irreg), yp, lbuf);
	xp += SizeSctWd + PercSctWd + 8;

	//�O���t
	TRect rc = Rect;
	rc.Left = xp; rc.Right = xp + 100;
	rc.Top += 6; rc.Bottom -= 6;
	draw_BarGraph(cv, rc, r);
	xp += 108;

	//���σT�C�Y
	lbuf = get_size_str_G(ip->av_size, 10, SizeDecDigits);
	cv->Brush->Color = bgcol;
	cv->Font->Color  = get_SizeColor(ip->av_size, col_fgList);
	cv->TextOut(xp + SizeSctWd - get_TextWidth(cv, lbuf, is_irreg), yp, lbuf);

	//�J�[�\��
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::InfoListBoxClick(TObject *Sender)
{
	FileListBox->Clear();

	int idx = InfoListBox->ItemIndex;
	if (idx>=0 && idx<FextInfoList->Count) {
		ext_inf_rec *ip = (ext_inf_rec*)FextInfoList->Objects[idx];
		CurFileCount = ip->f_count;
		FileItemList->Text = ip->f_names;
		SortListF();
		FileListBox->Count = FileItemList->Count + ((ip->f_count>MAX_FLIST_CNT)? 1 : 0);
		FileListBox->ItemIndex = -1;
		UnicodeString msg = FextInfoList->Strings[idx];
		__int64 g_size = ip->oc_size - ip->f_size;
		msg.cat_sprintf(_T("    �t�@�C����:%s  ���v:%s  ��L:%s"),
			get_size_str_B(ip->f_count, 0).c_str(),
			get_size_str_G(ip->f_size, 0, SizeDecDigits).c_str(),
			get_size_str_G(ip->oc_size, 0, SizeDecDigits).c_str());
		if (ip->oc_size>0) {
			msg.cat_sprintf(_T("  �M���b�v:%s(%.1f%%)"),
				get_size_str_G(g_size, 0, SizeDecDigits).c_str(), 100.0 * g_size/ip->oc_size);
		}
		msg.cat_sprintf(_T("  ����:%s"), get_size_str_G(ip->av_size, 0, SizeDecDigits).c_str());
		FileInfBar->Panels->Items[0]->Text = msg;
	}

	FileScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//�g���q�ꗗ: �L�[����
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::InfoListBoxKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V))
		InfoListBoxClick(NULL);
	else if (USAME_TI(cmd_F, "ReturnList"))
		ModalResult = mrCancel;
	else if (is_ToRightOpe(KeyStr, cmd_F) || equal_ENTER(KeyStr))
		FileListBox->SetFocus();
	//�������T�[�`
	else if (is_IniSeaKey(KeyStr))	//KeyStr �ɐ��K�\���p�^�[�����Ԃ�
		IniSearchList(lp, KeyStr);
	//�E�N���b�N���j���[
	else if (contained_wd_i(KeysStr_Popup, KeyStr))
		show_PopupMenu(lp);
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::InfoListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//�C���N�������^���T�[�`�����
	Key = 0;
}

//---------------------------------------------------------------------------
//�t�@�C���ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(ListFont);

	cv->Brush->Color = is_AltLnBgCol(Index)? col_bgList2 : col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + ScaledIntX(4);
	int yp = Rect.Top  + get_TopMargin(cv);

	if (Index==MAX_FLIST_CNT) {
		cv->Font->Color = col_Teal;
		cv->TextOut(xp, yp, UnicodeString().sprintf(_T("�c�� %u Files"), CurFileCount - MAX_FLIST_CNT));
	}
	else {
		int w_fn = FileListHeader->Sections->Items[0]->Width;
		UnicodeString fnam = GetCurFileItem(Index);
		if (IniFile->IsMarked(fnam)) {
			cv->Brush->Color = col_bgMark;
			TRect mrc  = Rect;
			mrc.Left   = xp;
			mrc.Right  = xp + w_fn + 4;
			mrc.Bottom = yp + cv->TextHeight("Q");
			cv->FillRect(mrc);
		}

		cv->Font->Color = col_fgList;

		//�g���q�񕪗�
		if (NoSpaceFExt) {
			cv->TextOut(xp, yp, minimize_str(fnam, cv, w_fn, OmitEndOfName));
		}
		//�g���q����
		else {
			UnicodeString fext = get_extension(fnam);
			int x_wd = std::min(get_TextWidth(cv, get_FExtMaxStr(), is_irreg), get_TextWidth(cv, fext.UpperCase(), is_irreg));
			int x_fx = Rect.Left + w_fn - x_wd - ScaledIntX(8);
			cv->TextOut(xp, yp, minimize_str(get_base_name(fnam), cv, x_fx - xp, OmitEndOfName));
			cv->TextOut(x_fx, yp, minimize_str(fext, cv, x_wd, true));
		}
		xp += w_fn;

		UnicodeString pnam = ExtractFilePath(fnam).Delete(1, PathName.Length());
		if (!pnam.IsEmpty()) {
			TRect rc = Rect;
			if (ScrBarStyle>0) {
				rc.SetWidth(FileScrPanel->ParentPanel->Width);
				if (FileScrPanel->VisibleV) rc.Right -= (FileScrPanel->KnobWidth - 1);
			}
			cv->Font->Color = col_Folder;
			PathNameOut(pnam, cv, xp, yp, rc.Right - xp - ScaledIntX(4));
		}
	}

	//�J�[�\��
	if (lp->Focused() || FileInfoDlg->Visible) draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index<=MAX_FLIST_CNT)? GetCurFileItem(Index) : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxEnter(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	if (lp->Count>0) lp->ItemIndex = 0;
	FileListBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxExit(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	lp->ItemIndex = -1;

	FileInfBar->Panels->Items[0]->Text = EmptyStr;
}

//---------------------------------------------------------------------------
//�t�@�C���ꗗ: �L�[����
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);
	UnicodeString fnam	 = GetCurFileItem();

	TListBox *lp = (TListBox*)Sender;
	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V)) {
		FileListBoxClick(NULL);
	}
	else if (contained_wd_i(_T("Mark|Mark_ND"), cmd_F)) {
		if (!fnam.IsEmpty()) {
			IniFile->FileMark(fnam, -1);
			if (USAME_TI(cmd_F, "Mark")) ListBoxCursorDown(lp);
			lp->Invalidate();
		}
		else beep_Warn();
	}
	else if (USAME_TI(cmd_F, "ReturnList")) {
		ModalResult = mrCancel;
	}
	else if (is_ToLeftOpe(KeyStr, cmd_F)) {
		InfoListBox->SetFocus();
	}
	else if (equal_ENTER(KeyStr)) {
		if (!fnam.IsEmpty()) {
			FileName = fnam;  ModalResult = mrOk;
		}
		else beep_Warn();
	}
	//�������T�[�`
	else if (is_IniSeaKey(KeyStr)) {	//KeyStr �ɐ��K�\���p�^�[�����Ԃ�
		IniSearchList(lp, KeyStr);
	}
	//�t�@�C�����
	else if (StartsText("ShowFileInfo", cmd_F)) {
		ShowFileInfoAction->Execute();
	}
	//�v���p�e�B
	else if (USAME_TI(cmd_F, "PropertyDlg")) {
		PropertyAction->Execute();
	}
	//�E�N���b�N���j���[
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
		show_PopupMenu(lp);
	}
	else return;

	Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListBoxClick(TObject *Sender)
{
	UnicodeString fnam = GetCurFileItem();
	if (!fnam.IsEmpty()) {
		UnicodeString lbuf = get_file_attr_str(file_GetAttr(fnam));
		__int64 f_size = get_file_size(fnam);
		lbuf.cat_sprintf(_T("  %s    �T�C�Y:%s"),
			format_DateTime(get_file_age(fnam)).c_str(), get_size_str_G(f_size, 0, SizeDecDigits).c_str());
		if (ClusterSize>0) {
			__int64 clu_n  = f_size/ClusterSize + ((f_size%ClusterSize>0)? 1 : 0);
			__int64 o_size = clu_n * ClusterSize;
			lbuf.cat_sprintf(_T("  ��L:%s"), get_size_str_G(o_size, 0, SizeDecDigits).c_str());
			if (o_size>0) {
				__int64 g_size = o_size - f_size;
				lbuf.cat_sprintf(_T("  �M���b�v:%s(%.1f%%)"),
					get_size_str_G(g_size, 0, SizeDecDigits).c_str(), 100.0 * g_size/o_size);
			}
		}
		FileInfBar->Panels->Items[0]->Text = lbuf;
	}
	else {
		FileInfBar->Panels->Items[0]->Text = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	FileListHeader->Sections->Items[1]->Width = ClientWidth - FileListHeader->Sections->Items[0]->Width;
	FileListBox->Invalidate();
}

//---------------------------------------------------------------------------
//�\�[�g�ύX
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FileListHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	int idx = Section->Index;
	if (idx==FileSortMode)  FileSortOdr[idx] = -FileSortOdr[idx];
	SortListF(idx);
	FileListBox->Invalidate();
	FileListBoxClick(NULL);
}

//---------------------------------------------------------------------------
//�N���b�v�{�[�h�ɃR�s�[
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextClipCopyActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> r_lst(new TStringList());
	GetResult(r_lst.get());
	copy_to_Clipboard(r_lst->Text);
}
//---------------------------------------------------------------------------
//���O�ɏo��
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextLogOutActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> r_lst(new TStringList());
	GetResult(r_lst.get());
	AddLogStrings(r_lst.get());
}
//---------------------------------------------------------------------------
//�t�@�C���ɕۑ�
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextSaveActionExecute(TObject *Sender)
{
	int mode = ((TAction*)Sender)->Tag;

	UserModule->SaveTxtDlg->Title = LoadUsrMsg(USTR_SaveAs, (mode==1)? _T("�ꗗ(CSV)") : (mode==2)? _T("�ꗗ(TSV)") : _T("�ꗗ"));
	UserModule->SaveTxtDlg->Filter		  = (mode==1)? F_FILTER_CSV : (mode==2)? F_FILTER_TSV : F_FILTER_TXT;
	UserModule->SaveTxtDlg->DefaultExt	  = (mode==1)? "csv" : "txt";
	UserModule->SaveTxtDlg->FileName	  = (mode==1)? "*.csv" : "*.txt";
	UserModule->SaveTxtDlg->InitialDir	  = PathName;
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		GetResult(fbuf.get(), mode);

		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, USTR_List));
	}
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextOutActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FextInfoList->Count>0);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextCsvActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = InfoListBox->Focused();
	ap->Enabled = (FextInfoList->Count>0);
}

//---------------------------------------------------------------------------
//�t�@�C���[�Ŋg���q����
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FextMaskFindActionExecute(TObject *Sender)
{
	int idx = InfoListBox->ItemIndex;
	if (idx>=0 && idx< FextInfoList->Count) {
		FextMask	= "*" + FextInfoList->Strings[idx];
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
//�g���q�ꗗ: �\�[�g
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortFextActionExecute(TObject *Sender)
{
	SortListX(((TAction*)Sender)->Tag);
	InfoListBox->Invalidate();
	InfoListBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortFextActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = InfoListBox->Focused();
	ap->Checked = (ap->Tag==ExtSortMode);
}

//---------------------------------------------------------------------------
//�t�@�C���ꗗ: �\�[�g
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortFileActionExecute(TObject *Sender)
{
	SortListF(((TAction*)Sender)->Tag);
	InfoListBox->Invalidate();
	InfoListBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::SortFileActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = FileListBox->Focused();
	ap->Checked = (ap->Tag==FileSortMode);
}

//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}

//---------------------------------------------------------------------------
//�t�@�C�����
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::ShowFileInfoActionExecute(TObject *Sender)
{
	FileInfoDlg->ShowModalEx(GetCurFileItem());
}
//---------------------------------------------------------------------------
//�t�@�C���̃v���p�e�B
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::PropertyActionExecute(TObject *Sender)
{
	pos_ListBoxItem(FileListBox, FileListHeader->Sections->Items[0]->Width/2);
	ShowPropertyDialog(GetCurFileItem());
}
//---------------------------------------------------------------------------
void __fastcall TFileExtensionDlg::PropertyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = FileListBox->Focused();
	ap->Enabled = !GetCurFileItem().IsEmpty();
}
//---------------------------------------------------------------------------

