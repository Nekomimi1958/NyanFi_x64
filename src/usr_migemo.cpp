//----------------------------------------------------------------------//
// MIGEMO																//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_migemo.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
MigemoUnit::MigemoUnit(UnicodeString dnam)
{
	Available = false;
	DictReady = false;
	MigemoObj = NULL;
	MinLength = 1;

	PathName  = dnam;
	if (PathName.IsEmpty())
		PathName = ExtractFilePath(Application->ExeName);
	else
		PathName = IncludeTrailingPathDelimiter(PathName);

	FileName = PathName + "migemo.dll";
	hMigemo  = ::LoadLibrary(FileName.c_str());
	if (hMigemo) {
		f_migemo_open	 = (FUNC_migemo_open)::GetProcAddress(hMigemo,		"migemo_open");
		f_migemo_close	 = (FUNC_migemo_close)::GetProcAddress(hMigemo,		"migemo_close");
		f_migemo_query	 = (FUNC_migemo_query)::GetProcAddress(hMigemo,		"migemo_query");
		f_migemo_release = (FUNC_migemo_release)::GetProcAddress(hMigemo,	"migemo_release");
		Available = f_migemo_open && f_migemo_close && f_migemo_query && f_migemo_release;

		if (Available) {
			//辞書を検索
			TSearchOption opt = TSearchOption::soAllDirectories;
			TStringDynArray f_list = TDirectory::GetFiles(PathName, "migemo-dict", opt);
			UnicodeString dic_name;
			for (int i=0; i<f_list.Length; ++i) {
				UnicodeString fnam = f_list[i].LowerCase();
				if (!ContainsText(fnam, "utf-8")) {
					dic_name = fnam; break;
				}
			}
			//辞書を読み込む
			if (!dic_name.IsEmpty()) {
				MigemoObj = f_migemo_open(AnsiString(dic_name).c_str());
				DictReady = (MigemoObj!=NULL);
			}
		}
	}
}

//---------------------------------------------------------------------------
MigemoUnit::~MigemoUnit()
{
	if (MigemoObj) f_migemo_close(MigemoObj);
	if (hMigemo) ::FreeLibrary(hMigemo);
}

//---------------------------------------------------------------------------
//サーチ用正規表現を取得
//---------------------------------------------------------------------------
UnicodeString MigemoUnit::Query(UnicodeString qstr)
{
	UnicodeString ret_str;
	if (MigemoObj) {
		unsigned char *qp = f_migemo_query(MigemoObj, (unsigned char*)AnsiString(qstr).c_str());
		if (qp) {
			ret_str = UnicodeString((char*)qp);
			f_migemo_release(MigemoObj, qp);
			//※エスケープされていない "+" が含まれている場合に対する対処
			ret_str = TRegEx::Replace(ret_str, "([^\\\\])\\+", "\\1\\\\+");
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//正規表現パターンを取得
//---------------------------------------------------------------------------
UnicodeString MigemoUnit::GetRegExPtn(
	bool sw,			//migemo を利用
	UnicodeString w,	//検索語
	int  minlen)		//検索開始文字数	(default = 0: MinLength を適用)
{
	if (minlen==0) minlen = MinLength;
	if (w.IsEmpty() || (sw && w.Length()<minlen)) return EmptyStr;

	UnicodeString ptn = sw? Query(w) : TRegEx::Escape(w);
	return chk_RegExPtn(ptn)? ptn : EmptyStr;
}
//---------------------------------------------------------------------------
