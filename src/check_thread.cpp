//----------------------------------------------------------------------//
// NyanFi																//
//  UNCパスの存在チェック・スレッド										//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "check_thread.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TCheckPathThread::TCheckPathThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLower;
	FreeOnTerminate = true;

	TaskRWLock = new TMultiReadExclusiveWriteSynchronizer();

	PathName = EmptyStr;
	isOk	 = false;
}

//---------------------------------------------------------------------------
void __fastcall TCheckPathThread::Execute()
{
	while (!Terminated) {
		if (!PathName.IsEmpty()) {
			isOk	 = ::PathIsDirectory(PathName.c_str());
			PathName = EmptyStr;
		}
		Sleep(50);
	}

	delete TaskRWLock;
}
//---------------------------------------------------------------------------
