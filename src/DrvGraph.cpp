//----------------------------------------------------------------------//
// NyanFi																//
//  ドライブ使用率推移													//
//----------------------------------------------------------------------//
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
	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	DataList = new TStringList();

	for (int i=1; i<=16; i++) SizeComboBox->Items->Add(IntToStr(i));
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormShow(TObject *Sender)
{
	DlgInitialized = false;
	SttIndex = -1;

	IniFile->LoadPosInfo(this, DialogCenter);

	setup_ToolBar(OptToolBar);
	DriveComboBox->Width  = IniFile->ReadIntGen( _T("DrvGraphSlctrWidth"),	120);
	OldOdrAction->Checked = IniFile->ReadBoolGen(_T("DrvGraphOldOdr"));
	MinMaxAction->Checked = IniFile->ReadBoolGen(_T("DrvGraphMinMax"));
	CursorAction->Checked = IniFile->ReadBoolGen(_T("DrvGraphCursor"));

	StatusBar1->Font->Assign(SttBarFont);

	BarSize = IniFile->ReadIntGen( _T("DrvGraphBarSize"), 8);
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
			UnicodeString().sprintf(_T("%s %s"), get_tkn(dp->drive_str, '\\').c_str(), dp->label.c_str()));
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

	SetDarkWinTheme(this);

	DlgInitialized = true;

	SizeComboBoxChange(NULL);
	SetStatus(DataList->Count - 1);
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen( _T("DrvGraphSlctrWidth"),	DriveComboBox->Width);
	IniFile->WriteBoolGen(_T("DrvGraphOldOdr"),		OldOdrAction);
	IniFile->WriteBoolGen(_T("DrvGraphMinMax"),		MinMaxAction);
	IniFile->WriteBoolGen(_T("DrvGraphCursor"),		CursorAction);
	IniFile->WriteIntGen( _T("DrvGraphBarSize"),	BarSize);
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
void __fastcall TDriveGraph::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
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
		int x_cur = -1;
		if (SttIndex>=0 && SttIndex<DataList->Count) {
			UnicodeString lbuf = DataList->Strings[SttIndex];
			__int64 used_sz = StrToInt64Def(get_csv_item(lbuf, 2), -1);
			__int64 free_sz = StrToInt64Def(get_csv_item(lbuf, 3), -1);
			if (used_sz>=0 && free_sz>=0) x_cur = GraphTopX + xw * (1.0 * used_sz / (used_sz + free_sz));
		}

		TColor col_grid = AdjustColor(col_bgPrgBar, 32);
		cv->Pen->Width	= SCALED_THIS(1);
		cv->Font->Assign(Font);

		int yt	= 0;
		int ym	= cv->TextHeight("9") * 1.5;	//***
		int idx = OldOdrAction->Checked? 0 : DataList->Count - 1;
		for (int i=0,yp=0; i<DataList->Count; i++,yp+=BarSize) {
			UnicodeString lbuf = DataList->Strings[idx];
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
				double r = 1.0 * used_sz / (used_sz + free_sz);
				draw_BarGraph(cv, Rect(GraphTopX, yp, GraphTopX + xw, yp + BarSize), r);
			}
			//グリッド
			cv->Pen->Color = col_grid;
			cv->MoveTo(x25, yp);  cv->LineTo(x25, yp + BarSize);
			cv->MoveTo(x50, yp);  cv->LineTo(x50, yp + BarSize);
			cv->MoveTo(x75, yp);  cv->LineTo(x75, yp + BarSize);
			//Min/Max ライン
			if (MinMaxAction->Checked) {
				cv->Pen->Color = clBlue;
				cv->MoveTo(x_min, yp);  cv->LineTo(x_min, yp + BarSize);
				cv->Pen->Color = clRed;
				cv->MoveTo(x_max, yp);  cv->LineTo(x_max, yp + BarSize);
			}
			//Cur ライン
			if (CursorAction->Checked && x_cur!=-1) {
				cv->Pen->Color = col_Cursor;
				cv->MoveTo(x_cur, yp);  cv->LineTo(x_cur, yp + BarSize);
			}

			//ラインカーソル
			if (HiddenEdit->Focused() && idx==SttIndex) {
				alpha_blend_Rect(cv, Rect(0, yp, GraphScrollBox->ClientWidth, yp + BarSize), col_Cursor, 64);
			}

			idx += OldOdrAction->Checked? 1 : -1;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::PaintBox1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
	if (DataList->Count>0 && GraphTopX<X) {
		int p = Y/BarSize;
		SetStatus(OldOdrAction->Checked? p : DataList->Count - 1 - p);
		PaintBox1->Invalidate();
		HiddenEdit->SetFocus();
	}
}

//---------------------------------------------------------------------------
//ステータスバー情報の設定
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::SetStatus(int idx)
{
	UnicodeString msg;
	if (idx>=0 && idx<DataList->Count) {
		SttIndex = idx;
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

		//スクロール位置の調整
		int yp = BarSize * (OldOdrAction->Checked? idx : DataList->Count - 1 - idx);
		if (yp < GraphScrollBox->VertScrollBar->Position)
			GraphScrollBox->VertScrollBar->Position = std::max(yp - BarSize, 0);
		else if (yp > (GraphScrollBox->VertScrollBar->Position + GraphScrollBox->ClientHeight - BarSize))
			GraphScrollBox->VertScrollBar->Position = (yp + BarSize - GraphScrollBox->ClientHeight);
	}

	StatusBar1->Panels->Items[0]->Text = msg;
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	draw_SttBarPanel(StatusBar, Panel, Rect);
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
		SetStatus(DataList->Count - 1);
		PaintBox1->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::OptComboBoxKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	if (contained_wd_i(KeysStr_ToList, get_KeyStr(Key, Shift))) HiddenEdit->SetFocus();
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
void __fastcall TDriveGraph::SizeBtnClick(TObject *Sender)
{
	SizeComboBox->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TDriveGraph::ToggleActionExecute(TObject *Sender)
{
	TAction *ap = (TAction *)Sender;
	ap->Checked = !ap->Checked;

	if (DlgInitialized) PaintBox1->Invalidate();
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::HiddenEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	HiddenEdit->Text = EmptyStr;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);

	if (contained_wd_i(_T("NextDrive|PrevDrive"), cmd_F) && DriveComboBox->Items->Count>1) {
		int idx = DriveComboBox->ItemIndex;
		if (USAME_TI(cmd_F, "NextDrive"))
			idx = (idx<DriveComboBox->Items->Count-1)? idx + 1 : 0;
		else
			idx = (idx>0)? idx - 1 : DriveComboBox->Items->Count-1;
		DriveComboBox->ItemIndex = idx;
		DriveComboBoxChange(NULL);
		Key = 0;
		return;
	}

	bool is_old  = OldOdrAction->Checked;
	bool is_down = USAME_TI(cmd_F, "CursorDown") || contained_wd_i(KeysStr_CsrDown, KeyStr);
	bool is_up	 = USAME_TI(cmd_F, "CursorUp")   || contained_wd_i(KeysStr_CsrUp,   KeyStr);
	bool is_inc  =  is_old? is_down : is_up;
	bool is_dec  = !is_old? is_down : is_up;
	int  max_idx = DataList->Count - 1;

	int idx = (is_dec && SttIndex>0)? SttIndex - 1 :
		(is_inc && SttIndex<max_idx)? SttIndex + 1 :
		(USAME_TI(cmd_F, "CursorTop") || equal_HOME(KeyStr))? ( is_old? 0 : max_idx) :
		(USAME_TI(cmd_F, "CursorEnd") ||  equal_END(KeyStr))? (!is_old? 0 : max_idx) : -1;

	if (idx!=-1) {
		SetStatus(idx);
		PaintBox1->Invalidate();
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TDriveGraph::HiddenEditEnterExit(TObject *Sender)
{
	HiddenEdit->Text = EmptyStr;
	PaintBox1->Invalidate();
}
//---------------------------------------------------------------------------


