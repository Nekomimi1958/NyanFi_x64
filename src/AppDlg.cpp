//----------------------------------------------------------------------//
// NyanFi																//
//  アプリケーション一覧												//
//----------------------------------------------------------------------//
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpDir.h"
#include "InpExDlg.h"
#include "EditItem.h"
#include "FileInfDlg.h"
#include "AppDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAppListDlg *AppListDlg = NULL;

//---------------------------------------------------------------------------
HMODULE hPsApi = NULL;
FUNC_GetProcessMemoryInfo lpfGetProcessMemoryInfo = NULL;

//---------------------------------------------------------------------------
//ソート用比較関数
//---------------------------------------------------------------------------
bool SortByIcon = true;

//---------------------------------------------------------------------------
int __fastcall SortComp_Launch(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	if (fp0->is_dir && fp1->is_dir) {
		if (SortByIcon)
			return CompDirIcon(fp0, fp1);
		else
			return CompDirName(fp0, fp1);
	}

	if (fp0->is_dir) return -1;
	if (fp1->is_dir) return  1;

	if (SameText(fp0->f_ext, fp1->f_ext)) {
		UnicodeString nam0 = !fp0->alias.IsEmpty()? fp0->alias : fp0->b_name;
		UnicodeString nam1 = !fp1->alias.IsEmpty()? fp1->alias : fp1->b_name;
		int res = StrCmpLogicalW(nam0.c_str(), nam1.c_str());
		return (res==0)? StrCmpLogicalW(fp0->p_name.c_str(), fp1->p_name.c_str()) : res;
	}

	return CompareText(fp0->f_ext, fp1->f_ext);
}
//---------------------------------------------------------------------------
int __fastcall SortComp_LaunchS(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (SameText(fp0->f_ext, fp1->f_ext)) {
		int res = StrCmpLogicalW(fp0->b_name.c_str(), fp1->b_name.c_str());
		return (res==0)? StrCmpLogicalW(fp0->p_name.c_str(), fp1->p_name.c_str()) : res;
	}

	return CompareText(fp0->f_ext, fp1->f_ext);
}

//---------------------------------------------------------------------------
//アプリケーション情報クラス
//---------------------------------------------------------------------------
AppWinInf::AppWinInf()
{
	WinHandle = NULL;
	toClose	  = false;
	isNoRes	  = false;
	Icon	  = new TIcon();
	PngImg	  = new TPngImage();
	hThumb	  = NULL;
}
//---------------------------------------------------------------------------
AppWinInf::~AppWinInf()
{
	delete Icon;
	delete PngImg;

	if (hThumb) ::DwmUnregisterThumbnail(hThumb);
}

//---------------------------------------------------------------------------
//アプリケーション情報リスト (TList から継承)
//---------------------------------------------------------------------------
__fastcall TAppWinList::TAppWinList(): TList()
{
}

//---------------------------------------------------------------------------
//項目削除時にメモリを解放
//---------------------------------------------------------------------------
void __fastcall TAppWinList::Notify(void *Ptr, TListNotification Action)
{
	if (Action==lnDeleted) delete (AppWinInf*)Ptr;
}

//---------------------------------------------------------------------------
//TAppListDlg クラス
//---------------------------------------------------------------------------
__fastcall TAppListDlg::TAppListDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormCreate(TObject *Sender)
{
	HMODULE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
	if (hKernel32) {
		if (::GetProcAddress(hKernel32, "K32EnumProcessModules")) {
			//Windows 7 から psapi.dll の関数が K32～の名で Kernel32 に移っている
			lpfGetProcessMemoryInfo = (FUNC_GetProcessMemoryInfo)::GetProcAddress(hKernel32, "K32GetProcessMemoryInfo");
		}
		else {
			//Vista
			hPsApi = ::LoadLibrary(_T("psapi.dll"));
			if (hPsApi) lpfGetProcessMemoryInfo = (FUNC_GetProcessMemoryInfo)::GetProcAddress(hPsApi, "GetProcessMemoryInfo");
		}
	}

	org_SttBar1WndProc	   = StatusBar1->WindowProc;
	StatusBar1->WindowProc = SttBar1WndProc;

	AppScrPanel	   = new UsrScrollPanel(AppListPanel, AppListBox,	 USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	LaunchScrPanel = new UsrScrollPanel(LaunchPanel,  LaunchListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	AppInfoList = new TAppWinList();

	LaunchFileList = new TStringList();
	LaunchTopList  = new TStringList();

	ToAppList = ToLauncher = ToIncSea = AddStart = false;
	isFuzzy = false;
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormShow(TObject *Sender)
{
	SetDarkWinTheme(this);

	if (OnlyLauncher) {
		set_FormTitle(this, _T("ランチャー"));
		AppPanel->Visible	= false;
		LRSplitter->Visible = false;
		IniFile->LoadPosInfo(this, DialogCenter, "Launcher");
		LaunchPanel->Align	 = alClient;
		LaunchPanel->Visible = true;
	}
	else {
		set_FormTitle(this, _T("アプリケーション一覧"));
		AppPanel->Visible	= true;
		LRSplitter->Visible = true;
		IniFile->LoadPosInfo(this, DialogCenter);
		ShowMonNoAction->Checked	= IniFile->ReadBoolGen(_T("AppListShowMonNo"));
		ShowCmdParamAction->Checked = IniFile->ReadBoolGen(_T("AppListShowCmdParam"));
		ViewPanel->Height	= IniFile->ReadIntGen(_T("AppListThumbHi"), 100);
		ViewPanel->Color	= col_bgImage;
		ViewSplitter->Color = col_Splitter;
		if (OnlyAppList) {
			LRSplitter->Visible  = false;
			LaunchPanel->Visible = false;
		}
		else {
			LRSplitter->Visible  = true;
			LaunchPanel->Visible = true;
			set_PanelAlign(LaunchPanel, LRSplitter, IniFile->ReadIntGen(_T("AppListLaunchPos"), 1), 5);
			LaunchPanel->Width = IniFile->ReadIntGen(_T("AppListLaunchWd"), 120);
			LRSplitter->Color  = col_Splitter;
		}
	}

	ExcAppText = IniFile->ReadStrGen(_T("AppListExcAppText"));

	setup_StatusBar(StatusBar1);

	LaunchPath = IncludeTrailingPathDelimiter(IniFile->ReadStrGen(_T("AppListLaunchPath"), ExePath));
	if (!dir_exists(LaunchPath)) LaunchPath = ExePath;
	CurLaunchPath = LaunchPath;
	IsMigemo	  = IniFile->ReadBoolGen(_T("AppListLaunchMigemo"));

	AppInfoList->Clear();

	//一覧初期化
	TListBox *lp = AppListBox;
	set_StdListBox(lp, LBTAG_OPT_LOOP);
	set_UsrScrPanel(AppScrPanel);
	int min_hi = ScaledInt(36, this);
	if (lp->ItemHeight<min_hi) lp->ItemHeight = min_hi;
	lp->Count = 0;
	lp->ItemIndex = -1;
	SetDarkWinTheme(lp);

	MaxWd_f = 0;
	if (!OnlyLauncher) UpdateAppList();

	//ランチャーの初期化
	if (!OnlyAppList) {
		SortByRem  = IniFile->ReadBoolGen(_T("AppListLaunchRemSort"));
		SortByIcon = IniFile->ReadBoolGen(_T("AppListLaunchIconSort"), true);
		lp = LaunchListBox;
		set_StdListBox(lp, LBTAG_OPT_LOOP, NULL, true);
		set_UsrScrPanel(LaunchScrPanel);
		lp->Count = 0;
		SetIncSeaMode(ToIncSea);
	}

	JumpFileName   = JumpPathName = EmptyStr;
	LaunchFileName = EmptyStr;
	FocusClass	   = EmptyStr;
	isNyan = false;

	//ファイルのドロップを許可
	::DragAcceptFiles(Handle, LaunchPanel->Visible);

	::PostMessage(Handle, WM_FORM_SHOWED, 0, 0);
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::WmFormShowed(TMessage &msg)
{
	Application->BringToFront();

	if (ToAppList) {
		AppListBox->SetFocus();
	}
	else if (OnlyLauncher || ToLauncher || ToIncSea) {
		LaunchListBox->SetFocus();
		UpdateLaunchSttBar();
	}

	if (!OnlyLauncher) Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	UserModule->SetBlinkTimer(NULL);
	Timer1->Enabled = false;

	AppInfoList->Clear();
	LaunchFileList->Clear();
	LaunchTopList->Clear();
	clear_FileList(LaunchList);

	if (OnlyLauncher) {
		IniFile->SavePosInfo(this, "Launcher");
	}
	else {
		IniFile->SavePosInfo(this);
		IniFile->WriteBoolGen(_T("AppListShowMonNo"),		ShowMonNoAction);
		IniFile->WriteBoolGen(_T("AppListShowCmdParam"),	ShowCmdParamAction);
		IniFile->WriteIntGen(_T("AppListThumbHi"),			ViewPanel->Height);
	}

	if (!OnlyAppList && !OnlyLauncher)
		IniFile->WriteIntGen(_T("AppListLaunchPos"),	(LaunchPanel->Align==alLeft)? 2 : 1);

	if (!OnlyAppList) {
		IniFile->WriteIntGen( _T("AppListLaunchWd"),		LaunchPanel->Width);
		IniFile->WriteBoolGen(_T("AppListLaunchRemSort"),	SortByRem);
		IniFile->WriteBoolGen(_T("AppListLaunchIconSort"),	SortByIcon);
		IniFile->WriteStrGen( _T("AppListLaunchPath"),		LaunchPath);
		IniFile->WriteBoolGen(_T("AppListLaunchMigemo"),	IsMigemo);
	}

	IniFile->WriteStrGen(_T("AppListExcAppText"),	ExcAppText);

	OnlyLauncher = OnlyAppList = false;
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormDestroy(TObject *Sender)
{
	delete AppInfoList;
	delete LaunchFileList;
	delete LaunchTopList;

	delete AppScrPanel;
	delete LaunchScrPanel;

	if (hPsApi) ::FreeLibrary(hPsApi);
}

//---------------------------------------------------------------------------
//ステータスバーの描画
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel,
	const TRect &Rect)
{
	TCanvas *cv = StatusBar->Canvas;
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(Rect);
	cv->Font->Color = col_fgSttBar;
	cv->TextOut(Rect.Left + ScaledInt(2, this), Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//ショートカットファイルのドロップ受け入れ
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::WMDropFiles(TWMDropFiles &msg)
{
	UINT f_cnt = ::DragQueryFile((HDROP)msg.Drop, 0xFFFFFFFF, NULL, 0);
	if (f_cnt) {
		try {
			std::unique_ptr<TStringList> f_list(new TStringList());
			UnicodeString fnam;
			for (UINT i=0; i<f_cnt; i++) {
				fnam.SetLength(MAX_PATH);
				int len = ::DragQueryFile((HDROP)msg.Drop, i, fnam.c_str(),fnam.Length());
				fnam.SetLength(len);
				int atr = file_GetAttr(fnam);
				if (atr==faInvalid)		LoadUsrMsg(USTR_CantAccessDir);
				if (faDirectory & atr)	UserAbort(USTR_IncludeDir);
				if (!test_FileExt(get_extension(fnam), ".lnk.url")) continue;
				f_list->Add(fnam);
			}
			//コピー
			if (f_list->Count>0) {
				for (int i=0; i<f_list->Count; i++) {
					fnam = f_list->Strings[i];
					if (!copy_File(fnam, CurLaunchPath + ExtractFileName(fnam))) UserAbort(USTR_FaildCopy);
				}
				UpdateLaunchList(CurLaunchPath + ExtractFileName(f_list->Strings[0]));
				LaunchListBox->SetFocus();
			}
		}
		catch (EAbort &e) {
			msgbox_ERR(e.Message);
		}
	}
	::DragFinish((HDROP)msg.Drop);
}

//---------------------------------------------------------------------------
//ランチャーの file_rec ポインタを取得
//---------------------------------------------------------------------------
file_rec* __fastcall TAppListDlg::GetLaunchFrecPtr()
{
	int idx = LaunchListBox->ItemIndex;
	return (idx>=0 && idx<LaunchList->Count)? (file_rec*)LaunchList->Objects[idx] : NULL;
}

//---------------------------------------------------------------------------
//メモリサイズの文字列に(KB単位)
//---------------------------------------------------------------------------
UnicodeString __fastcall TAppListDlg::get_size_str_K(SIZE_T sz)
{
	UnicodeString szstr;
	if (sz>0) {
		int sz_k = sz/1024;
		szstr.sprintf(_T("%u KB"), (int) sz_k);
		if (sz_k>=1000)    szstr.Insert(",", szstr.Length() - 5);
		if (sz_k>=1000000) szstr.Insert(",", szstr.Length() - 9);
	}
	else {
		szstr = "???? KB";
	}
	return szstr;
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxEnter(TObject *Sender)
{
	UpdateAppSttBar();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxEnter(TObject *Sender)
{
	UpdateLaunchSttBar();
}

//---------------------------------------------------------------------------
//インクリメンタルサーチモードの設定
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SetIncSeaMode(bool sw)
{
	UnicodeString l_fnam;
	if (IsIncSea) {
		file_rec *cfp = GetLaunchFrecPtr();
		if (cfp) l_fnam = cfp->f_name;
	}

	IsIncSea = sw;
	UserModule->SetBlinkTimer(NULL);

	setup_Panel(DirPanel, DirInfFont);
	DirPanel->Color 	  = col_bgDirInf;
	DirPanel->Font->Color = col_fgDirInf;
	DirPanel->BevelOuter  = FlatInfPanel? bvNone : bvRaised;
	setup_Panel(InpPanel, ListFont);
	InpPanel->Color 	  = col_bgList;
	InpPanel->BevelOuter  = bvLowered;

	//インクリメンタルサーチモード
	if (IsIncSea) {
		InpPanel->Visible = true;
		UserModule->SetBlinkTimer(InpPaintBox);

		LaunchTopList->Clear();
		LaunchTopList->Add(get_parent_path(LaunchPath));

		IncSeaWord = EmptyStr;
		LaunchFileList->Clear();
		get_files(LaunchPath, _T("*.lnk"), LaunchFileList, true);
		get_files(LaunchPath, _T("*.url"), LaunchFileList, true);

		//スタートメニュー項目を追加
		if (AddStart) {
			std::unique_ptr<TStringList> st_lst(new TStringList());
			usr_SH->AddKnownPath(FOLDERID_StartMenu,		st_lst.get());
			usr_SH->AddKnownPath(FOLDERID_CommonStartMenu,	st_lst.get());
			for (int i=0; i<st_lst->Count; i++) {
				UnicodeString pnam = get_pre_tab(st_lst->Strings[i]);
				LaunchTopList->Add(pnam);
				get_files(pnam, _T("*.lnk"), LaunchFileList, true);
				get_files(pnam, _T("*.url"), LaunchFileList, true);
			}
		}
	}
	//通常表示
	else {
		CurLaunchPath = StartsText(LaunchPath, l_fnam)? ExtractFilePath(l_fnam) : LaunchPath;
		InpPanel->Visible = false;
	}

	UpdateLaunchList(l_fnam);
}

//---------------------------------------------------------------------------
//一覧の更新
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateAppList()
{
	TListBox *lp = AppListBox;
	int idx = lp->ItemIndex;
	bool changed = false;
	bool req_upd = false;

	for (int i=0; i<AppInfoList->Count; i++) AppInfoList->Items[i]->Exist = false;

	AppWinInf *ap_Res = NULL;

	//Zオーダー順に取得
	HWND hWnd = ::GetWindow(Application->Handle, GW_HWNDFIRST);
	while (hWnd) {
		do {
			//プロセスID
			DWORD pid;
			DWORD tid = ::GetWindowThreadProcessId(hWnd, &pid);

			//クラス名
			UnicodeString cnam = get_WndClassName(hWnd);
			if (!::IsWindowVisible(hWnd) && !Application->MainForm->ClassNameIs(cnam)) break;
			if (ProcessId==pid && contained_wd_i("TAppListDlg|HH Parent", cnam)) break;

			BOOL cloaked;
			if (::DwmGetWindowAttribute(hWnd, DWMWA_CLOAKED, &cloaked, sizeof(cloaked))==S_OK && cloaked) break;

			//拡張ウィンドウスタイル
			LONG w_style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			if (w_style & WS_EX_TOOLWINDOW) break;
			if ((w_style & WS_EX_LAYERED) && !(w_style & WS_EX_WINDOWEDGE)) break;
			if (::GetParent(hWnd) && !(w_style & WS_EX_APPWINDOW)) break;

			//ウィンドウサイズ
			TRect w_rect = get_window_rect(hWnd);
			if (w_rect.IsEmpty()) break;

			//テキスト
			UnicodeString wtxt = get_WndText(hWnd);
			if (wtxt.IsEmpty()) break;
			//除外チェック
			if (!ExcAppText.IsEmpty()) {
				TStringDynArray exc_txt_lst = split_strings_semicolon(ExcAppText);
				bool ok = true;
				for (int i=0; i<exc_txt_lst.Length && ok; i++) {
					if (!exc_txt_lst[i].IsEmpty() && ContainsText(wtxt, exc_txt_lst[i])) ok = false;
				}
				if (!ok) break;
			}

			//UWPアプリ
			bool is_uwp = SameText(cnam, "ApplicationFrameWindow");
			DWORD pid2 = 0, tid2 = 0;
			UnicodeString cnam2;
			if (is_uwp) {
				HWND hCore = ::FindWindowEx(hWnd, NULL, _T("Windows.UI.Core.CoreWindow"), NULL);
				if (!hCore) hCore = ::FindWindow(_T("Windows.UI.Core.CoreWindow"), wtxt.c_str());
				if (hCore) {
					tid2  = ::GetWindowThreadProcessId(hCore, &pid2);
					cnam2 = "Windows.UI.Core.CoreWindow";
				}
			}

			//既存か?
			AppWinInf *ap = NULL;
			bool is_new = false;
			for (int i=0; i<AppInfoList->Count && !ap; i++)
				if (AppInfoList->Items[i]->WinHandle==hWnd) ap = AppInfoList->Items[i];

			if (!ap) {
				ap = new AppWinInf();
				AppInfoList->Add(ap);
				is_new	= true;
				changed = true;
			}

			ap->WinHandle  = hWnd;
			ap->PID 	   = (pid2!=0)? pid2 : pid;
			ap->TID 	   = (tid2!=0)? tid2 : tid;
			ap->Exist	   = true;
			ap->isNyan	   = (ProcessId==pid);
			ap->WinText    = wtxt;
			ap->ClassName  = cnam;
			ap->ClassName2 = cnam2;
			ap->topMost    = (w_style & WS_EX_TOPMOST);
			ap->win_xstyle = w_style;
			ap->isWow64    = false;
			ap->isUWP	   = is_uwp;
			ap->win_wd	   = w_rect.Width();
			ap->win_hi	   = w_rect.Height();
			ap->win_left   = w_rect.Left;
			ap->win_top    = w_rect.Top;
			//表示モニタ
			ap->mon_no = -1;
			for (int i=0; i<Screen->MonitorCount; i++) {
				TRect m_rc = Screen->Monitors[i]->BoundsRect;
				TRect rc   = TRect::Intersect(m_rc, w_rect);
				if ((rc.Width() * rc.Height()) > (ap->win_wd * ap->win_hi / 2)) {
					ap->mon_no = i;
					break;
				}
			}

			//無応答チェック
			bool last_nores = ap->isNoRes;
			DWORD_PTR res;
			ap->isNoRes = (::SendMessageTimeout(hWnd, WM_NULL, 0, 0, SMTO_ABORTIFHUNG, 50, &res)==0);
			if (last_nores != ap->isNoRes) changed = true;

			ap->mem_WS = ap->mem_pWS = ap->mem_PF = ap->mem_pPF = 0;
			HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ap->PID);
			if (hProcess) {
				BOOL wow64;
				if (::IsWow64Process(hProcess, &wow64)) ap->isWow64 = wow64;
				//メモリ情報
				PROCESS_MEMORY_COUNTERS pmc = {0};
				if (lpfGetProcessMemoryInfo && lpfGetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
					ap->mem_WS	= pmc.WorkingSetSize;
					ap->mem_pWS = pmc.PeakWorkingSetSize;
					ap->mem_PF	= pmc.PagefileUsage;
					ap->mem_pPF	= pmc.PeakPagefileUsage;
				}
				//作成時刻
				FILETIME s_tm, x_tm, k_tm, u_tm, f_tm;
				if (::GetProcessTimes(hProcess, &s_tm, &x_tm, &k_tm, &u_tm)) ap->StartTime = utc_to_DateTime(&s_tm);

				//コマンドライン
				HINSTANCE hNtDll = ::GetModuleHandle(_T("ntdll.dll"));
				NTSTATUS (NTAPI* lpfNtQueryInformationProcess)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG) =
					(NTSTATUS (NTAPI*)(HANDLE, PROCESSINFOCLASS, PVOID, ULONG, PULONG))::GetProcAddress(hNtDll, "NtQueryInformationProcess");
				ULONG (NTAPI* lpfRtlNtStatusToDosError)(NTSTATUS) =
					(ULONG (NTAPI*)(NTSTATUS))::GetProcAddress(hNtDll, "RtlNtStatusToDosError");

				if (lpfNtQueryInformationProcess && lpfRtlNtStatusToDosError) {
					PROCESS_BASIC_INFORMATION pbi;
					ULONG len;
					NTSTATUS status = lpfNtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &len);
					::SetLastError(lpfRtlNtStatusToDosError(status));
					if(!NT_ERROR(status) && pbi.PebBaseAddress) {
						SIZE_T bytesRead = 0;
						PEB_INTERNAL peb;
						if(::ReadProcessMemory(hProcess, pbi.PebBaseAddress, &peb, sizeof(peb), &bytesRead)) {
							RTL_USER_PROCESS_PARAMETERS_I upp;
							if(::ReadProcessMemory(hProcess, peb.ProcessParameters, &upp, sizeof(upp), &bytesRead)) {
								if(upp.CommandLine.Length>0) {
									DWORD need_len = (upp.CommandLine.Length + 1) / sizeof(wchar_t) + 1;
									std::unique_ptr<_TCHAR[]> lpszCbuf(new _TCHAR[need_len]);
									lpszCbuf[need_len - 1] = L'\0';
									if(::ReadProcessMemory(hProcess, upp.CommandLine.Buffer,
										lpszCbuf.get(), upp.CommandLine.Length, &bytesRead))
									{
										ap->CmdParam = lpszCbuf.get();
										split_file_param(ap->CmdParam);
									}
								}
							}
						}
					}
				}

				::CloseHandle(hProcess);
			}

			if (is_new) {
				//閉じようとしているプロセスから新たにウィンドウが出ているか?
				for (int i=0; i<AppInfoList->Count; i++) {
					AppWinInf *bp = AppInfoList->Items[i];
					if (bp->toClose && bp->PID==ap->PID) {
						ap_Res = ap;  break;
					}
				}

				//実行ファイル名とアイコン
				if (ap->isNyan) {
					ap->FileName = Application->ExeName;
					ap->Caption	 = get_base_name(Application->ExeName);
					ap->Icon->Assign(Application->Icon);
				}
				else {
					if (!ap->isNoRes && !ap->isUWP) {
						ap->Icon->Handle = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);
					}

					hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ap->PID);
					if (hProcess) {
						_TCHAR sbuf[MAX_PATH];
						DWORD sz = MAX_PATH;
						UnicodeString fnam = ::QueryFullProcessImageName(hProcess, 0, sbuf, &sz)? sbuf : _T("");
						::CloseHandle(hProcess);
						ap->FileName = fnam;
						ap->Caption	 = get_base_name(fnam);
					}

					if (ap->isUWP) {
						hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, ap->PID);
						if (hProcess) {
							try {
								HINSTANCE hKernel32 = ::GetModuleHandle(_T("kernel32"));
								LONG (WINAPI*lpfGetPackageFullName)(HANDLE, UINT32*, PWSTR) =
									(LONG (WINAPI*)(HANDLE, UINT32*, PWSTR))::GetProcAddress(hKernel32, "GetPackageFullName");
								LONG (WINAPI*lpfGetPackagePathByFullName)(PCWSTR,UINT32*,PWSTR) =
									(LONG (WINAPI*)(PCWSTR, UINT32*, PWSTR))::GetProcAddress(hKernel32, "GetPackagePathByFullName");
								if (!lpfGetPackageFullName || !lpfGetPackagePathByFullName) Abort();

								UINT32 buf_len = 0;
								LONG res = lpfGetPackageFullName(hProcess, &buf_len, NULL);
								if (res!=ERROR_INSUFFICIENT_BUFFER || buf_len==0) Abort();
								std::unique_ptr<_TCHAR[]> pkg_buf(new _TCHAR[buf_len]);
								PWSTR pkg_name = pkg_buf.get();
								if (lpfGetPackageFullName(hProcess, &buf_len, pkg_name)!=ERROR_SUCCESS) Abort();

								buf_len = 0;
								res = lpfGetPackagePathByFullName(pkg_name, &buf_len, NULL);
								if (res!=ERROR_INSUFFICIENT_BUFFER || buf_len==0) Abort();
								std::unique_ptr<_TCHAR[]> pth_buf(new _TCHAR[buf_len]);
								PWSTR pth_name = pth_buf.get();
								if (lpfGetPackagePathByFullName(pkg_name, &buf_len, pth_name)!=ERROR_SUCCESS) Abort();
								UnicodeString pnam = IncludeTrailingPathDelimiter(pth_buf.get());
								std::unique_ptr<TStringList> fbuf(new TStringList());
								if (load_text_ex(pnam + "AppxManifest.xml", fbuf.get())==0) Abort();
								UnicodeString snam = get_tkn(get_tkn_r(fbuf->Text, "Square44x44Logo=\""), "\"");
								if (!snam.IsEmpty()) {
									UnicodeString fext = get_extension(snam);
									UnicodeString bnam = get_base_name(snam);
									UnicodeString dnam = ExtractFilePath(pnam + snam);
									UnicodeString inam = dnam + bnam + fext;
									if (!file_exists(inam)) inam = dnam + bnam + ".targetsize-32" + fext;
									if (file_exists(inam)) {
										ap->PngImg->LoadFromFile(inam);
										ap->PngImg->Transparent = true;
									}
								}
							}
							catch (...) {
								;
							}

							::CloseHandle(hProcess);
						}
					}

					if (ap->Icon->Empty && !ap->FileName.IsEmpty()) {
						int size = 32;
						ap->Icon->Handle = usr_SH->get_Icon(ap->FileName, size, false);
					}
				}
			}
		} while (false);

		hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
	}

	//存在しなくなった項目を破棄
	int i = 0;
	while(i < AppInfoList->Count) {
		AppWinInf *ap = AppInfoList->Items[i];
		if (!ap->Exist) {
			req_upd = USAME_TI(ap->Caption, "NyanFi");
			AppInfoList->Delete(i);
			changed = true;
		}
		else i++;
	}

	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	for (int i=0; i<AppInfoList->Count; i++) {
		AppWinInf *ap = AppInfoList->Items[i];
		int wd = get_TextWidth(cv, ap->Caption, is_irreg);
		if (ap->isWow64) wd += get_TextWidth(cv, ISWOW64_STR, is_irreg);
		MaxWd_f = std::max(wd, MaxWd_f);
	}

	//リストボックスに割り当て(仮想)
	if (lp->Count!=AppInfoList->Count)
		lp->Count = AppInfoList->Count;
	else if (changed)
		lp->Invalidate();

	Application->ProcessMessages();
	ListBoxSetIndex(lp, idx);
	AppScrPanel->UpdateKnob();

	UpdateAppSttBar();

	//プロパティ変更を反映
	if (req_upd) UpdateLaunchList();

	//閉じようとしているプロセスから出たウィンドウへ切り替え
	if (ap_Res) ::SetForegroundWindow(ap_Res->WinHandle);
}

//---------------------------------------------------------------------------
AppWinInf* __fastcall TAppListDlg::GetCurAppWinInf()
{
	int idx = AppListBox->ItemIndex;
	return (idx>=0 && idx<AppInfoList->Count)? AppInfoList->Items[idx] : NULL;
}

//---------------------------------------------------------------------------
//ランチャー項目を作成
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AddLnkFileRec(UnicodeString fnam, TStringList *lst,
	UnicodeString rnam)		//編集距離取得のための基準名	(default = EmptyStr)
{
	file_rec *fp = cre_new_file_rec(fnam); 
	if (fp) {
		//fp->l_name = リンク先, fp->alias = コメント, fp->faild = リンク切れ
		if (SortByRem && test_LnkExt(fp->f_ext)) {
			usr_SH->get_LnkInf(fp->f_name, NULL, &fp->l_name, NULL, NULL, NULL, &fp->alias);
			fp->failed = !fp->l_name.IsEmpty() && !StartsStr("::", fp->l_name) && !file_exists(fp->l_name);
		}
		//URL
		else if (SortByRem && USAME_TI(fp->f_ext, ".url")) {
			std::unique_ptr<UsrIniFile> url_file(new UsrIniFile(fnam));
			fp->l_name = url_file->ReadString("InternetShortcut", "URL");
		}
		//編集距離
		if (!rnam.IsEmpty()) {
	 		fp->distance = get_NrmLevenshteinDistance(rnam, fp->b_name, true);
		}
		lst->AddObject(fp->f_name, (TObject*)fp);
	}
}

//---------------------------------------------------------------------------
//ランチャーリストの更新
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateLaunchList(UnicodeString lnam)
{
	if (IsIncSea) UserModule->RepaintBlink();

	clear_FileList(LaunchList);

	//インクリメンタルサーチ
	if (IsIncSea) {
		bool is_all = contained_wd_i(_T("*|?| "), IncSeaWord);
		UnicodeString ptn = is_all? UnicodeString(".+") : 
		    (isFuzzy && !IsMigemo)? get_fuzzy_ptn(IncSeaWord) : 
								 	usr_Migemo->GetRegExPtn(IsMigemo, IncSeaWord);
		if (!ptn.IsEmpty()) {
			UnicodeString rnam = (!is_all && isFuzzy && !IsMigemo)? IncSeaWord : EmptyStr;
			TRegExOptions opt; 
			if (IncSeaWord.LowerCase()==IncSeaWord) opt << roIgnoreCase;
			for (int i=0; i<LaunchFileList->Count; i++) {
				UnicodeString fnam = LaunchFileList->Strings[i];
				if (TRegEx::IsMatch(get_base_name(fnam), ptn, opt)) AddLnkFileRec(fnam, LaunchList, rnam);
			}
			if (!rnam.IsEmpty())
				LaunchList->CustomSort(SortComp_Distance);
			else
				LaunchList->CustomSort(SortComp_LaunchS);
		}

		DirPanel->Caption = EmptyStr;
	}
	//通常モード
	else {
		DirPanel->Caption =
			get_MiniPathName(get_tkn_r(CurLaunchPath, get_parent_path(LaunchPath)), DirPanel->ClientWidth, DirPanel->Font);
		TSearchRec sr;
		if (FindFirst(CurLaunchPath + "*.*", faAnyFile, sr)==0) {
			do {
				UnicodeString fnam = sr.Name;	if (USAME_TS(fnam, ".")) continue;
				if (sr.Attr & faDirectory) {
					if (USAME_TS(fnam, "..")) {
						if (SameText(CurLaunchPath, LaunchPath)) continue;
					}
					else {
						fnam = IncludeTrailingPathDelimiter(fnam);
					}
				}
				else if (!test_FileExt(get_extension(fnam), ".lnk.url")) {
					continue;
				}

				if (!USAME_TS(fnam, "..")) fnam = CurLaunchPath + fnam;
				AddLnkFileRec(fnam, LaunchList);
			} while(FindNext(sr)==0);
			FindClose(sr);
		}
		LaunchList->CustomSort(SortComp_Launch);
	}

	//リストボックスに割り当て(仮想)
	TListBox *lp = LaunchListBox;
	int idx =lp->ItemIndex;
	lp->Count = LaunchList->Count;
	Application->ProcessMessages();

	if (USAME_TS(lnam, ".."))
		idx = 0;
	else if (!lnam.IsEmpty())
		idx = LaunchList->IndexOf(lnam);

	ListBoxSetIndex(lp, idx);
	LaunchScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//ステータスバーの更新(ランチャー)
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateLaunchSttBar()
{
	UnicodeString msg;
	file_rec *cfp = GetLaunchFrecPtr();
	if (cfp) {
		msg = cfp->n_name;
		if (!cfp->is_dir) {
			//コメント
			if (!cfp->alias.IsEmpty()) msg.cat_sprintf(_T(" (%s)"),  cfp->alias.c_str());
			//リンク先
			if (!cfp->l_name.IsEmpty()) {
				if (cfp->failed)
					msg += " → リンク切れ";
				else
					msg.cat_sprintf(_T(" → %s"), yen_to_delimiter(cfp->l_name).c_str());
			}
		}

		if (IsIncSea) {
			UnicodeString pnam = cfp->p_name;
			for (int i=0; i<LaunchTopList->Count; i++) {
				if (remove_top_text(pnam, LaunchTopList->Strings[i])) break;
			}
			DirPanel->Caption = get_MiniPathName(pnam, DirPanel->ClientWidth, DirPanel->Font);
		}
	}

	StatusBar1->Panels->Items[0]->Text = msg;
	StatusBar1->Hint = msg;

}

//---------------------------------------------------------------------------
//ステータスバーおよびライブサムネイルの更新
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateAppSttBar()
{
	TListBox *lp = AppListBox;
	AppWinInf *c_ap = GetCurAppWinInf();
	if (!c_ap) {
		if (lp->Focused()) {
			StatusBar1->Panels->Items[0]->Text = EmptyStr;
			StatusBar1->Hint = EmptyStr;
		}
		return;
	}

	//ステータスバーの更新
	if (lp->Focused()) {
		UnicodeString msg;
		msg.sprintf(_T("PID:%u   WS:%s - %s / PF:%s - %s   Win:%u×%u"),
			c_ap->PID,
			get_size_str_K(c_ap->mem_WS).c_str(), get_size_str_K(c_ap->mem_pWS).c_str(),
			get_size_str_K(c_ap->mem_PF).c_str(), get_size_str_K(c_ap->mem_pPF).c_str(),
			c_ap->win_wd, c_ap->win_hi);
		if (::IsIconic(c_ap->WinHandle)) {
			msg += " (_)";
		}
		else {
			if (c_ap->mon_no>=0)
				msg.cat_sprintf(_T(" (%u)"), c_ap->mon_no + 1);
			else
				msg += " (?)";
		}
		if (c_ap->isNoRes) msg += " (無応答)";
		msg += "  Started:";
		msg += format_DateTime(c_ap->StartTime, true);
		StatusBar1->Panels->Items[0]->Text = msg;
		StatusBar1->Hint = msg;
	}

	//カーソル位置以外のサムネイルは解放
	for (int i=0; i<AppInfoList->Count; i++) {
		AppWinInf *ap = AppInfoList->Items[i];
		if (ap!=c_ap && ap->hThumb) {
			::DwmUnregisterThumbnail(ap->hThumb);
			ap->hThumb = NULL;
		}
	}

	//カーソル位置アプリのサムネイルが無ければ設定
	if (!c_ap->hThumb) {
		ThumbRect = Rect(0, 0, 0, 0);
		LiveRect  = Rect(0, 0, 0, 0);
		if (ViewPanel->Height>0) ::DwmRegisterThumbnail(Handle, c_ap->WinHandle, &c_ap->hThumb);
	}

	//初回およびサイズ変化時にサムネイル設定を更新
	if (c_ap->hThumb) {
		TRect rc_thum = Rect(0, 0, 0, 0);
		SIZE size;
		if (SUCCEEDED(::DwmQueryThumbnailSourceSize(c_ap->hThumb, &size))) {
			rc_thum.SetWidth(size.cx);
			rc_thum.SetHeight(size.cy);
		}

		TRect rc_dest = ViewPanel->BoundsRect;
		OffsetRect(rc_dest, AppPanel->Left, 0);
		InflateRect(rc_dest, -16, -8);
		if (size.cx>0 && size.cy>0 && rc_dest.Width()>0 && rc_dest.Height()>0
			&& (rc_dest!=ThumbRect || LiveRect!=rc_thum))
		{
			ThumbRect = rc_dest;
			LiveRect  = rc_thum;
			double dest_w = rc_dest.Width();
			double dest_h = rc_dest.Height();
			double size_w = size.cx;
			double size_h = size.cy;
			for (int i=0; (size_w>dest_w || size_h>dest_h) && i<2; i++) {
				double r = (size_w>dest_w)? dest_w/size_w : dest_h/size_h;
				size_w *= r;
				size_h *= r;
			}
			rc_dest.SetWidth((int)size_w);
			rc_dest.SetHeight((int)size_h);

			DWM_THUMBNAIL_PROPERTIES prop;
			prop.dwFlags = DWM_TNP_RECTDESTINATION | DWM_TNP_VISIBLE | DWM_TNP_SOURCECLIENTAREAONLY;
			prop.fSourceClientAreaOnly = FALSE;
			prop.fVisible			   = TRUE;
			prop.opacity			   = 255;
			prop.rcDestination		   = rc_dest;
			::DwmUpdateThumbnailProperties(c_ap->hThumb, &prop);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::Timer1Timer(TObject *Sender)
{
	UpdateAppList();
}

//---------------------------------------------------------------------------
//一覧の描画
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect,
	TOwnerDrawState State)
{
	AppWinInf *ap = AppInfoList->Items[Index];

	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	//背景
	cv->Brush->Color = ap->toClose ? clMaroon
								   : State.Contains(odSelected) ? (lp->Focused()? col_selItem : SelectWorB(col_bgList, 0.15))
																: col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + ScaledInt(4, this);

	//無応答表示
	if (ap->isNoRes) {
		TRect rc = Rect;
		rc.Left	 = xp + ScaledInt(36, this);
		rc.Right = xp + ScaledInt(44, this) + MaxWd_f + get_CharWidth(cv, 2);
		InflateRect(rc, 0, -2);
		TColor br_col	 = cv->Brush->Color;
		cv->Brush->Color = col_Error;
		cv->FrameRect(rc);
		cv->Brush->Color = br_col;
	}

	//アイコン
	int s_32 = ScaledInt(32, this);
	if (!ap->PngImg->Empty) {
		TRect rc = Rect;
		rc.Left = xp; rc.Top += 2;
		rc.SetWidth(s_32); rc.SetHeight(s_32);
		ap->PngImg->Draw(cv, rc);
	}
	else if (ap->Icon->Handle) {
		::DrawIconEx(cv->Handle, xp, Rect.Top + 2, ap->Icon->Handle, s_32, s_32, 0, NULL, DI_NORMAL);
	}

	//最小化マーク
	if (::IsIconic(ap->WinHandle)) {
		TRect rc(xp + ScaledInt(22, this), Rect.Top + ScaledInt(24, this),
				 xp + ScaledInt(34, this), Rect.Top + ScaledInt(36, this));
		cv->Brush->Color = scl_BtnFace;
		cv->FillRect(rc);
		draw_Line(cv, rc.Left + 2, rc.Bottom - 3, rc.Right -2, rc.Bottom - 3, 2, scl_BtnText);
		::DrawEdge(cv->Handle, &rc, BDR_RAISEDOUTER, BF_RECT);
	}
	xp += ScaledInt(40, this);
	cv->Brush->Style = bsClear;

	bool use_fgsel = is_SelFgCol(State);

	//数字
	int yp = Rect.Top + (Rect.Height() - cv->TextHeight("I"))/2;
	if (Index<10) {
		cv->Font->Color = use_fgsel? col_fgSelItem : col_fgList;
		cv->Font->Style = cv->Font->Style << fsUnderline;
		cv->TextOut(xp, yp, UnicodeString().sprintf(_T("%u"), (Index + 1)%10));
	}
	cv->Font->Style = cv->Font->Style >> fsUnderline;
	xp += get_CharWidth(cv, 2);

	//モニタ番号
	if (Screen->MonitorCount>1 && ShowMonNoAction->Checked) {
		xp += get_CharWidth(cv, 1);
		if (::IsIconic(ap->WinHandle)) {
			cv->Font->Color = col_InvItem;
			cv->TextOut(xp, yp, "_");
		}
		else {
			if (ap->mon_no>=0) {
				cv->Font->Color = use_fgsel? col_fgSelItem : AdjustColor(col_fgList, ADJCOL_LIGHT);
				cv->TextOut(xp, yp, UnicodeString().sprintf(_T("%u"), ap->mon_no + 1));
			}
			else {
				cv->Font->Color = col_Error;
				cv->TextOut(xp, yp, "?");
			}
		}
		xp += get_CharWidth(cv, 2);
	}

	//名前
	TColor col_x = get_ExtColor(ExtractFileExt(ap->FileName));
	cv->Font->Color = (lp->Focused() && use_fgsel)? col_fgSelItem : col_x;
	cv->TextOut(xp, yp, ap->Caption);
	if (ap->isWow64) cv->TextOut(xp + get_TextWidth(cv, ap->Caption, is_irreg), yp, ISWOW64_STR);
	xp += MaxWd_f + ScaledInt(12, this);
	//テキスト
	UnicodeString s = yen_to_delimiter(ap->WinText);
	cv->Font->Color = (lp->Focused() && use_fgsel)? col_fgSelItem : col_fgList;
	cv->TextOut(xp, yp, s);
	//コマンドラインパラメータ
	if (ShowCmdParamAction->Checked && !ap->CmdParam.IsEmpty()) {
		cv->Font->Color = (lp->Focused() && use_fgsel)? col_fgSelItem : AdjustColor(col_fgList, ADJCOL_LIGHT);
		xp += (get_TextWidth(cv, s, is_irreg) + get_CharWidth(cv, 2));
		int w = Rect.Right - xp - cv->TextWidth("… ");
		if (w>0) cv->TextOut(xp, yp, minimize_str(yen_to_delimiter(ap->CmdParam), cv, w, true));
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	AppWinInf *ap = AppInfoList->Items[Index];
	UnicodeString s = ap->Caption + "\r\n" + yen_to_delimiter(ap->WinText)+ "\r\n";
	if (!ap->CmdParam.IsEmpty()) s += (yen_to_delimiter(ap->CmdParam) + "\r\n");
	Data = s;
}

//---------------------------------------------------------------------------
//一覧のキー操作
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString CmdStr = Key_to_CmdF(KeyStr);
	if (CmdStr.IsEmpty()) CmdStr = get_CsrKeyCmd(KeyStr);

	TListBox *lp = (TListBox*)Sender;
	bool handled = true;
	//切り替え
	if (is_Num0to9(KeyStr) || equal_ENTER(KeyStr)) {
		int idx = is_Num0to9(KeyStr)? ((KeyStr.ToIntDef(0) + 9)%10) : lp->ItemIndex;
		if (idx>=0 && idx<AppInfoList->Count) {
			AppWinInf *ap = AppInfoList->Items[idx];
			isNyan = ap->isNyan;
			if (!ap->isNoRes) {
				if (isNyan) {
					if (::IsIconic(ap->WinHandle)) ::PostMessage(ap->WinHandle, WM_SYSCOMMAND, SC_RESTORE, 0);
					if (!Application->MainForm->ClassNameIs(ap->ClassName)) FocusClass = ap->ClassName;
				}
				else {
					if (::IsIconic(ap->WinHandle)) ::SendMessage(ap->WinHandle, WM_SYSCOMMAND, SC_RESTORE, 0);
					::SetForegroundWindow(ap->WinHandle);
				}
			}
			ModalResult = mrOk;
		}
	}
	//ランチャーへ
	else if (LaunchPanel->Visible && LaunchPanel->Align==alRight && is_ToRightOpe(KeyStr, CmdStr)) {
		LaunchListBox->SetFocus();
	}
	else if (LaunchPanel->Visible && LaunchPanel->Align==alLeft  && is_ToLeftOpe(KeyStr, CmdStr)) {
		LaunchListBox->SetFocus();
	}
	//ウィンドウ操作
	else if (USAME_TI(CmdStr, "WinMinimize")) {
		MinimizeAction->Execute();
	}
	else if (USAME_TI(CmdStr, "WinMaximize")) {
		MaximizeAction->Execute();
	}
	else if (USAME_TI(CmdStr, "WinNormal")) {
		RestoreAction->Execute();
	}
	else if (USAME_TI(CmdStr, "Delete")) {
		CloseItem->Click();
	}
	else if (ExeCmdListBox(lp, CmdStr)) {
		;
	}
	//右クリックメニュー
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
		show_PopupMenu(lp);
	}
	//頭文字サーチ
	else if (is_IniSeaKey(KeyStr)) {
		ListBoxInitialSearch(AppListBox, KeyStr);
	}
	else if (equal_ESC(KeyStr)) {
		ModalResult = mrCancel;
	}
	//アプリケーション情報
	else if (StartsText("ShowFileInfo", CmdStr) || USAME_TI(CmdStr, "ListFileInfo")) {
		AppInfoAction->Execute();
	}
	else handled = false;

	if (!is_DialogKey(Key) || handled) Key = 0;

	UpdateAppSttBar();
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxClick(TObject *Sender)
{
	UpdateAppSttBar();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ViewPanelDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)AppListBox);
}

//---------------------------------------------------------------------------
//右クリックメニュー
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::PopupMenu1Popup(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		TopMostItem->Checked  = ap->topMost;
		bool has_f = !ap->FileName.IsEmpty();
		JumpExeItem->Enabled  = has_f;
		CopyNameItem->Enabled = has_f;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::PopupMenu2Popup(TObject *Sender)
{
	if (OnlyLauncher) {
		LaunchPosItem->Visible	 = false;
	}
	else {
		LaunchPosItem->Visible	 = !OnlyLauncher;
		LaunchRightItem->Checked = (LaunchPanel->Align==alRight);
		LaunchLeftItem->Checked  = (LaunchPanel->Align==alLeft);
	}
}

//---------------------------------------------------------------------------
//元のサイズに戻す
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::RestoreActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		if (ap->isNyan) {
			for (int i=0; i<Screen->FormCount; i++) {
				TForm *fp = Screen->Forms[i];
				if (fp->Handle==ap->WinHandle) fp->WindowState = wsNormal;
			}
			SetFocus();
		}
		else {
			::ShowWindow(ap->WinHandle, SW_RESTORE);
			ModalResult = mrOk;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::RestoreActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction*)Sender)->Enabled = (ap && ap->WinHandle!=Application->MainForm->Handle);
}
//---------------------------------------------------------------------------
//最小化
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::MinimizeActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		if (ap->isNyan) {
			if (ap->WinHandle==Application->MainForm->Handle) {
				ModalResult = mrRetry;
			}
			else {
				for (int i=0; i<Screen->FormCount; i++) {
					TForm *fp = Screen->Forms[i];
					if (fp->Handle==ap->WinHandle) fp->WindowState = wsMinimized;
				}
			}
			SetFocus();
		}
		else {
			::ShowWindow(ap->WinHandle, SW_MINIMIZE);
		}
		AppListBox->Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::MinimizeActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction*)Sender)->Enabled = (ap!=NULL);
}
//---------------------------------------------------------------------------
//最大化
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::MaximizeActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		if (ap->isNyan) {
			for (int i=0; i<Screen->FormCount; i++) {
				TForm *fp = Screen->Forms[i];
				if (fp->Handle==ap->WinHandle) fp->WindowState = wsMaximized;
			}
			SetFocus();
		}
		else {
			::ShowWindow(ap->WinHandle, SW_MAXIMIZE);
			ModalResult = mrOk;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::MaximizeActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction*)Sender)->Enabled = (ap && ap->WinHandle!=Application->MainForm->Handle);
}

//---------------------------------------------------------------------------
//プライマリモニタに移動
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ToPrimaryActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap && !::IsIconic(ap->WinHandle)) {
		TRect rc = Screen->PrimaryMonitor->BoundsRect;
		if (ap->win_wd>rc.Width())  ap->win_wd = rc.Width();
		if (ap->win_hi>rc.Height()) ap->win_hi = rc.Height();
		rc.SetWidth(ap->win_wd);
		rc.SetHeight(ap->win_hi);
		set_window_pos_ex(ap->WinHandle, rc);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ToPrimaryActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction *)Sender)->Enabled = (ap && !::IsIconic(ap->WinHandle));
}

//---------------------------------------------------------------------------
//ファイルリストに合わせる
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FitToFileListActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		if (::IsIconic(ap->WinHandle)) ::ShowWindow(ap->WinHandle, SW_RESTORE);
		set_window_pos_ex(ap->WinHandle, UserModule->FileListRect);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FitToFileListActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction*)Sender)->Enabled = (ap && !ap->isNyan && Application->MainForm->WindowState!=wsMinimized);
}
//---------------------------------------------------------------------------
//常に手前に表示
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::TopMostActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		ap->topMost = !ap->topMost;
		::EndDeferWindowPos(
			::DeferWindowPos(::BeginDeferWindowPos(1), ap->WinHandle,
			(ap->topMost? HWND_TOPMOST : HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE));
		ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::TopMostActionUpdate(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	((TAction*)Sender)->Enabled = (ap && !ap->isNyan);
}
//---------------------------------------------------------------------------
//閉じる
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::CloseItemClick(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		ap->toClose = true;
		AppListBox->Invalidate();
		//内部
		if (ap->isNyan) {
			if (ap->WinHandle==Application->MainForm->Handle) {
				ModalResult = mrClose;
			}
			else {
				for (int i=0; i<Screen->FormCount; i++) {
					TForm *fp = Screen->Forms[i];
					if (fp->Handle==ap->WinHandle) fp->Close();
				}
			}
			SetFocus();
		}
		//外部
		else {
			::PostMessage(ap->WinHandle, WM_SYSCOMMAND, SC_CLOSE, 0);
		}
	}
}
//---------------------------------------------------------------------------
//まとめて閉じる
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::CloseSameItemClick(TObject *Sender)
{
	AppWinInf *c_ap = GetCurAppWinInf();
	if (c_ap) {
		if (c_ap->isNyan) {
			c_ap->toClose = true;
			AppListBox->Invalidate();
			ModalResult = mrClose;
		}
		else {
			for (int i=0; i<AppInfoList->Count; i++) {
				AppWinInf *ap = AppInfoList->Items[i];
				if (ap==c_ap || SameText(ap->FileName, c_ap->FileName)) {
					ap->toClose = true;
					AppListBox->Invalidate();
					::PostMessage(ap->WinHandle, WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//デスクトップを表示
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::DesktopItemClick(TObject *Sender)
{
	::keybd_event(VK_RWIN, 0, 0, 0);
	::keybd_event('D', 0, 0, 0);
	::keybd_event('D', 0, KEYEVENTF_KEYUP, 0);
	::keybd_event(VK_RWIN, 0, KEYEVENTF_KEYUP, 0);
	ModalResult = mrCancel;
}

//---------------------------------------------------------------------------
//強制修了
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::TerminateItemClick(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		ap->toClose = true;
		AppListBox->Invalidate();
		HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, ap->PID);
		if (hProcess) {
			if (!::TerminateProcess(hProcess, 0)) msgbox_ERR(USTR_FaildProc);
			::CloseHandle(hProcess);
		}
		else {
			msgbox_ERR(SysErrorMessage(GetLastError()));
			ap->toClose = false;
		}
	}
}

//---------------------------------------------------------------------------
//実行ファイル名をコピー
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::CopyNameItemClick(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) copy_to_Clipboard(ap->FileName);
}
//---------------------------------------------------------------------------
//実行ファイル位置に移動
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::JumpExeItemClick(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		JumpFileName = ap->FileName;
		ModalResult  = mrOk;
	}
}

//---------------------------------------------------------------------------
//除外テキストを設定
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ExcAppTextItemClick(TObject *Sender)
{
	if (!EditItemDlg) EditItemDlg = new TEditItemDlg(this);	//初回に動的作成
	EditItemDlg->AssignText("除外テキスト", ExcAppText);
	if (show_ModalDlg(EditItemDlg)==mrOk) {
		ExcAppText = EditItemDlg->RetStr;
		if (!OnlyLauncher) UpdateAppList();
	}
}

//---------------------------------------------------------------------------
//アプリケーション情報
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppInfoActionExecute(TObject *Sender)
{
	AppWinInf *c_ap = GetCurAppWinInf();
	if (c_ap) {
		cursor_HourGlass();
		const _TCHAR *fmt_08X   = _T("0x%08X");
		const _TCHAR *fmt_08X_U = _T("0x%08X (%u)");

		std::unique_ptr<TStringList> i_lst(new TStringList());
		UnicodeString tmp;
		add_PropLine(_T("キャプション"),	c_ap->WinText,	i_lst.get());
		tmp = ExtractFileName(c_ap->FileName);
		if (c_ap->isWow64) tmp += "  (32-bit)"; else tmp += "  (64-bit)";
		add_PropLine(_T("実行ファイル"),tmp, i_lst.get(), LBFLG_PATH_FIF);
		add_PropLine(_T("場所"),		yen_to_delimiter(ExtractFilePath(c_ap->FileName)), i_lst.get(), LBFLG_PATH_FIF);
		add_PropLine(_T("パラメータ"),	c_ap->CmdParam,	i_lst.get());

		i_lst->Add(get_img_size_str(c_ap->win_wd, 	c_ap->win_hi, "画面サイズ"));
		tmp.sprintf(_T("(%d, %d) - (%d, %d)"),
			c_ap->win_left, c_ap->win_top, c_ap->win_left + c_ap->win_wd, c_ap->win_top + c_ap->win_hi);
		add_PropLine(_T("矩形"),		tmp,	i_lst.get()),
		add_PropLine(_T("開始タイム"),	format_DateTime(c_ap->StartTime),	i_lst.get());
		i_lst->Add(EmptyStr);

		HWND hWnd = c_ap->WinHandle;
		tmp = c_ap->ClassName;
		if (!c_ap->ClassName2.IsEmpty()) tmp.cat_sprintf(_T(" (%s)"), c_ap->ClassName2.c_str());
		add_PropLine(_T("クラス名"),	tmp,	i_lst.get());
		add_PropLine(_T("スタイル"),	tmp.sprintf(fmt_08X, ::GetWindowLong(hWnd, GWL_STYLE)),	i_lst.get());

		tmp.sprintf(fmt_08X, c_ap->win_xstyle);
		UnicodeString tmp2;
		if (c_ap->win_xstyle & WS_EX_DLGMODALFRAME) tmp2 += "DLGMODALFRAME";
		if (c_ap->win_xstyle & WS_EX_TOPMOST) 		tmp2 += ", TOPMOST";
		if (c_ap->win_xstyle & WS_EX_TOOLWINDOW) 	tmp2 += ", TOOLWINDOW";
		if (c_ap->win_xstyle & WS_EX_WINDOWEDGE) 	tmp2 += ", WINDOWEDGE";
		if (c_ap->win_xstyle & WS_EX_CLIENTEDGE) 	tmp2 += ", CLIENTEDGE";
		if (c_ap->win_xstyle & WS_EX_STATICEDGE) 	tmp2 += ", STATICEDGE";
		if (c_ap->win_xstyle & WS_EX_APPWINDOW) 	tmp2 += ", APPWINDOW";
		if (c_ap->win_xstyle & WS_EX_LAYERED)		tmp2 += ", LAYERED";
		if (!tmp2.IsEmpty()) {
			remove_top_s(tmp2, _T(", "));
			tmp.cat_sprintf(_T(" (%s)"), tmp2.c_str());
		}
		add_PropLine(_T("拡張スタイル"),	tmp,	i_lst.get());

		i_lst->Add(EmptyStr);
		add_PropLine(_T("プロセスID"),		tmp.sprintf(fmt_08X_U, c_ap->PID, c_ap->PID),	i_lst.get());
		add_PropLine(_T("スレッドID"),		tmp.sprintf(fmt_08X_U, c_ap->TID, c_ap->TID),	i_lst.get());
		add_PropLine(_T("ハンドル"),		tmp.sprintf(fmt_08X_U, hWnd, hWnd),				i_lst.get());

		i_lst->Add(EmptyStr);
		get_AppInf(c_ap->FileName, i_lst.get(), false);
		cursor_Default();

		FileInfoDlg->ItemList->Assign(i_lst.get());
		FileInfoDlg->isAppInfo = true;
		FileInfoDlg->ShowModal();
	}
}

//---------------------------------------------------------------------------
//モニタ番号を表示
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ShowMonNoActionExecute(TObject *Sender)
{
	ShowMonNoAction->Checked = !ShowMonNoAction->Checked;
	AppListBox->Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ShowMonNoActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = (Screen->MonitorCount>1);
}
//---------------------------------------------------------------------------
//コマンドラインパラメータを表示
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::ShowCmdParamActionExecute(TObject *Sender)
{
	ShowCmdParamAction->Checked = !ShowCmdParamAction->Checked;
	AppListBox->Invalidate();
}

//---------------------------------------------------------------------------
//ランチャーリストの描画
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxDrawItem(TWinControl *Control, int Index,
		TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	cv->Font->Assign(lp->Font);
	cv->Brush->Color = (lp->Focused() && State.Contains(odSelected))? col_selItem : col_bgList;
	cv->FillRect(Rect);

	int xp = Rect.Left + ScaledInt(4, this);
	int yp = Rect.Top  + ScaledInt(2, this);

	file_rec *fp = (file_rec*)LaunchList->Objects[Index];
	//アイコン
	draw_SmallIcon(fp, cv, xp, Rect.Top + (Rect.Height() - ScaledInt(16, this))/2, true, this);
	xp += ScaledInt(18, this);

	//名前
	cv->Font->Color = fp->failed? col_Error :
	   (lp->Focused() && is_SelFgCol(State))? col_fgSelItem :
					  fp->is_dir? col_Folder: get_ExtColor(fp->f_ext);
	cv->TextOut(xp, yp, fp->b_name);

	//分割線
	if (Index>0 && !IsIncSea) {
		file_rec *fp2 = (file_rec*)LaunchList->Objects[Index - 1];
		if (!SameText(fp->f_ext, fp2->f_ext))	draw_separateLine(cv, Rect, 0);
		if (EndsStr('-', fp->alias)) 			draw_separateLine(cv, Rect, 2);	//コメント
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	file_rec *fp = (file_rec*)LaunchList->Objects[Index];
	Data = fp->b_name;
}

//---------------------------------------------------------------------------
//ランチャーのキー操作
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;
	UnicodeString CmdStr = KeyFuncList->Values[(IsIncSea? "S:" : "F:") + KeyStr];

	if (IsIncSea) {
		CmdStr = ReplaceText(CmdStr, "IncSearchUp",   "CursorUp");
		CmdStr = ReplaceText(CmdStr, "IncSearchDown", "CursorDown");
		CmdStr = ReplaceText(CmdStr, "IncSearchTop",  "CursorTop");
	}
	else if (CmdStr.IsEmpty()) {
		CmdStr = get_CsrKeyCmd(KeyStr);
	}

	TListBox *lp = (TListBox*)Sender;
	int idx = lp->ItemIndex;
	file_rec *cfp = GetLaunchFrecPtr();

	bool handled = true;
	//移動/実行
	if (equal_ENTER(KeyStr)) {
		if (cfp) {
			if (cfp->is_dir) {
				UnicodeString l_fnam;
				if (cfp->is_up) {
					l_fnam = ExcludeTrailingPathDelimiter(CurLaunchPath);
					CurLaunchPath = get_parent_path(CurLaunchPath);
				}
				else {
					l_fnam = "..";
					CurLaunchPath = IncludeTrailingPathDelimiter(cfp->f_name);
				}
				UpdateLaunchList(l_fnam);
			}
			else {
				LaunchFileName = cfp->f_name;
				if (test_LnkExt(cfp->f_ext)) {
					UnicodeString fnam = cv_env_str(usr_SH->get_LnkName(cfp->f_name));
					if (test_NbtExt(get_extension(fnam))) LaunchFileName = fnam;
				}
				ModalResult = mrOk;
			}
		}
	}
	//ESC
	else if (equal_ESC(KeyStr)) {
		if (IsIncSea)
			SetIncSeaMode(false);
		else
			ModalResult = mrCancel;
	}
	//インクリメンタルサーチモード
	else if (StartsText("IncSearch", CmdStr)) {
		SetIncSeaMode(!USAME_TI(CmdStr, "IncSearchExit"));
	}
	//キーワードクリア
	else if (USAME_TI(CmdStr, "ClearIncKeyword")) {
		IncSeaWord = EmptyStr;
		UpdateLaunchList();
	}
	//Migemoモード切替
	else if (contained_wd_i(_T("MigemoMode|NormalMode"), CmdStr)) {
		IsMigemo = USAME_TI(CmdStr, "MigemoMode")? (!IsMigemo && usr_Migemo->DictReady) : false;
		UpdateLaunchList();
	}
	//インクリメンタルサーチ
	else if (IsIncSea && update_IncSeaWord(IncSeaWord, KeyStr)) {
		UpdateLaunchList();
	}
	//名前の変更
	else if (USAME_TI(CmdStr, "RenameDlg")) {
		if (cfp) {
			UnicodeString new_name = cfp->b_name;
			if (input_query_ex(USTR_Rename, _T("名前"), &new_name)) {
				new_name = cfp->p_name + new_name;
				if (!cfp->is_dir) new_name += cfp->f_ext;
				if (move_FileT(cfp->f_name, new_name))
					UpdateLaunchList(new_name);
				else
					msgbox_ERR("改名に失敗しました。");
			}
		}
	}
	//削除
	else if (USAME_TI(CmdStr, "Delete")) {
		if (cfp && !cfp->is_up) {
			try {
				if (msgbox_Sure(LoadUsrMsg(USTR_DeleteQ,
					UnicodeString().sprintf(_T("%s "), get_DispName(cfp).c_str())), SureDelete))
				{
					if (ForceDel && !set_FileWritable(cfp->f_name)) UserAbort(USTR_FaildDelete);
					if (cfp->is_dir) {
						if (!delete_DirEx(cfp->f_name, DelUseTrash, ForceDel)) UserAbort(USTR_FaildDelete);
					}
					else {
						if (!delete_File(cfp->f_name, DelUseTrash)) UserAbort(USTR_FaildDelete);
					}
					UpdateLaunchList();
					ListBoxSetIndex(lp, idx);
				}
			}
			catch (EAbort &e) {
				msgbox_ERR(e.Message);
			}
		}
	}
	//ディレクトリの作成
	else if (USAME_TI(CmdStr, "CreateDir")) {
		InputExDlg->IpuntExMode = INPEX_CRE_DIR;
		InputExDlg->InputComboBox->Text = EmptyStr;
		UnicodeString inpstr = (InputExDlg->ShowModal()==mrOk)? InputExDlg->InputComboBox->Text : EmptyStr;
		if (!inpstr.IsEmpty()) {
			UnicodeString dnam = to_absolute_name(inpstr, CurLaunchPath);
			if (!dir_exists(dnam) && !create_ForceDirs(dnam))
				msgbox_ERR(LoadUsrMsg(USTR_CantCreDir, dnam));
			else
				UpdateLaunchList(dnam);
		}
	}
	//プロパティ
	else if (USAME_TI(CmdStr, "PropertyDlg")) {
		PropertyItem->Click();
	}
	//親ディレクトリへ
	else if (USAME_TI(get_CmdStr(CmdStr), "ToParent")) {
		if (!SameText(CurLaunchPath, LaunchPath)) {
			UnicodeString l_fnam = ExcludeTrailingPathDelimiter(CurLaunchPath);
			CurLaunchPath = get_parent_path(CurLaunchPath);
			UpdateLaunchList(l_fnam);
		}
		else {
			beep_Warn();
		}
	}
	//一覧へ
	else if (AppPanel->Visible && LaunchPanel->Align==alRight && is_ToLeftOpe(KeyStr, CmdStr)) {
		AppListBox->SetFocus();
	}
	else if (AppPanel->Visible && LaunchPanel->Align==alLeft  && is_ToRightOpe(KeyStr, CmdStr)) {
		AppListBox->SetFocus();
	}
	//一般コマンド
	else if (ExeCmdListBox(lp, CmdStr)) {
		;
	}
	//右クリックメニュー
	else if (contained_wd_i(KeysStr_Popup, KeyStr)) {
		show_PopupMenu(lp);
	}
	//頭文字サーチ
	else if (!IsIncSea && is_IniSeaKey(KeyStr)) {
		ListBoxInitialSearch(LaunchListBox, KeyStr);
	}
	else handled = false;

	UpdateLaunchSttBar();

	if (!is_DialogKey(Key) || handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxClick(TObject *Sender)
{
	UpdateLaunchSttBar();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchListBoxDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//トップディレクトリを設定
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SelLaunhDirItemClick(TObject *Sender)
{
	UnicodeString dnam = CurLaunchPath;
	if (UserModule->SelectDirEx(_T("ランチャーのトップディレクトリを設定"), dnam)) {
		CurLaunchPath = LaunchPath = to_path_name(dnam);
		UpdateLaunchList();
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::DirPanelDblClick(TObject *Sender)
{
	SelLaunhDirItem->Click();
}
//---------------------------------------------------------------------------
//このディレクトリに移動
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::JumpLaunchDirItemClick(TObject *Sender)
{
	UnicodeString pnam = CurLaunchPath;
	if (IsIncSea) {
		file_rec *cfp = GetLaunchFrecPtr();
		if (cfp) pnam = cfp->p_name;
	}

	JumpPathName = pnam;
	ModalResult  = mrOk;
}
//---------------------------------------------------------------------------
//リンク先に移動
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::JumpLinkedActionExecute(TObject *Sender)
{
	file_rec *cfp = GetLaunchFrecPtr();
	if (cfp && test_LnkExt(cfp->f_ext)) {
		JumpFileName = cv_env_str(usr_SH->get_LnkName(cfp->f_name));
		ModalResult  = mrOk;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::JumpLinkedActionUpdate(TObject *Sender)
{
	file_rec *cfp = GetLaunchFrecPtr();
	((TAction*)Sender)->Enabled = (cfp && test_LnkExt(cfp->f_ext));
}
//---------------------------------------------------------------------------
//ランチャーの表示位置
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::LaunchPosItemClick(TObject *Sender)
{
	set_PanelAlign(LaunchPanel, LRSplitter, ((TMenuItem*)Sender)->Tag, 5);
}

//---------------------------------------------------------------------------
//フォルダアイコンでソート
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SortByIconActionExecute(TObject *Sender)
{
	SortByIcon = !SortByIcon;
	UpdateLaunchList();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SortByIconActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !IsIncSea;
	ap->Checked = SortByIcon;
}
//---------------------------------------------------------------------------
//コメントでソート
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SortByRemActionExecute(TObject *Sender)
{
	SortByRem = !SortByRem;
	UpdateLaunchList();
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SortByRemActionUpdate(TObject *Sender)
{
	TAction *ap = (TAction*)Sender;
	ap->Enabled = !IsIncSea;
	ap->Checked = SortByRem;
}

//---------------------------------------------------------------------------
//アイコンを更新
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateIcoItemClick(TObject *Sender)
{
	for (int i=0; i<LaunchList->Count; i++)
		del_CachedIcon(((file_rec*)LaunchList->Objects[i])->f_name);

	UpdateLaunchList();
}

//---------------------------------------------------------------------------
//プロパティ
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::PropertyItemClick(TObject *Sender)
{
	file_rec *cfp = GetLaunchFrecPtr();
	if (cfp) {
		pos_ListBoxItem(LaunchListBox);
		ShowPropertyDialog(cfp->is_up? ExcludeTrailingPathDelimiter(cfp->p_name) : cfp->f_name);
	}
}

//---------------------------------------------------------------------------
//ホイール操作
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormMouseWheel(TObject *Sender, TShiftState Shift, int WheelDelta,
	TPoint &MousePos, bool &Handled)
{
	if (AppListBox->Focused()) {
		ExeCmdListBox(AppListBox, (WheelDelta<0)? _T("CursorDown") : _T("CursorUp"));
		UpdateAppSttBar();
		Handled = true;
	}
	else if (LaunchListBox->Focused()) {
		ExeCmdListBox(LaunchListBox, (WheelDelta<0)? _T("CursorDown") : _T("CursorUp"));
		UpdateLaunchSttBar();
		Handled = true;
	}
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}

//---------------------------------------------------------------------------
//入力欄の描画
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::InpPaintBoxPaint(TObject *Sender)
{
	draw_InputPaintBox((TPaintBox*)Sender, (IsMigemo? "Migemo: " : "フィルタ: ") + IncSeaWord);
}
//---------------------------------------------------------------------------

