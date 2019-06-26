//----------------------------------------------------------------------//
// NyanFi																//
//  タグ設定/検索														//
//  フォルダアイコン検索												//
//----------------------------------------------------------------------//
#ifndef TagDlgH
#define TagDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Graphics.hpp>
#include "usr_swatch.h"
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TTagManDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CountTagAction;
	TAction *DefColorAction;
	TAction *DelTagAction;
	TAction *MakeNbtCurAction;
	TAction *MakeNbtOppAction;
	TAction *RenTagAction;
	TAction *SetColorAction;
	TAction *ShowTagCountAction;
	TAction *TrimDataAction;
	TActionList *ActionList1;
	TBevel *Bevel1;
	TButton *CanButton;
	TButton *DisableTagBtn;
	TButton *OkButton;
	TButton *RefTagColBtn;
	TCheckBox *AndCheckBox;
	TCheckBox *HideCheckBox;
	TCheckBox *ResLinkCheckBox;
	TCheckBox *RevColCheckBox;
	TCheckBox *SelMaskCheckBox;
	TCheckListBox *TagCheckListBox;
	TEdit *TagEdit;
	TImage *SpuitImage;
	TMenuItem *CountTagItem;
	TMenuItem *DefColItem;
	TMenuItem *DelTagItem;
	TMenuItem *MakeNbtCurItem;
	TMenuItem *MakeNbtItem;
	TMenuItem *MakeNbtOppItem;
	TMenuItem *RenTagItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TMenuItem *SetColItem;
	TMenuItem *ShowCountItem;
	TMenuItem *TrimDataItem;
	TPanel *BlankPanel;
	TPanel *FindOptPanel;
	TPanel *InpOptPanel;
	TPanel *InpPanel;
	TPanel *ListPanel;
	TPanel *OptPanel;
	TPanel *SetColPanel;
	TPanel *SpuitPanel;
	TPopupMenu *TagPopupMenu;
	TSpeedButton *HideOptBtn;
	TSpeedButton *ShowOptBtn;
	TAction *AndAction;
	TAction *SelMaskAction;
	TAction *HideAction;
	TAction *RevColAction;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TAction *ResLinkAction;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TagEditChange(TObject *Sender);
	void __fastcall TagEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TagEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall TagCheckListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall TagCheckListBoxClick(TObject *Sender);
	void __fastcall TagCheckListBoxClickCheck(TObject *Sender);
	void __fastcall TagCheckListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TagCheckListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall ChgOptBtnClick(TObject *Sender);
	void __fastcall RenTagActionExecute(TObject *Sender);
	void __fastcall DelTagActionExecute(TObject *Sender);
	void __fastcall SetColorActionExecute(TObject *Sender);
	void __fastcall SelActionUpdate(TObject *Sender);
	void __fastcall CountTagActionExecute(TObject *Sender);
	void __fastcall CountTagActionUpdate(TObject *Sender);
	void __fastcall ShowTagCountActionExecute(TObject *Sender);
	void __fastcall ShowTagCountActionUpdate(TObject *Sender);
	void __fastcall TrimDataActionExecute(TObject *Sender);
	void __fastcall MakeNbtActionExecute(TObject *Sender);
	void __fastcall MakeNbtActionUpdate(TObject *Sender);
	void __fastcall DefColorActionExecute(TObject *Sender);
	void __fastcall TagPopupMenuPopup(TObject *Sender);
	void __fastcall RefTagColBtnClick(TObject *Sender);
	void __fastcall SpuitImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpuitImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall CanButtonClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ListPanelResize(TObject *Sender);
	void __fastcall TagCheckListBoxDblClick(TObject *Sender);
	void __fastcall AndActionExecute(TObject *Sender);
	void __fastcall SelMaskActionExecute(TObject *Sender);
	void __fastcall HideActionExecute(TObject *Sender);
	void __fastcall RevColActionExecute(TObject *Sender);
	void __fastcall ResLinkActionExecute(TObject *Sender);

private:	// ユーザー宣言
	bool DlgInitialized;
	int  MaxTagWidth;
	bool InhEdit;
	bool IsFolderIcon;

	UsrSwatchPanel *SwatchPanel;

	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall SetCtrlFocus();
	void __fastcall SetOptBtn();

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	UnicodeString CmdStr;

	__fastcall TTagManDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TTagManDlg *TagManDlg;
//---------------------------------------------------------------------------
#endif
