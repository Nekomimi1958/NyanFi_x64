//----------------------------------------------------------------------//
// NyanFi																//
//  画像変換ダイアログ													//
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
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormCreate(TObject *Sender)
{
	set_ComboBoxText(ScaleModeComboBox,
		_T("縮小・拡大を行わない\n")
		_T("倍率をパーセントで指定\n")
		_T("縦横の長い方のサイズを指定\n")
		_T("横サイズを指定\n")
		_T("縦サイズを指定\n")
		_T("指定サイズ内に収める\n")
		_T("指定サイズにストレッチ\n")
		_T("指定サイズに余白付きで収める\n")
		_T("指定サイズに合わせて切り出し\n"));

	set_ComboBoxText(YCrCbComboBox,
		_T("デフォルト\n4:2:0\n4:2:2\n4:4:4\n"));
	set_ComboBoxText(CmpModeComboBox,
		_T("自動選択\n圧縮なし\nCCITT3圧縮\nCCITT4圧縮\nLZW圧縮\nRLE圧縮\nZIP圧縮\nLZWH差分圧縮\n"));
	set_ComboBoxText(ScaleOptComboBox,
		_T("ニアレストネイバー\nバイリニア\nバイキュービック\nファントリサンプリング\n補間しない\n"));
	set_ComboBoxText(ChgNameComboBox,
		_T("ファイル名の先頭に挿入\nファイル名主部の末尾に追加\n"));
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	CvFmtRadioGroup->ItemIndex	 = IniFile->ReadIntGen(_T("CvImgFormat"));
	ImgQTrackBar->Position		 = IniFile->ReadIntGen(_T("CVImgJpgQuality"),	80);
	YCrCbComboBox->ItemIndex	 = IniFile->ReadIntGen(_T("CVImgJpgYCrCb"),	1);
	CmpModeComboBox->ItemIndex	 = IniFile->ReadIntGen(_T("CVImgTifCmpMode"));
	GrayScaleCheckBox->Checked	 = IniFile->ReadBoolGen(_T("CVImgGrayScale"));
	NotUsePrvCheckBox->Checked	 = IniFile->ReadBoolGen(_T("CvImgNotUsePreview"));
	ScaleModeComboBox->ItemIndex = IniFile->ReadIntGen(_T("CvImgScaleMode"));
	ScaleModeComboBoxChange(NULL);
	ScalePrm1Edit->Text 		 = IniFile->ReadIntGen(_T("CvImgScalePrm1"),	100);
	ScalePrm2Edit->Text 		 = IniFile->ReadIntGen(_T("CvImgScalePrm2"),	100);
	ScaleOptComboBox->ItemIndex  = IniFile->ReadIntGen(_T("CvImgScaleOpt"));
	MgnColPanel->Color	 = (TColor)IniFile->ReadIntGen(_T("CvImgMgnColor"), 	clBlack);
	ChgNameComboBox->ItemIndex   = IniFile->ReadIntGen(_T("CvImgChgNameMode"),	1);
	ChgNameEdit->Text			 = IniFile->ReadStrGen(_T("CvImgCngNameStr"));
	KeepTimeCheckBox->Checked	 = IniFile->ReadBoolGen(_T("CVImgKeepTime"));
	CvFmtRadioGroupClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("CvImgFormat"),			CvFmtRadioGroup);
	IniFile->WriteIntGen(_T("CVImgJpgQuality"),		ImgQTrackBar->Position);
	IniFile->WriteIntGen(_T("CVImgJpgYCrCb"),		YCrCbComboBox);
	IniFile->WriteIntGen(_T("CVImgTifCmpMode"),		CmpModeComboBox);
	IniFile->WriteBoolGen(_T("CVImgGrayScale"),		GrayScaleCheckBox);
	IniFile->WriteBoolGen(_T("CvImgNotUsePreview"),	NotUsePrvCheckBox);
	IniFile->WriteIntGen(_T("CvImgScaleMode"),		ScaleModeComboBox);
	IniFile->WriteIntGen(_T("CvImgScalePrm1"),		EditToInt(ScalePrm1Edit, 100));
	IniFile->WriteIntGen(_T("CvImgScalePrm2"),		EditToInt(ScalePrm2Edit, 100));
	IniFile->WriteIntGen(_T("CvImgScaleOpt"),		ScaleOptComboBox);
	IniFile->WriteIntGen(_T("CvImgMgnColor"),		MgnColPanel->Color);
	IniFile->WriteIntGen(_T("CvImgChgNameMode"),	ChgNameComboBox);
	IniFile->WriteStrGen(_T("CvImgCngNameStr"),		ChgNameEdit);
	IniFile->WriteBoolGen(_T("CVImgKeepTime"),		KeepTimeCheckBox);
}

//---------------------------------------------------------------------------
//出力形式の変更
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
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::ImgQTrackBarChange(TObject *Sender)
{
	ImgQLabel->Caption = UnicodeString().sprintf(_T("品質 %3u"), ImgQTrackBar->Position);
}
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::GrayScaleCheckBoxClick(TObject *Sender)
{
	YCrCbComboBox->Enabled = !GrayScaleCheckBox->Checked;
}

//---------------------------------------------------------------------------
//余白色の参照
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::RefMgnColBtnClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = MgnColPanel->Color;
	if (UserModule->ColorDlg->Execute()) MgnColPanel->Color = UserModule->ColorDlg->Color;
}

//---------------------------------------------------------------------------
//縮小・拡大方法を変更
//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::ScaleModeComboBoxChange(TObject *Sender)
{
	ScaleOptComboBox->Enabled = (ScaleModeComboBox->ItemIndex>0);
	RefMgnColBtn->Enabled	  = Enabled;

	UnicodeString lbl1, lbl2;

	switch (ScaleModeComboBox->ItemIndex) {
	case 1: lbl1.USET_T("倍率％");		break;
	case 2: lbl1.USET_T("サイズ");		break;
	case 3: lbl1.USET_T("横サイズ");	break;
	case 4: lbl1.USET_T("縦サイズ");	break;
	case 5: case 6: case 7: case 8:
		lbl1.USET_T("横サイズ");
		lbl2.USET_T("縦サイズ");
		break;
	}

	ScalePrm1Edit->Enabled = !lbl1.IsEmpty();
	ScalePrm2Edit->Enabled = !lbl2.IsEmpty();

	ScalePrm1Edit->EditLabel->Caption = lbl1;
	ScalePrm2Edit->EditLabel->Caption = lbl2;
}

//---------------------------------------------------------------------------
void __fastcall TCvImageDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------
