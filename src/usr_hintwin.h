//----------------------------------------------------------------------//
// ヒントウィンドウ														//
//  背景色変更のためにサブクラス化										//
//----------------------------------------------------------------------//
#ifndef UsrHintWinH
#define UsrHintWinH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>

//---------------------------------------------------------------------------
class UsrHintWindow : public THintWindow
{
private:
	void __fastcall Paint(void);

public:
	__fastcall UsrHintWindow(TComponent* AOwner) : THintWindow(AOwner) {}

	void __fastcall ActivateHintEx(UnicodeString msg, int max_w, int min_w,
		TControl *cp, TColor bg_col);
};
//---------------------------------------------------------------------------
#endif
