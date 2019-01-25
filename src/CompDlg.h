//----------------------------------------------------------------------//
// NyanFi																//
//  同名ファイルの比較ダイアログ										//
//----------------------------------------------------------------------//
#ifndef CompDlgH
#define CompDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TFileCompDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *CmpArcCheckBox;
	TCheckBox *CmpDirCheckBox;
	TCheckBox *ReverseCheckBox;
	TCheckBox *SelMaskCheckBox;
	TCheckBox *SelOppCheckBox;
	TPanel *HashPanel;
	TPanel *SizePanel;
	TPanel *TimePanel;
	TRadioGroup *AlgRadioGroup;
	TRadioGroup *HashRadioGroup;
	TRadioGroup *SizeRadioGroup;
	TRadioGroup *TimeRadioGroup;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OkActionUpdate(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	bool AllDirHasSize;		//すべての対象ディレクトリがサイズ取得済み

	__fastcall TFileCompDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFileCompDlg *FileCompDlg;
//---------------------------------------------------------------------------
#endif
