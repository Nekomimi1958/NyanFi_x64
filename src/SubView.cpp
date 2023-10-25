//----------------------------------------------------------------------//
// NyanFi																//
//  サブビュアー														//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "usr_id3.h"
#include "Global.h"
#include "UserMdl.h"
#include "FileInfDlg.h"
#include "SubView.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSubViewer *SubViewer;

//---------------------------------------------------------------------------
__fastcall TSubViewer::TSubViewer(TComponent* Owner)
	: TForm(Owner)
{
	formFileInf = isClip = false;
	ImgLocked	= false;
}
//---------------------------------------------------------------------------
void __fastcall TSubViewer::FormCreate(TObject *Sender)
{
	hasBitmap = false;
}
//---------------------------------------------------------------------------
void __fastcall TSubViewer::FormShow(TObject *Sender)
{
	//ファイル情報ダイアログと重ならないように初期化
	if (formFileInf && !IniFile->KeyExists(SCT_General, "SubViewerFILeft")) {
		Top  = FileInfoDlg->Top;
		Left = FileInfoDlg->Left + FileInfoDlg->Width;
		IniFile->SavePosInfo(this, "SubViewerFI");
	}

	IniFile->LoadPosInfo(this, false,
		(ScrMode==SCMD_TVIEW)? "SubViewerTV" : (ScrMode==SCMD_IVIEW)? "SubViewerIV" :
		formFileInf? "SubViewerFI" : "SubViewer");

	ImgPanel->Color = IniFile->ReadColGen(_T("SubViewerBgCol"),	col_bgImage);
	Color = ImgPanel->Color;

	SetToolWinBorder(this);
}
//---------------------------------------------------------------------------
void __fastcall TSubViewer::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this,
		(ScrMode==SCMD_TVIEW)? "SubViewerTV" : (ScrMode==SCMD_IVIEW)? "SubViewerIV" :
		formFileInf? "SubViewerFI" : "SubViewer");

	IniFile->WriteIntGen(_T("SubViewerBgCol"),	(int)ImgPanel->Color);

	Image1->Picture->Assign(NULL);
	FileName	= EmptyStr;
	formFileInf = isClip = false;
	ImgLocked	= false;
	hasBitmap	= false;
}

//---------------------------------------------------------------------------
void __fastcall TSubViewer::FormResize(TObject *Sender)
{
	if (!TitleInf.IsEmpty() && Image1->Picture->Width>0 && Image1->Picture->Height>0) {
		ZoomInf.sprintf(_T("  (%4.1f%%)"), GetZoomRatio());
		Caption = TitleInf + ZoomInf;
	}
}

//---------------------------------------------------------------------------
//ズーム倍率(%)を取得
//---------------------------------------------------------------------------
double __fastcall TSubViewer::GetZoomRatio()
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
void __fastcall TSubViewer::DrawImage(UnicodeString fnam)
{
	try {
		hasBitmap = false;
		UnicodeString ex_str;
		if (isClip) {
			if (!Clipboard()->HasFormat(CF_BITMAP)) Abort();
			Image1->Picture->Assign(Clipboard());
			Image1->Transparent = false;
			hasBitmap = true;
			FileName  = EmptyStr;
		}
		else {
			if (ImgLocked) return;
			if (!file_exists(fnam)) Abort();

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
			else {
				std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
				if (test_Mp3Ext(fext)) {
					if (!ID3_GetImage(fnam, bmp.get())) Abort();
				}
				else if (test_FlacExt(fext)) {
					if (!get_FlacImage(fnam, bmp.get())) Abort();
				}
				else if (test_FileExt(fext, get_img_fext())) {
					int res = load_ImageFile(fnam, bmp.get(), WICIMG_PREVIEW, ImgPanel->Color);
					if (res==0) Abort();

					if (test_ExifExt(fext)) {
						int ori = 0;
						ex_str	= get_ExifInfStr(fnam, &ori);
						//回転
						int rot = (ori==6)? 1 : (ori==3)? 2 : (ori==8)? 3 : 0;
						if (RotViewImg && rot>0 && res!=LOADED_BY_WIC) WIC_rotate_image(bmp.get(), rot);
					}
					else if (test_PngExt(fext))
						ex_str = get_PngInfStr(fnam);
					else if (SameText(fext, ".webp"))
						ex_str = get_WebpInfStr(ViewFileName);

				}
				else if (test_FileExt(fext, FEXT_INDIVICO)) {
					bmp->SetSize(256, 256);
					TCanvas *cv = bmp->Canvas;
					cv->Brush->Color = ImgPanel->Color;
					cv->FillRect(Rect(0, 0, bmp->Width, bmp->Height));

					int size_lst[6] = {256, 128, 64, 48, 32, 16};
					for (int i=0; i<6; i++) {
						int size = size_lst[i];
						HICON hIcon = usr_SH->get_Icon(fnam, size);
						if (hIcon && size==size_lst[i]) {
							::DrawIconEx(cv->Handle,
								(bmp->Width - size)/2, (bmp->Height - size)/2,
								hIcon, size, size, 0, NULL, DI_NORMAL);
							::DestroyIcon(hIcon);
							break;
						}
					}
				}
				else Abort();

				Image1->Picture->Assign(bmp.get());
				Image1->Transparent = false;
				hasBitmap = true;
			}
			FileName = fnam;
		}

		//サイズ
		if (ex_str.IsEmpty() || ex_str.Pos(" × ")==0) {
			if (!ex_str.IsEmpty()) ex_str += "  ";
			ex_str += get_wd_x_hi_str(Image1->Picture->Width, Image1->Picture->Height);
		}

		if (isClip)
			TitleInf.sprintf(_T("クリップボード  %s"), ex_str.c_str());
		else
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
void __fastcall TSubViewer::RotateImage(int rn)
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
void __fastcall TSubViewer::LockImage()
{
	if (Visible) ImgLocked = !ImgLocked;
}
//---------------------------------------------------------------------------
void __fastcall TSubViewer::SetImgLocked(bool Value)
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
void __fastcall TSubViewer::Image1Click(TObject *Sender)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TSubViewer::FormClick(TObject *Sender)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//ポップアップメニュー
//---------------------------------------------------------------------------
void __fastcall TSubViewer::PopupMenu1Popup(TObject *Sender)
{
	LockItem->Checked = ImgLocked;
}
//---------------------------------------------------------------------------
//背景色を設定
//---------------------------------------------------------------------------
void __fastcall TSubViewer::BgColItemClick(TObject *Sender)
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
void __fastcall TSubViewer::LockItemClick(TObject *Sender)
{
	ImgLocked = !ImgLocked;
	Repaint();
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//90度回転
//---------------------------------------------------------------------------
void __fastcall TSubViewer::RotateActionExecute(TObject *Sender)
{
	RotateImage(((TAction *)Sender)->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TSubViewer::RotateActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = hasBitmap;
}
//---------------------------------------------------------------------------

