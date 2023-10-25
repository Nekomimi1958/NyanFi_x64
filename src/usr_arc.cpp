//----------------------------------------------------------------------//
// アーカイブ処理														//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_arc.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UserArcUnit::UserArcUnit(HWND hWnd)
{
	MainHandle = hWnd;

	fpAddDebugLog = NULL;

	Use7zDll   = false;

	hCurArc    = NULL;
	CurType    = 0;
	Busy	   = false;
	RarUnpacking = false;

	ZipPrm_x   = 5;
	SevenPrm_x = 5;
	ZipPrm_sfx = false;
	TarPrm_z   = 6;
	CabPrm_z   = 0;

	for (int i=0; i<MAX_ARC_DLL; i++) {
		arc_func *fp  = &ArcFunc[i];
		fp->Available = false;
		fp->FileName  = EmptyStr;
		fp->VerStr	  = EmptyStr;
		fp->Prefix	  = get_word_i_idx("SevenZip|Unlha|Cab|Tar|Unrar|UnIso", i);
		fp->dll7zName = EmptyStr;

#if defined(_WIN64)
		fp->DllName = get_word_i_idx("7-zip64.dll|unlha64.dll|cab64.dll|tar64.dll|unrar64j.dll|uniso64.dll", i);
		//unlha64.dll cab64.dll uniso64.dll は現存しない
#else
		fp->DllName = get_word_i_idx("7-zip32.dll|unlha32.dll|cab32.dll|tar32.dll|unrar32.dll|uniso32.dll", i);
#endif
		fp->hDll = ::LoadLibrary(fp->DllName.c_str());
		if (fp->hDll) {
			_TCHAR szFname[MAX_PATH];
			if (::GetModuleFileName(fp->hDll, szFname, MAX_PATH)>0) fp->FileName = szFname;

			fp->Command 	 = (FUNC_Arc)GetProcAdr(fp, EmptyStr);
			fp->GetVersion	 = (FUNC_ArcGetVersion)GetProcAdr(fp,	"GetVersion");
			fp->GetRunning	 = (FUNC_ArcGetRunning)GetProcAdr(fp,	"GetRunning");
			fp->CheckArchive = (FUNC_ArcCheckArchive)GetProcAdr(fp, "CheckArchive");
			fp->OpenArchive  = (FUNC_ArcOpenArchive)GetProcAdr(fp,	"OpenArchive");
			fp->CloseArchive = (FUNC_ArcCloseArchive)GetProcAdr(fp,	"CloseArchive");
			fp->FindFirst	 = (FUNC_ArcFindFirst)GetProcAdr(fp,	"FindFirst");
			fp->FindNext	 = (FUNC_ArcFindNext)GetProcAdr(fp,		"FindNext");
			fp->GetOriginalSizeEx = (FUNC_ArcGetOriginalSizeEx)GetProcAdr(fp,	"GetOriginalSizeEx");
			fp->SetUnicodeMode	  = (FUNC_ArcSetUnicodeMode)GetProcAdr(fp,		"SetUnicodeMode");
			fp->GetAttribute	  = NULL;
			fp->Exists7zdll 	  = NULL;
			fp->GetFileCount	  = NULL;
			fp->SetBackGroundMode = NULL;

			fp->IsUnicode = (fp->SetUnicodeMode!=NULL);
			fp->hasRename = false;
			fp->use7zdll  = false;
			fp->err7zdll  = false;

			fp->Available = fp->Command && fp->GetVersion && fp->GetRunning && fp->CheckArchive
					&& fp->OpenArchive && fp->CloseArchive && fp->FindFirst && fp->FindNext;

			//独自API
			switch (i) {
			case 0: case 5: //ZIP, ISO
				fp->GetAttribute = (FUNC_ArcGetAttribute)GetProcAdr(fp, "GetAttribute");
				if (fp->Available) fp->Available = fp->GetAttribute;
				if (fp->Available && fp->SetUnicodeMode) fp->Available = fp->SetUnicodeMode(TRUE);

				if (i==0) {
					fp->Exists7zdll = (FUNC_SevenZipExists7zdll)GetProcAdr(fp, "Exists7zdll");
					if (fp->Exists7zdll) {
						fp->use7zdll = true;
						Use7zDll	 = fp->Exists7zdll();
						fp->err7zdll = !Use7zDll;
						if (fp->Available && fp->use7zdll && Use7zDll) {
							//7z.dll のフルパス名を取得
							if (::GetModuleFileName(::GetModuleHandle(NULL), szFname, MAX_PATH)>0) {
								fp->CheckArchive(UTF8String(szFname).c_str(), 0);	//※ロードするために実行ファイルをチェック
								HINSTANCE h7z = ::GetModuleHandle(_T("7z.dll"));
								if (h7z && ::GetModuleFileName(h7z, szFname, MAX_PATH)>0) fp->dll7zName = szFname;
							}
						}
					}
					fp->GetArchiveType = (FUNC_ArcGetArchiveType)GetProcAdr(fp, "GetArchiveType");
				}
				break;

			case 1:	//LHA
				fp->GetLastError = (FUNC_ArcGetLastError)GetProcAdr(fp, "GetLastError");
				if (fp->Available) fp->Available = fp->GetLastError;
				if (fp->Available && fp->SetUnicodeMode) fp->Available = fp->SetUnicodeMode(TRUE);
				break;

			case 3:	//TAR
				fp->GetArchiveType = (FUNC_ArcGetArchiveType)GetProcAdr(fp, "GetArchiveType");
				break;

			case 4:	//RAR
				fp->GetFileCount = (FUNC_ArcGetFileCount)GetProcAdr(fp, "GetFileCount");
				fp->SetBackGroundMode = (FUNC_ArcSetBackGroundMode)GetProcAdr(fp, "SetBackGroundMode");
				fp->Available = fp->Available && fp->GetFileCount && fp->SetBackGroundMode;
				if (fp->Available && fp->SetUnicodeMode) fp->Available = fp->SetUnicodeMode(TRUE);
				if (fp->Available) fp->SetBackGroundMode(FALSE);	//非バックグラウンドモードに設定
				break;
			}

			if (fp->Available) {
				int v = fp->GetVersion();
				fp->VerStr.sprintf(_T("v%u.%02u"), v/100, v%100);

				//7-zipXX.dll 私家版チェック
				if (i==0) {
					fp->hasRename = v>=1500;	//Rename 対応 - 文字化け対策版(V15.～)
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
UserArcUnit::~UserArcUnit()
{
	for (int i=0; i<MAX_ARC_DLL; i++) if (ArcFunc[i].hDll) ::FreeLibrary(ArcFunc[i].hDll);
}

//---------------------------------------------------------------------------
//DLLの関数のアドレスを取得。
//---------------------------------------------------------------------------
FARPROC UserArcUnit::GetProcAdr(arc_func *fp, UnicodeString pnam)
{
	return ::GetProcAddress(fp->hDll, AnsiString(fp->Prefix + pnam).c_str());
}

//---------------------------------------------------------------------------
//ファイル名から種別を判定
//---------------------------------------------------------------------------
int UserArcUnit::GetArcType(UnicodeString arc_file,
	bool ex_sw)		//7z.dll対応のものを含む	(default = false)
{
	int arc_t = 0;
	UnicodeString fext = get_extension(arc_file);

	//拡張子による判定
#if defined(_WIN64)
	if		(test_FileExt(fext, FEXT_ZIP))	arc_t = UARCTYP_ZIP;
	else if (test_FileExt(fext, FEXT_7Z) || (Use7zDll && test_FileExt(fext, FExt7zDll)))
											arc_t = UARCTYP_7Z;
	else if (test_FileExt(fext, FEXT_TAR))	arc_t = UARCTYP_TAR;
	else if (test_FileExt(fext, FEXT_RAR))	arc_t = UARCTYP_RAR;
	//以下は現存しない
	else if (test_FileExt(fext, FEXT_LHA))	arc_t = UARCTYP_LHA;
	else if (test_FileExt(fext, FEXT_CAB))	arc_t = UARCTYP_CAB;
	else if (test_FileExt(fext, FEXT_ISO))	arc_t = UARCTYP_ISO;
#else
	if		(test_FileExt(fext, FEXT_ZIP))	arc_t = UARCTYP_ZIP;
	else if (test_FileExt(fext, FEXT_LHA))	arc_t = UARCTYP_LHA;
	else if (test_FileExt(fext, FEXT_CAB))	arc_t = UARCTYP_CAB;
	else if (test_FileExt(fext, FEXT_TAR))	arc_t = UARCTYP_TAR;
	else if (test_FileExt(fext, FEXT_7Z) || (Use7zDll && test_FileExt(fext, FExt7zDll)))
											arc_t = UARCTYP_7Z;
	else if (test_FileExt(fext, FEXT_RAR))	arc_t = UARCTYP_RAR;
	else if (test_FileExt(fext, FEXT_ISO))	arc_t = UARCTYP_ISO;
#endif

	//APIによる判定
	if (arc_t==0) {
		arc_func *fp = GetArcFunc(UARCTYP_ZIP);
		if (fp && fp->Available && fp->GetArchiveType) {
			int sub_t = fp->IsUnicode?
						fp->GetArchiveType(UTF8String(arc_file).c_str()) :
						fp->GetArchiveType(AnsiString(arc_file).c_str());
			if		(sub_t==1)			arc_t = UARCTYP_ZIP;
			else if (sub_t==2)			arc_t = UARCTYP_7Z;
			else if (ex_sw && sub_t>0)	arc_t = UARCTYP_ZIP;	//7z.dll対応
		}
	}
	if (arc_t==0) {
		arc_func *fp = GetArcFunc(UARCTYP_TAR);
		if (fp && fp->Available && fp->GetArchiveType) {
			int sub_t = fp->IsUnicode?
						fp->GetArchiveType(UTF8String(arc_file).c_str()) :
						fp->GetArchiveType(AnsiString(arc_file).c_str());
			if (sub_t>=1 && sub_t<=7) arc_t = UARCTYP_TAR;
		}
	}

	return arc_t;
}

//---------------------------------------------------------------------------
//ファイル名から形式を取得 (ZIP,7Z,TAR のみ)
//---------------------------------------------------------------------------
UnicodeString UserArcUnit::GetSubTypeStr(UnicodeString arc_file)
{
	UnicodeString ret_str;

	arc_func *fp = GetArcFunc(UARCTYP_ZIP);
	if (fp && fp->Available && fp->GetArchiveType) {
		int sub_t = fp->IsUnicode?
				fp->GetArchiveType(UTF8String(arc_file).c_str()) :
				fp->GetArchiveType(AnsiString(arc_file).c_str());
		switch (sub_t) {
		case 1: ret_str = "ZIP";	break;
		case 2: ret_str = "7Z";		break;
		}
	}

	if (ret_str.IsEmpty()) {
		arc_func *fp = GetArcFunc(UARCTYP_TAR);
		if (fp && fp->Available && fp->GetArchiveType) {
			int sub_t = fp->IsUnicode?
					fp->GetArchiveType(UTF8String(arc_file).c_str()) :
					fp->GetArchiveType(AnsiString(arc_file).c_str());
			switch (sub_t) {
			case 1: ret_str = "TAR";			break;
			case 2: ret_str = "TAR.GZ(TGZ)";	break;
			case 3: ret_str = "TAR.Z(TAZ)";		break;
			case 4: ret_str = "GZ";				break;
			case 5: ret_str = "Z";				break;
			case 6: ret_str = "TAR.BZ2";		break;
			case 7: ret_str = "BZ2";			break;
			}
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
arc_func * UserArcUnit::GetArcFunc(int typ)
{
	arc_func *fp =
		(typ==UARCTYP_ZIP || typ==UARCTYP_7Z)? &ArcFunc[0] :
		(typ==UARCTYP_LHA)? &ArcFunc[1] :
		(typ==UARCTYP_CAB)? &ArcFunc[2] :
		(typ==UARCTYP_TAR)? &ArcFunc[3] :
		(typ==UARCTYP_RAR)? &ArcFunc[4] :
		(typ==UARCTYP_ISO)? &ArcFunc[5] : NULL;

	if (fp && !fp->Available) fp = NULL;
	return fp;
}

//---------------------------------------------------------------------------
//動作中か？
//---------------------------------------------------------------------------
bool UserArcUnit::IsRunning(UnicodeString arc_file)
{
	arc_func *fp = GetArcFunc(GetArcType(arc_file, true));
	return(!fp || !fp->Available || fp->GetRunning());
}

//---------------------------------------------------------------------------
bool UserArcUnit::IsUnicode(int typ)
{
	arc_func *fp = GetArcFunc(typ);
	return (fp && fp->IsUnicode);
}

//---------------------------------------------------------------------------
//ファイルを処理可能か？
//---------------------------------------------------------------------------
bool UserArcUnit::IsAvailable(int typ)
{
	ErrMsg = EmptyStr;
	arc_func *fp = GetArcFunc(typ);
	if (fp) {
		if (!fp->Available) ErrMsg.sprintf(_T("%s が利用できません"), fp->DllName.c_str());
		return fp->Available;
	}
	else {
		ErrMsg = "対応していない形式です";
		return false;
	}
}

//---------------------------------------------------------------------------
bool UserArcUnit::IsAvailable(UnicodeString arc_file,
	bool ex_sw)		//7z.dll対応のものを含む	(default = false)
{
	return IsAvailable(GetArcType(arc_file, ex_sw));
}

//---------------------------------------------------------------------------
//Raneme 機能を持つか?
//---------------------------------------------------------------------------
bool UserArcUnit::HasRename(UnicodeString arc_file)
{
	if (test_FileExt(get_extension(arc_file), ".mobi")) return false;

	arc_func *fp = GetArcFunc(UARCTYP_ZIP);
	if (!fp || !fp->Available || !fp->GetArchiveType) return false;
	int sub_t = fp->GetArchiveType(UTF8String(arc_file).c_str());
	return ((sub_t==1 || sub_t==2) && fp->hasRename);
}

//---------------------------------------------------------------------------
//コマンドの実行
//戻り値 : 0 = 成功
// ResMsg、ErrMsg を設定
//---------------------------------------------------------------------------
int UserArcUnit::ExeCommand(
	int arc_t,
	arc_func *fp,
	UnicodeString cmd,	//コマンド
	bool large_buf)		//バッファサイズを大きくとる (default = false)
{
	if (fpAddDebugLog) fpAddDebugLog(fp->Prefix, cmd, EmptyStr);

	DWORD buf_size = large_buf? 1048576 : 4096;
	std::unique_ptr<char[]> buf(new char[buf_size]);
	strcpy(buf.get(), "\0");

	Busy = true;
	int res = (fp->IsUnicode)?
		fp->Command(MainHandle, UTF8String(cmd).c_str(), buf.get(), buf_size) :
		fp->Command(MainHandle, AnsiString(cmd).c_str(), buf.get(), buf_size);
	Busy = false;

	ResMsg = GetDispStr(buf.get(), arc_t);
	if (fpAddDebugLog) fpAddDebugLog(EmptyStr, EmptyStr, ResMsg);

	if (res!=0) {
		if (res==ERROR_USER_CANCEL)
			ErrMsg = "処理を中断しました";
		else
			ErrMsg = ResMsg;
	}
	else ErrMsg = EmptyStr;

	return res;
}

//---------------------------------------------------------------------------
//圧縮
//---------------------------------------------------------------------------
bool UserArcUnit::Pack(
	int arc_t,					//アーカイブ種別
	UnicodeString arc_file,
	UnicodeString src_dir,
	UnicodeString src_files,	//対象ファイル名の並び
								//  名前が空白を含む場合は、それぞれ引用符で囲んでおくこと
								//  @で始まる場合は、レスポンスファイルとみなす
	UnicodeString password)		//パスワード
{
	ErrMsg = EmptyStr;

	if (arc_t==0 || arc_file.IsEmpty() || src_files.IsEmpty()) return false;
	arc_func *fp = GetArcFunc(arc_t);
	if (!fp || !fp->Available || fp->GetRunning()) return false;

	if		(arc_file.Length()>=MAX_PATH)				ErrMsg = SysErrorMessage(ERROR_BUFFER_OVERFLOW);
	else if (arc_t==UARCTYP_RAR || arc_t==UARCTYP_ISO)	ErrMsg = "圧縮には対応していません";
	if (!ErrMsg.IsEmpty()) return false;

	if (!src_dir.IsEmpty()) src_dir = IncludeTrailingPathDelimiter(src_dir);

	UnicodeString cmd;
	if (arc_t==UARCTYP_ZIP) {
		cmd = "a -tzip -r-";
	}
	else if (arc_t==UARCTYP_7Z) {
		cmd = "a -t7z";
		if (ZipPrm_sfx) cmd += " -sfx";
		cmd += " -r-";
	}
	else if (arc_t==UARCTYP_LHA) cmd = "a -d1";
	else if (arc_t==UARCTYP_CAB) cmd = "-a";
	else if (arc_t==UARCTYP_TAR) cmd = "-c";

	//パスワード
	if ((arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z) && !password.IsEmpty())
		cmd.cat_sprintf(_T(" -p%s"), password.c_str());

	//圧縮レベル
	if (arc_t==UARCTYP_ZIP && ZipPrm_x!=5) {
		cmd.cat_sprintf(_T(" -mx=%u"), ZipPrm_x);
	}
	if (arc_t==UARCTYP_7Z && SevenPrm_x!=5) {
		cmd.cat_sprintf(_T(" -mx=%u"), SevenPrm_x);
	}
	else if (arc_t==UARCTYP_CAB) {
		if (CabPrm_z>=15 && CabPrm_z<=21)
			cmd.cat_sprintf(_T(" -ml:%u"), CabPrm_z);
		else
			cmd += " -mz";
	}
	else if (arc_t==UARCTYP_TAR && TarPrm_z>0) {
		cmd.cat_sprintf(_T(" -z%u"), TarPrm_z);
	}

	//7z.dll対応版への対応
	if (fp->use7zdll && starts_AT(exclude_quot(src_files))) {
		cmd.cat_sprintf(_T(" -i%s"), src_files.c_str());
		src_files = EmptyStr;
	}

	//追加オプション
	if		(arc_t==UARCTYP_ZIP && !ExSw_Zip.IsEmpty())	cmd.cat_sprintf(_T(" %s"), ExSw_Zip.c_str());
	else if (arc_t==UARCTYP_7Z  && !ExSw_7z.IsEmpty())	cmd.cat_sprintf(_T(" %s"), ExSw_7z.c_str());
	else if (arc_t==UARCTYP_LHA && !ExSw_Lha.IsEmpty())	cmd.cat_sprintf(_T(" %s"), ExSw_Lha.c_str());
	else if (arc_t==UARCTYP_CAB && !ExSw_Cab.IsEmpty())	cmd.cat_sprintf(_T(" %s"), ExSw_Cab.c_str());
	else if (arc_t==UARCTYP_TAR && !ExSw_Tar.IsEmpty())	cmd.cat_sprintf(_T(" %s"), ExSw_Tar.c_str());

	//これ以降、構文解析停止
	if		(arc_t==UARCTYP_LHA) cmd += " --3";
	else if (arc_t!=UARCTYP_CAB) cmd += " --";
	cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(arc_file).c_str());
	if (!src_dir.IsEmpty())   cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(src_dir).c_str());
	if (!src_files.IsEmpty()) cmd.cat_sprintf(_T(" %s"), src_files.c_str());

	return (ExeCommand(arc_t, fp, cmd)==0);
}

//---------------------------------------------------------------------------
//解凍
//---------------------------------------------------------------------------
bool UserArcUnit::UnPack(
	UnicodeString arc_file,		//アーカイブファイル名
	UnicodeString dst_dir,		//解凍先ディレクトリ名
	UnicodeString fnam,			//対象ファイル名 (default = EmptyStr)
								//  名前に空白が含まれる場合は引用符で囲む
								//  複数の場合は空白で区切って指定
								//  @で始まる場合はレスポンスファイルとみなす
	bool dir_sw,				//ディレクトリ付きで解凍	(default = true)
	bool hide_sw,				//処理状況ダイアログ非表示	(default = false)
	bool ow_sw)					//確認無しで上書き			(default = false)
{
	ErrMsg = EmptyStr;

	if		(arc_file.Length()>=MAX_PATH)	ErrMsg = SysErrorMessage(ERROR_BUFFER_OVERFLOW);
	else if (dst_dir.Length()>=MAX_PATH)	ErrMsg = "解凍先のパス名が長すぎます";
	else if (!FileExists(arc_file))			ErrMsg = SysErrorMessage(ERROR_FILE_NOT_FOUND);
	if (!ErrMsg.IsEmpty()) return false;

	int arc_t = GetArcType(arc_file, true);
	if (arc_t==0) return false;

	arc_func *fp = GetArcFunc(arc_t);
	if (!fp || !fp->Available || fp->GetRunning()) return false;

	if (!DirectoryExists(dst_dir))	return false;

	//7-zipXX.dll が解凍先ディレクトリ名内の連続空白を扱えない問題に対処
	UnicodeString org_dir = dst_dir;
	if ((arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z) && ContainsStr(dst_dir, "  ")) {
		TStringDynArray o_plst = split_path(org_dir);
		UnicodeString rep_str;
		for (int i=_T('A'); i <= _T('Z'); i++) {
			rep_str.sprintf(_T("~%c~"), i);
			dst_dir = ReplaceStr(org_dir, "  ", rep_str);
			TStringDynArray t_plst = split_path(dst_dir);
			bool ok = true;
			UnicodeString tmp_dir;
			for (int j=0; j<o_plst.Length && j<t_plst.Length && ok; j++) {
				tmp_dir += IncludeTrailingPathDelimiter(t_plst[j]);
				ok = SameText(o_plst[j], t_plst[j]) || !DirectoryExists(tmp_dir);
			}
			if (ok) break;
		}

		if (!rename_Path(org_dir, dst_dir)) {
			ErrMsg = "解凍先の一時改名に失敗";
			return false;
		}

		//必要ならアーカイブのディレクトリ名も一時変更
		TStringDynArray a_plst = split_path(ExtractFileDir(arc_file));
		for (int i=0; i<a_plst.Length && i<o_plst.Length; i++) {
			if (!SameText(a_plst[i], o_plst[i])) break;
			a_plst[i] = ReplaceStr(a_plst[i], "  ", rep_str);
		}
		UnicodeString a_pnam;
		for (int i=0; i<a_plst.Length; i++) a_pnam += IncludeTrailingPathDelimiter(a_plst[i]);
		arc_file = a_pnam + ExtractFileName(arc_file);
	}

	UnicodeString cmd;
	if (arc_t==UARCTYP_CAB) {
		cmd.sprintf(_T("%s"), dir_sw? _T("-x") : _T("-x -j"));
	}
	else if (arc_t==UARCTYP_TAR) {
		cmd.sprintf(_T("%s"), dir_sw? _T("-x") : _T("-x --use-directory=0"));
		cmd += " --confirm-overwrite=1";
	}
	else if (arc_t==UARCTYP_RAR) {
		cmd.sprintf(_T("%s"), dir_sw? _T("-x") : _T("-e"));
	}
	else {
		cmd.sprintf(_T("%s"), dir_sw? _T("x") : _T("e"));
	}

	//処理状況ダイアログ非表示
	if (hide_sw) {
		if		(arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z || arc_t==UARCTYP_ISO)
									 cmd += " -hide";
		else if (arc_t==UARCTYP_LHA) cmd += " -n1";
		else if (arc_t==UARCTYP_CAB) cmd += " -i";
		else if (arc_t==UARCTYP_TAR) cmd += " --display-dialog=0";
		else if (arc_t==UARCTYP_RAR) cmd += " -q";
	}

	if (!fnam.IsEmpty()) {
		//ファイル名をフルパスで比較
		if		(arc_t==UARCTYP_RAR) cmd += " -s";
		else if (arc_t==UARCTYP_LHA) cmd += " -p";
	}

	//確認無しで上書き
	if (ow_sw) {
		if		(arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z || arc_t==UARCTYP_ISO)
									 cmd += " -aoa";
		else if (arc_t==UARCTYP_LHA) cmd += " -jyo";
		else if (arc_t==UARCTYP_CAB || arc_t==UARCTYP_RAR)
									 cmd += " -o";
	}

	//アーカイブ
	cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(arc_file).c_str());

	//出力先ディレクトリ
	if (!dst_dir.IsEmpty()) {
		if (arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z || arc_t==UARCTYP_ISO)
			cmd.cat_sprintf(_T(" -o%s"), add_quot_if_spc(dst_dir).c_str());
		else
			cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(dst_dir).c_str());
	}

	//リストファイルの文字コード
	if (fp->IsUnicode) {
		if (arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z || arc_t==UARCTYP_ISO)
			cmd += " -scsUTF-8";
	}

	//ファイル名
	if (!fnam.IsEmpty()) {
		if (!starts_AT(exclude_quot(fnam))) {
			//レスポンスファイルでない場合、これ以降、構文解析停止
			if		(arc_t==UARCTYP_LHA) cmd += " --3";
			else if (arc_t!=UARCTYP_CAB) cmd += " --";
		}
		cmd.cat_sprintf(_T(" %s"), fnam.c_str());
	}

	RarUnpacking	= (arc_t==UARCTYP_RAR);
	HWND hLastFocus = ::GetFocus();
	ExeCommand(arc_t, fp, cmd);

	//※ RAR 実行後にアクティブ化されない現象の対策
	if (hLastFocus) ::SetFocus(hLastFocus);
	RarUnpacking = false;

	//解凍先を改名していたら元に戻す
	if (!SameText(dst_dir, org_dir) && !rename_Path(dst_dir, org_dir))
		ErrMsg += "\r\n解凍先ディレクトリ名の復帰に失敗";

	return ErrMsg.IsEmpty();
}

//---------------------------------------------------------------------------
//ファイルの追加
//---------------------------------------------------------------------------
bool UserArcUnit::AddFile(
	UnicodeString arc_file,		//アーカイブファイル名
	UnicodeString fnam)			//対象ファイル名 (空白を含んでいても引用符で囲まないこと)
								// @で始まる場合は、レスポンスファイルとみなす
{
	ErrMsg = EmptyStr;

	int arc_t = GetArcType(arc_file);
	if (arc_t==0) return false;

	if		(arc_file.Length()>=MAX_PATH)	ErrMsg = SysErrorMessage(ERROR_BUFFER_OVERFLOW);
	else if (!FileExists(arc_file))			ErrMsg = SysErrorMessage(ERROR_FILE_NOT_FOUND);
	else if (arc_t==UARCTYP_CAB || arc_t==UARCTYP_TAR || arc_t==UARCTYP_RAR || arc_t==UARCTYP_ISO)
											ErrMsg = "追加には対応していません";
	if (!ErrMsg.IsEmpty()) return false;

	arc_func *fp = GetArcFunc(arc_t);
	if (!fp || !fp->Available || fp->GetRunning()) return false;

	//コマンド
	UnicodeString cmd = "a";
	if		(arc_t==UARCTYP_ZIP) cmd += " -tzip";
	else if (arc_t==UARCTYP_7Z)  cmd += " -t7z";
	if		(arc_t==UARCTYP_7Z)  cmd += " -ms=off";

	//7z.dll対応版への対応
	if (fp->use7zdll && starts_AT(fnam)) {
		cmd.cat_sprintf(_T(" -i%s"), add_quot_if_spc(fnam).c_str());
		fnam = EmptyStr;
	}

	//これ以降、構文解析停止
	if		(arc_t==UARCTYP_LHA) cmd += " --3";
	else if (arc_t!=UARCTYP_CAB) cmd += " --";

	cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(arc_file).c_str());
	if (!fnam.IsEmpty()) cmd += " " + add_quot_if_spc(fnam);

	return (ExeCommand(arc_t, fp, cmd, true)==0);
}
//---------------------------------------------------------------------------
//ファイルの削除
//---------------------------------------------------------------------------
bool UserArcUnit::DelFile(
	UnicodeString arc_file,		//アーカイブファイル名
	UnicodeString fnam, 		//対象ファイル名 (空白を含んでいても引用符で囲まないこと)
								//  @で始まる場合は、レスポンスファイルとみなす
	bool hide_sw)				//処理状況ダイアログ表示の抑止
{
	ErrMsg = EmptyStr;

	int arc_t = GetArcType(arc_file);
	if (arc_t==0) return false;

	arc_func *fp = GetArcFunc(arc_t);
	if (!fp || !fp->Available || fp->GetRunning()) return false;

	if 		(arc_file.Length()>=MAX_PATH)	ErrMsg = SysErrorMessage(ERROR_BUFFER_OVERFLOW);
	else if (!FileExists(arc_file))			ErrMsg = SysErrorMessage(ERROR_FILE_NOT_FOUND);
	else if (arc_t==UARCTYP_CAB || arc_t==UARCTYP_TAR || arc_t==UARCTYP_RAR || arc_t==UARCTYP_ISO)
											ErrMsg = "削除には対応していません";
	if (!ErrMsg.IsEmpty()) return false;

	//コマンド
	UnicodeString cmd = "d";
	if (arc_t==UARCTYP_7Z) cmd += " -ms=off";

	if (hide_sw) {
		if (arc_t==UARCTYP_ZIP || arc_t==UARCTYP_7Z)
									 cmd += " -hide";
		else if (arc_t==UARCTYP_LHA) cmd += " -n1";
		else if (arc_t==UARCTYP_CAB) cmd += " -i";
		else if (arc_t==UARCTYP_TAR) cmd += " --display-dialog=0";
	}

	//7z.dll対応版への対応
	if (fp->use7zdll && starts_AT(fnam)) {
		cmd.cat_sprintf(_T(" -i%s"), add_quot_if_spc(fnam).c_str());
		fnam = EmptyStr;
	}

	//これ以降、構文解析停止
	if		(arc_t==UARCTYP_LHA) cmd += " --3";
	else if (arc_t!=UARCTYP_CAB) cmd += " --";

	cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(arc_file).c_str());
	if (!fnam.IsEmpty()) cmd += " " + add_quot_if_spc(fnam);

	return (ExeCommand(arc_t, fp, cmd, true)==0);
}

//---------------------------------------------------------------------------
//ファイル名の変更
//---------------------------------------------------------------------------
bool UserArcUnit::RenFile(UnicodeString arc_file, UnicodeString onam, UnicodeString nnam)
{
	int arc_t = GetArcType(arc_file);
	if (arc_t!=UARCTYP_ZIP && arc_t!=UARCTYP_7Z) return false;
	arc_func *fp = GetArcFunc(arc_t);
	if (!fp || !fp->Available || !fp->hasRename || fp->GetRunning()) return false;

	//コマンド
	UnicodeString cmd = "rn";
	cmd.cat_sprintf(_T(" %s"), add_quot_if_spc(arc_file).c_str());
	cmd.cat_sprintf(_T(" -- %s %s"), add_quot_if_spc(onam).c_str(), add_quot_if_spc(nnam).c_str());
	return (ExeCommand(arc_t, fp, cmd, true)==0);
}

//---------------------------------------------------------------------------
//アーカイブを開く
//---------------------------------------------------------------------------
bool UserArcUnit::OpenArc(UnicodeString arc_file)
{
	try {
		CloseArc();

		CurArcFile = arc_file;
		if (arc_file.Length()>=MAX_PATH) Abort();

		CurType = GetArcType(arc_file, true);
		if (CurType==0) Abort();

		arc_func *fp = GetArcFunc(CurType);	if (!fp) Abort();

		if (fp->Available) {
			//書庫のチェック
			bool res = fp->IsUnicode?
				fp->CheckArchive(UTF8String(arc_file).c_str(), 0) :
				fp->CheckArchive(AnsiString(arc_file).c_str(), 0);
			if (!res) Abort();

			//RAR:「ボリュームの交換」ダイアログのキャンセルに対応
			if (CurType==UARCTYP_RAR) {
				Busy = true;
				int cnt = fp->IsUnicode?
					fp->GetFileCount(UTF8String(arc_file).c_str()) :
					fp->GetFileCount(AnsiString(arc_file).c_str());
				Busy = false;
				if (cnt==-1) Abort();
			}

			//オープン
			if (fp->IsUnicode)
				hCurArc = fp->OpenArchive(MainHandle, UTF8String(arc_file).c_str(), M_ERROR_MESSAGE_OFF);
			else
				hCurArc = fp->OpenArchive(MainHandle, AnsiString(arc_file).c_str(), M_ERROR_MESSAGE_OFF);
		}
		return (hCurArc!=NULL);
	}
	catch (...) {
		hCurArc	   = NULL;
		CurType    = 0;
		CurArcFile = EmptyStr;
		return false;
	}
}

//---------------------------------------------------------------------------
//アーカイブを閉じる
//---------------------------------------------------------------------------
void UserArcUnit::CloseArc()
{
	if (hCurArc) {
		arc_func *fp = GetArcFunc(CurType);
		if (fp) fp->CloseArchive(hCurArc);
	}

	hCurArc    = NULL;
	CurType    = 0;
	CurArcFile = EmptyStr;
}

//---------------------------------------------------------------------------
//検索情報を設定 (FindFirstEx, FindNextEx で使用)
//---------------------------------------------------------------------------
void UserArcUnit::SetFindInf(arc_find_inf *inf,
	bool sub_dir)		//サブディレクトリも含む (default = false);
{
	if (!inf) return;
	arc_func *fp = GetArcFunc(CurType);  if (!fp) return;

	//初期化
	inf->f_name = EmptyStr;
	inf->is_dir = false;
	inf->f_size = -1;
	inf->f_attr = 0;
	inf->f_time = 0;

	//名前
	UnicodeString pnam = GetDispStr(FindInf.szFileName);
	pnam = slash_to_yen(pnam);
	UnicodeString atrstr = UnicodeString(FindInf.szAttribute).UpperCase();
	if (ContainsText(atrstr, "D")) pnam = IncludeTrailingPathDelimiter(pnam);	//(dll による違いを吸収)
	if (!FindDir.IsEmpty() && !StartsText(FindDir, pnam)) return;

	UnicodeString fnam = pnam;
	remove_top_text(fnam, FindDir);
	if (!sub_dir) {
		if (contains_PathDlmtr(fnam)) {
			fnam = get_tkn(fnam, '\\');
			inf->is_dir = true;
		}
	}
	else {
		inf->is_dir = (ends_PathDlmtr(fnam) || fnam.IsEmpty());
	}
	inf->f_name = ExcludeTrailingPathDelimiter(fnam);

	//サイズの計算(4GB超に対応)
	if (!inf->is_dir) {
		__int64 org_size = -1;

		if (fp->GetOriginalSizeEx) {
			if (!fp->GetOriginalSizeEx(hCurArc, &org_size)) org_size = FindInf.dwOriginalSize;
		}
		else if (CurType==UARCTYP_RAR) {
			if (FindInf.dwOriginalSize==0xffffffff) {
				//4GB超の場合 -l コマンドで取得
				UnicodeString cmd = "-l " + add_quot_if_spc(CurArcFile) + " " + add_quot_if_spc(pnam);
				if (ExeCommand(CurType, fp, cmd)==0) {
					std::unique_ptr<TStringList> r_lst(new TStringList());
					r_lst->Text = ResMsg;
					for (int i=0; i<r_lst->Count; i++) {
						UnicodeString lbuf = r_lst->Strings[i];
						if (StartsText(pnam, lbuf)) {
							lbuf.Delete(1, pnam.Length());
							lbuf = get_tkn(Trim(lbuf), ' ');
							org_size = StrToInt64Def(lbuf, 0);
							break;
						}
					}
				}
			}
			else {
				org_size = FindInf.dwOriginalSize;
			}
		}
		else {	//CAB
			org_size = FindInf.dwOriginalSize;
		}

		//サイズ
		inf->f_size = org_size;
	}

	//タイムスタンプ
	FILETIME f_tm;
	SYSTEMTIME s_tm;
	if (::DosDateTimeToFileTime(FindInf.wDate, FindInf.wTime, &f_tm)) {
		if (::FileTimeToSystemTime(&f_tm, &s_tm)) inf->f_time = SystemTimeToDateTime(s_tm);
	}
	//属性
	inf->f_attr = 0;
	if (inf->is_dir) {
		inf->f_attr |= faDirectory;
	}
	else {
		//属性
		if (ContainsText(atrstr, "A")) inf->f_attr |= faArchive;
		if (ContainsText(atrstr, "S")) inf->f_attr |= faSysFile;
		if (ContainsText(atrstr, "H")) inf->f_attr |= faHidden;
		if (ContainsText(atrstr, "R")) inf->f_attr |= faReadOnly;
	}
}

//---------------------------------------------------------------------------
//最初の格納ファイル情報を取得
//---------------------------------------------------------------------------
bool UserArcUnit::FindFirst(UnicodeString mask, INDIVIDUALINFO *inf)
{
	if (!hCurArc) return false;
	arc_func *fp = GetArcFunc(CurType);  if (!fp) return false;

	if (CurType!=UARCTYP_ISO && fp->IsUnicode)
		return (fp->FindFirst(hCurArc, UTF8String(mask).c_str(), inf) == 0);
	else
		return (fp->FindFirst(hCurArc, AnsiString(mask).c_str(), inf) == 0);
	//※unisoXX.dll だとUnicodeモードにしてもUTF-8のマスクを受け付けてくれない?
}

//---------------------------------------------------------------------------
bool UserArcUnit::FindFirstEx(
	UnicodeString dnam,	//基準ディレクトリ￥
	arc_find_inf *inf,
	bool sub_dir)		//サブディレクトリも含む (default = false);
{
	FindDir = dnam;
	if (!FindFirst(add_quot_if_spc(dnam + "*"), &FindInf)) return false;
	SetFindInf(inf, sub_dir);
	return true;
}

//---------------------------------------------------------------------------
//次の格納ファイル情報を取得
//---------------------------------------------------------------------------
bool UserArcUnit::FindNext(INDIVIDUALINFO *inf)
{
	if (!hCurArc) return false;
	arc_func *fp = GetArcFunc(CurType);  if (!fp) return false;
	return (fp->FindNext(hCurArc, inf) == 0);
}
//---------------------------------------------------------------------------
bool UserArcUnit::FindNextEx(arc_find_inf *inf,
	bool sub_dir)	//サブディレクトリも含む (default = false);
{
	if (!FindNext(&FindInf)) return false;
	SetFindInf(inf, sub_dir);
	return true;
}

//---------------------------------------------------------------------------
//表示用の文字列を取得
// (dll による違いを吸収)
//---------------------------------------------------------------------------
UnicodeString UserArcUnit::GetDispStr(
	char *s,
	int arc_t)	// default = 0 : CurType
{
	UnicodeString rstr;
	if (arc_t==0) arc_t = CurType;
	arc_func *fp = GetArcFunc(arc_t);
	if (fp->IsUnicode)
		rstr = UTF8String(s);
	else
		rstr = s;
	return rstr;
}

//---------------------------------------------------------------------------
//アーカイブのルート項目数を取得
//---------------------------------------------------------------------------
int UserArcUnit::GetRootCount(UnicodeString arc_file)
{
	int i_cnt = -1;
	if (OpenArc(arc_file)) {
		arc_find_inf inf;
		std::unique_ptr<TStringList> lst(new TStringList());
		if (FindFirstEx(EmptyStr, &inf)) {
			i_cnt = 0;
			do {
				UnicodeString fnam = inf.f_name;
				if (fnam.IsEmpty() || (inf.is_dir && lst->IndexOf(fnam)!=-1)) continue;
				lst->Add(fnam);
				i_cnt++;
			} while (FindNextEx(&inf));
		}
		CloseArc();
	}
	return i_cnt;
}

//---------------------------------------------------------------------------
//ファイル情報の取得
//---------------------------------------------------------------------------
bool UserArcUnit::GetFileInf(
	UnicodeString arc_file,		//アーカイブファイル名
	UnicodeString fnam,			//対象項目名
	arc_find_inf *inf)			//[0] ファイル情報
{
	bool res = false;
	if (OpenArc(arc_file)) {
		if (!fnam.IsEmpty()) fnam = ExcludeTrailingPathDelimiter(fnam);
		UnicodeString dnam = ExtractFilePath(fnam);
		fnam = ExtractFileName(fnam);
		if (FindFirstEx(dnam, inf)) {
			do {
				if (SameText(inf->f_name, fnam)) {
					res = true; break;
				}
			} while (FindNextEx(inf));
		}
		CloseArc();
	}
	return res;
}

//---------------------------------------------------------------------------
//ファイル一覧の取得
//---------------------------------------------------------------------------
bool UserArcUnit::GetFileList(UnicodeString arc_file, TStringList *lst)
{
	if (arc_file.Length()>=MAX_PATH) return false;		//MAX_PATH超のアーカイブはダメ

	bool ret = true;
	if (OpenArc(arc_file)) {
		try {
			arc_find_inf inf;
			if (FindFirstEx(EmptyStr, &inf, true)) {
				do {
					if (!inf.is_dir) lst->Add(inf.f_name);
				} while (FindNextEx(&inf, true));

				if (CurType==UARCTYP_LHA) {
					DWORD dwError;
					arc_func *fp = GetArcFunc(CurType);
					if (!fp || fp->GetLastError(&dwError)!=0) Abort();
				}
			}
		}
		catch (...) {
			ret = false;
		}
		CloseArc();
	}
	return ret;
}

//---------------------------------------------------------------------------
//指定拡張子にマッチする最初のファイル名を取得
//戻り値 : アーカイブ名/ファイル名
//---------------------------------------------------------------------------
UnicodeString UserArcUnit::GetFirstFile(UnicodeString arc_file, UnicodeString xlist)
{
	UnicodeString ret_nam;
	if (OpenArc(arc_file)) {
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			arc_func *fp = GetArcFunc(CurType);
			if (fp && (fp->GetAttribute && (fp->GetAttribute(hCurArc) & FA_ENCRYPTED)==0)) {
				do {
					if (!inf.is_dir) {
						if (test_FileExt(get_extension(inf.f_name), xlist)) {
							ret_nam = arc_file + "/" + inf.f_name;
							break;
						}
					}
				} while (FindNextEx(&inf, true));
			}
		}
		CloseArc();
	}
	return ret_nam;
}

//---------------------------------------------------------------------------
//アーカイブ内の全ディレクトリのリストを取得
//---------------------------------------------------------------------------
void UserArcUnit::GetDirList(
	UnicodeString arc_file,		//アーカイブのファイル名
	TStrings *d_lst)		//[o] (空)ディレクトリ名リスト
{
	if (OpenArc(arc_file)) {
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			do {
				UnicodeString dnam = ExtractFileDir(inf.f_name);
				if (!dnam.IsEmpty() && d_lst->IndexOf(dnam)==-1) d_lst->Add(dnam);
			} while (FindNextEx(&inf, true));
		}
		CloseArc();
	}
}
//---------------------------------------------------------------------------
//アーカイブから指定ディレクトリ以下の全項目のリストを取得
//---------------------------------------------------------------------------
void UserArcUnit::GetItemList(
	UnicodeString arc_file,	//アーカイブファイル名
	UnicodeString dnam,		//対象ディレクトリ名
	TStrings *f_lst,		//[o] ファイル名リスト
	TStrings *d_lst)		//[o] (空)ディレクトリ名リスト
{
	if (OpenArc(arc_file)) {
		dnam = !dnam.IsEmpty()? IncludeTrailingPathDelimiter(dnam) : EmptyStr;
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			do {
				UnicodeString fnam = inf.f_name;
				if (!dnam.IsEmpty()) {
					if (!StartsText(dnam, fnam)) continue;
					fnam.Delete(1, dnam.Length());
				}
				//ディレクトリ
				if (inf.is_dir || fnam.IsEmpty()) {
					if (d_lst && d_lst->IndexOf(inf.f_name)==-1) d_lst->Add(inf.f_name);
				}
				//ファイル
				else if (f_lst) {
					f_lst->Add(inf.f_name);
				}
			} while (FindNextEx(&inf, true));
		}
		CloseArc();
	}
}

//---------------------------------------------------------------------------
//ZIP アーカイブ内に画像があるか?
//  実行ファイルなどが含まれている場合は無効
//---------------------------------------------------------------------------
bool UserArcUnit::HasZipImg(
	UnicodeString arc_file,	//アーカイブファイル名
	UnicodeString xlist)	//対応画像拡張子
{
	if (!test_FileExt(get_extension(arc_file), FEXT_ZIPIMG)) return false;

	bool ret = false;
	if (OpenArc(arc_file)) {
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			arc_func *fp = GetArcFunc(CurType);
			if (fp && (fp->GetAttribute && (fp->GetAttribute(hCurArc) & FA_ENCRYPTED)==0)) {
				do {
					if (!inf.is_dir && !starts_AT(inf.f_name)) {
						UnicodeString fext = get_extension(inf.f_name);
						if (test_FileExt(fext, ".exe.com.msi.msu.scr.dll.rll.cpl.ocx")) {
							ret = false;  break;
						}
						if (test_FileExt(fext, xlist)) {
							ret = true;   break;
						}
					}
				} while (FindNextEx(&inf, true));
			}
		}
		CloseArc();
	}
	return ret;
}

//---------------------------------------------------------------------------
//アーカイブ情報の取得
//---------------------------------------------------------------------------
bool UserArcUnit::GetArcInfo(
	UnicodeString arc_file,
	int *f_cnt,			//ファイル数
	__int64 *org_size,	//元サイズ
	float *ratio,		//圧縮率			(default = NULL)
	bool *is_enc,		//パスワード保護	(default = NULL)
	TStringList *lst)	//ファイル名リスト	(default = NULL)
{
	*f_cnt	  = 0;
	*org_size = 0;
	if (ratio)  *ratio	= 0.0;
	if (is_enc) *is_enc	= false;

	if (arc_file.Length()>=MAX_PATH) return false;
	if (!OpenArc(arc_file)) return false;
	arc_func *fp = GetArcFunc(CurType);  if (!fp) return false;

	bool is_rar_part = (CurType==UARCTYP_RAR)? ContainsText(get_extension(get_base_name(arc_file)), "part") : false;

	bool ret = true;
	__int64 o_size = 0;
	try {
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			if (is_enc && (CurType==UARCTYP_ZIP || CurType==UARCTYP_7Z)) {
				*is_enc = (fp->GetAttribute && (fp->GetAttribute(hCurArc) & FA_ENCRYPTED)!=0);
			}

			do {
				if (!inf.is_dir) {
					o_size += inf.f_size;
					(*f_cnt)++;
					if (lst) lst->Add(inf.f_name);
				}
			} while (FindNextEx(&inf, true));

			if (CurType==UARCTYP_LHA) {
				DWORD dwError;
				if (fp->GetLastError(&dwError)!=0) Abort();
			}
		}
	}
	catch (...) {
		ret = false;
	}
	CloseArc();

	//圧縮率の計算
	if (ret) {
		__int64 a_size = get_file_size(arc_file);
		*org_size = o_size;
		if (ratio && !is_rar_part) *ratio = (o_size>0 && a_size>0)? 100.0 * a_size/o_size : 0.0;
	}

	return ret;
}

//---------------------------------------------------------------------------
//リストを取得
//---------------------------------------------------------------------------
bool UserArcUnit::GetArcList(
	UnicodeString arc_file,		//アーカイブファイル名
	UnicodeString cmd,	 		//コマンド (l または t)
	TStringList *lst)
{
	if (cmd.IsEmpty()) return false;

	int arc_t = GetArcType(arc_file, true);
	if (arc_t==0) return false;

	arc_func *fp = GetArcFunc(arc_t);
	if (!fp) return false;

	if (arc_t==UARCTYP_CAB || arc_t==UARCTYP_TAR || arc_t==UARCTYP_RAR) cmd = "-" + cmd;
	cmd += (" " + add_quot_if_spc(arc_file) + " *");

	HWND hLastFocus = ::GetFocus();

	ExeCommand(arc_t, fp, cmd, true);

	//※ RAR 実行後にアクティブ化されない現象の対策
	if (hLastFocus) ::SetFocus(hLastFocus);

	lst->Text = ResMsg;

	return (lst->Count>0);
}

//---------------------------------------------------------------------------
//タイムスタンプを最新ファイルのものに設定
//---------------------------------------------------------------------------
bool UserArcUnit::SetArcTime(UnicodeString arc_file, bool force)
{
	bool ret = false;

	if (OpenArc(arc_file)) {
		TDateTime dt = 0.0;
		arc_find_inf inf;
		if (FindFirstEx(EmptyStr, &inf, true)) {
			do {
				dt = std::max(dt, inf.f_time);
			} while (FindNextEx(&inf, true));
		}
		CloseArc();

		//タイムスタンプ設定
		if (set_file_age(arc_file, dt, force)) ret = true;
	}

	ErrMsg = !ret? "タイムスタンプ設定に失敗しました" : "";
	return ret;
}
//---------------------------------------------------------------------------
