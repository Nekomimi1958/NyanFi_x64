//----------------------------------------------------------------------//
// NyanFi																//
//  関数／ユーザ定義文字列／マーク行一覧								//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <Vcl.Clipbrd.hpp>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "TxtViewer.h"
#include "FuncDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFuncListDlg *FuncListDlg;

//---------------------------------------------------------------------------
__fastcall TFuncListDlg::TFuncListDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	ListScrPanel = new UsrScrollPanel(ListPanel, FuncListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	UserDefComboBox->Tag = CBTAG_HISTORY;
	FuncListBox->Tag	 = LBTAG_OPT_ZOOM | LBTAG_OPT_LNNO | LBTAG_OPT_LCPY;

	FunctionList = new TStringList();
	UserDefList  = new TStringList();
	MarkLineList = new TStringList();

	ListMode = 0;
	LineNo	 = -1;
	ToFilter = false;
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	DlgInitialized = false;

	IniFile->LoadPosInfo(this, DialogCenter);

	set_MigemoCheckBox(MigemoCheckBox, _T("FuncListMigemo"));

	NameOnlyCheckBox->Checked = IniFile->ReadBoolGen(_T("FuncListNameOnly"));
	LinkCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FuncListLinkLine"));
	RegExCheckBox->Checked	  = IniFile->ReadBoolGen(_T("FuncListRegEx"));

	IniFile->LoadComboBoxItems(UserDefComboBox, RegExCheckBox->Checked? _T("UserPtnHistory") : _T("UserDefHistory"));
	UserDefComboBox->Text = !UserDefStr.IsEmpty()? UserDefStr : IniFile->ReadStrGen(_T("FuncListUserStr"));

	TListBox *lp = FuncListBox;
	lp->Clear();
	ListPanel->Color = lp->Color;
	set_StdListBox(lp);
	set_UsrScrPanel(ListScrPanel);

	SetDarkWinTheme(this);
	FilterEdit->Color = get_WinColor(!ToFilter);

	if (!UnInitializing) InitializeList();

	(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)lp)->SetFocus();
	FilterEdit->Text = EmptyStr;

	ReqEdit = false;
	DlgInitialized = true;
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserDefStr = EmptyStr;

	IniFile->SavePosInfo(this);

	IniFile->WriteBoolGen(_T("FuncListMigemo"),		MigemoCheckBox);
	IniFile->WriteBoolGen(_T("FuncListNameOnly"),	NameOnlyCheckBox);
	IniFile->WriteBoolGen(_T("FuncListLinkLine"),	LinkCheckBox);
	IniFile->WriteStrGen( _T("FuncListUserStr"),	UserDefComboBox->Text);
	IniFile->WriteBoolGen(_T("FuncListRegEx"),		RegExCheckBox);

	IniFile->SaveComboBoxItems(UserDefComboBox, RegExCheckBox->Checked? _T("UserPtnHistory") : _T("UserDefHistory"));
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FormDestroy(TObject *Sender)
{
	delete FunctionList;
	delete UserDefList;
	delete MarkLineList;
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::ClearList()
{
	FunctionList->Clear();
	UserDefList->Clear();
	MarkLineList->Clear();
	FuncListBox->Clear();
}

//---------------------------------------------------------------------------
//一覧の初期化
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::InitializeList(int mode)
{
	if (mode==ListMode) return;
	if (mode!=-1) ListMode = mode;

	cursor_HourGlass();

	NamePtn = EmptyStr;
	is_DFM	= false;

	LineNo	= LinkCheckBox->Checked? TxtViewer->get_CurLineNo() : -1;

	UnicodeString cap_str;

	//ユーザ定義一覧
	if (ListMode==1) {
		cap_str = "ユーザ定義文字列一覧";
		UserDefPanel->Visible = true;
		if (UserDefComboBox->Text.IsEmpty()) {
			UserDefList->Clear();
			UpdateList();
		}
		else if (UserDefList->Count==0) {
			UpdUserDefAction->Execute();
		}
		else {
			UpdateList();
		}
	}
	//マーク行一覧
	else if (ListMode==2) {
		cap_str = "マーク行一覧";
		UserDefPanel->Visible = false;

		//一覧を作成
		MarkLineList->Clear();
		TStringDynArray m_lst = split_strings_semicolon(TxtViewer->MarkListStr);
		for (int i=0; i<m_lst.Length; i++) {
			int lno = m_lst[i].ToIntDef(0);  if (lno==0) continue;
			for (int j=0; j<TxtViewer->MaxDispLine; j++) {
				line_rec *lp = TxtViewer->get_LineRec(j);
				if (lp->LineIdx==0 && lno==lp->LineNo) {
					MarkLineList->AddObject(TxtViewer->DispLines->Strings[j], (TObject*)(NativeInt)lno);
					break;
				}
			}
		}
		UpdateList();
	}
	//関数一覧	(ListMode==0)
	else {
		cap_str = "関数一覧";
		UserDefPanel->Visible = false;

		UnicodeString fext = get_extension(TxtViewer->FileName);
		is_DFM = test_FileExt(fext, _T(".dfm"));

		UnicodeString fnc_ptn;

		//ユーザ定義から取得
		if (UserHighlight->GetSection(TxtViewer->FileName,
			TxtViewer->isClip, TxtViewer->isLog, TxtViewer->isHtm2Txt))
		{
			fnc_ptn = UserHighlight->ReadKeyStr(_T("FunctionPtn"));
			if (!fnc_ptn.IsEmpty()) NamePtn = UserHighlight->ReadKeyStr(_T("FuncNamePtn"));
		}
		//デフォルト
		else {
			fnc_ptn = GetDefFunctionPtn(fext, NamePtn, TxtViewer->isHtm2Txt);
		}

		//関数以外
		if (fnc_ptn.IsEmpty()) {
			if (test_FileExt(fext, _T(".bat.cmd.qbt"))) {
				fnc_ptn = "^:[^:]+";
				cap_str = "ラベル一覧";
			}
			else if (is_DFM) {
				fnc_ptn = "^\\s*object\\s";
				NamePtn = "\\s\\w+:";
				cap_str = "オブジェクト一覧";
			}
			else if (TxtViewer->isIniFmt) {
				fnc_ptn = "^\\[.+\\]";
				cap_str = "セクション一覧";
			}
			else if (!TxtViewer->HeadlinePtn.IsEmpty()) {
				fnc_ptn = TxtViewer->HeadlinePtn;
				cap_str = test_FileExt(fext, _T(".eml"))? "件名一覧" : TxtViewer->isLog? "タスク一覧" : "見出し一覧";
			}
		}

		if (!chk_RegExPtn(fnc_ptn)) fnc_ptn = EmptyStr;
		if (!chk_RegExPtn(NamePtn)) NamePtn = EmptyStr;

		//一覧を作成
		if (FunctionList->Count==0 && !fnc_ptn.IsEmpty()) {
			bool has_par = (ListMode==0 && ContainsStr(fnc_ptn, "\\("));
			bool non_tab = StartsStr('^', fnc_ptn) && !StartsStr("^\\s*", fnc_ptn);
			TRegExOptions opt; opt << roIgnoreCase;
			for (int i=0; i<TxtViewer->MaxDispLine; i++) {
				line_rec *rp = TxtViewer->get_LineRec(i);  if (rp->LineIdx>0) continue;
				UnicodeString lbuf = TrimRight(TxtViewer->get_DispLine(i));
				if (non_tab && (StartsStr('\t', lbuf) || StartsStr(' ', lbuf))) continue;
				if (has_par && (!ContainsStr(lbuf, "(") || ContainsStr(lbuf, "="))) continue;
				if (TRegEx::IsMatch(lbuf, fnc_ptn, opt)) {
					if (TxtViewer->isAozora) lbuf = replace_regex(lbuf, _T("［＃「.*?」は((中|大)見出し)］"), null_TCHAR);
					FunctionList->AddObject(lbuf, (TObject*)(NativeInt)rp->LineNo);
				}
			}
		}
		UpdateList();
	}

	Opt1Panel->Visible = (ListMode==0 && !NamePtn.IsEmpty());	//名前
	Caption = cap_str;

	cursor_Default();
}

//---------------------------------------------------------------------------
//一覧の表示更新
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::UpdateList(
	bool link)	//ビュアー行を項目に連動 (default = false)
{
	std::unique_ptr<TStringList> flst(new TStringList());
	flst->Assign((ListMode==0)? FunctionList : ((ListMode==1)? UserDefList : MarkLineList));

	//フィルタ
	UnicodeString ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Checked, FilterEdit->Text);
	if (!ptn.IsEmpty()) {
		TRegExOptions opt; opt << roIgnoreCase;
		int i=0;
		while (i<flst->Count) {
			UnicodeString lbuf = flst->Strings[i];
			if (NameOnlyCheckBox->Checked && ListMode==0 && !NamePtn.IsEmpty()) {
				TRegExOptions opt; opt << roIgnoreCase;
				TMatch mt = TRegEx::Match(lbuf, NamePtn, opt);
				if (mt.Success) {
					lbuf = mt.Value;
					remove_end_s(lbuf, '(');
					remove_end_s(lbuf, ':');
				}
			}
			if (!TRegEx::IsMatch(lbuf, ptn, opt)) flst->Delete(i); else i++;
		}
	}

	TListBox *lp = FuncListBox;
	set_RedrawOff(lp);
	{
		lp->Items->Assign(flst.get());
		if (lp->Count>0) {
			int idx = 0;
			if (LineNo!=-1) {
				for (int i=0; i<lp->Count; i++) {
					int lno = (int)lp->Items->Objects[i];
					if (LineNo < lno) break;
					if (LineNo >= lno) idx = i;
				}
			}
			lp->ItemIndex = idx;
		}
	}
	set_RedrawOn(lp);
	ListScrPanel->UpdateKnob();

	if (link) ToLine();
}

//---------------------------------------------------------------------------
//リスト項目の行へ移動
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::ToLine()
{
	TListBox *lp = FuncListBox;
	int idx = lp->ItemIndex;
	if (idx>=0 && idx<lp->Count) {
		LineNo = (int)lp->Items->Objects[idx];
		TxtViewer->ToLine(LineNo);
	}
}

//---------------------------------------------------------------------------
//項目の描画
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FuncListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = col_bgList;
	TRect rc = Rect;
	cv->FillRect(rc);

	UnicodeString lbuf = lp->Items->Strings[Index];
	int lno = (int)lp->Items->Objects[Index];

	int xp = Rect.Left;
	int yp = Rect.Top + get_TopMargin2(cv);

	//マーク
	if (ListMode!=2) {
		rc.Right = rc.Left + ScaledIntX(8);
		cv->Brush->Color = col_bgLineNo;  cv->FillRect(rc);
		if (TxtViewer->IsMarked(lno)) {
			InflateRect(rc, ScaledIntX(-1), ScaledIntX(-4));
			cv->Brush->Color = col_Mark;  cv->FillRect(rc);
		}
		xp += ScaledIntX(8);
	}

	//行番号
	rc = Rect;  rc.Left = xp;
	cv->Font->Assign(lp->Font);
	LineNoOut(cv, rc, lno);
	xp = rc.Left + ScaledIntX(4);

	//内容
	cv->Brush->Color = col_bgList;
	cv->Font->Color = col_fgList;
	if (ListMode==0) {
		if (TxtViewer->isIniFmt) {
			cv->Font->Color = col_Headline;
			cv->TextOut(xp, yp, lbuf);
		}
		else if (!NamePtn.IsEmpty()) {
			//行頭の空白を取得
			UnicodeString pre_str =
				is_DFM? (StringOfChar(_T(' '), lbuf.Length() - TrimLeft(lbuf).Length())) : EmptyStr;

			//関数名を強調
			std::unique_ptr<TStringList> elist(new TStringList());
			TRegExOptions opt; opt << roIgnoreCase;
			TMatch mt = TRegEx::Match(lbuf, NamePtn, opt);
			if (mt.Success) {
				UnicodeString wd = Trim(mt.Value);
				if (ends_tchs(_T("{([:"), wd)) delete_end(wd);
				elist->Add(wd);
				if (NameOnlyCheckBox->Checked) lbuf = wd;
				lbuf.Insert(pre_str, 1);
			}

			EmphasisTextOut(lbuf, elist.get(), cv, xp, yp, false, true, col_Headline, cv->Brush->Color);
		}
		else {
			cv->TextOut(xp, yp, lbuf);
		}
	}
	else {
		cv->TextOut(xp, yp, lbuf);
	}

	//カーソル
	draw_ListCursor(lp, Rect, Index, State);
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FuncListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TListBox *lp = FuncListBox;
	int idx = lp->ItemIndex;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	try {
		if (equal_ENTER(KeyStr)) {
			CloseListAction->Execute();
		}
		else if	(equal_ESC(KeyStr) || USAME_TI(cmd_F, "ReturnList")) {
			ModalResult = mrCancel;
		}
		else if (USAME_TI(cmd_F, "FileEdit") || USAME_TI(cmd_V, "FileEdit")) {
			ReqEditAction->Execute();
		}
		else if (USAME_TI(cmd_V, "FunctionList")) {
			InitializeList(0);	//関数一覧
		}
		else if (USAME_TI(cmd_V, "UserDefList")) {
			InitializeList(1);	//ユーザ定義文字列一覧
		}
		else if (USAME_TI(cmd_V, "MarkList")) {
			InitializeList(2);	//マーク行一覧
		}
		//マーク
		else if (USAME_TI(cmd_V, "Mark")) {
			if (ListMode!=2 && idx>=0 && idx<lp->Count) {
				TxtViewer->MarkLine((int)lp->Items->Objects[idx]);
				lp->Repaint();
			}
		}
		//その他の移動など
		else if (StartsText("IncSearch", cmd_F)) {
			FilterEdit->SetFocus();
		}
		else if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V)) {
			;
		}
		else if (contained_wd_i(_T("S|Ctrl+S"), KeyStr) && ListMode==1) {
			UserDefComboBox->SetFocus();
		}
		else if (contained_wd_i(KeyStr_Filter, KeyStr)) {
			FilterEdit->SetFocus();
		}
		else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
			show_PopupMenu(lp);
		}

		//連動
		if (LinkCheckBox->Checked && idx!=-1 && idx!=lp->ItemIndex) ToLine();
	}
	catch (EAbort &e) {
		beep_Warn();
	}

	if (!is_DialogKey(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FuncListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//インクリメンタルサーチを回避
	if (_istalnum(Key) || Key==VK_SPACE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FuncListBoxDblClick(TObject *Sender)
{
	CloseListAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FuncListBoxClick(TObject *Sender)
{
	if (LinkCheckBox->Checked) ToLine();
}

//---------------------------------------------------------------------------
//フィルタによる一覧の更新
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FilterEditChange(TObject *Sender)
{
	UpdateList(LinkCheckBox->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = get_WinColor();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}

//---------------------------------------------------------------------------
//フィルタ欄でのキー操作
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (KeyStr.IsEmpty()) return;

	TListBox *lp = FuncListBox;
	int idx = lp->ItemIndex;

	if (contained_wd_i(KeysStr_ToList, KeyStr))	lp->SetFocus();
	else if (MovListBoxFromFilter(lp, KeyStr))	;
	else if (SameText(KeyStr, KeyStr_Migemo))	MigemoCheckBox->Checked = !MigemoCheckBox->Checked;
	else if (equal_ENTER(KeyStr))				CloseListAction->Execute();
	else if	(equal_ESC(KeyStr))					ModalResult = mrCancel;
	else return;

	lp->Repaint();
	if (LinkCheckBox->Checked && idx!=lp->ItemIndex) ToLine();

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::RegExActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	//検索履歴の入れ換え
	if (DlgInitialized) {
		change_ComboBoxHistory(UserDefComboBox, _T("UserDefHistory"), _T("UserPtnHistory"), ap->Checked);
	}
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::UserDefComboBoxEnter(TObject *Sender)
{
	UpdateActions();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::UserDefComboBoxClick(TObject *Sender)
{
	UpdUserDefAction->Execute();
}

//---------------------------------------------------------------------------
//ユーザ定義文字列一覧の更新
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::UpdUserDefActionExecute(TObject *Sender)
{
	cursor_HourGlass();
	UserDefList->Clear();
	UnicodeString mat_ptn = UserDefComboBox->Text;
	if (!RegExCheckBox->Checked) mat_ptn = TRegEx::Escape(mat_ptn);
	if (!mat_ptn.IsEmpty()) {
		TRegExOptions opt; opt << roIgnoreCase;
		TStringList *lst = ((test_HtmlExt(get_extension(TxtViewer->FileName)) && TxtViewer->isHtm2Txt) || TxtViewer->isJsonFmt)?
								 TxtViewer->TxtBufList2 : TxtViewer->TxtBufList;
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			if (TRegEx::IsMatch(lbuf, mat_ptn, opt)) UserDefList->AddObject(lbuf, (TObject*)(NativeInt)(i + 1));
		}

		if (UserDefList->Count>0) {
			add_ComboBox_history(UserDefComboBox);
			IniFile->SaveComboBoxItems(UserDefComboBox, RegExCheckBox->Checked? _T("UserPtnHistory") : _T("UserDefHistory"));
		}
	}

	UpdateList(LinkCheckBox->Checked);
	TxtViewer->Repaint(true);

	FuncListBox->SetFocus();
	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::UpdUserDefActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;

	if (ListMode==1) {
		//正規表現パターンのチェック
		UnicodeString uwd = UserDefComboBox->Text;
		bool reg_ng = RegExCheckBox->Checked && !uwd.IsEmpty() && !chk_RegExPtn(uwd);
		set_ErrColor(UserDefComboBox, reg_ng);
		ap->Enabled = RegExCheckBox->Checked? (!uwd.IsEmpty() && !reg_ng) : !uwd.IsEmpty();
	}
	else {
		ap->Enabled = false;
	}

	UserDefComboBox->Tag
		= CBTAG_HISTORY | (UserDefComboBox->Focused()? CBTAG_RGEX_V : 0) | (RegExCheckBox->Checked? CBTAG_RGEX_E : 0);
}

//---------------------------------------------------------------------------
//一覧を閉じる
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::CloseListActionExecute(TObject *Sender)
{
	TListBox *lp = FuncListBox;
	int idx = lp->ItemIndex;
	if (idx>=0 && idx<lp->Count) {
		ToLine();
		ModalResult = mrOk;
	}
	else beep_Warn();
}
//---------------------------------------------------------------------------
//テキストエディタで開く
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::ReqEditActionExecute(TObject *Sender)
{
	CmdRequestList->Add("FileEditV");
	CloseListAction->Execute();
}

//---------------------------------------------------------------------------
//一覧の内容文字列を取得
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::GetStrList(TStringList *lst)
{
	for (int i=0; i<FuncListBox->Count; i++) {
		UnicodeString lbuf = Trim(FuncListBox->Items->Strings[i]);

		if (NameOnlyCheckBox->Checked && ListMode==0 && !NamePtn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			TMatch mt = TRegEx::Match(lbuf, NamePtn, opt);
			if (mt.Success) {
				lbuf = mt.Value;
				if (ends_tchs(_T("([:"), lbuf)) delete_end(lbuf);
			}
		}

		lst->Add(lbuf);
	}
}

//---------------------------------------------------------------------------
//一覧をコピー
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::CopyListActionExecute(TObject *Sender)
{
	std::unique_ptr<TStringList> cbuf(new TStringList());
	GetStrList(cbuf.get());
	copy_to_Clipboard(cbuf->Text);
}

//---------------------------------------------------------------------------
//一覧をファイルに保存
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::SaveListActionExecute(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, USTR_List);
	UserModule->SaveTxtDlg->InitialDir	  = ExtractFileDir(TxtViewer->FileName);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		GetStrList(fbuf.get());
		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, USTR_List));
	}

	FuncListBox->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::ListActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = FuncListBox->Count>0;
}

//---------------------------------------------------------------------------
//ユーザ定義文字列を拡張子に対する見出しとして登録
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::RegHeaderActionExecute(TObject *Sender)
{
	UnicodeString xstr	= get_tkn_r(get_extension(TxtViewer->FileName), '.').LowerCase();
	UnicodeString h_ptn = UserDefComboBox->Text;
	if (!xstr.IsEmpty()) {
		int ix = HeadlineList->IndexOfName(xstr);
		if (ix!=-1) HeadlineList->Delete(ix);
		if (!h_ptn.IsEmpty()) {
			HeadlineList->Add(xstr + "=" + h_ptn);
			TxtViewer->HeadlinePtn = h_ptn;
		}

		TxtViewer->Repaint(true);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::RegHeaderActionUpdate(TObject *Sender)
{
	UnicodeString uwd = UserDefComboBox->Text;
	bool reg_ok = RegExCheckBox->Checked && !uwd.IsEmpty() && chk_RegExPtn(uwd);

	UnicodeString fext = get_extension(TxtViewer->FileName);
	bool fext_ok = !fext.IsEmpty() && !test_HtmlExt(fext);

	((TAction*)Sender)->Enabled = (ListMode==1 && fext_ok && (reg_ok || uwd.IsEmpty()));
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::MigemoActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	UpdateList(LinkCheckBox->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::NameOnlyActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	FuncListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::LinkActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TFuncListDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	//チェックボックスの疑似アクセラレータ処理
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+L")) LinkAction->Execute();
	else if (USAME_TI(KeyStr, "Alt+N")) NameOnlyAction->Execute();
	else if (USAME_TI(KeyStr, "Alt+M")) MigemoAction->Execute();
	else if (USAME_TI(KeyStr, "Alt+R")) RegExAction->Execute();
	else if (USAME_TI(KeyStr, "Alt+E")) ReqEditAction->Execute();
	else SpecialKeyProc(this, Key, Shift, HelpContext);
}
//---------------------------------------------------------------------------

