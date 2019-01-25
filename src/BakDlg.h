//----------------------------------------------------------------------//
// NyanFi																//
//  �o�b�N�A�b�v�_�C�A���O												//
//----------------------------------------------------------------------//
#ifndef BakDlgH
#define BakDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TBackupDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *MakeNbtAction;
	TAction *SaveSetupAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TButton *Button1;
	TButton *CanButton;
	TButton *OkButton;
	TCheckBox *MirrorCheckBox;
	TCheckBox *SubDirCheckBox;
	TCheckBox *SureCheckBox;
	TCheckBox *SyncCheckBox;
	TComboBox *BakExcMaskComboBox;
	TComboBox *BakIncMaskComboBox;
	TComboBox *SetupComboBox;
	TLabel *Label1;
	TLabeledEdit *BakDateCondEdit;
	TLabeledEdit *BakSkipDirEdit;
	TLabeledEdit *DstDirEdit;
	TLabeledEdit *SrcDirEdit;
	TPanel *BottomPanel;
	TSpeedButton *SaveFmtBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SaveSetupActionExecute(TObject *Sender);
	void __fastcall SaveSetupActionUpdate(TObject *Sender);
	void __fastcall SetupComboBoxClick(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall MakeNbtActionExecute(TObject *Sender);
	void __fastcall MakeNbtActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	__fastcall TBackupDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBackupDlg *BackupDlg;
//---------------------------------------------------------------------------
#endif
