//----------------------------------------------------------------------//
// NyanFi																//
//  ディレクトリ履歴/ スタック											//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MainFrm.h"
#include "HistDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDirHistoryDlg *DirHistoryDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TDirHistoryDlg::TDirHistoryDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::FormCreate(TObject *Sender)
{
	ListBuff = new TStringList();

	ListScrPanel = new UsrScrollPanel(ListPanel, DirHistListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	IsDirStack = IsAllDirHist = IsFindDirHist = IdRecentDir = false;
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	IsMigemo = IniFile->ReadBoolGen(_T("DirHistoryMigemo"));

	RefListTag	 = CurListTag;
	WorkListName = EmptyStr;
	IncSeaWord	 = EmptyStr;

	FindBusy = FindAborted = false;
	HintPanel->Visible = false;

	setup_Panel(InpPanel, ListFont);
	InpPanel->Color   = col_bgList;
	InpPanel->Visible = IsFindDirHist;
	if (IsFindDirHist) UserModule->SetBlinkTimer(InpPaintBox);

	set_StdListBox(DirHistListBox);
	set_UsrScrPanel(ListScrPanel);

	int *h_ptr = (IsDirStack || IsAllDirHist)? NULL : get_DirHistPtr(CurTabIndex, CurListTag);
	UpdateListBox(h_ptr? *h_ptr : 0);
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->SetBlinkTimer(NULL);

	if (FindBusy) FindAborted = true;

	IsDirStack = IsAllDirHist = IsFindDirHist = IdRecentDir = false;

	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("DirHistoryMigemo"), IsMigemo);
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::FormDestroy(TObject *Sender)
{
	delete ListBuff;
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (!FindBusy && Key==VK_ESCAPE)
		Close();
	else
		SpecialKeyProc(this, Key, Shift);
}

//---------------------------------------------------------------------------
//一覧の更新
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::UpdateListBox(
	int idx)	//インデックス	(default = 0)
{
	if (IsFindDirHist) UserModule->RepaintBlink();

	ListBuff->Clear();

	//フィルタ検索モード
	if (IsFindDirHist) {
		if (!IncSeaWord.IsEmpty()) {
			std::unique_ptr<TStringList> f_lst(new TStringList());
			SearchOption opt;  opt << soAndOr << soCSV;
			if (IsMigemo) opt << soMigemo;
			if (contains_upper(IncSeaWord)) opt << soCaseSens;
			filter_List(AllDirHistory, f_lst.get(), slash_to_yen(IncSeaWord), opt);
			ListBuff->Assign(f_lst.get());
		}
		else {
			ListBuff->Assign(AllDirHistory);
		}
	}
	//最近使ったディレクトリ
	else if (IdRecentDir) {
		UnicodeString dnam = usr_SH->KnownGuidToPath(FOLDERID_Recent);
		if (!dnam.IsEmpty()) {
			std::unique_ptr<TStringList> i_lst(new TStringList());
			std::unique_ptr<TStringList> lnk_lst(new TStringList());
			get_files(dnam, "*.lnk", lnk_lst.get());
			for (int i=0; i<lnk_lst->Count; i++) {
				UnicodeString fnam = lnk_lst->Strings[i];
				UnicodeString lnam = usr_SH->get_LnkName(fnam);
				if (lnam.IsEmpty() || StartsStr("::{", lnam)) continue;
				UnicodeString pnam = ExtractFilePath(lnam);
				if (!NoCheckRecentUnc || !StartsStr("\\\\", pnam)) {
					if (!is_InvalidUnc(pnam) && NyanFiForm->CheckUncPath(pnam) && !dir_exists(lnam)) continue;
				}
				else {
					if (!dir_exists(lnam)) continue;
				}
				//降順ソートのために使用日時を一時的に付加
				i_lst->Add(FormatDateTime("yyyymmddhhnnss", get_file_age(fnam))
							+ "\t\"" + IncludeTrailingPathDelimiter(lnam) + "\",\"" + fnam + "\"");
			}
			i_lst->CustomSort(comp_DescendOrder);
			for (int i=0; i<i_lst->Count; i++) {
				UnicodeString lbuf = get_post_tab(i_lst->Strings[i]);
				if (ListBuff->IndexOf(lbuf)==-1) ListBuff->Add(lbuf);
			}
		}
	}
	//通常モード
	else {
		TStringList *h_lst = IsDirStack? DirStack : IsAllDirHist? AllDirHistory : get_DirHistory(CurTabIndex, RefListTag);
		if (h_lst) ListBuff->Assign(h_lst);
	}

	//リストボックスに割り当て(仮想)
	TListBox *lp = DirHistListBox;
	int top = lp->TopIndex;
	lp->Count = ListBuff->Count;
	lp->TopIndex = top;
	if (lp->Count>0) lp->ItemIndex = (idx<lp->Count)? idx : lp->Count - 1;
	lp->SetFocus();
	ListScrPanel->UpdateKnob();

	//タイトル
	UnicodeString tit;
	if (IsDirStack) {
		tit = "ディレクトリ・スタック";
	}
	else if (IsFindDirHist) {
		tit.sprintf(_T("ディレクトリ履歴 - 全体検索 (%u/%u)"), ListBuff->Count, AllDirHistory->Count);
	}
	else if (IsAllDirHist) {
		tit = "ディレクトリ履歴 - 全体";
	}
	else if (IdRecentDir) {
		tit = "最近使ったディレクトリ";
	}
	else {
		tit.sprintf(_T("ディレクトリ履歴 - %s"), get_LRUD_str(RefListTag).c_str());
		if (RefListTag!=CurListTag) tit += " (反対側を参照)";
	}
	Caption = tit;
}

//---------------------------------------------------------------------------
//項目の描画
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::DirHistListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + SCALED_THIS(4);
	int yp = Rect.Top  + get_TopMargin2(cv);

	if (!IsFindDirHist) {
		if (Index<10) {
			cv->Font->Color = col_fgList;
			cv->Font->Style = cv->Font->Style << fsUnderline;
			cv->TextOut(xp, yp, UnicodeString().sprintf(_T("%u"), (Index + 1)%10));
		}
		cv->Font->Style = cv->Font->Style >> fsUnderline;
		xp += get_CharWidth(cv, 2);
	}

	UnicodeString lbuf = get_csv_item(lp->Items->Strings[Index], 0);
	if (UncToNetDrive) lbuf = UNC_to_NetDriveName(lbuf);
	if (DispRegName)   lbuf = get_RegDirName(lbuf);

	cv->Font->Color = col_Folder;
	if (IsFindDirHist && !IncSeaWord.IsEmpty()) {
		std::unique_ptr<TStringList> wlist(new TStringList());
		SearchOption opt;  opt << soAndOr;
		if (IsMigemo) opt << soMigemo;
		if (contains_upper(IncSeaWord)) opt << soCaseSens;
		get_MatchWordList(lbuf, slash_to_yen(IncSeaWord), opt, wlist.get());
		PathNameOut(lbuf, wlist.get(), opt.Contains(soCaseSens), cv, xp, yp);
	}
	else {
		PathNameOut(lbuf, NULL, false, cv, xp, yp);
	}

	//カーソル
	draw_ListCursor2(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::DirHistListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<ListBuff->Count)? ListBuff->Strings[Index] : EmptyStr;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::DirHistListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;

	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_S  = IsFindDirHist? KeyFuncList->Values["S:" + KeyStr] : EmptyStr;
	bool handled = true;

	try {
		bool is_all = (IsAllDirHist || IsFindDirHist);
		TStringList *h_lst = is_all? AllDirHistory :
						 IsDirStack? DirStack : get_DirHistory(CurTabIndex, RefListTag);
		int *h_ptr = (IsDirStack || IsAllDirHist || IsFindDirHist)? NULL : get_DirHistPtr(CurTabIndex, CurListTag);

		TListBox *lp = (TListBox*)Sender;
		UnicodeString cur_dnam = (lp->ItemIndex!=-1)? get_csv_item(lp->Items->Strings[lp->ItemIndex], 0) : EmptyStr;

		//確定
		if ((!IsFindDirHist && is_Num0to9(KeyStr)) || equal_ENTER(KeyStr)) {
			int idx = is_Num0to9(KeyStr)? ((KeyStr.ToIntDef(0) + 9)%10) : lp->ItemIndex;
			if (idx<0 || idx>=lp->Count) SkipAbort();
			UnicodeString dnam = get_csv_item(lp->Items->Strings[idx], 0);
			//ディレクトリ
			if (ends_PathDlmtr(dnam)) {
				if (!is_drive_accessible(dnam)) TextAbort(_T("ドライブが無効です。"));

				if (dir_exists(dnam)) {
					bool is_cur = (RefListTag==CurListTag);
					if (!is_all && is_cur && h_ptr) *h_ptr = idx;
					NyanFiForm->UpdateCurPath(dnam, -1, is_cur);
					ModalResult = mrOk;
				}
				else {
					msgbox_WARN(LoadUsrMsg(USTR_NotFound));
					//存在しないディレクトリを削除
					if (h_lst) {
						int i = 0;
						while (i<h_lst->Count) {
							if (StartsText(dnam, get_csv_item(h_lst->Strings[i], 0))) h_lst->Delete(i); else i++;
						}
						UpdateListBox(idx);
					}
				}
			}
			//ワークリスト
			else if (test_NwlExt(get_extension(dnam))) {
				WorkListName = dnam;
				ModalResult  = mrOk;
			}
		}
		//フィルタ検索
		else if (IsFindDirHist && update_IncSeaWord(IncSeaWord, KeyStr)) {
			UpdateListBox();
		}
		else if (SameText(cmd_S, "ClearIncKeyword")) {
			IncSeaWord = EmptyStr;
			UpdateListBox();
		}
		else if (contained_wd_i("MigemoMode|NormalMode", cmd_S)) {
			IsMigemo = SameText(cmd_S, "MigemoMode")? (!IsMigemo && usr_Migemo->DictReady) : false;
			UpdateListBox();
		}
		//カーソル移動
		else if (SameText(cmd_S, "IncSearchUp"))	ListBoxCursorUp(lp);
		else if (SameText(cmd_S, "IncSearchDown"))	ListBoxCursorDown(lp);
		else if (MovListBoxFromFilter(lp, KeyStr))	;
		else if	(ExeCmdListBox(lp, cmd_F))			;
		//履歴項目の削除
		else if (equal_DEL(KeyStr) || SameText(cmd_F, "Delete")) {
			if (lp->ItemIndex==-1) SkipAbort();
			if (IsFindDirHist) {
				for (;;) {
					int idx = indexof_csv_list(AllDirHistory, cur_dnam, 0);
					if (idx!=-1) AllDirHistory->Delete(idx); else break;
				}
			}
			else if (IdRecentDir) {
				if (lp->ItemIndex==-1) SkipAbort();
				UnicodeString fnam = get_csv_item(lp->Items->Strings[lp->ItemIndex], 1);
				if (!file_exists(fnam) || !delete_File(fnam)) Abort();
			}
			else {
				if (h_lst) h_lst->Delete(lp->ItemIndex);
			}
			UpdateListBox(lp->ItemIndex);
		}
		//反対側の履歴を参照
		else if (!IsFindDirHist && is_ToRightOpe(KeyStr, cmd_F)) {
			if (h_ptr && RefListTag==0) {
				RefListTag = 1;
				int *h_p = get_DirHistPtr(CurTabIndex, RefListTag);
				if (h_p) UpdateListBox(*h_p);
			}
		}
		else if (!IsFindDirHist && is_ToLeftOpe(KeyStr, cmd_F)) {
			if (h_ptr && RefListTag==1) {
				RefListTag = 0;
				int *h_p = get_DirHistPtr(CurTabIndex, RefListTag);
				if (h_p) UpdateListBox(*h_p);
			}
		}
		//プロパティ
		else if (SameText(cmd_F, "PropertyDlg"))		PropertyAction->Execute();
		//右クリックメニュー
		else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
		//閉じる
		else if (SameText(cmd_F, "ReturnList")) ModalResult = mrCancel;
		else handled = false;
	}
	catch (EAbort &e) {
		UnicodeString msg = e.Message;
		if (SameText(msg, "SKIP") || StartsText("Abort", msg)) msg = EmptyStr;
		if (!msg.IsEmpty()) msgbox_WARN(msg); else beep_Warn();
	}

	if (!is_DialogKey(Key) || handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::DirHistListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//インクリメンタルサーチを回避
	if (_istalnum(Key) || is_KeyDown(VK_CONTROL)) Key = 0;
}
//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::DirHistListBoxDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//入力欄の描画
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::InpPaintBoxPaint(TObject *Sender)
{
	draw_InputPaintBox((TPaintBox*)Sender, (IsMigemo? "Migemo: " : "") + IncSeaWord);
}
//---------------------------------------------------------------------------
//指定ディレクトリ以下を全体履歴に追加
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::AddDirsCore(
	UnicodeString pnam,
	TStrings *lst,
	TStringDynArray ex_lst)	//除外リスト	(環境変数、%ExePath% 対応)
{
	if (pnam.IsEmpty() || !dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);
	TSearchRec sr;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		do {
			if ((sr.Attr & faDirectory)==0 || ContainsStr("..", sr.Name)) continue;
			UnicodeString snam = pnam + sr.Name;

			//除外
			for (int i=0; i<ex_lst.Length && !snam.IsEmpty(); i++) {
				if (ex_lst[i].IsEmpty()) continue;
				if (ContainsText(snam, cv_env_str(ex_lst[i]))) snam = EmptyStr;
			}
			if (snam.IsEmpty()) continue;

			lst->Add(snam);
			HintLabel2->Caption = lst->Count;
			HintLabel2->Invalidate();

			AddDirsCore(snam, lst, ex_lst);
			if (is_KeyPress_ESC()) FindAborted = true;	//中断
		} while(!FindAborted && FindNext(sr)==0);
		FindClose(sr);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::AddDirsActionExecute(TObject *Sender)
{
	UnicodeString dnam = CurPathName;
	if (UserModule->SelectDirEx(_T("全体履歴に追加するディレクトリを選択"), dnam)) {
		FindBusy	= true;
		FindAborted = false;

		//検索中ヒント表示の設定
		AssignScaledFont(HintLabel1, HintFont);
		AssignScaledFont(HintLabel2, HintFont);
		HintLabel1->Caption   = "検索中...　ESCキーで中断";
		HintLabel2->Caption   = EmptyStr;
		HintPanel->Color	  = col_bgHint;
		HintPanel->BoundsRect = DirHistListBox->ClientRect.CenteredRect(HintPanel->ClientRect);
		HintPanel->Visible	  = true;
		HintPanel->Repaint();
		cursor_HourGlass();

		TStringDynArray ex_lst = split_strings_semicolon(NoDirHistPath);
		std::unique_ptr<TStringList> sbuf(new TStringList());
		sbuf->Add(IncludeTrailingPathDelimiter(dnam));
		AddDirsCore(dnam, sbuf.get(), ex_lst);

		HintLabel1->Caption = "追加中...　ESCキーで中断";
		HintLabel2->Caption = "0";
		int cnt = 0;
		UnicodeString all_buf = AllDirHistory->Text.UpperCase();
		for (int i=0; !FindAborted && i<sbuf->Count; i++) {
			UnicodeString pnam;
			pnam.sprintf(_T("\"%s\""), IncludeTrailingPathDelimiter(sbuf->Strings[i]).c_str());
			if (all_buf.Pos(pnam.UpperCase())==0) {
				AllDirHistory->Add(pnam + ",0");
				HintLabel2->Caption = ++cnt;
				HintLabel2->Invalidate();
			}
			if (is_KeyPress_ESC()) FindAborted = true;		//中断
		}

		AllDirHistory->Sort();
		UpdateListBox();

		cursor_Default();
		HintPanel->Visible = false;
		FindBusy = false;

		if (FindAborted)
			msgbox_WARN(LoadUsrMsg(USTR_Canceled));
		else
			msgbox_OK(UnicodeString().sprintf(_T("%u個のディレクトリを追加しました。"), cnt));
	}
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::AddDirsActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = IsFindDirHist;
	ap->Enabled = ExtSaveDirHist && !FindBusy;
}

//---------------------------------------------------------------------------
//履歴をすべて削除
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::ClearAllActionExecute(TObject *Sender)
{
	if (IsAllDirHist || IsFindDirHist) {
		if (msgbox_Sure(LoadUsrMsg(USTR_DelHistoryQ, _T("全体"))))
			AllDirHistory->Clear();
	}
	else if (!IsDirStack) {
		if (msgbox_Sure(LoadUsrMsg(USTR_DelHistoryQ, get_LRUD_str() + "側")))
			clear_DirHistory(CurTabIndex, CurListTag);
	}
	UpdateListBox();
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::ClearAllActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !IsDirStack && !FindBusy;
	ap->Caption = (IsAllDirHist || IsFindDirHist)? "全体履歴をすべて削除(&A)" : "カレント側の履歴をすべて削除(&A)";
}
//---------------------------------------------------------------------------
//絞り込み表示中の履歴を削除
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::ClearFltActionExecute(TObject *Sender)
{
	if (msgbox_Sure(LoadUsrMsg(USTR_DelHistoryQ, _T("絞り込み表示中")))) {
		cursor_HourGlass();
		for (int i=0; i<ListBuff->Count; i++) {
			UnicodeString dnam = get_csv_item(ListBuff->Strings[i], 0);
			int idx = indexof_csv_list(AllDirHistory, dnam, 0);
			if (idx!=-1) AllDirHistory->Delete(idx);
			Application->ProcessMessages();
		}
		UpdateListBox();
		cursor_Default();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::ClearFltActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = IsFindDirHist;
	ap->Enabled = !FindBusy && ListBuff->Count>0 && ListBuff->Count<AllDirHistory->Count;
}

//---------------------------------------------------------------------------
//一覧をコピー
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::CopyAllActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	for (int i=0; i<ListBuff->Count; i++)
		lst->Add(get_csv_item(ListBuff->Strings[i], 0));

	copy_to_Clipboard(lst->Text);
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::CopyAllActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FindBusy && ListBuff->Count>0;
}

//---------------------------------------------------------------------------
//ディレクトリのプロパティ
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::PropertyActionExecute(TObject *Sender)
{
	TListBox *lp = DirHistListBox;
	if (lp->ItemIndex!=-1) {
		pos_ListBoxItem(lp);
		UnicodeString dnam = get_csv_item(lp->Items->Strings[lp->ItemIndex], 0);
		ShowPropertyDialog(dnam);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDirHistoryDlg::PropertyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = DirHistListBox->ItemIndex!=-1;
}
//---------------------------------------------------------------------------

