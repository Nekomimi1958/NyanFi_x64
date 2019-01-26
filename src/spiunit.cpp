//----------------------------------------------------------------------//
// Susie 32bit Plug-in 操作クラス (x64用ダミー)							//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include "spiunit.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
__fastcall TSpiList::TSpiList(): TList()
{
}

//---------------------------------------------------------------------------
SpiUnit::SpiUnit()
{
	PlgList = new TSpiList();
}

//---------------------------------------------------------------------------
SpiUnit::~SpiUnit()
{
	delete PlgList;
}

//---------------------------------------------------------------------------
bool SpiUnit::TestFExt(UnicodeString fext, bool is_arc)
{
	return false;
}
//---------------------------------------------------------------------------
UnicodeString SpiUnit::GetImgFExt()
{
	return EmptyStr;
}
//---------------------------------------------------------------------------
spi_info* SpiUnit::FindPlugin(UnicodeString fnam, bool is_arc)
{
	return NULL;
}
//---------------------------------------------------------------------------
bool SpiUnit::GetImgSize(UnicodeString fnam, unsigned int *w, unsigned int *h)
{
	return false;
}
//---------------------------------------------------------------------------
bool SpiUnit::LoadImage(UnicodeString fnam, Graphics::TBitmap *bmp, spi_info *sp)
{
	return false;
}
//---------------------------------------------------------------------------
bool SpiUnit::UnPack(UnicodeString arc_file, UnicodeString fnam, TMemoryStream *ms)
{
	return false;
}
//---------------------------------------------------------------------------
bool SpiUnit::UnPack(UnicodeString arc_file, UnicodeString fnam, UnicodeString dst_dir)
{
	return false;
}
//---------------------------------------------------------------------------
