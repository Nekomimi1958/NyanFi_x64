//----------------------------------------------------------------------//
// NyanFi																//
//  ƒ‹[ƒy																//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "Global.h"
#include "MainFrm.h"
#include "LoupeFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoupeForm *LoupeForm;

//---------------------------------------------------------------------------
__fastcall TLoupeForm::TLoupeForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormShow(TObject *Sender)
{
	if (Floating) IniFile->LoadPosInfo(this, 50, 100, 200, 200);

	SetToolWinBorder(this, Floating);

	DragKind = NyanFiForm->ImgSidePanel->Visible? dkDock : dkDrag;
	DragMode = NyanFiForm->ImgSidePanel->Visible? dmAutomatic : dmManual;

	OptPanel->Visible = IniFile->ReadBoolGen(_T("LoupeMagBtn"),	true);

	setup_ToolBar(MagToolBar);

	int n = IniFile->ReadIntGen(_T("LoupeMag"),	1);
	for (int i=0; i<MagToolBar->ButtonCount; i++)
		MagToolBar->Buttons[i]->Down = (MagToolBar->Buttons[i]->Tag==n);
}
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormHide(TObject *Sender)
{
	if (Floating) IniFile->SavePosInfo(this);
	SaveOption();

	if (!Floating) ShowLoupe = false;
}
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::FormEndDock(TObject *Sender, TObject *Target, int X, int Y)
{
	SetToolWinBorder(this, Floating);
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::SaveOption()
{
	IniFile->WriteBoolGen(_T("LoupeFloating"),	Floating);

	IniFile->WriteBoolGen(_T("LoupeMagBtn"),	OptPanel->Visible);

	int n = 1;
	for (int i=0; i<MagToolBar->ButtonCount; i++)
		if (MagToolBar->Buttons[i]->Down) n = MagToolBar->Buttons[i]->Tag;
	IniFile->WriteIntGen(_T("LoupeMag"), n);

	if (!Floating) {
		TPanel *pp = NyanFiForm->LoupeDockPanel;
		IniFile->WriteScaledIntGen(_T("LoupeHeight"), pp->Height, this);
		pp->Constraints->MinHeight = 1;
		pp->Constraints->MaxHeight = 1;
		NyanFiForm->SideDockPanel->Height = NyanFiForm->HistDockPanel->Height;
		NyanFiForm->SideDockSplitter->Height = 0;
	}
}

//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ToolBtnClick(TObject *Sender)
{
	DrawImage();
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//•`‰æ
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::DrawImage(Graphics::TBitmap *bmp, int x, int y)
{
	if (!ImgViewThread) return;

	if (bmp) {
		int n = 1;
		for (int i=0; i<MagToolBar->ButtonCount; i++)
			if (MagToolBar->Buttons[i]->Down) n = MagToolBar->Buttons[i]->Tag;
		int vw = LoupeImage->Width / n;
		int vh = LoupeImage->Height/ n;
		TRect irc = Rect(x - vw/2, y - vh/2, x + vw/2, y + vh/2);

		std::unique_ptr<Graphics::TBitmap> tmp_bmp(new Graphics::TBitmap());
		tmp_bmp->SetSize(LoupeImage->Width, LoupeImage->Height);
		tmp_bmp->Canvas->Lock();
		tmp_bmp->Canvas->Brush->Color = col_bgImage;
		tmp_bmp->Canvas->FillRect(LoupeImage->ClientRect);
		tmp_bmp->Canvas->CopyRect(LoupeImage->ClientRect, bmp->Canvas, irc);
		if (ImgViewThread->GrayScaled) WIC_grayscale_image(tmp_bmp.get());
		LoupeImage->Picture->Bitmap->Assign(tmp_bmp.get());
		tmp_bmp->Canvas->Unlock();
	}
	else {
		LoupeImage->Picture->Bitmap->Handle = NULL;
	}
}

//---------------------------------------------------------------------------
//“Ë‚«”²‚¯•\Ž¦
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::LoupeImageMouseMove(TObject *Sender, TShiftState Shift, int X, int Y)
{
	NyanFiForm->UpdateLoupe();
}

//---------------------------------------------------------------------------
//”{—¦
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::MagActionExecute(TObject *Sender)
{
	for (int i=0; i<MagToolBar->ButtonCount; i++)
		MagToolBar->Buttons[i]->Down = (i==((TAction *)Sender)->Tag);
}
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::MagActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = MagToolBar->Buttons[ap->Tag]->Down;
}

//---------------------------------------------------------------------------
//”{—¦ƒ{ƒ^ƒ“‚Ì•\Ž¦
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ShowBtnActionExecute(TObject *Sender)
{
	OptPanel->Visible = !OptPanel->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TLoupeForm::ShowBtnActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Checked = OptPanel->Visible;
}
//---------------------------------------------------------------------------

