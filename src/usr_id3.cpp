//----------------------------------------------------------------------//
// MP3 ID3タグ情報の処理												//
//																		//
//----------------------------------------------------------------------//
#include "usr_str.h"
#include "usr_wic.h"
#include "usr_file_ex.h"
#include "usr_shell.h"
#include "usr_id3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
unsigned int get_SynchsafeInt(BYTE *p)
{
	return (unsigned int)(((p[0]&0x7f) << 21) + ((p[1]&0x7f) << 14) + ((p[2]&0x7f) << 7) + (p[3] & 0x7f));
}
//---------------------------------------------------------------------------
unsigned int get_32bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 24) + (p[1] << 16) + (p[2] << 8) + p[3]);
}
//---------------------------------------------------------------------------
unsigned int get_24bitInt(BYTE *p)
{
	return (unsigned int)((p[0] << 16) + (p[1] << 8) + p[2]);
}

//---------------------------------------------------------------------------
UnicodeString get_str_from_buf(char *c, int size)
{
	std::unique_ptr<char[]> ts(new char[size + 1]);
	char *p = ts.get();
	::CopyMemory(p, c, size);
	p += size;
	*p = '\0';
	return UnicodeString(ts.get());
}

//---------------------------------------------------------------------------
//バージョンを取得し、先頭フレームヘッダへ
//  戻り値: 1=v1?  3=v2.3  4=v2.4
//---------------------------------------------------------------------------
bool ID3_getVer_toFrame(TFileStream *fs, unsigned int *last, unsigned int *majv)
{
	try {
		id3_hdr idhdr;
		fs->ReadBuffer(&idhdr, 10);
		if (idhdr.hdr_id[0]!='I' || idhdr.hdr_id[1]!='D' || idhdr.hdr_id[2]!='3') Abort();
		unsigned int size = get_SynchsafeInt(&idhdr.size[0]);
		*last = size + 10;
		*majv = idhdr.maj_ver;
		if (idhdr.maj_ver!=2 && idhdr.maj_ver!=3 && idhdr.maj_ver!=4) Abort();
		if (idhdr.maj_ver>=3 && (idhdr.flag & 0x40)) {
			//拡張ヘッダがあれば読み飛ばす
			BYTE exsz[4];
			id3_ex_hdr exhdr;
			fs->ReadBuffer(&exsz[0], 4);
			int ex_size = (idhdr.maj_ver==4)? get_SynchsafeInt(&exsz[0]) : get_32bitInt(&exsz[0]);
			if (idhdr.maj_ver==4) ex_size-=4,
			fs->Seek(ex_size, soFromCurrent);
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//MP3ファイルの ID3 Tag v2/v1 情報を取得
//---------------------------------------------------------------------------
bool ID3_GetInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst,		//結果リスト(項目=内容 の形式で格納)
	bool id_min)			//最小限の情報を T??=の形式で取得
{
	bool ret = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		unsigned int maj_ver = 0, last_p = 0;
		if (ID3_getVer_toFrame(fs.get(), &last_p, &maj_ver)) {
			//ID3v2
			lst->Add(get_PropTitle(_T("ID3バージョン")).cat_sprintf(_T("2.%u"), maj_ver));

			//フレームヘッダ
			for (;;) {
				id3_frm_hdr   frmhdr;
				id3_frm_hdr_2 frmhdr2;
				UnicodeString id_str;
				if (maj_ver==2) {
					fs->ReadBuffer(&frmhdr2, 6);
					id_str = get_str_from_buf(frmhdr2.frame_id, 3);
				}
				else {
					fs->ReadBuffer(&frmhdr, 10);
					id_str = get_str_from_buf(frmhdr.frame_id, 4);
				}
				if (id_str.IsEmpty()) break;

				int size;
				switch (maj_ver) {
				case 2: size = get_24bitInt(&frmhdr2.size[0]);		break;
				case 3: size = get_32bitInt(&frmhdr.size[0]);		break;
				case 4: size = get_SynchsafeInt(&frmhdr.size[0]);	break;
				}

				if (id_str[1]=='T')  {
					//テキストエンコーディング
					BYTE code = fsRead_byte(fs.get());
					size--;
					//表示名に変換
					UnicodeString lbuf = id_str;
					if (id_min) {
						if		(contained_wd_i(_T("TPE1|TP1"), id_str)) lbuf = "TP1=";
						else if (contained_wd_i(_T("TALB|TAL"), id_str)) lbuf = "TAL=";
						else if (contained_wd_i(_T("TIT2|TT2"), id_str)) lbuf = "TT2=";
						else if (contained_wd_i(_T("TRCK|TRK"), id_str)) lbuf = "TRK=";
						else if (contained_wd_i(_T("TYER|TYE"), id_str)) lbuf = "TYE=";
						else										     lbuf = EmptyStr;
					}
					else {
						if		(contained_wd_i(_T("TALB|TAL"), id_str)) lbuf = "アルバム名";
						else if (contained_wd_i(_T("TCOM|TCM"), id_str)) lbuf = "作曲者";
						else if (contained_wd_i(_T("TCON|TCO"), id_str)) lbuf = "ジャンル";
						else if (contained_wd_i(_T("TCOP|TCR"), id_str)) lbuf = "著作権情報";
						else if (contained_wd_i(_T("TDAT|TDA"), id_str)) lbuf = "日付";
						else if (contained_wd_i(_T("TENC|TEN"), id_str)) lbuf = "エンコード";
						else if (contained_wd_i(_T("TIT1|TT1"), id_str)) lbuf = "グループ";
						else if (contained_wd_i(_T("TIT2|TT2"), id_str)) lbuf = "タイトル";
						else if (contained_wd_i(_T("TIT3|TT3"), id_str)) lbuf = "サブタイトル";
						else if (contained_wd_i(_T("TLEN|TLE"), id_str)) lbuf = "曲の長さ";
						else if (contained_wd_i(_T("TORY|TOR|TDOR"), id_str))
														 	 	   		 lbuf = "原曲リリース年";
						else if (contained_wd_i(_T("TPE1|TP1"), id_str)) lbuf = "アーティスト";
						else if (contained_wd_i(_T("TPE2|TP2"), id_str)) lbuf = "バンド/伴奏";
						else if (contained_wd_i(_T("TPE3|TP3"), id_str)) lbuf = "指揮者";
						else if (contained_wd_i(_T("TPOS|TPA"), id_str)) lbuf = "ディスク＃";
						else if (contained_wd_i(_T("TPUB|TPB"), id_str)) lbuf = "出版社";
						else if (contained_wd_i(_T("TRCK|TRK"), id_str)) lbuf = "トラック＃";
						else if (contained_wd_i(_T("TRDA|TRD|TDRC"), id_str))
															 			 lbuf = "録音日時";
						else if (contained_wd_i(_T("TYER|TYE"), id_str)) lbuf = "リリース年";
						else											 lbuf = id_str;
						lbuf = get_PropTitle(lbuf);
					}

					//文字列
					std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
					if (ms->CopyFrom(fs.get(), size)<size) Abort();
					ms->Seek(0, soFromBeginning);
					std::unique_ptr<TStringList> ts(new TStringList());
					switch (code) {
					case 3:	 ts->LoadFromStream(ms.get(), TEncoding::UTF8); break;
					default: ts->LoadFromStream(ms.get());
					}
					if (ts->Count>0 && !lbuf.IsEmpty()) lst->Add(lbuf + Trim(ts->Strings[0]));
				}
				else {
					fs->Seek(size, soFromCurrent);
				}
				if (fs->Position>=last_p) break;
			}

			ret = true;
		}
		else {
			//ID3v1
			char fbuf[128];
			fs->Seek(-128, soFromEnd);
			fs->ReadBuffer(fbuf, 128);
			std::unique_ptr<char[]> sbuf(new char[32]);
			char *sp = sbuf.get();
			UnicodeString tmp;
			char *s = &fbuf[0]; memcpy(sp, s, 3); sp[ 3] = '\0'; tmp = sp;
			if (SameText(tmp, "TAG")) {
				add_PropLine(_T("ID3バージョン"), "1", lst);
				s = &fbuf[ 3]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TT2=") : get_PropTitle(_T("トラック名"))) + tmp);
				s = &fbuf[33]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TP1=") : get_PropTitle(_T("アーティスト名"))) + tmp);
				s = &fbuf[63]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TAL=") : get_PropTitle(_T("アルバム名"))) + tmp);
				s = &fbuf[93]; memcpy(sp, s,  4); sp[ 4] = '\0'; tmp = sp;
				lst->Add((id_min? UnicodeString("TYE=") : get_PropTitle(_T("リリース年"))) + tmp);
				s = &fbuf[97]; memcpy(sp, s, 30); sp[30] = '\0'; tmp = sp;
				if (!id_min) add_PropLine(_T("コメント"), tmp, lst);
				ret = true;
			}
		}
	}
	catch (...) {
		lst->Clear();
	}
	return ret;
}

//---------------------------------------------------------------------------
//MP3ファイルから埋め込み画像をビットマップで取得 or ファイルに出力
//---------------------------------------------------------------------------
bool ID3_GetImage(
	UnicodeString fnam,			//ファイル名
	Graphics::TBitmap *bmpimg,	//出力ビットマップ (default = NULL)
	UnicodeString o_fnam)		//出力ファイル名	 (default = EmptyStr)
{
	bool ok = false;
	try {
		if (!file_exists(fnam)) Abort();
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));

		//ID3v2タグヘッダ
		unsigned int maj_ver = 0, last_p = 0;
		if (!ID3_getVer_toFrame(fs.get(), &last_p, &maj_ver)) Abort();

		//フレームヘッダ
		for (;;) {
			id3_frm_hdr   frmhdr;
			id3_frm_hdr_2 frmhdr2;
			UnicodeString id_str;
			if (maj_ver==2) {
				fs->ReadBuffer(&frmhdr2, 6);
				id_str = get_str_from_buf(frmhdr2.frame_id, 3);
			}
			else {
				fs->ReadBuffer(&frmhdr, 10);
				id_str = get_str_from_buf(frmhdr.frame_id, 4);
			}
			if (id_str.IsEmpty()) break;

			int size;
			switch (maj_ver) {
			case 2: size = get_24bitInt(&frmhdr2.size[0]);		break;
			case 3: size = get_32bitInt(&frmhdr.size[0]);		break;
			case 4: size = get_SynchsafeInt(&frmhdr.size[0]);	break;
			}

			if (contained_wd_i(_T("APIC|PIC"), id_str)) {
				//テキストエンコーディング
				BYTE code = fsRead_byte(fs.get());
				size--;
				bool is_16 = (code==1 || code==2);

				std::unique_ptr<char[]> tbuf(new char[32]);
				//画像形式
				if (maj_ver==2) {
					fs->ReadBuffer(tbuf.get(), 3);
					tbuf[3] = '\0';
					size -= 3;
				}
				//MIMIタイプ
				else {
					char *pb = tbuf.get();
					for (int i=0; i<32; i++,pb++) {
						fs->ReadBuffer(pb, 1);
						size--;
						if (*pb=='\0') break;
					}
				}
				UnicodeString img_typ = tbuf.get();

				//画像タイプ
				BYTE pic_typ;
				fs->ReadBuffer(&pic_typ, 1);
				size--;
				//Description は読み飛ばす
				int nul_cnt = 0;
				for (;;) {
					char d = fsRead_byte(fs.get());
					size--;
					if (d!='\0') continue;
					nul_cnt++;
					if (!is_16 && nul_cnt==1) break;
					if ( is_16 && nul_cnt==2) break;
				}
				//イメージ
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
			else {
				fs->Seek(size, soFromCurrent);
			}
			if (fs->Position>=last_p) break;
		}
	}
	catch (...) {
		ok = false;
	}

	return ok;
}
//---------------------------------------------------------------------------
