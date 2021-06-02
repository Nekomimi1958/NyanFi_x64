//----------------------------------------------------------------------//
// NyanFi																//
//  �J���[�s�b�J�[														//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "ColPicker.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TColorPicker *ColorPicker;

//---------------------------------------------------------------------------
__fastcall TColorPicker::TColorPicker(TComponent* Owner)
	: TForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TColorPicker::FormCreate(TObject *Sender)
{
	ViewImage = NULL;

	set_ComboBoxText(FmtComboBox,
		_T("R,G,B\n")
		_T("H,S,V(B)\n")
		_T("H,S,L\n")
		_T("COLORREF\n")
		_T("Delphi�`��\n")
		_T("VB�`��\n")
		_T("HTML�`��\n")
		_T("RGB(x,x,x)\n")
		_T("RGB(d,d,d)\n")
		_T("�萔�V���{��\n")
		_T("BITMAP(32)\n")
		_T("BITMAP(128)\n")
		_T("BITMAP(256)\n"));

	DefColCaption = "X___ Y___ (x___ y___)\nR___ G___ B___  #______\nH___ S___ V___ / H___ S___ L___";
}
//---------------------------------------------------------------------------
void __fastcall TColorPicker::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);

	FmtComboBox->ItemIndex	 = IniFile->ReadIntGen(_T("ColorPickerFmt"));
	Ave9pxCheckBox->Checked  = IniFile->ReadBoolGen(_T("ColorPickerAve9px"));
	CmpColCheckBox->Checked  = IniFile->ReadBoolGen(_T("ColorPickerCmpCol"));
	SafeColCheckBox->Checked = IniFile->ReadBoolGen(_T("ColorPickerSafeCol"));
	RepCntEdit->Text		 = IniFile->ReadStrGen(_T("ColorPickerRepCnt"), 4);
	RepComboBox->ItemIndex	 = IniFile->ReadIntGen(_T("ColorPickerRepMode"), 0);

	Repeating = false;
	RepCount  = 0;
	RepSttLabel->Caption = EmptyStr;

	ColLabel->Font->Size = 9;
	ColLabel->Caption	 = DefColCaption;

	ColImage->Picture->Bitmap->SetSize(ColImage->Width, ColImage->Width);
	TCanvas *cv = ColImage->Canvas;
	cv->Brush->Color = col_bgImage;
	cv->FillRect(ColImage->ClientRect);

	if (ViewImage) ViewImage->Cursor = UserModule->crSpuitTool;

	SetDarkWinTheme(this);
}
//---------------------------------------------------------------------------
void __fastcall TColorPicker::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("ColorPickerFmt"),		FmtComboBox);
	IniFile->WriteBoolGen(_T("ColorPickerAve9px"),	Ave9pxCheckBox);
	IniFile->WriteBoolGen(_T("ColorPickerCmpCol"),	CmpColCheckBox);
	IniFile->WriteBoolGen(_T("ColorPickerSafeCol"),	SafeColCheckBox);
	IniFile->WriteStrGen(_T("ColorPickerRepCnt"),	RepCntEdit);
	IniFile->WriteIntGen(_T("ColorPickerRepMode"),	RepComboBox->ItemIndex);

	if (ViewImage) ViewImage->Cursor = crDefault;
}

//---------------------------------------------------------------------------
//�\���̍X�V
//---------------------------------------------------------------------------
void __fastcall TColorPicker::UpdateStt(int x, int y, float ratio)
{
	ColLabel->Color		= get_PanelColor();
	ColLabel->Font->Color = IsDarkMode? dcl_BtnText : scl_BtnText;

	if (!ViewImage || isViewAGif) {
		ColLabel->Caption = EmptyStr;
		return;
	}

	Graphics::TBitmap *bmp = ViewImage->Picture->Bitmap;

	TRect rc = Rect(0, 0, bmp->Width, bmp->Height);
	if (ViewImage->Center)
		rc.Offset((ViewImage->ClientWidth - bmp->Width)/2, (ViewImage->ClientHeight - bmp->Height)/2);
	x -= rc.Left;
	y -= rc.Top;

	int c_size = ColImage->Width/3;
	TCanvas *cv = ColImage->Canvas;
	if (x<0 || x>=bmp->Width || y<0 || y>=bmp->Height) {
		ColLabel->Caption = DefColCaption;
		cv->Brush->Color  = col_bgImage;
		cv->FillRect(Rect(0, 0, c_size*3, c_size*3));
		Col2Panel->Color  = col_bgImage;
		ColEdit->Color	  = IsDarkMode? col_DkInval : col_Invalid;
		ColEdit->Text	  = EmptyStr;
	}
	else {
		TColor col[9];
		for (int i=0; i<3; i++) {
			int xp	= i*c_size;
			int x_i = x + i - 1;
			for (int j=0; j<3; j++) {
				int yp	= j*c_size;
				int y_j = y + j - 1;
				cv->Brush->Color = (x_i>=0 && x_i<bmp->Width && y_j>=0 && y_j<bmp->Height)?
										bmp->Canvas->Pixels[x_i][y_j] : col_bgImage;
				col[i*3 + j] = cv->Brush->Color;
				TRect rc = Rect(xp, yp, xp + c_size, yp + c_size);
				cv->FillRect(rc);
			}
		}

		//���F�l�̕\��
		TColor c = col[4];
		int cref = ColorToRGB(c);
		int r = GetRValue(cref);
		int g = GetGValue(cref);
		int b = GetBValue(cref);
		int h, s, v, l;
		UnicodeString msg;
		RgbToHsv(c, &h, &s, &v);
		msg.sprintf(_T("X%d Y%d (x%d y%d)\nR%d G%d B%d  #%02x%02x%02x\nH%d S%d V%d"), 
						(int)(x/ratio + 0.5), (int)(y/ratio + 0.5),  x, y,  r, g, b,  r, g, b,  h, s, v);
		RgbToHsl(c, &h, &s, &l);
		msg.cat_sprintf(_T(" / H%d S%d L%d"), h, s, l);
		ColLabel->Caption = msg;
		//�Ώۘg
		bool same_col = true;
		for (int i=1; i<9 && same_col; i++) same_col = (col[i]==col[i - 1]);
		cv->Brush->Color = RatioCol((h>=90 && h<270)? clRed : clAqua, (std::min(std::max(100 - l, 25), 75))/100.0);
		cv->FrameRect(same_col? Rect(0, 0, c_size*3, c_size*3) : Rect(c_size, c_size, c_size*2, c_size*2));

		//���ԏ���
		int r2 = r, g2 = g, b2 = b;
		//9�_����
		if (Ave9pxCheckBox->Checked) {
			int sum_r = 0, sum_g = 0, sum_b = 0;
			for (int i=0; i<9; i++) {
				int cref = ColorToRGB(col[i]);
				sum_r += GetRValue(cref);
				sum_g += GetGValue(cref);
				sum_b += GetBValue(cref);
			}
			r2 = sum_r/9;
			g2 = sum_g/9;
			b2 = sum_b/9;
		}
		//��F
		if (CmpColCheckBox->Checked) {
			int x = std::max(r, std::max(g2, b2)) + std::min(r2, std::min(g2, b2));
			r2 = x - r2;
			g2 = x - g2;
			b2 = x - b2;
		}
		//�Z�[�t�J���[�ߎ�
		if (SafeColCheckBox->Checked) {
			r2 = (int)(r2/51.0 + 0.5) * 51;
			g2 = (int)(g2/51.0 + 0.5) * 51;
			b2 = (int)(b2/51.0 + 0.5) * 51;
		}

		//�擾�F
		TColor c2 = TColor(RGB(r2, g2, b2));
		int cref2 = ColorToRGB(c2);
		int h2, s2, v2;  RgbToHsv(c2, &h2, &s2, &v2);
		int h3, s3, l2;  RgbToHsl(c2, &h3, &s3, &l2);
		Col2Panel->Color = c2;

		//�R�s�[������
		UnicodeString colstr;
		bool is_bmp = false;

		switch (FmtComboBox->ItemIndex) {
		case 0: colstr.sprintf(_T("%d,%d,%d"),			r2, g2, b2);	break;
		case 1: colstr.sprintf(_T("%d,%d,%d"),			h2, s2, v2);	break;
		case 2: colstr.sprintf(_T("%d,%d,%d"),			h3, s3, l2);	break;
		case 3: colstr.sprintf(_T("0x%08x"),			cref2);			break;
		case 4: colstr.sprintf(_T("$00%02X%02X%02X"),	b2, g2, r2);	break;
		case 5: colstr.sprintf(_T("&H00%02X%02X%02X&"),	b2, g2, r2);	break;
		case 6: colstr.sprintf(_T("#%02x%02x%02x"),		r2, g2, b2);	break;
		case 7: colstr.sprintf(_T("RGB(0x%02x,0x%02x,0x%02x)"), r2, g2, b2);	break;
		case 8: colstr.sprintf(_T("RGB(%d,%d,%d)"),		r2, g2, b2);	break;
		case 9:
			colstr = ColorToString(c2);
			if (StartsStr('0', colstr)) colstr = RGBToWebColorName(cref2);
			if (StartsStr('#', colstr)) colstr.sprintf(_T("0x%08x"), cref2);
			break;
		default:	//10..12
			if (StartsStr("BITMAP(", FmtComboBox->Text)) {
				colstr.sprintf(_T("R%d G%d B%d"), r2, g2, b2);
				is_bmp = true;
			}
		}

		set_ErrColor(ColEdit, is_bmp);
		ColEdit->Text = colstr;
	}
}

//---------------------------------------------------------------------------
//���ʂ̃R�s�[
//---------------------------------------------------------------------------
void __fastcall TColorPicker::CopyColor()
{
	UnicodeString fmt = FmtComboBox->Text;
	//�r�b�g�}�b�v
	if (remove_top_s(fmt, _T("BITMAP("))) {
		int size = get_tkn(fmt, ')').ToIntDef(32);
		std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
		bmp->SetSize(size, size);
		TCanvas *cv = bmp->Canvas;
		cv->Brush->Color = Col2Panel->Color;
		cv->FillRect(Rect(0, 0, size, size));
		copy_to_Clipboard(bmp.get());
	}
	//������
	else {
		//�A���擾
		if (Repeating) {
			if (RepCount>0) {
				if (!RepCopyBuff.IsEmpty()) {
					UnicodeString sep_str;
					switch (RepComboBox->ItemIndex) {
					case 0:  sep_str = ",";		break;
					case 1:  sep_str = "\t";	break;
					case 2:  sep_str = " ";		break;
					case 3:  sep_str = "\n";	break;
					default: sep_str = EmptyStr;
					}
					RepCopyBuff += sep_str;
				}

				RepCopyBuff += ColEdit->Text;
				if (RepComboBox->ItemIndex==3) RepCopyBuff += "\n";
				RepCount--;
			}
			if (RepCount>0) return;			//�p��
			copy_to_Clipboard(RepCopyBuff);
		}
		//1��擾
		else {
			copy_to_Clipboard(ColEdit->Text);
		}
	}

	Close();
}

//---------------------------------------------------------------------------
//�A���擾�J�n
//---------------------------------------------------------------------------
void __fastcall TColorPicker::StartRepActionExecute(TObject *Sender)
{
	RepCount	= EditToInt(RepCntEdit);
	RepCopyBuff = EmptyStr;
	Repeating	= true;
}
//---------------------------------------------------------------------------
void __fastcall TColorPicker::StartRepActionUpdate(TObject *Sender)
{
	bool is_bmp = StartsStr("BITMAP(", FmtComboBox->Text);
	((TAction *)Sender)->Enabled = !is_bmp && !Repeating && EditToInt(RepCntEdit)>0;

	RepSttLabel->Font->Color = is_bmp? col_Error : get_TextColor();
	UnicodeString stt_str;
	if		(is_bmp)	stt_str = "�r�b�g�}�b�v�̃R�s�[�ł͗��p�ł��܂���B";
	else if (Repeating)	stt_str.sprintf(_T("�A���擾��...�@���� %u ��"), RepCount);
	else 				stt_str = "[�J�n]�ŘA���擾���n�܂�܂��B";
	RepSttLabel->Caption = stt_str;
}

//---------------------------------------------------------------------------
void __fastcall TColorPicker::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key==VK_ESCAPE)
		Close();
	else
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_IV) _T("#ColorPicker"));
}
//---------------------------------------------------------------------------

