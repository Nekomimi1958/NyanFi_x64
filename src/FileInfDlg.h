//----------------------------------------------------------------------//
// NyanFi																//
//  ファイル情報/項目の集計												//
//----------------------------------------------------------------------//
#ifndef FileInfDlgH
#define FileInfDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "usr_scrpanel.h"
#include "Global.h"

//---------------------------------------------------------------------------
class TFileInfoDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *CopyAction;
	TAction *CopyInfoAction;
	TAction *CopyValueAction;
	TAction *EmpItemAction;
	TAction *ImgPreviewAction;
	TAction *OpenLocAction;
	TAction *PropertyDlgAction;
	TAction *SelAllAction;
	TActionList *ActionList1;
	TListBox *InfListBox;
	TMenuItem *EmpItemItem;
	TMenuItem *ImgPreviewItem;
	TMenuItem *InfEditCopyItem;
	TMenuItem *InfEditSelectAllItem;
	TMenuItem *OpenDirItem;
	TMenuItem *PopCopyFileInfoItm;
	TMenuItem *PopCopyValueItem;
	TMenuItem *PropertyDlgItem;
	TMenuItem *Sep_i_1;
	TMenuItem *Sep_i_2;
	TMenuItem *Sep_i_3;
	TPanel *ListPanel;
	TPopupMenu *InfPopupMenu;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InfListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall InfListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InfListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall InfListBoxDblClick(TObject *Sender);
	void __fastcall CopyInfoActionExecute(TObject *Sender);
	void __fastcall CopyValueActionExecute(TObject *Sender);
	void __fastcall EmpItemActionExecute(TObject *Sender);
	void __fastcall EmpItemActionUpdate(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall SelAllActionExecute(TObject *Sender);
	void __fastcall OpenLocActionExecute(TObject *Sender);
	void __fastcall OpenLocActionUpdate(TObject *Sender);
	void __fastcall PropertyDlgActionExecute(TObject *Sender);
	void __fastcall PropertyDlgActionUpdate(TObject *Sender);
	void __fastcall ImgPreviewActionExecute(TObject *Sender);
	void __fastcall ImgPreviewActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	bool DlgInitialized;

	int Max_freq;
	int MaxColWd0;
	int MaxColWd1;
	int MaxColWd2;
	int FreqIndex;

	bool __fastcall UpdateInfo();

public:		// ユーザー宣言
	UsrScrollPanel *ListScrPanel;	//シンプルスクロールバー

	file_rec *FileRec;
	UnicodeString CmdStr;
	bool isAppInfo;
	bool isGitInfo;
	bool inhNxtPre;					//Next/PreFile 禁止
	bool inhImgPrv;					//イメージプレビューの禁止
	bool useImgPrv;

	bool isCalcItem;
	int  CsvCol;
	bool TopIsHeader;

	UnicodeString JumpFileName;

	TStringList *DataList;			//集計元データへのポインタ
	TStringList *ItemList;			//表示項目リスト

	__fastcall TFileInfoDlg(TComponent* Owner);
	int __fastcall ShowModalEx(UnicodeString fnam);
};
//---------------------------------------------------------------------------
extern PACKAGE TFileInfoDlg *FileInfoDlg;
//---------------------------------------------------------------------------
#endif
