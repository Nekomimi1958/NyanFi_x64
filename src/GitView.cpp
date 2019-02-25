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
#include "FileInfDlg.h"
#include "GitTag.h"
#include "GenInfDlg.h"
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
	LastBrListIdx = -1;
	LastCmListIdx = -1;
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormCreate(TObject *Sender)
{
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

	ShowRBranchAction->Checked	= IniFile->ReadBoolGen(_T("GitViewShowRBranch"),true);
	ShowTagAction->Checked		= IniFile->ReadBoolGen(_T("GitViewShowTag"),	true);
	ShowBranchesAction->Checked = IniFile->ReadBoolGen(_T("GitViewShowBranches"));
	ShowRemoteAction->Checked	= IniFile->ReadBoolGen(_T("GitViewShowRemote"));
	ShowAuthorAction->Checked	= IniFile->ReadBoolGen(_T("GitViewShowAuthor"));
	AppFextColorAction->Checked = IniFile->ReadBoolGen(_T("GitViewFExtColor"),	true);

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
	DiffListBox->Clear();
	ClearCommitList();

	FilterName  = EmptyStr;
	HistoryLimit = GIT_DEF_HISTLIMIT;

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("GitViewBranchWidth"),		BranchPanel->Width);
	IniFile->WriteIntGen(_T("GitViewDiffHeight"),		DiffPanel->Height);
	IniFile->WriteIntGen(_T("GitViewFindWidth"),		FindCommitEdit->Width);
	IniFile->WriteBoolGen(_T("GitViewShowRBranch"),		ShowRBranchAction);
	IniFile->WriteBoolGen(_T("GitViewShowTag"),			ShowTagAction);
	IniFile->WriteBoolGen(_T("GitViewShowBranches"),	ShowBranchesAction);
	IniFile->WriteBoolGen(_T("GitViewShowRemote"),		ShowRemoteAction);
	IniFile->WriteBoolGen(_T("GitViewShowAuthor"),		ShowAuthorAction);
	IniFile->WriteBoolGen(_T("GitViewFExtColor"),		AppFextColorAction);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::FormDestroy(TObject *Sender)
{
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
			if (!contained_wd_i(_T("archive|cat-file|config|difftool|gui|rev-parse|show"), cmd)) {
				UpdateCommitList();
			}
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//git実行結果を TStringDynArray で取得
//  エラー表示/ 一覧の更新は行わない
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
		for (int i=0; i<o_lst->Count; i++) add_dyn_array(ret_array, o_lst->Strings[i]);
	}
	return ret_array;
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
git_rec * __fastcall TGitViewer::cre_GitRec(UnicodeString msg)
{
	git_rec *gp  = new git_rec;
	gp->is_head  = false;
	gp->is_work  = false;
	gp->is_index = false;
	gp->msg 	 = msg;
	return gp;
}

//---------------------------------------------------------------------------
void __fastcall TGitViewer::ClearCommitList()
{
	TListBox *lp = CommitListBox;
	for (int i=0; i<lp->Count; i++) delete (git_rec*)lp->Items->Objects[i];
	lp->Clear();

	LastBrListIdx = -1;
	LastCmListIdx = -1;

	MaxGrWidth = lp->ItemHeight;
	MaxAnWidth = 0;
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
	git_rec *tmp_gp = cre_GitRec("取得中...");
	tmp_gp->graph = "#";
	c_lp->Items->AddObject(EmptyStr, (TObject *)tmp_gp);
	c_lp->Repaint();

	GitBusy = true;
	std::unique_ptr<TStringList> c_lst(new TStringList());

	//ワーキングツリー/インデックス
	std::unique_ptr<TStringList> o_lst(new TStringList());
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
	UnicodeString prm = "log --graph";
	if (ShowBranchesAction->Checked && commit_id.IsEmpty()) prm += " --branches";
	if (HistoryLimit>0) prm.cat_sprintf(_T(" -%u"), HistoryLimit);
	if (!FilterName.IsEmpty()) prm.UCAT_T(" --follow");
	prm += " --date=format:\"%Y/%m/%d %H:%M:%S\" --pretty=format:\"\t%H\t%P\t%ad\t%s\t%d\t%an\"";
	if (!commit_id.IsEmpty())  prm.cat_sprintf(_T(" %s"), commit_id.c_str());
	if (!FilterName.IsEmpty()) prm.cat_sprintf(_T(" \"%s\""), FilterName.c_str());

	//グラフ / コミットID / 親ID / 日時 / 件名 / ブランチ / タグ / Author名
	int idx_h = -1;
	o_lst->Clear();
	UnicodeString last_parent;
	DWORD exit_code;
	if (GitShellExe(prm, WorkDir, o_lst.get(), &exit_code) && exit_code==0 && o_lst->Count>0) {
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
							if (!gp->tags.IsEmpty()) gp->tags.UCAT_T("\t");
							gp->tags += ss;
						}
						else if (ss.Pos("/")) {
							if (!EndsStr("/HEAD", ss)) gp->branch_r = ss;
						}
						else {
							gp->is_head = remove_top_text(ss, "HEAD -> ") || USAME_TS(ss, "HEAD");
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
			if (GitShellExe(prm, WorkDir, o_lst.get()) && o_lst->Count>0) {
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
	c_lp->Clear();
	delete tmp_gp;
	c_lp->Items->Assign(c_lst.get());
	SetCommitListIndex(idx_h);
	CommitScrPanel->UpdateKnob();

	GitBusy = false;

	UpdateBranchList();
}

//---------------------------------------------------------------------------
//ブランチ一覧/コミット履歴の更新
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
	if (GitShellExe("branch", WorkDir, o_lst.get())) { 
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
		if (GitShellExe("branch -r", WorkDir, o_lst.get())) { 
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
		if (GitShellExe("tag", WorkDir, o_lst.get())) { 
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
				UnicodeString tag = o_lst->Strings[i];  if (tag.IsEmpty()) continue;
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

	int xp = Rect.Left + 4;
	int yp = Rect.Top + 2;
	UnicodeString lbuf = lp->Items->Strings[Index];
	//ヘッダ
	if (flag & GIT_FLAG_HDRLN) {
		out_TextEx(cv, xp, yp, lbuf, col_fgListHdr, col_bgListHdr);
	}
	//項目
	else {
		//HEADマーク
		UnicodeString s = _T("\u25b6");
		if (flag & GIT_FLAG_HEAD) {
			cv->Font->Color = (flag & GIT_FLAG_REMOTE)? col_GitBraR : col_GitHEAD;
			cv->TextOut(xp, yp, s);
		}
		xp += cv->TextWidth(s);
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
	UnicodeString KeyStr  = get_KeyStr(Key, Shift);
	UnicodeString cmd_F = Key_to_CmdF(KeyStr);

	if		(ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
													BranchListBoxClick(BranchListBox);
	else if (is_ToRightOpe(KeyStr, cmd_F))			CommitListBox->SetFocus();
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (USAME_TI(cmd_F, "RenameDlg")) 			RenBranchAction->Execute();
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
	cv->Brush->Color = is_match? col_matchItem : lp->Color;
	cv->FillRect(Rect);

	git_rec *gp = (git_rec *)lp->Items->Objects[Index];
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;

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
		cv->Font->Color = col_Folder;
		out_TextEx(cv, xp, yp, s);
		cv->Font->Color = col_fgList;
		cv->TextOut(xp, yp, gp->msg);
	}
	else if (!gp->hash.IsEmpty()) {
		//ハッシュ
		cv->Font->Color = col_GitHash;
		cv->TextOut(xp, yp, gp->hash.SubString(1, 7));
		xp += cv->TextWidth("9999999 ");

		//Author
		if (ShowAuthorAction->Checked) {
			cv->Font->Color = col_fgList;
			cv->TextOut(xp, yp, gp->author);
			xp += (MaxAnWidth + Scaled8);
		}

		//日時
		UnicodeString tmp = FormatDateTime(TimeStampFmt, gp->f_time);
		cv->Font->Color = get_TimeColor(gp->f_time, col_fgList);
		cv->TextOut(xp, yp, tmp);
		xp += (cv->TextWidth(tmp) + Scaled8);

		//タグ
		if (!gp->tags.IsEmpty()) {
			TStringDynArray t_lst = split_strings_tab(gp->tags);
			for (int i=0; i<t_lst.Length; i++) draw_GitTag(cv, xp, yp, t_lst[i], Scaled2);
			xp += Scaled4;
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
		cv->Font->Color = col_fgList;
		cv->TextOut(xp, yp, gp->msg);
	}

	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxClick(TObject *Sender)
{
	DiffListBox->Clear();
	CommitID = ParentID = BranchName = EmptyStr;
	MaxDfWidth = 0;

	TListBox *c_lp = CommitListBox;
	int idx = c_lp->ItemIndex;	if (idx==-1) return;
	if (idx!=LastCmListIdx) {
		bool found = false;
		bool is_dw = idx>LastCmListIdx;
		for (int i=idx; (is_dw? i<c_lp->Count : i>=0) && !found; i+=(is_dw? 1 : -1)) {
			git_rec *gp = (git_rec *)c_lp->Items->Objects[i];
			if (gp->is_work || gp->is_index || !gp->hash.IsEmpty()) {
				idx = i;  found = true;
			}
		}
		if (!found) idx = LastCmListIdx;
		c_lp->ItemIndex = LastCmListIdx = idx;
	}

	git_rec *gp = (git_rec *)c_lp->Items->Objects[idx];
	if (!gp->is_work && !gp->is_index && gp->hash.IsEmpty()) return;

	CommitID   = gp->hash;
	ParentID   = gp->parent;
	BranchName = gp->branch;
	TagNames   = gp->tags;

	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (gp->diff_inf.IsEmpty()) {
		UnicodeString prm = "diff --stat-width=120 ";
		UnicodeString parent = get_tkn(ParentID, ' ');
		if		(gp->is_work)		;
		else if (gp->is_index)		 prm += "--cached";
		else if (ParentID.IsEmpty()) prm += CommitID;
		else 						 prm += (parent + " " + CommitID);

		if (!FilterName.IsEmpty()) prm.cat_sprintf(_T(" -- \"%s\""), FilterName.c_str());
		GitBusy = true;
		if (GitShellExe(prm, WorkDir, o_lst.get()) && o_lst->Count>0)
			gp->diff_inf = o_lst->Text;
		GitBusy = false;
	}
	else {
		o_lst->Text = gp->diff_inf;
	}

	//リストボックスに割り当て
	TListBox *d_lp = DiffListBox;
	int max_fwd = 0;
	for (int i=0; i<o_lst->Count; i++) {
		UnicodeString lbuf = o_lst->Strings[i];
		if (lbuf.Pos(" | "))
			max_fwd = std::max(max_fwd, d_lp->Canvas->TextWidth(get_tkn(lbuf, " | ") + " "));
	}
	MaxDfWidth = max_fwd;
	d_lp->Items->Assign(o_lst.get());
	d_lp->ItemIndex = 0;
	DiffScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CommitListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
													CommitListBoxClick(NULL);
	else if (is_ToRightOpe(KeyStr, cmd_F))			DiffListBox->SetFocus();
	else if (is_ToLeftOpe(KeyStr, cmd_F))			BranchListBox->SetFocus();
	else if (USAME_TI(cmd_F, "ReturnList"))			ModalResult = mrCancel;
	else if (USAME_TI(cmd_F, "IncSearch"))			FindCommitEdit->SetFocus();
	else if (USAME_TI(cmd_F, "ShowFileInfo"))		CommitInfoAction->Execute();
	else if (USAME_TI(get_CmdStr(cmd_F), "Pack"))	ArchiveAction->Execute();
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
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

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top + Scaled2;

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (lbuf.Pos(" | ")) {
		//ファイル名
		UnicodeString s = split_tkn(lbuf, " | ");
		if (s.Pos(" => ")) {
			UnicodeString s1 = split_tkn(s, " => ");
			out_TextEx(cv, xp, yp, s1,
				AppFextColorAction->Checked? get_ExtColor(get_extension(s1), col_fgTxtPrv) : col_fgTxtPrv);
			out_TextEx(cv, xp, yp, " => ", col_fgTxtPrv);
		}
		out_TextEx(cv, xp, yp, s,
			AppFextColorAction->Checked? get_ExtColor(get_extension(Trim(s)), col_fgTxtPrv) : col_fgTxtPrv);

		//罫線
		TRect rc = Rect; rc.Left = xp = MaxDfWidth;
		RuledLnTextOut("│", cv, rc, col_fgTxtPrv);
		xp = rc.Left;
		//統計
		s = split_tkn_spc(lbuf) + " ";
		if (StartsText("Bin", Trim(s))) {
			out_TextEx(cv, xp, yp, s,	 AdjustColor(col_fgTxtPrv, 72));
			lbuf = ReplaceStr(lbuf, "->", "→");
			out_TextEx(cv, xp, yp, lbuf, col_fgTxtPrv);
		}
		else {
			//変更数
			UnicodeString tmp = ReplaceStr(s, " ", "0");
			xp += (cv->TextWidth(tmp) - cv->TextWidth(s));
			out_TextEx(cv, xp, yp, s, col_fgTxtPrv);
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
	else {
		for (int i=1; i<=lbuf.Length(); i++) {
			WideChar c = lbuf[i];
			out_TextEx(cv, xp, yp, c, (c=='-')? col_GitDel : (c=='+')? col_GitIns : col_fgTxtPrv);
		}
	}

	cv->Brush->Color = lp->Color;
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = (TListBox*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if		(ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, Key_to_CmdV(KeyStr)))
												;
	else if (USAME_TI(cmd_F, "ReturnList")) 	ModalResult = mrCancel;
	else if (is_ToLeftOpe(KeyStr, cmd_F))		BranchListBox->SetFocus();
	else if (is_ToRightOpe(KeyStr, cmd_F))		CommitListBox->SetFocus();
	else if (USAME_TI(cmd_F, "FileEdit"))		EditFileAction->Execute();
	else if (USAME_TI(cmd_F, "ShowFileInfo"))	DiffDetailAction->Execute();

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
	SetGitTagDlg->CommitID	 = BranchListBox->Focused()? GetCurBranchName(true, true) : CommitID;
	SetGitTagDlg->TagMsgFile = IncludeTrailingPathDelimiter(WorkDir) + ".git\\TAGMESSAGE";
	if (SetGitTagDlg->ShowModal()==mrOk) GitExeStr(SetGitTagDlg->GitParam);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::SetTagActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	if (BranchListBox->Focused()) {
		ap->Visible = !GetCurBranchName(true, true).IsEmpty();
	}
	else if (CommitListBox->Focused())
		ap->Visible = !CommitID.IsEmpty();
	else
		ap->Visible = false;
}

//---------------------------------------------------------------------------
//タグの削除
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DelTagActionExecute(TObject *Sender)
{
	UnicodeString tag = get_pre_tab(TagNames);
	if (!tag.IsEmpty()) {
		UnicodeString msg;
		msg.sprintf(_T("タグ[%s]を削除してもよいですか?"), tag.c_str());
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
//ブランチ名をコピー
//---------------------------------------------------------------------------
void __fastcall TGitViewer::CopyBranchNameActionExecute(TObject *Sender)
{
	copy_to_Clipboard(BranchName);
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
	msg.sprintf(_T("[%s]を直前のコミットに戻しますか(%s)?"), RefHEAD.c_str(), prm.c_str());
	if (msgbox_Sure(msg)) GitExeStr("reset --" + prm.LowerCase() + " HEAD^");
}

//---------------------------------------------------------------------------
//アーカイブ作成
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionExecute(TObject *Sender)
{
	if (!CommitID.IsEmpty()) {
		UserModule->PrepareSaveDlg(_T("アーカイブの作成"), _T("アーカイブ (*.zip)|*.zip"), _T("archive.zip"), WorkDir);
		if (UserModule->SaveDlg->Execute()) {
			UnicodeString arc_name = UserModule->SaveDlg->FileName;
			UnicodeString prm;
			prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), CommitID.c_str(), yen_to_slash(arc_name).c_str());
			GitExeStr(prm);
		}
	}
}
//---------------------------------------------------------------------------
//一時アーカイブとして開く
//---------------------------------------------------------------------------
void __fastcall TGitViewer::OpenTmpArcActionExecute(TObject *Sender)
{
	if (!CommitID.IsEmpty()) {
		UnicodeString tmp_name = TempPathA + CommitID.SubString(1, 8) + "_"
									+ ExtractFileName(ExcludeTrailingPathDelimiter(WorkDir)) + ".zip";
		UnicodeString prm;
		prm.sprintf(_T("archive --format=zip %s --output=\"%s\""), CommitID.c_str(), yen_to_slash(tmp_name).c_str());

		if (GitShellExe(prm, WorkDir) && file_exists(tmp_name)) {
			RetArcFile	= tmp_name;
			ModalResult = mrOk;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::ArchiveActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Visible = !CommitID.IsEmpty();
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
		if (fnam.Pos(" => ")) fnam =  get_tkn_r(fnam, " => ");
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

	//基本情報
	UnicodeString prm =
		"show --quiet --date=format:\"%Y/%m/%d %H:%M:%S\" --pretty=format:\""
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

	if (i_lst->Count>0) {
		TListBox *lp = FileInfoDlg->InfListBox;
		assign_InfListBox(lp, i_lst.get());
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
//このファイルの差分詳細を表示
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffDetailActionExecute(TObject *Sender)
{
	TListBox *c_lp = CommitListBox;
	int idx = c_lp->ItemIndex;	if (idx==-1) return;
	git_rec *gp = (git_rec *)c_lp->Items->Objects[idx];

	UnicodeString fnam = GetDiffFileName();
	if (!fnam.IsEmpty()) {
		UnicodeString prm = "diff";
		UnicodeString fnam2;
		UnicodeString fnam1  = get_GitDiffFiles(fnam, fnam2);

		if		(gp->is_work)		 prm.cat_sprintf(_T(" -- %s"), fnam1.c_str());
		else if (gp->is_index)		 prm.cat_sprintf(_T(" --cached -- %s"), fnam1.c_str());
		else if (ParentID.IsEmpty()) prm.cat_sprintf(_T(" %s -- %s"), CommitID.c_str(), fnam1.c_str());
		else {
			UnicodeString parent = get_tkn(ParentID, ' ');
			if (SameText(fnam1, fnam2)) {
				prm.cat_sprintf(_T(" %s %s"),  parent.c_str(), CommitID.c_str());
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
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffDetailActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty();
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
	TListBox *lp = CommitListBox;
	int c_idx = lp->ItemIndex;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(contained_wd_i(KeysStr_CsrUp, KeyStr))   KeyStr = "UP";
	else if (contained_wd_i(KeysStr_CsrDown, KeyStr)) KeyStr = "DOWN";

	if		(equal_UP(KeyStr))	 { FindUpAction->Execute();		Key = 0; }
	else if (equal_DOWN(KeyStr)) { FindDownAction->Execute();	Key = 0; }
	else if (contained_wd_i(KeysStr_ToList, KeyStr) || equal_ENTER(KeyStr)) lp->SetFocus();
	else return;

	if (c_idx!=lp->ItemIndex) lp->Invalidate();
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
	TListBox *c_lp = CommitListBox;
	int idx = c_lp->ItemIndex;	if (idx==-1) return;
	git_rec *gp = (git_rec *)c_lp->Items->Objects[idx];

	UnicodeString fnam = GetDiffFileName();
	if (fnam.IsEmpty()) return;

	UnicodeString prm = "difftool -y";
	UnicodeString fnam2;
	UnicodeString fnam1 = get_GitDiffFiles(fnam, fnam2);

	if		(gp->is_work)	prm.cat_sprintf(_T(" %s"), fnam2.c_str());
	else if (gp->is_index)	prm.cat_sprintf(_T(" --cached %s"), fnam2.c_str());
	else if (!CommitID.IsEmpty()) {
		if (!ParentID.IsEmpty()) {
			prm.cat_sprintf(_T(" %s:%s %s:%s"),
				get_tkn(ParentID, ' ').c_str(), fnam1.c_str(),
				CommitID.c_str(), fnam2.c_str());
		}
		else {
			prm.cat_sprintf(_T(" %s %s"), CommitID.c_str(), fnam2.c_str());
		}
	}

	if (!GitShellExe(prm, WorkDir)) msgbox_ERR(USTR_FaildExec);
}
//---------------------------------------------------------------------------
void __fastcall TGitViewer::DiffActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = !GetDiffFileName().IsEmpty();
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
	if (USAME_TI(KeyStr, "F5")) UpdateLogAction->Execute();
	else SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_GIT) _T("#GitViewer"));
}
//---------------------------------------------------------------------------

