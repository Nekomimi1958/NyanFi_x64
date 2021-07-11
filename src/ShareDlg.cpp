//----------------------------------------------------------------------//
// NyanFi																//
//  ���L�t�H���_/�T�u�f�B���N�g���I��/���C�u�����ꗗ					//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "Global.h"
#include "MainFrm.h"
#include "ShareDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNetShareDlg *NetShareDlg;

//---------------------------------------------------------------------------
__fastcall TNetShareDlg::TNetShareDlg(TComponent* Owner)
	: TForm(Owner)
{
	ComputerName = PathName = EmptyStr;
	rqRetPath = false;
	isShare = isSelDir = isSelSub = isLibrary = false;
	isPC = false;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, ShareListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::FormShow(TObject *Sender)
{
	isPC = false;

	if (isSelDir)
		IniFile->LoadPosInfo(this, DialogCenter, isSelSub? "SelSub" : "SelDir");
	else if (isLibrary)
		IniFile->LoadPosInfo(this, DialogCenter, "Library");
	else
		IniFile->LoadPosInfo(this, DialogCenter);

	//��������
	TControl *cp = (TControl *)((CurListTag==0)? NyanFiForm->L_Panel : NyanFiForm->R_Panel);
	TPoint p = cp->ClientToScreen(cp->ClientRect.CenteredRect(BoundsRect).Location);
	Left = p.x;
	Top  = p.y;

	//�͂ݏo���C��
	cp = (TControl *)NyanFiForm->ListPanel;
	TPoint p0 = cp->ClientToScreen(Point(0, 0));
	TPoint p1 = cp->ClientToScreen(Point(0, cp->Height));
	if (Top < p0.y)			 	Top  = p0.y;
	if ((Top + Height) > p1.y)	Top  = p1.y - Height;
	if (Left < p0.x)			Left = p0.x;

	set_StdListBox(ShareListBox);
	set_UsrScrPanel(ListScrPanel);

	//�p���������X�g
	PathTabControl->Visible = false;
	PathTabControl->Height = get_FontHeight(PathTabControl->Font, 8, 8) + 2;
	if (isSelDir && isSelSub) {
		PathTabControl->Tabs->Clear();
		PathTabControl->Style = IsDarkMode? tsButtons : tsFlatButtons;
		if (!StartsStr("\\\\", PathName)) PathTabControl->Tabs->Add("PC");
		TStringDynArray plst = split_path(PathName);
		for (int i=0; i<plst.Length; i++) PathTabControl->Tabs->Add(plst[i]);
		PathTabControl->Visible  = true;
		PathTabControl->TabIndex = PathTabControl->Tabs->Count - 1;
	}

	SetDarkWinTheme(this);
	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::WmFormShowed(TMessage &msg)
{
	PathTabControl->Repaint();

	//�f�B���N�g���̑I��
	if (isSelDir) {
		PathName = IncludeTrailingPathDelimiter(PathName);
		UpdatePathList(PathName);
	}
	//���C�u����
	else if (isLibrary) {
		set_FormTitle(this, _T("���C�u����"));
		std::unique_ptr<TStringList> l_lst(new TStringList());
		get_files(LibraryPath, _T("*.library-ms"), l_lst.get());
		ShareListBox->Items->Assign(l_lst.get());
		ListBoxSetIndex(ShareListBox, 0);
	}
	//���L�t�H���_
	else if (isShare) {
		if (!ComputerName.IsEmpty() && !StartsStr("\\\\", ComputerName)) ComputerName.Insert("\\\\", 1);
		ComputerName = ExcludeTrailingPathDelimiter(ComputerName);
		UpdateShareList(ComputerName);
	}

	ListScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (isSelDir)
		IniFile->SavePosInfo(this, isSelSub? "SelSub" : "SelDir");
	else if (isLibrary)
		IniFile->SavePosInfo(this, "Library");
	else
		IniFile->SavePosInfo(this);

	rqRetPath = false;
	isShare = isSelDir = isSelSub = isLibrary = false;
	isPC = false;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
//���L�t�H���_�ꗗ�̎擾
//---------------------------------------------------------------------------
NET_API_STATUS __fastcall TNetShareDlg::GetShareList(
	UnicodeString cnam,		//\\�R���s���[�^��
	TListBox *lp)
{
	lp->Clear();
	static PSHARE_INFO_1 p_si = NULL;
	DWORD entry_cnt, total_cnt;
	cursor_HourGlass();
	NET_API_STATUS res = ::NetShareEnum(cnam.c_str(), 1, (LPBYTE *)&p_si, MAX_PREFERRED_LENGTH, &entry_cnt, &total_cnt, NULL);
	if (res==NERR_Success) {
		for (DWORD i=0; i<entry_cnt; i++) {
			if (p_si[i].shi1_type==STYPE_DISKTREE) {
				UnicodeString nnam = p_si[i].shi1_netname;
				if (!EndsStr("$", nnam)) lp->Items->Add(nnam);
			}
		}
	}
	if (p_si) ::NetApiBufferFree(p_si);
	cursor_Default();
	return res;
}
//---------------------------------------------------------------------------
//���L�t�H���_�ꗗ�̍X�V
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::UpdateShareList(
	UnicodeString cnam)	//\\�R���s���[�^��
{
	cnam = ExcludeTrailingPathDelimiter(cnam);
	Caption = yen_to_delimiter(cnam) + " - ���L�t�H���_�ꗗ";

	TListBox *lp = ShareListBox;
	NET_API_STATUS res = GetShareList(cnam, lp);
	if (res!=NERR_Success) {
		//�R���s���[�^�ɐڑ�
		NETRESOURCEW nr = {};
		nr.dwType		= RESOURCETYPE_ANY;
		nr.lpRemoteName = (LPWSTR)cnam.c_str();
		DWORD n_res = WNetAddConnection3W(this->Handle, &nr, NULL, NULL, CONNECT_INTERACTIVE);
		if (n_res==NO_ERROR) {
			res = GetShareList(cnam, lp);
		}
		else if (n_res==ERROR_CANCELLED) {
			Close();
			return;
		}
	}
	if (res!=NERR_Success) {
		beep_Warn();
		Caption = Caption + " - " + SysErrorMessage(res);
	}
	ListScrPanel->UpdateKnob();

	UnicodeString pnam = CurPathName;
	if (remove_top_s(pnam, "\\\\"))
		pnam = ExcludeTrailingPathDelimiter(get_tkn_r(pnam, "\\"));
	else
		pnam = EmptyStr;

	ListBoxSetIndex(lp, lp->Items->IndexOf(pnam));
}

//---------------------------------------------------------------------------
//�f�B���N�g���ꗗ�̍X�V
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::UpdatePathList(
	UnicodeString pnam)	//��f�B���N�g�� (EmptyStr �Ȃ�h���C�u�I��)
{
	if (isSelSub)
		Caption = pnam.IsEmpty()? UnicodeString("PC") : yen_to_delimiter(ExcludeTrailingPathDelimiter(pnam));

	cursor_HourGlass();
	LibIdxBase = -1;
	std::unique_ptr<TStringList> d_lst(new TStringList());
	//�h���C�u
	if (pnam.IsEmpty()) {
		isPC = true;
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
			if (!dp->accessible) continue;
			d_lst->Add(UnicodeString().sprintf(_T("%s %s"),
						get_tkn(dp->drive_str, '\\').c_str(), dp->volume.c_str()));
		}
		//���C�u����
		std::unique_ptr<TStringList> l_lst(new TStringList());
		get_files(LibraryPath, _T("*.library-ms"), l_lst.get());
		if (l_lst->Count>0) {
			d_lst->Add("-");
			LibIdxBase = d_lst->Count;
			d_lst->AddStrings(l_lst.get());
		}
		else {
			LibIdxBase = d_lst->Count;
		}
	}
	//�f�B���N�g��
	else {
		isPC = false;
		pnam = IncludeTrailingPathDelimiter(pnam);
		TSearchRec sr;
		if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
				if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
				if (ContainsStr("..", sr.Name)) continue;
				d_lst->AddObject(pnam + sr.Name, (TObject*)(NativeInt)sr.Attr);
			} while(FindNext(sr)==0);
			FindClose(sr);
		}
		if (!is_root_dir(pnam) && !isSelSub) d_lst->Add(pnam + "..");
		d_lst->CustomSort(comp_NaturalOrder);
	}

	TListBox *lp = ShareListBox;
	lp->Items->Assign(d_lst.get());
	ListScrPanel->UpdateKnob();

	int idx = -1;
	for (int i=0; i<lp->Count && idx==-1; i++) {
		UnicodeString lbuf = lp->Items->Strings[i];
		if (pnam.IsEmpty()) lbuf = get_tkn(lbuf, ' ');
		if (StartsStr(IncludeTrailingPathDelimiter(lbuf), IncludeTrailingPathDelimiter(CurPathName))) idx = i;
	}
	ListBoxSetIndex(lp, idx);
	cursor_Default();
}

//---------------------------------------------------------------------------
//�p���������X�g�̏���
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::PathTabControlDrawTab(TCustomTabControl *Control, int TabIndex,
	const TRect &Rect, bool Active)
{
	TTabControl *tp = (TTabControl*)Control;
	TCanvas *cv = tp->Canvas;
	cv->Brush->Color = Active? col_bgOptTab : get_PanelColor();
	cv->Font->Color  = Active? col_fgOptTab : get_LabelColor();
	cv->FillRect(Rect);
	int yp = (Rect.Height() - cv->TextHeight("Q")) /2;
	cv->TextOut(Rect.Left + ScaledInt(2), yp, yen_to_delimiter(PathTabControl->Tabs->Strings[TabIndex]));
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::PathTabControlChange(TObject *Sender)
{
	int idx = PathTabControl->TabIndex;
	if (idx!=-1) {
		UnicodeString pnam;
		for (int i=(StartsStr("\\\\", PathName)? 0 : 1); i<PathTabControl->Tabs->Count && i<=idx; i++)
			pnam += PathTabControl->Tabs->Strings[i] + "\\";

		if (is_computer_name(pnam)) {
			isSelDir = isSelSub = false;
			ComputerName = ExcludeTrailingPathDelimiter(pnam);
			UpdateShareList(ComputerName);
		}
		else {
			isSelDir = isSelSub = true;
			UpdatePathList(pnam);
		}
	}
}

//---------------------------------------------------------------------------
//���X�g���ڂ̕`��
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::ShareListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(ListFont);
	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (USAME_TS(lbuf, "-")) {
		draw_Separator(cv, Rect, col_bgList);
	}
	else {
		int xp = Rect.Left + ScaledInt(2);
		int yp = Rect.Top  + get_TopMargin2(cv);

		//�A�N�Z�X�L�[
		cv->Font->Color = col_fgList;
		cv->Font->Style = cv->Font->Style << fsUnderline;
		if (isPC) {
			if (Index<LibIdxBase) {
				cv->TextOut(xp, yp, lbuf.SubString(1, 1));
			}
			else {
				int idx = Index - LibIdxBase;
				if (idx<10) cv->TextOut(xp, yp, UnicodeString().sprintf(_T("%u"), (idx + 1)%10));
			}
		}
		else if (Index<10) {
			cv->TextOut(xp, yp, UnicodeString().sprintf(_T("%u"), (Index + 1)%10));
		}

		cv->Font->Style = cv->Font->Style >> fsUnderline;
		xp += get_CharWidth(cv, 2);

		//���C�u����
		if (isLibrary || (isPC && Index>=LibIdxBase)) {
			draw_SmallIcon2(lbuf, cv, xp, std::max(yp + (cv->TextHeight("Q") - ScaledInt(16))/2, 0));
			xp += ScaledInt(20);
			lbuf = get_base_name(lbuf);
			cv->Font->Color = col_Folder;
		}
		else if (isSelDir) {
			//�h���C�u
			if (isPC) {
				drive_info *dp = get_DriveInfo(lbuf);
				if (dp) {
					TIcon *ip = dp->small_ico;
					if (ip && ip->Handle)
						::DrawIconEx(cv->Handle, xp, yp + 1, ip->Handle, ScaledInt(16), ScaledInt(16), 0, NULL, DI_NORMAL);
				}
				xp += ScaledInt(20);
				lbuf = get_tkn_r(lbuf, ' ');
				cv->Font->Color = col_fgList;
			}
			//�f�B���N�g��
			else {
				lbuf = ExtractFileName(lbuf);
				int attr = (int)lp->Items->Objects[Index];
				cv->Font->Color =
					(attr & faSysFile)  ? col_System :
					(attr & faHidden)   ? col_Hidden :
					(attr & faReadOnly) ? col_ReadOnly :
					(attr & faSymLink)  ? col_SymLink : col_Folder;
			}
		}
		cv->TextOut(xp, yp, lbuf);
	}

	//�J�[�\��
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::ShareListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	//�h���C�u�m��
	UnicodeString dstr;
	if (isPC && (equal_ENTER(KeyStr) || (!KeyStr.IsEmpty() && isalpha(KeyStr[1])))) {
		if (equal_ENTER(KeyStr)) {
			int idx = lp->ItemIndex;
			if (idx!=-1 && idx<LibIdxBase-1)
				dstr = IncludeTrailingPathDelimiter(get_tkn(lp->Items->Strings[idx], ' '));
		}
		else {
			for (int i=0; i<LibIdxBase; i++) {
				if (StartsText(KeyStr, lp->Items->Strings[i])) {
					dstr.sprintf(_T("%c:\\"), KeyStr[1]);
					break;
				}
			}
		}
	}
	if (!dstr.IsEmpty()) {
		if (is_drive_accessible(dstr)) {
			NyanFiForm->RecoverFileList();
			NyanFiForm->UpdateCurDrive(dstr);
			Key = 0;
			ModalResult = mrOk;
			return;
		}
		else beep_Warn();
	}

	//�m��
	bool handled = true;
	if (is_Num0to9(KeyStr) || equal_ENTER(KeyStr)) {
		int idx;
		if (is_Num0to9(KeyStr)) {
			idx = (KeyStr.ToIntDef(0) + 9)%10;
			if (isPC) idx += LibIdxBase;
		}
		else {
			idx = lp->ItemIndex;
		}

		if (idx!=-1) {
			UnicodeString lbuf = lp->Items->Strings[idx];
			if (!USAME_TS(lbuf, "-")) {
				//���C�u����
				if (isLibrary || (isPC && idx>=LibIdxBase)) {
					NyanFiForm->PopSelLibrary(lbuf, CurListTag, lp);
				}
				//�f�B���N�g��
				else if (isSelDir) {
					UnicodeString pnam;
					pnam = lp->Items->Strings[idx];
					if (USAME_TS(ExtractFileName(pnam), "..")) pnam = get_parent_path(ExtractFilePath(pnam));
					pnam = IncludeTrailingPathDelimiter(pnam);
					if (rqRetPath) {
						PathName = pnam;
					}
					else if (isSelSub) {
						cursor_HourGlass();
						NyanFiForm->UpdateCurPath(IncludeTrailingPathDelimiter(pnam));
						cursor_Default();
					}
				}
				//���L
				else if (isShare) {
					UnicodeString dnam = IncludeTrailingPathDelimiter(ComputerName) + lp->Items->Strings[idx];
					cursor_HourGlass();
					NyanFiForm->UpdateCurPath(IncludeTrailingPathDelimiter(dnam));
					cursor_Default();
				}
				ModalResult = mrOk;
			}
		}
	}
	//�p�X�؂�ւ�
	else if (PathTabControl->Visible && is_ToRightOpe(KeyStr, cmd_F)) {
		int idx = PathTabControl->TabIndex;
		if (idx<PathTabControl->Tabs->Count-1) {
			PathTabControl->TabIndex = idx + 1;
			PathTabControlChange(PathTabControl);
		}
	}
	else if (PathTabControl->Visible && (is_ToLeftOpe(KeyStr, cmd_F) || USAME_TI(get_CmdStr(cmd_F), "ToParent"))) {
		int idx = PathTabControl->TabIndex;
		if (idx>0) {
			PathTabControl->TabIndex = idx -1;
			PathTabControlChange(PathTabControl);
		}
	}
	//�J�[�\���ړ�
	else if	(ExeCmdListBox(lp, cmd_F)) {
		;
	}
	//�������T�[�`
	else if (isSelDir && is_IniSeaKey(KeyStr)) {
		ListBoxInitialSearch(lp, KeyStr, true);
	}
	//����
	else if (USAME_TI(cmd_F, "ReturnList")) {
		ModalResult = mrCancel;
	}
	else {
		handled = false;
	}

	if (!is_DialogKey(Key) || handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::ShareListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//�C���N�������^���T�[�`�����
	if (_istalnum(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::ShareListBoxDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//UNC�p�X���R�s�[
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyUncActionExecute(TObject *Sender)
{
	TListBox *lp = ShareListBox;
	if (lp->ItemIndex!=-1)
		Clipboard()->AsText = IncludeTrailingPathDelimiter(ComputerName) + lp->Items->Strings[lp->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyUncActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isShare;
	ap->Enabled = ap->Visible && (ShareListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//���ׂĂ�UNC�p�X���R�s�[
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyUncAllActionExecute(TObject *Sender)
{
	TListBox *lp = ShareListBox;
	std::unique_ptr<TStringList> lst(new TStringList());
	for (int i=0; i<lp->Count; i++)
		lst->Add(IncludeTrailingPathDelimiter(ComputerName) + lp->Items->Strings[i]);
	Clipboard()->AsText = lst->Text;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyUncAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isShare;
	ap->Enabled = ap->Visible && (ShareListBox->Count>0);
}
//---------------------------------------------------------------------------
//�f�B���N�g���E�p�X���R�s�[
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyPathActionExecute(TObject *Sender)
{
	TListBox *lp = ShareListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString lbuf = lp->Items->Strings[lp->ItemIndex];
		if (isPC) lbuf = IncludeTrailingPathDelimiter(get_tkn(lbuf, ' '));
		Clipboard()->AsText = lbuf;
	}
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyPathActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isSelSub;
	ap->Enabled = ap->Visible && (ShareListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
//���ׂẴf�B���N�g���E�p�X���R�s�[
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyPathAllActionExecute(TObject *Sender)
{
	TListBox *lp = ShareListBox;
	std::unique_ptr<TStringList> lst(new TStringList());
	bool is_drv = isPC;
	for (int i=0; i<lp->Count; i++) {
		UnicodeString lbuf = lp->Items->Strings[i];
		if (is_drv && is_separator(lbuf)) is_drv = false;
		if (is_drv) lbuf = IncludeTrailingPathDelimiter(get_tkn(lbuf, ' '));
		lst->Add(lbuf);
	}
	Clipboard()->AsText = lst->Text;
}
//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::CopyPathAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = isSelSub;
	ap->Enabled = ap->Visible && (ShareListBox->Count>0);
}

//---------------------------------------------------------------------------
void __fastcall TNetShareDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString topic = HELPTOPIC_FL;
	if		(isShare)				topic += "#ShareList";
	else if (isSelDir && isSelSub)	topic += "#SubDirList";
	else if (isLibrary)				topic += "#Library";
	else							topic = EmptyStr;

	if (!topic.IsEmpty())
		SpecialKeyProc(this, Key, Shift, topic.c_str());
	else
		SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

