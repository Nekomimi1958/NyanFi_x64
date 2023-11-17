//----------------------------------------------------------------------//
// NyanFi																//
//  タスクマネージャ													//
//----------------------------------------------------------------------//
#ifndef TaskDlgH
#define TaskDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ComCtrls.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TTaskManDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CancelTaskAction;
	TAction *CanTaskAllAction;
	TAction *ExtStartAction;
	TAction *PauseAction;
	TAction *RestartAction;
	TAction *StartAction;
	TAction *SuspendAction;
	TActionList *ActionList1;
	TButton *btnStartAction;
	TButton *Button1;
	TButton *CanAllBtn;
	TButton *CanTaskBtn;
	TButton *CloseBtn;
	TButton *PauseBtn;
	TButton *RestartBtn;
	TButton *SuspendBtn;
	THeaderControl *TaskHeader;
	TPanel *GridPanel;
	TPanel *OpePanel;
	TStringGrid *TaskGrid;
	TTimer *Timer1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TaskHeaderResize(TObject *Sender);
	void __fastcall TaskHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall TaskHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall TaskGridDrawCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow, TRect &Rect, TGridDrawState State);
	void __fastcall TaskGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CancelTaskActionExecute(TObject *Sender);
	void __fastcall CancelTaskActionUpdate(TObject *Sender);
	void __fastcall CanTaskAllActionExecute(TObject *Sender);
	void __fastcall CanTaskAllActionUpdate(TObject *Sender);
	void __fastcall PauseActionExecute(TObject *Sender);
	void __fastcall PauseActionUpdate(TObject *Sender);
	void __fastcall RestartActionExecute(TObject *Sender);
	void __fastcall RestartActionUpdate(TObject *Sender);
	void __fastcall SuspendActionExecute(TObject *Sender);
	void __fastcall SuspendActionUpdate(TObject *Sender);
	void __fastcall StartActionExecute(TObject *Sender);
	void __fastcall StartActionUpdate(TObject *Sender);
	void __fastcall ExtStartActionExecute(TObject *Sender);
	void __fastcall ExtStartActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TTaskThread * __fastcall get_CurTask();
	int RsrCellHi;

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー

	__fastcall TTaskManDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TTaskManDlg *TaskManDlg;
//---------------------------------------------------------------------------
#endif
