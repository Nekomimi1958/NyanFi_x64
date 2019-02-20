//----------------------------------------------------------------------//
//  ドライブ使用率推移													//
//----------------------------------------------------------------------//
#ifndef DrvGraphH
#define DrvGraphH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>

//---------------------------------------------------------------------------
class TDriveGraph : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *HiddenCanBtn;
	TCheckBox *MinMaxCheckBox;
	TCheckBox *OldOdrCheckBox;
	TComboBox *DriveComboBox;
	TComboBox *SizeComboBox;
	TPaintBox *PaintBox1;
	TPanel *Panel1;
	TPanel *Panel2;
	TPanel *Panel3;
	TScrollBox *GraphScrollBox;
	TStatusBar *StatusBar1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PaintBox1Paint(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall PaintBox1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall DriveComboBoxChange(TObject *Sender);
	void __fastcall SizeComboBoxChange(TObject *Sender);
	void __fastcall OptionChanged(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);

private:	// ユーザー宣言
	bool DlgInitialized;
	TStringList *DataList;
	int  GraphTopX;
	int  BarSize;
	double MinUsed, MaxUsed;

	void __fastcall SetStatus(int idx);

public:		// ユーザー宣言
	UnicodeString DriveName;

	__fastcall TDriveGraph(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDriveGraph *DriveGraph;
//---------------------------------------------------------------------------
#endif
