//----------------------------------------------------------------------//
// NyanFi																//
//  �c�[���o�[�̐ݒ�													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "CmdListDlg.h"
#include "BtnDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TToolBtnDlg *ToolBtnDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TToolBtnDlg::TToolBtnDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	//�h���b�v�^�[�Q�b�g��ݒ�
	usr_SH->AddTargetList(this, BtnCmdsComboBox);

	IniIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	std::unique_ptr<TStringList> cmd_lst(new TStringList());
	if (ScrMode==SCMD_FLIST) {
		cmd_lst->Text = _T("ExeMenuFile\nFileEdit\nFileRun\nLoadWorkList\nOpenByApp\nOpenByWin\nTextViewer\n")
						_T("ChangeDir\nOpenByExp\nPlayList\nSubDirList\n");
	}

	//�G�C���A�X
	for (int i=0; i<ExtMenuList->Count; i++) {
		UnicodeString itm = get_csv_item(ExtMenuList->Strings[i], 3);
	 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
	}
	for (int i=0; i<ExtToolList->Count; i++) {
		UnicodeString itm = get_csv_item(ExtToolList->Strings[i], 4);
	 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
	}

	BtnCmdsComboBox->Items->Assign(cmd_lst.get());
	BtnCmdsComboBox->Text = EmptyStr;

	ButtonList = GetCurBtnList();

	BtnListBox->Tag = LBTAG_HAS_SICO;
	set_ListBoxItemHi(BtnListBox);
	UserModule->InitializeListBox(BtnListBox);

	BtnListBox->Items->Assign(ButtonList);
	CmdLabel->Caption = "���s����R�}���h (ExeCommands �̃p�����[�^)";
	CaptionEdit->Text = EmptyStr;
	IconEdit->Text	  = EmptyStr;

	//�������ڈʒu��ݒ�
	if (IniIndex!=-1) {
		BtnListBox->ItemIndex = IniIndex;
		BtnListBoxClick(BtnListBox);
		CaptionEdit->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	IniIndex = -1;
}

//---------------------------------------------------------------------------
//���b�v�󂯓���
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::WmDropped(TMessage &msg)
{
	TWinControl *cp = (TWinControl *)msg.WParam;
	if (cp==BtnCmdsComboBox) {
		UnicodeString s;
		if (DroppedMode==DROPEFFECT_LINK) {
			if (contains_s(DroppedList->Strings[0], _T('\t')))
				BtnCmdsComboBox->Text = s.sprintf(_T("OpenByWin_\"%s\""), get_post_tab(DroppedList->Strings[0]).c_str());
		}
		else {
			UnicodeString fnam = DroppedList->Strings[0];
			BtnCmdsComboBox->Text = s.sprintf(_T("OpenByWin_\"%s\""), fnam.c_str());
			if (test_LnkExt(get_extension(fnam))) fnam = usr_SH->get_LnkName(fnam);
			if (test_ExeExt(get_extension(fnam))) IconEdit->Text = exclude_env_path(fnam);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::FormResize(TObject *Sender)
{
	BtnListBox->Invalidate();
}

//---------------------------------------------------------------------------
//�m��
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::OkButtonClick(TObject *Sender)
{
	if (ButtonList) ButtonList->Assign(BtnListBox->Items);
}

//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::BtnCmdsComboBoxChange(TObject *Sender)
{
	UnicodeString anam = BtnCmdsComboBox->Text;
	if (remove_top_Dollar(anam)) {
		//�A�C�R���擾
		UnicodeString inam;
		//�O���c�[��
		TStringDynArray itm_buf = record_of_csv_list(ExtToolList, anam, 4, EXTTOOL_CSVITMCNT);
		if (itm_buf.Length==EXTTOOL_CSVITMCNT)
			inam = itm_buf[1];
		//�ǉ����j���[
		else {
			itm_buf = record_of_csv_list(ExtMenuList, anam, 3, EXTMENU_CSVITMCNT);
			if (itm_buf.Length==EXTMENU_CSVITMCNT) inam = itm_buf[5];
		}

		if (!inam.IsEmpty()) IconEdit->Text = inam;
	}
}

//---------------------------------------------------------------------------
//�t�@�C��/�f�B���N�g���̎Q��
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::RefCmdsBtnClick(TObject *Sender)
{
	UnicodeString cmd = get_CmdStr(BtnCmdsComboBox->Text);

	//�f�B���N�g��
	if (contained_wd_i(_T("ChangeDir|ChangeOppDir|CopyTo|MoveTo|OpenByExp|PlayList|SubDirList"), cmd)) {
		UnicodeString dnam;
		if (UserModule->SelectDirEx(_T("�R�}���h�p�����[�^�̎w��"), dnam)) {
			BtnCmdsComboBox->Text = UnicodeString().sprintf(_T("%s_\"%s\""), cmd.c_str(), dnam.c_str());
		}
	}
	//�t�@�C��
	else if (contained_wd_i(_T("ExeMenuFile|FileEdit|FileRun|LoadTabGroup|LoadWorkList|OpenByApp|OpenByWin|TextViewer"), cmd)) {
		UserModule->PrepareOpenDlg(_T("�R�}���h�p�����[�^�̎w��"), F_FILTER_ALL, NULL, RefParamPath);

		UnicodeString fnam;
		if (UserModule->OpenDlgToStr(fnam)) {
			RefParamPath = ExtractFilePath(fnam);
			if (contained_wd_i(_T("FileRun|OpenByWin"), cmd)) {
				if (test_LnkExt(get_extension(fnam))) fnam = usr_SH->get_LnkName(fnam);
				if (test_ExeExt(get_extension(fnam))) IconEdit->Text = fnam = exclude_env_path(fnam);
			}
			else if (contained_wd_i(_T("ExeMenuFile|LoadTabGroup|LoadWorkList"), cmd))
				fnam = to_relative_name(fnam);

			BtnCmdsComboBox->Text = UnicodeString().sprintf(_T("%s_\"%s\""), cmd.c_str(), fnam.c_str());
		}
	}
	else if (CmdFileListDlg->ShowToSelect(get_cmdfile(cmd))==mrOk) {
		BtnCmdsComboBox->Text = "@" + to_relative_name(CmdFileListDlg->CmdFileName);
	}
}
//---------------------------------------------------------------------------
//�A�C�R���Q��
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::RefIconBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("�A�C�R���̎w��"), F_FILTER_ICO, NULL, IconFilePath);
	if (UserModule->OpenDlgToEdit(IconEdit, true)) IconFilePath = ExtractFilePath(rel_to_absdir(IconEdit->Text));
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::IconEditChange(TObject *Sender)
{
	usr_SH->draw_SmallIcon(rel_to_absdir(get_actual_name(IconEdit->Text)), IcoImage, scl_BtnFace);
}

//---------------------------------------------------------------------------
//�t�@�C���ҏW
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::EditFileActionExecute(TObject *Sender)
{
	open_by_TextEditor(get_cmds_prm_file(BtnCmdsComboBox->Text));
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::EditFileActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !get_cmds_prm_file(BtnCmdsComboBox->Text).IsEmpty();
}

//---------------------------------------------------------------------------
//�ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::BtnListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv  = lp->Canvas;
	cv->Font->Assign(lp->Font);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	int x = Rect.Left + 4;
	int y = Rect.Top + get_TopMargin(cv);

	TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[Index], 3, true);
	//�Z�p���[�^
	if (is_separator(itm_buf[0])) {
		draw_Separator(cv, Rect);
		int w = extract_int_def(itm_buf[1], -1);
		if (w>=0) cv->TextOut(BtnCmdsComboBox->Left, y, UnicodeString().sprintf(_T("< %u >"), w));
	}
	//�{�^��
	else {
		x += 4;
		if (usr_SH->draw_SmallIcon(rel_to_absdir(get_actual_name(itm_buf[2])), cv, x, y)) x += ScaledInt(18);
		cv->TextOut(x, y, itm_buf[0]);
		x = BtnCmdsComboBox->Left;
		cv->TextOut(x, y, itm_buf[1]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::BtnListBoxClick(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	if (lp->Count==0) return;

	if (lp->ItemIndex==-1) lp->ItemIndex = 0;
	int idx = lp->ItemIndex;

	TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], 3, true);
	CaptionEdit->Text	  = itm_buf[0];
	BtnCmdsComboBox->Text = itm_buf[1];
	IconEdit->Text		  = itm_buf[2];

	CmdLabel->Caption = is_separator(itm_buf[0])? "�Z�p���[�^�̕� (�ȗ�����4)" : "���s����R�}���h (ExeCommands �̃p�����[�^)";
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::BtnListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}

//---------------------------------------------------------------------------
//CSV���ڂ��쐬
//---------------------------------------------------------------------------
UnicodeString __fastcall TToolBtnDlg::MakeCsvItem()
{
	return UnicodeString().sprintf(_T("%s,%s,%s"),
			make_csv_str(CaptionEdit->Text).c_str(),
			make_csv_str(BtnCmdsComboBox->Text).c_str(),
			make_csv_str(IconEdit->Text).c_str());
}

//---------------------------------------------------------------------------
//�ǉ�
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::AddBtnActionExecute(TObject *Sender)
{
	BtnListBox->ItemIndex = BtnListBox->Items->Add(MakeCsvItem());
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::AddBtnActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !CaptionEdit->Text.IsEmpty() || !IconEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//�}��
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::InsBtnActionExecute(TObject *Sender)
{
	BtnListBox->Items->Insert(BtnListBox->ItemIndex, MakeCsvItem());
	BtnListBox->ItemIndex = BtnListBox->ItemIndex - 1;
}
//---------------------------------------------------------------------------
//�ύX
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::ChgBtnActionExecute(TObject *Sender)
{
	int idx = BtnListBox->ItemIndex;  if (idx==-1) return;
	BtnListBox->Items->Strings[idx] = MakeCsvItem();
}
//---------------------------------------------------------------------------
void __fastcall TToolBtnDlg::ChgBtnActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= (!CaptionEdit->Text.IsEmpty() || !IconEdit->Text.IsEmpty()) && BtnListBox->ItemIndex!=-1;
}

//---------------------------------------------------------------------------
//�w���v
//---------------------------------------------------------------------------
bool __fastcall TToolBtnDlg::FormHelp(WORD Command, NativeInt Data, bool &CallHelp)
{
	if (Command==HELP_CONTEXT || Command==HELP_CONTEXTPOPUP) {
		bool handled = false;
		if (BtnCmdsComboBox->Focused() && !is_separator(CaptionEdit->Text)) {
			UnicodeString kwd = get_tkn(get_CmdStr(BtnCmdsComboBox->Text), ':');
			UnicodeString topic;
			if (!kwd.IsEmpty() && !starts_AT(kwd) && !starts_Dollar(kwd)) {
				if		(ScrMode==SCMD_TVIEW)	topic.UCAT_T(HELPTOPIC_TV);
				else if (ScrMode==SCMD_IVIEW)	topic.UCAT_T(HELPTOPIC_IV);
				else							topic.UCAT_T(HELPTOPIC_FL);
				topic.cat_sprintf(_T("#%s"), kwd.c_str());
				HtmlHelpTopic(topic.c_str());
				handled = true;
			}
		}

		if (!handled) HtmlHelpContext(Data);
		CallHelp = false;
	}
	return true;
}
//---------------------------------------------------------------------------

