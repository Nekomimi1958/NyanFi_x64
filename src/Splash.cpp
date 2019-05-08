//----------------------------------------------------------------------//
// �X�v���b�V���E�t�H�[��												//
//																		//
//	�I�v���W�F�N�g�I�v�V�����ŁA���������t�H�[������O���Ă�������		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include "Splash.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSplashForm *SplashForm = NULL;

//---------------------------------------------------------------------------
__fastcall TSplashForm::TSplashForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormCreate(TObject *Sender)
{
	TitleLabel->Font->Assign(Application->DefaultFont);
	TitleLabel->Font->Size = 22;
	TitleLabel->Font->Color = clBlack;

	VerLabel->Font->Assign(Application->DefaultFont);
	VerLabel->Font->Size = 9;
	VerLabel->Font->Color = clBlack;

	MsgLabel->Font->Assign(Application->DefaultFont);
	MsgLabel->Font->Size = 9;
	MsgLabel->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormDestroy(TObject *Sender)
{
	SplashForm = NULL;
}

//---------------------------------------------------------------------------
//�w�i�̕`��
//---------------------------------------------------------------------------
void __fastcall TSplashForm::FormPaint(TObject *Sender)
{
	int c0 = RGB( 16,   0,  64);
	int c1 = RGB(128, 128, 192);
	int c2 = RGB(128, 192, 255);
	int h = ClientHeight;
	int w = ClientWidth;

	for (int y=0; y<h; y++) {
		double x = (cos(M_PI*4*(1.0*y/h - 0.5)) + 1.0)/2;
		double yr = 1.0* y / h;
		if (yr<0.25 || yr>=0.75) {
			Canvas->Pen->Color = TColor(RGB(
				std::min((int)(GetRValue(c0)*x + GetRValue(c1)*(1 - x)), 255),
				std::min((int)(GetGValue(c0)*x + GetGValue(c1)*(1 - x)), 255),
				std::min((int)(GetBValue(c0)*x + GetBValue(c1)*(1 - x)), 255)));
		}
		else if (yr<0.75) {
			Canvas->Pen->Color = TColor(RGB(
				std::min((int)(GetRValue(c2)*x + GetRValue(c1)*(1 - x)), 255),
				std::min((int)(GetGValue(c2)*x + GetGValue(c1)*(1 - x)), 255),
				std::min((int)(GetBValue(c2)*x + GetBValue(c1)*(1 - x)), 255)));
		}
		Canvas->MoveTo(0, y);
		Canvas->LineTo(w, y);
	}

	//�A�C�R��
	std::unique_ptr<Graphics::TIcon> ico(new Graphics::TIcon());
	ico->Handle = (HICON)::LoadImage(HInstance, _T("NYANFI_ICO"), IMAGE_ICON, 48, 48, LR_SHARED);
	Canvas->Draw((w - 48)/2, (h - 48)/2, ico.get());
}

//---------------------------------------------------------------------------
void __fastcall TSplashForm::SetMsgLabel(const _TCHAR *msg)
{
	MsgLabel->Caption = msg;
	Repaint();
}
//---------------------------------------------------------------------------

