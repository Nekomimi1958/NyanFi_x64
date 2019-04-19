//----------------------------------------------------------------------//
// �t�@�C������ (260�����ȏ�̃p�X�ɑΉ�)								//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <winioctl.h>
#include <memory>
#include "usr_str.h"
#include "usr_file_ex.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UnicodeString ExePath;		//�N���p�X

//---------------------------------------------------------------------------
//260�����ȏ�̃p�X���ɑΉ����邽�߂̃t�@�C�����ϊ�
//  �������󔒂̏ꍇ�ɂ��Ή�
//---------------------------------------------------------------------------
UnicodeString cv_ex_filename(UnicodeString fnam)
{
	if (fnam.Length()>=MAX_PATH || EndsStr(' ', fnam)) {
		if (StartsStr("\\\\", fnam)) fnam.Insert("?\\UNC\\", 3); else fnam.Insert("\\\\?\\", 1);
	}
	return fnam;
}

//---------------------------------------------------------------------------
//���ϐ���ϊ�
//---------------------------------------------------------------------------
UnicodeString cv_env_var(UnicodeString s)
{
	for (;;) {
		if (!ContainsStr(s, "%")) break;
		UnicodeString estr = get_tkn_m(s, '%', '%');
		if (!ContainsText(s, "%" + estr + "%")) break;
		UnicodeString eval = GetEnvironmentVariable(estr);  if (eval.IsEmpty()) break;
		s = ReplaceText(s, "%" + estr + "%", eval);
	}

	return s;
}

//---------------------------------------------------------------------------
//���ϐ����܂ޕ������ϊ�
//---------------------------------------------------------------------------
UnicodeString cv_env_str(UnicodeString s)
{
	s = ReplaceText(s, "%ExePath%", ExePath);
	s = cv_env_var(s);
	return s;
}

//---------------------------------------------------------------------------
//���ϐ��⏑����������܂ޕ����񂩂���ۂ̃p�X�����擾
//---------------------------------------------------------------------------
UnicodeString get_actual_path(UnicodeString pnam)
{
	pnam = cv_env_str(pnam);

	if (ContainsStr(pnam, "$")) {
		pnam = ReplaceStr(pnam, "$$", "\f");	//$$ ���ꎞ�I�� \f �ɕς��Ă�����
		pnam = ReplaceStr(pnam, "$X", ExcludeTrailingPathDelimiter(ExePath));
		pnam = ReplaceStr(pnam, "$D", ExtractFileDrive(ExePath));
		pnam = ReplaceStr(pnam, "\f", "$");		//$$(\f) �� $ �ɕϊ�
	}
	return pnam;
}

//---------------------------------------------------------------------------
//�p�X���ʂ��Ă���ꍇ�A�p�X�����t�@�C�������p�X�t���ɕϊ�
//---------------------------------------------------------------------------
UnicodeString get_actual_name(UnicodeString fnam)
{
	fnam = cv_env_str(fnam);

	if (!fnam.IsEmpty() && fnam.Pos('\\')==0) {
		bool chk_ext = get_extension(fnam).IsEmpty();
		TStringDynArray plst = split_strings_semicolon(GetEnvironmentVariable("PATH"));
		TStringDynArray xlst = split_strings_semicolon(GetEnvironmentVariable("PATHEXT"));
		UnicodeString rnam;
		for (int i=0; i<plst.Length && rnam.IsEmpty(); i++) {
			UnicodeString pnam = IncludeTrailingPathDelimiter(plst[i]);
			if (chk_ext) {
				for (int j=0; j<xlst.Length && rnam.IsEmpty(); j++) {
					UnicodeString anam = pnam + fnam + xlst[j];
					if (file_exists(anam)) rnam = anam;
				}
			}
			else if (file_exists(pnam + fnam)) {
				rnam = pnam + fnam;
			}
		}
		if (!rnam.IsEmpty()) fnam = rnam;
	}

	return fnam;
}

//---------------------------------------------------------------------------
//�p�X���ʂ��Ă���ꍇ�A�t�@�C��������p�X�Ɗg���q����菜��
//---------------------------------------------------------------------------
UnicodeString exclude_env_path(UnicodeString fnam)
{
	if (fnam.IsEmpty() || ExtractFileDrive(fnam).IsEmpty()) return fnam;

	UnicodeString fext = get_extension(fnam);
	if (fext.IsEmpty()) return fnam;

	//�g���q���`�F�b�N
	TStringDynArray xlst = split_strings_semicolon(GetEnvironmentVariable("PATHEXT"));
	bool ok = false;
	for (int i=0; i<xlst.Length && !ok; i++) ok = SameText(fext, xlst[i]);

	//�p�X���`�F�b�N
	if (ok) {
		TStringDynArray plst = split_strings_semicolon(GetEnvironmentVariable("PATH"));
		UnicodeString pnam = ExtractFilePath(fnam);
		for (int i=0; i<plst.Length; i++) {
			if (SameText(IncludeTrailingPathDelimiter(plst[i]), pnam)) {
				fnam = ChangeFileExt(ExtractFileName(fnam), EmptyStr);
				break;
			}
		}
	}

	return fnam;
}

//---------------------------------------------------------------------------
//���s�t�@�C���̃f�B���N�g������̑��΃t�@�C�����ɕϊ�
//---------------------------------------------------------------------------
UnicodeString to_relative_name(UnicodeString fnam)
{
	remove_top_text(fnam, ExePath);
	return fnam;
}

//---------------------------------------------------------------------------
//��΃p�X�ɕϊ� (�h���C�u�����܂ޏꍇ�͐��K��)
//---------------------------------------------------------------------------
UnicodeString to_absolute_name(
	UnicodeString fnam,		//�ϊ��Ώۃt�@�C����
	UnicodeString rnam)		//��f�B���N�g���� (default = EmptyStr/ ExePath);
{
	if (fnam.IsEmpty()) return EmptyStr;

	//.. ���܂ރp�X�𐳋K��
	if (!ExtractFileDrive(fnam).IsEmpty()) {
		const _TCHAR *ptn = _T("\\\\[^\\\\\\.]+\\\\\\.\\.\\\\");
		while (is_match_regex(fnam, ptn)) fnam = replace_regex(fnam, ptn, _T("\\"));
		return fnam;
	}

	//���΃p�X�����΃p�X��
	if (rnam.IsEmpty()) rnam = ExePath;
	for (;;) {
		if (remove_top_text(fnam, _T(".\\"))) continue;
		if (!remove_top_text(fnam, _T("..\\"))) break;
		rnam = get_parent_path(rnam);
	}
	return contains_PathDlmtr(rnam)? rnam + fnam : EmptyStr;
}

//---------------------------------------------------------------------------
//��փf�[�^�X�g���[�����l�������p�X���̎擾
//---------------------------------------------------------------------------
UnicodeString extract_file_path(UnicodeString fnam)
{
	int p = fnam.Length() - ExtractFileName(fnam).Length();
	if (p>2 && fnam[p]==':') return ExtractFilePath(fnam.SubString(1, p - 1));

	return ExtractFilePath(fnam);
}

//---------------------------------------------------------------------------
//�p�X���X�g�Ƀ}�b�`���邩?
//---------------------------------------------------------------------------
bool match_path_list(
	UnicodeString dnam,		//�`�F�b�N�Ώ�
	UnicodeString plist,	//�p�X���X�g(���ϐ��A%ExePath% �Ή�)
	bool start_sw)			//true : �O����v	(default = false : ������v)
{
	bool ret = false;
	TStringDynArray path_lst = split_strings_semicolon(plist);
	for (int i=0; i<path_lst.Length && !ret; i++) {
		UnicodeString pnam = cv_env_str(IncludeTrailingPathDelimiter(path_lst[i]));
		if (pnam.IsEmpty()) continue;
		ret = (start_sw? StartsText(pnam, dnam) : ContainsText(dnam, pnam));
	}
	return ret;
}

//---------------------------------------------------------------------------
//����t�@�C����?
//---------------------------------------------------------------------------
bool is_same_file(UnicodeString fnam1, UnicodeString fnam2,
	UnicodeString rnam)		//��t�@�C���� (default = EmptyStr/ ExePath);
{
	fnam1 = to_absolute_name(exclude_quot(fnam1), rnam);
	fnam2 = to_absolute_name(exclude_quot(fnam2), rnam);
	return SameText(fnam1, fnam2);
}

//---------------------------------------------------------------------------
//���[�g��(�h���C�u���܂���UNC�̃R���s���[�^��)���擾
//---------------------------------------------------------------------------
UnicodeString get_root_name(UnicodeString pnam)
{
	UnicodeString rstr = pnam;
	if (remove_top_text(rstr, _T("\\\\")))
		rstr = "\\\\" + get_tkn(rstr, '\\');
	else
		rstr = ExtractFileDrive(rstr);
	return IncludeTrailingPathDelimiter(rstr);
}

//---------------------------------------------------------------------------
//���ۂ̑召�����Ńt�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_case_name(UnicodeString fnam)
{
	UnicodeString drv_nam = IncludeTrailingPathDelimiter(ExtractFileDrive(fnam));
	UnicodeString ret_str = drv_nam;
	UnicodeString pnam;
	TStringDynArray plst = split_path(fnam);
	for (int i=0; i<plst.Length-1; i++) {
		pnam += plst[i] + "\\";
		if (USAME_TS(pnam, "\\") || USAME_TS(pnam, "\\\\")) continue;
		UnicodeString sea_str = pnam + plst[i + 1];
		if (SameText(IncludeTrailingPathDelimiter(sea_str), drv_nam)) continue;
		if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);
		TSearchRec sr;
		if (FindFirst(sea_str, faAnyFile, sr)==0) {
			ret_str += sr.Name;
			if (sr.Attr & faDirectory) ret_str = IncludeTrailingPathDelimiter(ret_str);
			FindClose(sr);
		}
		else {
			//������Ȃ������炻�̂܂ܕԂ�
			ret_str = fnam;
			break;
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//���[�g�f�B���N�g����?
//---------------------------------------------------------------------------
bool is_root_dir(UnicodeString dnam)
{
	if (remove_top_s(dnam, _T("\\\\")))
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(get_tkn_r(dnam, '\\')));
	else
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(dnam));
}
//---------------------------------------------------------------------------
bool is_root_unc(UnicodeString dnam)
{
	if (remove_top_s(dnam, _T("\\\\")))
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(get_tkn_r(dnam, '\\')));
	else
		return false;
}

//---------------------------------------------------------------------------
//�f�B���N�g�������[�g�̏ꍇ�A������ \ ����菜��
//---------------------------------------------------------------------------
UnicodeString exclede_delimiter_if_root(UnicodeString dnam)
{
	if (!dnam.IsEmpty()) {
		dnam = is_root_dir(dnam) ? ExcludeTrailingPathDelimiter(dnam)
								 : IncludeTrailingPathDelimiter(dnam);
	}
	return dnam;
}

//---------------------------------------------------------------------------
//�h���C�u��(�啶���A\�t��)���擾
//---------------------------------------------------------------------------
UnicodeString get_drive_str(UnicodeString dnam)
{
	return IncludeTrailingPathDelimiter(ExtractFileDrive(dnam)).UpperCase();
}

//---------------------------------------------------------------------------
//�h���C�u���L����?
//---------------------------------------------------------------------------
bool drive_exists(UnicodeString drv)
{
	drv = get_tkn(drv, ':');	if (drv.IsEmpty())	 return false;
	int d_n = drv[1] - _T('A');	if (d_n<0 || d_n>25) return false;
	DWORD d_flag = 0x00000001 << d_n;
	DWORD d_bit = ::GetLogicalDrives();	//���p�\�ȃh���C�u���r�b�g�}�X�N�`���Ŏ擾
	return (d_bit & d_flag);
}

//---------------------------------------------------------------------------
//�h���C�u�̎�ނ��擾
//---------------------------------------------------------------------------
UINT get_drive_type(UnicodeString dnam)
{
	return ::GetDriveType(get_drive_str(dnam).c_str());
}

//---------------------------------------------------------------------------
//�N���X�^�T�C�Y���擾
//---------------------------------------------------------------------------
int get_ClusterSize(UnicodeString dnam)
{
	DWORD SecPerCl, BytePerSec, FreeCl, TotalCl;
	if (::GetDiskFreeSpace(get_drive_str(dnam).c_str(), &SecPerCl, &BytePerSec, &FreeCl, &TotalCl))
		return (BytePerSec * SecPerCl);
	else
		return 0;
}

//---------------------------------------------------------------------------
//�t�@�C���̃x�[�X�����擾
//---------------------------------------------------------------------------
UnicodeString get_base_name(UnicodeString fnam)
{
	fnam = ExtractFileName(fnam);
	if (fnam.IsEmpty()) return EmptyStr;
	return StartsStr('.', fnam)? "." + ChangeFileExt(exclude_top(fnam), EmptyStr) : ChangeFileExt(fnam, EmptyStr);
}
//---------------------------------------------------------------------------
//�g���q���擾 (�h�b�g�t�@�C�����l��)
//---------------------------------------------------------------------------
UnicodeString get_extension(UnicodeString fnam)
{
	fnam = ExtractFileName(fnam);
	if (fnam.IsEmpty()) return EmptyStr;
	return ExtractFileExt(StartsStr('.', fnam)? exclude_top(fnam) : fnam);
}
//---------------------------------------------------------------------------
//�t�@�C���Ȃ�g���q���擾
//---------------------------------------------------------------------------
UnicodeString get_extension_if_file(UnicodeString fnam)
{
	return dir_exists(fnam)? EmptyStr : get_extension(fnam);
}

//---------------------------------------------------------------------------
//�g���q�̃`�F�b�N
//---------------------------------------------------------------------------
bool test_FileExt(UnicodeString fext, UnicodeString list)
{
	if (list.IsEmpty() || fext.IsEmpty() || USAME_TS(fext, ".")) return false;
	if (USAME_TS(list, "*") || USAME_TS(list, ".*")) return true;
	if (!StartsStr('.', fext)) fext.Insert(".", 1);
	if (!EndsStr('.', fext))   fext += ".";
	if (!StartsStr('.', list)) list.Insert(".", 1);
	if (!EndsStr('.', list))   list += ".";
	return ContainsText(list, fext);
}
//---------------------------------------------------------------------------
bool test_FileExt(UnicodeString fext, const _TCHAR *list)
{
	return test_FileExt(fext, UnicodeString(list));
}
//---------------------------------------------------------------------------
//�g���q�̃`�F�b�N(�T�C�Y�����Ή�)
//---------------------------------------------------------------------------
bool test_FileExtSize(UnicodeString fext, UnicodeString list, __int64 size)
{
	bool ret = false;
	TStringDynArray xlst = SplitString(list, ".");
	UnicodeString xstr;
	for (int i=0; i<xlst.Length; i++) {
		if (test_FileExt(fext, get_tkn(xlst[i], ":"))) xstr =xlst[i];
	}
	if (!xstr.IsEmpty()) {
		__int64 sz = get_tkn_r(xstr, ":").ToIntDef(0) * 1048576ul;	//MB
		ret = (size>=sz);
	}

	return ret;
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̖����� \\ �� (EmptyStr �̏ꍇ�� EmptyStr ��Ԃ�)
//---------------------------------------------------------------------------
UnicodeString to_path_name(UnicodeString dnam)
{
	return (!dnam.IsEmpty()? IncludeTrailingPathDelimiter(dnam) : EmptyStr);
}

//---------------------------------------------------------------------------
//�f�B���N�g����(������ \\ ����)���擾
//---------------------------------------------------------------------------
UnicodeString get_dir_name(UnicodeString dnam)
{
	return ExtractFileName(ExcludeTrailingPathDelimiter(dnam));
}

//---------------------------------------------------------------------------
//�e�f�B���N�g�����擾
//---------------------------------------------------------------------------
UnicodeString get_parent_path(UnicodeString dnam)
{
	return ExtractFilePath(ExcludeTrailingPathDelimiter(dnam));
}

//---------------------------------------------------------------------------
//�p�X�𕪊�
//---------------------------------------------------------------------------
TStringDynArray split_path(UnicodeString pnam, UnicodeString dlmt)
{
	if (EndsStr(dlmt, pnam)) pnam.Delete(pnam.Length(), 1);
	UnicodeString top_s = remove_top_s(pnam, dlmt + dlmt)? dlmt + dlmt : EmptyStr;

	TStringDynArray ret_array = SplitString(pnam, dlmt);
	if (!top_s.IsEmpty() && ret_array.Length>0) ret_array[0] = top_s + ret_array[0];

	return ret_array;
}
//---------------------------------------------------------------------------
TStringDynArray split_path(UnicodeString pnam)
{
	pnam = ExcludeTrailingPathDelimiter(pnam);
	const _TCHAR *top_s = remove_top_s(pnam, "\\\\")? _T("\\\\") : NULL;

	TStringDynArray ret_array = SplitString(pnam, "\\");
	if (top_s && ret_array.Length>0) ret_array[0] = top_s + ret_array[0];

	return ret_array;
}

//---------------------------------------------------------------------------
//�R���s���[�^����?
//---------------------------------------------------------------------------
bool is_computer_name(UnicodeString pnam)
{
	if (!remove_top_s(pnam, _T("\\\\"))) return false;
	return (ExcludeTrailingPathDelimiter(pnam).Pos('\\')==0);
}

//---------------------------------------------------------------------------
//�f�B���N�g�����A�N�Z�X�\���H
//---------------------------------------------------------------------------
bool is_dir_accessible(UnicodeString dnam)
{
	if (!is_drive_accessible(dnam)) return false;
	if (is_root_dir(dnam)) return true;

	if (!contains_PathDlmtr(ExcludeTrailingPathDelimiter(dnam))) dnam = to_path_name(dnam);
	if (dnam.IsEmpty()) return false;

	if (dnam.Length()>=248) {
		if (StartsStr("\\\\", dnam)) dnam.Insert("?\\UNC\\", 3); else dnam.Insert("\\\\?\\", 1);
	}
	TSearchRec sr;
	if (FindFirst(dnam + "*.*", faAnyFile, sr)==0) {
		FindClose(sr);
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//�h���C�u���A�N�Z�X�\���H
//---------------------------------------------------------------------------
bool is_drive_accessible(UnicodeString dnam)
{
	if (dnam.IsEmpty()) return false;
	UnicodeString dstr = get_drive_str(dnam);
	if (dstr.IsEmpty()) return false;

	UINT old_mode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
	DWORD MaxCompLen, Flags;
	bool res = (::GetVolumeInformation(dstr.c_str(), NULL, 0, NULL, &MaxCompLen, &Flags, NULL, 0)!=0);
	::SetErrorMode(old_mode);

	return res;
}
//---------------------------------------------------------------------------
//�h���C�u�����C�g�v���e�N�g����Ă��邩�H
//---------------------------------------------------------------------------
bool is_drive_protected(UnicodeString dnam)
{
	bool ret = false;
	dnam = "\\\\.\\" + ExtractFileDrive(dnam);
	HANDLE hDevice = ::CreateFile(dnam.c_str(),
		GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hDevice!=INVALID_HANDLE_VALUE) {
		DWORD dwWritten;
		if (::DeviceIoControl(hDevice, IOCTL_DISK_IS_WRITABLE, NULL, 0, NULL, 0, &dwWritten, NULL)==FALSE &&
			GetLastError()==ERROR_WRITE_PROTECT)
				ret = true;
		::CloseHandle(hDevice);
	}
	return ret;
}

//---------------------------------------------------------------------------
//NTFS���k�����̐ݒ�
//---------------------------------------------------------------------------
bool set_CompressAttr(UnicodeString fnam, bool sw)
{
	int attr = file_GetAttr(fnam);	if (attr==faInvalid) return false;
	bool ret = false;

	bool chg_ro = (attr & faReadOnly);
	if (!file_SetAttr(fnam, attr & ~faReadOnly)) return false;

	HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
		GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
		((attr & faDirectory)? FILE_FLAG_BACKUP_SEMANTICS : 0),
		NULL);

	if (hFile!=INVALID_HANDLE_VALUE) {
		USHORT format = sw? COMPRESSION_FORMAT_DEFAULT : COMPRESSION_FORMAT_NONE;
		DWORD dummy;
		ret = ::DeviceIoControl(hFile, FSCTL_SET_COMPRESSION, &format, sizeof(format), NULL, 0, &dummy, NULL);
		::CloseHandle(hFile);
	}

	if (chg_ro) file_SetAttr(fnam, attr);

	return ret;
}

//---------------------------------------------------------------------------
//�t�@�C���̑������擾
//---------------------------------------------------------------------------
int file_GetAttr(UnicodeString fnam)
{
	if (fnam.Length()>=MAX_PATH || EndsStr(' ', fnam)) {
		if (StartsStr("\\\\", fnam)) fnam.Insert("?\\UNC\\", 3); else fnam.Insert("\\\\?\\", 1);
	}
	return ::GetFileAttributes(fnam.c_str());
}

//---------------------------------------------------------------------------
//�t�@�C���̑�����ݒ�
//---------------------------------------------------------------------------
bool file_SetAttr(UnicodeString fnam, int attr)
{
	try {
		return (FileSetAttr(cv_ex_filename(fnam).c_str(), attr)==0);
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
//�f�B���N�g���̑������R�s�[
//�߂�l: false = ���s
//dst_nam �͖������Ƃ݂Ȃ�(�쐬����̏��)
//---------------------------------------------------------------------------
bool dir_CopyAttr(UnicodeString src_nam, UnicodeString dst_nam,
	bool remove_ro)	//�Ǎ���p����������
{
	int attr = file_GetAttr(src_nam);	if (attr==faInvalid) return false;
	if (remove_ro) attr &= ~faReadOnly;
	if ((attr & (faReadOnly|faHidden|faSysFile)) == 0) return true;
	return file_SetAttr(dst_nam, attr);
}

//---------------------------------------------------------------------------
//�t�@�C���̓ǂݏo����p����������
//---------------------------------------------------------------------------
bool set_FileWritable(UnicodeString fnam)
{
	bool ret = true;
	if (file_exists(fnam)) {
		int atr = file_GetAttr(fnam);
		if (atr!=faInvalid && (atr & faReadOnly)) {
			atr &= ~faReadOnly;
			ret = file_SetAttr(fnam, atr);
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
//���p�[�X�|�C���g?
//---------------------------------------------------------------------------
bool is_SymLink(UnicodeString fnam)
{
	fnam = cv_ex_filename(ExcludeTrailingPathDelimiter(fnam));
	DWORD attr = ::GetFileAttributes(fnam.c_str());
	return (attr!=0xffffffff && (attr & FILE_ATTRIBUTE_REPARSE_POINT));
}

//---------------------------------------------------------------------------
//�����N�J�E���g���擾(2�ȏ�Ȃ�n�[�h�����N)
//---------------------------------------------------------------------------
int get_HardLinkCount(UnicodeString fnam)
{
	int cnt = 0;

	HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
		0, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile!=INVALID_HANDLE_VALUE) {
		BY_HANDLE_FILE_INFORMATION fi;
		if (::GetFileInformationByHandle(hFile, &fi)) cnt = fi.nNumberOfLinks;
		::CloseHandle(hFile);
	}

    return cnt;
}
//---------------------------------------------------------------------------
//�t�@�C���̓��ꐫ�`�F�b�N
//---------------------------------------------------------------------------
bool is_IdenticalFile(UnicodeString fnam1, UnicodeString fnam2)
{
	bool res = false;
	HANDLE hFile1 = ::CreateFile(cv_ex_filename(fnam1).c_str(),
		0, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
	HANDLE hFile2 = ::CreateFile(cv_ex_filename(fnam2).c_str(),
		0, FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile1!=INVALID_HANDLE_VALUE && hFile2!=INVALID_HANDLE_VALUE) {
		BY_HANDLE_FILE_INFORMATION fi1, fi2;
		if (::GetFileInformationByHandle(hFile1, &fi1) && ::GetFileInformationByHandle(hFile2, &fi2)) {
			res = (fi1.dwVolumeSerialNumber==fi2.dwVolumeSerialNumber)
					&& (fi1.nFileIndexHigh==fi2.nFileIndexHigh) && (fi1.nFileIndexLow==fi2.nFileIndexLow);
		}
	}

	if (hFile1) ::CloseHandle(hFile1);
	if (hFile2) ::CloseHandle(hFile2);

	return res;
}

//---------------------------------------------------------------------------
//�t�@�C���̑��݃`�F�b�N
//---------------------------------------------------------------------------
bool file_exists(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return false;
	return (file_GetAttr(fnam) != faInvalid);
}
//---------------------------------------------------------------------------
bool file_exists_x(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return false;
	int attr = file_GetAttr(fnam);

	return (attr!=faInvalid && (attr & faDirectory)==0);
}

//---------------------------------------------------------------------------
//�f�B���N�g���̑��݃`�F�b�N
//---------------------------------------------------------------------------
bool dir_exists(UnicodeString dnam)
{
	if (dnam.IsEmpty()) return false;
	return ::PathIsDirectory(cv_ex_filename(dnam).c_str());
}

//---------------------------------------------------------------------------
//���E���莞�����烍�[�J���� TDateTime ��
//---------------------------------------------------------------------------
TDateTime utc_to_DateTime(FILETIME *utc)
{
	FILETIME f_tm;
	SYSTEMTIME st;
	::FileTimeToLocalFileTime(utc, &f_tm);
	::FileTimeToSystemTime(&f_tm, &st);
	return SystemTimeToDateTime(st);
}

//---------------------------------------------------------------------------
//�t�@�C���̃^�C���X�^���v�� TDateTime �^�Ŏ擾
//---------------------------------------------------------------------------
TDateTime get_file_age(UnicodeString fnam)
{
	TDateTime ft = 0;
	fnam = cv_ex_filename(fnam);
	HANDLE hFile = ::CreateFile(fnam.c_str(),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hFile!=INVALID_HANDLE_VALUE) {
		FILETIME c_tm, a_tm, w_tm, f_tm;
		if (::GetFileTime(hFile, &c_tm, &a_tm, &w_tm)) ft = utc_to_DateTime(&w_tm);
		::CloseHandle(hFile);
	}
	else {
		try {
			FileAge(fnam, ft);
		}
		catch (...) {
			ft = 0;
		}
	}

	return ft;
}

//---------------------------------------------------------------------------
//�^�C���X�^���v��ݒ�
//---------------------------------------------------------------------------
bool set_file_age(UnicodeString fnam, TDateTime dt,
	bool force)		//�Ǎ���p�ł������I�ɕύX (default = false)
{
	bool res = false;

	//�Ǎ���p�Ȃ�ꎞ�I�ɉ���
	int atr = file_GetAttr(fnam);
	bool chg_ro = force && atr!=faInvalid && (atr & faReadOnly);
	if (chg_ro && !set_FileWritable(fnam)) chg_ro = false;

	UnicodeString l_fnam = cv_ex_filename(fnam);
	HANDLE hFile = ::CreateFile(l_fnam.c_str(),
		GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hFile!=INVALID_HANDLE_VALUE) {
		SYSTEMTIME s_tm;
		FILETIME l_tm, f_tm;
		DateTimeToSystemTime(dt, s_tm);
		//�V�X�e������ �� �t�@�C������ �� ���E���莞��
		if (::SystemTimeToFileTime(&s_tm, &l_tm) && ::LocalFileTimeToFileTime(&l_tm, &f_tm)) {
			res = ::SetFileTime(hFile, NULL, NULL, &f_tm);
		}
		::CloseHandle(hFile);
	}

	//���������ɖ߂�
	if (chg_ro) file_SetAttr(fnam, atr);

	return res;
}

//---------------------------------------------------------------------------
//�t�@�C���T�C�Y���擾
//---------------------------------------------------------------------------
__int64 get_file_size(UnicodeString fnam)
{
	__int64 size = 0;
	HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
		GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile!=INVALID_HANDLE_VALUE) {
		DWORD size_hi;
		DWORD size_lo = ::GetFileSize(hFile, &size_hi);
		size = size_hi;
		size = (size << 32) + size_lo;
		::CloseHandle(hFile);
	}
	return size;
}
//---------------------------------------------------------------------------
//���k�T�C�Y���擾
//---------------------------------------------------------------------------
__int64 get_comp_size(UnicodeString fnam)
{
	DWORD sz_hi;
	DWORD sz_lo = ::GetCompressedFileSize(cv_ex_filename(fnam).c_str(), &sz_hi);
	if (sz_lo==INVALID_FILE_SIZE && GetLastError()!=NO_ERROR) return -1;
	return ((__int64)sz_hi<<32) + sz_lo;
}

//---------------------------------------------------------------------------
//��փf�[�^�X�g���[���t���p�X����?
//---------------------------------------------------------------------------
bool is_ADS_name(UnicodeString fnam)
{
	int p = fnam.Length() - ExtractFileName(fnam).Length();
	return (p>2 && fnam[p]==':');
}

//---------------------------------------------------------------------------
//��փf�[�^�X�g���[���̍폜
//�߂�l: 1=����/ 0=���s(lst->Text �ɃG���[���b�Z�[�W)/ -1=ADS�Ȃ�
//---------------------------------------------------------------------------
int delete_ADS(
	UnicodeString fnam,	//[i] �t�@�C����
	bool force, 		//�Ǎ���p�ł������I�ɕύX		(default = false)
	bool zi_only,		//Zone.Identifier �݂̂��폜	(default = false)
	TStringList *lst)	//[o] �폜�����X�g���[����		(default = NULL)
{
	int res = -1;

	//�Ǎ���p�Ȃ�ꎞ�I�ɉ���
	int atr = file_GetAttr(fnam);
	bool chg_ro = force && atr!=faInvalid && (atr & faReadOnly);
	if (chg_ro && !set_FileWritable(fnam)) chg_ro = false;

	WIN32_FIND_STREAM_DATA sd;
	HANDLE hFS = ::FindFirstStreamW(fnam.c_str(), FindStreamInfoStandard, &sd, 0);
	if (hFS!=INVALID_HANDLE_VALUE) {
		do {
			UnicodeString snam = get_tkn(sd.cStreamName, ":$DATA");
			remove_top_s(snam, ':');
			if (!snam.IsEmpty() && (!zi_only || USAME_TI(snam, "Zone.Identifier")) ) {
				if (lst) lst->Add(snam);
				res = ::DeleteFile(cv_ex_filename(fnam + ":" + snam).c_str())? 1 : 0;
				if (res==0) lst->Text = SysErrorMessage(GetLastError());
			}
		} while (::FindNextStreamW(hFS, &sd) && (res!=0));
		::FindClose(hFS);
	}

	//���������ɖ߂�
	if (chg_ro && atr!=-1) file_SetAttr(fnam, atr);

	return res;
}

//---------------------------------------------------------------------------
//�w��f�B���N�g�������݂��Ȃ��ꍇ�͍쐬
//---------------------------------------------------------------------------
UnicodeString chk_cre_dir(UnicodeString dnam)
{
	if (!dir_exists(dnam)) dnam = create_Dir(dnam)? IncludeTrailingPathDelimiter(dnam) : EmptyStr;
	return dnam;
}

//---------------------------------------------------------------------------
//��f�B���N�g�����H
//---------------------------------------------------------------------------
bool is_EmptyDir(UnicodeString dnam, 
	bool no_file)	//true = �t�@�C�����܂܂�Ă��Ȃ�(�T�C�Y=0) (default = false);
{
	bool e_flag = true;
	dnam = IncludeTrailingPathDelimiter(dnam);
	TSearchRec sr;
	if (FindFirst(cv_ex_filename(dnam + "*"), faAnyFile, sr)==0) {
		do {
			if (ContainsStr("..", sr.Name)) continue;
			if (no_file && (sr.Attr & faDirectory))
				e_flag = is_EmptyDir(dnam + sr.Name, no_file);
			else
				e_flag = false;
		} while(FindNext(sr)==0 && e_flag);
		FindClose(sr);
	}
	return e_flag;
}

//---------------------------------------------------------------------------
//���p�\�ȃh���C�u�̃��X�g���擾
//---------------------------------------------------------------------------
int get_available_drive_list(TStringList *lst)
{
	lst->Clear();
	DWORD d_bit = ::GetLogicalDrives();	//���p�\�ȃh���C�u���r�b�g�}�X�N�`���Ŏ擾
	DWORD d_flag = 0x00000001;
	UINT old_mode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
	for (int d_n=_T('A'); d_n <= _T('Z'); d_n++,d_flag<<=1) {
		if ((d_bit & d_flag)==0) continue;
		UnicodeString drvnam; drvnam.sprintf(_T("%c:\\"), d_n);
		if (is_drive_accessible(drvnam)) lst->Add(drvnam);
	}
	::SetErrorMode(old_mode);
	return lst->Count;
}

//---------------------------------------------------------------------------
//�t�@�C���̑�����������擾
//---------------------------------------------------------------------------
UnicodeString get_file_attr_str(int atr)
{
	UnicodeString atr_str = StringOfChar(_T('_'), 5);
	if (atr!=faInvalid) {
		if (atr & faReadOnly)	atr_str[1] = 'R';
		if (atr & faHidden)		atr_str[2] = 'H';
		if (atr & faSysFile)	atr_str[3] = 'S';
		if (atr & faArchive)	atr_str[4] = 'A';
		if (atr & faCompressed)	atr_str[5] = 'C';
	}
	return atr_str;
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̃}�X�N�ɊY������S�t�@�C�����擾
//---------------------------------------------------------------------------
void get_files(
	UnicodeString pnam,	//�f�B���N�g����
	const _TCHAR *mask,	//�}�X�N
	TStrings *lst,		//���ʊi�[���X�g
	bool subSW)			//�T�u�f�B���N�g����	 (default = false)
{
	if (!dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);

	UnicodeString mask_u = def_if_empty(mask, _T("*.*"));

	//�T�u�f�B���N�g��������
	TSearchRec sr;
	if (subSW) {
		if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				if (ContainsStr("..", sr.Name)) continue;
				get_all_files_ex(pnam + sr.Name, mask_u, lst, subSW);
			} while(FindNext(sr)==0);
			FindClose(sr);
		}
	}

	//�t�@�C��������
	if (FindFirst(cv_ex_filename(pnam + mask_u), faAnyFile, sr)==0) {
		do {
			if (sr.Attr & faDirectory) continue;
			lst->Add(pnam + sr.Name);
		} while(FindNext(sr)==0);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̃}�X�N�ɊY������S�t�@�C���̃��X�g��ǉ��擾
//  �߂�l: �t�@�C����(��f�B���N�g���͏���)
//---------------------------------------------------------------------------
int get_all_files_ex(
	UnicodeString pnam,		//�f�B���N�g����
	UnicodeString mask,		//�}�X�N
	TStrings *lst,			//���ʂ��i�[���� TStrings
	bool sub_sw,			//�T�u�f�B���N�g��������			(default = fals)
	int sub_n,				//�T�u�f�B���N�g���̊K�w�̐[��		(default = 99)
	UnicodeString skip_dir,	//���ODir�}�X�N(;�ŋ�؂蕡����)	(default = EmptyStr)
	bool inc_hide,			//�B��������						(default = true)
	bool inc_sys)			//�V�X�e��������					(default = true)
{
	int fcnt = 0;
	if (!dir_exists(pnam)) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	//�T�u�f�B���N�g��������
	TSearchRec sr;
	if (sub_sw && sub_n>0) {
		TStringDynArray skip_dir_lst = split_strings_semicolon(skip_dir);
		if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				if (!inc_hide && (sr.Attr & faHidden))  continue;
				if (!inc_sys  && (sr.Attr & faSysFile)) continue;
		
				UnicodeString dnam = sr.Name;
				if (!ContainsStr("..", dnam)) {
					//���O�f�B���N�g���̃`�F�b�N
					bool skip = false;
					for (int i=0; i<skip_dir_lst.Length; i++) {
						UnicodeString snam = skip_dir_lst[i];
						if (str_match(skip_dir_lst[i], dnam)) { skip = true; break; }
					}

					if (!skip) {
						int scnt = get_all_files_ex(pnam + dnam, mask, lst, sub_sw, sub_n - 1, skip_dir,
													inc_hide, inc_sys);
						fcnt += scnt;
					}
				}
			} while(FindNext(sr)==0);
			FindClose(sr);
		}
	}

	//�t�@�C��������
	if (FindFirst(cv_ex_filename(pnam + mask), faAnyFile, sr)==0) {
		do {
			if (sr.Attr & faDirectory) continue;
			if (!inc_hide && (sr.Attr & faHidden))  continue;
			if (!inc_sys  && (sr.Attr & faSysFile)) continue;
			lst->Add(pnam + sr.Name);
			fcnt++;
		} while(FindNext(sr)==0);
		FindClose(sr);
	}

	return fcnt;
}

//---------------------------------------------------------------------------
//��̃t�@�C�����쐬
//---------------------------------------------------------------------------
bool create_EmptyFile(UnicodeString fnam)
{
	HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
		GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile==INVALID_HANDLE_VALUE) return false;
	::CloseHandle(hFile);
	return true;
}

//---------------------------------------------------------------------------
//�f�B���N�g���̍쐬
//---------------------------------------------------------------------------
bool create_Dir(UnicodeString dnam)
{
	if (dnam.Length()>=248) {
		if (StartsStr("\\\\", dnam)) dnam.Insert("?\\UNC\\", 3); else dnam.Insert("\\\\?\\", 1);
	}
	return ::CreateDirectory(dnam.c_str(), NULL);
}
//---------------------------------------------------------------------------
//�f�B���N�g���̍쐬(�����K�w��)
//---------------------------------------------------------------------------
bool create_ForceDirs(UnicodeString dnam)
{
	TStringDynArray plst = split_path(dnam);
	UnicodeString rnam = get_root_name(dnam);
	UnicodeString pnam;
	for (int i=0; i<plst.Length; i++) {
		pnam += IncludeTrailingPathDelimiter(plst[i]);
		if (USAME_TS(pnam, "\\") || USAME_TS(pnam, "\\\\") || SameStr(pnam, rnam)) continue;
		if (dir_exists(pnam)) continue;
		if (!create_Dir(pnam)) return false;
	}
	return true;
}

//---------------------------------------------------------------------------
//�f�B���N�g���̍폜
//---------------------------------------------------------------------------
bool delete_Dir(UnicodeString dnam)
{
	return ::RemoveDirectory(cv_ex_filename(dnam).c_str());
}
//---------------------------------------------------------------------------
//�T�u�f�B���N�g�����܂߂��f�B���N�g���̍폜
//---------------------------------------------------------------------------
bool delete_Dirs(UnicodeString pnam)
{
	pnam = to_path_name(pnam);
	if (!dir_exists(pnam)) return false;

	//�T�u�f�B���N�g�����������č폜
	TSearchRec sr;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		bool ok = true;
		do {
			if ((sr.Attr & faDirectory)==0 || ContainsStr("..", sr.Name)) continue;
			ok = delete_Dirs(pnam + sr.Name);
		} while(FindNext(sr)==0 && ok);
		FindClose(sr);
	}

	//�f�B���N�g�����폜
	if (!set_FileWritable(pnam)) return false;
	return delete_Dir(pnam);
}

//---------------------------------------------------------------------------
//�t�@�C���̈ړ�
//---------------------------------------------------------------------------
bool move_File(UnicodeString old_nam, UnicodeString new_nam)
{
	return ::MoveFileEx(cv_ex_filename(old_nam).c_str(), cv_ex_filename(new_nam).c_str(),
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH);
}
//---------------------------------------------------------------------------
//�t�@�C���̃R�s�[
//---------------------------------------------------------------------------
bool copy_File(UnicodeString src_nam, UnicodeString dst_nam)
{
	return ::CopyFile(cv_ex_filename(src_nam).c_str(), cv_ex_filename(dst_nam).c_str(), FALSE);
}

//---------------------------------------------------------------------------
//�t�@�C�����O�̕ύX
//---------------------------------------------------------------------------
bool rename_File(UnicodeString old_nam, UnicodeString new_nam)
{
	return ::MoveFile(cv_ex_filename(old_nam).c_str(), cv_ex_filename(new_nam).c_str());
}

//---------------------------------------------------------------------------
//�p�X���̕ύX
//---------------------------------------------------------------------------
bool rename_Path(UnicodeString old_nam, UnicodeString new_nam)
{
	if (!SameText(get_root_name(old_nam),  get_root_name(new_nam))) return false;

	TStringDynArray o_plst = split_path(old_nam);
	TStringDynArray n_plst = split_path(new_nam);
	if (o_plst.Length!=n_plst.Length) return false;

	UnicodeString cur_dir = GetCurrentDir();

	bool ok = true;
	UnicodeString o_pnam, n_pnam;
	for (int i=0; i<o_plst.Length && ok; i++) {
		o_pnam += IncludeTrailingPathDelimiter(o_plst[i]);
		n_pnam += IncludeTrailingPathDelimiter(n_plst[i]);
		if (USAME_TS(o_pnam, "\\") || USAME_TS(o_pnam, "\\\\") || USAME_TS(n_pnam, "\\") || USAME_TS(n_pnam, "\\\\")) continue;
		if (!SameText(o_plst[i], n_plst[i])) {
			SetCurrentDir(get_parent_path(o_pnam));
			ok = rename_File(o_pnam, n_pnam);
		}
		o_pnam = n_pnam;
	}

	SetCurrentDir(cur_dir);

	return ok;
}

//---------------------------------------------------------------------------
//�p�[�X�|�C���g�̎Q�Ɛ���擾
//---------------------------------------------------------------------------
UnicodeString get_ReparsePointTarget(
	UnicodeString pnam,
	bool &is_jct)		//[o] true = �W�����N�V����/ false = �V���{���b�N�����N
{
	is_jct = false;
	HANDLE hDir = ::CreateFile(pnam.c_str(), GENERIC_READ,
						FILE_SHARE_READ, NULL, OPEN_EXISTING,
						FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hDir==INVALID_HANDLE_VALUE) return EmptyStr;

	std::unique_ptr<BYTE[]> dat_buf(new BYTE[MAXIMUM_REPARSE_DATA_BUFFER_SIZE]);
	::ZeroMemory(dat_buf.get(), MAXIMUM_REPARSE_DATA_BUFFER_SIZE);

	UnicodeString rnam;
	DWORD dat_sz;
	if (::DeviceIoControl(hDir, FSCTL_GET_REPARSE_POINT,
		NULL, 0, dat_buf.get(), MAXIMUM_REPARSE_DATA_BUFFER_SIZE, &dat_sz, NULL))
	{
		REPARSE_DATA_BUFFER *rp_buf = (_REPARSE_DATA_BUFFER*)dat_buf.get();
		if (IsReparseTagMicrosoft(rp_buf->ReparseTag)) {
			unsigned int tag = rp_buf->ReparseTag;
			switch (tag) {
			case IO_REPARSE_TAG_MOUNT_POINT:	//�W�����N�V����
				rnam = (WCHAR*)rp_buf->MountPointReparseBuffer.PathBuffer
						+ rp_buf->MountPointReparseBuffer.SubstituteNameOffset/sizeof(WCHAR);
				is_jct = true;
				break;
			case IO_REPARSE_TAG_SYMLINK:		//�V���{���b�N�����N
				rnam = (WCHAR*)rp_buf->SymbolicLinkReparseBuffer.PathBuffer
						+ rp_buf->SymbolicLinkReparseBuffer.SubstituteNameOffset/sizeof(WCHAR);
				break;
			}
			remove_top_s(rnam, _T("\\??\\"));
		}
	}

	::CloseHandle(hDir);

	return rnam;
}

//---------------------------------------------------------------------------
//�W�����N�V�����̍쐬
//---------------------------------------------------------------------------
bool cre_Junction(UnicodeString pnam, UnicodeString target)
{
	if (!::CreateDirectory(pnam.c_str(), NULL)) return false;

	int buf_size = MAX_PATH*sizeof(WCHAR) + sizeof(REPARSE_DATA_BUFFER);
	std::unique_ptr<BYTE[]> dat_buf(new BYTE[buf_size]);
	::ZeroMemory(dat_buf.get(), buf_size);
	REPARSE_DATA_BUFFER* reparse = (REPARSE_DATA_BUFFER*)dat_buf.get();

	target.Insert("\\??\\", 1);
	int path_len = target.Length() + 1;
	reparse->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
	reparse->ReparseDataLength = (path_len + 2) * sizeof(WCHAR) + 6;
	reparse->MountPointReparseBuffer.SubstituteNameLength = (path_len-1) * sizeof(WCHAR);
	wcscpy((wchar_t *)reparse->MountPointReparseBuffer.PathBuffer, target.c_str());
	reparse->MountPointReparseBuffer.PrintNameOffset = path_len * sizeof(WCHAR);
	int data_len = reparse->ReparseDataLength + 8;

	HANDLE hDir = ::CreateFile(pnam.c_str(), GENERIC_WRITE,
						0, NULL, OPEN_EXISTING,
						FILE_FLAG_OPEN_REPARSE_POINT|FILE_FLAG_BACKUP_SEMANTICS, NULL);
	if (hDir==INVALID_HANDLE_VALUE) return false;

	DWORD size;
	BOOL res = ::DeviceIoControl(hDir, FSCTL_SET_REPARSE_POINT, dat_buf.get(), data_len, NULL, 0, &size, NULL);
	::CloseHandle(hDir);
	if (res) return true;

	::RemoveDirectory(pnam.c_str());
	return false;
}

//---------------------------------------------------------------------------
//�t�@�C���X�g���[������̓ǂݍ���
//�I�G���[���� EReadError ��O���o
//---------------------------------------------------------------------------
BYTE fsRead_byte(TFileStream *fs)
{
	BYTE xbuf;
	fs->ReadBuffer(&xbuf, 1);
	return xbuf;
}
//---------------------------------------------------------------------------
int fsRead_int2(TFileStream *fs,
	bool be_sw)		//true = �r�b�O�G���f�B�A�� (default = false)
{
	BYTE xbuf[2];
	fs->ReadBuffer(xbuf, 2);
	return (be_sw? ((xbuf[0] << 8) | xbuf[1]) : ((xbuf[1] << 8) | xbuf[0]));
}
//---------------------------------------------------------------------------
int fsRead_int3(TFileStream *fs,
	bool be_sw)		//true = �r�b�O�G���f�B�A�� (default = false)
{
	BYTE xbuf[3];
	fs->ReadBuffer(xbuf, 3);
	return (be_sw?
			((xbuf[0] << 16) | (xbuf[1] << 8) | xbuf[2]) :
			(xbuf[0] | (xbuf[1] << 8) | (xbuf[2] << 16)));
}
//---------------------------------------------------------------------------
int fsRead_int4(TFileStream *fs,
	bool be_sw)		//true = �r�b�O�G���f�B�A�� (default = false)
{
	BYTE xbuf[4];
	fs->ReadBuffer(xbuf, 4);
	return (be_sw?
			((xbuf[0] << 24) | (xbuf[1] << 16) | (xbuf[2] << 8) | xbuf[3]) :
			(xbuf[0] | (xbuf[1] << 8) | (xbuf[2] << 16) | (xbuf[3] << 24)));
}
//---------------------------------------------------------------------------
double fsRead_double(TFileStream *fs)
{
	double r;
	fs->ReadBuffer(&r, 8);
	return r;
}

//---------------------------------------------------------------------------
//�t�@�C���X�g���[������̕�����ǂݍ���
//�I�r���� \0 ������΂���ȍ~�͐؂�̂ĂĕԂ�
//---------------------------------------------------------------------------
UnicodeString fsRead_char(TFileStream *fs, int size)
{
	std::unique_ptr<BYTE[]> buf(new BYTE[size + 1]);
	if (fs->Read(buf.get(), size)!=size) return EmptyStr;
	buf[size] = 0;
	return UnicodeString((char*)buf.get());
}

//---------------------------------------------------------------------------
//�R�����g�̓ǂݍ���
//  length  : 32bit
//  comment : UTF-8[length]
//�I�G���[���� EReadError ��O���o
//---------------------------------------------------------------------------
UnicodeString fsRead_comment_utf8(TFileStream *fs)
{
	int s_len;
	fs->ReadBuffer(&s_len, 4);
	if (s_len==0) return EmptyStr;

	TBytes Bytes;
	Bytes.Length = s_len;
	fs->ReadBuffer(Bytes, s_len);
	return TEncoding::UTF8->GetString(Bytes, 0, Bytes.Length);
}

//---------------------------------------------------------------------------
// �t�@�C���̌��݈ʒu�Ɏw�蕶���񂪂��邩�H
// ����Β���ʒu�ցA������Ό��݈ʒu�̂܂�
//---------------------------------------------------------------------------
bool fsRead_check_char(TFileStream *fs, const char *s)
{
	int size = strlen(s);
	if (size==0) return false;

	int p0 = fs->Seek(0, soFromCurrent);
	try {
		std::unique_ptr<BYTE[]> buf(new BYTE[size]);
		fs->ReadBuffer(buf.get(), size);
		for (int i=0; i<size; i++) if (s[i]!=buf[i]) Abort();
		return true;
	}
	catch (...) {
		fs->Seek(p0, soFromBeginning);
		return false;
	}
}
//---------------------------------------------------------------------------
