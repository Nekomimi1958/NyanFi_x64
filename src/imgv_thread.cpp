//----------------------------------------------------------------------//
// NyanFi																//
//  画像ビュー処理スレッド												//
//----------------------------------------------------------------------//
#include "usr_wic.h"
#include "usr_exif.h"
#include "usr_id3.h"
#include "usr_swatch.h"
#include "UserFunc.h"
#include "Global.h"
#include "HistFrm.h"
#include "MainFrm.h"
#include "PrnImgDlg.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//スレッドの初期化
//---------------------------------------------------------------------------
__fastcall TImgViewThread::TImgViewThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority		= tpLowest;
	FreeOnTerminate = true;

	TaskRWLock	= new TMultiReadExclusiveWriteSynchronizer();
	RequestList = new TStringList();

	Rotation   = 0;
	ExifOri	   = 0;
	ZoomRatio  = 100;
	ZoomRatioF = 1.0;
	Fitted	   = true;
	ShowGrid   = false;
	GrayScaled = false;

	ImgBuff    = new Graphics::TBitmap();
	ViewBuff   = new Graphics::TBitmap();
	ChgBuff    = new Graphics::TBitmap();
	MetaBuff   = new TMetafile();

	Img_f_size	= 0;
	Img_f_time	= 0;
	ImgIsWmf	= false;
	ChgPrvCur	= false;

	TaskBusy	= false;
	ReqRedraw	= false;
	ReqReload	= false;
	ReqClear	= false;
	ReqVideo	= false;
	ReqWait 	= false;

	UseViewBuff = false;
	MsgIsErr	= false;
}

//---------------------------------------------------------------------------
bool __fastcall TImgViewThread::IsReady()
{
	TaskRWLock->BeginRead();
	bool ret = !FTaskBusy && (RequestList->Count==0);
	TaskRWLock->EndRead();
	return ret;
}
//---------------------------------------------------------------------------
bool __fastcall TImgViewThread::IsWaiting()
{
	TaskRWLock->BeginRead();
	bool ret = (ReqRedraw || ReqReload || ReqClear);
	if (!ret) {
		for (int i=0; i<RequestList->Count && !ret; i++) {
			UnicodeString cmd = get_pre_tab(RequestList->Strings[i]);
			ret = contains_wd_i(cmd, "REDRAW|RELOAD|CLEAR");
		}
	}
	TaskRWLock->EndRead();
	return ret;
}

//---------------------------------------------------------------------------
//キューに処理要求を追加
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::AddRequest(const _TCHAR *cmd, UnicodeString prm1, UnicodeString prm2)
{
	UnicodeString lbuf = cmd;
	if (!prm1.IsEmpty()) {
		lbuf.cat_sprintf(_T("\t%s"), prm1.c_str());
		if (!prm2.IsEmpty()) lbuf.cat_sprintf(_T("\t%s"), prm2.c_str());
	}

	TaskRWLock->BeginWrite();
	RequestList->Add(lbuf);
	TaskRWLock->EndWrite();
}
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::AddRequest(const _TCHAR *cmd, int prm)
{
	AddRequest(cmd, IntToStr(prm));
}

//---------------------------------------------------------------------------
//画像をクリア(Synchronize で呼び出すこと)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::ClearImage()
{
	TImage *v_img = NULL;

	//イメージビューア
	if (NyanFiForm->ImgViewPanel->Visible) {
		v_img = NyanFiForm->ViewerImage;
	}
	//プレビュー
	else if (NyanFiForm->PreviewPanel->Visible) {
		v_img = NyanFiForm->PreviewImage;
		v_img->Cursor = crDefault;
		NyanFiForm->PreviewSttLabel->Visible = false;
		NyanFiForm->PreviewSttLabel->Caption = EmptyStr;
		NyanFiForm->PreviewPanel->Color 	 = col_bgImage;
	}

	if (v_img) {
		v_img->Picture->Bitmap->SetSize(v_img->ClientWidth, v_img->ClientHeight);
		TCanvas *cv = v_img->Picture->Bitmap->Canvas;
		cv->Lock();
		cv->Brush->Color = col_bgImage;
		cv->FillRect(v_img->ClientRect);
		cv->Unlock();
	}

	Img_f_name = EmptyStr;  Img_f_size = 0;  Img_f_time = 0;
	ImgIsWmf  = false;
	ChgPrvCur = false;

	NyanFiForm->ImgInfListBox->Clear();
	if (HistForm->Visible) HistForm->DrawHistogram();
}

//---------------------------------------------------------------------------
//画像を表示 (Synchronize で呼び出すこと)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::DrawImage()
{
	if (ImgIsWmf && MetaBuff->Empty) return;
	if (!ImgIsWmf && ImgBuff->Empty) return;

	TImage *v_img	= NULL;
	TPanel *v_panel = NULL;

	//イメージビューア
	if (NyanFiForm->ImgViewPanel->Visible) {
		v_img	= NyanFiForm->ViewerImage;
		v_panel = NyanFiForm->ImgScrollPanel;
	}
	//プレビュー
	else if (NyanFiForm->PreviewPanel->Visible) {
		v_img	= NyanFiForm->PreviewImage;
		v_img->Transparent = false;
		v_panel = NyanFiForm->PreviewPanel;
	}

	if (!v_img) return;

	bool is_viewer = SameStr(v_img->Name, "ViewerImage");

	//回転
	if (!ImgBuff->Empty && Rotation>0) {
		WIC_rotate_image(ImgBuff, Rotation);
		Rotation = 0;
	}

	if (!ReqRedraw || ReqReload) {
		ViewBuff->Handle = NULL;

		int iw = ImgIsWmf? MetaBuff->Width  : ImgBuff->Width;
		int ih = ImgIsWmf? MetaBuff->Height : ImgBuff->Height;
		if (iw==0 || ih==0) return;

		double r = 1.0;
		//プレビューまたはフィット表示
		if (!is_viewer || Fitted) {
			r = get_ZoomRatio(v_panel->ClientWidth, v_panel->ClientHeight, iw, ih,
				is_viewer? ImgFrameMargin : 0,
				Fitted? (std::max(ImgFitMaxZoom, 100)/100.0) : 1.0);
			if (is_viewer) ZoomRatio = r * 100;
		}
		//ズーム
		else if (is_viewer && ZoomRatio!=100) {
			r = ZoomRatio / 100.0;
		}

		ZoomRatioF = r;

		//メタファイル
		if (ImgIsWmf) {
			if (r!=1.0) {
				iw *= r;  ih *= r;
			}
			ViewBuff->SetSize(iw, ih);
			TRect rc = Rect(0, 0, iw, ih);
			TCanvas *cv = ViewBuff->Canvas;
			cv->Lock();
			cv->Brush->Color = col_bgWMF;
			cv->FillRect(rc);
			cv->StretchDraw(rc, MetaBuff);
			cv->Unlock();
			//回転・反転
			if (Rotation>0) {
				Img_rot_str.cat_sprintf(_T("%u"), Rotation);
				Rotation = 0;
			}
			for (int i=1; i<=Img_rot_str.Length(); i++) {
				WIC_rotate_image(ViewBuff, Img_rot_str.SubString(i, 1).ToIntDef(0));
			}
			//冗長な処理履歴を削除
			if (Img_rot_str.Length()>1)
				Img_rot_str = TRegEx::Replace(Img_rot_str, "13|31|44|55|1111|3333", EmptyStr);
		}
		//画像ファイル
		else {
			if (r!=1.0) {
				//拡大・縮小
				if (WicScaleOpt>=0 && WicScaleOpt<=4) {
					WIC_resize_image(ImgBuff, ViewBuff, r, 0, 0, WicScaleOpt);
				}
				else {
					iw *= r;  ih *= r;
					ViewBuff->Canvas->Lock();
					ViewBuff->PixelFormat = pf24bit;
					ViewBuff->SetSize(iw, ih);
					ViewBuff->Canvas->StretchDraw(Rect(0, 0, iw, ih), ImgBuff);
					ViewBuff->Canvas->Unlock();
				}
			}
			else {
				//等倍
				ViewBuff->Assign(ImgBuff);
			}
		}

		//グレースケール化
		if (GrayScaled) WIC_grayscale_image(ViewBuff);

		if (is_viewer) {
			UnicodeString sttstr; sttstr.sprintf(_T("%3u%"), ZoomRatio);
			if (ZoomRatio!=100) sttstr += (" " + UnicodeString("NLCFHX").SubString(WicScaleOpt + 1, 1));
			if (NyanFiForm->ImgSttHeader->Visible) NyanFiForm->ImgSttHeader->Panels->Items[3]->Text = sttstr;
			if (NyanFiForm->ImgInfBar->Visible)    NyanFiForm->ImgInfBar->Panels->Items[1]->Text 	= sttstr;
		}
	}

	//描画
	v_img->Center = true;

	//イメージビューア
	if (is_viewer) {
		TControlScrollBar *hbar = NyanFiForm->ImgScrollBox->HorzScrollBar;
		TControlScrollBar *vbar = NyanFiForm->ImgScrollBox->VertScrollBar;

		if (ViewBuff->Width>v_panel->ClientWidth || ViewBuff->Height>v_panel->ClientHeight) {
			//スクロールバー設定
			hbar->Range = ViewBuff->Width;
			vbar->Range = ViewBuff->Height;
			//センターに移動
			hbar->Position = (hbar->Range - v_panel->ClientWidth)/2;
			vbar->Position = (vbar->Range - v_panel->ClientHeight)/2;
		}
		else {
			hbar->Range = v_panel->ClientWidth;
			vbar->Range = v_panel->ClientHeight;
		}

		NyanFiForm->ImgScrollBox->Color = ImgIsWmf? col_bgWMF : col_bgImage;
		v_img->Picture->Bitmap->Assign(ViewBuff);

		//分割グリッド
		if (is_viewer && ShowGrid) draw_ImgGrid(v_img->Picture->Bitmap->Canvas, ViewBuff);

		NyanFiForm->SetSttBarInf();

		//ヒストグラム
		if (HistForm->Visible) HistForm->DrawHistogram(ImgBuff);

		//印刷設定
		if (PrintImgDlg && PrintImgDlg->Visible) PrintImgDlg->DrawImage();
	}
	//プレビュー表示
	else {
		NyanFiForm->PreviewSttLabel->Visible = false;
		NyanFiForm->PreviewSttLabel->Caption = EmptyStr;
		NyanFiForm->PreviewPanel->Color = ImgIsWmf? col_bgWMF : col_bgImage;
		v_img->Align   = alClient;
		v_img->Stretch = false;
		v_img->Picture->Bitmap->Assign(ViewBuff);
	}

	NyanFiForm->ImgBuff->Assign(ImgBuff);
	NyanFiForm->UpdateLoupe();
}

//---------------------------------------------------------------------------
//イメージプレビューに ViewBuff を表示 (Synchronize で呼び出すこと)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::DrawViewBuff()
{
	if (NyanFiForm->PreviewPanel->Visible) {
		NyanFiForm->PreviewSttLabel->Visible = false;
		NyanFiForm->PreviewSttLabel->Caption = EmptyStr;
		NyanFiForm->PreviewPanel->Color 	 = col_bgImage;

		TImage *v_img = NyanFiForm->PreviewImage;
		v_img->Transparent = false;
		v_img->Align	   = alClient;
		v_img->Stretch	   = false;
		v_img->Center	   = false;
		v_img->Picture->Bitmap->Assign(ViewBuff);

		NyanFiForm->SetPrvImgCursor(ChgPrvCur);
	}

	NyanFiForm->ImgBuff->Assign(ImgBuff);
}

//---------------------------------------------------------------------------
//メッセージを表示 (Synchronize で呼び出すこと)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::DrawMessage()
{
	if (MsgStr.IsEmpty()) {
		if (NyanFiForm->ImgViewPanel->Visible) {
			if (ShowSttBar)
				NyanFiForm->StatusBar1->Panels->Items[0]->Text = EmptyStr;
			else
				NyanFiForm->ShowMessageHint();
		}
		else if (NyanFiForm->PreviewPanel->Visible) {
			NyanFiForm->PreviewSttLabel->Visible = false;
			NyanFiForm->PreviewSttLabel->Caption = EmptyStr;
		}
	}
	else {
		//イメージビューア
		if (NyanFiForm->ImgViewPanel->Visible) {
			if (ShowSttBar) {
				NyanFiForm->StatusBar1->Panels->Items[0]->Text = MsgStr;
				NyanFiForm->StatusBar1->Repaint();
				if (MsgIsErr) beep_Warn();
			}
			else if (ShowMsgHint && MsgIsErr) {
				NyanFiForm->ShowMessageHint(MsgStr, col_bgWarn, true);
			}
		}
		//プレビュー
		else if (NyanFiForm->PreviewPanel->Visible) {
			TLabel *lp = NyanFiForm->PreviewSttLabel;
			lp->Caption 	= MsgStr;
			lp->Font->Color = MsgIsErr? col_Error : col_Teal;
			lp->Top 		= NyanFiForm->PreviewPanel->ClientHeight - lp->Height - 4;
			lp->Left		= 8;
			lp->Visible 	= true;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TImgViewThread::Execute()
{
	::CoInitialize(NULL);

	while (!Terminated) {
		//処理キューから取り出し
		TaskRWLock->BeginWrite();
		while (RequestList->Count>0) {
			TStringDynArray r_buf = SplitString(RequestList->Strings[0], "\t");
			RequestList->Delete(0);
			if (r_buf.Length>0) {
				UnicodeString cmd = r_buf[0];
				if		(SameStr(cmd, "CLEAR"))  ReqClear	= true;
				else if (SameStr(cmd, "REDRAW")) ReqRedraw = true;
				else if (SameStr(cmd, "RELOAD")) ReqReload = true;
				else if (SameStr(cmd, "WAIT"))   ReqWait	= true;
				else if (SameStr(cmd, "EMPTY"))  FileName	= FileName2 = EmptyStr;
				else if (contains_wd_i(cmd, "FILE|VIDEO|USEBUF")) {
					ReqVideo	= SameStr(cmd, "VIDEO");
					UseViewBuff = SameStr(cmd, "USEBUF");
					if (r_buf.Length>1) FileName  = r_buf[1];
					FileName2 = (r_buf.Length>2)? r_buf[2] : EmptyStr;
				}
				else if (r_buf.Length>1) {
					if		(SameStr(cmd, "FITTED"))	Fitted	   = SameStr(r_buf[1], "1");
					else if (SameStr(cmd, "GRAY"))		GrayScaled = SameStr(r_buf[1], "1");
					else if (SameStr(cmd, "GRID"))		ShowGrid   = SameStr(r_buf[1], "1");
					else if (SameStr(cmd, "ZOOM"))		ZoomRatio  = r_buf[1].ToIntDef(100);
					else if (SameStr(cmd, "ROTATION"))	Rotation   = r_buf[1].ToIntDef(0);
					else if (SameStr(cmd, "EXIFORI"))	ExifOri    = r_buf[1].ToIntDef(0);
				}
			}
		}
		TaskRWLock->EndWrite();

		//描画要求
		if (ReqRedraw || ReqReload) {
			Synchronize(&DrawImage);
			TaskRWLock->BeginWrite();
			ReqRedraw = ReqReload = false;
			TaskRWLock->EndWrite();
		}
		//クリア要求
		else if (ReqClear) {
			Synchronize(&ClearImage);
			ImgBuff->Handle  = NULL;
			ViewBuff->Handle = NULL;
			TaskRWLock->BeginWrite();  ReqClear = false;  TaskRWLock->EndWrite();
		}
		//待ち要求
		else if (ReqWait) {
			TaskBusy = true;
			Sleep(MinShowTime);
			TaskBusy = false;
			TaskRWLock->BeginWrite();  ReqWait = false;  TaskRWLock->EndWrite();
		}
		//ファイルから読み込み
		else if (!FileName.IsEmpty()) {
			TaskBusy = true;
			try {
				if (!file_exists(FileName)) throw Exception(EmptyStr);
				UnicodeString fnam = FileName;
				UnicodeString fext = ExtractFileExt(fnam);
				__int64	  f_size = get_file_size(fnam);
				TDateTime f_time = get_file_age(fnam);
				bool is_prv_buf = false;

				//アイコン/フォントのプレビュー
				if (UseViewBuff) {
					UseViewBuff = false;
					is_prv_buf	= true;
					Img_f_name	= fnam; Img_f_size = f_size; Img_f_time = f_time;
					ImgIsWmf	= false;
					ImgBuff->Handle = NULL;
					ChgPrvCur = false;

					std::unique_ptr<Graphics::TBitmap> bg_bmp(new Graphics::TBitmap());
					bg_bmp->SetSize(ViewBuff->Width, ViewBuff->Height);
					TCanvas *cv = bg_bmp->Canvas;
					cv->Lock();
					try {
						AssignScaledFont(cv->Font, FileInfFont, Application->MainForm, 9, SelectWorB(col_bgImage));
						int fh = cv->TextHeight("Q") + SCALED_MAIN(2);

						//背景
						cv->Brush->Color = col_bgImage;
						cv->FillRect(Rect(0, 0, bg_bmp->Width, bg_bmp->Height));

						//フォント
						if (test_FontExt(fext)) {
							UnicodeString fnt_name = get_FontInf(Img_f_name);
							if (!fnt_name.IsEmpty()) {
								//ユーザ指定色
								if (FontSampleBgCol!=col_None) {
									cv->Brush->Color = FontSampleBgCol;
									cv->FillRect(Rect(0, 0, bg_bmp->Width, bg_bmp->Height));
								}
								if (FontSampleFgCol!=col_None) cv->Font->Color = FontSampleFgCol;

								//フォント名
								cv->TextOut(SCALED_MAIN(2), SCALED_MAIN(2), fnt_name);

								bool tmp_flag = (Screen->Fonts->IndexOf(fnt_name)==-1)?
												(::AddFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL)>0) : false;

								//サンプル表示
								std::unique_ptr<TFont> tmp_font(new TFont());
								tmp_font->Name	= fnt_name;
								tmp_font->Color = cv->Font->Color;
								AssignScaledFont(cv->Font, tmp_font.get(), Application->MainForm, FontSampleSize);

								TEXTMETRIC tm;
								if (::GetTextMetrics(cv->Handle, &tm)) {
									TRect rc = Rect(0, 0, bg_bmp->Width, bg_bmp->Height);
									rc.Left = SCALED_MAIN(2);
									rc.Top  = SCALED_MAIN(4) + fh;
									//文字/シンボル
									UnicodeString lbuf = (tm.tmCharSet==SYMBOL_CHARSET)? FontSampleSym : FontSampleTxt;
									::DrawText(cv->Handle, lbuf.c_str(), -1, &rc, DT_LEFT);

									//基準線の描画
									if (FontSampleShowGrid && FontSampleGridCol!=col_None) {
										DynamicArray<int> y_lst;
										y_lst.Length = 4;
										y_lst[0] = rc.Top;							//上端
										y_lst[1] = y_lst[0] + tm.tmInternalLeading;	//内部レディング
										y_lst[2] = y_lst[0] + tm.tmAscent;			//ベースライン(アセント)
										y_lst[3] = y_lst[2] + tm.tmDescent;			//下端

										cv->Pen->Style = psSolid;
										cv->Pen->Width = ScaledInt(1);
										cv->Pen->Color = FontSampleGridCol;
										for (int i=0; i<y_lst.Length; i++) {
											cv->MoveTo(0, y_lst[i]);  cv->LineTo(rc.Right, y_lst[i]);
										}
									}
								}

								if (tmp_flag) ::RemoveFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL);
							}
						}
						//色見本
						else if (IsSwatchbook(fnam)) {
							if (LoadSwatchbook(fnam, true)>0) {
								TRect rc = Rect(0,0, std::min(bg_bmp->Width - 8, 280), std::min(bg_bmp->Height - 8, 144));
								rc.Location = Point(4, 8);
								DrawSwatchbook(cv, rc, true);
							}
						}
						//アイコン
						else {
							int x = SCALED_MAIN(2);
							int y = SCALED_MAIN(2);
							int h = 0;

							if (!test_FontExt(fext)) {
								int size_lst[5] = {128, 64, 48, 32, 16};
								int top_i = ends_PathDlmtr(Img_f_name)? 1: test_FileExt(fext, FEXT_ICONVIEW)? 0 : 3;
								for (int i=top_i; i<5; i++) {
									int size = size_lst[i];
									HICON hIcon = usr_SH->get_Icon(Img_f_name, size);
									if (hIcon && size==size_lst[i]) {
										cv->TextOut(x + SCALED_MAIN(2), y, size);
										::DrawIconEx(cv->Handle, x, y + fh, hIcon, size, size, 0, NULL, DI_NORMAL);
										::DestroyIcon(hIcon);
										if (size+fh > h) h = size + fh;
										if (size==64 && x>128) y += (68 + fh); else x += size + 8;
									}
								}
							}

							//抽出アイコン
							if (test_FileExt(fext, FEXT_ICONVIEW)) {
								int ixn = (int)::ExtractIcon(HInstance, Img_f_name.c_str(), -1);
								if (ixn>1) {
									x = SCALED_MAIN(2);
									if (h>0) y = h + SCALED_MAIN(12);
									for (int i=0; i<ixn; i++) {
										HICON hIcon = ::ExtractIcon(HInstance, fnam.c_str(), i);
										if (!hIcon) continue;
										::DrawIconEx(cv->Handle, x, y, hIcon, 32, 32, 0, NULL, DI_NORMAL);
										::DestroyIcon(hIcon);
										if ((x + 2*32) > bg_bmp->Width) {
											x = SCALED_MAIN(2);  y += 34;
										}
										else {
											x += 34;
										}
									}
								}
							}

							if (SetPrvCursor && (test_CurExt(fext) || test_AniExt(fext))) {
								Screen->Cursors[crTmpPrev] = (HCURSOR)::LoadImage(HInstance, Img_f_name.c_str(),
																IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE|LR_DEFAULTSIZE);
								ChgPrvCur = true;
							}
						}

						ViewBuff->Assign(bg_bmp.get());
					}
					catch (...) {
						ViewBuff->Handle = NULL;
					}
					cv->Unlock();
				}
				//その他のファイル
				else if (Img_f_name!=fnam || Img_f_size!=f_size || Img_f_time!=f_time) {
					Img_f_name = fnam; Img_f_size = f_size; Img_f_time = f_time;
					ImgIsWmf = test_MetaExt(fext);
					ImgBuff->Handle  = NULL;
					ViewBuff->Handle = NULL;

					MsgStr	 = "読込中...";
					MsgIsErr = false;
					Synchronize(&DrawMessage);

					//動画
					if (ReqVideo || test_FileExt(fext, FEXT_VIDEO)) {
						ReqVideo = false;
						if (!usr_SH->get_Thumbnail(Img_f_name, ImgBuff, 2048)) TextAbort(_T("サムネイルが取得できません"));
					}
					//メタファイル
					else if (ImgIsWmf) {
						MetaBuff->LoadFromFile(Img_f_name);
						Img_rot_str = EmptyStr;
					}
					//PSP
					else if (test_PspExt(fext)) {
						if (!get_PspThumbnail(Img_f_name, ImgBuff)) {
							TaskRWLock->BeginWrite();  ReqClear = false;  TaskRWLock->EndWrite();
						}
					}
					//MP3
					else if (test_Mp3Ext(fext)) {
						if (!ID3_GetImage(Img_f_name, ImgBuff)) {
							TaskRWLock->BeginWrite();  ReqClear = false;  TaskRWLock->EndWrite();
						}
					}
					//FLAC
					else if (test_FlacExt(fext)) {
						if (!get_FlacImage(Img_f_name, ImgBuff)) {
							TaskRWLock->BeginWrite(); ReqClear = false;  TaskRWLock->EndWrite();
						}
					}
					//ディレクトリの特大アイコン
					else if (ends_PathDlmtr(Img_f_name) && ShowDirJumboIco) {
						UnicodeString dnam = ExcludeTrailingPathDelimiter(Img_f_name);
						int size = 256;
						std::unique_ptr<Graphics::TBitmap> bg_bmp(new Graphics::TBitmap());
						bg_bmp->SetSize(size, size);
						TCanvas *cv = bg_bmp->Canvas;
						cv->Lock();
						{
							cv->Brush->Color = get_WinColor();
							cv->FillRect(Rect(0, 0, size, size));
							//サムネイル
							std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
							if (usr_SH->get_Thumbnail(dnam, bmp.get(), size)) {
								bmp->AlphaFormat = afDefined;
								cv->Draw(0, 0, bmp.get());
							}
							//アイコン
							else {
								HICON hIcon = usr_SH->get_Icon(dnam, size);
								if (hIcon) {
									::DrawIconEx(cv->Handle, 0, 0, hIcon, size, size, 0, NULL, DI_NORMAL);
									::DestroyIcon(hIcon);
								}
							}
							ImgBuff->Assign(bg_bmp.get());
						}
						cv->Unlock();
						Rotation = 0;
					}
					//ZIP内画像
					else if (test_FileExt(fext, FEXT_ZIPIMG)) {
						int res = 0;
						UnicodeString tmp_name = ExtractInZipImg(Img_f_name, get_img_fext());
						if (!tmp_name.IsEmpty()) {
							res = load_ImageFile(tmp_name, ImgBuff, WICIMG_PREVIEW, col_bgImage);
							DeleteFile(tmp_name);
						}
						if (res==0) UserAbort(USTR_FaildLoad);
						Rotation = 0;
					}
					//画像
					else if (test_FileExt(fext, FEXT_META + get_img_fext())
						|| (EndsText(FAVICON_ADS, Img_f_name) && test_Png(Img_f_name)))
					{
						int res = load_ImageFile(Img_f_name, ImgBuff, WICIMG_PREVIEW, col_bgImage);
						if (res==0) UserAbort(USTR_FaildLoad);
						//見開きモード
						if (DoublePage) {
							if (!FileName2.IsEmpty()) {
								std::unique_ptr<Graphics::TBitmap> i2buf(new Graphics::TBitmap());
								if (load_ImageFile(FileName2, i2buf.get(), WICIMG_PREVIEW, col_bgImage)==0)
									UserAbort(USTR_FaildLoad);

								int i_h1 = ImgBuff->Height;
								int i_h2 = i2buf->Height;
								//見開き間隔の計算
								int w_dp = 0;
								if (ImgDblMargin>0) {
									int vw = NyanFiForm->ImgScrollPanel->ClientWidth - ImgFrameMargin*2 - ImgDblMargin;
									int vh = NyanFiForm->ImgScrollPanel->ClientHeight - ImgFrameMargin*2;
									int ih = std::max(i_h1, i_h2);
									int iw = ImgBuff->Width + i2buf->Width;
									bool v_is_v = (vh>vw);
									bool i_is_v = (ih>iw);
									float r;
									if ((v_is_v && i_is_v) || (!v_is_v && !i_is_v))
										r = std::max(std::min(1.0 * vw/ih, 1.0 * vh/iw), std::min(1.0 * vw/iw, 1.0 * vh/ih));
									else
										r = std::min(std::min(1.0 * vw/ih, 1.0 * vh/iw), std::min(1.0 * vw/iw, 1.0 * vh/ih));
									if (r>1.0) r = 1.0;
									if (r>0) w_dp = ImgDblMargin / r;
								}
								//画像の結合
								std::unique_ptr<Graphics::TBitmap> wbuf(new Graphics::TBitmap());
								wbuf->SetSize(
									ImgBuff->Width + i2buf->Width + w_dp, std::max(ImgBuff->Height, i2buf->Height));
								TCanvas *w_cv = wbuf->Canvas;
								w_cv->Lock();
								{
									w_cv->Brush->Color = col_bgDblPg;
									w_cv->FillRect(Rect(0, 0, wbuf->Width, wbuf->Height));
									int xp = 0, yp;
									//右綴じ
									if (RightBind) {
										yp = (i_h2<i_h1)? (i_h1 - i_h2)/2 : 0;
										w_cv->Draw(xp, yp, i2buf.get());
										xp += w_dp + i2buf->Width;
										yp = (i_h1<i_h2)? (i_h2 - i_h1)/2 : 0;
										w_cv->Draw(xp, yp, ImgBuff);
									}
									//左綴じ
									else {
										yp = (i_h1<i_h2)? (i_h2 - i_h1)/2 : 0;
										w_cv->Draw(xp, yp, ImgBuff);
										xp += w_dp + ImgBuff->Width;
										yp = (i_h2<i_h1)? (i_h1 - i_h2)/2 : 0;
										w_cv->Draw(xp, yp, i2buf.get());
									}
									ImgBuff->Assign(wbuf.get());
								}
								w_cv->Unlock();
							}
							Rotation = 0;
						}
						//通常モード
						else {
							if (RotViewImg && ExifOri>0 && res!=LOADED_BY_WIC) {
								if		(ExifOri==6) Rotation = 1;
								else if (ExifOri==3) Rotation = 2;
								else if (ExifOri==8) Rotation = 3;
							}
						}
						ExifOri = 0;
					}
					MsgStr = EmptyStr;
					Synchronize(&DrawMessage);
				}

				//表示
				if (is_prv_buf) {
					Synchronize(&DrawViewBuff);
				}
				else {
					if (SameText(FileName, Img_f_name) && !ReqClear) Synchronize(&DrawImage);
					Sleep(MinShowTime);
				}

				if (SameText(FileName, Img_f_name)) FileName = FileName2 = EmptyStr;
			}
			catch (EAbort &e) {
				if (SameText(FileName, Img_f_name)) {
					FileName  = EmptyStr;
					FileName2 = EmptyStr;
				}
				Synchronize(&ClearImage);
				MsgStr = e.Message; MsgIsErr = true; Synchronize(&DrawMessage);
				Img_f_name = EmptyStr;  Img_f_size = 0;  Img_f_time = 0;  ImgIsWmf = false;
				ExifOri  = 0;
			}
			catch (...) {
				FileName  = EmptyStr;
				FileName2 = EmptyStr;
				Synchronize(&ClearImage);
				MsgStr = EmptyStr; Synchronize(&DrawMessage);
				Img_f_name = EmptyStr;  Img_f_size = 0;  Img_f_time = 0;  ImgIsWmf = false;
			}

			ReqVideo = false;
			TaskBusy = false;
		}
		else {
			Sleep(10);
		}
	}
	
	delete ImgBuff;
	delete ViewBuff;
	delete ChgBuff;
	delete MetaBuff;
	delete RequestList;
	delete TaskRWLock;

	::CoUninitialize();
}
//---------------------------------------------------------------------------
