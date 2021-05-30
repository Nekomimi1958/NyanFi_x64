//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビュアーの配色												//
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
		"bgView=背景色\n"
		"fgView=文字色\n"
		"Margin=余白白\n"
		"bgRuler=ルーラの背景色\n"
		"fgRuler=ルーラの目盛色\n"
		"Cursor=ラインカーソルの色\n"
		"selItem=選択項目の背景色\n"
		"fgSelItem=選択項目の文字色\n"
		"bgLineNo=行番号背景色\n"
		"LineNo=行番号文字色\n"
		"Mark=行マーク\n"
		"bdrLine=行番号境界線\n"
		"bdrFold=折り返し境界線\n"
		"bdrFixed=固定長表示の縦罫線\n"
		"Comment=|コメントの文字色\n"
		"Strings=文字列の文字色\n"
		"Reserved=予約語の文字色\n"
		"Symbol=シンボルの文字色\n"
		"Numeric=数値の文字色\n"
		"fgEmpBin1=バイナリ強調文字色1\n"
		"fgEmpBin2=バイナリ強調文字色2\n"
		"fgEmpBin3=バイナリ強調文字色3\n"
		"Headline=見出しの文字色\n"
		"Ruby=ルビ\n"
		"URL=URLの文字色\n"
		"LocalLink=ローカルファイルへのリンク\n"
		"fgEmp=強調文字色\n"
		"bgEmp=強調背景色\n"
		"TAB=タブ表示色\n"
		"CR=改行表示色\n"
		"HR=横罫線の色\n"
		"Ctrl=コントロールコード\n"
		"Folder=ディレクトリの文字色\n"
		"Error=エラーの文字色\n";

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
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ColorListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	draw_ColorListBox((TListBox*)Control, Rect, Index, State, ColBufList);
}

//---------------------------------------------------------------------------
//色の参照
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
//スポイト
//---------------------------------------------------------------------------
void __fastcall TColorDlg::SpuitImageMouseDown(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	TListBox *lp = ColorListBox;
	if (Button!=mbLeft || lp->ItemIndex==-1) return;

	//スウォッチブック
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
//インポート
//---------------------------------------------------------------------------
void __fastcall TColorDlg::InportBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("配色のインポート"), F_FILTER_INI, _T("*.INI"));
	UnicodeString fnam = UserModule->OpenDlgExecute();
	if (!fnam.IsEmpty()) {
		std::unique_ptr<UsrIniFile> inp_file(new UsrIniFile(fnam));
		inp_file->ReadSection("Color", ColBufList);
		ColorListBox->Repaint();
	}
	SetFocus();
}
//---------------------------------------------------------------------------
//エクスポート
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ExportBtnClick(TObject *Sender)
{
	UserModule->PrepareSaveDlg(_T("配色のエクスポート"), F_FILTER_INI, FileName.c_str());
	UnicodeString fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) {
		std::unique_ptr<UsrIniFile> exp_file(new UsrIniFile(fnam));
		exp_file->AssignSection("Color", ColBufList);
		if (!exp_file->UpdateFile(true)) msgbox_ERR(USTR_FaildSave);
	}
	SetFocus();
}
//---------------------------------------------------------------------------
//全体に反映
//---------------------------------------------------------------------------
void __fastcall TColorDlg::OptApplyBtnClick(TObject *Sender)
{
	if (msgbox_Sure(_T("全体のオプション設定に反映させますか？"))) {
		ObjViewer->SetColor(ColBufList);
		ObjViewer->SetOptColor();
		ObjViewer->Repaint(true);
	}
}
//---------------------------------------------------------------------------
//適用
//---------------------------------------------------------------------------
void __fastcall TColorDlg::ApplyBtnClick(TObject *Sender)
{
	ObjViewer->SetColor(ColBufList);
	ObjViewer->Repaint(true);
}

//---------------------------------------------------------------------------
//確定
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
//キャンセル
//---------------------------------------------------------------------------
void __fastcall TColorDlg::CancelBtnClick(TObject *Sender)
{
	//スポイトのキャンセル
	if (UserModule->SpuitEnabled()) {
		SwatchPanel->Visible = false;
		UserModule->EndSpuit();
		SpuitImage->Visible = true;
	}
	//閉じる
	else {
		ModalResult = mrCancel;
	}
}

//---------------------------------------------------------------------------
//配色の無効化
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
//ヘルプ
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

