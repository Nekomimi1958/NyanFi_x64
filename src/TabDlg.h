//----------------------------------------------------------------------//
// NyanFi																//
//  タブの設定															//
//----------------------------------------------------------------------//
#ifndef TabDlgH
#define TabDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TTabSetDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefHome1Btn;
	TButton *RefHome2Btn;
	TButton *RefIconBtn;
	TButton *RefWorkBtn;
	TButton *SetCurDirBtn;
	TEdit *HomeDir1Edit;
	TEdit *HomeDir2Edit;
	TEdit *WorkListEdit;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TImage *IcoImage;
	TLabeledEdit *CaptionEdit;
	TLabeledEdit *IconEdit;
	TRadioButton *Work0RadioBtn;
	TRadioButton *Work1RadioBtn;
	TRadioButton *Work2RadioBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RefIconBtnClick(TObject *Sender);
	void __fastcall IconEditChange(TObject *Sender);
	void __fastcall RefHomeBtnClick(TObject *Sender);
	void __fastcall RefWorkBtnClick(TObject *Sender);
	void __fastcall SetCurDirBtnClick(TObject *Sender);
	void __fastcall WorkRadioBtnClick(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣SetTabDlg TabIndex;
	int TabIndex;
	UnicodeString CurPath1, CurPath2;

	__fastcall TTabSetDlg(TComponent* Owner);
};
//---------------------SetTabDlg--SetTabDlg----------------------------------
extern PACKAGE TTabSetDlg *TabSetDlg;
//---------------------------------------------------------------------------
#endif
