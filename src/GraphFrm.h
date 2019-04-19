//----------------------------------------------------------------------//
// NyanFi																//
//  �O���t�\��															//
//----------------------------------------------------------------------//
#ifndef GraphFrmH
#define GraphFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>

//---------------------------------------------------------------------------
class TGraphForm : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *ShowAveAction;
	TAction *ShowMaxAction;
	TAction *ShowMinAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TMenuItem *CopyImgItem;
	TMenuItem *Sep_1;
	TMenuItem *ShowAveItem;
	TMenuItem *ShowMaxItem;
	TMenuItem *ShowMinItem;
	TPaintBox *PaintBox1;
	TPopupMenu *PopupMenu1;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ShowInfActionExecute(TObject *Sender);
	void __fastcall CopyImgItemClick(TObject *Sender);

private:	// ���[�U�[�錾
	long double MinItemValue;
	long double MaxItemValue;
	long double AveItemValue;
	int f_Width;	//�������̕\������

	int x_Margin;

	TStringList *ValueList;

public:		// ���[�U�[�錾
	TStringList *DataList;
	int  CsvCol;
	bool TopIsHeader;

	__fastcall TGraphForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGraphForm *GraphForm;
//---------------------------------------------------------------------------
#endif
