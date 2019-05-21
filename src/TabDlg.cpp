//----------------------------------------------------------------------//
// NyanFi																//
//  タブの設定															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "TabDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTabSetDlg *TabSetDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TTabSetDlg::TTabSetDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	TabIndex = 0;
	CurPath1 = CurPath2 = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::FormShow(TObject *Sender)
{
	UnicodeString tit = "タブの設定";
	if (!TabGroupName.IsEmpty()) tit.cat_sprintf(_T(" - %s"), ExtractFileName(TabGroupName).c_str()); 
	Caption = tit;

	if (TabIndex<TabList->Count) {
		TStringDynArray itm_buf = get_csv_array(TabList->Strings[TabIndex], TABLIST_CSVITMCNT, true);
		CaptionEdit->Text  = itm_buf[2];
		IconEdit->Text	   = itm_buf[3];
		HomeDir1Edit->Text = itm_buf[4];
		HomeDir2Edit->Text = itm_buf[5];

		switch (itm_buf[6].ToIntDef(0)) {
		case 1:
			Work1RadioBtn->Checked = true;
			WorkListEdit->Text = EmptyStr;
			break;
		case 2:
			Work2RadioBtn->Checked = true;
			WorkListEdit->Text = itm_buf[7];
			break;
		default:
			Work0RadioBtn->Checked = true;
			WorkListEdit->Text = EmptyStr;
		}
	}
}

//---------------------------------------------------------------------------
//アイコンを参照
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::RefIconBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("タブのアイコン"), F_FILTER_ICO);
	UserModule->OpenDlgToEdit(IconEdit, true);
}
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::IconEditChange(TObject *Sender)
{
	usr_SH->draw_SmallIcon(to_absolute_name(IconEdit->Text), IcoImage, col_bgActTab);
}
//---------------------------------------------------------------------------
//ホームを参照
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::RefHomeBtnClick(TObject *Sender)
{
	TEdit *ep = (TEdit*)((((TComponent*)Sender)->Tag==1)? HomeDir2Edit : HomeDir1Edit);
	UnicodeString dnam = ep->Text;
	if (UserModule->SelectDirEx(_T("タブのホーム"), dnam)) ep->Text = to_path_name(dnam);
}
//---------------------------------------------------------------------------
//ワークリストを参照
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::RefWorkBtnClick(TObject *Sender)
{
	UserModule->PrepareOpenDlg(_T("ワークリストの指定"), F_FILTER_NWL, _T("*.nwl"), WorkListPath);
	UserModule->OpenDlgToEdit(WorkListEdit, true);
}

//---------------------------------------------------------------------------
//現在のディレクトリを設定
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::SetCurDirBtnClick(TObject *Sender)
{
	HomeDir1Edit->Text = CurPath1;
	HomeDir2Edit->Text = CurPath2;
}

//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::WorkRadioBtnClick(TObject *Sender)
{
	WorkListEdit->Color = Work2RadioBtn->Checked? scl_Window : col_Invalid;
}

//---------------------------------------------------------------------------
//確定
//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::OkButtonClick(TObject *Sender)
{
	if (TabIndex<TabList->Count) {
		TStringDynArray itm_buf = get_csv_array(TabList->Strings[TabIndex], TABLIST_CSVITMCNT, true);
		itm_buf[2] = CaptionEdit->Text;
		itm_buf[3] = IconEdit->Text;
		itm_buf[4] = HomeDir1Edit->Text;
		itm_buf[5] = HomeDir2Edit->Text;
		itm_buf[6] = Work1RadioBtn->Checked? "1" : Work2RadioBtn->Checked? "2" : "0";
		itm_buf[7] = Work2RadioBtn->Checked? WorkListEdit->Text : EmptyStr;
		TabList->Strings[TabIndex] = make_csv_rec_str(itm_buf);
	}
}

//---------------------------------------------------------------------------
void __fastcall TTabSetDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (equal_F1(get_KeyStr(Key, Shift))) {
		HtmlHelpTopic(_T(HELPTOPIC_FL) _T("#TabDlg"));
		Key = 0;
	}
}
//---------------------------------------------------------------------------

