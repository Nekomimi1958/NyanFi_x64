//----------------------------------------------------------------------//
// �V���v���X�N���[���o�[�E�p�l��										//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�Ɋ֘A�t��
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TListBox *lp, int flag)
{
	ParentPanel    = pp;
	AssoListBox    = lp;
	AssoChkListBox = NULL;
	AssoStrGrid    = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//�`�F�b�N���X�g�{�b�N�X�Ɋ֘A�t��
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TCheckListBox *lp, int flag)
{
	ParentPanel    = pp;
	AssoChkListBox = lp;
	AssoListBox    = NULL;
	AssoStrGrid    = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//�O���b�h�Ɋ֘A�t��
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TStringGrid *gp, int flag)
{
	ParentPanel    = pp;
	AssoStrGrid    = gp;
	AssoListBox    = NULL;
	AssoChkListBox = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//�����X�N���[���o�[�Ɋ֘A�t��
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TScrollBar *sp, int flag)
{
	ParentPanel    = pp;
	AssoScrollBar  = sp;
	AssoListBox    = NULL;
	AssoChkListBox = NULL;
	AssoStrGrid    = NULL;
	Flag		   = flag;

	InitializePanel();
}

//---------------------------------------------------------------------------
UsrScrollPanel::~UsrScrollPanel()
{
	ScrPanelV->WindowProc  = org_ScrPanelVWndProc;
	if (ScrPanelH && org_ScrPanelHWndProc) 	ScrPanelH->WindowProc	= org_ScrPanelHWndProc;

	if (org_ParentPanelWndProc)						ParentPanel->WindowProc    = org_ParentPanelWndProc;
	if (AssoListBox && org_AssoListWndProc) 		AssoListBox->WindowProc    = org_AssoListWndProc;
	if (AssoChkListBox && org_AssoChkListWndProc)	AssoChkListBox->WindowProc = org_AssoChkListWndProc;
	if (AssoStrGrid && org_AssoGridWndProc)			AssoStrGrid->WindowProc    = org_AssoGridWndProc;

	delete HitLines;
}

//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::InitializePanel()
{
	HitLines = new TStringList();

	KnobImgBuffV = NULL;
	KnobImgBuffH = NULL;

	OnRButtonUp  = NULL;

	KnobWidth = ::GetSystemMetrics(SM_CXVSCROLL);

	ListCsrVisible = false;

	//�����X�N���[���o�[
	ScrPanelV = new TPanel(ParentPanel);
	ScrPanelV->Parent		  = ParentPanel;
	ScrPanelV->DoubleBuffered = true;
	ScrPanelV->BevelOuter	  = bvNone;
	ScrPanelV->Align		  = alRight;
	ScrPanelV->Width		  = KnobWidth;

	org_ScrPanelVWndProc  = ScrPanelV->WindowProc;
	ScrPanelV->WindowProc = ScrPanelVWndProc;

	ScrPaintBoxV = new TPaintBox(ParentPanel);
	ScrPaintBoxV->Parent	  = ScrPanelV;
	ScrPaintBoxV->Align 	  = alClient;
	ScrPaintBoxV->OnPaint	  = ScrPaintBoxPaint;
	ScrPaintBoxV->OnMouseDown = ScrPaintBoxMouseDown;
	ScrPaintBoxV->OnMouseMove = ScrPaintBoxMouseMove;
	ScrPaintBoxV->OnMouseUp   = ScrPaintBoxMouseUp;
	ScrPaintBoxV->Tag		  = 0;

	//�����X�N���[��
	ScrPanelH	 = NULL;
	ScrPaintBoxH = NULL;
	org_ScrPanelHWndProc = NULL;
	if (Flag & USCRPNL_FLAG_HS) {
		ScrPanelH = new TPanel(ParentPanel);
		ScrPanelH->Parent		  = ParentPanel;
		ScrPanelH->DoubleBuffered = true;
		ScrPanelH->BevelOuter	  = bvNone;
		ScrPanelH->Height		  = KnobWidth;
		ScrPanelH->Width		  = ParentPanel->ClientWidth;
		ScrPanelH->Left 		  = 0;
		ScrPanelH->Top			  = ParentPanel->ClientHeight - ScrPanelH->Height;
		TAnchors anchor;
		anchor << akLeft << akRight << akBottom;
		ScrPanelH->Anchors	  = anchor;

		org_ScrPanelHWndProc  = ScrPanelH->WindowProc;
		ScrPanelH->WindowProc = ScrPanelHWndProc;

		ScrPaintBoxH = new TPaintBox(ParentPanel);
		ScrPaintBoxH->Parent	  = ScrPanelH;
		ScrPaintBoxH->Align 	  = alClient;
		ScrPaintBoxH->OnPaint	  = ScrPaintBoxPaint;
		ScrPaintBoxH->OnMouseDown = ScrPaintBoxMouseDown;
		ScrPaintBoxH->OnMouseMove = ScrPaintBoxMouseMove;
		ScrPaintBoxH->OnMouseUp   = ScrPaintBoxMouseUp;
		ScrPaintBoxH->Tag		  = 1;
	}

	ScrKnobRectV = Rect(0, 0, 0, 0);
	ScrKnobMaxY  = 0;
	ScrCatchYp	 = 0;

	ScrKnobRectH = Rect(0, 0, 0, 0);
	ScrKnobMaxX	 = 0;
	ScrCatchXp	 = 0;

	ScrCatchKnob = false;

	ScrPage 	 = 0;

	Color		 = clAppWorkSpace;
	KnobColor	 = clBtnFace;
	KnobBdrColor = clBtnShadow;
	HitLineColor = clYellow;
	HitLineAlpha = 128;

	Visible 	 = false;
	VisibleV	 = VisibleH = false;

	org_ParentPanelWndProc = NULL;
	if (Flag & USCRPNL_FLAG_P_WP) {
		org_ParentPanelWndProc	= ParentPanel->WindowProc;
		ParentPanel->WindowProc = ParentPanelWndProc;
	}

	org_AssoListWndProc = NULL;
	if (AssoListBox && (Flag & USCRPNL_FLAG_L_WP)) {
		org_AssoListWndProc 	= AssoListBox->WindowProc;
		AssoListBox->WindowProc = AssoListWndProc;
	}

	org_AssoChkListWndProc = NULL;
	if (AssoChkListBox && (Flag & USCRPNL_FLAG_L_WP)) {
		org_AssoChkListWndProc	   = AssoChkListBox->WindowProc;
		AssoChkListBox->WindowProc = AssoChkListWndProc;
	}

	org_AssoGridWndProc = NULL;
	if (AssoStrGrid && (Flag & USCRPNL_FLAG_G_WP)) {
		org_AssoGridWndProc 	= AssoStrGrid->WindowProc;
		AssoStrGrid->WindowProc = AssoGridWndProc;
	}
}

//---------------------------------------------------------------------------
//�O���b�h�̗񕝂̍��v
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::get_GridTotalWidth()
{
	if (!AssoStrGrid) return 1;

	int wd = 0;
	for (int i=0; i<AssoStrGrid->ColCount; i++) wd += AssoStrGrid->ColWidths[i];
	return wd;
}
//---------------------------------------------------------------------------
//�����X�N���[���ʒu����O���b�h�̐擪���ݒ�
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::set_GridLefCol(int pos)
{
	if (!AssoStrGrid) return;

	if (pos>=ScrKnobMaxX) {
		AssoStrGrid->LeftCol = AssoStrGrid->ColCount - 1;
	}
	else {
		int scr_p = (int)(1.0 * pos / (ParentPanel->ClientWidth - KnobWidth) * get_GridTotalWidth());
		for (int i=0,w=0; i<AssoStrGrid->ColCount; i++,w+=AssoStrGrid->ColWidths[i]) {
			if (scr_p<=w) { AssoStrGrid->LeftCol = i; break; }
		}
	}
}

//---------------------------------------------------------------------------
//�O���b�h�̍s���̍��v
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::get_GridTotalHeight()
{
	if (!AssoStrGrid) return 1;

	int hi = 0;
	for (int i=0; i<AssoStrGrid->RowCount; i++) hi += AssoStrGrid->RowHeights[i];
	return hi;
}
//---------------------------------------------------------------------------
//�����X�N���[���ʒu����O���b�h�̐擪�s��ݒ�
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::set_GridTopRow(int pos)
{
	if (!AssoStrGrid) return;

	int scr_p = (int)(1.0 * pos / AssoStrGrid->ClientHeight * get_GridTotalHeight());
	for (int i=0,h=0; i<AssoStrGrid->RowCount; i++,h+=AssoStrGrid->RowHeights[i]) {
		if (scr_p<=h) { AssoStrGrid->TopRow = i; break; }
	}
}

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̐����X�N���[���ʒu���擾���m�u�ʒu�ɕϊ�
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::GetKnobPosH()
{
	int xp = 0;
	if (AssoListBox && AssoListBox->ScrollWidth>0) {
		SCROLLINFO si;
		::ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask  = SIF_TRACKPOS;
		if (::GetScrollInfo(AssoListBox->Handle, SB_HORZ, &si))
			xp = (int)(1.0 * si.nTrackPos / AssoListBox->ScrollWidth * (ParentPanel->ClientWidth - KnobWidth));
	}
	else if (AssoStrGrid && AssoStrGrid->ColCount>0) {
		int x0 = 0;
		for (int i=0; i<AssoStrGrid->LeftCol; i++) x0 += AssoStrGrid->ColWidths[i];
		xp = (int)(1.0 * x0 / get_GridTotalWidth() * (ParentPanel->ClientWidth - KnobWidth));
	}
	return xp;
}

//---------------------------------------------------------------------------
//�\���A�m�u�T�C�Y���̍X�V
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::UpdateKnob()
{
	if (Visible) {
		ScrKnobRectV = Rect(0, 0, 0, 0);
		ScrKnobRectH = Rect(0, 0, 0, 0);
		ScrPanelV->Width	= KnobWidth;
		ScrPaintBoxV->Width = ScrPanelV->ClientWidth;

		if (ScrPanelH && ScrPaintBoxH) {
			ScrPanelH->Width	 = ParentPanel->ClientWidth;
			ScrPanelH->Height	 = KnobWidth;
			ScrPanelH->Top		 = ParentPanel->ClientHeight - ScrPanelH->Height;
			ScrPaintBoxH->Height = ScrPanelH->ClientHeight;
		}

		//���X�g�{�b�N�X
		if (AssoListBox) {
			TListBox *lp = AssoListBox;
			//�����X�N���[���o�[
			if (lp->ItemHeight>0 && lp->Count>0) {
				int lst_hi = lp->ClientHeight;
				ScrPage = lst_hi/lp->ItemHeight;
				if (lp->Count>ScrPage) {
					double r = 1.0 * ScrPage / lp->Count;
					ScrKnobRectV = Rect(0, 0, ScrPaintBoxV->Width, std::max((int)(lst_hi * r), KnobWidth));
					ScrKnobMaxY  = lst_hi - ScrKnobRectV.Height() - 1;
					int yp = std::min((int)(1.0 * lp->TopIndex / lp->Count * lst_hi), ScrKnobMaxY);
					ScrKnobRectV.Location = Point(0, yp);
					ScrPanelV->Visible = true;
					ScrPaintBoxV->Invalidate();
				}
				else ScrPanelV->Visible = false;
			}
			else ScrPanelV->Visible = false;

			//�����X�N���[���o�[
			if (ScrPanelH && ScrPaintBoxH) {
				if (lp->ScrollWidth>lp->ClientWidth) {
					double r = 1.0 * lp->ClientWidth / lp->ScrollWidth;
					int hscr_wd  = ParentPanel->ClientWidth - KnobWidth;
					ScrKnobRectH = Rect(0, 0, std::max((int)(hscr_wd * r), KnobWidth - 2), ScrPaintBoxH->Height);
					ScrKnobMaxX  = hscr_wd - ScrKnobRectH.Width() - 1;
					ScrKnobRectH.Location = Point(GetKnobPosH(), 0);
					ScrPanelH->Visible = true;
					ScrPaintBoxH->Invalidate();
				}
				else ScrPanelH->Visible = false;

				//���X�g�{�b�N�X�̍�������
				lp->Height = ParentPanel->ClientHeight
								+ (ScrPanelH->Visible? (::GetSystemMetrics(SM_CYHSCROLL) - ScrPanelH->Height + 1) : 0);
			}
		}
		//�`�F�b�N���X�g�{�b�N�X
		else if (AssoChkListBox) {
			TCheckListBox *lp = AssoChkListBox;
			//�����X�N���[���o�[
			if (lp->ItemHeight>0 && lp->Count>0) {
				int lst_hi = lp->ClientHeight;
				ScrPage = lst_hi/lp->ItemHeight;
				if (lp->Count>ScrPage) {
					double r = 1.0 * ScrPage / lp->Count;
					ScrKnobRectV = Rect(0, 0, ScrPaintBoxV->Width, std::max((int)(lst_hi * r), KnobWidth));
					ScrKnobMaxY  = lst_hi - ScrKnobRectV.Height() - 1;
					int yp = std::min((int)(1.0 * lp->TopIndex / lp->Count * lst_hi), ScrKnobMaxY);
					ScrKnobRectV.Location = Point(0, yp);
					ScrPanelV->Visible = true;
					ScrPaintBoxV->Invalidate();
				}
				else ScrPanelV->Visible = false;
			}
			else ScrPanelV->Visible = false;
		}
		//�O���b�h
		else if (AssoStrGrid) {
			TStringGrid *gp = AssoStrGrid;
			//�����X�N���[���o�[
			if (gp->RowCount>0) {
				int grid_hi = gp->ClientHeight;
				ScrPage = gp->VisibleRowCount;
				if (gp->RowCount > ScrPage) {
					double r = 1.0 * grid_hi / get_GridTotalHeight();
					ScrKnobRectV = Rect(0, 0, ScrPaintBoxV->Width, std::max((int)(grid_hi * r), KnobWidth));
					ScrKnobMaxY  = grid_hi - ScrKnobRectV.Height() - 1;
					int yp = std::min((int)(1.0 * gp->TopRow / gp->RowCount * grid_hi), ScrKnobMaxY);
					ScrKnobRectV.Location = Point(0, yp);
					ScrPanelV->Visible = true;
					ScrPaintBoxV->Invalidate();
				}
				else ScrPanelV->Visible = false;
			}
			else ScrPanelV->Visible = false;

			//�����X�N���[���o�[
			if (ScrPanelH && ScrPaintBoxH) {
				if (gp->ColCount>gp->VisibleColCount) {
					double r = 1.0 * gp->ClientWidth / get_GridTotalWidth();
					int hscr_wd  = ParentPanel->ClientWidth - KnobWidth;
					ScrKnobRectH = Rect(0, 0, std::max((int)(hscr_wd * r), KnobWidth - 2), ScrPaintBoxH->Height);
					ScrKnobMaxX  = hscr_wd - ScrKnobRectH.Width() - 1;
					ScrKnobRectH.Location = Point(GetKnobPosH(), 0);
					ScrPanelH->Visible = true;
					ScrPaintBoxH->Invalidate();
				}
				else ScrPanelH->Visible = false;

				//�O���b�h�̍�������
				gp->Height = ParentPanel->ClientHeight
								+ (ScrPanelH->Visible? (::GetSystemMetrics(SM_CYHSCROLL) - ScrPanelH->Height + 1) : 0);
			}
		}
		//�X�N���[���o�[
		else if (AssoScrollBar) {
			TScrollBar *bp = AssoScrollBar;
			//�����X�N���[���o�[
			if (bp->Max>0) {
				ScrPage = bp->LargeChange;
				if (bp->Max>ScrPage) {
					int scr_hi = ScrPanelV->ClientHeight;
					double r = 1.0 * ScrPage / (bp->Max - bp->Min);
					ScrKnobRectV = Rect(0, 0, ScrPaintBoxV->Width, std::max((int)(scr_hi * r), KnobWidth));
					ScrKnobMaxY  = scr_hi - ScrKnobRectV.Height() - 1;
					int yp = std::min((int)(1.0 * (bp->Position - bp->Min) / (bp->Max - bp->Min) * scr_hi), ScrKnobMaxY);
					ScrKnobRectV.Location = Point(0, yp);
					ScrPanelV->Visible = true;
					ScrPaintBoxV->Invalidate();
				}
				else ScrPanelV->Visible = false;
			}
			else ScrPanelV->Visible = false;
		}
		else ScrPanelV->Visible = false;
	}
	else ScrPanelV->Visible = false;

	VisibleV = ScrPanelV->Visible;
	VisibleH = (ScrPanelH && ScrPanelH->Visible);
}

//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ParentPanelWndProc(TMessage &msg)
{
	//�w�i�̂�����h�~
	if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }

	org_ParentPanelWndProc(msg);

	if (msg.Msg==WM_SIZE) UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::AssoListWndProc(TMessage &msg)
{
	//�m�u�\���𓯊�
	if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();

	org_AssoListWndProc(msg);
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::AssoChkListWndProc(TMessage &msg)
{
	//�m�u�\���𓯊�
	if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();

	org_AssoChkListWndProc(msg);
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::AssoGridWndProc(TMessage &msg)
{
	//�m�u�\���𓯊�
	if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();

	org_AssoGridWndProc(msg);
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPanelVWndProc(TMessage &msg)
{
	//�w�i�̂�����h�~
	if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }

	org_ScrPanelVWndProc(msg);

	if (org_ParentPanelWndProc==NULL && msg.Msg==WM_SIZE) UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPanelHWndProc(TMessage &msg)
{
	//�w�i�̂�����h�~
	if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }

	org_ScrPanelHWndProc(msg);

	if (org_ParentPanelWndProc==NULL && msg.Msg==WM_SIZE) UpdateKnob();
}

//---------------------------------------------------------------------------
//�`��
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxPaint(TObject *Sender)
{
	if (ScrPanelV->Visible || (ScrPanelH && ScrPanelH->Visible)) {
		TPaintBox *pp = (TPaintBox *)Sender;
		TCanvas *cv = pp->Canvas;

		//�w�i
		cv->Brush->Color = Color;
		cv->FillRect(pp->ClientRect);

		//�܂�
		TRect rc = (pp->Tag==1)? ScrKnobRectH : ScrKnobRectV;
		Graphics::TBitmap *bp = (pp->Tag==1)? KnobImgBuffH : KnobImgBuffV;
		if (bp && !bp->Empty) {
			InflateRect(rc, -1, -1);
			if (pp->Tag==1) rc.Right += 1; else rc.Bottom += 1;
			cv->StretchDraw(rc, bp);
		}
		else if (KnobBdrColor==clNone) {
			InflateRect(rc, -1, -1);
			if (pp->Tag==1) rc.Right += 1; else rc.Bottom += 1;
			cv->Brush->Style = bsSolid;
			cv->Brush->Color = KnobColor;
			cv->FillRect(rc);
		}
		else {
			if (pp->Tag==1) {
				InflateRect(rc, 0, -2);  rc.Left += 1;
			}
			else {
				InflateRect(rc, -2, 0);  rc.Top += 1;
			}
			cv->Brush->Style = bsSolid;
			cv->Brush->Color = KnobColor;
			cv->FillRect(rc);
			//�֊s
			cv->Pen->Style = psSolid;
			cv->Pen->Width = 1;
			cv->Pen->Color = KnobBdrColor;
			cv->MoveTo(rc.Left,		rc.Top - 1);	cv->LineTo(rc.Right, 	rc.Top - 1);
			cv->MoveTo(rc.Left - 1, rc.Top);		cv->LineTo(rc.Left - 1,	rc.Bottom);
			cv->MoveTo(rc.Left, 	rc.Bottom);		cv->LineTo(rc.Right,	rc.Bottom);
			cv->MoveTo(rc.Right, 	rc.Top);		cv->LineTo(rc.Right,	rc.Bottom);
		}

		//�q�b�g�s�\��
		if (pp->Tag==0 && HitLineColor!=Graphics::clNone && HitLines->Count>1) {
			int max_n = (int)HitLines->Objects[0];	//���s��
			if (max_n>0) {
				BLENDFUNCTION blend_f;
				blend_f.BlendOp             = AC_SRC_OVER;
				blend_f.BlendFlags          = 0;
				blend_f.SourceConstantAlpha = HitLineAlpha;
				blend_f.AlphaFormat         = 0;

				std::unique_ptr<Graphics::TBitmap> bp_l(new Graphics::TBitmap());
				int w = pp->ClientWidth;
				bp_l->SetSize(w, 1);
				bp_l->Canvas->Brush->Style = bsSolid;
				bp_l->Canvas->Brush->Color = HitLineColor;
				bp_l->Canvas->FillRect(TRect(0, 0, w, 1));

				int lst_y = -1;
				for (int i=1; i<HitLines->Count; i++) {
					double r = 1.0 * (int)HitLines->Objects[i]/max_n;
					int y = (int)(pp->ClientHeight * r);
					if (y>lst_y && y<pp->ClientHeight) {
						::AlphaBlend(cv->Handle, 0, y, w, 1,
							bp_l->Canvas->Handle, 0, 0, w, 1, blend_f);
						lst_y = y;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::Repaint()
{
	//�����X�N���[���o�[
	if (ScrPanelV->Visible) {
		//���X�g�{�b�N�X
		if (AssoListBox && AssoListBox->Count>0) {
			if (ScrKnobRectV.Height()>0) {
				int yp = std::min((int)(1.0 * AssoListBox->TopIndex / AssoListBox->Count * AssoListBox->ClientHeight), ScrKnobMaxY);
				ScrKnobRectV.Location = Point(0, yp);
			}
		}
		//�`�F�b�N���X�g�{�b�N�X
		else if (AssoChkListBox && AssoChkListBox->Count>0) {
			if (ScrKnobRectV.Height()>0) {
				int yp = std::min((int)(1.0 * AssoChkListBox->TopIndex / AssoChkListBox->Count * AssoChkListBox->ClientHeight), ScrKnobMaxY);
				ScrKnobRectV.Location = Point(0, yp);
			}
		}
		//�O���b�h
		else if (AssoStrGrid && AssoStrGrid->RowCount>0) {
			if (ScrKnobRectV.Height()>0) {
				int yp = std::min((int)(1.0 * AssoStrGrid->TopRow / AssoStrGrid->RowCount * AssoStrGrid->ClientHeight), ScrKnobMaxY);
				ScrKnobRectV.Location = Point(0, yp);
			}
		}
		//�X�N���[���o�[
		else if (AssoScrollBar && AssoScrollBar->Max>0) {
			double r = 1.0 * (AssoScrollBar->Position - AssoScrollBar->Min) / AssoScrollBar->Max;
			int yp = std::min((int)(r * ScrPanelV->ClientHeight), ScrKnobMaxY);
			ScrKnobRectV.Location = Point(0, yp);
		}
		ScrPaintBoxV->Repaint();
	}

	//�����X�N���[���o�[
	if (ScrPanelH && ScrPaintBoxH && ScrPanelH->Visible && ScrKnobRectH.Width()>0) {
		if ((AssoListBox && AssoListBox->Count>0) || (AssoStrGrid && AssoStrGrid->RowCount>0)) {
			ScrKnobRectH.Location = Point(GetKnobPosH(), 0);
			ScrPaintBoxH->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
//�}�E�X����
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft) {
		TPaintBox *pp = (TPaintBox *)Sender;
		TRect rc = (pp->Tag==1)? ScrKnobRectH : ScrKnobRectV;
		ScrCatchKnob = rc.PtInRect(Point(X, Y));
		//�m�u�ړ��J�n
		if (ScrCatchKnob) {
			if (pp->Tag==0)
				ScrCatchYp = Y - rc.Top;
			else
				ScrCatchXp = X - rc.Left;
		}
		//�O��y�[�W
		else {
			//���X�g�{�b�N�X
			if (AssoListBox) {
				TListBox *lp = AssoListBox;
				//�����X�N���[���o�[
				if (pp->Tag==0) {
					if (Y<rc.Top)
						lp->TopIndex = std::max(lp->TopIndex - ScrPage, 0);
					else if (Y>rc.Bottom)
						lp->TopIndex = lp->TopIndex + ScrPage;
				}
				//�����X�N���[���o�[
				else {
					if (X<rc.Left) {
						lp->Perform(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT,  0), (NativeInt)0);
						lp->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
					else if (X>rc.Right) {
						lp->Perform(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (NativeInt)0);
						lp->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
				}
			}
			//�`�F�b�N���X�g�{�b�N�X
			else if (AssoChkListBox) {
				TCheckListBox *lp = AssoChkListBox;
				//�����X�N���[���o�[
				if (pp->Tag==0) {
					if (Y<rc.Top)
						lp->TopIndex = std::max(lp->TopIndex - ScrPage, 0);
					else if (Y>rc.Bottom)
						lp->TopIndex = lp->TopIndex + ScrPage;
				}
			}
			//�O���b�h
			else if (AssoStrGrid) {
				TStringGrid *gp = AssoStrGrid;
				//�����X�N���[���o�[
				if (pp->Tag==0) {
					if (Y<rc.Top)
						gp->TopRow = std::max(gp->TopRow - ScrPage, 0);
					else if (Y>rc.Bottom)
						gp->TopRow = gp->TopRow + ScrPage;
				}
				//�����X�N���[���o�[
				else {
					if (X<rc.Left) {
						gp->Perform(WM_HSCROLL, MAKEWPARAM(SB_PAGELEFT,  0), (NativeInt)0);
						gp->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
					else if (X>rc.Right) {
						gp->Perform(WM_HSCROLL, MAKEWPARAM(SB_PAGERIGHT, 0), (NativeInt)0);
						gp->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
				}
			}
			//�X�N���[���o�[
			else if (AssoScrollBar) {
				if (Y<rc.Top)
					AssoScrollBar->Position = std::max(AssoScrollBar->Position - ScrPage, 0);
				else if (Y>rc.Bottom)
					AssoScrollBar->Position = AssoScrollBar->Position + ScrPage;
			}
			Repaint();
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if (ScrCatchKnob) {
		TPaintBox *pp = (TPaintBox *)Sender;
		//�����X�N���[���o�[
		if (pp->Tag==0) {
			int yp = std::min(std::max(Y - ScrCatchYp, 0), ScrKnobMaxY);
			ScrKnobRectV.Location = Point(0, yp);
			pp->Repaint();

			//���X�g�{�b�N�X
			if (AssoListBox) {
				if (yp==ScrKnobMaxY)
					AssoListBox->TopIndex = AssoListBox->Count - 1;
				else
					AssoListBox->TopIndex = 1.0 * yp / AssoListBox->ClientHeight * AssoListBox->Count;

				if (Flag & (USCRPNL_FLAG_FL|USCRPNL_FLAG_GL)) AssoListBox->Perform(WM_NYANFI_USCROLL, 0, (NativeInt)0);

				//�J�[�\������ɉ��̈��
				if (ListCsrVisible) {
					if (AssoListBox->ItemIndex < AssoListBox->TopIndex)
						AssoListBox->ItemIndex = AssoListBox->TopIndex;
					else {
						int idx_btm = AssoListBox->TopIndex + AssoListBox->ClientHeight/AssoListBox->ItemHeight - 1;
						if (AssoListBox->ItemIndex > idx_btm) AssoListBox->ItemIndex = idx_btm;
					}
				}
			}
			//�`�F�b�N���X�g�{�b�N�X
			else if (AssoChkListBox) {
				if (yp==ScrKnobMaxY)
					AssoChkListBox->TopIndex = AssoChkListBox->Count - 1;
				else
					AssoChkListBox->TopIndex = 1.0 * yp / AssoChkListBox->ClientHeight * AssoChkListBox->Count;

				if (Flag & (USCRPNL_FLAG_FL|USCRPNL_FLAG_GL)) AssoChkListBox->Perform(WM_NYANFI_USCROLL, 0, (NativeInt)0);

				//�J�[�\������ɉ��̈��
				if (ListCsrVisible) {
					if (AssoChkListBox->ItemIndex < AssoChkListBox->TopIndex)
						AssoChkListBox->ItemIndex = AssoChkListBox->TopIndex;
					else {
						int idx_btm = AssoChkListBox->TopIndex + AssoChkListBox->ClientHeight/AssoChkListBox->ItemHeight - 1;
						if (AssoChkListBox->ItemIndex > idx_btm) AssoChkListBox->ItemIndex = idx_btm;
					}
				}
			}
			//�O���b�h
			else if (AssoStrGrid) {
				set_GridTopRow(yp);
			}
			//�X�N���[���o�[
			else if (AssoScrollBar) {
				if (yp==ScrKnobMaxY)
					AssoScrollBar->Position = std::max(AssoScrollBar->Max - ScrPage, AssoScrollBar->Min);
				else
					AssoScrollBar->Position = (1.0 * yp / ScrPanelV->ClientHeight * AssoScrollBar->Max) + AssoScrollBar->Min;
			}
		}
		//�����X�N���[���o�[
		else {
			if (ScrPaintBoxH && (AssoListBox || AssoStrGrid) && ScrKnobMaxX>0) {
				int xp = std::min(std::max(X - ScrCatchXp, 0), ScrKnobMaxX);
				ScrKnobRectH.Location = Point(xp, 0);
				ScrPaintBoxH->Repaint();

				int hscr_wd  = ParentPanel->ClientWidth - KnobWidth;
				if (hscr_wd>0) {
					//���X�g�{�b�N�X
					if (AssoListBox) {
						int scr_p = (int)(1.0 * xp / hscr_wd * AssoListBox->ScrollWidth);
						AssoListBox->Perform(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, scr_p), (NativeInt)0);
						AssoListBox->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
					//�O���b�h
					else if (AssoStrGrid && AssoStrGrid->ColCount>0) {
						set_GridLefCol(xp);
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TPaintBox *pp = (TPaintBox *)Sender;
	ScrCatchKnob = false;

	//�E�N���b�N�C�x���g
	if (Button==mbRight && OnRButtonUp && pp->Tag==0) OnRButtonUp(Sender, Button, Shift, X, Y);
}

//---------------------------------------------------------------------------
//�R�[�i�[�p�l���̐ݒ�
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::SetRBCornerPanel(TPanel *pp)
{
	pp->Left = ParentPanel->ClientWidth  - pp->Width  - (VisibleV? KnobWidth : 0);
	pp->Top  = ParentPanel->ClientHeight - pp->Height - (VisibleH? KnobWidth : 0);
	pp->BringToFront();
}

//---------------------------------------------------------------------------
//�����ꂪ�ω�������?
//  �ω������ꍇ HitLines ��������
//---------------------------------------------------------------------------
bool __fastcall UsrScrollPanel::KeyWordChanged(
	UnicodeString kwd,	//������
	int  max_cnt,		//�ő�s��
	bool case_sw,		//�召���������	(default = false)
	int  code_page)		//�R�[�h�y�[�W		(default = 0)
{
	if (HitLineColor==Graphics::clNone) {
		HitLines->Clear();
		return false;
	}

	UnicodeString lst_kwd = (HitLines->Count>0)? HitLines->Strings[0] : EmptyStr;
	int           lst_cnt = (HitLines->Count>0)? (int)HitLines->Objects[0] : 0;
	if (!kwd.IsEmpty()) kwd.cat_sprintf(_T("\t%u:%u"), (case_sw? 1 : 0), code_page);

	bool ret = false;
	if (kwd.IsEmpty() || max_cnt==0 || !SameStr(kwd, lst_kwd) || max_cnt!=lst_cnt) {
		HitLines->Clear();
		if (!kwd.IsEmpty() && max_cnt>0) {
			HitLines->AddObject(kwd, (TObject *)(NativeInt)max_cnt);
			ret = true;
		}
	}
	return ret;
}
//---------------------------------------------------------------------------
//�q�b�g�s��ǉ�
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::AddHitLine(int n)
{
	HitLines->AddObject(EmptyStr, (TObject *)(NativeInt)n);
}
//---------------------------------------------------------------------------
