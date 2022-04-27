//----------------------------------------------------------------------//
// NyanFi																//
//  拡張入力ボックス													//
//  カレントリストの中央に表示											//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "usr_tag.h"
#include "InpExDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInputExDlg *InputExDlg;

//---------------------------------------------------------------------------
__fastcall TInputExDlg::TInputExDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInputExDlg::FormCreate(TObject *Sender)
{
	IpuntExMode = 0;
	CustomWidth = 0;

	InputComboBox->Tag = CBTAG_HISTORY;

	for (int i=0; i<MAX_SAVE_CODEPAGES; i++) CodePageComboBox->Items->Add(SaveCodePages[i].name);
	CodePageComboBox->ItemIndex = 0;

	KeyHandled = false;
}
//---------------------------------------------------------------------------
void __fastcall TInputExDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	//拡張モードのキャプション設定
	switch (IpuntExMode) {
	case INPEX_CRE_DIR:		 Caption = "ディレクトリの作成 - " + yen_to_delimiter(PathName);	break;
	case INPEX_NEW_TEXTFILE: Caption = "新規テキストの作成";	break;
	case INPEX_CLONE:		 Caption = "クローン作成";			break;
	case INPEX_CRE_TESTFILE: Caption = "テストファイルの作成";	break;
	case INPEX_JUMP_LINE:	 Caption = "指定行番号に移動";		break;
	case INPEX_JUMP_ADDR:	 Caption = "指定アドレスに移動";	break;
	case INPEX_SET_TOPADDR:	 Caption = "先頭アドレスを設定";	break;
	case INPEX_FIND_TAG:	 Caption = "タグ検索";				break;
	case INPEX_ADD_TAG:		 Caption = "タグの追加";			break;
	case INPEX_SET_TAG:		 Caption = "タグの設定";			break;
	case INPEX_TAG_SELECT:	 Caption = "タグ選択";				break;
	case INPEX_CLIP_PASTE:	 Caption = "クリップボードから新規テキスト作成";	break;
	default:
		//メイン側で設定
		;
	}

	InputEdit->Visible	   = false;
	InputComboBox->Tag	   = (IpuntExMode==INPEX_CRE_DIR || IpuntExMode==INPEX_NEW_TEXTFILE)? EDTAG_REF_CDIR : 0;
	InputComboBox->Visible = false;
	NameLabel->Visible	   = false;
	int r_mgn = ScaledInt(8, this);

	//ComboBox
	if (IpuntExMode==INPEX_CRE_DIR || IpuntExMode==INPEX_NEW_TEXTFILE || IpuntExMode==INPEX_CLIP_PASTE
		|| IpuntExMode==INPEX_FIND_TAG || IpuntExMode==INPEX_ADD_TAG || IpuntExMode==INPEX_SET_TAG || IpuntExMode==INPEX_TAG_SELECT)
	{
		ClientWidth = 480;
		NameLabel->Visible	   = true;
		NameLabel->Caption	   = (IpuntExMode==INPEX_FIND_TAG || IpuntExMode==INPEX_ADD_TAG || IpuntExMode==INPEX_SET_TAG)? "タグ" : "名前";
		InputComboBox->Visible = true;
		UnicodeString def_str  = InputComboBox->Text;

		//タグ名リスト
		if (IpuntExMode==INPEX_FIND_TAG || IpuntExMode==INPEX_ADD_TAG || IpuntExMode==INPEX_SET_TAG || IpuntExMode==INPEX_TAG_SELECT) {
			InputComboBox->Items->Assign(usr_TAG->TagNameList);
		}
		//履歴
		else {
			IniFile->LoadComboBoxItems(InputComboBox, (IpuntExMode==INPEX_CRE_DIR)? _T("CreateDirHistory") : _T("NewTextHistory"));
		}

		InputComboBox->Text  = def_str;
		InputComboBox->Width = ClientWidth - InputComboBox->Left - r_mgn;

		InputExDlg->InputComboBox->Hint =
			(IpuntExMode==INPEX_FIND_TAG || IpuntExMode==INPEX_TAG_SELECT)? "; 区切りでAND検索、｜区切りでOR検索" :
				(IpuntExMode==INPEX_ADD_TAG || IpuntExMode==INPEX_SET_TAG)? "; で区切って複数指定可能" : "";

		InputComboBoxChange(NULL);
	}
	//Edit
	else {
		InputEdit->Visible = true;
		int c_wd = ScaledInt((CustomWidth>0)? CustomWidth : InputEdit->NumbersOnly? 260 : 480, this);
		int l_wd = Canvas->TextWidth(InputEdit->EditLabel->Caption);
		ClientWidth = std::max(c_wd, l_wd + ScaledInt(70, this));
		InputEdit->Left  = l_wd + ScaledInt(12, this);
		InputEdit->Width = ClientWidth - InputEdit->Left - r_mgn;
	}

	//表示位置を調整
	TPoint p;
	TRect tmp_rc = BoundsRect; tmp_rc.SetHeight(ScaledInt(120, this));
	if (Screen->ActiveForm==Application->MainForm && StartsText("ファンクションキー", Caption)) {
		int idx = (extract_int_def(InputEdit->EditLabel->Caption, 1) - 1) / 4;
		TControl *cp = Application->MainForm;
		p = cp->ClientToScreen(cp->ClientRect.CenteredRect(tmp_rc).Location);
		p.y += cp->ClientHeight/4;
		if		(idx==0) p.x -= cp->ClientWidth/3;
		else if (idx==2) p.x += cp->ClientWidth/3;
	}
	else {
		TControl *cp = (Screen->ActiveForm!=Application->MainForm)? (TControl*)Screen->ActiveForm :
										 	 (ScrMode==SCMD_FLIST)? (TControl*)FileListBox[CurListTag] :
																	(TControl*)Application->MainForm;
		p = cp->ClientToScreen(cp->ClientRect.CenteredRect(tmp_rc).Location);
	}

	TRect main_rc = Application->MainForm->BoundsRect;
	if (p.x<main_rc.Left)			 p.x = main_rc.Left + 8;
	if ((p.x + Width)>main_rc.Right) p.x = main_rc.Right - Width - 8;
	Left = p.x;
	Top  = p.y;

	NewTextPanel->Visible  = false;
	CreDirPanel->Visible   = false;
	CreTestPanel->Visible  = false;
	IniSttPanel->Visible   = false;
	NotationPanel->Visible = false;

	int hi = InputPanel->Height;
	switch (IpuntExMode) {
	case INPEX_CRE_DIR:
		DirChgCheckBox->Checked = CreDirChg;
		CnvChCheckBox->Checked	= CreDirCnvChar;
		CreDirPanel->Visible	= true;
		hi += CreDirPanel->Height;
		if (!InputComboBox->Text.IsEmpty()) {
			SelDefCheckBox->Checked = IniFile->ReadBoolGen(_T("CreDirIniSel"));
			IniSttPanel->Visible	= true;
		}
		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#CreateDir"));
		break;
	case INPEX_NEW_TEXTFILE:
	case INPEX_CLIP_PASTE:
		CodePageComboBox->ItemIndex = IniFile->ReadIntGen(_T("NewTextCodePage"));
		CodePageComboBox->Width = ClientWidth - CodePageComboBox->Left - r_mgn;
		ClipCheckBox->Checked = IniFile->ReadBoolGen(_T("NewTextClip"));
		EditCheckBox->Checked = EditNewText;
		ClipPanel->Visible	  = (IpuntExMode!=INPEX_CLIP_PASTE);
		NewTextPanel->Visible = true;
		hi += NewTextPanel->Height;
		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#NewTextFile"));
		break;
	case INPEX_CLONE:
		SelDefCheckBox->Checked = IniFile->ReadBoolGen(_T("CloneIniSel"));
		IniSttPanel->Visible	= true;
		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#Clone"));
		break;
	case INPEX_CRE_TESTFILE:
		InputEdit->Text    = IniFile->ReadStrGen(_T("CreTestFileName"));
		TestSizeEdit->Text = IniFile->ReadStrGen(_T("CreTestFileSize"), "1M");
		TestCntEdit->Text  = "1";
		CreTestPanel->Visible = true;
		hi += CreTestPanel->Height;
		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#CreateTestFile"));
		break;
	case INPEX_JUMP_ADDR: case INPEX_SET_TOPADDR:
		InputEdit->Text = EmptyStr;
		if (IniFile->ReadBoolGen(_T("HexAddress"), true))
			HexRadioBtn->Checked = true;
		else
			DecRadioBtn->Checked = true;
		NotationPanel->Visible = true;

		if (IpuntExMode==INPEX_JUMP_ADDR)
			HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_TV) _T("#JumpLine"));
		else
			HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_TV) _T("#SetTopAddr"));
		break;
	case INPEX_JUMP_LINE:
		InputEdit->Text = EmptyStr;
		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_TV) _T("#JumpLine"));
		break;
	case INPEX_FIND_TAG:	HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#FindTag"));	break;
	case INPEX_ADD_TAG:		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#AddTag"));	break;
	case INPEX_SET_TAG:		HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#SetTag"));	break;
	case INPEX_TAG_SELECT:	HelpTopic.sprintf(_T("%s"), _T(HELPTOPIC_FL) _T("#TagSelect"));	break;
	default:
		HelpTopic = EmptyStr;
	}

	hi += BottomPanel->Height;
	ClientHeight = hi;

	SetDarkWinTheme(this);
	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}

//---------------------------------------------------------------------------
void __fastcall TInputExDlg::WmFormShowed(TMessage &msg)
{
	if (InputEdit->Visible) {
		InputEdit->SetFocus();
		if (!InputEdit->Text.IsEmpty()) {
			if (SameText(Caption, LoadUsrMsg(USTR_Rename))) {
				int p = pos_r(_T("."), InputEdit->Text) - 1;
				if (p<0) p = InputEdit->Text.Length();
				switch (IniFile->ReadIntGen(_T("RenameDlgIniStt"))) {
				case 1:
					InputEdit->SelStart = 0;
					break;
				case 2:
					InputEdit->SelStart  = 0;
					InputEdit->SelLength = p;
					break;
				case 3:
					InputEdit->SelectAll();
					break;
				default:
					InputEdit->SelStart = p;
				}
			}
			else {
				if (IniSttPanel->Visible && SelDefCheckBox->Checked)
					InputEdit->SelectAll();
				else
					InputEdit->SelStart = InputEdit->Text.Length();
			}
		}
	}
	else if (InputComboBox->Visible) {
		InputComboBox->SetFocus();
		if (IniSttPanel->Visible && SelDefCheckBox->Checked)
			InputComboBox->SelectAll();
		else
			InputComboBox->SelStart = InputComboBox->Text.Length();
	}
}

//---------------------------------------------------------------------------
void __fastcall TInputExDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	CloseIME(Handle);

	//履歴の保存
	if (ModalResult==mrOk && (IpuntExMode==INPEX_CRE_DIR || IpuntExMode==INPEX_NEW_TEXTFILE)) {
		add_ComboBox_history(InputComboBox);
		IniFile->SaveComboBoxItems(InputComboBox,
					(IpuntExMode==INPEX_CRE_DIR)? _T("CreateDirHistory") : _T("NewTextHistory"));
	}

	//その他のオプションの保存
	if (IpuntExMode==INPEX_CRE_DIR) {
		CreDirChg	  = DirChgCheckBox->Checked;
		CreDirCnvChar = CnvChCheckBox->Checked;
	}
	else if (IpuntExMode==INPEX_NEW_TEXTFILE) {
		IniFile->WriteIntGen(_T("NewTextCodePage"),	CodePageComboBox);
		IniFile->WriteBoolGen(_T("NewTextClip"),	ClipCheckBox);
		EditNewText = EditCheckBox->Checked;
	}
	else if (IpuntExMode==INPEX_CRE_TESTFILE) {
		IniFile->WriteStrGen(_T("CreTestFileName"),	InputEdit->Text);
		IniFile->WriteStrGen(_T("CreTestFileSize"),	TestSizeEdit->Text);
	}
	else if (IpuntExMode==INPEX_JUMP_ADDR || IpuntExMode==INPEX_SET_TOPADDR) {
		IniFile->WriteBoolGen(_T("HexAddress"),		HexRadioBtn->Checked);
		UnicodeString lbuf = Trim(InputEdit->Text);
		if (HexRadioBtn->Checked) {
			//16進の場合、0x を付加
			if (!lbuf.IsEmpty()) lbuf.Insert("0x", starts_tchs(_T("+-"), lbuf)? 2 : 1);
		}
		InputEdit->Text = lbuf;
	}

	if (IniSttPanel->Visible) {
		switch (IpuntExMode) {
		case INPEX_CLONE:	IniFile->WriteBoolGen(_T("CloneIniSel"),  SelDefCheckBox); break;
		case INPEX_CRE_DIR:	IniFile->WriteBoolGen(_T("CreDirIniSel"), SelDefCheckBox); break;
		}
	}

	IpuntExMode = 0;
	CustomWidth = 0;
	InputEdit->NumbersOnly = false;
	InputEdit->Hint 	= EmptyStr;
	InputComboBox->Hint = EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TInputExDlg::OptionClick(TObject *Sender)
{
	if		(InputEdit->Visible)	 InputEdit->SetFocus();
	else if (InputComboBox->Visible) InputComboBox->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TInputExDlg::SelDefCheckBoxClick(TObject *Sender)
{
	if (InputEdit->Visible) {
		InputEdit->SetFocus();
		if (IniSttPanel->Visible && SelDefCheckBox->Checked)
			InputEdit->SelectAll();
		else
			InputEdit->SelStart = InputEdit->Text.Length();
	}
	else if (InputComboBox->Visible) {
		InputComboBox->SetFocus();
		if (IniSttPanel->Visible && SelDefCheckBox->Checked)
			InputComboBox->SelectAll();
		else
			InputComboBox->SelStart = InputComboBox->Text.Length();
	}
}

//---------------------------------------------------------------------------
void __fastcall TInputExDlg::InputEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	KeyHandled = SpecialEditProc(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TInputExDlg::InputEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (KeyHandled) {
		KeyHandled = false;
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TInputExDlg::InputComboBoxChange(TObject *Sender)
{
	if (IpuntExMode==INPEX_CRE_DIR) {
		int f_len = InputComboBox->Text.Length();
		int p_len = PathName.Length() + f_len;
		TColor fg = get_TextColor();
		PathInfLabel->Font->Color = (p_len<248 || PathName.Pos('/'))? fg : col_Error;
		NameInfLabel->Font->Color = (f_len<256)? fg : col_Error;
		UnicodeString tmp;
		PathInfLabel->Caption = tmp.sprintf(_T("フルパス名の文字数 = %u"), p_len);
		NameInfLabel->Caption = tmp.sprintf(_T("名前の文字数 = %u"), f_len);
	}
}

//---------------------------------------------------------------------------
void __fastcall TInputExDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (equal_F1(KeyStr) && !HelpTopic.IsEmpty()) {
		HtmlHelpTopic(HelpTopic.c_str());
		Key = 0;
	}
	else {
		if (CreDirPanel->Visible) {
			if		(USAME_TI(KeyStr, "Alt+C")) invert_CheckBox(DirChgCheckBox);
			else if (USAME_TI(KeyStr, "Alt+R")) invert_CheckBox(CnvChCheckBox);
		}
		if (NewTextPanel->Visible) {
			if		(USAME_TI(KeyStr, "Alt+P")) invert_CheckBox(ClipCheckBox);
			else if (USAME_TI(KeyStr, "Alt+E")) invert_CheckBox(EditCheckBox);
		}
		if (IniSttPanel->Visible) {
			if		(USAME_TI(KeyStr, "Alt+P")) invert_CheckBox(SelDefCheckBox);
		}
		if (NotationPanel) {
			if		(USAME_TI(KeyStr, "Alt+H")) HexRadioBtn->Checked = true;
			else if (USAME_TI(KeyStr, "Alt+D")) DecRadioBtn->Checked = true;
		}
	}
}

//---------------------------------------------------------------------------
//InputExDlg 用関数
//---------------------------------------------------------------------------
UnicodeString inputbox_ex(
	const _TCHAR *tit,		//タイトル
	const _TCHAR *prm,		//プロンプト
	UnicodeString def,		//デフォルト	(default = EmptyStr)
	bool num_only)			//数字のみ入力	(default = false)
{
	InputExDlg->Caption = tit;
	InputExDlg->InputEdit->EditLabel->Caption = prm? prm : null_TCHAR;
	InputExDlg->InputEdit->NumbersOnly = num_only;
	InputExDlg->InputEdit->Text = def;
	return ((InputExDlg->ShowModal()==mrOk)? InputExDlg->InputEdit->Text : EmptyStr);
}
//---------------------------------------------------------------------------
int inputbox_ex_n(
	const _TCHAR *tit,
	const _TCHAR *prm,
	UnicodeString def_s,	//デフォルト文字列	(default = EmptyStr)
	int def_n)				//デフォルト数値	(default = 0)
{
	return inputbox_ex(tit, prm, def_s, true).ToIntDef(def_n);
}
//---------------------------------------------------------------------------
bool input_query_ex(
	const _TCHAR *tit,		//タイトル
	const _TCHAR *prm,		//プロンプト
	UnicodeString *value,
	int width,				//表示幅指定	(default = 0 : なし)
	bool num_only,			//数字のみ入力	(default = false)
	UnicodeString hint)		//ヒント		(default = EmptyStr)
{
	InputExDlg->Caption = tit;
	InputExDlg->InputEdit->EditLabel->Caption = prm;
	InputExDlg->CustomWidth = width;
	InputExDlg->InputEdit->NumbersOnly = num_only;
	InputExDlg->InputEdit->Hint = hint;
	InputExDlg->InputEdit->Text = *value;
	bool res = (InputExDlg->ShowModal()==mrOk);
	if (res) *value = InputExDlg->InputEdit->Text;
	return res;
}
//---------------------------------------------------------------------------
bool input_query_ex(
	int tid,				//メッセージID
	const _TCHAR *prm,
	UnicodeString *value,
	int width,
	bool num_only,
	UnicodeString hint)
{
	return input_query_ex(LoadUsrMsg(tid).c_str(), prm, value, width, num_only, hint);
}
//---------------------------------------------------------------------------

