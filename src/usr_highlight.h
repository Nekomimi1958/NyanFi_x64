//----------------------------------------------------------------------//
// 構文強調表示															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrHighlightH
#define UsrHighlightH

//---------------------------------------------------------------------------
#include "UIniFile.h"

//---------------------------------------------------------------------------
// 構文強調表示定義ファイルのクラス
//---------------------------------------------------------------------------
class HighlightFile : public UsrIniFile
{
private:
	TDateTime HighlightTime;	//現在のタイムスタンプ

	void CheckValues();

public:
	UnicodeString CurSection;	//使用中のセクション
	TStringList   *ErrorList;

	HighlightFile(UnicodeString fnam);
	~HighlightFile();

	UnicodeString ReadKeyStr(const _TCHAR *key);
	int			  ReadKeyInt(const _TCHAR *key, int def = 0);
	bool		  ReadKeyBool(const _TCHAR *key, bool def = false);
	UnicodeString ReadRegExPtn(const _TCHAR *key);
	TColor		  ReadColorRGB6H(const _TCHAR *key, TColor def = clBlack);

	bool Recycle();

	bool GetSection(UnicodeString fnam,
			bool is_clip = false, bool is_log = false, bool is_h2t = false);

	bool GetCommentList(UnicodeString fnam,
			TStringList *ln_lst, TStringList *bgn_lst, TStringList *end_lst,
			bool is_h2t = false, bool only_def = false);

	int  GetCommentPos(UnicodeString fnam, TListBox *lp, int idx, bool en_mlt = true);

	UnicodeString GetDefHeadlnPtn(UnicodeString fext);
	UnicodeString GetHeadlinePtn(UnicodeString fnam, TStringList *hl_lst = NULL, bool only_def = false);
};
//---------------------------------------------------------------------------
extern HighlightFile *UserHighlight;

//---------------------------------------------------------------------------
UnicodeString GetDefReservedPtn(UnicodeString fext, bool &sns_case, bool is_xml, bool is_log, bool is_h2t);
UnicodeString GetDefNumericPtn(UnicodeString fext);
UnicodeString GetDefSymbolChars(UnicodeString fext, bool is_xml, bool is_h2t);
UnicodeString GetDefQuotChars(UnicodeString fext, bool &use_esc, bool is_xml, bool is_ini, bool is_h2t);
UnicodeString GetDefFunctionPtn(UnicodeString fext, UnicodeString &name_ptn, bool is_h2t);
bool GetSearchPairPtn(UnicodeString fext, TStringList *lst);
bool GetDefaultHighlight(UnicodeString fext, TStringList *lst, TStringList *hl_lst = NULL);
//---------------------------------------------------------------------------
#endif
