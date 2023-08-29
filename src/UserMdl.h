//----------------------------------------------------------------------//
// 汎用共用モジュール													//
//																		//
//----------------------------------------------------------------------//
#ifndef UserMdlH
#define UserMdlH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ImgList.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
//リストボックスのオプションタグ
#define LBTAG_OPT_FOCS	0x10000000		//操作後、リストボックスにフォーカス
#define LBTAG_OPT_INHI	0x20000000		//変更禁止

//コンボボックスのオプションタグ
#define CBTAG_RGEX_V	0x01000000		//正規表現参照を表示
#define CBTAG_RGEX_E	0x02000000		//正規表現参照を有効
#define CBTAG_HISTORY	0x04000000		//履歴を持つ
#define CBTAG_NO_CALC	0x10000000		//電卓を表示しない
#define CBTAG_HAS_POP	0x40000000		//ポップアップが割り当てられている

//エディットボックスのオプションタグ
#define EDTAG_RGEX_V	0x01000000		//正規表現参照を表示
#define EDTAG_RGEX_E	0x02000000		//正規表現参照を有効
#define EDTAG_DST_FMT	0x04000000		//振分先の書式文字列参照
#define EDTAG_REF_CDIR	0x08000000		//カレントのパス無しディレクトリ名
#define EDTAG_NO_CALC	0x10000000		//電卓を表示しない

//ファイルダイアログ用フィルタ
#define F_FILTER_EXE	_T("実行ファイル (*.exe)|*.EXE")
#define F_FILTER_EXE2	_T("実行ファイル (*.exe)|*.EXE|ショートカット (*.lnk)|*.LNK")
#define F_FILTER_NBT	_T("コマンドファイル(*.nbt)|*.nbt")
#define F_FILTER_INI	_T("設定ファイル (*.ini)|*.INI")
#define F_FILTER_TXT	_T("テキストファイル (*.txt)|*.txt")
#define F_FILTER_CSV	_T("CSVファイル (*.csv)|*.csv")
#define F_FILTER_TSV	_T("TSVファイル (*.txt,*.csv,*.tsv)|*.txt;*.csv;*.tsv")
#define F_FILTER_NWL	_T("ワークリスト (*.nwl)|*.nwl")
#define F_FILTER_LST	_T("テキストファイル (*.txt)|*.TXT|ワークリスト (*.nwl)|*.nwl")
#define F_FILTER_ICO	_T("(*.ico,*.exe,*.dll,*.lnk)|*.ICO;*.EXE;*.DLL;*.LNK")
#define F_FILTER_WAV	_T("WAVファイル|*.WAV|すべてのファイル(*.*)|*.*")
#define F_FILTER_ALL	_T("すべてのファイル(*.*)|*.*")

//コードページ
struct code_page_inf {
	unsigned int  page;
	UnicodeString name;
};

#define MAX_SAVE_CODEPAGES 7
extern code_page_inf SaveCodePages[MAX_SAVE_CODEPAGES];

//---------------------------------------------------------------------------
typedef BOOL (WINAPI *FUNC_PickIconDlg)(HWND, LPWSTR, DWORD, LPDWORD);

//---------------------------------------------------------------------------
class TUserModule : public TDataModule
{
__published:	// IDE で管理されるコンポーネント
	TAction *CalculatorAction;
	TAction *ClrHistComboAction;
	TAction *ClrListItemAction;
	TAction *CopyComboAction;
	TAction *CutComboAction;
	TAction *DelHistComboAction;
	TAction *DelListItemAction;
	TAction *DownListItemAction;
	TAction *PasteComboAction;
	TAction *RefCmdNameAction;
	TAction *RefCurDirAction;
	TAction *RefCurFNameAction;
	TAction *RefCurPathAction;
	TAction *RefCurPNameAction;
	TAction *RefFileNameAction;
	TAction *RefOppPathAction;
	TAction *RegExCheckerAction;
	TAction *RenInsNameAction;
	TAction *SelAllComboAction;
	TAction *SizePosToFlieListAction;
	TAction *UpListItemAction;
	TAction *WebSeaComboAction;
	TAction *WebSeaEditAction;
	TActionList *CommonActList;
	TColorDialog *ColorDlg;
	TEditCopy *EditCopy1;
	TEditCut *EditCut1;
	TEditDelete *EditDelete1;
	TEditPaste *EditPaste1;
	TEditSelectAll *EditSelectAll1;
	TEditUndo *EditUndo1;
	TMenuItem *CalculatorEItem;
	TMenuItem *CalculatorItem;
	TMenuItem *ClrHistItem;
	TMenuItem *CmdParamItem;
	TMenuItem *DelHistItem;
	TMenuItem *EditItemItem;
	TMenuItem *GrepCopyItem;
	TMenuItem *GrepCutItem;
	TMenuItem *GrepPasteItem;
	TMenuItem *PopAllSelItem;
	TMenuItem *PopCopyItem;
	TMenuItem *PopCurFnamItem;
	TMenuItem *PopCurPathItem;
	TMenuItem *PopCutItem;
	TMenuItem *PopDelItem;
	TMenuItem *PopDFmt01Item;
	TMenuItem *PopDFmt02Item;
	TMenuItem *PopDFmt03Item;
	TMenuItem *PopDFmt04Item;
	TMenuItem *PopDFmt05Item;
	TMenuItem *PopDFmt06Item;
	TMenuItem *PopDFmt07Item;
	TMenuItem *PopDFmt08Item;
	TMenuItem *PopDFmt09Item;
	TMenuItem *PopDlmt1Item;
	TMenuItem *PopDlmt2Item;
	TMenuItem *PopDlmt3Item;
	TMenuItem *PopDlmt4Item;
	TMenuItem *PopDlmt5Item;
	TMenuItem *PopDlmt6Item;
	TMenuItem *PopEditItemItem;
	TMenuItem *PopOppPathItem;
	TMenuItem *PopPasteItem;
	TMenuItem *PopRefCmdItem;
	TMenuItem *PopRefDlmtItem;
	TMenuItem *PopRefFmtItem;
	TMenuItem *PopRefSndIdItem;
	TMenuItem *PopUndoItem;
	TMenuItem *RefCmdItem;
	TMenuItem *RefCurDirEItem;
	TMenuItem *RefCurDirItem;
	TMenuItem *RefCurFnamItem;
	TMenuItem *RefCurPnamItem;
	TMenuItem *RefFileNameItem;
	TMenuItem *RegExChkEItem;
	TMenuItem *RegExChkItem;
	TMenuItem *RegExpEItem;
	TMenuItem *RegExpItem;
	TMenuItem *RenInputNameItem;
	TMenuItem *SelAllItem;
	TMenuItem *Sep_e_1;
	TMenuItem *Sep_e_2;
	TMenuItem *Sep_e_3;
	TMenuItem *Sep_e_4;
	TMenuItem *Sep_e_5;
	TMenuItem *Sep_g_1;
	TMenuItem *Sep_g_2;
	TMenuItem *Sep_g_3;
	TMenuItem *Sep_g_4;
	TMenuItem *WebSeaComboItem;
	TMenuItem *WebSeaEditItem;
	TOpenDialog *OpenDlg;
	TOpenPictureDialog *OpenImgDlg;
	TPopupMenu *EditPopupMenuC;
	TPopupMenu *EditPopupMenuE;
	TSaveDialog *SaveDlg;
	TSaveTextFileDialog *SaveTxtDlg;
	TTimer *BlinkTimer;
	TTimer *ScrollTimer;
	TTimer *SpuitTimer;
	TFontDialog *FontDlg;

	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall DataModuleDestroy(TObject *Sender);
	void __fastcall UpListItemActionExecute(TObject *Sender);
	void __fastcall UpListItemActionUpdate(TObject *Sender);
	void __fastcall DownListItemActionExecute(TObject *Sender);
	void __fastcall DownListItemActionUpdate(TObject *Sender);
	void __fastcall DelListItemActionExecute(TObject *Sender);
	void __fastcall DelListItemActionUpdate(TObject *Sender);
	void __fastcall ClrListItemActionExecute(TObject *Sender);
	void __fastcall ClrListItemActionUpdate(TObject *Sender);
	void __fastcall ScrollTimerTimer(TObject *Sender);
	void __fastcall SpuitTimerTimer(TObject *Sender);
	void __fastcall FileDlgClose(TObject *Sender);
	void __fastcall CutComboActionExecute(TObject *Sender);
	void __fastcall EditComboActionUpdate(TObject *Sender);
	void __fastcall CopyComboActionExecute(TObject *Sender);
	void __fastcall PasteComboActionExecute(TObject *Sender);
	void __fastcall PasteComboActionUpdate(TObject *Sender);
	void __fastcall EditPaste1Execute(TObject *Sender);
	void __fastcall SelAllComboActionExecute(TObject *Sender);
	void __fastcall SelAllComboActionUpdate(TObject *Sender);
	void __fastcall DelHistComboActionExecute(TObject *Sender);
	void __fastcall DelHistComboActionUpdate(TObject *Sender);
	void __fastcall ClrHistComboActionExecute(TObject *Sender);
	void __fastcall ClrHistComboActionUpdate(TObject *Sender);
	void __fastcall RefCmdNameActionExecute(TObject *Sender);
	void __fastcall RefCmdNameActionUpdate(TObject *Sender);
	void __fastcall RefCurFNameActionExecute(TObject *Sender);
	void __fastcall RefCurFNameActionUpdate(TObject *Sender);
	void __fastcall RefCurPNameActionExecute(TObject *Sender);
	void __fastcall RefCurPNameActionUpdate(TObject *Sender);
	void __fastcall RefCurPathActionExecute(TObject *Sender);
	void __fastcall RefCurPathActionUpdate(TObject *Sender);
	void __fastcall RefOppPathActionExecute(TObject *Sender);
	void __fastcall RefOppPathActionUpdate(TObject *Sender);
	void __fastcall RefFileNameActionExecute(TObject *Sender);
	void __fastcall RefFileNameActionUpdate(TObject *Sender);
	void __fastcall RefCurDirActionExecute(TObject *Sender);
	void __fastcall RefCurDirActionUpdate(TObject *Sender);
	void __fastcall EditPopupMenuCPopup(TObject *Sender);
	void __fastcall EditPopupMenuEPopup(TObject *Sender);
	void __fastcall CmdParamItemClick(TObject *Sender);
	void __fastcall PopSndIdItemClick(TObject *Sender);
	void __fastcall RefRegExItemClick(TObject *Sender);
	void __fastcall PopDlmtItemClick(TObject *Sender);
	void __fastcall PopDFmtItemClick(TObject *Sender);
	void __fastcall PopEditItemItemClick(TObject *Sender);
	void __fastcall EditItemItemClick(TObject *Sender);
	void __fastcall RegExCheckerActionExecute(TObject *Sender);
	void __fastcall RegExCheckerActionUpdate(TObject *Sender);
	void __fastcall WebSeaComboActionExecute(TObject *Sender);
	void __fastcall WebSeaComboActionUpdate(TObject *Sender);
	void __fastcall WebSeaEditActionExecute(TObject *Sender);
	void __fastcall WebSeaEditActionUpdate(TObject *Sender);
	void __fastcall CalculatorActionExecute(TObject *Sender);
	void __fastcall CalculatorActionUpdate(TObject *Sender);
	void __fastcall SizePosToFlieListActionExecute(TObject *Sender);
	void __fastcall BlinkTimerTimer(TObject *Sender);

private:	// ユーザー宣言
	TCustomListBox *ActiveListBox;		//操作対象リストボックス
	UsrScrollPanel *ActiveScrPanel;		//シンプルスクロールバー
	int PrevListIdx;

	TComboBox *LastComboBox;
	int LastComboBoxSS;
	int LastComboBoxSL;

	TPanel *SpuitPanel;
	TImage *SpuitImage;
	TColor SpuitDefColor;
	TColor SpuitPanelColor;

	TComponent *BlinkObj;

	void __fastcall ListBoxStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall ListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y,
			TDragState State, bool &Accept);
	void __fastcall ListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall ListBoxEndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall ComboBoxContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);

	TComboBox   * __fastcall GetActiveComboBox();
	TCustomEdit * __fastcall GetActiveCustomEdit();

	void __fastcall SelectIconDlg(UnicodeString &fnam);

public:		// ユーザー宣言
	TCursor crSpuitTool;
	TCursor crHandGrabR;

	UnicodeString ExePath;		//NyanFi実行パス
	UnicodeString CurFileName;	//カーソル位置のファイル名
	UnicodeString CurPathName;	//現在のカレントパス名
	UnicodeString OppPathName;	//現在の反対パス名
	UnicodeString RefFileName;	//メニュー参照用ファイル名

	UnicodeString IconFilePath;	//アイコン参照パス

	TStringList *RefRegExList;	//正規表現の参照リスト
	TStringList *CmdParamList;	//コマンドパラメータの参照リスト

	TRect FileListRect;			//ファイルリスト部分のスクリーン座標Rect

	int ScrMode;				//画面モード

	__fastcall TUserModule(TComponent* Owner);

	void __fastcall InitializeListBox(TCustomListBox *lp, UsrScrollPanel *sp = NULL);
	void __fastcall UninitializeListBox();

	bool __fastcall ListBoxOpeItem(UnicodeString keystr);

	void __fastcall BeginSpuit(TPanel *pp, TImage *ip, TColor col_def);
	TColor __fastcall EndSpuit();
	bool __fastcall SpuitEnabled();

	void __fastcall SetUsrPopupMenu(TForm *fp);
	bool __fastcall ShowPopupMenu();

	void __fastcall SetOpenImgFilter(UnicodeString fnam);
	bool __fastcall SelectDirEx(const _TCHAR *tit, UnicodeString &dnam, bool to_rel = false);

	UnicodeString __fastcall get_DlgDefExt(UnicodeString filter);
	void __fastcall PrepareOpenDlg(const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam = NULL, UnicodeString inidir = EmptyStr);
	UnicodeString __fastcall OpenDlgExecute();
	bool __fastcall OpenDlgToEdit(TWinControl *ep, bool to_rel = false);
	bool __fastcall OpenDlgToStr(UnicodeString &s, bool to_rel = false);
	bool __fastcall OpenImgDlgToEdit(TWinControl *ep, bool to_rel = false);
	bool __fastcall FontDlgToFont(TFont *f);
	UnicodeString __fastcall OpenDlgIconIndex(bool to_rel = false);

	void __fastcall PrepareSaveDlg(const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam = NULL, UnicodeString inidir = EmptyStr);
	UnicodeString __fastcall SaveDlgExecute();

	void __fastcall SaveLastComboBox();
	void __fastcall RestoreLastComboBox();

	void __fastcall SetBlinkTimer(TComponent *cp);
	void __fastcall RepaintBlink(bool rst_sw = true);
};
//---------------------------------------------------------------------------
extern PACKAGE TUserModule *UserModule;
//---------------------------------------------------------------------------
#endif
