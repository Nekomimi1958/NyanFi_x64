//----------------------------------------------------------------------//
// NyanFi																//
// ディレクトリを開く													//
//----------------------------------------------------------------------//
#ifndef InpDirH
#define InpDirH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TInpDirDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefDirBtn;
	TCheckBox *CmpByKeyCheckBox;
	TComboBox *InpDirComboBox;
	TComboBox *SubComboBox;
	TPanel *BottomPanel;
	TPanel *CmpByKeyPanel;
	TPanel *InpPanel;
	TPanel *RefBtnPanel;
	TPanel *TopPanel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall InpDirComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InpDirComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall InpDirComboBoxChange(TObject *Sender);
	void __fastcall InpDirComboBoxSelect(TObject *Sender);
	void __fastcall SubComboBoxClick(TObject *Sender);
	void __fastcall SubComboBoxCloseUp(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	bool isWebSea;
	bool NoCheck;

	TStringList *ItemList;		//候補リスト
	bool InhFilter;

	UnicodeString CurSeaPath;

	UnicodeString __fastcall GetInpWord(bool path_sw = false);
	void __fastcall SetList();
	void __fastcall Filter();

public:		// ユーザー宣言
	UnicodeString InitialPath;
	UnicodeString CommandStr;

	__fastcall TInpDirDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInpDirDlg *InpDirDlg;
//---------------------------------------------------------------------------
#endif
