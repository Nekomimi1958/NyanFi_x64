//----------------------------------------------------------------------//
// NyanFi																//
//  置換確認ダイアログ													//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "Global.h"
#include "AskRep.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAskRepDlg *AskRepDlg = NULL;

//---------------------------------------------------------------------
__fastcall TAskRepDlg::TAskRepDlg(TComponent* AOwner)
	: TForm(AOwner)
{
	FoundPos = FoundLine = 0;
	CurIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	ListScrPanel = new UsrScrollPanel(ListPanel, RepListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
}
//---------------------------------------------------------------------
void __fastcall TAskRepDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	set_ListBoxItemHi(RepListBox, GrepResFont);
	set_UsrScrPanel(ListScrPanel);
	RepListBox->Color = col_bgList;
	ListScrPanel->UpdateKnob();

	StatusBar1->Font->Assign(ViewHdrFont);
	StatusBar1->Panels->Items[0]->Text = yen_to_delimiter(FileName).cat_sprintf(_T(" - %u行 %u桁"), FoundLine, FoundPos);
	StatusBar1->Panels->Items[1]->Text = UnicodeString().sprintf(_T(" File %u/%u"), FileIndex + 1, FileCount);

	SetDarkWinTheme(this);

	OKBtn->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
}
//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::FormResize(TObject *Sender)
{
	RepListBox->Invalidate();
	StatusBar1->Panels->Items[0]->Width = ClientWidth - SCALED_THIS(120);
}

//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}

//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Font->Assign(StatusBar->Font);
	cv->Brush->Color = col_bgInfHdr;
	cv->FillRect(Rect);
	cv->Font->Color  = col_fgInfHdr;
	cv->TextOut(Rect.Left + SCALED_THIS(2), Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::RepListBoxDrawItem(TWinControl *Control,
		int Index, TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	UnicodeString itmstr = lp->Items->Strings[Index];
	int xp = Rect.Left + SCALED_THIS(2);
	int yp = Rect.Top  + get_TopMargin(cv);

	//行番号
	TRect rc = Rect;
	LineNoOut(cv, rc, FoundLine - CurIndex + Index);
	cv->Brush->Color = col_bgList;
	xp = rc.Left + 4;

	//マッチ行
	if (Index==CurIndex) {
		cv->Font->Color = col_fgList;
		if (FoundPos>0) {
			//マッチ語前
			UnicodeString s0 = itmstr.SubString(1, FoundPos - 1);
			int wd = Rect.Width() - xp;
			if (get_TextWidth(cv, s0 + KeyWord, is_irreg) > wd) {
				int p1 = FoundPos;
				wd /= 2;
				int w_r = get_TextWidth(cv, itmstr.SubString(p1, itmstr.Length() - p1 + 1), is_irreg);
				if (w_r<wd) wd += (wd - w_r);
				wd -= get_TextWidth(cv, "… ", is_irreg);
				int w = 0;
				for (int i=p1; i>4; i--) {
					w += get_TextWidth(cv, itmstr.SubString(i, 1), is_irreg);
					if (w<wd) continue;
					s0.Delete(1, i);
					s0.Insert("…", 1);
					break;
				}
			}
			TabCrTextOut(s0, cv, xp, yp, col_fgList, Rect.Right);
			itmstr.Delete(1, FoundPos - 1);
			//マッチ語強調表示
			EmphasisTextOut(itmstr, KeyWord, cv, xp, yp, false, true);
		}
		else {
			TabCrTextOut(itmstr, cv, xp, yp, col_fgList, Rect.Right);
		}

		//カーソル線
		int lw = std::max(CursorWidth, 1);
		int yp = Rect.Bottom - lw;
		draw_Line(cv, Rect.Left, yp, Rect.Right, yp, lw, col_Cursor);
	}
	//前後行
	else {
		TabCrTextOut(itmstr, cv, xp, yp, col_fgList, Rect.Right);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAskRepDlg::RepListBoxEnter(TObject *Sender)
{
	OKBtn->SetFocus();
}
//---------------------------------------------------------------------------

