//----------------------------------------------------------------------//
// スウォッチブック・パネル												//
//																		//
//----------------------------------------------------------------------//
#include "usr_scale.h"
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "UIniFile.h"
#include "usr_swatch.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//色見本のカラーデーブル (TColor の整数値)
//最後の16色は「色の設定」ダイアログで作成した色
//---------------------------------------------------------------------------
int SWATCH_ColTable[MAX_SWATCH_COLOR] =
{
	0x000000ff,0x0000ffff,0x0000ff00,0x00ffff00,0x00ff0000,0x00ff00ff,0x00ffffff,0x00e3e3e3,
	0x00c7c7c7,0x00aaaaaa,0x008e8e8e,0x00717171,0x00555555,0x00383838,0x001c1c1c,0x00000000,
	0x001000e6,0x0036f3ff,0x004b9b00,0x00e69f00,0x00851a1d,0x007c00e4,0x00a6c0d1,0x007d93a6,
	0x005b6b7f,0x0040495a,0x002c2f37,0x0075aace,0x005489b3,0x00376d99,0x00215281,0x000c396a,
	0x00789bf2,0x0082b4f6,0x008dcefa,0x009ff8ff,0x009de3cc,0x009bd6ac,0x009aca8a,0x00c9cc85,
	0x00f2cd7e,0x00d8aa89,0x00c9948d,0x00ba818f,0x00bc88aa,0x00bd8fc4,0x00c19df1,0x009f9cf2,
	0x004369ec,0x004e91f1,0x0058b7f8,0x0068f6ff,0x006dd6b3,0x006fc37f,0x0070b231,0x00b2b510,
	0x00ecb500,0x00c78844,0x00b26d55,0x009e4f5f,0x009e5589,0x009f5cad,0x00a066ea,0x007667eb,
	0x001000e6,0x000e62ec,0x001d99f3,0x0036f3ff,0x0036c68f,0x0043ad23,0x004b9b00,0x00979e01,
	0x00e69f00,0x00b56600,0x009a4400,0x00851a1d,0x00830e60,0x00810093,0x007c00e4,0x004e00e5,
	0x000000a4,0x000643a8,0x00116cad,0x0023acb7,0x00218e64,0x002d7e0d,0x00357200,0x006e7300,
	0x00a77401,0x00844800,0x00712c00,0x00620111,0x00600045,0x005d006a,0x005900a4,0x003400a4,
	0x0000007d,0x00032e80,0x000a5083,0x0018828a,0x00126b49,0x001d6000,0x00245700,0x00535800,
	0x00805800,0x00653401,0x00571a00,0x004b0105,0x00470032,0x00450050,0x0041007e,0x0021007d,

	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,
	0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000,0x00000000
};

//プレビュー用カラーテーブル
int SWATCH_ColBuff[MAX_SWATCH_COLOR];

//---------------------------------------------------------------------------
//スウォッチブックか?
//---------------------------------------------------------------------------
bool IsSwatchbook(UnicodeString fnam)
{
	return (
		test_FileExt(get_extension(fnam), ".ini")
		&& StartsText("Swatchbook", ExtractFileName(fnam))
		&& SameText(get_top_line(fnam), "[Swatchbook]"));
}

//---------------------------------------------------------------------------
//スウォッチブックをファイルに保存
//---------------------------------------------------------------------------
bool SaveSwatchbook(UnicodeString fnam)
{
	try {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->Add("[Swatchbook]");
		for (int i=0; i<MAX_SWATCH_COLOR-16; i++) {
			fbuf->Add(UnicodeString().sprintf(_T("Color%03u=%s"), i + 1, col_to_xRRGGBB((TColor)SWATCH_ColTable[i]).c_str()));
		}
		fbuf->SaveToFile(fnam);
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
//スウォッチブックをファイルから読み込み
//---------------------------------------------------------------------------
int LoadSwatchbook(
	UnicodeString fnam,
	bool tmp_sw)		//true = SWATCH_ColBuff に一時読込	(default = false)
{
	try {
		if (!file_exists(fnam)) Abort();

		std::unique_ptr<UsrIniFile> sb_file(new UsrIniFile(fnam));
		int cnt = 0;
		for (int i=0; i<MAX_SWATCH_COLOR-16; i++) {
			UnicodeString key; key.sprintf(_T("Color%03u"), i+1);
			UnicodeString s = sb_file->ReadString("Swatchbook", key, EmptyStr);
			if (s.Length()==6) {
				(tmp_sw? SWATCH_ColBuff[i] : SWATCH_ColTable[i]) = (int)xRRGGBB_to_col(s);
				cnt++;
			}
		}
		return cnt;
	}
	catch (...) {
		return 0;
	}
}

//---------------------------------------------------------------------------
//カスタムカラーを設定
//---------------------------------------------------------------------------
void SetCustomColToSwatch(TStrings *lst)
{
	int idx = MAX_SWATCH_COLOR - 16;
	for (int i=0; i<16; i++) {
		SWATCH_ColTable[idx++] = (i<lst->Count)? StrToIntDef("0x" + lst->ValueFromIndex[i], 0) : 0;
	}
}

//---------------------------------------------------------------------------
//描画 (16列8行)
//---------------------------------------------------------------------------
void DrawSwatchbook(
	TCanvas *cv,
	TRect rc,
	bool tmp_sw)		//true = SWATCH_ColBuff の内容を描画	(default = false)
{
	cv->Brush->Color = TColor(RGB(64,64,64));
	cv->FillRect(rc);

	int wd = ((rc.Width() - ScaledInt(8)) / 16) - 1;
	int hi = ((rc.Height() - ScaledInt(8)) / 8) - 1;
	int s_4 = ScaledInt(4);

	TRect trc = Rect(0,0, wd, hi);
	for (int i=0; i<MAX_SWATCH_COLOR; i++) {
		int x = rc.Left + s_4 + (wd + 1) * (i%16);
		int y = rc.Top  + s_4 + (hi + 1) * (i/16);
		trc.Location = Point(x, y);
		cv->Brush->Color = (TColor)(tmp_sw? SWATCH_ColBuff[i] : SWATCH_ColTable[i]);
		cv->FillRect(trc);
	}
}

//---------------------------------------------------------------------------
// UsrSwatchPanel クラス
//---------------------------------------------------------------------------
__fastcall UsrSwatchPanel::UsrSwatchPanel(TComponent* Owner) : TPanel(Owner)
{
	Visible    = false;
	Align	   = alNone;
	Width	   = 248;
	Height	   = 128;
	BevelOuter = bvNone;

	StyleElements >> seClient;
	Color = TColor(RGB(64,64,64));

	SwatchPaintBox = new TPaintBox(this);
	SwatchPaintBox->Parent  = this;
	SwatchPaintBox->Align   = alClient;
	SwatchPaintBox->OnPaint = SwatchPaintBoxPaint;
}

//---------------------------------------------------------------------------
//描画 (16列8行)
//---------------------------------------------------------------------------
void __fastcall UsrSwatchPanel::SwatchPaintBoxPaint(TObject *Sender)
{
	DrawSwatchbook(SwatchPaintBox->Canvas, ClientRect);
}

//---------------------------------------------------------------------------
//16列8段のパネルサイズをタイルサイズに基づいて設定
//---------------------------------------------------------------------------
void __fastcall UsrSwatchPanel::SetPanelSize16x8(int tile_size)
{
	Align  = alNone;
	Anchors.Clear();
	Anchors << akRight << akBottom;

	ClientWidth  = ScaledInt(tile_size * 16 + 8, this) + 16;
	ClientHeight = ScaledInt(tile_size * 8  + 8, this) + 8;
}
//---------------------------------------------------------------------------
