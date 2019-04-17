//----------------------------------------------------------------------//
// NyanFi																//
//  �������															//
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
	IniFile->LoadPosInfo(this);

	SetToolWinBorder(this);

	TListBox *lp = InfoListBox;
	lp->Color = col_bgInf;
	lp->Font->Assign(FileInfFont);
	set_ListBoxItemHi(lp);
	ClientHeight = CharPanel->Height + FontNamePanel->Height + lp->ItemHeight * 8;
	CharPanel->Color = scl_Window;

	SetCharFont();
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);
}

//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::SetCharFont()
{
	CharPanel->Font->Assign(CharInfFont);
	CharPanel->Font->Height = CharPanel->ClientHeight - 32;
	CharPanel->Font->Color  = scl_WindowText;
	FontNamePanel->Caption  = CharInfFont->Name;
}

//---------------------------------------------------------------------------
//�\�����X�V
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::UpdateChar(UnicodeString c)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());

	if (!c.IsEmpty()) {
		CharPanel->Caption = SameStr(c, '&')? _T("&&") : c.c_str();

		//�����A���s�̃`�F�b�N
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
		cd_str.USET_T(" Shift_JIS: ");
		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_sjis(TEncoding::GetEncoding(932));
			DynamicArray<System::Byte> buf_sj = enc_sjis->GetBytes(c);
			for (int i=0; i<buf_sj.Length; i++) cd_str.cat_sprintf(_T("%02X"), buf_sj[i]);
		}
		i_lst->Add(cd_str);

		//JIS
		cd_str.USET_T("       JIS: ");
		kt_str.USET_T("      ��_: ");
		tp_str.USET_T("      ���: ");

		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_jis(TEncoding::GetEncoding(50220));
			DynamicArray<System::Byte> byt_jis = enc_jis->GetBytes(c);
			//�S�p
			if (byt_jis.Length==8) {
				cd_str.cat_sprintf(_T("%02X%02X"), byt_jis[3], byt_jis[4]);
				int k = byt_jis[3] - 32;	//��
				int t = byt_jis[4] - 32;	//�_
				kt_str.cat_sprintf(_T("%02u-%02u "), k, t);
				if		(k>=16 && k<=47) tp_str.UCAT_T("��ꐅ������");
				else if (k>=48 && k<=84) tp_str.UCAT_T("��񐅏�����");
				else tp_str += get_word_i_idx(
						_T("|�L��|�L��|�p����|�Ђ炪��|�J�^�J�i|�M���V�A����|�L��������|�r���f��"), k);
			}
			else if (byt_jis.Length==9 && byt_jis[3]=='D') {
				cd_str.cat_sprintf(_T("%02X%02X (X0212)"), byt_jis[4], byt_jis[5]);
				tp_str.UCAT_T("�⏕����");
			}
			//���p
			else if (byt_jis.Length==1) {
				unsigned char c1 = byt_jis[0];
				cd_str.cat_sprintf(_T("%02X"), c1);
				if (c1<=0x1f) {
					tp_str.UCAT_T("���䕶��");
					if		(c1=='\t') tp_str.UCAT_T("  TAB");
					else if (c1=='\a') tp_str.UCAT_T("  BEL");
					else if (c1=='\f') tp_str.UCAT_T("  FF");
				}
				else if (c1==0x20) {
					tp_str.UCAT_T("���p��");
				}
				else if (c1>=0x21 && c1<=0x7e) {
					tp_str.cat_sprintf(_T("%s"), isalnum(c1)? _T("���p�p����") : _T("���p�L��"));
				}
			}
		}
		//���s
		else if (is_cr) {
			tp_str.UCAT_T("���s");
		}
		//Unicode ���䕶��
		else {
			UnicodeString chx;
			DynamicArray<System::Byte> cbt = WideBytesOf(c);
			for (int i=cbt.Length-1; i>=0; i--) chx.cat_sprintf(_T("%02x"), cbt[i]);
			std::unique_ptr<TStringList> clst(new TStringList());
			clst->Text = "200e=LRM\n200f=RLM\n200d=ZWJ\n200c=ZWNJ\n202a=LRE\n"
						 "202b=RLE\n202d=LRO\n202e=RLO\n202c=PDF\n206e=NADS\n"
						 "206f=NODS\n206b=ASS\n206a=ISS\n206d=AAFS\n206c=IAFS\n";
			UnicodeString s = clst->Values[chx];
			if (!s.IsEmpty()) tp_str.cat_sprintf(_T("���䕶�� %s"), s.c_str());
		}

		if (c.Length()==2) tp_str.UCAT_T("�T���Q�[�g�y�A");

		i_lst->Add(cd_str);
		i_lst->Add(kt_str);
		i_lst->Add(tp_str);

		//EUC-JP
		cd_str.USET_T("    EUC-JP: ");
		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_euc(TEncoding::GetEncoding(20932));
			DynamicArray<System::Byte> buf_euc = enc_euc->GetBytes(c);
			for (int i=0; i<buf_euc.Length; i++) cd_str.cat_sprintf(_T("%02X"), buf_euc[i]);
		}
		i_lst->Add(cd_str);

		//Unicode
		cd_str.USET_T("   Unicode: ");
		if (c.Length()==2) {
			//�T���Q�[�g�y�A
			unsigned int ld = (unsigned int)c[1];
			unsigned int tl = (unsigned int)c[2];
			int plane = ((ld - 0xd800) / 0x0040) + 1;
			int ld2 = ld - (0xd800 + 0x0040 * (plane - 1));
			int tl2 = tl - 0xdc00;
			int uc	= plane * 0x10000 + ld2* 0x100 + ld2* 0x300 + tl2;
			cd_str.cat_sprintf(_T("U+%05X (%04X %04X)"), uc, ld, tl);
		}
		else if (!is_cr) {
			cd_str.cat_sprintf(_T("U+%04X"), (unsigned int)c[1]);
		}
		i_lst->Add(cd_str);

		//UTF-8
		cd_str.USET_T("     UTF-8: ");
		if (!is_cr) {
			std::unique_ptr<TEncoding> enc_u8(TEncoding::GetEncoding(CP_UTF8));
			DynamicArray<System::Byte> buf_u8 = enc_u8->GetBytes(c);
			for (int i=0; i<buf_u8.Length; i++) cd_str.cat_sprintf(_T("%02X "), buf_u8[i]);
		}
		i_lst->Add(cd_str);

	}
	else {
		CharPanel->Caption = EmptyStr;
	}

	assign_InfListBox(InfoListBox, i_lst.get());

	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//���̕`��
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::InfoListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	draw_InfListBox((TListBox*)Control, Rect, Index, State);
}

//---------------------------------------------------------------------------
//���̃R�s�[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::CopyItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	i_lst->Add(CharPanel->Caption);
	i_lst->AddStrings(InfoListBox->Items);
	copy_to_Clipboard(i_lst->Text);
}
//---------------------------------------------------------------------------
//�t�H���g�̕ύX
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::ChgFontItemClick(TObject *Sender)
{
	UserModule->FontDlg->Options.Clear();
	UserModule->FontDlg->Options << fdTrueTypeOnly << fdAnsiOnly;
	if (UserModule->FontDlgToFont(CharInfFont)) SetCharFont();
}
//---------------------------------------------------------------------------

