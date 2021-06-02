//----------------------------------------------------------------------//
// �������}�b�v�h�t�@�C���N���X											//
//																		//
//----------------------------------------------------------------------//
#include "usr_file_ex.h"
#include "usr_mmfile.h"

//---------------------------------------------------------------------------
MemMapFile::MemMapFile()
{
	Initialize();
}
//---------------------------------------------------------------------------
MemMapFile::~MemMapFile()
{
	Close();
}

//---------------------------------------------------------------------------
//�ǂݎ���p��(MapEnabled=true�Ȃ�)�������}�b�v�h�t�@�C���Ƃ��ĊJ��
//�J���Ȃ��ꍇ�̓������ɒʏ�ǂݍ���
//---------------------------------------------------------------------------
bool MemMapFile::OpenRO(UnicodeString fnam, __int64 top_adr, unsigned int max_size)
{
	Close();

	FileSize = get_file_size(fnam);
	if (FileSize==0) {
		BuffSize = 0;
		ByteBuff.Length = 0;
		return true;
	}
	else if (top_adr>=FileSize) return false;

	SYSTEM_INFO si;
	::GetSystemInfo(&si);
	if (max_size==0) max_size = si.dwAllocationGranularity;
	if (max_size>MAX_MEMMAP_SIZE) max_size = MAX_MEMMAP_SIZE;
	BuffSize = (unsigned int)std::min<__int64>(FileSize - top_adr, max_size);

	try {
		if (!MapEnabled) Abort();

		hFile = ::CreateFile(cv_ex_filename(fnam).c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile==INVALID_HANDLE_VALUE) Abort();

		hMap = ::CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, 0, NULL);
		if (!hMap) {
			::CloseHandle(hFile);
			hFile = INVALID_HANDLE_VALUE;
			Abort();
		}

		//�|�C���^���擾
		DWORD offset = top_adr%si.dwAllocationGranularity;
		top_adr -= offset;
		DWORD top_adr_hi = (top_adr >> 32) & 0xffffffffLL;
		DWORD top_adr_lo = top_adr & 0xffffffffLL;
		DWORD map_size   = BuffSize + offset;
		pPointer = (BYTE*)::MapViewOfFile(hMap, FILE_MAP_READ, top_adr_hi, top_adr_lo, map_size);
		if (!pPointer) {
			::CloseHandle(hMap);
			::CloseHandle(hFile);
			hMap   = 0;
			hFile  = INVALID_HANDLE_VALUE;
			ErrMsg = SysErrorMessage(GetLastError());
			Abort();
		}
		pPointer += offset;
		isMaped = true;
		return true;
	}
	catch (...) {
		isMaped = false;
		//MapEnabled=false �܂��̓������}�b�v�h�t�@�C���̃I�[�v���Ɏ��s�����ꍇ�̓������ɒʏ�Ǎ�
		try {
			if (MapEnabled && BuffSize>FAILED_BUF_SIZE) BuffSize = FAILED_BUF_SIZE;	//���s������8MB�ȉ��œǍ�
			std::unique_ptr<TFileStream> fs(
				new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
			ByteBuff.Length = BuffSize;
			fs->Seek(top_adr, (TSeekOrigin)soFromBeginning);
			BuffSize = fs->Read(ByteBuff, BuffSize);
			return true;
		}
		catch (...) {
			BuffSize = 0;
			ByteBuff.Length = 0;
			return false;
		}
	}
}

//---------------------------------------------------------------------------
//������
//---------------------------------------------------------------------------
void MemMapFile::Initialize()
{
	hFile	 = INVALID_HANDLE_VALUE;
	hMap	 = NULL;
	pPointer = NULL;
	FileSize = 0;
	BuffSize = 0;
	ByteBuff.Length = 0;
	isMaped  = false;
	ErrMsg	 = EmptyStr;
}

//---------------------------------------------------------------------------
//���ď�����
//---------------------------------------------------------------------------
void MemMapFile::Close()
{
	if (pPointer) ::UnmapViewOfFile(pPointer);
	if (hMap)     ::CloseHandle(hMap);
	if (hFile!=INVALID_HANDLE_VALUE) ::CloseHandle(hFile);

	Initialize();
}
//---------------------------------------------------------------------------
