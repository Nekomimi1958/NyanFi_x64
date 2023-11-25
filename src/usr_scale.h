//----------------------------------------------------------------------//
// スケーリング処理														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrScaleH
#define UsrScaleH

//---------------------------------------------------------------------------
#include <Vcl.ComCtrls.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
#define DEFAULT_PPI	96

//----------------------------------------------------------------------
//現在のPPIを取得
//----------------------------------------------------------------------
inline int GetCurPPI(TControl *cp = NULL)
{
	return (			cp? cp->CurrentPPI : 
		Screen->ActiveForm? Screen->ActiveForm->CurrentPPI
						  : Application->MainForm->CurrentPPI);
}

//----------------------------------------------------------------------
//スケーリングされた整数値を取得
//----------------------------------------------------------------------
#define SCALED_THIS(n)		MulDiv(n, CurrentPPI, DEFAULT_PPI)
#define UNSCALED_THIS(n)	MulDiv(n, DEFAULT_PPI, CurrentPPI)
#define SCALED_MAIN(n)		MulDiv(n, Application->MainForm->CurrentPPI, DEFAULT_PPI)

//----------------------------------------------------------------------
inline int ScaledInt(int n, TControl *cp = NULL)
{
	return MulDiv(n, GetCurPPI(cp), DEFAULT_PPI);
}
//----------------------------------------------------------------------
inline int UnscaledInt(int n, TControl *cp = NULL)
{
	return MulDiv(n, DEFAULT_PPI, GetCurPPI(cp));
}

//----------------------------------------------------------------------
//システム・メトリックの取得
//----------------------------------------------------------------------
inline int get_SysMetricsForPPI(int idx, int ppi)
{
	if (IsWindows10OrGreater())
		return GetSystemMetricsForDpi(idx, ppi);
	else
		return GetSystemMetrics(idx);
}

//---------------------------------------------------------------------------
int  get_FontHeightMgn(TFont *font, int mgn = 0, int min_mgn = 2);
int  get_FontHeightMgnS(TFont *font, int mgn = 0, int min_mgn = 2);

void AssignScaledFont(TFont *s_font, TFont *font, TControl *cp, int size = 0, TColor bg = Graphics::clNone);
void AssignScaledFont(TWinControl *cp, TFont *font = NULL);
void AssignScaledFont(TLabel *lp, TFont *font = NULL);
void AssignScaledFont(TPaintBox *pp, TFont *font = NULL);

int  ScaledFontHeight(int sz, TControl *cp);

//---------------------------------------------------------------------------
#endif

