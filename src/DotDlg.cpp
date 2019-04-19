//----------------------------------------------------------------------//
// NyanFi																//
//  .nyanfi ファイルの設定												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "CmdListDlg.h"
#include "DotDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDotNyanDlg *DotNyanDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TDotNyanDlg::TDotNyanDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	PathMaskComboBox->Tag = CBTAG_HISTORY;
	GrepMaskComboBox->Tag = CBTAG_HISTORY;

	UnicodeString hntstr = LoadUsrMsg(USTR_HintMltSepSC);
	PathMaskComboBox->Hint = hntstr;
	GrepMaskComboBox->Hint = hntstr;

	ColBufList	= new TStringList();
	InheritList = new TStringList();

	set_ComboBoxText(ColorComboBox,
		_T("Color_bgDirInf=ディレクトリ情報の背景色\n")
		_T("Color_fgDirInf=ディレクトリ情報の文字色\n")
		_T("Color_bgDrvInf=ドライブ情報の背景色\n")
		_T("Color_fgDrvInf=ドライブ情報の文字色\n")
		_T("Color_Cursor=ラインカーソルの色\n")
		_T("Color_selItem=選択項目の背景色\n"));

	SwatchPanel = new UsrSwatchPanel(this);
	SwatchPanel->Parent = this;
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::FormShow(TObject *Sender)
{
	SwatchPanel->SetPanelSize16x8(15);
	SwatchPanel->Left = ColGroupBox->Left + ColGroupBox->Width - SwatchPanel->Width;
	SwatchPanel->Top  = ColGroupBox->Top + ColGroupBox->Height;

	set_ComboBox_AutoComp(this);

	//タイトルバーを設定
	HFONT hFont = NULL;
	NONCLIENTMETRICS nonclientmetrics;
	nonclientmetrics.cbSize = sizeof(NONCLIENTMETRICS);
	if (::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, nonclientmetrics.cbSize, &nonclientmetrics, 0))
		hFont = ::CreateFontIndirect(&nonclientmetrics.lfCaptionFont);
	std::unique_ptr<TFont> titFont(new TFont());
	if (hFont) titFont->Handle = hFont; else titFont->Assign(DialogFont);
	Caption = get_MiniPathName(DotNyanName + " の設定", ClientWidth - ScaledInt(48), titFont.get(), true);

	IniFile->LoadComboBoxItems(PathMaskComboBox, _T("DotMaskHistory"));
	IniFile->LoadComboBoxItems(GrepMaskComboBox, _T("GrepMaskHistory"));

	ColBufList->Text =
		"Color_bgDirInf=\n"
		"Color_fgDirInf=\n"
		"Color_bgDrvInf=\n"
		"Color_fgDrvInf=\n"
		"Color_Cursor=\n"
		"Color_selItem=\n";

	NyanExists = file_exists(DotNyanName);
	if (NyanExists) {
		std::unique_ptr<TStringList> cfg_lst(new TStringList());
		load_text_ex(DotNyanName, cfg_lst.get());
		UnicodeString vstr = cfg_lst->Values["SortMode"];
		SortRadioGroup->ItemIndex = vstr.IsEmpty()? 0 : SortIdStr.Pos(vstr[1]);

		SetOderOption(cfg_lst.get());

		SoundEdit->Text			 = cfg_lst->Values["PlaySound"];
		BgImgEdit->Text			 = cfg_lst->Values["BgImage"];
		UnicodeString tmp		 = cfg_lst->Values["ExeCommands"];
		remove_top_AT(tmp);
		ExeCmdsEdit->Text		 = tmp;
		HandledCheckBox->Checked = ListVal_equal_1(cfg_lst.get(), _T("Handled"));
		HideCheckBox->Checked    = (file_GetAttr(DotNyanName) & faHidden);
	}
	else {
		SortRadioGroup->ItemIndex = 0;
		NoOderCheckBox->Checked	  = true;
		NoOderCheckBoxClick(NULL);

		HideRadioGroup->ItemIndex = 0;
		SysRadioGroup->ItemIndex  = 0;
		SizeRadioGroup->ItemIndex = 0;
		IconRadioGroup->ItemIndex = 0;
		SyncRadioGroup->ItemIndex = 0;

		PathMaskComboBox->Text	  = EmptyStr;
		GrepMaskComboBox->Text	  = EmptyStr;
		ListWdEdit->Text		  = EmptyStr;
		SoundEdit->Text 		  = EmptyStr;
		BgImgEdit->Text 		  = EmptyStr;
		DescEdit->Text			  = EmptyStr;
		ExeCmdsEdit->Text		  = EmptyStr;
		HandledCheckBox->Checked  = false;
	}

	//上位から強制的に継承内容を取得
	InheritList->Clear();
	UnicodeString inam = get_dotNaynfi(ExtractFilePath(DotNyanName), true, true);
	if (file_exists(inam)) {
		load_text_ex(inam, InheritList);
		InheritLabel->Caption = UnicodeString().sprintf(_T("[%s] から継承可能"),
									get_MiniPathName(ExtractFileDir(inam), 400, Font, true).c_str());
	}
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::FormDestroy(TObject *Sender)
{
	delete ColBufList;
	delete InheritList;
}

//---------------------------------------------------------------------------
//設定をコントロールに反映
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::SetOderOption(TStringList *lst)
{
	NoOderCheckBox->Checked = ListVal_is_empty(lst, _T("NaturalOrder")) &&
							  ListVal_is_empty(lst, _T("DscNameOrder")) &&
							  ListVal_is_empty(lst, _T("SmallOrder")) &&
							  ListVal_is_empty(lst, _T("OldOrder")) &&
							  ListVal_is_empty(lst, _T("DscAttrOrder"));
	NoOderCheckBoxClick(NULL);
	if (!NoOderCheckBox->Checked) {
		NaturalCheckBox->Checked = ListVal_equal_1(lst, _T("NaturalOrder"));
		DscNameCheckBox->Checked = ListVal_equal_1(lst, _T("DscNameOrder"));
		SmallCheckBox->Checked	 = ListVal_equal_1(lst, _T("SmallOrder"));
		OldCheckBox->Checked	 = ListVal_equal_1(lst, _T("OldOrder"));
		DscAttrCheckBox->Checked = ListVal_equal_1(lst, _T("DscAttrOrder"));
	}

	int n;
	n = get_ListIntVal(lst, _T("ShowHideAtr"),   -1);	HideRadioGroup->ItemIndex = (n==1)? 1 : (n==0)? 2 : 0;
	n = get_ListIntVal(lst, _T("ShowHideAtr"),   -1);	HideRadioGroup->ItemIndex = (n==1)? 1 : (n==0)? 2 : 0;
	n = get_ListIntVal(lst, _T("ShowSystemAtr"), -1);	SysRadioGroup->ItemIndex  = (n==1)? 1 : (n==0)? 2 : 0;
	n = get_ListIntVal(lst, _T("ShowByteSize"),  -1);	SizeRadioGroup->ItemIndex = (n==1)? 1 : (n==0)? 2 : 0;
	n = get_ListIntVal(lst, _T("ShowIcon"),		 -1);	IconRadioGroup->ItemIndex = (n==1)? 1 : (n==0)? 2 : (n==2)? 3 : 0;
	n = get_ListIntVal(lst, _T("SyncLR"),		 -1);	SyncRadioGroup->ItemIndex = (n==1)? 1 : (n==0)? 2 : 0;

	PathMaskComboBox->Text = lst->Values["PathMask"];
	GrepMaskComboBox->Text = lst->Values["GrepMask"];
	ListWdEdit->Text	   = lst->Values["ListWidth"];
	DescEdit->Text		   = lst->Values["Description"];

	SetColorOption(lst);
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::SetColorOption(TStringList *lst)
{
	TStringDynArray n_lst =
		SplitString("Color_bgDirInf|Color_fgDirInf|Color_bgDrvInf|Color_fgDrvInf|Color_Cursor|Color_selItem", "|");
	int idx = -1;
	for (int i=0; i<n_lst.Length; i++) {
		UnicodeString lbuf = lst->Values[n_lst[i]];
		ColBufList->Values[n_lst[i]] = lbuf;
		if ((TColor)lbuf.ToIntDef(clNone)!=clNone && idx==-1) idx = i;
	}

	ColorComboBox->ItemIndex = idx;
	ColorComboBox->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::NoOderCheckBoxClick(TObject *Sender)
{
	if (NoOderCheckBox->Checked) {
		NaturalCheckBox->Checked = FlOdrNatural[CurListTag];
		DscNameCheckBox->Checked = FlOdrDscName[CurListTag];
		SmallCheckBox->Checked	 = FlOdrSmall[CurListTag];
		OldCheckBox->Checked	 = FlOdrOld[CurListTag];
		DscAttrCheckBox->Checked = FlOdrDscAttr[CurListTag];
	}
}

//---------------------------------------------------------------------------
//サウンドの参照
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::RefSndBtnClick(TObject *Sender)
{
	UnicodeString inidir = def_if_empty(ExtractFileDir(to_absolute_name(SoundEdit->Text)), ExePath);
	UserModule->PrepareOpenDlg(_T("サウンドファイルの指定"), F_FILTER_WAV, NULL, inidir);
	if (UserModule->OpenDlgToEdit(SoundEdit, true)) play_sound(SoundEdit->Text);
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::TestSndBtnClick(TObject *Sender)
{
	play_sound(SoundEdit->Text);
}

//---------------------------------------------------------------------------
//背景画像の参照
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::RefImgBtnClick(TObject *Sender)
{
	UserModule->OpenImgDlg->Title = "背景画像の指定";
	UserModule->SetOpenImgFilter(BgImgEdit->Text);
	UserModule->OpenImgDlg->InitialDir = to_absolute_name(ExtractFilePath(BgImgEdit->Text), ExePath);
	UserModule->OpenImgDlg->FileName   = EmptyStr;
	UserModule->OpenImgDlgToEdit(BgImgEdit, true);
}

//---------------------------------------------------------------------------
//配色
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::ColorComboBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TComboBox *cp = (TComboBox*)Control;
	TCanvas   *cv = cp->Canvas;
	int yp = Rect.Top + get_TopMargin(cv);

	UnicodeString col_nam = cp->Items->Names[Index];
	UnicodeString vbuf	  = cp->Items->ValueFromIndex[Index];

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(Rect.Left + 34, yp, vbuf);

	//カラー
	TRect rc = Rect;  rc.Right = rc.Left + 30;
	cv->Brush->Color = (TColor)ColBufList->Values[col_nam].ToIntDef(clNone);
	if (cv->Brush->Color!=clNone) {
		cv->FillRect(rc);
	}
	else {
		cv->Brush->Color = scl_BtnFace;
		cv->FillRect(rc);
		out_Text(cv, rc.Left + 2, yp, _T("無効"), scl_WindowText);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::RefColBtnClick(TObject *Sender)
{
	TComboBox *cp = ColorComboBox;
	int idx = cp->ItemIndex;	if (idx==-1) return;

	UnicodeString col_nam = cp->Items->Names[idx];
	UserModule->ColorDlg->Color = (TColor)ColBufList->Values[col_nam].ToIntDef(clBlack);
	if (UserModule->ColorDlg->Execute()) {
		ColBufList->Values[col_nam] = IntToStr(UserModule->ColorDlg->Color);
		cp->Repaint();
	}
}
//---------------------------------------------------------------------------
//スポイト
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	TComboBox *cp = ColorComboBox;
	if (Button!=mbLeft || cp->ItemIndex==-1) return;

	//スウォッチブック
	SetCustomColToSwatch(UserModule->ColorDlg->CustomColors);
	SwatchPanel->Visible = true;
	SwatchPanel->BringToFront();

	TColor col_def = (TColor)ColBufList->Values[cp->Items->Names[cp->ItemIndex]].ToIntDef(clNone);
	UserModule->BeginSpuit(SpuitPanel, SpuitImage, col_def);
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::SpuitImageMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	SwatchPanel->Visible = false;

	if (UserModule->SpuitEnabled()) {
		TColor col = UserModule->EndSpuit();
		TComboBox *cp = ColorComboBox;
		if (cp->ItemIndex!=-1) {
			ColBufList->Values[cp->Items->Names[cp->ItemIndex]] = col;
			cp->Repaint();
		}
	}
}
//---------------------------------------------------------------------------
//配色の無効化
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::DisableColActionExecute(TObject *Sender)
{
	TComboBox *cp = ColorComboBox;
	if (cp->ItemIndex!=-1) {
		ColBufList->Values[cp->Items->Names[cp->ItemIndex]] = IntToStr(clNone);
		cp->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::DisableColActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (ColorComboBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//コマンドファイルの参照
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::RefCmdsBtnClick(TObject *Sender)
{
	if (CmdFileListDlg->ShowToSelect(get_cmdfile(ExeCmdsEdit->Text))==mrOk)
		ExeCmdsEdit->Text = to_relative_name(CmdFileListDlg->CmdFileName);
}

//---------------------------------------------------------------------------
//キャンセル
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::CancelBtnClick(TObject *Sender)
{
	//スポイトのキャンセル
	if (UserModule->SpuitEnabled()) {
		SwatchPanel->Visible = false;
		UserModule->EndSpuit();
		SpuitImage->Visible = true;
	}
	//閉じる
	else ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
//作成/変更
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::CreNyanActionExecute(TObject *Sender)
{
	UnicodeString lbuf;

	if (SortRadioGroup->ItemIndex>0)
		lbuf.cat_sprintf(_T("SortMode=%s\r\n"), SortIdStr.SubString(SortRadioGroup->ItemIndex, 1).c_str());

	if (!NoOderCheckBox->Checked) {
		lbuf.cat_sprintf(_T("NaturalOrder=%u\r\n"),	NaturalCheckBox->Checked? 1 : 0);
		lbuf.cat_sprintf(_T("DscNameOrder=%u\r\n"),	DscNameCheckBox->Checked? 1 : 0);
		lbuf.cat_sprintf(_T("SmallOrder=%u\r\n"),	SmallCheckBox->Checked  ? 1 : 0);
		lbuf.cat_sprintf(_T("OldOrder=%u\r\n"),		OldCheckBox->Checked    ? 1 : 0);
		lbuf.cat_sprintf(_T("DscAttrOrder=%u\r\n"),	DscAttrCheckBox->Checked? 1 : 0);
	}

	if (HideRadioGroup->ItemIndex>0) lbuf.cat_sprintf(_T("ShowHideAtr=%u\r\n"),   (HideRadioGroup->ItemIndex==1)? 1 : 0);
	if (SysRadioGroup->ItemIndex>0)  lbuf.cat_sprintf(_T("ShowSystemAtr=%u\r\n"), (SysRadioGroup->ItemIndex==1) ? 1 : 0);
	if (SizeRadioGroup->ItemIndex>0) lbuf.cat_sprintf(_T("ShowByteSize=%u\r\n"),  (SizeRadioGroup->ItemIndex==1)? 1 : 0);
	if (IconRadioGroup->ItemIndex>0) lbuf.cat_sprintf(_T("ShowIcon=%u\r\n"),	  (IconRadioGroup->ItemIndex==1)? 1 :
																				  (IconRadioGroup->ItemIndex==2)? 0 : 2);
	if (SyncRadioGroup->ItemIndex>0) lbuf.cat_sprintf(_T("SyncLR=%u\r\n"),		  (SyncRadioGroup->ItemIndex==1)? 1 : 0);

	if (!PathMaskComboBox->Text.IsEmpty())	{
		lbuf.cat_sprintf(_T("PathMask=%s\r\n"), PathMaskComboBox->Text.c_str());
		add_ComboBox_history(PathMaskComboBox, PathMaskComboBox->Text);
	}
	IniFile->SaveComboBoxItems(PathMaskComboBox, _T("DotMaskHistory"));

	if (!GrepMaskComboBox->Text.IsEmpty())	lbuf.cat_sprintf(_T("GrepMask=%s\r\n"), 	GrepMaskComboBox->Text.c_str());
	if (!ListWdEdit->Text.IsEmpty())		lbuf.cat_sprintf(_T("ListWidth=%s\r\n"),	ListWdEdit->Text.c_str());
	if (!SoundEdit->Text.IsEmpty())			lbuf.cat_sprintf(_T("PlaySound=%s\r\n"),	SoundEdit->Text.c_str());
	if (!BgImgEdit->Text.IsEmpty())			lbuf.cat_sprintf(_T("BgImage=%s\r\n"),		BgImgEdit->Text.c_str());
	if (!DescEdit->Text.IsEmpty())			lbuf.cat_sprintf(_T("Description=%s\r\n"),	DescEdit->Text.c_str());
	if (!ExeCmdsEdit->Text.IsEmpty())		lbuf.cat_sprintf(_T("ExeCommands=@%s\r\n"),	ExeCmdsEdit->Text.c_str());
	if (HandledCheckBox->Checked)			lbuf += "Handled=1\r\n";

	for (int i=0; i<ColBufList->Count; i++)
		if ((TColor)ColBufList->ValueFromIndex[i].ToIntDef(clNone)!=clNone)
			lbuf.cat_sprintf(_T("%s\r\n"), ColBufList->Strings[i].c_str());

	std::unique_ptr<TStringList> cfg_lst(new TStringList());
	cfg_lst->Text = lbuf;

	//隠し/システム属性だったら一旦解除してから保存・再設定
	int attr = NyanExists? file_GetAttr(DotNyanName) : faArchive;
	bool chg_atr = (attr & (faHidden | faSysFile));
	if (chg_atr) file_SetAttr(DotNyanName, faArchive);

	if (saveto_TextUTF8(DotNyanName, cfg_lst.get())) {
		if (HideCheckBox->Checked) attr |= faHidden; else attr &= ~faHidden;
		file_SetAttr(DotNyanName, attr);
		cursor_HourGlass(); Sleep(250); cursor_Default();
		ModalResult = mrOk;
	}
	else msgbox_ERR(LoadUsrMsg(USTR_FaildSave, get_dotNaynfi()));
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::CreNyanActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Caption = NyanExists? "変更" : "作成";

	NaturalCheckBox->Enabled = !NoOderCheckBox->Checked;
	DscNameCheckBox->Enabled = !NoOderCheckBox->Checked;
	SmallCheckBox->Enabled	 = !NoOderCheckBox->Checked;
	OldCheckBox->Enabled	 = !NoOderCheckBox->Checked;
	DscAttrCheckBox->Enabled = !NoOderCheckBox->Checked;

	ExeCmdsEdit->Enabled = PermitDotCmds;
	RefCmdsBtn->Enabled  = PermitDotCmds;
	HandledCheckBox->Enabled = ExeCmdsEdit->Text.IsEmpty();
	if (!ExeCmdsEdit->Text.IsEmpty()) HandledCheckBox->Checked = false;
	if (!UserModule->SpuitEnabled()) SpuitImage->Visible = (ColorComboBox->ItemIndex!=-1);

	bool inherit = (InheritList->Count>0);
	InhSndBtn->Enabled	= inherit;
	InhImgBtn->Enabled	= inherit;
	InhColBtn->Enabled	= inherit;
	InhCmdBtn->Enabled	= inherit;
	InheritBtn->Enabled = inherit;
}

//---------------------------------------------------------------------------
//継承
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::InhSndBtnClick(TObject *Sender)
{
	SoundEdit->Text = InheritList->Values["PlaySound"];
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::InhImgBtnClick(TObject *Sender)
{
	BgImgEdit->Text = InheritList->Values["BgImage"];
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::InhColBtnClick(TObject *Sender)
{
	SetColorOption(InheritList);
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::InhCmdBtnClick(TObject *Sender)
{
	UnicodeString tmp = InheritList->Values["ExeCommands"];
	remove_top_AT(tmp);
	ExeCmdsEdit->Text = tmp;
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::InheritBtnClick(TObject *Sender)
{
	UnicodeString vstr;
	vstr = InheritList->Values["SortMode"];		 SortRadioGroup->ItemIndex = vstr.IsEmpty()? 0 : SortIdStr.Pos(vstr[1]);

	SetOderOption(InheritList);

	HandledCheckBox->Checked = equal_1(InheritList->Values["Handled"]);
	HideCheckBox->Checked    = (file_GetAttr(DotNyanName) & faHidden);

	InhSndBtnClick(NULL);
	InhImgBtnClick(NULL);
	InhCmdBtnClick(NULL);
}

//---------------------------------------------------------------------------
//削除
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::DelNyanActionExecute(TObject *Sender)
{
	UnicodeString msg = make_LogHdr(_T("DELETE"), DotNyanName);
	try {
		if (msgbox_Sure(LoadUsrMsg(USTR_DeleteQ, get_dotNaynfi()), SureDelete)) {
			if (ForceDel && !set_FileWritable(DotNyanName))	Abort();
			if (!delete_File(DotNyanName, DelUseTrash))	 	Abort();
			AddLog(msg);
			cursor_HourGlass(); Sleep(250); cursor_Default();
			ModalResult = mrOk;
		}
	}
	catch (EAbort &e) {
		set_LogErrMsg(msg);
		AddLog(msg);
	}
}
//---------------------------------------------------------------------------
void __fastcall TDotNyanDlg::DelNyanActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = NyanExists;
}
//---------------------------------------------------------------------------
