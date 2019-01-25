//----------------------------------------------------------------------//
// NyanFi																//
//  Edit コマンド用メモボックス											//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <System.NetEncoding.hpp>
#include <Vcl.Clipbrd.hpp>
#include "UserMdl.h"
#include "Global.h"
#include "MemoFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMemoForm *MemoForm = NULL;

//---------------------------------------------------------------------------
__fastcall TMemoForm::TMemoForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormCreate(TObject *Sender)
{
	LinesBuff = new TStringList();

	OptVisible = true;
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormShow(TObject *Sender)
{
	set_ButtonMark(HideOptBtn,  UBMK_BDOWN);
	set_ButtonMark(ShowOptBtn,  UBMK_BUP);
	HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
	ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);

	IniFile->LoadPosInfo(this, DialogCenter);

	color_MemoBg = IniFile->ReadColGen(_T("MemoFormBgCol"),	clNone);
	color_MemoFg = IniFile->ReadColGen(_T("MemoFormfgCol"),	clNone);

	MemoBox->Font->Assign(ViewerFont);
	MemoBox->Font->Color = (color_MemoFg!=clNone)? color_MemoFg : col_fgView;
	MemoBox->Color		 = (color_MemoBg!=clNone)? color_MemoBg : col_bgView;

	//※入力の際にフォントが変わってしまうのを抑制
	DWORD lParam = ::SendMessage(MemoBox->Handle, EM_GETLANGOPTIONS, 0, 0);
	lParam &= ~IMF_DUALFONT;
	::SendMessage(MemoBox->Handle, EM_SETLANGOPTIONS, 0, lParam);

	//タブストップ幅をリセット(8)
	int tab_size = 8 * 4;
	MemoBox->Perform(EM_SETTABSTOPS, 1, (LPARAM)&tab_size);
	//左余白設定
	MemoBox->Perform(EM_SETMARGINS, EC_LEFTMARGIN, (LPARAM)8);

	//テキスト内容を設定
	MemoBox->Lines->Assign(LinesBuff);

	//行間設定
	MemoBox->SelectAll();
	int f_hi = abs(MemoBox->Font->Height);
	TParaFormat2 pf2;
	::ZeroMemory(&pf2, sizeof(pf2));
	pf2.cbSize = sizeof(pf2);
	pf2.dwMask = PFM_LINESPACING;
	pf2.bLineSpacingRule = 5;	//dyLineSpacing/20 行単位
	pf2.dyLineSpacing	 = 20 * ViewTxtInterLn / f_hi;
	MemoBox->Perform(EM_SETPARAFORMAT, 0, (LPARAM)&pf2);

	//下部パネル
	OptPanel->Visible = IniFile->ReadBoolGen(_T("MemoFormShowOpt"), true);
	OptVisible = OptPanel->Visible;
	BlankPanel->Visible = !OptVisible;
	set_ControlRBCorner(MemoBox, BlankPanel);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::WmFormShowed(TMessage &msg)
{
	MemoBox->SetFocus();
	MemoBox->SelStart = 0;
	MemoBox->ClearUndo();
}

//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	LinesBuff->Assign(MemoBox->Lines);
	MemoBox->Lines->Clear();

	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("MemoFormShowOpt"),		OptPanel->Visible);
	IniFile->WriteIntGen(_T("MemoFormBgCol"),		(int)color_MemoBg);
	IniFile->WriteIntGen(_T("MemoFormfgCol"),		(int)color_MemoFg);
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormDestroy(TObject *Sender)
{
	delete LinesBuff;
}

//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormResize(TObject *Sender)
{
	set_ControlRBCorner(MemoBox, BlankPanel);
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::MemoBoxChange(TObject *Sender)
{
	set_ControlRBCorner(MemoBox, BlankPanel);
}

//---------------------------------------------------------------------------
//オプションの開閉
//---------------------------------------------------------------------------
void __fastcall TMemoForm::ChgOptBtnClick(TObject *Sender)
{
	BlankPanel->Visible = !BlankPanel->Visible;
	OptPanel->Visible	= !OptPanel->Visible;
	OptVisible = OptPanel->Visible;
	set_ControlRBCorner(MemoBox, BlankPanel);
}

//---------------------------------------------------------------------------
//元に戻す
//---------------------------------------------------------------------------
void __fastcall TMemoForm::UndoActionExecute(TObject *Sender)
{
	MemoBox->Undo();
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::UndoActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = MemoBox->CanUndo;
}

//---------------------------------------------------------------------------
//貼り付け
//  UserModule.EditPaste1 が機能しないため
//---------------------------------------------------------------------------
void __fastcall TMemoForm::PasteActionExecute(TObject *Sender)
{
	MemoBox->PasteFromClipboard();
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::PasteActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !Clipboard()->AsText.IsEmpty();
}

//---------------------------------------------------------------------------
//カーソル行を削除
//---------------------------------------------------------------------------
void __fastcall TMemoForm::DelLineActionExecute(TObject *Sender)
{
	int lno = MemoBox->Perform(EM_LINEFROMCHAR, -1, (LPARAM)0);
	if (lno<MemoBox->Lines->Count) MemoBox->Lines->Delete(lno);
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::DelLineActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = MemoBox->Focused();
}

//---------------------------------------------------------------------------
//Web で検索
//---------------------------------------------------------------------------
void __fastcall TMemoForm::WebSearchActionExecute(TObject *Sender)
{
	UnicodeString kwd = MemoBox->SelText;
	kwd = System::Netencoding::TURLEncoding::URL->Encode(kwd);
	UnicodeString url = ReplaceStr(WebSeaUrl, "\\S", kwd);
	if (!url.IsEmpty()) Execute_ex(url);
}
//---------------------------------------------------------------------------
void __fastcall TMemoForm::WebSearchActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !WebSeaUrl.IsEmpty() && !MemoBox->SelText.IsEmpty();
	ap->Caption = get_WebSeaCaption(MemoBox->SelText);
}

//---------------------------------------------------------------------------
//背景色を変更
//---------------------------------------------------------------------------
void __fastcall TMemoForm::ColBgItemClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = color_MemoBg;
	if (UserModule->ColorDlg->Execute()) {
		MemoBox->Color = color_MemoBg = UserModule->ColorDlg->Color;
		IniFile->WriteIntGen(_T("MemoFormBgCol"),	(int)color_MemoBg);
	}
}
//---------------------------------------------------------------------------
//文字色を変更
//---------------------------------------------------------------------------
void __fastcall TMemoForm::ColFgItemClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = color_MemoFg;
	if (UserModule->ColorDlg->Execute()) {
		MemoBox->Font->Color = color_MemoFg = UserModule->ColorDlg->Color;
		IniFile->WriteIntGen(_T("MemoFormfgCol"),	(int)color_MemoFg);
	}
}
//---------------------------------------------------------------------------
//デフォルトに戻す
//---------------------------------------------------------------------------
void __fastcall TMemoForm::ColDefItemClick(TObject *Sender)
{
	MemoBox->Color		 = col_bgView;
	MemoBox->Font->Color = col_fgView;
	color_MemoBg = clNone;
	color_MemoFg = clNone;
}

//---------------------------------------------------------------------------
void __fastcall TMemoForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (equal_ESC(KeyStr)) {
		Key = 0;
		ModalResult = mrCancel;
	}
	else if (USAME_TI(KeyStr, "Ctrl+ENTER")) {
		Key = 0;
		ModalResult = mrOk;
	}
	else SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

