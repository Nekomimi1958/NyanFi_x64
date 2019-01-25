//----------------------------------------------------------------------//
// NyanFi																//
//  文字列検索(テキストビュアー)										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "TxtViewer.h"
#include "MainFrm.h"
#include "GenInfDlg.h"
#include "ExTViewer.h"
#include "FindTxtDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindTextDlg *FindTextDlg;

//---------------------------------------------------------------------------
__fastcall TFindTextDlg::TFindTextDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	FindComboBox->Tag = CBTAG_HISTORY;

	set_ComboBoxText(CodePageComboBox,
		_T("Shift_JIS\n")
		_T("ISO-8859-1\n")
		_T("UTF-16\n")
		_T("UTF-16BE\n")
		_T("US-ASCII\n")
		_T("EUC-JP\n")
		_T("UTF-7\n")
		_T("UTF-8\n"));

	fromTV = false;
}

//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	DlgInitialized = false;

	Viewer = ExTViewer? ExTViewer->ExViewer : fromTV? TxtViewer : NULL;

	IniFile->LoadPosInfo(this, DialogCenter, (ExTViewer? _T("ExFindText"): null_TCHAR));

	CaseCheckBox->Checked  = IniFile->ReadBoolGen(_T("FindTxtDlgCase"));
	RegExCheckBox->Checked = IniFile->ReadBoolGen(_T("FindTxtDlgRegEx"));
	set_MigemoCheckBox(MigemoCheckBox, _T("FindTxtDlgMigemo"));

	IniFile->LoadComboBoxItems(FindComboBox, RegExCheckBox->Checked? _T("FindPtnHistory") : _T("FindTxtHistory"));

	if (Viewer) {
		FindComboBox->Text = Viewer->get_SelText();
		if (FindComboBox->Text.IsEmpty()) FindComboBox->Text = Viewer->FindWord;
	}
	else
		FindComboBox->Text = GeneralInfoDlg->FindWord;

	UpDownGroup->ItemIndex		= IniFile->ReadIntGen(_T("FindTxtDlgUpDown"),	1);

	BytesCheckBox->Checked		= IniFile->ReadBoolGen(_T("FindTxtDlgBytes"));
	HighlightCheckBox->Checked	= IniFile->ReadBoolGen(_T("FindTxtDlgHighlight"));
	HighlightCheckBox->Visible	= (Viewer!=NULL);
	CloseCheckBox->Checked		= IniFile->ReadBoolGen(_T("FindTxtDlgClose"));
	CodePageComboBox->ItemIndex = IniFile->ReadIntGen(_T("FindTxtDlgCodePage"));

	DlgInitialized = true;
	FindOptChangedClick(NULL);
	if (Viewer) Viewer->Repaint(true);
}
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (Viewer) Viewer->Highlight = HighlightCheckBox->Checked;

	IniFile->SavePosInfo(this, (ExTViewer? _T("ExFindText"): null_TCHAR));

	IniFile->SaveComboBoxItems(FindComboBox, RegExCheckBox->Checked? _T("FindPtnHistory") : _T("FindTxtHistory"));

	IniFile->WriteIntGen(_T("FindTxtDlgUpDown"),		UpDownGroup);
	IniFile->WriteBoolGen(_T("FindTxtDlgCase"),			CaseCheckBox);
	IniFile->WriteBoolGen(_T("FindTxtDlgRegEx"),		RegExCheckBox);
	IniFile->WriteBoolGen(_T("FindTxtDlgMigemo"),		MigemoCheckBox);
	IniFile->WriteBoolGen(_T("FindTxtDlgBytes"),		BytesCheckBox);
	IniFile->WriteBoolGen(_T("FindTxtDlgHighlight"),	HighlightCheckBox);
	IniFile->WriteBoolGen(_T("FindTxtDlgClose"),		CloseCheckBox);
	IniFile->WriteIntGen(_T("FindTxtDlgCodePage"),		CodePageComboBox);

	fromTV	  = false;
	ExTViewer = NULL;
}

//---------------------------------------------------------------------------
//オプションの変更
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindOptChangedClick(TObject *Sender)
{
	if (!DlgInitialized) return;

	BytesCheckBox->Visible	   = Viewer && Viewer->isBinary;
	CodePageComboBox->Visible  = Viewer && Viewer->isBinary;

	bool not_bytes = !BytesCheckBox->Visible || !BytesCheckBox->Checked;
	set_FormTitle(this, not_bytes? _T("文字列検索") : _T("バイト列検索"));
	HighlightCheckBox->Enabled = not_bytes;
	if (Viewer) Viewer->Highlight = not_bytes;
	CodePageComboBox->Enabled  = !not_bytes;

	FindComboBox->SetFocus();
	FindComboBoxChange(FindComboBox);
}

//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::MigemoCheckBoxClick(TObject *Sender)
{
	if (MigemoCheckBox->Enabled && MigemoCheckBox->Checked) {
		RegExCheckBox->Checked = false;
		RegExCheckBox->Enabled = false;
	}
	else {
		RegExCheckBox->Enabled = !BytesCheckBox->Visible || !BytesCheckBox->Checked;
	}

	FindComboBox->SetFocus();
	FindComboBoxChange(FindComboBox);
}
//---------------------------------------------------------------------------
//検索履歴の入れ換え
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::RegExCheckBoxClick(TObject *Sender)
{
	if (DlgInitialized) {
		change_ComboBoxHistory(FindComboBox, _T("FindTxtHistory"), _T("FindPtnHistory"), RegExCheckBox->Checked);
		FindComboBox->SetFocus();
		FindComboBoxChange(FindComboBox);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::SubOptClick(TObject *Sender)
{
	FindComboBox->SetFocus();
}

//---------------------------------------------------------------------------
//検索文字列欄でのキー操作
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	switch (idx_of_word_i(_T("Ctrl+N|Ctrl+D|Ctrl+P|Ctrl+U|Ctrl+M|Ctrl+R|Ctrl+B"), get_KeyStr(Key, Shift))) {
	case 0: case 1:
		UpDownGroup->ItemIndex = 1;  Repaint();  FindNextAction->Execute();
		break;
	case 2: case 3:
		UpDownGroup->ItemIndex = 0;  Repaint();  FindNextAction->Execute();
		break;
	case 4:
		if (MigemoCheckBox->Enabled) MigemoCheckBox->Checked = !MigemoCheckBox->Checked;
		break;
	case 5:
		RegExCheckBox->Checked = !RegExCheckBox->Checked;
		break;
	case 6:
		if (BytesCheckBox->Visible) BytesCheckBox->Checked = !BytesCheckBox->Checked;
		break;
	default:
		return;
	}

	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindComboBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCVX(Key)) Key = 0;
}

//---------------------------------------------------------------------------
//検索文字列が変化したらビュアーの表示を更新
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindComboBoxChange(TObject *Sender)
{
	if (Viewer) {
		Viewer->isCase		= CaseCheckBox->Checked;
		Viewer->isRegEx 	= RegExCheckBox->Checked;
		Viewer->isMigemo	= MigemoCheckBox->Enabled && MigemoCheckBox->Checked;
		Viewer->isBytes 	= BytesCheckBox->Visible && BytesCheckBox->Checked;
		Viewer->FindWord	= FindComboBox->Text;
		Viewer->BinCodePage = get_CodePageOfName(CodePageComboBox->Text);

		UnicodeString ptn;
		if (!Viewer->FindWord.IsEmpty()) {
			//バイト列
			if (Viewer->isBytes) {
				ptn = Viewer->FindWord;
				if (!StartsStr('\"', ptn) && !StartsStr('\'', ptn)) {
					//数値チェック
					TStringDynArray b_lst = SplitString(Trim(ptn), " ");
					try {
						for (int i=0; i<b_lst.Length; i++) {
							UnicodeString s = b_lst[i];
							s = REPLACE_TS(s, "?", "0");
							if		(s.Length()==2) ("0x" + s).ToInt();
							else if (s.Length()==8) s.ToInt();
							else Abort();
						}
					}
					catch (...) {
						ptn = EmptyStr;
					}
				}
			}
			//文字列
			else {
				if (Viewer->isRegEx && !Viewer->isMigemo)
					ptn = Viewer->FindWord;
				else
					ptn = usr_Migemo->GetRegExPtn(Viewer->isMigemo, Viewer->FindWord, 1);

				//正規表現パターンのチェック
				if (!chk_RegExPtn(ptn)) {
					FindComboBox->Color = col_Illegal;
					ptn = EmptyStr;
				}
				else FindComboBox->Color = scl_Window;
			}
		}
		else FindComboBox->Color = scl_Window;

		Viewer->RegExPtn = RegExPtn = ptn;
		Viewer->Repaint(true);
	}
	else {
		GeneralInfoDlg->FindWord = FindComboBox->Text;
		UnicodeString ptn;
		if (!FindComboBox->Text.IsEmpty()) {
			if (RegExCheckBox->Checked)
				ptn = FindComboBox->Text;
			else
				ptn = usr_Migemo->GetRegExPtn(MigemoCheckBox->Enabled && MigemoCheckBox->Checked, FindComboBox->Text, 1);
			//正規表現パターンのチェック
			if (!chk_RegExPtn(ptn)) {
				FindComboBox->Color = col_Illegal;
				ptn = EmptyStr;
			}
			else FindComboBox->Color = scl_Window;
		}
		else FindComboBox->Color = scl_Window;

		GeneralInfoDlg->RegExPtn = RegExPtn = ptn;
	}
}
//---------------------------------------------------------------------------
//検索
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindNextActionExecute(TObject *Sender)
{
	bool to_save =false;

	if (Viewer) {
		if (fromTV && !NyanFiForm->TxtScrollPanel->Visible) return;

		Viewer->ExeCommand((UpDownGroup->ItemIndex==1)? _T("FindDown") : _T("FindUp"));
		if (Viewer->LastFound && !Viewer->isMigemo) {
			add_ComboBox_history(FindComboBox, Viewer->FindWord);
			to_save = true;
		}
	}
	else {
		if (GeneralInfoDlg->FindText(UpDownGroup->ItemIndex==1)) {
			if (!MigemoCheckBox->Enabled || !MigemoCheckBox->Checked) {
				add_ComboBox_history(FindComboBox, FindComboBox->Text);
				to_save = true;
			}
		}
	}

	if (to_save) IniFile->SaveComboBoxItems(FindComboBox, RegExCheckBox->Checked? _T("FindPtnHistory") : _T("FindTxtHistory"));

	if (CloseCheckBox->Checked) Close();
}
//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FindNextActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !RegExPtn.IsEmpty();

	FindComboBox->Tag
		= CBTAG_HISTORY | (FindComboBox->Focused()? CBTAG_RGEX_V : 0) | (RegExCheckBox->Checked? CBTAG_RGEX_E : 0);
}

//---------------------------------------------------------------------------
void __fastcall TFindTextDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

