//----------------------------------------------------------------------//
//	Œ—î/Œ‘Š/–ŒŒvZ													//
//																		//
//----------------------------------------------------------------------//
#ifndef moonageH
#define moonageH
//---------------------------------------------------------------------------
#include <vcl.h>

//Œ‚É‚Â‚¢‚Ä‚Ìî•ñ‚ğæ“¾
struct mooninf {
	TDateTime cur_dt;		//Œ»İ‚Ì“ú•t
	double age;				//Œ—î
	double phase;			//Œ‘Š	(0.0`1.0)
	double illuminated;		//‹P–Ê”ä(0.0`1.0)
};

void GetMoonInfo(mooninf *mf);

void NextMoonPhase(TDateTime *t, double p);

void DrawMoon(TCanvas *cv, int x0, int y0, int rr, double mp, TColor mc, TColor sc, TColor bg);
void DrawMoon(TImage *img, double mp, TColor mc, TColor sc, TColor bg);

//---------------------------------------------------------------------------
#endif
