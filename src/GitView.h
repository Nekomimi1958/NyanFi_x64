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
	UnicodeString tag;		//タグ
	UnicodeString author;	//Author名
	TDateTime f_time;
	bool is_head;
	bool is_work;
	bool is_index;
};

#define GIT_DEF_HISTLIMIT	30

//---------------------------------------------------------------------------
class TGitViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AppFextColorAction;
	TAction *ArchiveAction;
	TAction *BlameAction;
	TAction *ChckoutAction;
	TAction *ConsoleAction;
	TAction *CopyBranchNameAction;
	TAction *CopyCommitIDAction;
	TAction *DelTagAction;
	TAction *DiffToolAction;
	TAction *FindDownAction;
	TAction *FindUpAction;
	TAction *MergeAction;
	TAction *RenBranchAction;
	TAction *SetTagAction;
	TAction *ShowAuthorAction;
	TAction *ShowBranchesAction;
	TAction *ShowRemoteAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TEdit *FindCommitEdit;
	TListBox *BranchListBox;
	TListBox *CommitListBox;
	TListBox *DiffListBox;
	TMenuItem *AppFextColItem;
	TMenuItem *ArchiveItem;
	TMenuItem *BlameItem;
	TMenuItem *CheckoutItem;
	TMenuItem *CopyBranchNameItem;
	TMenuItem *CopyCommitIDItem;
	TMenuItem *Cosole1;
	TMenuItem *CreBranchItem;
	TMenuItem *DelBranchItem;
	TMenuItem *DiffToolItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *HardResetItem;
	TMenuItem *MergeItem;
	TMenuItem *MixedResetItem;
	TMenuItem *N2;
	TMenuItem *RenBranchItem;
	TMenuItem *ResetItem;
	TMenuItem *Sep_c_1;
	TMenuItem *Sep_c_2;
	TMenuItem *Sep_c_3;
	TMenuItem *Sep_c_4;
	TMenuItem *Sep_d_1;
	TMenuItem *SetTagItem;
	TMenuItem *ShowAuthorAction1;
	TMenuItem *ShowBranchesItem;
	TMenuItem *ShowRemoteItem;
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
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *FintBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BranchPanelResize(TObject *Sender);
	void __fastcall CommitPanelResize(TObject *Sender);
	void __fastcall DiffPanelResize(TObject *Sender);
	void __fastcall BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall BranchListBoxDblClick(TObject *Sender);
	void __fastcall BranchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxClick(TObject *Sender);
	void __fastcall GitListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ChckoutActionExecute(TObject *Sender);
	void __fastcall CreBranchActionExecute(TObject *Sender);
	void __fastcall DelBranchActionExecute(TObject *Sender);
	void __fastcall RenBranchActionExecute(TObject *Sender);
	void __fastcall DelTagActionExecute(TObject *Sender);
	void __fastcall DelTagActionUpdate(TObject *Sender);
	void __fastcall MergeActionExecute(TObject *Sender);
	void __fastcall ShowBranchesActionExecute(TObject *Sender);
	void __fastcall SetTagActionExecute(TObject *Sender);
	void __fastcall ActTagActionUpdate(TObject *Sender);
	void __fastcall InactBranchActionUpdate(TObject *Sender);
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
	void __fastcall ShowRemoteActionExecute(TObject *Sender);
	void __fastcall BrPopupMenuPopup(TObject *Sender);
	void __fastcall GitPopupMenuPopup(TObject *Sender);
	void __fastcall FindCommitEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindCommitEditChange(TObject *Sender);
	void __fastcall FindUpActionExecute(TObject *Sender);
	void __fastcall FindUpActionUpdate(TObject *Sender);
	void __fastcall FindDownActionExecute(TObject *Sender);
	void __fastcall FindDownActionUpdate(TObject *Sender);
	void __fastcall ShowAuthorActionExecute(TObject *Sender);
	void __fastcall FindCommitEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ConsoleActionExecute(TObject *Sender);
	void __fastcall ConsoleActionUpdate(TObject *Sender);
	void __fastcall Cosole1Click(TObject *Sender);
	void __fastcall UpdateBtnClick(TObject *Sender);
	void __fastcall FintBtnClick(TObject *Sender);
	void __fastcall FindCommitEditEnter(TObject *Sender);
	void __fastcall FindCommitEditExit(TObject *Sender);
	void __fastcall GitListBoxKeyPress(TObject *Sender, System::WideChar &Key);

private:	// ユーザー宣言
	UnicodeString RefHEAD;
	UnicodeString CommitID;
	UnicodeString ParentID;
	UnicodeString DiffOpt;

	TStringList *CommitList;

	UsrScrollPanel *BranchScrPanel;
	UsrScrollPanel *CommitScrPanel;
	UsrScrollPanel *DiffScrPanel;

	int MaxGrWidth;
	int MaxAnWidth;
	int MaxDfWidth;

	UnicodeString __fastcall GitExeStr(UnicodeString prm);
	void __fastcall ClearCommitList();
	void __fastcall UpdateCommitList();

	bool FGitBusy;
	void __fastcall SetGitBusy(bool Value)
	{
		FGitBusy = Value;
		if (Value) {
			cursor_HourGlass();
			Enabled = false;
		}
		else {
			cursor_Default();
			Enabled = true;
		}
	}
	__property bool GitBusy = {read = FGitBusy,  write = SetGitBusy};

	bool __fastcall CurIsHEAD()
	{
		TListBox *lp = BranchListBox;
		return (lp->Focused() && lp->ItemIndex!=-1 && (int)lp->Items->Objects[lp->ItemIndex]==1);
	}
	UnicodeString __fastcall GetCurBranchName()
	{
		TListBox *lp = BranchListBox;
		return ((lp->Focused() && lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr);
	}

	git_rec * __fastcall GetCurCommitPtr()
	{
		TListBox *lp = CommitListBox;
		return ((lp->Focused() && lp->ItemIndex!=-1)? (git_rec *)lp->Items->Objects[lp->ItemIndex] : NULL);
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

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
	UnicodeString WorkDir;
	int HistoryLimit;

	__fastcall TGitViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGitViewer *GitViewer;
//---------------------------------------------------------------------------
#endif
