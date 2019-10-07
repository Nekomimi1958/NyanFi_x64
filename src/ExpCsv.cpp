//----------------------------------------------------------------------//
// NyanFi																//
//  CSV/TSVエクスポート													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "ExpCsv.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TExpCsvDlg *ExpCsvDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TExpCsvDlg::TExpCsvDlg(TComponent* Owner)
	: TForm(Owner)
{
	Viewer = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	QuotCheckBox->Checked = IniFile->ReadBoolGen(_T("ExpCsvDlgQuot"),	true);

	set_ListBoxItemHi(SrcListBox);
	set_ListBoxItemHi(DstListBox);
	UserModule->InitializeListBox(DstListBox);
	UserModule->RefFileName = Viewer->FileName;

	SrcListBox->Clear();
	DstListBox->Clear();
	OutNameEdit->Text = EmptyStr;
	isTSV	= false;
	ExpBusy = false;

	if (Viewer && Viewer->TxtBufList->Count>0) {
		UnicodeString lbuf = Viewer->TxtBufList->Strings[0];
		isTSV = ContainsStr(lbuf, "\t");
		TStringDynArray hlst = isTSV? split_strings_tab(lbuf) : get_csv_array(lbuf, 99);
		if (hlst.Length<2) Abort();
		for (int i=0; i<hlst.Length; i++) SrcListBox->Items->AddObject(hlst[i], (TObject*)(NativeInt)i);
	}

	if (isTSV) TsvRadioBtn->Checked = true; else CsvRadioBtn->Checked = true;

	SetDarkWinTheme(this);
	SrcListBox->Color  = IsDarkMode? dcl_Window : scl_Window;
	DstListBox->Color = IsDarkMode? dcl_Window : scl_Window;
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->UninitializeListBox();
	UserModule->RefFileName = EmptyStr;

	IniFile->SavePosInfo(this);

	IniFile->WriteBoolGen(_T("ExpCsvDlgQuot"),	QuotCheckBox->Checked);

	Viewer = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::FormResize(TObject *Sender)
{
	SrcListBox->Width = (ClientWidth - MidPanel->Width) /2;
}

//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::ItemListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->Font->Color = get_TextColor(State.Contains(odSelected));
	cv->TextOut(Rect.Left + Scaled4, Rect.Top + get_TopMargin(cv), lp->Items->Strings[Index]);
}
//---------------------------------------------------------------------------
//項目の追加
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::AddItemActionExecute(TObject *Sender)
{
	int idx = SrcListBox->ItemIndex;
	DstListBox->Items->AddObject(SrcListBox->Items->Strings[idx], (TObject*)(NativeInt)idx);
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::AddItemActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (SrcListBox->ItemIndex != -1);
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::AddAllItemActionExecute(TObject *Sender)
{
	for (int i=0; i<SrcListBox->Count; i++)
		DstListBox->Items->AddObject(SrcListBox->Items->Strings[i], (TObject*)(NativeInt)i);
}

//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::RefOutNameBtnClick(TObject *Sender)
{
	UnicodeString fnam = Viewer->FileName;
	UserModule->PrepareSaveDlg(_T("出力ファイルの指定"),
		(TsvRadioBtn->Checked? F_FILTER_TSV : F_FILTER_CSV), fnam.c_str(), ExtractFileDir(fnam));
	fnam = UserModule->SaveDlgExecute();
	if (!fnam.IsEmpty()) OutNameEdit->Text = fnam;
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::SrcListBoxDblClick(TObject *Sender)
{
	AddItemAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::DstListBoxDblClick(TObject *Sender)
{
	UserModule->DelListItemAction->Execute();
}
//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::DstListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (UserModule->ListBoxOpeItem(get_KeyStr(Key, Shift))) Key = 0;
}

//---------------------------------------------------------------------------
//開始
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::ExportActionExecute(TObject *Sender)
{
	if (!Viewer) return;

	UnicodeString fnam = to_absolute_name(OutNameEdit->Text, ExtractFilePath(Viewer->FileName));
	if (file_exists(fnam) && !msgbox_Sure(USTR_OverwriteQ)) return;

	ExpBusy = true;
	ExportAction->Update();
	cursor_HourGlass();
	//読み込み
	std::unique_ptr<TStringList> i_buf(new TStringList());
	if (load_text_ex(Viewer->FileName, i_buf.get())==0) {
		msgbox_ERR(LoadUsrMsg(USTR_FaildLoad));
		return;
	}

	//ソート
	if (Viewer->SortMode!=0) {
		for (int i=0; i<i_buf->Count; i++) i_buf->Objects[i] = (TObject*)(NativeInt)i;
		i_buf->CustomSort(isTSV? comp_TsvNaturalOrder : comp_CsvNaturalOrder);
	}

	//作成
	std::unique_ptr<TStringList> o_buf(new TStringList());
	bool o_tsv = TsvRadioBtn->Checked;
	for (int i=0; i<i_buf->Count; i++) {
		UnicodeString lbuf;
		for (int j=0; j<DstListBox->Count; j++) {
			int idx = (int)DstListBox->Items->Objects[j];
			UnicodeString s = i_buf->Strings[i];
			s = isTSV? get_tsv_item(s, idx) : get_csv_item(s, idx);
			if (QuotCheckBox->Checked) s = make_csv_str(s);
			if (j==0)
				lbuf = s;
			else
				lbuf.cat_sprintf((o_tsv? _T("\t%s") : _T(",%s")), s.c_str());
		}
		o_buf->Add(lbuf);
	}
	cursor_Default();
	ExpBusy = false;

	//保存
	if (!saveto_TextFile(fnam, o_buf.get(), TxtViewer->TxtBufList->Encoding)) {
		msgbox_ERR(LoadUsrMsg(USTR_FaildSave, _T("出力ファイル")));
		return;
	}

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::ExportActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (!ExpBusy && DstListBox->Count>0 && !OutNameEdit->Text.IsEmpty());
}

//---------------------------------------------------------------------------
void __fastcall TExpCsvDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_TV) _T("#ExportCsv"));
}
//---------------------------------------------------------------------------

