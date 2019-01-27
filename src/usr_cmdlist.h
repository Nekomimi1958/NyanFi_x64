//----------------------------------------------------------------------//
//ƒRƒ}ƒ“ƒhˆê——															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrCmdListH
#define UsrCmdListH

//---------------------------------------------------------------------------
#include <vcl.h>
#include <tchar.h>
#include "usr_str.h"

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
