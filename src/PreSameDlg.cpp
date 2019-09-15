//----------------------------------------------------------------------//
// NyanFi																//
//  同名時処理の事前指定ダイアログ										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "Global.h"
#include "PreSameDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPreSameNemeDlg *PreSameNemeDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TPreSameNemeDlg::TPreSameNemeDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPreSameNemeDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	SetDarkWinTheme(this);

	PreMode = 0;
	set_focus_RadioGroup(PreModeRadioGroup, 0);
}
//---------------------------------------------------------------------------
void __fastcall TPreSameNemeDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	PreMode = PreModeRadioGroup->ItemIndex;
}

//---------------------------------------------------------------------------
void __fastcall TPreSameNemeDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(contained_wd_i(_T("R|Alt+R"), KeyStr)) set_focus_RadioGroup(PreModeRadioGroup, 0);
	else if (contained_wd_i(_T("O|Alt+O"), KeyStr)) set_focus_RadioGroup(PreModeRadioGroup, 1);
	else if (contained_wd_i(_T("N|Alt+N"), KeyStr)) set_focus_RadioGroup(PreModeRadioGroup, 2);
	else if (contained_wd_i(_T("S|Alt+S"), KeyStr)) set_focus_RadioGroup(PreModeRadioGroup, 3);
	else if (contained_wd_i(_T("U|Alt+U"), KeyStr)) set_focus_RadioGroup(PreModeRadioGroup, 4);
	else											SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------
