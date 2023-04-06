//----------------------------------------------------------------------//
// NyanFi																//
//  ビットマップビュー													//
//----------------------------------------------------------------------//
#ifndef BitmapFrmH
#define BitmapFrmH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>

//---------------------------------------------------------------------------
class TBitmapForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *SetEndAdrAction;
	TAction *SetNextAdrAction;
	TAction *SetPreAdrAction;
	TAction *SetTopAdrAction;
	TAction *W128Action;
	TAction *W256Action;
	TActionList *ActionList1;
	TMenuItem *SetEndAdrItem;
	TMenuItem *SetNextAdrItem;
	TMenuItem *SetPreAdrItem;
	TMenuItem *SetTopAdr0Item;
	TMenuItem *W128Item;
	TMenuItem *W256Item;
	TPaintBox *BitmapBox;
	TPaintBox *FileMapBox;
	TPanel *BorderPanel;
	TPanel *ClientPanel;
	TPanel *FileMapPanel;
	TPanel *SttPanel;
	TPopupMenu *PopupMenu1;
	TPopupMenu *PopupMenu2;
	TScrollBox *MapScrBox;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TShape *BorderShape3;
	TSpeedButton *EndButton;
	TSpeedButton *NxtButton;
	TSpeedButton *PreButton;
	TSpeedButton *TopButton;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BitmapBoxPaint(TObject *Sender);
	void __fastcall BitmapBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall BitmapBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FileMapBoxPaint(TObject *Sender);
	void __fastcall FileMapBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FileMapBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall MapWidthActionExecute(TObject *Sender);
	void __fastcall MapWidthActionUpdate(TObject *Sender);
	void __fastcall SetTopAdrActionExecute(TObject *Sender);
	void __fastcall SetTopAdrActionUpdate(TObject *Sender);
	void __fastcall SetPreAdrActionExecute(TObject *Sender);
	void __fastcall SetPreAdrActionUpdate(TObject *Sender);
	void __fastcall SetNextAdrActionExecute(TObject *Sender);
	void __fastcall SetNextAdrActionUpdate(TObject *Sender);
	void __fastcall SetEndAdrActionExecute(TObject *Sender);
	void __fastcall SetEndAdrActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TColor BitPallet[256];		//ビットマップビュー用パレット

	void __fastcall SetMapWidth(int w);
	__int64 __fastcall GetMapAddr(int X, int Y);

public:		// ユーザー宣言
	int MapWidth;				//マップの幅

	__fastcall TBitmapForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBitmapForm *BitmapForm;
//---------------------------------------------------------------------------
#endif
