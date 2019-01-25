//----------------------------------------------------------------------//
// NyanFi																//
//  Edit �R�}���h�p�����{�b�N�X											//
//----------------------------------------------------------------------//
#ifndef MemoFrmH
#define MemoFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Buttons.hpp>

//-----------------------------------------------------
class TMemoForm : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *DelLineAction;
	TAction *PasteAction;
	TAction *UndoAction;
	TAction *WebSearchAction;
	TActionList *ActionList1;
	TButton *CancelBtn;
	TButton *OkBtn;
	TMenuItem *ColBgItem;
	TMenuItem *ColDefItem;
	TMenuItem *ColFgItem;
	TMenuItem *ColorItem;
	TMenuItem *CopyItem;
	TMenuItem *CutItem;
	TMenuItem *DelItem;
	TMenuItem *DelLineItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *PasteItem;
	TMenuItem *SelAllItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep3;
	TMenuItem *UndoItem;
	TMenuItem *WebSearchItem;
	TPanel *BlankPanel;
	TPanel *BtnPanel;
	TPanel *OptPanel;
	TPopupMenu *PopupMenu1;
	TRichEdit *MemoBox;
	TSpeedButton *HideOptBtn;
	TSpeedButton *ShowOptBtn;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall WebSearchActionExecute(TObject *Sender);
	void __fastcall WebSearchActionUpdate(TObject *Sender);
	void __fastcall PasteActionExecute(TObject *Sender);
	void __fastcall PasteActionUpdate(TObject *Sender);
	void __fastcall ChgOptBtnClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ColBgItemClick(TObject *Sender);
	void __fastcall ColFgItemClick(TObject *Sender);
	void __fastcall ColDefItemClick(TObject *Sender);
	void __fastcall UndoActionExecute(TObject *Sender);
	void __fastcall UndoActionUpdate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall DelLineActionExecute(TObject *Sender);
	void __fastcall DelLineActionUpdate(TObject *Sender);
	void __fastcall MemoBoxChange(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

private:	// ���[�U�[�錾
	bool OptVisible;		//�{�^���p�l���̕\�����
	TColor color_MemoBg;	//�w�i�F
	TColor color_MemoFg;	//�����F

	void __fastcall WmFormShowed(TMessage &msg);

public:		// ���[�U�[�錾
	TStringList *LinesBuff;

	__fastcall TMemoForm(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,		TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMemoForm *MemoForm;
//---------------------------------------------------------------------------
#endif
