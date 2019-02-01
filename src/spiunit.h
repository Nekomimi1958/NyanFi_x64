//----------------------------------------------------------------------//
// Susie 32bit Plug-in 操作クラス										//
//																		//
//----------------------------------------------------------------------//
#ifndef SpiUnitH
#define SpiUnitH

//---------------------------------------------------------------------------
struct spi_info {
	bool is_Arc;
	UnicodeString VerType;	//"00IN" or "00AM"
	UnicodeString FileName;
	UnicodeString FileInfo;
	UnicodeString FileExt;
	UnicodeString FileType;
};

//---------------------------------------------------------------------------
//SPIリスト (TList から継承)
//---------------------------------------------------------------------------
class TSpiList : public TList
{
private:
	spi_info * __fastcall Get(int Index)
	{
		return (spi_info*)(TList::Get(Index));
	}
	void __fastcall Put(int Index, spi_info *Item)
	{
		TList::Put(Index, Item);
	}

public:
	__fastcall TSpiList();
	__property spi_info * Items[int Index] = {read=Get, write=Put};
};

//---------------------------------------------------------------------------
class SpiUnit
{
public:
	UnicodeString SpiDir;
	TSpiList *PlgList;
	UnicodeString ErrMsg;

	SpiUnit(UnicodeString spidir);
	~SpiUnit();

	bool TestFExt(UnicodeString fext, bool is_arc = false);
	UnicodeString GetImgFExt();
	bool GetImgSize(UnicodeString fnam, unsigned int *w, unsigned int *h);
	spi_info* FindPlugin(UnicodeString fnam, bool is_arc = false);
	bool LoadImage(UnicodeString fnam, Graphics::TBitmap *bmp, spi_info *sp = NULL);

	bool UnPack(UnicodeString arc_file, UnicodeString fnam, TMemoryStream *ms);
	bool UnPack(UnicodeString arc_file, UnicodeString fnam, UnicodeString dst_dir);
};
//---------------------------------------------------------------------------
#endif
