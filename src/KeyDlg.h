//----------------------------------------------------------------------//
// NyanFi																//
//  �L�[���蓖�Ĉꗗ													//
//----------------------------------------------------------------------//
#ifndef KeyDlgH
#define KeyDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TKeyListDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *HelpCmdAction;
	TAction *SortCmdAction;
	TAction *SortKeyAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TCheckBox *CnfExeCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *ShowAllCmdCheckBox;
	THeaderControl *KeyListHeader;
	TLabeledEdit *FilterEdit;
	TMenuItem *CopyCmdItem;
	TMenuItem *CopyListItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *HelpCmdItem;
	TMenuItem *OptionItem;
	TMenuItem *SaveAsListItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *SortCmdItem;
	TMenuItem *SortDscItem;
	TMenuItem *SortKeyItem;
	TPanel *GridPanel;
	TPopupMenu *PopupMenu1;
	TStringGrid *KeyListGrid;
	TTabControl *KeyTabControl;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KeyTabControlChange(TObject *Sender);
	void __fastcall KeyListHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall KeyListHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall KeyListGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall KeyListGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KeyListGridDblClick(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ShowAllCmdCheckBoxClick(TObject *Sender);
	void __fastcall MigemoCheckBoxClick(TObject *Sender);
	void __fastcall SaveAsListItemClick(TObject *Sender);
	void __fastcall CopyListItemClick(TObject *Sender);
	void __fastcall OptionItemClick(TObject *Sender);
	void __fastcall KeyListHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall SortKeyActionUpdate(TObject *Sender);
	void __fastcall SortKeyActionExecute(TObject *Sender);
	void __fastcall KeyTabControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall KeyListHeaderResize(TObject *Sender);
	void __fastcall CopyCmdItemClick(TObject *Sender);
	void __fastcall HelpCmdActionExecute(TObject *Sender);
	void __fastcall HelpCmdActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	TStringList *CurList;
	int KeySortMode;

	void __fastcall GetStrList(TStringList *lst);

public:		// ���[�U�[�錾
	UsrScrollPanel *GridScrPanel;	//�V���v���X�N���[���o�[

	UnicodeString CommandStr;
	bool ToFilter;

	__fastcall TKeyListDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TKeyListDlg *KeyListDlg;
//---------------------------------------------------------------------------
#endif
