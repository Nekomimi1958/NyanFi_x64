//----------------------------------------------------------------------//
// NyanFi																//
//  XMLビュアー															//
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
#include "XmlView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TXmlViewer *XmlViewer = NULL;

//---------------------------------------------------------------------------
__fastcall TXmlViewer::TXmlViewer(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	ViewMode = 0;

	XmlnsList = new TStringList();
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);

	ViewMode			  = IniFile->ReadIntGen(_T("XmlViewViewMode"),	2);
	AutoCheckBox->Checked = IniFile->ReadBoolGen(_T("XmlViewAutoExp"));
	NamCheckBox->Checked  = IniFile->ReadBoolGen(_T("XmlViewFindName"),	true);
	AtrCheckBox->Checked  = IniFile->ReadBoolGen(_T("XmlViewFindAttr"),	true);
	ValCheckBox->Checked  = IniFile->ReadBoolGen(_T("XmlViewFindVal"),	true);

	XmlTreeView->Items->Clear();
	XmlTreeView->Color = col_bgList;
	XmlTreeView->Font->Assign(ListFont);

	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);

	Caption = yen_to_delimiter(FileName) + " - XMLビュアー";

	SetDarkWinTheme(XmlTreeView);
	SetDarkWinTheme(OpePanel);
	SetDarkWinTheme(FindEdit);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::WmFormShowed(TMessage &msg)
{
	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);
	Repaint();

	ErrMsg = EmptyStr;

	ViewBusy = true;
	set_RedrawOff(XmlTreeView);
	try {
		XmlTreeView->SetFocus();
		//XMLファイルの読み込み
		XMLDocument1->Options = XMLDocument1->Options >> doAutoPrefix;
		XMLDocument1->LoadFromFile(FileName);

		//最初に宣言された名前空間のリストを取得
		XmlnsList->Clear();
		_di_IXMLNode XMLNode = XMLDocument1->Node;
		for (int i=0; i<XMLNode->ChildNodes->Count; i++) {
			_di_IXMLNode node = XMLNode->ChildNodes->Nodes[i];
			if (node->ChildNodes->Count==0) continue;
			_di_IXMLNodeList atrs = node->AttributeNodes;
			if (atrs && atrs->Count>0) {
				for (int i=0; i<atrs->Count; i++) {
					UnicodeString anam = atrs->Nodes[i]->NodeName;
					if (TRegEx::Match(anam, "^xmlns:?").Success && !atrs->Nodes[i]->Text.IsEmpty()) {
						XmlnsList->Add(UnicodeString().sprintf(_T(" %s=\"%s\""),
										atrs->Nodes[i]->NodeName.c_str(), atrs->Nodes[i]->Text.c_str()));
					}
				}
			}
			break;
		}

		//ツリービューに割り当て
		AssignView(NULL, XMLDocument1->Node);
		TTreeNode *TopNode = XmlTreeView->Items->GetFirstNode();
		TopNode->Selected  = true;
		TopNode->Expanded  = true;
	}
	catch (Exception &e) {
		ErrMsg = e.Message;
	}
	set_RedrawOn(XmlTreeView);
	ViewBusy = false;

	if (!ErrMsg.IsEmpty()) {
		XmlTreeView->Items->AddChildObject(NULL, "ERROR: " + LoadUsrMsg(USTR_FaildLoad), (void*)-1);
		msgbox_ERR(ErrMsg);
		StatusBar1->Panels->Items[0]->Text = ErrMsg;
	}
}

//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FormClose(TObject *Sender, TCloseAction &Action)
{
	XmlTreeView->Items->Clear();
	XmlnsList->Clear();

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("XmlViewViewMode"),		ViewMode);
	IniFile->WriteBoolGen(_T("XmlViewAutoExp"),		AutoCheckBox);
	IniFile->WriteBoolGen(_T("XmlViewFindName"),	NamCheckBox);
	IniFile->WriteBoolGen(_T("XmlViewFindAttr"),	AtrCheckBox);
	IniFile->WriteBoolGen(_T("XmlViewFindVal"),		ValCheckBox);
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FormDestroy(TObject *Sender)
{
	delete XmlnsList;
}

//---------------------------------------------------------------------------
//ViewBusy プロパティ
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::SetViewBusy(bool Value)
{
	FViewBusy = Value;

	if (Value) {
		cursor_HourGlass();
		StatusBar1->Panels->Items[0]->Text = "処理中...";	StatusBar1->Repaint();
	}
	else {
		StatusBar1->Panels->Items[0]->Text = GetXPath();
		cursor_Default();
	}
}

//---------------------------------------------------------------------------
//ツリービューに割り当て
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::AssignView(TTreeNode *TreeNode, _di_IXMLNode XMLNode)
{
	TTreeNode *Node;

	if (XMLNode->NodeType==ntText) {
		//値
		Node = XmlTreeView->Items->AddChildObject(TreeNode, XMLNode->NodeValue, (void*)0);	//0=値
		Node->ImageIndex	= 0;
		Node->SelectedIndex = 0;
	}
	else {
		UnicodeString lbuf;
		//ノード名/属性モード
		if (ViewMode<2) {
			//名前
			lbuf = XMLNode->NodeName;
			//属性
			if (ViewMode==1) {
				_di_IXMLNodeList atrs = XMLNode->AttributeNodes;
				if (atrs && atrs->Count>0) {
					for (int i=0; i<atrs->Count; i++)
						lbuf.cat_sprintf(_T(" [%s=%s]"), atrs->Nodes[i]->NodeName.c_str(), atrs->Nodes[i]->Text.c_str());
				}
			}
		}
		//XMLモード
		else {
			if (!TreeNode) {
				lbuf = XMLNode->NodeName;
			}
			else {
				lbuf = get_tkn(XMLNode->XML, _T("\r\n"));
				if (lbuf.Pos('>')) lbuf = get_tkn(lbuf, '>') + ">";
				//不要な xmlns を削除
				if (XmlnsList->Count>0 && TreeNode && TreeNode->Level>0) {
					for (int i=0; i<XmlnsList->Count; i++)
						lbuf = ReplaceStr(lbuf, XmlnsList->Strings[i], EmptyStr);
				}
			}
		}

		Node = XmlTreeView->Items->AddChildObject(TreeNode, lbuf, (void*)1);	//1=ノード
		Node->ImageIndex	= 1;
		Node->SelectedIndex = 1;
	}

	for (int i=0; i<XMLNode->ChildNodes->Count; i++) AssignView(Node, XMLNode->ChildNodes->Nodes[i]);
}

//---------------------------------------------------------------------------
//ノードのパスを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TXmlViewer::GetXPath(TTreeNode *Node)
{
	UnicodeString ret_str;
	if (!Node) Node = XmlTreeView->Selected;
	if (Node) {
		if (!Node->HasChildren) Node = Node->Parent;
		while (Node && Node->Level>0) {
			UnicodeString nnam = Node->Text;
			if (ViewMode==2) {
				remove_top_s(nnam, '<');
				remove_end_s(nnam, '>');
				nnam = get_tkn(nnam, ' ');
			}
			else {
				nnam = get_tkn(nnam, _T(" ["));
			}
			ret_str.Insert("/" + nnam, 1);
			Node = Node->Parent;
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//指定パスのXMLソースを取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TXmlViewer::GetXmlStr()
{
	UnicodeString ret_str;
	TTreeNode *sp = XmlTreeView->Selected;
	if (sp) {
		UnicodeString xpath = GetXPath(sp);
		remove_top_s(xpath, '/');
		TStringDynArray p_buf = SplitString(xpath, "/");

		_di_IXMLNode XMLNode = XMLDocument1->Node;
		for (int i=0; i<p_buf.Length && XMLNode; i++) XMLNode = XMLNode->ChildNodes->FindNode(p_buf[i]);
		if (XMLNode) {
			ret_str = XMLNode->XML;
			//不要な xmlns を削除
			if (XmlnsList->Count>0 && p_buf.Length>1) {
				for (int i=0; i<XmlnsList->Count; i++)
					ret_str = ReplaceStr(ret_str, XmlnsList->Strings[i], EmptyStr);
			}
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//ノードが検索語にマッチするか?
//---------------------------------------------------------------------------
bool __fastcall TXmlViewer::MatchNode(TTreeNode *Node)
{
	if (!Node) return false;

	UnicodeString lbuf = Node->Text;
	UnicodeString s;
	//ノード
	if ((int)Node->Data==1) {
		if (NamCheckBox->Checked) {
			if (ViewMode==2)
				s = get_tkn(lbuf, ' ');
			else
				s = get_tkn(lbuf, _T(" ["));
		}
		if (AtrCheckBox->Checked) {
			if (ViewMode==2)
				s += get_tkn_r(lbuf, ' ');
			else
				s.cat_sprintf(_T(" %s"), get_tkn_r(lbuf, _T(" [")).c_str());
		}
	}
	//値
	else {
		if (ValCheckBox->Checked) s += lbuf;
	}

	return ContainsText(s, FindEdit->Text);
}

//---------------------------------------------------------------------------
//リストビューの描画
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::XmlTreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
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
	UnicodeString lbuf = Node->Text;

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
		//ノード名/属性モード
		if (ViewMode<2) {
			//名前
			tmp_cv->Font->Color = is_selfg? col_fgSelItem : col_Reserved;
			UnicodeString nnam = split_tkn(lbuf, ' ');
			tmp_cv->TextOut(xp, 0, nnam);
			//属性
			if (!lbuf.IsEmpty()) {
				xp += get_TextWidth(tmp_cv, nnam, is_irreg);
				lbuf.Insert(" ", 1);
				bool pre_eq = true;
				TColor fg = col_Reserved;
				UnicodeString s;
				for (int i=1; i<=lbuf.Length(); i++) {
					WideChar c = lbuf[i];
					bool is_eq = (pre_eq && c=='=');
					if (is_eq) pre_eq = false; else if (c=='[') pre_eq = true;
					TColor col = (c=='[' || c==']' || is_eq)? col_Symbol: !pre_eq? col_Strings : col_Reserved;
					if (fg!=col) {
						tmp_cv->Font->Color = is_selfg? col_fgSelItem : fg;
						tmp_cv->TextOut(xp, 0, s);
						xp += get_TextWidth(tmp_cv, s, is_irreg);
						s = EmptyStr;
						fg = col;
					}
					s.cat_sprintf(_T("%c"), c);
				}
				if (!s.IsEmpty()) {
					tmp_cv->Font->Color = is_selfg? col_fgSelItem : fg;
					tmp_cv->TextOut(xp, 0, s);
				}
			}
		}
		//XMLモード
		else {
			//コメント
			if (StartsStr("<!--", lbuf)) {
				tmp_cv->Font->Color = is_selfg? col_fgSelItem : col_Comment;
				tmp_cv->TextOut(xp, 0, lbuf);
			}
			else {
				//名前
				bool in_qt	= false;
				TColor fg = col_Reserved;
				UnicodeString s;
				for (int i=1; i<=lbuf.Length(); i++) {
					WideChar c = lbuf[i];
					TColor col;
					if (c=='\"') {
						col = col_Strings;	in_qt = !in_qt;
					}
					else if (in_qt) {
						col = col_Strings;
					}
					else {
						col = (c=='<' || c=='>' ||c=='/' || c=='=')? col_Symbol: col_Reserved;
					}

					if (fg!=col) {
						tmp_cv->Font->Color = is_selfg? col_fgSelItem : fg;
						tmp_cv->TextOut(xp, 0, s);
						xp += get_TextWidth(tmp_cv, s, is_irreg);
						s = EmptyStr;
						fg = col;
					}
					s.cat_sprintf(_T("%c"), c);
				}
	
				if (!s.IsEmpty()) {
					tmp_cv->Font->Color = is_selfg? col_fgSelItem : fg;
					tmp_cv->TextOut(xp, 0, s);
				}
			}
		}
	}
	//値
	else if (ntyp==0) {
		tmp_cv->Font->Color = is_selfg? col_fgSelItem :
					(StartsStr("http", lbuf) && TRegEx::Match(lbuf, URL_MATCH_PTN).Success)? col_URL : col_fgList;
		tmp_cv->TextOut(0, 0, lbuf);
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
	SCROLLINFO si;
	::ZeroMemory(&si, sizeof(SCROLLINFO));
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_TRACKPOS;
	if (::GetScrollInfo(Sender->Handle, SB_HORZ, &si)) scr_p = si.nTrackPos;

	//親ライン
	cv->Pen->Style = psSolid;
	cv->Pen->Width = 1;
	cv->Pen->Color = col_HR;
	rc_s.Right = rc_t.Left;
	int l_ofs  = rc_s.Left + ScaledInt(11);
	TTreeNode *pp = Node->Parent;
	while (pp) {
		if (pp->getNextSibling()) {
			int xl = XmlTreeView->Indent * pp->Level + l_ofs - scr_p;
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
		int w_btn = ScaledInt(11);	//ボタンサイズ
		int xp = rc_s.Right - ScaledInt(w_btn + 5);
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
		int xl = XmlTreeView->Indent * Node->Level + l_ofs - scr_p;
		int x2 = xl + Scaled8;
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
void __fastcall TXmlViewer::XmlTreeViewChange(TObject *Sender, TTreeNode *Node)
{
	StatusBar1->Panels->Items[0]->Text = !ErrMsg.IsEmpty()? ErrMsg : GetXPath(Node);
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::XmlTreeViewHint(TObject *Sender, TTreeNode * const Node, UnicodeString &Hint)
{
	Hint = EmptyStr;
}

//---------------------------------------------------------------------------
//ステータスバーの描画
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect);
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(Rect.Left + Scaled2, Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//すべて展開
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::ExpandItemClick(TObject *Sender)
{
	ViewBusy = true;
	set_RedrawOff(XmlTreeView);
	{
		XmlTreeView->FullExpand();
		XmlTreeView->TopItem = XmlTreeView->Items->GetFirstNode();
	}
	set_RedrawOn(XmlTreeView);
	ViewBusy = false;
}
//---------------------------------------------------------------------------
//すべて畳む
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CollapseItemClick(TObject *Sender)
{
	XmlTreeView->FullCollapse();
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyActionExecute(TObject *Sender)
{
	TTreeNode *sp = XmlTreeView->Selected;
	if (sp) copy_to_Clipboard(sp->Text);
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = Active && XmlTreeView->Selected;
}

//---------------------------------------------------------------------------
//XMLソースをコピー
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyXmlActionExecute(TObject *Sender)
{
	UnicodeString xml = GetXmlStr();
	copy_to_Clipboard(xml);
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyXmlActionUpdate(TObject *Sender)
{
	TTreeNode *sp = XmlTreeView->Selected;
	((TAction*)Sender)->Enabled = (sp && sp->Level>0);
}

//---------------------------------------------------------------------------
//XPath をコピー
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyXPathActionExecute(TObject *Sender)
{
	TTreeNode *sp = XmlTreeView->Selected;
	if (sp) copy_to_Clipboard(GetXPath(sp));
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CopyXPathActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XmlTreeView->Selected;
}

//---------------------------------------------------------------------------
//URLを開く
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::OpenUrlActionExecute(TObject *Sender)
{
	if (XmlTreeView->Selected) Execute_ex(extract_URL(XmlTreeView->Selected->Text));
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::OpenUrlActionUpdate(TObject *Sender)
{
	TTreeNode *sp = XmlTreeView->Selected;
	((TAction*)Sender)->Enabled = (sp && sp->Text.Pos("://") && TRegEx::Match(sp->Text, "^" URL_MATCH_PTN).Success);
}

//---------------------------------------------------------------------------
//表示モード
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::PopupMenu1Popup(TObject *Sender)
{
	ViewMode0Item->Checked = ViewMode==0;
	ViewMode1Item->Checked = ViewMode==1;
	ViewMode2Item->Checked = ViewMode==2;
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::ViewModeItemClick(TObject *Sender)
{
	ViewMode = ((TMenuItem*)Sender)->Tag;

	XmlTreeView->Items->Clear();

	ViewBusy = true;
	set_RedrawOff(XmlTreeView);
	{
		AssignView(NULL, XMLDocument1->Node);
		TTreeNode *TopNode = XmlTreeView->Items->GetFirstNode();
		TopNode->Selected  = true;
		TopNode->Expanded  = true;
	}
	set_RedrawOn(XmlTreeView);
	ViewBusy = false;
}

//---------------------------------------------------------------------------
//下方向に検索
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FindDownActionExecute(TObject *Sender)
{
	if (!FindEdit->Text.IsEmpty()) {
		bool found = false;
		TTreeNode *sp = XmlTreeView->Selected;
		sp = sp? sp->GetNext() : XmlTreeView->Items->GetFirstNode();
		while (sp && !found) {
			found = MatchNode(sp);
			if (!found) sp = sp->GetNext();
		}
		if (found) XmlTreeView->Selected = sp; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
//上方向に検索
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FindUpActionExecute(TObject *Sender)
{
	if (!FindEdit->Text.IsEmpty()) {
		bool found = false;
		TTreeNode *sp = XmlTreeView->Selected;
		sp = sp? sp->GetPrev() : XmlTreeView->Items->Item[XmlTreeView->Items->Count - 1];
		while (sp && !found) {
			found = MatchNode(sp);
			if (!found) sp = sp->GetPrev();
		}
		if (found) XmlTreeView->Selected = sp; else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FindActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	AtrCheckBox->Enabled = (ViewMode!=0);
	ap->Enabled = !FindEdit->Text.IsEmpty() &&
					(NamCheckBox->Checked || (AtrCheckBox->Enabled && AtrCheckBox->Checked) || ValCheckBox->Checked);
}

//---------------------------------------------------------------------------
//展開
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::ExpandActionExecute(TObject *Sender)
{
	ViewBusy = true;
	set_RedrawOff(XmlTreeView);
	{
		TTreeNode *sp = XmlTreeView->Selected;
		TTreeNode *sp0 = sp;
		while (sp) {
			sp->Expanded = true;
			sp = sp->GetNext();
			if (!sp || sp->Level<=sp0->Level) break;
		}
	}
	set_RedrawOn(XmlTreeView);
	ViewBusy = false;
}
//---------------------------------------------------------------------------
//畳む
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::CollapseActionExecute(TObject *Sender)
{
	ViewBusy = true;
	TTreeNode *sp = XmlTreeView->Selected;
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
void __fastcall TXmlViewer::AutoActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	ViewBusy = true;
	XmlTreeView->AutoExpand = AutoCheckBox->Checked;
	XmlTreeView->SetFocus();
	ViewBusy = false;
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::XmlTreeViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	TTreeNode *sp = XmlTreeView->Selected;
	//移動
	if		(USAME_TI(cmd_F, "CursorDown") || USAME_TI(cmd_V, "CursorDown"))	perform_Key(XmlTreeView, VK_DOWN);
	else if (USAME_TI(cmd_F, "CursorUp")   || USAME_TI(cmd_V, "CursorUp"))		perform_Key(XmlTreeView, VK_UP);
	else if (USAME_TI(cmd_F, "PageDown")   || USAME_TI(cmd_V, "PageDown"))		perform_Key(XmlTreeView, VK_NEXT);
	else if (USAME_TI(cmd_F, "PageUp")	   || USAME_TI(cmd_V, "PageUp"))		perform_Key(XmlTreeView, VK_PRIOR);
	else if (USAME_TI(cmd_F, "CursorTop")  || USAME_TI(cmd_V, "TextTop"))		perform_Key(XmlTreeView, VK_HOME);
	else if (USAME_TI(cmd_F, "CursorEnd")  || USAME_TI(cmd_V, "TextEnd"))		perform_Key(XmlTreeView, VK_END);

	//開閉
	else if (equal_ENTER(KeyStr)) {
		if (sp) {
			if (sp->HasChildren) {
				if (sp->Expanded) {
					sp->Expanded = false;
				}
				else {
					TTreeNode *pp = sp->Parent;
					if (pp && XmlTreeView->AutoExpand) {
						ViewBusy = true;
						set_RedrawOff(XmlTreeView);
						for (int i=0; i<pp->Count; i++) {
							TTreeNode *ip = pp->Item[i];
							ip->Expanded = (ip==sp);
						}
						set_RedrawOn(XmlTreeView);
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
void __fastcall TXmlViewer::XmlTreeViewKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN || is_KeyDown(VK_CONTROL)) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FindEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if		(equal_UP(KeyStr))	 FindUpAction->Execute();
	else if (equal_DOWN(KeyStr)) FindDownAction->Execute();
	else if (contained_wd_i(KeysStr_ToList, KeyStr)) XmlTreeView->SetFocus();
	else return;

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FindEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key) || Key==VK_RETURN || Key==VK_ESCAPE) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TXmlViewer::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (USAME_TI(KeyStr, "Alt+A"))
		AutoAction->Execute();
	else
		SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

