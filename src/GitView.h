//----------------------------------------------------------------------//
// Gitビュアー															//
//																		//
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
	TDateTime f_time;
	bool is_head;
	bool is_work;
	bool is_index;
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

//---------------------------------------------------------------------------
class TGitViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AppFextColorAction;
	TAction *ArchiveAction;
	TAction *BlameAction;
	TAction *ChckoutAction;
	TAction *CommitInfoAction;
	TAction *ConsoleAction;
	TAction *CopyBranchNameAction;
	TAction *CopyCommitIDAction;
	TAction *DelTagAction;
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
	TMenuItem *AppFextColItem;
	TMenuItem *ArchiveItem;
	TMenuItem *BlameItem;
	TMenuItem *BrDelTagItem;
	TMenuItem *BrSetTagItem;
	TMenuItem *CheckoutItem;
	TMenuItem *CommitInfoItem;
	TMenuItem *CopyBranchNameItem;
	TMenuItem *CopyCommitIDItem;
	TMenuItem *CreBranchItem;
	TMenuItem *DelBranchItem;
	TMenuItem *DelTagItem;
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
	TMenuItem *ResetItem;
	TMenuItem *SelConsoleItem;
	TMenuItem *SelGuiItem;
	TMenuItem *Sep_b_1;
	TMenuItem *Sep_c_1;
	TMenuItem *Sep_c_3;
	TMenuItem *Sep_c_4;
	TMenuItem *Sep_d_1;
	TMenuItem *Sep_d_2;
	TMenuItem *SetTagItem;
	TMenuItem *ShowAuthorAction1;
	TMenuItem *ShowBranchesItem;
	TMenuItem *ShowRemoteBrItem;
	TMenuItem *ShowRemoteItem;
	TMenuItem *ShowTagItem;
	TMenuItem *SoftResetItem;
	TPanel *BranchPanel;
	TPanel *CommitPanel;
	TPanel *DiffPanel;
	TPanel *RightPanel;
	TPopupMenu *BrPopupMenu;
	TPopupMenu *CmPopupMenu;
	TPopupMenu *ComOptPopupMenu;
	TPopupMenu *DiffPopupMenu;
	TSplitter *CommitSplitter;
	TSplitter *DiffSplitter;
	TSplitter *FindSplitter;
	TToolBar *FindBar;
	TToolButton *BarSpcBtn;
	TToolButton *ConsoleBtn;
	TToolButton *FindBtn;
	TToolButton *FindDownBtn;
	TToolButton *FindUpBtn;
	TToolButton *GuiBtn;
	TToolButton *UpdateBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BranchPanelResize(TObject *Sender);
	void __fastcall CommitPanelResize(TObject *Sender);
	void __fastcall DiffPanelResize(TObject *Sender);
	void __fastcall BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall BranchListBoxClick(TObject *Sender);
	void __fastcall BranchListBoxDblClick(TObject *Sender);
	void __fastcall BranchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxClick(TObject *Sender);
	void __fastcall GitListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall GitListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall CheckoutActionExecute(TObject *Sender);
	void __fastcall CreBranchActionExecute(TObject *Sender);
	void __fastcall DelBranchActionExecute(TObject *Sender);
	void __fastcall RenBranchActionExecute(TObject *Sender);
	void __fastcall DelTagActionExecute(TObject *Sender);
	void __fastcall DelTagActionUpdate(TObject *Sender);
	void __fastcall MergeActionExecute(TObject *Sender);
	void __fastcall ShowBranchesActionExecute(TObject *Sender);
	void __fastcall ShowRemoteActionExecute(TObject *Sender);
	void __fastcall ShowAuthorActionExecute(TObject *Sender);
	void __fastcall BrPopupMenuPopup(TObject *Sender);
	void __fastcall SetTagActionExecute(TObject *Sender);
	void __fastcall CheckoutActionUpdate(TObject *Sender);
	void __fastcall CopyCommitIDActionExecute(TObject *Sender);
	void __fastcall CopyCommitIDActionUpdate(TObject *Sender);
	void __fastcall CopyBranchNameActionExecute(TObject *Sender);
	void __fastcall CopyBranchNameActionUpdate(TObject *Sender);
	void __fastcall ResetItemClick(TObject *Sender);
	void __fastcall DiffToolActionExecute(TObject *Sender);
	void __fastcall DiffActionUpdate(TObject *Sender);
	void __fastcall BlameActionExecute(TObject *Sender);
	void __fastcall ArchiveActionExecute(TObject *Sender);
	void __fastcall ArchiveActionUpdate(TObject *Sender);
	void __fastcall AppFextColorActionExecute(TObject *Sender);
	void __fastcall GitPopupMenuPopup(TObject *Sender);
	void __fastcall FindCommitEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindCommitEditChange(TObject *Sender);
	void __fastcall FindCommitEditEnter(TObject *Sender);
	void __fastcall FindCommitEditExit(TObject *Sender);
	void __fastcall FindBtnClick(TObject *Sender);
	void __fastcall FindUpActionExecute(TObject *Sender);
	void __fastcall FindUpActionUpdate(TObject *Sender);
	void __fastcall FindDownActionExecute(TObject *Sender);
	void __fastcall FindDownActionUpdate(TObject *Sender);
	void __fastcall FindCommitEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ConsoleActionExecute(TObject *Sender);
	void __fastcall ConsoleActionUpdate(TObject *Sender);
	void __fastcall SelConsoleItemClick(TObject *Sender);
	void __fastcall GuiActionExecute(TObject *Sender);
	void __fastcall GuiActionUpdate(TObject *Sender);
	void __fastcall SelGuiItemClick(TObject *Sender);
	void __fastcall EditFileActionExecute(TObject *Sender);
	void __fastcall EditFileActionUpdate(TObject *Sender);
	void __fastcall OpenTmpArcActionExecute(TObject *Sender);
	void __fastcall ShowRBranchActionExecute(TObject *Sender);
	void __fastcall ShowTagActionExecute(TObject *Sender);
	void __fastcall CreBranchActionUpdate(TObject *Sender);
	void __fastcall DelBranchActionUpdate(TObject *Sender);
	void __fastcall MergeActionUpdate(TObject *Sender);
	void __fastcall SetTagActionUpdate(TObject *Sender);
	void __fastcall RenBranchActionUpdate(TObject *Sender);
	void __fastcall CommitInfoActionUpdate(TObject *Sender);
	void __fastcall CommitInfoActionExecute(TObject *Sender);
	void __fastcall LogThisFileActionExecute(TObject *Sender);
	void __fastcall UpdateLogActionExecute(TObject *Sender);
	void __fastcall LogThisCommitActionExecute(TObject *Sender);
	void __fastcall LogThisCommitActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString RefHEAD;
	UnicodeString CommitID;
	UnicodeString ParentID;
	UnicodeString BranchName;
	UnicodeString TagNames;
	UnicodeString DiffOpt;
	bool AutoCrlf;

	TStringList *CommitList;

	UsrScrollPanel *BranchScrPanel;
	UsrScrollPanel *CommitScrPanel;
	UsrScrollPanel *DiffScrPanel;

	int MaxGrWidth;
	int MaxAnWidth;
	int MaxDfWidth;

	int LastBrListIdx;

	UnicodeString __fastcall GitExeStr(UnicodeString prm);
	TStringDynArray __fastcall GitExeStrArray(UnicodeString prm);
	UnicodeString __fastcall SaveRevAsTemp(UnicodeString id, UnicodeString fnam);

	git_rec * __fastcall cre_GitRec(UnicodeString msg = EmptyStr);
	void __fastcall ClearCommitList();
	void __fastcall UpdateBranchList();
	void __fastcall UpdateCommitList(UnicodeString commit_id = EmptyStr);

	bool FGitBusy;
	void __fastcall SetGitBusy(bool Value)
	{
		FGitBusy = Value;
		Enabled  = !Value;
		if (Value) cursor_HourGlass(); else cursor_Default();
	}
	__property bool GitBusy = {read = FGitBusy,  write = SetGitBusy};

	int __fastcall GetCurBranchFlag()
	{
		TListBox *lp = BranchListBox;
		return ((lp->Focused() && lp->ItemIndex!=-1)? (int)lp->Items->Objects[lp->ItemIndex] : 0);
	}

	bool __fastcall CurIsHEAD()
	{
		TListBox *lp = BranchListBox;
		int flag = (lp->Focused() && lp->ItemIndex!=-1)? (int)lp->Items->Objects[lp->ItemIndex] : 0;
		return ((flag & GIT_FLAG_HEAD) && (flag & GIT_FLAG_LOCAL));
	}

	UnicodeString __fastcall GetDiffFileName()
	{
		TListBox *lp = DiffListBox;
		UnicodeString lbuf = ((lp->Focused() && lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr);
		return lbuf.Pos(" | ")? Trim(split_tkn(lbuf, " | ")) : EmptyStr;
	}

	void __fastcall SetCommitListIndex(int idx)
	{
		if (idx!=-1) {
			CommitListBox->ItemIndex = idx;
			CommitListBoxClick(NULL);
		}
	}

	UnicodeString __fastcall GetCurBranchName(bool lc_only = false, bool br_only = false, bool av_only = true);

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
	UnicodeString WorkDir;		//Gitトップディレクトリ
	UnicodeString FilterName;	//履歴絞り込み時の対象名
	int HistoryLimit;			//履歴の表示制限数

	UnicodeString RetArcFile;	//[o] 一時アーカイブ名

	__fastcall TGitViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGitViewer *GitViewer;
//---------------------------------------------------------------------------
#endif
