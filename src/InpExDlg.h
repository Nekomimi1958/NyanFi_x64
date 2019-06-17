//----------------------------------------------------------------------//
// NyanFi																//
//  拡張入力ボックス													//
//----------------------------------------------------------------------//
#ifndef InpExDlgH
#define InpExDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "Global.h"

//---------------------------------------------------------------------------
//拡張モード
#define INPEX_CRE_DIR		1
#define INPEX_NEW_TEXTFILE	2
#define INPEX_CLONE			3
#define INPEX_FUNCTIONKEY	4
#define INPEX_CRE_TESTFILE	5
#define INPEX_JUMP_LINE		6
#define INPEX_JUMP_ADDR		7
#define INPEX_SET_TOPADDR	8
#define INPEX_FIND_TAG		10
#define INPEX_ADD_TAG		11
#define INPEX_SET_TAG		12
#define INPEX_TAG_SELECT	13
#define INPEX_CLIP_PASTE	14

//---------------------------------------------------------------------------
class TInputExDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *CancelBtn;
	TButton *OkBtn;
	TCheckBox *ClipCheckBox;
	TCheckBox *CnvChCheckBox;
	TCheckBox *DirChgCheckBox;
	TCheckBox *EditCheckBox;
	TCheckBox *SelDefCheckBox;
	TComboBox *CodePageComboBox;
	TComboBox *InputComboBox;
	TLabel *CodePageLabel;
	TLabel *NameInfLabel;
	TLabel *NameLabel;
	TLabel *PathInfLabel;
	TLabeledEdit *InputEdit;
	TLabeledEdit *TestCntEdit;
	TLabeledEdit *TestSizeEdit;
	TPanel *BottomPanel;
	TPanel *CreDirPanel;
	TPanel *CreTestPanel;
	TPanel *IniSttPanel;
	TPanel *InputPanel;
	TPanel *NewTextPanel;
	TPanel *NotationPanel;
	TRadioButton *DecRadioBtn;
	TRadioButton *HexRadioBtn;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OptionClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InputEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InputEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall SelDefCheckBoxClick(TObject *Sender);
	void __fastcall InputComboBoxChange(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString HelpTopic;

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
	int  IpuntExMode;	//拡張モード	(0 の場合は単純な入力ボックス)
	int  CustomWidth;	//カスタム表示幅

	UnicodeString PathName;

	__fastcall TInputExDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TInputExDlg *InputExDlg;

//---------------------------------------------------------------------------
UnicodeString inputbox_ex(const _TCHAR *tit, const _TCHAR *prm = NULL, UnicodeString def = EmptyStr, bool num_only = false);
int inputbox_ex_n(const _TCHAR *tit, const _TCHAR *prm = NULL, UnicodeString def_s = EmptyStr, int def_n = 0);

bool input_query_ex(const _TCHAR *tit, const _TCHAR *prm, UnicodeString *value, int width = 0, bool num_only = false,
	UnicodeString hint = EmptyStr);
bool input_query_ex(int tid, const _TCHAR *prm, UnicodeString *value, int width = 0, bool num_only = false,
	UnicodeString hint = EmptyStr);

//---------------------------------------------------------------------------
#endif
