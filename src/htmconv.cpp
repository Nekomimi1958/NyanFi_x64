//----------------------------------------------------------------------//
// HTML���e�L�X�g�ϊ������N���X											//
// Markdown�L�@�ɂ��Ή�													//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "htmconv.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// HtmConv �N���X
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
//URL���΃p�X�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString HtmConv::ToAbsUrl(UnicodeString url)
{
	if (!url.IsEmpty()) {
		if (!Scheme.IsEmpty() && TRegEx::IsMatch(url, "^//[^/]")) {
			url.Insert(Scheme, 1);
		}
		else if (!TopLevel.IsEmpty() && TRegEx::IsMatch(url, "^/[^/]")) {
			url.Insert(TopLevel, 1);
		}
		else if (!BaseUrl.IsEmpty() && is_rel_url(url)) {
			url.Insert(BaseUrl, 1);
		}
	}
	return url;
}

//---------------------------------------------------------------------------
//�t�@�C�����𑊑΃p�X�����΃p�X�֕ϊ�
// �h���C�u�����w�肳��Ă�����A�ϊ������ɂ��̂܂ܕԂ�
//---------------------------------------------------------------------------
UnicodeString HtmConv::rel_to_abs(
	UnicodeString fnam,		//�ϊ��Ώۃt�@�C����
	UnicodeString rnam)		//��t�@�C����
{
	if (fnam.IsEmpty() || !ExtractFileDrive(fnam).IsEmpty()) return fnam;

	UnicodeString dnam = ExtractFileDir(rnam);
	if (dnam.IsEmpty()) return fnam;

	for (;;) {
		if (!StartsStr("..\\", fnam)) break;
		int p = pos_r("\\", dnam); if (p==0) break;
		dnam = dnam.SubString(1, p - 1);
		fnam.Delete(1, 3);
	}

	return IncludeTrailingPathDelimiter(dnam) + fnam;
}

//---------------------------------------------------------------------------
//������ a�`z �ɂ��\�L�ɕϊ�
//  a�`z, aa�`az, ba�`bz...
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
//����(1�`999)�����[�}����(������)�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString HtmConv::int_to_roman(int n)
{
	UnicodeString ret_str;
	//100�̈�
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

	//10�̈�
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

	//1�̈�
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
//�w�蕶����ň͂܂ꂽ�������폜
//---------------------------------------------------------------------------
void HtmConv::del_tag_block(TStringList *lst,
	const _TCHAR *start_wd,	//�J�n������
	const _TCHAR *end_wd)		//�I��������
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
//�w�葮���̃u���b�N���폜
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
			if (contained_wd_i("DIV|SECTION|ARTICLE|NAV|TABLE|UL|OL|DL", tnam)) {
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
//�^�O�����擾
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
//�v�f����w�肵�������̒l���擾
//---------------------------------------------------------------------------
UnicodeString HtmConv::GetTagAtr(UnicodeString s,
	UnicodeString t,	//�^�O��
	const _TCHAR *a)	//������
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
			else if (c==' ') {	//�^�O�m��
				tag  = tag.Trim();
				atr  = EmptyStr;
				flag = 2;
			}
			else {
				tag.cat_sprintf(_T("%c"), c);
			}
			break;
		case 2:
			if (c=='=') {		//�������m��
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
			else if (c!=' ') {	//�����l�J�n
				qflag = (c=='\"');
				if (qflag) vstr = EmptyStr; else vstr = c;
				flag = 4;
			}
			break;
		case 4:
			if ((qflag && c=='\"') || (!qflag && (c==' ' || c=='>'))) {
				if (SameText(tag, t) && SameText(atr, a)) {
					retstr = vstr;
					flag   = -1;	//�����l�m��!
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
//���̎Q�Ƃ̉���
//---------------------------------------------------------------------------
UnicodeString HtmConv::RefEntity(UnicodeString s)
{
	if (!ContainsStr(s, "&")) return s;

	//�������̎Q�Ƃ��琔�l�����Q�ƂɈ�U�ϊ�
	s = ChEntRef_to_NumChRef(s);

	//�񃆃j�R�[�h�̏ꍇ�A�K���ȕ����ɕϊ�
	if (CodePage!=1200 && CodePage!=1201 && CodePage!=65000 && CodePage!=65001) {
		std::unique_ptr<TStringList> ent_lst(new TStringList());
		ent_lst->Text =
			"&#160;\t \n"		//&nbsp;
			"&#169;\t(c)\n"		//&copy;
			"&#174;\t(R)\n"		//&reg;
			"&#8194;\t \n"		//&ensp;
			"&#8195;\t�@\n"		//&emsp;
			"&#8211;\t-\n"		//&ndash;
			"&#8212;\t�|\n";	//&mdash;
		s = replace_str_by_list(s, ent_lst.get());
	}

	//���l�����Q�Ƃ�����
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
						if (ToMarkdown && (cn==60 || cn==62))
							s.Insert((cn==60)? "&lt;" : "&gt;", p0);
						else if (cn>=32 && cn<65535)
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
//TxtLineBuf �� TxtBuf ��
//---------------------------------------------------------------------------
void HtmConv::FlushText(UnicodeString s)
{
	if (s.IsEmpty()) s = TxtLineBuf;
	if (s.IsEmpty()) return;

	if (!Skip) {
		if (!fXMP) s = RefEntity(s);

		if (s.IsEmpty()) {
			TxtBuf->Add(EmptyStr);
		}
		else {
			if (!fPRE && !fXMP && ToMarkdown) {
				//���s(���p�X�y�[�X�~2)��t��
				if (!TRegEx::IsMatch(s, NO_CRSPC_PTN)) s += "  ";
			}
			//���p
			if (BQ_level>0) s = StringOfChar(_T('>'), BQ_level) + s;
			TxtBuf->Add(s);
		}
	}

	TxtLineBuf = EmptyStr;
}

//---------------------------------------------------------------------------
//�r����ǉ�
//---------------------------------------------------------------------------
void HtmConv::AddHR()
{
	UnicodeString s;
	if (ToMarkdown) {
		s = StringOfChar(_T('-'), LineWidth);
	}
	else {
		s = StringOfChar(_T('��'), LineWidth/2);
	}
	TxtBuf->Add(s);
}

//---------------------------------------------------------------------------
//�ϊ�����
//---------------------------------------------------------------------------
void HtmConv::Convert()
{
	TxtBuf->Clear();
	Title = Description = Keywords = EmptyStr;

	if (ToMarkdown) {
		HeaderStr = "# ;## ;### ;#### ;##### ;###### ";
		UlMarkStr = "- ";
	}
	else {
		UlMarkStr = "�E";
	}

	std::unique_ptr<TStringList> lst(new TStringList());
	TStringList *tmp_buf = lst.get();

	//------------------------------
	//�^�O���s���ɕ���
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
		tmp_buf->Add("</>");	//���s�}�[�N
	}

	//------------------------------
	//�R�����g�������폜
	//------------------------------
	del_tag_block(tmp_buf, _T("<!--"), _T("-->"));

	//------------------------------
	//script�Asytle �������폜
	//�R�����g�A�E�g����Ă��Ȃ��ꍇ�̑΍�
	//------------------------------
	del_tag_block(tmp_buf, _T("<SCRIPT"), 	 _T("</SCRIPT>"));
	del_tag_block(tmp_buf, _T("<NOSCRIPT"),  _T("</NOSCRIPT>"));
	del_tag_block(tmp_buf, _T("<STYLE"),	 _T("</STYLE>"));

	//php �������폜
	del_tag_block(tmp_buf, _T("<?PHP"),	 _T("?>"));

	//------------------------------
	//�s���f����Ă���^�O��A��
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
			if (SameStr(s, "</>")) {
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
	//�]���Ȕ��p�X�y�[�X�����폜
	//------------------------------
	fPRE = fXMP = false;
	i = 0;
	while (i<tmp_buf->Count) {
		UnicodeString s = tmp_buf->Strings[i];
		UnicodeString tag = GetTag(s);
		if (fPRE) {
			if (SameStr(tag, "/PRE")) fPRE = false;
			i++;
		}
		else if (fXMP) {
			if (SameStr(tag, "/XMP")) fXMP = false;
			i++;
		}
		else if (!tag.IsEmpty()) {
			if		(SameStr(tag, "PRE")) fPRE = true;
			else if (SameStr(tag, "XMP")) fXMP = true;
			tmp_buf->Strings[i++] = Trim(s);
		}
		else {
			//�A���������p�X�y�[�X�����
			s = Trim(s);
			while (ContainsStr(s, "  ")) s = ReplaceStr(s, "  ", " ");
			if (s.IsEmpty()) tmp_buf->Delete(i); else tmp_buf->Strings[i++] = s;
		}
	}

	//�w��N���X�̃u���b�N���폜
	TStringDynArray cls_lst = SplitString(DelBlkCls, ";");
	for (int i=0; i<cls_lst.Length; i++) DelAtrBlock(tmp_buf, "class", cls_lst[i]);
	//�w��ID�̃u���b�N���폜
	TStringDynArray id_lst = SplitString(DelBlkId, ";");
	for (int i=0; i<id_lst.Length; i++)  DelAtrBlock(tmp_buf, "id", id_lst[i]);

	//------------------------------
	//�ϊ�
	//------------------------------
	int  fTITLE   = 0;
	int  DL_level = 0;
	int  LI_level = 0;
	int  LI_No[100];
	char OL_type[100];
	int  fTR    = 0;
	int  fP_al  = 0;
	int  fTABLE = 0;
	bool fRUBY  = false;
	bool fRP	= false;
	bool fRT	= false;
	bool noSPC  = false;

	fPRE = fXMP = false;
	BQ_level = 0;
	Skip = false;

	TxtLineBuf = EmptyStr;
	UnicodeString HrefStr, tmpstr;
	std::unique_ptr<TStringList> tr_list(new TStringList());

	for (int i=0; i<tmp_buf->Count; i++) {
		UnicodeString lbuf = tmp_buf->Strings[i];
		UnicodeString tag  = GetTag(lbuf);
		if (tag.IsEmpty()) {
			if (fTITLE) {
				Title += lbuf;
			}
			else if (fPRE || fXMP || fRUBY) {
				TxtLineBuf += lbuf;
			}
			else if (!Skip) {
				//���p�����ԂɃX�y�[�X��}��
				do {
					if (TxtLineBuf.IsEmpty() || lbuf.IsEmpty()) break;
					if (EndsStr(" ", TxtLineBuf) || StartsStr(" ", lbuf)) break;
					if (noSPC) { noSPC = false; break; }
					if (fRT)   { fRT = false; break; }
					TxtLineBuf += " ";
				} while (0);
				TxtLineBuf += lbuf;
			}
		}
		else {
			//<P> ���u���[�N���ꂽ���H
			if (fP_al>0 && contained_wd_i("H1|H2|H3|H4|H5|H6|P|HR|DIV|CENTER|BLOCKQUOTE|TABLE|TR|TH|TD|UL|OL|DL|LI|PRE|XMP", tag)) {
				FlushText();
				fP_al--;
			}

			//�C���f���g������
			UnicodeString indent_str;
			int n;
			if (ToMarkdown) {
				n = ((LI_level>0)? LI_level - 1 : 0) + ((DL_level>0)? DL_level - 1 : 0);
				n *= 4;
				indent_str = StringOfChar(_T(' '), n);
				if (SameStr(tag, "DD")) indent_str += ":   ";
			}
			else {
				if (SameStr(tag, "DT"))
					n = (DL_level>0)? LI_level + DL_level - 1 : 0;
				else
					n = LI_level + DL_level;
				n *= 2;
				indent_str = StringOfChar(_T(' '), n);
			}

			//�v�f
			if (SameStr(tag, "/")) {
				if (fPRE || fXMP) {
					UnicodeString s = trim_ex(TxtLineBuf);
					if (s.IsEmpty() && !ToMarkdown) TxtBuf->Add(TxtLineBuf); else FlushText();
				}
			}
			else if (SameStr(tag, "/XMP")) {
				FlushText();	fXMP = false;
			}
			else if (fXMP) {
				TxtLineBuf += lbuf;
			}
			else if (SameStr(tag, "P")) {
				FlushText();
				TxtBuf->Add(EmptyStr);
				fP_al++;
			}
			else if (SameStr(tag, "/P")) {
				FlushText();
				if (fP_al>0) fP_al--;
			}
			else if (SameStr(tag, "BLOCKQUOTE")) {
				FlushText();
				if (ToMarkdown && BQ_level==0) TxtBuf->Add(EmptyStr);
				BQ_level++;
			}
			else if (SameStr(tag, "/BLOCKQUOTE")) {
				FlushText();
				DecLevel(BQ_level);
			}
			else if (contained_wd_i("BR|BR/|BR /", tag)) {
				if (!TxtLineBuf.IsEmpty()) FlushText(); else TxtBuf->Add(EmptyStr);
				TxtLineBuf += indent_str;
			}
			else if (SameStr(tag, "PRE")) {
				FlushText();
				if (ToMarkdown) {
					TxtBuf->Add(EmptyStr);
					TxtBuf->Add("```");
				}
				fPRE = true;
			}
			else if (SameStr(tag, "/PRE")) {
				FlushText();
				if (ToMarkdown) {
					TxtBuf->Add("```");
					TxtBuf->Add(EmptyStr);
				}
				fPRE = false;
			}
			else if (SameStr(tag, "XMP")) {
				FlushText();
				fXMP = true;
			}
			else if (contained_wd_i("CENTER|/CENTER", tag)) {
				FlushText();
			}
			else if (contained_wd_i("DIV|/DIV", tag)) {
				FlushText();
				//�w��N���X�̑O�Ɍr���}��
				if (SameStr(tag, "DIV") && !InsHrClass.IsEmpty()) {
					UnicodeString cnam = GetTagAtr(lbuf, tag, _T("CLASS"));
					if (!cnam.IsEmpty() && ContainsText(";" + InsHrClass + ";", ";" + cnam + ";")) AddHR();
				}
			}
			else if (contained_wd_i("SECTION|/SECTION", tag)) {
				FlushText();	if (SameStr(tag, "SECTION") && InsHrSection) AddHR();
			}
			else if (contained_wd_i("ARTICLE|/ARTICLE", tag)) {
				FlushText();	if (SameStr(tag, "ARTICLE") && InsHrArticle) AddHR();
			}
			else if (contained_wd_i("NAV|/NAV", tag)) {
				FlushText();	if (SameStr(tag, "NAV") && InsHrNav) AddHR();
			}
			else if (contained_wd_i("H1|H2|H3|H4|H5|H6", tag)) {
				FlushText();
				if (TxtBuf->Count>0) TxtBuf->Add(EmptyStr);
				//���o������
				TStringDynArray hlst = split_strings_semicolon(HeaderStr);
				int idx = tag.SubString(2, 1).ToIntDef(1) - 1;
				TxtLineBuf = (idx<hlst.Length)? hlst[idx] : EmptyStr;
				noSPC = true;
			}
			else if (contained_wd_i("/H1|/H2|/H3|/H4|/H5|/H6", tag)) {
				FlushText();
				if (ToMarkdown) TxtBuf->Add(EmptyStr);
			}
			else if (SameStr(tag, "HR")) {
				FlushText();  AddHR();
			}
			//�ӏ�����
			else if (SameStr(tag, "UL")) {
				FlushText();
				if (LI_level==0 && ToMarkdown) TxtBuf->Add(EmptyStr);
				LI_No[++LI_level] = -1;
			}
			else if (SameStr(tag, "OL")) {
				FlushText();
				if (LI_level==0 && ToMarkdown) TxtBuf->Add(EmptyStr);
				LI_No[++LI_level] = GetTagAtr(lbuf, tag, _T("START")).ToIntDef(1);
				tmpstr = GetTagAtr(lbuf, tag, _T("TYPE"));
				if (tmpstr.IsEmpty()) OL_type[LI_level] = '1'; else OL_type[LI_level] = tmpstr[1];
			}
			else if (contained_wd_i("/UL|/OL", tag)) {
				FlushText();
				DecLevel(LI_level);
				if (LI_level==0 && ToMarkdown) TxtBuf->Add(EmptyStr);
			}
			else if (SameStr(tag, "LI")) {
				FlushText();
				if (LI_level>0) {
					//�����t�����X�g
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
						if (ToMarkdown) tmpstr += " ";
						TxtLineBuf += tmpstr;
						LI_No[LI_level]++;
					}
					//�����������X�g
					else {
						TxtLineBuf += UlMarkStr;
					}
					noSPC = true;
				}
			}
			else if (SameStr(tag, "DL")) {
				FlushText();
				if (DL_level==0 && ToMarkdown) TxtBuf->Add(EmptyStr);
				DL_level++;
			}
			else if (SameStr(tag, "/DL")) {
				FlushText();
				DecLevel(DL_level);
			}
			else if (SameStr(tag, "DT")) {
				FlushText();
				TxtLineBuf += indent_str;
			}
			else if (SameStr(tag, "DD")) {
				if (DL_level>0) {
					FlushText();
					TxtLineBuf += indent_str;
				}
			}
			//�e�[�u��
			else if (SameStr(tag, "TABLE")) {
				tr_list->Clear();
				FlushText();
				TxtBuf->Add(EmptyStr);
				fTABLE++;
			}
			else if (SameStr(tag, "/TABLE")) {
				tr_list->Add(TxtLineBuf);
				if (ToMarkdown && tr_list->Count>0) {
					//�Z������ݒ�
					TStringDynArray wd_lst = split_strings_tab(tr_list->Strings[0]);
					for (int j=0; j<wd_lst.Length; j++) wd_lst[j] = "0";
					for (int j=0; j<tr_list->Count; j++) {
						TStringDynArray td_lst = split_strings_tab(tr_list->Strings[j]);
						for (int k=0; k<td_lst.Length && k<wd_lst.Length; k++) {
							wd_lst[k] = std::max(str_len_half(RefEntity(Trim(td_lst[k]))), wd_lst[k].ToIntDef(2));
						}
					}
					//�s������쐬
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
					//�㉺�r����}��
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
			else if (contained_wd_i("CAPTION|/CAPTION", tag)) {
				FlushText();
			}
			else if (contained_wd_i("TD|TH", tag)) {
				if (fTR>0) fTR = 0; else TxtLineBuf += "\t";
			}
			else if (SameStr(tag, "TR")) {
				if (!TxtLineBuf.IsEmpty()) {
					tr_list->Add(TxtLineBuf);
					TxtLineBuf = EmptyStr;
				}
				fTR = 1;
			}
			//���r
			else if (SameStr(tag, "RUBY")) {
				fRUBY = true;
				fRP   = fRT = false;
			}
			else if (SameStr(tag, "/RUBY")) {
				fRUBY = false;
			}
			else if (SameStr(tag, "RP")) {
				fRP   = true;
			}
			else if (SameStr(tag, "RT")) {
				fRT = true;
				if (!fRP) TxtLineBuf += "(";
			}
			else if (SameStr(tag, "/RT")) {
				if (!fRP) TxtLineBuf += ")";
			}
			//�����N
			else if (SameStr(tag, "A")) {
				HrefStr = ToAbsUrl(GetTagAtr(lbuf, tag, _T("HREF")));
				if (is_rel_url(HrefStr) && !FileName.IsEmpty()) {
					UnicodeString fnam = ReplaceStr(get_tkn(HrefStr, '#'), "/", "\\");
					UnicodeString anam = get_tkn_r(HrefStr, '#');
					fnam = rel_to_abs(fnam, FileName);
					if (FileExists(fnam)) {
						HrefStr.sprintf(_T("file:///%s"), ReplaceStr(fnam, "\\", "/").c_str());
						if (!anam.IsEmpty()) HrefStr.cat_sprintf(_T("#%s"), anam.c_str());
					}
				}
				if (ToMarkdown && !HrefStr.IsEmpty()) TxtLineBuf += "[";
				noSPC = true;
			}
			else if (SameStr(tag, "/A")) {
				if (!HrefStr.IsEmpty()) {
					if (ToMarkdown)
						TxtLineBuf.cat_sprintf(_T("](%s)"), HrefStr.c_str());
					else
						TxtLineBuf.cat_sprintf(_T("�i%s�j"), HrefStr.c_str());
				}
				HrefStr = EmptyStr;
			}

			else if (SameStr(tag, "HEAD")) {
				Skip = true;
			}
			else if (contained_wd_i("/HEAD|BODY", tag)) {
				Skip   = false;
				fTITLE = 0;
			}
			else if (SameStr(tag, "TITLE")) {
				fTITLE ++;
			}
			else if (SameStr(tag, "/TITLE")) {
				fTITLE = 0;
			}
			else if (SameStr(tag, "BASE")) {
				tmpstr = GetTagAtr(lbuf, tag, _T("HREF"));
				if (!tmpstr.IsEmpty()) {
					if (!EndsStr("/", tmpstr)) tmpstr += "/";
					BaseUrl  = tmpstr;
					TopLevel = TRegEx::Replace(BaseUrl, "(^https?://[\\w\\.\\-]+)(/.*)*", "$1");
					Scheme   = get_tkn(TopLevel, "//");
				}
			}
			else if (SameStr(tag, "META")) {
				UnicodeString mnam = GetTagAtr(lbuf, tag, _T("NAME"));
				UnicodeString cont = GetTagAtr(lbuf, tag, _T("CONTENT"));
				if 		(SameText(mnam, "description"))	Description = cont;
				else if (SameText(mnam, "Keywords"))	Keywords	= cont;
			}

			//Markdown �̏ꍇ�̂�
			else if (ToMarkdown) {
				if (contained_wd_i("B|/B|STRONG|/STRONG", tag)) {
					TxtLineBuf += "**";
					noSPC = true;
				}
				else if (contained_wd_i("I|/I|EM|/EM", tag)) {
					TxtLineBuf += "*";
					noSPC = true;
				}
				else if (contained_wd_i("S|/S|DEL|/DEL", tag)) {
					TxtLineBuf += "~~";
					noSPC = true;
				}
				//�摜
				else if (SameStr(tag, "IMG")) {
					UnicodeString alt_str = def_if_empty(GetTagAtr(lbuf, tag, _T("ALT")), "�摜");
					UnicodeString src_str = ToAbsUrl(GetTagAtr(lbuf, tag, _T("SRC")));
					TxtLineBuf.cat_sprintf(_T("![%s](%s)"), alt_str.c_str(), src_str.c_str());
				}
			}
		}
	}
	FlushText();

	//�擪��s�̍폜�A�A����s�̐���
	i = 0;
	int blkln = 0;
	while (i<TxtBuf->Count) {
		if (TxtBuf->Strings[i].IsEmpty()) {
			blkln++;
			if (blkln>DEF_BLANK_LN_LIMIT) {
				TxtBuf->Delete(i);
			}
			else if (ToMarkdown && blkln>1 && i>0 && i<TxtBuf->Count-1) {
				if (TRegEx::IsMatch(TxtBuf->Strings[i + 1], NO_CRSPC_PTN)) {
					if (TxtBuf->Strings[i - 1].IsEmpty()) TxtBuf->Delete(i); else i++;
				}
				else {
					TxtBuf->Strings[i++] = "<br>";
				}
			}
			else {
				i++;
			}
		}
		else {
			blkln = 0;
			i++;
		}
	}

	//�w�b�_���e��}��
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
		if (top>0) TxtBuf->Insert(top, StringOfChar(_T('��'), LineWidth/2));
	}
}
//---------------------------------------------------------------------------
