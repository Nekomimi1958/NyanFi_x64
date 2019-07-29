//----------------------------------------------------------------------//
// NyanFi																//
//  �f�B���N�g������/ �X�^�b�N											//
//----------------------------------------------------------------------//
#ifndef HistDlgH
#define HistDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TDirHistoryDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *AddDirsAction;
	TAction *ClearAllAction;
	TAction *ClearFltAction;
	TAction *CopyAllAction;
	TAction *PropertyAction;
	TActionList *ActionList1;
	TLabel *HintLabel1;
	TLabel *HintLabel2;
	TListBox *DirHistListBox;
	TMenuItem *AddDirsItem;
	TMenuItem *ClearAllItem;
	TMenuItem *ClearFltItem;
	TMenuItem *CopyAllItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *PropertyItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TPaintBox *InpPaintBox;
	TPanel *HintPanel;
	TPanel *InpPanel;
	TPanel *ListPanel;
	TPopupMenu *HistPopupMenu;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DirHistListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall DirHistListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall DirHistListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall DirHistListBoxDblClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ClearAllActionExecute(TObject *Sender);
	void __fastcall ClearAllActionUpdate(TObject *Sender);
	void __fastcall DirHistListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall AddDirsActionExecute(TObject *Sender);
	void __fastcall AddDirsActionUpdate(TObject *Sender);
	void __fastcall InpPaintBoxPaint(TObject *Sender);
	void __fastcall ClearFltActionExecute(TObject *Sender);
	void __fastcall ClearFltActionUpdate(TObject *Sender);
	void __fastcall CopyAllActionExecute(TObject *Sender);
	void __fastcall CopyAllActionUpdate(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	TStringList *ListBuff;

	UnicodeString IncSeaWord;
	bool IsMigemo;

	bool FindBusy;
	bool FindAborted;

	void __fastcall UpdateListBox(int idx = 0);
	void __fastcall AddDirsCore(UnicodeString pnam, TStrings *lst, TStringDynArray ex_lst);

public:		// ���[�U�[�錾
	UsrScrollPanel *ListScrPanel;	//�V���v���X�N���[���o�[

	bool IsDirStack;		//�f�B���N�g���E�X�^�b�N
	bool IsAllDirHist;		//�S�̗���
	bool IsFindDirHist;		//�S�̗���(�C���N�������^���T�[�`)
	bool IdRecentDir;		//�ŋߎg�����f�B���N�g��

	int  RefListTag;
	UnicodeString WorkListName;

	__fastcall TDirHistoryDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDirHistoryDlg *DirHistoryDlg;
//---------------------------------------------------------------------------
#endif
