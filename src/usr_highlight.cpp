//----------------------------------------------------------------------//
// 構文強調表示															//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "usr_highlight.h"

//---------------------------------------------------------------------------
// 構文強調表示定義ファイルのクラス
//---------------------------------------------------------------------------
HighlightFile *UserHighlight = NULL;

//---------------------------------------------------------------------------
HighlightFile::HighlightFile(UnicodeString fnam) : UsrIniFile(fnam)
{
	HighlightTime = get_file_age(FileName);
	CurSection	  = EmptyStr;
	ErrorList	  = new TStringList();
	CheckValues();
};
//---------------------------------------------------------------------------
HighlightFile::~HighlightFile()
{
	delete ErrorList;
}

//---------------------------------------------------------------------------
//設定内容のチェック
//---------------------------------------------------------------------------
void HighlightFile::CheckValues()
{
	ErrorList->Clear();
	for (int i=0; i<SectionList->Count; i++) {
		UnicodeString sct = SectionList->Strings[i];
		TStringList *klist = (TStringList*)SectionList->Objects[i];
		for (int j=0; j<klist->Count; j++) {
			UnicodeString key  = klist->Names[j];
			UnicodeString vstr = klist->ValueFromIndex[j];

			bool err = false;
			if (TRegEx::IsMatch(key, "Ptn\\d*$"))
				 err = !chk_RegExPtn(vstr);
			else if (EndsText("Col", key))
				err = !TRegEx::IsMatch(vstr, "^[0-9a-fA-F]{6}");
			else if (EndsText("File",key))
				err = !file_exists(to_absolute_name(vstr));

			if (err) {
				ErrorList->Add(UnicodeString().sprintf(_T("    [%s] %s=%s"),
					sct.c_str(), key.c_str(), vstr.c_str()));
			}
		}
	}

	if (ErrorList->Count>0) ErrorList->Add("    設定に誤りがあります。");
}

//---------------------------------------------------------------------------
//タイムスタンプが変化していたら再読み込み
//---------------------------------------------------------------------------
bool HighlightFile::Recycle()
{
	CurSection = EmptyStr;

	TDateTime ft = get_file_age(FileName);
	if (HighlightTime!=ft) {
		Reload();
		CheckValues();
		HighlightTime = ft;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//構文強調表示定義ファイルから対応セクションを取得
//---------------------------------------------------------------------------
bool HighlightFile::GetSection(
	UnicodeString fnam,		//ファイル名
	bool is_clip,			//クリップボード		(default = false)
	bool is_log,			//タスクログ			(default = false)
	bool is_h2t)			//HTML→TEXT変換モード	(default = false)
{
	CurSection = EmptyStr;
	UnicodeString fext = get_extension(fnam);

	for (int i=0; i<SectionList->Count; i++) {
		UnicodeString sct = SectionList->Strings[i];
		if (   (is_clip && SameText(sct, "CLIPBOARD"))
			|| (is_log  && SameText(sct, "TASKLOG"))
			|| (is_h2t  && test_FileExt(fext, FEXT_HTML) && SameText(sct, "HTML2TEXT"))
			|| (fext.IsEmpty() && get_tkn(sct, ':')=='.')
			|| (test_FileExt(fext, get_tkn(sct, ':'))))
		{
			CurSection = sct;
		}
		if (CurSection.IsEmpty()) continue;

		if (!is_clip && !is_log) {
			//対象パス
			TStringDynArray path_lst = split_strings_semicolon(ReadKeyStr(_T("TargetPath")));
			if (path_lst.Length>0) {
				bool ok = false;
				for (int i=0; i<path_lst.Length && !ok; i++) {
					if (path_lst[i].IsEmpty()) continue;
					if (StartsText(cv_env_str(path_lst[i]), fnam)) ok = true;
				}
				if (!ok) CurSection = EmptyStr;
			}
			//除外対象パス
			if (!CurSection.IsEmpty()) {
				TStringDynArray path_lst = split_strings_semicolon(ReadKeyStr(_T("ExcludePath")));
				if (path_lst.Length>0) {
					bool ok = true;
					for (int i=0; i<path_lst.Length && ok; i++) {
						if (path_lst[i].IsEmpty()) continue;
						if (StartsText(cv_env_str(path_lst[i]), fnam)) ok = false;
					}
					if (!ok) CurSection = EmptyStr;
				}
			}
			//対象名
			if (!CurSection.IsEmpty()) {
				TStringDynArray name_lst = split_strings_semicolon(ReadKeyStr(_T("TargetName")));
				if (name_lst.Length>0) {
					bool ok = false;
					UnicodeString bnam = get_base_name(fnam);
					for (int i=0; i<name_lst.Length && !ok; i++) {
						if (name_lst[i].IsEmpty()) continue;
						if (StartsText(name_lst[i], bnam)) ok = true;
					}
					if (!ok) CurSection = EmptyStr;
				}
			}
		}

		//マッチ
		if (!CurSection.IsEmpty()) break;
	}

	return !CurSection.IsEmpty();
}

//---------------------------------------------------------------------------
UnicodeString HighlightFile::ReadKeyStr(const _TCHAR *key)
{
	if (CurSection.IsEmpty()) return EmptyStr;

	return ReadString(CurSection, key);
}
//---------------------------------------------------------------------------
int HighlightFile::ReadKeyInt(const _TCHAR *key,
	int def)	//デフォルト値	(default = 0)
{
	if (CurSection.IsEmpty()) return 0;
	return ReadInteger(CurSection, key, def);
}
//---------------------------------------------------------------------------
bool HighlightFile::ReadKeyBool(const _TCHAR *key,
	bool def)	//デフォルト値	(default = false)
{
	if (CurSection.IsEmpty()) return false;

	return ReadBool(CurSection, key, def);
}

//---------------------------------------------------------------------------
//正規表現パターン
//---------------------------------------------------------------------------
UnicodeString HighlightFile::ReadRegExPtn(const _TCHAR *key)
{
	if (CurSection.IsEmpty()) return EmptyStr;

	UnicodeString ptn = ReadString(CurSection, key);
	return (chk_RegExPtn(ptn)? ptn : EmptyStr);
}
//---------------------------------------------------------------------------
//16進2桁RRGGBB形式を TColor として取得
//---------------------------------------------------------------------------
TColor HighlightFile::ReadColorRGB6H(const _TCHAR *key,
	TColor def)	//デフォルト値	(default = clBlack)
{
	UnicodeString s = ReadString(CurSection, key, EmptyStr);
	return TRegEx::IsMatch(s, "^[0-9a-fA-F]{6}")? xRRGGBB_to_col(s) : def;
}

//---------------------------------------------------------------------------
//コメント識別文字列を取得
//---------------------------------------------------------------------------
bool HighlightFile::GetCommentList(
	UnicodeString fnam,		//ファイル名
	TStringList *ln_lst,	//１行コメント
	TStringList *bgn_lst,	//ブロック開始
	TStringList *end_lst,	//ブロック終了
	bool is_h2t,			//HTML→TEXT変換モード	(default = false)
	bool only_def)			//デフォルトのみを取得	(default = false)
{
	if (fnam.IsEmpty()) return false;

	//ユーザ定義から取得
	if (!only_def && GetSection(fnam)) {
		UnicodeString key;
		for (int i=1; ; i++) {
			UnicodeString vstr = ReadKeyStr(key.sprintf(_T("Comment%u"), i).c_str());
			if (vstr.IsEmpty()) break;
			ln_lst->Add(vstr);
		}
		for (int i=1; ; i++) {
			UnicodeString b_str = ReadKeyStr(key.sprintf(_T("CommentBgn%u"), i).c_str());
			UnicodeString e_str = ReadKeyStr(key.sprintf(_T("CommentEnd%u"), i).c_str());
			if (b_str.IsEmpty() || e_str.IsEmpty()) break;
			bgn_lst->Add(b_str);
			end_lst->Add(e_str);
		}
	}
	//デフォルト
	else {
		UnicodeString fext = get_extension(fnam);

		if (test_FileExt(fext, FEXT_NRM_REM)) {
			ln_lst->Add("//");
			bgn_lst->Add("/*");	end_lst->Add("*/");
		}
		else if (test_FileExt(fext, FEXT_SMCL_REM)) {
			ln_lst->Add(";");
		}
		else if (test_FileExt(fext, FEXT_PASCAL)) {
			ln_lst->Add("//");
			bgn_lst->Add("{");	end_lst->Add("}");
			bgn_lst->Add("(*");	end_lst->Add("*)");
		}
		//個別
		else if (test_FileExt(fext, ".ahk")) {
			ln_lst->Add(";");
			bgn_lst->Add("/*");	end_lst->Add("*/");
		}
		else if (test_FileExt(fext, ".au3")) {
			ln_lst->Add(";");
			bgn_lst->Add("#comments-start");	end_lst->Add("#comments-end");
			bgn_lst->Add("#cs");	end_lst->Add("#ce");
		}
		else if (test_FileExt(fext, ".awk.pl.pm.pod.sh")) {
			ln_lst->Add("#");
		}
		else if (test_FileExt(fext, ".bat.cmd")) {
			ln_lst->Add("@REM");
			ln_lst->Add("REM");
			ln_lst->Add("::");
		}
		else if (test_FileExt(fext, ".css.drc")) {
			bgn_lst->Add("/*");	end_lst->Add("*/");
		}
		else if (test_FileExt(fext, ".fs")) {
			ln_lst->Add("//");
			bgn_lst->Add("(*");	end_lst->Add("*)");
		}
		else if (test_FileExt(fext, ".hs")) {
			ln_lst->Add("--");
			bgn_lst->Add("{-");	end_lst->Add("-}");
		}
		else if (test_FileExt(fext, ".lisp.lsp")) {
			ln_lst->Add(";");
			bgn_lst->Add("#|");	end_lst->Add("|#");
		}
		else if (test_FileExt(fext, ".lua")) {
			ln_lst->Add("--");
		}
		else if (test_FileExt(fext, ".m")) {
			ln_lst->Add("%");
			bgn_lst->Add("%{");	end_lst->Add("%}");
		}
		else if (test_FileExt(fext, ".rb")) {
			ln_lst->Add("#");
			bgn_lst->Add("=begin");	end_lst->Add("=end");
		}
		else if (test_FileExt(fext, ".sql")) {
			ln_lst->Add("--");
			bgn_lst->Add("/*");	end_lst->Add("*/");
		}
		else if (test_FileExt(fext, ".bas.vb.vbs.mac")) {
			ln_lst->Add("'");
		}
		else if (test_FileExt(fext, ".php")) {
			ln_lst->Add("//");
			ln_lst->Add("#");
			bgn_lst->Add("/*");		end_lst->Add("*/");
			bgn_lst->Add("<!--");	end_lst->Add("-->");
		}
		else if (test_FileExt(fext, ".ps1.psm1.psd1")) {
			ln_lst->Add("#");
			bgn_lst->Add("<#");	end_lst->Add("#>");
		}
		else if (test_FileExt(fext, ".py")) {
			ln_lst->Add("#");
			bgn_lst->Add("\"\"\""); end_lst->Add("\"\"\"");
			bgn_lst->Add("\'\'\'"); end_lst->Add("\'\'\'");
		}
		else if (test_FileExt(fext, ".st")) {
			bgn_lst->Add("\"");	end_lst->Add("\"");
		}
		else if (test_FileExt(fext, ".vhd")) {
			ln_lst->Add("--");
		}
		//その他
		else if (test_FileExt(fext, ".dsp.dsw.nodoka.yml.yaml.PspScript")) {
			ln_lst->Add("#");
		}
		else if ((!is_h2t && test_FileExt(fext, FEXT_HTML)) || test_FileExt(fext, FEXT_XML)) {
			bgn_lst->Add("<!--");	end_lst->Add("-->");
		}
		else if (test_FileExt(fext, ".txt") && StartsStr("awstats", ExtractFileName(fnam))) {
			ln_lst->Add("#");
		}
	}

	return (ln_lst->Count>0 || bgn_lst->Count>0);
}

//---------------------------------------------------------------------------
//リストボックスの指定行のコメント開始位置を取得
//---------------------------------------------------------------------------
int HighlightFile::GetCommentPos(
	UnicodeString fnam,		//ファイル名
	TListBox *lp, int idx,	//リストボックス、インデックス
	bool en_mlt)			//複数行コメントに対応	(default = true)
{
	int p = 0;

	std::unique_ptr<TStringList> ln_lst(new TStringList());		//１行コメント
	std::unique_ptr<TStringList> bg_lst(new TStringList());		//ブロック開始
	std::unique_ptr<TStringList> ed_lst(new TStringList());		//ブロック終了

	if (GetCommentList(fnam, ln_lst.get(), bg_lst.get(), ed_lst.get())) {
		UnicodeString s = lp->Items->Strings[idx];

		//行コメント
		int p_ln = 0;
		for (int i=0; i<ln_lst->Count && p_ln==0; i++) p_ln = pos_i(ln_lst->Strings[i], s);

		//ブロックコメント(行全体)
		bool flag = false;
		if (en_mlt && p_ln!=1) {
			for (int i=0; i<bg_lst->Count && !flag; i++) {
				UnicodeString bg_str = bg_lst->Strings[i];
				UnicodeString ed_str = ed_lst->Strings[i];
				bool is_same = SameStr(bg_str, ed_str);
				UnicodeString lbuf	 = Trim(s);
				//ブロックコメント内か?
				if (!ContainsStr(lbuf, bg_str) && !ContainsStr(lbuf, ed_str)) {
					int range = 50;	//*** チェック範囲
					int idxj = idx - 1;
					//上方チェック
					for (int j=0; j<range && !flag && idx>=0; j++,idxj--) {
						UnicodeString jbuf = Trim(lp->Items->Strings[idxj]);
						if (EndsStr(ed_str, jbuf)) break;
						if (StartsStr(bg_str, jbuf)) flag = true;
					}
					//下方チェック
					if (flag) {
						flag = false;
						idxj = idx + 1;
						for (int j=0; j<range && !flag && idxj<lp->Count; j++,idxj++) {
							UnicodeString jbuf = Trim(lp->Items->Strings[idxj]);
							if (!is_same && StartsStr(bg_str, jbuf)) break;
							if (EndsStr(ed_str, jbuf)) flag = true;
						}
					}
				}
				//ブロックコメント開始行か?
				else if (remove_top_s(lbuf, bg_str)) {
					flag = EndsStr(ed_str, lbuf) || !ContainsStr(lbuf, ed_str);
				}
				//ブロックコメント終了行か?
				else if (remove_end_s(lbuf, ed_str)) {
					flag = StartsStr(bg_str, lbuf) || !ContainsStr(lbuf, bg_str);
				}
			}
		}

		if (flag) {
			p = 1;
		}
		else {
			//１行に収まるブロックコメント
			int p_mlt = 0;
			for (int i=0; i<bg_lst->Count && p_mlt==0; i++) {
				int p_end = EndsStr(ed_lst->Strings[i], s)? (s.Length() - ed_lst->Strings[i].Length() + 1) : 0;
				if (p_end>0) {
					p_mlt = s.Pos(bg_lst->Strings[i]);
					if (p_mlt==p_end) p_mlt = 0;
				}
			}

			if (p_ln>0 && (p_mlt==0 || p_ln<p_mlt)) {
				UnicodeString fext = get_extension(fnam);
				p = p_ln;
				if (test_FileExt(fext, FEXT_NRM_REM FEXT_PASCAL _T(".fs.php"))) {
					int p_u = s.Pos("://");
					if (p_u>0 && p==p_u + 1) p = 0;
				}
				else if (test_FileExt(fext, ".ps1.psm1") && p>0 && (s.Pos("<#") || s.Pos("#>"))) {
					p = 0;
				}
			}

			if (p==0) p = p_mlt;
		}
	}

	return p;
}

//---------------------------------------------------------------------------
//拡張子依存のデフォルト見出しパターンを取得
//---------------------------------------------------------------------------
UnicodeString HighlightFile::GetDefHeadlnPtn(UnicodeString fext)
{
	return (
		test_FileExt(fext, ".bat.cmd.qbt") ? "^:[^:]+" :
		test_FileExt(fext, ".dfm")		   ? "^\\s*object\\s" :
		test_FileExt(fext, ".eml")		   ? "^Subject:" :
		test_FileExt(fext, ".hsp")		   ? "^\\*\\w+" :
		test_FileExt(fext, ".md")		   ? "^#+\\s" :
		test_FileExt(fext, ".pod.pl.pm")   ? "^=head\\d\\b" : "");
}
//---------------------------------------------------------------------------
//見出しパターンを取得
//---------------------------------------------------------------------------
UnicodeString HighlightFile::GetHeadlinePtn(
	UnicodeString fnam,		//ファイル名
	TStringList *hl_lst,	//拡張子対応見出しリスト	(default = NULL)
	bool only_def)			//デフォルトのみを取得		(default = false)
{
	UnicodeString h_ptn;
	UnicodeString fext = get_extension(fnam);
	//ユーザ定義
	if (!only_def && GetSection(fnam)) h_ptn = ReadRegExPtn(_T("HeadlinePtn"));

	//拡張子関連付け
	if (h_ptn.IsEmpty() && hl_lst && !fext.IsEmpty())
		h_ptn = hl_lst->Values[get_tkn_r(fext, '.').LowerCase()];

	//デフォルト
	if (h_ptn.IsEmpty()) h_ptn = GetDefHeadlnPtn(fext);
	if (h_ptn.IsEmpty()) {
		h_ptn = test_FileExt(fext, ".ini.inf.reg.url") ? "^\\[.+?\\]" :
				test_FileExt(fext, FEXT_HTML)		   ? "<[hH][1-6]" : "";
	}

	return (chk_RegExPtn(h_ptn)? h_ptn : EmptyStr);
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//以下はデフォルトの設定を取得
//---------------------------------------------------------------------------
//予約語パターンを取得
//---------------------------------------------------------------------------
UnicodeString GetDefReservedPtn(
	UnicodeString fext,		//拡張子
	bool &sns_case, 		//大小文字を区別
	bool is_xml,			//対象がXML
	bool is_log,			//対象がタスクログ
	bool is_h2t)			//HTML→TEXT変換モード
{
	UnicodeString ret_str;

	sns_case = true;

	if (is_xml) {
		ret_str  = "<[^<>]+(>|\\s)";
		sns_case = false;
	}
	else if (is_log) {
		ret_str.sprintf(_T("%s"),
			_T("\\s(ALARM|BGIMG|CHMOD|CLIPBOARD|CMPDEL|COPY|CREATE|CVIMG|DELETE|DLEXIF|DROP|EXECUTE|EXTRACT|")
			_T("FILEINFO|FTPGET|FTPPUT|GIFBMP|HTM2TX|JOIN|LIST|LOAD|MOVE|NOPTM|PACK|PLAY|RCVMSG|REMOVE|")
			_T("RENAME|SAVE|TEST|TIME|TIMER|UNDO|UNPACK|WATCH|WORKLIST)\\s|\\s(OK:|SKIP:)"));
	}
	else if (test_FileExt(fext, ".ahk")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(gosub|goto|if|else|loop)\\b|")
			_T("^\\s*#\\w+\\b"));
	}
	else if (test_FileExt(fext, ".as")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(as|break|case|catch|class|const|continue|default|delete|do|else|extends|false|")
			_T("finally|for|function|if|implements|import|in|instanceof|interface|internal|is|native|")
			_T("new|null|package|private|protected|public|return|super|switch|this|throw|to|true|try|")
			_T("typeof|use|var|void|while|with)\\b"));
	}
	else if (test_FileExt(fext, ".au3")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(ContinuelOop|Dim|Global|Local|Const|Do|Until|Exit|Exitloop|For|Next|Func|Return|")
			_T("Endfunc|If|Then|Elseif|Else|Endif|Redim|Select|Case|Endselect|While|Wend)\\b|")
			_T("^\\s*#\\w+\\b"));
		sns_case = false;
	}
	else if (test_FileExt(fext, ".awk")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(BEGIN|break|close|continue|delete|do|else|END|exit|for|function|getline|gsub|if|index|")
			_T("length|match|next|print|printf|return|split|sprintf|sub|substr|system|tolower|toupper|while)\\b"));
	}
	else if (test_FileExt(fext, ".bat.cmd.qbt")) {
		ret_str  = "(@|\\b)(call|cls|cmd|echo|errorlevel|exit|for|goto|if|pause|rem|set|shift|start)\\b";
		sns_case = false;
	}
	else if (test_FileExt(fext, ".cs")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|as|async|await|base|bool|break|byte|case|catch|char|checked|class|")
			_T("const|continue|decimal|default|delegate|do|double|else|enum|event|explicit|extern|")
			_T("false|finally|fixed|float|for|foreach|goto|if|implicit|in|int|interface|internal|")
			_T("is|lock|long|namespace|new|null|object|operator|out|override|params|private|protected|")
			_T("public|readonly|ref|return|sbyte|sealed|short|sizeof|stackalloc|static|string|struct|")
			_T("switch|this|throw|true|try|typeof|uint|ulong|unchecked|unsafe|ushort|using|virtual|")
			_T("volatile|void|while)\\b|")
			_T("^\\s*#\\w+\\b"));
	}
	else if (test_FileExt(fext, FEXT_C_SH _T(".d.idl"))) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(alignas|alignof|_{0,2}asm|auto|_automated|axiom|_Bool|bool|false|true|")
			_T("break|case|catch|_{0,2}cdecl|char|char(16|32)_t|class|__classid|__classmethod|")
			_T("__closure|_Complex|concept|concept_map|const|const_cast|constexpr|continue|__declspec|")
			_T("decltype|default|delete|__delphirtti|deprecated|__dispid|do|double|dynamic_cast|")
			_T("enum|__except|explicit|export|_export|__export|extern|_fastcall|__fastcall|final|")
			_T("__finally|float|for|friend|goto|if|else|_{1,2}import|(__)?inline|")
			_T("int|__int(8|16|32|64)|late_check|long|__msfastcall|__msreturn|mutable|namespace|")
			_T("new|noreturn|nullptr|operator|_{0,2}pascal|private|__property|protected|")
			_T("public|__published|register|reinterpret_cast|requires|restrict|return|__rtti|short|")
			_T("signed|sizeof|static|static_assert|static_cast|_{1,2}stdcall|struct|switch|")
			_T("template|this|thread_local|__thread|throw|(__)?try|typedef|typeid|typename|union|")
			_T("unsigned|using|virtual|void|volatile|wchar_t|while)\\b|")
			_T("^\\s*#\\w+\\b"));
	}
	else if (test_FileExt(fext, ".dfm")) {
		ret_str = "\\b(object|item|end)\\b";
	}
	else if (test_FileExt(fext, ".dsp")) {
		ret_str = "^!\\w+\\b";
	}
	else if (test_FileExt(fext, ".eml")) {
		ret_str  = "^(Bcc|Cc|Date|From|Subject|To):";
		sns_case = false;
	}
	else if (test_FileExt(fext, ".fs")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|and|as|asr|assert|base|begin|class|default|delegate|do|done|downcast|")
			_T("downto|elif|else|end|exception|extern|false|finally|for|fun|function|global|if|")
			_T("in|inherit|inline|interface|internal|land|lazy|let|let!|lor|lsl|lsr|lxor|match|")
			_T("member|mod|module|mutable|namespace|new|not|null|of|open|or|override|private|public|")
			_T("rec|return!?|select|sig|static|struct|then|to|try|type|upcast|use!?|val|void|when|")
			_T("while|with|yield!?)\\b|")
			_T("^\\s*#\\w+\\b"));
	}
	else if (test_FileExt(fext, ".go")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(break|case|chan|const|continue|default|defer|else|fallthrough|for|func|go|goto|")
			_T("if|import|interface|map|package|range|return|select|struct|switch|type|var)\\b"));
	}
	else if (test_FileExt(fext, ".hs")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(as|case|of|class|data|default|deriving|do|forall|foreign|hiding|if|then|else|")
			_T("import|infix|infixl|infixr|instance|let|in|mdo|module|newtype|qualified|type|where)\\b"));
	}
	else if (test_FileExt(fext, ".hsp")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(await|break|continue|else|end|exec|exgoto|foreach|gosub|goto|if|loop|on|onclick|")
			_T("oncmd|onerror|onexit|onkey|repeat|return|run|stop|wait|array|double|int|str|var)\\b|")
			_T("^\\s*#\\w+\\b"));
	}
	else if (test_FileExt(fext, ".js.jsx.ts")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|boolean|break|byte|case|catch|char|class|const|continue|default|do|")
			_T("double|else|extends|false|final|finally|float|for|function|goto|if|implements|import|")
			_T("in|instanceof|int|interface|long|native|new|null|package|private|protected|public|")
			_T("return|short|static|super|switch|synchronized|this|throw|throws|transient|true|")
			_T("try|typeof|var|void|while|with)\\b"));
	}
	else if (test_FileExt(fext, ".java")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|assert|boolean|break|byte|case|catch|char|class|const|continue|default|")
			_T("do|double|else|enum|extends|final|finally|float|for|goto|if|implements|import|instanceof|")
			_T("int|interface|long|native|new|package|private|protected|public|return|short|static|")
			_T("strictfp|super|switch|synchrnized|this|throw|throws|transient|try|void|volatile|while)\\b"));
	}
	else if (test_FileExt(fext, ".json")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(true|false|null)\\b"));
	}
	else if (test_FileExt(fext, ".kt.kts")) {
		ret_str.sprintf(_T("%s"),
			//Hard
			_T("\\b(as|break|class|continue|do|else|false|for|fun|if|in|interface|is|null|object|package|")
			_T("return|super|this|throw|true|try|typealias|typeof|val|var|when|while|")
			//Soft
			_T("by|catch|constructor|delegate|dynamic|field|file|finally|get|import|init|param|property|")
			_T("receiver|set|setparam|where)\\b|")
			//Modifier
			_T("\\b(actual|abstract|annotation|companion|const|crossinline|data|enum|expect|external|final|")
			_T("infix|inline|inner|internal|lateinit|noinline|open|operator|out|override|private|")
			_T("protected|public|reified|sealed|suspend|tailrec|vararg)\\s"));
	}
	else if (test_FileExt(fext, ".lua")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(and|break|do|else|elseif|end|false|for|function|if|in|local|nil|not|or|repeat|")
			_T("return|then|true|until|while)\\b"));
	}
	else if (test_FileExt(fext, FEXT_PASCAL)) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(absolute|abstract|and|array|as|asm|assembler|at|automated|begin|case|cdecl|")
			_T("class|const|constructor|default|destructor|dispid|div|do|downto|dynamic|else|end|")
			_T("except|exports|external|file|finalization|finally|for|forward|function|goto|if|")
			_T("implementation|in|index|inherited|initialization|inline|interface|is|label|library|")
			_T("message|mod|name|nil|nodefault|not|object|of|on|or|override|packed|pascal|private|")
			_T("procedure|program|property|protected|public|published|raise|read|record|register|")
			_T("repeat|resident|set|shl|shr|stdcall|stored|string|then|threadvar|to|try|type|unit|")
			_T("until|uses|var|virtual|while|with|write|xor)\\b"));
		sns_case = false;
	}
	else if (test_FileExt(fext, ".php") && !is_h2t) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|and|array|as|break|case|catch|cfunction|class|clone|const|continue|")
			_T("declare|default|die|do|echo|else|elseif|empty|enddeclare|endfor|endforeach|endif|")
			_T("endswitch|endwhile|eval|exception|exit|extends|extends|final|for|foreach|function|")
			_T("global|if|implements|include|include_once|interface|isset|list|new|old_function|")
			_T("or|php_user_filter|php_user_filter|print|private|protected|public|require|require_once|")
			_T("return|static|switch|this|throw|try|unset|use|var|while|xor|__CLASS__|__FILE__|")
			_T("__FUNCTION__|__LINE__|__METHOD__)\\b"));
	}
	else if (test_FileExt(fext, ".pl.pm")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abs|accept|alarm|and|atan2|bind|binmode|bless|caller|chdir|chmod|chomp|chop|")
			_T("chown|chr|chroot|close|closedir|cmp|connect|continue|cos|crypt|dbmclose|dbmopen|")
			_T("defined|delete|die|do|dump|each|else|elsif|endgrent|endhostent|endnetent|endprotoent|")
			_T("endpwent|endservent|eof|eq|eval|exec|exists|exit|exp|fcntl|fileno|flock|for|foreach|")
			_T("fork|formline|ge|getc|getgrent|getgrgid|getgrnam|gethostby(addr|name)|gethostent|")
			_T("getlogin|getnetby(addr|name)|getnetent|getpeername|getpgrp|getppid|getpriority|")
			_T("getprotobyname|getprotobynumber|getprotoent|getpwent|getpwnam|getpwuid|getservbyname|")
			_T("getservbyport|getservent|getsock(name|opt)|glob|gmtime|goto|grep|gt|hex|if|import|")
			_T("index|int|ioctl|join|keys|kill|last|lc|lcfirst|le|length|link|listen|local|localtime|")
			_T("log|lstat|lt|map|mkdir|msgctl|msgget|msgrcv|msgsnd|my|ne|next|no|not|oct|open|opendir|")
			_T("or|ord|pack|pipe|pop|pos|print|printf|push|quotemeta|rand|read|readdir|readlink|")
			_T("recv|redo|ref|rename|require|reset|return|reverse|rewinddir|rindex|rmdir|scalar|")
			_T("seek|seekdir|select|semctl|semget|semop|send|setgrent|sethostent|setnetent|setpgrp|")
			_T("setpriority|setprotoent|setpwent|setservent|setsockopt|shift|shmctl|shmget|shmread|")
			_T("shmwrite|shutdown|sin|sleep|socket|socketpair|sort|splice|split|sprintf|sqrt|srand|")
			_T("stat|study|sub|substr|symlink|syscall|sysread|system|syswrite|tell|telldir|tie|")
			_T("time|times|truncate|uc|ucfirst|umask|undef|unless|unlink|unpack|unshift|untie|use|")
			_T("utime|values|vec|wait|waitpid|wantarray|warn|while|write|xor|)\\b|")
			_T("^=[a-zA-Z]+\\b"));
	}
	else if (test_FileExt(fext, ".pod")) {
		ret_str = "^=[a-zA-Z]+\\b";
	}
	else if (test_FileExt(fext, ".py")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(access|and|break|class|continue|def|del|elif|else|expect|exec|finally|for|form|")
			_T("global|if|import|in|is|lambda|not|or|pass|print|raise|return|try|while)\\b"));
	}
	else if (test_FileExt(fext, ".ps1.psm1")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(Begin|Break|Catch|Class|Continue|Data|Define|Do|DynamicParam|Else|ElseIf|End|Exit|")
			_T("Filter|Finally|For|ForEach|From|Function|If|In|InlineScript|Parallel|Param|Process|")
			_T("Return|Sequence|Switch|Throw|Trap|Try|Until|Using|Var|While|Workflow)\\b"));
		sns_case = false;
	}
	else if (test_FileExt(fext, ".rb")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(BEGIN|END|alias|and|begin|break|case|class|def|defined|do|else|elsif|end|ensure|")
			_T("false|for|if|in|module|next|nil|not|or|redo|rescue|retry|return|self|super|then|")
			_T("true|undef|unless|until|when|while|yield)\\b"));
	}
	else if (test_FileExt(fext, ".rc.drc")) {
		ret_str = "^\\s*#\\w+\\b";
	}
	else if (test_FileExt(fext, ".rs")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|alignof|as|become|box|break|const|continue|crate|do|else|enum|extern|")
			_T("false|final|fn|for|if|impl|in|let|loop|macro|match|mod|move|mut|offsetof|override|")
			_T("priv|proc|pub|pure|ref|return|Self|self|sizeof|static|struct|super|trait|true|type|")
			_T("typeof|unsafe|unsized|use|virtual|where|while|yiel)\\b"));
	}
	else if (test_FileExt(fext, ".scala")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abstract|case|catch|class|def|do|else|extends|false|finalfinally|for|forSome|if|")
			_T("implicit|import|lazy|match|new|null|object|override|package|private|protected|requires|")
			_T("return|sealed|super|this|throw|trait|try|true|type|val|var|while|with|yield|_)\\b|=>|<:|<%|>:"));
	}
	else if (test_FileExt(fext, ".st")) {
		ret_str = "\\b(self|super|nil|true|false|thisContext)\\b";
	}
	else if (test_FileExt(fext, ".swift")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(as|associativity|break|case|class|continue|default|deinit|didSet|do|dynamicType|")
			_T("else|enum|extension|fallthrough|for|func|get|if|import|in|infix|init|inout|is|left|")
			_T("let|mutating|new|none|nonmutating|operator|override|postfix|precedence|prefix|protocol|")
			_T("return|right|Self|self|set|static|struct|subscript|super|switch|Type|typealias|")
			_T("unowned|var|weak|where|while|willSet|__COLUMN__|__FILE__|__FUNCTION__|__LINE__)\\b"));
	}
	else if (test_FileExt(fext, ".sh")) {
		ret_str = "\\b(case|in|esac|for|do|done|function|if|then|fi|elif|else|select|time|until|while)\\b";
	}
	else if (test_FileExt(fext, ".tex")) {
		ret_str = "\\\\\\w+\\b";
	}
	else if (test_FileExt(fext, ".vb.bas")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(AddHandler|AddressOf|Alias|And|AndAlso|As|Boolean|ByRef|Byte|ByVal|Call|Case|")
			_T("Catch|CBool|CByte|CChar|CDate|CDec|CDbl|Char|CInt|Class|CLng|CObj|Const|Continue|")
			_T("CSByte|CShort|CSng|CStr|CType|CUInt|CULng|CUShort|Date|Decimal|Declare|Default|")
			_T("Delegate|Dim|DirectCast|Do|Double|Each|Else|ElseIf|End|EndIf|Enum|Erase|Error|Event|")
			_T("Exit|False|Finally|For|Friend|Function|Get|GetType|Global|GoSub|GoTo|Handles|If|")
			_T("Implements|Imports|In|Inherits|Integer|Interface|Is|IsNot|Let|Lib|Like|Long|Loop|")
			_T("Me|Mod|Module|MustInherit|MustOverride|MyBase|MyClass|Namespace|Narrowing|New|Next|")
			_T("Not|Nothing|NotInheritable|NotOverridable|Object|Of|On|Operator|Option|Optional|")
			_T("Or|OrElse|Overloads|Overridable|Overrides|ParamArray|Partial|Private|Property|Protected|")
			_T("Public|RaiseEvent|ReadOnly|ReDim|REM|RemoveHandler|Resume|Return|SByte|Select|Set|")
			_T("Shadows|Shared|Short|Single|Static|Step|Stop|String|Structure|Sub|SyncLock|Then|")
			_T("Throw|To|True|Try|TryCast|TypeOf|Variant|Wend|UInteger|ULong|UShort|Using|When|")
			_T("While|Widening|With|WithEvents|WriteOnly|Xor)\\b|(#(Const|Else|ElseIf|End|If))\\b"));
		sns_case = false;
	}
	else if (test_FileExt(fext, ".vbs.mac")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(And|As|Boolean|ByRef|Byte|ByVal|Call|Case|Class|Const|Currency|Debug|Dim|Do|")
			_T("Double|Each|Else|ElseIf|Empty|End|EndIf|Enum|Eqv|Event|Exit|False|For|Function|")
			_T("Get|GoTo|If|Imp|Implements|In|Integer|Is|Let|Like|Long|Loop|LSet|Me|Mod|New|Next|")
			_T("Not|Nothing|Null|On|Option|Optional|Or|ParamArray|Preserve|Private|Public|RaiseEvent|")
			_T("ReDim|Rem|Resume|RSet|Select|Set|Shared|Single|Static|Stop|Sub|Then|To|True|Type|")
			_T("TypeOf|Until|Variant|Wend|While|With|Xor)\\b"));
		sns_case = false;
	}
	else if (test_FileExt(fext, ".v")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(always|and|assign|begin|buf|bufif0|bufif1|case|casx|casez|cmos|deassign|default|")
			_T("defparam|disable|edge|else|end|endcase|endmodule|endfuncion|endprimitive|endspecify|")
			_T("endtable|endtask|event|for|force|forever|fork|function|highz0|highz1|if|ifnone|initial|")
			_T("inout|input|integer|join|large|macromodule|medium|module|nand|negedge|nmos|nor|not|")
			_T("notif0|notif1|output|parameter|pmos|posedge|primitive|pull0|pull1|pullup|pulldown|rcmos|")
			_T("real|realtime|reg|release|repeat|rnmos|rpmos|rtran|rtranif0|rtranif1|scalared|small|")
			_T("specify|specparam|strong0|strong1|supply0|supply1|table|task|time|tran|tranif0|tranif1|")
			_T("tri|tri0|tri1|triand|trior|trireg|vectored|wait|wand|weak0|weak1|while|wire|wor|xnor|xor)\\b|")
			_T("^\\s*`\\w+\\b"));
	}
	else if (test_FileExt(fext, ".vhd")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(abs|access|after|alias|all|and|architecture|array|assert|attribute|begin|block|")
			_T("body|buffer|bus|case|component|configuration|constant|disconnect|downto|else|elsif|")
			_T("end|entity|exit|file|for|function|generate|generic|group|guarded|if|impure|in|inertial|")
			_T("inout|is|label|library|linkage|literal|loop|map|mod|nand|new|next|nor|not|null|of|on|")
			_T("open|or|others|out|package|port|posponed|procedure|process|pure|range|record|register|")
			_T("reject|rem|report|return|rol|ror|select|severity|shared|signal|sla|sll|sra|srl|subtype|")
			_T("then|to|transport|type|unaffected|units|until|use|variable|wait|when|while|with|xnor|xor)\\b"));
	}
	else if (test_FileExt(fext, FEXT_HTML)) {
		if (is_h2t)
			ret_str = "^(Description : |Title : )";
		else {
			ret_str  = "<[^<>]+>";
			sns_case = false;
		}
	}
	else if (test_FileExt(fext, ".nbt")) {
		ret_str.sprintf(_T("%s"),
			_T("\\b(ActivateWnd|Add|(Append|Filter|Load|Replace|Save)Buffer|Break|CallCommands|Close|Continue|")
			_T("Debug|Download|Echo(On|Off|T?)|Edit|Else|End(If|Repeat)|Exit|FileExists|FlashWin|Format(DT|DI|FN)?|")
			_T("Git|Goto|Grep(SetMask|Start)|Hint|If[^_]*(_(Break|Continue|Exit||Goto))?|Input|Ins|JumpCommands|")
			_T("List(Buffer|Var)|Match(Buffer|Dir|Ext|Name)|MsgBox[YNC]*|Mid|MsgOff|Nop|PlaySound|PopupMenu|")
			_T("(Push|Pop)(Index|TopIndex|Var)|Random|ReadINI|Redraw|Repeat|ReturnCommands|Right|SearchDown[FD]?|")
			_T("SearchUp[FD]?|SendCommands|Set_|Set(Clipboard|CopyMode|FileTime|Index)|SetExe(File|Param|Dir|Opt)|")
			_T("ShellExecute|Sleep|Test|Timer|Trim|WaitForKey|WaitForTask2?|Warn|WriteINI)_?"));
		sns_case = false;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//数値パターンを取得
//---------------------------------------------------------------------------
UnicodeString GetDefNumericPtn(UnicodeString fext)
{
	return (
		test_FileExt(fext, ".css")  ? "([: ]+#[0-9a-f]+)|(\\b[0-9][0-9.]*)" :
		test_FileExt(fext, ".dfm")  ? "\\s-?[0-9]+\\b" :
		test_FileExt(fext, ".fs")   ? "\\b[0-9][box0-9a-f.]*\\b" :
		test_FileExt(fext, ".json") ? "\\b-?[0-9][0-9.]*\\b" :
		test_FileExt(fext, ".v")    ? "\\b[0-9]+('(b[01_]+|o[0-7_]+|d[0-9_]+|h[0-9a-f_]+))?" :
		test_FileExt(fext, ".swift")?
			"\\b([0-9][0-9._]*(e[\\+\\-]?[0-9]+)?|0x[0-9a-f.]+(p[\\+\\-]?[0-9]+)?|0b[01]+|0o[0-7]+)\\b" :
		test_FileExt(fext, ".vb.vbs.bas.mac")?
			"(\\b[0-9][0-9.]*\\b)|(&h[0-9a-f]+&?)" :
		test_FileExt(fext, FEXT_PASCAL) ? "((\\b[0-9][0-9.]*)|(\\$[0-9a-f]+))\\b" :
		test_FileExt(fext, FEXT_PROGRAM)? "\\b[0-9][x0-9a-f.]*[ul]*\\b" :
		test_FileExt(fext, ".nbt")  ? "\\b[0-9]+\\b" : "");
}

//---------------------------------------------------------------------------
//シンボル文字を取得
//---------------------------------------------------------------------------
UnicodeString GetDefSymbolChars(
	UnicodeString fext,		//拡張子
	bool is_xml,			//対象がXML
	bool is_h2t)			//HTML→TEXT変換モード
{
	return (
		is_xml?									"{}/=<>:;?" :
		test_FileExt(fext, ".au3")?				"{}()[]+-*/%&!~=<>,.:" :
		test_FileExt(fext, ".css")?				"{}()*:;" :
		test_FileExt(fext, ".dfm")?				"{}()[]<>=:" :
		test_FileExt(fext, ".dsp.dsw")?			"{}/=<>:?" :
		test_FileExt(fext, ".json")?			"{}[],:" :
		test_FileExt(fext, ".ps1.psm1")?		"{}()[]+-*/%|=,:@" :
		test_FileExt(fext, ".vb.vbs.bas.mac")?	"{}()[]+-*/%|^!~=<>,:;?" :
		test_FileExt(fext, ".vhd")?				"{}()[]+-*/%&!=<>,:;" :
		test_FileExt(fext, FEXT_PROGRAM)?		"{}()[]+-*/%&|^!~=<>,:;?" :
		test_FileExt(fext, ".nbt")?				"+-*/%!=<>#_" :
		(test_FileExt(fext, FEXT_HTML) && !is_h2t)?	"{}</>=:;" : "");
}

//---------------------------------------------------------------------------
//文字列引用符を取得
//---------------------------------------------------------------------------
UnicodeString GetDefQuotChars(
	UnicodeString fext,		//拡張子
	bool &use_esc,			//[o] エスケープシーケンス有り
	bool is_xml,			//対象がXML
	bool is_ini,			//対象がINIファイル書式
	bool is_h2t)			//HTML→TEXT変換モード
{
	use_esc = (!test_FileExt(fext, ".vbs.bas.vb.mac.qbt") && !is_ini);

	return (
		test_FileExt(fext,
			FEXT_C_SH
			_T(".ahk.awk.bas.bat.cbproj.cmd.cs.drc.dsp.dsw.fs.hs.hsp.idl")
			_T(".java.json.lisp.lsp.lua.mac.qbt.qml.rc.rs.scala.swift.vb.vbs.v.vhd"))?
														"\"" :
		test_FileExt(fext, FEXT_PASCAL _T(".dfm.st"))?	"\'" :
		(test_FileExt(fext, ".php") && !is_h2t)?		"\"\'" :
		test_FileExt(fext, ".as.au3.css.js.jsx.kt.kts.pl.ps1.psm1.rb.py.sql.PspScript.ts")?
														"\"\'" :
		test_FileExt(fext, ".go")?						"\"`" :
		test_FileExt(fext, ".sh")?						"\"\'`" :
		(test_FileExt(fext, FEXT_HTML) && !is_h2t)?		"\"" :
		(is_xml || is_ini)?								"\"" :
		test_FileExt(fext, ".nbt")?						"\"%" : "");
}

//---------------------------------------------------------------------------
//関数パターンを取得
//---------------------------------------------------------------------------
UnicodeString GetDefFunctionPtn(
	UnicodeString fext,			//拡張子
	UnicodeString &name_ptn,	//[o] 名前取得パターン
	bool is_h2t)				//HTML→TEXT変換モード
{
	UnicodeString func_ptn;
	name_ptn = EmptyStr;

	UnicodeString std_ptn = "\\b[_a-zA-Z]\\w*\\(";

	if (test_FileExt(fext, ".ahk")) {
		func_ptn = "^[_a-zA-Z]\\w*\\(\\w*\\)\\s*{";
		name_ptn = "^[_a-zA-Z]\\w*\\(";
	}
	else if (test_FileExt(fext, ".au3")) {
		func_ptn = "^Func\\s+[_a-zA-Z]\\w*\\(";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, FEXT_C_SH)) {
		func_ptn = "^([_a-zA-Z][^\\(=;:,]*[\\t ]+)*\\**([_a-zA-Z]\\w*::)?[~_a-zA-Z]\\w*\\([^;]*$";
		name_ptn = "(\\b|~)[_a-zA-Z]\\w*\\(";
	}
	else if (test_FileExt(fext, ".go")) {
		func_ptn = "^func(\\s\\(.+\\))?\\s+[_a-zA-Z]\\w*";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".hsp")) {
		func_ptn = "^#(defcfunc|deffunc)\\s+\\w+";
		name_ptn = "\\b\\w+\\b";
	}
	else if (test_FileExt(fext, ".js.jsx.ts.as.awk")) {
		func_ptn = "^function\\s+[_a-zA-Z]\\w*\\(";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".java")) {
		func_ptn = "^\\s*((public|private|protected)\\s+)?[_a-zA-Z][^\\(=;:]*\\s+[_a-zA-Z]\\w*\\([^;]*$";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".kt.kts")) {
		func_ptn = "^\\s*fun\\s+.+\\(.*\\)";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".lua")) {
		func_ptn = "^(local\\s)?function\\s+[_a-zA-Z][\\w\\.:]*\\(";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".mac")) {
		func_ptn = "^(sub|proc|function)\\s+[_a-zA-Z]\\w*";
	}
	else if (test_FileExt(fext, ".pas.dpr.dpk.inc")) {
		func_ptn = "^(procedure|function)\\s+[_a-zA-Z]\\w*";
	}
	else if (test_FileExt(fext, ".php") && !is_h2t) {
		func_ptn = "^(function)\\s+[_a-zA-Z]\\w*";
	}
	else if (test_FileExt(fext, ".pl.pm")) {
		func_ptn = "^(sub\\s+[_a-zA-Z]\\w*|=head\\d\\b)";
	}
	else if (test_FileExt(fext, ".ps1.psm1")) {
		func_ptn = "^function\\s+[_a-zA-Z](\\w|-)*\\b";
		name_ptn = "\\s[_a-zA-Z](\\w|-)*\\b";
	}
	else if (test_FileExt(fext, ".rb.py"))
		func_ptn = "^\\s*def\\s+[_a-zA-Z]\\w*";
	else if (test_FileExt(fext, ".rs"))
		func_ptn = "^(pub\\s)?(fn)\\s+[_a-zA-Z]\\w*";
	else if (test_FileExt(fext, ".scala")) {
		func_ptn = "^\\s*def\\s+[_a-zA-Z][^:]*:[^:]*";
		name_ptn = "\\b[_a-zA-Z]\\w*[\\(\\[:]";
	}
	else if (test_FileExt(fext, ".sh")) {
		func_ptn = "^(function\\s)?[_a-zA-Z]\\w*\\(";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".swift")) {
		func_ptn = "^\\s*func\\s+[_a-zA-Z]\\w*\\(";
		name_ptn = std_ptn;
	}
	else if (test_FileExt(fext, ".vbs.vb")) {
		func_ptn = "^\\s*((public|private)\\s)?(shared\\s)?(sub|proc|function)\\s+[_a-zA-Z]\\w*";
	}

	return func_ptn;
}

//---------------------------------------------------------------------------
//SearchPair コマンド用検索パターンを取得
//---------------------------------------------------------------------------
bool GetSearchPairPtn(UnicodeString fext, TStringList *lst)
{
	UnicodeString tab = "\t";
	if (test_FileExt(fext, ".dfm")) {
		lst->Add("^\\s*((object\\s)|item$)" + tab + "^\\s*end>?$");
	}
	else if (test_FileExt(fext, ".pas")) {
		lst->Add("(^\\s*(((else\\s)?begin)|case|try|record)\\b)|(\\w+\\s=\\s(class|interface|record)\\b)"
					+ tab + "^\\s*end[;).]?\\b");
	}
	else if (test_FileExt(fext, FEXT_C_SH)) {
		lst->Add("^\\s*#\\s*if\\w*" + tab + "^\\s*#\\s*endif");
	}
	else if (test_FileExt(fext, ".vbs.vb.mac")) {
		lst->Add("^\\s*sub\\s+\\w+\\b" + tab + "^\\s*end\\s+sub\\b");
		lst->Add("^\\s*function\\s+\\w+\\b" + tab + "^\\s*end\\s+function\\b");
		lst->Add("^\\s*proc\\s+\\w+\\b" + tab + "^\\s*end\\s+proc\\b");
		lst->Add("^\\s*if\\s.+\\s+then\\b" + tab + "^\\s*end\\s+if\\b");
		lst->Add("^\\s*for\\s+.+" + tab + "^\\s*next\\b");
		lst->Add("^\\s*do(\\s+(until|while)\\s)?" + tab + "^\\s*loop\\b");
		lst->Add("^\\s*select\\s+case\\s" + tab + "^\\s*end\\s+select\\b");
		lst->Add("^\\s*class\\s+\\w+\\b" + tab + "^\\s*end\\s+class\\b");
	}
	else if (test_FileExt(fext, ".pod.pl.pm")) {
		lst->Add("^=pod\\b" + tab + "^=cut\\b");
		lst->Add("^=over\\b" + tab + "^=back\\b");
	}

	return (lst->Count>0);
}

//---------------------------------------------------------------------------
//指定拡張子に対するデフォルトの構文強調表示定義を取得
//---------------------------------------------------------------------------
bool GetDefaultHighlight(
	UnicodeString fext,		//拡張子
	TStringList *lst,		//[o] 結果リスト
	TStringList *hl_lst)	//拡張子対応見出しリスト	(default = NULL)
{
	if (!StartsStr('.', fext)) fext.Insert(".", 1);

	bool is_xml = test_FileExt(fext, FEXT_XML);
	bool is_ini = test_FileExt(fext, ".ini.inf.reg.url");

	UnicodeString lbuf;
	lst->Clear();
	lst->Add(lbuf.sprintf(_T("[%s]"), fext.c_str()));

	//コメント
	std::unique_ptr<TStringList> ln_lst(new TStringList());		//１行コメント
	std::unique_ptr<TStringList> bgn_lst(new TStringList());	//ブロック開始
	std::unique_ptr<TStringList> end_lst(new TStringList());	//ブロック終了
	if (UserHighlight->GetCommentList("*" + fext, ln_lst.get(), bgn_lst.get(), end_lst.get(), false, true)) {
		for (int i=0; i<ln_lst->Count; i++)
			lst->Add(lbuf.sprintf(_T("Comment%u=%s"), i + 1, ln_lst->Strings[i].c_str()));

		for (int i=0; i<bgn_lst->Count && i<end_lst->Count; i++) {
			UnicodeString s = bgn_lst->Strings[i];
			if (s.Length()>1) {
				if		(StartsStr("\"", s) && EndsStr("\"", s)) s = "\'" + s + "\'";
				else if (StartsStr("\'", s) && EndsStr("\'", s)) s = "\"" + s + "\"";
			}
			lst->Add(lbuf.sprintf(_T("CommentBgn%u=%s"), i + 1, s.c_str()));
			s = end_lst->Strings[i];
			if (s.Length()>1) {
				if		(StartsStr("\"", s) && EndsStr("\"", s)) s = "\'" + s + "\'";
				else if (StartsStr("\'", s) && EndsStr("\'", s)) s = "\"" + s + "\"";
			}
			lst->Add(lbuf.sprintf(_T("CommentEnd%u=%s"), i + 1, s.c_str()));
		}
	}

	//見出し
	UnicodeString hl_ptn = UserHighlight->GetHeadlinePtn("*" + fext, hl_lst, true);
	if (!hl_ptn.IsEmpty()) lst->Add("HeadlinePtn=" + hl_ptn);

	//予約語
	bool sns_case;
	UnicodeString res_str = GetDefReservedPtn(fext, sns_case, is_xml, false, false);
	if (!res_str.IsEmpty()) {
		lst->Add("ReservedPtn=" + res_str);
		lst->Add(lbuf.sprintf(_T("ReservedCase=%u"), sns_case? 1 : 0));
	}
	//数値
	res_str = GetDefNumericPtn(fext);
	if (!res_str.IsEmpty()) lst->Add("NumericPtn=" + res_str);
	//シンボル
	res_str = GetDefSymbolChars(fext, is_xml, false);
	if (!res_str.IsEmpty()) lst->Add("Symbol=" + res_str);
	//文字列
	bool use_esc;
	res_str = GetDefQuotChars(fext, use_esc, is_xml, is_ini, false);
	if (!res_str.IsEmpty()) {
		lst->Add("Strings=" + res_str);
		lst->Add(lbuf.sprintf(_T("UseEsc=%u"), use_esc? 1 : 0));
	}

	//関数
	UnicodeString name_ptn;
	res_str = GetDefFunctionPtn(fext, name_ptn, false);
	if (!res_str.IsEmpty()) {
		lst->Add("FunctionPtn=" + res_str);
		if (!name_ptn.IsEmpty()) lst->Add("FuncNamePtn=" + name_ptn);
	}

	//SearchPair 用パターン
	std::unique_ptr<TStringList> ptn_lst(new TStringList());
	if (GetSearchPairPtn(fext, ptn_lst.get())) {
		for (int i=0; i<ptn_lst->Count; i++) {
			UnicodeString ptn = ptn_lst->Strings[i];
			UnicodeString tmp;
			lst->Add(tmp.sprintf(_T("PairBeginPtn%u=%s"), i + 1, get_pre_tab(ptn).c_str()));
			lst->Add(tmp.sprintf(_T("PairEndPtn%u=%s"),   i + 1, get_post_tab(ptn).c_str()));
		}
	}

	return (lst->Count>0);
}
//---------------------------------------------------------------------------
