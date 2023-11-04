//----------------------------------------------------------------------//
// NyanFi																//
//  タスク処理スレッド													//
//----------------------------------------------------------------------//
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "usr_wic.h"
#include "usr_exif.h"
#include "usr_excmd.h"
#include "usr_tag.h"
#include "UserFunc.h"
#include "Global.h"
#include "task_thread.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//タスク設定クラス
//---------------------------------------------------------------------------
TaskConfig::TaskConfig()
{
	TaskList   = new TStringList();
	UpdDirList = new TStringList();

	CopyAll    = false;
	CopyMode   = 0;
	DstPosMode = 0;
	KeepTime   = false;
	NoSubDir   = false;
	FilterMode = 0;

	CvImg_f_ext 	   = ".jpg";
	CvImg_quality	   = 80;
	CvImg_ycrcb 	   = 0;
	CvImg_cmp_mode	   = 0;
	CvImg_scale_mode   = 0;
	CvImg_scale_prm1   = 100;
	CvImg_scale_prm2   = 100;
	CvImg_scale_opt    = 0;
	CvImg_from_clip    = false;
	CvImg_not_use_prvw = false;
	CvImg_grayscale    = false;
	CvImg_mgn_color    = clBlack;

	Bakup_sub_sw   = false;
	Bakup_mirror   = false;

	Distribute_sw  = false;

	DirComp_sw	   = false;
	DirComp_ext_sw = true;
}
//---------------------------------------------------------------------------
TaskConfig::~TaskConfig()
{
	delete TaskList;
	delete UpdDirList;
}

//---------------------------------------------------------------------------
void TaskConfig::Assign(TaskConfig *cp)
{
	TaskList->Assign(cp->TaskList);
	UpdDirList->Assign(cp->UpdDirList);

	CopyAll 		   = cp->CopyAll;
	CopyMode		   = cp->CopyMode;
	DstPosMode		   = cp->DstPosMode;

	CmdStr			   = cp->CmdStr;
	InfStr			   = cp->InfStr;

	CvImg_f_ext 	   = cp->CvImg_f_ext;
	CvImg_quality	   = cp->CvImg_quality;
	CvImg_ycrcb 	   = cp->CvImg_ycrcb;
	CvImg_cmp_mode	   = cp->CvImg_cmp_mode;
	CvImg_scale_mode   = cp->CvImg_scale_mode;
	CvImg_scale_prm1   = cp->CvImg_scale_prm1;
	CvImg_scale_prm2   = cp->CvImg_scale_prm2;
	CvImg_scale_opt    = cp->CvImg_scale_opt;
	CvImg_from_clip    = cp->CvImg_from_clip;
	CvImg_not_use_prvw = cp->CvImg_not_use_prvw;
	CvImg_grayscale    = cp->CvImg_grayscale;
	CvImg_mgn_color    = cp->CvImg_mgn_color;
}

//---------------------------------------------------------------------------
// タスク設定リスト (TList から継承)
//---------------------------------------------------------------------------
__fastcall TaskConfigList::TaskConfigList(): TList()
{
}
//---------------------------------------------------------------------------
__fastcall TaskConfigList::~TaskConfigList()
{
	ClearAll();
}

//---------------------------------------------------------------------------
//マークをすべてクリア
//---------------------------------------------------------------------------
void __fastcall TaskConfigList::ClearAll()
{
	for (int i=0; i<Count; i++) delete Items[i];
	Clear();
}

//---------------------------------------------------------------------------
int TaskIdCount = 0;

//---------------------------------------------------------------------------
//スレッドの初期化
//---------------------------------------------------------------------------
__fastcall TTaskThread::TTaskThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpNormal;
	FreeOnTerminate = true;

	TaskRWLock = new TMultiReadExclusiveWriteSynchronizer();

	TaskObjList = new TStringList();

	TaskReady	 = false;
	TaskFinished = false;
	TaskCancel	 = false;
	TaskPause	 = false;
	TaskAskSame  = false;

	TaskIsFast	 = ReqTaskSlow = false;
	DirDeleted	 = LnkDeleted = false;

	PreCount = SubCount = OkCount = SkipCount = RenCount = ErrCount = 0;
	CurTotalSize = 0;
	CurProgress  = -1;

	Config = new TaskConfig();

	IsSamePath = false;

	Tag = 0;
	ID  = TaskIdCount++;

	LastTransferred.QuadPart = 0;
	SmplSize  = 0;
	LastCount = 0;
	Speed	  = 0;

	ErrCancel = false;
}

//---------------------------------------------------------------------------
//MoveFileWithProgress、CopyFileEx 用コールバック関数
//---------------------------------------------------------------------------
DWORD CALLBACK TTaskThread::ProgressRoutine(
	LARGE_INTEGER TotalFileSize,
	LARGE_INTEGER TotalBytesTransferred,
	LARGE_INTEGER StreamSize,
	LARGE_INTEGER StreamBytesTransferred,
	DWORD dwStreamNumber,
	DWORD dwCallbackReason,
	HANDLE hSourceFile,
	HANDLE hDestinationFile,
	LPVOID lpData)
{
	return ((TTaskThread*)lpData)->ProgressCore(TotalFileSize, TotalBytesTransferred);
}
//---------------------------------------------------------------------------
DWORD __fastcall TTaskThread::ProgressCore(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred)
{
	//一旦停止中の処理
	if (TaskPause) {
		WaitIfPause();
		TaskIsFast = false;
		return TaskCancel? PROGRESS_CANCEL : PROGRESS_CONTINUE;
	}
	//中断
	if (TaskCancel) return PROGRESS_CANCEL;

	//進捗率(0.0～1.0)
	CurProgress  = (TotalFileSize.QuadPart>0)? 1.0*TotalBytesTransferred.QuadPart/TotalFileSize.QuadPart : -1;
	CurTotalSize = TotalFileSize.QuadPart;

	//転送速度を計算
	SmplSize += (TotalBytesTransferred.QuadPart - LastTransferred.QuadPart);
	LastTransferred = TotalBytesTransferred;
	int cur_cnt  = GetTickCount();
	int smpl_cnt = (cur_cnt - LastCount);
	if (smpl_cnt>1000) {	//***
		LastCount = cur_cnt;
		Speed = SmplSize / smpl_cnt;
		SmplSize = 0;
	}

	//残り時間を計算
	RemCount = (Speed>0)? (TotalFileSize.QuadPart-TotalBytesTransferred.QuadPart)/Speed : 0;

	//待ち時間 (小サイズファイルの処理高速化のため、最初と最後には待ちを入れない)
	if (CurProgress>0.0 && CurProgress<1.0) {
		if (ReqTaskSlow) {
			ReqTaskSlow = false;
			Sleep(SLOW_INTERVAL);
		}
		else {
			TaskIsFast = (NopDtctTime>0) && ((int)(GetTickCount() - LastOpCount) > NopDtctTime*1000);
			if (TaskIsFast)
				Sleep((Speed>67108)? 0 : MIN_INTERVAL);	//*** 64MB/秒以上なら 0
			else
				Sleep(std::max(IsRemote? RemoteWaitTime : NormalWaitTime, MIN_INTERVAL));
		}
	}

	return PROGRESS_CONTINUE;
}

//---------------------------------------------------------------------------
//ログにメッセージを追加
//---------------------------------------------------------------------------
void __fastcall TTaskThread::AddLog(UnicodeString msg, bool must_log)
{
	if (!LogErrOnly || must_log || StartsStr("E ", msg) || StartsStr("C ", msg)) {
		UnicodeString s;
		if (!msg.IsEmpty()) s.sprintf(_T("%u>%s"), Tag + 1, msg.c_str());
		LogRWLock->BeginWrite();
			LogBufStr.cat_sprintf(_T("%s\r\n"), s.c_str());
		LogRWLock->EndWrite();
	}
}
//---------------------------------------------------------------------------
//ログにデバッグ情報を追加
//---------------------------------------------------------------------------
void __fastcall TTaskThread::AddDebugLog(UnicodeString msg, UnicodeString info)
{
	if (!LogDebugInf) return;

	TFormatSettings fs = TFormatSettings::Create();
	msg = "       ! " + FormatDateTime("hh:nn:ss.zzz ", Now(), fs) + msg;
	if (!info.IsEmpty()) msg.cat_sprintf(_T(" %s"), info.c_str());
	AddLog(msg);
}

//---------------------------------------------------------------------------
//ファイルが操作可能になるまで待つ(最大10秒、中断可)
//  プレビュー読込中の場合の対処
//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_wait_file_ready(UnicodeString fnam)
{
	int wait = 20;
	while (!TaskCancel && wait>0
		&& ImgViewThread && !ImgViewThread->IsReady() && SameText(fnam, ImgViewThread->FileName))
	{
		if (wait==20) AddDebugLog("Wait for File Ready");
		Sleep(500); wait--;
	}
	return (wait>0);
}

//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_file_exists(UnicodeString fnam)
{
	AddDebugLog("File Exists");

	if (fnam.IsEmpty()) return false;
	return (file_GetAttr(fnam) != faInvalid);
}

//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_dir_exists(UnicodeString dnam)
{
	AddDebugLog("Dir Exists");

	if (dnam.IsEmpty()) return false;
	return ::PathIsDirectory(cv_ex_filename(dnam).c_str());
}

//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_set_writable(UnicodeString fnam)
{
	if (!ForceDel) return true;

	bool ret = true;
	if (file_exists(fnam)) {
		AddDebugLog("Check Attribute");
		int atr = file_GetAttr(fnam);
		if (atr!=faInvalid && (atr & faReadOnly)) {
			AddDebugLog("Set Attribute");
			atr &= ~faReadOnly;
			ret = file_SetAttr(fnam, atr);
		}
	}
	return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_delete_File(UnicodeString fnam, bool use_trash)
{
	AddDebugLog("Call DeleteFile", use_trash? "(Trasn Can)" : "");
	bool res = delete_File(fnam, use_trash);
	AddDebugLog("Return");
	return res;
}
//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_delete_Dir(UnicodeString dnam)
{
	AddDebugLog("Call RemoveDirectory");
	bool res = ::RemoveDirectory(cv_ex_filename(dnam).c_str());
	AddDebugLog("Return");
	if (res) usr_TAG->DelItem(dnam);
	return res;
}

//---------------------------------------------------------------------------
//ディレクトリ下のマスクに該当する全ファイルを取得
//---------------------------------------------------------------------------
int __fastcall TTaskThread::GetFiles(
	UnicodeString pnam,		//ディレクトリ名
	UnicodeString mask,		//マスク
	TStrings *lst,			//結果を格納リスト
	bool empSW,				//空のサブディレクトリも取得 (default = false)
	bool symSW)				//シンボリックリンク内も	 (default = false)
{
	int fcnt = 0;
	if (!dir_exists(pnam) || (!symSW && is_SymLink(pnam))) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	//サブディレクトリを検索
	UnicodeString sea_str;
	TSearchRec sr;
	sea_str = cv_ex_filename(pnam + "*");
	if (!TaskCancel && FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			if ((sr.Attr & faDirectory)==0) continue;

			UnicodeString dnam = sr.Name;
			if (ContainsStr("..", dnam)) continue;
			if (!symSW && is_SymLink(pnam + dnam)) continue;
			int scnt = GetFiles(pnam + dnam, mask, lst, empSW, symSW);
			fcnt += scnt;
			if (empSW && scnt==0) {
				//空のサブディレクトリ
				lst->Add(IncludeTrailingPathDelimiter(pnam + dnam));
				PreCount = lst->Count;
			}
		} while(!TaskCancel && FindNext(sr)==0);
		FindClose(sr);
	}

	//ファイルを検索
	sea_str = cv_ex_filename(pnam + mask);
	if (!TaskCancel && FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			if (sr.Attr & faDirectory) continue;

			//タイムスタンプによるフィルタリング
			if (Config->FilterMode>0) {
				TValueRelationship res = System::Dateutils::CompareDate(sr.TimeStamp, Config->FilterTime);
				bool ok = false;
				switch (Config->FilterMode) {
				case 1: ok = (res==LessThanValue);		break;
				case 2: ok = (res==EqualsValue);		break;
				case 3: ok = (res==GreaterThanValue);	break;
				}
				if (!ok) continue;
			}

			lst->Add(pnam + sr.Name);
			fcnt++;
			PreCount = lst->Count;
		} while(!TaskCancel && FindNext(sr)==0);
		FindClose(sr);
	}

	return fcnt;
}
//---------------------------------------------------------------------------
//ディレクトリ下のマスクに該当する全ファイルのリストを追加取得
//  戻り値: ファイル数(空ディレクトリは除く)
//---------------------------------------------------------------------------
int __fastcall TTaskThread::GetFilesEx(
	UnicodeString pnam,			//ディレクトリ名
	UnicodeString inc_mask,		//対象マスク(;で区切り複数指定可)
	UnicodeString exc_mask,		//除外マスク(;で区切り複数指定可)
	TStrings *lst,				//結果を格納する TStrings
	bool subSW,					//サブディレクトリも検索				(default = fals)
	UnicodeString skip_dir,		//除外ディレクトリ(;で区切り複数指定可)	(default = EmptyStr)
	TStringList *d_lst,			//サブディレクトリのリスト				(default = NUL)
	bool symSW,					//シンボリックリンク内も		 		(default = false)
	bool dateSW)				//日付条件を利用						(default = false)
{
	int fcnt = 0;
	if (!dir_exists(pnam) || (!symSW && is_SymLink(pnam))) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);

	//除外ディレクトリ
	TStringDynArray skip_dir_lst = split_strings_semicolon(skip_dir);
	//マスク
	if (inc_mask.IsEmpty()) inc_mask = "*";

	TStringDynArray inc_msk_lst = split_strings_semicolon(inc_mask);	//対象
	TStringDynArray exc_msk_lst = split_strings_semicolon(exc_mask);	//除外

	TSearchRec sr;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			//サブディレクトリを検索
			if (subSW && (sr.Attr & faDirectory)) {
				UnicodeString dnam = sr.Name;
				if (ContainsStr("..", dnam)) continue;
				if (!symSW && is_SymLink(pnam + dnam)) continue;

				if (d_lst) d_lst->Add(pnam + dnam);

				//除外ディレクトリのチェック
				bool skip = false;
				for (int i=0; i<skip_dir_lst.Length; i++) {
					if (skip_dir_lst[i].IsEmpty()) continue;
					if (str_match(skip_dir_lst[i], dnam)) {
						skip = true; break;
					}
				}

				if (!skip) fcnt += GetFilesEx(pnam + dnam, inc_mask, exc_mask, lst, subSW, skip_dir, d_lst, symSW, dateSW);
			}
			//ファイルを検索
			else {
				//対象チェック
				bool ok = false;
				for (int i=0; i<inc_msk_lst.Length; i++) {
					if (inc_msk_lst[i].IsEmpty()) continue;
					if (str_match(inc_msk_lst[i], sr.Name)) {
						ok = true; break;
					}
				}
				if (!ok) continue;
				//除外チェック
				for (int i=0; i<exc_msk_lst.Length; i++) {
					if (exc_msk_lst[i].IsEmpty()) continue;
					if (str_match(exc_msk_lst[i], sr.Name)) {
						ok = false; break;
					}
				}
				if (!ok) continue;

				//タイムスタンプによるフィルタリング
				if (dateSW && Config->FilterMode>0) {
					TValueRelationship res = System::Dateutils::CompareDate(sr.TimeStamp, Config->FilterTime);
					ok = false;
					switch (Config->FilterMode) {
					case 1: ok = (res==LessThanValue);		break;
					case 2: ok = (res==EqualsValue);		break;
					case 3: ok = (res==GreaterThanValue);	break;
					}
					if (!ok) continue;
				}

				lst->Add(pnam + sr.Name); fcnt++;
				PreCount = lst->Count;
			}
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}

	return fcnt;
}

//---------------------------------------------------------------------------
//ディレクトリ下の全ディレクトリを取得
//---------------------------------------------------------------------------
void __fastcall TTaskThread::GetDirs(UnicodeString pnam, TStrings *lst)
{
	if (pnam.IsEmpty() || !dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);
	UnicodeString sea_str = cv_ex_filename(pnam + "*");
	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			if ((sr.Attr & faDirectory)==0) continue;
			UnicodeString dnam = sr.Name;
			if (ContainsStr("..", dnam)) continue;
			lst->Add(pnam + dnam);
			PreCount = lst->Count;
			GetDirs(pnam + dnam, lst);
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//ファイルのコピー/移動
//---------------------------------------------------------------------------
void __fastcall TTaskThread::CPY_core(
	UnicodeString fnam, 		//コピー元
	UnicodeString dst_path,		//コピー先
	bool mov_sw,				//true = 移動/ false = コピー
	bool remove_ro,				//true = 読込専用属性を解除
	TStringList *skip_list)		//スキップしたディレクトリ(移動処理後に削除しない)
								//	(default = NULL)
{
	WaitIfPause();

	UnicodeString msg;

	//コピー先がなければ作成
	if (!dir_exists(dst_path)) {
		msg = make_LogHdr(_T("CREATE"), dst_path, true);
		SetLastError(NO_ERROR);
		if (create_ForceDirs(dst_path)) {
			UnicodeString src_dnam = ExtractFileDir(fnam);
			dir_CopyAttr(src_dnam, dst_path, remove_ro);
			if (CopyTags) usr_TAG->Copy(src_dnam, dst_path);
			if (mov_sw) move_FolderIcon(src_dnam, dst_path); else copy_FolderIcon(src_dnam, dst_path);
		}
		else set_LogErrMsg(msg);
		AddLog(msg);
	}

	if (ends_PathDlmtr(fnam)) return;

	msg = make_LogHdr(mov_sw? _T("MOVE") : _T("COPY"), fnam);
	if (Config->Distribute_sw) {
		UnicodeString dnam = ExcludeTrailingPathDelimiter(dst_path);
		if (!LogFullPath) remove_top_text(dnam, Config->DistPath);
		msg.cat_sprintf(_T(" --> [%s]"), dnam.c_str());
	}
	else {
		cat_DestDir(msg, dst_path);
	}

	CurFileName = fnam;
	CopyName	= ExtractFileName(fnam);
	CurProgress = -1;

	UnicodeString dst_fnam = dst_path + CopyName;

	//同名ディレクトリが存在
	if (dir_exists(dst_fnam)) {
		ErrCount++;
		set_LogErrMsg(msg, LoadUsrMsg(USTR_NgSameName));
		AddLog(msg);
		CurFileName  = EmptyStr;
		CurTotalSize = 0;
		return;
	}

	//同名処理
	if (!Config->CopyAll && file_exists(dst_fnam)) {
		CurSrcName  = fnam;
		CurDstName	= dst_fnam;
		IsSamePath	= SameText(ExtractFilePath(fnam), dst_path);
		TaskAskSame = true;
		while (TaskAskSame && !TaskCancel) Sleep(250);
	}

	//中断
	if (TaskCancel) {
		msg[1] = 'C';
		AddLog(msg);
		CurFileName = EmptyStr;
		return;
	}

	bool w_flag = true;
	if (file_exists(dst_fnam) || Config->CopyMode==CPYMD_REN_CLONE) {
		bool skiped = false;

		switch (Config->CopyMode) {
		case CPYMD_OW:	//上書き
			msg[1] = 'O';
			break;

		case CPYMD_NEW:	//最新なら
			if (is_NewerTime(get_file_age(fnam), get_file_age(dst_fnam))) {
				msg[1] = 'N';
			}
			else {
				w_flag = false;
				SkipCount++;
				if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
				skiped = true;
			}
			break;

		case CPYMD_SKIP:	//スキップ
			w_flag = false;
			SkipCount++;
			if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
			skiped = true;
			break;

		case CPYMD_AUT_REN:		//自動的に名前を変更
		case CPYMD_REN_CLONE:	//単独クローン化で改名
			dst_fnam = format_CloneName(Config->CopyFmt, fnam, dst_path);
			set_RenameLog(msg, dst_fnam);
			break;

		case CPYMD_MAN_REN:	//名前を変更
			dst_fnam = dst_path + CopyName;
			if (!file_exists(dst_fnam)) {
				set_RenameLog(msg, CopyName);
			}
			else {
				w_flag = false;
				ErrCount++;
				set_LogErrMsg(msg, LoadUsrMsg(USTR_DuplicateName));
			}
			break;

		case CPYMD_NEW_BACKUP:	//最新ならバックアップ
			if (is_NewerTime(get_file_age(fnam), get_file_age(dst_fnam))) {
				msg[1] = 'N';
			}
			else {
				w_flag = false;
				SkipCount++;
				msg = EmptyStr;
			}
			break;
		}

		if (skiped && skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	DstFileName = dst_fnam;
	if (Config->DstPosMode==1 && FirstDstName.IsEmpty()) FirstDstName = dst_fnam;
	if (Config->DstPosMode==2) LastDstName = dst_fnam;

	//上書き(改名)実行
	if (w_flag) {
		bool failed = false;
		int  err_id = NO_ERROR;

		try {
			SetLastError(NO_ERROR);
			if (!EX_set_writable(dst_fnam))		throw Exception(EmptyStr);

			LastTransferred.QuadPart = 0;
			SmplSize  = 0;
			LastCount = 0;
			Speed	  = 0;

			//移動
			if (mov_sw) {
				if (!EX_wait_file_ready(fnam))	throw Exception(EmptyStr);
				if (TaskCancel)					Abort();
				if (SameText(fnam, dst_fnam))	throw Exception(EmptyStr);

				AddDebugLog("Call MoveFileWithProgress");
				bool res = ::MoveFileWithProgress(cv_ex_filename(fnam).c_str(), cv_ex_filename(dst_fnam).c_str(),
								ProgressRoutine, (LPVOID)this,
								MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED|MOVEFILE_WRITE_THROUGH);
				if (!res) err_id = GetLastError();
				AddDebugLog("Return");
				if (!res) {
					if (TaskCancel) Abort(); else throw Exception(EmptyStr);
				}

				usr_TAG->Rename(fnam, dst_fnam);	//タグの移動
				rename_FolderIcon(fnam, dst_fnam);	//フォルダアイコンの移動
			}
			//コピー
			else {
				BOOL cancel = FALSE;	//ダミー (中断は TaskCancel で)

				//※RAMディスクとの間で小さなファイルをコピーするとエラーになる現象への対策
				//  10MB以下の場合は COPY_FILE_NO_BUFFERING を使わない
				DWORD cpyflag = (CopyNoBuffering && get_file_size(fnam)>10485760)? COPY_FILE_NO_BUFFERING : 0;

				AddDebugLog("Call CopyFileEx");
				bool res = ::CopyFileEx(cv_ex_filename(fnam).c_str(), cv_ex_filename(dst_fnam).c_str(),
								ProgressRoutine, (LPVOID)this, (LPBOOL)&cancel, cpyflag);
				if (!res) err_id = GetLastError();
				AddDebugLog("Return");
				if (!res) {
					if (TaskCancel) Abort(); else throw Exception(EmptyStr);
				}

				//タグのコピー
				if (CopyTags) usr_TAG->Copy(fnam, dst_fnam);

				//CD-ROMからの場合、読込専用属性を解除
				if (remove_ro) {
					int attr = file_GetAttr(dst_fnam);	if (attr==faInvalid) throw Exception(EmptyStr);
					AddDebugLog("Set Attribute");
					if (!file_SetAttr(dst_fnam, attr & ~faReadOnly)) throw Exception(EmptyStr);
				}

				if (test_FileExt(get_extension(dst_fnam), ".url")) del_CachedIcon(dst_fnam);
			}
			OkCount++;
		}
		//中断
		catch (EAbort &e) {
			msg[1] = 'C';  msg += get_LogErrMsg(EmptyStr, true, EmptyStr, err_id);
			failed = true;
		}
		//エラー
		catch (...) {
			ErrCount++;
			set_LogErrMsg(msg, EmptyStr, fnam, err_id);
			if (err_id==ERROR_DISK_FULL) {
				TaskCancel = true;
				ErrCancel  = true;
			}
			failed = true;
		}

		//移動に失敗したファイルのディレクトリは処理後の削除対象から除外
		if (mov_sw && failed && skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	if (!msg.IsEmpty()) AddLog(msg);

	CurProgress  = -1;
	CurFileName  = EmptyStr;
	CurTotalSize = 0;
	DstFileName  = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CPY(
	UnicodeString prm,	//コピー元 \t コピー先
	bool mov_sw)		//true = 移動/ false = コピー (default)
{
	UnicodeString src_prm  = split_pre_tab(prm);
	UnicodeString dst_path = prm;
	UnicodeString msg;

	if (src_prm.IsEmpty() || dst_path.IsEmpty()) return;

	UINT src_drv_typ = get_drive_type(src_prm);
	UINT dst_drv_typ = get_drive_type(dst_path);
	bool remove_ro	 = RemoveCdReadOnly && (src_drv_typ==DRIVE_CDROM);
	IsRemote		 = (src_drv_typ==DRIVE_REMOTE || dst_drv_typ==DRIVE_REMOTE);

	//ファイル
	if (!ExtractFileName(src_prm).IsEmpty()) {
		//タイムスタンプによるフィルタリング
		if (Config->FilterMode>0) {
			bool ok = false;
			TValueRelationship res = System::Dateutils::CompareDate(get_file_age(src_prm), Config->FilterTime);
			switch (Config->FilterMode) {
			case 1:  ok = (res==LessThanValue);		break;
			case 2:  ok = (res==EqualsValue);		break;
			case 3:  ok = (res==GreaterThanValue);	break;
			}
			if (!ok) return;
		}

		CPY_core(src_prm, dst_path, mov_sw, remove_ro);
	}
	//ディレクトリ
	else {
		bool is_same_drv = SameText(get_root_name(src_prm), get_root_name(dst_path));
		UnicodeString src_nam = ExcludeTrailingPathDelimiter(src_prm);
		UnicodeString dst_nam = dst_path + ExtractFileName(src_nam);

		//同名ファイルが存在
		if (file_exists_x(dst_nam)) {
			ErrCount++;
			msg = make_LogHdr(_T("MOVE"), src_nam);
			set_LogErrMsg(msg, LoadUsrMsg(USTR_NgSameName));
			AddLog(msg);
			return;
		}

		if (Config->DstPosMode==1 && FirstDstName.IsEmpty()) FirstDstName = dst_nam;
		if (Config->DstPosMode==2) LastDstName = dst_nam;

		//同ドライブ内での移動
		if (mov_sw && is_same_drv && !dir_exists(dst_nam)) {
			msg = make_LogHdr(_T("MOVE"), src_nam);
			try {
				//移動先がなければ作成
				if (!dir_exists(dst_path)) {
					msg = make_LogHdr(_T("CREATE"), dst_path, true);
					SetLastError(NO_ERROR);
					if (!create_ForceDirs(dst_path)) Abort();
				}
				//移動
				SetLastError(NO_ERROR);
				if (!move_FileT(src_nam, dst_nam)) throw Exception(EmptyStr);
				OkCount++;
			}
			catch (...) {
				ErrCount++;
				set_LogErrMsg(msg);
			}
			AddLog(msg);
		}
		//コピー元取得
		else {
			UnicodeString org_path = get_parent_path(src_prm);
			std::unique_ptr<TStringList> fbuf(new TStringList());
			PreCount = 0;
			GetFiles(src_prm, "*.*", fbuf.get(), true, true);
			PreCount = 0;

			//ディレクトリのタイムスタンプを取得
			std::unique_ptr<TStringList> tbuf(new TStringList());
			if (Config->KeepTime) {
				std::unique_ptr<TStringList> dbuf(new TStringList());
				dbuf->Add(src_prm);
				PreCount = 1;
				GetDirs(src_prm, dbuf.get());
				PreCount = 0;
				for (int i=0; i<dbuf->Count; i++) {
					UnicodeString dnam = dbuf->Strings[i];
					file_rec *fp = cre_new_file_rec(dnam);
					TDateTime ft = get_file_age(dnam);
					if (ft>(TDateTime)0) {
						UnicodeString sub_path = get_tkn_r(dnam, org_path);
						tbuf->Add(dst_path + sub_path + "\t" + FormatDateTime("yyyymmddhhnnss", ft));
					}
				}
			}

			if (!TaskCancel) {
				std::unique_ptr<TStringList> skip_lst(new TStringList());
				//コピー
				if (fbuf->Count>0) {
					fbuf->Sort();
					for (int i=0; i<fbuf->Count; i++) {
						UnicodeString fnam = fbuf->Strings[i];
						if (!TaskCancel) {
							SubCount = fbuf->Count - i;
							UnicodeString sub_path = fnam;
							sub_path.Delete(1, org_path.Length());
							sub_path = ExtractFilePath(sub_path);
							CPY_core(fnam, dst_path + sub_path, mov_sw, remove_ro, skip_lst.get());
							Sleep(MIN_INTERVAL);
						}
						else if (mov_sw) {
							UnicodeString dnam = ExtractFilePath(fnam);
							if (skip_lst->IndexOf(dnam)==-1) skip_lst->Add(dnam);
						}
					}
				}
				//空ディレクトリ
				else if (!dir_exists(dst_nam)) {
					msg = make_LogHdr(_T("CREATE"), dst_nam, true);
					SetLastError(NO_ERROR);
					if (create_ForceDirs(dst_nam)) {
						dir_CopyAttr(src_prm, dst_nam, remove_ro);
						if (CopyTags) usr_TAG->Copy(src_prm, dst_nam);
						if (mov_sw) move_FolderIcon(src_prm, dst_nam); else copy_FolderIcon(src_prm, dst_nam);
					}
					else {
						set_LogErrMsg(msg);
						UnicodeString dnam = IncludeTrailingPathDelimiter(src_nam);
						if (skip_lst->IndexOf(dnam)==-1) skip_lst->Add(dnam);
					}
					AddLog(msg);
				}

				//タイムスタンプ
				if (Config->KeepTime) {
					for (int i=0; i<tbuf->Count; i++) {
						UnicodeString dnam = get_pre_tab(tbuf->Strings[i]);
						UnicodeString tstr = get_post_tab(tbuf->Strings[i]);
						if (dir_exists(dnam) && TRegEx::IsMatch(tstr, "^\\d{14}$")) {
							UnicodeString msg = make_LogHdr(_T("TIME"), dnam, true, 14);
							try {
								unsigned short y = tstr.SubString( 1, 4).ToIntDef(0);
								unsigned short m = tstr.SubString( 5, 2).ToIntDef(0);
								unsigned short d = tstr.SubString( 7, 2).ToIntDef(0);
								unsigned short h = tstr.SubString( 9, 2).ToIntDef(0);
								unsigned short n = tstr.SubString(11, 2).ToIntDef(0);
								unsigned short s = tstr.SubString(13, 2).ToIntDef(0);
								TDateTime dt = TDateTime(y, m, d, h, n, s, 0);
								msg.cat_sprintf(_T("  %s"), format_DateTime(dt).c_str());
								SetLastError(NO_ERROR);
								AddDebugLog("Call SetFileTime");
								if (!set_file_age(dnam, dt, ForceDel)) set_LogErrMsg(msg);
								AddDebugLog("Return");
							}
							catch (EConvertError &e) {
								AddDebugLog(LoadUsrMsg(USTR_IllegalDate));
							}
							AddLog(msg);
						}
					}
				}

				//移動元のディレクトリ削除
				if (mov_sw) {
					DEL_dirs(src_prm, skip_lst.get());
					usr_TAG->Rename(src_prm, dst_nam);
				}
			}
		}
	}

	//エラー中断
	if (ErrCancel) AddLog("C エラーによりタスクを中断しました");
}

//---------------------------------------------------------------------------
//ディレクトリ削除
//---------------------------------------------------------------------------
void __fastcall TTaskThread::DEL_dirs(
	UnicodeString pnam,			//削除するディレクトリ
	TStringList *skip_list)		//スキップするディレクトリのリスト	(default = NULL)
{
	if (pnam.IsEmpty()) return;
	pnam = to_path_name(pnam);
	bool is_sym = is_SymLink(pnam);

	//サブディレクトリを検索して削除
	if (!is_sym) {
		UnicodeString sea_str = cv_ex_filename(pnam + "*");
		TSearchRec sr;
		if (FindFirst(sea_str, faAnyFile, sr)==0) {
			do {
				WaitIfPause();
				if ((sr.Attr & faDirectory)==0 || ContainsStr("..", sr.Name)) continue;
				DEL_dirs(pnam + sr.Name, skip_list);
			} while(FindNext(sr)==0);
			FindClose(sr);

			Sleep(MIN_INTERVAL);
		}
	}

	//ディレクトリを削除
	UnicodeString msg = make_LogHdr(is_sym? _T("REMOVE") : _T("DELETE"), pnam, true);

	try {
		bool skiped = false;
		if (skip_list) {
			for (int i=0; i<skip_list->Count && !skiped; i++)
				skiped = StartsText(pnam, skip_list->Strings[i]);
		}

		if (skiped) {
			msg = EmptyStr;
		}
		else {
			SetLastError(NO_ERROR);
			if (!EX_set_writable(pnam))	Abort();
			if (!EX_delete_Dir(pnam))	Abort();
			set_FolderIcon(pnam);
			DirDeleted = true;
		}
	}
	catch (EAbort &e) {
		ErrCount++;
		set_LogErrMsg(msg);
	}

	if (!msg.IsEmpty()) AddLog(msg);
}

//---------------------------------------------------------------------------
//ファイル削除
//---------------------------------------------------------------------------
void __fastcall TTaskThread::DEL_core(
	UnicodeString fnam,
	bool use_trash,			//ごみ箱を使用
	TStringList *skip_list)	//スキップするディレクトリ	(default = NULL)
{
	CurFileName = fnam;
	UnicodeString msg = make_LogHdr(_T("DELETE"), fnam);

	try {
		SetLastError(NO_ERROR);
		if (!EX_file_exists(fnam))		throw Exception(EmptyStr);
		if (!EX_wait_file_ready(fnam))	throw Exception(EmptyStr);
		if (TaskCancel)	 				Abort();
		if (!EX_set_writable(fnam))		throw Exception(EmptyStr);

		bool del_lnk = (!LnkDeleted && !use_trash && get_HardLinkCount(fnam)>1);
		if (!EX_delete_File(fnam, use_trash)) throw Exception(EmptyStr);
		if (del_lnk) LnkDeleted = true;

		del_CachedIcon(fnam);
		if (IniFile->IsMarked(fnam)) IniFile->FileMark(fnam, 0);

		UnicodeString snam = fnam;
		UnicodeString lnam = split_ADS_name(snam);
		if (SameText(get_extension(lnam), ".url") && ContainsText(snam, FAVICON_ADS)) del_CachedIcon(lnam);

		OkCount++;
	}
	//中断
	catch (EAbort &e) {
		msg[1] = 'C';
	}
	//エラー
	catch (...) {
		ErrCount++;
		set_LogErrMsg(msg, EmptyStr, fnam);

		if (skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	AddLog(msg);
	CurFileName = EmptyStr;
}
//---------------------------------------------------------------------------
//ごみ箱によるディレクトリ削除
//---------------------------------------------------------------------------
void __fastcall TTaskThread::DEL_dir_trash(UnicodeString dnam)
{
	UnicodeString msg = make_LogHdr(_T("DELETE"), dnam, true);
	try {
		SetLastError(NO_ERROR);
		if (!EX_set_writable(dnam)) Abort();
		if (!EX_delete_File(ExcludeTrailingPathDelimiter(dnam), true)) Abort();
		OkCount++;
	}
	catch (EAbort &e) {
		ErrCount++;
		set_LogErrMsg(msg);
	}
	AddLog(msg);
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_DEL(UnicodeString prm)
{
	if (prm.IsEmpty()) return;

	//ファイル
	if (!ExtractFileName(prm).IsEmpty()) {
		DEL_core(prm, DelUseTrash);
	}
	//ディレクトリ(末尾が \)
	else if (EX_dir_exists(prm)) {
		//ごみ箱
		if (DelUseTrash) {
			DEL_dir_trash(prm);
		}
		else {
			//シンボリックリンク、ジャンクション
			if (is_SymLink(prm)) {
				DEL_dirs(prm);
			}
			//通常
			else {
				//ディレクトリ内の削除
				std::unique_ptr<TStringList> fbuf(new TStringList());
				PreCount = 0;
				GetFiles(prm, "*.*", fbuf.get());
				PreCount = 0;

				if (!TaskCancel) {
					std::unique_ptr<TStringList> skip_lst(new TStringList());
					for (int i=0; i<fbuf->Count; i++) {
						UnicodeString fnam = fbuf->Strings[i];
						if (!TaskCancel) {
							WaitIfPause();
							SubCount = fbuf->Count - i;
							DEL_core(fnam, DelUseTrash, skip_lst.get());
							if (ReqTaskSlow) {
								ReqTaskSlow = false;
								Sleep(SLOW_INTERVAL);
							}
							else if (i%32==0) {
								Sleep(MIN_INTERVAL);
							}
						}
						else {
							UnicodeString dnam = ExtractFilePath(fnam);
							if (skip_lst->IndexOf(dnam)==-1) skip_lst->Add(dnam);
						}
					}

					if (!TaskCancel) DEL_dirs(prm, skip_lst.get());
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//ファイル完全削除
//---------------------------------------------------------------------------
void __fastcall TTaskThread::CMPDEL_core(
	UnicodeString fnam,
	TStringList *skip_list)	//スキップするディレクトリ	(default = NULL)
{
	//ハードリングの場合、通常削除
	if (get_HardLinkCount(fnam)>1) {
		DEL_core(fnam, false);
		return;
	}

	UnicodeString msg = make_LogHdr(_T("CMPDEL"), fnam);
	try {
		SetLastError(NO_ERROR);
		if (!EX_file_exists(fnam))		throw Exception(EmptyStr);
		if (!EX_wait_file_ready(fnam))	throw Exception(EmptyStr);
		if (TaskCancel)					Abort();
		if (!EX_set_writable(fnam))		throw Exception(EmptyStr);

		CurFileName = fnam;
		Randomize();
		std::unique_ptr<char[]> wbuf(new char[CMPDEL_BUFF_SIZE]);

		if		(CmpDelOwCnt<=0) CmpDelOwCnt = 3;
		else if (CmpDelOwCnt>50) CmpDelOwCnt = 50;

		__int64 f_size	   = get_file_size(fnam);
		__int64 total_sz   = f_size * CmpDelOwCnt;
		__int64 written_sz = 0;
		int wcnt = f_size / CMPDEL_BUFF_SIZE;
		int rcnt = f_size % CMPDEL_BUFF_SIZE;
		CurProgress = -1;

		AddDebugLog("CreateFile");
		HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
			GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE) throw Exception(EmptyStr);

		try {
			for (int i=0; i<CmpDelOwCnt; i++) {
				//ランダムパターン作成
				for (int j=0; j<CMPDEL_BUFF_SIZE; j++) wbuf[j] = Random(256);
				//先頭へシーク
				::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
				//書き込み
				DWORD wbyt;
				for (int j=0; j<wcnt; j++) {
					WaitIfPause();
					if (ReqTaskSlow) {
						ReqTaskSlow = false;
						Sleep(SLOW_INTERVAL);
					}
					else if (j%128==0) {
						Sleep(MIN_INTERVAL);
					}

					if (TaskCancel) throw Exception(EmptyStr);	//中断
					if (!WriteFile(hFile, wbuf.get(), CMPDEL_BUFF_SIZE, &wbyt, NULL)) throw Exception(EmptyStr);
					written_sz += wbyt;
					CurProgress = 1.0*written_sz/total_sz;
					if (Random(8)==0)
						for (int k=0; k<CMPDEL_BUFF_SIZE; k++) wbuf[k] = Random(256);	//ときどきパターン変更
				}
				//端数バイト
				if (rcnt>0) {
					if (!WriteFile(hFile, wbuf.get(), rcnt, &wbyt, NULL)) throw Exception(EmptyStr);
					written_sz += wbyt;
					CurProgress = 1.0*written_sz/total_sz;
				}
			}
			//サイズを0に切り詰める
			::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
			::SetEndOfFile(hFile);
			AddDebugLog("CloseHandle");
			::CloseHandle(hFile);
		}
		catch (...) {
			AddDebugLog("CloseHandle");
			::CloseHandle(hFile);
			if (TaskCancel) Abort();
		}

		//削除
		if (!EX_delete_File(fnam)) throw Exception(EmptyStr);

		del_CachedIcon(fnam);
		if (IniFile->IsMarked(fnam)) IniFile->FileMark(fnam, 0);

		OkCount++;
	}
	//中断
	catch (EAbort &e) {
		msg[1] = 'C';  msg += "\r\n    要求は中断されました。";
	}
	//エラー
	catch (...) {
		ErrCount++;
		set_LogErrMsg(msg, EmptyStr, fnam);

		if (skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	AddLog(msg);
	CurFileName = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CMPDEL(UnicodeString prm)
{
	if (!ExtractFileName(prm).IsEmpty()) {
		CMPDEL_core(prm);
	}
	else if (EX_dir_exists(prm)) {
		std::unique_ptr<TStringList> skip_lst(new TStringList());
		if (!is_SymLink(prm)) {
			//ディレクトリ内の削除
			std::unique_ptr<TStringList> fbuf(new TStringList());
			PreCount = 0;
			GetFiles(prm, "*.*", fbuf.get());
			PreCount = 0;

			if (!TaskCancel) {
				for (int i=0; i<fbuf->Count && !TaskCancel; i++) {
					SubCount = fbuf->Count - i;
					CMPDEL_core(fbuf->Strings[i], skip_lst.get());
					Sleep(MIN_INTERVAL);
				}
			}
		}

		if (!TaskCancel) DEL_dirs(prm, skip_lst.get());
	}
}

//---------------------------------------------------------------------------
//ディレクトリ構造のコピー
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CPYDIR(UnicodeString prm)
{
	UnicodeString src_prm  = split_pre_tab(prm);
	UnicodeString dst_path = prm;
	if (src_prm.IsEmpty() || dst_path.IsEmpty()) return;

	bool remove_ro = RemoveCdReadOnly && (get_drive_type(src_prm)==DRIVE_CDROM);

	//取得
	std::unique_ptr<TStringList> dbuf(new TStringList());
	dbuf->Add(src_prm);
	if (!Config->NoSubDir) {
		PreCount = 1;
		GetDirs(src_prm, dbuf.get());
		PreCount = 0;
	}

	//コピー(作成)
	UnicodeString org_path = get_parent_path(src_prm);
	for (int i=0; i<dbuf->Count && !TaskCancel; i++) {
		WaitIfPause();
		SubCount = dbuf->Count - i;
		UnicodeString dnam = dbuf->Strings[i];
		UnicodeString sub_path = dnam;
		sub_path.Delete(1, org_path.Length());
		UnicodeString dst_nam = dst_path + sub_path;
		UnicodeString msg = make_LogHdr(_T("CREATE"), dst_nam, true);
		if (!dir_exists(dst_nam)) {
			SetLastError(NO_ERROR);
			if (create_ForceDirs(dst_nam)) {
				//属性
				dir_CopyAttr(dnam, dst_nam, remove_ro);
				//タイムスタンプ
				if (Config->KeepTime) {
					TDateTime ft = get_file_age(dnam);
					if (ft>(TDateTime)0) set_file_age(dst_nam, ft);
				}
				OkCount++;
			}
			else {
				ErrCount++;
				set_LogErrMsg(msg);
			}
		}
		else {
			SkipCount++;
			if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
		}

		if (!msg.IsEmpty()) AddLog(msg);
	}
}

//---------------------------------------------------------------------------
//画像ファイルの変換
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CVIMG(UnicodeString prm)
{
	bool is_clip = Config->CvImg_from_clip;
	UnicodeString fnam	   = split_pre_tab(prm);
	UnicodeString dst_path = prm;

	CurFileName = fnam;
	bool is_wmf = test_MetaExt(ExtractFileExt(fnam));

	UnicodeString msg = make_LogHdr(_T("CVIMG"), fnam);

	std::unique_ptr<Graphics::TBitmap> i_img(new Graphics::TBitmap());
	std::unique_ptr<Graphics::TBitmap> r_img(new Graphics::TBitmap());
	try {
		i_img->Canvas->Lock();
		r_img->Canvas->Lock();
		try {
			WaitIfPause();
			if (!is_clip && !EX_file_exists(fnam)) throw Exception(EmptyStr);

			//処理前にタイムスタンプを取っておく
			TDateTime ft;
			if (!is_clip && Config->KeepTime) ft = get_file_age(fnam);

			//読み込み
			std::unique_ptr<TMetafile> mf(new TMetafile());
			unsigned int i_wd, i_hi;
			//クリップボード
			if (is_clip) {
				if (!Clipboard()->HasFormat(CF_BITMAP)) UserAbort(USTR_NoObject);
				i_img->Assign(Clipboard());
				i_wd = i_img->Width;
				i_hi = i_img->Height;
			}
			//メタファイル
			else if (is_wmf) {
				mf->LoadFromFile(fnam);
				i_wd = mf->Width;
				i_hi = mf->Height;
			}
			//画像ファイル
			else {
				TColor bg = test_FileExt(ExtractFileExt(fnam), ".png.gif")? Config->CvImg_mgn_color : col_None;
				if (load_ImageFile(fnam, i_img.get(), (Config->CvImg_not_use_prvw? WICIMG_FRAME : WICIMG_PREVIEW), bg)==0)
					UserAbort(USTR_FaildLoad);
				i_wd = i_img->Width;
				i_hi = i_img->Height;
			}
			if (i_wd==0 || i_hi==0) TextAbort(_T("元画像のサイズが不正です。"));

			//縮小・拡大
			if (Config->CvImg_scale_mode>0) {
				float r, r0, r1;

				//パラメータチェック
				switch (Config->CvImg_scale_mode) {
				case 1:
					if (Config->CvImg_scale_prm1==0) UserAbort(USTR_IllegalParam);
					break;
				case 5: case 6: case 7: case 8:
					if (Config->CvImg_scale_prm1==0 || Config->CvImg_scale_prm2==0) UserAbort(USTR_IllegalParam);
					break;
				default:
					if (Config->CvImg_scale_prm1==0) UserAbort(USTR_IllegalParam);
				}

				//変更サイズを求める
				switch (Config->CvImg_scale_mode) {
				case 2:	//縦横の長い方
					if (i_wd>i_hi) {	//横の方が長い
						r = 1.0 * i_hi / i_wd;
						i_wd = Config->CvImg_scale_prm1;
						i_hi = i_wd * r;
					}
					else {			//縦の方が長い
						r = 1.0 * i_wd / i_hi;
						i_hi = Config->CvImg_scale_prm1;
						i_wd = i_hi * r;
					}
					break;
				case 3:	//横サイズを指定
					r = 1.0 * i_hi / i_wd;
					i_wd = Config->CvImg_scale_prm1;
					i_hi = i_wd * r;
					break;
				case 4:	//縦サイズを指定
					r = 1.0 * i_wd / i_hi;
					i_hi = Config->CvImg_scale_prm1;
					i_wd = i_hi * r;
					break;
				case 5: case 7: //指定サイズに収める/ 余白付き
					r0 = 1.0 * i_wd / i_hi;
					r1 = 1.0 * Config->CvImg_scale_prm1 / Config->CvImg_scale_prm2;
					if (r0>r1) {
						r = 1.0 * i_hi / i_wd;
						i_wd = Config->CvImg_scale_prm1;
						i_hi = i_wd * r;
					}
					else {
						r = 1.0 * i_wd / i_hi;
						i_hi = Config->CvImg_scale_prm2;
						i_wd = i_hi * r;
					}
					break;
				case 6:	//指定サイズにストレッチ
					i_wd = Config->CvImg_scale_prm1;
					i_hi = Config->CvImg_scale_prm2;
					break;
				case 8:	//指定サイズに合わせて切り出し
					r0 = 1.0 * i_wd / i_hi;
					r1 = 1.0 * Config->CvImg_scale_prm1 / Config->CvImg_scale_prm2;
					if (r0<r1) {
						r = 1.0 * i_hi / i_wd;
						i_wd = Config->CvImg_scale_prm1;
						i_hi = i_wd * r;
					}
					else {
						r = 1.0 * i_wd / i_hi;
						i_hi = Config->CvImg_scale_prm2;
						i_wd = i_hi * r;
					}
					break;
				}

				//縮小・拡大
				//メタファイル
				if (is_wmf) {
					if (Config->CvImg_scale_mode==1) {
						//倍率指定
						float r = Config->CvImg_scale_prm1/100.0;
						r_img->SetSize(i_wd * r, i_hi * r);
					}
					else {
						//縦・横サイズ指定
						r_img->SetSize(i_wd, i_hi);
					}
					TRect rc = Rect(0, 0, r_img->Width, r_img->Height);
					r_img->Canvas->Brush->Color = Config->CvImg_mgn_color;
					r_img->Canvas->FillRect(rc);
					r_img->Canvas->StretchDraw(rc, mf.get());
				}
				//画像ファイル
				else {
					//補間する
					if (Config->CvImg_scale_opt>=0 && Config->CvImg_scale_opt<=3) {
						//倍率指定
						if (Config->CvImg_scale_mode==1) {
							if (!WIC_resize_image(i_img.get(), r_img.get(),
								Config->CvImg_scale_prm1/100.0, 0, 0, Config->CvImg_scale_opt))
									UserAbort(USTR_FaildProc);
						}
						//縦・横サイズ指定
						else {
							if (!WIC_resize_image(i_img.get(), r_img.get(), 0.0, i_wd, i_hi, Config->CvImg_scale_opt))
								UserAbort(USTR_FaildProc);
						}
					}
					//補間しない
					else {
						int r_wd = i_wd;
						int r_hi = i_hi;
						if (Config->CvImg_scale_mode==1) {
							float r = Config->CvImg_scale_prm1/100.0;
							r_wd = i_wd * r;
							r_hi = i_hi * r;
						}
						r_img->PixelFormat = pf24bit;
						r_img->SetSize(r_wd, r_hi);
						r_img->Canvas->StretchDraw(Rect(0, 0, r_wd, r_hi), i_img.get());
					}
				}

				//結果を割り当てる
				int xp, yp;
				switch (Config->CvImg_scale_mode) {
				case 7:	//余白を付ける
					xp = (Config->CvImg_scale_prm1 - i_wd)/2;
					yp = (Config->CvImg_scale_prm2 - i_hi)/2;
					i_img->SetSize(Config->CvImg_scale_prm1, Config->CvImg_scale_prm2);
					i_img->Canvas->Brush->Color = Config->CvImg_mgn_color;
					i_img->Canvas->FillRect(Rect(0, 0, Config->CvImg_scale_prm1, Config->CvImg_scale_prm2));
					i_img->Canvas->Draw(xp, yp, r_img.get());
					break;
				case 8:	//切り出し
					xp = (i_wd - Config->CvImg_scale_prm1)/2;
					yp = (i_hi - Config->CvImg_scale_prm2)/2;
					i_img->SetSize(Config->CvImg_scale_prm1, Config->CvImg_scale_prm2);
					i_img->Canvas->CopyRect(Rect(0, 0, Config->CvImg_scale_prm1, Config->CvImg_scale_prm2),
						r_img->Canvas, Rect(xp, yp, xp + Config->CvImg_scale_prm1, yp + Config->CvImg_scale_prm2));
					break;
				default:
					i_img->Handle = r_img->ReleaseHandle();
				}
			}

			//グレースケール化
			if (Config->CvImg_grayscale) WIC_grayscale_image(i_img.get());

			//ファイル名の変更
			UnicodeString bnam;
			if (is_clip) {
				bnam = Config->CvImg_clip_name;
			}
			else {
				bnam = get_base_name(fnam);
				if (!Config->CvImg_chg_name_str.IsEmpty()) {
					switch (Config->CvImg_chg_name_mode) {
					case 0: bnam  = Config->CvImg_chg_name_str + bnam;	break;
					case 1: bnam += Config->CvImg_chg_name_str;			break;
					}
				}
			}

			UnicodeString cv_nam = dst_path + bnam + Config->CvImg_f_ext;
			if (is_clip) {
				if (file_exists(cv_nam) && Config->CopyMode==CPYMD_AUT_REN) {
					cv_nam = format_CloneName(Config->CopyFmt, cv_nam, dst_path);
				}
				LastDstName = cv_nam;
			}
			cat_DestFile(msg, cv_nam);
			DstFileName = cv_nam;

			//形式変換して保存
			if (!WIC_save_image(cv_nam, i_img.get(),
				Config->CvImg_quality, Config->CvImg_ycrcb, Config->CvImg_grayscale, Config->CvImg_cmp_mode))
					UserAbort(USTR_FaildSave);

			//タイムスタンプを設定
			if (!is_clip && Config->KeepTime && ft>(TDateTime)0) set_file_age(cv_nam, ft);

			OkCount++;
		}
		catch (EAbort &e) {
			ErrCount++;
			set_LogErrMsg(msg, e.Message);
		}
		catch (...) {
			ErrCount++;
			msg[1] = 'E';
		}
	}
	__finally {
		i_img->Canvas->Unlock();
		r_img->Canvas->Unlock();
	}

	AddLog(msg);
	CurFileName = EmptyStr;
	DstFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//Jpeg ファイルから Exif 情報を削除
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_DLEXIF(UnicodeString prm)
{
	UnicodeString fnam = split_pre_tab(prm);
	UnicodeString onam = prm + ExtractFileName(fnam);

	CurFileName = fnam;
	UnicodeString msg = make_LogHdr(_T("DLEXIF"), fnam);

	try {
		int res = EXIF_DelJpgExif(fnam, onam, Config->KeepTime);
		if (res==-1) throw Exception(EmptyStr);
		if (res==1) {
			if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
			SkipCount++;
		}
		else {
			OkCount++;
		}
	}
	catch (...) {
		ErrCount++;
		msg[1] = 'E';
	}

	if (!msg.IsEmpty()) AddLog(msg);
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//HTMLヘルプ(.chm)からソースを抽出
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_EXTCHM(UnicodeString prm)
{
	UnicodeString fnam	= split_pre_tab(prm);
	CurFileName = fnam;
	UnicodeString o_dir = ExcludeTrailingPathDelimiter(prm);
	UnicodeString c_dir = ExtractFileDir(fnam);
	fnam = ExtractFileName(fnam);

	UnicodeString cmdln;
	cmdln.sprintf(_T("hh.exe -decompile %s %s"), o_dir.c_str(), fnam.c_str());
	AddLog("  EXECUTE " + cmdln);
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	si.dwFlags	   = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION pi;
	if (::CreateProcess(NULL, cmdln.c_str(), NULL, NULL, TRUE, 0, NULL, c_dir.c_str(), &si, &pi)) {
		//終了待ち
		while (::WaitForSingleObject(pi.hProcess, 50)==WAIT_TIMEOUT) Sleep(MIN_INTERVAL);
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//バックアップ
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_BACKUP(UnicodeString prm)
{
	UnicodeString src_path = split_pre_tab(prm);
	UnicodeString dst_path = prm;

	//コピー元取得
	std::unique_ptr<TStringList> src_lst(new TStringList());
	PreCount = 0;
	GetFilesEx(src_path, Config->Bakup_inc_mask, Config->Bakup_exc_mask, src_lst.get(),
				Config->Bakup_sub_sw, Config->Bakup_skip_dir, NULL, true, true);
	PreCount = 0;

	//コピー
	if (src_lst->Count>0) {
		src_lst->Sort();
		for (int i=0; i<src_lst->Count && !TaskCancel; i++) {
			SubCount = src_lst->Count - i;
			UnicodeString fnam = src_lst->Strings[i];
			UnicodeString sub_path = fnam;
			sub_path.Delete(1, src_path.Length());
			sub_path = ExtractFilePath(sub_path);
			Task_CPY(fnam + "\t" + dst_path + sub_path);
		}
	}
	if (TaskCancel) return;
	AddLog("         コピー終了" + get_ResCntStr(OkCount, ErrCount, SkipCount, 0, true));
	SubCount = OkCount = SkipCount = ErrCount = 0;

	//ミラーリング
	if (Config->Bakup_mirror) {
		UnicodeString msg;
		msg.sprintf(_T("         ミラーリング開始  %s"), dst_path.c_str());
		if (!dir_exists(src_path)) {
			set_LogErrMsg(msg, "バックアップ元が見つかりません。");
			AddLog(msg);
			return;
		}
		AddLog(msg);

		//バックアップ元にないファイルを削除
		std::unique_ptr<TStringList> dst_lst(new TStringList());
		std::unique_ptr<TStringList> dir_lst(new TStringList());
		PreCount = 0;
		GetFilesEx(dst_path, "*.*", EmptyStr, dst_lst.get(), Config->Bakup_sub_sw, EmptyStr, dir_lst.get());
		PreCount = 0;
		for (int i=0; i<dst_lst->Count && !TaskCancel; i++) {
			SubCount = dst_lst->Count - i;
			UnicodeString fnam = dst_lst->Strings[i];
			if (!file_exists(src_path + get_tkn_r(fnam, dst_path))) Task_DEL(fnam);
		}

		//バックアップ元にないディレクトリを削除
		if (Config->Bakup_sub_sw) {
			for (int i=0; i<dir_lst->Count && !TaskCancel; i++) {
				UnicodeString pnam = dir_lst->Strings[i];
				UnicodeString snam = get_tkn_r(pnam, dst_path);
				while (!snam.IsEmpty() && !dir_exists(src_path + snam) && dir_exists(pnam)) {
					Task_DEL(IncludeTrailingPathDelimiter(pnam));
					snam = get_parent_path(snam);
				}
			}
		}

		AddLog("         ミラーリング終了" + get_ResCntStr(OkCount, ErrCount, SkipCount, 0, true));
		SubCount = OkCount = SkipCount = ErrCount = 0;
	}
}

//---------------------------------------------------------------------------
//ディレクトリのタイムスタンプを最新ファイルのものに設定
//---------------------------------------------------------------------------
TDateTime __fastcall TTaskThread::SetDirTime(UnicodeString dnam)
{
	dnam = IncludeTrailingPathDelimiter(dnam);
	UnicodeString sea_str = dnam + "*";
	if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);

	TDateTime dt = 0.0;
	int i_cnt = 0;
	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		int cnt = 0;
		do {
			WaitIfPause();
			if ((sr.Attr & faDirectory)==0) continue;
			if (ContainsStr("..", sr.Name)) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			dt = std::max(dt, SetDirTime(dnam + sr.Name));
			if (++cnt%32==0) Sleep(MIN_INTERVAL);
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
		i_cnt += cnt;
	}

	//ファイル
	if (!TaskCancel && FindFirst(sea_str, faAnyFile, sr)==0) {
		int cnt = 0;
		do {
			WaitIfPause();
			if (sr.Attr & faDirectory) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			dt = std::max(dt, sr.TimeStamp);
			if (++cnt%128==0) Sleep(MIN_INTERVAL);
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
		i_cnt += cnt;
	}

	if (!TaskCancel) {
		CurFileName = ExcludeTrailingPathDelimiter(dnam);
		TDateTime o_dt = get_file_age(dnam);
		if (!WithinPastMilliSeconds(o_dt, dt, TimeTolerance) && i_cnt>0) {
			UnicodeString msg = make_LogHdr(_T("TIME"), dnam, true, 14);	//***
			msg.cat_sprintf(_T("  %s ---> %s"), format_DateTime(o_dt).c_str(), format_DateTime(dt).c_str());
			SetLastError(NO_ERROR);
			AddDebugLog("Call SetFileTime");
			if (set_file_age(dnam, dt, ForceDel)) {
				OkCount++;
			}
			else {
				ErrCount++;
				set_LogErrMsg(msg);
			}
			AddDebugLog("Return");
			AddLog(msg);
		}
		else {
			if (i_cnt==0) dt = o_dt;
			SkipCount++;
		}
	}

	return dt;
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_DIRTIME(UnicodeString prm)
{
	SetDirTime(prm);
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//ディレクトリのNTFS圧縮/解除
//---------------------------------------------------------------------------
void TTaskThread::MakeCompList(UnicodeString dnam)
{
	dnam = IncludeTrailingPathDelimiter(dnam);

	UnicodeString sea_str = dnam + "*";
	if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);
	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		int cnt = 0;
		do {
			WaitIfPause();
			if ((sr.Attr & faDirectory)==0) continue;
			if (ContainsStr("..", sr.Name)) continue;
			MakeCompList(dnam + sr.Name);
			if (++cnt%32==0) Sleep(MIN_INTERVAL);
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}

	//ファイル
	if (!TaskCancel && FindFirst(sea_str, faAnyFile, sr)==0) {
		int cnt = 0;
		do {
			WaitIfPause();
			if (sr.Attr & faDirectory) continue;

			UnicodeString fnam = dnam + sr.Name;
			if (Config->DirComp_sw && Config->DirComp_ext_sw) {
				UnicodeString fext = get_extension(fnam);
				if (!test_TxtExt(fext) && !test_FileExt(fext, ".bmp.ico.wav")) {
					bool skip = test_FileExt(fext, FEXT_ARCHIVE) || test_FileExt(fext, FExt7zDll);
					if (!skip) skip = test_FileExt(fext, FEXT_IMAGE FEXT_META);
					if (!skip) skip = test_FileExt(fext, get_img_fext());
					if (!skip) skip = is_AudioVideo(fnam);
					if (skip) {
						SkipCount++;
						continue;
					}
				}
			}

			TaskObjList->Add(fnam);
			PreCount = TaskObjList->Count;
			if (++cnt%128==0) Sleep(MIN_INTERVAL);		//***
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}

	if (!TaskCancel) {
		TaskObjList->Add(dnam);
		PreCount = TaskObjList->Count;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_DIRCOMP(UnicodeString prm)
{
	PreCount = 0;
	MakeCompList(prm);
	PreCount = 0;

	if (!TaskCancel && TaskObjList->Count>0) {
		UnicodeString msg;
		Speed = 0;
		__int64 r_size = 0;
		for (int i=0; i<TaskObjList->Count && !TaskCancel; i++) {
			WaitIfPause();

			if (ReqTaskSlow) {
				ReqTaskSlow = false;
				Sleep(SLOW_INTERVAL);
			}
			else if (i%32==0) {
				Sleep(MIN_INTERVAL);
			}

			SubCount = TaskObjList->Count - i;
			CurProgress = 1.0 * i/TaskObjList->Count;

			UnicodeString fnam = TaskObjList->Strings[i];
			bool is_dir = remove_end_s(fnam, '\\');
			CurFileName = ExtractFileDir(fnam);
			msg = make_LogHdr((Config->DirComp_sw? _T("COMP") : _T("UNCOMP")), fnam, is_dir);
			if (set_CompressAttr(fnam, Config->DirComp_sw)) {
				OkCount++;
				if (Config->DirComp_sw) {
					__int64 c_size = get_comp_size(fnam);
					if (c_size>=0) r_size += (get_file_size(fnam) - c_size);
				}
				if (is_dir) AddLog(msg);
			}
			else {
				ErrCount++;
				set_LogErrMsg(msg);
				AddLog(msg);
			}
		}

		if (r_size>0) {
			AddLog(msg.sprintf(_T("         サイズが %s 減少しました。"), get_size_str_G(r_size, 0, SizeDecDigits, 1).c_str()));
		}
	}

	CurProgress = -1;
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//タスクの開始
//---------------------------------------------------------------------------
bool __fastcall TTaskThread::TaskStart()
{
	OkCount = SkipCount = ErrCount = 0;
	CurProgress = -1;
	TaskCancel	= false;
	TaskPause	= false;
	TaskIsFast	= ReqTaskSlow = false;
	TaskReady	= true;
	StartCount	= GetTickCount();
	return true;
}

//---------------------------------------------------------------------------
//タスクの終了(中断)処理
//---------------------------------------------------------------------------
void __fastcall TTaskThread::FinishTask()
{
	//ログ表示
	float tcnt = (GetTickCount() - StartCount)/1000.0;
	TFormatSettings fs = TFormatSettings::Create();
	UnicodeString msg = FormatDateTime("hh:nn:ss ", Now(), fs);
	msg.cat_sprintf(_T("%s終了 %5.1f秒"), TaskCmdList->Values[CmdName].c_str(), tcnt);
	msg += get_ResCntStr(OkCount, ErrCount, SkipCount, 0, true);
	if (TaskCancel) {
		msg.cat_sprintf(_T("  CANCELED:%u"), Config->TaskList->Count + 1);
		Config->TaskList->Clear();
		XCMD_Aborted = true;
	}
	AddLog(msg, true);

	TaskTime	 = (int)(tcnt + 0.5);
	TaskReady	 = false;
	TaskFinished = true;
}

//---------------------------------------------------------------------------
void __fastcall TTaskThread::Execute()
{
	::CoInitialize(NULL);

	::timeBeginPeriod(MIN_INTERVAL);

	while (!Terminated) {
		try {
			if (!TaskFinished && TaskCancel) {
				FinishTask();
			}
			else if (TaskPause) {
				WaitIfPause();
			}
			else if (TaskReady) {
				if (Config->TaskList->Count>0) {
					SubCount = 0;

					if (!Config->DistPath.IsEmpty() && Config->UpdDirList->IndexOf(Config->DistPath)==-1)
						Config->UpdDirList->Add(Config->DistPath);

					UnicodeString lbuf = Config->TaskList->Strings[0];
					CmdName = split_pre_tab(lbuf);
					UnicodeString prmstr = lbuf;
					if (!CmdName.IsEmpty() && !prmstr.IsEmpty()) {
						AddDebugLog("Start:", CmdName);

						switch (idx_of_word_i("CPY|MOV|DEL|CPYDIR|CMPDEL|CVIMG|DLEXIF|EXTCHM|BACKUP|DTIME|DCOMP", CmdName)) {
						case  0: Task_CPY(prmstr);			break;
						case  1: Task_CPY(prmstr, true);	break;
						case  2: Task_DEL(prmstr);			break;
						case  3: Task_CPYDIR(prmstr);		break;
						case  4: Task_CMPDEL(prmstr);		break;
						case  5: Task_CVIMG(prmstr);		break;
						case  6: Task_DLEXIF(prmstr);		break;
						case  7: Task_EXTCHM(prmstr);		break;
						case  8: Task_BACKUP(prmstr);		break;
						case  9: Task_DIRTIME(prmstr);		break;
						case 10: Task_DIRCOMP(prmstr);		break;
						}
					}
					Config->TaskList->Delete(0);
					SubCount = 0;
					if (Config->TaskList->Count>0) Sleep(MIN_INTERVAL);
				}
				else FinishTask();
			}
			else Sleep(WAIT_INTERVAL);
		}
		catch (...) {
			ErrCount++;
			AddLog("E 例外エラー\r\n    " + SysErrorMessage(EVENT_E_INTERNALEXCEPTION));
			TaskCancel = true;
			FinishTask();
		}
	}

	::timeEndPeriod(MIN_INTERVAL);

	delete TaskObjList;
	delete Config;
	delete TaskRWLock;

	::CoUninitialize();
}
//---------------------------------------------------------------------------
