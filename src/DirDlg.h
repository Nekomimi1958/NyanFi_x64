//----------------------------------------------------------------------//
// NyanFi																//
//  登録ディレクトリ/特殊フォルダ一覧/参照								//
//----------------------------------------------------------------------//
#ifndef DirDlgH
#define DirDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ToolWin.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TRegDirDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddItemAction;
	TAction *AddNyanFiAction;
	TAction *AddPathAction;
	TAction *AndOrAction;
	TAction *AppInfoAction;
	TAction *CopyPathAction;
	TAction *DelItemAction;
	TAction *EditItemAction;
	TAction *MigemoAction;
	TAction *OpenByExpAction;
	TAction *PropertyAction;
	TAction *SaveAsWorkAction;
	TAction *ShowIconAction;
	TAction *UseEnvVarAction;
	TActionList *ActionList1;
	TButton *AddButton;
	TButton *DelButton;
	TButton *DowButton;
	TButton *EditButton;
	TButton *HiddenCanBtn;
	TButton *RefDirBtn;
	TButton *RefSpBtn;
	TButton *UpButton;
	TCheckBox *CursorTopCheckBox;
	TCheckBox *MoveTopCheckBox;
	TEdit *DescEdit;
	TEdit *DirEdit;
	TEdit *FilterEdit;
	TEdit *KeyEdit;
	THeaderControl *RegDirHeader;
	TListBox *RegDirListBox;
	TMenuItem *AddNyanFiItem;
	TMenuItem *AppInfoItem;
	TMenuItem *CopyPathItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *OpenByExpItem;
	TMenuItem *PATHP1;
	TMenuItem *PropertyItem;
	TMenuItem *SaveAsWorkItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *ShowIconItem;
	TMenuItem *UseEnvVarItem;
	TPanel *BlankPanel;
	TPanel *ListPanel;
	TPanel *OptPanel;
	TPopupMenu *PopupMenu1;
	TSpeedButton *HideOptBtn;
	TSpeedButton *ShowOptBtn;
	TSplitter *FilterSplitter;
	TToolBar *OpeToolBar;
	TToolButton *AndOrBtn;
	TToolButton *FilterBtn;
	TToolButton *MigemoBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegDirHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall RegDirHeaderResize(TObject *Sender);
	void __fastcall RegDirHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall RegDirListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RegDirListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegDirListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall RegDirListBoxClick(TObject *Sender);
	void __fastcall RegDirListBoxDblClick(TObject *Sender);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall AddItemActionUpdate(TObject *Sender);
	void __fastcall EditItemActionExecute(TObject *Sender);
	void __fastcall EditItemActionUpdate(TObject *Sender);
	void __fastcall DelItemActionExecute(TObject *Sender);
	void __fastcall DelItemActionUpdate(TObject *Sender);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall RefSpBtnClick(TObject *Sender);
	void __fastcall CopyPathActionExecute(TObject *Sender);
	void __fastcall CopyPathActionUpdate(TObject *Sender);
	void __fastcall UseEnvVarActionExecute(TObject *Sender);
	void __fastcall ToggleActionExecute(TObject *Sender);
	void __fastcall AddNyanFiActionExecute(TObject *Sender);
	void __fastcall AddPathActionExecute(TObject *Sender);
	void __fastcall OpenByExpActionExecute(TObject *Sender);
	void __fastcall OpenByExpActionUpdate(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);
	void __fastcall AppInfoActionExecute(TObject *Sender);
	void __fastcall AppInfoActionUpdate(TObject *Sender);
	void __fastcall SaveAsWorkActionExecute(TObject *Sender);
	void __fastcall SaveAsWorkActionUpdate(TObject *Sender);
	void __fastcall ChgOptBtnClick(TObject *Sender);
	void __fastcall FilterBtnClick(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);

private:	// ユーザー宣言
	int SelIndex;

	bool KeyHandled;

	TStringList *EnvVarList;
	TStringList *SpDirList;
	TStringList *SpDirBuff;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall WmNyanFiFlIcon(TMessage &msg) { RegDirListBox->Invalidate(); }

	void __fastcall UpdateSpDirList(bool reload = false);
	void __fastcall SetOptBtn();
	int  __fastcall IndexOfDir(UnicodeString dnam);
	void __fastcall ChangeItemActionExecute(int chg_mod);

	UnicodeString __fastcall GetCurDirItem(bool dsp_sw = false, bool nam_sw = false, bool exe_sw = false);

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	bool IsSpecial;		//特殊フォルダ一覧
	bool IsAddMode;		//追加モード
	bool IsSelect;		//選択モード
	bool ToFilter;		//フィルタ欄にフォーカス
	UnicodeString CmdStr;

	__fastcall TRegDirDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,		TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,		TMessage,	WmMenuChar)
		VCL_MESSAGE_HANDLER(WM_NYANFI_FLICON,	TMessage,	WmNyanFiFlIcon)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TRegDirDlg *RegDirDlg;
//---------------------------------------------------------------------------
#endif
