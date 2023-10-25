//----------------------------------------------------------------------//
// ファイル操作 (260文字以上のパスに対応)								//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_file_ex.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UnicodeString ExePath;		//起動パス

//---------------------------------------------------------------------------
//ファイル名主部によるソート用比較関数
//---------------------------------------------------------------------------
int __fastcall FileComp_Base(TStringList *List, int Index1, int Index2)
{
	UnicodeString bnam0 = get_base_name(List->Strings[Index1]);
	UnicodeString bnam1 = get_base_name(List->Strings[Index2]);
	return SameText(bnam0, bnam1)?
		StrCmpLogicalW(ExtractFileDir(List->Strings[Index1]).c_str(), ExtractFileDir(List->Strings[Index2]).c_str()) :
		StrCmpLogicalW(bnam0.c_str(), bnam1.c_str());
}

//---------------------------------------------------------------------------
//260文字以上のパス名に対応するためのファイル名変換
//  末尾が空白の場合にも対応
//---------------------------------------------------------------------------
UnicodeString cv_ex_filename(UnicodeString fnam)
{
	if (fnam.Length()>=MAX_PATH || EndsStr(' ', fnam)) {
		if (StartsStr("\\\\", fnam)) fnam.Insert("?\\UNC\\", 3); else fnam.Insert("\\\\?\\", 1);
	}
	return fnam;
}

//---------------------------------------------------------------------------
//環境変数を変換
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
//環境変数を含む文字列を変換
//---------------------------------------------------------------------------
UnicodeString cv_env_str(UnicodeString s)
{
	s = ReplaceText(s, "%ExePath%", ExePath);
	s = cv_env_var(s);
	return s;
}

//---------------------------------------------------------------------------
//環境変数や書式文字列を含む文字列から実際のパス名を取得
//---------------------------------------------------------------------------
UnicodeString get_actual_path(UnicodeString pnam)
{
	pnam = cv_env_str(pnam);

	if (ContainsStr(pnam, "$")) {
		pnam = ReplaceStr(pnam, "$$", "\f");	//$$ を一時的に \f に変えておいて
		pnam = ReplaceStr(pnam, "$X", ExcludeTrailingPathDelimiter(ExePath));
		pnam = ReplaceStr(pnam, "$D", ExtractFileDrive(ExePath));
		pnam = ReplaceStr(pnam, "\f", "$");		//$$(\f) を $ に変換
	}
	return pnam;
}

//---------------------------------------------------------------------------
//パスが通っている場合、パス無しファイル名をパス付きに変換
//---------------------------------------------------------------------------
UnicodeString get_actual_name(
	UnicodeString fnam,
	bool sw_fmt)			//書式文字列を展開	(default = false)
{
	fnam = sw_fmt? get_actual_path(fnam) : cv_env_str(fnam);

	if (!fnam.IsEmpty() && fnam.Pos('\\')==0) {
		//PATH
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
			//シンボリックリンクなら App Paths をチェック
			if (!rnam.IsEmpty() && is_SymLink(rnam)) {
				rnam = EmptyStr;
				std::unique_ptr<TRegistry> reg(new TRegistry());
				reg->RootKey = HKEY_CURRENT_USER;
				UnicodeString key = "Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\" + fnam;
				if (chk_ext) key += ".exe";
				if (reg->OpenKeyReadOnly(key)) {
					rnam = reg->ReadString(EmptyStr);
					reg->CloseKey();
				}
			}
		}

		if (!rnam.IsEmpty()) fnam = rnam;
	}

	return fnam;
}

//---------------------------------------------------------------------------
//パスが通っている場合、ファイル名からパスと拡張子を取り除く
//---------------------------------------------------------------------------
UnicodeString exclude_env_path(UnicodeString fnam)
{
	if (fnam.IsEmpty() || ExtractFileDrive(fnam).IsEmpty()) return fnam;

	UnicodeString fext = get_extension(fnam);
	if (fext.IsEmpty()) return fnam;

	//拡張子をチェック
	TStringDynArray xlst = split_strings_semicolon(GetEnvironmentVariable("PATHEXT"));
	bool ok = false;
	for (int i=0; i<xlst.Length && !ok; i++) ok = SameText(fext, xlst[i]);

	//パスをチェック
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
//実行ファイルのディレクトリからの相対ファイル名に変換
//---------------------------------------------------------------------------
UnicodeString to_relative_name(UnicodeString fnam)
{
	remove_top_text(fnam, ExePath);
	return fnam;
}

//---------------------------------------------------------------------------
//絶対パスに変換 (ドライブ名を含む場合は正規化)
//---------------------------------------------------------------------------
UnicodeString to_absolute_name(
	UnicodeString fnam,		//変換対象ファイル名
	UnicodeString rnam)		//基準ディレクトリ名 (default = EmptyStr/ ExePath);
{
	if (fnam.IsEmpty()) return EmptyStr;

	//.. を含むパスを正規化
	if (!ExtractFileDrive(fnam).IsEmpty()) {
		UnicodeString ptn = "\\\\[^\\\\\\.]+\\\\\\.\\.\\\\";
		while (TRegEx::IsMatch(fnam, ptn)) fnam = TRegEx::Replace(fnam, ptn, "\\");
		return fnam;
	}

	//相対パスから絶対パスへ
	if (rnam.IsEmpty()) rnam = ExePath;
	for (;;) {
		if (remove_top_text(fnam, ".\\")) continue;
		if (!remove_top_text(fnam, "..\\")) break;
		rnam = get_parent_path(rnam);
	}

	return contains_PathDlmtr(rnam)? rnam + fnam : EmptyStr;
}

//---------------------------------------------------------------------------
//代替データストリームを考慮したパス名の取得
//---------------------------------------------------------------------------
UnicodeString extract_file_path(UnicodeString fnam)
{
	int p = pos_ADS_delimiter(fnam);
	if (p>0) fnam = fnam.SubString(1, p - 1);
	return ExtractFilePath(fnam);
}

//---------------------------------------------------------------------------
//パスリストにマッチするか?
//---------------------------------------------------------------------------
bool match_path_list(
	UnicodeString dnam,		//チェック対象
	UnicodeString plist,	//パスリスト(環境変数、%ExePath% 対応)
	bool start_sw)			//true : 前方一致	(default = false : 部分一致)
{
	bool ret = false;
	TStringDynArray path_lst = split_strings_semicolon(plist);
	for (int i=0; i<path_lst.Length && !ret; i++) {
		UnicodeString pnam = cv_env_str(path_lst[i]);
		if (pnam.IsEmpty()) continue;
		ret = (start_sw? StartsText(pnam, dnam) : ContainsText(dnam, pnam));
	}
	return ret;
}

//---------------------------------------------------------------------------
//同一ファイルか?
//---------------------------------------------------------------------------
bool is_same_file(UnicodeString fnam1, UnicodeString fnam2,
	UnicodeString rnam)		//基準ファイル名 (default = EmptyStr/ ExePath);
{
	fnam1 = to_absolute_name(exclude_quot(fnam1), rnam);
	fnam2 = to_absolute_name(exclude_quot(fnam2), rnam);
	return SameText(fnam1, fnam2);
}

//---------------------------------------------------------------------------
//ルート名(ドライブ名またはUNCのコンピュータ名)を取得
//---------------------------------------------------------------------------
UnicodeString get_root_name(UnicodeString pnam)
{
	UnicodeString rstr = pnam;
	if (remove_top_text(rstr, "\\\\"))
		rstr = "\\\\" + get_tkn(rstr, '\\');
	else
		rstr = ExtractFileDrive(rstr);
	return IncludeTrailingPathDelimiter(rstr);
}

//---------------------------------------------------------------------------
//実際の大小文字でファイル名を取得
//---------------------------------------------------------------------------
UnicodeString get_case_name(UnicodeString fnam)
{
	UnicodeString drv_nam = IncludeTrailingPathDelimiter(ExtractFileDrive(fnam));
	UnicodeString ret_str = drv_nam;
	UnicodeString pnam;
	TStringDynArray plst = split_path(fnam);
	for (int i=0; i<plst.Length-1; i++) {
		pnam += plst[i] + "\\";
		if (SameStr(pnam, "\\") || SameStr(pnam, "\\\\")) continue;
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
			//見つからなかったらそのまま返す
			ret_str = fnam;
			break;
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//ルートディレクトリか?
//---------------------------------------------------------------------------
bool is_root_dir(UnicodeString dnam)
{
	if (remove_top_s(dnam, "\\\\"))
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(get_tkn_r(dnam, '\\')));
	else
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(dnam));
}
//---------------------------------------------------------------------------
bool is_root_unc(UnicodeString dnam)
{
	if (remove_top_s(dnam, "\\\\"))
		return !contains_PathDlmtr(ExcludeTrailingPathDelimiter(get_tkn_r(dnam, '\\')));
	else
		return false;
}

//---------------------------------------------------------------------------
//ディレクトリがルートの場合、末尾の \ を取り除く
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
//ドライブ名(大文字、\付き)を取得
//---------------------------------------------------------------------------
UnicodeString get_drive_str(UnicodeString dnam)
{
	return IncludeTrailingPathDelimiter(ExtractFileDrive(dnam)).UpperCase();
}

//---------------------------------------------------------------------------
//ドライブが有効か?
//---------------------------------------------------------------------------
bool drive_exists(UnicodeString drv)
{
	drv = get_tkn(drv, ':');	if (drv.IsEmpty())	 return false;
	int d_n = drv[1] - _T('A');	if (d_n<0 || d_n>25) return false;
	DWORD d_flag = 0x00000001 << d_n;
	DWORD d_bit = ::GetLogicalDrives();	//利用可能なドライブをビットマスク形式で取得
	return (d_bit & d_flag);
}

//---------------------------------------------------------------------------
//ドライブの種類を取得
//---------------------------------------------------------------------------
UINT get_drive_type(UnicodeString dnam)
{
	return ::GetDriveType(get_drive_str(dnam).c_str());
}

//---------------------------------------------------------------------------
//クラスタサイズを取得
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
//ファイルのベース名を取得
//---------------------------------------------------------------------------
UnicodeString get_base_name(UnicodeString fnam)
{
	fnam = ExtractFileName(fnam);
	if (fnam.IsEmpty()) return EmptyStr;
	return StartsStr('.', fnam)? "." + ChangeFileExt(exclude_top(fnam), EmptyStr) : ChangeFileExt(fnam, EmptyStr);
}

//---------------------------------------------------------------------------
//拡張子を取得 (ドットファイルを考慮)
//---------------------------------------------------------------------------
UnicodeString get_extension(UnicodeString fnam)
{
	fnam = ExtractFileName(fnam);
	if (fnam.IsEmpty()) return EmptyStr;
	return ExtractFileExt(StartsStr('.', fnam)? exclude_top(fnam) : fnam);
}
//---------------------------------------------------------------------------
//ファイルなら拡張子を取得
//---------------------------------------------------------------------------
UnicodeString get_extension_if_file(UnicodeString fnam)
{
	return dir_exists(fnam)? EmptyStr : get_extension(fnam);
}

//---------------------------------------------------------------------------
//拡張子のチェック
//---------------------------------------------------------------------------
UnicodeString nrm_FileExt(UnicodeString fext)
{
	if (fext.IsEmpty()) return EmptyStr;
	if (!StartsStr('.', fext)) fext.Insert(".", 1);
	if (!EndsStr('.', fext))   fext += ".";
	return fext;
}
//---------------------------------------------------------------------------
bool test_FileExt(UnicodeString fext, UnicodeString list)
{
	if (list.IsEmpty() || fext.IsEmpty() || SameStr(fext, ".")) return false;
	if (SameStr(list, "*") || SameStr(list, ".*")) return true;
	return ContainsText(nrm_FileExt(list), nrm_FileExt(fext));
}

//---------------------------------------------------------------------------
//拡張子のチェック(サイズ制限対応)
//---------------------------------------------------------------------------
bool test_FileExtSize(UnicodeString fext, UnicodeString list, __int64 size)
{
	bool ret = false;
	TStringDynArray xlst = SplitString(list, ".");
	UnicodeString xstr;
	for (int i=0; i<xlst.Length; i++) {
		if (test_FileExt(fext, get_tkn(xlst[i], ':'))) xstr =xlst[i];
	}
	if (!xstr.IsEmpty()) {
		__int64 sz = get_tkn_r(xstr, ':').ToIntDef(0) * 1048576ul;	//MB
		ret = (size>=sz);
	}

	return ret;
}

//---------------------------------------------------------------------------
//ディレクトリ名の末尾に \\ を (EmptyStr の場合は EmptyStr を返す)
//---------------------------------------------------------------------------
UnicodeString to_path_name(UnicodeString dnam)
{
	return (!dnam.IsEmpty()? IncludeTrailingPathDelimiter(dnam) : EmptyStr);
}

//---------------------------------------------------------------------------
//ディレクトリ名(末尾の \\ 無し)を取得
//---------------------------------------------------------------------------
UnicodeString get_dir_name(UnicodeString dnam)
{
	return ExtractFileName(ExcludeTrailingPathDelimiter(dnam));
}

//---------------------------------------------------------------------------
//親ディレクトリを取得
//---------------------------------------------------------------------------
UnicodeString get_parent_path(UnicodeString dnam)
{
	return ExtractFilePath(ExcludeTrailingPathDelimiter(dnam));
}

//---------------------------------------------------------------------------
//適切なFTPパスに変換
//---------------------------------------------------------------------------
UnicodeString nrm_ftp_path(UnicodeString pnam)
{
	if		(pnam.IsEmpty())		pnam = "/";
	else if (!StartsStr('/', pnam)) pnam.Insert("/", 1);
	return pnam;
}

//---------------------------------------------------------------------------
//パスを分割
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
//"UNCパス:ユーザ名" からユーザ名を分離
//---------------------------------------------------------------------------
UnicodeString split_user_name(UnicodeString &dnam)
{
	UnicodeString unam;
	if (StartsStr("\\\\", dnam) && dnam.Pos(":")>3) {
		unam = get_tkn_r(dnam, ':');
		dnam = get_tkn(dnam, ':');
	}
	return unam;
}

//---------------------------------------------------------------------------
//コンピュータ名か?
//---------------------------------------------------------------------------
bool is_computer_name(UnicodeString pnam)
{
	if (!remove_top_s(pnam, "\\\\")) return false;
	return (ExcludeTrailingPathDelimiter(pnam).Pos('\\')==0);
}

//---------------------------------------------------------------------------
//ディレクトリがアクセス可能か？
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
//ドライブがアクセス可能か？
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
//ドライブがライトプロテクトされているか？
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
//NTFS圧縮属性の設定
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
//ファイルの属性を取得
//---------------------------------------------------------------------------
int file_GetAttr(UnicodeString fnam)
{
	if (fnam.Length()>=MAX_PATH || EndsStr(' ', fnam)) {
		if (StartsStr("\\\\", fnam)) fnam.Insert("?\\UNC\\", 3); else fnam.Insert("\\\\?\\", 1);
	}
	return ::GetFileAttributes(fnam.c_str());
}

//---------------------------------------------------------------------------
//ファイルの属性を設定
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
//ディレクトリの属性をコピー
//戻り値: false = 失敗
//dst_nam は無属性とみなす(作成直後の状態)
//---------------------------------------------------------------------------
bool dir_CopyAttr(UnicodeString src_nam, UnicodeString dst_nam,
	bool remove_ro)	//読込専用属性を解除
{
	int attr = file_GetAttr(src_nam);	if (attr==faInvalid) return false;
	if (remove_ro) attr &= ~faReadOnly;
	if ((attr & (faReadOnly|faHidden|faSysFile)) == 0) return true;
	return file_SetAttr(dst_nam, attr);
}

//---------------------------------------------------------------------------
//ファイルの読み出し専用属性を解除
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
//リパースポイント?
//---------------------------------------------------------------------------
bool is_SymLink(UnicodeString fnam)
{
	fnam = cv_ex_filename(ExcludeTrailingPathDelimiter(fnam));
	DWORD attr = ::GetFileAttributes(fnam.c_str());
	return (attr!=0xffffffff && (attr & FILE_ATTRIBUTE_REPARSE_POINT));
}

//---------------------------------------------------------------------------
//リンクカウントを取得(2以上ならハードリンク)
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
//ファイルの同一性チェック
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
//ファイルの存在チェック
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
bool file_exists_ico(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return false;

	int ico_idx = get_tkn_r(fnam, ',').ToIntDef(-1);
	if (ico_idx!=-1) fnam = get_tkn(fnam, ",");

	return (file_GetAttr(fnam) != faInvalid);
}

//---------------------------------------------------------------------------
//ワイルドカードに対応した存在チェック
//---------------------------------------------------------------------------
bool file_exists_wc(
	UnicodeString &fnam)	//ファイル名(ワイルドカード可)
							//最初にマッチしたものを返す、なければ EmptyStr
{
	UnicodeString mask = ExtractFileName(fnam);
	if (mask.Pos('*') || mask.Pos('?')) {
		std::unique_ptr<TStringList> lst(new TStringList());
		get_files(ExtractFilePath(fnam), mask, lst.get());
		fnam = (lst->Count>0)? lst->Strings[0] : EmptyStr;
	}
	return file_exists(fnam);
}

//---------------------------------------------------------------------------
//ディレクトリの存在チェック
//---------------------------------------------------------------------------
bool dir_exists(UnicodeString dnam)
{
	if (dnam.IsEmpty()) return false;
	return ::PathIsDirectory(cv_ex_filename(dnam).c_str());
}

//---------------------------------------------------------------------------
//世界協定時刻からローカルの TDateTime へ
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
//ファイルのタイムスタンプを TDateTime 型で取得
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
//タイムスタンプを設定
//---------------------------------------------------------------------------
bool set_file_age(UnicodeString fnam, TDateTime dt,
	bool force)		//読込専用でも強制的に変更 (default = false)
{
	bool res = false;

	//読込専用なら一時的に解除
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
		//システム日時 → ファイル時刻 → 世界協定時刻
		if (::SystemTimeToFileTime(&s_tm, &l_tm) && ::LocalFileTimeToFileTime(&l_tm, &f_tm)) {
			res = ::SetFileTime(hFile, NULL, NULL, &f_tm);
		}
		::CloseHandle(hFile);
	}

	//属性を元に戻す
	if (chg_ro) file_SetAttr(fnam, atr);

	return res;
}

//---------------------------------------------------------------------------
//ファイルサイズを取得
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
//圧縮サイズを取得
//---------------------------------------------------------------------------
__int64 get_comp_size(UnicodeString fnam)
{
	DWORD sz_hi;
	DWORD sz_lo = ::GetCompressedFileSize(cv_ex_filename(fnam).c_str(), &sz_hi);
	if (sz_lo==INVALID_FILE_SIZE && GetLastError()!=NO_ERROR) return -1;
	return ((__int64)sz_hi<<32) + sz_lo;
}

//---------------------------------------------------------------------------
//代替データストリームの区切り位置を取得
//---------------------------------------------------------------------------
int pos_ADS_delimiter(UnicodeString fnam)
{
	int p = fnam.Length() - ExtractFileName(fnam).Length();
	return (p>2 && fnam[p]==':')? p : 0;
}
//---------------------------------------------------------------------------
//代替データストリーム付きパス名か?
//---------------------------------------------------------------------------
bool is_ADS_name(UnicodeString fnam)
{
	return (pos_ADS_delimiter(fnam)>0);
}
//---------------------------------------------------------------------------
//代替データストリーム付きパス名を分解
//戻り値: ファイル名/ fnam = :ストリーム名
//---------------------------------------------------------------------------
UnicodeString split_ADS_name(UnicodeString &fnam)
{
	UnicodeString ret_str;
	int p = pos_ADS_delimiter(fnam);
	if (p>0) {
		ret_str = fnam.SubString(1, p - 1);
		fnam.Delete(1, p - 1);
	}
	else {
		ret_str = fnam;
		fnam	= EmptyStr;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//代替データストリームの削除
//戻り値: 1=成功/ 0=失敗(lst->Text にエラーメッセージ)/ -1=ADSなし
//---------------------------------------------------------------------------
int delete_ADS(
	UnicodeString fnam,	//[i] ファイル名
	bool force, 		//読込専用でも強制的に変更		(default = false)
	UnicodeString mask,	//マスク						(default = "*")
	TStringList *lst)	//[o] 削除したストリーム名		(default = NULL)
{
	int res = -1;

	//読込専用なら一時的に解除
	int atr = file_GetAttr(fnam);
	bool chg_ro = force && atr!=faInvalid && (atr & faReadOnly);
	if (chg_ro && !set_FileWritable(fnam)) chg_ro = false;

	WIN32_FIND_STREAM_DATA sd;
	HANDLE hFS = ::FindFirstStreamW(fnam.c_str(), FindStreamInfoStandard, &sd, 0);
	if (hFS!=INVALID_HANDLE_VALUE) {
		do {
			UnicodeString snam = get_tkn(sd.cStreamName, ":$DATA");
			remove_top_s(snam, ':');
			if (!snam.IsEmpty() && str_match(mask, snam)) {
				if (lst) lst->Add(snam);
				res = ::DeleteFile(cv_ex_filename(fnam + ":" + snam).c_str())? 1 : 0;
				if (res==0) lst->Text = SysErrorMessage(GetLastError());
			}
		} while (::FindNextStreamW(hFS, &sd) && (res!=0));
		::FindClose(hFS);
	}

	//属性を元に戻す
	if (chg_ro && atr!=-1) file_SetAttr(fnam, atr);

	return res;
}
//---------------------------------------------------------------------------
//代替データストリーム名の変更
//---------------------------------------------------------------------------
bool rename_ADS(
	UnicodeString fnam,			//"元ファイル名:ストリーム名"
	UnicodeString new_name)		//":新ストリーム名"
{
	bool res = false;
	HMODULE hNtDll = ::GetModuleHandle(_T("ntdll.dll"));
	if (hNtDll) {
		FUNC_NtSetInformationFile NtSetInformationFile = (FUNC_NtSetInformationFile)::GetProcAddress(hNtDll, "NtSetInformationFile");
		if (NtSetInformationFile) {
			HANDLE hFile = ::CreateFile(fnam.c_str(),
				DELETE|SYNCHRONIZE|GENERIC_READ,
				FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
				NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile!=INVALID_HANDLE_VALUE) {
			    const size_t bufLen = sizeof(FILE_RENAME_INFORMATION) + sizeof(wchar_t) * new_name.Length();
				std::unique_ptr<BYTE[]> ri(new BYTE[bufLen]);
				PFILE_RENAME_INFORMATION p_ri = (PFILE_RENAME_INFORMATION) ri.get();
				p_ri->ReplaceIfExists = TRUE;
				p_ri->RootDirectory   = NULL;
				p_ri->FileNameLength  = sizeof(wchar_t) * new_name.Length();
				memcpy(p_ri->FileName, new_name.c_str(), sizeof(wchar_t) * new_name.Length());
				IO_STATUS_BLOCK iosb;
				res = (NtSetInformationFile(hFile, &iosb, p_ri, bufLen, FileRenameInformation)>=0);
				::CloseHandle(hFile);
			}
		}
	}
	return res;
}

//---------------------------------------------------------------------------
//指定ディレクトリが存在しない場合は作成
//---------------------------------------------------------------------------
UnicodeString chk_cre_dir(UnicodeString dnam)
{
	if (!dir_exists(dnam)) dnam = create_Dir(dnam)? IncludeTrailingPathDelimiter(dnam) : EmptyStr;
	return dnam;
}

//---------------------------------------------------------------------------
//空ディレクトリか？
//---------------------------------------------------------------------------
bool is_EmptyDir(UnicodeString dnam,
	bool no_file)	//true = ファイルが含まれていない(サイズ=0) (default = false);
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
//利用可能なドライブのリストを取得
//---------------------------------------------------------------------------
int get_available_drive_list(TStringList *lst)
{
	lst->Clear();
	DWORD d_bit = ::GetLogicalDrives();	//利用可能なドライブをビットマスク形式で取得
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
//ファイルの属性文字列を取得
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
//ディレクトリ下のマスクに該当する全ファイルを取得
//---------------------------------------------------------------------------
void get_files(
	UnicodeString pnam,	//ディレクトリ名
	UnicodeString mask,	//マスク
	TStrings *lst,		//結果格納リスト
	bool subSW)			//サブディレクトリも	 (default = false)
{
	if (!dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);

	UnicodeString mask_u = def_if_empty(mask, "*.*");

	//サブディレクトリを検索
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

	//ファイルを検索
	if (FindFirst(cv_ex_filename(pnam + mask_u), faAnyFile, sr)==0) {
		do {
			if (sr.Attr & faDirectory) continue;
			lst->Add(pnam + sr.Name);
		} while(FindNext(sr)==0);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//ディレクトリ下のマスクに該当する全ファイルのリストを追加取得
//  戻り値: ファイル数(空ディレクトリは除く)
//---------------------------------------------------------------------------
int get_all_files_ex(
	UnicodeString pnam,		//ディレクトリ名
	UnicodeString mask,		//マスク
	TStrings *lst,			//結果を格納する TStrings
	bool sub_sw,			//サブディレクトリも検索			(default = fals)
	int sub_n,				//サブディレクトリの階層の深さ		(default = 99)
	UnicodeString skip_dir,	//除外Dirマスク(;で区切り複数可)	(default = EmptyStr)
	bool inc_hide,			//隠し属性も						(default = true)
	bool inc_sys)			//システム属性も					(default = true)
{
	int fcnt = 0;
	if (!dir_exists(pnam)) return fcnt;

	pnam = IncludeTrailingPathDelimiter(pnam);
	if (mask.IsEmpty()) mask = "*.*";

	//サブディレクトリを検索
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
					//除外ディレクトリのチェック
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

	//ファイルを検索
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
//空のファイルを作成
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
//ディレクトリの作成
//---------------------------------------------------------------------------
bool create_Dir(UnicodeString dnam)
{
	if (dnam.Length()>=248) {
		if (StartsStr("\\\\", dnam)) dnam.Insert("?\\UNC\\", 3); else dnam.Insert("\\\\?\\", 1);
	}
	return ::CreateDirectory(dnam.c_str(), NULL);
}
//---------------------------------------------------------------------------
//ディレクトリの作成(複数階層可)
//---------------------------------------------------------------------------
bool create_ForceDirs(UnicodeString dnam)
{
	TStringDynArray plst = split_path(dnam);
	UnicodeString rnam = get_root_name(dnam);
	UnicodeString pnam;
	for (int i=0; i<plst.Length; i++) {
		pnam += IncludeTrailingPathDelimiter(plst[i]);
		if (SameStr(pnam, "\\") || SameStr(pnam, "\\\\") || SameStr(pnam, rnam)) continue;
		if (dir_exists(pnam)) continue;
		if (!create_Dir(pnam)) return false;
	}
	return true;
}

//---------------------------------------------------------------------------
//ディレクトリの削除
//---------------------------------------------------------------------------
bool delete_Dir(
	UnicodeString dnam,
	bool force)			//強制的に削除 (default = false)
{
	if (force && !set_FileWritable(dnam)) return false;
	return ::RemoveDirectory(cv_ex_filename(dnam).c_str());
}
//---------------------------------------------------------------------------
//サブディレクトリを含めたディレクトリの削除
//---------------------------------------------------------------------------
bool delete_Dirs(UnicodeString pnam)
{
	pnam = to_path_name(pnam);
	if (!dir_exists(pnam)) return false;

	//サブディレクトリを検索して削除
	TSearchRec sr;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		bool ok = true;
		do {
			if ((sr.Attr & faDirectory)==0 || ContainsStr("..", sr.Name)) continue;
			ok = delete_Dirs(pnam + sr.Name);
		} while(FindNext(sr)==0 && ok);
		FindClose(sr);
	}

	//ディレクトリを削除
	if (!set_FileWritable(pnam)) return false;
	return delete_Dir(pnam);
}

//---------------------------------------------------------------------------
//ファイルの移動
//---------------------------------------------------------------------------
bool move_File(UnicodeString old_nam, UnicodeString new_nam)
{
	return ::MoveFileEx(cv_ex_filename(old_nam).c_str(), cv_ex_filename(new_nam).c_str(),
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH);
}
//---------------------------------------------------------------------------
//ファイルのコピー
//---------------------------------------------------------------------------
bool copy_File(UnicodeString src_nam, UnicodeString dst_nam)
{
	return ::CopyFile(cv_ex_filename(src_nam).c_str(), cv_ex_filename(dst_nam).c_str(), FALSE);
}

//---------------------------------------------------------------------------
//ファイル名前の変更
//---------------------------------------------------------------------------
bool rename_File(UnicodeString old_nam, UnicodeString new_nam)
{
	return ::MoveFile(cv_ex_filename(old_nam).c_str(), cv_ex_filename(new_nam).c_str());
}

//---------------------------------------------------------------------------
//パス名の変更
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
		if (SameStr(o_pnam, "\\") || SameStr(o_pnam, "\\\\") || SameStr(n_pnam, "\\") || SameStr(n_pnam, "\\\\")) continue;
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
//ジャンクションの作成
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
//ファイルストリームからの読み込み
//！エラー時に EReadError 例外送出
//---------------------------------------------------------------------------
BYTE fsRead_byte(TFileStream *fs)
{
	BYTE xbuf;
	fs->ReadBuffer(&xbuf, 1);
	return xbuf;
}
//---------------------------------------------------------------------------
int fsRead_int2(TFileStream *fs,
	bool be_sw)		//true = ビッグエンディアン (default = false)
{
	BYTE xbuf[2];
	fs->ReadBuffer(xbuf, 2);
	return (be_sw? ((xbuf[0] << 8) | xbuf[1]) : ((xbuf[1] << 8) | xbuf[0]));
}
//---------------------------------------------------------------------------
int fsRead_int3(TFileStream *fs,
	bool be_sw)		//true = ビッグエンディアン (default = false)
{
	BYTE xbuf[3];
	fs->ReadBuffer(xbuf, 3);
	return (be_sw?
			((xbuf[0] << 16) | (xbuf[1] << 8) | xbuf[2]) :
			(xbuf[0] | (xbuf[1] << 8) | (xbuf[2] << 16)));
}
//---------------------------------------------------------------------------
int fsRead_int4(TFileStream *fs,
	bool be_sw)		//true = ビッグエンディアン (default = false)
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
//ファイルストリームからの文字列読み込み
//！途中に \0 があればそれ以降は切り捨てて返す
//---------------------------------------------------------------------------
UnicodeString fsRead_char(TFileStream *fs, int size)
{
	std::unique_ptr<BYTE[]> buf(new BYTE[size + 1]);
	if (fs->Read(buf.get(), size)!=size) return EmptyStr;
	buf[size] = 0;
	return UnicodeString((char*)buf.get());
}

//---------------------------------------------------------------------------
//コメントの読み込み
//  length  : 32bit
//  comment : UTF-8[length]
//！エラー時に EReadError 例外送出
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
// ファイルの現在位置に指定文字列があるか？
// あれば直後位置へ、無ければ現在位置のまま
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
