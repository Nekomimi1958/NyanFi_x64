//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビュアー													//
//----------------------------------------------------------------------//
#ifndef TxtViewerH
#define TxtViewerH

//---------------------------------------------------------------------------
#include "usr_mmfile.h"
#include "Global.h"

//---------------------------------------------------------------------------
//単語マッチ用正規表現
#define WORD_MATCH_PTN	"\\w+|[一-龠]+|[ぁ-ん]+|[ァ-ヴー]+|[ａ-ｚＡ-Ｚ０-９]+|[ｦ-ﾟ]+|#[0-9a-fA-F]{3,6}"

//リンク先検索の正規表現
#define LINK_MATCH_PTN	"(https?://[\\w/:%#$&?()~.=+-]+)|(file:///[^/*?\"<>|）]+)|(mailto:[a-zA-Z0-9]+[\\w.-]*@[\\w.-]+)"
	
#define MAX_BIN_HCH_X	67
#define MARK_WIDTH		8

#define MAX_EM_PTN		7	//強調表示パターン数

//---------------------------------------------------------------------------
//表示行内容
struct line_rec {
	int  LineNo;			//行番号
	int  LineIdx;			//改行行内インデックス
	bool hasCR;				//行末に改行コードあり
	WideChar topQch;
	int  RemPos0;			//コメント開始位置
	int  RemPos1;			//コメント終了位置
};

//---------------------------------------------------------------------------
class TTxtViewer
{
private:
	TPaintBox	*ViewBox;		//ビュアー描画用PaintBox
	TCanvas		*ViewCanvas;
	TScrollBar	*ScrBar;
	UsrScrollPanel *ScrPanel;
	TPaintBox	*RulerBox;		//ルーラ
	TStatusBar	*SttHeader;		//ステータスヘッダ
	TPanel		*ColorPanel;	//カーソル位置値のカラー表示
	TPaintBox	*MarginBox;		//右余白

	bool isExtWnd;				//外部ウィンドウで表示

	UnicodeString EmPtn[MAX_EM_PTN];	//強調表示パターン
	TColor EmFgC[MAX_EM_PTN];			//強調文字色
	TColor EmBgC[MAX_EM_PTN];			//強調背景色

	Graphics::TBitmap *ImgBuff;

	TPoint LastPos;
	int	 LastTop;
	bool LastSel;
	bool SelSkip;
	bool inh_Repaint;

	UnicodeString useFontName;		//使用フォント名
	int  useFontSize;				//使用フォントサイズ(リセット値)

	int  FontHeight;				//フォント高
	int  LineHeight;				//行高
	int  HchWidth;					//半角幅(px)
	int  TabWidth;					//タブ幅(px)
	int  CurHchX;					//上下移動時のX位置(半角単位)
	int  MaxHchX;					//X位置の最大値(半角単位)
	bool IsIrregFont;				//取得幅と表示幅が不一致(Ricty Diminished 対策)
	int  LeftMargin;
	int  CurTop;					//表示先頭行(1ベース)
	int  CurBottom;					//表示最下行
	int  TopXpos;					//行頭の表示開始位置
	int  TopMargin;					//行頭の余白

	DynamicArray<int>  FixWdList;	//固定長幅リスト
	DynamicArray<bool> IsNumList;	//項目が数値か

	TStringList *RemLnList;			//１行コメント識別文字列
	TStringList *RemBgnList;		//ブロックコメント開始文字列
	TStringList *RemEndList;		//ブロックコメント終了文字列

	UnicodeString QuotStr;			//文字列の引用符
	bool useEsc;					//文字列内でエスケープシーケンスあり

	UnicodeString ReservedPtn;		//予約語
	bool ReservedCase;				//予約語で大小文字を区別

	UnicodeString SymbolChs;		//シンボル
	UnicodeString NumericPtn;		//数値
	UnicodeString CharPtn;			//文字

	UnicodeString UsrKeyword;		//ユーザ定義キーワード
	bool   UsrKeywdCase;			//ユーザ定義キーワードで大小文字を区別
	TColor UsrKeywdCol;				//ユーザ定義キーワードの文字色

	UnicodeString UsrKeyword2;		//ユーザ定義キーワード
	bool   UsrKeywdCase2;			//ユーザ定義キーワードで大小文字を区別
	TColor UsrKeywdCol2;			//ユーザ定義キーワードの文字色

	TStringList *PairPtnList;		//SearchPair 用の開始/終了行パターンリスト

	TBytes FindByte0, FindByte1;	//バイナリ検索語
	TBytes FindMask;				//バイナリ検索用マスク

	bool alt_BackSlash;				//\ を ＼(U+2216)で表示

	line_rec BinLineRec;
	line_rec* __fastcall AddDispLine(UnicodeString s, int lno, int lidx = 0);
	void __fastcall ClearDispLine();

	void __fastcall AlphaBlendCsvCol(TCanvas *cv, int max_x, int y, int h);
	void __fastcall onRulerPaint(TObject *Sender);
	void __fastcall PaintText();
	bool __fastcall SetToggleSw(bool &sw, UnicodeString prm);

	void __fastcall SetScrBar();

	UnicodeString __fastcall ArrayToTsv(TStringDynArray lst);
	void __fastcall FormatFixed(TStringList *txt_lst);
	void __fastcall ConvDfmText(TStringList *txt_lst);

	TPoint __fastcall nrm_Pos(TPoint p);
	bool __fastcall has_CR(UnicodeString s);
	int  __fastcall add_CharWidth(WideChar c, int w);
	int  __fastcall get_StrWidth(UnicodeString s);
	int  __fastcall get_CurEndPos();
	void __fastcall adjust_PosX();
	void __fastcall to_Lead_if_Trail(bool sel = false);
	void __fastcall set_SelEnd();
	void __fastcall set_CurCsvCol();

	WideChar __fastcall get_CurChar();
	int  __fastcall cv_PosX_to_HchX(int px);
	void __fastcall TabTextOut(UnicodeString s, TCanvas *cv, int &x, int max_x);
	void __fastcall IniSelected(bool sel = false);
	void __fastcall SetPosFromPt(int x, int y);
	bool __fastcall PtInSelected(int x, int y);
	void __fastcall UpdatePos(bool up_pos = false, bool force = false);
	void __fastcall UpdateVisible();
	int  __fastcall get_MovePrm(UnicodeString prm);
	int  __fastcall to_Bytes(UnicodeString s, bool &case_sw, int code_page = 932);

	int  __fastcall GetComCmdIndex(UnicodeString cmd);

public:
	TForm *OwnerForm;

	MemMapFile	 *MMF;				//メモリマップドファイル

	TStringList  *TxtBufList;		//内容リスト
	TStringList  *TxtBufList2;		//HTML→TEXT変換後の内容リスト
	TStringList  *DispLines;		//表示行リスト(テキスト用)
	TStringList  *NyanFiDef;		//ADS定義ファイルの内容

	UnicodeString FileName;			//ファイル名
	UnicodeString OrgName;			//一時解凍している場合の元ファイル名
	file_rec     *FileRec;
	bool 		  HasBOM;			//BOM付き
	UnicodeString LineBreakStr;		//改行コード
	UnicodeString SttHdrInf;		//ファイル情報
	UnicodeString MarkListStr;		//マークリスト

	bool isReady;
	bool isFitWin;					//折り返しをウィンドウ幅に
	bool isText;					//テキスト表示
	bool isBinary;					//バイナリ表示
	bool isXDoc2Txt;				//xdoc2txtを使用
	bool isHtm2Txt;					//HTML→TEXT変換モード
	bool isFixedLen;				//固定長表示モード
	bool isCSV;						//CSV(TSVを含む)
	bool isTSV;
	bool isSelected;				//ファイル選択中
	bool ShowRuby;					//ルビ表示
	bool isAozora;					//青空文庫形式
	bool isLog;						//タスクログ
	bool isClip;					//クリップボード
	bool isIniFmt;					//INIファイルなど
	bool isAwstats;					//Awstatsログ
	bool isNyanTxt;					//NyanFi用テキスト
	bool isLimited;					//サイズ制限有り
	bool isContinue;				//最後から続く
	bool isSelMode;					//選択モード
	bool isBoxMode;					//箱形選択モード
	bool ExtClicked;				//テキスト外をクリックされた
	bool Selecting;					//マウスで選択中
	bool PtInSelect;				//選択範囲内をクリックした
	bool reqCurPos;					//閉じたときにファイラーのカーソル位置を設定

	int  TabLength;					//タブ数
	int  LineCount;					//1ページ分の行数(-1)
	int  MaxLine;					//最大行数
	int  MaxDispLine;				//最大表示行

	bool isTail;					//末尾を表示
	bool isReverse;					//逆順
	int  TailLine;					//末尾行数
	bool TopIsHeader;				//先頭行はCSV項目名

	unsigned int BinarySize;		//バイナリ読込サイズ
	unsigned __int64 BinFileSize;	//ファイルサイズ
	__int64 TopAddress;				//先頭アドレス

	TPoint CurPos;					//カーソル位置(文字単位 0ベース)
	TPoint SelStart, SelEnd;		//通常選択範囲(文字単位)
	TPoint BoxStart, BoxEnd;		//箱形選択範囲(半角単位)
	int  MaxFoldWd;					//最大折り返し幅
	int  CsvCol;					//CSV列(固定長表示モード時)

	int  SortMode;					//ソートモード(0:なし/ 1:昇順/ -1:降順)
	int  SortCol;					//ソート列(固定長表示モード時)

	UnicodeString FindWord;			//検索文字列
	UnicodeString RegExPtn;			//正規表現パターン
	UnicodeString LastSelWord;		//前回 FindSel～で用いた検索語
	bool isCase;					//大小文字を区別
	bool isRegEx;					//正規表現
	bool isMigemo;					//Migemoモード
	bool isBytes;					//バイト列検索
	int  BinCodePage;				//コードページ
	bool Highlight;					//マッチの強調表示
	bool LastFound;					//直前の検索結果
	TPoint FoundPos;				//直前のヒット位置

	UnicodeString IncSeaWord;		//インクリメンタルサーチ入力語
	bool isIncSea;					//インクリメンタルサーチ
	bool isIncMigemo;				//Migemoモード

	UnicodeString HeadlinePtn;		//見出しの正規表現
	UnicodeString RubyPtn;			//ルビの正規表現

	TStringList *ColBufList;
	TColor color_Cursor;		//ラインカーソルの色
	TColor color_selItem;		//選択項目の背景色
	TColor color_fgSelItem;		//選択項目の文字色
	TColor color_Folder;		//ディレクトリの文字色
	TColor color_Error;			//エラーの文字色
	TColor color_bgView;		//テキストビュアーの背景色
	TColor color_fgView;		//テキストビュアーの文字色
	TColor color_Margin;		//テキストビュアーの余白白
	TColor color_bgLineNo;		//テキストビュアーの行番号背景色
	TColor color_LineNo;		//テキストビュアーの行番号文字色
	TColor color_Mark;			//テキストビュアーのマーク
	TColor color_bgRuler;		//ルーラの背景色
	TColor color_fgRuler;		//ルーラの目盛色
	TColor color_bdrLine;		//行番号の境界線
	TColor color_bdrFold;		//折り返し境界線
	TColor color_bdrFixed;		//固定長表示の縦罫線
	TColor color_Comment;		//コメントの文字色
	TColor color_Strings;		//文字列の文字色
	TColor color_Reserved;		//予約語の文字色
	TColor color_Symbol;		//シンボルの文字色
	TColor color_Numeric;		//数値の文字色
	TColor color_fgEmpBin1;		//バイナリ強調文字色1
	TColor color_fgEmpBin2;		//バイナリ強調文字色2
	TColor color_fgEmpBin3;		//バイナリ強調文字色3
	TColor color_Headline;		//見出しの文字色
	TColor color_URL;			//URLの文字色
	TColor color_LocalLink;		//ローカファイルへのリンク
	TColor color_fgEmp;			//強調文字色
	TColor color_bgEmp;			//強調背景色
	TColor color_Ruby;			//ルビ
	TColor color_TAB;			//タブ表示色
	TColor color_CR;			//改行表示色
	TColor color_HR;			//罫線の色
	TColor color_Ctrl;			//コントロールコード

	__fastcall TTxtViewer(TForm *frm, TPaintBox *viewbox, TScrollBar *scrbar, UsrScrollPanel *sp,
							TStatusBar *stthdr, TPaintBox *ruler, TPanel *colref, TPaintBox *mgn_box = NULL);
	__fastcall ~TTxtViewer();

	bool __fastcall CloseAuxForm();
	bool __fastcall SaveNyanFiDef();
	void __fastcall Clear();
	void __fastcall SetColor(TStringList *lst);
	void __fastcall SetColor(UnicodeString prm = EmptyStr);
	void __fastcall SetOptColor();
	void __fastcall SetMetric(bool set_hi = false);
	void __fastcall UpdateScr(int lno = 1);
	void __fastcall AssignText(TStrings *lst = NULL, int lno = 1, int sort_mode = 0);
	bool __fastcall AssignBin(__int64 top_adr = 0, bool reload = false, unsigned int adr = 0);
	void __fastcall Repaint(bool force = false);

	void __fastcall SetSttInf(UnicodeString msg = EmptyStr);
	void __fastcall SttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, TRect Rect);

	void __fastcall onMouseDown(int x, int y);
	void __fastcall onMouseMove(int x, int y);
	void __fastcall onMouseUp();
	void __fastcall onDblClick();
	void __fastcall onRulerDblClick(TObject *Sender);

	void __fastcall CursorLeft(bool sel = false);
	void __fastcall CursorRight(bool sel = false);
	void __fastcall CursorUp(bool sel = false, UnicodeString prm = EmptyStr);
	void __fastcall CursorDown(bool sel = false, UnicodeString prm = EmptyStr);
	void __fastcall MovePage(bool is_down, bool sel = false);
	void __fastcall ScrollAdjust();
	void __fastcall MoveScroll(bool is_down, UnicodeString prm, bool move_csr = false);
	void __fastcall LineTop(bool sel = false);
	void __fastcall MoveLineTop(bool is_next, bool sel = false);
	void __fastcall LineEnd(bool sel = false);
	void __fastcall TextTop(bool sel = false);
	void __fastcall TextEnd(bool sel = false);
	bool __fastcall set_PosFromCol(int col);

	void __fastcall WordLeft(bool sel = false);
	void __fastcall WordRight(bool sel = false);

	void __fastcall ClipCopy(bool append = false);

	line_rec* __fastcall get_LineRec(int idx);
	UnicodeString __fastcall get_DispLine(int idx, int b0 = 0, int b1 = 15);

	UnicodeString __fastcall get_SelText(bool clr_sel = false);
	UnicodeString __fastcall get_CurLine(bool to_cr = false);
	int __fastcall get_CurLineNo();
	__int64 __fastcall get_CurAddrA();
	unsigned int __fastcall get_CurAddrR();
	unsigned int __fastcall get_OfsR(int xp);
	TPoint __fastcall get_CurLinePos();
	TStringDynArray __fastcall GetCsvHdrList();

	void __fastcall SelectAll();
	UnicodeString __fastcall GetCurWord(bool select = false, UnicodeString ptn = EmptyStr, int *xp = NULL);
	void __fastcall SelLine(bool cr = false);

	int  __fastcall change_CodePage(UnicodeString prm);

	UnicodeString __fastcall GetCurImgFile();

	bool __fastcall SearchCore(bool is_down, UnicodeString kwd,
		bool case_sw = false, bool reg_sw = false, bool bytes_sw = false, bool from_pos = false);
	bool __fastcall SearchDown(UnicodeString kwd,
		bool case_sw = false, bool reg_sw = false, bool bytes_sw = false, bool from_pos = false);
	bool __fastcall SearchUp(UnicodeString kwd,
		bool case_sw = false, bool reg_sw = false, bool bytes_sw = false);
	bool __fastcall SearchDownBytes(UnicodeString kwd, bool case_sw, bool reg_sw);
	bool __fastcall SearchUpBytes(UnicodeString kwd, bool case_sw, bool reg_sw);
	bool __fastcall SearchSel(bool up_sw, bool em_sw);

	bool __fastcall SearchPairCore(UnicodeString bgn_ptn, UnicodeString end_ptn);
	bool __fastcall SearchPair(UnicodeString prm);

	bool __fastcall ToLine(int lno, int col = -1);
	bool __fastcall ToAddrR(unsigned int adr);
	bool __fastcall ToAddrA(__int64 adr);
	void __fastcall JumpLine(UnicodeString ln_str);

	void __fastcall MarkLine(int lno);
	bool __fastcall IsMarked(int lno = 0);

	void __fastcall add_ViewHistory();

	void __fastcall GetDumpList(TStringList *lst);

	void __fastcall IncSearch(UnicodeString keystr);

	UnicodeString __fastcall GetStdKeyCommand(UnicodeString keystr);
	bool __fastcall ExeCommand(const _TCHAR *cmd, UnicodeString prm = EmptyStr);
	bool __fastcall IsCmdAvailable(UnicodeString cmd);
	bool __fastcall IsCmdInhibited(UnicodeString cmd);

};
//---------------------------------------------------------------------------
extern TTxtViewer *TxtViewer;

//---------------------------------------------------------------------------
#endif
