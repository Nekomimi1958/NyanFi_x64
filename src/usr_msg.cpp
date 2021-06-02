//----------------------------------------------------------------------//
// メッセージ															//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_msg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//メッセージ文字列の取得
//---------------------------------------------------------------------------
UnicodeString LoadUsrMsg(int id, UnicodeString s)
{
	std::unique_ptr<TStringList> msg_lst(new TStringList());
	msg_lst->Text =
		"1001=%sが見つかりません。\n"							//USTR_NotFound
		"1002=%sこの操作には対応していません。\n"				//USTR_OpeNotSuported
		"1003=この操作はできません。\n"							//USTR_CantOperate
		"1004=対応していない形式です。\n"						//USTR_FmtNotSuported
		"1005=一時解凍に失敗しました。\n"						//USTR_FaildTmpUnpack
		"1006=移動先が同じです。\n"								//USTR_SameMoveDest
		"1007=コピー先が同じです。\n"							//USTR_SameCopyDest
		"1008=ディレクトリが含まれています。\n"					//USTR_IncludeDir
		"1009=ディレクトリにアクセスできません。\n"				//USTR_CantAccessDir
		"1010=不正な日付または時刻です。\n"						//USTR_IllegalDate
		"1011=不正な指定です。\n"								//USTR_IllegalParam
		"1012=不正な形式です。\n"								//USTR_IllegalFormat
		"1013=不正なアドレスです\n"								//USTR_IllegalAddress
		"1014=起動に失敗しました。\n"							//USTR_FaildExec
		"1015=処理に失敗しました。\n"							//USTR_FaildProc
		"1016=読み込みに失敗しました。\n"						//USTR_FaildLoad
		"1017=%sの保存に失敗しました。\n"						//USTR_FaildSave
		"1018=削除に失敗しました。\n"							//USTR_FaildDelete
		"1019=コピーに失敗しました。\n"							//USTR_FaildCopy
		"1020=メニューが実行できません。\n"						//USTR_FaildMenu
		"1021=中断しました。\n"									//USTR_Canceled
		"1022=ワークリストを開けません。\n"						//USTR_WlistCantOpen
		"1023=処理中にこの操作はできません。\n"					//USTR_ProcBusy
		"1024=対象がありません。\n"								//USTR_NoObject
		"1025=正規表現に誤りがあります。\n"						//USTR_IllegalRegEx
		"1026=書式文字列の入力\n"								//USTR_InputFmtStr
		"1027=すでに登録されています。\n"						//USTR_Registered
		"1028=　検索中...／　ESCキーで中断します。\n"			//USTR_SearchingESC
		"1029=　計算中...／　ESCキーで中断します。\n"			//USTR_CalculatingESC
		"1030=　処理中...／　ESCキーで中断します。\n"			//USTR_ProcessingESC
		"1031=　処理中...／　しばらくお持ちください。\n"		//USTR_WaitForReady
		"1032=レスポンスファイルの作成に失敗しました。\n"		//USTR_FaildListFile
		"1033=一時ディレクトリが作成できません。\n"				//USTR_CantMakeTmpDir
		"1034=不正な構文です。\n"								//USTR_SyntaxError
		"1035=対応する制御文が見つかりません。\n"				//USTR_BadStatmet
		"1036=パラメータが指定されていません。\n"				//USTR_NoParameter
		"1037=アプリケーションが見つかりません。\n"				//USTR_AppNotFound
		"1038=画像の準備ができていません。\n"					//USTR_ImgNotReady
		"1039=ファイルを開けません。\n"							//USTR_FileNotOpen
		"1040=ディレクトリが見つかりません。\n"					//USTR_DirNotFound
		"1041=無効なコマンドです。\n"							//USTR_InvalidCmd
		"1042=. で区切って複数指定可能\n"						//USTR_HintMltFExt
		"1043=; で区切って複数指定可能\n"						//USTR_HintMltSepSC
		"1044=対応するエディタがありません\n"					//USTR_NoEditor
		"1045=再生できません。\n"								//USTR_CantPlay
		"1046=%sに名前を付けて保存\n"							//USTR_SaveAs
		"1047=ディレクトリ[%s]が作成できません。\n"				//USTR_CantCreDir
		"1048=%sをコピーしますか?\n"							//USTR_CopyQ
		"1049=%sを削除しますか?\n"								//USTR_DeleteQ
		"1050=%sを完全削除しますか?\n"							//USTR_CompDeleteQ
		"1051=%sの履歴をすべて削除しますか?\n"					//USTR_DelHistoryQ
		"1052=すべての場所のすべてのマークを解除しますか?\n"	//USTR_ClrAllMarkQ
		"1053=コマンドを中断しますか?\n"						//USTR_CancelCmdQ
		"1054=アーカイブを開けません。\n"						//USTR_ArcNotOpen
		"1055=テキストファイルではありません。\n"				//USTR_NotText
		"1056=%sを抽出しますか?\n"								//USTR_Extract
		"1057=%s情報が取得できません。\n"						//USTR_CantGetInfo
		"1058=名前が重複しています。\n"							//USTR_DuplicateName
		"1059=上書きしてもよいですか?\n"						//USTR_OverwriteQ
		"1060=%sをアップロードしますか?\n"						//USTR_UploadQ
		"1061=%sをダウンロードしますか?\n"						//USTR_DownloadQ
		"1062=削除開始  %s\n"									//USTR_BeginDelete
		"1063=同名ファイルの処理 <%s>\n"						//USTR_SameName
		"1064=名前の変更\n"										//USTR_Rename
		"1065=ディレクトリ属性の異なる同名対象が存在します。\n"	//USTR_NgSameName
		"1066=不正な日付条件です。\n"							//USTR_IllegalDtCond
		"1067=選択マスク中のワークリストは変更できません。\n"	//USTR_WorkFiltered
		"1068=左右が同一ディレクトリです。\n"					//USTR_SameDirLR
		"1069=Gitの作業ディレクトリではありません。\n"			//USTR_NotRepository
		"1070=設定パネルを隠す\n"								//USTR_HideOptPanel
		"1071=設定パネルを表示\n"								//USTR_ShowOptPanel
		"1072=%s コマンドで変更可能\n"							//USTR_HintOptCmd
		"1073=サウンド識別子でも可\n"							//USTR_HintSndID

		"1100=選択項目\n"										//USTR_SelectedItem
		"1101=一覧\n"											//USTR_List
		"1102=アップロード\n"									//USTR_Upload
		"1103=ダウンロード\n";									//USTR_Download

	UnicodeString ret_str = ReplaceStr(msg_lst->Values[IntToStr(id)], "／", "\r\n");
	ret_str = s.IsEmpty()? replace_regex(ret_str, _T("%s[のをがに]?"), null_TCHAR) : ReplaceStr(ret_str, "%s", s);
	return ret_str;
}
//---------------------------------------------------------------------------
UnicodeString LoadUsrMsg(int id, const _TCHAR *s)
{
	return LoadUsrMsg(id, UnicodeString(s));
}
//---------------------------------------------------------------------------
UnicodeString LoadUsrMsg(int id, int id_s)
{
	return LoadUsrMsg(id, LoadUsrMsg(id_s));
}

//---------------------------------------------------------------------------
//ユーザ定義メッセージの EAbort を送出
//---------------------------------------------------------------------------
void UserAbort(unsigned id)
{
	throw EAbort(LoadUsrMsg(id));
}

//---------------------------------------------------------------------------
void SysErrAbort(unsigned id)
{
	throw EAbort(SysErrorMessage(id));
}
//---------------------------------------------------------------------------
void LastErrAbort()
{
	throw EAbort(SysErrorMessage(GetLastError()));
}
//---------------------------------------------------------------------------
void TextAbort(const _TCHAR *msg)
{
	throw EAbort(UnicodeString(msg));
}
//---------------------------------------------------------------------------
void SkipAbort()
{
	throw EAbort("SKIP");
}
//---------------------------------------------------------------------------
void CancelAbort()
{
	throw EAbort("CANCELED");
}
//---------------------------------------------------------------------------
void EmptyAbort()
{
	throw EAbort(EmptyStr);
}

//---------------------------------------------------------------------------
//確認メッセージ
//---------------------------------------------------------------------------
bool SureCopy;					//コピー
bool SureMove;					//移動
bool SureDelete;				//削除
bool SureUnPack;				//解凍
bool SureExec;					//実行
bool SureOther;					//その他
bool SureExit;					//終了
bool SureCmpDel;				//完全削除は2度確認
bool SureWorkList;				//ワークリストの再読み込み

//---------------------------------------------------------------------------
