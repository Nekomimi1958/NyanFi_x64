//----------------------------------------------------------------------//
// NyanFi																//
//  電卓																//
//----------------------------------------------------------------------//
#ifndef CalcDlgH
#define CalcDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.Math.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <math.h>

//---------------------------------------------------------------------------
class TCalculator : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AngleAction;
	TAction *EditDefAction;
	TAction *NotAction;
	TAction *ToHexDecAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TComboBox *HistComboBox;
	TEdit *LineEdit;
	TPopupMenu *ConstPopupMenu;
	TToolBar *ToolBar1;
	TToolButton *AcBtn;
	TToolButton *DegBtn;
	TToolButton *NotBtn;
	TToolButton *NowBtn;
	TToolButton *RefDefBtn;
	TToolButton *SpBtn1;
	TToolButton *SpBtn2;
	TToolButton *SpBtn3;
	TToolButton *ToDecBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LineEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LineEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall HistComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AngleActionExecute(TObject *Sender);
	void __fastcall AngleActionUpdate(TObject *Sender);
	void __fastcall RefDefBtnClick(TObject *Sender);
	void __fastcall RefDefItemClick(TObject *Sender);
	void __fastcall EditDefActionExecute(TObject *Sender);
	void __fastcall NowBtnClick(TObject *Sender);
	void __fastcall ToHexDecActionExecute(TObject *Sender);
	void __fastcall ToHexDecActionUpdate(TObject *Sender);
	void __fastcall NotActionExecute(TObject *Sender);
	void __fastcall NotActionUpdate(TObject *Sender);
	void __fastcall AcBtnClick(TObject *Sender);

private:	// ユーザー宣言
	TStringList *ConstList;
	TStringList *FuncList;
	int  AngleMode;
	int  OutDigit;
	int  DefConstCount;
	bool HideDefConst;

	bool KommaFlag;
	bool isError;
	UnicodeString ErrMsg;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		int hi = LineEdit->Height + HistComboBox->Height + ToolBar1->Height;
		Constraints->MinHeight = hi + Height - ClientHeight;
		Constraints->MaxHeight = hi + Height - ClientHeight;
	}

	void __fastcall InitUsrDef();

	bool __fastcall IsTimeStr(UnicodeString s);
	bool __fastcall IsHexStr(UnicodeString s);
	bool __fastcall IsHexOrInt(UnicodeString s);
	bool __fastcall DoubleIsInt(long double v, long double &ip);
	bool __fastcall DoubleIsInt(long double v);

	UnicodeString __fastcall LongDoubleToStr(long double v, bool is_x = false, bool is_t = false, bool is_ans = false);
	long double __fastcall EvalNumStr(UnicodeString s);
	long double __fastcall EvalFunc(UnicodeString s);
	void __fastcall EvalOpeItem(TStringList *o_lst, TStringList *m_lst, int p);
	UnicodeString __fastcall EvalExpr(UnicodeString s);
	UnicodeString __fastcall EvalLine(UnicodeString s);
	UnicodeString __fastcall SetLineStr(UnicodeString s);
	void __fastcall SetLineEdit(UnicodeString s);
	void __fastcall ShowError(_TCHAR *msg = NULL);
	void __fastcall AppendToLine(UnicodeString s);

	bool __fastcall is_IllegalVal(long double v)
	{
		return (v==HUGE_VALL || IsInfinite(v) || IsNan(v));
	}

public:		// ユーザー宣言
	UnicodeString InitialLine;
	UnicodeString OutputLine;

	__fastcall TCalculator(TComponent* Owner);
	void __fastcall CalcLine(UnicodeString s, bool use_cb = false);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCalculator *Calculator;
extern bool Calculating;
//---------------------------------------------------------------------------
#endif
