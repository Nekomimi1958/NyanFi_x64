//----------------------------------------------------------------------//
// NyanFi																//
//  拡張子別一覧														//
//----------------------------------------------------------------------//
#ifndef FileExtDlgH
#define FileExtDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
struct ext_inf_rec {
	int 	f_count;		//ファイル数
	__int64 f_size;			//合計サイズ
	__int64 oc_size;		//占有サイズ
	__int64 av_size;		//平均サイズ
	UnicodeString f_names;	//タブ区切りのファイル名リスト(最大数=MAX_FLIST_CNT)
};

#define MAX_FLIST_CNT	1000

//---------------------------------------------------------------------------
class TFileExtensionDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *FextClipCopyAction;
	TAction *FextCsvAction;
	TAction *FextLogOutAction;
	TAction *FextMaskFindAction;
	TAction *FextSaveAction;
	TAction *FextTsvAction;
	TAction *PropertyAction;
	TAction *ShowFileInfoAction;
	TAction *SortCntAction;
	TAction *SortDirAction;
	TAction *SortFextAction;
	TAction *SortNameAction;
	TAction *SortSizeAction;
	TActionList *ActionList1;
	TButton *HiddenCanBtn;
	THeaderControl *FextInfHeader;
	THeaderControl *FileListHeader;
	TLabel *HintLabel;
	TListBox *FileListBox;
	TListBox *InfoListBox;
	TMenuItem *FextCopyItem;
	TMenuItem *FextLogOutItem;
	TMenuItem *FextSaveItem;
	TMenuItem *FileInfoItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *MaskFindItem;
	TMenuItem *RropertyItem;
	TMenuItem *SaveCsvItem;
	TMenuItem *SaveTsvItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TMenuItem *Sep_4;
	TMenuItem *SortCntItem;
	TMenuItem *SortDirItem;
	TMenuItem *SortExtItem;
	TMenuItem *SortNameItem;
	TMenuItem *SortSizeItem;
	TPanel *FilePanel;
	TPanel *HintPanel;
	TPanel *InfoPanel;
	TPanel *ListPanel;
	TPopupMenu *PopupMenu1;
	TSplitter *ListSplitter;
	TStatusBar *FextInfBar;
	TStatusBar *FileInfBar;

	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FextHeaderDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall FextInfHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall InfoListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall InfoListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InfoListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall InfoListBoxClick(TObject *Sender);
	void __fastcall FextInfBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall FileListHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall FileListHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall FileListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall FileListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FileListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FileListBoxEnter(TObject *Sender);
	void __fastcall FileListBoxExit(TObject *Sender);
	void __fastcall FileListBoxClick(TObject *Sender);
	void __fastcall FextClipCopyActionExecute(TObject *Sender);
	void __fastcall FextLogOutActionExecute(TObject *Sender);
	void __fastcall FextSaveActionExecute(TObject *Sender);
	void __fastcall FextCsvActionUpdate(TObject *Sender);
	void __fastcall FextOutActionUpdate(TObject *Sender);
	void __fastcall SortFextActionUpdate(TObject *Sender);
	void __fastcall SortFextActionExecute(TObject *Sender);
	void __fastcall FextMaskFindActionExecute(TObject *Sender);
	void __fastcall SortFileActionExecute(TObject *Sender);
	void __fastcall SortFileActionUpdate(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall ListSplitterMoved(TObject *Sender);
	void __fastcall ShowFileInfoActionExecute(TObject *Sender);

private:	// ユーザー宣言
	TStringList *FextInfoList;
	TStringList *FileItemList;
	int TotalFileCount;
	__int64 TotalSize;
	__int64 AverageSize;
	int CurFileCount;
	int ClusterSize;

	int FExtSctWd, FCntSctWd, SizeSctWd, PercSctWd;
	int ExtSortMode, FileSortMode;
	int AccDeniedCnt;
	UnicodeString ErrMesage;

	void __fastcall WmFormShowed(TMessage &msg);
	bool __fastcall GetInfo(UnicodeString pnam);
	void __fastcall GetResult(TStringList *lst, int mode = 0);
	void __fastcall SortListX(int mode = -1);
	void __fastcall SortListF(int mode = -1);
	void __fastcall IniSearchList(TListBox *lp, UnicodeString ptn);
	UnicodeString __fastcall GetCurFileItem(int idx = -1);

public:		// ユーザー宣言
	UsrScrollPanel *InfoScrPanel;
	UsrScrollPanel *FileScrPanel;

	UnicodeString PathName;
	UnicodeString FileName;
	UnicodeString FextMask;

	__fastcall TFileExtensionDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TFileExtensionDlg *FileExtensionDlg;
//---------------------------------------------------------------------------
#endif
