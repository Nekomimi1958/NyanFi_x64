//----------------------------------------------------------------------//
// NyanFi																//
// �f�B���N�g�����J��													//
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
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *OkAction;
	TActionList *ActionList1;
	TButton *CanButton;
	TButton *OkButton;
	TButton *RefDirBtn;
	TCheckBox *CmpByKeyCheckBox;
	TComboBox *InpDirComboBox;
	TComboBox *SubComboBox;
	TPanel *BottomPanel;
	TPanel *InpPanel;
	TPanel *RefBtnPanel;
	TPanel *TopPanel;
	TPanel *CmpByKeyPanel;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall OkActionUpdate(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SubComboBoxClick(TObject *Sender);
	void __fastcall SubComboBoxCloseUp(TObject *Sender);
	void __fastcall InpDirComboBoxChange(TObject *Sender);
	void __fastcall InpDirComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InpDirComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall InpDirComboBoxSelect(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);

private:	// ���[�U�[�錾
	bool isWebSea;
	bool NoCheck;

	TStringList *ItemList;		//��⃊�X�g
	bool InhFilter;

	UnicodeString CurSeaPath;

	UnicodeString __fastcall GetInpWord(bool path_sw = false);
	void __fastcall SetList();
	void __fastcall Filter();

public:		// ���[�U�[�錾
	UnicodeString InitialPath;
	UnicodeString CommandStr;

	__fastcall TInpDirDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInpDirDlg *InpDirDlg;
//---------------------------------------------------------------------------
#endif
