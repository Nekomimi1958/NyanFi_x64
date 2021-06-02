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
			//����������
			TSearchOption opt = TSearchOption::soAllDirectories;
			TStringDynArray f_list = TDirectory::GetFiles(PathName, "migemo-dict", opt);
			UnicodeString dic_name;
			for (int i=0; i<f_list.Length; ++i) {
				UnicodeString fnam = f_list[i].LowerCase();
				if (!ContainsText(fnam, "utf-8")) {
					dic_name = fnam; break;
				}
			}
			//������ǂݍ���
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
//�T�[�`�p���K�\�����擾
//---------------------------------------------------------------------------
UnicodeString MigemoUnit::Query(UnicodeString qstr)
{
	UnicodeString ret_str;
	if (MigemoObj) {
		unsigned char *qp = f_migemo_query(MigemoObj, (unsigned char*)AnsiString(qstr).c_str());
		if (qp) {
			ret_str = UnicodeString((char*)qp);
			f_migemo_release(MigemoObj, qp);
			//���G�X�P�[�v����Ă��Ȃ� "+" ���܂܂�Ă���ꍇ�ɑ΂���Ώ�
			ret_str = replace_regex(ret_str, _T("([^\\\\])\\+"), _T("\\1\\\\+"));
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//���K�\���p�^�[�����擾
//---------------------------------------------------------------------------
UnicodeString MigemoUnit::GetRegExPtn(
	bool sw,			//migemo �𗘗p
	UnicodeString w,	//������
	int  minlen)		//�����J�n������	(default = 0: MinLength ��K�p)
{
	if (minlen==0) minlen = MinLength;
	if (w.IsEmpty() || (sw && w.Length()<minlen)) return EmptyStr;

	UnicodeString ptn = sw? Query(w) : TRegEx::Escape(w);
	return chk_RegExPtn(ptn)? ptn : EmptyStr;
}
//---------------------------------------------------------------------------
