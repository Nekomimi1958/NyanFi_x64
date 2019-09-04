//----------------------------------------------------------------------//
// �e��t�H�[�}�b�g�̃t�@�C�����擾									//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrFileInfH
#define UsrFileInfH

//---------------------------------------------------------------------------
#include <vcl.h>
#include "usr_arc.h"

//---------------------------------------------------------------------------
#define FILE_RBUF_SIZE	32768		//�t�@�C���ǂݍ��ݗp�o�b�t�@�T�C�Y(32KB)
#define TXT_DETECT_SIZE	0x10000		//�e�L�X�g����̓ǂݍ��݃T�C�Y(64KB)

//---------------------------------------------------------------------------
//�g���q
#if defined(_WIN64)
#define FEXT_ARCHIVE	FEXT_ZIP FEXT_7Z FEXT_TAR FEXT_RAR
#else
#define FEXT_ARCHIVE	FEXT_ZIP FEXT_7Z FEXT_LHA FEXT_CAB FEXT_TAR FEXT_RAR FEXT_ISO
#endif																			//�A�[�J�C�u

#define FEXT_EXECUTE	_T(".exe.com.bat.cmd.msi.msu.lnk.scr")					//���s�\
#define FEXT_JPEG		_T(".jpg.jpeg.jpe")										//Jpeg
#define FEXT_META		_T(".wmf.emf")											//���^�t�@�C��
#define FEXT_RAW		_T(".3fr.arw.cr2.crw.dng.erf.mef.nef.nrw.orf.pef.raf.raw.rw2.sr2.srw.x3f")
																				//RAW
#define FEXT_PSP		_T(".psp.pspimage")										//Paint Shop Pro
#define FEXT_WICSTD		FEXT_JPEG _T(".bmp.png.gif.tif.wdp.hdp")				//WIC���W���ŕ\���\
#define FEXT_EXIF		FEXT_JPEG FEXT_RAW _T(".heic")							//Exif���擾�\
#define FEXT_IMAGE		FEXT_WICSTD FEXT_RAW FEXT_META _T(".heic.webp")			//�摜
#define FEXT_WEBIMG		FEXT_JPEG _T(".gif.png.webp.ico")						//Web�ŗp������摜
#define FEXT_FONT		_T(".ttf.ttc.fon.otf")									//�t�H���g
#define FEXT_MCISND		_T(".wav.wma.mp3.mid")									//MCI�Ή��T�E���h
#define FEXT_VIDEO		_T(".avi.asf.flv.mkv.mov.mp4.mpg.mts.ogm.ts.m2ts.vob.webm.wmv")
																				//����

#define FEXT_DURATION	FEXT_MCISND FEXT_VIDEO _T(".cda.opus.flac")				//����������
#define FEXT_ICONVIEW	_T(".exe.dll.bpl.dpl.cpl.ico.cur.ani")					//�A�C�R�����o�\
#define FEXT_ICON_CNT	_T(".exe.dll.bpl.dpl.cpl.ico.cur")						//�A�C�R�����擾�\
#define FEXT_APPINFO	_T(".exe.dll.ax.rll.bpl.dpl.cpl.ocx.spi.sph.scr")		//�A�v�����擾�\
#define FEXT_INDIVICO	_T(".exe.ico.cur.ani.lnk.url.library-ms")				//�ŗL�̃A�C�R��������

#define FEXT_CSV		_T(".csv.tsv")											//CSV

#define FEXT_TEXT		FEXT_PROGRAM FEXT_HTML FEXT_XML FEXT_CSV _T(".txt.ini.md.inf.json.def.log.nbt.bat.cmd.reg.cbproj.dfm.dsp.dsw.css.hhp.hex.m3u.m3u8.mot.wpl.yml.yaml")
																				//�e�L�X�g�t�@�C��

#define FEXT_NONTEXT	_T(".ani.bin.chm.cur.hlp.ico.lnk.pdf.res.swf") FEXT_APPINFO FEXT_ARCHIVE FEXT_IMAGE FEXT_VIDEO FEXT_MCISND FEXT_FONT
																				//��e�L�X�g�t�@�C��

#define FEXT_ARCVIEW	FEXT_TEXT _T(".ico.cur.wav.mp3")						//�A�[�J�C�u���\��

//---------------------------------------------------------------------------
#define FEXT_C_SRC	_T(".c.cc.cpp.cxx")
#define FEXT_C_HDR	_T(".h.hh.hpp.hxx")
#define FEXT_C_SH	FEXT_C_SRC FEXT_C_HDR
#define FEXT_PASCAL	_T(".pas.dpr.dpk.inc")

#define FEXT_PROGRAM	FEXT_C_SH FEXT_PASCAL _T(".ahk.as.asm.awk.bas.bsh.cgi.cs.d.fs.go.hs.hsp.inc.java.js.jsx.kt.kts.lisp.lsp.lua.mac.php.pl.pm.ps1.psm1.py.qml.rb.rc.drc.rs.sh.swift.scala.sql.st.vb.vbs.v.vhd.PspScript")
															//�v���O����

#define FEXT_HTML	_T(".htm.html.sht.shtm.shtml.hta.xhtml.xht.hhc.hhk.php")
															//HTML����
#define FEXT_XML	_T(".xml.xsd.xsl.xslt.dtd.svg.rdf.manifest.library-ms.cdxml.ps1xml.ptxml.csproj.vbproj.vcxproj.cbproj.xaml")
															//XML

#define FEXT_NRM_REM	FEXT_C_SH _T(".as.cs.css.d.go.idl.java.js.jsx.kt.qml.rc.rs.scala.swift.v")	//�R�����g�� // or /*�`*/
#define FEXT_SMCL_REM	_T(".as.asm.h51.h32.hsp.nbt.qbt")	//�R�����g�� ;

//---------------------------------------------------------------------------
bool test_ExifExt(UnicodeString fext);
bool test_HtmlExt(UnicodeString fext);
bool test_FontExt(UnicodeString fext);
bool test_LibExt(UnicodeString fext);
bool test_ExeExt(UnicodeString fext);
bool test_LnkExt(UnicodeString fext);
bool test_IcoExt(UnicodeString fext);
bool test_CurExt(UnicodeString fext);
bool test_AniExt(UnicodeString fext);
bool test_JpgExt(UnicodeString fext);
bool test_GifExt(UnicodeString fext);
bool test_PngExt(UnicodeString fext);
bool test_PspExt(UnicodeString fext);
bool test_MetaExt(UnicodeString fext);
bool test_MciSndExt(UnicodeString fext);
bool test_Mp3Ext(UnicodeString fext);
bool test_FlacExt(UnicodeString fext);
bool test_NbtExt(UnicodeString fext);
bool test_NwlExt(UnicodeString fext);
bool test_TxtExt(UnicodeString fext);
bool test_AppInfExt(UnicodeString fext);

//---------------------------------------------------------------------------
void add_list_errmsg(TStringList *lst, UnicodeString msg);

UnicodeString get_top_line(UnicodeString fnam, int code_page = 0);

UnicodeString get_id_str4(TFileStream *fs);
bool sea_chunk(TFileStream *fs, const char *s);
UnicodeString get_chunk_hdr(TFileStream *fs, unsigned int *size = NULL);

//---------------------------------------------------------------------------
void get_WavInf(UnicodeString fnam, TStringList *lst);
void get_OpusInf(UnicodeString fnam, TStringList *lst);
bool get_CdaInf(UnicodeString fnam, TStringList *lst, int *len = NULL);
bool get_FlacInf(UnicodeString fnam, TStringList *lst = NULL, int *len = NULL, bool id_min = false);
bool get_FlacImage(UnicodeString fnam, Graphics::TBitmap *bmpimg, UnicodeString o_fnam = EmptyStr);

//---------------------------------------------------------------------------
UnicodeString get_wd_x_hi_str(int w, int h);
UnicodeString get_img_size_str(int w, int h, UnicodeString name = EmptyStr);

void get_JpgExInf(UnicodeString fnam, TStringList *lst);

bool get_ExifInf(UnicodeString fnam, TStringList *lst, int *ori = NULL, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
UnicodeString get_ExifInfStr(UnicodeString fnam, int *ori = NULL, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
UnicodeString get_ExifInfThumb(UnicodeString fnam, int *ori);
bool get_GpsInf(UnicodeString fnam, double *lat, double *lng);

void get_MetafileInf(UnicodeString fnam, TStringList *lst);
bool test_Png(UnicodeString fnam);
bool get_PngInf(UnicodeString fnam, TStringList *lst, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
UnicodeString get_PngInfStr(UnicodeString fnam, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
bool get_GifInf(UnicodeString fnam, TStringList *lst, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL, int *i_cnt = NULL);
bool get_WebpInf(UnicodeString fnam, TStringList *lst, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
UnicodeString get_WebpInfStr(UnicodeString fnam, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);

bool get_PspInf(UnicodeString fnam, TStringList *lst, unsigned int *i_wd = NULL, unsigned int *i_hi = NULL);
bool get_PspThumbnail(UnicodeString fnam, Graphics::TBitmap *bmpimg);

//---------------------------------------------------------------------------
bool test_Icon(UnicodeString fnam, int size = 0);
void get_IconInf(UnicodeString fnam, TStringList* lst, int *ico_n = NULL);
bool get_AniInf(UnicodeString fnam, TStringList *lst);

//---------------------------------------------------------------------------
void get_AppInf(UnicodeString fnam, TStringList *lst, bool check = true);
bool get_DllExpFunc(UnicodeString fnam, TStringList *lst);
void get_BorlandInf(UnicodeString fnam, TStringList *lst);
void get_TagsInf(UnicodeString fnam, TStringList *lst);

//---------------------------------------------------------------------------
void get_PdfVer(UnicodeString fnam, TStringList  *lst);

//---------------------------------------------------------------------------
UnicodeString get_html_header(UnicodeString fnam, int code_page = 0);
void get_HtmlInf(UnicodeString fnam, TStringList *lst);

//---------------------------------------------------------------------------
#define HASH_ID_STR	_T("MD5|SHA1|SHA256|SHA384|SHA512|CRC32")

UnicodeString get_CRC32_str(UnicodeString fnam, bool only_1blk = false, bool prc_msg = false);
UnicodeString get_HashStr(UnicodeString fnam, ALG_ID algid, bool only_1blk = false, bool prc_msg = false);
UnicodeString get_HashStr(UnicodeString fnam, UnicodeString idstr, bool only_1blk = false, bool prc_msg = false);

//---------------------------------------------------------------------------
int  get_duration(UnicodeString fnam);

int  get_filename_warn(UnicodeString fnam, TStringList *lst = NULL, bool is_dir = false);

//---------------------------------------------------------------------------
int get_ADS_count(UnicodeString fnam);
void get_ADS_Inf(UnicodeString fnam, TStringList *lst);

//---------------------------------------------------------------------------
int get_ProcessingInf(UnicodeString fnam, TStringList *lst = NULL, bool only_fnam = false);

//---------------------------------------------------------------------------
#endif
