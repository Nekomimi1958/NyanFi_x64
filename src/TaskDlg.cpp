//----------------------------------------------------------------------//
// NyanFi																//
//  タスクマネージャ													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "UserFunc.h"
#include "Global.h"
#include "MainFrm.h"
#include "TaskDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTaskManDlg *TaskManDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TTaskManDlg::TTaskManDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::FormCreate(TObject *Sender)
{
	GridScrPanel = new UsrScrollPanel(GridPanel, TaskGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::FormShow(TObject *Sender)
{
	Timer1->Enabled = false;

	IniFile->LoadPosInfo(this, DialogCenter);

	TStringGrid *gp = TaskGrid;
	InitializeListGrid(gp);
	InitializeListHeader(TaskHeader, _T("#|タスク|詳細|残|状態|経過時間"));
	IniFile->LoadGridColWidth(gp, 6, 30,80,200,60,100,100);
	set_HeaderFromGrid(gp, TaskHeader);
	set_UsrScrPanel(GridScrPanel);

	RsrCellHi = get_FontHeight(gp->Font, ListInterLn);
	int maxn  = get_MaxTaskCount();
	gp->DefaultRowHeight = RsrCellHi * 2 + 4;
	gp->RowCount = maxn + TaskReserveList->Count;
	for (int i=0; i<gp->RowCount; i++)
		if (i>=maxn) gp->RowHeights[i] = RsrCellHi;

	gp->SetFocus();
	GridScrPanel->UpdateKnob();

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::WmFormShowed(TMessage &msg)
{
	if (!UnInitializing) Timer1->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;

	IniFile->SavePosInfo(this);
	IniFile->SaveGridColWidth(TaskGrid);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::FormDestroy(TObject *Sender)
{
	delete GridScrPanel;
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::TaskHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::TaskHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(TaskGrid, TaskHeader);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::TaskHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	set_GridFromHeader(TaskHeader, TaskGrid);

	GridScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//タイマー処理
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::Timer1Timer(TObject *Sender)
{
	TStringGrid *gp = TaskGrid;
	int total_speed = 0;
	int maxn = get_MaxTaskCount();

	//高さ調整
	bool changeed = false;
	for (int i=0; i<gp->RowCount; i++) {
		int r_hi = (i<maxn)? gp->DefaultRowHeight : RsrCellHi;
		if (gp->RowHeights[i]!=r_hi) {
			gp->RowHeights[i] = r_hi;
			changeed = true;
		}
	}
	if (changeed) gp->Invalidate();

	for (int i=0; i<gp->RowCount; i++) {
		int idx = i;
		if (idx<maxn) {
			TTaskThread *tp = TaskThread[idx];
			if (tp) {
				//コマンド
				gp->Cells[1][i] = TaskCmdList->Values[tp->CmdName];
				//詳細
				UnicodeString cell_str;
				cell_str.sprintf(_T("%s\t%s"), tp->Config->InfStr.c_str(), ExtractFileName(tp->CurFileName).c_str());
				if (tp->CurTotalSize>0)
					cell_str.cat_sprintf(_T("  (%s)"), get_size_str_T(tp->CurTotalSize, SizeDecDigits).c_str());

				gp->Cells[2][i] = cell_str;
				//残
				cell_str = tp->Config->TaskList->Count;
				if (tp->SubCount>0) cell_str.cat_sprintf(_T("\t%u"), tp->SubCount);
				gp->Cells[3][i] = cell_str;
				//状態
				cell_str = tp->TaskPause? "一旦停止中" :
						   tp->TaskCancel? "中断要求中" :
						   (tp->PreCount>0)? "準備中" :
						   tp->TaskIsFast?  "高速実行中" : "実行中";
				if (tp->PreCount>0)
					cell_str.cat_sprintf(_T("\t%u"), tp->PreCount);
				else if (!tp->CurFileName.IsEmpty() && tp->CurProgress>=0) {
					cell_str.cat_sprintf(_T("\t%3u%%"), (int)(tp->CurProgress*100));
					if (tp->Speed>0) {
						cell_str.cat_sprintf(_T("  (%s/秒)"), get_size_str_T(tp->Speed*1000, 1).c_str());
						total_speed += tp->Speed;
					}
				}
				gp->Cells[4][i] = cell_str;
				//経過時間
				cell_str = mSecToTStr(::GetTickCount() - tp->StartCount, false);
				if (tp->RemCount>0) {
					UnicodeString rt_str = mSecToTStr(tp->RemCount, false);
					remove_top_text(rt_str, _T("00:"));
					cell_str.cat_sprintf(_T("\t(残り %s)"), rt_str.c_str());
				}
				gp->Cells[5][i] = cell_str;
			}
			else clear_GridRow(gp, i, 1);
		}
		else {
			//予約
			idx -= (get_MaxTaskCount());
			if (idx<TaskReserveList->Count) {
				TaskConfig *cp = TaskReserveList->Items[idx];
				gp->Cells[1][i] = TaskCmdList->Values[get_pre_tab(cp->TaskList->Strings[0])];
				gp->Cells[2][i] = cp->InfStr;
				gp->Cells[3][i] = EmptyStr;
				gp->Cells[4][i] = RsvSuspended? "保留中" : "待機中";
				gp->Cells[5][i] = EmptyStr;
			}
			else clear_GridRow(gp, i, 1);
		}
	}

	UnicodeString hstr = "状態";
	if (total_speed>0) hstr.cat_sprintf(_T("    (%s/秒)"), get_size_str_T(total_speed*1000, 1).c_str());
	TaskHeader->Sections->Items[4]->Text = hstr;
}

//---------------------------------------------------------------------------
TTaskThread * __fastcall TTaskManDlg::get_CurTask()
{
	TTaskThread *tp = NULL;
	int idx = TaskGrid->Row;
	if (idx>=0 && idx<get_MaxTaskCount()) tp = TaskThread[idx];
	return tp;
}

//---------------------------------------------------------------------------
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::TaskGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	if (UnInitializing) return;

	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	int idx  = ARow;
	int maxn = get_MaxTaskCount();

	TColor fg;
	if (idx<maxn) {
		TTaskThread *tp = TaskThread[idx];
		if (tp)
			fg = (ACol==4 && tp->PreCount>0)? clGreen : (ACol==4 && tp->TaskPause) ? clRed : col_fgList;
		else
			fg = AdjustColor(col_fgList, 96);
	}
	else
		fg = AdjustColor(col_fgList, 72);

	cv->Font->Color  = is_SelFgCol(State)? col_fgSelItem : fg;
	cv->Brush->Color = State.Contains(gdSelected)? col_selItem : col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + 4;
	int yp = Rect.Top + get_TopMargin2(cv);
	int l_hi = get_FontHeight(gp->Font, ListInterLn);

	TRect rc = Rect;
	UnicodeString cellstr;
	if (ACol==0)
		cellstr = (ACol<maxn)? UnicodeString(ARow + 1) : EmptyStr;
	else
		cellstr = gp->Cells[ACol][ARow];

	TStringDynArray l_lst = split_strings_tab(cellstr);
	for (int i=0; i<l_lst.Length; i++) {
		if (ACol==3) xp = Rect.Right - get_TextWidth(cv, l_lst[i], is_irreg) - 4;
		cv->TextRect(rc, xp, yp, l_lst[i]);
		if (ACol==4 && i==1 && contains_s(l_lst[i], _T('%'))) {
			//進捗バー
			int prg = get_tkn(l_lst[i], '%').ToIntDef(-1);
			if (prg>=0) {
				TRect rc_f	 = Rect;
				rc_f.Left	+= 4;
				rc_f.Right	-= 4;
				rc_f.Bottom -= 2;
				rc_f.Top	 = rc_f.Bottom - 4;
				draw_ProgressBar(cv, rc_f, prg/100.0);
			}
		}
		rc.Top += l_hi;
		yp	   += l_hi;
	}
	//カーソル
	draw_GridCursor(gp, Rect, ARow, State);
}

//---------------------------------------------------------------------------
//一覧でのキー操作
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::TaskGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	UnicodeString CmdStr = Key_to_CmdF(KeyStr);

	//カーソル移動
	if (GridCursorMove(CmdStr, gp))	;
	//閉じる
	else if (USAME_TI(CmdStr, "ReturnList")) ModalResult = mrCancel;
	//中止
	else if (equal_DEL(KeyStr) || USAME_TI(CmdStr, "Delete")) CancelTaskAction->Execute();

	if (!is_DialogKey(Key)) Key = 0;
}

//---------------------------------------------------------------------------
//タスクの中止
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::CancelTaskActionExecute(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	if (tp)
		tp->TaskCancel = true;
	else {
		int idx = TaskGrid->Row - get_MaxTaskCount();
		if (idx>=0 && idx<TaskReserveList->Count) {
			delete TaskReserveList->Items[idx];
			TaskReserveList->Delete(idx);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::CancelTaskActionUpdate(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	if (tp)
		((TAction*)Sender)->Enabled = !tp->TaskCancel;
	else {
		int idx = TaskGrid->Row - get_MaxTaskCount();
		((TAction*)Sender)->Enabled = (idx>=0 && idx<TaskReserveList->Count);
	}
}
//---------------------------------------------------------------------------
//全タスクの中止
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::CanTaskAllActionExecute(TObject *Sender)
{
	if (msgbox_Sure(_T("すべてのタスクを中止しますか?")))
		NyanFiForm->CancelAllTaskAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::CanTaskAllActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (get_BusyTaskCount()>0);
}

//---------------------------------------------------------------------------
//一旦停止
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::PauseActionExecute(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	if (tp) tp->TaskPause = true;
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::PauseActionUpdate(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	((TAction*)Sender)->Enabled = tp? !tp->TaskPause : false;
}
//---------------------------------------------------------------------------
//再開
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::RestartActionExecute(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	if (tp) tp->TaskPause = false;
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::RestartActionUpdate(TObject *Sender)
{
	TTaskThread *tp = get_CurTask();
	((TAction*)Sender)->Enabled = tp? tp->TaskPause: false;
}

//---------------------------------------------------------------------------
//予約項目の保留
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::SuspendActionExecute(TObject *Sender)
{
	NyanFiForm->SuspendAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::SuspendActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Caption = RsvSuspended? "解除(&U)" : "保留(&U)";
}

//---------------------------------------------------------------------------
//保留予約項目の開始
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::StartActionExecute(TObject *Sender)
{
	int idx  = TaskGrid->Row;
	int maxn = get_MaxTaskCount();
	if (idx>=maxn) StartReserve(idx - maxn);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::StartActionUpdate(TObject *Sender)
{
	int idx = TaskGrid->Row - get_MaxTaskCount();
	((TAction*)Sender)->Enabled = RsvSuspended && has_EmptyTask() && idx>=0 && idx<TaskReserveList->Count;
}
//---------------------------------------------------------------------------
//保留予約項目の割り込み実行
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::ExtStartActionExecute(TObject *Sender)
{
	TStringGrid *gp = TaskGrid;
	int idx  = gp->Row;
	int maxn = get_MaxTaskCount();
	if (idx>=maxn) StartReserve(idx - maxn, true);
}
//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::ExtStartActionUpdate(TObject *Sender)
{
	int idx = TaskGrid->Row - get_MaxTaskCount();
	((TAction*)Sender)->Enabled = has_EmptyTask(true) && idx>=0 && idx<TaskReserveList->Count;
}

//---------------------------------------------------------------------------
void __fastcall TTaskManDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

