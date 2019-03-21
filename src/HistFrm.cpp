//----------------------------------------------------------------------//
// NyanFi																//
//  ヒストグラム表示													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "Global.h"
#include "MainFrm.h"
#include "LoupeFrm.h"
#include "HistFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
THistForm *HistForm;
//---------------------------------------------------------------------------
__fastcall THistForm::THistForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall THistForm::FormShow(TObject *Sender)
{
	if (Floating) IniFile->LoadPosInfo(this);

	ClientWidth  = 256 + H_LR_MARGIN*2 + 2;
	ClientHeight = CLHI_HISTOGRAM;

	DragKind = NyanFiForm->ImgSidePanel->Visible? dkDock : dkDrag;
	DragMode = NyanFiForm->ImgSidePanel->Visible? dmAutomatic : dmManual;

	HistImage->Picture->Bitmap->SetSize(ClientWidth, ClientHeight);
	HistPanel->Color = col_bgImage;

	if (!UnInitializing) DrawHistogram();
}
//---------------------------------------------------------------------------
void __fastcall THistForm::FormHide(TObject *Sender)
{
	if (Floating)
		IniFile->SavePosInfo(this);
	else {
		TPanel *pp = NyanFiForm->HistDockPanel;
		pp->Constraints->MinHeight = 1;
		pp->Constraints->MaxHeight = 1;
		NyanFiForm->SideDockPanel->Height	 = NyanFiForm->SideDockPanel->Height - CLHI_HISTOGRAM;
		NyanFiForm->SideDockSplitter->Height = (LoupeForm->Visible && !LoupeForm->Floating)? SplitterWidth2 : 0;
		ShowHistogram = false;
	}
}
//---------------------------------------------------------------------------
void __fastcall THistForm::WmSysCommand(TWMSysCommand &SysCom)
{
	//[×]ボタンが押された
	if (SysCom.CmdType==SC_CLOSE) {
		Close();
		ShowHistogram = false;
	}
	else TForm::Dispatch(&SysCom);
}

//---------------------------------------------------------------------------
//ヒストグラムの描画
//---------------------------------------------------------------------------
void __fastcall THistForm::DrawHistogram(Graphics::TBitmap *i_bmp)
{
	TCanvas *cv = HistImage->Picture->Bitmap->Canvas;
	int h_hi = 128;
	cv->Brush->Color = col_bgImage;
	cv->FillRect(ClientRect);
	cv->Brush->Color = clBlack;
	cv->FillRect(Rect(H_LR_MARGIN, H_TOP_MARGIN, 256 + H_LR_MARGIN, h_hi + H_TOP_MARGIN));

	if (!i_bmp || i_bmp->Empty || isViewIcon) return;

	std::unique_ptr<Graphics::TBitmap> h_bmp(new Graphics::TBitmap());
	h_bmp->Assign(i_bmp);
	h_bmp->PixelFormat = pf24bit;

	for (int i=0; i<256; i++) R_buf[i] = G_buf[i] = B_buf[i] = 0;

	for (int y=0; y<h_bmp->Height; y++) {
		TRGBTriple *pYX = reinterpret_cast<TRGBTriple *>(h_bmp->ScanLine[y]);
		for (int x=0; x<h_bmp->Width; x++,pYX++) {
			R_buf[pYX->rgbtRed]++;
			G_buf[pYX->rgbtGreen]++;
			B_buf[pYX->rgbtBlue]++;
		}
	}

	int v_max = 0;
	for (int i=0; i<256; i++) {
		if (R_buf[i]>v_max) v_max = R_buf[i];
		if (G_buf[i]>v_max) v_max = G_buf[i];
		if (B_buf[i]>v_max) v_max = B_buf[i];
	}

	cv->Pen->Width = 1;
	cv->Pen->Mode  = pmMerge;
	int y0 = h_hi + H_TOP_MARGIN;
	//R
	cv->Pen->Color = TColor(RGB(255, 0, 0));
	int x = H_LR_MARGIN;
	for (int i=0; i<256; i++,x++) {
		cv->MoveTo(x, y0);	cv->LineTo(x, y0 - (1.0 * h_hi * R_buf[i]/v_max));
	}
	//G
	cv->Pen->Color = TColor(RGB(0, 255, 0));
	x = H_LR_MARGIN;
	for (int i=0; i<256; i++,x++) {
		cv->MoveTo(x, y0);	cv->LineTo(x, y0 - (1.0 * h_hi * G_buf[i]/v_max));
	}
	//B
	cv->Pen->Color = TColor(RGB(0, 0, 255));
	x = H_LR_MARGIN;
	for (int i=0; i<256; i++,x++) {
		cv->MoveTo(x, y0);	cv->LineTo(x, y0 - (1.0 * h_hi * B_buf[i]/v_max));
	}

	//グリッド
	cv->Pen->Mode  = pmCopy;
	cv->Pen->Color = clDkGray;
	cv->Pen->Width = 1;
	cv->Pen->Style = psSolid;
	x = H_LR_MARGIN + 64;
	for (int i=1; i<4; i++,x+=64) {
		cv->MoveTo(x, H_TOP_MARGIN);
		cv->LineTo( x, y0);
	}
	int y = H_TOP_MARGIN + 64;
	for (int i=1; i<3; i++,y+=64) {
		cv->MoveTo(H_LR_MARGIN, y);
		cv->LineTo(H_LR_MARGIN + 255, y);
	}

	//境界線
	if (Floating && col_TlBorder!=col_None) {
		cv->Pen->Width = TlWinBorderWidth;
		cv->Pen->Color = col_TlBorder;
		cv->MoveTo(0, 0);
		cv->LineTo(0, ClientHeight -1);
		cv->LineTo(ClientWidth-1 , ClientHeight-1);
		cv->LineTo(ClientWidth-1, -1);
	}
}
//---------------------------------------------------------------------------

