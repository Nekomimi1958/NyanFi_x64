//----------------------------------------------------------------------//
// NyanFi																//
//  文字情報															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <Vcl.Clipbrd.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "ChInfFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCharInfoForm *CharInfoForm;

//---------------------------------------------------------------------------
__fastcall TCharInfoForm::TCharInfoForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::FormShow(TObject *Sender)
{
	FontNamePanel->Font->Assign(DialogFont);
	FontNamePanel->Font->Size = 8;
	FontNamePanel->Height = abs(FontNamePanel->Font->Height) + Scaled4;

	Splitter1->MinSize = FontNamePanel->Height * 2;

	IniFile->LoadPosInfo(this);
	SetToolWinBorder(this);

	SamplePanel->Height = std::max(IniFile->ReadIntGen( _T("CharInfoCharHeight"), 180), Splitter1->MinSize);

	TListBox *lp = InfoListBox;
	lp->Color = col_bgInf;
	set_ListBoxItemHi(lp, FileInfFont);

	ClientHeight = SamplePanel->Height + lp->ItemHeight * CHARINF_ITEMCOUNT;
	ClientWidth  = get_CharWidth_Font(lp->Font, 41);	//***
	SamplePanel->Constraints->MaxHeight = ClientWidth;

	Splitter1->Color = col_Splitter;

	SetCharFont();
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("CharInfoCharHeight"),	SamplePanel->Height);
}

//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::Splitter1Moved(TObject *Sender)
{
	SetCharFont();
	ClientHeight = SamplePanel->Height + InfoListBox->ItemHeight * CHARINF_ITEMCOUNT;
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::SetCharFont()
{
	CharPanel->Font->Assign(CharInfFont);
	CharPanel->Font->Height = CharPanel->ClientHeight * 4 / 5;
	CharPanel->Font->Color	= col_fgChInf;
	CharPanel->Color		= col_bgChInf;

	FontNamePanel->Font->Color = col_fgChInf;
	FontNamePanel->Color	   = col_bgChInf;
	FontNamePanel->Caption	   = CharInfFont->Name;
}

//---------------------------------------------------------------------------
//表示を更新
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::UpdateChar(UnicodeString c)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());

	if (!c.IsEmpty()) {
		CharPanel->Caption = SameStr(c, '&')? _T("&&") : c.c_str();

		//欠損、改行のチェック
		bool nc, is_cr;
		if (c.Length()==1) {
			AnsiString    c_a = c;
			UnicodeString c_u = c_a;
			nc	  = (!SameStr(c, c_u) && USAME_TS(c_u, "?"));
			is_cr = (c[1]=='\n');
		}
		else {
			nc	  = true;
			is_cr = false;
		}

		//ShiftJIS
		UnicodeString cd_str, kt_str, tp_str;
		cd_str = " Shift_JIS: ";
		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_sjis(TEncoding::GetEncoding(932));
			DynamicArray<System::Byte> buf_sj = enc_sjis->GetBytes(c);
			for (int i=0; i<buf_sj.Length; i++) cd_str.cat_sprintf(_T("%02X"), buf_sj[i]);
		}
		i_lst->Add(cd_str);

		//JIS
		cd_str = "       JIS: ";
		kt_str = "      区点: ";
		tp_str = "      種類: ";

		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_jis(TEncoding::GetEncoding(50220));
			DynamicArray<System::Byte> byt_jis = enc_jis->GetBytes(c);
			//全角
			if (byt_jis.Length==8) {
				cd_str.cat_sprintf(_T("%02X%02X"), byt_jis[3], byt_jis[4]);
				int k = byt_jis[3] - 32;	//区
				int t = byt_jis[4] - 32;	//点
				kt_str.cat_sprintf(_T("%02u-%02u "), k, t);
				if		(k>=16 && k<=47) tp_str += "第一水準漢字";
				else if (k>=48 && k<=84) tp_str += "第二水準漢字";
				else tp_str += get_word_i_idx(
						_T("|記号|記号|英数字|ひらがな|カタカナ|ギリシア文字|キリル文字|罫線素片"), k);
			}
			else if (byt_jis.Length==9 && byt_jis[3]=='D') {
				cd_str.cat_sprintf(_T("%02X%02X (X0212)"), byt_jis[4], byt_jis[5]);
				tp_str += "補助漢字";
			}
			//半角
			else if (byt_jis.Length==1) {
				unsigned char c1 = byt_jis[0];
				cd_str.cat_sprintf(_T("%02X"), c1);
				if (c1<=0x1f) {
					tp_str += "制御文字";
					if		(c1=='\t') tp_str += "  TAB";
					else if (c1=='\a') tp_str += "  BEL";
					else if (c1=='\f') tp_str += "  FF";
				}
				else if (c1==0x20) {
					tp_str += "半角空白";
				}
				else if (c1>=0x21 && c1<=0x7e) {
					tp_str.cat_sprintf(_T("%s"), isalnum(c1)? _T("半角英数字") : _T("半角記号"));
				}
			}
		}
		//改行
		else if (is_cr) {
			tp_str += "改行";
		}
		//Unicode 制御文字
		else {
			UnicodeString chx;
			DynamicArray<System::Byte> cbt = WideBytesOf(c);
			for (int i=cbt.Length-1; i>=0; i--) chx.cat_sprintf(_T("%02x"), cbt[i]);
			std::unique_ptr<TStringList> clst(new TStringList());
			clst->Text = "200e=LRM\n200f=RLM\n200d=ZWJ\n200c=ZWNJ\n202a=LRE\n"
						 "202b=RLE\n202d=LRO\n202e=RLO\n202c=PDF\n206e=NADS\n"
						 "206f=NODS\n206b=ASS\n206a=ISS\n206d=AAFS\n206c=IAFS\n";
			UnicodeString s = clst->Values[chx];
			if (!s.IsEmpty()) tp_str.cat_sprintf(_T("制御文字 %s"), s.c_str());
		}

		if (c.Length()==2) tp_str += "サロゲートペア";

		i_lst->Add(cd_str);
		i_lst->Add(kt_str);
		i_lst->Add(tp_str);

		//EUC-JP
		cd_str = "    EUC-JP: ";
		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_euc(TEncoding::GetEncoding(20932));
			DynamicArray<System::Byte> buf_euc = enc_euc->GetBytes(c);
			for (int i=0; i<buf_euc.Length; i++) cd_str.cat_sprintf(_T("%02X"), buf_euc[i]);
		}
		i_lst->Add(cd_str);

		//Unicode
		cd_str = "   Unicode: ";
		int uc;
		if (c.Length()==2) {
			//サロゲートペア
			unsigned int ld = (unsigned int)c[1];
			unsigned int tl = (unsigned int)c[2];
			int plane = ((ld - 0xd800) / 0x0040) + 1;
			int ld2 = ld - (0xd800 + 0x0040 * (plane - 1));
			int tl2 = tl - 0xdc00;
			uc = plane * 0x10000 + ld2* 0x100 + ld2* 0x300 + tl2;
			cd_str.cat_sprintf(_T("U+%05X (%04X %04X)"), uc, ld, tl);
		}
		else if (!is_cr) {
			uc = (unsigned int)c[1];
			cd_str.cat_sprintf(_T("U+%04X"), uc);
		}
		i_lst->Add(cd_str);

		//UTF-8
		cd_str = "     UTF-8: ";
		if (!is_cr) {
			std::unique_ptr<TEncoding> enc_u8(TEncoding::GetEncoding(CP_UTF8));
			DynamicArray<System::Byte> buf_u8 = enc_u8->GetBytes(c);
			for (int i=0; i<buf_u8.Length; i++) cd_str.cat_sprintf(_T("%02X "), buf_u8[i]);
		}
		i_lst->Add(cd_str);

		//ブロック名
		cd_str = "  ブロック: " + get_UnicodeBlockName(uc);
		i_lst->Add(cd_str);
	}
	else {
		CharPanel->Caption = EmptyStr;
	}

	assign_InfListBox(InfoListBox, i_lst.get());

	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//情報の描画
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::InfoListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	draw_InfListBox((TListBox*)Control, Rect, Index, State);
}

//---------------------------------------------------------------------------
//情報のコピー
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::CopyItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	i_lst->Add(CharPanel->Caption);
	i_lst->AddStrings(InfoListBox->Items);
	copy_to_Clipboard(i_lst->Text);
}

//---------------------------------------------------------------------------
//フォントの変更
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::ChgFontItemClick(TObject *Sender)
{
	UserModule->FontDlg->Options.Clear();
	UserModule->FontDlg->Options << fdTrueTypeOnly << fdAnsiOnly;
	if (UserModule->FontDlgToFont(CharInfFont)) SetCharFont();
}
//---------------------------------------------------------------------------
//サンプルの配所を設定
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::SetColItemClick(TObject *Sender)
{
	int tag = ((TComponent*)Sender)->Tag;
	UserModule->ColorDlg->Color = (tag==0)? col_bgChInf : col_fgChInf;
	if (UserModule->ColorDlg->Execute()) {
		TColor col = UserModule->ColorDlg->Color;
		((tag==0)? col_bgChInf : col_fgChInf) = col;
		ColorList->Values[(tag==0)? "bgChInf" : "fgChInf"] = IntToStr((int)col);
		SetCharFont();
	}
}
//---------------------------------------------------------------------------

