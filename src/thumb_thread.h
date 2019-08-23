//----------------------------------------------------------------------//
// NyanFi																//
//  �T���l�C���擾�X���b�h												//
//----------------------------------------------------------------------//
#ifndef ThumbnailThreadH
#define ThumbnailThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class TThumbnailThread : public TThread
{
private:
	TMultiReadExclusiveWriteSynchronizer *TaskRWLock;

	//�X���b�h�Z�[�t���l�������v���p�e�B
	bool FReqClear;
	bool __fastcall GetReqClear()
	{
		TaskRWLock->BeginRead();
		bool v = FReqClear;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetReqClear(bool Value)
	{
		TaskRWLock->BeginWrite();
		FReqClear = Value;
		TaskRWLock->EndWrite();
	}

	bool FReqStart;
	bool __fastcall GetReqStart()
	{
		TaskRWLock->BeginRead();
		bool v = FReqStart;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetReqStart(bool Value)
	{
		TaskRWLock->BeginWrite();
		FReqStart = Value;
		TaskRWLock->EndWrite();
	}

	bool FReqMake;
	bool __fastcall GetReqMake()
	{
		TaskRWLock->BeginRead();
		bool v = FReqMake;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetReqMake(bool Value)
	{
		TaskRWLock->BeginWrite();
		FReqMake = Value;
		TaskRWLock->EndWrite();
	}

	bool FIsEmpty;
	bool __fastcall GetIsEmpty()
	{
		TaskRWLock->BeginRead();
		bool v = FIsEmpty;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetIsEmpty(bool Value)
	{
		TaskRWLock->BeginWrite();
		FIsEmpty = Value;
		TaskRWLock->EndWrite();
	}

	int FCount;
	int __fastcall GetCount()
	{
		TaskRWLock->BeginRead();
		int v = ThumbnailList->Count;
		TaskRWLock->EndRead();
		return v;
	}

	void __fastcall Execute();

public:
	HWND CallbackWnd;

	__property bool ReqClear = {read = GetReqClear, write = SetReqClear};	//���X�g�̃N���A�v��
	__property bool ReqStart = {read = GetReqStart, write = SetReqStart};	//�擾�X�^�[�g�v��
	__property bool ReqMake  = {read = GetReqMake,  write = SetReqMake};	//�ʍ쐬�v��
	__property bool IsEmpty  = {read = GetIsEmpty,  write = SetIsEmpty};	//�T���l�C�����擾
	__property int  Count    = {read = GetCount};							//���X�g���ڐ�

	int MakeIndex;
	int StartIndex;

	TStringList *ThumbnailList;	//�T���l�C�����X�g
	UnicodeString __fastcall GetListItem(int idx);
	void __fastcall SetListItem(int idx, UnicodeString s);
	Graphics::TBitmap* __fastcall GetListBitmap(int idx);
	Graphics::TBitmap* __fastcall GetListBitmap(UnicodeString fnam);

	__fastcall TThumbnailThread(bool CreateSuspended);
	bool __fastcall FitSize(int *wd, int *hi);
	void __fastcall MakeThumbnail(int idx);
};
//---------------------------------------------------------------------------
#endif
