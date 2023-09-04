//----------------------------------------------------------------------//
// スケーリング処理														//
//																		//
//----------------------------------------------------------------------//
#include "usr_scale.h"

//---------------------------------------------------------------------------
//スケーリングされたフォントを割り当てる
//---------------------------------------------------------------------------
void AssignScaledFont(
	TFont *s_font, 		//割当先フォント
	TFont *font,
	TControl *cp,		//PPIを取得するコントロール
	int size,			//サイズ	(default = 0 : サイズ指定なし)
	TColor col)			//色		(default = clNone)
{
	s_font->Assign(font? font : Application->DefaultFont);
	if (size>0) s_font->Size = size;
	s_font->Height = ScaledInt(MulDiv(-s_font->Size, DEFAULT_PPI, 72), cp);
	if (col!=Graphics::clNone) s_font->Color = col;
}

//---------------------------------------------------------------------------
//コントロールにスケーリングされたフォントを設定
//---------------------------------------------------------------------------
void AssignScaledFont(TWinControl *cp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	AssignScaledFont(s_font.get(), font, cp);

	if		(cp->ClassNameIs("TListBox"))		((TListBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TCheckListBox"))	((TCheckListBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TToolBar"))		((TToolBar *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TStatusBar"))		((TStatusBar *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TTabControl"))	((TTabControl *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TTreeView"))		((TTreeView *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TEdit"))			((TEdit *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TRichEdit"))		((TRichEdit *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TMemo"))			((TMemo *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TComboBox"))		((TComboBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TStringGrid"))	((TStringGrid *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("THeaderControl"))	((THeaderControl *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TPanel"))			((TPanel *)cp)->Font->Assign(s_font.get());
}
//---------------------------------------------------------------------------
void AssignScaledFont(TLabel *lp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	AssignScaledFont(s_font.get(), font, lp);
	lp->Font->Assign(s_font.get());
}
//---------------------------------------------------------------------------
void AssignScaledFont(TPaintBox *pp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	AssignScaledFont(s_font.get(), font, pp);
	pp->Font->Assign(s_font.get());
}

//---------------------------------------------------------------------------
//サイズからスケーリングされたフォント高を取得
//---------------------------------------------------------------------------
int ScaledFontHeight(int sz, TControl *cp)
{
	return ScaledInt(MulDiv(-sz, DEFAULT_PPI, 72), cp);
}
//---------------------------------------------------------------------------
