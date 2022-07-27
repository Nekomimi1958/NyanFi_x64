//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビュアー(外部ウィンドウ)									//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "usr_xd2tx.h"
#include "FindTxtDlg.h"
#include "InpCmds.h"
#include "EditHistDlg.h"
#include "ExpCsv.h"
#include "ExTViewer.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TExTxtViewer::TExTxtViewer(TComponent* Owner)
	: TForm(Owner)
{
	LastWidth  = 0;
	LineNo	   = 0;
	isClip	   = false;
	isXDoc2Txt = isRichText = isViewText = false;
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormCreate(TObject *Sender)
{
	LONG w_style = ::GetWindowLongPtr(Handle, GWL_EXSTYLE);
	w_style |= WS_EX_APPWINDOW;
	::SetWindowLongPtr(Handle, GWL_EXSTYLE, w_style);

	org_TvViewPanelWndProc	   = TxtViewPanel->WindowProc;
	TxtViewPanel->WindowProc   = TvViewPanelWndProc;
	org_TvScrlPanelWndProc	   = TxtScrollPanel->WindowProc;
	TxtScrollPanel->WindowProc = TvScrlPanelWndProc;
	org_TxtSttHdrWndProc	   = TxtSttHeader->WindowProc;
	TxtSttHeader->WindowProc   = TxtSttHdrWndProc;

	TextRulerBox->Enabled = false;

	TxtViewScrPanel = new UsrScrollPanel(TxtScrollPanel, TextScrollBar, USCRPNL_FLAG_TV|USCRPNL_FLAG_L_WP);

	ExViewer = new TTxtViewer(this, TextPaintBox, TextScrollBar, TxtViewScrPanel, TxtSttHeader, TextRulerBox, TextCurColPanel);
	ExViewer->isHtm2Txt   = TxtViewer->isHtm2Txt;	//内部ビュアーから反映
	ExViewer->isFixedLen  = TxtViewer->isFixedLen;
	ExViewer->ShowRuby	  = IniFile->ReadBoolGen(_T("ShowRuby"), true);
	ExViewer->TopIsHeader = IniFile->ReadBoolGen(_T("TopIsHeader"));
	ExViewer->isIncMigemo = IniFile->ReadBoolGen(_T("TvIncMigemo"));

	//ドロップターゲットを登録
	IDropTarget *pDropTarget = NULL;
	TDropTarget::CreateInstance(&pDropTarget);
	if (pDropTarget) {
		::RegisterDragDrop(this->Handle, pDropTarget);
		pDropTarget->Release();
		usr_SH->AddTargetList(this, TxtScrollPanel);
	}
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormShow(TObject *Sender)
{
	DlgInitialized = false;

	IniFile->LoadPosInfo(this);
	LastWidth = Width;

	TxtViewPanel->Color   = col_bgView;
	TxtScrollPanel->Color = col_bgView;

	//情報ヘッダ
	TxtSttHeader->Align = TxtSttIsBottom? alBottom : alTop;
	setup_StatusBar(TxtSttHeader, ViewHdrFont);

	set_UsrScrPanel(TxtViewScrPanel);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::WmFormShowed(TMessage &msg)
{
	//ファイル
	if (!FileName.IsEmpty()) {
		if (!OpenViewer(false, 0, LineNo)) msgbox_ERR(USTR_FileNotOpen);
	}
	else {
		cursor_HourGlass();
		TxtMainPanel->Visible	= false;
		TxtScrollPanel->Visible = false;
		TxtViewPanel->Font->Color = col_Teal;
		TxtViewPanel->Caption = "読込中...";	TxtViewPanel->Repaint();
		ExViewer->Clear();
		ExViewer->FileName = ExViewer->OrgName = EmptyStr;
		UnicodeString tmp;

		//クリップボード
		if (isClip) {
			set_FormTitle(this, _T("クリップボード - テキストビュアー"));
			std::unique_ptr<TStringList> vbuf(new TStringList());
			vbuf->Text = GetClipboardText();
			ExViewer->isClip = isClip;
			ExViewer->AssignText(vbuf.get());
			ExViewer->SttHdrInf = tmp.sprintf(_T("クリップボード  行数:%u"), ExViewer->MaxLine);
		}
		//ログ
		else {
			set_FormTitle(this, _T("タスクログ - テキストビュアー"));
			ExViewer->AssignText(LogBufList);
			ExViewer->SttHdrInf = tmp.sprintf(_T("タスクログ  行数:%s"), get_size_str_B(ExViewer->MaxLine, 0).c_str());
		}

		AdjustHdrWidth();
		ExViewer->SetSttInf();
		TxtViewPanel->Caption	= EmptyStr;
		TxtScrollPanel->Visible = true;
		TxtMainPanel->Visible	= true;
		cursor_Default();
	}

	DlgInitialized = true;
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormClose(TObject *Sender, TCloseAction &Action)
{
	usr_SH->DelTargetItem(this);
	IniFile->SavePosInfo(this);

	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormDestroy(TObject *Sender)
{
	::RevokeDragDrop(this->Handle);

	delete ExViewer;
	delete TxtViewScrPanel;
}

//---------------------------------------------------------------------------
//ロップ受け入れ
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::WmDropped(TMessage &msg)
{
	if (DroppedList->Count>0) {
		ExViewer->add_ViewHistory();
		if (!OpenViewer(DroppedList->Strings[0])) GlobalAbort();
	}
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormDeactivate(TObject *Sender)
{
	ExViewer->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormActivate(TObject *Sender)
{
	ExViewer->Repaint();
}

//---------------------------------------------------------------------
//サイズ変更終了
//---------------------------------------------------------------------
void __fastcall TExTxtViewer::WmExitSizeMove(TMessage &msg)
{
	if (ExViewer->isReady && LastWidth!=Width) {
		ExViewer->UpdateScr(0);
		LastWidth = Width;
	}
}

//---------------------------------------------------------------------------
//情報ヘッダのパネル幅調整
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::AdjustHdrWidth()
{
	TCanvas *cv = TxtSttHeader->Canvas;
	cv->Font->Assign(TxtSttHeader->Font);
	int s_12 = ScaledInt(12, this);
	TxtSttHeader->Panels->Items[1]->Width = cv->TextWidth(TxtSttHeader->Panels->Items[1]->Text) + s_12;
	TxtSttHeader->Panels->Items[2]->Width = cv->TextWidth(TxtSttHeader->Panels->Items[2]->Text) + s_12;
	TxtSttHeader->Panels->Items[3]->Width = cv->TextWidth(TxtSttHeader->Panels->Items[3]->Text) + s_12;
	UnicodeString tmp = "00000行 000桁 0000字選択 ";
	if (ExViewer->CsvCol>=0) tmp += " 00列";
	TxtSttHeader->Panels->Items[4]->Width = cv->TextWidth(tmp);

	TxtSttHeader->Panels->Items[0]->Width = ClientWidth
		- TxtSttHeader->Panels->Items[1]->Width
		- TxtSttHeader->Panels->Items[2]->Width
		- TxtSttHeader->Panels->Items[3]->Width
		- TxtSttHeader->Panels->Items[4]->Width;
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormResize(TObject *Sender)
{
	if (!DlgInitialized) return;

	AdjustHdrWidth();

	ExViewer->SetMetric(true);
	ExViewer->Repaint(true);
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextPaintBoxPaint(TObject *Sender)
{
	ExViewer->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextScrollBarChange(TObject *Sender)
{
	ExViewer->ScrollAdjust();
	TxtViewScrPanel->Repaint();
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextPaintBoxMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	CancelKeySeq();

	if (Button==mbLeft) ExViewer->onMouseDown(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextPaintBoxMouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
	if (Shift.Contains(ssLeft)) ExViewer->onMouseMove(X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextPaintBoxMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	ExViewer->onMouseUp();
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TextPaintBoxDblClick(TObject *Sender)
{
	if (ExViewer->ExtClicked) ExeCommandV(_T("Close")); else ExViewer->onDblClick();
}

//---------------------------------------------------------------------------
//情報ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::TxtSttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect)
{
	ExViewer->SttHeaderDrawPanel(StatusBar, Panel, Rect);
}

//---------------------------------------------------------------------------
bool __fastcall TExTxtViewer::OpenViewer(
	bool bin_mode,		//バイナリ	(default = false)
	int  code_page,		//コードページ (default = 0 : 判定)
	int  lno,			//行番号	(default = 0 : 履歴を参照)
	bool force_txt)		//強制的にテキストとして開く(bin_mode=false の場合 default = false)
{
	GlobalErrMsg = EmptyStr;

	try {
		ExViewer->Clear();

		if (!file_exists(FileName)) Abort();
		UnicodeString fext = get_extension(FileName);
		Caption = UnicodeString().sprintf(_T("%s - テキストビュアー"), yen_to_delimiter(OrgName).c_str());

		int cpag = 0;

		isXDoc2Txt = bin_mode? false : (UseXd2tx && xd2tx_TestExt(fext));
		isRichText = bin_mode? false : test_FileExt(fext, ".rtf.wri");
		isViewText = bin_mode? false : (force_txt? true : is_TextFile(FileName, &cpag, NULL, &ExViewer->HasBOM));

		TxtMainPanel->Visible	= false;
		TxtScrollPanel->Visible = false;

		cursor_HourGlass();

		ExViewer->FileName = FileName;
		ExViewer->OrgName  = OrgName;

		TxtViewPanel->Font->Color = col_Teal;
		TxtViewPanel->Caption = "読込中...";	TxtViewPanel->Repaint();

		UnicodeString inf_str;
		inf_str.sprintf(_T("%s  %s"), ExtractFileName(FileName).c_str(), get_FileInfStr(FileName).c_str());

		//------------------------------
		//xdoc2txt
		//------------------------------
		if (isXDoc2Txt) {
			std::unique_ptr<TStringList> vbuf(new TStringList());
			if (!xd2tx_Extract(FileName, vbuf.get())) Abort();
			ExViewer->AssignText(vbuf.get(), lno);
			ExViewer->LineBreakStr = EmptyStr;
		}
		//------------------------------
		//リッチテキスト
		//------------------------------
		else if (isRichText) {
			TempRichEdit->Lines->LoadFromFile(FileName);
			ExViewer->AssignText(TempRichEdit->Lines, lno);
		}
		//------------------------------
		//テキスト表示
		//------------------------------
		else if (isViewText) {
			//コード判定して読込(サイズ制限)
			std::unique_ptr<TStringList> vbuf(new TStringList());
			if (code_page==0) code_page = cpag;
			load_text_ex(FileName, vbuf.get(), code_page, ViewTxtLimitSize, force_txt, &ExViewer->LineBreakStr);
			if (!GlobalErrMsg.IsEmpty()) Abort();
			ExViewer->AssignText(vbuf.get(), lno);
			//行数
			inf_str.cat_sprintf(_T("  %s:%s"),
				(test_HtmlExt(fext)? _T("ソース行数") : _T("行数")),
				get_size_str_B(vbuf->Count, 0).c_str());
			if (ExViewer->isLimited) inf_str += "(部分)";
		}
		//------------------------------
		//バイナリ・ダンプ表示
		//------------------------------
		else {
			if (!ExViewer->AssignBin()) Abort();
			if (ExViewer->isLimited) inf_str += "    (部分)";
		}

		AdjustHdrWidth();

		ExViewer->SttHdrInf 	= inf_str;
		ExViewer->SetSttInf();
		TxtSttHeader->Hint		= ExViewer->FileName;
		TxtViewPanel->Caption	= EmptyStr;
		TxtScrollPanel->Visible = true;
		TxtMainPanel->Visible	= true;
		cursor_Default();

		if (!ExViewer->JsonErrMsg.IsEmpty()) msgbox_ERR(ExViewer->JsonErrMsg);

		return true;
	}
	catch (...) {
		if (GlobalErrMsg.IsEmpty()) GlobalErrMsg = LoadUsrMsg(USTR_FileNotOpen);
		TxtViewPanel->Caption	= EmptyStr;
		cursor_Default();
		return false;
	}
}
//---------------------------------------------------------------------------
bool __fastcall TExTxtViewer::OpenViewer(
	UnicodeString fnam,	//ファイル名
	bool bin_mode,		//バイナリ	(default = false)
	int  code_page,		//コードページ (default = 0 : 判定)
	int  lno)			//行番号	(default = 0 : 履歴を参照)
{
	FileName = OrgName = fnam;
	return OpenViewer(bin_mode, code_page, lno);
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::GlobalAbort()
{
	throw EAbort(GlobalErrMsg);
}

//---------------------------------------------------------------------------
//2ストローク操作の処理
//---------------------------------------------------------------------------
UnicodeString __fastcall TExTxtViewer::TwoStrokeSeq(WORD &Key, TShiftState Shift)
{
	UnicodeString key_str = get_KeyStr(Key);	if (key_str.IsEmpty()) return EmptyStr;
	key_str = get_ShiftStr(Shift) + key_str;

	//2ストローク目
	if (Wait2ndKey) {
		Wait2ndKey = false;
		key_str.sprintf(_T("%s~%s"), FirstKey.c_str(), get_KeyStr(Key).c_str());
		CancelKeySeq();
		if (Key_to_CmdV(key_str).IsEmpty()) Key = 0;
	}
	//1ストローク目
	else if (is_FirstKey("V", key_str)) {
		FirstKey   = key_str;
		Wait2ndKey = true;
		ExViewer->SetSttInf(!FirstKey.IsEmpty()? "2ストロークキー: " + FirstKey : EmptyStr);
		Key = 0;
	}

	return key_str;
}
//---------------------------------------------------------------------------
//2ストローク操作を中断
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CancelKeySeq()
{
	FirstKey   = EmptyStr;
	Wait2ndKey = false;
	ExViewer->SetSttInf();
}

//---------------------------------------------------------------------------
//ダイレクトタグジャンプ
//！失敗したら EAbort 例外
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::DirectTagJumpCore(
	bool is_edit,			//true = 編集/ false = 閲覧	(default = false)
	UnicodeString prm)		//単語指定					(default = EmptyStr)
{
	UnicodeString tnam = prm.IsEmpty()? ExViewer->GetCurWord() : prm;
	UnicodeString rnam = ExtractFilePath(ExViewer->FileName);
	std::unique_ptr<TStringList> j_lst(new TStringList());
	if (get_FileNameByTag(tnam, rnam, j_lst.get())==0) GlobalAbort();

	UnicodeString fnam;
	if (j_lst->Count==1) {
		fnam = j_lst->Strings[0];
	}
	else {
		EditHistoryDlg->isTags  = true;
		EditHistoryDlg->isView  = !is_edit;
		EditHistoryDlg->TagJumpList->Assign(j_lst.get());
		EditHistoryDlg->TagName = tnam;
		EditHistoryDlg->TagPath = rnam;
		int res = EditHistoryDlg->ShowModal();
		if (res!=mrOk && res!=mrClose) SkipAbort();
		fnam = EditHistoryDlg->TagJumpInf;
	}

	if (!fnam.IsEmpty()) {
		int lno = 1;
		if (!divide_FileName_LineNo(fnam, lno, rnam)) throw EAbort(GlobalErrMsg);
		//編集
		if (is_edit) {
			if (!open_by_TextEditor(fnam, lno)) GlobalAbort();
		}
		//閲覧
		else {
			//閲覧中ファイル内
			if (SameText(ExViewer->FileName, fnam)) {
				ExViewer->ToLine(lno);
			}
			//その他のファイル
			else {
				ExViewer->add_ViewHistory();
				if (test_HtmlExt(get_extension(fnam)) && lno>1) ExViewer->isHtm2Txt = false;
				if (!OpenViewer(fnam, false, 0, lno)) GlobalAbort();
			}
		}
	}
}

//---------------------------------------------------------------------------
//コマンド処理
//---------------------------------------------------------------------------
bool __fastcall TExTxtViewer::ExeCommandV(UnicodeString cmd, UnicodeString prm)
{
	if (cmd.IsEmpty()) return false;

	if (prm.IsEmpty()) {
		prm = exclude_quot(get_PrmStr(cmd));
		cmd = get_CmdStr(cmd);
	}

	AddCmdHistory(cmd, ActionParam, "V",
		!ExViewer->FileName.IsEmpty()? ExViewer->FileName :
					 ExViewer->isClip? UnicodeString("<クリップボード>")
					 				 : UnicodeString("<タスクログ>"));

	ActionParam = extract_ExeParam(prm, &ActionDesc);
	ActionOk	= true;
	Application->ProcessMessages();

	bool res;
	InhCmdHistory = true;
	try {
		bool handled = ExViewer->ExeCommand(cmd.c_str(), ActionParam);
		if (handled && !GlobalErrMsg.IsEmpty()) GlobalAbort();
		if (handled) TextAbort(_T("HANDLED"));

		bool req_tEdit = false;
		bool req_bEdit = false;
		bool req_close = false;
		handled = true;

		//クリップボードにコピー
		if (USAME_TI(cmd, "ClipCopy")) {
			ExViewer->ClipCopy(USAME_TI(ActionParam, "AD"));
		}
		//前/次のウィンドウを表示
		else if (contained_wd_i(_T("PrevFile|NextFile"), cmd)) {
			std::unique_ptr<TStringList> lst(new TStringList());
			get_ExViewerList(lst.get());

			int idx = -1;
			for (int i=0; i<lst->Count; i++) if ((TForm *)lst->Objects[i]==this) idx = i;

			if (lst->Count>1 && idx!=-1) {
				if (USAME_TI(cmd, "PrevFile")) {
					idx--; if (idx<0) idx = lst->Count -1;
				}
				else {
					idx++; if (idx>=lst->Count) idx = 0;
				}
				((TForm *)lst->Objects[idx])->SetFocus();
			}
		}
		//ファイルの再読み込み
		else if (USAME_TI(cmd, "ReloadFile")) {
			if (!OpenViewer(ExViewer->isBinary)) GlobalAbort();
		}
		//テキスト/バイナリ表示の切り換え
		else if (USAME_TI(cmd, "ChangeViewMode")) {
			//バイナリ表示
			if (isXDoc2Txt || isRichText || isViewText) {
				if (!OpenViewer(true)) GlobalAbort();
			}
			//テキスト表示(強制)
			else {
				if (!OpenViewer(false, 0, 0, true)) GlobalAbort();
			}
		}
		//文字コード変更
		else if (USAME_TI(cmd, "ChangeCodePage")) {
			if (ExViewer->isXDoc2Txt || ExViewer->isBinary) UserAbort(USTR_InvalidCmd);
			int code_page = ExViewer->change_CodePage(ActionParam);
			if (code_page==0) UserAbort(USTR_IllegalParam);
			if (!OpenViewer(false, code_page, 0, true)) GlobalAbort();
		}
		//編集
		else if (USAME_TI(cmd, "FileEdit")) {
			if (isXDoc2Txt || !ActionParam.IsEmpty()) UserAbort(USTR_OpeNotSuported);
			req_tEdit = true;
			req_close = true;
		}
		//バイナリ編集
		else if (USAME_TI(cmd, "BinaryEdit")) {
			if (!file_exists(get_actual_path(BinaryEditor))) UserAbort(USTR_AppNotFound);
			req_bEdit = true;
			req_close = true;
		}
		//ダンプリストをファイルに保存
		else if (USAME_TI(cmd, "SaveDump"))	{
			SaveDumpAction->Execute();
		}
		//タグジャンプ
		else if (contained_wd_i(_T("TagJump|TagView"), cmd)) {
			if (ExViewer->isBinary) UserAbort(USTR_InvalidCmd);

			int lno = 1, pos = 0;
			UnicodeString fnam = ExViewer->get_SelText();
			if (fnam.IsEmpty()) {
				fnam = ExViewer->get_CurLine(true);
				pos  = ExViewer->CurPos.x;
			}

			bool is_edit = ContainsText(cmd, "Jump");
			if (divide_FileName_LineNo(fnam, lno, ExViewer->FileName, pos)) {
				if (is_edit) {
					if (!open_by_TextEditor(fnam, lno)) GlobalAbort();
				}
				else {
					ExViewer->add_ViewHistory();
					if (test_HtmlExt(get_extension(fnam)) && lno>1) ExViewer->isHtm2Txt = false;
					if (!OpenViewer(fnam, false, 0, lno)) GlobalAbort();
				}
			}
			else if (USAME_TI(ActionParam, "DJ")) {
				DirectTagJumpCore(is_edit);
			}
			else GlobalAbort();
		}
		//ダイレクトタグジャンプ
		else if (contained_wd_i(_T("TagJumpDirect|TagViewDirect"), cmd)) {
			DirectTagJumpCore(ContainsText(cmd, "Jump"), ActionParam);
		}
		//ビュアーの履歴を戻る
		else if (USAME_TI(cmd, "BackViewHist")) {
			if (TextViewHistory->Count==0) Abort();
			TStringDynArray itm_buf = get_csv_array(TextViewHistory->Strings[0], 2, true);
			TextViewHistory->Delete(0);
			if (!OpenViewer(itm_buf[0], false, 0, itm_buf[1].ToIntDef(0))) GlobalAbort();
		}
		//ファイル名主部が同じ次のファイルに切り替え
		else if (USAME_TI(cmd, "SwitchSameName")) {
			UnicodeString fnam = get_NextSameName(ExViewer->FileName, true);
			if (fnam.IsEmpty()) SysErrAbort(ERROR_FILE_NOT_FOUND);
			ExViewer->add_ViewHistory();
			if (!OpenViewer(fnam)) GlobalAbort();
		}
		//ソース／ヘッダの切り換え
		else if (USAME_TI(cmd, "SwitchSrcHdr")) {
			UnicodeString fnam = get_SrcHdrName(ExViewer->FileName);
			if (fnam.IsEmpty()) SysErrAbort(ERROR_FILE_NOT_FOUND);
			ExViewer->add_ViewHistory();
			if (!OpenViewer(fnam)) GlobalAbort();
		}
		//文字列検索
		else if (USAME_TI(cmd, "FindText")) {
			FindTextAction->Execute();
		}
		//インクリメンタルサーチへ
		else if (USAME_TI(cmd, "IncSearch")) {
			if (USAME_TI(ActionParam, "MM") && usr_Migemo->DictReady)
				ExViewer->isIncMigemo = true;
			else if (USAME_TI(ActionParam, "NM"))
				ExViewer->isIncMigemo = false;
			ExViewer->IncSeaWord = EmptyStr;
			ExViewer->isIncSea   = true;
			ExViewer->SetSttInf();
		}
		//テキストビュアーを閉じる
		else if (USAME_TI(cmd, "Close")) {
			req_close = true;
			if (USAME_TI(ActionParam, "AL")) close_all_ExViewer(this);
		}
		//メインフォームでのコマンド実行
		else if (contained_wd_i(_T("AppList|Calculator|OptionDLg|RegExChecker"), cmd)) {
			UnicodeString cmds = cmd;
			if (!ActionParam.IsEmpty()) cmds.cat_sprintf(_T("_%s"), ActionParam.c_str());
			COPYDATASTRUCT cd;
			cd.dwData = CPYDTID_EXECMDS;
			cd.cbData = sizeof(_TCHAR) * (cmds.Length() + 1);
			cd.lpData = cmds.c_str();
			::SendMessage(Application->MainForm->Handle, WM_COPYDATA, 0, (LPARAM)&cd);
		}
		//未対応コマンド
		else {
			msgbox_WARN(LoadUsrMsg(USTR_OpeNotSuported, UnicodeString().sprintf(_T("[%s] "), cmd.c_str())));
			handled = false;
		}

		//------------------
		//閉じる
		//------------------
		if (req_close) {
			ExViewer->add_ViewHistory();
			ExViewer->Clear();

			//テキスト編集
			if (req_tEdit) {
				if (!open_by_TextEditor(ExViewer->FileName, ExViewer->get_CurLineNo())) GlobalAbort();
			}
			//バイナリ編集
			else if (req_bEdit) {
				if (!Execute_ex(get_actual_path(BinaryEditor), add_quot_if_spc(ExViewer->FileName))) UserAbort(USTR_FaildExec);
			}

			Close();
		}
		res = handled;
	}
	catch (EAbort &e) {
		UnicodeString msg = e.Message;
		if (!contained_wd_i(_T("SKIP|HANDLED"), msg)) {
			if (StartsText("Abort", msg)) msg = EmptyStr;
			ActionErrMsg = msg;
			ActionOk	 = false;
		}
		res = true;
	}
	InhCmdHistory = false;

	return res;
}
//---------------------------------------------------------------------------
bool __fastcall TExTxtViewer::ExeCommandV(const _TCHAR *cmd)
{
	return ExeCommandV(UnicodeString(cmd));
}

//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
	TPoint &MousePos, bool &Handled)
{
	ClearNopStt();

	int kmd = 0;
	if (Shift.Contains(ssCtrl))  kmd |= 1;
	if (Shift.Contains(ssShift)) kmd |= 2;

	UnicodeString cmd_V = WheelCmdV[kmd];
	cmd_V = Trim((WheelDelta>0)? get_tkn(cmd_V, '/') : get_tkn_r(cmd_V, '/'));

	CancelKeySeq();
	Handled = ExeCommandV(cmd_V);
}

//---------------------------------------------------------------------------
//文字列検索
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FindTextActionExecute(TObject *Sender)
{
	FindTextDlg->ExTViewer = this;
	FindTextDlg->fromTV    = false;
	FindTextDlg->ShowModal();
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CopyActionExecute(TObject *Sender)
{
	ExViewer->ClipCopy();
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::AddCopyActionExecute(TObject *Sender)
{
	ExViewer->ClipCopy(true);
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CopyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = ExViewer->SelStart!=ExViewer->SelEnd;
}
//---------------------------------------------------------------------------
//すべて選択
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SelectAllActionExecute(TObject *Sender)
{
	ExViewer->SelectAll();
}

//---------------------------------------------------------------------------
//Web で検索
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::WebSearchActionExecute(TObject *Sender)
{
	ExeCommandV(_T("WebSearch"));
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::WebSearchActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !WebSeaUrl.IsEmpty();
	ap->Caption = get_WebSeaCaption(ExViewer->get_SelText());
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::PopWebSearchItemMeasureItem(TObject *Sender, TCanvas *ACanvas,
	int &Width, int &Height)
{
	Width = ACanvas->TextWidth(PopWebSearchItem->Caption) + ScaledInt(32, this);
}

//---------------------------------------------------------------------------
//ソート
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SortAscActionExecute(TObject *Sender)
{
	ExViewer->ExeCommand(_T("Sort_AO"));
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SortDesActionExecute(TObject *Sender)
{
	ExViewer->ExeCommand(_T("Sort_DO"));
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::OrgOrderActionExecute(TObject *Sender)
{
	ExViewer->ExeCommand(_T("Sort"));
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SortActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = ExViewer->isText;
	ap->Enabled = ap->Visible && ExViewer->MaxLine>1;
}

//---------------------------------------------------------------------------
//コードページ変更
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::PopCodePageActionExecute(TObject *Sender)
{
	//ダミー
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CodePageActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Visible = isViewText && !isRichText && !isXDoc2Txt;
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CodePageActionExecute(TObject *Sender)
{
	int code_page = ((TAction*)Sender)->Tag;
	ExViewer->saveDef_CodePage(code_page);
	OpenViewer(false, code_page, 0, true);
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::CP_xxx_ActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Checked = ExViewer->TxtBufList->Encoding->CodePage==(unsigned int)ap->Tag;
}

//---------------------------------------------------------------------------
//CSV/TSVエクスポート
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::ExportCsvActionExecute(TObject *Sender)
{
	if (!ExpCsvDlg) ExpCsvDlg = new TExpCsvDlg(this);	//初回に動的作成
	ExpCsvDlg->Viewer = ExViewer;
	ExpCsvDlg->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::ExportCsvActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = ExViewer->isText;
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//ダンプリストをファイルに保存
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SaveDumpActionExecute(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, _T("ダンプリスト"));
	UserModule->SaveTxtDlg->FileName	  = ChangeFileExt(ExViewer->FileName, ".txt");
	UserModule->SaveTxtDlg->InitialDir	  = ExtractFileDir(ExViewer->FileName);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		ExViewer->GetDumpList(fbuf.get());

		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex)) msgbox_ERR(USTR_FaildSave);
	}
}
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::SaveDumpActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = ExViewer->isBinary;
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TExTxtViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	try {
		bool hadled = true;
		//インクリメンタルサーチ
		if (ExViewer->isIncSea) {
			UnicodeString KeyStr = get_KeyStr(Key, Shift);		if (KeyStr.IsEmpty()) return;
			ExViewer->IncSearch(KeyStr);
		}
		//通常
		else {
			UnicodeString KeyStr = TwoStrokeSeq(Key, Shift);	if (KeyStr.IsEmpty()) return;
			UnicodeString CmdStr = Key_to_CmdV(KeyStr);
			if (CmdStr.IsEmpty()) CmdStr = ExViewer->GetStdKeyCommand(KeyStr);
			CancelHelp	= !CmdStr.IsEmpty() && EndsStr("F1", KeyStr);
			ActionParam = EmptyStr;

			//コマンド処理
			if (ExeCommandV(CmdStr)) {
				if (!ActionOk) throw EAbort(ActionErrMsg);
				ClearKeyBuff(true);
			}
			//右クリックメニュー
			else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
				show_PopupMenu(ViewPopupMenu, TextPaintBox);
			}
			//閉じる
			else if (equal_ESC(KeyStr) || equal_ENTER(KeyStr)) {
				ExeCommandV(_T("Close"));
			}
			//その他
			else {
				hadled = SpecialKeyProc(this, Key, Shift);
				if (hadled && LastWidth!=Width) {
					ExViewer->UpdateScr(ExViewer->get_CurLineNo());
					LastWidth = Width;
				}
			}
		}

		if (hadled) Key = 0;
	}
	catch (EAbort &e) {
		msgbox_ERR(e.Message);
	}
}
//---------------------------------------------------------------------------

