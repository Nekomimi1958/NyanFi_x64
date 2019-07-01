//----------------------------------------------------------------------//
// NyanFi																//
//  グローバル(ダークモード対応)										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include <memory>
#include "UserFunc.h"
#include "GlobalDark.h"

//---------------------------------------------------------------------------
//非公開API
HMODULE hUxTheme = NULL;
FUNC_ShouldAppsUseDarkMode	lpfShouldAppsUseDarkMode  = NULL;
FUNC_AllowDarkModeForWindow	lpfAllowDarkModeForWindow = NULL;
FUNC_AllowDarkModeForApp	lpfAllowDarkModeForApp	  = NULL;
FUNC_FlushMenuThemes		lpfFlushMenuThemes		  = NULL;

bool SupportDarkMode = false;		//ダークモード適用可能
bool IsDarkMode 	 = false;		//ダークモードが適用されている
bool AllowDarkMode   = false;		//ダークモードを適用(オプション設定)

TColor col_Invalid;		//無効な項目の背景色
TColor col_Illegal;		//不正な入力項目の背景色

TColor col_DkPanel;		//ダークモード: パネルの背景色
TColor col_DkInval;		//ダークモード: 無効な項目の背景色
TColor col_DkIlleg;		//ダークモード: 不正な入力項目の背景色

const TColor col_None = Graphics::clNone;

//---------------------------------------------------------------------------
//システム色
TColor scl_Window;
TColor scl_WindowText;
TColor scl_Highlight;
TColor scl_HighlightText;
TColor scl_BtnFace;
TColor scl_BtnText;
TColor scl_Menu;
TColor scl_MenuText;

//ダーク色
TColor dcl_Window;
TColor dcl_WindowText;
TColor dcl_Highlight;
TColor dcl_HighlightText;
TColor dcl_BtnFace;
TColor dcl_BtnText;
TColor dcl_Menu;
TColor dcl_MenuText;

//---------------------------------------------------------------------------
//システム色の初期化
//---------------------------------------------------------------------------
void InitializeSysColor()
{
	scl_Window		  = clWindow;
	scl_WindowText	  = clWindowText;
	scl_Highlight	  = clHighlight;
	scl_HighlightText = clHighlightText;
	scl_BtnFace		  = clBtnFace;
	scl_BtnText		  = clBtnText;
	scl_Menu		  = clMenu;
	scl_MenuText	  = clMenuText;

	dcl_Window		  = TColor(RGB(0x20, 0x20, 0x20));
	dcl_WindowText	  = clWhite;
	dcl_Highlight	  = TColor(RGB(0x41, 0x41, 0x41));
	dcl_HighlightText = clWhite;
	dcl_BtnFace 	  = TColor(RGB(0x33, 0x33, 0x33));
	dcl_BtnText 	  = clWhite;
	dcl_Menu		  = TColor(RGB(0x2b, 0x2b, 0x2b));
	dcl_MenuText	  = clWhite;
}

//---------------------------------------------------------------------------
void InitializeDarkMode()
{
	//ダークモード初期化
	if (CheckWin32Version(10) && TOSVersion::Build >= 17763 && !is_HighContrast()) {
		hUxTheme = ::LoadLibrary(_T("uxtheme.dll"));
		if (hUxTheme) {
			lpfShouldAppsUseDarkMode  = (FUNC_ShouldAppsUseDarkMode)::GetProcAddress(hUxTheme, MAKEINTRESOURCEA(132));
			lpfAllowDarkModeForWindow = (FUNC_AllowDarkModeForWindow)::GetProcAddress(hUxTheme, MAKEINTRESOURCEA(133));
			lpfAllowDarkModeForApp	  = (FUNC_AllowDarkModeForApp)::GetProcAddress(hUxTheme, MAKEINTRESOURCEA(135));
			lpfFlushMenuThemes		  = (FUNC_FlushMenuThemes)::GetProcAddress(hUxTheme, MAKEINTRESOURCEA(136));
			SupportDarkMode = (lpfShouldAppsUseDarkMode && lpfAllowDarkModeForWindow
								&& lpfAllowDarkModeForApp && lpfFlushMenuThemes);
		}
	}

	InitializeSysColor();
}
//---------------------------------------------------------------------------
void EndDarkMode()
{
	if (hUxTheme) ::FreeLibrary(hUxTheme);
}

//---------------------------------------------------------------------------
//ダークモードを適用
//---------------------------------------------------------------------------
bool ApplyDarkMode()
{
	IsDarkMode = false;
	if (SupportDarkMode && lpfShouldAppsUseDarkMode()) {
		lpfAllowDarkModeForApp(AllowDarkMode);
		lpfFlushMenuThemes();
		IsDarkMode = AllowDarkMode;
	}
	return IsDarkMode;
}

//---------------------------------------------------------------------------
//ダークモードを考慮した色の取得
//---------------------------------------------------------------------------
TColor get_TextColor(
	bool hl)	//ハイライト	(default = false)
{
	return hl? (IsDarkMode? dcl_HighlightText : scl_HighlightText) : (IsDarkMode? dcl_WindowText : scl_WindowText);
}
//---------------------------------------------------------------------------
TColor get_PanelColor(
	bool is_inv)	//無効な項目	(default = false)
{
	if (is_inv) return (IsDarkMode? col_DkInval : col_Invalid);
	return (IsDarkMode? ((col_DkPanel!=col_None)? col_DkPanel : dcl_BtnFace) : scl_BtnFace);
}
//---------------------------------------------------------------------------
TColor get_LabelColor()
{
	return (IsDarkMode? dcl_BtnText : scl_BtnText);
}
//---------------------------------------------------------------------------
TColor get_WinColor(
	bool is_inv)	//無効な項目	(default = false)
{
	return is_inv? (IsDarkMode? col_DkInval : col_Invalid) : (IsDarkMode? dcl_Window : scl_Window);
}

//---------------------------------------------------------------------------
//ハイライト色の設定
//---------------------------------------------------------------------------
void SetHighlight(TCanvas *cv, bool hl,
	bool is_dark)	//ダークモード	(default = false);
{
	if (is_dark) {
		cv->Brush->Color = hl? dcl_Highlight : dcl_Window;
		cv->Font->Color  = hl? dcl_HighlightText : dcl_WindowText;
	}
	else {
		cv->Brush->Color = hl? scl_Highlight : scl_Window;
		cv->Font->Color  = hl? scl_HighlightText : scl_WindowText;
	}
}

//---------------------------------------------------------------------------
//コントロールにダークモードを適用
//---------------------------------------------------------------------------
void SetDarkWinTheme(TWinControl *wp)
{
	if (!wp) return;
	if (wp->ClassNameIs("TStatusBar")) return;

	TColor bg_panel = get_PanelColor();
	TColor fg_label = get_LabelColor();
	TColor bg_win	= get_WinColor();
	TColor fg_txt   = get_TextColor();

	if (wp->InheritsFrom(__classid(TForm))) {
		TForm *fp = (TForm*)wp;
		fp->Color = bg_panel;
		for (int i=0; i<fp->ControlCount; i++) {
			TControl *cp = wp->Controls[i];
			if (cp->ClassNameIs("TLabel")) {
				TLabel *lp = (TLabel *)cp;
				lp->Color		= bg_panel;
				lp->Font->Color = fg_label;
			}
			else if (cp->InheritsFrom(__classid(TWinControl))) {
				SetDarkWinTheme((TWinControl *)cp);
			}
		}
	}
	else if (wp->ClassNameIs("TPageControl")) {
		TPageControl *pp = (TPageControl *)wp;
		for (int i=0; i<pp->PageCount; i++) SetDarkWinTheme(pp->Pages[i]);
	}
	else if (wp->ClassNameIs("TPanel") || wp->ClassNameIs("TTabSheet")
		|| wp->ClassNameIs("TGroupBox") || wp->ClassNameIs("TRadioGroup")
		|| wp->ClassNameIs("TTabControl"))
	{
		if (wp->ClassNameIs("TPanel")) {
			TPanel *pp = (TPanel *)wp;
			pp->StyleElements.Clear();
			pp->Color		= bg_panel;
			pp->Font->Color = fg_label;
		}
		else if (wp->ClassNameIs("TGroupBox") || wp->ClassNameIs("TRadioGroup")) {
			AttachLabelToGroup(wp);
		}

		//ラジオボタン・グループ
		if (wp->ClassNameIs("TRadioGroup")) {
			if (IsDarkMode) {
				TRadioGroup *gp = (TRadioGroup *)wp;
				for (int i=0; i<gp->Items->Count; i++) {
					if (!gp->Items->Strings[i].IsEmpty()) {
						//ボタンのキャプションをラベルに置き換える
						TRadioButton *cp = gp->Buttons[i];
						TLabel *lp = new TLabel(cp->Parent);
						lp->Parent	= cp->Parent;
						lp->Font->Assign(cp->Font);
						lp->Color		= bg_panel;
						lp->Font->Color = fg_label;
						lp->Caption = cp->Caption;
						cp->Caption = EmptyStr;
						lp->Left	= cp->Left + 16;
						lp->Top 	= cp->Top + (cp->Height - lp->Height)/2;
						gp->Items->Strings[i] = EmptyStr;
						cp->Caption = EmptyStr;
						lp->BringToFront();
					}
				}
			}

			for (int i=0; i<wp->ControlCount; i++) {
				TControl *cp = wp->Controls[i];
				if (cp->ClassNameIs("TLabel")) {
					TLabel *lp = (TLabel *)cp;
					lp->Color		= bg_panel;
					lp->Font->Color = fg_label;
				}
			}
		}
		//その他一般
		else {
			for (int i=0; i<wp->ControlCount; i++) {
				TControl *cp = wp->Controls[i];
				if (cp->ClassNameIs("TLabel")) {
					TLabel *lp = (TLabel *)cp;
					lp->Color		= bg_panel;
					lp->Font->Color = fg_label;
				}
				else if (cp->InheritsFrom(__classid(TWinControl))) {
					SetDarkWinTheme((TWinControl *)cp);
				}
			}
		}
	}
	//以下は単独の子コントロール
	else if (class_is_Edit(wp)) {
		TEdit *ep = (TEdit *)wp;
		ep->Color		= bg_win;
		ep->Font->Color = fg_txt;
	}
	else if (class_is_LabeledEdit(wp)) {
		TLabeledEdit *ep = (TLabeledEdit *)wp;
		ep->Color		= bg_win;
		ep->Font->Color = fg_txt;
		ep->EditLabel->Font->Color = ep->Font->Color;
	}
	else if (class_is_MaskEdit(wp)) {
		TMaskEdit *ep = (TMaskEdit *)wp;
		ep->Color		= bg_win;
		ep->Font->Color = fg_txt;
	}
	else if (wp->ClassNameIs("TMemo")) {
		::SetWindowTheme(wp->Handle, IsDarkMode? _T("DarkMode_Explorer") : NULL, NULL);
	}
	else if (wp->ClassNameIs("TSpeedButton")) {
		TSpeedButton *bp = (TSpeedButton *)wp;
		bp->Font->Color = fg_label;
	}
	else if (class_is_ComboBox(wp)) {
		::SetWindowTheme(wp->Handle, IsDarkMode? _T("DarkMode_CFD") : NULL, NULL);
		TComboBox *cp = (TComboBox *)wp;
		cp->Color		= bg_win;
		cp->Font->Color = fg_txt;
	}
	else if (class_is_CheckBox(wp)) {
		if (IsDarkMode) {
			TCheckBox *cp = (TCheckBox *)wp;
			if (!cp->Caption.IsEmpty()) {
				//キャプションをラベルに置き換える
				TLabel *lp = new TLabel(cp->Parent);
				lp->Parent	= cp->Parent;
				lp->Font->Assign(cp->Font);
				lp->Color		= bg_panel;
				lp->Font->Color = fg_label;
				lp->Caption = cp->Caption;
				lp->Anchors = cp->Anchors;
				lp->Left	= cp->Left + 16;
				lp->Top 	= cp->Top;
				cp->Caption = EmptyStr;
				lp->BringToFront();
			}
		}
	}
	else if (wp->ClassNameIs("TRadioButton")) {
		if (IsDarkMode) {
			TRadioButton *cp = (TRadioButton *)wp;
			if (!cp->Caption.IsEmpty()) {
				//キャプションをラベルに置き換える
				TLabel *lp = new TLabel(cp->Parent);
				lp->Parent	= cp->Parent;
				lp->Font->Assign(cp->Font);
				lp->Color		= bg_panel;
				lp->Font->Color = fg_label;
				lp->Caption = cp->Caption;
				lp->Left	= cp->Left + 16;
				lp->Top 	= cp->Top;
				cp->Caption = EmptyStr;
				lp->BringToFront();
			}
		}
	}
	else {
		::SetWindowTheme(wp->Handle, IsDarkMode? _T("DarkMode_Explorer") : NULL, NULL);
	}
}

//---------------------------------------------------------------------------
//TGroupBox/TRadioGroup にキャプション疑似表示用のラベルを追加
//---------------------------------------------------------------------------
TLabel* AttachLabelToGroup(TWinControl *wp,
	UnicodeString s)	//キャプション	(default = EmptyStr)
{
	TLabel *ret_p = NULL;

	UnicodeString cap_str;
	TWinControl *parent;
	TFont		*font;
	int xp, yp;
	int size8 = (int)(8 * Screen->PixelsPerInch / 96.0);

	if (wp->ClassNameIs("TGroupBox")) {
		TGroupBox *gp = (TGroupBox *)wp;
		parent = gp->Parent;
		font = gp->Font;
		xp = gp->Left + size8;
		yp = gp->Top - 1;
		gp->Color		= get_PanelColor();
		gp->Font->Color = get_LabelColor();
		if (s.IsEmpty()) s = gp->Caption;
		if (!gp->Caption.IsEmpty()) gp->Caption = " ";
	}
	else if (wp->ClassNameIs("TRadioGroup")) {
		TRadioGroup *gp = (TRadioGroup *)wp;
		parent = gp->Parent;
		font = gp->Font;
		xp = gp->Left + size8;
		yp = gp->Top - 1;
		gp->Color		= get_PanelColor();
		gp->Font->Color = get_LabelColor();
		if (s.IsEmpty()) s = gp->Caption;
		if (!gp->Caption.IsEmpty()) gp->Caption = " ";
	}
	else {
		return NULL;
	}

	if (!Trim(s).IsEmpty()) {
		ret_p		= new TLabel(parent);
		TPanel *pp	= new TPanel(parent);
		pp->Parent	= parent;
		ret_p->Font->Assign(font);
		ret_p->Color	   = get_PanelColor();
		ret_p->Font->Color = get_LabelColor();
		pp->Left		   = xp;
		pp->Top 		   = yp;
		pp->Color		   = get_PanelColor();
		pp->Caption 	   = EmptyStr;
		pp->BorderWidth    = 1;
		pp->BevelOuter	   = bvNone;
		pp->StyleElements.Clear();
		ret_p->Caption	   = s;
		ret_p->Parent	   = pp;
		pp->AutoSize	   = true;
		pp->BringToFront();
	}

	return ret_p;
}

//---------------------------------------------------------------------------
//エラーメッセージ
//---------------------------------------------------------------------------
void msgbox_ERR(UnicodeString msg)
{
	if (msg.IsEmpty()) return;

	TForm *MsgDlg = CreateMessageDialog(msg, mtError, TMsgDlgButtons() << mbOK, mbOK);
	SetDarkWinTheme(MsgDlg);
	MsgDlg->ShowModal();
	delete MsgDlg;
}
//---------------------------------------------------------------------------
void msgbox_ERR(unsigned id)
{
	msgbox_ERR(LoadUsrMsg(id));
}

//---------------------------------------------------------------------------
//警告メッセージ
//---------------------------------------------------------------------------
void msgbox_WARN(UnicodeString msg)
{
	if (msg.IsEmpty()) return;

	TForm *MsgDlg = CreateMessageDialog(msg, mtWarning, TMsgDlgButtons() << mbOK, mbOK);
	SetDarkWinTheme(MsgDlg);
	MsgDlg->ShowModal();
	delete MsgDlg;
}
//---------------------------------------------------------------------------
void msgbox_WARN(unsigned id)
{
	msgbox_WARN(LoadUsrMsg(id));
}

//---------------------------------------------------------------------------
//確認メッセージ
//---------------------------------------------------------------------------
void msgbox_OK(UnicodeString msg, UnicodeString tit)
{
	TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation, TMsgDlgButtons() << mbOK, mbOK);
	SetDarkWinTheme(MsgDlg);
	MsgDlg->ShowModal();
	delete MsgDlg;
}
//---------------------------------------------------------------------------
bool msgbox_Y_N(UnicodeString msg, UnicodeString tit)
{
	TMsgDlgButtons opt = TMsgDlgButtons() << mbYes << mbNo;
	TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation, opt, SureDefNo? mbNo : mbYes);
	SetDarkWinTheme(MsgDlg);
	int res = MsgDlg->ShowModal();
	delete MsgDlg;
	return (res==mrYes);
}
//---------------------------------------------------------------------------
TModalResult msgbox_Y_N_C(UnicodeString msg, UnicodeString tit)
{
	TMsgDlgButtons opt = TMsgDlgButtons() << mbYes << mbNo << mbCancel;
	TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation, opt, SureDefNo? mbNo : mbYes);
	SetDarkWinTheme(MsgDlg);
	TModalResult res = MsgDlg->ShowModal();
	delete MsgDlg;
	return res;
}

//---------------------------------------------------------------------------
bool msgbox_Sure(UnicodeString msg, bool ask, bool center)
{
	if (!ask) return true;

	TMsgDlgButtons opt = TMsgDlgButtons() << mbYes << mbNo;
	if (SureCancel) opt << mbCancel; 
	TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation, opt, SureDefNo? mbNo : mbYes);
	SetDarkWinTheme(MsgDlg);

	MsgPosCenter = center;
	int res = MsgDlg->ShowModal();
	MsgPosCenter = false;
	delete MsgDlg;

	return (res==mrYes);
}
//---------------------------------------------------------------------------
bool msgbox_Sure(const _TCHAR *msg, bool ask, bool center)
{
	return msgbox_Sure(UnicodeString(msg), ask, center);
}
//---------------------------------------------------------------------------
bool msgbox_Sure(int id, bool ask, bool center)
{
	if (!ask) return true;
	return msgbox_Sure(LoadUsrMsg(id), ask, center);
}

//---------------------------------------------------------------------------
//「すべてに適用」チェックボックス付き確認メッセージ
//---------------------------------------------------------------------------
int msgbox_SureAll(UnicodeString msg, bool &app_chk, bool center)
{
	TForm *MsgDlg = CreateMessageDialog(msg, mtConfirmation,
						TMsgDlgButtons() << mbYes << mbNo << mbCancel, 
						SureDefNo? mbNo : mbYes);

	//「すべてに適用」チェックボックスを追加
	TCheckBox *cp = new TCheckBox(MsgDlg);
	MsgDlg->ClientHeight = MsgDlg->ClientHeight + cp->Height + 12;
	SetDarkWinTheme(MsgDlg);

	cp->Caption = "すべてに適用(&A)";
	cp->Parent	= MsgDlg;
	cp->Left	= 20;
	cp->Top		= MsgDlg->ClientHeight - cp->Height - 12;
	cp->Width	= MsgDlg->ClientWidth - cp->Left;
	if (IsDarkMode) {
		TLabel *lp	= new TLabel(MsgDlg);
		TPanel *pp	= new TPanel(MsgDlg);
		pp->Parent	= MsgDlg;
		lp->Font->Assign(MsgDlg->Font);
		lp->Color		= get_PanelColor();
		lp->Font->Color = get_LabelColor();
		pp->Left		= cp->Left + 14;
		pp->Top 		= cp->Top  - 2;
		pp->Color		= get_PanelColor();
		pp->Caption 	= EmptyStr;
		pp->BorderWidth = 2;
		pp->BevelOuter	= bvNone;
		pp->StyleElements.Clear();
		lp->Caption 	= cp->Caption;
		lp->Parent		= pp;
		pp->AutoSize	= true;
		pp->BringToFront();
	}

	MsgPosCenter = center;
	int res = MsgDlg->ShowModal();
	MsgPosCenter = false;

	app_chk = cp->Checked;
	delete MsgDlg;

	return res;
}

//---------------------------------------------------------------------------
//以下は擬似的コントロールのアクセラレータ処理に利用
//---------------------------------------------------------------------------
bool set_focus_RadioGroup(TRadioGroup *gp)
{
	if (!gp->Parent->Visible || !gp->Parent->Enabled || !gp->Visible || !gp->Enabled) return false;

	if (gp->ItemIndex==-1) gp->ItemIndex = 0;
	gp->Buttons[gp->ItemIndex]->SetFocus();
	return true;
}
//---------------------------------------------------------------------------
bool set_focus_GroupBox(TGroupBox *gp)
{
	if (!gp->Parent->Visible || !gp->Parent->Enabled || !gp->Visible || !gp->Enabled) return false;

	TWinControl *top_wp = NULL;
	for (int i=0; i<gp->ControlCount; i++) {
		TControl *cp = gp->Controls[i];
		if (!cp->InheritsFrom(__classid(TWinControl))) continue;
		TWinControl *wp = (TWinControl *)cp;
		if (!top_wp)
			top_wp = wp;
		else if (wp->TabOrder < top_wp->TabOrder)
			top_wp = wp;
	}

	if (!top_wp) return false;

	if (top_wp->ClassNameIs("TRadioGroup")) {
		set_focus_RadioGroup((TRadioGroup *)top_wp);
	}
	else {
		top_wp->SetFocus();
	}
	return true;
}

//---------------------------------------------------------------------------
void invert_CheckBox(TCheckBox *cp)
{
	if (!cp->Parent->Visible || !cp->Parent->Enabled || !cp->Visible || !cp->Enabled) return;

	if (cp->Caption.IsEmpty()) {	//ダークモード
		cp->SetFocus();
		cp->Checked = !cp->Checked;
	}
}

//---------------------------------------------------------------------------
//ボタンにマークを設定
//---------------------------------------------------------------------------
void set_ButtonMark(TSpeedButton *bp,
	int id,		//識別子	(default = UBMK_DOWN)
	TColor fg,	//前景色	(default = col_BtnText)
	TColor bg)	//背景色	(default = col_BtnFace)
{
	int size;
	switch (id) {
	case UBMK_HTOP: case UBMK_HEND: case UBMK_LEFT: case UBMK_RIGHT:
		size = 16;	break;
	default:
		size = 9;
	}

	Graphics::TBitmap *bmp = bp->Glyph;
	TCanvas *cv = bmp->Canvas;
	bmp->SetSize(size, size);
	bmp->Transparent = true;
	cv->Brush->Color = bg;
	cv->FillRect(TRect(0, 0, size, size));
	cv->Pen->Style = psSolid;
	cv->Pen->Color = fg;

	switch (id) {
	case UBMK_UP:
		draw_SortMark(cv, 0, 2, true,  fg);	break;
	case UBMK_DOWN:
		draw_SortMark(cv, 0, 2, false, fg);	break;

	case UBMK_BUP:
		cv->Pen->Width = 2;
		cv->MoveTo(1, 5);  cv->LineTo(4, 2);
		cv->MoveTo(7, 5);  cv->LineTo(4, 2);
		break;
	case UBMK_BDOWN:
		cv->Pen->Width = 2;
		cv->MoveTo(1, 2);  cv->LineTo(4, 5);
		cv->MoveTo(7, 2);  cv->LineTo(4, 5);
		break;

	case UBMK_VUP:
		cv->Pen->Width = 2;
		cv->MoveTo(1, 7);  cv->LineTo(4, 4);
		cv->MoveTo(7, 7);  cv->LineTo(4, 4);
		break;
	case UBMK_VDOWN:
		cv->Pen->Width = 2;
		cv->MoveTo(1, 2);  cv->LineTo(4, 5);
		cv->MoveTo(7, 2);  cv->LineTo(4, 5);
		break;

	case UBMK_VTOP:
		cv->Pen->Width = 1;
		cv->MoveTo(0, 2);  cv->LineTo(9, 2);
		cv->MoveTo(0, 3);  cv->LineTo(9, 3);
		cv->Pen->Width = 2;
		cv->MoveTo(1, 7);  cv->LineTo(4, 4);
		cv->MoveTo(7, 7);  cv->LineTo(4, 4);
		break;
	case UBMK_VEND:
		cv->Pen->Width = 1;
		cv->MoveTo(0, 7);  cv->LineTo(9, 7);
		cv->MoveTo(0, 6);  cv->LineTo(9, 6);
		cv->Pen->Width = 2;
		cv->MoveTo(1, 2);  cv->LineTo(4, 5);
		cv->MoveTo(7, 2);  cv->LineTo(4, 5);
		break;

	case UBMK_HTOP:
		cv->Pen->Width = 1;
		cv->MoveTo(4, 1);  cv->LineTo( 4, 14);
		cv->Pen->Width = 2;
		cv->MoveTo(6, 7);  cv->LineTo(11,  2);
		cv->MoveTo(6, 7);  cv->LineTo(11, 12);
		break;
	case UBMK_HEND:
		cv->Pen->Width = 1;
		cv->MoveTo(11, 1);  cv->LineTo(11, 14);
		cv->Pen->Width = 2;
		cv->MoveTo(9, 7);  cv->LineTo(4,  2);
		cv->MoveTo(9, 7);  cv->LineTo(4, 12);
		break;
	case UBMK_LEFT:
		cv->Pen->Width = 2;
		cv->MoveTo(6, 7);  cv->LineTo(11,  2);
		cv->MoveTo(6, 7);  cv->LineTo(11, 12);
		break;
	case UBMK_RIGHT:
		cv->Pen->Width = 2;
		cv->MoveTo(9, 7);  cv->LineTo(4,  2);
		cv->MoveTo(9, 7);  cv->LineTo(4, 12);
		break;
	}
}
//---------------------------------------------------------------------------
