//----------------------------------------------------------------------//
// ÉoÅ[ÉWÉáÉìèÓïÒ														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include "Global.h"
#include "About.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TAboutBox *AboutBox = NULL;

//---------------------------------------------------------------------
__fastcall TAboutBox::TAboutBox(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormCreate(TObject *Sender)
{
	std::unique_ptr<Graphics::TIcon> ico(new Graphics::TIcon());
	ico->Handle = (HICON)::LoadImage(HInstance, _T("NYANFI_ICO"), IMAGE_ICON, 48, 48, LR_SHARED);

	ProgramIcon->Picture->Assign(ico.get());
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	SetDarkWinTheme(this);

	ProductName->Font->Assign(Application->DefaultFont);
	ProductName->Font->Size  = 20;
	ProductName->Font->Color = get_TextColor();
	SupportURL->Font->Color  = IsDarkMode? TColor(RGB(0x66, 0x99, 0xFF)) : clNavy;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::SupportURLClick(TObject *Sender)
{
	Execute_ex(SupportURL->Caption);
}
//---------------------------------------------------------------------------

