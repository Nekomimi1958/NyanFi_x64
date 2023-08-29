//----------------------------------------------------------------------//
// NyanFi																//
//  �}�X�N/�}�b�`�I���_�C�A���O											//
//----------------------------------------------------------------------//
#ifndef MaskSelDlgH
#define MaskSelDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

//---------------------------------------------------
class TMaskSelectDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TButton *CanButton;
	TButton *OkButton;
	TComboBox *MaskSelComboBox;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// ���[�U�[�錾
	bool IsMask;

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

public:		// ���[�U�[�錾
	UnicodeString CmdName;

	__fastcall TMaskSelectDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMaskSelectDlg *MaskSelectDlg;
//---------------------------------------------------------------------------
#endif
