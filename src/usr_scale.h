//----------------------------------------------------------------------//
// �X�P�[�����O����														//
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
//���݂�PPI���擾
//----------------------------------------------------------------------
inline int GetCurPPI(TControl *cp = NULL)
{
	return (			cp? cp->CurrentPPI : 
		Screen->ActiveForm? Screen->ActiveForm->CurrentPPI
						  : Application->MainForm->CurrentPPI);
}

//----------------------------------------------------------------------
//�X�P�[�����O���ꂽ�����l���擾
//----------------------------------------------------------------------
#define SCALED_THIS(n)		MulDiv(n, CurrentPPI, DEFAULT_PPI)
#define UNSCALED_THIS(n)	MulDiv(n, DEFAULT_PPI, CurrentPPI)

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
//�V�X�e���E���g���b�N�̎擾
//----------------------------------------------------------------------
inline int get_SysMetricsForPPI(int idx, int ppi)
{
	if (IsWindows10OrGreater())
		return GetSystemMetricsForDpi(idx, ppi);
	else
		return GetSystemMetrics(idx);
}

//---------------------------------------------------------------------------
void GetScaledFont(TFont *s_font, TFont *font, TControl *cp);
void AssignScaledFont(TWinControl *cp, TFont *font = NULL);
void AssignScaledFont(TLabel *lp, TFont *font = NULL);
void AssignScaledFont(TPaintBox *pp, TFont *font = NULL);

//---------------------------------------------------------------------------
#endif

