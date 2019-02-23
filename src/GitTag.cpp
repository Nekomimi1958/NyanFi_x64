//----------------------------------------------------------------------//
// NyanFi																//
//  GitÉ^ÉOê›íË															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "GitTag.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSetGitTagDlg *SetGitTagDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TSetGitTagDlg::TSetGitTagDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSetGitTagDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	AnnotateCheckBox->Checked = IniFile->ReadBoolGen(_T("GitTagAnnotate"));

	NameEdit->Text = EmptyStr;
	AnnotateMemo->Lines->Clear();
	GitParam = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TSetGitTagDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("GitTagAnnotate"),	AnnotateCheckBox->Checked);
}

//---------------------------------------------------------------------------
//ämíË
//---------------------------------------------------------------------------
void __fastcall TSetGitTagDlg::OkActionExecute(TObject *Sender)
{
	GitParam.USET_T("tag");
	if (AnnotateCheckBox->Checked) {
		GitParam.cat_sprintf(_T(" -a -F \"%s\""), yen_to_slash(TagMsgFile).c_str());
		saveto_TextFile(TagMsgFile, AnnotateMemo->Lines);
	}
	GitParam.cat_sprintf(_T(" %s"), NameEdit->Text.c_str());
	if (!CommitID.IsEmpty()) GitParam.cat_sprintf(_T(" %s"), CommitID.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TSetGitTagDlg::OkActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !NameEdit->Text.IsEmpty()
			&& (!AnnotateCheckBox->Checked || !AnnotateMemo->Lines->Text.IsEmpty());

	AnnotateMemo->Color = AnnotateCheckBox->Checked? scl_Window : col_Invalid;
}
//---------------------------------------------------------------------------

