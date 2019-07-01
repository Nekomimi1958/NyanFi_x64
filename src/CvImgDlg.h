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
	TComboBox *ClipNameComboBox;
	TComboBox *CmpModeComboBox;
	TComboBox *ScaleModeComboBox;
	TComboBox *ScaleOptComboBox;
	TComboBox *YCrCbComboBox;
	TEdit *ChgNameEdit;
	TGroupBox *NameGroupBox;
	TGroupBox *RenGroupBox;
	TGroupBox *ScaleGroupBox;
	TGroupBox *SubOptGroupBox;
	TLabel *CmpModeLabel;
	TLabel *FextLabel;
	TLabel *ImgQLabel;
	TLabel *SttLabel;
	TLabel *YCrCbLabel;
	TLabeledEdit *ScalePrm1Edit;
	TLabeledEdit *ScalePrm2Edit;
	TPanel *BtnPanel;
	TPanel *MainPanel;
	TPanel *MgnColPanel;
	TPanel *NamePanel;
	TPanel *SubPanel;
	TRadioButton *ClipAutoBtn;
	TRadioButton *ClipOWBtn;
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
	void __fastcall ClipNameComboBoxChange(TObject *Sender);

private:	// ユーザー宣言
	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ユーザー宣言
	bool fromClip;
	UnicodeString DistPath;

	__fastcall TCvImageDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCvImageDlg *CvImageDlg;
//---------------------------------------------------------------------------
#endif
