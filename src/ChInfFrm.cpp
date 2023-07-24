//----------------------------------------------------------------------//
// NyanFi																//
//  �������															//
//----------------------------------------------------------------------//
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

	setup_Panel(FontNamePanel, DialogFont);
	FontNamePanel->Visible = IniFile->ReadBoolGen(_T("CharInfoShowFontName"), true);
	Splitter1->MinSize = FontNamePanel->Height * 2;

	SetToolWinBorder(this);
	SamplePanel->Height  = std::max(IniFile->ReadIntGen(_T("CharInfoCharHeight"), 180), Splitter1->MinSize);

	TListBox *lp = InfoListBox;
	lp->Color = col_bgInf;
	set_ListBoxItemHi(lp, FileInfFont);

	SetInfoHeight();
	ClientWidth  = get_CharWidth_Font(lp->Font, 41);	//***
	SamplePanel->Constraints->MaxHeight = ClientWidth;

	Splitter1->Color = col_Splitter;

	SetCharFont();
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::FormHide(TObject *Sender)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("CharInfoCharHeight"),		SamplePanel->Height);
	IniFile->WriteBoolGen(_T("CharInfoShowFontName"),	FontNamePanel->Visible);
}

//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::Splitter1Moved(TObject *Sender)
{
	SetCharFont();
	SetInfoHeight();
}
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::SetInfoHeight()
{
	ClientHeight = SamplePanel->Height + InfoListBox->ItemHeight * (CHARINF_ITEMCOUNT + (!RefInfo.IsEmpty()? 1 : 0));
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
//�\�����X�V
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::UpdateChar(
	UnicodeString c,		//�Ώە���(�T���Q�[�g�y�A�Ή�)
	UnicodeString inf)		//�t�����	(default = EmptyStr)
{
	UniCharName = NumRefHex = NumRefDec = RefInfo = EmptyStr;

	std::unique_ptr<TStringList> i_lst(new TStringList());
	if (!inf.IsEmpty()) {
		RefInfo = inf;
		i_lst->AddObject(RefInfo, (TObject*)(LBFLG_STD_FINF));
	}

	if (!c.IsEmpty()) {
		CharPanel->Caption = SameStr(c, '&')? _T("&&") : c.c_str();

		//�����A���s�̃`�F�b�N
		bool nc, is_cr;
		if (c.Length()==1) {
			nc    = !SameStr(c, UnicodeString(AnsiString(c)));
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
		kt_str = "      ��_: ";
		tp_str = "      ���: ";

		if (!nc && !is_cr) {
			std::unique_ptr<TEncoding> enc_jis(TEncoding::GetEncoding(50220));
			DynamicArray<System::Byte> byt_jis = enc_jis->GetBytes(c);
			//�S�p
			if (byt_jis.Length==8) {
				cd_str.cat_sprintf(_T("%02X%02X"), byt_jis[3], byt_jis[4]);
				int k = byt_jis[3] - 32;	//��
				int t = byt_jis[4] - 32;	//�_
				kt_str.cat_sprintf(_T("%02u-%02u "), k, t);
				if		(k>=16 && k<=47) tp_str += "��ꐅ������";
				else if (k>=48 && k<=84) tp_str += "��񐅏�����";
				else tp_str += get_word_i_idx(
						_T("|�L��|�L��|�p����|�Ђ炪��|�J�^�J�i|�M���V�A����|�L��������|�r���f��"), k);
			}
			else if (byt_jis.Length==9 && byt_jis[3]=='D') {
				cd_str.cat_sprintf(_T("%02X%02X (X0212)"), byt_jis[4], byt_jis[5]);
				tp_str += "�⏕����";
			}
			//���p
			else if (byt_jis.Length==1) {
				unsigned char c1 = byt_jis[0];
				cd_str.cat_sprintf(_T("%02X"), c1);
				if (c1<=0x1f) {
					tp_str += "���䕶��";
					if		(c1=='\t') tp_str += "  TAB";
					else if (c1=='\a') tp_str += "  BEL";
					else if (c1=='\f') tp_str += "  FF";
				}
				else if (c1==0x20) {
					tp_str += "���p��";
				}
				else if (c1>=0x21 && c1<=0x7e) {
					tp_str.cat_sprintf(_T("%s"), isalnum(c1)? _T("���p�p����") : _T("���p�L��"));
				}
			}
		}
		//���s
		else if (is_cr) {
			tp_str += "���s";
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

		if (c.Length()==2) tp_str += "�T���Q�[�g�y�A";

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
		if (!is_cr) {
			if (c.Length()==2) {
				//�T���Q�[�g�y�A
				uc = SurrogateToUnicodePoint(c);
				cd_str.cat_sprintf(_T("U+%05X (%04X %04X)"), uc, c[1], c[2]);
				UniCharName.sprintf(_T("\\U%08X"), uc);
			}
			else {
				uc = (unsigned int)c[1];
				cd_str.cat_sprintf(_T("U+%04X"), uc);
				UniCharName.sprintf(_T("\\u%04X"), uc);
			}
			NumRefHex.sprintf(_T("&#x%X;"), uc);
			NumRefDec.sprintf(_T("&#%u;"), uc);
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

		//�u���b�N��
		cd_str = "  �u���b�N: " + get_UnicodeBlockName(uc);
		i_lst->Add(cd_str);
	}
	else {
		CharPanel->Caption = EmptyStr;
	}

	SetInfoHeight();
	assign_InfListBox(InfoListBox, i_lst.get());

	Application->MainForm->SetFocus();
}

//---------------------------------------------------------------------------
//���̕`��
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::InfoListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	draw_InfListBox(lp, Rect, Index, State);
	if (Index==0 && ((int)lp->Items->Objects[Index] & LBFLG_STD_FINF)) draw_separateLine(lp->Canvas, Rect, 2);
}

//---------------------------------------------------------------------------
//�E�N���b�N���j���[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::PopupMenu1Popup(TObject *Sender)
{
	CopyUniChNameItem->Enabled = !UniCharName.IsEmpty();
	NumRefDecItem->Enabled	   = !NumRefDec.IsEmpty();
	NumRefHexItem->Enabled	   = !NumRefHex.IsEmpty();
	FontNameItem->Checked	   = FontNamePanel->Visible;
}

//---------------------------------------------------------------------------
//�����R�s�[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::CopyItemClick(TObject *Sender)
{
	std::unique_ptr<TStringList> i_lst(new TStringList());
	i_lst->Add(CharPanel->Caption);
	i_lst->AddStrings(InfoListBox->Items);
	copy_to_Clipboard(i_lst->Text);
}
//---------------------------------------------------------------------------
//���j�o�[�T�����������R�s�[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::CopyUniChNameItemClick(TObject *Sender)
{
	copy_to_Clipboard(UniCharName);
}
//---------------------------------------------------------------------------
//���l�����Q��(10�i��)���R�s�[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::NumRefDecItemClick(TObject *Sender)
{
	copy_to_Clipboard(NumRefDec);
}
//---------------------------------------------------------------------------
//���l�����Q��(16�i��)���R�s�[
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::NumRefHexItemClick(TObject *Sender)
{
	copy_to_Clipboard(NumRefHex);
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
//�T���v���̔z�F��ݒ�
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
//�t�H���g����\��
//---------------------------------------------------------------------------
void __fastcall TCharInfoForm::FontNameItemClick(TObject *Sender)
{
	FontNamePanel->Visible = !FontNamePanel->Visible;
	SetCharFont();
}
//---------------------------------------------------------------------------
