//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリの比較ダイアログ										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "DiffDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDiffDirDlg *DiffDirDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TDiffDirDlg::TDiffDirDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDiffDirDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	IncMaskComboBox->Tag	= CBTAG_HISTORY;
	ExcMaskComboBox->Tag	= CBTAG_HISTORY;
	DiffExcDirComboBox->Tag = CBTAG_HISTORY;

	UnicodeString hntstr = LoadUsrMsg(USTR_HintMltSepSC);
	DiffExcDirComboBox->Hint = hntstr;
	IncMaskComboBox->Hint	 = hntstr;
	ExcMaskComboBox->Hint	 = hntstr;
}
//---------------------------------------------------------------------------
void __fastcall TDiffDirDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	IniFile->LoadPosInfo(this, DialogCenter);

	IniFile->LoadComboBoxItems(IncMaskComboBox,		_T("DiffIncMaskHistory"));
	IniFile->LoadComboBoxItems(ExcMaskComboBox,		_T("DiffExcMaskHistory"));
	IniFile->LoadComboBoxItems(DiffExcDirComboBox,	_T("DiffExcDirHistory"));

	IncMaskComboBox->Text	 = IniFile->ReadStrGen(_T("DiffIncMask"), "*.*");
	ExcMaskComboBox->Text	 = IniFile->ReadStrGen(_T("DiffExcMask"));
	DiffExcDirComboBox->Text = IniFile->ReadStrGen(_T("DiffExcDir"));
	SubDirCheckBox->Checked  = IniFile->ReadBoolGen(_T("DiffSubDir"));

	IncMaskComboBox->SetFocus();
	IncMaskComboBox->SelStart = IncMaskComboBox->Text.Length();
}
//---------------------------------------------------------------------------
void __fastcall TDiffDirDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (ModalResult==mrOk) {
		if (IncMaskComboBox->Text.IsEmpty()) IncMaskComboBox->Text = "*.*";
		add_ComboBox_history(IncMaskComboBox);
		add_ComboBox_history(ExcMaskComboBox);
		add_ComboBox_history(DiffExcDirComboBox);
	}

	IniFile->SaveComboBoxItems(IncMaskComboBox,		_T("DiffIncMaskHistory"));
	IniFile->SaveComboBoxItems(ExcMaskComboBox,		_T("DiffExcMaskHistory"));
	IniFile->SaveComboBoxItems(DiffExcDirComboBox,	_T("DiffExcDirHistory"));

	IniFile->WriteStrGen(_T("DiffIncMask"),	IncMaskComboBox->Text);
	IniFile->WriteStrGen(_T("DiffExcMask"),	ExcMaskComboBox->Text);
	IniFile->WriteStrGen(_T("DiffExcDir"),	DiffExcDirComboBox->Text);
	IniFile->WriteBoolGen(_T("DiffSubDir"),	SubDirCheckBox);
}

//---------------------------------------------------------------------------
void __fastcall TDiffDirDlg::StartActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = true;

	DiffExcDirComboBox->Color = SubDirCheckBox->Checked? scl_Window : col_Invalid;
}

//---------------------------------------------------------------------------
void __fastcall TDiffDirDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#DiffDir"));
}
//---------------------------------------------------------------------------

