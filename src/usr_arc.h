//----------------------------------------------------------------------//
// �A�[�J�C�u����														//
//																		//
//----------------------------------------------------------------------//
#ifndef UserArcUnitH
#define UserArcUnitH

//---------------------------------------------------------------------------
#ifndef FNAME_MAX32
#define FNAME_MAX32	512
#endif

typedef	HGLOBAL		HARC;

#pragma pack(push, 1)
//---------------------------------------------------------------------------
typedef struct {
	DWORD	dwOriginalSize;
	DWORD	dwCompressedSize;
	DWORD	dwCRC;
	UINT	uFlag;
	UINT	uOSType;
	WORD	wRatio;
	WORD	wDate;
	WORD	wTime;
	char	szFileName[FNAME_MAX32 + 1];
	char	dummy1[3];
	char	szAttribute[8];
	char	szMode[8];
} INDIVIDUALINFO, *LPINDIVIDUALINFO;
//---------------------------------------------------------------------------
#pragma pack(pop)

//���Ɍ`��
#ifndef ARCHIVETYPE_ZIP
#define ARCHIVETYPE_ZIP		1
#define ARCHIVETYPE_7Z		2
#endif

//�t�@�C������
#ifndef FA_RDONLY
#define FA_RDONLY		0x01			//�ǂݎ���p
#define FA_HIDDEN		0x02			//�B���t�@�C��
#define FA_SYSTEM		0x04			//�V�X�e���t�@�C��
#define FA_LABEL		0x08			//�{�����[�����x��
#define FA_DIREC		0x10			//�f�B���N�g��
#define FA_ARCH 		0x20			//�A�[�J�C�u
#define FA_ENCRYPTED	0x40			//�p�X���[�h�ی�
#endif

//OpenArchive���[�h
#define M_ERROR_MESSAGE_OFF		0x00800000L	//�G���[��\�����Ȃ�

//�G���[
#define ERROR_USER_CANCEL		0x8020

//---------------------------------------------------------------------------
#define MAX_ARC_DLL	6

//�A�[�J�C�u���
#define	UARCTYP_ZIP	1
#define	UARCTYP_7Z	2
#define	UARCTYP_LHA	3
#define	UARCTYP_CAB	4
#define	UARCTYP_TAR	5
#define	UARCTYP_RAR	6
#define	UARCTYP_ISO	7

//�Ή��g���q
#define FEXT_ZIP	_T(".zip.apk.jar.ear.sar.war.cbz.epub.gdtf.mobi.docx.dotx.pptx.xlsx.xltx.odt.ods.odp.odg.odc.odf.odi.odm.odb")
#define FEXT_7Z		_T(".7z.cb7")
#define FEXT_LHA	_T(".lzh")
#define FEXT_CAB	_T(".cab")
#define FEXT_TAR	_T(".tar.gz.tgz.taz.bz2.xz.lzma.cbt")
#define FEXT_RAR	_T(".rar.cbr")
#define FEXT_ISO	_T(".iso")

#define FEXT_ZIPIMG	_T(".zip.cbz.epub.gdtf")	//�摜�����\���̂���ZIP�t�@�C��

#define ARC_OUTBUF_SIZE_L	1048576	//1MB
#define ARC_OUTBUF_SIZE_S	4096

//---------------------------------------------------------------------------
struct arc_find_inf {
	UnicodeString f_name;		//�p�X�t�t�@�C����
	bool is_dir;				//�f�B���N�g��
	__int64 f_size;				//�T�C�Y
	int  f_attr;				//����
	TDateTime f_time;			//�^�C���X�^���v
};

//---------------------------------------------------------------------------
typedef int   (WINAPI *FUNC_Arc)(const HWND hwnd, LPCSTR szCmdLine, LPSTR szOutput, const DWORD dwSize);
typedef WORD  (WINAPI *FUNC_ArcGetVersion)(VOID);
typedef BOOL  (WINAPI *FUNC_ArcGetRunning)(VOID);
typedef int   (WINAPI *FUNC_ArcGetFileCount)(LPCSTR szFileName);
typedef HARC  (WINAPI *FUNC_ArcOpenArchive)(const HWND hwnd, LPCSTR szFileName, const DWORD dwMode);
typedef int   (WINAPI *FUNC_ArcCloseArchive)(HARC harc);
typedef int   (WINAPI *FUNC_ArcFindFirst)(HARC harc, LPCSTR szWildName, INDIVIDUALINFO *lpSubInfo);
typedef int   (WINAPI *FUNC_ArcFindNext)(HARC harc, INDIVIDUALINFO *lpSubInfo);
typedef BOOL  (WINAPI *FUNC_ArcGetOriginalSizeEx)(HARC _harc, __int64 *_lpllSize);
typedef int   (WINAPI *FUNC_ArcGetAttribute)(HARC _harc);
typedef BOOL  (WINAPI *FUNC_ArcSetUnicodeMode)(BOOL bUnicode);
typedef BOOL  (WINAPI *FUNC_ArcSetBackGroundMode)(const BOOL bBackGroundMode);
typedef BOOL  (WINAPI *FUNC_ArcCheckArchive)(LPCSTR szFileName, const int _iMode);
typedef int   (WINAPI *FUNC_ArcGetLastError)(LPDWORD lpdwSystemError);
typedef BOOL  (WINAPI *FUNC_SevenZipExists7zdll)(VOID);
typedef int   (WINAPI *FUNC_ArcGetArchiveType)(LPCSTR szFileName);

/*
typedef DWORD (WINAPI *FUNC_ArcGetArcFileSize)(HARC harc);
typedef BOOL  (WINAPI *FUNC_ArcGetArcFileSizeEx)(HARC _harc, __int64 *_lpllSize);
typedef DWORD (WINAPI *FUNC_ArcGetArcOriginalSize)(HARC harc);
typedef BOOL  (WINAPI *FUNC_ArcGetArcOriginalSizeEx)(HARC _harc, __int64 *_lpllSize);
*/

//---------------------------------------------------------------------------
struct arc_func {
	FUNC_Arc						Command;
	FUNC_ArcGetVersion				GetVersion;
	FUNC_ArcGetRunning				GetRunning;
	FUNC_ArcGetFileCount			GetFileCount;
	FUNC_ArcOpenArchive				OpenArchive;
	FUNC_ArcCloseArchive			CloseArchive;
	FUNC_ArcFindFirst				FindFirst;
	FUNC_ArcFindNext				FindNext;
	FUNC_ArcGetOriginalSizeEx		GetOriginalSizeEx;
	FUNC_ArcGetAttribute			GetAttribute;
	FUNC_ArcSetUnicodeMode			SetUnicodeMode;
	FUNC_ArcSetBackGroundMode		SetBackGroundMode;
	FUNC_ArcCheckArchive			CheckArchive;
	FUNC_ArcGetLastError			GetLastError;
	FUNC_ArcGetArchiveType			GetArchiveType;		//7-zip32.dll/tar32.dll �Ǝ�
	FUNC_SevenZipExists7zdll		Exists7zdll;		//7z.dll�Ή���7-zip32.dll �Ǝ�

/*	���m���łȂ��ꍇ�����邽�߈ȉ��͎g�p���Ȃ�(TAR,CAB)
	FUNC_ArcGetArcFileSize			GetArcFileSize;
	FUNC_ArcGetArcFileSizeEx		GetArcFileSizeEx;
	FUNC_ArcGetArcOriginalSize		GetArcOriginalSize;
	FUNC_ArcGetArcOriginalSizeEx	GetArcOriginalSizeEx;
*/

	UnicodeString DllName;
	UnicodeString FileName;
	UnicodeString VerStr;
	HINSTANCE hDll;
	UnicodeString Prefix;

	bool Available;
	bool IsUnicode;
	bool hasRename;

	bool use7zdll;
	bool err7zdll;
};

//---------------------------------------------------------------------------
typedef void (*FUNC_AddDebugLog)(UnicodeString, UnicodeString, UnicodeString);

//---------------------------------------------------------------------------
class UserArcUnit
{
private:
	HWND MainHandle;
	int  CurType;
	HARC hCurArc;
	UnicodeString CurArcFile;

	INDIVIDUALINFO FindInf;
	UnicodeString  FindDir;

	FARPROC GetProcAdr(arc_func *fp, UnicodeString pnam);
	arc_func* GetArcFunc(int typ);
	void SetFindInf(arc_find_inf *inf, bool sub_dir = false);
	UnicodeString GetDispStr(char *s, int arc_t = 0);

public:
	arc_func ArcFunc[MAX_ARC_DLL];

	UnicodeString ResMsg;		//�R�}���h�̉������b�Z�[�W
	UnicodeString ErrMsg;		//�G���[���b�Z�[�W

	FUNC_AddDebugLog fpAddDebugLog;		//�f�o�b�O���o�͊֐�

	bool Busy;
	bool RarUnpacking;			//RAR �𓀒�

	bool Use7zDll;				//7z.dll �𗘗p�\
	UnicodeString FExt7zDll;	//7z.dll �Ή��g���q

	bool ZipPrm_sfx;			//ZIP���ȉ�
	int  ZipPrm_x;				//ZIP���k���x��
	int  SevenPrm_x;			//7z���k���x��
	int  TarPrm_z;				//TAR���k���x��(gzip)
	int  CabPrm_z;				//CAB���k���x��(0=MSZIP/ 15�`21=LZX)

	UnicodeString ExSw_Zip;		//�ǉ��X�C�b�`�I�v�V����
	UnicodeString ExSw_7z;
	UnicodeString ExSw_Lha;
	UnicodeString ExSw_Tar;
	UnicodeString ExSw_Cab;

	UserArcUnit(HWND hWnd);
	~UserArcUnit();

	int  GetArcType(UnicodeString arc_file, bool ex_sw = false);
	UnicodeString GetSubTypeStr(UnicodeString arc_file);

	bool IsRunning(UnicodeString arc_file);
	bool IsUnicode(int typ);
	bool IsAvailable(int typ);
	bool IsAvailable(UnicodeString arc_file, bool ex_sw = false);
	bool HasRename(UnicodeString arc_file);

	int ExeCommand(int arc_t, arc_func *fp, UnicodeString cmd, bool large_buf = false);

	bool Pack(int arc_t, UnicodeString arc_file, UnicodeString src_dir, UnicodeString src_files,
		UnicodeString password = EmptyStr);
	bool UnPack(UnicodeString arc_file, UnicodeString dst_dir,
		UnicodeString fnam = EmptyStr, bool dir_sw = true, bool hide_sw = false, bool ow_sw = false);

	bool AddFile(UnicodeString arc_file, UnicodeString fnam);
	bool DelFile(UnicodeString arc_file, UnicodeString fnam, bool hide_sw = false);
	bool RenFile(UnicodeString arc_file, UnicodeString onam, UnicodeString nnam);

	bool OpenArc(UnicodeString arc_file);
	bool FindFirst(UnicodeString mask, INDIVIDUALINFO *inf);
	bool FindFirstEx(UnicodeString dnam, arc_find_inf *inf, bool sub_dir = false);
	bool FindNext(INDIVIDUALINFO *inf);
	bool FindNextEx(arc_find_inf *inf, bool sub_dir = false);
	void CloseArc();

	int  GetRootCount(UnicodeString arc_file);

	bool GetFileInf(UnicodeString arc_file, UnicodeString fnam, arc_find_inf *inf);
	void GetDirList(UnicodeString arc_file, TStrings *d_lst);
	void GetItemList(UnicodeString arc_file, UnicodeString dnam, TStrings *f_lst, TStrings *d_lst);

	bool HasZipImg(UnicodeString arc_file, UnicodeString xlist);
	bool GetFileList(UnicodeString arc_file, TStringList *lst);
	UnicodeString GetFirstFile(UnicodeString arc_file, UnicodeString xlist);

	bool GetArcInfo(UnicodeString arc_file, int *f_cnt, __int64 *org_size,
		float *ratio = NULL, bool *is_enc = NULL, TStringList *lst = NULL);
	bool GetArcList(UnicodeString arc_file, UnicodeString cmd, TStringList *lst);

	bool SetArcTime(UnicodeString arc_file, bool force = false);
};
//---------------------------------------------------------------------------
#endif
