//----------------------------------------------------------------------//
// NyanFi																//
//  ���L�t�H���_/�T�u�f�B���N�g���I��/���C�u�����ꗗ					//
//----------------------------------------------------------------------//
#ifndef ShareDlgH
#define ShareDlgH

//---------------------------------------------------------------------------
#include <lm.h>
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Tabs.hpp>
#include <Vcl.ActnList.hpp>
#include "usr_scrpanel.h"

//-----------------------------------------------
class TNetShareDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *CopyPathAction;
	TAction *CopyPathAllAction;
	TAction *CopyUncAction;
	TAction *CopyUncAllAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TListBox *ShareListBox;
	TMenuItem *CopyPathAllItem;
	TMenuItem *CopyPathItem;
	TMenuItem *CopyUncAllItem;
	TMenuItem *CopyUncItem;
	TPanel *ListPanel;
	TPopupMenu *PopupMenu1;
	TTabControl *PathTabControl;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ShareListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall ShareListBoxDblClick(TObject *Sender);
	void __fastcall ShareListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ShareListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall PathTabControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall PathTabControlChange(TObject *Sender);
	void __fastcall CopyUncActionExecute(TObject *Sender);
	void __fastcall CopyUncActionUpdate(TObject *Sender);
	void __fastcall CopyPathActionExecute(TObject *Sender);
	void __fastcall CopyPathActionUpdate(TObject *Sender);
	void __fastcall CopyUncAllActionExecute(TObject *Sender);
	void __fastcall CopyUncAllActionUpdate(TObject *Sender);
	void __fastcall CopyPathAllActionExecute(TObject *Sender);
	void __fastcall CopyPathAllActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	UsrScrollPanel *ListScrPanel;	//�V���v���X�N���[���o�[

	bool isPC;
	int  LibIdxBase;

	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this);
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	NET_API_STATUS __fastcall GetShareList(UnicodeString cnam, TListBox *lp);
	void __fastcall UpdateShareList(UnicodeString cnam);
	void __fastcall UpdatePathList(UnicodeString pnam);

public:		// ���[�U�[�錾
	UnicodeString ComputerName;
	UnicodeString PathName;

	bool isShare;		//���L�t�H���_
	bool isSelDir;		//�f�B���N�g���I��
	bool isSelSub;		//�T�u�f�B���N�g���I��
	bool isLibrary;		//���C�u����
	bool rqRetPath;		//�I�������p�X��Ԃ�(�f�B���N�g���ύX�͍s��Ȃ�)

	__fastcall TNetShareDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TNetShareDlg *NetShareDlg;
//---------------------------------------------------------------------------
#endif
