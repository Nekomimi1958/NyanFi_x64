//----------------------------------------------------------------------//
// NyanFi																//
//  ファイル名検索														//
//----------------------------------------------------------------------//
#ifndef FindDlgH
#define FindDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TFindFileDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *FindOkAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TBevel *Bevel2;
	TBevel *PanelSpacer;
	TButton *CanButton;
	TButton *DateBtn1;
	TButton *DateBtn2;
	TButton *DateBtn3;
	TButton *DateBtn4;
	TButton *DateBtn5;
	TButton *OkButton;
	TCheckBox *AndCheckBox;
	TCheckBox *ArcCheckBox;
	TCheckBox *AttrACheckBox;
	TCheckBox *AttrCCheckBox;
	TCheckBox *AttrHCheckBox;
	TCheckBox *AttrRCheckBox;
	TCheckBox *AttrSCheckBox;
	TCheckBox *CaseCheckBox;
	TCheckBox *DirLinkCheckBox;
	TCheckBox *DirWarnCheckBox;
	TCheckBox *ExifAndCheckBox;
	TCheckBox *ExifCaseCheckBox;
	TCheckBox *ExifRegExCheckBox;
	TCheckBox *ExtraCheckBox;
	TCheckBox *HasAdsCheckBox;
	TCheckBox *ImgMaxCheckBox;
	TCheckBox *NameWarnCheckBox;
	TCheckBox *PrpAndCheckBox;
	TCheckBox *PrpCaseCheckBox;
	TCheckBox *PrpRegExCheckBox;
	TCheckBox *RegExCheckBox;
	TCheckBox *ResLinkCheckBox;
	TCheckBox *SubDirCheckBox;
	TCheckBox *TxtAndCheckBox;
	TCheckBox *TxtCaseCheckBox;
	TCheckBox *TxtRegExCheckBox;
	TCheckBox *UseProcCheckBox;
	TComboBox *CodePageComboBox;
	TComboBox *KeywordComboBox;
	TComboBox *LatLngComboBox;
	TComboBox *LineBrkComboBox;
	TComboBox *MaskComboBox;
	TComboBox *TagsComboBox;
	TComboBox *TxtKwdComboBox;
	TEdit *SizeEdit;
	TGroupBox *AttrGroupBox;
	TGroupBox *CodeGroupBox;
	TGroupBox *DateGroupBox;
	TGroupBox *ExifGroupBox;
	TGroupBox *FpsGroupBox;
	TGroupBox *FrmtGroupBox;
	TGroupBox *FrSzGroupBox;
	TGroupBox *GpsGroupBox;
	TGroupBox *GroupBox1;
	TGroupBox *IconGroupBox;
	TGroupBox *ImgSzGroupBox;
	TGroupBox *LCntGroupBox;
	TGroupBox *OtherGroupBox;
	TGroupBox *PropGroupBox;
	TGroupBox *RateGroupBox;
	TGroupBox *SizeGroupBox;
	TGroupBox *TextGroupBox;
	TGroupBox *TimeGroupBox;
	TLabel *KmLabel;
	TLabel *KwdLabel;
	TLabel *Label1;
	TLabeledEdit *ExifKwdEdit;
	TLabeledEdit *FpsEdit;
	TLabeledEdit *FrHiEdit;
	TLabeledEdit *FrWdEdit;
	TLabeledEdit *GpsRangeEdit;
	TLabeledEdit *IconEdit;
	TLabeledEdit *ImgHiEdit;
	TLabeledEdit *ImgWdEdit;
	TLabeledEdit *LCntEdit;
	TLabeledEdit *PrpKwdEdit;
	TLabeledEdit *RateEdit;
	TLabeledEdit *SmplEdit;
	TMaskEdit *DateMaskEdit;
	TMaskEdit *TimeMaskEdit;
	TPanel *AttrPanel;
	TPanel *BasicPanel;
	TPanel *CodePanel;
	TPanel *ContPanel;
	TPanel *DatePanel;
	TPanel *ExifPanel;
	TPanel *ExtPanel;
	TPanel *FpsPanel;
	TPanel *FrmtPanel;
	TPanel *FrSzPanel;
	TPanel *IconPanel;
	TPanel *ImgSzPanel;
	TPanel *KeywordPanel;
	TPanel *LCntPanel;
	TPanel *MaskPanel;
	TPanel *OkPanel;
	TPanel *OtherPanel;
	TPanel *PropPanel;
	TPanel *RatePanel;
	TPanel *SizePanel;
	TPanel *TagsPanel;
	TPanel *TextPanel;
	TPanel *TimePanel;
	TRadioButton *BRadioButton;
	TRadioButton *GBRadioButton;
	TRadioButton *KBRadioButton;
	TRadioButton *MBRadioButton;
	TRadioGroup *AttrRadioGroup;
	TRadioGroup *BitRadioGroup;
	TRadioGroup *BomRadioGroup;
	TRadioGroup *ChnRadioGroup;
	TRadioGroup *ContRadioGroup;
	TRadioGroup *DateRadioGroup;
	TRadioGroup *FpsRadioGroup;
	TRadioGroup *FrHiRadioGroup;
	TRadioGroup *FrWdRadioGroup;
	TRadioGroup *IconRadioGroup;
	TRadioGroup *ImgHiRadioGroup;
	TRadioGroup *ImgWdRadioGroup;
	TRadioGroup *LCntRadioGroup;
	TRadioGroup *RateRadioGroup;
	TRadioGroup *SizeRadioGroup;
	TRadioGroup *SmplRadioGroup;
	TRadioGroup *TimeRadioGroup;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall DateBtnClick(TObject *Sender);
	void __fastcall AttrCheckBoxClick(TObject *Sender);
	void __fastcall CondChangeUpdate(TObject *Sender);
	void __fastcall FindOkActionExecute(TObject *Sender);
	void __fastcall FindOkActionUpdate(TObject *Sender);
	void __fastcall MaskComboBoxChange(TObject *Sender);
	void __fastcall ExtraCheckBoxClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ArcCheckBoxClick(TObject *Sender);
	void __fastcall RegExCheckBoxClick(TObject *Sender);
	void __fastcall TxtRegExCheckBoxClick(TObject *Sender);

private:	// ユーザー宣言
	bool DlgInitialized;
	int  BasicHeight;

	void __fastcall ClearExtraCond();

public:		// ユーザー宣言
	UnicodeString SubTitle;

	bool FindDir;
	bool FindBoth;
	bool Narrow;
	bool NoMask;
	bool ToKeywd;

	TDateTime FindDate;
	UnicodeString FindDateStr;
	TTime FindTime;
	int  FindAttr;
	int  FindRate;
	int  FindFps;
	int  FindFrWd;
	int  FindFrHi;
	int  FindImgWd;
	int  FindImgHi;
	int  FindSmpl;
	int  FindIcon;
	int  FindLnkCnt;

	bool hasTime;
	bool hasRate;
	bool hasFrame;
	bool hasFormat;
	bool hasImage;
	bool hasExif;
	bool hasProp;
	bool hasText;
	bool hasIcon;
	bool hasCode;
	bool hasTags;
	bool hasLCnt;
	bool hasOther;

	__fastcall TFindFileDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindFileDlg *FindFileDlg;
//---------------------------------------------------------------------------
#endif
