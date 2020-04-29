//----------------------------------------------------------------------//
// メッセージ															//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrMsgH
#define UsrMsgH

//---------------------------------------------------------------------------
#include <vcl.h>

//---------------------------------------------------------------------------
#define USTR_NotFound		1001
#define USTR_OpeNotSuported	1002
#define USTR_CantOperate	1003
#define USTR_FmtNotSuported	1004
#define USTR_FaildTmpUnpack	1005
#define USTR_SameMoveDest	1006
#define USTR_SameCopyDest	1007
#define USTR_IncludeDir		1008
#define USTR_CantAccessDir	1009
#define USTR_IllegalDate	1010
#define USTR_IllegalParam	1011
#define USTR_IllegalFormat	1012
#define USTR_IllegalAddress	1013
#define USTR_FaildExec		1014
#define USTR_FaildProc		1015
#define USTR_FaildLoad		1016
#define USTR_FaildSave		1017
#define USTR_FaildDelete	1018
#define USTR_FaildCopy		1019
#define USTR_FaildMenu		1020
#define USTR_Canceled		1021
#define USTR_WlistCantOpen	1022
#define USTR_ProcBusy		1023
#define USTR_NoObject		1024
#define USTR_IllegalRegEx	1025
#define USTR_InputFmtStr	1026
#define USTR_Registered		1027
#define USTR_SearchingESC	1028
#define USTR_CalculatingESC 1029
#define USTR_ProcessingESC	1030
#define USTR_WaitForReady	1031
#define USTR_FaildListFile	1032
#define USTR_CantMakeTmpDir	1033
#define USTR_SyntaxError	1034
#define USTR_BadStatmet		1035
#define USTR_NoParameter	1036
#define USTR_AppNotFound	1037
#define USTR_ImgNotReady	1038
#define USTR_FileNotOpen	1039
#define USTR_DirNotFound	1040
#define USTR_InvalidCmd		1041
#define USTR_HintMltFExt	1042
#define USTR_HintMltSepSC	1043
#define USTR_NoEditor		1044
#define USTR_CantPlay		1045
#define USTR_SaveAs			1046
#define USTR_CantCreDir		1047
#define USTR_CopyQ			1048
#define USTR_DeleteQ		1049
#define USTR_CompDeleteQ	1050
#define USTR_DelHistoryQ	1051
#define USTR_ClrAllMarkQ	1052
#define USTR_CancelCmdQ		1053
#define USTR_ArcNotOpen		1054
#define USTR_NotText		1055
#define USTR_Extract		1056
#define USTR_CantGetInfo	1057
#define USTR_DuplicateName	1058
#define USTR_OverwriteQ		1059
#define USTR_UploadQ		1060
#define USTR_DownloadQ		1061
#define USTR_BeginDelete	1062
#define USTR_SameName		1063
#define USTR_Rename			1064
#define USTR_NgSameName		1065
#define USTR_IllegalDtCond	1066
#define USTR_WorkFiltered	1067
#define USTR_SameDirLR		1068
#define USTR_NotRepository	1069
#define USTR_HideOptPanel	1070
#define USTR_ShowOptPanel	1071
#define USTR_OptCmdHint		1072

#define USTR_SelectedItem	1100
#define USTR_List			1101
#define USTR_Upload			1102
#define USTR_Download		1103

UnicodeString LoadUsrMsg(int id, UnicodeString s = EmptyStr);
UnicodeString LoadUsrMsg(int id, const _TCHAR *s);
UnicodeString LoadUsrMsg(int id, int id_s);

void UserAbort(unsigned id);
void SysErrAbort(unsigned id);
void LastErrAbort();
void TextAbort(const _TCHAR *msg);
void SkipAbort();
void CancelAbort();
void EmptyAbort();

//---------------------------------------------------------------------------
extern bool SureCopy;
extern bool SureMove;
extern bool SureDelete;
extern bool SureUnPack;
extern bool SureExec;
extern bool SureOther;
extern bool SureExit;
extern bool SureCmpDel;
extern bool SureWorkList;

//---------------------------------------------------------------------------
#endif
