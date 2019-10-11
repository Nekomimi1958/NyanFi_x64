//----------------------------------------------------------------------//
// NyanFi																//
//  ドライブ一覧														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <algorithm>
#include <winioctl.h>
#include "UserFunc.h"
#include "Global.h"
#include "InpExDlg.h"
#include "MainFrm.h"
#include "DrvGraph.h"
#include "DriveDlg.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSelDriveDlg *SelDriveDlg = NULL;

//---------------------------------------------------------------------------
__fastcall TSelDriveDlg::TSelDriveDlg(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormCreate(TObject *Sender)
{
	GridScrPanel = new UsrScrollPanel(GridPanel, DriveGrid,	USCRPNL_FLAG_P_WP | USCRPNL_FLAG_G_WP | USCRPNL_FLAG_HS);
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);
	OptPanel->Visible	= IniFile->ReadBoolGen(_T("DriveDlgShowOpt"),	true);
	BlankPanel->Visible = !OptPanel->Visible;
	SetDarkWinTheme(this);
	SetOptBtn();

	set_BtnMarkDark(HideOptBtn, UBMK_BDOWN);
	set_BtnMarkDark(ShowOptBtn, UBMK_BUP);
	HideOptBtn->Hint = LoadUsrMsg(USTR_HideOptPanel);
	ShowOptBtn->Hint = LoadUsrMsg(USTR_ShowOptPanel);

	TStringGrid *gp = DriveGrid;
	InitializeListGrid(gp);
	InitializeListHeader(DriveHeader, _T("キー|ボリューム|種類 (I/F)|使用容量|空き容量|全体容量|システム"));
	IniFile->LoadGridColWidth(gp, 7, 40,100,160,75,120,75,60);
	set_HeaderFromGrid(gp, DriveHeader);
	set_UsrScrPanel(GridScrPanel);

	OnlyAccCheckBox->Checked   = IniFile->ReadBoolGen(_T("DriveDlgOnlyAcc"));
	ShowIconCheckBox->Checked  = IniFile->ReadBoolGen(_T("DriveDlgShowIcon"));
	LargeIconCheckBox->Checked = IniFile->ReadBoolGen(_T("DriveDlgLargeIcon"));
	ToRootCheckBox->Checked    = IniFile->ReadBoolGen(_T("DriveDlgToRoot"));

	UpdateDriveList();

	UnicodeString dstr = ExtractFileDrive(CurPathName);
	if (dstr.Length()==2) {
		for (int i=0; i<gp->RowCount; i++) {
			if (SameText(gp->Cells[0][i], dstr[1])) {
				gp->Row = i; break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteBoolGen(_T("DriveDlgShowOpt"),	OptPanel->Visible);

	IniFile->SaveGridColWidth(DriveGrid);
	IniFile->WriteBoolGen(_T("DriveDlgOnlyAcc"),	OnlyAccCheckBox);
	IniFile->WriteBoolGen(_T("DriveDlgShowIcon"),	ShowIconCheckBox);
	IniFile->WriteBoolGen(_T("DriveDlgLargeIcon"),	LargeIconCheckBox);
	IniFile->WriteBoolGen(_T("DriveDlgToRoot"),		ToRootCheckBox);
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormDestroy(TObject *Sender)
{
	delete GridScrPanel;
}

//---------------------------------------------------------------------------
//ヘッダの描画
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveHeaderDrawSection(THeaderControl *HeaderControl,
	THeaderSection *Section, const TRect &Rect, bool Pressed)
{
	draw_SortHeader(HeaderControl, Section, Rect);
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveHeaderResize(TObject *Sender)
{
	set_HeaderFromGrid(DriveGrid, DriveHeader);
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormResize(TObject *Sender)
{
	SetOptBtn();
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveHeaderSectionResize(THeaderControl *HeaderControl, THeaderSection *Section)
{
	set_GridFromHeader(DriveHeader, DriveGrid);
	GridScrPanel->UpdateKnob();
	SetOptBtn();
}

//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::SetOptBtn()
{
	if (ScrBarStyle==0)
		set_ControlRBCorner(DriveGrid, BlankPanel);
	else
		GridScrPanel->SetRBCornerPanel(BlankPanel);
}

//---------------------------------------------------------------------------
UnicodeString __fastcall TSelDriveDlg::getCurDrvStr(const _TCHAR *suf)
{
	UnicodeString dstr = DriveGrid->Cells[0][DriveGrid->Row];
	if (suf) dstr.cat_sprintf(_T("%s"), suf);
	return dstr;
}

//---------------------------------------------------------------------------
//ドライブ一覧の更新
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::UpdateDriveList()
{
	cursor_HourGlass();

	TStringGrid *gp = DriveGrid;
	int icon_md = ShowIconCheckBox->Checked? (LargeIconCheckBox->Checked? 2 : 1) : 0;
	int icon_sz = ScaledInt((icon_md==2)? 32 : (icon_md==1)? 16 : 0);
	gp->DefaultRowHeight = get_FontHeight(gp->Font, ListInterLn) + std::max(8 - ListInterLn/2, 0);
	if (icon_md==2 && gp->DefaultRowHeight<(icon_sz + 4)) gp->DefaultRowHeight = icon_sz + 4;

	//表示ドライブを抽出
	update_DriveInfo();
	std::unique_ptr<TStringList> lst(new TStringList());
	for (int i=0; i<DriveInfoList->Count; i++) {
		drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
		if (OnlyAccCheckBox->Checked && !dp->accessible) continue;
		lst->AddObject(DriveInfoList->Strings[i], (TObject*)dp);
	}

	if (lst->Count>0) {
		gp->RowCount = lst->Count;

		UINT old_mode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
		for (int i=0; i<lst->Count; i++) {
			drive_info *dp = (drive_info *)lst->Objects[i];
			UnicodeString drv_str = dp->drive_str;
			UnicodeString typ_str = dp->is_virtual? UnicodeString("仮想ドライブ") : dp->type_str;
			if (!dp->is_virtual && !dp->bus_type.IsEmpty()) typ_str.cat_sprintf(_T(" (%s)"), dp->bus_type.c_str());

			int rn = i;
			gp->Cells[0][rn] = get_tkn(drv_str, ':');	//ドライブ文字
			gp->Cells[2][rn] = typ_str;					//種類
			gp->Cells[6][rn] = dp->f_system;			//ファイルシステム

			//容量
			bool ok = false;
			if (!drv_str.IsEmpty() && dp->accessible &&
				(gp->ColWidths[3]>=COL_WD_HIDE || gp->ColWidths[4]>=COL_WD_HIDE || gp->ColWidths[5]>=COL_WD_HIDE))
			{
				int dn = (char)drv_str[1] - 'A' + 1;
				__int64 sTotal = DiskSize(dn);
				__int64 sFree  = DiskFree(dn);
				__int64 sUsed  = sTotal - sFree;
				if (sTotal>0 && sFree>=0) {
					gp->Cells[3][rn] = get_size_str_T(sUsed,  SizeDecDigits).cat_sprintf(_T(" (%4.1f%%)"), 100.0*sUsed/sTotal);
					gp->Cells[4][rn] = get_size_str_T(sFree,  SizeDecDigits).cat_sprintf(_T(" (%4.1f%%)"), 100.0*sFree/sTotal);
					gp->Cells[5][rn] = get_size_str_T(sTotal, SizeDecDigits);
					ok = true;
				}
			}

			if (!ok) clear_GridRow(gp, rn, 3);
		}
		::SetErrorMode(old_mode);
	}
	else {
		gp->RowCount = 1;
		clear_GridRow(gp, 0);
	}

	GridScrPanel->UpdateKnob();

	cursor_Default();
	gp->SetFocus();
}

//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::OptCheckBoxClick(TObject *Sender)
{
	LargeIconCheckBox->Enabled = ShowIconCheckBox->Checked;
	UpdateDriveList();
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::ToRootCheckBoxClick(TObject *Sender)
{
	DriveGrid->SetFocus();
}

//---------------------------------------------------------------------------
//セルの描画
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveGridDrawCell(TObject *Sender, int ACol, int ARow,
		TRect &Rect, TGridDrawState State)
{
	TStringGrid *gp = (TStringGrid*)Sender;
	TRect rc = Rect;
	TCanvas *cv = gp->Canvas;
	cv->Font->Assign(gp->Font);
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = State.Contains(gdSelected)? col_selItem : col_bgList;
	cv->FillRect(rc);

	if (ACol==0 || gp->ColWidths[ACol]>=COL_WD_HIDE) {
		int xp = rc.Left + 4;
		int yp = rc.Top;
		if (ShowIconCheckBox->Checked && LargeIconCheckBox->Checked)
			yp += std::max(get_TopMargin2(cv), (rc.Height() - ListInterLn -get_FontHeight(cv->Font))/2);
		else
			yp += get_TopMargin2(cv);

		int c_wd = rc.Right - xp - 4;

		//カーソル位置のドライブ情報を取得
		drive_info *dp = NULL;
		int idx = 0;
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *di = (drive_info *)DriveInfoList->Objects[i];
			if (OnlyAccCheckBox->Checked && !di->accessible) continue;
			if (idx==ARow) dp = di;
			idx++;
		}

		cv->Font->Style = (ACol==0)? (cv->Font->Style << fsBold) : (cv->Font->Style >> fsBold);
		TColor fcol = gp->Cells[3][ARow].IsEmpty()? AdjustColor(col_fgList, ADJCOL_FGLIST) :
								is_SelFgCol(State)? col_fgSelItem : col_None;

		//ボリューム/パス
		if (ACol==1) {
			if (dp) {
				//アイコン
				if (ShowIconCheckBox->Checked) {
					int icon_sz = ScaledInt(ShowIconCheckBox->Checked? (LargeIconCheckBox->Checked? 32 : 16) : 0);
					TIcon *ip = LargeIconCheckBox->Checked? dp->large_ico : dp->small_ico;
					if (ip && ip->Handle) {
						::DrawIconEx(cv->Handle, rc.Left + 2, rc.Top + 2, ip->Handle, icon_sz, icon_sz, 0, NULL, DI_NORMAL);
						xp += icon_sz + 4;
						rc.Left = xp;
						c_wd = rc.Width() - 4;
					}
				}

				UnicodeString vnam = dp->volume;
				UnicodeString pnam = (dp->drv_type==DRIVE_REMOTE)? dp->unc : dp->mnt_dir;
				//ボリューム
				if (pnam.IsEmpty() || (get_TextWidth(cv, vnam + pnam.SubString(1, pnam.Length()/2), is_irreg) + 16 < c_wd)) {
					cv->Font->Color = (fcol==col_None)? col_fgList : fcol;
					cv->TextRect(rc, xp, yp, vnam);
					xp += get_TextWidth(cv, vnam, is_irreg) + 16;
					rc.Left = xp;
					c_wd = rc.Width() - 4;
				}

				//パス
				if (!pnam.IsEmpty()) {
					if (dp->is_virtual) {
						UnicodeString dnam = ExtractFileName(pnam);
						pnam = get_MiniPathName(pnam, c_wd, cv->Font, true);
						if (!ContainsText(pnam, dnam)) {
							pnam = "[" + minimize_str(dnam, cv, c_wd - cv->TextWidth("[]"), true) + "]";
						}
					}
					else if (dp->drv_type==DRIVE_REMOTE) {
						pnam = get_MiniPathName(pnam, c_wd, cv->Font, true);
					}
					cv->Font->Color = (fcol==col_None)? col_Folder : fcol;
					cv->TextRect(rc, xp, yp, pnam);
				}
			}
		}
		//種類
		else {
			UnicodeString cellstr = gp->Cells[ACol][ARow];
			if (ACol==2) {
				if (get_TextWidth(cv, cellstr, is_irreg)>c_wd) {
					remove_text(cellstr, _T("・メディア"));
					remove_text(cellstr, _T("・ドライブ"));
					remove_text(cellstr, _T("ドライブ"));
				}
				if (get_TextWidth(cv, cellstr, is_irreg)>c_wd) {
					cellstr = ReplaceStr(cellstr, "CD-ROM",				"CD");
					cellstr = ReplaceStr(cellstr, "ハードディスク",		"HDD");
					cellstr = ReplaceStr(cellstr, "ソリッドステート",	"SSD");
					cellstr = ReplaceStr(cellstr, "リムーバブル",		"RM");
					cellstr = ReplaceStr(cellstr, "ネットワーク",		"NET");
					remove_text(cellstr, _T("ディスク"));
				}
				if (get_TextWidth(cv, cellstr, is_irreg)>c_wd) cellstr = get_tkn(cellstr, _T(" ("));
				//接続I/F を右寄せ
				int p = cellstr.Pos(" (");
				if (p>0) while (get_TextWidth(cv, cellstr + " ", is_irreg) < c_wd) cellstr.Insert(" ", p);
			}
			//使用容量
			else if (ACol==3) {
				int w = 0;
				for (int i=1; i<gp->RowCount; i++) w = std::max(w, get_TextWidth(cv, gp->Cells[3][i], is_irreg));
				if (w>c_wd) cellstr = get_tkn(cellstr, _T(" ("));
			}
			//空き容量
			else if (ACol==4) {
				int w = 0;
				for (int i=1; i<gp->RowCount; i++) w = std::max(w, get_TextWidth(cv, gp->Cells[4][i], is_irreg));
				if (w>c_wd) cellstr = get_tkn(cellstr, _T(" ("));
			}

			int s_wd = get_TextWidth(cv, cellstr, is_irreg);

			switch (ACol) {
			case 0:	//ドライブ(キー)
				if (s_wd<c_wd) xp += (c_wd - s_wd)/2;	//センタリング
				break;
			case 3: case 4: case 5:	//容量
				if (s_wd<c_wd) xp += (c_wd - s_wd);	//右寄せ
				break;
			}

			cv->Font->Color = (fcol==col_None)? col_fgList : fcol;
			cv->TextRect(rc, xp, yp, cellstr);

			//使用率グラフ
			if (ACol==5) {
				UnicodeString s = get_tkn_m(gp->Cells[4][ARow], '(', '%');
				if (!s.IsEmpty()) {
					try {
						float r = 1.0 - StrToFloat(s)/100.0;
						InflateRect(rc, -4, 0);
						rc.Top	  = yp + cv->TextHeight("Q");
						rc.Bottom = rc.Top + 4;
						draw_BarGraph(cv, rc, r);
					}
					catch (...) {
						;
					}
				}
			}
		}
	}

	//カーソル
	draw_GridCursor(gp, Rect, ARow, State);
}

//---------------------------------------------------------------------------
//キー操作
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);	if (KeyStr.IsEmpty()) return;

	UnicodeString dstr;
	if (equal_ENTER(KeyStr)) {
		dstr = getCurDrvStr();
	}
	else if (KeyStr.Length()==1) {
		for (int i=0; i<DriveGrid->RowCount; i++) {
			if (StartsText(KeyStr, DriveGrid->Cells[0][i])) {
				dstr = DriveGrid->Cells[0][i];
				break;
			}
		}
	}

	bool handled = true;
	if (!dstr.IsEmpty()) {
		if (ToRootCheckBox->Checked) dstr += ":"; else dstr += ":\\";
		if (is_drive_accessible(dstr)) {
			NyanFiForm->RecoverFileList2();
			NyanFiForm->UpdateCurDrive(dstr);
			Close();
		}
		else beep_Warn();
	}
	else {
		UnicodeString cmd_F = Key_to_CmdF(KeyStr);
		//コンテキストメニュー
		if (StartsText("ContextMenu", cmd_F)) {
			TStringGrid *gp = (TStringGrid*)Sender;
			Mouse->CursorPos = gp->ClientToScreen(Point(gp->ColWidths[0] + gp->ColWidths[1], 16));
			ClearKeyBuff(true);
			ShowDriveMenu();
		}
		//容量グラフ
		else if (StartsText("DriveGraph", cmd_F)) {
			ShowDriveGraph();
		}
		else handled = false;
	}

	if (!is_DialogKey(Key) || handled) Key = 0;
}

//---------------------------------------------------------------------------
//マウス操作
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveGridMouseUp(TObject *Sender, TMouseButton Button,
		TShiftState Shift, int X, int Y)
{
	//ドライブのコンテキストメニューを開く
	if (Button==mbRight) ShowDriveMenu();
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DriveGridDblClick(TObject *Sender)
{
	perform_Key_RETURN((TControl*)Sender);
}

//---------------------------------------------------------------------------
//ドライブ使用率推移を表示
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::ShowDriveGraph()
{
	if (!DriveGraph) DriveGraph = new TDriveGraph(this);	//初回に動的作成
	DriveGraph->DriveName = getCurDrvStr();
	DriveGraph->ShowModal();
}

//---------------------------------------------------------------------------
//ドライブのコンテキストメニューを表示
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::ShowDriveMenu()
{
	UnicodeString drvnam = getCurDrvStr(_T(":\\"));
	UnicodeString typstr = DriveGrid->Cells[2][DriveGrid->Row];

	std::unique_ptr<TStringList> ex_item(new TStringList());
	ex_item->Text =
		"-\n"
		"ネットワークドライブの割り当て(&N)...	NetConnect\n"
		"ネットワークドライブの切断(&D)...	NetDisConnect\n"
		"-\n"
		"ボリューム名の変更	Rename\n"
		"最新の情報に更新	ReloadList\n"
		"ドライブ使用率推移	DriveGraph\n";
	if (ContainsText(typstr, "CD-ROM")) {
		ex_item->Add("-");
		ex_item->Add("ドライブのトレイを開く(&T)	EjectTray");
	}

	UnicodeString res_str = usr_SH->DriveContextMenu(Handle, drvnam, ex_item.get());
	TStringDynArray m_buf = split_strings_tab(res_str);
	if (m_buf.Length>1) {
		if		(USAME_TI(m_buf[1], "NetConnect"))		NyanFiForm->NetConnectAction->Execute();
		else if (USAME_TI(m_buf[1], "NetDisConnect"))	NyanFiForm->NetDisconnectAction->Execute();
		else if (USAME_TI(m_buf[1], "EjectTray"))		EjectTrayAction->Execute();
		else if (USAME_TI(m_buf[1], "ReloadList"))		UpdateDriveList();
		else if (USAME_TI(m_buf[1], "DriveGraph"))		ShowDriveGraph();
		else if (USAME_TI(m_buf[1], "Rename")) {
			UnicodeString new_name = get_VolumeInfo(drvnam);
			if (input_query_ex(USTR_Rename, _T("名前"), &new_name)) {
				if (::SetVolumeLabel(drvnam.c_str(), new_name.c_str())) UpdateDriveList();
			}
		}
	}
}

//---------------------------------------------------------------------------
//ドライブのプロパティ
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::DrivePropBtnClick(TObject *Sender)
{
	ShowPropertyDialog(getCurDrvStr(_T(":\\")));
	DriveGrid->SetFocus();
}

//---------------------------------------------------------------------------
//ドライブの取り出し
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::EjectDriveActionExecute(TObject *Sender)
{
	NyanFiForm->ExeEjectDrive(getCurDrvStr());
	UpdateDriveList();
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::EjectDriveActionUpdate(TObject *Sender)
{
	drive_info *dp = (DriveGrid->Row!=1)? get_DriveInfo(getCurDrvStr()) : NULL;
	((TAction*)Sender)->Enabled = (dp && (dp->ejectable || dp->is_virtual));
}

//---------------------------------------------------------------------------
//ドライブのトレイを開く
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::EjectTrayActionExecute(TObject *Sender)
{
	MCI_OPEN_PARMS mci_prm;
	mci_prm.lpstrDeviceType = (LPCTSTR)MCI_DEVTYPE_CD_AUDIO;
	DWORD flag = MCI_OPEN_TYPE|MCI_OPEN_TYPE_ID|MCI_OPEN_SHAREABLE|MCI_WAIT|MCI_OPEN_ELEMENT;

	mci_prm.lpstrElementName = getCurDrvStr(_T(":")).c_str();

	if (::mciSendCommand(0, MCI_OPEN, flag, (DWORD)&mci_prm) == 0) {
		::mciSendCommand(mci_prm.wDeviceID, MCI_SET, MCI_SET_DOOR_OPEN, NULL);
		::mciSendCommand(mci_prm.wDeviceID, MCI_CLOSE, MCI_WAIT, NULL);
		UpdateDriveList();
	}
}
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::EjectTrayActionUpdate(TObject *Sender)
{
	drive_info *dp = (DriveGrid->Row!=1)? get_DriveInfo(getCurDrvStr()) : NULL;
	((TAction*)Sender)->Enabled = (dp && dp->drv_type==DRIVE_CDROM);
}
//---------------------------------------------------------------------------
//エクスプローラで開く
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::OpenExpItemClick(TObject *Sender)
{
	Execute_ex(getCurDrvStr(_T(":\\")));
}
//---------------------------------------------------------------------------
//プロパティ
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::PropertyActionExecute(TObject *Sender)
{
	ShowPropertyDialog(getCurDrvStr(_T(":\\")));
	DriveGrid->SetFocus();
}

//---------------------------------------------------------------------------
//設定パネルの開閉
//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::ChgOptBtnClick(TObject *Sender)
{
	OptPanel->Visible	= !OptPanel->Visible;
	BlankPanel->Visible = !OptPanel->Visible;
	SetOptBtn();
}

//---------------------------------------------------------------------------
void __fastcall TSelDriveDlg::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (USAME_TI(get_KeyStr(Key, Shift), "Alt+O"))
		ChgOptBtnClick(NULL);
	else
		SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

