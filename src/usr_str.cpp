//----------------------------------------------------------------------//
// 文字列操作															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <vector>
#include <memory>
#include <math.h>
#include <System.StrUtils.hpp>
#include <System.Masks.hpp>
#include <RegularExpressions.hpp>
#include <Vcl.Direct2D.hpp>
#include "usr_str.h"

//---------------------------------------------------------------------------
const _TCHAR *null_TCHAR = _T("");

//---------------------------------------------------------------------------
//自然順ソートに対応した比較
//---------------------------------------------------------------------------
int __fastcall comp_NaturalOrder(TStringList *List, int Index1, int Index2)
{
	return StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str());
}

//---------------------------------------------------------------------------
//自然昇順 文字列 (カンマ対応/ 同じ場合は Objects で比較)
//---------------------------------------------------------------------------
int __fastcall comp_AscendOrder(TStringList *List, int Index1, int Index2)
{
	UnicodeString s1  = List->Strings[Index1];
	UnicodeString s2  = List->Strings[Index2];
	UnicodeString ns1 = extract_top_num_str(s1);
	UnicodeString ns2 = extract_top_num_str(s2);

	int res = (!ns1.IsEmpty() && !ns2.IsEmpty())? StrCmpLogicalW(ns1.c_str(), ns2.c_str()) : 0;
	if (res==0) res = StrCmpLogicalW(s1.c_str(), s2.c_str());
	if (res==0) res = (int)List->Objects[Index1] - (int)List->Objects[Index2];
	return res;
}
//---------------------------------------------------------------------------
//自然降順 文字列 (カンマ対応/ 同じ場合は Objects で比較)
//---------------------------------------------------------------------------
int __fastcall comp_DescendOrder(TStringList *List, int Index1, int Index2)
{
	UnicodeString s1  = List->Strings[Index1];
	UnicodeString s2  = List->Strings[Index2];
	UnicodeString ns1 = extract_top_num_str(s1);
	UnicodeString ns2 = extract_top_num_str(s2);

	int res = (!ns1.IsEmpty() && !ns2.IsEmpty())? StrCmpLogicalW(ns2.c_str(), ns1.c_str()) : 0;
	if (res==0) res = StrCmpLogicalW(s2.c_str(), s1.c_str());
	if (res==0) res = (int)List->Objects[Index2] - (int)List->Objects[Index1];
	return res;
}
//---------------------------------------------------------------------------
//昇順 Objects
//---------------------------------------------------------------------------
int __fastcall comp_ObjectsOrder(TStringList *List, int Index1, int Index2)
{
	return (int)List->Objects[Index1] - (int)List->Objects[Index2];
}

//---------------------------------------------------------------------------
//CSV用比較関数
//---------------------------------------------------------------------------
int  USR_CsvCol 	 = 0;		//ソート対象列
int  USR_CsvSortMode = 1;		//ソートモード 1:昇順/ -1降順
bool USR_CsvTopIsHdr = false;	//先頭行は項目名

//---------------------------------------------------------------------------
//文字列を数値として比較
//---------------------------------------------------------------------------
int comp_NumStr(UnicodeString s1, UnicodeString s2)
{
	int res = 0;
	s1 = extract_top_num_str(s1);
	s2 = extract_top_num_str(s2);
	if (!s1.IsEmpty() && !s2.IsEmpty()) {
		try {
			//符号無し整数
			if (s1.Pos('.')==0 && s2.Pos('.')==0 && s1[1]!='+' && s1[1]!='+' && s1[1]!='-' && s1[1]!='-') {
				res = StrCmpLogicalW(s1.c_str(), s2.c_str());
			}
			//実数
			else {
				double r1 = s1.ToDouble();
				double r2 = s2.ToDouble();
				double r0 = std::min(fabsl(r1), fabsl(r2));
				double k  = (r0>0 && r0<1)? 1000/r0 : 1000;
				res = (int)((r1 - r2) * k);
			}
		}
		catch (...) {
			res = 0;
		}
	}
	return res;
}

//---------------------------------------------------------------------------
//CSV 自然昇順 (同じ場合は Objects で比較)
//  数値として比較できなければ文字列順、同じならば行番号順
//---------------------------------------------------------------------------
int __fastcall comp_CsvNaturalOrder(TStringList *List, int Index1, int Index2)
{
	if (USR_CsvTopIsHdr) {	//先頭行は項目名
		if ((int)List->Objects[Index1]==0) return -1;
		if ((int)List->Objects[Index2]==0) return  1;
	}

	UnicodeString s1 = get_csv_item(List->Strings[Index1], USR_CsvCol);
	UnicodeString s2 = get_csv_item(List->Strings[Index2], USR_CsvCol);

	int res = comp_NumStr(s1, s2);
	if (res==0) res = StrCmpLogicalW(s1.c_str(), s2.c_str());
	if (res==0) res = (int)List->Objects[Index1] - (int)List->Objects[Index2];
	res *= USR_CsvSortMode;
	return res;
}
//---------------------------------------------------------------------------
//TSV 自然昇順 (同じ場合は Objects で比較)
//  数値として比較できなければ文字列順、同じならば行番号順
//---------------------------------------------------------------------------
int __fastcall comp_TsvNaturalOrder(TStringList *List, int Index1, int Index2)
{
	if (USR_CsvTopIsHdr) {	//先頭行は項目名
		if ((int)List->Objects[Index1]==0) return -1;
		if ((int)List->Objects[Index2]==0) return  1;
	}

	TStringDynArray itm1 = SplitString(List->Strings[Index1], "\t");
	TStringDynArray itm2 = SplitString(List->Strings[Index2], "\t");
	UnicodeString s1  = (USR_CsvCol<itm1.Length)? itm1[USR_CsvCol] : EmptyStr;
	UnicodeString s2  = (USR_CsvCol<itm2.Length)? itm2[USR_CsvCol] : EmptyStr;

	int res = comp_NumStr(s1, s2);
	if (res==0) res = StrCmpLogicalW(s1.c_str(), s2.c_str());
	if (res==0) res = (int)List->Objects[Index1] - (int)List->Objects[Index2];
	res *= USR_CsvSortMode;
	return res;
}

//---------------------------------------------------------------------------
//指定セパレータの前の文字列を取得
//  セパレータが見つからない場合は、文字列をそのまま返す
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, const _TCHAR *sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}
//---------------------------------------------------------------------------
UnicodeString get_tkn(UnicodeString s, WideChar sp)
{
	int p = s.Pos(sp);
	return (p==0)? s : s.SubString(1, p - 1);
}

//---------------------------------------------------------------------------
//指定セパレータの後の文字列を取得
//  セパレータが見つからない場合は、EmptyStr を返す
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, UnicodeString sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + sp.Length(),  s.Length() - (p - sp.Length() + 1));
}
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, const _TCHAR *sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + _tcslen(sp),  s.Length() - (p - _tcslen(sp) + 1));
}
//---------------------------------------------------------------------------
UnicodeString get_tkn_r(UnicodeString s, WideChar sp)
{
	int p = s.Pos(sp);
	return (p==0)? EmptyStr : s.SubString(p + 1,  s.Length() - p);
}

//---------------------------------------------------------------------------
//指定セパレータ間の文字列を取得
//  後セパレータが見つからない場合は、前セパレータ以降の文字列を返す
//---------------------------------------------------------------------------
UnicodeString get_tkn_m(UnicodeString s, const _TCHAR *sp1, const _TCHAR *sp2)
{
	return get_tkn(get_tkn_r(s, sp1), sp2);
}
//---------------------------------------------------------------------------
UnicodeString get_tkn_m(UnicodeString s, WideChar sp1, WideChar sp2)
{
	return get_tkn(get_tkn_r(s, sp1), sp2);
}

//---------------------------------------------------------------------------
//タブの前の文字列を取得
//  タブがない場合は、文字列をそのまま返す
//---------------------------------------------------------------------------
UnicodeString get_pre_tab(UnicodeString s)
{
	return get_tkn(s, '\t');
}
//---------------------------------------------------------------------------
//タブの後の文字列を取得
//  タブがない場合は、EmptyStr を返す
//---------------------------------------------------------------------------
UnicodeString get_post_tab(UnicodeString s)
{
	return get_tkn_r(s, '\t');
}

//---------------------------------------------------------------------------
//複数行文字列の1行目の内容を取得
//---------------------------------------------------------------------------
UnicodeString get_first_line(UnicodeString s)
{
	UnicodeString ret_str;
	for (int i=1; i<=s.Length(); i++) {
		if (s.IsDelimiter("\r\n", i)) break;
		ret_str.cat_sprintf(_T("%c"), s[i]);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//文字列中の (～) 内を取得
//---------------------------------------------------------------------------
UnicodeString get_in_paren(UnicodeString s)
{
	int p1 = s.Pos('(');
	int p2 = s.Pos(')');
	if (p1==0 || p2==0 || p1>=p2) return EmptyStr;
	return s.SubString(p1 + 1,  p2 - p1 - 1);
}
//---------------------------------------------------------------------------
//文字列中の (～) 内を取得、元文字列は (～) の後続文字列に
//---------------------------------------------------------------------------
UnicodeString split_in_paren(UnicodeString &s)
{
	return get_tkn_r(split_tkn(s, ')'), '(');
}

//---------------------------------------------------------------------------
//前後にタブを含まない文字列を取得
//  複数行の場合、空でない最初の文字列
//---------------------------------------------------------------------------
UnicodeString get_norm_str(UnicodeString s)
{
	UnicodeString ret_str;
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = s;
	for (int i=0; i<lst->Count; i++) {
		ret_str = lst->Strings[i];
		ret_str = replace_regex(ret_str, _T("^\\t+"), null_TCHAR);
		ret_str = replace_regex(ret_str, _T("\\t+$"), null_TCHAR);
		if (!ret_str.IsEmpty()) break;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//指定セパレータ前の文字列を分離して取得
//  セパレータが見つからない場合は、文字列をそのまま返し、元文字列は空に
//---------------------------------------------------------------------------
UnicodeString split_tkn(UnicodeString &s, UnicodeString sp)
{
	UnicodeString r;
	int p = s.Pos(sp);
	if (p==0) {
		r = s;
		s = EmptyStr;
	}
	else {
		r = s.SubString(1, p - 1);
		s = s.SubString(p + sp.Length(), s.Length() - (p - sp.Length() + 1));
	}
	return r;
}
//---------------------------------------------------------------------------
UnicodeString split_tkn(UnicodeString &s, const _TCHAR *sp)
{
	return split_tkn(s, UnicodeString(sp));
}
//---------------------------------------------------------------------------
UnicodeString split_tkn(UnicodeString &s, WideChar sp)
{
	UnicodeString r;
	int p = s.Pos(sp);
	if (p==0) {
		r = s;
		s = EmptyStr;
	}
	else {
		r = s.SubString(1, p - 1);
		s = s.SubString(p + 1, s.Length() - p);
	}
	return r;
}

//---------------------------------------------------------------------------
//空白区切り前の文字列を分離して取得(先頭部分の空白を考慮)
//---------------------------------------------------------------------------
UnicodeString split_tkn_spc(UnicodeString &s)
{
	UnicodeString r;
	for (int i=1; i<=s.Length(); i++) {
		if (s[i]!=' ') break;
		r.UCAT_T(" ");
	}
	if (!r.IsEmpty()) s.Delete(1, r.Length());
	return (r + split_tkn(s, ' '));
}

//---------------------------------------------------------------------------
//タブ前の文字列を分離して取得
//  タブがない場合は、文字列をそのまま返し、元文字列は空に
//---------------------------------------------------------------------------
UnicodeString split_pre_tab(UnicodeString &s)
{
	return split_tkn(s, '\t');
}

//---------------------------------------------------------------------------
//文字列から説明部分(^:～:)を分離し取得
//---------------------------------------------------------------------------
UnicodeString split_dsc(UnicodeString &s)
{
	UnicodeString dsc;
	if (!StartsStr("::{", s) && StartsStr(":", s)) {
		s.Delete(1, 1);
		dsc = split_tkn(s, ':');
		s = Trim(s);
	}
	return dsc;
}

//---------------------------------------------------------------------------
//文字列から先頭文字を分離し取得
//---------------------------------------------------------------------------
UnicodeString split_top_ch(UnicodeString &s)
{
	UnicodeString c;
	if (!s.IsEmpty()) {
		c = s.SubString(1, 1);
		s.Delete(1, 1);
	}
	return c;
}
//---------------------------------------------------------------------------
WideChar split_top_wch(UnicodeString &s)
{
	WideChar c = '\0';
	if (!s.IsEmpty()) {
		c = s[1];
		s.Delete(1, 1);
	}
	return c;
}

//---------------------------------------------------------------------------
//コマンドラインをパラメータに分解
//---------------------------------------------------------------------------
int split_cmd_line(UnicodeString s, TStringList *lst)
{
	bool in_qut = false;
	UnicodeString lbuf;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		//引用符外
		if (!in_qut) {
			if (c=='\"')
				in_qut = true;
			else if (c==' ') {
				if (!lbuf.IsEmpty()) {
					lst->Add(lbuf); 
					lbuf = EmptyStr;
				}
				continue;
			}
		}
		//引用符内
		else if (c=='\"') {
			in_qut = false;
		}

		lbuf.cat_sprintf(_T("%c"), c);
	}

	if (!lbuf.IsEmpty()) lst->Add(lbuf);

	return lst->Count;
}

//---------------------------------------------------------------------------
//コマンドラインからファイル名を分離
//---------------------------------------------------------------------------
UnicodeString split_file_param(UnicodeString &s)
{
	UnicodeString ret_str;
	s = Trim(s);
	if (remove_top_s(s, '\"')) {
		ret_str = split_tkn(s, '\"');
		s = Trim(s);
	}
	else
		ret_str = split_tkn(s, ' ');
	return ret_str;
}

//---------------------------------------------------------------------------
//文字列を指定区切りで分割
//---------------------------------------------------------------------------
TStringDynArray split_strings_tab(UnicodeString s)
{
	return SplitString(s, "\t");
}
//---------------------------------------------------------------------------
TStringDynArray split_strings_semicolon(UnicodeString s)
{
	return SplitString(s, ";");
}

//---------------------------------------------------------------------------
//文字列から指定語を削除
//  戻り値: ture=削除した/ false=語が含まれていない
//---------------------------------------------------------------------------
bool remove_text(UnicodeString &s, UnicodeString w)
{
	if (ContainsText(s, w)) {
		s = ReplaceText(s, w, EmptyStr);
		return true;
	}
	else
		return false;
}
//---------------------------------------------------------------------------
bool remove_text(UnicodeString &s, const _TCHAR *w)
{
	return remove_text(s, UnicodeString(w));
}

//---------------------------------------------------------------------------
//文字列の先頭に指定語があったら削除 (大小文字を無視)
//  戻り値: ture=削除した/ false=先頭に語がない
//---------------------------------------------------------------------------
bool remove_top_text(UnicodeString &s, UnicodeString w)
{
	if (w.IsEmpty() || !StartsText(w, s)) return false;
	s.Delete(1, w.Length());
	return true;
}
//---------------------------------------------------------------------------
bool remove_top_text(UnicodeString &s, const _TCHAR *w)
{
	return remove_top_text(s, UnicodeString(w));
}

//---------------------------------------------------------------------------
//文字列の先頭に指定語があったら削除 (大小文字を区別)
//  戻り値:  ture=削除した/ false=先頭に語がない
//---------------------------------------------------------------------------
bool remove_top_s(UnicodeString &s, UnicodeString w)
{
	if (!StartsStr(w, s)) return false;
	s.Delete(1, w.Length());
	return true;
}
//---------------------------------------------------------------------------
bool remove_top_s(UnicodeString &s, const _TCHAR *w)
{
	if (!StartsStr(w, s)) return false;
	s.Delete(1, _tcslen(w));
	return true;
}
//---------------------------------------------------------------------------
bool remove_top_s(UnicodeString &s, WideChar w)
{
	if (!StartsStr(w, s)) return false;
	s.Delete(1, 1);
	return true;
}

//---------------------------------------------------------------------------
//文字列の先頭に @ があったら削除
//  戻り値: ture=先頭の @ を削除した
//---------------------------------------------------------------------------
bool remove_top_AT(UnicodeString &s)
{
	return remove_top_s(s, '@');
}
//---------------------------------------------------------------------------
//文字列の先頭に $ があったら削除
//  戻り値: ture=先頭の $ を削除した
//---------------------------------------------------------------------------
bool remove_top_Dollar(UnicodeString &s)
{
	return remove_top_s(s, '$');
}

//---------------------------------------------------------------------------
//文字列の末尾に指定語があったら削除 (大小文字を無視)
//  戻り値: ture=末尾語を削除した
//---------------------------------------------------------------------------
bool remove_end_text(UnicodeString &s, UnicodeString w)
{
	if (!EndsText(w, s)) return false;
	s.Delete(s.Length() - w.Length() + 1, w.Length());
	return true;
}
//---------------------------------------------------------------------------
//文字列の末尾に指定語があったら削除 (大小文字を区別)
//  戻り値: ture=末尾語を削除した
//---------------------------------------------------------------------------
bool remove_end_s(UnicodeString &s, UnicodeString w)
{
	if (!EndsStr(w, s)) return false;
	s.Delete(s.Length() - w.Length() + 1, w.Length());
	return true;
}
//---------------------------------------------------------------------------
bool remove_end_s(UnicodeString &s, WideChar w)
{
	if (!EndsStr(w, s)) return false;
	s.Delete(s.Length(), 1);
	return true;
}

//---------------------------------------------------------------------------
void delete_end(UnicodeString &s)
{
	s.Delete(s.Length(), 1);
}

//---------------------------------------------------------------------------
//先頭を除外した文字列を取得
//---------------------------------------------------------------------------
UnicodeString exclude_top(UnicodeString s)
{
	return s.SubString(2, s.Length() - 1);
}
//---------------------------------------------------------------------------
//先頭と末尾を除外した文字列を取得
//---------------------------------------------------------------------------
UnicodeString exclude_top_end(UnicodeString s)
{
	return s.SubString(2, s.Length() - 2);
}

//---------------------------------------------------------------------------
//全角スペースも考慮したトリミング
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString s)
{
	//先頭
	for (;;) {
		if (remove_top_s(s, "　")) continue;
		if (remove_top_s(s, ' '))  continue;
		if (remove_top_s(s, '\t')) continue;
		break;
	}
	//末尾
	for (;;) {
		if (remove_end_s(s, "　")) continue;
		if (remove_end_s(s, ' '))  continue;
		if (remove_end_s(s, '\t')) continue;
		break;
	}

	return Trim(s);
}

//---------------------------------------------------------------------------
//文字列の置換
//---------------------------------------------------------------------------
UnicodeString replace_i(UnicodeString s, const _TCHAR *o, const _TCHAR *r)
{
	return ReplaceText(s, o, r);
}
//---------------------------------------------------------------------------
UnicodeString replace_s(UnicodeString s, const _TCHAR *o, const _TCHAR *r)
{
	return ReplaceStr(s, o, r);
}

//---------------------------------------------------------------------------
//正規表現による置換
//---------------------------------------------------------------------------
UnicodeString replace_regex(UnicodeString s, const _TCHAR *o, const _TCHAR *r)
{
	return TRegEx::Replace(s, o, r);
}

//---------------------------------------------------------------------------
//リストにしたがって文字列を置換
//  リスト書式: 検索文字列 TAB 置換文字列
//---------------------------------------------------------------------------
UnicodeString replace_str_by_list(UnicodeString s, TStringList *lst)
{
	for (int i=0; i<lst->Count; i++) {
		UnicodeString t_s = lst->Strings[i];
		UnicodeString f_s = split_pre_tab(t_s);
		if (!f_s.IsEmpty()) s = ReplaceStr(s, f_s, t_s);
	}
	return s;
}

//---------------------------------------------------------------------------
//文字列を末尾に追加したものを取得
//---------------------------------------------------------------------------
UnicodeString append_str(UnicodeString s, const _TCHAR *t)
{
	return s + UnicodeString(t);
}

//---------------------------------------------------------------------------
//; 区切りで文字列を追加
//---------------------------------------------------------------------------
void cat_str_semicolon(UnicodeString &s, UnicodeString t)
{
	if (EndsStr(";", t)) t.Delete(t.Length(), 1);
	if (t.IsEmpty()) return;

	if (!s.IsEmpty() && !EndsStr(";", s)) s.UCAT_T(";");
	s += t;
}

//---------------------------------------------------------------------------
//指定長になるまで先頭に空白を挿入
//---------------------------------------------------------------------------
UnicodeString ins_spc_length(UnicodeString s, int len)
{
	int n = len - s.Length();
	if (n>0) s.Insert(StringOfChar(_T(' '), n), 1);
	return s;
}

//---------------------------------------------------------------------------
//文字列が空ならデフォルト文字列を取得
//---------------------------------------------------------------------------
UnicodeString def_if_empty(UnicodeString s, UnicodeString def)
{
	return s.IsEmpty()? def : s;
}
//---------------------------------------------------------------------------
UnicodeString def_if_empty(UnicodeString s, const _TCHAR *def)
{
	return s.IsEmpty()? UnicodeString(def) : s;
}

//---------------------------------------------------------------------------
//文字列を NativeInt 値に変換
//---------------------------------------------------------------------------
NativeInt str_to_NativeInt(UnicodeString s)
{
#if defined(_WIN64)
	return (NativeInt)StrToInt64Def(s, 0);
#else
	return (NativeInt)s.ToIntDef(0);
#endif
}

//---------------------------------------------------------------------------
//文字列から整数値を抽出
//！失敗したら例外を送出
//---------------------------------------------------------------------------
int extract_int(UnicodeString s)
{
	int i = 1;
	while (i<=s.Length()) if (!iswdigit(s[i])) s.Delete(i, 1); else i++;
	return s.ToInt();
}
//---------------------------------------------------------------------------
//文字列から整数値を抽出
//無効の場合はデフォルト値を返す
//---------------------------------------------------------------------------
int extract_int_def(UnicodeString s, int def)
{
	int i = 1;
	while (i<=s.Length()) if (!iswdigit(s[i])) s.Delete(i, 1); else i++;
	return s.ToIntDef(def);
}

//---------------------------------------------------------------------------
//文字列の先頭から数値部分を取得(カンマは無視、+/-、コンマに対応)
//---------------------------------------------------------------------------
UnicodeString extract_top_num_str(UnicodeString s)
{
	UnicodeString ret_str;

	s = Trim(s);
	if (s.Pos('/')==0 && s.Pos(':')==0) {
		for (int i=1; i<=s.Length(); i++) {
			WideChar c = s[i];
			if (i==1) {
				if (c==',') break;
				if (c=='+' || c=='-' || c=='.' || iswdigit(c)) ret_str = c;
			}
			else if (c==',') continue;
			else if (iswdigit(c) || c=='.') ret_str.cat_sprintf(_T("%c"), c);
			else break;
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//long double 値を文字列に変換
//---------------------------------------------------------------------------
UnicodeString ldouble_to_str(
	long double v,
	int f_w)		//double の小数部桁数	(default = 3)
{
	UnicodeString ret_str;

	long double ip;
	//整数
	if (modfl(v, &ip)==0 && ip>=_I64_MIN && ip<=_I64_MAX) {
		ret_str.sprintf(_T("%lld"), (__int64)ip);
	}
	//実数
	else {
		ret_str.sprintf(_T("%.*LG"), 16, v);
		if (v>=DBL_MIN && v<=DBL_MAX && ret_str.Pos('.') && f_w>0)
			ret_str.sprintf(_T("%.*f"), f_w, (double)v);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//サイズの数値を文字列に(TBまで)
//---------------------------------------------------------------------------
UnicodeString get_size_str_T(__int64 sz, int dg)
{
	UnicodeString szstr;
	float sz_k = sz/1024.0;
	float sz_m = sz_k/1024.0;
	float sz_g = sz_m/1024.0;

	if      (sz_g>1024) szstr.sprintf(_T("%.*f TB"), dg, sz_g/1024.0);
	else if (sz_m>1024) szstr.sprintf(_T("%.*f GB"), dg, sz_m/1024.0);
	else if (sz_k>1024) szstr.sprintf(_T("%.*f MB"), dg, sz_k/1024.0);
	else				szstr.sprintf(_T("%.*f KB"), dg, sz_k);
	return szstr;
}
//---------------------------------------------------------------------------
//サイズの数値を文字列に(GBまで)
//---------------------------------------------------------------------------
UnicodeString get_size_str_G(
	__int64 sz,
	int wd,		//桁指定 default = 10
	int dg)
{
	UnicodeString szstr;
	if (sz>=0) {
		if		(sz>=1073741824ull)	szstr.sprintf(_T("%5.*f GB"), dg, 1.0*sz/1073741824ull);
		else if (sz>=1048576ul)		szstr.sprintf(_T("%5.*f MB"), dg, 1.0*sz/1048576ul);
		else						szstr = FormatFloat(",0", (double)sz);
	}
	return ins_spc_length(szstr, wd);
}
//---------------------------------------------------------------------------
UnicodeString get_size_str_G(__int64 sz, int wd, int dg, int fmt_mode)
{
	UnicodeString szstr;
	if (sz>=0) {
		if		(sz>=1073741824ull)	szstr.sprintf(_T("%5.*f GB"), dg, 1.0*sz/1073741824ull);
		else if (sz>=1048576ul)		szstr.sprintf(_T("%5.*f MB"), dg, 1.0*sz/1048576ul);
		else {
			if ((fmt_mode==1 && sz>=1024) || fmt_mode==2)
				szstr.sprintf(_T("%5.*f KB"), dg, 1.0*sz/1024);
			else
				szstr = FormatFloat(",0", (double)sz);
		}
	}
	return ins_spc_length(szstr, wd);
}

//---------------------------------------------------------------------------
UnicodeString get_size_str_B(
	__int64 sz,
	int wd)		//桁指定 default = 14
{
	return ins_spc_length(((sz>=0)? FormatFloat(",0", (double)sz) : EmptyStr), wd);
}
//---------------------------------------------------------------------------
UnicodeString get_size_str_C(int sz)
{
	return FormatFloat(",0", (double)sz);
}

//---------------------------------------------------------------------------
//RRGGBB形式の文字列を TColor に変換
//---------------------------------------------------------------------------
TColor xRRGGBB_to_col(UnicodeString s)
{
	return (s.Length()==6)?
			TColor(RGB(("0x" + s.SubString(1,2)).ToIntDef(0),
					   ("0x" + s.SubString(3,2)).ToIntDef(0),
					   ("0x" + s.SubString(5,2)).ToIntDef(0))) :
			clNone;
}
//---------------------------------------------------------------------------
//TColor をRRGGBB形式の文字列に変換
//---------------------------------------------------------------------------
UnicodeString col_to_xRRGGBB(TColor col)
{
	int cref = ColorToRGB(col);
	return UnicodeString().sprintf(_T("%02X%02X%02X"), GetRValue(cref), GetGValue(cref), GetBValue(cref));
}

//---------------------------------------------------------------------------
//大小文字を区別せずに文字列を検索
//---------------------------------------------------------------------------
int pos_i(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;
	return s.UpperCase().Pos(wd.UpperCase());
}
//---------------------------------------------------------------------------
int pos_i(const _TCHAR *wd, UnicodeString s)
{
	return pos_i(UnicodeString(wd), s);
}

//---------------------------------------------------------------------------
//文字列が最後に現れる位置を取得
//---------------------------------------------------------------------------
int pos_r(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;

	int p  = 0;
	int p0 = 1;
	for (;;) {
		int p1 = PosEx(wd, s, p0);
		if (p1==0) break;
		p  = p1;
		p0 = p + 1;
	}

	return p;
}
//---------------------------------------------------------------------------
int pos_r(const _TCHAR *wd, UnicodeString s)
{
	return pos_r(UnicodeString(wd), s);
}

//---------------------------------------------------------------------------
//大小文字を区別せずに文字列が最後に現れる位置を取得
//---------------------------------------------------------------------------
int pos_r_i(
	UnicodeString wd,	//検索語
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;

	UnicodeString ss = s;
	int p = 0;
	for (;;) {
		int p1 = pos_i(wd, ss);
		if (p1==0) break;
		ss.Delete(1, p1);
		p += p1;
	}

	return p;
}

//---------------------------------------------------------------------------
//引用符外で、文字列が最後に現れる位置を取得
//---------------------------------------------------------------------------
int pos_r_q(
	UnicodeString wd,	//検索語( \v は除く)
	UnicodeString s)	//対象文字列
{
	if (wd.IsEmpty()) return 0;
	UnicodeString ss;
	bool q_flag = false;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		//引用符外
		if (!q_flag) {
			if (c=='\"') q_flag = true;
		}
		//引用符内
		else {
			if (c=='\"')
				q_flag = false;
			else
				c = '\v';
		}
		ss += c;
	}
	return pos_r(wd, ss);
}

//---------------------------------------------------------------------------
//引用符外で、: が最後に現れる位置を取得 (:\\ は除く)
//---------------------------------------------------------------------------
int pos_r_q_colon(UnicodeString s)
{
	return pos_r_q(":", REPLACE_TS(s, ":\\", "\f\f"));
}

//---------------------------------------------------------------------------
bool contains_i(UnicodeString s, const _TCHAR *w)
{
	return ContainsText(s, w);
}
//---------------------------------------------------------------------------
bool contains_i(UnicodeString s, WideChar w)
{
	return ContainsText(s, w);
}
//---------------------------------------------------------------------------
bool contains_s(UnicodeString s, const _TCHAR *w)
{
	return ContainsStr(s, w);
}
//---------------------------------------------------------------------------
bool contains_s(UnicodeString s, WideChar w)
{
	return ContainsStr(s, w);
}

//---------------------------------------------------------------------------
//文字列に検索語が含まれているか?
//  AND(' ') / OR('|') 検索対応
//---------------------------------------------------------------------------
bool contains_word_and_or(
	UnicodeString s,
	UnicodeString kwd,	//検索語
	bool case_sw)		//大小文字区別
{
	std::unique_ptr<TStringList> ptn_lst(new TStringList());
	TStringDynArray wd_lst = SplitString(Trim(kwd), " ");
	for (int i=0; i<wd_lst.Length; i++) if (!wd_lst[i].IsEmpty()) ptn_lst->Add(wd_lst[i]);

	if (ptn_lst->Count==0) return false;

	bool ok = true;
	for (int i=0; i<ptn_lst->Count && ok; i++) {
		UnicodeString wds = ptn_lst->Strings[i];
		bool or_ok = false;
		do {
			UnicodeString wd = split_tkn(wds, '|');
			if (!wd.IsEmpty()) {
				if		(case_sw && ContainsStr(s, wd))	or_ok = true;
				else if (!case_sw && ContainsText(s, wd))	or_ok = true;
			}
		} while (!or_ok && !wds.IsEmpty());
		ok = or_ok;
	}
	
	return ok;
}

//---------------------------------------------------------------------------
// | 区切りリストに指定語が含まれているか？
//---------------------------------------------------------------------------
bool contained_wd_i(UnicodeString lst, UnicodeString wd)
{
	if (wd.IsEmpty()) return false;

	if (!StartsStr("|", lst)) lst.Insert("|" ,1);
	if (!EndsStr("|", lst))   lst.UCAT_T("|");
	return ContainsText(lst, "|" + wd + "|");
}
//---------------------------------------------------------------------------
bool contained_wd_i(const _TCHAR *lst, UnicodeString wd)
{
	return contained_wd_i(UnicodeString(lst), wd);
}

//---------------------------------------------------------------------------
// | 区切りリストの語を含んでいるか？
//---------------------------------------------------------------------------
bool contains_wd_i(UnicodeString s, const _TCHAR *lst)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	for (int i=0; i<w_lst.Length; i++) if (ContainsText(s, w_lst[i])) return true;
	return false;
}

//---------------------------------------------------------------------------
// | 区切りリストから指定インデックスの語を取得
//---------------------------------------------------------------------------
UnicodeString get_word_i_idx(const _TCHAR *lst, int idx)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	return (idx>=0 && idx<w_lst.Length)? w_lst[idx] : EmptyStr;
}
//---------------------------------------------------------------------------
// | 区切りリストから指定語のインデックスを取得
//---------------------------------------------------------------------------
int idx_of_word_i(const _TCHAR *lst, UnicodeString wd)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	int idx = -1;
	for (int i=0; i<w_lst.Length && idx==-1; i++) if (SameText(w_lst[i], wd)) idx = i;
	return idx;
}
//---------------------------------------------------------------------------
// | 区切りリストにある項目か?
//---------------------------------------------------------------------------
bool test_word_i(UnicodeString w, UnicodeString list)
{
	if (list.IsEmpty() || w.IsEmpty()) return false;
	if (!StartsStr('|', w))		w.Insert("|", 1);
	if (!EndsStr('|', w)) 		w.UCAT_T("|");
	if (!StartsStr('|', list))	list.Insert("|", 1);
	if (!EndsStr('|', list)) 	list.UCAT_T("|");
	return ContainsText(list, w);
}

//---------------------------------------------------------------------------
//ワイルドカードを含むパターンによる文字列一致
//---------------------------------------------------------------------------
bool str_match(UnicodeString ptn, UnicodeString s)
{
	UnicodeString c = ptn.SubString(1, 1);
	UnicodeString next_str = s.SubString(2, s.Length() - 1);
	UnicodeString next_ptn = ptn.SubString(2, ptn.Length() - 1);

	if (c.IsEmpty())
		return s.IsEmpty();
	else if (SameStr(c, "*"))
		return str_match(next_ptn, s) || (!s.IsEmpty() && str_match(ptn, next_str));
	else if (SameStr(c, "?"))
		return !s.IsEmpty() && str_match(next_ptn, next_str);
	else {
		return (CompareText(c, s.SubString(1, 1))==0) && str_match(next_ptn, next_str);
	}
}

//---------------------------------------------------------------------------
//正規表現パターンのチェック
//---------------------------------------------------------------------------
bool chk_RegExPtn(UnicodeString ptn)
{
	if (ptn.IsEmpty()) return false;

	try {
		TRegEx x(ptn, TRegExOptions() << roCompiled);
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
//正規表現によるマッチ
//---------------------------------------------------------------------------
bool is_match_regex(UnicodeString s, const _TCHAR *ptn)
{
	return TRegEx::IsMatch(s, ptn);
}
//---------------------------------------------------------------------------
bool is_match_regex_i(UnicodeString s, const _TCHAR *ptn)
{
	TRegExOptions opt; opt << roIgnoreCase;
	return TRegEx::IsMatch(s, ptn, opt);
}

//---------------------------------------------------------------------------
// ; 区切り文字列から正規表現パターン(/～/)を抽出
//---------------------------------------------------------------------------
UnicodeString extract_prm_RegExPtn(UnicodeString &s)
{
	UnicodeString ret_str;

	TStringDynArray lst = split_strings_semicolon(s);
	int idx = -1;
	for (int i=0; i<lst.Length && idx==-1; i++) {
		if (is_regex_slash(lst[i])) {
			ret_str = lst[i];
			idx = i;
		}
	}

	//該当パラメータがあったら削除
	if (idx!=-1) {
		UnicodeString lbuf;
		for (int i=0; i<lst.Length; i++) {
			if (i==idx) continue;
			if (!lbuf.IsEmpty()) lbuf.UCAT_T(";");
			lbuf += lst[i];
		}
		s = lbuf;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//文字列に指定語(またはパターン)が含まれているか？
//  戻り値: マッチした部分の文字列
//---------------------------------------------------------------------------
UnicodeString ptn_match_str(
	UnicodeString ptn,	//キーワード (;で区切って複数指定可)
						//  /～/ なら正規表現パターン
	UnicodeString s)	//対象文字列
{
	if (ptn.IsEmpty() || s.IsEmpty()) return EmptyStr;

	TStringDynArray p_lst = SplitString(ptn, ";");
	TRegExOptions opt; opt << roIgnoreCase;
	try {
		UnicodeString ret_str;
		for (int i=0; i<p_lst.Length && ret_str.IsEmpty(); i++) {
			UnicodeString kwd = p_lst[i];	if (kwd.IsEmpty()) continue;
			//正規表現
			if (is_regex_slash(kwd)) {
				TMatch mt = TRegEx::Match(s, exclude_top_end(kwd), opt);
				ret_str = mt.Success? mt.Value : EmptyStr;
			}
			//通常
			else {
				int p = pos_i(kwd, s);
				ret_str = (p>0)? s.SubString(p, kwd.Length()) : EmptyStr;
			}
		}
		return ret_str;
	}
	catch (...) {
		return EmptyStr;
	}
}
//---------------------------------------------------------------------------
//文字列に指定語(またはパターン)で始まるか？
//---------------------------------------------------------------------------
bool starts_ptn(
	UnicodeString ptn,	//開始文字列 (/～/ なら正規表現パターン)
	UnicodeString s)	//対象文字列
{
	try {
		bool res;
		//正規表現
		if (is_regex_slash(ptn)) {
			TRegExOptions opt; opt << roIgnoreCase;
			TMatch mt = TRegEx::Match(s, exclude_top_end(ptn), opt);
			res = mt.Success && (mt.Index==1);
		}
		//通常
		else {
			res = StartsText(ptn, s);
		}
		return res;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//検索語リスト作成
//---------------------------------------------------------------------------
void get_find_wd_list(UnicodeString wd, TStringList *lst)
{
	for (;;) {
		if (wd.IsEmpty()) break;
		int ph = wd.Pos(' ');
		int pw = wd.Pos("　");
		int p = (ph && !pw)? ph : (!ph && pw)? pw : (ph && pw)? std::min(ph, pw) : 0;
		if (p==1)
			wd.Delete(1, 1);
		else if (p>1) {
			lst->Add(wd.SubString(1, p - 1));
			wd.Delete(1, p);
		}
		else {
			lst->Add(wd); break;
		}
	}

	//空白やタブを変換
	for (int i=0; i<lst->Count; i++) lst->Strings[i] = conv_esc_char(lst->Strings[i]);
}

//---------------------------------------------------------------------------
//複数の文字列を OR/AND 検索
//---------------------------------------------------------------------------
bool find_mlt(
	UnicodeString wd,	//検索語 (複数の場合は半角または全角空白で区切る）
	UnicodeString s,	//対象文字列
	bool and_sw,		//AND検索			(default = false)
	bool not_sw,		//NOT検索			(default = false)
	bool case_sw)		//大小文字を区別	(default = false)
{
	if (wd.IsEmpty()) return false;

	//検索語リスト作成
	std::unique_ptr<TStringList> wlst(new TStringList());
	get_find_wd_list(wd, wlst.get());

	//検索
	bool ret = and_sw;
	for (int i=0; i<wlst->Count; i++) {
		int p = case_sw? s.Pos(wlst->Strings[i]) : pos_i(wlst->Strings[i], s);
		if (!not_sw) {
			//含む
			if (p>0) {
				//いずれかを
				if (!and_sw) { ret = true; break; }
			}
			else {
				//全てを
				if (and_sw) { ret = false; break; }
			}
		}
		else {
			//含まない
			if (p==0) {
				//いずれかを
				if (!and_sw) { ret = true; break; }
			}
			else {
				//全てを
				if (and_sw) { ret = false; break; }
			}
		}
	}

	return ret;
}

//---------------------------------------------------------------------------
//複数の文字列を検索し、マッチした語のリストを取得する
//---------------------------------------------------------------------------
bool find_mlt_str(
	UnicodeString wd,	//検索語 (複数の場合は半角または全角空白で区切る）
	UnicodeString s,	//対象文字列
	TStringList *lst,	//[o] 実際にマッチした語のリスト
	bool case_sw)		//大小文字を区別	(default = false)
{
	lst->Clear();
	if (wd.IsEmpty()) return false;

	//検索語リスト作成
	std::unique_ptr<TStringList> wlst(new TStringList());
	get_find_wd_list(wd, wlst.get());

	//検索
	for (int i=0; i<wlst->Count; i++) {
		UnicodeString wd = wlst->Strings[i];
		int p = case_sw? s.Pos(wd) : pos_i(wd, s);
		if (p>0) {
			UnicodeString fwd = s.SubString(p, wd.Length());
			if (lst->IndexOf(fwd)==-1) lst->Add(fwd);
		}
	}

	return (lst->Count>0);
}

//---------------------------------------------------------------------------
//行数を取得
//---------------------------------------------------------------------------
int get_line_count(UnicodeString s)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = s;
	return lst->Count;
}

//---------------------------------------------------------------------------
//TStringDynArray に文字列を追加
//---------------------------------------------------------------------------
void add_dyn_array(TStringDynArray &lst, UnicodeString s,
	bool no_dupl)	//既存の場合は追加しない	(default = false)
{
	if (no_dupl) {
		for (int i=0; i<lst.Length; i++) if (SameText(s, lst[i])) return;
	}

	int len    = lst.Length;
	lst.Length = len + 1;
	lst[len]   = s;
}

//---------------------------------------------------------------------------
//CSV項目内容取得
//---------------------------------------------------------------------------
TStringDynArray get_csv_array(UnicodeString src, int size,
	bool force_size)	//サイズを強制的に設定  (default = false)
{
	TStringDynArray ret_array;
	ret_array.Length = size;
	std::unique_ptr<TStringList> rec(new TStringList());
	rec->Delimiter		 = ',';
	rec->QuoteChar		 = '\"';
	rec->StrictDelimiter = true;
	rec->DelimitedText	 = src;
	for (int i=0; i<rec->Count && i<size; i++) ret_array[i] = rec->Strings[i];
	if (!force_size) ret_array.Length = rec->Count;
	return ret_array;
}
//---------------------------------------------------------------------------
UnicodeString get_csv_item(UnicodeString src, int idx)
{
	std::unique_ptr<TStringList> rec(new TStringList());
	rec->Delimiter		 = ',';
	rec->QuoteChar		 = '\"';
	rec->StrictDelimiter = true;
	rec->DelimitedText	 = src;
	return (idx>=0 && idx<rec->Count)? rec->Strings[idx] : EmptyStr;
}

//---------------------------------------------------------------------------
UnicodeString get_tsv_item(UnicodeString src, int idx)
{
	TStringDynArray rec = SplitString(src, "\t");
	return (idx>=0 && idx<rec.Length)? rec[idx] : EmptyStr;
}

//---------------------------------------------------------------------------
//CSV項目を作成
//---------------------------------------------------------------------------
UnicodeString make_csv_str(UnicodeString s)
{
	return UnicodeString().sprintf(_T("\"%s\""), REPLACE_TS(s, "\"", "\"\"").c_str());
}
//---------------------------------------------------------------------------
UnicodeString make_csv_str(bool sw)
{
	return UnicodeString().sprintf(_T("\"%u\""), sw? 1 : 0);
}

//---------------------------------------------------------------------------
UnicodeString make_csv_rec_str(TStringDynArray lst)
{
	UnicodeString lbuf;
	for (int i=0; i<lst.Length; i++) {
		if (i>0) lbuf.UCAT_T(",");
		lbuf += make_csv_str(lst[i]);
	}
	return lbuf;
}

//---------------------------------------------------------------------------
//CSVのリストから p 番目の項目が s のインデックスを取得
//---------------------------------------------------------------------------
int indexof_csv_list(TStringList *lst, UnicodeString s, int p)
{
	int idx = -1;
	for (int i=0; i<lst->Count && idx==-1; i++)
		if (SameText(s, get_csv_item(lst->Strings[i], p))) idx = i;
	return idx;
}
//---------------------------------------------------------------------------
//CSVのリストから p 番目の項目が s のレコードを取得
//---------------------------------------------------------------------------
TStringDynArray record_of_csv_list(TStringList *lst, UnicodeString s, int p, int size)
{
	TStringDynArray ret_array;
	if (lst) {
		int idx = indexof_csv_list(lst, s, p);
		if (idx!=-1) ret_array = get_csv_array(lst->Strings[idx], size, true);
	}
	return ret_array;
}

//---------------------------------------------------------------------------
//エスケープシーケンス文字の変換
//---------------------------------------------------------------------------
UnicodeString conv_esc_char(UnicodeString s)
{
	if (s.IsEmpty()) return EmptyStr;

	s = REPLACE_TS(s, "\\\\", "\f");	//\\ を一時的に \f に変えておいて
	s = REPLACE_TS(s, "\\t",  "\t");	//タブ
	s = REPLACE_TS(s, "\\n",  "\r\n");	//改行
	s = REPLACE_TS(s, "\\s",  " ");		//NyanFi 固有
	s = REPLACE_TS(s, "\f",   "\\");	//\\(\f) を \ に変換
	return s;
}

//---------------------------------------------------------------------------
//置換 \\ → /
//---------------------------------------------------------------------------
UnicodeString yen_to_slash(UnicodeString s)
{
	return REPLACE_TS(s, "\\", "/");
}
//---------------------------------------------------------------------------
//置換 / → \\
//---------------------------------------------------------------------------
UnicodeString slash_to_yen(UnicodeString s)
{
	return REPLACE_TS(s, "/", "\\");
}

//---------------------------------------------------------------------------
//SHA-1 を7桁の短縮形に変換
//---------------------------------------------------------------------------
UnicodeString sha1_to_short(UnicodeString s)
{
	return replace_regex(s, _T("\\b([0-9a-f]{7})[0-9a-f]{33}\\b"), _T("\\1"));
}

//---------------------------------------------------------------------------
//文字列を大小文字を区別せずに比較
//---------------------------------------------------------------------------
bool same_ut_i(UnicodeString s, const _TCHAR *t)
{
	return (_tcsicmp(s.c_str(), t)==0);
}
//---------------------------------------------------------------------------
//文字列を大小文字を区別してに比較
//---------------------------------------------------------------------------
bool same_ut_s(UnicodeString s, const _TCHAR *t)
{
	return (_tcscmp(s.c_str(), t)==0);
}

//---------------------------------------------------------------------------
//文字列判定
//---------------------------------------------------------------------------
bool equal_1(UnicodeString s)
{
	return (_tcscmp(s.c_str(), _T("1"))==0);
}
//---------------------------------------------------------------------------
bool equal_0(UnicodeString s)
{
	return (_tcscmp(s.c_str(), _T("0"))==0);
}
//---------------------------------------------------------------------------
bool equal_ENTER(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("ENTER"))==0);
}
//---------------------------------------------------------------------------
bool equal_ESC(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("ESC"))==0);
}
//---------------------------------------------------------------------------
bool equal_TAB(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("TAB"))==0);
}
//---------------------------------------------------------------------------
bool equal_DEL(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("DEL"))==0);
}
//---------------------------------------------------------------------------
bool equal_LEFT(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("LEFT"))==0);
}
//---------------------------------------------------------------------------
bool equal_RIGHT(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("RIGHT"))==0);
}
//---------------------------------------------------------------------------
bool equal_UP(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("UP"))==0);
}
//---------------------------------------------------------------------------
bool equal_DOWN(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("DOWN"))==0);
}
//---------------------------------------------------------------------------
bool equal_F1(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("F1"))==0);
}

//---------------------------------------------------------------------------
//セパレータ(=="-") ?
//---------------------------------------------------------------------------
bool is_separator(UnicodeString s)
{
	return (_tcscmp(s.c_str(), _T("-"))==0);
}

//---------------------------------------------------------------------------
bool starts_tchs(const _TCHAR *lst, UnicodeString s)
{
	return s.IsDelimiter(lst, 1);
}
//---------------------------------------------------------------------------
bool ends_tchs(const _TCHAR *lst, UnicodeString s)
{
	return s.IsDelimiter(lst, s.Length());
}

//---------------------------------------------------------------------------
// @ で始まるか ?
//---------------------------------------------------------------------------
bool starts_AT(UnicodeString s)
{
	return StartsStr('@', s);
}
//---------------------------------------------------------------------------
// $ で始まるか ?
//---------------------------------------------------------------------------
bool starts_Dollar(UnicodeString s)
{
	return StartsStr('$', s);
}

//---------------------------------------------------------------------------
// \ で終わる ?
//---------------------------------------------------------------------------
bool ends_PathDlmtr(UnicodeString s)
{
	return EndsStr('\\', s);
}
//---------------------------------------------------------------------------
// \ を含む ?
//---------------------------------------------------------------------------
bool contains_PathDlmtr(UnicodeString s)
{
	return contains_s(s, '\\');
}
//---------------------------------------------------------------------------
// / を含む ?
//---------------------------------------------------------------------------
bool contains_Slash(UnicodeString s)
{
	return contains_s(s, '/');
}

//---------------------------------------------------------------------------
// /～/ (正規表現) ?
//---------------------------------------------------------------------------
bool is_regex_slash(UnicodeString s)
{
	return (s.Length()>=2 && StartsStr('/', s) && EndsStr('/', s));
}

//---------------------------------------------------------------------------
//引用符で囲まれているか?
//---------------------------------------------------------------------------
bool is_quot(UnicodeString s)
{
	return (s.Length()>=2 && ((StartsStr('\"', s) && EndsStr('\"', s)) || ((StartsStr('\'', s) && EndsStr('\'', s)))));
}
//---------------------------------------------------------------------------
//空白を含んでいる場合、引用符で囲む
//---------------------------------------------------------------------------
UnicodeString add_quot_if_spc(UnicodeString s)
{
	if (!is_quot(s) && (ContainsStr(s, " ") || ContainsStr(s, "　"))) s = "\"" + s + "\"";
	return s;
}

//---------------------------------------------------------------------------
//引用符を外す
//---------------------------------------------------------------------------
UnicodeString exclude_quot(UnicodeString s)
{
	if (is_quot(s)) s = exclude_top_end(s);
	return s;
}

//---------------------------------------------------------------------------
//TStringList Vlues[name] の整数値を取得
//---------------------------------------------------------------------------
int get_ListIntVal(TStringList *lst, const _TCHAR *name, int def)
{
	return lst->Values[name].ToIntDef(def);
}
//---------------------------------------------------------------------------
//TStringList Vlues[name] が空か?
//---------------------------------------------------------------------------
bool ListVal_is_empty(TStringList *lst, const _TCHAR *name)
{
	return lst->Values[name].IsEmpty();
}
//---------------------------------------------------------------------------
//TStringList Vlues[name]=="1"?
//---------------------------------------------------------------------------
bool ListVal_equal_1(TStringList *lst, const _TCHAR *name)
{
	return (_tcscmp(lst->Values[name].c_str(), _T("1"))==0);
}

//---------------------------------------------------------------------------
//m秒から時間文字列へ
//---------------------------------------------------------------------------
UnicodeString __fastcall mSecToTStr(
	unsigned int ms,
	bool cs)			//1/100秒を表示 (default = true)
{
	int scnt = ms/1000;	//秒
	int c = (int)(ms/10.0 + 0.5) % 100;
	int s = scnt % 60;	scnt /= 60;
	int m = scnt % 60;
	int h = scnt / 60;

	UnicodeString ret_str;
	if (cs)
		ret_str.sprintf(_T("%02u:%02u:%02u.%02u"), h, m, s, c);
	else {
		if (c>50) s++;
		ret_str.sprintf(_T("%02u:%02u:%02u"), h, m, s);
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//パラメータをm秒に変換
//---------------------------------------------------------------------------
int param_to_mSec(UnicodeString prm)
{
	int k;
	if		(remove_end_text(prm, "S")) k = 1000;
	else if (remove_end_text(prm, "M")) k = (60*1000);
	else if (remove_end_text(prm, "H")) k = (60*60*1000);
	else								k = 1;
	return prm.ToIntDef(0) * k;
}

//---------------------------------------------------------------------------
//取得幅と表示幅が一致しない例外フォント(Ricty Diminished ～)か?
//---------------------------------------------------------------------------
bool IsIrregularFont(TFont *fnt)
{
	if (!StartsStr("Ricty Diminished", fnt->Name)) return false;

	switch (fnt->Size) {
	case 11: case 13: case 14: case 16: case 17: case 19: case 20:
	case 32: case 34: case 35: case 37: case 38: case 40: case 41:
	case 53: case 55: case 56: case 58: case 59: case 61: case 62:
		// ((fsz>=11 && fsz<=20) || (fsz>=32 && fsz<=41) || (fsz>=53 && fsz<=62)) && Size*96%72!=0
		return true;
	default:
		return false;
	}
}

//---------------------------------------------------------------------------
//文字列の表示幅を取得 (例外フォントに対応)
//※Ricty Diminished は、半角文字の実際の表示幅が取得値+1になる場合がある
//---------------------------------------------------------------------------
int get_TextWidth(
	TCanvas *cv,
	UnicodeString s,	//文字列
	bool is_irreg)		//true = 例外フォント(Ricty Diminished ～)
{
	if (is_irreg) {
		int wd = 0;
		for (int i=1; i<=s.Length(); i++) {
			wd += cv->TextWidth(s[i]);
			if (str_len_half(s[i])==1) wd++;
		}
		return wd;
	}
	else
		return cv->TextWidth(s);
}

//---------------------------------------------------------------------------
//指定文字数の表示幅を取得
//---------------------------------------------------------------------------
int get_CharWidth(
	TCanvas *cv,
	int n,			//文字数	(半角単位)
	int mgn)		//余裕		(default = 0);
{
	TEXTMETRIC tm;
	::GetTextMetrics(cv->Handle , &tm);
	return (tm.tmAveCharWidth * n + mgn);
}

//---------------------------------------------------------------------------
//TPanel 内での文字列幅を取得
//---------------------------------------------------------------------------
int get_WidthInPanel(
	UnicodeString s,
	TPanel *pp,
	bool d2d_sw)	//Direct2D で取得	(default = false)
{
	int wd = 0;
	HDC hDc = ::GetDC(pp->Handle);
	if (hDc) {
		if (d2d_sw && TDirect2DCanvas::Supported()) {
			std::unique_ptr<TDirect2DCanvas> dcv(new TDirect2DCanvas(hDc, pp->ClientRect));
			dcv->Font->Assign(pp->Font);
			wd = dcv->TextWidth(s);
		}
		else {
			std::unique_ptr<TCanvas> cv(new TCanvas());
			cv->Handle = hDc;
			cv->Font->Assign(pp->Font);
			wd = cv->TextWidth(s);
		}
		::ReleaseDC(pp->Handle, hDc);
	}
	return wd;
}

//---------------------------------------------------------------------------
// 文字列が TPanel の幅いっぱいに収まるように、指定位置を空白を挿入
//---------------------------------------------------------------------------
UnicodeString fit_StrToPanel(UnicodeString s, int p, TPanel *pp)
{
	HDC hDc = ::GetDC(pp->Handle);
	if (hDc) {
		std::unique_ptr<TCanvas> cv(new TCanvas());
		cv->Handle = hDc;
		cv->Font->Assign(pp->Font);
		while (cv->TextWidth(UAPP_T(s, "  "))<pp->ClientWidth) s.Insert(" ", p);
		::ReleaseDC(pp->Handle, hDc);
	}
	return s;
}

//---------------------------------------------------------------------------
//半角換算の文字列長を取得
//---------------------------------------------------------------------------
int str_len_half(UnicodeString s)
{
	AnsiString    s_a = s;
	UnicodeString s_u = s_a;
	if (s!=s_u && s.Length()==s_u.Length()) {
		UnicodeString r_u;
		for (int i=1; i<=s.Length(); i++) {
			if (!s.IsTrailSurrogate(i)) {
				if (s[i]!=s_u[i] && s_u[i]=='?')
					r_u.UCAT_T("？");
				else
					r_u += s_u[i];
			}
		}
		s_a = r_u;
	}
	return s_a.Length();
}

//---------------------------------------------------------------------------
//サロゲートペアを考慮して文字長を取得
//---------------------------------------------------------------------------
int str_len_unicode(UnicodeString s)
{
	int n = 0;
	for (int i=1; i<=s.Length(); i++) if (!s.IsTrailSurrogate(i)) n++;
	return n;
}

//---------------------------------------------------------------------------
//半角換算の最大幅を更新
//---------------------------------------------------------------------------
void max_len_half(
	int &w,				//[o] 最大幅
	UnicodeString s)
{
	w = std::max(w, str_len_half(s));
}

//---------------------------------------------------------------------------
//指定幅(半角単位)内で文字列が右詰になるように空白を挿入
//---------------------------------------------------------------------------
UnicodeString align_r_str(UnicodeString s, int wd, UnicodeString post_s)
{
	int n = str_len_half(s);
	if (n<wd) s.Insert(StringOfChar(_T(' '), wd - n), 1);
	return s + post_s;
}
//---------------------------------------------------------------------------
//指定幅(半角単位)内で文字列が左詰になるように空白を挿入
//---------------------------------------------------------------------------
UnicodeString align_l_str(UnicodeString s, int wd, UnicodeString post_s)
{
	int n = str_len_half(s);
	if (n<wd) s += StringOfChar(_T(' '), wd - n);
	return s + post_s;
}

//---------------------------------------------------------------------------
//文字列が指定幅に収まるように調整
//---------------------------------------------------------------------------
UnicodeString minimize_str(
	UnicodeString s,	//対象文字列
	TCanvas *cv,
	int  wd,			//制限幅
	bool omit_end)		//末尾を省略 (default = false : 中間を省略)
{
	bool is_irreg = IsIrregularFont(cv->Font);

	int ww = get_TextWidth(cv, s, is_irreg);
	if (ww > wd) {
		if (omit_end) {
			s.UCAT_T("…");
			int p = s.Length() - 1;
			while (p>0) {
				s.Delete(p--, 1);
				if (get_TextWidth(cv, s, is_irreg) <= wd) break;
			}
		}
		else {
			int p = 0;
			for (int i=1; i<s.Length(); i++) {
				if (get_TextWidth(cv, s.SubString(1, i), is_irreg) > wd/2) {
					p = i - 1; break;
				}
			}
			if (p==0) p = 1;
			s.Insert("…", p);	p += 1;
			while (s.Length()>=p) {
				s.Delete(p, 1);
				if (get_TextWidth(cv, s, is_irreg) <= wd) break;
			}
		}
	}
	return s;
}

//---------------------------------------------------------------------------
UnicodeString fit_str(
	UnicodeString s,	//対象文字列
	TCanvas *cv,
	int  wd,			//制限幅
	bool omit_end)		//末尾を省略 (default = false : 中間を省略)
{
	if (cv->TextWidth(s) > wd)
		s = minimize_str(s, cv, wd, omit_end);
	else {
		for (;;) {
			if (cv->TextWidth(s + " ")>wd) break;
			s.UCAT_T(" ");
		}
	}
	return s;
}

//---------------------------------------------------------------------------
//全角/半角変換
//---------------------------------------------------------------------------
UnicodeString to_Full_or_Half(UnicodeString s, bool to_w)
{
	const int size_s = s.Length() + 1;
	int size_d = ::LCMapString(::GetUserDefaultLCID(), (to_w? LCMAP_FULLWIDTH : LCMAP_HALFWIDTH), s.c_str(), size_s, NULL, 0);
	std::unique_ptr<_TCHAR[]> sbuf(new _TCHAR[size_d]);
	::ZeroMemory(sbuf.get(), size_d * sizeof(_TCHAR));
	::LCMapString(::GetUserDefaultLCID(), (to_w? LCMAP_FULLWIDTH : LCMAP_HALFWIDTH), s.c_str(), size_s, sbuf.get(), size_d);
	return UnicodeString(sbuf.get());
}
//---------------------------------------------------------------------------
//全角に変換
//---------------------------------------------------------------------------
UnicodeString to_FullWidth(UnicodeString s)
{
	return to_Full_or_Half(s, true);
}
//---------------------------------------------------------------------------
//半角に変換
//---------------------------------------------------------------------------
UnicodeString to_HalfWidth(UnicodeString s)
{
	return to_Full_or_Half(s, false);
}

//---------------------------------------------------------------------------
//罫線行か？
//---------------------------------------------------------------------------
int is_RuledLine(UnicodeString s)
{
	return SameStr(s, StringOfChar(_T('─'), s.Length())) ? 1 :
			(SameStr(s, StringOfChar(_T('━'), s.Length()))? 2 : 0);
}

//---------------------------------------------------------------------------
//罫線行文字列を取得 (---- -------- ...)
//---------------------------------------------------------------------------
UnicodeString make_RuledLine(int cnt, ...)
{
	UnicodeString ret_str;

	va_list ap;
	va_start(ap, cnt);
	for (int i=0; i<cnt; i++) {
		int w = va_arg(ap, int);
		if (w>0) {
			if (!ret_str.IsEmpty()) ret_str.UCAT_T(" ");
			ret_str += StringOfChar(_T('-'), w);
		}
	}
	va_end(ap);

	return ret_str;
}

//---------------------------------------------------------------------------
//アドレス文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_AddrStr(__int64 adr,
	int w)		//表示桁 (default = 0)
{
	UnicodeString ret_str;
	if (adr>=0) {
		unsigned int adr_l = adr & 0xffffffffLL;
		int adr_h = adr >> 32;
		ret_str.sprintf(_T("%08X"), adr_l);
		ret_str.Insert(":", 5);
		if (adr_h>0) ret_str = IntToHex(adr_h, 1) + ":" + ret_str;
		ret_str = align_r_str(ret_str, w);
	}
	else
		ret_str.USET_T("____:____");
	return ret_str;
}

//---------------------------------------------------------------------------
//アスペクト比文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_AspectStr(int w, int h)
{
	if (w==0 || h==0) return EmptyStr;

	int w1 = w;
	int h1 = h;
	if (h1>w1) std::swap(w1, h1);
	for (;;) {
		int r = w1%h1;
		if (r==0) break;
		w1 = h1; h1 = r;
	}
	w /= h1;
	h /= h1;

	UnicodeString ret_str;
	if (w<20)
		ret_str.cat_sprintf(_T("%u : %u"), w, h);
	else {
		if (w>h)
			ret_str.cat_sprintf(_T("%5.4g : 1"), 1.0 * w/h);
		else
			ret_str.cat_sprintf(_T("1 : %5.4g"), 1.0 * h/w);
	}
	return Trim(ret_str);
}

//---------------------------------------------------------------------------
//文字セット名を取得
//---------------------------------------------------------------------------
UnicodeString get_NameOfCharSet(int charset)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text =
		"ANSI=0\n"
		"DEFAULT=1\n"
		"SYMBOL=2\n"
		"MAC=77\n"
		"SHIFTJIS=128\n"
		"HANGUL=129\n"
		"JOHAB=130\n"
		"GB2312=134\n"
		"CHINESEBIG5=136\n"
		"GREEK=161\n"
		"TURKISH=162\n"
		"VIETNAMESE=163\n"
		"HEBREW=177\n"
		"ARABIC=178\n"
		"BALTIC=186\n"
		"RUSSIAN=204\n"
		"THAI=222\n"
		"EASTEUROPE=238\n"
		"OEM=255\n";

	UnicodeString ret_str;
	for (int i=0; i<lst->Count; i++) {
		if (lst->ValueFromIndex[i].ToIntDef(0)==charset) {
			ret_str.sprintf(_T("%s (%u)"), lst->Names[i].c_str(), charset);
			break;
		}
	}
	if (ret_str.IsEmpty()) ret_str.sprintf(_T("??? (%u)"), charset);
	return ret_str;
}
//---------------------------------------------------------------------------
//文字のウエイト名を取得
//---------------------------------------------------------------------------
UnicodeString get_NameOfWeight(int w)
{
	UnicodeString ret_str =
		(w==0)?   "DONTCARE (0)" :
		(w==100)? "THIN (100)" :
		(w==200)? "EXTRALIGHT (200)" :
		(w==300)? "LIGHT (300)" :
		(w==400)? "NORMAL (400)" :
		(w==500)? "MEDIUM (500)" :
		(w==600)? "SEMIBOLD (600)" :
		(w==700)? "BOLD (700)" :
		(w==800)? "EXTRABOLD (800)" :
		(w==900)? "HEAVY (900" : "";

	if (ret_str.IsEmpty()) ret_str.sprintf(_T("??? (%u)"), w);
	return ret_str;
}

//---------------------------------------------------------------------------
//コードページリスト
//---------------------------------------------------------------------------
const UnicodeString CodePageList =
	"Shift_JIS=932\n"
	"ISO-8859-1=1252\n"
	"UTF-16=1200\n"
	"UTF-16(BE)=1201\n"		//UTF-16 (Big Endian)
	"US-ASCII=20127\n"
	"EUC-JP=20932\n"
	"ISO-2022-JP=50220\n"
	"UTF-7=65000\n"
	"UTF-8=65001\n";

const UnicodeString CodePageListX =
	"Shift_JIS=932\n"
	"Shift-JIS=932\n"
	"X-SJIS=932\n"
	"ISO-8859-1=1252\n"
	"UTF-16=1200\n"
	"UTF-16(BE)=1201\n"
	"US-ASCII=20127\n"
	"EUC-JP=20932\n"
	"X-EUC-JP=20932\n"
	"ISO-2022-JP=50220\n"
	"UTF-7=65000\n"
	"UTF-8=65001\n";

//---------------------------------------------------------------------------
//コードページの名前を取得
//---------------------------------------------------------------------------
UnicodeString get_NameOfCodePage(int code_page, bool long_sw, bool has_bom)
{
	UnicodeString ret_str;
	if (code_page>0) {
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Text = CodePageList;
		for (int i=0; i<lst->Count; i++) {
			if (lst->ValueFromIndex[i].ToIntDef(0)==code_page) {
				ret_str = lst->Names[i];
				if (long_sw) ret_str = ReplaceText(ret_str, "(BE)", " (Big Endian)");
				break;
			}
		}

		if (ret_str.IsEmpty()) ret_str.sprintf(_T("%u"), code_page);
		if (code_page==1200 || code_page==1201 || code_page==65001) {
			if (long_sw)
				ret_str += (has_bom? " BOM付き" : " BOM無し");
			else
				ret_str += (has_bom? " BOM付" : " BOM無");
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//名前のコードページを取得
//---------------------------------------------------------------------------
int get_CodePageOfName(UnicodeString code_name)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = CodePageListX;
	return lst->Values[code_name].ToIntDef(0);
}

//---------------------------------------------------------------------------
//UTF8チェック
//---------------------------------------------------------------------------
int check_UTF8(BYTE *bp, int size)
{
	int cnt = 0;
	try {
		int i = 0;
		while (i<size) {
			if (bp[i]<0x80)
				i++;
			else {
				int bn;
				if		(0xc2<=bp[i] && bp[i]<=0xdf) bn = 1;
				else if (0xe0<=bp[i] && bp[i]<=0xef) bn = 2;
				else if (0xf0<=bp[i] && bp[i]<=0xf4) bn = 3;
				else Abort();
				i++;	if (i==size) break;
				for (int j=0; j<bn && i<size; j++) {
					if (0x80<=bp[i] && bp[i]<=0xbf) {
						i++;	if (i==size) break;
					}
					else Abort();
				}
				cnt++;
			}
		}
	}
	catch (...) {
		cnt = 0;
	}
	return cnt;
}

//---------------------------------------------------------------------------
//メモリストリームのコードページを取得
//  戻り値: コードページ (0:不明/ -1:バイナリ)
//---------------------------------------------------------------------------
int get_MemoryCodePage(
	TMemoryStream *ms,
	bool *has_bom)		//[o] BOM有り	(default = NULL)
{
	int code_page = 0;
	if (has_bom) *has_bom = false;

	int src_size = ms->Size;
	if (src_size==0) return 932;

	//予備判定
	BYTE *bp = (BYTE*)ms->Memory;
	int x00_cnt	= 0;	//0x00 の個数
	int z_rpt	= 0;	//0x00 の連続数
	int z_max	= 0;	//0x00 の最大連続数
	int esc_cnt = 0;	//ESC の個数
	int asc_cnt = 0;	//ASCII の個数
	int ltn_cnt = 0;	//ISO8859(Latin-1を想定) の個数
	int b7_cnt	= 0;	//b7==1 の個数

	for (int i=0; i<src_size; i++) {
		BYTE b0 = bp[i];
		if (b0==0x00) {
			x00_cnt++; z_rpt++;
		}
		else {
			if (z_rpt>z_max) z_max = z_rpt;
			z_rpt = 0;
			if		(b0==0x1b) 			 esc_cnt++;
			else if (0x06<b0 && b0<0x7f) asc_cnt++;
			else if (0xa0<=b0)			 ltn_cnt++;
			if (b0 & 0x80)				 b7_cnt++;
		}
	}

	if (z_max>3 || (src_size<3 && x00_cnt>0)) return -1;	//バイナリー

	//BOMの有無をチェック
	if		(bp[0]==0xfe && bp[1]==0xff)				code_page = 1201;		//UTF-16BE BOM付
	else if (bp[0]==0xff && bp[1]==0xfe)				code_page = 1200;		//UTF-16 (LE) BOM付
	else if (bp[0]==0xef && bp[1]==0xbb && bp[2]==0xbf)	code_page = CP_UTF8;	//UTF-8 BOM付

	if (code_page>0) {
		if (has_bom) *has_bom = true;
		return code_page;
	}

	//BOM無し
	if (check_UTF8(bp, src_size)>0) code_page = CP_UTF8;	//UTF-8 BOM無し
	if (code_page>0) return code_page;

	if (x00_cnt==0) {
		if (esc_cnt>0) {
			//ISO-2022-JP?
			try {
				int jis_cnt = 0;
				int flag = 0;
				for (int i=0; i<src_size-1; i++) {
					BYTE b = bp[i];
					switch (flag) {
					case 0:
						if (b==0x1b) flag = 1;
						else if ((b>=0x80 && b<=0x8d) || (b>=0x90 && b<=0xa0) || b==0xfe) Abort();
						break;
					case 1:	//ESC
						flag = (b=='$')? 2 : (b=='(')? 5 : 0;
						break;
					case 2:	//ESC$
						flag = (b=='B' || b=='@')? 3 : 0;
						break;
					case 3:	//ESC$B/@
						if (b==0x1b) flag = 1; else jis_cnt++;
						break;
					case 5:	//ESC(
						flag = (b=='B' || b=='J')? 6 : 0;
						break;
					case 6:	//ESC(B/J
						if (b==0x1b) flag = 1; else jis_cnt++;
						break;
					}
				}
				if (jis_cnt > 0) code_page = 50220;
			}
			catch (...) {
				;
			}
		}
		else {
			//ShiftJIS?
			int sj_cnt	= 0;
			for (int i=0; i<src_size-1; i++) {
				BYTE b0 = bp[i];
				BYTE b1 = bp[i + 1];
				if (((0x81<=b0 && b0<=0x9f) || (0xe0<=b0 && b0<=0xfc))
					&& ((0x40<=b1 && b1<=0x7e) || (0x80<=b1 && b1<=0xfc)))
				{
					sj_cnt += 2;  i++;
				}
			}
			//EUC-JP?
			int euc_cnt = 0;
			for (int i=0; i<src_size-1; i++) {
				BYTE b0 = bp[i];
				BYTE b1 = bp[i + 1];
				if (((0xa1<=b0 && b0<=0xfe) && (0xa1<=b1 && b1<=0xfe))
					|| (b0==0x8e && (0xa1<=b1 && b1<=0xdf)))
						{ euc_cnt += 2;  i++; }
				else if (i < src_size-2) {
					BYTE b2 = bp[i + 2];
					if (b0==0x8f && (0xa1<=b1 && b1<=0xfe) && (0xa1<=b2 && b2<=0xfe))
						{ euc_cnt += 3;  i += 2; }
				}
			}
			//UTF-7?
			int utf7_cnt = 0;
			if (b7_cnt==0) {
				int  b64cnt = 0;
				bool is_b64 = false;
				for (int i=0; i<src_size; i++) {
					BYTE b0 = bp[i];
					if (!is_b64) {
						if (b0=='+') {
							b64cnt = 0; is_b64 = true;
						}
					}
					else {
						if (b0=='-') {
							utf7_cnt += b64cnt;
							is_b64 = false;
						}
						else if (!isalnum(b0) && b0!='+' && b0!='/') {
							utf7_cnt = 0; break;
						}
						else b64cnt++;
					}
				}
			}
			if ((euc_cnt + sj_cnt + asc_cnt) > src_size/2) {
				if		(euc_cnt>sj_cnt)	code_page = 20932;
				else if (utf7_cnt>sj_cnt)	code_page = 65000;
				else if (sj_cnt>0)			code_page = 932;
				else if (ltn_cnt>0)			code_page = 1252;
				else if (asc_cnt==src_size)	code_page = 20127;
			}
		}
		if (code_page>0) return code_page;
	}

	//UTF-16か?
	int be_asc = 1, le_asc = 1;
	int be_hk  = 1, le_hk  = 1;		//ひらがな、カタカナ
	src_size -= (src_size%2);
	for (int i=0; i<src_size-1; i+=2) {
		BYTE b0 = bp[i];
		BYTE b1 = bp[i + 1];
		if		(b0==0x00 && 0x1f<b1 && b1<0x7f) be_asc++;
		else if (b1==0x00 && 0x1f<b0 && b0<0x7f) le_asc++;
		else if (b0==0x30 && 0x00<b1 && b1<0xf7) be_hk++;
		else if (b1==0x30 && 0x00<b0 && b0<0xf7) le_hk++;
	}

	int be_point = 0, le_point = 0;
	if		(be_asc/le_asc>10) be_point++;
	else if (le_asc/be_asc>10) le_point++;

	if		(be_hk/le_hk>10)   be_point++;
	else if (le_hk/be_hk>10)   le_point++;

	if		(be_point>le_point) code_page = 1201;	//BE
	else if	(le_point>be_point) code_page = 1200;	//LE

	return code_page;
}

//---------------------------------------------------------------------------
//メモリストリームのコードページを判定し、文字列として取得
//---------------------------------------------------------------------------
UnicodeString get_MemoryStrins(TMemoryStream *ms)
{
	//コードページ取得
	ms->Seek(0, soFromBeginning);
	int code_page = get_MemoryCodePage(ms);
	if (code_page<0)  return EmptyStr;
	if (code_page==0) code_page = 932;

	//バッファに読み込み
	TBytes Bytes;
	Bytes.Length = ms->Size;
	ms->Seek(0, soFromBeginning);
	ms->Read(Bytes, ms->Size);

	//エンコード
	std::unique_ptr<TStringList> lst(new TStringList());
	std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
	return enc->GetString(Bytes, 0, Bytes.Length);
}

//---------------------------------------------------------------------------
//サロゲートペア文字をチェック
//戻り値: 見つかった文字を空白区切りで連結して返す。なければ EmptyStr
//---------------------------------------------------------------------------
UnicodeString check_Surrogates(UnicodeString s)
{
	UnicodeString ret_str;
	int s_len = s.Length();
	int i = 1;
	while(i<=s_len) {
		if (s.IsLeadSurrogate(i)) {
			if (!ret_str.IsEmpty()) ret_str.UCAT_T(" ");
			ret_str.cat_sprintf(_T("%c"), s[i++]);
			if (i<=s_len && s.IsTrailSurrogate(i)) ret_str.cat_sprintf(_T("%c"), s[i++]);
		}
		else i++;
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//環境依存文字(ローマ数字、囲み文字、単位記号、合成文字など)をチェック
//戻り値: 見つかった文字を空白区切りで連結して返す。なければ EmptyStr
//---------------------------------------------------------------------------
UnicodeString check_EnvDepandChars(UnicodeString s)
{
	int tbl[] = {0x2116, 0x2121,							//№ ℡
				 0x2211, 0x221a, 0x221f,					//∑ √ ∟
				 0x2220, 0x2229, 0x222a, 0x222b, 0x222e,	//∠ ∩ ∪ ∫ ∮
				 0x2235, 0x2252, 0x2261, 0x22a5, 0x22bf};	//∵ ≒ ≡ ⊥ ⊿
	int n = sizeof(tbl)/sizeof(tbl[0]);

	UnicodeString ret_str;
	for (int i=1; i<=s.Length(); i++) {
		int c = (unsigned int)s[i];
		bool found = false;
		for (int j=0; j<n && !found; j++) found = (tbl[j]==c);
		if (found ||
			(c>=0x2150 && c<=0x218f) || (c>=0x2194 && c<=0x219f) || (c>=0x2460 && c<=0x24ef) ||
			(c>=0x2600 && c<=0x266f) || (c>=0x3220 && c<=0x324f) || (c>=0x3280 && c<=0x33ff))
		{
			if (!ret_str.IsEmpty()) ret_str.UCAT_T(" ");
			ret_str.cat_sprintf(_T("%c"), s[i]);
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//標準化されたレーベンシュタイン距離を取得
//  最大文字数=256
//戻り値: 0～1000
//---------------------------------------------------------------------------
int get_NrmLevenshteinDistance(UnicodeString s1, UnicodeString s2,
	bool ig_case,	//大小文字を無視	(default = false)
	bool ig_num,	//数字部分を無視	(default = false)
	bool ig_fh)		//全角/半角を無視	(default = false)
{
	if (s1.IsEmpty() || s2.IsEmpty()) return SameStr(s1, s2)? 0 : 1000;

	if (ig_case) {
		s1 = s1.UpperCase();
		s2 = s2.UpperCase();
	}

	if (ig_num) {
		s1 = replace_regex(s1, _T("\\d"), null_TCHAR);
		s2 = replace_regex(s2, _T("\\d"), null_TCHAR);
	}

	if (ig_fh) {
		s1 = to_FullWidth(s1);
		s2 = to_FullWidth(s2);
	}

	int len1 = std::min(s1.Length(), 256);
	int len2 = std::min(s2.Length(), 256);

	std::vector< std::vector<int> > d(len1 + 1, std::vector<int>(len2 + 1));

	for (int i=0; i<=len1; i++) d[i][0] = i;
	for (int i=0; i<=len2; i++) d[0][i] = i;

	for (int i=1; i<=len1; i++) {
		for (int j=1; j<=len2; j++) {
			int cost = (s1[i]==s2[j])? 0 : 1;
			int a = d[i - 1][j] + 1;
			int b = d[i][j - 1] + 1;
			int c = d[i - 1][j - 1] + cost;
			d[i][j] = (a>b)? std::min(b, c) : std::min(a, c);
		}
	}

	return (int)(1000.0 * d[len1][len2] / (std::max(len1, len2)));
}
//---------------------------------------------------------------------------
