//----------------------------------------------------------------------//
// NyanFi																//
//  登録ディレクトリ													//
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
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TRegDirDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddItemAction;
	TAction *CopyPathAction;
	TAction *DelItemAction;
	TAction *EditItemAction;
	TAction *OpenByExpAction;
	TAction *PropertyAction;
	TAction *SaveAsWorkAction;
	TAction *UseEnvVarAction;
	TActionList *ActionList1;
	TButton *AddButton;
	TButton *DelButton;
	TButton *DowButton;
	TButton *EditButton;
	TButton *HiddenCanBtn;
	TButton *RefDirButton;
	TButton *UpButton;
	TCheckBox *MoveTopCheckBox;
	TEdit *DescEdit;
	TEdit *DirEdit;
	TEdit *KeyEdit;
	THeaderControl *RegDirHeader;
	TListBox *RegDirListBox;
	TMenuItem *C1;
	TMenuItem *FitSizePosItem;
	TMenuItem *OpenByExpItem;
	TMenuItem *PropertyItem;
	TMenuItem *S1;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *UseEnvVarItem;
	TPanel *BlankPanel;
	TPanel *ListPanel;
	TPanel *OptPanel;
	TPopupMenu *PopupMenu1;
	TSpeedButton *HideOptBtn;
	TSpeedButton *ShowOptBtn;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RegDirHeaderResize(TObject *Sender);
	void __fastcall RegDirHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall RegDirListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RegDirListBoxClick(TObject *Sender);
	void __fastcall RegDirListBoxDblClick(TObject *Sender);
	void __fastcall RegDirListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegDirListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditItemActionExecute(TObject *Sender);
	void __fastcall EditItemActionUpdate(TObject *Sender);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall AddItemActionUpdate(TObject *Sender);
	void __fastcall RefDirButtonClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ChgOptBtnClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall RegDirHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);
	void __fastcall SaveAsWorkActionExecute(TObject *Sender);
	void __fastcall SaveAsWorkActionUpdate(TObject *Sender);
	void __fastcall CopyPathActionExecute(TObject *Sender);
	void __fastcall CopyPathActionUpdate(TObject *Sender);
	void __fastcall UseEnvVarActionExecute(TObject *Sender);
	void __fastcall UseEnvVarActionUpdate(TObject *Sender);
	void __fastcall OpenByExpActionExecute(TObject *Sender);
	void __fastcall OpenByExpActionUpdate(TObject *Sender);
	void __fastcall DelItemActionExecute(TObject *Sender);
	void __fastcall DelItemActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	int  SelIndex;
	TStringList *EnvVarList;

	void __fastcall SetOptBtn();
	int  __fastcall IndexOfDir(UnicodeString dnam);
	void __fastcall ChangeItemActionExecute(int chg_mod);
	UnicodeString __fastcall GetCurDirItem(bool dsp_sw = false, bool id_sw = false);

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	bool IsSpecial;		//特殊フォルダ一覧
	bool IsAddMode;		//追加モード
	UnicodeString CmdStr;

	__fastcall TRegDirDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRegDirDlg *RegDirDlg;
//---------------------------------------------------------------------------
#endif
