//----------------------------------------------------------------------//
// ���[�_���\�����ʗp�X�N���[��											//
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
__published:	// IDE �ŊǗ������R���|�[�l���g
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// ���[�U�[�錾

public:		// ���[�U�[�錾
	__fastcall TModalScrForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TModalScrForm *ModalScrForm;
//---------------------------------------------------------------------------
#endif
