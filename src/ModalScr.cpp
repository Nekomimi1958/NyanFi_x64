//----------------------------------------------------------------------//
// NyanFi																//
//  モーダル表示効果用スクリーン										//
//----------------------------------------------------------------------//
#include "Global.h"
#include "ModalScr.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModalScrForm *ModalScrForm;

//---------------------------------------------------------------------------
__fastcall TModalScrForm::TModalScrForm(TComponent* Owner)
	: TForm(Owner)
{
	FullScr = false;
}
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::FormShow(TObject *Sender)
{
	Color			= col_ModalScr;
	AlphaBlendValue = ModalScrAlpha;

	if (FullScr) {
		set_window_pos_ex(Handle, Screen->DesktopRect);
	}
	else {
		TRect mon_rc = Application->MainForm->Monitor->BoundsRect;	mon_rc.Inflate(-1, -1);
		TRect app_rc = get_window_rect(Application->MainForm->Handle);
		set_window_pos_ex(Handle, app_rc.Contains(mon_rc)? mon_rc : app_rc);
	}
}
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::FormActivate(TObject *Sender)
{
	HWND hWnd = get_ModalWnd();
	if (hWnd) ::SetForegroundWindow(hWnd);
}
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::FormHide(TObject *Sender)
{
	FullScr = false;
}

//---------------------------------------------------------------------------
//デスクトップ全体を覆う
//---------------------------------------------------------------------------
void __fastcall TModalScrForm::CoverDesktop()
{
	if (!Visible && ModalScreen && ModalScrAlpha>0) {
		FullScr = true;
		Visible = true;
	}
}
//---------------------------------------------------------------------------

