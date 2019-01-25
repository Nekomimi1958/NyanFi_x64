//----------------------------------------------------------------------//
// NyanFi																//
//  グラフ表示															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include "UserFunc.h"
#include "Global.h"
#include "TxtViewer.h"
#include "GraphFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGraphForm *GraphForm;

//---------------------------------------------------------------------------
__fastcall TGraphForm::TGraphForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormCreate(TObject *Sender)
{
	ValueList = new TStringList();

	CsvCol	 = -1;
	DataList = NULL;
	f_Width  = 0;
	x_Margin = ScaledInt(12);
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	ShowMaxAction->Checked = IniFile->ReadBoolGen(_T("GraphShowMax"),	true);
	ShowAveAction->Checked = IniFile->ReadBoolGen(_T("GraphShowAve"),	true);
	ShowMinAction->Checked = IniFile->ReadBoolGen(_T("GraphShowMin"),	true);

	for (int i=0; i<ValueList->Count; i++) delete (long double*)ValueList->Objects[i];
	ValueList->Clear();

	MinItemValue = MaxItemValue = 0;

	try {
		if (!DataList) Abort();

		UnicodeString top_str = DataList->Strings[0];
		bool IsCSV = !contains_s(top_str, _T('\t'));
		TStringDynArray top_buf = IsCSV? get_csv_array(top_str, 99) : split_strings_tab(top_str);
		if (CsvCol==-1) CsvCol = 0;
		if (CsvCol<0 || CsvCol>=top_buf.Length) Abort();

		UnicodeString tmp = UnicodeString(IsCSV? "CSV" : "TSV").UCAT_T("項目のグラフ - [");
		if (TopIsHeader)
			tmp += IsCSV? get_csv_item(top_str, CsvCol) : get_tsv_item(top_str, CsvCol);
		else
			tmp.cat_sprintf(_T("項目%u"), CsvCol + 1);
		Caption = tmp.UCAT_T("]");

		long double total = 0.0;
		f_Width = 0;
		for (int i=(TopIsHeader? 1 : 0); i<DataList->Count; i++) {
			UnicodeString lbuf = DataList->Strings[i];
			UnicodeString istr = extract_top_num_str(IsCSV? get_csv_item(lbuf, CsvCol) : get_tsv_item(lbuf, CsvCol));
			if (istr.IsEmpty()) continue;
			try {
				long double r = istr.ToDouble();
				total += r;
				if (ValueList->Count==0) {
					MinItemValue = MaxItemValue = r;
				}
				else {
					MinItemValue = std::min(MinItemValue, r);
					MaxItemValue = std::max(MaxItemValue, r);
				}
				long double *dp = new long double;
				*dp = r;
				ValueList->AddObject(IntToStr(i), (TObject*)dp);

				int p = istr.Pos('.');
				if (p) f_Width = std::max(f_Width, istr.Length() - p);
			}
			catch (...) {
				;
			}
		}

		if (ValueList->Count==0) Abort();

		AveItemValue = total/ValueList->Count;
	}
	catch (...) {
		msgbox_ERR("有効な数値項目がありません");
		::PostMessage(Handle, WM_CLOSE, 0, 0);
	}
}

//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteBoolGen(_T("GraphShowMax"),	ShowMaxAction->Checked);
	IniFile->WriteBoolGen(_T("GraphShowAve"),	ShowAveAction->Checked);
	IniFile->WriteBoolGen(_T("GraphShowMin"),	ShowMinAction->Checked);

	for (int i=0; i<ValueList->Count; i++) delete (long double*)ValueList->Objects[i];
	ValueList->Clear();

	CsvCol	 = -1;
	DataList = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormDestroy(TObject *Sender)
{
	delete ValueList;
}

//---------------------------------------------------------------------------
//グラフの描画
//---------------------------------------------------------------------------
void __fastcall TGraphForm::PaintBox1Paint(TObject *Sender)
{
	TPaintBox *pp = (TPaintBox*)Sender;
	TCanvas *cv = pp->Canvas;
	cv->Brush->Color = col_GrBack;
	cv->FillRect(PaintBox1->ClientRect);

	cv->Brush->Color = AdjustColor(col_GrBack, 32);
	TRect rc = PaintBox1->ClientRect;
	rc.Right = x_Margin;
	cv->FillRect(rc);
	rc = PaintBox1->ClientRect;
	rc.Left = rc.Right - x_Margin;
	cv->FillRect(rc);

	cv->Pen->Style = psSolid;
	cv->Pen->Width = 1;
	cv->Pen->Color = col_GrGrid;
	cv->Font->Assign(ListFont);
	cv->Font->Height = ScaledInt(12);
	cv->Font->Color  = col_GrText;
	cv->Brush->Style = bsClear;

	cv->MoveTo(x_Margin, 0);
	cv->LineTo(x_Margin, pp->Height);
	cv->MoveTo(pp->Width - x_Margin, 0);
	cv->LineTo(pp->Width - x_Margin, pp->Height);

	//最大値
	int y_hi = pp->Height*0.1;	//上限
	if (ShowMaxAction->Checked) {
		cv->MoveTo(x_Margin, y_hi);	cv->LineTo(pp->Width - x_Margin, y_hi);
		cv->TextOut(x_Margin + ScaledInt(4), y_hi - ScaledInt(16),	"最大値=" + ldouble_to_str(MaxItemValue, f_Width));
	}
	//最小値
	int y_lo = pp->Height*0.9;	//下限
	if (ShowMinAction->Checked) {
		cv->MoveTo(x_Margin, y_lo);	cv->LineTo(pp->Width - x_Margin, y_lo);
		cv->TextOut(x_Margin + ScaledInt(4), y_lo + ScaledInt(4),	"最小値=" + ldouble_to_str(MinItemValue, f_Width));
	}

	//グラフ
	int y_w = pp->Height*0.8;
	int x_w = pp->Width - 2*x_Margin;
	long double r_w = MaxItemValue - MinItemValue;
	cv->Pen->Color = col_GrLine;
	for (int i=0; i<ValueList->Count; i++) {
		int idx = ValueList->Strings[i].ToIntDef(0);
		long double v = *(long double*)ValueList->Objects[i];
		int yp = y_lo - (y_w * ((v - MinItemValue) / r_w));
		int xp = x_Margin + x_w * (1.0 * idx/DataList->Count);
		if (i==0) cv->MoveTo(xp, yp); else cv->LineTo(xp, yp);
	}

	//平均値
	if (ShowAveAction->Checked) {
		int y_av = y_lo - (y_w * ((AveItemValue - MinItemValue) / r_w));
		cv->Pen->Color = col_GrGrid;
		cv->MoveTo(x_Margin, y_av);	cv->LineTo(pp->Width - x_Margin, y_av);
		cv->TextOut(x_Margin + ScaledInt(4), y_av - ScaledInt(16),	"平均値=" + ldouble_to_str(AveItemValue, f_Width + 1));
	}
}

//---------------------------------------------------------------------------
//項目位置に移動
//---------------------------------------------------------------------------
void __fastcall TGraphForm::PaintBox1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	if (Button==mbLeft) {
		TPaintBox *pp = (TPaintBox*)Sender;
		int idx = (int)(1.0 * (X - x_Margin)/(pp->Width - x_Margin*2) * (DataList->Count-1) + 0.5);
		idx = std::min(std::max(idx, TopIsHeader? 1 : 0), DataList->Count-1);
		TxtViewer->ToLine(idx + 1, CsvCol);
	}
}

//---------------------------------------------------------------------------
//画像としてコピー
//---------------------------------------------------------------------------
void __fastcall TGraphForm::CopyImgItemClick(TObject *Sender)
{
	TRect rc = PaintBox1->BoundsRect;
	int w = rc.Width();
	int h = rc.Height();
	std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
	::BitBlt(bmp->Canvas->Handle, 0, 0, w, h, PaintBox1->Canvas->Handle, rc.left, rc.top, SRCCOPY|CAPTUREBLT);
	copy_to_Clipboard(bmp.get());
}

//---------------------------------------------------------------------------
//最大値/平均値/最小値の表示
//---------------------------------------------------------------------------
void __fastcall TGraphForm::ShowInfActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;
	PaintBox1->Repaint();
}

//---------------------------------------------------------------------------
void __fastcall TGraphForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_TV) _T("#CsvGraph"));
}
//---------------------------------------------------------------------------



