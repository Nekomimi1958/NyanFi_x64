//----------------------------------------------------------------------//
// NyanFi																//
//  キー割り当て一覧													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <Vcl.Clipbrd.hpp>
#include <RegularExpressions.hpp>
#include "UserMdl.h"
#include "Global.h"
#include "KeyDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TKeyListDlg *KeyListDlg;

//---------------------------------------------------------------------------
__fastcall TKeyListDlg::TKeyListDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormCreate(TObject *Sender)
{
	GridScrPanel = new UsrScrollPanel(GridPanel, KeyListGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);

	CurList = new TStringList();

	ToFilter = false;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	TStringGrid *gp = KeyListGrid;
	InitializeListGrid(gp);
	InitializeListHeader(KeyListHeader, _T("キー|コマンド|説明"));
	IniFile->LoadGridColWidth(gp, 3, 100,120,220);
	set_HeaderFromGrid(gp, KeyListHeader);
	set_UsrScrPanel(GridScrPanel);

	KeySortMode 				= IniFile->ReadIntGen(_T("KeyListSortMode"));
	ShowAllCmdCheckBox->Checked = IniFile->ReadBoolGen(_T("KeyListShowAll"));
	CnfExeCheckBox->Checked 	= IniFile->ReadBoolGen(_T("KeyListCnfExe"));

	set_MigemoCheckBox(MigemoCheckBox, _T("KeyListMigemo"));

	if (!UnInitializing) KeyTabControlChange(NULL);

	(ToFilter? (TWinControl*)FilterEdit : (TWinControl*)gp)->SetFocus();
	FilterEdit->Color = ToFilter? scl_Window : col_Invalid;
	FilterEdit->Text  = EmptyStr;

	CommandStr = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->SaveGridColWidth(KeyListGrid);

	IniFile->WriteIntGen(_T("KeyListSortMode"),	KeySortMode);
	IniFile->WriteBoolGen(_T("KeyListShowAll"),	ShowAllCmdCheckBox);
	IniFile->WriteBoolGen(_T("KeyListMigemo"),	MigemoCheckBox);
	IniFile->WriteBoolGen(_T("KeyListCnfExe"),	CnfExeCheckBox);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormDestroy(TObject *Sender)
{
	delete CurList;
	delete GridScrPanel;
}

//---------------------------------------------------------------------------
//下部タブの描画
//※テーマ利用時に下部タブが正しく描画されない不具合の対策
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyTabControlDrawTab(TCustomTabControl *Control, int TabIndex,
		const TRect &Rect, bool Active)
{
	draw_BottomTab(Control, TabIndex, Rect, Active);
}
//---------------------------------------------------------------------------
//一覧の切り替え
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyTabControlChange(TObject *Sender)
{
	//該当する設定を抽出
	std::unique_ptr<TStringList> cmd_lst(new TStringList());
	UnicodeString id_str = ScrModeIdStr.SubString(KeyTabControl->TabIndex + 1, 1).UCAT_T(":");
	for (int i=0; i<CmdSetList->Count; i++) {
		UnicodeString lbuf = CmdSetList->Strings[i];
		if (StartsText(id_str, lbuf)) cmd_lst->Add(lbuf);
	}

	//追加メニュー/外部ツールのエイリアス
	if (KeyTabControl->TabIndex==0) {
		UnicodeString itm;
		for (int i=0; i<ExtMenuList->Count; i++) {
			itm = get_csv_item(ExtMenuList->Strings[i], 3);
		 	if (!itm.IsEmpty()) cmd_lst->Add("F:$" + itm);
		}
		for (int i=0; i<ExtToolList->Count; i++) {
			itm = get_csv_item(ExtToolList->Strings[i], 4);
		 	if (!itm.IsEmpty()) cmd_lst->Add("F:$" + itm);
		}
	}

	CurList->Clear();
	for (int i=0; i<KeyFuncList->Count; i++) {
		UnicodeString lbuf = KeyFuncList->Strings[i];
		if (StartsText(id_str, lbuf)) CurList->Add(lbuf);
	}

	if (KeyTabControl->TabIndex==0) {
		CurList->Add(UAPP_T(IniSeaShift, "A～Z,＼=InitialSearch"));
		if (IniSeaByNum || IniSeaBySign) CurList->Add(UAPP_T(IniSeaShift, "0～9=InitialSearch"));
	}

	//未登録コマンドを追加
	if (ShowAllCmdCheckBox->Checked) {
		for (int i=0; i<CurList->Count; i++) {
			int idx = cmd_lst->IndexOf(id_str + get_CmdStr(CurList->ValueFromIndex[i]));
			if (idx!=-1) cmd_lst->Delete(idx);
		}
		for (int i=0; i<cmd_lst->Count; i++) cmd_lst->Strings[i] = ReplaceStr(cmd_lst->Strings[i], ":", ":=");
		CurList->AddStrings(cmd_lst.get());
	}

	//フィルタ
	if (!FilterEdit->Text.IsEmpty()) {
		UnicodeString ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Checked, FilterEdit->Text);
		if (!ptn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			int i=0;
			while (i<CurList->Count) {
				UnicodeString lbuf = get_tkn_r(CurList->Strings[i], ':');
				UnicodeString desc = get_CmdDesc(get_CmdStr(get_tkn_r(lbuf, '=')), true, NULL,NULL, USAME_TI(id_str, "V:"));
				lbuf.cat_sprintf(_T(" %s"), desc.c_str());
				if (!TRegEx::IsMatch(lbuf, ptn, opt)) CurList->Delete(i); else i++;
			}
		}
		else if (!MigemoCheckBox->Checked) beep_Warn();
	}

	//ソート
	switch (KeySortMode) {
	case  1:	//コマンド
		CurList->CustomSort(KeyComp_Cmd);	break;
	case  2:	//説明
		cursor_HourGlass();
		CurList->CustomSort(KeyComp_Dsc);
		cursor_Default();
		break;
	default:	//キー
		CurList->CustomSort(KeyComp_Key);
	}
	KeyListHeader->Invalidate();

	//一覧に表示
	TStringGrid *gp = KeyListGrid;
	if (CurList->Count>0) {
		gp->RowCount = CurList->Count;
		for (int i=0; i<CurList->Count; i++) {
			UnicodeString kbuf = get_tkn_r(CurList->Names[i], ':');
			int cp = 1;
			while (cp<=kbuf.Length()) {
				if (kbuf[cp]=='+' || kbuf[cp]=='~') {
					if (cp>1 && kbuf[cp - 1]!='_') {
						kbuf.Insert(" ", cp); cp += 2;
						if (cp<=kbuf.Length()) kbuf.Insert(" ", cp++);
					}
					else cp++;
				}
				else cp++;
			}
			gp->Cells[0][i] = kbuf;
			UnicodeString cmd = CurList->ValueFromIndex[i];
			gp->Cells[2][i] = get_CmdDesc(cmd, true, NULL,NULL, USAME_TI(id_str, "V:"));
			gp->Cells[1][i] = del_CmdDesc(cmd);
		}
	} 
	else {
		gp->RowCount = 1;
		clear_GridRow(gp, 0);
	}

	gp->Row = 0;
	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderDrawSection(THeaderControl *HeaderControl,
		THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect, (Section->Index==KeySortMode)? 1 : 0);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(KeyListGrid, KeyListHeader);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	set_GridFromHeader(KeyListHeader, KeyListGrid);
	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//ソート変更
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListHeaderSectionClick(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	KeySortMode = Section->Index;
	KeyTabControlChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SortKeyActionExecute(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	KeySortMode = ap->Tag;
	KeyTabControlChange(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SortKeyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Checked = (ap->Tag==KeySortMode);
}

//---------------------------------------------------------------------------
//セルの描画
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin2(cv);
	UnicodeString cellstr = gp->Cells[ACol][ARow];
	bool use_fgsel = is_SelFgCol(State);

	cv->Brush->Color = State.Contains(gdSelected)? (gp->Focused()? col_selItem : col_oppItem) : col_bgList;
	cv->FillRect(Rect);
	cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;

	//キー
	if (ACol==0) {
		// + と ~ をシンボル色で強調表示
		std::unique_ptr<TStringList> em_lst(new TStringList());
		em_lst->Text = "+\r\n~\r\n";
		EmphasisTextOut(cellstr, em_lst.get(), cv, xp, yp, true, false,
			use_fgsel? col_fgSelItem : col_Symbol, cv->Brush->Color);
	}
	//コマンド
	else if (ACol==1) {
		cellstr = minimize_str(cellstr, gp->Canvas, gp->ColWidths[1] - 8, true);
		if (starts_Dollar(cellstr)) {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_Headline;
			cv->TextRect(Rect, xp, yp, cellstr);
		}
		else {
			cv->Font->Color = use_fgsel? col_fgSelItem : col_Strings;
			EmphasisTextOut(cellstr, get_CmdStr(cellstr), cv, xp, yp, false, true,
				use_fgsel? col_fgSelItem : col_Reserved, cv->Brush->Color);
		}
	}
	//説明
	else {
		cv->TextRect(Rect, xp, yp, cellstr);
	}

	//カーソル
	draw_GridCursor(gp, Rect, ARow, State);

	//候補確定
	if (gp->RowCount==1 && ARow==0 && ACol==1 && !cellstr.IsEmpty()) {
		cv->Brush->Color = col_Cursor;
		cv->FrameRect(Rect);
	}
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::MigemoCheckBoxClick(TObject *Sender)
{
	KeyTabControlChange(NULL);
}

//---------------------------------------------------------------------------
//フィルタによる一覧の更新
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditChange(TObject *Sender)
{
	KeyTabControlChange(NULL);

	//確定即実行
	TStringGrid *gp = KeyListGrid;
	if (CnfExeCheckBox->Checked && FilterEdit->Focused() && !FilterEdit->Text.IsEmpty()
		&& gp->RowCount==1 && !gp->Cells[1][0].IsEmpty())
	{
		Application->ProcessMessages();  Sleep(500);
		CommandStr	= gp->Cells[1][0];
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditEnter(TObject *Sender)
{
	FilterEdit->Color = scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditExit(TObject *Sender)
{
	CloseIME(Handle);
	InvColIfEmpty(FilterEdit);
}

//---------------------------------------------------------------------------
//フィルタ欄でのキー操作
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if		(contained_wd_i(KeysStr_ToList, KeyStr))	KeyListGrid->SetFocus();
	else if (MovGridFromFilter(KeyListGrid, KeyStr))	;
	else if (SameText(KeyStr, KeyStr_Migemo))			MigemoCheckBox->Checked = !MigemoCheckBox->Checked;
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
//フィルタからEnterキーで選択項目を実行
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FilterEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key)) {
		Key = 0;
	}
	else if (Key==VK_RETURN) {
		if (KeyListGrid->Row!=-1) {
			CommandStr	= KeyListGrid->Cells[1][KeyListGrid->Row];
			ModalResult = mrOk;
		}
		Key = 0;
	}
}

//---------------------------------------------------------------------------
//未登録コマンドも表示
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::ShowAllCmdCheckBoxClick(TObject *Sender)
{
	KeyTabControlChange(NULL);
	KeyListGrid->SetFocus();
}

//---------------------------------------------------------------------------
//一覧でのキー操作
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	//確定
	if (equal_ENTER(KeyStr)) {
		CommandStr	= gp->Cells[1][gp->Row];
		ModalResult = mrOk;
	}
	//カーソル移動
	else if (GridCursorMove(cmd_F, gp) || GridCursorMove(cmd_V, gp))	;
	else if (StartsText("IncSearch", cmd_F))	FilterEdit->SetFocus();
	//閉じる
	else if (USAME_TI(cmd_F, "ReturnList"))	ModalResult = mrCancel;
	//タブ切り換え
	else if (is_ToRightOpe(KeyStr, cmd_F)) {
		KeyTabControl->TabIndex = (KeyTabControl->TabIndex + 1) % KeyTabControl->Tabs->Count;
		KeyTabControlChange(NULL);
	}
	else if (is_ToLeftOpe(KeyStr, cmd_F)) {
		KeyTabControl->TabIndex = (KeyTabControl->TabIndex>0) ? KeyTabControl->TabIndex - 1
															  : KeyTabControl->Tabs->Count - 1;
		KeyTabControlChange(NULL);
	}
	//行コピー
	else if (USAME_TI(cmd_V, "ClipCopy")) {
		UnicodeString ibuf = get_tkn_r(CurList->Names[gp->Row], ':');
		UnicodeString cmd  = CurList->ValueFromIndex[gp->Row];
		UnicodeString dsc  = get_CmdDesc(cmd, true);
		cmd = del_CmdDesc(cmd);
		copy_to_Clipboard(ibuf.cat_sprintf(_T("\t%s\t%s"), cmd.c_str(), dsc.c_str()));
	}
	//頭文字サーチ
	else if (is_IniSeaKey(KeyStr)) {	//KeyStr に正規表現パターンが返る
		TRegExOptions opt; opt << roIgnoreCase;
		int idx0=0, idx1=0;
		for (int i=0; i<gp->RowCount && idx1==0; i++) {
			if (i<=gp->Row && idx0!=0) continue;
			if (TRegEx::IsMatch(gp->Cells[0][i], KeyStr, opt)) ((i<=gp->Row)? idx0 : idx1) = i;
		}
		int rn = (idx1>0)? idx1 : idx0;
		if (rn>0) gp->Row = rn;
	}
	//右クリックメニュー
	else if (StartsText("ContextMenu", cmd_F)) show_PopupMenu(gp);

	if (!is_DialogKey(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::KeyListGridDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//現在の一覧内容を取得(TAB区切り)
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::GetStrList(TStringList *lst)
{
	lst->Add("キー\tコマンド\t説明");
	for (int i=0; i<CurList->Count; i++) {
		UnicodeString ibuf = get_tkn_r(CurList->Names[i], ':');
		UnicodeString cmd  = CurList->ValueFromIndex[i];
		UnicodeString dsc  = get_CmdDesc(cmd, true);
		cmd = del_CmdDesc(cmd);
		lst->Add(ibuf.cat_sprintf(_T("\t%s\t%s"), cmd.c_str(), dsc.c_str()));
	}
}

//---------------------------------------------------------------------------
//一覧をコピー
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::CopyListItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> cbuf(new TStringList());
	GetStrList(cbuf.get());
	copy_to_Clipboard(cbuf->Text);
}
//---------------------------------------------------------------------------
//一覧をファイルに保存
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::SaveAsListItemClick(TObject *Sender)
{
	UserModule->SaveTxtDlg->Title		  = LoadUsrMsg(USTR_SaveAs, _T("キー割り当て一覧"));
	UserModule->SaveTxtDlg->InitialDir	  = ExcludeTrailingPathDelimiter(SaveTxtPath);
	UserModule->SaveTxtDlg->EncodingIndex = SaveEncIndex;
	if (UserModule->SaveTxtDlg->Execute()) {
		SaveTxtPath  = ExtractFilePath(UserModule->SaveTxtDlg->FileName);
		SaveEncIndex = UserModule->SaveTxtDlg->EncodingIndex;

		std::unique_ptr<TStringList> fbuf(new TStringList());
		GetStrList(fbuf.get());
		if (!saveto_TextFile(UserModule->SaveTxtDlg->FileName, fbuf.get(), SaveEncIndex))
			msgbox_ERR(LoadUsrMsg(USTR_FaildSave, _T("キー割り当て一覧")));
	}

	KeyListGrid->SetFocus();
}
//---------------------------------------------------------------------------
//コマンドをコピー
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::CopyCmdItemClick(TObject *Sender)
{
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count)
		copy_to_Clipboard(del_CmdDesc(CurList->ValueFromIndex[idx]));
}
//---------------------------------------------------------------------------
//コマンドのヘルプ
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::HelpCmdActionExecute(TObject *Sender)
{
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count) {
		UnicodeString kwd = get_CmdStr(CurList->ValueFromIndex[idx]);
		UnicodeString topic;
		switch (KeyTabControl->TabIndex) {
		case  1: topic.UCAT_T(HELPTOPIC_IS);	break;	//テキストビュアー
		case  2: topic.UCAT_T(HELPTOPIC_TV);	break;	//テキストビュアー
		case  3: topic.UCAT_T(HELPTOPIC_IV);	break;	//イメージビュアー
		case  4: topic.UCAT_T(HELPTOPIC_CILW);	break;	//ログ
		default: topic.UCAT_T(HELPTOPIC_FL);	break;	//ファイラー
		}

		if (topic.Pos('#')==0 && !kwd.IsEmpty()) topic.cat_sprintf(_T("#%s"), kwd.c_str());
		HtmlHelpTopic(topic.c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::HelpCmdActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	int idx = KeyListGrid->Row;
	if (idx>=0 && idx<CurList->Count) {
		UnicodeString kwd = CurList->ValueFromIndex[idx];
		ap->Enabled = (!kwd.IsEmpty() && !starts_AT(kwd) && !starts_Dollar(kwd));
	}
	else {
		ap->Enabled = false;
	}
}

//---------------------------------------------------------------------------
//オプション設定へ
//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::OptionItemClick(TObject *Sender)
{
	CommandStr.USET_T("OptionDlg_KY");
	ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TKeyListDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

