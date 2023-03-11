//----------------------------------------------------------------------//
// NyanFi																//
//  インスペクタ														//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "Global.h"
#include "InspectFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TInspectForm *InspectForm;

//---------------------------------------------------------------------------
__fastcall TInspectForm::TInspectForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::FormCreate(TObject *Sender)
{
	org_InsSttBarWndProc	 = InsStatusBar->WindowProc;
	InsStatusBar->WindowProc = InsSttBarWndProc;

	InspectScrPanel = new UsrScrollPanel(InspectPanel, InspectGrid,  USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);
	CodeScrPanel	= new UsrScrollPanel(CodePanel,    CodePageGrid, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);

	Address  = -1;
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this);

	SetToolWinBorder(this);
	setup_ToolBar(OptToolBar);

	GridPanel->Height		 = IniFile->ReadIntGen(_T("InspectGridHeight"),	200);
	UnsignedAction->Checked  = IniFile->ReadBoolGen(_T("InspectUnsigned"));
	BigEndianAction->Checked = IniFile->ReadBoolGen(_T("InspectBigEndian"));

	setup_StatusBar(InsStatusBar,ViewerFont);

	TStringGrid *gp = InspectGrid;
	InitializeListGrid(gp, ViewerFont);
	InitializeListHeader(InspectHeader, _T("型|値"));
	IniFile->LoadGridColWidth(gp, 2, 180,200);
	set_HeaderFromGrid(gp, InspectHeader);
	set_UsrScrPanel(InspectScrPanel);
	gp->RowCount = 8;
	InspectScrPanel->UpdateKnob();

	gp = CodePageGrid;
	InitializeListGrid(gp, ViewerFont);
	InitializeListHeader(CodePageHeader, _T("文字コード|文字列"));
	IniFile->LoadGridColWidth(gp, 2, 120,280);
	set_HeaderFromGrid(gp, CodePageHeader);
	set_UsrScrPanel(CodeScrPanel);
	gp->RowCount = get_line_count(CodePageList);
	CodeScrPanel->UpdateKnob();

	GridSplitter->Color = col_Splitter;

	OptCheckBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);
	IniFile->SaveGridColWidth(InspectGrid);
	IniFile->SaveGridColWidth(CodePageGrid);

	IniFile->WriteIntGen(_T("InspectGridHeight"),	GridPanel->Height);
	IniFile->WriteBoolGen(_T("InspectUnsigned"),	UnsignedAction);
	IniFile->WriteBoolGen(_T("InspectBigEndian"),	BigEndianAction);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::FormDestroy(TObject *Sender)
{
	delete InspectScrPanel;
	delete CodeScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TInspectForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key==VK_ESCAPE) Close(); else Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TInspectForm::InsStatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = col_bgInfHdr;
	cv->FillRect(Rect);
	cv->Font->Color = col_fgInfHdr;
	cv->TextOut(Rect.Left + ScaledInt(2, this), Rect.Top, Panel->Text);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::InspectHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::InspectHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(InspectGrid, InspectHeader);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::InspectHeaderSectionResize(THeaderControl *HeaderControl,
	THeaderSection *Section)
{
	set_GridFromHeader(InspectHeader, InspectGrid);
	InspectScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//コードページ・ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TInspectForm::CodePageHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::CodePageHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(CodePageGrid, CodePageHeader);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::CodePageHeaderSectionResize(THeaderControl *HeaderControl,
		THeaderSection *Section)
{
	set_GridFromHeader(CodePageHeader, CodePageGrid);
	CodeScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
void __fastcall TInspectForm::OptCheckBoxClick(TObject *Sender)
{
	const char *tit[8] = {
		UnsignedAction->Checked? "unsigned char"  : "char",
		UnsignedAction->Checked? "unsigned short" : "short",
		UnsignedAction->Checked? "unsigned int"   : "int",
		UnsignedAction->Checked? "unsigned int64" : "int64",
		"float",
		"double",
		"synchsafe int BE",
		"COLORREF"
	};

	for (int i=0; i<8; i++) InspectGrid->Cells[0][i] = tit[i];

	UpdateValue();
}

//---------------------------------------------------------------------------
//表示値の更新
//---------------------------------------------------------------------------
void __fastcall TInspectForm::UpdateValue()
{
	if (UnInitializing) return;

	//ヘッダ
	UnicodeString hstr;
	if (Address!=-1) {
		hstr = get_AddrStr(Address);
		UnicodeString astr;
		for (int i=0; i<8; i++) {
			if (i<Bytes.Length) {
				unsigned char c = Bytes[i];
				hstr.cat_sprintf(_T(" %02X"), c);
				if (isprint(c))
					astr.cat_sprintf(_T("%c"), c);
				else
					astr += ".";
			}
			else {
				hstr += "   ";
			}
		}
		hstr.cat_sprintf(_T("  %s"), alt_yen_to(astr).c_str());
	}
	InsStatusBar->Panels->Items[0]->Text = hstr;

	//値
	TStringGrid *gp = InspectGrid;
	for (int i=0; i<gp->RowCount; i++) gp->Cells[1][i] = EmptyStr;

	UnicodeString vstr;
	ColorRef = clBlack;

	//1バイト
	BYTE buf[8];
	if (Bytes.Length>=1) {
		BYTE n = Bytes[0];
		if (UnsignedAction->Checked)
			vstr.sprintf(_T("%u"), (unsigned char)n);
		else
			vstr.sprintf(_T("%d"), (char)n);
		//ビット表示
		vstr += "\t(";
		unsigned int b = 0x0080;
		for (int i=0; i<8; i++,b>>=1) vstr.cat_sprintf(_T("%u"), (n & b)? 1 : 0);
		vstr += ")";
		//ASCII表示
		if (isprint((char)n)) vstr.cat_sprintf(_T(" '%c'"), (char)n);
		gp->Cells[1][0] = vstr;
	}
	//2バイト
	if (Bytes.Length>=2) {
		for (int i=0; i<2; i++) buf[BigEndianAction->Checked? 1 - i : i] = Bytes[i];
		gp->Cells[1][1] = UnsignedAction->Checked ? FormatFloat(",0", *(unsigned short*)&buf[0])
												  : FormatFloat(",0", *(short*)&buf[0]);
	}
	//4バイト
	if (Bytes.Length>=4) {
		for (int i=0; i<4; i++) buf[BigEndianAction->Checked? 3 - i : i] = Bytes[i];
		gp->Cells[1][2] = UnsignedAction->Checked ? FormatFloat(",0", *(unsigned int*)&buf[0])
												  : FormatFloat(",0", *(int*)&buf[0]);
		//float
		float v = *(float*)&buf[0];
		if (IsNan(v)) vstr = "NAN"; else vstr.sprintf(_T("%g"), v);
		gp->Cells[1][4] = vstr;
		//COLORREF
		gp->Cells[1][7] = UnicodeString().sprintf(_T("R:%u, G:%u, B:%u"),buf[0], buf[1], buf[2]);
		ColorRef = TColor(RGB(buf[0], buf[1], buf[2]));
		//Synchsafe int
		for (int i=0; i<4; i++) buf[i] = Bytes[i];
		unsigned int sn =(unsigned int)
			(((buf[0]&0x7f) << 21) + ((buf[1]&0x7f) << 14) + ((buf[2]&0x7f) << 7) + (buf[3] & 0x7f));
		gp->Cells[1][6] = FormatFloat(",0", sn);
	}
	//8バイト
	if (Bytes.Length>=8) {
		for (int i=0; i<8; i++) buf[BigEndianAction->Checked? 7-i : i] = Bytes[i];
		if (UnsignedAction->Checked)
			vstr.sprintf(_T("%Lu"), *(unsigned __int64*)&buf[0]);
		else
			vstr.sprintf(_T("%Ld"), *(__int64*)&buf[0]);
		//int64 にカンマ挿入
		int n = vstr.Length();
		if (StartsStr('-', vstr)) n--;
		for (int j=4,p=2; j<20 && j<=n; j+=3,p+=4) vstr.Insert(",", vstr.Length() - p);
		gp->Cells[1][3] = vstr;

		//double
		double v = *(double*)&buf[0];
		if (IsNan(v)) vstr = "NAN"; else vstr.sprintf(_T("%g"), v);
		gp->Cells[1][5] = vstr;
	}

	//文字コード解析
	gp = CodePageGrid;
	for (int i=0; i<gp->RowCount; i++) gp->Cells[1][i] = EmptyStr;
	if (Bytes.Length>=4) {
		//各種コードページで強制的にデコード
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Text = CodePageList;
		for (int i=0; i<lst->Count; i++) {
			try {
				std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(lst->ValueFromIndex[i].ToIntDef(0)));
				gp->Cells[0][i] = lst->Names[i];
				UnicodeString s = enc->GetString(Bytes, 0, Bytes.Length);
				int n = wcslen(s.c_str());
				if (n>0) s.SetLength(n); else s = EmptyStr;
				gp->Cells[1][i] = s;
			}
			catch (...) {
				;
			}
		}
	}

	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//セルの描画
//---------------------------------------------------------------------------
void __fastcall TInspectForm::InspectGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	int xp = Rect.Left + ScaledInt(4, this);
	int yp = Rect.Top  + get_TopMargin2(cv);
	UnicodeString cellstr = gp->Cells[ACol][ARow];

	//値解析
	if (gp->Tag==0) {
		cv->Brush->Color = col_bgList;
		cv->FillRect(Rect);
		cv->Font->Color = col_fgList;
		//COLORREF
		if (ACol==1 && ARow==7 && !cellstr.IsEmpty()) {
			TRect rc = Rect;
			rc.Right = rc.Left + 40;  InflateRect(rc, -2, -4);
			cv->Brush->Color = ColorRef;	cv->FillRect(rc);
			cv->Brush->Color = clLtGray;	cv->FrameRect(rc);
			cv->Brush->Color = col_bgList;	cv->TextOut(xp + 40, yp, cellstr);
		}
		else {
			cv->TextRect(Rect, xp, yp, split_pre_tab(cellstr));
			if (!cellstr.IsEmpty()) {
				xp += get_CharWidth(cv, 7);		//"99999__";
				cv->TextOut(xp, yp, cellstr);
			}
		}
	}
	//文字解析
	else {
		cv->Brush->Color = State.Contains(gdSelected)? col_selItem : col_bgList;
		cv->FillRect(Rect);
		TabCrTextOut(cellstr, cv, xp, yp, is_SelFgCol(State)? col_fgSelItem : col_fgList);
	}

	//区切り線
	cv->Pen->Width = ScaledInt(1, this);
	cv->Pen->Color = SelectWorB(cv->Brush->Color, 0.3);
	cv->MoveTo(Rect.Right, Rect.Top);  cv->LineTo(Rect.Right, Rect.Bottom);
}
//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TInspectForm::CopyItemClick(TObject *Sender)
{
	UnicodeString cpystr = InsStatusBar->Panels->Items[0]->Text + "\r\n";
	TStringGrid *gp;
	//値
	if (InspectGrid->Focused()) {
		gp = InspectGrid;
		for (int i=0; i<gp->RowCount; i++)
			cpystr.cat_sprintf(_T("%s\t%s\r\n"), gp->Cells[0][i].c_str(), gp->Cells[1][i].c_str());
	}
	//文字
	else if (CodePageGrid->Focused()) {
		gp = CodePageGrid;
		cpystr.cat_sprintf(_T("%s\t%s"), gp->Cells[0][gp->Row].c_str(), gp->Cells[1][gp->Row].c_str());
	}
	copy_to_Clipboard(cpystr);
	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TInspectForm::UnsignedActionExecute(TObject *Sender)
{
	UnsignedAction->Checked = !UnsignedAction->Checked;
	OptCheckBoxClick(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TInspectForm::BigEndianActionExecute(TObject *Sender)
{
	BigEndianAction->Checked = !BigEndianAction->Checked;
	OptCheckBoxClick(NULL);
}
//---------------------------------------------------------------------------

