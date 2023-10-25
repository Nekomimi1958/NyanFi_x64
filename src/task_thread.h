//----------------------------------------------------------------------//
// NyanFi																//
//  �^�X�N�����X���b�h													//
//----------------------------------------------------------------------//
#ifndef TaskThreadH
#define TaskThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
#define MIN_INTERVAL	1
#define WAIT_INTERVAL	50
#define SLOW_INTERVAL	500

//---------------------------------------------------------------------------
class TaskConfig
{
public:
	int  CopyMode;
	bool CopyAll;
	UnicodeString CopyFmt;
	int  DstPosMode;
	bool KeepTime;
	bool NoSubDir;

	TDateTime FilterTime;
	int  FilterMode;

	UnicodeString CmdStr;
	UnicodeString InfStr;
	UnicodeString DistPath;
	UnicodeString WorkList;

	UnicodeString CvImg_f_ext;
	int  CvImg_quality;
	int  CvImg_ycrcb;
	int  CvImg_cmp_mode;
	int  CvImg_scale_mode;
	int  CvImg_scale_prm1;
	int  CvImg_scale_prm2;
	int  CvImg_scale_opt;
	bool CvImg_from_clip;
	bool CvImg_not_use_prvw;
	bool CvImg_grayscale;
	TColor CvImg_mgn_color;
	int  CvImg_chg_name_mode;
	UnicodeString CvImg_chg_name_str;
	UnicodeString CvImg_clip_name;

	UnicodeString Bakup_inc_mask;
	UnicodeString Bakup_exc_mask;
	UnicodeString Bakup_skip_dir;
	bool Bakup_sub_sw;
	bool Bakup_mirror;

	bool Distribute_sw;

	bool DirComp_sw;
	bool DirComp_ext_sw;

	TStringList *TaskList;
	TStringList *UpdDirList;

	TaskConfig();
	~TaskConfig();

	void Assign(TaskConfig *cp);
};

//---------------------------------------------------------------------------
//�^�X�N�ݒ胊�X�g (TList ����p��)
//---------------------------------------------------------------------------
class TaskConfigList : public TList
{
private:
	TaskConfig * __fastcall Get(int Index)
	{
		return (TaskConfig*)(TList::Get(Index));
	}
	void __fastcall Put(int Index, TaskConfig *Item)
	{
		TList::Put(Index, Item);
	}

public:
	__fastcall TaskConfigList();
	__fastcall ~TaskConfigList();

	void __fastcall ClearAll();

	__property TaskConfig * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
// �^�X�N�����X���b�h
//---------------------------------------------------------------------------
class TTaskThread : public TThread
{
private:
	bool IsRemote;			//�R�s�[/�ړ����E�悪�l�b�g���[�N�h���C�u

	LARGE_INTEGER LastTransferred;
	__int64 SmplSize;
	int  LastCount;

	bool ErrCancel;

	TStringList *TaskObjList;

	static DWORD CALLBACK ProgressRoutine(
		LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred,
		LARGE_INTEGER StreamSize, LARGE_INTEGER StreamBytesTransferred,
		DWORD dwStreamNumber, DWORD dwCallbackReason,
		HANDLE hSourceFile, HANDLE hDestinationFile, LPVOID lpData);

	DWORD __fastcall ProgressCore(LARGE_INTEGER TotalFileSize, LARGE_INTEGER TotalBytesTransferred);

	void __fastcall FinishTask();
	void __fastcall Execute();

	int  __fastcall GetFiles(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool empSW = false, bool symSW = false);

	int  __fastcall GetFilesEx(UnicodeString pnam, UnicodeString inc_mask, UnicodeString exc_mask,
						TStrings *lst, bool subSW = false, UnicodeString skip_dir = EmptyStr, TStringList *d_lst = NULL,
						bool symSW = false, bool dateSW = false);

	void __fastcall GetDirs(UnicodeString pnam, TStrings *lst);

	void __fastcall CPY_core(UnicodeString fnam, UnicodeString dst_path,
								bool mov_sw = false, bool remove_ro = false, TStringList *skip_list = NULL);
	void __fastcall Task_CPY(UnicodeString prm, bool mov_sw = false);

	void __fastcall AddDebugLog(UnicodeString msg, UnicodeString info = EmptyStr);
	bool __fastcall EX_file_exists(UnicodeString fnam);
	bool __fastcall EX_dir_exists(UnicodeString dnam);
	bool __fastcall EX_set_writable(UnicodeString fnam);
	bool __fastcall EX_wait_file_ready(UnicodeString fnam);
	bool __fastcall EX_delete_File(UnicodeString fnam, bool use_trash = false);
	bool __fastcall EX_delete_Dir(UnicodeString dnam);

	void __fastcall DEL_core(UnicodeString fnam, bool use_trash, TStringList *skip_list = NULL);
	void __fastcall DEL_dir_trash(UnicodeString dnam);
	void __fastcall DEL_dirs(UnicodeString pnam, TStringList *skip_list = NULL);
	void __fastcall Task_DEL(UnicodeString prm);

	void __fastcall CMPDEL_core(UnicodeString fnam, TStringList *skip_list = NULL);
	void __fastcall Task_CMPDEL(UnicodeString prm);

	void __fastcall Task_CPYDIR(UnicodeString prm);

	void __fastcall Task_CVIMG(UnicodeString prm);
	void __fastcall Task_DLEXIF(UnicodeString prm);
	void __fastcall Task_EXTCHM(UnicodeString prm);

	void __fastcall Task_BACKUP(UnicodeString prm);

	TDateTime __fastcall SetDirTime(UnicodeString dnam);
	void __fastcall Task_DIRTIME(UnicodeString prm);

	void MakeCompList(UnicodeString dnam);
	void __fastcall Task_DIRCOMP(UnicodeString prm);

	void __fastcall AddLog(UnicodeString msg, bool must_log = false);

	TMultiReadExclusiveWriteSynchronizer *TaskRWLock;

	//�X���b�h�Z�[�t���l�������v���p�e�B
	bool FTaskReady;
	bool __fastcall GetTaskReady()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskReady;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskReady(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskReady = Value;
		TaskRWLock->EndWrite();
	}

	bool FTaskFinished;
	bool __fastcall GetTaskFinished()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskFinished;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskFinished(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskFinished = Value;
		TaskRWLock->EndWrite();
	}

	bool FTaskCancel;
	bool __fastcall GetTaskCancel()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskCancel;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskCancel(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskCancel = Value;
		TaskRWLock->EndWrite();
	}

	bool FTaskPause;
	bool __fastcall GetTaskPause()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskPause;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskPause(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskPause = Value;
		TaskRWLock->EndWrite();
	}

	bool FTaskAskSame;
	bool __fastcall GetTaskAskSame()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskAskSame;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskAskSame(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskAskSame = Value;
		TaskRWLock->EndWrite();
	}

public:
	__property bool TaskReady	 = {read = GetTaskReady,	write = SetTaskReady};
	__property bool TaskFinished = {read = GetTaskFinished,	write = SetTaskFinished};	//�^�X�N���I��
	__property bool TaskCancel	 = {read = GetTaskCancel,	write = SetTaskCancel};		//���f
	__property bool TaskPause	 = {read = GetTaskPause,	write = SetTaskPause};		//��U��~
	__property bool TaskAskSame  = {read = GetTaskAskSame,	write = SetTaskAskSame};	//�����t�@�C��������v��

	int Tag;
	int ID;

	bool TaskIsFast;			//�������s��
	bool ReqTaskSlow;			//�ᑬ���s��v��
	bool DirDeleted;			//�f�B���N�g�����폜���ꂽ
	bool LnkDeleted;			//�n�[�h�����N���폜���ꂽ

	int  TaskTime;
	int  PreCount, SubCount, OkCount, SkipCount, RenCount, ErrCount;
	int  StartCount;

	int  Speed;					//�]�����x(byte/ms)
	int  RemCount;				//�c�莞��(ms)
	__int64 CurTotalSize;

	UnicodeString CurFileName;	//�����Ώ�
	double CurProgress;			//���݃R�s�[/�ړ����t�@�C���̐i����

	UnicodeString DstFileName;	//�R�s�[/�ړ�/�ϊ���

	UnicodeString CurSrcName;	//�R�s�[/�ړ���
	UnicodeString CurDstName;	//�R�s�[/�ړ���

	UnicodeString FirstDstName;	//�ŏ��ɃR�s�[/�ړ������t�@�C����
	UnicodeString LastDstName;	//���̎��_�ōŌ�ɃR�s�[/�ړ������t�@�C����

	TaskConfig *Config;

	bool IsSamePath;
	UnicodeString CopyName;
	UnicodeString CloneName;

	UnicodeString CmdName;

	__fastcall TTaskThread(bool CreateSuspended);
	bool __fastcall TaskStart();

	void WaitIfPause()
	{
		while (TaskPause && !TaskCancel) Sleep(WAIT_INTERVAL);
	}
};
//---------------------------------------------------------------------------
extern int TaskIdCount;

#endif
