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
	Caption = yen_to_delimiter(WorkDir) + " - Gitビュアー";

	IniFile->LoadPosInfo(this);

	FindBar->GradientStartColor = col_bgTlBar1;
	FindBar->GradientEndColor	= col_bgTlBar2;
	FindBar->Font->Color		= col_fgTlBar;
	FindBar->HotTrackColor		= col_htTlBar;
	FindSplitter->Color 		= Mix2Colors(col_bgTlBar1, col_bgTlBar2);

	set_ListBoxItemHi(BranchListBox, ListFont);
	set_UsrScrPanel(BranchScrPanel);
	set_ListBoxItemHi(CommitListBox, ListFont);
	set_UsrScrPanel(CommitScrPanel);
	set_ListBoxItemHi(DiffListBox,	 TxtPrvFont);
	set_UsrScrPanel(DiffScrPanel);

	BranchPanel->Width	  = IniFile->ReadIntGen(_T("GitViewBranchWidth"),	200);
	DiffPanel->Height	  = IniFile->ReadIntGen(_T("GitViewDiffHeight"),	120);

	FindCommitEdit->Font->Assign(DialogFont);
	FindCommitEdit->Width = IniFile->ReadIntGen(_T("GitViewFindWidth"),		200);

	ShowBranchesAction->Checked = IniFile->ReadBoolGen(_T("GitViewShowBranches"));
	ShowRemoteAction->Checked	= IniFile->ReadBoolGen(_T("GitViewShowRemote"));
	ShowAuthorAction->Checked	= IniFile->ReadBoolGen(_T("GitViewShowAuthor"));
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
	FindCommitEdit->Color = col_Invalid;
	Repaint();

	AutoCrlf   = SameText(Trim(GitExeStr("config --get core.autocrlf")), "true");
	RetArcFile = EmptyStr;

	UpdateCommitList();
	CommitListBox->SetFocus();
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
	IniFile->WriteIntGen(_T("GitViewFindWidth"),		FindCommitEdit->Width);
	IniFile->WriteBoolGen(_T("GitViewShowBranches"),	ShowBranchesAction);
	IniFile->WriteBoolGen(_T("GitViewShowRemote"),		ShowRemoteAction);
	IniFile->WriteBoolGen(_T("GitViewShowAuthor"),		ShowAuthorAction);
	IniFile->WriteBoolGen(_T("GitViewFExtColor"),		AppFextColorAction);
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
//git の実行結果文字列を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::GitExeStr(UnicodeString prm)
{
	GitBusy  = true;
	UnicodeString cmd = get_tkn(prm, " ");
	if (SameText("archive", cmd)) CommitListBox->Invalidate();
	std::unique_ptr<TStringList> o_lst(new TStringList());
	DWORD exit_code;
	bool res = GitShellExe(prm, WorkDir, o_lst.get(), &exit_code);
	GitBusy  = false;

	UnicodeString ret_str;
	if (res) {
		if (exit_code!=0) {
			msgbox_ERR(o_lst->Text);
			ret_str = "ERROR";
		}
		else {
			ret_str = o_lst->Text;
			if (!contained_wd_i(_T("archive|cat-file|config|difftool|gui"), cmd))
				UpdateCommitList();
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//指定リビジョンのファイルを一時ディレクトリに保存
//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::SaveRevAsTemp(UnicodeString id, UnicodeString fnam)
{
	try {
		//ファイルイメージの取得
		UnicodeString src_nam = id + ":" + fnam;
		UnicodeString prm;  prm.sprintf(_T("cat-file -p %s"), src_nam.c_str());
		std::unique_ptr<TMemoryStream> o_ms(new TMemoryStream());
		GitBusy  = true;	DiffListBox->Invalidate();
		DWORD exit_code;
		bool res = GitShellExe(prm, WorkDir, o_ms.get(), &exit_code);
		GitBusy  = false;	DiffListBox->Invalidate();
		if (!res || exit_code!=0) UserAbort(USTR_FaildProc);

		//コードページのチェック
		bool has_bom;
		int code_page = get_MemoryCodePage(o_ms.get(), &has_bom);
		if (code_page<=0) UserAbort(USTR_NotText);

		//改行コードのチェック
		std::unique_ptr<TStringList> o_lst(new TStringList());
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
		UnicodeString line_brk = get_StreamLineBreak(o_ms.get(), code_page);
		if		(SameStr(line_brk, "CR")) o_lst->LineBreak = "\r";
		else if (SameStr(line_brk, "LF")) o_lst->LineBreak = "\n";
		else o_lst->LineBreak = "\r\n";
		//必要なら変換
		if (!SameStr(o_lst->LineBreak, "\r\n") && AutoCrlf) o_lst->LineBreak = "\r\n";

		//リストに読み込み
		o_ms->Seek(0, soFromBeginning);
		o_lst->LoadFromStream(o_ms.get(), enc.get());

		//保存
		o_lst->WriteBOM = has_bom;
		UnicodeString tmp_nam = TempPathA + CommitID.SubString(1, 8) + "_" + ReplaceStr(fnam, "/", "_");
		if (!saveto_TextFile(tmp_nam, o_lst.get(), enc.get())) UserAbort(USTR_FaildProc);
		return tmp_nam;
	}
	catch (EAbort &e) {
		msgbox_ERR(e.Message);
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::ClearCommitList()
{
	for (int i=0; i<CommitList->Count; i++) delete (git_rec*)CommitList->Objects[i];
	CommitList->Clear();
}

//---------------------------------------------------------------------------
//ブランチ一覧/コミット履歴の更新
//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateCommitList()
{
	TListBox *lp = CommitListBox;
	lp->Clear();
	lp->Items->Add("取得中...");
	lp->Repaint();

	CommitScrPanel->HitLines->Clear();

	GitBusy = true;
	std::unique_ptr<TStringList> o_lst(new TStringList());
	MaxGrWidth = MaxAnWidth = 0;

	//----------------------------------
	//ブランチ一覧
	//----------------------------------
	lp = BranchListBox;
	lp->Clear();
	RefHEAD = EmptyStr;
	if (GitShellExe("branch", WorkDir, o_lst.get())) { 
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

	//----------------------------------
	//コミット履歴
	//----------------------------------
	lp = CommitListBox;
	lp->Clear();
	ClearCommitList();

	//ワーキングツリー/インデックス
	if (GitShellExe("status --porcelain", WorkDir, o_lst.get())) { 
		int flag[8][2] = {};
		UnicodeString stt_wk, stt_ix;
		UnicodeString flag_str = "MADRCU?";
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			if (lbuf.Length()<2) continue;
			int p0 = flag_str.Pos(lbuf[1]);
			int p1 = flag_str.Pos(lbuf[2]);
			if (p0>0) flag[p0][0]++;
			if (p1>0) flag[p1][1]++;
		}
		for (int i=1; i<8; i++) {
			if (flag[i][1]>0) stt_wk.cat_sprintf(_T(" %c:%u"), flag_str[i], flag[i][1]);
			if (flag[i][0]>0) stt_ix.cat_sprintf(_T(" %c:%u"), flag_str[i], flag[i][0]);
		}
		stt_wk = def_if_empty(Trim(stt_wk), "Clean");
		stt_ix = def_if_empty(Trim(stt_ix), "Nothing to commit");

		//ワーキングツリー
		git_rec *gp  = new git_rec;
		gp->is_head  = false;
		gp->is_work  = true;
		gp->is_index = false;
		gp->graph	 = "*";
		gp->msg		 = stt_wk;

		CommitList->AddObject(EmptyStr, (TObject *)gp);
		//インデックス
		gp = new git_rec;
		gp->is_head  = false;
		gp->is_work  = false;
		gp->is_index = true;
		gp->graph	 = "*";
		gp->msg		 = stt_ix;
		CommitList->AddObject(EmptyStr, (TObject *)gp);
		//セパレータ
		gp = new git_rec;
		gp->is_work = gp->is_index = gp->is_head = false;
		gp->graph	= "-";
		CommitList->AddObject(EmptyStr, (TObject *)gp);
	}

	//コミット
	UnicodeString prm = "log --graph";
	if (ShowBranchesAction->Checked) prm += " --branches";
	if (HistoryLimit>0) prm.cat_sprintf(_T(" -%u"), HistoryLimit);
	prm += " --date=format:\"%Y-%m-%d %H:%M:%S\" --pretty=format:\"\t%H\t%P\t%ad\t%s\t%d\t%an\"";
	//グラフ / コミットID / 親ID / 日時 / 件名 / ブランチ / タグ / Author名
	int idx_h = -1;
	if (GitShellExe(prm, WorkDir, o_lst.get())) {
		int max_gr_len = 0, max_an_wd = 0;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			TStringDynArray ibuf = split_strings_tab(lbuf);
			if (ibuf.Length>0) {
				git_rec *gp = new git_rec;
				gp->is_work = gp->is_index = gp->is_head = false;
				gp->graph	= ibuf[0];
				max_gr_len	= std::max(max_gr_len, gp->graph.Length());

				UnicodeString br_str;
				if (ibuf.Length==7) {
					gp->hash   = ibuf[1];
					gp->parent = ibuf[2];
					gp->msg    = ibuf[4];
					gp->author = ibuf[6];
					max_an_wd = std::max(max_an_wd, lp->Canvas->TextWidth(gp->author));

					br_str = get_in_paren(ibuf[5]);
					TStringDynArray b_buf = SplitString(br_str, ",");
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
						}
					}

					try {
						gp->f_time = VarToDateTime(ibuf[3]);
					}
					catch (...) {
						gp->f_time = 0;
					}
				}

				lbuf = gp->hash.SubString(1, 7);
				if (ShowAuthorAction->Checked) lbuf.sprintf(_T(" %s"), gp->author.c_str());
				lbuf.sprintf(_T(" %s %s"), br_str.c_str(), gp->msg.c_str());
				int idx = CommitList->AddObject(lbuf, (TObject *)gp);
				if (gp->is_head) idx_h = idx;
			}
		}

		MaxGrWidth = (max_gr_len + 1) * lp->ItemHeight / 3;
		MaxAnWidth = max_an_wd;
	}

	//リストボックスに割り当て
	lp->Items->Assign(CommitList);
	SetCommitListIndex(idx_h);
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

	cv->Font->Color = is_SelFgCol(State)? col_fgSelItem : col_fgList;
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

	Key = 0;
}

//---------------------------------------------------------------------------
// コミット履歴の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;

	UnicodeString kwd = FindCommitEdit->Text;
	bool is_match = !kwd.IsEmpty() && ContainsText(lp->Items->Strings[Index], kwd);

	TCanvas *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)?
						((!State.Contains(odFocused) || GitBusy)? col_oppItem : col_selItem) :
						(is_match? col_matchItem : lp->Color);
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;
	bool use_fgsel = is_SelFgCol(State);

	git_rec *gp = (git_rec *)lp->Items->Objects[Index];
	if (gp) {
		//グラフ
		if (USAME_TS(gp->graph, "-")) {
			draw_Separator(cv, Rect);
		}
		else if (!gp->graph.IsEmpty()) {
			git_rec *gp1 = (Index>0)? (git_rec *)lp->Items->Objects[Index - 1] : NULL;
			git_rec *gp2 = (Index<(lp->Count - 1))? (git_rec *)lp->Items->Objects[Index + 1] : NULL;
			UnicodeString s1 = (gp1 && !USAME_TS(gp1->graph, "-"))? gp1->graph : EmptyStr;
			UnicodeString s2 = (!gp->is_index && gp2)? gp2->graph : EmptyStr;
			TRect rc = Rect; rc.Left = xp;
			draw_GitGraph(gp->graph, s1, s2, cv, rc, gp->is_head, (gp->is_work || gp->is_index));
		}

		xp += MaxGrWidth;
		if (gp->is_work || gp->is_index) {
			UnicodeString s1 = "作業ツリー ";
			UnicodeString s2 = "インデックス ";
			UnicodeString s  = gp->is_work? s1 : s2;
			xp += (std::max(cv->TextWidth(s1), cv->TextWidth(s2)) - cv->TextWidth(s));
			cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
			out_TextEx(cv, xp, yp, s);
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
			cv->TextOut(xp, yp, gp->msg);
		}
		else if (!gp->hash.IsEmpty()) {
			//ハッシュ
			cv->Font->Color = use_fgsel? col_fgSelItem : col_GitHash;
			cv->TextOut(xp, yp, gp->hash.SubString(1, 7));
			xp += cv->TextWidth("9999999 ");

			//Author
			if (ShowAuthorAction->Checked) {
				cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
				cv->TextOut(xp, yp, gp->author);
				xp += (MaxAnWidth + Scaled8);
			}

			//日時
			UnicodeString tmp = FormatDateTime(TimeStampFmt, gp->f_time);
			cv->Font->Color = use_fgsel? col_fgSelItem : get_TimeColor(gp->f_time, col_fgList);
			cv->TextOut(xp, yp, tmp);
			xp += (cv->TextWidth(tmp) + Scaled8);

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
			if (ShowRemoteAction->Checked && !gp->branch_r.IsEmpty()) {
				out_TextEx(cv, xp, yp, gp->branch_r, col_bgList, col_GitBraR, Scaled8);
			}

			//メッセージ
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
			cv->TextOut(xp, yp, gp->msg);
		}
	}
	else {
		cv->Font->Color = use_fgsel? col_fgSelItem : col_Teal;
		cv->TextOut(xp, yp, lp->Items->Strings[Index]);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxClick(TObject *Sender)
{
	DiffListBox->Clear();
	CommitID = ParentID = DiffOpt = EmptyStr;
	MaxDfWidth = 0;

	TListBox *lp = CommitListBox;
	int idx = lp->ItemIndex;
	if (idx==-1 || idx==lp->Count-1) return;

	git_rec *gp = (git_rec *)lp->Items->Objects[idx];
	if (!gp) return;
	if (!gp->is_work && !gp->is_index && gp->hash.IsEmpty()) return;

	CommitID = gp->hash;
	ParentID = gp->parent;

	int b_idx =BranchListBox->Items->IndexOf(gp->branch);
	if (b_idx!=-1) BranchListBox->ItemIndex = b_idx;

	if (gp->is_index) DiffOpt = "--cached";

	std::unique_ptr<TStringList> o_lst(new TStringList());
	UnicodeString prm =
		(gp->is_work || gp->is_index)? 
				"diff --stat-width=120 " + DiffOpt :
				"diff --stat-width=120 " + gp->hash + "^ " + gp->hash;

	GitBusy = true;
	if (GitShellExe(prm, WorkDir, o_lst.get()) && o_lst->Count>0) {
		TListBox *lp = DiffListBox;
		int max_fwd = 0;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			if (lbuf.Pos(" | "))
				max_fwd = std::max(max_fwd, lp->Canvas->TextWidth(get_tkn(lbuf, " | ") + " "));
		}
		MaxDfWidth = max_fwd;
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
	else if (StartsText("IncSearch", cmd_F))		FindCommitEdit->SetFocus();
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))			BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))			DiffListBox->SetFocus();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	else if (USAME_TI(get_CmdStr(cmd_F), "Pack"))	ArchiveAction->Execute();
	else return;

	Key = 0;
}

//---------------------------------------------------------------------------
//差分概略の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)?
						((!State.Contains(odFocused) || GitBusy)? col_oppItem : col_selItem) : lp->Color;
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;
	bool use_fgsel = is_SelFgCol(State);

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (lbuf.Pos(" | ")) {
		//ファイル名
		UnicodeString s = split_tkn(lbuf, " | ");
		if (s.Pos(" => ")) {
			UnicodeString s1 = split_tkn(s, " => ");
			out_TextEx(cv, xp, yp, s1,
				use_fgsel? col_fgSelItem :
				AppFextColorAction->Checked? get_ExtColor(get_extension(s1), col_fgTxtPrv) : col_fgTxtPrv);
			out_TextEx(cv, xp, yp, " => ", use_fgsel? col_fgSelItem : col_fgTxtPrv);
		}
		out_TextEx(cv, xp, yp, s,
			use_fgsel? col_fgSelItem :
			AppFextColorAction->Checked? get_ExtColor(get_extension(Trim(s)), col_fgTxtPrv) : col_fgTxtPrv);

		//罫線
		TRect rc = Rect; rc.Left = xp = MaxDfWidth;
		RuledLnTextOut("│", cv, rc, col_fgTxtPrv);
		xp = rc.Left;
		//統計
		s = split_tkn_spc(lbuf) + " ";
		if (StartsText("Bin", Trim(s))) {
			out_TextEx(cv, xp, yp, s,	 use_fgsel? col_fgSelItem : AdjustColor(col_fgTxtPrv, 72));
			lbuf = ReplaceStr(lbuf, "->", "→");
			out_TextEx(cv, xp, yp, lbuf, use_fgsel? col_fgSelItem : col_fgTxtPrv);
		}
		else {
			//変更数
			UnicodeString tmp = ReplaceStr(s, " ", "0");
			xp += (cv->TextWidth(tmp) - cv->TextWidth(s));
			out_TextEx(cv, xp, yp, s, use_fgsel? col_fgSelItem :col_fgTxtPrv);
			//グラフ
			int g_w = rc.Height() / 3;
			TRect g_rc = rc;
			g_rc.Left = xp; g_rc.SetWidth(g_w); g_rc.SetHeight(g_w); g_rc.Offset(0, g_w);
			for (int i=1; i<=lbuf.Length(); i++) {
				WideChar c = lbuf[i];
				cv->Brush->Color = (c=='-')? clRed : (c=='+')? clGreen : col_None;
				if (cv->Brush->Color!=col_None) {
					cv->FillRect(g_rc);
					g_rc.Offset(g_w, 0);
				}
			}
		}
	}
	else {
		for (int i=1; i<=lbuf.Length(); i++) {
			WideChar c = lbuf[i];
			out_TextEx(cv, xp, yp, c,
				use_fgsel? col_fgSelItem : (c=='-')? clRed : (c=='+')? clGreen : col_fgTxtPrv);
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
	else if (USAME_TI(cmd_F, "ReturnList")) ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))	BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))	CommitListBox->SetFocus();
	else if (USAME_TI(cmd_F, "FileEdit"))	EditFileAction->Execute();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))
											show_PopupMenu(lp);
	else return;

	Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::GitListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	Key = 0;
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
	reduction_MenuLine(((TPopupMenu*)Sender)->Items);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ResetItemClick(TObject *Sender)
{
	int tag = ((TMenuItem *)Sender)->Tag;
	UnicodeString prm = (tag==2)? "Hard" : (tag==0)? "Soft" : "Mixed";
	UnicodeString msg;
	msg.sprintf(_T("[%s]を直前のコミットに戻しますか(%s)?"), RefHEAD.c_str(), prm.c_str());
	if (msgbox_Sure(msg)) GitExeStr("reset --" + prm.LowerCase() + " HEAD^");
}

//---------------------------------------------------------------------------
//アーカイブ作成
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->hash.IsEmpty()) {
		UserModule->PrepareSaveDlg(_T("アーカイブの作成"), _T("アーカイブ (*.zip)|*.zip"), _T("archive.zip"), WorkDir);
		if (UserModule->SaveDlg->Execute()) {
			UnicodeString arc_name = UserModule->SaveDlg->FileName;
			UnicodeString prm;
			prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), gp->hash.c_str(), yen_to_slash(arc_name).c_str());
			GitExeStr(prm);
		}
	}
}
//---------------------------------------------------------------------------
//一時アーカイブとして開く
//---------------------------------------------------------------------------
void __fastcall TGitViewer::OpenTmpArcActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (gp && !gp->hash.IsEmpty()) {
		UnicodeString tmp_name = TempPathA + gp->hash.SubString(1, 8) + "_"
									+ ExtractFileName(ExcludeTrailingPathDelimiter(WorkDir)) + ".zip";
		UnicodeString prm;
		prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), gp->hash.c_str(), yen_to_slash(tmp_name).c_str());
		if (!SameText(GitExeStr(prm), "ERROR") && file_exists(tmp_name)) {
			RetArcFile	= tmp_name;
			ModalResult = mrOk;
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
//Diffツールを起動
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffToolActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		UnicodeString fnam2;
		if (fnam.Pos(" => ")) {
			fnam2 = get_tkn_r(fnam, " => ");
			fnam =  get_tkn(fnam, " => ");
		}
		if (fnam2.IsEmpty()) fnam2 = fnam;

		UnicodeString prm;
		if (!CommitID.IsEmpty() && !ParentID.IsEmpty()) {
			prm.sprintf(_T("difftool -y %s:%s %s:%s"),
				get_tkn(ParentID, " ").c_str(), fnam.c_str(), CommitID.c_str(), fnam2.c_str());
		}
		else {
			prm.sprintf(_T("difftool -y %s %s"), DiffOpt.c_str(), fnam.c_str());
		}
		GitExeStr(prm);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty();
}
//---------------------------------------------------------------------------
//親コミットをblame
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BlameActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		if (fnam.Pos(" => ")) fnam =  get_tkn_r(fnam, " => ");
		UnicodeString prm;
		prm.sprintf(_T("gui blame %s %s"), get_tkn(ParentID, " ").c_str(), fnam.c_str());
		GitExeStr(prm);
	}
}

//---------------------------------------------------------------------------
//他のブランチも表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowBranchesActionExecute(TObject *Sender)
{
	ShowBranchesAction->Checked = !ShowBranchesAction->Checked;
	UpdateCommitList();
}
//---------------------------------------------------------------------------
//リモート参照を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowRemoteActionExecute(TObject *Sender)
{
	ShowRemoteAction->Checked = !ShowRemoteAction->Checked;
	UpdateCommitList();
}
//---------------------------------------------------------------------------
//Author の名前を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowAuthorActionExecute(TObject *Sender)
{
	ShowAuthorAction->Checked = !ShowAuthorAction->Checked;
	UpdateCommitList();
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
void __fastcall TGitViewer::GitPopupMenuPopup(TObject *Sender)
{
	reduction_MenuLine(((TPopupMenu*)Sender)->Items);
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(contained_wd_i(KeysStr_CsrUp, KeyStr))   KeyStr = "UP";
	else if (contained_wd_i(KeysStr_CsrDown, KeyStr)) KeyStr = "DOWN";

	if		(equal_UP(KeyStr))	 { FindUpAction->Execute();		Key = 0; }
	else if (equal_DOWN(KeyStr)) { FindDownAction->Execute();	Key = 0; }
	else if (contained_wd_i(KeysStr_ToList, KeyStr) || equal_ENTER(KeyStr)) CommitListBox->SetFocus();
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditChange(TObject *Sender)
{
	TListBox *lp = CommitListBox;
	UnicodeString kwd = FindCommitEdit->Text;
	if (!kwd.IsEmpty()) {
		int idx = -1;
		for (int i=0; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
		SetCommitListIndex(idx);

		if (CommitScrPanel->KeyWordChanged(kwd, lp->Count)) {
			for (int i=0; i<lp->Count; i++) {
				if (ContainsText(lp->Items->Strings[i], kwd)) CommitScrPanel->AddHitLine(i);
			}
			CommitScrPanel->Repaint();
		}
	}
	else {
		lp->ItemIndex = -1;
		CommitScrPanel->KeyWordChanged(EmptyStr, 0);
	}

	lp->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditEnter(TObject *Sender)
{
	FindCommitEdit->Color = scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FindCommitEdit);
}

//---------------------------------------------------------------------------
//コミット検索
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindUpActionExecute(TObject *Sender)
{
	TListBox *lp = CommitListBox;
	UnicodeString kwd = FindCommitEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && idx==-1; i--)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
	}
	SetCommitListIndex(idx);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindUpActionUpdate(TObject *Sender)
{
	TListBox *lp = CommitListBox;
	UnicodeString kwd = FindCommitEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex>0) {
		for (int i=lp->ItemIndex-1; i>=0 && !found; i--)
			found = ContainsText(lp->Items->Strings[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindDownActionExecute(TObject *Sender)
{
	TListBox *lp = CommitListBox;
	UnicodeString kwd = FindCommitEdit->Text;
	int idx = -1;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && idx==-1; i++)
			if (ContainsText(lp->Items->Strings[i], kwd)) idx = i;
	}
	SetCommitListIndex(idx);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindDownActionUpdate(TObject *Sender)
{
	TListBox *lp = CommitListBox;
	UnicodeString kwd = FindCommitEdit->Text;
	bool found = false;
	if (!kwd.IsEmpty() && lp->ItemIndex<lp->Count-1) {
		for (int i=lp->ItemIndex+1; i<lp->Count && !found; i++)
			found = ContainsText(lp->Items->Strings[i], kwd);
	}
	((TAction*)Sender)->Enabled = found;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindBtnClick(TObject *Sender)
{
	FindCommitEdit->SetFocus();
}

//---------------------------------------------------------------------------
//更新
//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateBtnClick(TObject *Sender)
{
	UpdateCommitList();
}

//---------------------------------------------------------------------------
//git-gui の起動
//---------------------------------------------------------------------------
void __fastcall TGitViewer::GuiActionExecute(TObject *Sender)
{
	UnicodeString prm = add_quot_if_spc(GitGuiExe);
	if (is_quot(prm)) prm.Insert("\"\" ", 1);
	if (!Execute_ex("cmd", "/c start " + prm, WorkDir, "H")) msgbox_ERR(USTR_FaildExec);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::GuiActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !GitGuiExe.IsEmpty();
}

//---------------------------------------------------------------------------
//Console 起動
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ConsoleActionExecute(TObject *Sender)
{
	UnicodeString prm = add_quot_if_spc(GitBashExe);
	if (is_quot(prm)) prm.Insert("\"\" ", 1);
	if (!Execute_ex("cmd", "/c start " + prm, WorkDir, "H")) msgbox_ERR(USTR_FaildExec);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ConsoleActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !GitBashExe.IsEmpty();
}

//---------------------------------------------------------------------------
//テキストエディタで開く
//---------------------------------------------------------------------------
void __fastcall TGitViewer::EditFileActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		if (fnam.Pos(" => ")) fnam =  get_tkn_r(fnam, " => ");
		fnam = CommitID.IsEmpty()? IncludeTrailingPathDelimiter(WorkDir) + slash_to_yen(fnam)
								 : SaveRevAsTemp(CommitID, fnam);
		if (!fnam.IsEmpty() && !open_by_TextEditor(fnam)) msgbox_ERR(GlobalErrMsg);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::EditFileActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Enabled = !GetDiffFileName().IsEmpty();

	UnicodeString s = "テキストエディタで開く";
	s += (CommitID.IsEmpty()? "(&E)" : " - Temp (&E)");
	ap->Caption = s;
}

//---------------------------------------------------------------------------
//GUIツールの選択
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SelGuiItemClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("GUIツールの選択"), F_FILTER_EXE2,
		ExtractFileName(GitGuiExe).c_str(), ExtractFileDir(GitGuiExe));
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) GitGuiExe = fnam;
}
//---------------------------------------------------------------------------
//Console の選択
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SelConsoleItemClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("Console の選択"), F_FILTER_EXE2,
		ExtractFileName(GitBashExe).c_str(), ExtractFileDir(GitBashExe));
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam)) GitBashExe = fnam;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "F5")) UpdateCommitList();
	else SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_GIT) _T("#GitViewer"));
}
//---------------------------------------------------------------------------

