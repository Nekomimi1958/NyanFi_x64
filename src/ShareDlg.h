//----------------------------------------------------------------------//
// NyanFi																//
//  共有フォルダ/サブディレクトリ選択/ライブラリ一覧					//
//----------------------------------------------------------------------//
#ifndef ShareDlgH
#define ShareDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Tabs.hpp>
#include "usr_scrpanel.h"

//-----------------------------------------------
class TNetShareDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TButton *HiddenCanBtn;
	TListBox *ShareListBox;
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
	void __fastcall ShareListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall ShareListBoxDblClick(TObject *Sender);
	void __fastcall ShareListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ShareListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall CopyUncItemClick(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall CopyUncAllItemClick(TObject *Sender);
	void __fastcall PathTabControlDrawTab(TCustomTabControl *Control, int TabIndex,
          const TRect &Rect, bool Active);
	void __fastcall PathTabControlChange(TObject *Sender);

private:	// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	bool isPC;
	int  LibIdxBase;

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall WmMenuChar(TMessage &msg);
	void __fastcall UpdateShareList(UnicodeString cnam);
	void __fastcall UpdatePathList(UnicodeString pnam);

public:		// ユーザー宣言
	UnicodeString ComputerName;
	UnicodeString PathName;

	bool isShare;		//共有フォルダ
	bool isSelDir;		//ディレクトリ選択
	bool isSelSub;		//サブディレクトリ選択
	bool isLibrary;		//ライブラリ
	bool rqRetPath;		//選択したパスを返す(ディレクトリ変更は行わない)

	__fastcall TNetShareDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TNetShareDlg *NetShareDlg;
//---------------------------------------------------------------------------
#endif
