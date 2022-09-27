//----------------------------------------------------------------------//
// NyanFi																//
//  UNCパスの存在チェック・スレッド										//
//----------------------------------------------------------------------//
#include "check_thread.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCheckPathThread::TCheckPathThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLowest;
	FreeOnTerminate = true;

	TaskRWLock = new TMultiReadExclusiveWriteSynchronizer();

	PathName = EmptyStr;
	isOk	 = false;
	ErrCode  = NO_ERROR;
}

//---------------------------------------------------------------------------
void __fastcall TCheckPathThread::Execute()
{
	while (!Terminated) {
		if (!PathName.IsEmpty()) {
			ErrMsg	= EmptyStr;
			ErrCode = NO_ERROR;

			UnicodeString dstr = IncludeTrailingPathDelimiter(ExtractFileDrive(PathName));
			UINT old_mode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
			DWORD MaxCompLen, Flags;
			isOk = (::GetVolumeInformation(dstr.c_str(), NULL, 0, NULL, &MaxCompLen, &Flags, NULL, 0)!=0);
			::SetErrorMode(old_mode);
			if (isOk) isOk = ::PathIsDirectory(PathName.c_str());

			if (!isOk) {
				ErrCode = GetLastError();
				ErrMsg	= SysErrorMessage(ErrCode);
			}
			PathName = EmptyStr;
		}
		Sleep(50);
	}

	delete TaskRWLock;
}
//---------------------------------------------------------------------------
