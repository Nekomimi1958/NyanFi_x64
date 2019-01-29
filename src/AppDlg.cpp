//----------------------------------------------------------------------//
// NyanFi																//
//  アプリケーション一覧												//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <algorithm>
#include <RegularExpressions.hpp>
#include "UserFunc.h"
#include "UserMdl.h"
#include "Global.h"
#include "InpDir.h"
#include "InpExDlg.h"
#include "FileInfDlg.h"
#include "AppDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAppListDlg *AppListDlg = NULL;

//---------------------------------------------------------------------------
HINSTANCE hPsApi = NULL;
FUNC_GetProcessMemoryInfo lpfGetProcessMemoryInfo = NULL;


//---------------------------------------------------------------------------
//アプリケーション情報クラス
//---------------------------------------------------------------------------
AppWinInf::AppWinInf()
{
	WinHandle = NULL;
	toClose	  = false;
	isNoRes	  = false;
	Icon	  = new TIcon();
	hThumb	  = NULL;
}
//---------------------------------------------------------------------------
AppWinInf::~AppWinInf()
{
	delete Icon;

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
	HINSTANCE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
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

	AppScrPanel    = new UsrScrollPanel(AppListPanel, AppListBox,	 USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	LaunchScrPanel = new UsrScrollPanel(LaunchPanel,  LaunchListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	AppInfoList = new TAppWinList();

	LaunchFileList = new TStringList();

	ToAppList = ToLauncher = ToIncSea = false;
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormShow(TObject *Sender)
{
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

	StatusBar1->Font->Assign(SttBarFont);
	StatusBar1->ClientHeight = get_FontHeight(SttBarFont, 4, 4);

	LaunchPath = IncludeTrailingPathDelimiter(IniFile->ReadStrGen(_T("AppListLaunchPath"), ExePath));
	if (!dir_exists(LaunchPath)) LaunchPath = ExePath;
	CurLaunchPath = LaunchPath;
	IsMigemo = IniFile->ReadBoolGen(_T("AppListLaunchMigemo"));

	AppInfoList->Clear();

	//一覧初期化
	TListBox *lp = AppListBox;
	set_StdListBox(lp, LBTAG_OPT_LOOP);
	set_UsrScrPanel(AppScrPanel);
	int min_hi = ScaledInt(36);
	if (lp->ItemHeight<min_hi) lp->ItemHeight = min_hi;
	lp->Count = 0;
	lp->ItemIndex = -1;

	MaxWd_f = 0;
	if (!OnlyLauncher) UpdateAppList();

	//ランチャーの初期化
	if (!OnlyAppList) {
		SortByRem = IniFile->ReadBoolGen(_T("AppListLaunchRemSort"));
		lp = LaunchListBox;
		set_StdListBox(lp, LBTAG_OPT_LOOP|LBTAG_HAS_SICO);
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

	if (ToAppList)
		AppListBox->SetFocus();
	else if (OnlyLauncher || ToLauncher || ToIncSea) {
		LaunchListBox->SetFocus();
		UpdateLaunchSttBar();
	}

	if (!OnlyLauncher) Timer1->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	Timer1->Enabled = false;

	AppInfoList->Clear();
	LaunchFileList->Clear();
	clear_FileList(LaunchList);

	if (OnlyLauncher)
		IniFile->SavePosInfo(this, "Launcher");
	else {
		IniFile->SavePosInfo(this);
		IniFile->WriteIntGen(_T("AppListThumbHi"),		ViewPanel->Height);
	}

	if (!OnlyAppList && !OnlyLauncher)
		IniFile->WriteIntGen(_T("AppListLaunchPos"),	(LaunchPanel->Align==alLeft)? 2 : 1);

	if (!OnlyAppList) {
		IniFile->WriteIntGen( _T("AppListLaunchWd"),		LaunchPanel->Width);
		IniFile->WriteBoolGen(_T("AppListLaunchRemSort"),	SortByRem);
		IniFile->WriteStrGen( _T("AppListLaunchPath"),		LaunchPath);
		IniFile->WriteBoolGen(_T("AppListLaunchMigemo"),	IsMigemo);
	}

	OnlyLauncher = OnlyAppList = false;
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FormDestroy(TObject *Sender)
{
	delete AppInfoList;
	delete LaunchFileList;

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
	cv->TextOut(Rect.Left + 2, Rect.Top, Panel->Text);
}

//---------------------------------------------------------------------------
//ランチャーアイコンの処理
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::WmNyanFiFlIcon(TMessage &msg)
{
	LaunchListBox->Invalidate();
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
				if (!test_FileExt(get_extension(fnam), _T(".lnk.url"))) continue;
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
	else szstr.USET_T("???? KB");
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
	IsIncSea = sw;
	//インクリメンタルサーチモード
	if (IsIncSea) {
		DirPanel->Font->Assign(ListFont);
		DirPanel->ClientHeight = get_FontHeight(ListFont, 4, 4);
		DirPanel->Color 	   = col_bgList;
		DirPanel->BevelOuter   = bvLowered;
		DirPanel->Caption	   = EmptyStr;
		InpPaintBox->Visible   = true;

		IncSeaWord = EmptyStr;
		LaunchFileList->Clear();
		get_files(LaunchPath, _T("*.lnk"), LaunchFileList, true);
		get_files(LaunchPath, _T("*.url"), LaunchFileList, true);
	}
	//通常表示
	else {
		InpPaintBox->Visible   = false;;
		DirPanel->Font->Assign(DirInfFont);
		DirPanel->ClientHeight = get_FontHeight(DirInfFont, 6, 6);
		DirPanel->Color 	   = col_bgDirInf;
		DirPanel->Font->Color  = col_fgDirInf;
		DirPanel->Alignment    = taCenter;
		DirPanel->BevelOuter   = FlatInfPanel? bvNone : bvRaised;
	}

	UpdateLaunchList();
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

			//拡張ウィンドウスタイル
			LONG w_style = ::GetWindowLong(hWnd, GWL_EXSTYLE);
			if (w_style & WS_EX_TOOLWINDOW) break;
			if ((w_style & WS_EX_LAYERED) && !(w_style & WS_EX_WINDOWEDGE)) break;
			if (::GetParent(hWnd) && !(w_style & WS_EX_APPWINDOW)) break;

			//ウィンドウサイズ
			TRect w_rect;
			::GetWindowRect(hWnd, &w_rect);
			if (w_rect.IsEmpty()) break;

			//テキスト
			UnicodeString wtxt = get_WndText(hWnd);
			if (wtxt.IsEmpty()) break;
			if (USAME_TI(wtxt, "Windows シェル エクスペリエンス ホスト")) break;
			if (USAME_TI(cnam, "Windows.UI.Core.CoreWindow")) break;
			if (USAME_TI(cnam, "ApplicationFrameWindow") && USAME_TS(wtxt, "Microsoft Edge")) break;	//*** 
			if (ProcessId==pid && contained_wd_i("TAppListDlg|HH Parent", cnam)) break;

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
			ap->PID 	   = pid;
			ap->TID 	   = tid;
			ap->Exist	   = true;
			ap->isNyan	   = (ProcessId==pid);
			ap->WinText    = wtxt;
			ap->ClassName  = cnam;
			ap->topMost    = (w_style & WS_EX_TOPMOST);
			ap->win_wd	   = w_rect.Width();
			ap->win_hi	   = w_rect.Height();
			ap->win_left   = w_rect.Left;
			ap->win_top    = w_rect.Top;
			ap->win_xstyle = w_style;
			ap->isWow64    = false;
			ap->isUWP	   = USAME_TI(cnam, "ApplicationFrameWindow");

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
					//無応答でなければアイコンを取得(UWPは除く)
					if (!ap->isNoRes && !ap->isUWP)
						ap->Icon->Handle = (HICON)::SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);

					hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
					if (hProcess) {
						_TCHAR sbuf[1024];
						//ファイル名
						DWORD sz = 1023;
						UnicodeString fnam = ::QueryFullProcessImageName(hProcess, 0, sbuf, &sz)? sbuf : _T("");
						::CloseHandle(hProcess);
						ap->FileName = fnam;
						ap->Caption	 = get_base_name(fnam);

						if (ap->Icon->Empty && !fnam.IsEmpty()) {
							SHFILEINFO sif;
							if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_LARGEICON))
								ap->Icon->Handle = sif.hIcon;
						}
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
//ランチャーリストの更新
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::UpdateLaunchList(UnicodeString lnam)
{
	if (IsIncSea) InpPaintBox->Repaint();

	TStringList *lst = LaunchList;
	clear_FileList(lst);

	//インクリメンタルサーチ
	if (IsIncSea) {
		UnicodeString ptn = usr_Migemo->GetRegExPtn(IsMigemo, IncSeaWord);
		if (!ptn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			for (int i=0; i<LaunchFileList->Count; i++) {
				UnicodeString fnam = LaunchFileList->Strings[i];
				if (TRegEx::IsMatch(get_base_name(fnam), ptn, opt)) {
					file_rec *fp = cre_new_file_rec(fnam);  if (!fp) continue;
					lst->AddObject(fp->f_name, (TObject*)fp);
				}
			}
		}
	}
	//通常モード
	else {
		DirPanel->Caption = get_MiniPathName(ExtractFileName(ExcludeTrailingPathDelimiter(CurLaunchPath)),
								DirPanel->ClientWidth, DirPanel->Font);
		TSearchRec sr;
		if (FindFirst(UAPP_T(CurLaunchPath, "*.*"), faAnyFile, sr)==0) {
			do {
				UnicodeString fnam = sr.Name;	if (USAME_TS(fnam, ".")) continue;
				if (sr.Attr & faDirectory) {
					if (USAME_TS(fnam, "..")) {
						if (SameText(CurLaunchPath, LaunchPath)) continue;
					}
					else fnam.UCAT_T("\\");
				}
				else if (!test_FileExt(get_extension(fnam), _T(".lnk.url"))) continue;

				if (!USAME_TS(fnam, "..")) fnam = CurLaunchPath + fnam;
				file_rec *fp = cre_new_file_rec(fnam);  if (!fp) continue;
				//fp->l_name = リンク先, fp->alias = コメント
				if (SortByRem && test_LnkExt(fp->f_ext)) {
					usr_SH->get_LnkInf(fp->f_name, NULL, &fp->l_name, NULL, NULL, NULL, &fp->alias);
				}
				//URL
				else if (SortByRem && USAME_TI(fp->f_ext, ".url")) {
					std::unique_ptr<UsrIniFile> url_file(new UsrIniFile(fnam));
					fp->l_name = url_file->ReadString("InternetShortcut", "URL");
				}
				lst->AddObject(fp->f_name, (TObject*)fp);
			} while(FindNext(sr)==0);
			FindClose(sr);
		}
		lst->CustomSort(SortComp_Ext);
	}

	//リストボックスに割り当て(仮想)
	TListBox *lp = LaunchListBox;
	int idx =lp->ItemIndex;
	lp->Count = lst->Count;
	Application->ProcessMessages();
	if		(USAME_TS(lnam, ".."))	idx = 0;
	else if (!lnam.IsEmpty())		idx = lst->IndexOf(lnam);
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
			if (!cfp->alias.IsEmpty())  msg.cat_sprintf(_T(" (%s)"),  cfp->alias.c_str());	//コメント
			if (!cfp->l_name.IsEmpty()) msg.cat_sprintf(_T(" → %s"), cfp->l_name.c_str());	//リンク先
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
		if (::IsIconic(c_ap->WinHandle)) msg.UCAT_T(" (min)");
		if (c_ap->isNoRes) msg.UCAT_T(" (無応答)");
		msg.UCAT_T("  Started:");
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

	int xp = Rect.Left + 4;

	//無応答表示
	if (ap->isNoRes) {
		TRect rc = Rect;
		rc.Left	 = xp + 36;
		rc.Right = xp + 44 + MaxWd_f + get_CharWidth(cv,  2);
		InflateRect(rc, 0, -2);
		TColor br_col	 = cv->Brush->Color;
		cv->Brush->Color = col_Error;
		cv->FrameRect(rc);
		cv->Brush->Color = br_col;
	}

	//アンコン
	if (ap->Icon->Handle) {
		::DrawIconEx(cv->Handle, xp, Rect.Top + 2, ap->Icon->Handle, 32, 32, 0, NULL, DI_NORMAL);
	}
	//UWP用擬似アイコン
	else if (ap->isUWP) {
		TRect rc = Rect;
		rc.Left = xp; rc.Top += 2;
		rc.SetWidth(32); rc.SetHeight(32);
		cv->Brush->Color = TColor(0x00d5780a);
		cv->FillRect(rc);
		int org_fs = cv->Font->Size;
		{
			bool is_edge = EndsStr("Microsoft Edge", ap->WinText);
			UnicodeString tit = is_edge? "ｅ" : "UWP";
			if (is_edge) cv->Font->Style = cv->Font->Style << fsBold;
			for (int sz=8;;sz++) {
				cv->Font->Height = sz;
				if (cv->TextWidth(tit)>24 || cv->TextHeight(tit)>28) {
					cv->Font->Height = sz -1;	break;
				}
			}
			cv->Font->Color = clWhite;
			cv->TextOut(rc.Left + (32 - cv->TextWidth(tit))/2, rc.Top + (is_edge? 0 : (32 - cv->TextHeight(tit))/2), tit);
			cv->Font->Style = cv->Font->Style >> fsBold;
		}
		cv->Font->Size = org_fs;
	}

	//最小化マーク
	if (::IsIconic(ap->WinHandle)) {
		TRect rc(xp + 22, Rect.Top + 24, xp + 34, Rect.Top + 36);
		cv->Brush->Color = scl_BtnFace;
		cv->FillRect(rc);
		draw_Line(cv, rc.Left + 2, rc.Bottom - 3, rc.Right -2, rc.Bottom - 3, 2, scl_BtnText);
		::DrawEdge(cv->Handle, &rc, BDR_RAISEDOUTER, BF_RECT);
	}
	xp += 40;
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
	xp += get_CharWidth(cv,  2);
	//名前
	TColor col_x = get_ExtColor(ExtractFileExt(ap->FileName));
	cv->Font->Color = (lp->Focused() && use_fgsel)? col_fgSelItem : col_x;
	cv->TextOut(xp, yp, ap->Caption);
	if (ap->isWow64) cv->TextOut(xp + get_TextWidth(cv, ap->Caption, is_irreg), yp, ISWOW64_STR);
	xp += MaxWd_f + 12;
	//テキスト
	cv->Font->Color = (lp->Focused() && use_fgsel)? col_fgSelItem : col_fgList;
	cv->TextOut(xp, yp, ap->WinText);
}
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = AppInfoList->Items[Index]->Caption;
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
	else if (LaunchPanel->Visible && LaunchPanel->Align==alRight && contained_wd_i(_T("ToRight|ToParentOnRight"), CmdStr))
		LaunchListBox->SetFocus();
	else if (LaunchPanel->Visible && LaunchPanel->Align==alLeft  && contained_wd_i(_T("ToLeft|ToParentOnLeft"), CmdStr))
		LaunchListBox->SetFocus();
	//ウィンドウ操作
	else if (USAME_TI(CmdStr, "WinMinimize"))	MinimizeAction->Execute();
	else if (USAME_TI(CmdStr, "WinMaximize"))	MaximizeAction->Execute();
	else if (USAME_TI(CmdStr, "WinNormal"))		RestoreAction->Execute();
	else if (USAME_TI(CmdStr, "Delete"))		CloseItemClick(NULL);
	else if (ExeCmdListBox(lp, CmdStr))			;
	//右クリックメニュー
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	//頭文字サーチ
	else if (is_IniSeaKey(KeyStr))	ListBoxInitialSearch(AppListBox, KeyStr);
	else if (equal_ESC(KeyStr)) 	ModalResult = mrCancel;
	//アプリケーション情報
	else if (StartsText("ShowFileInfo", CmdStr) || USAME_TI(CmdStr, "ListFileInfo"))
		AppInfoAction->Execute();
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
	if (OnlyLauncher) 
		LaunchPosItem->Visible	 = false;
	else {
		LaunchPosItem->Visible	 = !OnlyLauncher;
		LaunchRightItem->Checked = (LaunchPanel->Align==alRight);
		LaunchLeftItem->Checked  = (LaunchPanel->Align==alLeft);
	}
	SortByRemItem->Checked = SortByRem;
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
		else
			::ShowWindow(ap->WinHandle, SW_MINIMIZE);
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
//ファイルリストに合わせる
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::FitToFileListActionExecute(TObject *Sender)
{
	AppWinInf *ap = GetCurAppWinInf();
	if (ap) {
		if (::IsIconic(ap->WinHandle)) ::ShowWindow(ap->WinHandle, SW_RESTORE);
		TRect rc = UserModule->FileListRect;
		::SetWindowPos(ap->WinHandle, HWND_TOP, rc.Left, rc.Top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
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
		else
			::PostMessage(ap->WinHandle, WM_SYSCOMMAND, SC_CLOSE, 0);
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
//アプリケーション情報を表示
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::AppInfoActionExecute(TObject *Sender)
{
	TListBox *lp = FileInfoDlg->InfListBox;
	lp->Clear();

	AppWinInf *c_ap = GetCurAppWinInf();
	if (c_ap) {
		cursor_HourGlass();
		std::unique_ptr<TStringList> lst(new TStringList());
		const _TCHAR *fmt_08X   = _T("0x%08X");
		const _TCHAR *fmt_08X_U = _T("0x%08X (%u)");
		UnicodeString tmp;
		add_PropLine(_T("キャプション"),	c_ap->WinText,									lst.get());
		tmp = ExtractFileName(c_ap->FileName);
		if (c_ap->isWow64) tmp.UCAT_T("  (32-bit)"); else tmp.UCAT_T("  (64-bit)");
		add_PropLine(_T("実行ファイル"),tmp, lst.get(), LBFLG_PATH_FIF);
		add_PropLine(_T("実行パス"),	yen_to_delimiter(ExtractFilePath(c_ap->FileName)), lst.get(), LBFLG_PATH_FIF);
		lst->Add(get_img_size_str(c_ap->win_wd, 	c_ap->win_hi, "画面サイズ"));
		tmp.sprintf(_T("(%d, %d) - (%d, %d)"), 
			c_ap->win_left, c_ap->win_top, c_ap->win_left + c_ap->win_wd, c_ap->win_top + c_ap->win_hi);
		add_PropLine(_T("矩形"),		tmp,	lst.get()),
		add_PropLine(_T("開始タイム"),	format_DateTime(c_ap->StartTime),	lst.get());
		lst->Add(EmptyStr);

		HWND hWnd = c_ap->WinHandle;
		add_PropLine(_T("クラス名"),		c_ap->ClassName,	lst.get());
		add_PropLine(_T("スタイル"),		tmp.sprintf(fmt_08X, ::GetWindowLong(hWnd, GWL_STYLE)),	lst.get());

		tmp.sprintf(fmt_08X, c_ap->win_xstyle);
		UnicodeString tmp2;
		if (c_ap->win_xstyle & WS_EX_DLGMODALFRAME) tmp2.UCAT_T("DLGMODALFRAME");
		if (c_ap->win_xstyle & WS_EX_TOPMOST) 		tmp2.UCAT_T(", TOPMOST");
		if (c_ap->win_xstyle & WS_EX_TOOLWINDOW) 	tmp2.UCAT_T(", TOOLWINDOW");
		if (c_ap->win_xstyle & WS_EX_WINDOWEDGE) 	tmp2.UCAT_T(", WINDOWEDGE");
		if (c_ap->win_xstyle & WS_EX_CLIENTEDGE) 	tmp2.UCAT_T(", CLIENTEDGE");
		if (c_ap->win_xstyle & WS_EX_STATICEDGE) 	tmp2.UCAT_T(", STATICEDGE");
		if (c_ap->win_xstyle & WS_EX_APPWINDOW) 	tmp2.UCAT_T(", APPWINDOW");
		if (c_ap->win_xstyle & WS_EX_LAYERED)		tmp2.UCAT_T(", LAYERED");
		if (!tmp2.IsEmpty()) {
			remove_top_s(tmp2, _T(", "));
			tmp.cat_sprintf(_T(" (%s)"), tmp2.c_str());
		}
		add_PropLine(_T("拡張スタイル"),	tmp,	lst.get());

		lst->Add(EmptyStr);
		add_PropLine(_T("プロセスID"),		tmp.sprintf(fmt_08X_U, c_ap->PID, c_ap->PID),	lst.get());
		add_PropLine(_T("スレッドID"),		tmp.sprintf(fmt_08X_U, c_ap->TID, c_ap->TID),	lst.get());
		add_PropLine(_T("ハンドル"),		tmp.sprintf(fmt_08X_U, hWnd, hWnd),				lst.get());
		lst->Add(EmptyStr);
		add_PropLine(_T("ウインドウID"),	tmp.sprintf(fmt_08X, ::GetWindowLongPtr(hWnd, GWL_ID)),			lst.get());
		add_PropLine(_T("インスタンス"),	tmp.sprintf(fmt_08X, ::GetWindowLongPtr(hWnd, GWLP_HINSTANCE)),	lst.get());
		add_PropLine(_T("プロシージャ"),	tmp.sprintf(fmt_08X, ::GetWindowLongPtr(hWnd, GWLP_WNDPROC)),	lst.get());
		add_PropLine(_T("ユーザデータ"),	tmp.sprintf(fmt_08X, ::GetWindowLongPtr(hWnd, GWLP_USERDATA)),	lst.get());
		lst->Add(EmptyStr);
		get_AppInf(c_ap->FileName, lst.get(), false);

		assign_InfListBox(lp, lst.get());
		cursor_Default();
	}

	FileInfoDlg->isAppInfo = true;
	FileInfoDlg->ShowModal();
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

	int xp = Rect.Left + 4;
	int yp = Rect.Top + 2;

	file_rec *fp = (file_rec*)LaunchList->Objects[Index];
	//アイコン
	draw_SmallIcon(fp, cv, xp, Rect.Top + (Rect.Height() - SIcoSize)/2, true);
	xp += SIcoSize + Scaled2;

	//名前
	cv->Font->Color = (lp->Focused() && is_SelFgCol(State))? col_fgSelItem :
						fp->is_dir? col_Folder: get_ExtColor(fp->f_ext);
	cv->TextOut(xp, yp, fp->b_name);

	//分割線
	if (Index>0) {
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
	UnicodeString CmdStr = KeyFuncList->Values[(IsIncSea? "S:": "F:") + KeyStr];

	if (IsIncSea) {
		CmdStr = REPLACE_TI(CmdStr, "IncSearchUp",   "CursorUp");
		CmdStr = REPLACE_TI(CmdStr, "IncSearchDown", "CursorDown");
	}
	else if (CmdStr.IsEmpty())
		CmdStr = get_CsrKeyCmd(KeyStr);

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
					l_fnam.USET_T("..");
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
				if (cfp->is_dir && !is_EmptyDir(cfp->f_name)) SysErrAbort(ERROR_DIR_NOT_EMPTY);
				if (msgbox_Sure(LoadUsrMsg(USTR_DeleteQ,
					UnicodeString().sprintf(_T("%s "), get_DispName(cfp).c_str())), SureDelete))
				{
					if (ForceDel && !set_FileWritable(cfp->f_name)) UserAbort(USTR_FaildDelete);
					if (cfp->is_dir) {
						if (!delete_Dir(cfp->f_name)) UserAbort(USTR_FaildDelete);
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
			UnicodeString dnam = rel_to_absdir(inpstr, CurLaunchPath);
			if (!dir_exists(dnam) && !create_ForceDirs(dnam))
				msgbox_ERR(LoadUsrMsg(USTR_CantCreDir, dnam));
			else
				UpdateLaunchList(dnam);
		}
	}
	//プロパティ
	else if (USAME_TI(CmdStr, "PropertyDlg")) {
		PropertyItemClick(NULL);
	}
	//親ディレクトリへ
	else if (USAME_TI(CmdStr, "ToParent")) {
		if (!SameText(CurLaunchPath, LaunchPath)) {
			UnicodeString l_fnam = ExcludeTrailingPathDelimiter(CurLaunchPath);
			CurLaunchPath = get_parent_path(CurLaunchPath);
			UpdateLaunchList(l_fnam);
		}
		else beep_Warn();
	}
	//一覧へ
	else if (AppPanel->Visible && LaunchPanel->Align==alRight && contained_wd_i(_T("ToLeft|ToParentOnLeft"), CmdStr))
		AppListBox->SetFocus();
	else if (AppPanel->Visible && LaunchPanel->Align==alLeft  && contained_wd_i(_T("ToRight|ToParentOnRight"), CmdStr))
		AppListBox->SetFocus();

	//一般コマンド
	else if (ExeCmdListBox(lp, CmdStr))				;
	//右クリックメニュー
	else if (contained_wd_i(KeysStr_Popup, KeyStr))	show_PopupMenu(lp);
	//頭文字サーチ
	else if (!IsIncSea && is_IniSeaKey(KeyStr))		ListBoxInitialSearch(LaunchListBox, KeyStr);
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
	SelLaunhDirItemClick(NULL);
}
//---------------------------------------------------------------------------
//このディレクトリに移動
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::JumpLaunchDirItemClick(TObject *Sender)
{
	JumpPathName = LaunchPath;
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
//コメントでソート
//---------------------------------------------------------------------------
void __fastcall TAppListDlg::SortByRemItemClick(TObject *Sender)
{
	SortByRem = !SortByRem;
	UpdateLaunchList();
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
	}
	else if (LaunchListBox->Focused()) {
		ExeCmdListBox(LaunchListBox, (WheelDelta<0)? _T("CursorDown") : _T("CursorUp"));
		UpdateLaunchSttBar();
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

