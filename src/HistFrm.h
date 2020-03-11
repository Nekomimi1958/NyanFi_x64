//----------------------------------------------------------------------//
// NyanFi																//
//  ヒストグラム表示													//
//----------------------------------------------------------------------//
#ifndef HistFrmH
#define HistFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

//---------------------------------------------------------------------------
#define H_LR_MARGIN		6
#define H_TOP_MARGIN	4
#define H_BTM_MARGIN	8
#define CLHI_HISTOGRAM	(128 + H_TOP_MARGIN + H_BTM_MARGIN)

//---------------------------------------------------------------------------
class THistForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TImage *HistImage;
	TPanel *HistPanel;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);

private:	// ユーザー宣言
	void __fastcall WmSysCommand(TWMSysCommand & SysCom)
	{
		if (SysCom.CmdType==SC_CLOSE) {	//[×]ボタンが押された
			Close();
			ShowHistogram = false;
		}
		else TForm::Dispatch(&SysCom);
	}

public:		// ユーザー宣言
	__fastcall THistForm(TComponent* Owner);
	void __fastcall DrawHistogram(Graphics::TBitmap *i_bmp = NULL);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_SYSCOMMAND, TWMSysCommand, WmSysCommand)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE THistForm *HistForm;
//---------------------------------------------------------------------------
#endif
