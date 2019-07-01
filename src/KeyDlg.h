//----------------------------------------------------------------------//
// NyanFi																//
//  キー割り当て一覧													//
//----------------------------------------------------------------------//
#ifndef KeyDlgH
#define KeyDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TKeyListDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *HelpCmdAction;
	TAction *MigemoAction;
	TAction *ShowAllCmdAction;
	TAction *SortCmdAction;
	TAction *SortKeyAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TCheckBox *CnfExeCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *ShowAllCmdCheckBox;
	THeaderControl *KeyListHeader;
	TLabeledEdit *FilterEdit;
	TMenuItem *CopyCmdItem;
	TMenuItem *CopyListItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *HelpCmdItem;
	TMenuItem *OptionItem;
	TMenuItem *SaveAsListItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *SortCmdItem;
	TMenuItem *SortDscItem;
	TMenuItem *SortKeyItem;
	TPanel *GridPanel;
	TPanel *OpePanel;
	TPopupMenu *PopupMenu1;
	TStringGrid *KeyListGrid;
	TTabControl *KeyTabControl;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KeyTabControlChange(TObject *Sender);
	void __fastcall KeyListHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall KeyListHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall KeyListGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall KeyListGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KeyListGridDblClick(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SaveAsListItemClick(TObject *Sender);
	void __fastcall CopyListItemClick(TObject *Sender);
	void __fastcall OptionItemClick(TObject *Sender);
	void __fastcall KeyListHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall SortKeyActionUpdate(TObject *Sender);
	void __fastcall SortKeyActionExecute(TObject *Sender);
	void __fastcall KeyTabControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall KeyListHeaderResize(TObject *Sender);
	void __fastcall CopyCmdItemClick(TObject *Sender);
	void __fastcall HelpCmdActionExecute(TObject *Sender);
	void __fastcall HelpCmdActionUpdate(TObject *Sender);
	void __fastcall MigemoActionExecute(TObject *Sender);
	void __fastcall ShowAllCmdActionExecute(TObject *Sender);

private:	// ユーザー宣言
	TStringList *CurList;
	int KeySortMode;

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall GetStrList(TStringList *lst);

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー

	UnicodeString CommandStr;
	bool ToFilter;

	__fastcall TKeyListDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TKeyListDlg *KeyListDlg;
//---------------------------------------------------------------------------
#endif
