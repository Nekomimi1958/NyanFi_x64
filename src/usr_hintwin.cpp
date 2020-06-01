//----------------------------------------------------------------------//
// �q���g�E�B���h�E														//
//  �w�i�F�ύX�̂��߂ɃT�u�N���X��										//
//----------------------------------------------------------------------//
#include <memory>
#include "usr_str.h"
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
	UnicodeString msg,	//���b�Z�[�W
	int max_w,			//�ő啝
	int min_w,			//�ŏ���
	TControl *cp,		//��R���g���[��
	TColor bg_col)		//�w�i�F
{
	if (!cp) return;

	std::unique_ptr<TFont> font_buf(new TFont());
	font_buf->Assign(Screen->HintFont);
	Screen->HintFont->Assign(Font);
	Color = bg_col;

	TRect rc = CalcHintRect(max_w, msg, NULL);  rc.Right += 8;
	if (rc.Width()<min_w) rc.SetWidth(min_w);

	TPoint p = cp->ClientToScreen(cp->ClientRect.CenteredRect(rc).Location);
	int mon_left = Screen->MonitorFromPoint(cp->ClientToScreen(Point(0, 0)) , mdNearest)->Left;
	if (p.x<mon_left) p.x = mon_left;
	rc.Offset(p.x, p.y);

	ActivateHint(rc, msg);
	Repaint();

	Screen->HintFont->Assign(font_buf.get());
}
//---------------------------------------------------------------------------
