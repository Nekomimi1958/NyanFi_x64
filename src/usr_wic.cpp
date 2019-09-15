//----------------------------------------------------------------------//
// WIC �֘A																//
//																		//
// �I�X���b�h���� CoInitialize/CoUninitialize ���K�v					//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <utilcls.h>
#include <VersionHelpers.h>
#include <System.StrUtils.hpp>
#include <System.Win.Registry.hpp>
#include "usr_file_ex.h"
#include "usr_wic.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//WIC �g���R�[�f�b�N�ꗗ�̎擾
//---------------------------------------------------------------------------
int WIC_get_ex_list(
	TStringList *lst,		//���ꗗ
	UnicodeString *fxstr)	//�g���q	(default = NULL);
{
	int cnt = 0;
	std::unique_ptr<TRegistry> reg(new TRegistry());
	reg->RootKey = HKEY_CLASSES_ROOT;
	if (reg->OpenKeyReadOnly("CLSID\\{7ED96837-96F0-4812-B211-F13C24117ED3}\\Instance")) {
		UnicodeString key = reg->CurrentPath;
		UnicodeString tmp;

		std::unique_ptr<TStringList> id_lst(new TStringList());
		reg->GetKeyNames(id_lst.get());
		reg->CloseKey();
		for (int i=0; i<id_lst->Count; i++) {
			if (reg->OpenKeyReadOnly(tmp.sprintf(_T("%s\\%s"), key.c_str(), id_lst->Strings[i].c_str()))) {
				id_lst->Strings[i] = reg->ReadString("CLSID");
				reg->CloseKey();
			}
		}

		std::unique_ptr<TStringList> x_lst(new TStringList());	//�g���q���X�g
		for (int i=0; i<id_lst->Count; i++) {
			if (reg->OpenKeyReadOnly(tmp.sprintf(_T("CLSID\\%s"), id_lst->Strings[i].c_str()))) {
				UnicodeString xstr = reg->ReadString("FileExtensions").LowerCase();
				x_lst->AddStrings(SplitString(xstr, ","));
				if (lst) {
					lst->Add(xstr);
					lst->Add("  " + reg->ReadString("FriendlyName"));
					lst->Add("  " + id_lst->Strings[i]);
					lst->Add(EmptyStr);
				}
				reg->CloseKey();
				cnt++;
			}
		}

		if (fxstr && x_lst->Count>0) {
			x_lst->Sort();
			UnicodeString s = x_lst->Strings[0];
			*fxstr = s;
			int i = 1;
			while (i < x_lst->Count) {
				if (SameStr(s, x_lst->Strings[i])) {
					x_lst->Delete(i);	//�d�����폜
				}
				else {
					s = x_lst->Strings[i++];
					*fxstr += s;
				}
			}
			//PDF �͏��O
			*fxstr = ReplaceStr(*fxstr, ".pdf", EmptyStr);
		}

	}
	return cnt;
}

//---------------------------------------------------------------------------
//���������e���w��`���Ńf�R�[�h���ăr�b�g�}�b�v���擾
//---------------------------------------------------------------------------
bool WIC_get_from_memory(TMemoryStream *ms, Graphics::TBitmap *o_bmp, GUID fmt)
{
	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICStream>				stream;
		TComInterface<IWICBitmapDecoder>		decoder;
		TComInterface<IWICFormatConverter>		converter;
		TComInterface<IWICBitmapFrameDecode>	frame;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		if (FAILED(factory->CreateStream(&stream))) Abort();
		if (FAILED(stream->InitializeFromMemory(reinterpret_cast<BYTE*>(ms->Memory), ms->Size))) Abort();

		if (FAILED(factory->CreateDecoderFromStream(stream, &fmt,
			WICDecodeMetadataCacheOnLoad, &decoder)))
				Abort();

		if (FAILED(decoder->GetFrame( 0, &frame))) Abort();
		if (FAILED(factory->CreateFormatConverter(&converter))) Abort();

		if (FAILED(converter->Initialize(frame,
			GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
				Abort();

		//�T�C�Y���擾
		UINT i_wd, i_hi;
		if (FAILED(converter->GetSize(&i_wd, &i_hi))) Abort();
		//�C���[�W���������ɃR�s�[
		UINT stride = i_wd * 3;
		std::unique_ptr<BYTE[]> ibuf(new BYTE[stride * i_hi]);
		if (FAILED(converter->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
		//����������r�b�g�}�b�v�ɃR�s�[
		o_bmp->PixelFormat = pf24bit;
		o_bmp->SetSize(i_wd, i_hi);
		BYTE *bp = ibuf.get();
		for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(o_bmp->ScanLine[i], bp, stride);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�t�@�C������w�肵����ނ̃r�b�g�}�b�v���擾
//---------------------------------------------------------------------------
bool WIC_load_image(UnicodeString fnam, Graphics::TBitmap *o_bmp,
	int img_type)	//default = WICIMG_PREVIEW
{
	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICBitmapDecoder>		decoder;
		TComInterface<IWICFormatConverter>		converter;
		TComInterface<IWICBitmapFrameDecode>	frame;
		TComInterface<IWICBitmapSource> 		preview;
		TComInterface<IWICBitmapSource>			thumbnail;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		if (FAILED(factory->CreateDecoderFromFilename(fnam.c_str(), NULL,
			GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
				Abort();

		if (FAILED(factory->CreateFormatConverter(&converter))) Abort();

		if (test_FileExt(get_extension(fnam), FEXT_FRAME_PRI) && img_type==WICIMG_PREVIEW)
			img_type = WICIMG_FRAME;

		for (int i=0; i<3; i++) {
			HRESULT res = S_FALSE;

			switch (img_type) {
			case WICIMG_THUMBNAIL: res = decoder->GetThumbnail(&thumbnail);	break;
			case WICIMG_PREVIEW:   res = decoder->GetPreview(&preview);		break;
			case WICIMG_FRAME:	   res = decoder->GetFrame(0, &frame);		break;
			default:			   Abort();
			}

			if (res==S_OK) {
				switch (img_type) {
				case WICIMG_THUMBNAIL:
					if (FAILED(converter->Initialize(thumbnail,
						GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
							Abort();
					break;
				case WICIMG_PREVIEW:
					if (FAILED(converter->Initialize(preview,
						GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
							Abort();
					break;
				default:
					if (FAILED(converter->Initialize(frame,
						GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
							Abort();
				}
				break;
			}
			else {
				switch (img_type) {
				case WICIMG_THUMBNAIL:	img_type = WICIMG_PREVIEW;	break;
				case WICIMG_PREVIEW:	img_type = WICIMG_FRAME;	break;
				default:				Abort();
				}
			}
		}

		//�T�C�Y���擾
		UINT i_wd, i_hi;
		if (FAILED(converter->GetSize(&i_wd, &i_hi))) Abort();
		//�C���[�W���������ɃR�s�[
		UINT stride = i_wd * 3;
		std::unique_ptr<BYTE[]> ibuf(new BYTE[stride * i_hi]);
		if (FAILED(converter->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
		//����������r�b�g�}�b�v�ɃR�s�[
		o_bmp->PixelFormat = pf24bit;
		o_bmp->SetSize(i_wd, i_hi);
		BYTE *bp = ibuf.get();
		for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(o_bmp->ScanLine[i], bp, stride);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�r�b�g�}�b�v���w��`���Ńt�@�C���ɕۑ�
//---------------------------------------------------------------------------
bool WIC_save_image(
	UnicodeString fnam,			//�t�@�C����(�g���q����`����ݒ�)
	Graphics::TBitmap *i_bmp,	//�r�b�g�}�b�v
								//�I�O���[�ۑ�����ꍇ�AWIC_grayscale_image �ŃO���[�����Ă���
	int  jpg_q,					//jpeg �掿				(0�`100 default = 100)
	int  jpg_ycrcb,				//jpeg �T�u�T���v�����O	(0�`3	default = 0)
	bool grayscale,				//jpeg/PNG �O���[�X�P�[��(8bit)�ŕۑ�
	int  tif_cmp)				//tiff ���k				(0�`7   default = 0)
{
	UnicodeString fext = ExtractFileExt(fnam) + ".";
	GUID fmt;
	if		(ContainsText(".BMP.", fext))			fmt = GUID_ContainerFormatBmp;
	else if (ContainsText(".JPG.JPEG.JPE.", fext))	fmt = GUID_ContainerFormatJpeg;
	else if (ContainsText(".PNG.", fext))			fmt = GUID_ContainerFormatPng;
	else if (ContainsText(".GIF.", fext))			fmt = GUID_ContainerFormatGif;
	else if (ContainsText(".TIF.", fext))			fmt = GUID_ContainerFormatTiff;
	else if (ContainsText(".HDP.WDP.", fext))		fmt = GUID_ContainerFormatWmp;
	else return false;

	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICStream>				stream;
		TComInterface<IWICBitmapEncoder>		encoder;
		TComInterface<IWICBitmap>				bitmap;
		TComInterface<IWICBitmapFrameEncode>	frame;
		TComInterface<IPropertyBag2>			properties;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		if (FAILED(factory->CreateStream(&stream)))							Abort();
		if (FAILED(stream->InitializeFromFilename(fnam.c_str(), GENERIC_WRITE))) Abort();
		if (FAILED(factory->CreateEncoder(fmt, NULL, &encoder)))			Abort();
		if (FAILED(encoder->Initialize(stream, WICBitmapEncoderNoCache)))	Abort();
		if (FAILED(encoder->CreateNewFrame(&frame, &properties)))			Abort();

		//�I�v�V�����ݒ�
		PROPBAG2 option = { 0 };
		VARIANT varValue;
		//Jpeg�AHDP: �i��
		if (fmt==GUID_ContainerFormatJpeg || fmt==GUID_ContainerFormatWmp) {
			option.pstrName = UnicodeString("ImageQuality").c_str();
			VariantInit(&varValue);
			varValue.vt 	= VT_R4;
			varValue.fltVal = (float)((jpg_q<100)? jpg_q/100.0 : 1.0);
			if (FAILED(properties->Write(1, &option, &varValue))) Abort();
		}
		//JpegP: YCrCb �T�u�T���v�����O
		if (fmt==GUID_ContainerFormatJpeg && jpg_ycrcb>=0 && jpg_ycrcb<=3) {
			option.pstrName = UnicodeString("JpegYCrCbSubsampling").c_str();
			VariantInit(&varValue);
			varValue.vt 	= VT_UI1;
			varValue.bVal	= jpg_ycrcb;
			if (FAILED(properties->Write(1, &option, &varValue))) Abort();
		}
		//Tiff: ���k���@
		if (fmt==GUID_ContainerFormatTiff && tif_cmp>=0 && tif_cmp<=7) {
			option.pstrName = UnicodeString("TiffCompressionMethod").c_str();
			VariantInit(&varValue);
			varValue.vt 	= VT_UI1;
			varValue.bVal	= tif_cmp;
			if (FAILED(properties->Write(1, &option, &varValue))) Abort();
		}

		if (FAILED(frame->Initialize(properties)))	Abort();

		//�\�[�X����r�b�g�}�b�v���쐬
		if (FAILED(factory->CreateBitmapFromHBITMAP(
			i_bmp->Handle, NULL, WICBitmapIgnoreAlpha, &bitmap)))
				Abort();

		if ((fmt==GUID_ContainerFormatJpeg || fmt==GUID_ContainerFormatPng) && grayscale) {
			WICPixelFormatGUID pPixelFormat = GUID_WICPixelFormat8bppGray;
			if (FAILED(frame->SetPixelFormat(&pPixelFormat)))	Abort();
		}
		if (FAILED(frame->WriteSource(bitmap, 0)))	Abort();
		if (FAILED(frame->Commit()))				Abort();
		if (FAILED(encoder->Commit()))				Abort();
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�摜�̏k���E�g��
//---------------------------------------------------------------------------
bool WIC_resize_image(
	Graphics::TBitmap *i_bmp,	//���̓r�b�g�}�b�v
	Graphics::TBitmap *o_bmp,	//�o�̓r�b�g�}�b�v
	float ratio,				//�{��		0.0 : ����
	unsigned int s_wd,			//���T�C�Y	  0 : ����
	unsigned int s_hi,			//�c�T�C�Y
								//  �{�����T�C�Y�̂����ꂩ���w��
	int s_opt)					//�A���S���Y��
{
	if (ratio<=0.0 && (s_wd==0 || s_hi==0)) return false;

	try {
		TComInterface<IWICImagingFactory>	factory;
		TComInterface<IWICBitmap>			bitmap;
		TComInterface<IWICBitmapScaler> 	scaler;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		//�\�[�X����r�b�g�}�b�v���쐬
		i_bmp->PixelFormat = pf24bit;
		if (FAILED(factory->CreateBitmapFromHBITMAP(
			i_bmp->Handle, NULL, WICBitmapIgnoreAlpha, &bitmap)))
				Abort();

		if (FAILED(factory->CreateBitmapScaler(&scaler))) Abort();

		//���T�C�Y
		UINT i_wd, i_hi;
		if (ratio>0) {
			i_wd = i_bmp->Width  * ratio;
			i_hi = i_bmp->Height * ratio;
		}
		else {
			i_wd = s_wd;
			i_hi = s_hi;
		}

		WICBitmapInterpolationMode opt;
		switch (s_opt) {
		case 1:  opt = WICBitmapInterpolationModeLinear; break;				//�o�C���j�A
		case 2:  opt = WICBitmapInterpolationModeCubic;	 break;				//�o�C�L���[�r�b�N
		case 3:  opt = WICBitmapInterpolationModeFant;	 break;				//�t�@���g���T���v�����O
		case 4:  opt = WICBitmapInterpolationModeHighQualityCubic; break;	//���i���o�C�L���[�r�b�N
		default: opt = WICBitmapInterpolationModeNearestNeighbor;			//�ŋߖT�@
		}

		if (FAILED(scaler->Initialize(bitmap, i_wd, i_hi, opt))) Abort();

		//�C���[�W���������ɃR�s�[
		UINT stride = i_wd * 3;
		std::unique_ptr<BYTE[]> ibuf(new BYTE[stride * i_hi]);
		if (FAILED(scaler->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
		//����������r�b�g�}�b�v�ɃR�s�[
		o_bmp->PixelFormat = pf24bit;
		o_bmp->SetSize(i_wd, i_hi);
		BYTE *bp = ibuf.get();
		for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(o_bmp->ScanLine[i], bp, stride);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�w��T�C�Y�Ɏ��܂�悤�ɏk���E�g��A�؂�o��
//---------------------------------------------------------------------------
bool WIC_fit_trim_image(Graphics::TBitmap *i_bmp, Graphics::TBitmap *o_bmp,
	unsigned int s_wd,		//���T�C�Y
	unsigned int s_hi,		//�c�T�C�Y
	int s_opt)				//�A���S���Y��
{
	if (i_bmp->Empty || s_hi==0) return false;

	float r0 = 1.0 * i_bmp->Width / i_bmp->Height;
	float r1 = 1.0 * s_wd / s_hi;
	int i_wd, i_hi;
	if (r0<r1) {
		float r = 1.0 * i_bmp->Height / i_bmp->Width;
		i_wd = s_wd;
		i_hi = i_wd * r;
	}
	else {
		float r = 1.0 * i_bmp->Width / i_bmp->Height;
		i_hi = s_hi;
		i_wd = i_hi * r;
	}
	int xp = (i_wd - s_wd)/2;
	int yp = (i_hi - s_hi)/2;
	std::unique_ptr<Graphics::TBitmap> r_bmp(new Graphics::TBitmap());
	r_bmp->SetSize(i_wd, i_hi);
	if (WIC_resize_image(i_bmp, r_bmp.get(), 0.0, i_wd, i_hi, s_opt)) {
		o_bmp->Canvas->CopyRect(Rect(0, 0, s_wd, s_hi), r_bmp->Canvas, Rect(xp, yp, xp + s_wd, yp + s_hi));
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//�摜�̉�]�E���]
//---------------------------------------------------------------------------
bool WIC_rotate_image(Graphics::TBitmap *i_bmp,
	int rot_opt)	//1:90/ 2:180/ 3:270(����90)/ 4:���E���]/ 5:�㉺���]
{
	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICBitmap>				bitmap;
		TComInterface<IWICBitmapFlipRotator> 	rotator;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		//�\�[�X����r�b�g�}�b�v���쐬
		i_bmp->PixelFormat = pf24bit;
		if (FAILED(factory->CreateBitmapFromHBITMAP(i_bmp->Handle, NULL,
			WICBitmapIgnoreAlpha, &bitmap)))
				Abort();

		//��]
		if (FAILED(factory->CreateBitmapFlipRotator(&rotator))) Abort();
		WICBitmapTransformOptions opt;
		switch (rot_opt) {
		case  1: opt = WICBitmapTransformRotate90;	break;
		case  2: opt = WICBitmapTransformRotate180;	break;
		case  3: opt = WICBitmapTransformRotate270;	break;
		case  4: opt = WICBitmapTransformFlipHorizontal;	break;
		case  5: opt = WICBitmapTransformFlipVertical;		break;
		default: opt = WICBitmapTransformRotate0;	break;
		}
		if (FAILED(rotator->Initialize(bitmap, opt))) Abort();

		//�T�C�Y���擾
		UINT i_wd, i_hi;
		if (FAILED(rotator->GetSize(&i_wd, &i_hi))) Abort();
		//�C���[�W���������ɃR�s�[
		UINT stride = i_wd * 3;
		std::unique_ptr<BYTE[]> ibuf(new BYTE[stride * i_hi]);
		if (FAILED(rotator->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
		//����������r�b�g�}�b�v�ɃR�s�[
		i_bmp->PixelFormat = pf24bit;
		i_bmp->SetSize(i_wd, i_hi);
		BYTE *bp = ibuf.get();
		for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(i_bmp->ScanLine[i], bp, stride);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�O���[�X�P�[����
//---------------------------------------------------------------------------
bool WIC_grayscale_image(Graphics::TBitmap *i_bmp)
{
	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICBitmap>				bitmap;
		TComInterface<IWICFormatConverter>		g_converter;
		TComInterface<IWICFormatConverter>		o_converter;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		//�\�[�X����r�b�g�}�b�v���쐬
		if (FAILED(factory->CreateBitmapFromHBITMAP(i_bmp->Handle, NULL, WICBitmapIgnoreAlpha, &bitmap)))
			Abort();
		//�O���[�X�P�[���ɕϊ�
		if (FAILED(factory->CreateFormatConverter(&g_converter))) Abort();
		if (FAILED(g_converter->Initialize(bitmap,
			GUID_WICPixelFormat8bppGray, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
				Abort();
		//24bit BGR �ɕϊ�
		if (FAILED(factory->CreateFormatConverter(&o_converter))) Abort();
		if (FAILED(o_converter->Initialize(g_converter,
			GUID_WICPixelFormat24bppBGR, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom)))
				Abort();

		//�T�C�Y���擾
		UINT i_wd, i_hi;
		if (FAILED(o_converter->GetSize(&i_wd, &i_hi))) Abort();
		//�C���[�W���������ɃR�s�[
		UINT stride = i_wd * 3;
		std::unique_ptr<BYTE[]> ibuf(new BYTE[stride * i_hi]);
		if (FAILED(o_converter->CopyPixels(NULL, stride, stride*i_hi, ibuf.get()))) Abort();
		//����������r�b�g�}�b�v�ɃR�s�[
		i_bmp->PixelFormat = pf24bit;
		i_bmp->SetSize(i_wd, i_hi);
		BYTE *bp = ibuf.get();
		for (unsigned int i=0; i<i_hi; i++, bp+=stride) ::CopyMemory(i_bmp->ScanLine[i], bp, stride);
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�摜�̃T�C�Y���擾
//---------------------------------------------------------------------------
bool WIC_get_img_size(UnicodeString fnam, unsigned int *wd, unsigned int *hi)
{
	try {
		TComInterface<IWICImagingFactory>		factory;
		TComInterface<IWICBitmapDecoder>		decoder;
		TComInterface<IWICBitmapFrameDecode>	frame;

		if (FAILED(::CoCreateInstance(CLSID_WICImagingFactory, NULL,
			CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory)))
				Abort();

		if (FAILED(factory->CreateDecoderFromFilename(fnam.c_str(), NULL,
			GENERIC_READ, WICDecodeMetadataCacheOnDemand, &decoder)))
				Abort();

		if (FAILED(decoder->GetFrame(0, &frame)))	Abort();
		if (FAILED(frame->GetSize(wd, hi)))			Abort();
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
