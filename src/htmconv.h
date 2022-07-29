//----------------------------------------------------------------------//
// HTML→テキスト変換処理クラス											//
//																		//
//----------------------------------------------------------------------//
#ifndef htmconvH
#define htmconvH

//---------------------------------------------------------------------------
#define DEF_LINE_WIDTH		80
#define DEF_BLANK_LN_LIMIT	3				//連続空行制限

//---------------------------------------------------------------------------
#define NO_CRSPC_PTN	"^(#{1,6}|\\s*[\\*\\+\\-]|\\s*\\d+\\.)\\s.+|^```"

//---------------------------------------------------------------------------
class HtmConv
{
private:
	UnicodeString TxtLineBuf;
	bool Skip;
	bool fPRE, fXMP;
	int  BQ_level;

	UnicodeString rel_to_abs(UnicodeString fnam, UnicodeString rnam);

	UnicodeString int_to_alpha(int n);
	UnicodeString int_to_roman(int n);
	void del_tag_block(TStringList *lst, const _TCHAR *start_wd, const _TCHAR *end_wd);

	UnicodeString ToAbsUrl(UnicodeString url);
	UnicodeString GetTag(UnicodeString s);
	void FlushText(UnicodeString s = EmptyStr);
	void AddHR();

	bool is_rel_url(UnicodeString s)
	{
		return (!s.IsEmpty() && !TRegEx::IsMatch(s, "^(https?|mailto|file):|^//"));
	}

	void DecLevel(int &n)
	{
		n = std::max(n - 1, 0);
	}

public:
	HtmConv();
	~HtmConv();

	UnicodeString FileName;		//ファイル名
	UnicodeString UrlStr;		//URL
	UnicodeString TopLevel;		//トップレベル
	UnicodeString Scheme;		//スキーム
	UnicodeString BaseUrl;

	TStringList *HtmBuf;		//HTML文書入力バッファ
	TStringList *TxtBuf;		//テキスト出力バッファ

	int  CodePage;				//出力コードページ

	bool ToMarkdown;			//Markdown に変換

	int  LineWidth;				//桁数
	UnicodeString HeaderStr;	//見出し文字(;区切り)
	UnicodeString UlMarkStr;	//ULマーク
	UnicodeString InsHrClass;	//指定クラスのDIVの前に罫線挿入

	bool InsHdrInf;				//ヘッダ情報を挿入
	bool InsHrSection;			//<section>の前に罫線挿入
	bool InsHrArticle;			//<article>の前に罫線挿入
	bool InsHrNav;				//<nav>の前に罫線挿入

	UnicodeString DelBlkCls;	//削除部分の class リスト
	UnicodeString DelBlkId;		//削除部分の id リスト

	UnicodeString Title;
	UnicodeString Description;
	UnicodeString Keywords;

	void DelAtrBlock(TStringList *lst, UnicodeString anam, UnicodeString aval);
	UnicodeString GetTagAtr(UnicodeString s, UnicodeString t, const _TCHAR *a);
	UnicodeString RefEntity(UnicodeString s);
	void Convert();
};
//---------------------------------------------------------------------------
#endif
