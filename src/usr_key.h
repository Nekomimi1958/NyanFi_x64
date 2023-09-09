//----------------------------------------------------------------------//
//キーボード															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrKeyH
#define UsrKeyH

//---------------------------------------------------------------------------
#define ESC_INTERVAL	256

#define REPEAT_WAIT		50

extern UnicodeString KeyStr_Shift;
extern UnicodeString KeyStr_Ctrl;
extern UnicodeString KeyStr_Alt;
extern int KeyboardMode;

void make_KeyList(TStringList *lst);
void assign_KeyList(TComboBox *cp, bool ins_empty = false, bool keep_item = false);

bool is_KeyDown(WORD key);
bool is_DialogKey(WORD Key);
bool is_Num0to9(UnicodeString keystr);
int  get_shift_from_wparam(WPARAM prm);

bool is_JpKeybd();

void ClearKeyBuff(bool key_only = false);

void perform_Key(TControl *cp, int key);
void perform_Key_RETURN(TControl *cp);

TShiftState   get_Shift();
UnicodeString get_ShiftStr(TShiftState Shift);
UnicodeString get_KeyStr(WORD Key);
UnicodeString get_KeyStr(WORD Key, TShiftState Shift);
UnicodeString get_AlNumChar(WORD Key);

UnicodeString WaitForKey();

//---------------------------------------------------------------------------
inline bool has_KeyDownMsg()
{
	MSG msg;
	return ::PeekMessage(&msg, NULL, WM_KEYDOWN, WM_KEYDOWN, PM_NOREMOVE);
}
//---------------------------------------------------------------------------
inline bool is_KeyPress_ESC()
{
	if (!Application->Active) return false;
	Application->ProcessMessages();		//！要注意
	return (HIBYTE(::GetAsyncKeyState(VK_ESCAPE)) != 0);
}
//---------------------------------------------------------------------------
#endif
