//----------------------------------------------------------------------//
// タグ管理ユニット														//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrTagManagerH
#define UsrTagManagerH

//---------------------------------------------------------------------------
#include <Vcl.CheckLst.hpp>
#include "usr_str.h"

//---------------------------------------------------------------------------
class TagManager
{
private:
	TDateTime TagDataTime;		//現在のタイムスタンプ
	bool Modified;

	TColor DefColor;			//デフォルトのタグ色

	int  GetIndex(UnicodeString fnam);
	void MakeTagNameList();

	UnicodeString GetDataTags(int idx)
	{
		return (idx!=-1)? get_post_tab(TagDataList->Strings[idx]) : EmptyStr;
	}

	UnicodeString GetDataTags(int idx, UnicodeString &fnam)
	{
		if (idx==-1) return EmptyStr;

		UnicodeString ret_str = TagDataList->Strings[idx];
		fnam = split_pre_tab(ret_str);
		return ret_str;
	}

public:
	UnicodeString FileName;
	bool SortTags;

	TStringList *TagNameList;
	TStringList *TagDataList;
	TStringList *TagColList;

	TMultiReadExclusiveWriteSynchronizer *TagRWLock;	//TagDataList の保護用

	TagManager(UnicodeString fnam);
	~TagManager();

	bool UpdateFile();
	bool Recycle();
	int  TrimData();

	UnicodeString GetTags(UnicodeString fnam);
	UnicodeString NormTags(UnicodeString tags, bool sw_add = true);
	void DelItem(UnicodeString fnam);
	void SetTags(UnicodeString fnam, UnicodeString tags);
	void AddTags(UnicodeString fnam, UnicodeString tags);
	void Rename(UnicodeString old_nam, UnicodeString new_nam);
	void Copy(UnicodeString from_nam, UnicodeString to_nam);
	int  RenTag(UnicodeString old_tag, UnicodeString new_tag);
	int  DelTagData(UnicodeString tag);

	int IniCheckList(TCheckListBox *lp, UnicodeString tnam = EmptyStr, bool count_sw = false);
	UnicodeString CheckToTags(TCheckListBox *lp, UnicodeString tags);
	void CountTags(TCheckListBox *lp, bool clr_sw = false);

	int  GetMatchList(UnicodeString tags, bool and_sw, TStringList *lst);
	int  GetAllList(TStringList *lst);
	bool HasTag(UnicodeString fnam);
	bool Match(UnicodeString fnam, UnicodeString ktags, bool and_sw);

	TColor GetColor(UnicodeString tag, TColor col_def);
	void SetColor(UnicodeString tag, TColor col);
	void DrawTags(UnicodeString tags, TCanvas *cv, int x, int y, TColor col_rev = Graphics::clNone, bool blk_sw = false);
};
//---------------------------------------------------------------------------
extern TagManager *usr_TAG;

//---------------------------------------------------------------------------
#endif
