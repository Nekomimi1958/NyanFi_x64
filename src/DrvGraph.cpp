//----------------------------------------------------------------------//
//  ドライブ使用率推移													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <System.Math.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "DrvGraph.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDriveGraph *DriveGraph =  NULL;

//---------------------------------------------------------------------------
__fastcall TDriveGraph::TDriveGraph(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormCreate(TObject *Sender)
{
	DataList = new TStringList();

	for (int i=1; i<=16; i++) SizeComboBox->Items->Add(IntToStr(i));
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormShow(TObject *Sender)
{
	DlgInitialized = false;

	IniFile->LoadPosInfo(this, DialogCenter);

	OldOdrCheckBox->Checked = IniFile->ReadBoolGen(_T("DrvGrpahOldOdr"));
	MinMaxCheckBox->Checked = IniFile->ReadBoolGen(_T("DrvGrpahMinMax"));

	BarSize = IniFile->ReadIntGen( _T("DrvGrpahBarSize"), 8);
	if (BarSize<1 || BarSize>16) BarSize = 8;
	SizeComboBox->ItemIndex = BarSize - 1;
	GraphTopX = Canvas->TextWidth("9999/99/99 ") + 4;

	DriveName = DriveName.SubString(1, 1);
	DriveComboBox->Clear();
	int d_idx = -1;
	for (int i=0; i<DriveInfoList->Count; i++) {
		drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
		if (!dp->accessible || dp->drv_type==DRIVE_CDROM) continue;
		int idx = DriveComboBox->Items->Add(
			UnicodeString().sprintf(_T("%s %s"), get_tkn(dp->drive_str, '\\').c_str(), dp->volume.c_str()));
		if (StartsText(DriveName, dp->drive_str)) d_idx = idx;
	}

	DataList->Clear();
	if (d_idx!=-1) {
		DriveComboBox->ItemIndex = d_idx;
		get_DriveLogList(DriveName, DataList, MinUsed, MaxUsed);
	}
	else {
		DriveComboBox->ItemIndex = -1;
	}

	DlgInitialized = true;

	SizeComboBoxChange(NULL);
	SetStatus(DataList->Count - 1);
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("DrvGrpahOldOdr"),		OldOdrCheckBox->Checked);
	IniFile->WriteBoolGen(_T("DrvGrpahMinMax"),		MinMaxCheckBox->Checked);
	IniFile->WriteIntGen( _T("DrvGrpahBarSize"),	BarSize);
	DataList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormDestroy(TObject *Sender)
{
	delete DataList;
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormResize(TObject *Sender)
{
	PaintBox1->Invalidate();
}

//---------------------------------------------------------------------------
//描画
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::PaintBox1Paint(TObject *Sender)
{
	TCanvas *cv = PaintBox1->Canvas;

	cv->Brush->Color = col_bgList;
	cv->FillRect(PaintBox1->ClientRect);

	if (DataList->Count>0) {
		int xw	= GraphScrollBox->ClientWidth - GraphTopX - 1;
		int x25 = GraphTopX + xw * 0.25;
		int x50 = GraphTopX + xw * 0.50;
		int x75 = GraphTopX + xw * 0.75;
		int x_min = GraphTopX + xw * MinUsed;
		int x_max = GraphTopX + xw * MaxUsed;

		TColor col_grid = AdjustColor(col_bgPrgBar, 32);
		cv->Pen->Width = Scaled1;
		cv->Font->Assign(Font);

		int yt	= 0;
		int ym	= cv->TextHeight("9") * 1.5;	//***
		int idx = OldOdrCheckBox->Checked? 0 : DataList->Count - 1;
		for (int i=0,yp=0; i<DataList->Count; i++,yp+=BarSize) {
			UnicodeString lbuf = DataList->Strings[idx];
			idx += OldOdrCheckBox->Checked? 1 : -1;
			//日付
			if (yp>=yt) {
				cv->Font->Color = col_fgList;
				cv->TextOut(4, yp, get_csv_item(lbuf, 0));
				yt = yp + ym;
			}
			//グラフ
			__int64 used_sz = StrToInt64Def(get_csv_item(lbuf, 2), -1);
			__int64 free_sz = StrToInt64Def(get_csv_item(lbuf, 3), -1);
			if (used_sz>=0 && free_sz>=0) {
				double r = 1.0 * used_sz / (used_sz + free_sz);;
				draw_BarGraph(cv, Rect(GraphTopX, yp, GraphTopX + xw, yp + BarSize), r);
			}
			//グリッド
			cv->Pen->Color = col_grid;
			cv->MoveTo(x25, yp);  cv->LineTo(x25, yp + BarSize);
			cv->MoveTo(x50, yp);  cv->LineTo(x50, yp + BarSize);
			cv->MoveTo(x75, yp);  cv->LineTo(x75, yp + BarSize);
			//Min/Max ライン
			if (MinMaxCheckBox->Checked) {
				cv->Pen->Color = clBlue;
				cv->MoveTo(x_min, yp);  cv->LineTo(x_min, yp + BarSize);
				cv->Pen->Color = clRed;
				cv->MoveTo(x_max, yp);  cv->LineTo(x_max, yp + BarSize);
			}
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::SetStatus(int idx)
{
	UnicodeString msg;
	if (idx>=0 && idx<DataList->Count) {
		UnicodeString lbuf = DataList->Strings[idx];
		msg = get_csv_item(lbuf, 0);

		__int64 used_sz = StrToInt64Def(get_csv_item(lbuf, 2), -1);
		__int64 free_sz = StrToInt64Def(get_csv_item(lbuf, 3), -1);
		if (used_sz>=0 && free_sz>=0) {
			//使用
			msg.cat_sprintf(_T("  %4.1f%% %s Used"),
					100.0 * used_sz / (used_sz + free_sz), get_size_str_T(used_sz, 1).c_str());
			//差分
			if (idx>0) {
				__int64 used_yd = StrToInt64Def(get_csv_item(DataList->Strings[idx - 1], 2), -1);
				if (used_sz>=used_yd)
					msg.cat_sprintf(_T("(+%s)"), get_size_str_T(used_sz - used_yd, 1).c_str());
				else
					msg.cat_sprintf(_T("(-%s)"), get_size_str_T(used_yd - used_sz, 1).c_str());
			}
			//空き
			msg.cat_sprintf(_T("/ %s Free"), get_size_str_T(free_sz, 1).c_str());
		}
	}
	StatusBar1->SimpleText = msg;
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::PaintBox1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	if (DataList->Count>0 && GraphTopX<X) {
		int p = Y/BarSize;
		SetStatus(OldOdrCheckBox->Checked? p : DataList->Count - 1 - p);
	}
}

//---------------------------------------------------------------------------
//ドライブの変更
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::DriveComboBoxChange(TObject *Sender)
{
	if (!DlgInitialized) return;

	DataList->Clear();
	int idx = DriveComboBox->ItemIndex;
	if (idx!=-1) {
		get_DriveLogList(get_tkn(DriveComboBox->Text, ':'), DataList, MinUsed, MaxUsed);
		GraphScrollBox->VertScrollBar->Range = DataList->Count * BarSize + 20;
		PaintBox1->Invalidate();
		SetStatus(DataList->Count - 1);
	}
}
//---------------------------------------------------------------------------
//棒幅
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::SizeComboBoxChange(TObject *Sender)
{
	if (!DlgInitialized) return;

	BarSize = SizeComboBox->ItemIndex + 1;
	GraphScrollBox->VertScrollBar->Range = DataList->Count * BarSize + 20;
	PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::OptionChanged(TObject *Sender)
{
	if (!DlgInitialized) return;
	PaintBox1->Invalidate();
}

//---------------------------------------------------------------------------
//ホイール操作
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
	TPoint &MousePos, bool &Handled)
{
	//ズーム
	if (SameText(get_ShiftStr(Shift), KeyStr_Ctrl)) {
		int last_sz = BarSize;
		if (WheelDelta<0 && BarSize>1)  --BarSize;
		if (WheelDelta>0 && BarSize<16) ++BarSize;
		if (BarSize!=last_sz) {
			SizeComboBox->ItemIndex = BarSize - 1;
			SizeComboBoxChange(NULL);
		}
	}
	//スクロール
	else {
		GraphScrollBox->VertScrollBar->Position -= Sign(WheelDelta) * (std::max(BarSize*4, 8));
	}

	Handled = true;
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

