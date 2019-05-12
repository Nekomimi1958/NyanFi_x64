//----------------------------------------------------------------------//
// NyanFi																//
//  オプション設定														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <mmsystem.h>
#include <Vcl.FileCtrl.hpp>
#include <RegularExpressions.hpp>
#include "usr_shell.h"
#include "usr_xd2tx.h"
#include "usr_arc.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "usr_tag.h"
#include "Global.h"
#include "InpExDlg.h"
#include "CmdListDlg.h"
#include "OptDlg.h"
#include "FindKey.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TOptionDlg *OptionDlg = NULL;

TStringList *FKeyLabelBuf;

//---------------------------------------------------------------------------
//Fキー表示名ソート用の比較関数
//---------------------------------------------------------------------------
int __fastcall KeyComp_FKey(TStringList *List, int Index1, int Index2)
{
	UnicodeString key0 = List->Names[Index1];
	UnicodeString key1 = List->Names[Index2];

	UnicodeString ptn = ":(Shift\\+|Ctrl\\+|Alt\\+)*F\\d+";
	TRegExOptions opt; opt << roIgnoreCase;
	UnicodeString lvl0 = TRegEx::IsMatch(key0, ptn, opt)? FKeyLabelBuf->Values[key0] : EmptyStr;
	UnicodeString lvl1 = TRegEx::IsMatch(key1, ptn, opt)? FKeyLabelBuf->Values[key1] : EmptyStr;

	if (lvl0.IsEmpty() && lvl1.IsEmpty()) return KeyComp_Key(List, Index1, Index2);
	if (lvl0.IsEmpty()) return 1;
	if (lvl1.IsEmpty()) return -1;
	return CompareText(lvl0, lvl1);
}

//---------------------------------------------------------------------------
// TOptionDlg クラス
//---------------------------------------------------------------------------
__fastcall TOptionDlg::TOptionDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormCreate(TObject *Sender)
{
	cursor_HourGlass();

	//入力欄にポップアップメニューを設定
	UserModule->SetUsrPopupMenu(this);

	FindMarkList = new MarkList(this);
	ColBufList	 = new TStringList();

	SwatchPanel = new UsrSwatchPanel(this);
	SwatchPanel->Parent = this;

	set_ButtonMark(FindDownBtn,		UBMK_BDOWN);
	set_ButtonMark(FindUpBtn,		UBMK_BUP);
	set_ButtonMark(FindDownColBtn,	UBMK_BDOWN);
	set_ButtonMark(FindUpColBtn,	UBMK_BUP);
	set_ButtonMark(FindDownEvBtn,	UBMK_BDOWN);
	set_ButtonMark(FindUpEvBtn,		UBMK_BUP);

	for (int i=0; i<MAX_KEYTABS; i++) KeyListBuf[i] = new TStringList();
	FKeyLabelBuf = new TStringList();

	set_ComboBoxText(LayoutComboBox,
		_T("ファイルリストが上\nファイルリストが左\nファイルリストが右\n"));

	set_ComboBoxText(SizeFmtComboBox,
		_T("GB/MB/バイト\nGB/MB/KB/バイト\nGB/MB/KB\n"));

	set_ComboBoxText(TabStyleComboBox,
		_T("タブ\nタブ(斜)\nボタン\nフラット\n"));

	set_ComboBoxText(ScrBarStyleComboBox,
		_T("標準\nシンプル\nシンプル(3/4幅)\nシンプル(1/2幅)\nシンプル(画像)\n"));


	FontBufList = new TStringList();
	for (int i=0; i<FontList->Count-1; i++) {
		TFont *f = new TFont();
		FontBufList->AddObject(FontList->Strings[i], (TObject *)f);
		FontComboBox->Items->Add(FontList->ValueFromIndex[i]);
	}

	set_ComboBoxText(SizeDecDgtComboBox, _T("0\n1\n2\n3\n"));
	set_ComboBoxText(MaxTasksComboBox,   _T("1\n2\n3\n4\n"));

	OptColListBox->Items->Text =
		"bgList=リストの背景色\n"
		"bgList2=リストの背景色(交互)\n"
		"fgList=リストの文字色\n"
		"bgArc=仮想ディレクトリの背景色\n"
		"bgFind=結果リストの背景色\n"
		"bgWork=ワークリストの背景色\n"
		"bgFTP=FTPリモート側の背景色\n"
		"bgADS=代替データストリーム一覧の背景色\n"
		"selItem=|選択項目の背景色\n"
		"oppItem=反対パス選択項目の背景色\n"
		"fgSelItem=選択項目の文字色\n"
		"bgMark=栞マーク項目の背景色\n"
		"matchItem=INC.サーチのマッチ項目背景色\n"
		"Differ=結果リストの相違箇所背景色\n"
		"DifferN=結果リストの注目箇所背景色\n"
		"Folder=|ディレクトリの文字色\n"
		"SymLink=シンボリックリンク/ジャンクションの文字色\n"
		"Protect=ディレクトリ(削除制限)の文字色\n"
		"ReadOnly=読み込み専用属性の文字色\n"
		"Hidden=隠し属性の文字色\n"
		"System=システム属性の文字色\n"
		"Compress=圧縮属性の文字色\n"
		"fgSpace=空白記号の文字色\n"
		"fgTagNam=デフォルトのタグ色\n"
		"InvItem=無効な項目の文字色\n"
		"Cursor=ラインカーソルの色\n"
		"bgScrBar=|シンプルスクロールバーの背景色\n"
		"bgScrKnob=シンプルスクロールノブの色\n"
		"frScrKnob=シンプルスクロールノブの輪郭色\n"
		"lnScrHit=シンプルスクロールバーのヒット行\n"
		"Splitter=分割境界線\n"
		"bgTlBar1=ツールバーのグラデーション開始色\n"
		"bgTlBar2=ツールバーのグラデーション終了色\n"
		"fgTlBar=ツールバーの文字色\n"
		"htTlBar=ツールバーのホットトラッキング色\n"
		"bgTabBar=|タブバーの背景色\n"
		"bgActTab=アクティブなタブの背景色\n"
		"bgInAcTab=非アクティブなタブの背景色\n"
		"frmTab=タブの輪郭線\n"
		"fgTab=タブの文字色\n"
		"bgListHdr=|リストヘッダの背景色\n"
		"fgListHdr=リストヘッダの文字色\n"
		"bgDirInf=ディレクトリ情報の背景色\n"
		"fgDirInf=ディレクトリ情報の文字色\n"
		"bgDirRel=ディレクトリ関係の背景色\n"
		"fgDirRel=ディレクトリ関係の文字色\n"
		"bgDrvInf=ドライブ情報の背景色\n"
		"fgDrvInf=ドライブ情報の文字色\n"
		"bgInf=|ファイル情報の背景色\n"
		"fgInf=ファイル情報の文字色\n"
		"fgInfNam=ファイル情報の項目名文字色\n"
		"fgInfEmp=ファイル情報の強調文字色\n"
		"bgTxtPrv=テキストプレビューの背景色\n"
		"fgTxtPrv=テキストプレビューの文字色\n"
		"bgLog=ログの背景色\n"
		"fgLog=ログの文字色\n"
		"bgTask=タスク状態表示の背景色\n"
		"fgPrgBar=タスク進捗バー色\n"
		"bgPrgBar=タスク進捗背景色\n"
		"Error=エラー/注意の文字色\n"
		"bgSttBar=|ステータスバーの背景色\n"
		"fgSttBar=ステータスバーの文字色\n"
		"bgInfHdr=情報ヘッダの背景色\n"
		"fgInfHdr=情報ヘッダの文字色\n"
		"bgView=|テキストビュアーの背景色\n"
		"fgView=テキストビュアーの文字色\n"
		"Margin=テキストビュアーの余白白\n"
		"bgRuler=ルーラの背景色\n"
		"fgRuler=ルーラの目盛色\n"
		"bgLineNo=行番号背景色\n"
		"LineNo=行番号文字色\n"
		"Mark=行マーク\n"
		"bdrLine=行番号境界線\n"
		"bdrFold=折り返し境界線\n"
		"bdrFixed=固定長表示の縦罫線\n"
		"Comment=|コメントの文字色\n"
		"Strings=文字列の文字色\n"
		"Reserved=予約語の文字色\n"
		"Symbol=シンボルの文字色\n"
		"Numeric=数値の文字色\n"
		"fgEmpBin1=バイナリ強調文字色1\n"
		"fgEmpBin2=バイナリ強調文字色2\n"
		"fgEmpBin3=バイナリ強調文字色3\n"
		"Headline=見出しの文字色\n"
		"Ruby=ルビ\n"
		"URL=URLの文字色\n"
		"LocalLink=ローカルファイルへのリンク\n"
		"fgEmp=強調文字色\n"
		"bgEmp=強調背景色\n"
		"TAB=タブ表示色\n"
		"CR=改行表示色\n"
		"HR=罫線の色\n"
		"Ctrl=コントロールコード\n"
		"bgImage=|画像の背景色\n"
		"bgDblPg=見開き表示の余白色\n"
		"bgWMF=メタファイルの背景色\n"
		"bdrThumb=サムネイルの境界線\n"
		"ThumbExif=サムネイルのExif情報\n"
		"ImgGrid=画像分割グリッド\n"
		"GrBack=|グラフの背景色\n"
		"GrLine=グラフのライン\n"
		"GrGrid=グラフのグリッド\n"
		"GrText=グラフの文字色\n"
		"GitHEAD=|Git: HEADマーク\n"
		"GitMark=Git: コミットマーク\n"
		"GitBra=Git: ブランチ\n"
		"GitBraR=Git: リモート参照\n"
		"GitTag=Git: タグ\n"
		"GitHash=Git: ハッシュ\n"
		"GitIns=Git: + 行\n"
		"GitDel=Git: - 行\n"
		"bgHint=|ヒント表示の背景色\n"
		"fgHint=ヒント表示の文字色\n"
		"bgWarn=警告表示の背景色\n"
		"Invalid=無効な項目の背景色\n"
		"Illegal=不正な入力項目の背景色\n"
		"bgOptTab=|アクティブな設定タブの背景色\n"
		"fgOptTab=アクティブな設定タブの文字色\n"
		"OptFind=オプション設定の検索結果\n"
		"TlBorder=|ツールウインドウの境界線\n";

	TimColListBox->Items->Text =
		"Tim1H=1時間以内\n"
		"Tim3H=3時間以内\n"
		"Tim1D=1日以内\n"
		"Tim3D=3日以内\n"
		"Tim7D=7日以内\n"
		"Tim1M=1ヶ月以内\n"
		"Tim3M=3ヶ月以内\n"
		"Tim6M=6ヶ月以内\n"
		"Tim1Y=1年以内\n"
		"Tim3Y=3年以内\n"
		"TimOld=それより前\n"
		"Size4G=|4GB以上\n"
		"Size1G=1GB以上\n"
		"Size1M=1MB以上\n"
		"Size1K=1KB以上\n"
		"SizeLT=1KB未満\n"
		"Size0=0\n";

	set_ComboBoxText(BgImgModeComboBox,
		_T("非表示\n")
		_T("2画面に渡って表示\n")
		_T("それぞれに表示\n")
		_T("デスクトップ背景を表示\n"));
	set_ComboBoxText(BgImgSubModeComboBox,
		_T("中央に表示\n")
		_T("並べて表示\n")
		_T("表示サイズにストレッチ\n")
		_T("表示サイズに合わせて切り出し\n"));

	set_ComboBoxText(ScaleOptComboBox,
		_T("ニアレストネイバー\nバイリニア\nバイキュービック\nファントリサンプリング\n補間しない\n"));

	//マウスホイール
	set_ComboBoxText(WheelCmdF0ComboBox,
		_T("AlphaBlend_+32/AlphaBlend_-32\n")
		_T("BackDirHist/ForwardDirHist\n")
		_T("BorderLeft/BorderRight\n")
		_T("CursorUp/CursorDown\n")
		_T("PageUp/PageDown\n")
		_T("PrevDrive/NextDrive\n")
		_T("PrevNyanFi/NextNyanFi\n")
		_T("PrevTab/NextTab\n")
		_T("ScrollUp/ScrollDown\n")
		_T("ScrollUpLog/ScrollDownLog\n")
		_T("ScrollUpText/ScrollDownText\n")
		_T("SetSubSize_-16/SetSubSize_+16\n")
		_T("ZoomIn/ZoomOut\n"));
	WheelCmdF1ComboBox->Items->Assign(WheelCmdF0ComboBox->Items);
	WheelCmdF2ComboBox->Items->Assign(WheelCmdF0ComboBox->Items);
	WheelCmdF3ComboBox->Items->Assign(WheelCmdF0ComboBox->Items);

	set_ComboBoxText(WheelCmdV0ComboBox,
		_T("AlphaBlend_+32/AlphaBlend_-32\n")
		_T("CursorUp/CursorDown\n")
		_T("PageUp/PageDown\n")
		_T("PrevFile/NextFile\n")
		_T("PrevNyanFi/NextNyanFi\n")
		_T("ScrollCursorUp/ScrollCursorDown\n")
		_T("ScrollUp/ScrollDown\n")
		_T("ZoomIn/ZoomOut\n"));
	WheelCmdV1ComboBox->Items->Assign(WheelCmdV0ComboBox->Items);
	WheelCmdV2ComboBox->Items->Assign(WheelCmdV0ComboBox->Items);
	WheelCmdV3ComboBox->Items->Assign(WheelCmdV0ComboBox->Items);

	set_ComboBoxText(WheelCmdI0ComboBox,
		_T("AlphaBlend_+32/AlphaBlend_-32\n")
		_T("PrevFile/NextFile\n")
		_T("PrevNyanFi/NextNyanFi\n")
		_T("PrevPage/NextPage\n")
		_T("RotateLeft/RotateRight\n")
		_T("ScrollLeft/ScrollRight\n")
		_T("ScrollUp/ScrollDown\n")
		_T("ZoomIn/ZoomOut\n"));
	WheelCmdI1ComboBox->Items->Assign(WheelCmdI0ComboBox->Items);
	WheelCmdI2ComboBox->Items->Assign(WheelCmdI0ComboBox->Items);
	WheelCmdI3ComboBox->Items->Assign(WheelCmdI0ComboBox->Items);

	//マウスボタン
	set_ComboBoxText(WheelBtnF0ComboBox,
		_T("AlphaBlend\n")
		_T("BackDirHist\n")
		_T("EqualListWidth\n")
		_T("ForwardDirHist\n")
		_T("HomeWorkList\n")
		_T("ToParent\n")
		_T("ToRoot\n")
		_T("ZoomReset\n"));
	WheelBtnF1ComboBox->Items->Assign(WheelBtnF0ComboBox->Items);
	WheelBtnF2ComboBox->Items->Assign(WheelBtnF0ComboBox->Items);
	WheelBtnF3ComboBox->Items->Assign(WheelBtnF0ComboBox->Items);

	set_ComboBoxText(X1BtnFComboBox,
		_T("BackDirHist\n")
		_T("ForwardDirHist\n")
		_T("NextDrive\n")
		_T("NextNyanFi\n")
		_T("NextTab\n")
		_T("PageDown\n")
		_T("PageUp\n")
		_T("PrevDrive\n")
		_T("PrevNyanFi\n")
		_T("PrevTab\n")
		_T("ToParent\n"));
	X2BtnFComboBox->Items->Assign(X1BtnFComboBox->Items);

	set_ComboBoxText(WheelBtnVComboBox,
		_T("AlphaBlend\n")
		_T("Close\n")
		_T("TextEnd\n")
		_T("TextTop\n")
		_T("ZoomReset\n"));

	set_ComboBoxText(X1BtnVComboBox,
		_T("BackViewHist\n")
		_T("NextFile\n")
		_T("NextNyanFi\n")
		_T("PageDown\n")
		_T("PageUp\n")
		_T("PrevFile\n")
		_T("PrevNyanFi\n"));
	X2BtnVComboBox->Items->Assign(X1BtnVComboBox->Items);

	set_ComboBoxText(WheelBtnIComboBox,
		_T("AlphaBlend\n")
		_T("Close\n")
		_T("EndFile\n")
		_T("FittedSize\n")
		_T("FullScreen\n")
		_T("TopFile\n"));

	set_ComboBoxText(X1BtnIComboBox,
		_T("Histogram\n")
		_T("Loupe\n")
		_T("NextFile\n")
		_T("NextNyanFi\n")
		_T("NextPage\n")
		_T("PrevFile\n")
		_T("PrevNyanFi\n")
		_T("PrevPage\n")
		_T("ThumbnailEx\n")
		_T("Thumbnail\n")
		_T("WarnHighlight\n"));
	X2BtnIComboBox->Items->Assign(X1BtnIComboBox->Items);

	set_ComboBoxText(MenuCmdComboBox,
		_T("MaskSel      マスクに該当するファイルを選択\n")
		_T("MaskDel      マスクに該当するファイルを削除\n")
		_T("MaskCopy     マスクに該当するファイルをコピー\n")
		_T("MaskMove     マスクに該当するファイルを移動\n")
		_T("MaskPack     マスクに該当するファイルを圧縮\n")
		_T("ExeCommands  指定したコマンドを実行\n"));
	RefMenuPrmBtn->Enabled = false;

	set_ComboBoxText(AppPrmComboBox,
		_T("\n")
		_T("FA : 一覧側\n")
		_T("FL : ランチャー側\n")
		_T("FI : INC.サーチ\n")
		_T("AO : 一覧のみ表示\n")
		_T("LO : ランチャーのみ表示\n")
		_T("LI : ランチャーのみ(INC.)\n"));

	set_ComboBoxText(OpenByModeComboBox, _T("そのまま\nWindowsの関連付けで開く\n標準の Enter キー動作\n"));

	set_ComboBoxText(DblClickFlComboBox,
		_T("標準の Enter キー動作 (OpenStandard)\n")
		_T("Windowsの関連付けで開く (OpenByWin)\n")
		_T("独自の関連付けで開く (OpenByApp)\n"));

	set_ComboBoxText(DefDropComboBox, _T("Windows 標準\n常にコピー\n常に移動\n"));

	set_ComboBoxText(StdCmdComboBox,
		_T("Windowsの関連付けで開く (OpenByWin)\n")
		_T("独自の関連付けで開く (OpenByApp)\n")
		_T("テキストビュアーで開く (TextViewer)\n")
		_T("エディタで開く (FileEdit)\n")
		_T("バイナリエディタで開く (BinaryEdit)\n")
		_T("コマンドファイルとして実行 (ExeCommands)\n")
		_T("指定コマンドを実行 (ExeCommands_)\n"));

	LayoutChanged  = false;
	inh_KeySet	   = 0;

	UnicodeString hntstr	= LoadUsrMsg(USTR_HintMltSepSC);
	NoWatchPathEdit->Hint	= hntstr;
	NoInfPathEdit->Hint 	= hntstr;
	NoImgPrvPathEdit->Hint	= hntstr;
	InsHrClsEdit->Hint		= hntstr;
	DelBlkClsEdit->Hint 	= hntstr;
	DelBlkIdEdit->Hint		= hntstr;
	NoDirHistPathEdit->Hint = hntstr;
	MarkPathEdit->Hint		= hntstr;
	L_IniMaskComboBox->Hint = hntstr;
	R_IniMaskComboBox->Hint = hntstr;

	hntstr = LoadUsrMsg(USTR_HintMltFExt);
	ExtColorEdit->Hint	   = hntstr;
	FExtGetInfEdit->Hint   = hntstr;
	FExtNoInfEdit->Hint    = hntstr;
	FExtImgPrvEdit->Hint   = hntstr;
	FExtNoImgPrvEdit->Hint = hntstr;
	FExtNoIViewEdit->Hint  = hntstr;
	MarkFextEdit->Hint	   = hntstr;
	FExt7zDllEdit->Hint    = hntstr;
	FExtImgEdit->Hint	   = hntstr;
	FExtEtcEdit->Hint	   = hntstr;
	ViewTab4Edit->Hint	   = hntstr;
	ViewTabXEdit->Hint	   = hntstr;
	FExtExeFileEdit->Hint  = hntstr;
	FExtStdEdit->Hint	   = hntstr;

	//コントロールのタグに、対応する変数のポインタを設定
	//UnicodeString
	L_IniPatEdit->Tag			= (int)&InitialPath[0];
	R_IniPatEdit->Tag			= (int)&InitialPath[1];
	TextEditorEdit->Tag 		= (int)&TextEditor;
	TextEditorFrmtEdit->Tag 	= (int)&TextEditorFrmt;
	ExtTxViewerEdit->Tag		= (int)&ExtTxViewer;
	ExtTxViewerFmtEdit->Tag 	= (int)&ExtTxViewerFrmt;
	ImageEditorEdit->Tag		= (int)&ImageEditor;
	FExtImgEdit->Tag			= (int)&FExtImgEidt;
	BinaryEditorEdit->Tag		= (int)&BinaryEditor;
	SndWarnEdit->Tag			= (int)&SoundWarning;
	SndTaskFinEdit->Tag 		= (int)&SoundTaskFin;
	SndFindFinEdit->Tag 		= (int)&SoundFindFin;
	SndWatchEdit->Tag			= (int)&SoundWatch;
	DirDelimiterEdit->Tag		= (int)&DirDelimiter;
	DirBraEdit->Tag 			= (int)&DirBraStr;
	DirKetEdit->Tag 			= (int)&DirKetStr;
	DrvInfFmtREdit->Tag 		= (int)&DrvInfFmtR;
	DrvInfFmtSEdit->Tag 		= (int)&DrvInfFmtS;
	DrvInfFmtNEdit->Tag 		= (int)&DrvInfFmtN;
	SttBarFmtEdit->Tag			= (int)&SttBarFmt;
	ClockFmtEdit->Tag			= (int)&SttClockFmt;
	TmStampFmtEdit->Tag 		= (int)&TimeStampFmt;
	ViewTab4Edit->Tag			= (int)&FExtViewTab4;
	ViewTabXEdit->Tag			= (int)&FExtViewTabX;
	TabXWdEdit->Tag				= (int)&ViewTabWidthX;
	EmpBin1Edit->Tag			= (int)&EmpBinPtn1;
	EmpBin2Edit->Tag			= (int)&EmpBinPtn2;
	EmpBin3Edit->Tag			= (int)&EmpBinPtn3;
	InsHrClsEdit->Tag			= (int)&HtmInsHrCls;
	HdrStrEdit->Tag 			= (int)&HtmHdrStr;
	DelBlkClsEdit->Tag			= (int)&HtmDelBlkCls;
	DelBlkIdEdit->Tag			= (int)&HtmDelBlkId;
	FExtGetInfEdit->Tag 		= (int)&FExtGetInf;
	FExtNoInfEdit->Tag			= (int)&FExtNoInf;
	NoWatchPathEdit->Tag		= (int)&NoWatchPath;
	NoInfPathEdit->Tag			= (int)&NoInfPath;
	NoImgPrvPathEdit->Tag		= (int)&NoImgPrvPath;
	FExtImgPrvEdit->Tag 		= (int)&FExtImgPrv;
	FExtNoImgPrvEdit->Tag		= (int)&FExtNoImgPrv;
	FExtNoIViewEdit->Tag		= (int)&FExtNoIView;
	MarkPathEdit->Tag			= (int)&MarkImgPath;
	MarkFextEdit->Tag			= (int)&MarkImgFExt;
	MarkMemoEdit->Tag			= (int)&MarkImgMemo;
	FExt7zDllEdit->Tag			= (int)&FExt7zDll;
	CmdGitExeEdit->Tag			= (int)&CmdGitExe;
	AutoRenFmtEdit->Tag			= (int)&AutoRenFmt;
	NoDirHistPathEdit->Tag		= (int)&NoDirHistPath;
	FExtExeFileEdit->Tag		= (int)&FExtExeFile;
	HomeWorkListEdit->Tag		= (int)&HomeWorkList;
	WebSeaUrlEdit->Tag			= (int)&WebSeaUrl;
	BgImg1Edit->Tag				= (int)&BgImgName[0];
	BgImg2Edit->Tag				= (int)&BgImgName[1];
	SpImgEdit->Tag				= (int)&BgImgName[2];
	SpImg2Edit->Tag				= (int)&BgImgName[3];
	KnobImgEdit->Tag			= (int)&BgImgName[4];
	TimerCnd1Edit->Tag			= (int)&Timer_Condition[0];
	TimerCmd1Edit->Tag			= (int)&OnTimerEvent[0];
	TimerCnd2Edit->Tag			= (int)&Timer_Condition[1];
	TimerCmd2Edit->Tag			= (int)&OnTimerEvent[1];

	//int (NumbersOnly = true)
	BatLowLimitEdit->Tag		= (int)&BatLowerLimit;
	MaxLogLinesEdit->Tag		= (int)&MaxLogLines;
	MaxLogFilesEdit->Tag		= (int)&MaxLogFiles;
	FwCntWarnEdit->Tag			= (int)&FlashCntWarning;
	FwTmWarnEdit->Tag			= (int)&FlashTimeWarning;
	FwCntTaskFinEdit->Tag		= (int)&FlashCntTaskfin;
	FwTmTaskFinEdit->Tag		= (int)&FlashTimeTaskfin;
	NotifyTaskTimeEdit->Tag		= (int)&NotifyTaskTime;
	FrmMgnEdit->Tag				= (int)&ImgFrameMargin;
	FitMaxZoomEdit->Tag			= (int)&ImgFitMaxZoom;
	MinShowTimeEdit->Tag		= (int)&MinShowTime;
	MsgHintTimeEdit->Tag		= (int)&MsgHintTime;
	KeyHintTimeEdit->Tag		= (int)&KeyHintTime;
	ImgGridHEdit->Tag			= (int)&ImgGridHorzN;
	ImgGridVEdit->Tag			= (int)&ImgGridVertN;
	ThumbSizeEdit->Tag	 		= (int)&ThumbnailSize;
	ThumbBdrEdit->Tag			= (int)&ThumbBdrWidth;
	IntrPgEdit->Tag				= (int)&ImgDblMargin;
	BdrMovWdLabeledEdit->Tag	= (int)&BorderMoveWidth;
	ListInterLnEdit->Tag		= (int)&ListInterLn;
	CursorWidthEdit->Tag		= (int)&CursorWidth;
	CursorAlphaEdit->Tag		= (int)&CursorAlpha;
	ListPercentEdit->Tag		= (int)&ListPercent;
	TabWidthEdit->Tag			= (int)&FlTabWidth;
	TextInterLnEdit->Tag		= (int)&ViewTxtInterLn;
	LeftMarginEdit->Tag 		= (int)&ViewLeftMargin;
	FoldWdEdit->Tag 			= (int)&ViewFoldWidth;
	FixedLimitEdit->Tag			= (int)&ViewFixedLimit;
	CellAlphaEdit->Tag			= (int)&CellAlpha;
	ViewWheelScrLnEdit->Tag 	= (int)&ViewWheelScrLn;
	ListWheelScrLnEdit->Tag 	= (int)&ListWheelScrLn;
	PrvTxtInfLnEdit->Tag		= (int)&PrvTxtInfLn;
	PrvTxtTailLnEdit->Tag		= (int)&PrvTxtTailLn;
	PrvActTailLnEdit->Tag		= (int)&PrvActTailLn;
	WatchIntervalEdit->Tag		= (int)&WatchInterval;
	LimitSndEdit->Tag			= (int)&PlaySndLimitTime;
	LogIntervalEdit->Tag		= (int)&LogInterval;
	IniWinWdEdit->Tag			= (int)&IniWinWidth;
	IniWinHiEdit->Tag			= (int)&IniWinHeight;
	DlgMoveEdit->Tag			= (int)&DlgMovePrm;
	DlgSizeEdit->Tag			= (int)&DlgSizePrm;
	CmpDelOwCntEdit->Tag		= (int)&CmpDelOwCnt;
	MigemoMinEdit->Tag			= (int)&IncSeaMigemoMin;
	ToleranceEdit->Tag 			= (int)&TimeTolerance;
	RemoteWaitTimeEdit->Tag 	= (int)&RemoteWaitTime;
	NormalWaitTimeEdit->Tag 	= (int)&NormalWaitTime;
	NopDtctTimeEdit->Tag		= (int)&NopDtctTime;
	ModalAlphaEdit->Tag 		= (int)&ModalScrAlpha;
	BgColAlphaEdit->Tag 		= (int)&BgColAlpha;
	HideTimeEdit->Tag			= (int)&BgHideTime;
	TileIfSzEdit->Tag			= (int)&BgTileSize;
	SplitWidthEdit->Tag 		= (int)&SplitterWidth;
	SplitWidth2Edit->Tag 		= (int)&SplitterWidth2;
	IconCacheEdit->Tag			= (int)&IconCache;
	FindPathWdEdit->Tag 		= (int)&FindPathWidth;
	FindTagsWdEdit->Tag 		= (int)&FindTagsWidth;

	//bool
	DivFlistUDCheckBox->Tag 	= (int)&DivFileListUD;
	DivDirInfUDCheckBox->Tag	= (int)&DivDirInfUD;
	HideAtrCheckBox->Tag		= (int)&ShowHideAtr;
	SysAtrCheckBox->Tag 		= (int)&ShowSystemAtr;
	DispRegNameCheckBox->Tag	= (int)&DispRegName;
	UncToNetDriveCheckBox->Tag	= (int)&UncToNetDrive;
	PathInTitleBarCheckBox->Tag = (int)&PathInTitleBar;
	UseIndIconCheckBox->Tag 	= (int)&UseIndIcon;
	OmitEndOfNameCheckBox->Tag	= (int)&OmitEndOfName;
	ShowAdsInfCheckBox->Tag		= (int)&ShowAdsInf;
	ShowProcInfCheckBox->Tag	= (int)&ShowUseProcInf;
	DirJumboCheckBox->Tag		= (int)&ShowDirJumboIco;
	InZipImgCheckBox->Tag		= (int)&ShowInZipImg;
	AniGifCheckBox->Tag 		= (int)&PreviewAniGif;
	SetCursorCheckBox->Tag 		= (int)&SetPrvCursor;
	PrevTxtToInfCheckBox->Tag	= (int)&ShowTextPreview;
	PrevTailCheckBox->Tag		= (int)&ShowTailPreview;
	ViewArcInfCheckBox->Tag 	= (int)&ViewArcInf;
	ReloadActCheckBox->Tag		= (int)&ReloadOnActive;
	OpenAddDrvCheckBox->Tag 	= (int)&OpenAddedDrive;
	CheckUncCheckBox->Tag		= (int)&CheckUnc;
	ForceDelCheckBox->Tag		= (int)&ForceDel;
	RemoveCdRoCheckBox->Tag 	= (int)&RemoveCdReadOnly;
	CopyTagsCheckBox->Tag		= (int)&CopyTags;
	CopyNoBuffCheckBox->Tag 	= (int)&CopyNoBuffering;
	ShowArcCopyPrgCheckBox->Tag = (int)&ShowArcCopyProg;
	DelUseTrsCheckBox->Tag		= (int)&DelUseTrash;
	NotShowNoTaskCheckBox->Tag	= (int)&NotShowNoTask;
	ShowMsgHintCheckBox->Tag	= (int)&ShowMsgHint;
	ShowKeyHintCheckBox->Tag	= (int)&ShowKeyHint;
	ShowNoKeyHintCheckBox->Tag	= (int)&ShowNoKeyHint;
	ShowCopyHintCheckBox->Tag	= (int)&ShowCopyHint;
	ShowDuplNotifyCheckBox->Tag = (int)&ShowDuplNotify;
	ShowTooltipCheckBox->Tag	= (int)&ShowTooltip;
	LogErrOnlyCheckBox->Tag 	= (int)&LogErrOnly;
	LogErrMsgCheckBox->Tag		= (int)&LogErrMsg;
	LogDebugInfCheckBox->Tag	= (int)&LogDebugInf;
	LogHideSkipCheckBox->Tag	= (int)&LogHideSkip;
	LogFullPathCheckBox->Tag	= (int)&LogFullPath;
	SaveLogCheckBox->Tag		= (int)&SaveLog;
	AppendLogCheckBox->Tag		= (int)&AppendLog;
	NotSortWLCheckBox->Tag		= (int)&NotSortWorkList;
	RegWkCsrPosCheckBox->Tag	= (int)&RegWorkCsrPos;
	AutoDelWLCheckBox->Tag		= (int)&AutoDelWorkList;
	NoChkWUncCheckBox->Tag		= (int)&NoCheckWorkUnc;
	AddToRecentCheckBox->Tag	= (int)&AddToRecent;
	NoChkRUncCheckBox->Tag		= (int)&NoCheckRecentUnc;
	DirHistCsrPosCheckBox->Tag	= (int)&DirHistCsrPos;
	DelDplDirHistCheckBox->Tag	= (int)&DelDplDirHist;
	WorkToDirHistCheckBox->Tag	= (int)&WorkToDirHist;
	NoChkDirHistCheckBox->Tag	= (int)&NoCheckDirHist;
	ExtSavDirHistCheckBox->Tag	= (int)&ExtSaveDirHist;
	LoopFlCsrCheckBox->Tag		= (int)&LoopFilerCursor;
	FlCsrVisiCheckBox->Tag		= (int)&FlCursorVisible;
	AutoCompCheckBox->Tag		= (int)&AutoCompComboBox;
	RestoreComboCheckBox->Tag	= (int)&RestoreComboBox;
	AutoHotkeyCheckBox->Tag 	= (int)&MenuAutoHotkey;
	EscHelpCheckBox->Tag		= (int)&EscapeHelp;
	InhAltMenuCheckBox->Tag 	= (int)&InhbitAltMenu;
	PermitCmdsCheckBox->Tag 	= (int)&PermitDotCmds;
	InheritDotCheckBox->Tag 	= (int)&InheritDotNyan;
	DotPerUserCheckBox->Tag 	= (int)&DotNyanPerUser;
	SelByMouseCheckBox->Tag 	= (int)&SelectByMouse;
	SelBaseOnlyCheckBox->Tag	= (int)&SelectBaseOnly;
	SelIconCheckBox->Tag		= (int)&SelectIconSngl;
	PriFExtColCheckBox->Tag 	= (int)&PriorFExtCol;
	ColOnlyFExtCheckBox->Tag	= (int)&ColorOnlyFExt;
	SymColToCheckBox->Tag		= (int)&SymColorToName;
	RevTagColCheckBox->Tag		= (int)&RevTagCololr;
	ShowDirTypeCheckBox->Tag	= (int)&ShowDirType;
	ShowSpaceCheckBox->Tag		= (int)&ShowSpace;
	ShowByteSizeCheckBox->Tag	= (int)&ShowByteSize;
	ShowTargetInfCheckBox->Tag	= (int)&ShowTargetInf;
	ShowImgPrvCheckBox->Tag 	= (int)&ShowImgPreview;
	ShowFilePropCheckBox->Tag	= (int)&ShowProperty;
	HideScrBarCheckBox->Tag 	= (int)&HideScrBar;
	ShowHeaderCheckBox->Tag 	= (int)&ShowHeader;
	FixListCheckBox->Tag		= (int)&FixListWidth;
	KeepOnResizeCheckBox->Tag	= (int)&KeepOnResize;
	KeepCurListWdCheckBox->Tag	= (int)&KeepCurListWidth;
	NoSpcFExtCheckBox->Tag		= (int)&NoSpaceFExt;
	ShowClsTabBtnCheckBox->Tag	= (int)&ShowClsTabBtn;
	ShowPopTabBtnCheckBox->Tag	= (int)&ShowPopTabBtn;
	ShowMainMenuCheckBox->Tag	= (int)&ShowMainMenu;
	HideTitMenuCheckBox->Tag	= (int)&HideTitleMenu;
	FlatInfPanelCheckBox->Tag	= (int)&FlatInfPanel;
	GrayScaleCheckBox->Tag		= (int)&BgImgGray;
	HideScrCheckBox->Tag		= (int)&BgImgHideScr;
	ShowPopDirBtnCheckBox->Tag	= (int)&ShowPopDirBtn;
	TileIfCheckBox->Tag 		= (int)&BgImgTileIf;
	ModalScrCheckBox->Tag		= (int)&ModalScreen;
	DlgCenterCheckBox->Tag		= (int)&DialogCenter;
	ShowLnNoCheckBox->Tag		= (int)&ShowLineNo;
	ShowLnCsrCheckBox->Tag		= (int)&ShowLineCursor;
	ShowTabCheckBox->Tag		= (int)&ShowTAB;
	ShowCrCheckBox->Tag 		= (int)&ShowCR;
	ShowRulerCheckBox->Tag		= (int)&ShowTextRuler;
	TxtSttIsBtmCheckBox->Tag	= (int)&TxtSttIsBottom;
	UseXd2txCheckBox->Tag		= (int)&UseXd2tx;		UseXd2txCheckBox->Enabled = xd2tx_Available;
	AozoraCheckBox->Tag 		= (int)&ChkAozora;
	ClickableUrlCheckBox->Tag	= (int)&ClickableUrl;
	RstViewLnCheckBox->Tag		= (int)&RestoreViewLine;
	ScrBarFoldCheckBox->Tag		= (int)&ScrBarToFoldPos;
	TvCsrVisiCheckBox->Tag		= (int)&TvCursorVisible;
	LimitBinCsrCheckBox->Tag	= (int)&LimitBinCsr;
	ColorHintCheckBox->Tag		= (int)&TxtColorHint;
	AltBackSlashCheckBox->Tag	= (int)&AltBackSlash;
	BinMemMapedCheckBox->Tag	= (int)&BinMemMaped;
	EmRemCheckBox->Tag			= (int)&EmpComment;
	EmStrCheckBox->Tag			= (int)&EmpStrings;
	EmRsvCheckBox->Tag			= (int)&EmpReserved;
	EmSymCheckBox->Tag			= (int)&EmpSymbol;
	EmNumCheckBox->Tag			= (int)&EmpNumeric;
	EmHdlnCheckBox->Tag 		= (int)&EmpHeadline;
	EmRubyCheckBox->Tag 		= (int)&EmpRuby;
	InsHrSctCheckBox->Tag		= (int)&HtmInsHrSct;
	InsHrArtCheckBox->Tag		= (int)&HtmInsHrArt;
	InsHrNavCheckBox->Tag		= (int)&HtmInsHrNav;
	MarkdownCheckBox->Tag		= (int)&ToMarkdown;
	RotViewImgCheckBox->Tag 	= (int)&RotViewImg;
	KeepZoomCheckBox->Tag		= (int)&KeepZoomRatio;
	ThumbNameCheckBox->Tag		= (int)&ShowThumbName;
	ThumbExifCheckBox->Tag		= (int)&ShowThumbExif;
	ThumbTagCheckBox->Tag		= (int)&ShowThumbTags;
	ThumbFExtCheckBox->Tag		= (int)&ShowThumbFExt;
	NotThumbIfArcCheckBox->Tag	= (int)&NotThumbIfArc;
	LoopCurCheckBox->Tag		= (int)&LoopViewCursor;
	HintTopEndCheckBox->Tag 	= (int)&HintTopEndImg;
	BeepTopEndCheckBox->Tag 	= (int)&BeepTopEndImg;
	ImgSttIsBtmCheckBox->Tag	= (int)&ImgSttIsBottom;
	ToolBarISideCheckBox->Tag	= (int)&ToolBarISide;
	SeekBindDirCheckBox->Tag	= (int)&SeekBindDir;
	SeekSwapNPCheckBox->Tag 	= (int)&SeekSwapNxtPrv;
	HideCsrFullCheckBox->Tag	= (int)&HideCsrInFull;
	HideThumbCheckBox->Tag		= (int)&HideThumbInFull;
	AnimateGifCheckBox->Tag 	= (int)&AnimateGif;
	ThumbScrCheckBox->Tag		= (int)&ShowThumbScroll;
	MarkImgCheckBox->Tag		= (int)&MarkImgClose;
															//<<<<<<<X86_SPI
	SureCopyCheckBox->Tag		= (int)&SureCopy;
	SureMoveCheckBox->Tag		= (int)&SureMove;
	SureDeleteCheckBox->Tag 	= (int)&SureDelete;
	SureUnPackCheckBox->Tag 	= (int)&SureUnPack;
	SureExecCheckBox->Tag		= (int)&SureExec;
	SureOtherCheckBox->Tag		= (int)&SureOther;
	SureExitCheckBox->Tag		= (int)&SureExit;
	SureCmpDelCheckBox->Tag 	= (int)&SureCmpDel;
	SureWorkListCheckBox->Tag 	= (int)&SureWorkList;
	SureCancelCheckBox->Tag 	= (int)&SureCancel;
	SureDefNoCheckBox->Tag		= (int)&SureDefNo;
	SureAdjPosCheckBox->Tag 	= (int)&SureAdjPos;
	WarnPowFailCheckBox->Tag	= (int)&WarnPowerFail;
	WarnLowBatheckBox->Tag		= (int)&WarnLowBattery;
	WarnDisconCheckBox->Tag 	= (int)&WarnDisconnect;
	PrtSubCheckBox->Tag 		= (int)&ProtectSubDir;
	PrtFileCheckBox->Tag		= (int)&ProtectFile;
	ImgEditSglCheckBox->Tag 	= (int)&ImageEditSgl;

	AppMinCheckBox->Tag			= (int)&AppListChgMin;
	OpenDirByStdCheckBox->Tag	= (int)&OpenDirByStd;
	OpenOnlyCurEditCheckBox->Tag= (int)&OpenOnlyCurEdit;
	DontClrSelEditCheckBox->Tag	= (int)&DontClrSelEdit;
	OpenOnlyCurAppCheckBox->Tag	= (int)&OpenOnlyCurApp;
	DontClrSelAppCheckBox->Tag 	= (int)&DontClrSelApp;
	OpenOnlyCurWinCheckBox->Tag = (int)&OpenOnlyCurWin;
	DontClrSelWinCheckBox->Tag 	= (int)&DontClrSelWin;
	StdTabGroupCheckBox->Tag	= (int)&OpenStdTabGroup;
	StdMenuFileCheckBox->Tag	= (int)&OpenStdMenuFile;
	StdResultListCheckBox->Tag	= (int)&OpenStdResultList;
	DowAfterStdCheckBox->Tag	= (int)&DownAfterOpenStd;
	OpnStdResCheckBox->Tag		= (int)&OpenStdOnResList;
	IncSeaCaseCheckBox->Tag 	= (int)&IncSeaCaseSens;
	IncSeaLoopCheckBox->Tag 	= (int)&IncSeaLoop;
	IncSea1ExitCheckBox->Tag	= (int)&IncSeaMatch1Exit;
	IniPathToTab1CheckBox->Tag	= (int)&IniPathToTab1;
	IniTabHomeAllCheckBox->Tag	= (int)&IniTabHomeAll;
	SplashCheckBox->Tag 		= (int)&ShowSplash;
	FixWinCheckBox->Tag 		= (int)&FixWinPos;
	IniSeaByNumCheckBox->Tag	= (int)&IniSeaByNum;
	IniSeaBySignCheckBox->Tag	= (int)&IniSeaBySign;
	MulInstCheckBox->Tag		= (int)&MultiInstance;
	CloseOtherCheckBox->Tag 	= (int)&CloseOthers;
	TaskTrayCheckBox->Tag		= (int)&StoreTaskTray;
	FindPathColCheckBox->Tag	= (int)&FindPathColumn;
	FindTagsColCheckBox->Tag	= (int)&FindTagsColumn;
	FindTagsSortCheckBox->Tag	= (int)&usr_TAG->SortTags;

	//UnicodeString (Style = csDropDown)
	L_IniMaskComboBox->Tag		= (int)&InitialMask[0];
	R_IniMaskComboBox->Tag		= (int)&InitialMask[1];
	WheelCmdF0ComboBox->Tag 	= (int)&WheelCmdF[0];
	WheelCmdF1ComboBox->Tag 	= (int)&WheelCmdF[1];
	WheelCmdF2ComboBox->Tag 	= (int)&WheelCmdF[2];
	WheelCmdF3ComboBox->Tag 	= (int)&WheelCmdF[3];
	WheelCmdV0ComboBox->Tag 	= (int)&WheelCmdV[0];
	WheelCmdV1ComboBox->Tag 	= (int)&WheelCmdV[1];
	WheelCmdV2ComboBox->Tag 	= (int)&WheelCmdV[2];
	WheelCmdV3ComboBox->Tag 	= (int)&WheelCmdV[3];
	WheelCmdI0ComboBox->Tag 	= (int)&WheelCmdI[0];
	WheelCmdI1ComboBox->Tag 	= (int)&WheelCmdI[1];
	WheelCmdI2ComboBox->Tag 	= (int)&WheelCmdI[2];
	WheelCmdI3ComboBox->Tag 	= (int)&WheelCmdI[3];
	WheelBtnF0ComboBox->Tag 	= (int)&WheelBtnCmdF[0];
	WheelBtnF1ComboBox->Tag 	= (int)&WheelBtnCmdF[1];
	WheelBtnF2ComboBox->Tag 	= (int)&WheelBtnCmdF[2];
	WheelBtnF3ComboBox->Tag 	= (int)&WheelBtnCmdF[3];
	WheelBtnVComboBox->Tag		= (int)&WheelBtnCmdV;
	WheelBtnIComboBox->Tag		= (int)&WheelBtnCmdI;
	X1BtnFComboBox->Tag 		= (int)&X1BtnCmdF;
	X2BtnFComboBox->Tag 		= (int)&X2BtnCmdF;
	X1BtnVComboBox->Tag 		= (int)&X1BtnCmdV;
	X2BtnVComboBox->Tag 		= (int)&X2BtnCmdV;
	X1BtnIComboBox->Tag 		= (int)&X1BtnCmdI;
	X2BtnIComboBox->Tag 		= (int)&X2BtnCmdI;

	//int (Style = csDropDownList)
	LayoutComboBox->Tag 		= (int)&LayoutMode;
	SizeFmtComboBox->Tag		= (int)&SizeFormatMode;
	TabStyleComboBox->Tag		= (int)&FlTabStyle;
	ScrBarStyleComboBox->Tag	= (int)&ScrBarStyle;
	BgImgModeComboBox->Tag		= (int)&BgImgMode;
	BgImgSubModeComboBox->Tag	= (int)&BgImgSubMode;

	ScaleOptComboBox->Tag		= (int)&WicScaleOpt;
	OpenByModeComboBox->Tag 	= (int)&OpenByMode;
	DblClickFlComboBox->Tag 	= (int)&DblClickFlMode;
	DefDropComboBox->Tag		= (int)&DefDropMode;
	SizeDecDgtComboBox->Tag 	= (int)&SizeDecDigits;

	IniModeIRadioGroup->Tag 	= (int)&InitialModeI;
	L_IniSortRadioGroup->Tag	= (int)&IniSortMode[0];
	R_IniSortRadioGroup->Tag	= (int)&IniSortMode[1];

	PrtDirRadioGroup->Tag		= (int)&ProtectDirMode;

	KeybdRadioGroup->Tag		= (int)&KeyboardMode;

	cursor_Default();

	//ドロップターゲットを設定
	usr_SH->AddTargetList(this, ToolExeEdit);
	usr_SH->AddTargetList(this, TextEditorEdit);
	usr_SH->AddTargetList(this, ImageEditorEdit);
	usr_SH->AddTargetList(this, BinaryEditorEdit);
	usr_SH->AddTargetList(this, EtcEditorEdit);
	usr_SH->AddTargetList(this, ExtTxViewerEdit);
	usr_SH->AddTargetList(this, AsoAppComboBox);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormShow(TObject *Sender)
{
	SwatchPanel->SetPanelSize16x8(15);

	set_ComboBox_AutoComp(this);
	SetKeyComboBox();

	DlgInitialized = false;
	cursor_HourGlass();

	//INIファイルがデフォルトと異なる場合、ファイル名を表示
	UnicodeString inam = IniFile->FileName;
	UnicodeString tit = "オプション設定";
	if (!SameText(inam, ChangeFileExt(Application->ExeName, ".INI"))) tit.cat_sprintf(_T(" - %s"), to_relative_name(inam).c_str());
	Caption = tit;

	set_ListBoxItemHi(ExtColListBox);
	set_ListBoxItemHi(TagColListBox);
												//<<<<<<<X86_SPI
	set_ListBoxItemHi(EtcEditorListBox);
	set_ListBoxItemHi(AssociateListBox);
	set_ListBoxItemHi(ExtMenuListBox);
	set_ListBoxItemHi(ExtToolListBox);
	set_ListBoxItemHi(KeyListBox);
	set_ListBoxItemHi(EventListBox);
	set_ListBoxItemHi(PrtDirListBox);
	set_ListBoxItemHi(VirDrvListBox);

	InitializeListHeader(ExtMenuHeader, _T("キャプション|エイリアス|設定"), Font);
	THeaderSections *sp = ExtMenuHeader->Sections;
	sp->Items[0]->Width = IniFile->ReadIntGen(_T("ExtMenuColWidth0"),	200);
	sp->Items[1]->Width = IniFile->ReadIntGen(_T("ExtMenuColWidth1"),	120);
	adjust_HeaderSecWidth(ExtMenuHeader, 2);

	InitializeListHeader(ExtToolHeader, _T("キャプション|エイリアス|設定"), Font);
	sp = ExtToolHeader->Sections;
	sp->Items[0]->Width = IniFile->ReadIntGen(_T("ExtToolColWidth0"),	200);
	sp->Items[1]->Width = IniFile->ReadIntGen(_T("ExtToolColWidth1"),	120);
	adjust_HeaderSecWidth(ExtToolHeader, 2);

	L_IniMaskComboBox->Clear();
	for (int i=0; i<PathMaskList->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(PathMaskList->Strings[i], 3, true);
		if (itm_buf[2].IsEmpty() || USAME_TS(itm_buf[2], "*")) continue;
		UnicodeString lbuf;
		if (!itm_buf[1].IsEmpty()) lbuf.sprintf(_T(":%s:"), itm_buf[1].c_str());
		L_IniMaskComboBox->Items->Add(lbuf + itm_buf[2]);
	}
	R_IniMaskComboBox->Items->Assign(L_IniMaskComboBox->Items);

	VirDrvComboBox->Clear();
	for (int i=0; i<26; i++) VirDrvComboBox->Items->Add(UnicodeString().sprintf(_T("%c"), 'A' + i));

	FindEdit->Text = EmptyStr;

	HelpContext = PageControl1->ActivePage->HelpContext;

	//タグを用いてコントロールに値を設定
	BringOptionByTag(this);

	//タグを用いない設定
	TempDirEdit->Text	 = TempPath;
	MigemoDirEdit->Text  = MigemoPath;
														//<<<<<<<X86_SPI
	LimitTxtEdit->Text	 = ViewTxtLimitSize/1024;
	LimitBinEdit->Text	 = ViewBinLimitSize/1048576;
	IniWinLeftEdit->Text = IniWinLeft;
	IniWinTopEdit->Text  = IniWinTop;
	PinMarkEdit->Text	 = TabPinMark;

	MaxTasksComboBox->ItemIndex = MaxTasks - 1;
	AppPrmComboBox->ItemIndex	= idx_of_word_i(_T("|FA|FL|FI|AO|LO|LI"), AppListHotPrm);
	ShowIconCheckBox->Checked	= IconMode>0;

	for (int i=0; i<FontBufList->Count; i++) {
		TFont *f = (TFont *)FontBufList->Objects[i];
		f->Assign((TFont *)FontList->Objects[i]);
	}

	ColBufList->Assign(ColorList);
	ModalColorBox->Selected = (TColor)get_ListIntVal(ColBufList, _T("ModalScr"), clBlack);

	AssociateListBox->Items->Assign(AssociateList);
	EtcEditorListBox->Items->Assign(EtcEditorList);
	ExtColListBox->Items->Assign(ExtColList);
	StdCmdListBox->Items->Assign(OpenStdCmdList);
	PrtDirListBox->Items->Assign(ProtectDirList);
	VirDrvListBox->Items->Assign(VirDriveList);

	TagColListBox->Clear();
	for (int i=0; i<usr_TAG->TagNameList->Count; i++) {
		UnicodeString tag = usr_TAG->TagNameList->Strings[i];
		TagColListBox->Items->Add(
			UnicodeString().sprintf(_T("%s=%d"), tag.c_str(), usr_TAG->GetColor(tag, clNone)));
	}

	TCheckListBox *lp = ExtMenuListBox;
	lp->Items->Assign(ExtMenuList);
	for (int i=0; i<lp->Count; i++) lp->Checked[i] = !equal_0(get_csv_item(lp->Items->Strings[i], 4));

	lp = ExtToolListBox;
	lp->Items->Assign(ExtToolList);
	for (int i=0; i<lp->Count; i++) lp->Checked[i] = !equal_0(get_csv_item(lp->Items->Strings[i], 5));

	//キー一覧のヘッダを設定
	InitializeListHeader(KeyHeaderControl, _T("キー|コマンド_パラメータ|説明|Fキー表示名"), Font);
	TCanvas *cv = KeyListBox->Canvas;
	cv->Font->Assign(KeyListBox->Font);
	int w0 = cv->TextWidth("Shift+Ctrl+Alt+Space ");
	int w1 = get_CharWidth(cv, 40);
	int w2 = KeyHeaderControl->ClientWidth - w0 - w1 - get_CharWidth(cv, 16) - ::GetSystemMetrics(SM_CXVSCROLL) - 4;
	set_HeaderSecWidth(KeyHeaderControl, 3, w0, w1, w2);

	KeySortMode = IniFile->ReadIntGen(_T("OptKeySortMode"),	0);
	KeyFuncList->CustomSort(KeyComp_Key);

	for (int i=0; i<MAX_KEYTABS; i++) {
		KeyListBuf[i]->Clear();
		UnicodeString modstr = GetCmdModeStr(i + 1);
		for (int j=0; j<KeyFuncList->Count; j++){
			UnicodeString lbuf = KeyFuncList->Strings[j];
			if (StartsText(modstr, lbuf)) KeyListBuf[i]->Add(lbuf);
		}
	}
	FKeyLabelBuf->Assign(FKeyLabelList);

	PageControl1Change(NULL);

	(ViewFoldFitWin? FitFoldRadioBtn : SetFoldRadioBtn)->Checked = true;

	switch (IniPathMode[0]) {
	case 1:  L_IniPatMod1RadioBtn->Checked = true; break;
	case 2:  L_IniPatMod2RadioBtn->Checked = true; break;
	default: L_IniPatMod0RadioBtn->Checked = true;
	}

	switch (IniPathMode[1]) {
	case 1:  R_IniPatMod1RadioBtn->Checked = true; break;
	case 2:  R_IniPatMod2RadioBtn->Checked = true; break;
	default: R_IniPatMod0RadioBtn->Checked = true;
	}

	((IniWorkMode==1)? IniWork1RadioBtn : IniWork0RadioBtn)->Checked = true;
	((IniWinMode==1)?  IniWin1RadioBtn  : IniWin0RadioBtn)->Checked  = true;

	set_MigemoCheckBox(MigemoCheckBox, _T("OptKeyMigemo"));

	UnRegCmdCheckBox->Checked = IniFile->ReadBoolGen(_T("OptUnregCmd"));
	SetCmdCombo();

	AsoSortCheckBox->Checked  = IniFile->ReadBoolGen(_T("OptAsoSort"));

	DlgMoveShiftCheckBox->Checked = ContainsText(DlgMoveShift, KeyStr_Shift);
	DlgMoveCtrlCheckBox->Checked  = ContainsText(DlgMoveShift, KeyStr_Ctrl);
	DlgMoveAltCheckBox->Checked   = ContainsText(DlgMoveShift, KeyStr_Alt);
	DlgSizeShiftCheckBox->Checked = ContainsText(DlgSizeShift, KeyStr_Shift);
	DlgSizeCtrlCheckBox->Checked  = ContainsText(DlgSizeShift, KeyStr_Ctrl);
	DlgSizeAltCheckBox->Checked   = ContainsText(DlgSizeShift, KeyStr_Alt);

	UnicodeString kstr = CallHotKey;
	HotShiftCheckBox->Checked	  = remove_text(kstr, KeyStr_Shift);
	HotCtrlCheckBox->Checked	  = remove_text(kstr, KeyStr_Ctrl);
	HotAltCheckBox->Checked 	  = remove_text(kstr, KeyStr_Alt);
	HotWinCheckBox->Checked 	  = remove_text(kstr, _T("Win+"));
	HotKeyComboBox->ItemIndex	  = HotKeyComboBox->Items->IndexOf(kstr);

	kstr = AppListHotKey;
	AppShiftCheckBox->Checked	  = remove_text(kstr, KeyStr_Shift);
	AppCtrlCheckBox->Checked	  = remove_text(kstr, KeyStr_Ctrl);
	AppAltCheckBox->Checked 	  = remove_text(kstr, KeyStr_Alt);
	AppWinCheckBox->Checked 	  = remove_text(kstr, _T("Win+"));
	AppKeyComboBox->ItemIndex	  = AppKeyComboBox->Items->IndexOf(kstr);

	IniSeaShiftCheckBox->Checked  = ContainsText(IniSeaShift, KeyStr_Shift);
	IniSeaCtrlCheckBox->Checked   = ContainsText(IniSeaShift, KeyStr_Ctrl);
	IniSeaAltCheckBox->Checked	  = ContainsText(IniSeaShift, KeyStr_Alt);

				//<<<<<<<X86_SPI

	//イベント
	EventListBox->Clear();
	for (int i=0; i<MAX_EVENT_CMD; i++) {
		EventListBox->Items->Add(
			UnicodeString().sprintf(_T("%s\t%s"), EventCmdList[i].dsc, EventCmdList[i].sp->c_str()));
	}
	EventCmdsEdit->Text = EmptyStr;

	LayoutChanged	= false;
	WinSizeChanged	= false;
	TlBarColChanged = false;

	KeyWd = EmptyStr;

	FExt7zDllEdit->Enabled = usr_ARC->Use7zDll;

	UpdateMaxItemWidth();

	cursor_Default();
	DlgInitialized = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (ScrMode==SCMD_FLIST) OptionPageIndex = PageControl1->ActivePageIndex;

	UserModule->CmdParamList->Clear();

	IconImgListP->Clear();
	UserModule->UninitializeListBox();

	THeaderSections *sp = ExtMenuHeader->Sections;
	IniFile->WriteIntGen(_T("ExtMenuColWidth0"),	sp->Items[0]->Width);
	IniFile->WriteIntGen(_T("ExtMenuColWidth1"),	sp->Items[1]->Width);
	sp = ExtToolHeader->Sections;
	IniFile->WriteIntGen(_T("ExtToolColWidth0"),	sp->Items[0]->Width);
	IniFile->WriteIntGen(_T("ExtToolColWidth1"),	sp->Items[1]->Width);

	IniFile->WriteIntGen(_T("OptKeySortMode"),		KeySortMode);
	IniFile->WriteBoolGen(_T("OptKeyMigemo"),		MigemoCheckBox);
	IniFile->WriteBoolGen(_T("OptUnregCmd"),		UnRegCmdCheckBox);
	IniFile->WriteBoolGen(_T("OptAsoSort"),			AsoSortCheckBox);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormDestroy(TObject *Sender)
{
	delete FindMarkList;

	for (int i=0; i<FontBufList->Count; i++) delete (TFont*)FontBufList->Objects[i];
	delete FontBufList;

	delete ColBufList;

	for (int i=0; i<MAX_KEYTABS; i++) delete KeyListBuf[i];
	delete FKeyLabelBuf;
}

//---------------------------------------------------------------------------
//ロップ受け入れ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::WmDropped(TMessage &msg)
{
	TWinControl *cp = (TWinControl *)msg.WParam;
	if (cp==ToolExeEdit)
		SetExtToolExeName(DroppedList->Strings[0]);
	else
		SetExtNameToCtrl(DroppedList->Strings[0], cp, cp==AsoAppComboBox);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::PageControl1Change(TObject *Sender)
{
	HelpContext = PageControl1->ActivePage->HelpContext;

	if (!Sender || PageControl1->ActivePage==KeySetSheet) {
		KeyTabControl->TabIndex =
			LogWndListBox->Focused()? 4 :
			   (ScrMode==SCMD_FLIST)? (CurStt->is_IncSea? 1 : 0) :
			   (ScrMode==SCMD_TVIEW)? 2 :
			   (ScrMode==SCMD_IVIEW)? 3 : 0;

		KeyTabControlChange(KeyTabControl);
		KeyComboBox->ItemIndex = -1;
	}

	if (!Sender || PageControl1->ActivePage==AssoSheet) 	AssociateListBoxClick(NULL);
	if (!Sender || PageControl1->ActivePage==ExtMenuSheet)	ExtMenuListBoxClick(NULL);
	if (!Sender || PageControl1->ActivePage==ExtToolSheet)	ExtToolListBoxClick(NULL);

	UserModule->InitializeListBox(GetCurListBox());
}

//---------------------------------------------------------------------------
//タブの描画 (検索結果の強調表示)
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::PageControl1DrawTab(TCustomTabControl *Control, int TabIndex,
		const TRect &Rect, bool Active)
{
	TTabControl *tp = (TTabControl*)Control;
	//背景
	TCanvas *cv = tp->Canvas;
	cv->Brush->Color = (PageControl1->Pages[TabIndex]->Tag==0)? (Active? col_bgOptTab : Color) : col_OptFind;
	cv->FillRect(Rect);
	//タイトル
	cv->Font->Color = Active? col_fgOptTab : scl_BtnText;
	cv->Font->Style = Active? (cv->Font->Style << fsBold) : (cv->Font->Style >> fsBold);
	UnicodeString tstr = tp->Tabs->Strings[TabIndex];
	cv->TextOut(Rect.Left + (Rect.Width() - cv->TextWidth(tstr))/2, Rect.Top + (Active? 4 : 2), tstr);
}

//---------------------------------------------------------------------------
//キー設定コンボボックスを設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetKeyComboBox()
{
	UnicodeString kmd;
	KeyKeyGroupBox->Caption = kmd.sprintf(_T("キー (%s)"), is_JpKeybd()? _T("JP") : _T("US"));
	KeybdRadioGroup->Items->Strings[0] = kmd.sprintf(_T("自動(%s)"), (::GetKeyboardType(0)==7)? _T("JP") : _T("US"));

	make_KeyList(KeyComboBox->Items);
	make_KeyList(Key2ComboBox->Items);
	Key2ComboBox->Items->Insert(0, EmptyStr);
	Key2ComboBox->Enabled = false;

	make_KeyList(HotKeyComboBox->Items);
	HotKeyComboBox->Items->Insert(0, EmptyStr);
	make_KeyList(AppKeyComboBox->Items);
	AppKeyComboBox->Items->Insert(0, EmptyStr);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeybdRadioGroupClick(TObject *Sender)
{
	if (!DlgInitialized) return;

	KeyboardMode = KeybdRadioGroup->ItemIndex;
	SetKeyComboBox();
}

//---------------------------------------------------------------------------
//現在の対象リストボックスを取得
//---------------------------------------------------------------------------
TCustomListBox* __fastcall TOptionDlg::GetCurListBox()
{
	if (PageControl1->ActivePage==ExtMenuSheet) return ExtMenuListBox;
	if (PageControl1->ActivePage==ExtToolSheet) return ExtToolListBox;
	if (PageControl1->ActivePage==FontColSheet) return ExtColListBox;
	if (PageControl1->ActivePage==EditorSheet)  return EtcEditorListBox;
	if (PageControl1->ActivePage==AssoSheet)    return AssociateListBox;
	if (PageControl1->ActivePage==CommadSheet)  return StdCmdListBox;
	if (PageControl1->ActivePage==StartupSheet) return VirDrvListBox;
	if (PageControl1->ActivePage==NotifySheet)  return PrtDirListBox;
												return NULL;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::UpdateMaxItemWidth()
{
	TCanvas *cv = AssociateListBox->Canvas;
	cv->Font->Assign(DialogFont);
	MaxWd_AssExt = 0;
	for (int i=0; i<AssociateListBox->Count; i++)
		MaxWd_AssExt = std::max(cv->TextWidth(AssociateListBox->Items->Names[i]), MaxWd_AssExt);
	MaxWd_AssExt += ScaledInt(20);

	cv = EventListBox->Canvas;
	cv->Font->Assign(DialogFont);
	MaxWd_Ev = 0;
	for (int i=0; i<MAX_EVENT_CMD; i++)
		MaxWd_Ev = std::max(cv->TextWidth(EventCmdList[i].dsc), MaxWd_Ev);
	MaxWd_Ev += ScaledInt(20);

	cv = TagColListBox->Canvas;
	cv->Font->Assign(DialogFont);
	MaxWd_Tag = 0;
	for (int i=0; i<usr_TAG->TagNameList->Count; i++)
		MaxWd_Tag = std::max(cv->TextWidth(usr_TAG->TagNameList->Strings[i]), MaxWd_Tag);
	MaxWd_Tag += ScaledInt(20);
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TOptionDlg::GetCmdModeStr(int idx)
{
	if (idx==0) idx = KeyTabControl->TabIndex + 1;
	return (idx>=1 && idx<=MAX_KEYTABS)? (ScrModeIdStr.SubString(idx, 1) + ":") : EmptyStr;
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TOptionDlg::GetCmdKeyStr()
{
	UnicodeString kstr = GetCmdModeStr();
	if (SelCheckBox->Checked)   kstr += KeyStr_SELECT;
	if (ShiftCheckBox->Checked) kstr += KeyStr_Shift;
	if (CtrlCheckBox->Checked)  kstr += KeyStr_Ctrl;
	if (AltCheckBox->Checked)   kstr += KeyStr_Alt;
	kstr += KeyComboBox->Text;
	if (!Key2ComboBox->Text.IsEmpty()) kstr.cat_sprintf(_T("~%s"), Key2ComboBox->Text.c_str());
	return kstr;
}

//---------------------------------------------------------------------------
//2ストローク開始キーか?
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::IsFirstCmdKey()
{
	bool res = false;

	UnicodeString key = get_tkn_r(GetCmdKeyStr(), ':');
	if (!ContainsStr(key, "~")) {
		TStrings *sp = KeyListBox->Items;
		for (int i=0; i<sp->Count; i++) {
			UnicodeString fkey = sp->Names[i];
			if (ContainsStr(fkey, "~")) {
				fkey = get_tkn_m(fkey, ':', '~');
				if (SameText(key, fkey)) { res = true; break; }
			}
		}
	}

	return res;
}

//---------------------------------------------------------------------------
//タブを設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetSheet(UnicodeString prm)
{
	int idx = idx_of_word_i(_T("GN|G2|MO|DS|FC|TV|IV|ED|KY|AC|XM|XT|ST|NT|CM|EV"), prm);
	if (idx!=-1) {
		PageControl1->ActivePageIndex = idx;
	}
	else {
		if		(ScrMode==SCMD_TVIEW)	PageControl1->ActivePage = TxtViewerSheet;
		else if (ScrMode==SCMD_IVIEW)	PageControl1->ActivePage = ImgViewerSheet;
		else 							PageControl1->ActivePageIndex = OptionPageIndex;
	}
}

//---------------------------------------------------------------------------
//一時ディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefTmpDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = ExePath;
	if (UserModule->SelectDirEx(_T("一時ディレクトリ"), dnam, true)) TempDirEdit->Text = dnam;
}

//---------------------------------------------------------------------------
//Migemoディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefMigemoBtnClick(TObject *Sender)
{
	UnicodeString dnam = ExePath;
	if (UserModule->SelectDirEx(_T("Migemo ディレクトリ"), dnam, true)) MigemoDirEdit->Text = dnam;
}

//---------------------------------------------------------------------------
//git.ext 参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefCmdGitExeBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("git.exe の指定"), F_FILTER_EXE, _T("git.exe"), cv_env_var("%PROGRAMFILES%"));
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) CmdGitExeEdit->Text = fnam;
}

//---------------------------------------------------------------------------
//その他のエディタ一覧の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EtcEditorListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);

	int w_x = 0;
	for (int i=0; i<lp->Count; i++) w_x = std::max(cv->TextWidth(lp->Items->Names[i]), w_x);

	UnicodeString etc_fext = lp->Items->Names[Index];
	UnicodeString etc_edtr = exclude_quot(lp->Items->ValueFromIndex[Index]);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(xp, yp, etc_fext);
	cv->TextOut(xp + w_x + 20, yp, etc_edtr);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EtcEditorListBoxClick(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		FExtEtcEdit->Text	= lp->Items->Names[idx];
		EtcEditorEdit->Text = exclude_quot(lp->Items->ValueFromIndex[idx]);
	}
}

//---------------------------------------------------------------------------
//エディタ/ビュアーの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefEditorBtnClick(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;
	UnicodeString tit = get_word_i_idx(
		_T("テキストエディタ|イメージエディタ|バイナリエディタ|その他のエディタ|外部テキストビュアー"), tag);
	UserModule->PrepareOpenDlg(tit.c_str(), F_FILTER_EXE2);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) {
		switch (tag) {
		case 0: SetExtNameToCtrl(fnam, TextEditorEdit);		break;
		case 1: SetExtNameToCtrl(fnam, ImageEditorEdit);	break;
		case 2: SetExtNameToCtrl(fnam, BinaryEditorEdit);	break;
		case 3: SetExtNameToCtrl(fnam, EtcEditorEdit);		break;
		case 4: SetExtNameToCtrl(fnam, ExtTxViewerEdit);	break;
		}
	}
}

//---------------------------------------------------------------------------
//タスク終了時の通知音の参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefSoundBtnClick(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;

	UnicodeString inidir = def_if_empty(ExtractFileDir(to_absolute_name((tag==0)? SndWarnEdit->Text : SndTaskFinEdit->Text)), ExePath);
	UserModule->PrepareOpenDlg(_T("サウンドファイルの指定"), F_FILTER_WAV, NULL, inidir);

	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam, true)) {
		switch (tag) {
		case 0: SndWarnEdit->Text    = fnam;	break;
		case 1: SndTaskFinEdit->Text = fnam;	break;
		case 2: SndFindFinEdit->Text = fnam;	break;
		case 3: SndWatchEdit->Text	 = fnam;	break;
		}
		play_sound(UserModule->OpenDlg->FileName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TestSoundBtnClick(TObject *Sender)
{
	switch (((TComponent*)Sender)->Tag) {
	case 0: play_sound(SndWarnEdit->Text);		break;
	case 1: play_sound(SndTaskFinEdit->Text);	break;
	case 2: play_sound(SndFindFinEdit->Text);	break;
	case 3: play_sound(SndWatchEdit->Text);		break;
	}
}

//---------------------------------------------------------------------------
//全体履歴ファイル設定の変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtSavDirHistCheckBoxClick(TObject *Sender)
{
	if (!DlgInitialized) return;

	if (ExtSavDirHistCheckBox->Checked)	{
		UnicodeString fnam = ExePath + DIR_HIST_FILE;
		if (file_exists(fnam) && msgbox_Sure(_T("既存の履歴ファイルを読み込みますか？"))) {
			std::unique_ptr<UsrIniFile> hst_file(new UsrIniFile(fnam));
			hst_file->LoadListItems("AllDirHistory", AllDirHistory, 0, false);
		}
	}
}

//---------------------------------------------------------------------------
//レイアウト変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::LayoutChange(TObject *Sender)
{
	LayoutChanged = true;
}

//---------------------------------------------------------------------------
//背景画像の参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefBgImgBtnClick(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;
	TLabeledEdit *ep = (tag==0)? BgImg1Edit : (tag==1)? BgImg2Edit : (tag==2)? SpImgEdit : SpImg2Edit;
	UserModule->OpenImgDlg->Title = "背景画像の指定";
	UserModule->SetOpenImgFilter(ep->Text);
	UserModule->OpenImgDlg->InitialDir = to_absolute_name(ExtractFilePath(ep->Text));
	UserModule->OpenImgDlg->FileName   = EmptyStr;
	UserModule->OpenImgDlgToEdit(ep, true);
}
//---------------------------------------------------------------------------
//シンプルスクロールノブ画像の参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ScrBarStyleComboBoxClick(TObject *Sender)
{
	if (ScrBarStyleComboBox->ItemIndex==4) {
		UserModule->OpenImgDlg->Title = "ノブ画像の指定";
		UserModule->SetOpenImgFilter(KnobImgEdit->Text);
		UserModule->OpenImgDlg->InitialDir = to_absolute_name(ExtractFilePath(KnobImgEdit->Text));
		UserModule->OpenImgDlg->FileName   = EmptyStr;
		if (!UserModule->OpenImgDlgToEdit(KnobImgEdit, true)) {
			ScrBarStyleComboBox->ItemIndex = 0;
			KnobImgEdit->Text = EmptyStr;
		}
	}
	else {
		KnobImgEdit->Text = EmptyStr;
	}
}

//---------------------------------------------------------------------------
//フォント
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FontComboBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas   *cv = cp->Canvas;
	cv->Font->Assign(cp->Font);
	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);
	int fh = abs(cv->Font->Height);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(xp, yp, cp->Items->Strings[Index]);
	xp += fh*10;

	if (Index<FontBufList->Count) {
		TFont *f = (TFont*)FontBufList->Objects[Index];
		//サイズ
		UnicodeString szstr = f->Size;
		cv->TextOut(xp + get_CharWidth(cv, 2) - cv->TextWidth(szstr), yp, szstr);
		xp += fh*2;
		//名前
		cv->Font->Assign(f);
		cv->Font->Size = cp->Font->Size;
		SetHighlight(cv, State.Contains(odSelected));
		yp = Rect.Top + get_TopMargin(cv);
		cv->TextOut(xp, yp, f->Name);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefFontBtnClick(TObject *Sender)
{
	int idx = FontComboBox->ItemIndex;
	if (idx>=0 && idx<FontComboBox->Items->Count && idx<FontBufList->Count) {
		TFont *f = (TFont*)FontBufList->Objects[idx];
		UserModule->FontDlg->Options.Clear();
		if (USAME_TI(FontBufList->ValueFromIndex[idx], "テキストビュアー"))
			UserModule->FontDlg->Options << fdFixedPitchOnly;	//テキストビュアーは等幅

		if (UserModule->FontDlgToFont(f)) {
			if (USAME_TI(FontBufList->ValueFromIndex[idx], "ダイアログ")) {
				if (f->Size>10) f->Size = 10;
			}
			FontComboBox->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
//配色
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OptColListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	draw_ColorListBox((TListBox*)Control, Rect, Index, State, ColBufList);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefColBtnClick(TObject *Sender)
{
	TListBox *lp = (((TButton*)Sender)->Tag==1)? TimColListBox : OptColListBox;
	int idx = lp->ItemIndex;	if (idx==-1) return;

	UnicodeString col_nam = lp->Items->Names[idx];
	UserModule->ColorDlg->Color = (TColor)ColBufList->Values[col_nam].ToIntDef(clBlack);
	if (UserModule->ColorDlg->Execute()) {
		ColBufList->Values[col_nam] = IntToStr(UserModule->ColorDlg->Color);
		lp->Repaint();
		TagColListBox->Invalidate();
		if (contained_wd_i(_T("bgTlBar1|bgTlBar2"), col_nam)) TlBarColChanged = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ModalColorBoxChange(TObject *Sender)
{
	ColBufList->Values["ModalScr"] = IntToStr(ModalColorBox->Selected);
}

//---------------------------------------------------------------------------
//配色の無効化
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableColActionExecute(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	if (lp->ItemIndex!=-1) {
		ColBufList->Values[lp->Items->Names[lp->ItemIndex]] = IntToStr(clNone);
		lp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableColActionUpdate(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString col_id = (lp->ItemIndex!=-1)? lp->Items->Names[lp->ItemIndex] : EmptyStr;

	((TAction*)Sender)->Enabled
		= contained_wd_i(
			_T("bgList2|fgSelItem|Protect|Compress|frScrKnob|lnScrHit|frmTab|bdrLine|bdrFold|bdrFixed|TlBorder"), col_id);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableTimColActionExecute(TObject *Sender)
{
	TListBox *lp = TimColListBox;
	if (lp->ItemIndex!=-1) {
		ColBufList->Values[lp->Items->Names[lp->ItemIndex]] = IntToStr(clNone);
		lp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DisableTimColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = TimColListBox->ItemIndex!=-1;
}

//---------------------------------------------------------------------------
//スポイト
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	if (Button!=mbLeft) return;

	TImage *ip = (TImage*)Sender;
	SpuitTag = ip->Tag;

	TColor col_def;
	if (SpuitTag==2) {
		col_def = RefExtColPanel->Color;
	}
	else {
		TListBox *lp = (SpuitTag==3)? TagColListBox : (SpuitTag==1)? TimColListBox : OptColListBox;
		if (lp->ItemIndex==-1) return;

		col_def = (SpuitTag==3)?
					(TColor)lp->Items->ValueFromIndex[lp->ItemIndex].ToIntDef(clNone) :
					(TColor)ColBufList->Values[lp->Items->Names[lp->ItemIndex]].ToIntDef(clNone);
	}

	//スウォッチブック
	SetCustomColToSwatch(UserModule->ColorDlg->CustomColors);

	TGroupBox *gp = (SpuitTag==3)? TagColGroupBox :
					(SpuitTag==2)? ExtColGroupBox :
					(SpuitTag==1)? TimColGroupBox : OptColGroupBox;
	SwatchPanel->Parent  = PageControl1->ActivePage;
	SwatchPanel->Left	 = gp->Left + gp->Width - SwatchPanel->Width;
	SwatchPanel->Top	 = gp->Top + gp->Height - SwatchPanel->Height + ((SpuitTag==2)? 50 : 0);
	SwatchPanel->Visible = true;
	SwatchPanel->BringToFront();

	UserModule->BeginSpuit(
		(SpuitTag==3)? SpuitPanel4 : (SpuitTag==2)? SpuitPanel3 : (SpuitTag==1)? SpuitPanel2 : SpuitPanel,
		ip, col_def);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SpuitImageMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	SwatchPanel->Visible = false;

	if (UserModule->SpuitEnabled()) {
		TColor col = UserModule->EndSpuit();
		if (SpuitTag==2) {
			RefExtColPanel->Color = col;
		}
		else {
			TListBox *lp = (SpuitTag==3)? TagColListBox : (SpuitTag==1)? TimColListBox : OptColListBox;
			if (lp->ItemIndex!=-1) {
				if (SpuitTag==3) {
					lp->Items->ValueFromIndex[lp->ItemIndex] = IntToStr(col);
				}
				else {
					UnicodeString col_nam = lp->Items->Names[lp->ItemIndex];
					ColBufList->Values[col_nam] = IntToStr(col);
					if (contained_wd_i(_T("bgTlBar1|bgTlBar2"), col_nam)) TlBarColChanged = true;
				}
				lp->Repaint();
				if (SpuitTag!=3) TagColListBox->Invalidate();
			}
		}
	}
}
//---------------------------------------------------------------------------
//透明度制限
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ModalAlphaEditChange(TObject *Sender)
{
	TLabeledEdit *ep = (TLabeledEdit*)Sender;
	if (EditToInt(ep, 0)>255) ep->Text = 255;
}

//---------------------------------------------------------------------------
//拡張子別配色
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtColListBoxClick(TObject *Sender)
{
	int idx = ExtColListBox->ItemIndex;  if (idx==-1) return;
	UnicodeString lbuf = ExtColListBox->Items->Strings[idx];
	RefExtColPanel->Color = (TColor)split_tkn(lbuf, ',').ToIntDef(clNone);
	ExtColorEdit->Text	  = lbuf;
}

//---------------------------------------------------------------------------
//拡張子別配色一覧の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtColListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);

	UnicodeString lbuf = lp->Items->Strings[Index];
	cv->Font->Color = (TColor)split_tkn(lbuf, ',').ToIntDef(clNone);
	bool is_dot = USAME_TS(lbuf, ".");
	UnicodeString ext = ReplaceStr(lbuf, ".", " .");
	cv->Brush->Color = col_bgList;

	UnicodeString smpl_str;
	smpl_str.sprintf(_T("%s"), is_dot? _T("\u2588 dot") : _T("\u2588.xxx"));
	int smpl_wd = cv->TextWidth(_T("\u2588.xxx")) + ScaledInt(10);
	TRect rc = Rect;  rc.Right = rc.Left + smpl_wd;
	cv->FillRect(rc);
	cv->TextOut(xp, yp, smpl_str);

	SetHighlight(cv, State.Contains(odSelected));
	rc = Rect;  rc.Left += smpl_wd;
	cv->FillRect(rc);
	xp = rc.Left + 4;
	cv->TextOut(xp, yp, ext);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefExtColBtnClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = RefExtColPanel->Color;
	if (UserModule->ColorDlg->Execute()) RefExtColPanel->Color = UserModule->ColorDlg->Color;
}

//---------------------------------------------------------------------------
//拡張子の重複をチェック
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::CheckDuplExt(
	int skip_idx)	//!=-1 なら変更項目のインデックス	(default = -1: 追加)
{
	std::unique_ptr<TStringList> dpl_lst(new TStringList());
	TListBox *lp = ExtColListBox;

	if (USAME_TS(ExtColorEdit->Text, ".")) {
		for (int i=0,idx=-1; i<lp->Count; i++) {
			if (i==skip_idx) continue;
			if (USAME_TS(get_tkn_r(lp->Items->Strings[i], ","), ".")) {
				dpl_lst->Add(".");
				if (idx==-1 && skip_idx==-1) lp->ItemIndex = idx = i;
			}
		}
	}
	else {
		TStringDynArray x_lst = SplitString(ExtColorEdit->Text, ".");
		for (int i=0,idx=-1; i<lp->Count; i++) {
			if (i==skip_idx) continue;
			UnicodeString lbuf = get_tkn_r(lp->Items->Strings[i], ",");
			for (int j=0; j<x_lst.Length; j++) {
				UnicodeString fext = x_lst[j];
				if (test_FileExt(fext, lbuf) && dpl_lst->IndexOf(fext)==-1) {
					dpl_lst->Add(fext);
					if (idx==-1 && skip_idx==-1) lp->ItemIndex = idx = i;
				}
			}
		}
	}

	if (dpl_lst->Count>0) {
		UnicodeString msg = "[";
		for (int i=0; i<dpl_lst->Count; i++) {
			if (!msg.IsEmpty()) msg += " ";
			UnicodeString fext = dpl_lst->Strings[i];
			if (USAME_TS(fext, "."))
				msg += fext;
			else
				msg.cat_sprintf(_T(".%s"), fext.c_str());
		}
		msgbox_WARN(msg + "]はすでに登録されています。");
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//拡張子別配色の追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddExtColActionExecute(TObject *Sender)
{
	if (!CheckDuplExt()) {
		UnicodeString s;
		s.sprintf(_T("0x%06x,%s"), (int)RefExtColPanel->Color, ExtColorEdit->Text.c_str());
		ExtColListBox->Items->Add(s);
		ExtColListBox->ItemIndex = ExtColListBox->Count - 1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddExtColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !ExtColorEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//拡張子別配色の変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgExtColActionExecute(TObject *Sender)
{
	int idx = ExtColListBox->ItemIndex;  if (idx==-1) return;
	if (!CheckDuplExt(idx)) {
		UnicodeString s;
		s.sprintf(_T("0x%06x,%s"), (int)RefExtColPanel->Color, ExtColorEdit->Text.c_str());
		ExtColListBox->Items->Strings[idx] = s;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgExtColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !ExtColorEdit->Text.IsEmpty() && ExtColListBox->ItemIndex!=-1;
}

//---------------------------------------------------------------------------
//拡張子の検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindXColEditChange(TObject *Sender)
{
	UnicodeString kwd = FindXColEdit->Text;
	TListBox *lp = ExtColListBox;
	lp->ItemIndex = -1;
	int idx = -1;
	if (USAME_TS(kwd, ".")) {
		for (int i=0; i<lp->Count && idx==-1; i++)
			if (USAME_TS(get_tkn_r(lp->Items->Strings[i], ","), ".")) idx = i;
	}
	else {
		for (int i=0; i<lp->Count && idx==-1; i++)
			if (test_FileExt(kwd, get_tkn_r(lp->Items->Strings[i], ","))) idx = i;
	}

	lp->ItemIndex = idx;
	ExtColListBoxClick(lp);
}

//---------------------------------------------------------------------------
//タグ別配色一覧の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::TagColListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);

	UnicodeString tag = lp->Items->Names[Index];
	UnicodeString stt;
	TColor col = (TColor)lp->Items->ValueFromIndex[Index].ToIntDef(clNone);
	if (col==clNone) {
		col = (TColor)ColBufList->Values["fgTagNam"].ToIntDef(clBlack);;
		stt = "<デフォルト>";
	}

	TRect rc = Rect;
	UnicodeString blk = _T("\u2588 ");
	int blk_wd = cv->TextWidth(blk);
	rc.Right = rc.Left + blk_wd + MaxWd_Tag;
	cv->Brush->Color = col_bgList;
	cv->FillRect(rc);

	int xp = rc.Left + 4;
	int yp = Rect.Top + get_TopMargin(cv);
	cv->Font->Color = col;
	cv->TextOut(xp, yp, blk);
	xp += blk_wd;

	if (RevTagColCheckBox->Checked) {
		cv->Brush->Color = col;
		cv->Font->Color  = col_bgList;
	}
	else {
		cv->Font->Color = col;
	}
	cv->TextOut(xp, yp, tag);

	SetHighlight(cv, State.Contains(odSelected));
	rc = Rect;
	rc.Left += (blk_wd + MaxWd_Tag);
	cv->FillRect(rc);
	cv->TextOut(rc.Left + 8, yp, stt);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefTagColBtnClick(TObject *Sender)
{
	TListBox *lp = TagColListBox;
	int idx = lp->ItemIndex;	if (idx==-1) return;

	UnicodeString tag = lp->Items->Names[idx];
	UserModule->ColorDlg->Color = (TColor)lp->Items->ValueFromIndex[idx].ToIntDef(clBlack);
	if (UserModule->ColorDlg->Execute()) {
		UnicodeString lbuf;
		TagColListBox->Items->Strings[idx] = lbuf.sprintf(_T("%s=%u"), tag.c_str(), UserModule->ColorDlg->Color);
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DefTagColActionExecute(TObject *Sender)
{
	TListBox *lp = TagColListBox;
	if (lp->ItemIndex!=-1) {
		lp->Items->Values[lp->Items->Names[lp->ItemIndex]] = IntToStr(clNone);
		lp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DefTagColActionUpdate(TObject *Sender)
{
	TListBox *lp = TagColListBox;
	int idx = lp->ItemIndex;
	((TAction*)Sender)->Enabled = (idx!=-1 && lp->Items->ValueFromIndex[idx].ToIntDef(clNone) != clNone);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RevTagColCheckBoxClick(TObject *Sender)
{
	TagColListBox->Repaint();
}

//---------------------------------------------------------------------------
//配色の検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!Shift.Empty()) return;

	if (Key==VK_UP) {
		FindColUpAction->Execute();
		Key = 0;
	}
	else if (Key==VK_DOWN) {
		FindColDownAction->Execute();
		Key = 0;
	}
}
//---------------------------------------------------------------------------
//配色を下方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColDownActionExecute(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString kwd = FindColEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->ValueFromIndex[i], kwd)) idx = i;
	}
	if (idx!=-1) lp->ItemIndex = idx; else beep_Warn();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColDownActionUpdate(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString kwd = FindColEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && !found; i++)
			found = ContainsText(lp->Items->ValueFromIndex[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}
//---------------------------------------------------------------------------
//配色を上方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColUpActionExecute(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString kwd = FindColEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && idx==-1; i--)
			if (ContainsText(lp->Items->ValueFromIndex[i], kwd)) idx = i;
	}
	if (idx!=-1) lp->ItemIndex = idx; else beep_Warn();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColUpActionUpdate(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString kwd = FindColEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && !found; i--)
			found = ContainsText(lp->Items->ValueFromIndex[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindColEditChange(TObject *Sender)
{
	TListBox *lp = OptColListBox;
	UnicodeString kwd = FindColEdit->Text;
	if (!kwd.IsEmpty()) {
		int idx = -1;
		for (int i=0; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->ValueFromIndex[i], kwd)) idx = i;
		if (idx!=-1) lp->ItemIndex = idx;
	}
	else {
		lp->ItemIndex = -1;
	}
}

//---------------------------------------------------------------------------
//リストでのキー操作
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OptListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TControl *cp = (TControl*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if (cp==KeyListBox) {
		if (equal_DEL(KeyStr)) DelKeyAction->Execute(); else return;
	}
	else if (cp==PrtDirListBox) {
		if (equal_DEL(KeyStr)) UserModule->DelListItemAction->Execute(); else return;
	}
	else {
		if (equal_DEL(KeyStr)) {
			if (cp==EventListBox) {
				TListBox *lp = (TListBox*)cp;
				int idx = lp->ItemIndex;
				if (idx!=-1) lp->Items->Strings[idx] = get_pre_tab(lp->Items->Strings[idx]) + "\t";
			}
			else {
				UserModule->DelListItemAction->Execute();
			}
		}
		else if (!UserModule->ListBoxOpeItem(KeyStr)) {
			return;
		}
	}

	Key = 0;
}

				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI

//---------------------------------------------------------------------------
//その他のエディタの追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddEtcEditorActionExecute(TObject *Sender)
{
	EtcEditorListBox->Items->Add(
		UnicodeString().cat_sprintf(_T("%s=\"%s\""), FExtEtcEdit->Text.LowerCase().c_str(), EtcEditorEdit->Text.c_str()));
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddEtcEditorActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FExtEtcEdit->Text.IsEmpty() && !EtcEditorEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//その他のエディタの変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgEtcEditorActionExecute(TObject *Sender)
{
	int idx = EtcEditorListBox->ItemIndex;  if (idx==-1) return;
	EtcEditorListBox->Items->Strings[idx] =
		UnicodeString().cat_sprintf(_T("%s=\"%s\""), FExtEtcEdit->Text.LowerCase().c_str(), EtcEditorEdit->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgEtcEditorActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FExtEtcEdit->Text.IsEmpty() && !EtcEditorEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//エイリアス変更を反映
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::UpdateAlias(UnicodeString o_name, UnicodeString n_name)
{
	if (SameText(o_name, n_name)) return;

	UnicodeString tmp;
	//キー設定
	for (int i=0; i<MAX_KEYTABS; i++) {
		for (int j=0; j<KeyListBuf[i]->Count; j++) {
			UnicodeString vbuf = KeyListBuf[i]->ValueFromIndex[j];
			if (!remove_top_Dollar(vbuf)) continue;
			if (SameText(o_name, vbuf))
				KeyListBuf[i]->ValueFromIndex[j] = tmp.sprintf(_T("$%s"), n_name.c_str());
		}
	}
	//関連付け
	TListBox *lp = AssociateListBox;
	for (int i=0; i<lp->Count; i++) {
		UnicodeString vbuf = exclude_quot(lp->Items->ValueFromIndex[i]);
		if (!remove_top_Dollar(vbuf)) continue;
		if (SameText(o_name, vbuf))
			lp->Items->ValueFromIndex[i] = tmp.sprintf(_T("\"$%s\""), n_name.c_str());
	}
}
//---------------------------------------------------------------------------
//エイリアスに関連付けられている拡張子を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TOptionDlg::get_AliasInfo(UnicodeString alias)
{
	//キー
	UnicodeString k_inf;
	for (int i=0; i<MAX_KEYTABS && k_inf.IsEmpty(); i++) k_inf = get_Alias_KeyStr(alias, KeyListBuf[i]);

	//関連付け
	UnicodeString a_inf;
	TListBox *lp = AssociateListBox;
	for (int i=0; i<lp->Count; i++) {
		UnicodeString fext = lp->Items->Names[i];
		UnicodeString vbuf = exclude_quot(lp->Items->ValueFromIndex[i]);
		if (!remove_top_Dollar(vbuf)) continue;
		if (!SameText(alias, vbuf))   continue;
		if (USAME_TS(fext, "..")) a_inf += "[..]"; else a_inf.cat_sprintf(_T(".%s"), fext.c_str());
	}

	UnicodeString ret_str = k_inf;
	if (!a_inf.IsEmpty()) {
		if (!ret_str.IsEmpty()) ret_str += " / ";
		ret_str += a_inf;
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//エイリアスの重複メッセージを取得
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::CheckDuplAliasMsg(
	UnicodeString alias,	//エイリアス
	TCheckListBox *lp)		//変更対象リストボックス (追加の場合は NULL)
{
	if (alias.IsEmpty()) return false;

	UnicodeString mgs;
	mgs.sprintf(_T("エイリアス \"%s\" が\r\n"), alias.c_str());

	for (int i=0; i<ExtMenuListBox->Count; i++) {
		if (lp==ExtMenuListBox && i==lp->ItemIndex) continue;
		TStringDynArray itm_buf = get_csv_array(ExtMenuListBox->Items->Strings[i], EXTMENU_CSVITMCNT, true);
		if (SameText(alias, itm_buf[3])) {
			msgbox_WARN(mgs.cat_sprintf(_T("追加メニュー \"%s\" で重複しています。"), itm_buf[0].c_str()));
			return true;
		}
	}
	for (int i=0; i<ExtToolListBox->Count; i++) {
		if (lp==ExtToolListBox && i==lp->ItemIndex) continue;
		TStringDynArray itm_buf = get_csv_array(ExtToolListBox->Items->Strings[i], EXTTOOL_CSVITMCNT, true);
		if (SameText(alias, itm_buf[4])) {
			msgbox_WARN(mgs.cat_sprintf(_T("外部ツール \"%s\" で重複しています。"), itm_buf[0].c_str()));
			return true;
		}
	}

	return false;
}

//---------------------------------------------------------------------------
//外部ツール
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtToolListBoxClick(TObject *Sender)
{
	TCheckListBox *lp = ExtToolListBox;
	if (lp->Count==0) return;
	if (!Sender) lp->ItemIndex = -1;

	if (lp->ItemIndex!=-1) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[lp->ItemIndex], EXTTOOL_CSVITMCNT, true);
		ToolTitEdit->Text	= itm_buf[0];
		ToolExeEdit->Text	= itm_buf[1];
		ToolPrmEdit->Text	= itm_buf[2];
		ToolDirEdit->Text	= itm_buf[3];
		ToolAliasEdit->Text = itm_buf[4];
		ToolAliasLabel->Caption = get_AliasInfo(itm_buf[4]);
	}
	else {
		ToolTitEdit->Text	= EmptyStr;
		ToolExeEdit->Text	= EmptyStr;
		ToolPrmEdit->Text	= EmptyStr;
		ToolDirEdit->Text	= EmptyStr;
		ToolAliasEdit->Text = EmptyStr;
		ToolAliasLabel->Caption = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ToolTitEditChange(TObject *Sender)
{
	if (starts_tchs(_T("-<>"), ToolTitEdit->Text)) {
		ToolExeEdit->Text = EmptyStr;
		ToolPrmEdit->Text = EmptyStr;
		ToolDirEdit->Text = EmptyStr;
		if (starts_tchs(_T("-<"), ToolTitEdit->Text)) {
			ToolTitEdit->Text	= ToolTitEdit->Text.SubString(1, 1);
			ToolAliasEdit->Text = EmptyStr;
		}
	}
}

//---------------------------------------------------------------------------
//外部ツールの実行ファイルを設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetExtToolExeName(UnicodeString fnam)
{
	if (test_LnkExt(get_extension(fnam))) {
		UnicodeString xnam, prm, fld;
		int shw;
		usr_SH->get_LnkInf(fnam, NULL, &xnam, &prm, &fld, &shw);
		if (test_FileExt(get_extension(xnam), ".exe")) {
			ToolTitEdit->Text = get_base_name(fnam);
			ToolExeEdit->Text = xnam;
			ToolPrmEdit->Text = prm;
			ToolDirEdit->Text = fld;
			ToolAliasEdit->Text = EmptyStr;
		}
		else {
			msgbox_WARN("リンク先が .exe ファイルではありません");
		}
	}
	else {
		ToolExeEdit->Text = fnam;
	}
}
//---------------------------------------------------------------------------
//外部ツールの追加・変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChangeExtToolList(bool add)
{
	TCheckListBox *lp = ExtToolListBox;
	if (CheckDuplAliasMsg(ToolAliasEdit->Text, add? NULL : lp)) return;

	int  idx = lp->ItemIndex;
	bool chk = add? true : (idx!=-1)? lp->Checked[idx] : false;

	UnicodeString lbuf;
	lbuf.sprintf(_T("%s,%s,%s,%s,%s,%s"),
		make_csv_str(ToolTitEdit->Text).c_str(),	make_csv_str(ToolExeEdit->Text).c_str(),
		make_csv_str(ToolPrmEdit->Text).c_str(),	make_csv_str(ToolDirEdit->Text).c_str(),
		make_csv_str(ToolAliasEdit->Text).c_str(),	make_csv_str(chk).c_str());

	//追加
	if (add) {
		lp->Items->Add(lbuf);
		lp->Checked[lp->Count - 1] = true;
	}
	//変更
	else {
		if (idx==-1) return;
		UnicodeString last_alias = get_csv_item(lp->Items->Strings[idx], 4);
		lp->Items->Strings[idx] = lbuf;
		//エイリアス変更を反映
		UpdateAlias(last_alias, ToolAliasEdit->Text);
	}
}
//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddToolActionExecute(TObject *Sender)
{
	ChangeExtToolList(true);
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgToolActionExecute(TObject *Sender)
{
	ChangeExtToolList(false);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgToolActionUpdate(TObject *Sender)
{
	bool is_special = starts_tchs(_T("-<>"), ToolTitEdit->Text);
	((TAction*)Sender)->Enabled = is_special || (!ToolTitEdit->Text.IsEmpty() && !ToolExeEdit->Text.IsEmpty());
	ToolAliasEdit->Enabled = !starts_tchs(_T("-<"), ToolTitEdit->Text);
	ToolExeEdit->Enabled   = !is_special;
	ToolPrmEdit->Enabled   = !is_special;
	ToolDirEdit->Enabled   = !is_special;
}

//---------------------------------------------------------------------------
//外部ツールの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefExeBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("外部ツール"), F_FILTER_EXE2);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) SetExtToolExeName(fnam);
}
//---------------------------------------------------------------------------
//外部ツールパラメータ書式文字列の参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefToolFmtBtnClick(TObject *Sender)
{
	TPoint p = ExtToolPanel->ClientToScreen(Point(RefToolFmtBtn->Left + 8, RefToolFmtBtn->Top + 8));
	ToolPopupMenu->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FrmtItemClick(TObject *Sender)
{
	ToolPrmEdit->SelText = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
}
//---------------------------------------------------------------------------
//外部ツールの作業ディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefDirBtnClick(TObject *Sender)
{
	UnicodeString dnam = ExePath;
	if (UserModule->SelectDirEx(_T("作業ディレクトリ"), dnam)) ToolDirEdit->Text = dnam;
}

//---------------------------------------------------------------------------
//追加メニュー
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtMenuListBoxClick(TObject *Sender)
{
	TCheckListBox *lp = ExtMenuListBox;
	if (lp->Count==0) return;
	if (!Sender) lp->ItemIndex = -1;

	if (lp->ItemIndex!=-1) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[lp->ItemIndex], EXTMENU_CSVITMCNT, true);
		MenuTitEdit->Text = itm_buf[0];
		if (starts_tchs(_T("-<"), MenuTitEdit->Text)) {
			MenuAliasEdit->Text 	   = EmptyStr;
			MenuAliasLabel->Caption    = EmptyStr;
			MenuCmdComboBox->ItemIndex = -1;
			MenuPrmEdit->Text		   = EmptyStr;
			MenuIconEdit->Text		   = EmptyStr;
		}
		else {
			TComboBox *cp = MenuCmdComboBox;
			for (int i=0; i<cp->Items->Count; i++) {
				if (StartsText(itm_buf[1] + " ", cp->Items->Strings[i])) {
					cp->ItemIndex = i;  break;
				}
			}
			MenuPrmEdit->Text		= itm_buf[2];
			MenuAliasEdit->Text 	= itm_buf[3];
			MenuAliasLabel->Caption = get_AliasInfo(itm_buf[3]);
			MenuIconEdit->Text		= itm_buf[5];
		}
	}
	else {
		MenuTitEdit->Text		   = EmptyStr;
		MenuAliasEdit->Text 	   = EmptyStr;
		MenuAliasLabel->Caption    = EmptyStr;
		MenuCmdComboBox->ItemIndex = -1;
		MenuPrmEdit->Text		   = EmptyStr;
		MenuIconEdit->Text		   = EmptyStr;
	}

	RefMenuPrmBtn->Enabled = USAME_TI(get_tkn(MenuCmdComboBox->Text, ' '), "ExeCommands");
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::MenuCmdComboBoxChange(TObject *Sender)
{
	RefMenuPrmBtn->Enabled = USAME_TI(get_tkn(MenuCmdComboBox->Text, ' '), "ExeCommands");
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::MenuTitEditChange(TObject *Sender)
{
	if (starts_tchs(_T("-<>"), MenuTitEdit->Text)) {
		MenuCmdComboBox->ItemIndex = -1;
		MenuPrmEdit->Text	= EmptyStr;
		if (starts_tchs(_T("-<"), MenuTitEdit->Text)) {
			MenuTitEdit->Text	= MenuTitEdit->Text.SubString(1, 1);
			MenuAliasEdit->Text = EmptyStr;
		}
	}
}

//---------------------------------------------------------------------------
//メニュー項目の追加・変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChangeExtMenuList(bool add)
{
	TCheckListBox *lp = ExtMenuListBox;
	if (CheckDuplAliasMsg(MenuAliasEdit->Text, add? NULL : lp)) return;

	int  idx = lp->ItemIndex;
	bool chk = add? true : (idx!=-1)? lp->Checked[idx] : false;

	UnicodeString lbuf;
	lbuf.sprintf(_T("%s,%s,%s,%s,%s,%s"),
		make_csv_str(MenuTitEdit->Text).c_str(), make_csv_str(get_tkn(MenuCmdComboBox->Text, ' ')).c_str(),
		make_csv_str(MenuPrmEdit->Text).c_str(), make_csv_str(MenuAliasEdit->Text).c_str(),
		make_csv_str(chk).c_str(), make_csv_str(MenuIconEdit->Text).c_str());

	if (add) {
		lp->Items->Add(lbuf);
		lp->Checked[lp->Count - 1] = true;
	}
	else {
		if (idx==-1) return;
		UnicodeString last_alias = get_csv_item(lp->Items->Strings[idx], 3);
		lp->Items->Strings[idx] = lbuf;
		//エイリアス変更を反映
		UpdateAlias(last_alias, MenuAliasEdit->Text);
	}
}
//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddMenuActionExecute(TObject *Sender)
{
	ChangeExtMenuList(true);
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgMenuActionExecute(TObject *Sender)
{
	ChangeExtMenuList(false);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgMenuActionUpdate(TObject *Sender)
{
	bool is_special = starts_tchs(_T("-<>"), MenuTitEdit->Text);
	((TAction*)Sender)->Enabled = is_special ||
		(!MenuTitEdit->Text.IsEmpty() && !MenuCmdComboBox->Text.IsEmpty() && !MenuPrmEdit->Text.IsEmpty());
	MenuAliasEdit->Enabled   = !starts_tchs(_T("-<"), MenuTitEdit->Text);
	MenuCmdComboBox->Enabled = !is_special;
	MenuPrmEdit->Enabled	 = !is_special;
}

//---------------------------------------------------------------------------
//パラメータファイルの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefCmdsBtnClick(TObject *Sender)
{
	UnicodeString subtit;
	if (PageControl1->ActivePage==ExtMenuSheet) {
		if (!MenuTitEdit->Text.IsEmpty()) subtit.sprintf(_T(" - [%s]"), MenuTitEdit->Text.c_str());
	}
	else if (PageControl1->ActivePage==EventSheet) {
		int idx = EventListBox->ItemIndex;
		if (idx!=-1) subtit.sprintf(_T(" - [%s]"), get_pre_tab(EventListBox->Items->Strings[idx]).c_str());
	}

	UnicodeString cmd;
	if		(PageControl1->ActivePage==ExtMenuSheet) cmd = get_CmdStr(MenuPrmEdit->Text);
	else if (PageControl1->ActivePage==EventSheet)	 cmd = get_CmdStr(EventCmdsEdit->Text);

	//通常ファイルの選択
	if (contained_wd_i(
		_T("ContextMenu|ExeMenuFile|FileEdit|LoadWorkList|OpenByApp|OpenByWin|OpenStandard|SelByList|SetFolderIcon|TextViewer"),
			cmd))
	{
		UnicodeString tit = "パラメータの指定" + subtit;
		UserModule->PrepareOpenDlg(tit.c_str(), F_FILTER_ALL, NULL, RefParamPath);
		UnicodeString fnam;
		if (UserModule->OpenDlgToStr(fnam)) {
			RefParamPath = ExtractFilePath(fnam);
			if (contained_wd_i(_T("ContextMenu|ExeMenuFile|LoadWorkList|SelByList|SetFolderIcon"), cmd))
				fnam = to_relative_name(fnam);
			else if (remove_top_text(fnam, ExePath))
				fnam.Insert("%ExePath%", 1);
			cmd.cat_sprintf(_T("_\"%s\""), fnam.c_str());

			if (PageControl1->ActivePage==ExtMenuSheet) {
				MenuPrmEdit->Text = cmd;
			}
			else if (PageControl1->ActivePage==EventSheet) {
				EventCmdsEdit->Text = cmd;
				int idx = EventListBox->ItemIndex;
				if (idx!=-1)
					EventListBox->Items->Strings[idx] = get_pre_tab(EventListBox->Items->Strings[idx]) + "\t" + cmd;
			}
		}
	}
	//コマンドファイルの選択
	else if (CmdFileListDlg->ShowToSelect()==mrOk) {
		UnicodeString fnam = CmdFileListDlg->CmdFileName;
		fnam = "@" + to_relative_name(fnam);
		if (PageControl1->ActivePage==ExtMenuSheet) {
			MenuPrmEdit->Text = fnam;
		}
		else if (PageControl1->ActivePage==EventSheet) {
			switch (((TComponent*)Sender)->Tag) {
			case 100:
				EventCmdsEdit->Text = fnam;
				if (EventListBox->ItemIndex!=-1)
					EventListBox->Items->Strings[EventListBox->ItemIndex]
						 = get_pre_tab(EventListBox->Items->Strings[EventListBox->ItemIndex]) + "\t" + fnam;
				break;
			case 101: TimerCmd1Edit->Text = fnam;	break;
			case 102: TimerCmd2Edit->Text = fnam;	break;
			}
		}
	}
}
//---------------------------------------------------------------------------
//アイコンの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefMenuIconBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("アイコンの指定"), F_FILTER_ICO, NULL, IconFilePath);
	if (UserModule->OpenDlgToEdit(MenuIconEdit, true)) IconFilePath = ExtractFilePath(to_absolute_name(MenuIconEdit->Text));
}

//---------------------------------------------------------------------------
//関連付け
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AssociateListBoxClick(TObject *Sender)
{
	TListBox *lp = AssociateListBox;
	if (!Sender) lp->ItemIndex = -1;

	if (lp->ItemIndex!=-1) {
		AsoExtEdit->Text	 = AssociateListBox->Items->Names[lp->ItemIndex];
		AsoAppComboBox->Text = exclude_quot(AssociateListBox->Items->ValueFromIndex[lp->ItemIndex]);
	}
	else {
		AsoExtEdit->Text	 = EmptyStr;
		AsoAppComboBox->Text = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AssociateListBoxMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	if (Button!=mbRight) return;
	TListBox *lp = AssociateListBox;
	int idx = lp->ItemIndex;  if (idx==-1) return;

	TStringDynArray app_lst;
	UnicodeString fext = lp->Items->Names[idx];
	for (int i=0; i<lp->Count; i++) {
		if (test_FileExt(fext, lp->Items->Names[i]))
			add_dyn_array(app_lst, exclude_quot(lp->Items->ValueFromIndex[i]));
	}

	if (app_lst.Length>1) {
		std::unique_ptr<TStringList> m_lst(new TStringList());
		make_AssoMenuList(app_lst, m_lst.get());
		//テストメニューを作成
		TestPopupMenu->Items->Clear();
		TestPopupMenu->AutoHotkeys = AutoHotkeyCheckBox->Checked? maAutomatic : maManual;
		IconImgListP->Clear();
		for (int i=0; i<m_lst->Count; i++) {
			TStringDynArray m_buf = split_strings_tab(Trim(m_lst->Strings[i]));
			if (m_buf.Length>0) {
				TMenuItem *mp  = new TMenuItem(TestPopupMenu);
				mp->Caption    = m_buf[0];
				mp->ImageIndex = (m_buf.Length==3)? add_IconImage(m_buf[2], IconImgListP) : -1;
				TestPopupMenu->Items->Add(mp);
			}
		}
		//テストメニューを表示
		TestPopupMenu->Popup(Mouse->CursorPos.x, Mouse->CursorPos.y);
	}
}

//---------------------------------------------------------------------------
//関連付け一覧のソート (同じ拡張子内はそのまま)
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SortAssoListBox(UnicodeString item)
{
	TListBox *lp = AssociateListBox;
	if (lp->Count<2) return;

	std::unique_ptr<TStringList> x_lst(new TStringList());
	for (int i=0; i<lp->Count; i++) {
		UnicodeString lbuf = lp->Items->Strings[i];
		UnicodeString fext = lp->Items->Names[i];
		TStringList *o_lst;
		int idx = x_lst->IndexOf(fext);
		if (idx==-1) {
			o_lst = new TStringList();
			x_lst->AddObject(fext, (TObject*)o_lst);
		}
		else {
			o_lst = (TStringList*)x_lst->Objects[idx];
		}
		o_lst->Add(lbuf);
	}
	x_lst->Sort();

	lp->Clear();
	for (int i=0; i<x_lst->Count; i++) {
		TStringList *o_lst = (TStringList*)x_lst->Objects[i];
		for (int j=0; j<o_lst->Count; j++) lp->Items->Add(o_lst->Strings[j]);
		delete o_lst;
	}

	if (!item.IsEmpty()) {
		int idx = lp->Items->IndexOf(item);
		if (idx!=-1) lp->ItemIndex = idx;
	}
}

//---------------------------------------------------------------------------
//関連付け項目の追加・変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChangeAssociateList(bool add)
{
	if (AsoExtEdit->Text.IsEmpty() || AsoAppComboBox->Text.IsEmpty()) return;

	UnicodeString s = AsoExtEdit->Text.LowerCase();
	if (!USAME_TS(s, "..")) remove_top_s(s, '.');
	s.cat_sprintf(_T("=\"%s\""), AsoAppComboBox->Text.c_str());

	//追加
	if (add) {
		AssociateListBox->Items->Add(s);
		if (AsoSortCheckBox->Checked) SortAssoListBox(s);
	}
	//変更
	else {
		if (AssociateListBox->ItemIndex==-1) return;
		AssociateListBox->Items->Strings[AssociateListBox->ItemIndex] = s;
		if (AsoSortCheckBox->Checked) SortAssoListBox(s);
	}

	MaxWd_AssExt = 0;
	for (int i=0; i<AssociateListBox->Count; i++)
		MaxWd_AssExt = std::max(AssociateListBox->Canvas->TextWidth(AssociateListBox->Items->Names[i]), MaxWd_AssExt);
	MaxWd_AssExt += ScaledInt(20);
}

//---------------------------------------------------------------------------
//関連付けの参照内容を設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoAppComboBoxDropDown(TObject *Sender)
{
	std::unique_ptr<TStringList> cmd_lst(new TStringList());
	for (int i=0; i<AssociateListBox->Count; i++) {
		UnicodeString lbuf = exclude_quot(AssociateListBox->Items->ValueFromIndex[i]);
		if (!StartsStr('$', lbuf) && !USAME_TS(lbuf, "-")) cmd_lst->Add(lbuf);
	}
	cmd_lst->Sort();

	//重複削除
	if (cmd_lst->Count>1) {
		UnicodeString laststr = cmd_lst->Strings[0];
		int i = 1;
		while (i<cmd_lst->Count) {
			UnicodeString lbuf = cmd_lst->Strings[i];
			if (SameStr(laststr, lbuf)) {
				cmd_lst->Delete(i);
			}
			else {
				laststr = lbuf;
				i++;
			}
		}
	}

	//エイリアス
	for (int i=0; i<ExtMenuListBox->Count; i++) {
		UnicodeString itm = get_csv_item(ExtMenuListBox->Items->Strings[i], 3);
	 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
	}
	for (int i=0; i<ExtToolListBox->Count; i++) {
		UnicodeString itm = get_csv_item(ExtToolListBox->Items->Strings[i], 4);
	 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
	}

	AsoAppComboBox->Items->Assign(cmd_lst.get());
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddAssoActionExecute(TObject *Sender)
{
	ChangeAssociateList(true);
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgAssoActionExecute(TObject *Sender)
{
	ChangeAssociateList(false);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgAssoActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !AsoExtEdit->Text.IsEmpty() && !AsoAppComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//関連付け実行ファイルの選択
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AsoRefBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("関連付け"), F_FILTER_EXE2 _T("|") F_FILTER_NBT);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) SetExtNameToCtrl(fnam, AsoAppComboBox, true);
}

//---------------------------------------------------------------------------
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OptListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);

	if (lp==EventListBox && USAME_TI(EventCmdList[Index].key, "OnNewDrive") && !State.Contains(odSelected)) {
		cv->Brush->Color = !OpenAddDrvCheckBox->Checked? col_Invalid : scl_Window;
		cv->Font->Color  = scl_WindowText;
	}
	else {
		SetHighlight(cv, State.Contains(odSelected));
	}
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);
	UnicodeString lbuf = lp->Items->Strings[Index];
	bool brk = false;

													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
													//<<<<<<<X86_SPI
	//イベント										//<<<<<<<X86_SPI
	if (lp==EventListBox) { 						//<<<<<<<X86_SPI
		UnicodeString tmp = split_pre_tab(lbuf);
		brk = remove_top_s(tmp, '|');
		cv->TextOut(xp, yp, tmp);
		cv->TextOut(xp + MaxWd_Ev, yp, lbuf);
	}
	//仮想ドライブ
	else if (lp==VirDrvListBox) {
		cv->TextOut(xp, yp, split_tkn(lbuf, '=') + ": => ");
		xp += cv->TextWidth("W: => ");
		cv->TextOut(xp, yp, lbuf);
	}
	//関連付け
	else {
		if (Index>0) brk = !SameText(lp->Items->Names[Index], lp->Items->Names[Index-1]);
		cv->TextOut(xp, yp, split_tkn(lbuf, '='));
		cv->TextOut(xp + MaxWd_AssExt, yp, exclude_quot(lbuf));
	}

	//境界線(上辺)
	if (brk) draw_separateLine(cv, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OptMenuListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	bool is_tool = (lp==ExtToolListBox);
	THeaderSections *sp = ((THeaderControl *)(is_tool? ExtToolHeader : ExtMenuHeader))->Sections;

	TCanvas *cv  = lp->Canvas;
	cv->Font->Assign(lp->Font);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	//項目の階層を調べる
	int sub_lvl = 0;
	for (int i=0; i<=Index; i++) {
		UnicodeString itm = get_csv_item(lp->Items->Strings[i], 0);
		if (i<Index && StartsStr('>', itm))
			sub_lvl++;
		else if (USAME_TS(itm, "<") && sub_lvl>0)
			sub_lvl--;
	}

	int xp = Rect.Left + Scaled2 + ScaledInt(18) + (sub_lvl * cv->TextWidth("W"));
	int yp = Rect.Top  + get_TopMargin(cv);

	UnicodeString lbuf = lp->Items->Strings[Index];
	TStringDynArray itm_buf = get_csv_array(lbuf, (is_tool? EXTTOOL_CSVITMCNT : EXTMENU_CSVITMCNT), true);
	//セパレータ
	if (is_separator(itm_buf[0])) {
		TRect rc = Rect;  rc.Left = xp - 4;
		draw_Separator(cv, rc);
	}
	//項目
	else {
		//アイコン
		usr_SH->draw_SmallIcon(to_absolute_name(cv_env_str(itm_buf[is_tool? 1 : 5])), cv, Rect.Left + 2, yp);
		//キャプション
		UnicodeString lbuf = minimize_str(itm_buf[0], cv, sp->Items[0]->Width - xp, true);
		cv->TextOut(xp, yp, lbuf);
		//エイリアス
		if (!starts_tchs(_T("-<"), lbuf)) {
			cv->TextOut(sp->Items[0]->Width, yp, minimize_str(itm_buf[is_tool? 4 : 3], cv, sp->Items[1]->Width, true));
		}
		//設定
		if (!starts_tchs(_T("-<>"), lbuf)) {
			lbuf = EmptyStr;
			for (int i=1; i<(is_tool? 4 : 3); i++) {
				if (i>1) lbuf += ", ";
				lbuf += itm_buf[i];
			}
			cv->TextOut(sp->Items[0]->Width + sp->Items[1]->Width, yp, lbuf);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtMenuHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	ExtMenuListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExtToolHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	ExtToolListBox->Repaint();
}

//---------------------------------------------------------------------------
//キー設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyTabControlDrawTab(TCustomTabControl *Control, int TabIndex,
	const TRect &Rect, bool Active)
{
	//※テーマ利用時に下部タブが正しく描画されない不具合の対策
	draw_BottomTab(Control, TabIndex, Rect, Active);
}
//---------------------------------------------------------------------------
//カテゴリー変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyTabControlChanging(TObject *Sender, bool &AllowChange)
{
	KeyListBuf[KeyTabControl->TabIndex]->Assign(KeyListBox->Items);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyTabControlChange(TObject *Sender)
{
	KeyListBox->Items->Assign(KeyListBuf[KeyTabControl->TabIndex]);

	Key2ComboBox->ItemIndex = -1;
	Key2ComboBox->Enabled	= (KeyTabControl->TabIndex==0 || KeyTabControl->TabIndex==2 || KeyTabControl->TabIndex==3);
	SelCheckBox->Enabled	= (KeyTabControl->TabIndex==0);

	KeyFilterEdit->Text = EmptyStr;
	SortKeyListBox();
	SetCmdCombo();
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyHeaderControlDrawSection(THeaderControl *HeaderControl,
		  THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect, (Section->Index==KeySortMode)? 1 : 0, true);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyHeaderControlSectionClick(THeaderControl *HeaderControl,
		  THeaderSection *Section)
{
	KeySortMode = Section->Index;
	SortKeyListBox();
}

//---------------------------------------------------------------------------
//コマンド(CmdComboBox)の項目を設定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SetCmdCombo(
	UnicodeString kwd)		//フィルタ文字列 (default = EmptyStr : 絞り込み無し)
{
	PrmComboBox->Text	 = EmptyStr;
	PrmComboBox->Enabled = false;

	TStrings *sp = CmdComboBox->Items;
	sp->Clear();
	if (KeyTabControl->TabIndex==-1) return;

	std::unique_ptr<TStringList> cmd_lst(new TStringList());
	UnicodeString modstr = GetCmdModeStr();
	for (int i=0; i<CmdSetList->Count; i++) {
		UnicodeString lbuf = CmdSetList->Strings[i];
		if (StartsText(modstr, lbuf)) cmd_lst->Add(get_tkn_r(lbuf, ':'));
	}

	//追加メニュー/外部ツールのエイリアス
	if (KeyTabControl->TabIndex==0 || KeyTabControl->TabIndex==2 || KeyTabControl->TabIndex==3) {
		for (int i=0; i<ExtMenuListBox->Count; i++) {
			UnicodeString itm = get_csv_item(ExtMenuListBox->Items->Strings[i], 3);
		 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
		}
		for (int i=0; i<ExtToolListBox->Count; i++) {
			UnicodeString itm = get_csv_item(ExtToolListBox->Items->Strings[i], 4);
		 	if (!itm.IsEmpty()) cmd_lst->Add("$" + itm);
		}
	}

	if (UnRegCmdCheckBox->Checked) {
		TListBox *lp = KeyListBox;
		//登録済コマンドを抽出
		std::unique_ptr<TStringList> reg_lst(new TStringList());
		for (int i=0; i<lp->Count; i++) {
			if (StartsText(modstr, lp->Items->Strings[i])) reg_lst->Add(lp->Items->ValueFromIndex[i]);
		}
		//未登録項目のみを残す
		int i = 0;
		while (i<cmd_lst->Count) {
			bool found = false;
			for (int j=0; j<reg_lst->Count && !found; j++)
				found = SameText(cmd_lst->Strings[i], get_CmdStr(reg_lst->Strings[j]));
			if (found) cmd_lst->Delete(i); else i++;
		}
		//現在位置の項目を追加
		int idx = lp->ItemIndex;
		if (idx!=-1) {
			cmd_lst->Add(get_CmdStr(lp->Items->ValueFromIndex[idx]));
			cmd_lst->Sort();
		}
	}

	//フィルタ
	bool is_TV = USAME_TI(modstr, "V:");
	if (!kwd.IsEmpty()) {
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Assign(cmd_lst.get());
		UnicodeString ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Checked, kwd);
		if (!ptn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			int i=0;
			while (i<lst->Count) {
				UnicodeString lbuf = get_CmdDesc(lst->Strings[i], false, ExtMenuListBox->Items, ExtToolListBox->Items, is_TV);
				if (!TRegEx::IsMatch(lbuf, ptn, opt)) lst->Delete(i); else i++;
			}
		}
		cmd_lst->Assign(lst.get());
	}

	//ドロップダウン項目を割り当て
	for (int i=0; i<cmd_lst->Count; i++) {
		sp->Add(get_CmdDesc(cmd_lst->Strings[i], false, ExtMenuListBox->Items, ExtToolListBox->Items, is_TV));
	}
}

//---------------------------------------------------------------------------
//コマンドコンボボックスの描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::CmdComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas   *cv = cp->Canvas;
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + get_TopMargin(cv);

	UnicodeString lbuf = cp->Items->Strings[Index];
	UnicodeString cmd  = split_tkn(lbuf, ' ');

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(xp, yp, cmd);
	xp += cv->TextHeight("Q")*10;
	cv->TextOut(xp, yp, Trim(lbuf));
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::CmdComboBoxChange(TObject *Sender)
{
	UnicodeString cmd = get_tkn(CmdComboBox->Text," ");

	//パラメータ項目を設定
	std::unique_ptr<TStringList> p_list(new TStringList());
	get_PrmList(cmd, KeyTabControl->TabIndex, p_list.get(), PrmComboBox);

	//動的な項目を設定
	UnicodeString tmp;
	if (contained_wd_i(_T("ChangeRegDir|ChangeOppRegDir"), cmd)) {
		for (int i=0; i<RegDirList->Count; i++) {
			TStringDynArray itm_buf = get_csv_array(RegDirList->Strings[i], 3);
			if (itm_buf.Length!=3 || itm_buf[0].IsEmpty()) continue;
			p_list->Add(tmp.sprintf(_T("%s : %s"), itm_buf[0].UpperCase().c_str(), itm_buf[1].c_str()));
		}
	}
	else if (USAME_TI(cmd, "Eject")) {
		p_list->Add(EmptyStr);
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
			if (dp->drv_type==DRIVE_CDROM) p_list->Add(get_tkn(dp->drive_str, ':'));
		}
	}
	else if (contained_wd_i(_T("ExeExtMenu|ExeExtTool"), cmd)) {
		TStringList *lst = USAME_TI(cmd, "ExeExtMenu")? ExtMenuList : ExtToolList;
		for (int i=0; i<lst->Count; i++) {
			UnicodeString itm = get_csv_item(lst->Strings[i], 0);
			if (!ContainsStr(itm, "&")) continue;
			UnicodeString ak = get_tkn_r(itm, '&').SubString(1, 1);
			if (ak.IsEmpty() || !_istalnum(ak[1])) continue;
			p_list->Add(tmp.sprintf(_T("%s : %s"), ak.UpperCase().c_str(), itm.c_str()));
		}
	}
	else if (USAME_TI(cmd, "ExeToolBtn")) {
		TStringList *lst= ToolBtnList;
		for (int i=0; i<lst->Count; i++) {
			TStringDynArray itm_buf = get_csv_array(lst->Strings[i], 2, true);
			if (is_separator(itm_buf[0])) continue;
			p_list->Add(tmp.sprintf(_T("%u : %s"), i + 1, (!itm_buf[0].IsEmpty()? itm_buf[0] : itm_buf[1]).c_str()));
		}
	}
	else if (USAME_TI(cmd, "ToTab")) {
		p_list->Add(EmptyStr);
		for (int i=0; i<TabList->Count; i++) {
			UnicodeString s = get_csv_item(TabList->Strings[i], 2);
			if (!s.IsEmpty()) p_list->Add(s);
		}
	}

	PrmComboBox->Items->Assign(p_list.get());
	PrmComboBox->PopupMenu = (PrmComboBox->Style==csDropDown)? UserModule->EditPopupMenuC : NULL;

	UserModule->CmdParamList->Clear();
	if (PrmComboBox->PopupMenu) {
		for (int i=0; i<PrmComboBox->Items->Count; i++) {
			UnicodeString lbuf = PrmComboBox->Items->Strings[i];
			if (!lbuf.IsEmpty()) UserModule->CmdParamList->Add(lbuf);
		}
	}

	if (PrmComboBox->Items->Count>0) {
		PrmComboBox->Enabled   = true;
		PrmComboBox->ItemIndex = 0;
	}

	RefCmdPrmBtn->Enabled = need_RefDirFile(cmd);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::UnRegCmdCheckBoxClick(TObject *Sender)
{
	SetCmdCombo();
	KeyListBoxClick(KeyListBox);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyListBoxClick(TObject *Sender)
{
	inh_KeySet++;

	if (!KeyFilterEdit->Text.IsEmpty() || UnRegCmdCheckBox->Checked) {
		KeyFilterEdit->Text = EmptyStr;
		SetCmdCombo();
	}

	TListBox *lp = (TListBox *)Sender;
	int idx = lp->ItemIndex;  if (idx==-1) return;
	UnicodeString kstr = get_tkn_r(lp->Items->Names[idx], ':');
	UnicodeString fstr = lp->Items->ValueFromIndex[idx];

	ShiftCheckBox->Checked = remove_text(kstr, KeyStr_Shift);
	CtrlCheckBox->Checked  = remove_text(kstr, KeyStr_Ctrl);
	AltCheckBox->Checked   = remove_text(kstr, KeyStr_Alt);
	SelCheckBox->Checked   = remove_text(kstr, KeyStr_SELECT);

	if (ContainsStr(kstr, "~")) {
		//2ストローク
		KeyComboBox->ItemIndex	= KeyComboBox->Items->IndexOf(get_tkn(kstr, '~'));
		Key2ComboBox->ItemIndex = Key2ComboBox->Items->IndexOf(get_tkn_r(kstr, '~'));
	}
	else {
		KeyComboBox->ItemIndex	= KeyComboBox->Items->IndexOf(kstr);
		Key2ComboBox->ItemIndex = 0;
	}

	//コマンド
	UnicodeString cmd = fstr;
	UnicodeString prm;
	if (!starts_Dollar(cmd)) {
		prm = get_PrmStr(cmd);
		cmd = get_CmdStr(cmd);
	}

	TComboBox *cp = CmdComboBox;
	for (int i=0; i<cp->Items->Count; i++) {
		if (StartsText(cmd + " ", cp->Items->Strings[i])) {
			cp->ItemIndex = i;
			CmdComboBoxChange(cp);
			break;
		}
	}

	//パラメータ
	cp = PrmComboBox;
	if (cp->Enabled) {
		if (cp->Style==csDropDown) {
			cp->Text = prm;
		}
		else {
			for (int i=0; i<cp->Items->Count; i++) {
				if (SameText(prm, get_tkn(cp->Items->Strings[i], ' '))) {
					cp->ItemIndex = i; break;
				}
			}
		}
	}

	inh_KeySet--;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyListBoxDblClick(TObject *Sender)
{
	TListBox *lp = (TListBox *)Sender;
	int idx = lp->ItemIndex;  if (idx==-1) return;

	UnicodeString kstr = lp->Items->Names[idx];
	TRegExOptions opt; opt << roIgnoreCase;
	if (TRegEx::IsMatch(kstr, ":(Shift\\+|Ctrl\\+|Alt\\+)*F\\d+", opt)) {
		UnicodeString lbl  = FKeyLabelBuf->Values[kstr];
		UnicodeString tit = "ファンクションキー表示名の変更";
		tit.cat_sprintf(_T(" - %s"), KeyListBuf[KeyTabControl->TabIndex]->Values[kstr].c_str());
		UnicodeString prm;  prm.sprintf(_T("[%s]"), get_tkn_r(kstr, ":").c_str());
		if (input_query_ex(tit.c_str(), prm.c_str(), &lbl)) {
			FKeyLabelBuf->Values[kstr] = lbl;
			lp->Invalidate();
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeySetClick(TObject *Sender)
{
	if (inh_KeySet>0) return;

	UnicodeString kbuf = GetCmdKeyStr() + "=";

	TListBox *lp = KeyListBox;
	int idx = -1;
	for (int i=0; i<lp->Count; i++) {
		if (StartsText(kbuf, lp->Items->Strings[i])) { idx = i;  break; }
	}

	if (idx!=-1) {
		lp->ItemIndex = idx;
		KeyListBoxClick(lp);
	}
}

//---------------------------------------------------------------------------
//フィルタ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyFilterEdit_Change(TObject *Sender)
{
	UnicodeString kwd = ((TEdit*)Sender)->Text;
	cursor_Default();
	set_RedrawOff(CmdComboBox);
	{
		CmdComboBox->DroppedDown = false;
		SetCmdCombo(kwd);
		if (!kwd.IsEmpty()) {
			CmdComboBox->DroppedDown = true;
			CmdComboBox->ItemIndex	 = 0;
			CmdComboBoxChange(CmdComboBox);
		}
	}
	set_RedrawOn(CmdComboBox);
	Screen->Cursor = crArrow;	//※ドロップダウン時にカーソルが消える現象への対策
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyFilterEdit_KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if (contained_wd_i(KeysStr_CsrDown, KeyStr)) {
		CmdComboBox->SetFocus();
		perform_Key(CmdComboBox, VK_DOWN);
	}
	else if (equal_ENTER(KeyStr)) {
		//確定
		if (CmdComboBox->DroppedDown) {
			CmdComboBox->SetFocus();
			perform_Key_RETURN(CmdComboBox);
		}
	}
	else if	(equal_ESC(KeyStr) && !KeyFilterEdit->Text.IsEmpty()) {
		KeyFilterEdit->Text = EmptyStr;
	}
	else if (SameText(KeyStr, KeyStr_Migemo)) {
		MigemoCheckBox->Checked = !MigemoCheckBox->Checked;
	}
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyFilterEdit_KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
//キー検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyBtnClick(TObject *Sender)
{
	if (!FindKeyDlg) FindKeyDlg = new TFindKeyDlg(this);	//初回に動的作成

	FindKeyDlg->KeyList->Assign(KeyListBox->Items);
	if (FindKeyDlg->ShowModal()==mrOk) {
		UnicodeString kstr = FindKeyDlg->KeyStr;
		ShiftCheckBox->Checked = remove_text(kstr, KeyStr_Shift);
		CtrlCheckBox->Checked  = remove_text(kstr, KeyStr_Ctrl);
		AltCheckBox->Checked   = remove_text(kstr, KeyStr_Alt);

		if (ContainsStr(kstr, "~")) {
			//2ストローク
			KeyComboBox->ItemIndex	= KeyComboBox->Items->IndexOf(get_tkn(kstr, '~'));
			Key2ComboBox->ItemIndex = Key2ComboBox->Items->IndexOf(get_tkn_r(kstr, '~'));
		}
		else {
			KeyComboBox->ItemIndex	= KeyComboBox->Items->IndexOf(kstr);
			Key2ComboBox->ItemIndex = 0;
		}

		TListBox *lp = KeyListBox;
		int idx = -1;
		kstr = FindKeyDlg->KeyStr + "=";
		for (int i=0; i<lp->Count; i++) {
			if (StartsText(kstr, get_tkn_r(lp->Items->Strings[i], ':'))) { idx = i;  break; }
		}

		lp->ItemIndex = idx;
		if (idx!=-1) {
			KeyListBoxClick(lp);
		}
		else {
			CmdComboBox->ItemIndex = -1;
			PrmComboBox->Items->Clear();
			PrmComboBox->Text = EmptyStr;
		}
	}
}

//---------------------------------------------------------------------------
//キー設定の検索
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::MatchKeyItem(int idx)
{
	bool found = false;
	TListBox *lp = KeyListBox;
	UnicodeString kwd = FindKeyEdit->Text;
	if (!kwd.IsEmpty() && idx>=0 && idx<lp->Count) {
		UnicodeString dsc = get_CmdDesc(lp->Items->ValueFromIndex[idx],
								false, ExtMenuListBox->Items, ExtToolListBox->Items,
								USAME_TI(GetCmdModeStr(), "V:"));
		found = ContainsText(dsc, kwd);
	}
	return found;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyEditChange(TObject *Sender)
{
	TListBox *lp = KeyListBox;
	if (!FindKeyEdit->Text.IsEmpty()) {
		int idx = -1;
		for (int i=0; i<lp->Count && idx==-1; i++) if (MatchKeyItem(i)) idx = i;
		lp->ItemIndex = idx;
		if (idx!=-1) KeyListBoxClick(lp);
	}
	else {
		lp->ItemIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!Shift.Empty()) return;

	if (Key==VK_UP) {
		 FindKeyUpAction->Execute();
		 Key = 0;
	}
	else if (Key==VK_DOWN) {
		FindKeyDownAction->Execute();
		Key = 0;
	}
}
//---------------------------------------------------------------------------
//下方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyDownActionExecute(TObject *Sender)
{
	TListBox *lp = KeyListBox;
	if (!FindKeyEdit->Text.IsEmpty() && lp->Count>0) {
		int i0 = (lp->ItemIndex!=-1)? lp->ItemIndex : 0;
		if (MatchKeyItem(i0)) i0++;
		int idx = -1;
		for (int i=i0; i<lp->Count && idx==-1; i++) if (MatchKeyItem(i)) idx = i;
		if (idx!=-1) {
			lp->ItemIndex = idx;
			KeyListBoxClick(lp);
		}
		else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyDownActionUpdate(TObject *Sender)
{
	TListBox *lp = KeyListBox;
	int idx = -1;
	for (int i=lp->Count-1; i>=0 && idx==-1; i--) if (MatchKeyItem(i)) idx = i;
	((TAction*)Sender)->Enabled = (idx!=-1 && idx>lp->ItemIndex);
}
//---------------------------------------------------------------------------
//上方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyUpActionExecute(TObject *Sender)
{
	TListBox *lp = KeyListBox;
	if (!FindKeyEdit->Text.IsEmpty() && lp->ItemIndex>0) {
		int i0 = lp->ItemIndex;
		if (MatchKeyItem(i0)) i0--;
		int idx = -1;
		for (int i=i0; i>=0 && idx==-1; i--) if (MatchKeyItem(i)) idx = i;
		if (idx!=-1) {
			lp->ItemIndex = idx;
			KeyListBoxClick(lp);
		}
		else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindKeyUpActionUpdate(TObject *Sender)
{
	TListBox *lp = KeyListBox;
	int idx = -1;
	for (int i=0; i<lp->Count && idx==-1; i++) if (MatchKeyItem(i)) idx = i;
	((TAction*)Sender)->Enabled = (idx!=-1 && idx<lp->ItemIndex);
}

//---------------------------------------------------------------------------
//キー一覧の描画
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::KeyListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);

	UnicodeString key = lp->Items->Names[Index];
	UnicodeString cmd = lp->Items->ValueFromIndex[Index];

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	cv->TextOut(xp, yp, get_tkn_r(key, ":"));
	xp += cv->TextWidth("Shift+Ctrl+Alt+Space ");

	UnicodeString dsc = get_CmdDesc(cmd, true,
							ExtMenuListBox->Items, ExtToolListBox->Items, USAME_TI(GetCmdModeStr(), "V:"));

	int w_cmd = get_CharWidth(cv, 40);
	cmd = minimize_str(del_CmdDesc(cmd), cv, w_cmd - 8, true);
	cv->TextOut(xp, yp, cmd);
	xp += w_cmd;
	cv->TextOut(xp, yp, dsc);

	//ファンクションキー名
	UnicodeString lbl = FKeyLabelBuf->Values[key];
	if (!lbl.IsEmpty()) {
		xp = Rect.Left + Rect.Width() - get_CharWidth(cv, 16);
		cv->TextOut(xp, yp, lbl);
	}
}

//---------------------------------------------------------------------------
//キー割当一覧のソート
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::SortKeyListBox(UnicodeString item)
{
	TListBox *lp = KeyListBox;
	std::unique_ptr<TStringList> key_lst(new TStringList());
	key_lst->Assign(lp->Items);

	cursor_HourGlass();
	switch (KeySortMode) {
	case  1: key_lst->CustomSort(KeyComp_Cmd);	break;
	case  2: key_lst->CustomSort(KeyComp_Dsc);	break;
	case  3: key_lst->CustomSort(KeyComp_FKey);	break;
	default: key_lst->CustomSort(KeyComp_Key);
	}
	cursor_Default();

	lp->Items->Assign(key_lst.get());
	if (!item.IsEmpty()) {
		int idx = lp->Items->IndexOf(item);
		if (idx!=-1) lp->ItemIndex = idx;
	}

	KeyHeaderControl->Repaint();
}

//---------------------------------------------------------------------------
//キー設定の追加・変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChangeKeyList(bool add)
{
	if (KeyComboBox->ItemIndex!=-1 && CmdComboBox->ItemIndex!=-1) {
		try {
			UnicodeString ibuf = GetCmdKeyStr();
			UnicodeString cmd  = get_tkn(CmdComboBox->Text, ' ');
			if (PrmComboBox->Enabled && !PrmComboBox->Text.IsEmpty())
				cmd.cat_sprintf(_T("_%s"), get_tkn(PrmComboBox->Text, _T(" : ")).c_str());

			TListBox *lp = KeyListBox;
			//追加
			if (add) {
				ibuf.cat_sprintf(_T("=%s"), cmd.c_str());
				lp->Items->Add(ibuf);
			}
			//変更
			else {
				int kidx = -1;
				TStrings *sp = lp->Items;
				for (int i=0; i<sp->Count; i++) {
					if (SameText(sp->Names[i], ibuf)) { kidx = i;  break; }
				}
				int c_idx = lp->ItemIndex;
		 		if (kidx!=-1 && kidx!=c_idx) UserAbort(USTR_Registered);
				ibuf.cat_sprintf(_T("=%s"), cmd.c_str());
				lp->Items->Strings[c_idx] = ibuf;
			}
			SortKeyListBox(ibuf);

			SetCmdCombo();
			KeySetClick(NULL);
			KeyListBuf[KeyTabControl->TabIndex]->Assign(KeyListBox->Items);
			KeyListBoxClick(lp);
		}
		catch (EAbort &e) {
			msgbox_WARN(e.Message);
		}
	}
}

//---------------------------------------------------------------------------
//パラメータの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefCmdPrmBtnClick(TObject *Sender)
{
	UnicodeString cmd = get_tkn(CmdComboBox->Text, ' ');
	//ディレクトリ
	if (contained_wd_i(_T("ChangeDir|ChangeOppDir|CopyTo|ListTree|MoveTo|OpenByExp|PlayList|SubDirList"), cmd)) {
		UnicodeString dnam = PrmComboBox->Text;
		if (UserModule->SelectDirEx(_T("パラメータ指定 : ディレクトリ"), dnam)) PrmComboBox->Text = dnam;
	}
	//ファイル
	else {
		if (contained_wd_i(_T("DebugCmdFile|ExeCommands"), cmd)) {
			if (CmdFileListDlg->ShowToSelect()==mrOk) {
				PrmComboBox->Text = "@" + to_relative_name(CmdFileListDlg->CmdFileName);
			}
		}
		else {
			UnicodeString fmsk =
				USAME_TI(cmd, "LoadWorkList")? "*.nwl" :
				USAME_TI(cmd, "HelpCurWord") ? "*.chm" :
				contained_wd_i(_T("DistributionDlg|LoadTabGroup|Restart|SetColor"), cmd)? "*.ini" :
				contained_wd_i(_T("ContextMenu|ExeMenuFile|FindFileDlg|LoadResultList|PlayList|SelByList"), cmd)? "*.txt"
											 : "*.*";
			UnicodeString pnam =
				contained_wd_i(_T("DistributionDlg|Restart|SetColor"), cmd)? ExePath : RefParamPath;

			UserModule->PrepareOpenDlg(_T("パラメータ指定 : ファイル"), NULL, fmsk.c_str(), pnam);

			UnicodeString fnam;
			if (UserModule->OpenDlgToStr(fnam)) {
				RefParamPath = ExtractFilePath(fnam);
				//実行ファイル
				if (USAME_TI(cmd, "FileRun")) {
					TStringDynArray plst = split_strings_semicolon(GetEnvironmentVariable("PATH"));
					for (int i=0; i<plst.Length; i++) {
						if (SameText(IncludeTrailingPathDelimiter(plst[i]), RefParamPath)) {
							fnam = ExtractFileName(fnam); break;
						}
					}
				}
				//NyanFi 相対
				else if (contained_wd_i(
					_T("ContextMenu|DistributionDlg|ExeMenuFile|FindFileDlg|LoadBgImage|LoadResultList|")
					_T("LoadTabGroup|LoadWorkList|PlayList|Restart|SaveAsTabGroup|SetColor"), cmd))
				{
					fnam = to_relative_name(fnam);
				}
				PrmComboBox->Text = fnam;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::CheckKeyAction(bool ok, bool add)
{
	if (ok && PrmComboBox->Enabled) {
		if (PrmComboBox->Style==csDropDownList)
			ok = (PrmComboBox->ItemIndex!=-1);
		else if (contained_wd_i(_T("ChangeDir|ChangeOppDir|ExeCommands|MaskFind"), get_tkn(CmdComboBox->Text, ' ')))
			ok = !PrmComboBox->Text.IsEmpty();
	}

	//2ストローク開始キーのチェック
	if (ok) ok = !IsFirstCmdKey();

	//既存チェック
	if (add && ok) {
		UnicodeString kbuf = GetCmdKeyStr();
		TStrings *sp = KeyListBox->Items;
		for (int i=0; i<sp->Count; i++)
			if (SameText(sp->Names[i], kbuf)) { ok = false;  break; }
	}

	return ok;
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddKeyActionExecute(TObject *Sender)
{
	ChangeKeyList(true);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddKeyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled =
		CheckKeyAction(KeyComboBox->ItemIndex!=-1 && CmdComboBox->ItemIndex!=-1, true);
}

//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgKeyActionExecute(TObject *Sender)
{
	ChangeKeyList(false);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgKeyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled =
		CheckKeyAction(KeyListBox->ItemIndex!=-1 && KeyComboBox->ItemIndex!=-1 && CmdComboBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//キー設定の削除
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelKeyActionExecute(TObject *Sender)
{
	delete_ListItem(KeyListBox);
	SetCmdCombo();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelKeyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (KeyListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEditChange(TObject *Sender)
{
	TPageControl *pp = PageControl1;
	for (int i=0; i<pp->PageCount; i++) pp->Pages[i]->Tag = 0;
	pp->Repaint();
	FindMarkList->ClearAll();
	FindMarkList->MarkColor = col_OptFind;

	UnicodeString wd = FindEdit->Text;
	if (!wd.IsEmpty()) {
		//タブシート毎に検索
		for (int i=0; i<pp->PageCount; i++) {
			TTabSheet *sp = pp->Pages[i];
			sp->Tag = (FindMarkList->SearchControl((TWinControl*)sp, wd)>0 || ContainsText(sp->Caption, wd))? 1 : 0;
			//見つかったら Tag=1 に
		}
		FindMarkList->ShowMark();
		PageControl1->Repaint();
		//見つかった項目を含むタブをアクティブに
		bool found = false;
		for (int i=0; i<pp->PageCount && !found; i++) {
			if (pp->Pages[i]->Tag==1) {
				pp->ActivePage = pp->Pages[i];
				found = true;
			}
		}
		if (!found) beep_Warn();
	}
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefIniPatBtnClick(TObject *Sender)
{
	UnicodeString dnam = ExePath;
	if (UserModule->SelectDirEx(_T("初期パス"), dnam)) {
		dnam = to_path_name(dnam);
		if (((TComponent*)Sender)->Tag==1) R_IniPatEdit->Text = dnam; else L_IniPatEdit->Text = dnam;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::L_IniPatMod1RadioBtnClick(TObject *Sender)
{
	if (R_IniPatMod1RadioBtn->Checked) R_IniPatMod0RadioBtn->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::R_IniPatMod1RadioBtnClick(TObject *Sender)
{
	if (L_IniPatMod1RadioBtn->Checked) L_IniPatMod0RadioBtn->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefHomeWorkBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("ホームワークリストの指定"), F_FILTER_NWL, _T("*.nwl"), WorkListPath);
	UserModule->OpenDlgToEdit(HomeWorkListEdit, true);
}

//---------------------------------------------------------------------------
//仮想ドライブにするディレクトリの参照
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefVDrvBtnClick(TObject *Sender)
{
	UnicodeString dnam = VirDriveEdit->Text;
	if (UserModule->SelectDirEx(_T("マウントするディレクトリ"), dnam)) VirDriveEdit->Text = dnam;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::VirDrvListBoxClick(TObject *Sender)
{
	int idx = VirDrvListBox->ItemIndex;
	if (idx!=-1) {
		VirDrvComboBox->ItemIndex = VirDrvComboBox->Items->IndexOf(VirDrvListBox->Items->Names[idx]);
		VirDriveEdit->Text = VirDrvListBox->Items->ValueFromIndex[idx];
	}
}

//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddDrvActionExecute(TObject *Sender)
{
	UnicodeString lbuf = VirDrvComboBox->Text + "=" + VirDriveEdit->Text;
	if (VirDrvListBox->Items->IndexOf(lbuf)==-1) {
		VirDrvListBox->Items->Add(lbuf);
	}
	else msgbox_WARN(USTR_Registered);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddDrvActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (VirDrvComboBox->ItemIndex!=-1) && !VirDriveEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgDrvActionExecute(TObject *Sender)
{
	int idx = VirDrvListBox->ItemIndex;
	if (idx!=-1) VirDrvListBox->Items->Strings[idx] = VirDrvComboBox->Text + "=" + VirDriveEdit->Text;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgDrvActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= (VirDrvListBox->ItemIndex!=-1) && (VirDrvComboBox->ItemIndex!=-1) && !VirDriveEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//削除
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelDrvActionExecute(TObject *Sender)
{
	delete_ListItem(VirDrvListBox);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::DelDrvActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (VirDrvListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//配色のインポート
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::InpColBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("配色のインポート"), F_FILTER_INI, _T("*.INI"));
	if (UserModule->OpenDlg->Execute()) {
		std::unique_ptr<TStringList> col_lst(new TStringList());
		std::unique_ptr<TStringList> ext_lst(new TStringList());
		std::unique_ptr<TStringList> tag_lst(new TStringList());
		std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(UserModule->OpenDlg->FileName));
		inp_file->ReadSection(  "Color",		col_lst.get());
		inp_file->LoadListItems("ExtColList",	ext_lst.get());
		inp_file->ReadSection(  "TagColList",	tag_lst.get());
		if ((col_lst->Count + ext_lst->Count + tag_lst->Count)==0) {
			msgbox_WARN("配色設定がありません。");
		}
		else {
			if (col_lst->Count>0) ColBufList->Assign(col_lst.get());
			if (ext_lst->Count>0) ExtColListBox->Items->Assign(ext_lst.get());
			if (tag_lst->Count>0) TagColListBox->Items->Assign(tag_lst.get());
			//表示を更新
			OptColListBox->Repaint();
			TimColListBox->Repaint();
			ExtColListBox->Repaint();
			TagColListBox->Repaint();
		}
	}
}
//---------------------------------------------------------------------------
//配色のエクスポート
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExpColBtnClick(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("配色のエクスポート"), F_FILTER_INI, _T("Export_Col.INI"));
	if (UserModule->SaveDlg->Execute()) {
		std::unique_ptr<UsrIniFile> exp_file(new UsrIniFile(UserModule->SaveDlg->FileName));
		//配色
		exp_file->AssignSection("Color", ColBufList);
		//拡張子別配色
		std::unique_ptr<TStringList> exp_lst(new TStringList());
		exp_lst->Assign(ExtColListBox->Items);
		exp_file->SaveListItems("ExtColList", exp_lst.get());
		//タグ別色
		exp_lst->Assign(TagColListBox->Items);
		exp_file->AssignSection("TagColList", exp_lst.get());
		exp_file->WriteBool(SCT_Option, "RevTagCololr",	RevTagCololr);

		//保存
		if (!exp_file->UpdateFile(true)) msgbox_ERR(USTR_FaildSave);
	}
}

//---------------------------------------------------------------------------
//キー設定のインポート
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::InpKeyBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("キー設定のインポート"), F_FILTER_INI, _T("*.INI"));
	if (UserModule->OpenDlg->Execute()) {
		std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(UserModule->OpenDlg->FileName));
		std::unique_ptr<TStringList> key_lst(new TStringList());
		inp_file->ReadSection("KeyFuncList", key_lst.get());
		if (key_lst->Count>0) {
			for (int i=0; i<MAX_KEYTABS; i++) {
				KeyListBuf[i]->Clear();
				UnicodeString modstr = GetCmdModeStr(i + 1);
				for (int j=0; j<key_lst->Count; j++){
					UnicodeString lbuf = key_lst->Strings[j];
					if (StartsText(modstr, lbuf)) KeyListBuf[i]->Add(lbuf);
				}
			}
			inp_file->ReadSection("FKeyLabelList", FKeyLabelBuf);
			KeyTabControlChange(KeyTabControl);
			//その他のキー設定
			UnicodeString sstr = inp_file->ReadString(SCT_Option, "DlgMoveShift");
			DlgMoveShiftCheckBox->Checked = ContainsText(sstr, KeyStr_Shift);
			DlgMoveCtrlCheckBox->Checked  = ContainsText(sstr, KeyStr_Ctrl);
			DlgMoveAltCheckBox->Checked	  = ContainsText(sstr, KeyStr_Alt);
			sstr = inp_file->ReadString(SCT_Option, "DlgSizeShift");
			DlgSizeShiftCheckBox->Checked = ContainsText(sstr, KeyStr_Shift);
			DlgSizeCtrlCheckBox->Checked  = ContainsText(sstr, KeyStr_Ctrl);
			DlgSizeAltCheckBox->Checked	  = ContainsText(sstr, KeyStr_Alt);
			sstr = inp_file->ReadString(SCT_Option, "IniSeaShift", "F:Ctrl+");
			IniSeaShiftCheckBox->Checked  = ContainsText(sstr, KeyStr_Shift);
			IniSeaCtrlCheckBox->Checked   = ContainsText(sstr, KeyStr_Ctrl);
			IniSeaAltCheckBox->Checked	  = ContainsText(sstr, KeyStr_Alt);
			IniSeaByNumCheckBox->Checked  = inp_file->ReadBool(SCT_Option, "IniSeaByNum");
			IniSeaBySignCheckBox->Checked = inp_file->ReadBool(SCT_Option, "IniSeaBySign");
		}
		else msgbox_WARN("キー設定がありません。");
	}
}
//---------------------------------------------------------------------------
//キー設定のエクスポート
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ExpKeyBtnClick(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("キー設定のエクスポート"), F_FILTER_INI, _T("Export_Key.INI"));
	if (UserModule->SaveDlg->Execute()) {
		std::unique_ptr<UsrIniFile> exp_file(new UsrIniFile(UserModule->SaveDlg->FileName));
		std::unique_ptr<TStringList> key_lst(new TStringList());
		KeyListBuf[KeyTabControl->TabIndex]->Assign(KeyListBox->Items);
		for (int i=0; i<MAX_KEYTABS; i++) key_lst->AddStrings(KeyListBuf[i]);
		key_lst->CustomSort(KeyComp_Key);
		exp_file->AssignSection("KeyFuncList",	 key_lst.get());
		exp_file->AssignSection("FKeyLabelList", FKeyLabelBuf);
		//その他のキー設定
		UnicodeString sstr;
		if (DlgMoveShiftCheckBox->Checked) sstr += KeyStr_Shift;
		if (DlgMoveCtrlCheckBox->Checked)  sstr += KeyStr_Ctrl;
		if (DlgMoveAltCheckBox->Checked)   sstr += KeyStr_Alt;
		exp_file->WriteString(SCT_Option, "DlgMoveShift", sstr);
		sstr = EmptyStr;
		if (DlgSizeShiftCheckBox->Checked) sstr += KeyStr_Shift;
		if (DlgSizeCtrlCheckBox->Checked)  sstr += KeyStr_Ctrl;
		if (DlgSizeAltCheckBox->Checked)   sstr += KeyStr_Alt;
		exp_file->WriteString(SCT_Option, "DlgSizeShift", sstr);
		sstr = "F:";
		if (IniSeaShiftCheckBox->Checked) sstr += KeyStr_Shift;
		if (IniSeaCtrlCheckBox->Checked)  sstr += KeyStr_Ctrl;
		if (IniSeaAltCheckBox->Checked)   sstr += KeyStr_Alt;
		exp_file->WriteString(SCT_Option, "IniSeaShift", sstr);
		exp_file->WriteBool(SCT_Option, "IniSeaByNum",	IniSeaByNumCheckBox);
		exp_file->WriteBool(SCT_Option, "IniSeaBySign",	IniSeaBySignCheckBox);
		//書き出し
		if (!exp_file->UpdateFile(true)) msgbox_ERR(USTR_FaildSave);
	}
}
//---------------------------------------------------------------------------
//右寄せ
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AlRightBtnClick(TObject *Sender)
{
	IniWinWdEdit->Text = Screen->Width - EditToInt(IniWinLeftEdit);
	WinSizeChanged = IniWin1RadioBtn->Checked;
}
//---------------------------------------------------------------------------
//最大幅
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::MaxWdBtnClick(TObject *Sender)
{
	IniWinLeftEdit->Text = "0";
	IniWinWdEdit->Text	 = Screen->Width;
	WinSizeChanged = IniWin1RadioBtn->Checked;
}
//---------------------------------------------------------------------------
//最大高
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::MaxHiBtnClick(TObject *Sender)
{
	IniWinTopEdit->Text = "0";
	IniWinHiEdit->Text	= Screen->Height;
	WinSizeChanged = IniWin1RadioBtn->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::IniWinSizeChange(TObject *Sender)
{
	WinSizeChanged = IniWin1RadioBtn->Checked;
}

//---------------------------------------------------------------------------
//OpenStandard コマンドへの関連付け
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::StdCmdListBoxClick(TObject *Sender)
{
	int idx = StdCmdListBox->ItemIndex;  if (idx==-1) return;
	FExtStdEdit->Text = StdCmdListBox->Items->Names[idx];
	UnicodeString cmd = StdCmdListBox->Items->ValueFromIndex[idx];
	if (ContainsStr(cmd, "_")) {
		StdCmdPrmEdit->Text = get_PrmStr(cmd);
		cmd = get_CmdStr(cmd) + "_";
	}

	for (int i=0; i<StdCmdComboBox->Items->Count; i++) {
		if (ContainsText(StdCmdComboBox->Items->Strings[i], cmd)) {
			StdCmdComboBox->ItemIndex = i;  break;
		}
	}

	StdCmdComboBoxChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::StdCmdComboBoxChange(TObject *Sender)
{
	bool has_prm = ContainsStr(StdCmdComboBox->Text, "ExeCommands_");
	StdCmdPrmEdit->Enabled	 = has_prm;
	RefStdCmdPrmBtn->Enabled = has_prm;
	if (!has_prm) StdCmdPrmEdit->Text = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::StdCmdListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
		TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	int w_x = 0;
	for (int i=0; i<lp->Count; i++) w_x = std::max(w_x, cv->TextWidth(lp->Items->Names[i]));
	cv->TextOut(xp, yp, lp->Items->Names[Index]);
	cv->TextOut(xp + w_x + 20, yp, lp->Items->ValueFromIndex[Index]);
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::RefStdCmdPrmBtnClick(TObject *Sender)
{
	if (ContainsText(StdCmdComboBox->Text, "ExeCommands_")) {
		if (CmdFileListDlg->ShowToSelect()==mrOk)
			StdCmdPrmEdit->Text = "@" + to_relative_name(CmdFileListDlg->CmdFileName);
	}
}

//---------------------------------------------------------------------------
//標準コマンドの追加・変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChangeStdCmdList(bool add)
{
	if (FExtStdEdit->Text.IsEmpty() || StdCmdComboBox->Text.IsEmpty()) return;
	UnicodeString fext = Trim(FExtStdEdit->Text.LowerCase());	if (fext.IsEmpty()) return;
	if (fext[1]!='.') fext.Insert(".", 1);
	UnicodeString cmd = get_in_paren(StdCmdComboBox->Text);
	if (ContainsStr(cmd, "_")) cmd += StdCmdPrmEdit->Text;

	//追加
	if (add) {
		StdCmdListBox->Items->Add(fext.cat_sprintf(_T("=%s"), cmd.c_str()));
	}
	//変更
	else {
		int idx = StdCmdListBox->ItemIndex;  if (idx==-1) return;
		StdCmdListBox->Items->Strings[idx] = fext.cat_sprintf(_T("=%s"), cmd.c_str());
	}
}
//---------------------------------------------------------------------------
//追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddStdCmdActionExecute(TObject *Sender)
{
	ChangeStdCmdList(true);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddStdCmdActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FExtStdEdit->Text.IsEmpty() && !StdCmdComboBox->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//変更
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgStdCmdActionExecute(TObject *Sender)
{
	ChangeStdCmdList(false);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::ChgStdCmdActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		 = StdCmdListBox->ItemIndex!=-1 && !FExtStdEdit->Text.IsEmpty() && !StdCmdComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//イベント
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EventListBoxClick(TObject *Sender)
{
	EventCmdsEdit->Enabled = false;
	EventCmdsEdit->Text    = get_post_tab(EventListBox->Items->Strings[EventListBox->ItemIndex]);
	EventCmdsEdit->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EventCmdsEditChange(TObject *Sender)
{
	if (!EventCmdsEdit->Enabled) return;

	int idx = EventListBox->ItemIndex;
	if (idx!=-1)
		EventListBox->Items->Strings[idx]
			= get_pre_tab(EventListBox->Items->Strings[idx]) + "\t" + EventCmdsEdit->Text;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvEditChange(TObject *Sender)
{
	TListBox *lp = EventListBox;
	UnicodeString kwd = FindEvEdit->Text;
	if (!kwd.IsEmpty()) {
		int idx = -1;
		for (int i=0; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
		if (idx!=-1) lp->ItemIndex = idx;
	}
	else {
		lp->ItemIndex = -1;
	}
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!Shift.Empty()) return;

	if (Key==VK_UP) {
		FindEvUpAction->Execute();
		Key = 0;
	}
	else if (Key==VK_DOWN) {
		FindEvDownAction->Execute();
		Key = 0;
	}
}
//---------------------------------------------------------------------------
//イベントを下方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvDownActionExecute(TObject *Sender)
{
	TListBox *lp = EventListBox;
	UnicodeString kwd = FindEvEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
	}
	if (idx!=-1) {
		lp->ItemIndex = idx;
		EventListBoxClick(NULL);
	}
	else beep_Warn();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvDownActionUpdate(TObject *Sender)
{
	TListBox *lp = EventListBox;
	UnicodeString kwd = FindEvEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && !found; i++)
			found = ContainsText(lp->Items->Strings[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}
//---------------------------------------------------------------------------
//イベントを上方向に検索
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvUpActionExecute(TObject *Sender)
{
	TListBox *lp = EventListBox;
	UnicodeString kwd = FindEvEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && idx==-1; i--)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
	}
	if (idx!=-1) {
		lp->ItemIndex = idx;
		EventListBoxClick(NULL);
	}
	else beep_Warn();
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FindEvUpActionUpdate(TObject *Sender)
{
	TListBox *lp = EventListBox;
	UnicodeString kwd = FindEvEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && !found; i--)
			found = ContainsText(lp->Items->Strings[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}

//---------------------------------------------------------------------------
//コマンド/メニューファイルの編集
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EditFileActionExecute(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;
	UnicodeString fnam = get_cmds_prm_file(
		(PageControl1->ActivePage==EventSheet)?
			((tag==0)? EventCmdsEdit->Text : (tag==1)? TimerCmd1Edit->Text : TimerCmd2Edit->Text) : MenuPrmEdit->Text);
	if (!fnam.IsEmpty()) open_by_TextEditor(fnam);
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::EditFileActionUpdate(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;
	UnicodeString fnam = get_cmds_prm_file(
		(PageControl1->ActivePage==EventSheet)?
			((tag==0)? EventCmdsEdit->Text : (tag==1)? TimerCmd1Edit->Text : TimerCmd2Edit->Text) : MenuPrmEdit->Text);
	((TAction*)Sender)->Enabled = !fnam.IsEmpty();
}

//---------------------------------------------------------------------------
//削除制限ディレクトリの追加
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AddPrtDirActionExecute(TObject *Sender)
{
	UnicodeString dnam;
	if (UserModule->SelectDirEx(_T("作業ディレクトリ"), dnam)) {
		if (PrtDirListBox->Items->IndexOf(dnam)==-1) {
			PrtDirListBox->Items->Add(dnam);
			PrtDirListBox->ItemIndex = PrtDirListBox->Items->IndexOf(dnam);
		}
	}
}

//---------------------------------------------------------------------------
//デザインの適用
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AppDesignBtnClick(TObject *Sender)
{
	ApplyOptionByTag(DesignSheet);

	if (IconMode==0 && ShowIconCheckBox->Checked)
		IconMode = 1;
	else if (!ShowIconCheckBox->Checked)
		IconMode = 0;

	//メイン画面に通知
	::SendMessage(MainHandle, WM_NYANFI_APPEAR, (LayoutChanged? 1 : 0), 0);
}
//---------------------------------------------------------------------------
//フォント・配色の適用
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::AppColorBtnClick(TObject *Sender)
{
	ApplyOptionByTag(FontColSheet);

	//フォント
	for (int i=0; i<FontBufList->Count; i++) {
		TFont *f = (TFont *)FontList->Objects[i];
		f->Assign((TFont *)FontBufList->Objects[i]);

		if (USAME_TI(FontBufList->ValueFromIndex[i], "ダイアログ")) {
			Application->DefaultFont->Assign(f);
			UpdateMaxItemWidth();
		}
	}

	//配色
	ColorList->Assign(ColBufList);
	set_col_from_ColorList();

	ExtColList->Assign(ExtColListBox->Items);
	usr_TAG->TagColList->Assign(TagColListBox->Items);

	PageControl1->Repaint();
	ExtColListBox->Repaint();
	TagColListBox->Repaint();

	//メイン画面に通知
	::SendMessage(MainHandle, WM_NYANFI_APPEAR, 0, 0);
}

//---------------------------------------------------------------------------
//確定
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OkActionExecute(TObject *Sender)
{
	ModalResult = mrNone;

	//仮想ドライブ
	VirDriveList->Assign(VirDrvListBox->Items);
	if (VirDriveList->Count>0) {
		int cnt = 0;
		for (int i=0; i<VirDriveList->Count; i++) {
			UnicodeString msg = mount_VirDriveList(i);
			if (!msg.IsEmpty()) {
				if (msg[1]!='E') cnt++;
				AddLog(msg);
			}
		}
		if (cnt>0) get_DriveInfoList();
	}

	//NyanFi ホットキー再登録
	if (IsPrimary) {
		UnicodeString kstr;
		if (!HotKeyComboBox->Text.IsEmpty()) {
			if (HotShiftCheckBox->Checked) kstr += KeyStr_Shift;
			if (HotCtrlCheckBox->Checked)  kstr += KeyStr_Ctrl;
			if (HotAltCheckBox->Checked)   kstr += KeyStr_Alt;
			if (HotWinCheckBox->Checked)   kstr += "Win+";
			kstr += HotKeyComboBox->Text;
		}

		if (register_HotKey(ID_CALL_HOTKEY, kstr)) {
			CallHotKey = kstr;
		}
		else {
			FindEdit->Text = EmptyStr;
			FindEdit->Text = "呼び出しホットキー";
			msgbox_ERR(GlobalErrMsg);
			return;
		}

		//AppList ホットキー再登録
		kstr = EmptyStr;
		if (!AppKeyComboBox->Text.IsEmpty()) {
			if (AppShiftCheckBox->Checked) kstr += KeyStr_Shift;
			if (AppCtrlCheckBox->Checked)  kstr += KeyStr_Ctrl;
			if (AppAltCheckBox->Checked)   kstr += KeyStr_Alt;
			if (AppWinCheckBox->Checked)   kstr += "Win+";
			kstr += AppKeyComboBox->Text;
		}

		if (register_HotKey(ID_APP_HOTKEY, kstr)) {
			AppListHotKey = kstr;
		}
		else {
			FindEdit->Text = EmptyStr;
			FindEdit->Text = "AppList";
			msgbox_ERR(GlobalErrMsg);
			return;
		}
	}

	//コントロールのタグにしたがって変更を適用
	ApplyOptionByTag((TForm *)this);

	//タグを用いない設定
	TempPath = to_path_name(TempDirEdit->Text);
	SetTempPathA(TempPath);
	MaxTasks		 = MaxTasksComboBox->ItemIndex + 1;
	AppListHotPrm	 = get_word_i_idx(_T("|FA|FL|FI|AO|LO|LI"), AppPrmComboBox->ItemIndex);
	ViewTxtLimitSize = EditToInt(LimitTxtEdit) * 1024;
	ViewBinLimitSize = std::max(StrToInt64Def(LimitBinEdit->Text, 0) * 1048576, 1048576LL);
	DirDelimiter	 = DirDelimiter.IsEmpty()? UnicodeString("/") : DirDelimiter.SubString(1, 1);
	IniWinLeft		 = EditToInt(IniWinLeftEdit);
	IniWinTop		 = EditToInt(IniWinTopEdit);
	TabPinMark		 = def_if_empty(PinMarkEdit->Text, u"\U0001F4CD");

	if (IconMode==0 && ShowIconCheckBox->Checked)
		IconMode = 1;
	else if (!ShowIconCheckBox->Checked)
		IconMode = 0;

	UnicodeString tmp;
	//Migemo 再初期化
	if (!SameText(MigemoPath, MigemoDirEdit->Text)) {
		MigemoPath = to_path_name(MigemoDirEdit->Text);
		delete usr_Migemo;
		usr_Migemo = new MigemoUnit(to_absolute_name(MigemoPath));
		AddLog(tmp.sprintf(_T("  LOAD migemo.dll   %s"), usr_Migemo->DictReady? _T("OK") : _T("NONE")));
	}
	usr_Migemo->MinLength = IncSeaMigemoMin;

				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI
				//<<<<<<<X86_SPI

	//7z.dll対応拡張子
	usr_ARC->FExt7zDll = FExt7zDll;

	EtcEditorList->Assign(EtcEditorListBox->Items);

	//フォント
	for (int i=0; i<FontBufList->Count; i++) {
		TFont *f = (TFont *)FontList->Objects[i];
		f->Assign((TFont *)FontBufList->Objects[i]);
	}

	//配色
	ColorList->Assign(ColBufList);
	set_col_from_ColorList();
	ExtColList->Assign(ExtColListBox->Items);
	usr_TAG->TagColList->Assign(TagColListBox->Items);

	AssociateList->Assign(AssociateListBox->Items);
	OpenStdCmdList->Assign(StdCmdListBox->Items);
	ProtectDirList->Assign(PrtDirListBox->Items);

	TCheckListBox *lp = ExtMenuListBox;
	for (int i=0; i<lp->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], EXTMENU_CSVITMCNT, true);
		itm_buf[4].sprintf(_T("%s"), lp->Checked[i]? _T("1") : _T("0"));
		lp->Items->Strings[i] = make_csv_rec_str(itm_buf);
	}
	ExtMenuList->Assign(lp->Items);

	lp = ExtToolListBox;
	for (int i=0; i<lp->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[i], EXTTOOL_CSVITMCNT, true);
		itm_buf[5].sprintf(_T("%s"), lp->Checked[i]? _T("1") : _T("0"));
		lp->Items->Strings[i] = make_csv_rec_str(itm_buf);
	}
	ExtToolList->Assign(lp->Items);

	ViewFoldFitWin = FitFoldRadioBtn->Checked;

	//キー設定
	KeyListBuf[KeyTabControl->TabIndex]->Assign(KeyListBox->Items);
	std::unique_ptr<TStringList> key_lst(new TStringList());
	for (int i=0; i<MAX_KEYTABS; i++) key_lst->AddStrings(KeyListBuf[i]);
	key_lst->CustomSort(KeyComp_Key);
	KeyFuncList->Assign(key_lst.get());
	//ファンクションキーの表示名
	FKeyLabelList->Assign(FKeyLabelBuf);
	int i = 0;
	while (i<FKeyLabelList->Count) {
		if (KeyFuncList->IndexOfName(FKeyLabelList->Names[i])==-1) FKeyLabelList->Delete(i); else i++;
	}

	IniPathMode[0] = L_IniPatMod1RadioBtn->Checked? 1 : L_IniPatMod2RadioBtn->Checked? 2 : 0;
	IniPathMode[1] = R_IniPatMod1RadioBtn->Checked? 1 : R_IniPatMod2RadioBtn->Checked? 2 : 0;
	IniWorkMode    = IniWork1RadioBtn->Checked? 1 : 0;
	IniWinMode	   = IniWin1RadioBtn->Checked? 1 : 0;

	DlgMoveShift = EmptyStr;
	if (DlgMoveShiftCheckBox->Checked) DlgMoveShift += KeyStr_Shift;
	if (DlgMoveCtrlCheckBox->Checked)  DlgMoveShift += KeyStr_Ctrl;
	if (DlgMoveAltCheckBox->Checked)   DlgMoveShift += KeyStr_Alt;
	DlgSizeShift = EmptyStr;
	if (DlgSizeShiftCheckBox->Checked) DlgSizeShift += KeyStr_Shift;
	if (DlgSizeCtrlCheckBox->Checked)  DlgSizeShift += KeyStr_Ctrl;
	if (DlgSizeAltCheckBox->Checked)   DlgSizeShift += KeyStr_Alt;

	//頭文字サーチ
	IniSeaShift = "F:";
	if (IniSeaShiftCheckBox->Checked) IniSeaShift += KeyStr_Shift;
	if (IniSeaCtrlCheckBox->Checked)  IniSeaShift += KeyStr_Ctrl;
	if (IniSeaAltCheckBox->Checked)   IniSeaShift += KeyStr_Alt;

	//イベント
	for (int i=0; i<MAX_EVENT_CMD; i++)
		 *(EventCmdList[i].sp) = get_post_tab(EventListBox->Items->Strings[i]);

	//ツールチップの表示
	Application->ShowHint = ShowTooltip;

	SetupTimer();

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::OkActionUpdate(TObject *Sender)
{
	HotKeyComboBox->Enabled =
		(HotShiftCheckBox->Checked || HotCtrlCheckBox->Checked || HotAltCheckBox->Checked || HotWinCheckBox->Checked);

	DirBraEdit->Color = ShowIconCheckBox->Checked? col_Invalid : scl_Window;
	DirKetEdit->Color = ShowIconCheckBox->Checked? col_Invalid : scl_Window;

	bool flag = (DlgMoveShiftCheckBox->Checked || DlgMoveCtrlCheckBox->Checked || DlgMoveAltCheckBox->Checked);
	DlgMoveLabel->Font->Color = flag? scl_WindowText : col_Invalid;
	DlgMoveEdit->Color		  = flag? scl_Window	 : col_Invalid;

	flag = (DlgSizeShiftCheckBox->Checked ||DlgSizeCtrlCheckBox->Checked || DlgSizeAltCheckBox->Checked);
	DlgSizeLabel->Font->Color = flag? scl_WindowText : col_Invalid;
	DlgSizeEdit->Color		  = flag? scl_Window	 : col_Invalid;

	InvColIfEmpty(NoWatchPathEdit);
	InvColIfEmpty(NoInfPathEdit);
	InvColIfEmpty(FExtGetInfEdit);
	InvColIfEmpty(FExtNoInfEdit);
	InvColIfEmpty(NoImgPrvPathEdit);
	InvColIfEmpty(FExtImgPrvEdit);
	InvColIfEmpty(FExtNoImgPrvEdit);
	InvColIfEmpty(ExtTxViewerEdit);
	ExtTxViewerFmtEdit->Color = ExtTxViewerEdit->Color;

	HdrStrEdit->Color = MarkdownCheckBox->Checked? col_Invalid : scl_Window;

	SelBaseOnlyCheckBox->Enabled  = SelByMouseCheckBox->Checked;
	SelIconCheckBox->Enabled	  = SelByMouseCheckBox->Checked;

	PrvTxtInfLnEdit->Color		  = PrevTxtToInfCheckBox->Checked? scl_Window : col_Invalid;
	PrevTailCheckBox->Enabled	  = PrevTxtToInfCheckBox->Checked;
	flag = (PrevTxtToInfCheckBox->Checked && PrevTailCheckBox->Checked);
	PrvTxtTailLnEdit->Color		  = flag? scl_Window : col_Invalid;
	PrvActTailLnEdit->Color		  = flag? scl_Window : col_Invalid;

	AppendLogCheckBox->Enabled	  = SaveLogCheckBox->Checked;
	MaxLogFilesEdit->Enabled	  = SaveLogCheckBox->Checked;
	BeepTopEndCheckBox->Enabled   = HintTopEndCheckBox->Checked;
	SeekSwapNPCheckBox->Enabled   = SeekBindDirCheckBox->Checked;

	flag = (EditToInt(FwCntWarnEdit)==0 || EditToInt(FwTmWarnEdit)==0);
	FwCntWarnEdit->Color = flag? col_Invalid : scl_Window;
	FwTmWarnEdit->Color  = flag? col_Invalid : scl_Window;

	flag = (EditToInt(FwCntTaskFinEdit)==0 || EditToInt(FwTmTaskFinEdit)==0);
	FwCntTaskFinEdit->Color = flag? col_Invalid : scl_Window;
	FwTmTaskFinEdit->Color	= flag? col_Invalid : scl_Window;

	EventCmdsEdit->Enabled = (EventListBox->ItemIndex!=-1);
	RefCmdsBtn->Enabled	   = (EventListBox->ItemIndex!=-1);

	InvColIfEmpty(NopDtctTimeEdit);
	InvColIfEmpty(WatchIntervalEdit);
	InvColIfEmpty(FixedLimitEdit);

	ListPercentEdit->Color	 = !KeepOnResizeCheckBox->Checked? col_Invalid : scl_Window;
	FindPathWdEdit->Color	 = !FindPathColCheckBox->Checked?  col_Invalid : scl_Window;
	FindTagsWdEdit->Color	 = !FindTagsColCheckBox->Checked?  col_Invalid : scl_Window;

	ModalAlphaEdit->Color	 = !ModalScrCheckBox->Checked? col_Invalid : scl_Window;
	ModalColorBox->Color 	 = !ModalScrCheckBox->Checked? col_Invalid : scl_Window;

	RefFontBtn->Enabled		 = (FontComboBox->ItemIndex!=-1);
	RefColBtn->Enabled		 = (OptColListBox->ItemIndex!=-1);
	RefTimColBtn->Enabled	 = (TimColListBox->ItemIndex!=-1);

	if (!UserModule->SpuitEnabled()) {
		SpuitImage->Visible  = (OptColListBox->ItemIndex!=-1);
		SpuitImage2->Visible = (TimColListBox->ItemIndex!=-1);
		SpuitImage3->Visible = !ExtColorEdit->Text.IsEmpty();
		SpuitImage4->Visible = (TagColListBox->ItemIndex!=-1);
	}

	DivDirInfUDCheckBox->Enabled = DivFlistUDCheckBox->Checked;

	int bgmod = BgImgModeComboBox->ItemIndex;
	BgImgSubModeComboBox->Enabled = (bgmod==1 || bgmod==2);
	BgImg1Edit->Color	= (bgmod==0 || bgmod==3)?		col_Invalid : scl_Window;
	BgImg2Edit->Color	= (bgmod!=2)?					col_Invalid : scl_Window;
	HideTimeEdit->Color = !HideScrCheckBox->Checked?	col_Invalid : scl_Window;
	TileIfSzEdit->Color = !TileIfCheckBox->Checked?		col_Invalid : scl_Window;
	InvColIfEmpty(SpImgEdit);
	InvColIfEmpty(SpImg2Edit);

	CloseOtherCheckBox->Enabled = !TaskTrayCheckBox->Checked;
	if (TaskTrayCheckBox->Checked) CloseOtherCheckBox->Checked = true;

	MarkPathEdit->Color = !MarkImgCheckBox->Checked? col_Invalid : scl_Window;
	MarkFextEdit->Color = !MarkImgCheckBox->Checked? col_Invalid : scl_Window;
	MarkMemoEdit->Color = !MarkImgCheckBox->Checked? col_Invalid : scl_Window;

	ViewTabXEdit->Color = (EditToInt(TabXWdEdit)==0)? col_Invalid : scl_Window;

	L_IniPatEdit->Color = L_IniPatMod2RadioBtn->Checked? scl_Window : col_Invalid;
	R_IniPatEdit->Color = R_IniPatMod2RadioBtn->Checked? scl_Window : col_Invalid;
}

//---------------------------------------------------------------------------
//キャンセル処理
//---------------------------------------------------------------------------
void __fastcall TOptionDlg::CanButtonClick(TObject *Sender)
{
	//スポイトのキャンセル
	if (UserModule->SpuitEnabled()) {
		SwatchPanel->Visible = false;
		UserModule->EndSpuit();
	}
	//閉じる
	else {
		ModalResult = mrCancel;
	}
}

//---------------------------------------------------------------------------
//ヘルプ
//---------------------------------------------------------------------------
bool __fastcall TOptionDlg::FormHelp(WORD Command, NativeInt Data, bool &CallHelp)
{
	if (Command==HELP_CONTEXT || Command==HELP_CONTEXTPOPUP) {
		if (PageControl1->ActivePage==KeySetSheet && (CmdComboBox->Focused() || PrmComboBox->Focused())) {
			UnicodeString topic;
			switch (KeyTabControl->TabIndex) {
			case 0: topic = HELPTOPIC_FL;	break;	//ファイラー
			case 1: topic = HELPTOPIC_IS;	break;	//INC.サーチ
			case 2: topic = HELPTOPIC_TV;	break;	//テキストビュアー
			case 3: topic = HELPTOPIC_IV;	break;	//イメージビュアー
			case 4: topic = HELPTOPIC_CILW;	break;	//ログ
			}

			UnicodeString kwd = get_tkn(CmdComboBox->Text, ' ');
			if (topic.Pos('#')==0 && !kwd.IsEmpty() && !starts_Dollar(kwd))
				topic.cat_sprintf(_T("#%s"), kwd.c_str());

			HtmlHelpTopic(topic.c_str());
		}
		else {
			HtmlHelpContext(Data);
		}
		CallHelp = false;
	}
	return true;
}

//---------------------------------------------------------------------------
void __fastcall TOptionDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "Ctrl+F")) {
		FindEdit->SetFocus();
		Key = 0;
	}
}
//---------------------------------------------------------------------------

