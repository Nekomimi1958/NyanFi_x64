//----------------------------------------------------------------------//
// NyanFi																//
//  �t�@�C�����/���ڂ̏W�v												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <Vcl.Clipbrd.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "MainFrm.h"
#include "SubView.h"
#include "FileInfDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFileInfoDlg *FileInfoDlg;

//---------------------------------------------------------------------------
__fastcall TFileInfoDlg::TFileInfoDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormCreate(TObject *Sender)
{
	FileRec    = NULL;
	isAppInfo  = isGitInfo = false;
	isCalcItem = inhNxtPre = inhImgPrv = useImgPrv = false;

	CsvCol	  = -1;
	DataList  = NULL;

	ListScrPanel = new UsrScrollPanel(ListPanel, InfListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	InfListBox->Tag = LBTAG_FIF_LIST | LBTAG_OPT_FIF2 | LBTAG_OPT_ZOOM;
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormShow(TObject *Sender)
{
	CmdStr = EmptyStr;
	useImgPrv = !inhImgPrv && !isCalcItem && !isAppInfo && !isGitInfo;

	if (isCalcItem)
		IniFile->LoadPosInfo(this, DialogCenter, "CalcItem");
	else
		IniFile->LoadPosInfo(this, DialogCenter);

	if (!isAppInfo && !isGitInfo) InfListBox->Clear();

	InfListBox->Color = col_bgInf;
	InfListBox->Font->Assign(FileInfFont);
	set_ListBoxItemHi(InfListBox);
	set_UsrScrPanel(ListScrPanel);

	if (!UpdateInfo()) ::PostMessage(Handle, WM_CLOSE, 0, 0);

	ImgPreviewAction->Checked = useImgPrv? IniFile->ReadBoolGen(_T("FileInfoImgPrv")) : false;
	if (ImgPreviewAction->Checked) {
		if (!SubViewer->Visible) {
			SubViewer->formFileInf = true;
			SubViewer->Show();
		}
		if (FileRec) SubViewer->DrawImage(FileRec->f_name);
		SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (isCalcItem) {
		IniFile->SavePosInfo(this, "CalcItem");
	}
	else {
		IniFile->SavePosInfo(this);
		if (useImgPrv) IniFile->WriteBoolGen(_T("FileInfoImgPrv"), ImgPreviewAction);
		if (SubViewer->Visible) SubViewer->Close();
	}

	FileRec    = NULL;
	isAppInfo  = isGitInfo = false;
	isCalcItem = inhNxtPre = inhImgPrv = useImgPrv = false;
	CsvCol	   = -1;
	DataList   = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormResize(TObject *Sender)
{
	if (isCalcItem) InfListBox->Invalidate();
}

//---------------------------------------------------------------------------
//�w��t�@�C���̏��\�� (�O��ړ��֎~)
//---------------------------------------------------------------------------
int __fastcall TFileInfoDlg::ShowModalEx(UnicodeString fnam)
{
	int res = mrNone;
	if (file_exists(fnam)) {
		file_rec *cfp = cre_new_file_rec(fnam);
		FileRec   = cfp;
		inhImgPrv = !test_FileExt(cfp->f_ext, FEXT_IMAGE + get_img_fext() + ".mp3.flac" + FEXT_INDIVICO);
		inhNxtPre = true;
		res = ShowModal();
		del_file_rec(cfp);
	}

	return res;
}

//---------------------------------------------------------------------------
//�ꗗ�̍X�V
//---------------------------------------------------------------------------
int __fastcall comp_ObjLD(TStringList *List, int Index1, int Index2)
{
	long double *v0 = (long double*)List->Objects[Index1];
	long double *v1 = (long double*)List->Objects[Index2];
	if (!v0 || !v1)	return 0;

	if (*v0==*v1)	return 0;
	return (*v0 > *v1)? 1 : -1;
}

//---------------------------------------------------------------------------
bool __fastcall TFileInfoDlg::UpdateInfo()
{
	if (isAppInfo) {
		set_FormTitle(this, _T("�A�v���P�[�V�������"));
		ListScrPanel->UpdateKnob();
		return true;
	}

	if (isGitInfo) {
		set_FormTitle(this, _T("�R�~�b�g���"));
		ListScrPanel->UpdateKnob();
		return true;
	}

	TListBox *lp = InfListBox;

	//���ڂ̏W�v
	if (isCalcItem) {
		try {
			if (!DataList) Abort();
			UnicodeString top_str = DataList->Strings[0];
			bool IsCSV = !contains_s(top_str, _T('\t'));
			set_FormTitle(this, IsCSV? _T("CSV���ڂ̏W�v") : _T("TSV���ڂ̏W�v"));
			TStringDynArray top_buf = IsCSV? get_csv_array(top_str, 99) : split_strings_tab(top_str);
			if (CsvCol==-1) CsvCol = 0;
			if (CsvCol>=top_buf.Length) Abort();

			//���ږ�
			UnicodeString i_nam;
			if (TopIsHeader)
				i_nam = IsCSV? get_csv_item(top_str, CsvCol) : get_tsv_item(top_str, CsvCol);
			else
				i_nam.sprintf(_T("����%u"), CsvCol + 1);

			long double total = 0.0;
			long double i_max = 0, i_min = 0;
			int f_w = 0;	//�������̕\������
			std::unique_ptr<TStringList> s_lst(new TStringList());	//�L���Ȑ��l���ڂ̃��X�g
			for (int i=(TopIsHeader? 1 : 0); i<DataList->Count; i++) {
				UnicodeString lbuf = DataList->Strings[i];
				UnicodeString istr = extract_top_num_str(IsCSV? get_csv_item(lbuf, CsvCol) : get_tsv_item(lbuf, CsvCol));
				if (istr.IsEmpty()) continue;
				try {
					long double r = istr.ToDouble();
					total += r;
					if (s_lst->Count==0) {
						i_min = i_max = r;
					}
					else {
						i_min = std::min(i_min, r);
						i_max = std::max(i_max, r);
					}

					long double *dp = new long double;
					*dp = r;
					s_lst->AddObject(istr, (TObject*)dp);

					int p = istr.Pos('.');
					if (p) f_w = std::max(f_w, istr.Length() - p);
				}
				catch (...) {
					;
				}
			}

			int i_cnt = s_lst->Count;
			if (i_cnt==0) Abort();
	
			//���ϒl
			long double i_ave = total/i_cnt;
			//�����l
			s_lst->CustomSort(comp_ObjLD);
			long double i_med = (i_cnt%2==1)?
		 		*(long double*)s_lst->Objects[(i_cnt - 1) / 2] :
				(*(long double*)s_lst->Objects[i_cnt/2 - 1] + *(long double*)s_lst->Objects[i_cnt/2]) /2;
			//���U
			long double sum = 0;
			for (int i=0; i<s_lst->Count; i++) {
				long double v = *(long double*)s_lst->Objects[i];
				sum += (v - i_ave) * (v - i_ave);
			}
			long double i_var = sum/i_cnt;
			//�W���΍�
			long double i_std = sqrtl(i_var);

			//�K����(�X�^�[�W�F�X�̌���)
			int k = 1 + (int)(log10l(i_cnt)/log10l(2) + 0.5);
			//�K����
			long double d = (i_max - i_min)/k;
			//�x�����z
			std::unique_ptr<int[]> freq(new int[k]);
			std::unique_ptr<TStringList> f_lst(new TStringList());
			for (int i = 0; i < k; i++) freq[i] = 0;
			for (int i=0; i<i_cnt; i++) {
				long double v = *(long double*)s_lst->Objects[i];
				for (int j=0; j<k; j++) {
					long double lmt = i_min + d * (j + 1);
					if (f_w==0) lmt = floorl(lmt + 0.5);
					if ((j==(k - 1) && v <=i_max) || (v < lmt)) {
						freq[j]++;	break;
					}
				}
			}

			MaxColWd0 = MaxColWd1 = MaxColWd2 = 0;;

			TCanvas *cv = lp->Canvas;
			cv->Font->Assign(FileInfFont);
			bool is_irreg = IsIrregularFont(cv->Font);

			Max_freq = 0;
			for (int i=0; i<k; i++) Max_freq = std::max(Max_freq, freq[i]);
			if (Max_freq>0) {
				int f_sum = 0;
				for (int i=0; i<k; i++) {
					UnicodeString s0, s1, s2;
					//�K�������l
					long double lmt = i_min + d * i;
					if (f_w==0) lmt = floorl(lmt + 0.5);
					s0.sprintf(_T("%s�`"), ldouble_to_str(lmt, f_w).c_str());
					//�x��
					s1 = IntToStr(freq[i]);
					f_sum += freq[i];
					//�ݐϑ��Γx��
					s2.sprintf(_T("%.3f"), 1.0*f_sum/i_cnt);
					f_lst->Add(s0 + "\t" + s1 + "\t" + s2);
					MaxColWd0 = std::max(MaxColWd0, get_TextWidth(cv, s0, is_irreg));
					MaxColWd1 = std::max(MaxColWd1, get_TextWidth(cv, s1, is_irreg));
					MaxColWd2 = std::max(MaxColWd2, get_TextWidth(cv, s2, is_irreg));
				}
			}

			//�l�̉��
			for (int i=0; i<s_lst->Count; i++) delete (long double*)s_lst->Objects[i];

			//���ʕ\��
			std::unique_ptr<TStringList> i_lst(new TStringList());
			UnicodeString tmp;
			FreqIndex = 0;
			add_PropLine(_T("���ږ�"),		i_nam,	i_lst.get());
			i_lst->Add(EmptyStr);
			add_PropLine(_T("�L�����ڐ�"),	i_cnt,	i_lst.get());
			add_PropLine(_T("���v�l"),		ldouble_to_str(total, f_w),	i_lst.get());
			add_PropLine(_T("�ŏ��l"),		ldouble_to_str(i_min, f_w),	i_lst.get());
			add_PropLine(_T("�ő�l"),		ldouble_to_str(i_max, f_w),	i_lst.get());
			i_lst->Add(EmptyStr);
			add_PropLine(_T("���ϒl"),		ldouble_to_str(i_ave, f_w + 1),	i_lst.get());
			add_PropLine(_T("�����l"),		ldouble_to_str(i_med, f_w),		i_lst.get());
			add_PropLine(_T("���U"),		ldouble_to_str(i_var, f_w + 1),	i_lst.get());
			add_PropLine(_T("�W���΍�"),	ldouble_to_str(i_std, f_w + 1),	i_lst.get());
			if (f_lst->Count>0) {
				i_lst->Add(EmptyStr);
				FreqIndex = i_lst->Add(_T("�x�����z"));
				i_lst->AddStrings(f_lst.get());
			}

			assign_InfListBox(lp, i_lst.get(), ListScrPanel);
			return true;
		}
		catch (...) {
			msgbox_ERR("�L���Ȑ��l���ڂ�����܂���");
			return false;
		}
	}

	//�t�@�C�����
	if (!FileRec) FileRec = NyanFiForm->GetCurFrecPtr(false, true);
	if (FileRec) {
		cursor_HourGlass();

		Caption = UnicodeString().sprintf(_T("%s �̏��"), (FileRec->is_up? FileRec->p_name : FileRec->n_name).c_str());

		if (!FileRec->is_dir && FileRec->is_virtual && !FileRec->failed) FileRec->failed = !SetTmpFile(FileRec);
		if (!FileRec->is_dummy && FileRec->inf_list->Count==0) GetFileInfList(FileRec, true);

		std::unique_ptr<TStringList> i_lst(new TStringList());
		i_lst->Assign(FileRec->inf_list);

		if (i_lst->Count>=3) {
			int idx = 0;
			if (!FileRec->is_up) {
				i_lst->Strings[idx] = make_PropLine(FileRec->is_dir? _T("�f�B���N�g����") : _T("�t�@�C����"), i_lst->Strings[idx]);
				i_lst->Objects[idx] = (TObject*)LBFLG_FILE_FIF;
				idx++;
				i_lst->Strings[idx] = make_PropLine(_T("�ꏊ"), i_lst->Strings[idx]);
				i_lst->Objects[idx] = (TObject*)LBFLG_PATH_FIF;
			}
			else {
				i_lst->Delete(0);
				i_lst->Strings[idx] = make_PropLine(_T("�f�B���N�g����"), i_lst->Strings[idx]);
				i_lst->Objects[idx] = (TObject*)LBFLG_FILE_FIF;
			}
			idx++;
			i_lst->Strings[idx] = make_PropLine(FileRec->is_ftp? "�p�[�~�b�V����" : "����", FileRec->attr_str);
			i_lst->Objects[idx] = (TObject*)LBFLG_ATTR_FIF;
			idx++;

			//�t�@�C���T�C�Y
			if (!FileRec->is_dir) {
				//�T�C�Y
				if (FileRec->f_size>=0) {
					i_lst->Insert(idx++,
						get_PropTitle(_T("�T�C�Y")).cat_sprintf(_T("%s (%s)"),
							get_size_str_G(FileRec->f_size, 10, SizeDecDigits, 1).c_str(), 
							get_size_str_B(FileRec->f_size, 5).c_str()));
				}
				//��L�T�C�Y
				if (!FileRec->is_virtual && !FileRec->is_ftp) {
					int clu_sz = get_ClusterSize(FileRec->p_name);	//�N���X�^�T�C�Y
					if (clu_sz>0) {
						__int64 o_size;
						if (FileRec->is_dir)
							o_size = FileRec->o_size;
						else {
							__int64 clu_n = FileRec->f_size/clu_sz;
							if (FileRec->f_size%clu_sz>0) clu_n++;
							o_size = (clu_n * clu_sz);
						}
						if (o_size>=0) {
							i_lst->Insert(idx++,
								get_PropTitle(_T("��L�T�C�Y")).cat_sprintf(_T("%s (%s)"),
									get_size_str_G(o_size, 10, SizeDecDigits, 1).c_str(), 
									get_size_str_B(o_size, 5).c_str()));
						}
					}
				}
			}

			//����
			i_lst->InsertObject(idx++,
				make_PropLine(_T("�X�V����"), format_DateTime(FileRec->f_time)), (TObject*)LBFLG_TIME_FIF);

			if (!FileRec->is_virtual && !FileRec->is_ftp) {
				HANDLE hFile = ::CreateFile(cv_ex_filename(FileRec->f_name).c_str(),
					GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
				if (hFile!=INVALID_HANDLE_VALUE) {
					FILETIME c_tm, a_tm, w_tm, f_tm;
					if (::GetFileTime(hFile, &c_tm, &a_tm, &w_tm)) {
						i_lst->InsertObject(idx++,
							make_PropLine(_T("�쐬����"), format_DateTime(utc_to_DateTime(&c_tm))),
							(TObject*)LBFLG_TIME_FIF);
						i_lst->InsertObject(idx++,
							make_PropLine(_T("�A�N�Z�X����"), format_DateTime(utc_to_DateTime(&a_tm))),
							(TObject*)LBFLG_TIME_FIF);
					}
					::CloseHandle(hFile);
				}
			}
			i_lst->Insert(idx, EmptyStr);
		}

		assign_InfListBox(lp, i_lst.get());
		cursor_Default();
	}
	else {
		set_FormTitle(this, _T("�t�@�C�����"));
	}

	ListScrPanel->UpdateKnob();
	return true;
}

//---------------------------------------------------------------------------
//�ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::InfListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;

	//�x�����z
	if (isCalcItem && FreqIndex>0 && Index>=FreqIndex) {
		TCanvas *cv = lp->Canvas;
		cv->Font->Assign(FileInfFont);
		bool is_irreg = IsIrregularFont(cv->Font);

		cv->Brush->Color = (State.Contains(odSelected) && lp->Focused())? col_selItem : col_bgInf;
		cv->FillRect(Rect);

		int xp = Rect.Left + Scaled8;
		int yp = Rect.Top + 1;
		bool use_fgsel = lp->Focused() && is_SelFgCol(State);

		TStringDynArray itm_buf = split_strings_tab(lp->Items->Strings[Index]);
		if (itm_buf.Length==1) {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgInfNam;
			cv->TextOut(xp, yp, itm_buf[0]);
		}
		else if (itm_buf.Length==3) {
			//�K��
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgInf;
			cv->TextOut(xp + MaxColWd0 - get_TextWidth(cv, itm_buf[0], is_irreg), yp, itm_buf[0]);
			xp += MaxColWd0 + Scaled8;
			//��؂��
			cv->Pen->Style = psSolid;
			cv->Pen->Width = 1;
			cv->Pen->Color = col_HR;
			cv->MoveTo(xp, Rect.Top);  cv->LineTo(xp, Rect.Bottom);
			//�x��
			xp += Scaled8;
			int n = itm_buf[1].ToIntDef(0);
			cv->TextOut(xp + MaxColWd1 - get_TextWidth(cv, itm_buf[1], is_irreg), yp, itm_buf[1]);
			xp += MaxColWd1 + Scaled4;
			cv->MoveTo(xp, Rect.Top);  cv->LineTo(xp, Rect.Bottom);
			//�ݐϑ��Γx��
			xp += Scaled8;
			cv->TextOut(xp + MaxColWd2 - get_TextWidth(cv, itm_buf[2], is_irreg), yp, itm_buf[2]);
			xp += MaxColWd2 + Scaled4;

			//�O���t
			TRect rc = Rect;
			rc.Left  = xp;
			if (ScrBarStyle>0) {
				rc.Right = ListPanel->ClientWidth;
				if (ListScrPanel->VisibleV) rc.Right -= ListScrPanel->KnobWidth;
			}
			rc.Right -= 2;
			cv->Brush->Color = col_bgPrgBar;
			cv->FillRect(rc);
			if (Max_freq>0) {
				rc.Right = rc.Left + rc.Width() * (0.9 * n / Max_freq);
				cv->Brush->Color = col_fgPrgBar;	cv->FillRect(rc);
				cv->Brush->Color = scl_WindowText;	cv->FrameRect(rc);
			}
		}
	}
	//���
	else {
		draw_InfListBox(lp, Rect, Index, State);
	}

	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::InfListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);
	TListBox *lp = (TListBox*)Sender;

	bool handled = true;
	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V))
		;
	//�O��̃t�@�C���֐؂�ւ�
	else if (FileRec && !FileRec->is_ftp && !inhNxtPre && contained_wd_i(_T("PrevFile|NextFile"), cmd_V)) {
		CmdStr = cmd_V;
		//��ʂ��������Ɏc��
		this->Perform(WM_SETREDRAW, 0, (NativeInt)0);
		if (SubViewer->Visible) SubViewer->Perform(WM_SETREDRAW, 0, (NativeInt)0);
		ModalResult = mrRetry;
	}
	else if (USAME_TI(cmd_F, "PropertyDlg"))		PropertyDlgAction->Execute();
	else if (USAME_TI(cmd_F, "ShowPreview") || USAME_TI(cmd_V, "ImgPreview"))
													ImgPreviewAction->Execute();
	else if (SameText(KeyStr, KeyStr_Copy))			CopyAction->Execute();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	else if	(equal_ESC(KeyStr))						ModalResult = mrOk;
	else handled = false;

	if (handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::InfListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//�C���N�������^���T�[�`�����
	if (_istalnum(Key) || Key==VK_SPACE) Key = 0;
}

//---------------------------------------------------------------------------
//�t�@�C�������R�s�[
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::CopyInfoActionExecute(TObject *Sender)
{
	copy_to_Clipboard(InfListBox->Items->Text);
}
//---------------------------------------------------------------------------
//���ڂ̒l���R�s�[
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::CopyValueActionExecute(TObject *Sender)
{
	TListBox *lp = InfListBox;
	if (lp->ItemIndex!=-1) {
		copy_to_Clipboard(Trim(get_tkn_r(lp->Items->Strings[lp->ItemIndex], ": ")));
	}
}

//---------------------------------------------------------------------------
//���̍��ړ��e�������\��
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::EmpItemActionExecute(TObject *Sender)
{
	TListBox *lp = InfListBox;
	UnicodeString inam = Trim(get_tkn(lp->Items->Strings[lp->ItemIndex], ':'));
	if (!inam.IsEmpty()) {
		std::unique_ptr<TStringList> n_lst(new TStringList());
		n_lst->Delimiter = '|';
		n_lst->QuoteChar = '\0';
		n_lst->DelimitedText = EmpInfItems;
		int idx = n_lst->IndexOf(inam);
		if (idx!=-1) n_lst->Delete(idx); else n_lst->Add(inam);
		EmpInfItems = n_lst->DelimitedText;
		lp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::EmpItemActionUpdate(TObject *Sender)
{
	TAction *ap  = (TAction *)Sender;
	TListBox *lp = InfListBox;
	int idx = lp->ItemIndex;
	int flag = (idx!=-1)? (int)lp->Items->Objects[idx] : 0;
	UnicodeString inam = (idx!=-1 && (flag & LBFLG_STD_FINF)==0)?
							Trim(get_tkn(lp->Items->Strings[idx], ':')) : EmptyStr;
	ap->Enabled = !inam.IsEmpty();
	ap->Checked = test_word_i(inam, EmpInfItems);
}

//---------------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::CopyActionExecute(TObject *Sender)
{
	ExeCmdListBox(InfListBox, _T("ClipCopy"));
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (InfListBox->SelCount>0);
}
//---------------------------------------------------------------------------
//���ׂđI��
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::SelAllActionExecute(TObject *Sender)
{
	ListBoxSelectAll(InfListBox);
}
//---------------------------------------------------------------------------
//URL���J��
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::OpenUrlActionExecute(TObject *Sender)
{
	Execute_ex(ListBoxGetURL(InfListBox));
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::OpenUrlActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !ListBoxGetURL(InfListBox).IsEmpty();
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::InfListBoxDblClick(TObject *Sender)
{
	OpenUrlAction->Execute();
}

//---------------------------------------------------------------------------
//�v���p�e�B
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::PropertyDlgActionExecute(TObject *Sender)
{
	ShowPropertyDialog(FileRec->f_name);
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::PropertyDlgActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (FileRec && !FileRec->is_virtual);
}
//---------------------------------------------------------------------------
//�C���[�W�v���r���[
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::ImgPreviewActionExecute(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Checked = !ap->Checked;

	if (ap->Checked) {
		if (!SubViewer->Visible) {
			SubViewer->formFileInf = true;
			SubViewer->Show();
		}
		if (FileRec) SubViewer->DrawImage(FileRec->f_name);
	}
	else {
		if (SubViewer->Visible) SubViewer->Close();
	}

	SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::ImgPreviewActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = useImgPrv;
	ap->Enabled = ap->Visible && FileRec;
	ap->Checked = ap->Enabled && SubViewer->Visible;
}

//---------------------------------------------------------------------------
void __fastcall TFileInfoDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (isCalcItem)
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_TV) _T("#CsvCalc"));
	else
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#ShowFileInfo"));
}
//---------------------------------------------------------------------------

