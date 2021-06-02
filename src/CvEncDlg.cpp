//----------------------------------------------------------------------//
// NyanFi																//
//  文字コードの変換													//
//----------------------------------------------------------------------//
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "CvEncDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCvTxtEncDlg *CvTxtEncDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TCvTxtEncDlg::TCvTxtEncDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCvTxtEncDlg::FormCreate(TObject *Sender)
{
	for (int i=0; i<MAX_SAVE_CODEPAGES; i++) {
		if (!SameText(SaveCodePages[i].name, "UTF-8N")) OutCodeComboBox->Items->Add(SaveCodePages[i].name);
	}

	set_ComboBoxText(OutLnBrkComboBox, _T("CR/LF\r\nLF\r\nCR\r\n"));
}
//---------------------------------------------------------------------------
void __fastcall TCvTxtEncDlg::FormShow(TObject *Sender)
{
	Caption = "文字コードの変換" + TitleInf;

	IniFile->LoadPosInfo(this, DialogCenter);
	OutCodeComboBox->ItemIndex	= IniFile->ReadIntGen(_T("CvTextEncCode"));
	OutLnBrkComboBox->ItemIndex = IniFile->ReadIntGen(_T("CvTextEncLnBrk"));
	BomCheckBox->Checked		= IniFile->ReadBoolGen(_T("CvTextEncBOM"), true);
	OutCodeComboBoxClick(NULL);

	SetDarkWinTheme(this);
}
//---------------------------------------------------------------------------
void __fastcall TCvTxtEncDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("CvTextEncCode"),	OutCodeComboBox);
	IniFile->WriteIntGen(_T("CvTextEncLnBrk"),	OutLnBrkComboBox);
	IniFile->WriteBoolGen(_T("CvTextEncBOM"),	BomCheckBox);

	TitleInf = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TCvTxtEncDlg::OutCodeComboBoxClick(TObject *Sender)
{
	BomCheckBox->Enabled = OutCodeComboBox->Text.Pos("UTF");
}

//---------------------------------------------------------------------------
void __fastcall TCvTxtEncDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#ConvertTextEnc"));
}
//---------------------------------------------------------------------------

