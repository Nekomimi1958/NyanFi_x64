//----------------------------------------------------------------------//
// HTML→テキスト変換処理クラス											//
// Markdown記法にも対応													//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "htmconv.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// HtmConv クラス
//---------------------------------------------------------------------------
HtmConv::HtmConv()
{
	HtmBuf = new TStringList();
	TxtBuf = new TStringList();

	CodePage = 65001;

	ToMarkdown	 = false;
	InsHdrInf	 = false;
	InsHrSection = InsHrArticle = InsHrNav = false;

	LineWidth	 = DEF_LINE_WIDTH;
}

//---------------------------------------------------------------------------
HtmConv::~HtmConv()
{
	delete HtmBuf;
	delete TxtBuf;
}

//---------------------------------------------------------------------------
//ファイル名を相対パスから絶対パスへ変換
// ドライブ名が指定されていたら、変換せずにそのまま返す
//---------------------------------------------------------------------------
UnicodeString HtmConv::rel_to_abs(
	UnicodeString fnam,		//変換対象ファイル名
	UnicodeString rnam)		//基準ファイル名
{
	if (fnam.IsEmpty() || !ExtractFileDrive(fnam).IsEmpty()) return fnam;

	UnicodeString dnam = ExtractFileDir(rnam);
	if (dnam.IsEmpty()) return fnam;

	for (;;) {
		if (!StartsStr("..\\", fnam)) break;
		int p = pos_r(_T("\\"), dnam); if (p==0) break;
		dnam = dnam.SubString(1, p - 1);
		fnam.Delete(1, 3);
	}

	return IncludeTrailingPathDelimiter(dnam) + fnam;
}

//---------------------------------------------------------------------------
//整数を a〜z による表記に変換
//  a〜z, aa〜az, ba〜bz...
//---------------------------------------------------------------------------
UnicodeString HtmConv::int_to_alpha(int n)
{
	UnicodeString ret_str;
	n--;
	if (n<0) {
		ret_str = "0";
	}
	else {
		int n1 = n /26;
		if (n1>0) ret_str.cat_sprintf(_T("%c"), (char)('a' + n1 - 1));
		ret_str.cat_sprintf(_T("%c"), (char)('a' + (n % 26)));
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//整数(1〜999)をローマ数字(小文字)に変換
//---------------------------------------------------------------------------
UnicodeString HtmConv::int_to_roman(int n)
{
	UnicodeString ret_str;
	//100の位
	int rn = (n/100)%10;
	if (rn>0) {
		switch (rn) {
		case 1: case 2: case 3:
			ret_str += StringOfChar(_T('c'), rn); break;
		case 4: ret_str += "cd"; break;
		case 5: ret_str += "d";  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("d%s"), StringOfChar(_T('c'), rn - 5).c_str()); break;
		case 9: ret_str += "cm"; break;
		}
	}

	//10の位
	rn = (n/10)%10;
	if (rn>0) {
		switch (rn) {
		case 1: case 2: case 3:
			ret_str += StringOfChar(_T('x'), rn); break;
		case 4: ret_str += "xl"; break;
		case 5: ret_str += "l";  break;
		case 6: case 7: case 8:
			ret_str.cat_sprintf(_T("l%s"), StringOfChar(_T('x'), rn - 5).c_str()); break;
		case 9: ret_str += "xc"; break;
		}
	}

	//1の位
	rn = n%10;
	switch (rn) {
	case 1: case 2: case 3:
		ret_str += StringOfChar(_T('i'), rn); break;
	case 4: ret_str += "iv"; break;
	case 5: ret_str += "v";  break;
	case 6: case 7: case 8:
		ret_str.cat_sprintf(_T("v%s"), StringOfChar(_T('i'), rn - 5).c_str()); break;
	case 9: ret_str += "ix"; break;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//指定文字列で囲まれた部分を削除
//---------------------------------------------------------------------------
void HtmConv::del_tag_block(TStringList *lst,
	const _TCHAR *start_wd,	//開始文字列
	const _TCHAR *end_wd)		//終了文字列
{
	bool flag = false;
	int i = 0;
	while (i<lst->Count) {
		UnicodeString s = lst->Strings[i];
		if (!flag) {
			if (StartsText(start_wd, s)) {
				flag = !ContainsText(s, end_wd);
				lst->Delete(i);
			}
			else {
				i++;
			}
		}
		else {
			flag = !ContainsText(s, end_wd);
			lst->Delete(i);
		}
	}
}

//---------------------------------------------------------------------------
//指定属性のブロックを削除
//---------------------------------------------------------------------------
void HtmConv::DelAtrBlock(TStringList *lst, UnicodeString anam, UnicodeString aval)
{
	UnicodeString tag;
	bool del_sw = false;
	int t_level = 0;
	int i = 0;
	while (i<lst->Count) {
		UnicodeString s = lst->Strings[i];
		if (!del_sw) {
			UnicodeString tnam = GetTag(s);
			if (contained_wd_i(_T("DIV|SECTION|ARTICLE|NAV|TABLE|UL|OL|DL"), tnam)) {
				if (SameStr(GetTagAtr(s, tnam, anam.c_str()), aval)) {
					tag = tnam;
					t_level = 1;
					del_sw = true;
				}
			}
		}
		else {
			UnicodeString tnam = GetTag(s);
			if (StartsStr("/", tnam)) {
				tnam.Delete(1, 1);
				if (SameText(tag, tnam)) t_level--;
			}
			else {
				if (SameText(tag, tnam)) t_level++;
			}
		}

		if (del_sw) {
			lst->Delete(i);
			if (t_level==0) del_sw = false;
		}
		else {
			i++;
		}
	}
}

//---------------------------------------------------------------------------
//タグ名を取得
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTag(UnicodeString s)
{
	UnicodeString ret_str;
	if (s.IsEmpty()) return ret_str;

	int flag = 0;
	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		switch (flag) {
		case 0:
			flag = (c!='<')? -1 : 1;
			break;
		case 1:
			if (c=='>' || c==' ')
				flag = -1;
			else
				ret_str += UpperCase(c);
			break;
		}
		if (flag==-1) break;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//要素から指定した属性の値を取得
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTagAtr(UnicodeString s,
	UnicodeString t,	//タグ名
	const _TCHAR *a)	//属性名
{
	if (s.IsEmpty()) return EmptyStr;

	UnicodeString retstr, tag, atr, vstr;
	int flag = 0;
	bool qflag;

	for (int i=1; i<=s.Length(); i++) {
		WideChar c = s[i];
		switch (flag) {
		case 0:
			if (c=='<') {
				tag  = EmptyStr;
				flag = 1;
			}
			break;
		case 1:
			if (c=='>') {
				flag = 0;
			}
			else if (c==' ') {	//タグ確定
				tag  = tag.Trim();
				atr  = EmptyStr;
				flag = 2;
			}
			else {
				tag.cat_sprintf(_T("%c"), c);
			}
			break;
		case 2:
			if (c=='=') {		//属性名確定
				atr  = atr.Trim();
				flag = 3;
			}
			else if (c==' ') {
				atr = EmptyStr;
			}
			else if (c=='>') {
				flag = 0;
			}
			else {
				atr.cat_sprintf(_T("%c"), c);
			}
			break;
		case 3:
			if (c=='>') {
				flag = 0;
			}
			else if (c!=' ') {	//属性値開始
				qflag = (c=='\"');
				if (qflag) vstr = EmptyStr; else vstr = c;
				flag = 4;
			}
			break;
		case 4:
			if ((qflag && c=='\"') || (!qflag && (c==' ' || c=='>'))) {
				if (SameText(tag, t) && SameText(atr, a)) {
					retstr = vstr;
					flag   = -1;	//属性値確定!
				}
				else {
					atr = EmptyStr;
					if (c=='>') flag = 0; else flag = 2;
				}
			}
			else {
				vstr.cat_sprintf(_T("%c"), c);
			}
			break;
		}

		if (flag==-1) break;
	}

	return retstr;
}

//---------------------------------------------------------------------------
//実体参照の解決
//---------------------------------------------------------------------------
UnicodeString HtmConv::RefEntity(UnicodeString s)
{
	if (!ContainsStr(s, "&")) return s;

	//文字実体参照から数値文字参照に一旦変換
	s = ChEntRef_to_NumChRef(s);

	//非ユニコードの場合、適当な文字に変換
	if (CodePage!=1200 && CodePage!=1201 && CodePage!=65000 && CodePage!=65001) {
		std::unique_ptr<TStringList> ent_lst(new TStringList());
		ent_lst->Text =
			"&#160;\t \n"		//&nbsp;
			"&#169;\t(c)\n"		//&copy;
			"&#174;\t(R)\n"		//&reg;
			"&#8194;\t \n"		//&ensp;
			"&#8195;\t　\n"		//&emsp;
			"&#8211;\t-\n"		//&ndash;
			"&#8212;\t−\n";	//&mdash;
		s = replace_str_by_list(s, ent_lst.get());
	}

	//数値文字参照を解決
	if (ContainsStr(s, "&")) {
		int stt = 0;
		int p = 1;
		int p0;
		while (p<=s.Length()) {
			if (s[p]=='&') {
				stt = 1;
				p++;
			}
			else {
				switch (stt) {
				case 1:
					if (s[p]=='#') {
						p0 = p + 1;
						stt = 2;
					}
					else {
						stt = 0;
					}
					p++;
					break;
				case 2:
					if (s[p]==';') {
						int cn = s.SubString(p0, p - p0).ToIntDef(0);
						p0 -= 2;
						s.Delete(p0, p - p0 + 1);
						if (cn>=32 && cn<65535)
							s.Insert((WideChar)cn, p0);
						else
							s.Insert("?", p0);
						p = p0 + 1;
						stt = 0;
					}
					else {
						p++;
					}
					break;
				default:
					p++;
				}
			}
		}
	}

	s = ReplaceStr(s, "&amp;", "&");

	return s;
}

//---------------------------------------------------------------------------
//TxtLineBuf を TxtBuf へ
//---------------------------------------------------------------------------
void HtmConv::FlushText(UnicodeString s)
{
	if (s.IsEmpty()) s = TxtLineBuf;
	if (s.IsEmpty()) return;

	if (!Skip) {
		if (!fXMP) s = RefEntity(s);
		TxtBuf->Add(s);
	}

	TxtLineBuf = EmptyStr;
}

//---------------------------------------------------------------------------
//罫線を追加
//---------------------------------------------------------------------------
void HtmConv::AddHR()
{
	if (ToMarkdown) {
		TxtBuf->Add(EmptyStr);
		TxtLineBuf = StringOfChar(_T('-'), LineWidth);
	}
	else {
		TxtLineBuf = StringOfChar(_T('─'), LineWidth/2);
	}

	FlushText();

	if (ToMarkdown) TxtBuf->Add(EmptyStr);
}

//---------------------------------------------------------------------------
//変換処理
//---------------------------------------------------------------------------
void HtmConv::Convert()
{
	TxtBuf->Clear();
	Title = Description = Keywords = EmptyStr;

	if (ToMarkdown) {
		HeaderStr = "#;##;###;####;#####;######";
		UlMarkStr = "- ";
	}
	else {
		UlMarkStr = "・";
	}

	std::unique_ptr<TStringList> lst(new TStringList());
	TStringList *tmp_buf = lst.get();

	//------------------------------
	//タグを行毎に分解
	//------------------------------
	for (int i=0; i<HtmBuf->Count; i++) {
		UnicodeString s = HtmBuf->Strings[i];
		if (!s.IsEmpty()) {
			for (;;) {
				int p0 = s.Pos('<');
				int p1 = s.Pos('>');
				if (p0>1) {
					int n = (p1>0)? ((p0<p1)? p0 - 1 : p1) : p0 - 1;
					tmp_buf->Add(s.SubString(1, n));
					s.Delete(1, n);
				}
				else if (p1>0) {
					tmp_buf->Add(s.SubString(1, p1));
					s.Delete(1, p1);
				}
				else break;
			}
			if (!s.IsEmpty()) tmp_buf->Add(s);
		}
		tmp_buf->Add("</>");	//改行マーク
	}

	//------------------------------
	//コメント部分を削除
	//------------------------------
	del_tag_block(tmp_buf, _T("<!--"), _T("-->"));

	//------------------------------
	//script、sytle 部分を削除
	//コメントアウトされていない場合の対策
	//------------------------------
	del_tag_block(tmp_buf, _T("<SCRIPT"), 	 _T("</SCRIPT>"));
	del_tag_block(tmp_buf, _T("<NOSCRIPT"), _T("</NOSCRIPT>"));
	del_tag_block(tmp_buf, _T("<STYLE"),	 _T("</STYLE>"));

	//php 部分を削除
	del_tag_block(tmp_buf, _T("<?PHP"),	 _T("?>"));

	//------------------------------
	//行分断されているタグを連結
	//------------------------------
	int i = 0;
	bool flag = false;
	while (i<tmp_buf->Count) {
		UnicodeString s = tmp_buf->Strings[i];
		if (!flag) {
			flag = (StartsStr('<', s) && !EndsStr('>', s));
			i++;
		}
		else {
			if (USAME_TS(s, "</>")) {
				tmp_buf->Delete(i);
			}
			else {
				if (EndsStr('>', s)) flag = false;
				tmp_buf->Strings[i - 1] = tmp_buf->Strings[i - 1] + " " + tmp_buf->Strings[i];
				tmp_buf->Delete(i);
			}
		}
	}

	//------------------------------
	//余分な半角スペース等を削除
	//------------------------------
	fPRE = fXMP = false;
	i = 0;
	while (i<tmp_buf->Count) {
		UnicodeString s = tmp_buf->Strings[i];
		UnicodeString tag = GetTag(s);
		if (fPRE) {
			if (USAME_TS(tag, "/PRE")) fPRE = false;
			i++;
		}
		else if (fXMP) {
			if (USAME_TS(tag, "/XMP")) fXMP = false;
			i++;
		}
		else if (!tag.IsEmpty()) {
			if		(USAME_TS(tag, "PRE")) fPRE = true;
			else if (USAME_TS(tag, "XMP")) fXMP = true;
			tmp_buf->Strings[i++] = Trim(s);
		}
		else {
			//連続した半角スペースを一つに
			s = Trim(s);
			while (ContainsStr(s, "  ")) s = ReplaceStr(s, "  ", " ");
			if (s.IsEmpty()) tmp_buf->Delete(i); else tmp_buf->Strings[i++] = s;
		}
	}

	//指定クラスのブロックを削除
	TStringDynArray cls_lst = SplitString(DelBlkCls, ";");
	for (int i=0; i<cls_lst.Length; i++) DelAtrBlock(tmp_buf, "class", cls_lst[i]);
	//指定IDのブロックを削除
	TStringDynArray id_lst = SplitString(DelBlkId, ";");
	for (int i=0; i<id_lst.Length; i++)  DelAtrBlock(tmp_buf, "id", id_lst[i]);

	//------------------------------
	//変換
	//------------------------------
	int  fTITLE   = 0;
	int  DL_level = 0;
	int  LI_level = 0;
	int  LI_No[100];
	char OL_type[100];
	int  fTR    = 0;
	int  fP_al  = 0;
	int  fTABLE = 0;
	int  fBLKQ	= 0;
	bool fRUBY  = false;
	bool fRP	= false;
	bool fRT	= false;
	bool pstHdr = false;
	fPRE = fXMP = false;
	Skip = false;

	TxtLineBuf = EmptyStr;
	UnicodeString HrefStr, tmpstr;
	std::unique_ptr<TStringList> tr_list(new TStringList());

	for (int i=0; i<tmp_buf->Count; i++) {
		UnicodeString lbuf = tmp_buf->Strings[i];
		UnicodeString tag  = GetTag(lbuf);
		if (tag.IsEmpty()) {
			pstHdr = false;
			if (fTITLE) {
				Title += lbuf;
			}
			else if (fPRE || fXMP || fRUBY) {
				if (fPRE) lbuf.Insert("    ", 1);
				TxtLineBuf += lbuf;
			}
			else if (!Skip) {
				//半角文字間にスペースを挿入
				do {
					if (TxtLineBuf.IsEmpty() || lbuf.IsEmpty()) break;
					if (ends_tchs(_T(" [*_"), TxtLineBuf)) break;
					if (StartsStr(' ', lbuf)) break;
					if (fRT) { fRT = false; break; }
					TxtLineBuf += " ";
				} while (0);

				//引用
				if (fBLKQ>0) lbuf.Insert(StringOfChar(_T('>'), fBLKQ) + " ", 1);
				TxtLineBuf += lbuf;
			}
		}
		else {
			//<P> がブレークされたか？
			if (fP_al>0 && contained_wd_i(_T("H1|H2|H3|H4|H5|H6|P|HR|DIV|CENTER|BLOCKQUOTE|TABLE|TR|TH|TD|UL|OL|DL|LI|PRE|XMP"), tag)) {
				FlushText();
				fP_al--;
			}

			//インデント文字列
			UnicodeString indent_str;
			int n;
			if (ToMarkdown) {
				n = ((LI_level>0)? LI_level - 1 : 0) + ((DL_level>0)? DL_level - 1 : 0);
				n *= 4;
				indent_str = StringOfChar(_T(' '), n);
				if (USAME_TS(tag, "DD")) indent_str += ":   ";
			}
			else {
				if (USAME_TS(tag, "DT"))
					n = (DL_level>0)? LI_level + DL_level - 1 : 0;
				else
					n = LI_level + DL_level;
				n *= 2;
				indent_str = StringOfChar(_T(' '), n);
			}

			//要素
			if (USAME_TS(tag, "/")) {
				if (fPRE || fXMP) {
					UnicodeString s = trim_ex(TxtLineBuf);
					if (s.IsEmpty()) TxtBuf->Add(TxtLineBuf); else FlushText();
				}
			}
			else if (USAME_TS(tag, "/XMP")) {
				FlushText();	fXMP = false;
			}
			else if (fXMP) {
				TxtLineBuf += lbuf;
			}
			else if (USAME_TS(tag, "P")) {
				FlushText();
				if (!pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				fP_al++;
			}
			else if (USAME_TS(tag, "/P")) {
				FlushText();
				if (fP_al>0) fP_al--;
			}
			else if (USAME_TS(tag, "BLOCKQUOTE")) {
				FlushText();
				if (!pstHdr) TxtBuf->Add((fBLKQ>0)? StringOfChar(_T('>'), fBLKQ) : EmptyStr);
				pstHdr = false;
				fBLKQ++;
			}
			else if (USAME_TS(tag, "/BLOCKQUOTE")) {
				FlushText();
				if (fBLKQ>0) fBLKQ--;
			}
			else if (contained_wd_i(_T("BR|BR/|BR /"), tag)) {
				if (!TxtLineBuf.IsEmpty()) FlushText(); else TxtBuf->Add(EmptyStr);
				TxtLineBuf += indent_str;
			}
			else if (USAME_TS(tag, "PRE")) {
				FlushText();	fPRE = true;
			}
			else if (USAME_TS(tag, "/PRE")) {
				FlushText();	fPRE = false;
			}
			else if (USAME_TS(tag, "XMP")) {
				FlushText();	fXMP = true;
			}
			else if (contained_wd_i(_T("CENTER|/CENTER"), tag)) {
				FlushText();
			}
			else if (contained_wd_i(_T("DIV|/DIV"), tag)) {
				FlushText();
				//指定クラスの前に罫線挿入
				if (USAME_TS(tag, "DIV") && !InsHrClass.IsEmpty()) {
					UnicodeString cnam = GetTagAtr(lbuf, tag, _T("CLASS"));
					if (!cnam.IsEmpty() && ContainsText(";" + InsHrClass + ";", ";" + cnam + ";")) AddHR();
				}
			}
			else if (contained_wd_i(_T("SECTION|/SECTION"), tag)) {
				FlushText();	if (USAME_TS(tag, "SECTION") && InsHrSection) AddHR();
			}
			else if (contained_wd_i(_T("ARTICLE|/ARTICLE"), tag)) {
				FlushText();	if (USAME_TS(tag, "ARTICLE") && InsHrArticle) AddHR();
			}
			else if (contained_wd_i(_T("NAV|/NAV"), tag)) {
				FlushText();	if (USAME_TS(tag, "NAV") && InsHrNav) AddHR();
			}
			else if (contained_wd_i(_T("H1|H2|H3|H4|H5|H6"), tag)) {
				FlushText();
				if (!pstHdr && TxtBuf->Count>0) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				//見出し文字
				TStringDynArray hlst = split_strings_semicolon(HeaderStr);
				int idx = tag.SubString(2, 1).ToIntDef(1) - 1;
				TxtLineBuf = (idx<hlst.Length)? hlst[idx] : EmptyStr;
			}
			else if (contained_wd_i(_T("/H1|/H2|/H3|/H4|/H5|/H6"), tag)) {
				FlushText();
			}
			else if (USAME_TS(tag, "HR")) {
				FlushText();  AddHR();
			}
			//箇条書き
			else if (USAME_TS(tag, "UL")) {
				FlushText();
				if (LI_level==0 && !pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				LI_No[++LI_level] = -1;
			}
			else if (USAME_TS(tag, "OL")) {
				FlushText();
				if (LI_level==0 && !pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				LI_No[++LI_level] = GetTagAtr(lbuf, tag, _T("START")).ToIntDef(1);
				tmpstr = GetTagAtr(lbuf, tag, _T("TYPE"));
				if (tmpstr.IsEmpty()) OL_type[LI_level] = '1'; else OL_type[LI_level] = tmpstr[1];
			}
			else if (contained_wd_i(_T("/UL|/OL"), tag)) {
				FlushText();
				LI_level = std::max(LI_level - 1, 0);
			}
			else if (USAME_TS(tag, "LI")) {
				FlushText();
				if (LI_level>0) {
					//順序付きリスト
					TxtLineBuf += indent_str;
					if (LI_No[LI_level]>0) {
						n = LI_No[LI_level];
						switch (OL_type[LI_level]) {
						case '1': tmpstr = n;			 				break;
						case 'a': tmpstr = int_to_alpha(n); 			break;
						case 'A': tmpstr = int_to_alpha(n).UpperCase(); break;
						case 'i': tmpstr = int_to_roman(n); 			break;
						case 'I': tmpstr = int_to_roman(n).UpperCase(); break;
						}
						tmpstr += ".";
						TxtLineBuf += tmpstr;
						LI_No[LI_level]++;
					}
					//順序無しリスト
					else {
						TxtLineBuf += UlMarkStr;
					}
				}
			}
			else if (USAME_TS(tag, "DL")) {
				FlushText();
				if (DL_level==0 && !pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				DL_level++;
			}
			else if (USAME_TS(tag, "/DL")) {
				FlushText();
				DL_level = std::max(DL_level - 1, 0);
			}
			else if (USAME_TS(tag, "DT")) {
				FlushText();
				TxtLineBuf += indent_str;
			}
			else if (USAME_TS(tag, "DD")) {
				if (DL_level>0) {
					FlushText();
					TxtLineBuf += indent_str;
				}
			}
			//テーブル
			else if (USAME_TS(tag, "TABLE")) {
				tr_list->Clear();
				FlushText();
				if (!pstHdr) TxtBuf->Add(EmptyStr);
				pstHdr = false;
				fTABLE++;
			}
			else if (USAME_TS(tag, "/TABLE")) {
				tr_list->Add(TxtLineBuf);
				if (ToMarkdown && tr_list->Count>0) {
					//セル幅を設定
					TStringDynArray wd_lst = split_strings_tab(tr_list->Strings[0]);
					for (int j=0; j<wd_lst.Length; j++) wd_lst[j] = "0";
					for (int j=0; j<tr_list->Count; j++) {
						TStringDynArray td_lst = split_strings_tab(tr_list->Strings[j]);
						for (int k=0; k<td_lst.Length && k<wd_lst.Length; k++) {
							wd_lst[k] = std::max(str_len_half(RefEntity(Trim(td_lst[k]))), wd_lst[k].ToIntDef(2));
						}
					}
					//行文字列作成
					for (int j=0; j<tr_list->Count; j++) {
						TStringDynArray td_lst = split_strings_tab(tr_list->Strings[j]);
						UnicodeString trbuf;
						for (int k=0; k<td_lst.Length; k++) {
							if (k>0) trbuf += " ";
							int wd = (k<wd_lst.Length)? wd_lst[k].ToIntDef(2) : str_len_half(td_lst[k]);
							trbuf += align_l_str(Trim(td_lst[k]), wd + 2);	//***
						}
						tr_list->Strings[j] = trbuf;
					}
					//上下罫線を挿入
					UnicodeString hrstr;
					for (int j=0; j<wd_lst.Length; j++) {
						if (j>0) hrstr += " ";
						hrstr += StringOfChar(_T('-'), wd_lst[j].ToIntDef(2) + 2);	//***
					}
					tr_list->Insert(0, hrstr);
					tr_list->Add(hrstr);
				}
				for (int j=0; j<tr_list->Count; j++) FlushText(tr_list->Strings[j]);
				if (fTABLE>0) fTABLE--;
			}
			else if (contained_wd_i(_T("CAPTION|/CAPTION"), tag)) {
				FlushText();
			}
			else if (contained_wd_i(_T("TD|TH"), tag)) {
				if (fTR>0) fTR = 0; else TxtLineBuf += "\t";
			}
			else if (USAME_TS(tag, "TR")) {
				if (!TxtLineBuf.IsEmpty()) {
					tr_list->Add(TxtLineBuf);
					TxtLineBuf = EmptyStr;
				}
				fTR = 1;
			}
			//ルビ
			else if (USAME_TS(tag, "RUBY")) {
				fRUBY = true;
				fRP   = fRT = false;
			}
			else if (USAME_TS(tag, "/RUBY")) {
				fRUBY = false;
			}
			else if (USAME_TS(tag, "RP")) {
				fRP   = true;
			}
			else if (USAME_TS(tag, "RT")) {
				fRT = true;
				if (!fRP) TxtLineBuf += "(";
			}
			else if (USAME_TS(tag, "/RT")) {
				if (!fRP) TxtLineBuf += ")";
			}
			//リンク
			else if (USAME_TS(tag, "A")) {
				HrefStr = GetTagAtr(lbuf, tag, _T("HREF"));
				if (ToMarkdown && !HrefStr.IsEmpty()) {
					TxtLineBuf += "[";
				}
				else {
					if (StartsText("file://", HrefStr)) {
						HrefStr = "file:///" + slash_to_yen(get_tkn_m(HrefStr, _T("file:///"), _T("#")));
					}
					else if (!StartsText("http", HrefStr) && !StartsText("mailto:", HrefStr) && !StartsStr("//", HrefStr)) {
						UnicodeString fnam = slash_to_yen(get_tkn(HrefStr, '#'));
						fnam = rel_to_abs(fnam, FileName);
						if (FileExists(fnam)) HrefStr.sprintf(_T("file:///%s"), fnam.c_str()); else HrefStr = EmptyStr;
					}
				}
			}
			else if (USAME_TS(tag, "/A")) {
				if (!HrefStr.IsEmpty()) {
					if (ToMarkdown)
						TxtLineBuf.cat_sprintf(_T("](%s)"), HrefStr.c_str());
					else
						TxtLineBuf.cat_sprintf(_T("（%s）"), HrefStr.c_str());
				}
				HrefStr = EmptyStr;
			}

			else if (USAME_TS(tag, "HEAD")) {
				Skip = true;
			}
			else if (contained_wd_i(_T("/HEAD|BODY"), tag)) {
				Skip   = false;
				fTITLE = 0;
			}
			else if (USAME_TS(tag, "TITLE")) {
				fTITLE ++;
			}
			else if (USAME_TS(tag, "/TITLE")) {
				fTITLE = 0;
			}
			else if (USAME_TS(tag, "META")) {
				UnicodeString mnam = GetTagAtr(lbuf, tag, _T("NAME"));
				UnicodeString cont = GetTagAtr(lbuf, tag, _T("CONTENT"));
				if (USAME_TI(mnam, "description"))
					Description = cont;
				else if (USAME_TI(mnam, "Keywords"))
					Keywords	= cont;
			}

			//Markdown の場合のみ
			else if (ToMarkdown) {
				if (contained_wd_i(_T("B|/B|EM|/EM|STRONG|/STRONG"), tag)) {
					TxtLineBuf += "**";
				}
				else if (contained_wd_i(_T("I|/I"), tag)) {
					TxtLineBuf += "*";
				}
				else if (contained_wd_i(_T("S|/S|DEL|/DEL"), tag)) {
					TxtLineBuf += "~~";
				}
				//画像
				else if (USAME_TS(tag, "IMG")) {
					UnicodeString alt_str = def_if_empty(GetTagAtr(lbuf, tag, _T("ALT")), _T("画像"));
					UnicodeString src_str = GetTagAtr(lbuf, tag, _T("SRC"));
					if (!src_str.IsEmpty()) TxtLineBuf.cat_sprintf(_T("![%s](%s)"), alt_str.c_str(), src_str.c_str());
				}
			}
		}
	}
	FlushText();

	//先頭空行の削除、連続空行の制限
	i = 0;
	int blkln = 0;
	while (i<TxtBuf->Count) {
		if (TxtBuf->Strings[i].IsEmpty()) {
			blkln++;
			if (blkln>DEF_BLANK_LN_LIMIT) TxtBuf->Delete(i); else i++;
		}
		else {
			blkln = 0;
			i++;
		}
	}

	//ヘッダ内容を挿入
	if (InsHdrInf) {
		int top = 0;
		if (!Description.IsEmpty()) {
			TxtBuf->Insert(0, "Description : " + Description);
			top++;
		}
		if (!Title.IsEmpty()) {
			Title = RefEntity(Title);
			TxtBuf->Insert(0, "Title : " + Title);
			top++;
		}
		if (top>0) TxtBuf->Insert(top, StringOfChar(_T('─'), LineWidth/2));
	}
}
//---------------------------------------------------------------------------
