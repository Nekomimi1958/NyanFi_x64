//----------------------------------------------------------------------//
// スプラッシュ・フォーム												//
//----------------------------------------------------------------------//
#ifndef SplashH
#define SplashH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Graphics.hpp>

//---------------------------------------------------------------------------
class TSplashForm : public TForm
{
__published:	// IDE 管理のコンポーネント
	TLabel *MsgLabel;
	TLabel *TitleLabel;
	TLabel *VerLabel;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	void __fastcall SetMsgLabel(const _TCHAR *msg);

	__fastcall TSplashForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
