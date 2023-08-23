//----------------------------------------------------------------------//
// NyanFi																//
//  JSONビュアー														//
//----------------------------------------------------------------------//
#ifndef JsonViewH
#define JsonViewH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.JSON.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Xml.XMLDoc.hpp>
#include <Xml.xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TJsonViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AutoAction;
	TAction *CollapseAction;
	TAction *CopyAction;
	TAction *CopyFormatAction;
	TAction *CopyJsonAction;
	TAction *CopyJsonPathAction;
	TAction *EditErrAction;
	TAction *ExpandAction;
	TAction *FindDownAction;
	TAction *FindUpAction;
	TAction *OpenUrlAction;
	TActionList *ActionList1;
	TButton *Button1;
	TButton *ColBtn;
	TButton *ExpBtn;
	TButton *FindDownBtn;
	TButton *FindUpBtn;
	TCheckBox *AutoCheckBox;
	TCheckBox *NamCheckBox;
	TCheckBox *ValCheckBox;
	TLabeledEdit *FindEdit;
	TMenuItem *CollapseItem;
	TMenuItem *CopyFormatItem;
	TMenuItem *CopyItem;
	TMenuItem *CopyJsonItem;
	TMenuItem *CopyPathItem;
	TMenuItem *E1;
	TMenuItem *ExpandItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *OpenUrlItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TPanel *OpePanel;
	TPanel *ViewPanel;
	TPopupMenu *PopupMenu1;
	TStatusBar *StatusBar1;
	TTreeView *JsonTreeView;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall JsonTreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
		TCustomDrawState State, bool &DefaultDraw);
	void __fastcall JsonTreeViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall JsonTreeViewHint(TObject *Sender, TTreeNode * const Node, UnicodeString &Hint);
	void __fastcall JsonTreeViewDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall ExpandItemClick(TObject *Sender);
	void __fastcall CollapseItemClick(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall CopyJsonActionExecute(TObject *Sender);
	void __fastcall CopyJsonActionUpdate(TObject *Sender);
	void __fastcall CopyJsonPathActionExecute(TObject *Sender);
	void __fastcall CopyJsonPathActionUpdate(TObject *Sender);
	void __fastcall CopyFormatActionExecute(TObject *Sender);
	void __fastcall CopyFormatActionUpdate(TObject *Sender);
	void __fastcall OpenUrlActionExecute(TObject *Sender);
	void __fastcall OpenUrlActionUpdate(TObject *Sender);
	void __fastcall EditErrActionExecute(TObject *Sender);
	void __fastcall EditErrActionUpdate(TObject *Sender);
	void __fastcall FindDownActionExecute(TObject *Sender);
	void __fastcall FindUpActionExecute(TObject *Sender);
	void __fastcall FindActionUpdate(TObject *Sender);
	void __fastcall ExpandActionExecute(TObject *Sender);
	void __fastcall CollapseActionExecute(TObject *Sender);
	void __fastcall AutoActionExecute(TObject *Sender);
	void __fastcall JsonTreeViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall JsonTreeViewKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FindEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindEditKeyPress(TObject *Sender, System::WideChar &Key);

private:	// ユーザー宣言
	//ViewBusy プロパティ  ツリービュー処理中
	bool FViewBusy;
	void __fastcall SetViewBusy(bool Value);
	__property bool ViewBusy = {read = FViewBusy,  write = SetViewBusy};

	UnicodeString OrgText;
	UnicodeString ErrMsg;
	bool isJsonErr;
	int  ErrLine;
	bool KeyHandled;

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall AssignJsonView(TJSONValue *val, TTreeNode *np);
	UnicodeString __fastcall GetTreeViewText(TTreeNode *Node = NULL, bool is_json = false);
	UnicodeString __fastcall GetJsonPath(TTreeNode *Node = NULL);
	bool __fastcall MatchNode(TTreeNode *Node);

public:		// ユーザー宣言
	UnicodeString FileName;
	bool isClip;

	__fastcall TJsonViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TJsonViewer *JsonViewer;
//---------------------------------------------------------------------------
#endif
