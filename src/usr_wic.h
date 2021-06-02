//----------------------------------------------------------------------//
// WIC 関連																//
//																		//
//----------------------------------------------------------------------//
#ifndef UserWicH
#define UserWicH

//---------------------------------------------------------------------------
#define WICIMG_FRAME		0
#define WICIMG_PREVIEW		1
#define WICIMG_THUMBNAIL	2

//フレームを優勢的に取得する拡張子
#define FEXT_FRAME_PRI		_T(".bmp.png.gif.tif.wdp.hdp.jpg.jpeg.jpe.webp.heic.eps.psd")

//---------------------------------------------------------------------------
int  WIC_get_ex_list(TStringList *lst, UnicodeString *fxstr = NULL);
bool WIC_get_from_memory(TMemoryStream *ms, Graphics::TBitmap *o_bmp, GUID fmt);
bool WIC_load_image(UnicodeString fnam, Graphics::TBitmap *o_bmp, int img_type = WICIMG_PREVIEW);
bool WIC_save_image(UnicodeString fnam, Graphics::TBitmap *i_bmp,
	int jpg_q = 100, int jpg_ycrcb = 0, bool grayscale = false, int tif_cmp = 0);
bool WIC_resize_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	float ratio, unsigned int s_wd = 0, unsigned int s_hi = 0, int s_opt = 0);
bool WIC_fit_trim_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	unsigned int s_wd, unsigned int s_hi, int s_opt = 0);
bool WIC_rotate_image(Graphics::TBitmap *i_bmp, int rot_opt);
bool WIC_grayscale_image(Graphics::TBitmap *i_bmp);
bool WIC_get_img_size(UnicodeString fnam, unsigned int *wd, unsigned int *hi);

//---------------------------------------------------------------------------

#endif
