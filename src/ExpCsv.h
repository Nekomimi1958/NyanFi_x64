//----------------------------------------------------------------------//
// NyanFi																//
//  CSV/TSVエクスポート													//
//----------------------------------------------------------------------//
#ifndef ExpCsvH
#define ExpCsvH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "TxtViewer.h"
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TExpCsvDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddAllItemAction;
	TAction *AddItemAction;
	TAction *ExportAction;
	TActionList *ActionList1;
	TButton *AddAllBtn;
	TButton *AddBtn;
	TButton *CanButton;
	TButton *ClrItemBtn;
	TButton *DelItemBtn;
	TButton *DownItemBtn;
	TButton *OkButton;
	TButton *RefOutNameBtn;
	TButton *UpItemBtn;
	TCheckBox *QuotCheckBox;
	TLabel *Label1;
	TLabeledEdit *OutNameEdit;
	TListBox *DstListBox;
	TListBox *SrcListBox;
	TPanel *MidPanel;
	TPanel *OpePanel;
	TRadioButton *CsvRadioBtn;
	TRadioButton *TsvRadioBtn;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ExportActionExecute(TObject *Sender);
	void __fastcall ExportActionUpdate(TObject *Sender);
	void __fastcall AddItemActionExecute(TObject *Sender);
	void __fastcall AddItemActionUpdate(TObject *Sender);
	void __fastcall AddAllItemActionExecute(TObject *Sender);
	void __fastcall SrcListBoxDblClick(TObject *Sender);
	void __fastcall DstListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DstListBoxDblClick(TObject *Sender);
	void __fastcall ItemListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RefOutNameBtnClick(TObject *Sender);

private:	// ユーザー宣言
	bool isTSV;
	bool ExpBusy;

public:		// ユーザー宣言
	TTxtViewer *Viewer;

	__fastcall TExpCsvDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TExpCsvDlg *ExpCsvDlg;
//---------------------------------------------------------------------------
#endif
