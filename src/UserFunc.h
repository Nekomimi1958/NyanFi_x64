//----------------------------------------------------------------------//
// 汎用共通関数															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFuncH
#define UsrFuncH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <shlobj.h>
#include <System.StrUtils.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.CheckLst.hpp>
#include "usr_str.h"
#include "usr_key.h"
#include "usr_msg.h"
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "UserMdl.h"

//---------------------------------------------------------------------------
#define URL_MATCH_PTN	_T("h?ttps?://[\\w/:%#$&?()~.=+-]+[\\w/]")		//URL用正規表現
#define MAIL_MATCH_PTN	_T("mailto:[a-zA-Z0-9]+[\\w.-]*@[\\w.-]+")		//メール用正規表現
#define LOCAL_FILE_PTN	_T("file:///[^/*?\"<>|）]+\\.[a-zA-Z0-9]+")		//ローカルファイルの正規表現

#define LISTBOX_SCRMGN	2

//---------------------------------------------------------------------------
HWND get_window_from_pos();
TRect get_window_rect(HWND hWnd);
void set_window_pos_ex(HWND hWnd, TRect rc);

void show_PopupMenu(TPopupMenu *mp, TControl *cp);
void show_PopupMenu(TListBox *cp);
void show_PopupMenu(TCheckListBox *cp);
void show_PopupMenu(TStringGrid *gp);

void set_ListBoxText(TListBox *lp, const _TCHAR *s);
void set_ComboBoxText(TComboBox *cp, const _TCHAR *s);

void add_ComboBox_history(TComboBox *cp, UnicodeString kwd = EmptyStr);
int  del_ComboBox_history(TComboBox *cp, UnicodeString kwd = EmptyStr);

void CloseIME(HWND hWnd);

TPoint get_ListItemPos(TListBox *lp, int xp = 0);
void pos_ListBoxItem(TListBox *lp, int xp = 0);

//---------------------------------------------------------------------------
bool has_Leading(TCanvas *cv);
int  get_FontHeight(TFont *font, int mgn = 0, int min_mgn = 2);
int  get_CharWidth_Font(TFont *font, int n);

int  EditToInt(TLabeledEdit *ep, int def = 0);
int  EditToInt(TEdit *ep, int def = 0);

TDate set_NormDay(unsigned short y, unsigned short m, unsigned short d);
bool str_to_DateTime(UnicodeString s, TDateTime *dt);
UnicodeString format_DateTime(TDateTime dt, bool omit = false);
UnicodeString format_Date(TDateTime dt);
UnicodeString format_DateTimeEx(UnicodeString fmt, TDateTime dt);

int get_DateCond(UnicodeString prm, TDateTime &dt);

UnicodeString get_res_cnt_str(int ok_cnt = 0, int er_cnt = 0, int sk_cnt = 0, int ng_cnt = 0);

int  format_res_list(TStringList *lst, int mgn = 2, int min_wd = 16);

void ShowPropertyDialog(UnicodeString fnam);

void delete_ListItem(TCustomListBox *lp);
void draw_ListItemLine(TCustomListBox *lp, int idx);

void draw_Line(TCanvas *cv, int x0, int y0, int x1, int y1, int w, TColor col, TPenStyle = psSolid);
void draw_Separator(TCanvas *cv, TRect rc, TColor bg = Graphics::clNone);
void draw_separateLine(TCanvas *cv, TRect rc, int pos = 0);

double get_ZoomRatio(int vw, int vh, int iw, int ih, int mgn = 0, double limit = 1.0);

double GetLuminance(TColor col);
TColor RatioCol(TColor col, float rt);
TColor GrayCol(TColor col);
void RgbToHsl(TColor col, int *h, int *s, int *l);
void RgbToHsv(TColor col, int *h, int *s, int *v);
TColor SelectWorB(TColor col, float rt = 1.0);

#define ADJCOL_LIGHT	72
#define ADJCOL_FGLIST	96
TColor AdjustColor(TColor col, int adj);

TColor Mix2Colors(TColor col1, TColor col2);
void str_to_Color(TColor &col, UnicodeString s);

void alpha_blend_Rect(TCanvas *cv, int x, int y, int w, int h, TColor col, int alpha);
void alpha_blend_Rect(TCanvas *cv, TRect rc, TColor col, int alpha);

void init_UpDown(TUpDown *udp, int n);

void set_GridFromHeader(THeaderControl *hp, TStringGrid *gp);
void set_HeaderFromGrid(TStringGrid *gp, THeaderControl *hp);
void set_HeaderSecWidth(THeaderControl *hp, int cnt, ...);
void adjust_HeaderSecWidth(THeaderControl *hp, int idx);

int  set_SttBarPanelWidth(TStatusBar *sp, int idx, int n);
int  set_SttBarPanelWidth(TStatusBar *sp, int idx, UnicodeString s);

void clear_GridRow(TStringGrid *gp, int row, int col = 0);
int  get_GridIndex(TStringGrid *gp, int max_count);
void set_GridIndex(TStringGrid *gp, int idx, int max_count);

void ListBoxCursorDown(TListBox *lp);
void ListBoxCursorDown(TListBox *lp, int n);
void ListBoxCursorUp(TListBox *lp);
void ListBoxCursorUp(TListBox *lp, int n);
void ListBoxPageDown(TListBox *lp);
void ListBoxPageUp(TListBox *lp);
void ListBoxTop(TListBox *lp);
void ListBoxEnd(TListBox *lp);
void ListBoxSetIndex(TListBox *lp, int idx, bool top_sw = false);
bool ListBoxCsrToVisible(TListBox *lp);
void ListBoxScrollDown(TListBox *lp, int n, bool move_csr = false);
void ListBoxScrollDown(TListBox *lp, UnicodeString prm);
void ListBoxScrollUp(TListBox *lp, int n, bool move_csr = false);
void ListBoxScrollUp(TListBox *lp, UnicodeString prm);
void ListBoxSelectAll(TListBox *lp, bool sw = true);
int  ListBoxInitialSearch(TListBox *lp, UnicodeString kptn, bool fnam_sw = false);
UnicodeString ListBoxGetURL(TListBox *lp);
void set_ListBox_ItemNo(TListBox *lp);
int get_CheckListCount(TCheckListBox *lp);

void GridCursorLeft(TStringGrid *gp);
void GridCursorRight(TStringGrid *gp);
void GridCursorDown(TStringGrid *gp);
void GridCursorUp(TStringGrid *gp);
void GridPageDown(TStringGrid *gp);
void GridPageUp(TStringGrid *gp);
bool GridCursorMove(UnicodeString cmd , TStringGrid *gp);

int up_StrListItem(TStringList *lst, int idx);
int down_StrListItem(TStringList *lst, int idx);

int get_IndexFromAccKey(TStringList *lst, UnicodeString key);

UnicodeString extract_URL(UnicodeString s);
UnicodeString get_EncodedUrl(UnicodeString url);
bool InternetConnected();
int  get_OnlineFile(UnicodeString url, UnicodeString fnam, bool *cancel = NULL, TProgressBar *prg_bar = NULL);

void set_SplitterWidht(TSplitter *sp, int sp_wd);
void set_PanelAlign(TPanel *pp, TSplitter *sp, int mode,  int sp_wd);
void set_PanelAlign(TPanel *pp, TSplitter *sp, TAlign al, int sp_wd);

void set_ControlRBCorner(TControl *cp0, TControl *cp1);

UnicodeString get_BatteryPercentStr();
UnicodeString get_BatteryTimeStr();

bool make_url_file(UnicodeString fnam, UnicodeString url);

bool EjectDrive(UnicodeString drvnam, bool eject);
bool EjectDrive2(UnicodeString drvnam, bool eject);

bool class_is_LabeledEdit(TObject *op);
bool class_is_MaskEdit(TObject *op);
bool class_is_CustomEdit(TObject *op);
bool class_is_Edit(TObject *op);
bool class_is_CheckBox(TObject *op);
bool class_is_ComboBox(TObject *op);
bool class_is_ListBox(TObject *op);
bool class_is_CheckListBox(TObject *op);
bool class_is_RadioGroup(TObject *op);

void cursor_HourGlass();
void cursor_Default();

void reduction_MenuLine(TMenuItem *mp);

//---------------------------------------------------------------------------
#endif
