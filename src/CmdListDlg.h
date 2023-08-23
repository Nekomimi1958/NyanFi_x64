//----------------------------------------------------------------------//
// NyanFi																//
//  コマンドファイル一覧												//
//----------------------------------------------------------------------//
#ifndef CmdListDlgH
#define CmdListDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
class TCmdFileListDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CnfExeAction;
	TAction *EditCopyAction;
	TAction *EditSelectAllAction;
	TAction *MigemoAction;
	TAction *PreviewAction;
	TAction *ReqEditAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	TButton *OkBtn;
	TEdit *FilterEdit;
	THeaderControl *CmdFileHeader;
	TListBox *PreviewListBox;
	TListBox *ReferListBox;
	TMenuItem *FitSizePosItem;
	TMenuItem *RefEditCopyItem;
	TMenuItem *RefEditSelAllItem;
	TMenuItem *Sep_1;
	TPanel *GridPanel;
	TPanel *MainPanel;
	TPanel *OpePanel;
	TPanel *PreviewPanel;
	TPanel *PrevPanel;
	TPanel *ReferPanel;
	TPanel *ToolPanel;
	TPopupMenu *ListPopupMenu;
	TSplitter *FilterSplitter;
	TSplitter *PreviewSplitter;
	TSplitter *ReferSplitter;
	TStringGrid *CmdFileGrid;
	TToolBar *OpeToolBar;
	TToolButton *CnfExeBtn;
	TToolButton *FilterBtn;
	TToolButton *MigemoBtn;
	TToolButton *PreviewBtn;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CmdFileHeaderResize(TObject *Sender);
	void __fastcall CmdFileHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall CmdFileHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
		const TRect &Rect, bool Pressed);
	void __fastcall CmdFileGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall FilterBtnClick(TObject *Sender);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall MigemoActionExecute(TObject *Sender);
	void __fastcall CnfExeActionExecute(TObject *Sender);
	void __fastcall CmdFileGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CmdFileGridDblClick(TObject *Sender);
	void __fastcall CmdFileGridClick(TObject *Sender);
	void __fastcall PreviewListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall PreviewListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PreviewActionExecute(TObject *Sender);
	void __fastcall ReferListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall OkBtnClick(TObject *Sender);
	void __fastcall EditCopyActionExecute(TObject *Sender);
	void __fastcall EditSelectAllActionExecute(TObject *Sender);
	void __fastcall EditCopyActionUpdate(TObject *Sender);
	void __fastcall ReqEditActionExecute(TObject *Sender);
	void __fastcall ReqEditActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TStringList *GridItemList;
	TStringList *cmdfile_List;

	bool ToSelect;
	bool KeyHandled;

	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall UpdateList();
	void __fastcall UpdateGrid();
	int  __fastcall GetGridIndex();

public:		// ユーザー宣言
	UsrScrollPanel *GridScrPanel;	//シンプルスクロールバー
	UsrScrollPanel *PrevScrPanel;
	UsrScrollPanel *ReferScrPanel;

	UnicodeString CmdFileName;
	bool ToFilter;

	__fastcall TCmdFileListDlg(TComponent* Owner);

	int __fastcall ShowToSelect(UnicodeString fnam = EmptyStr);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TCmdFileListDlg *CmdFileListDlg;
//---------------------------------------------------------------------------
#endif
