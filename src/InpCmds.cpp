//----------------------------------------------------------------------//
// NyanFi																//
//  �R�}���h�̓���														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <RegularExpressions.hpp>
#include "usr_excmd.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpCmds.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInpCmdsDlg *InpCmdsDlg;

//---------------------------------------------------------------------------
__fastcall TInpCmdsDlg::TInpCmdsDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::FormCreate(TObject *Sender)
{
	ItemList   = new TStringList();

	SubCmdList = new TStringList();
	SubCmdList->Text = XCMD_SubCmds;
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::FormShow(TObject *Sender)
{
	Constraints->MinHeight = 0;
	Constraints->MaxHeight = 0;

	if (toCopy || toRefer) {
		set_FormTitle(this, toCopy? _T("�R�}���h�����R�s�[") : _T("�R�}���h���̎Q��"));
		ModePanel->Visible = true;
		ClientHeight = MainPanel->Height + ModePanel->Height;
		CmdsComboBox->Tag = 0;
	}
	else {
		set_FormTitle(this, _T("�R�}���h�̓���"));
		ModePanel->Visible = false;
		ClientHeight = MainPanel->Height;
		CmdsComboBox->Tag = CBTAG_HISTORY;
	}
	Constraints->MinHeight = Height;
	Constraints->MaxHeight = Height;

	IniFile->LoadPosInfo(this, DialogCenter);

	set_MigemoCheckBox(MigemoCheckBox, _T("InpCmdsMigemo"));

	HistoryList = NULL;
	InhFilter	= false;
	IsRef		= toCopy || toRefer;
	SetList();

	CmdsComboBox->AutoComplete = false;
	SubComboBox->AutoComplete  = false;
	SubComboBox->Enabled	   = false;
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	CmdsComboBox->DroppedDown = false;
	SubComboBox->DroppedDown  = false;

	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("InpCmdsMigemo"),	MigemoCheckBox);

	if (HistoryList) HistoryList->Assign(CmdsComboBox->Items);
	HistoryList = NULL;

	toCopy = toRefer = false;
	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::FormDestroy(TObject *Sender)
{
	delete ItemList;
	delete SubCmdList;
}

//---------------------------------------------------------------------------
//��⃊�X�g��ݒ�
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::SetList()
{
	if (HistoryList) HistoryList->Assign(CmdsComboBox->Items);
	HistoryList = NULL;

	UnicodeString idxStr;
	bool is_TV = false;
	if ((IsRef && ModeTabControl->TabIndex==1) || (!IsRef && ScrMode==SCMD_TVIEW)) {
		if (!IsRef) HistoryList = InputCmdsHistory;
		idxStr.USET_T("V");
		is_TV = true;
	}
	else if ((IsRef && ModeTabControl->TabIndex==2) || (!IsRef && ScrMode==SCMD_IVIEW)) {
		if (!IsRef) HistoryList = InputCmdsHistoryV;
		idxStr.USET_T("I");
	}
	else if (IsRef && ModeTabControl->TabIndex==3) {
		idxStr.USET_T("L");
	}
	else {
		if (!IsRef) HistoryList = InputCmdsHistoryI;
		idxStr.USET_T("F");
	}
	if (HistoryList) CmdsComboBox->Items->Assign(HistoryList); else CmdsComboBox->Clear();

	//��⃊�X�g�̐ݒ�
	ItemList->Clear();
	for (int i=0; i<CmdSetList->Count; i++) {
		UnicodeString lbuf = CmdSetList->Strings[i];
		if (StartsStr(idxStr, lbuf))
			ItemList->Add(get_CmdDesc(get_tkn_r(lbuf, ':'), false,NULL,NULL, is_TV));
	}

	if (!IsRef) {
		//�R�}���h�t�@�C��
		std::unique_ptr<TStringList> f_lst(new TStringList());
		get_files(ExePath, _T("*.nbt"), f_lst.get(), true);
		for (int i=0; i<f_lst->Count; i++) ItemList->Add("@" + to_relative_name(f_lst->Strings[i]));

		//�G�C���A�X
		if (ScrMode==SCMD_FLIST) {
			UnicodeString tmp;
			//�ǉ����j���[
			for (int i=0; i<ExtMenuList->Count; i++) {
				TStringDynArray itm_buf = get_csv_array(ExtMenuList->Strings[i], EXTMENU_CSVITMCNT, true);
				if (!itm_buf[3].IsEmpty()) ItemList->Add(tmp.sprintf(_T("$%s %s"), itm_buf[3].c_str(), itm_buf[0].c_str()));
			}
			//�O���c�[��
			for (int i=0; i<ExtToolList->Count; i++) {
				TStringDynArray itm_buf = get_csv_array(ExtToolList->Strings[i], EXTTOOL_CSVITMCNT, true);
				if (!itm_buf[4].IsEmpty()) ItemList->Add(tmp.sprintf(_T("$%s %s"), itm_buf[4].c_str(), itm_buf[0].c_str()));
			}
		}

		//�⏕�R�}���h
		ItemList->AddStrings(SubCmdList);
	}

	CmdsComboBox->Text = EmptyStr;
	CmdsComboBox->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::ModeTabControlChange(TObject *Sender)
{
	SetList();
}

//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::MigemoCheckBoxClick(TObject *Sender)
{
	Filter();
}

//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::CmdsComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TComboBox *cp_inp  = CmdsComboBox;
	TComboBox *cp_down = SubComboBox->DroppedDown? SubComboBox :
					    CmdsComboBox->DroppedDown? CmdsComboBox : NULL;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (contained_wd_i(KeysStr_CsrUp, KeyStr)) KeyStr = "UP";

	bool handled = true;
	if (equal_ENTER(KeyStr)) {
		if (SubComboBox->DroppedDown) {
			UnicodeString wd = get_tkn(SubComboBox->Text, ' ');
			SubComboBox->DroppedDown = false;
			UnicodeString lbuf = cp_inp->Text;
			int p = pos_r_q_colon(lbuf); 
			lbuf = (p>0)? lbuf.SubString(1, p).UCAT_T(" ") : EmptyStr;
			lbuf += wd;
			cp_inp->SetFocus();
			cp_inp->Text	  = lbuf;
			cp_inp->SelLength = 0;
			cp_inp->SelStart  = lbuf.Length();
		}
		else {
			if (!cp_inp->Text.IsEmpty()) {
				add_ComboBox_history(cp_inp, cp_inp->Text);
				ModalResult = mrOk;
			}
			else ModalResult = mrCancel;
		}
	}
	else if (equal_ESC(KeyStr)) {
		if (cp_down)
			cp_down->DroppedDown = false;
		else
			ModalResult = mrCancel;
	}
	else if (equal_DEL(KeyStr)) {
		UnicodeString kwd = cp_inp->Text;
		int p = pos_r_q_colon(kwd); 
		UnicodeString wd = (p>0)? kwd.SubString(p + 1, kwd.Length() - p) : kwd;
		//�p�����[�^�Ȃ��Ȃ�R�}���h���ƍ폜
		if (!contains_s(wd, _T('_'))) {
			cp_inp->Text	  = (p>1)? kwd.SubString(1, p - 1) : EmptyStr;
			cp_inp->SelLength = 0;
			cp_inp->SelStart  = cp_inp->Text.Length();
		}
		//�p�����[�^���͒��͒ʏ�̕����폜
		else {
			InhFilter = true;
			handled   = false;
		}
	}
	else if (USAME_TI(KeyStr, "DOWN")) {
		if (!cp_down)
			 cp_inp->DroppedDown = (cp_inp->Items->Count>0);
		else if (cp_down->ItemIndex<cp_down->Items->Count - 1)
			cp_down->ItemIndex = cp_down->ItemIndex + 1;
	}
	else if (USAME_TI(KeyStr, "UP")) {
		if (!cp_down) {
			if (cp_inp->Items->Count>0) {
				cp_inp->DroppedDown = true;
				cp_inp->ItemIndex	= 0;
			}
		}
		else if (cp_down->ItemIndex>0)
			cp_down->ItemIndex = cp_down->ItemIndex - 1;
	}
	//�󋵈ˑ��w���v
	else if (equal_F1(KeyStr)) {
		UnicodeString kwd;
		if (SubComboBox->DroppedDown && SubComboBox->ItemIndex!=-1)
			kwd = get_tkn(SubComboBox->Items->Strings[SubComboBox->ItemIndex], ' ');
		if (kwd.IsEmpty()) kwd = get_tkn(get_CmdStr(CmdsComboBox->Text), ':');

		UnicodeString topic;
		if (kwd.IsEmpty() || starts_AT(kwd) || starts_Dollar(kwd)) {
			topic.UCAT_T(HELPTOPIC_FL);
			topic.cat_sprintf(_T("#%s"), IsRef? _T("CopyCmdName") : _T("InputCommands"));
		}
		else {
			if (SubCmdList->IndexOf(kwd)!=-1)
				topic.UCAT_T(HELPTOPIC_XC);		//�⏕�R�}���h
			else if ((IsRef && ModeTabControl->TabIndex==1) || (!IsRef && ScrMode==SCMD_TVIEW))
				topic.UCAT_T(HELPTOPIC_TV);		//�e�L�X�g�r���A�[
			else if ((IsRef && ModeTabControl->TabIndex==2) || (!IsRef && ScrMode==SCMD_IVIEW))
				topic.UCAT_T(HELPTOPIC_IV);		//�C���[�W�r���A�[
			else if (IsRef && ModeTabControl->TabIndex==3)
				topic.UCAT_T(HELPTOPIC_CILW);	//���O
			else
				topic.UCAT_T(HELPTOPIC_FL);		//�t�@�C���[

			if (!contains_s(topic, _T('#')) && !kwd.IsEmpty()) topic.cat_sprintf(_T("#%s"), kwd.c_str());
		}
		HtmlHelpTopic(topic.c_str());
	}
	else handled = false;

	if		(handled) Key = 0;
	else if (CmdsComboBox->DroppedDown) CmdsComboBox->DroppedDown = false;
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::CmdsComboBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key==VK_RETURN || Key==VK_ESCAPE ||
		(is_KeyDown(VK_CONTROL) && Key!=0x03 && Key!=0x04 && Key!=0x16))
			Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::CmdsComboBoxChange(TObject *Sender)
{
	Filter();
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::CmdsComboBoxSelect(TObject *Sender)
{
	SubComboBox->Enabled = false;
}

//---------------------------------------------------------------------------
//���̕`��
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::SubComboBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TCanvas *cv = SubComboBox->Canvas;
	cv->FillRect(Rect);

	int x = Rect.Left + 2;
	int y = Rect.Top + get_TopMargin(cv);
	UnicodeString lbuf = SubComboBox->Items->Strings[Index];

	cv->TextOut(x, y, split_tkn(lbuf, ' '));
	x += abs(cv->Font->Height) * 16;	//***
	cv->TextOut(x, y, Trim(lbuf));
}

//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::SubComboBoxClick(TObject *Sender)
{
	UnicodeString wd = get_tkn(SubComboBox->Text, ' ');
	UnicodeString lbuf = CmdsComboBox->Text;
	int p = pos_r_q_colon(lbuf); 
	lbuf = (p>0)? lbuf.SubString(1, p).UCAT_T(" ") : EmptyStr;
	lbuf += wd;

	CmdsComboBox->SetFocus();
	CmdsComboBox->Text		= lbuf;
	CmdsComboBox->SelLength = 0;
	CmdsComboBox->SelStart	= lbuf.Length();
}
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::SubComboBoxCloseUp(TObject *Sender)
{
	SubComboBox->Enabled = false;
}

//---------------------------------------------------------------------------
//�t�B���^
//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::Filter()
{
	UnicodeString kwd = CmdsComboBox->Text;

	if (InhFilter) {
		InhFilter = false;
		if (kwd.IsEmpty()) {
			CmdsComboBox->DroppedDown = false;
			SubComboBox->DroppedDown  = false;
		}
		return;
	}

	int p = pos_r_q_colon(kwd); 
	if (p>0) kwd = Trim(kwd.Delete(1, p));
	if (contains_s(kwd, _T('_'))) kwd = EmptyStr;

	std::unique_ptr<TStringList> lst(new TStringList());
	if (!kwd.IsEmpty()) {
		lst->Assign(ItemList);
		if (!contained_wd_i(_T("*|?| "), kwd)) {
			UnicodeString ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Checked, kwd);
			if (!ptn.IsEmpty()) {
				TRegExOptions opt; opt << roIgnoreCase;
				int i=0;
				while (i<lst->Count) if (!TRegEx::IsMatch(lst->Strings[i], ptn, opt)) lst->Delete(i); else i++;
			}
		}
	}

	if (lst->Count>0) {
		//���R���{�{�b�N�X�Ɋ��蓖��
		cursor_Default();
		set_RedrawOff(SubComboBox);
		{
			SubComboBox->DroppedDown = false;
			SubComboBox->Items->Assign(lst.get());
			SubComboBox->DroppedDown = true;
			SubComboBox->ItemIndex	 = 0;
			SubComboBox->Enabled	 = true;
		}
		set_RedrawOn(SubComboBox);
		Screen->Cursor = crArrow;	//���h���b�v�_�E�����ɃJ�[�\���������錻�ۂւ̑΍�
	}
	else {
		SubComboBox->DroppedDown = false;
		SubComboBox->Clear();
	}

	CmdsComboBox->SetFocus();
	if (!kwd.IsEmpty()) {
		CmdsComboBox->SelLength = 0;
		CmdsComboBox->SelStart  = CmdsComboBox->Text.Length();
	}
}

//---------------------------------------------------------------------------
void __fastcall TInpCmdsDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (SpecialKeyProc(this, Key, Shift)) {
		CmdsComboBox->DroppedDown = false;
		SubComboBox->DroppedDown  = false;
	}
}
//---------------------------------------------------------------------------

