//----------------------------------------------------------------------//
// NyanFi																//
//  関数／ユーザ定義文字列／マーク行一覧								//
//----------------------------------------------------------------------//
#ifndef FuncDlgH
#define FuncDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.ComCtrls.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TFuncListDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CloseListAction;
	TAction *CopyListAction;
	TAction *LinkAction;
	TAction *MigemoAction;
	TAction *NameOnlyAction;
	TAction *RegExAction;
	TAction *RegHeaderAction;
	TAction *ReqEditAction;
	TAction *SaveListAction;
	TAction *UpdUserDefAction;
	TActionList *ActionList1;
	TButton *RegHeaderBtn;
	TButton *UpdateBtn;
	TCheckBox *LinkCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *NameOnlyCheckBox;
	TCheckBox *RegExCheckBox;
	TComboBox *UserDefComboBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *UsrStrLabel;
	TLabeledEdit *FilterEdit;
	TListBox *FuncListBox;
	TMenuItem *CopyListItem;
	TMenuItem *SaveListItem;
	TPanel *BottomPanel;
	TPanel *FilterPanel;
	TPanel *ListPanel;
	TPanel *Opt1Panel;
	TPanel *Opt2Panel;
	TPanel *UserDefPanel;
	TPopupMenu *PopupMenu1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FuncListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FuncListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FuncListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FuncListBoxClick(TObject *Sender);
	void __fastcall FuncListBoxDblClick(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall UpdUserDefActionExecute(TObject *Sender);
	void __fastcall UpdUserDefActionUpdate(TObject *Sender);
	void __fastcall UserDefComboBoxClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ReqEditActionExecute(TObject *Sender);
	void __fastcall CloseListActionExecute(TObject *Sender);
	void __fastcall CopyListActionExecute(TObject *Sender);
	void __fastcall SaveListActionExecute(TObject *Sender);
	void __fastcall ListActionUpdate(TObject *Sender);
	void __fastcall RegHeaderActionExecute(TObject *Sender);
	void __fastcall RegHeaderActionUpdate(TObject *Sender);
	void __fastcall UserDefComboBoxEnter(TObject *Sender);
	void __fastcall MigemoActionExecute(TObject *Sender);
	void __fastcall NameOnlyActionExecute(TObject *Sender);
	void __fastcall LinkActionExecute(TObject *Sender);
	void __fastcall RegExActionExecute(TObject *Sender);

private:	// ユーザー宣言
	TStringList *FunctionList;
	TStringList *UserDefList;
	TStringList *MarkLineList;

	bool DlgInitialized;

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall InitializeList(int mode = -1);
	void __fastcall UpdateList(bool link = false);
	void __fastcall ToLine();
	void __fastcall GetStrList(TStringList *lst);

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	int  ListMode;			//0:関数  1:ユーザ定義  2:マーク行一覧
	int  LineNo;
	bool ToFilter;
	bool ReqEdit;
	UnicodeString UserDefStr;
	UnicodeString NamePtn;	//関数名の強調パターン

	bool is_DFM;

	__fastcall TFuncListDlg(TComponent* Owner);
	void __fastcall ClearList();

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFuncListDlg *FuncListDlg;
//---------------------------------------------------------------------------
#endif
