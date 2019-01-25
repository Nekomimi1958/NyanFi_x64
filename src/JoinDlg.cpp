//----------------------------------------------------------------------//
// NyanFi																//
//  �e�L�X�g�t�@�C�������_�C�A���O										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "JoinDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJoinTextDlg *JoinTextDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TJoinTextDlg::TJoinTextDlg(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::FormCreate(TObject *Sender)
{
	set_ComboBoxText(OutCodeComboBox,
		_T("����(�擪�̃R�[�h�ɓ���)\n")
		_T("Shift_JIS\n")
		_T("ISO-2022-JP\n")
		_T("EUC-JP\n")
		_T("UTF-8\n")
		_T("UTF-16\n"));

	OutCodeComboBox->ItemIndex = 0;

	set_ComboBoxText(OutLnBrkComboBox, _T("CR/LF\nLF\nCR\n"));
	OutLnBrkComboBox->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	OutCodeComboBox->ItemIndex = IniFile->ReadIntGen(_T("JoinTextOutCode"));
	TemplateEdit->Text		   = IniFile->ReadStrGen(_T("JoinTextTemplate"));
	BomCheckBox->Checked 	   = IniFile->ReadBoolGen(_T("JoinTextBOM"));

	UserModule->InitializeListBox(SrcFileListBox);
	OutNameEdit->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("JoinTextOutCode"),		OutCodeComboBox);
	IniFile->WriteStrGen(_T("JoinTextTemplate"),	TemplateEdit);
	IniFile->WriteBoolGen(_T("JoinTextBOM"),		BomCheckBox);
}

//---------------------------------------------------------------------------
//�ꗗ�ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::SrcFileListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}

//---------------------------------------------------------------------------
//�e���v���[�g�̎Q��
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::RefTpltBtnClick(TObject *Sender)
{
	UnicodeString inidir = def_if_empty(ExtractFileDir(rel_to_absdir(TemplateEdit->Text)), ExePath);
	UserModule->PrepareOpenDlg(_T("�o�̓e���v���[�g�̑I��"), F_FILTER_TXT, NULL, inidir);
	UserModule->OpenDlgToEdit(TemplateEdit, true);
}
//---------------------------------------------------------------------------
//�e���v���[�g�̕ҏW
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::EditTmpltActionExecute(TObject *Sender)
{
	open_by_TextEditor(rel_to_absdir(TemplateEdit->Text));
}
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::EditTmpltActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !TemplateEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//�J�n
//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::JoinActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = SrcFileListBox->Count>0 && !OutNameEdit->Text.IsEmpty();

	InvColIfEmpty(TemplateEdit);

	BomCheckBox->Enabled = StartsStr("UTF-", OutCodeComboBox->Text);
}

//---------------------------------------------------------------------------
void __fastcall TJoinTextDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------
