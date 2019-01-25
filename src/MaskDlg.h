//----------------------------------------------------------------------//
// NyanFi																//
//  パスマスク															//
//----------------------------------------------------------------------//
#ifndef MaskDlgH
#define MaskDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TPathMaskDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddItemAction;
	TAction *EditItemAction;
	TActionList *ActionList1;
	TButton *AddButton;
	TButton *DelButton;
	TButton *DowButton;
	TButton *EditButton;
	TButton *HiddenCanBtn;
	TButton *UpButton;
	TEdit *DescEdit;
	TEdit *KeyEdit;
	TEdit *MaskEdit;
	THeaderControl *PathMaskHeader;
	TListBox *PathMaskListBox;
	TPanel *ListPanel;
	TPanel *OpePanel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall PathMaskHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall EditItemActionExecute(TObject *Sender);
	void __fastcall EditItemActionUpdate(TObject *Sender);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall AddItemActionUpdate(TObject *Sender);
	void __fastcall PathMaskListBoxClick(TObject *Sender);
	void __fastcall PathMaskListBoxDblClick(TObject *Sender);
	void __fastcall PathMaskListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall PathMaskListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PathMaskHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
          const TRect &Rect, bool Pressed);
	void __fastcall PathMaskHeaderResize(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	__fastcall TPathMaskDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPathMaskDlg *PathMaskDlg;
//---------------------------------------------------------------------------
#endif
