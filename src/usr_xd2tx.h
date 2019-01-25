//----------------------------------------------------------------------//
// xdoc2txt																//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrXDoc2TxtH
#define UsrXDoc2TxtH

//---------------------------------------------------------------------------
typedef int (*FUNC_ExtractText)(BSTR, bool, BSTR*);		//！cdecl 呼び出し

//---------------------------------------------------------------------------
//！.html .eml は除外
#define FEXT_XD2T_OPN	_T(".sxw.sxc.sxi.sxd.odt.ods.odp.odg")
#define FEXT_XD2T_MSO	_T(".docx.docm.xlsx.xlsm.pptx.pptm.doc.xls.ppt")
#define FEXT_XD2T_ETC	_T(".jaw.jtw.jbw.juw.jfw.jvw.jtd.jtt.oas.oa2.oa3.bun.wj2.wj3.wk3.wk4.123.pdf.mht")
#define FEXT_XD2T_PRP	FEXT_XD2T_MSO _T(".sxw.sxc.sxi.sxd.jtd.jtt")	//プロパティ取得可能
#define FEXT_XD2TX		FEXT_XD2T_OPN FEXT_XD2T_MSO FEXT_XD2T_ETC		//抽出可能

//---------------------------------------------------------------------------
extern bool xd2tx_Available;
extern UnicodeString xd2tx_FileName;

//---------------------------------------------------------------------------
bool xd2tx_Initialize();

bool xd2tx_TestExt(UnicodeString fext, bool inc_rtf = false);
bool xd2tx_PropExt(UnicodeString fext);

bool xd2tx_Extract(UnicodeString fnam, TStringList *lst, bool prop = false);
bool xd2tx_GetInfo(UnicodeString fnam, TStringList *lst);

//---------------------------------------------------------------------------
#endif
