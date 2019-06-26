//----------------------------------------------------------------------//
// ビットマップビュー													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include "UserFunc.h"
#include "Global.h"
#include "TxtViewer.h"
#include "BitmapFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBitmapForm *BitmapForm;

//---------------------------------------------------------------------------
__fastcall TBitmapForm::TBitmapForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FormCreate(TObject *Sender)
{
	org_SttPanelWndProc  = SttPanel->WindowProc;
	SttPanel->WindowProc = SttPanelWndProc;

	//ビットマップビュー用パレットの初期化
	for (int i=0; i<256; i++) {
		BitPallet[i] = (i==0)				? clBlack   :
					   (i>0 && i<=0x1f) 	? clFuchsia :
					   (i>=0x20 && i<=0x7f) ? clYellow  : clBlue;
	}

	MapWidth = 128;
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	SetToolWinBorder(this);
	SetMapWidth(IniFile->ReadIntGen(_T("BitmapFormMapWidth"),	128));

	set_BtnMarkDark(TopButton, UBMK_VTOP);
	set_BtnMarkDark(PreButton, UBMK_VUP);
	set_BtnMarkDark(NxtButton, UBMK_VDOWN);
	set_BtnMarkDark(EndButton, UBMK_VEND);

	BorderPanel->Color = IsDarkMode? dcl_Window : clAppWorkSpace;
	MapScrBox->Color   = BorderPanel->Color;
	SetDarkWinTheme(MapScrBox);
	SetDarkWinTheme(FileMapPanel);

	SttPanel->Color = col_bgSttBar;
	SttPanel->Font->Assign(ViewerFont);
	SttPanel->Font->Color  = col_fgSttBar;
	SttPanel->ClientHeight = get_FontHeight(ViewerFont, 4);
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("BitmapFormMapWidth"),	MapWidth);
}
//---------------------------------------------------------------------------
//ファイルマップの描画
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FileMapBoxPaint(TObject *Sender)
{
	if (UnInitializing) return;

	TPaintBox *pp = (TPaintBox*)Sender;
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->SetSize(pp->ClientWidth, pp->ClientHeight);
	TCanvas *cv = bmp->Canvas;
	TRect rc = pp->ClientRect;
	cv->Brush->Color = col_bgPrgBar;
	cv->FillRect(rc);

	__int64 f_size = TxtViewer->BinFileSize;
	if (f_size>0) {
		int hi	  = pp->ClientHeight;
		rc.Top	  = hi * (1.0*TxtViewer->TopAddress/f_size);
		rc.Bottom = rc.Top + hi * (1.0*TxtViewer->BinarySize/f_size);
		cv->Brush->Color = clLime;
		cv->FillRect(rc);

		unsigned int p = MapScrBox->VertScrollBar->Position;
		__int64 adr0 = TxtViewer->TopAddress + p * MapWidth;
		__int64 adr1 = TxtViewer->TopAddress + (p + MapScrBox->ClientHeight) * MapWidth;
		rc.Top	  = hi * (1.0*adr0/f_size);
		rc.Bottom = hi * (1.0*adr1/f_size);
		cv->Brush->Color = clRed;
		cv->FrameRect(rc);
	}

	pp->Canvas->Draw(0, 0, bmp.get());
}

//---------------------------------------------------------------------------
//ビットマップの描画
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::BitmapBoxPaint(TObject *Sender)
{
	try {
		std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
		bmp->PixelFormat = pf24bit;
		bmp->SetSize(MapWidth, MapScrBox->ClientHeight);

		int top_y = MapScrBox->VertScrollBar->Position;
		unsigned int adr = top_y * MapWidth;
		for (int y = 0; y<bmp->Height; y++) {
			TRGBTriple *pYX = reinterpret_cast<TRGBTriple *>(bmp->ScanLine[y]);
			for (int i=0; i<MapWidth; i++,adr++,pYX++) {
				TColor col = (adr<TxtViewer->BinarySize) ? BitPallet[TxtViewer->MMF->Bytes[adr]]
														 : MapScrBox->Color;
				int c = ColorToRGB(col);
				pYX->rgbtRed   = GetRValue(c);
				pYX->rgbtGreen = GetGValue(c);
				pYX->rgbtBlue  = GetBValue(c);
			}
		}
		BitmapBox->Canvas->Draw(0, top_y, bmp.get());

		FileMapBox->Invalidate();
	}
	catch (...) {
		;
	}

	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//ビットマップ幅の変更
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::MapWidthActionExecute(TObject *Sender)
{
	SetMapWidth(((TAction*)Sender)->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::MapWidthActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Checked = (ap->Tag == MapWidth);
}

//---------------------------------------------------------------------------
//ビットマップ幅を設定
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetMapWidth(int w)
{
	if (w!=128 && w!=256) w = 128;
	MapWidth = w;

	Constraints->MinWidth
		= FileMapPanel->Width + MapWidth + (Width - ClientWidth) + ::GetSystemMetrics(SM_CXVSCROLL)
			+ (BorderShape1->Visible? TlWinBorderWidth * 2 : 0) + 12;

	Constraints->MaxWidth = Constraints->MinWidth;

	BitmapBox->ClientWidth  = MapWidth;
	BitmapBox->ClientHeight = TxtViewer->BinarySize/MapWidth + 1;
	MapScrBox->VertScrollBar->Position = 0;
}

//---------------------------------------------------------------------------
//先頭アドレスをファイルの先頭に
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetTopAdrActionExecute(TObject *Sender)
{
	TxtViewer->TextTop();
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetTopAdrActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (TxtViewer->TopAddress!=0);
}
//---------------------------------------------------------------------------
//先頭アドレスをファイルの先行部に
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetPreAdrActionExecute(TObject *Sender)
{
	__int64 adr = TxtViewer->TopAddress - ViewBinLimitSize;
	if (adr<0) adr = 0;
	TxtViewer->AssignBin(adr, true);
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetPreAdrActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (TxtViewer->TopAddress!=0);
}
//---------------------------------------------------------------------------
//先頭アドレスをファイルの後続部に
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetNextAdrActionExecute(TObject *Sender)
{
	TxtViewer->AssignBin(TxtViewer->TopAddress + TxtViewer->BinarySize, true);
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetNextAdrActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = TxtViewer->isContinue;
}

//---------------------------------------------------------------------------
//終端アドレスをファイルの最後に
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetEndAdrActionExecute(TObject *Sender)
{
	TxtViewer->TextEnd();
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::SetEndAdrActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = TxtViewer->isContinue;
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
__int64 __fastcall TBitmapForm::GetMapAddr(int X, int Y)
{
	__int64 adr = -1;
	__int64 f_size = TxtViewer->BinFileSize;
	if (f_size>0) {
		adr = f_size * (1.0*Y/FileMapBox->ClientHeight);
		adr -= adr%16;
		if (adr>=f_size) adr = -1;
	}
	return adr;
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FileMapBoxMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	if (Shift.Contains(ssLeft)) {
		__int64 adr = GetMapAddr(X, Y);
		if (adr!=-1) {
			TxtViewer->AssignBin(adr, true);
			TxtViewer->ToAddrR(0);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FileMapBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	__int64 adr = GetMapAddr(X, Y);
	SttPanel->Caption = (adr!=-1)? " " + get_AddrStr(adr) : EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TBitmapForm::BitmapBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	__int64 adr = TxtViewer->TopAddress + Y*MapWidth + X;
	if (adr>=TxtViewer->BinFileSize) adr = -1;
	SttPanel->Caption = (adr!=-1)? " " + get_AddrStr(adr) : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TBitmapForm::BitmapBoxMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	if (Shift.Contains(ssLeft)) {
		TxtViewer->ToAddrR(Y*MapWidth + X);
		Application->MainForm->SetFocus();
	}
}

//---------------------------------------------------------------------------
void __fastcall TBitmapForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	Application->MainForm->SetFocus();
}
//---------------------------------------------------------------------------

