//----------------------------------------------------------------------//
// NyanFi																//
//  グローバル(ダークモード対応)										//
//----------------------------------------------------------------------//
#ifndef GlobalDarkH
#define GlobalDarkH

//---------------------------------------------------------------------------
#include <vcl.h>

//---------------------------------------------------------------------------
typedef WINAPI BOOL (WINAPI *FUNC_ShouldAppsUseDarkMode)();
typedef WINAPI BOOL (WINAPI *FUNC_AllowDarkModeForWindow)(HWND, BOOL);
typedef WINAPI BOOL (WINAPI *FUNC_AllowDarkModeForApp)(BOOL);
typedef WINAPI VOID (WINAPI *FUNC_FlushMenuThemes)();

extern  HMODULE hUxTheme;
extern  FUNC_ShouldAppsUseDarkMode	lpfShouldAppsUseDarkMode;
extern  FUNC_AllowDarkModeForWindow	lpfAllowDarkModeForWindow;
extern  FUNC_AllowDarkModeForApp	lpfAllowDarkModeForApp;
extern  FUNC_FlushMenuThemes		lpfFlushMenuThemes ;

extern bool SupportDarkMode;
extern bool IsDarkMode;
extern bool AllowDarkMode;

//---------------------------------------------------------------------------
extern TColor scl_Window;
extern TColor scl_WindowText;
extern TColor scl_Highlight;
extern TColor scl_HighlightText;
extern TColor scl_BtnFace;
extern TColor scl_BtnText;
extern TColor scl_Menu;
extern TColor scl_MenuText;

extern TColor dcl_Window;
extern TColor dcl_WindowText;
extern TColor dcl_Highlight;
extern TColor dcl_HighlightText;
extern TColor dcl_BtnFace;
extern TColor dcl_BtnText;
extern TColor dcl_Menu;
extern TColor dcl_MenuText;

extern TColor col_Invalid;
extern TColor col_Illegal;

extern TColor col_DkPanel;
extern TColor col_DkInval;
extern TColor col_DkIlleg;

extern const TColor col_None;

//---------------------------------------------------------------------------
void InitializeSysColor();
void InitializeDarkMode();
void EndDarkMode();
bool ApplyDarkMode();

void SetDarkWinTheme(TWinControl *wp);
TLabel* AttachLabelToGroup(TWinControl *wp, UnicodeString s = EmptyStr);

TColor get_TextColor(bool hl = false);
TColor get_PanelColor(bool is_inv = false);
TColor get_LabelColor();
TColor get_WinColor(bool is_inv = false);

void SetHighlight(TCanvas *cv, bool hl, bool is_dark = false);

void msgbox_ERR(UnicodeString msg);
void msgbox_ERR(unsigned id);
void msgbox_WARN(UnicodeString msg);
void msgbox_WARN(unsigned id);
void msgbox_OK(UnicodeString msg, UnicodeString tit = EmptyStr);
bool msgbox_Y_N(UnicodeString msg, UnicodeString tit = EmptyStr);
TModalResult msgbox_Y_N_C(UnicodeString msg, UnicodeString tit = EmptyStr);
bool msgbox_Sure(UnicodeString msg, bool ask = true, bool cente = false);
bool msgbox_Sure(const _TCHAR *msg, bool ask, bool center = false);
bool msgbox_Sure(int id, bool ask = true, bool center = false);
int  msgbox_SureAll(UnicodeString msg, bool &app_chk, bool center = false);

//---------------------------------------------------------------------------
bool set_focus_RadioGroup(TRadioGroup *gp);
bool set_focus_GroupBox(TGroupBox *gp);
void invert_CheckBox(TCheckBox *cp);

//---------------------------------------------------------------------------
#define UBMK_UP		1
#define UBMK_DOWN	2
#define UBMK_BUP	3
#define UBMK_BDOWN	4
#define UBMK_VUP	5
#define UBMK_VDOWN	6
#define UBMK_VTOP	7
#define UBMK_VEND	8
#define UBMK_HTOP	9
#define UBMK_HEND	10
#define UBMK_LEFT	11
#define UBMK_RIGHT	12

void set_ButtonMark(TSpeedButton *bp, int id = UBMK_DOWN, TColor fg = scl_BtnText, TColor bg = scl_BtnFace);

//---------------------------------------------------------------------------
#endif
