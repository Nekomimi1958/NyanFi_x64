//----------------------------------------------------------------------//
// NyanFi																//
//  CSV/TSVレコード														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include <memory>
#include <Vcl.Clipbrd.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "FileInfDlg.h"
#include "GraphFrm.h"
#include "TxtViewer.h"
#include "CsvRecFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCsvRecForm *CsvRecForm;

//---------------------------------------------------------------------------
__fastcall TCsvRecForm::TCsvRecForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::FormCreate(TObject *Sender)
{
	GridScrPanel = new UsrScrollPanel(GridPanel, RecordGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::FormShow(TObject *Sender)
{
	DataList  = NULL;
	RecIndex  = DatIndex  = 0;
	ItemIndex = TxtViewer->CsvCol;
	HeaderStr = RecordStr = EmptyStr;
	IsCSV	  = false;

	IniFile->LoadPosInfo(this);

	TStringGrid *gp = RecordGrid;
	InitializeListGrid(gp, ViewerFont);
	InitializeListHeader(RecordHeader, _T("項目名|内容"));
	IniFile->LoadGridColWidth(gp, 2, 120,280);
	set_HeaderFromGrid(gp, RecordHeader);
	set_UsrScrPanel(GridScrPanel);

	TopIsHeaderCheckBox->Checked = TxtViewer->TopIsHeader;

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::WmFormShowed(TMessage &msg)
{
	UpdateRecord(NULL, RecIndex, ItemIndex);
	RecordGrid->Col = 1;
}

//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);
	IniFile->SaveGridColWidth(RecordGrid);

	TxtViewer->TopIsHeader = TopIsHeaderCheckBox->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::FormDestroy(TObject *Sender)
{
	delete GridScrPanel;
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::RecordHeaderDrawSection(THeaderControl *HeaderControl,
		THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::RecordHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(RecordGrid, RecordHeader);
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::RecordHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	set_GridFromHeader(RecordHeader, RecordGrid);
}

//---------------------------------------------------------------------------
//セルの描画
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::RecordGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	cv->Brush->Color = State.Contains(gdSelected)? col_selItem : col_bgList;
	cv->FillRect(Rect);

	UnicodeString cellstr = minimize_str(gp->Cells[ACol][ARow], gp->Canvas, gp->ColWidths[ACol] - 8, true);
	cv->Font->Color = is_SelFgCol(State)? col_fgSelItem : (ACol==0)? col_Headline : col_fgList;
	cv->TextOut(Rect.Left + 4, Rect.Top + get_TopMargin2(cv), cellstr);

	//区切り線
	cv->Pen->Width = 1;
	cv->Pen->Color = SelectWorB(cv->Brush->Color, 0.3);
	cv->MoveTo(Rect.Right, Rect.Top);  cv->LineTo(Rect.Right, Rect.Bottom);
}
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::RecordGridClick(TObject *Sender)
{
	if (RecordGrid->Enabled) {
		ItemIndex = RecordGrid->Row;
		TxtViewer->set_PosFromCol(ItemIndex);
	}
}

//---------------------------------------------------------------------------
//項目名を設定
//  戻り値: 項目数
//---------------------------------------------------------------------------
int __fastcall TCsvRecForm::UpdateItemName()
{
	if (!DataList || DataList->Count==0) return 0;

	int rec_no	= RecIndex + 1;
	int max_rec = DataList->Count;
	if (TopIsHeaderCheckBox->Checked) { rec_no--; max_rec--; }

	UnicodeString tmp;
	tmp.sprintf(_T("%sレコード"), IsCSV? _T("CSV") : _T("TSV"));
	Caption = (rec_no>0)? tmp.cat_sprintf(_T(" - [%u/%u]"), rec_no, max_rec) : tmp.cat_sprintf(_T(" - [_/%u]"), max_rec);

	int itm_count = 0;
	if (!HeaderStr.IsEmpty()) {
		TStringDynArray hdr_buf = IsCSV? get_csv_array(HeaderStr, 99) : split_strings_tab(HeaderStr);
		itm_count = hdr_buf.Length;
		TStringGrid *gp = RecordGrid;
		for (int i=0; i<itm_count; i++) {
			if (i<gp->RowCount)
				gp->Cells[0][i] = TopIsHeaderCheckBox->Checked?
									hdr_buf[i] : UnicodeString().sprintf(_T("項目%u"), i + 1);
		}
	}
	return itm_count;
}

//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::TopIsHeaderCheckBoxClick(TObject *Sender)
{
	UpdateItemName();
	TxtViewer->TopIsHeader = TopIsHeaderCheckBox->Checked;
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//内容の更新
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::UpdateRecord(
	TStringList *lst,
	int r_idx,			//レコードインデックス	(default = -1)
	int i_idx)			//項目インテリア		(default = -1)
{
	if (lst) DataList = lst;
	TStringGrid *gp = RecordGrid;
	gp->Enabled = false;

	try {
		if (!DataList || DataList->Count==0) Abort();

		RecIndex = (r_idx>=0 && r_idx<DataList->Count)? r_idx : 0;
		DatIndex = DataList->IndexOfObject((TObject*)(NativeInt)r_idx);
		if (DatIndex==-1) DatIndex = 0;

		HeaderStr = DataList->Strings[0];
		RecordStr = DataList->Strings[DatIndex];
		IsCSV	  = !contains_s(HeaderStr, _T('\t'));

		int org_row = gp->Row;
		int org_col = gp->Col;
		if (HeaderStr.IsEmpty() && RecordStr.IsEmpty()) gp->RowCount = 1;	//選択状態クリアのため

		int itm_count = UpdateItemName();	if (itm_count==0) Abort();
		gp->RowCount = itm_count;
		//内容
		TStringDynArray rec_buf = IsCSV? get_csv_array(RecordStr, MAX_CSV_ITEM) : split_strings_tab(RecordStr);
		for (int i=0; i<gp->RowCount; i++) gp->Cells[1][i] = (i<rec_buf.Length)? rec_buf[i] : EmptyStr;

		if (org_row<gp->RowCount) gp->Row = (i_idx>=0 && i_idx<gp->RowCount)? i_idx : org_row;
		if (org_col<gp->ColCount) gp->Col = org_col;
	}
	catch (...) {
		gp->RowCount = 1;
		clear_GridRow(gp, 0);
	}

	gp->Enabled = true;
	GridScrPanel->UpdateKnob();
	Application->MainForm->SetFocus();
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::CopyItemClick(TObject *Sender)
{
	UnicodeString copy_str;
	TStringGrid *gp = RecordGrid;
	for (int i=gp->Selection.Top; i<=gp->Selection.Bottom; i++) {
		if (gp->Selection.Left!=gp->Selection.Right)
			copy_str.cat_sprintf(_T("%s\t%s\r\n"), gp->Cells[0][i].c_str(), gp->Cells[1][i].c_str());
		else
			copy_str.cat_sprintf(_T("%s\r\n"), gp->Cells[gp->Selection.Left][i].c_str());
	}
	copy_to_Clipboard(copy_str);
	UpdateRecord(NULL, RecIndex);
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//項目の集計
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::CalcBtnClick(TObject *Sender)
{
	FileInfoDlg->isCalcItem  = true;
	FileInfoDlg->DataList	 = DataList;
	FileInfoDlg->TopIsHeader = TopIsHeaderCheckBox->Checked;
	FileInfoDlg->CsvCol 	 = ItemIndex;
	FileInfoDlg->ShowModal();
}
//---------------------------------------------------------------------------
//項目のグラフ表示
//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::GraphBtnClick(TObject *Sender)
{
	GraphForm->DataList    = DataList;
	GraphForm->TopIsHeader = TopIsHeaderCheckBox->Checked;
	GraphForm->CsvCol	   = ItemIndex;
	GraphForm->ShowModal();
}

//---------------------------------------------------------------------------
void __fastcall TCsvRecForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key==VK_ESCAPE) Close(); else Application->MainForm->SetFocus();
}
//---------------------------------------------------------------------------

