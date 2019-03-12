//----------------------------------------------------------------------//
// �^�O���̌���															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "FindTag.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindTagForm *FindTagForm;

//---------------------------------------------------------------------------
__fastcall TFindTagForm::TFindTagForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::FormCreate(TObject *Sender)
{
	TagList    = new TStringList();
	ResListBuf = new TStringList();

	TagsScrPanel = new UsrScrollPanel(ListPanel, TagsListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	InfoScrPanel = new UsrScrollPanel(InfoPanel, InfoListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	InfoPanel->Height = IniFile->ReadIntGen(_T("FintTagInfHi"), 100);
	IsMigemo = IniFile->ReadBoolGen(_T("FindTagMigemo"));

	set_StdListBox(TagsListBox, LBTAG_OPT_LCPY);
	set_UsrScrPanel(TagsScrPanel);

	set_StdListBox(InfoListBox);
	set_UsrScrPanel(InfoScrPanel);

	ListSplitter->Color = col_Splitter;

	UnicodeString tit = "�^�O������";
	if (!FileName.IsEmpty()) tit.cat_sprintf(_T(" - %s"), ExtractFileName(FileName).c_str());
	Caption = tit;

	TagInfo = EmptyStr;

	IncSeaWord = EmptyStr;
	InpPanel->Font->Assign(ListFont);
	InpPanel->ClientHeight = get_FontHeight(ListFont, 4, 4);
	InpPanel->Color 	   = col_bgList;

	ClearTagList();
	std::unique_ptr<TStringList> fbuf(new TStringList());
	if (load_text_ex(TagFile, fbuf.get())!=0) {
		fbuf->Sort();
		UnicodeString lst_tag;
		UnicodeString rnam = ExtractFilePath(TagFile);
		int idx = -1;
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString lbuf = fbuf->Strings[i];
			if (StartsStr("!", lbuf)) continue;
			if (!FileName.IsEmpty() && !SameText(FileName, to_absolute_name(get_tkn(get_tkn_r(lbuf, '\t'), '\t'), rnam))) continue;

			UnicodeString tnam = get_pre_tab(lbuf);
			if (!SameStr(lst_tag, tnam)) {
				TStringList *lst = new TStringList();
				lst->Add(lbuf);
				idx = TagList->AddObject(tnam, (TObject*)lst);
				lst_tag = tnam;
			}
			else if (idx!=-1) {
				TStringList *lst = (TStringList *)TagList->Objects[idx];
				lst->Add(lbuf);
			}
		}

		UpdateList();
	}
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	ResListBuf->Clear();
	TagsListBox->Count = 0;
	ClearTagList();

	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("FintTagInfHi"),	InfoPanel->Height);
	IniFile->WriteBoolGen(_T("FindTagMigemo"),	IsMigemo);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::FormDestroy(TObject *Sender)
{
	ClearTagList();
	delete TagList;

	delete ResListBuf;
	delete TagsScrPanel;
	delete InfoScrPanel;
}

//---------------------------------------------------------------------------
void __fastcall TFindTagForm::ClearTagList()
{
	for (int i=0; i<TagList->Count; i++) delete (TStringList*)TagList->Objects[i];
	TagList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::UpdateList()
{
	InpPaintBox->Repaint();

	cursor_HourGlass();
	filter_List(TagList, ResListBuf, IncSeaWord, IsMigemo, true);	//AND/OR�����L��
	TListBox *lp = TagsListBox;
	lp->Canvas->Font->Assign(ListFont);
	int list_n = lp->ClientHeight / lp->ItemHeight + 1;
	MaxTagWidth = 0;
	for (int i=0; i<ResListBuf->Count && i<list_n; i++)
		MaxTagWidth = std::max(lp->Canvas->TextWidth(ResListBuf->Strings[i]), MaxTagWidth);
	cursor_Default();

	//���X�g�{�b�N�X�Ɋ��蓖��(���z)
	lp->Count = ResListBuf->Count;
	if (lp->Count>0) lp->ItemIndex = 0;
	lp->Invalidate();
	lp->SetFocus();
	TagsScrPanel->UpdateKnob();

	TagsListBoxClick(NULL);
}

//---------------------------------------------------------------------------
//���͗��̕`��
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::InpPaintBoxPaint(TObject *Sender)
{
	draw_InputPaintBox((TPaintBox*)Sender, (IsMigemo? "Migemo: " : "") + IncSeaWord);
}

//---------------------------------------------------------------------------
//�ꗗ�̕`��
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox *)Control;
	TCanvas  *cv = lp->Canvas;

	cv->Font->Assign(ListFont);
	cv->Brush->Color = State.Contains(odSelected)? col_selItem : col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + 4;
	int yp = Rect.Top + get_TopMargin2(cv);
	cv->Font->Color = (State.Contains(odSelected) && col_fgSelItem!=clNone)? col_fgSelItem : col_fgList;

	UnicodeString lbuf = lp->Items->Strings[Index];
	if (!IncSeaWord.IsEmpty()) {
		std::unique_ptr<TStringList> wlist(new TStringList());
		get_MatchWordList(lbuf, IncSeaWord, IsMigemo, false, true, wlist.get());
		EmphasisTextOut(lbuf, wlist.get(), cv, xp, yp);
	}
	else {
		cv->TextOut(xp, yp, lbuf);
	}

	TStringList *lst = (TStringList *)lp->Items->Objects[Index];
	lbuf = IntToStr(lst->Count);
	xp = (MaxTagWidth + cv->TextWidth("__9999") - cv->TextWidth(lbuf));
	cv->TextOut(xp, yp, lbuf);

	//�J�[�\��
	draw_ListCursor2(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<ResListBuf->Count)? ResListBuf->Strings[Index] : EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxDataObject(TWinControl *Control, int Index,
	TObject *&DataObject)
{
	DataObject = ResListBuf->Objects[Index];
}

//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxClick(TObject *Sender)
{
	InfoListBox->Clear();
	int idx = TagsListBox->ItemIndex;
	if (idx>=0 && idx<TagsListBox->Count) {
		InfoListBox->Items->Assign((TStringList *)TagsListBox->Items->Objects[idx]);
		InfoScrPanel->UpdateKnob();
	}
}

//---------------------------------------------------------------------------
//�m��
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxDblClick(TObject *Sender)
{
	int idx = TagsListBox->ItemIndex;
	if (idx!=-1) TagName = TagsListBox->Items->Strings[idx];
	ModalResult = !TagName.IsEmpty()? mrOk : mrCancel;
}

//---------------------------------------------------------------------------
//�^�O���ꗗ�ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);		if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);
	UnicodeString cmd_S  = KeyFuncList->Values["S:" + KeyStr];

	TListBox *lp = (TListBox*)Sender;
	int last_idx = lp->ItemIndex;
	bool handled = true;

	if (equal_ENTER(KeyStr))
		TagsListBoxDblClick(Sender);
	else if (equal_ESC(KeyStr))
		ModalResult = mrCancel;
	else if (update_IncSeaWord(IncSeaWord, KeyStr))
		UpdateList();
	else if (USAME_TI(cmd_S, "ClearIncKeyword")) {
		IncSeaWord = EmptyStr;
		UpdateList();
	}
	else if (USAME_TI(cmd_S, "MigemoMode")) {
		IsMigemo = (!IsMigemo && usr_Migemo->DictReady);
		UpdateList();
	}
	else if (USAME_TI(cmd_F, "TextViewer")) {
		TagCmd.USET_T("VIEW");
		TagsListBoxDblClick(Sender);
	}
	else if (USAME_TI(cmd_F, "FileEdit") || USAME_TI(cmd_V, "FileEdit")) {
		TagCmd.USET_T("EDIT");
		TagsListBoxDblClick(Sender);
	}
	else if (ExeCmdListBox(lp, cmd_F) || ExeCmdListBox(lp, cmd_V)) {
		if (USAME_TI(cmd_V, "ClipCopy")) {
			TagCmd.USET_T("COPY");
			ModalResult = mrOk;
		}
	}
	else if (USAME_TI(cmd_S, "IncSearchUp"))	ListBoxCursorUp(lp);
	else if (USAME_TI(cmd_S, "IncSearchDown"))	ListBoxCursorDown(lp);
	else if (MovListBoxFromFilter(lp, KeyStr))	;
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) show_PopupMenu(lp);
	else handled = false;

	if (last_idx!=lp->ItemIndex) TagsListBoxClick(NULL);

	if (handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagsListBoxKeyPress(TObject *Sender, System::WideChar &Key)
{
	//�C���N�������^���T�[�`�����
	if (_istalnum(Key) || Key==VK_SPACE || Key==VK_RETURN) Key = 0;
}

//---------------------------------------------------------------------------
//�t�@�C�������X�g�̕`��
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::InfoListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	int xp = Rect.Left + 4;
	int yp = Rect.Top + get_TopMargin(cv);

	cv->Brush->Color = col_bgList;
	cv->FillRect(Rect);

	UnicodeString fnam = get_pre_tab(get_post_tab(lp->Items->Strings[Index]));
	cv->Font->Color = get_ExtColor(get_extension(fnam));
	cv->TextOut(xp, yp, fnam);

	//�J�[�\��
	draw_ListCursor(lp, Rect, Index, State);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::InfoListBoxDblClick(TObject *Sender)
{
	int idx = InfoListBox->ItemIndex;
	if (idx>=0 && idx<InfoListBox->Count) {
		TagInfo = InfoListBox->Items->Strings[idx];
		ModalResult = !TagInfo.IsEmpty()? mrOk : mrCancel;
	}
}
//---------------------------------------------------------------------------
//�t�@�C�����ꗗ�ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::InfoListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);		if (KeyStr.IsEmpty()) return;
	UnicodeString cmd_F  = Key_to_CmdF(KeyStr);
	UnicodeString cmd_V  = Key_to_CmdV(KeyStr);

	TListBox *lp = (TListBox*)Sender;
	bool handled = true;
	if (equal_ENTER(KeyStr))
		InfoListBoxDblClick(Sender);
	else if (equal_ESC(KeyStr))
		ModalResult = mrCancel;
	else if (USAME_TI(cmd_F, "TextViewer")) {
		TagCmd.USET_T("VIEW");
		InfoListBoxDblClick(Sender);
	}
	else if (USAME_TI(cmd_F, "FileEdit") || USAME_TI(cmd_V, "FileEdit")) {
		TagCmd.USET_T("EDIT");
		InfoListBoxDblClick(Sender);
	}
	else if (MovListBoxFromFilter(lp, KeyStr))	;
	else handled = false;

	if (handled) Key = 0;
}

//---------------------------------------------------------------------------
//�r���A�[�Ń_�C���N�g�^�O�W�����v
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagViewActionExecute(TObject *Sender)
{
	TagCmd.USET_T("VIEW");
	if		(TagsListBox->Focused()) TagsListBoxDblClick(Sender);
	else if (InfoListBox->Focused()) InfoListBoxDblClick(Sender);
}
//---------------------------------------------------------------------------
//�G�f�B�^�Ń_�C���N�g�^�O�W�����v
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::TagJumpActionExecute(TObject *Sender)
{
	TagCmd.USET_T("EDIT");
	if		(TagsListBox->Focused()) TagsListBoxDblClick(Sender);
	else if (InfoListBox->Focused()) InfoListBoxDblClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::PopupItemUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = (TagsListBox->ItemIndex!=-1);
}

//---------------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::CopyActionExecute(TObject *Sender)
{
	if (TagsListBox->Focused())  ExeCmdListBox(TagsListBox, _T("ClipCopy"));

	TagCmd.USET_T("COPY");
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
//���ׂđI��
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::SelAllActionExecute(TObject *Sender)
{
	ListBoxSelectAll(TagsListBox);
}
//---------------------------------------------------------------------------
void __fastcall TFindTagForm::CopyActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Visible = TagsListBox->Focused();
	ap->Enabled = ap->Visible;
}

//---------------------------------------------------------------------------
void __fastcall TFindTagForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

