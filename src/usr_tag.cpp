//----------------------------------------------------------------------//
// �^�O�Ǘ����j�b�g														//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <System.StrUtils.hpp>
#include "usr_file_ex.h"
#include "usr_tag.h"

//---------------------------------------------------------------------------
TagManager *usr_TAG = NULL;

//---------------------------------------------------------------------------
TagManager::TagManager(UnicodeString fnam)
{
	TagRWLock	= new TMultiReadExclusiveWriteSynchronizer();
	TagDataList = new TStringList();
	TagNameList = new TStringList();
	TagColList	= new TStringList();

	DefColor = clWhite;
	SortTags = false;

	FileName = fnam;
	if (FileExists(FileName)) {
		TagDataTime = get_file_age(FileName);

		TagRWLock->BeginWrite();
		TagDataList->LoadFromFile(FileName);
		TagRWLock->EndWrite();

		Modified = false;
		MakeTagNameList();
	}
}
//---------------------------------------------------------------------------
TagManager::~TagManager()
{
	delete TagRWLock;
	delete TagDataList;
	delete TagNameList;
	delete TagColList;
}

//---------------------------------------------------------------------------
//�t�@�C���E�C���f�b�N�X���擾
//�ITagRWLock ���Ŏg�p���邱��
//---------------------------------------------------------------------------
int TagManager::GetIndex(UnicodeString fnam)
{
	UnicodeString f_str = fnam + "\t";
	int idx = -1;

	for (int i=0; i<TagDataList->Count; i++) {
		if (StartsText(f_str, TagDataList->Strings[i])) {
			idx = i; break;
		}
	}

	return idx;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�V
//---------------------------------------------------------------------------
bool TagManager::UpdateFile()
{
	try {
		if (Modified) {
			TagRWLock->BeginRead();
			TagDataList->SaveToFile(FileName, TEncoding::UTF8);
			TagRWLock->EndRead();

			TagDataTime = get_file_age(FileName);
			Modified = false;
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�^�O�����X�g�쐬
//---------------------------------------------------------------------------
void TagManager::MakeTagNameList()
{
	TagRWLock->BeginRead();
	for (int i=0; i<TagDataList->Count; i++) {
		TStringDynArray tag_lst = SplitString(GetDataTags(i), ';');
		for (int j=0; j<tag_lst.Length; j++) {
			UnicodeString tag = tag_lst[j];
			if (TagNameList->IndexOf(tag)==-1) TagNameList->Add(tag);
		}
	}
	TagRWLock->EndRead();

	TagNameList->Sort();
}

//---------------------------------------------------------------------------
//�^�C���X�^���v���ω����Ă�����ēǂݍ���
//---------------------------------------------------------------------------
bool TagManager::Recycle()
{
	if (!FileExists(FileName)) return false;

	TDateTime ft = get_file_age(FileName);
	if (TagDataTime!=ft) {
		TagDataTime = ft;

		TagRWLock->BeginWrite();
		TagDataList->LoadFromFile(FileName);
		TagRWLock->EndWrite();

		Modified = false;
		MakeTagNameList();
		return true;
	}
	else
		return false;
}

//---------------------------------------------------------------------------
//���݂��Ȃ����ڂ̃^�O�f�[�^�𐮗�
//---------------------------------------------------------------------------
int TagManager::TrimData()
{
	int cnt = 0;
	int i = 0;

	TagRWLock->BeginWrite();
	while (i<TagDataList->Count) {
		if (!file_exists(get_pre_tab(TagDataList->Strings[i]))) {
			TagDataList->Delete(i); cnt++;
		}
		else i++;
	}
	TagRWLock->EndWrite();

	if (cnt>0) Modified = true;
	UpdateFile();

	return cnt;
}

//---------------------------------------------------------------------------
//�^�O���擾
//---------------------------------------------------------------------------
UnicodeString TagManager::GetTags(UnicodeString fnam)
{
	TagRWLock->BeginRead();
	UnicodeString ret_str = GetDataTags(GetIndex(fnam));
	TagRWLock->EndRead();

	return ret_str;
}

//---------------------------------------------------------------------------
//�^�O�̑召�����������̂��̂ɍ��킹�A��^�O������
//�����ɂȂ���Βǉ�
//---------------------------------------------------------------------------
UnicodeString TagManager::NormTags(
	UnicodeString tags,
	bool sw_add)			//������Βǉ�	(default = true)
{
	UnicodeString ret_str;

	for (;;) {
		UnicodeString tnam = split_tkn(tags, ';');
		if (!tnam.IsEmpty()) {
			int idx = TagNameList->IndexOf(tnam);
			if (idx==-1) {
				if (sw_add) TagNameList->Add(tnam);
			}
			else
				tnam = TagNameList->Strings[idx];
			cat_str_semicolon(ret_str, tnam);
		}
		if (tags.IsEmpty()) break;
	}

	if (sw_add) TagNameList->Sort();

	return ret_str;
}

//---------------------------------------------------------------------------
//�w��t�@�C���̃^�O�����폜
//---------------------------------------------------------------------------
void TagManager::DelItem(UnicodeString fnam)
{
	TagRWLock->BeginWrite();
	int idx = GetIndex(fnam);
	if (idx!=-1) {
		TagDataList->Delete(idx);
		Modified = true;
	}
	TagRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//�^�O�̐ݒ�
//---------------------------------------------------------------------------
void TagManager::SetTags(
	UnicodeString fnam,		//�t�@�C��
	UnicodeString tags)		//�ǉ��^�O(;��؂�)
{
	fnam = ExcludeTrailingPathDelimiter(fnam);

	TagRWLock->BeginWrite();
	int idx = GetIndex(fnam);
	if (idx!=-1) {
		if (!tags.IsEmpty())
			TagDataList->Strings[idx] = fnam + "\t" + tags;
		else
			TagDataList->Delete(idx);
	}
	else if (!tags.IsEmpty()) {
		TagDataList->Add(fnam + "\t" + tags);
	}
	TagRWLock->EndWrite();

	Modified = true;
}

//---------------------------------------------------------------------------
//�^�O�̒ǉ�
//---------------------------------------------------------------------------
void TagManager::AddTags(
	UnicodeString fnam,		//�t�@�C��
	UnicodeString tags)		//�ǉ��^�O(;��؂�)
{
	if (tags.IsEmpty()) return;

	fnam = ExcludeTrailingPathDelimiter(fnam);

	int idx = GetIndex(fnam);
	if (idx!=-1) {
		UnicodeString   tag_str = GetDataTags(idx);
		TStringDynArray tag_lst = SplitString(tag_str, ';');
		TStringDynArray add_lst = SplitString(tags, ';');
		for (int j=0; j<add_lst.Length; j++) {
			if (add_lst[j].IsEmpty()) continue;
			bool found = false;
			for (int k=0; k<tag_lst.Length && !found; k++) {
				found = SameText(add_lst[j], tag_lst[k]);
			}
			if (!found) cat_str_semicolon(tag_str, add_lst[j]);
		}

		TagRWLock->BeginWrite();
		TagDataList->Strings[idx] = fnam + "\t" + tag_str;
		TagRWLock->EndWrite();
	}
	else {
		TagRWLock->BeginWrite();
		TagDataList->Add(fnam + "\t" + tags);
		TagRWLock->EndWrite();
	}

	Modified = true;
}

//---------------------------------------------------------------------------
//�t�@�C��/�f�B���N�g�����̕ύX
//---------------------------------------------------------------------------
void TagManager::Rename(UnicodeString old_nam, UnicodeString new_nam)
{
	old_nam = ExcludeTrailingPathDelimiter(old_nam);
	new_nam = ExcludeTrailingPathDelimiter(new_nam);

	if (::PathIsDirectory(cv_ex_filename(new_nam).c_str())) {
		old_nam = IncludeTrailingPathDelimiter(old_nam);
		new_nam = IncludeTrailingPathDelimiter(new_nam);
	}
	else {
		old_nam += "\t";
		new_nam += "\t";
	}

	TagRWLock->BeginWrite();
	for (int i=0; i<TagDataList->Count; i++) {
		UnicodeString lbuf = TagDataList->Strings[i];
		if (StartsText(old_nam, lbuf)) TagDataList->Strings[i] = new_nam + lbuf.Delete(1, old_nam.Length());
	}
	TagRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//�^�O�̉���
//---------------------------------------------------------------------------
int TagManager::RenTag(UnicodeString old_tag, UnicodeString new_tag)
{
	int idx_o = TagNameList->IndexOf(old_tag);
	int idx_c = TagColList->IndexOfName(old_tag);
	if (idx_o!=-1) {
		int idx_n = TagNameList->IndexOf(new_tag);
		//����
		if (idx_n!=-1 && idx_n!=idx_o) {
			TagNameList->Delete(idx_o);
			if (idx_c!=-1) TagColList->Delete(idx_c);
		}
		//����
		else {
			TagNameList->Strings[idx_o] = new_tag;
			if (idx_c!=-1) TagColList->Strings[idx_c] = new_tag + "=" + TagColList->ValueFromIndex[idx_c];
		}
		TagNameList->Sort();
	}

	TagRWLock->BeginWrite();
	std::unique_ptr<TStringList> tlst(new TStringList());
	int cnt = 0;
	for (int i=0; i<TagDataList->Count; i++) {
		UnicodeString fnam;
		UnicodeString tags = GetDataTags(i, fnam);
		tlst->Text = ReplaceStr(tags, ";", "\n") + "\n";
		int idx_o = tlst->IndexOf(old_tag);
		if (idx_o!=-1) {
			int idx_n = tlst->IndexOf(new_tag);
			if (idx_n!=-1 && idx_n!=idx_o)
				tlst->Delete(idx_o);
			else
				tlst->Strings[idx_o] = new_tag;

			UnicodeString new_tags;
			for (int j=0; j<tlst->Count; j++) cat_str_semicolon(new_tags, tlst->Strings[j]);
			TagDataList->Strings[i] = fnam + "\t" + new_tags;
			cnt++;
		}
	}
	TagRWLock->EndWrite();

	if (cnt>0) Modified = true;
	UpdateFile();

	return cnt;
}

//---------------------------------------------------------------------------
//�^�O�f�[�^�̍폜
//---------------------------------------------------------------------------
int TagManager::DelTagData(UnicodeString tag)
{
	int idx = TagNameList->IndexOf(tag);
	if (idx==-1) return 0;

	TagNameList->Delete(idx);

	TagRWLock->BeginWrite();
	std::unique_ptr<TStringList> tlst(new TStringList());
	int cnt = 0;
	int i = 0;
	while (i<TagDataList->Count) {
		UnicodeString fnam;
		UnicodeString tags = GetDataTags(i, fnam);
		tlst->Text = ReplaceStr(tags, ";", "\n") + "\n";
		int idx = tlst->IndexOf(tag);
		if (idx!=-1) {
			tlst->Delete(idx);
			cnt++;

			UnicodeString upd_tags;
			for (int j=0; j<tlst->Count; j++) cat_str_semicolon(upd_tags, tlst->Strings[j]);

			if (upd_tags.IsEmpty())
				TagDataList->Delete(i);
			else {
				TagDataList->Strings[i] = fnam + "\t" + upd_tags;
				i++;
			}
		}
		else i++;
	}
	TagRWLock->EndWrite();

	if (cnt>0) Modified = true;
	UpdateFile();

	return cnt;
}

//---------------------------------------------------------------------------
//�^�O�`�F�b�N���X�g�̏�����
//�߂�l: �^�O�̍ő�\����
//---------------------------------------------------------------------------
int TagManager::IniCheckList(
	TCheckListBox *lp,
	UnicodeString tnam,		//�I������^�O		(default = EmptyStr)
	bool count_sw)			//�g�p�����J�E���g	(default = false)
{
	lp->Items->Assign(TagNameList);

	if (count_sw) CountTags(lp);

	int max_wd = 0;
	for (int i=0; i<lp->Count; i++) {
		lp->Checked[i] = false;
		max_wd = std::max(max_wd, lp->Canvas->TextWidth(lp->Items->Strings[i]));
	}

	if (!tnam.IsEmpty()) lp->ItemIndex = lp->Items->IndexOf(tnam);

	return max_wd;
}

//---------------------------------------------------------------------------
//���X�g�`�F�b�N�{�b�N�X����^�O������ւ̔��f
//---------------------------------------------------------------------------
UnicodeString TagManager::CheckToTags(TCheckListBox *lp, UnicodeString tags)
{
	bool end_sc = EndsStr(";", tags);

	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = ReplaceStr(tags, ";", "\n") + "\n";

	for (int i=0; i<lp->Count; i++) {
		UnicodeString tnam = lp->Items->Strings[i];
		int idx = lst->IndexOf(tnam);
		if (idx==-1 && lp->Checked[i])
			lst->Add(tnam);
		else if (idx!=-1 && !lp->Checked[i])
			lst->Delete(idx);
	}

	UnicodeString ret_str, tag1;
	for (int i=0; i<lst->Count; i++) {
		UnicodeString tnam = lst->Strings[i];
		int idx = lp->Items->IndexOf(tnam);
		if (idx!=-1)
			cat_str_semicolon(ret_str, lp->Items->Strings[idx]);
		else
			cat_str_semicolon(tag1, tnam);
	}

	cat_str_semicolon(ret_str, tag1);

	if (end_sc) ret_str.UCAT_T(";");

	return ret_str;
}

//---------------------------------------------------------------------------
//�^�O�̎g�p�����J�E���g
//---------------------------------------------------------------------------
void TagManager::CountTags(TCheckListBox *lp,
	bool clr_sw)	//tru = �N���A�̂� (default = false)
{
	for (int i=0; i<lp->Count; i++) lp->Items->Objects[i] = (TObject*)0;

	if (!clr_sw) {
		TagRWLock->BeginRead();
		for (int i=0; i<TagDataList->Count; i++) {
			UnicodeString tags = GetDataTags(i);
			while (!tags.IsEmpty()) {
				int idx = lp->Items->IndexOf(split_tkn(tags, ';'));
				if (idx!=-1) lp->Items->Objects[idx] = (TObject*)((NativeInt)lp->Items->Objects[idx] + 1);
			}
		}
		TagRWLock->EndRead();
	}

	lp->Repaint();
}

//---------------------------------------------------------------------------
//�w��^�O�Ƀ}�b�`���郊�X�g���擾
//---------------------------------------------------------------------------
int TagManager::GetMatchList(
	UnicodeString tags,	//�^�O
	bool and_sw,		//AND����
	TStringList *lst)	//���ʃ��X�g
{
	TagRWLock->BeginRead();
	for (int i=0; i<TagDataList->Count; i++) {
		UnicodeString lbuf = TagDataList->Strings[i];
		TStringDynArray tag_lst = split_strings_semicolon(get_post_tab(lbuf));
		TStringDynArray key_lst = split_strings_semicolon(tags);

		bool ok = and_sw;
		if (and_sw) {
			for (int j=0; j<key_lst.Length && ok; j++) {
				bool found = false;
				for (int k=0; k<tag_lst.Length && !found; k++) found = SameText(key_lst[j], tag_lst[k]);
				ok = found;
			}
		}
		else {
			for (int j=0; j<key_lst.Length && !ok; j++) {
				bool found = false;
				for (int k=0; k<tag_lst.Length && !found; k++) found = SameText(key_lst[j], tag_lst[k]);
				ok = found;
			}
		}

		if (ok) lst->Add(lbuf);
	}
	TagRWLock->EndRead();

	return lst->Count;
}
//---------------------------------------------------------------------------
//���ׂẴ^�O���ڂ̃��X�g���擾
//---------------------------------------------------------------------------
int TagManager::GetAllList(TStringList *lst)
{
	TagRWLock->BeginRead();
	lst->Assign(TagDataList);
	TagRWLock->EndRead();

	return lst->Count;
}

//---------------------------------------------------------------------------
//�t�@�C���Ƀ^�O���t�����Ă��邩?
//---------------------------------------------------------------------------
bool TagManager::HasTag(UnicodeString fnam)
{
	TagRWLock->BeginRead();
	int idx = GetIndex(fnam);
	TagRWLock->EndRead();
	return (idx!=-1);
}

//---------------------------------------------------------------------------
//�t�@�C�����w��^�O�Ƀ}�b�`���邩?
//---------------------------------------------------------------------------
bool TagManager::Match(
	UnicodeString fnam,		//�t�@�C����
	UnicodeString ktags,	//�^�O
	bool and_sw)			//AND����
{
	TagRWLock->BeginRead();
	UnicodeString ftags = GetDataTags(GetIndex(fnam));
	TagRWLock->EndRead();
	if (ftags.IsEmpty() || ktags.IsEmpty()) return false;

	TStringDynArray tag_lst = split_strings_semicolon(ftags);
	TStringDynArray key_lst = split_strings_semicolon(ktags);

	bool ok = and_sw;
	if (and_sw) {
		for (int j=0; j<key_lst.Length && ok; j++) {
			bool found = false;
			for (int k=0; k<tag_lst.Length && !found; k++) found = SameText(key_lst[j], tag_lst[k]);
			ok = found;
		}
	}
	else {
		for (int j=0; j<key_lst.Length && !ok; j++) {
			bool found = false;
			for (int k=0; k<tag_lst.Length && !found; k++) found = SameText(key_lst[j], tag_lst[k]);
			ok = found;
		}
	}

	return ok;
}

//---------------------------------------------------------------------------
//�^�O�ʃJ���[���擾
//---------------------------------------------------------------------------
TColor TagManager::GetColor(UnicodeString tag, TColor col_def)
{
	TColor col = Graphics::clNone;
	if (!tag.IsEmpty()) {
		UnicodeString vstr = TagColList->Values[tag];
		if (!vstr.IsEmpty()) col = (TColor)vstr.ToIntDef(Graphics::clNone);
	}
	if (col==Graphics::clNone) col = col_def;
	return col;
}
//---------------------------------------------------------------------------
//�^�O�ʃJ���[��ݒ�
// tag : �^�O�� (EmptyStr �̏ꍇ DefColor ��ݒ�)
// col : �F
//---------------------------------------------------------------------------
void TagManager::SetColor(UnicodeString tag, TColor col)
{
	if (tag.IsEmpty()) {
		DefColor = col;
		return;
	}

	UnicodeString tmp;
	int idx = TagColList->IndexOfName(tag);
	if (idx!=-1)
		TagColList->ValueFromIndex[idx] = tmp.sprintf(_T("%d"), col);
	else 
		TagColList->Add(tmp.sprintf(_T("%s=%d"), tag.c_str(), col));
}
//---------------------------------------------------------------------------
//�^�O�̕`��
//  #: �Ŏn�܂�ꍇ AND/OR �t��
//---------------------------------------------------------------------------
void TagManager::DrawTags(
	UnicodeString tags,
	TCanvas *cv,
	int x, int y,		//�\���ʒu
	TColor col_rev,		//�w��w�i�F�Ŕ��]�\��	(default = clNone : �ʏ�\��)
	bool blk_sw)		//�u���b�N�L����\��	(default = false)
{
	UnicodeString tag_hd = "#:";
	UnicodeString tag_opt;
	if (remove_top_s(tags, tag_hd)) {
		cv->TextOut(x, y, tag_hd);
		x += cv->TextWidth(tag_hd + " ");
		tag_opt = tags.Pos('|')? "(OR)" : tags.Pos(';')? "(AND)" : "";
		tags = ReplaceStr(tags, "|", ";");
	}

	std::unique_ptr<TStringList> tlst(new TStringList());
	UnicodeString tag = Trim(split_tkn(tags, ';'));
	while (!tag.IsEmpty()) {
		tlst->Add(tag);
		tag = Trim(split_tkn(tags, ';'));
	}

	if (SortTags) tlst->Sort();

	TColor org_fg = cv->Font->Color;
	TColor org_bg = cv->Brush->Color;
	bool rev = (col_rev!=clNone);

	for (int i=0; i<tlst->Count; i++) {
		UnicodeString tag = tlst->Strings[i];
		TColor col_tag = GetColor(tag, DefColor);

		if (i==0 && blk_sw) {
			UnicodeString blk = _T("\u2588 ");
			cv->Font->Color = col_tag;
			cv->TextOut(x, y, blk);
			x += cv->TextWidth(blk);
		}

		if (rev) {
			cv->Font->Color  = col_rev;
			cv->Brush->Color = col_tag;
		}
		else {
			cv->Font->Color = col_tag;
		}

		cv->TextOut(x, y, tag);
		x += cv->TextWidth(tag + " ");
	}

	cv->Font->Color  = org_fg;
	cv->Brush->Color = org_bg;

	if (!tag_opt.IsEmpty()) cv->TextOut(x, y, tag_opt);
}
//---------------------------------------------------------------------------
