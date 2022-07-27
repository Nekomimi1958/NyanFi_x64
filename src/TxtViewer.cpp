//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビュアー													//
//----------------------------------------------------------------------//
#include <IdURI.hpp>
#include "htmconv.h"
#include "usr_mmfile.h"
#include "usr_xd2tx.h"
#include "usr_excmd.h"
#include "usr_highlight.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "FuncDlg.h"
#include "FileInfDlg.h"
#include "ChInfFrm.h"
#include "GraphFrm.h"
#include "CsvRecFrm.h"
#include "InspectFrm.h"
#include "BitmapFrm.h"
#include "SubView.h"
#include "ColDlg.h"
#include "InpDir.h"
#include "InpExDlg.h"
#include "ExTViewer.h"
#include "TxtViewer.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TTxtViewer *TxtViewer;

//---------------------------------------------------------------------------
// TTxtViewer クラス
//---------------------------------------------------------------------------
TTxtViewer::TTxtViewer(
	TForm *frm,
	TPaintBox *viewbox,		//表示領域
	TScrollBar *scrbar,		//スクロールバー
	UsrScrollPanel *sp, 	//独自スクロールバー
	TStatusBar *stthdr,		//情報ヘッダ
	TPaintBox *ruler,		//ルーラ
	TPanel *colref,			//カラー表示
	TPaintBox *mgn_box)		//右余白パネル	(default = NULL)
{
	isReady    = false;
	OwnerForm  = frm;
	isExtWnd   = OwnerForm->ClassNameIs("TExTxtViewer");

	ViewBox    = viewbox;
	ViewCanvas = ViewBox->Canvas;

	ScrBar	   = scrbar;
	ScrPanel   = sp;
	SttHeader  = stthdr;
	RulerBox   = ruler;
	ColorPanel = colref;
	MarginBox  = mgn_box;

	if (RulerBox) {
		RulerBox->OnPaint	 = onRulerPaint;
		RulerBox->OnDblClick = onRulerDblClick;
	}

	ImgBuff = new Graphics::TBitmap();

	ColBufList	= new TStringList();

	TxtBufList	= new TStringList();
	TxtBufList2 = new TStringList();
	MaxLine 	= 0;

	DispLines	= new TStringList();
	MaxDispLine = 0;

	RemLnList	= new TStringList();
	RemBgnList	= new TStringList();
	RemEndList	= new TStringList();

	PairPtnList	= new TStringList();

	MMF = new MemMapFile();

	SetColor();

	BinarySize	= 0;
	BinFileSize = 0;
	TopAddress	= 0;

	BinLineRec.LineNo  = 1;
	BinLineRec.LineIdx = 0;
	BinLineRec.hasCR   = true;
	BinLineRec.topQch  = '\0';
	BinLineRec.RemPos0 = 0;
	BinLineRec.RemPos1 = 0;

	FileRec		= NULL;

	EmpComment	= true;
	EmpStrings	= true;
	EmpReserved = true;
	useEsc		= true;

	isSelected  = false;
	isText		= isBinary = false;
	isCSV		= isTSV    = false;
	isXDoc2Txt	= false;
	isHtm2Txt	= isFixedLen = false;
	isLimited	= false;
	isTail		= false;
	isContinue	= false;
	ShowRuby	= true;
	isAozora	= isLog = isClip = isIniFmt = isAwstats = isNyanTxt = isJsonFmt = false;
	isSelMode	= isBoxMode = false;
	reqCurPos	= false;

	FontHeight	= abs(ViewBox->Font->Height);
	LineHeight	= FontHeight + ViewTxtInterLn;
	HchWidth	= 0;
	LineCount	= 0;
	TabLength	= 0;
	TabWidth	= 0;
	TopXpos 	= 0;
	TopMargin	= 8;
	SortMode	= 0;
	SortCol 	= 0;

	CurPos		= Point(0, 0);
	CurHchX		= 0;
	MaxHchX 	= 80;
	MaxFoldWd	= 80;
	SelStart	= CurPos;
	SelEnd		= CurPos;
	BoxStart	= Point(0, 0);
	BoxEnd		= BoxStart;
	CsvCol		= -1;

	isRegEx 	= isCase = isMigemo = isBytes = false;
	isIncSea	= false;
	FoundPos	= Point(-1, -1);

	Highlight	= IniFile->ReadBoolGen(_T("FindTxtDlgHighlight"));

	ExtClicked	= false;
	Selecting	= false;
	PtInSelect  = false;
	LastFound   = false;

	LastPos		= Point(-1, -1);
	LastTop		= -1;
	LastSel 	= false;
	SelSkip 	= false;
	inh_Repaint = false;
}
//---------------------------------------------------------------------------
TTxtViewer::~TTxtViewer()
{
	ClearDispLine();

	delete ColBufList;
	delete DispLines;
	delete ImgBuff;
	delete TxtBufList;
	delete TxtBufList2;
	delete RemLnList;
	delete RemBgnList;
	delete RemEndList;
	delete PairPtnList;
	delete MMF;
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetTopIsHeader(bool Value)
{
	USR_CsvTopIsHdr = FTopIsHeader = Value;
	if (isCSV) write_NyanFiDef(FileName, "TopIsHeader", FTopIsHeader? "1" : "0");
}

//---------------------------------------------------------------------------
//補助画面を閉じる
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::CloseAuxForm()
{
	bool closed = false;
	if (CharInfoForm->Visible) { CharInfoForm->Close();	closed = true; }
	if (CsvRecForm->Visible)   { CsvRecForm->Close();	closed = true; }
	if (BitmapForm->Visible)   { BitmapForm->Close();	closed = true; }
	if (InspectForm->Visible)  { InspectForm->Close();	closed = true; }
	if (SubViewer->Visible)    { SubViewer->Close();	closed = true; }
	return closed;
}

//---------------------------------------------------------------------------
//表示行レコードを追加
//---------------------------------------------------------------------------
line_rec* __fastcall TTxtViewer::AddDispLine(UnicodeString s, int lno, int lidx)
{
	line_rec *lp = new line_rec;
	lp->LineNo	 = lno;
	lp->LineIdx  = lidx;
	lp->hasCR	 = ends_tchs(_T("\n\r"), s);
	lp->topQch	 = '\0';
	lp->RemPos0  = 0;
	lp->RemPos1  = 0;
	DispLines->AddObject(s, (TObject*)lp);
	return lp;
}

//---------------------------------------------------------------------------
//指定行の表示行レコード(line_rec)を取得
//---------------------------------------------------------------------------
line_rec * __fastcall TTxtViewer::get_LineRec(int idx)
{
	line_rec *lp;
	if (isBinary) {
		lp = &BinLineRec;
		lp->LineNo = (idx<MaxDispLine-1)? (TopAddress/16 + idx + 1) : -1;
	}
	else {
		lp = (line_rec*)DispLines->Objects[idx];
	}
	return lp;
}

//---------------------------------------------------------------------------
//指定行の表示文字列を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::get_DispLine(
	int idx,
	int b0,		//取得開始オフセット(バイナリ)　(default = 0);
	int b1)		//取得終了オフセット(バイナリ)　(default = 15);
{
	UnicodeString lbuf;
	if (isBinary) {
		unsigned int adr = idx*16;
		if (adr<BinarySize) {
			UnicodeString abuf = " ";	//ASCII表示
			for (int i=0; i<16; i++,adr++) {
				if (i==8) lbuf += " ";
				if (i>=b0 && i<=b1 && adr<BinarySize) {
					unsigned char c = MMF->Bytes[adr];
					lbuf.cat_sprintf(_T("%02X "), c);
					abuf.cat_sprintf(_T("%c"), isprint(c)? c : '.');
				}
				else {
					lbuf += "   ";
					abuf += " ";
				}
			}
			lbuf += abuf;
		}
	}
	else {
		if (idx>=0 && idx<DispLines->Count) lbuf = DispLines->Strings[idx];
	}
	return lbuf;
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::Clear()
{
	isReady = false;

	TxtBufList->Clear();
	MaxLine = 0;
	ClearDispLine();

	AssignScaledFont(ViewBox->Font, ViewerFont, OwnerForm);
	useFontName = ViewBox->Font->Name;
	useFontSize = ViewBox->Font->Size;

	isSelected = false;
	isText	   = isBinary = isXDoc2Txt = false;
	isCSV	   = isTSV    = false;
	isClip	   = false;
	isLog	   = false;
	isTail	   = false;
	isAozora   = isLog = isIniFmt = isAwstats = isJsonFmt = false;

	isIncSea   = false;
	IncSeaWord = EmptyStr;
	UserModule->SetBlinkTimer(NULL);

	FindWord   = RegExPtn = EmptyStr;

	HasBOM = false;
	LineBreakStr = EmptyStr;

	MMF->Close();

	ImgBuff->Handle = NULL;

	LastPos = Point(-1, -1);
	LastTop = -1;
	LastSel = false;

	if (FileRec) {
		del_file_rec(FileRec);
		FileRec = NULL;
	}

	SetColor();
	TabLength = 0;
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::ClearDispLine()
{
	ScrPanel->HitLines->Clear();

	for (int i=0; i<DispLines->Count; i++) delete (line_rec*)DispLines->Objects[i];
	DispLines->Clear();
	MaxDispLine = 0;

	TxtBufList2->Clear();
	FixWdList.Length = 0;

	JsonErrMsg = EmptyStr;
}

//---------------------------------------------------------------------------
//配色の設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetColor(TStringList *lst)
{
	if (!lst) return;

	color_Cursor	= read_ColorList(_T("Cursor"),		col_Cursor,		lst);
	color_selItem 	= read_ColorList(_T("selItem"),		col_selItem,	lst);
	color_fgSelItem = read_ColorList(_T("fgSelItem"),	col_fgSelItem,	lst);
	color_Folder	= read_ColorList(_T("Folder"),		col_Folder,		lst);
	color_Error		= read_ColorList(_T("Error"),		col_Error,		lst);
	color_bgView	= read_ColorList(_T("bgView"),		col_bgView,		lst);
	color_fgView	= read_ColorList(_T("fgView"),		col_fgView,		lst);
	color_Margin	= read_ColorList(_T("Margin"),		col_Margin,		lst);
	color_bgRuler	= read_ColorList(_T("bgRuler"),		col_bgRuler,	lst);
	color_fgRuler	= read_ColorList(_T("fgRuler"),		col_fgRuler,	lst);
	color_bgLineNo	= read_ColorList(_T("bgLineNo"),	col_bgLineNo,	lst);
	color_LineNo	= read_ColorList(_T("LineNo"),		col_LineNo,		lst);
	color_Mark		= read_ColorList(_T("Mark"),		col_Mark,		lst);
	color_bdrLine	= read_ColorList(_T("bdrLine"),		col_bdrLine,	lst);
	color_bdrFold	= read_ColorList(_T("bdrFold"),		col_bdrFold,	lst);
	color_bdrFixed	= read_ColorList(_T("bdrFixed"),	col_bdrFixed,	lst);
	color_Comment	= read_ColorList(_T("Comment"),		col_Comment,	lst);
	color_Strings	= read_ColorList(_T("Strings"),		col_Strings,	lst);
	color_Reserved	= read_ColorList(_T("Reserved"),	col_Reserved,	lst);
	color_Symbol	= read_ColorList(_T("Symbol"),		col_Symbol,		lst);
	color_Numeric	= read_ColorList(_T("Numeric"),		col_Numeric,	lst);
	color_fgEmpBin1 = read_ColorList(_T("fgEmpBin1"),	col_fgEmpBin1,	lst);
	color_fgEmpBin2 = read_ColorList(_T("fgEmpBin2"),	col_fgEmpBin2,	lst);
	color_fgEmpBin3 = read_ColorList(_T("fgEmpBin3"),	col_fgEmpBin3,	lst);
	color_Headline	= read_ColorList(_T("Headline"),	col_Headline,	lst);
	color_URL		= read_ColorList(_T("URL"),			col_URL,		lst);
	color_LocalLink = read_ColorList(_T("LocalLink"),	col_LocalLink,	lst);
	color_fgEmp 	= read_ColorList(_T("fgEmp"),		col_fgEmp,		lst);
	color_bgEmp 	= read_ColorList(_T("bgEmp"),		col_bgEmp,		lst);
	color_Ruby		= read_ColorList(_T("Ruby"),		col_Ruby,		lst);
	color_TAB		= read_ColorList(_T("TAB"),			col_TAB,		lst);
	color_CR		= read_ColorList(_T("CR"),			col_CR,			lst);
	color_HR		= read_ColorList(_T("HR"),			col_HR,			lst);
	color_Ctrl		= read_ColorList(_T("Ctrl"),		col_Ctrl,		lst);

	if (lst!=ColBufList) ColBufList->Assign(lst);
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetColor(UnicodeString prm)
{
	ColBufList->Clear();

	if (!USAME_TI(prm, "RS")) {
		UnicodeString fnam = to_absolute_name(prm);
		if (file_exists(fnam)) {
			std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(fnam));
			inp_file->ReadSection("Color", ColBufList);
		}
	}

	SetColor(ColBufList);
}

//---------------------------------------------------------------------------
//オプション設定に配色を反映
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetOptColor()
{
	col_Cursor	  = color_Cursor;
	col_selItem   = color_selItem;
	col_fgSelItem = color_fgSelItem;
	col_Folder	  = color_Folder;
	col_Error	  = color_Error;
	col_bgView	  = color_bgView;
	col_fgView	  = color_fgView;
	col_Margin	  = color_Margin;
	col_bgRuler   = color_bgRuler;
	col_fgRuler   = color_fgRuler;
	col_bgLineNo  = color_bgLineNo;
	col_LineNo	  = color_LineNo;
	col_Mark	  = color_Mark;
	col_bdrLine   = color_bdrLine;
	col_bdrFold   = color_bdrFold;
	col_Comment   = color_Comment;
	col_Strings   = color_Strings;
	col_Reserved  = color_Reserved;
	col_Symbol	  = color_Symbol;
	col_Numeric   = color_Numeric;
	col_fgEmpBin1 = color_fgEmpBin1;
	col_fgEmpBin2 = color_fgEmpBin2;
	col_fgEmpBin3 = color_fgEmpBin3;
	col_Headline  = color_Headline;
	col_URL 	  = color_URL;
	col_LocalLink = color_LocalLink;
	col_fgEmp	  = color_fgEmp;
	col_bgEmp	  = color_bgEmp;
	col_Ruby	  = color_Ruby;
	col_TAB 	  = color_TAB;
	col_CR		  = color_CR;
	col_HR		  = color_HR;
	col_Ctrl	  = color_Ctrl;

	for (int i=0; i<ColBufList->Count; i++) {
		int idx = ColorList->IndexOfName(ColBufList->Names[i]);
		if (idx!=-1) ColorList->Strings[idx] = ColBufList->Strings[i];
	}
}

//---------------------------------------------------------------------------
//メトリックの設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetMetric(bool set_hi)
{
	isFitWin = isExtWnd? true : ViewFoldFitWin;

	IsIrregFont = IsIrregularFont(ViewBox->Font);

	//ズーム時の行間調整
	int zm_InterLn = ViewTxtInterLn;
	if (ViewBox->Font->Size != useFontSize) zm_InterLn *= (1.0 * ViewBox->Font->Size/useFontSize);
	ViewCanvas->Font->Assign(ViewBox->Font);
	FontHeight = abs(ViewCanvas->Font->Height);
	LineHeight = FontHeight + ScaledInt(zm_InterLn, OwnerForm);
	LineCount  = ViewBox->ClientHeight / LineHeight - 1;

	if (TabLength==0) TabLength = get_ViewTabWidth(get_extension(FileName));
	HchWidth   = get_TextWidth(ViewCanvas, "W", IsIrregFont);
	TabWidth   = TabLength * HchWidth;

	LeftMargin = isExtWnd? 0 : ViewLeftMargin;
	TopXpos    = LeftMargin + MARK_WIDTH;

	if (isBinary)
		TopXpos += (HchWidth * (get_AddrStr(BinFileSize - 1).Length() + 2));
	else if (ShowLineNo)
		TopXpos += (HchWidth * 8);

	TopMargin = HchWidth;

	//スクロールバー位置の設定
	if (MarginBox) {
		MaxFoldWd = (ViewBox->ClientWidth + MarginBox->Width - HchWidth*2 - TopXpos) / HchWidth;
		//折り返し位置
		if (ScrBarToFoldPos && (isBinary || (!isFitWin && ViewFoldWidth>0))) {
			int w = ViewBox->ClientWidth + MarginBox->Width;
			w -= (TopXpos + ((isBinary? MAX_BIN_HCH_X : ViewFoldWidth) + 2) * HchWidth);
			if (w>=0) MarginBox->Width = w;
		}
		//右端
		else {
			MarginBox->Width = 0;
		}
	}
	else {
		MaxFoldWd = (ViewBox->ClientWidth - HchWidth*2 - TopXpos) / HchWidth;
	}

	if (ColorPanel) {
		ColorPanel->ClientHeight = FontHeight;
		ColorPanel->ClientWidth  = FontHeight * 2;
	}

	if (set_hi) {
		ImgBuff->Handle = NULL;
		SetScrBar();
		UpdatePos(true, true);
	}
}

//---------------------------------------------------------------------------
//スクロールバー更新
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetScrBar()
{
	if (ScrBar && MaxDispLine>0) {
		ScrBar->Visible = (ScrBarStyle==0);

		ScrBar->Max = MaxDispLine + 1;
		ScrBar->LargeChange = LineCount;

		SCROLLINFO si = {sizeof(SCROLLINFO)};
		si.fMask = SIF_PAGE;
		si.nPage = LineCount + 1;
		::SetScrollInfo(ScrBar->Handle, SB_CTL, &si, TRUE);

		ScrPanel->UpdateKnob();
		ScrPanel->Repaint();
	}
}

//---------------------------------------------------------------------------
//配列をTSV形式に変換
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::ArrayToTsv(TStringDynArray lst)
{
	UnicodeString ret_str;
	for (int i=0; i<lst.Length; i++) {
		if (i>0) ret_str += "\t";
		ret_str += lst[i];
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//CSV/TSV を固定長表示に整形
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::FormatFixed(TStringList *txt_lst)
{
	if (txt_lst->Count==0) return;

	//１行目
	bool is_tsv = ContainsStr(txt_lst->Strings[0], "\t");
	TStringDynArray hdr_buf = is_tsv? split_strings_tab(txt_lst->Strings[0]) :
									  get_csv_array(txt_lst->Strings[0], MAX_CSV_ITEM);
	if (hdr_buf.Length==0) return;

	int c_cnt = hdr_buf.Length;
	IsNumList.Length = c_cnt;
	FixWdList.Length = c_cnt;
	for (int i=0; i<c_cnt; i++) {
		FixWdList[i] = std::max(str_len_half(hdr_buf[i]), 2);
		IsNumList[i] = true;
	}
	if (!is_tsv) txt_lst->Strings[0] = ArrayToTsv(hdr_buf);

	//２行目以降
	for (int i=1; i<txt_lst->Count; i++) {
		UnicodeString lbuf = txt_lst->Strings[i];
		if (i==txt_lst->Count-1 && USAME_TS(lbuf, TXLIMIT_MARK)) break;

		TStringDynArray itm_buf = is_tsv? split_strings_tab(lbuf) : get_csv_array(lbuf, c_cnt, true);
		itm_buf.Length = c_cnt;
		for (int j=0; j<c_cnt; j++) {
			//最大幅
			int hlen = str_len_half(itm_buf[j]);
			if (hlen>FixWdList[j]) FixWdList[j] = hlen;
			//数値チェック
			if (IsNumList[j]) {
				UnicodeString s = Trim(itm_buf[j]);
				for (int k=1; k<=s.Length(); k++) {
					if (!iswdigit(s[k]) && s[k]!='.' && s[k]!=',' && s[k]!='-' && s[k]!='+') {
						IsNumList[j] = false; break;
					}
				}
			}
		}
		if (!is_tsv) txt_lst->Strings[i] = ArrayToTsv(itm_buf);
	}

	//列幅の最小化
	TStringDynArray itm_buf = get_csv_array(read_NyanFiDef(FileName, "MinFixedCols"), c_cnt);
	for (int i=0; i<itm_buf.Length; i++) {
		int idx = itm_buf[i].ToIntDef(-1);
		if (idx>=0 && idx<c_cnt) FixWdList[idx] = (idx<26)? 0 : 1;
	}

	//幅制限
	if (ViewFixedLimit>0 && ViewFixedLimit<4) ViewFixedLimit = 4;	//***
	if (ViewFixedLimit>0) {
		DynamicArray<int> wd_buf = FixWdList.Copy();
		//制限
		int fld_wd = !isFitWin? ViewFoldWidth : MaxFoldWd;
		for (int i=c_cnt-1; i>=0; i--) {
			int rec_w = 0;
			for (int j=0; j<c_cnt; j++) rec_w += (wd_buf[j] + ((j>0)? 2 : 0));
			if (rec_w<=fld_wd) break;
			wd_buf[i] = std::min(wd_buf[i], ViewFixedLimit);
		}

		int mgn = fld_wd;
		for (int i=0; i<c_cnt; i++) mgn -= (wd_buf[i] + ((i>0)? 2 : 0));
		//余裕があれば再調整
		if (mgn>0) {
			for (;;) {
				int cnt = 0;
				for (int i=c_cnt-1; i>=0 && mgn>0; i--) {
					if (wd_buf[i]>=2 && wd_buf[i]<FixWdList[i]) {
						wd_buf[i] += 1; mgn--;  cnt++;
					}
				}
				if (cnt==0 || mgn==0) break;
			}
		}

		FixWdList = wd_buf.Copy();
	}

	//整形
	for (int i=0; i<txt_lst->Count; i++) {
		UnicodeString lbuf = txt_lst->Strings[i];
		if (i==txt_lst->Count-2 && lbuf.IsEmpty()) continue;
		if (i==txt_lst->Count-1 && USAME_TS(lbuf, TXLIMIT_MARK)) break;

		UnicodeString nbuf;
		for (int j=0; j<c_cnt; j++) {
			UnicodeString s = split_pre_tab(lbuf);
			if (j>0) nbuf += "  ";
			int w_j = FixWdList[j];
			if (w_j>=2) {
				int dlen = w_j - str_len_half(s);
				//省略
				if (dlen<0) {
					int s_w = w_j - 2;
					for (int k=1; k<=s.Length(); k++) {
						if (str_len_half(s.SubString(1, k)) > s_w) {
							s = s.SubString(1, k - 1) + "…";
							break;
						}
					}
					dlen = w_j - str_len_half(s);
				}
				//空白付加
				if (dlen>0) {
					if (IsNumList[j])
						s = StringOfChar(_T(' '), dlen) + s;
					else
						s += StringOfChar(_T(' '), dlen);
				}
				nbuf += s;
			}
			else if (w_j>0) {
				nbuf += " ";
			}
		}
		txt_lst->Strings[i] = nbuf;
	}
}

//---------------------------------------------------------------------------
//CSV見出しリストを取得
//---------------------------------------------------------------------------
TStringDynArray __fastcall TTxtViewer::GetCsvHdrList()
{
	UnicodeString lbuf = (TxtBufList->Count>0)? TxtBufList->Strings[0] : EmptyStr;
	TStringDynArray ret_array = ContainsStr(lbuf, "\t")? split_strings_tab(lbuf) : get_csv_array(lbuf, MAX_CSV_ITEM);
	if (!TopIsHeader) {
		for (int i=0; i<ret_array.Length; i++) ret_array[i] = UnicodeString().sprintf(_T("項目%u"), i + 1);
	}
	return ret_array;
}

//---------------------------------------------------------------------------
//画面に合わせて行内容を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::UpdateScr(
	int lno)	//行番号 0 = 現在行		(default = 1)
{
	isReady = false;
	cursor_HourGlass();

	if (UserHighlight->Recycle()) AddErr_Highlight();

	if (lno==0) lno = get_CurLineNo();
	int cno = 1;

	if (RulerBox) {
		RulerBox->Visible = ShowTextRuler;
		if (RulerBox->Visible) {
			RulerBox->Height = ScaledInt(16, OwnerForm);
			RulerBox->Repaint();
		}
	}

	UnicodeString fext = get_extension(FileName);

	bool is_xml = false;
	if (!isBinary && TxtBufList->Count>0)
		is_xml = (StartsText("<?xml ", TxtBufList->Strings[0]) || test_FileExt(fext, FEXT_XML));

	//青空文庫か？
	isAozora = ChkAozora && test_FileExt(fext, ".txt") && is_match_regex(TxtBufList->Text, _T("［＃.*?］"));

	isIniFmt = isAwstats = isNyanTxt = false;
	if (!isBinary && !isLog && !is_xml && !isAozora) {
		//セクションを持っているか？
		isIniFmt = test_FileExt(fext, ".ini.inf.reg.url");
		if (!isIniFmt && !test_FileExt(fext, _T(".txt.log") FEXT_PROGRAM FEXT_HTML)) {
			TRegExOptions opt; opt << roMultiLine;
			isIniFmt = TRegEx::IsMatch(TxtBufList->Text, "^\\[[a-zA-Z0-9_ ]+\\]$", opt);
		}
		//AWStatsデーか？
		if (test_FileExt(fext, ".txt") && StartsStr("awstats", ExtractFileName(FileName))) {
			isAwstats = StartsStr("AWSTATS DATA FILE", TxtBufList->Text);
		}
		//NyanFi用テキストか？
		if (!isIniFmt) {
			UnicodeString lbuf = get_top_line(FileName);
			isNyanTxt = USAME_TI(lbuf, ";[MenuFile]") || USAME_TI(lbuf, ";[ResultList]");
		}
	}

	//見出しパターンを設定
	HeadlinePtn = EmptyStr;
	if (EmpHeadline && !isBinary) {
		//拡張子依存
		HeadlinePtn = UserHighlight->GetDefHeadlnPtn(fext);
		//内容/モード依存
		if (HeadlinePtn.IsEmpty()) {
			if (isAozora) {
				HeadlinePtn = "［＃.*?(大|中)見出し］|^　*?([０-９]+|[一二三四五六七八九十壱弐参拾百〇]+)$";
			}
			else if (isAwstats) {
				HeadlinePtn = "^BEGIN_.+?";
			}
			else if (isLog) {
				HeadlinePtn = "^.>\\d{2}:\\d{2}:\\d{2}\\s(NyanFi|.+(開始|接続))";
			}
			else if (test_HtmlExt(fext)) {
				if (isHtm2Txt) {
					if (ToMarkdown) {
						HeadlinePtn = "^#+\\s";
					}
					else if (!HtmHdrStr.IsEmpty()) {
						HeadlinePtn = "^(";
						UnicodeString hbuf = HtmHdrStr;
						for (int i=0; i<6; i++) {
							UnicodeString hc = split_tkn(hbuf, ';');
							if (!hc.IsEmpty()) {
								if (i>0) HeadlinePtn += "|";
								HeadlinePtn += hc;
							}
						}
						HeadlinePtn += ").+";
					}
				}
				else {
					HeadlinePtn = "<[hH][1-6]";
				}
			}
			else if (isIniFmt) {
				HeadlinePtn = "^\\[.+?\\]";
			}
			else {
				HeadlinePtn = HeadlineList->Values[get_tkn_r(fext, '.').LowerCase()];
			}
		}

		if (!chk_RegExPtn(HeadlinePtn)) HeadlinePtn = EmptyStr;
	}

	//--------------------------
	//構文強調を取得
	//--------------------------
	RemLnList->Clear();  RemBgnList->Clear();  RemEndList->Clear();
	ReservedPtn   = EmptyStr;
	ReservedCase  = true;
	NumericPtn	  = EmptyStr;
	SymbolChs	  = EmptyStr;
	QuotStr 	  = EmptyStr;
	useEsc		  = true;
	CharPtn 	  = EmptyStr;
	RubyPtn 	  = EmptyStr;
	UsrKeyword	  = EmptyStr;
	UsrKeywdCase  = true;
	UsrKeyword2   = EmptyStr;
	UsrKeywdCase2 = true;
	UsrKeywdCol   = color_fgView;
	UsrKeywdCol2  = color_fgView;
	alt_BackSlash = AltBackSlash;
	PairPtnList->Clear();

	//ユーザ定義の取得
	bool usr_hl = UserHighlight->GetSection(FileName, isClip, isLog, isHtm2Txt);

	//フォント
	useFontName = usr_hl? UserHighlight->ReadKeyStr(_T("FontName")) : ViewerFont->Name;
	if (useFontName.IsEmpty()) useFontName = ViewerFont->Name;
	useFontSize = ScaledInt(usr_hl? UserHighlight->ReadKeyInt(_T("FontSize")) : ViewerFont->Size, OwnerForm);;
	if (useFontSize==0) useFontSize = ScaledInt(ViewerFont->Size, OwnerForm);
	ViewBox->Font->Assign(ViewerFont);
	ViewBox->Font->Name = useFontName;
	ViewBox->Font->Size = useFontSize;

	if (!isBinary) {
		if (usr_hl) {
			UnicodeString key;
			//行コメント
			for (int i=1; ; i++) {
				UnicodeString vstr = UserHighlight->ReadKeyStr(key.sprintf(_T("Comment%u"), i).c_str());
				if (vstr.IsEmpty()) break;
				RemLnList->Add(vstr);
			}
			//ブロックコメント
			for (int i=1; ; i++) {
				UnicodeString b_str = UserHighlight->ReadKeyStr(key.sprintf(_T("CommentBgn%u"), i).c_str());
				UnicodeString e_str = UserHighlight->ReadKeyStr(key.sprintf(_T("CommentEnd%u"), i).c_str());
				if (b_str.IsEmpty() || e_str.IsEmpty()) break;
				RemBgnList->Add(b_str);
				RemEndList->Add(e_str);
			}

			ReservedPtn   = UserHighlight->ReadRegExPtn(_T("ReservedPtn"));
			ReservedCase  = UserHighlight->ReadKeyBool( _T("ReservedCase"),	true);
			NumericPtn	  = UserHighlight->ReadRegExPtn(_T("NumericPtn"));
			SymbolChs	  = UserHighlight->ReadKeyStr(  _T("Symbol"));
			QuotStr 	  = UserHighlight->ReadKeyStr(  _T("Strings"));
			useEsc		  = UserHighlight->ReadKeyBool( _T("UseEsc"),		true);
			UsrKeyword	  = UserHighlight->ReadRegExPtn(_T("KeywordPtn"));
			UsrKeywdCase  = UserHighlight->ReadKeyBool( _T("KeywordCase"),	true);
			UsrKeyword2   = UserHighlight->ReadRegExPtn(_T("KeywordPtn2"));
			UsrKeywdCase2 = UserHighlight->ReadKeyBool( _T("KeywordCase2"),	true);

			//SearchPair 用パターン
			UnicodeString bgn_ptn, end_ptn, tmp;
			for (int i=1; ; i++) {
				tmp.sprintf(_T("PairBeginPtn%u"), i);
				bgn_ptn = UserHighlight->ReadRegExPtn(tmp.c_str());
				tmp.sprintf(_T("PairEndPtn%u"), i);
				end_ptn = UserHighlight->ReadRegExPtn(tmp.c_str());
				if (bgn_ptn.IsEmpty() || end_ptn.IsEmpty()) break;
				PairPtnList->Add(bgn_ptn + "\t" + end_ptn);
			}

			//見出し行
			UnicodeString lbuf = UserHighlight->ReadRegExPtn(_T("HeadlinePtn"));
			if (!lbuf.IsEmpty()) HeadlinePtn = lbuf;

			//配色
			UnicodeString col_fnam = UserHighlight->ReadKeyStr(_T("ColorIniFile"));
			if (!col_fnam.IsEmpty()) SetColor(col_fnam);
			color_Comment  = UserHighlight->ReadColorRGB6H(_T("CommentCol"),	color_Comment);
			color_Reserved = UserHighlight->ReadColorRGB6H(_T("ReservedCol"),	color_Reserved);
			color_Numeric  = UserHighlight->ReadColorRGB6H(_T("NumericCol"),	color_Numeric);
			color_Symbol   = UserHighlight->ReadColorRGB6H(_T("SymbolCol"),		color_Symbol);
			color_Strings  = UserHighlight->ReadColorRGB6H(_T("StringsCol"),	color_Strings);
			color_Headline = UserHighlight->ReadColorRGB6H(_T("HeadlineCol"),	color_Headline);
			UsrKeywdCol    = UserHighlight->ReadColorRGB6H(_T("KeywordCol"),	color_fgView);
			UsrKeywdCol2   = UserHighlight->ReadColorRGB6H(_T("KeywordCol2"),	color_fgView);

			//その他
			if (UserHighlight->KeyExists(UserHighlight->CurSection, _T("AltBackSlash")))
				alt_BackSlash = UserHighlight->ReadKeyBool(_T("AltBackSlash"));
		}
		//デフォルト
		else {
			//コメント
			if (EmpComment) {
				if (isAwstats)
					RemLnList->Add("#");
				else if (test_FileExt(fext, ".bat.cmd"))
					RemLnList->Add("$BAT");
				else if (test_FileExt(fext, ".nbt.qbt"))
					RemLnList->Add("$QBT");
				else if (isIniFmt || isNyanTxt)
					RemLnList->Add("$INI");
				else
					UserHighlight->GetCommentList(FileName, RemLnList, RemBgnList, RemEndList, isHtm2Txt, true);
			}

			//予約語
			if (EmpReserved) ReservedPtn = GetDefReservedPtn(fext, ReservedCase, is_xml, isLog, isHtm2Txt);
			//数値
			if (EmpNumeric)  NumericPtn  = GetDefNumericPtn(fext);
			//シンボル
			if (EmpSymbol)   SymbolChs   = GetDefSymbolChars(fext, is_xml, isHtm2Txt);

			//引用符
			if (EmpStrings) {
				QuotStr = GetDefQuotChars(fext, useEsc, is_xml, isIniFmt, isHtm2Txt);
				if (test_FileExt(fext, FEXT_C_SH _T(".idl.cs.hs.js.jsx.java.vhd"))) CharPtn = "'\\\\?.'";
			}

			//SearchPair 用パターン
			GetSearchPairPtn(fext, PairPtnList);

			SetColor();
		}

		//ルビ
		if (EmpRuby) {
			if (isAozora)
				RubyPtn = "《.*?》|｜";
			else if (test_HtmlExt(fext) && isHtm2Txt)
				RubyPtn = "(\\(|（)[ぁ-んァ-ヶ]+(\\)|）)";
		}
	}

	//--------------------------
	//強調表示パターンを設定
	//--------------------------
	//0									予約語
	EmPtn[0] = ReservedPtn;
	EmBgC[0] = col_None;
	EmFgC[0] = color_Reserved;
	//1									URL
	EmPtn[1] = "(" URL_MATCH_PTN ")|(" MAIL_MATCH_PTN ")";
	EmBgC[1] = col_None;
	EmFgC[1] = color_URL;
	//2									検索強調語
	EmPtn[2] = EmptyStr;
	EmBgC[2] = color_bgEmp;
	EmFgC[2] = color_fgEmp;

	//3
	if (isLog && UserHighlight->CurSection.IsEmpty()) {	//ディレクトリ
		EmPtn[3] = "(([a-zA-Z]:|\\\\)\\\\([^/*?\"<>|]+\\\\)*)|(\\[.*?\\])";
		EmFgC[3] = color_Folder;
	}
	else if (!CharPtn.IsEmpty()) {		//文字
		EmPtn[3] = CharPtn;
		EmFgC[3] = color_Strings;
	}
	else if (isBinary) {				//バイナリ強調3
		EmPtn[3] = EmpBinPtn3;
		EmFgC[3] = color_fgEmpBin3;
	}
	else {								//ローカルファイル
		EmPtn[3] = LOCAL_FILE_PTN;
		EmFgC[3] = color_LocalLink;
	}
	EmBgC[3] = col_None;

	//4
	if (isBinary) {						//バイナリ強調2
		EmPtn[4] = EmpBinPtn2;
		EmFgC[4] = color_fgEmpBin2;
	}
	else if (!NumericPtn.IsEmpty())	{	//数値
		EmPtn[4] = NumericPtn;
		EmFgC[4] = color_Numeric;
	}
	else if (isLog) {					//ログのデバッグ情報
		EmPtn[4] = "^.>       !( .+)?";
		EmFgC[4] = AdjustColor(color_fgView, ADJCOL_FGLIST);
	}
	else {								//ルビ
		EmPtn[4] = RubyPtn;
		EmFgC[4] = color_Ruby;
	}
	EmBgC[4] = col_None;

	//5
	if (isBinary) {						//バイナリ強調1
		EmPtn[5] = EmpBinPtn1;
		EmFgC[5] = color_fgEmpBin1;
	}
	else if (isLog && UserHighlight->CurSection.IsEmpty()) {	//ログのエラー等
		EmPtn[5] = "(^.>([ECW]|(     [45]\\d{2})) .*)|(\\b(ERR|NG):\\d+)";
		EmFgC[5] = color_Error;
	}
	else if (isAozora) {				//青空文庫
		EmPtn[5] = "※|［＃.*?］";
		EmFgC[5] = color_Comment;
	}
	else {								//ユーザ定義キーワード
		EmPtn[5] = UsrKeyword;
		EmFgC[5] = UsrKeywdCol;
	}
	EmBgC[5] = col_None;

	//6									ユーザ定義キーワード2
	EmPtn[6] = UsrKeyword2;
	EmFgC[6] = UsrKeywdCol2;
	EmBgC[6] = col_None;
	//--------------------------

	SetMetric();
	SelStart = SelEnd = CurPos = Point(0, 0);
	LastPos  = Point(-1, -1);
	LastTop  = -1;
	LastSel  = false;

	//表示バッファを設定
	//バイナリダンプ
	if (isBinary) {
		MaxHchX = MAX_BIN_HCH_X;
	}
	//テキスト
	else {
		ClearDispLine();

		int max_wd;
		if (!isFitWin && ViewFoldWidth>0) {
			max_wd	= (ViewFoldWidth - 1) * HchWidth;
			MaxHchX = ViewFoldWidth;
		}
		else {
			max_wd	 = ViewBox->ClientWidth - HchWidth*2 - TopXpos;
			MaxHchX  = MaxFoldWd + 1;
			isFitWin = true;
		}

		isLimited = isContinue = false;

		std::unique_ptr<TStringList> txt_buf(new TStringList());
		txt_buf->Assign(TxtBufList);

		//HTML→テキスト変換
		if (test_HtmlExt(fext) && isHtm2Txt) {
			std::unique_ptr<HtmConv> htmcnv(new HtmConv());
			ini_HtmConv_def(htmcnv.get(), FileName);
			htmcnv->LineWidth = MaxHchX;
			htmcnv->InsHdrInf = true;
			htmcnv->HtmBuf->Assign(txt_buf.get());
			htmcnv->Convert();
			for (int i=0; i<htmcnv->TxtBuf->Count; i++) htmcnv->TxtBuf->Objects[i] = (TObject*)(NativeInt)i;
			TxtBufList2->Assign(htmcnv->TxtBuf);
			txt_buf->Assign(htmcnv->TxtBuf);
		}
		//.json ファイルを整形
		else if (test_FileExt(fext, ".json")) {
			if (FormatJson) {
				try {
					std::unique_ptr<TStringList> jbuf(new TStringList());
					format_Json(TJSONObject::ParseJSONValue(txt_buf->Text, false, true), jbuf.get());	//RaiseExc = true
					for (int i=0; i<jbuf->Count; i++) jbuf->Objects[i] = (TObject*)(NativeInt)i;
					TxtBufList2->Assign(jbuf.get());
					txt_buf->Assign(jbuf.get());
					isJsonFmt = true;
				}
				catch (EJSONParseException &e) {
					JsonErrMsg = e.Message;
					lno = e.Line;
					cno = e.Position;
				}
			}
		}
		//.dfm ファイル内の文字列をデコード
		else if (test_FileExt(fext, ".dfm")) {
			if (DecodeDfmStr) conv_DfmText(txt_buf.get());
		}
		//CSV/TSV(固定長表示)
		else if (test_FileExt(fext, FEXT_CSV)) {
			isCSV = true;
			isTSV = (txt_buf->Count>0 && ContainsStr(txt_buf->Strings[0], "\t"));
			bool ok = false;
			std::unique_ptr<TStringList> fbuf(new TStringList());
			if (load_NyanFiDef(FileName, fbuf.get())) {
				UnicodeString v = fbuf->Values["TopIsHeader"];
				if (!v.IsEmpty()) {
					TopIsHeader = SameStr(v, "1");
					ok = true;
				}
			}
			if (!ok) {
				fbuf->Values["TopIsHeader"] = FTopIsHeader? "1" : "0";
				save_NyanFiDef(FileName, fbuf.get());
			}
			if (isFixedLen) FormatFixed(txt_buf.get());
		}
		//青空文庫形式
		else if (isAozora) {
			int ind_n   = 0;
			for (int i=0; i<txt_buf->Count; i++) {
				UnicodeString lbuf = txt_buf->Strings[i];
				if (StartsStr("［＃", lbuf)) {
					//１行字下げ
					if (is_match_regex(lbuf, _T("［＃[１-９]+字下げ］"))) {
						lbuf  = replace_regex(lbuf, _T("［＃([１-９]+)字下げ］"), _T("\\1\t"));
						ind_n = to_HalfWidth(get_pre_tab(lbuf)).ToIntDef(0);
						if (ind_n>0)
							txt_buf->Strings[i] = StringOfChar(_T('　'), ind_n) + get_post_tab(lbuf);
					}
					//字上げ
					else if (is_match_regex(lbuf, _T("［＃地から[１-９]+字上げ］"))) {
						lbuf  = replace_regex(lbuf, _T("［＃地から([１-９]+)字上げ］"), _T("\\1\t"));
						ind_n = to_HalfWidth(get_pre_tab(lbuf)).ToIntDef(0);
						if (ind_n>0) {
							lbuf  = get_post_tab(lbuf);
							ind_n = MaxHchX/2 - lbuf.Length()-ind_n;
							if (ind_n>0) txt_buf->Strings[i] = StringOfChar(_T('　'), ind_n) + lbuf;
						}
					}
					//地付き
					else if (USAME_TS(lbuf, "［＃地付き］")) {
						lbuf.Delete(1, 6);
						ind_n = MaxHchX/2 - lbuf.Length();
						if (ind_n>0) txt_buf->Strings[i] = StringOfChar(_T('　'), ind_n) + lbuf;
					}
					//改丁、改ページ
					else if (USAME_TS(lbuf, "［＃改丁］") || USAME_TS(lbuf, "［＃改ページ］")) {
						txt_buf->Strings[i] = StringOfChar(_T('─'), MaxHchX/2);
					}
				}
				//罫線
				else {
					if (i<30 && is_match_regex(lbuf, _T("^-{10,}$")))
						txt_buf->Strings[i] = StringOfChar(_T('─'), MaxHchX/2);
				}
			}
		}

		//ルビ非表示
		if (!ShowRuby && !RubyPtn.IsEmpty()) {
			for (int i=0; i<txt_buf->Count; i++)
				txt_buf->Strings[i] = TRegEx::Replace(txt_buf->Strings[i], RubyPtn, EmptyStr);
			RubyPtn = EmptyStr;
		}

		MaxLine = txt_buf->Count;
		if (lno>MaxLine) lno = MaxLine;

		bool isNrm	= true;
		int  ind_n	= -1;	//字下げ
		int  ind_n2 = -1;	//折り返し後の字下げ
		int  alr_n	= -1;	//地からの字上げ

		bool q_in_rem = false;
		if (!QuotStr.IsEmpty()) {
			for (int i=0; i<RemBgnList->Count && !q_in_rem; i++) {
				for (int j=1; j<=RemBgnList->Strings[i].Length() && !q_in_rem; j++) {
					if (ContainsStr(QuotStr, RemBgnList->Strings[i][j])) q_in_rem = true;
				}
			}
			for (int i=0; i<RemEndList->Count && !q_in_rem; i++) {
				for (int j=1; j<=RemEndList->Strings[i].Length() && !q_in_rem; j++) {
					if (ContainsStr(QuotStr, RemEndList->Strings[i][j])) q_in_rem = true;
				}
			}
		}

		for (int i=0; i<MaxLine; i++) {
			UnicodeString lbuf = txt_buf->Strings[i];
			int org_lno  = (int)txt_buf->Objects[i] + 1;
			int lbuf_len = lbuf.Length();

			//文字列の解析
			std::unique_ptr<WideChar []> qch_buf(new WideChar[lbuf_len + 1]);
			WideChar *qch_ln = qch_buf.get();
			WideChar qch = '\0';
			if (!QuotStr.IsEmpty()) {
				bool isEsc = false;
				for (int j=1; j<=lbuf_len; j++) {
					WideChar cj = lbuf[j];
					if		(isEsc)				 isEsc = false;
					else if (useEsc && cj=='\\') isEsc = true;
					else if (ContainsText(QuotStr, cj))	qch = (qch==cj)? '\0' : cj;
					qch_ln[j] = qch;
				}
			}
			else {
				for (int j=1; j<=lbuf_len; j++) qch_ln[j] = '\0';
			}

			//コメントの解析
			int RemPos0 = 0, RemPos1 = 0;
			if (RemLnList->Count>0 || RemBgnList->Count>0) {
				UnicodeString tbuf = TrimLeft(lbuf);
				bool ok = false;
				if (RemLnList->Count>0) {
					ok = true;
					if (USAME_TS(RemLnList->Strings[0], "$BAT")) {
						if (StartsText("REM", tbuf) || StartsText("@REM",tbuf) || StartsStr("::", tbuf)) {
							RemPos0 = 1;  RemPos1 = lbuf_len;
						}
					}
					else if (USAME_TS(RemLnList->Strings[0], "$QBT")) {
						if (StartsText("REM", tbuf) || StartsStr(';', tbuf)) {
							RemPos0 = 1;  RemPos1 = lbuf_len;
						}
					}
					else if (USAME_TS(RemLnList->Strings[0], "$INI")) {
						if (StartsStr(';', lbuf)) {
							RemPos0 = 1;  RemPos1 = lbuf_len;
						}
					}
					else {
						ok = false;
					}
				}

				if (!ok) {
					int j = 1;
					while (j<=lbuf_len) {
						bool matched = false;
						WideChar qch = qch_ln[j];
						//文字列内ではない
						if (qch=='\0') {
							//１行コメント
							if (isNrm) {
								for (int i_r=0; i_r<RemLnList->Count; i_r++) {
									int rn = RemLnList->Strings[i_r].Length();
									for (int k=1; k<=rn; k++) {
										int cp = j + k - 1;  if (cp>lbuf_len) break;
										if (RemLnList->Strings[i_r][k]!=lbuf[cp]) break;
										matched = (k==rn);
									}
								}
								if (matched) {
									bool ln_ok = true;
									for (int i_r=0; ln_ok && i_r<RemBgnList->Count; i_r++) {
										int rn = RemBgnList->Strings[i_r].Length();
										for (int k=1; k<=rn; k++) {
											int cp = j + k - 1;  if (cp>lbuf_len) break;
											if (RemBgnList->Strings[i_r][k]!=lbuf[cp]) break;
											if (k==rn) ln_ok = false;
										}
									}
									for (int i_r=0; ln_ok && i_r<RemEndList->Count; i_r++) {
										int rn = RemEndList->Strings[i_r].Length();
										for (int k=1; k<=rn; k++) {
											int cp = j + k - 1;  if (cp>lbuf_len) break;
											if (RemEndList->Strings[i_r][k]!=lbuf[cp]) break;
											if (k==rn) ln_ok = false;
										}
									}
									if (ln_ok) {
										RemPos0 = j;
										break;
									}
									else {
										matched = false;
									}
								}
							}
						}

						//複数行コメント
						if (qch=='\0' || q_in_rem) {
							for (int i_r=0; !matched && i_r<RemBgnList->Count; i_r++) {
								if (isNrm) {
									int rn = RemBgnList->Strings[i_r].Length();
									for (int k=1; k<=rn; k++) {
										int cp = j + k - 1;  if (cp>lbuf_len) break;
										if (RemBgnList->Strings[i_r][k]!=lbuf[cp]) break;
										matched = (k==rn);
									}
									if (matched) {
										RemPos0 = j;  j += (rn - 1);
										isNrm = false;
									}
								}
								else {
									int rn = RemEndList->Strings[i_r].Length();
									for (int k=1; k<=rn; k++) {
										int cp = j + k - 1;  if (cp>lbuf_len) break;
										if (RemEndList->Strings[i_r][k]!=lbuf[cp]) break;
										matched = (k==rn);
									}
									if (matched) {
										RemPos1 = j + rn -1;  j += rn;
										isNrm	= true;
									}
								}
							}
						}
						j++;
					}

					if (RemPos0>0 && RemPos1==0) {
						RemPos1 = lbuf_len;
					}
					else if (RemPos1>0 && RemPos0==0) {
						RemPos0 = 1;
					}
					else if (!isNrm && RemPos0==0 && RemPos1==0) {
						RemPos0 = 1;  RemPos1 = lbuf_len;
					}
				}
			}

			//青空文庫のブロック処理
			if (isAozora) {
				if (StartsStr("［＃", lbuf)) {
					//字下げ
					if (is_match_regex(lbuf, _T("［＃ここから[１-９]+字下げ］"))) {
						UnicodeString nbuf = to_HalfWidth(
							replace_regex(lbuf, _T("［＃ここから([１-９]+)字下げ］"), _T("\\1")));
						ind_n = nbuf.ToIntDef(0);
						lbuf  = EmptyStr;
					}
					else if (is_match_regex(lbuf, _T("［＃ここから[１-９]+字下げ、折り返して[１-９]+字下げ］"))) {
						UnicodeString nbuf = to_HalfWidth(
							replace_regex(lbuf, _T("［＃ここから([１-９]+)字下げ、折り返して([１-９]+)字下げ］"), _T("\\1,\\2")));
						ind_n  = split_tkn(nbuf, ',').ToIntDef(0);
						ind_n2 = nbuf.ToIntDef(0);
						lbuf   = EmptyStr;
					}
					else if (USAME_TS(lbuf, "［＃ここで字下げ終わり］")) {
						ind_n = ind_n2 = -1;
						lbuf  = EmptyStr;
					}
					//地付き
					else if (USAME_TS(lbuf, "［＃ここから地付き］")) {
						alr_n = 0;
						lbuf  = EmptyStr;
					}
					else if (USAME_TS(lbuf, "［＃ここで地付き終わり］")) {
						alr_n = -1;
						lbuf  = EmptyStr;
					}
					//字上げ
					else if (is_match_regex(lbuf, _T("［＃ここから地から[１-９]+字上げ］"))) {
						UnicodeString nbuf = to_HalfWidth(
							replace_regex(lbuf, _T("［＃ここから地から([１-９]+)字上げ］"), _T("\\1")));
						alr_n = nbuf.ToIntDef(0);
						lbuf  = EmptyStr;
					}
					else if (USAME_TS(lbuf, "［＃ここで字上げ終わり］")) {
						alr_n = -1;
						lbuf  = EmptyStr;
					}
				}

				if (ind_n>0) lbuf.Insert(StringOfChar(_T('　'), ind_n), 1);
				if (alr_n>=0) {
					int n = MaxHchX/2 - lbuf_len - alr_n;
					if (n>0) lbuf = StringOfChar(_T('　'), n) + lbuf;
				}
			}

			//--------------------------
			//表示バッファを設定
			//--------------------------
			if (lbuf.IsEmpty()) {
				AddDispLine("\n", org_lno);
			}
			else if ((i==MaxLine-1) && USAME_TS(lbuf, TXLIMIT_MARK)) {
				isLimited = isContinue = true;
				AddDispLine(EmptyStr, org_lno);
			}
			else {
				int idx = 0, ofs = 0;
				UnicodeString tmp_buf = lbuf;
				while (!tmp_buf.IsEmpty()) {
					int n = 1;
					int w = 0;
					for (int j=1; j<tmp_buf.Length(); j++,n++) {
						w = add_CharWidth(tmp_buf[j], w);	if (w>max_wd) break;
					}

					UnicodeString sbuf = tmp_buf.SubString(1, n);	tmp_buf.Delete(1, n);
					if (ind_n2>0 && !tmp_buf.IsEmpty()) tmp_buf.Insert(StringOfChar(_T('　'), ind_n2), 1);
					if (tmp_buf.IsEmpty()) sbuf += "\n";
					line_rec *lp = AddDispLine(sbuf, org_lno, idx++);

					//先頭が文字列かを設定
					if (ofs<lbuf_len) lp->topQch = qch_ln[1 + ofs];
					//コメント範囲設定
					if (RemPos0>0) {
						lp->RemPos0 = (RemPos0 <= n+ofs)? std::max(RemPos0-ofs, 1) : 0;
						lp->RemPos1 = (RemPos1 <= n+ofs)? std::max(RemPos1-ofs, 0) : (lp->RemPos0>0)? n : 0;
					}
					ofs += n;
				}
			}
		}	//end of for

		AddDispLine(EmptyStr, -1);
		MaxDispLine = DispLines->Count;
		BinarySize = 0;
	}

	SetScrBar();

	ToLine(lno);
	if (cno>1) for (int i=1; i<cno; i++) CursorRight(false, true);

	ScrPanel->UpdateKnob();

	isReady = true;
	Repaint(true);

	IniSelected();
	SetSttInf();

	cursor_Default();
}

//---------------------------------------------------------------------------
//テキスト内容を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::AssignText(
	TStrings *lst,
	int lno,			//行番号 (default = 1)  0:履歴を参照
	int sort_mode)		//ソートモード 0:解除/ 1:昇順/ -1:降順
{
	isText = isBinary = isXDoc2Txt = false;
	isCSV  = isTSV    = false;
	if (!ViewBox) return;

	isText = true;
	isLog  = !isClip && (FileName.IsEmpty() || str_match(ExePath + "tasklog*.txt", FileName));
	isXDoc2Txt = UseXd2tx && xd2tx_TestExt(get_extension(FileName));

	if (lst) {
		SortMode = 0;
		TxtBufList->Assign(lst);
		for (int i=0; i<TxtBufList->Count; i++) TxtBufList->Objects[i] = (TObject*)(NativeInt)i;
	}
	else {
		//ソート
		cursor_HourGlass();
		SortMode = sort_mode;
		if (SortMode==0) {
			TxtBufList->CustomSort(comp_ObjectsOrder);
		}
		else {
			//CSV/TSV指定列
			if (CsvCol>=0) {
				USR_CsvCol		= CsvCol;
				USR_CsvSortMode = SortMode;
				bool is_tsv = (TxtBufList->Count>0 && ContainsStr(TxtBufList->Strings[0], "\t"));
				TxtBufList->CustomSort(is_tsv? comp_TsvNaturalOrder : comp_CsvNaturalOrder);
			}
			//通常
			else {
				TxtBufList->CustomSort((SortMode==1)? comp_AscendOrder : comp_DescendOrder);
			}
		}
		cursor_Default();
	}

	//履歴から行位置を復元
	if (lno==0) {
		TStringDynArray itm_buf = record_of_csv_list(TextViewHistory, FileName, 0, 3);
		if (itm_buf.Length==3) {
			if (RestoreViewLine) lno = itm_buf[1].ToIntDef(0);
			MarkListStr = itm_buf[2];
		}
		if (lno==0) lno = 1;
	}

	UpdateScr(lno);
	FuncListDlg->ClearList();
}

//---------------------------------------------------------------------------
//バイナリファイルを読み込んで内容を設定
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::AssignBin(
	__int64 top_adr,		//先頭アドレス	(default = 0)
	bool reload,			//再読み込み	(default = false: 履歴からマークを復元)
	unsigned int adr)		//移動アドレス	(default = 0)
{
	isText = isBinary = isXDoc2Txt = false;
	if (!ViewBox) return false;

	try {
		cursor_HourGlass();
		isBinary = true;

		TxtBufList->Clear();
		ClearDispLine();

		//ファイルを読み込む
		TopAddress	= top_adr;
		MMF->MapEnabled = BinMemMaped;
		if (!MMF->OpenRO(FileName, TopAddress, ViewBinLimitSize)) Abort();
		BinFileSize = MMF->FileSize;
		BinarySize	= MMF->BuffSize;
		isLimited	= BinarySize < BinFileSize;
		isContinue	= (TopAddress + BinarySize) < BinFileSize;
		MaxDispLine = BinarySize/16 + 1;
		if (BinarySize%16) MaxDispLine++;
		MaxLine = MaxDispLine - 1;

		//履歴からマークを復元
		if (!reload) {
			TStringDynArray itm_buf = record_of_csv_list(TextViewHistory, FileName, 0, 3);
			if (itm_buf.Length==3) MarkListStr = itm_buf[2];
		}

		UpdateScr();
		FuncListDlg->ClearList();

		//ビットマップビューの更新
		if (BitmapForm->Visible) {
			BitmapForm->BitmapBox->ClientHeight = BinarySize/BitmapForm->MapWidth + 1;
			BitmapForm->BitmapBox->Invalidate();
		}
		cursor_Default();
		return ToAddrR(adr);
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		cursor_Default();
		return false;
	}
	catch (...) {
		cursor_Default();
		return false;
	}
}

//---------------------------------------------------------------------------
//末尾が改行か？
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::has_CR(UnicodeString s)
{
	return ends_tchs(_T("\n\r"), s);
}

//---------------------------------------------------------------------------
//タブを考慮して1文字分の表示幅を追加
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::add_CharWidth(WideChar c, int w)
{
	if (c=='\t') {
		w += HchWidth;
		int dt = w % TabWidth;
		if (dt>0) w += (TabWidth - dt);
	}
	else {
		w += get_TextWidth(ViewCanvas, c, IsIrregFont);
	}
	return w;
}
//---------------------------------------------------------------------------
//タブを考慮して文字列の表示幅を取得
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::get_StrWidth(UnicodeString s)
{
	int w = 0;
	int i = 1;
	while (i<=s.Length()) {
		WideChar c = s[i];
		if (c=='\t') {
			w += HchWidth;
			int dt = w % TabWidth;
			if (dt>0) w += (TabWidth - dt);
			i++;
		}
		else {
			if (s.IsLeadSurrogate(i)) {
				w += get_TextWidth(ViewCanvas, s.SubString(i, 2), IsIrregFont);
				i += 2;
			}
			else {
				w += get_TextWidth(ViewCanvas, c, IsIrregFont);
				i++;
			}
		}
	}
	return w;
}

//---------------------------------------------------------------------------
//カーソル行の末尾位置を取得
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::get_CurEndPos()
{
	UnicodeString lbuf = get_CurLine();
	int p = lbuf.Length(); if (p>0 && has_CR(lbuf)) p--;
	if (isBinary && LimitBinCsr) p = std::min(p, 46);
	return p;
}

//---------------------------------------------------------------------------
//桁位置から半角換算位置への変換(タブを考慮)
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::cv_PosX_to_HchX(int px)
{
	UnicodeString lbuf = get_CurLine();
	if (isBoxMode) lbuf += StringOfChar(_T(' '), MaxHchX);
	lbuf = lbuf.SubString(1, px);

	int w = 0;
	while (!lbuf.IsEmpty()) {
		int p_t = lbuf.Pos('\t');
		if (p_t>0) {
			if (p_t>1) w += get_TextWidth(ViewCanvas, lbuf.SubString(1, p_t - 1), IsIrregFont);
			w = add_CharWidth('\t', w);
			lbuf.Delete(1, p_t);
		}
		else {
			w += get_TextWidth(ViewCanvas, lbuf, IsIrregFont);
			lbuf = EmptyStr;
		}
	}

	int x = w/HchWidth;
	if (isBoxMode) x = std::min(x, MaxHchX);
	return x;
}

//---------------------------------------------------------------------------
//CSV列位置を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::set_CurCsvCol()
{
	CsvCol = -1;
	if (isCSV && isFixedLen && FixWdList.Length>0) {
		line_rec *lp = get_LineRec(CurPos.y);
		int cp = lp->LineIdx*MaxHchX + cv_PosX_to_HchX(CurPos.x);
		CsvCol = 0;
		for (int i=0,wd=0; i<FixWdList.Length-1; i++,CsvCol++) {
			wd += (FixWdList[i] + 2); if (wd>cp) break;
		}
	}
}
//---------------------------------------------------------------------------
//CSV列位置から桁を設定 (レコードの1行目でのみ有効)
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::set_PosFromCol(int col)
{
	if (get_LineRec(CurPos.y)->LineIdx!=0) return false;
	if (!isFixedLen || FixWdList.Length==0 || col<0) return false;

	int xw = 0;
	for (int i=0; i<FixWdList.Length && i<col; i++) xw += (FixWdList[i] + 2);

	bool res = false;
	UnicodeString lbuf = get_CurLine();
	for (int i=1; i<=lbuf.Length() && !res; i++) {
		if (xw < str_len_half(lbuf.SubString(1, i))) {
			CurHchX  = xw;
			CurPos.x = i - 1;
			if (lbuf.IsTrailSurrogate(i)) CurPos.x--;
			CsvCol = col;
			res = true;
		}
	}

	if (res) {
		Repaint();
		SetSttInf();
	}

	return res;
}

//---------------------------------------------------------------------------
//選択末尾を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::set_SelEnd()
{
	SelEnd = BoxEnd = CurPos;
	BoxEnd.x = CurHchX;
}

//---------------------------------------------------------------------------
//桁位置が現在行の文字数より大きい場合の位置調整
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::adjust_PosX()
{
	int p_end = cv_PosX_to_HchX(get_CurEndPos());
	if (CurHchX==0 || p_end==0) {
		CurPos.x = 0; return;
	}

	int wp = ((CurHchX>p_end)? p_end : CurHchX) * HchWidth;
	int w = 0;
	UnicodeString lbuf = get_CurLine();
	for (int i=1; i<=lbuf.Length(); i++) {
		w = add_CharWidth(lbuf[i], w);
		if (w>wp) {
			CurPos.x = i - 1;	break;
		}
	}
}

//---------------------------------------------------------------------------
//カーソル位置が下位サロゲートだったら左へ
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::to_Lead_if_Trail(bool sel)
{
	if (CurPos.x>0 && get_DispLine(CurPos.y).IsTrailSurrogate(CurPos.x + 1)) CursorLeft(sel);
}

//---------------------------------------------------------------------------
//タブ、末尾の改行を含む文字列の表示
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::TabTextOut(
	UnicodeString s,	//表示文字列
	TCanvas *cv,
	int &x,				//[io] X開始位置 (更新)
	int max_x)			//最大幅
{
	TColor fgcol = cv->Font->Color;
	int y = 0;
	int f_h = cv->TextHeight("Q");

	while (!s.IsEmpty()) {
		//制御文字の位置を取得
		int ctrl_p = 0;
		for (int i=1; i<=s.Length() && ctrl_p==0; i++)
			if (iscntrl(s[i]) && s[i]!=_T('\r') && s[i]!=_T('\n')) ctrl_p = i;

		if (ctrl_p>0) {
			char ctrl_ch = s[ctrl_p];
			if (ctrl_p>1) {
				//タブ文字前の文字列(通常表示)
				UnicodeString sbuf = s.SubString(1, ctrl_p - 1);
				TRect rc = Rect(x, y, x + get_TextWidth(cv, sbuf, IsIrregFont), f_h);
				//罫線
				int hr_w = is_RuledLine(sbuf);
				if (hr_w>0) {
					cv->FillRect(rc);
					draw_Line(cv, rc.Left, rc.Top + rc.Height()/2, rc.Right, rc.Top + rc.Height()/2, hr_w, color_HR);
				}
				//文字
				else {
					cv->TextRect(rc, x, y, alt_BackSlash? ReplaceStr(s, "\\", _T("\u2216")) : s);
				}
				x += rc.Width();
			}
			//タブ文字
			if (ctrl_ch==_T('\t')) {
				if (ShowTAB) draw_TAB(cv, x, y, HchWidth, f_h); else cv->TextOut(x, y, " ");
				x += HchWidth;
				int dt = (x - TopXpos) % TabWidth;
				if (dt>0) {
					int x1 = x + TabWidth - dt;
					while (x<x1) {
						cv->TextOut(x, y, " ");
						x += HchWidth;
						if (x>=max_x) break;
					}
				}
				s.Delete(1, ctrl_p);
			}
			//その他の制御文字
			else {
				cv->Font->Color = col_Ctrl;
				UnicodeString ctrl_str;  ctrl_str.sprintf(_T("%c"), ctrl_ch + 0x40);
				TRect rc = Rect(x, y, x + get_TextWidth(cv, ctrl_str, IsIrregFont), f_h);
				cv->TextRect(rc, x, y, ctrl_str);	x += get_TextWidth(cv, ctrl_str, IsIrregFont);
				s.Delete(1, ctrl_p);
				cv->Font->Color = fgcol;
			}
		}
		else {
			//通常表示
			bool has_cr = has_CR(s);
			if (has_cr) delete_end(s);
			TRect rc = Rect(x, y, x + get_TextWidth(cv, s, IsIrregFont), f_h);
			//罫線
			int hr_w = is_RuledLine(s);
			if (hr_w>0) {
				cv->FillRect(rc);
				draw_Line(cv, rc.Left, rc.Top + rc.Height()/2, rc.Right, rc.Top + rc.Height()/2, hr_w, color_HR);
			}
			//文字
			else {
				cv->TextRect(rc, x, y, alt_BackSlash? ReplaceStr(s, "\\", _T("\u2216")) : s);
			}
			x += rc.Width();

			//改行
			if (has_cr) {
				if (ShowCR) draw_CR(cv, x, y, HchWidth, f_h); else cv->TextOut(x, y, " ");
			}
			s = EmptyStr;
		}
	}
}

//---------------------------------------------------------------------------
//固定長表示の列アルファブレンド
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::AlphaBlendCsvCol(TCanvas *cv, int max_x, int y, int h)
{
	if (CsvCol<0 || CsvCol>=FixWdList.Length) return;

	int x = TopXpos;
	for (int j=0; j<CsvCol; j++) x += (FixWdList[j] + 2) * HchWidth;
	int w = (FixWdList[CsvCol] + 2) * HchWidth;
	if (CsvCol==0) x -= TopMargin; else x -= HchWidth;
	x++; w--;

	if (x<max_x) alpha_blend_Rect(cv, x, y, w, h, color_Cursor, CellAlpha);
}

//---------------------------------------------------------------------------
//描画
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::PaintText()
{
	if (!isReady || !ViewBox->Parent->Showing) return;

	std::unique_ptr<Graphics::TBitmap> tmp_bmp(new Graphics::TBitmap());
	tmp_bmp->SetSize(ViewBox->ClientWidth - (ScrBar->Visible? 1 : 0), LineHeight);
	TRect    tmp_rc	= Rect(0, 0, tmp_bmp->Width, tmp_bmp->Height);
	TCanvas *tmp_cv = tmp_bmp->Canvas;
	TRect    img_rc = Rect(0, 0, tmp_bmp->Width, ceil(1.0 * ViewBox->ClientHeight / LineHeight) * LineHeight);
	tmp_cv->Font->Assign(ViewBox->Font);

	int fld_xp	= isFitWin ? tmp_bmp->Width
						   : (TopXpos + (isBinary? MaxHchX : ViewFoldWidth + 1) * HchWidth + 4);
	int top_idx = ScrBar->Position - 1;
	int d_idx	= top_idx - LastTop;
	int btm_idx	= top_idx + LineCount + 1;
	int adr_len = get_AddrStr(BinFileSize - 1).Length();

	WideChar strQch = '\0';

	//強調表示情報
	bool mt_Case[MAX_EM_PTN] = {ReservedCase, false, false, false, false, false, UsrKeywdCase2};
	mt_Case[2] = isIncSea? IncSeaCaseSens : isCase;
	mt_Case[5] = (!isBinary && !isAozora && !isLog)? UsrKeywdCase : false;
	int  mt_Len[MAX_EM_PTN]  = {0, 0, 0, 0, 0, 0, 0};
	int  mt_Idx[MAX_EM_PTN]  = {0, 0, 0, 0, 0, 0, 0};

	EmPtn[2] = !isBytes && Highlight? RegExPtn : EmptyStr;	//検索強調語

	int csr_yp = -1;
	int csr_xp = 0;
	int csr_bp = 0;
	int csr_lw = std::max(CursorWidth, 1);
	int csr_yl = FontHeight + csr_lw + 1;

	//バッファされている部分を描画
	std::unique_ptr<Graphics::TBitmap> buf_bmp(new Graphics::TBitmap());
	buf_bmp->SetSize(img_rc.Width(), img_rc.Height());
	TCanvas *buf_cv = buf_bmp->Canvas;

	int  buf_idx0, buf_idx1;
	bool buffered = (LastTop!=-1 && !ImgBuff->Empty && (abs(d_idx)<3));
	if (buffered) {
		TRect v_rc = img_rc;  OffsetRect(v_rc, 0, -(d_idx * LineHeight));
		buf_cv->CopyRect( v_rc, ImgBuff->Canvas, img_rc);
		ViewCanvas->CopyRect(v_rc, ImgBuff->Canvas, img_rc);
		buf_idx0 = top_idx - d_idx;
		buf_idx1 = btm_idx - d_idx - 1;
	}

	//行内容の描画
	int v_yp = 0;
	for (int i=top_idx; i<=btm_idx; i++, v_yp+=LineHeight) {
		//余白
		tmp_cv->Brush->Color = color_Margin;
		TRect bg_rc = tmp_rc;
		int rc_xp = 0;
		bg_rc.Right = rc_xp = LeftMargin;
		tmp_cv->FillRect(bg_rc);

		//行番号背景
		if ((ShowLineNo || isBinary)) {
			tmp_cv->Brush->Color = color_bgLineNo;
			bg_rc = tmp_rc;
			bg_rc.Left	= rc_xp;
			bg_rc.Right = rc_xp = TopXpos - TopMargin;
			tmp_cv->FillRect(bg_rc);
		}

		//背景
		tmp_cv->Brush->Color = color_bgView;
		bg_rc = tmp_rc;
		bg_rc.Left = rc_xp;
		if (isFitWin) {
			tmp_cv->FillRect(bg_rc);
		}
		else {
			rc_xp = (MarginBox && MarginBox->Width>0)? tmp_rc.Right : fld_xp + 4;
			bg_rc.Right = rc_xp;
			tmp_cv->FillRect(bg_rc);
			//右余白
			if (rc_xp<tmp_rc.Right) {
				tmp_cv->Brush->Color = color_Margin;
				bg_rc = tmp_rc;
				bg_rc.Left = rc_xp;
				tmp_cv->FillRect(bg_rc);
				tmp_cv->Brush->Color = color_bgView;
			}
		}

		if (i<MaxDispLine) {
			line_rec *lp = get_LineRec(i);
			UnicodeString lbuf = get_DispLine(i);
			int lbuf_len = lbuf.Length();

			//カーソル位置取得
			if (CurPos.y==i) {
				csr_yp = v_yp;
				csr_xp = TopXpos - 1 +
							(isBoxMode? CurHchX * HchWidth : get_StrWidth(lbuf.SubString(1, CurPos.x )));
				//対応アドレスカーソル(バイナリ)
				if (isBinary) {
					int ofs = get_OfsR(CurPos.x);
					csr_bp = (CurPos.x<50)? (ofs + 50) : (ofs * 3 + ((ofs>7)? 1 : 0));
					csr_bp *= HchWidth;
					csr_bp += (TopXpos - 1);
				}
			}

			//不要な描画はスキップ
			if (buffered && i>buf_idx0 && i<buf_idx1) {
				//行カーソルを一旦描画しておく
				if (CurPos.y==i && ShowLineCursor)
					draw_Line(ViewCanvas, LeftMargin + 1, v_yp + csr_yl, fld_xp - 2, v_yp + csr_yl, csr_lw, color_Cursor);

				//選択なし
				if (SelStart==SelEnd) {
					if (!LastSel) continue;
					if (LastPos.y==CurPos.y && CurPos.y!=i) continue;
				}
				//選択中
				else {
					if (LastPos.y==CurPos.y && CurPos.y!=i) {
						int y0 = std::min(SelStart.y, SelEnd.y);
						int y1 = std::max(SelStart.y, SelEnd.y);
						if (i<y0 || i>y1) continue;
					}
				}
			}

			int xp = TopXpos;
			if (i==MaxDispLine-1) {
				//ファイルの終わり
				out_Text(tmp_cv, xp, 0, isContinue? _T("……… 続く ………") : _T("[EOF]"), color_LineNo);
			}
			else {
				if (lp->LineIdx==0) {
					int x0 = LeftMargin + MARK_WIDTH;
					//行番号/アドレス
					if ((ShowLineNo || isBinary)) {
						UnicodeString ln_str;
						//アドレス
						if (isBinary) {
							if (lp->LineNo>0) ln_str = get_AddrStr(TopAddress + i*16, adr_len);
						}
						//行番号
						else {
							if (lp->LineNo>0) ln_str.sprintf(_T("%6u"), lp->LineNo);
						}
						tmp_cv->Brush->Color = color_bgLineNo;
						tmp_cv->Font->Color  = color_LineNo;
						tmp_cv->TextOut(x0, 0, ln_str);
					}
					//マーク
					if (IsMarked(lp->LineNo)) {
						tmp_cv->Brush->Color = color_Mark;
						tmp_cv->FillRect(Rect(LeftMargin, tmp_rc.Top, x0 - 2, tmp_rc.Top + FontHeight + 1));
					}
					tmp_cv->Brush->Color = color_bgView;
				}

				//内容
				int q0 = -1, q1 = -1;
				//選択中
				if (SelStart!=SelEnd) {
					//箱形選択
					if (isBoxMode) {
						TPoint sp0 = BoxStart;
						TPoint sp1 = BoxEnd;
						bool flag = (sp0.y==sp1.y)? (sp0.x>sp1.x) : (sp0.y>sp1.y);
						if (flag) std::swap(sp0, sp1);
						if (sp0.x>sp1.x) std::swap(sp0.x, sp1.x);
						if (lp->LineIdx==0 && sp0.y<=i && i<=sp1.y) {
							int wp0 = sp0.x * HchWidth;
							int wp1 = sp1.x * HchWidth;
							int w = 0;
							for (int j=1; j<=lbuf_len; j++) {
								w = add_CharWidth(lbuf[j], w);
								if (w>wp1) break;
								if (w>wp0) {
									if (q0==-1) q0 = j;
									q1 = j;
								}
							}
						}
					}
					//通常選択
					else {
						TPoint sp0 = SelStart;
						TPoint sp1 = SelEnd;
						bool flag = (sp0.y==sp1.y)? (sp0.x>sp1.x) : (sp0.y>sp1.y);
						if (flag) std::swap(sp0, sp1);
						if (sp0.y==i) {
							q0 = sp0.x + 1;
							q1 = (sp1.y==i)? sp1.x : lbuf_len;
						}
						else if (sp1.y==i) {
							q0 = 1;
							q1 = sp1.x;
						}
						else if (sp0.y<i && i<sp1.y) {
							q0 = 1;
							q1 = lbuf_len;
						}
					}
				}

				//属性バッファ
				TColor curFgCol[4096], curBgCol[4096];

				//選択色
				for (int j=1; j<=lbuf_len; j++) {
					curFgCol[j] = color_fgView;
					curBgCol[j] = (q0<=j && j<=q1)? color_selItem : color_bgView;
				}

				UnicodeString lbuf2 = lbuf;
				if (isText) {
					//現在行から改行行末までの内容を取得
					for (int i2=i+1; i2<MaxDispLine; i2++) {
						if (has_CR(lbuf2)) break;
						if (get_LineRec(i2)->LineIdx==0) break;
						lbuf2.cat_sprintf(_T("%s"), get_DispLine(i2).c_str());
						if (lbuf2.Length()>4096) break;		//***
					}

					//見出し
					if (!HeadlinePtn.IsEmpty()) {
						if (lp->LineIdx==0 || HeadlinePtn[1]!='^') {
							TMatch mt = TRegEx::Match(lbuf, HeadlinePtn);
							if (mt.Success) {
								int mlen = isLog? mt.Length : lbuf_len;
								for (int j=1; j<=mlen; j++) curFgCol[j] = color_Headline;
								if (isLog) {
									int p = lbuf.Pos("--->");
									if (p>0) {
										for (int j=0; j<4; j++) curFgCol[p++] = color_Symbol;
									}
								}
							}
						}
					}

					//コメント
					if (lp->RemPos0>0) for (int j=lp->RemPos0; j<=lp->RemPos1; j++) curFgCol[j] = color_Comment;

					//文字列
					if (!QuotStr.IsEmpty()) {
						if (!lbuf.IsEmpty()) {
							bool isEsc = false;
							WideChar c1 = lbuf[1];
							bool isQ = ContainsText(QuotStr, c1);
							if (useEsc && c1=='\\') isEsc = true;
							strQch = lp->topQch;
							if (curFgCol[1]!=color_Comment && (strQch!='\0' || isQ)) curFgCol[1] = color_Strings;
							for (int j=2; j<=lbuf_len; j++) {
								WideChar cj = lbuf[j];
								bool isQ = ContainsText(QuotStr, cj);
								if		(isEsc)				 isEsc = false;
								else if (useEsc && cj=='\\') isEsc = true;
								else if (isQ)				 strQch = (strQch==cj)? '\0' : cj;
								TColor *fg = &curFgCol[j];
								if (*fg==color_Comment) continue;
								if (strQch!='\0' || isQ) *fg = color_Strings;
							}
							if (lp->hasCR) strQch = '\0';
						}
						else {
							strQch = '\0';
						}
					}

					//シンボル
					if (!SymbolChs.IsEmpty()) {
						for (int j=1; j<=lbuf_len; j++) {
							TColor *fg = &curFgCol[j];
							if (*fg==color_Comment || *fg==color_Strings) continue;
							if (lbuf.IsDelimiter(SymbolChs, j)) *fg = color_Symbol;
						}
					}
				}

				//予約語、URL、ローカルファイル、マッチ語の強調表示
				for (int i_p=0; i_p<MAX_EM_PTN; i_p++) {
					if (EmPtn[i_p].IsEmpty()) continue;

					//前行からの続きを処理
					if (mt_Idx[i_p]>0 && mt_Len[i_p]>0) {
						if (mt_Idx[i_p]<=lbuf_len) {
							for (int i_x=mt_Idx[i_p]; mt_Len[i_p]>0 && i_x<=lbuf_len; i_x++,mt_Len[i_p]--) {
								TColor *fg = &curFgCol[i_x];
								if (*fg!=color_Comment && *fg!=color_Strings) *fg = EmFgC[i_p];
								TColor *bg = &curBgCol[i_x];
								if (EmBgC[i_p]!=col_None && *bg!=color_selItem) *bg = EmBgC[i_p];
							}
							mt_Idx[i_p] = (mt_Len[i_p]>0)? 1 : 0;
						}
						else {
							mt_Idx[i_p] -= lbuf_len;
						}

						if (lp->hasCR) mt_Len[i_p] = 0;
					}

					//正規表現で強調語を検索
					if (!lbuf2.IsEmpty() && (lp->LineIdx==0 || EmPtn[i_p][1]!='^')) {
						if (i_p==1 && lbuf2.Pos(':')==0) continue;	//URL 処理時間短縮のための前チェック

						TRegExOptions opt;
						if (!mt_Case[i_p]) opt << roIgnoreCase;
						TMatchCollection mts = TRegEx::Matches(lbuf2, EmPtn[i_p], opt);
						mt_Len[i_p] = 0;
						for (int i_m=0; i_m<mts.Count; i_m++) {
							mt_Len[i_p] = mts.Item[i_m].Length;
							mt_Idx[i_p] = mts.Item[i_m].Index;
							if (mt_Idx[i_p]<=lbuf_len) {
								for (int i_x=mt_Idx[i_p]; mt_Len[i_p]>0 && i_x<=lbuf_len; i_x++,mt_Len[i_p]--) {
									//文字
									TColor *fg = &curFgCol[i_x];
									if (EmFgC[i_p]==color_URL)		//URL優先
										*fg = EmFgC[i_p];
									else if (*fg!=color_Comment && *fg!=color_Strings && *fg!=color_URL && (isText || i_x<50))
										*fg = EmFgC[i_p];
									//背景
									TColor *bg = &curBgCol[i_x];
									if (EmBgC[i_p]!=col_None && *bg!=color_selItem) *bg = EmBgC[i_p];
								}
								if (mt_Len[i_p]>0) {
									mt_Idx[i_p] = 1; break;	//次行に続く
								}
								else {
									mt_Idx[i_p] = 0;
								}
							}
							else {
								mt_Idx[i_p] -= lbuf_len;  break;
							}
						}
					}
				}

				//INIファイルのキー名と数値を強調表示
				if (isIniFmt && lp->LineIdx==0) {
					int p = lbuf.Pos('=');
					if (p>1 && curFgCol[1]!=color_Comment && curFgCol[1]!=color_Headline) {
						//キー
						for (int i_x=1; i_x<p; i_x++) curFgCol[i_x] = color_Reserved;
						// =
						curFgCol[p++] = color_Symbol;
						//数値
						UnicodeString sbuf = TrimRight(lbuf);
						if ((sbuf.Length() - p + 1) <= 20) {
							bool flag = true;
							for (int i_x=p; i_x<=sbuf.Length() && flag; i_x++)
								flag = iswdigit(sbuf[i_x]) || (i_x==p && sbuf[i_x]=='-');
							if (flag) for (int i_x=p; i_x<=sbuf.Length(); i_x++) curFgCol[i_x] = color_Numeric;
						}
					}
				}

				//バイナリ強調をASCII部分に反映
				if (isBinary) {
					int i_x = 1;
					for (int i_h=0; i_h<16; i_h++) {
						if (curFgCol[i_x]==curFgCol[i_x + 1]) curFgCol[51 + i_h] = curFgCol[i_x];
						i_x += 3; if (i_h==7) i_x++;
					}
				}

				//文字列描画
				if (color_fgSelItem!=col_None) {
					for (int j=1; j<=lbuf_len; j++)
						if (curBgCol[j]==color_selItem) curFgCol[j] = color_fgSelItem;
				}
				tmp_cv->Font->Color  = curFgCol[1];
				tmp_cv->Brush->Color = curBgCol[1];
				UnicodeString sbuf;
				for (int j=1; j<=lbuf_len; j++) {
					TColor *fg = &curFgCol[j];
					TColor *bg = &curBgCol[j];
					if (*fg==tmp_cv->Font->Color && *bg==tmp_cv->Brush->Color) {
						sbuf.cat_sprintf(_T("%c"), lbuf[j]);
					}
					else {
						TabTextOut(sbuf, tmp_cv, xp, fld_xp);
						sbuf = lbuf[j];
						tmp_cv->Font->Color  = *fg;
						tmp_cv->Brush->Color = *bg;
					}
				}
				if (!sbuf.IsEmpty()) TabTextOut(sbuf, tmp_cv, xp, fld_xp);

				//固定長表示の縦罫線
				if (isFixedLen && lp->LineIdx==0 && FixWdList.Length>0 && !lbuf.IsEmpty() && color_bdrFixed!=col_None) {
					int v_xp = TopXpos;
					for (int j=0; j<FixWdList.Length-1; j++) {
						v_xp += (FixWdList[j] + 1) * HchWidth;
						if (v_xp>fld_xp) break;
						draw_Line(tmp_cv, v_xp, 0, v_xp, LineHeight, 1, color_bdrFixed);
						v_xp += HchWidth;
					}
				}
			}
		}

		//行番号境界線
		if ((ShowLineNo || isBinary) && color_bdrLine!=col_None)
			draw_Line(tmp_cv, TopXpos - TopMargin, 0, TopXpos - TopMargin, LineHeight, 1, color_bdrLine);
		//折り返し境界線
		if (!isFitWin && color_bdrFold!=col_None)
			draw_Line(tmp_cv, fld_xp, 0, fld_xp, LineHeight, 1, color_bdrFold);

		//バッファに描画
		TRect v_rc = tmp_rc;  OffsetRect(v_rc, 0, v_yp);
		buf_cv->CopyRect(v_rc, tmp_bmp->Canvas, tmp_rc);
		//行カーソルを一旦描画しておく
		if (CurPos.y==i && ShowLineCursor)
			draw_Line(tmp_cv, LeftMargin + 1, csr_yl, fld_xp - 2, csr_yl, csr_lw, color_Cursor);
		ViewCanvas->CopyRect(v_rc, tmp_cv, tmp_rc);
	}

	//現在の画面をバッファにコピー
	ImgBuff->SetSize(img_rc.Width(), img_rc.Height());
	ImgBuff->Canvas->CopyRect(img_rc, buf_cv, img_rc);

	if (csr_yp>=0) {
		int yp_l = csr_yp + csr_yl;
		//行カーソル
		if (ShowLineCursor) draw_Line(ViewCanvas, LeftMargin + 1, yp_l, fld_xp - 2, yp_l, csr_lw, color_Cursor);
		//桁カーソル
		draw_Line(ViewCanvas, csr_xp, csr_yp, csr_xp, yp_l - 1, 2, color_Cursor);
		//対応アドレスカーソル
		if (isBinary) draw_Line(ViewCanvas, csr_bp, yp_l - LineHeight/2, csr_bp, yp_l - 1, 1, color_Cursor);
		//固定長表示のセル背景アルファ
		if (CellAlpha>0 && get_LineRec(CurPos.y)->LineIdx==0 && !get_DispLine(CurPos.y).IsEmpty())
			AlphaBlendCsvCol(ViewCanvas, fld_xp, csr_yp, LineHeight);
	}

	LastPos = CurPos;
	LastTop = top_idx;
	LastSel = (SelStart!=SelEnd);
}

//---------------------------------------------------------------------------
//ルーラの描画
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onRulerPaint(TObject *Sender)
{
	if (!isReady) return;

	TCanvas *cv = RulerBox->Canvas;
	TRect rc = RulerBox->ClientRect;
	cv->Brush->Color = color_bgRuler;
	cv->FillRect(rc);

	cv->Pen->Color = color_fgRuler;
	cv->Pen->Width = 1;
	cv->Font->Assign(DialogFont);
	cv->Font->Size	= ScaledInt(DialogFont->Size, OwnerForm);
	cv->Font->Color = color_fgRuler;
	int xp = TopXpos - 1;

	//固定長表示
	if (isCSV && isFixedLen && FixWdList.Length>0) {
		cv->Font->Height = RulerBox->ClientHeight - 2;
		UnicodeString al_str;
		for (int i=0; i<FixWdList.Length; i++) {
			if (i==0) {
				xp -= (HchWidth - 1);
				cv->MoveTo(xp, rc.Top + 2);
				cv->LineTo(xp, rc.Bottom - 1);
			}
			al_str = al_str.IsEmpty()? UnicodeString("A") : get_NextAlStr(al_str);
			cv->TextOut(xp + 4, rc.Top, al_str);
			xp += ((FixWdList[i] + 2) * HchWidth);
			cv->MoveTo(xp, rc.Top + 2);
			cv->LineTo(xp, rc.Bottom - 1);
		}

		//固定長表示のセル背景アルファ
		if (CellAlpha>0) AlphaBlendCsvCol(cv, rc.Right, rc.Top + 2, rc.Height() - 2);
	}
	//バイナリ
	else if (isBinary) {
		cv->Font->Assign(ViewBox->Font);
		cv->Font->Color  = color_fgRuler;
		cv->Font->Height = std::min(RulerBox->ClientHeight, abs(cv->Font->Height)) - 2;
		UnicodeString tmp;
		for (int i=0; i<16; i++) {
			tmp.sprintf(_T("+%X"), i);
			cv->TextOut(xp + HchWidth*2 - cv->TextWidth(tmp), rc.Top, tmp);
			xp += HchWidth * ((i==7)? 4 : 3);
		}
		xp += HchWidth;
		for (int i=0; i<16; i++) {
			tmp.sprintf(_T("%X"), i);
			cv->TextOut(xp + HchWidth - cv->TextWidth(tmp), rc.Top, tmp);
			xp += HchWidth;
		}
	}
	//通常表示
	else {
		cv->Font->Height = ScaledInt(10, OwnerForm);
		//目盛
		for (int i=0; i<MaxFoldWd; i++) {
			if (i%10==0) {
				cv->MoveTo(xp, rc.Top + 2);
				cv->LineTo(xp, rc.Bottom - 1);
				cv->TextOut(xp + 2, rc.Top, i);
			}
			else {
				cv->MoveTo(xp, rc.Bottom - 4);
				cv->LineTo(xp, rc.Bottom - 1);
			}
			xp += HchWidth;
		}
	}

	//折り返し位置
	if (!isFitWin && !isBinary) {
		xp = TopXpos + ViewFoldWidth * HchWidth - 1;
		TPoint mkp[3] = {Point(xp, rc.Bottom - 1), Point(xp - 3, rc.Bottom - 6), Point(xp + 3, rc.Bottom - 6)};
		cv->Brush->Style = bsSolid;
		cv->Brush->Color = color_fgRuler;
		cv->Polygon(mkp, 2);
	}

	//カーソル位置
	if (!isCSV || !isFixedLen || CellAlpha==0) {
		xp = TopXpos + cv_PosX_to_HchX(CurPos.x) * HchWidth - 1;
		draw_Line(cv, xp, rc.Top + 1, xp, rc.Bottom, 2, color_fgRuler);
	}
}

//---------------------------------------------------------------------------
//再描画要求
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::Repaint(bool force)
{
	if (inh_Repaint) return;

	if (force) {
		LastPos = Point(-1, -1);
		LastTop = -1;
	}

	PaintText();

	if (RulerBox->Visible) RulerBox->Repaint();
}

//---------------------------------------------------------------------------
//行・桁情報の正規化
//---------------------------------------------------------------------------
TPoint __fastcall TTxtViewer::nrm_Pos(TPoint p)
{
	if (p.x<0) {
		//現在行より前
		for (;;) {
			p.y--;
			if (p.y<0) {
				p = Point(0, 0);  break;
			}
			p.x += get_DispLine(p.y).Length();
			if (p.x>0) break;
		}
	}
	else if (p.y<MaxDispLine) {
		for (;;) {
			int end_p = get_DispLine(p.y).Length();
			if (p.x<=end_p) break;
			p.x -= end_p;
			p.y++;
			if (p.y>=MaxDispLine) {
				p.y = MaxDispLine - 1;
				UnicodeString lbuf = get_DispLine(p.y);
				p.x = lbuf.Length();	if (has_CR(lbuf)) p.x--;
				break;
			}
		}
	}
	return p;
}

//---------------------------------------------------------------------------
//選択状態の初期化
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::IniSelected(bool sel)
{
	if (!sel || SelStart==SelEnd) {
		if (!sel) isSelMode = isBoxMode = false;
		SelStart = BoxStart = CurPos;
		BoxStart.x = cv_PosX_to_HchX(BoxStart.x);
		set_SelEnd();
	}
}

//---------------------------------------------------------------------------
//画面位置から行・桁を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetPosFromPt(int x, int y)
{
	y += (ScrBar->Position - 1) * LineHeight;
	x -= TopXpos;

	if (x<0) x = 0;
	if (y<0) y = 0;

	//行
	int n = y / LineHeight;
	if (n>=MaxDispLine) n = MaxDispLine - 1;
	CurPos.y = n;

	//桁
	int xp = -1;
	UnicodeString lbuf = get_CurLine();
	for (int i=1; i<=lbuf.Length(); i++) {
		int w = get_StrWidth(lbuf.SubString(1, i));
		if (x<=w) {
			xp = i - 1;
			if (lbuf.IsTrailSurrogate(i)) xp--;
			break;
		}
	}

	if (xp==-1) {
		ExtClicked = true;		//行末より後がクリックされた
		xp = lbuf.Length();
	}
	else {
		ExtClicked = false;
	}

	CurPos.x = xp;
	CurHchX  = cv_PosX_to_HchX(CurPos.x);

	set_CurCsvCol();
}
//---------------------------------------------------------------------------
//ポイントが選択範囲内にあるか?
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::PtInSelected(int x, int y)
{
	if (SelStart==SelEnd) return false;

	y += (ScrBar->Position - 1) * LineHeight;
	x -= TopXpos;
	if (y<0 || x<0) return false;

	//行
	int py = y / LineHeight;
	if (py<SelStart.y || py>SelEnd.y) return false;

	//桁
	UnicodeString lbuf = get_DispLine(py);
	int px = -1;
	for (int i=1; i<=lbuf.Length(); i++) {
		int w = get_StrWidth(lbuf.SubString(1, i));
		if (x<=w) {
			px = i - 1;
			if (lbuf.IsTrailSurrogate(i)) px--;
			break;
		}
	}

	if (px==-1) return false;
	if (py==SelStart.y && px<SelStart.x) return false;
	if (py==SelEnd.y   && px>=SelEnd.x)  return false;
	return true;
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::UpdatePos(
	bool up_pos,		// (default = false)
	bool force)			// (default = false)
{
	LineCount = ViewBox->ClientHeight / LineHeight - 1;

	if (up_pos) {
		CurTop = CurPos.y - 5;
		if (CurTop<1) CurTop = 1;
		CurTop = std::min(CurTop, ScrBar->Max - ScrBar->LargeChange + 1);
		ScrBar->Position = CurTop;
	}
	else {
		CurTop = ScrBar->Position;
	}

	CurBottom = CurTop + LineCount;

	bool changed = false;
	if (CurPos.y<CurTop) {
		CurTop	  = CurPos.y;
		CurBottom = CurTop + LineCount;
		changed = true;
	}
	else if (CurPos.y >= CurBottom-1) {
		CurBottom = CurPos.y + 1;
		CurTop = CurBottom - LineCount;
		if (CurTop<1) {
			CurTop	  = 1;
			CurBottom = CurTop + LineCount;
		}
		changed = true;
	}
	if (changed) ScrBar->Position = CurTop;

	set_CurCsvCol();

	Repaint(force);

	//イメージプレビュー
	if (SubViewer->Visible) {
		UnicodeString fnam = GetCurImgFile();
		if (!fnam.IsEmpty() && !SameText(SubViewer->FileName, fnam)) SubViewer->DrawImage(fnam);
	}
}

//---------------------------------------------------------------------------
//カーソルが表示範囲外になっていたら、見える位置へ移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::UpdateVisible()
{
	CurTop	  = ScrBar->Position;
	CurBottom = CurTop + LineCount;
	if (CurPos.y<CurTop || CurPos.y>CurBottom) UpdatePos(true);
}

//---------------------------------------------------------------------------
//状態表示を設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetSttInf(UnicodeString msg)
{
	if (!isReady) return;

	//ルーラ
	if (RulerBox && RulerBox->Visible) RulerBox->Repaint();

	//ステータス
	if (SttHeader) {
		//ファイル情報/インクリメンタルサーチ
		if (msg.IsEmpty() && isIncSea) {
			msg.sprintf(_T(" %s \t%s"), (isIncMigemo? _T(" Migemo ") : _T(" サーチ ")), IncSeaWord.c_str());
			//位置/ヒット行数
			int hit_cnt = ScrPanel->HitLines->Count;
			if (hit_cnt>1) {
				int cnt = 0, pos = 0;
				for (int i=1; i<hit_cnt; i++) {
					cnt++;
					if (CurPos.y==(int)ScrPanel->HitLines->Objects[i]) {
						pos = cnt;
						break;
					}
				}
				if (pos>0) msg.cat_sprintf(_T("\t%u/%u(行)"), pos, hit_cnt - 1);
			}
		}

		SttHeader->Panels->Items[0]->Text = !msg.IsEmpty()? msg : SttHdrInf;

		//文字コード
		UnicodeString fext = get_extension(FileName);
		UnicodeString sttstr =
			(!isXDoc2Txt && !test_FileExt(fext, ".rtf") && isText)?
				get_NameOfCodePage(TxtBufList->Encoding->CodePage, false, HasBOM) : UnicodeString("----");
		SttHeader->Panels->Items[1]->Text = sttstr;

		//改行コード
		SttHeader->Panels->Items[2]->Text = !isXDoc2Txt? LineBreakStr : UnicodeString("--");

		//種別
		if (isXDoc2Txt) {
			sttstr = "XD2TX";
		}
		else if (isBinary) {
			sttstr = "BINARY";
		}
		else if (test_HtmlExt(fext)) {
			sttstr.sprintf(_T("%s"), isHtm2Txt? (ToMarkdown? _T("MARKDOWN") : _T("TEXT")) : _T("SOURCE"));
		}
		else if (test_FileExt(fext, ".md")) {
			sttstr = "MARKDOWN";
		}
		else if (isCSV) {
			sttstr.sprintf(_T("%s"), isTSV? _T("TSV") : _T("CSV"));
			if (isFixedLen) sttstr.Insert("FIXED - ", 1);
		}
		else if (test_FileExt(fext, ".rtf.wri")) {
			sttstr = "PLAIN";
		}
		else if (isJsonFmt) {
			sttstr = "JSON(整形)";
		}
		else if (!JsonErrMsg.IsEmpty()) {
			sttstr = "JSON(不正)";
		}
		else if (isAozora) {
			sttstr = "青空文庫";
		}
		else if (isLog) {
			sttstr = "LOG";
		}
		else if (isTail) {
			sttstr = "TAIL";
		}
		else {
			sttstr = "TEXT";
		}

		//ユーザ書式セクション(デバッグ表示)
		if (LogDebugInf) {
			UnicodeString sct = UserHighlight->CurSection;
			if (!sct.IsEmpty()) {
				if (sct.Pos(':')) {
					sct = get_tkn_r(sct, ":");
					if (sct.IsEmpty()) sct = "*";
				}
				sttstr.cat_sprintf(_T(":%s"), sct.c_str());
			}
		}

		SttHeader->Panels->Items[3]->Text = sttstr;

		sttstr = EmptyStr;
		//アドレス
		if (isBinary) {
			__int64 adr = get_CurAddrA();
			sttstr.sprintf(_T("%s/%s"), get_AddrStr(adr).c_str(), get_AddrStr(BinFileSize - 1).c_str());
		}
		//行桁
		else {
			TPoint clp = get_CurLinePos();
			sttstr.cat_sprintf(_T("%5u行 %4u桁"), clp.y, clp.x);
			if (isCSV && CsvCol>=0) sttstr.cat_sprintf(_T("  %u列"), CsvCol + 1);
			//選択状態
			if (SelStart!=SelEnd) {
				if (!isBoxMode && SelStart.x==0 && SelEnd.x==0)
					sttstr.cat_sprintf(_T(" %4u行選択"), abs((int)(SelEnd.y - SelStart.y)));
				else
					sttstr.cat_sprintf(_T(" %4u字選択"), str_len_unicode(get_SelText()));
			}
			else if (isCSV && isFixedLen && TopIsHeader && TxtBufList->Count>0) {
				UnicodeString top_str = TxtBufList->Strings[0];
				sttstr.cat_sprintf(_T(" [%s]"), (isTSV? get_tsv_item(top_str, CsvCol) : get_csv_item(top_str, CsvCol)).c_str());
			}
		}

		SttHeader->Panels->Items[4]->Text = sttstr;
		if (isIncSea) SttHeader->Repaint();
	}

	//カラー
	if (isText && TxtColorHint && ColorPanel && SelStart==SelEnd) {
		int xp;
		UnicodeString colstr = GetCurWord(false,
				"(#[0-9a-fA-F]{3}\\b)|(#[0-9a-fA-F]{6}\\b)|"
				"(0x|\\$)[0-9a-fA-F]{6,8}|"
				"(=[0-9a-fA-F]{6})", NULL, &xp);

		colstr = ReplaceStr(colstr, "$", "0x");
		int col = -1;
		//HTML
		if (remove_top_s(colstr, '#')) {
			UnicodeString r_str, g_str, b_str;
			if (colstr.Length()==6) {
				r_str = colstr.SubString(1, 2);
				g_str = colstr.SubString(3, 2);
				b_str = colstr.SubString(5, 2);
			}
			else if (colstr.Length()==3) {
				r_str = StringOfChar(colstr[1], 2);
				g_str = StringOfChar(colstr[2], 2);
				b_str = StringOfChar(colstr[3], 2);
			}
			colstr.sprintf(_T("0x00%s%s%s"), b_str.c_str(), g_str.c_str(), r_str.c_str());
			col = colstr.ToIntDef(-1);
		}
		//COLORREF
		else if (StartsStr("0x", colstr)) {
			col = colstr.ToIntDef(-1);
		}
		//Swatchbook.INI (RRBBGG)
		else if (remove_top_s(colstr, '=') && SameText(ExtractFileName(FileName), SWATCH_FILE)) {
			col = (int)xRRGGBB_to_col(colstr);
		}

		if (col!=-1) {
			ColorPanel->Color	= TColor(col);
			ColorPanel->Left	= TopXpos + cv_PosX_to_HchX(xp + 1) * HchWidth;
			ColorPanel->Top 	= ViewBox->Top + (CurPos.y - CurTop + 1) * LineHeight;
			ColorPanel->Visible = true;
		}
		else {
			ColorPanel->Visible = false;
		}
	}

	//文字情報
	if (isText && CharInfoForm->Visible) {
		UnicodeString chstr, inf;
		UnicodeString s = get_SelText();
		if (!s.IsEmpty()) {
			TMatch mt = TRegEx::Match(s, "&[a-zA-Z]+[0-9]*;");
			if (mt.Success) {
				s = ChEntRef_to_NumChRef(mt.Value);
				inf = mt.Value + " (文字実体参照)";
			}
			int		   uc = extract_UnicodePoint(s, "&#([0-9]{1,7});", true);
			if (uc==0) uc = extract_UnicodePoint(s, "&#x([0-9a-fA-F]{1,5});");
			if (uc==0) uc = extract_UnicodePoint(s, "\\\\u([0-9a-fA-F]{4})");
			if (uc==0) uc = extract_UnicodePoint(s, "\\\\U([0-9a-fA-F]{8})");
			if (uc>0) {
				chstr = UnicodePointToStr(uc);
				if (inf.IsEmpty()) inf = s + (StartsStr("&#", s)? " (数値文字参照)" : " (ユニバーサル文字名)");
			}
		}

		if (chstr.IsEmpty()) {
			UnicodeString lnstr = get_DispLine(CurPos.y);
			int p = CurPos.x;
			if (p<lnstr.Length()) {
				chstr = lnstr[++p];
				if (lnstr.IsLeadSurrogate(p) && p<lnstr.Length())
					chstr += lnstr[++p];
				else if (lnstr.IsTrailSurrogate(p))
					chstr = EmptyStr;
			}
		}

		CharInfoForm->UpdateChar(chstr, inf);
	}

	//CSV/TSVレコード
	if (isText && CsvRecForm->Visible) {
		line_rec *lp = get_LineRec(CurPos.y);
		if (lp->LineIdx==0 && lp->LineNo>0) CsvRecForm->UpdateRecord(TxtBufList, lp->LineNo - 1, CsvCol);
	}

	//インスペクタ
	if (isBinary) {
		if (InspectForm->Visible) {
			__int64 adr = get_CurAddrA();
			if (adr>=0) {
				InspectForm->Address = adr;
				unsigned int adr_r = adr - TopAddress;
				int cnt = std::min(128U, BinarySize - adr_r);	//***
				InspectForm->Bytes.Length = cnt;
				for (int i=0; i<cnt; i++) InspectForm->Bytes[i] = MMF->Bytes[adr_r + i];
			}
			else {
				InspectForm->Address = 0;
				InspectForm->Bytes.Length = 0;
			}
			InspectForm->UpdateValue();
		}
	}
}

//---------------------------------------------------------------------------
//情報ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, TRect Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	bool sel_flag = (Panel->Index==0 && isSelected);
	bool inc_flag = (Panel->Index==0 && isIncSea);

	cv->Brush->Color = inc_flag? ((StatusBar->Tag==SHOW_WARN_TAG)? col_bgWarn : col_bgView) :
					   sel_flag? col_selItem : col_bgInfHdr;
	cv->FillRect(Rect);

	int xp = Rect.Left;
	const int yp = Rect.Top;
	//インクリメンタルサーチ
	if (inc_flag) {
		UnicodeString s = Panel->Text;
		out_TextEx(cv, xp, yp, split_pre_tab(s), color_fgEmp, color_bgEmp);
		xp += ScaledInt(4, OwnerForm);
		out_TextEx(cv, xp, yp, split_pre_tab(s), col_fgView);

		//疑似キャレット
		if (UserModule->BlinkTimer->Tag>0) draw_Caret(cv, xp, yp + ScaledInt(2, OwnerForm));

		//行位置/ヒット行数
		if (!s.IsEmpty()) {
			int ws = cv->TextWidth(s);
			int w8 = get_CharWidth_Font(cv->Font, 8);
			int x1 = w8 * 4;
			while (xp>=(x1 - ws - w8) && x1<Rect.Right) x1 += w8;
			xp = x1 - ws;
			out_TextEx(cv, xp, yp, s, col_Comment);
		}
	}
	else {
		cv->Font->Color = sel_flag? ((col_fgSelItem!=col_None)? col_fgSelItem : col_fgList) : col_fgInfHdr;
		cv->TextOut(xp + 2, yp, Panel->Text);
	}

	//選択ファイル数
	if (Panel->Index==0 && sel_flag) {
		int sel_cnt = GetSelCount(GetCurList(true));
		if (sel_cnt>0) {
			UnicodeString tmp; tmp.sprintf(_T("選択 %u"), sel_cnt);
			cv->TextOut(Rect.Right - (cv->TextWidth(tmp) + 4), Rect.Top, tmp);
		}
	}
}

//---------------------------------------------------------------------------
//トグル動作のコマンドをパラメータで設定
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SetToggleSw(bool &sw, UnicodeString prm)
{
	sw = USAME_TI(prm, "ON")? true : USAME_TI(prm, "OFF")? false : !sw;
	return sw;
}
//---------------------------------------------------------------------------
//HtmlToText のオプション設定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SetHtmlToText(UnicodeString prm)
{
	TStringDynArray lst = split_strings_semicolon(prm);
	UnicodeString tgl_prm;
	for (int i=0; i<lst.Length; i++) {
		UnicodeString s = lst[i];
		if (contained_wd_i("ON|OFF", s)) tgl_prm = s;
		else if (SameText(s, "MD"))	 ToMarkdown = true;
		else if (SameText(s, "^MD")) ToMarkdown = !ToMarkdown;
		else if (SameText(s, "TX"))	 ToMarkdown = false;
	}
	SetToggleSw(isHtm2Txt, tgl_prm);
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onMouseDown(int x, int y)
{
	if (SelSkip) {
		SelSkip = false;
		return;
	}

	if (PtInSelected(x, y)) {
		PtInSelect = true;
	}
	else {
		IniSelected();

		SetPosFromPt(x, y);
		to_Lead_if_Trail();

		IniSelected(true);
		Repaint(true);
		SetSttInf();
		Selecting = true;

		//イメージプレビュー
		if (SubViewer->Visible) {
			UnicodeString fnam = GetCurImgFile();
			if (!fnam.IsEmpty() && !SameText(SubViewer->FileName, fnam)) SubViewer->DrawImage(fnam);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onMouseMove(int x, int y)
{
	if (PtInSelect) {
		UnicodeString tmp = GetClipboardText();
		Clipboard()->AsText = get_SelText();
		usr_SH->DoClipDragDrop();
		Clipboard()->AsText = tmp;
		PtInSelect = false;
	}
	else if (Selecting) {
		SetPosFromPt(x, y);
		SelEnd = CurPos;
		Repaint();
		SetSttInf();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onMouseUp()
{
	Selecting = PtInSelect = false;
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onDblClick()
{
	TPoint p = ViewBox->ScreenToClient(Mouse->CursorPos);
	//左側余白/行番号
	if (p.x<TopXpos) {
		//行選択(改行単位)
		SelLine(true);
	}
	//本文
	else {
		//URLを開く/カーソル位置の単語選択
		UnicodeString url = ClickableUrl? get_CurUrl() : EmptyStr;
		if (!url.IsEmpty()) Execute_ex(url); else GetCurWord(true);
	}

	SelSkip = true;	//DblClick後の選択を回避
}
//---------------------------------------------------------------------------
//ルーラのマウス操作
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::onRulerDblClick(TObject *Sender)
{
	if (!RulerBox || isBinary) return;

	TPoint p = RulerBox->ScreenToClient(Mouse->CursorPos);
	int wd = (p.x >= TopXpos)? (p.x - TopXpos)/HchWidth : 0;
	ViewFoldFitWin = (wd==0);
	if (!ViewFoldFitWin) ViewFoldWidth = wd;

	UpdateScr(get_CurLineNo());
}

//---------------------------------------------------------------------------
//カーソル移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::CursorLeft(bool sel)
{
	if (isBinary && !sel && TopAddress>0 && get_CurAddrR()==0) {
		CursorUp(); LineEnd();
	}
	else {
		IniSelected(sel);

		int dx = -1;
		if (isBinary) {
			if (CurPos.x<=50) {
				int p = CurPos.x;
				if (p>=24) p--;
				p = (p/3 - 1)*3;
				if (p>=24) p++;
				dx = p - CurPos.x;
			}
		}
		else {
			if (CurPos.x>0 && get_DispLine(CurPos.y).IsTrailSurrogate(CurPos.x)) dx--;
		}

		int n = CurPos.x + dx;
		if (n<0) {
			if (CurPos.y>0) {
				CurPos.y--;
				n = get_CurEndPos();
			}
			else {
				n = 0;
			}
		}
		CurPos.x = n;
		CurHchX = cv_PosX_to_HchX(CurPos.x);

		if (sel) set_SelEnd();
		UpdatePos();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::CursorRight(
	bool sel,			//選択					(default = false)
	bool skip_end)		//行末文字から次行頭へ	(default = false)
{
	if (isBinary && !sel && isContinue && get_CurAddrR()==BinarySize-1) {
		CursorDown(); LineTop();
	}
	else {
		IniSelected(sel);

		int dx = 1;
		if (isBinary) {
			if (CurPos.x<50) {
				int p = CurPos.x;
				if (p>=24) p--;
				p /= 3;
				if (p==15) {
					p = 50;
				}
				else {
					p = (p + 1)*3;
					if (p>=24) p++;
				}
				dx = p - CurPos.x;
			}
		}
		else {
			UnicodeString lnstr = get_DispLine(CurPos.y);
			if (CurPos.x<lnstr.Length() && lnstr.IsLeadSurrogate(CurPos.x + 1)) dx++;
		}

		if (isBoxMode) {
			if (CurHchX<MaxHchX) CurPos.x += dx;
		}
		else {
			int n = CurPos.x + dx;
			if (n > (get_CurEndPos() - (skip_end? 1 : 0))) {
				if (CurPos.y < MaxDispLine - (CsvRecForm->Visible? 2 : 1)) {
					CurPos.y++;	//次行へ
					n = 0;
				}
				else {
					n = CurPos.x;
				}
			}
			CurPos.x = n;
		}
		CurHchX = cv_PosX_to_HchX(CurPos.x);

		if (sel) set_SelEnd();
		UpdatePos();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::CursorUp(bool sel, UnicodeString prm)
{
	int n = get_MovePrm(prm);

	if (isBinary && !sel && CurPos.y==0 && TopAddress>0) {
		__int64 lst_adr = get_CurAddrA();
		__int64 adr = std::max<__int64>(TopAddress - ViewBinLimitSize, 0);
		AssignBin(adr, true);
		ToAddrR(lst_adr - 16*n - TopAddress);
	}
	else {
		UpdateVisible();
		if (CsvRecForm->Visible && n==1 && !isFixedLen) {
			MoveLineTop(false, sel);
		}
		else {
			IniSelected(sel);
			for (int i=0; i<n && CurPos.y>0; i++) CurPos.y--;
			adjust_PosX();
			if (sel) set_SelEnd();
			UpdatePos();
			to_Lead_if_Trail(sel);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::CursorDown(bool sel, UnicodeString prm)
{
	int n = get_MovePrm(prm);

	if (isBinary && !sel && isContinue && (CurPos.y >= MaxDispLine-2)) {
		__int64 lst_adr = get_CurAddrA();
		AssignBin(TopAddress + BinarySize, true);
		ToAddrR(lst_adr + 16*n - TopAddress);
	}
	else {
		UpdateVisible();
		if (CsvRecForm->Visible && n==1 && !isFixedLen) {
			if (CurPos.y < MaxDispLine-2) MoveLineTop(true, sel);
		}
		else {
			IniSelected(sel);
			for (int i=0; i<n && CurPos.y<MaxDispLine-1; i++) CurPos.y++;
			adjust_PosX();
			if (sel) set_SelEnd();
			UpdatePos();
			to_Lead_if_Trail(sel);
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::MovePage(bool is_down, bool sel)
{
	UpdateVisible();

	int r = CurPos.y - CurTop;
	int n = CurPos.y;
	if (is_down) {
	 	n += (LineCount + 1);
		if (n>=MaxDispLine) n = MaxDispLine - ((CsvRecForm->Visible || isBinary)? 2 : 1);
	}
	else {
		n -= (LineCount + 1);
		if (n<0) n = 0;
	}

	if (CurPos.y!=n) {
		IniSelected(sel);
		CurPos.y = n;
		adjust_PosX();
		if (sel) set_SelEnd();

		CurTop = CurPos.y - r;
		if (CurTop<1) CurTop = 1;
		CurTop = std::min(CurTop, ScrBar->Max - ScrBar->LargeChange - 1);
		CurBottom = CurTop + LineCount;

		Repaint(true);
		ScrBar->Position = CurTop;
	}
	else if (isBinary && !sel) {
		if (is_down) {
			if (isContinue) CursorDown();
		}
		else {
			if (TopAddress>0) CursorUp();
		}
	}
}

//---------------------------------------------------------------------------
//スクロール時の表示調整
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::ScrollAdjust()
{
	CurTop	  = ScrBar->Position;
	CurBottom = CurTop + LineCount;
	//カーソルを常に可視領域に
	if (TvCursorVisible) {
		if (CurPos.y < CurTop-1)
			CurPos.y = CurTop -1;
		else if (CurPos.y > CurBottom-1)
			CurPos.y = CurBottom - 1;
	}
	SetSttInf();
	Repaint();
}

//---------------------------------------------------------------------------
//スクロール
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::MoveScroll(
	bool is_down,		//下方向
	UnicodeString prm,	//パラメータ
	bool move_csr)		//カーソルも移動 (default = false)
{
	if (is_down && CurBottom>=MaxDispLine) return;
	if (!is_down && CurTop<=1) return;

	int last_top = CurTop;

	if (prm.IsEmpty()) prm = "MW";
	int n = get_MovePrm(prm);
	CurTop += is_down? n : -n;

	if (is_down && CurTop>MaxDispLine)	CurTop = MaxDispLine;
	if (!is_down && CurTop<0)			CurTop = 0;

	if (move_csr) {
		if (is_down) {
			CurPos.y += (CurTop - last_top);
			if (CurPos.y > CurBottom-1) CurPos.y = CurBottom - 1;
		}
		else {
			CurPos.y -= (last_top - CurTop);
			if (CurPos.y<CurTop) CurPos.y = CurTop;
		}
	}

	CurBottom = CurTop + LineCount;
	ScrBar->Position = CurTop;
	Repaint(true);
}

//---------------------------------------------------------------------------
//行頭に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::LineTop(bool sel)
{
	IniSelected(sel);
	CurPos.x = CurHchX = 0;
	if (sel) set_SelEnd();
	UpdatePos();
}

//---------------------------------------------------------------------------
//行頭に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::MoveLineTop(
	bool is_next,	//true = 次/ false = 前
	bool sel)
{
	IniSelected(sel);
	int lno = get_CurLineNo() + (is_next? 1 : -1);
	for (int i=CurPos.y; i>=0 && i<MaxDispLine; i += (is_next? 1 : -1)) {
		line_rec *lp = get_LineRec(i);
		if ((is_next && lp->LineNo<lno) || (!is_next && (lp->LineIdx>0 || lp->LineNo>lno))) continue;
		CurPos  = Point(0, i);
		CurHchX = cv_PosX_to_HchX(CurPos.x);
		if (isSelMode) SelEnd = CurPos;
		UpdatePos(true);
		break;
	}
}

//---------------------------------------------------------------------------
//行末に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::LineEnd(bool sel)
{
	IniSelected(sel);
	CurPos.x = get_CurEndPos();
	CurHchX  = cv_PosX_to_HchX(CurPos.x);
	if (sel) set_SelEnd();
	UpdatePos();
}

//---------------------------------------------------------------------------
//先頭に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::TextTop(bool sel)
{
	if (isBinary && !sel && TopAddress!=0) {
		AssignBin(0, true, 0);
	}
	else {
		IniSelected(sel);
		CurPos.y = 0;
		CurPos.x = CurHchX = 0;
		if (sel) set_SelEnd();
		ScrBar->Position = 1;
		UpdatePos();
	}

	if (BitmapForm->Visible) BitmapForm->MapScrBox->VertScrollBar->Position = 0;
}
//---------------------------------------------------------------------------
//最後尾に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::TextEnd(bool sel)
{
	if (isBinary && isContinue) {
		IniSelected();
		ToAddrA(BinFileSize - 1);
	}
	else {
		IniSelected(sel);
		CurPos.y = MaxDispLine - (CsvRecForm->Visible? 2 : 1);
		CurPos.x = CsvRecForm->Visible? 0 : get_CurEndPos();
		CurHchX  = cv_PosX_to_HchX(CurPos.x);
		if (sel) set_SelEnd();
		UpdatePos(true);
	}

	if (BitmapForm->Visible)
		BitmapForm->MapScrBox->VertScrollBar->Position = BitmapForm->MapScrBox->VertScrollBar->Range;
}

//---------------------------------------------------------------------------
//クリップボードにコピー
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::ClipCopy(
	bool append)	//true = 追加コピー (default = false)
{
	if (SelStart==SelEnd) return;

	UnicodeString sel_str;

	if (isBinary && !isBoxMode) {
		std::unique_ptr<TStringList> sel_lst(new TStringList());
		GetDumpList(sel_lst.get());
		sel_str = sel_lst->Text;
	}
	else {
		sel_str = get_SelText(true);
	}

	if (!sel_str.IsEmpty()) {
		if (append) sel_str.Insert(GetClipboardText(), 1);	//追加
		copy_to_Clipboard(sel_str);
		IniSelected();
		SetSttInf();
		Repaint(true);
	}
}

//---------------------------------------------------------------------------
//次の単語に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::WordRight(bool sel)
{
	if (isBinary) {
		CursorRight(sel); CursorRight(sel);
	}
	else {
		IniSelected(sel);

		bool moved = false;
		if (isCSV && isFixedLen && get_LineRec(CurPos.y)->LineIdx==0
			&& FixWdList.Length>0 && CsvCol<(FixWdList.Length - 1))
		{
			moved = set_PosFromCol(CsvCol + 1);
			if (!moved && (CurPos.x < get_CurEndPos())) {
				LineEnd(sel);
				moved = true;
			}
		}

		if (!moved) {
			UnicodeString lbuf = get_CurLine();
			if (!has_CR(lbuf)) lbuf += get_DispLine(CurPos.y + 1);

			TMatchCollection mts = TRegEx::Matches(lbuf, WORD_MATCH_PTN);
			bool found = false;
			for (int i=0; i<mts.Count && !found; i++) {
				int idx = mts.Item[i].Index - 1;
				if (idx>CurPos.x) {
					CurPos.x = idx;
					CurPos	 = nrm_Pos(CurPos);
					found	 = true;
				}
			}
			if (!found) {
				if (CurPos.x < get_CurEndPos()) {
					CurPos.x = get_CurEndPos();
				}
				else if (CurPos.y < MaxDispLine-1) {
					CurPos.y++;
					CurPos.x = 0;
				}
			}
			CurHchX = cv_PosX_to_HchX(CurPos.x);
		}

		if (sel) set_SelEnd();
		UpdatePos();
	}
}

//---------------------------------------------------------------------------
//前の単語に移動
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::WordLeft(bool sel)
{
	if (isBinary) {
		CursorLeft(sel); CursorLeft(sel);
	}
	else {
		IniSelected(sel);

		line_rec *lp = get_LineRec(CurPos.y);
		if (isCSV && isFixedLen && FixWdList.Length>0 && lp->LineIdx==0 && CsvCol>0) {
			set_PosFromCol(CsvCol - 1);
		}
		else {
			UnicodeString lbuf = get_CurLine().SubString(1, CurPos.x);
			int ofs = 0;
			if (CurPos.y>0) {
				UnicodeString sbuf = get_DispLine(CurPos.y - 1);
				if (!has_CR(sbuf)) {
					lbuf = sbuf + lbuf;
					ofs += sbuf.Length();
				}
			}

			bool found = false;
			if (!lbuf.IsEmpty()) {
				TMatchCollection mts = TRegEx::Matches(lbuf, WORD_MATCH_PTN);
				if (mts.Count>0) {
					CurPos.x = mts.Item[mts.Count - 1].Index - 1 - ofs;
					CurPos	 = nrm_Pos(CurPos);
					found	 = true;
				}
			}
			if (!found) {
				if (CurPos.y>0) {
					CurPos.y--;
					CurPos.x = get_CurEndPos();
				}
				else {
					CurPos.x = 0;
				}
			}
			CurHchX = cv_PosX_to_HchX(CurPos.x);
		}

		if (sel) set_SelEnd();
		UpdatePos();
	}
}

//---------------------------------------------------------------------------
//選択文字列を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::get_SelText(
	bool clr_sel)	//取得後、選択状態をクリア (default = false)
{
	if (SelStart==SelEnd) return EmptyStr;

	UnicodeString sel_str;
	//１行
	if (SelStart.y==SelEnd.y) {
		TPoint sp0 = SelStart;
		TPoint sp1 = SelEnd;
		if (sp0.x>sp1.x) std::swap(sp0, sp1);
		sel_str = get_DispLine(sp0.y).SubString(sp0.x + 1, sp1.x - sp0.x);
	}
	//複数行
	else {
		//箱形選択
		if (isBoxMode) {
			TPoint sp0 = BoxStart;
			TPoint sp1 = BoxEnd;
			bool flag = (sp0.y==sp1.y)? (sp0.x>sp1.x) : (sp0.y>sp1.y);
			if (flag) std::swap(sp0, sp1);
			if (sp0.x>sp1.x) std::swap(sp0.x, sp1.x);
			int wp0 = sp0.x * HchWidth;
			int wp1 = sp1.x * HchWidth;
			for (int i=sp0.y; i<=sp1.y && i<MaxDispLine; i++) {
				UnicodeString lbuf = get_DispLine(i);
				if (get_LineRec(i)->LineIdx>0) continue;
				UnicodeString sbuf;
				int w = 0;
				for (int j=1; j<=lbuf.Length(); j++) {
					WideChar c = lbuf[j];
					w = add_CharWidth(c, w);
					if (w>wp1) break;
					if (w>wp0) sbuf.cat_sprintf(_T("%c"), c);
				}
				if (!has_CR(sbuf)) sbuf += "\r\n";
				sel_str += sbuf;
			}
		}
		//通常選択
		else {
			TPoint sp0 = SelStart;
			TPoint sp1 = SelEnd;
			bool flag = (sp0.y==sp1.y)? (sp0.x>sp1.x) : (sp0.y>sp1.y);
			if (flag) std::swap(sp0, sp1);
			for (int i=sp0.y; i<=sp1.y && i<MaxDispLine; i++) {
				UnicodeString lbuf = get_DispLine(i);
				if (i==sp0.y) {
					if (sp0.x>0) lbuf.Delete(1, sp0.x);
				}
				if (i==sp1.y) {
					if (sp1.x<lbuf.Length()) lbuf = lbuf.SubString(1, sp1.x);
				}
				sel_str += ReplaceStr(lbuf, "\n", "\r\n");
				if (isBinary) sel_str += "\r\n";
			}
		}
	}

	if (clr_sel) {
		IniSelected();
		SetSttInf();
	}

	return sel_str;
}

//---------------------------------------------------------------------------
//カーソル位置/行からURLを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::get_CurUrl()
{
	UnicodeString lbuf = get_SelText();
	//固定長表示
	if (isCSV && isFixedLen && CsvCol>=0) {
		line_rec *lp = get_LineRec(CurPos.y);
		if (lp->LineNo>0) {
			lbuf = TxtBufList->Strings[lp->LineNo - 1];
			lbuf = isTSV? get_tsv_item(lbuf, CsvCol) : get_csv_item(lbuf, CsvCol);
		}
	}
	//通常表示
	else {
		lbuf = get_CurLine(true);
	}
	return extract_URL(lbuf);
}

//---------------------------------------------------------------------------
//カーソル位置の文字を取得
//---------------------------------------------------------------------------
WideChar __fastcall TTxtViewer::get_CurChar()
{
	WideChar chstr = '\0';
	UnicodeString lnstr = get_DispLine(CurPos.y);
	if (CurPos.x<lnstr.Length()) chstr = lnstr[CurPos.x + 1];
	return chstr;
}

//---------------------------------------------------------------------------
//カーソル行の文字列を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::get_CurLine(
	bool to_cr)		//改行まで取得 (default = false)
{
	UnicodeString lnstr;
	if (isBinary || !to_cr) {
		lnstr = get_DispLine(CurPos.y);
	}
	else {
		for (int i=CurPos.y; i<MaxDispLine; i++) {
			lnstr += get_DispLine(i);
			if (has_CR(lnstr)) break;
		}
	}
	return lnstr;
}
//---------------------------------------------------------------------------
//カーソル行の行番号を取得
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::get_CurLineNo()
{
	if (!isReady) return 0;

	int idx = std::min<int>(CurPos.y, MaxDispLine - 1);
	if (idx>=0) {
		line_rec *lp = get_LineRec(idx);
		return (lp->LineNo!=-1)? lp->LineNo : MaxLine;
	}
	else return 1;
}

//---------------------------------------------------------------------------
//カーソル位置の絶対アドレスを取得 (バイナリ)
//---------------------------------------------------------------------------
__int64 __fastcall TTxtViewer::get_CurAddrA()
{
	return TopAddress + get_CurAddrR();
}
//---------------------------------------------------------------------------
//カーソル位置の相対アドレスを取得 (バイナリ)
//---------------------------------------------------------------------------
unsigned int __fastcall TTxtViewer::get_CurAddrR()
{
	return std::min<unsigned int>(CurPos.y*16 + get_OfsR(CurPos.x), BinarySize - 1);
}
//---------------------------------------------------------------------------
//行頭からのオフセットを取得 (バイナリ)
//---------------------------------------------------------------------------
unsigned int __fastcall TTxtViewer::get_OfsR(int xp)
{
	unsigned int p = xp;
	if (p>=50) {
		p -= 50;
	}
	else {
		if (p>=24) p--;
		p /= 3;
	}
	if (p>15) p = 15;
	return p;
}

//---------------------------------------------------------------------------
//カーソル位置の行(y)・桁(x)を取得 (テキスト)
//---------------------------------------------------------------------------
TPoint __fastcall TTxtViewer::get_CurLinePos()
{
	if (CurPos.y < MaxDispLine-1) {
		int n = 1;
		int p = (CurPos.x>0)? str_len_unicode(get_DispLine(CurPos.y).SubString(1, CurPos.x)) + 1 : 1;
		for (int i=CurPos.y; i>=0; i--) {
			if (i!=CurPos.y) p += str_len_unicode(get_DispLine(i));
			line_rec *lp = get_LineRec(i);
			if (lp->LineIdx==0) { n = lp->LineNo;  break; }
		}
		return Point(p, n);
	}
	else {
		return Point(1, MaxLine);
	}
}

//---------------------------------------------------------------------------
//全て選択
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SelectAll()
{
	if (MaxDispLine>0) {
		unsigned int idx = MaxDispLine - 1;
		unsigned int pos = get_DispLine(idx).Length();
		SelEnd	  = Point(pos, idx);
		SelStart  = Point(0, 0);
		isBoxMode = false;
		Repaint();
	}
}

//---------------------------------------------------------------------------
//カーソル位置の単語を取得/選択
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::GetCurWord(
	bool select,			//選択				(default = false)
	UnicodeString ptn,		//マッチパターン	(default = EmptyStr)
	int *p_s,				//開始位置			(default = NULL)
	int *p_e)				//終了位置			(default = NULL)
{
	if (select) IniSelected();

	UnicodeString lbuf = get_CurLine(true);
	line_rec *lp = get_LineRec(CurPos.y);
	int ofs = 0;
	if (lp->LineIdx>0 && CurPos.y>0) {
		UnicodeString pbuf = get_DispLine(CurPos.y - 1);
		lbuf = pbuf + lbuf;
		ofs  = pbuf.Length();
	}

	UnicodeString ret_str;
	if (!lbuf.IsEmpty()) {
		TMatchCollection  mts;
		if (!ptn.IsEmpty()) {
			mts = TRegEx::Matches(lbuf, ptn);
		}
		else {
			mts = TRegEx::Matches(lbuf, LINK_MATCH_PTN);
			if (mts.Count==0) mts = TRegEx::Matches(lbuf, WORD_MATCH_PTN);
		}

		for (int i=0; i<mts.Count; i++) {
			int p0 = mts.Item[i].Index - 1 - ofs;
			int p1 = p0 + mts.Item[i].Length;
			if (p0<=CurPos.x && CurPos.x<p1) {
				ret_str = mts.Item[i].Value;
				if (p_s) *p_s = p0;
				if (p_e) *p_e = p1;
				if (select) {
					SelStart = nrm_Pos(Point(p0, CurPos.y));
					SelEnd	 = nrm_Pos(Point(p1, CurPos.y));
					CurPos   = SelEnd;
					CurHchX	 = cv_PosX_to_HchX(CurPos.x);
					Repaint(true);
				}
				break;
			}
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//改行単位で行全体を選択
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::SelLine(bool cr)
{
	inh_Repaint = true;
	LineTop();
	if (cr) {
		while (CurPos.y > 0) {
			if (get_LineRec(CurPos.y)->LineIdx==0) break;
			CursorUp();
		}
	}
	LineEnd(true);
	if (cr) {
		while (CurPos.y < MaxDispLine-1) {
			if (get_LineRec(CurPos.y)->hasCR) break;
			CursorDown(true);
		}
	}
	inh_Repaint = false;
	Repaint();
}

//---------------------------------------------------------------------------
//コードページの変更
// 戻り値 : 0 = 不正な指定
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::saveDef_CodePage(int code_page)
{
	if (code_page>0 && code_page!=TxtBufList->Encoding->CodePage) {
		write_NyanFiDef(FileName, "CodePage", IntToStr(code_page));
	}
}
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::change_CodePage(UnicodeString prm)
{
	std::unique_ptr<TStringList> cp_lst(new TStringList());
	cp_lst->Text =
		"932\n"		//Shift_JIS
		"50220\n"	//JIS(ISO-2022-JP)
		"20932\n"	//EUC-JP
		"1252\n"	//Latin-1
		"65001\n"	//UTF-8
		"1200\n";	//UTF-16

	int code_page;
	if (!prm.IsEmpty()) {
		code_page = (cp_lst->IndexOf(prm)==-1)? 0 : prm.ToIntDef(932);
	}
	//無指定なら次のコードページに切り替え
	else {
		int pn = cp_lst->IndexOf(IntToStr((int)TxtBufList->Encoding->CodePage));
		code_page = (pn!=-1)? cp_lst->Strings[(pn + 1)%cp_lst->Count].ToIntDef(932) : 932;
	}

	saveDef_CodePage(code_page);
	return code_page;
}

//---------------------------------------------------------------------------
//カーソル位置の画像ファイル名を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::GetCurImgFile()
{
	UnicodeString fnam = GetCurWord(false, "[^\"(=*?<>|（ ]+\\.\\w+\\b");
	remove_top_text(fnam, _T("file:///"));
	fnam = slash_to_yen(fnam);
	fnam = to_absolute_name(fnam, ExtractFilePath(FileName));
	UnicodeString fext = get_extension(fnam);
	if (test_FileExt(fext, get_img_fext())) return fnam;
	return EmptyStr;
}

//---------------------------------------------------------------------------
//パラメータから移動行数を取得
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::get_MovePrm(UnicodeString prm)
{
	int n = 1;
	if (!prm.IsEmpty()) {
		switch (idx_of_word_i(_T("HP|FP|MW"), prm)) {
		case  0: n = (LineCount + 1)/2;	break;
		case  1: n = LineCount + 1;		break;
		case  2: n = ViewWheelScrLn;	break;
		default: n = prm.ToIntDef(1);
		}
	}
	return n;
}

//---------------------------------------------------------------------------
//バイト文字列(スペース区切りの2桁16進)に変換
// 戻り値 : バイト数
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::to_Bytes(
	UnicodeString s,
	bool &case_sw,		//引用符で囲まれた文字列の大小文字を区別
	int code_page)
{
	int len = s.Length();
	if (len==0) return 0;

	//文字列
	if (is_quot(s)) {
		s = exclude_quot(s);
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
		if (!case_sw) {
			FindByte0 = enc->GetBytes(s.UpperCase());
			FindByte1 = enc->GetBytes(s.LowerCase());
		}
		else {
			FindByte0 = enc->GetBytes(s);
		}

		FindMask.Length = FindByte0.Length;
		for (int i=0; i<FindMask.Length; i++) FindMask[i] = 0xff;
	}
	//数値
	else {
		case_sw = true;
		TStringDynArray b_lst = SplitString(Trim(s), " ");
		FindByte0.Length = b_lst.Length;
		FindMask.Length  = b_lst.Length;
		try {
			for (int i=0; i<b_lst.Length; i++) {
				UnicodeString s = b_lst[i];
				//HEX
				if (s.Length()==2) {
					//ワイルドカード ? からマスク設定
					BYTE m = 0xff;
					if (s[1]=='?') m &= 0x0f;
					if (s[2]=='?') m &= 0xf0;
					FindMask[i] = m;
					s = ReplaceStr(s, "?", "0");
					//検索値を設定
					FindByte0[i] = ("0x" + s).ToInt();
				}
				//BIT
				else if (s.Length()==8) {
					BYTE b = 0;
					BYTE m = 0;
					for (int i=1; i<=8; i++) {
						b <<= 1;  m <<= 1;
						if		(s[i]=='1') { b |= 0x01;  m |= 0x01; }
						else if (s[i]=='0') m |= 0x01;
						else if (s[i]!='?') Abort();
					}
					FindByte0[i] = b;
					FindMask[i]  = m;
				}
				else Abort();
			}
		}
		catch (...) {
			FindByte0.Length = 0;
			beep_Warn();
		}
	}
	return FindByte0.Length;
}

//---------------------------------------------------------------------------
//文字列検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchCore(
	bool is_down,		//下方向
	UnicodeString kwd,	//検索語
	bool case_sw,		//大小文字を区別
	bool reg_sw,		//正規表現
	bool bytes_sw,		//バイト列検索
	bool from_pos)		//現在位置から検索	(default = false)
{
	LastFound = false;

	if (!kwd.IsEmpty() && (!reg_sw || chk_RegExPtn(kwd))) {
		//バイト列検索
		if (isBinary && bytes_sw) {
			LastFound = is_down? SearchDownBytes(kwd, case_sw, reg_sw) : SearchUpBytes(kwd, case_sw, reg_sw);

			//ヒット行リストの更新
			if (ScrPanel->KeyWordChanged(kwd, MaxDispLine, case_sw, BinCodePage)
				&& to_Bytes(kwd, case_sw, BinCodePage)>0)
			{
				cursor_HourGlass();
				unsigned int adr = 0;
				int p = 0;
				while (adr<BinarySize) {
					if (case_sw? ((MMF->Bytes[adr] & FindMask[p])==(FindByte0[p] & FindMask[p])) :
								 (MMF->Bytes[adr]==FindByte0[p] || MMF->Bytes[adr]==FindByte1[p]))
					{
						p++;
						if (p>=FindByte0.Length) {
							int idx = (adr - p + 1)/16;
							ScrPanel->AddHitLine(idx);
							p = 0;
							adr = (idx + 1) * 16;
						}
						else adr++;
					}
					else {
						p = 0;
						adr++;
					}
				}
				ScrPanel->Repaint();
				cursor_Default();
			}
		}
		//文字列検索
		else {
			cursor_HourGlass();
			TRegExOptions opt;
			if (!case_sw) opt << roIgnoreCase;
			for (int i=CurPos.y; i>=0 && i<MaxDispLine; i += (is_down? 1 : -1)) {
				UnicodeString lbuf, sbuf;
				int p = 0, ofs = 0;
				if (is_down) {
					lbuf = get_DispLine(i);
					if (isText) {
						for (int i2=i+1; i2<MaxDispLine; i2++) {
							if (has_CR(lbuf) || get_LineRec(i2)->LineIdx==0) break;
							lbuf += get_DispLine(i2);
						}
					}

					p = (i==CurPos.y)? CurPos.x + (from_pos? 1 : 2) : 1;
					sbuf = lbuf.SubString(p, lbuf.Length() - p + 1);
				}
				else {
					if (isText && i>1) {
						lbuf = get_DispLine(i - 1);
						if (has_CR(lbuf)) lbuf = EmptyStr;
						ofs += lbuf.Length();
					}
					lbuf += get_DispLine(i);
					if (i==CurPos.y) lbuf = lbuf.SubString(1, CurPos.x + ofs);
				}

				int match_p;
				int match_len;
				if (reg_sw) {
					if (is_down) {
						if (StartsStr('^', kwd) && p>1) {
							match_p = 0;
						}
						else {
							TMatch mt = TRegEx::Match(sbuf, kwd, opt);
							if (mt.Success) {
								match_p   = mt.Index;
								match_len = mt.Length;
							}
							else {
								match_p = 0;
							}
						}
					}
					else {
						TMatchCollection mts = TRegEx::Matches(lbuf, kwd, opt);
						if (mts.Count>0) {
							match_p   = mts.Item[mts.Count - 1].Index;
							match_len = mts.Item[mts.Count - 1].Length;
						}
						else {
							match_p = 0;
						}
					}
				}
				else {
					match_p = is_down? (case_sw? sbuf.Pos(kwd) : pos_i(kwd, sbuf)) :
									   (case_sw? pos_r(kwd, lbuf) : pos_r_i(kwd, lbuf));
					match_len = kwd.Length();
				}

				if (match_p>0) {
					match_p  += is_down? (p - 1) : -ofs;
					LastFound = true;
					isSelMode = isBoxMode = false;
					CurPos	  = nrm_Pos(Point(match_p - 1, i));
					CurHchX   = cv_PosX_to_HchX(CurPos.x);
					SelStart  = CurPos;
					SelEnd	  = nrm_Pos(Point(CurPos.x + match_len, CurPos.y));
					UpdatePos(true);
					break;
				}
			}
			cursor_Default();

			//ヒット行リストの更新
			if (ScrPanel->KeyWordChanged(kwd, MaxDispLine, case_sw)) {
				cursor_HourGlass();
				TRegExOptions opt;
				if (!case_sw) opt << roIgnoreCase;
				for (int i=0; i<MaxDispLine; i++) {
					UnicodeString lbuf = get_DispLine(i);
					if (reg_sw? TRegEx::IsMatch(lbuf, kwd, opt) : case_sw? lbuf.Pos(kwd) : pos_i(kwd, lbuf)) {
						ScrPanel->AddHitLine(i);
					}
				}
				ScrPanel->Repaint();
				cursor_Default();
			}
		}
	}
	else {
		ScrPanel->KeyWordChanged(EmptyStr, 0);
	}

	if (!LastFound && !XCMD_IsBusy && !isIncSea) beep_Warn();

	return LastFound;
}

//---------------------------------------------------------------------------
//下方向に文字列検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchDown(
	UnicodeString kwd,	//検索語
	bool case_sw,		//大小文字を区別	(default = false)
	bool reg_sw,		//正規表現			(default = false)
	bool bytes_sw,		//バイト列検索		(default = false)
	bool from_pos)		//現在位置から検索	(default = false)
{
	return SearchCore(true, kwd, case_sw, reg_sw, bytes_sw, from_pos);
}
//---------------------------------------------------------------------------
//上方向に文字列検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchUp(UnicodeString kwd, bool case_sw, bool reg_sw, bool bytes_sw)
{
	return SearchCore(false, kwd, case_sw, reg_sw, bytes_sw);
}

//---------------------------------------------------------------------------
//下方向にバイト列検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchDownBytes(UnicodeString kwd, bool case_sw, bool reg_sw)
{
	if (to_Bytes(kwd, case_sw, BinCodePage)==0) return false;

	cursor_HourGlass();
	bool found = false;
	unsigned int adr = get_CurAddrR() + 1;
	int p = 0;
	while (adr<BinarySize) {
		if (case_sw? ((MMF->Bytes[adr] & FindMask[p])==(FindByte0[p] & FindMask[p])) :
					 (MMF->Bytes[adr]==FindByte0[p] || MMF->Bytes[adr]==FindByte1[p]))
		{
			p++;
			if (p>=FindByte0.Length) {
				adr -= (FindByte0.Length - 1);
				found = ToAddrR(adr);
				break;
			}
		}
		else {
			p = 0;
		}
		adr++;
	}
	cursor_Default();
	return found;
}

//---------------------------------------------------------------------------
//上方向にバイト列検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchUpBytes(UnicodeString kwd, bool case_sw, bool reg_sw)
{
	if (to_Bytes(kwd, case_sw, BinCodePage)==0) return false;
	unsigned int adr = get_CurAddrR();	if (adr==0) return false;

	cursor_HourGlass();
	bool found = false;
	if (FindByte0.Length==1) adr--;
	int p = FindByte0.Length - 1;
	for (;;) {
		if (case_sw? ((MMF->Bytes[adr] & FindMask[p]) == (FindByte0[p] & FindMask[p])) :
					 (MMF->Bytes[adr]==FindByte0[p] || MMF->Bytes[adr]==FindByte1[p]))
		{
			if (p==0) { found = ToAddrR(adr);  break; }
			p--;
		}
		else {
			p = FindByte0.Length - 1;
		}

		if (adr==0) break;
		adr--;
	}
	cursor_Default();
	return found;
}

//---------------------------------------------------------------------------
//選択文字列を検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchSel(
	bool up_sw,		//true = 上方向/ false = 下方向
	bool em_sw)		//true = マッチ語を強調表示
{
	UnicodeString s = get_SelText();
	if (!s.IsEmpty()) LastSelWord = s; else s = LastSelWord;

	if (em_sw) {
		RegExPtn  = TRegEx::Escape(s);
		FindWord  = s;
		Highlight = true;
		Repaint(true);
	}

	return (up_sw? SearchUp(s, false) : SearchDown(s, false));
}

//---------------------------------------------------------------------------
//対応する行要素を検索
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchPairCore(UnicodeString bgn_ptn, UnicodeString end_ptn)
{
	if (!chk_RegExPtn(bgn_ptn) || !chk_RegExPtn(end_ptn)) {
		GlobalErrMsg = LoadUsrMsg(USTR_IllegalRegEx);
		return false;
	}

	bool found = false;
	cursor_HourGlass();
	UnicodeString s = get_CurLine();
	//begin --> end
	TRegExOptions opt; opt << roIgnoreCase;
	if (TRegEx::IsMatch(s, bgn_ptn, opt)) {
		int lvl = 0;
		for (int i=CurPos.y + 1; !found && i<MaxDispLine; i++) {
			s = get_DispLine(i);
			if (TRegEx::IsMatch(s, end_ptn, opt)) {
				if (lvl==0) {
					CurPos = Point(s.Length() - TrimLeft(s).Length(), i);
					found  = true;
				}
				else {
					lvl--;
				}
			}
			else if (TRegEx::IsMatch(s, bgn_ptn, opt)) {
				lvl++;
			}
		}
	}
	//end --> begin
	else if (TRegEx::IsMatch(s, end_ptn, opt)) {
		int lvl = 0;
		for (int i=CurPos.y - 1; !found && i>=0; i--) {
			s = get_DispLine(i);
			if (TRegEx::IsMatch(s, bgn_ptn, opt)) {
				if (lvl==0) {
					CurPos = Point(s.Length() - TrimLeft(s).Length(), i);
					found  = true;
				}
				else {
					lvl--;
				}
			}
			else if (TRegEx::IsMatch(s, end_ptn, opt)) {
				lvl++;
			}
		}
	}

	cursor_Default();
	return found;
}

//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::SearchPair(
	UnicodeString prm)	// "/開始正規表現/;/終了正規表現/"
{
	UnicodeString br_str = "（〔［｛〈《「『【({[｢";
	UnicodeString kt_str = "）〕］｝〉》」』】)}]｣";

	WideChar bk_ch = get_CurChar();
	int p_b = br_str.Pos(bk_ch);
	int p_k = kt_str.Pos(bk_ch);

	bool found = false;
	//括弧開き → 括弧閉じ
	if (p_b>0) {
		WideChar b_ch = bk_ch;
		WideChar k_ch = kt_str[p_b];
		int lvl = 0;
		for (int i=CurPos.y; !found && i<MaxDispLine; i++) {
			UnicodeString lbuf = get_DispLine(i);
			int p = (i==CurPos.y)? CurPos.x + 2 : 1;
			for (int j=p; !found && j<lbuf.Length(); j++) {
				if (lbuf[j]==k_ch) {
					if (lvl==0) {
						CurPos = Point(j - 1, i);
						found  = true;
					}
					else {
						lvl--;
					}
				}
				else if (lbuf[j]==b_ch) {
					lvl++;
				}
			}
		}
	}
	//括弧閉じ → 括弧開き
	else if (p_k>0) {
		WideChar b_ch = br_str[p_k];
		WideChar k_ch = bk_ch;
		int lvl = 0;
		for (int i=CurPos.y; !found && i>=0; i--) {
			UnicodeString lbuf = get_DispLine(i);
			int p = (i==CurPos.y)? CurPos.x : lbuf.Length();
			for (int j=p; !found && j>0; j--) {
				if (lbuf[j]==b_ch) {
					if (lvl==0) {
						CurPos = Point(j - 1, i);
						found = true;
					}
					else {
						lvl--;
					}
				}
				else if (lbuf[j]==k_ch) {
					lvl++;
				}
			}
		}
	}

	//HTML ブロック
	if (!found && test_HtmlExt(get_extension(FileName)) && !isHtm2Txt) {
		int p_s, p_e;
		UnicodeString tag_str =
			GetCurWord(false,
				"</?(head|body|div|header|footer|nav|main|section|article|aside|table|ul|ol|form|script).*?>",
				&p_s, &p_e);

		if (!tag_str.IsEmpty()) {
			//開始 → 終了タグ
			if (tag_str[2]!='/') {
				UnicodeString b_str = get_tkn(get_tkn(tag_str, ">"), " ");
				UnicodeString e_str = b_str + ">";	e_str.Insert("/", 2);
				int lvl = 0;
				for (int i=CurPos.y; !found && i<MaxDispLine; i++) {
					UnicodeString lbuf = get_DispLine(i);
					int p = (i==CurPos.y)? p_e : 1;
					for (int j=p; !found && j<lbuf.Length(); j++) {
						UnicodeString s = lbuf.SubString(j, lbuf.Length() - j + 1);
						if (!has_CR(lbuf) && StartsStr("<", s) && !s.Pos(">") && i<(MaxDispLine - 1)) {
							UnicodeString tmp = get_tkn(get_DispLine(i + 1), "<");
							int q = tmp.Pos(">");
							s += (q>0)? tmp.SubString(1, q) : get_tkn(tmp, " ");
						}

						if (StartsText(e_str, s)) {
							if (lvl==0) {
								CurPos = nrm_Pos(Point(j - 1, i));
								found  = true;
							}
							else {
								lvl--;
							}
						}
						else if (StartsText(b_str, s)) {
							lvl++;
						}
					}
				}
			}
			//終了 → 開始タグ
			else {
				UnicodeString e_str = tag_str;
				UnicodeString b_str = get_tkn(tag_str, ">");	b_str.Delete(2, 1);
				int lvl = 0;
				for (int i=CurPos.y; !found && i>=0; i--) {
					UnicodeString lbuf = get_DispLine(i);
					int p = (i==CurPos.y)? p_s : lbuf.Length();
					if (i>0) {
						UnicodeString tmp = get_DispLine(i - 1);
						if (!has_CR(tmp)) {
							lbuf.Insert(tmp, 1);
							i--;
							p += tmp.Length();
						}
					}

					for (int j=p; !found && j>0; j--) {
						UnicodeString s = lbuf.SubString(j, lbuf.Length() - j + 1);
						if (StartsText(b_str, s)) {
							if (lvl==0) {
								CurPos = nrm_Pos(Point(j - 1, i));
								found = true;
							}
							else {
								lvl--;
							}
						}
						else if (StartsText(e_str, s)) {
							lvl++;
						}
					}
				}
			}

			if (!found) beep_Warn();
		}
	}

	//行対応(パラメータ指定)
	if (!found && !prm.IsEmpty()) {
		int p = prm.Pos("/;/");
		if (p>2) {
			UnicodeString bgn_ptn = prm.SubString(1, p);
			p += 2;
			UnicodeString end_ptn = prm.SubString(p, prm.Length() - p + 1);
			if (is_regex_slash(bgn_ptn) && is_regex_slash(end_ptn)) {
				found = SearchPairCore(exclude_top_end(bgn_ptn), exclude_top_end(end_ptn));
			}
			else {
				GlobalErrMsg = LoadUsrMsg(USTR_IllegalParam);
				return false;
			}
		}
	}

	//行対応(デフォルト/ユーザ定義)
	if (!found && PairPtnList->Count>0) {
		for (int i=0; i<PairPtnList->Count && !found; i++) {
			UnicodeString ptn = PairPtnList->Strings[i];
			found = SearchPairCore(get_pre_tab(ptn), get_post_tab(ptn));
		}
	}

	if (found) {
		CurHchX = cv_PosX_to_HchX(CurPos.x);
		if (isSelMode) SelEnd = CurPos;
		if (CurPos.y<CurTop || CurPos.y>=CurBottom-1) {
			UpdatePos(true);
		}
		else {
			Repaint();
			SetSttInf();
		}
	}

	return found;
}

//---------------------------------------------------------------------------
//指定行番号に移動
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::ToLine(
	int lno, 	//行番号
	int col)	//列番号	(固定長表示	default = -1)
{
	if (lno<=0) return false;

	bool jumped = false;
	for (int i=0; i<MaxDispLine && !jumped; i++) {
		line_rec *lp = get_LineRec(i);
		if (lp->LineIdx>0 || lno!=lp->LineNo) continue;
		CurPos  = Point(0, i);
		CurHchX = cv_PosX_to_HchX(CurPos.x);
		if (isSelMode) SelEnd = CurPos;
		UpdatePos(true);
		set_PosFromCol(col);
		jumped = true;
	}

	return jumped;
}
//---------------------------------------------------------------------------
//TopAddress からの相対アドレスに移動
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::ToAddrR(unsigned int adr)
{
	if (!isBinary) return false;

	bool found = false;
	int idx = adr/16;
	if (adr<BinarySize && idx<MaxDispLine) {
		int x = (adr%16) * 3;	if (x>21) x++;
		CurPos	= Point(x, idx);
		CurHchX = cv_PosX_to_HchX(CurPos.x);
		UpdatePos(true);
		SetSttInf();
		found = true;
	}
	else if (BinarySize==0 && adr==0) {
		TextTop();
		found = true;
	}
	return found;
}
//---------------------------------------------------------------------------
//絶対アドレスに移動
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::ToAddrA(__int64 adr)
{
	bool found = false;
	__int64 adr_r = adr - TopAddress;
	if (adr_r>=0 && adr_r<BinarySize) {
		found = ToAddrR(adr_r);
	}
	else if (adr<BinFileSize) {
		int ofs = adr%ViewBinLimitSize;
		adr -= ofs;
		found = AssignBin(adr, true, ofs);
	}
	return found;
}

//---------------------------------------------------------------------------
void __fastcall TTxtViewer::JumpLine(UnicodeString ln_str)
{
	try {
		bool jumped = false;
		UnicodeString msg;
		//バイナリ
		if (isBinary) {
			if (ln_str.IsEmpty()) {
				InputExDlg->IpuntExMode = INPEX_JUMP_ADDR;
				InputExDlg->InputEdit->EditLabel->Caption
					= UnicodeString().sprintf(_T("0～%s (%llu)"), get_AddrStr(BinFileSize - 1).c_str(), BinFileSize - 1);
				ln_str = (InputExDlg->ShowModal()==mrOk)? InputExDlg->InputEdit->Text : EmptyStr;
			}

			remove_text(ln_str, _T(":"));
			if (ln_str.IsEmpty()) throw EAbort(EmptyStr);	//キャンセル

			bool is_rel = starts_tchs(_T("+-"), ln_str);	//相対指定か?
			int rel_sig = 0;
			if (is_rel) {
				rel_sig = (ln_str[1]=='-')? -1 : 1;
				ln_str.Delete(1, 1);
			}
			__int64 inp_adr = StrToInt64Def(ln_str, -1);
			if (inp_adr==-1) UserAbort(USTR_IllegalAddress);
			if (is_rel) inp_adr = get_CurAddrA() + rel_sig * inp_adr;
			jumped = ToAddrA(inp_adr);
		}
		//テキスト
		else {
			InputExDlg->IpuntExMode = INPEX_JUMP_LINE;
			InputExDlg->InputEdit->EditLabel->Caption = msg.sprintf(_T("行番号(1～%u)"), MaxLine);
			ln_str = (InputExDlg->ShowModal()==mrOk)? InputExDlg->InputEdit->Text : EmptyStr;
			if (ln_str.IsEmpty()) throw EAbort(EmptyStr);	//キャンセル
			if (contains_wd_i(ln_str, _T("+|="))) throw EAbort(EmptyStr);

			int inp_n = ln_str.ToIntDef(-1);
			if (inp_n==-1)				   UserAbort(USTR_IllegalParam);
			if (inp_n<=0 || inp_n>MaxLine) SysErrAbort(DISP_E_OVERFLOW);

			for (int i=0; i<MaxDispLine && !jumped; i++) {
				if (inp_n != get_LineRec(i)->LineNo) continue;
				CurPos	= Point(0, i);	set_PosFromCol(CsvCol);
				CurHchX = cv_PosX_to_HchX(CurPos.x);
				if (isSelMode) SelEnd = CurPos;
				UpdatePos(true);
				jumped = true;
			}
		}

		if (!jumped) throw EAbort(LoadUsrMsg(USTR_NotFound, _T("移動先")));
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
	}
}

//---------------------------------------------------------------------------
//マーク (トグル)
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::MarkLine(int lno)
{
	//設定
	if (!IsMarked(lno)) {
		MarkListStr.cat_sprintf(_T("%u;"), lno);
	}
	//解除
	else if (!MarkListStr.IsEmpty()) {
		UnicodeString s0, s1;
		MarkListStr = ReplaceStr(s0.sprintf(_T(";%s"), MarkListStr.c_str()), s1.sprintf(_T(";%u;"), lno), ";");
		remove_top_s(MarkListStr, ';');
	}
	Repaint(true);
}
//---------------------------------------------------------------------------
//マークされているか？
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::IsMarked(
	int lno)	//行番号 (default = 0: 現在行)
{
	if (!isReady) return false;

	if (lno==0) lno = get_CurLineNo();
	UnicodeString s0, s1;
	return ContainsText(s0.sprintf(_T(";%s"), MarkListStr.c_str()), s1.sprintf(_T(";%u;"), lno));
}

//---------------------------------------------------------------------------
//閲覧履歴に追加
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::add_ViewHistory()
{
	if (FileName.IsEmpty() || ContainsText(FileName, TempPathA)) return;
	if (match_path_list(FileName, NoViewHistPath)) return;

	for (;;) {
		int idx = indexof_csv_list(TextViewHistory, FileName, 0);
		if (idx!=-1) TextViewHistory->Delete(idx); else break;
	}

	UnicodeString lbuf;
	lbuf.sprintf(_T("%s,%u,"), add_quot_if_spc(FileName).c_str(), get_CurLineNo());
	lbuf += MarkListStr;
	TextViewHistory->Insert(0, lbuf);
}

//---------------------------------------------------------------------------
//ダンプリストを取得
//選択中ならその範囲、そうでない場合全体
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::GetDumpList(TStringList *lst)
{
	cursor_HourGlass();

	TPoint sp0, sp1;
	int ofs0, ofs1;
	//選択範囲
	if (SelStart!=SelEnd) {
		sp0 = SelStart;
		sp1 = SelEnd;
		bool flag = (sp0.y==sp1.y)? (sp0.x>sp1.x) : (sp0.y>sp1.y);
		if (flag) std::swap(sp0, sp1);
		if (sp1.x==0) { sp1.y--;  sp1.x = 48; }
		ofs0 = get_OfsR(sp0.x);
		ofs1 = (sp1.x<48 || (sp1.x>=50 && sp1.x<66))? get_OfsR(sp1.x) - 1 : 15;
	}
	//全体
	else {
		sp0 = Point(0, 0);
		sp1 = Point(48, MaxDispLine - 1);
		ofs0 = 0;
		ofs1 = 15;
	}

	int adr_len  = get_AddrStr(TopAddress + sp1.y*16).Length();
	UnicodeString hdstr;
	hdstr.sprintf(_T("%-*s"), adr_len + 2, _T("ADDRESS"));
	hdstr += "+0 +1 +2 +3 +4 +5 +6 +7  +8 +9 +A +B +C +D +E +F  0123456789ABCDEF";
	lst->Add(hdstr);
	lst->Add(StringOfChar(_T('-'), adr_len + 68));

	for (int i=sp0.y; i<=sp1.y; i++) {
		UnicodeString lbuf = get_DispLine(i, (i==sp0.y)? ofs0 : 0,  (i==sp1.y)? ofs1 : 15);
		if (lbuf.IsEmpty()) break;
		lbuf.Insert(get_AddrStr(TopAddress + i*16, adr_len) + "  ", 1);
		lst->Add(lbuf);
	}
	cursor_Default();
}

//---------------------------------------------------------------------------
//インクリメンタルサーチ
//---------------------------------------------------------------------------
void __fastcall TTxtViewer::IncSearch(UnicodeString keystr)
{
	UnicodeString CmdStr = KeyFuncList->Values["S:" + keystr];
	bool chg_wd = false;

	//サーチモードから抜ける
	if (USAME_TI(CmdStr, "IncSearchExit") || equal_ESC(keystr)) {
		isIncSea   = false;
		IncSeaWord = EmptyStr;
		UserModule->SetBlinkTimer(NULL);
	}
	//キーワードをクリア
	else if (USAME_TI(CmdStr, "ClearIncKeyword")) {
		IncSeaWord = EmptyStr;
		chg_wd = true;
	}
	//Migemoモード切替
	else if (USAME_TI(CmdStr, "MigemoMode")) {
		isIncMigemo = (!isIncMigemo && usr_Migemo->DictReady);
		IncSeaWord	= EmptyStr;
		chg_wd = true;
	}
	else if (USAME_TI(CmdStr, "NormalMode")) {
		isIncMigemo = false;
		IncSeaWord	= EmptyStr;
		chg_wd = true;
	}
	//キーワード更新
	else {
		chg_wd = update_IncSeaWord(IncSeaWord, keystr);
	}

	RegExPtn = usr_Migemo->GetRegExPtn(isIncMigemo, IncSeaWord);

	bool found = false;
	if (isIncSea && !RegExPtn.IsEmpty()) {
		bool csr_up   = (USAME_TI(CmdStr, "IncSearchUp")   || equal_UP(keystr));
		bool csr_down = (contained_wd_i(_T("IncSearchDown|IncSearchTop"), CmdStr) || equal_DOWN(keystr));
		if (USAME_TI(CmdStr, "IncSearchTop")) TextTop();

		//下方向へサーチ
		if (chg_wd || csr_down) {
			if (chg_wd) Repaint(true);
			found = SearchDown((isIncMigemo? RegExPtn : IncSeaWord), IncSeaCaseSens, isIncMigemo, false, chg_wd);
			//通常モードで見つからない場合、警告して一文字後退
			if (!found && chg_wd && !isIncMigemo) {
				SttHeader->Tag = SHOW_WARN_TAG;
				SetSttInf();
				beep_Warn();  Sleep(250);
				SttHeader->Tag = 0;
				delete_end(IncSeaWord);
			}
		}
		//上方向へサーチ
		else if (csr_up) {
			found = SearchUp((isIncMigemo? RegExPtn : IncSeaWord), IncSeaCaseSens, isIncMigemo);
		}

		if (found)
			FoundPos = CurPos;
		else if (!chg_wd)
			found = (FoundPos==CurPos);
	}

	if (!found) {
		IniSelected();
		Repaint(true);
		FoundPos = Point(-1, -1);
		ScrPanel->HitLines->Clear();
		ScrPanel->Repaint();
	}

	SetSttInf();
}

//---------------------------------------------------------------------------
//標準キー操作に対応するコマンドを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TTxtViewer::GetStdKeyCommand(UnicodeString keystr)
{
	bool is_sel = remove_top_text(keystr, "Shift+");
	UnicodeString ret_str =
		SameText(keystr, "DOWN") ? "CursorDown" :
		SameText(keystr, "UP")	 ? "CursorUp" :
		SameText(keystr, "LEFT") ? "CursorLeft" :
		SameText(keystr, "RIGHT")? "CursorRight" :
		SameText(keystr, "PGDN") ? "PageDown" :
		SameText(keystr, "PGUP") ? "PageUp" :
		SameText(keystr, "HOME") ? "TextTop" :
		SameText(keystr, "END")  ? "TextEnd" : "";

	if (!ret_str.IsEmpty() && is_sel) ret_str += "Sel";
	return ret_str;
}

//---------------------------------------------------------------------------
//共通コマンドのインデックスを取得
//---------------------------------------------------------------------------
int __fastcall TTxtViewer::GetComCmdIndex(UnicodeString cmd)
{
	return idx_of_word_i(
		_T("CursorDown|CursorDownSel|CursorUp|CursorUpSel|PageDown|PageDownSel|PageUp|PageUpSel|CursorLeft|CursorLeftSel|")	  // 0～ 9
		_T("CursorRight|CursorRightSel|LineTop|LineTopSel|LineEnd|LineEndSel|TextTop|TextTopSel|TextEnd|TextEndSel|")		  //10～19
		_T("WordLeft|WordRight|ScrollUp|ScrollDown|ScrollCursorUp|ScrollCursorDown|SearchPair|SelCurWord|SelLine|SelLineCR|") //20～29
		_T("SelectAll|JumpLine|SelectMode|FindSelDown|FindSelUp"), cmd);													  //30～34
}

//---------------------------------------------------------------------------
//コマンドを実行
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::ExeCommand(const _TCHAR *t_cmd, UnicodeString prm)
{
	bool handled = true;
	GlobalErrMsg = EmptyStr;

	UnicodeString cmd = t_cmd;

	if (prm.IsEmpty()) {
		prm = exclude_quot(get_PrmStr(cmd));
		cmd = get_CmdStr(cmd);
	}
	prm = extract_ExeParam(prm);
	if (!IsCmdAvailable(cmd)) return false;

	AddCmdHistory(cmd, prm, "V");
		 isClip? UnicodeString("<クリップボード>") :
		  isLog? UnicodeString("<タスクログ>") : FileName;

	int cur_lno = get_CurLineNo();

	//-------------------------------
	//共通のコマンド
	//-------------------------------
	int idx = GetComCmdIndex(cmd);
	if (idx!=-1) {
		switch (idx) {
		case  0: CursorDown(isSelMode, prm);	break;
		case  1: CursorDown(true);				break;
		case  2: CursorUp(isSelMode, prm);		break;
		case  3: CursorUp(true);				break;
		case  4: MovePage(true,  isSelMode);	break;
		case  5: MovePage(true,  true);			break;
		case  6: MovePage(false, isSelMode);	break;
		case  7: MovePage(false, true);			break;
		case  8: CursorLeft(isSelMode);			break;
		case  9: CursorLeft(true);				break;
		case 10: CursorRight(isSelMode);		break;
		case 11: CursorRight(true);				break;
		case 12: LineTop(isSelMode);			break;
		case 13: LineTop(true);					break;
		case 14: LineEnd(isSelMode);			break;
		case 15: LineEnd(true);					break;
		case 16: TextTop(isSelMode);			break;
		case 17: TextTop(true);					break;
		case 18: TextEnd(isSelMode);			break;
		case 19: TextEnd(true);					break;
		case 20: WordLeft(isSelMode);			break;
		case 21: WordRight(isSelMode);			break;
		case 22: MoveScroll(false, prm);		break;
		case 23: MoveScroll(true,  prm);		break;
		case 24: MoveScroll(false, prm, true);	break;
		case 25: MoveScroll(true,  prm, true);	break;
		case 26: SearchPair(prm);				break;
		case 27: GetCurWord(true);				break;
		case 28: SelLine();						break;
		case 29: SelLine(true);					break;
		case 30: SelectAll();					break;
		case 31: JumpLine(prm);					break;
		case 32: isSelMode = !isSelMode;		break;
		case 33: SearchSel(false, USAME_TI(prm, "EM"));	break;
		case 34: SearchSel(true,  USAME_TI(prm, "EM"));	break;
		}
	}
	//配色の設定
	else if (USAME_TI(cmd, "SetColor")) {
		if (!prm.IsEmpty()) {
			SetColor(prm);
		}
		else {
			if (!ColorDlg) ColorDlg = new TColorDlg(Application->MainForm);	//初回に動的作成
			UnicodeString col_fnam = to_absolute_name(UserHighlight->ReadKeyStr(_T("ColorIniFile")));
			if (isExtWnd) {
				TExTxtViewer *xtv = dynamic_cast<TExTxtViewer *>(OwnerForm);
				if (xtv) {
					ColorDlg->ObjViewer = xtv->ExViewer;
					ColorDlg->FileName  = col_fnam;
					show_ModalDlg(ColorDlg);
				}
			}
			else {
				ColorDlg->FileName = col_fnam;
				show_ModalDlg(ColorDlg);
			}
		}
		Repaint(true);
	}
	//ルーラを表示
	else if (USAME_TI(cmd, "ShowRuler")) {
		if (RulerBox) {
			RulerBox->Visible = SetToggleSw(ShowTextRuler, prm);
			Application->ProcessMessages();
			SetMetric(true);
		}
	}
	//行番号を表示
	else if (USAME_TI(cmd, "ShowLineNo")) {
		SetToggleSw(ShowLineNo, prm);
		SetMetric(true);
	}
	//タブを表示
	else if (USAME_TI(cmd, "ShowTAB")) {
		SetToggleSw(ShowTAB, prm);
		Repaint(true);
	}
	//改行を表示
	else if (USAME_TI(cmd, "ShowCR")) {
		SetToggleSw(ShowCR, prm);
		Repaint(true);
	}
	//ズーム
	else if (contained_wd_i(_T("ZoomIn|ZoomOut"), cmd)) {
		int d_sz = std::min(prm.ToIntDef(1), 12);
		ViewBox->Font->Size =
			USAME_TI(cmd, "ZoomIn") ? std::min(ViewBox->Font->Size + d_sz, MAX_FNTZOOM_SZ)
									: std::max(ViewBox->Font->Size - d_sz, MIN_FNTZOOM_SZ);
		SetMetric(true);
	}
	else if (USAME_TI(cmd, "ZoomReset")) {
		ViewBox->Font->Size = useFontSize;
		SetMetric(true);
	}
	//フォントサイズ変更
	else if (USAME_TI(cmd, "SetFontSize")) {
		bool x_sw = remove_top_s(prm, '^');
		if (!prm.IsEmpty()) {
			int f_sz = prm.ToIntDef(ViewBox->Font->Size);
			f_sz = std::max(std::min(f_sz, MAX_FNTZOOM_SZ), MIN_FNTZOOM_SZ);
			ViewBox->Font->Size = (x_sw && ViewBox->Font->Size==f_sz)? useFontSize : f_sz;
			SetMetric(true);
		}
	}
	else if (USAME_TI(cmd, "BoxSelMode")) {
		isBoxMode = !isBoxMode;
		isSelMode = isBoxMode;
	}
	else if (USAME_TI(cmd, "Highlight")) {
		SetToggleSw(Highlight, prm);
		Repaint(true);
	}
	//再検索
	else if (contained_wd_i(_T("FindDown|FindUp"), cmd)) {
		if (!prm.IsEmpty()) {
			UnicodeString ptn;
			if (is_regex_slash(prm)) {
				isRegEx = true;
				ptn = exclude_top_end(prm);
			}
			else if (isMigemo) {
				ptn = usr_Migemo->Query(prm);
			}
			else {
				ptn = TRegEx::Escape(prm);
			}
			RegExPtn = chk_RegExPtn(ptn)? ptn : EmptyStr;
			FindWord = prm;
			Repaint(true);
		}

		if (USAME_TI(cmd, "FindDown"))
			SearchDown((isRegEx||isMigemo)? RegExPtn : FindWord, isCase, isRegEx||isMigemo, isBytes);
		else
			SearchUp((isRegEx||isMigemo)? RegExPtn : FindWord, isCase, isRegEx||isMigemo, isBytes);
	}
	//マーク
	else if (USAME_TI(cmd, "Mark")) {
		MarkLine(cur_lno);
	}
	else if (USAME_TI(cmd, "ClearMark")) {
		MarkListStr = EmptyStr;
		Repaint(true);
	}
	//マークを検索
	else if (USAME_TI(cmd, "FindMarkDown")) {
		TStringDynArray m_lst = split_strings_semicolon(MarkListStr);
		int f_lno = 0;
		for (int i=0; i<m_lst.Length; i++) {
			int lno = m_lst[i].ToIntDef(0);  if (lno==0) continue;
			if (lno>cur_lno) {
				if		(f_lno==0)  f_lno = lno;
				else if (lno<f_lno) f_lno = lno;
			}
		}
		LastFound = (f_lno>0);
		if (LastFound) {
			if (isBinary) ToAddrA((__int64)(f_lno - 1)*16); else ToLine(f_lno);
		}
		else if (!XCMD_IsBusy) beep_Warn();
	}
	else if (USAME_TI(cmd, "FindMarkUp")) {
		TStringDynArray m_lst = split_strings_semicolon(MarkListStr);
		int f_lno = 0;
		for (int i=0; i<m_lst.Length; i++) {
			int lno = m_lst[i].ToIntDef(0);  if (lno==0) continue;
			if (lno<cur_lno) {
				if		(f_lno==0)  f_lno = lno;
				else if (lno>f_lno) f_lno = lno;
			}
		}
		LastFound = (f_lno>0);
		if (LastFound) {
			if (isBinary) ToAddrA((__int64)(f_lno - 1)*16); else ToLine(f_lno);
		}
		else if (!XCMD_IsBusy) beep_Warn();
	}
	//-------------------------------
	//テキスト表示のみのコマンド
	//-------------------------------
	else if (isText) {
		//HTML→テキスト変換
		if (USAME_TI(cmd, "HtmlToText")) {
			SetHtmlToText(prm);
			if (isExtWnd) TxtViewer->isHtm2Txt = isHtm2Txt;		//内部ビュアーに反映
			AssignText();
		}
		//CSV/TSVを固定長表示
		else if (USAME_TI(cmd, "FixedLen")) {
			int lmt = extract_int_def(prm);
			if (lmt>0) {
				ViewFixedLimit = std::max(lmt, 4);
				prm = "ON";
			}
			SetToggleSw(isFixedLen, prm);
			if (isExtWnd) TxtViewer->isFixedLen = isFixedLen;	//内部ビュアーに反映
			AssignText(NULL, cur_lno);
		}
		//ルビを表示
		else if (USAME_TI(cmd, "ShowRuby")) {
			SetToggleSw(ShowRuby, prm);
			AssignText();
		}
		//ソート
		else if (USAME_TI(cmd, "Sort")) {
			int xp = CurPos.x;
			int yi = get_LineRec(CurPos.y)->LineIdx;
			AssignText(NULL, cur_lno, USAME_TI(prm, "AO")? 1 : USAME_TI(prm, "DO")? -1 : 0);
			CurPos.x = xp;
			CurPos.y += yi;
			CurHchX  = cv_PosX_to_HchX(CurPos.x);
			UpdatePos();
		}
		//リンク先を検索
		else if (USAME_TI(cmd, "FindLinkDown")) {
			SearchDown(LINK_MATCH_PTN, false, true);
		}
		else if (USAME_TI(cmd, "FindLinkUp")) {
			SearchUp(LINK_MATCH_PTN, false, true);
		}
		else if (USAME_TI(cmd, "SetUserDefStr")) {
			FuncListDlg->UserDefStr = prm;
		}
		//URLを開く
		else if (USAME_TI(cmd, "OpenURL")) {
			UnicodeString url;
			if (!prm.IsEmpty()) {
				try {
					url = TIdURI::URLEncode(prm, IndyTextEncoding_UTF8());
				}
				catch (Exception &e) {
					url = EmptyStr;
				}
			}
			else {
				url = get_CurUrl();
			}
			if (!url.IsEmpty()) Execute_ex(url); else GlobalErrMsg = LoadUsrMsg(USTR_NotFound, _T("URL"));
		}
		//Web で検索
		else if (USAME_TI(cmd, "WebSearch")) {
			UnicodeString kwd = USAME_TI(prm, "CB")? GetClipboardText(true) :
													 get_tkn(def_if_empty(prm, get_SelText()), _T("\r\n"));
			if (kwd.IsEmpty()) kwd = inputbox_dir(get_WebSeaCaption().c_str(), _T("WebSearch"));
			if (!kwd.IsEmpty() && !exe_WebSearch(kwd)) GlobalErrMsg = LoadUsrMsg(USTR_FaildProc);
		}
		//カーソル位置の単語のヘルプ
		else if (USAME_TI(cmd, "HelpCurWord")) {
			HtmlHelpKeyword(prm, GetCurWord(false));
		}
		//文字情報を表示
		else if (USAME_TI(cmd, "CharInfo")) {
			bool sw = CharInfoForm->Visible;
			CharInfoForm->Visible = SetToggleSw(sw, prm);
		}
		//CSV/TSV項目の集計
		else if (USAME_TI(cmd, "CsvCalc")) {
			FileInfoDlg->isCalcItem  = true;
			FileInfoDlg->DataList	 = TxtBufList;
			FileInfoDlg->TopIsHeader = TopIsHeader;
			FileInfoDlg->CsvCol 	 = CsvCol;
			FileInfoDlg->ShowModal();
		}
		//CSV/TSV項目のグラフ
		else if (USAME_TI(cmd, "CsvGraph")) {
			GraphForm->DataList    = TxtBufList;
			GraphForm->TopIsHeader = TopIsHeader;
			GraphForm->CsvCol	   = CsvCol;
			GraphForm->ShowModal();
		}
		//CSV/TSVレコード表示
		else if (USAME_TI(cmd, "CsvRecord")) {
			bool sw = CsvRecForm->Visible;
			CsvRecForm->Visible = SetToggleSw(sw, prm);
			if (CsvRecForm->Visible && get_LineRec(CurPos.y)->LineIdx>0) ToLine(get_CurLineNo());
		}
		//イメージプレビュー
		else if (USAME_TI(cmd, "ImgPreview")) {
			UnicodeString fnam = GetCurImgFile();
			if (!SubViewer->Visible && !fnam.IsEmpty()) {
				SubViewer->Show();
				SubViewer->DrawImage(fnam);
				Application->MainForm->SetFocus();
			}
			else {
				if (fnam.IsEmpty() || SameText(SubViewer->FileName, fnam))
					SubViewer->Close();
				else
					SubViewer->DrawImage(fnam);
			}
		}
		//エラー検索
		else if (contained_wd_i(_T("NextErr|PrevErr"), cmd)) {
			if (isLog) {
				UnicodeString ptn = "^.>([ECW]|(     [45]\\d{2})) .*";
				if (USAME_TI(cmd, "NextErr")) SearchDown(ptn, true, true); else SearchUp(ptn, true, true);
			}
		}
		//無効
		else if (contained_wd_i(_T("BitmapView|Inspector"), cmd)) {
			GlobalErrMsg = LoadUsrMsg(USTR_InvalidCmd);
		}
		else handled = false;
	}
	//-------------------------------
	//バイナリのみのコマンド
	//-------------------------------
	else if (isBinary) {
		//ビットマップビュー
		if (USAME_TI(cmd, "BitmapView")) {
			bool sw = BitmapForm->Visible;
			BitmapForm->Visible = SetToggleSw(sw, prm);
		}
		//インスペクタ
		else if (USAME_TI(cmd, "Inspector")) {
			bool sw = InspectForm->Visible;
			InspectForm->Visible = SetToggleSw(sw, prm);
		}
		//無効
		else if (contained_wd_i(_T("HtmlToText|ShowRuby|FindLinkDown|FindLinkUp|OpenURL|CharInfo"), cmd)) {
			GlobalErrMsg = LoadUsrMsg(USTR_InvalidCmd);
		}
		else handled = false;
	}
	else handled = false;

	SetSttInf();
	return handled;
}

//---------------------------------------------------------------------------
//テキストビュアーで利用可能なコマンドか?
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::IsCmdAvailable(UnicodeString cmd)
{
	cmd = get_CmdStr(cmd);

	//別ウィンドウ固有の判定
	if (isExtWnd) {
		//有効
		if (contained_wd_i(
			_T("AppList|BackViewHist|BinaryEdit|Calculator|ChangeCodePage|ChangeViewMode|ClipCopy|Close|")
			_T("FileEdit|FindText|IncSearch|NextFile|OptionDlg|PrevFile|RegExChecker|ReloadFile|SaveDump|")
			_T("SwitchSameName|SwitchSrcHdr|TagJump|TagJumpDirect|TagView|TagViewDirect"), cmd))
		{
			return true;
		}
		//無効
		if (contained_wd_i(
			_T("BitmapView|CharInfo|CsvCalc|CsvGraph|CsvRecord|Inspector|ImgPreview||")
			_T("ListFileInfo|ShowFileInfo "), cmd))
		{
			return false;
		}
	}

	//共通コマンド
	if (GetComCmdIndex(cmd)!=-1) return true;

	if (contained_wd_i(
		_T("BoxSelMode|ClearMark|FindDown|FindUp|FindMarkDown|FindMarkUp|Highlight|Mark|SetColor|SetFontSize|")
		_T("ShowCR|ShowLineNo|ShowRuler|ShowTAB|ZoomIn|ZoomOut|ZoomReset"), cmd))
	{
		return true;
	}

	//テキスト表示のみのコマンド
	if (isText) {
		return(contained_wd_i(
			_T("CharInfo|CsvCalc|CsvGraph|CsvRecord|FindLinkDown|FindLinkUp|FixedLen|HelpCurWord|HtmlToText|ImgPreview|")
			_T("OpenURL|SetUserDefStr|ShowRuby|Sort|WebSearch"), cmd));
	}

	//バイナリのみのコマンド
	if (isBinary) {
		return(contained_wd_i(_T("BitmapView|Inspector"), cmd));
	}

	return false;
}

//---------------------------------------------------------------------------
//モード固有の禁止コマンドか？
//---------------------------------------------------------------------------
bool __fastcall TTxtViewer::IsCmdInhibited(UnicodeString cmd)
{
	cmd = get_CmdStr(cmd);

	if (isBinary) {
		return(contained_wd_i(
			_T("ChangeCodePage|CharInfo|CsvCalc|CsvGraph|CsvRecord|FindLinkDown|FindLinkUp|FixedLen|")
			_T("HelpCurWord|HtmlToText|ImgPreview|OpenURL|SetUserDefStr|ShowRuby|Sort|WebSearch|")
			_T("FunctionList|UserDefList|MarkList|TagJump|TagJumpDirect|TagView|TagViewDirect"), cmd));
	}

	if (isText) {
		return(contained_wd_i(_T("BitmapView|Inspector|SaveDump"), cmd));
	}

	return false;
}
//---------------------------------------------------------------------------

