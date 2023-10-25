//----------------------------------------------------------------------//
// NyanFi																//
//  ソートダイアログ													//
//----------------------------------------------------------------------//
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
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
	UserModule->SetUsrPopupMenu(this);

	set_ComboBoxText(DirSortModeComboBox,
		_T("ファイルと同じ\n名前\n更新日時\nサイズ\n属性\nディレクトリを区別しない\nアイコン(ファイル:名前/拡張子)\n"));

	ExtListEdit->Hint = LoadUsrMsg(USTR_HintMltFExt);
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormShow(TObject *Sender)
{
	DlgInitialized = false;
	InhOk = false;

	IniFile->LoadPosInfo(this, DialogCenter);
	AccDtoTCheckBox->Checked   = IniFile->ReadBoolGen(_T("SortModeAccDtoT"));
	SameCloseCheckBox->Checked = IniFile->ReadBoolGen(_T("SortModeSameClose"));
	ExOptCheckBox->Checked	   = IniFile->ReadBoolGen(_T("SortModeExOpt"));

	DirOptPanel->Visible = (ScrMode==SCMD_FLIST);
	int hi = ModePanel->Height + BtnPanel->Height;
	if (DirOptPanel->Visible) hi += DirOptPanel->Height;
	ClientHeight = hi;
	ClientWidth  = ExOptCheckBox->Checked? MainPanel->Width * 2 : MainPanel->Width;

	Changed  = false;
	SelByKey = false;
	Caption = "ソート - " + get_LRUD_str(CurListTag, SortBoth);
	SortModeRadioGroup->ItemIndex  = SortMode[CurListTag];
	if (SortModeRadioGroup->ItemIndex!=5) {
		PrimeComboBox->ItemIndex = SortModeRadioGroup->ItemIndex;
		PrimeComboBoxClick(NULL);
	}
	else {
		PrimeComboBox->ItemIndex = -1;
		for (int i=0; i<SubModeRadioGroup->Items->Count; i++)
			SubModeRadioGroup->Buttons[i]->Enabled = false;
		SubModeRadioGroup->ItemIndex = -1;
	}

	DirSortModeComboBox->ItemIndex = DirSortMode[CurListTag];
	NaturalCheckBox->Checked       = FlOdrNatural[CurListTag];
	DscNameCheckBox->Checked       = FlOdrDscName[CurListTag];
	OldCheckBox->Checked           = FlOdrOld[CurListTag];
	SmallCheckBox->Checked         = FlOdrSmall[CurListTag];
	DscAttrCheckBox->Checked       = FlOdrDscAttr[CurListTag];
	SortBothCheckBox->Checked      = SortBoth;
	SortLogicalCheckBox->Checked   = SortLogical;
	ExtListEdit->Text              = SortExtList;

	SortModeRadioGroup->SetFocus();
	this->Perform(WM_NEXTDLGCTL, 0, (NativeInt)0);
	DlgInitialized = true;

	SetDarkWinTheme(this);
	SetAccDT();
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("SortModeAccDtoT"),	AccDtoTCheckBox);
	IniFile->WriteBoolGen(_T("SortModeSameClose"),	SameCloseCheckBox);
	IniFile->WriteBoolGen(_T("SortModeExOpt"),		ExOptCheckBox);

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

		SortLogical = SortLogicalCheckBox->Checked;
		SortExtList = ExtListEdit->Text;
	}
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#SortDlg"))) return;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	int idx = -1;
	if		(contained_wd_i("F|Alt+F", KeyStr)) idx = 0;
	else if (contained_wd_i("E|Alt+E", KeyStr)) idx = 1;
	else if (AccDtoTCheckBox->Checked? contained_wd_i("T|Alt+T", KeyStr) : contained_wd_i("D|Alt+D", KeyStr)) idx = 2;
	else if (contained_wd_i("S|Alt+S", KeyStr)) idx = 3;
	else if (contained_wd_i("A|Alt+A", KeyStr)) idx = 4;
	else if (contained_wd_i("U|Alt+U", KeyStr)) idx = 5;

	if (idx!=-1) {
		if (idx==SortModeRadioGroup->ItemIndex) {
			SelByKey = true;	//現在のモードのキーが押された
			Key = 0;
			if (SameCloseCheckBox->Checked) {
				Application->ProcessMessages();		//！これがないとフォーカスエラーになる
				ModalResult = mrOk;
			}
		}
		else {
			SortModeRadioGroup->ItemIndex = idx;
			Application->ProcessMessages();		//！これがないとフォーカスエラーになる
		}
	}
	else {
		if		(SameText(KeyStr, "Alt+N")) invert_CheckBox(NaturalCheckBox);
		else if (SameText(KeyStr, "Alt+R")) invert_CheckBox(DscNameCheckBox);
		else if (SameText(KeyStr, "Alt+O")) invert_CheckBox(OldCheckBox);
		else if (SameText(KeyStr, "Alt+M")) invert_CheckBox(SmallCheckBox);
		else if (SameText(KeyStr, "Alt+V")) invert_CheckBox(DscAttrCheckBox);
		else if (SameText(KeyStr, "Alt+B")) invert_CheckBox(SortBothCheckBox);
	}
}
//---------------------------------------------------------------------------
//更新日時のアクセラレータを設定
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SetAccDT()
{
	bool ok = false;
	UnicodeString dt_s;  dt_s.sprintf(_T("更新日時(%c)"), AccDtoTCheckBox->Checked? 'T' : 'D');
	for (int i=0; i<SortModeRadioGroup->Items->Count; i++) {
		if (StartsStr("更新日時", SortModeRadioGroup->Items->Strings[i])) {
			SortModeRadioGroup->Items->Strings[i] = dt_s;
			ok = true;
			break;
		}
	}
	if (!ok) {
		for (int i=0; i<SortModeRadioGroup->ControlCount; i++) {
			TControl *cp = SortModeRadioGroup->Controls[i];
			if (cp->ClassNameIs("TLabel")) {
				TLabel *lp = (TLabel *)cp;
				if (StartsStr("更新日時", lp->Caption)) {
					lp->Caption = dt_s;
					break;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortBothCheckBoxClick(TObject *Sender)
{
	Caption = "ソート - " + get_LRUD_str(CurListTag, SortBothCheckBox->Checked);
}

//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::OptCheckBoxClick(TObject *Sender)
{
	Changed = true;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupClick(TObject *Sender)
{
	if (!DlgInitialized) return;

	if (!InhOk) {	//TNyanFiForm::ApplicationEvents1Message でカーソルキーを抑止
		Changed = true;
		ModalResult = mrOk;
	}
	else {
		InhOk = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupEnter(TObject *Sender)
{
	if (SelByKey) ModalResult = Changed? mrOk : mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SortModeRadioGroupExit(TObject *Sender)
{
	SelByKey = false;
}

//---------------------------------------------------------------------------
//拡張設定
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::ExOptActionExecute(TObject *Sender)
{
	ClientWidth = MainPanel->Width * 2;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::ExOptActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = !ExOptCheckBox->Checked;
	ap->Enabled = (ClientWidth == MainPanel->Width);
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::OkActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = true;
	ExOptAction->Visible = !ExOptCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::PrimeComboBoxClick(TObject *Sender)
{
	int idx = PrimeComboBox->ItemIndex;
	for (int i=0; i<SubModeRadioGroup->Items->Count; i++) {
		TRadioButton *bp = SubModeRadioGroup->Buttons[i];
		bp->Enabled = (idx==0)? (i==5) : (i!=idx);
	}
	SubModeRadioGroup->ItemIndex = SubSortMode[idx];
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::SubModeRadioGroupClick(TObject *Sender)
{
	if (PrimeComboBox->ItemIndex!=-1)
		SubSortMode[PrimeComboBox->ItemIndex] = SubModeRadioGroup->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TSortModeDlg::AccDtoTCheckBoxClick(TObject *Sender)
{
	SetAccDT();
}
//---------------------------------------------------------------------------

