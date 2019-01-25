//----------------------------------------------------------------------//
// �������}�b�v�h�t�@�C���N���X											//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrMMFileH
#define UsrMMFileH

//---------------------------------------------------------------------------
#define MAX_MEMMAP_SIZE	1073741824L		//�ő�}�b�v�T�C�Y		   (1GB)
#define FAILED_BUF_SIZE	8388608L		//�}�b�v���s���̓Ǎ��T�C�Y (8MB)

//---------------------------------------------------------------------------
class MemMapFile
{
private:
	HANDLE hFile;
	HANDLE hMap;
	BYTE *pPointer;		//�}�b�v�p�|�C���^

	TBytes ByteBuff;	//�ʏ�Ǎ��p�o�b�t�@

	void Initialize();

	BYTE Get(unsigned int Index)
	{
		if (isMaped) {
			if (!pPointer || Index>=FileSize) return 0;
			BYTE *p = pPointer;
			p += Index;
			return *p;
		}
		else
			return (Index<BuffSize)? ByteBuff[Index] : 0;
	}

public:
	__property BYTE Bytes[unsigned int Index] = {read=Get};

	__int64 FileSize;		//�t�@�C���T�C�Y
	unsigned int BuffSize;	//�o�b�t�@(�}�b�v)�T�C�Y
	bool MapEnabled;		//�}�b�v�L��
	bool isMaped;			//�}�b�v����Ă���
	UnicodeString ErrMsg;	//�G���[���b�Z�[�W

	MemMapFile();
	~MemMapFile();

	bool OpenRO(UnicodeString fnam, __int64 top_adr, unsigned int max_size);
	void Close();
};
//---------------------------------------------------------------------------
#endif
