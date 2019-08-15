//----------------------------------------------------------------------//
// NyanFi																//
//  名前の変更															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <System.DateUtils.hpp>
#include <RegularExpressions.hpp>
#include "usr_exif.h"
#include "usr_id3.h"
#include "UserFunc.h"
#include "InpExDlg.h"
#include "UserMdl.h"
#include "RenDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRenameDlg *RenameDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TRenameDlg::TRenameDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormCreate(TObject *Sender)
{
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	UserModule->SetUsrPopupMenu(this);

	SrcStrComboBox->Tag  = CBTAG_HISTORY;
	RepStrComboBox->Tag  = CBTAG_HISTORY;
	Mp3FmtComboBox->Tag  = CBTAG_HISTORY;
	RenListComboBox->Tag = CBTAG_HISTORY;

	set_ComboBoxText(IniSttComboBox,
		_T("ファイル名主部の末尾\n")
		_T("名前の先頭\n")
		_T("ファイル名主部を選択\n")
		_T("ファイル名全体を選択\n")
		_T("指定語の後\n")
		_T("指定語の前\n"));
	set_ComboBoxText(IniStt2ComboBox,
		_T("ファイル名主部の末尾\n")
		_T("名前の先頭\n")
		_T("ファイル名主部を選択\n")
		_T("ファイル名全体を選択\n"));

	IniSttWdEdit->Hint = LoadUsrMsg(USTR_HintMltSepSC);

	//右クリックメニューのアクションを設定
	UserModule->RenInsNameAction->OnExecute = Inp1stNameActionExecute;
	UserModule->RenInsNameAction->OnUpdate	= Inp1stNameActionUpdate;

	SttPrgBar = new SttProgressBar(StatusBar1, 1);
	SttPrgBar->MsgIndex = 0;

	ItemList	  = new TStringList();
	NewNameList   = new TStringList();
	SerFormatList = new TStringList();
	CurNameList   = new TStringList();
	RepRenList	  = new TStringList();

	EditedList	 = false;
	IsMulti 	 = false;
	LastEdit	 = NULL;
	LastSelStart = LastSelLength = 0;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	DlgInitialized = false;
	Previewing	   = Previewed = false;
	ExistErr	   = false;
	ChangeCount    = 0;

	IsMulti = (ItemList->Count>1);

	NewNameList->Clear();
	for (int i=0; i<ItemList->Count; i++) NewNameList->Add(EmptyStr);

	//設定の読込
	IniFile->LoadPosInfo(this, DialogCenter);

	IniSttComboBox->ItemIndex  = IniFile->ReadIntGen(_T("RenameDlgIniStt"));
	IniSttWdEdit->Text		   = IniFile->ReadStrGen(_T("RenameDlgIniSttWd"));
	IniStt2ComboBox->ItemIndex = IniFile->ReadIntGen(_T("RenameDlgIniStt2"));
	SerLeftPanel->Width 	   = IniFile->ReadIntGen(_T("RenameDlgSerLWidth"),	280);
	SerNoEdit->Text 		   = IniFile->ReadStrGen(_T("RenameDlgSerNo"),		"01");
	IncNoEdit->Text 		   = IniFile->ReadStrGen(_T("RenameDlgIncNo"),		"1");
	CaseCheckBox->Checked	   = IniFile->ReadBoolGen(_T("RenameDlgCase"));
	Case2CheckBox->Checked	   = CaseCheckBox->Checked;
	RegExCheckBox->Checked	   = IniFile->ReadBoolGen(_T("RenameDlgRegEx"));
	OnlyBaseCheckBox->Checked  = IniFile->ReadBoolGen(_T("RenameDlgOnlyBase"));
	OnlyBase2CheckBox->Checked = OnlyBaseCheckBox->Checked;
	CnvCharCheckBox->Checked   = IniFile->ReadBoolGen(_T("RenameDlgCnvChar"));
	CnvKanaCheckBox->Checked   = IniFile->ReadBoolGen(_T("RenameDlgCnvKana"));
	AutoPrvCheckBox->Checked   = IniFile->ReadBoolGen(_T("RenameDlgAutoPrv"),	true);
	EndMatchCheckBox->Checked  = IniFile->ReadBoolGen(_T("RenameDlgEndMatch"));
	NoRenLogCheckBox->Checked  = IniFile->ReadBoolGen(_T("RenameDlgNoRenLog"));
	UnicodeString lastSheet    = IniFile->ReadStrGen(_T("RenameDlgLastSheet"),	"Serial");
	UnicodeString lastMp3Sheet = IniFile->ReadStrGen(_T("RenameDlgLastMp3Sheet"),	"Mp3");

	IniFile->LoadComboBoxItems(SrcStrComboBox,	RegExCheckBox->Checked? _T("RenSrcPtnHistory") : _T("RenSrcStrHistory"));
	IniFile->LoadComboBoxItems(RepStrComboBox,	_T("RenRepStrHistory"));

	IniFile->LoadComboBoxItems(Mp3FmtComboBox,	_T("RenMp3FmtHistory"));
	if (Mp3FmtComboBox->Items->Count==0) Mp3FmtComboBox->Items->Add("\\ID3(TT2)");

	//リストファイル履歴
	IniFile->LoadComboBoxItems(RenListComboBox,	_T("RenListHistory"));
	int i = 0;
	while (i<RenListComboBox->Items->Count) {
		if (!file_exists(to_absolute_name(RenListComboBox->Items->Strings[i])))
			RenListComboBox->Items->Delete(i);
		else
			i++;
	}
	RenListComboBox->Enabled = !EditedList;

	IniFile->ReadSection("SerFormatList",	SerFormatList);

	SerFmtComboBox->Clear();
	for (int i=0; i<SerFormatList->Count; i++) SerFmtComboBox->Items->Add(SerFormatList->Names[i]);
	SerFmtComboBox->Text = EmptyStr;

	set_ListBoxItemHi(CnvCharListBox);
	CnvCharListBox->Items->Assign(CnvCharList);
	CnvChSEdit->Text = EmptyStr;
	CnvChREdit->Text = EmptyStr;

	FbaseRadioGroup->ItemIndex = 0;
	FextRadioGroup->ItemIndex  = 0;

	StatusBar1->Canvas->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);

	if (UnInitializing) return;

	//関連改名設定
	TCheckListBox *lp = AssRenListBox;
	set_ListBoxItemHi(lp);
	lp->Items->Assign(AssRenList);
	for (int i=0; i<lp->Count; i++) lp->Checked[i] = !equal_0(get_csv_item(lp->Items->Strings[i], 1));
	lp->ItemIndex = -1;
	UserModule->InitializeListBox(lp);
	AssExtEdit->Text = EmptyStr;

	//プレビュー表示の初期化
	TStringGrid *gp = PreviewGrid;
	gp->RowCount = ItemList->Count;
	for (int i=0; i<gp->RowCount; i++) clear_GridRow(gp, i);
	AutoPrvCheckBox->Enabled = IsMulti;

	//属性の初期化
	int atr = file_GetAttr(ItemList->Strings[0]);
	ReadOnlyCheckBox->Checked = (atr & faReadOnly);
	HiddenCheckBox->Checked   = (atr & faHidden);
	SysCheckBox->Checked	  = (atr & faSysFile);
	ArcCheckBox->Checked	  = (atr & faArchive);
	CmpCheckBox->Checked	  = (atr & faCompressed);

	ReadOnlyCheckBox->Tag	  = -1;	//変更有りのチェック用
	HiddenCheckBox->Tag 	  = -1;
	SysCheckBox->Tag		  = -1;
	ArcCheckBox->Tag		  = -1;
	CmpCheckBox->Tag		  = -1;

	//タイムスタンプの初期化
	TimeMaskEdit->Text = format_DateTime(get_file_age(ItemList->Strings[0]));
	TimeMaskEdit->Font->Color = get_TextColor();

	//リスト
	Case2CheckBox->Enabled	   = !EditedList;
	OnlyBase2CheckBox->Enabled = !EditedList;
	RefListBtn->Enabled		   = !EditedList;
	ListErrLabel->Caption	   = EmptyStr;
	ListErrLabel->Font->Color  = col_Error;

	int f_cnt = 0;
	//---------------------------
	//単独
	//---------------------------
	if (!IsMulti) {
		UnicodeString f_name = ItemList->Strings[0];
		bool is_dir = ends_PathDlmtr(f_name);
		f_name	= ExcludeTrailingPathDelimiter(f_name);
		cmnFext = is_dir? EmptyStr : get_extension(f_name);
		isMp3	= test_Mp3Ext(cmnFext);
		isFlac	= test_FlacExt(cmnFext);
		if (!is_dir) f_cnt++;

		NameSheet->TabVisible		= !EditedList;
		SerialSheet->TabVisible 	= false;
		ReplaceSheet->TabVisible	= !EditedList;
		RenListSheet->TabVisible	= true;
		Mp3Sheet->Caption			= isFlac? "&FLAC" : "MP&3";
		Mp3Sheet->TabVisible		= isMp3 || isFlac;
		mp3fextLabel->Caption		= cmnFext;
		NamePageControl->ActivePage = EditedList? RenListSheet : NameSheet;
	}
	//---------------------------
	//複数
	//---------------------------
	else {
		//共通拡張子のチェック
		for (int i=0; i<ItemList->Count; i++) {
			UnicodeString f_name = ItemList->Strings[i];
			bool is_dir = ends_PathDlmtr(f_name);
			f_name  = ExcludeTrailingPathDelimiter(f_name);
			UnicodeString f_ext = is_dir? EmptyStr : get_extension(f_name);
			if (!is_dir) f_cnt++;
			if (i==0) cmnFext = f_ext;
			else if (!SameText(cmnFext, f_ext)) cmnFext = EmptyStr;
		}
		isMp3  = test_Mp3Ext(cmnFext);
		isFlac = test_FlacExt(cmnFext);

		NameSheet->TabVisible	 = false;
		SerialSheet->TabVisible  = !EditedList;
		ReplaceSheet->TabVisible = !EditedList;
		RenListSheet->TabVisible = true;
		Mp3Sheet->TabVisible	 = !EditedList && (isMp3 || isFlac);
		Mp3Sheet->Caption		 = isFlac? "&FLAC" : "MP&3";
		mp3fextLabel->Caption	 = cmnFext;
		if (!EditedList && (isMp3 || isFlac)) {
			NamePageControl->ActivePage =
				USAME_TI(lastMp3Sheet, "Mp3")? Mp3Sheet : (USAME_TI(lastMp3Sheet, "Replace")? ReplaceSheet : SerialSheet);
		}
		else {
			NamePageControl->ActivePage =
				(EditedList || USAME_TI(lastSheet, "RenList"))? RenListSheet :
							   USAME_TI(lastSheet, "Replace") ? ReplaceSheet : SerialSheet;
		}
	}

	//ファイルを含む場合、拡張子の設定を有効に
	bool inc_file = (f_cnt>0);
	FextRadioGroup->Enabled = inc_file;
	ExtEdit->Text			= inc_file? get_tkn_r(get_extension_if_file(ItemList->Strings[0]), '.') : EmptyStr;
	ExtEdit->Enabled		= inc_file;

	IniSttComboBoxClick(NULL);

	NameChanged = FExtChanged = AttrChanged = TimeChanged = false;
	DlgInitialized = true;

	NamePageControl->Enabled = true;
	CommonPanel->Enabled	 = true;
	OkButton->Enabled		 = true;
	CanButton->Enabled		 = true;

	SetDarkWinTheme(this);
	SetDarkWinTheme(NameComPanel);

	TColor bg_p = IsMulti? get_WinColor(true) : get_PanelColor();
	ReadOnlyPanel->Color = bg_p;
	HiddenPanel->Color	 = bg_p;
	SysPanel->Color 	 = bg_p;
	ArcPanel->Color 	 = bg_p;
	CmpPanel->Color 	 = bg_p;
	TimeMaskEdit->Color  = get_WinColor(!IsMulti);

	AssRenListBox->Color  = get_WinColor();
	CnvCharListBox->Color = get_WinColor();

	SttPrgBar->BgColor	  = col_bgPrgBar;
	SttPrgBar->BarColor   = col_fgPrgBar;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::WmFormShowed(TMessage &msg)
{
	Repaint();
	UserModule->RenInsNameAction->Update();

	//※起動後最初に開いたとき、名前欄が全選択になってしまう現象への対処
	NamePageControlChange(NULL);

	if (EditedList) {
		RepRenList->Clear();
		if (open_by_TextEditor(RenListFile)) {
			if (msgbox_Sure(_T("リストファイルを読み込みますか?"))) LoadListFile();
		}
		else msgbox_ERR(GlobalErrMsg);
	}
	else if (IsRenListSheet()) {
		if (RenListComboBox->Items->Count>0) {
			RenListComboBox->ItemIndex = 0;
			RenListFile = to_absolute_name(RenListComboBox->Text);
			LoadListFile();
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("RenameDlgIniStt"),		IniSttComboBox);
	IniFile->WriteStrGen(_T("RenameDlgIniSttWd"),	IniSttWdEdit);
	IniFile->WriteIntGen(_T("RenameDlgIniStt2"),	IniStt2ComboBox);
	IniFile->WriteIntGen(_T("RenameDlgSerLWidth"),	SerLeftPanel->Width);
	IniFile->WriteStrGen(_T("RenameDlgSerNo"),		SerNoEdit);
	IniFile->WriteStrGen(_T("RenameDlgIncNo"),		IncNoEdit);
	IniFile->WriteBoolGen(_T("RenameDlgCase"),		IsRenListSheet()? Case2CheckBox : CaseCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgRegEx"),		RegExCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgOnlyBase"),	IsRenListSheet()? OnlyBase2CheckBox : OnlyBaseCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgCnvChar"),	CnvCharCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgCnvKana"),	CnvKanaCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgAutoPrv"),	AutoPrvCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgEndMatch"),	EndMatchCheckBox);
	IniFile->WriteBoolGen(_T("RenameDlgNoRenLog"),	NoRenLogCheckBox);

	const _TCHAR *key = (isMp3 || isFlac)? _T("RenameDlgLastMp3Sheet") : _T("RenameDlgLastSheet");
	if 		(IsMp3Sheet())		IniFile->WriteStrGen(key,	"Mp3");
	else if (IsReplaceSheet())	IniFile->WriteStrGen(key,	"Replace");
	else if (IsRenListSheet())	IniFile->WriteStrGen(key,	"RenList");
	else if (IsSerialSheet())	IniFile->WriteStrGen(key,	"Serial");

	IniFile->SaveComboBoxItems(SrcStrComboBox,	RegExCheckBox->Checked? _T("RenSrcPtnHistory") : _T("RenSrcStrHistory"));
	IniFile->SaveComboBoxItems(RepStrComboBox,	_T("RenRepStrHistory"));
	IniFile->SaveComboBoxItems(Mp3FmtComboBox,	_T("RenMp3FmtHistory"));
	IniFile->SaveComboBoxItems(RenListComboBox,	_T("RenListHistory"));

	CnvCharList->Assign(CnvCharListBox->Items);

	IniFile->AssignSection("SerFormatList",	SerFormatList);

	UserModule->RenInsNameAction->Visible = false;

	RepRenList->Clear();

	if (EditedList) delete_FileIf(RenListFile);
	EditedList	= false;
	RenListFile = EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormDestroy(TObject *Sender)
{
	delete ItemList;
	delete NewNameList;
	delete SerFormatList;
	delete CurNameList;
	delete RepRenList;
	delete SttPrgBar;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormResize(TObject *Sender)
{
	int s_wd = ::GetSystemMetrics(SM_CXVSCROLL) + 2;	//スクロールバーの幅を取得
	TStringGrid *gp = PreviewGrid;
	gp->ColWidths[1] = 20;
	int wd = (gp->ClientWidth - s_wd - gp->ColWidths[1])/2;
	gp->ColWidths[0] = wd;
	gp->ColWidths[2] = wd;
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Opt2MainPanelResize(TObject *Sender)
{
	int wd = CnvCharGroupBox->ClientWidth - 20;
	CnvCharPanelL->Width = wd/2;

	CnvChSEdit->Width = (CnvCharPanelR->Width - 16 - (CnvChLabel->Width + 12)) / 2;
	CnvChREdit->Width = CnvChSEdit->Width;
	CnvChLabel->Left  = CnvChSEdit->Left + CnvChSEdit->Width + 6;
	CnvChREdit->Left  = CnvChLabel->Left + CnvChLabel->Width + 6;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenPageControlDrawTab(TCustomTabControl *Control,
	int TabIndex, const TRect &Rect, bool Active)
{
	draw_OwnerTab(Control, TabIndex, Rect, Active, IsDarkMode);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = IsDarkMode? col_bgSttBar : scl_BtnFace;
	cv->FillRect(Rect);
	cv->Font->Color = (ExistErr && Panel->Index==0)? col_Error : IsDarkMode? col_fgSttBar : scl_BtnText;
	cv->TextOut(Rect.Left + 2, Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//タブページ変更
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::NamePageControlChange(TObject *Sender)
{
	TColor bg_inv = get_WinColor(true);

	if (!IsOptionSheet()) {
		TStringGrid *gp = PreviewGrid;
		gp->Color = bg_inv;
		gp->DefaultRowHeight   = get_FontHeight(gp->Font, 4);
		NameComPanel->Parent   = NamePageControl->ActivePage;
		PathInfMLabel->Caption = EmptyStr;
		NameInfMLabel->Caption = EmptyStr;
		SttInfLabel->Caption   = EmptyStr;
		FbaseRadioGroup->ItemIndex = 0;
		FextRadioGroup->ItemIndex  = 0;
		SetDarkWinTheme(gp);
		Repaint();
	}

	//名前
	if (IsNameSheet()) {
		UnicodeString f_name = ItemList->Strings[0];
		bool is_dir = ends_PathDlmtr(f_name);
		f_name = ExcludeTrailingPathDelimiter(f_name);
		RenameEdit->SetFocus();
		RenameEdit->Text = ExtractFileName(f_name);

		//初期状態の設定
		int p = !is_dir? (pos_r(_T("."), RenameEdit->Text) - 1) : -1;
		if (p<0) p = RenameEdit->Text.Length();

		int idx = IniSttComboBox->ItemIndex;
		int wd_p = 0, wd_len = 0;
		if (idx==4 || idx==5) {
			//指定語のチェック
			TStringDynArray wlst = split_strings_semicolon(IniSttWdEdit->Text);
			for (int i=0; i<wlst.Length; i++) {
				wd_p = (EndMatchCheckBox->Checked)? pos_r(wlst[i], RenameEdit->Text) : pos_i(wlst[i], RenameEdit->Text);
				if (wd_p>0) {
					wd_len = wlst[i].Length();
					break;
				}
			}
			//指定語が含まれていなければ次候補
			if (wd_p==0) idx = IniStt2ComboBox->ItemIndex;
		}

		switch (idx) {
		case 1:	//名前の先頭
			RenameEdit->SelStart = 0;
			break;
		case 2:	//ファイル名主部を選択
			RenameEdit->SelStart  = 0;
			RenameEdit->SelLength = p;
			break;
		case 3:	//ファイル名全体を選択
			RenameEdit->SelectAll();
			break;
		case 4:	//指定語の後
			p = wd_p;
			if (p>0) p += (wd_len - 1);
			RenameEdit->SelStart = p;
			break;
		case 5:	//指定語の前
			p = wd_p;
			if (p>0) p--;
			RenameEdit->SelStart = p;
			break;
		default:
			RenameEdit->SelStart = p;
		}
	}
	//連番
	else if (IsSerialSheet()) {
		PreNameEdit->Text	= EmptyStr;
		PreNameEdit->Color	= bg_inv;
		SerNoEdit->Color	= bg_inv;
		IncNoEdit->Color	= bg_inv;
		PostNameEdit->Text  = EmptyStr;
		PostNameEdit->Color = bg_inv;
		ExtEdit->Color		= bg_inv;
		NameChanged = FExtChanged = false;
		PreNameEdit->SetFocus();
		SerFmtComboBox->ItemIndex = -1;
		SerFmtComboBox->Text = EmptyStr;
	}
	//置換
	else if (IsReplaceSheet()) {
		ReplaceComboChange(NULL);
		SrcStrComboBox->SetFocus();
	}
	//リスト
	else if (IsRenListSheet()) {
		if (!EditedList && RenListComboBox->Items->Count>0) {
			if (RenListComboBox->ItemIndex==-1) RenListComboBox->ItemIndex = 0;
			RenListFile = to_absolute_name(RenListComboBox->Text);
			LoadListFile();
		}
	}
	//MP3 / FLAC
	else if (IsMp3Sheet()) {
		Mp3FmtComboBox->ItemIndex = -1;
		Mp3FmtComboBoxChange(NULL);
	}

	Previewed = false;
	TStringGrid *gp = PreviewGrid;
	set_RedrawOff(gp);
	for (int i=0; i<gp->RowCount; i++) clear_GridRow(gp, i);
	gp->Color = bg_inv;
	set_RedrawOn(gp);

	if (IsOptionSheet()) {
		CnvCharListBox->ItemIndex = -1;
		CnvCharListBoxClick(NULL);
	}
	else {
		CnvCharList->Assign(CnvCharListBox->Items);
		UpdatePreview();
	}
}

//---------------------------------------------------------------------------
//改名欄フォーカス時の処理
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenameEditExit(TObject *Sender)
{
	TEdit *ep = (TEdit*)Sender;
	switch (ep->Tag) {
	case 10: case 13:	//前後
		ep->Color = get_WinColor(!NameChanged);
		break;
	case 11: case 12:	//連番
		ep->Color = get_WinColor(!NameChanged || SerNoEdit->Text.IsEmpty());
		break;
	case 14:			//拡張子
		ep->Color = get_WinColor(!FExtChanged);
		break;
	}

	LastEdit	  = ep;
	LastSelStart  = ep->SelStart;
	LastSelLength = ep->SelLength;
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::SrcStrComboBoxEnter(TObject *Sender)
{
	((TComboBox*)Sender)->Color = get_WinColor();
	UpdateActions();
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::SrcStrComboBoxExit(TObject *Sender)
{
	TComboBox *bp = (TComboBox*)Sender;
	bp->Color = get_WinColor(bp->Text.IsEmpty());
}

//---------------------------------------------------------------------------
//リストファイルの読み込み
//---------------------------------------------------------------------------
bool __fastcall TRenameDlg::LoadListFile()
{
	if (RenListFile.IsEmpty()) return false;

	std::unique_ptr<TStringList> fbuf(new TStringList());
	if (load_text_ex(RenListFile, fbuf.get())!=0) {
		RepRenList->Clear();
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString lbuf = fbuf->Strings[i];
			if (lbuf.IsEmpty()) break;
			if (lbuf.Pos('\t')==0) continue;
			RepRenList->Add(lbuf);
		}
		UpdatePreview();
		return true;
	}
	else {
		RenListFile = EmptyStr;
		return false;
	}
}

//---------------------------------------------------------------------------
//新しい名前のリストを更新
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::UpdateNewNameList()
{
	if (Previewing) return;

	//フォーカス状態を待避
	TWinControl *last_cp = Screen->ActiveControl;
	TCustomEdit *last_ep = class_is_CustomEdit(last_cp)? (TCustomEdit *)last_cp : NULL;
	TComboBox	*last_bp = class_is_ComboBox(last_cp)? (TComboBox *)last_cp : NULL;
	int last_ss = last_ep? last_ep->SelStart :  last_bp? last_bp->SelStart : 0;
	int last_sl = last_ep? last_ep->SelLength : last_bp? last_bp->SelLength :0;

	MainPanel->Enabled = false;
	Previewing = true;
	ExistErr   = false;
	SttPrgBar->Begin(_T("プレビュー..."));

	//連番
	int sn	 = EditToInt(SerNoEdit);
	int dn	 = EditToInt(IncNoEdit, 1);
	int n_wd = SerNoEdit->Text.Length();

	//置換
	UnicodeString swd, rwd;
	TRegExOptions opt;
	bool only_base;
	if (IsReplaceSheet()) {
		swd = SrcStrComboBox->Text;
		rwd = RepStrComboBox->Text;
		if (!RegExCheckBox->Checked) swd = TRegEx::Escape(swd);
		RepStrComboBox->Color = get_WinColor(swd.IsEmpty());
		if (CaseCheckBox->Checked)  opt << roIgnoreCase;
		only_base = OnlyBaseCheckBox->Checked;
	}
	else if (IsRenListSheet()) {
		if (Case2CheckBox->Checked) opt << roIgnoreCase;
		only_base = OnlyBase2CheckBox->Checked;
	}

	TStringGrid *gp = PreviewGrid;
	set_RedrawOff(gp);

	int max_flen = 0;
	int max_plen = 0;
	int mat_cnt  = 0;
	int top_idx  = 0;

	bool illegal = false;
	ListErrLabel->Caption = EmptyStr;

	bool show_prg = (!AutoPrvCheckBox->Checked || ItemList->Count>1000);	//***

	ChangeCount = 0;
	UnicodeString pre_alstr, post_alstr;
	for (int i=0; i<ItemList->Count; i++) {
		if (show_prg) {
			SttPrgBar->SetPosI(i, ItemList->Count);
			if (i%100==0) Application->ProcessMessages();
		}

		UnicodeString f_name = ItemList->Strings[i];
		bool is_dir = ends_PathDlmtr(f_name);
		f_name = ExcludeTrailingPathDelimiter(f_name);
		UnicodeString p_name   = ExtractFilePath(f_name);
		UnicodeString b_name   = is_dir? ExtractFileName(f_name) : get_base_name(f_name);
		UnicodeString f_ext    = is_dir? EmptyStr : get_extension(f_name);
		UnicodeString old_name = b_name + f_ext;
		UnicodeString new_name = old_name;

		//名前
		if (IsNameSheet()) {
			new_name = RenameEdit->Text;
		}
		//連番
		else if (IsSerialSheet()) {
			if (NameChanged) {
				new_name  = format_FileName(PreNameEdit->Text, f_name, &pre_alstr);
				if (n_wd>0) new_name.cat_sprintf(_T("%0*u"), n_wd, sn);
				new_name += format_FileName(PostNameEdit->Text, f_name, &post_alstr);

				if (ExtEdit->Enabled) {
					if (FExtChanged && !ExtEdit->Text.IsEmpty()) {
						UnicodeString fext = f_ext;  remove_top_s(fext, '.');
						new_name.cat_sprintf(_T(".%s"), ReplaceStr(ExtEdit->Text, "\\E", fext).c_str());
					}
					else {
						new_name += f_ext;
					}
				}
			}
			else if (FExtChanged) {
				//拡張子のみ変更
				new_name = b_name;
				if (ExtEdit->Enabled && !ExtEdit->Text.IsEmpty()) {
					UnicodeString fext = f_ext;  remove_top_s(fext, '.');
					new_name.cat_sprintf(_T(".%s"), ReplaceStr(ExtEdit->Text, "\\E", fext).c_str());
				}
			}
			sn += dn;
		}
		//置換
		else if (IsReplaceSheet()) {
			if (!illegal) {
				try {
					if (only_base) {
						if (TRegEx::IsMatch(b_name, swd, opt)) {
							new_name = replace_regex_2(b_name, swd, rwd, opt) + f_ext;
							mat_cnt++;
						}
					}
					else {
						if (TRegEx::IsMatch(old_name, swd, opt)) {
							new_name = replace_regex_2(old_name, swd, rwd, opt);
							mat_cnt++;
						}
					}
				}
				catch (...) {
					illegal = true;
				}
			}
		}
		//リスト
		else if (IsRenListSheet()) {
			//編集
			if (EditedList) {
				for (int j=top_idx; j<RepRenList->Count; j++) {
					UnicodeString lbuf = RepRenList->Strings[j];
					int idx = get_pre_tab(lbuf).ToIntDef(0) - 1;
					if (idx==i) {
						new_name = get_post_tab(lbuf);
						top_idx  = j + 1;
						break;
					}
				}
			}
			//置換
			else if (!illegal) {
				UnicodeString r_name = only_base? b_name : old_name;
				bool matched = false;
				for (int j=0; j<RepRenList->Count; j++) {
					UnicodeString lbuf = RepRenList->Strings[j];
					swd = split_pre_tab(lbuf);	if (swd.IsEmpty()) continue;
					rwd = lbuf;

					try {
						swd = !is_regex_slash(swd)? TRegEx::Escape(swd) : exclude_top_end(swd);
						if (TRegEx::IsMatch(r_name, swd, opt)) {
							r_name  = replace_regex_2(r_name, swd, rwd, opt);
							matched = true;
						}
					}
					catch (...) {
						illegal = true;
						ListErrLabel->Caption
							= UnicodeString().sprintf(_T("リストファイルに不正な行があります(%u)"), j + 1); 
						break;
					}
				}

				if (matched) {
					mat_cnt++;
					new_name = r_name;
					if (only_base) new_name += f_ext;
				}
			}
		}
		//MP3 / FLAC
		else if (IsMp3Sheet() && !Mp3FmtComboBox->Text.IsEmpty()) {
			std::unique_ptr<TStringList> tlst(new TStringList());
			UnicodeString s = Mp3FmtComboBox->Text;
			new_name = EmptyStr;
			//MP3
			if (isMp3 && ID3_GetInf(f_name, tlst.get(), true)) {
				int p = 1;
				while (p<=s.Length()) {
					WideChar c = s[p];
					if (c=='\\') {
						UnicodeString fmt = get_tkn(s.SubString(p, 256), ')') + ")";
						if (StartsStr("\\ID3(", fmt)) {
							UnicodeString istr = get_in_paren(fmt);
							UnicodeString vstr = tlst->Values[istr.SubString(1, 3)];
							if (istr.Length()>3 && StartsStr("TRK",istr)) {
								int trkn = get_tkn(vstr, '/').ToIntDef(0);
								UnicodeString nstr = istr.SubString(4, 1);
								if		(USAME_TS(nstr, "1")) vstr.sprintf(_T("%1u"), trkn);
								else if (USAME_TS(nstr, "2")) vstr.sprintf(_T("%02u"),trkn);
								else if (USAME_TS(nstr, "3")) vstr.sprintf(_T("%03u"),trkn);
							}
							new_name += vstr;
						}
						p += fmt.Length();
					}
					else {
						new_name += c;	p++;
					}
				}
				new_name += cmnFext;
			}
			//FLAC
			else if (isFlac && get_FlacInf(f_name, tlst.get(), NULL, true)) {
				std::unique_ptr<TStringList> id_lst(new TStringList());
				id_lst->Text =
					"TP1\tARTIST\n"
					"TAL\tALBUM\n"
					"TT2\tTITLE\n"
					"TRK1\tTRACKNUMBER_1\n"
					"TRK2\tTRACKNUMBER_2\n"
					"TRK3\tTRACKNUMBER_3\n"
					"TRK\tTRACKNUMBER\n"
					"TYE\tDATe\n";
				int p = 1;
				while (p<=s.Length()) {
					WideChar c = s[p];
					if (c=='\\') {
						UnicodeString fmt = get_tkn(s.SubString(p, 256), ')') + ")";
						if (StartsStr("\\ID3(", fmt)) {
							UnicodeString istr = get_in_paren(fmt);
							istr = replace_str_by_list(istr, id_lst.get());
							UnicodeString vstr = tlst->Values[get_tkn(istr, '_')];
							UnicodeString nstr = get_tkn_r(istr, '_');
							if (!nstr.IsEmpty()) {
								int trkn = get_tkn(vstr, '/').ToIntDef(0);
								if		(USAME_TS(nstr, "1")) vstr.sprintf(_T("%1u"), trkn);
								else if (USAME_TS(nstr, "2")) vstr.sprintf(_T("%02u"),trkn);
								else if (USAME_TS(nstr, "3")) vstr.sprintf(_T("%03u"),trkn);
							}
							new_name += vstr;
						}
						p += fmt.Length();
					}
					else {
						new_name += c;	p++;
					}
				}
				new_name += cmnFext;
			}
		}

		//主部の文字種変換
		int x_p = !is_dir? pos_r('.', new_name) : 0;
		UnicodeString bnam = (x_p>1)? new_name.SubString(1, x_p - 1) : new_name;
		if (FbaseRadioGroup->ItemIndex>0) bnam = ConvCharType(bnam, FbaseRadioGroup->ItemIndex);
		//拡張子の文字種変換
		UnicodeString xnam = (x_p>1 && x_p<new_name.Length())?
								new_name.SubString(x_p + 1, new_name.Length() - x_p) : EmptyStr;
		if (FextRadioGroup->ItemIndex>0)  xnam = ConvCharType(xnam, FextRadioGroup->ItemIndex);

		//禁止文字／ユーザ定義文字の変換
		if (CnvCharCheckBox->Checked) bnam = ApplyCnvCharList(bnam);

		new_name = bnam;
		if (!xnam.IsEmpty()) new_name.cat_sprintf(_T(".%s"), xnam.c_str());

		//表示設定
		NewNameList->Strings[i] = p_name + new_name;
		gp->Cells[0][i] = old_name;
		gp->Cells[2][i] = new_name;

		if (!SameStr(old_name, new_name)) ChangeCount++;
		if (new_name.Length()>max_flen) max_flen = new_name.Length();
		if (NewNameList->Strings[i].Length()>max_plen) max_plen = NewNameList->Strings[i].Length();
	}
	SttPrgBar->End(EmptyStr, 0);

	//-------------------
	//エラーチェック
	//-------------------
	int err_cnt = 0;
	int err_row = -1;

	if (ChangeCount==0) {
		for (int i=0; i<NewNameList->Count; i++) gp->Cells[1][i] = "＝";
	}
	else {
		SttPrgBar->Begin(_T("エラーチェック..."));
		for (int i=0; i<NewNameList->Count; i++) {
			if (show_prg) {
				SttPrgBar->SetPosI(i, NewNameList->Count);
				if (i%100==0) Application->ProcessMessages();
			}
			UnicodeString old_name = gp->Cells[0][i];
			UnicodeString new_name = gp->Cells[2][i];
			try {
				if (new_name.IsEmpty()) {
					gp->Cells[2][i] = "(empty)";  Abort();
				}

				if (USAME_TS(new_name, " ")) Abort();

				//禁止文字
				UnicodeString ngstr = "\\/:*?\"<>|";
				for (int j=1; j<=ngstr.Length(); j++) if (new_name.Pos(ngstr[j])) Abort();

				//既存名との衝突
				if (CurNameList->IndexOf(NewNameList->Strings[i])!=-1) {
					bool is_ng = true;
					for (int j=0; j<ItemList->Count && is_ng; j++) {
						if (SameText(ExcludeTrailingPathDelimiter(ItemList->Strings[j]), NewNameList->Strings[i]))
							is_ng = false;
					}
					if (is_ng) Abort();
				}

				gp->Cells[1][i] = SameStr(old_name, new_name)? "＝" : "→";
			}
			catch (...) {
				gp->Cells[1][i] = "×";
				err_row = i;
				err_cnt++;
			}
		}
		SttPrgBar->End(EmptyStr, 0);

		//重複チェック
		if (IsMulti) {
			SttPrgBar->Begin(_T("重複チェック..."));
			for (int i=0; i<NewNameList->Count-1; i++) {
				if (show_prg) {
					SttPrgBar->SetPosI(i, NewNameList->Count);
					if (i%100==0) Application->ProcessMessages();
				}

				for (int j=i+1; j<NewNameList->Count; j++) {
					if (SameText(NewNameList->Strings[i], NewNameList->Strings[j])) {
						gp->Cells[1][i] = "×";
						gp->Cells[1][j] = "×";
						if (err_row==-1) err_row = i;
						err_cnt++;
					}
				}
			}
			SttPrgBar->End(EmptyStr, 0);
		}
	}

	ExistErr = (err_cnt>0);
	RenameEdit->Font->Color = (!IsMulti && ExistErr)? col_Error : get_TextColor();
	if (ExistErr) {
		gp->Row = err_row;
		StatusBar1->Panels->Items[0]->Text = "設定に問題があります。";
	}
	else {
		gp->Row = 0;
		StatusBar1->Panels->Items[0]->Text = EmptyStr;
	}

	gp->Color = get_WinColor(ChangeCount==0);
	set_RedrawOn(gp);

	//文字数情報の表示
	PathInfMLabel->Font->Color = (max_plen>=MAX_PATH)? col_Error : get_TextColor();
	NameInfMLabel->Font->Color =	  (max_flen>=256)? col_Error : get_TextColor();

	UnicodeString msg;
	if (ItemList->Count>1) {
		PathInfMLabel->Caption = msg.sprintf(_T("フルパス名の最大文字数 = %u"), max_plen);
		NameInfMLabel->Caption = msg.sprintf(_T("名前の最大文字数 = %u"), max_flen);
	}
	else {
		PathInfMLabel->Caption = msg.sprintf(_T("フルパス名の文字数 = %u"), max_plen);
		NameInfMLabel->Caption = msg.sprintf(_T("名前の文字数 = %u"), max_flen);
	}

	//状態表示
	msg.sprintf(_T("改名:%u /%u"), ChangeCount, ItemList->Count);
	if (IsReplaceSheet()) msg = UnicodeString().sprintf(_T("マッチ:%u %s"), mat_cnt, msg.c_str());
	SttInfLabel->Caption = msg;

	Previewed  = true;
	Previewing = false;
	MainPanel->Enabled = true;

	//フォーカス状態を復帰
	if (last_cp) {
		last_cp->SetFocus();
		if (last_ep) {
			last_ep->SelStart  = last_ss;
			last_ep->SelLength = last_sl;
		}
		else if (last_bp) {
			last_bp->SelStart  = last_ss;
			last_bp->SelLength = last_sl;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::UpdatePreview()
{
	if (DlgInitialized && (AutoPrvCheckBox->Checked || !IsMulti)) UpdateNewNameList(); else Previewed = false;
}

//---------------------------------------------------------------------------
//連番改名の設定が変更された
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenameEditChange(TObject *Sender)
{
	if (!DlgInitialized) return;

	switch (((TComponent*)Sender)->Tag) {
	case 10: case 11: case 12: case 13:
		NameChanged = true;
		PreNameEdit->Color	= get_WinColor();
		InvColIfEmpty(SerNoEdit);
		IncNoEdit->Color	= SerNoEdit->Color;
		PostNameEdit->Color = get_WinColor();
		break;
	case 14:
		FExtChanged    = true;
		ExtEdit->Color = get_WinColor();
		break;
	}

	UpdatePreview();
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenameEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialEditProc(Sender, Key, Shift);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenameEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (is_KeyPress_CtrlNotCV(Key)) Key = 0;
}

//---------------------------------------------------------------------------
//検索履歴の入れ換え
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RegExCheckBoxClick(TObject *Sender)
{
	if (DlgInitialized) {
		change_ComboBoxHistory(SrcStrComboBox, _T("RenSrcStrHistory"), _T("RenSrcPtnHistory"), RegExCheckBox->Checked);
		UpdatePreview();
	}
}

//---------------------------------------------------------------------------
//置換改名の設定が変更された
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ReplaceComboChange(TObject *Sender)
{
	if (Sender) UpdatePreview();
}

//---------------------------------------------------------------------------
//MP3/FLAC書式設定が変更された
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Mp3FmtComboBoxChange(TObject *Sender)
{
	Mp3FmtComboBox->Color = get_WinColor(Mp3FmtComboBox->Text.IsEmpty());
	if (Sender) UpdatePreview();
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RefFmtBtnClick(TObject *Sender)
{
	UnicodeString s = get_word_i_idx(
		_T("\\ID3(TP1)|\\ID3(TAL)|\\ID3(TT2)|\\ID3(TRK)|\\ID3(TYE)"), ((TComponent*)Sender)->Tag);
	Mp3FmtComboBox->SelText = s;
}

//---------------------------------------------------------------------------
//文字種設定を変更した
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FModRadioGroupClick(TObject *Sender)
{
	UpdatePreview();
}

//---------------------------------------------------------------------------
//属性設定を変更した
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AttrCheckBoxClick(TObject *Sender)
{
	if (!DlgInitialized) return;

	TCheckBox *cp = (TCheckBox*)Sender;
	cp->Tag 	= 1;	//変更あり
	AttrChanged = true;

	TPanel *pp = dynamic_cast<TPanel *>(cp->Parent);
	if (pp) pp->Color = get_PanelColor();
}

//---------------------------------------------------------------------------
//タイムスタンプ設定を変更した
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::TimeMaskEditChange(TObject *Sender)
{
	if (!DlgInitialized) return;

	TMaskEdit *ep = (TMaskEdit*)Sender;
	ep->Color = get_WinColor(ep->EditText.Pos('_'));
	TimeChanged = true;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ChangeTime(TDateTime dt)
{
	TimeMaskEdit->Text	= format_DateTime(dt);
	TimeMaskEdit->Color = get_WinColor();
	TimeChanged = true;
}

//---------------------------------------------------------------------------
//現在時刻
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::TimeNowBtnClick(TObject *Sender)
{
	ChangeTime(Now());
}
//---------------------------------------------------------------------------
//00:00:00
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Time00BtnClick(TObject *Sender)
{
	ChangeTime(DateOf(Now()));
}
//---------------------------------------------------------------------------
//12:00:00
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Time12BtnClick(TObject *Sender)
{
	ChangeTime(IncHour(DateOf(Now()), 12));
}
//---------------------------------------------------------------------------
//XX:00:00
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::TimeX0BtnClick(TObject *Sender)
{
	TDateTime dt = Now();
	unsigned short h, n, s, ms;
	DecodeTime(dt, h, n, s, ms);
	dt -= TDateTime(0, n, s, ms);
	if (n>=30) dt += TDateTime(1, 0, 0, 0);
	ChangeTime(dt);
}

//---------------------------------------------------------------------------
//最新
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::LatestTimeActionExecute(TObject *Sender)
{
	TDateTime dt = 0.0;
	for (int i=0; i<ItemList->Count; i++)
		dt = std::max(dt, get_file_age(ItemList->Strings[i]));

	ChangeTime(dt);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::LatestTimeActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = IsMulti;
}

//---------------------------------------------------------------------------
//連番書式を設定
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::set_FmtEdit(const _TCHAR *s)
{
	UnicodeString sbuf = UnicodeString(s);

	if (LastEdit) {
		LastEdit->SetFocus();
		LastEdit->SelStart	= LastSelStart;
		LastEdit->SelLength = LastSelLength;
	}

	if (PreNameEdit->Focused()) {
		PreNameEdit->SetSelTextBuf(sbuf.c_str());
	}
	else if (PostNameEdit->Focused()) {
		PostNameEdit->SetSelTextBuf(sbuf.c_str());
	}
	else if (ExtEdit->Focused()) {
		if (USAME_TS(sbuf, "\\E")) ExtEdit->SetSelTextBuf(sbuf.c_str()); else beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::set_FmtEdit(UnicodeString s)
{
	set_FmtEdit(s.c_str());
}

//---------------------------------------------------------------------------
//ファイル名の左からn文字
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_L_BtnClick(TObject *Sender)
{
	UnicodeString tmp;
	if (input_query_ex(USTR_InputFmtStr, _T("ファイル名主部の左端から取得する文字数"), &tmp, 0, true)) {
		int n = tmp.ToIntDef(0);
		if (n>0) set_FmtEdit(tmp.sprintf(_T("\\L(%u)"), n));
	}
}
//---------------------------------------------------------------------------
//ファイル名のm番目からn文字
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_S_Btn2Click(TObject *Sender)
{
	UnicodeString tmp;
	if (input_query_ex(USTR_InputFmtStr, _T("ファイル名主部からの取得開始位置"), &tmp, 0, true)) {
		int m = tmp.ToIntDef(0);
		if (m>0) {
			tmp = EmptyStr;
			if (input_query_ex(USTR_InputFmtStr, _T("取得する文字数"), &tmp, 0, true)) {
				int n = tmp.ToIntDef(0);
				if (n>0) set_FmtEdit(tmp.sprintf(_T("\\S(%u,%u)"), m, n));
			}
		}
	}
}
//---------------------------------------------------------------------------
//ファイル名の右からn文字
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_R_BtnClick(TObject *Sender)
{
	UnicodeString tmp;
	if (input_query_ex(USTR_InputFmtStr, _T("ファイル名主部の右端から取得する文字数"), &tmp, 0, true)) {
		int n = tmp.ToIntDef(0);
		if (n>0) set_FmtEdit(tmp.sprintf(_T("\\R(%u)"), n));
	}
}
//---------------------------------------------------------------------------
//ファイル名全体
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_A_BtnClick(TObject *Sender)
{
	set_FmtEdit(_T("\\A"));
}
//---------------------------------------------------------------------------
//拡張子
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_E_BtnClick(TObject *Sender)
{
	set_FmtEdit(_T("\\E"));
}
//---------------------------------------------------------------------------
//タイムスタンプ
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_TS_BtnClick(TObject *Sender)
{
	set_FmtEdit(_T("\\TS(yyyymmdd-hhnnss)"));
}
//---------------------------------------------------------------------------
//Exif撮影日時
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Fmt_XT_BtnClick(TObject *Sender)
{
	set_FmtEdit(_T("\\XT(yyyymmdd-hhnnss)"));
}

//---------------------------------------------------------------------------
//連番改名書式の保存
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::SaveSerFmtActionExecute(TObject *Sender)
{
	TComboBox *cp = SerFmtComboBox;
	UnicodeString lbuf;
	lbuf.sprintf(_T("%s=%s,%s,%s,%s"),
		cp->Text.c_str(),
		make_csv_str(PreNameEdit->Text).c_str(), make_csv_str(SerNoEdit->Text).c_str(),
		make_csv_str(IncNoEdit->Text).c_str(),	 make_csv_str(PostNameEdit->Text).c_str());

	if (FExtChanged && ExtEdit->Enabled && !ExtEdit->Text.IsEmpty())
		lbuf.cat_sprintf(_T(",%s"), make_csv_str(ExtEdit->Text).c_str());

	int idx = cp->Items->IndexOf(cp->Text);
	if (idx!=-1) {
		SerFormatList->Strings[idx] = lbuf;
	}
	else {
		SerFormatList->Insert(0, lbuf);
		cp->Items->Insert(0, cp->Text);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::SaveSerFmtActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !SerFmtComboBox->Text.IsEmpty() && NameChanged;
}

//---------------------------------------------------------------------------
//連番改名書式の呼び出し
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::SerFmtComboBoxClick(TObject *Sender)
{
	int idx = SerFmtComboBox->ItemIndex;
	if (idx>=0 && idx<SerFormatList->Count) {
		TStringDynArray itm_buf = get_csv_array(SerFormatList->ValueFromIndex[idx], 5);
		if (itm_buf.Length>=4) {
			//主部
			PreNameEdit->Text  = itm_buf[0];
			SerNoEdit->Text    = itm_buf[1];
			IncNoEdit->Text    = itm_buf[2];
			PostNameEdit->Text = itm_buf[3];
			NameChanged = true;
			PreNameEdit->Color	= get_WinColor();
			InvColIfEmpty(SerNoEdit);
			IncNoEdit->Color	= SerNoEdit->Color;
			PostNameEdit->Color = get_WinColor();
			//拡張子
			if (itm_buf.Length==5 && ExtEdit->Enabled) {
				ExtEdit->Text = itm_buf[4];
				FExtChanged   = true;
			}
			else {
				ExtEdit->Text = get_tkn_r(get_extension_if_file(ItemList->Strings[0]), '.');
				FExtChanged   = false;
			}
			ExtEdit->Color = get_WinColor(!FExtChanged);

			UpdatePreview();
		}
	}
}

//---------------------------------------------------------------------------
//項目の削除
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelItemActionExecute(TObject *Sender)
{
	if (IsSerialSheet()) {
		int idx = del_ComboBox_history(SerFmtComboBox);
		if (idx!=-1) SerFormatList->Delete(idx);
	}
	else if (IsMp3Sheet()) {
		del_ComboBox_history(Mp3FmtComboBox);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelItemActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled =
		IsSerialSheet()? !SerFmtComboBox->Text.IsEmpty() :
		   IsMp3Sheet()? !Mp3FmtComboBox->Text.IsEmpty() : false;
}

//---------------------------------------------------------------------------
//文字種の変更
//---------------------------------------------------------------------------
UnicodeString __fastcall TRenameDlg::ConvCharType(UnicodeString s, int idx)
{
	if (!s.IsEmpty()) {
		switch (idx) {
		case 1: s = s.UpperCase();		break;
		case 2: s = s.LowerCase();		break;
		case 3: s = s.SubString(1, 1).UpperCase() + exclude_top(s).LowerCase();	break;
		case 4: s = to_FullWidth(s);	break;
		case 5:
			{
				if (CnvKanaCheckBox->Checked) {
					s = to_HalfWidth(s);
					std::unique_ptr<TStringList> lst(new TStringList());
					lst->Text = "\\=￥\n/=／\n:=：\n*=＊\n?=？\n\"=”\n<=＜\n>=＞\n|=｜\n";
					for (int i=0; i<lst->Count; i++) s = ReplaceStr(s, lst->Names[i], lst->ValueFromIndex[i]);
				}
				else {
					UnicodeString full_ch =
						"！＃＄％＆’（）＋，－＿０１２３４５６７８９；＝＠"
						"ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ［］"
						"ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ｛｝～　";
					UnicodeString half_ch = "!#$%&'()+,-_0123456789;=@ABCDEFGHIJKLMNOPQRSTUVWXYZ[]abcdefghijklmnopqrstuvwxyz{}~ ";
					for (int i=1; i<=s.Length(); i++) {
						int p = full_ch.Pos(s[i]);
						if (p>0) s[i] = half_ch[p];
					}
				}
			}
			break;
		}
	}
	return s;
}

//---------------------------------------------------------------------------
//プレビューの描画
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::PreviewGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	cv->Brush->Color = gp->Color;
	cv->Font->Color  = get_TextColor();

	UnicodeString cell_str = gp->Cells[ACol][ARow];
	UnicodeString rel_str  = gp->Cells[1][ARow];
	if (ACol==1 || ACol==2) {
		if (USAME_TS(rel_str, "＝"))
			cv->Font->Color = clGray;
		else if (rel_str.IsEmpty() || USAME_TS(rel_str, "×"))
			cv->Font->Color = col_Error;
	}
	cell_str = minimize_str(cell_str, cv, Rect.Width() - 4, OmitEndOfName);

	cv->FillRect(Rect);
	cv->TextRect(Rect, Rect.Left + 2, Rect.Top + 2, cell_str);

	if(State.Contains(gdFocused)) cv->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------
//プレビューでのキー操作
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::PreviewGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TStringGrid *gp = (TStringGrid*)Sender;

	switch (idx_of_word_i(_T("Shift+Ctrl+DOWN|Shift+Ctrl+UP"), get_KeyStr(Key, Shift))) {
	case 0:	//項目を下に移動
		if (AutoPrvCheckBox->Checked) {
			int idx = down_StrListItem(ItemList, gp->Row);
			UpdateNewNameList();
			gp->Row = idx;
		}
		else beep_Warn();
		Key = 0;
		break;
	case 1:	//項目を上に移動
		if (AutoPrvCheckBox->Checked) {
			int idx = up_StrListItem(ItemList, gp->Row);
			UpdateNewNameList();
			gp->Row = idx;
		}
		else beep_Warn();
		Key = 0;
		break;
	}
}

//---------------------------------------------------------------------------
//禁止文字置換一覧の描画
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::CnvCharListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int x = Rect.Left + Scaled4;
	int y = Rect.Top  + get_TopMargin(cv);

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);

	if (!State.Contains(odSelected)) {
		cv->Brush->Color = (Index<9)? get_PanelColor() : get_WinColor();
	}

	UnicodeString tmp;
	tmp.sprintf(_T("[%s]"), lp->Items->Names[Index].c_str());
	SpaceTextOut(tmp, cv, x, y, cv->Font->Color, true);
	SetHighlight(cv, State.Contains(odSelected));
	out_TextEx(cv, x, y, " → ");
	tmp.sprintf(_T("[%s]"), lp->Items->ValueFromIndex[Index].c_str());
	SpaceTextOut(tmp, cv, x, y, cv->Font->Color, true);

	if (Index==8) draw_separateLine(cv, Rect, 2);
	if (State.Contains(odFocused)) cv->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::CnvCharListBoxClick(TObject *Sender)
{
	int idx = CnvCharListBox->ItemIndex;
	if (idx!=-1) {
		CnvChSEdit->Text = CnvCharListBox->Items->Names[idx];
		CnvChREdit->Text = CnvCharListBox->Items->ValueFromIndex[idx];
	}
	else {
		CnvChSEdit->Text = EmptyStr;
		CnvChREdit->Text = EmptyStr;
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::CnvChSEditChange(TObject *Sender)
{
	TEdit *ep = (TEdit*)Sender;
	set_ErrColor(ep, StartsStr("\\x{", ep->Text) && EndsStr("}", ep->Text) && !chk_RegExPtn(ep->Text));
}

//---------------------------------------------------------------------------
//ユーザ定義文字置換の追加
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AddCnvChActionExecute(TObject *Sender)
{
	CnvCharListBox->Items->Add(CnvChSEdit->Text + "=" + CnvChREdit->Text);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AddCnvChActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	UnicodeString sch = CnvChSEdit->Text;
	if (StartsStr("\\x{", sch) && EndsStr("}", sch))
		ap->Enabled = chk_RegExPtn(sch);
	else
		ap->Enabled = (!sch.IsEmpty() && UnicodeString("\\/:*?\"<>|").Pos(sch)==0);
}

//---------------------------------------------------------------------------
//ユーザ定義文字置換の変更
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ChgCnvChActionExecute(TObject *Sender)
{
	int idx = CnvCharListBox->ItemIndex;	if (idx==-1) return;
	CnvCharListBox->Items->Strings[idx] = CnvChSEdit->Text + "=" + CnvChREdit->Text;
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ChgCnvChActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	UnicodeString sch = CnvChSEdit->Text;
	int idx = CnvCharListBox->ItemIndex;
	if (idx==-1) {
		ap->Enabled = false;
	}
	else if (idx>=0 && idx<9) {
		ap->Enabled = (CnvCharListBox->Items->Names[idx]==sch);
	}
	else if (idx>=9) {
		if (StartsStr("\\x{", sch) && EndsStr("}", sch))
			ap->Enabled = chk_RegExPtn(sch);
		else 
			ap->Enabled = (!sch.IsEmpty() && UnicodeString("\\/:*?\"<>|").Pos(sch)==0);
	}
}

//---------------------------------------------------------------------------
//ユーザ定義文字置換の削除
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelCnvChActionExecute(TObject *Sender)
{
	delete_ListItem(CnvCharListBox);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelCnvChActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (CnvCharListBox->ItemIndex>8);
}

//---------------------------------------------------------------------------
//関連改名設定
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AssRenListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TCheckListBox *lp = (TCheckListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + get_TopMargin(cv);

	int w_x = 50;
	for (int i=0; i<lp->Count; i++)
		w_x = std::max(cv->TextWidth(get_csv_item(lp->Items->Strings[i], 0)), w_x);

	TStringDynArray itm_lst = get_csv_array(lp->Items->Strings[Index], 3, true);
	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(xp, yp, itm_lst[0]);	xp += (w_x + 16);
	if (ContainsStr(itm_lst[2], "P")) out_Text(cv, xp, yp, _T("部分一致"));
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AssRenListBoxClick(TObject *Sender)
{
	AssExtEdit->Text = EmptyStr;

	TCheckListBox *lp = (TCheckListBox*)Sender;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray itm_lst = get_csv_array(lp->Items->Strings[idx], 3, true);
		AssExtEdit->Text = itm_lst[0];
		PrtChgCheckBox->Checked = ContainsStr(itm_lst[2], "P");
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AssRenListBoxClickCheck(TObject *Sender)
{
	TCheckListBox *lp = (TCheckListBox*)Sender;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		TStringDynArray itm_lst = get_csv_array(lp->Items->Strings[idx], 3, true);
		itm_lst[1].sprintf(_T("%s"), lp->Checked[idx]? _T("1") : _T("0"));
		lp->Items->Strings[idx] = make_csv_rec_str(itm_lst);
	}
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AssRenListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}

//---------------------------------------------------------------------------
//登録項目文字列を作成
//---------------------------------------------------------------------------
UnicodeString __fastcall TRenameDlg::MakeAssRenItem(int idx)
{
	UnicodeString ret_str = make_csv_str(AssExtEdit->Text);
	ret_str.cat_sprintf(_T("%s"), (idx!=-1 && AssRenListBox->Checked[idx])? _T(",\"1\"") : _T(",\"0\""));
	ret_str.cat_sprintf(_T("%s"), PrtChgCheckBox->Checked? _T(",P") : _T(","));
	return ret_str;
}
//---------------------------------------------------------------------------
//関連改名設定の追加
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AddAssRenActionExecute(TObject *Sender)
{
	TCheckListBox *lp = AssRenListBox;
	lp->Items->Add(MakeAssRenItem());
	lp->ItemIndex = lp->Count - 1;
	AssRenListBoxClick(lp);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AddAssRenActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !AssExtEdit->Text.IsEmpty();
}
//---------------------------------------------------------------------------
//関連改名設定の変更
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ChgAssRenActionExecute(TObject *Sender)
{
	TCheckListBox *lp = AssRenListBox;
	int idx = lp->ItemIndex;
	if (idx!=-1) lp->Items->Strings[idx] = MakeAssRenItem(idx);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::ChgAssRenActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (AssRenListBox->ItemIndex!=-1 && !AssExtEdit->Text.IsEmpty());
}
//---------------------------------------------------------------------------
//関連改名設定の削除
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelAssRenActionExecute(TObject *Sender)
{
	TCheckListBox *lp = AssRenListBox;
	delete_ListItem(lp);
	AssRenListBoxClick(lp);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::DelAssRenActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (AssRenListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//1番目のファイル名を入力
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Inp1stNameActionExecute(TObject *Sender)
{
	UnicodeString fnam = ItemList->Strings[0];
	if (dir_exists(fnam))
		fnam = ExtractFileName(ExcludeTrailingPathDelimiter(fnam));
	else
		fnam = ChangeFileExt(ExtractFileName(fnam), EmptyStr);

	SrcStrComboBox->SetFocus();
	SrcStrComboBox->Text	  = fnam;
	SrcStrComboBox->SelLength = 0;
	SrcStrComboBox->SelStart  = fnam.Length();
	ReplaceComboChange(SrcStrComboBox);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::Inp1stNameActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = Visible && SrcStrComboBox->Focused();
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::IniSttComboBoxClick(TObject *Sender)
{
	int idx = IniSttComboBox->ItemIndex;
	IniSttWdEdit->Enabled	  = (idx==4 || idx==5);
	IniStt2ComboBox->Enabled  = IniSttWdEdit->Enabled;
	EndMatchCheckBox->Enabled = IniSttWdEdit->Enabled;
}

//---------------------------------------------------------------------------
//プレビュー
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::PreviewBtnClick(TObject *Sender)
{
	UpdateNewNameList();
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::AutoPrvCheckBoxClick(TObject *Sender)
{
	if (DlgInitialized && (AutoPrvCheckBox->Checked || !IsMulti)) UpdateNewNameList(); else Previewed = false;
}

//---------------------------------------------------------------------------
//確定
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenOkActionExecute(TObject *Sender)
{
	if (!Previewed) {
		UpdateNewNameList();
		if (ExistErr) return;
	}

	OkButton->Enabled  = false;
	CanButton->Enabled = false;

	NamePageControl->Enabled = false;
	CommonPanel->Enabled	 = false;

	//関連改名設定を更新
	AssRenList->Assign(AssRenListBox->Items);

	try {
		//------------------------------------------
		//名前の変更
		//------------------------------------------
		UnicodeString msg;
		if (ChangeCount>0) {
			//関連改名チェック
			SttPrgBar->Begin(_T("関連改名チェック..."));
			int i = 0;
			while (i<ItemList->Count) {
				SttPrgBar->SetPosI(i, ItemList->Count);
				Application->ProcessMessages();

				bool is_dir = ends_PathDlmtr(ItemList->Strings[i]);
				ItemList->Strings[i] = ExcludeTrailingPathDelimiter(ItemList->Strings[i]);
				UnicodeString fnam0 = ItemList->Strings[i];
				UnicodeString nnam0 = NewNameList->Strings[i];

				//関連改名のチェック
				if (!is_dir && !SameText(fnam0, nnam0) && AssRenList->Count>0) {
					UnicodeString fext0 = get_extension(fnam0);
					UnicodeString pnam	= ExtractFilePath(fnam0);
					UnicodeString mask	= "*" + get_base_name(fnam0) + "*.*";
					std::unique_ptr<TStringList> lst(new TStringList());
					get_files(pnam, mask.c_str(), lst.get());

					for (int j=0; j<lst->Count; j++) {
						UnicodeString fnam1 = lst->Strings[j];
						if (SameStr(fnam1, fnam0)) continue;
						if (ItemList->IndexOf(fnam1)!=-1) continue;	//改名対象にあるものはスキップ

						//関連拡張子か?
						UnicodeString fext1 = get_extension(fnam1);
						bool found = false;
						UnicodeString opt; 
						for (int k=0; k<AssRenList->Count; k++) {
							TStringDynArray itm_buf = get_csv_array(AssRenList->Strings[k], 3, true);
							if (!equal_1(itm_buf[1])) continue;
							if (test_FileExt(fext0, itm_buf[0]) && test_FileExt(fext1, itm_buf[0])) {
								opt = itm_buf[2];  found = true;  break;
							}
						}
						if (!found) continue;						//関連ファイルでないものはスキップ

						UnicodeString nnam1;
						UnicodeString bnam0 = get_base_name(fnam0);
						UnicodeString bnam1 = get_base_name(fnam1);
						//部分一致
						if (ContainsStr(opt, "P")) {
							nnam1 = ExtractFilePath(nnam0) + ReplaceText(bnam1, bnam0, get_base_name(nnam0)) + fext1;
						}
						//完全一致
						else {
							if (!SameText(bnam0, bnam1)) continue;	//部分一致ファイルはスキップ
							nnam1 = ChangeFileExt(nnam0, fext1);
						}
						if (SameStr(fnam1, nnam1)) continue;		//改名無しならスキップ

						//同時改名対象を追加
						if (found) {
							i++;
							ItemList->Insert(i, fnam1);
							NewNameList->Insert(i, nnam1);
						}
					}
				}
				i++;
			}
			SttPrgBar->End();

			//改名中の重複チェック
			bool duplicated = false;
			if (IsMulti) {
				SttPrgBar->Begin(_T("重複チェック..."));
				for (int i=0; !duplicated && i<ItemList->Count; i++) {
					SttPrgBar->SetPosI(i, ItemList->Count);
					Application->ProcessMessages();
					if (SameStr(ItemList->Strings[i], NewNameList->Strings[i])) continue;
					//改名中の重複チェック
					for (int j=0; !duplicated && j<ItemList->Count; j++) {
						if (i==j) continue;
						if (SameText(ItemList->Strings[j], NewNameList->Strings[i])) duplicated = true;
					}
				}
				SttPrgBar->End();
			}

			//改名開始
			int ok_cnt = 0, er_cnt = 0, sk_cnt = 0;
			UnicodeString ModeStr = get_word_i_idx(_T("|連番|置換|MP3"), NamePageControl->ActivePageIndex) + "改名";
			StartLog(msg.sprintf(_T("%s開始  %s"), ModeStr.c_str(), CurPathName.c_str()));
			Application->ProcessMessages();

			//元の名前一覧を取得
			std::unique_ptr<TStringList> org_lst(new TStringList());
			org_lst->Assign(ItemList);

			//重複回避のための中間改名
			if (duplicated) {
				SttPrgBar->Begin(_T("中間処理..."));
				//一時ファイルが残っていないかチェック
				std::unique_ptr<TStringList> t_lst(new TStringList());
				get_files(ExtractFilePath(ItemList->Strings[0]), _T("$~NF*.~TMP"), t_lst.get());
				if (t_lst->Count>0 &&
					msgbox_Sure(_T("一時ファイル($~NF*.~TMP)が残っていますが、削除してもよいですか?")))
						for (int i=0; i<t_lst->Count; i++) DeleteFile(t_lst->Strings[i]);
				//中間改名
				for (int i=0; i<ItemList->Count; i++) {
					SttPrgBar->SetPosI(i, ItemList->Count);
					Application->ProcessMessages();
					UnicodeString tmp_name = ExtractFilePath(ItemList->Strings[i]);
					tmp_name.cat_sprintf(_T("$~NF%04u.~TMP"), i);
					if (move_FileT(ItemList->Strings[i], tmp_name))
						ItemList->Strings[i] = tmp_name;
					else
						TextAbort(_T("中間処理に失敗、中断しました。"));
				}
				SttPrgBar->End();
			}

			//改名処理
			SttPrgBar->Begin(_T("名前変更..."));
			bool checked = OppPath.IsEmpty();
			std::unique_ptr<TStringList> r_lst(new TStringList());
			for (int i=0; i<ItemList->Count; i++) {
				SttPrgBar->SetPosI(i, ItemList->Count);
				Application->ProcessMessages();

				UnicodeString f_name   = ItemList->Strings[i];
				UnicodeString org_name = org_lst->Strings[i];
				UnicodeString new_name = NewNameList->Strings[i];
				msg = make_LogHdr(_T("RENAME"), org_name);
				//スキップ
				if (SameStr(f_name, new_name)) {
					if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
					sk_cnt++;
				}
				//改名
				else {
					msg.cat_sprintf(_T(" ---> %s"), ExtractFileName(new_name).c_str());
					for (;;) {
						if (move_FileT(f_name, new_name)) {
							ItemList->Strings[i] = new_name;
							UnicodeString log_str; log_str.sprintf(_T("%s\t%s"), org_name.c_str(), new_name.c_str());
							r_lst->Add(log_str);

							UnicodeString org_fext = get_extension(org_name);
							//コマンドファイルの改名を登録
							if (test_NbtExt(org_fext) && test_NbtExt(get_extension(new_name))) {
								RenCmdFileList->Add(log_str);
							}
							//アーカイブファイルの改名を登録
							if (test_ArcExt(org_fext) && (file_GetAttr(new_name) & faDirectory)==0) {
								RenArcFileList->Add(log_str);
							}
							//フォルダアイコンの改名
							rename_FolderIcon(org_name, new_name);
							//反対パスに反映されるかをチェック
							if (!checked && (file_GetAttr(new_name) & faDirectory)) {
								UnicodeString org_pnam = IncludeTrailingPathDelimiter(org_lst->Strings[i]);
								if (StartsText(org_pnam, OppPath)) {
									OppPath = IncludeTrailingPathDelimiter(new_name) + OppPath.Delete(1, org_pnam.Length());
									checked = true;
								}
							}
							ok_cnt++;
							break;
						}

						//失敗、再試行
						UnicodeString err_msg  = get_LogErrMsg(EmptyStr, false, f_name);
						UnicodeString caption = "改名に失敗しました\r\n" + err_msg;
						if (msgbox_Retry(caption, "エラー")==IDRETRY) continue;
						set_LogErrMsg(msg, err_msg);
						er_cnt++;
						break;
					}
				}

				if (!msg.IsEmpty()) AddLog(msg);
				Application->ProcessMessages();
			}

			//改名ログの保存
			if (!NoRenLogCheckBox->Checked && !saveto_TextUTF8(ExePath + RENLOG_FILE, r_lst.get()))
				throw EAbort(LoadUsrMsg(USTR_FaildSave, _T("改名ログ")));

			SttPrgBar->End();

			if (IsReplaceSheet()) {
				add_ComboBox_history(SrcStrComboBox);
				IniFile->SaveComboBoxItems(SrcStrComboBox,
							RegExCheckBox->Checked? _T("RenSrcPtnHistory") : _T("RenSrcStrHistory"));
				add_ComboBox_history(RepStrComboBox);
			}
			else if (IsRenListSheet()) {
				add_ComboBox_history(RenListComboBox);
			}
			else if (IsMp3Sheet()) {
				add_ComboBox_history(Mp3FmtComboBox);
			}

			AddLog(ModeStr + "終了" + get_res_cnt_str(ok_cnt, er_cnt, sk_cnt), true);
			if (er_cnt>0) throw EAbort(msg.sprintf(_T("%u個のファイルで改名に失敗しました。"), er_cnt));
		}

		//------------------------------------------
		//タイムスタンプの変更
		//------------------------------------------
		if (TimeChanged) {
			TDateTime dt;
			if (!str_to_DateTime(TimeMaskEdit->Text, &dt)) UserAbort(USTR_IllegalDate);
			SttPrgBar->Begin(_T("タイム変更..."));
			int er_cnt = 0;
			for (int i=0; i<ItemList->Count; i++) {
				SttPrgBar->SetPosI(i, ItemList->Count);
				Application->ProcessMessages();

				for (;;) {
					UnicodeString f_name   = ItemList->Strings[i];
					if (set_file_age(f_name, dt, ForceDel)) break;

					//失敗、再試行
					UnicodeString err_msg  = get_LogErrMsg(EmptyStr, false, f_name);
					UnicodeString caption = "タイムスタンプ設定に失敗しました\r\n" + err_msg;
					if (msgbox_Retry(caption, "エラー")==IDRETRY) continue;

					msg = make_LogHdr(_T("TIME"), ItemList->Strings[i]);
					set_LogErrMsg(msg, err_msg);
					AddLog(msg);
					er_cnt++;
					break;
				}
			}
			SttPrgBar->End();
			if (er_cnt>0) throw EAbort(msg.sprintf(_T("%u個のファイルでタイムスタンプ変更に失敗しました。"), er_cnt));
		}

		//------------------------------------------
		//属性の変更
		//------------------------------------------
		if (AttrChanged) {
			int er_cnt = 0;
			SttPrgBar->Begin(_T("属性変更..."));
			for (int i=0; i<ItemList->Count; i++) {
				SttPrgBar->SetPosI(i, ItemList->Count);
				Application->ProcessMessages();

				for (;;) {
					UnicodeString f_name = ItemList->Strings[i];
					int attr = file_GetAttr(f_name);
					if (ReadOnlyCheckBox->Tag==1) {
						if (ReadOnlyCheckBox->Checked)	attr |= faReadOnly; else attr &= ~faReadOnly;
					}
					if (HiddenCheckBox->Tag==1) {
						if (HiddenCheckBox->Checked)	attr |= faHidden;   else attr &= ~faHidden;
					}
					if (SysCheckBox->Tag==1) {
						if (SysCheckBox->Checked)		attr |= faSysFile;  else attr &= ~faSysFile;
					}
					if (ArcCheckBox->Tag==1) {
						if (ArcCheckBox->Checked)		attr |= faArchive;  else attr &= ~faArchive;
					}
					bool is_err = !file_SetAttr(f_name, attr);

					//圧縮
					if (!is_err && CmpCheckBox->Tag==1 && ((attr & faCompressed) != CmpCheckBox->Checked))
						is_err = !set_CompressAttr(f_name, CmpCheckBox->Checked);

					if (!is_err) break;

					//失敗、再試行
					UnicodeString err_msg  = get_LogErrMsg(EmptyStr, false, f_name);
					UnicodeString caption = "属性設定に失敗しました\r\n" + err_msg;
					if (msgbox_Retry(caption, "エラー")==IDRETRY) continue;

					msg = make_LogHdr(_T("ATTR"), f_name);
					set_LogErrMsg(msg, err_msg);
					AddLog(msg);
					er_cnt++;
					break;
				}
			}
			SttPrgBar->End();
			if (er_cnt>0) throw EAbort(msg.sprintf(_T("%u個のファイルで属性の変更に失敗しました。"), er_cnt));
		}
	}
	catch (EAbort &e) {
		SttPrgBar->End();
		if (!e.Message.IsEmpty()) msgbox_ERR(e.Message); else beep_Warn();
	}

	NamePageControl->Enabled = true;
	CommonPanel->Enabled	 = true;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenOkActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;

	bool time_ng = (TimeChanged && TimeMaskEdit->EditText.Pos('_'));

	if (IsOptionSheet()) {
		ap->Enabled = false;
	}
	else if (IsReplaceSheet()) {
		UnicodeString kwd = SrcStrComboBox->Text;
		bool reg_ng = RegExCheckBox->Checked && !kwd.IsEmpty() && !chk_RegExPtn(kwd);
		UnicodeString swd = SrcStrComboBox->Text;
		SrcStrComboBox->Color = reg_ng? (IsDarkMode? col_DkIlleg : col_Illegal) : get_WinColor(swd.IsEmpty());
		RepStrComboBox->Color = get_WinColor(swd.IsEmpty());
		ap->Enabled = !ExistErr && !reg_ng && !time_ng;
	}
	else {
		ap->Enabled = !ExistErr && !time_ng;
	}

	SrcStrComboBox->Tag =
		CBTAG_HISTORY | (SrcStrComboBox->Focused()? CBTAG_RGEX_V : 0) | (RegExCheckBox->Checked? CBTAG_RGEX_E : 0);

	PreviewBtn->Enabled = (!Previewed && !IsOptionSheet());
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RenListComboBoxChange(TObject *Sender)
{
	if (EditedList) return;

	RenListFile = to_absolute_name(RenListComboBox->Text);
	if (!LoadListFile()) del_ComboBox_history(RenListComboBox);
}
//---------------------------------------------------------------------------
//リストファイルの指定
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::RefListBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("リストファイルの指定"), F_FILTER_TXT, NULL);
	UnicodeString fnam;
	if (UserModule->OpenDlgToStr(fnam, true)) {
		RenListFile = to_absolute_name(fnam);
		add_ComboBox_history(RenListComboBox, fnam);
		LoadListFile();
	}
}
//---------------------------------------------------------------------------
//リストファイルの編集
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::EditListActionExecute(TObject *Sender)
{
	if (open_by_TextEditor(RenListFile)) {
		if (msgbox_Sure(_T("リストファイルを読み込みますか?"))) LoadListFile();
	}
	else msgbox_ERR(GlobalErrMsg);
}
//---------------------------------------------------------------------------
void __fastcall TRenameDlg::EditListActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = IsRenListSheet() && !RenListFile.IsEmpty();
}

//---------------------------------------------------------------------------
void __fastcall TRenameDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (ActiveControl!=TimeMaskEdit && SpecialKeyProc(this, Key, Shift)) return;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if		(USAME_TI(KeyStr, "Alt+R")) invert_CheckBox(ReadOnlyCheckBox);
	else if (USAME_TI(KeyStr, "Alt+H")) invert_CheckBox(HiddenCheckBox);
	else if (USAME_TI(KeyStr, "Alt+S")) invert_CheckBox(SysCheckBox);
	else if (USAME_TI(KeyStr, "Alt+A")) invert_CheckBox(ArcCheckBox);
	else if (USAME_TI(KeyStr, "Alt+C")) invert_CheckBox(CmpCheckBox);
	else if (USAME_TI(KeyStr, "Alt+V")) invert_CheckBox(AutoPrvCheckBox);
	else if (USAME_TI(KeyStr, "Alt+T")) set_focus_GroupBox(TimeGroupBox);
	else if (!IsOptionSheet()) {
		if		(USAME_TI(KeyStr, "Alt+M")) set_focus_RadioGroup(FbaseRadioGroup);
		else if (USAME_TI(KeyStr, "Alt+E")) set_focus_RadioGroup(FextRadioGroup);
		else if (USAME_TI(KeyStr, "Alt+U")) invert_CheckBox(CnvCharCheckBox);
		else if (USAME_TI(KeyStr, "Alt+K")) invert_CheckBox(CnvKanaCheckBox);
	}
}
//---------------------------------------------------------------------------

