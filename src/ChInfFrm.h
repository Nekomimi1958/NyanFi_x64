//----------------------------------------------------------------------//
// NyanFi																//
//  �������															//
//----------------------------------------------------------------------//
#ifndef ChInfFrmH
#define ChInfFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
class TCharInfoForm : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TListBox *InfoListBox;
	TMenuItem *ChgFontItem;
	TMenuItem *CopyItem;
	TPanel *CharPanel;
	TPanel *ClientPanel;
	TPanel *FontNamePanel;
	TPanel *InfoPanel;
	TPopupMenu *PopupMenu1;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TShape *BorderShape3;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall InfoListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CopyItemClick(TObject *Sender);
	void __fastcall ChgFontItemClick(TObject *Sender);

private:	// ���[�U�[�錾
	void __fastcall SetCharFont();

public:		// ���[�U�[�錾
	__fastcall TCharInfoForm(TComponent* Owner);

	void __fastcall UpdateChar(UnicodeString c);
};
//---------------------------------------------------------------------------
extern PACKAGE TCharInfoForm *CharInfoForm;
//---------------------------------------------------------------------------
#endif
