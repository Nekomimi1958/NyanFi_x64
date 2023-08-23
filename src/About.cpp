//----------------------------------------------------------------------//
// NyanFi																//
//  ÉoÅ[ÉWÉáÉìèÓïÒ														//
//----------------------------------------------------------------------//
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
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	bmp->Assign(ico.get());
	TImageCollectionItem *ip = ImageCollection1->Images->Add();
	TImageCollectionSourceItem *sp = ip->SourceImages->Add();
	sp->Image->Assign(bmp.get());
	VirtualImage1->ImageIndex = ImageCollection1->Images->Count - 1;
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::FormShow(TObject *Sender)
{
	SetDarkWinTheme(this);

	ProductName->Font->Color = get_TextColor();
	SupportURL->Font->Color  = IsDarkMode? TColor(RGB(0x66, 0x99, 0xFF)) : clNavy;
	ListNyanBtn->Enabled	 = ScrMode==SCMD_FLIST;
	OKButton->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::SupportURLClick(TObject *Sender)
{
	Execute_ex(SupportURL->Caption);
}
//---------------------------------------------------------------------------
void __fastcall TAboutBox::ListNyanBtnClick(TObject *Sender)
{
	HWND hWnd = Application->MainFormHandle;
	if (hWnd) {
		if (::IsIconic(hWnd)) ::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		UnicodeString lbuf = "-XListNyanFi_ED;XT;LS";
		COPYDATASTRUCT cd;
		cd.dwData = 1;
		cd.cbData = sizeof(_TCHAR) * (lbuf.Length() + 1);
		cd.lpData = lbuf.c_str();
		::SendMessage(hWnd, WM_COPYDATA, 0, (LPARAM)&cd);
		Close();
	}
}
//---------------------------------------------------------------------------
