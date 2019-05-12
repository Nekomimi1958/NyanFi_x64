//----------------------------------------------------------------------//
// NyanFi																//
//  �^�X�N�����X���b�h													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <mmsystem.h>
#include <System.DateUtils.hpp>
#include <Vcl.Clipbrd.hpp>
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
//�^�X�N�ݒ�N���X
//---------------------------------------------------------------------------
TaskConfig::TaskConfig()
{
	TaskList   = new TStringList();
	UpdDirList = new TStringList();

	CopyAll    = false;
	CopyMode   = 0;
	DstPosMode = 0;
	KeepTime   = false;
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
// �^�X�N�ݒ胊�X�g (TList ����p��)
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
//�}�[�N�����ׂăN���A
//---------------------------------------------------------------------------
void __fastcall TaskConfigList::ClearAll()
{
	for (int i=0; i<Count; i++) delete Items[i];
	Clear();
}

//---------------------------------------------------------------------------
int TaskIdCount = 0;

//---------------------------------------------------------------------------
//�X���b�h�̏�����
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
}

//---------------------------------------------------------------------------
//MoveFileWithProgress�ACopyFileEx �p�R�[���o�b�N�֐�
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
	//��U��~���̏���
	if (TaskPause) {
		WaitIfPause();
		TaskIsFast = false;
		return TaskCancel? PROGRESS_CANCEL : PROGRESS_CONTINUE;
	}
	//���f
	if (TaskCancel) return PROGRESS_CANCEL;

	//�i����(0.0�`1.0)
	CurProgress  = (TotalFileSize.QuadPart>0)? 1.0*TotalBytesTransferred.QuadPart/TotalFileSize.QuadPart : -1;
	CurTotalSize = TotalFileSize.QuadPart;

	//�]�����x���v�Z
	SmplSize += (TotalBytesTransferred.QuadPart - LastTransferred.QuadPart);
	LastTransferred = TotalBytesTransferred;
	int cur_cnt  = GetTickCount();
	int smpl_cnt = (cur_cnt - LastCount);
	if (smpl_cnt>1000) {	//***
		LastCount = cur_cnt;
		Speed = SmplSize / smpl_cnt;
		SmplSize = 0;
	}

	//�c�莞�Ԃ��v�Z
	RemCount = (Speed>0)? (TotalFileSize.QuadPart-TotalBytesTransferred.QuadPart)/Speed : 0;

	//�҂����� (���T�C�Y�t�@�C���̏����������̂��߁A�ŏ��ƍŌ�ɂ͑҂������Ȃ�)
	if (CurProgress>0.0 && CurProgress<1.0) {
		if (ReqTaskSlow) {
			ReqTaskSlow = false;
			Sleep(SLOW_INTERVAL);
		}
		else {
			TaskIsFast = (NopDtctTime>0) && ((int)(GetTickCount() - LastOpCount) > NopDtctTime*1000);
			if (TaskIsFast)
				Sleep((Speed>67108)? 0 : MIN_INTERVAL);	//*** 64MB/�b�ȏ�Ȃ� 0
			else
				Sleep(std::max(IsRemote? RemoteWaitTime : NormalWaitTime, MIN_INTERVAL));
		}
	}

	return PROGRESS_CONTINUE;
}

//---------------------------------------------------------------------------
//���O�Ƀ��b�Z�[�W��ǉ�
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
//���O�Ƀf�o�b�O����ǉ�
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
//�t�@�C��������\�ɂȂ�܂ő҂�(���f��)
//  �v���r���[�Ǎ����̏ꍇ�̑Ώ�
//---------------------------------------------------------------------------
bool __fastcall TTaskThread::EX_wait_file_ready(UnicodeString fnam)
{
	int wait = 20;	//*** �ő�10�b
	while (!TaskCancel
		&& ImgViewThread && !ImgViewThread->IsReady() && SameText(fnam, ImgViewThread->FileName)
		&& wait>0)
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
//�f�B���N�g�����̃}�X�N�ɊY������S�t�@�C�����擾
//---------------------------------------------------------------------------
int __fastcall TTaskThread::GetFiles(
	UnicodeString pnam,		//�f�B���N�g����
	UnicodeString mask,		//�}�X�N
	TStrings *lst,			//���ʂ��i�[���X�g
	bool empSW,				//��̃T�u�f�B���N�g�����擾 (default = false)
	bool symSW)				//�V���{���b�N�����N����	 (default = false)
{
	int fcnt = 0;
	if (!dir_exists(pnam) || (!symSW && is_SymLink(pnam))) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	//�T�u�f�B���N�g��������
	UnicodeString sea_str;
	TSearchRec sr;
	sea_str = cv_ex_filename(pnam + "*");
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			if ((sr.Attr & faDirectory)==0) continue;

			UnicodeString dnam = sr.Name;
			if (ContainsStr("..", dnam)) continue;
			if (!symSW && is_SymLink(pnam + dnam)) continue;
			int scnt = GetFiles(pnam + dnam, mask, lst, empSW, symSW);
			fcnt += scnt;
			if (empSW && scnt==0) {
				//��̃T�u�f�B���N�g��
				lst->Add(IncludeTrailingPathDelimiter(pnam + dnam));
				PreCount = lst->Count;
			}
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}

	//�t�@�C��������
	sea_str = cv_ex_filename(pnam + mask);
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			if (sr.Attr & faDirectory) continue;

			//�^�C���X�^���v�ɂ��t�B���^�����O
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
		} while(FindNext(sr)==0 && !TaskCancel);
		FindClose(sr);
	}

	return fcnt;
}
//---------------------------------------------------------------------------
//�f�B���N�g�����̃}�X�N�ɊY������S�t�@�C���̃��X�g��ǉ��擾
//  �߂�l: �t�@�C����(��f�B���N�g���͏���)
//---------------------------------------------------------------------------
int __fastcall TTaskThread::GetFilesEx(
	UnicodeString pnam,			//�f�B���N�g����
	UnicodeString inc_mask,		//�Ώۃ}�X�N(;�ŋ�؂蕡���w���)
	UnicodeString exc_mask,		//���O�}�X�N(;�ŋ�؂蕡���w���)
	TStrings *lst,				//���ʂ��i�[���� TStrings
	bool subSW,					//�T�u�f�B���N�g��������				(default = fals)
	UnicodeString skip_dir,		//���O�f�B���N�g��(;�ŋ�؂蕡���w���)	(default = EmptyStr)
	TStringList *d_lst,			//�T�u�f�B���N�g���̃��X�g				(default = NUL)
	bool symSW,					//�V���{���b�N�����N����		 		(default = false)
	bool dateSW)				//���t�����𗘗p						(default = false)
{
	int fcnt = 0;
	if (!dir_exists(pnam) || (!symSW && is_SymLink(pnam))) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);

	//���O�f�B���N�g��
	TStringDynArray skip_dir_lst = split_strings_semicolon(skip_dir);
	//�}�X�N
	if (inc_mask.IsEmpty()) inc_mask = "*";

	TStringDynArray inc_msk_lst = split_strings_semicolon(inc_mask);	//�Ώ�
	TStringDynArray exc_msk_lst = split_strings_semicolon(exc_mask);	//���O

	TSearchRec sr;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		do {
			WaitIfPause();
			//�T�u�f�B���N�g��������
			if (subSW && (sr.Attr & faDirectory)) {
				UnicodeString dnam = sr.Name;
				if (ContainsStr("..", dnam)) continue;
				if (!symSW && is_SymLink(pnam + dnam)) continue;

				if (d_lst) d_lst->Add(pnam + dnam);

				//���O�f�B���N�g���̃`�F�b�N
				bool skip = false;
				for (int i=0; i<skip_dir_lst.Length; i++) {
					if (skip_dir_lst[i].IsEmpty()) continue;
					if (str_match(skip_dir_lst[i], dnam)) {
						skip = true; break;
					}
				}

				if (!skip) fcnt += GetFilesEx(pnam + dnam, inc_mask, exc_mask, lst, subSW, skip_dir, d_lst, symSW, dateSW);
			}
			//�t�@�C��������
			else {
				//�Ώۃ`�F�b�N
				bool ok = false;
				for (int i=0; i<inc_msk_lst.Length; i++) {
					if (inc_msk_lst[i].IsEmpty()) continue;
					if (str_match(inc_msk_lst[i], sr.Name)) {
						ok = true; break;
					}
				}
				if (!ok) continue;
				//���O�`�F�b�N
				for (int i=0; i<exc_msk_lst.Length; i++) {
					if (exc_msk_lst[i].IsEmpty()) continue;
					if (str_match(exc_msk_lst[i], sr.Name)) {
						ok = false; break;
					}
				}
				if (!ok) continue;
	
				//�^�C���X�^���v�ɂ��t�B���^�����O
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
//�f�B���N�g�����̑S�f�B���N�g�����擾
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
//�t�@�C���̃R�s�[/�ړ�
//---------------------------------------------------------------------------
void __fastcall TTaskThread::CPY_core(
	UnicodeString fnam, 		//�R�s�[��
	UnicodeString dst_path,		//�R�s�[��
	bool mov_sw,				//true = �ړ�/ false = �R�s�[
	bool remove_ro,				//true = �Ǎ���p����������
	TStringList *skip_list)		//�X�L�b�v�����f�B���N�g��(�ړ�������ɍ폜���Ȃ�)
								//	(default = NULL)
{
	WaitIfPause();

	UnicodeString msg;

	//�R�s�[�悪�Ȃ���΍쐬
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

	CurFileName = fnam;
	CopyName	= ExtractFileName(fnam);
	CurProgress = -1;

	UnicodeString dst_fnam = dst_path + CopyName;

	//�����f�B���N�g��������
	if (dir_exists(dst_fnam)) {
		ErrCount++;
		set_LogErrMsg(msg, LoadUsrMsg(USTR_NgSameName));
		AddLog(msg);
		CurFileName  = EmptyStr;
		CurTotalSize = 0;
		return;
	}

	//��������
	if (!Config->CopyAll && file_exists(dst_fnam)) {
		CurSrcName  = fnam;
		CurDstName	= dst_fnam;
		IsSamePath	= SameText(ExtractFilePath(fnam), dst_path);
		TaskAskSame = true;
		while (TaskAskSame && !TaskCancel) Sleep(250);
	}
	//���f
	if (TaskCancel) {
		msg[1] = 'C';
		AddLog(msg);
		return;
	}

	bool w_flag = true;
	if (file_exists(dst_fnam) || Config->CopyMode==CPYMD_REN_CLONE) {
		bool skiped = false;

		switch (Config->CopyMode) {
		case CPYMD_OW:	//�㏑��
			msg[1] = 'O';
			break;

		case CPYMD_NEW:	//�ŐV�Ȃ�
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

		case CPYMD_SKIP:	//�X�L�b�v
			w_flag = false;
			SkipCount++;
			if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
			skiped = true;
			break;

		case CPYMD_AUT_REN:		//�����I�ɖ��O��ύX
		case CPYMD_REN_CLONE:	//�P�ƃN���[�����ŉ���
			dst_fnam = format_CloneName(Config->CopyFmt, fnam, dst_path);
			set_RenameLog(msg, dst_fnam);
			break;

		case CPYMD_MAN_REN:	//���O��ύX
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

		case CPYMD_NEW_BACKUP:	//�ŐV�Ȃ�o�b�N�A�b�v
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

	if (Config->DstPosMode==1 && FirstDstName.IsEmpty()) FirstDstName = dst_fnam;
	if (Config->DstPosMode==2) LastDstName = dst_fnam;

	//�㏑��(����)���s
	if (w_flag) {
		bool failed = false;
		try {
			SetLastError(NO_ERROR);
			if (!EX_set_writable(dst_fnam))		throw Exception(EmptyStr);

			LastTransferred.QuadPart = 0;
			SmplSize  = 0;
			LastCount = 0;
			Speed	  = 0;

			//�ړ�
			if (mov_sw) {
				if (!EX_wait_file_ready(fnam))	throw Exception(EmptyStr);
				if (TaskCancel)					Abort();
				if (SameText(fnam, dst_fnam))	throw Exception(EmptyStr);

				AddDebugLog("Call MoveFileWithProgress");
				bool res = ::MoveFileWithProgress(cv_ex_filename(fnam).c_str(), cv_ex_filename(dst_fnam).c_str(),
								ProgressRoutine, (LPVOID)this,
								MOVEFILE_REPLACE_EXISTING|MOVEFILE_COPY_ALLOWED|MOVEFILE_WRITE_THROUGH);
				AddDebugLog("Return");
				if (!res) {
					if (TaskCancel) Abort(); else throw Exception(EmptyStr);
				}

				usr_TAG->Rename(fnam, dst_fnam);	//�^�O�̈ړ�
				rename_FolderIcon(fnam, dst_fnam);	//�t�H���_�A�C�R���̈ړ�
			}
			//�R�s�[
			else {
				BOOL cancel = FALSE;	//�_�~�[ (���f�� TaskCancel ��)

				//��RAM�f�B�X�N�Ƃ̊Ԃŏ����ȃt�@�C�����R�s�[����ƃG���[�ɂȂ錻�ۂւ̑΍�
				//  10MB�ȉ��̏ꍇ�� COPY_FILE_NO_BUFFERING ���g��Ȃ�
				DWORD cpyflag = (CopyNoBuffering && get_file_size(fnam)>10485760)? COPY_FILE_NO_BUFFERING : 0;

				AddDebugLog("Call CopyFileEx");
				bool res = ::CopyFileEx(cv_ex_filename(fnam).c_str(), cv_ex_filename(dst_fnam).c_str(),
								ProgressRoutine, (LPVOID)this, (LPBOOL)&cancel, cpyflag);
				AddDebugLog("Return");
				if (!res) {
					if (TaskCancel) Abort(); else throw Exception(EmptyStr);
				}

				//�^�O�̃R�s�[
				if (CopyTags) usr_TAG->Copy(fnam, dst_fnam);

				//CD-ROM����̏ꍇ�A�Ǎ���p����������
				if (remove_ro) {
					int attr = file_GetAttr(dst_fnam);	if (attr==faInvalid) throw Exception(EmptyStr);
					AddDebugLog("Set Attribute");
					if (!file_SetAttr(dst_fnam, attr & ~faReadOnly)) throw Exception(EmptyStr);
				}
			}
			OkCount++;
		}
		//���f
		catch (EAbort &e) {
			msg[1] = 'C';  msg += get_LogErrMsg();
			failed = true;
		}
		//�G���[
		catch (...) {
			ErrCount++;
			set_LogErrMsg(msg, EmptyStr, fnam);
			failed = true;
		}

		//�ړ��Ɏ��s�����t�@�C���̃f�B���N�g���͏�����̍폜�Ώۂ��珜�O
		if (mov_sw && failed && skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	if (!msg.IsEmpty()) AddLog(msg);

	CurProgress  = -1;
	CurFileName  = EmptyStr;
	CurTotalSize = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CPY(
	UnicodeString prm,	//�R�s�[�� \t �R�s�[��
	bool mov_sw)		//true = �ړ�/ false = �R�s�[ (default)
{
	UnicodeString src_prm  = split_pre_tab(prm);
	UnicodeString dst_path = prm;
	UnicodeString msg;

	if (src_prm.IsEmpty() || dst_path.IsEmpty()) return;

	UINT src_drv_typ = get_drive_type(src_prm);
	UINT dst_drv_typ = get_drive_type(dst_path);
	bool remove_ro	 = RemoveCdReadOnly && (src_drv_typ==DRIVE_CDROM);
	IsRemote		 = (src_drv_typ==DRIVE_REMOTE || dst_drv_typ==DRIVE_REMOTE);

	//�t�@�C��
	if (!ExtractFileName(src_prm).IsEmpty()) {
		//�^�C���X�^���v�ɂ��t�B���^�����O
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
	//�f�B���N�g��
	else {
		bool is_same_drv = SameText(get_root_name(src_prm), get_root_name(dst_path));
		UnicodeString src_nam = ExcludeTrailingPathDelimiter(src_prm);
		UnicodeString dst_nam = dst_path + ExtractFileName(src_nam);

		//�����t�@�C��������
		if (file_exists_x(dst_nam)) {
			ErrCount++;
			msg = make_LogHdr(_T("MOVE"), src_nam);
			set_LogErrMsg(msg, LoadUsrMsg(USTR_NgSameName));
			AddLog(msg);
			return;
		}

		if (Config->DstPosMode==1 && FirstDstName.IsEmpty()) FirstDstName = dst_nam;
		if (Config->DstPosMode==2) LastDstName = dst_nam;

		//���h���C�u���ł̈ړ�
		if (mov_sw && is_same_drv && !dir_exists(dst_nam)) {
			msg = make_LogHdr(_T("MOVE"), src_nam);
			try {
				//�ړ��悪�Ȃ���΍쐬
				if (!dir_exists(dst_path)) {
					msg = make_LogHdr(_T("CREATE"), dst_path, true);
					SetLastError(NO_ERROR);
					if (!create_ForceDirs(dst_path)) Abort();
				}
				//�ړ�
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
		//�R�s�[���擾
		else {
			UnicodeString org_path = get_parent_path(src_prm);
			std::unique_ptr<TStringList> fbuf(new TStringList());
			PreCount = 0;
			GetFiles(src_prm, "*.*", fbuf.get(), true, true);
			PreCount = 0;
			std::unique_ptr<TStringList> skip_lst(new TStringList());

			//�R�s�[
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
						//�ړ��𒆒f���f�B���N�g�����X�L�b�v���X�g�ɒǉ�
						UnicodeString dnam = ExtractFilePath(fnam);
						if (skip_lst->IndexOf(dnam)==-1) skip_lst->Add(dnam);
					}
				}
			}
			//��f�B���N�g��
			else if (!dir_exists(dst_nam)) {
				msg = make_LogHdr(_T("CREATE"), dst_nam, true);
				SetLastError(NO_ERROR);
				if (create_ForceDirs(dst_nam)) {
					dir_CopyAttr(src_prm, dst_nam, remove_ro);
					if (CopyTags) usr_TAG->Copy(src_prm, dst_nam);
					if (mov_sw) move_FolderIcon(src_prm, dst_nam); else copy_FolderIcon(src_prm, dst_nam);
				}
				else set_LogErrMsg(msg);
				AddLog(msg);
			}

			//�ړ����̃f�B���N�g���폜
			if (mov_sw) {
				DEL_dirs(src_prm, skip_lst.get());
				usr_TAG->Rename(src_prm, dst_nam);	//�^�O�̈ړ�
			}
		}
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���폜
//---------------------------------------------------------------------------
void __fastcall TTaskThread::DEL_dirs(
	UnicodeString pnam,			//�폜����f�B���N�g��
	TStringList *skip_list)		//�X�L�b�v����f�B���N�g���̃��X�g (default = NULL)
{
	if (pnam.IsEmpty()) return;
	pnam = to_path_name(pnam);
	bool is_sym = is_SymLink(pnam);

	//�T�u�f�B���N�g�����������č폜
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

	//�f�B���N�g�����폜
	UnicodeString msg = make_LogHdr(is_sym? _T("REMOVE") : _T("DELETE"), pnam, true);

	try {
		bool skiped = false;
		if (skip_list) {
			for (int i=0; i<skip_list->Count && !skiped; i++)
				skiped = StartsText(pnam, skip_list->Strings[i]);
		}

		if (skiped) {
			if (LogHideSkip) msg = EmptyStr; else msg[1] = 'S';
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
//�t�@�C���폜
//---------------------------------------------------------------------------
void __fastcall TTaskThread::DEL_core(
	UnicodeString fnam,
	bool use_trash,			//���ݔ����g�p
	TStringList *skip_list)	//�X�L�b�v����f�B���N�g��(�ړ�������ɍ폜���Ȃ�)
							//	(default = NULL)
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
		OkCount++;
	}
	//���f
	catch (EAbort &e) {
		msg[1] = 'C';
	}
	//�G���[
	catch (...) {
		ErrCount++;
		set_LogErrMsg(msg, EmptyStr, fnam);
		//�폜�Ɏ��s�����t�@�C���̃f�B���N�g���͏�����̍폜�Ώۂ��珜�O
		if (skip_list) {
			UnicodeString dnam = ExtractFilePath(fnam);
			if (skip_list->IndexOf(dnam)==-1) skip_list->Add(dnam);
		}
	}

	AddLog(msg);
	CurFileName = EmptyStr;
}
//---------------------------------------------------------------------------
//���ݔ��ɂ��f�B���N�g���폜
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

	//�t�@�C��
	if (!ExtractFileName(prm).IsEmpty()) {
		DEL_core(prm, DelUseTrash);
	}
	//�f�B���N�g��(������ \)
	else if (EX_dir_exists(prm)) {
		//���ݔ�
		if (DelUseTrash) {
			DEL_dir_trash(prm);
		}
		else {
			//�V���{���b�N�����N�A�W�����N�V����
			if (is_SymLink(prm)) {
				DEL_dirs(prm);
			}
			//�ʏ�
			else {
				std::unique_ptr<TStringList> skip_lst(new TStringList());
				//�f�B���N�g�����̍폜
				std::unique_ptr<TStringList> fbuf(new TStringList());
				PreCount = 0;
				GetFiles(prm, "*.*", fbuf.get());
				PreCount = 0;
				for (int i=0; i<fbuf->Count && !TaskCancel; i++) {
					WaitIfPause();
					SubCount = fbuf->Count - i;
					DEL_core(fbuf->Strings[i], DelUseTrash, skip_lst.get());
					if (ReqTaskSlow) {
						ReqTaskSlow = false;
						Sleep(SLOW_INTERVAL);
					}
					else if (i%32==0) {
						Sleep(MIN_INTERVAL);
					}
				}

				Sleep(MIN_INTERVAL);

				if (!TaskCancel) DEL_dirs(prm, skip_lst.get());
			}
		}
	}
}

//---------------------------------------------------------------------------
//�t�@�C�����S�폜
//---------------------------------------------------------------------------
void __fastcall TTaskThread::CMPDEL_core(UnicodeString fnam)
{
	//�n�[�h�����O�̏ꍇ�A�ʏ�폜
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
				//�����_���p�^�[���쐬
				for (int j=0; j<CMPDEL_BUFF_SIZE; j++) wbuf[j] = Random(256);
				//�擪�փV�[�N
				::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
				//��������
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

					if (TaskCancel) throw Exception(EmptyStr);	//���f
					if (!WriteFile(hFile, wbuf.get(), CMPDEL_BUFF_SIZE, &wbyt, NULL)) throw Exception(EmptyStr);
					written_sz += wbyt;
					CurProgress = 1.0*written_sz/total_sz;
					if (Random(8)==0)
						for (int k=0; k<CMPDEL_BUFF_SIZE; k++) wbuf[k] = Random(256);	//�Ƃ��ǂ��p�^�[���ύX
				}
				//�[���o�C�g
				if (rcnt>0) {
					if (!WriteFile(hFile, wbuf.get(), rcnt, &wbyt, NULL)) throw Exception(EmptyStr);
					written_sz += wbyt;
					CurProgress = 1.0*written_sz/total_sz;
				}
			}
			//�T�C�Y��0�ɐ؂�l�߂�
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

		//�폜
		if (!EX_delete_File(fnam)) throw Exception(EmptyStr);
		del_CachedIcon(fnam);

		OkCount++;
	}
	//���f
	catch (EAbort &e) {
		msg[1] = 'C';  msg += "\r\n    �v���͒��f����܂����B";
	}
	//�G���[
	catch (...) {
		ErrCount++;
		set_LogErrMsg(msg, EmptyStr, fnam);
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
		if (!is_SymLink(prm)) {
			//�f�B���N�g�����̍폜
			std::unique_ptr<TStringList> fbuf(new TStringList());
			PreCount = 0;
			GetFiles(prm, "*.*", fbuf.get());
			PreCount = 0;
			for (int i=0; i<fbuf->Count && !TaskCancel; i++) {
				SubCount = fbuf->Count - i;
				CMPDEL_core(fbuf->Strings[i]);
				Sleep(MIN_INTERVAL);
			}
		}
		if (!TaskCancel) DEL_dirs(prm);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���\���̃R�s�[
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CPYDIR(UnicodeString prm)
{
	UnicodeString src_prm  = split_pre_tab(prm);
	UnicodeString dst_path = prm;
	if (src_prm.IsEmpty() || dst_path.IsEmpty()) return;

	bool remove_ro = RemoveCdReadOnly && (get_drive_type(src_prm)==DRIVE_CDROM);

	//�擾
	std::unique_ptr<TStringList> dbuf(new TStringList());
	dbuf->Add(src_prm);
	PreCount = 1;
	GetDirs(src_prm, dbuf.get());
	PreCount = 0;

	//�R�s�[(�쐬)
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
				//����
				dir_CopyAttr(dnam, dst_nam, remove_ro);
				//�^�C���X�^���v
				if (Config->KeepTime) {
					TDateTime ft = get_file_age(dnam);
					if ((int)ft>0) set_file_age(dst_nam, ft);
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
//�摜�t�@�C���̕ϊ�
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_CVIMG(UnicodeString prm)
{
	bool is_clip = Config->CvImg_from_clip;
	UnicodeString fnam	   = split_pre_tab(prm);
	UnicodeString dst_path = prm;

	CurFileName = fnam;
	bool is_wmf = test_MetaExt(ExtractFileExt(fnam));

	UnicodeString msg  = make_LogHdr(_T("CVIMG"), fnam);

	try {
		WaitIfPause();
		if (!is_clip && !EX_file_exists(fnam)) throw Exception(EmptyStr);

		//�����O�Ƀ^�C���X�^���v������Ă���
		TDateTime ft;
		if (!is_clip && Config->KeepTime) ft = get_file_age(fnam);

		//�ǂݍ���
		std::unique_ptr<Graphics::TBitmap> i_img(new Graphics::TBitmap());
		std::unique_ptr<TMetafile> mf(new TMetafile());
		unsigned int i_wd, i_hi;
		//�N���b�v�{�[�h
		if (is_clip) {
			if (!Clipboard()->HasFormat(CF_BITMAP)) UserAbort(USTR_NoObject);
			i_img->Assign(Clipboard());
			i_wd = i_img->Width;
			i_hi = i_img->Height;
		}
		//���^�t�@�C��
		else if (is_wmf) {
			mf->LoadFromFile(fnam);
			i_wd = mf->Width;
			i_hi = mf->Height;
		}
		//�摜�t�@�C��
		else {
			TColor bg = test_FileExt(ExtractFileExt(fnam), _T(".png.gif"))? Config->CvImg_mgn_color : clNone;
			if (load_ImageFile(fnam, i_img.get(), (Config->CvImg_not_use_prvw? WICIMG_FRAME : WICIMG_PREVIEW), bg)==0)
				UserAbort(USTR_FaildLoad);
			i_wd = i_img->Width;
			i_hi = i_img->Height;
		}
		if (i_wd==0 || i_hi==0) TextAbort(_T("���摜�̃T�C�Y���s���ł��B"));

		//�k���E�g��
		if (Config->CvImg_scale_mode>0) {
			float r, r0, r1;
			std::unique_ptr<Graphics::TBitmap> r_img(new Graphics::TBitmap());

			//�p�����[�^�`�F�b�N
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

			//�ύX�T�C�Y�����߂�
			switch (Config->CvImg_scale_mode) {
			case 2:	//�c���̒�����
				if (i_wd>i_hi) {	//���̕�������
					r = 1.0 * i_hi / i_wd;
					i_wd = Config->CvImg_scale_prm1;
					i_hi = i_wd * r;
				}
				else {			//�c�̕�������
					r = 1.0 * i_wd / i_hi;
					i_hi = Config->CvImg_scale_prm1;
					i_wd = i_hi * r;
				}
				break;
			case 3:	//���T�C�Y���w��
				r = 1.0 * i_hi / i_wd;
				i_wd = Config->CvImg_scale_prm1;
				i_hi = i_wd * r;
				break;
			case 4:	//�c�T�C�Y���w��
				r = 1.0 * i_wd / i_hi;
				i_hi = Config->CvImg_scale_prm1;
				i_wd = i_hi * r;
				break;
			case 5: case 7: //�w��T�C�Y�Ɏ��߂�/ �]���t��
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
			case 6:	//�w��T�C�Y�ɃX�g���b�`
				i_wd = Config->CvImg_scale_prm1;
				i_hi = Config->CvImg_scale_prm2;
				break;
			case 8:	//�w��T�C�Y�ɍ��킹�Đ؂�o��
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

			//�k���E�g��
			//���^�t�@�C��
			if (is_wmf) {
				if (Config->CvImg_scale_mode==1) {
					//�{���w��
					float r = Config->CvImg_scale_prm1/100.0;
					r_img->SetSize(i_wd * r, i_hi * r);
				}
				else {
					//�c�E���T�C�Y�w��
					r_img->SetSize(i_wd, i_hi);
				}
				TRect rc = Rect(0, 0, r_img->Width, r_img->Height);
				r_img->Canvas->Brush->Color = Config->CvImg_mgn_color;
				r_img->Canvas->FillRect(rc);
				r_img->Canvas->StretchDraw(rc, mf.get());
			}
			//�摜�t�@�C��
			else {
				//��Ԃ���
				if (Config->CvImg_scale_opt>=0 && Config->CvImg_scale_opt<=3) {
					//�{���w��
					if (Config->CvImg_scale_mode==1) {
						if (!WIC_resize_image(i_img.get(), r_img.get(),
							Config->CvImg_scale_prm1/100.0, 0, 0, Config->CvImg_scale_opt))
								UserAbort(USTR_FaildProc);
					}
					//�c�E���T�C�Y�w��
					else {
						if (!WIC_resize_image(i_img.get(), r_img.get(), 0.0, i_wd, i_hi, Config->CvImg_scale_opt))
							UserAbort(USTR_FaildProc);
					}
				}
				//��Ԃ��Ȃ�
				else {
					int r_wd = i_wd;
					int r_hi = i_hi;
					if (Config->CvImg_scale_mode==1) {
						float r = Config->CvImg_scale_prm1/100.0;
						r_wd = i_wd * r;
						r_hi = i_hi * r;
					}
					r_img->SetSize(r_wd, r_hi);
					r_img->PixelFormat = pf24bit;
					r_img->Canvas->StretchDraw(Rect(0, 0, r_wd, r_hi), i_img.get());
				}
			}

			//���ʂ����蓖�Ă�
			int xp, yp;
			switch (Config->CvImg_scale_mode) {
			case 7:	//�]����t����
				xp = (Config->CvImg_scale_prm1 - i_wd)/2;
				yp = (Config->CvImg_scale_prm2 - i_hi)/2;
				i_img->SetSize(Config->CvImg_scale_prm1, Config->CvImg_scale_prm2);
				i_img->Canvas->Brush->Color = Config->CvImg_mgn_color;
				i_img->Canvas->FillRect(Rect(0, 0, Config->CvImg_scale_prm1, Config->CvImg_scale_prm2));
				i_img->Canvas->Draw(xp, yp, r_img.get());
				break;
			case 8:	//�؂�o��
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

		//�O���[�X�P�[����
		if (Config->CvImg_grayscale) WIC_grayscale_image(i_img.get());

		//�t�@�C�����̕ύX
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
		msg.cat_sprintf(_T(" ---> %s"), ExtractFileName(cv_nam).c_str());

		//�`���ϊ����ĕۑ�
		if (!WIC_save_image(cv_nam, i_img.get(),
			Config->CvImg_quality, Config->CvImg_ycrcb, Config->CvImg_grayscale, Config->CvImg_cmp_mode))
				UserAbort(USTR_FaildSave);

		//�^�C���X�^���v��ݒ�
		if (!is_clip && Config->KeepTime && (int)ft>0) set_file_age(cv_nam, ft);

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

	AddLog(msg);
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//Jpeg �t�@�C������ Exif �����폜
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
//HTML�w���v(.chm)����\�[�X�𒊏o
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
	STARTUPINFO si;
	::ZeroMemory(&si, sizeof(si) );
	si.cb = sizeof(si);
	si.dwFlags	   = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	PROCESS_INFORMATION pi;
	if (::CreateProcess(NULL, cmdln.c_str(), NULL, NULL, TRUE, 0, NULL, c_dir.c_str(), &si, &pi)) {
		//�I���҂�
		while (::WaitForSingleObject(pi.hProcess, 50)==WAIT_TIMEOUT) Sleep(MIN_INTERVAL);
		::CloseHandle(pi.hThread);
		::CloseHandle(pi.hProcess);
	}
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//�o�b�N�A�b�v
//---------------------------------------------------------------------------
void __fastcall TTaskThread::Task_BACKUP(UnicodeString prm)
{
	UnicodeString src_path = split_pre_tab(prm);
	UnicodeString dst_path = prm;

	//�R�s�[���擾
	std::unique_ptr<TStringList> src_lst(new TStringList());
	PreCount = 0;
	GetFilesEx(src_path, Config->Bakup_inc_mask, Config->Bakup_exc_mask, src_lst.get(),
				Config->Bakup_sub_sw, Config->Bakup_skip_dir, NULL, true, true);
	PreCount = 0;

	//�R�s�[
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
	AddLog("         �R�s�[�I��" + get_res_cnt_str(OkCount, ErrCount, SkipCount));
	SubCount = OkCount = SkipCount = ErrCount = 0;

	//�~���[�����O
	if (Config->Bakup_mirror) {
		UnicodeString msg;
		msg.sprintf(_T("         �~���[�����O�J�n  %s"), dst_path.c_str());
		if (!dir_exists(src_path)) {
			set_LogErrMsg(msg, "�o�b�N�A�b�v����������܂���B");
			AddLog(msg);
			return;
		}
		AddLog(msg);

		//�o�b�N�A�b�v���ɂȂ��t�@�C�����폜
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

		//�o�b�N�A�b�v���ɂȂ��f�B���N�g�����폜
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

		AddLog("         �~���[�����O�I��" + get_res_cnt_str(OkCount, ErrCount, SkipCount));
		SubCount = OkCount = SkipCount = ErrCount = 0;
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���̃^�C���X�^���v���ŐV�t�@�C���̂��̂ɐݒ�
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

	//�t�@�C��
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
//�f�B���N�g����NTFS���k/����
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

	//�t�@�C��
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
			AddLog(msg.sprintf(_T("         �T�C�Y�� %s �������܂����B"), get_size_str_G(r_size, 0, SizeDecDigits, 1).c_str()));
		}
	}

	CurProgress = -1;
	CurFileName = EmptyStr;
}

//---------------------------------------------------------------------------
//�^�X�N�̊J�n
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
//�^�X�N�̏I��(���f)����
//---------------------------------------------------------------------------
void __fastcall TTaskThread::FinishTask()
{
	//���O�\��
	float tcnt = (GetTickCount() - StartCount)/1000.0;
	TFormatSettings fs = TFormatSettings::Create();
	UnicodeString msg = FormatDateTime("hh:nn:ss ", Now(), fs);
	msg.cat_sprintf(_T("%s�I�� %5.1f�b"), TaskCmdList->Values[CmdName].c_str(), tcnt);
	msg += get_res_cnt_str(OkCount, ErrCount, SkipCount);
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

						switch (idx_of_word_i(_T("CPY|MOV|DEL|CPYDIR|CMPDEL|CVIMG|DLEXIF|EXTCHM|BACKUP|DTIME|DCOMP"), CmdName)) {
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
			AddLog("E ��O�G���[\r\n    " + SysErrorMessage(EVENT_E_INTERNALEXCEPTION));
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
