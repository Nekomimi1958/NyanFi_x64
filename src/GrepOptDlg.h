//----------------------------------------------------------------------//
// NyanFi																//
//  GREP拡張設定														//
//----------------------------------------------------------------------//
#ifndef GrepOptDlgH
#define GrepOptDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TGrepExOptDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefAppBtn;
	TButton *RefBakBtn;
	TButton *RefDirBtn;
	TButton *RefLogBtn;
	TButton *RefOutBtn;
	TCheckBox *AppendCheckBox;
	TCheckBox *AppendLogCheckBox;
	TCheckBox *BakRepCheckBox;
	TCheckBox *OpenLogCheckBox;
	TCheckBox *RepCrCheckBox;
	TCheckBox *RepTabCheckBox;
	TCheckBox *SaveLogCheckBox;
	TCheckBox *TrimLeftCheckBox;
	TEdit *RepCrEdit;
	TGroupBox *AppGroupBox;
	TGroupBox *BakGroupBox;
	TGroupBox *FileGroupBox;
	TGroupBox *LogGroupBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabeledEdit *AppDirEdit;
	TLabeledEdit *AppNameEdit;
	TLabeledEdit *AppParamEdit;
	TLabeledEdit *FExtBakEdit;
	TLabeledEdit *FileFmtEdit;
	TLabeledEdit *InsStrW2Edit;
	TLabeledEdit *InsStrWEdit;
	TLabeledEdit *LogFileEdit;
	TLabeledEdit *OutFileEdit;
	TLabeledEdit *RepBakDirEdit;
	TMemo *SampleMemo;
	TPageControl *PageControl1;
	TPanel *BottomPanel;
	TPanel *GrepOpt1Panel;
	TPanel *GrepOpt2Panel;
	TPanel *GrepOpt3Panel;
	TRadioGroup *OutModeRadioGroup;
	TTabSheet *OutFormSheet;
	TTabSheet *OutModeSheet;
	TTabSheet *ReplaceSheet;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall RefAppBtnClick(TObject *Sender);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall RefOutBtnClick(TObject *Sender);
	void __fastcall OutModeRadioGroupClick(TObject *Sender);
	void __fastcall SampleChange(TObject *Sender);
	void __fastcall RefLogBtnClick(TObject *Sender);
	void __fastcall RefBakBtnClick(TObject *Sender);
	bool __fastcall FormHelp(WORD Command, NativeInt Data, bool &CallHelp);

private:	// ユーザー宣言
	void __fastcall WmDropped(TMessage &msg);

public:		// ユーザー宣言
	__fastcall TGrepExOptDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,	WmDropped)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGrepExOptDlg *GrepExOptDlg;
//---------------------------------------------------------------------------
#endif
