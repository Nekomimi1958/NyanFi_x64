//----------------------------------------------------------------------//
// NyanFi																//
//  �����񌟍�(�e�L�X�g�r���A�[)										//
//----------------------------------------------------------------------//
#ifndef FindTxtDlgH
#define FindTxtDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include "ExTViewer.h"
#include "TxtViewer.h"

//---------------------------------------------------------------------------
class TFindTextDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *FindNextAction;
	TActionList *ActionList1;
	TButton *CalcelBtn;
	TButton *FindBtn;
	TCheckBox *BytesCheckBox;
	TCheckBox *CaseCheckBox;
	TCheckBox *CloseCheckBox;
	TCheckBox *HighlightCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *RegExCheckBox;
	TComboBox *CodePageComboBox;
	TComboBox *FindComboBox;
	TGroupBox *FindDirGroupBox;
	TLabel *KwdLabel;
	TPanel *BinPanel;
	TPanel *HighlightPanel;
	TRadioButton *DownRadioBtn;
	TRadioButton *UpRadioBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SubOptClick(TObject *Sender);
	void __fastcall FindComboBoxChange(TObject *Sender);
	void __fastcall FindComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FindOptChangedClick(TObject *Sender);
	void __fastcall FindNextActionUpdate(TObject *Sender);
	void __fastcall FindNextActionExecute(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegExCheckBoxClick(TObject *Sender);
	void __fastcall MigemoCheckBoxClick(TObject *Sender);

private:	// ���[�U�[�錾
	TTxtViewer *Viewer;
	bool DlgInitialized;
	bool KeyHandled;

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ���[�U�[�錾
	bool fromTV;		//�e�L�X�g�r���A�[����

	TExTxtViewer *ExTViewer;

	UnicodeString RegExPtn;

	__fastcall TFindTextDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFindTextDlg *FindTextDlg;
//---------------------------------------------------------------------------
#endif
