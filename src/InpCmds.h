//----------------------------------------------------------------------//
// NyanFi																//
// �R�}���h�̓���														//
//----------------------------------------------------------------------//
#ifndef InpCmdsH
#define InpCmdsH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class TInpCmdsDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TButton *CancelBtn;
	TCheckBox *MigemoCheckBox;
	TComboBox *CmdsComboBox;
	TComboBox *SubComboBox;
	TPanel *MainPanel;
	TPanel *ModePanel;
	TTabControl *ModeTabControl;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CmdsComboBoxChange(TObject *Sender);
	void __fastcall CmdsComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CmdsComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall SubComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall SubComboBoxClick(TObject *Sender);
	void __fastcall MigemoCheckBoxClick(TObject *Sender);
	void __fastcall SubComboBoxCloseUp(TObject *Sender);
	void __fastcall CmdsComboBoxSelect(TObject *Sender);
	void __fastcall ModeTabControlChange(TObject *Sender);
	void __fastcall ModeTabControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);

private:	// ���[�U�[�錾
	TStringList *ItemList;		//��⃊�X�g
	TStringList *SubCmdList;	//�⏕�R�}���h���X�g
	TStringList *HistoryList;
	UnicodeString IdChar;
	bool InhFilter;
	bool IsRef;

	void __fastcall SetList();
	void __fastcall Filter();

public:		// ���[�U�[�錾
	bool toCopy;
	bool toRefer;

	__fastcall TInpCmdsDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TInpCmdsDlg *InpCmdsDlg;
//---------------------------------------------------------------------------
#endif
