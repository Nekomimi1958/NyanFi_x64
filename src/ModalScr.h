//----------------------------------------------------------------------//
// モーダル表示効果用スクリーン											//
//																		//
//----------------------------------------------------------------------//
#ifndef ModalScrH
#define ModalScrH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//---------------------------------------------------------------------------
class TModalScrForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// ユーザー宣言

public:		// ユーザー宣言
	__fastcall TModalScrForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TModalScrForm *ModalScrForm;
//---------------------------------------------------------------------------
#endif
