//----------------------------------------------------------------------//
// NyanFi																//
//  JSONビュアー														//
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
#include "JsonView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TJsonViewer *JsonViewer = NULL;

//---------------------------------------------------------------------------
__fastcall TJsonViewer::TJsonViewer(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;
	isClip = false;
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);

	AutoCheckBox->Checked = IniFile->ReadBoolGen(_T("JsonViewAutoExp"));
	NamCheckBox->Checked  = IniFile->ReadBoolGen(_T("JsonViewFindName"),	true);
	ValCheckBox->Checked  = IniFile->ReadBoolGen(_T("JsonViewFindVal"),	true);

	JsonTreeView->Items->Clear();
	JsonTreeView->Color = col_bgList;
	JsonTreeView->Font->Assign(ListFont);

	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);

	Caption = (isClip? UnicodeString("クリップボード") : yen_to_delimiter(FileName)) + " - JSONビュアー";

	SetDarkWinTheme(this);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::WmFormShowed(TMessage &msg)
{
	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);
	Repaint();

	ErrMsg = EmptyStr;

	ViewBusy = true;
	set_RedrawOff(JsonTreeView);
	try {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		if (isClip) {
			if (!Clipboard()->HasFormat(CF_TEXT)) UserAbort(USTR_NoObject);
			fbuf->Text = Clipboard()->AsText;
		}
		else {
			if (load_text_ex(FileName, fbuf.get())==0) UserAbort(USTR_FileNotOpen);
			if (Trim(fbuf->Text).IsEmpty()) UserAbort(USTR_NoObject);
		}
		JsonTreeView->SetFocus();
		AssignJsonView(fbuf->Text, NULL);

		TTreeNode *TopNode = JsonTreeView->Items->GetFirstNode();
		TopNode->Selected  = true;
		TopNode->Expanded  = true;
	}
	catch (Exception &e) {
		ErrMsg = e.Message;
	}
	set_RedrawOn(JsonTreeView);
	ViewBusy = false;

	if (!ErrMsg.IsEmpty()) {
		JsonTreeView->Items->AddChildObject(NULL, "ERROR: " + LoadUsrMsg(USTR_FaildLoad), (void*)-1);
		msgbox_ERR(ErrMsg);
		StatusBar1->Panels->Items[0]->Text = ErrMsg;
	}
}

//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FormClose(TObject *Sender, TCloseAction &Action)
{
	JsonTreeView->Items->Clear();
	FileName = EmptyStr;
	isClip	 = false;

	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("JsonViewAutoExp"),	AutoCheckBox);
	IniFile->WriteBoolGen(_T("JsonViewFindName"),	NamCheckBox);
	IniFile->WriteBoolGen(_T("JsonViewFindVal"),	ValCheckBox);
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TJsonViewer::get_JsonValStr(TJSONValue *val)
{
	return
		val->ClassNameIs("TJSONTrue")  ? UnicodeString("true") :
		val->ClassNameIs("TJSONFalse") ? UnicodeString("false") :
		val->ClassNameIs("TJSONNull")  ? UnicodeString("null") :
		val->ClassNameIs("TJSONString")? ("\"" + val->Value() + "\"") : val->Value();
}
//---------------------------------------------------------------------------
//ツリービューに割り当て
//	表示文字列 [TAB] JSON文字列 で登録
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::AssignJsonView(UnicodeString s, TTreeNode *np)
{
	TJSONValue *val = dynamic_cast<TJSONValue*>(TJSONObject::ParseJSONValue(s));

	if (!np) {
		JsonTreeView->Items->Clear();
		np = JsonTreeView->Items->AddChildObject(NULL, "JSON\t" + (val? val->ToJSON() : EmptyStr), (void*)1);	//1=ノード
	}

	if (!val) return;

	UnicodeString tmp;
	if (val->ClassNameIs("TJSONObject")) {
		TJSONObject* obj = dynamic_cast<TJSONObject*>(val);
		if (obj) {
			for (int i=0; i<obj->Count; i++) {
				TJSONPair* pair = obj->Pairs[i];
				if (pair->JsonValue->ClassNameIs("TJSONObject") || pair->JsonValue->ClassNameIs("TJSONArray")) {
					tmp.sprintf(_T("%s\t%s"), pair->JsonString->Value().c_str(), pair->ToJSON().c_str());
					TTreeNode *sp = JsonTreeView->Items->AddChildObject(np, tmp, (void*)1);	//1=ノード
					AssignJsonView(pair->JsonValue->ToString(), sp);
				}
				else {
					tmp.sprintf(_T("%s:%s\t%s"),
						pair->JsonString->Value().c_str(), get_JsonValStr(pair->JsonValue).c_str(), pair->ToJSON().c_str());
					JsonTreeView->Items->AddChildObject(np, tmp, (void*)0);	//0=値
				}
			}
		}
	}
	else if (val->ClassNameIs("TJSONArray")) {
		TJSONArray* ary = dynamic_cast<TJSONArray*>(val);
		if (ary) {
			for (int i=0; i<ary->Count; i++) {
				TJSONValue *itm = ary->Items[i];
				if (itm->ClassNameIs("TJSONObject") || itm->ClassNameIs("TJSONArray")) {
					tmp.sprintf(_T("[%u]\t%s"), i, itm->ToJSON().c_str());
					TTreeNode *sp = JsonTreeView->Items->AddChildObject(np, tmp, (void*)1);	//1=ノード
					AssignJsonView(itm->ToString(), sp);
				}
				else {
					tmp.sprintf(_T("[%u]:%s\t%s"), i, get_JsonValStr(itm).c_str(), itm->ToJSON().c_str());
					JsonTreeView->Items->AddChildObject(np, tmp, (void*)0);	//0=値
				}
			}
		}
	}
	else {
		tmp.sprintf(_T("%s:%s\t%s"), val->Value().c_str(), get_JsonValStr(val).c_str(), val->ToJSON().c_str());
		JsonTreeView->Items->AddChildObject(np, tmp, (void*)0);	//0=値
	}
}

//---------------------------------------------------------------------------
//ViewBusy プロパティ
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::SetViewBusy(bool Value)
{
	FViewBusy = Value;

	if (Value) {
		cursor_HourGlass();
		StatusBar1->Panels->Items[0]->Text = "処理中...";	StatusBar1->Repaint();
	}
	else {
		StatusBar1->Panels->Items[0]->Text = GetJsonPath();
		cursor_Default();
	}
}

//---------------------------------------------------------------------------
//項目の文字列を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TJsonViewer::GetTreeViewText(
	TTreeNode *Node,
	bool is_json)	//JSON文字列を取得	(default = false)
{
	if (!Node) Node = JsonTreeView->Selected;
	return (Node? (is_json? get_post_tab(Node->Text) : get_pre_tab(Node->Text)) : EmptyStr);
}

//---------------------------------------------------------------------------
//ノードのパスを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TJsonViewer::GetJsonPath(TTreeNode *Node)
{
	UnicodeString ret_str;
	if (!Node) Node = JsonTreeView->Selected;
	if (Node) {
		while (Node && Node->Level>=0) {
			UnicodeString s = (Node->Level==0)? UnicodeString("$") : get_tkn(GetTreeViewText(Node), ':');
			if (Node->Level>0 && (!StartsStr('[', s) || !EndsStr(']', s))) s.Insert(".", 1);
			ret_str.Insert(s, 1);
			Node = Node->Parent;
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//ノードが検索語にマッチするか?
//---------------------------------------------------------------------------
bool __fastcall TJsonViewer::MatchNode(TTreeNode *Node)
{
	if (!Node) return false;

	UnicodeString lbuf = GetTreeViewText(Node);
	UnicodeString s;
	//ノード
	if ((int)Node->Data==1) {
		if (NamCheckBox->Checked) s = lbuf;
	}
	//ペア
	else {
		s = (NamCheckBox->Checked && ValCheckBox->Checked)? lbuf:
			 						  NamCheckBox->Checked? get_tkn(lbuf, ':') :
			 						  ValCheckBox->Checked? get_tkn_r(lbuf, ':') : EmptyStr;
	}

	return ContainsText(s, FindEdit->Text);
}

//---------------------------------------------------------------------------
//リストビューの描画
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::JsonTreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
		TCustomDrawState State, bool &DefaultDraw)
{
	DefaultDraw = false;

	if (ViewBusy) return;

	TRect rc_t = Node->DisplayRect(true);
	if (rc_t.Left==0) return;

	TRect rc_s = Node->DisplayRect(false);
	TCanvas *cv = Sender->Canvas;
	cv->Brush->Color = Node->Selected? col_selItem : col_bgList;
	cv->FillRect(rc_s);
	bool is_irreg = IsIrregularFont(ListFont);

	//テキスト
	UnicodeString lbuf = GetTreeViewText(Node);

	//※Sender->Canvas だとフォント色を途中で変更しても効かないようなので一旦バッファに描画
	std::unique_ptr<Graphics::TBitmap> tmp_bmp(new Graphics::TBitmap());
	tmp_bmp->SetSize(get_TextWidth(cv, lbuf, is_irreg) + 4, rc_t.Height());

	TCanvas *tmp_cv = tmp_bmp->Canvas;
	TRect    tmp_rc	= Rect(0, 0, tmp_bmp->Width, tmp_bmp->Height);
	tmp_cv->Font->Assign(ListFont);
	tmp_cv->Brush->Color = Node->Selected? col_selItem : col_bgList;
	tmp_cv->FillRect(tmp_rc);

	bool is_selfg = (Node->Selected && col_fgSelItem!=clNone);

	int ntyp = (int)Node->Data;
	//ノード
	if (ntyp==1) {
		int xp = 0;
		//名前
		tmp_cv->Font->Color = is_selfg? col_fgSelItem : col_Reserved;
		tmp_cv->TextOut(xp, 0, lbuf);
	}
	//ペア
	else if (ntyp==0) {
		int xp = 0;
		//名前
		TColor fg = is_selfg? col_fgSelItem : col_Reserved;
		out_TextEx(tmp_cv, xp, 0, get_tkn(lbuf, ':'), fg, col_None, 0, is_irreg);
		//:
		fg = is_selfg? col_fgSelItem : col_Symbol;
		out_TextEx(tmp_cv, xp, 0, ": ", fg, col_None, 0, is_irreg);
		//値
		UnicodeString vstr	= get_tkn_r(lbuf, ':');
		fg = is_selfg? col_fgSelItem :
				(StartsStr("\"http", vstr) && TRegEx::Match(vstr, URL_MATCH_PTN).Success)? col_URL :
				is_quot(vstr)? col_Strings : col_Numeric;
		out_TextEx(tmp_cv, xp, 0, vstr, fg, col_None, 0, is_irreg);
	}
	//エラー
	else {
		tmp_cv->Font->Color = col_Error;
		tmp_cv->TextOut(0, 0, lbuf);
	}

	if (State.Contains(cdsSelected)) tmp_cv->DrawFocusRect(tmp_rc);
	//描画
	cv->Draw(rc_t.Left, rc_t.Top, tmp_bmp.get());

	//横スクロール位置を取得
	int scr_p = 0;
	SCROLLINFO si = {sizeof(SCROLLINFO), SIF_TRACKPOS};
	if (::GetScrollInfo(Sender->Handle, SB_HORZ, &si)) scr_p = si.nTrackPos;

	//親ライン
	cv->Pen->Style = psSolid;
	cv->Pen->Width = 1;
	cv->Pen->Color = col_HR;
	rc_s.Right = rc_t.Left;
	int l_ofs  = rc_s.Left + ScaledIntX(11);
	TTreeNode *pp = Node->Parent;
	while (pp) {
		if (pp->getNextSibling()) {
			int xl = JsonTreeView->Indent * pp->Level + l_ofs - scr_p;
			if (xl>=0) {
				cv->MoveTo(xl, rc_s.Top);
				cv->LineTo(xl, rc_s.Bottom);
			}
		}
		if (pp->Level==0) break;
		pp = pp->Parent;
	}

	//プラス/マイナスボタン
	if (Node->HasChildren) {
		cv->Pen->Color	 = col_HR;
		cv->Brush->Color = col_HR;
		int w_btn = ScaledIntX(11);	//ボタンサイズ
		int xp = rc_s.Right - ScaledIntX(w_btn + 5);
		int yp = rc_s.Top + (rc_s.Height() - w_btn)/2;
		if ((xp + w_btn)>=0) {
			//枠
			cv->FrameRect(Rect (xp, yp, xp + w_btn, yp + w_btn));
			//横棒
			cv->MoveTo(xp + 2, yp + w_btn/2);
			cv->LineTo(xp + w_btn - 2, yp + w_btn/2);
			//縦棒
			if (!Node->Expanded) {
				cv->MoveTo(xp + w_btn/2, yp + 2);
				cv->LineTo(xp + w_btn/2, yp + w_btn -2);
			}
		}
	}
	//子ライン
	else if (Node->Level>0) {
		cv->Pen->Color = col_HR;
		int xl = JsonTreeView->Indent * Node->Level + l_ofs - scr_p;
		int x2 = xl + ScaledIntX(8);
		if (x2>=0) {
			if (Node->getNextSibling()) {
				cv->MoveTo(xl, rc_s.Top);
				cv->LineTo(xl, rc_s.Bottom);
				cv->MoveTo(xl, rc_s.Top + rc_s.Height()/2);
				cv->LineTo(x2, rc_s.Top + rc_s.Height()/2);
			}
			else {
				cv->MoveTo(xl, rc_s.Top);
				cv->LineTo(xl, rc_s.Top + rc_s.Height()/2);
				cv->LineTo(x2, rc_s.Top + rc_s.Height()/2);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::JsonTreeViewChange(TObject *Sender, TTreeNode *Node)
{
	StatusBar1->Panels->Items[0]->Text = !ErrMsg.IsEmpty()? ErrMsg : GetJsonPath(Node);
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::JsonTreeViewHint(TObject *Sender, TTreeNode * const Node, UnicodeString &Hint)
{
	Hint = EmptyStr;
}

//---------------------------------------------------------------------------
//ステータスバーの描画
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::StatusBar1DrawPanel(TStatusBar *StatusBar,
	TStatusPanel *Panel, const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect);
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(Rect.Left + ScaledIntX(2), Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//すべて展開
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::ExpandItemClick(TObject *Sender)
{
	ViewBusy = true;
	set_RedrawOff(JsonTreeView);
	{
		JsonTreeView->FullExpand();
		JsonTreeView->TopItem = JsonTreeView->Items->GetFirstNode();
	}
	set_RedrawOn(JsonTreeView);
	ViewBusy = false;
}
//---------------------------------------------------------------------------
//すべて畳む
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CollapseItemClick(TObject *Sender)
{
	JsonTreeView->FullCollapse();
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyActionExecute(TObject *Sender)
{
	copy_to_Clipboard(GetTreeViewText());
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !GetTreeViewText(JsonTreeView->Selected).IsEmpty();
}

//---------------------------------------------------------------------------
//JSONソースをコピー
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyJsonActionExecute(TObject *Sender)
{
	copy_to_Clipboard(GetTreeViewText(JsonTreeView->Selected, true));
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyJsonActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !GetTreeViewText(JsonTreeView->Selected, true).IsEmpty();
}

//---------------------------------------------------------------------------
//XPath をコピー
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyJsonPathActionExecute(TObject *Sender)
{
	copy_to_Clipboard(GetJsonPath(JsonTreeView->Selected));
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CopyJsonPathActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = JsonTreeView->Selected;
}

//---------------------------------------------------------------------------
//URLを開く
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::OpenUrlActionExecute(TObject *Sender)
{
	if (JsonTreeView->Selected) Execute_ex(extract_URL(exclude_quot(get_tkn_r(GetTreeViewText(), ':'))));
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::OpenUrlActionUpdate(TObject *Sender)
{
	UnicodeString vstr = exclude_quot(get_tkn_r(GetTreeViewText(), ':'));
	((TAction*)Sender)->Enabled = (vstr.Pos("://") && TRegEx::Match(vstr, "^" URL_MATCH_PTN).Success);
}

//---------------------------------------------------------------------------
//下方向に検索
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FindDownActionExecute(TObject *Sender)
{
	if (!FindEdit->Text.IsEmpty()) {
		bool found = false;
		TTreeNode *sp = JsonTreeView->Selected;
		sp = sp? sp->GetNext() : JsonTreeView->Items->GetFirstNode();
		while (sp && !found) {
			found = MatchNode(sp);
			if (!found) sp = sp->GetNext();
		}
		if (found) JsonTreeView->Selected = sp; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
//上方向に検索
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FindUpActionExecute(TObject *Sender)
{
	if (!FindEdit->Text.IsEmpty()) {
		bool found = false;
		TTreeNode *sp = JsonTreeView->Selected;
		sp = sp? sp->GetPrev() : JsonTreeView->Items->Item[JsonTreeView->Items->Count - 1];
		while (sp && !found) {
			found = MatchNode(sp);
			if (!found) sp = sp->GetPrev();
		}
		if (found) JsonTreeView->Selected = sp; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FindActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !FindEdit->Text.IsEmpty() && (NamCheckBox->Checked || ValCheckBox->Checked);
}

//---------------------------------------------------------------------------
//展開
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::ExpandActionExecute(TObject *Sender)
{
	ViewBusy = true;
	set_RedrawOff(JsonTreeView);
	{
		TTreeNode *sp = JsonTreeView->Selected;
		TTreeNode *sp0 = sp;
		while (sp) {
			sp->Expanded = true;
			sp = sp->GetNext();
			if (!sp || sp->Level<=sp0->Level) break;
		}
	}
	set_RedrawOn(JsonTreeView);
	ViewBusy = false;
}
//---------------------------------------------------------------------------
//畳む
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::CollapseActionExecute(TObject *Sender)
{
	ViewBusy = true;
	TTreeNode *sp = JsonTreeView->Selected;
	TTreeNode *sp0 = sp;
	while (sp) {
		sp->Expanded = false;
		sp = sp->GetNext();
		if (!sp || sp->Level<=sp0->Level) break;
	}
	ViewBusy = false;
}

//---------------------------------------------------------------------------
//自動開閉
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::AutoActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	ViewBusy = true;
	JsonTreeView->AutoExpand = AutoCheckBox->Checked;
	JsonTreeView->SetFocus();
	ViewBusy = false;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::JsonTreeViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	TTreeNode *sp = JsonTreeView->Selected;
	//移動
	if		(USAME_TI(cmd_F, "CursorDown") || USAME_TI(cmd_V, "CursorDown"))	perform_Key(JsonTreeView, VK_DOWN);
	else if (USAME_TI(cmd_F, "CursorUp")   || USAME_TI(cmd_V, "CursorUp"))		perform_Key(JsonTreeView, VK_UP);
	else if (USAME_TI(cmd_F, "PageDown")   || USAME_TI(cmd_V, "PageDown"))		perform_Key(JsonTreeView, VK_NEXT);
	else if (USAME_TI(cmd_F, "PageUp")	   || USAME_TI(cmd_V, "PageUp"))		perform_Key(JsonTreeView, VK_PRIOR);
	else if (USAME_TI(cmd_F, "CursorTop")  || USAME_TI(cmd_V, "TextTop"))		perform_Key(JsonTreeView, VK_HOME);
	else if (USAME_TI(cmd_F, "CursorEnd")  || USAME_TI(cmd_V, "TextEnd"))		perform_Key(JsonTreeView, VK_END);

	//開閉
	else if (equal_ENTER(KeyStr)) {
		if (sp) {
			if (sp->HasChildren) {
				if (sp->Expanded) {
					sp->Expanded = false;
				}
				else {
					TTreeNode *pp = sp->Parent;
					if (pp && JsonTreeView->AutoExpand) {
						ViewBusy = true;
						set_RedrawOff(JsonTreeView);
						for (int i=0; i<pp->Count; i++) {
							TTreeNode *ip = pp->Item[i];
							ip->Expanded = (ip==sp);
						}
						set_RedrawOn(JsonTreeView);
						ViewBusy = false;
					}
					else {
						sp->Expanded = true;
					}
				}
			}
			else OpenUrlAction->Execute();
		}
	}
	//検索
	else if (USAME_TI(cmd_F, "FindDown") || USAME_TI(KeyStr, "F3")) {
		FindDownAction->Execute();
	}
	else if (USAME_TI(cmd_F, "FindUp")) {
		FindUpAction->Execute();
	}
	else if (StartsText("IncSearch", cmd_F) || contained_wd_i(KeyStr_Filter, KeyStr)) {
		FindEdit->SetFocus();
	}
	//閉じる
	else if (USAME_TI(cmd_F, "ReturnList")) {
		ModalResult = mrCancel;
	}
	else return;

	ClearKeyBuff(true);
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::JsonTreeViewKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN || is_KeyDown(VK_CONTROL)) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FindEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if		(equal_UP(KeyStr))	 FindUpAction->Execute();
	else if (equal_DOWN(KeyStr)) FindDownAction->Execute();
	else if (contained_wd_i(KeysStr_ToList, KeyStr)) JsonTreeView->SetFocus();
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FindEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TJsonViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "Alt+A"))
		AutoAction->Execute();
	else
		SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

