//----------------------------------------------------------------------//
// NyanFi																//
//  最近編集/閲覧した/使ったファイル一覧								//
//  栞マーク一覧/ ダイレクトタグジャンプ								//
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
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TEditHistoryDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *ClrAllRecentAction;
	TAction *ClrBrkRecentAction;
	TAction *ShowFileInfoAction;
	TAction *ShowPropertyAction;
	TAction *ShowStatusBarAction;
	TAction *ShowUsedTimeAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TButton *HiddenCanBtn;
	TCheckBox *MigemoCheckBox;
	TCheckBox *OpenCheckBox;
	THeaderControl *EditHistHeader;
	TLabeledEdit *FilterEdit;
	TMenuItem *CleAllRecentItem;
	TMenuItem *ClearAllItem;
	TMenuItem *ClearBrokenItem;
	TMenuItem *ClrAllMarkItem;
	TMenuItem *ClrMemoItem;
	TMenuItem *DelMarkItem;
	TMenuItem *hFileInfoItem;
	TMenuItem *hFitSizePosItem;
	TMenuItem *hPropertyItem;
	TMenuItem *hStatusBarItem;
	TMenuItem *MemoItem;
	TMenuItem *mFileInfoItem;
	TMenuItem *mFitSizePosItem;
	TMenuItem *mPropertyItem;
	TMenuItem *mStatusBarItem;
	TMenuItem *NoHistItem;
	TMenuItem *rFileInfoItem;
	TMenuItem *rFitSizePosItem;
	TMenuItem *rPropertyItem;
	TMenuItem *rStatusBarItem;
	TMenuItem *Sep_h_1;
	TMenuItem *Sep_h_2;
	TMenuItem *Sep_m_1;
	TMenuItem *Sep_m_2;
	TMenuItem *Sep_m_3;
	TMenuItem *Sep_r_1;
	TMenuItem *Sep_r_2;
	TMenuItem *ShowUsedTimeItem;
	TPanel *BlankPanel;
	TPanel *BtnPanel;
	TPanel *GridPanel;
	TPanel *MarkPanel;
	TPanel *OptInPanel;
	TPanel *OptPanel;
	TPanel *OptSubPanel;
	TPanel *RadioPanel;
	TPopupMenu *HistPopupMenu;
	TPopupMenu *MarkPopupMenu;
	TPopupMenu *RecentPopupMenu;
	TRadioGroup *OptRadioGroup;
	TSpeedButton *HideOptBtn;
	TSpeedButton *MemoBtn;
	TSpeedButton *ShowOptBtn;
	TStatusBar *StatusBar1;
	TStringGrid *EditHistGrid;

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
	void __fastcall OptRadioGroupClick(TObject *Sender);
	void __fastcall OpenCheckBoxClick(TObject *Sender);
	void __fastcall ChgOptBtnClick(TObject *Sender);
	void __fastcall EditHistHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall MemoItemClick(TObject *Sender);
	void __fastcall ClrMemoItemClick(TObject *Sender);
	void __fastcall DelMarkItemClick(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditHistHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall ClearAllItemClick(TObject *Sender);
	void __fastcall NoHistItemClick(TObject *Sender);
	void __fastcall EditHistHeaderResize(TObject *Sender);
	void __fastcall ClrAllMarkItemClick(TObject *Sender);
	void __fastcall ShowPropertyActionExecute(TObject *Sender);
	void __fastcall ShowPropertyActionUpdate(TObject *Sender);
	void __fastcall ClrAllRecentActionExecute(TObject *Sender);
	void __fastcall ClrAllRecentActionUpdate(TObject *Sender);
	void __fastcall ShowUsedTimeActionExecute(TObject *Sender);
	void __fastcall ShowFileInfoActionExecute(TObject *Sender);
	void __fastcall ShowFileInfoActionUpdate(TObject *Sender);
	void __fastcall ClrBrkRecentActionExecute(TObject *Sender);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall ShowStatusBarActionExecute(TObject *Sender);
	void __fastcall EditHistGridClick(TObject *Sender);

private:	// ユーザー宣言
	TStringList *HistoryList;
	TStringList *HistBufList;

	int MaxFextWd;
	int MarkSortMode;		//マーク一覧のソートモード
	int MarkSortOdr[6];		//マーク一覧のソート方向

	void __fastcall WmNyanFiFlIcon(TMessage &msg);

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg);

	UnicodeString __fastcall get_CurFileName();
	file_rec *    __fastcall get_CurFileRec();

	void __fastcall UpdateList();
	void __fastcall UpdateGrid();
	void __fastcall SetOptBtn();
	void __fastcall SetSttBar();
	void __fastcall SortItemList();
	bool __fastcall del_HistItem();
	bool __fastcall set_FileName(int idx);

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー

	bool isView;			//最近閲覧したファイル一覧(false = 編集した～)
	bool isRecent;			//最近使ったファイル一覧
	bool isMark;			//マーク一覧
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
