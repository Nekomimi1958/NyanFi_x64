//----------------------------------------------------------------------//
// 正規表現チェッカー													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <System.StrUtils.hpp>
#include <RegularExpressions.hpp>
#include <Vcl.Clipbrd.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "RegExChk.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegExChecker *RegExChecker = NULL;

//---------------------------------------------------------------------------
__fastcall TRegExChecker::TRegExChecker(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	MatchWdList = new TStringList();

	//正規表現の説明を設定
	TStringList *lst = UserModule->RefRegExList;
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (!lbuf.IsEmpty()) ReferListBox->Items->Add((lbuf[1]=='-')? EmptyStr : lbuf);
	}
	//サンプルを追加
	std::unique_ptr<TStringList> s_lst(new TStringList());
	s_lst->Text =
		"\n\n【 サンプル 】\n"
		"日付\t[12]\\d{3}/(0?[1-9]|1[0-2])/([12][0-9]|3[01]|0?[1-9])\n"
		"時刻\t([01]?[0-9]|2[0-3])(:[0-5][0-9]){1,2}\n"
		"URL\thttps?://[\\w/:%#$&?()~.=+-]+\n"
		"偶数\t\\d*[02468]\n"
		"奇数\t\\d*[13579]\n"
		"全角\t[^ -~｡-ﾟ\\t]+\n"
		"半角ｶﾅ\t[｡-ﾟ]+";

	ReferListBox->Items->AddStrings(s_lst.get());

	PtnComboBox->Tag = CBTAG_HISTORY;

	ObjComboBox   = NULL;
	ObjCustomEdit = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	IniFile->LoadPosInfo(this, DialogCenter);

	MainPanel->Width	  = IniFile->ReadIntGen( _T("RegExMainWidth"),	MainPanel->Constraints->MinWidth);
	OpePanel->Height	  = IniFile->ReadIntGen( _T("RegExOpeHeight"),	OpePanel->Constraints->MinHeight);
	ReplaceEdit->Text	  = IniFile->ReadStrGen( _T("RegExChkRepStr"));
	CaseCheckBox->Checked = IniFile->ReadBoolGen(_T("RegExChkCase"),	false);
	UpdtCheckBox->Checked = IniFile->ReadBoolGen(_T("RegExChkUpdate"),	false);

	//フォントの設定
	std::unique_ptr<TFont> ttFont(new TFont());
	ttFont->Assign(ViewerFont);
	ttFont->Size = Font->Size;
	ObjMemo->Font->Assign(ttFont.get());
	set_ListBoxItemHi(ResListBox,	ttFont.get());
	set_ListBoxItemHi(ReferListBox,	ttFont.get());

	//検索対象の設定
	ObjMemo->Clear();
	bool ok = false;
	FileEdit->Text = IniFile->ReadStrGen(_T("RegExChkFile"));
	UnicodeString fnam = rel_to_absdir(FileEdit->Text);
	if (file_exists(fnam)) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		if (load_text_ex(fnam, fbuf.get())) {
			ObjMemo->Text = fbuf->Text;
			ok = true;
		}
		else msgbox_ERR(USTR_FileNotOpen);
	}

	if (!ok) {
		TStringDynArray ibuf = get_csv_array(IniFile->ReadStrGen(_T("RegExChkText"), EmptyStr, false), 20);
		for (int i=0; i<ibuf.Length; i++) ObjMemo->Lines->Add(ibuf[i]);
	}

	IniFile->LoadComboBoxItems(PtnComboBox, _T("RegExChkHistory"));
	if (PtnComboBox->Items->Count>0) PtnComboBox->ItemIndex = 0;

	with_Slash = is_regex_slash(PatternStr);
	if (!PatternStr.IsEmpty()) {
		if (with_Slash) PatternStr = exclude_top_end(PatternStr);
		PtnComboBox->Text = PatternStr;
	}

	PtnSelStart  = PtnComboBox->SelStart;
	PtnSelLength = PtnComboBox->SelLength;

	ResultLabel->Caption = "結果";
	ResListBox->Clear();
	MatchWdList->Clear();
	MatchCount = 0;
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::FormClose(TObject *Sender, TCloseAction &Action)
{
	ObjComboBox   = NULL;
	ObjCustomEdit = NULL;
	PatternStr	  = EmptyStr;

	IniFile->SavePosInfo(this);

	//検索対象の保存
	UnicodeString lbuf;
	for (int i=0; i<ObjMemo->Lines->Count && i<20; i++) {
		if (i>0) lbuf.UCAT_T(",");
		lbuf += make_csv_str(ObjMemo->Lines->Strings[i]);
	}
	IniFile->WriteStrGen( _T("RegExChkText"),	lbuf);
	//検索対象ファイルの更新
	IniFile->WriteStrGen( _T("RegExChkFile"),	FileEdit->Text);
	if (!FileEdit->Text.IsEmpty() && UpdtCheckBox->Checked && ObjMemo->Modified) {
		UnicodeString fnam = rel_to_absdir(FileEdit->Text);
		if (!saveto_TextFile(fnam, ObjMemo->Lines)) msgbox_ERR(USTR_FaildSave);
	}

	IniFile->WriteIntGen( _T("RegExMainWidth"),	MainPanel->Width);
	IniFile->WriteIntGen( _T("RegExOpeHeight"),	OpePanel->Height);
	IniFile->WriteStrGen( _T("RegExChkRepStr"),	ReplaceEdit->Text);
	IniFile->WriteBoolGen(_T("RegExChkCase"),	CaseCheckBox->Checked);
	IniFile->WriteBoolGen(_T("RegExChkUpdate"),	UpdtCheckBox->Checked);

	IniFile->SaveComboBoxItems(PtnComboBox, _T("RegExChkHistory"));
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::FormDestroy(TObject *Sender)
{
	delete MatchWdList;
}
//---------------------------------------------------------------------------
//テスト
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::TestActionExecute(TObject *Sender)
{
	try {
		cursor_HourGlass();
		ResListBox->Clear();
		MatchWdList->Clear();
		MatchCount = 0;

		UnicodeString ptnstr = PtnComboBox->Text;
		TRegExOptions opt;
		if (!CaseCheckBox->Checked) opt<<roIgnoreCase;
		for (int i=0; i<ObjMemo->Lines->Count; i++) {
			UnicodeString lbuf = ObjMemo->Lines->Strings[i];
			TMatchCollection mts = TRegEx::Matches(lbuf, ptnstr, opt);
			if (mts.Count>0) {
				UnicodeString mbuf;
				for (int j=0; j<mts.Count; j++) {
					if (mts.Item[j].Success) {
						if (j>0) mbuf.UCAT_T("\t");
						mbuf.cat_sprintf(_T("%u,%u"), mts.Item[j].Index, mts.Item[j].Length);
						MatchCount++;
					}
				}
				MatchWdList->Add(mbuf);
				ResListBox->Items->Add(lbuf);
			}
		}
		cursor_Default();

		//結果
		if (ResListBox->Count>0) {
			add_ComboBox_history(PtnComboBox);
			ResultLabel->Caption = UnicodeString().sprintf(_T("%u箇所でマッチしました"), MatchCount);
			ResultLabel->Font->Color = scl_WindowText;;
		}
		else {
			ResultLabel->Caption = "マッチしません";
			ResultLabel->Font->Color = col_Error;
			beep_Warn();
		}
	}
	catch (...) {
		cursor_Default();
		ResultLabel->Caption = "失敗しました";
		ResultLabel->Font->Color = col_Error;
		beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::TestActionUpdate(TObject *Sender)
{
	bool reg_ok = chk_RegExPtn(PtnComboBox->Text);
	PtnComboBox->Color = (PtnComboBox->Text.IsEmpty() || reg_ok)? scl_Window: col_Illegal;
	((TAction *)Sender)->Enabled = !ObjMemo->Lines->Text.IsEmpty() && reg_ok;

	if (PtnComboBox->Focused()) {
		PtnSelStart  = PtnComboBox->SelStart;
		PtnSelLength = PtnComboBox->SelLength;
	}
}
//---------------------------------------------------------------------------
//置換
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::ReplaceActionExecute(TObject *Sender)
{
	try {
		cursor_HourGlass();
		ResListBox->Clear();
		MatchWdList->Clear();
		MatchCount = 0;

		UnicodeString ptnstr = PtnComboBox->Text;
		TRegExOptions opt;
		if (!CaseCheckBox->Checked) opt<<roIgnoreCase;
		for (int i=0; i<ObjMemo->Lines->Count; i++) {
			UnicodeString lbuf = ObjMemo->Lines->Strings[i];
			TMatchCollection mts = TRegEx::Matches(lbuf, ptnstr, opt);
			if (mts.Count>0) {
				UnicodeString rbuf = TRegEx::Replace(lbuf, ptnstr, ReplaceEdit->Text, opt);
				UnicodeString mbuf;
				int ofs = 0;
				for (int j=0; j<mts.Count; j++) {
					if (mts.Item[j].Success) {
						if (j>0) mbuf.UCAT_T("\t");
						UnicodeString rwd = TRegEx::Replace(mts.Item[j].Value, ptnstr, ReplaceEdit->Text, opt);
						int p = mts.Item[j].Index + ofs;
						int n = rwd.Length();
						ofs += n - mts.Item[j].Value.Length();
						mbuf.cat_sprintf(_T("%u,%u"), p, n);
						MatchCount++;
					}
				}
				MatchWdList->Add(mbuf);
				ResListBox->Items->Add(rbuf);
			}
		}
		cursor_Default();

		//結果
		if (ResListBox->Count>0) {
			ResultLabel->Caption = UnicodeString().sprintf(_T("%u箇所を置換しました"), MatchCount);
			ResultLabel->Font->Color = scl_WindowText;;
		}
		else {
			ResultLabel->Caption = "マッチしません";
			ResultLabel->Font->Color = col_Error;
			beep_Warn();
		}
	}
	catch (...) {
		cursor_Default();
		ResultLabel->Caption = "失敗しました";
		ResultLabel->Font->Color = col_Error;
		beep_Warn();
	}
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::ReplaceActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = TestAction->Enabled;
}

//---------------------------------------------------------------------------
//貼り付け
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::PasteActionExecute(TObject *Sender)
{
	UnicodeString s = PtnComboBox->Text;
	if (with_Slash) s = "/" + s + "/";

	if (ObjComboBox) {
		ObjComboBox->Text = s;
		ObjComboBox->SelStart = s.Length();
	}
	else if (ObjCustomEdit) {
		ObjCustomEdit->Text = s;
		ObjCustomEdit->SelStart = s.Length();
	}
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::PasteActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !PtnComboBox->Text.IsEmpty() && (ObjComboBox || ObjCustomEdit);
}

//---------------------------------------------------------------------------
//コピー
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::CopyActionExecute(TObject *Sender)
{
	UnicodeString ptn = PtnComboBox->Text;
	if (((TAction*)Sender)->Tag==1) {
		ptn = ReplaceStr(ptn, "\\", "\\\\");
		ptn = ReplaceStr(ptn, "\"", "\\\"");
		ptn = "\"" + ptn + "\"";
	}
	copy_to_Clipboard(ptn);
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::CopyActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !PtnComboBox->Text.IsEmpty();
}

//---------------------------------------------------------------------------
void __fastcall TRegExChecker::ReferListBoxDblClick(TObject *Sender)
{
	TListBox *lp = (TListBox *)Sender;
	int idx = lp->ItemIndex;
	if (idx!=-1) {
		UnicodeString s = lp->Items->Strings[idx];
		if (s.Pos('\t')) {
			s = (idx<UserModule->RefRegExList->Count)? get_pre_tab(s) : get_post_tab(s);
			PtnComboBox->SetFocus();
			PtnComboBox->SelStart  = PtnSelStart;
			PtnComboBox->SelLength = PtnSelLength;
			PtnComboBox->SelText   = s;
		}
	}
}

//---------------------------------------------------------------------------
//検索対象ファイルの指定
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::RefFileBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("検索対象の指定"), F_FILTER_TXT, _T("*.txt"));
	if (UserModule->OpenDlgToEdit(FileEdit, true)) {
		UnicodeString fnam = rel_to_absdir(FileEdit->Text);
		if (file_exists(fnam)) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			if (load_text_ex(fnam, fbuf.get()))
				ObjMemo->Text = fbuf->Text;
			else
				msgbox_ERR(USTR_FileNotOpen);
		}
	}
}

//---------------------------------------------------------------------------
//結果の描画
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::ResListBoxDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas *cv  = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = scl_Window;
	TRect rc = Rect;
	cv->FillRect(rc);

	UnicodeString lbuf = lp->Items->Strings[Index];
	int l_len = lbuf.Length();

	//強調表示の設定
	std::unique_ptr<TColor[]> FgCol(new TColor[l_len + 1]);
	std::unique_ptr<TColor[]> BgCol(new TColor[l_len + 1]);
	for (int i=0; i<=l_len; i++) {
		FgCol[i] = cv->Font->Color;
		BgCol[i] = cv->Brush->Color;
	}
	//背景色を交互に変えるため補色を取得
	int cref = ColorToRGB(clHighlight);
	int r = GetRValue(cref);
	int g = GetGValue(cref);
	int b = GetBValue(cref);
	int x = std::max(r, std::max(g, b)) + std::min(r, std::min(g, b));
	r = x - r;  g = x - g;  b = x - b;
	TColor c2 = TColor(RGB(r, g, b));

	TStringDynArray mbuf = split_strings_tab(MatchWdList->Strings[Index]);
	for (int i=0; i<mbuf.Length; i++) {
		UnicodeString ibuf = mbuf[i];
		int p = split_tkn(ibuf, ',').ToIntDef(0);
		int n = ibuf.ToIntDef(0);
		if (p>0 && n>0) {
			for (int j=0; j<n; j++) {
				FgCol[p + j] = clHighlightText;
				BgCol[p + j] = (i%2==0)? clHighlight : c2;
			}
		}
	}

	//描画
	cv->Font->Color  = FgCol[1];
	cv->Brush->Color = BgCol[1];
	rc.Left += 4;
	rc.Top  += 1; rc.Bottom -= 1;
	int cp = 0;
	for (int i=1; i<=l_len; i++) {
		UnicodeString sbuf;
		if (lbuf[i]=='\t') {
			int n = 8 - cp%8;
			sbuf = StringOfChar(_T(' '), n);
			cp += n;
		}
		else {
			sbuf = lbuf[i];
			cp += str_len_half(sbuf);
		}

		int w = get_TextWidth(cv, sbuf, is_irreg);
		rc.Right = rc.Left + w;
		cv->Font->Color  = FgCol[i];
		cv->Brush->Color = BgCol[i];
		cv->TextRect(rc, rc.Left, rc.Top, sbuf);
		rc.Left += w;
	}
}

//---------------------------------------------------------------------------
void __fastcall TRegExChecker::PtnComboBoxEnter(TObject *Sender)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegExChecker::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

