//----------------------------------------------------------------------//
// NyanFi																//
//  アーカイブ作成ダイアログ											//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include "Global.h"
#include "usr_arc.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "PackDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPackArcDlg *PackArcDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TPackArcDlg::TPackArcDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	usr_ARC->ZipPrm_x	= IniFile->ReadIntGen(_T("ZipPrm_x"),	5);
	usr_ARC->SevenPrm_x	= IniFile->ReadIntGen(_T("SevenPrm_x"),	5);
	usr_ARC->ZipPrm_sfx = IniFile->ReadBoolGen(_T("ZipPrm_sfx"));
	usr_ARC->TarPrm_z	= IniFile->ReadIntGen(_T("TarPrm_z"),	6);
	usr_ARC->CabPrm_z	= IniFile->ReadIntGen(_T("CabPrm_z"),	0);

	usr_ARC->ExSw_Zip	= IniFile->ReadStrGen(_T("ExSw_Zip"));
	usr_ARC->ExSw_7z	= IniFile->ReadStrGen(_T("ExSw_7z"));
	usr_ARC->ExSw_Lha 	= IniFile->ReadStrGen(_T("ExSw_Lha"));
	usr_ARC->ExSw_Cab 	= IniFile->ReadStrGen(_T("ExSw_Cab"));
	usr_ARC->ExSw_Tar 	= IniFile->ReadStrGen(_T("ExSw_Tar"));

	IncDirCheckBox->Checked   = IniFile->ReadBoolGen(_T("PackArcDlgIncDir"));
	SureSameCheckBox->Checked = IniFile->ReadBoolGen(_T("SureSameArc"),	true);

	int idx = IniFile->ReadIntGen(_T("SameArcMode"));
	SameRadioGroup->ItemIndex = (idx==0 || idx==1)? idx : 0;

	FormatRadioGroup->Buttons[0]->Enabled = usr_ARC->IsAvailable(UARCTYP_ZIP);
	FormatRadioGroup->Buttons[1]->Enabled = usr_ARC->IsAvailable(UARCTYP_7Z);
	FormatRadioGroup->Buttons[2]->Enabled = usr_ARC->IsAvailable(UARCTYP_LHA);
	FormatRadioGroup->Buttons[3]->Enabled = usr_ARC->IsAvailable(UARCTYP_CAB);
	FormatRadioGroup->Buttons[4]->Enabled = usr_ARC->IsAvailable(UARCTYP_TAR);

	idx = IniFile->ReadIntGen(_T("ArcFormat"));
	if (idx<0 || idx>=FormatRadioGroup->Items->Count || !FormatRadioGroup->Buttons[idx]->Enabled) {
		FormatRadioGroup->ItemIndex = 0;
		for (int i=0; i<FormatRadioGroup->Items->Count; i++) {
			if (!FormatRadioGroup->Buttons[i]->Enabled) continue;
			FormatRadioGroup->ItemIndex = i;
			break;
		}
	}
	FormatRadioGroupClick(NULL);

	PerDirCheckBoxClick(NULL);
	if (ArcNameEdit->Enabled) {
		ArcNameEdit->SetFocus();
		ArcNameEdit->SelStart = ArcNameEdit->Text.Length();
	}
}
//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteBoolGen(_T("PackArcDlgIncDir"),	IncDirCheckBox);
	IniFile->WriteBoolGen(_T("SureSameArc"),		SureSameCheckBox);
	IniFile->WriteIntGen(_T("SameArcMode"),			SameRadioGroup->ItemIndex);
	IniFile->WriteIntGen(_T("ArcFormat"),			FormatRadioGroup);

	UnicodeString exsw = ExSwEdit->Text.Trim();
	switch (FormatRadioGroup->ItemIndex) {
	case 0: //ZIP
		usr_ARC->ZipPrm_x	= (ParamComboBox->ItemIndex==0)? 0 : ParamComboBox->ItemIndex*2 - 1;
		IniFile->WriteIntGen(_T("ZipPrm_x"),	usr_ARC->ZipPrm_x);
		usr_ARC->ExSw_Zip	= exsw;
		IniFile->WriteStrGen(_T("ExSw_Zip"),	exsw);
		break;
	case 1: //7Z
		usr_ARC->SevenPrm_x	= (ParamComboBox->ItemIndex==0)? 0 : ParamComboBox->ItemIndex*2 - 1;
		IniFile->WriteIntGen(_T("SevenPrm_x"),	usr_ARC->SevenPrm_x);
		usr_ARC->ZipPrm_sfx = SfxCheckBox->Checked;
		IniFile->WriteBoolGen(_T("ZipPrm_sfx"),	usr_ARC->ZipPrm_sfx);
		usr_ARC->ExSw_7z	= exsw;
		IniFile->WriteStrGen(_T("ExSw_7z"),		exsw);
		break;
	case 2:	//LHA
		usr_ARC->ExSw_Lha	= exsw;
		IniFile->WriteStrGen(_T("ExSw_Lha"),	exsw);
		break;
	case 3:	//CAB
		usr_ARC->CabPrm_z   = (ParamComboBox->ItemIndex==0)? 0 : ParamComboBox->ItemIndex + 14;
		IniFile->WriteIntGen(_T("CabPrm_z"),	usr_ARC->CabPrm_z);
		usr_ARC->ExSw_Cab	= exsw;
		IniFile->WriteStrGen(_T("ExSw_CAB"),	exsw);
		break;
	case 4:	//TAR
		usr_ARC->TarPrm_z	= ParamComboBox->ItemIndex;
		IniFile->WriteIntGen(_T("TarPrm_z"),	usr_ARC->TarPrm_z);
		usr_ARC->ExSw_Tar	= exsw;
		IniFile->WriteStrGen(_T("ExSw_Tar"),	exsw);
		break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::PerDirCheckBoxClick(TObject *Sender)
{
	IncDirCheckBox->Enabled = PerDirCheckBox->Enabled && PerDirCheckBox->Checked;
	ArcNameEdit->Enabled	= !IncDirCheckBox->Enabled;
	if (!ArcNameEdit->Enabled) ArcNameEdit->Text = EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::FormatRadioGroupClick(TObject *Sender)
{
	ParamComboBox->Enabled = false;
	ParamComboBox->Clear();
	SfxCheckBox->Checked   = false;
	SfxCheckBox->Enabled   = false;
	PasswordEdit->Text	   = EmptyStr;
	PasswordEdit->Color    = scl_BtnFace;
	PasswordEdit->Enabled  = false;

	UnicodeString fext;

	switch (FormatRadioGroup->ItemIndex) {
	case 0:	//ZIP
	case 1:	//7Z
		set_ComboBoxText(ParamComboBox,
			_T("無圧縮\n")
			_T("圧縮レベル1\n")
			_T("圧縮レベル3\n")
			_T("圧縮レベル5 (デフォルト)\n")
			_T("圧縮レベル7\n")
			_T("圧縮レベル9\n"));
		ParamComboBox->Enabled = true;

		//ZIP
		if (FormatRadioGroup->ItemIndex==0) {
			ParamComboBox->ItemIndex = (usr_ARC->ZipPrm_x + 1)/2;
			SfxCheckBox->Enabled  = false;
			PasswordEdit->Color   = scl_Window;
			PasswordEdit->Enabled = true;
			fext.USET_T(".zip");
			ExSwEdit->Text = usr_ARC->ExSw_Zip;
		}
		//7Z
		else {
			ParamComboBox->ItemIndex = (usr_ARC->SevenPrm_x + 1)/2;
			SfxCheckBox->Checked  = usr_ARC->ZipPrm_sfx;
			SfxCheckBox->Enabled  = true;
			PasswordEdit->Color   = !usr_ARC->ZipPrm_sfx? scl_Window : scl_BtnFace;
			PasswordEdit->Enabled = !usr_ARC->ZipPrm_sfx;
			fext.sprintf(_T("%s"), usr_ARC->ZipPrm_sfx? _T(".exe") : _T(".7z"));
			ExSwEdit->Text = usr_ARC->ExSw_7z;
		}
		break;

	case 2:	//LHA
		fext.USET_T(".lzh");
		ExSwEdit->Text = usr_ARC->ExSw_Lha;
		break;

	case 3:	//CAB
		set_ComboBoxText(ParamComboBox,
			_T("MSZIP形式 (デフォルト)\n")
			_T("LZX形式 圧縮レベル15\n")
			_T("LZX形式 圧縮レベル16\n")
			_T("LZX形式 圧縮レベル17\n")
			_T("LZX形式 圧縮レベル18\n")
			_T("LZX形式 圧縮レベル19\n")
			_T("LZX形式 圧縮レベル20\n")
			_T("LZX形式 圧縮レベル21\n"));
		ParamComboBox->Enabled	 = true;
		ParamComboBox->ItemIndex = (usr_ARC->CabPrm_z>=15 && usr_ARC->CabPrm_z<=21)? usr_ARC->CabPrm_z - 14 : 0;
		fext.USET_T(".cab");
		ExSwEdit->Text = usr_ARC->ExSw_Cab;
		break;

	case 4:	//TAR
		set_ComboBoxText(ParamComboBox,
			_T("無圧縮\n")
			_T("圧縮レベル gzip1\n")
			_T("圧縮レベル gzip2\n")
			_T("圧縮レベル gzip3\n")
			_T("圧縮レベル gzip4\n")
			_T("圧縮レベル gzip5\n")
			_T("圧縮レベル gzip6 (デフォルト)\n")
			_T("圧縮レベル gzip7\n")
			_T("圧縮レベル gzip8\n")
			_T("圧縮レベル gzip9\n"));
		ParamComboBox->Enabled	 = true;
		ParamComboBox->ItemIndex = usr_ARC->TarPrm_z;
		fext.USET_T(".tar");
		ExSwEdit->Text = usr_ARC->ExSw_Tar;
		break;
	}

	FextLabel->Caption = fext;
	OkButton->Enabled  = FormatRadioGroup->Buttons[FormatRadioGroup->ItemIndex]->Enabled;
}

//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::SfxCheckBoxClick(TObject *Sender)
{
	if (FormatRadioGroup->ItemIndex==1) {	//7z
		if (SfxCheckBox->Checked) {
			PasswordEdit->Text	  = EmptyStr;
			PasswordEdit->Color   = scl_BtnFace;
			PasswordEdit->Enabled = false;
			FextLabel->Caption	  = ".exe";
		}
		else {
			PasswordEdit->Color   = scl_Window;
			PasswordEdit->Enabled = true;
			FextLabel->Caption	  = ".7z";
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TPackArcDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#Pack"));
}
//---------------------------------------------------------------------------
