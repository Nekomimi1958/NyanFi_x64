//----------------------------------------------------------------------//
// NyanFi																//
//  XMLビュアー															//
//----------------------------------------------------------------------//
#ifndef XmlViewH
#define XmlViewH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
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

//---------------------------------------------------------------------------
class TXmlViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AutoAction;
	TAction *CollapseAction;
	TAction *CopyAction;
	TAction *CopyXmlAction;
	TAction *CopyXPathAction;
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
	TCheckBox *AtrCheckBox;
	TCheckBox *AutoCheckBox;
	TCheckBox *NamCheckBox;
	TCheckBox *ValCheckBox;
	TLabeledEdit *FindEdit;
	TMenuItem *CollapseItem;
	TMenuItem *CopyItem;
	TMenuItem *CopyXmlItem;
	TMenuItem *ExpandItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *OpenUrlItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TMenuItem *ViewMode0Item;
	TMenuItem *ViewMode1Item;
	TMenuItem *ViewMode2Item;
	TMenuItem *ViewModeItem;
	TMenuItem *XPath1;
	TPanel *OpePanel;
	TPanel *ViewPanel;
	TPopupMenu *PopupMenu1;
	TStatusBar *StatusBar1;
	TTreeView *XmlTreeView;
	TXMLDocument *XMLDocument1;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall XmlTreeViewCustomDrawItem(TCustomTreeView *Sender, TTreeNode *Node,
		TCustomDrawState State, bool &DefaultDraw);
	void __fastcall XmlTreeViewChange(TObject *Sender, TTreeNode *Node);
	void __fastcall XmlTreeViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall XmlTreeViewKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall XmlTreeViewHint(TObject *Sender, TTreeNode * const Node, UnicodeString &Hint);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall ExpandItemClick(TObject *Sender);
	void __fastcall CollapseItemClick(TObject *Sender);
	void __fastcall FindEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FindEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall ViewModeItemClick(TObject *Sender);
	void __fastcall OpenUrlActionExecute(TObject *Sender);
	void __fastcall OpenUrlActionUpdate(TObject *Sender);
	void __fastcall FindDownActionExecute(TObject *Sender);
	void __fastcall FindActionUpdate(TObject *Sender);
	void __fastcall FindUpActionExecute(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ExpandActionExecute(TObject *Sender);
	void __fastcall CollapseActionExecute(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall CopyXPathActionExecute(TObject *Sender);
	void __fastcall CopyXPathActionUpdate(TObject *Sender);
	void __fastcall CopyXmlActionExecute(TObject *Sender);
	void __fastcall CopyXmlActionUpdate(TObject *Sender);
	void __fastcall AutoActionExecute(TObject *Sender);

private:	// ユーザー宣言
	//ViewBusy プロパティ  ツリービュー処理中
	bool FViewBusy;
	void __fastcall SetViewBusy(bool Value);
	__property bool ViewBusy = {read = FViewBusy,  write = SetViewBusy};

	int ViewMode;				//表示モード 0: 名前/ 1: 名前 [属性]/ 2:XML

	TStringList *XmlnsList;		//名前空間リスト
	UnicodeString ErrMsg;
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
	void __fastcall AssignView(TTreeNode *TreeNode, _di_IXMLNode XMLNode);
	UnicodeString __fastcall GetXPath(TTreeNode *Node = NULL);
	UnicodeString __fastcall GetXmlStr();
	bool __fastcall MatchNode(TTreeNode *Node);

public:		// ユーザー宣言
	UnicodeString FileName;

	__fastcall TXmlViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TXmlViewer *XmlViewer;
//---------------------------------------------------------------------------
#endif
