//----------------------------------------------------------------------//
// NyanFi																//
//  ����ݒ�															//
//----------------------------------------------------------------------//
#ifndef PrnImgDlgH
#define PrnImgDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TPrintImgDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *PrintAction;
	TActionList *ActionList1;
	TButton *OkButton;
	TButton *PrnButton;
	TButton *PrnPropBtn;
	TButton *RefFontBtn;
	TCheckBox *GrayCheckBox;
	TCheckBox *StrCheckBox;
	TGroupBox *PrnGroupBox;
	TImage *PrvImage;
	TLabel *PrnNameLabel;
	TLabel *SizeLabel;
	TLabeledEdit *OfsXEdit;
	TLabeledEdit *OfsYEdit;
	TLabeledEdit *PrnCpsEdit;
	TLabeledEdit *ScaleEdit;
	TLabeledEdit *StrFmtEdit;
	TLabeledEdit *StrFontEdit;
	TLabeledEdit *StrMgnEdit;
	TPageControl *PageControl1;
	TPanel *LeftPanel;
	TPanel *PrviewPanel;
	TPrinterSetupDialog *PrinterSetupDialog1;
	TRadioButton *OriHBtn;
	TRadioButton *OriVBtn;
	TRadioGroup *OptRadioGroup;
	TRadioGroup *StrAlRadioGroup;
	TRadioGroup *StrPosRadioGroup;
	TSpeedButton *EndBtn;
	TSpeedButton *NextBtn;
	TSpeedButton *PrevBtn;
	TSpeedButton *TopBtn;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TUpDown *CpsUpDown;
	TUpDown *OfsXUpDown;
	TUpDown *OfsYUpDown;
	TUpDown *ScaleUpDown;
	TUpDown *StrMgnUpDown;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall PrnPropBtnClick(TObject *Sender);
	void __fastcall OptionChanged(TObject *Sender);
	void __fastcall OptionUpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall PrintActionExecute(TObject *Sender);
	void __fastcall PrintActionUpdate(TObject *Sender);
	void __fastcall RefFontBtnClick(TObject *Sender);
	void __fastcall NextBtnClick(TObject *Sender);
	void __fastcall PrevBtn_Click(TObject *Sender);
	void __fastcall TopBtnClick(TObject *Sender);
	void __fastcall EndBtnClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// ���[�U�[�錾
	bool DlgInitialized;
	TFont *StrFont;
	UnicodeString FormatStr;

public:		// ���[�U�[�錾
	file_rec *FileRec;

	__fastcall TPrintImgDlg(TComponent* Owner);
	void __fastcall DrawImage(TCanvas *cv = NULL);
	void __fastcall UpdatePreview();
	void __fastcall ClearPreview();
};
//---------------------------------------------------------------------------
extern PACKAGE TPrintImgDlg *PrintImgDlg;
//---------------------------------------------------------------------------
#endif
