//----------------------------------------------------------------------//
// NyanFi																//
//  印刷設定															//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "usr_exif.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MainFrm.h"
#include "PrnImgDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPrintImgDlg *PrintImgDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TPrintImgDlg::TPrintImgDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::FormCreate(TObject *Sender)
{
	FileRec = NULL;
	PrvImage->Center = true;
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	DlgInitialized = false;

	StrFont = IniFile->ReadFontInf(_T("PrintImg"),	Font);

	init_UpDown(CpsUpDown,		Printer()->Copies);
	init_UpDown(ScaleUpDown,	IniFile->ReadIntGen(_T("PrintImgScale"),	100));
	init_UpDown(OfsXUpDown,		IniFile->ReadIntGen(_T("PrintImgOfsX")));
	init_UpDown(OfsYUpDown,		IniFile->ReadIntGen(_T("PrintImgOfsY")));
	init_UpDown(StrMgnUpDown,	IniFile->ReadIntGen(_T("PrintImgStrMgn"),	5));

	if (Printer()->Orientation==poLandscape)
		OriHBtn->Checked = true;
	else
		OriVBtn->Checked = true;

	OptRadioGroup->ItemIndex	= IniFile->ReadIntGen(_T("PrintImgSzOpt"));
	GrayCheckBox->Checked		= IniFile->ReadBoolGen(_T("PrintImgGray"));
	StrCheckBox->Checked		= IniFile->ReadBoolGen(_T("PrintImgStr"));
	StrFont->Color				= IniFile->ReadColGen(_T("PrintImgStrCol"),	clBlack);
	StrFmtEdit->Text			= IniFile->ReadStrGen(_T("PrintImgStrFmt"),	"\\XT(yy/mm/dd)");
	StrPosRadioGroup->ItemIndex = IniFile->ReadIntGen(_T("PrintImgStrPos"),	1);
	StrAlRadioGroup->ItemIndex	= IniFile->ReadIntGen(_T("PrintImgStrAl"),	2);

	SetDarkWinTheme(this);
	PrviewPanel->Color		 = clAppWorkSpace;
	SizeLabel->Color		 = clAppWorkSpace;
	SizeLabel->Font->Color	 = scl_WindowText;

	if (!UnInitializing) {
		DlgInitialized = true;
		UpdatePreview();
	}
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("PrintImgSzOpt"),	OptRadioGroup);
	IniFile->WriteIntGen(_T("PrintImgScale"),	ScaleUpDown->Position);
	IniFile->WriteIntGen(_T("PrintImgOfsX"),	OfsXUpDown->Position);
	IniFile->WriteIntGen(_T("PrintImgOfsY"),	OfsYUpDown->Position);
	IniFile->WriteBoolGen(_T("PrintImgGray"),	GrayCheckBox);
	IniFile->WriteBoolGen(_T("PrintImgStr"),	StrCheckBox);
	IniFile->WriteStrGen(_T("PrintImgStrFmt"),	StrFmtEdit);
	IniFile->WriteIntGen(_T("PrintImgStrPos"),	StrPosRadioGroup);
	IniFile->WriteIntGen(_T("PrintImgStrAl"),	StrAlRadioGroup);
	IniFile->WriteIntGen(_T("PrintImgStrMgn"),	StrMgnUpDown->Position);
	IniFile->WriteIntGen(_T("PrintImgStrCol"),	(int)StrFont->Color);
	IniFile->WriteFontInf("PrintImg", StrFont);
	delete StrFont;
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_IV) _T("#Print"));
}

//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::PageControl1DrawTab(TCustomTabControl *Control,
	int TabIndex, const TRect &Rect, bool Active)
{
	draw_OwnerTab(Control, TabIndex, Rect, Active, IsDarkMode);
}

//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::OptionChanged(TObject *Sender)
{
	Printer()->Orientation = OriHBtn->Checked? poLandscape : poPortrait;
	UpdatePreview();
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::OptionUpDownClick(TObject *Sender, TUDBtnType Button)
{
	UpdatePreview();
}

//---------------------------------------------------------------------------
//プリンターの設定
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::PrnPropBtnClick(TObject *Sender)
{
	PrinterSetupDialog1->Execute();
	UpdatePreview();
}

//---------------------------------------------------------------------------
//フォントの参照
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::RefFontBtnClick(TObject *Sender)
{
	UserModule->FontDlgToFont(StrFont);
	UpdatePreview();
}

//---------------------------------------------------------------------------
//描画
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::DrawImage(TCanvas *cv)
{
	if (!ImgViewThread) return;

	if (!cv) cv = PrvImage->Picture->Bitmap->Canvas;

	int p_wd = Printer()->PageWidth;
	int p_hi = Printer()->PageHeight;
	if (p_wd==0 || p_hi==0) return;

	int i_wd, i_hi;		//描画サイズ
	if (ImgViewThread->ImgIsWmf) {
		i_wd = ImgViewThread->MetaBuff->Width;
		i_hi = ImgViewThread->MetaBuff->Height;
	}
	else {
		i_wd = ImgViewThread->ImgBuff->Width;
		i_hi = ImgViewThread->ImgBuff->Height;
	}
	if (i_wd==0 || i_hi==0) return;

	int d_wd, d_hi;		//描画領域
	float r;

	bool is_prn = (cv==Printer()->Canvas);
	if (is_prn) {
		d_wd = p_wd;
		d_hi = p_hi;
	}
	else {
		//プレビュー領域を設定
		if (Printer()->Orientation==poLandscape) {
			r = 1.0 * p_hi / p_wd;
			d_wd = PrvImage->Width;
			d_hi = d_wd * r;
		}
		else {
			r = 1.0 * p_wd / p_hi;
			d_hi = PrvImage->Height;
			d_wd = d_hi* r;
		}
		PrvImage->Picture->Bitmap->SetSize(d_wd, d_hi);
		TRect rc = Rect(0, 0, d_wd, d_hi);
		cv->Brush->Color = clWhite;
		cv->FillRect(rc);
	}

	float i_r = 1.0 * i_wd / i_hi;
	float d_r = 1.0 * d_wd / d_hi;

	switch (OptRadioGroup->ItemIndex) {
	case 0:	//ページに会わせる
		if (i_r>d_r) {
			r = 1.0 * i_hi / i_wd;
			i_wd = d_wd;
			i_hi = i_wd * r;
		}
		else {
			r = 1.0 * i_wd / i_hi;
			i_hi = d_hi;
			i_wd = i_hi * r;
		}
		break;
	case 1:	//ページのサイズでトリミング
		if (i_r>d_r) {
			r = 1.0 * i_wd / i_hi;
			i_hi = d_hi;
			i_wd = i_hi * r;
		}
		else {
			r = 1.0 * i_hi / i_wd;
			i_wd = d_wd;
			i_hi = i_wd * r;
		}
		break;
	case 2:	case 3:	//ページ中央/左上
		if (i_r>d_r) {
			r = 1.0 * i_hi / i_wd;
			i_wd = d_wd;
			i_hi = i_wd * r;
		}
		else {
			r = 1.0 * i_wd / i_hi;
			i_hi = d_hi;
			i_wd = i_hi * r;
		}
		r = EditToInt(ScaleEdit, 100) / 100.0;
		i_hi = (int)(i_hi * r + 0.5);
		i_wd = (int)(i_wd * r + 0.5);
		break;
	}

	std::unique_ptr<Graphics::TBitmap> r_bp(new Graphics::TBitmap());
	//メタファイル
	if (ImgViewThread->ImgIsWmf) {
		r_bp->SetSize(i_wd, i_hi);
		TRect rc = Rect(0, 0, i_wd, i_hi);
		if (!is_prn) {
			r_bp->Canvas->Brush->Color = clWhite;
			r_bp->Canvas->FillRect(rc);
		}
		r_bp->Canvas->StretchDraw(rc, ImgViewThread->MetaBuff);
	}
	//ビットマップ
	else {
		WIC_resize_image(ImgViewThread->ImgBuff, r_bp.get(), 0.0, i_wd, i_hi, WicScaleOpt);
	}

	//グレースケール
	if (GrayCheckBox->Checked) WIC_grayscale_image(r_bp.get());

	//オフセット
	int x, y;
	if (OptRadioGroup->ItemIndex==3) {
		x = d_wd * EditToInt(OfsXEdit) / 100.0;
		y = d_hi * EditToInt(OfsYEdit) / 100.0;
	}
	else {
		x = (d_wd - i_wd)/2;
		y = (d_hi - i_hi)/2;
	}
	cv->Draw(x, y, r_bp.get());

	//文字入れ
	if (StrCheckBox->Checked) {
		TDateTime xt;
		UnicodeString fnam = FileRec? (FileRec->is_virtual? FileRec->tmp_name : FileRec->f_name) : EmptyStr;
		if (FileExists(fnam)) {
			if (!EXIF_GetExifTime(fnam, xt)) xt = get_file_age(fnam);
		}
		else {
			xt = Now();
		}

		//書式文字列の展開
		UnicodeString s = StrFmtEdit->Text;
		UnicodeString lbuf;
		UnicodeString bnam = get_base_name(fnam);
		UnicodeString fext = get_extension(fnam);	remove_top_s(fext, '.');
		s = ReplaceStr(s, "$F", fnam);
		s = ReplaceStr(s, "$B", ExtractFileName(fnam));
		int i = 1;
		while (i<=s.Length()) {
			WideChar c = s[i];
			if (c=='$') {
				UnicodeString fmt = get_tkn(s.SubString(i, 512), ')') + ")";
				if (StartsStr("$XT(", fmt)) {
					lbuf += FormatDateTime(get_in_paren(fmt), !fnam.IsEmpty()? xt : Now());
				}
				i += fmt.Length();
			}
			else {
				lbuf += c;
				i++;
			}
		}

		//描画
		int f_hi = StrFont->Size * 0.3527 * p_hi / ::GetDeviceCaps(Printer()->Handle, VERTSIZE);
		if (!is_prn) f_hi = std::max((int)(1.0 * f_hi * d_hi/p_hi + 0.5), 2);
		int mgn  = std::min(d_wd, d_hi) * EditToInt(StrMgnEdit) / 100.0;
		cv->Font->Assign(StrFont);
		cv->Font->Height = -f_hi;
		cv->Brush->Style = bsClear;
		int l_wd = cv->TextWidth(lbuf);
		int x = (StrAlRadioGroup->ItemIndex==0)? mgn :
				(StrAlRadioGroup->ItemIndex==1)? (d_wd - l_wd)/2 : d_wd - l_wd - mgn;
		int y = (StrPosRadioGroup->ItemIndex==0)? mgn : d_hi - mgn - f_hi;
		cv->TextOut(x, y, lbuf);
		FormatStr = lbuf;
	}
	else {
		FormatStr = EmptyStr;
	}

	UnicodeString tmp = minimize_str(FormatStr, Canvas, SizeLabel->Width - 4);
	SizeLabel->Hint = FormatStr;
	SizeLabel->Caption = tmp.cat_sprintf(_T("\r\n印刷領域: %u × %umm (%u × %upx)"),
		::GetDeviceCaps(Printer()->Handle, HORZSIZE), ::GetDeviceCaps(Printer()->Handle, VERTSIZE),
		::GetDeviceCaps(Printer()->Handle, HORZRES), ::GetDeviceCaps(Printer()->Handle, VERTRES));
}

//---------------------------------------------------------------------------
//プレビューの更新
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::UpdatePreview()
{
	if (!DlgInitialized) return;

	PrnNameLabel->Caption = Printer()->Printers->Strings[Printer()->PrinterIndex];

	ScaleEdit->Enabled = OptRadioGroup->ItemIndex==2 || OptRadioGroup->ItemIndex==3;
	OfsXEdit->Enabled  = OptRadioGroup->ItemIndex==3;
	OfsYEdit->Enabled  = OptRadioGroup->ItemIndex==3;

	DrawImage();

	StrFontEdit->Color		 = clSilver;
	StrFontEdit->Font->Color = StrFont->Color;
	StrFontEdit->Font->Style = StrFont->Style;
	UnicodeString tmp;
	StrFontEdit->Text = tmp.sprintf(_T("%s  %upt"), StrFont->Name.c_str(), StrFont->Size);
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::ClearPreview()
{
	TCanvas *cv = PrvImage->Picture->Bitmap->Canvas;
	cv->Brush->Color = clWhite;
	cv->FillRect(PrvImage->ClientRect);
	SizeLabel->Hint    = EmptyStr;
	SizeLabel->Caption = EmptyStr;
}

//---------------------------------------------------------------------------
//印刷を実行
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::PrintActionExecute(TObject *Sender)
{
	cursor_HourGlass();
	Printer()->Copies = EditToInt(PrnCpsEdit, 1);
	Printer()->BeginDoc();
	DrawImage(Printer()->Canvas);
	Printer()->EndDoc();
	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::PrintActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !Printer()->Printing && !isViewIcon &&  !isViewAGif;
}

//---------------------------------------------------------------------------
//前の画像
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::PrevBtn_Click(TObject *Sender)
{
	NyanFiForm->PrevFileAction->Execute();
	if (isViewIcon || isViewAGif) ClearPreview();
}
//---------------------------------------------------------------------------
//次の画像
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::NextBtnClick(TObject *Sender)
{
	NyanFiForm->NextFileAction->Execute();
	if (isViewIcon || isViewAGif) ClearPreview();
}
//---------------------------------------------------------------------------
//先頭の画像
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::TopBtnClick(TObject *Sender)
{
	NyanFiForm->TopFileAction->Execute();
	if (isViewIcon || isViewAGif) ClearPreview();
}
//---------------------------------------------------------------------------
//最後の画像
//---------------------------------------------------------------------------
void __fastcall TPrintImgDlg::EndBtnClick(TObject *Sender)
{
	NyanFiForm->EndFileAction->Execute();
	if (isViewIcon || isViewAGif) ClearPreview();
}
//---------------------------------------------------------------------------

