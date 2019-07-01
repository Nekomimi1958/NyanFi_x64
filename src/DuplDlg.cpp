//----------------------------------------------------------------------//
// NyanFi																//
//  重複ファイルの検索ダイアログ										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "UserFunc.h"
#include "Global.h"
#include "DuplDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindDuplDlg *FindDuplDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TFindDuplDlg::TFindDuplDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindDuplDlg::FormCreate(TObject *Sender)
{
	set_ComboBoxText(AlgComboBox, HASH_ALG_LIST);
}
//---------------------------------------------------------------------------
void __fastcall TFindDuplDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	AlgComboBox->ItemIndex	 = IniFile->ReadIntGen(_T("FindDuplAlg"));
	MaxSizeEdit->Text		 = IniFile->ReadStrGen(_T("FindDuplMax"),		"128");
	OppPathCheckBox->Checked = IniFile->ReadBoolGen(_T("FindDuplOppPath"));
	SubDirCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindDuplSubDir"));
	ExcSymCheckBox->Checked	 = IniFile->ReadBoolGen(_T("FindDuplExcSym"),	true);
	ResLinkCheckBox->Checked = IniFile->ReadBoolGen(_T("FindDuplResLink"));

	SetDarkWinTheme(this);
}
//---------------------------------------------------------------------------
void __fastcall TFindDuplDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("FindDuplAlg"),			AlgComboBox->ItemIndex);
	IniFile->WriteStrGen(_T("FindDuplMax"),			MaxSizeEdit->Text);
	IniFile->WriteBoolGen(_T("FindDuplOppPath"),	OppPathCheckBox->Checked);
	IniFile->WriteBoolGen(_T("FindDuplSubDir"),		SubDirCheckBox);
	IniFile->WriteBoolGen(_T("FindDuplExcSym"),		ExcSymCheckBox);
	IniFile->WriteBoolGen(_T("FindDuplResLink"),	ResLinkCheckBox);
}

//---------------------------------------------------------------------------
void __fastcall TFindDuplDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+O") && OppPathCheckBox->Enabled)
										invert_CheckBox(OppPathCheckBox);
	else if (USAME_TI(KeyStr, "Alt+R")) invert_CheckBox(SubDirCheckBox);
	else if (USAME_TI(KeyStr, "Alt+S")) invert_CheckBox(ExcSymCheckBox);
	else if (USAME_TI(KeyStr, "Alt+L")) invert_CheckBox(ResLinkCheckBox);
	else	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#FindDuplDlg"));
}
//---------------------------------------------------------------------------

