//----------------------------------------------------------------------//
//	月齢/月相/満月計算													//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <math.h>
#include "usr_wic.h"
#include "moonage.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//----------------------------------------------------------
// 準ユリウス日を求める
//----------------------------------------------------------
double calcMjd(int y, int m, int d)
{
	if (m <= 2) {
		y--; m += 12;
	}
	double ret;
	ret  = floorl(365.25 * y) + floor(y/400.0) - floor(y/100.0) + floor(30.59*(m - 2)) + d;
	ret -= 678912.0;
	return ret;
}

//----------------------------------------------------------
// 月視黄経を計算
//----------------------------------------------------------
const long double m_DEG = M_PI/180.0;

long double CalcLambdaM(long double mjd)
{
	long double T = (mjd - 51544.5 + 0.00074074)/36525.0;
	long double y = 218.3166 + 481267.8811 * T - 0.0015 * T * T;

	y += 6.2888 * cosl(( 477198.868 * T +  44.963) * m_DEG);
	y += 1.2740 * cosl(( 413335.35  * T +  10.74)  * m_DEG);
	y += 0.6583 * cosl(( 890534.22  * T + 145.7)   * m_DEG);
	y += 0.2136 * cosl(( 954397.74  * T + 179.93)  * m_DEG);
	y += 0.1851 * cosl((  35999.05  * T +  87.53)  * m_DEG);
	y += 0.1144 * cosl(( 966404.0   * T + 276.5)   * m_DEG);
	y += 0.0588 * cosl((  63863.5   * T + 124.2)   * m_DEG);
	y += 0.0571 * cosl(( 377336.3   * T +  13.2)   * m_DEG);
	y += 0.0533 * cosl((1367733.1   * T + 280.7)   * m_DEG);
	y += 0.0458 * cosl(( 854535.2   * T + 148.2)   * m_DEG);
	y += 0.0409 * cosl(( 441199.8   * T +  47.4)   * m_DEG);
	y += 0.0347 * cosl(( 445267.1   * T +  27.9)   * m_DEG);
	y += 0.0304 * cosl(( 513197.9   * T + 222.5)   * m_DEG);
	y += 0.0154 * cosl((  75870.0   * T +  41.0)   * m_DEG);
	y += 0.0125 * cosl((1443603.0   * T +  52.0)   * m_DEG);
	y += 0.0110 * cosl(( 489205.0   * T + 142.0)   * m_DEG);
	y += 0.0107 * cosl((1303870.0   * T + 246.0)   * m_DEG);
	y += 0.0100 * cosl((1431597.0   * T + 315.0)   * m_DEG);
	y += 0.0085 * cosl(( 826671.0   * T + 111.0)   * m_DEG);
	y += 0.0079 * cosl(( 449334.0   * T + 188.0)   * m_DEG);
	y += 0.0068 * cosl(( 926533.0   * T + 323.0)   * m_DEG);
	y += 0.0052 * cosl((  31932.0   * T + 107.0)   * m_DEG);
	y += 0.0050 * cosl(( 481266.0   * T + 205.0)   * m_DEG);
	y += 0.0040 * cosl((1331734.0   * T + 283.0)   * m_DEG);
	y += 0.0040 * cosl((1844932.0   * T +  56.0)   * m_DEG);
	y += 0.0040 * cosl((    133.0   * T +  29.0)   * m_DEG);
	y += 0.0038 * cosl((1781068.0   * T +  21.0)   * m_DEG);
	y += 0.0037 * cosl(( 541062.0   * T + 259.0)   * m_DEG);
	y += 0.0028 * cosl((   1934.0   * T + 145.0)   * m_DEG);
	y += 0.0027 * cosl(( 918399.0   * T + 182.0)   * m_DEG);
	y += 0.0026 * cosl((1379739.0   * T +  17.0)   * m_DEG);
	y += 0.0024 * cosl((  99863.0   * T + 122.0)   * m_DEG);
	y += 0.0023 * cosl(( 922466.0   * T + 163.0)   * m_DEG);
	y += 0.0022 * cosl(( 818536.0   * T + 151.0)   * m_DEG);
	y += 0.0021 * cosl(( 990397.0   * T + 357.0)   * m_DEG);
	y += 0.0021 * cosl((  71998.0   * T +  85.0)   * m_DEG);
	y += 0.0021 * cosl(( 341337.0   * T +  16.0)   * m_DEG);
	y += 0.0018 * cosl(( 401329.0   * T + 274.0)   * m_DEG);
	y += 0.0016 * cosl((1856938.0   * T + 152.0)   * m_DEG);
	y += 0.0012 * cosl((1267871.0   * T + 249.0)   * m_DEG);
	y += 0.0011 * cosl((1920802.0   * T + 186.0)   * m_DEG);
	y += 0.0009 * cosl(( 858602.0   * T + 129.0)   * m_DEG);
	y += 0.0008 * cosl((1403732.0   * T +  98.0)   * m_DEG);
	y += 0.0007 * cosl(( 790672.0   * T + 114.0)   * m_DEG);
	y += 0.0007 * cosl(( 405201.0   * T +  50.0)   * m_DEG);
	y += 0.0007 * cosl(( 485333.0   * T + 186.0)   * m_DEG);
	y += 0.0007 * cosl((  27864.0   * T + 127.0)   * m_DEG);
	y += 0.0006 * cosl(( 111869.0   * T + 38.0)    * m_DEG);
	y += 0.0006 * cosl((2258267.0   * T + 156.0)   * m_DEG);
	y += 0.0005 * cosl((1908795.0   * T + 90.0)    * m_DEG);
	y += 0.0005 * cosl((1745069.0   * T + 24.0)    * m_DEG);
	y += 0.0005 * cosl(( 509131.0   * T + 242.0)   * m_DEG);
	y += 0.0004 * cosl((  39871.0   * T + 223.0)   * m_DEG);
	y += 0.0004 * cosl((  12006.0   * T + 187.0)   * m_DEG);
	y += 0.0003 * cosl(( 958465.0   * T + 340.0)   * m_DEG);
	y += 0.0003 * cosl(( 381404.0   * T + 354.0)   * m_DEG);
	y += 0.0003 * cosl(( 349472.0   * T + 337.0)   * m_DEG);
	y += 0.0003 * cosl((1808933.0   * T + 58.0)    * m_DEG);
	y += 0.0003 * cosl(( 549197.0   * T + 220.0)   * m_DEG);
	y += 0.0003 * cosl((   4067.0   * T + 70.0)    * m_DEG);
	y += 0.0003 * cosl((2322131.0   * T + 191.0)   * m_DEG);

	y = fmodl(y, 360.0);
	if (y < 0) y += 360.0;

	return y;
}

//----------------------------------------------------------
// 太陽視黄経の計算
//----------------------------------------------------------
long double CalcLambdaS(long double mjd)
{
	long double Ts = (mjd - 51544.5 + 0.00074074)/365.25;
	long double ys = 280.4603 + 360.00769 * Ts;

	ys += (1.9146 - 0.00005 * Ts) * sin((359.991 * Ts + 357.538) * m_DEG);
	ys += 0.0200 * sin((719.981 * Ts + 355.05) * m_DEG);
	ys += 0.0048 * sin(( 19.341 * Ts + 234.95) * m_DEG);
	ys += 0.0020 * sin(( 329.64 * Ts + 247.1)  * m_DEG);
	ys += 0.0018 * sin((4452.67 * Ts + 297.8)  * m_DEG);
	ys += 0.0018 * sin((   0.20 * Ts + 251.3)  * m_DEG);
	ys += 0.0015 * sin(( 450.37 * Ts + 343.2)  * m_DEG);
	ys += 0.0013 * sin(( 225.18 * Ts +  81.4)  * m_DEG);
	ys += 0.0008 * sin(( 659.29 * Ts + 132.5)  * m_DEG);
	ys += 0.0007 * sin((  90.38 * Ts + 153.3)  * m_DEG);
	ys += 0.0007 * sin((  30.35 * Ts + 206.8)  * m_DEG);
	ys += 0.0006 * sin(( 337.18 * Ts +  29.8)  * m_DEG);
	ys += 0.0005 * sin((   1.50 * Ts + 207.4)  * m_DEG);
	ys += 0.0005 * sin((  22.81 * Ts + 291.2)  * m_DEG);
	ys += 0.0004 * sin(( 315.56 * Ts + 234.9)  * m_DEG);
	ys += 0.0004 * sin(( 299.30 * Ts + 157.3)  * m_DEG);
	ys += 0.0004 * sin(( 720.02 * Ts +  21.1)  * m_DEG);
	ys += 0.0003 * sin((1079.97 * Ts + 352.5)  * m_DEG);
	ys += 0.0003 * sin((  44.43 * Ts + 329.7)  * m_DEG);

	ys  = fmodl(ys, 360.0);
	if (ys < 0) ys += 360.0;
	return ys;
}

//----------------------------------------------------------
// 指定準ユリウス日の月齢を求める
// 引数	mjd: 準ユリウス日
//----------------------------------------------------------
double MoonAgeJ(long double mjd)
{
	long double mjdc = mjd;
	long double dy;
	double mage;

	for (;;) {
		for (int i=1; i<10; i++) {
			dy = CalcLambdaM(mjdc) - CalcLambdaS(mjdc);
			if (fabsl(dy)<=0.05) break;
			if		(dy < -180.0) dy += 360.0;
			else if (dy > 180.0)  dy -= 360.0;
			mjdc -= (dy/12.1818);
		}
		mage = (double)(mjd - mjdc);
		if (mage>=0.0 && mage<30.0) break;
		if (mage<0.0) dy += 360.0; else dy -= 360.0;
		mjdc -= (dy/12.1818);
	}

	return mage;
}


//----------------------------------------------------------
// 指定準ユリウス日以降の指定月相の日時を求める
// 引数	mjd: 準ユリウス日/ p:月相(0.0～360.0)
//----------------------------------------------------------
double NextMoonPhaseJ(long double mjd, double p)
{
	long double mjdf = mjd;
	long double dy;

	for (;;) {
		for (int i=1; i<10; i++) {
			dy = (p - (CalcLambdaM(mjdf) - CalcLambdaS(mjdf)));
			if (fabsl(dy) <= 0.05) break;
			if		(dy <- 180.0) dy += 360.0;
			else if (dy > 180.0)  dy -= 360.0;
			mjdf += (dy/12.1818);
		}
		double to_p = (double)(mjdf - mjd);
		if (to_p>=0.0 && to_p<30.0) break;
		if (to_p<0.0) dy += 360.0; else dy -= 360.0;
		mjdf += (dy/12.1818);
	}
	return mjdf;
}


//----------------------------------------------------------
// 指定年月日(JST)以降の指定月相の日時を求める
// 引数・戻り値	t:基準日時/  p:月相(0.0～360.0)
//----------------------------------------------------------
void NextMoonPhase(TDateTime *t, double p)
{
	unsigned short y,m,d, h,n,s,c;
	DecodeDate(*t, y, m, d);
	DecodeTime(*t, h, n, s, c);
	long double mjdJ = calcMjd(y, m, d) + (h/24.0 + n/1440.0 - 0.375);	//JST
	double dd = NextMoonPhaseJ(mjdJ, p) - mjdJ;
	//次が1時間以内だったらその次を検索
	if (dd < (1.0/24.0)) {
		mjdJ += 1.0;
		dd = NextMoonPhaseJ(mjdJ, p) - mjdJ;
	}
	*t += dd;
	*t += 30.0/1440.0;	//時以下四捨五入
}

//----------------------------------------------------------
//月についての情報を取得
//----------------------------------------------------------
void GetMoonInfo(mooninf *mf)
{
	unsigned short y,m,d, h,n,s,c;
	DecodeDate(mf->cur_dt, y, m, d);
	DecodeTime(mf->cur_dt, h, n, s, c);
	long double mjdJ = calcMjd(y, m, d) + (h/24.0 + n/1440.0 - 0.375);

	//月齢
	mf->age = MoonAgeJ(mjdJ);

	//月相(360換算)
	long double dy = CalcLambdaM(mjdJ) - CalcLambdaS(mjdJ);
	if (dy < 0.0) dy += 360.0;
	mf->phase = dy/360.0;

	//輝面比
	mf->illuminated = (1.0 - cos(M_PI * 2.0 * mf->phase))/2.0;
}

//----------------------------------------------------------
//月を描画
//----------------------------------------------------------
void DrawMoon(TCanvas *cv, int x0, int y0, int rr,
	double mp, TColor mc, TColor sc, TColor bg)
{
	rr *= 2;	//一旦2倍サイズの画像を作成
	std::unique_ptr<Graphics::TBitmap> tmp_buf(new Graphics::TBitmap());
	TCanvas *tmp_cv = tmp_buf->Canvas;
	tmp_buf->SetSize(rr, rr);
	TRect rc = Rect(0, 0, rr, rr);
	tmp_cv->Brush->Color = bg;
	tmp_cv->FillRect(rc);

	//輝面
	HRGN hIllumRgn = ::CreateRectRgn(0, 0, 0, 0);
	HRGN hMoonRgn  = ::CreateEllipticRgn(0, 0, rr, rr);
	::CombineRgn(hIllumRgn, hIllumRgn, hMoonRgn, RGN_OR);
	HRGN hTmpRgn;
	if (mp < 0.5)
		hTmpRgn = ::CreateRectRgn(rr/2, 0, rr, rr);
	else
		hTmpRgn = ::CreateRectRgn(0, 0, rr/2, rr);
	::CombineRgn(hIllumRgn, hIllumRgn, hTmpRgn, RGN_AND);
	::DeleteObject(hTmpRgn);
	int md = floor(rr/2.0*(1.0-fabs(cos(M_PI*2.0*mp))));
	hTmpRgn = ::CreateEllipticRgn(md, 0, rr - md, rr);
	if (mp<=0.25 || mp>=0.75)
		::CombineRgn(hIllumRgn, hIllumRgn, hTmpRgn, RGN_DIFF);
	else
		::CombineRgn(hIllumRgn, hIllumRgn, hTmpRgn, RGN_OR);
	::DeleteObject(hTmpRgn);
	HRGN hOrgRgn = (HRGN)::SelectObject(tmp_cv->Handle, hIllumRgn);
	tmp_cv->Brush->Color = mc;
	tmp_cv->FillRect(rc);

	//影面
	HRGN hShadowRgn = ::CreateRectRgn(0, 0, 0, 0);
	::CombineRgn(hShadowRgn, hMoonRgn, hIllumRgn, RGN_XOR);
	::SelectObject(tmp_cv->Handle, hShadowRgn);
	tmp_cv->Brush->Color = sc;
	tmp_cv->FillRect(rc);
	::SelectObject(cv->Handle, hOrgRgn);
	::DeleteObject(hIllumRgn);
	::DeleteObject(hShadowRgn);
	::DeleteObject(hMoonRgn);

	//元のサイズに戻して描画
	rr /= 2;
	std::unique_ptr<Graphics::TBitmap> img_buf(new Graphics::TBitmap());
	if (WIC_resize_image(tmp_buf.get(), img_buf.get(), 0.0, rr, rr, 3))	//ファントリサンプリング
		cv->Draw(x0, y0, img_buf.get());
}
//----------------------------------------------------------
void DrawMoon(TImage *img, double mp, TColor mc, TColor sc, TColor bg)
{
	std::unique_ptr<Graphics::TBitmap> v_bmp(new Graphics::TBitmap());
	TCanvas *cv = v_bmp->Canvas;
	v_bmp->SetSize(img->ClientWidth, img->ClientHeight);
	cv->Brush->Color = bg;
	cv->FillRect(img->ClientRect);

	int rr = std::min(v_bmp->Width, v_bmp->Height) * 4/5;
	int x0 = (v_bmp->Width - rr)/2;
	int y0 = (v_bmp->Height - rr)/2;
	DrawMoon(cv, x0, y0, rr, mp, mc, sc, bg);
	img->Picture->Bitmap->Assign(v_bmp.get());
	img->Transparent = false;
}
//----------------------------------------------------------
