//----------------------------------------------------------------------//
// NyanFi																//
//  サブビュアー(旧GIFビュアー)											//
//  アニメーションGIF対応、GIF以外も表示								//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <memory>
#include <Vcl.Imaging.GIFImg.hpp>
#include "usr_wic.h"
#include "Global.h"
#include "UserMdl.h"
#include "GifView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGifViewer *GifViewer;

//---------------------------------------------------------------------------
__fastcall TGifViewer::TGifViewer(TComponent* Owner)
	: TForm(Owner)
{
	ImgLocked = false;
}
//---------------------------------------------------------------------------
void __fastcall TGifViewer::FormCreate(TObject *Sender)
{
	org_ImgPanelWndProc  = ImgPanel->WindowProc;
	ImgPanel->WindowProc = ImgPanelWndProc;

	hasBitmap = false;
}
//---------------------------------------------------------------------------
void __fastcall TGifViewer::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);
	ImgPanel->Color = IniFile->ReadColGen(_T("GifViewBgCol"),	col_bgImage);
	Color = ImgPanel->Color;
}
//---------------------------------------------------------------------------
void __fastcall TGifViewer::FormHide(TObject *Sender)
{
	FileName  = EmptyStr;
	ImgLocked = false;
	hasBitmap = false;
	Image1->Picture->Assign(NULL);

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("GifViewBgCol"),	(int)ImgPanel->Color);
}

//---------------------------------------------------------------------------
void __fastcall TGifViewer::WmSysCommand(TWMSysCommand &SysCom)
{
	//[×]ボタンが押された
	if (SysCom.CmdType==SC_CLOSE) {
		Close();
		if (ScrMode==SCMD_IVIEW) ShowGifViewer = false;
	}
	else TForm::Dispatch(&SysCom);
}
//---------------------------------------------------------------------
void __fastcall TGifViewer::WmExitSizeMove(TMessage &msg)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TGifViewer::FormResize(TObject *Sender)
{
	if (!TitleInf.IsEmpty() && Image1->Picture->Width>0 && Image1->Picture->Height>0) {
		ZoomInf.sprintf(_T("  (%4.1f%%)"), GetZoomRatio());
		Caption = TitleInf + ZoomInf;
	}
}

//---------------------------------------------------------------------------
//ズーム倍率(%)を取得
//---------------------------------------------------------------------------
double __fastcall TGifViewer::GetZoomRatio()
{
	int p_w = Image1->Picture->Width;
	int p_h = Image1->Picture->Height;
	double r_w = (p_w>0 && Image1->Width<p_w)?  1.0 * Image1->Width/p_w  : 1.0;
	double r_h = (p_h>0 && Image1->Height<p_h)? 1.0 * Image1->Height/p_h : 1.0;
	return (std::min(r_w, r_h) * 100);
}

//---------------------------------------------------------------------------
//画像の描画
//---------------------------------------------------------------------------
void __fastcall TGifViewer::DrawImage(UnicodeString fnam)
{
	if (ImgLocked) return;

	try {
		hasBitmap = false;
		if (!file_exists(fnam)) Abort();

		UnicodeString ex_str;
		UnicodeString fext = ExtractFileExt(fnam);
		if (test_GifExt(fext)) {
			std::unique_ptr<TGIFImage> gif_buf(new TGIFImage());
			gif_buf->LoadFromFile(fnam);
			gif_buf->Animate	 = true;
			gif_buf->Transparent = true;
			Image1->Picture->Assign(gif_buf.get());
			Image1->Transparent = true;
		}
		else if (test_MetaExt(fext)) {
			std::unique_ptr<TMetafile> meta_buf(new TMetafile());
			meta_buf->LoadFromFile(fnam);
			Image1->Picture->Assign(meta_buf.get());
			Image1->Transparent = true;
		}
		else if (test_FileExt(fext, get_img_fext())) {
			std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
			int res = load_ImageFile(fnam, bmp.get(), WICIMG_PREVIEW, ImgPanel->Color);
			if (res==0) Abort();

			if (test_ExifExt(fext)) {
				int ori = 0;
				ex_str	= get_ExifInfStr(fnam, &ori);
				//回転
				int rot = (ori==6)? 1 : (ori==8)? 3 : 0;
				if (RotViewImg && rot>0 && res!=LOADED_BY_WIC) WIC_rotate_image(bmp.get(), rot);
			}
			else if (test_PngExt(fext))
				ex_str = get_PngInfStr(fnam);
			else if (USAME_TI(fext, ".webp"))
				ex_str = get_WebpInfStr(ViewFileName);

			Image1->Picture->Assign(bmp.get());
			Image1->Transparent = false;
			hasBitmap = true;
		}
		else Abort();

		FileName = fnam;

		//サイズ
		if (ex_str.IsEmpty() || ex_str.Pos(" × ")==0) {
			if (!ex_str.IsEmpty()) ex_str.UCAT_T("  ");
			ex_str += get_wd_x_hi_str(Image1->Picture->Width, Image1->Picture->Height);
		}
		TitleInf.sprintf(_T("%s  %s"), ExtractFileName(FileName).c_str(), ex_str.c_str());
		//表示倍率
		ZoomInf.sprintf(_T("  (%4.1f%%)"), GetZoomRatio());

		Caption = TitleInf + ZoomInf;
	}
	catch (...) {
		Image1->Picture->Assign(NULL);
		FileName = EmptyStr;
		TitleInf = ZoomInf = EmptyStr;
		Caption  = "none";
	}
}

//---------------------------------------------------------------------------
//回転
//---------------------------------------------------------------------------
void __fastcall TGifViewer::RotateImage(int rn)
{
	if (Visible && hasBitmap) {
		std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
		bmp->Assign(Image1->Picture->Bitmap);
		WIC_rotate_image(bmp.get(), rn);
		Image1->Picture->Bitmap->Assign(bmp.get());
	}
}

//---------------------------------------------------------------------------
//ロック
//---------------------------------------------------------------------------
void __fastcall TGifViewer::LockImage()
{
	if (Visible) ImgLocked = !ImgLocked;
}
//---------------------------------------------------------------------------
void __fastcall TGifViewer::SetImgLocked(bool Value)
{
	FImgLocked = Value;

	if (FImgLocked) {
		Color = clRed;
		BorderWidth = 1;
	}
	else {
		BorderWidth = 0;
		Color = ImgPanel->Color;
	}
}

//---------------------------------------------------------------------------
void __fastcall TGifViewer::Image1Click(TObject *Sender)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TGifViewer::FormClick(TObject *Sender)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//ポップアップメニュー
//---------------------------------------------------------------------------
void __fastcall TGifViewer::PopupMenu1Popup(TObject *Sender)
{
	LockItem->Checked = ImgLocked;
}
//---------------------------------------------------------------------------
//背景色を設定
//---------------------------------------------------------------------------
void __fastcall TGifViewer::BgColItemClick(TObject *Sender)
{
	UserModule->ColorDlg->Color = Color;
	if (UserModule->ColorDlg->Execute()) {
		ImgPanel->Color = UserModule->ColorDlg->Color;
		if (!ImgLocked) Color = ImgPanel->Color;

		DrawImage(FileName);
	}
	Application->MainForm->SetFocus();
}
//---------------------------------------------------------------------------
//ロック
//---------------------------------------------------------------------------
void __fastcall TGifViewer::LockItemClick(TObject *Sender)
{
	ImgLocked = !ImgLocked;
	Repaint();
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//90度回転
//---------------------------------------------------------------------------
void __fastcall TGifViewer::RotateActionExecute(TObject *Sender)
{
	RotateImage(((TAction *)Sender)->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TGifViewer::RotateActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = hasBitmap;
}
//---------------------------------------------------------------------------



