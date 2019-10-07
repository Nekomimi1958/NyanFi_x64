//----------------------------------------------------------------------//
// �����񑀍�															//
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
#include <Vcl.Direct2D.hpp>
#include "usr_str.h"

//---------------------------------------------------------------------------
const _TCHAR *null_TCHAR = _T("");

//---------------------------------------------------------------------------
//���R���\�[�g�ɑΉ�������r
//---------------------------------------------------------------------------
int __fastcall comp_NaturalOrder(TStringList *List, int Index1, int Index2)
{
	return StrCmpLogicalW(List->Strings[Index1].c_str(), List->Strings[Index2].c_str());
}

//---------------------------------------------------------------------------
//���R���� ������ (�J���}�Ή�/ �����ꍇ�� Objects �Ŕ�r)
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
//���R�~�� ������ (�J���}�Ή�/ �����ꍇ�� Objects �Ŕ�r)
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
//���� Objects
//---------------------------------------------------------------------------
int __fastcall comp_ObjectsOrder(TStringList *List, int Index1, int Index2)
{
	return (int)List->Objects[Index1] - (int)List->Objects[Index2];
}

//---------------------------------------------------------------------------
//CSV�p��r�֐�
//---------------------------------------------------------------------------
int  USR_CsvCol 	 = 0;		//�\�[�g�Ώۗ�
int  USR_CsvSortMode = 1;		//�\�[�g���[�h 1:����/ -1�~��
bool USR_CsvTopIsHdr = false;	//�擪�s�͍��ږ�

//---------------------------------------------------------------------------
//������𐔒l�Ƃ��Ĕ�r
//---------------------------------------------------------------------------
int comp_NumStr(UnicodeString s1, UnicodeString s2)
{
	int res = 0;
	s1 = extract_top_num_str(s1);
	s2 = extract_top_num_str(s2);
	if (!s1.IsEmpty() && !s2.IsEmpty()) {
		try {
			double r1 = s1.ToDouble();
			double r2 = s2.ToDouble();
			res = (r1==r2)? 0 : (r1>r2)? 1 : -1;
		}
		catch (...) {
			res = 0;
		}
	}
	return res;
}

//---------------------------------------------------------------------------
//CSV ���R�� (�����ꍇ�� Objects �Ŕ�r)
//  ���l�Ƃ��Ĕ�r�ł��Ȃ���Ε����񏇁A�����Ȃ�΍s�ԍ���
//---------------------------------------------------------------------------
int __fastcall comp_CsvNaturalOrder(TStringList *List, int Index1, int Index2)
{
	if (USR_CsvTopIsHdr) {	//�擪�s�͍��ږ�
		if ((int)List->Objects[Index1]==0) return -1;
		if ((int)List->Objects[Index2]==0) return  1;
	}

	UnicodeString s1 = List->Strings[Index1];
	UnicodeString s2 = List->Strings[Index2];
	if (USAME_TS(s1, TXLIMIT_MARK)) return 1;
	if (USAME_TS(s2, TXLIMIT_MARK)) return -1;

	s1 = get_csv_item(s1, USR_CsvCol);
	s2 = get_csv_item(s2, USR_CsvCol);
	int res = comp_NumStr(s1, s2);
	if (res==0) res = StrCmpLogicalW(s1.c_str(), s2.c_str());
	if (res==0) res = (int)List->Objects[Index1] - (int)List->Objects[Index2];
	res *= USR_CsvSortMode;
	return res;
}
//---------------------------------------------------------------------------
//TSV ���R�� (�����ꍇ�� Objects �Ŕ�r)
//  ���l�Ƃ��Ĕ�r�ł��Ȃ���Ε����񏇁A�����Ȃ�΍s�ԍ���
//---------------------------------------------------------------------------
int __fastcall comp_TsvNaturalOrder(TStringList *List, int Index1, int Index2)
{
	if (USR_CsvTopIsHdr) {	//�擪�s�͍��ږ�
		if ((int)List->Objects[Index1]==0) return -1;
		if ((int)List->Objects[Index2]==0) return  1;
	}

	UnicodeString s1 = List->Strings[Index1];
	UnicodeString s2 = List->Strings[Index2];
	if (USAME_TS(s1, TXLIMIT_MARK)) return 1;
	if (USAME_TS(s2, TXLIMIT_MARK)) return -1;

	TStringDynArray itm1 = SplitString(s1, "\t");
	TStringDynArray itm2 = SplitString(s2, "\t");
	s1  = (USR_CsvCol<itm1.Length)? itm1[USR_CsvCol] : EmptyStr;
	s2  = (USR_CsvCol<itm2.Length)? itm2[USR_CsvCol] : EmptyStr;
	int res = comp_NumStr(s1, s2);
	if (res==0) res = StrCmpLogicalW(s1.c_str(), s2.c_str());
	if (res==0) res = (int)List->Objects[Index1] - (int)List->Objects[Index2];
	res *= USR_CsvSortMode;
	return res;
}

//---------------------------------------------------------------------------
//�w��Z�p���[�^�̑O�̕�������擾
//  �Z�p���[�^��������Ȃ��ꍇ�́A����������̂܂ܕԂ�
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
//�w��Z�p���[�^�̌�̕�������擾
//  �Z�p���[�^��������Ȃ��ꍇ�́AEmptyStr ��Ԃ�
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
//�w��Z�p���[�^�Ԃ̕�������擾
//  ��Z�p���[�^��������Ȃ��ꍇ�́A�O�Z�p���[�^�ȍ~�̕������Ԃ�
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
//�^�u�̑O�̕�������擾
//  �^�u���Ȃ��ꍇ�́A����������̂܂ܕԂ�
//---------------------------------------------------------------------------
UnicodeString get_pre_tab(UnicodeString s)
{
	return get_tkn(s, '\t');
}
//---------------------------------------------------------------------------
//�^�u�̌�̕�������擾
//  �^�u���Ȃ��ꍇ�́AEmptyStr ��Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_post_tab(UnicodeString s)
{
	return get_tkn_r(s, '\t');
}

//---------------------------------------------------------------------------
//�����s�������1�s�ڂ̓��e���擾
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
//�����񒆂� (�`) �����擾
//---------------------------------------------------------------------------
UnicodeString get_in_paren(UnicodeString s)
{
	int p1 = s.Pos('(');
	int p2 = s.Pos(')');
	if (p1==0 || p2==0 || p1>=p2) return EmptyStr;
	return s.SubString(p1 + 1,  p2 - p1 - 1);
}
//---------------------------------------------------------------------------
//�����񒆂� (�`) �����擾�A��������� (�`) �̌㑱�������
//---------------------------------------------------------------------------
UnicodeString split_in_paren(UnicodeString &s)
{
	return get_tkn_r(split_tkn(s, ')'), '(');
}

//---------------------------------------------------------------------------
//�O��Ƀ^�u���܂܂Ȃ���������擾
//  �����s�̏ꍇ�A��łȂ��ŏ��̕�����
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
//�w��Z�p���[�^�O�̕�����𕪗����Ď擾
//  �Z�p���[�^��������Ȃ��ꍇ�́A����������̂܂ܕԂ��A��������͋��
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
//�󔒋�؂�O�̕�����𕪗����Ď擾(�擪�����̋󔒂��l��)
//---------------------------------------------------------------------------
UnicodeString split_tkn_spc(UnicodeString &s)
{
	UnicodeString r;
	for (int i=1; i<=s.Length(); i++) {
		if (s[i]!=' ') break;
		r += " ";
	}
	if (!r.IsEmpty()) s.Delete(1, r.Length());
	return (r + split_tkn(s, ' '));
}

//---------------------------------------------------------------------------
//�^�u�O�̕�����𕪗����Ď擾
//  �^�u���Ȃ��ꍇ�́A����������̂܂ܕԂ��A��������͋��
//---------------------------------------------------------------------------
UnicodeString split_pre_tab(UnicodeString &s)
{
	return split_tkn(s, '\t');
}

//---------------------------------------------------------------------------
//�����񂩂��������(^:�`:)�𕪗����擾
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
//�����񂩂�擪�����𕪗����擾
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
//�R�}���h���C�����p�����[�^�ɕ���
//---------------------------------------------------------------------------
int split_cmd_line(UnicodeString s, TStringList *lst)
{
	bool in_qut = false;
	UnicodeString lbuf;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		//���p���O
		if (!in_qut) {
			if (c=='\"') {
				in_qut = true;
			}
			else if (c==' ') {
				if (!lbuf.IsEmpty()) {
					lst->Add(lbuf); 
					lbuf = EmptyStr;
				}
				continue;
			}
		}
		//���p����
		else if (c=='\"') {
			in_qut = false;
		}

		lbuf.cat_sprintf(_T("%c"), c);
	}

	if (!lbuf.IsEmpty()) lst->Add(lbuf);

	return lst->Count;
}

//---------------------------------------------------------------------------
//�R�}���h���C������t�@�C�����𕪗�
//---------------------------------------------------------------------------
UnicodeString split_file_param(UnicodeString &s)
{
	UnicodeString ret_str;
	s = Trim(s);
	if (remove_top_s(s, '\"')) {
		ret_str = split_tkn(s, '\"');
		s = Trim(s);
	}
	else {
		ret_str = split_tkn(s, ' ');
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//��������w���؂�ŕ���
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
//�����񂩂�w�����폜
//  �߂�l: ture=�폜����/ false=�ꂪ�܂܂�Ă��Ȃ�
//---------------------------------------------------------------------------
bool remove_text(UnicodeString &s, UnicodeString w)
{
	if (ContainsText(s, w)) {
		s = ReplaceText(s, w, EmptyStr);
		return true;
	}
	else {
		return false;
	}
}
//---------------------------------------------------------------------------
bool remove_text(UnicodeString &s, const _TCHAR *w)
{
	return remove_text(s, UnicodeString(w));
}

//---------------------------------------------------------------------------
//������̐擪�Ɏw��ꂪ��������폜 (�召�����𖳎�)
//  �߂�l: ture=�폜����/ false=�擪�Ɍꂪ�Ȃ�
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
//������̐擪�Ɏw��ꂪ��������폜 (�召���������)
//  �߂�l:  ture=�폜����/ false=�擪�Ɍꂪ�Ȃ�
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
//������̐擪�� @ ����������폜
//  �߂�l: ture=�擪�� @ ���폜����
//---------------------------------------------------------------------------
bool remove_top_AT(UnicodeString &s)
{
	return remove_top_s(s, '@');
}
//---------------------------------------------------------------------------
//������̐擪�� $ ����������폜
//  �߂�l: ture=�擪�� $ ���폜����
//---------------------------------------------------------------------------
bool remove_top_Dollar(UnicodeString &s)
{
	return remove_top_s(s, '$');
}

//---------------------------------------------------------------------------
//������̖����Ɏw��ꂪ��������폜 (�召�����𖳎�)
//  �߂�l: ture=��������폜����
//---------------------------------------------------------------------------
bool remove_end_text(UnicodeString &s, UnicodeString w)
{
	if (!EndsText(w, s)) return false;
	s.Delete(s.Length() - w.Length() + 1, w.Length());
	return true;
}
//---------------------------------------------------------------------------
//������̖����Ɏw��ꂪ��������폜 (�召���������)
//  �߂�l: ture=��������폜����
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
//�擪�����O������������擾
//---------------------------------------------------------------------------
UnicodeString exclude_top(UnicodeString s)
{
	return s.SubString(2, s.Length() - 1);
}
//---------------------------------------------------------------------------
//�擪�Ɩ��������O������������擾
//---------------------------------------------------------------------------
UnicodeString exclude_top_end(UnicodeString s)
{
	return s.SubString(2, s.Length() - 2);
}

//---------------------------------------------------------------------------
//�S�p�X�y�[�X���l�������g���~���O
//---------------------------------------------------------------------------
UnicodeString trim_ex(UnicodeString s)
{
	//�擪
	for (;;) {
		if (remove_top_s(s, "�@")) continue;
		if (remove_top_s(s, ' '))  continue;
		if (remove_top_s(s, '\t')) continue;
		break;
	}
	//����
	for (;;) {
		if (remove_end_s(s, "�@")) continue;
		if (remove_end_s(s, ' '))  continue;
		if (remove_end_s(s, '\t')) continue;
		break;
	}

	return Trim(s);
}

//---------------------------------------------------------------------------
//���K�\���ɂ��u��
//---------------------------------------------------------------------------
UnicodeString replace_regex(UnicodeString s, const _TCHAR *o, const _TCHAR *r)
{
	return TRegEx::Replace(s, o, r);
}
//---------------------------------------------------------------------------
//�� ^ �ɂ��擪�̘A���폜����������u��
//---------------------------------------------------------------------------
UnicodeString replace_regex_2(UnicodeString s, UnicodeString ptn, UnicodeString rep, TRegExOptions opt)
{
	UnicodeString ret_str;

	TMatchCollection mts = TRegEx::Matches(s, ptn, opt);
	int p = 1;
	if (mts.Count>0) {
		std::unique_ptr<TStringList> lst(new TStringList());
		for (int i=0; i<mts.Count; i++) {
			if (mts.Item[i].Success) {
				int idx = mts.Item[i].Index;
				if (idx>p) ret_str += s.SubString(p, idx - p);
				ret_str += TRegEx::Replace(mts.Item[i].Value, ptn, rep, opt);
				p = idx + mts.Item[i].Length;
			}
		}
	}

	if ((p - 1)<s.Length()) ret_str += s.SubString(p, s.Length() - p + 1);

	return ret_str;
}

//---------------------------------------------------------------------------
//���X�g�ɂ��������ĕ������u��
//  ���X�g����: ���������� TAB �u��������
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
//; ��؂�ŕ������ǉ�
//---------------------------------------------------------------------------
void cat_str_semicolon(UnicodeString &s, UnicodeString t)
{
	if (EndsStr(";", t)) t.Delete(t.Length(), 1);
	if (t.IsEmpty()) return;

	if (!s.IsEmpty() && !EndsStr(";", s)) s += ";";
	s += t;
}

//---------------------------------------------------------------------------
//�w�蒷�ɂȂ�܂Ő擪�ɋ󔒂�}��
//---------------------------------------------------------------------------
UnicodeString ins_spc_length(UnicodeString s, int len)
{
	int n = len - s.Length();
	if (n>0) s.Insert(StringOfChar(_T(' '), n), 1);
	return s;
}

//---------------------------------------------------------------------------
//�����񂪋�Ȃ�f�t�H���g��������擾
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
//������� NativeInt �l�ɕϊ�
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
//�����񂩂琮���l�𒊏o
//�I���s�������O�𑗏o
//---------------------------------------------------------------------------
int extract_int(UnicodeString s)
{
	int i = 1;
	while (i<=s.Length()) if (!iswdigit(s[i])) s.Delete(i, 1); else i++;
	return s.ToInt();
}
//---------------------------------------------------------------------------
//�����񂩂琮���l�𒊏o
//�����̏ꍇ�̓f�t�H���g�l��Ԃ�
//---------------------------------------------------------------------------
int extract_int_def(UnicodeString s, int def)
{
	int i = 1;
	while (i<=s.Length()) if (!iswdigit(s[i])) s.Delete(i, 1); else i++;
	return s.ToIntDef(def);
}

//---------------------------------------------------------------------------
//������̐擪���琔�l�������擾(�J���}�͖����A+/-�A�R���}�ɑΉ�)
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
//long double �l�𕶎���ɕϊ�
//---------------------------------------------------------------------------
UnicodeString ldouble_to_str(
	long double v,
	int f_w)		//double �̏���������	(default = 3)
{
	UnicodeString ret_str;

	long double ip;
	//����
	if (modfl(v, &ip)==0 && ip>=_I64_MIN && ip<=_I64_MAX) {
		ret_str.sprintf(_T("%lld"), (__int64)ip);
	}
	//����
	else {
		ret_str.sprintf(_T("%.*LG"), 16, v);
		if (v>=DBL_MIN && v<=DBL_MAX && ret_str.Pos('.') && f_w>0)
			ret_str.sprintf(_T("%.*f"), f_w, (double)v);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�T�C�Y�̐��l�𕶎����(TB�܂�)
//---------------------------------------------------------------------------
UnicodeString get_size_str_T(__int64 sz, int dg)
{
	UnicodeString szstr;
	float sz_k = sz/1024.0;
	float sz_m = sz_k/1024.0;
	float sz_g = sz_m/1024.0;

	if		(sz_g>1024) szstr.sprintf(_T("%.*f TB"), dg, sz_g/1024.0);
	else if (sz_m>1024) szstr.sprintf(_T("%.*f GB"), dg, sz_m/1024.0);
	else if (sz_k>1024) szstr.sprintf(_T("%.*f MB"), dg, sz_k/1024.0);
	else				szstr.sprintf(_T("%.*f KB"), dg, sz_k);

	return szstr;
}
//---------------------------------------------------------------------------
//�T�C�Y�̐��l�𕶎����(GB�܂�)
//---------------------------------------------------------------------------
UnicodeString get_size_str_G(
	__int64 sz,
	int wd,		//���w��	(default = 10)
	int dg)		//������	(default = 1)
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
	int wd)		//���w�� default = 14
{
	return ins_spc_length(((sz>=0)? FormatFloat(",0", (double)sz) : EmptyStr), wd);
}
//---------------------------------------------------------------------------
UnicodeString get_size_str_C(int sz)
{
	return FormatFloat(",0", (double)sz);
}

//---------------------------------------------------------------------------
//RRGGBB�`���̕������ TColor �ɕϊ�
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
//TColor ��RRGGBB�`���̕�����ɕϊ�
//---------------------------------------------------------------------------
UnicodeString col_to_xRRGGBB(TColor col)
{
	int cref = ColorToRGB(col);
	return UnicodeString().sprintf(_T("%02X%02X%02X"), GetRValue(cref), GetGValue(cref), GetBValue(cref));
}

//---------------------------------------------------------------------------
//�召��������ʂ����ɕ����������
//---------------------------------------------------------------------------
int pos_i(
	UnicodeString wd,	//������
	UnicodeString s)	//�Ώە�����
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
//�����񂪍Ō�Ɍ����ʒu���擾
//---------------------------------------------------------------------------
int pos_r(
	UnicodeString wd,	//������
	UnicodeString s)	//�Ώە�����
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
//�召��������ʂ����ɕ����񂪍Ō�Ɍ����ʒu���擾
//---------------------------------------------------------------------------
int pos_r_i(
	UnicodeString wd,	//������
	UnicodeString s)	//�Ώە�����
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
//���p���O�ŁA�����񂪍Ō�Ɍ����ʒu���擾
//---------------------------------------------------------------------------
int pos_r_q(
	UnicodeString wd,	//������( \v �͏���)
	UnicodeString s)	//�Ώە�����
{
	if (wd.IsEmpty()) return 0;
	UnicodeString ss;
	bool q_flag = false;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		//���p���O
		if (!q_flag) {
			if (c=='\"') q_flag = true;
		}
		//���p����
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
//���p���O�ŁA: ���Ō�Ɍ����ʒu���擾 (:\\ �͏���)
//---------------------------------------------------------------------------
int pos_r_q_colon(UnicodeString s)
{
	return pos_r_q(":", ReplaceStr(s, ":\\", "\f\f"));
}

//---------------------------------------------------------------------------
//������Ɍ����ꂪ�܂܂�Ă��邩?
//  AND(' ') / OR('|') �����Ή�
//---------------------------------------------------------------------------
bool contains_word_and_or(
	UnicodeString s,
	UnicodeString kwd,	//������
	bool case_sw)		//�召�������
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
// | ��؂胊�X�g�Ɏw��ꂪ�܂܂�Ă��邩�H
//---------------------------------------------------------------------------
bool contained_wd_i(UnicodeString lst, UnicodeString wd)
{
	if (wd.IsEmpty()) return false;

	if (!StartsStr("|", lst)) lst.Insert("|" ,1);
	if (!EndsStr("|", lst))   lst += "|";
	return ContainsText(lst, "|" + wd + "|");
}
//---------------------------------------------------------------------------
bool contained_wd_i(const _TCHAR *lst, UnicodeString wd)
{
	return contained_wd_i(UnicodeString(lst), wd);
}

//---------------------------------------------------------------------------
// | ��؂胊�X�g�̌���܂�ł��邩�H
//---------------------------------------------------------------------------
bool contains_wd_i(UnicodeString s, const _TCHAR *lst)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	for (int i=0; i<w_lst.Length; i++) if (ContainsText(s, w_lst[i])) return true;
	return false;
}

//---------------------------------------------------------------------------
// | ��؂胊�X�g����w��C���f�b�N�X�̌���擾
//---------------------------------------------------------------------------
UnicodeString get_word_i_idx(const _TCHAR *lst, int idx)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	return (idx>=0 && idx<w_lst.Length)? w_lst[idx] : EmptyStr;
}
//---------------------------------------------------------------------------
// | ��؂胊�X�g����w���̃C���f�b�N�X���擾
//---------------------------------------------------------------------------
int idx_of_word_i(const _TCHAR *lst, UnicodeString wd)
{
	TStringDynArray w_lst = SplitString(lst, "|");
	int idx = -1;
	for (int i=0; i<w_lst.Length && idx==-1; i++) if (SameText(w_lst[i], wd)) idx = i;
	return idx;
}
//---------------------------------------------------------------------------
// | ��؂胊�X�g�ɂ��鍀�ڂ�?
//---------------------------------------------------------------------------
bool test_word_i(UnicodeString w, UnicodeString list)
{
	if (list.IsEmpty() || w.IsEmpty()) return false;
	if (!StartsStr('|', w))		w.Insert("|", 1);
	if (!EndsStr('|', w)) 		w += "|";
	if (!StartsStr('|', list))	list.Insert("|", 1);
	if (!EndsStr('|', list)) 	list += "|";
	return ContainsText(list, w);
}

//---------------------------------------------------------------------------
//���C���h�J�[�h���܂ރp�^�[���ɂ�镶�����v
//---------------------------------------------------------------------------
bool str_match(UnicodeString ptn, UnicodeString s)
{
	UnicodeString c = ptn.SubString(1, 1);
	UnicodeString next_str = s.SubString(2, s.Length() - 1);
	UnicodeString next_ptn = ptn.SubString(2, ptn.Length() - 1);

	if (c.IsEmpty())		return s.IsEmpty();
	if (SameStr(c, "*"))	return str_match(next_ptn, s) || (!s.IsEmpty() && str_match(ptn, next_str));
	if (SameStr(c, "?"))	return !s.IsEmpty() && str_match(next_ptn, next_str);

	return (CompareText(c, s.SubString(1, 1))==0) && str_match(next_ptn, next_str);
}

//---------------------------------------------------------------------------
//���K�\���p�^�[���̃`�F�b�N
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
//���K�\���ɂ��}�b�`
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
UnicodeString get_match_regex(UnicodeString s, const _TCHAR *ptn)
{
	TMatch mt = TRegEx::Match(s, ptn);
	return mt.Success? mt.Value : EmptyStr;
}

//---------------------------------------------------------------------------
// ; ��؂蕶���񂩂琳�K�\���p�^�[��(/�`/)�𒊏o
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

	//�Y���p�����[�^����������폜
	if (idx!=-1) {
		UnicodeString lbuf;
		for (int i=0; i<lst.Length; i++) {
			if (i==idx) continue;
			if (!lbuf.IsEmpty()) lbuf += ";";
			lbuf += lst[i];
		}
		s = lbuf;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//������Ɏw���(�܂��̓p�^�[��)���܂܂�Ă��邩�H
//  �߂�l: �}�b�`���������̕�����
//---------------------------------------------------------------------------
UnicodeString ptn_match_str(
	UnicodeString ptn,	//�L�[���[�h (;�ŋ�؂��ĕ����w���)
						//  /�`/ �Ȃ琳�K�\���p�^�[��
	UnicodeString s)	//�Ώە�����
{
	if (ptn.IsEmpty() || s.IsEmpty()) return EmptyStr;

	TStringDynArray p_lst = SplitString(ptn, ";");
	TRegExOptions opt; opt << roIgnoreCase;
	try {
		UnicodeString ret_str;
		for (int i=0; i<p_lst.Length && ret_str.IsEmpty(); i++) {
			UnicodeString kwd = p_lst[i];	if (kwd.IsEmpty()) continue;
			//���K�\��
			if (is_regex_slash(kwd)) {
				TMatch mt = TRegEx::Match(s, exclude_top_end(kwd), opt);
				ret_str = mt.Success? mt.Value : EmptyStr;
			}
			//�ʏ�
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
//������Ɏw���(�܂��̓p�^�[��)�Ŏn�܂邩�H
//---------------------------------------------------------------------------
bool starts_ptn(
	UnicodeString ptn,	//�J�n������ (/�`/ �Ȃ琳�K�\���p�^�[��)
	UnicodeString s)	//�Ώە�����
{
	try {
		bool res;
		//���K�\��
		if (is_regex_slash(ptn)) {
			TRegExOptions opt; opt << roIgnoreCase;
			TMatch mt = TRegEx::Match(s, exclude_top_end(ptn), opt);
			res = mt.Success && (mt.Index==1);
		}
		//�ʏ�
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
//�����ꃊ�X�g�쐬
//---------------------------------------------------------------------------
void get_find_wd_list(UnicodeString wd, TStringList *lst)
{
	for (;;) {
		if (wd.IsEmpty()) break;
		int ph = wd.Pos(' ');
		int pw = wd.Pos("�@");
		int p = (ph && !pw)? ph : (!ph && pw)? pw : (ph && pw)? std::min(ph, pw) : 0;
		if (p==1) {
			wd.Delete(1, 1);
		}
		else if (p>1) {
			lst->Add(wd.SubString(1, p - 1));
			wd.Delete(1, p);
		}
		else {
			lst->Add(wd); break;
		}
	}

	//�󔒂�^�u��ϊ�
	for (int i=0; i<lst->Count; i++) lst->Strings[i] = conv_esc_char(lst->Strings[i]);
}

//---------------------------------------------------------------------------
//�����̕������ OR/AND ����
//---------------------------------------------------------------------------
bool find_mlt(
	UnicodeString wd,	//������ (�����̏ꍇ�͔��p�܂��͑S�p�󔒂ŋ�؂�j
	UnicodeString s,	//�Ώە�����
	bool and_sw,		//AND����			(default = false)
	bool not_sw,		//NOT����			(default = false)
	bool case_sw)		//�召���������	(default = false)
{
	if (wd.IsEmpty()) return false;

	//�����ꃊ�X�g�쐬
	std::unique_ptr<TStringList> wlst(new TStringList());
	get_find_wd_list(wd, wlst.get());

	//����
	bool ret = and_sw;
	for (int i=0; i<wlst->Count; i++) {
		int p = case_sw? s.Pos(wlst->Strings[i]) : pos_i(wlst->Strings[i], s);
		if (!not_sw) {
			//�܂�
			if (p>0) {
				//�����ꂩ��
				if (!and_sw) { ret = true; break; }
			}
			else {
				//�S�Ă�
				if (and_sw) { ret = false; break; }
			}
		}
		else {
			//�܂܂Ȃ�
			if (p==0) {
				//�����ꂩ��
				if (!and_sw) { ret = true; break; }
			}
			else {
				//�S�Ă�
				if (and_sw) { ret = false; break; }
			}
		}
	}

	return ret;
}

//---------------------------------------------------------------------------
//�����̕�������������A�}�b�`������̃��X�g���擾����
//---------------------------------------------------------------------------
bool find_mlt_str(
	UnicodeString wd,	//������ (�����̏ꍇ�͔��p�܂��͑S�p�󔒂ŋ�؂�j
	UnicodeString s,	//�Ώە�����
	TStringList *lst,	//[o] ���ۂɃ}�b�`������̃��X�g
	bool case_sw)		//�召���������	(default = false)
{
	lst->Clear();
	if (wd.IsEmpty()) return false;

	//�����ꃊ�X�g�쐬
	std::unique_ptr<TStringList> wlst(new TStringList());
	get_find_wd_list(wd, wlst.get());

	//����
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
//�s�����擾
//---------------------------------------------------------------------------
int get_line_count(UnicodeString s)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = s;
	return lst->Count;
}

//---------------------------------------------------------------------------
//TStringDynArray �ɕ������ǉ�
//---------------------------------------------------------------------------
void add_dyn_array(TStringDynArray &lst, UnicodeString s,
	bool no_dupl)	//�����̏ꍇ�͒ǉ����Ȃ�	(default = false)
{
	if (no_dupl) {
		for (int i=0; i<lst.Length; i++) if (SameText(s, lst[i])) return;
	}

	int len    = lst.Length;
	lst.Length = len + 1;
	lst[len]   = s;
}

//---------------------------------------------------------------------------
//CSV���ړ��e�擾
//---------------------------------------------------------------------------
TStringDynArray get_csv_array(UnicodeString src, int size,
	bool force_size)	//�T�C�Y�������I�ɐݒ�  (default = false)
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
//CSV���ڂ��쐬
//---------------------------------------------------------------------------
UnicodeString make_csv_str(UnicodeString s)
{
	return UnicodeString().sprintf(_T("\"%s\""), ReplaceStr(s, "\"", "\"\"").c_str());
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
		if (i>0) lbuf += ",";
		lbuf += make_csv_str(lst[i]);
	}
	return lbuf;
}

//---------------------------------------------------------------------------
//CSV�̃��X�g���� p �Ԗڂ̍��ڂ� s �̃C���f�b�N�X���擾
//---------------------------------------------------------------------------
int indexof_csv_list(TStringList *lst, UnicodeString s, int p)
{
	int idx = -1;
	for (int i=0; i<lst->Count && idx==-1; i++)
		if (SameText(s, get_csv_item(lst->Strings[i], p))) idx = i;
	return idx;
}
//---------------------------------------------------------------------------
//CSV�̃��X�g���� p �Ԗڂ̍��ڂ� s �̃��R�[�h���擾
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
//�G�X�P�[�v�V�[�P���X�����̕ϊ�
//---------------------------------------------------------------------------
UnicodeString conv_esc_char(UnicodeString s)
{
	if (s.IsEmpty()) return EmptyStr;

	s = ReplaceStr(s, "\\\\", "\f");	//\\ ���ꎞ�I�� \f �ɕς��Ă�����
	s = ReplaceStr(s, "\\t",  "\t");	//�^�u
	s = ReplaceStr(s, "\\n",  "\r\n");	//���s
	s = ReplaceStr(s, "\\s",  " ");		//NyanFi �ŗL
	s = ReplaceStr(s, "\f",   "\\");	//\\(\f) �� \ �ɕϊ�
	return s;
}

//---------------------------------------------------------------------------
//�u�� \\ �� /
//---------------------------------------------------------------------------
UnicodeString yen_to_slash(UnicodeString s)
{
	return ReplaceStr(s, "\\", "/");
}
//---------------------------------------------------------------------------
//�u�� / �� \\
//---------------------------------------------------------------------------
UnicodeString slash_to_yen(UnicodeString s)
{
	return ReplaceStr(s, "/", "\\");
}

//---------------------------------------------------------------------------
//SHA-1 ��7���̒Z�k�`�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString sha1_to_short(UnicodeString s)
{
	return replace_regex(s, _T("\\b([0-9a-f]{7})[0-9a-f]{33}\\b"), _T("\\1"));
}

//---------------------------------------------------------------------------
//�������召��������ʂ����ɔ�r
//---------------------------------------------------------------------------
bool same_ut_i(UnicodeString s, const _TCHAR *t)
{
	return (_tcsicmp(s.c_str(), t)==0);
}
//---------------------------------------------------------------------------
//�������召��������ʂ��Ăɔ�r
//---------------------------------------------------------------------------
bool same_ut_s(UnicodeString s, const _TCHAR *t)
{
	return (_tcscmp(s.c_str(), t)==0);
}

//---------------------------------------------------------------------------
//�����񔻒�
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
bool equal_HOME(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("HOME"))==0);
}
//---------------------------------------------------------------------------
bool equal_END(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("END"))==0);
}
//---------------------------------------------------------------------------
bool equal_F1(UnicodeString s)
{
	return (_tcsicmp(s.c_str(), _T("F1"))==0);
}

//---------------------------------------------------------------------------
//�p��������?
//---------------------------------------------------------------------------
bool is_alnum_str(UnicodeString s)
{
	if (s.IsEmpty()) return false;

	for (int i=1; i<=s.Length(); i++) {
		if (!_istalnum(s[i])) return false;
	}

	return true;
}

//---------------------------------------------------------------------------
//�Z�p���[�^(=="-") ?
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
// @ �Ŏn�܂邩 ?
//---------------------------------------------------------------------------
bool starts_AT(UnicodeString s)
{
	return StartsStr('@', s);
}
//---------------------------------------------------------------------------
// $ �Ŏn�܂邩 ?
//---------------------------------------------------------------------------
bool starts_Dollar(UnicodeString s)
{
	return StartsStr('$', s);
}

//---------------------------------------------------------------------------
// \ �ŏI��� ?
//---------------------------------------------------------------------------
bool ends_PathDlmtr(UnicodeString s)
{
	return EndsStr('\\', s);
}
//---------------------------------------------------------------------------
// \ ���܂� ?
//---------------------------------------------------------------------------
bool contains_PathDlmtr(UnicodeString s)
{
	return ContainsStr(s, "\\");
}
//---------------------------------------------------------------------------
// / ���܂� ?
//---------------------------------------------------------------------------
bool contains_Slash(UnicodeString s)
{
	return ContainsStr(s, "/");
}

//---------------------------------------------------------------------------
//�܂܂�Ă��� \ �̌����擾
//---------------------------------------------------------------------------
int count_PathDlmtr(UnicodeString s)
{
	int cnt = 0;
	for (int i=1; i<=s.Length(); i++) if (s[i]=='\\') cnt++;
	return cnt;
}

//---------------------------------------------------------------------------
// /�`/ (���K�\��) ?
//---------------------------------------------------------------------------
bool is_regex_slash(UnicodeString s)
{
	return (s.Length()>=2 && StartsStr('/', s) && EndsStr('/', s));
}

//---------------------------------------------------------------------------
//���p���ň͂܂�Ă��邩?
//---------------------------------------------------------------------------
bool is_quot(UnicodeString s)
{
	return (s.Length()>=2 && ((StartsStr('\"', s) && EndsStr('\"', s)) || ((StartsStr('\'', s) && EndsStr('\'', s)))));
}
//---------------------------------------------------------------------------
//�󔒂��܂�ł���ꍇ�A���p���ň͂�
//---------------------------------------------------------------------------
UnicodeString add_quot_if_spc(UnicodeString s)
{
	if (!is_quot(s) && (ContainsStr(s, " ") || ContainsStr(s, "�@"))) s = "\"" + s + "\"";
	return s;
}

//---------------------------------------------------------------------------
//���p�����O��
//---------------------------------------------------------------------------
UnicodeString exclude_quot(UnicodeString s)
{
	if (is_quot(s)) s = exclude_top_end(s);
	return s;
}

//---------------------------------------------------------------------------
//TStringList Vlues[name] �̐����l���擾
//---------------------------------------------------------------------------
int get_ListIntVal(TStringList *lst, const _TCHAR *name, int def)
{
	return lst->Values[name].ToIntDef(def);
}
//---------------------------------------------------------------------------
//TStringList Vlues[name] ����?
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
//m�b���玞�ԕ������
//---------------------------------------------------------------------------
UnicodeString __fastcall mSecToTStr(
	unsigned int ms,
	bool cs)			//1/100�b��\�� (default = true)
{
	int scnt = ms/1000;	//�b
	int c = (int)(ms/10.0 + 0.5) % 100;
	int s = scnt % 60;	scnt /= 60;
	int m = scnt % 60;
	int h = scnt / 60;

	UnicodeString ret_str;
	if (cs) {
		ret_str.sprintf(_T("%02u:%02u:%02u.%02u"), h, m, s, c);
	}
	else {
		if (c>50) s++;
		ret_str.sprintf(_T("%02u:%02u:%02u"), h, m, s);
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//�p�����[�^��m�b�ɕϊ�
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
//�擾���ƕ\��������v���Ȃ���O�t�H���g(Ricty Diminished �`)��?
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
//������̕\�������擾 (��O�t�H���g�ɑΉ�)
//��Ricty Diminished �́A���p�����̎��ۂ̕\�������擾�l+1�ɂȂ�ꍇ������
//---------------------------------------------------------------------------
int get_TextWidth(
	TCanvas *cv,
	UnicodeString s,	//������
	bool is_irreg)		//true = ��O�t�H���g(Ricty Diminished �`)
{
	if (is_irreg) {
		int wd = 0;
		for (int i=1; i<=s.Length(); i++) {
			wd += cv->TextWidth(s[i]);
			if (str_len_half(s[i])==1) wd++;
		}
		return wd;
	}

	return cv->TextWidth(s);
}

//---------------------------------------------------------------------------
//�w�蕶�����̕\�������擾
//---------------------------------------------------------------------------
int get_CharWidth(
	TCanvas *cv,
	int n,			//������	(���p�P��)
	int mgn)		//�]�T		(default = 0);
{
	TEXTMETRIC tm;
	::GetTextMetrics(cv->Handle , &tm);
	return (tm.tmAveCharWidth * n + mgn);
}

//---------------------------------------------------------------------------
//TPanel ���ł̕����񕝂��擾
//---------------------------------------------------------------------------
int get_WidthInPanel(
	UnicodeString s,
	TPanel *pp,
	bool d2d_sw)	//Direct2D �Ŏ擾	(default = false)
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
//���p���Z�̕����񒷂��擾
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
					r_u += "�H";
				else
					r_u += s_u[i];
			}
		}
		s_a = r_u;
	}
	return s_a.Length();
}

//---------------------------------------------------------------------------
//�T���Q�[�g�y�A���l�����ĕ��������擾
//---------------------------------------------------------------------------
int str_len_unicode(UnicodeString s)
{
	int n = 0;
	for (int i=1; i<=s.Length(); i++) if (!s.IsTrailSurrogate(i)) n++;
	return n;
}

//---------------------------------------------------------------------------
//���p���Z�̍ő啝���X�V
//---------------------------------------------------------------------------
void max_len_half(
	int &w,				//[o] �ő啝
	UnicodeString s)
{
	w = std::max(w, str_len_half(s));
}

//---------------------------------------------------------------------------
//�w�蕝(���p�P��)���ŕ����񂪉E�l�ɂȂ�悤�ɋ󔒂�}��
//---------------------------------------------------------------------------
UnicodeString align_r_str(UnicodeString s, int wd, UnicodeString post_s)
{
	int n = str_len_half(s);
	if (n<wd) s.Insert(StringOfChar(_T(' '), wd - n), 1);
	return s + post_s;
}
//---------------------------------------------------------------------------
//�w�蕝(���p�P��)���ŕ����񂪍��l�ɂȂ�悤�ɋ󔒂�}��
//---------------------------------------------------------------------------
UnicodeString align_l_str(UnicodeString s, int wd, UnicodeString post_s)
{
	int n = str_len_half(s);
	if (n<wd) s += StringOfChar(_T(' '), wd - n);
	return s + post_s;
}

//---------------------------------------------------------------------------
//�����񂪎w�蕝�Ɏ��܂�悤�ɒ���
//---------------------------------------------------------------------------
UnicodeString minimize_str(
	UnicodeString s,	//�Ώە�����
	TCanvas *cv,
	int  wd,			//������
	bool omit_end)		//�������ȗ� (default = false : ���Ԃ��ȗ�)
{
	bool is_irreg = IsIrregularFont(cv->Font);

	int ww = get_TextWidth(cv, s, is_irreg);
	if (ww > wd) {
		if (omit_end) {
			s += "�c";
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
			s.Insert("�c", p);	p += 1;
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
	UnicodeString s,	//�Ώە�����
	TCanvas *cv,
	int  wd,			//������
	bool omit_end)		//�������ȗ� (default = false : ���Ԃ��ȗ�)
{
	if (cv->TextWidth(s) > wd) {
		s = minimize_str(s, cv, wd, omit_end);
	}
	else {
		for (;;) {
			if (cv->TextWidth(s + " ")>wd) break;
			s += " ";
		}
	}
	return s;
}

//---------------------------------------------------------------------------
//�S�p/���p�ϊ�
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
//�S�p�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString to_FullWidth(UnicodeString s)
{
	return to_Full_or_Half(s, true);
}
//---------------------------------------------------------------------------
//���p�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString to_HalfWidth(UnicodeString s)
{
	return to_Full_or_Half(s, false);
}

//---------------------------------------------------------------------------
//�r���s���H
//---------------------------------------------------------------------------
int is_RuledLine(UnicodeString s)
{
	return SameStr(s, StringOfChar(_T('��'), s.Length())) ? 1 :
			(SameStr(s, StringOfChar(_T('��'), s.Length()))? 2 : 0);
}

//---------------------------------------------------------------------------
//�r���s��������擾 (---- -------- ...)
//---------------------------------------------------------------------------
UnicodeString make_RuledLine(int cnt, ...)
{
	UnicodeString ret_str;

	va_list ap;
	va_start(ap, cnt);
	for (int i=0; i<cnt; i++) {
		int w = va_arg(ap, int);
		if (w>0) {
			if (!ret_str.IsEmpty()) ret_str += " ";
			ret_str += StringOfChar(_T('-'), w);
		}
	}
	va_end(ap);

	return ret_str;
}

//---------------------------------------------------------------------------
//�A�h���X��������擾
//---------------------------------------------------------------------------
UnicodeString get_AddrStr(__int64 adr,
	int w)		//�\���� (default = 0)
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
	else {
		ret_str = "____:____";
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�A�X�y�N�g�䕶������擾
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
	if (w<20) {
		ret_str.cat_sprintf(_T("%u : %u"), w, h);
	}
	else {
		if (w>h)
			ret_str.cat_sprintf(_T("%5.4g : 1"), 1.0 * w/h);
		else
			ret_str.cat_sprintf(_T("1 : %5.4g"), 1.0 * h/w);
	}
	return Trim(ret_str);
}

//---------------------------------------------------------------------------
//�����Z�b�g�����擾
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
//�����̃E�G�C�g�����擾
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
//�R�[�h�y�[�W���X�g
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
//�R�[�h�y�[�W�̖��O���擾
//---------------------------------------------------------------------------
UnicodeString get_NameOfCodePage(
	int code_page, 
	bool long_sw,		//true = �����\��	(default = false)
	bool has_bom)		//true = BOM�t��	(default = false)
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
				ret_str += (has_bom? " BOM�t��" : " BOM����");
			else
				ret_str += (has_bom? " BOM�t" : " BOM��");
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//���O�̃R�[�h�y�[�W���擾
//---------------------------------------------------------------------------
int get_CodePageOfName(UnicodeString code_name)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = CodePageListX;
	return lst->Values[code_name].ToIntDef(0);
}

//---------------------------------------------------------------------------
//Unicode�u���b�N�����擾
//---------------------------------------------------------------------------
UnicodeString get_UnicodeBlockName(int code)
{
	struct {
		int bgn_cd;
		int end_cd;
		const _TCHAR *name;
	} blk_list[] = {
		{0x000000, 0x00007f, _T("��{���e������")},
		{0x000080, 0x0000ff, _T("���e��1�⏕")},
		{0x000100, 0x00017f, _T("���e�������g��A")},
		{0x000180, 0x00024f, _T("���e�������g��B")},
		{0x000250, 0x0002af, _T("IPA�g��")},
		{0x0002b0, 0x0002ff, _T("�O�i�𔺂��C������")},
		{0x000300, 0x00036f, _T("�����\�ȃ_�C�A�N���e�B�J���}�[�N")},
		{0x000370, 0x0003ff, _T("�M���V�A�����y�уR�v�g����")},
		{0x000400, 0x0004ff, _T("�L��������")},
		{0x000500, 0x00052f, _T("�L���������⏕")},
		{0x000530, 0x00058f, _T("�A�����j�A����")},
		{0x000590, 0x0005ff, _T("�w�u���C����")},
		{0x000600, 0x0006ff, _T("�A���r�A����")},
		{0x000700, 0x00074f, _T("�V���A����")},
		{0x000750, 0x00077f, _T("�A���r�A�����⏕")},
		{0x000780, 0x0007bf, _T("�^�[�i����")},
		{0x0007c0, 0x0007ff, _T("���R����")},
		{0x000800, 0x00083f, _T("�T�}���A����")},
		{0x000840, 0x00085f, _T("�}���_����")},
		{0x000860, 0x00086f, _T("�V���A�����g��")},
		{0x0008a0, 0x0008ff, _T("�A���r�A�����g��A")},
		{0x000900, 0x00097f, _T("�f�[���@�i�[�K���[����")},
		{0x000980, 0x0009ff, _T("�x���K������")},
		{0x000a00, 0x000a7f, _T("�O�����L�[����")},
		{0x000a80, 0x000aff, _T("�O�W�����[�g����")},
		{0x000b00, 0x000b7f, _T("�I�����[����")},
		{0x000b80, 0x000bff, _T("�^�~������")},
		{0x000c00, 0x000c7f, _T("�e���O����")},
		{0x000c80, 0x000cff, _T("�J���i�_����")},
		{0x000d00, 0x000d7f, _T("�}�����[��������")},
		{0x000d80, 0x000dff, _T("�V���n������")},
		{0x000e00, 0x000e7f, _T("�^�C����")},
		{0x000e80, 0x000eff, _T("���I�X����")},
		{0x000f00, 0x000fff, _T("�`�x�b�g����")},
		{0x001000, 0x00109f, _T("�r���}����")},
		{0x0010a0, 0x0010ff, _T("�O���W�A����")},
		{0x001100, 0x0011ff, _T("�n���O������")},
		{0x001200, 0x00137f, _T("�G�`�I�s�A����")},
		{0x001380, 0x00139f, _T("�G�`�I�s�A�����⏕")},
		{0x0013a0, 0x0013ff, _T("�`�F���L�[����")},
		{0x001400, 0x00167f, _T("�����J�i�_��Z������")},
		{0x001680, 0x00169f, _T("�I�K������")},
		{0x0016a0, 0x0016ff, _T("���[������")},
		{0x001700, 0x00171f, _T("�^�K���O����")},
		{0x001720, 0x00173f, _T("�n�k�m�I����")},
		{0x001740, 0x00175f, _T("�u�q�b�h����")},
		{0x001760, 0x00177f, _T("�^�O�o�k�A����")},
		{0x001780, 0x0017ff, _T("�N���[������")},
		{0x001800, 0x0018af, _T("�����S������")},
		{0x0018b0, 0x0018ff, _T("�����J�i�_��Z�������g��")},
		{0x001900, 0x00194f, _T("�����u����")},
		{0x001950, 0x00197f, _T("�^�C�E�i����")},
		{0x001980, 0x0019df, _T("�V�^�C������")},
		{0x0019e0, 0x0019ff, _T("�N���[�������p�L��")},
		{0x001a00, 0x001a1f, _T("�u�M�X����")},
		{0x001a20, 0x001aaf, _T("�^�C�^������")},
		{0x001ab0, 0x001aff, _T("�����\�ȃ_�C�A�N���e�B�J���}�[�N�g��")},
		{0x001b00, 0x001b7f, _T("�o������")},
		{0x001b80, 0x001bbf, _T("�X���_����")},
		{0x001bc0, 0x001bff, _T("�o�^�N����")},
		{0x001c00, 0x001c4f, _T("���v�`������")},
		{0x001c50, 0x001c7f, _T("�I���`�L����")},
		{0x001c80, 0x001c8f, _T("�L���������g��C")},
		{0x001c90, 0x001cbf, _T("�O���W�A�����g��")},
		{0x001cc0, 0x001ccf, _T("�X���_�����⏕")},
		{0x001cd0, 0x001cff, _T("���F�[�_�p�g��")},
		{0x001d00, 0x001d7f, _T("�����L���g��")},
		{0x001d80, 0x001dbf, _T("�����L���g���⏕")},
		{0x001dc0, 0x001dff, _T("�����\�ȃ_�C�A�N���e�B�J���}�[�N�⏕")},
		{0x001e00, 0x001eff, _T("���e�������g���ǉ�")},
		{0x001f00, 0x001fff, _T("�M���V�A�����g��")},
		{0x002000, 0x00206f, _T("��ʋ�Ǔ_")},
		{0x002070, 0x00209f, _T("��t���E���t��")},
		{0x0020a0, 0x0020cf, _T("�ʉ݋L��")},
		{0x0020d0, 0x0020ff, _T("�����\�ȋL���p�_�C�A�N���e�B�J���}�[�N")},
		{0x002100, 0x00214f, _T("�����l�L��")},
		{0x002150, 0x00218f, _T("�����ɏ��������")},
		{0x002190, 0x0021ff, _T("���")},
		{0x002200, 0x0022ff, _T("���w�L��")},
		{0x002300, 0x0023ff, _T("���̑��̋Z�p�p�L��")},
		{0x002400, 0x00243f, _T("����@�\�p�L��")},
		{0x002440, 0x00245f, _T("���w�I�����F��")},
		{0x002460, 0x0024ff, _T("�͂݉p����")},
		{0x002500, 0x00257f, _T("�r���f��")},
		{0x002580, 0x00259f, _T("�u���b�N�v�f")},
		{0x0025a0, 0x0025ff, _T("�􉽊w�͗l")},
		{0x002600, 0x0026ff, _T("���̑��̋L��")},
		{0x002700, 0x0027bf, _T("�����L��")},
		{0x0027c0, 0x0027ef, _T("���̑��̐��w�L��A")},
		{0x0027f0, 0x0027ff, _T("�⏕���A")},
		{0x002800, 0x0028ff, _T("�_���}�`")},
		{0x002900, 0x00297f, _T("�⏕���B")},
		{0x002980, 0x0029ff, _T("���̑��̐��w�L��B")},
		{0x002a00, 0x002aff, _T("�⏕���w�L��")},
		{0x002b00, 0x002bff, _T("���̑��̋L���y�і��")},
		{0x002c00, 0x002c5f, _T("�O���S������")},
		{0x002c60, 0x002c7f, _T("���e�������g��C")},
		{0x002c80, 0x002cff, _T("�R�v�g����")},
		{0x002d00, 0x002d2f, _T("�O���W�A�����⏕")},
		{0x002d30, 0x002d7f, _T("�e�B�t�B�i�O����")},
		{0x002d80, 0x002ddf, _T("�G�`�I�s�A�����g��")},
		{0x002de0, 0x002dff, _T("�L���������g��A")},
		{0x002e00, 0x002e7f, _T("�⏕��Ǔ_")},
		{0x002e80, 0x002eff, _T("CJK����⏕")},
		{0x002f00, 0x002fdf, _T("�N������")},
		{0x002ff0, 0x002fff, _T("�����\���L�q����")},
		{0x003000, 0x00303f, _T("CJK�̋L���y�ы�Ǔ_")},
		{0x003040, 0x00309f, _T("������")},
		{0x0030a0, 0x0030ff, _T("�Љ���")},
		{0x003100, 0x00312f, _T("��������")},
		{0x003130, 0x00318f, _T("�n���O���݊�����")},
		{0x003190, 0x00319f, _T("�����p�L��")},
		{0x0031a0, 0x0031bf, _T("��������g��")},
		{0x0031c0, 0x0031ef, _T("CJK�̕M��")},
		{0x0031f0, 0x0031ff, _T("�Љ����g��")},
		{0x003200, 0x0032ff, _T("�͂�CJK�����E��")},
		{0x003300, 0x0033ff, _T("CJK�݊��p����")},
		{0x003400, 0x004dbf, _T("CJK���������g��A")},
		{0x004dc0, 0x004dff, _T("�Ռo�L��")},
		{0x004e00, 0x009fff, _T("CJK��������")},
		{0x00a000, 0x00a48f, _T("�C����")},
		{0x00a490, 0x00a4cf, _T("�C��������")},
		{0x00a4d0, 0x00a4ff, _T("���X����")},
		{0x00a500, 0x00a63f, _T("���@�C����")},
		{0x00a640, 0x00a69f, _T("�L���������g��B")},
		{0x00a6a0, 0x00a6ff, _T("�o��������")},
		{0x00a700, 0x00a71f, _T("�����C������")},
		{0x00a720, 0x00a7ff, _T("���e�������g��D")},
		{0x00a800, 0x00a82f, _T("�V���e�B�E�i�O������")},
		{0x00a830, 0x00a83f, _T("�C���h���p���ʋL��")},
		{0x00a840, 0x00a87f, _T("�p�X�p����")},
		{0x00a880, 0x00a8df, _T("�T�E���[�V���g������")},
		{0x00a8e0, 0x00a8ff, _T("�f�[���@�i�[�K���[�����g��")},
		{0x00a900, 0x00a92f, _T("�J���[����")},
		{0x00a930, 0x00a95f, _T("���W��������")},
		{0x00a960, 0x00a97f, _T("�n���O������g��A")},
		{0x00a980, 0x00a9df, _T("�W��������")},
		{0x00a9e0, 0x00a9ff, _T("�r���}�����g��B")},
		{0x00aa00, 0x00aa5f, _T("�`��������")},
		{0x00aa60, 0x00aa7f, _T("�r���}�����g��A")},
		{0x00aa80, 0x00aadf, _T("�^�C�E���B�G�g����")},
		{0x00aae0, 0x00aaff, _T("���C�e�C�����g��")},
		{0x00ab00, 0x00ab2f, _T("�G�`�I�s�A�����g��A")},
		{0x00ab30, 0x00ab6f, _T("���e�������g��E")},
		{0x00ab70, 0x00abbf, _T("�`�F���L�[�����⏕")},
		{0x00abc0, 0x00abff, _T("���C�e�C����")},
		{0x00ac00, 0x00d7af, _T("�n���O�����ߕ���")},
		{0x00d7b0, 0x00d7ff, _T("�n���O������g��B")},
		{0x00d800, 0x00db7f, _T("�T���Q�[�g")},
		{0x00db80, 0x00dbff, _T("�T���Q�[�g")},
		{0x00dc00, 0x00dfff, _T("�T���Q�[�g")},
		{0x00e000, 0x00f8ff, _T("���p�̈�")},
		{0x00f900, 0x00faff, _T("CJK�݊�����")},
		{0x00fb00, 0x00fb4f, _T("�A���t�@�x�b�g�\���`")},
		{0x00fb50, 0x00fdff, _T("�A���r�A�\���`A")},
		{0x00fe00, 0x00fe0f, _T("���`�I���q")},
		{0x00fe10, 0x00fe1f, _T("�c�����`")},
		{0x00fe20, 0x00fe2f, _T("�����\�Ȕ��L��")},
		{0x00fe30, 0x00fe4f, _T("CJK�݊��`")},
		{0x00fe50, 0x00fe6f, _T("�����`")},
		{0x00fe70, 0x00feff, _T("�A���r�A�\���`B")},
		{0x00ff00, 0x00ffef, _T("���p�E�S�p�`")},
		{0x00fff0, 0x00ffff, _T("����p�r����")},
		{0x010000, 0x01007f, _T("������B���ߕ���")},
		{0x010080, 0x0100ff, _T("������B�\�ӕ���")},
		{0x010100, 0x01013f, _T("�G�[�Q����")},
		{0x010140, 0x01018f, _T("�Ñ�M���V�A����")},
		{0x010190, 0x0101cf, _T("�Ñ�L��")},
		{0x0101d0, 0x0101ff, _T("�t�@�C�X�g�X�̉~�Օ���")},
		{0x010280, 0x01029f, _T("�����L�A����")},
		{0x0102a0, 0x0102df, _T("�J���A����")},
		{0x0102e0, 0x0102ff, _T("�R�v�g�E�G�p�N�g����")},
		{0x010300, 0x01032f, _T("�Ñ�C�^���A����")},
		{0x010330, 0x01034f, _T("�S�[�g����")},
		{0x010350, 0x01037f, _T("�Ãy��������")},
		{0x010380, 0x01039f, _T("�E�K���g����")},
		{0x0103a0, 0x0103df, _T("�Ñ�y���V������")},
		{0x010400, 0x01044f, _T("�f�U���b�g����")},
		{0x010450, 0x01047f, _T("�V���[����")},
		{0x010480, 0x0104af, _T("�I�X�}�j�A����")},
		{0x0104b0, 0x0104ff, _T("�I�Z�[�W����")},
		{0x010500, 0x01052f, _T("�G���o�T������")},
		{0x010530, 0x01056f, _T("�J�t�J�X�E�A���o�j�A����")},
		{0x010600, 0x01077f, _T("������A")},
		{0x010800, 0x01083f, _T("�L�v���X���ߕ���")},
		{0x010840, 0x01085f, _T("�鍑�A��������")},
		{0x010860, 0x01087f, _T("�p���~������")},
		{0x010880, 0x0108af, _T("�i�o�e�A����")},
		{0x0108e0, 0x0108ff, _T("�n�g������")},
		{0x010900, 0x01091f, _T("�t�F�j�L�A����")},
		{0x010920, 0x01093f, _T("�����f�B�A����")},
		{0x010980, 0x01099f, _T("�����G����������")},
		{0x0109a0, 0x0109ff, _T("�����G����������")},
		{0x010a00, 0x010a5f, _T("�J���[�V���e�B�[����")},
		{0x010a60, 0x010a7f, _T("�Ñ��A���r�A����")},
		{0x010a80, 0x010a9f, _T("�Ñ�k�A���r�A����")},
		{0x010ac0, 0x010aff, _T("�}�j����")},
		{0x010b00, 0x010b3f, _T("�A���F�X�^����")},
		{0x010b40, 0x010b5f, _T("�蕶�p���e�B�A����")},
		{0x010b60, 0x010b7f, _T("�蕶�p�t�����B����")},
		{0x010b80, 0x010baf, _T("���҃p�t�����B����")},
		{0x010c00, 0x010c4f, _T("�˙Ε���")},
		{0x010c80, 0x010cff, _T("�Ãn���K���[����")},
		{0x010d00, 0x010d3f, _T("�n�j�[�t�B�[�E���q���M������")},
		{0x010e60, 0x010e7f, _T("���~�����L��")},
		{0x010f00, 0x010f2f, _T("�Ã\�O�h����")},
		{0x010f30, 0x010f6f, _T("�\�O�h����")},
		{0x010fe0, 0x010fff, _T("�G���}�C�X����")},
		{0x011000, 0x01107f, _T("�u���[�t�~�[����")},
		{0x011080, 0x0110cf, _T("�J�C�e�B�[����")},
		{0x0110d0, 0x0110ff, _T("�\�����O�\���y���O����")},
		{0x011100, 0x01114f, _T("�`���N�}����")},
		{0x011150, 0x01117f, _T("�}�n�[�W���j�[����")},
		{0x011180, 0x0111df, _T("�V���[���_�[����")},
		{0x0111e0, 0x0111ff, _T("���V���n������")},
		{0x011200, 0x01124f, _T("�z�W������")},
		{0x011280, 0x0112af, _T("�����^�[�j�[����")},
		{0x0112b0, 0x0112ff, _T("�t�_�[���[�f�B�[����")},
		{0x011300, 0x01137f, _T("�O�����^����")},
		{0x011400, 0x01147f, _T("�l������")},
		{0x011480, 0x0114df, _T("�e�B���t�[�^����")},
		{0x011580, 0x0115ff, _T("���ܕ���")},
		{0x011600, 0x01165f, _T("���[�f�B�[����")},
		{0x011660, 0x01167f, _T("�����S�������⏕")},
		{0x011680, 0x0116cf, _T("�^�[�N���[����")},
		{0x011700, 0x01173f, _T("�A�[�z������")},
		{0x011800, 0x01184f, _T("�h�[�O���[����")},
		{0x0118a0, 0x0118ff, _T("�������O�E�N�V�e�B����")},
		{0x0119a0, 0x0119ff, _T("�i���f�B�i�[�K���[����")},
		{0x011a00, 0x011a4f, _T("�U�i�o�U�����`����")},
		{0x011a50, 0x011aaf, _T("�\�����{����")},
		{0x011ac0, 0x011aff, _T("�p�E�E�`���E�n�E����")},
		{0x011c00, 0x011c6f, _T("�o�C�N�V���L�[����")},
		{0x011c70, 0x011cbf, _T("�}���`�F������")},
		{0x011d00, 0x011d5f, _T("�}�T�����E�S�[���f�B�[����")},
		{0x011d60, 0x011daf, _T("�O���W�����E�S�[���f�B�[����")},
		{0x011ee0, 0x011eff, _T("�}�J�b�T������")},
		{0x011fc0, 0x011fff, _T("�^�~�������⏕")},
		{0x012000, 0x0123ff, _T("���`����")},
		{0x012400, 0x01247f, _T("���`�����̐����y�ы�Ǔ_")},
		{0x012480, 0x01254f, _T("�V�����[�����`����")},
		{0x013000, 0x01342f, _T("�G�W�v�g��������")},
		{0x013430, 0x01343f, _T("�G�W�v�g����������������L��")},
		{0x014400, 0x01467f, _T("�A�i�g���A��������")},
		{0x016800, 0x016a3f, _T("�o���������⏕")},
		{0x016a40, 0x016a6f, _T("��������")},
		{0x016ad0, 0x016aff, _T("�o�T����")},
		{0x016b00, 0x016b8f, _T("�p�n�E�E�t��������")},
		{0x016e40, 0x016e9f, _T("���f�t�@�C�h��������")},
		{0x016f00, 0x016f9f, _T("�~���I����")},
		{0x016fe0, 0x016fff, _T("�����̋L���y�ы�Ǔ_")},
		{0x017000, 0x0187ff, _T("���ĕ���")},
		{0x018800, 0x018aff, _T("���ĕ����̍\���v�f")},
		{0x01b000, 0x01b0ff, _T("�����⏕")},
		{0x01b100, 0x01b12f, _T("�����g��A")},
		{0x01b130, 0x01b16f, _T("�����������g��")},
		{0x01b170, 0x01b2ff, _T("����")},
		{0x01bc00, 0x01bc9f, _T("�f���v�����C�G�����L")},
		{0x01bca0, 0x01bcaf, _T("���L��������L��")},
		{0x01d000, 0x01d0ff, _T("�r�U���`�����y�L��")},
		{0x01d100, 0x01d1ff, _T("���y�L��")},
		{0x01d200, 0x01d24f, _T("�Ñ�M���V�A�����L��")},
		{0x01d2e0, 0x01d2ff, _T("�}������")},
		{0x01d300, 0x01d35f, _T("�����o�L��")},
		{0x01d360, 0x01d37f, _T("�Z�ؗp����")},
		{0x01d400, 0x01d7ff, _T("���w�p�p�����L��")},
		{0x01d800, 0x01daaf, _T("�T�b�g����b�\�L�@")},
		{0x01e000, 0x01e02f, _T("�O���S�������⏕")},
		{0x01e100, 0x01e14f, _T("Nyiakeng����")},
		{0x01e2c0, 0x01e2ff, _T("�����`������")},
		{0x01e800, 0x01e8df, _T("�����f����")},
		{0x01e900, 0x01e95f, _T("�A�h��������")},
		{0x01ec70, 0x01ecbf, _T("�C���h�E�V���N����")},
		{0x01ed00, 0x01ed4f, _T("�I�X�}���E�V���N����")},
		{0x01ee00, 0x01eeff, _T("�A���r�A�����L��")},
		{0x01f000, 0x01f02f, _T("�}�[�W�����L��")},
		{0x01f030, 0x01f09f, _T("�h�~�m�L��")},
		{0x01f0a0, 0x01f0ff, _T("�g�����v�L��")},
		{0x01f100, 0x01f1ff, _T("�͂݉p�����⏕")},
		{0x01f200, 0x01f2ff, _T("�͂݊����⏕")},
		{0x01f300, 0x01f5ff, _T("���̑��̋L���y�ъG�L��")},
		{0x01f600, 0x01f64f, _T("�當��")},
		{0x01f650, 0x01f67f, _T("�����p�G�L��")},
		{0x01f680, 0x01f6ff, _T("��ʋy�ђn�}�L��")},
		{0x01f700, 0x01f77f, _T("�B���p�L��")},
		{0x01f780, 0x01f7ff, _T("�􉽊w�͗l�g��")},
		{0x01f800, 0x01f8ff, _T("�⏕���C")},
		{0x01f900, 0x01f9ff, _T("�⏕�L���y�ъG�L��")},
		{0x01fa00, 0x01fa6f, _T("�`�F�X�L��")},
		{0x01fa70, 0x01faff, _T("�L���y�ъG�L���g��A")},
		{0x020000, 0x02a6df, _T("CJK���������g��B")},
		{0x02a700, 0x02b73f, _T("CJK���������g��C")},
		{0x02b740, 0x02b81f, _T("CJK���������g��D")},
		{0x02b820, 0x02ceaf, _T("CJK���������g��E")},
		{0x02ceb0, 0x02ebef, _T("CJK���������g��F")},
		{0x02f800, 0x02fa1f, _T("CJK�݊������⏕")},
		{0x0e0000, 0x0e007f, _T("�^�O")},
		{0x0e0100, 0x0e01ef, _T("���`�I���q�⏕")},
		{0x0f0000, 0x0fffff, _T("�⏕���p�̈�A")},
		{0x100000, 0x10ffff, _T("�⏕���p�̈�B")}
	};

	UnicodeString ret_str;
	int cnt  = sizeof(blk_list)/sizeof(blk_list[0]);
	for (int i=0; i<cnt; i++) {
		if (code>=blk_list[i].bgn_cd && code<=blk_list[i].end_cd) {
			ret_str = blk_list[i].name;
			break;
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//UTF8�`�F�b�N
//---------------------------------------------------------------------------
int check_UTF8(BYTE *bp, int size)
{
	int cnt = 0;
	try {
		int i = 0;
		while (i<size) {
			if (bp[i]<0x80) {
				i++;
			}
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
//�������X�g���[���̃R�[�h�y�[�W���擾
//  �߂�l: �R�[�h�y�[�W (0:�s��/ -1:�o�C�i��)
//---------------------------------------------------------------------------
int get_MemoryCodePage(
	TMemoryStream *ms,
	bool *has_bom)		//[o] BOM�L��	(default = NULL)
{
	int code_page = 0;
	if (has_bom) *has_bom = false;

	int src_size = ms->Size;
	if (src_size==0) return 932;

	//�\������
	BYTE *bp = (BYTE*)ms->Memory;
	int x00_cnt	= 0;	//0x00 �̌�
	int z_rpt	= 0;	//0x00 �̘A����
	int z_max	= 0;	//0x00 �̍ő�A����
	int esc_cnt = 0;	//ESC �̌�
	int asc_cnt = 0;	//ASCII �̌�
	int ltn_cnt = 0;	//ISO8859(Latin-1��z��) �̌�
	int b7_cnt	= 0;	//b7==1 �̌�

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

	if (z_max>3 || (src_size<3 && x00_cnt>0)) return -1;	//�o�C�i���[

	//BOM�̗L�����`�F�b�N
	if		(bp[0]==0xfe && bp[1]==0xff)				code_page = 1201;		//UTF-16BE BOM�t
	else if (bp[0]==0xff && bp[1]==0xfe)				code_page = 1200;		//UTF-16 (LE) BOM�t
	else if (bp[0]==0xef && bp[1]==0xbb && bp[2]==0xbf)	code_page = CP_UTF8;	//UTF-8 BOM�t

	if (code_page>0) {
		if (has_bom) *has_bom = true;
		return code_page;
	}

	//BOM����
	if (check_UTF8(bp, src_size)>0) code_page = CP_UTF8;	//UTF-8 BOM����
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
				if (((0xa1<=b0 && b0<=0xfe) && (0xa1<=b1 && b1<=0xfe)) || (b0==0x8e && (0xa1<=b1 && b1<=0xdf))) {
					euc_cnt += 2;  i++;
				}
				else if (i < src_size-2) {
					BYTE b2 = bp[i + 2];
					if (b0==0x8f && (0xa1<=b1 && b1<=0xfe) && (0xa1<=b2 && b2<=0xfe)) {
						 euc_cnt += 3;  i += 2;
					}
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
						else {
							b64cnt++;
						}
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

	//UTF-16��?
	int be_asc = 1, le_asc = 1;
	int be_hk  = 1, le_hk  = 1;		//�Ђ炪�ȁA�J�^�J�i
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
//�������X�g���[���̃R�[�h�y�[�W�𔻒肵�A������Ƃ��Ď擾
//---------------------------------------------------------------------------
UnicodeString get_MemoryStrins(TMemoryStream *ms)
{
	//�R�[�h�y�[�W�擾
	ms->Seek(0, soFromBeginning);
	int code_page = get_MemoryCodePage(ms);
	if (code_page<0)  return EmptyStr;
	if (code_page==0) code_page = 932;

	//�o�b�t�@�ɓǂݍ���
	TBytes Bytes;
	Bytes.Length = ms->Size;
	ms->Seek(0, soFromBeginning);
	ms->Read(Bytes, ms->Size);

	//�G���R�[�h
	std::unique_ptr<TStringList> lst(new TStringList());
	std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
	return enc->GetString(Bytes, 0, Bytes.Length);
}

//---------------------------------------------------------------------------
//�T���Q�[�g�y�A�������`�F�b�N
//�߂�l: ���������������󔒋�؂�ŘA�����ĕԂ��B�Ȃ���� EmptyStr
//---------------------------------------------------------------------------
UnicodeString check_Surrogates(UnicodeString s)
{
	UnicodeString ret_str;
	int s_len = s.Length();
	int i = 1;
	while(i<=s_len) {
		if (s.IsLeadSurrogate(i)) {
			if (!ret_str.IsEmpty()) ret_str += " ";
			ret_str.cat_sprintf(_T("%c"), s[i++]);
			if (i<=s_len && s.IsTrailSurrogate(i)) ret_str.cat_sprintf(_T("%c"), s[i++]);
		}
		else i++;
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//���ˑ�����(���[�}�����A�͂ݕ����A�P�ʋL���A���������Ȃ�)���`�F�b�N
//�߂�l: ���������������󔒋�؂�ŘA�����ĕԂ��B�Ȃ���� EmptyStr
//---------------------------------------------------------------------------
UnicodeString check_EnvDepandChars(UnicodeString s)
{
	int tbl[] = {0x2116, 0x2121,							//�� ��
				 0x2211, 0x221a, 0x221f,					//�� �� ��
				 0x2220, 0x2229, 0x222a, 0x222b, 0x222e,	//�� �� �� �� ��
				 0x2235, 0x2252, 0x2261, 0x22a5, 0x22bf};	//�� �� �� �� ��
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
			if (!ret_str.IsEmpty()) ret_str += " ";
			ret_str.cat_sprintf(_T("%c"), s[i]);
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�W�������ꂽ���[�x���V���^�C���������擾
//  �ő啶����=256
//�߂�l: 0�`1000
//---------------------------------------------------------------------------
int get_NrmLevenshteinDistance(UnicodeString s1, UnicodeString s2,
	bool ig_case,	//�召�����𖳎�	(default = false)
	bool ig_num,	//���������𖳎�	(default = false)
	bool ig_fh)		//�S�p/���p�𖳎�	(default = false)
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
//#nnnn �`���̒l�𕶎���Ƀf�R�[�h
//---------------------------------------------------------------------------
UnicodeString decode_TxtVal(UnicodeString s,
	bool with_q)	//������Ȃ���p���ň͂�	(default = false)
{
	UnicodeString sbuf;

	bool is_str = false;
	while (!s.IsEmpty()) {
		if (remove_top_s(s, '\'')) {
			sbuf += split_tkn(s, '\'');
			is_str = true;
		}
		else if (remove_top_s(s, '#')) {
			UnicodeString tmpstr = split_tkn(s, '\'');
			if (!s.IsEmpty()) s.Insert("\'", 1);
			TStringDynArray c_lst = SplitString(tmpstr, "#");
			UnicodeString tstr;
			for (int j=0; j<c_lst.Length; j++) {
				int c = c_lst[j].ToIntDef(0);
				switch (c) {
				case 0x0d: tstr += "\\r"; break;
				case 0x0a: tstr += "\\n"; break;
				default:   tstr += wchar_t(c);
				}
			}
			sbuf += tstr;
			is_str = true;
		}
		else {
			sbuf += s;
			break;
		}
	}

	if (is_str && with_q) sbuf = "\'" + sbuf + "\'";
	return sbuf;
}

//---------------------------------------------------------------------------
//�C���f�b�N�X���烁�j���[�p�A�N�Z�����[�^��������쐬
//---------------------------------------------------------------------------
UnicodeString make_MenuAccStr(
	int idx, 		//�C���f�b�N�X�ԍ�
	bool alp_sw)	//true = A�`Z���g�p	(default = false : 1�`9,0 �̂�)
{
	UnicodeString ret_str;
	if (idx<10)
		ret_str.sprintf(_T("&%u: "), (idx + 1)%10);
	else if (alp_sw && idx<36)
		ret_str.sprintf(_T("&%c: "), (char)('A' + (idx - 10)));
	else
		ret_str = "   ";
	return ret_str;
}

//---------------------------------------------------------------------------
//���̃A���t�@�x�b�g��������擾
//---------------------------------------------------------------------------
UnicodeString get_NextAlStr(UnicodeString s)
{
	if (s.IsEmpty()) return EmptyStr;

	bool lo_sw = (s[s.Length()]>='a');
	s = s.UpperCase();

	bool c_flag = true;
	for (int i=s.Length(); i>=1 && c_flag; i--) {
		if (s[i]<'Z') {
			s[i] += 1;
			c_flag = false;
		}
		else {
			s[i] = 'A';
			c_flag = true;
		}
	}
	if (c_flag) s.Insert("A", 1);

	if (lo_sw) s = s.LowerCase();

	return s;
}
//---------------------------------------------------------------------------
