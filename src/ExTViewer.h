//----------------------------------------------------------------------//
// NyanFi																//
//  テキストビューア(別ウィンドウ)										//
//----------------------------------------------------------------------//
#ifndef ExTViewerH
#define ExTViewerH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>
#include "Global.h"
#include "TxtViewer.h"

//---------------------------------------------------------------------------
class TExTxtViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddCopyAction;
	TAction *CodePageAction;
	TAction *CopyAction;
	TAction *CP_AutoAction;
	TAction *CP_EucAction;
	TAction *CP_JisAction;
	TAction *CP_Latin1Action;
	TAction *CP_ShiftJisAction;
	TAction *CP_UTF8Action;
	TAction *CP_UTF16Action;
	TAction *ExportCsvAction;
	TAction *FindTextAction;
	TAction *OrgOrderAction;
	TAction *SaveDumpAction;
	TAction *SelectAllAction;
	TAction *SortAscAction;
	TAction *SortDesAction;
	TAction *TopIsHdrAction;
	TAction *WebSearchAction;
	TActionList *ActionList1;
	TMenuItem *CP_AutoItem;
	TMenuItem *CP_EUCItem;
	TMenuItem *CP_JisItem;
	TMenuItem *CP_Latin1Item;
	TMenuItem *CP_ShiftJISItem;
	TMenuItem *CP_UTF8Item;
	TMenuItem *CP_UTF16Item;
	TMenuItem *OrgOrderItem;
	TMenuItem *PopAddCopyItem;
	TMenuItem *PopCodePageItem;
	TMenuItem *PopCopyItem;
	TMenuItem *PopExpCsvItem;
	TMenuItem *PopFindItem;
	TMenuItem *PopSaveDumpItem;
	TMenuItem *PopSelAllItem;
	TMenuItem *PopSortItem;
	TMenuItem *PopTopIsHdrItem;
	TMenuItem *PopWebSearchItem;
	TMenuItem *Sep_vp_1;
	TMenuItem *Sep_vp_2;
	TMenuItem *Sep_vp_3;
	TMenuItem *SortAscItem;
	TMenuItem *SortDesItem;
	TPaintBox *TextPaintBox;
	TPaintBox *TextRulerBox;
	TPanel *TextCurColPanel;
	TPanel *TxtMainPanel;
	TPanel *TxtScrollPanel;
	TPanel *TxtViewPanel;
	TPopupMenu *ViewPopupMenu;
	TRichEdit *ViewMemo;
	TScrollBar *TextScrollBar;
	TStatusBar *TxtSttHeader;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormDeactivate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall TextPaintBoxPaint(TObject *Sender);
	void __fastcall TextScrollBarChange(TObject *Sender);
	void __fastcall TextPaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxDblClick(TObject *Sender);
	void __fastcall TxtSttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall FindTextActionExecute(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall AddCopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall SelectAllActionExecute(TObject *Sender);
	void __fastcall WebSearchActionExecute(TObject *Sender);
	void __fastcall WebSearchActionUpdate(TObject *Sender);
	void __fastcall PopWebSearchItemMeasureItem(TObject *Sender, TCanvas *ACanvas, int &Width, int &Height);
	void __fastcall SortAscActionExecute(TObject *Sender);
	void __fastcall SortDesActionExecute(TObject *Sender);
	void __fastcall OrgOrderActionExecute(TObject *Sender);
	void __fastcall SortActionUpdate(TObject *Sender);
	void __fastcall PopCodePageActionExecute(TObject *Sender);
	void __fastcall CodePageActionUpdate(TObject *Sender);
	void __fastcall CodePageActionExecute(TObject *Sender);
	void __fastcall CP_xxx_ActionUpdate(TObject *Sender);
	void __fastcall ExportCsvActionExecute(TObject *Sender);
	void __fastcall ExportCsvActionUpdate(TObject *Sender);
	void __fastcall SaveDumpActionExecute(TObject *Sender);
	void __fastcall SaveDumpActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	UsrScrollPanel *TxtViewScrPanel;	//独自スクロールバー

	bool DlgInitialized;

	UnicodeString FirstKey;				//2ストローク開始キー
	bool Wait2ndKey;					//2ストローク目待ち状態

	UnicodeString ActionParam;			//コマンドアクションのパラメータ
	UnicodeString ActionDesc;			//コマンドアクションの説明
	UnicodeString ActionErrMsg;			//コマンドアクションのエラーメッセージ
	bool ActionOk;						//コマンドアクションの実行結果

	bool isViewText;					//テキスト表示
	bool isRichText;					//リッチテキスト表示
	bool isXDoc2Txt;					//xdoc2txt を利用して表示

	int  LastWidth;

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall WmExitSizeMove(TMessage &msg);
	void __fastcall WmDropped(TMessage &msg);

	TWndMethod org_TxtSttHdrWndProc;
	void __fastcall TxtSttHdrWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(TxtSttHeader, msg)) return;
		org_TxtSttHdrWndProc(msg);
	}

	void __fastcall AdjustHdrWidth();

	void __fastcall GlobalAbort();
	UnicodeString __fastcall TwoStrokeSeq(WORD &Key, TShiftState Shift);

public:		// ユーザー宣言
	TTxtViewer *ExViewer;

	file_rec *FileRec;

	UnicodeString FileName;
	UnicodeString OrgName;
	int  LineNo;
	bool isClip;

	__fastcall TExTxtViewer(TComponent* Owner);

	bool __fastcall OpenViewer(bool bin_mode = false, int code_page = 0, int lno = 0, bool force_txt = false);
	bool __fastcall OpenViewer(UnicodeString fnam, bool bin_mode = false, int code_page = 0, int lno = 0);

	void __fastcall CancelKeySeq();

	void __fastcall DirectTagJumpCore(bool is_edit = false, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandV(UnicodeString cmd, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandV(const _TCHAR *cmd);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,		TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_EXITSIZEMOVE,	TMessage,	WmExitSizeMove)
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,	WmDropped)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
#endif
