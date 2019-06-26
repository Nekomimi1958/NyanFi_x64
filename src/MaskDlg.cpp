//----------------------------------------------------------------------//
// NyanFi																//
//  パスマスク															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MainFrm.h"
#include "MaskDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPathMaskDlg *PathMaskDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TPathMaskDlg::TPathMaskDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormCreate(TObject *Sender)
{
	ListScrPanel = new UsrScrollPanel(ListPanel, PathMaskListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	Caption = "パスマスク - " + get_LRUD_str();
	set_BtnMarkDark(HideOptBtn, UBMK_BDOWN);
	set_BtnMarkDark(ShowOptBtn, UBMK_BUP);
	HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
	ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);

	OptPanel->Visible	= IniFile->ReadBoolGen(_T("PathMaskDlgShowOpt"),  true);
	BlankPanel->Visible = !OptPanel->Visible;
	SetDarkWinTheme(OptPanel);
	SetDarkWinTheme(BlankPanel);

	InitializeListHeader(PathMaskHeader, _T("キー|名前|マスク"));
	PathMaskHeader->Sections->Items[0]->Width = IniFile->ReadInteger("PathMaskGrid", "ColWidth0",	40);
	PathMaskHeader->Sections->Items[1]->Width = IniFile->ReadInteger("PathMaskGrid", "ColWidth1",	200);
	adjust_HeaderSecWidth(PathMaskHeader, 2);

	TListBox *lp = PathMaskListBox;
	set_StdListBox(lp, LBTAG_OPT_FOCS);
	set_UsrScrPanel(ListScrPanel);

	lp->Items->Assign(PathMaskList);
	lp->SetFocus();
	UserModule->InitializeListBox(lp, ListScrPanel);

	ListScrPanel->UpdateKnob();

	SetOptBtn();
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();

	PathMaskList->Assign(PathMaskListBox->Items);

	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("PathMaskDlgShowOpt"),	OptPanel->Visible);

	IniFile->WriteInteger("PathMaskGrid", "ColWidth0",	PathMaskHeader->Sections->Items[0]->Width);
	IniFile->WriteInteger("PathMaskGrid", "ColWidth1",	PathMaskHeader->Sections->Items[1]->Width);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormDestroy(TObject *Sender)
{
	delete ListScrPanel;
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormResize(TObject *Sender)
{
	SetOptBtn();
	PathMaskListBox->Invalidate();
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskHeaderResize(TObject *Sender)
{
	adjust_HeaderSecWidth((THeaderControl *)Sender, 2);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	PathMaskHeaderResize(HeaderControl);
	PathMaskListBox->Repaint();
}

//---------------------------------------------------------------------------
//項目の描画
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(ListFont);
	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	if (Index<lp->Count) {
		TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[Index], 3, true);
		THeaderSections *sp = PathMaskHeader->Sections;
		cv->Font->Color  = col_fgList;
		cv->Font->Style  = cv->Font->Style << fsBold;
		int s_wd = cv->TextWidth(itm_buf[0]);
		int c_wd = sp->Items[0]->Width - 4;
		int xp = Rect.Left + Scaled2;
		int yp = Rect.Top  + get_TopMargin2(cv);
		if (s_wd<c_wd) xp += (c_wd - s_wd)/2;	//センタリング
		cv->TextOut(xp, yp, itm_buf[0]);
		xp = sp->Items[0]->Width + 1;
		cv->Font->Style  = cv->Font->Style >> fsBold;
		cv->TextOut(xp, yp, itm_buf[1]);
		xp += sp->Items[1]->Width + 1;
		cv->TextOut(xp, yp, itm_buf[2]);
	}

	//カーソル
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskListBoxClick(TObject *Sender)
{
	TListBox *lp = (TListBox*)Sender;
	int idx = lp->ItemIndex;  if (idx==-1) return;
	TStringDynArray itm_buf = get_csv_array(lp->Items->Strings[idx], 3, true);
	KeyEdit->Text  = itm_buf[0];
	DescEdit->Text = itm_buf[1];
	MaskEdit->Text = itm_buf[2];
}
//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	TListBox *lp = (TListBox*)Sender;

	int idx = -1;
	if (KeyStr.Length()==1) {
		for (int i=0; i<lp->Count && idx==-1; i++) {
			if (SameText(get_csv_item(lp->Items->Strings[i], 0), KeyStr)) idx = i;
		}
	}
	if (idx==-1 && equal_ENTER(KeyStr)) idx = lp->ItemIndex;
	if (idx!=-1) {
		UnicodeString kstr = get_csv_item(lp->Items->Strings[idx], 0);
		if (!kstr.IsEmpty()) {
			NyanFiForm->SetCurPathMask(kstr);
			Close();
		}
		else beep_Warn();
	}
}
//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::PathMaskListBoxDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}
//---------------------------------------------------------------------------
//項目を変更
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::EditItemActionExecute(TObject *Sender)
{
	TListBox *lp = PathMaskListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		lp->Items->Strings[idx] =
			UnicodeString().sprintf(_T("%s,%s,%s"),
				KeyEdit->Text.SubString(1, 1).UpperCase().c_str(),
				make_csv_str(DescEdit->Text).c_str(),  make_csv_str(MaskEdit->Text).c_str());
		lp->Repaint();
		lp->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::EditItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (PathMaskListBox->ItemIndex!=-1)?
		!KeyEdit->Text.IsEmpty() && !DescEdit->Text.IsEmpty() && !MaskEdit->Text.IsEmpty() : false;
}
//---------------------------------------------------------------------------
//項目を追加
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::AddItemActionExecute(TObject *Sender)
{
	TListBox *lp = PathMaskListBox;
	lp->Items->Add(UnicodeString().sprintf(_T("%s,%s,%s"),
		KeyEdit->Text.SubString(1, 1).UpperCase().c_str(),
		make_csv_str(DescEdit->Text).c_str(), make_csv_str(MaskEdit->Text).c_str()));
	lp->ItemIndex = lp->Count - 1;
	lp->SetFocus();
	ListScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled =
		!KeyEdit->Text.IsEmpty() && !DescEdit->Text.IsEmpty() && !MaskEdit->Text.IsEmpty();
}

//---------------------------------------------------------------------------
//オプションの開閉
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::SetOptBtn()
{
	if (ScrBarStyle==0)
		set_ControlRBCorner(PathMaskListBox, BlankPanel);
	else
		ListScrPanel->SetRBCornerPanel(BlankPanel);
}
//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::ChgOptBtnClick(TObject *Sender)
{
	OptPanel->Visible	= !OptPanel->Visible;
	BlankPanel->Visible = !OptPanel->Visible;
	ListScrPanel->UpdateKnob();
	SetOptBtn();
	if (!OptPanel->Visible) PathMaskListBox->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TPathMaskDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (USAME_TI(get_KeyStr(Key, Shift), "Alt+O"))
		ChgOptBtnClick(NULL);
	else
		SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_FL) _T("#PathMaskDlg"));
}
//---------------------------------------------------------------------------

