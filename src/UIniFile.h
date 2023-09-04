//----------------------------------------------------------------------//
// INIファイル処理クラス												//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrIniFileH
#define UsrIniFileH

//---------------------------------------------------------------------------
#include <Vcl.Grids.hpp>

//----------------------------------------------------------------------
extern UnicodeString SCT_General;
extern UnicodeString SCT_Option;

//----------------------------------------------------------------------
class UsrIniFile
{
private:
	TStringList *AddSection(UnicodeString sct);
	TStringList *OfsDlgList;

	void Clear();
	void LoadValues();

public:
	UnicodeString FileName;
	TStringList *SectionList;
	TStringList *MarkIdxList;
	bool Modified;
	int PosOffsetLeft;
	int PosOffsetTop;

	UsrIniFile(UnicodeString fnam);
	~UsrIniFile();

	void Reload();

	void EraseSection(UnicodeString sct);
	bool DeleteKey(UnicodeString sct, UnicodeString key);
	bool KeyExists(UnicodeString sct, UnicodeString key);
	void RenameKey(UnicodeString sct, UnicodeString old_key, UnicodeString new_key);
	void ReplaceKey(UnicodeString sct, UnicodeString s0, UnicodeString s1);
	bool RectionExists(UnicodeString sct);
	void ReadSection(UnicodeString sct, TStringList *lst);
	void AssignSection(UnicodeString sct, TStringList *lst);

	UnicodeString ReadString(UnicodeString sct, UnicodeString key,
		UnicodeString def = EmptyStr, bool del_quot = true);
	UnicodeString ReadStrGen(const _TCHAR *key, UnicodeString def = EmptyStr, bool del_quot = true);

	int  ReadInteger(UnicodeString sct, UnicodeString key, int def = 0);
	int  ReadScaledInteger(UnicodeString sct, UnicodeString key, int def = 0, TControl *cp = NULL);
	int  ReadIntGen(const _TCHAR *key, int def = 0);
	int  ReadScaledIntGen(const _TCHAR *key, int def, TControl *cp = NULL);

	bool ReadBool(UnicodeString sct, UnicodeString key, bool def = false);
	bool ReadBoolGen(const _TCHAR *key, bool def = false);

	TColor ReadColor(UnicodeString sct, UnicodeString key, TColor def = clBlack);
	TColor ReadColGen(const _TCHAR *key, TColor def = clBlack);

	TFont* ReadFontInf(const _TCHAR *sct, TFont *def = NULL, TStringList *lst = NULL, const _TCHAR *dsc = NULL);

	void WriteString(UnicodeString sct, UnicodeString key, UnicodeString v);
	void WriteString(UnicodeString sct, UnicodeString key, TEdit *ep);
	void WriteString(UnicodeString sct, UnicodeString key, TLabeledEdit *ep);
	void WriteStrGen(const _TCHAR *key, UnicodeString v);
	void WriteStrGen(const _TCHAR *key, TEdit *ep);
	void WriteStrGen(const _TCHAR *key, TLabeledEdit *ep);

	void WriteInteger(UnicodeString sct, UnicodeString key, int v);
	void WriteInteger(UnicodeString sct, UnicodeString key, TComboBox *cp);
	void WriteInteger(UnicodeString sct, UnicodeString key, TRadioGroup *rp);
	void WriteScaledInteger(UnicodeString sct, UnicodeString key, int v, TControl *cp = NULL);
	void WriteIntGen(const _TCHAR *key, int v);
	void WriteIntGen(const _TCHAR *key, TComboBox *cp);
	void WriteIntGen(const _TCHAR *key, TRadioGroup *rp);
	void WriteScaledIntGen(const _TCHAR *key, int v, TControl *cp = NULL);

	void WriteBool(UnicodeString sct, UnicodeString key, bool v);
	void WriteBool(UnicodeString sct, UnicodeString key, TCheckBox *cp);
	void WriteBoolGen(const _TCHAR *key, bool v);
	void WriteBoolGen(const _TCHAR *key, TCheckBox *cp);
	void WriteBoolGen(const _TCHAR *key, TAction *ap);

	void WriteFontInf(UnicodeString sct, TFont *f);

	void LoadFormPos(TForm *frm, int w, int h, UnicodeString prfx = EmptyStr);
	void SaveFormPos(TForm *frm);
	void LoadPosInfo(TForm *frm, int x, int y, int w, int h, UnicodeString key = EmptyStr);
	void LoadPosInfo(TForm *frm, bool dlg_center = false, UnicodeString key = EmptyStr);
	void SavePosInfo(TForm *frm, UnicodeString key = EmptyStr);

	void LoadGridColWidth(TStringGrid *gp, int cnt, ...);
	void SaveGridColWidth(TStringGrid *gp);

	void LoadComboBoxItems(TComboBox *cp, const _TCHAR *sct, int max_items = 20, bool del_quot = false);
	void SaveComboBoxItems(TComboBox *cp, const _TCHAR *sct, int max_items = 20);

	void LoadListItems(UnicodeString sct, TStringList *lst, int max_items = 20, bool del_quot = true);
	void SaveListItems(UnicodeString sct, TStringList *lst, int max_items = 20);

	void UpdateMarkIdxList();
	void CheckMarkItems();
	void ClearAllMark();
	bool FileMark(UnicodeString fnam, int flag = -1, UnicodeString memo = EmptyStr);
	bool IsMarked(UnicodeString fnam);
	UnicodeString MarkedInArc(UnicodeString arc_file, UnicodeString xlist = EmptyStr);
	UnicodeString GetMarkMemo(UnicodeString fnam);

	bool UpdateFile(bool force = false);
};
//----------------------------------------------------------------------
extern UsrIniFile  *IniFile;

//----------------------------------------------------------------------
#endif
