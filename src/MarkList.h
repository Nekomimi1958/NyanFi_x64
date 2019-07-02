//----------------------------------------------------------------------//
// コントロールの検索＆マーク											//
//																		//
//----------------------------------------------------------------------//
#ifndef MarkListH
#define MarkListH

//---------------------------------------------------------------------------
#include <vcl.h>

//---------------------------------------------------------------------------
//マークリスト (TList から継承)
//---------------------------------------------------------------------------
class MarkList : public TList
{
private:
	TShape * __fastcall Get(int Index)
	{
		return (TShape*)(TList::Get(Index));
	}
	void __fastcall Put(int Index, TShape *Item)
	{
		TList::Put(Index, Item);
	}

public:
	__fastcall MarkList(Classes::TComponent* AOwner);
	__fastcall ~MarkList();

	void __fastcall ClearAll();
	int  __fastcall SearchControl(TWinControl *ctrl, UnicodeString wd);
	void __fastcall ShowMark();

	double ScrScale;			//画面スケーリング

	TForm *MarkOwner;
	TColor MarkColor;

	__property TShape * Items[int Index] = {read=Get, write=Put};
};
//---------------------------------------------------------------------------
#endif
