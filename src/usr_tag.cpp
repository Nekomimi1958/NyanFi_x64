//----------------------------------------------------------------------//
// タグ管理ユニット														//
//																		//
//----------------------------------------------------------------------//
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
//ファイル・インデックスを取得
//！TagRWLock 内で使用すること
//---------------------------------------------------------------------------
int TagManager::GetIndex(UnicodeString fnam)
{
	UnicodeString f_str = ExcludeTrailingPathDelimiter(fnam) + "\t";
	int idx = -1;

	for (int i=0; i<TagDataList->Count; i++) {
		if (StartsText(f_str, TagDataList->Strings[i])) {
			idx = i; break;
		}
	}

	return idx;
}

//---------------------------------------------------------------------------
//ファイルを更新
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
//タグ名リスト作成
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
//タイムスタンプが変化していたら再読み込み
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

	return false;
}

//---------------------------------------------------------------------------
//存在しない項目のタグデータを整理
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
//タグを取得
//---------------------------------------------------------------------------
UnicodeString TagManager::GetTags(UnicodeString fnam)
{
	TagRWLock->BeginRead();
	UnicodeString ret_str = GetDataTags(GetIndex(fnam));
	TagRWLock->EndRead();

	return ret_str;
}

//---------------------------------------------------------------------------
//タグの大小文字を既存のものに合わせ、空タグを除去
//既存になければ追加
//---------------------------------------------------------------------------
UnicodeString TagManager::NormTags(
	UnicodeString tags,
	bool sw_add)			//無ければ追加	(default = true)
{
	UnicodeString ret_str;

	for (;;) {
		UnicodeString tnam = split_tkn(tags, ';');
		if (!tnam.IsEmpty()) {
			int idx = TagNameList->IndexOf(tnam);
			if (idx==-1) {
				if (sw_add) TagNameList->Add(tnam);
			}
			else {
				tnam = TagNameList->Strings[idx];
			}
			cat_str_semicolon(ret_str, tnam);
		}
		if (tags.IsEmpty()) break;
	}

	if (sw_add) TagNameList->Sort();

	return ret_str;
}

//---------------------------------------------------------------------------
//指定ファイルのタグ情報を削除
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
//タグの設定
//---------------------------------------------------------------------------
void TagManager::SetTags(
	UnicodeString fnam,		//ファイル
	UnicodeString tags)		//追加タグ(;区切り)
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
//タグの追加
//---------------------------------------------------------------------------
void TagManager::AddTags(
	UnicodeString fnam,		//ファイル
	UnicodeString tags)		//追加タグ(;区切り)
{
	if (tags.IsEmpty()) return;

	fnam = ExcludeTrailingPathDelimiter(fnam);

	int idx = GetIndex(fnam);
	if (idx!=-1) {
		TagRWLock->BeginRead();
		UnicodeString tag_str = GetDataTags(idx);
		TagRWLock->EndRead();

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
//ファイル/ディレクトリ名の変更
//---------------------------------------------------------------------------
void TagManager::Rename(UnicodeString old_nam, UnicodeString new_nam)
{
	old_nam = ExcludeTrailingPathDelimiter(old_nam);
	new_nam = ExcludeTrailingPathDelimiter(new_nam);
	bool is_dir = ::PathIsDirectory(cv_ex_filename(new_nam).c_str());

	TagRWLock->BeginWrite();
	for (int i=0; i<TagDataList->Count; i++) {
		UnicodeString lbuf = TagDataList->Strings[i];
		UnicodeString fnam = get_pre_tab(lbuf);
		if (SameText(fnam, old_nam)) {
			TagDataList->Strings[i] = new_nam + "\t" + get_post_tab(lbuf);
		}
		else if (is_dir) {
			UnicodeString old_path = IncludeTrailingPathDelimiter(old_nam);
			if (StartsText(old_path, fnam)) {
				fnam.Delete(1, old_path.Length());
				fnam.Insert(IncludeTrailingPathDelimiter(new_nam), 1);
				TagDataList->Strings[i] = fnam + "\t" + get_post_tab(lbuf);
			}
		}
	}
	TagRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//タグのコピー
//---------------------------------------------------------------------------
void TagManager::Copy(UnicodeString from_nam, UnicodeString to_nam)
{
	UnicodeString tags = GetTags(from_nam);
	if (!tags.IsEmpty()) AddTags(to_nam, tags);
}

//---------------------------------------------------------------------------
//タグの改名
//---------------------------------------------------------------------------
int TagManager::RenTag(UnicodeString old_tag, UnicodeString new_tag)
{
	int idx_o = TagNameList->IndexOf(old_tag);
	int idx_c = TagColList->IndexOfName(old_tag);
	if (idx_o!=-1) {
		int idx_n = TagNameList->IndexOf(new_tag);
		//統合
		if (idx_n!=-1 && idx_n!=idx_o) {
			TagNameList->Delete(idx_o);
			if (idx_c!=-1) TagColList->Delete(idx_c);
		}
		//改名
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
//タグデータの削除
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

			if (upd_tags.IsEmpty()) {
				TagDataList->Delete(i);
			}
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
//タグチェックリストの初期化
//戻り値: タグの最大表示幅
//---------------------------------------------------------------------------
int TagManager::IniCheckList(
	TCheckListBox *lp,
	UnicodeString tnam,		//選択するタグ		(default = EmptyStr)
	bool count_sw)			//使用数をカウント	(default = false)
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
//リストチェックボックスからタグ文字列への反映
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

	if (end_sc) ret_str += ";";

	return ret_str;
}

//---------------------------------------------------------------------------
//タグの使用数をカウント
//---------------------------------------------------------------------------
void TagManager::CountTags(TCheckListBox *lp,
	bool clr_sw)	//tru = クリアのみ (default = false)
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
//指定タグにマッチするリストを取得
//---------------------------------------------------------------------------
int TagManager::GetMatchList(
	UnicodeString tags,	//タグ
	bool and_sw,		//AND検索
	TStringList *lst)	//結果リスト
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
//すべてのタグ項目のリストを取得
//---------------------------------------------------------------------------
int TagManager::GetAllList(TStringList *lst)
{
	TagRWLock->BeginRead();
	lst->Assign(TagDataList);
	TagRWLock->EndRead();

	return lst->Count;
}

//---------------------------------------------------------------------------
//ファイルにタグが付けられているか?
//---------------------------------------------------------------------------
bool TagManager::HasTag(UnicodeString fnam)
{
	TagRWLock->BeginRead();
	int idx = GetIndex(fnam);
	TagRWLock->EndRead();
	return (idx!=-1);
}

//---------------------------------------------------------------------------
//ファイルが指定タグにマッチするか?
//---------------------------------------------------------------------------
bool TagManager::Match(
	UnicodeString fnam,		//ファイル名
	UnicodeString ktags,	//タグ
	bool and_sw)			//AND検索
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
//タグ別カラーを取得
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
//タグ別カラーを設定
// tag : タグ名 (EmptyStr の場合 DefColor を設定)
// col : 色
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
//タグの描画
//  #: で始まる場合 AND/OR 付き
//---------------------------------------------------------------------------
void TagManager::DrawTags(
	UnicodeString tags,
	TCanvas *cv,
	int x, int y,		//表示位置
	TColor col_rev,		//指定背景色で反転表示	(default = clNone : 通常表示)
	bool blk_sw)		//ブロック記号を表示	(default = false)
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
	bool rev = (col_rev!=Graphics::clNone);

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
