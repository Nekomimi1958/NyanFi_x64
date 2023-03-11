//----------------------------------------------------------------------//
// NyanFi																//
//  同名ファイルの処理ダイアログ										//
//----------------------------------------------------------------------//
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "SameDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSameNameDlg *SameNameDlg;

//---------------------------------------------------------------------------
__fastcall TSameNameDlg::TSameNameDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormCreate(TObject *Sender)
{
	CopyAll    = false;
	CopyMode   = 0;
	IsSamePath = false;
	TaskNo	   = -1;
	InhAllChk  = false;

	CurSrcName = CurDstName = EmptyStr;
	KeyHandled = false;
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	set_ListBoxItemHi(InfoListBox);
	InfoListBox->Color = col_bgList;

	UnicodeString tmp;
	InhAllChk = true;
	if (CopyMode!=CPYMD_MAN_REN) {
		tmp.sprintf(_T("SameDlgCopyAll%u"), TaskNo + 1);
		AllCheckBox->Checked = CopyAll = IniFile->ReadBoolGen(tmp.c_str(), true);
	}
	else {
		AllCheckBox->Checked = CopyAll = false;
	}
	InhAllChk = false;

	Mode0Btn->Enabled = !IsSamePath;
	Mode1Btn->Enabled = !IsSamePath;
	Mode2Btn->Enabled = !IsSamePath;

	if (IsSamePath && CopyMode<CPYMD_AUT_REN) CopyMode = CPYMD_MAN_REN;

	switch (CopyMode) {
	case CPYMD_OW:		Mode0Btn->Checked = true; break;
	case CPYMD_NEW:		Mode1Btn->Checked = true; break;
	case CPYMD_SKIP:	Mode2Btn->Checked = true; break;
	case CPYMD_AUT_REN:	Mode3Btn->Checked = true; break;
	case CPYMD_MAN_REN:	Mode4Btn->Checked = true; break;
	}

	ModeBtnClick(NULL);

	InfoListBox->Clear();
	InfoListBox->Items->Add(ExtractFileName(CurSrcName));

	InfoListBox->Items->Add("元: " + ExtractFileDir(CurSrcName));
	InfoListBox->Items->Add(EmptyStr);

	InfoListBox->Items->Add("先: " + ExtractFileDir(CurDstName));
	InfoListBox->Items->Add(EmptyStr);

	tmp = "サイズ: ";
	if (CurSrcSize == CurDstSize)
		tmp += "同じ";
	else
		tmp.cat_sprintf(_T("転送先の方が%s"), (CurSrcSize<CurDstSize)? _T("大きい") : _T("小さい"));
	InfoListBox->Items->Add(tmp);

	tmp = "タイム: ";
	if (WithinPastMilliSeconds(CurSrcTime, CurDstTime, TimeTolerance)) {
		tmp += "同じ";
		if (CurSrcTime!=CurDstTime) tmp += " (許容誤差内)";
	}
	else {
		tmp.cat_sprintf(_T("転送先の方が%s"), (CurSrcTime<CurDstTime)? _T("新しい") : _T("古い"));
	}
	InfoListBox->Items->Add(tmp);

	SetDarkWinTheme(this);

	if (SupportDarkMode) {
		Mode0Label->Font->Color = Mode0Btn->Enabled? get_LabelColor() : clGrayText;
		Mode1Label->Font->Color = Mode1Btn->Enabled? get_LabelColor() : clGrayText;
		Mode2Label->Font->Color = Mode2Btn->Enabled? get_LabelColor() : clGrayText;

		Mode0Label->Top = Mode0Btn->Top + (Mode0Btn->Height - Mode0Label->Height)/2;
		Mode1Label->Top = Mode1Btn->Top + (Mode1Btn->Height - Mode1Label->Height)/2;
		Mode2Label->Top = Mode2Btn->Top + (Mode2Btn->Height - Mode2Label->Height)/2;
		Mode3Label->Top = Mode3Btn->Top + (Mode3Btn->Height - Mode3Label->Height)/2;
		Mode4Label->Top = Mode4Btn->Top + (Mode4Btn->Height - Mode4Label->Height)/2;
	}
	else {
		Label2RadioBtn(Mode0Label, Mode0Btn);
		Label2RadioBtn(Mode1Label, Mode1Btn);
		Label2RadioBtn(Mode2Label, Mode2Btn);
		Label2RadioBtn(Mode3Label, Mode3Btn);
		Label2RadioBtn(Mode4Label, Mode4Btn);
	}
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (CopyMode!=CPYMD_MAN_REN) {
		UnicodeString key;
		key.sprintf(_T("SameDlgCopyAll%u"), TaskNo + 1);
		IniFile->WriteBoolGen(key.c_str(), CopyAll);
	}
}

//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+O") && Mode0Btn->Enabled) Mode0Btn->Checked = true;
	else if (USAME_TI(KeyStr, "Alt+N") && Mode1Btn->Enabled) Mode1Btn->Checked = true;
	else if (USAME_TI(KeyStr, "Alt+S") && Mode2Btn->Enabled) Mode2Btn->Checked = true;
	else if (USAME_TI(KeyStr, "Alt+U") && Mode3Btn->Enabled) Mode3Btn->Checked = true;
	else if (USAME_TI(KeyStr, "Alt+R") && Mode4Btn->Enabled) Mode4Btn->Checked = true;
	else if (USAME_TI(KeyStr, "Alt+A")) invert_CheckBox(AllCheckBox);
	else if (!RenameEdit->Focused()) {
		if		(USAME_TI(KeyStr, "O") && Mode0Btn->Enabled) Mode0Btn->Checked = true;
		else if (USAME_TI(KeyStr, "N") && Mode1Btn->Enabled) Mode1Btn->Checked = true;
		else if (USAME_TI(KeyStr, "S") && Mode2Btn->Enabled) Mode2Btn->Checked = true;
		else if (USAME_TI(KeyStr, "U") && Mode3Btn->Enabled) Mode3Btn->Checked = true;
		else if (USAME_TI(KeyStr, "R") && Mode4Btn->Enabled) Mode4Btn->Checked = true;
		else if (USAME_TI(KeyStr, "A")) invert_CheckBox(AllCheckBox);
	}
	else {
		SpecialKeyProc(this, Key, Shift);
	}
}

//---------------------------------------------------------------------------
//モード変更
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::ModeBtnClick(TObject *Sender)
{
	RenameEdit->Enabled = false;

	if (Mode0Btn->Checked) {
		CopyMode = CPYMD_OW;
		Mode0Btn->SetFocus();
	}
	else if (Mode1Btn->Checked) {
		CopyMode = CPYMD_NEW;
		Mode1Btn->SetFocus();
	}
	else if (Mode2Btn->Checked) {
		CopyMode = CPYMD_SKIP;
		Mode2Btn->SetFocus();
	}
	else if (Mode3Btn->Checked) {
		CopyMode = CPYMD_AUT_REN;
		Mode3Btn->SetFocus();
	}
	else if (Mode4Btn->Checked) {
		CopyMode = CPYMD_MAN_REN;
		SameNameDlg->AllCheckBox->Checked = CopyAll = false;
		RenameEdit->Enabled = true;
		RenameEdit->SetFocus();
		int p = pos_r(_T("."), RenameEdit->Text) - 1;
		if (p<0) p = RenameEdit->Text.Length();
		RenameEdit->SelStart = p;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::AllCheckBoxClick(TObject *Sender)
{
	if (InhAllChk) return;

	CopyAll = SameNameDlg->AllCheckBox->Checked;
	Mode4Btn->Enabled = !CopyAll;
	Mode4Label->Font->Color = Mode4Btn->Enabled? get_LabelColor() : clGrayText;
	Mode4Label->Repaint();

	if (CopyAll && Mode4Btn->Checked) Mode3Btn->Checked = true;
	RenameEdit->Enabled = Mode4Btn->Checked;
}

//---------------------------------------------------------------------------
//情報欄の描画
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::InfoListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);
	int xp = Rect.Left + ScaledInt(4, this);
	int yp = Rect.Top  + get_TopMargin(cv);

	if (Index==2 || Index==4) {
		//サイズ
		UnicodeString lbuf = "    " + get_FileSizeStr((Index==2)? CurSrcSize : CurDstSize) + " ";
		cv->Font->Color = col_fgList;
		cv->TextOut(xp, yp, lbuf);
		xp += cv->TextWidth(lbuf);
		//タイム
		TDateTime dt = (Index==2)? CurSrcTime : CurDstTime;
		cv->Font->Color = get_TimeColor(dt, col_fgList);
		cv->TextOut(xp, yp, format_DateTime(dt));
	}
	else {
		UnicodeString lbuf = lp->Items->Strings[Index];

		cv->Font->Color = (Index==0)? get_ExtColor(ExtractFileExt(lbuf)) :
			  (Index==1 || Index==3)? col_Folder : col_fgList;

		if (Index==0 || Index==1 || Index==3)
			lbuf = minimize_str(lbuf, cv, Rect.Width() - 8, OmitEndOfName);

		if (Index==1 || Index==3)
			PathNameOut(lbuf, cv, xp, yp);
		else
			cv->TextOut(xp, yp, lbuf);
	}
}

//---------------------------------------------------------------------------
//名前欄でのキー操作
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::RenameEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (equal_UP(KeyStr))	 {
		Mode3Btn->SetFocus();
		Key = 0;
	}
	else if (equal_DOWN(KeyStr)) {
		AllCheckBox->SetFocus();
		Key = 0;
	}
	else {
		KeyHandled = SpecialEditProc(Sender, Key, Shift);
	}
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::RenameEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (KeyHandled) {
		KeyHandled = false;
		Key = 0;
	}
}
//---------------------------------------------------------------------------

