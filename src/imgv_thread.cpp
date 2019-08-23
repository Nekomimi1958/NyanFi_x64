//----------------------------------------------------------------------//
// NyanFi																//
//  �摜�r���[�����X���b�h												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
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
//�X���b�h�̏�����
//---------------------------------------------------------------------------
__fastcall TImgViewThread::TImgViewThread(bool CreateSuspended) : TThread(CreateSuspended)
{
	Priority = tpLower;
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
			ret = contains_wd_i(cmd, _T("REDRAW|RELOAD|CLEAR"));
		}
	}
	TaskRWLock->EndRead();
	return ret;
}

//---------------------------------------------------------------------------
//�L���[�ɏ����v����ǉ�
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
//�摜���N���A(Synchronize �ŌĂяo������)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::ClearImage()
{
	TImage *v_img = NULL;

	//�C���[�W�r���A�[
	if (NyanFiForm->ImgViewPanel->Visible) {
		v_img = NyanFiForm->ViewerImage;
	}
	//�v���r���[
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
		{
			cv->Brush->Color = col_bgImage;
			cv->FillRect(v_img->ClientRect);
		}
		cv->Unlock();
	}

	Img_f_name = EmptyStr;  Img_f_size = 0;  Img_f_time = 0;
	ImgIsWmf  = false;
	ChgPrvCur = false;

	NyanFiForm->ImgInfListBox->Clear();
	if (HistForm->Visible) HistForm->DrawHistogram();
}

//---------------------------------------------------------------------------
//�摜��\�� (Synchronize �ŌĂяo������)
//---------------------------------------------------------------------------
void __fastcall TImgViewThread::DrawImage()
{
	if (ImgIsWmf && MetaBuff->Empty) return;
	if (!ImgIsWmf && ImgBuff->Empty) return;

	TImage *v_img	= NULL;
	TPanel *v_panel = NULL;

	//�C���[�W�r���A�[
	if (NyanFiForm->ImgViewPanel->Visible) {
		v_img	= NyanFiForm->ViewerImage;
		v_panel = NyanFiForm->ImgScrollPanel;
	}
	//�v���r���[
	else if (NyanFiForm->PreviewPanel->Visible) {
		v_img	= NyanFiForm->PreviewImage;
		v_img->Transparent = false;
		v_panel = NyanFiForm->PreviewPanel;
	}

	if (!v_img) return;

	bool is_viewer = USAME_TS(v_img->Name, "ViewerImage");

	//��]
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
		//�v���r���[�܂��̓t�B�b�g�\��
		if (!is_viewer || Fitted) {
			r = get_ZoomRatio(v_panel->ClientWidth, v_panel->ClientHeight, iw, ih,
				is_viewer? ImgFrameMargin : 0,
				Fitted? (std::max(ImgFitMaxZoom, 100)/100.0) : 1.0);
			if (is_viewer) ZoomRatio = r * 100;
		}
		//�Y�[��
		else if (is_viewer && ZoomRatio!=100) {
			r = ZoomRatio / 100.0;
		}

		ZoomRatioF = r;

		//���^�t�@�C��
		if (ImgIsWmf) {
			if (r!=1.0) {
				iw *= r;  ih *= r;
			}
			ViewBuff->SetSize(iw, ih);
			TRect rc = Rect(0, 0, iw, ih);
			TCanvas *cv = ViewBuff->Canvas;
			cv->Lock();
			{
				cv->Brush->Color = col_bgWMF;
				cv->FillRect(rc);
				cv->StretchDraw(rc, MetaBuff);
			}
			cv->Unlock();
			//��]�E���]
			if (Rotation>0) {
				Img_rot_str.cat_sprintf(_T("%u"), Rotation);
				Rotation = 0;
			}
			for (int i=1; i<=Img_rot_str.Length(); i++)
				WIC_rotate_image(ViewBuff, Img_rot_str.SubString(i, 1).ToIntDef(0));
			//�璷�ȏ����������폜
			if (Img_rot_str.Length()>1)
				Img_rot_str = replace_regex(Img_rot_str, _T("13|31|44|55|1111|3333"), null_TCHAR);
		}
		//�摜�t�@�C��
		else {
			if (r!=1.0) {
				//�g��E�k��
				if (WicScaleOpt>=0 && WicScaleOpt<=4) {
					WIC_resize_image(ImgBuff, ViewBuff, r, 0, 0, WicScaleOpt);
				}
				else {
					iw *= r;  ih *= r;
					ViewBuff->SetSize(iw, ih);
					ViewBuff->PixelFormat = pf24bit;
					ViewBuff->Canvas->StretchDraw(Rect(0, 0, iw, ih), ImgBuff);
				}
			}
			else {
				//���{
				ViewBuff->Assign(ImgBuff);
			}
		}

		//�O���[�X�P�[����
		if (GrayScaled) WIC_grayscale_image(ViewBuff);

		if (is_viewer) {
			UnicodeString sttstr; sttstr.sprintf(_T("%3u%"), ZoomRatio);
			if (ZoomRatio!=100) sttstr += (" " + UnicodeString("NLCFHX").SubString(WicScaleOpt + 1, 1));
			if (NyanFiForm->ImgSttHeader->Visible) NyanFiForm->ImgSttHeader->Panels->Items[3]->Text = sttstr;
			if (NyanFiForm->ImgInfBar->Visible)    NyanFiForm->ImgInfBar->Panels->Items[1]->Text 	= sttstr;
		}
	}

	//�`��
	v_img->Center = true;

	//�C���[�W�r���A�[
	if (is_viewer) {
		TControlScrollBar *hbar = NyanFiForm->ImgScrollBox->HorzScrollBar;
		TControlScrollBar *vbar = NyanFiForm->ImgScrollBox->VertScrollBar;

		if (ViewBuff->Width>v_panel->ClientWidth || ViewBuff->Height>v_panel->ClientHeight) {
			//�X�N���[���o�[�ݒ�
			hbar->Range = ViewBuff->Width;
			vbar->Range = ViewBuff->Height;
			//�Z���^�[�Ɉړ�
			hbar->Position = (hbar->Range - v_panel->ClientWidth)/2;
			vbar->Position = (vbar->Range - v_panel->ClientHeight)/2;
		}
		else {
			hbar->Range = v_panel->ClientWidth;
			vbar->Range = v_panel->ClientHeight;
		}

		NyanFiForm->ImgScrollBox->Color = ImgIsWmf? col_bgWMF : col_bgImage;
		v_img->Picture->Bitmap->Assign(ViewBuff);

		//�����O���b�h
		if (is_viewer && ShowGrid) draw_ImgGrid(v_img->Picture->Bitmap->Canvas, ViewBuff);

		NyanFiForm->SetSttBarInf();

		//�q�X�g�O����
		if (HistForm->Visible) HistForm->DrawHistogram(ImgBuff);

		//����ݒ�
		if (PrintImgDlg && PrintImgDlg->Visible) PrintImgDlg->DrawImage();
	}
	//�v���r���[�\��
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
//�C���[�W�v���r���[�� ViewBuff ��\�� (Synchronize �ŌĂяo������)
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
//���b�Z�[�W��\�� (Synchronize �ŌĂяo������)
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
		//�C���[�W�r���A�[
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
		//�v���r���[
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
		//�����L���[������o��
		TaskRWLock->BeginWrite();
		while (RequestList->Count>0) {
			TStringDynArray r_buf = SplitString(RequestList->Strings[0], "\t");
			RequestList->Delete(0);
			if (r_buf.Length>0) {
				UnicodeString cmd = r_buf[0];
				if		(USAME_TS(cmd, "CLEAR"))  ReqClear	= true;
				else if (USAME_TS(cmd, "REDRAW")) ReqRedraw = true;
				else if (USAME_TS(cmd, "RELOAD")) ReqReload = true;
				else if (USAME_TS(cmd, "WAIT"))   ReqWait	= true;
				else if (USAME_TS(cmd, "EMPTY"))  FileName	= FileName2 = EmptyStr;
				else if (contains_wd_i(cmd, _T("FILE|VIDEO|USEBUF"))) {
					ReqVideo	= USAME_TS(cmd, "VIDEO");
					UseViewBuff = USAME_TS(cmd, "USEBUF");
					if (r_buf.Length>1) FileName  = r_buf[1];
					FileName2 = (r_buf.Length>2)? r_buf[2] : EmptyStr;
				}
				else if (r_buf.Length>1) {
					if		(USAME_TS(cmd, "FITTED"))	Fitted	   = USAME_TS(r_buf[1], "1");
					else if (USAME_TS(cmd, "GRAY"))		GrayScaled = USAME_TS(r_buf[1], "1");
					else if (USAME_TS(cmd, "GRID"))		ShowGrid   = USAME_TS(r_buf[1], "1");
					else if (USAME_TS(cmd, "ZOOM"))		ZoomRatio  = r_buf[1].ToIntDef(100);
					else if (USAME_TS(cmd, "ROTATION"))	Rotation   = r_buf[1].ToIntDef(0);
					else if (USAME_TS(cmd, "EXIFORI"))	ExifOri    = r_buf[1].ToIntDef(0);
				}
			}
		}
		TaskRWLock->EndWrite();

		//�`��v��
		if (ReqRedraw || ReqReload) {
			Synchronize(&DrawImage);
			TaskRWLock->BeginWrite();
			ReqRedraw = ReqReload = false;
			TaskRWLock->EndWrite();
		}
		//�N���A�v��
		else if (ReqClear) {
			Synchronize(&ClearImage);
			ImgBuff->Handle  = NULL;
			ViewBuff->Handle = NULL;
			TaskRWLock->BeginWrite();  ReqClear = false;  TaskRWLock->EndWrite();
		}
		//�҂��v��
		else if (ReqWait) {
			TaskBusy = true;
			Sleep(MinShowTime);
			TaskBusy = false;
			TaskRWLock->BeginWrite();  ReqWait = false;  TaskRWLock->EndWrite();
		}
		//�t�@�C������ǂݍ���
		else if (!FileName.IsEmpty()) {
			TaskBusy = true;
			try {
				if (!file_exists(FileName)) throw Exception(EmptyStr);
				UnicodeString fnam = FileName;
				UnicodeString fext = ExtractFileExt(fnam);
				__int64	  f_size = get_file_size(fnam);
				TDateTime f_time = get_file_age(fnam);
				bool is_prv_buf = false;

				//�A�C�R��/�t�H���g�̃v���r���[
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
						cv->Font->Assign(FileInfFont);
						cv->Font->Size	= 9;
						cv->Font->Color = SelectWorB(col_bgImage);
						int fh = cv->TextHeight("Q") + 2;

						//�w�i
						cv->Brush->Color = col_bgImage;
						cv->FillRect(Rect(0, 0, bg_bmp->Width, bg_bmp->Height));

						//�t�H���g
						if (test_FontExt(fext)) {
							UnicodeString fnt_name = get_FontInf(Img_f_name);
							if (!fnt_name.IsEmpty()) {
								//���[�U�w��F
								if (FontSampleBgCol!=col_None) {
									cv->Brush->Color = FontSampleBgCol;
									cv->FillRect(Rect(0, 0, bg_bmp->Width, bg_bmp->Height));
								}
								if (FontSampleFgCol!=col_None) cv->Font->Color = FontSampleFgCol;

								//�t�H���g��
								cv->TextOut(2, 2, fnt_name);

								bool tmp_flag = (Screen->Fonts->IndexOf(fnt_name)==-1)?
												(::AddFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL)>0) : false;

								//�T���v���\��
								std::unique_ptr<TFont> tmp_font(new TFont());
								tmp_font->Name	= fnt_name;
								tmp_font->Size	= FontSampleSize;
								tmp_font->Color = cv->Font->Color;
								cv->Font->Assign(tmp_font.get());

								TEXTMETRIC tm;
								if (::GetTextMetrics(cv->Handle, &tm)) {
									TRect rc = Rect(0, 0, bg_bmp->Width, bg_bmp->Height);
									rc.Left = 2; rc.Top = 4 + fh;
									//����/�V���{��
									UnicodeString lbuf = (tm.tmCharSet==SYMBOL_CHARSET)? FontSampleSym : FontSampleTxt;
									::DrawText(cv->Handle, lbuf.c_str(), -1, &rc, DT_LEFT);

									//����̕`��
									if (FontSampleShowGrid && FontSampleGridCol!=col_None) {
										DynamicArray<int> y_lst;
										y_lst.Length = 4;
										y_lst[0] = rc.Top;							//��[
										y_lst[1] = y_lst[0] + tm.tmInternalLeading;	//�������f�B���O
										y_lst[2] = y_lst[0] + tm.tmAscent;			//�x�[�X���C��(�A�Z���g)
										y_lst[3] = y_lst[2] + tm.tmDescent;			//���[

										cv->Pen->Style = psSolid;
										cv->Pen->Width = Scaled1;
										cv->Pen->Color = FontSampleGridCol;
										for (int i=0; i<y_lst.Length; i++) {
											cv->MoveTo(0, y_lst[i]);  cv->LineTo(rc.Right, y_lst[i]);
										}
									}
								}

								if (tmp_flag) ::RemoveFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL);
							}
						}
						//�F���{
						else if (IsSwatchbook(fnam)) {
							if (LoadSwatchbook(fnam, true)>0) {
								TRect rc = Rect(0,0, std::min(bg_bmp->Width - 8, 280), std::min(bg_bmp->Height - 8, 144));
								rc.Location = Point(4, 8);
								DrawSwatchbook(cv, rc, true);
							}
						}
						//�A�C�R��
						else {
							int x = 2, y = 2, h = 0;

							if (!test_FontExt(fext)) {
								int size_lst[5] = {128, 64, 48, 32, 16};
								int top_i = ends_PathDlmtr(Img_f_name)? 1: test_FileExt(fext, FEXT_ICONVIEW)? 0 : 3;
								for (int i=top_i; i<5; i++) {
									int size = size_lst[i];
									HICON hIcon = usr_SH->get_Icon(Img_f_name, size);
									if (hIcon && size==size_lst[i]) {
										cv->TextOut(x + 2, y, size);
										::DrawIconEx(cv->Handle, x, y + fh, hIcon, size, size, 0, NULL, DI_NORMAL);
										::DestroyIcon(hIcon);
										if (size+fh > h) h = size + fh;
										if (size==64 && x>128) y += (68 + fh); else x += size + 8;
									}
								}
							}

							//���o�A�C�R��
							if (test_FileExt(fext, FEXT_ICONVIEW)) {
								int ixn = (int)::ExtractIcon(HInstance, Img_f_name.c_str(), -1);
								if (ixn>1) {
									x = 2;
									if (h>0) y = h + 12;
									for (int i=0; i<ixn; i++) {
										HICON hIcon = ::ExtractIcon(HInstance, fnam.c_str(), i);
										if (!hIcon) continue;
										::DrawIconEx(cv->Handle, x, y, hIcon, 32, 32, 0, NULL, DI_NORMAL);
										::DestroyIcon(hIcon);
										if ((x + 2*32) > bg_bmp->Width) {
											x = 2;  y += 34;
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
				//���̑��̃t�@�C��
				else if (Img_f_name!=fnam || Img_f_size!=f_size || Img_f_time!=f_time) {
					Img_f_name = fnam; Img_f_size = f_size; Img_f_time = f_time;
					ImgIsWmf = test_MetaExt(fext);
					ImgBuff->Handle  = NULL;
					ViewBuff->Handle = NULL;

					MsgStr	 = "�Ǎ���...";
					MsgIsErr = false;
					Synchronize(&DrawMessage);

					//����
					if (ReqVideo || test_FileExt(fext, FEXT_VIDEO)) {
						ReqVideo = false;
						if (!usr_SH->get_Thumbnail(Img_f_name, ImgBuff, 2048)) TextAbort(_T("�T���l�C�����擾�ł��܂���"));
					}
					//���^�t�@�C��
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
					//�f�B���N�g���̓���A�C�R��
					else if (ends_PathDlmtr(Img_f_name) && ShowDirJumboIco) {
						UnicodeString dnam = ExcludeTrailingPathDelimiter(Img_f_name);
						int size = 256;
						std::unique_ptr<Graphics::TBitmap> bg_bmp(new Graphics::TBitmap());
						bg_bmp->SetSize(size, size);
						TCanvas *cv = bg_bmp->Canvas;
						cv->Lock();
						{
							cv->Brush->Color = scl_Window;
							cv->FillRect(Rect(0, 0, size, size));
							//�T���l�C��
							std::unique_ptr<Graphics::TBitmap> bmp(new Graphics::TBitmap());
							if (usr_SH->get_Thumbnail(dnam, bmp.get(), size)) {
								bmp->AlphaFormat = afDefined;
								cv->Draw(0, 0, bmp.get());
							}
							//�A�C�R��
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
					//ZIP���摜
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
					//�摜
					else if (test_FileExt(fext, FEXT_META + get_img_fext())) {
						int res = load_ImageFile(Img_f_name, ImgBuff, WICIMG_PREVIEW, col_bgImage);
						if (res==0) UserAbort(USTR_FaildLoad);
						//���J�����[�h
						if (DoublePage) {
							if (!FileName2.IsEmpty()) {
								std::unique_ptr<Graphics::TBitmap> i2buf(new Graphics::TBitmap());
								if (load_ImageFile(FileName2, i2buf.get(), WICIMG_PREVIEW, col_bgImage)==0)
									UserAbort(USTR_FaildLoad);

								int i_h1 = ImgBuff->Height;
								int i_h2 = i2buf->Height;
								//���J���Ԋu�̌v�Z
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
								//�摜�̌���
								std::unique_ptr<Graphics::TBitmap> wbuf(new Graphics::TBitmap());
								wbuf->SetSize(
									ImgBuff->Width + i2buf->Width + w_dp, std::max(ImgBuff->Height, i2buf->Height));
								TCanvas *w_cv = wbuf->Canvas;
								w_cv->Lock();
								{
									w_cv->Brush->Color = col_bgDblPg;
									w_cv->FillRect(Rect(0, 0, wbuf->Width, wbuf->Height));
									int xp = 0, yp;
									//�E�Ԃ�
									if (RightBind) {
										yp = (i_h2<i_h1)? (i_h1 - i_h2)/2 : 0;
										w_cv->Draw(xp, yp, i2buf.get());
										xp += w_dp + i2buf->Width;
										yp = (i_h1<i_h2)? (i_h2 - i_h1)/2 : 0;
										w_cv->Draw(xp, yp, ImgBuff);
									}
									//���Ԃ�
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
						//�ʏ탂�[�h
						else {
							if (RotViewImg && ExifOri>0 && res!=LOADED_BY_WIC) {
								if		(ExifOri==6) Rotation = 1;
								else if (ExifOri==8) Rotation = 3;
							}
						}
						ExifOri = 0;
					}
					MsgStr = EmptyStr;
					Synchronize(&DrawMessage);
				}

				//�\��
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
		else Sleep(10);
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
