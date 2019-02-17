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

//---------------------------------------------------------------------------
struct git_rec {
	UnicodeString graph;
	UnicodeString msg;
	UnicodeString hash;
	UnicodeString branch;
	UnicodeString branch_r;
	UnicodeString tag;
	TDateTime f_time;
	bool is_head;
};

#define GIT_DEF_HISTLIMIT	30

//---------------------------------------------------------------------------
class TGitViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *ChckoutAction;
	TAction *CopyBranchNameAction;
	TAction *CopyCommitIDAction;
	TAction *DelTagAction;
	TAction *MergeAction;
	TAction *RenBranchAction;
	TAction *SetTagAction;
	TAction *ShowBranchesAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TListBox *BranchListBox;
	TListBox *CommitListBox;
	TListBox *DiffListBox;
	TMenuItem *CheckoutItem;
	TMenuItem *CopyBranchNameItem;
	TMenuItem *CopyCommitIDItem;
	TMenuItem *CreBranchItem;
	TMenuItem *DelBranchItem;
	TMenuItem *MergeItem;
	TMenuItem *N2;
	TMenuItem *RenBranchItem;
	TMenuItem *Sep_c_1;
	TMenuItem *Sep_c_2;
	TMenuItem *SetTagItem;
	TMenuItem *ShowBranchesItem;
	TPanel *BranchPanel;
	TPanel *CommitPanel;
	TPanel *DiffPanel;
	TPanel *RightPanel;
	TPopupMenu *BrPopupMenu;
	TPopupMenu *CmPopupMenu;
	TSplitter *CommitSplitter;
	TSplitter *DiffSplitter;
	TMenuItem *ResetItem;
	TMenuItem *SoftResetItem;
	TMenuItem *MixedResetItem;
	TMenuItem *HardResetItem;
	TPopupMenu *DiffPopupMenu;
	TAction *DiffToolAction;
	TMenuItem *DiffToolItem;
	TAction *ArchiveAction;
	TMenuItem *Sep_c_3;
	TMenuItem *ArchiveItem;
	TAction *AppFextColorAction;
	TMenuItem *Sep_d_1;
	TMenuItem *AppFextColItem;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall BranchListBoxDblClick(TObject *Sender);
	void __fastcall CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CommitListBoxClick(TObject *Sender);
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
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DiffPanelResize(TObject *Sender);
	void __fastcall CommitPanelResize(TObject *Sender);
	void __fastcall BranchPanelResize(TObject *Sender);
	void __fastcall BranchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CopyCommitIDActionExecute(TObject *Sender);
	void __fastcall CopyCommitIDActionUpdate(TObject *Sender);
	void __fastcall CopyBranchNameActionExecute(TObject *Sender);
	void __fastcall CopyBranchNameActionUpdate(TObject *Sender);
	void __fastcall GitListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall BrPopupMenuPopup(TObject *Sender);
	void __fastcall RestItemClick(TObject *Sender);
	void __fastcall DiffToolActionExecute(TObject *Sender);
	void __fastcall DiffToolActionUpdate(TObject *Sender);
	void __fastcall DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ArchiveActionExecute(TObject *Sender);
	void __fastcall ArchiveActionUpdate(TObject *Sender);
	void __fastcall AppFextColorActionExecute(TObject *Sender);


private:	// ユーザー宣言
	UnicodeString RepoDir;
	UnicodeString RefHEAD;
	UnicodeString DiffID;

	TStringList *CommitList;

	UsrScrollPanel *BranchScrPanel;
	UsrScrollPanel *CommitScrPanel;
	UsrScrollPanel *DiffScrPanel;

	UnicodeString __fastcall GitExeStr(UnicodeString prm);
	void __fastcall ClearCommitList();
	void __fastcall GetCommitList();

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
		UnicodeString lbuf = (!DiffID.IsEmpty() && (lp->Focused() && lp->ItemIndex!=-1)?
													 lp->Items->Strings[lp->ItemIndex] : EmptyStr);
		return lbuf.Pos(" | ")? Trim(split_tkn(lbuf, " | ")) : EmptyStr;
	}

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
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
