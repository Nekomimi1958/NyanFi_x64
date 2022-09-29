//----------------------------------------------------------------------//
// NyanFi																//
//  バックアップダイアログ												//
//----------------------------------------------------------------------//
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "BakDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBackupDlg *BackupDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TBackupDlg::TBackupDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::FormCreate(TObject *Sender)
{
	SetupComboBox->Tag = CBTAG_HAS_POP;
	UserModule->SetUsrPopupMenu(this);

	BakIncMaskComboBox->Tag = CBTAG_HISTORY;
	BakExcMaskComboBox->Tag = CBTAG_HISTORY;

	UnicodeString hntstr = LoadUsrMsg(USTR_HintMltSepSC);
	BakExcMaskComboBox->Hint = hntstr;
	BakIncMaskComboBox->Hint = hntstr;
	BakSkipDirEdit->Hint	 = hntstr;
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	IniFile->LoadPosInfo(this, DialogCenter);

	IniFile->LoadComboBoxItems(BakIncMaskComboBox, _T("BackupIncMaskHistory"));
	BakIncMaskComboBox->Text = IniFile->ReadStrGen(_T("BackupIncMask"));
	IniFile->LoadComboBoxItems(BakExcMaskComboBox, _T("BackupExcMaskHistory"));
	BakExcMaskComboBox->Text = IniFile->ReadStrGen(_T("BackupExcMask"));

	BakSkipDirEdit->Text	 = IniFile->ReadStrGen(_T("BackupSkipDir"));
	SubDirCheckBox->Checked  = IniFile->ReadBoolGen(_T("BackupSubDir"));
	MirrorCheckBox->Checked  = IniFile->ReadBoolGen(_T("BackupMirror"));
	SyncCheckBox->Checked	 = IniFile->ReadBoolGen(_T("BackupSync"));
	SureCheckBox->Checked	 = IniFile->ReadBoolGen(_T("BackupSureStart"), true);

	SetupComboBox->Clear();
	for (int i=0; i<BakSetupList->Count; i++) SetupComboBox->Items->Add(BakSetupList->Names[i]);
	SetupComboBox->ItemIndex = IniFile->ReadIntGen(_T("BakupSetupIdx"));
	SetupComboBoxClick(SetupComboBox);

	BakIncMaskComboBox->SetFocus();
	BakIncMaskComboBox->SelStart = BakIncMaskComboBox->Text.Length();

	std::unique_ptr<TStringList> dst_lst(new TStringList());
	get_SyncDirList(DstDirEdit->Text, dst_lst.get());
	UnicodeString s = "(同期ディレクトリ: ";
	if (dst_lst->Count>1)
		s += IntToStr(dst_lst->Count - 1);
	else
		s += "なし";
	s += ")";
	SyncLabel->Caption = s;

	SetDarkWinTheme(this);
	Shape1->Pen->Color = IsDarkMode? dcl_BtnShadow : scl_BtnShadow;
	Shape2->Pen->Color = IsDarkMode? dcl_BtnHighlight : scl_BtnHighlight;
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (ModalResult==mrOk) {
		add_ComboBox_history(BakIncMaskComboBox);
		add_ComboBox_history(BakExcMaskComboBox);
	}

	IniFile->SaveComboBoxItems(BakIncMaskComboBox, _T("BackupIncMaskHistory"));
	IniFile->SaveComboBoxItems(BakExcMaskComboBox, _T("BackupExcMaskHistory"));

	IniFile->WriteStrGen(_T("BackupIncMask"),	BakIncMaskComboBox->Text);
	IniFile->WriteStrGen(_T("BackupExcMask"),	BakExcMaskComboBox->Text);
	IniFile->WriteStrGen(_T("BackupSkipDir"),	BakSkipDirEdit);
	IniFile->WriteBoolGen(_T("BackupSubDir"),	SubDirCheckBox);
	IniFile->WriteBoolGen(_T("BackupMirror"),	MirrorCheckBox);
	IniFile->WriteBoolGen(_T("BackupSync"),		SyncCheckBox);
	IniFile->WriteBoolGen(_T("BackupSureStart"),SureCheckBox);
	IniFile->WriteIntGen(_T("BakupSetupIdx"),	SetupComboBox);
}

//---------------------------------------------------------------------------
void __fastcall TBackupDlg::SetupComboBoxClick(TObject *Sender)
{
	int idx = SetupComboBox->ItemIndex;
	if (idx>=0 && idx<BakSetupList->Count) {
		TStringDynArray itm_buf  = get_csv_array(BakSetupList->ValueFromIndex[idx], 7, true);
		BakIncMaskComboBox->Text = itm_buf[0];
		BakExcMaskComboBox->Text = itm_buf[1];
		BakSkipDirEdit->Text	 = itm_buf[2];
		SubDirCheckBox->Checked  = equal_1(itm_buf[3]);
		MirrorCheckBox->Checked  = equal_1(itm_buf[4]);
		SyncCheckBox->Checked	 = equal_1(itm_buf[5]);
		BakDateCondEdit->Text	 = itm_buf[6];
	}
}

//---------------------------------------------------------------------------
//設定の保存
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::SaveSetupActionExecute(TObject *Sender)
{
	TComboBox *cp = SetupComboBox;
	UnicodeString snam = cp->Text;
	UnicodeString lbuf;
	lbuf.sprintf(_T("%s=%s,%s,%s,%s,%s,%s,%s"),
		snam.c_str(),
		make_csv_str(BakIncMaskComboBox->Text).c_str(),
		make_csv_str(BakExcMaskComboBox->Text).c_str(),
		make_csv_str(BakSkipDirEdit->Text).c_str(),
		make_csv_str(SubDirCheckBox->Checked).c_str(),
		make_csv_str(MirrorCheckBox->Checked).c_str(),
		make_csv_str(SyncCheckBox->Checked).c_str(),
		make_csv_str(BakDateCondEdit->Text).c_str());

	int idx = cp->Items->IndexOf(snam);
	if (idx!=-1) {
		BakSetupList->Strings[idx] = lbuf;
		cp->Items->Strings[idx] = snam;
		cp->ItemIndex = idx;
	}
	else {
		BakSetupList->Insert(0, lbuf);
		cp->Items->Insert(0, snam);
	}
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::SaveSetupActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !SetupComboBox->Text.IsEmpty();

	BakSkipDirEdit->Enabled = SubDirCheckBox->Checked;
}
//---------------------------------------------------------------------------
//この項目を削除
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::DelSetupActionExecute(TObject *Sender)
{
	TComboBox *cp = SetupComboBox;
	int idx = cp->Items->IndexOf(cp->Text);
	if (idx!=-1) {
		BakSetupList->Delete(idx);
		cp->Items->Delete(idx);
	}
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::DelSetupActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (SetupComboBox->Items->IndexOf(SetupComboBox->Text) != -1);
}

//---------------------------------------------------------------------------
//コマンドファイルとして保存
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::MakeNbtActionExecute(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("コマンドファイルとして保存"), F_FILTER_NBT, NULL, CmdFilePath);
	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) {
		CmdFilePath = ExtractFilePath(fnam);

		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Text =
			";バックアップ\r\n"
			"PushDir\r\n"
			"PushDir_OP\r\n"
			"ChangeDir_\"" + SrcDirEdit->Text + "\"\r\n"
			"ChangeOppDir_\"" + DstDirEdit->Text + "\"\r\n"
			"BackUp_\"" + SetupComboBox->Text + "\"\r\n"
			"PopDir_OP\r\n"
			"PopDir\r\n";

		if (!saveto_TextUTF8(fnam, fbuf.get())) msgbox_ERR(USTR_FaildSave);
	}
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::MakeNbtActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !SetupComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//開始
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::OkButtonClick(TObject *Sender)
{
	if (SureCheckBox->Checked) {
		UnicodeString msg = "バックアップを開始しますか?\r\n\r\n";
		msg.cat_sprintf(_T("バックアップ元: %s\r\n"), SrcDirEdit->Text.c_str());
		msg.cat_sprintf(_T("バックアップ先: %s\r\n"), DstDirEdit->Text.c_str());
		if (SyncCheckBox->Checked) {
			std::unique_ptr<TStringList> dst_lst(new TStringList());
			get_SyncDirList(DstDirEdit->Text, dst_lst.get());
			for (int i=1; i<dst_lst->Count; i++) {
				msg.cat_sprintf(_T("同期先%u: %s\r\n"), i, dst_lst->Strings[i].c_str());
			}
		}
		if (msgbox_Sure(msg)) ModalResult = mrOk;
	}
	else {
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TBackupDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

