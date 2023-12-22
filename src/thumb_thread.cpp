//----------------------------------------------------------------------//
// NyanFi																//
//  サムネイル取得スレッド												//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "usr_shell.h"
#include "UserFunc.h"
#include "Global.h"
#include "thumb_thread.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//スレッドの初期化
//---------------------------------------------------------------------------
__fastcall TThumbnailThread::TThumbnailThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLowest;
	FreeOnTerminate = true;

	TaskRWLock	  = new TMultiReadExclusiveWriteSynchronizer();
	ThumbnailList = new TStringList();

	CallbackWnd = NULL;
	ReqClear	= false;
	ReqStart	= false;
	ReqMake		= false;
	MakeIndex	= -1;
	StartIndex  = -1;
	ReqMake		= false;
	IsEmpty 	= true;
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TThumbnailThread::GetListItem(int idx)
{
	TaskRWLock->BeginRead();
	UnicodeString v = (idx>=0 && idx<ThumbnailList->Count)? ThumbnailList->Strings[idx] : EmptyStr;
	TaskRWLock->EndRead();
	return v;
}
//---------------------------------------------------------------------------
void __fastcall TThumbnailThread::SetListItem(int idx, UnicodeString s)
{
	TaskRWLock->BeginWrite();
	if (idx>=0 && idx<ThumbnailList->Count) ThumbnailList->Strings[idx] = s;
	TaskRWLock->EndWrite();
}
//---------------------------------------------------------------------------
Graphics::TBitmap* __fastcall TThumbnailThread::GetListBitmap(int idx)
{
	TaskRWLock->BeginRead();
	Graphics::TBitmap *vp = (idx>=0 && idx<ThumbnailList->Count)? (Graphics::TBitmap*)ThumbnailList->Objects[idx] : NULL;
	TaskRWLock->EndRead();
	return vp;
}
//---------------------------------------------------------------------------
Graphics::TBitmap* __fastcall TThumbnailThread::GetListBitmap(UnicodeString fnam)
{
	Graphics::TBitmap *vp = NULL;
	TaskRWLock->BeginRead();
	for (int i=0; i<Count; i++) {
		if (SameText(fnam, get_pre_tab(ThumbnailList->Strings[i]))) {
			vp = (Graphics::TBitmap*)ThumbnailList->Objects[i];
			break;
		}
	}
	TaskRWLock->EndRead();
	return vp;
}

//---------------------------------------------------------------------------
bool __fastcall TThumbnailThread::FitSize(int *wd, int *hi)
{
	if (*wd>ThumbnailSize || *hi>ThumbnailSize) {
		//横の方が長い
		if (*wd>*hi) {
			float r = 1.0 * *hi / *wd;
			*wd = ThumbnailSize;
			*hi = *wd * r;
		}
		//縦の方が長い
		else {
			float r = 1.0 * *wd / *hi;
			*hi = ThumbnailSize;
			*wd = *hi * r;
		}
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
void __fastcall TThumbnailThread::MakeThumbnail(int idx)
{
	if (idx>=0 && idx<Count) {
		TaskRWLock->BeginRead();
		UnicodeString    fnam = get_pre_tab(ThumbnailList->Strings[idx]);
		Graphics::TBitmap *bp = (Graphics::TBitmap*)ThumbnailList->Objects[idx];
		TaskRWLock->EndRead();
		if (fnam.IsEmpty() || !bp || !bp->Empty) return;

		try {
			if (!file_exists(fnam)) throw Exception(EmptyStr);

			UnicodeString fext = get_extension(fnam);
			UnicodeString tmp;

			while (!ImgViewThread->IsReady()) Sleep(50);

			//サムネイルキャッシュ
			UnicodeString ibuf = load_ImageThumbCache(fnam, bp);
			if (!ibuf.IsEmpty()) {
				SetListItem(idx, ibuf);
			}
			else {
				std::unique_ptr<Graphics::TBitmap> i_bp(new Graphics::TBitmap());
				bool to_resize;

				//メタファイル
				if (test_MetaExt(fext)) {
					std::unique_ptr<TMetafile> mf(new TMetafile());
					mf->LoadFromFile(fnam);
					int i_wd = mf->Width;
					int i_hi = mf->Height;
					to_resize = FitSize(&i_wd, &i_hi);
					i_bp->SetSize(i_wd, i_hi);
					i_bp->Canvas->Lock();
					{
						i_bp->Canvas->Brush->Color = col_bgWMF;
						TRect rc = Rect(0, 0, i_wd, i_hi);
						i_bp->Canvas->FillRect(rc);
						i_bp->Canvas->StretchDraw(rc, mf.get());
					}
					i_bp->Canvas->Unlock();
					SetListItem(idx, tmp.sprintf(_T("%s\t%s"), fnam.c_str(), get_wd_x_hi_str(mf->Width, mf->Height).c_str()));
				}
				//ビットマップ
				else {
					int img_ori = 0;
					//アイコン
					if (test_FileExt(fext, FEXT_ICONVIEW)) {
						int size = 256;
						for (int i=0; i<3; i++) {
							HICON hIcon = usr_SH->get_Icon(fnam, size);
							if (hIcon) {
								i_bp->Canvas->Lock();
								{
									i_bp->SetSize(size, size);
									i_bp->Canvas->Brush->Color = col_bgImage;
									i_bp->Canvas->FillRect(Rect(0, 0, size, size));
									::DrawIconEx(i_bp->Canvas->Handle, 0, 0, hIcon, size, size, 0, NULL, DI_NORMAL);
									::DestroyIcon(hIcon);
									i_bp->PixelFormat = pf24bit;
								}
								i_bp->Canvas->Unlock();
								break;
							}
							if (size==256) size = 48; else if (size==48) size = 32;
						}
						int ixn = (int)::ExtractIcon(HInstance, fnam.c_str(), -1);
						if (ixn>0) SetListItem(idx, tmp.sprintf(_T("%s\t%u Icons"), fnam.c_str(), ixn));
					}
					//画像
					else {
						int res = load_ImageFile(fnam, i_bp.get(), WICIMG_THUMBNAIL, col_bgImage);
						if (res==0) throw Exception(EmptyStr);
						//Exif情報の取得
						UnicodeString exif_inf;
						//Exifを取得できる場合はそれを設定
						if (test_ExifExt(fext)) exif_inf = get_ExifInfThumb(fnam, &img_ori);
						//取得できない場合はサイズ情報を設定
						if (exif_inf.IsEmpty() && !i_bp->Empty) exif_inf = get_wd_x_hi_str(i_bp->Width, i_bp->Height);
						if (!exif_inf.IsEmpty()) SetListItem(idx, tmp.sprintf(_T("%s\t%s"), fnam.c_str(), exif_inf.c_str()));
						if (res==LOADED_BY_WIC) img_ori = 0;
					}

					//リサイズ
					int i_wd = i_bp->Width;
					int i_hi = i_bp->Height;
					to_resize = FitSize(&i_wd, &i_hi);
					if (to_resize) {
						std::unique_ptr<Graphics::TBitmap> r_bp(new Graphics::TBitmap());
						if (!WIC_resize_image(i_bp.get(), r_bp.get(), 0.0, i_wd, i_hi, ThumbScaleOpt)) throw Exception(EmptyStr);
						i_bp->Handle = r_bp->ReleaseHandle();
					}

					//回転
					if (RotViewImg && img_ori>0) {
						if		(img_ori==6) WIC_rotate_image(i_bp.get(), 1);
						else if (img_ori==3) WIC_rotate_image(i_bp.get(), 2);
						else if (img_ori==8) WIC_rotate_image(i_bp.get(), 3);
					}
				}

				bp->Handle = i_bp->ReleaseHandle();

				//ADSにキャッシュ
				UnicodeString pnam = ExtractFilePath(fnam);
				if (CacheThumbADS && to_resize && is_NTFS_Drive(pnam)
					&& !is_drive_protected(pnam) && !is_NoInfPath(pnam, NoCachePath))
				{
					int atr = file_GetAttr(fnam);
					if (atr!=faInvalid && (atr & faReadOnly)==0) {
						TDateTime ft = get_file_age(fnam);
						UnicodeString fnam_t = fnam + THUMB_JPG_ADS;
						UnicodeString fnam_x = fnam + THUMB_TXT_ADS;
						if (WIC_save_image(fnam_t, bp, 80, 0)) {
							//表示情報 [TAB] タイムスタンプ [TAB] サムネイルサイズ [TAB] アルゴリズム
							UnicodeString lbuf = get_post_tab(GetListItem(idx));
							lbuf.cat_sprintf(_T("\t%s\t%u\t%u"),
									FormatDateTime("yyyymmddhhnnss", ft).c_str(), ThumbnailSize, ThumbScaleOpt);
							saveto_TextUTF8(fnam_x, lbuf);
						}
						else {
							delete_FileIf(fnam_t);
							delete_FileIf(fnam_x);
						}
						set_file_age(fnam, ft);
					}
				}
			}
		}
		catch (...) {
			SetListItem(idx, EmptyStr);
		}

		if (CallbackWnd) ::SendMessage(CallbackWnd, WM_NYANFI_THUMBNAIL, (WPARAM)0, (LPARAM)idx);
	}
}

//---------------------------------------------------------------------------
void __fastcall TThumbnailThread::Execute()
{
	::CoInitialize(NULL);

	while (!Terminated) {
		//サムネイルリストの作成
		if (ReqStart) {
			IsEmpty  = true;
			//現在位置から前後交互に順次取得
			int idx = ThumbnailThread->StartIndex;
			int cnt = Count;
			int tag = 0, n = idx, p = idx;
			for (int i=0; !ReqClear && i<cnt; i++) {
				switch (tag) {
				case 0:
					tag = 1; break;
				case 1:
					tag = (++n<cnt)? 2 : 4;
					idx = (tag==2)? n : (--p>=0)? p : -1;
					break;
				case 2:
					tag = (--p>=0)? 1 : 3;
					idx = (tag==1)? p : (++n<cnt)? n : -1;
					break;
				case 3:
					idx = (++n<cnt)? n : -1;
					break;
				case 4:
					idx = (--p>=0)? p : -1;
					break;
				}

				if (idx==-1) break;
				MakeThumbnail(idx);
			}
			ReqStart = false;
			IsEmpty  = !ReqClear;
		}
		//個別作成
		else if (ReqMake) {
			MakeThumbnail(MakeIndex);
			ReqMake = false;
		}

		//リストをクリア
		if (ReqClear) {
			TaskRWLock->BeginWrite();
			for (int i=0; i<ThumbnailList->Count; i++) delete (Graphics::TBitmap*)ThumbnailList->Objects[i];
			ThumbnailList->Clear();
			TaskRWLock->EndWrite();

			ReqStart = false;
			ReqClear = false;
			IsEmpty  = true;
		}

		Sleep(50);
	}

	delete ThumbnailList;
	delete TaskRWLock;

	::CoUninitialize();
}
//---------------------------------------------------------------------------
