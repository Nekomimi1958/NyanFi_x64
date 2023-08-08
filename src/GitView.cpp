//----------------------------------------------------------------------//
// NyanFi																//
//  Gitビュアー															//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "FileInfDlg.h"
#include "GitTag.h"
#include "GenInfDlg.h"
#include "InpExDlg.h"
#include "GitView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGitViewer *GitViewer = NULL;

//---------------------------------------------------------------------------
__fastcall TGitViewer::TGitViewer(TComponent* Owner)
	: TForm(Owner)
{
	HistoryLimit  = GIT_DEF_HISTLIMIT;
	LastBrListIdx = LastCmListIdx = LastDfListIdx = -1;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormCreate(TObject *Sender)
{
	BranchScrPanel = new UsrScrollPanel(BranchPanel, BranchListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	CommitScrPanel = new UsrScrollPanel(CommitPanel, CommitListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	DiffScrPanel   = new UsrScrollPanel(DiffPanel,   DiffListBox,   USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	MsgHint = new UsrHintWindow(this);
	MsgHint->Font->Assign(HintFont);

	WarnList   = new TStringList();
	StatusList = new TStringList();

	Staged     = false;
	KeyHandled = false;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormShow(TObject *Sender)
{
	Caption = yen_to_delimiter(WorkDir) + " - Gitビュアー";

	IniFile->LoadPosInfo(this);

	setup_ToolBar(FindBar);
	setup_ToolBar(DiffBar);

	set_ListBoxItemHi(BranchListBox, ListFont);
	set_UsrScrPanel(BranchScrPanel);
	set_ListBoxItemHi(CommitListBox, ListFont);
	set_UsrScrPanel(CommitScrPanel);
	set_ListBoxItemHi(DiffListBox,	 ListFont);
	set_UsrScrPanel(DiffScrPanel);

	BranchPanel->Width = IniFile->ReadIntGen(_T("GitViewBranchWidth"),	200);
	RL_Panel->Height   = IniFile->ReadIntGen(_T("GitViewDiffHeight"),	120);

	FindCommitEdit->Font->Assign(DialogFont);
	FindCommitEdit->Width = IniFile->ReadIntGen(_T("GitViewFindWidth"),	200);

	ShowRBranchAction->Checked  = IniFile->ReadBoolGen(_T("GitViewShowRBranch"),true);
	ShowTagAction->Checked      = IniFile->ReadBoolGen(_T("GitViewShowTag"),	true);
	ShowBranchesAction->Checked = IniFile->ReadBoolGen(_T("GitViewShowBranches"));
	ShowRemoteAction->Checked   = IniFile->ReadBoolGen(_T("GitViewShowRemote"));
	ShowAuthorAction->Checked   = IniFile->ReadBoolGen(_T("GitViewShowAuthor"));

	SetDarkWinTheme(this);
	CommitSplitter->Color = col_Splitter;
	DiffSplitter->Color   = col_Splitter;
	BranchListBox->Color  = col_bgList;
	CommitListBox->Color  = col_bgList;
	DiffListBox->Color	  = col_bgList;
	FindCommitEdit->Color = get_WinColor(true);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::WmFormShowed(TMessage &msg)
{
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
	DiffListBox->Clear();
	ClearCommitList();

	FilterName  = EmptyStr;
	HistoryLimit = GIT_DEF_HISTLIMIT;

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("GitViewBranchWidth"),		BranchPanel->Width);
	IniFile->WriteIntGen(_T("GitViewDiffHeight"),		RL_Panel->Height);
	IniFile->WriteIntGen(_T("GitViewFindWidth"),		FindCommitEdit->Width);
	IniFile->WriteBoolGen(_T("GitViewShowRBranch"),		ShowRBranchAction);
	IniFile->WriteBoolGen(_T("GitViewShowTag"),			ShowTagAction);
	IniFile->WriteBoolGen(_T("GitViewShowBranches"),	ShowBranchesAction);
	IniFile->WriteBoolGen(_T("GitViewShowRemote"),		ShowRemoteAction);
	IniFile->WriteBoolGen(_T("GitViewShowAuthor"),		ShowAuthorAction);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormDestroy(TObject *Sender)
{
	delete BranchScrPanel;
	delete CommitScrPanel;
	delete DiffScrPanel;
	delete MsgHint;
	delete WarnList;
	delete StatusList;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "F5"))
		UpdateLogAction->Execute();
	else
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_GIT) _T("#GitViewer"));
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
//git実行結果文字列を取得
//  必要に応じて一覧を更新
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
		}
		else {
			ret_str = o_lst->Text;
			//状態を更新
			if (StartsText("reset HEAD", prm) || contained_wd_i(_T("add|rm"), cmd)) {
				GitBusy = true;
				UnicodeString stt_str = get_post_tab(UpdateStatusList());
				GitBusy = false;
				if (!stt_str.IsEmpty()) {
					UnicodeString stt_wk = get_pre_tab(stt_str);
					UnicodeString stt_ix = get_post_tab(stt_str);

					TListBox *c_lp = CommitListBox;
					int cnt = 0;
					for (int i=0; i<c_lp->Count && cnt<2; i++) {
						git_rec *gp = (git_rec *)c_lp->Items->Objects[i];
						if (gp->is_work) {
							gp->msg = get_pre_tab(stt_str);
							gp->diff_inf = EmptyStr;
							cnt++;
						}
						else if (gp->is_index) {
							gp->msg = get_post_tab(stt_str);
							gp->diff_inf = EmptyStr;
							cnt++;
						}
					}
					c_lp->Invalidate();

					UpdateDiffList(true);
				}
			}
			//コミット履歴を更新
			else if (!contained_wd_i(_T("archive|cat-file|config|difftool|gui|rev-parse|show"), cmd)) {
				UpdateCommitList();
			}
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//git実行結果を TStringDynArray で取得
//  エラー表示/ 一覧の更新は行わない/ 警告を分離取得
//---------------------------------------------------------------------------
TStringDynArray __fastcall TGitViewer::GitExeStrArray(UnicodeString prm)
{
	GitBusy  = true;
	std::unique_ptr<TStringList> o_lst(new TStringList());
	DWORD exit_code;
	bool res = GitShellExe(prm, WorkDir, o_lst.get(), &exit_code);
	GitBusy  = false;

	TStringDynArray ret_array;
	if (res && exit_code==0) {
		split_GitWarning(o_lst.get(), WarnList);
		for (int i=0; i<o_lst->Count; i++) add_dyn_array(ret_array, o_lst->Strings[i]);
	}
	return ret_array;
}

//---------------------------------------------------------------------------
//git実行結果をリストで取得 (ヒント表示可)
//---------------------------------------------------------------------------
bool __fastcall TGitViewer::GitExeList(UnicodeString prm, TStringList *o_lst, UnicodeString hint)
{
	if (!hint.IsEmpty()) ShowMsgHint(hint);

	GitBusy = true;
	DWORD exit_code;
	bool res = (GitShellExe(prm, WorkDir, o_lst, &exit_code) && exit_code==0);
	GitBusy = false;

	if (!hint.IsEmpty()) MsgHint->ReleaseHandle();
	return res;
}

//---------------------------------------------------------------------------
//指定リビジョンのファイルを一時ディレクトリに保存
//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::SaveRevAsTemp(UnicodeString id, UnicodeString fnam)
{
	try {
		//ファイルイメージの取得
		UnicodeString src_nam = id + ":" + fnam;
		UnicodeString prm;
		prm.sprintf(_T("cat-file -p %s"), src_nam.c_str());
		std::unique_ptr<TMemoryStream> o_ms(new TMemoryStream());
		ShowMsgHint("ファイル抽出中...");
		GitBusy  = true;
		DWORD exit_code;
		bool res = (GitShellExe(prm, WorkDir, o_ms.get(), &exit_code) && exit_code==0);
		GitBusy  = false;
		MsgHint->ReleaseHandle();
		if (!res) UserAbort(USTR_FaildProc);

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
git_rec * __fastcall TGitViewer::cre_GitRec(UnicodeString msg)
{
	git_rec *gp  = new git_rec;
	gp->is_head  = false;
	gp->is_work  = false;
	gp->is_index = false;
	gp->is_stash = false;
	gp->msg 	 = msg;
	return gp;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::ClearCommitList()
{
	TListBox *lp = CommitListBox;
	for (int i=0; i<lp->Count; i++) delete (git_rec*)lp->Items->Objects[i];
	lp->Clear();

	LastBrListIdx = LastCmListIdx = LastDfListIdx = -1;
	MaxGrWidth = lp->ItemHeight;
	MaxAnWidth = 0;

	WarnList->Clear();
	StatusList->Clear();
	Staged = false;
}

//---------------------------------------------------------------------------
//状態リストの更新
//  戻り値: 全体の状態 [TAB ] ワーキングツリー [TAB] インデックス
//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::UpdateStatusList()
{
	StatusList->Clear();
	Staged = false;

	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (get_GitStatusList(WorkDir, o_lst.get())>=0) {
		StatusList->Assign(o_lst.get());
		return get_GitStatusStr(o_lst.get(), &Staged);
	}

	return EmptyStr;
}

//---------------------------------------------------------------------------
//コミット一覧を更新
//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateCommitList(
	UnicodeString commit_id)	//対象コミットID	(default = EmptyStr : HEAD)
{
	ClearCommitList();
	CommitScrPanel->HitLines->Clear();

	TListBox *c_lp = CommitListBox;
	ShowMsgHint("取得中...", c_lp);
	GitBusy = true;

	std::unique_ptr<TStringList> c_lst(new TStringList());
	std::unique_ptr<TStringList> o_lst(new TStringList());

	//Stash
	UnicodeString prm = "stash list";
	if (GitShellExe(prm, WorkDir, o_lst.get(), NULL, WarnList) && o_lst->Count>0) {
		git_rec *gp;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			gp = cre_GitRec();
			gp->graph	 = "*";
			gp->is_stash = true;
			gp->stash	 = split_tkn(lbuf, ": ");
			gp->msg		 = lbuf;
			c_lst->AddObject(EmptyStr, (TObject *)gp);
		}
		//セパレータ
		gp = cre_GitRec();
		gp->graph = "-";
		c_lst->AddObject(EmptyStr, (TObject *)gp);
	}

	//ワーキングツリー/インデックス
	UnicodeString stt_str = get_post_tab(UpdateStatusList());
	if (!stt_str.IsEmpty()) {
		UnicodeString stt_wk = get_pre_tab(stt_str);
		UnicodeString stt_ix = get_post_tab(stt_str);

		//ワーキングツリー
		git_rec *gp  = cre_GitRec(stt_wk);
		gp->is_work  = true;
		gp->graph	 = "*";

		c_lst->AddObject(EmptyStr, (TObject *)gp);
		//インデックス
		gp  = cre_GitRec(stt_ix);
		gp->is_index = true;
		gp->graph	 = "*";
		c_lst->AddObject(EmptyStr, (TObject *)gp);
		//セパレータ
		gp  = cre_GitRec();
		gp->graph	= "-";
		c_lst->AddObject(EmptyStr, (TObject *)gp);
	}

	//コミット履歴
	prm = "log --graph";
	if (ShowBranchesAction->Checked && commit_id.IsEmpty()) prm += " --branches";
	if (HistoryLimit>0) prm.cat_sprintf(_T(" -%u"), HistoryLimit);
	if (!FilterName.IsEmpty()) prm += " --follow";
	prm += " --date=format:\"%Y/%m/%d %H:%M:%S\" --pretty=format:\"\t%H\t%P\t%ad\t%s\t%d\t%an\"";
	if (!commit_id.IsEmpty())  prm.cat_sprintf(_T(" %s"), commit_id.c_str());
	if (!FilterName.IsEmpty()) prm.cat_sprintf(_T(" \"%s\""), FilterName.c_str());

	//グラフ / コミットID / 親ID / 日時 / 件名 / ブランチ / タグ / Author名
	int idx_h = -1;
	UnicodeString last_parent;
	DWORD exit_code;
	o_lst->Clear();
	if (GitShellExe(prm, WorkDir, o_lst.get(), &exit_code, WarnList) && exit_code==0 && o_lst->Count>0) {
		int max_gr_len = 0, max_an_wd = 0;
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			TStringDynArray ibuf = split_strings_tab(lbuf);
			if (ibuf.Length>0) {
				git_rec *gp = cre_GitRec();
				gp->graph	= ibuf[0];
				max_gr_len	= std::max(max_gr_len, gp->graph.Length());

				UnicodeString br_str;
				if (ibuf.Length==7) {
					gp->hash   = ibuf[1];
					gp->parent = last_parent = ibuf[2];
					gp->msg    = ibuf[4];
					gp->author = ibuf[6];
					max_an_wd = std::max(max_an_wd, c_lp->Canvas->TextWidth(gp->author));

					br_str = get_in_paren(ibuf[5]);
					TStringDynArray b_buf = SplitString(br_str, ",");
					for (int j=0; j<b_buf.Length; j++) {
						UnicodeString ss = Trim(b_buf[j]);
						if (remove_top_text(ss, "tag: ")) {
							if (!gp->tags.IsEmpty()) gp->tags += "\t";
							gp->tags += ss;
						}
						else if (ss.Pos("/")) {
							if (!EndsStr("/HEAD", ss)) gp->branch_r = ss;
						}
						else {
							gp->is_head |= StartsStr("HEAD -> ", ss) || USAME_TS(ss, "HEAD");
							if (!gp->branch.IsEmpty()) gp->branch += ",";
							gp->branch += ss;
						}
					}

					try {
						gp->f_time = str_to_DateTime(ibuf[3]);
					}
					catch (...) {
						gp->f_time = 0;
					}
				}

				lbuf = gp->hash.SubString(1, 7);
				if (ShowAuthorAction->Checked) lbuf.sprintf(_T(" %s"), gp->author.c_str());
				lbuf.sprintf(_T(" %s %s"), br_str.c_str(), gp->msg.c_str());
				int idx = c_lst->AddObject(lbuf, (TObject *)gp);
				if (gp->is_head) idx_h = idx;
			}
		}

		MaxGrWidth = (max_gr_len + 1) * c_lp->ItemHeight / 3;
		MaxAnWidth = max_an_wd;

		//残りのコミット数
		if (!last_parent.IsEmpty()) {
			prm.sprintf(_T("rev-list --count %s"), last_parent.c_str());
			if (!FilterName.IsEmpty()) prm.cat_sprintf(_T(" -- %s"), add_quot_if_spc(FilterName).c_str());
			o_lst->Clear();
			if (GitShellExe(prm, WorkDir, o_lst.get(), NULL, WarnList) && o_lst->Count>0) {
				int cnt = o_lst->Strings[0].ToIntDef(0);
				if (cnt>0) {
					git_rec *gp = cre_GitRec(
						UnicodeString().sprintf(_T("……… これ以前に %u 個のコミットがあります ………"), cnt));
					gp->graph = "#";
					c_lst->AddObject(EmptyStr, (TObject *)gp);
				}
			}
		}
	}

	if (!CommitID.IsEmpty()) {
		for (int i=0; i<c_lst->Count; i++) {
			git_rec *gp = (git_rec *)c_lst->Objects[i];
			if (SameStr(gp->hash, CommitID)) {
				idx_h = i; break;
			}
		}
	}

	if (idx_h==-1) idx_h = 0;

	//リストボックスに割り当て
	c_lp->Items->Assign(c_lst.get());
	SetCommitListIndex(idx_h);
	CommitScrPanel->UpdateKnob();

	GitBusy = false;
	MsgHint->ReleaseHandle();

	UpdateBranchList();

	if (WarnList->Count>0) msgbox_WARN(WarnList->Text);
}

//---------------------------------------------------------------------------
//ブランチ一覧の更新
//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateBranchList()
{
	GitBusy = true;

	//ローカル
	TListBox *b_lp = BranchListBox;
	b_lp->Clear();
	int idx = -1;
	RefHEAD = EmptyStr;
	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (GitShellExe("branch", WorkDir, o_lst.get(), NULL, WarnList)) {
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->AddObject("ブランチ", (TObject *)(NativeInt)(GIT_FLAG_HDRLN|GIT_FLAG_LOCAL));
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			bool is_hd = remove_top_s(lbuf, '*');
			lst->AddObject(Trim(lbuf), (TObject *)(NativeInt)(GIT_FLAG_LOCAL | (is_hd? GIT_FLAG_HEAD : 0)));
			if (is_hd) {
				idx = lst->Count - 1;
				RefHEAD = lbuf;
			}
		}
		b_lp->Items->Assign(lst.get());
	}
	//リモート
	if (ShowRBranchAction->Checked) {
		o_lst->Clear();
		if (GitShellExe("branch -r", WorkDir, o_lst.get(), NULL, WarnList)) {
			std::unique_ptr<TStringList> lst(new TStringList());
			lst->AddObject(EmptyStr, (TObject *)(NativeInt)GIT_FLAG_BLANK);
			lst->AddObject("リモート", (TObject *)(NativeInt)(GIT_FLAG_HDRLN|GIT_FLAG_REMOTE));
			for (int i=0; i<o_lst->Count; i++) {
				UnicodeString lbuf = Trim(o_lst->Strings[i]);
				bool is_hd = ContainsStr(lbuf, "/HEAD -> ");
				if (is_hd) lbuf = get_tkn_r(lbuf, "/HEAD -> ");
				if (lst->IndexOf(lbuf)==-1)
					lst->AddObject(lbuf, (TObject *)(NativeInt)(GIT_FLAG_REMOTE | (is_hd? GIT_FLAG_HEAD : 0)));
			}
			b_lp->Items->AddStrings(lst.get());
		}
	}
	//タグ
	if (ShowTagAction->Checked) {
		o_lst->Clear();
		if (GitShellExe("tag", WorkDir, o_lst.get(), NULL, WarnList)) {
			//コミット一覧にあるタグのリストを作成
			std::unique_ptr<TStringList> tag_lst(new TStringList());
			for (int i=0; i<CommitListBox->Count; i++) {
				git_rec *gp = (git_rec *)CommitListBox->Items->Objects[i];
				if (!gp->tags.IsEmpty()) {
					TStringDynArray t_lst = split_strings_tab(gp->tags);
					for (int j=0; j<t_lst.Length; j++) {
					 	if (tag_lst->IndexOf(t_lst[j])==-1) tag_lst->Add(t_lst[j]);
					}
				}
			}
			//タグ項目を追加
			std::unique_ptr<TStringList> lst(new TStringList());
			lst->AddObject(EmptyStr, (TObject *)(NativeInt)GIT_FLAG_BLANK);
			lst->AddObject("タグ", (TObject *)(NativeInt)(GIT_FLAG_HDRLN|GIT_FLAG_TAG));
			for (int i=0; i<o_lst->Count; i++) {
				UnicodeString tag = o_lst->Strings[i];
				if (tag.IsEmpty()) continue;
				int flag = GIT_FLAG_TAG;
				if (tag_lst->IndexOf(tag)==-1) flag |= GIT_FLAG_INVAL;
				lst->AddObject(tag, (TObject *)(NativeInt)flag);
			}
			b_lp->Items->AddStrings(lst.get());
		}
	}

	b_lp->ItemIndex = LastBrListIdx = idx;
	BranchScrPanel->UpdateKnob();

	GitBusy = false;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateDiffList(
	bool keep_idx)	//ItemIndex を維持	(default = false);
{
	int org_idx = DiffListBox->ItemIndex;
	DiffListBox->Clear();
	CommitID = ParentID = BranchName = TagNames = StashName = EmptyStr;
	MaxDfWidth = 0;

	TListBox *c_lp = CommitListBox;
	git_rec *gp = (git_rec *)c_lp->Items->Objects[c_lp->ItemIndex];
	if (!gp->is_stash && !gp->is_work && !gp->is_index && gp->hash.IsEmpty()) return;

	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (!gp->stash.IsEmpty()) {
		StashName = gp->stash;
		UnicodeString prm = "stash show " + gp->stash;
		GitBusy = true;
		if (GitShellExe(prm, WorkDir, o_lst.get(), NULL, WarnList) && o_lst->Count>0)
			gp->diff_inf = o_lst->Text;
		GitBusy = false;
	}
	else {
		CommitID   = gp->hash;
		ParentID   = gp->parent;
		BranchName = gp->branch;
		TagNames   = gp->tags;

		if (gp->diff_inf.IsEmpty()) {
			UnicodeString prm = "diff --stat-width=120 ";
			UnicodeString parent = get_tkn(ParentID, ' ');
			if (parent.IsEmpty()) parent = GIT_NULL_ID;

			if		(gp->is_work)	;
			else if (gp->is_index)	prm += "--cached";
			else					prm += (parent + " " + CommitID);

			if (!FilterName.IsEmpty()) prm.cat_sprintf(_T(" -- \"%s\""), FilterName.c_str());
			GitBusy = true;
			if (GitShellExe(prm, WorkDir, o_lst.get(), NULL, WarnList) && o_lst->Count>0)
				gp->diff_inf = o_lst->Text;
			GitBusy = false;
		}
		else {
			o_lst->Text = gp->diff_inf;
		}
	}

	//作業ツリーに ? ファイルを追加
	if (gp->is_work && StatusList->Text.Pos('?')) {
		o_lst->Add(EmptyStr);
		o_lst->Add("-");
		o_lst->Add("*追跡されていないファイル");
		std::unique_ptr<TStringList> u_lst(new TStringList());
		for (int i=0; i<StatusList->Count; i++) {
			UnicodeString lbuf = StatusList->Strings[i];
			if (lbuf[2]=='?') o_lst->Add(lbuf);
		}
	}

	//リストボックスに割り当て
	TListBox *d_lp = DiffListBox;
	bool is_irreg = IsIrregularFont(d_lp->Canvas->Font);
	int max_fwd = 0;
	for (int i=0; i<o_lst->Count; i++) {
		UnicodeString lbuf = o_lst->Strings[i];
		if (lbuf.Pos(" | "))
			max_fwd = std::max(max_fwd, get_TextWidth(d_lp->Canvas, get_tkn(lbuf, " | ") + " ", is_irreg));
	}
	MaxDfWidth = max_fwd;
	d_lp->Items->Assign(o_lst.get());
	d_lp->ItemIndex = keep_idx? std::min(org_idx, d_lp->Count - 1) : 0;
	DiffScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//カーソル位置のブランチ名を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TGitViewer::GetCurBranchName(
	bool lc_only,	//ローカルのみ	(default = false)
	bool br_only,	//ブランチのみ	(default = false)
	bool av_only)	//有効のみ		(default = true)
{
	TListBox *lp = BranchListBox;
	int idx = lp->ItemIndex;
	if (!lp->Focused() || idx==-1) return EmptyStr;

	int flag = (int)lp->Items->Objects[idx];
	if (flag==0) return EmptyStr;
	if (br_only && (flag & GIT_FLAG_TAG))	 return EmptyStr;
	if (lc_only && (flag & GIT_FLAG_REMOTE)) return EmptyStr;
	if (av_only && (flag & GIT_FLAG_INVAL))	 return EmptyStr;
	
	UnicodeString bnam = lp->Items->Strings[idx];
	if (flag & GIT_FLAG_REMOTE) bnam = get_tkn_r(bnam, '/');
	return bnam;
}

//---------------------------------------------------------------------------
//ブランチ一覧の描画
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	int flag = (int)lp->Items->Objects[Index];

	TCanvas *cv = lp->Canvas;
	cv->Brush->Color = State.Contains(odSelected)? (State.Contains(odFocused)? col_selItem : col_oppItem) :
						  (flag & GIT_FLAG_HDRLN)? col_bgListHdr : lp->Color;
	cv->FillRect(Rect);
	if (flag & GIT_FLAG_BLANK) return;

	int xp = Rect.Left + SCALED_THIS(4);
	int yp = Rect.Top  + SCALED_THIS(2);
	UnicodeString lbuf = lp->Items->Strings[Index];
	//ヘッダ
	if (flag & GIT_FLAG_HDRLN) {
		out_TextEx(cv, xp, yp, lbuf, col_fgListHdr, col_bgListHdr);
	}
	//項目
	else {
		//HEADマーク
		if (flag & GIT_FLAG_HEAD) {
			cv->Font->Color = (flag & GIT_FLAG_REMOTE)? col_GitBraR : col_GitHEAD;
			cv->TextOut(xp, yp, HEAD_Mark);
		}
		xp += cv->TextWidth(HEAD_Mark);
		//ブランチ/タグ
		cv->Font->Color = is_SelFgCol(State)? col_fgSelItem : (flag & GIT_FLAG_INVAL)? col_InvItem : col_fgList;
		cv->TextOut(xp, yp, lbuf);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BranchListBoxClick(TObject *Sender)
{
	TListBox *b_lp = (TListBox*)Sender;
	int idx = b_lp->ItemIndex;	if (idx==-1) return;
	if (idx!=LastBrListIdx) {
		bool found = false;
		if (idx==0) idx = 1;
		bool is_dw = idx>LastBrListIdx;
		for (int i=idx; (is_dw? i<b_lp->Count : i>0) && !found; i+=(is_dw? 1 : -1)) {
			int flag = (int)b_lp->Items->Objects[i];
			if ((flag & (GIT_FLAG_HDRLN|GIT_FLAG_BLANK))==0) {
				idx = i;  found = true;
			}
		}
		if (!found) idx = LastBrListIdx;
		b_lp->ItemIndex = LastBrListIdx = idx;
	}

	TagNames = EmptyStr;

	//コミット履歴のカーソル位置に反映
	if (idx!=-1) {
		int flag = (int)b_lp->Items->Objects[b_lp->ItemIndex];
		UnicodeString lbuf	 = b_lp->Items->Strings[idx];
		UnicodeString b_name = (flag & GIT_FLAG_LOCAL)? lbuf : EmptyStr;
		if (flag & GIT_FLAG_TAG) TagNames = lbuf;

		TListBox *c_lp = CommitListBox;
		if (!b_name.IsEmpty()) {
			for (int i=0; i<c_lp->Count; i++) {
				git_rec *gp = (git_rec *)c_lp->Items->Objects[i];
				if (SameText(b_name, gp->branch)) {
					c_lp->ItemIndex = i;
					c_lp->Invalidate();
					break;
				}
			}
		}
		else if (!TagNames.IsEmpty()) {
			for (int i=0; i<c_lp->Count; i++) {
				git_rec *gp = (git_rec *)c_lp->Items->Objects[i];
				if (contained_wd_i(ReplaceStr(gp->tags, "\t", "|"), TagNames)) {
					c_lp->ItemIndex = i;
					c_lp->Invalidate();
					break;
				}
			}
		}
	}
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
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
												BranchListBoxClick(BranchListBox);
	else if (is_ToRightOpe(KeyStr, cmd_F))		CommitListBox->SetFocus();
	else if (USAME_TI(cmd_F, "ReturnList"))		ModalResult = mrCancel;
	else if (USAME_TI(cmd_F, "RenameDlg")) 		RenBranchAction->Execute();
	else if (equal_ENTER(KeyStr)) 				ChckoutAction->Execute();
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
	cv->Brush->Color = is_match? col_matchItem : lp->Color;
	cv->FillRect(Rect);

	int max_h_wd = 0;
	bool is_irreg = IsIrregularFont(lp->Font);
	for (int i=0; i<=0x0f; i++) {
		max_h_wd = std::max(max_h_wd, get_TextWidth(cv, UnicodeString().sprintf(_T("%x"), i), is_irreg));
	}

	int s_4 = SCALED_THIS(4);
	int s_8 = SCALED_THIS(8);

	git_rec *gp = (git_rec *)lp->Items->Objects[Index];
	int xp = Rect.Left + s_4;
	int yp = Rect.Top  + SCALED_THIS(2);

	if (USAME_TS(gp->graph, "#")) {
		cv->Font->Color = col_LineNo;
		out_TextEx(cv, xp, yp, gp->msg);
	}
	if (USAME_TS(gp->graph, "...")) {
		cv->Font->Color = col_LineNo;
		out_TextEx(cv, xp, yp, "……");
	}
	//セパレータ
	else if (USAME_TS(gp->graph, "-")) {
		draw_Separator(cv, Rect);
	}
	//グラフ
	else if (!gp->graph.IsEmpty()) {
		git_rec *gp1 = (Index>0)? (git_rec *)lp->Items->Objects[Index - 1] : NULL;
		git_rec *gp2 = (Index<(lp->Count - 1))? (git_rec *)lp->Items->Objects[Index + 1] : NULL;
		UnicodeString s1 = (!gp->is_stash && gp1 && !USAME_TS(gp1->graph, "-"))? gp1->graph : EmptyStr;
		UnicodeString s2 = (!gp->is_stash && !gp->is_index && gp2)? gp2->graph : EmptyStr;
		TRect rc = Rect; rc.Left = xp;
		draw_GitGraph(gp->graph, s1, s2, cv, rc, gp->is_head, (gp->is_stash || gp->is_work || gp->is_index));
	}

	xp += MaxGrWidth;
	if (gp->is_stash) {
		out_TextEx(cv, xp, yp, gp->stash, col_Folder, col_None, s_8, is_irreg);
		UnicodeString s = gp->msg;
		out_TextEx(cv, xp, yp, split_tkn(s, ": "), col_GitBra, col_None, s_8, is_irreg);
		if (is_match_regex(s, _T("^[0-9a-f]{7}\\s")))
			out_TextEx(cv, xp, yp, split_tkn(s, ' '), col_GitHash, col_None, s_8);
		out_TextEx(cv, xp, yp, s, col_fgList, col_None);
	}
	if (gp->is_work || gp->is_index) {
		UnicodeString s1 = "作業ツリー ";
		UnicodeString s2 = "インデックス ";
		UnicodeString s  = gp->is_work? s1 : s2;
		xp += (std::max(cv->TextWidth(s1), cv->TextWidth(s2)) - cv->TextWidth(s));
		out_TextEx(cv, xp, yp, s, col_Folder, col_None, 0, is_irreg);
		out_TextEx(cv, xp, yp, gp->msg, col_fgList);
	}
	else if (!gp->hash.IsEmpty()) {
		//ハッシュ
		cv->Font->Color = col_GitHash;
		for (int i=1; i<8; i++) {
			UnicodeString c = gp->hash.SubString(i, 1);
			cv->TextOut(xp + (max_h_wd - cv->TextWidth(c))/2, yp, c);
			xp += max_h_wd;
		}
		xp += max_h_wd;

		//Author
		if (ShowAuthorAction->Checked) {
			cv->Font->Color = col_fgList;
			cv->TextOut(xp, yp, gp->author);
			xp += (MaxAnWidth + s_8);
		}

		//日時
		out_TextEx(cv, xp, yp, get_TimeStampStr(gp->f_time), get_TimeColor(gp->f_time, col_fgList), 
			col_None, max_h_wd, is_irreg);

		//タグ
		if (!gp->tags.IsEmpty()) {
			TStringDynArray t_lst = split_strings_tab(gp->tags);
			for (int i=0; i<t_lst.Length; i++) draw_GitTag(cv, xp, yp, t_lst[i], s_4);
			xp += s_4;
		}

		//ブランチ名
		if (!gp->branch.IsEmpty()) {
			TStringDynArray b_buf = SplitString(gp->branch, ",");
			for (int i=0; i<b_buf.Length; i++) {
				UnicodeString ss = b_buf[i];
				if (remove_top_s(ss, "HEAD -> ") || USAME_TS(ss, "HEAD"))
					out_TextEx(cv, xp, yp, HEAD_Mark, col_GitHEAD, col_None, 0, is_irreg);
				out_TextEx(cv, xp, yp, ss, col_bgList, col_GitBra, s_4, is_irreg);
				xp += s_4;
			}
		}
		if (ShowRemoteAction->Checked && !gp->branch_r.IsEmpty()) {
			out_TextEx(cv, xp, yp, gp->branch_r, col_bgList, col_GitBraR, s_8, is_irreg);
		}

		//メッセージ
		out_TextEx(cv, xp, yp, gp->msg, col_fgList);
	}

	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxClick(TObject *Sender)
{
	//コミットではない項目をスキップ
	TListBox *c_lp = CommitListBox;
	if (c_lp->ItemIndex==-1) return;
	if (c_lp->ItemIndex!=LastCmListIdx) {
		int idx = c_lp->ItemIndex;
		bool found = false;
		bool is_dw = idx>LastCmListIdx;
		for (int i=idx; (is_dw? i<c_lp->Count : i>=0) && !found; i+=(is_dw? 1 : -1)) {
			git_rec *gp = (git_rec *)c_lp->Items->Objects[i];
			if (gp->is_stash || gp->is_work || gp->is_index || !gp->hash.IsEmpty()) {
				idx = i;  found = true;
			}
		}
		if (!found) idx = LastCmListIdx;
		c_lp->ItemIndex = LastCmListIdx = idx;
	}

	UpdateDiffList();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
												CommitListBoxClick(NULL);
	else if (is_ToRightOpe(KeyStr, cmd_F))		DiffListBox->SetFocus();
	else if (is_ToLeftOpe(KeyStr, cmd_F))		BranchListBox->SetFocus();
	else if (USAME_TI(cmd_F, "ReturnList"))		ModalResult = mrCancel;
	else if (USAME_TI(cmd_F, "IncSearch"))		FindCommitEdit->SetFocus();
	else if (USAME_TI(cmd_F, "ShowFileInfo") || equal_ENTER(KeyStr))
												CommitInfoAction->Execute();
	else if (USAME_TI(get_CmdStr(cmd_F), "Pack"))
												ArchiveAction->Execute();
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
	cv->Brush->Color = lp->Color;
	cv->FillRect(Rect);

	int xp = Rect.Left + SCALED_THIS(4);
	int yp = Rect.Top  + SCALED_THIS(2);
	bool is_irreg = IsIrregularFont(cv->Font);

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (lbuf.Pos(" | ")) {
		UnicodeString s = split_tkn(lbuf, " | ");

		//ディレクトリ名
		int p = pos_r('/', s);
		if (p>1) {
			UnicodeString dnam = s.SubString(1, p);
			s.Delete(1, p);
			if (dnam.Pos(" => ")) {
				UnicodeString s1 = split_tkn(dnam, " => ");
				out_TextEx(cv, xp, yp, s1, col_Folder, col_None, 0, is_irreg);
				out_TextEx(cv, xp, yp, " → ", col_fgList, col_None, 0, is_irreg);
			}
			out_TextEx(cv, xp, yp, dnam, col_Folder, col_None, 0, is_irreg);
		}

		//ファイル名
		if (s.Pos(" => ")) {
			UnicodeString s1 = split_tkn(s, " => ");
			out_TextEx(cv, xp, yp, s1, get_ExtColor(get_extension(s1), col_fgList), col_None, 0, is_irreg);
			out_TextEx(cv, xp, yp, " → ", col_fgList);
		}
		out_TextEx(cv, xp, yp, s, get_ExtColor(get_extension(Trim(s)), col_fgList));

		//罫線
		TRect rc = Rect; rc.Left = xp = MaxDfWidth;
		RuledLnTextOut("│", cv, rc, col_fgList);
		xp = rc.Left;
		//統計
		s = split_tkn_spc(lbuf) + " ";
		if (StartsText("Bin", Trim(s))) {
			out_TextEx(cv, xp, yp, s, AdjustColor(col_fgList, ADJCOL_FGLIST));
			lbuf = ReplaceStr(lbuf, "->", "→");
			out_TextEx(cv, xp, yp, lbuf, col_fgList);
		}
		else {
			//変更数
			UnicodeString tmp = ReplaceStr(s, " ", "0");
			xp += (cv->TextWidth(tmp) - cv->TextWidth(s));
			out_TextEx(cv, xp, yp, s, col_fgList);
			//グラフ
			int g_w = rc.Height() / 3;
			TRect g_rc = rc;
			g_rc.Left = xp; g_rc.SetWidth(g_w); g_rc.SetHeight(g_w); g_rc.Offset(0, g_w);
			for (int i=1; i<=lbuf.Length(); i++) {
				WideChar c = lbuf[i];
				cv->Brush->Color = (c=='-')? col_GitDel : (c=='+')? col_GitIns : col_None;
				if (cv->Brush->Color!=col_None) {
					cv->FillRect(g_rc);
					g_rc.Offset(g_w, 0);
				}
			}
		}
	}
	//セパレータ
	else if (SameStr(lbuf, "-")) {
		draw_Separator(cv, Rect);
	}
	//追跡されていない～
	else if (lbuf.Pos('*')) {
		out_TextEx(cv, xp, yp, ReplaceStr(lbuf, "*", "?"), col_Headline);
	}
	//? ファイル
	else if (SameStr(lbuf.SubString(2, 1), '?')) {
		lbuf.Delete(1, 2);
		out_TextEx(cv, xp, yp, lbuf, get_ExtColor(get_extension(Trim(lbuf)), col_fgList));
	}
	//計
	else {
		TStringDynArray t_buf = get_csv_array(lbuf, 3);
		if (t_buf.Length==3) {
			for (int i=0; i<3; i++) {
				UnicodeString s = t_buf[i];
				if (i>0) out_TextEx(cv, xp, yp, ",", col_fgList);
				out_TextEx(cv, xp, yp, s,
					remove_end_text(s, "(-)")? col_GitDel : remove_end_text(s, "(+)")? col_GitIns : col_fgList,
					col_None, 0, is_irreg);
			}
			draw_separateLine(cv, Rect);
		}
	}

	cv->Brush->Color = lp->Color;
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxEnter(TObject *Sender)
{
	DiffListBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxClick(TObject *Sender)
{
	TListBox *d_lp = DiffListBox;
	if (d_lp->ItemIndex==-1) return;
	if (d_lp->ItemIndex!=LastDfListIdx) {
		int idx = d_lp->ItemIndex;
		bool found = false;
		bool is_dw = idx>LastDfListIdx;
		for (int i=idx; (is_dw? i<d_lp->Count : i>=0) && !found; i+=(is_dw? 1 : -1)) {
			UnicodeString lbuf = d_lp->Items->Strings[i];
			if (lbuf.Pos(" | ") || lbuf.Pos("? ")) {
				idx = i;  found = true;
			}
		}
		if (!found) idx = LastDfListIdx;
		d_lp->ItemIndex = LastDfListIdx = idx;
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
												DiffListBoxClick(NULL);
	else if (USAME_TI(cmd_F, "ReturnList")) 	ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))		BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))		CommitListBox->SetFocus();
	else if (USAME_TI(cmd_F, "FileEdit"))		EditFileAction->Execute();
	else if (USAME_TI(cmd_F, "TextViewer"))		ViewFileAction->Execute();
	else if (USAME_TI(cmd_F, "ShowFileInfo") || equal_ENTER(KeyStr))
												DiffDetailAction->Execute();
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
void __fastcall TGitViewer::CheckoutActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName();
	if (!bnam.IsEmpty()) {
		CommitID = EmptyStr;
		GitExeStr("checkout " + bnam);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CheckoutActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	int flag = GetCurBranchFlag();
	ap->Visible = ((flag & GIT_FLAG_HEAD)==0 || (flag & GIT_FLAG_LOCAL)==0);
	ap->Enabled = ap->Visible && ((flag & GIT_FLAG_INVAL)==0);
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
void __fastcall TGitViewer::CreBranchActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = CurIsHEAD();
}

//---------------------------------------------------------------------------
//ブランチの削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelBranchActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName(true, true);
	if (!bnam.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("ブランチ[%s]を削除してもよいですか?"), bnam.c_str());
		Screen->MessageFont->Assign(Application->DefaultFont);
		TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation,
							TMsgDlgButtons() << mbYes << mbNo << mbCancel, mbNo);
		TCheckBox *cp = new TCheckBox(MsgDlg);
		MsgDlg->ClientHeight = MsgDlg->ClientHeight + cp->Height + 12;
		cp->Caption = "強制的に削除(&D)";
		cp->Parent	= MsgDlg;
		cp->Left	= 20;
		cp->Top		= MsgDlg->ClientHeight - cp->Height - 12;
		cp->Width	= MsgDlg->ClientWidth - cp->Left;
		SetDarkWinTheme(MsgDlg);

		if (MsgDlg->ShowModal()==mrYes) {
			UnicodeString prm;
			prm.sprintf(_T("branch -%s %s"), (cp->Checked? _T("D") : _T("d")), bnam.c_str());
			GitExeStr(prm);
		}

		delete MsgDlg;
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelBranchActionUpdate(TObject *Sender)
{
	int flag = GetCurBranchFlag();
	((TAction *)Sender)->Visible = !(flag & GIT_FLAG_HEAD) && (flag & GIT_FLAG_LOCAL);
}

//---------------------------------------------------------------------------
//ブランチの名前を変更
//---------------------------------------------------------------------------
void __fastcall TGitViewer::RenBranchActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName(true, true, false);
	if (!bnam.IsEmpty()) {
		UnicodeString new_nam = InputBox("ブランチの名前を変更", "ブランチ名", bnam);
		if (!new_nam.IsEmpty()) {
			UnicodeString prm;
			prm.sprintf(_T("branch -m %s %s"), bnam.c_str(), new_nam.c_str());
			GitExeStr(prm);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::RenBranchActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !GetCurBranchName(true, true, false).IsEmpty();
}

//---------------------------------------------------------------------------
//タグを付ける
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SetTagActionExecute(TObject *Sender)
{
	if (!SetGitTagDlg) SetGitTagDlg = new TSetGitTagDlg(this);	//初回に動的作成
	SetGitTagDlg->CommitID	  = BranchListBox->Focused()? GetCurBranchName(true, true) : CommitID;
	SetGitTagDlg->EditMsgFile = IncludeTrailingPathDelimiter(WorkDir) + ".git\\TAG_EDITMSG";
	if (SetGitTagDlg->ShowModal()==mrOk) GitExeStr(SetGitTagDlg->GitParam);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SetTagActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible =
		BranchListBox->Focused()? !GetCurBranchName(true, true).IsEmpty() :
		CommitListBox->Focused()? !CommitID.IsEmpty() : false;
}

//---------------------------------------------------------------------------
//タグの削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelTagActionExecute(TObject *Sender)
{
	UnicodeString tag = get_pre_tab(TagNames);
	if (!tag.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("タグ [%s] を削除してもよいですか?"), tag.c_str());
		if (msgbox_Sure(msg)) GitExeStr("tag -d " + tag);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelTagActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !get_pre_tab(TagNames).IsEmpty();
}

//---------------------------------------------------------------------------
//マージ
//---------------------------------------------------------------------------
void __fastcall TGitViewer::MergeActionExecute(TObject *Sender)
{
	UnicodeString bnam = GetCurBranchName(true, true);
	if (!CurIsHEAD() && !bnam.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("[%s] を [%s] にマージしますか?"), bnam.c_str(), RefHEAD.c_str());
		if (msgbox_Sure(msg)) GitExeStr("merge " + bnam);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::MergeActionUpdate(TObject *Sender)
{
	int flag = GetCurBranchFlag();
	((TAction *)Sender)->Visible = !(flag & GIT_FLAG_HEAD) && (flag & GIT_FLAG_LOCAL);
}
//---------------------------------------------------------------------------
//チェリーピック
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CherryPickActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	if (!SameText(gp->branch, RefHEAD)) {
		if (msgbox_Sure("このコミットをチェリーピックしますか?")) GitExeStr("cherry-pick " + gp->hash);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CherryPickActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CommitID.IsEmpty();
}

//---------------------------------------------------------------------------
//ブランチ名をコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyBranchNameActionExecute(TObject *Sender)
{
	TStringDynArray b_buf = SplitString(BranchName, ",");
	UnicodeString bnam;
	for (int i=0; i<b_buf.Length; i++) {
		UnicodeString ss = b_buf[i];
		remove_top_s(ss, "HEAD -> ");
		remove_top_s(ss, "HEAD");
		if (!ss.IsEmpty()) {
			if (!bnam.IsEmpty()) bnam += ",";
			bnam += ss;
		}
	}
	copy_to_Clipboard(bnam);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyBranchNameActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !BranchName.IsEmpty();
}
//---------------------------------------------------------------------------
//コミットIDをコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyCommitIDActionExecute(TObject *Sender)
{
	copy_to_Clipboard(CommitID);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyCommitIDActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CommitID.IsEmpty();
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
	msg.sprintf(_T("[%s] を直前のコミットに戻しますか(%s)?"), RefHEAD.c_str(), prm.c_str());
	if (msgbox_Sure(msg)) GitExeStr("reset --" + prm.LowerCase() + " HEAD^");
}

//---------------------------------------------------------------------------
//アーカイブ作成
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionExecute(TObject *Sender)
{
	if (!CommitID.IsEmpty()) {
		UserModule->PrepareSaveDlg(_T("アーカイブの作成"), _T("アーカイブ (*.zip)|*.zip"), _T("archive.zip"), WorkDir);
		UnicodeString arc_name = UserModule->SaveDlgExecute();
		if (!arc_name.IsEmpty()) {
			UnicodeString prm;
			prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), CommitID.c_str(), yen_to_slash(arc_name).c_str());
			GitExeList(prm, NULL, "アーカイブ作成中...");
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CommitID.IsEmpty();
}
//---------------------------------------------------------------------------
//一時アーカイブとして開く
//---------------------------------------------------------------------------
void __fastcall TGitViewer::OpenTmpArcActionExecute(TObject *Sender)
{
	if (!CommitID.IsEmpty()) {
		UnicodeString tmp_name = TempPathA + CommitID.SubString(1, 8) + "_" + get_dir_name(WorkDir) + ".zip";
		UnicodeString prm;
		prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), CommitID.c_str(), yen_to_slash(tmp_name).c_str());
		if (GitExeList(prm, NULL, "アーカイブ作成中...") && file_exists(tmp_name)) {
			RetArcFile	= tmp_name;
			ModalResult = mrOk;
		}
	}
}

//---------------------------------------------------------------------------
//このコミットまでの履歴を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::LogThisCommitActionExecute(TObject *Sender)
{
	UpdateCommitList(CommitID);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::LogThisCommitActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CommitID.IsEmpty();
}

//---------------------------------------------------------------------------
//親コミットをblame
//---------------------------------------------------------------------------
void __fastcall TGitViewer::BlameActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		fnam = get_GitDiffFile2(fnam);

		UnicodeString prm;
		prm.sprintf(_T("gui blame %s %s"), get_tkn(ParentID, " ").c_str(), fnam.c_str());
		GitExeStr(prm);
	}
}
//---------------------------------------------------------------------------
//このファイルのコミット履歴を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::LogThisFileActionExecute(TObject *Sender)
{
	FilterName = GetDiffFileName();
	UpdateCommitList();
}

//---------------------------------------------------------------------------
//コミット情報を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitInfoActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	WarnList->Clear();

	//基本情報
	UnicodeString prm =
		"show --no-patch --date=format:\"%Y/%m/%d %H:%M:%S\" --pretty=format:\""
		"コミット=%H\n"
		"ツリー=%T\n"
		"親=%P\n\n"
		"作者=%an\n作者Email=%ae\n作者日付=%ad\n"
		"コミッタ=%cn\nコミッタEmail=%ce\nコミッタ日付=%cd\n\n"
		"件名=%s\n\"";
	prm.cat_sprintf(_T(" %s"), CommitID.c_str());
	TStringDynArray r_buf = GitExeStrArray(prm);
	if (r_buf.Length>0) {
		for (int i=0; i<r_buf.Length; i++) {
			UnicodeString nstr = get_tkn(r_buf[i], "=");
			if (nstr.IsEmpty()) {
				i_lst->Add(EmptyStr);
			}
			else {
				UnicodeString vstr = get_tkn_r(r_buf[i], "=");
				if (vstr.IsEmpty()) continue;
				int flag = (i<3)? LBFLG_GIT_HASH : EndsStr("日付", nstr)? LBFLG_TIME_FIF : 0;
				if (SameText(nstr, "親")) {
					for (int j=0; !vstr.IsEmpty(); j++) {
						add_PropLine((j==0)? nstr : UnicodeString(" "), split_tkn_spc(vstr), i_lst.get(), flag);
					}
				}
				else {
					add_PropLine(nstr, vstr, i_lst.get(), flag);
				}
			}
		}
	}

	//メッセージ
	prm.sprintf(_T("show --no-patch --pretty=medium %s"), CommitID.c_str());
	r_buf = GitExeStrArray(prm);
	if (r_buf.Length>0) {
		int cnt = 0;
		for (int i=0; i<r_buf.Length; i++) {
			UnicodeString lbuf = TrimLeft(r_buf[i]);
			if (cnt==0 && lbuf.IsEmpty()) {
				cnt++;
			}
			else if (cnt>0) {
				cnt++;
				if (cnt==2 || (cnt==3 && lbuf.IsEmpty())) continue;
				add_PropLine(" ", lbuf, i_lst.get());
			}
		}
	}

	//注釈
	prm.sprintf(_T("notes show %s"), CommitID.c_str());
	r_buf = GitExeStrArray(prm);
	if (r_buf.Length>0) {
		for (int i=0; i<r_buf.Length; i++) {
			UnicodeString lbuf = r_buf[i];
			if (r_buf.Length==1) lbuf = Trim(lbuf);
			add_PropLine((i==0)? "注釈" : " ", lbuf, i_lst.get());
		}
	}

	i_lst->Add(EmptyStr);

	//ブランチ
	prm.sprintf(_T("branch --contains %s"), CommitID.c_str());
	r_buf = GitExeStrArray(prm);
	if (r_buf.Length>0) {
		for (int i=0; i<r_buf.Length; i++) {
			UnicodeString lbuf = r_buf[i];
			if (r_buf.Length==1) lbuf = Trim(lbuf);
			add_PropLine((i==0)? "ブランチ" : " ", lbuf, i_lst.get());
		}
	}

	//タグ
	if (!TagNames.IsEmpty()) {
		TStringDynArray t_lst = split_strings_tab(TagNames);
		for (int i=0; i<t_lst.Length; i++) {
			add_PropLine("タグ", t_lst[i], i_lst.get(), LBFLG_GIT_TAG);
			//注釈
			prm = "show --no-patch --oneline " + t_lst[i];
			r_buf = GitExeStrArray(prm);
			if (r_buf.Length>0) {
				int cnt = 0;
				for (int i=0; i<r_buf.Length; i++) {
					UnicodeString lbuf = r_buf[i];
					if (cnt==0 && lbuf.IsEmpty()) {
						cnt++;
					}
					else if (cnt>0) {
						if (lbuf.IsEmpty()) break;
						if (StartsStr(get_tkn(lbuf, " "), CommitID)) continue;
						add_PropLine(" ", lbuf, i_lst.get());
						cnt++;
					}
				}
			}
		}
	}
	//最新タグ
	else {
		prm.sprintf(_T("describe --tags --abbrev=0 %s"), CommitID.c_str());
		r_buf = GitExeStrArray(prm);
		if (r_buf.Length>0) {
			add_PropLine("最新タグ", r_buf[0], i_lst.get(), LBFLG_GIT_TAG);
		}
	}

	//相対参照
	prm.sprintf(_T("name-rev --name-only %s"), CommitID.c_str());
	r_buf = GitExeStrArray(prm);
	if (r_buf.Length>0) {
		add_PropLine("相対参照", r_buf[0], i_lst.get(),
			StartsStr("tags/", r_buf[0])? LBFLG_GIT_TAG : 0);
	}

	//警告
	if (WarnList->Count>0) {
		i_lst->Add(EmptyStr);
		i_lst->AddStrings(WarnList);
		WarnList->Clear();
	}

	if (i_lst->Count>0) {
		FileInfoDlg->ItemList->Assign(i_lst.get());
		FileInfoDlg->isGitInfo = true;
		FileInfoDlg->inhNxtPre = true;
		FileInfoDlg->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitInfoActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = !CommitID.IsEmpty();
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//このファイルのハッシュをコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyFileHashActionExecute(TObject *Sender)
{
	UnicodeString prm;
	prm.sprintf(_T("rev-parse %s:%s"), CommitID.c_str(), GetDiffFileName().c_str());
	copy_to_Clipboard(GitExeStr(prm));
}
//---------------------------------------------------------------------------
//このファイルのパスをコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyPathActionExecute(TObject *Sender)
{
	copy_to_Clipboard(get_GitDiffFile2(GetDiffTextName()));
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyFileInfActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = !CommitID.IsEmpty() && !GetDiffFileName().IsEmpty();
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//このファイルの差分詳細を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffDetailActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();		if (!gp) return;
	UnicodeString fnam = GetDiffFileName();	if (fnam.IsEmpty()) return;

	UnicodeString fnam2;
	UnicodeString fnam1 = get_GitDiffFiles(fnam, fnam2);
	UnicodeString prm = "diff";

	if (gp->is_stash) {
		prm.cat_sprintf(_T(" %s %s"), gp->stash.c_str(), fnam2.c_str());
	}
	else if (gp->is_work) {
		prm.cat_sprintf(_T(" -- %s"), fnam1.c_str());
	}
	else if (gp->is_index) {
		prm.cat_sprintf(_T(" --cached -- %s"), fnam1.c_str());
	}
	else {
		UnicodeString parent = get_tkn(ParentID, ' ');
		if (parent.IsEmpty()) parent = GIT_NULL_ID;
		if (SameText(fnam1, fnam2)) {
			prm.cat_sprintf(_T(" %s %s"), parent.c_str(), CommitID.c_str());
			prm.cat_sprintf(_T(" -- %s"), add_quot_if_spc(fnam1).c_str());
		}
		else {
			prm.cat_sprintf(_T(" %s:%s %s:%s"),
				parent.c_str(), fnam1.c_str(), CommitID.c_str(), fnam2.c_str());
		}
	}

	GitBusy = true;
	std::unique_ptr<TStringList> o_lst(new TStringList());
	bool ok = (GitShellExe(prm, WorkDir, o_lst.get()) && o_lst->Count>0);
	GitBusy = false;
	if (ok) {
		UnicodeString tit = "$ git " + sha1_to_short(prm);
		o_lst->Insert(0, tit);
		GeneralInfoDlg->Caption = tit.sprintf(_T("差分詳細 - %s"), fnam2.c_str());
		GeneralInfoDlg->fromGitView = true;
		if (gp->is_work || gp->is_index)
			GeneralInfoDlg->FileName = IncludeTrailingPathDelimiter(WorkDir) + slash_to_yen(fnam1);
		GeneralInfoDlg->GenInfoList->Assign(o_lst.get());
		GeneralInfoDlg->ShowModal();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffDetailActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty();
}
//---------------------------------------------------------------------------
//このファイルの内容を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ViewFileActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();	if (!gp) return;

	UnicodeString fnam = GetDiffTextName();
	if (!fnam.IsEmpty()) {
		fnam = get_GitDiffFile2(fnam);

		bool ok;
		std::unique_ptr<TStringList> o_lst(new TStringList());
		if (gp->is_work) {
			fnam = IncludeTrailingPathDelimiter(WorkDir) + slash_to_yen(fnam);
			ok = load_text_ex(fnam, o_lst.get())!=0;
		}
		else {
			UnicodeString prm = "cat-file -p";
			if (gp->is_stash)
				prm.cat_sprintf(_T(" %s:./%s"), gp->stash.c_str(), fnam.c_str());
			else if (gp->is_index)
				prm.cat_sprintf(_T(" :%s"), fnam.c_str());
			else
				prm.cat_sprintf(_T(" %s:%s"), CommitID.c_str(), fnam.c_str());

			ok = GitExeList(prm, o_lst.get(), "ファイル抽出中...");
		}

		if (ok) {
			GeneralInfoDlg->Caption = UnicodeString().sprintf(_T("ファイル内容 - %s"), fnam.c_str());
			GeneralInfoDlg->fromGitView = true;
			GeneralInfoDlg->FileName = ExtractFileName(fnam);
			GeneralInfoDlg->GenInfoList->Assign(o_lst.get());
			GeneralInfoDlg->ShowModal();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ViewFileActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffTextName().IsEmpty();
}

//---------------------------------------------------------------------------
//リモートブランチを表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowRBranchActionExecute(TObject *Sender)
{
	ShowRBranchAction->Checked = !ShowRBranchAction->Checked;
	UpdateBranchList();
}
//---------------------------------------------------------------------------
//タグを表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ShowTagActionExecute(TObject *Sender)
{
	ShowTagAction->Checked = !ShowTagAction->Checked;
	UpdateBranchList();
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
void __fastcall TGitViewer::GitPopupMenuPopup(TObject *Sender)
{
	reduction_MenuLine(((TPopupMenu*)Sender)->Items);
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = CommitListBox;
	int c_idx = lp->ItemIndex;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(contained_wd_i(KeysStr_CsrUp,   KeyStr)) { FindUpAction->Execute();	Key = 0; }
	else if (contained_wd_i(KeysStr_CsrDown, KeyStr)) { FindDownAction->Execute();	Key = 0; }
	else if (contained_wd_i(KeysStr_ToList,  KeyStr) || equal_ENTER(KeyStr)) lp->SetFocus();
	else return;

	if (c_idx!=lp->ItemIndex) lp->Invalidate();
	KeyHandled = true;
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FindCommitEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (KeyHandled || Key==VK_RETURN || Key==VK_ESCAPE) {
		KeyHandled = false;
		Key = 0;
	}
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
	FindCommitEdit->Color = get_WinColor();
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
void __fastcall TGitViewer::FindBtnClick(TObject *Sender)
{
	FindCommitEdit->SetFocus();
}

//---------------------------------------------------------------------------
//更新
//---------------------------------------------------------------------------
void __fastcall TGitViewer::UpdateLogActionExecute(TObject *Sender)
{
	FilterName = EmptyStr;
	UpdateCommitList();
}

//---------------------------------------------------------------------------
//Diffツールを起動
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffToolActionExecute(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();		if (!gp) return;
	UnicodeString fnam = GetDiffFileName();	if (fnam.IsEmpty()) return;

	UnicodeString fnam2;
	UnicodeString fnam1 = get_GitDiffFiles(fnam, fnam2);
	UnicodeString prm = "difftool -y";

	if (gp->is_stash) {
		prm.cat_sprintf(_T(" %s %s"), gp->stash.c_str(), fnam2.c_str());
	}
	else if (gp->is_work) {
		prm.cat_sprintf(_T(" %s"), fnam2.c_str());
	}
	else if (gp->is_index) {
		prm.cat_sprintf(_T(" --cached %s"), fnam2.c_str());
	}
	else {
		prm.cat_sprintf(_T(" %s:%s %s:%s"), get_tkn(ParentID, ' ').c_str(), fnam1.c_str(), CommitID.c_str(), fnam2.c_str());
	}

	if (!GitShellExe(prm, WorkDir)) msgbox_ERR(USTR_FaildExec);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffActionUpdate(TObject *Sender)
{
	git_rec *gp = GetCurCommitPtr();
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty()
			&& ((gp && (gp->is_stash || gp->is_work || gp->is_index)) || (!CommitID.IsEmpty() && !ParentID.IsEmpty()));
}

//---------------------------------------------------------------------------
//git-guiを起動
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
//Consoleを起動
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
	git_rec *gp = GetCurCommitPtr();		if (!gp) return;
	UnicodeString fnam = GetDiffFileName();	if (fnam.IsEmpty()) return;

	fnam = get_GitDiffFile2(fnam);
	if (gp->is_work)
		fnam = IncludeTrailingPathDelimiter(WorkDir) + slash_to_yen(fnam);
	else if (gp->is_index)
		fnam = SaveRevAsTemp(EmptyStr, fnam);
	else
		fnam = SaveRevAsTemp(CommitID, fnam);

	if (!fnam.IsEmpty() && !open_by_TextEditor(fnam)) msgbox_ERR(GlobalErrMsg);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::EditFileActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	UnicodeString s = "テキストエディタで開く";
	s += (CommitID.IsEmpty()? "(&E)" : " - Temp (&E)");
	ap->Caption = s;
	ap->Enabled = !GetDiffTextName().IsEmpty();
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
//追加
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AddActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName(true);
	if (!fnam.IsEmpty()) GitExeStr("add " + add_quot_if_spc(fnam));
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AddActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	git_rec *gp = GetCurCommitPtr();
	ap->Enabled = (gp && gp->is_work && !GetDiffFileName(true).IsEmpty());
}

//---------------------------------------------------------------------------
//すべて追加
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AddAActionExecute(TObject *Sender)
{
	GitExeStr("add -A");
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AddUActionExecute(TObject *Sender)
{
	GitExeStr("add -u");
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::AddAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	git_rec *gp = GetCurCommitPtr();
	ap->Visible = (gp && gp->is_work);
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//取り消し
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ResetActionExecute(TObject *Sender)
{
	UnicodeString fnam = GetDiffFileName(true);
	if (!fnam.IsEmpty()) {
		//HEAD の存在確認
		UnicodeString prm = "rev-parse --verify --quiet HEAD";
		GitBusy = true;
		DWORD exit_code;
		bool ok = (GitShellExe(prm, WorkDir, (TStringList *)NULL, &exit_code) && exit_code==0);
		GitBusy = false;
		//取り消し
		prm = (ok? "reset HEAD " : "rm --cached ") + add_quot_if_spc(fnam);
		GitExeStr(prm);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ResetActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	git_rec *gp = GetCurCommitPtr();
	ap->Enabled = (gp && gp->is_index && !GetDiffFileName(true).IsEmpty());
}
//---------------------------------------------------------------------------
//すべて取り消し
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ResetAllActionExecute(TObject *Sender)
{
	//HEAD の存在確認
	UnicodeString prm = "rev-parse --verify --quiet HEAD";
	GitBusy = true;
	DWORD exit_code;
	bool ok = (GitShellExe(prm, WorkDir, (TStringList *)NULL, &exit_code) && exit_code==0);
	GitBusy = false;
	GitExeStr(ok? "reset HEAD" : "rm --cached -r .");
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ResetAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	git_rec *gp = GetCurCommitPtr();
	ap->Visible = (gp && gp->is_index);
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//コミット
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitActionExecute(TObject *Sender)
{
	if (!SetGitTagDlg) SetGitTagDlg = new TSetGitTagDlg(this);	//初回に動的作成
	SetGitTagDlg->IsCommit	 = true;
	SetGitTagDlg->EditMsgFile = IncludeTrailingPathDelimiter(WorkDir) + ".git\\COMMIT_EDITMSG";
	if (SetGitTagDlg->ShowModal() == mrOk) GitExeStr(SetGitTagDlg->GitParam);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = Staged;
}
//---------------------------------------------------------------------------
//待避
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashActionExecute(TObject *Sender)
{
	UnicodeString msg;
	if (input_query_ex(_T("待避 (stash)"), _T("メッセージ"), &msg)) {
		UnicodeString prm;
		prm.sprintf(_T("stash save \"%s\""), msg.c_str());
		GitExeStr(prm);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (StatusList->Count>0);
}

//---------------------------------------------------------------------------
//復帰/削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashPopActionExecute(TObject *Sender)
{
	if (!StashName.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("[%s] を復帰/削除しますか?"), StashName.c_str());
		if (msgbox_Sure(msg)) GitExeStr("stash pop " + StashName);
	}
}
//---------------------------------------------------------------------------
//復帰
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashApplyActionExecute(TObject *Sender)
{
	if (!StashName.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("[%s] を復帰しますか?"), StashName.c_str());
		if (msgbox_Sure(msg)) GitExeStr("stash apply " + StashName);
	}
}
//---------------------------------------------------------------------------
//削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashDropActionExecute(TObject *Sender)
{
	if (!StashName.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("[%s] を削除しますか?"), StashName.c_str());
		if (msgbox_Sure(msg)) GitExeStr("stash drop " + StashName);
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::StashPopActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = !StashName.IsEmpty();
	ap->Enabled = ap->Visible;
}
//---------------------------------------------------------------------------

