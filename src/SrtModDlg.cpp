//----------------------------------------------------------------------//
// NyanFi																//
//  ソートダイアログ													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "Global.h"
#include "UserFunc.h"
#include "SrtModDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSortModeDlg *SortModeDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TSortModeDlg::TSortModeDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormCreate(TObject *Sender)
{
	set_ComboBoxText(DirSortModeComboBox,
		_T("ファイルと同じ\n名前\n更新日時\nサイズ\n属性\nディレクトリを区別しない\nアイコン(ファイル:名前/拡張子)\n"));
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	DirOptPanel->Visible = (ScrMode==SCMD_FLIST);
	int hi = ModePanel->Height + BtnPanel->Height;
	if (DirOptPanel->Visible) hi += DirOptPanel->Height;
	ClientHeight = hi;

	Changed  = false;
	SelByKey = false;
	InhOk	 = true;

	Caption = "ソート - " + get_LRUD_str(CurListTag, SortBoth);

	SortModeRadioGroup->ItemIndex  = SortMode[CurListTag];
	DirSortModeComboBox->ItemIndex = DirSortMode[CurListTag];
	NaturalCheckBox->Checked	   = FlOdrNatural[CurListTag];
	DscNameCheckBox->Checked	   = FlOdrDscName[CurListTag];
	OldCheckBox->Checked		   = FlOdrOld[CurListTag];
	SmallCheckBox->Checked		   = FlOdrSmall[CurListTag];
	DscAttrCheckBox->Checked	   = FlOdrDscAttr[CurListTag];
	SortBothCheckBox->Checked	   = SortBoth;
	SortModeRadioGroup->SetFocus();

	this->Perform(WM_NEXTDLGCTL, 0, (NativeInt)0);
	InhOk = false;

	SetDarkWinTheme(this);
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (ModalResult==mrOk) {
		SortMode[CurListTag]	 = SortModeRadioGroup->ItemIndex;
		DirSortMode[CurListTag]  = DirSortModeComboBox->ItemIndex;
		FlOdrNatural[CurListTag] = NaturalCheckBox->Checked;
		FlOdrDscName[CurListTag] = DscNameCheckBox->Checked;
		FlOdrOld[CurListTag]	 = OldCheckBox->Checked;
		FlOdrSmall[CurListTag]	 = SmallCheckBox->Checked;
		FlOdrDscAttr[CurListTag] = DscAttrCheckBox->Checked;

		SortBoth = SortBothCheckBox->Checked;
		if (SortBoth) {
			SortMode[OppListTag]	 = SortMode[CurListTag];
			DirSortMode[OppListTag]  = DirSortMode[CurListTag];
			FlOdrNatural[OppListTag] = FlOdrNatural[CurListTag];
			FlOdrDscName[OppListTag] = FlOdrDscName[CurListTag];
			FlOdrOld[OppListTag]	 = FlOdrOld[CurListTag];
			FlOdrSmall[OppListTag]	 = FlOdrSmall[CurListTag];
			FlOdrDscAttr[OppListTag] = FlOdrDscAttr[CurListTag];
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortBothCheckBoxClick(TObject *Sender)
{
	Caption = "ソート - " + get_LRUD_str(CurListTag, SortBothCheckBox->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupClick(TObject *Sender)
{
	if (!InhOk) {
		Changed = true;
		if (SortModeRadioGroup->Enabled) ModalResult = mrOk;
	}
	else {
		InhOk = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::OptCheckBoxClick(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupEnter(TObject *Sender)
{
	if (SortModeRadioGroup->Enabled && SelByKey)
		ModalResult = Changed? mrOk : mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupExit(TObject *Sender)
{
	SelByKey = false;
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	int idx = -1;
	if		(contained_wd_i("F|Alt+F", KeyStr)) idx = 0;
	else if (contained_wd_i("E|Alt+E", KeyStr)) idx = 1;
	else if (contained_wd_i("D|Alt+D", KeyStr)) idx = 2;
	else if (contained_wd_i("S|Alt+S", KeyStr)) idx = 3;
	else if (contained_wd_i("A|Alt+A", KeyStr)) idx = 4;
	else if (contained_wd_i("U|Alt+U", KeyStr)) idx = 5;

	if (idx!=-1) {
		if (SortModeRadioGroup->Enabled) {
			if (idx==SortModeRadioGroup->ItemIndex) {
				SelByKey = true;	//現在のモードのキーが押された
				Key = 0;
			}
			else {
				SortModeRadioGroup->ItemIndex = idx;
			}
		}
	}
	else {
		if		(USAME_TI(KeyStr, "Alt+N")) invert_CheckBox(NaturalCheckBox);
		else if (USAME_TI(KeyStr, "Alt+R")) invert_CheckBox(DscNameCheckBox);
		else if (USAME_TI(KeyStr, "Alt+O")) invert_CheckBox(OldCheckBox);
		else if (USAME_TI(KeyStr, "Alt+M")) invert_CheckBox(SmallCheckBox);
		else if (USAME_TI(KeyStr, "Alt+V")) invert_CheckBox(DscAttrCheckBox);
		else if (USAME_TI(KeyStr, "Alt+B")) invert_CheckBox(SortBothCheckBox);
		else SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#SortDlg"));
	}
}
//---------------------------------------------------------------------------

