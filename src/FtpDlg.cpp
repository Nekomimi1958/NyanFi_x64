//----------------------------------------------------------------------//
// NyanFi																//
//  FTP接続ダイアログ													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "FtpDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFtpConnectDlg *FtpConnectDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TFtpConnectDlg::TFtpConnectDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::FormCreate(TObject *Sender)
{
	HostList = new TStringList();

	UserModule->SetUsrPopupMenu(this);

	set_ComboBoxText(SSLComboBox,
		_T("暗号化しない\n")
		_T("Explicit TLS\n")
		_T("Implicit TLS\n"));
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);

	TListBox *lp = HostListBox;
	set_ListBoxItemHi(lp);

	HostList->Clear();
	IniFile->LoadListItems("FtpHostList", HostList, 20, false);
	lp->Items->Assign(HostList);
	if (lp->Count>0) lp->ItemIndex = IniFile->ReadIntGen(_T("FtpDlgHostIdx"),	0);

	UserModule->InitializeListBox(lp);

	HostNameEdit->Text	   = EmptyStr;
	HostAddrEdit->Text	   = EmptyStr;
	UserIdEdit->Text	   = EmptyStr;
	PassWdEdit->Text	   = EmptyStr;
	HostDirEdit->Text	   = EmptyStr;
	LocalDirEdit->Text	   = EmptyStr;
	PasvCheckBox->Checked  = true;
	AnonyCheckBox->Checked = false;
	SSLComboBox->ItemIndex = -1;
	HostItem			   = EmptyStr;

	RemoteRadioGroup->ItemIndex = FTPRemoteSide;
	TextFExtEdit->Text			= FTPTextModeFExt;
	DisconTimeEdit->Text		= FTPDisconTimeout;
	DlKeepTimeCheckBox->Checked = FTPDlKeepTime;
	UpKeepTimeCheckBox->Checked = FTPUpKeepTime;
	UpToLowerCheckBox->Checked	= FTPUpToLower;
	LogResCheckBox->Checked 	= FTPLogResponse;
	SndConnectEdit->Text		= FTPSndConnect;
	SndDisconEdit->Text 		= FTPSndDiscon;
	SndTransferEdit->Text		= FTPSndTransfer;

	PageControl1->ActivePage = HostSheet;
	HostListBox->SetFocus();
	HostListBoxClick(HostListBox);
}

//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	FTPRemoteSide	 = RemoteRadioGroup->ItemIndex;
	FTPTextModeFExt  = TextFExtEdit->Text;
	FTPDisconTimeout = EditToInt(DisconTimeEdit, 300);
	FTPDlKeepTime	 = DlKeepTimeCheckBox->Checked;
	FTPUpKeepTime	 = UpKeepTimeCheckBox->Checked;
	FTPUpToLower	 = UpToLowerCheckBox->Checked;
	FTPLogResponse	 = LogResCheckBox->Checked;
	FTPSndConnect	 = SndConnectEdit->Text;
	FTPSndDiscon	 = SndDisconEdit->Text;
	FTPSndTransfer	 = SndTransferEdit->Text;

	IniFile->SavePosInfo(this);

	HostList->Assign(HostListBox->Items);
	IniFile->SaveListItems("FtpHostList", HostList, 20);
	IniFile->WriteIntGen(_T("FtpDlgHostIdx"), HostListBox->ItemIndex);
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::FormDestroy(TObject *Sender)
{
	delete HostList;
}

//---------------------------------------------------------------------------
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::HostListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv  = lp->Canvas;
	cv->Font->Assign(lp->Font);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);
	cv->TextOut(xp, yp, get_csv_item(lp->Items->Strings[Index], 0));
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::HostListBoxClick(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	if (lp->Count==0) return;

	if (lp->ItemIndex==-1) lp->ItemIndex = 0;
	int idx = lp->ItemIndex;
	UnicodeString lbuf = lp->Items->Strings[idx];
	TStringDynArray itm_buf = get_csv_array(lbuf, 7, true);
	HostNameEdit->Text = itm_buf[0];
	HostAddrEdit->Text = itm_buf[1];
	UserIdEdit->Text   = itm_buf[2];
	PassWdEdit->Text   = uncipher(itm_buf[3]);
	HostDirEdit->Text  = itm_buf[4];
	LocalDirEdit->Text = itm_buf[5];

	SSLComboBox->ItemIndex  = ContainsText(itm_buf[6], "EXPLICIT")? 1 : ContainsText(itm_buf[6], "IMPLICIT")? 2 : 0;
	SyncLRCheckBox->Checked = ContainsText(itm_buf[6], "SyncLR");
	PasvCheckBox->Checked	= !ContainsText(itm_buf[6], "PORT");
	AnonyCheckBox->Checked	= USAME_TI(UserIdEdit->Text, "anonymous");
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::HostListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::AnonyCheckBoxClick(TObject *Sender)
{
	if (AnonyCheckBox->Checked) UserIdEdit->Text = "anonymous";
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TFtpConnectDlg::MakeHostItem()
{
	UnicodeString optstr;
	if (!PasvCheckBox->Checked)	 optstr.UCAT_T("PORT;");

	if		(SSLComboBox->ItemIndex==1) optstr.UCAT_T("EXPLICIT;");
	else if (SSLComboBox->ItemIndex==2) optstr.UCAT_T("IMPLICIT;");

	if (SyncLRCheckBox->Checked) optstr.UCAT_T("SyncLR;");

	return UnicodeString().sprintf(_T("%s,%s,%s,%s,%s,%s,%s"),
			make_csv_str(HostNameEdit->Text).c_str(),
			make_csv_str(HostAddrEdit->Text).c_str(),
			make_csv_str(UserIdEdit->Text).c_str(),
			make_csv_str(cipher(PassWdEdit->Text)).c_str(),
			make_csv_str(HostDirEdit->Text).c_str(),
			make_csv_str(LocalDirEdit->Text).c_str(),
			make_csv_str(optstr).c_str());
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::AddHostActionExecute(TObject *Sender)
{
	int idx = HostListBox->Items->Add(MakeHostItem());
	HostListBox->ItemIndex = idx;
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::ChgHostActionExecute(TObject *Sender)
{
	int idx = HostListBox->ItemIndex;  if (idx==-1) return;
	HostListBox->Items->Strings[idx] = MakeHostItem();
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::HostItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= !HostNameEdit->Text.IsEmpty() && !HostAddrEdit->Text.IsEmpty() &&
		  !UserIdEdit->Text.IsEmpty() && (AnonyCheckBox->Checked || !PassWdEdit->Text.IsEmpty());
}

//---------------------------------------------------------------------------
//ローカル開始ディレクトリの選択
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = LocalDirEdit->Text;
	if (UserModule->SelectDirEx(_T("ローカル開始ディレクトリ"), dnam)) LocalDirEdit->Text = dnam;
}

//---------------------------------------------------------------------------
//接続
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::ConnectActionExecute(TObject *Sender)
{
	TListBox *lp = HostListBox;
	if (lp->ItemIndex!=-1) HostItem = lp->Items->Strings[lp->ItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::ConnectActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (HostListBox->ItemIndex!=-1);

	UserIdEdit->Enabled 	 = !AnonyCheckBox->Checked;
	PassWdEdit->PasswordChar = !AnonyCheckBox->Checked? '*' : '\0';
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::HostListBoxDblClick(TObject *Sender)
{
	ConnectAction->Execute();
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#FtpConnect"));
}

//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::RefSoundBtnClick(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;

	UnicodeString inidir = def_if_empty(
		ExtractFileDir(to_absolute_name((tag==0)? SndConnectEdit->Text : (tag==1)? SndDisconEdit->Text : SndTransferEdit->Text)), ExePath);
	UserModule->PrepareOpenDlg(_T("サウンドファイルの指定"), F_FILTER_WAV, NULL, inidir);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam, true)) {
		switch (tag) {
		case 0: SndConnectEdit->Text  = fnam;	break;
		case 1: SndDisconEdit->Text   = fnam;	break;
		case 2: SndTransferEdit->Text = fnam;	break;
		}
		play_sound(UserModule->OpenDlg->FileName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFtpConnectDlg::TestSoundBtnClick(TObject *Sender)
{
	switch (((TComponent*)Sender)->Tag) {
	case 0: play_sound(SndConnectEdit->Text);	break;
	case 1: play_sound(SndDisconEdit->Text);	break;
	case 2: play_sound(SndTransferEdit->Text);	break;
	}
}
//---------------------------------------------------------------------------
