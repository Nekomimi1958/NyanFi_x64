//----------------------------------------------------------------------//
// NyanFi																//
//  �摜�ϊ��_�C�A���O													//
//----------------------------------------------------------------------//
#pragma hdrstop
#include <tchar.h>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "CvImgDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCvImageDlg *CvImageDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TCvImageDlg::TCvImageDlg(TComponent* Owner)
	: TForm(Owner)
{
	fromClip = false;
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormCreate(TObject *Sender)
{
	set_ComboBoxText(ScaleModeComboBox,
		_T("�k���E�g����s��Ȃ�\n")
		_T("�{�����p�[�Z���g�Ŏw��\n")
		_T("�c���̒������̃T�C�Y���w��\n")
		_T("���T�C�Y���w��\n")
		_T("�c�T�C�Y���w��\n")
		_T("�w��T�C�Y���Ɏ��߂�\n")
		_T("�w��T�C�Y�ɃX�g���b�`\n")
		_T("�w��T�C�Y�ɗ]���t���Ŏ��߂�\n")
		_T("�w��T�C�Y�ɍ��킹�Đ؂�o��\n"));

	set_ComboBoxText(YCrCbComboBox,
		_T("�f�t�H���g\n4:2:0\n4:2:2\n4:4:4\n"));
	set_ComboBoxText(CmpModeComboBox,
		_T("�����I��\n���k�Ȃ�\nCCITT3���k\nCCITT4���k\nLZW���k\nRLE���k\nZIP���k\nLZWH�������k\n"));
	set_ComboBoxText(ScaleOptComboBox,
		_T("�j�A���X�g�l�C�o�[\n�o�C���j�A\n�o�C�L���[�r�b�N\n�t�@���g���T���v�����O\n��Ԃ��Ȃ�\n"));
	set_ComboBoxText(ChgNameComboBox,
		_T("�t�@�C�����̐擪�ɑ}��\n�t�@�C�����啔�̖����ɒǉ�\n"));

	ClipNameComboBox->Tag = CBTAG_HISTORY;
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	if (fromClip) {
		Caption = "�N���b�v�{�[�h�摜�̕ϊ�/�ۑ�";
		SubPanel->Visible  = false;
		NamePanel->Visible = true;
		SttLabel->Caption  = EmptyStr;
		ClientHeight = MainPanel->Height + NamePanel->Height + BtnPanel->Height;

		IniFile->LoadComboBoxItems(ClipNameComboBox, _T("CvClpNameHistory"));
		ClipNameComboBox->Text = IniFile->ReadStrGen(_T("CvClpFileName"));
		ClipNameComboBox->SetFocus();
		ClipNameComboBox->SelStart = ClipNameComboBox->Text.Length();
		(IniFile->ReadBoolGen(_T("CvClpAutoRen"))? ClipAutoBtn : ClipOWBtn)->Checked = true;
	}
	else {
		Caption = "�摜�t�@�C���̕ϊ�";
		SubPanel->Visible  = true;
		NamePanel->Visible = false;
		ClientHeight = MainPanel->Height + SubPanel->Height + BtnPanel->Height;

		NotUsePrvCheckBox->Checked = IniFile->ReadBoolGen(_T("CvImgNotUsePreview"));
		ChgNameComboBox->ItemIndex = IniFile->ReadIntGen(_T("CvImgChgNameMode"),	1);
		ChgNameEdit->Text		   = IniFile->ReadStrGen(_T("CvImgCngNameStr"));
		KeepTimeCheckBox->Checked  = IniFile->ReadBoolGen(_T("CVImgKeepTime"));
	}

	UnicodeString pfx = fromClip? "CvClp" : "CvImg";
	CvFmtRadioGroup->ItemIndex	 = IniFile->ReadIntGen( (pfx + "Format").c_str());
	ImgQTrackBar->Position		 = IniFile->ReadIntGen( (pfx + "JpgQuality").c_str(),	80);
	YCrCbComboBox->ItemIndex	 = IniFile->ReadIntGen( (pfx + "JpgYCrCb").c_str(),		1);
	CmpModeComboBox->ItemIndex	 = IniFile->ReadIntGen( (pfx + "TifCmpMode").c_str());
	GrayScaleCheckBox->Checked	 = IniFile->ReadBoolGen((pfx + "GrayScale").c_str());
	ScaleModeComboBox->ItemIndex = IniFile->ReadIntGen( (pfx + "ScaleMode").c_str());
	ScalePrm1Edit->Text 		 = IniFile->ReadIntGen( (pfx + "ScalePrm1").c_str(),	100);
	ScalePrm2Edit->Text 		 = IniFile->ReadIntGen( (pfx + "ScalePrm2").c_str(),	100);
	ScaleOptComboBox->ItemIndex  = IniFile->ReadIntGen( (pfx + "ScaleOpt").c_str());
	MgnColPanel->Color	 = (TColor)IniFile->ReadIntGen( (pfx + "MgnColor").c_str(),		clBlack);

	ScaleModeComboBoxChange(NULL);
	CvFmtRadioGroupClick(NULL);
	if (fromClip) ClipNameComboBoxChange(NULL);

	SetDarkWinTheme(this);
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (fromClip) {
		if (ModalResult==mrOk) add_ComboBox_history(ClipNameComboBox);
		IniFile->SaveComboBoxItems(ClipNameComboBox, _T("CvClpNameHistory"));
		IniFile->WriteStrGen( _T("CvClpFileName"),	ClipNameComboBox->Text);
		IniFile->WriteBoolGen(_T("CvClpAutoRen"),	ClipAutoBtn->Checked);
	}
	else {
		IniFile->WriteBoolGen(_T("CvImgNotUsePreview"),	NotUsePrvCheckBox->Checked);
		IniFile->WriteIntGen( _T("CvImgChgNameMode"),	ChgNameComboBox->ItemIndex);
		IniFile->WriteStrGen( _T("CvImgCngNameStr"),	ChgNameEdit->Text);
		IniFile->WriteBoolGen(_T("CVImgKeepTime"),		KeepTimeCheckBox->Checked);
	}

	UnicodeString pfx = fromClip? "CvClp" : "CvImg";
	IniFile->WriteIntGen( (pfx + "Format").c_str(), 	CvFmtRadioGroup);
	IniFile->WriteIntGen( (pfx + "JpgQuality").c_str(),	ImgQTrackBar->Position);
	IniFile->WriteIntGen( (pfx + "JpgYCrCb").c_str(),	YCrCbComboBox);
	IniFile->WriteIntGen( (pfx + "TifCmpMode").c_str(),	CmpModeComboBox);
	IniFile->WriteBoolGen((pfx + "GrayScale").c_str(),	GrayScaleCheckBox);
	IniFile->WriteIntGen( (pfx + "ScaleMode").c_str(),	ScaleModeComboBox);
	IniFile->WriteIntGen( (pfx + "ScalePrm1").c_str(),	EditToInt(ScalePrm1Edit, 100));
	IniFile->WriteIntGen( (pfx + "ScalePrm2").c_str(),	EditToInt(ScalePrm2Edit, 100));
	IniFile->WriteIntGen( (pfx + "ScaleOpt").c_str(),	ScaleOptComboBox);
	IniFile->WriteIntGen( (pfx + "MgnColor").c_str(),	MgnColPanel->Color);

	fromClip = false;
}

//---------------------------------------------------------------------------
//�o�͌`���̕ύX
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::CvFmtRadioGroupClick(TObject *Sender)
{
	int idx = CvFmtRadioGroup->ItemIndex;
	ImgQLabel->Visible		 = idx==1 || idx==5;
	ImgQTrackBar->Visible	 = idx==1 || idx==5;
	YCrCbLabel->Visible 	 = idx==1;
	YCrCbComboBox->Visible	 = idx==1;
	YCrCbComboBox->Enabled	 = !GrayScaleCheckBox->Checked;
	CmpModeLabel->Visible	 = idx==4;
	CmpModeComboBox->Visible = idx==4;

	switch (CvFmtRadioGroup->ItemIndex) {
	case 0: FextLabel->Caption = ".bmp";	break;
	case 1: FextLabel->Caption = ".jpg";	break;
	case 2: FextLabel->Caption = ".png";	break;
	case 3: FextLabel->Caption = ".gif";	break;
	case 4: FextLabel->Caption = ".tif";	break;
	case 5: FextLabel->Caption = ".hdp";	break;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::ImgQTrackBarChange(TObject *Sender)
{
	ImgQLabel->Caption = UnicodeString().sprintf(_T("�i�� %3u"), ImgQTrackBar->Position);
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::GrayScaleCheckBoxClick(TObject *Sender)
{
	YCrCbComboBox->Enabled = !GrayScaleCheckBox->Checked;
}

//---------------------------------------------------------------------------
//�]���F�̎Q��
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::RefMgnColBtnClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = MgnColPanel->Color;
	if (UserModule->ColorDlg->Execute()) MgnColPanel->Color = UserModule->ColorDlg->Color;
}

//---------------------------------------------------------------------------
//�k���E�g����@��ύX
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::ScaleModeComboBoxChange(TObject *Sender)
{
	ScaleOptComboBox->Enabled = (ScaleModeComboBox->ItemIndex>0);
	RefMgnColBtn->Enabled	  = Enabled;

	UnicodeString lbl1, lbl2;

	switch (ScaleModeComboBox->ItemIndex) {
	case 1: lbl1 = "�{����";	break;
	case 2: lbl1 = "�T�C�Y";	break;
	case 3: lbl1 = "���T�C�Y";	break;
	case 4: lbl1 = "�c�T�C�Y";	break;
	case 5: case 6: case 7: case 8:
		lbl1 = "���T�C�Y";
		lbl2 = "�c�T�C�Y";
		break;
	}

	ScalePrm1Edit->Enabled = !lbl1.IsEmpty();
	ScalePrm2Edit->Enabled = !lbl2.IsEmpty();

	ScalePrm1Edit->EditLabel->Caption = lbl1;
	ScalePrm2Edit->EditLabel->Caption = lbl2;
}

//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::ClipNameComboBoxChange(TObject *Sender)
{
	if (fromClip) {
		SttLabel->Caption =
			file_exists(DistPath + ClipNameComboBox->Text + FextLabel->Caption)?
				"��������" : "";
	}
}

//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+B")) CvFmtRadioGroup->ItemIndex = 0;
	else if (USAME_TI(KeyStr, "Alt+J")) CvFmtRadioGroup->ItemIndex = 1;
	else if (USAME_TI(KeyStr, "Alt+P")) CvFmtRadioGroup->ItemIndex = 2;
	else if (USAME_TI(KeyStr, "Alt+G")) CvFmtRadioGroup->ItemIndex = 3;
	else if (USAME_TI(KeyStr, "Alt+T")) CvFmtRadioGroup->ItemIndex = 4;
	else if (USAME_TI(KeyStr, "Alt+H")) CvFmtRadioGroup->ItemIndex = 5;
	else if (USAME_TI(KeyStr, "Alt+S")) invert_CheckBox(GrayScaleCheckBox);
	
	else SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

