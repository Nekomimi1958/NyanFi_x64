//----------------------------------------------------------------------//
// NyanFi																//
//  アイコン取得スレッド												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "Global.h"
#include "icon_thread.h"
#include "AppDlg.h"
#include "EditHistDlg.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//スレッドの初期化
//---------------------------------------------------------------------------
__fastcall TGetIconThread::TGetIconThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLower;
	FreeOnTerminate = true;

	CallbackWnd 	= NULL;
}

//---------------------------------------------------------------------------
//取得の通知
//---------------------------------------------------------------------------
void __fastcall TGetIconThread::IconNotify()
{
	//ファイラー
	if (CallbackWnd)
		::SendMessage(CallbackWnd, WM_NYANFI_FLICON, (WPARAM)0, (LPARAM)0);

	//ランチャー
	if (AppListDlg && AppListDlg->Visible)
		::SendMessage(AppListDlg->Handle, WM_NYANFI_FLICON, (WPARAM)0, (LPARAM)0);

	//最近編集/閲覧したファイル一覧/栞マーク一覧
	if (EditHistoryDlg && EditHistoryDlg->Visible)
		::SendMessage(EditHistoryDlg->Handle, WM_NYANFI_FLICON, (WPARAM)0, (LPARAM)0);
}

//---------------------------------------------------------------------------
void __fastcall TGetIconThread::Execute()
{
	while (!Terminated) {
		//キャシュ数制限
		IconRWLock->BeginWrite();
			while (CachedIcoList->Count > IconCache) {
				delete (TIcon*)CachedIcoList->Objects[0];
				CachedIcoList->Delete(0);
			}
		IconRWLock->EndWrite();

		//取得
		int last_cnt = GetTickCount();
		int i = 0, cnt = 0;
		bool quit = false;

		while (!quit) {
			UnicodeString fnam = EmptyStr;
			IconRWLock->BeginRead();
				if (i<CachedIcoList->Count) {
					if (!CachedIcoList->Objects[i]) fnam = CachedIcoList->Strings[i];
					i++;
				}
				else quit = true;
			IconRWLock->EndRead();

			if (!fnam.IsEmpty()) {
				//アイコンを取得
				HICON hIcon;
				SHFILEINFO sif;
				if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_SMALLICON))
					hIcon = sif.hIcon;
				else
					hIcon = get_fext_icon(get_extension(fnam));

				//キャッシュに設定
				IconRWLock->BeginWrite();
					int idx = CachedIcoList->IndexOf(fnam);
					if (idx!=-1) {
						TIcon *icon  = new TIcon();
						icon->Handle = hIcon;
						CachedIcoList->Objects[idx] = (TObject*)icon;
						cnt ++;
					}
				IconRWLock->EndWrite();

				//200ms毎に通知
				if (((int)GetTickCount() - last_cnt)>200 && cnt>0) {	//***
					IconNotify();
					last_cnt = GetTickCount();
					cnt = 0;
				}
			}
		}

		//残りがあれば通知
		if (cnt>0) IconNotify();

		Sleep(50);
	}
}
//---------------------------------------------------------------------------
