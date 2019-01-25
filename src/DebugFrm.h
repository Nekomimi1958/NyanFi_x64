//----------------------------------------------------------------------//
// NyanFi																//
//  �f�o�b�O���														//
//----------------------------------------------------------------------//
#ifndef DebugFrmH
#define DebugFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TDebugForm : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *BreakAction;
	TAction *EditAction;
	TAction *EditCopyAction;
	TAction *ExitAction;
	TAction *GoAction;
	TAction *HelpAction;
	TAction *QuitAction;
	TAction *StepAction;
	TAction *VarListAction;
	TActionList *ActionList1;
	TListBox *PreviewListBox;
	TListBox *ReferListBox;
	TPanel *DebugPanel;
	TPanel *OpPanel;
	TPanel *PrevPanel;
	TPanel *ReferPanel;
	TSplitter *ReferSplitter;
	TToolBar *ToolBar1;
	TToolButton *SepBtn1;
	TToolButton *SepBtn2;
	TToolButton *SepBtn3;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TToolButton *ToolButton3;
	TToolButton *ToolButton4;
	TToolButton *ToolButton5;
	TToolButton *ToolButton6;
	TToolButton *ToolButton7;
	TToolButton *ToolButton8;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PreviewListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall ReferListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ReferListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall EditCopyActionExecute(TObject *Sender);
	void __fastcall EditCopyActionUpdate(TObject *Sender);
	void __fastcall StepActionExecute(TObject *Sender);
	void __fastcall StepActionUpdate(TObject *Sender);
	void __fastcall GoActionExecute(TObject *Sender);
	void __fastcall GoActionUpdate(TObject *Sender);
	void __fastcall BreakActionExecute(TObject *Sender);
	void __fastcall BreakActionUpdate(TObject *Sender);
	void __fastcall VarListActionExecute(TObject *Sender);
	void __fastcall QuitActionExecute(TObject *Sender);
	void __fastcall QuitActionUpdate(TObject *Sender);
	void __fastcall ExitActionExecute(TObject *Sender);
	void __fastcall ExitActionUpdate(TObject *Sender);
	void __fastcall HelpActionExecute(TObject *Sender);
	void __fastcall PreviewListBoxEnter(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall PreviewListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall EditActionExecute(TObject *Sender);
	void __fastcall EditActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	UsrScrollPanel *PrevScrPanel;	//�V���v���X�N���[���o�[
	UsrScrollPanel *ReferScrPanel;

	TStringList *ListBuff;

	void __fastcall UpdateToolBar();

public:		// ���[�U�[�錾
	UnicodeString FileName;

	__fastcall TDebugForm(TComponent* Owner);
	void __fastcall SetPreview(TStringList *lst, int idx);
};
//---------------------------------------------------------------------------
extern PACKAGE TDebugForm *DebugForm;
//---------------------------------------------------------------------------
#endif
