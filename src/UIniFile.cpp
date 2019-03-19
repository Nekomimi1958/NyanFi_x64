//----------------------------------------------------------------------//
// INI�t�@�C�������N���X												//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <System.StrUtils.hpp>
#include "usr_str.h"
#include "usr_file_ex.h"
#include "UIniFile.h"

//---------------------------------------------------------------------------
UnicodeString SCT_General;
UnicodeString SCT_Option;

//---------------------------------------------------------------------------
UsrIniFile *IniFile = NULL;

//---------------------------------------------------------------------------
//�Z�N�V�����̃\�[�g�p��r�֐�
//---------------------------------------------------------------------------
int __fastcall SortComp_Sct(TStringList *List, int Index1, int Index2)
{
	UnicodeString s0 = List->Strings[Index1];
	UnicodeString s1 = List->Strings[Index2];

	if (contains_PathDlmtr(s0) && !contains_PathDlmtr(s1))
		return 1;
	else if (contains_PathDlmtr(s1) && !contains_PathDlmtr(s0))
		return -1;
	else
		return CompareText(s0, s1);
}

//---------------------------------------------------------------------------
UsrIniFile::UsrIniFile(UnicodeString fnam)
{
	OfsDlgList = new TStringList();

	PosOffsetLeft = PosOffsetTop = 0;

	SCT_General.USET_T("General");
	SCT_Option.USET_T("Option");

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
// �S�����ꊇ�œǂݍ���
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
			if (s2.IsEmpty() || s2[1]==';') continue;	//��s��R�����g�s�͖���
			if (StartsStr('[', s) && EndsStr(']', s))
				klist = AddSection(exclude_top_end(s));
			else
				if (klist) klist->Add(s);
		}

		//�}�[�N���̃`�F�b�N
		UpdateMarkIdxList();
		CheckMarkItems();
	}
	catch (...) {
		Clear();
	}

	Modified = false;
}

//---------------------------------------------------------------------------
//�ēǂݍ���
//---------------------------------------------------------------------------
void UsrIniFile::Reload()
{
	Clear();
	LoadValues();
}

//---------------------------------------------------------------------------
//�Z�N�V�������폜
//---------------------------------------------------------------------------
void UsrIniFile::EraseSection(UnicodeString sct)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		delete (TStringList*)SectionList->Objects[idx];
		SectionList->Delete(idx);
		Modified = true;
	}
}

//---------------------------------------------------------------------------
//�L�[���폜
//---------------------------------------------------------------------------
bool UsrIniFile::DeleteKey(UnicodeString sct, UnicodeString key)
{
	bool deleted = false;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		idx = klist->IndexOfName(key);
		if (idx>=0) {
			klist->Delete(idx);
			deleted = true;
		}
	}
	return deleted;
}

//---------------------------------------------------------------------------
//�L�[�����݂��邩�H
//---------------------------------------------------------------------------
bool UsrIniFile::KeyExists(UnicodeString sct, UnicodeString key)
{
	bool ret = false;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		ret = (klist->IndexOfName(key) != -1);
	}
	return ret;
}
//---------------------------------------------------------------------------
//�L�[���̕ύX
//---------------------------------------------------------------------------
void UsrIniFile::RenameKey(UnicodeString sct, UnicodeString old_key, UnicodeString new_key)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		idx = klist->IndexOfName(old_key);
		if (idx>=0) klist->Strings[idx] = new_key.UCAT_T("=") + klist->ValueFromIndex[idx];
	}
}
//---------------------------------------------------------------------------
//�L�[���̒u��
//---------------------------------------------------------------------------
void UsrIniFile::ReplaceKey(UnicodeString sct, UnicodeString s0, UnicodeString s1)
{
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)SectionList->Objects[idx];
		for (int i=0; i<klist->Count; i++) {
			UnicodeString key = klist->Names[i];
			if (ContainsText(key, s0)) klist->Strings[i] = ReplaceText(key, s0, s1).UCAT_T("=") + klist->ValueFromIndex[i];
		}
	}
}

//---------------------------------------------------------------------------
//�Z�N�V���������݂��邩�H
//---------------------------------------------------------------------------
bool UsrIniFile::RectionExists(UnicodeString sct)
{
	return (SectionList->IndexOf(sct)!=-1);
}
//---------------------------------------------------------------------------
//�Z�N�V������ǂݍ���
//---------------------------------------------------------------------------
void UsrIniFile::ReadSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) lst->Assign((TStringList*)SectionList->Objects[idx]);
}
//---------------------------------------------------------------------------
//�Z�N�V�����֕���
//---------------------------------------------------------------------------
void UsrIniFile::AssignSection(UnicodeString sct, TStringList *lst)
{
	if (!lst) return;

	int idx = SectionList->IndexOf(sct);
	TStringList *klist = (idx>=0)? (TStringList*)SectionList->Objects[idx] : AddSection(sct);
	klist->Assign(lst);
	Modified = true;
}

//---------------------------------------------------------------------------
UnicodeString UsrIniFile::ReadString(UnicodeString sct, UnicodeString key,
	UnicodeString def,	//�f�t�H���g�l	(default = EmptyStr)
	bool del_quot)		//���p�����O��	(default = true)
{
	UnicodeString ret;
	int idx = SectionList->IndexOf(sct);
	if (idx>=0) {
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
	int def)	//�f�t�H���g�l	(default = 0)
{
	return StrToIntDef(ReadString(sct, key, EmptyStr), def);
}
//---------------------------------------------------------------------------
int UsrIniFile::ReadIntGen(const _TCHAR *key, int def)
{
	return StrToIntDef(ReadString(SCT_General, key, EmptyStr), def);
}

//---------------------------------------------------------------------------
bool UsrIniFile::ReadBool(UnicodeString sct, UnicodeString key,
	bool def)	//�f�t�H���g�l	(default = false)
{
	return (ReadInteger(sct, key, (int)def) != 0);
}
//---------------------------------------------------------------------------
bool UsrIniFile::ReadBoolGen(const _TCHAR *key,
	bool def)	//�f�t�H���g�l	(default = false)
{
	return (ReadInteger(SCT_General, key, (int)def) != 0);
}

//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColor(UnicodeString sct, UnicodeString key,
	TColor def)	//�f�t�H���g�l	(default = clBlack)
{
	return (TColor)StrToIntDef(ReadString(sct, key, EmptyStr), (int)def);
}
//---------------------------------------------------------------------------
TColor UsrIniFile::ReadColGen(const _TCHAR *key, TColor def)
{
	return (TColor)StrToIntDef(ReadString(SCT_General, key, EmptyStr), (int)def);
}

//---------------------------------------------------------------------------
//�t�H���g���쐬���A����ǂݍ���Őݒ�
//  �쐬���ꂽ�t�H���g�́A�Ō�ɔj�����邱��
//  �Z�N�V������=����/�t�H���g�E�I�u�W�F�N�g�����X�g�ɓo�^
//---------------------------------------------------------------------------
TFont* UsrIniFile::ReadFontInf(
	const _TCHAR *sct,	//�Z�N�V������
	TFont *def,			//�f�t�H���g	(default = NULL)
	TStringList *lst,	//���X�g		(default = NULL)
	const _TCHAR *dsc)	//���� 			(default = nULL)
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
	TStringList *klist = (idx>=0)? (TStringList*)SectionList->Objects[idx] : AddSection(sct);

	UnicodeString s;
	s.sprintf(_T("%s=%s"), key.c_str(), v.c_str());

	idx = klist->IndexOfName(key);
	if (idx>=0) {
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
//���C���t�H�[���̈ʒu�E�T�C�Y�𕜌�
//---------------------------------------------------------------------------
void UsrIniFile::LoadFormPos(TForm *frm, int w, int h,
	UnicodeString prfx)	//�L�[���̃v���t�B�b�N�X
{
	if (prfx.IsEmpty()) prfx = "Win";

	if (w>0) frm->Width  = ReadInteger(SCT_General, UAPP_T(prfx, "Width"),	w);
	if (h>0) frm->Height = ReadInteger(SCT_General, UAPP_T(prfx, "Height"),	h);
	frm->Left = ReadInteger(SCT_General, UAPP_T(prfx, "Left"), (Screen->Width - frm->Width)/2);
	frm->Top  = ReadInteger(SCT_General, UAPP_T(prfx, "Top"),  (Screen->Height - frm->Height)/2);

	if (Screen->MonitorCount==1) {
		//1��ʂ̏ꍇ�A��ʊO�ɏo�Ȃ��悤��
		if (frm->Left<0) frm->Left = 0;
		if (frm->Top<0)  frm->Top  = 0;
		if (frm->BoundsRect.Right>Screen->Width)   frm->Left = Screen->Width - frm->Width;
		if (frm->BoundsRect.Bottom>Screen->Height) frm->Top  = Screen->Height - frm->Height;
	}

	frm->WindowState = (TWindowState)ReadInteger(SCT_General, UAPP_T(prfx, "State"), (int)wsNormal);
}
//---------------------------------------------------------------------------
//���C���t�H�[���̈ʒu�E�T�C�Y��ۑ�
//---------------------------------------------------------------------------
void UsrIniFile::SaveFormPos(TForm *frm)
{
	if (frm->WindowState==wsMinimized) frm->WindowState = wsNormal;
	WriteIntGen(_T("WinState"),		(int)frm->WindowState);
	if (frm->WindowState==wsMaximized) frm->WindowState = wsNormal;

	WriteIntGen(_T("WinLeft"),		frm->Left);
	WriteIntGen(_T("WinTop"),		frm->Top);
	WriteIntGen(_T("WinWidth"),		frm->Width);
	WriteIntGen(_T("WinHeight"),	frm->Height);
}

//---------------------------------------------------------------------------
//�ʒu����Ǎ�
//---------------------------------------------------------------------------
void UsrIniFile::LoadPosInfo(TForm *frm, int x, int y, int w, int h, UnicodeString key)
{
	if (key.IsEmpty()) key = frm->Name;

	frm->Left = ReadInteger(SCT_General, UAPP_T(key, "Left"), x);
	frm->Top  = ReadInteger(SCT_General, UAPP_T(key, "Top"),  y);

	if ((frm->BorderStyle==bsSizeable || frm->BorderStyle==bsSizeToolWin) && w>0 && h>0) {
		int ww = ReadInteger(SCT_General, UAPP_T(key, "Width"),  w);
		if (ww<=0) ww = w;
		int wh = ReadInteger(SCT_General, UAPP_T(key, "Height"), h);
		if (wh<=0) wh = h;
		frm->Width	= ww;
		frm->Height = wh;
	}

	//1��ʂ̏ꍇ�A�����ȏオ��ʊO�������璆���ɖ߂�
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
		//�I�t�Z�b�g��K�p
		frm->Left = frm->Left + PosOffsetLeft;
		frm->Top  = frm->Top  + PosOffsetTop;
		if (OfsDlgList->IndexOf(frm->Name)==-1) OfsDlgList->Add(frm->Name);
	}
}

//---------------------------------------------------------------------------
//�ʒu���ۑ�
//---------------------------------------------------------------------------
void UsrIniFile::SavePosInfo(TForm *frm, UnicodeString key)
{
	if (key.IsEmpty()) key = frm->Name;

	bool ofs = (OfsDlgList->IndexOf(frm->Name)!=-1);
	WriteInteger(SCT_General, UAPP_T(key, "Left"), frm->Left - (ofs? PosOffsetLeft : 0));
	WriteInteger(SCT_General, UAPP_T(key, "Top"),  frm->Top  - (ofs? PosOffsetTop  : 0));

	if (frm->BorderStyle!=bsDialog) {
		WriteInteger(SCT_General, UAPP_T(key, "Width"),  frm->Width);
		WriteInteger(SCT_General, UAPP_T(key, "Height"), frm->Height);
	}
}

//---------------------------------------------------------------------------
//�O���b�h�̃J�������ݒ�
//�ό����ɁAcnt�̐����l��
//---------------------------------------------------------------------------
void UsrIniFile::LoadGridColWidth(TStringGrid *gp, int cnt, ...)
{
	va_list ap;
	va_start(ap, cnt);
	UnicodeString key;
	for (int i=0; i<cnt; i++) {
		int arg = va_arg(ap, int);
		if (i<gp->FixedCols)
			gp->ColWidths[i] = arg;
		else if (i<gp->ColCount && arg>0 && arg<1200)
			gp->ColWidths[i] = ReadInteger(gp->Name, key.sprintf(_T("ColWidth%u"), i), arg);
	}
	va_end(ap);
}
//---------------------------------------------------------------------------
//�O���b�h�̃J�������ۑ�
//---------------------------------------------------------------------------
void UsrIniFile::SaveGridColWidth(TStringGrid *gp)
{
	UnicodeString key;
	for (int i=0; i<gp->ColCount; i++)
		WriteInteger(gp->Name, key.sprintf(_T("ColWidth%u"), i), gp->ColWidths[i]);
}

//---------------------------------------------------------------------------
//ComboBox �̍��ڂ�Ǎ�
//---------------------------------------------------------------------------
void UsrIniFile::LoadComboBoxItems(
	TComboBox *cp,
	const _TCHAR *sct,	//�Z�N�V������
	int max_items,		//�ő區�ڐ�	(default = 20)
	bool del_quot)		//���p�����O��	(default = false)
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
//ComboBox �̍��ڂ�ۑ�
//---------------------------------------------------------------------------
void UsrIniFile::SaveComboBoxItems(
	TComboBox *cp,
	const _TCHAR *sct,	//�Z�N�V������
	int max_items)		//�ő區�ڐ�	(default = 20)
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
//���X�g���ڂ̓Ǎ�
//---------------------------------------------------------------------------
void UsrIniFile::LoadListItems(UnicodeString sct, TStringList *lst,
	int max_items,		//�ő區�ڐ�	(default = 20)	  0 �̏ꍇ������
	bool del_quot)		//���p�����O��	(default = true)  �ICSV���ڂ̏ꍇ�� false ��
{
	UnicodeString key;
	lst->Clear();
	for (int i=0; (max_items==0 || i<max_items); i++) {
		UnicodeString itm_str = ReadString(sct, key.sprintf(_T("Item%u"), i + 1), EmptyStr, del_quot);
		if (itm_str.IsEmpty()) break;
		lst->Add(itm_str);
	}
}
//---------------------------------------------------------------------------
//���X�g���ڂ̕ۑ�
//---------------------------------------------------------------------------
void UsrIniFile::SaveListItems(UnicodeString sct, TStringList *lst,
	int max_items)		//�ő區�ڐ�	(default = 20)	  0 �̏ꍇ������
{
	UnicodeString key;
	for (int i=0; (max_items==0 || i<max_items); i++) {
		key.sprintf(_T("Item%u"), i + 1);
		if (i<lst->Count)
			WriteString(sct, key, lst->Strings[i]);
		else
			if (!DeleteKey(sct, key)) break;
	}
}

//---------------------------------------------------------------------------
//�}�[�N�C���f�b�N�X�̍X�V
//---------------------------------------------------------------------------
void UsrIniFile::UpdateMarkIdxList()
{
	for (int i=0; i<MarkIdxList->Count; i++) delete (TStringList*)MarkIdxList->Objects[i];
	MarkIdxList->Clear();

	//�}�[�N�Z�N�V�����̃R�s�[���쐬
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
//�}�[�N���ڂ̑��݃`�F�b�N
//---------------------------------------------------------------------------
void UsrIniFile::CheckMarkItems()
{
	for (int i=0; i<MarkIdxList->Count; i++) {
		UnicodeString dnam = MarkIdxList->Strings[i];
		if (is_drive_accessible(dnam)) {
			//�f�B���N�g��
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
				else EraseSection(dnam);
			}
			//�A�[�J�C�u(�����̓`�F�b�N���Ȃ�)
			else if (!file_exists(dnam)) {
				EraseSection(dnam);
			}
		}
		//�A�N�Z�X�ł��Ȃ�UNC�p�X�̃Z�N�V�����͍폜
		else if (StartsStr("\\\\", dnam)) {
			EraseSection(dnam);
		}
	}

	UpdateMarkIdxList();
}

//---------------------------------------------------------------------------
//���ׂẴ}�[�N������
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
		else i++;
	}
}

//---------------------------------------------------------------------------
//�t�@�C���}�[�N/����
//---------------------------------------------------------------------------
bool UsrIniFile::FileMark(
	UnicodeString fnam,		//�t�@�C����
	int flag,				//-1 = ���]/ 0 = ����/ 1 = �}�[�N	(default = -1)
	UnicodeString memo)		//����								(default = EmptyStr)
{
	bool res	 = false;
	bool deleted = false;

	UnicodeString sct, itm;
	//�A�[�J�C�u
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//�ʏ�
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	int s_idx = SectionList->IndexOf(sct);
	TStringList *klist = NULL;
	if (s_idx>=0) {
		klist = (TStringList*)SectionList->Objects[s_idx];
		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;

		if (k_idx>=0 && flag!=1) {
			//����
			klist->Delete(k_idx);
			if (klist->Count==0) EraseSection(sct);
			deleted = true;
			res = false;
		}
	}

	if (flag==0)
		res = false;
	else if (!deleted) {
		//�}�[�N
		if (!klist) klist = AddSection(sct);

		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;

		itm.cat_sprintf(_T("\t%s\t%s"), memo.c_str(), FormatDateTime("yyyy/mm/dd hh:nn:ss", Now()).c_str());
		if (k_idx!=-1)
			klist->Strings[k_idx] = itm;
		else {
			klist->Add(itm);
			Modified = true;
		}
		res = true;
	}

	//�}�[�N�C���f�b�N�X�̍X�V
	UpdateMarkIdxList();

	return res;
}

//---------------------------------------------------------------------------
//�t�@�C�����}�[�N����Ă��邩�H
//---------------------------------------------------------------------------
bool UsrIniFile::IsMarked(UnicodeString fnam)
{
	bool ret = false;

	UnicodeString sct, itm;
	//�A�[�J�C�u
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//�ʏ�
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	int idx = MarkIdxList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)MarkIdxList->Objects[idx];
		int k_idx = -1;
		for (int i=0; i<klist->Count && k_idx==-1; i++)
			if (SameText(itm, get_pre_tab(klist->Strings[i]))) k_idx = i;
		ret = (k_idx != -1);
	}
	return ret;
}
//---------------------------------------------------------------------------
//�A�[�J�C�u���̃}�[�N�t�@�C�����擾
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::MarkedInArc(
	UnicodeString arc_file,	//�A�[�J�C�u�t�@�C����
	UnicodeString xlist)	//�Ή��g���q	(default = EmptyStr : ���ׂă}�b�`)
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
//�}�[�N�t�@�C���̃������擾
//---------------------------------------------------------------------------
UnicodeString UsrIniFile::GetMarkMemo(UnicodeString fnam)
{
	UnicodeString sct, itm;
	//�A�[�J�C�u
	if (contains_Slash(fnam)) {
		sct = split_tkn(fnam, '/');
		itm = fnam;
	}
	//�ʏ�
	else {
		sct = ExtractFilePath(fnam);
		itm = ExtractFileName(fnam);
	}

	UnicodeString memo;
	int idx = MarkIdxList->IndexOf(sct);
	if (idx>=0) {
		TStringList *klist = (TStringList*)MarkIdxList->Objects[idx];
		for (int i=0; i<klist->Count; i++) {
			UnicodeString lbuf = klist->Strings[i];	//�t�@�C���� \t ���� \t �ݒ����
			if (SameText(itm, split_pre_tab(lbuf))) {
				memo = get_pre_tab(lbuf);  break;
			}
		}
	}

	return Trim(memo);
}

//---------------------------------------------------------------------------
//INI�t�@�C���̍X�V
//---------------------------------------------------------------------------
bool UsrIniFile::UpdateFile(
	bool force)		//true = �ύX���Ȃ��Ă������I�ɕۑ�/ false = �ύX������Εۑ�
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
