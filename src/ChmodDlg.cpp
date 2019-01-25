//----------------------------------------------------------------------//
// NyanFi																//
//  パーミッション設定													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "ChmodDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFtpChmodDlg *FtpChmodDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TFtpChmodDlg::TFtpChmodDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFtpChmodDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	InhChange = false;
}
//---------------------------------------------------------------------------
void __fastcall TFtpChmodDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
}
//---------------------------------------------------------------------------
void __fastcall TFtpChmodDlg::PrmCheckBoxClick(TObject *Sender)
{
	if (InhChange) return;

	int v = 0;

	if (R1CheckBox->Checked) v += 0x400;
	if (W1CheckBox->Checked) v += 0x200;
	if (X1CheckBox->Checked) v += 0x100;
	if (R2CheckBox->Checked) v += 0x040;
	if (W2CheckBox->Checked) v += 0x020;
	if (X2CheckBox->Checked) v += 0x010;
	if (R3CheckBox->Checked) v += 0x004;
	if (W3CheckBox->Checked) v += 0x002;
	if (X3CheckBox->Checked) v += 0x001;

	InhChange = true;
	OctalEdit->Text = IntToHex(v, 3);
	InhChange = false;
}
//---------------------------------------------------------------------------
void __fastcall TFtpChmodDlg::OctalEditChange(TObject *Sender)
{
	if (InhChange) return;

	int v1 = OctalEdit->Text.SubString(1, 1).ToIntDef(0);
	int v2 = OctalEdit->Text.SubString(2, 1).ToIntDef(0);
	int v3 = OctalEdit->Text.SubString(3, 1).ToIntDef(0);

	InhChange = true;
	R1CheckBox->Checked = (v1 & 0x04);
	W1CheckBox->Checked = (v1 & 0x02);
	X1CheckBox->Checked = (v1 & 0x01);
	R2CheckBox->Checked = (v2 & 0x04);
	W2CheckBox->Checked = (v2 & 0x02);
	X2CheckBox->Checked = (v2 & 0x01);
	R3CheckBox->Checked = (v3 & 0x04);
	W3CheckBox->Checked = (v3 & 0x02);
	X3CheckBox->Checked = (v3 & 0x01);
	InhChange = false;
}

//---------------------------------------------------------------------------
//記号表記からパーミッションを設定
//---------------------------------------------------------------------------
bool __fastcall TFtpChmodDlg::SetPermission(UnicodeString prm)
{
	if (!is_match_regex(prm, _T("^[-dl][-rwx]{9}"))) return false;

	R1CheckBox->Checked = (prm[2]=='r');
	W1CheckBox->Checked = (prm[3]=='w');
	X1CheckBox->Checked = (prm[4]=='x');
	R2CheckBox->Checked = (prm[5]=='r');
	W2CheckBox->Checked = (prm[6]=='w');
	X2CheckBox->Checked = (prm[7]=='x');
	R3CheckBox->Checked = (prm[8]=='r');
	W3CheckBox->Checked = (prm[9]=='w');
	X3CheckBox->Checked = (prm[10]=='x');
	return true;
}

//---------------------------------------------------------------------------
void __fastcall TFtpChmodDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------
