//----------------------------------------------------------------------//
// NyanFi																//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>

//---------------------------------------------------------------------------
USEFORM("RenDlg.cpp", RenameDlg);
USEFORM("SameDlg.cpp", SameNameDlg);
USEFORM("ShareDlg.cpp", NetShareDlg);
USEFORM("PackDlg.cpp", PackArcDlg);
USEFORM("PrnImgDlg.cpp", PrintImgDlg);
USEFORM("RegExChk.cpp", RegExChecker);
USEFORM("Splash.cpp", SplashForm);
USEFORM("TagDlg.cpp", TagManDlg);
USEFORM("TaskDlg.cpp", TaskManDlg);
USEFORM("SrtModDlg.cpp", SortModeDlg);
USEFORM("SyncDlg.cpp", RegSyncDlg);
USEFORM("TabDlg.cpp", TabSetDlg);
USEFORM("LoupeFrm.cpp", LoupeForm);
USEFORM("MainFrm.cpp", NyanFiForm);
USEFORM("InspectFrm.cpp", InspectForm);
USEFORM("JoinDlg.cpp", JoinTextDlg);
USEFORM("KeyDlg.cpp", KeyListDlg);
USEFORM("MaskDlg.cpp", PathMaskDlg);
USEFORM("NewDlg.cpp", NewFileDlg);
USEFORM("OptDlg.cpp", OptionDlg);
USEFORM("MaskSelDlg.cpp", MaskSelectDlg);
USEFORM("MemoFrm.cpp", MemoForm);
USEFORM("ModalScr.cpp", ModalScrForm);
USEFORM("XmlView.cpp", XmlViewer);
USEFORM("UserMdl.cpp", UserModule); /* TDataModule: File Type */
USEFORM("CvEncDlg.cpp", CvTxtEncDlg);
USEFORM("CvImgDlg.cpp", CvImageDlg);
USEFORM("DebugFrm.cpp", DebugForm);
USEFORM("CompDlg.cpp", FileCompDlg);
USEFORM("CreDirsDlg.cpp", CreateDirsDlg);
USEFORM("CsvRecFrm.cpp", CsvRecForm);
USEFORM("DiffDlg.cpp", DiffDirDlg);
USEFORM("DriveDlg.cpp", SelDriveDlg);
USEFORM("DrvGraph.cpp", DriveGraph);
USEFORM("DuplDlg.cpp", FindDuplDlg);
USEFORM("DirDlg.cpp", RegDirDlg);
USEFORM("DistrDlg.cpp", DistributionDlg);
USEFORM("DotDlg.cpp", DotNyanDlg);
USEFORM("BakDlg.cpp", BackupDlg);
USEFORM("BitmapFrm.cpp", BitmapForm);
USEFORM("BtnDlg.cpp", ToolBtnDlg);
USEFORM("About.cpp", AboutBox);
USEFORM("AppDlg.cpp", AppListDlg);
USEFORM("AskRep.cpp", AskRepDlg);
USEFORM("CalcDlg.cpp", Calculator);
USEFORM("CmdListDlg.cpp", CmdFileListDlg);
USEFORM("ColDlg.cpp", ColorDlg);
USEFORM("ColPicker.cpp", ColorPicker);
USEFORM("ChInfFrm.cpp", CharInfoForm);
USEFORM("ChmodDlg.cpp", FtpChmodDlg);
USEFORM("GraphFrm.cpp", GraphForm);
USEFORM("GrepOptDlg.cpp", GrepExOptDlg);
USEFORM("HistDlg.cpp", DirHistoryDlg);
USEFORM("GenInfDlg.cpp", GeneralInfoDlg);
USEFORM("GifView.cpp", GifViewer);
USEFORM("HistFrm.cpp", HistForm);
USEFORM("InpCmds.cpp", InpCmdsDlg);
USEFORM("InpDir.cpp", InpDirDlg);
USEFORM("InpExDlg.cpp", InputExDlg);
USEFORM("ExpCsv.cpp", ExpCsvDlg);
USEFORM("ExTViewer.cpp", ExTxtViewer);
USEFORM("FileExtDlg.cpp", FileExtensionDlg);
USEFORM("EditHistDlg.cpp", EditHistoryDlg);
USEFORM("EditItem.cpp", EditItemDlg);
USEFORM("ExeDlg.cpp", ExeCmdDlg);
USEFORM("FileInfDlg.cpp", FileInfoDlg);
USEFORM("FindTxtDlg.cpp", FindTextDlg);
USEFORM("FtpDlg.cpp", FtpConnectDlg);
USEFORM("FuncDlg.cpp", FuncListDlg);
USEFORM("FindDlg.cpp", FindFileDlg);
USEFORM("FindKey.cpp", FindKeyDlg);
USEFORM("FindTag.cpp", FindTagForm);
//---------------------------------------------------------------------------
#include "Global.h"
#include "Splash.h"

//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, int)
{
	bool restart   = false;
	bool duplicate = false;
	bool not_act   = false;

	StartedCount = GetTickCount();

	ExePath = REPLACE_TS(ExtractFilePath(Application->ExeName), "\\.\\", "\\");	//IDE実行時の対策

	//起動時オプションを取得
	std::unique_ptr<TStringList> opt_lst(new TStringList());
	split_cmd_line(lpCmdLine, opt_lst.get());

	//INIファイル名の設定
	UnicodeString inam;
	for (int i=0; i<opt_lst->Count; i++) {
		UnicodeString lbuf = opt_lst->Strings[i];
		if (remove_top_s(lbuf, "-I"))
			inam = rel_to_absdir(exclude_quot(lbuf));
		if (StartsText("-XCalculator_CB", lbuf))
			not_act = true;
	}
	if (inam.IsEmpty()) inam = ChangeFileExt(Application->ExeName, ".INI");

	//二重起動が許可されているかをチェック
	try {
		std::unique_ptr<TStringList> ibuf(new TStringList());
		ibuf->LoadFromFile(inam, TEncoding::UTF8);
		duplicate = equal_1(ibuf->Values["MultiInstance"]);
	}
	catch (...) {
		;
	}

	//ミューテックスにより二重起動をチェック
	HANDLE hMutex = ::CreateMutex(NULL, FALSE, _T("NyanFi_is_running"));
	if (GetLastError()==ERROR_ALREADY_EXISTS) {
		//既存有り
		HWND hPrimeWnd = get_PrimNyanWnd();
		if (hPrimeWnd) {
			IsPrimary = false;
			int cmdlen = lstrlen(lpCmdLine);
			if ((cmdlen>0 || !duplicate) && !not_act) {
				HWND hApp = ::GetWindow(hPrimeWnd, GW_OWNER);
				if (::IsIconic(hApp)) ::SendMessage(hApp, WM_SYSCOMMAND, SC_RESTORE, 0);
				if (::IsIconic(hPrimeWnd)) ::SendMessage(hPrimeWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				::SetForegroundWindow(hPrimeWnd);
			}
			//コマンドラインの内容を送る
			if (cmdlen>0) {
				COPYDATASTRUCT cd;
				cd.dwData = 1;
				cd.cbData = sizeof(_TCHAR) * (cmdlen + 1);
				cd.lpData = lpCmdLine;
				::SendMessage(hPrimeWnd, WM_COPYDATA, 0, (LPARAM)&cd);
				duplicate = false;
				for (int i=0; i<opt_lst->Count && !restart; i++)
					restart = SameText(opt_lst->Strings[i], "-Q");	//-Q or -q
			}
		}

		if (!restart && !duplicate) {	//再起動で無い場合二重起動せずに終了
			::CloseHandle(hMutex);
			return -1;
		}
	}

	//再起動のために既存の終了を待つ(最大20秒)
	if (restart) {
		bool ok = false;
		for (int i=0; i<200 && !ok; i++) if (::WaitForSingleObject(hMutex, 100)==WAIT_OBJECT_0) ok = true;
		if (!ok) {	//既存が終了されないので終了
			::CloseHandle(hMutex);
			return -1;
		}
		IsPrimary = true;
	}

	::CoInitialize(NULL);

	try {
		Screen->Cursor = crHourGlass;
		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		//INIファイルの初期化
		IniFile = new UsrIniFile(inam);

		//スプラッシュ表示
		if (IniFile->ReadBoolGen(_T("ShowSplash"))) {
			SplashForm = new TSplashForm(Application);
			//バージョン
			unsigned mj, mi, bl;
			if (GetProductVersion(Application->ExeName, mj, mi, bl)) {
				SplashForm->VerLabel->Caption = UnicodeString().sprintf(_T("V%.2f (x64)"), (mj*100 + mi*10 + bl)/100.0);
				SplashForm->Show();
				SplashForm->SetMsgLabel(_T("初期化中..."));
			}
		}

		Application->Title = get_NewTitle(IsPrimary);
		Application->CreateForm(__classid(TUserModule), &UserModule);
		Application->CreateForm(__classid(TNyanFiForm), &NyanFiForm);
		Application->CreateForm(__classid(TBitmapForm), &BitmapForm);
		Application->CreateForm(__classid(TCharInfoForm), &CharInfoForm);
		Application->CreateForm(__classid(TCmdFileListDlg), &CmdFileListDlg);
		Application->CreateForm(__classid(TColorPicker), &ColorPicker);
		Application->CreateForm(__classid(TCsvRecForm), &CsvRecForm);
		Application->CreateForm(__classid(TEditHistoryDlg), &EditHistoryDlg);
		Application->CreateForm(__classid(TFileInfoDlg), &FileInfoDlg);
		Application->CreateForm(__classid(TFindTextDlg), &FindTextDlg);
		Application->CreateForm(__classid(TFuncListDlg), &FuncListDlg);
		Application->CreateForm(__classid(TGeneralInfoDlg), &GeneralInfoDlg);
		Application->CreateForm(__classid(TGifViewer), &GifViewer);
		Application->CreateForm(__classid(TGraphForm), &GraphForm);
		Application->CreateForm(__classid(THistForm), &HistForm);
		Application->CreateForm(__classid(TInpCmdsDlg), &InpCmdsDlg);
		Application->CreateForm(__classid(TInpDirDlg), &InpDirDlg);
		Application->CreateForm(__classid(TInputExDlg), &InputExDlg);
		Application->CreateForm(__classid(TInspectForm), &InspectForm);
		Application->CreateForm(__classid(TKeyListDlg), &KeyListDlg);
		Application->CreateForm(__classid(TLoupeForm), &LoupeForm);
		Application->CreateForm(__classid(TModalScrForm), &ModalScrForm);
		Application->CreateForm(__classid(TSameNameDlg), &SameNameDlg);
		Application->Run();
	}
	catch (Exception &exception) {
		Application->ShowException(&exception);
	}
	catch (...) {
		try {
			throw Exception("");
		}
		catch (Exception &exception) {
			Application->ShowException(&exception);
		}
	}

	::CoUninitialize();
	::CloseHandle(hMutex);

	//再起動用バッチファイルを起動
	if (!RstBatName.IsEmpty()) {
		if (!Execute_ex(RstBatName, EmptyStr, ExtractFilePath(Application->ExeName), "H"))
			msgbox_ERR("バッチファイルの起動に失敗しました。");
	}

	return 0;
}
//---------------------------------------------------------------------------
