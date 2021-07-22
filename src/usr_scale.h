//----------------------------------------------------------------------//
// スケーリング処理														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrScaleH
#define UsrScaleH

//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
#define DEFAULT_PPI	96

//----------------------------------------------------------------------
//スケーリングされた整数値を取得
//----------------------------------------------------------------------
inline int ScaledInt(int n, TControl *cp = NULL)
{
	int ppi = cp? cp->CurrentPPI : Screen->ActiveForm? Screen->ActiveForm->CurrentPPI : Application->MainForm->CurrentPPI;
	return (n * ppi / DEFAULT_PPI);
}
//----------------------------------------------------------------------
inline int UnscaledInt(int n, TControl *cp = NULL)
{
	int ppi = cp? cp->CurrentPPI : Screen->ActiveForm? Screen->ActiveForm->CurrentPPI : Application->MainForm->CurrentPPI;
	return floor((n * 1.0 * DEFAULT_PPI / ppi) + 0.5);
}

//---------------------------------------------------------------------------
//スケーリングされたフォントを割り当てる
//----------------------------------------------------------------------
void AssignScaledFont(TFont *dst, TFont *src, TControl *cp = NULL, TColor fg = Graphics::clNone);

//---------------------------------------------------------------------------
#endif

