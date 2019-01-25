//----------------------------------------------------------------------//
// xdoc2txt2															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_shell.h"
#include "usr_xd2tx.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
bool xd2tx_Available;			//���p�\
UnicodeString xd2tx_FileName;	//dll�̃p�X�t���t�@�C����

//---------------------------------------------------------------------------
//������
// �߂�l : true = xd2txlib.dll �����p�\
//---------------------------------------------------------------------------
bool xd2tx_Initialize()
{
	xd2tx_Available = false;
	xd2tx_FileName	= EmptyStr;

	HINSTANCE hXdoc2txt = ::LoadLibrary(_T("xd2txlib.dll"));
	if (hXdoc2txt) {
		xd2tx_Available = ((FUNC_ExtractText)::GetProcAddress(hXdoc2txt, "ExtractText")!=NULL);

		_TCHAR szFname[MAX_PATH];
		if (GetModuleFileName(hXdoc2txt, szFname, MAX_PATH)>0) xd2tx_FileName = szFname;

		::FreeLibrary(hXdoc2txt);
	}

	return xd2tx_Available;
}

//---------------------------------------------------------------------------
//�Ή��g���q��?
//---------------------------------------------------------------------------
bool xd2tx_TestExt(UnicodeString fext,
	bool inc_rtf)	//.rtf .wri ���܂�	(default = false)
{
	return (xd2tx_Available
				&& (test_FileExt(fext, FEXT_XD2TX) || (inc_rtf && test_FileExt(fext, _T(".rtf.wri")))));
}

//---------------------------------------------------------------------------
//�v���p�e�B�擾�\��?
//---------------------------------------------------------------------------
bool xd2tx_PropExt(UnicodeString fext)
{
	return (xd2tx_Available && test_FileExt(fext, FEXT_XD2T_PRP));
}

//---------------------------------------------------------------------------
//�e�L�X�g�̒��o
//---------------------------------------------------------------------------
bool xd2tx_Extract(UnicodeString fnam, TStringList *lst, bool prop)
{
	bool ok = false;

	HINSTANCE hXdoc2txt = NULL;

	try {
		hXdoc2txt = ::LoadLibrary(_T("xd2txlib.dll"));
		if (hXdoc2txt) {
			FUNC_ExtractText f_ExtractText = (FUNC_ExtractText)::GetProcAddress(hXdoc2txt, "ExtractText");
			if (f_ExtractText) {
				BSTR fileText = ::SysAllocString(null_TCHAR);
				if (f_ExtractText(fnam.c_str(), prop, &fileText)>0) {
					lst->Text = (LPCTSTR)fileText;
					ok = true;
				}
				::SysFreeString(fileText);
			}
		}
	}
	catch (...) {
		ok = false;
	}

	if (hXdoc2txt) ::FreeLibrary(hXdoc2txt);

	return ok;
}

//---------------------------------------------------------------------------
//�v���p�e�B�擾
//---------------------------------------------------------------------------
bool xd2tx_GetInfo(UnicodeString fnam, TStringList *lst)
{
	bool ok = false;

	std::unique_ptr<TStringList> i_lst(new TStringList());
	if (xd2tx_Extract(fnam, i_lst.get(), true)) {
		for (int i=0; i<i_lst->Count; i++) {
			UnicodeString lbuf = i_lst->Strings[i];
			if (remove_top_s(lbuf, '<')) {
				UnicodeString vnam = split_tkn(lbuf, '>');
				add_PropLine(vnam, get_tkn(lbuf, "</"), lst);
				ok = true;
			}
		}
	}

	return ok;
}
//---------------------------------------------------------------------------
