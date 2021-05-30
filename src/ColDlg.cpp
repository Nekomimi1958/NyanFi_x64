//----------------------------------------------------------------------//
// NyanFi																//
//  �e�L�X�g�r���A�[�̔z�F												//
//----------------------------------------------------------------------//
#pragma hdrstop
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "ColDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TColorDlg *ColorDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TColorDlg::TColorDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::FormCreate(TObject *Sender)
{
	ObjViewer = NULL;

	ColBufList = new TStringList();

	ColorListBox->Items->Text =
		"bgView=�w�i�F\n"
		"fgView=�����F\n"
		"Margin=�]����\n"
		"bgRuler=���[���̔w�i�F\n"
		"fgRuler=���[���̖ڐ��F\n"
		"Cursor=���C���J�[�\���̐F\n"
		"selItem=�I�����ڂ̔w�i�F\n"
		"fgSelItem=�I�����ڂ̕����F\n"
		"bgLineNo=�s�ԍ��w�i�F\n"
		"LineNo=�s�ԍ������F\n"
		"Mark=�s�}�[�N\n"
		"bdrLine=�s�ԍ����E��\n"
		"bdrFold=�܂�Ԃ����E��\n"
		"bdrFixed=�Œ蒷�\���̏c�r��\n"
		"Comment=|�R�����g�̕����F\n"
		"Strings=������̕����F\n"
		"Reserved=�\���̕����F\n"
		"Symbol=�V���{���̕����F\n"
		"Numeric=���l�̕����F\n"
		"fgEmpBin1=�o�C�i�����������F1\n"
		"fgEmpBin2=�o�C�i�����������F2\n"
		"fgEmpBin3=�o�C�i�����������F3\n"
		"Headline=���o���̕����F\n"
		"Ruby=���r\n"
		"URL=URL�̕����F\n"
		"LocalLink=���[�J���t�@�C���ւ̃����N\n"
		"fgEmp=���������F\n"
		"bgEmp=�����w�i�F\n"
		"TAB=�^�u�\���F\n"
		"CR=���s�\���F\n"
		"HR=���r���̐F\n"
		"Ctrl=�R���g���[���R�[�h\n"
		"Folder=�f�B���N�g���̕����F\n"
		"Error=�G���[�̕����F\n";

	SwatchPanel = new UsrSwatchPanel(this);
	SwatchPanel->Parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::FormShow(TObject *Sender)
{
	if (!ObjViewer) ObjViewer = TxtViewer;

	SwatchPanel->SetPanelSize16x8(15);
	SwatchPanel->Left = ClientWidth  - SwatchPanel->Width;
	SwatchPanel->Top  = ClientHeight - SwatchPanel->Height;

	set_ListBoxItemHi(ColorListBox);

	ColBufList->Assign(ObjViewer->ColBufList);

	SetDarkWinTheme(this, true);
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	ObjViewer = NULL;
	FileName  = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::FormDestroy(TObject *Sender)
{
	delete ColBufList;
}

//---------------------------------------------------------------------------
//�ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ColorListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	draw_ColorListBox((TListBox*)Control, Rect, Index, State, ColBufList);
}

//---------------------------------------------------------------------------
//�F�̎Q��
//---------------------------------------------------------------------------
void __fastcall TColorDlg::RefColBtnClick(TObject *Sender)
{
	TListBox *lp = ColorListBox;
	int idx = lp->ItemIndex;	if (idx==-1) return;

	UnicodeString col_nam = lp->Items->Names[idx];
	UserModule->ColorDlg->Color = (TColor)ColBufList->Values[col_nam].ToIntDef(clBlack);
	if (UserModule->ColorDlg->Execute()) {
		ColBufList->Values[col_nam] = IntToStr(UserModule->ColorDlg->Color);
		lp->Repaint();
	}
}

//---------------------------------------------------------------------------
//�X�|�C�g
//---------------------------------------------------------------------------
void __fastcall TColorDlg::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	TListBox *lp = ColorListBox;
	if (Button!=mbLeft || lp->ItemIndex==-1) return;

	//�X�E�H�b�`�u�b�N
	SetCustomColToSwatch(UserModule->ColorDlg->CustomColors);
	SwatchPanel->Visible = true;
	SwatchPanel->BringToFront();

	TColor col_def = (TColor)ColBufList->Values[lp->Items->Names[lp->ItemIndex]].ToIntDef(clNone);
	UserModule->BeginSpuit(SpuitPanel, SpuitImage, col_def);
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::SpuitImageMouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	SwatchPanel->Visible = false;

	if (UserModule->SpuitEnabled()) {
		TColor col = UserModule->EndSpuit();
		TListBox *lp = ColorListBox;
		if (lp->ItemIndex!=-1) {
			ColBufList->Values[lp->Items->Names[lp->ItemIndex]] = col;
			lp->Repaint();
		}
	}
}

//---------------------------------------------------------------------------
//�C���|�[�g
//---------------------------------------------------------------------------
void __fastcall TColorDlg::InportBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("�z�F�̃C���|�[�g"), F_FILTER_INI, _T("*.INI"));
	UnicodeString fnam = UserModule->OpenDlgExecute();
	if (!fnam.IsEmpty()) {
		std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(fnam));
		inp_file->ReadSection("Color", ColBufList);
		ColorListBox->Repaint();
	}
	SetFocus();
}
//---------------------------------------------------------------------------
//�G�N�X�|�[�g
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ExportBtnClick(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("�z�F�̃G�N�X�|�[�g"), F_FILTER_INI, FileName.c_str());
	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) {
		std::unique_ptr<UsrIniFile> exp_file(new UsrIniFile(fnam));
		exp_file->AssignSection("Color", ColBufList);
		if (!exp_file->UpdateFile(true)) msgbox_ERR(USTR_FaildSave);
	}
	SetFocus();
}
//---------------------------------------------------------------------------
//�S�̂ɔ��f
//---------------------------------------------------------------------------
void __fastcall TColorDlg::OptApplyBtnClick(TObject *Sender)
{
	if (msgbox_Sure(_T("�S�̂̃I�v�V�����ݒ�ɔ��f�����܂����H"))) {
		ObjViewer->SetColor(ColBufList);
		ObjViewer->SetOptColor();
		ObjViewer->Repaint(true);
	}
}
//---------------------------------------------------------------------------
//�K�p
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ApplyBtnClick(TObject *Sender)
{
	ObjViewer->SetColor(ColBufList);
	ObjViewer->Repaint(true);
}

//---------------------------------------------------------------------------
//�m��
//---------------------------------------------------------------------------
void __fastcall TColorDlg::OkActionExecute(TObject *Sender)
{
	ObjViewer->SetColor(ColBufList);
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::OkActionUpdate(TObject *Sender)
{
	if (!UserModule->SpuitEnabled()) SpuitImage->Visible = (ColorListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//�L�����Z��
//---------------------------------------------------------------------------
void __fastcall TColorDlg::CancelBtnClick(TObject *Sender)
{
	//�X�|�C�g�̃L�����Z��
	if (UserModule->SpuitEnabled()) {
		SwatchPanel->Visible = false;
		UserModule->EndSpuit();
		SpuitImage->Visible = true;
	}
	//����
	else {
		ModalResult = mrCancel;
	}
}

//---------------------------------------------------------------------------
//�z�F�̖�����
//---------------------------------------------------------------------------
void __fastcall TColorDlg::DisableColActionExecute(TObject *Sender)
{
	TListBox *lp = ColorListBox;
	if (lp->ItemIndex!=-1) {
		ColBufList->Values[lp->Items->Names[lp->ItemIndex]] = IntToStr(clNone);
		lp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TColorDlg::DisableColActionUpdate(TObject *Sender)
{
	TListBox *lp = ColorListBox;
	UnicodeString col_id = (lp->ItemIndex!=-1)? lp->Items->Names[lp->ItemIndex] : EmptyStr;
	((TAction*)Sender)->Enabled = contained_wd_i(_T("fgSelItem|bdrLine|bdrFold|bdrFixed"), col_id);
}

//---------------------------------------------------------------------------
//�w���v
//---------------------------------------------------------------------------
bool __fastcall TColorDlg::FormHelp(WORD Command, NativeInt Data, bool &CallHelp)
{
	if (Command==HELP_CONTEXT || Command==HELP_CONTEXTPOPUP) {
		HtmlHelpTopic(_T(HELPTOPIC_TV) _T("#SetColor"));
		CallHelp = false;
	}
	return true;
}
//---------------------------------------------------------------------------

