//----------------------------------------------------------------------//
// ファイル操作 (260文字以上のパスに対応)								//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFileExH
#define UsrFileExH

//---------------------------------------------------------------------------
#include <winternl.h>

//---------------------------------------------------------------------------
#define	FileRenameInformation (FILE_INFORMATION_CLASS)10

typedef struct _FILE_RENAME_INFORMATION {
	BOOLEAN ReplaceIfExists;
	HANDLE	RootDirectory;
	ULONG	FileNameLength;
	WCHAR	FileName[1];
} FILE_RENAME_INFORMATION, *PFILE_RENAME_INFORMATION;

typedef LONG (WINAPI *FUNC_NtSetInformationFile)(HANDLE, PIO_STATUS_BLOCK, PVOID, ULONG, FILE_INFORMATION_CLASS);

//---------------------------------------------------------------------------
extern UnicodeString ExePath;

//---------------------------------------------------------------------------
int __fastcall FileComp_Base(TStringList *List, int Index1, int Index2);

UnicodeString cv_ex_filename(UnicodeString fnam);
UnicodeString cv_env_var(UnicodeString s);
UnicodeString cv_env_str(UnicodeString s);
UnicodeString get_actual_path(UnicodeString pnam);
UnicodeString get_actual_name(UnicodeString fnam, bool sw_fmt = false);
UnicodeString exclude_env_path(UnicodeString fnam);
UnicodeString to_relative_name(UnicodeString fnam);
UnicodeString to_absolute_name(UnicodeString fnam, UnicodeString rnam = EmptyStr);
UnicodeString extract_file_path(UnicodeString fnam);
bool is_same_file(UnicodeString fnam1, UnicodeString fnam2, UnicodeString rnam = EmptyStr);
bool match_path_list(UnicodeString dnam, UnicodeString plist, bool start_sw = false);

UnicodeString get_root_name(UnicodeString pnam);
UnicodeString get_case_name(UnicodeString fnam);

bool is_root_dir(UnicodeString dnam);
bool is_root_unc(UnicodeString dnam);
UnicodeString exclede_delimiter_if_root(UnicodeString dnam);

UnicodeString get_drive_str(UnicodeString dnam);
bool drive_exists(UnicodeString drv);
UINT get_drive_type(UnicodeString dnam);

int get_ClusterSize(UnicodeString dnam);

UnicodeString get_base_name(UnicodeString fnam);

UnicodeString get_extension(UnicodeString fnam);
UnicodeString get_extension_if_file(UnicodeString fnam);

UnicodeString nrm_FileExt(UnicodeString fext);
bool test_FileExt(UnicodeString fext, UnicodeString list);
bool test_FileExtSize(UnicodeString fext, UnicodeString list, __int64 size);

UnicodeString to_path_name(UnicodeString dnam);
UnicodeString get_dir_name(UnicodeString dnam);
UnicodeString get_parent_path(UnicodeString dnam);

UnicodeString nrm_ftp_path(UnicodeString pnam);

TStringDynArray split_path(UnicodeString pnam, UnicodeString dlmt);
TStringDynArray split_path(UnicodeString pnam);

UnicodeString split_user_name(UnicodeString &dnam);

bool is_computer_name(UnicodeString pnam);

bool is_dir_accessible(UnicodeString dnam);
bool is_drive_accessible(UnicodeString dnam);
bool is_drive_protected(UnicodeString dnam);

bool set_CompressAttr(UnicodeString fnam, bool sw);

int  file_GetAttr(UnicodeString fnam);
bool file_SetAttr(UnicodeString fnam, int attr);
bool dir_CopyAttr(UnicodeString src_nam, UnicodeString dst_nam, bool remove_ro = false);
bool set_FileWritable(UnicodeString fnam);
bool is_SymLink(UnicodeString fnam);
int  get_HardLinkCount(UnicodeString fnam);
bool is_IdenticalFile(UnicodeString fnam1, UnicodeString fnam2);

bool file_exists(UnicodeString fnam);
bool file_exists_x(UnicodeString fnam);
bool file_exists_ico(UnicodeString fnam);
bool file_exists_wc(UnicodeString &fnam);
bool dir_exists(UnicodeString dnam);

TDateTime utc_to_DateTime(FILETIME *utc);
TDateTime get_file_age(UnicodeString fnam);
__int64 get_file_size(UnicodeString fnam);
__int64 get_comp_size(UnicodeString fnam);

bool is_ADS_name(UnicodeString fnam);
int  pos_ADS_delimiter(UnicodeString fnam);
UnicodeString split_ADS_name(UnicodeString &fnam);
int  delete_ADS(UnicodeString fnam, bool force = false, UnicodeString mask = "*", TStringList *lst = NULL);
bool rename_ADS(UnicodeString fnam, UnicodeString new_name);

UnicodeString chk_cre_dir(UnicodeString dnam);

bool is_EmptyDir(UnicodeString dnam, bool no_file = false);

int  get_available_drive_list(TStringList *lst);
UnicodeString get_file_attr_str(int atr);

void get_files(UnicodeString pnam, UnicodeString mask, TStrings *lst, bool subSW = false);

int  get_all_files_ex(UnicodeString pnam, UnicodeString mask, TStrings *lst,
	bool sub_sw = false, int sub_n = 99, UnicodeString skip_dir = EmptyStr,
	bool inc_hide = true, bool inc_sys = true);

bool set_file_age(UnicodeString fnam, TDateTime dt, bool force = false);
bool create_EmptyFile(UnicodeString fnam);
bool create_Dir(UnicodeString dnam);
bool create_ForceDirs(UnicodeString dnam);
bool delete_Dir(UnicodeString dnam, bool force = false);
bool delete_Dirs(UnicodeString pnam);
bool move_File(UnicodeString old_nam, UnicodeString new_nam);
bool copy_File(UnicodeString src_nam, UnicodeString dst_nam);
bool rename_File(UnicodeString old_nam, UnicodeString new_nam);
bool rename_Path(UnicodeString old_nam, UnicodeString new_nam);

//---------------------------------------------------------------------------
#ifndef REPARSE_DATA_BUFFER
typedef struct _REPARSE_DATA_BUFFER {
	ULONG  ReparseTag;
	USHORT ReparseDataLength;
	USHORT Reserved;

	union {
		struct {
		  USHORT SubstituteNameOffset;
		  USHORT SubstituteNameLength;
		  USHORT PrintNameOffset;
		  USHORT PrintNameLength;
		  ULONG  Flags;
		  WCHAR  PathBuffer[1];
		} SymbolicLinkReparseBuffer;

		struct {
		  USHORT SubstituteNameOffset;
		  USHORT SubstituteNameLength;
		  USHORT PrintNameOffset;
		  USHORT PrintNameLength;
		  WCHAR  PathBuffer[1];
		} MountPointReparseBuffer;

		struct {
		  UCHAR DataBuffer[1];
		} GenericReparseBuffer;
	};
} REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#endif

//---------------------------------------------------------------------------
bool cre_Junction(UnicodeString pnam, UnicodeString target);

//---------------------------------------------------------------------------
BYTE   fsRead_byte(TFileStream *fs);
int    fsRead_int2(TFileStream *fs, bool be_sw = false);
int    fsRead_int3(TFileStream *fs, bool be_sw = false);
int    fsRead_int4(TFileStream *fs, bool be_sw = false);
double fsRead_double(TFileStream *fs);
UnicodeString fsRead_char(TFileStream *fs, int size);
UnicodeString fsRead_comment_utf8(TFileStream *fs);
bool fsRead_check_char(TFileStream *fs, const char *s);

//---------------------------------------------------------------------------
inline UnicodeString warn_filename_RLO(UnicodeString fnam)
{
	return ReplaceStr(ExtractFileName(fnam), L"\u202e", "<RLO>");
}
//---------------------------------------------------------------------------
inline UnicodeString warn_pathname_RLO(UnicodeString pnam)
{
	return ReplaceStr(pnam, L"\u202e", "<RLO>");
}
//---------------------------------------------------------------------------
#endif
