//----------------------------------------------------------------------//
// NyanFi																//
//  �����t�@�C���̏����_�C�A���O										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <System.DateUtils.hpp>
#include <RegularExpressions.hpp>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "SameDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSameNameDlg *SameNameDlg;

//---------------------------------------------------------------------------
__fastcall TSameNameDlg::TSameNameDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormCreate(TObject *Sender)
{
	CopyAll    = false;
	CopyMode   = 0;
	IsSamePath = false;
	TaskNo	   = -1;
	InhAllChk  = false;

	CurSrcName = CurDstName = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	set_ListBoxItemHi(InfoListBox);
	InfoListBox->Color = col_bgInf;

	UnicodeString tmp;
	InhAllChk = true;
	if (CopyMode!=CPYMD_MAN_REN) {
		tmp.sprintf(_T("SameDlgCopyAll%u"), TaskNo + 1);
		AllCheckBox->Checked = CopyAll = IniFile->ReadBoolGen(tmp.c_str(), true);
	}
	else {
		AllCheckBox->Checked = CopyAll = false;
	}
	InhAllChk = false;

	Mode0Btn->Enabled = !IsSamePath;
	Mode1Btn->Enabled = !IsSamePath;
	Mode2Btn->Enabled = !IsSamePath;

	if (IsSamePath && CopyMode<CPYMD_AUT_REN) CopyMode = CPYMD_MAN_REN;

	switch (CopyMode) {
	case CPYMD_OW:		Mode0Btn->Checked = true; break;
	case CPYMD_NEW:		Mode1Btn->Checked = true; break;
	case CPYMD_SKIP:	Mode2Btn->Checked = true; break;
	case CPYMD_AUT_REN:	Mode3Btn->Checked = true; break;
	case CPYMD_MAN_REN:	Mode4Btn->Checked = true; break;
	}

	ModeBtnClick(NULL);

	InfoListBox->Clear();
	InfoListBox->Items->Add(ExtractFileName(CurSrcName));

	InfoListBox->Items->Add("��: " + ExtractFileDir(CurSrcName));
	InfoListBox->Items->Add(EmptyStr);

	InfoListBox->Items->Add("��: " + ExtractFileDir(CurDstName));
	InfoListBox->Items->Add(EmptyStr);

	tmp = "�T�C�Y: ";
	if (CurSrcSize == CurDstSize)
		tmp += "����";
	else
		tmp.cat_sprintf(_T("�]����̕���%s"), (CurSrcSize<CurDstSize)? _T("�傫��") : _T("������"));
	InfoListBox->Items->Add(tmp);

	tmp = "�^�C��: ";
	if (WithinPastMilliSeconds(CurSrcTime, CurDstTime, TimeTolerance)) {
		tmp += "����";
		if (CurSrcTime!=CurDstTime) tmp += " (���e�덷��)";
	}
	else {
		tmp.cat_sprintf(_T("�]����̕���%s"), (CurSrcTime<CurDstTime)? _T("�V����") : _T("�Â�"));
	}
	InfoListBox->Items->Add(tmp);
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (CopyMode!=CPYMD_MAN_REN) {
		UnicodeString key;
		key.sprintf(_T("SameDlgCopyAll%u"), TaskNo + 1);
		IniFile->WriteBoolGen(key.c_str(), CopyAll);
	}
}

//---------------------------------------------------------------------------
//���[�h�ύX
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::ModeBtnClick(TObject *Sender)
{
	RenameEdit->Enabled = false;

	if (Mode0Btn->Checked) {
		CopyMode = CPYMD_OW;
		Mode0Btn->SetFocus();
	}
	else if (Mode1Btn->Checked) {
		CopyMode = CPYMD_NEW;
		Mode1Btn->SetFocus();
	}
	else if (Mode2Btn->Checked) {
		CopyMode = CPYMD_SKIP;
		Mode2Btn->SetFocus();
	}
	else if (Mode3Btn->Checked) {
		CopyMode = CPYMD_AUT_REN;
		Mode3Btn->SetFocus();
	}
	else if (Mode4Btn->Checked) {
		CopyMode = CPYMD_MAN_REN;
		SameNameDlg->AllCheckBox->Checked = CopyAll = false;
		RenameEdit->Enabled = true;
		RenameEdit->SetFocus();
		int p = pos_r(_T("."), RenameEdit->Text) - 1;
		if (p<0) p = RenameEdit->Text.Length();
		RenameEdit->SelStart = p;
	}
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::AllCheckBoxClick(TObject *Sender)
{
	if (InhAllChk) return;

	CopyAll = SameNameDlg->AllCheckBox->Checked;
	Mode4Btn->Enabled = !CopyAll;
	if (CopyAll && Mode4Btn->Checked) Mode3Btn->Checked = true;
	RenameEdit->Enabled = Mode4Btn->Checked;
}

//---------------------------------------------------------------------------
//��񗓂̕`��
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::InfoListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Brush->Color = col_bgInf;
	cv->FillRect(Rect);
	int xp = Rect.Left + Scaled4;
	int yp = Rect.Top  + get_TopMargin(cv);

	if (Index==2 || Index==4) {
		//�T�C�Y
		UnicodeString lbuf = "    " + get_FileSizeStr((Index==2)? CurSrcSize : CurDstSize) + " ";
		cv->Font->Color = col_fgList;
		cv->TextOut(xp, yp, lbuf);
		xp += cv->TextWidth(lbuf);
		//�^�C��
		TDateTime dt = (Index==2)? CurSrcTime : CurDstTime;
		cv->Font->Color = get_TimeColor(dt, col_fgList);
		cv->TextOut(xp, yp, format_DateTime(dt));
	}
	else {
		UnicodeString lbuf = lp->Items->Strings[Index];

		cv->Font->Color = (Index==0)? get_ExtColor(ExtractFileExt(lbuf)) :
			  (Index==1 || Index==3)? col_Folder : col_fgList;
	
		if (Index==0 || Index==1 || Index==3)
			lbuf = minimize_str(lbuf, cv, Rect.Width() - 8, OmitEndOfName);

		if (Index==1 || Index==3)
			PathNameOut(lbuf, cv, xp, yp);
		else
			cv->TextOut(xp, yp, lbuf);
	}
}

//---------------------------------------------------------------------------
//���O���ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::RenameEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(equal_UP(KeyStr))	 { Mode3Btn->SetFocus();	Key = 0; }
	else if (equal_DOWN(KeyStr)) { AllCheckBox->SetFocus();	Key = 0; }
	else SpecialEditProc(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::RenameEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key)) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TSameNameDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

