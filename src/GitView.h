//----------------------------------------------------------------------//
// NyanFi																//
//  Gitビューア															//
//----------------------------------------------------------------------//
#ifndef GitViewH
#define GitViewH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.IniFiles.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include "usr_hintwin.h"

//---------------------------------------------------------------------------
struct git_rec {
	UnicodeString graph;	//グラフ文字
	UnicodeString msg;		//メッセージ
	UnicodeString hash;		//コミットのハッシュ
	UnicodeString parent;	//親コミットのハッシュ
	UnicodeString branch;	//ブランチ
	UnicodeString branch_r;	//リモート部ラッチ
	UnicodeString tags;		//タグ(複数の場合 \t 区切り)
	UnicodeString author;	//Author名
	UnicodeString stash;	//Stash名
	TDateTime f_time;
	bool is_head;
	bool is_work;
	bool is_index;
	bool is_stash;
	UnicodeString diff_inf;	//差分情報
};

#define GIT_DEF_HISTLIMIT	100	//デフォルトのコミット履歴表示数

//ブランチ/タグリストの項目タグ
#define GIT_FLAG_HEAD	0x0001
#define GIT_FLAG_LOCAL	0x0100
#define GIT_FLAG_REMOTE	0x0200
#define GIT_FLAG_TAG	0x0400
#define GIT_FLAG_INVAL	0x0800
#define GIT_FLAG_HDRLN	0x1000	//見出し
#define GIT_FLAG_BLANK	0x2000	//空行

#define GIT_NULL_ID	"4b825dc642cb6eb9a060e54bf8d69288fbee4904"

//---------------------------------------------------------------------------
class TGitViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddAAction;
	TAction *AddAction;
	TAction *AddUAction;
	TAction *ArchiveAction;
	TAction *BlameAction;
	TAction *ChckoutAction;
	TAction *CherryPickAction;
	TAction *CommitAction;
	TAction *CommitInfoAction;
	TAction *ConsoleAction;
	TAction *CopyBranchNameAction;
	TAction *CopyCommitIDAction;
	TAction *CopyFileHashAction;
	TAction *DelTagAction;
	TAction *DiffDetailAction;
	TAction *DiffToolAction;
	TAction *EditFileAction;
	TAction *FindDownAction;
	TAction *FindUpAction;
	TAction *GuiAction;
	TAction *LogThisCommitAction;
	TAction *LogThisFileAction;
	TAction *MergeAction;
	TAction *OpenTmpArcAction;
	TAction *RenBranchAction;
	TAction *ResetAction;
	TAction *ResetAllAction;
	TAction *SetTagAction;
	TAction *ShowAuthorAction;
	TAction *ShowBranchesAction;
	TAction *ShowRBranchAction;
	TAction *ShowRemoteAction;
	TAction *ShowTagAction;
	TAction *UpdateLogAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TEdit *FindCommitEdit;
	TListBox *BranchListBox;
	TListBox *CommitListBox;
	TListBox *DiffListBox;
	TMenuItem *AddA2Item;
	TMenuItem *AddAItem;
	TMenuItem *AddU2Item;
	TMenuItem *AddUItem;
	TMenuItem *ArchiveItem;
	TMenuItem *BlameItem;
	TMenuItem *BrDelTagItem;
	TMenuItem *BrSetTagItem;
	TMenuItem *CheckoutItem;
	TMenuItem *CherryPickItem;
	TMenuItem *CommitInfoItem;
	TMenuItem *CopyBranchNameItem;
	TMenuItem *CopyCommitIDItem;
	TMenuItem *CopyFileHashItem;
	TMenuItem *CreBranchItem;
	TMenuItem *DelBranchItem;
	TMenuItem *DelTagItem;
	TMenuItem *DiffDetailItem;
	TMenuItem *DiffToolItem;
	TMenuItem *EditFileItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *HardResetItem;
	TMenuItem *LogThisCommitItem;
	TMenuItem *LogThisFileItem;
	TMenuItem *MergeItem;
	TMenuItem *MixedResetItem;
	TMenuItem *OpenTmpArcItem;
	TMenuItem *RenBranchItem;
	TMenuItem *ResetAll2Item;
	TMenuItem *ResetAllItem;
	TMenuItem *ResetItem;
	TMenuItem *SelConsoleItem;
	TMenuItem *SelGuiItem;
	TMenuItem *Sep_b_1;
	TMenuItem *Sep_c_1;
	TMenuItem *Sep_c_2;
	TMenuItem *Sep_c_3;
	TMenuItem *Sep_c_4;
	TMenuItem *Sep_d_1;
	TMenuItem *Sep_d_2;
	TMenuItem *SetTagItem;
	TMenuItem *ShowAuthorItem;
	TMenuItem *ShowBranchesItem;
	TMenuItem *ShowRemoteBrItem;
	TMenuItem *ShowRemoteItem;
	TMenuItem *ShowTagItem;
	TMenuItem *SoftResetItem;
	TPanel *BranchPanel;
	TPanel *CommitPanel;
	TPanel *DiffPanel;
	TPanel *R_Panel;
	TPanel *RL_Panel;
	TPanel *RU_Panel;
	TPopupMenu *BrPopupMenu;
	TPopupMenu *CmPopupMenu;
	TPopupMenu *ComOptPopupMenu;
	TPopupMenu *DiffPopupMenu;
	TSplitter *CommitSplitter;
	TSplitter *DiffSplitter;
	TSplitter *FindSplitter;
	TToolBar *DiffBar;
	TToolBar *FindBar;
	TToolButton *AddBtn;
	TToolButton *BlameBtn;
	TToolButton *ConsoleBtn;
	TToolButton *DeffToolBtn;
	TToolButton *DetailBtn;
	TToolButton *DiffSpcBtn1;
	TToolButton *DiffSpcBtn2;
	TToolButton *DiffSpcBtn3;
	TToolButton *EditBtn;
	TToolButton *FindBtn;
	TToolButton *FindDownBtn;
	TToolButton *FindSpcBtn;
	TToolButton *FindUpBtn;
	TToolButton *GuiBtn;
	TToolButton *ResetBtn;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *UpdateBtn;
	TAction *StashAction;
	TToolButton *FindSpcBtn2;
	TAction *StashPopAction;
	TAction *StashApplyAction;
	TAction *StashDropAction;
	TMenuItem *StashApplyItem;
	TMenuItem *StashPopItem;
	TMenuItem *StashDropItem;
	TMenuItem *Sep_c_5;
	TAction *ViewFileAction;
	TMenuItem *ViewFileItem;
	TAction *CopyPathAction;
	TMenuItem *CopyPathItem;
	TMenuItem *Sep_d_3;
	TTimer *WaitTimer;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BranchPanelResize(TObject *Sender);
	void __fastcall CommitPanelResize(TObject *Sender);
	void __fastcall DiffPanelResize(TObject *Sender);
	void __fastcall WaitTimerTimer(TObject *Sender);
	void __fastcall BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall BranchListBoxClick(TObject *Sender);
	void __fastcall BranchListBoxDblClick(TObject *Sender);
	void __fastcall BranchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CommitListBoxClick(TObject *Sender);
	void __fastcall CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DiffListBoxEnter(TObject *Sender);
	void __fastcall DiffListBoxClick(TObject *Sender);
	void __fastcall DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GitListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall GitListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall CheckoutActionExecute(TObject *Sender);
	void __fastcall CheckoutActionUpdate(TObject *Sender);
	void __fastcall CreBranchActionExecute(TObject *Sender);
	void __fastcall CreBranchActionUpdate(TObject *Sender);
	void __fastcall DelBranchActionExecute(TObject *Sender);
	void __fastcall DelBranchActionUpdate(TObject *Sender);
	void __fastcall RenBranchActionExecute(TObject *Sender);
	void __fastcall RenBranchActionUpdate(TObject *Sender);
	void __fastcall SetTagActionExecute(TObject *Sender);
	void __fastcall SetTagActionUpdate(TObject *Sender);
	void __fastcall DelTagActionExecute(TObject *Sender);
	void __fastcall DelTagActionUpdate(TObject *Sender);
	void __fastcall MergeActionExecute(TObject *Sender);
	void __fastcall MergeActionUpdate(TObject *Sender);
	void __fastcall CherryPickActionExecute(TObject *Sender);
	void __fastcall CherryPickActionUpdate(TObject *Sender);
	void __fastcall CopyBranchNameActionExecute(TObject *Sender);
	void __fastcall CopyBranchNameActionUpdate(TObject *Sender);
	void __fastcall CopyCommitIDActionExecute(TObject *Sender);
	void __fastcall CopyCommitIDActionUpdate(TObject *Sender);
	void __fastcall BrPopupMenuPopup(TObject *Sender);
	void __fastcall ResetItemClick(TObject *Sender);
	void __fastcall ArchiveActionExecute(TObject *Sender);
	void __fastcall ArchiveActionUpdate(TObject *Sender);
	void __fastcall OpenTmpArcActionExecute(TObject *Sender);
	void __fastcall LogThisCommitActionExecute(TObject *Sender);
	void __fastcall LogThisCommitActionUpdate(TObject *Sender);
	void __fastcall BlameActionExecute(TObject *Sender);
	void __fastcall LogThisFileActionExecute(TObject *Sender);
	void __fastcall CommitInfoActionExecute(TObject *Sender);
	void __fastcall CommitInfoActionUpdate(TObject *Sender);
	void __fastcall CopyFileHashActionExecute(TObject *Sender);
	void __fastcall CopyPathActionExecute(TObject *Sender);
	void __fastcall CopyFileInfActionUpdate(TObject *Sender);
	void __fastcall DiffDetailActionExecute(TObject *Sender);
	void __fastcall DiffDetailActionUpdate(TObject *Sender);
	void __fastcall ViewFileActionExecute(TObject *Sender);
	void __fastcall ViewFileActionUpdate(TObject *Sender);
	void __fastcall ShowRBranchActionExecute(TObject *Sender);
	void __fastcall ShowTagActionExecute(TObject *Sender);
	void __fastcall ShowBranchesActionExecute(TObject *Sender);
	void __fastcall ShowRemoteActionExecute(TObject *Sender);
	void __fastcall ShowAuthorActionExecute(TObject *Sender);
	void __fastcall GitPopupMenuPopup(TObject *Sender);
	void __fastcall FindCommitEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindCommitEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FindCommitEditChange(TObject *Sender);
	void __fastcall FindCommitEditEnter(TObject *Sender);
	void __fastcall FindCommitEditExit(TObject *Sender);
	void __fastcall FindUpActionExecute(TObject *Sender);
	void __fastcall FindUpActionUpdate(TObject *Sender);
	void __fastcall FindDownActionExecute(TObject *Sender);
	void __fastcall FindDownActionUpdate(TObject *Sender);
	void __fastcall FindBtnClick(TObject *Sender);
	void __fastcall UpdateLogActionExecute(TObject *Sender);
	void __fastcall DiffToolActionExecute(TObject *Sender);
	void __fastcall DiffActionUpdate(TObject *Sender);
	void __fastcall GuiActionExecute(TObject *Sender);
	void __fastcall GuiActionUpdate(TObject *Sender);
	void __fastcall ConsoleActionExecute(TObject *Sender);
	void __fastcall ConsoleActionUpdate(TObject *Sender);
	void __fastcall EditFileActionExecute(TObject *Sender);
	void __fastcall EditFileActionUpdate(TObject *Sender);
	void __fastcall SelGuiItemClick(TObject *Sender);
	void __fastcall SelConsoleItemClick(TObject *Sender);
	void __fastcall AddActionExecute(TObject *Sender);
	void __fastcall AddActionUpdate(TObject *Sender);
	void __fastcall AddAActionExecute(TObject *Sender);
	void __fastcall AddUActionExecute(TObject *Sender);
	void __fastcall AddAllActionUpdate(TObject *Sender);
	void __fastcall ResetActionExecute(TObject *Sender);
	void __fastcall ResetActionUpdate(TObject *Sender);
	void __fastcall ResetAllActionExecute(TObject *Sender);
	void __fastcall ResetAllActionUpdate(TObject *Sender);
	void __fastcall CommitActionExecute(TObject *Sender);
	void __fastcall CommitActionUpdate(TObject *Sender);
	void __fastcall StashActionExecute(TObject *Sender);
	void __fastcall StashActionUpdate(TObject *Sender);
	void __fastcall StashPopActionExecute(TObject *Sender);
	void __fastcall StashApplyActionExecute(TObject *Sender);
	void __fastcall StashDropActionExecute(TObject *Sender);
	void __fastcall StashPopActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString RefHEAD;
	UnicodeString CommitID;
	UnicodeString ParentID;
	UnicodeString BranchName;
	UnicodeString TagNames;
	UnicodeString StashName;

	bool AutoCrlf;
	bool KeyHandled;

	UsrHintWindow *MsgHint;

	TStringList *WarnList;
	TStringList *StatusList;
	bool Staged;

	UsrScrollPanel *BranchScrPanel;
	UsrScrollPanel *CommitScrPanel;
	UsrScrollPanel *DiffScrPanel;

	int MaxGrWidth;
	int MaxAnWidth;
	int MaxDfWidth;

	int LastBrListIdx;
	int LastCmListIdx;
	int LastDfListIdx;

	UnicodeString   __fastcall GitExeStr(UnicodeString prm);
	TStringDynArray __fastcall GitExeStrArray(UnicodeString prm);
	bool __fastcall GitExeList(UnicodeString prm, TStringList *o_lst = NULL, UnicodeString hint = EmptyStr);
	UnicodeString   __fastcall SaveRevAsTemp(UnicodeString id, UnicodeString fnam);

	git_rec * __fastcall cre_GitRec(UnicodeString msg = EmptyStr);
	void __fastcall ClearCommitList();
	void __fastcall UpdateBranchList();
	void __fastcall UpdateDiffList(bool keep_idx = false);
	UnicodeString __fastcall UpdateStatusList();
	void __fastcall UpdateCommitList(UnicodeString commit_id = EmptyStr);

	int __fastcall GetCurBranchFlag()
	{
		TListBox *lp = BranchListBox;
		return ((lp->ItemIndex!=-1)? (int)lp->Items->Objects[lp->ItemIndex] : 0);
	}

	bool __fastcall CurIsHEAD()
	{
		TListBox *lp = BranchListBox;
		int flag = (lp->ItemIndex!=-1)? (int)lp->Items->Objects[lp->ItemIndex] : 0;
		return ((flag & GIT_FLAG_HEAD) && (flag & GIT_FLAG_LOCAL));
	}

	void __fastcall SetCommitListIndex(int idx)
	{
		if (idx!=-1) {
			CommitListBox->ItemIndex = LastCmListIdx = idx;
			CommitListBoxClick(NULL);
		}
	}

	git_rec * __fastcall GetCurCommitPtr()
	{
		TListBox *lp = CommitListBox;
		return (lp->ItemIndex!=-1)? (git_rec *)lp->Items->Objects[lp->ItemIndex] : NULL;
	}

	UnicodeString __fastcall GetDiffFileName(bool inc_u = false)
	{
		TListBox *lp = DiffListBox;
		UnicodeString lbuf = ((lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr);
		return Trim(lbuf.Pos(" | ")? get_tkn(lbuf, " | ") : (inc_u && lbuf.Pos("? "))? get_tkn_r(lbuf, "? ") : EmptyStr);
	}
	UnicodeString __fastcall GetDiffTextName()
	{
		TListBox *lp = DiffListBox;
		UnicodeString lbuf = ((lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr);
		if (lbuf.IsEmpty() || lbuf.Pos(" Bin ")) return EmptyStr;
		return Trim(lbuf.Pos(" | ")? get_tkn(lbuf, " | ") : lbuf.Pos("? ")? get_tkn_r(lbuf, "? ") : EmptyStr);
	}

	UnicodeString __fastcall GetCurBranchName(bool lc_only = false, bool br_only = false, bool av_only = true);

	void __fastcall ShowMsgHint(UnicodeString msg, TWinControl *cp = NULL)
	{
		if (msg.IsEmpty()) return;
		if (!cp) cp = Screen->ActiveControl;
		if (!cp) cp = this;
		MsgHint->ActivateHintEx("\r\n" + msg + "\r\n", ScaledInt(480), ScaledInt(240), cp, col_bgHint);
	}

	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

public:		// ユーザー宣言
	UnicodeString WorkDir;		//Gitトップディレクトリ
	UnicodeString FilterName;	//履歴絞り込み時の対象名
	int HistoryLimit;			//履歴の表示制限数
	UnicodeString RetArcFile;	//[o] 一時アーカイブ名

	bool FGitBusy;
	void __fastcall SetGitBusy(bool Value)
	{
		FGitBusy = Value;
		Enabled  = !Value;
		if (Value) cursor_HourGlass(); else cursor_Default();
	}
	__property bool GitBusy = {read = FGitBusy,  write = SetGitBusy};

	__fastcall TGitViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGitViewer *GitViewer;
//---------------------------------------------------------------------------
#endif
