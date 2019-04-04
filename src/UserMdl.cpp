//----------------------------------------------------------------------//
// 汎用共用モジュール													//
//																		//
//----------------------------------------------------------------------//
#pragma hdrstop
#include <Vcl.Clipbrd.hpp>
#include <Vcl.FileCtrl.hpp>
#include <RegularExpressions.hpp>
#include "usr_msg.h"
#include "usr_file_ex.h"
#include "usr_swatch.h"
#include "UserFunc.h"
#include "InpCmds.h"
#include "RegExChk.h"
#include "CalcDlg.h"
#include "UserMdl.h"
#include "EditItem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TUserModule *UserModule;

//---------------------------------------------------------------------------
//テキスト保存コードページ
code_page_inf SaveCodePages[MAX_SAVE_CODEPAGES] = {
	{  932,	"Shift_JIS"},
	{65001,	"UTF-8"},
	{65001,	"UTF-8N"},
	{ 1200,	"UTF-16"},
	{ 1201,	"UTF-16(BE)"},
	{20932,	"EUC-JP"},
	{50220,	"ISO-2022-JP"}
};

//---------------------------------------------------------------------------
__fastcall TUserModule::TUserModule(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DataModuleCreate(TObject *Sender)
{
	//マウスポインターを設定
	crHandGrabR = (TCursor)6;	//イメージ移動
	crSpuitTool = (TCursor)7;	//スポイト
	Screen->Cursors[crHandGrabR] = (HCURSOR)::LoadImage(HInstance, _T("HNDGRB_R"),	 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	Screen->Cursors[crSpuitTool] = (HCURSOR)::LoadImage(HInstance, _T("SPUIT_TOOL"), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

	SpuitPanel = NULL;
	SpuitImage = NULL;

	ActiveListBox  = NULL;
	ActiveScrPanel = NULL;

	SaveTxtDlg->Filter	   = F_FILTER_TXT;
	SaveTxtDlg->DefaultExt = "txt";
	SaveTxtDlg->Encodings->Clear();
	for (int i=0; i<MAX_SAVE_CODEPAGES; i++) SaveTxtDlg->Encodings->Add(SaveCodePages[i].name);

	//コマンドパラメータの参照
	CmdParamList = new TStringList();

	//正規表現の参照
	RefRegExList = new TStringList();
	RefRegExList->Text =
		"\\	次のメタ文字をクォート\n"
		"^	行頭\n"
		".	改行以外のすべての文字\n"
		"$	行末\n"
		"|	選択\n"
		"()	グループ化\n"
		"[]	文字クラス\n"
		"-\n"
		"\\w	単語の構成文字(英数字と\"_\")\n"
		"\\W	単語の構成文字以外\n"
		"\\s	空白文字\n"
		"\\S	空白文字以外\n"
		"\\d	数字\n"
		"\\D	数字以外\n"
		"\\b	単語の境界\n"
		"\\B	単語の境界以外\n"
		"-\n"
		"*	0 回以上\n"
		"+	1 回以上\n"
		"?	1 回または 0 回\n"
		"{n}	ちょうど n 回\n"
		"{n,}	n 回以上\n"
		"{n,m}	n 回以上 m 回以下\n";

	//正規表現メニューの設定
	for (int i=0; i<RefRegExList->Count; i++) {
		UnicodeString lbuf = RefRegExList->Strings[i];
		if (lbuf.IsEmpty()) continue;
		TMenuItem *mp1 = new TMenuItem(RegExpItem);
		TMenuItem *mp2 = new TMenuItem(RegExpEItem);
		if (lbuf[1]=='-') {
			mp1->Caption = lbuf;
			mp2->Caption = lbuf;
		}
		else {
			UnicodeString s1 = get_pre_tab(lbuf);
			UnicodeString s2 = get_post_tab(lbuf);
			lbuf.sprintf(_T("%-6s %s"), s1.c_str(), s2.c_str());
			mp1->Caption = lbuf;
			mp2->Caption = lbuf;
			mp1->OnClick = RefRegExItemClick;
			mp2->OnClick = RefRegExItemClick;
			RegExpItem->Add(mp1);
			RegExpEItem->Add(mp2);
		}
	}

	FileListRect = Rect(0,0, 600,400);	//仮設定

	LastComboBox   = NULL;
	LastComboBoxSS = 0;
	LastComboBoxSL = 0;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DataModuleDestroy(TObject *Sender)
{
	delete CmdParamList;
	delete RefRegExList;
}

//---------------------------------------------------------------------------
//リストボックスの初期化 (TListBox/TCheckListBox に対応)
//  DragMode=dmAutomatic ならドラッグ&ドロップによる項目移動が可能
//  項目の上下移動および削除用アクションを提供
//---------------------------------------------------------------------------
void __fastcall TUserModule::InitializeListBox(TCustomListBox *lp, UsrScrollPanel *sp)
{
	ActiveListBox = lp;
	if (ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0) {
		if (class_is_ListBox(ActiveListBox)) {
			if (((TListBox*)ActiveListBox)->DragMode==dmAutomatic) {
				((TListBox*)ActiveListBox)->OnStartDrag = ListBoxStartDrag;
				((TListBox*)ActiveListBox)->OnDragOver	= ListBoxDragOver;
				((TListBox*)ActiveListBox)->OnDragDrop	= ListBoxDragDrop;
				((TListBox*)ActiveListBox)->OnEndDrag	= ListBoxEndDrag;
			}
		}
		else if (class_is_CheckListBox(ActiveListBox)) {
			if (((TCheckListBox*)ActiveListBox)->DragMode==dmAutomatic) {
				((TCheckListBox*)ActiveListBox)->OnStartDrag = ListBoxStartDrag;
				((TCheckListBox*)ActiveListBox)->OnDragOver  = ListBoxDragOver;
				((TCheckListBox*)ActiveListBox)->OnDragDrop  = ListBoxDragDrop;
				((TCheckListBox*)ActiveListBox)->OnEndDrag	 = ListBoxEndDrag;
			}
		}
	}
	PrevListIdx = -1;

	ActiveScrPanel = sp;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::UninitializeListBox()
{
	ScrollTimer->Enabled = false;

	if (ActiveListBox) {
		if (class_is_ListBox(ActiveListBox)) {
			((TListBox*)ActiveListBox)->OnStartDrag = NULL;
			((TListBox*)ActiveListBox)->OnDragOver  = NULL;
			((TListBox*)ActiveListBox)->OnDragDrop  = NULL;
			((TListBox*)ActiveListBox)->OnEndDrag	= NULL;
		}
		else if (class_is_CheckListBox(ActiveListBox)) {
			((TCheckListBox*)ActiveListBox)->OnStartDrag = NULL;
			((TCheckListBox*)ActiveListBox)->OnDragOver  = NULL;
			((TCheckListBox*)ActiveListBox)->OnDragDrop  = NULL;
			((TCheckListBox*)ActiveListBox)->OnEndDrag	 = NULL;
		}
		ActiveListBox  = NULL;
		ActiveScrPanel = NULL;
	}
}

//---------------------------------------------------------------------------
//リスト項目を一つ上へ
//---------------------------------------------------------------------------
void __fastcall TUserModule::UpListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		int idx = ActiveListBox->ItemIndex;
		if (idx>=1 && idx<ActiveListBox->Count) {
			ActiveListBox->Items->Move(idx, idx - 1);
			ActiveListBox->ItemIndex = idx - 1;
		}
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::UpListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex>=1 && ActiveListBox->ItemIndex<ActiveListBox->Count;

}
//---------------------------------------------------------------------------
//リスト項目を一つ下へ
//---------------------------------------------------------------------------
void __fastcall TUserModule::DownListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		int idx = ActiveListBox->ItemIndex;
		if (idx>=0 && idx<ActiveListBox->Count-1) {
			ActiveListBox->Items->Move(idx, idx + 1);
			ActiveListBox->ItemIndex = idx + 1;
		}
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DownListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex>=0 && ActiveListBox->ItemIndex<ActiveListBox->Count-1;
}
//---------------------------------------------------------------------------
//リスト項目を削除
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		delete_ListItem(ActiveListBox);
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex!=-1;
}
//---------------------------------------------------------------------------
//リスト項目をクリア
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		ActiveListBox->Clear();
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->Count>0;
}

//---------------------------------------------------------------------------
//ドラッグ&ドロップによるリスト項目の移動
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxStartDrag(TObject *Sender, TDragObject *&DragObject)
{
	PrevListIdx = -1;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y,
		TDragState State, bool &Accept)
{
	ScrollTimer->Enabled = false;

	TCustomListBox *lp = (TCustomListBox*)Sender;
	Accept = (Source == Sender) && (lp->ItemIndex!=-1);

	int ch = lp->ClientHeight;
	int ih = (class_is_CheckListBox(lp)? ((TCheckListBox*)lp)->ItemHeight : ((TListBox*)lp)->ItemHeight);
	if (lp->TopIndex>0 && Y<(ih - 4)) {
		//上へスクロール
		ScrollTimer->Interval = 100;
		ScrollTimer->Tag	  = (Y<=0)? -2 : -1;
		ScrollTimer->Enabled  = true;
	}
	else if (Y>(ch - (ih - 4))) {
		//下へスクロール
		ScrollTimer->Interval = 100; 
		ScrollTimer->Tag	  = (Y>=ch)? 2 : 1;
		ScrollTimer->Enabled  = true;
	}
	else if (Accept) {
		int idx = lp->ItemAtPos(Point(X, Y), true);
		draw_ListItemLine(lp, PrevListIdx);		//直前のドロップ先表示を消去
		draw_ListItemLine(lp, idx);				//新しいドロップ先表示を描画
		PrevListIdx = idx;
	}

	if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
	ScrollTimer->Enabled  = false;

	TCustomListBox *lp = (TCustomListBox*)Sender;
	int idx = lp->ItemAtPos(Point(X, Y), true);
	draw_ListItemLine(lp, PrevListIdx);		//直前のドロップ先表示を消去
	if (idx!=-1) {
		lp->Items->Move(lp->ItemIndex, idx);
		lp->ItemIndex = idx;
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxEndDrag(TObject *Sender, TObject *Target, int X, int Y)
{
	ScrollTimer->Enabled  = false;
}
//---------------------------------------------------------------------------
//リストボックスの自動スクロール
//---------------------------------------------------------------------------
void __fastcall TUserModule::ScrollTimerTimer(TObject *Sender)
{
	if (ActiveListBox) {
		int last_top = ActiveListBox->TopIndex;
		ActiveListBox->TopIndex = ActiveListBox->TopIndex + ((TComponent*)Sender)->Tag;
		if (ActiveListBox->TopIndex!=last_top) {
			draw_ListItemLine(ActiveListBox, PrevListIdx);		//直前のドロップ先表示を消去
			PrevListIdx = -1;
			ActiveListBox->Invalidate();
		}
		else ScrollTimer->Enabled = false;
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}

//---------------------------------------------------------------------------
//リストボックスの項目に対するキー操作 (移動、削除)
//---------------------------------------------------------------------------
bool __fastcall TUserModule::ListBoxOpeItem(UnicodeString keystr)
{ 
	switch (idx_of_word_i(_T("Shift+Ctrl+DOWN|Shift+Ctrl+UP|DEL"), keystr)) {
	case  0: DownListItemAction->Execute();	break;
	case  1: UpListItemAction->Execute();	break;
	case  2: DelListItemAction->Execute();	break;
	default: 								return false;
	}

	if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	return true;
}

//---------------------------------------------------------------------------
//マウス位置のピクセル色を取得するスポイト機能
//---------------------------------------------------------------------------
//開始
//---------------------------------------------------------------------------
void __fastcall TUserModule::BeginSpuit(
	TPanel *pp,			//スポイトパネル
	TImage *ip,			//スポイトイメージ
	TColor col_def)		//デフォルト色
{
	SpuitPanel = pp;

	SpuitImage = ip;
	if (SpuitImage) SpuitImage->Visible = false;

	SpuitDefColor = col_def;
	if (col_def!=clNone) pp->Color = col_def;

	Screen->Cursor = crSpuitTool;
	SpuitTimer->Enabled = true;
}
//---------------------------------------------------------------------------
//終了
//---------------------------------------------------------------------------
TColor __fastcall TUserModule::EndSpuit()
{
	cursor_Default();
	SpuitTimer->Enabled = false;

	TColor col = (SpuitPanel && FindControl(get_window_from_pos())!=SpuitPanel)? SpuitPanel->Color : SpuitDefColor;
	SpuitPanel->Color = scl_BtnFace;
	SpuitPanel = NULL;

	if (SpuitImage) SpuitImage->Visible = true;
	SpuitImage = NULL;

	return col;
}
//---------------------------------------------------------------------------
//スポイト動作中?
//---------------------------------------------------------------------------
bool __fastcall TUserModule::SpuitEnabled()
{
	return SpuitTimer->Enabled;
}
//---------------------------------------------------------------------------
//スポイトでマウス位置のピクセル色を取得
//---------------------------------------------------------------------------
void __fastcall TUserModule::SpuitTimerTimer(TObject *Sender)
{
	if (SpuitPanel && FindControl(get_window_from_pos())!=SpuitPanel) {
		HDC hDc = ::GetDC(0);
		if (hDc) {
			COLORREF c = ::GetPixel(hDc, Mouse->CursorPos.x, Mouse->CursorPos.y);
			::ReleaseDC(0, hDc);
			SpuitPanel->Color = (TColor)c;
		}
	}
}

//---------------------------------------------------------------------------
//ファイル保存/開くダイアログを閉じたときの処理
//---------------------------------------------------------------------------
void __fastcall TUserModule::FileDlgClose(TObject *Sender)
{
	CloseIME(Application->ActiveFormHandle);

	SaveTxtDlg->Filter	   = F_FILTER_TXT;
	SaveTxtDlg->DefaultExt = "txt";
}

//---------------------------------------------------------------------------
void __fastcall TUserModule::ComboBoxContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled)
{
	TComboBox *bp = dynamic_cast<TComboBox *>(Sender);
	if (bp && !bp->Focused()) {
		bp->SetFocus();
		Application->ProcessMessages();

		TMenuItem *mp = bp->PopupMenu? bp->PopupMenu->Items : NULL;
		if (mp) {
			for (int i=0; i<mp->Count; i++) {
				TMenuItem *ip = mp->Items[i];
				if (ip->Action) ip->Action->Update();
			}
		}
	}
}

//---------------------------------------------------------------------------
//エディット/コンボボックスに独自ポップアップメニューを設定
//---------------------------------------------------------------------------
void __fastcall TUserModule::SetUsrPopupMenu(TForm *fp)
{
	//入力欄にポップアップメニューを設定
	for (int i=0; i<fp->ComponentCount; i++) {
		TComponent *cp = fp->Components[i];
		if (class_is_Edit(cp)) {
			((TEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_LabeledEdit(cp)) {
			((TLabeledEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_MaskEdit(cp)) {
			((TMaskEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_ComboBox(cp)) {
			TComboBox *bp = (TComboBox*)cp;
			if (bp->Style==csDropDown) bp->PopupMenu = EditPopupMenuC;
			if (bp->PopupMenu) bp->OnContextPopup = ComboBoxContextPopup;
		}
	}
}

//---------------------------------------------------------------------------
//エディット/コンボボックスのポップアップメニューを表示
//---------------------------------------------------------------------------
bool __fastcall TUserModule::ShowPopupMenu()
{
	TWinControl *wp = Screen->ActiveControl;	if (!wp) return false;

	TPopupMenu  *mp = class_is_Edit(wp)? 		((TEdit*)wp)->PopupMenu :
					  class_is_LabeledEdit(wp)?	((TLabeledEdit*)wp)->PopupMenu :
					  class_is_ComboBox(wp)?	((TComboBox*)wp)->PopupMenu : NULL;
	if (!mp) return false;

	TPoint p = wp->ClientToScreen(Point(0, wp->ClientHeight));
	if ((p.y + ::GetSystemMetrics(SM_CYMENU)*mp->Items->Count)>Screen->Height) p.y -= wp->ClientHeight;
	mp->Popup(p.x, p.y);
	return true;
}

//---------------------------------------------------------------------------
//フォーカスのあるコンボボックスを取得
//---------------------------------------------------------------------------
TComboBox * __fastcall TUserModule::GetActiveComboBox()
{
	TWinControl *cp = Screen->ActiveControl;
	return class_is_ComboBox(cp)? (TComboBox*)cp : NULL;
}
//---------------------------------------------------------------------------
//フォーカスのあるカスタムエディットを取得
//---------------------------------------------------------------------------
TCustomEdit * __fastcall TUserModule::GetActiveCustomEdit()
{
	TWinControl *cp = Screen->ActiveControl;
	return class_is_CustomEdit(cp)? (TCustomEdit *)cp : NULL;
}

//---------------------------------------------------------------------------
//切り取り
//---------------------------------------------------------------------------
void __fastcall TUserModule::CutComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->Perform(WM_CUT, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditComboActionUpdate(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	((TAction*)Sender)->Enabled = (cp && !cp->SelText.IsEmpty());
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TUserModule::CopyComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->Perform(WM_COPY, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
//貼り付け
//---------------------------------------------------------------------------
void __fastcall TUserModule::PasteComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelText = get_norm_str(Clipboard()->AsText);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::PasteComboActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (Clipboard()->HasFormat(CF_TEXT) && GetActiveComboBox());
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPaste1Execute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;	if (!wp) return;

	UnicodeString s = get_norm_str(Clipboard()->AsText);
	if (USAME_TI(wp->Name, "DistrDirEdit") && dir_exists(s)) s = REPLACE_TS(s, "\\", "\\\\");

	if (class_is_MaskEdit(wp)) {
		TMaskEdit *ep = (TMaskEdit*)wp;
		if (ep->SelText.Length()==ep->EditText.Length() || ep->SelText.Length()==0) {
			TDateTime dt;
			if (str_to_DateTime(s, &dt)) {
				//タイムスタンプ
				if (USAME_TI(ep->Name, "TimeMaskEdit"))
					s = format_DateTime(dt);
				//日付
				else if(USAME_TI(ep->Name, "DateMaskEdit"))
					s = format_Date(dt);
				ep->Text	  = s;
				ep->SelStart  = 0;
				ep->SelLength = ep->Text.Length();
			}
			else {
				msgbox_ERR(USTR_IllegalDate);
				return;
			}
		}
		else
			ep->SelText = s;
	}
	else if (class_is_CustomEdit(wp)) {
		((TCustomEdit *)wp)->SelText = s;
	}
}

//---------------------------------------------------------------------------
//すべて選択
//---------------------------------------------------------------------------
void __fastcall TUserModule::SelAllComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::SelAllComboActionUpdate(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	((TAction*)Sender)->Enabled = (cp && !cp->Text.IsEmpty());
}

//---------------------------------------------------------------------------
//この検索文字列を履歴から削除
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelHistComboActionExecute(TObject *Sender)
{
	del_ComboBox_history(GetActiveComboBox());
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelHistComboActionUpdate(TObject *Sender)
{
	TAction   *ap = (TAction*)Sender;
	TComboBox *cp = GetActiveComboBox();
	ap->Visible = (cp && (cp->Tag & CBTAG_HISTORY));
	ap->Enabled = (ap->Visible && cp && cp->Items->Count>0 && !cp->Text.IsEmpty());
}
//---------------------------------------------------------------------------
//検索履歴をすべて削除
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrHistComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp && msgbox_Y_N_C(LoadUsrMsg(USTR_DelHistoryQ))==IDYES) cp->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrHistComboActionUpdate(TObject *Sender)
{
	TAction   *ap = (TAction*)Sender;
	TComboBox *cp = GetActiveComboBox();
	ap->Visible = (cp && (cp->Tag & CBTAG_HISTORY));
	ap->Enabled = (ap->Visible && cp && cp->Items->Count>0);
}

//---------------------------------------------------------------------------
//コマンド名の参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCmdNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;

	if (class_is_CustomEdit(wp)) {
		InpCmdsDlg->toRefer = true;
		if (InpCmdsDlg->ShowModal()==mrOk)
			((TCustomEdit*)wp)->SelText = InpCmdsDlg->CmdsComboBox->Text;
	}
	else if (class_is_ComboBox(wp)) {
		//フォーカスが戻ったとき全選択になってしまう現象への対策
		TComboBox *cp = (TComboBox*)wp;
		int s_start = cp->SelStart;
		int s_len	= cp->SelLength;
		InpCmdsDlg->toRefer = true;

		InpCmdsDlg->ModeTabControl->TabIndex
			= contains_s(wp->Name, _T('V'))? 1 :
			  contains_s(wp->Name, _T('I'))? 2 :
				   (UserModule->ScrMode==2)? 1 :
				   (UserModule->ScrMode==4)? 2 : 0;

		if (InpCmdsDlg->ShowModal()==mrOk) {
			cp->SelStart  = s_start;
			cp->SelLength = s_len;
			cp->SelText   = InpCmdsDlg->CmdsComboBox->Text;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCmdNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp &&
		 (  str_match("WheelBtn*ComboBox", wp->Name)
		 || str_match("WheelCmd*ComboBox", wp->Name)
		 || str_match("X?Btn?ComboBox",	   wp->Name)
		 || contained_wd_i(_T("MenuPrmEdit|EventCmdsEdit|TimerCmd1Edit|TimerCmd2Edit|NewExeCmdEdit|BtnCmdsComboBox"), wp->Name)));
}

//---------------------------------------------------------------------------
//カーソル位置のファイル名を参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurFNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_CustomEdit(wp))
		((TCustomEdit *)wp)->Text = ExtractFileName(CurFileName);
	else if (class_is_ComboBox(wp))
		((TComboBox*)wp)->SelText = ExtractFileName(CurFileName);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurFNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrMaskEdit|ExeComboBox"), wp->Name));
}
//---------------------------------------------------------------------------
//カーソル位置のパス付きファイル名を参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_CustomEdit(wp))
		((TCustomEdit *)wp)->SelText = CurFileName;
	else if (class_is_ComboBox(wp))
		((TComboBox*)wp)->SelText	 = CurFileName;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && USAME_TI(wp->Name, "ExeComboBox"));
}

//---------------------------------------------------------------------------
//カレントパスを参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPathActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_LabeledEdit(wp))
		((TLabeledEdit*)wp)->Text = REPLACE_TS(ExcludeTrailingPathDelimiter(CurPathName), "\\", "\\\\");
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPathActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrDirEdit"), wp->Name));
}
//---------------------------------------------------------------------------
//反対パスを参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefOppPathActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_LabeledEdit(wp))
		((TLabeledEdit*)wp)->Text = REPLACE_TS(ExcludeTrailingPathDelimiter(OppPathName), "\\", "\\\\");
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefOppPathActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrDirEdit"), wp->Name));
}

//---------------------------------------------------------------------------
//コンボボックスのポップアップメニュー
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPopupMenuCPopup(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	RegExpItem->Visible = (cp && (cp->Tag & CBTAG_RGEX_V));
	RegExpItem->Enabled = (cp && (cp->Tag & CBTAG_RGEX_E));

	EditItemItem->Visible = (cp && (StartsStr(". で区切って", cp->Hint) || StartsStr("; で区切って", cp->Hint)));
	EditItemItem->Enabled = EditItemItem->Visible;

	//コマンドパラメータの参照
	CmdParamItem->Visible = (cp && SameText(cp->Name, "PrmComboBox") && CmdParamList->Count>0);
	if (CmdParamItem->Visible) {
		CmdParamItem->Clear();
		for (int i=0; i<CmdParamList->Count; i++) {
			UnicodeString lbuf = CmdParamList->Strings[i];
			if (lbuf.IsEmpty()) continue;
			TMenuItem *mp = new TMenuItem(CmdParamItem);
			mp->Caption   = lbuf;
			mp->OnClick   = CmdParamItemClick;
			CmdParamItem->Add(mp);
		}
	}

	reduction_MenuLine(EditPopupMenuC->Items);
}
//---------------------------------------------------------------------------
//エディットのポップアップメニュー
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPopupMenuEPopup(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	int tag = (class_is_LabeledEdit(wp) && !USAME_TS(Screen->ActiveForm->Name, "OptionDlg"))? wp->Tag : 0;

	PopRefFmtItem->Visible	 = (tag & EDTAG_DST_FMT);
	PopRefFmtItem->Enabled	 = PopRefFmtItem->Visible;

	RegExpEItem->Visible	 = (tag & EDTAG_RGEX_V);
	RegExpEItem->Enabled	 = (tag & EDTAG_RGEX_E);

	bool dd_sw = wp && USAME_TI(wp->Name, "DirDelimiterEdit");
	PopRefDlmtItem->Visible  = dd_sw;
	PopRefDlmtItem->Enabled  = dd_sw;

	PopEditItemItem->Visible = wp && (StartsStr(". で区切って", wp->Hint) || StartsStr("; で区切って", wp->Hint));
	PopEditItemItem->Enabled = PopEditItemItem->Visible;

	reduction_MenuLine(EditPopupMenuE->Items);
}

//---------------------------------------------------------------------------
//コマンドパラメータの参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::CmdParamItemClick(TObject *Sender)
{
	UnicodeString lbuf = get_tkn(((TMenuItem*)Sender)->Caption, _T(" : "));
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelText = lbuf;
}
//---------------------------------------------------------------------------
//正規表現の参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefRegExItemClick(TObject *Sender)
{
	UnicodeString lbuf = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		cp->SelText = lbuf;
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->SelText = lbuf;
}
//---------------------------------------------------------------------------
//ディレクトリ区切りの参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopDlmtItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->Text = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
}
//---------------------------------------------------------------------------
//振分書式文字列の参照
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopDFmtItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->SelText = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
}

//---------------------------------------------------------------------------
//設定項目の編集
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopEditItemItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		if (!EditItemDlg) EditItemDlg = new TEditItemDlg(this);	//初回に動的作成
		EditItemDlg->AssignItems(ep);
		if (EditItemDlg->ShowModal()==mrOk && !EditItemDlg->RetStr.IsEmpty()) {
			ep->Text = EditItemDlg->RetStr;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditItemItemClick(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		if (!EditItemDlg) EditItemDlg = new TEditItemDlg(this);	//初回に動的作成
		EditItemDlg->AssignItems(cp);
		if (EditItemDlg->ShowModal()==mrOk && !EditItemDlg->RetStr.IsEmpty()) {
			cp->Text = EditItemDlg->RetStr;
		}
	}
}

//---------------------------------------------------------------------------
//画像フィルタを設定
//---------------------------------------------------------------------------
void __fastcall TUserModule::SetOpenImgFilter(UnicodeString fnam)
{
	UnicodeString flt_str;
	flt_str.USET_T("ビットマップ (*.bmp)|*.BMP");
	flt_str.UCAT_T("|JPEGファイル(*.jpg)|*.JPG;*.JPE;*.JPEG");
	flt_str.UCAT_T("|PNGファイル(*.png)|*.PNG");
	flt_str.UCAT_T("|GIFファイル(*.gif)|*.GIF");
	OpenImgDlg->Filter = flt_str;

	UnicodeString ext = ExtractFileExt(fnam);
	if (!ext.IsEmpty()) {
		OpenImgDlg->FilterIndex =
			test_FileExt(ext, _T(".bmp"))? 			1 :
			test_FileExt(ext, _T(".jpg.jpe.jpeg"))?	2 :
			test_FileExt(ext, _T(".png"))?			3 :
			test_FileExt(ext, _T(".gif"))?			4 : 1;
	}
}

//---------------------------------------------------------------------------
//ディレクトリの選択
//---------------------------------------------------------------------------
bool __fastcall TUserModule::SelectDirEx(const _TCHAR *tit, UnicodeString &dnam,
	bool to_rel)	//実行ディレクトリからの相対指定に変換 (default = false);
{
	if (!SelectDirectory(tit, EmptyStr, dnam, TSelectDirExtOpts() << sdNewUI << sdShowShares, Screen->ActiveForm)) return false;
	if (!dir_exists(dnam)) {
		msgbox_WARN(USTR_DirNotFound);
		return false;
	}

	if (to_rel) {
		remove_top_text(dnam, ExePath);
		if (!dnam.IsEmpty()) dnam = IncludeTrailingPathDelimiter(dnam);
	}

	return true;
}

//---------------------------------------------------------------------------
//フィルタからデフォルトの拡張子を取得
//---------------------------------------------------------------------------
UnicodeString __fastcall TUserModule::get_DlgDefExt(UnicodeString filter)
{
	UnicodeString fext;

	TStringDynArray f_buf = SplitString(filter, "|");
	if (f_buf.Length>=2) {
		fext = ExtractFileExt(get_tkn(f_buf[1], ';')).LowerCase();
		fext = test_FileExt(fext, _T(".txt.ini.csv.nwl.wav.zip"))? get_tkn_r(fext, '.') : EmptyStr;
	}

	return fext;
}

//---------------------------------------------------------------------------
//開くダイアログの準備
//---------------------------------------------------------------------------
void __fastcall TUserModule::PrepareOpenDlg(
	const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam, UnicodeString inidir)
{
	OpenDlg->Title		= tit;
	OpenDlg->Filter 	= filter? filter : null_TCHAR;
	OpenDlg->DefaultExt = get_DlgDefExt(OpenDlg->Filter);
	OpenDlg->FileName	= fnam? fnam : null_TCHAR;
	OpenDlg->InitialDir = def_if_empty(inidir, ExePath);
}
//---------------------------------------------------------------------------
//保存ダイアログの準備
//---------------------------------------------------------------------------
void __fastcall TUserModule::PrepareSaveDlg(
	const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam, UnicodeString inidir)
{
	SaveDlg->Title		= tit;
	SaveDlg->Filter 	= filter? filter : null_TCHAR;
	SaveDlg->DefaultExt = get_DlgDefExt(SaveDlg->Filter);
	SaveDlg->FileName	= fnam? fnam : null_TCHAR;
	SaveDlg->InitialDir = def_if_empty(inidir, ExePath);
}

//---------------------------------------------------------------------------
//開くダイアログの結果を設定
//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenDlgToEdit(
	TWinControl *ep,
	bool to_rel)	//実行ディレクトリからの相対指定に変換 (default = false);
{
	if (!OpenDlg->Execute()) return false;

	UnicodeString fnam = OpenDlg->FileName;
	if (to_rel) remove_top_text(fnam, ExePath);

	if (ep->InheritsFrom(__classid(TCustomEdit))) {
		((TCustomEdit *)ep)->Text = fnam;
		return true;
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenImgDlgToEdit(TWinControl *ep, bool to_rel)
{
	if (!OpenImgDlg->Execute()) return false;

	UnicodeString fnam = OpenImgDlg->FileName;
	if (to_rel) remove_top_text(fnam, ExePath);

	if (ep->InheritsFrom(__classid(TCustomEdit))) {
		((TCustomEdit *)ep)->Text = fnam;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenDlgToStr(UnicodeString &s, bool to_rel)
{
	if (!OpenDlg->Execute()) return false;

	s = OpenDlg->FileName;
	if (to_rel) remove_top_text(s, ExePath);
	return true;
}

//---------------------------------------------------------------------------
//フォントダイアログの結果をフォントに設定
//---------------------------------------------------------------------------
bool __fastcall TUserModule::FontDlgToFont(TFont *f)
{
	FontDlg->Font->Assign(f);
	if (!UserModule->FontDlg->Execute()) return false;

	f->Assign(FontDlg->Font);
	return true;
}

//---------------------------------------------------------------------------
//正規表現チェッカー
//---------------------------------------------------------------------------
void __fastcall TUserModule::RegExCheckerActionExecute(TObject *Sender)
{
	if (!RegExChecker) RegExChecker = new TRegExChecker(this);	//初回に動的作成

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		RegExChecker->ObjComboBox = cp;
		RegExChecker->PatternStr  = cp->Text;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		RegExChecker->ObjCustomEdit = ep;
		RegExChecker->PatternStr	= ep->Text;
	}

	if (!RegExChecker->Visible) RegExChecker->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RegExCheckerActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		ap->Visible = (cp && (cp->Tag & CBTAG_RGEX_V));
		ap->Enabled = (cp && (cp->Tag & CBTAG_RGEX_E));
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		ap->Visible = (ep && (ep->Tag & EDTAG_RGEX_V));
		ap->Enabled = (ep && (ep->Tag & EDTAG_RGEX_E));
		return;
	}

	ap->Visible = false;
}

//---------------------------------------------------------------------------
//電卓
//---------------------------------------------------------------------------
void __fastcall TUserModule::CalculatorActionExecute(TObject *Sender)
{
	if (!Calculator) Calculator = new TCalculator(this);	//初回に動的作成

	Calculator->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::CalculatorActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		ap->Visible = (cp && (cp->Tag & CBTAG_NO_CALC)==0);
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		ap->Visible = (ep && (ep->Tag & EDTAG_NO_CALC)==0);
		return;
	}

	ap->Visible = false;
}

//---------------------------------------------------------------------------
//アクティブなフォームのサイズと位置をファイルリストに合わせる
//---------------------------------------------------------------------------
void __fastcall TUserModule::SizePosToFlieListActionExecute(TObject *Sender)
{
	TForm *frm = Screen->ActiveForm;
	if (frm && (frm->BorderStyle==bsSizeable || frm->BorderStyle==bsSizeToolWin)) {
		set_window_pos_ex(frm->Handle, FileListRect);
	}
}

//---------------------------------------------------------------------------
//コンボボックスの選択状態を保存
//---------------------------------------------------------------------------
void __fastcall TUserModule::SaveLastComboBox()
{
	if (Application->Active) {
		LastComboBox   = GetActiveComboBox();
		LastComboBoxSS = LastComboBox? LastComboBox->SelStart  : 0;
		LastComboBoxSL = LastComboBox? LastComboBox->SelLength : 0;
	}
}
//---------------------------------------------------------------------------
//コンボボックスの選択状態を復帰
//---------------------------------------------------------------------------
void __fastcall TUserModule::RestoreLastComboBox()
{
	if (Application->Active) {
		TComboBox *cp = GetActiveComboBox();
		if (cp && cp==LastComboBox) {
			LastComboBox->SelStart	= LastComboBoxSS;
			LastComboBox->SelLength = LastComboBoxSL;
		}
		LastComboBox = NULL;
	}
}
//---------------------------------------------------------------------------

