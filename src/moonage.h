//----------------------------------------------------------------------//
//	����/����/�����v�Z													//
//																		//
//----------------------------------------------------------------------//
#ifndef moonageH
#define moonageH
//---------------------------------------------------------------------------
#include <vcl.h>

//���ɂ��Ă̏����擾
struct mooninf {
	TDateTime cur_dt;		//���݂̓��t
	double age;				//����
	double phase;			//����	(0.0�`1.0)
	double illuminated;		//�P�ʔ�(0.0�`1.0)
};

void GetMoonInfo(mooninf *mf);

void NextMoonPhase(TDateTime *t, double p);

void DrawMoon(TCanvas *cv, int x0, int y0, int rr, double mp, TColor mc, TColor sc, TColor bg);
void DrawMoon(TImage *img, double mp, TColor mc, TColor sc, TColor bg);

//---------------------------------------------------------------------------
#endif
