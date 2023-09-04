//----------------------------------------------------------------------//
// INIファイル処理クラス												//
//																		//
//----------------------------------------------------------------------//
#include "usr_scale.h"
#include "usr_str.h"
#include "usr_file_ex.h"
#include "UserFunc.h"
#include "UIniFile.h"

//---------------------------------------------------------------------------
UnicodeString SCT_General;
UnicodeString SCT_Option;

//---------------------------------------------------------------------------
UsrIniFile *IniFile = NULL;

//---------------------------------------------------------------------------
//セクションのソート用比較関数
//---------------------------------------------------------------------------
int __fastcall SortComp_Sct(TStringList *List, int Index1, int Index2)
{
	UnicodeString s0 = List->Strings[Index1];
	UnicodeString s1 = List->Strings[Index2];

	if (contains_PathDlmtr(s0) && !contains_PathDlmtr(s1)) return 1;
	if (contains_PathDlmtr(s1) && !contains_PathDlmtr(s0)) return -1;
	return CompareText(s0, s1);
}

//---------------------------------------------------------------------------
UsrIniFile::UsrIniFile(UnicodeString fnam)
{
	OfsDlgList = new TStringList();

	PosOffsetLeft = PosOffsetTop = 0;

	SCT_General = "General";
	SCT_Option	= "Option";

	FileName	= fnam;
	SectionList = new TStringList();
	MarkIdxList = new TStringList();

	LoadValues();
}
//---------------------------------------------------------------------------
UsrIniFile::~UsrIniFile()
{
	Clear();
	delete SectionList;
	delete MarkIdxList;
	delete OfsDlgList;
}

//---------------------------------------------------------------------------
TStringList * UsrIniFile::AddSection(UnicodeString sct)
{
	TStringList *klist = new TStringList();
	SectionList->AddObject(sct, (TObject*)klist);
	return klist;
}
//---------------------------------------------------------------------------
void UsrIniFile::Clear()
{
	for (int i=0; i<SectionList->Count; i++) delete (TStringList*)SectionList->Objects[i];
	SectionList->Clear();

	for (int i=0; i<MarkIdxList->Count; i++) delete (TStringList*)MarkIdxList->Objects[i];
	MarkIdxList->Clear();
}

//---------------------------------------------------------------------------
// 全情報を一括で読み込む
//---------------------------------------------------------------------------
void UsrIniFile::LoadValues()
{
	try {
		if (!FileExists(FileName)) Abort();
		std::unique_ptr<TStringList> fbuf(new TStringList());
		fbuf->LoadFromFile(FileName);
		TStringList *klist = NULL;
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString s  = fbuf->Strings[i];
			UnicodeString s2 = Trim(s);
			if (s2.IsEmpty() || s2[1]==';') continue;	//空行やコメント行は無視
			if (StartsStr('[', s) && EndsStr(']', s))
				klist = AddSection(exclude_top_end(s));
			else
				if (klist) klist->Add(s);
		}

		//マーク情報のチェック
		UpdateMarkIdxList();
		CheckMarkItems();
	}
	catch (...) {
		Clear();
	}

	Modified = false;
}

//---------------------------------------------------------------------------
//再読み込み
//---------------------------------------------------------------------------
void UsrIniFile::Reload()
{
	Clear();
	LoadValues();
}

//---------------------------------------------------------------------------
//セクションを削除
//---------------------------------------------------------------------------
void UsrIniFile::EraseSection(UnicodeString sct)
{
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) {
		delete (TStringList*)SectionList->Objects[idx];
		SectionList->Delete(idx);
		Modified = true;
	}
}

//---------------------------------------------------------------------------
//キーを削除
//---------------------------------------------------------------------------
bool UsrIniFile::DeleteKey(UnicodeString sct, UnicodeString key)
{
	bool deleted = false;
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		idx = klist->IndexOfName(key);
		if (idx!=-1) {
			klist->Delete(idx);
			deleted = true;
		}
	}
	return deleted;
}

//---------------------------------------------------------------------------
//キーが存在するか？
//---------------------------------------------------------------------------
bool UsrIniFile::KeyExists(UnicodeString sct, UnicodeString key)
{
	bool ret = false;
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		ret = (klist->IndexOfName(key) != -1);
	}
	return ret;
}
//---------------------------------------------------------------------------
//キー名の変更
//---------------------------------------------------------------------------
void UsrIniFile::RenameKey(UnicodeString sct, UnicodeString old_key, UnicodeString new_key)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		idx = klist->IndexOfName(old_key);
		if (idx!=-1) klist->Strings[idx] = new_key + "=" + klist->ValueFromIndex[idx];
	}
}
//---------------------------------------------------------------------------
//キー名の置換
//---------------------------------------------------------------------------
void UsrIniFile::ReplaceKey(UnicodeString sct, UnicodeString s0, UnicodeString s1)
{
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		int i = 0;
		while (i<klist->Count) {
			UnicodeString key0 = klist->Names[i];
			if (ContainsText(key0, s0)) {
				UnicodeString key1 = ReplaceText(key0, s0, s1);
				if (klist->IndexOfName(key1)!=-1) {
					klist->Delete(i);
				}
				else {
					klist->Strings[i] = key1 + "=" + klist->ValueFromIndex[i];
					i++;
				}
			}
			else {
				i++;
			}
		}
	}
}

//---------------------------------------------------------------------------
//セクションが存在するか？
//---------------------------------------------------------------------------
bool UsrIniFile::RectionExists(UnicodeString sct)
{
	return (SectionList->IndexOf(sct)!=-1);
}
//---------------------------------------------------------------------------
//セクションを読み込む
//---------------------------------------------------------------------------
void UsrIniFile::ReadSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) lst->Assign((TStringList*)SectionList->Objects[idx]);
}
//---------------------------------------------------------------------------
//セクションへ複写
//---------------------------------------------------------------------------
void UsrIniFile::AssignSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;

	int idx = SectionList->IndexOf(sct);
	TStringList *klist = (idx!=-1)? (TStringList*)SectionList->Objects[idx] : AddSection(sct);
	klist->Assign(lst);
	Modified = true;
}

//---------------------------------------------------------------------------
UnicodeString UsrIniFile::ReadString(UnicodeString sct, UnicodeString key,
	UnicodeString def,	//デフォルト値	(default = EmptyStr)
	bool del_quot)		//引用符を外す	(default = true)
{
	UnicodeString ret;
	int idx = SectionList->IndexOf(sct);
	if (idx!=-1) {
		TStringList * klist = (TStringList*)SectionList->Objects[idx];
		ret = klist->Values[key];
		if (del_quot) ret = exclude_quot(ret);
	}

	if (ret.IsEmpty()) ret = def;

	return ret;
}
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::ReadStrGen(const _TCHAR *key, UnicodeString def, bool del_quot)
{
	return ReadString(SCT_General, key, def, del_quot);
}

//---------------------------------------------------------------------------
int UsrIniFile::ReadInteger(UnicodeString sct, UnicodeString key,
	int def)	//デフォルト値	(default = 0)
{
	return StrToIntDef(ReadString(sct, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
int UsrIniFile::ReadScaledInteger(UnicodeString sct, UnicodeString key,
	int def,		//デフォルト値	(default = 0)
	TControl *cp)	//	(default = NULL : Application->MainForm) 
{
	if (!cp) cp = Application->MainForm;
	if (!KeyExists(sct, key)) WriteBool(sct, key + "_Scaled", true);
	if (ReadBool(sct, key + "_Scaled"))
		return ScaledInt(ReadInteger(sct, key, def), cp);
	else
		return ReadInteger(sct, key, def);
}
//---------------------------------------------------------------------------
int UsrIniFile::ReadIntGen(const _TCHAR *key, int def)
{
	return StrToIntDef(ReadString(SCT_General, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
int UsrIniFile::ReadScaledIntGen(const _TCHAR *key, int def,
	TControl *cp)	//	(default = NULL : Application->MainForm) 
{
	if (!cp) cp = Application->MainForm;
	if (!KeyExists(SCT_General, key)) WriteBool(SCT_General, UnicodeString(key) + "_Scaled", true);
	if (ReadBoolGen((UnicodeString(key) + "_Scaled").c_str()))
		return ScaledInt(ReadIntGen(key, def), cp);
	else
		return ReadIntGen(key, def);
}

//---------------------------------------------------------------------------
bool UsrIniFile::ReadBool(UnicodeString sct, UnicodeString key,
	bool def)	//デフォルト値	(default = false)
{
	return (ReadInteger(sct, key, (int)def) != 0);
}
//---------------------------------------------------------------------------
bool UsrIniFile::ReadBoolGen(const _TCHAR *key,
	bool def)	//デフォルト値	(default = false)
{
	return (ReadInteger(SCT_General, key, (int)def) != 0);
}

//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColor(UnicodeString sct, UnicodeString key,
	TColor def)	//デフォルト値	(default = clBlack)
{
	return (TColor)StrToIntDef(ReadString(sct, key, EmptyStr), (int)def);
}
//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColGen(const _TCHAR *key, TColor def)
{
	return (TColor)StrToIntDef(ReadString(SCT_General, key, EmptyStr), (int)def);
}

//---------------------------------------------------------------------------
//フォントを作成し、情報を読み込んで設定
//  作成されたフォントは、最後に破棄すること
//  セクション名=説明/フォント・オブジェクトをリストに登録
//---------------------------------------------------------------------------
TFont* UsrIniFile::ReadFontInf(
	const _TCHAR *sct,	//セクション名
	TFont *def,			//デフォルト	(default = NULL)
	TStringList *lst,	//リスト		(default = NULL)
	const _TCHAR *dsc)	//説明 			(default = nULL)
{
	TFont *f = new TFont();
	f->Name    = ReadString( sct, "FontName",	(def? def->Name : EmptyStr));
	f->Size    = ReadInteger(sct, "FontSize",	(def? def->Size : 11));
	f->Charset = (TFontCharset)ReadInteger(sct, "FontCharset",	(int)(def? def->Charset : DEFAULT_CHARSET));

	TFontStyles fstyle = TFontStyles();
	if (ReadBool(sct, "FontBold",	(def? def->Style.Contains(fsBold) : false)))	fstyle << fsBold;
	if (ReadBool(sct, "FontItalic",	(def? def->Style.Contains(fsItalic) : false)))	fstyle << fsItalic;
	f->Style = fstyle;

	if (lst) {
		UnicodeString lbuf;
		lbuf.sprintf(_T("%s=%s"), sct, (dsc? dsc : null_TCHAR));
		lst->AddObject(lbuf, (TObject*)f);
	}

	return f;
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteString(UnicodeString sct, UnicodeString key, UnicodeString v)
{
	int idx = SectionList->IndexOf(sct);
	TStringList *klist = (idx!=-1)? (TStringList*)SectionList->Objects[idx] : AddSection(sct);

	UnicodeString s;
	s.sprintf(_T("%s=%s"), key.c_str(), v.c_str());

	idx = klist->IndexOfName(key);
	if (idx!=-1) {
		if (!SameStr(klist->Strings[idx], s)) {
			klist->Strings[idx] = s;
			Modified = true;
		}
	}
	else {
		klist->Add(s);
		Modified = true;
	}
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteString(UnicodeString sct, UnicodeString key, TEdit *ep)
{
	WriteString(sct, key, ep->Text);
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteString(UnicodeString sct, UnicodeString key, TLabeledEdit *ep)
{
	WriteString(sct, key, ep->Text);
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteStrGen(const _TCHAR *key, UnicodeString v)
{
	WriteString(SCT_General, key, v);
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteStrGen(const _TCHAR *key, TEdit *ep)
{
	WriteString(SCT_General, key, ep->Text);
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteStrGen(const _TCHAR *key, TLabeledEdit *ep)
{
	WriteString(SCT_General, key, ep->Text);
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteInteger(UnicodeString sct, UnicodeString key, int v)
{
	WriteString(sct, key, UnicodeString(v));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteInteger(UnicodeString sct, UnicodeString key, TComboBox *cp)
{
	WriteString(sct, key, UnicodeString(cp->ItemIndex));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteInteger(UnicodeString sct, UnicodeString key, TRadioGroup *rp)
{
	WriteString(sct, key, UnicodeString(rp->ItemIndex));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteScaledInteger(UnicodeString sct, UnicodeString key, int v,
	TControl *cp)	//	(default = NULL : Application->MainForm)
{
	if (!cp) cp = Application->MainForm;
	WriteBool(sct, key + "_Scaled", true);
	WriteInteger(sct, key, UnscaledInt(v, cp));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteIntGen(const _TCHAR *key, int v)
{
	WriteString(SCT_General, key, UnicodeString(v));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteIntGen(const _TCHAR *key, TComboBox *cp)
{
	WriteString(SCT_General, key, UnicodeString(cp->ItemIndex));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteIntGen(const _TCHAR *key, TRadioGroup *rp)
{
	WriteString(SCT_General, key, UnicodeString(rp->ItemIndex));
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteScaledIntGen(const _TCHAR *key, int v,
	TControl *cp)	//	(default = NULL : Application->MainForm)
{
	if (!cp) cp = Application->MainForm;
	WriteBool(SCT_General, UnicodeString(key) + "_Scaled", true);
	WriteIntGen(key, UnscaledInt(v, cp));
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteBool(UnicodeString sct, UnicodeString key, bool v)
{
	WriteString(sct, key, v? "1" : "0");
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBool(UnicodeString sct, UnicodeString key, TCheckBox *cp)
{
	WriteString(sct, key, cp->Checked? "1" : "0");
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBoolGen(const _TCHAR *key, bool v)
{
	WriteString(SCT_General, key, v? "1" : "0");
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBoolGen(const _TCHAR *key, TCheckBox *cp)
{
	WriteString(SCT_General, key, cp->Checked? "1" : "0");
}
//---------------------------------------------------------------------------
void UsrIniFile::WriteBoolGen(const _TCHAR *key, TAction *ap)
{
	WriteString(SCT_General, key, ap->Checked? "1" : "0");
}

//---------------------------------------------------------------------------
void UsrIniFile::WriteFontInf(UnicodeString sct, TFont *f)
{
	WriteString( sct, "FontName",		f->Name);
	WriteInteger(sct, "FontSize",		f->Size);
	WriteBool(   sct, "FontBold",		f->Style.Contains(fsBold));
	WriteBool(   sct, "FontItalic",		f->Style.Contains(fsItalic));
	WriteInteger(sct, "FontCharset",	(int)f->Charset);
}

//---------------------------------------------------------------------------
//メインフォームの位置・サイズを復元
//---------------------------------------------------------------------------
void UsrIniFile::LoadFormPos(
	TForm *frm, int w, int h,
	UnicodeString prfx)	//キー名のプリフィックス
{
	if (prfx.IsEmpty()) prfx = "Win";

	frm->Left = ReadInteger(SCT_General, prfx + "Left", (Screen->Width  - ScaledInt(w, frm))/2);
	frm->Top  = ReadInteger(SCT_General, prfx + "Top",  (Screen->Height - ScaledInt(h, frm))/2);
	//Left, Top よって CurrentPPI が変化

	frm->Width	= ScaledInt(ReadInteger(SCT_General, prfx + "Width",  w), frm);
	frm->Height = ScaledInt(ReadInteger(SCT_General, prfx + "Height", h), frm);

	//1画面の場合、画面外に出ないように
	if (Screen->MonitorCount==1) {
		int mgn = 8;
		if (frm->Left<-mgn) frm->Left = 0;
		if (frm->Top<-mgn)  frm->Top  = 0;
		if (frm->BoundsRect.Right > Screen->Width+mgn)   frm->Left = Screen->Width  - frm->Width;
		if (frm->BoundsRect.Bottom > Screen->Height+mgn) frm->Top  = Screen->Height - frm->Height;
	}

	frm->WindowState = (TWindowState)ReadInteger(SCT_General, prfx + "State", (int)wsNormal);
}
//---------------------------------------------------------------------------
//メインフォームの位置・サイズを保存
//---------------------------------------------------------------------------
void UsrIniFile::SaveFormPos(TForm *frm)
{
	if (frm->WindowState==wsMinimized) frm->WindowState = wsNormal;
	WriteIntGen(_T("WinState"),	(int)frm->WindowState);
	if (frm->WindowState==wsMaximized) frm->WindowState = wsNormal;

	WriteIntGen(_T("WinLeft"),	frm->Left);
	WriteIntGen(_T("WinTop"),	frm->Top);
	WriteIntGen(_T("WinWidth"),	UnscaledInt(frm->Width,  frm));
	WriteIntGen(_T("WinHeight"),UnscaledInt(frm->Height, frm));
	WriteIntGen(_T("WinPPI"),	frm->CurrentPPI);
}

//---------------------------------------------------------------------------
//位置情報を読込
//---------------------------------------------------------------------------
void UsrIniFile::LoadPosInfo(TForm *frm, int x, int y, int w, int h, UnicodeString key)
{
	if (key.IsEmpty()) key = frm->Name;

	frm->Left = ReadInteger(SCT_General, key + "Left", x);
	frm->Top  = ReadInteger(SCT_General, key + "Top",  y);

	if ((frm->BorderStyle==bsSizeable || frm->BorderStyle==bsSizeToolWin) && w>0 && h>0) {
		frm->Width	= ScaledInt(ReadInteger(SCT_General, key + "Width",  w), frm);
		frm->Height = ScaledInt(ReadInteger(SCT_General, key + "Height", h), frm);
	}

	//1画面の場合、半分以上が画面外だったら中央に戻す
	if (Screen->MonitorCount==1) {
		TRect rc = TRect::Intersect(Rect(0,0,Screen->Width,Screen->Height), frm->BoundsRect);
		if (rc.Width()  < frm->Width/2)  frm->Left = (Screen->Width - frm->Width) / 2;
		if (rc.Height() < frm->Height/2) frm->Top  = (Screen->Height - frm->Height) / 2;
	}
}
//---------------------------------------------------------------------------
void UsrIniFile::LoadPosInfo(TForm *frm, bool dlg_center, UnicodeString key)
{
	int w = frm->Constraints->MinWidth;		if (w==0) w = frm->Width;
	int h = frm->Constraints->MinHeight;	if (h==0) h = frm->Height;

	LoadPosInfo(frm, (Screen->Width - w)/2, (Screen->Height - h)/2, w, h, key);

	if (dlg_center) {
		frm->Left = Application->MainForm->Left + (Application->MainForm->Width - frm->Width)/2;
		frm->Top  = Application->MainForm->Top + (Application->MainForm->Height - frm->Height)/2;
	}
	else {
		//オフセットを適用
		frm->Left = frm->Left + PosOffsetLeft;
		frm->Top  = frm->Top  + PosOffsetTop;
		if (OfsDlgList->IndexOf(frm->Name)==-1) OfsDlgList->Add(frm->Name);
	}

	adjust_form_pos(frm);
}

//---------------------------------------------------------------------------
//位置情報保存
//---------------------------------------------------------------------------
void UsrIniFile::SavePosInfo(TForm *frm, UnicodeString key)
{
	if (key.IsEmpty()) key = frm->Name;

	bool ofs = (OfsDlgList->IndexOf(frm->Name)!=-1);
	WriteInteger(SCT_General, key + "Left", frm->Left - (ofs? PosOffsetLeft : 0));
	WriteInteger(SCT_General, key + "Top",  frm->Top  - (ofs? PosOffsetTop  : 0));

	if (frm->BorderStyle!=bsDialog) {
		WriteInteger(SCT_General, key + "Width",  UnscaledInt(frm->Width,  frm));
		WriteInteger(SCT_General, key + "Height", UnscaledInt(frm->Height, frm));
	}
}

//---------------------------------------------------------------------------
//グリッドのカラム幅設定 (スケーリングを考慮)
//---------------------------------------------------------------------------
void UsrIniFile::LoadGridColWidth(TStringGrid *gp, int cnt, ...)
{
	bool is_scaled = ReadBool(gp->Name, "ColWidth_Scaled");

	va_list ap;
	va_start(ap, cnt);
	UnicodeString key;
	for (int i=0; i<cnt; i++) {
		int arg = va_arg(ap, int);
		int wd = 0;
		if (i<gp->FixedCols)
			wd = arg;
		else if (i<gp->ColCount && arg>0 && arg<1200)
			wd = ReadInteger(gp->Name, key.sprintf(_T("ColWidth%u"), i), arg);

		if (wd>0) {
			if (is_scaled) wd = ScaledInt(wd, gp);
			gp->ColWidths[i] = wd;
		}
	}
	va_end(ap);
}
//---------------------------------------------------------------------------
//グリッドのカラム幅保存 (スケーリングを考慮)
//---------------------------------------------------------------------------
void UsrIniFile::SaveGridColWidth(TStringGrid *gp)
{
	UnicodeString key;
	for (int i=0; i<gp->ColCount; i++) {
		WriteInteger(gp->Name, key.sprintf(_T("ColWidth%u"), i), UnscaledInt(gp->ColWidths[i]));
	}

	WriteBool(gp->Name, "ColWidth_Scaled", true);
}

//---------------------------------------------------------------------------
//ComboBox の項目を読込
//---------------------------------------------------------------------------
void UsrIniFile::LoadComboBoxItems(
	TComboBox *cp,
	const _TCHAR *sct,	//セクション名
	int max_items,		//最大項目数	(default = 20)
	bool del_quot)		//引用符を外す	(default = false)
{
	UnicodeString sct_u = def_if_empty(sct, cp->Name);
	UnicodeString key;

	cp->Clear();
	for (int i=0; i<max_items; i++) {
		UnicodeString itm_str = ReadString(sct_u, key.sprintf(_T("Item%u"), i + 1), EmptyStr, del_quot);
		if (itm_str.IsEmpty()) break;
		if (cp->Items->IndexOf(itm_str)==-1) cp->Items->Add(itm_str);
	}
}
//---------------------------------------------------------------------------
//ComboBox の項目を保存
//---------------------------------------------------------------------------
void UsrIniFile::SaveComboBoxItems(
	TComboBox *cp,
	const _TCHAR *sct,	//セクション名
	int max_items)		//最大項目数	(default = 20)
{
	UnicodeString sct_u = def_if_empty(sct, cp->Name);
	UnicodeString key;

	for (int i=0; i<max_items; i++) {
		key.sprintf(_T("Item%u"), i + 1);
		if (i<cp->Items->Count)
			WriteString(sct_u, key, cp->Items->Strings[i]);
		else
			DeleteKey(sct_u, key);
	}
}

//---------------------------------------------------------------------------
//リスト項目の読込
//---------------------------------------------------------------------------
void UsrIniFile::LoadListItems(UnicodeString sct, TStringList *lst,
	int max_items,		//最大項目数	(default = 20)	  0 の場合無制限
	bool del_quot)		//引用符を外す	(default = true)  ！CSV項目の場合は false に
{
	lst->Clear();
	for (int i=0; (max_items==0 || i<max_items); i++) {
		UnicodeString itm_str = ReadString(sct, UnicodeString().sprintf(_T("Item%u"), i + 1), EmptyStr, del_quot);
		if (itm_str.IsEmpty()) break;
		lst->Add(itm_str);
	}
}
//---------------------------------------------------------------------------
//リスト項目の保存
//---------------------------------------------------------------------------
void UsrIniFile::SaveListItems(UnicodeString sct, TStringList *lst,
	int max_items)		//最大項目数	(default = 20)	  0 の場合無制限
{
	EraseSection(sct);
	for (int i=0; i<lst->Count && (max_items==0 || i<max_items); i++)
		WriteString(sct, UnicodeString().sprintf(_T("Item%u"), i + 1), lst->Strings[i]);
}

//---------------------------------------------------------------------------
//マークインデックスの更新
//---------------------------------------------------------------------------
void UsrIniFile::UpdateMarkIdxList()
{
	for (int i=0; i<MarkIdxList->Count; i++) delete (TStringList*)MarkIdxList->Objects[i];
	MarkIdxList->Clear();

	//マークセクションのコピーを作成
	for (int i=0; i<SectionList->Count; i++) {
		UnicodeString sct = SectionList->Strings[i];
		if (contains_PathDlmtr(sct)) {
			TStringList *klist = new TStringList();
			klist->Assign((TStringList*)SectionList->Objects[i]);
			MarkIdxList->AddObject(sct, (TObject*)klist);
		}
	}
}

//---------------------------------------------------------------------------
//マーク項目の存在チェック
//---------------------------------------------------------------------------
void UsrIniFile::CheckMarkItems()
{
	for (int i=0; i<MarkIdxList->Count; i++) {
		UnicodeString dnam = MarkIdxList->Strings[i];
		if (is_drive_accessible(dnam)) {
			//ディレクトリ
			if (ends_PathDlmtr(dnam)) {
				if (dir_exists(dnam)) {
					int idx = SectionList->IndexOf(dnam);
					if (idx!=-1) {
						TStringList *klist = (TStringList*)SectionList->Objects[idx];
						int j = 0;
						while (j<klist->Count) {
							UnicodeString fnam = dnam + get_pre_tab(klist->Strings[j]);
							if (!file_exists(fnam)) klist->Delete(j); else j++;
						}
						if (klist->Count==0) EraseSection(dnam);
					}
				}
				else {
					EraseSection(dnam);
				}
			}
			//アーカイブ(内部はチェックしない)
			else if (!file_exists(dnam)) {
				EraseSection(dnam);
			}
		}
		//アクセスできないUNCパスのセクションは削除
		else if (StartsStr("\\\\", dnam)) {
			EraseSection(dnam);
		}
	}

	UpdateMarkIdxList();
}

//---------------------------------------------------------------------------
//すべてのマークを解除
//---------------------------------------------------------------------------
void UsrIniFile::ClearAllMark()
{
	for (int i=0; i<MarkIdxList->Count; i++) delete (TStringList*)MarkIdxList->Objects[i];
	MarkIdxList->Clear();

	int i = 0;
	while (i<SectionList->Count) {
		if (contains_PathDlmtr(SectionList->Strings[i])) {
			delete (TStringList*)SectionList->Objects[i];
			SectionList->Delete(i);
			Modified = true;
		}
		else {
			i++;
		}
	}
}

//---------------------------------------------------------------------------
//ファイルマーク/解除
//---------------------------------------------------------------------------
bool UsrIniFile::FileMark(
	UnicodeString fnam,		//ファイル名
	int flag,				//-1 = 反転/ 0 = 解除/ 1 = マーク	(default = -1)
	UnicodeString memo)		//メモ								(default = EmptyStr)
{
	bool res	 = false;
	bool deleted = false;

	UnicodeString sct, itm;
	//アーカイブ
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//通常
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	int s_idx = SectionList->IndexOf(sct);
	TStringList *klist = NULL;
	if (s_idx!=-1) {
		klist = (TStringList*)SectionList->Objects[s_idx];
		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;

		if (k_idx>=0 && flag!=1) {
			//解除
			klist->Delete(k_idx);
			if (klist->Count==0) EraseSection(sct);
			deleted = true;
			res = false;
		}
	}

	if (flag==0) {
		res = false;
	}
	else if (!deleted) {
		//マーク
		if (!klist) klist = AddSection(sct);

		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;

		itm.cat_sprintf(_T("\t%s\t%s"), memo.c_str(), FormatDateTime("yyyy'/'mm'/'dd hh:nn:ss", Now()).c_str());
		if (k_idx!=-1) {
			klist->Strings[k_idx] = itm;
		}
		else {
			klist->Add(itm);
			Modified = true;
		}
		res = true;
	}

	//マークインデックスの更新
	UpdateMarkIdxList();

	return res;
}

//---------------------------------------------------------------------------
//ファイルがマークされているか？
//---------------------------------------------------------------------------
bool UsrIniFile::IsMarked(UnicodeString fnam)
{
	bool ret = false;

	UnicodeString sct, itm;
	//アーカイブ
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//通常
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	int idx = MarkIdxList->IndexOf(sct);
	if (idx!=-1) {
		TStringList *klist = (TStringList*)MarkIdxList->Objects[idx];
		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;
		ret = (k_idx != -1);
	}
	return ret;
}
//---------------------------------------------------------------------------
//アーカイブ内のマークファイルを取得
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::MarkedInArc(
	UnicodeString arc_file,	//アーカイブファイル名
	UnicodeString xlist)	//対応拡張子	(default = EmptyStr : すべてマッチ)
{
	int idx = MarkIdxList->IndexOf(arc_file);
	if (idx==-1) return EmptyStr;

	UnicodeString ret_nam;
	TStringList *klist = (TStringList*)MarkIdxList->Objects[idx];
	for (int i=0; i<klist->Count; i++) {
		UnicodeString fnam = get_pre_tab(klist->Strings[i]);
		if (xlist.IsEmpty() || test_FileExt(get_extension(fnam), xlist)) {
			ret_nam = arc_file + "/" + fnam;
			break;
		}
	}
	return ret_nam;
}
//---------------------------------------------------------------------------
//マークファイルのメモを取得
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::GetMarkMemo(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return EmptyStr;

	UnicodeString sct, itm;
	//アーカイブ
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//通常
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	UnicodeString memo;
	int idx = MarkIdxList->IndexOf(sct);
	if (idx!=-1) {
		TStringList *klist = (TStringList*)MarkIdxList->Objects[idx];
		for (int i=0; i<klist->Count; i++) {
			UnicodeString lbuf = klist->Strings[i];	//ファイル名 \t メモ \t 設定日時
			if (SameText(itm, split_pre_tab(lbuf))) {
				memo = get_pre_tab(lbuf);  break;
			}
		}
	}

	return Trim(memo);
}

//---------------------------------------------------------------------------
//INIファイルの更新
//---------------------------------------------------------------------------
bool UsrIniFile::UpdateFile(
	bool force)		//true = 変更がなくても強制的に保存/ false = 変更があれば保存
{
	try {
		if (Modified || force) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			SectionList->CustomSort(SortComp_Sct);
			UnicodeString sct;
			for (int i=0; i<SectionList->Count; i++) {
				fbuf->Add(sct.sprintf(_T("[%s]"), SectionList->Strings[i].c_str()));
				TStringList *klist = (TStringList*)SectionList->Objects[i];
				for (int j=0; j<klist->Count; j++) fbuf->Add(klist->Strings[j]);
				fbuf->Add(EmptyStr);
			}
			fbuf->SaveToFile(FileName, TEncoding::UTF8);
			Modified = false;
		}
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
