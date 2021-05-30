//----------------------------------------------------------------------//
// MP3 ID3タグ情報の処理												//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrID3H
#define UsrID3H

//---------------------------------------------------------------------------
//ID3ヘッダ
struct id3_hdr {
	char hdr_id[3];
	BYTE maj_ver;
	BYTE revision;
	BYTE flag;
	BYTE size[4];
};
//拡張ヘッダ
struct id3_ex_hdr {
	BYTE size[4];
	BYTE flag[2];
};

//フレームヘッダ(v2.3,v2.4)
struct id3_frm_hdr {
	char frame_id[4];
	BYTE size[4];
	BYTE flag[2];
};
//フレームヘッダ(v2.2)
struct id3_frm_hdr_2 {
	char frame_id[3];
	BYTE size[3];
};

//---------------------------------------------------------------------------
bool ID3_GetInf(UnicodeString fnam, TStringList* lst, bool id_min = false);
bool ID3_GetImage(UnicodeString fnam, Graphics::TBitmap *bmpimg, UnicodeString o_fnam = EmptyStr);

//---------------------------------------------------------------------------
#endif
