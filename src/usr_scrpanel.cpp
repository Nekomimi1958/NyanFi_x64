//----------------------------------------------------------------------//
// シンプルスクロールバー・パネル										//
//																		//
//----------------------------------------------------------------------//
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//リストボックスに関連付け
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TListBox *lp, int flag)
{
	ParentPanel	   = pp;
	AssoListBox	   = lp;
	AssoChkListBox = NULL;
	AssoStrGrid	   = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//チェックリストボックスに関連付け
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TCheckListBox *lp, int flag)
{
	ParentPanel	   = pp;
	AssoChkListBox = lp;
	AssoListBox	   = NULL;
	AssoStrGrid	   = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//グリッドに関連付け
//---------------------------------------------------------------------------
UsrScrollPanel::UsrScrollPanel(TPanel *pp, TStringGrid *gp, int flag)
{
	ParentPanel	   = pp;
	AssoStrGrid	   = gp;
	AssoListBox	   = NULL;
	AssoChkListBox = NULL;
	AssoScrollBar  = NULL;
	Flag		   = flag;

	InitializePanel();
}
//---------------------------------------------------------------------------
//垂直スクロールバーに関連付け
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
	delete SelLines;
}

//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::InitializePanel()
{
	HitLines = new TStringList();
	SelLines = new TStringList();

	KnobImgBuffV = NULL;
	KnobImgBuffH = NULL;

	OnRButtonUp  = NULL;

	KnobWidth = get_SysMetricsForPPI(SM_CXVSCROLL, ParentPanel->CurrentPPI);

	ListCsrVisible = false;

	//垂直スクロールバー
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
	ScrPaintBoxV->Tag		  = USCRPNL_SCRTYPE_V;

	//水平スクロール
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
		ScrPaintBoxH->Tag		  = USCRPNL_SCRTYPE_H;
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
	KnobActColor = Graphics::clNone;
	HitLineColor = clYellow;
	PosLineAlpha = 128;			//***
	SelLineColor = Graphics::clNone;

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
//グリッドの列幅の合計
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::get_GridTotalWidth()
{
	if (!AssoStrGrid) return 1;

	int wd = 0;
	for (int i=0; i<AssoStrGrid->ColCount; i++) wd += AssoStrGrid->ColWidths[i];
	return wd;
}
//---------------------------------------------------------------------------
//水平スクロール位置からグリッドの先頭列を設定
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
//グリッドの行高の合計
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::get_GridTotalHeight()
{
	if (!AssoStrGrid) return 1;

	int hi = 0;
	for (int i=0; i<AssoStrGrid->RowCount; i++) hi += AssoStrGrid->RowHeights[i];
	return hi;
}
//---------------------------------------------------------------------------
//垂直スクロール位置からグリッドの先頭行を設定
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
//リストボックスの水平スクロール位置を取得しノブ位置に変換
//---------------------------------------------------------------------------
int __fastcall UsrScrollPanel::GetKnobPosH()
{
	int xp = 0;
	if (AssoListBox && AssoListBox->ScrollWidth>0) {
		SCROLLINFO si = {sizeof(SCROLLINFO), SIF_TRACKPOS};
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
//表示、ノブサイズ等の更新
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::UpdateKnob()
{
	if (Visible) {
		if (LastPPI!=ParentPanel->CurrentPPI) KnobWidth = ScaledInt(UsKnobWidth, ParentPanel);

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
		int s_hi = get_SysMetricsForPPI(SM_CYHSCROLL, ParentPanel->CurrentPPI);	//水平スクロールバーの幅を取得

		//リストボックス
		if (AssoListBox) {
			TListBox *lp = AssoListBox;
			//垂直スクロールバー
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
				else {
					ScrPanelV->Visible = false;
				}
			}
			else {
				ScrPanelV->Visible = false;
			}

			//水平スクロールバー
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
				else {
					ScrPanelH->Visible = false;
				}

				//リストボックスの高さ調整
				lp->Height = ParentPanel->ClientHeight + (ScrPanelH->Visible? (s_hi - ScrPanelH->Height + 1) : 0);
			}
		}
		//チェックリストボックス
		else if (AssoChkListBox) {
			TCheckListBox *lp = AssoChkListBox;
			//垂直スクロールバー
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
				else {
					ScrPanelV->Visible = false;
				}
			}
			else {
				ScrPanelV->Visible = false;
			}
		}
		//グリッド
		else if (AssoStrGrid) {
			TStringGrid *gp = AssoStrGrid;
			//垂直スクロールバー
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
				else {
					ScrPanelV->Visible = false;
				}
			}
			else {
				ScrPanelV->Visible = false;
			}

			//水平スクロールバー
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
				else {
					ScrPanelH->Visible = false;
				}

				//グリッドの高さ調整
				gp->Height = ParentPanel->ClientHeight + (ScrPanelH->Visible? (s_hi - ScrPanelH->Height + 1) : 0);
			}
		}
		//スクロールバー
		else if (AssoScrollBar) {
			TScrollBar *bp = AssoScrollBar;
			//垂直スクロールバー
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
				else {
					ScrPanelV->Visible = false;
				}
			}
			else {
				ScrPanelV->Visible = false;
			}
		}
		else {
			ScrPanelV->Visible = false;
		}
	}
	else {
		ScrPanelV->Visible = false;
	}

	VisibleV = ScrPanelV->Visible;
	VisibleH = (ScrPanelH && ScrPanelH->Visible);
}

//---------------------------------------------------------------------------
//描画
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxPaint(TObject *Sender)
{
	if (ScrPanelV->Visible || (ScrPanelH && ScrPanelH->Visible)) {
		TPaintBox *pp = (TPaintBox *)Sender;
		TCanvas *cv = pp->Canvas;

		//背景
		cv->Brush->Color = Color;
		cv->FillRect(pp->ClientRect);

		//つまみ
		TRect rc = (pp->Tag==USCRPNL_SCRTYPE_H)? ScrKnobRectH : ScrKnobRectV;
		Graphics::TBitmap *bp = (pp->Tag==USCRPNL_SCRTYPE_H)? KnobImgBuffH : KnobImgBuffV;
		if (bp && !bp->Empty) {
			InflateRect(rc, -1, -1);
			if (pp->Tag==USCRPNL_SCRTYPE_H) rc.Right += 1; else rc.Bottom += 1;
			cv->StretchDraw(rc, bp);
		}
		else {
			cv->Brush->Style = bsSolid;
			cv->Brush->Color = (ScrCatchKnob && KnobActColor!=Graphics::clNone)? KnobActColor : KnobColor;
			if (KnobBdrColor==Graphics::clNone) {
				InflateRect(rc, -1, -1);
				if (pp->Tag==USCRPNL_SCRTYPE_H) rc.Right += 1; else rc.Bottom += 1;
				cv->FillRect(rc);
			}
			else {
				if (pp->Tag==USCRPNL_SCRTYPE_H) {
					InflateRect(rc, 0, -2);  rc.Left += 1;
				}
				else {
					InflateRect(rc, -2, 0);  rc.Top += 1;
				}
				cv->FillRect(rc);

				//輪郭
				cv->Pen->Style = psSolid;
				cv->Pen->Width = 1;
				cv->Pen->Color = KnobBdrColor;
				cv->MoveTo(rc.Left,		rc.Top - 1);	cv->LineTo(rc.Right, 	rc.Top - 1);
				cv->MoveTo(rc.Left - 1, rc.Top);		cv->LineTo(rc.Left - 1,	rc.Bottom);
				cv->MoveTo(rc.Left, 	rc.Bottom);		cv->LineTo(rc.Right,	rc.Bottom);
				cv->MoveTo(rc.Right, 	rc.Top);		cv->LineTo(rc.Right,	rc.Bottom);
			}
		}

		//位置表示
		bool has_hit = (HitLineColor!=Graphics::clNone && HitLines->Count>1);
		bool has_sel = (SelLineColor!=Graphics::clNone && SelLines->Count>1);
		if (pp->Tag==USCRPNL_SCRTYPE_V && (has_hit || has_sel)) {
			BLENDFUNCTION blend_f;
			blend_f.BlendOp 			= AC_SRC_OVER;
			blend_f.BlendFlags			= 0;
			blend_f.AlphaFormat 		= 0;
			blend_f.SourceConstantAlpha = PosLineAlpha;

			std::unique_ptr<Graphics::TBitmap> bp_l(new Graphics::TBitmap());
			int w = pp->ClientWidth;
			bp_l->SetSize(w, 1);
			bp_l->Canvas->Brush->Style = bsSolid;

			//ヒット位置
			if (has_hit) {
				int max_n = (int)HitLines->Objects[0];	//総行数
				if (max_n>0) {
					bp_l->Canvas->Brush->Color = HitLineColor;
					bp_l->Canvas->FillRect(TRect(0, 0, w, 1));

					int lst_y = -1;
					for (int i=1; i<HitLines->Count; i++) {
						double r = 1.0 * (int)HitLines->Objects[i]/max_n;
						int y = (int)(pp->ClientHeight * r);
						if (y>lst_y && y<pp->ClientHeight) {
							::AlphaBlend(cv->Handle, 0, y, w, 1, bp_l->Canvas->Handle, 0, 0, w, 1, blend_f);
							lst_y = y;
						}
					}
				}
			}

			//選択位置
			if (has_sel) {
				int max_n = (int)SelLines->Objects[0];	//総行数
				if (max_n>0) {
					bp_l->Canvas->Brush->Color = SelLineColor;
					bp_l->Canvas->FillRect(TRect(0, 0, w, 1));

					int lst_y = -1;
					for (int i=1; i<SelLines->Count; i++) {
						double r = 1.0 * (int)SelLines->Objects[i]/max_n;
						int y = (int)(pp->ClientHeight * r);
						if (y>lst_y && y<pp->ClientHeight) {
							::AlphaBlend(cv->Handle, 0, y, w, 1, bp_l->Canvas->Handle, 0, 0, w, 1, blend_f);
							lst_y = y;
						}
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::Repaint()
{
	//垂直スクロールバー
	if (ScrPanelV->Visible && ScrKnobRectV.Height()>0) {
		int yp = -1;
		//リストボックス
		if (AssoListBox && AssoListBox->Count>0) {
			yp = (int)(1.0 * AssoListBox->TopIndex / AssoListBox->Count * AssoListBox->ClientHeight);
		}
		//チェックリストボックス
		else if (AssoChkListBox && AssoChkListBox->Count>0) {
			yp = (int)(1.0 * AssoChkListBox->TopIndex / AssoChkListBox->Count * AssoChkListBox->ClientHeight);
		}
		//グリッド
		else if (AssoStrGrid && AssoStrGrid->RowCount>0) {
			yp = (int)(1.0 * AssoStrGrid->TopRow / AssoStrGrid->RowCount * AssoStrGrid->ClientHeight);
		}
		//スクロールバー
		else if (AssoScrollBar && AssoScrollBar->Max>0) {
			double r = 1.0 * (AssoScrollBar->Position - AssoScrollBar->Min) / AssoScrollBar->Max;
			yp = (int)(r * ScrPanelV->ClientHeight);
		}
	
		if (yp>=0) {
			ScrKnobRectV.Location = Point(0, std::min(yp, ScrKnobMaxY));
			ScrPaintBoxV->Repaint();
		}
	}

	//水平スクロールバー
	if (ScrPanelH && ScrPaintBoxH && ScrPanelH->Visible && ScrKnobRectH.Width()>0) {
		if ((AssoListBox && AssoListBox->Count>0) || (AssoStrGrid && AssoStrGrid->RowCount>0)) {
			ScrKnobRectH.Location = Point(GetKnobPosH(), 0);
			ScrPaintBoxH->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft) {
		TPaintBox *pp = (TPaintBox *)Sender;
		TRect rc = (pp->Tag==USCRPNL_SCRTYPE_H)? ScrKnobRectH : ScrKnobRectV;
		ScrCatchKnob = rc.PtInRect(Point(X, Y));
		//ノブ移動開始
		if (ScrCatchKnob) {
			if (pp->Tag==USCRPNL_SCRTYPE_V)
				ScrCatchYp = Y - rc.Top;
			else
				ScrCatchXp = X - rc.Left;
		}
		//前後ページ
		else {
			//リストボックス
			if (AssoListBox) {
				TListBox *lp = AssoListBox;
				//垂直スクロールバー
				if (pp->Tag==USCRPNL_SCRTYPE_V) {
					if (Y<rc.Top)
						lp->TopIndex = std::max(lp->TopIndex - ScrPage, 0);
					else if (Y>rc.Bottom)
						lp->TopIndex = lp->TopIndex + ScrPage;
				}
				//水平スクロールバー
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
			//チェックリストボックス
			else if (AssoChkListBox) {
				TCheckListBox *lp = AssoChkListBox;
				//垂直スクロールバー
				if (pp->Tag==USCRPNL_SCRTYPE_V) {
					if (Y<rc.Top)
						lp->TopIndex = std::max(lp->TopIndex - ScrPage, 0);
					else if (Y>rc.Bottom)
						lp->TopIndex = lp->TopIndex + ScrPage;
				}
			}
			//グリッド
			else if (AssoStrGrid) {
				TStringGrid *gp = AssoStrGrid;
				//垂直スクロールバー
				if (pp->Tag==USCRPNL_SCRTYPE_V) {
					if (Y<rc.Top)
						gp->TopRow = std::max(gp->TopRow - ScrPage, 0);
					else if (Y>rc.Bottom)
						gp->TopRow = gp->TopRow + ScrPage;
				}
				//水平スクロールバー
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
			//スクロールバー
			else if (AssoScrollBar) {
				if (Y<rc.Top)
					AssoScrollBar->Position = std::max(AssoScrollBar->Position - ScrPage, 0);
				else if (Y>rc.Bottom)
					AssoScrollBar->Position = AssoScrollBar->Position + ScrPage;
			}
		}

		Repaint();
	}
}
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::ScrPaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	if (ScrCatchKnob) {
		TPaintBox *pp = (TPaintBox *)Sender;
		//垂直スクロールバー
		if (pp->Tag==USCRPNL_SCRTYPE_V) {
			int yp = std::min(std::max(Y - ScrCatchYp, 0), ScrKnobMaxY);
			ScrKnobRectV.Location = Point(0, yp);
			pp->Repaint();

			//リストボックス
			if (AssoListBox) {
				if (yp==ScrKnobMaxY)
					AssoListBox->TopIndex = AssoListBox->Count - 1;
				else
					AssoListBox->TopIndex = 1.0 * yp / AssoListBox->ClientHeight * AssoListBox->Count;

				if (Flag & (USCRPNL_FLAG_FL|USCRPNL_FLAG_GL)) AssoListBox->Perform(WM_NYANFI_USCROLL, 0, (NativeInt)0);

				//カーソルを常に可視領域に
				if (ListCsrVisible) {
					if (AssoListBox->ItemIndex < AssoListBox->TopIndex)
						AssoListBox->ItemIndex = AssoListBox->TopIndex;
					else {
						int idx_btm = AssoListBox->TopIndex + AssoListBox->ClientHeight/AssoListBox->ItemHeight - 1;
						if (AssoListBox->ItemIndex > idx_btm) AssoListBox->ItemIndex = idx_btm;
					}
				}
			}
			//チェックリストボックス
			else if (AssoChkListBox) {
				if (yp==ScrKnobMaxY)
					AssoChkListBox->TopIndex = AssoChkListBox->Count - 1;
				else
					AssoChkListBox->TopIndex = 1.0 * yp / AssoChkListBox->ClientHeight * AssoChkListBox->Count;

				if (Flag & (USCRPNL_FLAG_FL|USCRPNL_FLAG_GL)) AssoChkListBox->Perform(WM_NYANFI_USCROLL, 0, (NativeInt)0);

				//カーソルを常に可視領域に
				if (ListCsrVisible) {
					if (AssoChkListBox->ItemIndex < AssoChkListBox->TopIndex) {
						AssoChkListBox->ItemIndex = AssoChkListBox->TopIndex;
					}
					else {
						int idx_btm = AssoChkListBox->TopIndex + AssoChkListBox->ClientHeight/AssoChkListBox->ItemHeight - 1;
						if (AssoChkListBox->ItemIndex > idx_btm) AssoChkListBox->ItemIndex = idx_btm;
					}
				}
			}
			//グリッド
			else if (AssoStrGrid) {
				set_GridTopRow(yp);
			}
			//スクロールバー
			else if (AssoScrollBar) {
				if (yp==ScrKnobMaxY)
					AssoScrollBar->Position = std::max(AssoScrollBar->Max - ScrPage, AssoScrollBar->Min);
				else
					AssoScrollBar->Position = (1.0 * yp / ScrPanelV->ClientHeight * AssoScrollBar->Max) + AssoScrollBar->Min;
			}
		}
		//水平スクロールバー
		else {
			if (ScrPaintBoxH && (AssoListBox || AssoStrGrid) && ScrKnobMaxX>0) {
				int xp = std::min(std::max(X - ScrCatchXp, 0), ScrKnobMaxX);
				ScrKnobRectH.Location = Point(xp, 0);
				ScrPaintBoxH->Repaint();

				int hscr_wd  = ParentPanel->ClientWidth - KnobWidth;
				if (hscr_wd>0) {
					//リストボックス
					if (AssoListBox) {
						int scr_p = (int)(1.0 * xp / hscr_wd * AssoListBox->ScrollWidth);
						AssoListBox->Perform(WM_HSCROLL, MAKEWPARAM(SB_THUMBPOSITION, scr_p), (NativeInt)0);
						AssoListBox->Perform(WM_HSCROLL, MAKEWPARAM(SB_ENDSCROLL, 0), (NativeInt)0);
					}
					//グリッド
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
	Repaint();

	//右クリックイベント
	if (Button==mbRight && OnRButtonUp && pp->Tag==USCRPNL_SCRTYPE_V) OnRButtonUp(Sender, Button, Shift, X, Y);
}

//---------------------------------------------------------------------------
//コーナーパネルの設定
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::SetRBCornerPanel(TPanel *pp)
{
	pp->Left = ParentPanel->ClientWidth  - pp->Width  - (VisibleV? KnobWidth : 0);
	pp->Top  = ParentPanel->ClientHeight - pp->Height - (VisibleH? KnobWidth : 0);
	pp->BringToFront();
}

//---------------------------------------------------------------------------
//検索語が変化したか?
//  変化した場合 HitLines を初期化
//---------------------------------------------------------------------------
bool __fastcall UsrScrollPanel::KeyWordChanged(
	UnicodeString kwd,	//検索語
	int  max_cnt,		//最大行数
	bool case_sw,		//大小文字を区別	(default = false)
	int  code_page)		//コードページ		(default = 0)
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
//ヒット行を追加
//---------------------------------------------------------------------------
void __fastcall UsrScrollPanel::AddHitLine(int n)
{
	HitLines->AddObject(EmptyStr, (TObject *)(NativeInt)n);
}
//---------------------------------------------------------------------------
