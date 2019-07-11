//----------------------------------------------------------------------//
// NyanFi																//
//  キー検索															//
//----------------------------------------------------------------------//
#ifndef FindKeyH
#define FindKeyH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TFindKeyDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *DummyBtn;
	TLabel *KeyLabel;
	TLabel *Label1;
	TSpeedButton *CancelBtn;
	TPanel *CanInPanel;
	TPanel *CanOutPanel;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DummyBtnClick(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Shape1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// ユーザー宣言
	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	bool __fastcall IsFirstCmdKey(UnicodeString keystr);

public:		// ユーザー宣言
	TStringList *KeyList;
	UnicodeString KeyStr;

	__fastcall TFindKeyDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFindKeyDlg *FindKeyDlg;
//---------------------------------------------------------------------------
#endif
