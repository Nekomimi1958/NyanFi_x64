//----------------------------------------------------------------------//
// CSV/TSVレコード														//
//																		//
//----------------------------------------------------------------------//
#ifndef CsvRecFrmH
#define CsvRecFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TCsvRecForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *OpenUrlAction;
	TActionList *ActionList1;
	TButton *CalcBtn;
	TButton *GraphBtn;
	TCheckBox *TopIsHeaderCheckBox;
	THeaderControl *RecordHeader;
	TMenuItem *CopyItem;
	TMenuItem *OpenUrlItem;
	TPanel *ClientPanel;
	TPanel *GridPanel;
	TPanel *OptPanel;
	TPopupMenu *PopupMenu1;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TShape *BorderShape3;
	TStringGrid *RecordGrid;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RecordHeaderResize(TObject *Sender);
	void __fastcall RecordHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall RecordHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall RecordGridDrawCell(TObject *Sender, System::LongInt ACol, System::LongInt ARow,
		TRect &Rect, TGridDrawState State);
	void __fastcall RecordGridClick(TObject *Sender);
	void __fastcall TopIsHeaderCheckBoxClick(TObject *Sender);
	void __fastcall CopyItemClick(TObject *Sender);
	void __fastcall OpenUrlActionExecute(TObject *Sender);
	void __fastcall OpenUrlActionUpdate(TObject *Sender);
	void __fastcall CalcBtnClick(TObject *Sender);
	void __fastcall GraphBtnClick(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString HeaderStr;
	UnicodeString RecordStr;
	TStringList  *DataList;

	bool IsCSV;
	int  RecIndex;
	int  DatIndex;
	int  ItemIndex;

	void __fastcall WmFormShowed(TMessage &msg);
	int __fastcall UpdateItemName();

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー

	__fastcall TCsvRecForm(TComponent* Owner);
	void __fastcall UpdateRecord(TStringList *lst = NULL, int r_idx = -1, int i_idx = -1);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCsvRecForm *CsvRecForm;
//---------------------------------------------------------------------------
#endif
