//----------------------------------------------------------------------//
// NyanFi																//
//  �A�C�R���擾�X���b�h												//
//----------------------------------------------------------------------//
#include "Global.h"
#include "AppDlg.h"
#include "EditHistDlg.h"
#include "DirDlg.h"
#include "OptDlg.h"
#include "BtnDlg.h"
#include "icon_thread.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//�X���b�h�̏�����
//---------------------------------------------------------------------------
__fastcall TGetIconThread::TGetIconThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLower;
	FreeOnTerminate = true;
	CallbackWnd 	= NULL;
}

//---------------------------------------------------------------------------
//�擾�̒ʒm
//---------------------------------------------------------------------------
void __fastcall TGetIconThread::IconNotify()
{
	if (CallbackWnd) ::SendMessage(CallbackWnd, WM_NYANFI_FLICON, 0, 0);

	TForm *forms[] = {AppListDlg, EditHistoryDlg, RegDirDlg, OptionDlg, ToolBtnDlg};
	int cnt = sizeof(forms)/sizeof(forms[0]);
	for (int i=0; i<cnt; i++) {
		if (forms[i] && forms[i]->Visible) ::SendMessage(forms[i]->Handle, WM_NYANFI_FLICON, 0, 0);
	}
}

//---------------------------------------------------------------------------
void __fastcall TGetIconThread::Execute()
{
	while (!Terminated) {
		//�L���V��������
		IconRWLock->BeginWrite();
			while (CachedIcoList->Count > IconCache) {
				delete (TIcon*)CachedIcoList->Objects[0];
				CachedIcoList->Delete(0);
			}
		IconRWLock->EndWrite();

		//�擾
		int last_cnt = GetTickCount();
		int i = 0, cnt = 0;
		bool quit = false;

		while (!quit) {
			UnicodeString fnam;
			IconRWLock->BeginRead();
				if (i<CachedIcoList->Count) {
					if (!CachedIcoList->Objects[i]) fnam = CachedIcoList->Strings[i];
					i++;
				}
				else {
					quit = true;
				}
			IconRWLock->EndRead();
			if (fnam.IsEmpty()) continue;

			//�A�C�R�����擾
			HICON hIcon = get_file_SmallIcon(fnam);

			//�L���b�V���ɐݒ�
			IconRWLock->BeginWrite();
				int idx = CachedIcoList->IndexOf(fnam);
				if (idx!=-1) {
					TIcon *icon  = new TIcon();
					icon->Handle = hIcon;
					CachedIcoList->Objects[idx] = (TObject*)icon;
					cnt++;
				}
			IconRWLock->EndWrite();

			//200ms���ɒʒm
			if (((int)GetTickCount() - last_cnt)>200 && cnt>0) {	//***
				IconNotify();
				last_cnt = GetTickCount();
				cnt = 0;
			}
		}

		//�c�肪����Βʒm
		if (cnt>0) IconNotify();

		Sleep(50);
	}
}
//---------------------------------------------------------------------------
