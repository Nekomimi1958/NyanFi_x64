//----------------------------------------------------------------------//
//キーボード															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <System.StrUtils.hpp>
#include "usr_key.h"

//---------------------------------------------------------------------------
UnicodeString	KeyStr_Shift = "Shift+";
UnicodeString	KeyStr_Ctrl  = "Ctrl+";
UnicodeString	KeyStr_Alt	 = "Alt+";

int KeyboardMode = 0;	//0=自動認識/ 1=JP/ 2=US

//---------------------------------------------------------------------------
//キーリストの作成
//---------------------------------------------------------------------------
void make_KeyList(TStrings *lst)
{
	std::unique_ptr<TStringList> klst(new TStringList());

	UnicodeString itmbuf;
	for (int i=0; i<26; i++) klst->Add(itmbuf.sprintf(_T("%c"), 'A' + i));	//英字
	for (int i=0; i<10; i++) klst->Add(itmbuf.sprintf(_T("%c"), '0' + i));	//数字

	for (int i=0; i<12; i++) klst->Add(itmbuf.sprintf(_T("F%u"), i + 1));	//Fキー
	klst->AddStrings(
		SplitString("Del|Ins|BkSp|Left|Right|UP|Down|PgUp|PgDn|Home|End|Pause|Tab|Esc|Enter|Space|App", "|"));

	if (is_JpKeybd())
		klst->AddStrings(SplitString("-|^|\\|@|[|;|:|]|,|.|/|＼", "|"));		//JP
	else
		klst->AddStrings(SplitString("`|-|＝|[|]|\\|;|'|,|.|/", "|"));		//US

	for (int i=0; i<10; i++) klst->Add(itmbuf.sprintf(_T("10Key_%u"), i));	//10キー
	klst->AddStrings(SplitString("10Key_*|10Key_+|10Key_-|10Key_/|10Key_.", "|"));

	lst->Assign(klst.get());
}

//---------------------------------------------------------------------------
//キーが押されているか?
//---------------------------------------------------------------------------
bool is_KeyDown(WORD key)
{
	if (!Application->Active) return false;
	return (HIBYTE(::GetAsyncKeyState(key)) != 0);
}
//---------------------------------------------------------------------------
//Ctrl+C,V 以外の Ctrl+～ が押された？
//---------------------------------------------------------------------------
bool is_KeyPress_CtrlNotCV(WORD key)
{
	return (is_KeyDown(VK_CONTROL) && key!=0x03 && key!=0x16);
}
//---------------------------------------------------------------------------
//Ctrl+C,V,X 以外の Ctrl+～ が押された？
//---------------------------------------------------------------------------
bool is_KeyPress_CtrlNotCVX(WORD key)
{
	return (is_KeyDown(VK_CONTROL) && key!=0x03 && key!=0x16 && key!=0x18);
}

//---------------------------------------------------------------------------
//ダイアログキーか？
//---------------------------------------------------------------------------
bool is_DialogKey(WORD Key)
{
	return (Key==VK_LEFT || Key==VK_RIGHT || Key==VK_UP || Key==VK_DOWN ||
			Key==VK_PRIOR || Key==VK_NEXT || Key==VK_HOME || Key==VK_END || Key==VK_TAB);
}

//---------------------------------------------------------------------------
//0～9キーか?
//---------------------------------------------------------------------------
bool is_Num0to9(UnicodeString keystr)
{
	return (keystr.Length()==1 && CompareStr(keystr, "0")>=0 && CompareStr(keystr, "9")<=0);
}

//---------------------------------------------------------------------------
//WPARAM からシフト状態を取得
//---------------------------------------------------------------------------
int get_shift_from_wparam(WPARAM prm)
{
	int kmd = 0;
	int key = GET_KEYSTATE_WPARAM(prm);
	if (key & MK_CONTROL) kmd |= 1;
	if (key & MK_SHIFT)   kmd |= 2;
	return kmd;
}

//---------------------------------------------------------------------------
//日本語キーボードか?
//---------------------------------------------------------------------------
bool is_JpKeybd()
{
	return (KeyboardMode==0)? (::GetKeyboardType(0)==7) : (KeyboardMode==1);
}

//---------------------------------------------------------------------------
//キー・マウスバッファをクリア
//---------------------------------------------------------------------------
void ClearKeyBuff(
	bool key_only)	//true = キーバッファのみクリア (default = false)
{
	MSG msg;
	if (key_only) {
		do {
			Sleep(0);
		} while (::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE));
	}
	else {
		do {
			Sleep(50);
		} while (
			::PeekMessage(&msg, NULL, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE) ||
			::PeekMessage(&msg, NULL, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)
		);
	}
}

//---------------------------------------------------------------------------
//コントロールでキーを押す
//---------------------------------------------------------------------------
void perform_Key(TControl *cp, int key)
{
	cp->Perform(WM_KEYDOWN,	key, (NativeInt)0);
	cp->Perform(WM_KEYUP,	key, (NativeInt)0);
}
//---------------------------------------------------------------------------
void perform_Key_RETURN(TControl *cp)
{
	perform_Key(cp, VK_RETURN);
}

//---------------------------------------------------------------------------
//シフト状態を取得
//---------------------------------------------------------------------------
TShiftState get_Shift()
{
	TShiftState shift;
	if (Application->Active) {
		if (HIBYTE(::GetAsyncKeyState(VK_SHIFT)))	shift << ssShift;
		if (HIBYTE(::GetAsyncKeyState(VK_CONTROL)))	shift << ssCtrl;
		if (HIBYTE(::GetAsyncKeyState(VK_MENU)))	shift << ssAlt;
	}
	return shift;
}

//---------------------------------------------------------------------------
//シフト状態を文字列に
//---------------------------------------------------------------------------
UnicodeString get_ShiftStr(TShiftState Shift)
{
	UnicodeString sftstr;
	if (Shift.Contains(ssShift)) sftstr += KeyStr_Shift;
	if (Shift.Contains(ssCtrl))  sftstr += KeyStr_Ctrl;
	if (Shift.Contains(ssAlt))   sftstr += KeyStr_Alt;
	return  sftstr;
}

//---------------------------------------------------------------------------
//キーを文字列に変換
//  JP/USキーボード対応
//---------------------------------------------------------------------------
UnicodeString get_KeyStr(WORD Key)
{
	UnicodeString keystr;

	switch (Key) {
	case VK_RETURN: 	keystr = "ENTER";	break;
	case VK_ESCAPE: 	keystr = "ESC";		break;
	case VK_PAUSE:  	keystr = "PAUSE";	break;
	case VK_SPACE:  	keystr = "SPACE";	break;
	case VK_TAB:		keystr = "TAB";		break;
	case VK_LEFT:   	keystr = "LEFT";	break;
	case VK_RIGHT:  	keystr = "RIGHT";	break;
	case VK_DOWN:		keystr = "DOWN";	break;
	case VK_UP:			keystr = "UP";		break;
	case VK_PRIOR:		keystr = "PGUP";	break;
	case VK_NEXT:		keystr = "PGDN";	break;
	case VK_HOME:		keystr = "HOME";	break;
	case VK_END:		keystr = "END";		break;
	case VK_INSERT:		keystr = "INS";		break;
	case VK_DELETE: 	keystr = "DEL";		break;
	case VK_BACK: 		keystr = "BKSP";	break;
	case VK_F1:			keystr = "F1";		break;
	case VK_F2:			keystr = "F2";		break;
	case VK_F3:			keystr = "F3";		break;
	case VK_F4:			keystr = "F4";		break;
	case VK_F5:			keystr = "F5";		break;
	case VK_F6:			keystr = "F6";		break;
	case VK_F7:			keystr = "F7";		break;
	case VK_F8:			keystr = "F8";		break;
	case VK_F9:			keystr = "F9";		break;
	case VK_F10:		keystr = "F10";		break;
	case VK_F11:		keystr = "F11";		break;
	case VK_F12:		keystr = "F12";		break;
	case VK_APPS:		keystr = "APP";		break;
	case VK_OEM_1:		keystr = is_JpKeybd()? ":" : ";"; break;
	case VK_OEM_2:		keystr = "/";		break;
	case VK_OEM_3:		keystr = is_JpKeybd()? "@" : "`"; break;
	case VK_OEM_4:		keystr = "[";		break;
	case VK_OEM_5:		keystr = "\\";		break;
	case VK_OEM_6:		keystr = "]";		break;
	case VK_OEM_MINUS:  keystr = "-";		break;
	case VK_OEM_PLUS:   keystr = is_JpKeybd()? ";" : "＝"; break;
	case VK_OEM_COMMA:  keystr = ",";		break;
	case VK_OEM_PERIOD: keystr = ".";		break;
	case VK_OEM_102:    keystr = "＼";		break;

	case VK_NUMPAD0:	keystr = "10Key_0";	break;
	case VK_NUMPAD1:	keystr = "10Key_1";	break;
	case VK_NUMPAD2:	keystr = "10Key_2";	break;
	case VK_NUMPAD3:	keystr = "10Key_3";	break;
	case VK_NUMPAD4:	keystr = "10Key_4";	break;
	case VK_NUMPAD5:	keystr = "10Key_5";	break;
	case VK_NUMPAD6:	keystr = "10Key_6";	break;
	case VK_NUMPAD7:	keystr = "10Key_7";	break;
	case VK_NUMPAD8:	keystr = "10Key_8";	break;
	case VK_NUMPAD9:	keystr = "10Key_9";	break;
	case VK_MULTIPLY:	keystr = "10Key_*";	break;
	case VK_ADD:		keystr = "10Key_+";	break;
	case VK_SUBTRACT:	keystr = "10Key_-";	break;
	case VK_DIVIDE:		keystr = "10Key_/";	break;
	case VK_DECIMAL:	keystr = "10Key_.";	break;
	case VK_OEM_7:	  	keystr = is_JpKeybd()? "^" : "'"; break;

	default:
		if (_istalnum(Key)) keystr = (char)Key; else keystr = EmptyStr;
	}

	return  keystr;
}
//---------------------------------------------------------------------------
//キーとシフト状態を文字列に
//---------------------------------------------------------------------------
UnicodeString get_KeyStr(WORD Key, TShiftState Shift)
{
	UnicodeString keystr = get_KeyStr(Key);
	return !keystr.IsEmpty()? get_ShiftStr(Shift) + keystr : EmptyStr;
}
//---------------------------------------------------------------------------
