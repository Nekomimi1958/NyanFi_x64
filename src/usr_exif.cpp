//----------------------------------------------------------------------//
// Exif 情報の処理														//
//																		//
//----------------------------------------------------------------------//
#include <time.h>
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_exif.h"

//---------------------------------------------------------------------------
float CIFF_ev(int v)
{
	float sig = (v<0)? -1.0 : 1.0;
	if (v<0) v = -v;

	float fr = v & 0x1f;
	v -= fr;
	if		(fr==0x0c) fr = 0x20/3.0;
	else if (fr==0x14) fr = 0x40/3.0;
	return sig * (v + fr)/0x20;
}

//---------------------------------------------------------------------------
//！エラー時に EReadError 例外送出
//---------------------------------------------------------------------------
void CIFF_parse(TFileStream *fs, TStringList *lst, int ofs, int length, bool bsw)
{
	UnicodeString itmbuf;

	fs->Seek(ofs + length - 4, soFromBeginning);
	int tb_ofs = fsRead_int4(fs, bsw) + ofs;
	fs->Seek(tb_ofs, soFromBeginning);
	int recn = fsRead_int2(fs, bsw);
	for (int i=0; i<recn; i++) {
		int tag  = fsRead_int2(fs, bsw);
		int len  = fsRead_int4(fs, bsw);
		int aoff = ofs + fsRead_int4(fs, bsw);
		bool in_dir = (tag & 0x4000);

		int p = fs->Seek(0, soFromCurrent);
		if (((tag >> 8)==0x28 || (tag >> 8)==0x30) && !in_dir) {
			//Subdirectory
			CIFF_parse(fs, lst, aoff, len, bsw);
		}
		else {
			int tagID = tag & 0x3fff;
			if (in_dir) {
				fs->Seek(tb_ofs + i*10 + 4, soFromBeginning);
				//FocalLength
				if (tagID==0x1029) {
					fs->Seek(2, soFromCurrent);
					lst->Add(itmbuf.sprintf(_T("37386=%umm"), fsRead_int2(fs, bsw)));
				}
			}
			else {
				fs->Seek(aoff, soFromBeginning);
				//Camera Name
				if (tagID==0x080a) {
					std::unique_ptr<BYTE[]> buf(new BYTE[len + 2]);
					if (fs->Read(buf.get(), len)==len) {
						UnicodeString s;
						int cp = 0;
						while (cp<len && buf[cp]!='\0') s.cat_sprintf(_T("%c"), buf[cp++]);
						lst->Add("271=" + s);
						cp++; s = EmptyStr;
						while (cp<len && buf[cp]!='\0') s.cat_sprintf(_T("%c"), buf[cp++]);
						lst->Add("272=" + s);
					}
				}
				//ImageSpec
				else if (tagID==0x1810) {
					//サイズ
					lst->Add(itmbuf.sprintf(_T("256=%u"), fsRead_int4(fs, bsw)));
					lst->Add(itmbuf.sprintf(_T("257=%u"), fsRead_int4(fs, bsw)));
					fs->Seek(4, soFromCurrent);
					//方向
					int a = fsRead_int4(fs, bsw);
					a = (a==90)? 6 : (a==180)? 3 : (a==270)? 8 : 1;
					lst->Add(itmbuf.sprintf(_T("274=%u"), a));
				}
				//CapturedTime
				else if (tagID==0x180e) {
					TDateTime dt = fsRead_int4(fs, bsw)/(24.0 * 60 * 60);
					dt += (TDateTime(1970, 1, 1) - TDateTime(0));
					lst->Add("36867=" + FormatDateTime("yyyy'/'mm'/'dd hh:nn:ss", dt));
				}
				//ShotInfo
				else if (tagID==0x102a) {
					//index=1..
					fs->Seek(aoff + 1*2, soFromBeginning);
					int a_iso = fsRead_int2(fs, bsw);
					int b_iso = fsRead_int2(fs, bsw);
					a_iso = exp(a_iso/32.0*log(2.0))*100.0;
					b_iso = exp(b_iso/32.0*log(2.0))*100/32.0 + 0.5;
					lst->Add(itmbuf.sprintf(_T("AutoISO=%u"), a_iso));
					lst->Add(itmbuf.sprintf(_T("BaseISO=%u"), b_iso));
					//index=4..
					fs->Seek(aoff + 4*2, soFromBeginning);
					int f  = fsRead_int2(fs, bsw);
					int s  = fsRead_int2(fs, bsw);
					int e  = (short)fsRead_int2(fs, bsw);
					int wb = fsRead_int2(fs, bsw);
					// 4:F
					float ff = exp(CIFF_ev(f)*log(2.0)/2);
					lst->Add(itmbuf.sprintf(_T("33437=%.1f"), ff));
					// 5:ET
					float ss = exp(-CIFF_ev(s)*log(2.0));
					if (ss>0 && ss<0.25001) {
						int r = (int)(0.5 + 1/ss); if (r>50) r -= (r%10);
						lst->Add(itmbuf.sprintf(_T("33434=1/%u"), r));
					}
					else {
						lst->Add(itmbuf.sprintf(_T("33434=%.1f"), ss));
					}
					// 6:EV
					float ev = CIFF_ev(e);
					lst->Add(itmbuf.sprintf(_T("37380=%s%.1f"), (ev>0)? _T("+") : null_TCHAR, ev));
					// 7:WB
					lst->Add(itmbuf.sprintf(_T("CN:4.7=%u"), wb));
				}
				//CameraSettings
				else if (tagID==0x102d) {
					//Index=16 ISO
					fs->Seek(aoff + 16*2, soFromBeginning);
					int v = fsRead_int2(fs, bsw);
					UnicodeString vstr;
					if (v!=0x7fff) {
						if (v & 0x4000) {
						    vstr = v & 0x3fff;
						}
						else {
							switch (v) {
							//case  0: vstr = "n/a";       break;
							//case 14: vstr = "Auto High"; break;
							//case 15: vstr = "Auto";      break;
							case 16: vstr = "50";  break;
							case 17: vstr = "100"; break;
							case 18: vstr = "200"; break;
							case 19: vstr = "400"; break;
							case 20: vstr = "800"; break;
							}
						}
					}
					if (!vstr.IsEmpty()) lst->Add("CameraISO=" + vstr);
					//Index=23 レンズ
					fs->Seek(aoff + 23*2, soFromBeginning);
					lst->Add(itmbuf.sprintf(_T("CN:1.23=%u"), fsRead_int2(fs, bsw)));
					lst->Add(itmbuf.sprintf(_T("CN:1.24=%u"), fsRead_int2(fs, bsw)));
					lst->Add(itmbuf.sprintf(_T("CN:1.25=%u"), fsRead_int2(fs, bsw)));
				}
			}
		}
		fs->Seek(p, soFromBeginning);
	}
}

//---------------------------------------------------------------------------
// 指定ファイルの CIFF 情報を取得して Exif 一覧に変換
//---------------------------------------------------------------------------
bool CIFF_GetInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//結果格納リスト
{
	lst->Clear();
	if (!file_exists(fnam)) return false;

	bool ok;
	try {
		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		unsigned char xbuf[16];
		bool BigEndian;
		fs->ReadBuffer(xbuf, 2);
		if 		(xbuf[0]==0x4d && xbuf[1]==0x4d) BigEndian = true;
		else if (xbuf[0]==0x49 && xbuf[1]==0x49) BigEndian = false;
		else Abort();
		int hlen = fsRead_int4(fs.get(), BigEndian);

		if (!fsRead_check_char(fs.get(), "HEAPCCDR")) Abort();

		int p = fs->Seek(0, soFromEnd);
		CIFF_parse(fs.get(), lst, hlen, p - hlen, BigEndian);

		//ISO
		int iso   = get_ListIntVal(lst, "CameraISO");
		int a_iso = get_ListIntVal(lst, "AutoISO");
		int b_iso = get_ListIntVal(lst, "BaseISO");
		if (iso==0) iso = b_iso * a_iso / 100;
		if (iso==0) iso = b_iso;
		if (iso>0) lst->Add(UnicodeString().sprintf(_T("34855=%u"), iso));

		ok = true;
	}
	catch (...) {
		lst->Clear();
		ok = false;
	}
	return ok;
}

//---------------------------------------------------------------------------
//IFDから情報を取得
//！エラー時に EReadError 例外送出
//---------------------------------------------------------------------------
void EXIF_get_idf_inf(
	TFileStream *fs,
	int  top,
	bool bsw,			//BigEndian か？
	TStringList *lst,	//結果格納リスト
	UnicodeString id)	//追加ID	(default=EmptyStr)
{
	unsigned char vbuf[8];
	UnicodeString tmp;

	unsigned short entn = fsRead_int2(fs, bsw);
	for (unsigned short i=0; i<entn; i++) {
		int tag   = fsRead_int2(fs, bsw);	//タグ
		int dtype = fsRead_int2(fs, bsw);	//値のタイプ
		int count = fsRead_int4(fs, bsw);	//値の数
		fs->ReadBuffer(vbuf, 4);			//値または値へのオフセット

		unsigned short v_s0 = bsw ? (unsigned short)((vbuf[0] << 8)|vbuf[1])
								  : (unsigned short)((vbuf[1] << 8)|vbuf[0]);
		unsigned short v_s1 = bsw ? (unsigned short)((vbuf[2] << 8)|vbuf[3])
								  : (unsigned short)((vbuf[3] << 8)|vbuf[2]);

		int v_ui = bsw ? (unsigned int)((vbuf[0] << 24) | (vbuf[1] << 16) | (vbuf[2] << 8) | vbuf[3])
					   : (unsigned int)(vbuf[0] | (vbuf[1] << 8) | (vbuf[2] << 16) | (vbuf[3] << 24));
		int v_si = bsw ? (int)((vbuf[0] << 24) | (vbuf[1] << 16) | (vbuf[2] << 8) | vbuf[3])
					   : (int)(vbuf[0] | (vbuf[1] << 8) | (vbuf[2] << 16) | (vbuf[3] << 24));

		UnicodeString val_str;
		int p;

		if (tag==37500) {	//メーカーノート
			val_str = v_ui;
		}
		else {
			switch (dtype) {
			case  1:	//BYTE
				for (int i=0; i<count && i<4; i++) {
					if (i>0) val_str += ",";
					val_str.cat_sprintf(_T("%u"), vbuf[i]);
				}
				break;
			case  3:	//SHORT
				if (count>2) {
					p = fs->Seek(0, soFromCurrent);  fs->Seek((int)(top + v_ui), soFromBeginning);
					for (int i=0; i<count; i++) {
						if (i>0) val_str += ",";
						val_str.cat_sprintf(_T("%u"), fsRead_int2(fs, bsw));
					}
					fs->Seek(p, soFromBeginning);
				}
				else {
					val_str = v_s0;
					if (count==2) val_str.cat_sprintf(_T(",%u"), v_s1);
				}
				break;
			case  4:	//LONG
				if (count>1) {
					p = fs->Seek(0, soFromCurrent);  fs->Seek((int)(top + v_ui), soFromBeginning);
					for (int i=0; i<count; i++) {
						if (i>0) val_str += ",";
						val_str.cat_sprintf(_T("%u"), fsRead_int4(fs, bsw));
					}
					fs->Seek(p, soFromBeginning);
				}
				else {
					val_str = v_ui;
				}
				break;
			case  9:	//SLONG
				if (count>1) {
					p = fs->Seek(0, soFromCurrent);  fs->Seek((int)(top + v_ui), soFromBeginning);
					for (int i=0; i<count; i++) {
						if (i>0) val_str += ",";
						val_str.cat_sprintf(_T("%d"), fsRead_int4(fs, bsw));
					}
					fs->Seek(p, soFromBeginning);
				}
				else {
					val_str = v_si;
				}
				break;

			case  2:	//ASCII
				if (count<=4) {
					val_str = (char*)vbuf;
				}
				else {
					std::unique_ptr<char[]> sbuf(new char[count + 4]);
					p = fs->Seek(0, soFromCurrent);
					fs->Seek((int)(top + v_ui), soFromBeginning);
					fs->ReadBuffer((void*)sbuf.get(), count);
					fs->Seek(p, soFromBeginning);
					val_str = sbuf.get();
				}
				if (tag==36867 && val_str.Length()==19) {	// 撮影日時
					val_str[5] = '/';  val_str[8] = '/';
				}
				break;

			case  5:	//RATIONAL
				p = fs->Seek(0, soFromCurrent);  fs->Seek((int)(top + v_ui), soFromBeginning);
				for (int i=0; i<count; i++) {
					if (i>0) val_str += ",";
					unsigned int n0 = (unsigned int)fsRead_int4(fs, bsw);
					unsigned int n1 = (unsigned int)fsRead_int4(fs, bsw);
					if (SameStr(id, "GPS:"))	//GPS
						val_str.cat_sprintf(_T("%.8f"), (n0>0 && n1>0)? 1.0*n0/n1 : 0.0);
					else if (tag==33437)		//F
						val_str.cat_sprintf(_T("%.1f"), (n0>0 && n1>0)? 1.0*n0/n1 : 0.0);
					else if (tag==37386)		//焦点距離
						val_str.cat_sprintf(_T("%umm"), (n0>0 && n1>0)? (int)((1.0*n0/n1) + 0.5) : 0);
					else if (SameStr(id, "NK:") && tag==132) {	//レンズ
						if (n0>0 && n1>0) {
							if (i<2)
								val_str.cat_sprintf(_T("%u"), (n0>0 && n1>0)? (int)((1.0*n0/n1)+0.5) : 0);	//焦点距離
							else
								val_str.cat_sprintf(_T("%.1f"), (n0>0 && n1>0)? 1.0*n0/n1 : 0.0);			//F
						}
						if (i==3) {
							TStringDynArray ibuf = get_csv_array(val_str, 4, true);
							val_str = (ibuf[0]==ibuf[1])? ibuf[0] : tmp.sprintf(_T("%s-%s"), ibuf[0].c_str(), ibuf[1].c_str());
							val_str += "mm F/";
							val_str += (ibuf[2]==ibuf[3])? ibuf[2] : tmp.sprintf(_T("%s-%s"), ibuf[2].c_str(), ibuf[3].c_str());
						}
					}
					else {
						if (n0>0 && n1>0) {
							for (int j=0; j<4; j++) {
								unsigned int nn;
								switch (j) {
								case 0: nn = 10; break;
								case 1: nn =  2; break;
								case 2: nn =  3; break;
								case 3: nn =  5; break;
								}
								while (n0>=nn && n1>=nn) {
									if (n0%nn!=0 || n1%nn!=0) break;
									n0/=nn; n1/=nn;
								}
							}
							if (n0>=n1 && n0%n1==0) {
								val_str.cat_sprintf(_T("%u"), n0/n1);
							}
							else {
								float r = 1.0*n0/n1;
								if (r>0 && r<0.25001)
									val_str.cat_sprintf(_T("1/%u"), (unsigned int)(0.5 + 1/r));
								else
									val_str.cat_sprintf(_T("%u/%u"), n0, n1);
							}
						}
						else {
							val_str += "0";
						}
					}
				}
				fs->Seek(p, soFromBeginning);
				break;

			case 10:	//SRATIONAL
				p = fs->Seek(0, soFromCurrent);  fs->Seek((int)(top + v_ui), soFromBeginning);
				for (int i=0; i<count; i++) {
					if (i>0) val_str += ",";

					int n0 = fsRead_int4(fs, bsw);
					int n1 = fsRead_int4(fs, bsw);

					for (int j=0; j<4; j++) {
						int nn;
						switch (j) {
						case 0: nn = 10; break;
						case 1: nn =  2; break;
						case 2: nn =  3; break;
						case 3: nn =  5; break;
						}
						while (n0>=nn && n1>=nn) {
							if (n0%nn!=0 || n1%nn!=0) break;
							n0/=nn; n1/=nn;
						}
					}

					if (tag==37380) {	//露光補正
						if (n0==0 || n1==0)
							val_str += "0.0";
						else
							val_str.cat_sprintf(_T("%+.1f"), 1.0*n0/n1);
					}
					else if (n0==0 || n1==0) {
						val_str += "0";
					}
					else if (abs(n0)>=abs(n1) && n0%n1==0) {
						val_str.cat_sprintf(_T("%d"), n0/n1);
					}
					else {
						float r = 1.0*n0/n1;
						if (r>0 && r<0.25001)
							val_str.cat_sprintf(_T("1/%d"), (int)(0.5 + 1/r));
						else
							val_str.cat_sprintf(_T("%d/%d"), n0, n1);
					}
				}
				fs->Seek(p, soFromBeginning);
				break;

			case 7:	//UNDEFINED
				switch (tag) {
				case 36864:	//Exif Version
				case 40960:	//FlashPixVersion
					vbuf[4] = '\0';
					val_str = (char*)vbuf;
					break;
				case 37121:	//ComponentsConfiguration
					val_str.sprintf(_T("%u,%u,%u,%u"), vbuf[0], vbuf[1], vbuf[2], vbuf[3]);
					break;
				default:
					val_str = v_ui;
				}
				break;
			}
		}

		val_str = val_str.Trim();
		if (!val_str.IsEmpty()) lst->Add(tmp.sprintf(_T("%s%u=%s"), id.c_str(), tag, val_str.c_str()));
	}
}

//---------------------------------------------------------------------------
//項目内容の書式を整える
//---------------------------------------------------------------------------
void EXIF_format_inf(UnicodeString fext, TStringList *lst)
{
	UnicodeString vnam, vstr, tmp;

	vnam = "274";
	int idx  = lst->IndexOfName(vnam);
	if (idx!=-1) {
		vstr = lst->Values[vnam];
		if (!vstr.IsEmpty()) {
			vstr = get_word_i_idx("0 ?|1 横(そのまま)|2 横(左右反転)|3 横(180度回転)|4 横(上下反転)|5 縦(左右反転 + 270度回転)|6 縦(90度回転)|7 縦(左右反転 + 90度回転)|8 縦(270度回転)", vstr.ToIntDef(0));
			lst->Add(tmp.sprintf(_T("%sL=%s"), vnam.c_str(), vstr.c_str()));
		}
	}

	//ISO
	vnam = "34855";
	vstr = EmptyStr;
	idx  = lst->IndexOfName(vnam);
	if (test_FileExt(fext, ".rw2")) vstr = lst->Values["23"];
	if (vstr.IsEmpty()) vstr = lst->Values[vnam];
	if (vstr.IsEmpty() || SameStr(vstr, "0")) vstr = get_tkn_r(lst->Values["NK:2"], ',');
	if (!vstr.IsEmpty()) {
		if (idx!=-1) lst->ValueFromIndex[idx] = vstr;
		else		 lst->Add(tmp.sprintf(_T("%s=%s"), vnam.c_str(), vstr.c_str()));
	}

	//露出時間
	vnam = "33434";
	vstr = lst->Values[vnam];
	if (!vstr.IsEmpty()) lst->Add(tmp.sprintf(_T("%sU=%s秒"), vnam.c_str(), vstr.c_str()));

	//露出プログラム
	vnam = "34850";
	idx  = lst->IndexOfName(vnam);
	if (idx!=-1) {
		vstr = lst->Values[vnam];
		if (!vstr.IsEmpty()) {
			int n = vstr.ToIntDef(0);
			vstr = get_word_i_idx(
				"不明|マニュアル|ノーマル|絞り優先|シャッター優先|Creative|Action|ポートレート|ランドスケープ", n);
			if (vstr.IsEmpty()) vstr = "不明";
			lst->Add(tmp.sprintf(_T("%sL=%s"), vnam.c_str(), vstr.c_str()));
		}
	}

	//測光方式
	vnam = "37383";
	idx  = lst->IndexOfName(vnam);
	if (idx!=-1) {
		vstr = lst->ValueFromIndex[idx];
		if (!vstr.IsEmpty()) {
			int n = vstr.ToIntDef(0);
			if (n==255) {
				vstr = "その他";
			}
			else {
				vstr = get_word_i_idx("不明|平均|中央重視|スポット|マルチスポット|分割測光|部分測光", n);
				if (vstr.IsEmpty()) vstr = "不明";
			}
			lst->ValueFromIndex[idx] = vstr;
		}
	}

	//フラッシュ
	vnam = "37385";
	idx  = lst->IndexOfName(vnam);
	if (idx!=-1) {
		vstr = lst->ValueFromIndex[idx];
		if (!vstr.IsEmpty()) {
			vstr.sprintf(_T("%s"), ((vstr.ToIntDef(0) & 1)==1)? _T("ON") : _T("OFF"));
			lst->ValueFromIndex[idx] = vstr;
		}
	}

	//WB
	vnam = "CN:4.7";
	idx  = lst->IndexOfName(vnam);
	if (idx!=-1) {
		vstr = lst->ValueFromIndex[idx];
		if (!vstr.IsEmpty()) {
			vstr = get_word_i_idx(
				"Auto|Sunny|Cloudy|Tungsten|Fluorescent|Flash|Custom|Black & White|Shade|Manual", vstr.ToIntDef(-1));
			lst->ValueFromIndex[idx] = vstr;
		}
	}

	//LensModel
	vstr = lst->Values["42036"];
	if (vstr.IsEmpty()) {
		//Nikon
		vstr = lst->Values["NK:132"];
		if (vstr.IsEmpty()) {
			//Canon (焦点距離のみ)
			vstr = lst->Values["CN:1.23"];
			if (!vstr.IsEmpty()) {
				int t = vstr.ToIntDef(0);				//T端
				int w = get_ListIntVal(lst, "CN:1.24");	//W端
				int u = get_ListIntVal(lst, "CN:1.25");	//換算単位
				if (t>0 && w>0 && u>0) {
					t /=u; w /= u;
					vstr = w;
					if (w!=t) vstr.cat_sprintf(_T("-%u"), t);
					vstr += "mm";
				}
			}
		}
		if (!vstr.IsEmpty()) lst->Add(tmp.sprintf(_T("42036=%s"), vstr.c_str()));
	}

	//GPS
	for (int i=0; i<2; i++) {
		UnicodeString ref  = lst->Values[(i==0)? "GPS:1" : "GPS:3"];
		if (ref.Length()!=1) continue;

		UnicodeString vnam;
		vnam.sprintf(_T("%s"), (i==0)? _T("GPS:2") : _T("GPS:4"));
		idx  = lst->IndexOfName(vnam);	if (idx==-1) continue;
		vstr = lst->Values[vnam];		if (vstr.IsEmpty()) continue;
		TStringDynArray ibuf = get_csv_array(vstr, 3, true);

		try {
			double d = ibuf[0].ToDouble();
			double m = ibuf[1].ToDouble();
			double s = ibuf[2].ToDouble();
			//DEG
			double deg = d + m/60.0 + s/3600.0;
			vstr = (contained_wd_i(_T("S|W"), ref))? "-" : "";
			vstr.cat_sprintf(_T("%.8f ("), deg);
			//DMS
			m += modf(d, &d) * 60;
			s += modf(m, &m) * 60;
			vstr.cat_sprintf(_T("%s%.0f°%.0f′%.2f″)"), ref.c_str(), d, m, s);
		}
		catch (...) {
			vstr = EmptyStr;
		}

		lst->Strings[idx] = tmp.sprintf(_T("%s=%s"), vnam.c_str(), vstr.c_str());
	}
}

//---------------------------------------------------------------------------
// 指定ファイルの Exif 情報一覧を取得
//---------------------------------------------------------------------------
bool EXIF_GetInf(
	UnicodeString fnam,		//ファイル名
	TStringList *lst)		//結果格納リスト(タグNo.=文字列 形式)
{
	lst->Clear();
	if (!file_exists(fnam)) return false;

	UnicodeString fext = get_extension(fnam);
	bool isJpeg  = test_FileExt(fext, ".jpg.jpeg.jpe");
	bool isHeic	 = test_FileExt(fext, ".heic");
	bool isNikon = test_FileExt(fext, ".nef.nrw");
	bool isCR2	 = test_FileExt(fext, ".cr2");
	bool isCRW	 = test_FileExt(fext, ".crw");
	bool isRaf	 = test_FileExt(fext, ".raf");
	bool isX3f	 = test_FileExt(fext, ".x3f");

	unsigned char xbuf[16];
	UnicodeString itmbuf;

	try {
		if (isCRW) {
			if (!CIFF_GetInf(fnam, lst)) Abort();
		}
		else {
			std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
			TFileStream *fsp = fs.get();

			int cfa_ofs = 0;
			int top = 0;
			if (isRaf) {
				if (!fsRead_check_char(fsp, "FUJIFILMCCD-RAW")) Abort();
				if (fs->Seek(84, soFromBeginning)!=84) Abort();
				top = fsRead_int4(fsp, true);		if (top==0) Abort();
				fs->Seek(4, soFromCurrent);
				cfa_ofs = fsRead_int4(fsp, true);
			}
			else if (isX3f) {
				if (!fsRead_check_char(fsp, "FOVb")) Abort();
				int vi = fsRead_int2(fsp, false);
				int vj = fsRead_int2(fsp, false);
				int v = vj*10 + vi;
				top = (v>=40)? 0x300 : (v>22)? 104 + 160 : 72 + 160;
				lst->Add(itmbuf.sprintf(_T("FOVbVer=%u.%u"), vj, vi));
				if (fs->Seek(top, soFromBeginning)!=top) Abort();
				fs->Seek((v>=40)? 40 : 28, soFromBeginning);
				int w = fsRead_int4(fsp, false);
				int h = fsRead_int4(fsp, false);
				int r = fsRead_int4(fsp, false);
				if (r==90 || r==270) std::swap(w, h);
				if (w>0 && h>0) {
					lst->Add(itmbuf.sprintf(_T("X3F:256=%u"), w));
					lst->Add(itmbuf.sprintf(_T("X3F:257=%u"), h));
				}
				if (fs->Seek(top, soFromBeginning)!=top) Abort();
				if (!fsRead_check_char(fsp, "SECi")) Abort();
				top += 28;
			}

			if (fs->Seek(top, soFromBeginning)!=top) Abort();

			if (isRaf || isX3f) {
				//APP1
				if (fs->Read(xbuf, 4)<4) Abort();
				if (xbuf[0]!=0xff || xbuf[1]!=0xd8 || xbuf[2]!=0xff || xbuf[3]!=0xe1) Abort();
				fs->Seek(2, soFromCurrent);
				if (!fsRead_check_char(fsp, "Exif")) Abort();
				fs->Seek(2, soFromCurrent);
				top += 12;
			}
			else if (isJpeg) {
				if (fs->Read(xbuf, 4)<4) Abort();
				if (xbuf[0]!=0xff || xbuf[1]!=0xd8 || xbuf[2]!=0xff) Abort();
				//先頭がAPP0
				if (xbuf[3]==0xe0) {
					//APP0をスキップ
					int len = fsRead_int2(fsp, true);
					fs->Seek(len - 2, soFromCurrent);
					//APP1
					if (fs->Read(xbuf, 2)<2) Abort();
					if (xbuf[0]!=0xff || xbuf[1]!=0xe1) Abort();
				}
				//先頭がAPP1
				else if (xbuf[3]==0xe1) {
					fs->Seek(2, soFromCurrent);
					if (!fsRead_check_char(fsp, "Exif")) Abort();
					fs->Seek(2, soFromCurrent);
					top = fs->Position;
				}
				else Abort();
			}
			else if (isHeic) {
				int bsz = fsRead_int4(fsp, true);	//Boxサイズ
				if (bsz!=24) Abort();
				if (!fsRead_check_char(fsp, "ftyp")) Abort();
				//meta
				fs->Seek(24, soFromBeginning);
				bsz = fsRead_int4(fsp, true);
				if (!fsRead_check_char(fsp, "meta")) Abort();
				//mdat
				fs->Seek(24 + bsz, soFromBeginning);
				fs->Seek(4, soFromCurrent);
				if (!fsRead_check_char(fsp, "mdat")) Abort();
				fs->Seek(8, soFromCurrent);
				bsz = fsRead_int4(fsp, true);
				if (bsz>6) {
					fs->Seek(bsz, soFromCurrent);
					bsz = fsRead_int4(fsp, true);
				}
				if (bsz==6) {
					if (!fsRead_check_char(fsp, "Exif")) Abort();
					fs->Seek(2, soFromCurrent);
					top = fs->Position;
				}
				else Abort();
			}

			//Byte Order
			bool BigEndian;
			if (fs->Read(xbuf, 2)<2) Abort();
			if		(xbuf[0]==0x4d && xbuf[1]==0x4d) BigEndian = true;
			else if (xbuf[0]==0x49 && xbuf[1]==0x49) BigEndian = false;
			else Abort();

			//Magic Number
			int magic_no = fsRead_int2(fsp, BigEndian);
			if (test_FileExt(fext, ".rw2.raw")) {
				if (magic_no!=0x0055) Abort();
			}
			else if (test_FileExt(fext, ".orf")) {
				if (magic_no!=0x4f52 && magic_no!=0x5352) Abort();
			}
			else if (magic_no!=0x002a) Abort();

			//0th IFD
			int idf_ofs = fsRead_int4(fsp, BigEndian);
			fs->Seek(top + idf_ofs, soFromBeginning);
			EXIF_get_idf_inf(fsp, top,  BigEndian, lst);

			//Exif IFD
			UnicodeString tmpstr = lst->Values["34665"];
			if (!tmpstr.IsEmpty()) {
				idf_ofs = tmpstr.ToIntDef(0);
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					EXIF_get_idf_inf(fsp, top, BigEndian, lst);
				}
			}

			//SubIFD
			tmpstr = lst->Values["330"];
			if (!tmpstr.IsEmpty()) {
				TStringDynArray t_lst = SplitString(tmpstr, ",");
				//Sub-0
				idf_ofs = (t_lst.Length>0)? t_lst[0].ToIntDef(0) : 0;
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					EXIF_get_idf_inf(fsp, top, BigEndian, lst, "S0:");
					//プレビュー画像情報
					int typ  = get_ListIntVal(lst, "S0:259");
					int ptr  = get_ListIntVal(lst, "S0:513");
					int size = get_ListIntVal(lst, "S0:514");
					if (typ==6 && ptr>0 && size>0) {
						fs->Seek(ptr, soFromBeginning);
						for (;;) {
							if (fs->Read(xbuf, 1)<1) break;
							if (xbuf[0]!=0xff) continue;
							if (fs->Read(xbuf, 1)<1) break;
							int mark = xbuf[0];
							if (mark>=0xd0 && mark<=0xd9) continue;	//SOI,EOI,RSTnはスキップ
							int seglen = fsRead_int2(fsp, true);
							if (mark>=0xc0 && mark<=0xcf && mark!=0xc4 && mark!=0xc8) {
								//SOF
								fs->Seek(1, soFromCurrent);
								int h = fsRead_int2(fsp, true);
								int w = fsRead_int2(fsp, true);
								if (h>0 && w>0) {
									lst->Add(itmbuf.sprintf(_T("SOF:256="), w));
									lst->Add(itmbuf.sprintf(_T("SOF:257="), h));
								}
								break;
							}
							else fs->Seek(seglen - 2, soFromCurrent);
						}
					}
				}

				//Sub-1
				idf_ofs = (t_lst.Length>1)? t_lst[1].ToIntDef(0) : 0;
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					EXIF_get_idf_inf(fsp, top, BigEndian, lst, "S1:");
				}

				//Sub-2
				idf_ofs = (t_lst.Length>2)? t_lst[2].ToIntDef(0) : 0;
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					EXIF_get_idf_inf(fsp, top, BigEndian, lst, "S2:");
				}
			}

			//メーカーノート
			tmpstr = lst->Values["37500"];
			if (!tmpstr.IsEmpty()) {
				idf_ofs = tmpstr.ToIntDef(0);
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					//Nikon Type-3
					if (isNikon && fsRead_check_char(fsp, "Nikon") && fsRead_int2(fsp, true)==0x02) {
						fs->Seek(3 + 8, soFromCurrent);
						EXIF_get_idf_inf(fsp, top + idf_ofs + 10, BigEndian, lst, "NK:");
					}
					//Canon
					else if (isCR2) {
						int entn = fsRead_int2(fsp, false);
						for (int i=0; i<entn; i++) {
							int tag = fsRead_int2(fsp, false);	if (tag>4) break;
							int typ = fsRead_int2(fsp, false);
							int cnt = fsRead_int4(fsp, false);
							int ofs = fsRead_int4(fsp, false);
							if ((tag==1 || tag==4) && typ==3) {
								lst->Add(itmbuf.sprintf(_T("CN:%u=%u,%u,%u"),tag, typ, cnt, ofs));
								int p = fs->Seek(0, soFromCurrent);
								fs->Seek(ofs, soFromBeginning);
								for (int j=0; j<cnt; j++)
									lst->Add(itmbuf.sprintf(_T("CN:%u.%u=%u"),
												tag, j, fsRead_int2(fsp, false)));
								fs->Seek(p, soFromBeginning);
							}
						}
					}
				}
			}

			//FujiFilm(.raf) CFA Header
			if (isRaf && cfa_ofs>0) {
				UnicodeString val_str;
				if (fs->Seek(cfa_ofs, soFromBeginning)!=cfa_ofs) Abort();
				int n = fsRead_int4(fsp, true);	if (n==0) Abort();
				for (int i=0; i<n; i++) {
					int tag = fsRead_int2(fsp, true);	if (tag==0) break;
					int siz = fsRead_int2(fsp, true);	if (siz==0) break;
					if (tag==0x111) {	//公称RAW画像サイズ
						if (siz==4) {
							lst->Add(itmbuf.sprintf(_T("CFA:257=%u"), fsRead_int2(fsp, true)));
							lst->Add(itmbuf.sprintf(_T("CFA:256=%u"), fsRead_int2(fsp, true)));
						}
						break;
					}
					else fs->Seek(siz, soFromCurrent);
				}
			}

			//GPS
			tmpstr = lst->Values["34853"];
			if (!tmpstr.IsEmpty()) {
				idf_ofs = tmpstr.ToIntDef(0);
				if (idf_ofs>0) {
					fs->Seek(top + idf_ofs, soFromBeginning);
					EXIF_get_idf_inf(fsp, top, BigEndian, lst, "GPS:");
				}
			}
		}

		EXIF_format_inf(fext, lst);
		return true;
	}
	catch (...) {
		lst->Clear();
		return false;
	}
}

//---------------------------------------------------------------------------
//Exifリストから画像サイズを取得
//---------------------------------------------------------------------------
bool Exif_GetImgSize(TStringList *lst, UnicodeString fext, unsigned int *w, unsigned int *h)
{
	UnicodeString w_str, h_str;

	//RW2
	if (test_FileExt(fext, ".rw2.raw") && !lst->Values["4"].IsEmpty()) {
		w_str = lst->Values["2"]; h_str = lst->Values["3"];
		int s_w = get_ListIntVal(lst, "7") - get_ListIntVal(lst, "5");
		if (s_w>0) w_str = s_w;
		int s_h = get_ListIntVal(lst, "6") - get_ListIntVal(lst, "4");
		if (s_h>0) h_str = s_h;
	}
	//X3F
	else if (test_FileExt(fext, ".x3f") && !lst->Values["X3F:256"].IsEmpty()) {
		w_str = lst->Values["X3F:256"]; h_str = lst->Values["X3F:257"];
	}
	//RAF (CFA)
	else if (!lst->Values["CFA:256"].IsEmpty()) {
		w_str = lst->Values["CFA:256"]; h_str = lst->Values["CFA:257"];
	}
	else {
		int wd = 0, hi = 0;
		if (!ListVal_is_empty(lst, "40962")) {
			wd = get_ListIntVal(lst, "40962");
			hi = get_ListIntVal(lst, "40963");
		}

		if (!test_FileExt(fext, ".jpg.jpeg.jpe") || wd==0 || hi==0) {
			if (!ListVal_is_empty(lst, "S2:256")) {
				wd = std::max(get_ListIntVal(lst, "S2:256"), wd);
				hi = std::max(get_ListIntVal(lst, "S2:257"), hi);
			}
			if (!ListVal_is_empty(lst, "S1:256")) {
				wd = std::max(get_ListIntVal(lst, "S1:256"), wd);
				hi = std::max(get_ListIntVal(lst, "S1:257"), hi);
			}
			if (!ListVal_is_empty(lst, "S0:256")) {
				wd = std::max(get_ListIntVal(lst, "S0:256"), wd);
				hi = std::max(get_ListIntVal(lst, "S0:257"), hi);
			}
			if (!ListVal_is_empty(lst, "256")) {
				wd = std::max(get_ListIntVal(lst, "256"), wd);
				hi = std::max(get_ListIntVal(lst, "257"), hi);
			}
		}

		if (wd>0 && hi>0) {
			w_str = wd;
			h_str = hi;
		}
	}

	if (w_str.IsEmpty() || h_str.IsEmpty()) return false;

	//NEF,NRW
	//実効サイズとサムネイルサイズを比較して画像サイズを設定
	if (test_FileExt(fext, ".nef.nrw") && !ListVal_is_empty(lst, "SOF:256")) {
		float s0 = w_str.ToIntDef(0) * h_str.ToIntDef(0);
		float s1 = get_ListIntVal(lst, "SOF:256") * get_ListIntVal(lst, "SOF:257");
		if (fabs(s0/s1 - 1.0)<0.05) {	//***
			w_str = lst->Values["SOF:256"];
			h_str = lst->Values["SOF:257"];
		}
	}

	//方向
	if (!test_FileExt(fext, ".3fr")) {
		int ori = get_ListIntVal(lst, "274");
		if (ori==6 || ori==8) std::swap(w_str, h_str);
	}

	*w = w_str.ToIntDef(0);
	*h = h_str.ToIntDef(0);
	return true;
}

//---------------------------------------------------------------------------
//ファイルのExif撮影日時を取得
//---------------------------------------------------------------------------
bool EXIF_GetExifTime(UnicodeString fnam, TDateTime &dt)
{
	bool res = false;
	std::unique_ptr<TStringList> taglst(new TStringList());
	if (EXIF_GetInf(fnam, taglst.get())) {
		UnicodeString tstr = taglst->Values["36867"];	// = 撮影日時
		if (tstr.Length()==19) {
			try {
				dt	= str_to_DateTime(tstr);
				res = true;
			}
			catch (EConvertError &e) {
				;
			}
		}
	}
	return res;
}

//---------------------------------------------------------------------------
//ファイルのExif撮影日時を指定書式で取得
//---------------------------------------------------------------------------
UnicodeString EXIF_GetExifTimeStr(UnicodeString fnam, UnicodeString format)
{
	UnicodeString ret_str;
	TDateTime dt;
	if (EXIF_GetExifTime(fnam, dt)) ret_str = FormatDateTime(format, dt);
	return ret_str;
}

//---------------------------------------------------------------------------
//ファイルのタイムスタンプを Exif 撮影日時に
//---------------------------------------------------------------------------
bool EXIF_SetExifTime(UnicodeString fnam)
{
	bool ret = false;
	std::unique_ptr<TStringList> taglst(new TStringList());
	if (EXIF_GetInf(fnam, taglst.get())) {
		UnicodeString tstr = taglst->Values["36867"];	// = 撮影日時
		if (tstr.Length()==19) {
			TDateTime dt;
			try {
				dt	= str_to_DateTime(tstr);
				ret = set_file_age(fnam, dt);
			}
			catch (EConvertError &e) {
				ret = false;
			}
		}
	}
	return ret;
}

//---------------------------------------------------------------------------
//JpegファイルからExif情報を削除
//  戻り値:  0=成功 -1=失敗 1=Exifなし
//---------------------------------------------------------------------------
int EXIF_DelJpgExif(
	UnicodeString fnam,		//対象ファイル名
	UnicodeString onam,		//出力ファイル名
	bool keep_time)			//タイムスタンプを維持
{
	try {
		if (!file_exists(fnam)) Abort();

		//処理前にタイムスタンプを取っておく
		TDateTime ft;
		if (keep_time) ft = get_file_age(fnam);

		std::unique_ptr<TFileStream> i_fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		TFileStream *isp = i_fs.get();

		std::unique_ptr<BYTE[]> xbuf(new BYTE[32]);
		//APP1
		if (i_fs->Read(xbuf.get(), 4)<4) Abort();
		if (xbuf[0]!=0xff || xbuf[1]!=0xd8 || xbuf[2]!=0xff || xbuf[3]!=0xe1) throw EAbort("NoExif");
		int app1_len = fsRead_int2(isp, true);
		if (!fsRead_check_char(isp, "Exif")) Abort();
		i_fs->Seek(2, soFromCurrent);
		int app1_ofs = app1_len + 4;
		int img_size = i_fs->Size - app1_ofs;

		//Byte Order
		bool BigEndian;
		if (i_fs->Read(xbuf.get(), 2)<2) Abort();
		if (xbuf[0]==0x4d) {
			if (xbuf[1]!=0x4d) Abort();
			BigEndian = true;
		}
		else if (xbuf[0]==0x49) {
			if (xbuf[1]!=0x49) Abort();
			BigEndian = false;
		}
		else Abort();

		//Magic Number
		if (fsRead_int2(isp, BigEndian)!=0x002a) Abort();

		//0th IFD
		int top = 12;
		int idf_ofs = fsRead_int4(isp, BigEndian);
		i_fs->Seek(top + idf_ofs, soFromBeginning);
		std::unique_ptr<TStringList> i_lst(new TStringList());
		EXIF_get_idf_inf(isp, top, BigEndian, i_lst.get());

		//解像度を取得
		int rm = 2, rx = 72, ry = 72;	//デフォルト 72dpi
		if (!ListVal_is_empty(i_lst.get(), "296")) {
			rm = get_ListIntVal(i_lst.get(), "296", 2);	//dpi
			rx = get_ListIntVal(i_lst.get(), "282", 72);
			ry = get_ListIntVal(i_lst.get(), "283", 72);
		}
		if (rm==3) rm = 2; else rm = 1;	//APP1用に変換

		//APP1をスキップ
		i_fs->Seek(app1_ofs, soFromBeginning);
		//メモリストリームにイメージをコピー
		std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
		if (ms->CopyFrom(isp, img_size)<img_size) Abort();
		//入力ストリームを破棄
		TFileStream *sp = i_fs.release();
		delete sp;

		//APP0を作成して書き込み
		if (!file_exists(onam)) {
			if (!create_EmptyFile(onam)) Abort();
		}
		std::unique_ptr<TFileStream> o_fs(new TFileStream(onam, fmCreate));
		xbuf[0]  = 0xff; xbuf[1] = 0xd8;		//SOI
		xbuf[2]  = 0xff; xbuf[3] = 0xe0;		//APP0マーカ
		xbuf[4]  = 0x00; xbuf[5] = 0x10;
		xbuf[6]  = 'J'; xbuf[7] = 'F'; xbuf[8] = 'I'; xbuf[9] = 'F'; xbuf[10] = 0x00;
		xbuf[11] = 0x01; xbuf[12] = 0x01;		//バージョン1.01
		xbuf[13] = rm;							//解像度
		xbuf[14] = rx/256; xbuf[15] = rx%256;
		xbuf[16] = ry/256; xbuf[17] = ry%256;
		xbuf[18] = 0x00; xbuf[19] = 0x00;		//サムネイルなし
		o_fs->Write(xbuf.get(), 20);
		//イメージを書き込み
		o_fs->Write(ms->Memory, ms->Size);
		//出力ストリームを破棄
		sp = o_fs.release();
		delete sp;

		//タイムスタンプを設定
		if (keep_time && (int)ft>0) set_file_age(onam, ft);

		return 0;
	}
	catch (EAbort &e) {
		return SameText(e.Message, "NoExif")? 1 : -1;
	}
	catch (...) {
		//エラー
		return -1;
	}
}
//---------------------------------------------------------------------------
