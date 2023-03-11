//----------------------------------------------------------------------//
// ルーペ																//
//----------------------------------------------------------------------//
#ifndef LoupeFrmH
#define LoupeFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>

//---------------------------------------------------------------------------
#define MINHI_LOUPE 42

//---------------------------------------------------------------------------
class TLoupeForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *Mag1Action;
	TAction *Mag2Action;
	TAction *Mag4Action;
	TAction *Mag8Action;
	TAction *ShowBtnAction;
	TActionList *ActionList1;
	TImage *LoupeImage;
	TMenuItem *Mag1Item;
	TMenuItem *Mag2Item;
	TMenuItem *Mag4Item;
	TMenuItem *Mag8Item;
	TMenuItem *Sep_p_1;
	TMenuItem *ShowBtnItem;
	TPanel *ClientPanel;
	TPanel *ImgPanel;
	TPanel *OptPanel;
	TPopupMenu *PopupMenu1;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TShape *BorderShape3;
	TToolBar *MagToolBar;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormEndDock(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall ToolBtnClick(TObject *Sender);
	void __fastcall LoupeImageMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall MagActionExecute(TObject *Sender);
	void __fastcall MagActionUpdate(TObject *Sender);
	void __fastcall ShowBtnActionExecute(TObject *Sender);
	void __fastcall ShowBtnActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	void __fastcall WmSysCommand(TWMSysCommand & SysCom)
	{
		if (SysCom.CmdType==SC_CLOSE) {	//[×]ボタンが押された
			Close();
			ShowLoupe = false;
		}
		else TForm::Dispatch(&SysCom);
	}

public:		// ユーザー宣言
	__fastcall TLoupeForm(TComponent* Owner);

	void __fastcall SaveOption();
	void __fastcall DrawImage(Graphics::TBitmap *bmp = NULL, int x = 0, int y = 0);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_SYSCOMMAND,	TWMSysCommand,	WmSysCommand)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TLoupeForm *LoupeForm;
//---------------------------------------------------------------------------
#endif
