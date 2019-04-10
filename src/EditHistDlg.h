//----------------------------------------------------------------------//
// NyanFi																//
//  最近編集/閲覧した/使ったファイル一覧								//
//  栞マーク一覧/ リポジトリ一覧/ ダイレクトタグジャンプ				//
//----------------------------------------------------------------------//
#ifndef EditHistDlgH
#define EditHistDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TEditHistoryDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *ClrAllRecentAction;
	TAction *ClrBrkRecentAction;
	TAction *GitViewerAction;
	TAction *MemoAction;
	TAction *MigemoAction;
	TAction *OpenAction;
	TAction *OptMode0Action;
	TAction *OptMode1Action;
	TAction *OptMode2Action;
	TAction *ShowFileInfoAction;
	TAction *ShowPropertyAction;
	TAction *ShowStatusBarAction;
	TAction *ShowUsedTimeAction;
	TAction *UpdateGitInfAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TEdit *FilterEdit;
	THeaderControl *EditHistHeader;
	TMenuItem *CleAllRecentItem;
	TMenuItem *ClearAllItem;
	TMenuItem *ClearBrokenItem;
	TMenuItem *ClrAllMarkItem;
	TMenuItem *ClrMemoItem;
	TMenuItem *DelMarkItem;
	TMenuItem *gFileInfoItem;
	TMenuItem *gFitSizePosItem;
	TMenuItem *GitViewerItem;
	TMenuItem *gPropertyItem;
	TMenuItem *gStatusBarItem;
	TMenuItem *hFileInfoItem;
	TMenuItem *hFitSizePosItem;
	TMenuItem *hPropertyItem;
	TMenuItem *hStatusBarItem;
	TMenuItem *MemoItem;
	TMenuItem *mFileInfoItem;
	TMenuItem *mFitSizePosItem;
	TMenuItem *mPropertyItem;
	TMenuItem *mStatusBarItem;
	TMenuItem *N1;
	TMenuItem *NoHistItem;
	TMenuItem *rFileInfoItem;
	TMenuItem *rFitSizePosItem;
	TMenuItem *rPropertyItem;
	TMenuItem *rStatusBarItem;
	TMenuItem *Sep_g_1;
	TMenuItem *Sep_g_2;
	TMenuItem *Sep_h_1;
	TMenuItem *Sep_h_2;
	TMenuItem *Sep_m_1;
	TMenuItem *Sep_m_2;
	TMenuItem *Sep_m_3;
	TMenuItem *Sep_r_1;
	TMenuItem *Sep_r_2;
	TMenuItem *ShowUsedTimeItem;
	TPanel *GridPanel;
	TPopupMenu *HistPopupMenu;
	TPopupMenu *MarkPopupMenu;
	TPopupMenu *RecentPopupMenu;
	TPopupMenu *RepoPopupMenu;
	TSplitter *FilterSplitter;
	TStatusBar *StatusBar1;
	TStringGrid *EditHistGrid;
	TToolBar *OpeToolBar;
	TToolButton *FilterBtn;
	TToolButton *MemoBtn;
	TToolButton *MigemoBtn;
	TToolButton *OpenBtn;
	TToolButton *OptMode0Btn;
	TToolButton *OptMode1Btn;
	TToolButton *OptMode2Btn;
	TToolButton *ToolButton5;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditHistHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall EditHistGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall EditHistGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EditHistGridDblClick(TObject *Sender);
	void __fastcall EditHistHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall ClrMemoItemClick(TObject *Sender);
	void __fastcall DelMarkItemClick(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FilterBtnClick(TObject *Sender);
	void __fastcall EditHistHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall ClearAllItemClick(TObject *Sender);
	void __fastcall NoHistItemClick(TObject *Sender);
	void __fastcall EditHistHeaderResize(TObject *Sender);
	void __fastcall ClrAllMarkItemClick(TObject *Sender);
	void __fastcall ShowPropertyActionExecute(TObject *Sender);
	void __fastcall ClrAllRecentActionExecute(TObject *Sender);
	void __fastcall ClrAllRecentActionUpdate(TObject *Sender);
	void __fastcall ShowUsedTimeActionExecute(TObject *Sender);
	void __fastcall ShowFileInfoActionExecute(TObject *Sender);
	void __fastcall ShowFileInfoActionUpdate(TObject *Sender);
	void __fastcall ShowStatusBarActionExecute(TObject *Sender);
	void __fastcall ClrBrkRecentActionExecute(TObject *Sender);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall EditHistGridClick(TObject *Sender);
	void __fastcall ToggleActionExecute(TObject *Sender);
	void __fastcall MemoActionExecute(TObject *Sender);
	void __fastcall OptModeActionExecute(TObject *Sender);
	void __fastcall UpdateGitInfActionExecute(TObject *Sender);
	void __fastcall UpdateGitInfActionUpdate(TObject *Sender);
	void __fastcall GitViewerActionExecute(TObject *Sender);

private:	// ユーザー宣言
	bool DlgInitialized;

	TStringList *HistoryList;
	TStringList *HistBufList;

	int MaxFextWd;
	int ListSortMode;		//マーク/リポジトリ一覧のソートモード
	int ListSortOdr[6];		//マーク/リポジトリ一覧のソート方向

	void __fastcall WmNyanFiFlIcon(TMessage &msg) { EditHistGrid->Invalidate(); }

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	UnicodeString __fastcall get_CurFileName();
	file_rec *    __fastcall get_CurFileRec();

	void __fastcall UpdateList();
	void __fastcall UpdateGrid();
	void __fastcall SetSttBar();
	void __fastcall SortItemList();
	bool __fastcall del_HistItem();
	bool __fastcall set_FileName(int idx);

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー

	bool isView;			//最近閲覧したファイル一覧(false = 編集した～)
	bool isRecent;			//最近使ったファイル一覧
	bool isMark;			//マーク一覧
	bool isRepo;			//リポジトリ一覧
	bool isTags;			//ダイレクトタグジャンプ
	bool isTagPtn;
	bool ToFilter;			//フィルタ欄にフォーカス

	TStringList *TagJumpList;

	UnicodeString EditFileName;
	UnicodeString CmdStr;
	UnicodeString TagName;
	UnicodeString TagPath;
	UnicodeString TagJumpInf;

	__fastcall TEditHistoryDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_NYANFI_FLICON,	TMessage,	WmNyanFiFlIcon)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TEditHistoryDlg *EditHistoryDlg;
//---------------------------------------------------------------------------
#endif
