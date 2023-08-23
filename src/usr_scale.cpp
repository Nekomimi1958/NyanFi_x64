//----------------------------------------------------------------------//
// スケーリング処理														//
//																		//
//----------------------------------------------------------------------//
#include "usr_scale.h"

//---------------------------------------------------------------------------
//スケーリングされたフォントを取得
//---------------------------------------------------------------------------
void GetScaledFont(TFont *s_font, TFont *font, TControl *cp)
{
	s_font->Assign(font? font : Application->DefaultFont);
	s_font->Height = ScaledInt(-s_font->Size*DEFAULT_PPI/72, cp);
}

//---------------------------------------------------------------------------
//コントロールにスケーリングされたフォントを設定
//---------------------------------------------------------------------------
void AssignScaledFont(TWinControl *cp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	GetScaledFont(s_font.get(), font, cp);

	if		(cp->ClassNameIs("TListBox"))		((TListBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TCheckListBox"))	((TCheckListBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TToolBar"))		((TToolBar *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TStatusBar"))		((TStatusBar *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TTabControl"))	((TTabControl *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TTreeView"))		((TTreeView *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TEdit"))			((TEdit *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TComboBox"))		((TComboBox *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TRichEdit"))		((TRichEdit *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("TStringGrid"))	((TStringGrid *)cp)->Font->Assign(s_font.get());
	else if (cp->ClassNameIs("THeaderControl"))	((THeaderControl *)cp)->Font->Assign(s_font.get());
}
//---------------------------------------------------------------------------
void AssignScaledFont(TLabel *lp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	GetScaledFont(s_font.get(), font, lp);
	lp->Font->Assign(s_font.get());
}
//---------------------------------------------------------------------------
void AssignScaledFont(TPaintBox *pp, TFont *font)
{
	std::unique_ptr<TFont> s_font(new TFont());
	GetScaledFont(s_font.get(), font, pp);
	pp->Font->Assign(s_font.get());
}
//---------------------------------------------------------------------------
