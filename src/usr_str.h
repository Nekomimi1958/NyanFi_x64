//----------------------------------------------------------------------//
//	文字列操作															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrStrH
#define UsrStrH

//---------------------------------------------------------------------------
#include <System.JSON.hpp>

//---------------------------------------------------------------------------
extern const _TCHAR *null_TCHAR;

//---------------------------------------------------------------------------
#define USAME_TI(s1,s2)	same_ut_i(s1, _T(s2))
#define USAME_TS(s1,s2)	same_ut_s(s1, _T(s2))

//---------------------------------------------------------------------------
int __fastcall comp_NaturalOrder(TStringList *List, int Index1, int Index2);
int __fastcall comp_AscendOrder(TStringList *List, int Index1, int Index2);
int __fastcall comp_DescendOrder(TStringList *List, int Index1, int Index2);
int __fastcall comp_ObjectsOrder(TStringList *List, int Index1, int Index2);

//---------------------------------------------------------------------------
#define TXLIMIT_MARK "\x03\x03\x03"	//行数制限マーク

extern int  USR_CsvCol;
extern int  USR_CsvSortMode;
extern bool USR_CsvTopIsHdr;

int comp_NumStr(UnicodeString s1, UnicodeString s2);
int __fastcall comp_CsvNaturalOrder(TStringList *List, int Index1, int Index2);
int __fastcall comp_TsvNaturalOrder(TStringList *List, int Index1, int Index2);
int __fastcall comp_TsvTextOrder(TStringList *List, int Index1, int Index2);

//---------------------------------------------------------------------------
//引数を文字列リテラルで渡すことが多い場合、
//UnicodeString より _TCHAR にした方が、呼び出し側でのサイズが小さくなる

UnicodeString get_tkn(UnicodeString s, UnicodeString sp);
UnicodeString get_tkn(UnicodeString s, const _TCHAR *sp);
UnicodeString get_tkn(UnicodeString s, WideChar sp);

UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp);
UnicodeString get_tkn_r(UnicodeString s, const _TCHAR *sp);
UnicodeString get_tkn_r(UnicodeString s, WideChar sp);

UnicodeString get_tkn_m(UnicodeString s, const _TCHAR *sp1, const _TCHAR *sp2);
UnicodeString get_tkn_m(UnicodeString s, WideChar sp1, WideChar sp2);

UnicodeString get_pre_tab(UnicodeString s);
UnicodeString get_post_tab(UnicodeString s);

UnicodeString get_first_line(UnicodeString s);

UnicodeString get_in_paren(UnicodeString s);
UnicodeString split_in_paren(UnicodeString &s);
UnicodeString get_norm_str(UnicodeString s, bool trim_sw = false);

UnicodeString split_tkn(UnicodeString &s, UnicodeString sp);
UnicodeString split_tkn(UnicodeString &s, const _TCHAR *sp);
UnicodeString split_tkn(UnicodeString &s, WideChar sp);
UnicodeString split_tkn_spc(UnicodeString &s);

UnicodeString split_pre_tab(UnicodeString &s);
UnicodeString split_dsc(UnicodeString &s);
UnicodeString split_top_ch(UnicodeString &s);
WideChar split_top_wch(UnicodeString &s);

int split_cmd_line(UnicodeString s, TStringList *lst);
UnicodeString split_file_param(UnicodeString &s);

TStringDynArray split_strings_tab(UnicodeString s);
TStringDynArray split_strings_semicolon(UnicodeString s);

bool remove_text(UnicodeString &s, UnicodeString w);
bool remove_text(UnicodeString &s, const _TCHAR *w);

bool remove_top_text(UnicodeString &s, UnicodeString w);
bool remove_top_text(UnicodeString &s, const _TCHAR *w);

bool remove_top_s(UnicodeString &s, UnicodeString w);
bool remove_top_s(UnicodeString &s, const _TCHAR *w);
bool remove_top_s(UnicodeString &s, WideChar w);

bool remove_top_AT(UnicodeString &s);
bool remove_top_Dollar(UnicodeString &s);

bool remove_end_text(UnicodeString &s, UnicodeString w);

bool remove_end_s(UnicodeString &s, UnicodeString w);
bool remove_end_s(UnicodeString &s, WideChar w);

void delete_end(UnicodeString &s);

UnicodeString exclude_top(UnicodeString s);
UnicodeString exclude_top_end(UnicodeString s);

UnicodeString trim_ex(UnicodeString s);

UnicodeString replace_regex(UnicodeString s, const _TCHAR *o, const _TCHAR *r);
UnicodeString replace_regex_2(UnicodeString s, UnicodeString ptn, UnicodeString rep, TRegExOptions opt);
UnicodeString replace_str_by_list(UnicodeString s, TStringList *lst);

void cat_str_semicolon(UnicodeString &s, UnicodeString t);

UnicodeString ins_spc_length(UnicodeString s, int len);

UnicodeString def_if_empty(UnicodeString s, UnicodeString def);
UnicodeString def_if_empty(UnicodeString s, const _TCHAR *def);

NativeInt str_to_NativeInt(UnicodeString s);
int extract_int(UnicodeString s);
int extract_int_def(UnicodeString s, int def = 0);
UnicodeString extract_top_int_str(UnicodeString s);
UnicodeString extract_top_num_str(UnicodeString s);
UnicodeString ldouble_to_str(long double v, int f_w = 3);

UnicodeString get_size_str_T(__int64 sz, int dg);
UnicodeString get_size_str_G(__int64 sz, int wd = 10, int dg = 1);
UnicodeString get_size_str_G(__int64 sz, int wd, int dg, int fmt_mode);
UnicodeString get_size_str_B(__int64 sz, int wd = 14);
UnicodeString get_size_str_C(int sz);

TColor xRRGGBB_to_col(UnicodeString s);
UnicodeString col_to_xRRGGBB(TColor col);

int pos_i(UnicodeString wd, UnicodeString s);
int pos_i(const _TCHAR *wd, UnicodeString s);
int pos_r(UnicodeString wd, UnicodeString s);
int pos_r(const _TCHAR *wd, UnicodeString s);
int pos_r_i(UnicodeString wd, UnicodeString s);
int pos_r_q(UnicodeString wd, UnicodeString s);
int pos_r_q_colon(UnicodeString s);

bool contains_upper(UnicodeString s);

bool contains_word_and_or(UnicodeString s, UnicodeString kwd, bool case_sw = false);
bool contains_fuzzy_word(UnicodeString s, UnicodeString kwd, bool case_sw = false, UnicodeString sp = EmptyStr);

bool contained_wd_i(UnicodeString lst, UnicodeString wd);
bool contained_wd_i(const _TCHAR *lst, UnicodeString wd);

bool contains_wd_i(UnicodeString s, const _TCHAR *lst);

UnicodeString get_word_i_idx(const _TCHAR *lst, int idx);
int idx_of_word_i(const _TCHAR *lst, UnicodeString wd);
bool test_word_i(UnicodeString w, UnicodeString list);

bool str_match(UnicodeString ptn, UnicodeString s);

bool chk_RegExPtn(UnicodeString ptn);
bool is_match_regex(UnicodeString s, const _TCHAR *ptn);
bool is_match_regex_i(UnicodeString s, const _TCHAR *ptn);
UnicodeString get_match_regex(UnicodeString s, const _TCHAR *ptn);
UnicodeString extract_prm_RegExPtn(UnicodeString &s);
UnicodeString ptn_match_str(UnicodeString ptn, UnicodeString s);
bool starts_ptn(UnicodeString ptn, UnicodeString s);

UnicodeString get_fuzzy_ptn(UnicodeString kwd, bool sep_sw = false);

void get_find_wd_list(UnicodeString wd, TStringList *lst);
bool find_mlt(UnicodeString wd, UnicodeString s, bool and_sw = false, bool not_sw = false, bool case_sw = false);
bool find_mlt_str(UnicodeString wd, UnicodeString s, TStringList *lst, bool case_sw);

int get_line_count(UnicodeString s);

void add_dyn_array(TStringDynArray &lst, UnicodeString s, bool no_dupl = false);

TStringDynArray get_csv_array(UnicodeString src, int size, bool force_size = false);
UnicodeString get_csv_item(UnicodeString src, int idx);
UnicodeString get_tsv_item(UnicodeString src, int idx);
UnicodeString make_csv_str(UnicodeString s);
UnicodeString make_csv_str(bool sw);
UnicodeString make_csv_rec_str(TStringDynArray lst);
int indexof_csv_list(TStringList *lst, UnicodeString s, int p);
TStringDynArray record_of_csv_list(TStringList *lst, UnicodeString s, int p, int size);

UnicodeString conv_esc_char(UnicodeString s);

UnicodeString yen_to_slash(UnicodeString s);
UnicodeString slash_to_yen(UnicodeString s);
UnicodeString sha1_to_short(UnicodeString s);

bool same_ut_i(UnicodeString s, const _TCHAR *t);
bool same_ut_s(UnicodeString s, const _TCHAR *t);

bool equal_1(UnicodeString s);
bool equal_0(UnicodeString s);
bool equal_ENTER(UnicodeString s);
bool equal_ESC(UnicodeString s);
bool equal_TAB(UnicodeString s);
bool equal_DEL(UnicodeString s);
bool equal_LEFT(UnicodeString s);
bool equal_RIGHT(UnicodeString s);
bool equal_UP(UnicodeString s);
bool equal_DOWN(UnicodeString s);
bool equal_HOME(UnicodeString s);
bool equal_END(UnicodeString s);
bool equal_F1(UnicodeString s);

bool is_separator(UnicodeString s);
bool is_alnum_str(UnicodeString s);

bool starts_tchs(const _TCHAR *lst, UnicodeString s);
bool ends_tchs(const _TCHAR *lst, UnicodeString s);

bool starts_AT(UnicodeString s);
bool starts_Dollar(UnicodeString s);

bool ends_PathDlmtr(UnicodeString s);

bool contains_PathDlmtr(UnicodeString s);
bool contains_Slash(UnicodeString s);

int  count_PathDlmtr(UnicodeString s);

bool is_regex_slash(UnicodeString s);

bool is_quot(UnicodeString s);
UnicodeString add_quot_if_spc(UnicodeString s);
UnicodeString exclude_quot(UnicodeString s);

int  get_ListIntVal(TStringList *lst, const _TCHAR *name, int def = 0);
bool ListVal_is_empty(TStringList *lst, const _TCHAR *name);
bool ListVal_equal_1(TStringList *lst, const _TCHAR *name);

UnicodeString __fastcall mSecToTStr(unsigned int ms, bool cs = true);
int param_to_mSec(UnicodeString prm);

bool IsIrregularFont(TFont *fnt);
int get_TextWidth(TCanvas *cv, UnicodeString s, bool is_irreg, bool spc_sw = false);
int get_CharWidth(TCanvas *cv, int n, int mgn = 0);

int get_WidthInPanel(UnicodeString s, TPanel *pp, bool d2d_sw = false);

int str_len_half(UnicodeString s);
int str_len_unicode(UnicodeString s);
void max_len_half(int &w, UnicodeString s);

UnicodeString align_r_str(UnicodeString s, int wd, UnicodeString post_s = EmptyStr);
UnicodeString align_l_str(UnicodeString s, int wd, UnicodeString post_s = EmptyStr);
UnicodeString minimize_str(UnicodeString s, TCanvas *cv, int wd, bool omit_end = false, bool spc_sw = false);
UnicodeString fit_str(UnicodeString s, TCanvas *cv, int wd, bool omit_end = false);

UnicodeString to_Full_or_Half(UnicodeString s, bool to_w);
UnicodeString to_FullWidth(UnicodeString s);
UnicodeString to_HalfWidth(UnicodeString s);

int is_RuledLine(UnicodeString s);

UnicodeString make_RuledLine(int cnt, ...);

UnicodeString get_AddrStr(__int64 adr, int w = 0);
UnicodeString get_AspectStr(int w, int h);

//---------------------------------------------------------------------------
UnicodeString get_NameOfCharSet(int charset);
UnicodeString get_NameOfWeight(int w);

extern const UnicodeString CodePageList;
extern const UnicodeString CodePageListX;

UnicodeString get_NameOfCodePage(int code_page, bool long_sw = false, bool has_bom = false);
int get_CodePageOfName(UnicodeString code_name);
UnicodeString get_UnicodeBlockName(int code);
int extract_UnicodePoint(UnicodeString &s, UnicodeString ptn, bool dec_sw = false);
UnicodeString UnicodePointToStr(int uc);
int SurrogateToUnicodePoint(UnicodeString c);
UnicodeString ChEntRef_to_NumChRef(UnicodeString s);

int  check_UTF8(BYTE *bp, int size);
int  get_MemoryCodePage(TMemoryStream *ms, bool *has_bom = NULL);
UnicodeString get_MemoryStrins(TMemoryStream *ms);

UnicodeString check_Surrogates(UnicodeString s);
UnicodeString check_EnvDepandChars(UnicodeString s);

int get_NrmLevenshteinDistance(UnicodeString s1, UnicodeString s2,
	bool ig_case = false, bool ig_num = false, bool ig_fh = false);

UnicodeString get_JsonValStr(TJSONValue *val);
void format_Json(TJSONValue *val, TStringList *lst, int lvl = 0);

UnicodeString decode_TxtVal(UnicodeString s, bool with_q = false);
UnicodeString conv_DfmText(UnicodeString s);
void		  conv_DfmText(TStringList *txt_lst);

UnicodeString make_MenuAccStr(int idx, bool alp_sw = false);

UnicodeString get_NextAlStr(UnicodeString s);

TDateTime str_to_DateTime(UnicodeString ts, bool sw_dt = false);
//---------------------------------------------------------------------------
#endif
