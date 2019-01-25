//----------------------------------------------------------------------//
// NyanFi																//
//  画像変換ダイアログ													//
//----------------------------------------------------------------------//
#ifndef CvImgDlgH
#define CvImgDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TCvImageDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefMgnColBtn;
	TCheckBox *GrayScaleCheckBox;
	TCheckBox *KeepTimeCheckBox;
	TCheckBox *NotUsePrvCheckBox;
	TComboBox *ChgNameComboBox;
	TComboBox *CmpModeComboBox;
	TComboBox *ScaleModeComboBox;
	TComboBox *ScaleOptComboBox;
	TComboBox *YCrCbComboBox;
	TEdit *ChgNameEdit;
	TGroupBox *NameGroupBox;
	TGroupBox *ScaleGroupBox;
	TGroupBox *SubOptGroupBox;
	TLabel *CmpModeLabel;
	TLabel *ImgQLabel;
	TLabel *YCrCbLabel;
	TLabeledEdit *ScalePrm1Edit;
	TLabeledEdit *ScalePrm2Edit;
	TPanel *MgnColPanel;
	TRadioGroup *CvFmtRadioGroup;
	TTrackBar *ImgQTrackBar;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CvFmtRadioGroupClick(TObject *Sender);
	void __fastcall ImgQTrackBarChange(TObject *Sender);
	void __fastcall ScaleModeComboBoxChange(TObject *Sender);
	void __fastcall RefMgnColBtnClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GrayScaleCheckBoxClick(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	__fastcall TCvImageDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TCvImageDlg *CvImageDlg;
//---------------------------------------------------------------------------
#endif
