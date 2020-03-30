//----------------------------------------------------------------------//
// NyanFi																//
//  UNCパスの存在チェック・スレッド										//
//----------------------------------------------------------------------//
#ifndef TCheckPathThreadH
#define TCheckPathThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class TCheckPathThread : public TThread
{
private:
	void __fastcall Execute();

	TMultiReadExclusiveWriteSynchronizer *TaskRWLock;

	//スレッドセーフを考慮したプロパティ
	UnicodeString FPathName;
	UnicodeString __fastcall GetPathName()
	{
		TaskRWLock->BeginRead();
		UnicodeString v = FPathName;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetPathName(UnicodeString Value)
	{
		TaskRWLock->BeginWrite();
		FPathName = Value;
		TaskRWLock->EndWrite();
	}

public:
	__property UnicodeString PathName = {read = GetPathName,  write = SetPathName};

	bool isOk;
	UnicodeString ErrMsg;
	unsigned int  ErrCode;

	__fastcall TCheckPathThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
