//----------------------------------------------------------------------//
// NyanFi																//
//  アプリケーション一覧												//
//----------------------------------------------------------------------//
#ifndef AppDlgH
#define AppDlgH
//---------------------------------------------------------------------------
#include <psapi.h>
#include <winternl.h>
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
typedef BOOL (WINAPI *FUNC_GetProcessMemoryInfo)(HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD);

#ifndef NT_ERROR
#define NT_ERROR(Status) ((((ULONG)(Status)) >> 30) == 3)
#endif

struct RTL_USER_PROCESS_PARAMETERS_I {
	BYTE  Reserved1[16];
	PVOID Reserved2[10];
	UNICODE_STRING ImagePathName;
	UNICODE_STRING CommandLine;
};

struct PEB_INTERNAL {
	BYTE  InheritedAddressSpace;
	BYTE  ReadImageFileExecOptions;
	BYTE  BeingDebugged;
	BYTE  Spare;
	PVOID Mutant;
	PVOID ImageBaseAddress;
	PPEB_LDR_DATA Ldr;
	PRTL_USER_PROCESS_PARAMETERS	ProcessParameters;
	BYTE  Reserved4[104];
	PVOID Reserved5[52];
	PPS_POST_PROCESS_INIT_ROUTINE	PostProcessInitRoutine;
	BYTE  Reserved6[128];
	PVOID Reserved7[1];
	ULONG SessionId;
};

//---------------------------------------------------------------------------
#define ISWOW64_STR	_T(" \u208d\u2083\u2082\u208e")

//---------------------------------------------------------------------------
class AppWinInf
{
public:
	HWND WinHandle;
	UnicodeString WinText;
	UnicodeString ClassName;
	UnicodeString ClassName2;
	UnicodeString FileName;
	UnicodeString Caption;
	UnicodeString CmdParam;
	UnicodeString IconPng;

	int  PID;
	int  TID;

	TIcon *Icon;
	TPngImage *PngImg;

	bool isNyan;
	bool isWow64;
	bool isUWP;
	bool topMost;
	bool Exist;
	bool toClose;		//終了要求
	bool isNoRes;		//無応答かも

	SIZE_T mem_WS;
	SIZE_T mem_pWS;
	SIZE_T mem_PF;
	SIZE_T mem_pPF;

	LONG  win_xstyle;
	int win_wd, win_hi;
	int win_left, win_top;

	TDateTime StartTime;

	HTHUMBNAIL hThumb;

	AppWinInf();
	~AppWinInf();
};
//---------------------------------------------------------------------------
class TAppWinList : public TList
{
private:
	virtual void __fastcall Notify(void *Ptr, TListNotification Action);

	AppWinInf * __fastcall Get(int Index)
	{
	 	return (AppWinInf*)(TList::Get(Index));
	}

	void __fastcall Put(int Index, AppWinInf *Item)
	{
		TList::Put(Index, Item);
	}

public:
	__property AppWinInf *Items[int Index] = {read=Get, write=Put};

	__fastcall TAppWinList();
};

//---------------------------------------------------------------------------
// TAppListDlg
//---------------------------------------------------------------------------
class TAppListDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AppInfoAction;
	TAction *FitToFileListAction;
	TAction *JumpLinkedAction;
	TAction *MaximizeAction;
	TAction *MinimizeAction;
	TAction *RestoreAction;
	TAction *ShowCmdParamAction;
	TAction *SortByIconAction;
	TAction *SortByRemAction;
	TAction *TopMostAction;
	TActionList *ActionList1;
	TListBox *AppListBox;
	TListBox *LaunchListBox;
	TMenuItem *AppInfoItem;
	TMenuItem *CloseItem;
	TMenuItem *CloseSameItem;
	TMenuItem *CopyNameItem;
	TMenuItem *DesktopItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *FitToFileListItem;
	TMenuItem *JumpExeItem;
	TMenuItem *JumpLaunchDirItem;
	TMenuItem *JumpLinkItem;
	TMenuItem *LaunchLeftItem;
	TMenuItem *LaunchPosItem;
	TMenuItem *LaunchRightItem;
	TMenuItem *MaxWinItem;
	TMenuItem *MinWinItem;
	TMenuItem *PropertyItem;
	TMenuItem *RstWinItem;
	TMenuItem *SelLaunhDirItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TMenuItem *Sep_l_1;
	TMenuItem *Sep_l_2;
	TMenuItem *Sep_l_3;
	TMenuItem *ShowCmdParamItem;
	TMenuItem *SortByIconItem;
	TMenuItem *SortByRemItem;
	TMenuItem *TerminateItem;
	TMenuItem *TopMostItem;
	TMenuItem *UpdateIcoItem;
	TPaintBox *InpPaintBox;
	TPanel *AppListPanel;
	TPanel *AppPanel;
	TPanel *DirPanel;
	TPanel *LaunchPanel;
	TPanel *MainPanel;
	TPanel *ViewPanel;
	TPopupMenu *PopupMenu1;
	TPopupMenu *PopupMenu2;
	TSplitter *LRSplitter;
	TSplitter *ViewSplitter;
	TStatusBar *StatusBar1;
	TTimer *Timer1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AppListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall AppListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AppListBoxDblClick(TObject *Sender);
	void __fastcall AppListBoxClick(TObject *Sender);
	void __fastcall AppListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall AppListBoxEnter(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall CloseItemClick(TObject *Sender);
	void __fastcall TerminateItemClick(TObject *Sender);
	void __fastcall JumpExeItemClick(TObject *Sender);
	void __fastcall CopyNameItemClick(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall DesktopItemClick(TObject *Sender);
	void __fastcall LaunchListBoxDblClick(TObject *Sender);
	void __fastcall LaunchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LaunchListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall LaunchListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall LaunchListBoxClick(TObject *Sender);
	void __fastcall LaunchListBoxEnter(TObject *Sender);
	void __fastcall SelLaunhDirItemClick(TObject *Sender);
	void __fastcall JumpLaunchDirItemClick(TObject *Sender);
	void __fastcall DirPanelDblClick(TObject *Sender);
	void __fastcall PropertyItemClick(TObject *Sender);
	void __fastcall LaunchPosItemClick(TObject *Sender);
	void __fastcall PopupMenu2Popup(TObject *Sender);
	void __fastcall UpdateIcoItemClick(TObject *Sender);
	void __fastcall CloseSameItemClick(TObject *Sender);
	void __fastcall ViewPanelDblClick(TObject *Sender);
	void __fastcall JumpLinkedActionExecute(TObject *Sender);
	void __fastcall JumpLinkedActionUpdate(TObject *Sender);
	void __fastcall AppInfoActionExecute(TObject *Sender);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall MinimizeActionExecute(TObject *Sender);
	void __fastcall MinimizeActionUpdate(TObject *Sender);
	void __fastcall RestoreActionExecute(TObject *Sender);
	void __fastcall RestoreActionUpdate(TObject *Sender);
	void __fastcall TopMostActionExecute(TObject *Sender);
	void __fastcall TopMostActionUpdate(TObject *Sender);
	void __fastcall MaximizeActionExecute(TObject *Sender);
	void __fastcall MaximizeActionUpdate(TObject *Sender);
	void __fastcall InpPaintBoxPaint(TObject *Sender);
	void __fastcall FitToFileListActionExecute(TObject *Sender);
	void __fastcall FitToFileListActionUpdate(TObject *Sender);
	void __fastcall ShowCmdParamActionExecute(TObject *Sender);
	void __fastcall SortByIconActionExecute(TObject *Sender);
	void __fastcall SortByIconActionUpdate(TObject *Sender);
	void __fastcall SortByRemActionExecute(TObject *Sender);
	void __fastcall SortByRemActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	TAppWinList *AppInfoList;
	int MaxWd_f;		//名前の最大幅
	TRect ThumbRect;
	TRect LiveRect;

	int  LaunchPos;
	bool SortByRem;

	bool IsIncSea;
	bool IsMigemo;
	UnicodeString IncSeaWord;
	TStringList *LaunchFileList;

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	file_rec* __fastcall GetLaunchFrecPtr();
	UnicodeString __fastcall get_size_str_K(SIZE_T sz);

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall WMDropFiles(TWMDropFiles &msg);

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		SetDarkWinTheme(this);
		if (Timer1->Enabled) UpdateAppList();
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall WmNyanFiFlIcon(TMessage &msg) { LaunchListBox->Invalidate(); }

	void __fastcall SetIncSeaMode(bool sw);
	void __fastcall UpdateAppList();
	void __fastcall UpdateAppSttBar();
	AppWinInf* __fastcall GetCurAppWinInf();

	void __fastcall UpdateLaunchList(UnicodeString lnam = EmptyStr);
	void __fastcall UpdateLaunchSttBar();

public:		// ユーザー宣言
	UsrScrollPanel *AppScrPanel;	//シンプルスクロールバー
	UsrScrollPanel *LaunchScrPanel;	//シンプルスクロールバー

	UnicodeString LaunchPath;
	UnicodeString CurLaunchPath;
	UnicodeString LaunchFileName;

	UnicodeString JumpFileName;
	UnicodeString JumpPathName;

	UnicodeString FocusClass;

	bool isNyan;
	bool OnlyAppList;
	bool OnlyLauncher;
	bool ToAppList, ToLauncher, ToIncSea;

	__fastcall TAppListDlg(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,		TMessage,		WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_DROPFILES, 		TWMDropFiles,	WMDropFiles)
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,		TMessage,		WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,		TMessage,		WmMenuChar)
		VCL_MESSAGE_HANDLER(WM_NYANFI_FLICON,	TMessage,		WmNyanFiFlIcon)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TAppListDlg *AppListDlg;
//---------------------------------------------------------------------------
#endif
