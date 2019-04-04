//----------------------------------------------------------------------//
// �ėp���p���W���[��													//
//																		//
//----------------------------------------------------------------------//
#pragma hdrstop
#include <Vcl.Clipbrd.hpp>
#include <Vcl.FileCtrl.hpp>
#include <RegularExpressions.hpp>
#include "usr_msg.h"
#include "usr_file_ex.h"
#include "usr_swatch.h"
#include "UserFunc.h"
#include "InpCmds.h"
#include "RegExChk.h"
#include "CalcDlg.h"
#include "UserMdl.h"
#include "EditItem.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "Vcl.Controls.TControl"
#pragma resource "*.dfm"
TUserModule *UserModule;

//---------------------------------------------------------------------------
//�e�L�X�g�ۑ��R�[�h�y�[�W
code_page_inf SaveCodePages[MAX_SAVE_CODEPAGES] = {
	{  932,	"Shift_JIS"},
	{65001,	"UTF-8"},
	{65001,	"UTF-8N"},
	{ 1200,	"UTF-16"},
	{ 1201,	"UTF-16(BE)"},
	{20932,	"EUC-JP"},
	{50220,	"ISO-2022-JP"}
};

//---------------------------------------------------------------------------
__fastcall TUserModule::TUserModule(TComponent* Owner)
	: TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DataModuleCreate(TObject *Sender)
{
	//�}�E�X�|�C���^�[��ݒ�
	crHandGrabR = (TCursor)6;	//�C���[�W�ړ�
	crSpuitTool = (TCursor)7;	//�X�|�C�g
	Screen->Cursors[crHandGrabR] = (HCURSOR)::LoadImage(HInstance, _T("HNDGRB_R"),	 IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
	Screen->Cursors[crSpuitTool] = (HCURSOR)::LoadImage(HInstance, _T("SPUIT_TOOL"), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);

	SpuitPanel = NULL;
	SpuitImage = NULL;

	ActiveListBox  = NULL;
	ActiveScrPanel = NULL;

	SaveTxtDlg->Filter	   = F_FILTER_TXT;
	SaveTxtDlg->DefaultExt = "txt";
	SaveTxtDlg->Encodings->Clear();
	for (int i=0; i<MAX_SAVE_CODEPAGES; i++) SaveTxtDlg->Encodings->Add(SaveCodePages[i].name);

	//�R�}���h�p�����[�^�̎Q��
	CmdParamList = new TStringList();

	//���K�\���̎Q��
	RefRegExList = new TStringList();
	RefRegExList->Text =
		"\\	���̃��^�������N�H�[�g\n"
		"^	�s��\n"
		".	���s�ȊO�̂��ׂĂ̕���\n"
		"$	�s��\n"
		"|	�I��\n"
		"()	�O���[�v��\n"
		"[]	�����N���X\n"
		"-\n"
		"\\w	�P��̍\������(�p������\"_\")\n"
		"\\W	�P��̍\�������ȊO\n"
		"\\s	�󔒕���\n"
		"\\S	�󔒕����ȊO\n"
		"\\d	����\n"
		"\\D	�����ȊO\n"
		"\\b	�P��̋��E\n"
		"\\B	�P��̋��E�ȊO\n"
		"-\n"
		"*	0 ��ȏ�\n"
		"+	1 ��ȏ�\n"
		"?	1 ��܂��� 0 ��\n"
		"{n}	���傤�� n ��\n"
		"{n,}	n ��ȏ�\n"
		"{n,m}	n ��ȏ� m ��ȉ�\n";

	//���K�\�����j���[�̐ݒ�
	for (int i=0; i<RefRegExList->Count; i++) {
		UnicodeString lbuf = RefRegExList->Strings[i];
		if (lbuf.IsEmpty()) continue;
		TMenuItem *mp1 = new TMenuItem(RegExpItem);
		TMenuItem *mp2 = new TMenuItem(RegExpEItem);
		if (lbuf[1]=='-') {
			mp1->Caption = lbuf;
			mp2->Caption = lbuf;
		}
		else {
			UnicodeString s1 = get_pre_tab(lbuf);
			UnicodeString s2 = get_post_tab(lbuf);
			lbuf.sprintf(_T("%-6s %s"), s1.c_str(), s2.c_str());
			mp1->Caption = lbuf;
			mp2->Caption = lbuf;
			mp1->OnClick = RefRegExItemClick;
			mp2->OnClick = RefRegExItemClick;
			RegExpItem->Add(mp1);
			RegExpEItem->Add(mp2);
		}
	}

	FileListRect = Rect(0,0, 600,400);	//���ݒ�

	LastComboBox   = NULL;
	LastComboBoxSS = 0;
	LastComboBoxSL = 0;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DataModuleDestroy(TObject *Sender)
{
	delete CmdParamList;
	delete RefRegExList;
}

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̏����� (TListBox/TCheckListBox �ɑΉ�)
//  DragMode=dmAutomatic �Ȃ�h���b�O&�h���b�v�ɂ�鍀�ڈړ����\
//  ���ڂ̏㉺�ړ�����э폜�p�A�N�V�������
//---------------------------------------------------------------------------
void __fastcall TUserModule::InitializeListBox(TCustomListBox *lp, UsrScrollPanel *sp)
{
	ActiveListBox = lp;
	if (ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0) {
		if (class_is_ListBox(ActiveListBox)) {
			if (((TListBox*)ActiveListBox)->DragMode==dmAutomatic) {
				((TListBox*)ActiveListBox)->OnStartDrag = ListBoxStartDrag;
				((TListBox*)ActiveListBox)->OnDragOver	= ListBoxDragOver;
				((TListBox*)ActiveListBox)->OnDragDrop	= ListBoxDragDrop;
				((TListBox*)ActiveListBox)->OnEndDrag	= ListBoxEndDrag;
			}
		}
		else if (class_is_CheckListBox(ActiveListBox)) {
			if (((TCheckListBox*)ActiveListBox)->DragMode==dmAutomatic) {
				((TCheckListBox*)ActiveListBox)->OnStartDrag = ListBoxStartDrag;
				((TCheckListBox*)ActiveListBox)->OnDragOver  = ListBoxDragOver;
				((TCheckListBox*)ActiveListBox)->OnDragDrop  = ListBoxDragDrop;
				((TCheckListBox*)ActiveListBox)->OnEndDrag	 = ListBoxEndDrag;
			}
		}
	}
	PrevListIdx = -1;

	ActiveScrPanel = sp;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::UninitializeListBox()
{
	ScrollTimer->Enabled = false;

	if (ActiveListBox) {
		if (class_is_ListBox(ActiveListBox)) {
			((TListBox*)ActiveListBox)->OnStartDrag = NULL;
			((TListBox*)ActiveListBox)->OnDragOver  = NULL;
			((TListBox*)ActiveListBox)->OnDragDrop  = NULL;
			((TListBox*)ActiveListBox)->OnEndDrag	= NULL;
		}
		else if (class_is_CheckListBox(ActiveListBox)) {
			((TCheckListBox*)ActiveListBox)->OnStartDrag = NULL;
			((TCheckListBox*)ActiveListBox)->OnDragOver  = NULL;
			((TCheckListBox*)ActiveListBox)->OnDragDrop  = NULL;
			((TCheckListBox*)ActiveListBox)->OnEndDrag	 = NULL;
		}
		ActiveListBox  = NULL;
		ActiveScrPanel = NULL;
	}
}

//---------------------------------------------------------------------------
//���X�g���ڂ�����
//---------------------------------------------------------------------------
void __fastcall TUserModule::UpListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		int idx = ActiveListBox->ItemIndex;
		if (idx>=1 && idx<ActiveListBox->Count) {
			ActiveListBox->Items->Move(idx, idx - 1);
			ActiveListBox->ItemIndex = idx - 1;
		}
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::UpListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex>=1 && ActiveListBox->ItemIndex<ActiveListBox->Count;

}
//---------------------------------------------------------------------------
//���X�g���ڂ������
//---------------------------------------------------------------------------
void __fastcall TUserModule::DownListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		int idx = ActiveListBox->ItemIndex;
		if (idx>=0 && idx<ActiveListBox->Count-1) {
			ActiveListBox->Items->Move(idx, idx + 1);
			ActiveListBox->ItemIndex = idx + 1;
		}
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DownListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex>=0 && ActiveListBox->ItemIndex<ActiveListBox->Count-1;
}
//---------------------------------------------------------------------------
//���X�g���ڂ��폜
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		delete_ListItem(ActiveListBox);
		if (ActiveListBox->Tag & LBTAG_OPT_FOCS) ActiveListBox->SetFocus();
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->ItemIndex!=-1;
}
//---------------------------------------------------------------------------
//���X�g���ڂ��N���A
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrListItemActionExecute(TObject *Sender)
{
	if (ActiveListBox) {
		ActiveListBox->Clear();
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrListItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled
		= ActiveListBox && (ActiveListBox->Tag & LBTAG_OPT_INHI)==0
			&& ActiveListBox->Count>0;
}

//---------------------------------------------------------------------------
//�h���b�O&�h���b�v�ɂ�郊�X�g���ڂ̈ړ�
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxStartDrag(TObject *Sender, TDragObject *&DragObject)
{
	PrevListIdx = -1;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y,
		TDragState State, bool &Accept)
{
	ScrollTimer->Enabled = false;

	TCustomListBox *lp = (TCustomListBox*)Sender;
	Accept = (Source == Sender) && (lp->ItemIndex!=-1);

	int ch = lp->ClientHeight;
	int ih = (class_is_CheckListBox(lp)? ((TCheckListBox*)lp)->ItemHeight : ((TListBox*)lp)->ItemHeight);
	if (lp->TopIndex>0 && Y<(ih - 4)) {
		//��փX�N���[��
		ScrollTimer->Interval = 100;
		ScrollTimer->Tag	  = (Y<=0)? -2 : -1;
		ScrollTimer->Enabled  = true;
	}
	else if (Y>(ch - (ih - 4))) {
		//���փX�N���[��
		ScrollTimer->Interval = 100; 
		ScrollTimer->Tag	  = (Y>=ch)? 2 : 1;
		ScrollTimer->Enabled  = true;
	}
	else if (Accept) {
		int idx = lp->ItemAtPos(Point(X, Y), true);
		draw_ListItemLine(lp, PrevListIdx);		//���O�̃h���b�v��\��������
		draw_ListItemLine(lp, idx);				//�V�����h���b�v��\����`��
		PrevListIdx = idx;
	}

	if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y)
{
	ScrollTimer->Enabled  = false;

	TCustomListBox *lp = (TCustomListBox*)Sender;
	int idx = lp->ItemAtPos(Point(X, Y), true);
	draw_ListItemLine(lp, PrevListIdx);		//���O�̃h���b�v��\��������
	if (idx!=-1) {
		lp->Items->Move(lp->ItemIndex, idx);
		lp->ItemIndex = idx;
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ListBoxEndDrag(TObject *Sender, TObject *Target, int X, int Y)
{
	ScrollTimer->Enabled  = false;
}
//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̎����X�N���[��
//---------------------------------------------------------------------------
void __fastcall TUserModule::ScrollTimerTimer(TObject *Sender)
{
	if (ActiveListBox) {
		int last_top = ActiveListBox->TopIndex;
		ActiveListBox->TopIndex = ActiveListBox->TopIndex + ((TComponent*)Sender)->Tag;
		if (ActiveListBox->TopIndex!=last_top) {
			draw_ListItemLine(ActiveListBox, PrevListIdx);		//���O�̃h���b�v��\��������
			PrevListIdx = -1;
			ActiveListBox->Invalidate();
		}
		else ScrollTimer->Enabled = false;
		if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	}
}

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̍��ڂɑ΂���L�[���� (�ړ��A�폜)
//---------------------------------------------------------------------------
bool __fastcall TUserModule::ListBoxOpeItem(UnicodeString keystr)
{ 
	switch (idx_of_word_i(_T("Shift+Ctrl+DOWN|Shift+Ctrl+UP|DEL"), keystr)) {
	case  0: DownListItemAction->Execute();	break;
	case  1: UpListItemAction->Execute();	break;
	case  2: DelListItemAction->Execute();	break;
	default: 								return false;
	}

	if (ActiveScrPanel) ActiveScrPanel->UpdateKnob();
	return true;
}

//---------------------------------------------------------------------------
//�}�E�X�ʒu�̃s�N�Z���F���擾����X�|�C�g�@�\
//---------------------------------------------------------------------------
//�J�n
//---------------------------------------------------------------------------
void __fastcall TUserModule::BeginSpuit(
	TPanel *pp,			//�X�|�C�g�p�l��
	TImage *ip,			//�X�|�C�g�C���[�W
	TColor col_def)		//�f�t�H���g�F
{
	SpuitPanel = pp;

	SpuitImage = ip;
	if (SpuitImage) SpuitImage->Visible = false;

	SpuitDefColor = col_def;
	if (col_def!=clNone) pp->Color = col_def;

	Screen->Cursor = crSpuitTool;
	SpuitTimer->Enabled = true;
}
//---------------------------------------------------------------------------
//�I��
//---------------------------------------------------------------------------
TColor __fastcall TUserModule::EndSpuit()
{
	cursor_Default();
	SpuitTimer->Enabled = false;

	TColor col = (SpuitPanel && FindControl(get_window_from_pos())!=SpuitPanel)? SpuitPanel->Color : SpuitDefColor;
	SpuitPanel->Color = scl_BtnFace;
	SpuitPanel = NULL;

	if (SpuitImage) SpuitImage->Visible = true;
	SpuitImage = NULL;

	return col;
}
//---------------------------------------------------------------------------
//�X�|�C�g���쒆?
//---------------------------------------------------------------------------
bool __fastcall TUserModule::SpuitEnabled()
{
	return SpuitTimer->Enabled;
}
//---------------------------------------------------------------------------
//�X�|�C�g�Ń}�E�X�ʒu�̃s�N�Z���F���擾
//---------------------------------------------------------------------------
void __fastcall TUserModule::SpuitTimerTimer(TObject *Sender)
{
	if (SpuitPanel && FindControl(get_window_from_pos())!=SpuitPanel) {
		HDC hDc = ::GetDC(0);
		if (hDc) {
			COLORREF c = ::GetPixel(hDc, Mouse->CursorPos.x, Mouse->CursorPos.y);
			::ReleaseDC(0, hDc);
			SpuitPanel->Color = (TColor)c;
		}
	}
}

//---------------------------------------------------------------------------
//�t�@�C���ۑ�/�J���_�C�A���O������Ƃ��̏���
//---------------------------------------------------------------------------
void __fastcall TUserModule::FileDlgClose(TObject *Sender)
{
	CloseIME(Application->ActiveFormHandle);

	SaveTxtDlg->Filter	   = F_FILTER_TXT;
	SaveTxtDlg->DefaultExt = "txt";
}

//---------------------------------------------------------------------------
void __fastcall TUserModule::ComboBoxContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled)
{
	TComboBox *bp = dynamic_cast<TComboBox *>(Sender);
	if (bp && !bp->Focused()) {
		bp->SetFocus();
		Application->ProcessMessages();

		TMenuItem *mp = bp->PopupMenu? bp->PopupMenu->Items : NULL;
		if (mp) {
			for (int i=0; i<mp->Count; i++) {
				TMenuItem *ip = mp->Items[i];
				if (ip->Action) ip->Action->Update();
			}
		}
	}
}

//---------------------------------------------------------------------------
//�G�f�B�b�g/�R���{�{�b�N�X�ɓƎ��|�b�v�A�b�v���j���[��ݒ�
//---------------------------------------------------------------------------
void __fastcall TUserModule::SetUsrPopupMenu(TForm *fp)
{
	//���͗��Ƀ|�b�v�A�b�v���j���[��ݒ�
	for (int i=0; i<fp->ComponentCount; i++) {
		TComponent *cp = fp->Components[i];
		if (class_is_Edit(cp)) {
			((TEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_LabeledEdit(cp)) {
			((TLabeledEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_MaskEdit(cp)) {
			((TMaskEdit*)cp)->PopupMenu = EditPopupMenuE;
		}
		else if (class_is_ComboBox(cp)) {
			TComboBox *bp = (TComboBox*)cp;
			if (bp->Style==csDropDown) bp->PopupMenu = EditPopupMenuC;
			if (bp->PopupMenu) bp->OnContextPopup = ComboBoxContextPopup;
		}
	}
}

//---------------------------------------------------------------------------
//�G�f�B�b�g/�R���{�{�b�N�X�̃|�b�v�A�b�v���j���[��\��
//---------------------------------------------------------------------------
bool __fastcall TUserModule::ShowPopupMenu()
{
	TWinControl *wp = Screen->ActiveControl;	if (!wp) return false;

	TPopupMenu  *mp = class_is_Edit(wp)? 		((TEdit*)wp)->PopupMenu :
					  class_is_LabeledEdit(wp)?	((TLabeledEdit*)wp)->PopupMenu :
					  class_is_ComboBox(wp)?	((TComboBox*)wp)->PopupMenu : NULL;
	if (!mp) return false;

	TPoint p = wp->ClientToScreen(Point(0, wp->ClientHeight));
	if ((p.y + ::GetSystemMetrics(SM_CYMENU)*mp->Items->Count)>Screen->Height) p.y -= wp->ClientHeight;
	mp->Popup(p.x, p.y);
	return true;
}

//---------------------------------------------------------------------------
//�t�H�[�J�X�̂���R���{�{�b�N�X���擾
//---------------------------------------------------------------------------
TComboBox * __fastcall TUserModule::GetActiveComboBox()
{
	TWinControl *cp = Screen->ActiveControl;
	return class_is_ComboBox(cp)? (TComboBox*)cp : NULL;
}
//---------------------------------------------------------------------------
//�t�H�[�J�X�̂���J�X�^���G�f�B�b�g���擾
//---------------------------------------------------------------------------
TCustomEdit * __fastcall TUserModule::GetActiveCustomEdit()
{
	TWinControl *cp = Screen->ActiveControl;
	return class_is_CustomEdit(cp)? (TCustomEdit *)cp : NULL;
}

//---------------------------------------------------------------------------
//�؂���
//---------------------------------------------------------------------------
void __fastcall TUserModule::CutComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->Perform(WM_CUT, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditComboActionUpdate(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	((TAction*)Sender)->Enabled = (cp && !cp->SelText.IsEmpty());
}
//---------------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------------
void __fastcall TUserModule::CopyComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->Perform(WM_COPY, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
//�\��t��
//---------------------------------------------------------------------------
void __fastcall TUserModule::PasteComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelText = get_norm_str(Clipboard()->AsText);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::PasteComboActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (Clipboard()->HasFormat(CF_TEXT) && GetActiveComboBox());
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPaste1Execute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;	if (!wp) return;

	UnicodeString s = get_norm_str(Clipboard()->AsText);
	if (USAME_TI(wp->Name, "DistrDirEdit") && dir_exists(s)) s = REPLACE_TS(s, "\\", "\\\\");

	if (class_is_MaskEdit(wp)) {
		TMaskEdit *ep = (TMaskEdit*)wp;
		if (ep->SelText.Length()==ep->EditText.Length() || ep->SelText.Length()==0) {
			TDateTime dt;
			if (str_to_DateTime(s, &dt)) {
				//�^�C���X�^���v
				if (USAME_TI(ep->Name, "TimeMaskEdit"))
					s = format_DateTime(dt);
				//���t
				else if(USAME_TI(ep->Name, "DateMaskEdit"))
					s = format_Date(dt);
				ep->Text	  = s;
				ep->SelStart  = 0;
				ep->SelLength = ep->Text.Length();
			}
			else {
				msgbox_ERR(USTR_IllegalDate);
				return;
			}
		}
		else
			ep->SelText = s;
	}
	else if (class_is_CustomEdit(wp)) {
		((TCustomEdit *)wp)->SelText = s;
	}
}

//---------------------------------------------------------------------------
//���ׂđI��
//---------------------------------------------------------------------------
void __fastcall TUserModule::SelAllComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelectAll();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::SelAllComboActionUpdate(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	((TAction*)Sender)->Enabled = (cp && !cp->Text.IsEmpty());
}

//---------------------------------------------------------------------------
//���̌���������𗚗�����폜
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelHistComboActionExecute(TObject *Sender)
{
	del_ComboBox_history(GetActiveComboBox());
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::DelHistComboActionUpdate(TObject *Sender)
{
	TAction   *ap = (TAction*)Sender;
	TComboBox *cp = GetActiveComboBox();
	ap->Visible = (cp && (cp->Tag & CBTAG_HISTORY));
	ap->Enabled = (ap->Visible && cp && cp->Items->Count>0 && !cp->Text.IsEmpty());
}
//---------------------------------------------------------------------------
//�������������ׂč폜
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrHistComboActionExecute(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp && msgbox_Y_N_C(LoadUsrMsg(USTR_DelHistoryQ))==IDYES) cp->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::ClrHistComboActionUpdate(TObject *Sender)
{
	TAction   *ap = (TAction*)Sender;
	TComboBox *cp = GetActiveComboBox();
	ap->Visible = (cp && (cp->Tag & CBTAG_HISTORY));
	ap->Enabled = (ap->Visible && cp && cp->Items->Count>0);
}

//---------------------------------------------------------------------------
//�R�}���h���̎Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCmdNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;

	if (class_is_CustomEdit(wp)) {
		InpCmdsDlg->toRefer = true;
		if (InpCmdsDlg->ShowModal()==mrOk)
			((TCustomEdit*)wp)->SelText = InpCmdsDlg->CmdsComboBox->Text;
	}
	else if (class_is_ComboBox(wp)) {
		//�t�H�[�J�X���߂����Ƃ��S�I���ɂȂ��Ă��܂����ۂւ̑΍�
		TComboBox *cp = (TComboBox*)wp;
		int s_start = cp->SelStart;
		int s_len	= cp->SelLength;
		InpCmdsDlg->toRefer = true;

		InpCmdsDlg->ModeTabControl->TabIndex
			= contains_s(wp->Name, _T('V'))? 1 :
			  contains_s(wp->Name, _T('I'))? 2 :
				   (UserModule->ScrMode==2)? 1 :
				   (UserModule->ScrMode==4)? 2 : 0;

		if (InpCmdsDlg->ShowModal()==mrOk) {
			cp->SelStart  = s_start;
			cp->SelLength = s_len;
			cp->SelText   = InpCmdsDlg->CmdsComboBox->Text;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCmdNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp &&
		 (  str_match("WheelBtn*ComboBox", wp->Name)
		 || str_match("WheelCmd*ComboBox", wp->Name)
		 || str_match("X?Btn?ComboBox",	   wp->Name)
		 || contained_wd_i(_T("MenuPrmEdit|EventCmdsEdit|TimerCmd1Edit|TimerCmd2Edit|NewExeCmdEdit|BtnCmdsComboBox"), wp->Name)));
}

//---------------------------------------------------------------------------
//�J�[�\���ʒu�̃t�@�C�������Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurFNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_CustomEdit(wp))
		((TCustomEdit *)wp)->Text = ExtractFileName(CurFileName);
	else if (class_is_ComboBox(wp))
		((TComboBox*)wp)->SelText = ExtractFileName(CurFileName);
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurFNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrMaskEdit|ExeComboBox"), wp->Name));
}
//---------------------------------------------------------------------------
//�J�[�\���ʒu�̃p�X�t���t�@�C�������Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPNameActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_CustomEdit(wp))
		((TCustomEdit *)wp)->SelText = CurFileName;
	else if (class_is_ComboBox(wp))
		((TComboBox*)wp)->SelText	 = CurFileName;
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPNameActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && USAME_TI(wp->Name, "ExeComboBox"));
}

//---------------------------------------------------------------------------
//�J�����g�p�X���Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPathActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_LabeledEdit(wp))
		((TLabeledEdit*)wp)->Text = REPLACE_TS(ExcludeTrailingPathDelimiter(CurPathName), "\\", "\\\\");
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefCurPathActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrDirEdit"), wp->Name));
}
//---------------------------------------------------------------------------
//���΃p�X���Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefOppPathActionExecute(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	if (class_is_LabeledEdit(wp))
		((TLabeledEdit*)wp)->Text = REPLACE_TS(ExcludeTrailingPathDelimiter(OppPathName), "\\", "\\\\");
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefOppPathActionUpdate(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	((TAction*)Sender)->Visible = (wp && contained_wd_i(_T("DistrDirEdit"), wp->Name));
}

//---------------------------------------------------------------------------
//�R���{�{�b�N�X�̃|�b�v�A�b�v���j���[
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPopupMenuCPopup(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	RegExpItem->Visible = (cp && (cp->Tag & CBTAG_RGEX_V));
	RegExpItem->Enabled = (cp && (cp->Tag & CBTAG_RGEX_E));

	EditItemItem->Visible = (cp && (StartsStr(". �ŋ�؂���", cp->Hint) || StartsStr("; �ŋ�؂���", cp->Hint)));
	EditItemItem->Enabled = EditItemItem->Visible;

	//�R�}���h�p�����[�^�̎Q��
	CmdParamItem->Visible = (cp && SameText(cp->Name, "PrmComboBox") && CmdParamList->Count>0);
	if (CmdParamItem->Visible) {
		CmdParamItem->Clear();
		for (int i=0; i<CmdParamList->Count; i++) {
			UnicodeString lbuf = CmdParamList->Strings[i];
			if (lbuf.IsEmpty()) continue;
			TMenuItem *mp = new TMenuItem(CmdParamItem);
			mp->Caption   = lbuf;
			mp->OnClick   = CmdParamItemClick;
			CmdParamItem->Add(mp);
		}
	}

	reduction_MenuLine(EditPopupMenuC->Items);
}
//---------------------------------------------------------------------------
//�G�f�B�b�g�̃|�b�v�A�b�v���j���[
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditPopupMenuEPopup(TObject *Sender)
{
	TWinControl *wp = Screen->ActiveControl;
	int tag = (class_is_LabeledEdit(wp) && !USAME_TS(Screen->ActiveForm->Name, "OptionDlg"))? wp->Tag : 0;

	PopRefFmtItem->Visible	 = (tag & EDTAG_DST_FMT);
	PopRefFmtItem->Enabled	 = PopRefFmtItem->Visible;

	RegExpEItem->Visible	 = (tag & EDTAG_RGEX_V);
	RegExpEItem->Enabled	 = (tag & EDTAG_RGEX_E);

	bool dd_sw = wp && USAME_TI(wp->Name, "DirDelimiterEdit");
	PopRefDlmtItem->Visible  = dd_sw;
	PopRefDlmtItem->Enabled  = dd_sw;

	PopEditItemItem->Visible = wp && (StartsStr(". �ŋ�؂���", wp->Hint) || StartsStr("; �ŋ�؂���", wp->Hint));
	PopEditItemItem->Enabled = PopEditItemItem->Visible;

	reduction_MenuLine(EditPopupMenuE->Items);
}

//---------------------------------------------------------------------------
//�R�}���h�p�����[�^�̎Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::CmdParamItemClick(TObject *Sender)
{
	UnicodeString lbuf = get_tkn(((TMenuItem*)Sender)->Caption, _T(" : "));
	TComboBox *cp = GetActiveComboBox();
	if (cp) cp->SelText = lbuf;
}
//---------------------------------------------------------------------------
//���K�\���̎Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::RefRegExItemClick(TObject *Sender)
{
	UnicodeString lbuf = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		cp->SelText = lbuf;
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->SelText = lbuf;
}
//---------------------------------------------------------------------------
//�f�B���N�g����؂�̎Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopDlmtItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->Text = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
}
//---------------------------------------------------------------------------
//�U������������̎Q��
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopDFmtItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) ep->SelText = get_tkn(((TMenuItem*)Sender)->Caption, ' ');
}

//---------------------------------------------------------------------------
//�ݒ荀�ڂ̕ҏW
//---------------------------------------------------------------------------
void __fastcall TUserModule::PopEditItemItemClick(TObject *Sender)
{
	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		if (!EditItemDlg) EditItemDlg = new TEditItemDlg(this);	//����ɓ��I�쐬
		EditItemDlg->AssignItems(ep);
		if (EditItemDlg->ShowModal()==mrOk && !EditItemDlg->RetStr.IsEmpty()) {
			ep->Text = EditItemDlg->RetStr;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::EditItemItemClick(TObject *Sender)
{
	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		if (!EditItemDlg) EditItemDlg = new TEditItemDlg(this);	//����ɓ��I�쐬
		EditItemDlg->AssignItems(cp);
		if (EditItemDlg->ShowModal()==mrOk && !EditItemDlg->RetStr.IsEmpty()) {
			cp->Text = EditItemDlg->RetStr;
		}
	}
}

//---------------------------------------------------------------------------
//�摜�t�B���^��ݒ�
//---------------------------------------------------------------------------
void __fastcall TUserModule::SetOpenImgFilter(UnicodeString fnam)
{
	UnicodeString flt_str;
	flt_str.USET_T("�r�b�g�}�b�v (*.bmp)|*.BMP");
	flt_str.UCAT_T("|JPEG�t�@�C��(*.jpg)|*.JPG;*.JPE;*.JPEG");
	flt_str.UCAT_T("|PNG�t�@�C��(*.png)|*.PNG");
	flt_str.UCAT_T("|GIF�t�@�C��(*.gif)|*.GIF");
	OpenImgDlg->Filter = flt_str;

	UnicodeString ext = ExtractFileExt(fnam);
	if (!ext.IsEmpty()) {
		OpenImgDlg->FilterIndex =
			test_FileExt(ext, _T(".bmp"))? 			1 :
			test_FileExt(ext, _T(".jpg.jpe.jpeg"))?	2 :
			test_FileExt(ext, _T(".png"))?			3 :
			test_FileExt(ext, _T(".gif"))?			4 : 1;
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���̑I��
//---------------------------------------------------------------------------
bool __fastcall TUserModule::SelectDirEx(const _TCHAR *tit, UnicodeString &dnam,
	bool to_rel)	//���s�f�B���N�g������̑��Ύw��ɕϊ� (default = false);
{
	if (!SelectDirectory(tit, EmptyStr, dnam, TSelectDirExtOpts() << sdNewUI << sdShowShares, Screen->ActiveForm)) return false;
	if (!dir_exists(dnam)) {
		msgbox_WARN(USTR_DirNotFound);
		return false;
	}

	if (to_rel) {
		remove_top_text(dnam, ExePath);
		if (!dnam.IsEmpty()) dnam = IncludeTrailingPathDelimiter(dnam);
	}

	return true;
}

//---------------------------------------------------------------------------
//�t�B���^����f�t�H���g�̊g���q���擾
//---------------------------------------------------------------------------
UnicodeString __fastcall TUserModule::get_DlgDefExt(UnicodeString filter)
{
	UnicodeString fext;

	TStringDynArray f_buf = SplitString(filter, "|");
	if (f_buf.Length>=2) {
		fext = ExtractFileExt(get_tkn(f_buf[1], ';')).LowerCase();
		fext = test_FileExt(fext, _T(".txt.ini.csv.nwl.wav.zip"))? get_tkn_r(fext, '.') : EmptyStr;
	}

	return fext;
}

//---------------------------------------------------------------------------
//�J���_�C�A���O�̏���
//---------------------------------------------------------------------------
void __fastcall TUserModule::PrepareOpenDlg(
	const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam, UnicodeString inidir)
{
	OpenDlg->Title		= tit;
	OpenDlg->Filter 	= filter? filter : null_TCHAR;
	OpenDlg->DefaultExt = get_DlgDefExt(OpenDlg->Filter);
	OpenDlg->FileName	= fnam? fnam : null_TCHAR;
	OpenDlg->InitialDir = def_if_empty(inidir, ExePath);
}
//---------------------------------------------------------------------------
//�ۑ��_�C�A���O�̏���
//---------------------------------------------------------------------------
void __fastcall TUserModule::PrepareSaveDlg(
	const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam, UnicodeString inidir)
{
	SaveDlg->Title		= tit;
	SaveDlg->Filter 	= filter? filter : null_TCHAR;
	SaveDlg->DefaultExt = get_DlgDefExt(SaveDlg->Filter);
	SaveDlg->FileName	= fnam? fnam : null_TCHAR;
	SaveDlg->InitialDir = def_if_empty(inidir, ExePath);
}

//---------------------------------------------------------------------------
//�J���_�C�A���O�̌��ʂ�ݒ�
//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenDlgToEdit(
	TWinControl *ep,
	bool to_rel)	//���s�f�B���N�g������̑��Ύw��ɕϊ� (default = false);
{
	if (!OpenDlg->Execute()) return false;

	UnicodeString fnam = OpenDlg->FileName;
	if (to_rel) remove_top_text(fnam, ExePath);

	if (ep->InheritsFrom(__classid(TCustomEdit))) {
		((TCustomEdit *)ep)->Text = fnam;
		return true;
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenImgDlgToEdit(TWinControl *ep, bool to_rel)
{
	if (!OpenImgDlg->Execute()) return false;

	UnicodeString fnam = OpenImgDlg->FileName;
	if (to_rel) remove_top_text(fnam, ExePath);

	if (ep->InheritsFrom(__classid(TCustomEdit))) {
		((TCustomEdit *)ep)->Text = fnam;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool __fastcall TUserModule::OpenDlgToStr(UnicodeString &s, bool to_rel)
{
	if (!OpenDlg->Execute()) return false;

	s = OpenDlg->FileName;
	if (to_rel) remove_top_text(s, ExePath);
	return true;
}

//---------------------------------------------------------------------------
//�t�H���g�_�C�A���O�̌��ʂ��t�H���g�ɐݒ�
//---------------------------------------------------------------------------
bool __fastcall TUserModule::FontDlgToFont(TFont *f)
{
	FontDlg->Font->Assign(f);
	if (!UserModule->FontDlg->Execute()) return false;

	f->Assign(FontDlg->Font);
	return true;
}

//---------------------------------------------------------------------------
//���K�\���`�F�b�J�[
//---------------------------------------------------------------------------
void __fastcall TUserModule::RegExCheckerActionExecute(TObject *Sender)
{
	if (!RegExChecker) RegExChecker = new TRegExChecker(this);	//����ɓ��I�쐬

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		RegExChecker->ObjComboBox = cp;
		RegExChecker->PatternStr  = cp->Text;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		RegExChecker->ObjCustomEdit = ep;
		RegExChecker->PatternStr	= ep->Text;
	}

	if (!RegExChecker->Visible) RegExChecker->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::RegExCheckerActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		ap->Visible = (cp && (cp->Tag & CBTAG_RGEX_V));
		ap->Enabled = (cp && (cp->Tag & CBTAG_RGEX_E));
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		ap->Visible = (ep && (ep->Tag & EDTAG_RGEX_V));
		ap->Enabled = (ep && (ep->Tag & EDTAG_RGEX_E));
		return;
	}

	ap->Visible = false;
}

//---------------------------------------------------------------------------
//�d��
//---------------------------------------------------------------------------
void __fastcall TUserModule::CalculatorActionExecute(TObject *Sender)
{
	if (!Calculator) Calculator = new TCalculator(this);	//����ɓ��I�쐬

	Calculator->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TUserModule::CalculatorActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;

	TComboBox *cp = GetActiveComboBox();
	if (cp) {
		ap->Visible = (cp && (cp->Tag & CBTAG_NO_CALC)==0);
		return;
	}

	TCustomEdit *ep = GetActiveCustomEdit();
	if (ep) {
		ap->Visible = (ep && (ep->Tag & EDTAG_NO_CALC)==0);
		return;
	}

	ap->Visible = false;
}

//---------------------------------------------------------------------------
//�A�N�e�B�u�ȃt�H�[���̃T�C�Y�ƈʒu���t�@�C�����X�g�ɍ��킹��
//---------------------------------------------------------------------------
void __fastcall TUserModule::SizePosToFlieListActionExecute(TObject *Sender)
{
	TForm *frm = Screen->ActiveForm;
	if (frm && (frm->BorderStyle==bsSizeable || frm->BorderStyle==bsSizeToolWin)) {
		set_window_pos_ex(frm->Handle, FileListRect);
	}
}

//---------------------------------------------------------------------------
//�R���{�{�b�N�X�̑I����Ԃ�ۑ�
//---------------------------------------------------------------------------
void __fastcall TUserModule::SaveLastComboBox()
{
	if (Application->Active) {
		LastComboBox   = GetActiveComboBox();
		LastComboBoxSS = LastComboBox? LastComboBox->SelStart  : 0;
		LastComboBoxSL = LastComboBox? LastComboBox->SelLength : 0;
	}
}
//---------------------------------------------------------------------------
//�R���{�{�b�N�X�̑I����Ԃ𕜋A
//---------------------------------------------------------------------------
void __fastcall TUserModule::RestoreLastComboBox()
{
	if (Application->Active) {
		TComboBox *cp = GetActiveComboBox();
		if (cp && cp==LastComboBox) {
			LastComboBox->SelStart	= LastComboBoxSS;
			LastComboBox->SelLength = LastComboBoxSL;
		}
		LastComboBox = NULL;
	}
}
//---------------------------------------------------------------------------

