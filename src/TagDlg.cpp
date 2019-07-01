//----------------------------------------------------------------------//
// NyanFi																//
//  タグ設定/検索														//
//  フォルダアイコン検索												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <memory>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpExDlg.h"
#include "TagDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTagManDlg *TagManDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TTagManDlg::TTagManDlg(TComponent* Owner)
	: TForm(Owner)
{
	MaxTagWidth  = 0;
	InhEdit 	 = false;
	IsFolderIcon = false;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, TagCheckListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	SwatchPanel = new UsrSwatchPanel(this);
	SwatchPanel->Parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::FormShow(TObject *Sender)
{
	DlgInitialized = false;
	IsFolderIcon   = SameText(CmdStr, "FindFolderIcon");

	Constraints->MinWidth = 0;

	if (IsFolderIcon)
		IniFile->LoadPosInfo(this, DialogCenter, "FolderIconDlg");
	else
		IniFile->LoadPosInfo(this, DialogCenter);

	SwatchPanel->SetPanelSize16x8(15);
	SwatchPanel->Left = ClientWidth  - SwatchPanel->Width;
	SwatchPanel->Top  = ClientHeight - SwatchPanel->Height;

	RevColCheckBox->Checked = RevTagCololr;

	TCheckListBox *lp = TagCheckListBox;

	set_StdListBox(lp);
	if (!IsFolderIcon && RevTagCololr) lp->ItemHeight = lp->ItemHeight + Scaled2;

	if (IsFolderIcon) {
		std::unique_ptr<TStringList> lst(new TStringList());
		get_FolderIconList(lst.get());
		lp->Items->Assign(lst.get());
		MaxTagWidth = 0;
		for (int i=0; i<lst->Count; i++) {
			MaxTagWidth = std::max(MaxTagWidth, lp->Canvas->TextWidth(get_base_name(lp->Items->Strings[i])));
		}
		MaxTagWidth += ScaledInt(32);
	}
	else {
		MaxTagWidth = usr_TAG->IniCheckList(TagCheckListBox) + ScaledInt(32);
	}

	set_UsrScrPanel(ListScrPanel);
	ListScrPanel->UpdateKnob();

	set_BtnMarkDark(HideOptBtn, UBMK_BDOWN);
	set_BtnMarkDark(ShowOptBtn, UBMK_BUP);
	HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
	ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);
	TagEdit->Hint	 = LoadUsrMsg(USTR_HintMltSepSC);
	SetDarkWinTheme(TagEdit);

	AndPanel->Visible	  = false;
	SelMaskPanel->Visible = false;
	ResLinkPanel->Visible = false;
	HidePanel->Visible	  = false;

	FindOptPanel->Visible = false;
	OptPanel->Height	  = FindOptPanel->Height + InpOptPanel->Height;

	if (IsFolderIcon) {
		set_FormTitle(this, _T("フォルダアイコン検索"));
		FindOptPanel->Visible	 = true;
		ResLinkCheckBox->Checked = IniFile->ReadBoolGen(_T("FindIcoResLink"));
		ResLinkPanel->Visible	 = true;
		OptPanel->Visible		 = IniFile->ReadBoolGen(_T("FindIcoShowOpt"),	true);
		SetColPanel->Visible	 = false;
	}
	else if (SameText(CmdStr, "FindTag")) {
		FindOptPanel->Visible	 = true;
		AndCheckBox->Checked	 = IniFile->ReadBoolGen(_T("FindTagDlgAnd"),	true);
		AndPanel->Visible		 = true;
		ResLinkCheckBox->Checked = IniFile->ReadBoolGen(_T("FindTagResLink"));
		ResLinkPanel->Visible	 = true;
		OptPanel->Visible		 = IniFile->ReadBoolGen(_T("FindTagShowOpt"),	true);
		HideCheckBox->Checked	 = IniFile->ReadBoolGen(_T("FindTagDlgHide"));
		HidePanel->Visible		 = true;
		Caption = UnicodeString().cat_sprintf(_T("タグ検索 (%s)"), AndCheckBox->Checked? _T("AND") : _T("OR"));
	}
	else if (SameText(CmdStr, "SetTag")) {
		set_FormTitle(this, _T("タグの設定"));
		OptPanel->Height	  = InpOptPanel->Height;
		OptPanel->Visible	  = IniFile->ReadBoolGen(_T("SetTagShowOpt"),	true);
		HideCheckBox->Checked = IniFile->ReadBoolGen(_T("SetTagDlgHide"));
		HidePanel->Visible	  = true;

		TStringDynArray org_lst = split_strings_semicolon(TagEdit->Text);
		for (int i=0; i<org_lst.Length; i++) {
			int idx = TagCheckListBox->Items->IndexOf(org_lst[i]);
			if (idx!=-1) TagCheckListBox->Checked[idx] = true;
		}
	}
	else if (SameText(CmdStr, "AddTag")) {
		set_FormTitle(this, _T("タグの追加"));
		OptPanel->Height	  = InpOptPanel->Height;
		OptPanel->Visible	  = IniFile->ReadBoolGen(_T("AddTagShowOpt"),	true);
		HideCheckBox->Checked = IniFile->ReadBoolGen(_T("AddTagDlgHide"));
		HidePanel->Visible	  = true;
	}
	else if (SameText(CmdStr, "TagSelect")) {
		set_FormTitle(this, _T("タグ選択"));
		FindOptPanel->Visible	 = true;
		AndCheckBox->Checked	 = IniFile->ReadBoolGen(_T("TagSelDlgAnd"),	true);
		AndPanel->Visible		 = true;
		SelMaskPanel->Visible	 = (ScrMode==SCMD_FLIST);
		SelMaskCheckBox->Checked = IniFile->ReadBoolGen(_T("TagSelSelMask"));
		OptPanel->Visible		 = IniFile->ReadBoolGen(_T("TagSelShowOpt"),	true);
		HideCheckBox->Checked	 = IniFile->ReadBoolGen(_T("TagSelDlgHide"));
		HidePanel->Visible		 = true;
	}

	SetColPanel->Visible = !IsFolderIcon && OptPanel->Visible;
	BlankPanel->Visible  = !OptPanel->Visible;
	SetDarkWinTheme(SetColPanel);
	SetDarkWinTheme(OptPanel);
	SetDarkWinTheme(BlankPanel);

	TagCheckListBox->PopupMenu	= IsFolderIcon? NULL : TagPopupMenu;
	ShowTagCountAction->Checked = IniFile->ReadBoolGen(_T("TagDlgShowCount"));

	SetOptBtn();

	InpPanel->Visible = HidePanel->Visible && !HideCheckBox->Checked;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::WmFormShowed(TMessage &msg)
{
	if (!IsFolderIcon && ShowTagCountAction->Checked) {
		Repaint();
		usr_TAG->CountTags(TagCheckListBox);
	}

	SetCtrlFocus();
	DlgInitialized = true;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	CloseIME(Handle);

	if (IsFolderIcon)
		IniFile->SavePosInfo(this, "FolderIconDlg");
	else
		IniFile->SavePosInfo(this);

	if (IsFolderIcon) {
		IniFile->WriteBoolGen(_T("FindIcoResLink"),		ResLinkCheckBox);
		IniFile->WriteBoolGen(_T("FindIcoShowOpt"),		OptPanel->Visible);

		if (ModalResult==mrOk) {
			TCheckListBox *lp = TagCheckListBox;
			if (get_CheckListCount(lp)==0 && lp->ItemIndex!=-1)
				lp->Checked[lp->ItemIndex] = true;
		}
	}
	else if (SameText(CmdStr, "FindTag")) {
		IniFile->WriteBoolGen(_T("FindTagDlgHide"),		HideCheckBox);
		IniFile->WriteBoolGen(_T("FindTagDlgAnd"),		AndCheckBox);
		IniFile->WriteBoolGen(_T("FindTagResLink"),		ResLinkCheckBox);
		IniFile->WriteBoolGen(_T("FindTagShowOpt"),		OptPanel->Visible);
	}
	else if (SameText(CmdStr, "SetTag")) {
		IniFile->WriteBoolGen(_T("SetTagDlgHide"),		HideCheckBox);
		IniFile->WriteBoolGen(_T("SetTagShowOpt"),		OptPanel->Visible);
	}
	else if (SameText(CmdStr, "AddTag")) {
		IniFile->WriteBoolGen(_T("AddTagDlgHide"),		HideCheckBox);
		IniFile->WriteBoolGen(_T("AddTagShowOpt"),		OptPanel->Visible);
	}
	else if (SameText(CmdStr, "TagSelect")) {
		IniFile->WriteBoolGen(_T("TagSelDlgHide"),		HideCheckBox);
		IniFile->WriteBoolGen(_T("TagSelDlgAnd"),		AndCheckBox);
		IniFile->WriteBoolGen(_T("TagSelSelMask"),		SelMaskCheckBox);
		IniFile->WriteBoolGen(_T("TagSelShowOpt"),		OptPanel->Visible);
	}

	IniFile->WriteBoolGen(_T("TagDlgShowCount"),	ShowTagCountAction);

	DlgInitialized = false;
	CmdStr = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TTagManDlg::ListPanelResize(TObject *Sender)
{
	ListScrPanel->UpdateKnob();
	SetOptBtn();
}

//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SetCtrlFocus()
{
	if (InpPanel->Visible) {
		TagEdit->SetFocus();
		TagEdit->SelStart = TagEdit->Text.Length();
	}
	else {
		TagCheckListBox->SetFocus();
		if (TagCheckListBox->Count>0) {
			if (TagCheckListBox->ItemIndex==-1) TagCheckListBox->ItemIndex = 0;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TTagManDlg::HideActionExecute(TObject *Sender)
{
	if (!HidePanel->Visible) return;
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	InpPanel->Visible = !IsFolderIcon && !ap->Checked;
	SetCtrlFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::AndActionExecute(TObject *Sender)
{
	if (!AndPanel->Visible) return;
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	if (SameText(CmdStr, "FindTag")) {
		Caption = UnicodeString().cat_sprintf(_T("タグ検索 (%s)"), AndCheckBox->Checked? _T("AND") : _T("OR"));
	}
	SetCtrlFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::ResLinkActionExecute(TObject *Sender)
{
	if (!ResLinkPanel->Visible) return;
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	SetCtrlFocus();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SelMaskActionExecute(TObject *Sender)
{
	if (!SelMaskPanel->Visible) return;
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	SetCtrlFocus();
}

//---------------------------------------------------------------------------
//設定パネルの開閉
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SetOptBtn()
{
	if (BlankPanel->Visible) {
		if (ScrBarStyle==0)
			set_ControlRBCorner(TagCheckListBox, BlankPanel);
		else
			ListScrPanel->SetRBCornerPanel(BlankPanel);
	}

}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::ChgOptBtnClick(TObject *Sender)
{
	OptPanel->Visible	 = !OptPanel->Visible;
	BlankPanel->Visible  = !OptPanel->Visible;
	SetColPanel->Visible = !IsFolderIcon && OptPanel->Visible;

	Constraints->MinWidth = OptPanel->Visible? (IsFolderIcon? 240 : 320) : 160;

	SetOptBtn();
}

//---------------------------------------------------------------------------
//入力欄での操作
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagEditChange(TObject *Sender)
{
	if (!DlgInitialized || InhEdit) return;

	TStringDynArray org_lst = split_strings_semicolon(TagEdit->Text);
	std::unique_ptr<TStringList> lst(new TStringList());
	for (int i=0; i<org_lst.Length; i++)
		if (!org_lst[i].IsEmpty()) lst->Add(org_lst[i]);

	TCheckListBox *lp = TagCheckListBox;
	for (int i=0; i<lp->Count; i++) {
		lp->Checked[i] = (lst->IndexOf(lp->Items->Strings[i])!=-1);
		if (lp->Checked[i]) lp->ItemIndex = i;
	}

	TagCheckListBoxClickCheck(NULL);
	TagCheckListBox->Invalidate();

	if (InpPanel->Visible) {
		TagEdit->SetFocus();
		TagEdit->SelStart = TagEdit->Text.Length();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (KeyStr.IsEmpty()) return;

	if (contained_wd_i(KeysStr_ToList, KeyStr)) {
		TCheckListBox *lp = TagCheckListBox;
		if (lp->Count>0) {
			lp->SetFocus();
			if (lp->ItemIndex==-1) lp->ItemIndex = 0;
		}
	}
	else return;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
//タグ一覧での操作
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxClickCheck(TObject *Sender)
{
	if (!DlgInitialized) return;

	UnicodeString lbuf = usr_TAG->CheckToTags(TagCheckListBox, TagEdit->Text);
	if (!SameStr(lbuf, TagEdit->Text)) {
		InhEdit = true;
		TagEdit->Text = lbuf;
		InhEdit = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxClick(TObject *Sender)
{
	TagCheckListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxDblClick(TObject *Sender)
{
	if (IsFolderIcon) perform_Key_RETURN((TControl*)Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	TCheckListBox *lp = TagCheckListBox;

	if (contained_wd_i(KeysStr_CsrDown, KeyStr)) {
		Key = VK_DOWN;
	}
	else if (contained_wd_i(KeysStr_CsrUp, KeyStr)) {
		Key = VK_UP;
	}
	//タグの改名
	else if (USAME_TI(cmd_F, "RenameDlg")) {
		RenTagAction->Execute();
	}
	//タグの削除
	else if (USAME_TI(cmd_F, "Delete")) {
		DelTagAction->Execute();
	}
	//全チェック/解除
	else if (lp->Count>0 && StartsText("SelAll", cmd_F)) {
		lp->CheckAll((get_CheckListCount(lp)>0)? cbUnchecked : cbChecked);
		lp->Invalidate();
		TagCheckListBoxClickCheck(NULL);
	}
	//頭文字サーチ
	else if (is_IniSeaKey(KeyStr)) {
		int idx = lp->ItemIndex;
		int idx0=-1, idx1=-1;
		TRegExOptions opt; opt << roIgnoreCase;
		for (int i=0; i<lp->Count && idx1==-1; i++) {
			if (i<=idx && idx0!=-1) continue;
			UnicodeString lbuf = lp->Items->Strings[i];
			if (IsFolderIcon) lbuf = get_base_name(lbuf);
			if (TRegEx::IsMatch(lbuf, KeyStr, opt)) ((i<=idx)? idx0 : idx1) = i;
		}
		idx = (idx1!=-1)? idx1 : idx0;
		if (idx!=-1) {
			lp->ItemIndex = idx;
			lp->Invalidate();
		}
	}
	//右クリックメニュー
	else if (StartsText("ContextMenu", cmd_F)) {
		show_PopupMenu(lp);
	}

	if (!is_DialogKey(Key)) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	Key = 0;
}

//---------------------------------------------------------------------------
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagCheckListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = lp->Checked[Index]? col_selItem : col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);

	//フォルダアイコン
	if (IsFolderIcon) {
		UnicodeString inam = lp->Items->Strings[Index];
		draw_SmallIcon2(inam, cv, xp, yp);
		xp += ScaledInt(24);
		cv->Font->Color = get_ExtColor(get_extension(inam));
		cv->TextOut(xp, yp, get_base_name(inam));
	}
	//タグ
	else {
		if (RevTagCololr) yp += Scaled2;
		usr_TAG->DrawTags(lp->Items->Strings[Index], cv, xp, yp,
			(RevTagCololr? col_bgList : clNone), UserModule->SpuitEnabled());
	}

	//使用数
	int n = (int)lp->Items->Objects[Index];
	if (n>0) {
		cv->Font->Color = col_fgList;
		UnicodeString lbuf = n;
		xp += (MaxTagWidth + cv->TextWidth("99999999") - cv->TextWidth(lbuf));
		cv->TextOut(xp, yp, lbuf);
	}

	//カーソル
	if (Index==lp->ItemIndex) {
		int lw = lp->Focused()? std::max(CursorWidth, 1) : 1;
		int yp = Rect.Bottom - lw;
		draw_Line(lp->Canvas, Rect.Left, yp, Rect.Right, yp, lw, col_Cursor, lp->Focused()? psSolid : psDot);
	}

	if (State.Contains(odFocused)) lp->Canvas->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------
//タグ名の変更
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::RenTagActionExecute(TObject *Sender)
{
	if (TagCheckListBox->ItemIndex==-1) return;

	UnicodeString old_tag = TagCheckListBox->Items->Strings[TagCheckListBox->ItemIndex];
	UnicodeString new_tag = old_tag;
	if (input_query_ex(_T("タグ名の変更"), _T("タグ名"), &new_tag)
		&& !new_tag.IsEmpty() && !SameStr(old_tag, new_tag))
	{
		int new_idx = TagCheckListBox->Items->IndexOf(new_tag);
		int old_idx = TagCheckListBox->Items->IndexOf(old_tag);
		if (new_idx!=-1 && new_idx!=old_idx && !msgbox_Sure(_T("既存の同名タグに統合しますか？"))) return;

		int cnt = usr_TAG->RenTag(old_tag, new_tag);
		MaxTagWidth = usr_TAG->IniCheckList(TagCheckListBox, new_tag, ShowTagCountAction->Checked);
		ListScrPanel->UpdateKnob();
		if (cnt>0) msgbox_OK(UnicodeString().sprintf(_T("%u項目のタグ設定を更新しました。"), cnt));
	}
}
//---------------------------------------------------------------------------
//タグ名の削除
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::DelTagActionExecute(TObject *Sender)
{
	if (TagCheckListBox->ItemIndex==-1) return;

	UnicodeString tag = TagCheckListBox->Items->Strings[TagCheckListBox->ItemIndex];
	UnicodeString msg; msg.sprintf(_T("[%s] をタグデータから削除しますか?"), tag.c_str());
	if (msgbox_Sure(msg)) {
		int cnt = usr_TAG->DelTagData(tag);
		MaxTagWidth = usr_TAG->IniCheckList(TagCheckListBox, EmptyStr, ShowTagCountAction->Checked);
		ListScrPanel->UpdateKnob();
		TagEdit->Text = EmptyStr;
		if (cnt>0) msgbox_OK(UnicodeString().sprintf(_T("%u項目のタグを削除しました。"), cnt));
	}
}
//---------------------------------------------------------------------------
//タグ色の設定
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SetColorActionExecute(TObject *Sender)
{
	if (TagCheckListBox->ItemIndex==-1) return;

	UnicodeString tag = TagCheckListBox->Items->Strings[TagCheckListBox->ItemIndex];
	UserModule->ColorDlg->Color = usr_TAG->GetColor(tag, col_fgTagNam);
	if (UserModule->ColorDlg->Execute()) {
		usr_TAG->SetColor(tag, UserModule->ColorDlg->Color);
		TagCheckListBox->Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::DefColorActionExecute(TObject *Sender)
{
	if (TagCheckListBox->ItemIndex==-1) return;

	usr_TAG->SetColor(TagCheckListBox->Items->Strings[TagCheckListBox->ItemIndex], clNone);
	TagCheckListBox->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SelActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (TagCheckListBox->ItemIndex!=-1);

	if (!UserModule->SpuitEnabled()) SpuitImage->Visible = (TagCheckListBox->ItemIndex!=-1);
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::RefTagColBtnClick(TObject *Sender)
{
	SetColorAction->Execute();
}

//---------------------------------------------------------------------------
//タグの使用数をカウント
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::CountTagActionExecute(TObject *Sender)
{
	usr_TAG->CountTags(TagCheckListBox);
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::CountTagActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (TagCheckListBox->Count>0 && !ShowTagCountAction->Checked);
}

//---------------------------------------------------------------------------
//タグの使用数を表示
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::ShowTagCountActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	usr_TAG->CountTags(TagCheckListBox, !ap->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::ShowTagCountActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (TagCheckListBox->Count>0);
}

//---------------------------------------------------------------------------
//存在しない項目のデータ削除
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TrimDataActionExecute(TObject *Sender)
{
	if (msgbox_Sure(LoadUsrMsg(USTR_DeleteQ, _T("存在しない項目のデータ")), SureDelete)) {
		cursor_HourGlass();
		int cnt = usr_TAG->TrimData();
		cursor_Default();

		UnicodeString msg;
		if (cnt>0)
			msg.sprintf(_T("%u 個のデータを削除しました。"), cnt);
		else
			msg = "存在しない項目のデータはありませんでした。";
		msgbox_OK(msg);
	}
}

//---------------------------------------------------------------------------
//検索動作をコマンドファイルとして保存
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::TagPopupMenuPopup(TObject *Sender)
{
	MakeNbtItem->Visible = SameText(CmdStr, "FindTag");
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::MakeNbtActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	UserModule->PrepareSaveDlg(_T("コマンドファイルとして保存"), F_FILTER_NBT, NULL, CmdFilePath);
	if (UserModule->SaveDlg->Execute()) {
		CmdFilePath = ExtractFilePath(UserModule->SaveDlg->FileName);
		std::unique_ptr<TStringList> fbuf(new TStringList());
		UnicodeString kwd = TagEdit->Text;
		if (!AndCheckBox->Checked) kwd = ReplaceStr(kwd, ";", "|");
		UnicodeString tmp;
		fbuf->Add(tmp.sprintf(_T(";タグ検索 [%s]"), kwd.c_str()));
		if (ap->Tag==1) fbuf->Add("ToOpposite");
		fbuf->Add(tmp.sprintf(_T("FindTag_%s"), kwd.c_str()));
		if (!saveto_TextUTF8(UserModule->SaveDlg->FileName, fbuf.get())) msgbox_ERR(USTR_FaildSave);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::MakeNbtActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Visible = SameText(CmdStr, "FindTag");
	ap->Enabled = ap->Visible && !TagEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//スポイト
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	TCheckListBox *lp = TagCheckListBox;
	if (Button!=mbLeft || lp->ItemIndex==-1) return;

	//スウォッチブック
	SetCustomColToSwatch(UserModule->ColorDlg->CustomColors);
	SwatchPanel->Visible = true;
	SwatchPanel->BringToFront();

	TColor col_def = usr_TAG->GetColor(lp->Items->Strings[lp->ItemIndex], clNone);
	UserModule->BeginSpuit(SpuitPanel, (TImage*)Sender, col_def);
	lp->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::SpuitImageMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	SwatchPanel->Visible = false;

	if (UserModule->SpuitEnabled()) {
		TColor col = UserModule->EndSpuit();
		TCheckListBox *lp = TagCheckListBox;
		if (lp->ItemIndex!=-1) usr_TAG->SetColor(lp->Items->Strings[lp->ItemIndex], col);
		lp->Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::RevColActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	RevTagCololr = ap->Checked;
	TagCheckListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTagManDlg::CanButtonClick(TObject *Sender)
{
	if (UserModule->SpuitEnabled()) {
		SwatchPanel->Visible = false;
		UserModule->EndSpuit();
		TagCheckListBox->Repaint();
		ModalResult = mrNone;
	}
	else {
		ModalResult = mrCancel;
	}
}

//---------------------------------------------------------------------------
void __fastcall TTagManDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if (equal_ENTER(KeyStr)) {
		ModalResult = mrOk;
	}
	else if (equal_ESC(KeyStr)) {
		if (UserModule->SpuitEnabled()) {
			SwatchPanel->Visible = false;
			UserModule->EndSpuit();
			TagCheckListBox->Repaint();
		}
		else {
			ModalResult = mrCancel;
		}
	}
	else if (USAME_TI(KeyStr, "Alt+A")) {
		AndAction->Execute();
	}
	else if (USAME_TI(KeyStr, "Alt+M")) {
		SelMaskAction->Execute();
	}
	else if (USAME_TI(KeyStr, "Alt+H")) {
		HideAction->Execute();
	}
	else if (USAME_TI(KeyStr, "Alt+R")) {
		ResLinkAction->Execute();
	}
	else if (USAME_TI(KeyStr, "Alt+O")) {
		ChgOptBtnClick(NULL);
	}
	else {
		UnicodeString topic = HELPTOPIC_FL;
		topic.cat_sprintf(_T("#%s"), CmdStr.c_str());
		SpecialKeyProc(this, Key, Shift, topic.c_str());
	}
}
//---------------------------------------------------------------------------

