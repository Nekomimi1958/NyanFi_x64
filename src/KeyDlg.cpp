//----------------------------------------------------------------------//
// NyanFi																//
//  �L�[���蓖�Ĉꗗ													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <Vcl.Clipbrd.hpp>
#include <RegularExpressions.hpp>
#include "UserMdl.h"
#include "Global.h"
#include "KeyDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TKeyListDlg *KeyListDlg;

//---------------------------------------------------------------------------
__fastcall TKeyListDlg::TKeyListDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormCreate(TObject *Sender)
{
	GridScrPanel = new UsrScrollPanel(GridPanel, KeyListGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);

	CurList = new TStringList();

	ToFilter = false;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	TStringGrid *gp = KeyListGrid;
	InitializeListGrid(gp);
	InitializeListHeader(KeyListHeader, _T("�L�[|�R�}���h|����"));
	IniFile->LoadGridColWidth(gp, 3, 100,120,220);
	set_HeaderFromGrid(gp, KeyListHeader);
	set_UsrScrPanel(GridScrPanel);

	KeySortMode 				= IniFile->ReadIntGen(_T("KeyListSortMode"));
	ShowAllCmdCheckBox->Checked = IniFile->ReadBoolGen(_T("KeyListShowAll"));
	CnfExeCheckBox->Checked 	= IniFile->ReadBoolGen(_T("KeyListCnfExe"));

	set_MigemoCheckBox(MigemoCheckBox, _T("KeyListMigemo"));

	if (!UnInitializing) KeyTabControlChange(NULL);

	(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)gp)->SetFocus();
	FilterEdit->Color = ToFilter? scl_Window : col_Invalid;
	FilterEdit->Text  = EmptyStr;

	CommandStr = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->SaveGridColWidth(KeyListGrid);

	IniFile->WriteIntGen(_T("KeyListSortMode"),	KeySortMode);
	IniFile->WriteBoolGen(_T("KeyListShowAll"),	ShowAllCmdCheckBox);
	IniFile->WriteBoolGen(_T("KeyListMigemo"),	MigemoCheckBox);
	IniFile->WriteBoolGen(_T("KeyListCnfExe"),	CnfExeCheckBox);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormDestroy(TObject *Sender)
{
	delete CurList;
	delete GridScrPanel;
}

//---------------------------------------------------------------------------
//�����^�u�̕`��
//���e�[�}���p���ɉ����^�u���������`�悳��Ȃ��s��̑΍�
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyTabControlDrawTab(TCustomTabControl *Control, int TabIndex,
		const TRect &Rect, bool Active)
{
	draw_BottomTab(Control, TabIndex, Rect, Active);
}
//---------------------------------------------------------------------------
//�ꗗ�̐؂�ւ�
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyTabControlChange(TObject *Sender)
{
	//�Y������ݒ�𒊏o
	std::unique_ptr<TStringList> cmd_lst(new TStringList());
	UnicodeString id_str = ScrModeIdStr.SubString(KeyTabControl->TabIndex + 1, 1).UCAT_T(":");
	for (int i=0; i<CmdSetList->Count; i++) {
		UnicodeString lbuf = CmdSetList->Strings[i];
		if (StartsText(id_str, lbuf)) cmd_lst->Add(lbuf);
	}

	//�ǉ����j���[/�O���c�[���̃G�C���A�X
	if (KeyTabControl->TabIndex==0) {
		UnicodeString itm;
		for (int i=0; i<ExtMenuList->Count; i++) {
			itm = get_csv_item(ExtMenuList->Strings[i], 3);
		 	if (!itm.IsEmpty()) cmd_lst->Add("F:$" + itm);
		}
		for (int i=0; i<ExtToolList->Count; i++) {
			itm = get_csv_item(ExtToolList->Strings[i], 4);
		 	if (!itm.IsEmpty()) cmd_lst->Add("F:$" + itm);
		}
	}

	CurList->Clear();
	for (int i=0; i<KeyFuncList->Count; i++) {
		UnicodeString lbuf = KeyFuncList->Strings[i];
		if (StartsText(id_str, lbuf)) CurList->Add(lbuf);
	}

	if (KeyTabControl->TabIndex==0) {
		CurList->Add(UAPP_T(IniSeaShift, "A�`Z,�_=InitialSearch"));
		if (IniSeaByNum || IniSeaBySign) CurList->Add(UAPP_T(IniSeaShift, "0�`9=InitialSearch"));
	}

	//���o�^�R�}���h��ǉ�
	if (ShowAllCmdCheckBox->Checked) {
		for (int i=0; i<CurList->Count; i++) {
			int idx = cmd_lst->IndexOf(id_str + get_CmdStr(CurList->ValueFromIndex[i]));
			if (idx!=-1) cmd_lst->Delete(idx);
		}
		for (int i=0; i<cmd_lst->Count; i++) cmd_lst->Strings[i] = ReplaceStr(cmd_lst->Strings[i], ":", ":=");
		CurList->AddStrings(cmd_lst.get());
	}

	//�t�B���^
	if (!FilterEdit->Text.IsEmpty()) {
		UnicodeString ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Checked, FilterEdit->Text);
		if (!ptn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			int i=0;
			while (i<CurList->Count) {
				UnicodeString lbuf = get_tkn_r(CurList->Strings[i], ':');
				UnicodeString desc = get_CmdDesc(get_CmdStr(get_tkn_r(lbuf, '=')), true, NULL,NULL, USAME_TI(id_str, "V:"));
				lbuf.cat_sprintf(_T(" %s"), desc.c_str());
				if (!TRegEx::IsMatch(lbuf, ptn, opt)) CurList->Delete(i); else i++;
			}
		}
		else if (!MigemoCheckBox->Checked) beep_Warn();
	}

	//�\�[�g
	switch (KeySortMode) {
	case  1:	//�R�}���h
		CurList->CustomSort(KeyComp_Cmd);	break;
	case  2:	//����
		cursor_HourGlass();
		CurList->CustomSort(KeyComp_Dsc);
		cursor_Default();
		break;
	default:	//�L�[
		CurList->CustomSort(KeyComp_Key);
	}
	KeyListHeader->Invalidate();

	//�ꗗ�ɕ\��
	TStringGrid *gp = KeyListGrid;
	if (CurList->Count>0) {
		gp->RowCount = CurList->Count;
		for (int i=0; i<CurList->Count; i++) {
			UnicodeString kbuf = get_tkn_r(CurList->Names[i], ':');
			int cp = 1;
			while (cp<=kbuf.Length()) {
				if (kbuf[cp]=='+' || kbuf[cp]=='~') {
					if (cp>1 && kbuf[cp - 1]!='_') {
						kbuf.Insert(" ", cp); cp += 2;
						if (cp<=kbuf.Length()) kbuf.Insert(" ", cp++);
					}
					else cp++;
				}
				else cp++;
			}
			gp->Cells[0][i] = kbuf;
			UnicodeString cmd = CurList->ValueFromIndex[i];
			gp->Cells[2][i] = get_CmdDesc(cmd, true, NULL,NULL, USAME_TI(id_str, "V:"));
			gp->Cells[1][i] = del_CmdDesc(cmd);
		}
	} 
	else {
		gp->RowCount = 1;
		clear_GridRow(gp, 0);
	}

	gp->Row = 0;
	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//�w�b�_�̕`��
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderDrawSection(THeaderControl *HeaderControl,
		THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect, (Section->Index==KeySortMode)? 1 : 0);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(KeyListGrid, KeyListHeader);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	set_GridFromHeader(KeyListHeader, KeyListGrid);
	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//�\�[�g�ύX
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	KeySortMode = Section->Index;
	KeyTabControlChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SortKeyActionExecute(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	KeySortMode = ap->Tag;
	KeyTabControlChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SortKeyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Checked = (ap->Tag==KeySortMode);
}

//---------------------------------------------------------------------------
//�Z���̕`��
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin2(cv);
	UnicodeString cellstr = gp->Cells[ACol][ARow];
	bool use_fgsel = is_SelFgCol(State);

	cv->Brush->Color = State.Contains(gdSelected)? (gp->Focused()? col_selItem : col_oppItem) : col_bgList;
	cv->FillRect(Rect);
	cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;

	//�L�[
	if (ACol==0) {
		// + �� ~ ���V���{���F�ŋ����\��
		std::unique_ptr<TStringList> em_lst(new TStringList());
		em_lst->Text = "+\r\n~\r\n";
		EmphasisTextOut(cellstr, em_lst.get(), cv, xp, yp, true, false,
			use_fgsel? col_fgSelItem : col_Symbol, cv->Brush->Color);
	}
	//�R�}���h
	else if (ACol==1) {
		cellstr = minimize_str(cellstr, gp->Canvas, gp->ColWidths[1] - 8, true);
		if (starts_Dollar(cellstr)) {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_Headline;
			cv->TextRect(Rect, xp, yp, cellstr);
		}
		else {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_Strings;
			EmphasisTextOut(cellstr, get_CmdStr(cellstr), cv, xp, yp, false, true,
				use_fgsel? col_fgSelItem : col_Reserved, cv->Brush->Color);
		}
	}
	//����
	else {
		cv->TextRect(Rect, xp, yp, cellstr);
	}

	//�J�[�\��
	draw_GridCursor(gp, Rect, ARow, State);

	//���m��
	if (gp->RowCount==1 && ARow==0 && ACol==1 && !cellstr.IsEmpty()) {
		cv->Brush->Color = col_Cursor;
		cv->FrameRect(Rect);
	}
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::MigemoCheckBoxClick(TObject *Sender)
{
	KeyTabControlChange(NULL);
}

//---------------------------------------------------------------------------
//�t�B���^�ɂ��ꗗ�̍X�V
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditChange(TObject *Sender)
{
	KeyTabControlChange(NULL);

	//�m�葦���s
	TStringGrid *gp = KeyListGrid;
	if (CnfExeCheckBox->Checked && FilterEdit->Focused() && !FilterEdit->Text.IsEmpty()
		&& gp->RowCount==1 && !gp->Cells[1][0].IsEmpty())
	{
		Application->ProcessMessages();  Sleep(500);
		CommandStr	= gp->Cells[1][0];
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}

//---------------------------------------------------------------------------
//�t�B���^���ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if		(contained_wd_i(KeysStr_ToList, KeyStr))	KeyListGrid->SetFocus();
	else if (MovGridFromFilter(KeyListGrid, KeyStr))	;
	else if (SameText(KeyStr, KeyStr_Migemo))			MigemoCheckBox->Checked = !MigemoCheckBox->Checked;
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
//�t�B���^����Enter�L�[�őI�����ڂ����s
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key)) {
		Key = 0;
	}
	else if (Key==VK_RETURN) {
		if (KeyListGrid->Row!=-1) {
			CommandStr	= KeyListGrid->Cells[1][KeyListGrid->Row];
			ModalResult = mrOk;
		}
		Key = 0;
	}
}

//---------------------------------------------------------------------------
//���o�^�R�}���h���\��
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::ShowAllCmdCheckBoxClick(TObject *Sender)
{
	KeyTabControlChange(NULL);
	KeyListGrid->SetFocus();
}

//---------------------------------------------------------------------------
//�ꗗ�ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	//�m��
	if (equal_ENTER(KeyStr)) {
		CommandStr	= gp->Cells[1][gp->Row];
		ModalResult = mrOk;
	}
	//�J�[�\���ړ�
	else if (GridCursorMove(cmd_F, gp) || GridCursorMove(cmd_V, gp))	;
	else if (StartsText("IncSearch", cmd_F))	FilterEdit->SetFocus();
	//����
	else if (USAME_TI(cmd_F, "ReturnList"))	ModalResult = mrCancel;
	//�^�u�؂芷��
	else if (is_ToRightOpe(KeyStr, cmd_F)) {
		KeyTabControl->TabIndex = (KeyTabControl->TabIndex + 1) % KeyTabControl->Tabs->Count;
		KeyTabControlChange(NULL);
	}
	else if (is_ToLeftOpe(KeyStr, cmd_F)) {
		KeyTabControl->TabIndex = (KeyTabControl->TabIndex>0) ? KeyTabControl->TabIndex - 1
															  : KeyTabControl->Tabs->Count - 1;
		KeyTabControlChange(NULL);
	}
	//�s�R�s�[
	else if (USAME_TI(cmd_V, "ClipCopy")) {
		UnicodeString ibuf = get_tkn_r(CurList->Names[gp->Row], ':');
		UnicodeString cmd  = CurList->ValueFromIndex[gp->Row];
		UnicodeString dsc  = get_CmdDesc(cmd, true);
		cmd = del_CmdDesc(cmd);
		copy_to_Clipboard(ibuf.cat_sprintf(_T("\t%s\t%s"), cmd.c_str(), dsc.c_str()));
	}
	//�������T�[�`
	else if (is_IniSeaKey(KeyStr)) {	//KeyStr �ɐ��K�\���p�^�[�����Ԃ�
		TRegExOptions opt; opt << roIgnoreCase;
		int idx0=0, idx1=0;
		for (int i=0; i<gp->RowCount && idx1==0; i++) {
			if (i<=gp->Row && idx0!=0) continue;
			if (TRegEx::IsMatch(gp->Cells[0][i], KeyStr, opt)) ((i<=gp->Row)? idx0 : idx1) = i;
		}
		int rn = (idx1>0)? idx1 : idx0;
		if (rn>0) gp->Row = rn;
	}
	//�E�N���b�N���j���[
	else if (StartsText("ContextMenu", cmd_F)) show_PopupMenu(gp);

	if (!is_DialogKey(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//���݂̈ꗗ���e���擾(TAB��؂�)
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::GetStrList(TStringList *lst)
{
	lst->Add("�L�[\t�R�}���h\t����");
	for (int i=0; i<CurList->Count; i++) {
		UnicodeString ibuf = get_tkn_r(CurList->Names[i], ':');
		UnicodeString cmd  = CurList->ValueFromIndex[i];
		UnicodeString dsc  = get_CmdDesc(cmd, true);
		cmd = del_CmdDesc(cmd);
		lst->Add(ibuf.cat_sprintf(_T("\t%s\t%s"), cmd.c_str(), dsc.c_str()));
	}
}

//---------------------------------------------------------------------------
//�ꗗ���R�s�[
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::CopyListItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> cbuf(new TStringList());
	GetStrList(cbuf.get());
	copy_to_Clipboard(cbuf->Text);
}
//---------------------------------------------------------------------------
//�ꗗ���t�@�C���ɕۑ�
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SaveAsListItemClick(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, _T("�L�[���蓖�Ĉꗗ"));
	UserModule->SaveTxtDlg->InitialDir	  = ExcludeTrailingPathDelimiter(SaveTxtPath);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveTxtPath  = ExtractFilePath(UserModule->SaveTxtDlg->FileName);
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		GetStrList(fbuf.get());
		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, _T("�L�[���蓖�Ĉꗗ")));
	}

	KeyListGrid->SetFocus();
}
//---------------------------------------------------------------------------
//�R�}���h���R�s�[
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::CopyCmdItemClick(TObject *Sender)
{
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count)
		copy_to_Clipboard(del_CmdDesc(CurList->ValueFromIndex[idx]));
}
//---------------------------------------------------------------------------
//�R�}���h�̃w���v
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::HelpCmdActionExecute(TObject *Sender)
{
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count) {
		UnicodeString kwd = get_CmdStr(CurList->ValueFromIndex[idx]);
		UnicodeString topic;
		switch (KeyTabControl->TabIndex) {
		case  1: topic.UCAT_T(HELPTOPIC_IS);	break;	//�e�L�X�g�r���A�[
		case  2: topic.UCAT_T(HELPTOPIC_TV);	break;	//�e�L�X�g�r���A�[
		case  3: topic.UCAT_T(HELPTOPIC_IV);	break;	//�C���[�W�r���A�[
		case  4: topic.UCAT_T(HELPTOPIC_CILW);	break;	//���O
		default: topic.UCAT_T(HELPTOPIC_FL);	break;	//�t�@�C���[
		}

		if (topic.Pos('#')==0 && !kwd.IsEmpty()) topic.cat_sprintf(_T("#%s"), kwd.c_str());
		HtmlHelpTopic(topic.c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::HelpCmdActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count) {
		UnicodeString kwd = CurList->ValueFromIndex[idx];
		ap->Enabled = (!kwd.IsEmpty() && !starts_AT(kwd) && !starts_Dollar(kwd));
	}
	else {
		ap->Enabled = false;
	}
}

//---------------------------------------------------------------------------
//�I�v�V�����ݒ��
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::OptionItemClick(TObject *Sender)
{
	CommandStr.USET_T("OptionDlg_KY");
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

