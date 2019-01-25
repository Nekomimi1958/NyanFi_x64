//----------------------------------------------------------------------//
// NyanFi																//
//  �摜�r���[�����X���b�h												//
//----------------------------------------------------------------------//
#ifndef ImgViewThreadH
#define ImgViewThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class TImgViewThread : public TThread
{
private:
	UnicodeString Img_f_name;
	__int64		  Img_f_size;
	TDateTime	  Img_f_time;
	UnicodeString Img_rot_str;	//���^�t�@�C���̉�]�E���]��������
	bool ChgPrvCur;				//�v���r���[��̃}�E�X�|�C���^��ύX

	UnicodeString MsgStr;
	bool MsgIsErr;

	TStringList *RequestList;	//�����L���[

	TMultiReadExclusiveWriteSynchronizer *TaskRWLock;

	//�X���b�h�Z�[�t���l�������v���p�e�B
	bool FTaskBusy;
	bool __fastcall GetTaskBusy()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskBusy;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskBusy(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskBusy = Value;
		TaskRWLock->EndWrite();
	}
	__property bool TaskBusy = {read = GetTaskBusy,  write = SetTaskBusy};

	void __fastcall ClearImage();
	void __fastcall DrawImage();
	void __fastcall DrawViewBuff();
	void __fastcall DrawMessage();
	void __fastcall Execute();

public:
	UnicodeString FileName;		//�v���摜�t�@�C����
	UnicodeString FileName2;

	bool ReqRedraw;			//�ĕ`���v��
	bool ReqReload;			//�����[�h��v��
	bool ReqClear;			//��ʃN���A��v��
	bool ReqVideo;			//����(�T���l�C���\����v��)
	bool ReqWait;			//MinShowTime �ɂ��҂��v��
	bool UseViewBuff;		//���C�����Őݒ肳�ꂽ�o�b�t�@���e���g�p
	int  Rotation;			//��]/���]��v�� (1,3,4,5)
	int  ExifOri;			//Exif �̕������
	int  ZoomRatio;			//�Y�[���{��(%)
	double ZoomRatioF;

	bool Fitted;			//�t�B�b�g�\��
	bool GrayScaled;		//�O���[�X�P�[���\��
	bool ShowGrid;			//�O���b�h��\��

	bool ImgIsWmf;

	Graphics::TBitmap *ImgBuff;		//�摜�o�b�t�@(���T�C�Y)
	Graphics::TBitmap *ViewBuff;	//�\���o�b�t�@
	Graphics::TBitmap *ChgBuff;
	TMetafile *MetaBuff;			//���^�t�@�C���p�o�b�t�@

	__fastcall TImgViewThread(bool CreateSuspended);
	bool __fastcall IsReady();
	bool __fastcall IsWaiting();
	void __fastcall AddRequest(const _TCHAR *cmd, UnicodeString prm1 = EmptyStr, UnicodeString prm2 = EmptyStr);
	void __fastcall AddRequest(const _TCHAR *cmd, int prm);
};
//---------------------------------------------------------------------------
#endif
