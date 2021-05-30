//----------------------------------------------------------------------//
//ƒRƒ}ƒ“ƒhˆê——															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrCmdListH
#define UsrCmdListH

//---------------------------------------------------------------------------
#include "usr_str.h"

//---------------------------------------------------------------------------
#define HASH_ALG_LIST	_T("MD5\nSHA1\nSHA256\nSHA384\nSHA512\nCRC32\n")

//---------------------------------------------------------------------------
extern UnicodeString ScrModeIdStr;

//---------------------------------------------------------------------------
void set_CmdList(TStringList *c_list, TStringList *s_list);
void get_PrmList(UnicodeString cmd, int id_idx, TStringList *p_list, TComboBox *cp = NULL);
bool need_RefDirFile(UnicodeString cmd);

UnicodeString del_CmdDesc(UnicodeString cmd);
UnicodeString get_CsrKeyCmd(UnicodeString key_str);

//---------------------------------------------------------------------------
inline UnicodeString get_CmdStr(UnicodeString s)
{
	return starts_AT(s)? s : get_tkn(s, '_');
}

inline UnicodeString get_PrmStr(UnicodeString s)
{
	return get_tkn_r(s, '_');
}
//---------------------------------------------------------------------------
#endif
