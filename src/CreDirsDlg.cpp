//----------------------------------------------------------------------//
// NyanFi																//
//  �f�B���N�g���ꊇ�쐬�_�C�A���O										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <System.DateUtils.hpp>
#include "Global.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "InpExDlg.h"
#include "CreDirsDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCreateDirsDlg *CreateDirsDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TCreateDirsDlg::TCreateDirsDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	StrEdit->Tag = EDTAG_REF_CDIR;
	UserModule->SetUsrPopupMenu(this);
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	StartEdit->Text = IniFile->ReadStrGen(_T("CreDirsDlgStart"));
	IncEdit->Text	= IniFile->ReadStrGen(_T("CreDirsDlgInc"));

	if (IniFile->ReadIntGen(_T("CreDirsDlgSerMod"), 1)>0)
		PstSerRadioBtn->Checked = true;
	else
		PreSerRadioBtn->Checked = true;

	StrEdit->Text	= IniFile->ReadStrGen(_T("CreDirsDlgStr"));
	if (IniFile->ReadIntGen(_T("CreDirsDlgStrMod"), 1)>0)
		PstStrRadioBtn->Checked = true;
	else
		PreStrRadioBtn->Checked = true;

	DtFmtEdit->Text    = IniFile->ReadStrGen(_T("CreDirsDlgDtFmt"));
	DateMaskEdit->Text = IniFile->ReadStrGen(_T("CreDirsDlgDate"), FormatDateTime("yyyy/01/01", Now()));

	if (IniFile->ReadIntGen(_T("CreDirsDlgDateMod"), 1)>0)
		PstDateRadioBtn->Checked = true;
	else
		PreDateRadioBtn->Checked = true;

	RefDir = IniFile->ReadStrGen(_T("CreDirsDlgRefDir"));

	SetDarkWinTheme(this);
	ListMemo->Color 	  = get_WinColor();
	ListMemo->Font->Color = get_TextColor();
	ListMemo->Lines->Clear();
	ListMemo->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteStrGen(_T("CreDirsDlgStart"),		StartEdit);
	IniFile->WriteStrGen(_T("CreDirsDlgInc"),		IncEdit);
	IniFile->WriteIntGen(_T("CreDirsDlgSerMod"),	PstSerRadioBtn->Checked? 1 : 0);
	IniFile->WriteStrGen(_T("CreDirsDlgStr"),		StrEdit);
	IniFile->WriteIntGen(_T("CreDirsDlgStrMod"),	PstStrRadioBtn->Checked? 1 : 0);
	IniFile->WriteStrGen(_T("CreDirsDlgDtFmt"),		DtFmtEdit);
	IniFile->WriteStrGen(_T("CreDirsDlgDate"),		DateMaskEdit->Text);
	IniFile->WriteIntGen(_T("CreDirsDlgDateMod"),	PstDateRadioBtn->Checked? 1 : 0);
	IniFile->WriteStrGen(_T("CreDirsDlgRefDir"),	RefDir);
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = IsDarkMode? col_bgSttBar : scl_BtnFace;
	cv->FillRect(Rect);
	cv->Font->Color = IsDarkMode? col_fgSttBar : scl_BtnText;
	cv->TextOut(Rect.Left + ScaledIntX(2), Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//�A�Ԃ�t��
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddSerActionExecute(TObject *Sender)
{
	int sn	 = EditToInt(StartEdit);
	int dn	 = EditToInt(IncEdit, 1);
	int n_wd = StartEdit->Text.Length();

	cursor_HourGlass();
	UndoBuf = ListMemo->Lines->Text;
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Assign(ListMemo->Lines);
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		UnicodeString sno;
		sno.cat_sprintf(_T("%0*u"), n_wd, sn);
		if (PreSerRadioBtn->Checked) lbuf = sno + lbuf; else lbuf += sno;
		lst->Strings[i] = lbuf;
		sn += dn;
	}
	ListMemo->Lines->Assign(lst.get());
	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddSerActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !StartEdit->Text.IsEmpty() && EditToInt(IncEdit, 0)>0;
}
//---------------------------------------------------------------------------
//�������t��
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddStrActionExecute(TObject *Sender)
{
	cursor_HourGlass();
	UndoBuf = ListMemo->Lines->Text;
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Assign(ListMemo->Lines);
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (PreStrRadioBtn->Checked) lbuf = StrEdit->Text + lbuf; else lbuf += StrEdit->Text;
		lst->Strings[i] = lbuf;
	}
	ListMemo->Lines->Assign(lst.get());
	cursor_Default();
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddStrActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !StrEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//���t��t��
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddDateActionExecute(TObject *Sender)
{
	TDateTime dt;
	if (str_to_DateTime(DateMaskEdit->Text, &dt)) {
		cursor_HourGlass();
		UndoBuf = ListMemo->Lines->Text;
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Assign(ListMemo->Lines);
		UnicodeString fmt = DtFmtEdit->Text;
		int inc_mod = ContainsStr(fmt, "d")? 0 : ContainsStr(fmt, "m")? 1 : 2;
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			UnicodeString dbuf = FormatDateTime(DtFmtEdit->Text, dt);
			if (PreStrRadioBtn->Checked) lbuf = dbuf + lbuf; else lbuf += dbuf;
			lst->Strings[i] = lbuf;
			dt = (inc_mod==0)? IncDay(dt, 1) : (inc_mod==1)? IncMonth(dt, 1) : IncYear(dt, 1);
		}
		ListMemo->Lines->Assign(lst.get());
		cursor_Default();
	}
	else msgbox_ERR(USTR_IllegalDate);
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddDateActionUpdate(TObject *Sender)
{
	try {
		if (DtFmtEdit->Text.IsEmpty()) Abort();
		TDateTime(DateMaskEdit->Text);
		DateMaskEdit->Color 	  = get_WinColor();
		DateMaskEdit->Font->Color = get_TextColor();
		((TAction*)Sender)->Enabled = true;
	}
	catch (...) {
		set_ErrColor(DateMaskEdit, true);
		((TAction*)Sender)->Enabled = false;
	}
}

//---------------------------------------------------------------------------
//�󍀖ڂ�ǉ�
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::AddEmptyItemClick(TObject *Sender)
{
	int n = inputbox_ex_n(_T("�󍀖ڂ�ǉ�"), _T("���ڐ�"));
	if (n>0) {
		for (int i=0; i<n; i++) ListMemo->Lines->Add(EmptyStr);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���\�����Q��
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::GetSubDirItemClick(TObject *Sender)
{
	UnicodeString dnam = RefDir;
	if (UserModule->SelectDirEx(_T("�T�u�f�B���N�g�����擾"), dnam)) {
		cursor_HourGlass();
		StatusBar1->Panels->Items[0]->Text = "�擾��...";
		RefDir = dnam;
		std::unique_ptr<TStringList> lst(new TStringList());
		get_SubDirs(dnam, lst.get(), StatusBar1, 1);
		dnam = IncludeTrailingPathDelimiter(dnam);
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			remove_top_text(lbuf, dnam);
			lst->Strings[i] = lbuf;
		}
		lst->Sort();
		UndoBuf = ListMemo->Lines->Text;
		ListMemo->Lines->AddStrings(lst.get());
		cursor_Default();
	}
}

//---------------------------------------------------------------------------
//�֎~�����̒u��
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::RepCharItemClick(TObject *Sender)
{
	cursor_HourGlass();
	UndoBuf = ListMemo->Lines->Text;
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Assign(ListMemo->Lines);
	for (int i=0; i<lst->Count; i++) {
		lst->Strings[i] = ApplyCnvCharList(lst->Strings[i]);
	}
	ListMemo->Lines->Assign(lst.get());
	cursor_Default();
}

//---------------------------------------------------------------------------
//���ɖ߂�
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::UndoActionExecute(TObject *Sender)
{
	ListMemo->Lines->Text = UndoBuf;
	UndoBuf = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::UndoActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !UndoBuf.IsEmpty();
}

//---------------------------------------------------------------------------
//���X�g�ɖ��O��t���ĕۑ�
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::SaveListActionExecute(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("���X�g�̕ۑ�"), F_FILTER_TXT);
	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty() && !saveto_TextUTF8(fnam, ListMemo->Lines)) msgbox_ERR(USTR_FaildSave);
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::SaveListActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = ListMemo->Lines->Count>0;
}
//---------------------------------------------------------------------------
//���X�g���t�@�C������ǂݍ���
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::LoadListActionExecute(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("���X�g�̓ǂݍ���"), F_FILTER_TXT, NULL);
	UnicodeString fnam = UserModule->OpenDlgExecute();
	if (!fnam.IsEmpty()) {
		std::unique_ptr<TStringList> lst(new TStringList());
		if (load_text_ex(fnam, lst.get())!=0) {
			UndoBuf = ListMemo->Lines->Text;
			ListMemo->Lines->Assign(lst.get());
		}
		else msgbox_ERR(USTR_FileNotOpen);
	}
}
//---------------------------------------------------------------------------
//���X�g���N���A
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::ClearListItemClick(TObject *Sender)
{
	UndoBuf = ListMemo->Lines->Text;
	ListMemo->Lines->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::ListMemoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	if (contained_wd_i(KeysStr_Popup, KeyStr))
		show_PopupMenu(PopupMenu1, ListMemo);
	else if (equal_ESC(KeyStr))
		ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
//�쐬
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::CreateActionExecute(TObject *Sender)
{
	int ok_cnt	= 0;
	for (int i=0; i<ListMemo->Lines->Count; i++)
		if (!ExtractFileName(ListMemo->Lines->Strings[i]).IsEmpty()) ok_cnt++;
	if (ok_cnt==0) {
		msgbox_ERR(USTR_NoObject);
		ModalResult = mrNone;
	}
}
//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::CreateActionUpdate(TObject *Sender)
{
	int itm_cnt = ListMemo->Lines->Count;
	((TAction*)Sender)->Enabled = itm_cnt>0;

	if (itm_cnt>0) {
		UnicodeString tmp;
		StatusBar1->Panels->Items[0]->Text = tmp.cat_sprintf(_T("���ڐ�: %u"), itm_cnt);
		StatusBar1->Panels->Items[1]->Text = EmptyStr;
	}
	else {
		StatusBar1->Panels->Items[0]->Text = EmptyStr;
		StatusBar1->Panels->Items[1]->Text = "�쐬���鍀�ڂ���͂��Ă��������B";
	}
}

//---------------------------------------------------------------------------
void __fastcall TCreateDirsDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

