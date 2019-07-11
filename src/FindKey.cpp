//----------------------------------------------------------------------//
// NyanFi																//
//  キー検索															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "usr_key.h"
#include "Global.h"
#include "FindKey.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindKeyDlg *FindKeyDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TFindKeyDlg::TFindKeyDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::FormCreate(TObject *Sender)
{
	KeyList = new TStringList();
}
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::FormShow(TObject *Sender)
{
	KeyStr = EmptyStr;
	KeyLabel->Caption = EmptyStr;
	DummyBtn->SetFocus();

	SetDarkWinTheme(this);
	//TSpeedButton のダークモード対応
	if (IsDarkMode) {
		CancelBtn->Flat    = true;
		CanOutPanel->Color = dcl_BtnHighlight;
		CanInPanel->Color  = dcl_BtnFace;
	}
	else {
		CancelBtn->Flat    = false;
		CanOutPanel->Color = Color;
		CanInPanel->Color  = Color;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (ModalResult!=mrOk) KeyStr = EmptyStr;
	KeyList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::FormDestroy(TObject *Sender)
{
	delete KeyList;
}

//---------------------------------------------------------------------------
//2ストローク開始キーか?
//---------------------------------------------------------------------------
bool __fastcall TFindKeyDlg::IsFirstCmdKey(UnicodeString keystr)
{
	bool res = false;
	for (int i=0; i<KeyList->Count; i++) {
		UnicodeString fkey = KeyList->Names[i];
		if (ContainsStr(fkey, "~")) {
			fkey = get_tkn_m(fkey, ':', '~');
			if (SameText(keystr, fkey)) { res = true; break; }
		}
	}

	if (res) KeyLabel->Caption = "2ストロークキー: " + KeyStr + "~";
	return res;
}

//---------------------------------------------------------------------------
//ENTER以外のキー処理
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString key_str = get_KeyStr(Key);
	if (!key_str.IsEmpty()) {
		if (KeyStr.IsEmpty()) {
			KeyStr = get_ShiftStr(Shift) + key_str;
			if (!IsFirstCmdKey(KeyStr)) ModalResult = mrOk;
		}
		else {
			KeyStr.cat_sprintf(_T("~%s"), key_str.c_str());
			ModalResult = mrOk;
		}
	}

	Key = 0;
}
//---------------------------------------------------------------------------
//ENTERキーの処理 (DummyBtn で受け取る)
//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::DummyBtnClick(TObject *Sender)
{
	UnicodeString key_str = "ENTER";
	UnicodeString sft_str;
	if (is_KeyDown(VK_SHIFT))	sft_str += KeyStr_Shift;
	if (is_KeyDown(VK_CONTROL))	sft_str += KeyStr_Ctrl;
	if (is_KeyDown(VK_MENU))	sft_str += KeyStr_Alt;

	if (KeyStr.IsEmpty()) {
		KeyStr = sft_str + key_str;
		if (!IsFirstCmdKey(KeyStr)) ModalResult = mrOk;
	}
	else {
		KeyStr.cat_sprintf(_T("~%s"), key_str.c_str());
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFindKeyDlg::CancelBtnClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TFindKeyDlg::Shape1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
//
}
//---------------------------------------------------------------------------

