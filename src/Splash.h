//----------------------------------------------------------------------//
// �X�v���b�V���E�t�H�[��												//
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
__published:	// IDE �Ǘ��̃R���|�[�l���g
	TLabel *MsgLabel;
	TLabel *TitleLabel;
	TLabel *VerLabel;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	void __fastcall SetMsgLabel(const _TCHAR *msg);

	__fastcall TSplashForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TSplashForm *SplashForm;
//---------------------------------------------------------------------------
#endif
