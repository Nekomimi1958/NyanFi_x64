//----------------------------------------------------------------------//
// NyanFi																//
//  文字情報															//
//----------------------------------------------------------------------//
#ifndef ChInfFrmH
#define ChInfFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
#define CHARINF_ITEMCOUNT	9

//---------------------------------------------------------------------------
class TCharInfoForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TListBox *InfoListBox;
	TMenuItem *ChgFontItem;
	TMenuItem *CopyItem;
	TMenuItem *CopyUniChNameItem;
	TMenuItem *FontNameItem;
	TMenuItem *NumRefDecItem;
	TMenuItem *NumRefHexItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *SetBgColItem;
	TMenuItem *SetFgColItem;
	TPanel *CharPanel;
	TPanel *ClientPanel;
	TPanel *FontNamePanel;
	TPanel *InfoPanel;
	TPanel *SamplePanel;
	TPopupMenu *PopupMenu1;
	TShape *BorderShape1;
	TShape *BorderShape2;
	TSplitter *Splitter1;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall Splitter1Moved(TObject *Sender);
	void __fastcall InfoListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall CopyItemClick(TObject *Sender);
	void __fastcall CopyUniChNameItemClick(TObject *Sender);
	void __fastcall NumRefDecItemClick(TObject *Sender);
	void __fastcall NumRefHexItemClick(TObject *Sender);
	void __fastcall ChgFontItemClick(TObject *Sender);
	void __fastcall SetColItemClick(TObject *Sender);
	void __fastcall FontNameItemClick(TObject *Sender);

private:	// ユーザー宣言
	UnicodeString UniCharName;
	UnicodeString NumRefHex;
	UnicodeString NumRefDec;
	UnicodeString RefInfo;		//参照情報

	void __fastcall SetInfoHeight();
	void __fastcall SetCharFont();

public:		// ユーザー宣言
	__fastcall TCharInfoForm(TComponent* Owner);

	void __fastcall UpdateChar(UnicodeString c, UnicodeString inf = EmptyStr);
};
//---------------------------------------------------------------------------
extern PACKAGE TCharInfoForm *CharInfoForm;
//---------------------------------------------------------------------------
#endif
