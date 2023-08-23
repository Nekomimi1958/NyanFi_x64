//----------------------------------------------------------------------//
// NyanFi																//
//  .nyanfi ファイルの設定												//
//----------------------------------------------------------------------//
#ifndef DotDlgH
#define DotDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Mask.hpp>
#include "usr_swatch.h"
#include <Vcl.Imaging.pngimage.hpp>

//---------------------------------------------------------------------------
class TDotNyanDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CreNyanAction;
	TAction *DelNyanAction;
	TAction *DisableColAction;
	TActionList *ActionList1;
	TButton *CancelBtn;
	TButton *CreNyanBtn;
	TButton *DelNyanBtn;
	TButton *DisableBtn;
	TButton *InhCmdBtn;
	TButton *InhColBtn;
	TButton *InheritBtn;
	TButton *InhImgBtn;
	TButton *InhSndBtn;
	TButton *RefCmdsBtn;
	TButton *RefColBtn;
	TButton *RefImgBtn;
	TButton *RefSndBtn;
	TButton *TestSndBtn;
	TCheckBox *DscAttrCheckBox;
	TCheckBox *DscNameCheckBox;
	TCheckBox *HandledCheckBox;
	TCheckBox *HideCheckBox;
	TCheckBox *NaturalCheckBox;
	TCheckBox *NoOderCheckBox;
	TCheckBox *OldCheckBox;
	TCheckBox *SmallCheckBox;
	TComboBox *ColorComboBox;
	TComboBox *GrepMaskComboBox;
	TComboBox *PathMaskComboBox;
	TEdit *BgImgEdit;
	TEdit *SoundEdit;
	TGroupBox *CmdsGroupBox;
	TGroupBox *ColGroupBox;
	TGroupBox *GroupBox2;
	TGroupBox *ImgGroupBox;
	TGroupBox *SndGroupBox;
	TImage *SpuitImage;
	TLabel *InheritLabel;
	TLabel *Label1;
	TLabeledEdit *DescEdit;
	TLabeledEdit *ExeCmdsEdit;
	TLabeledEdit *ListWdEdit;
	TPanel *SpuitPanel;
	TRadioGroup *HideRadioGroup;
	TRadioGroup *IconRadioGroup;
	TRadioGroup *SizeRadioGroup;
	TRadioGroup *SortRadioGroup;
	TRadioGroup *SyncRadioGroup;
	TRadioGroup *SysRadioGroup;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall NoOderCheckBoxClick(TObject *Sender);
	void __fastcall RefSndBtnClick(TObject *Sender);
	void __fastcall TestSndBtnClick(TObject *Sender);
	void __fastcall RefImgBtnClick(TObject *Sender);
	void __fastcall ColorComboBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall RefColBtnClick(TObject *Sender);
	void __fastcall SpuitImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SpuitImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall DisableColActionExecute(TObject *Sender);
	void __fastcall DisableColActionUpdate(TObject *Sender);
	void __fastcall RefCmdsBtnClick(TObject *Sender);
	void __fastcall CancelBtnClick(TObject *Sender);
	void __fastcall CreNyanActionExecute(TObject *Sender);
	void __fastcall CreNyanActionUpdate(TObject *Sender);
	void __fastcall InhSndBtnClick(TObject *Sender);
	void __fastcall InhImgBtnClick(TObject *Sender);
	void __fastcall InhColBtnClick(TObject *Sender);
	void __fastcall InhCmdBtnClick(TObject *Sender);
	void __fastcall InheritBtnClick(TObject *Sender);
	void __fastcall DelNyanActionExecute(TObject *Sender);
	void __fastcall DelNyanActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	bool NyanExists;
	TStringList *ColBufList;
	TStringList *InheritList;
	UsrSwatchPanel *SwatchPanel;

	void __fastcall SetOderOption(TStringList *lst);
	void __fastcall SetColorOption(TStringList *lst);

public:		// ユーザー宣言
	UnicodeString DotNyanName;

	__fastcall TDotNyanDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDotNyanDlg *DotNyanDlg;
//---------------------------------------------------------------------------
#endif
