//----------------------------------------------------------------------//
// NyanFi																//
//  ƒA[ƒJƒCƒuì¬ƒ_ƒCƒAƒƒO											//
//----------------------------------------------------------------------//
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

	SetDarkWinTheme(this);
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
			_T("–³ˆ³k\n")
			_T("ˆ³kƒŒƒxƒ‹1\n")
			_T("ˆ³kƒŒƒxƒ‹3\n")
			_T("ˆ³kƒŒƒxƒ‹5 (ƒfƒtƒHƒ‹ƒg)\n")
			_T("ˆ³kƒŒƒxƒ‹7\n")
			_T("ˆ³kƒŒƒxƒ‹9\n"));
		ParamComboBox->Enabled = true;

		//ZIP
		if (FormatRadioGroup->ItemIndex==0) {
			ParamComboBox->ItemIndex = (usr_ARC->ZipPrm_x + 1)/2;
			SfxCheckBox->Enabled  = false;
			PasswordEdit->Color   = scl_Window;
			PasswordEdit->Enabled = true;
			fext = ".zip";
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
		fext = ".lzh";
		ExSwEdit->Text = usr_ARC->ExSw_Lha;
		break;

	case 3:	//CAB
		set_ComboBoxText(ParamComboBox,
			_T("MSZIPŒ`Ž® (ƒfƒtƒHƒ‹ƒg)\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹15\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹16\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹17\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹18\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹19\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹20\n")
			_T("LZXŒ`Ž® ˆ³kƒŒƒxƒ‹21\n"));
		ParamComboBox->Enabled	 = true;
		ParamComboBox->ItemIndex = (usr_ARC->CabPrm_z>=15 && usr_ARC->CabPrm_z<=21)? usr_ARC->CabPrm_z - 14 : 0;
		fext = ".cab";
		ExSwEdit->Text = usr_ARC->ExSw_Cab;
		break;

	case 4:	//TAR
		set_ComboBoxText(ParamComboBox,
			_T("–³ˆ³k\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip1\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip2\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip3\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip4\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip5\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip6 (ƒfƒtƒHƒ‹ƒg)\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip7\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip8\n")
			_T("ˆ³kƒŒƒxƒ‹ gzip9\n"));
		ParamComboBox->Enabled	 = true;
		ParamComboBox->ItemIndex = usr_ARC->TarPrm_z;
		fext = ".tar";
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
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "Alt+F"))	{
		FormatRadioGroup->SetFocus();
		if (FormatRadioGroup->ItemIndex!=-1)
			FormatRadioGroup->Buttons[FormatRadioGroup->ItemIndex]->SetFocus();
	}
	else if (USAME_TI(KeyStr, "Alt+O"))	{
		ParamComboBox->SetFocus();
	}
	else {
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#Pack"));
	}
}
//---------------------------------------------------------------------------
