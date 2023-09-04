//----------------------------------------------------------------------//
// ヒントウィンドウ														//
//  背景色変更のためにサブクラス化										//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_scale.h"
#include "usr_hintwin.h"

//---------------------------------------------------------------------------
void __fastcall UsrHintWindow::Paint(void)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = Caption;
	if (!lst->Text.IsEmpty()) {
		UnicodeString tbuf;
		for (int i=0; i<lst->Count; i++) {
			if (lst->Strings[i].IsEmpty()) continue;
			if (tbuf.IsEmpty())
				tbuf = lst->Strings[i];
			else {
				tbuf = EmptyStr; break;
			}
		}

		TRect rc = ClientRect;
		InflateRect(rc, -4, -2);

		UINT fmt = (!tbuf.IsEmpty() && Canvas->TextWidth(tbuf)<rc.Width())? DT_CENTER : DT_WORDBREAK;
		::DrawText(Canvas->Handle, Caption.c_str(), -1, &rc, fmt);
	}
}

//---------------------------------------------------------------------------
void __fastcall UsrHintWindow::ActivateHintEx(
	UnicodeString msg,	//メッセージ
	int max_w,			//最大幅
	int min_w,			//最小幅
	TControl *cp,		//基準コントロール
	TColor bg_col)		//背景色
{
	if (!cp) return;

	max_w = SCALED_THIS(max_w);
	min_w = SCALED_THIS(min_w);

	Screen->HintFont->Assign(Font);
	TRect rc = CalcHintRect(max_w, msg, NULL);
	rc.Right += SCALED_THIS(8);
	if (rc.Width()<min_w) rc.SetWidth(min_w);

	TPoint p = cp->ClientToScreen(cp->ClientRect.CenteredRect(rc).Location);
	int mon_left = Screen->MonitorFromPoint(cp->ClientToScreen(Point(0, 0)) , mdNearest)->Left;
	if (p.x<mon_left) p.x = mon_left;
	rc.Offset(p.x, p.y);

	Brush->Color = bg_col;
	ActivateHint(rc, msg);
	Repaint();
}
//---------------------------------------------------------------------------
