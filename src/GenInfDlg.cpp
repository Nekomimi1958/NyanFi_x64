//----------------------------------------------------------------------//
// NyanFi																//
//  汎用一覧ダイアログ													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <Vcl.Clipbrd.hpp>
#include <RegularExpressions.hpp>
#include "usr_id3.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "FindTxtDlg.h"
#include "FileInfDlg.h"
#include "GenInfDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGeneralInfoDlg *GeneralInfoDlg;

//---------------------------------------------------------------------------
__fastcall TGeneralInfoDlg::TGeneralInfoDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, GenListBox, USCRPNL_FLAG_GL|USCRPNL_FLAG_P_WP|USCRPNL_FLAG_L_WP|USCRPNL_FLAG_HS);

	org_GenListWndProc	   = GenListBox->WindowProc;
	GenListBox->WindowProc = GenListWndProc;

	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	GenInfoList = new TStringList();
	GenInfoBuff = new TStringList();
	GenSelList	= new TStringList();

	isVarList	 = isLog = isGit = isDirs = isTree = isFTP = isFileList = isPlayList = false;
	isTail		 = isReverse = false;
	ToFilter	 = ToEnd = false;
	ErrOnly 	 = false;
	isFiltered	 = false;

	CodePage	 = 0;
	HasBOM		 = false;
	MaxNameWidth = 0;
	LastIndex	 = 0;
	LastTopIndex = 0;
	TailLine	 = 100;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormShow(TObject *Sender)
{
	OpeToolBar->GradientStartColor = col_bgTlBar1;
	OpeToolBar->GradientEndColor   = col_bgTlBar2;
	OpeToolBar->Font->Color 	   = col_fgTlBar;
	OpeToolBar->HotTrackColor	   = col_htTlBar;
	FilterSplitter->Color = Mix2Colors(col_bgTlBar1, col_bgTlBar2);

	FilterEdit->Font->Assign(DialogFont);
	FilterEdit->Width = IniFile->ReadIntGen(_T("GenInfoFilterWidth"),	200);
	FilterEdit->Text = EmptyStr;
	RetStr = EmptyStr;
	GenSelList->Clear();

	CancelKeySeq();

	FindWord  = RegExPtn = EmptyStr;
	Found	  = false;
	SortMode  = 0;
	ListWidth = 0;

	//見出しパターンの設定
	if (UserHighlight->Recycle()) AddErr_Highlight();
	if (HdrLnStr.IsEmpty()) {
		HdrLnStr = UserHighlight->GetHeadlinePtn(FileName, HeadlineList);
		if (!HdrLnStr.IsEmpty()) HdrLnStr = "/" + HdrLnStr + "/";
	}

	if (isPlayList) GenInfoList->Assign(PlayList);

	if (Caption.IsEmpty()) {
		UnicodeString cap_str = def_if_empty(yen_to_delimiter(FileName), LoadUsrMsg(USTR_List));
		if (isTail) {
			cap_str.cat_sprintf(_T(" (末尾 %u行"), GenInfoList->Count);
			if (isReverse) cap_str.UCAT_T(":逆順");
			cap_str.UCAT_T(")");
		}
		else if (isTree) {
			cap_str.UCAT_T(" (ツリー表示)");
		}
		Caption = cap_str;
	}

	isDirs = USAME_TI(Caption, "ディレクトリ名一覧");

	IniFile->LoadPosInfo(this, DialogCenter);

	ShowLineNoAction->Checked  = IniFile->ReadBoolGen(_T("GenInfoShowLnNo"));
	KeepIndexAction->Checked   = IniFile->ReadBoolGen(_T("GenInfoKeepIndex"));
	FileName1stAction->Checked = IniFile->ReadBoolGen(_T("GenInfoFileName1st"));
	OmitComPathAction->Checked = IniFile->ReadBoolGen(_T("GenInfoOmitComPath"));

	set_MigemoAction(MigemoAction, _T("GenInfoMigemo"));

	AndOrAction->Checked	 = IniFile->ReadBoolGen(_T("GenInfoFltAndOr"));
	HighlightAction->Checked = IniFile->ReadBoolGen(_T("GenInfoHighlight"));

	if (isTail) {
		WatchAction->Checked  = IniFile->ReadBoolGen(_T("GenInfoTailWatch"),	false);
		NotifyAction->Checked = IniFile->ReadBoolGen(_T("GenInfoTailNotify"),	false);
	}

	//ログファイルか?
	if (!FileName.IsEmpty()) isLog = str_match(UAPP_T(ExePath, "tasklog*.txt"), FileName);

	//Git?
	isGit = (GenInfoList->Count>0 && StartsStr("$ git ", GenInfoList->Strings[0]));

	//「名前=値」形式か?
	if (!isVarList && !isLog) {
		bool all_n_v = (GenInfoList->Count>0);
		for (int i=0; i<GenInfoList->Count && all_n_v; i++) {
			if (i==0 && isGit) continue;
			if (!GenInfoList->Strings[i].IsEmpty()) {
				UnicodeString nbuf = GenInfoList->Names[i];
				all_n_v = !nbuf.IsEmpty() && !contains_s(nbuf, _T(' ')) && !contains_s(nbuf, _T('\t'));
			}
		}
		isVarList = all_n_v;
	}

	//リストボックスの初期化(仮想)
	TListBox *lp = GenListBox;
	lp->Tag = LBTAG_GEN_LIST | LBTAG_OPT_ZOOM
				| (ShowLineNoAction->Checked? LBTAG_OPT_LNNO : 0)
				| ((isFileList || isTree)? LBTAG_TAB_FNAM : 0)	//タブ以降に実ファイル名がある
				| (isTree? LBTAG_OPT_TREE : 0);

	lp->Color = isLog? col_bgLog : col_bgList;
	ListPanel->Color = lp->Color;
	lp->Font->Assign(GenListFont);
	set_ListBoxItemHi(lp);
	lp->TabWidth = get_ViewTabWidth(get_extension(FileName));	//タブ幅(描画のために流用)
	set_UsrScrPanel(ListScrPanel);

	//ファイル名/変数名の最大幅を取得
	MaxNameWidth = 0;
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	if (isPlayList && FileName1stAction->Checked) {
		for (int i=0; i<GenInfoList->Count; i++) {
			MaxNameWidth = std::max(get_TextWidth(cv, ExtractFileName(GenInfoList->Strings[i]), is_irreg), MaxNameWidth);
		}
		MaxNameWidth += get_TextWidth(cv, " ", is_irreg);
	}
	else if (isVarList) {
		for (int i=0; i<GenInfoList->Count; i++) {
			MaxNameWidth = std::max(get_TextWidth(cv, GenInfoList->Names[i], is_irreg), MaxNameWidth);
		}
		MaxNameWidth += get_TextWidth(cv, " = ", is_irreg);
	}

	//パスの共通部分を取得
	ComPathName = EmptyStr;
	if (isPlayList && GenInfoList->Count>1) {
		ComPathName = ExtractFilePath(GenInfoList->Strings[0]);
		for (int i=1; i<GenInfoList->Count; i++) {
			UnicodeString pnam = ExtractFilePath(GenInfoList->Strings[i]);
			if (!SameText(pnam, ComPathName)) {
				TStringDynArray plst0 = split_path(ComPathName);
				TStringDynArray plst1 = split_path(pnam);
				ComPathName = EmptyStr;
				for (int j=0; (j<plst0.Length && j<plst1.Length); j++) {
					if (!SameText(plst0[j], plst1[j])) break;
					ComPathName.cat_sprintf(_T("%s\\"), plst0[j].c_str());
				}
			}
		}
	}

	FileSize = -1;
	UpdateList();

	//カーソル位置を設定
	int idx = -1;
	int top = -1;
	if (lp->Count>0) {
		if (isLog || ToEnd)
			idx = lp->Count - 1;
		else if (KeepIndexAction->Checked) {
			idx = LastIndex;
			top = LastTopIndex;
			if		(idx==-1)		 idx = 0;
			else if (idx>=lp->Count) idx = lp->Count - 1;
		}
		else
			idx = 0;
	}

	if (top!=-1) lp->TopIndex = top;
	lp->ItemIndex = idx;
	ListScrPanel->UpdateKnob();

	(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)lp)->SetFocus();
	FilterEdit->Color = ToFilter? scl_Window : col_Invalid;

	//ステータスバー
	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);
	set_SttBarPanelWidth(StatusBar1, 0, 21 + IntToStr(GenInfoList->Count).Length() * 3);
												//"項目: nnn  -  nnn    選択: nnn"
	set_SttBarPanelWidth(StatusBar1, 1, 17);	//"UTF-16(BE) BOM付 ";
	set_SttBarPanelWidth(StatusBar1, 2,  5);	//"CR/LF";
	SetStatusBar();

	if (!ErrMsg.IsEmpty()) {
		msgbox_ERR(ErrMsg);
	}
	else {
		Timer1->Interval = 1000;	//***
		Timer1->Enabled  = isTail && WatchAction->Checked;
	}

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen( _T("GenInfoFilterWidth"),	FilterEdit->Width);
	IniFile->WriteBoolGen(_T("GenInfoShowLnNo"),	ShowLineNoAction);
	IniFile->WriteBoolGen(_T("GenInfoKeepIndex"),	KeepIndexAction);
	IniFile->WriteBoolGen(_T("GenInfoFileName1st"),	FileName1stAction);
	IniFile->WriteBoolGen(_T("GenInfoOmitComPath"),	OmitComPathAction);
	IniFile->WriteBoolGen(_T("GenInfoMigemo"),		MigemoAction);
	IniFile->WriteBoolGen(_T("GenInfoFltAndOr"),	AndOrAction);
	IniFile->WriteBoolGen(_T("GenInfoHighlight"),	HighlightAction);
	if (isTail) {
		IniFile->WriteBoolGen(_T("GenInfoTailWatch"),	WatchAction);
		IniFile->WriteBoolGen(_T("GenInfoTailNotify"),	NotifyAction);
	}

	LastIndex	 = (ModalResult==mrRetry)? GenListBox->ItemIndex : 0;
	LastTopIndex = (ModalResult==mrRetry)? GenListBox->TopIndex : 0;

	GenInfoList->Clear();
	GenInfoBuff->Clear();

	isVarList	 = isLog = isGit = isDirs = isTree = isFTP = isFileList = isPlayList = false;
	isTail		 = isReverse = false;
	ToFilter	 = ToEnd = false;
	ErrOnly 	 = false;
	isFiltered	 = false;

	FileName	 = EmptyStr;
	Caption 	 = EmptyStr;
	ErrMsg		 = EmptyStr;

	CodePage	 = 0;
	HasBOM		 = false;
	LineBreakStr = EmptyStr;
	HdrLnStr	 = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormDestroy(TObject *Sender)
{
	delete GenInfoList;
	delete GenInfoBuff;
	delete GenSelList;
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListWndProc(TMessage &msg)
{
	if (msg.Msg==WM_VSCROLL || msg.Msg==WM_NYANFI_USCROLL) {
		if (TvCursorVisible && ListBoxCsrToVisible(GenListBox)) {
			GenListBox->Invalidate();
			SetStatusBar();
		}
	}

	org_GenListWndProc(msg);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::WmFormShowed(TMessage &msg)
{
	Application->ProcessMessages();
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::WmMenuChar(TMessage &msg)
{
	if (msg.WParamHi==MF_POPUP)
		TForm::Dispatch(&msg);
	else
		msg.Result = (MNC_CLOSE << 16);		//ビープ音が鳴らないようにする
}

//---------------------------------------------------------------------------
//プレイリストの更新
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::WmNyanFiPlayList(TMessage &msg)
{
	if (isPlayList) {
		if (msg.WParam==1) {
			GenInfoList->Assign(PlayList);
			UpdateList();
		}
		else GenListBox->Repaint();
	}
}

//---------------------------------------------------------------------------
//ListTail の監視処理
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::Timer1Timer(TObject *Sender)
{
	if (!FileName.IsEmpty()) {
		__int64   fsz = get_file_size(FileName);
		TDateTime fdt = get_file_age(FileName);
		if (FileSize!=fsz || FileTime!=fdt) {
			if (FileSize>0) {
				if (UpdateList(true) && NotifyAction->Checked) {
					play_sound(SoundWatch);
					NotifyPrimNyan("監視中のファイルが変化しました");
				}
			}
			FileSize = fsz;
			FileTime = fdt;
		}
	}
}
//---------------------------------------------------------------------------
//一覧の更新
// GenInfoBuff->Objects を行インデックス付けに利用
//---------------------------------------------------------------------------
bool __fastcall TGeneralInfoDlg::UpdateList(bool reload)
{
	UnicodeString last_text = GenInfoBuff->Text;
	Found	  = false;
	SortMode  = 0;
	ListWidth = 0;

	ListScrPanel->HitLines->Clear();

	TListBox *lp = GenListBox;
	int idx = lp->ItemIndex;
	if (idx>=0 && idx<GenInfoBuff->Count && isFiltered) idx = (int)GenInfoBuff->Objects[idx];

	if (reload) {
		std::unique_ptr<TStringList> f_buf(new TStringList());
		if (load_text_tail(FileName, f_buf.get(), CodePage, TailLine, isReverse)!=0) {
			GenInfoList->Assign(f_buf.get());
		}
		else msgbox_ERR(USTR_FileNotOpen);
	}

	cursor_HourGlass();
	GenInfoBuff->Clear();
	//ログのエラー箇所のみ絞り込み
	if (isLog && ErrOnly) {
		bool er_flag = false;
		for (int i=0; i<GenInfoList->Count; i++) {
			UnicodeString lbuf = GenInfoList->Strings[i];
			if (lbuf.Length()<4) continue;
			if (lbuf[2]=='>' && lbuf[3]=='E' && lbuf[4]==' ') {
				er_flag = true;
				GenInfoBuff->AddObject(lbuf, (TObject*)(NativeInt)i);
			}
			else if (er_flag) {
				if (StartsStr("    ", lbuf)) {
					GenInfoBuff->AddObject(lbuf, (TObject*)(NativeInt)i);
				}
				er_flag = false;
			}
		}
		idx = 0;
		isFiltered = true;
	}
	//フィルタ
	else if (!FilterEdit->Text.IsEmpty()
		&& (!MigemoAction->Checked || (MigemoAction->Checked && FilterEdit->Text.Length()>=IncSeaMigemoMin)))
	{
		UnicodeString pnam = (isTree && GenInfoList->Count>0)? get_post_tab(GenInfoList->Strings[0]) : EmptyStr;

		filter_List(GenInfoList, GenInfoBuff, FilterEdit->Text,
					MigemoAction->Checked, AndOrAction->Checked, false, isFileList, isTree);

		if (isTree) {
			MakeTreeList(GenInfoBuff, pnam, true);
			idx = -1;
		}

		if (idx!=-1) idx = GenInfoBuff->IndexOfObject((TObject*)(NativeInt)idx);
		isFiltered = true;
	}
	//通常表示
	else {
		for (int i=0; i<GenInfoList->Count; i++) GenInfoBuff->AddObject(GenInfoList->Strings[i], (TObject*)(NativeInt)i);
		isFiltered = false;
	}
	cursor_Default();

	//リストボックスに割り当て(仮想)
	assign_FileListBox(lp, GenInfoBuff, 0);
	if (lp->Count>0) lp->ItemIndex = (idx!=-1)? idx : 0;
	ListScrPanel->UpdateKnob();

	SetStatusBar();

	return (reload && !SameStr(last_text, GenInfoBuff->Text));
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TGeneralInfoDlg::GetBuffText()
{
	if (isTree || isFileList) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Assign(GenInfoBuff);
		for (int i=0; i<fbuf->Count; i++) fbuf->Strings[i] = get_pre_tab(fbuf->Strings[i]);
		return fbuf->Text;
	}
	else {
		return GenInfoBuff->Text;
	}
}

//---------------------------------------------------------------------------
//文字列検索
//---------------------------------------------------------------------------
bool __fastcall TGeneralInfoDlg::FindText(
	bool down)		//true = 下方向/ false = 上方向
{
	if (FindWord.IsEmpty() && RegExPtn.IsEmpty()) return false;

	cursor_HourGlass();

	TListBox *lp = GenListBox;
	int idx0 = lp->ItemIndex;
	if (idx0==-1) idx0 = 0;
	int idx1 = -1;
	bool case_sw = FindTextDlg->CaseCheckBox->Checked;

	//正規表現
	if ((FindTextDlg->MigemoCheckBox->Enabled && FindTextDlg->MigemoCheckBox->Checked)
		|| FindTextDlg->RegExCheckBox->Checked)
	{
		if (chk_RegExPtn(RegExPtn)) {
			TRegExOptions opt;
			if (!case_sw) opt << roIgnoreCase;
			for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1))
				if (TRegEx::IsMatch(lp->Items->Strings[i], RegExPtn, opt)) { idx1 = i; break; }

			if (ListScrPanel->KeyWordChanged(RegExPtn, lp->Count, case_sw)) {
				for (int i=0; i<lp->Count; i++) {
					if (TRegEx::IsMatch(lp->Items->Strings[i], RegExPtn, opt))
						ListScrPanel->AddHitLine(i);
				}
				ListScrPanel->Repaint();
			}
		}
		else {
			ListScrPanel->KeyWordChanged(EmptyStr, 0);
		}
	}
	//通常
	else if (!FindWord.IsEmpty()) {
		for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1))
			if (case_sw? lp->Items->Strings[i].Pos(FindWord) : pos_i(FindWord, lp->Items->Strings[i])) {
				 idx1 = i; break;
			}

		if (ListScrPanel->KeyWordChanged(FindWord, lp->Count, case_sw)) {
			for (int i=0; i<lp->Count; i++) {
				if (case_sw? lp->Items->Strings[i].Pos(FindWord) : pos_i(FindWord, lp->Items->Strings[i]))
					ListScrPanel->AddHitLine(i);
			}
			ListScrPanel->Repaint();
		}
	}
	else {
		ListScrPanel->KeyWordChanged(EmptyStr, 0);
	}

	Found = (idx1!=-1);

	//結果表示
	UnicodeString msg; msg.sprintf(_T("[%s] "), FindWord.c_str());
	if (Found) {
		lp->ItemIndex = idx1;
		lp->Repaint();
		msg.UCAT_T("発見");
	}
	else {
		beep_Warn();
		msg.UCAT_T("見つかりません");
	}
	SetStatusBar(msg);
	cursor_Default();

	return (Found);
}

//---------------------------------------------------------------------------
//ステータスバーの設定
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SetStatusBar(UnicodeString msg)
{
	UnicodeString stt_str;
	TListBox *lp = GenListBox;
	stt_str.sprintf(_T("%s: %u"), (ErrOnly? _T("ERR") : _T("項目")), lp->Count);

	if (isFiltered) 		stt_str.cat_sprintf(_T("/%u"),	GenInfoList->Count);
	if (lp->ItemIndex!=-1)	stt_str.cat_sprintf(_T("  -  %u"),lp->ItemIndex + 1);
	if (lp->SelCount>0)		stt_str.cat_sprintf(_T("    選択: %u"), lp->SelCount);
	StatusBar1->Panels->Items[0]->Text = stt_str;

	StatusBar1->Panels->Items[1]->Text =
		 get_NameOfCodePage(CodePage)
		 	+ ((CodePage==1200 || CodePage==1201 || CodePage==65001)? (HasBOM? " BOM付" : " BOM無") : "");

	StatusBar1->Panels->Items[2]->Text = LineBreakStr;

	UnicodeString lbuf = (lp->ItemIndex!=-1)? lp->Items->Strings[lp->ItemIndex] : EmptyStr;
	UnicodeString fnam = (isTree || isFileList)? get_post_tab(lbuf) : EmptyStr;

	UnicodeString id3_str;
	if (isPlayList && test_Mp3Ext(get_extension(lbuf))) {
		std::unique_ptr<TStringList> lst(new TStringList());
		if (ID3_GetInf(lbuf, lst.get(), true)) {
			id3_str.cat_sprintf(_T("%s - %s / %s"),
				lst->Values["TP1"].c_str(), lst->Values["TT2"].c_str(), lst->Values["TAL"].c_str());
		}
	}

	int w3 = StatusBar1->ClientWidth - 16;
	for (int i=0; i<(StatusBar1->Panels->Count - 1); i++) w3 -= StatusBar1->Panels->Items[i]->Width;

	StatusBar1->Panels->Items[3]->Text =
		     !msg.IsEmpty()? msg :
		!FirstKey.IsEmpty()? "2ストロークキー: " + FirstKey :
		    !fnam.IsEmpty()? get_MiniPathName(fnam, w3, StatusBar1->Font, true) :
		 !id3_str.IsEmpty()? id3_str :
		!FileName.IsEmpty()? get_FileInfStr(FileName, false) : EmptyStr;

	StatusBar1->Repaint();
}
//---------------------------------------------------------------------------
//ステータスバーの描画
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
		const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect) ;

	UnicodeString lbuf = Panel->Text;
	int yp = (Rect.Height() - abs(cv->Font->Height)) / 2;
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(Rect.Left + Scaled2, yp, split_pre_tab(lbuf));
	if (!lbuf.IsEmpty()) cv->TextOut(Rect.Right - cv->TextWidth(lbuf) - Scaled4, yp, lbuf);
}

//---------------------------------------------------------------------------
//フィルタ
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditChange(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	TListBox *lp = GenListBox;

	if		(contained_wd_i(KeysStr_ToList, KeyStr) || equal_ENTER(KeyStr)) lp->SetFocus();
	else if (MovListBoxFromFilter(lp, KeyStr))		;
	else if (SameText(KeyStr, KeyStr_Migemo))		MigemoAction->Checked = !MigemoAction->Checked;
	else if	(equal_ESC(KeyStr))						ModalResult = mrCancel;
	else return;

	lp->Invalidate();
	SetStatusBar();
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterOptheckBoxClick(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::HighlightCheckBoxClick(TObject *Sender)
{
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FilterBtnClick(TObject *Sender)
{
	FilterEdit->SetFocus();
}

//---------------------------------------------------------------------------
//一覧の描画 (仮想)
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	TRect rc = Rect;
	int yp = rc.Top + 1;
	int tw = FileName.IsEmpty()? 4 : lp->TabWidth;
	int lw = 0;
	UnicodeString lbuf = lp->Items->Strings[Index];

	//文字数制限
	if (lbuf.Length()>1024) lbuf = lbuf.SubString(1, 1024).UCAT_T("…");	//***

	//フィルタ/検索のマッチ語
	std::unique_ptr<TStringList> wlist(new TStringList());
	if (HighlightAction->Checked) {
		if (Found && Index==lp->ItemIndex) {
			get_MatchWordList(lbuf, RegExPtn,
				false, true,
				AndOrAction->Checked, wlist.get());
		}
		else if (!FilterEdit->Text.IsEmpty()) {
			get_MatchWordList(lbuf, FilterEdit->Text,
				MigemoAction->Checked, false,
				AndOrAction->Checked, wlist.get());
		}
	}

	//行番号
	if (ShowLineNoAction->Checked) LineNoOut(cv, rc, (int)lp->Items->Objects[Index] + 1);

	//背景色
	cv->Brush->Color = State.Contains(odSelected)? (lp->Focused()? col_selItem : col_oppItem) :
												isLog? col_bgLog :
			   (isPlayList && is_AltLnBgCol(Index))? col_bgList2 : col_bgList;
	cv->FillRect(rc);

	rc.Left += 4;
	int xp = rc.Left;
	int w0 = rc.Left;

	bool use_fgsel = is_SelFgCol(State);

	//ログ
	if (isLog) {
		TColor fg =
			use_fgsel? col_fgSelItem :
			is_match_regex(lbuf, _T("^.>([ECW]|     [45]\\d{2})\\s"))? col_Error :
			(lbuf.Pos(':')==5 && contains_wd_i(lbuf, _T("開始|>>"))) ? col_Headline :
												 (lbuf.Pos('!')==10) ? AdjustColor(col_fgLog, 96) : col_fgLog;
		RuledLnTextOut(yen_to_delimiter(lbuf), cv, rc, fg, tw, wlist.get());
	}
	//変数一覧
	else if (isVarList) {
		if (lbuf.Pos('=')) {
			UnicodeString namstr = split_tkn(lbuf, '=').UCAT_T(" = ");
			rc.Left = xp + MaxNameWidth - get_TextWidth(cv, namstr, is_irreg);
			RuledLnTextOut(namstr, cv, rc, use_fgsel? col_fgSelItem : col_fgInfNam, tw, wlist.get());
			rc.Left = xp + MaxNameWidth;
			lw = MaxNameWidth + get_TabTextWidth(lbuf, cv, tw);
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_fgList, tw, wlist.get());
		}
		else
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_Headline, tw, wlist.get());
	}
	//ファイルリスト
	else if (isFileList) {
		UnicodeString fnam = get_post_tab(lbuf);
		bool is_dir = ends_PathDlmtr(fnam);
		fnam = ExcludeTrailingPathDelimiter(fnam);
		lbuf = get_pre_tab(lbuf);

		if (StartsText(fnam, lbuf)) {
			lbuf.Delete(1, fnam.Length());
		}
		else {
			fnam = ExtractFileName(fnam);
			if (StartsText(fnam, lbuf)) lbuf.Delete(1, fnam.Length()); else fnam = EmptyStr;
		}

		if (!fnam.IsEmpty()) {
			if (is_dir) {
				cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
				PathNameOut(fnam, wlist.get(), cv, xp, yp);
			}
			else {
				FileNameOut(cv, rc, fnam, use_fgsel, true, wlist.get());
			}
			RuledLnTextOut(lbuf, cv, rc, use_fgsel? col_fgSelItem : col_fgList, tw, wlist.get());
		}
		else {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
	}
	//ディレクトリ名一覧
	else if (isDirs) {
		cv->Font->Color = use_fgsel? col_fgSelItem : col_Folder;
		PathNameOut(lbuf, wlist.get(), cv, xp, yp);
	}
	//ディレクトリツリー表示
	else if (isTree) {
		lbuf = get_pre_tab(lbuf);
		TColor fg = use_fgsel? col_fgSelItem : col_Folder;
		if (Index==0) {
			cv->Font->Color = fg;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
		else
			RuledLnTextOut(lbuf, cv, rc, fg, tw, wlist.get());
	}
	//プレイリスト
	else if (isPlayList) {
		//再生中マーク
		if (SameText(PlayFile, lbuf)) out_Text(cv, rc.Left + 2, yp, L"\u25b6", col_Cursor);
		rc.Left += get_TextWidth(cv, L"\u25b6", is_irreg) + 4;
		if (OmitComPathAction->Checked) remove_top_text(lbuf, ComPathName);	//パスの共通部分を省略
		if (FileName1stAction->Checked) {
			xp = rc.Left;
			RuledLnTextOut(ExtractFileName(lbuf),
				cv, rc, use_fgsel? col_fgSelItem : get_ExtColor(get_extension(lbuf)), tw, wlist.get());
			rc.Left = xp + MaxNameWidth;
			FileNameOut(cv, rc, ExtractFilePath(lbuf), use_fgsel, true, wlist.get());
		}
		else
			FileNameOut(cv, rc, lbuf, use_fgsel, true, wlist.get());
	}
	//NyanFi 情報
	else if (USAME_TS(Caption, "NyanFi 情報")) {
		TColor fg =
			use_fgsel? col_fgSelItem :
			is_match_regex(lbuf, _T("^([^ :.]{3,}\\.(exe|dll|spi)|\\.|WIC)"))? col_Headline :
			StartsStr("         エラー:", lbuf)? col_Error :
			StartsStr('[', lbuf)? col_Comment : col_fgList;

		if (EndsStr('\\', lbuf) && lbuf.Pos(':')>1) {
			cv->Font->Color = fg;
			PathNameOut(lbuf, wlist.get(), cv, xp, yp);
		}
		else
			PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get());
	}
	//Git
	else if (isGit) {
		TColor fg = use_fgsel? col_fgSelItem :
			 		StartsStr("$ git ", lbuf)? col_Headline :
		 	 		StartsStr("-", lbuf)? clRed : StartsStr("+", lbuf)? clGreen : col_fgList;
		UnicodeString s = split_GitGraphStr(lbuf);
		if (!s.IsEmpty()) {
			UnicodeString s1 = (Index>1)? get_GitGraphStr(lp->Items->Strings[Index - 1]) : EmptyStr;
			UnicodeString s2 = (Index<(lp->Count - 1))? get_GitGraphStr(lp->Items->Strings[Index + 1]) : EmptyStr;
			draw_GitGraph(s, s1, s2, cv, rc, get_in_paren(lbuf).Pos("HEAD -> "));
		}
		if (!lbuf.IsEmpty()) RuledLnTextOut(lbuf, cv, rc, fg, tw, wlist.get());
	}
	//見出し行
	else if (!ptn_match_str(HdrLnStr, lbuf).IsEmpty()) {
		TColor fg = use_fgsel? col_fgSelItem : col_Headline;
		PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get());
	}
	//コメント or 通常行
	else {
		TColor fg = use_fgsel? col_fgSelItem : col_fgList;
		PrvTextOut(lp, Index, cv, rc, fg, tw, wlist.get(), FileName, (SortMode==0 && !isFiltered));
	}

	//スクロール幅を設定
	int lst_scr_wd = lp->ScrollWidth;
	ListWidth = std::max(ListWidth, ((lw>0)? lw : get_TabTextWidth(lbuf, cv, tw)) + get_CharWidth(cv, 4));
	lp->ScrollWidth = w0 + ListWidth;
	if (lst_scr_wd!=lp->ScrollWidth) ListScrPanel->UpdateKnob();

	//カーソル
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<GenInfoBuff->Count)? GenInfoBuff->Strings[Index] : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDataObject(TWinControl *Control, int Index, TObject *&DataObject)
{
	DataObject = (Index>=0 && Index<GenInfoBuff->Count)? GenInfoBuff->Objects[Index] : NULL;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxExit(TObject *Sender)
{
	CancelKeySeq();
	((TListBox*)Sender)->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxClick(TObject *Sender)
{
	CancelKeySeq();
}

//---------------------------------------------------------------------------
//2ストローク操作の処理
//---------------------------------------------------------------------------
UnicodeString __fastcall TGeneralInfoDlg::TwoStrokeSeq(WORD &Key, TShiftState Shift)
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
	else {
		bool found = false;
		for (int i=0; i<KeyFuncList->Count && !found; i++) {
			UnicodeString kbuf = KeyFuncList->Strings[i];
			if (!contains_s(kbuf, _T('~')))  continue;
			found = SameText("V:" + key_str, get_tkn(kbuf, '~'));
		}
		if (found) {
			FirstKey   = key_str;
			Wait2ndKey = true;
			SetStatusBar();
			Key = 0;
		}
	}

	return key_str;
}
//---------------------------------------------------------------------------
//2ストローク操作を中断
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CancelKeySeq()
{
	FirstKey   = EmptyStr;
	Wait2ndKey = false;
	SetStatusBar();
}

//---------------------------------------------------------------------------
//一覧のキー操作
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = TwoStrokeSeq(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	TListBox *lp = (TListBox*)Sender;

	//コマンド
	if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V))
		;
	//前後のファイルへ切り替え
	else if (!FileName.IsEmpty() && !isFTP && contained_wd_i(_T("PrevFile|NextFile"), cmd_V)) {
		RetStr = cmd_V;
		this->Perform(WM_SETREDRAW, 0, (NativeInt)0);	//画面を消さずに残す
		ModalResult = mrRetry;
	}
	//ソート
	else if (USAME_TI(cmd_V, "Sort_AO"))	SortAscAction->Execute();
	else if (USAME_TI(cmd_V, "Sort_DO"))	SortDesAction->Execute();
	else if (USAME_TI(cmd_V, "Sort"))		OrgOrderAction->Execute();
	//検索
	else if (USAME_TI(cmd_V, "FindText"))
		FindTextDlg->ShowModal();
	else if (contained_wd_i(_T("FindDown|FindUp"), cmd_V))
		FindText(USAME_TI(cmd_V, "FindDown"));
	//エラー検索
	else if (contained_wd_i(_T("NextErr|PrevErr"), cmd_V)) {
		if (isLog) {
			bool down = USAME_TI(cmd_V, "NextErr");
			cursor_HourGlass();
			int idx0 = lp->ItemIndex;
			if (idx0==-1) idx0 = 0;
			int idx1 = -1;
			UnicodeString ptn = "^.>([ECW]|(     [45]\\d{2})) .*";
			for (int i = (down? idx0 + 1 : idx0 - 1); i>=0 && i<lp->Count; i += (down? 1 : -1))
				if (TRegEx::IsMatch(lp->Items->Strings[i], ptn)) { idx1 = i; break; }
			if (idx1!=-1) {
				lp->ItemIndex = idx1;
				lp->Repaint();
			}
			else beep_Warn();
			cursor_Default();
		}
	}
	//閲覧
	else if (USAME_TI(cmd_F, "TextViewer")) {
		if (ViewFileAction->Enabled) ViewFileAction->Execute(); else ViewListAction->Execute();
	}
	//編集
	else if (USAME_TI(cmd_F, "FileEdit"))	EditFileAction->Execute();
	//フィルタ欄へ
	else if (StartsText("IncSearch", cmd_F))	FilterEdit->SetFocus();
	//ファイル情報
	else if (USAME_TI(cmd_F, "ListFileInfo") || StartsText("ShowFileInfo", cmd_F))
		ShowFileInfoAction->Execute();
	//プロパティ
	else if (USAME_TI(cmd_F, "PropertyDlg"))
		PropertyAction->Execute();
	//キー
	else if (equal_ENTER(KeyStr)) {
		TListBox *lp = GenListBox;
		if (lp->ItemIndex!=-1) {
			UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
			UnicodeString cmd;
			//カーソル位置に移動
			if (isFileList) {
				if (lp->SelCount>0) {
					for (int i=0; i<lp->Count; i++) {
						if (lp->Selected[i]) GenSelList->Add(get_post_tab(lp->Items->Strings[i]));
					}
				}
				RetStr = get_post_tab(fnam);
				ModalResult = mrOk;
			}
			else if (isDirs) {
				CmdRequestList->Add(cmd.sprintf(_T("ChangeDir_\"%s\""), IncludeTrailingPathDelimiter(fnam).c_str()));
				ModalResult = mrOk;
			}
			else if (isTree) {
				CmdRequestList->Add(cmd.sprintf(_T("ChangeDir_\"%s\""), get_post_tab(fnam).c_str()));
				ModalResult = mrOk;
			}
			//カーソル位置を再生
			else if (isPlayList) {
				int idx = PlayList->IndexOf(fnam);
				if (idx!=-1) {
					cursor_HourGlass();
					PlayStbIdx = idx;
					play_PlayList(false, true);	//シャッフル抑止
					lp->Repaint();
					cursor_Default();
				}
			}
		}
	}
	else if (SameText(KeyStr, KeyStr_Copy)) CopyAction->Execute();
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) show_PopupMenu(lp);
	//閉じる
	else if (USAME_TI(cmd_V, "Close") || equal_ESC(KeyStr))  ModalResult = mrCancel;
	else return;

	if (!StartsText("Find", cmd_V)) SetStatusBar();
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//インクリメンタルサーチを回避
	if (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::GenListBoxDblClick(TObject *Sender)
{
	CancelKeySeq();
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyActionExecute(TObject *Sender)
{
	ExeCmdListBox(GenListBox, _T("ClipCopy"));
	SetStatusBar();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (GenListBox->SelCount>0);
}
//---------------------------------------------------------------------------
//すべて選択
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SelAllActionExecute(TObject *Sender)
{
	ListBoxSelectAll(GenListBox);
}

//---------------------------------------------------------------------------
//検索
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FindTextItemClick(TObject *Sender)
{
	FindTextDlg->ShowModal();
}

//---------------------------------------------------------------------------
//一覧をファイルに保存
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SaveAsActionExecute(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, USTR_List);
	UserModule->SaveTxtDlg->InitialDir	  = ExcludeTrailingPathDelimiter(SaveTxtPath);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveTxtPath  = ExtractFilePath(UserModule->SaveTxtDlg->FileName);
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Text = GetBuffText();
		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, USTR_List));
	}
}
//---------------------------------------------------------------------------
//一覧をテキストビュアーで開く
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewListActionExecute(TObject *Sender)
{
	Clipboard()->AsText = GetBuffText();
	CmdRequestList->Add("TextViewer_CB");
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewListActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = FileName.IsEmpty();
	ap->Enabled = (GenListBox->Count>0);
}
//---------------------------------------------------------------------------
//ファイルをテキストビュアーで開く
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ViewFileActionExecute(TObject *Sender)
{
	int idx = GenListBox->ItemIndex;
	if (idx==-1) idx = 0;
	int lno =  (int)GenInfoBuff->Objects[idx] + 1;
	UnicodeString cmd;
	CmdRequestList->Add(cmd.sprintf(_T("TextViewer_\"%s\"\t%u"), FileName.c_str(), lno));
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
//ファイルをテキストエディタで開く
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::EditFileActionExecute(TObject *Sender)
{
	int idx = GenListBox->ItemIndex;
	if (idx==-1) idx = 0;
	int lno = (int)GenInfoBuff->Objects[idx] + 1;
	UnicodeString cmd;
	CmdRequestList->Add(cmd.sprintf(_T("FileEdit_\"%s\"\t%u"), FileName.c_str(), lno));
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OpenFileActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = !FileName.IsEmpty();
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//ファイル情報を表示
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ShowFileInfoActionExecute(TObject *Sender)
{
	TListBox *lp = GenListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
		if (lp->Tag & LBTAG_TAB_FNAM) fnam = get_post_tab(fnam);
		if (isPlayList && PlayList->IndexOf(fnam)==-1) fnam = EmptyStr;
		if (!fnam.IsEmpty()) {
			file_rec *cfp = cre_new_file_rec(fnam);
			FileInfoDlg->inhNxtPre = true;
			FileInfoDlg->FileRec   = cfp;
			FileInfoDlg->ShowModal();
			del_file_rec(cfp);
		}
	}
}
//---------------------------------------------------------------------------
//プロパティ
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PropertyActionExecute(TObject *Sender)
{
	TListBox *lp = GenListBox;
	if (lp->ItemIndex!=-1) {
		UnicodeString fnam = lp->Items->Strings[lp->ItemIndex];
		if (lp->Tag & LBTAG_TAB_FNAM) fnam = get_post_tab(fnam);
		if (isPlayList && PlayList->IndexOf(fnam)==-1) fnam = EmptyStr;
		if (!fnam.IsEmpty()) {
			pos_ListBoxItem(lp);
			ShowPropertyDialog(fnam);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::PropertyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isFileList || isPlayList || isTree;
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
//行番号を表示
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ShowLineNoActionExecute(TObject *Sender)
{
	ShowLineNoAction->Checked = !ShowLineNoAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
//ファイル名を先に表示
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FileName1stActionExecute(TObject *Sender)
{
	FileName1stAction->Checked = !FileName1stAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FileName1stActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isPlayList;
}
//---------------------------------------------------------------------------
//パスの共通部分を省略
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OmitComPathActionExecute(TObject *Sender)
{
	OmitComPathAction->Checked = !OmitComPathAction->Checked;
	GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OmitComPathActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = isPlayList;
}

//---------------------------------------------------------------------------
//カーソル位置を維持
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::KeepIndexActionExecute(TObject *Sender)
{
	KeepIndexAction->Checked = !KeepIndexAction->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::KeepIndexActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = !FileName.IsEmpty() && !isFTP;	//前後切り替え可能時
}

//---------------------------------------------------------------------------
//エラー箇所を絞り込み表示
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ErrOnlyActionExecute(TObject *Sender)
{
	ErrOnly = !ErrOnly;
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ErrOnlyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Visible = isLog;
	((TAction*)Sender)->Checked = ErrOnly;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::InfPopupMenuPopup(TObject *Sender)
{
	SortItem->Enabled		  = !isTree;
	DelDuplItem->Enabled	  = !isTree;
	RestoreListItem->Enabled  = !isTree;
}

//---------------------------------------------------------------------------
//一覧のソート
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortGenList(int mode)
{
	cursor_HourGlass();

	TListBox *lp = GenListBox;
	int id = (lp->ItemIndex!=-1)? (int)GenInfoBuff->Objects[lp->ItemIndex] : -1;

	GenInfoBuff->CustomSort((mode==1)? comp_AscendOrder : (mode==-1)? comp_DescendOrder: comp_ObjectsOrder);

	assign_FileListBox(lp, GenInfoBuff, 0);
	int idx = 0;
	if (id!=-1) idx = GenInfoBuff->IndexOfObject((TObject*)(NativeInt)id);
	lp->ItemIndex = idx;

	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortAscActionExecute(TObject *Sender)
{
	SortMode = 1;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::SortDesActionExecute(TObject *Sender)
{
	SortMode = -1;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::OrgOrderActionExecute(TObject *Sender)
{
	SortMode = 0;
	SortGenList(SortMode);
}
//---------------------------------------------------------------------------
//一覧の重複行を削除
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::DelDuplActionExecute(TObject *Sender)
{
	cursor_HourGlass();
	//一旦昇順に
	GenInfoBuff->CustomSort(comp_AscendOrder);
	//重複行削除
	UnicodeString laststr = GenInfoBuff->Strings[0];
	int i = 1;
	int cnt = 0;
	while (i<GenInfoBuff->Count) {
		UnicodeString lbuf = GenInfoBuff->Strings[i];
		if (SameStr(laststr, lbuf)) {
			GenInfoBuff->Delete(i);
			cnt++;
		}
		else {
			laststr = lbuf;
			i++;
		}
	}
	assign_FileListBox(GenListBox, GenInfoBuff);
	cursor_Default();

	//元の並びに直す
	SortGenList(SortMode);
	GenListBox->ItemIndex = 0;

	//結果表示
	UnicodeString msg;
	if (cnt>0)
		msg.sprintf(_T("%u行の重複行を削除"), cnt);
	else
		msg.UCAT_T("重複行はありません");
	SetStatusBar(msg);
}

//---------------------------------------------------------------------------
//一覧を元の内容に戻す
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::RestoreListActionExecute(TObject *Sender)
{
	UpdateList();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ListActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (GenListBox->Count>0 && !isTree);
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::ToggleActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	Timer1->Enabled = WatchAction->Checked;
	NotifyAction->Enabled = WatchAction->Checked;

	if (ap==HighlightAction) GenListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::TailActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Enabled = isTail;
	ap->Visible = isTail;
}

//---------------------------------------------------------------------------
void __fastcall TGeneralInfoDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, HelpContext);
}
//---------------------------------------------------------------------------

