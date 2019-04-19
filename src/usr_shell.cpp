//----------------------------------------------------------------------//
// Shell �֘A															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <mmsystem.h>
#include <utilcls.h>
#include <System.StrUtils.hpp>
#include "usr_str.h"
#include "usr_key.h"
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "usr_shell.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//�w��E�B���h�E�̃e�L�X�g���擾
//---------------------------------------------------------------------------
UnicodeString get_WndText(HWND hWnd)
{
	_TCHAR tbuf[1024];
	return ((::GetWindowText(hWnd, tbuf, 1023)>0)? UnicodeString(tbuf) : EmptyStr);
}
//---------------------------------------------------------------------------
//�w��E�B���h�E�̃N���X�����擾
//---------------------------------------------------------------------------
UnicodeString get_WndClassName(HWND hWnd)
{
	_TCHAR cbuf[256];
	return ((::GetClassName(hWnd, cbuf, 255)>0)? UnicodeString(cbuf) : EmptyStr);
}

//---------------------------------------------------------------------------
//�v���p�e�B���̉E�����\��������̎擾
//---------------------------------------------------------------------------
UnicodeString get_PropTitle(UnicodeString s)
{
	return align_r_str(s, FPRP_NAM_WD, ": ");
}
//---------------------------------------------------------------------------
UnicodeString get_PropTitle(const _TCHAR *s)
{
	return align_r_str(s, FPRP_NAM_WD, ": ");
}

//---------------------------------------------------------------------------
//�v���p�e�B���ڂ̍쐬("�v���p�e�B��: �l������");
//---------------------------------------------------------------------------
UnicodeString make_PropLine(UnicodeString tit, UnicodeString str)
{
	return align_r_str(tit, FPRP_NAM_WD, ": ") + str;
}
//---------------------------------------------------------------------------
UnicodeString make_PropLine(const _TCHAR *tit, UnicodeString str)
{
	return align_r_str(tit, FPRP_NAM_WD, ": ") + str;
}
//---------------------------------------------------------------------------
UnicodeString make_PropLine(UnicodeString tit, int n)
{
	return align_r_str(tit, FPRP_NAM_WD, ": ").cat_sprintf(_T("%u"), n);
}
//---------------------------------------------------------------------------
UnicodeString make_PropLine(const _TCHAR *tit, int n)
{
	return align_r_str(tit, FPRP_NAM_WD, ": ").cat_sprintf(_T("%u"), n);
}

//---------------------------------------------------------------------------
void add_PropLine(UnicodeString tit, UnicodeString str, TStrings *lst, int flag)
{
	UnicodeString lbuf = make_PropLine(tit, str);
	if (flag!=0) lst->AddObject(lbuf, (TObject*)(NativeInt)flag); else lst->Add(lbuf);
}
//---------------------------------------------------------------------------
void add_PropLine(const _TCHAR *tit, UnicodeString str, TStrings *lst, int flag)
{
	UnicodeString lbuf = make_PropLine(tit, str);
	if (flag!=0) lst->AddObject(lbuf, (TObject*)(NativeInt)flag); else lst->Add(lbuf);
}
//---------------------------------------------------------------------------
void add_PropLine(UnicodeString tit, int n, TStrings *lst, int flag)
{
	UnicodeString lbuf = make_PropLine(tit, n);
	if (flag!=0) lst->AddObject(lbuf, (TObject*)(NativeInt)flag); else lst->Add(lbuf);
}
//---------------------------------------------------------------------------
void add_PropLine(const _TCHAR *tit, int n, TStrings *lst, int flag)
{
	UnicodeString lbuf = make_PropLine(tit, n);
	if (flag!=0) lst->AddObject(lbuf, (TObject*)(NativeInt)flag); else lst->Add(lbuf);
}

//---------------------------------------------------------------------------
void add_PropLine_if(const _TCHAR *tit, UnicodeString str, TStrings *lst, int flag)
{
	if (!str.IsEmpty()) add_PropLine(tit, str, lst, flag);
}

//---------------------------------------------------------------------------
//�x����ǉ�
//---------------------------------------------------------------------------
void add_WarnLine(UnicodeString str, TStrings *lst)
{
	lst->AddObject(make_PropLine(_T("�x��"), str), (TObject*)LBFLG_ERR_FIF);
}

//---------------------------------------------------------------------------
UserShell *usr_SH = NULL;

HWND TargetHandle = NULL;
int  DragStartTag = -1;				//�h���b�O���J�n���ꂽ���X�g�^�O
int  DroppedTag	  = -1;				//�h���b�v���ꂽ���X�g�^�O
int  DroppedMode  = 0;				//�h���b�v�̃��[�h
int  DefDropMode  = 0;				//�f�t�H���g�̃h���b�v���� (0: �G�N�X�v���[���݊�/ 1:�R�s�[/ 2:�ړ�)

UnicodeString DragDrive;			//�h���b�O���̃h���C�u
bool DropRefused = false;			//�Ή��O�̑Ώۂ�����

TStringList *DroppedList = NULL;	//�h���b�v���ꂽ�t�@�C�����X�g
TListBox *L_FileListBox  = NULL;	//���t�@�C�����X�g
TListBox *R_FileListBox  = NULL;	//�E�t�@�C�����X�g

TDropTargetList *TargetList;
int TargetIndex;

//�R���e�L�X�g���j���[
IContextMenu2 *ContextMenu2 = NULL;
IContextMenu3 *ContextMenu3 = NULL;
#define USR_NENUID_BASE	1000

//---------------------------------------------------------------------------
//�h���b�v�̃��[�h���擾
//---------------------------------------------------------------------------
DWORD get_DropMode(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt)
{
	if (pDataObj) {	//DragEnter ���̃`�F�b�N
		FORMATETC formatetc;
		formatetc.cfFormat = CF_HDROP;
		formatetc.ptd	   = NULL;
		formatetc.dwAspect = DVASPECT_CONTENT;
		formatetc.lindex   = -1;
		formatetc.tymed    = TYMED_HGLOBAL;

		try {
			//�h���b�O���̃h���C�u���擾
			UnicodeString dnam;
			HRESULT hr = pDataObj->QueryGetData(&formatetc);	if (FAILED(hr)) Abort();
			STGMEDIUM medium;
			hr = pDataObj->GetData(&formatetc, &medium);		if (FAILED(hr)) Abort();
			HDROP dp = (HDROP)::GlobalLock(medium.hGlobal);
			UINT f_cnt = ::DragQueryFile(dp, 0xFFFFFFFF, NULL, 0);
			if (f_cnt>0) {
				dnam.SetLength(MAX_PATH);
				int len = ::DragQueryFile(dp, 0, dnam.c_str(), dnam.Length());
				dnam.SetLength(len);
			}
			::GlobalUnlock(medium.hGlobal);
			ReleaseStgMedium(&medium);
			DragDrive = ExtractFileDrive(dnam);
		}
		catch (...) {
			DragDrive = EmptyStr;
			//�Ή��O�̑Ώۂ�����
			formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);
			DropRefused = FAILED(pDataObj->QueryGetData(&formatetc));
			if (!DropRefused) {
				formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_INETURL);
				DropRefused = FAILED(pDataObj->QueryGetData(&formatetc));
				if (!DropRefused) {
					STGMEDIUM medium;
					DropRefused = FAILED(pDataObj->GetData(&formatetc, &medium));
					if (!DropRefused) ReleaseStgMedium(&medium);
				}
			}
		}
	}

	if (DropRefused) return DROPEFFECT_NONE;

	//���[�h�̐ݒ�
	DWORD d_mode = DROPEFFECT_NONE;
	const POINT p = {pt.x, pt.y};
	HWND hWnd = ::WindowFromPoint(p);
	TargetIndex = -1;

	//�t�@�C�����X�g
	if (hWnd==L_FileListBox->Handle || hWnd==R_FileListBox->Handle) {
		DWORD kstt = (grfKeyState & (MK_CONTROL|MK_SHIFT|MK_ALT));
		if		(kstt==MK_CONTROL)			  d_mode = DROPEFFECT_COPY;	//�R�s�[
		else if	(kstt==MK_SHIFT)			  d_mode = DROPEFFECT_MOVE;	//�ړ�
		else if (kstt==(MK_CONTROL|MK_SHIFT)) d_mode = DROPEFFECT_LINK;	//�V���[�g�J�b�g
		//�f�t�H���g�̑���
		else if (kstt==0) {
			//�h���b�v��̃h���C�u�Ɣ�r���ē�������߂�
			if (DefDropMode==0) {
				TListBox *lp = (hWnd==L_FileListBox->Handle)? L_FileListBox : R_FileListBox;
				UnicodeString drv = (lp->Count>0)? ExtractFileDrive(lp->Items->Strings[0]) : EmptyStr;
				if (!DragDrive.IsEmpty() && !drv.IsEmpty() && SameText(DragDrive, drv))
					d_mode = DROPEFFECT_MOVE;	//�ړ�
				else
					d_mode = DROPEFFECT_COPY;	//�R�s�[
			}
			else if (DefDropMode==2) {
				d_mode = DROPEFFECT_MOVE;	//�ړ�
			}
			else {
				d_mode = DROPEFFECT_COPY;	//�R�s�[
			}
		}

		if ((grfKeyState & (MK_LBUTTON|MK_RBUTTON)) == MK_RBUTTON)
			DroppedMode = DROPEFFECT_COPY| DROPEFFECT_MOVE| DROPEFFECT_LINK;
		else if ((grfKeyState & (MK_LBUTTON|MK_RBUTTON)) == (MK_LBUTTON|MK_RBUTTON))
			DroppedMode = DROPEFFECT_NONE;
		else
			DroppedMode = d_mode;
	}
	//���̑�
	else if (hWnd) {
		if (USAME_TI(get_WndClassName(hWnd), "EDIT")) hWnd = ::GetParent(hWnd);	//�R���{�{�b�N�X�΍�

		TPoint mp = Mouse->CursorPos;
		for (int i=0; i<TargetList->Count; i++) {
			drop_target_rec *dp = TargetList->Items[i];
			if (!dp->TargetForm->Visible || !dp->TargetForm->Enabled) continue;
			if (dp->TargetWinCtrl->Handle==hWnd) {
				TargetIndex = i;
				DroppedMode = d_mode = DROPEFFECT_COPY;
				break;
			}
		}
	}

	return d_mode;
}

//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::QueryInterface(REFIID riid, void **ppv)
{
	IUnknown *punk = NULL;
	if		(riid == IID_IUnknown)		punk = static_cast<IUnknown*>(this);
	else if (riid == IID_IDropSource)	punk = static_cast<IDropSource*>(this);

	*ppv = punk;
	if (punk) {
		punk->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropSource::AddRef()
{
	InterlockedIncrement(&FRefCount);
	return (ULONG)FRefCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropSource::Release()
{
	ULONG ret = (ULONG)InterlockedDecrement(&FRefCount);
	if (ret==0) delete this;
	return ret;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::QueryContinueDrag(BOOL fEsc, DWORD grfKeyState)
{
	//ESC�A�܂��͗����̃{�^���������ꂽ�璆�~
	if (fEsc || (grfKeyState & (MK_LBUTTON|MK_RBUTTON))==(MK_LBUTTON|MK_RBUTTON))
		return DRAGDROP_S_CANCEL;
	//�}�E�X�{�^���������ꂽ�ꍇ�̓h���b�v������
	if ((grfKeyState & (MK_LBUTTON|MK_RBUTTON))==0)
		return DRAGDROP_S_DROP;
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropSource::GiveFeedback(DWORD dwEffect)
{
	return DRAGDROP_S_USEDEFAULTCURSORS;
}

//---------------------------------------------------------------------------
// TDropTarget �N���X
//---------------------------------------------------------------------------
TDropTargetBase::TDropTargetBase()
{
	refCount = 0;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTargetBase::QueryInterface(const IID& iid, void **ppv)
{
	if (IsEqualIID(iid, IID_IUnknown) || IsEqualIID(iid, IID_IDropTarget)) {
		*ppv = static_cast<IDropTarget *>(this);
	}
	else {
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropTargetBase::AddRef()
{
	refCount++;
	return refCount;
}
//---------------------------------------------------------------------------
ULONG __stdcall TDropTargetBase::Release()
{
	refCount--;
	if (refCount==0) {
		delete this;
		return 0;
	}

	return refCount;
}

//---------------------------------------------------------------------------
// TDropTarget �N���X
//---------------------------------------------------------------------------
void TDropTarget::CreateInstance(IDropTarget **pp)
{
	if (pp) {
		TDropTarget *p = new TDropTarget();
		p->QueryInterface(IID_IDropTarget, (void **)pp);
	}
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragEnter(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	DragDrive	= EmptyStr;
	DropRefused = false;
	*pdwEffect	= get_DropMode(pDataObj, grfKeyState, pt);
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragOver(DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	*pdwEffect = get_DropMode(NULL, grfKeyState, pt);
	return (*pdwEffect==DROPEFFECT_NONE)? DRAGDROP_S_CANCEL : S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::DragLeave()
{
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT __stdcall TDropTarget::Drop(IDataObject *pDataObj, DWORD grfKeyState, POINTL pt, DWORD *pdwEffect)
{
	DragDrive	= EmptyStr;
	DropRefused = false;
	if (!DroppedList) return E_FAIL;
	DroppedList->Clear();
	if (DropRefused)  return E_FAIL;

	FORMATETC formatetc;
	formatetc.cfFormat = CF_HDROP;
	formatetc.ptd	   = NULL;
	formatetc.dwAspect = DVASPECT_CONTENT;
	formatetc.lindex   = -1;
	formatetc.tymed    = TYMED_HGLOBAL;
	STGMEDIUM medium;

	//�h���b�v���ꂽ�t�@�C���̃��X�g���쐬
	if (pDataObj->QueryGetData(&formatetc)==S_OK) {
		HRESULT hr = pDataObj->GetData(&formatetc, &medium);	if (FAILED(hr)) return hr;
		HDROP dp = (HDROP)::GlobalLock(medium.hGlobal);
		UINT f_cnt = ::DragQueryFile(dp, 0xFFFFFFFF, NULL, 0);
		UnicodeString fnam;
		for (UINT i=0; i<f_cnt; i++) {
			fnam.SetLength(MAX_PATH);
			int len = ::DragQueryFile(dp, i, fnam.c_str(),fnam.Length());
			fnam.SetLength(len);
			if (faDirectory & FileGetAttr(fnam)) fnam = IncludeTrailingPathDelimiter(fnam);
			DroppedList->Add(fnam);
		}
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
	}
	//�����N����
	else {
		//�^�C�g��
		UnicodeString fnam;
		formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_FILEDESCRIPTORW);
		HRESULT hr = pDataObj->QueryGetData(&formatetc);	if (FAILED(hr)) return hr;
		hr = pDataObj->GetData(&formatetc, &medium);		if (FAILED(hr)) return hr;
		FILEGROUPDESCRIPTOR *fg = (FILEGROUPDESCRIPTOR*)::GlobalLock(medium.hGlobal);
		if (fg->cItems>0) fnam = fg->fgd[0].cFileName;
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);
		//URL
		UnicodeString url;
		formatetc.cfFormat = ::RegisterClipboardFormat(CFSTR_INETURL);
		hr = pDataObj->QueryGetData(&formatetc);			if (FAILED(hr)) return hr;
		hr = pDataObj->GetData(&formatetc, &medium);		if (FAILED(hr)) return hr;
		url = (LPTSTR)::GlobalLock(medium.hGlobal);
		::GlobalUnlock(medium.hGlobal);
		ReleaseStgMedium(&medium);

		if (!fnam.IsEmpty() && !url.IsEmpty()) {
			//�摜�͂��̂܂܁A����ȊO�̓C���^�[�l�b�g�V���[�g�J�b�g
			if (!test_FileExt(get_extension(get_tkn(url, '?')), FEXT_WEBIMG)) fnam = ChangeFileExt(fnam, ".url");
			DroppedList->Add(UnicodeString().sprintf(_T("%s\t%s"), fnam.c_str(), url.c_str()));
			DroppedMode = DROPEFFECT_LINK;
		}
	}

	//�h���b�v������v��
	if (DroppedList && DroppedList->Count>0) {
		//�t�@�C�����X�g
		if (TargetIndex==-1) {
			::SendMessage(TargetHandle, WM_FORM_DROPPED, 0, 0);
		}
		//���̑�
		else if (TargetIndex>=0 && TargetIndex<TargetList->Count) {
			drop_target_rec *dp = TargetList->Items[TargetIndex];
			::SendMessage(dp->hTargetWnd, WM_FORM_DROPPED, (WPARAM)dp->TargetWinCtrl, 0);
		}
	}
	return S_OK;
}

//---------------------------------------------------------------------------
//�h���b�v�^�[�Q�b�g�E���X�g (TList ����p��)
//---------------------------------------------------------------------------
__fastcall TDropTargetList::TDropTargetList(): TList()
{
}
//---------------------------------------------------------------------------
void __fastcall TDropTargetList::Notify(void *Ptr, TListNotification Action)
{
	if (Action==lnDeleted) delete (drop_target_rec*)Ptr;
}

//---------------------------------------------------------------------------
//�o�^���ڂ̐V�K�ǉ�
//---------------------------------------------------------------------------
void __fastcall TDropTargetList::AddNew(TForm *form, TWinControl *ctrl)
{
	drop_target_rec *dp = new drop_target_rec;
	dp->TargetForm	  = form;
	dp->TargetWinCtrl = ctrl;
	dp->hTargetWnd	  = form->Handle;

	Add(dp);
}

//---------------------------------------------------------------------------
//UserShell �N���X
//---------------------------------------------------------------------------
UserShell::UserShell(HWND hWnd)
{
	::OleInitialize(NULL);

	TargetHandle = hWnd;
	ScrScale	 = 1.0;

	//���������_��O�̗}�~�}�X�N
	FpuTmpMask<<exInvalidOp<<exDenormalized<<exZeroDivide<<exOverflow<<exUnderflow<<exPrecision;

	//�v���p�e�B�̍��ڃ��X�g���쐬
	PropertyList  = new TStringList();
	PropNameWidth = 15;

	TComInterface<IShellFolder>  desktop;
	TComInterface<IShellFolder2> desktop2;
	_TCHAR szBuf[256];
	SHELLDETAILS details;
	try {
		if (FAILED(SHGetDesktopFolder(&desktop))) Abort();
		if (FAILED(desktop->QueryInterface(IID_IShellFolder2, (void **)&desktop2))) Abort();
		for (int i=0; i<400; i++) {
			if (FAILED(desktop2->GetDetailsOf(NULL, i, &details))) continue;
			StrRetToBuf(&details.str, NULL, szBuf, sizeof(szBuf)/sizeof(_TCHAR));
			UnicodeString tstr = szBuf;
			if (!tstr.IsEmpty()) PropertyList->Add(UnicodeString().sprintf(_T("%s=%u"), tstr.c_str(), i));
		}
	}
	catch (...) {
		PropertyList->Clear();
	}

	DragStartTag = -1;
	DroppedList  = new TStringList();

	TargetIndex  = -1;
	TargetList	 = new TDropTargetList();

	//�h���b�v�^�[�Q�b�g��o�^
	IDropTarget *pDropTarget = NULL;
	TDropTarget::CreateInstance(&pDropTarget);
	if (pDropTarget) {
		::RegisterDragDrop(TargetHandle, pDropTarget);
		pDropTarget->Release();
	}
}
//---------------------------------------------------------------------------
UserShell::~UserShell()
{
	::RevokeDragDrop(TargetHandle);

	delete PropertyList;
	delete DroppedList;
	delete TargetList;

	::OleUninitialize();
}

//---------------------------------------------------------------------------
//�h���b�v�^�[�Q�b�g��ǉ�
//---------------------------------------------------------------------------
void UserShell::AddTargetList(TForm *form, TWinControl *ctrl)
{
	TargetList->AddNew(form, ctrl);
}
//---------------------------------------------------------------------------
//�w��t�H�[���̃h���b�v�^�[�Q�b�g���폜
//---------------------------------------------------------------------------
void UserShell::DelTargetItem(TForm *form)
{
	int i = 0;
	while(i < TargetList->Count) {
		drop_target_rec *dp = TargetList->Items[i];
		if (dp->TargetForm==form)
			TargetList->Delete(i);
		else
			i++;
	}
}

//---------------------------------------------------------------------------
// �t�@�C�����X�g����IDataObject�C���^�[�t�F�C�X���擾
//---------------------------------------------------------------------------
IDataObject* UserShell::GetFilePathDataObject(UnicodeString dnam, TStringList *flist)
{
	if (flist->Count==0) return NULL;

	//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
	TComInterface<IShellFolder> desktop;
	if (FAILED(SHGetDesktopFolder(&desktop))) return NULL;

	//�Ώۃt�H���_�̃A�C�e��ID
	ITEMIDLIST *dir_pidl;
	if (FAILED(desktop->ParseDisplayName(NULL, NULL, dnam.c_str(), NULL, &dir_pidl, NULL))) return NULL;

	//�t�H���_��IShellFolder �C���^�[�t�F�C�X���擾
	TComInterface<IShellFolder> targetFolder;
	if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder, (void**)&targetFolder))) return NULL;

	//�Ώۃt�@�C���̃A�C�e��ID���X�g���쐬
	LPITEMIDLIST *p_pIDL = (LPITEMIDLIST*)CoTaskMemAlloc(sizeof(LPITEMIDLIST) * flist->Count);
	if (!p_pIDL) return NULL;
	for (int i=0; i<flist->Count; i++) {
		if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
			flist->Strings[i].c_str(), NULL, &p_pIDL[i], NULL)))
				return NULL;
	}

	//IDataObject �C���^�[�t�F�C�X���擾
	IDataObject *dataObject = NULL;
	targetFolder->GetUIObjectOf(
		NULL, flist->Count, (LPCITEMIDLIST*)p_pIDL, IID_IDataObject, NULL, (void**)&dataObject);
	return dataObject;
}

//---------------------------------------------------------------------------
//�h���b�O���h���b�v�̊J�n
//---------------------------------------------------------------------------
DWORD UserShell::DoFilesDragDrop(UnicodeString dnam, TStringList *flist, int tag)
{
	IDataObject *dataObject = GetFilePathDataObject(dnam, flist);
	if (!dataObject) return 0;

	DragStartTag = tag;

	DWORD dropEffect;
	TDropSource *dropSource = new TDropSource();
	dropSource->AddRef();
	::DoDragDrop(dataObject, dropSource, DROPEFFECT_COPY|DROPEFFECT_MOVE|DROPEFFECT_LINK, &dropEffect);

	dataObject->Release();
	dropSource->Release();

	return dropEffect;
}
//---------------------------------------------------------------------------
DWORD UserShell::DoClipDragDrop()
{
	IDataObject *dataObject;
	::OleGetClipboard(&dataObject);
	if (!dataObject) return 0;

	DWORD dropEffect;
	TDropSource *dropSource = new TDropSource();
	dropSource->AddRef();
	::DoDragDrop(dataObject, dropSource, DROPEFFECT_COPY, &dropEffect);

	dataObject->Release();
	dropSource->Release();

	return dropEffect;
}

//---------------------------------------------------------------------------
//�R���e�L�X�g���j���[��\��
//  �߂�l: ���s�̏ꍇ "ERROR"�A�ǉ����ڂȂ炻�̕�����A����ȊO�� EmptyStr
//�I���C������ WM_INITMENUPOPUP,WM_DRAWITEM,WM_MEASUREITEM,WM_MENUCHAR ���������邱��
//---------------------------------------------------------------------------
UnicodeString UserShell::ShowContextMenu(
	HWND hWnd,
	UnicodeString dnam,		//�f�B���N�g����
	TStringList *flist,		//�Ώۃ��X�g
	TStringList *ex_item)	//�ǉ����j���[���ڃ��X�g
							//  ����: �L���v�V���� \t �R�}���h \t �A�C�R��
{
	UnicodeString ret_str;
	ITEMIDLIST *dir_pidl = NULL;

	//���������_��O���ꎞ�I�Ƀ}�X�N
	TFPUExceptionMask orgMask = GetExceptionMask();
	SetExceptionMask(FpuTmpMask);

	try {
		try {
			if (flist->Count==0) Abort();

			//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL, dnam.c_str(), NULL, &dir_pidl, NULL))) Abort();

			//�t�H���_��IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder, (void**)&targetFolder))) Abort();

			//�Ώۃt�@�C���̃A�C�e��ID���X�g���쐬
			LPITEMIDLIST *p_pIDL = (LPITEMIDLIST*)CoTaskMemAlloc(sizeof(LPITEMIDLIST) * flist->Count);
			if (!p_pIDL) Abort();
			for (int i=0; i<flist->Count; i++) {
				if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
					flist->Strings[i].c_str(), NULL, &p_pIDL[i], NULL)))
						Abort();
			}

			//�R���e�L�X�g���j���[���擾
			TComInterface<IContextMenu> contextMenu;
			if (FAILED(targetFolder->GetUIObjectOf(hWnd, flist->Count,
				(LPCITEMIDLIST*)p_pIDL, IID_IContextMenu, NULL, (void **)&contextMenu)))
					Abort();

			contextMenu->QueryInterface(IID_PPV_ARGS(&ContextMenu2));
			contextMenu->QueryInterface(IID_PPV_ARGS(&ContextMenu3));

			LPCONTEXTMENU pCM;
			if		(ContextMenu3) pCM = ContextMenu3;
			else if (ContextMenu2) pCM = ContextMenu2;
			else				   pCM = contextMenu;

			std::unique_ptr<TList> m_lst(new TList());	//���j���[�n���h���̃��X�g(�j���p)
			std::unique_ptr<TList> s_lst(new TList());	//�T�u���j���[�̊K�w���X�g

			HMENU hMenu = ::CreatePopupMenu();
			m_lst->Add(hMenu);
			s_lst->Add(hMenu);

			if (SUCCEEDED(pCM->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
				//�ǉ����ڂ̑}��
				if (ex_item && ex_item->Count>0) {
					//�Ή��g���q�̃��X�g���쐬
					std::unique_ptr<TStringList> xlst(new TStringList());
					for (int i=0; i<flist->Count; i++) {
						if (dir_exists(dnam + flist->Strings[i])) {
							if (xlst->IndexOf("\\")==-1) xlst->Add("\\");
						}
						else {
							UnicodeString fext = get_extension(flist->Strings[i]);
							if		(fext.IsEmpty()) { xlst->Clear(); break; }
							else if (xlst->IndexOf(fext)==-1) xlst->Add(fext);
						}
					}

					MENUITEMINFO mii;
					::ZeroMemory(&mii, sizeof(MENUITEMINFO));
					mii.cbSize = sizeof(mii);

					HMENU hMP = hMenu;
					bool ins_top = false;
					int  pos_top = 0;
					for (int i=0; i<ex_item->Count; i++) {
						UnicodeString lbuf = Trim(ex_item->Strings[i]);
						if (lbuf.IsEmpty() || StartsStr(';', lbuf)) continue;

						if (USAME_TS(lbuf, "!")) {
							ins_top = true;
							pos_top = 0;
							continue;
						}

						int pos_last = std::max(::GetMenuItemCount(hMP) - ((s_lst->Count==1)? 2 : 0), 0);
						int pos = (!ins_top || (i==0 && USAME_TI(lbuf, "���O�̕ύX(&M)\tRenameDlg")))? pos_last : pos_top;

						//�T�u���j���[
						if (remove_top_s(lbuf, '>')) {
							HMENU hSub = ::CreatePopupMenu();
							m_lst->Add(hSub);
							mii.fMask	   = MIIM_TYPE|MIIM_SUBMENU|MIIM_ID;
							mii.fType	   = MFT_STRING;
							mii.wID 	   = USR_NENUID_BASE + i;
							mii.hSubMenu   = hSub;
							mii.dwTypeData = lbuf.c_str();
							mii.cch 	   = _tcslen(mii.dwTypeData);
							::InsertMenuItem(hMP, pos, TRUE, &mii);
							pos_top++;
							hMP = hSub;
							s_lst->Add(hSub);
						}
						else if (StartsStr('<', lbuf)) {
							if (s_lst->Count>1) {
								s_lst->Delete(s_lst->Count - 1);
								hMP = (HMENU)s_lst->Items[s_lst->Count - 1];
							}
							else hMP = hMenu;
						}
						//����
						else {
							TStringDynArray m_buf = split_strings_tab(lbuf);
							UnicodeString mext = Trim(m_buf[0]);
							UnicodeString cap  = split_tkn(mext, '|');

							//�Ή��g���q�̃`�F�b�N
							if (!mext.IsEmpty()) {
								bool ok = true;
								for (int j=0; j<xlst->Count && ok; j++) {
									if (!test_FileExt(xlst->Strings[j], mext)) ok = false;
								}
								if (!ok) continue;
							}

							mii.fMask	   = MIIM_TYPE|MIIM_ID;
							mii.fType	   = is_separator(cap)? MFT_SEPARATOR : MFT_STRING;
							mii.wID 	   = USR_NENUID_BASE + i;
							mii.hSubMenu   = NULL;
							mii.dwTypeData = cap.c_str();
							mii.cch 	   = _tcslen(mii.dwTypeData);
							::InsertMenuItem(hMP, pos, TRUE, &mii);
							pos_top++;
							//�A�C�R���̐ݒ�
							if (m_buf.Length==3) {
								SHFILEINFO sif;
								if (::SHGetFileInfo(m_buf[2].c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_SMALLICON)) {
									BITMAPINFOHEADER bmihdr;
									::ZeroMemory(&bmihdr, sizeof(BITMAPINFOHEADER));
									BITMAPINFO bmi;
									bmihdr.biSize	  = sizeof(BITMAPINFOHEADER);
									bmihdr.biWidth	  = 16;
									bmihdr.biHeight   = 16;
									bmihdr.biPlanes   = 1;
									bmihdr.biBitCount = 32;
									bmi.bmiHeader = bmihdr;
									LPVOID  lpBits;
									HBITMAP hBmp = ::CreateDIBSection(NULL, (LPBITMAPINFO)&bmi, DIB_RGB_COLORS, &lpBits, NULL, 0);
									HDC hdcMem = ::CreateCompatibleDC(NULL);
									HBITMAP hBmpPrev = (HBITMAP)::SelectObject(hdcMem, hBmp);
									::DrawIconEx(hdcMem, 0, 0, sif.hIcon, 16, 16, 0, NULL, DI_NORMAL);
									::SelectObject(hdcMem, hBmpPrev);
									::DeleteDC(hdcMem);
									mii.fMask = MIIM_BITMAP;
									mii.hbmpItem = hBmp;
									::SetMenuItemInfo(hMP, mii.wID, FALSE, &mii);
								}
							}
						}
					}
				}

				//�\��
				POINT pt;
				::GetCursorPos(&pt);
				int nId = ::TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, NULL);
				//�ǉ�����
				if (nId>=USR_NENUID_BASE) {
					ret_str = Trim(ex_item->Strings[nId - USR_NENUID_BASE]);
				}
				//�ʏ퍀��
				else if (nId!=0) {
					CMINVOKECOMMANDINFO ici;
					::ZeroMemory(&ici, sizeof(CMINVOKECOMMANDINFO));
					ici.cbSize		 = sizeof(CMINVOKECOMMANDINFO);
					ici.hwnd		 = hWnd;
					ici.lpVerb		 = (LPCSTR)MAKEINTRESOURCE(nId - 1);
					ici.nShow		 = SW_SHOW;
					if (FAILED(pCM->InvokeCommand(&ici))) ret_str = "ERROR";
				}
			}

			if (ContextMenu2) { ContextMenu2->Release();	ContextMenu2 = NULL; };
			if (ContextMenu3) { ContextMenu3->Release();	ContextMenu3 = NULL; };

			for (int i=0; i<m_lst->Count; i++) ::DestroyMenu((HMENU)m_lst->Items[i]);
		}
		catch (...) {
			ret_str = "ERROR";
		}
	}
	__finally {
		SetExceptionMask(orgMask);
		::ILFree(dir_pidl);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�h���C�u�̃R���e�L�X�g���j���[��\��/�w��ID�����s
//  �߂�l: ���s�̏ꍇ "ERROR"�A�ǉ����ڂȂ炻�̕�����A����ȊO�� EmptyStr
//�I���C������ WM_INITMENUPOPUP,WM_DRAWITEM,WM_MEASUREITEM,WM_MENUCHAR ���������邱��
//---------------------------------------------------------------------------
UnicodeString UserShell::DriveContextMenu(
	HWND hWnd,
	UnicodeString drvnam,	//�h���C�u��
	TStringList *ex_item,	//�ǉ����j���[���ڃ��X�g
							// ����: �L���v�V���� \t �R�}���h (�T�u���j���[�͔�Ή�)
	int ex_ID)				//�R�}���hID:
							// >0 �̏ꍇ���j���[��\�������� ID �����s (default = 0)
{
	drvnam = ExtractFileDrive(drvnam);
	if (drvnam.IsEmpty()) return EmptyStr;
	drvnam = IncludeTrailingPathDelimiter(drvnam);

	UnicodeString ret_str;

	ITEMIDLIST  *pidlComputer = NULL;
	ITEMIDLIST  *pidlEnum	  = NULL;

	try {
		try {
			//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			TComInterface<IShellFolder> computer;
			if (FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidlComputer))) Abort();
			if (FAILED(desktop->BindToObject(pidlComputer, NULL, IID_IShellFolder, (void**)&computer))) Abort();

			TComInterface<IEnumIDList> pEnum;
			if (FAILED(computer->EnumObjects(NULL, SHCONTF_FOLDERS, &pEnum))) Abort();
			_TCHAR szName[256];
			STRRET stFileName;
			ULONG ulFetched = 0;
			while (pEnum->Next(1, &pidlEnum, &ulFetched)==NOERROR) {
				computer->GetDisplayNameOf(pidlEnum, SHGDN_FORPARSING, &stFileName);
				StrRetToBuf(&stFileName, pidlEnum, szName, sizeof(szName) / sizeof(_TCHAR));
				if (SameText(szName, drvnam)) break;
				::ILFree(pidlEnum);
			}

			//�R���e�L�X�g���j���[���擾
			TComInterface<IContextMenu> contextMenu;
			if (FAILED(computer->GetUIObjectOf(NULL, 1,
				(LPCITEMIDLIST *)&pidlEnum, IID_IContextMenu, NULL, (void **)&contextMenu)))
					Abort();

			contextMenu->QueryInterface(IID_PPV_ARGS(&ContextMenu2));
			contextMenu->QueryInterface(IID_PPV_ARGS(&ContextMenu3));

			LPCONTEXTMENU pCM;
			if		(ContextMenu3) pCM = ContextMenu3;
			else if (ContextMenu2) pCM = ContextMenu2;
			else				   pCM = contextMenu;

			HMENU hMenu = ::CreatePopupMenu();
			if (SUCCEEDED(pCM->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
				//�ǉ����ڂ̑}��
				if (ex_item && ex_item->Count>0) {
					MENUITEMINFO mii;
					::ZeroMemory(&mii, sizeof(MENUITEMINFO));
					mii.cbSize = sizeof(mii);

					for (int i=0; i<ex_item->Count; i++) {
						UnicodeString lbuf = Trim(ex_item->Strings[i]);
						if (lbuf.IsEmpty() || StartsStr(';', lbuf)) continue;
						TStringDynArray m_buf = split_strings_tab(lbuf);
						UnicodeString cap = Trim(m_buf[0]);
						mii.fMask	   = MIIM_TYPE|MIIM_ID;
						mii.fType	   = is_separator(cap)? MFT_SEPARATOR : MFT_STRING;
						mii.wID 	   = USR_NENUID_BASE + i;
						mii.hSubMenu   = NULL;
						mii.dwTypeData = cap.c_str();
						mii.cch 	   = _tcslen(mii.dwTypeData);
						::InsertMenuItem(hMenu, ::GetMenuItemCount(hMenu) - 2, TRUE, &mii);
					}
				}

				int nId = (ex_ID>0)? ex_ID + 1 : 0;
				if (nId==0) {
					//���j���[��\��
					POINT pt;
					::GetCursorPos(&pt);
					nId = ::TrackPopupMenu(hMenu, TPM_LEFTALIGN|TPM_RETURNCMD|TPM_RIGHTBUTTON,
											pt.x, pt.y, 0, hWnd, NULL);
				}

				//�ǉ�����
				if (nId>=USR_NENUID_BASE) {
					ret_str = Trim(ex_item->Strings[nId - USR_NENUID_BASE]);
				}
				//�ʏ퍀��
				else if (nId!=0) {
					CMINVOKECOMMANDINFO ici;
					::ZeroMemory(&ici, sizeof(CMINVOKECOMMANDINFO));
					ici.cbSize		 = sizeof(CMINVOKECOMMANDINFO);
					ici.hwnd		 = hWnd;
					ici.lpVerb		 = (LPCSTR)MAKEINTRESOURCE(nId - 1);
					ici.nShow		 = SW_SHOW;
					if (FAILED(pCM->InvokeCommand(&ici))) ret_str = "ERROR";
				}
			}

			if (ContextMenu2) { ContextMenu2->Release();	ContextMenu2 = NULL; };
			if (ContextMenu3) { ContextMenu3->Release();	ContextMenu3 = NULL; };

			::DestroyMenu(hMenu);
		}
		catch (...) {
			ret_str = "ERROR";
		}
	}
	__finally {
		::ILFree(pidlComputer);
		::ILFree(pidlEnum);
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//Copy/ Cut �Ȃǂ����s
//---------------------------------------------------------------------------
bool UserShell::InvokeMenuCmd(HWND hWnd, UnicodeString dnam, TStringList *flist, LPCSTR cmd)
{
	bool ok = false;
	ITEMIDLIST *dir_pidl = NULL;

	try {
		try {
			if (flist->Count==0) Abort();

			//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL, dnam.c_str(), NULL, &dir_pidl, NULL))) Abort();

			//�t�H���_��IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder, (void**)&targetFolder))) Abort();

			//�Ώۃt�@�C���̃A�C�e��ID���X�g���쐬
			LPITEMIDLIST *p_pIDL = (LPITEMIDLIST*)CoTaskMemAlloc(sizeof(LPITEMIDLIST) * flist->Count);
			if (!p_pIDL) Abort();
			for (int i=0; i<flist->Count; i++) {
				if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
					flist->Strings[i].c_str(), NULL, &p_pIDL[i], NULL)))
						Abort();
			}

			//�R���e�L�X�g���j���[���擾
			TComInterface<IContextMenu> contextMenu;
			if (FAILED(targetFolder->GetUIObjectOf(hWnd, flist->Count,
				(LPCITEMIDLIST*)p_pIDL, IID_IContextMenu, NULL, (void **)&contextMenu)))
					Abort();

			HMENU hMenu = ::CreatePopupMenu();
			if (SUCCEEDED(contextMenu->QueryContextMenu(hMenu, 0, 1, 0x7fff, CMF_EXPLORE))) {
				CMINVOKECOMMANDINFO ici;
				::ZeroMemory(&ici, sizeof(CMINVOKECOMMANDINFO));
				ici.cbSize		 = sizeof(CMINVOKECOMMANDINFO);
				ici.hwnd		 = hWnd;
				ici.lpVerb		 = cmd;
				ici.nShow		 = SW_SHOW;
				ok = SUCCEEDED(contextMenu->InvokeCommand(&ici));
			}
			::DestroyMenu(hMenu);
		}
		catch (...) {
			;
		}
	}
	__finally {
		::ILFree(dir_pidl);
	}
	return ok;
}

//---------------------------------------------------------------------------
// �t�@�C�������w�肵�Ď��s
//---------------------------------------------------------------------------
bool UserShell::ShowFileRun()
{
	bool res = false;
	TComInterface<IShellDispatch> dispatch;
	if (::CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC, IID_IShellDispatch, (void **)&dispatch)==S_OK) {
		dispatch->FileRun();
		res = true;
	}
	return res;
}

//---------------------------------------------------------------------------
//�V���[�g�J�b�g�̍쐬
//---------------------------------------------------------------------------
bool UserShell::CreateShortCut(UnicodeString fnam, UnicodeString target, UnicodeString cmdprm,
	UnicodeString wdir, UnicodeString desc)
{
	bool res;

	try {
		if (wdir.IsEmpty() && !::PathIsDirectory(target.c_str())) wdir = ExtractFilePath(target);
		if (desc.IsEmpty()) desc = ExtractFileName(target) + "�ւ̃V���[�g�J�b�g";

		TComInterface<IShellLink>   psl;
		TComInterface<IPersistFile> ppf;
		if (FAILED(::CoCreateInstance(CLSID_ShellLink, NULL,
			CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl)))
				Abort();
		if (FAILED(psl->QueryInterface(IID_IPersistFile, (void **)&ppf))) Abort();

		psl->SetPath(target.c_str());
		psl->SetArguments(cmdprm.c_str());
		if (!wdir.IsEmpty()) psl->SetWorkingDirectory(wdir.c_str());
		psl->SetDescription(desc.c_str());
		res = SUCCEEDED(ppf->Save(fnam.c_str(), TRUE));
	}
	catch (...) {
		res = false;
	}

	return res;
}

//---------------------------------------------------------------------------
//�v���p�e�B�����擾
//  �߂�l: �F�����ꂽ���
//---------------------------------------------------------------------------
UnicodeString UserShell::get_PropInf(
	UnicodeString fnam,		//�t�@�C����
	TStringList  *lst,		//��񃊃X�g (default = NULL)
	UnicodeString idx_str,	//�v���p�e�B�� (, ��؂�ŕ����w��� default = EmptyStr)
	bool lst_sw)			//���O=�l�`���Ŏ擾 (default = false);
{
	UnicodeString typ_str;
	ITEMIDLIST *dir_pidl = NULL;
	ITEMIDLIST *object	 = NULL;

	//���������_��O���ꎞ�I�Ƀ}�X�N
	TFPUExceptionMask orgMask = GetExceptionMask();
	SetExceptionMask(FpuTmpMask);

	try {
		try {
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL,
				ExtractFileDir(fnam).c_str(), NULL, &dir_pidl, NULL)))
					Abort();
			//�t�H���_��IShellFolder2 �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder2> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder2, (void**)&targetFolder))) Abort();
			//�Ώۃt�@�C���̃A�C�e��ID
			if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
				ExtractFileName(fnam).c_str(), NULL, &object, NULL)))
					Abort();

			//�u�F�����ꂽ��ށv���擾
			SHELLDETAILS details;
			_TCHAR szBuf[256];
			int idx = get_ListIntVal(PropertyList, _T("�F�����ꂽ���"), -1);
			if (idx!=-1) {
				if (SUCCEEDED(targetFolder->GetDetailsOf(object, idx, &details))) {
					StrRetToBuf(&details.str, NULL, szBuf, sizeof(szBuf)/sizeof(_TCHAR));
					typ_str = szBuf;
					if (USAME_TI(typ_str, "���w��")) {
						if (USAME_TI(ExtractFileExt(fnam), ".ts")) typ_str = "�r�f�I";
					}
				}
			}

			if (idx_str.IsEmpty()) {
				//�u�F�����ꂽ��ށv�ɂ��������Ď擾���ڂ�I��
				switch (idx_of_word_i(_T("�r�f�I|�I�[�f�B�I|�C���[�W|�A�v���P�[�V����|�h�L�������g|���w��"), typ_str)) {
				case 0:
					idx_str = "�^�C�g��,����,�t���[����,�t���[����,$R,�f�[�^���x,���r�b�g ���[�g,�t���[����,$A,���쌠";
					break;
				case 1:
					idx_str = "����,�r�b�g ���[�g,���쌠";
					break;
				case 2:
					idx_str = "�r�b�g�̐[��";
					if (!USAME_TI(ExtractFileExt(fnam), ".png")) idx_str += ",���������̉𑜓x,���������̉𑜓x";
					idx_str += ",�v���O������";
					break;
				case 3:
					idx_str = "�t�@�C���̐���,�t�@�C�� �o�[�W����,���i��,���i�o�[�W����,���쌠,�t�@�C����";
					break;
				case 4:
					idx_str = "�^�C�g��,�쐬��,���,�v���O������";
					break;
				case 5:
					idx_str = "�^�C�g��,�쐬��,���,���쌠,���W,�t�@�C�� �o�[�W����";
					break;
				}
			}

			//���ړ��e���擾
			if (lst && !idx_str.IsEmpty()) {
				TStringDynArray i_lst = SplitString(idx_str, ",");
				UnicodeString tmp;
				int f_wd=0, f_hi=0;
				for (int i=0; i<i_lst.Length; i++) {
					UnicodeString inam = i_lst[i];
					if (starts_Dollar(inam)) { lst->Add(inam); continue; }
					UnicodeString istr = PropertyList->Values[inam];	if (istr.IsEmpty()) continue;
					int idx = istr.ToIntDef(-1);	if (idx==-1) continue;
					if (FAILED(targetFolder->GetDetailsOf(object, idx, &details))) continue;
					StrRetToBuf(&details.str, NULL, szBuf, sizeof(szBuf)/sizeof(_TCHAR));
					UnicodeString vstr = szBuf;	if (vstr.IsEmpty()) continue;

					if		(USAME_TS(inam, "�t���[����")) f_wd = extract_int(vstr);
					else if (USAME_TS(inam, "�t���[����")) f_hi = extract_int(vstr);

					if (lst_sw)
						lst->Add(tmp.sprintf(_T("%s=%s"), inam.c_str(), vstr.c_str()));
					else {
						tmp = ReplaceStr(inam, " ", "");
						tmp = ReplaceStr(tmp,  "�o�[�W����", "Ver.");
						tmp = ReplaceStr(tmp, "�t�@�C���̐���", "����");
						if (EndsStr("�𑜓x", tmp) && extract_int(vstr)>0x10000) continue;	//*** �ُ�l
						if (tmp.Length()>(PropNameWidth/2)) remove_text(tmp, "��");
						add_PropLine(tmp, vstr, lst);
					}
				}

				//�A�X�y�N�g��
				int idx = lst->IndexOf("$R");
				if (idx!=-1) {
					if (f_wd>0 && f_hi>0) lst->Strings[idx] = make_PropLine(_T("�A�X�y�N�g��"), get_AspectStr(f_wd, f_hi));
					else lst->Delete(idx);
				}

				//�I�[�f�B�I
				idx = lst->IndexOf("$A");
				if (idx!=-1) {
					UnicodeString vstr, ustr;
					VARIANT varValue;	VariantInit(&varValue);
					SHCOLUMNID psid;
					//�r�b�g���[�g
					psid.fmtid = Sysutils::StringToGUID("{64440490-4c8b-11d1-8b70-080036b11a03}");
					psid.pid = 4;
					if (FAILED(targetFolder->GetDetailsEx(object, &psid, &varValue))) Abort();
					int v = varValue.lVal;
					if (v>0) {
						ustr = "bps";
						if (v>1000) { v /= 1000; ustr.Insert("k" ,1); }
						if (!vstr.IsEmpty()) vstr += " ";
						vstr.cat_sprintf(_T("%u%s"), v, ustr.c_str());
					}
					//�T���v�����[�g
					psid.pid = 5;
					if (FAILED(targetFolder->GetDetailsEx(object, &psid, &varValue))) Abort();
					v = varValue.lVal;
					if (v>0) {
						ustr = "Hz";
						if (v>1000) { v /= 1000; ustr.Insert("k" ,1); }
						vstr.cat_sprintf(_T(" %u%s"), v, ustr.c_str());
					}
					//�`�����l��
					psid.pid = 7;
					if (FAILED(targetFolder->GetDetailsEx(object, &psid, &varValue))) Abort();
					switch (varValue.lVal) {
					case 1:  vstr += " ���m";		break;
					case 2:  vstr += " �X�e���I";	break;
					default: vstr.cat_sprintf(_T(" %uch"), varValue.lVal);
					}

					if (!vstr.IsEmpty()) lst->Strings[idx] = make_PropLine(_T("�I�[�f�B�I"), vstr);
					else lst->Delete(idx);
				}
			}
		}
		catch (...) {
			typ_str = EmptyStr;
		}
	}
	__finally {
		SetExceptionMask(orgMask);
		::ILFree(dir_pidl);
		::ILFree(object);
	}

	return typ_str;
}

//---------------------------------------------------------------------------
//�w��v���p�e�B���̏�񕶎�����擾
//---------------------------------------------------------------------------
UnicodeString UserShell::get_PropStr(UnicodeString fnam, UnicodeString prp_nam)
{
	UnicodeString ret_str;
	ITEMIDLIST *dir_pidl = NULL;
	ITEMIDLIST *object	 = NULL;

	//���������_��O���ꎞ�I�Ƀ}�X�N
	TFPUExceptionMask orgMask = GetExceptionMask();
	SetExceptionMask(FpuTmpMask);

	try {
		try {
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL,
				ExtractFileDir(fnam).c_str(), NULL, &dir_pidl, NULL)))
					Abort();
			//�t�H���_��IShellFolder2 �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder2> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder2, (void**)&targetFolder))) Abort();
			//�Ώۃt�@�C���̃A�C�e��ID
			if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
				ExtractFileName(fnam).c_str(), NULL, &object, NULL)))
					Abort();

			_TCHAR szBuf[256];
			SHELLDETAILS details;
			int idx = PropertyList->Values[prp_nam].ToIntDef(0);	if (idx==0) Abort();
			if (FAILED(targetFolder->GetDetailsOf(object, idx, &details))) Abort();
			StrRetToBuf(&details.str, NULL, szBuf, sizeof(szBuf)/sizeof(_TCHAR));
			ret_str = szBuf;
		}
		catch (...) {
			ret_str = EmptyStr;
		}
	}
	__finally {
		SetExceptionMask(orgMask);
		::ILFree(dir_pidl);
		::ILFree(object);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//���i�o�[�W������������擾
//---------------------------------------------------------------------------
UnicodeString UserShell::get_VerStr(UnicodeString fnam)
{
	UnicodeString vstr = get_PropStr(fnam, "���i�o�[�W����");
	vstr = ReplaceStr(vstr, ",", ".");
	vstr = ReplaceStr(vstr, " ", "");
	return vstr;
}

//---------------------------------------------------------------------------
//���X�g����w��v���p�e�B�̒l���擾
//---------------------------------------------------------------------------
UnicodeString UserShell::get_PropValue(TStringList *lst, UnicodeString name)
{
	UnicodeString ret_str;
	UnicodeString nam_str = get_PropTitle(name);
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (remove_top_s(lbuf, nam_str)) {
			ret_str = lbuf; break;
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�t�@�C���Đ�����(�b)���擾
//---------------------------------------------------------------------------
int UserShell::get_Duration(UnicodeString fnam)
{
	int t = -1;
	UnicodeString lnstr = get_PropStr(fnam, "����");
	if (lnstr.Length()==8 && lnstr[3]==':' && lnstr[6]==':') {
		int h = lnstr.SubString(1, 2).ToIntDef(0);
		int m = lnstr.SubString(4, 2).ToIntDef(0);
		int s = lnstr.SubString(7, 2).ToIntDef(0);
		t = h*3600 + m*60 + s;
	}
	return t;
}

//---------------------------------------------------------------------------
unsigned int UserShell::msRead_uint(TMemoryStream *ms, int ofs)
{
	BYTE *bp = (BYTE*)ms->Memory + ofs;
	return (unsigned int)(bp[0] | (bp[1] << 8) | (bp[2] << 16) | (bp[3] << 24));
}
//---------------------------------------------------------------------------
unsigned short UserShell::msRead_ushort(TMemoryStream *ms, int ofs)
{
	BYTE *bp = (BYTE*)ms->Memory + ofs;
	return (unsigned short)(bp[0] | (bp[1] << 8));
}
//---------------------------------------------------------------------------
UnicodeString UserShell::msRead_strdat(TMemoryStream *ms, int &ptr, bool is_uc)
{
	UnicodeString ret_str;
	int ch_len = msRead_ushort(ms, ptr);
	if (ch_len>0) {
		ptr += 2;
		BYTE *bp = (BYTE*)ms->Memory + ptr;
		if (is_uc) {
			for (int i=0; i<ch_len; i++, bp+=2) ret_str.cat_sprintf(_T("%c"), *(_TCHAR*)bp);
		}
		else {
			for (int i=0; i<ch_len; i++,bp++)   ret_str.cat_sprintf(_T("%c"), *(char*)bp);
		}
		ptr += ch_len * (is_uc? 2 : 1);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//�t�@�C���̎�ނ��擾
//---------------------------------------------------------------------------
UnicodeString UserShell::get_FileTypeStr(UnicodeString fnam)
{
	std::unique_ptr<TStringList> typ_lst(new TStringList());
	typ_lst->Text =
		".exe=�A�v���P�[�V����\n"
		".lnk=�V���[�g�J�b�g\n"
		".url=�C���^�[�l�b�g �V���[�g�J�b�g\n"
		".wmf=Windows���^�t�@�C��\n"
		".emf=�g�����^�t�@�C��\n"
		".mp3=MP3 �`���T�E���h\n"
		".wav=Wave �T�E���h\n"
		".ttf=TrueType �t�H���g �t�@�C��\n"
		".ttc=TrueType �R���N�V���� �t�H���g �t�@�C��\n"
		".otf=OpenType �t�H���g �t�@�C��\n"
		".fon=�t�H���g �t�@�C��\n"
		".nwl=NyanFi ���[�N���X�g\n"
		".nbt=NyanFi �R�}���h�t�@�C��\n";

	UnicodeString typ_str;
	UnicodeString fext = get_extension(fnam).LowerCase();
	if (!fext.IsEmpty()) {
		typ_str = typ_lst->Values[fext];
		if (typ_str.IsEmpty()) {
			std::unique_ptr<TStringList> lst(new TStringList());
			usr_SH->get_PropInf(fnam, lst.get(), "���", true);
			if (lst->Count>0) typ_str = lst->ValueFromIndex[0];
			if (typ_str.IsEmpty() && remove_top_s(fext, '.')) typ_str.sprintf(_T("%s �t�@�C��"), fext.UpperCase().c_str());
		}
	}
	else if (USAME_TI(get_tkn(ExtractFileName(fnam), '_'), ".nyanfi")) {
		typ_str = ".nyanfi �t�@�C��";
	}
	else {
		typ_str = ExtractFileName(fnam) + " �t�@�C��";
	}

	return typ_str;
}

//---------------------------------------------------------------------------
//�V���[�g�J�b�g���̎擾
//---------------------------------------------------------------------------
bool UserShell::get_LnkInf(UnicodeString fnam, TStringList *lst,
	UnicodeString *nam, UnicodeString *prm, UnicodeString *fld, int *shw, UnicodeString *rem)
{
	try {
		std::unique_ptr<TFileStream>   fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
		TMemoryStream *msp = ms.get();
		msp->CopyFrom(fs.get(), fs->Size);

		//SHELL_LINK_HEADER
		//HeaderSize
		const int sh_ofs = 0x4c;
		if (msRead_uint(msp, 0)!=sh_ofs) Abort();

		//LinkCLSID
		std::unique_ptr<BYTE[]> xbuf(new BYTE[32]);
		msp->Seek(4, soFromBeginning);
		if (msp->Read(xbuf.get(), 16)!=16) Abort();
		const BYTE clsid[] = {0x01,0x14,0x02,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x46};
		if (memcmp(xbuf.get(), clsid, sizeof(clsid))!=0) Abort();

		//LinkFlags
		unsigned int flags = msRead_uint(msp, 20);
		bool has_id_list  = (flags & 0x0001);
		bool has_link_inf = (flags & 0x0002);
		bool has_name	  = (flags & 0x0004);
		bool has_rel_path = (flags & 0x0008);
		bool has_workdir  = (flags & 0x0010);
		bool has_argument = (flags & 0x0020);
		bool has_icon_loc = (flags & 0x0040);
		bool is_unicode   = (flags & 0x0080);
		if (flags & 0x0100) has_link_inf = false;
		bool has_exstr	  = (flags & 0x0101);

		//ShowCommand
		unsigned int show_cmd = msRead_uint(msp, 60);

		//HotKey
		BYTE *bp = (BYTE*)msp->Memory + 64;
		UnicodeString l_key = get_KeyStr(*bp);
		bp++;
		if (*bp & 0x04) l_key = KeyStr_Alt   + l_key;
		if (*bp & 0x02) l_key = KeyStr_Ctrl  + l_key;
		if (*bp & 0x01) l_key = KeyStr_Shift + l_key;

		UnicodeString pnam, l_nam;
		//LinkTargetIDList
		int id_len = 0;
		if (has_id_list) {
			id_len = msRead_ushort(msp, sh_ofs) + 2;
			int id_p = sh_ofs + 2;
			for (int i=0;;i++) {
				int len = msRead_ushort(msp, id_p);
				if (len==0) break;
				BYTE *bp = (BYTE*)msp->Memory + id_p;
				//�p�X��
				_TCHAR pstr[MAX_PATH + 1];
				if (i==0 && SHGetPathFromIDList((LPCITEMIDLIST)bp, pstr)) {
					pnam = pstr;
				}
				//GUID (�������͕s�m��)
				else if (len==30 && bp[2]==0x71 && bp[3]==0x80) {
					GUID *ip = (GUID *)&bp[14];
					RPC_WSTR ws;
				 	if (::UuidToString(ip, &ws)==RPC_S_OK) {
				 		pnam = (WCHAR*)ws;
						if (!pnam.IsEmpty()) pnam = "::{" + pnam.UpperCase() + "}";
  						::RpcStringFree(&ws);
  					}
				}

				if (!pnam.IsEmpty()) break;
				id_p += len;
				if (id_p>=(sh_ofs + id_len)) break;
			}
		}

		//LINKINFO
		int li_len = 0;
		if (has_link_inf) {
			int li_top = sh_ofs + id_len;
			li_len = msRead_uint(msp, li_top);
			int li_hdr_size = msRead_uint(msp, li_top + 0x04);
			int li_flags	= msRead_uint(msp, li_top + 0x08);
			UnicodeString lbuf;
			//Local
			if (li_flags & 0x01) {
				//Unicode
				if (li_hdr_size>=0x24) {
					BYTE *bp = (BYTE*)msp->Memory + (li_top + msRead_uint(msp, li_top + 0x1c));
					l_nam = (_TCHAR*)bp;
				}
				//Default
				else {
					BYTE *bp = (BYTE*)msp->Memory + (li_top + msRead_uint(msp, li_top + 0x10));
					l_nam = (char*)bp;
				}
			}
			//Remote
			else if (li_flags & 0x02) {
				//CommonNetworkRelativeLink
				int net_vtbl_ofs  = li_top + msRead_uint(msp, li_top + 0x14);
				BYTE *bp = (BYTE*)msp->Memory + (net_vtbl_ofs + msRead_uint(msp, net_vtbl_ofs + 0x08));
				l_nam = (char*)bp;
				//CommonPathSuffix
				//Unicode
				if (li_hdr_size>=0x24) {
					bp = (BYTE*)msp->Memory + (li_top + msRead_uint(msp, li_top + 0x20));
					l_nam += "\\" + UnicodeString((_TCHAR*)bp);
				}
				//Default
				else {
					bp = (BYTE*)msp->Memory + (li_top + msRead_uint(msp, li_top + 0x18));
					l_nam += "\\" + UnicodeString((char*)bp);
				}
			}
		}

		if (ExtractFileDrive(pnam).Pos(':') && StartsStr("\\\\", l_nam))
			l_nam = pnam;
		else if (!l_nam.IsEmpty() && StartsText(l_nam, pnam) && l_nam.Length() < pnam.Length())
			l_nam = pnam;
		else if (l_nam.IsEmpty() && StartsStr("::", pnam))
			l_nam = pnam;

		//STRING_DATA
		int str_top = sh_ofs + id_len + li_len;
		UnicodeString l_rem, l_fld, l_prm;
		int p = str_top;
		if (p<msp->Size) {
			if (has_name)	  l_rem = msRead_strdat(msp, p, is_unicode);
			if (has_rel_path) msRead_strdat(msp, p, is_unicode);
			if (has_workdir)  l_fld = msRead_strdat(msp, p, is_unicode);
			if (has_argument) l_prm = msRead_strdat(msp, p, is_unicode);
			if (has_icon_loc) msRead_strdat(msp, p, is_unicode);
		}
		//EXTRA_DATA
		if (has_exstr) {
			for (int i=0; i<11; i++) {
				int blk_size = msRead_uint(msp, p); p += 4;
				if (blk_size<=4) break;
				unsigned int blk_sign = msRead_uint(msp, p); p += 4;
				if (blk_sign==0xa0000001) {
					BYTE *bp = (BYTE*)msp->Memory + p + 260;
					l_nam = (_TCHAR*)bp;
					break;
				}
				else {
					p += (blk_size - 8);
				}
			}
		}

		UnicodeString l_fnam = cv_env_var(l_nam);

		if (lst) {
			add_PropLine_if(_T("�����N��"),		l_nam, lst);
			add_PropLine_if(_T("�p�����[�^"),	l_prm, lst);
			add_PropLine_if(_T("��Əꏊ"),		l_fld, lst, LBFLG_PATH_FIF);
			add_PropLine_if(_T("�L�["),			l_key, lst);

			switch (show_cmd) {
			case SW_SHOWNORMAL:		add_PropLine(_T("�傫��"), "�ʏ�",   lst); 	break;
			case SW_SHOWMINIMIZED:	add_PropLine(_T("�傫��"), "�ŏ���", lst);	break;
			case SW_SHOWMAXIMIZED:	add_PropLine(_T("�傫��"), "�ő剻", lst);	break;
			}

			add_PropLine_if(_T("�R�����g"),		l_rem, lst);

			//�����N��`�F�b�N
			if (!l_fnam.IsEmpty() && !StartsStr("::", l_fnam)) {
				if (file_exists(l_fnam)) {
					if (dir_exists(l_fnam))
						add_PropLine(_T("�����N��̎��"), "�f�B���N�g��", lst);
					else
						add_PropLine(_T("�����N��̎��"), get_FileTypeStr(l_fnam), lst);
				}
				else add_PropLine_if(_T("�G���["), "�����N�悪������܂���B", lst, LBFLG_ERR_FIF);
			}
		}

		if (nam) *nam = l_fnam;
		if (prm) *prm = l_prm;
		if (fld) *fld = l_fld;
		if (shw) *shw = show_cmd;
		if (rem) *rem = l_rem;

		return (!l_nam.IsEmpty());
	}
	catch (...) {
		if (lst) lst->Clear();
		return false;
	}
}

//---------------------------------------------------------------------------
//�V���[�g�J�b�g�̃����N����擾
//---------------------------------------------------------------------------
UnicodeString UserShell::get_LnkName(UnicodeString fnam)
{
	UnicodeString lnam;
	if (!usr_SH->get_LnkInf(fnam, NULL, &lnam)) return EmptyStr;
	return lnam;
}

//---------------------------------------------------------------------------
//�t�@�C������w��T�C�Y�̃A�C�R�����擾
//---------------------------------------------------------------------------
HICON UserShell::get_Icon(
	UnicodeString fnam,	//�t�@�C����
	int &size,			//[i/o] �w��T�C�Y (�擾�T�C�Y���Ԃ�)
	bool chk_sz)		//�T�C�Y�`�F�b�N���s��	(default = true)
{
	if (is_ADS_name(fnam)) return NULL;

	HICON hIcon = NULL;

	//�}�E�X�|�C���^
	if (USAME_TI(ExtractFileExt(fnam), ".cur")) {
		if (size>32) return NULL;
		//���[�W�܂��̓X���[���A�C�R�����擾
		SHFILEINFO sif;
		if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO),
			SHGFI_ICON | ((size==32)? SHGFI_LARGEICON : SHGFI_SMALLICON)))
				hIcon = sif.hIcon;
		return hIcon;
	}

	//���̑����
	bool res = true;
	ITEMIDLIST *dir_pidl = NULL;
	ITEMIDLIST *object	 = NULL;
	try {
		try {
			//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL,
				ExtractFileDir(fnam).c_str(), NULL, &dir_pidl, NULL)))
					Abort();
			//�t�H���_��IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder, (void**)&targetFolder))) Abort();
			//�Ώۃt�@�C���̃A�C�e��ID
			if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
				ExtractFileName(fnam).c_str(), NULL, &object, NULL)))
					Abort();

			TComInterface<IExtractIcon> pExtractIcon;
			if (FAILED(targetFolder->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&object,
				IID_IExtractIcon, NULL, (void **)&pExtractIcon)))
					Abort();
			//Location�擾
			_TCHAR path[MAX_PATH] = _T("");
			int  idx   = 0;
			UINT flags = 0;
			if (pExtractIcon->GetIconLocation(GIL_FORSHELL, path, MAX_PATH, &idx, &flags)==S_OK) {
				// �A�C�R���擾
				if (pExtractIcon->Extract(path, idx, &hIcon, NULL, MAKELONG(size, 0))!=S_OK) Abort();
			}
		}
		catch (...) {
			if (size>32) {
				res = false;
			}
			else {
				SHFILEINFO sif;
				if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO),
					SHGFI_ICON | ((size==32)? SHGFI_LARGEICON : SHGFI_SMALLICON)))
						hIcon = sif.hIcon;
				res = (hIcon!=NULL);
			}
		}
	}
	__finally {
		::ILFree(dir_pidl);
		::ILFree(object);
	}
	if (!res) return NULL;

	//�T�C�Y�m�F
	if (hIcon && chk_sz) {
		bool ok = false;
		ICONINFO ii;
		::ZeroMemory(&ii, sizeof(ICONINFO));
		::GetIconInfo(hIcon, &ii);
		if (ii.hbmMask) ::DeleteObject(ii.hbmMask);
		if (ii.hbmColor) {
			BITMAP bm;
			if (::GetObject(ii.hbmColor, sizeof(BITMAP), &bm)!=0) {
				if (size>bm.bmWidth) size = bm.bmWidth;
				ok = true;
			}
			::DeleteObject(ii.hbmColor);
		}
		if (!ok) {
			::DestroyIcon(hIcon);
			hIcon = NULL;
		}
	}

	return hIcon;
}
//---------------------------------------------------------------------------
//�X���[���A�C�R�����擾
//---------------------------------------------------------------------------
HICON UserShell::get_SmallIcon(UnicodeString fnam)
{
	int size = 16 * ScrScale;
	return get_Icon(fnam, size, false);
}
//---------------------------------------------------------------------------
//�X���[���A�C�R����`��
//---------------------------------------------------------------------------
bool UserShell::draw_SmallIcon(UnicodeString fnam, TCanvas *cv, int x, int y)
{
	if (fnam.IsEmpty()) return false;

	HICON hIcon = usr_SH->get_SmallIcon(fnam);
	if (hIcon) {
		::DrawIconEx(cv->Handle, x, y, hIcon, 16 * ScrScale, 16 * ScrScale, 0, NULL, DI_NORMAL);
		::DestroyIcon(hIcon);
		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
bool UserShell::draw_SmallIcon(UnicodeString fnam, TImage *ip, TColor bg)
{
	ip->Picture->Bitmap->SetSize(ip->ClientWidth, ip->ClientHeight);
	TCanvas *cv = ip->Picture->Bitmap->Canvas;
	cv->Brush->Color = bg;
	cv->FillRect(ip->ClientRect);

	int size = 16 * ScrScale;
	return draw_SmallIcon(fnam, cv, (ip->ClientWidth - size) / 2, (ip->ClientHeight - size) / 2);
}

//---------------------------------------------------------------------------
//�t�@�C������T���l�C�����擾
//---------------------------------------------------------------------------
bool UserShell::get_Thumbnail(UnicodeString fnam, Graphics::TBitmap *o_bmp, int o_size)
{
	if (is_ADS_name(fnam)) return false;

	bool res = true;
	ITEMIDLIST *dir_pidl = NULL;
	ITEMIDLIST *object	 = NULL;

	try {
		try {
			//�f�X�N�g�b�v�� IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> desktop;
			if (FAILED(SHGetDesktopFolder(&desktop))) Abort();

			//�Ώۃt�H���_�̃A�C�e��ID
			if (FAILED(desktop->ParseDisplayName(NULL, NULL,
				ExtractFileDir(fnam).c_str(), NULL, &dir_pidl, NULL)))
					Abort();
			//�t�H���_��IShellFolder �C���^�[�t�F�C�X���擾
			TComInterface<IShellFolder> targetFolder;
			if (FAILED(desktop->BindToObject(dir_pidl, NULL, IID_IShellFolder, (void**)&targetFolder))) Abort();
			//�Ώۃt�@�C���̃A�C�e��ID
			if (FAILED(targetFolder->ParseDisplayName(NULL, NULL,
				ExtractFileName(fnam).c_str(), NULL, &object, NULL)))
	 				Abort();

			TComInterface<IExtractImage> pExtractImage;
			if (FAILED(targetFolder->GetUIObjectOf(NULL, 1, (LPCITEMIDLIST*)&object,
				IID_IExtractImage, NULL, (void **)&pExtractImage)))
					Abort();

			_TCHAR path[MAX_PATH] = _T("");
			SIZE size;
			size.cx = o_size;
			size.cy = o_size;
			DWORD dwFlags = IEIFLAG_ORIGSIZE;
			if (FAILED(pExtractImage->GetLocation(path, MAX_PATH, NULL, &size, 32, &dwFlags))) Abort();
			HBITMAP hBmp;
			if (FAILED(pExtractImage->Extract(&hBmp))) Abort();
			o_bmp->Handle = hBmp;
		}
		catch (...) {
			res = false;
		}
	}
	__finally {
		::ILFree(dir_pidl);
		::ILFree(object);
	}
	return res;
}

//---------------------------------------------------------------------------
//���m�p�X��GUID��������p�X���ɕϊ�
//---------------------------------------------------------------------------
UnicodeString UserShell::KnownGuidStrToPath(UnicodeString s)
{
	UnicodeString ret_str = s;
	try {
		TGUID guid = StringToGUID(s);
		PWSTR pbuf = NULL;
		if (FAILED(::SHGetKnownFolderPath(guid, 0, NULL, &pbuf))) Abort();
		ret_str = pbuf;
		CoTaskMemFree(pbuf);
	}
	catch (...) {
		ret_str = EmptyStr;
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//���m�p�XID���p�X���ɕϊ�
//---------------------------------------------------------------------------
UnicodeString UserShell::KnownGuidToPath(REFKNOWNFOLDERID id)
{
	UnicodeString ret_str;
	PWSTR pbuf = NULL;
	if (SUCCEEDED(::SHGetKnownFolderPath(id, 0, NULL, &pbuf))) {
		ret_str = pbuf;
		CoTaskMemFree(pbuf);
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//����t�H���_�ꗗ�ɒǉ�(���[�N���X�g�`��)
//---------------------------------------------------------------------------
void UserShell::AddKnownPath(
	REFKNOWNFOLDERID id,	//ID
	const _TCHAR *inam,		//���O	(�擪�� | ������Ə�[�ɋ�؂����\��)
	TStringList *lst)		//[o] ���X�g
{
	UnicodeString dnam = KnownGuidToPath(id);
	if (!dnam.IsEmpty()) {
		lst->Add(UnicodeString().sprintf(_T("%s\t%s"), IncludeTrailingPathDelimiter(dnam).c_str(), inam));
	}
}
//---------------------------------------------------------------------------
void UserShell::AddEnvPath(
	const _TCHAR *enam,
	const _TCHAR *inam,
	TStringList *lst)
{
	UnicodeString dnam = cv_env_str(enam);
	if (!dnam.IsEmpty() && dnam.Pos('%')==0) {
		lst->Add(UnicodeString().sprintf(_T("%s\t%s"), IncludeTrailingPathDelimiter(dnam).c_str(), inam));
	}
}

//---------------------------------------------------------------------------
//����t�H���_�ꗗ���擾(���[�N���X�g�`��)
//---------------------------------------------------------------------------
void UserShell::get_SpecialFolderList(TStringList *lst)
{
	AddKnownPath(FOLDERID_Profile,			_T("���[�U�v���t�@�C��"),	lst);
	AddKnownPath(FOLDERID_Desktop,			_T("�f�X�N�g�b�v"),			lst);
	AddKnownPath(FOLDERID_Favorites,		_T("���C�ɓ���"),			lst);
	AddKnownPath(FOLDERID_Documents,		_T("�h�L�������g"),			lst);
	AddKnownPath(FOLDERID_Downloads,		_T("�_�E�����[�h"),			lst);
	AddKnownPath(FOLDERID_Pictures,			_T("�s�N�`��"),				lst);
	AddKnownPath(FOLDERID_Videos,			_T("�r�f�I"),				lst);
	AddKnownPath(FOLDERID_Music,			_T("�~���[�W�b�N"),			lst);
	AddEnvPath(_T("%OneDrive%"),			_T("OneDrive"),				lst);
	AddKnownPath(FOLDERID_RoamingAppData,	_T("|Application Data"),	lst);
	AddEnvPath(_T("%APPDATA%\\Microsoft\\Windows\\Libraries"), _T("���C�u����"), lst);
	AddKnownPath(FOLDERID_StartMenu,		_T("�X�^�[�g���j���["),		lst);
	AddKnownPath(FOLDERID_Programs,			_T("�v���O����"),			lst);
	AddKnownPath(FOLDERID_Startup,			_T("�X�^�[�g�A�b�v"),		lst);
	AddKnownPath(FOLDERID_AdminTools,		_T("�Ǘ��c�[��"),			lst);
	AddKnownPath(FOLDERID_QuickLaunch,		_T("�N�C�b�N�N��"),			lst);
	AddKnownPath(FOLDERID_Recent,			_T("�ŋߎg��������"),		lst);
	AddKnownPath(FOLDERID_SendTo,			_T("SendTo"),				lst);
	AddKnownPath(FOLDERID_Templates,		_T("�e���v���[�g"),			lst);
	AddKnownPath(FOLDERID_PrintHood,		_T("�v�����^�["),			lst);
	AddKnownPath(FOLDERID_NetHood,			_T("NetHood"),				lst);
	AddKnownPath(FOLDERID_CDBurning,		_T("CDBURN AREA"),			lst);
	AddKnownPath(FOLDERID_Cookies,			_T("Cookies"),				lst);
	AddKnownPath(FOLDERID_History,			_T("����"),					lst);
	AddKnownPath(FOLDERID_InternetCache,	_T("�C���^�[�l�b�g�ꎞ�t�@�C��"),	lst);

	lst->Add("\t-");	//�Z�p���[�^
	AddKnownPath(FOLDERID_PublicDesktop,	_T("�f�X�N�g�b�v"),			lst);
	AddKnownPath(FOLDERID_PublicDocuments,	_T("�h�L�������g"),			lst);
	AddKnownPath(FOLDERID_PublicPictures,	_T("�s�N�`��"),				lst);
	AddKnownPath(FOLDERID_PublicVideos,		_T("�r�f�I"),				lst);
	AddKnownPath(FOLDERID_PublicMusic,		_T("�~���[�W�b�N"),			lst);
	AddKnownPath(FOLDERID_ProgramData,		_T("|Application Data"),	lst);
	AddKnownPath(FOLDERID_CommonStartMenu,	_T("�X�^�[�g���j���["),		lst);
	AddKnownPath(FOLDERID_CommonPrograms,	_T("�v���O����"),			lst);
	AddKnownPath(FOLDERID_CommonStartup,	_T("�X�^�[�g�A�b�v"),		lst);
	AddKnownPath(FOLDERID_CommonAdminTools,	_T("�Ǘ��c�[��"),			lst);
	AddKnownPath(FOLDERID_CommonTemplates,	_T("�e���v���[�g"),			lst);

	AddEnvPath(_T("%ProgramW6432%"),		_T("|Program Files"),		lst);
	AddKnownPath(FOLDERID_ProgramFilesX86,	_T("Program Files (x86)"),	lst);
	AddKnownPath(FOLDERID_Windows,			_T("Windows"),				lst);
	AddKnownPath(FOLDERID_System,			_T("Windows System"),		lst);
	AddKnownPath(FOLDERID_SystemX86,		_T("Windows System32"),		lst);
	AddKnownPath(FOLDERID_Fonts,			_T("�t�H���g"),				lst);
	AddEnvPath(_T("%TEMP%"),				_T("TEMP"),					lst);
	AddEnvPath(_T("%TMP%"),					_T("TMP"),					lst);

	lst->Add("\t-");	//�Z�p���[�^
	lst->Add("shell:::{20D04FE0-3AEA-1069-A2D8-08002B30309D}\tPC/�R���s���[�^");
	lst->Add("shell:::{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}\t�l�b�g���[�N");
	lst->Add("shell:::{208D2C60-3AEA-1069-A2D7-08002B30309D}\t�l�b�g���[�N(���[�N�O���[�v)");
	lst->Add("shell:::{ED7BA470-8E54-465E-825C-99712043E01C}\tGodMode");
	lst->Add("shell:ControlPanelFolder\t�R���g���[���p�l��");
	lst->Add("shell:ConnectionsFolder\t�l�b�g���[�N�ڑ�");
	lst->Add("shell:PrintersFolder\t�v�����^�[");
	lst->Add("shell:RecycleBinFolder\t���ݔ�");
}
//---------------------------------------------------------------------------
