//----------------------------------------------------------------------//
// NyanFi																//
//  新規ファイルの作成ダイアログ										//
//----------------------------------------------------------------------//
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "NewDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewFileDlg *NewFileDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TNewFileDlg::TNewFileDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::FormCreate(TObject *Sender)
{
	NewNameEdit->Tag = EDTAG_REF_CDIR;
	UserModule->SetUsrPopupMenu(this);

	TplComboBox->Tag = CBTAG_HISTORY;
}
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	IniFile->LoadPosInfo(this, DialogCenter);

	//ファイルリストの中央に表示
	TControl *cp = (TControl*)FileListBox[CurListTag];
	TPoint p = cp->ClientToScreen(cp->ClientRect.CenteredRect(BoundsRect).Location);
	TRect main_rc = Application->MainForm->BoundsRect;
	if (p.x<main_rc.Left)			 p.x = main_rc.Left + 8;
	if ((p.x + Width)>main_rc.Right) p.x = main_rc.Right - Width - 8;
	Left = p.x;
	Top  = p.y;

	IniFile->LoadComboBoxItems(TplComboBox,	_T("NewTplHistory"));

	TplRefDir = IniFile->ReadStrGen(_T("NewFileTplDir"), ExePath);
	NewExeCmdEdit->Text = IniFile->ReadStrGen(_T("NewFileExeCmd"),
							!IniFile->KeyExists("General", "NewFileExeCmd")? "OpenByWin" : "");

	SetDarkWinTheme(this);
	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::WmFormShowed(TMessage &msg)
{
	if (TplComboBox->Items->Count>0) {
		TplComboBox->ItemIndex = 0;
		TplComboBoxClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SaveComboBoxItems(TplComboBox,	_T("NewTplHistory"));
	IniFile->WriteStrGen(_T("NewFileTplDir"), TplRefDir);
	IniFile->WriteStrGen(_T("NewFileExeCmd"), NewExeCmdEdit);
}

//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::TplComboBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TCanvas *cv = TplComboBox->Canvas;
	cv->FillRect(Rect);

	int xp = Rect.Left + ScaledInt(2, this);
	int yp = Rect.Top  + get_TopMargin(cv);
	UnicodeString tnam = TplComboBox->Items->Strings[Index];

	//アイコン
	draw_SmallIcon2(tnam, cv, xp, std::max(yp + (cv->TextHeight("Q") - ScaledInt(16, this))/2, 0), this);
	xp += ScaledInt(20, this);
	//ファイル名
	cv->TextOut(xp, yp, ExtractFileName(tnam));
}

//---------------------------------------------------------------------------
//テンプレートの選択
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::RefTplBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("テンプレートの選択"), F_FILTER_ALL, NULL, TplRefDir);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam, true)) {
		add_ComboBox_history(TplComboBox, fnam);
		TplRefDir = ExtractFilePath(to_absolute_name(fnam));
		TplComboBox->ItemIndex = 0;
		TplComboBoxClick(NULL);
	}
}
//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::TplComboBoxClick(TObject *Sender)
{
	UnicodeString fnam = TplComboBox->Text;
	NewNameEdit->Text  = ExtractFileName(fnam);
	NewNameEdit->SetFocus();
	int p = pos_r(_T("."), NewNameEdit->Text) - 1;
	if (p<0) p = NewNameEdit->Text.Length();
	NewNameEdit->SelStart  = 0;
	NewNameEdit->SelLength = p;
}

//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::OkActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Enabled = !TplComboBox->Text.IsEmpty();
	NewNameEdit->Enabled = ap->Enabled;
}

//---------------------------------------------------------------------------
void __fastcall TNewFileDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#NewFile"));
}
//---------------------------------------------------------------------------

