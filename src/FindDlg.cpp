//----------------------------------------------------------------------//
// NyanFi																//
//  �t�@�C��������														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <System.DateUtils.hpp>
#include "usr_wic.h"
#include "usr_xd2tx.h"
#include "usr_highlight.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "FindDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindFileDlg *FindFileDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TFindFileDlg::TFindFileDlg(TComponent* Owner)
	: TForm(Owner)
{
	IsDark = false;
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	DateMaskEdit->Text = format_Date(Date());
	FindDate = Date();
	FindAttr = 0;
	FindDir  = FindBoth = NoMask = ToKeywd = false;

	FindTime = 0;
	FindRate = FindFps = FindFrWd = FindFrHi = FindSmpl = 0;

	Narrow	= false;
	ClearExtraCond();

	MaskComboBox->Tag	 = CBTAG_HISTORY;
	KeywordComboBox->Tag = CBTAG_HISTORY;
	TxtKwdComboBox->Tag  = CBTAG_HISTORY;
	LatLngComboBox->Tag  = CBTAG_HISTORY;

	set_ComboBoxText(CodePageComboBox,
		_T("\n")
		_T("  932: Shift_JIS\n")
		_T(" 1252: ISO-8859-1 (Latin-1)\n")
		_T(" 1200: UTF-16\n")
		_T(" 1201: UTF-16(BE)\n")
		_T("20127: US-ASCII\n")
		_T("20932: EUC-JP\n")
		_T("50220: ISO-2022-JP (JIS)\n")
		_T("65000: UTF-7\n")
		_T("65001: UTF-8\n"));

	set_ComboBoxText(LineBrkComboBox, _T("\nCR/LF\nLF\nCR\n����\n"));
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	DlgInitialized = false;

	Caption = ( FindDir? _T("�f�B���N�g��������") :
			   FindBoth? _T("�t�@�C��/�f�B���N�g��������") : _T("�t�@�C��������")) + SubTitle;
	HelpContext = FindDir? 53 : 52;

	ClientHeight = BasicHeight = 500;	//*** ��������� Top �ʒu���ς���Ă��܂��̂�h��
	IniFile->LoadPosInfo(this, DialogCenter);

	IniFile->LoadComboBoxItems(MaskComboBox, FindDir? _T("FindDirHistory") : _T("FindHistory"));
	MaskComboBox->Text = IniFile->ReadStrGen(FindDir? _T("FindDirLastMask") : _T("FindLastMask"));

	DateRadioGroup->ItemIndex = IniFile->ReadIntGen(FindDir? _T("FindDirDateMode") : _T("FindDateMode"));
	ResLinkCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindResLink"));
	DirLinkCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindDirLink"));
	DirLinkCheckBox->Visible  = FindDir || FindBoth;

	for (int i=0; i<ExtPanel->ControlCount; i++) ExtPanel->Controls[i]->Visible = false;
	ExtPanel->Visible = false;

	for (int i=0; i<BasicPanel->ControlCount; i++) {
		BasicPanel->Controls[i]->Visible = false;
		BasicPanel->Controls[i]->Align	 = alBottom;
	}

	TagsComboBox->Items->Assign(usr_TAG->TagNameList);
	TagsComboBox->Text = EmptyStr;

	//�f�B���N�g��
	if (FindDir) {
		MaskPanel->Visible		  = true;		MaskPanel->Align = alTop;
		KeywordPanel->Visible	  = false;
		DatePanel->Visible		  = true;		DatePanel->Align = alTop;
		SizePanel->Visible		  = false;
		AttrPanel->Visible		  = true;		AttrPanel->Align = alTop;
		ContPanel->Visible		  = true;		ContPanel->Align = alTop;
		ContRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindContMode"));
		DirWarnCheckBox->Checked  = false;
		SubDirCheckBox->Visible   = false;
		ArcCheckBox->Visible	  = false;
		ExtraCheckBox->Visible	  = false;
		ExtraCheckBox->Checked	  = false;
	}
	//�t�@�C��
	else {
		MaskPanel->Visible		  = !NoMask;	MaskPanel->Align	= alTop;
		KeywordPanel->Visible	  = true;		KeywordPanel->Align = alTop;
		DatePanel->Visible		  = true;		DatePanel->Align	= alTop;
		SizePanel->Visible		  = true;		SizePanel->Align	= alTop;
		AttrPanel->Visible		  = true;		AttrPanel->Align	= alTop;
		ContPanel->Visible		  = false;
		CaseCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FindOptCase"));
		AndCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FindOptAnd"));

		RegExCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FindOptRegEx"));
		IniFile->LoadComboBoxItems(KeywordComboBox, RegExCheckBox->Checked? _T("FindPatrnHistory") : _T("FindKeywdHistory"));
		SizeRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindSizeMode"));
		SizeEdit->Text			  = IniFile->ReadStrGen(_T("FindSizeText"),	"0");

		int s_unit = IniFile->ReadIntGen(_T("FindSizeUnit"));
		switch (s_unit) {
		case 1:  KBRadioButton->Checked = true; break;
		case 2:  MBRadioButton->Checked = true; break;
		case 3:  GBRadioButton->Checked = true; break;
		default: BRadioButton->Checked	= true;
		}

		TimeMaskEdit->Text		  = IniFile->ReadStrGen(_T("FindTimeText"),	"00:03:00");
		TimeRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindTimeMode"));

		RateEdit->Text			  = IniFile->ReadStrGen(_T("FindRateText"),	"128");
		RateRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindRateMode"));

		FpsEdit->Text			  = IniFile->ReadStrGen(_T("FindFpsText"),	"30");
		FpsRadioGroup->ItemIndex  = IniFile->ReadIntGen(_T("FindFpsMode"));
		FrWdEdit->Text			  = IniFile->ReadStrGen(_T("FindFrWdText"),	"360");
		FrWdRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindFrWdMode"));
		FrHiEdit->Text			  = IniFile->ReadStrGen(_T("FindFrHiText"),	"240");
		FrHiRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindFrHiMode"));

		SmplEdit->Text			  = IniFile->ReadStrGen(_T("FindSmplText"),	"44100");
		SmplRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindSmplMode"));
		BitRadioGroup->ItemIndex  = IniFile->ReadIntGen(_T("FindBitMode"));
		ChnRadioGroup->ItemIndex  = IniFile->ReadIntGen(_T("FindChnMode"));

		ImgWdEdit->Text 		   = IniFile->ReadStrGen(_T("FindImgWdText"),	"600");
		ImgWdRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindImgWdMode"));
		ImgHiEdit->Text 		   = IniFile->ReadStrGen(_T("FindImgHiText"),	"400");
		ImgHiRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindImgHiMode"));
		ImgMaxCheckBox->Checked	   = IniFile->ReadBoolGen(_T("FindImgMax"));

		GpsRangeEdit->Text		   = IniFile->ReadStrGen(_T("FindGpsRange"),	"1000");
		ExifCaseCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindExifCase"));
		ExifAndCheckBox->Checked   = IniFile->ReadBoolGen(_T("FindExifAnd"));
		ExifRegExCheckBox->Checked = IniFile->ReadBoolGen(_T("FindExifRegEx"));
		ExifKwdEdit->Text		   = EmptyStr;

		PrpCaseCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindPrpCase"));
		PrpAndCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FindPrpAnd"));
		PrpRegExCheckBox->Checked = IniFile->ReadBoolGen(_T("FindPrpRegEx"));
		PrpKwdEdit->Text		  = EmptyStr;

		TxtCaseCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindTxtCase"));
		TxtAndCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FindTxtAnd"));
		TxtRegExCheckBox->Checked = IniFile->ReadBoolGen(_T("FindTxtRegEx"));
		IniFile->LoadComboBoxItems(TxtKwdComboBox, TxtRegExCheckBox->Checked? _T("FindTxtPtnHistory") : _T("FindTxtKwdHistory"));
		TxtKwdComboBox->Text	  = EmptyStr;

		IconEdit->Text			  = IniFile->ReadStrGen(_T("FindIconText"),	"1");
		IconRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("FindIconMode"));

		LCntEdit->Text			  = IniFile->ReadStrGen(_T("FindLnkCntText"),	"2");
		LCntRadioGroup->ItemIndex = 0;	//***

		SubDirCheckBox->Visible   = true;
		SubDirCheckBox->Enabled   = !Narrow;
		ArcCheckBox->Visible	  = !FindBoth;
		ExtraCheckBox->Visible	  = !FindBoth && MaskPanel->Visible;

		LatLngComboBox->Items->Assign(LatLngHistory);
		LatLngComboBox->Text = EmptyStr;

		HasAdsCheckBox->Checked   = false;
		UseProcCheckBox->Checked  = false;
		NameWarnCheckBox->Checked = false;
	}

	OkPanel->Visible = true;

	//�����ݒ�
	int hi = 0;
	for (int i=0; i<BasicPanel->ControlCount; i++)
		if (BasicPanel->Controls[i]->Visible) hi += BasicPanel->Controls[i]->Height;
	ClientHeight = BasicHeight = hi;

	AttrRadioGroup->ItemIndex = IniFile->ReadIntGen( FindDir? _T("FindDirAttrMode") : _T("FindAttrMode"));
	AttrRCheckBox->Checked	  = IniFile->ReadBoolGen(FindDir? _T("FindDirAttrR") : _T("FindAttrR"));
	AttrHCheckBox->Checked	  = IniFile->ReadBoolGen(FindDir? _T("FindDirAttrH") : _T("FindAttrH"));
	AttrSCheckBox->Checked	  = IniFile->ReadBoolGen(FindDir? _T("FindDirAttrS") : _T("FindAttrS"));
	AttrACheckBox->Checked	  = IniFile->ReadBoolGen(FindDir? _T("FindDirAttrA") : _T("FindAttrA"));
	AttrCCheckBox->Checked	  = IniFile->ReadBoolGen(FindDir? _T("FindDirAttrC") : _T("FindAttrC"));
	AttrHCheckBox->Enabled	  = ShowHideAtr;
	AttrSCheckBox->Enabled	  = ShowSystemAtr;

	if (!PropLabel) PropLabel = AttachLabelToGroup(PropGroupBox, "���(&I)");
	if (!IconLabel) IconLabel = AttachLabelToGroup(IconGroupBox, "�A�C�R����(&N)");

	if (IsDark!=IsDarkMode || Color!=col_DkPanel) {
		SetDarkWinTheme(this);
		IsDark = IsDarkMode;
	}
	Shape1->Pen->Color = IsDarkMode? dcl_BtnShadow : scl_BtnShadow;
	Shape2->Pen->Color = IsDarkMode? dcl_BtnHighlight : scl_BtnHighlight;

	//�`�F�b�N�{�b�N�X�ƃ��x���̕\����Ԃ𓯊�
	for (int i=0; i<OkPanel->ControlCount; i++) {
		TControl *cp = OkPanel->Controls[i];
		if (cp->ClassNameIs("TLabel")) {
			TLabel *lp = (TLabel *)cp;
			if (ContainsText(lp->Caption, "�T�u�f�B���N�g��������"))
				lp->Visible = SubDirCheckBox->Visible;
			else if (ContainsText(lp->Caption, "�A�[�J�C�u��������"))
				lp->Visible = ArcCheckBox->Visible;
			else if (ContainsText(lp->Caption, "�g������"))
				lp->Visible = ExtraCheckBox->Visible;
			else if (ContainsText(lp->Caption, "�f�B���N�g�����𔽉f"))
				lp->Visible = DirLinkCheckBox->Visible;
		}
	}

	ExtraCheckBoxClick(NULL);
	ArcCheckBoxClick(NULL);

	DlgInitialized = true;

	CondChangeUpdate(NULL);

	if (MaskPanel->Visible) {
		(ToKeywd? KeywordComboBox : MaskComboBox)->SetFocus();
	}
	else {
		MaskComboBox->Text = EmptyStr;
		KeywordComboBox->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (ModalResult==mrOk) {
		try {
			if (DateRadioGroup->ItemIndex==1 && ContainsStr(DateMaskEdit->Text, "?")) {
				FindDateStr = DateMaskEdit->Text;
				FindDate	= 0;
			}
			else {
				FindDate	= TDate(DateMaskEdit->Text);
				FindDateStr = EmptyStr;
			}

			if (TimePanel->Visible) FindTime = TTime(TimeMaskEdit->Text);
		}
		catch (EConvertError &e) {
			CanClose = false;
		}

		FindRate   = EditToInt(RateEdit);
		FindFps    = EditToInt(FpsEdit);
		FindFrWd   = EditToInt(FrWdEdit);
		FindFrHi   = EditToInt(FrHiEdit);
		FindSmpl   = EditToInt(SmplEdit);
		FindImgWd  = EditToInt(ImgWdEdit);
		FindImgHi  = EditToInt(ImgHiEdit);
		FindIcon   = EditToInt(IconEdit);
		FindLnkCnt = EditToInt(LCntEdit);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteStrGen(FindDir? _T("FindDirLastMask") : _T("FindLastMask"), MaskComboBox->Text);
	IniFile->SaveComboBoxItems(MaskComboBox, FindDir? _T("FindDirHistory") : _T("FindHistory"));

	IniFile->SaveComboBoxItems(KeywordComboBox, RegExCheckBox->Checked?    _T("FindPatrnHistory")  : _T("FindKeywdHistory"));
	IniFile->SaveComboBoxItems(TxtKwdComboBox,  TxtRegExCheckBox->Checked? _T("FindTxtPtnHistory") : _T("FindTxtKwdHistory"));

	IniFile->WriteIntGen(FindDir? _T("FindDirDateMode") : _T("FindDateMode"),	DateRadioGroup);
	IniFile->WriteBoolGen(_T("FindResLink"),	ResLinkCheckBox);
	IniFile->WriteBoolGen(_T("FindDirLink"),	DirLinkCheckBox);

	//�f�B���N�g��
	if (FindDir) {
		IniFile->WriteIntGen(_T("FindContMode"),	ContRadioGroup);
	}
	//�t�@�C��
	else {
		IniFile->WriteBoolGen(_T("FindOptCase"),	CaseCheckBox);
		IniFile->WriteBoolGen(_T("FindOptAnd"),		AndCheckBox);
		IniFile->WriteBoolGen(_T("FindOptRegEx"),	RegExCheckBox);
		IniFile->WriteIntGen(_T("FindSizeMode"),	SizeRadioGroup);
		IniFile->WriteStrGen(_T("FindSizeText"),	SizeEdit);

		int s_unit = 0;
		if		(KBRadioButton->Checked) s_unit = 1;
		else if (MBRadioButton->Checked) s_unit = 2;
		else if (GBRadioButton->Checked) s_unit = 3;
		IniFile->WriteIntGen(_T("FindSizeUnit"),	s_unit);

		IniFile->WriteStrGen(_T("FindTimeText"),	TimeMaskEdit->Text);
		IniFile->WriteIntGen(_T("FindTimeMode"),	TimeRadioGroup);

		IniFile->WriteStrGen(_T("FindRateText"),	RateEdit);
		IniFile->WriteIntGen(_T("FindRateMode"),	RateRadioGroup);

		IniFile->WriteStrGen(_T("FindFpsText"),		FpsEdit);
		IniFile->WriteIntGen(_T("FindFpsMode"),		FpsRadioGroup);
		IniFile->WriteStrGen(_T("FindFrWdText"),	FrWdEdit);
		IniFile->WriteIntGen(_T("FindFrWdMode"),	FrWdRadioGroup);
		IniFile->WriteStrGen(_T("FindFrHiText"),	FrHiEdit);
		IniFile->WriteIntGen(_T("FindFrHiMode"),	FrHiRadioGroup);

		IniFile->WriteStrGen(_T("FindSmplText"),	SmplEdit);
		IniFile->WriteIntGen(_T("FindSmplMode"),	SmplRadioGroup);
		IniFile->WriteIntGen(_T("FindBitMode"),		BitRadioGroup);
		IniFile->WriteIntGen(_T("FindChnMode"),		ChnRadioGroup);

		IniFile->WriteStrGen(_T("FindImgWdText"),	ImgWdEdit);
		IniFile->WriteIntGen(_T("FindImgWdMode"),	ImgWdRadioGroup);
		IniFile->WriteStrGen(_T("FindImgHiText"),	ImgHiEdit);
		IniFile->WriteIntGen(_T("FindImgHiMode"),	ImgHiRadioGroup);
		IniFile->WriteBoolGen(_T("FindImgMax"),		ImgMaxCheckBox);

		IniFile->WriteStrGen(_T("FindGpsRange"),	GpsRangeEdit);
		IniFile->WriteBoolGen(_T("FindExifCase"),	ExifCaseCheckBox);
		IniFile->WriteBoolGen(_T("FindExifAnd"),	ExifAndCheckBox);
		IniFile->WriteBoolGen(_T("FindExifRegEx"),	ExifRegExCheckBox);

		IniFile->WriteBoolGen(_T("FindPrpCase"),	PrpCaseCheckBox);
		IniFile->WriteBoolGen(_T("FindPrpAnd"),		PrpAndCheckBox);
		IniFile->WriteBoolGen(_T("FindPrpRegEx"),	PrpRegExCheckBox);

		IniFile->WriteBoolGen(_T("FindTxtCase"),	TxtCaseCheckBox);
		IniFile->WriteBoolGen(_T("FindTxtAnd"),		TxtAndCheckBox);
		IniFile->WriteBoolGen(_T("FindTxtRegEx"),	TxtRegExCheckBox);

		IniFile->WriteStrGen(_T("FindIconText"),	IconEdit);
		IniFile->WriteIntGen(_T("FindIconMode"),	IconRadioGroup);

		IniFile->WriteStrGen(_T("FindLnkCntText"),	LCntEdit);

		IniFile->WriteBoolGen(_T("FindSubDir"),		SubDirCheckBox);
		IniFile->WriteBoolGen(_T("FindArchive"),	ArcCheckBox);

		if (ExtraCheckBox->Visible) IniFile->WriteBoolGen(_T("FindExtra"), ExtraCheckBox);

		LatLngHistory->Assign(LatLngComboBox->Items);
	}

	IniFile->WriteIntGen( FindDir? _T("FindDirAttrMode") : _T("FindAttrMode"),	AttrRadioGroup);
	IniFile->WriteBoolGen(FindDir? _T("FindDirAttrR") : _T("FindAttrR"),		AttrRCheckBox);
	IniFile->WriteBoolGen(FindDir? _T("FindDirAttrH") : _T("FindAttrH"),		AttrHCheckBox);
	IniFile->WriteBoolGen(FindDir? _T("FindDirAttrS") : _T("FindAttrS"),		AttrSCheckBox);
	IniFile->WriteBoolGen(FindDir? _T("FindDirAttrA") : _T("FindAttrA"),		AttrACheckBox);
	IniFile->WriteBoolGen(FindDir? _T("FindDirAttrC") : _T("FindAttrC"),		AttrCCheckBox);

	FindDir  = false;
	SubTitle = EmptyStr;
}

//---------------------------------------------------------------------------
//�g���������N���A
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::ClearExtraCond()
{
	hasTime = hasRate = hasFrame = hasFormat = hasImage = hasExif = hasProp = hasText = hasIcon = hasCode = false;
	hasTags	= hasLCnt = hasOther = false;
}

//---------------------------------------------------------------------------
//���������̓��ꊷ��
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::RegExCheckBoxClick(TObject *Sender)
{
	if (DlgInitialized)
		change_ComboBoxHistory(KeywordComboBox, _T("FindKeywdHistory"), _T("FindPatrnHistory"), RegExCheckBox->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::TxtRegExCheckBoxClick(TObject *Sender)
{
	if (DlgInitialized)
		change_ComboBoxHistory(TxtKwdComboBox, _T("FindTxtKwdHistory"), _T("FindTxtPtnHistory"), TxtRegExCheckBox->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::ExtraCheckBoxClick(TObject *Sender)
{
	if (ExtraCheckBox->Visible && ExtraCheckBox->Checked) {
		ExtPanel->Visible = true;
		ClientWidth = BasicPanel->Width + ExtPanel->Width + 8;
		MaskComboBoxChange(NULL);
	}
	else {
		ExtPanel->Visible = false;
		ClientWidth  = BasicPanel->Width;
		ClientHeight = BasicHeight;
		ClearExtraCond();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::ArcCheckBoxClick(TObject *Sender)
{
	if (ArcCheckBox->Checked && ExtraCheckBox->Visible) {
		ExtraCheckBox->Checked = false;
		ExtraCheckBoxClick(NULL);
	}
	ExtraCheckBox->Enabled = !ArcCheckBox->Checked;
}

//---------------------------------------------------------------------------
//�}�X�N�̕ύX
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::MaskComboBoxChange(TObject *Sender)
{
	if (!ExtPanel->Visible) {
		hasTime = hasRate = hasFrame = hasFormat = hasImage = hasProp = hasText = hasIcon = hasCode = false;
		hasTags	= hasLCnt = hasOther =  false;
		return;
	}

	UnicodeString fext = get_extension(MaskComboBox->Text);
	bool is_text = test_TxtFile(MaskComboBox->Text);
	hasTime   = test_FileExt(fext, FEXT_DURATION);
	hasRate   = test_FileExt(fext, _T(".mp3.wma"));
	hasFrame  = test_FileExt(fext, FEXT_VIDEO);
	hasFormat = test_FileExt(fext, _T(".wav.flac"));
	hasImage  = test_FileExt(fext, FEXT_IMAGE + get_img_fext() + FEXT_PSP);
	hasExif   = test_ExifExt(fext);
	hasText   = is_text || xd2tx_TestExt(fext);
	hasCode   = is_text;
	hasProp   = test_FileExt(fext,
					FEXT_APPINFO FEXT_VIDEO FEXT_HTML FEXT_FONT FEXT_XD2T_PRP FEXT_PSP
					_T(".mp3.flac.opus.png.gif.webp.ico.cur.ani.lnk.pdf"));
	hasIcon   = test_FileExt(fext, FEXT_ICON_CNT);
	hasTags	  = (usr_TAG->TagNameList->Count>0);
	hasLCnt   = hasOther = true;

	bool vTime   = TimePanel->Visible;
	bool vRate   = RatePanel->Visible;
	bool vFrame  = FpsPanel->Visible;
	bool vFormat = FrmtPanel->Visible;
	bool vImage  = ImgSzPanel->Visible;
	bool vExif   = ExifPanel->Visible;
	bool vText   = TextPanel->Visible;
	bool vCode   = CodePanel->Visible;
	bool vProp   = PropPanel->Visible;
	bool vIcon   = IconPanel->Visible;
	bool vLCnt	 = LCntPanel->Visible;
	bool vOther	 = OtherPanel->Visible;
	bool vTag	 = TagsPanel->Visible;

	if (vTime  != hasTime  || vRate != hasRate || vFrame != hasFrame || vFormat != hasFormat ||
		vImage != hasImage || vExif != hasExif || vText  != hasText  || vCode   != hasCode ||
		vProp  != hasProp  || vIcon != hasIcon || vTag   != hasTags  ||
		vLCnt  != hasLCnt || vOther != hasOther)
	{
		for (int i=0; i<ExtPanel->ControlCount; i++) {
			ExtPanel->Controls[i]->Visible = false;
			ExtPanel->Controls[i]->Align   = alBottom;
		}

		TimePanel->Visible	 = hasTime;		TimePanel->Align  = alTop;
		FpsPanel->Visible	 = hasFrame;	FpsPanel->Align   = alTop;
		FrSzPanel->Visible	 = hasFrame;	FrSzPanel->Align  = alTop;
		RatePanel->Visible	 = hasRate;		RatePanel->Align  = alTop;
		FrmtPanel->Visible	 = hasFormat;	FrmtPanel->Align  = alTop;
		ImgSzPanel->Visible  = hasImage; 	ImgSzPanel->Align = alTop;
		ExifPanel->Visible	 = hasExif;		ExifPanel->Align  = alTop;
		PropPanel->Visible	 = hasProp;		PropPanel->Align  = alTop;
		TextPanel->Visible	 = hasText;		TextPanel->Align  = alTop;
		CodePanel->Visible	 = hasCode;		CodePanel->Align  = alTop;
		IconPanel->Visible	 = hasIcon;		IconPanel->Align  = alTop;

		//�X�y�[�T
		for (int i=0; i<ExtPanel->ControlCount; i++) {
			if (ExtPanel->Controls[i]->Visible) {
				PanelSpacer->Visible = true;	PanelSpacer->Align = alTop;	break;
			}
		}

		TagsPanel->Visible	 = hasTags;		TagsPanel->Align  = alTop;
		LCntPanel->Visible	 = hasLCnt;		LCntPanel->Align  = alTop;
		OtherPanel->Visible	 = hasOther;	OtherPanel->Align = alTop;

		int hi = 0;
		for (int i=0; i<ExtPanel->ControlCount; i++)
			if (ExtPanel->Controls[i]->Visible) hi += ExtPanel->Controls[i]->Height;
		ClientHeight = std::max(BasicHeight, hi);

		CodePageComboBox->ItemIndex = -1;
		LineBrkComboBox->ItemIndex	= -1;
		BomRadioGroup->ItemIndex	= 0;

		if (PropLabel) {
			PropLabel->Caption =
				test_AppInfExt(fext)?	"�A�v���P�[�V�������(&I)" :
				test_HtmlExt(fext)?		"�w�b�_���(&I)" :
				hasExif?				"Exif���(&I)" :
				test_Mp3Ext(fext)?		"ID3�^�O���(&I)" :
				(test_FlacExt(fext) || USAME_TI(fext, ".opus"))?
										"�R�����g���(&I)" :
				test_LnkExt(fext)?		"�V���[�g�J�b�g���(&I)" :
				test_IcoExt(fext)?		"�A�C�R�����(&I)" :
				(test_CurExt(fext) || test_AniExt(fext))?
										"�J�[�\�����(&I)" : "�t�@�C�����(&I)";
		}
		if (IconLabel) {
			IconLabel->Caption = test_CurExt(fext)? "�摜��(&N)" : "�A�C�R����(&N)";
		}

		ExtPanel->Caption = EmptyStr;

		CondChangeUpdate(NULL);
	}
}

//---------------------------------------------------------------------------
//�{�^���ɂ����t�̕ύX
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::DateBtnClick(TObject *Sender)
{
	TButton *bp = (TButton*)Sender;

	if (USAME_TS(bp->Caption, "�{��")) {
		FindDate = Date();
	}
	else {
		try {
			FindDate = TDate(DateMaskEdit->Text);
			switch (idx_of_word_i(_T("-1D|-7D|-1M|-1Y"), bp->Caption)) {
			case 0: FindDate -= 1;						break;
			case 1: FindDate -= 7;						break;
			case 2: FindDate = IncMonth(FindDate, -1);	break;
			case 3: FindDate = IncYear(FindDate, -1);	break;
			}
		}
		catch (EConvertError &e) {
			msgbox_ERR(USTR_IllegalDate);
			DateMaskEdit->SetFocus();
			FindDate = Date();
		}
	}

	DateMaskEdit->Text = format_Date(FindDate);
}

//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::AttrCheckBoxClick(TObject *Sender)
{
	if (!AttrRCheckBox->Checked && !AttrACheckBox->Checked
		&& (!ShowHideAtr || (!AttrHCheckBox->Checked && ShowHideAtr))
		&& (!ShowSystemAtr || (!AttrSCheckBox->Checked && ShowSystemAtr))
		&& !AttrCCheckBox->Checked)
			AttrRadioGroup->ItemIndex = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::CondChangeUpdate(TObject *Sender)
{
	CodePanel->Color = get_PanelColor(CodePageComboBox->Text.IsEmpty() && LineBrkComboBox->Text.IsEmpty());
	DatePanel->Color = get_PanelColor(DateRadioGroup->ItemIndex==0);
	SizePanel->Color = get_PanelColor(SizeRadioGroup->ItemIndex==0);
	ContPanel->Color = get_PanelColor(ContRadioGroup->ItemIndex==0 && !DirWarnCheckBox->Checked);
	AttrPanel->Color = get_PanelColor(AttrRadioGroup->ItemIndex==0);
	TimePanel->Color = get_PanelColor(TimeRadioGroup->ItemIndex==0);
	RatePanel->Color = get_PanelColor(RateRadioGroup->ItemIndex==0);
	FpsPanel->Color  = get_PanelColor(FpsRadioGroup->ItemIndex==0);
	FrSzPanel->Color = get_PanelColor(FrWdRadioGroup->ItemIndex==0  && FrHiRadioGroup->ItemIndex==0);
	ImgSzPanel->Color= get_PanelColor(ImgWdRadioGroup->ItemIndex==0 && ImgHiRadioGroup->ItemIndex==0);
	IconPanel->Color = get_PanelColor(IconRadioGroup->ItemIndex==0);
	TagsPanel->Color = get_PanelColor(TagsComboBox->Text.IsEmpty());
	LCntPanel->Color = get_PanelColor(LCntRadioGroup->ItemIndex==0);
	OtherPanel->Color=
		get_PanelColor(!NameWarnCheckBox->Checked && !HasAdsCheckBox->Checked && !UseProcCheckBox->Checked);
	FrmtPanel->Color =
		get_PanelColor(SmplRadioGroup->ItemIndex==0 && BitRadioGroup->ItemIndex==0 && ChnRadioGroup->ItemIndex==0);
	ExifPanel->Color =
		get_PanelColor(ExifKwdEdit->Text.IsEmpty() && (LatLngComboBox->Text.IsEmpty() || GpsRangeEdit->Text.IsEmpty()));

	PropPanel->Color = get_PanelColor(PrpKwdEdit->Text.IsEmpty());
	TextPanel->Color = get_PanelColor(TxtKwdComboBox->Text.IsEmpty());

	double range = EditToInt(GpsRangeEdit, 0)/1000.0;
	KmLabel->Caption = (range>=1.0)? UnicodeString().sprintf(_T("%.1f km"), range) : EmptyStr;

	DateMaskEdit->EditMask = (DateRadioGroup->ItemIndex==1)? "!cccc/cc/cc;1;_" : "!9999/99/99;1;_";

	UnicodeString tmp = CodePageComboBox->Text;
	int cpag = tmp.IsEmpty()? -1 : get_tkn(tmp, ':').ToIntDef(0);
	BomRadioGroup->Enabled = (cpag==1200 || cpag==1201 || cpag==65001);
	if (!BomRadioGroup->Enabled) BomRadioGroup->ItemIndex = 0;

	if (PropLabel) {
		UnicodeString tit = get_tkn(PropLabel->Caption, "I)") + "I)";
		if (UseProcCheckBox->Checked) tit += " /�v���Z�X���";
		PropLabel->Caption = tit;
	}
}

//---------------------------------------------------------------------------
//OK
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FindOkActionExecute(TObject *Sender)
{
	add_ComboBox_history(MaskComboBox);

	if (!FindDir) {
		add_ComboBox_history(KeywordComboBox);
		IniFile->SaveComboBoxItems(KeywordComboBox, RegExCheckBox->Checked? _T("FindPatrnHistory") : _T("FindKeywdHistory"));
	}

	if (hasText) {
		add_ComboBox_history(TxtKwdComboBox);
		IniFile->SaveComboBoxItems(TxtKwdComboBox, TxtRegExCheckBox->Checked? _T("FindTxtPtnHistory") : _T("FindTxtKwdHistory"));
	}

	if (hasExif) add_ComboBox_history(LatLngComboBox);

	FindAttr = 0;
	if (AttrRCheckBox->Checked)					 FindAttr |= faReadOnly;
	if (AttrACheckBox->Checked)					 FindAttr |= faArchive;
	if (AttrHCheckBox->Checked && ShowHideAtr)	 FindAttr |= faHidden;
	if (AttrSCheckBox->Checked && ShowSystemAtr) FindAttr |= faSysFile;
	if (AttrCCheckBox->Checked)					 FindAttr |= faCompressed;
}
//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FindOkActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;

	//���K�\���p�^�[���̃`�F�b�N
	bool k_reg_ng = !FindDir && RegExCheckBox->Checked && !KeywordComboBox->Text.IsEmpty() && !chk_RegExPtn(KeywordComboBox->Text);
	set_ErrColor(KeywordComboBox, k_reg_ng);

	bool p_reg_ng = hasProp && PrpRegExCheckBox->Checked && !PrpKwdEdit->Text.IsEmpty() && !chk_RegExPtn(PrpKwdEdit->Text);
	set_ErrColor(PrpKwdEdit, p_reg_ng);

	bool t_reg_ng = hasText && TxtRegExCheckBox->Checked && !TxtKwdComboBox->Text.IsEmpty() && !chk_RegExPtn(TxtKwdComboBox->Text);
	set_ErrColor(TxtKwdComboBox, t_reg_ng);

	//���t�̃`�F�b�N
	UnicodeString dtstr = DateMaskEdit->Text;
	bool date_ng = false;
	bool use_wc  = (DateRadioGroup->ItemIndex==1 && ContainsStr(dtstr, "?"));
	try {
		if (DateRadioGroup->ItemIndex>0) {
			if (ContainsStr(dtstr, " ")) Abort();
			if (use_wc) {
				for (int i=1; i<dtstr.Length(); i++)
					if (!iswdigit(dtstr[i]) && dtstr[i]!='/' && dtstr[i]!='?') Abort();
			}
			else {
				if (ContainsStr(dtstr, "?")) Abort();
				FindDate = TDate(dtstr);
			}
		}
	}
	catch (...) {
		date_ng = true;
	}
	set_ErrColor(DateMaskEdit, date_ng);

	DateBtn2->Enabled = !use_wc;
	DateBtn3->Enabled = !use_wc;
	DateBtn4->Enabled = !use_wc;
	DateBtn5->Enabled = !use_wc;

	//�^�C���̃`�F�b�N
	bool time_ng = false;
	if (TimePanel->Visible) {
		try {
			FindTime = TTime(TimeMaskEdit->Text);
		}
		catch (EConvertError &e) {
			time_ng = true;
		}
		set_ErrColor(TimeMaskEdit, time_ng);
	}

	ap->Enabled = (!k_reg_ng && !p_reg_ng && !t_reg_ng && !date_ng && !time_ng);

	ImgWdEdit->EditLabel->Caption = ImgMaxCheckBox->Checked? "��" : "��";
	ImgHiEdit->EditLabel->Caption = ImgMaxCheckBox->Checked? "�Z" : "��";

	AndCheckBox->Enabled	 = !RegExCheckBox->Checked;
	PrpAndCheckBox->Enabled  = !PrpRegExCheckBox->Checked;
	TxtAndCheckBox->Enabled  = !TxtRegExCheckBox->Checked;

	KeywordComboBox->Tag
		= CBTAG_HISTORY | (KeywordComboBox->Focused()? CBTAG_RGEX_V : 0) | (RegExCheckBox->Checked? CBTAG_RGEX_E : 0);
	TxtKwdComboBox->Tag
		= CBTAG_HISTORY | (TxtKwdComboBox->Focused()? CBTAG_RGEX_V : 0) | (TxtRegExCheckBox->Checked? CBTAG_RGEX_E : 0);
	PrpKwdEdit->Tag
		= (PrpKwdEdit->Focused()? EDTAG_RGEX_V : 0) | (PrpRegExCheckBox->Checked? EDTAG_RGEX_E : 0);
}

//---------------------------------------------------------------------------
void __fastcall TFindFileDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (SpecialKeyProc(this, Key, Shift)) return;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+D")) set_focus_GroupBox(DateGroupBox);
	else if (USAME_TI(KeyStr, "Alt+S")) set_focus_GroupBox(SizeGroupBox);
	else if (USAME_TI(KeyStr, "Alt+A")) set_focus_GroupBox(AttrGroupBox);
	else if (USAME_TI(KeyStr, "Alt+R")) invert_CheckBox(SubDirCheckBox);
	else if (USAME_TI(KeyStr, "Alt+V")) invert_CheckBox(ArcCheckBox);
	else if (USAME_TI(KeyStr, "Alt+X")) invert_CheckBox(ExtraCheckBox);
	else if (ExtPanel->Visible) {
		if		(USAME_TI(KeyStr, "Alt+B")) set_focus_GroupBox(RateGroupBox);
		else if (USAME_TI(KeyStr, "Alt+C")) set_focus_GroupBox(CodeGroupBox);
		else if (USAME_TI(KeyStr, "Alt+E")) set_focus_GroupBox(ExifGroupBox);
		else if (USAME_TI(KeyStr, "Alt+F")) set_focus_GroupBox(FrmtGroupBox);
		else if (USAME_TI(KeyStr, "Alt+G")) set_focus_GroupBox(TagGroupBox);
		else if (USAME_TI(KeyStr, "Alt+I")) set_focus_GroupBox(PropGroupBox);
		else if (USAME_TI(KeyStr, "Alt+L")) set_focus_GroupBox(LCntGroupBox);
		else if (USAME_TI(KeyStr, "Alt+N")) set_focus_GroupBox(IconGroupBox);
		else if (USAME_TI(KeyStr, "Alt+O")) set_focus_GroupBox(OtherGroupBox);
		else if (USAME_TI(KeyStr, "Alt+P")) set_focus_GroupBox(FpsGroupBox);
		else if (USAME_TI(KeyStr, "Alt+T")) {
			if (!set_focus_GroupBox(TimeGroupBox)) set_focus_GroupBox(TextGroupBox);
		}
		else if (USAME_TI(KeyStr, "Alt+W")) {
			if (!set_focus_GroupBox(FrSzGroupBox)) set_focus_GroupBox(ImgSzGroupBox);
		}
	}
}
//---------------------------------------------------------------------------

