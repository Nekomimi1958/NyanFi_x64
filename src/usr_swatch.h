//----------------------------------------------------------------------//
// �X�E�H�b�`�u�b�N�E�p�l��												//
//																		//
//----------------------------------------------------------------------//
#ifndef UsrSwatchPanelH
#define UsrSwatchPanelH

//---------------------------------------------------------------------------
#define MAX_SWATCH_COLOR	128
#define SWATCH_FILE	  "Swatchbook.INI"	//�F���{

//---------------------------------------------------------------------------
extern int SWATCH_ColTable[MAX_SWATCH_COLOR];

bool IsSwatchbook(UnicodeString fnam);
bool SaveSwatchbook(UnicodeString fnam);
int  LoadSwatchbook(UnicodeString fnam, bool tmp_sw = false);
void SetCustomColToSwatch(TStrings *lst);
void DrawSwatchbook(TCanvas *cv, TRect rc, bool tmp_sw = false);

//---------------------------------------------------------------------------
class UsrSwatchPanel : public TPanel
{
private:
	TPaintBox *SwatchPaintBox;
	void __fastcall SwatchPaintBoxPaint(TObject *Sender);

public:
	__fastcall UsrSwatchPanel(TComponent* Owner);
	void __fastcall SetPanelSize16x8(int tile_size);
};
//---------------------------------------------------------------------------
#endif
