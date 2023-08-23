//----------------------------------------------------------------------//
// NyanFi																//
//  タグ名の検索														//
//----------------------------------------------------------------------//
#ifndef FindTagH
#define FindTagH

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
class TFindTagForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CopyAction;
	TAction *SelAllAction;
	TAction *TagJumpAction;
	TAction *TagViewAction;
	TActionList *ActionList1;
	TListBox *InfoListBox;
	TListBox *TagsListBox;
	TMenuItem *CopyItem;
	TMenuItem *SelAllItem;
	TMenuItem *Sep_1;
	TMenuItem *TagJumpItem;
	TMenuItem *TagViewItem;
	TPaintBox *InpPaintBox;
	TPanel *InfoPanel;
	TPanel *InpPanel;
	TPanel *ListPanel;
	TPopupMenu *PopupMenu1;
	TSplitter *ListSplitter;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InpPaintBoxPaint(TObject *Sender);
	void __fastcall TagsListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall TagsListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall TagsListBoxDataObject(TWinControl *Control, int Index, TObject *&DataObject);
	void __fastcall TagsListBoxClick(TObject *Sender);
	void __fastcall TagsListBoxDblClick(TObject *Sender);
	void __fastcall TagsListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TagsListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall InfoListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall InfoListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InfoListBoxDblClick(TObject *Sender);
	void __fastcall TagViewActionExecute(TObject *Sender);
	void __fastcall TagJumpActionExecute(TObject *Sender);
	void __fastcall PopupItemUpdate(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall SelAllActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TStringList *TagList;
	TStringList *ResListBuf;

	UnicodeString IncSeaWord;
	bool IsMigemo;

	int MaxTagWidth;

	void __fastcall ClearTagList();
	void __fastcall UpdateList();

public:		// ユーザー宣言
	UnicodeString TagFile;
	UnicodeString TagName;
	UnicodeString TagCmd;
	UnicodeString FileName;
	UnicodeString TagInfo;

	UsrScrollPanel *TagsScrPanel;	//シンプルスクロールバー
	UsrScrollPanel *InfoScrPanel;

	__fastcall TFindTagForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFindTagForm *FindTagForm;
//---------------------------------------------------------------------------
#endif
