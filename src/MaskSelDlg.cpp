//----------------------------------------------------------------------//
// NyanFi																//
//  �}�X�N/�}�b�`�I���_�C�A���O											//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop

#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "MaskSelDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMaskSelectDlg *MaskSelectDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TMaskSelectDlg::TMaskSelectDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMaskSelectDlg::FormCreate(TObject *Sender)
{
	IsMask = false;
}
//---------------------------------------------------------------------------
void __fastcall TMaskSelectDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	set_ComboBox_AutoComp(this);

	Caption = get_word_i_idx(_T("�p�X�}�X�N|�}�X�N�I��|�}�b�`�I��"),
				idx_of_word_i(_T("InputPathMask|MaskSelect|MatchSelect"), CmdName));

	IsMask = !USAME_TI(CmdName, "MatchSelect");
	IniFile->LoadComboBoxItems(MaskSelComboBox, IsMask? _T("MaskSelHistory") : _T("MatchSelHistory"));
	MaskSelComboBox->Tag  = CBTAG_HISTORY | (!IsMask? CBTAG_RGEX_V|CBTAG_RGEX_E : 0);
	MaskSelComboBox->Hint = IsMask? "; �ŋ�؂��ĕ����w��\" : "; �ŋ�؂��ĕ����w��\�A/�`/ �͐��K�\��";
	MaskSelComboBox->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TMaskSelectDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	if (ModalResult==mrOk) add_ComboBox_history(MaskSelComboBox, MaskSelComboBox->Text);
	IniFile->SaveComboBoxItems(MaskSelComboBox, IsMask? _T("MaskSelHistory") : _T("MatchSelHistory"));
}

//---------------------------------------------------------------------------
void __fastcall TMaskSelectDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString topic = HELPTOPIC_FL;
	if (!CmdName.IsEmpty()) topic.cat_sprintf(_T("#%s"), CmdName.c_str());
	SpecialKeyProc(this, Key, Shift, topic.c_str());
}
//---------------------------------------------------------------------------
