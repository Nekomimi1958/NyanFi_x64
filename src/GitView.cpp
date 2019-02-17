//----------------------------------------------------------------------//
// Gitビュアー															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <System.StrUtils.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "GitView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGitViewer *GitViewer = NULL;

//---------------------------------------------------------------------------
__fastcall TGitViewer::TGitViewer(TComponent* Owner)
	: TForm(Owner)
{
	HistoryLimit = GIT_DEF_HISTLIMIT;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormCreate(TObject *Sender)
{
	CommitList  = new TStringList();

	BranchScrPanel = new UsrScrollPanel(BranchPanel, BranchListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	CommitScrPanel = new UsrScrollPanel(CommitPanel, CommitListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	DiffScrPanel   = new UsrScrollPanel(DiffPanel,   DiffListBox,   USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormShow(TObject *Sender)
{
	RepoDir = ExcludeTrailingPathDelimiter(GetCurrentDir());
	Caption = yen_to_delimiter(RepoDir) + " - Gitビュアー";

	IniFile->LoadPosInfo(this);

	set_ListBoxItemHi(BranchListBox, ListFont);
	set_UsrScrPanel(BranchScrPanel);
	set_ListBoxItemHi(CommitListBox, ListFont);
	set_UsrScrPanel(CommitScrPanel);
	set_ListBoxItemHi(DiffListBox,	 TxtPrvFont);
	set_UsrScrPanel(DiffScrPanel);

	BranchPanel->Width = IniFile->ReadIntGen(_T("GitViewBranchWidth"),	200);
	DiffPanel->Height  = IniFile->ReadIntGen(_T("GitViewDiffHeight"),	120);
	ShowBranchesAction->Checked = IniFile->ReadBoolGen(_T("GitViewShowBranches"));
	AppFextColorAction->Checked = IniFile->ReadBoolGen(_T("GitViewFExtColor"), true);

	CommitSplitter->Color = col_Splitter;
	DiffSplitter->Color   = col_Splitter;

	BranchListBox->Color  = col_bgList;
	CommitListBox->Color  = col_bgList;
	DiffListBox->Color	  = col_bgTxtPrv;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::WmFormShowed(TMessage &msg)
{
	Repaint();
	GetCommitList();
	BranchListBox->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormClose(TObject *Sender, TCloseAction &Action)
{
	BranchListBox->Clear();
	CommitListBox->Clear();
	DiffListBox->Clear();

	HistoryLimit = GIT_DEF_HISTLIMIT;

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("GitViewBranchWidth"),		BranchPanel->Width);
	IniFile->WriteIntGen(_T("GitViewDiffHeight"),		DiffPanel->Height);
	IniFile->WriteBoolGen(_T("GitViewShowBranches"),	ShowBranchesAction->Checked);
	IniFile->WriteBoolGen(_T("GitViewFExtColor"),		AppFextColorAction->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormDestroy(TObject *Sender)
{
	for (int i=0; i<CommitList->Count; i++) delete (git_rec*)CommitList->Objects[i];
	delete CommitList;

	delete BranchScrPanel;
	delete CommitScrPanel;
	delete DiffScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchPanelResize(TObject *Sender)
{
	BranchScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitPanelResize(TObject *Sender)
{
	CommitScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffPanelResize(TObject *Sender)
{
	DiffScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::GitExeStr(UnicodeString prm)
{
	std::unique_ptr<TStringList> o_lst(new TStringList());
	GitBusy = true;
	DWORD exit_code = 0;
	bool res = GitShellExe(prm, RepoDir, o_lst.get(), &exit_code);
	GitBusy = false;

	UnicodeString ret_str;
	if (res) {
		if (exit_code!=0) {
			msgbox_ERR(o_lst->Text);
		}
		else {
			ret_str = o_lst->Text;
			if (!StartsText("archive", prm)) GetCommitList();
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::ClearCommitList()
{
	for (int i=0; i<CommitList->Count; i++) delete (git_rec*)CommitList->Objects[i];
	CommitList->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::GetCommitList()
{
	TListBox *lp = CommitListBox;
	lp->Clear();
	lp->Items->Add("取得中...");
	lp->Repaint();

	GitBusy = true;

	std::unique_ptr<TStringList> o_lst(new TStringList());

	//ブランチ一覧
	lp = BranchListBox;
	lp->Clear();
	RefHEAD = EmptyStr;
	if (GitShellExe("branch", RepoDir, o_lst.get())) { 
		std::unique_ptr<TStringList> lst(new TStringList());
		int idx = -1;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			bool is_cur = remove_top_s(lbuf, '*');
			lst->AddObject(Trim(lbuf), (TObject *)(NativeInt)(is_cur? 1 : 0));
			if (is_cur) {
				idx = i;
				RefHEAD = lbuf;
			}
		}
		lp->Items->Assign(lst.get());
		lp->ItemIndex = idx;
	}
	BranchScrPanel->UpdateKnob();

	//コミット履歴
	UnicodeString log_prm = "log --graph";
	if (ShowBranchesAction->Checked) log_prm += " --branches";
	if (HistoryLimit>0)	log_prm.cat_sprintf(_T(" -%u"), HistoryLimit);
	log_prm += " --date=format:\"%Y-%m-%d %H:%M:%S\" --pretty=format:\"\t%H\t%ad\t%s\t%d\"";

	if (GitShellExe(log_prm, RepoDir, o_lst.get())) { 
		lp = CommitListBox;
		lp->Clear();
		ClearCommitList();
		int max_glen = 0;
		int idx = -1;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			TStringDynArray ibuf = split_strings_tab(lbuf);
			if (ibuf.Length>0) {
				git_rec *gp = new git_rec;
				gp->is_head = false;
				gp->graph	= ibuf[0];
				max_glen	= std::max(max_glen, gp->graph.Length());
				if (ibuf.Length==5) {
					gp->msg  = ibuf[3];
					gp->hash = ibuf[1];

					UnicodeString s = get_in_paren(ibuf[4]);
					TStringDynArray b_buf = SplitString(s, ",");
					for (int j=0; j<b_buf.Length; j++) {
						UnicodeString ss = Trim(b_buf[j]);
						if (remove_top_text(ss, "tag: ")) {
							gp->tag = ss;
						}
						else if (ss.Pos("/")) {
							if (!gp->branch_r.IsEmpty()) gp->branch_r += ",";
							gp->branch_r += ss;
						}
						else {
							gp->is_head = remove_top_text(ss, "HEAD -> ");
							gp->branch	= ss;
							if (gp->is_head) idx = i;
						}
					}

					try {
						gp->f_time = VarToDateTime(ibuf[2]);
					}
					catch (...) {
						gp->f_time = 0;
					}
				}
				CommitList->AddObject(gp->hash, (TObject *)gp);
			}
		}

		lp->Tag = (max_glen + 1) * lp->ItemHeight / 3;
		lp->Items->Assign(CommitList);
		if (idx!=-1) {
			lp->ItemIndex = idx;
			CommitListBoxClick(NULL);
		}
	}
	CommitScrPanel->UpdateKnob();

	GitBusy = false;
}

//---------------------------------------------------------------------------
//ブランチ一覧の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)? (State.Contains(odFocused)? col_selItem : col_oppItem) : lp->Color;
	cv->FillRect(Rect);

	int xp = Rect.Left + 4;
	int yp = Rect.Top + 2;

	UnicodeString s  = _T("\u25b6");
	if ((int)lp->Items->Objects[Index]==1) {
		cv->Font->Color = col_GitHEAD;
		cv->TextOut(xp, yp, s);
	}
	xp += cv->TextWidth(s);

	cv->Font->Color = col_fgList;
	cv->TextOut(xp, yp, lp->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchListBoxDblClick(TObject *Sender)
{
	ChckoutAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr  = get_KeyStr(Key, Shift);
	UnicodeString cmd_F = Key_to_CmdF(KeyStr);

	if		(ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
		;
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (is_ToRightOpe(KeyStr, cmd_F))			CommitListBox->SetFocus();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	else if (equal_ENTER(KeyStr)) 					ChckoutAction->Execute();
	else return;

	if (!is_DialogKey(Key)) Key = 0;
}

//---------------------------------------------------------------------------
// コミット履歴の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)? (State.Contains(odFocused)? col_selItem : col_oppItem) : lp->Color;
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;

	git_rec *gp = (git_rec *)lp->Items->Objects[Index];
	if (gp) {
		//グラフ
		if (!gp->graph.IsEmpty()) {
			git_rec *gp1 = (Index>0)? (git_rec *)lp->Items->Objects[Index - 1] : NULL;
			git_rec *gp2 = (Index<(lp->Count - 1))? (git_rec *)lp->Items->Objects[Index + 1] : NULL;
			UnicodeString s1 = gp1? gp1->graph : EmptyStr;
			UnicodeString s2 = gp2? gp2->graph : EmptyStr;
			TRect rc = Rect; rc.Left = xp;
			draw_GitGraph(gp->graph, s1, s2, cv, rc, gp->is_head);
		}

		xp += lp->Tag;
		if (!gp->hash.IsEmpty()) {
			//ハッシュ
			cv->Font->Color = col_GitHash;
			cv->TextOut(xp, yp, gp->hash.SubString(1, 7));
			xp += cv->TextWidth("9999999 ");

			//日時
			UnicodeString tmp = FormatDateTime(TimeStampFmt, gp->f_time);
			cv->Font->Color = get_TimeColor(gp->f_time, col_fgList);
			cv->TextOut(xp, yp, tmp);
			xp += cv->TextWidth(tmp) + Scaled8;

			//タグ
			if (!gp->tag.IsEmpty()) {
				TColor org_bg = cv->Brush->Color;
				int mk_h = cv->TextHeight("Q");
				int mk_w = mk_h/4;
				TPoint shape[3]  = {Point(xp, yp + mk_h/2), Point(xp + mk_w, yp), Point(xp + mk_w, yp + mk_h - 1)};
				cv->Pen->Style	 = psSolid;
				cv->Pen->Color	 = clYellow;
				cv->Brush->Color = clYellow;
				cv->Polygon(shape, 2);
				xp += (mk_w + 1);
				cv->Brush->Color = org_bg;
				out_TextEx(cv, xp, yp, gp->tag, col_bgList, col_GitTag, Scaled8);
			}

			//ブランチ名
			if (!gp->branch.IsEmpty()) {
				if (gp->is_head) out_TextEx(cv, xp, yp, _T("\u25b6"), col_GitHEAD);
				out_TextEx(cv, xp, yp, gp->branch, col_bgList, col_GitBra, Scaled8);
			}
			if (!gp->branch_r.IsEmpty()) {
				out_TextEx(cv, xp, yp, gp->branch_r, col_bgList, col_GitBraR, Scaled8);
			}

			//メッセージ
			cv->Font->Color = col_fgList;
			cv->TextOut(xp, yp, gp->msg);
		}
	}
	else {
		cv->Font->Color = col_Teal;
		cv->TextOut(xp, yp, lp->Items->Strings[Index]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxClick(TObject *Sender)
{
	DiffListBox->Clear();
	DiffID = EmptyStr;

	TListBox *lp = CommitListBox;
	int idx = lp->ItemIndex;
	if (idx==-1 || idx==lp->Count-1) return;

	git_rec *gp = (git_rec *)lp->Items->Objects[idx];
	if (!gp || gp->hash.IsEmpty()) return;

	DiffID = gp->hash;
	int b_idx =BranchListBox->Items->IndexOf(gp->branch);
	if (b_idx!=-1) BranchListBox->ItemIndex = b_idx;

	std::unique_ptr<TStringList> o_lst(new TStringList());
	UnicodeString prm = "diff --stat " + gp->hash + "^ " + gp->hash;
	GitBusy = true;
	if (GitShellExe(prm, RepoDir, o_lst.get()) && o_lst->Count>0) {
		TListBox *lp = DiffListBox;
		int max_fwd = 0;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			if (lbuf.Pos(" | "))
				max_fwd = std::max(max_fwd, lp->Canvas->TextWidth(get_tkn(lbuf, " | ") + " "));
		}
		lp->Tag = max_fwd;
		lp->Items->Assign(o_lst.get());
		lp->ItemIndex = 0;
	}
	GitBusy = false;
	DiffScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if		(ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
													CommitListBoxClick(NULL);
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))			BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))			DiffListBox->SetFocus();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	else if (USAME_TI(get_CmdStr(cmd_F), "Pack"))	ArchiveAction->Execute();
	else return;

	if (!is_DialogKey(Key)) Key = 0;
}

//---------------------------------------------------------------------------
//差分概略の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)? (State.Contains(odFocused)? col_selItem : col_oppItem) : lp->Color;
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (lbuf.Pos(" | ")) {
		UnicodeString s = split_tkn(lbuf, " | ");
		out_TextEx(cv, xp, yp, s,
			AppFextColorAction->Checked? get_ExtColor(get_extension(Trim(s))) : col_fgTxtPrv);
		TRect rc = Rect; rc.Left = xp = lp->Tag;
		RuledLnTextOut("│", cv, rc, col_fgTxtPrv);
		xp = rc.Left;
		s = split_tkn(lbuf, " ") + " ";
		out_TextEx(cv, xp, yp, s, col_fgTxtPrv);
		if (!StartsText("Bin", s)) {
			for (int i=1; i<=lbuf.Length(); i++) {
				WideChar c = lbuf[i];
				out_TextEx(cv, xp, yp, c, ((c=='-')? clRed : (c=='+')? clGreen : col_fgTxtPrv));
			}
		}
		else {
			out_TextEx(cv, xp, yp, lbuf, col_fgTxtPrv);
		}
	}
	else {
		for (int i=1; i<=lbuf.Length(); i++) {
			WideChar c = lbuf[i];
			out_TextEx(cv, xp, yp, c, ((c=='-')? clRed : (c=='+')? clGreen : col_fgTxtPrv));
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if		(ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
		;
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))			BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))			CommitListBox->SetFocus();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	else return;

	if (!is_DialogKey(Key)) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::GitListBoxMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	((TListBox *)Sender)->SetFocus();
}

//---------------------------------------------------------------------------
//チェックアウト
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ChckoutActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName();
	if (!bnam.IsEmpty()) GitExeStr("checkout " + bnam);
}
//---------------------------------------------------------------------------
//ブランチの作成
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CreBranchActionExecute(TObject *Sender)
{
	UnicodeString bnam = InputBox("ブランチの作成", "ブランチ名", EmptyStr);
	if (!bnam.IsEmpty()) GitExeStr("branch " + bnam);
}
//---------------------------------------------------------------------------
//ブランチの削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelBranchActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName();
	if (!bnam.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("ブランチ[%s]を削除してもよいですか?"), bnam.c_str());
		TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation,
							TMsgDlgButtons() << mbYes << mbNo << mbCancel, mbNo);
		TCheckBox *cp = new TCheckBox(MsgDlg);
		MsgDlg->ClientHeight = MsgDlg->ClientHeight + cp->Height + 12;
		cp->Caption = "強制的に削除(&D)";
		cp->Parent	= MsgDlg;
		cp->Left	= 20;
		cp->Top		= MsgDlg->ClientHeight - cp->Height - 12;
		cp->Width	= MsgDlg->ClientWidth - cp->Left;

		if (MsgDlg->ShowModal()==mrYes) {
			UnicodeString prm;
			prm.sprintf(_T("branch -%s %s"), (cp->Checked? _T("D") : _T("d")), bnam.c_str());
			GitExeStr(prm);
		}

		delete MsgDlg;
	}
}

//---------------------------------------------------------------------------
//ブランチの名前を変更
//---------------------------------------------------------------------------
void __fastcall TGitViewer::RenBranchActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName();
	if (!bnam.IsEmpty()) {
		bnam = InputBox("ブランチの名前を変更", "ブランチ名", bnam);
		if (!bnam.IsEmpty()) GitExeStr("branch -m " + bnam);
	}
}
//---------------------------------------------------------------------------
//タグを付ける
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SetTagActionExecute(TObject *Sender)
{
	UnicodeString tnam = InputBox("タグを付ける", "タグ", EmptyStr);
	if (!tnam.IsEmpty()) GitExeStr("tag " + tnam);
}
//---------------------------------------------------------------------------
//タグの削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelTagActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->tag.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("タグ[%s]を削除してもよいですか?"), gp->tag.c_str());
		if (msgbox_Sure(msg)) GitExeStr("tag -d " + gp->tag);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelTagActionUpdate(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	((TAction *)Sender)->Visible = (gp && !gp->tag.IsEmpty());
}

//---------------------------------------------------------------------------
//マージ
//---------------------------------------------------------------------------
void __fastcall TGitViewer::MergeActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName();
	if (!CurIsHEAD() && !bnam.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("[%s] を [%s] にマージしますか?"), bnam.c_str(), RefHEAD.c_str());
		if (msgbox_Sure(msg)) GitExeStr("merge " + bnam);
	}
}

//---------------------------------------------------------------------------
//ブランチ名をコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyBranchNameActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->branch.IsEmpty()) copy_to_Clipboard(gp->branch);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyBranchNameActionUpdate(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	((TAction *)Sender)->Visible = (gp && !gp->branch.IsEmpty());
}
//---------------------------------------------------------------------------
//コミットIDをコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyCommitIDActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->hash.IsEmpty()) copy_to_Clipboard(gp->hash);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyCommitIDActionUpdate(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	((TAction *)Sender)->Visible = (gp && !gp->hash.IsEmpty());
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::ActTagActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = CurIsHEAD();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::InactBranchActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CurIsHEAD();
}

//---------------------------------------------------------------------------
//直前のコミットに戻す
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BrPopupMenuPopup(TObject *Sender)
{
	ResetItem->Visible = CurIsHEAD();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::RestItemClick(TObject *Sender)
{
	int tag = ((TMenuItem *)Sender)->Tag;
	UnicodeString prm = (tag==2)? "Hard" : (tag==0)? "Soft" : "Mixed";
	UnicodeString msg;
	msg.sprintf(_T("[%s]を直前のコミットに戻しますか(%s)?"), RefHEAD.c_str(), prm.c_str());
	if (msgbox_Sure(msg)) GitExeStr("reset --" + prm.LowerCase() + " HEAD^");
}

//---------------------------------------------------------------------------
//Diffツールを起動
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffToolActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		UnicodeString prm;
		prm.sprintf(_T("difftool -y %s^:%s %s:%s"),
			DiffID.c_str(), fnam.c_str(), DiffID.c_str(), fnam.c_str());
		GitExeStr(prm);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffToolActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty();
}

//---------------------------------------------------------------------------
//アーカイブ作成
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->hash.IsEmpty()) {
		UserModule->PrepareSaveDlg(_T("アーカイブの作成"), _T("アーカイブ (*.zip)|*.zip"), _T("archive.zip"), RepoDir);
		if (UserModule->SaveDlg->Execute()) {
			UnicodeString arc_name = UserModule->SaveDlg->FileName;
			UnicodeString prm;
			prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), gp->hash.c_str(), yen_to_slash(arc_name).c_str());
			GitExeStr(prm);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionUpdate(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	((TAction *)Sender)->Visible = (gp && !gp->hash.IsEmpty());
}

//---------------------------------------------------------------------------
//他のブランチも表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowBranchesActionExecute(TObject *Sender)
{
	ShowBranchesAction->Checked = !ShowBranchesAction->Checked;
	GetCommitList();
}
//---------------------------------------------------------------------------
//拡張子別配色を適用
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AppFextColorActionExecute(TObject *Sender)
{
	AppFextColorAction->Checked = !AppFextColorAction->Checked;
	DiffListBox->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_GIT) _T("#GitViewer"));
}
//---------------------------------------------------------------------------

