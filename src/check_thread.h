//----------------------------------------------------------------------//
// NyanFi																//
//  UNC�p�X�̑��݃`�F�b�N�E�X���b�h										//
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

	//�X���b�h�Z�[�t���l�������v���p�e�B
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

	__fastcall TCheckPathThread(bool CreateSuspended);
};
//---------------------------------------------------------------------------
#endif
