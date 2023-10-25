//----------------------------------------------------------------------//
// 各種フォーマットのファイル情報取得									//
//																		//
//----------------------------------------------------------------------//
#include <mmreg.h>
#include <imagehlp.h>
#include <RestartManager.h>
#include "usr_str.h"
#include "usr_key.h"
#include "usr_file_ex.h"
#include "usr_file_inf.h"
#include "usr_shell.h"
#include "usr_exif.h"
#include "usr_wic.h"
#include "usr_msg.h"
#include "htmconv.h"

#if defined(_WIN64)
#pragma link "win64\\release\\psdk\\Rstrtmgr.a"
#else
#pragma link "win32\\release\\psdk\\Rstrtmgr.lib"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//Exif情報取得可能の拡張子か?
//---------------------------------------------------------------------------
bool test_ExifExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_EXIF);
}
//---------------------------------------------------------------------------
//HTML文書の拡張子か?
//---------------------------------------------------------------------------
bool test_HtmlExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_HTML);
}
//---------------------------------------------------------------------------
//フォントの拡張子か?
//---------------------------------------------------------------------------
bool test_FontExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_FONT);
}
//---------------------------------------------------------------------------
//ライブラリの拡張子か?
//---------------------------------------------------------------------------
bool test_LibExt(UnicodeString fext)
{
	return SameText(fext, ".library-ms");
}
//---------------------------------------------------------------------------
//.exe か?
//---------------------------------------------------------------------------
bool test_ExeExt(UnicodeString fext)
{
	return SameText(fext, ".exe");
}
//---------------------------------------------------------------------------
//ショートカットの拡張子か?
//---------------------------------------------------------------------------
bool test_LnkExt(UnicodeString fext)
{
	return SameText(fext, ".lnk");
}
//---------------------------------------------------------------------------
//アイコンの拡張子か?
//---------------------------------------------------------------------------
bool test_IcoExt(UnicodeString fext)
{
	return SameText(fext, ".ico");
}
//---------------------------------------------------------------------------
//カーソルの拡張子か?
//---------------------------------------------------------------------------
bool test_CurExt(UnicodeString fext)
{
	return SameText(fext, ".cur");
}
//---------------------------------------------------------------------------
//アニメーションカーソルの拡張子か?
//---------------------------------------------------------------------------
bool test_AniExt(UnicodeString fext)
{
	return SameText(fext, ".ani");
}
//---------------------------------------------------------------------------
//Jpegの拡張子か?
//---------------------------------------------------------------------------
bool test_JpgExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_JPEG);
}
//---------------------------------------------------------------------------
//GIFの拡張子か?
//---------------------------------------------------------------------------
bool test_GifExt(UnicodeString fext)
{
	return SameText(fext, ".gif");
}
//---------------------------------------------------------------------------
//PNGの拡張子か?
//---------------------------------------------------------------------------
bool test_PngExt(UnicodeString fext)
{
	return SameText(fext, ".png");
}
//---------------------------------------------------------------------------
//PSPの拡張子か?
//---------------------------------------------------------------------------
bool test_PspExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_PSP);
}
//---------------------------------------------------------------------------
//メタファイルの拡張子か?
//---------------------------------------------------------------------------
bool test_MetaExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_META);
}
//---------------------------------------------------------------------------
//MP3の拡張子か?
//---------------------------------------------------------------------------
bool test_Mp3Ext(UnicodeString fext)
{
	return SameText(fext, ".mp3");
}
//---------------------------------------------------------------------------
//FLACの拡張子か?
//---------------------------------------------------------------------------
bool test_FlacExt(UnicodeString fext)
{
	return SameText(fext, ".flac");
}
//---------------------------------------------------------------------------
//MSI対応サウンドの拡張子か?
//---------------------------------------------------------------------------
bool test_MciSndExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_MCISND);
}

//---------------------------------------------------------------------------
//コマンドファイルの拡張子か?
//---------------------------------------------------------------------------
bool test_NbtExt(UnicodeString fext)
{
	return SameText(fext, ".nbt");
}
//---------------------------------------------------------------------------
//ワークリストの拡張子か?
//---------------------------------------------------------------------------
bool test_NwlExt(UnicodeString fext)
{
	return SameText(fext, ".nwl");
}

//---------------------------------------------------------------------------
//テキストの拡張子か?
//---------------------------------------------------------------------------
bool test_TxtExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_TEXT);
}

//---------------------------------------------------------------------------
//アプリ情報取得可能な拡張子か?
//---------------------------------------------------------------------------
bool test_AppInfExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_APPINFO);
}

//---------------------------------------------------------------------------
//リストにエラーメッセージ("エラー: メッセージ") を追加
//---------------------------------------------------------------------------
void add_list_errmsg(
	TStringList *lst,		//[o] NULLでも可
	UnicodeString msg)
{
	if (lst) add_PropLine_if(_T("エラー"), msg, lst, LBFLG_ERR_FIF);
}

//---------------------------------------------------------------------------
//テキストファイルの先頭行を取得
//---------------------------------------------------------------------------
UnicodeString get_top_line(UnicodeString fnam,
	int code_page)	//コードページ	(default = 0 : 取得)
{
	UnicodeString ret_str;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		//コードページ取得
		std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
		bool has_bom;
		ms->CopyFrom(fs.get(), std::min<__int64>(fs->Size, TXT_DETECT_SIZE));
		if (code_page==0) {
			code_page = get_MemoryCodePage(ms.get(), &has_bom);
			if (code_page<=0) Abort();
		}

		//先頭行のサイズ取得
		bool is_BE = (code_page==1201);
		int  ch_sz = (code_page==1200 || code_page==1201)? 2 : 1;
		BYTE cbuf[2]; cbuf[1] = 0;

		int top_p = has_bom? ((code_page==1200 || code_page==1201)? 2 : (code_page==CP_UTF8)? 3 : 0) : 0;
		int size  = 0;
		ms->Seek(top_p,  soFromBeginning);
		while (ms->Position < ms->Size) {
			if (ms->Read(cbuf, ch_sz)!=ch_sz) break;
			unsigned short c = is_BE? ((cbuf[0] << 8) | cbuf[1]) : ((cbuf[1] << 8) | cbuf[0]);
			if (c==0x000d || c==0x000a) break;
			size += ch_sz;
		}
		if (size==0) Abort();

		//バッファに読み込み
		TBytes Bytes;
		Bytes.Length = size;
		ms->Seek(top_p, soFromBeginning);
		ms->Read(Bytes, size);

		//エンコード
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
		ret_str = enc->GetString(Bytes, 0, Bytes.Length);
	}
	catch (...) {
		ret_str = EmptyStr;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//ファイルの現在位置から4文字の識別子を取得
//  戻り値: 識別子		EmptyStr: エラー
//---------------------------------------------------------------------------
UnicodeString get_id_str4(TFileStream *fs)
{
	try {
		BYTE buf[4];
		fs->ReadBuffer(&buf, 4);
		UnicodeString ret_str = UnicodeString((char *)&buf, 4);
		if (ret_str.Length()!=4) Abort();
		return ret_str;
	}
	catch (...) {
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//ファイルの現在位置からチャンクを検索
//  あれば直後位置(サイズ)へ
//---------------------------------------------------------------------------
bool sea_chunk(TFileStream *fs, const char *s)
{
	int size = strlen(s);
	if (size!=4) return false;

	try {
		std::unique_ptr<BYTE[]> buf(new BYTE[size]);

		for (;;) {
			fs->ReadBuffer(buf.get(), size);
			bool ok = true;
			for (int i=0; i<size && ok; i++) {
				if (!isprint(buf[i])) Abort();
				if (s[i]!=buf[i]) ok = false;
			}
			if (ok) break;

			DWORD dwLen;
			fs->ReadBuffer(&dwLen, 4);
			if (dwLen<2) Abort();
			int p = fs->Seek(0, soFromCurrent) + dwLen;
			fs->Seek(p, soFromBeginning);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//ファイルの現在位置からチャンク情報を取得
//  戻り値: 識別子		EmptyStr: エラー
//---------------------------------------------------------------------------
UnicodeString get_chunk_hdr(
	TFileStream *fs,
	unsigned int *size)		//[o] チャンクサイズ
{
	try {
		BYTE buf[8];
		fs->ReadBuffer(&buf, 4);
		UnicodeString ret_str = UnicodeString((char *)&buf, 4);
		if (ret_str.Length()!=4) Abort();

		fs->ReadBuffer(&buf, 4);
		if (size) *size = (unsigned int)(buf[0] | (buf[1] << 8) | (buf[2] << 16) | (buf[3] << 24));

		return ret_str;
	}
	catch (...) {
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//WAVファイルの情報を取得
//---------------------------------------------------------------------------
void get_WavInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!SameStr(get_chunk_hdr(fs.get()), "RIFF") || !fsRead_check_char(fs.get(), "WAVE")) UserAbort(USTR_IllegalFormat);
		if (!sea_chunk(fs.get(),  "fmt "))	UserAbort(USTR_IllegalFormat);

		DWORD dwBuf;
		fs->ReadBuffer(&dwBuf, 4);
		int p_data = fs->Seek(0, soFromCurrent) + dwBuf;

		//フォーマット情報を取得
		PCMWAVEFORMAT pwf;
		fs->ReadBuffer(&pwf, sizeof(PCMWAVEFORMAT));
		if (pwf.wf.wFormatTag==WAVE_FORMAT_PCM || pwf.wf.wFormatTag==WAVE_FORMAT_EXTENSIBLE) {
			if (pwf.wf.nChannels==0) TextAbort(_T("不正なチャンネル数です。"));
			int chn    = pwf.wf.nChannels;
			int s_rate = pwf.wf.nSamplesPerSec;
			int s_byte = pwf.wf.nBlockAlign/chn;
			int s_bit  = s_byte * 8;

			fs->Seek(p_data, soFromBeginning);
			if (!sea_chunk(fs.get(), "data")) TextAbort(_T("dataチャンクが見つかりません。"));

			fs->ReadBuffer(&dwBuf, 4);
			int length = (int)(dwBuf/(s_byte*chn)/(s_rate/1000.0));

			UnicodeString lbuf = get_PropTitle(_T("形式")).cat_sprintf(_T("%uHz %ubit "), s_rate, s_bit);
			if (chn<=2)
				lbuf.cat_sprintf(_T("%s"), (chn==2)? _T("ステレオ") : _T("モノ"));
			else
				lbuf.cat_sprintf(_T("%uch"), chn);
			lst->Add(lbuf);
			add_PropLine(_T("長さ"), mSecToTStr(length), lst);
		}
		else if (pwf.wf.wFormatTag==WAVE_FORMAT_MPEGLAYER3) {
			add_PropLine(_T("形式"), "MPEG 3 Layer 1", lst);
		}
		else {
			lst->Add(get_PropTitle(_T("形式")).cat_sprintf(_T("その他(0x%04X)"), pwf.wf.wFormatTag));
		}
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
}

//---------------------------------------------------------------------------
//Opusファイルの情報を取得
//---------------------------------------------------------------------------
unsigned int get_Ogg_seg_size(TFileStream *fs)
{
	unsigned int seg_sz = 0;
	BYTE seg_n, seg_d;
	if (fs->Read(&seg_n, 1)==1) {
		std::unique_ptr<BYTE[]> sbuf(new BYTE[seg_n]);
		if (fs->Read(sbuf.get(), seg_n)==seg_n) {
			for (int i=0; i<seg_n; i++) seg_sz += sbuf[i];
		}
	}
	return seg_sz;
}
//---------------------------------------------------------------------------
void get_OpusInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!fsRead_check_char(fs.get(), "OggS"))	Abort();
		fs->Seek(22, soFromCurrent);

		int seg_sz = get_Ogg_seg_size(fs.get());  if (seg_sz==0) Abort();
		int p = fs->Seek(0, soFromCurrent);

		if (!fsRead_check_char(fs.get(), "OpusHead")) Abort();
		fs->Seek(1, soFromCurrent);		//version
		BYTE chn;
		fs->ReadBuffer(&chn, 1);
		add_PropLine(_T("チャンネル数"), chn, lst);
		fs->Seek(2, soFromCurrent);		//pre-skip
		DWORD i_rate;
		fs->ReadBuffer(&i_rate, 4);
		add_PropLine(_T("サンプルレート"), UnicodeString().sprintf(_T("%uHz"), i_rate), lst);

		fs->Seek(p + seg_sz, soFromBeginning);
		if (fsRead_check_char(fs.get(), "OggS")) fs->Seek(22, soFromCurrent);

		seg_sz = get_Ogg_seg_size(fs.get());  if (seg_sz==0) Abort();
		if (!fsRead_check_char(fs.get(), "OpusTags")) Abort();

		//Vendor String
		UnicodeString s = fsRead_comment_utf8(fs.get());
		if (!s.IsEmpty()) add_PropLine(_T("VENDOR"), s, lst);
		//User Comment
		int n = fsRead_int4(fs.get());
		for (int i=0; i<n; i++) {
			UnicodeString lbuf = fsRead_comment_utf8(fs.get());
			if (lbuf.Pos("=")) add_PropLine(get_tkn(lbuf, '='), get_tkn_r(lbuf, '='), lst);
		}
	}
	catch (EAbort &e) {
		;
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
}

//---------------------------------------------------------------------------
//CDAファイルの情報を取得
//---------------------------------------------------------------------------
bool get_CdaInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//[o] 情報リスト	(NULLでも可)
	int *len)				//[o] 長さ
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!SameStr(get_chunk_hdr(fs.get()), "RIFF") || !fsRead_check_char(fs.get(), "CDDA")) Abort();
		if (!sea_chunk(fs.get(), "fmt ")) Abort();
		if (fsRead_int4(fs.get())!=24) Abort();
		fs->Seek(2, soFromCurrent);
		//トラック＃
		int n = fsRead_int2(fs.get());
		if (lst) add_PropLine(_T("トラック＃"), n, lst);
		fs->Seek(4, soFromCurrent);
		//開始位置
		int t = fsRead_int4(fs.get())*1000/75;	//= セクタ数*1000/75 (75=1秒間のセクタ数)
		if (lst) add_PropLine(_T("開始位置"), mSecToTStr(t), lst);
		//長さ
		t = fsRead_int4(fs.get())*1000/75;		//= セクタ数*1000/75 (75=1秒間のセクタ数)
		if (len) *len = t;
		if (lst) add_PropLine(_T("長さ"), mSecToTStr(t), lst);
		ret = true;
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, LoadUsrMsg(USTR_IllegalFormat));
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
	return ret;
}

//---------------------------------------------------------------------------
//FLACファイルの情報を取得
//---------------------------------------------------------------------------
bool get_FlacInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//結果リスト(項目=内容 の形式で格納)	(default = NULL)
	int *len,				//[o] 長さ								(default = NULL)
	bool id_min)			//情報を 名前=値 形式で取得				(default = false)
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		BYTE xbuf[4];
		fs->ReadBuffer(xbuf, 4);
		if (xbuf[0]!='f' || xbuf[1]!='L' || xbuf[2]!='a' || xbuf[3]!='C') Abort();

		for (;;) {
			fs->ReadBuffer(xbuf, 1);
			bool last_blk = ((xbuf[0]&0x80)!=0);

			int dlen  = fsRead_int3(fs.get(), true);
			int nxt_p = fs->Seek(0, soFromCurrent) + dlen;
	
			switch (xbuf[0]) {
			case 0:	//STREAMINFO
				if (id_min) break;
				fs->Seek(10, soFromCurrent);
				fs->ReadBuffer(xbuf, 4);
				{
					//形式
					unsigned int s_rate = ((xbuf[0]<<16) | (xbuf[1]<<8) | xbuf[2])>>4;
					unsigned int s_bit	= ((xbuf[2] & 0x01)<<4) + (xbuf[3]>>4) + 1;
					unsigned int chn	= ((xbuf[2]>>1) & 0x07) + 1;
					UnicodeString tmp;
					if (lst) {
						UnicodeString lbuf = get_PropTitle(_T("形式")).cat_sprintf(_T("%uHz %ubit "), s_rate, s_bit);
						if (chn<=2)
							lbuf.cat_sprintf(_T("%s"), (chn==2)? _T("ステレオ") : _T("モノ"));
						else
							lbuf.cat_sprintf(_T("%uch"), chn);
						lst->Add(lbuf);
					}
					//長さ
					unsigned __int64 total = (__int64)(xbuf[2] & 0x04)<<32;
					fs->ReadBuffer(xbuf, 4);
					total += (unsigned int)((xbuf[0] << 24) | (xbuf[1] << 16) | (xbuf[2] << 8) | xbuf[3]);
					if (s_rate>0 && total>0) {
						unsigned int ms = total/s_rate*1000;
						if (lst) add_PropLine(_T("長さ"), mSecToTStr(ms, false), lst);
						if (len) *len = ms;
					}
				}
				break;

			case 4:	//VORBIS_COMMENT
				if (lst) {
					//Vendor
					UnicodeString lbuf = fsRead_comment_utf8(fs.get());
					if (!id_min && !lbuf.IsEmpty()) add_PropLine(_T("VENDOR"), lbuf, lst);
					//Comment
					int n = fsRead_int4(fs.get());
					for (int i=0; i<n; i++) {
						lbuf = fsRead_comment_utf8(fs.get());
						if (lbuf.Pos("=")) {
							if (id_min)
								lst->Add(lbuf);
							else
								add_PropLine(get_tkn(lbuf, '='), get_tkn_r(lbuf, '='), lst);
						}
					}
				}
				break;
			}

			if (last_blk) break;
			fs->Seek(nxt_p, soFromBeginning);
		}

		ret = true;
	}
	catch (...) {
		if (lst) lst->Clear();
	}
	return ret;
}

//---------------------------------------------------------------------------
//FLACファイルから埋め込み画像をビットマップで取得 or ファイルに出力
//---------------------------------------------------------------------------
bool get_FlacImage(
	UnicodeString fnam,			//ファイル名
	Graphics::TBitmap *bmpimg,	//出力ビットマップ	(default = NULL)
	UnicodeString o_fnam)		//出力ファイル名	(default = EmptyStr)
{
	bool ok = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		BYTE xbuf[4];
		fs->ReadBuffer(xbuf, 4);
		if (xbuf[0]!='f' || xbuf[1]!='L' || xbuf[2]!='a' || xbuf[3]!='C') Abort();

		for (;;) {
			fs->ReadBuffer(xbuf, 1);
			bool last_blk = ((xbuf[0]&0x80)!=0);

			int dlen  = fsRead_int3(fs.get(), true);
			int nxt_p = fs->Seek(0, soFromCurrent) + dlen;

			if (xbuf[0]==6) {	//PICTURE
				fs->Seek(4, soFromCurrent);
				//MIMEタイプ
				int len = fsRead_int4(fs.get(), true);
				std::unique_ptr<char[]> sbuf(new char[len + 1]);
				fs->ReadBuffer(sbuf.get(), len);
				sbuf[len] = 0x00;
				UnicodeString img_typ = sbuf.get();
				//不用情報をスキップ
				len = fsRead_int4(fs.get(), true);
				fs->Seek((int)(len + 16), soFromCurrent);

				//イメージ
				int size = fsRead_int4(fs.get(), true);
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				if (ms->CopyFrom(fs.get(), size)<size) Abort();
				ms->Seek(0, soFromBeginning);

				if (contained_wd_i(_T("image/jpeg|jpg"), img_typ)) {
					if (bmpimg) {
						if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatJpeg)) Abort();
						ok = !bmpimg->Empty;
					}
					else if (!o_fnam.IsEmpty()) {
						o_fnam = ChangeFileExt(o_fnam, ".jpg");
						ms->SaveToFile(o_fnam);
						ok = true;
					}
				}
				else if (contained_wd_i(_T("image/png|png"), img_typ)) {
					if (bmpimg) {
						if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatPng)) Abort();
						ok = !bmpimg->Empty;
					}
					else if (!o_fnam.IsEmpty()) {
						o_fnam = ChangeFileExt(o_fnam, ".png");
						ms->SaveToFile(o_fnam);
						ok = true;
					}
				}
				break;
			}

			if (last_blk) break;

			fs->Seek(nxt_p, soFromBeginning);
		}
	}
	catch (...) {
		ok = false;
	}

	return ok;
}

//---------------------------------------------------------------------------
//幅 × 高 文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_wd_x_hi_str(int w, int h)
{
	return UnicodeString().sprintf(_T("%u × %u"), w, h);
}

//---------------------------------------------------------------------------
//画像サイズ情報文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_img_size_str(int w, int h, UnicodeString name)
{
	if (name.IsEmpty()) name = "画像サイズ";
	UnicodeString infstr = make_PropLine(name, get_wd_x_hi_str(w, h));
	if (w>0 && h>0) infstr.cat_sprintf(_T("  (%s)"), get_AspectStr(w, h).c_str());	//アスペクト比
	return infstr;
}

//---------------------------------------------------------------------------
//Jpeg 情報(形式、カラーなど)を取得
//---------------------------------------------------------------------------
void get_JpgExInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		std::unique_ptr<BYTE[]> buf(new BYTE[8]);
		for (int i=0; ; i++) {
			int mkr = fsRead_int2(fs.get(), true);
			int p	= fs->Position;
			if ((mkr & 0xff00)!=0xff00) Abort();
			if (i==0) {
				if (mkr==0xffd8) continue; else Abort();
			}
			if (mkr==0xffd9) Abort();
			int slen = fsRead_int2(fs.get(), true);
			//SOF0
			if (mkr==0xffc0 || mkr==0xffc2) {
				add_PropLine(_T("形式"), ((mkr==0xffc0)? "ベースライン" : "プログレッシブ"), lst);
				fs->Seek(5, soFromCurrent);

				BYTE n;	//構成要素数
				fs->ReadBuffer(&n, 1);
				//グレースケール
				if (n==1) {
					add_PropLine(_T("カラー"), "グレースケール", lst);
				}
				//YCbCr
				else if (n==3) {
					BYTE hn[3] = {0};	//水平サンプリング値
					BYTE vn[3] = {0};	//垂直サンプリング値
					for (int j=0; j<3; j++) {
						fs->ReadBuffer(buf.get(), 3);
						int id = buf[0];	if (id<1 || id>3) Abort();
						hn[id - 1] = (buf[1]>>4) & 0x0f;
						vn[id - 1] = buf[1] & 0x0f;
					}

					UnicodeString lbuf;
					lbuf += " (";
					for (int j=0; j<3; j++) lbuf.cat_sprintf(_T("%*ux%u"), ((j>0)? 2 : 1), hn[j], vn[j]);
					lbuf += ")";

					//※CbCrが11でない場合に対応(ここはちょっと怪しい)
					if (hn[0]%2==0 && hn[1]%2==0 && hn[2]%2==0) for (int j=0; j<3; j++) hn[j] /= 2;
					if (vn[0]%2==0 && vn[1]%2==0 && vn[2]%2==0) for (int j=0; j<3; j++) vn[j] /= 2;

					if (hn[1]==1 && hn[2]==1 && vn[1]==1 && vn[2]==1) {
						switch ((hn[0] << 4) | vn[0]) {
						case 0x11: lbuf.Insert("444", 1); break;
						case 0x12: lbuf.Insert("440", 1); break;
						case 0x14: lbuf.Insert("441", 1); break;
						case 0x21: lbuf.Insert("422", 1); break;
						case 0x22: lbuf.Insert("420", 1); break;
						case 0x24: lbuf.Insert("421", 1); break;
						case 0x41: lbuf.Insert("411", 1); break;
						case 0x42: lbuf.Insert("410", 1); break;
						default: Abort();
						}
					}

					add_PropLine(_T("YCbCr"), lbuf, lst);
				}
				//CMYK
				else if (n==4) {
					add_PropLine(_T("カラー"), "CMYK", lst);
				}
				else Abort();
				break;
			}
			fs->Seek(p + slen, soFromBeginning);
		}
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, LoadUsrMsg(USTR_IllegalFormat));
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
}

//---------------------------------------------------------------------------
//Exif情報の取得
//---------------------------------------------------------------------------
bool get_ExifInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//[o] 情報リスト
	int *ori,				//[o] 方向	(default = NULL)
	unsigned int *i_wd,		//[o] 幅	(default = NULL)
	unsigned int *i_hi)		//[o] 高さ	(default = NULL)
{
	int lst_cnt = lst->Count;

	std::unique_ptr<TStringList> i_lst(new TStringList());
	if (!EXIF_GetInf(fnam, i_lst.get())) return false;

	UnicodeString fext = get_extension(fnam);
	unsigned int iw, ih;
	if (Exif_GetImgSize(i_lst.get(), fext, &iw, &ih)) {
		lst->Add(get_img_size_str(iw, ih));
		if (i_wd && i_hi) { *i_wd = iw; *i_hi = ih; }
	}

	std::unique_ptr<TStringList> d_lst(new TStringList());
	d_lst->Text =
		"271=メーカー名\n"
		"272=モデル\n"
		"36867=撮影日時\n"
		"33437=絞り値\n"
		"33434U=露出時間\n"
		"37380=露光補正\n"
		"34850L=露出プログラム\n"
		"34855=ISO感度\n"
		"37383=測光方式\n"
		"37386=焦点距離\n"
		"NK:5=ＷＢ\n"
		"CN:4.7=ＷＢ\n"
		"37385=フラッシュ\n"
		"274L=画像方向\n"
		"NK:7=フォーカス\n"
		"42036=レンズ\n"
		"305=ソフト名\n"
		"FOVbVer=FOVb Ver.\n"
		"GPS:2=GPS緯度\n"
		"GPS:4=GPS経度\n";

	for (int i=0; i<d_lst->Count; i++)
		add_PropLine_if(d_lst->ValueFromIndex[i].c_str(), i_lst->Values[d_lst->Names[i]], lst);

	if (ori) *ori = get_ListIntVal(i_lst.get(), "274");

	return (lst->Count>lst_cnt);
}

//---------------------------------------------------------------------------
//Exif情報文字列(簡略)の取得
//---------------------------------------------------------------------------
UnicodeString get_ExifInfStr(
	UnicodeString fnam,		//ファイル名
	int *ori,				//[o] 方向	(default = NULL)
	unsigned int *i_wd,		//[o] 幅	(default = NULL)
	unsigned int *i_hi)		//[o] 高さ	(default = NULL)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	if (!EXIF_GetInf(fnam, i_lst.get())) return EmptyStr;

	UnicodeString fext = get_extension(fnam);
	if (i_wd && i_hi) Exif_GetImgSize(i_lst.get(), fext, i_wd, i_hi);

	struct {
		const _TCHAR *id;
		const _TCHAR *fmt;
	} inf_list[] = {
		{_T("33437"), _T(" F%s")}, {_T("33434U"), _T(" %s")}, {_T("34855"), _T(" ISO%s")},
		{_T("37380"), _T(" EV%s")}, {_T("37386"), _T(" %s")}
	};

	UnicodeString ret_str, vstr;
	int cnt = sizeof(inf_list)/sizeof(inf_list[0]);
	for (int i=0; i<cnt; i++) {
		vstr = i_lst->Values[inf_list[i].id];
		if (!vstr.IsEmpty()) ret_str.cat_sprintf(inf_list[i].fmt, vstr.c_str());
	}

	if (ori) *ori = get_ListIntVal(i_lst.get(), "274");
	return ret_str;
}

//---------------------------------------------------------------------------
//Exif情報文字列(サムネイル用)の取得
//---------------------------------------------------------------------------
UnicodeString get_ExifInfThumb(
	UnicodeString fnam,		//ファイル名
	int *ori)				//[o] 方向
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	if (!EXIF_GetInf(fnam, i_lst.get())) return EmptyStr;

	struct {
		const _TCHAR *id;
		const _TCHAR *fmt;
	} inf_list[] = {
		{_T("33437"), _T("F%s")}, {_T("33434"), _T(" %s")}, {_T("34855"), _T(" %s")}, {_T("37380"), _T(" %s")}
	};

	UnicodeString ret_str, vstr;
	int cnt = sizeof(inf_list)/sizeof(inf_list[0]);
	for (int i=0; i<cnt; i++) {
		vstr = i_lst->Values[inf_list[i].id];
		if (!vstr.IsEmpty()) ret_str.cat_sprintf(inf_list[i].fmt, vstr.c_str());
	}

	//方向
	if (ori) *ori = get_ListIntVal(i_lst.get(), "274");

	return ret_str;
}

//---------------------------------------------------------------------------
//GPS情報(緯度、経度)を取得
//---------------------------------------------------------------------------
bool get_GpsInf(
	UnicodeString fnam,		//ファイル名
	double *lat,			//[o] 緯度
	double *lng)			//[o] 経度
{
	std::unique_ptr<TStringList> xlst(new TStringList());
	if (!EXIF_GetInf(fnam, xlst.get())) return false;
	UnicodeString lat_str = Trim(get_tkn(xlst->Values["GPS:2"], '('));	//緯度
	UnicodeString lng_str = Trim(get_tkn(xlst->Values["GPS:4"], '('));	//経度
	if (lat_str.IsEmpty() || lng_str.IsEmpty()) return false;

	try {
		*lat = lat_str.ToDouble();
		*lng = lng_str.ToDouble();
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//メタファイルの情報を取得
//---------------------------------------------------------------------------
void get_MetafileInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	try {
		std::unique_ptr<TMetafile> mbuf(new TMetafile());
		mbuf->LoadFromFile(fnam);
		lst->Add(get_img_size_str(mbuf->Width, mbuf->Height, "サイズ"));
		add_PropLine_if(_T("作成者"), mbuf->CreatedBy,   lst);
		add_PropLine_if(_T("説明"),   mbuf->Description, lst);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
}

//---------------------------------------------------------------------------
//PNGか?
//---------------------------------------------------------------------------
bool test_Png(UnicodeString fnam)
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		BYTE buf;
		fs->ReadBuffer(&buf, 1);  if (buf!=0x89) Abort();
		if (!fsRead_check_char(fs.get(), "PNG\r\n\x1a\n")) Abort();
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//PNGファイルの情報を取得
//---------------------------------------------------------------------------
bool get_PngInf(
	UnicodeString fnam, 	//ファイル名
	TStringList *lst,		//[o] 情報リスト
	unsigned int *i_wd,		//[o] 幅	(default = NULL)
	unsigned int *i_hi)		//[o] 高さ	(default = NULL)
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		std::unique_ptr<BYTE[]> buf(new BYTE[16]);

		//識別部
		fs->ReadBuffer(buf.get(), 1);
		if (buf[0]!=0x89)									Abort();
		if (!fsRead_check_char(fs.get(), "PNG\r\n\x1a\n"))	Abort();
		//チャンク解析
		for (;;) {
			int size = fsRead_int4(fs.get(), true);
			UnicodeString name = get_id_str4(fs.get());
			if (SameStr(name, "IEND")) break;
			//イメージヘッダ
			if (SameStr(name, "IHDR")) {
				//サイズ
				int iw = fsRead_int4(fs.get(), true);
				int ih = fsRead_int4(fs.get(), true);
				lst->Add(get_img_size_str(iw, ih));
				if (i_wd && i_hi) { *i_wd = iw; *i_hi = ih; }
				//ビット深度
				fs->ReadBuffer(buf.get(), 1);
				add_PropLine(_T("ビット深度"), (int)buf[0], lst);
				//カラータイプ
				fs->ReadBuffer(buf.get(), 1);
				UnicodeString coltyp = get_PropTitle(_T("カラータイプ"));
				switch (buf[0]) {
				case 0: coltyp += "グレースケール";				break;
				case 2: coltyp += "トゥルーカラー";				break;
				case 3: coltyp += "インデックスカラー";			break;
				case 4: coltyp += "グレースケール + アルファ";	break;
				case 6: coltyp += "トゥルーカラー + アルファ";	break;
				}
				lst->Add(coltyp);
				fs->Seek(3, soFromCurrent);
			}
			//テキスト
			else if (SameStr(name, "tEXt")) {
				std::unique_ptr<BYTE[]> sbuf(new BYTE[size + 2]);
				fs->ReadBuffer(sbuf.get(), size);
				int p = -1;
				for (int i=0; i<size && p==-1; i++) if (sbuf[i]==0) p = i + 1;
				if (p>0 && p<size) {
					sbuf[size] = 0;
					std::unique_ptr<TStringList> vlst(new TStringList());
					vlst->Text = (char*)&sbuf[p];
					int i = 0;
					while (i<vlst->Count) if (Trim(vlst->Strings[i]).IsEmpty()) vlst->Delete(i); else i++;
					UnicodeString s = Trim(vlst->Text);
					if (SameText((char*)&sbuf[0], "Creation Time")) {
						s = TRegEx::Replace(get_tkn(s, '\n'), "[^\\d\\w\\s/:,+\\-]+", EmptyStr);
					}
					add_PropLine((char*)&sbuf[0], s, lst);
				}
			}
			//イメージガンマ
			else if (SameStr(name, "gAMA")) {
				lst->Add(get_PropTitle(_T("イメージガンマ")).cat_sprintf(
										_T("%-7.6g"), fsRead_int4(fs.get(), true)/100000.0));
			}
			//ICCプロファイル
			else if (SameStr(name, "iCCP")) {
				std::unique_ptr<BYTE[]> sbuf(new BYTE[size]);
				fs->ReadBuffer(sbuf.get(), size);
				add_PropLine(_T("ICCプロファイル"), (char*)&sbuf[0], lst);
			}
			else {
				fs->Seek(size, soFromCurrent);
			}

			//CRC をスキップ
			fs->Seek(4, soFromCurrent);
		}

		ret = true;
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, LoadUsrMsg(USTR_IllegalFormat));
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
	return ret;
}
//---------------------------------------------------------------------------
//PNG情報文字列(簡略)の取得
//---------------------------------------------------------------------------
UnicodeString get_PngInfStr(
	UnicodeString fnam,		//ファイル名
	unsigned int *i_wd,		//[o] 幅	(default = NULL)
	unsigned int *i_hi)		//[o] 高さ	(default = NULL)
{
	UnicodeString ret_str;

	std::unique_ptr<TStringList> i_lst(new TStringList());
	unsigned int iw, ih;
	if (!get_PngInf(fnam, i_lst.get(), &iw, &ih)) return EmptyStr;

	if (i_wd && i_hi) {
		*i_wd = iw;
		*i_hi = ih;
	}
	else {
		ret_str = get_wd_x_hi_str(iw, ih) + " ";
	}

	UnicodeString col_s, bit_s;
	for (int i=0; i<i_lst->Count; i++) {
		UnicodeString lbuf = i_lst->Strings[i];
		if		(ContainsStr(lbuf, "カラータイプ: ")) col_s = Trim(get_tkn_r(lbuf, ':'));
		else if (ContainsStr(lbuf, "ビット深度: "))	  bit_s = "  深度:" + Trim(get_tkn_r(lbuf, ':'));
	}

	ret_str += (col_s + bit_s);
	return ret_str;
}

//---------------------------------------------------------------------------
//Gifファイルの情報を取得
//---------------------------------------------------------------------------
bool get_GifInf(
	UnicodeString fnam,		//ファイル名
	TStringList  *lst,		//[o] 情報リスト	(NULLでも可)
	unsigned int *i_wd,		//[o] 幅			(default = NULL)
	unsigned int *i_hi,		//[o] 高さ			(default = NULL)
	int			 *i_cnt)	//[o] フレーム数	(default = NULL)
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		if (fs->Size<14) Abort();

		std::unique_ptr<BYTE[]> buf(new BYTE[16]);
		BYTE bt;
		//バージョン
		fs->ReadBuffer(buf.get(), 6);
		buf[6] = '\0';
		UnicodeString vstr = (char*)buf.get();
		if (!StartsStr("GIF", vstr)) Abort();
		//サイズ
		fs->ReadBuffer(buf.get(), 4);
		unsigned int iw = (buf[1] << 8) + buf[0];
		unsigned int ih = (buf[3] << 8) + buf[2];
		if (i_wd && i_hi) { *i_wd = iw; *i_hi = ih; }
		//パレット情報
		fs->ReadBuffer(&bt, 1);
		bool has_ct = (bt & 0x80);
		int ct_size = 2 << (bt & 0x07);
		if (lst) {
			add_PropLine(_T("バージョン"), vstr, lst);
			lst->Add(get_img_size_str(iw, ih));	//"画像サイズ"
			lst->Add(get_PropTitle(_T("カラーテーブル")).cat_sprintf(_T("%s"), has_ct? _T("あり") : _T("なし")));
			add_PropLine(_T("ビットの深さ"), ((bt >> 4) & 0x07) + 1, lst);
			add_PropLine(_T("色数"), ct_size, lst);
		}
		fs->Seek(2, soFromCurrent);
		if (has_ct) fs->Seek(ct_size * 3, soFromCurrent);

		//ブロック
		int img_cnt = 0;
		int t_cnt	= 0;
		for (;;) {
			BYTE blk_sz;
			fs->ReadBuffer(&bt, 1);
			//拡張ブロック
			if (bt==0x21) {
				fs->ReadBuffer(&bt, 1);
				if (bt==0xf9) {	//Graphic Control Extension
					fs->ReadBuffer(&blk_sz, 1);
					if (blk_sz!=4) Abort();
					fs->ReadBuffer(buf.get(), 4);
					t_cnt += ((buf[2] << 8) + buf[1]);
				}
				for (;;) {
					fs->ReadBuffer(&blk_sz, 1);
					if (blk_sz==0) break;
					fs->Seek(blk_sz, soFromCurrent);
				}
			}
			//データブロック
			else if (bt==0x2c) {
				img_cnt++;
				fs->Seek(8, soFromCurrent);
				fs->ReadBuffer(&bt, 1);
				if (bt & 0x80) fs->Seek((2 << (bt & 0x07)) * 3, soFromCurrent);
				fs->Seek(1, soFromCurrent);
				for (;;) {
					fs->ReadBuffer(&blk_sz, 1);
					if (blk_sz==0) break;
					fs->Seek(blk_sz, soFromCurrent);
				}
			}
			else break;
		}

		if (lst) {
			if (img_cnt>1) add_PropLine(_T("フレーム数"), img_cnt, lst);
			if (t_cnt>0)   lst->Add(get_PropTitle(_T("長さ")).cat_sprintf(_T("%.2f 秒"), t_cnt/100.0));
		}

		if (i_cnt) *i_cnt = img_cnt;
		ret = true;
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, LoadUsrMsg(USTR_IllegalFormat));
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}

	return ret;
}

//---------------------------------------------------------------------------
//WebPファイルの情報を取得
//  参考: https://developers.google.com/speed/webp/docs/riff_container
//---------------------------------------------------------------------------
bool get_WebpInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//[o] 情報リスト
	unsigned int *i_wd,		//[o] 幅
	unsigned int *i_hi)		//[o] 高さ
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!SameStr(get_chunk_hdr(fs.get()), "RIFF") || !fsRead_check_char(fs.get(), "WEBP")) Abort();

		std::unique_ptr<BYTE[]> buf(new BYTE[16]);
		unsigned int iw = 0, ih = 0, i_cnt = 0, t_cnt = 0;
		bool has_alph = false;
		int  lst_top  = lst->Count;
		unsigned int   ui_buf;
		unsigned short us_buf;
		UnicodeString tmp, fmt, meta;

		while (fs->Position < fs->Size) {
			unsigned int  chunk_sz;
			UnicodeString chunk = get_chunk_hdr(fs.get(), &chunk_sz);
			if (chunk.IsEmpty()) Abort();
			int p = fs->Seek(0, soFromCurrent);

			if (SameText(chunk, "VP8 ")) {
				fmt = "ロッシー";
				fs->Seek(6, soFromCurrent);
				if (iw==0) {
					fs->ReadBuffer(&ui_buf, 4);
					iw = (ui_buf & 0x3fff);
					ih = ((ui_buf >> 16) & 0x3fff);
				}
			}
			else if (SameText(chunk, "VP8L")) {
				fmt = "ロスレス";
				fs->ReadBuffer(buf.get(), 1);
				if (buf[0]!=0x2f) Abort();
				if (iw==0) {
					fs->ReadBuffer(&ui_buf, 4);
					iw = (ui_buf & 0x3fff) + 1;
					ih = ((ui_buf >> 14) & 0x3fff) + 1;
				}
			}
			else if (SameText(chunk, "VP8X")) {
				fs->Seek(4, soFromCurrent);
				if (iw==0) {
					fs->ReadBuffer(buf.get(), 6);
					iw = (buf[0] | (buf[1] << 8) | (buf[2] << 16)) + 1;
					ih = (buf[3] | (buf[4] << 8) | (buf[5] << 16)) + 1;
				}
			}
			else if (SameText(chunk, "ICCP")) {
				lst->Add(get_PropTitle(_T("ICCP")) + "有り");
			}
			else if (SameText(chunk, "ALPH")) {
				has_alph = true;
				/* ※仕様と合わない?
				fs->ReadBuffer(buf.get(), 4);
				tmp = ((buf[0]>>2) & 0x01)? "Level Reduction" : "None";
				add_PropLine(_T("α前処理"), tmp, lst);

				switch ((buf[0]>>4) & 0x03) {
				case  1: tmp = "Horizontal"; break;
				case  2: tmp = "Vertical"; break;
				case  3: tmp = "Gradient"; break;
				default: tmp = "None";
				}
				add_PropLine(_T("αフィルタ"), tmp, lst);

				tmp = ((buf[0]>>6) & 0x01)? "Lossless" : "None";
				add_PropLine(_T("α圧縮"), tmp, lst);
				*/
			}
			else if (SameText(chunk, "ANIM")) {
				fmt = "アニメーション";
				fs->ReadBuffer(buf.get(), 4);
				lst->Add(get_PropTitle(_T("背景色")).cat_sprintf(_T("R%u G%u B%u A%u"), buf[2], buf[1], buf[0], buf[3]));
				fs->ReadBuffer(&us_buf, 2);
				tmp = get_PropTitle(_T("ループ回数"));
				if (us_buf==0)
					lst->Add(tmp + "無限");
				else
					lst->Add(tmp.cat_sprintf(_T("%u"), us_buf));
			}
			else if (SameText(chunk, "ANMF")) {
				fs->Seek(12, soFromCurrent);
				fs->ReadBuffer(buf.get(), 3);
				t_cnt += (buf[0] | (buf[1] << 8) | (buf[2] << 16));
				i_cnt++;
			}
			else if (SameText(chunk, "EXIF")) {
				if (!meta.IsEmpty()) meta += ",";
				meta += "EXIF";
			}
			else if (SameText(chunk, "XMP ")) {
				if (!meta.IsEmpty()) meta += ",";
				meta += "XMP";
	 		}

			fs->Seek(p, soFromBeginning);
			fs->Seek((int)(chunk_sz + (chunk_sz%2)), soFromCurrent);
		}

		//サイズ
		if (iw>0 && ih>0) {
			tmp = get_img_size_str(iw, ih);
			if (lst_top==lst->Count) lst->Add(tmp); else lst->Insert(lst_top, tmp);
			lst_top++;
			if (i_wd && i_hi) { *i_wd = iw; *i_hi = ih; }
		}
		//形式
		if (!fmt.IsEmpty()) {
			tmp = make_PropLine(_T("形式"), fmt);
			if (has_alph) tmp += " + アルファ";
			if (lst_top==lst->Count) lst->Add(tmp); else lst->Insert(lst_top, tmp);
		}
		//フレーム数
		if (i_cnt>0) {
			add_PropLine(_T("フレーム数"), i_cnt, lst);
			lst->Add(get_PropTitle(_T("長さ")).cat_sprintf(_T("%.2f 秒"), t_cnt/1000.0));
		}
		//メタデータ
		add_PropLine_if(_T("メタデータ"), meta, lst);
		ret = true;
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, LoadUsrMsg(USTR_IllegalFormat));
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
	return ret;
}
//---------------------------------------------------------------------------
//WebP情報文字列(簡略)の取得
//---------------------------------------------------------------------------
UnicodeString get_WebpInfStr(
	UnicodeString fnam,		//ファイル名
	unsigned int *i_wd,		//[o] 幅
	unsigned int *i_hi)		//[o] 高さ
{
	UnicodeString ret_str;

	std::unique_ptr<TStringList> i_lst(new TStringList());
	unsigned int iw, ih;
	if (!get_WebpInf(fnam, i_lst.get(), &iw, &ih)) return EmptyStr;
	if (i_wd && i_hi) {
		*i_wd = iw;
		*i_hi = ih;
	}
	else {
		ret_str = get_wd_x_hi_str(iw, ih) + " ";
	}

	UnicodeString col_s, bit_s;
	for (int i=0; i<i_lst->Count; i++) {
		UnicodeString lbuf = i_lst->Strings[i];
		if (ContainsStr(lbuf, "形式: ")) {
			ret_str += Trim(get_tkn_r(lbuf, ':'));
			break;
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//PSPファイルの情報を取得
//---------------------------------------------------------------------------
bool get_PspInf(
	UnicodeString fnam,		//ファイル名
	TStringList  *lst,		//[o] 情報リスト	(NULLでも可)
	unsigned int *i_wd,		//[o] 幅			(default = NULL)
	unsigned int *i_hi)		//[o] 高さ			(default = NULL)
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!StartsStr(fsRead_char(fs.get(), 32), "Paint Shop Pro Image File\x0a\x1a")) Abort();
		int vno = fsRead_int4(fs.get());
		if (lst) lst->Add(get_PropTitle(_T("ファイルVer.")).cat_sprintf(_T("%u.0"), vno));

		for (;;) {
			if (!SameStr(fsRead_char(fs.get(), 4), "~BK")) break;
			int tag    = fsRead_int2(fs.get());
			int len1   = fsRead_int4(fs.get());
			int len2   = fsRead_int4(fs.get());
			int next_p = fs->Seek(0, soFromCurrent) - 4;
			next_p += (tag==0x10 || tag==0x12)? len2 : len1;
			if (vno<=3) next_p += 4;

			//Image Block
			if (tag==0) {
				//サイズ
				unsigned int iw = fsRead_int4(fs.get());
				unsigned int ih = fsRead_int4(fs.get());
				if (i_wd && i_hi) { *i_wd = iw; *i_hi = ih; }

				if (lst) {
					UnicodeString lbuf = get_PropTitle("画像サイズ").cat_sprintf(_T("%u × %u"), iw, ih);
					if (iw>0 && ih>0) lbuf.cat_sprintf(_T("  (%s)"), get_AspectStr(iw, ih).c_str());
					lst->Add(lbuf);
					//解像度
					double r = fsRead_double(fs.get());
					BYTE b	 = fsRead_byte(fs.get());
					switch (b) {
					case 1: lst->Add(get_PropTitle(_T("pixel/inch")).cat_sprintf(_T("%g"), r)); break;
					case 2: lst->Add(get_PropTitle(_T("pixel/cm")).cat_sprintf(_T("%g"), r));	 break;
					}
					//その他
					int cmp = fsRead_int2(fs.get());
					lst->Add(make_PropLine(_T("圧縮"), (cmp==1)? "RLE" : (cmp==2)? "LZ77" : (cmp==3)? "JPEG" : "None"));
					lst->Add(get_PropTitle(_T("Bit/Sample")).cat_sprintf(_T("%u"), fsRead_int2(fs.get())));
					fs->Seek(2, soFromCurrent);	//Planes
					lst->Add(get_PropTitle(_T("色数")).cat_sprintf(_T("%u"), fsRead_int4(fs.get())));
				}
				ret = true;
				break;
			}

			fs->Seek(next_p, soFromBeginning);
		}
	}
	catch (...) {
		if (lst) lst->Clear();
	}

	return ret;
}

//---------------------------------------------------------------------------
//PSPファイルのサムネイル画像を取得
//---------------------------------------------------------------------------
bool get_PspThumbnail(
	UnicodeString fnam,			//ファイル名
	Graphics::TBitmap *bmpimg)	//出力ビットマップ (default = NULL)
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!StartsStr(fsRead_char(fs.get(), 32), "Paint Shop Pro Image File\x0a\x1a")) Abort();

		bool ok = false;
		int vno = fsRead_int4(fs.get());
		for (;;) {
			if (!SameStr(fsRead_char(fs.get(), 4), "~BK")) break;
			int tag    = fsRead_int2(fs.get());
			int len1   = fsRead_int4(fs.get());
			int len2   = fsRead_int4(fs.get());
			int next_p = fs->Seek(0, soFromCurrent) - 4;
			next_p += (tag==0x10 || tag==0x12)? len2 : len1;
			if (vno<=3) next_p += 4;

			//PreviewImage
			if (tag==0x12) {
				fs->Seek(next_p, soFromBeginning);
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				if (ms->CopyFrom(fs.get(), len1)==len1) {
					if (!WIC_get_from_memory(ms.get(), bmpimg, GUID_ContainerFormatJpeg)) Abort();
					ok = !bmpimg->Empty;
				}
				break;
			}

			fs->Seek(next_p, soFromBeginning);
		}
		return ok;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//アイコンか? (サイズ指定可能)
//---------------------------------------------------------------------------
bool test_Icon(UnicodeString fnam,
	int size)	//サイズ	(default = 0 : 任意)
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		unsigned short us_buf;
		fs->ReadBuffer(&us_buf, 2);  if (us_buf!=0) Abort();
		fs->ReadBuffer(&us_buf, 2);  if (us_buf!=1) Abort();

		unsigned short n;
		fs->ReadBuffer(&n, 2);		 if (n==0) Abort();

		bool ret = false;
		if (size==0) {
			ret = true;
		}
		else {
			for (int i=0; i<n; i++) {
				unsigned char b_buf;
				fs->ReadBuffer(&b_buf, 1);
				int wd = (b_buf==0)? 256 : b_buf;
				fs->ReadBuffer(&b_buf, 1);
				int hi = (b_buf==0)? 256 : b_buf;
				if (wd==size && hi==size) {
					ret = true; break;
				}
				fs->Seek(14, soFromCurrent);
			}
		}
		return ret;
	}
	catch(...) {
		return false;
	}
}
//---------------------------------------------------------------------------
//アイコンファイルの情報を取得
//---------------------------------------------------------------------------
void get_IconInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//[o] 情報リスト		(NULLでも可)
	int *ico_n)				//[o] アイコン/画像数	(default = NULL)
{
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		unsigned short us_buf;
		fs->ReadBuffer(&us_buf, 2);
		if (us_buf!=0) UserAbort(USTR_IllegalFormat);

		fs->ReadBuffer(&us_buf, 2);
		if (us_buf!=1 && us_buf!=2) UserAbort(USTR_IllegalFormat);
		bool is_cur = (us_buf==2);

		unsigned short n;
		fs->ReadBuffer(&n, 2);
		if (ico_n) *ico_n = n;

		if (lst) {
			add_PropLine(is_cur? _T("画像数") : _T("アイコン数"), n, lst);

			for (int i=0; i<n; i++) {
				//サイズ
				unsigned char b_buf;
				fs->ReadBuffer(&b_buf, 1);
				int wd = (b_buf==0)? 256 : b_buf;
				fs->ReadBuffer(&b_buf, 1);
				int hi = (b_buf==0)? 256 : b_buf;
				UnicodeString lbuf;
				lbuf.sprintf(_T("%*u: %s"), FPRP_NAM_WD, i + 1, get_wd_x_hi_str(wd, hi).c_str());
				//カーソル
				if (is_cur) {
					fs->Seek(2, soFromCurrent);
					fs->ReadBuffer(&us_buf, 2);
					int hp_x = us_buf;
					fs->ReadBuffer(&us_buf, 2);
					int hp_y = us_buf;
					lbuf.cat_sprintf(_T("  Hotspot=(x:%u y:%u)"), hp_x, hp_y);
					fs->Seek(8, soFromCurrent);
				}
				//アイコン
				else {
					//色
					fs->ReadBuffer(&b_buf, 1);
					if (b_buf==0)
				 		lbuf += "  256色以上";
					else
				 		lbuf.cat_sprintf(_T("  %u色"), b_buf);
					fs->Seek(13, soFromCurrent);
				}
				lst->Add(lbuf);
			}
		}
	}
	catch(EAbort &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
}

//---------------------------------------------------------------------------
//アニメーションカーソルファイルの情報を取得
//---------------------------------------------------------------------------
bool get_AniInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	bool ret = false;

	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		if (!SameStr(get_id_str4(fs.get()), "RIFF")) UserAbort(USTR_IllegalFormat);
		fs->Seek(4, soFromCurrent);		//データ長
		if (!SameStr(get_id_str4(fs.get()), "ACON")) UserAbort(USTR_IllegalFormat);

		for (;;) {
			UnicodeString id_str = get_id_str4(fs.get());
			if (id_str.IsEmpty()) break;

			int d_size = fsRead_int4(fs.get());
			int next_p = fs->Seek(0, soFromCurrent) + d_size + (d_size%2);

			if (SameStr(id_str, "LIST")) {
				id_str = get_id_str4(fs.get());
				if (SameStr(id_str, "INFO")) {
					id_str = get_id_str4(fs.get());
					if (SameStr(id_str, "INAM")) {
						d_size = fsRead_int4(fs.get());
						UnicodeString lbuf = fsRead_char(fs.get(), d_size);
						if (!lbuf.IsEmpty()) add_PropLine(_T("タイトル"), lbuf, lst);
						fs->Seek(d_size%2, soFromCurrent);	//パディング
						id_str = get_id_str4(fs.get());
						if (SameStr(id_str, "IART")) {
							d_size = fsRead_int4(fs.get());
							UnicodeString lbuf = fsRead_char(fs.get(), d_size);
							if (!lbuf.IsEmpty()) add_PropLine(_T("作成者"), lbuf, lst);
						}
					}
				}
			}
			else if (SameStr(id_str, "anih")) {
				struct tagANIHeader {
					DWORD cbSizeOf;
					DWORD cFrames;
					DWORD cSteps;
					DWORD cx;
					DWORD cy;
					DWORD cBitCount;
					DWORD cPlanes;
					DWORD JifRate;
					DWORD flags;
				} ah;

				fs->ReadBuffer(&ah, sizeof(ah));
				add_PropLine(_T("フレーム数"), ah.cFrames, lst);
				add_PropLine(_T("ステップ数"), ah.cSteps,  lst);
			}
			else if (contained_wd_i(_T("rate|seq "), id_str)) {
				;
			}
			else break;

			fs->Seek(next_p, soFromBeginning);
		}
		ret = true;
	}
	catch (EAbort &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch(EReadError &e) {
		add_list_errmsg(lst, e.Message);
	}
	catch (Exception &e) {
		add_list_errmsg(lst, e.Message);
	}
	return ret;
}

//---------------------------------------------------------------------------
//アプリケーション情報を取得
//---------------------------------------------------------------------------
void get_AppInf(
	UnicodeString fnam,	//ファイル名
	TStringList *lst,	//[o] 情報リスト
	bool check)			//ヘッダをチェック、マシン情報取得 (default = true)
{
	//対応マシン
	if (check) {
		UnicodeString warn_msg;
		try {
			if (!file_exists(fnam)) Abort();
			std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
			if (fs->Size<=0x40) Abort();

			IMAGE_DOS_HEADER DOS_hdr;
			fs->ReadBuffer(&DOS_hdr, sizeof(DOS_hdr));
			if (DOS_hdr.e_magic!=IMAGE_DOS_SIGNATURE) {
				if (test_AppInfExt(get_extension(fnam))) warn_msg = LoadUsrMsg(USTR_IllegalFormat);
				Abort();
			}
			if (DOS_hdr.e_lfarlc<0x40 || fs->Size<(DOS_hdr.e_lfanew + sizeof(IMAGE_FILE_HEADER))) {
				warn_msg = "PE、NE、LE以外のフォーマットです。";
				Abort();
			}

			IMAGE_NT_HEADERS NT_hdr;
			fs->Seek((int)DOS_hdr.e_lfanew, soFromBeginning);
			fs->ReadBuffer(&NT_hdr, sizeof(NT_hdr));

			if (NT_hdr.Signature==IMAGE_NT_SIGNATURE) {
				IMAGE_FILE_HEADER *fh = &NT_hdr.FileHeader;
				UnicodeString vstr =
					(fh->Machine==IMAGE_FILE_MACHINE_I386)?  "x86" :
					(fh->Machine==IMAGE_FILE_MACHINE_AMD64)? "x64" :
					(fh->Machine==IMAGE_FILE_MACHINE_IA64)?  "Intel Itanium" :
					(fh->Machine==0x01c0)? "arm" :
					(fh->Machine==0x01c2)? "thumb" :
					(fh->Machine==0x01c4)? "armnt" :
					(fh->Machine==0xaa64)? "arm64" : "???";

				if (SameStr(vstr, "x86") && (fh->Characteristics & 0x20)) vstr += " (Large Address Aware)";
				add_PropLine(_T("マシン"), vstr, lst);
			}
			else {
				switch (NT_hdr.Signature & 0x0000ffff) {
				case IMAGE_OS2_SIGNATURE:		//NE
					warn_msg = "NEフォーマット(Win16)です。";
					Abort();
				case IMAGE_OS2_SIGNATURE_LE:	//LE
					warn_msg = "LEフォーマット(VXD)です。";
					Abort();
				default:
					warn_msg = "PE、NE、LE以外のフォーマットです。";
					Abort();
				}
			}
		}
		catch (...) {
			if (!warn_msg.IsEmpty()) add_WarnLine(warn_msg, lst);
			return;
		}
	}

	//アプリケーション情報
	WORD cpag_id[] = {0, 932, 949, 950, 1200, 1250, 1251, 1252, 1253, 1254, 1255, 1256};
	WORD lang_id[] = {
			0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408,
			0x0409, 0x040A, 0x040B, 0x040C, 0x040D, 0x040E, 0x040F, 0x0410,
			0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418,
			0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420, 0x0421,
			0x0804, 0x0807, 0x0809, 0x080A, 0x080C, 0x0810, 0x0813, 0x0814, 0x0816, 0x081A, 0x0C0C, 0x100C
		};

	DWORD dwReserved;
	DWORD dwSize	= ::GetFileVersionInfoSize(fnam.c_str(), &dwReserved);
	if (dwSize==0) return;
	LPVOID lpBuffer = ::HeapAlloc(::GetProcessHeap(), HEAP_ZERO_MEMORY, dwSize);
	if (!lpBuffer) return;

	if (::GetFileVersionInfo(fnam.c_str(), NULL, dwSize, lpBuffer)) {
		LPVOID lpLang = NULL;
		UINT dwLength;
		if (::VerQueryValue(lpBuffer, _T("\\VarFileInfo\\Translation"), &lpLang, &dwLength)) {
			WORD *lpTranslate = (WORD*)lpLang;
			UnicodeString substr;
			if (*lpTranslate!=0) substr.sprintf(_T("\\StringFileInfo\\%04x%04x\\"), *lpTranslate, *(lpTranslate + 1));

			std::unique_ptr<TStringList> s_lst(new TStringList());
			s_lst->Text =
				"FileDescription=説明\n"
				"FileVersion=ファイルVer.\n"
				"ProductName=製品名\n"
				"ProductVersion=製品Ver.\n"
				"LegalCopyright=著作権\n"
				"CompanyName=会社名\n"
				"LegalTrademarks=商標\n"
				"Comments=コメント\n"
				"OriginalFilename=元のファイル名\n"
				"InternalName=内部名\n";

			int l_cnt = 0;
			for (int j=0; j<s_lst->Count; j++) {
				UnicodeString nstr = s_lst->Names[j];
				UnicodeString subblock, vstr;
				LPVOID lpStr;
				if (!substr.IsEmpty()) {
					subblock = substr + nstr;
					if (::VerQueryValue(lpBuffer, subblock.c_str(), &lpStr, &dwLength))
						vstr.cat_sprintf(_T("%s"), lpStr);
				}
				else {
					//言語情報がない
					int c_cnt = sizeof(cpag_id)/sizeof(WORD);
					int i_cnt = sizeof(lang_id)/sizeof(WORD);
					for (int c_i=0; c_i<c_cnt && vstr.IsEmpty(); c_i++) {
						for (int l_i=0; l_i<i_cnt && vstr.IsEmpty(); l_i++) {
							substr.sprintf(_T("\\StringFileInfo\\%04x%04x\\"), lang_id[l_i], cpag_id[c_i]);
							subblock = substr + nstr;
							if (::VerQueryValue(lpBuffer, subblock.c_str(), &lpStr, &dwLength))
								vstr.cat_sprintf(_T("%s"), lpStr);
						}
					}
				}
				vstr = Trim(vstr);
				if (!vstr.IsEmpty()) {
					add_PropLine(s_lst->ValueFromIndex[j], vstr, lst);
					l_cnt++;
				}
			}

			//取得できなければプロパティから
			if (l_cnt==0) usr_SH->get_PropInf(fnam, lst);
		}
	}
	::HeapFree(::GetProcessHeap(), 0, lpBuffer);

	//アイコン数
	int ixn = (int)::ExtractIcon(HInstance, fnam.c_str(), -1);
	if (ixn>1) add_PropLine(_T("アイコン数"), ixn, lst);
}

//---------------------------------------------------------------------------
//DLLのエクスポート関数一覧を取得
//---------------------------------------------------------------------------
bool get_DllExpFunc(
	AnsiString fnam, 	//ファイル名
	TStringList *lst,	//[o] 関数リスト
	int sort_mode,		//0:序数順(default)/ 1:インデックス順/ 2: RVA順/ 3:名前順
	int list_mode)		//0:表示用(default)/ 1:CSV/ 2:TSV
{
	bool ret = false;

	LOADED_IMAGE LoadedImage;
	if (MapAndLoad(fnam.c_str(), NULL, &LoadedImage, TRUE, TRUE)) {
		PVOID img_base = LoadedImage.MappedAddress;
		ULONG nSize;
		IMAGE_EXPORT_DIRECTORY *pImgExpDir =
			(_IMAGE_EXPORT_DIRECTORY*)ImageDirectoryEntryToData(
				img_base, FALSE, IMAGE_DIRECTORY_ENTRY_EXPORT, &nSize);
		if (pImgExpDir) {
			DWORD *dFuncRVAs = (DWORD *)ImageRvaToVa(
					LoadedImage.FileHeader, img_base, pImgExpDir->AddressOfFunctions, NULL);
			DWORD *dNameRVAs = (DWORD *)ImageRvaToVa(
					LoadedImage.FileHeader, img_base, pImgExpDir->AddressOfNames, NULL);
			WORD  *dOrdRVAs  = (WORD *)ImageRvaToVa(
					LoadedImage.FileHeader, img_base, pImgExpDir->AddressOfNameOrdinals, NULL);

			UnicodeString tmp;
			std::unique_ptr<TStringList> flst(new TStringList());
			for (int i = 0; i < pImgExpDir->NumberOfNames; i++) {
				//Hint [TAB] RVA [TAB] Name
				UnicodeString name = (char *)ImageRvaToVa(LoadedImage.FileHeader, img_base, dNameRVAs[i], NULL);
				tmp.sprintf(_T("%04X\t%08X\t%s"), i, dFuncRVAs[dOrdRVAs[i]], name.c_str());
				flst->AddObject(tmp, (TObject*)(NativeInt)(dOrdRVAs[i] + pImgExpDir->Base));	//Obj=序数
			}

			//ソート
			USR_CsvTopIsHdr = false;
			USR_CsvSortMode = 1;
			switch (sort_mode) {
			case 1:		//インデックス順
				break;
			case 2:		//RVA順
				USR_CsvCol = 1;
				flst->CustomSort(comp_TsvTextOrder);
				break;
			case 3:		//名前順
				USR_CsvCol = 2;
				flst->CustomSort(comp_TsvNaturalOrder);
				break;
			default:	//序数順
				flst->CustomSort(comp_ObjectsOrder);
			}

			//CSV or TSV
			if (list_mode==1 || list_mode==2) {
				UnicodeString fmt = (list_mode==1)? "%d,%s,%s,\"%s\"" : "%d\t%s\t%s\t\"%s\"";
				for (int i=0; i<flst->Count; i++) {
					TStringDynArray l_buf = split_strings_tab(flst->Strings[i]);
					if (l_buf.Length!=3) continue;
					int ordn = (int)flst->Objects[i];
					flst->Strings[i] = tmp.sprintf(fmt.c_str(),
										ordn, l_buf[0].c_str(), l_buf[1].c_str(), l_buf[2].c_str());
				}
				lst->Add((list_mode==1)? "Ordinal,Hint,RVA,Name" : "Ordinal\tHint\tRVA\tName");
				lst->AddStrings(flst.get());
			}
			//通常の一覧
			else {
				int max_w = 4;
				for (int i=0; i<flst->Count; i++) {
					TStringDynArray l_buf = split_strings_tab(flst->Strings[i]);
					if (l_buf.Length!=3) continue;
					int ordn = (int)flst->Objects[i];
					flst->Strings[i] = tmp.sprintf(_T("%4d (%04X) %s %s %s"), ordn, ordn, 
											l_buf[0].c_str(), l_buf[1].c_str(), l_buf[2].c_str());
					max_w = std::max(l_buf[2].Length(), max_w);
				}
				lst->Add(fnam);
				lst->Add(tmp.sprintf(_T("       Ordinal Base: %u"), pImgExpDir->Base));
				lst->Add(tmp.sprintf(_T("Number of Functions: %u"), pImgExpDir->NumberOfFunctions));
				lst->Add(tmp.sprintf(_T("    Number of Names: %u"), pImgExpDir->NumberOfNames));
				lst->Add(EmptyStr);
				lst->Add("Ordinal     Hint RVA      Name");
				UnicodeString hr_str = make_RuledLine(4, 11,4,8,max_w);
				lst->Add(hr_str);
				lst->AddStrings(flst.get());
				lst->Add(hr_str);
			}
			ret = true;
		}
		UnMapAndLoad(&LoadedImage);
	}
	return ret;
}

//---------------------------------------------------------------------------
//Borland プロジェクト情報を取得
//---------------------------------------------------------------------------
void get_BorlandInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	if (!file_exists(fnam)) return;

	UnicodeString fext = get_extension(fnam);

	//プロジェクト
	if (test_FileExt(fext, ".cbproj.dproj")) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->LoadFromFile(fnam);
		bool flag = false;
		lst->Add(EmptyStr);
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString lbuf = Trim(fbuf->Strings[i]);
			if (!flag) {
				if (ContainsText(lbuf, "<PropertyGroup>")) flag = true;
			}
			else {
				if (ContainsText(lbuf, "</PropertyGroup>")) break;

				UnicodeString vstr = get_tkn_m(lbuf, ">", "</");
				if (ContainsText(lbuf, "<FrameworkType>"))
					add_PropLine(_T("FrameworkType"),	vstr, lst);
				else if (ContainsText(lbuf, "<AppType>"))
					add_PropLine(_T("AppType"),			vstr, lst);
				else if (ContainsText(lbuf, "<MainSource>"))
					add_PropLine(_T("MainSource"),		vstr, lst);
				else if (ContainsText(lbuf, "<Platform Condition="))
					add_PropLine(_T("Platform"),		vstr, lst);
				else if (ContainsText(lbuf, "<Config Condition=") || ContainsText(lbuf, "Configuration Condition="))
					add_PropLine(_T("Config"),			vstr, lst);
				else if (ContainsText(lbuf, "<ProjectType>"))
					add_PropLine(_T("ProjectType"),		vstr, lst);
			}
		}
		lst->Add(EmptyStr);
	}
	//ソース
	else if (test_FileExt(fext, ".cpp.pas.dfm.fmx.h")) {
		UnicodeString fm_nam = ChangeFileExt(fnam, ".dfm");
		if (!file_exists(fm_nam)) fm_nam = ChangeFileExt(fnam, ".fmx");
		if (file_exists(fm_nam)) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			fbuf->LoadFromFile(fm_nam);

			std::unique_ptr<TStringList> ibuf(new TStringList());
			for (int i=0; i<fbuf->Count; i++) {
				UnicodeString lbuf = Trim(fbuf->Strings[i]);
				if (StartsText("object", lbuf)) {
					if (i>0) break;
					add_PropLine(_T("Form Name"), get_tkn_m(lbuf, ' ', ':'), ibuf.get());
				}
				else {
					UnicodeString vnam = split_tkn(lbuf, " = ");
					UnicodeString vstr = exclude_quot(lbuf);
					if (SameText(vnam, "Caption")) {
						UnicodeString cap_str;
						while (!vstr.IsEmpty()) {
							if (remove_top_s(vstr, '\'')) {
								cap_str += split_tkn(vstr, '\'');
							}
							else if (vstr[1]=='#') {
								UnicodeString tmp = split_tkn(vstr, '\'');
								if (!vstr.IsEmpty()) vstr.Insert("'", 1);
								tmp.Delete(1, 1);
								TStringDynArray c_lst = SplitString(tmp, "#");
								UnicodeString tstr;
								for (int j=0; j<c_lst.Length; j++) tstr += _TCHAR(c_lst[j].ToIntDef(0));
								cap_str += tstr;
							}
							else {
								cap_str += vstr;  break;
							}
						}
						vstr = cap_str;
					}
					else if (!contained_wd_i(_T("BorderStyle|ClientWidth|ClientHeight|Position"), vnam)) continue;

					add_PropLine(vnam, vstr, ibuf.get());
				}
			}

			if (ibuf->Count>0) {
				lst->Add(EmptyStr);
				lst->AddStrings(ibuf.get());
				lst->Add(EmptyStr);
			}
		}
	}
}

//---------------------------------------------------------------------------
//tags ファイル情報を取得
//---------------------------------------------------------------------------
void get_TagsInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//[o] 情報リスト
{
	if (!file_exists(fnam)) return;

	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->LoadFromFile(fnam);
	bool flag = false;
	for (int i=0; i<fbuf->Count; i++) {
		UnicodeString lbuf = fbuf->Strings[i];
		if (remove_top_s(lbuf, "!_TAG_")) {
			if (!flag) lst->Add(EmptyStr);
			flag = true;
			if (remove_top_s(lbuf, "PROGRAM_") || remove_top_s(lbuf, "FILE_")) {
				UnicodeString vnam = split_pre_tab(lbuf);
				if (!SameText(vnam, "ENCODING")) {
					UnicodeString vstr = ReplaceStr(ReplaceStr(lbuf, "\t//", ""), "\t"," ");
					add_PropLine(vnam, vstr, lst);
				}
			}
		}
		else {
			if (flag) lst->Add(EmptyStr);
			break;
		}
	}
}

//---------------------------------------------------------------------------
//PDFのバージョンを取得
//---------------------------------------------------------------------------
void get_PdfVer(
	UnicodeString fnam,		//ファイル名
	TStringList  *lst)		//[o] 情報リスト
{
	try {
		if (!file_exists(fnam)) Abort();

		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		UnicodeString lbuf = fsRead_char(fs.get(), 8);
		if (StartsStr("%PDF-", lbuf)) add_PropLine(_T("PDFバージョン"), lbuf.SubString(6, 3).c_str(), lst);
	}
	catch (...) {
		;
	}
}

//---------------------------------------------------------------------------
//HTML文書のヘッダ文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_html_header(UnicodeString fnam, int code_page)
{
	UnicodeString headstr;

	try {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		if (code_page>0) {
			std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
			fbuf->LoadFromFile(fnam, enc.get());
		}
		else {
			fbuf->LoadFromFile(fnam);
		}

		int p = pos_i("</HEAD>", fbuf->Text);
		if (p>0) headstr = fbuf->Text.SubString(1, p + 6);
	}
	catch (...) {
		headstr = EmptyStr;
	}

	return headstr;
}

//---------------------------------------------------------------------------
//HTML文書のヘッダ情報(charset, TITLE, META)を取得
//---------------------------------------------------------------------------
void get_HtmlInf(
	UnicodeString fnam,		//HTMLファイル名
	TStringList *lst)		//[o] 情報リスト(名前=値 の形式で格納)
{
	if (!file_exists(fnam)) return;

	//ヘッダ部分を取得
	UnicodeString headstr = get_html_header(fnam);
	if (!headstr.IsEmpty()) {
		UnicodeString itmbuf;

		//文書型宣言
		UnicodeString dtd_str = get_tkn(get_tkn_r(headstr, "<!DOCTYPE "), '>');
		int p = pos_i("//DTD ",  dtd_str);
		if (p>0) dtd_str = get_tkn_m(dtd_str, "//DTD ", "//");
		if (!dtd_str.IsEmpty()) {
			if (SameText(dtd_str, "html")) dtd_str = "HTML5";
			lst->Add(itmbuf.sprintf(_T("%*s: %s"), FPRP_NAM_WD, _T("DOCTYPE"), dtd_str.c_str()));
		}

		//charset をチェック
		UnicodeString charset;
		p = pos_i("charset", headstr);
		if (p>0) {
			UnicodeString tmp = headstr.SubString(p, headstr.Length() - p + 1);
			tmp = get_tkn(get_tkn_m(tmp, '=', '>'), '/');
			charset = Trim(ReplaceStr(tmp, "\"", ""));
			if (!charset.IsEmpty())
				lst->Add(itmbuf.sprintf(_T("%*s: %s"), FPRP_NAM_WD, _T("charset"), charset.c_str()));
		}

		//エンコーディングを指定して再読み込み
		if (!charset.IsEmpty()) {
			int code_page = get_CodePageOfName(charset);
			if (code_page>0) headstr = get_html_header(fnam, code_page);
		}

		//コメント部分を削除
		int p0, p1;
		p0 = headstr.Pos("<!--");
		while (p0>0) {
			p1 = headstr.Pos("-->");
			if (p1>p0) {
				headstr.Delete(p0, p1 - p0 + 3);
				p0 = headstr.Pos("<!--");
			}
			else {
				headstr = headstr.SubString(1, p1 - 1);  break;
			}
		}

		//タイトル
		p0 = pos_i("<TITLE>",  headstr);
		p1 = pos_i("</TITLE>", headstr);
		if (p0>0 && p1>p0) {
			UnicodeString tit = headstr.SubString(p0 + 7, p1 - p0 - 7);
			if (!tit.IsEmpty()) lst->Add(itmbuf.sprintf(_T("%*s: %s"), FPRP_NAM_WD, _T("title"), tit.c_str()));
		}

		//メタ情報
		std::unique_ptr<HtmConv> htmcnv(new HtmConv());
		UnicodeString tmp = headstr;
		while (!tmp.IsEmpty()) {
			int p = tmp.Pos('<');  if (p==0) break;
			tmp.Delete(1, p - 1);
			UnicodeString tstr = tmp;
			p = tstr.Pos('>');  if (p==0) break;
			tstr = tstr.SubString(1, p);
			UnicodeString nstr = htmcnv->GetTagAtr(tstr, "META", _T("NAME"));
			if (!nstr.IsEmpty()) {
				UnicodeString cstr = htmcnv->GetTagAtr(tstr, "META", _T("CONTENT"));
				if (!cstr.IsEmpty()) lst->Add(itmbuf.sprintf(_T("%*s: %s"), FPRP_NAM_WD, nstr.c_str(), cstr.c_str()));
			}
			tmp.Delete(1, tstr.Length());
		}
	}
}

//---------------------------------------------------------------------------
//ファイルのCRC32文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_CRC32_str(
	UnicodeString fnam,		//ファイル名
	bool only_1blk,			//先頭の１ブロック(32KB)のみ	(default = false)
	bool prc_msg)			//メッセージを処理				(default = false)
{
	static uint32_t table[256];
	static bool has_tbl = false;

	//テーブルの初期化
	if (!has_tbl) {
		for (int i=0; i<256; i++) {
			uint32_t rem = i;
			for (int j=0; j<8; j++) {
				if (rem & 1) {
					rem >>= 1;
					rem ^= 0xedb88320;
				}
				else {
					rem >>= 1;
				}
			}
			table[i] = rem;
		}
		has_tbl = true;
	}

	SetLastError(NO_ERROR);
	HANDLE hFile = ::CreateFile(fnam.c_str(), GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE==hFile) return EmptyStr;

	UnicodeString ret_str;
	try {
		try {
			std::unique_ptr<BYTE[]> fbuf(new BYTE[FILE_RBUF_SIZE]);
			uint32_t crc32 = 0xFFFFFFFF;
			DWORD dwSize;
			for (int i=0;;i++) {
				if (!::ReadFile(hFile, fbuf.get(), FILE_RBUF_SIZE, &dwSize, NULL)) Abort();
				if (dwSize==0) break;
				char *bp = (char*)fbuf.get();
				for (DWORD i=0; i<dwSize; i++,bp++)
					crc32 = (crc32 >> 8) ^ table[(crc32 & 0xff) ^ (uint8_t)(*bp)];
				if (only_1blk) break;
				if (prc_msg && i%64==0 && is_KeyPress_ESC()) {	//***
					ClearKeyBuff(true);
					SetLastError(E_ABORT);
					Abort();
				}
			}
			crc32 ^= 0xFFFFFFFF;
			ret_str.sprintf(_T("%08x"), crc32);
		}
		catch (...) {
			ret_str = EmptyStr;
		}
	}
	__finally {
		::CloseHandle(hFile);
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//ファイルのハッシュ文字列を取得
//---------------------------------------------------------------------------
UnicodeString get_HashStr(
	UnicodeString fnam,		//ファイル名
	ALG_ID algid,			//アルゴリズム CALG_MD5/ CALG_SHA1/ CALG_SHA_256/384/512
	bool only_1blk,			//先頭の１ブロック(32KB)のみ	(default = false)
	bool prc_msg)			//メッセージを処理				(default = false)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	bool canceled = false;

	HANDLE hFile = ::CreateFile(fnam.c_str(), GENERIC_READ,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (INVALID_HANDLE_VALUE==hFile) return EmptyStr;

	if (!::CryptAcquireContext(&hProv, NULL, NULL,
		(algid==CALG_SHA_256 || algid==CALG_SHA_384 || algid==CALG_SHA_512)? PROV_RSA_AES : PROV_RSA_FULL,
			CRYPT_VERIFYCONTEXT))
				return EmptyStr;
	if (!::CryptCreateHash(hProv, algid, 0, 0, &hHash)) {
		::CryptReleaseContext(hProv, 0);
		return EmptyStr;
	}

	UnicodeString ret_str;
	try {
		try {
			std::unique_ptr<BYTE[]> fbuf(new BYTE[FILE_RBUF_SIZE]);
			DWORD dwSize;
			for (int i=0;;i++) {
				if (!::ReadFile(hFile, fbuf.get(), FILE_RBUF_SIZE, &dwSize, NULL)) Abort();
				if (dwSize==0) break;
				if (!::CryptHashData(hHash, fbuf.get(), dwSize, 0)) Abort();
				if (only_1blk) break;
				if (prc_msg && i%64==0 && is_KeyPress_ESC()) {	//***
					ClearKeyBuff(true);
					canceled = true;
					Abort();
				}
			}
			DWORD dwHashLen;
			if (!::CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0)) Abort();
			std::unique_ptr<BYTE[]> pbHash(new BYTE[dwHashLen]);
			if (!::CryptGetHashParam(hHash, HP_HASHVAL, pbHash.get(), &dwHashLen, 0)) Abort();
			for (DWORD i=0; i<dwHashLen; i++) ret_str.cat_sprintf(_T("%02x"), pbHash[i]);
		}
		catch (...) {
			ret_str = EmptyStr;
		}
	}
	__finally {
		::CryptDestroyHash(hHash);
		::CryptReleaseContext(hProv, 0);
		::CloseHandle(hFile);
	}

	SetLastError(NO_ERROR);
	if (canceled) SetLastError(E_ABORT);

	return ret_str;
}
//---------------------------------------------------------------------------
UnicodeString get_HashStr(
	UnicodeString fnam,
	UnicodeString idstr,	//アルゴリズム識別名
	bool only_1blk,			//先頭の１ブロックのみ	(default = false)
	bool prc_msg)			//メッセージを処理		(default = false)
{
	switch (idx_of_word_i(HASH_ID_STR, idstr)) {
	case  0: return get_HashStr(fnam, CALG_MD5,		only_1blk, prc_msg);
	case  1: return get_HashStr(fnam, CALG_SHA1,	only_1blk, prc_msg);
	case  2: return get_HashStr(fnam, CALG_SHA_256,	only_1blk, prc_msg);
	case  3: return get_HashStr(fnam, CALG_SHA_384,	only_1blk, prc_msg);
	case  4: return get_HashStr(fnam, CALG_SHA_512,	only_1blk, prc_msg);
	case  5: return get_CRC32_str(fnam, 			only_1blk, prc_msg);
	default: return EmptyStr;
	}
}
//---------------------------------------------------------------------------
//文字列のハッシュ文字列を取得
//  マルチバイト文字はUTF-8として処理
//---------------------------------------------------------------------------
UnicodeString get_TextHashStr(
	UnicodeString s,		//文字列
	ALG_ID algid)			//アルゴリズム CALG_MD5/ CALG_SHA1/ CALG_SHA_256/384/512
{
	if (s.IsEmpty()) return EmptyStr;

	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	if (!::CryptAcquireContext(&hProv, NULL, NULL,
		(algid==CALG_SHA_256 || algid==CALG_SHA_384 || algid==CALG_SHA_512)? PROV_RSA_AES : PROV_RSA_FULL,
			CRYPT_VERIFYCONTEXT))
				return EmptyStr;
	if (!::CryptCreateHash(hProv, algid, 0, 0, &hHash)) {
		::CryptReleaseContext(hProv, 0);
		return EmptyStr;
	}

	UnicodeString ret_str;
	try {
		try {	
			int s_len = s.Length();
			int b_len = ::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s_len, 0, NULL, NULL, NULL);
			if (b_len==0) Abort();
			std::unique_ptr<char[]> b_str(new char[b_len]);
			if (::WideCharToMultiByte(CP_UTF8, 0, s.c_str(), s_len, b_str.get(), b_len, NULL, NULL)==0) Abort();
			if (!::CryptHashData(hHash, (BYTE*)b_str.get(), b_len, 0)) Abort();
			DWORD dwHashLen;
			if (!::CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0)) Abort();
			std::unique_ptr<BYTE[]> pbHash(new BYTE[dwHashLen]);
			if (!::CryptGetHashParam(hHash, HP_HASHVAL, pbHash.get(), &dwHashLen, 0)) Abort();
			for (DWORD i=0; i<dwHashLen; i++) ret_str.cat_sprintf(_T("%02x"), pbHash[i]);
		}
		catch (...) {
			ret_str = EmptyStr;
		}
	}
	__finally {
		::CryptDestroyHash(hHash);
		::CryptReleaseContext(hProv, 0);
	}

	SetLastError(NO_ERROR);
	return ret_str;
}
//---------------------------------------------------------------------------
UnicodeString get_TextHashStr(
	UnicodeString s,
	UnicodeString idstr)	//アルゴリズム識別名
{
	switch (idx_of_word_i(HASH_ID_STR, idstr)) {
	case  0: return get_TextHashStr(s, CALG_MD5);
	case  1: return get_TextHashStr(s, CALG_SHA1);
	case  2: return get_TextHashStr(s, CALG_SHA_256);
	case  3: return get_TextHashStr(s, CALG_SHA_384);
	case  4: return get_TextHashStr(s, CALG_SHA_512);
	default: return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//オーディオ/ビデオファイルの長さ(ms)を取得
//---------------------------------------------------------------------------
int get_duration(UnicodeString fnam)
{
	int t = -1;
	UnicodeString fext = get_extension(fnam);

	if (test_FileExt(fext, ".cda")) {
		int len;
		if (get_CdaInf(fnam, NULL, &len)) t = len;
	}
	else if (test_FlacExt(fext)) {
		int len;
		if (get_FlacInf(fnam, NULL, &len)) t = len;
	}
	else if (test_MciSndExt(fext)) {
		UnicodeString cmd;
		cmd.sprintf(_T("open \"%s\" alias GETLENSND"), fnam.c_str());
		if (::mciSendString(cmd.c_str(), NULL, 0, NULL)==0) {
			::mciSendString(_T("set GETLENSND time format milliseconds"), NULL, 0, NULL);
			_TCHAR rbuf[256];
			if (::mciSendString(_T("status GETLENSND length"), rbuf, 255, NULL)==0)
				t = UnicodeString(rbuf).ToIntDef(0);
			::mciSendString(_T("close GETLENSND"), NULL, 0, NULL);
		}
	}

	//プロパティから取得
	if (t==-1) t = usr_SH->get_Duration(fnam) * 1000;

	return t;
}

//---------------------------------------------------------------------------
//ファイル名に関する警告を取得
//  戻り値: 警告数
//---------------------------------------------------------------------------
int get_filename_warn(
	UnicodeString fnam,		//(パス付き)ファイル名
	TStringList *lst,		//[o] 情報リスト		(default = NULL)
	bool is_dir)			//true = ディレクトリ	(default = false);
{
	std::unique_ptr<TStringList> wlst(new TStringList());
	UnicodeString tmp;

	//パス名
	if (fnam.Length()>=MAX_PATH)		add_WarnLine(tmp.sprintf(_T("パス名が長すぎます(%u)。"), fnam.Length()), wlst.get());

	//名前
	UnicodeString nnam = ExtractFileName(fnam);
	if (nnam.Length()>=256) 			add_WarnLine(tmp.sprintf(_T("名前が長すぎます(%u)。"), nnam.Length()), wlst.get());
	if (ContainsStr(nnam, L"\u202e"))	add_WarnLine("名前に制御文字「RLO」が含まれています。", wlst.get());

	//主部
	UnicodeString bnam = is_dir? nnam : get_base_name(fnam);
	if (StartsStr(' ', bnam))			add_WarnLine("名前の先頭に空白があります。", 		wlst.get());
	if (StartsStr("　", bnam))			add_WarnLine("名前の先頭に全角空白があります。", 	wlst.get());
	if (bnam.Length()>1) {
		if (EndsStr(' ', bnam))			add_WarnLine("名前の末尾に空白があります。", 		wlst.get());
		if (EndsStr("　", bnam))		add_WarnLine("名前の末尾に全角空白があります。", 	wlst.get());
		if (ContainsStr(bnam, "  "))	add_WarnLine("名前に連続した空白が含まれています。",wlst.get());
	}

	UnicodeString s = check_EnvDepandChars(bnam);
	if (!s.IsEmpty()) add_WarnLine(tmp.sprintf(_T("名前に環境依存文字が含まれています(%s)。"), s.c_str()), wlst.get());

	s = check_Surrogates(bnam);
	if (!s.IsEmpty()) add_WarnLine(tmp.sprintf(_T("名前にサロゲートペアが含まれています(%s)。"), s.c_str()), wlst.get());

	//拡張子
	if (!is_dir) {
		UnicodeString xnam = get_tkn_r(get_extension(fnam), '.');
		if (StartsStr(' ', xnam))		add_WarnLine("拡張子の先頭に空白があります。",		wlst.get());
		if (StartsStr("　", xnam))		add_WarnLine("拡張子の先頭に全角空白があります。",	wlst.get());

		if (xnam.Length()>1) {
			if (EndsStr(' ', xnam))		add_WarnLine("拡張子の末尾に空白があります。", 		wlst.get());
			if (EndsStr("　", xnam))	add_WarnLine("拡張子の末尾に全角空白があります。",	wlst.get());
			if (ContainsStr(xnam, "  "))	add_WarnLine("拡張子に連続した空白が含まれています。", wlst.get());
		}
	}

	if (lst) lst->AddStrings(wlst.get());

	return wlst->Count;
}

//---------------------------------------------------------------------------
//代替データストリームの数を取得
//---------------------------------------------------------------------------
int get_ADS_count(UnicodeString fnam)
{
	int cnt = 0;

	WIN32_FIND_STREAM_DATA sd;
	HANDLE hFS = ::FindFirstStreamW(cv_ex_filename(fnam).c_str(), FindStreamInfoStandard, &sd, 0);
	if (hFS!=INVALID_HANDLE_VALUE) {
		do {
			if (!SameStr(sd.cStreamName, "::$DATA")) cnt++;
		} while (::FindNextStreamW(hFS, &sd));
		::FindClose(hFS);
	}

	return cnt;
}

//---------------------------------------------------------------------------
//代替データストリーム情報を取得
//---------------------------------------------------------------------------
void get_ADS_Inf(UnicodeString fnam, TStringList *lst)
{
	WIN32_FIND_STREAM_DATA sd;
	HANDLE hFS = ::FindFirstStreamW(fnam.c_str(), FindStreamInfoStandard, &sd, 0);
	if (hFS!=INVALID_HANDLE_VALUE) {
		__int64 total = 0;
		int cnt = 0;
		do {
			UnicodeString snam = get_tkn(sd.cStreamName, ":$DATA");
			remove_top_s(snam, ':');
			if (!snam.IsEmpty()) {
				__int64 sz = ((__int64)sd.StreamSize.HighPart<<32) + sd.StreamSize.LowPart;
				total += sz;
				UnicodeString lbuf = get_PropTitle(UnicodeString().sprintf(_T("ストリーム%u"), ++cnt));
				lst->Add(lbuf.cat_sprintf(_T("%-18s  (%s)"), snam.c_str(), FormatFloat(",0", (double)sz).c_str()));
				//Zone.Identifier の内容
				if (SameText(snam, "Zone.Identifier")) {
					try {
						std::unique_ptr<TFileStream>   fs(new TFileStream(fnam + ":" + snam, fmOpenRead | fmShareDenyNone));
						std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
						ms->CopyFrom(fs.get(), fs->Size);
						int code_page = get_MemoryCodePage(ms.get());
						if (code_page>0) {
							std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
							ms->Seek(0, soFromBeginning);
							std::unique_ptr<TStringList> fbuf(new TStringList());
							fbuf->LoadFromStream(ms.get(), enc.get());
							for (int i=0; i<fbuf->Count; i++) add_PropLine(null_TCHAR, fbuf->Strings[i], lst);
						}
					}
					catch (...) {
						;
					}
				}
				else if (SameText(":" + snam, THUMB_TXT_ADS)) {
					UnicodeString lbuf = get_top_line(fnam + THUMB_TXT_ADS, 65001);
					if (!lbuf.IsEmpty()) add_PropLine(null_TCHAR, lbuf, lst);
				}
				else if (SameText(":" + snam, NYANFIDEF_ADS)) {
					std::unique_ptr<TStringList> fbuf(new TStringList());
					UnicodeString anam = fnam + NYANFIDEF_ADS;
					if (file_exists(anam)) {
						try {
							std::unique_ptr<TStringList> fbuf(new TStringList());
							fbuf->LoadFromFile(anam);
							for (int i=0; i<fbuf->Count; i++) add_PropLine(null_TCHAR, fbuf->Strings[i], lst);
						}
						catch (...) {
							;
						}
					}
				}
			}
		} while (::FindNextStreamW(hFS, &sd));
		::FindClose(hFS);

		if (cnt>1) {
			lst->Add(get_PropTitle(_T("合計サイズ")).cat_sprintf(_T("%s (%s)"),
						get_size_str_G(total, 10, 2, 1).c_str(), get_size_str_B(total, 0).c_str()));
		}
	}
}

//---------------------------------------------------------------------------
//ファイルを使用中のプロセス一覧を取得
//  戻り値: 使用中のプロセス数
//---------------------------------------------------------------------------
int get_ProcessingInf(
	UnicodeString fnam,		//対象ファイル名
	TStringList *lst,		//[o] 情報リスト		(default = NULL)
	bool only_fnam)			//実行ファイル名のみ	(default = false)
{
	int ret_cnt = 0;

	DWORD dwSession;
	WCHAR szSessionKey[CCH_RM_SESSION_KEY+1] = { 0 };
	PCWSTR pszFile = fnam.c_str();
	if (RmStartSession(&dwSession, 0, szSessionKey)==ERROR_SUCCESS) {
		if (RmRegisterResources(dwSession, 1, &pszFile, 0, NULL, 0, NULL)==ERROR_SUCCESS) {
			DWORD dwReason		  = 0;
			UINT  nProcInfoNeeded = 0;
			UINT  nProcInfo 	  = 0;
			if (RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, NULL, &dwReason)==ERROR_MORE_DATA) {
				ret_cnt = nProcInfo = nProcInfoNeeded;
				if (ret_cnt>0 && lst) {
					std::unique_ptr<RM_PROCESS_INFO[]> rm_pi(new RM_PROCESS_INFO[nProcInfoNeeded]);
					if (RmGetList(dwSession, &nProcInfoNeeded, &nProcInfo, rm_pi.get(), &dwReason)==ERROR_SUCCESS) {
						UnicodeString tmp;
						for (unsigned int i=0; i<nProcInfo; i++) {
							if (i>0 && !only_fnam) lst->Add(EmptyStr);

							if (!only_fnam)
								add_PropLine(tmp.sprintf(_T("%2u：プロセス名"), i + 1), Trim(rm_pi[i].strAppName).c_str(), lst);

							int pid = rm_pi[i].Process.dwProcessId;
							HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
							if (hProcess) {
								_TCHAR sbuf[MAX_PATH];
								DWORD sz = MAX_PATH;
								UnicodeString fnam = ::QueryFullProcessImageName(hProcess, 0, sbuf, &sz)? sbuf : _T("");
								::CloseHandle(hProcess);
								if (!fnam.IsEmpty()) {
									if (only_fnam)
										lst->Add(fnam);
									else
										add_PropLine(_T("実行ファイル"), fnam, lst, LBFLG_PATH_FIF);
								}
							}

							switch (rm_pi[i].ApplicationType) {
							case RmUnknownApp: 	tmp = "UnknownApp";	break;
							case RmMainWindow:	tmp = "MainWindow";	break;
							case RmOtherWindow:	tmp = "OtherWindow";break;
							case RmService:		tmp = "Service"; 	break;
							case RmExplorer:	tmp = "Explorer";	break;
							case RmConsole:		tmp = "Console";	break;
							case RmCritical:	tmp = "Critical";	break;
							}

							if (!only_fnam)
								add_PropLine(_T("プロセスID"), tmp.sprintf(_T("%u (Type=%s)"), pid, tmp.c_str()), lst);
						}
					}
				}
			}
		}
		RmEndSession(dwSession);
	}

	return ret_cnt;
}
//---------------------------------------------------------------------------
//拡張子に関連付けられた実行ファイルを取得
//---------------------------------------------------------------------------
UnicodeString get_AssocExeName(UnicodeString fext)  
{  
	UnicodeString xnam;
	_TCHAR pszFile[MAX_PATH*2];
	DWORD dwOut = MAX_PATH*2;
	if (::AssocQueryString(
		ASSOCF_INIT_DEFAULTTOSTAR|ASSOCF_INIT_DEFAULTTOFOLDER,
		ASSOCSTR_EXECUTABLE,
		fext.c_str(), _T("open"), pszFile, &dwOut)==S_OK)
	{
		xnam = pszFile;
	}
	return xnam;    
}
//---------------------------------------------------------------------------
