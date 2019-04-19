//----------------------------------------------------------------------//
// 2画面ファイラー NyanFi												//
//  メインフォーム														//
//----------------------------------------------------------------------//
#ifndef MainFrmH
#define MainFrmH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.AppEvnts.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.FileCtrl.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.ImgList.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <IdAntiFreezeBase.hpp>
#include <IdIOHandler.hpp>
#include <IdIOHandlerSocket.hpp>
#include <IdIOHandlerStack.hpp>
#include <IdSSL.hpp>
#include <IdSSLOpenSSL.hpp>
#include <IdSSLOpenSSLHeaders.hpp>
#include <Vcl.IdAntiFreeze.hpp>
#include "usr_shell.h"
#include "usr_scrpanel.h"
#include "usr_spbar.h"
#include "Global.h"
#include "task_thread.h"
#include "usr_hintwin.h"

//---------------------------------------------------------------------------
#define EXTMENU_BASE	1000
#define EXTTOOL_BASE	2000

#define REGDIR_BASE		1000
#define OPPDIR_BASE		1500
#define PATMSK_BASE		2000
#define FL_TAB_BASE		2500
#define TAGNAM_BASE		3000
#define TRYNYN_BASE		3500
#define OTHERS_BASE		5000

//---------------------------------------------------------------------------
#define IS_FullScr()			(BorderStyle==bsNone)
#define TEST_ActParam(s)		TestActionParam(_T(s))
#define TEST_DEL_ActParam(s)	TestDelActionParam(_T(s))

//---------------------------------------------------------------------------
class TNyanFiForm : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AboutNyanFiAction;
	TAction *AddTabAction;
	TAction *AddTagAction;
	TAction *AlphaBlendAction;
	TAction *AppListAction;
	TAction *BackDirHistAction;
	TAction *BackupAction;
	TAction *BgImgModeAction;
	TAction *BinaryEditAction;
	TAction *BitmapViewAction;
	TAction *BorderLeftAction;
	TAction *BorderRightAction;
	TAction *CalcDirSizeAction;
	TAction *CalcDirSizeAllAction;
	TAction *CalculatorAction;
	TAction *CancelAllTaskAction;
	TAction *ChangeDirAction;
	TAction *ChangeDriveAction;
	TAction *ChangeOppDirAction;
	TAction *ChangeOppRegDirAction;
	TAction *ChangeRegDirAction;
	TAction *CharInfoAction;
	TAction *CheckUpdateAction;
	TAction *ChgCodePageAction;
	TAction *ClearAllAction;
	TAction *ClearLogAction;
	TAction *ClearMarkAction;
	TAction *ClearMaskAction;
	TAction *ClipCopyAction;
	TAction *CloneAction;
	TAction *CloneToCurrAction;
	TAction *CloseIAction;
	TAction *CmdFileListAction;
	TAction *ColorPickerAction;
	TAction *CommandPromptAction;
	TAction *CompareDlgAction;
	TAction *CompareHashAction;
	TAction *CompleteDeleteAction;
	TAction *CompressDirAction;
	TAction *ContextMenuAction;
	TAction *ConvertDoc2TxtAction;
	TAction *ConvertHtm2TxtAction;
	TAction *ConvertImageAction;
	TAction *ConvertTextEncAction;
	TAction *CopyAction;
	TAction *CopyCmdNameAction;
	TAction *CopyDirAction;
	TAction *CopyFileInfoAction;
	TAction *CopyFileNameAction;
	TAction *CopyPreImgAction;
	TAction *CopyPreImgOrgAction;
	TAction *CopyToAction;
	TAction *CopyToClipAction;
	TAction *CountLinesAction;
	TAction *CP_AutoAction;
	TAction *CP_EucAction;
	TAction *CP_JisAction;
	TAction *CP_Latin1Action;
	TAction *CP_ShiftJisAction;
	TAction *CP_UTF8Action;
	TAction *CP_UTF16Action;
	TAction *CreateDirAction;
	TAction *CreateDirsDlgAction;
	TAction *CreateHardLinkAction;
	TAction *CreateJunctionAction;
	TAction *CreateShortcutAction;
	TAction *CreateSymLinkAction;
	TAction *CreateTestFileAction;
	TAction *CsrDirToOppAction;
	TAction *CsvCalcAction;
	TAction *CsvGraphAction;
	TAction *CsvRecordAction;
	TAction *CurrFromOppAction;
	TAction *CurrToOppAction;
	TAction *CursorDownAction;
	TAction *CursorDownSelAction;
	TAction *CursorEndAction;
	TAction *CursorEndSelAction;
	TAction *CursorTopAction;
	TAction *CursorTopSelAction;
	TAction *CursorUpAction;
	TAction *CursorUpSelAction;
	TAction *CutToClipAction;
	TAction *DateSelectAction;
	TAction *DebugCmdFileAction;
	TAction *DefHighlightAction;
	TAction *DeleteAction;
	TAction *DeleteADSAction;
	TAction *DeleteIAction;
	TAction *DeleteVAction;
	TAction *DelJpgExifAction;
	TAction *DelSelMaskAction;
	TAction *DelTabAction;
	TAction *DelTagAction;
	TAction *DiffDirAction;
	TAction *DirHistoryAction;
	TAction *DirStackAction;
	TAction *DistributionDlgAction;
	TAction *DotNyanDlgAction;
	TAction *DoublePageAction;
	TAction *DriveGraphAction;
	TAction *DriveListAction;
	TAction *DuplicateAction;
	TAction *EditHighlightAction;
	TAction *EditHistoryAction;
	TAction *EditIniFileAction;
	TAction *EjectAction;
	TAction *EjectDriveAction;
	TAction *EmptyTrashAction;
	TAction *EndFileAction;
	TAction *EqualListWidthAction;
	TAction *EqualSizeAction;
	TAction *ExeCommandLineAction;
	TAction *ExeCommandsAction;
	TAction *ExeExtMenuAction;
	TAction *ExeExtToolAction;
	TAction *ExeMenuFileAction;
	TAction *ExeToolBtnAction;
	TAction *ExitAction;
	TAction *ExitDuplAction;
	TAction *ExPopupMenuAction;
	TAction *ExportCsvAction;
	TAction *ExtractChmSrcAction;
	TAction *ExtractGifBmpAction;
	TAction *ExtractIconAction;
	TAction *ExtractMp3ImgAction;
	TAction *FileEditAction;
	TAction *FileExtListAction;
	TAction *FileListOnlyAction;
	TAction *FileRunAction;
	TAction *FilterAction;
	TAction *FindDirDlgAction;
	TAction *FindDuplDlgAction;
	TAction *FindFileDirDlgAction;
	TAction *FindFileDlgAction;
	TAction *FindFolderIconAction;
	TAction *FindHardLinkAction;
	TAction *FindMarkAction;
	TAction *FindTagAction;
	TAction *FindTagNameAction;
	TAction *FindTextAction;
	TAction *FittedSizeAction;
	TAction *FixedLenAction;
	TAction *FixTabPathAction;
	TAction *FK_RenameAction;
	TAction *FlipHorzAction;
	TAction *FlipVertAction;
	TAction *ForwardDirHistAction;
	TAction *FTPChmodAction;
	TAction *FTPConnectAction;
	TAction *FTPDisconnectAction;
	TAction *FullScreenAction;
	TAction *FunctionListAction;
	TAction *GetHashAction;
	TAction *GitDiffAction;
	TAction *GitViewerAction;
	TAction *GrayScaleAction;
	TAction *GrepAbortAction;
	TAction *GrepAction;
	TAction *GrepClipCopyAction;
	TAction *GrepConfirmAction;
	TAction *GrepEmFilterAction;
	TAction *GrepExceptAction;
	TAction *GrepExtractAction;
	TAction *GrepFileItemNoAction;
	TAction *GrepLineCopyAction;
	TAction *GrepOmitTopAction;
	TAction *GrepOpenUrlAction;
	TAction *GrepOptionAction;
	TAction *GrepReleaseAction;
	TAction *GrepSaveAsAction;
	TAction *GrepSelResAction;
	TAction *GrepShowItemNoAction;
	TAction *GrepShowSubDirAction;
	TAction *GrepStartAction;
	TAction *GrepTrimTopAction;
	TAction *HelpContentsAction;
	TAction *HideSizeTimeAction;
	TAction *HighlightAction;
	TAction *HistogramAction;
	TAction *HomeWorkListAction;
	TAction *HtmlToTextAction;
	TAction *ImageViewerAction;
	TAction *IncSearchAction;
	TAction *Inf_CopyValueAction;
	TAction *Inf_EditCopy;
	TAction *Inf_EditSelectAll;
	TAction *Inf_EmpItemAction;
	TAction *Inf_HideItemAction;
	TAction *Inf_OpenUrlAction;
	TAction *InputCommandsAction;
	TAction *InputDirAction;
	TAction *InputPathMaskAction;
	TAction *InspectorAction;
	TAction *InsSeparatorAction;
	TAction *ItemTmpDownAction;
	TAction *ItemTmpMoveAction;
	TAction *ItemTmpUpAction;
	TAction *IV_SidebarPosAction;
	TAction *IV_ThumbPosAction;
	TAction *JoinTextAction;
	TAction *JumpIndexAction;
	TAction *JumpToAction;
	TAction *KeyListAction;
	TAction *LibraryAction;
	TAction *LinkToOppAction;
	TAction *ListArchiveAction;
	TAction *ListClipboardAction;
	TAction *ListDurationAction;
	TAction *ListExpFuncAction;
	TAction *ListFileNameAction;
	TAction *ListLogAction;
	TAction *ListNyanFiAction;
	TAction *ListTailAction;
	TAction *ListTextAction;
	TAction *ListTreeAction;
	TAction *LoadBgImageAction;
	TAction *LoadResultListAction;
	TAction *LoadTabGroupAction;
	TAction *LoadWorkListAction;
	TAction *LockTextPreviewAction;
	TAction *Log_DebugInfAction;
	TAction *LogFileInfoAction;
	TAction *LoupeAction;
	TAction *MarkAction;
	TAction *MarkListAction;
	TAction *MarkListVAction;
	TAction *MarkMaskAction;
	TAction *MaskFindAction;
	TAction *MaskSelectAction;
	TAction *MatchSelectAction;
	TAction *MenuBarAction;
	TAction *MonitorOffAction;
	TAction *MoveAction;
	TAction *MoveTabAction;
	TAction *MoveToAction;
	TAction *MuteVolumeAction;
	TAction *NameFromClipAction;
	TAction *NameToLowerAction;
	TAction *NameToUpperAction;
	TAction *NetConnectAction;
	TAction *NetDisconnectAction;
	TAction *NewFileAction;
	TAction *NewTextFileAction;
	TAction *NewWorkListAction;
	TAction *NextDriveAction;
	TAction *NextFileAction;
	TAction *NextMarkAction;
	TAction *NextNyanFiAction;
	TAction *NextPageAction;
	TAction *NextSameNameAction;
	TAction *NextTabAction;
	TAction *OpenADSAction;
	TAction *OpenByAppAction;
	TAction *OpenByExpAction;
	TAction *OpenByWinAction;
	TAction *OpenGitURLAction;
	TAction *OpenStandardAction;
	TAction *OpenTrashAction;
	TAction *OptionDlgAction;
	TAction *PackAction;
	TAction *PackToCurrAction;
	TAction *PageBindAction;
	TAction *PageDownAction;
	TAction *PageDownIAction;
	TAction *PageDownSelAction;
	TAction *PageUpAction;
	TAction *PageUpIAction;
	TAction *PageUpSelAction;
	TAction *PasteAction;
	TAction *PathMaskDlgAction;
	TAction *PauseAllTaskAction;
	TAction *PlayListAction;
	TAction *PopDirAction;
	TAction *PopResultAction;
	TAction *PopupMainMenuAction;
	TAction *PopupTabAction;
	TAction *PowerOffAction;
	TAction *PowerShellAction;
	TAction *PrevDriveAction;
	TAction *PrevFileAction;
	TAction *PrevMarkAction;
	TAction *PrevNyanFiAction;
	TAction *PrevPageAction;
	TAction *PrevTabAction;
	TAction *PrintAction;
	TAction *PropertyDlgAction;
	TAction *PushDirAction;
	TAction *PushResultAction;
	TAction *RebootAction;
	TAction *RecentListAction;
	TAction *RegDirDlgAction;
	TAction *RegDirPopupAction;
	TAction *RegExCheckerAction;
	TAction *RegSyncDlgAction;
	TAction *ReloadListAction;
	TAction *RenameDlgAction;
	TAction *ReplaceStartAction;
	TAction *RepositoryListAction;
	TAction *RestartAction;
	TAction *ReturnListAction;
	TAction *RotateLeftAction;
	TAction *RotateRightAction;
	TAction *SaveAsResultListAction;
	TAction *SaveAsTabGroupAction;
	TAction *SaveAsWorkListAction;
	TAction *SaveDumpAction;
	TAction *SaveTabGroupAction;
	TAction *SaveWorkListAction;
	TAction *ScrollDownAction;
	TAction *ScrollDownIAction;
	TAction *ScrollDownLogAction;
	TAction *ScrollDownTextAction;
	TAction *ScrollLeftAction;
	TAction *ScrollRightAction;
	TAction *ScrollUpAction;
	TAction *ScrollUpIAction;
	TAction *ScrollUpLogAction;
	TAction *ScrollUpTextAction;
	TAction *SelAllFileAction;
	TAction *SelAllItemAction;
	TAction *SelByListAction;
	TAction *SelectAction;
	TAction *SelectUpAction;
	TAction *SelEmptyDirAction;
	TAction *SelGitChangedAction;
	TAction *SelMarkAction;
	TAction *SelMaskAction;
	TAction *SelOnlyCurAction;
	TAction *SelReverseAction;
	TAction *SelReverseAllAction;
	TAction *SelSameExtAction;
	TAction *SelSameNameAction;
	TAction *SelWorkItemAction;
	TAction *SendToWorkListAction;
	TAction *SetAliasAction;
	TAction *SetArcTimeAction;
	TAction *SetColorAction;
	TAction *SetDirTimeAction;
	TAction *SetExifTimeAction;
	TAction *SetFolderIconAction;
	TAction *SetFontSizeAction;
	TAction *SetMarginAction;
	TAction *SetPathMaskAction;
	TAction *SetSttBarFmtAction;
	TAction *SetSubSizeAction;
	TAction *SetTabAction;
	TAction *SetTagAction;
	TAction *SetTopAddrAction;
	TAction *SetWidthAction;
	TAction *ShareListAction;
	TAction *ShowByteSizeAction;
	TAction *ShowCRAction;
	TAction *ShowFileInfoAction;
	TAction *ShowFKeyBarAction;
	TAction *ShowGridAction;
	TAction *ShowHideAtrAction;
	TAction *ShowIconAction;
	TAction *ShowLineNoAction;
	TAction *ShowLogWinAction;
	TAction *ShowOpenAsRunAction;
	TAction *ShowPreviewAction;
	TAction *ShowPropertyAction;
	TAction *ShowRubyAction;
	TAction *ShowRulerAction;
	TAction *ShowSeekBarAction;
	TAction *ShowStatusBarAction;
	TAction *ShowSystemAtrAction;
	TAction *ShowTABAction;
	TAction *ShowTabBarAction;
	TAction *ShowToolBarAction;
	TAction *SidebarAction;
	TAction *SimilarSortAction;
	TAction *SoloTabAction;
	TAction *SortDlgAction;
	TAction *SortDlgIAction;
	TAction *SpecialDirListAction;
	TAction *SubDirListAction;
	TAction *SubViewerAction;
	TAction *SuspendAction;
	TAction *SwapLRAction;
	TAction *SwapNameAction;
	TAction *SyncLRAction;
	TAction *TabDlgAction;
	TAction *TabHomeAction;
	TAction *TagSelectAction;
	TAction *TaskManAction;
	TAction *TestArchiveAction;
	TAction *TextViewerAction;
	TAction *ThumbnailAction;
	TAction *ThumbnailExAction;
	TAction *ToExViewerAction;
	TAction *ToLeftAction;
	TAction *ToLogAction;
	TAction *ToNextOnRightAction;
	TAction *ToolBarDlgAction;
	TAction *ToOppositeAction;
	TAction *ToOppSameHashAction;
	TAction *ToOppSameItemAction;
	TAction *ToParentAction;
	TAction *ToParentOnLeftAction;
	TAction *ToParentOnRightAction;
	TAction *TopFileAction;
	TAction *ToPrevOnLeftAction;
	TAction *ToRightAction;
	TAction *ToRootAction;
	TAction *ToTabAction;
	TAction *ToTextAction;
	TAction *TrimTagDataAction;
	TAction *TV_EditAddAction;
	TAction *TV_OrgOrderAction;
	TAction *TV_SortAscAction;
	TAction *TV_SortDesAction;
	TAction *TV_TopIsHdrAction;
	TAction *Txt_EditCopy;
	TAction *Txt_EditSelectAll;
	TAction *Txt_KeepIndexAction;
	TAction *Txt_OpenUrlAction;
	TAction *Txt_ShowLnNoAction;
	TAction *Txt_ShowTailAction;
	TAction *UndoRenameAction;
	TAction *UnPackAction;
	TAction *UnPackToCurrAction;
	TAction *UpdateFromArcAction;
	TAction *UserDefListAction;
	TAction *UseTrashAction;
	TAction *ViewHistoryAction;
	TAction *ViewIniFileAction;
	TAction *ViewLogAction;
	TAction *ViewTailAction;
	TAction *WarnHighlightAction;
	TAction *WatchTailAction;
	TAction *WebMapAction;
	TAction *WebSearchAction;
	TAction *WidenCurListAction;
	TAction *WinMaximizeAction;
	TAction *WinMinimizeAction;
	TAction *WinNormalAction;
	TAction *WinPosAction;
	TAction *WorkItemDownAction;
	TAction *WorkItemMoveAction;
	TAction *WorkItemUpAction;
	TAction *WorkListAction;
	TAction *WorkListIAction;
	TAction *XmlViewerAction;
	TAction *ZoomInAction;
	TAction *ZoomInIAction;
	TAction *ZoomOutAction;
	TAction *ZoomOutIAction;
	TAction *ZoomResetAction;
	TActionList *ActionList1;
	TApplicationEvents *ApplicationEvents1;
	TButton *AbortBtn;
	TButton *AbortRBtn;
	TButton *CanDlBtn;
	TButton *GrepCanBtn;
	TButton *GrepExOptBtn;
	TButton *StartBtn;
	TButton *StartRBtn;
	TCheckBox *AndCheckBox;
	TCheckBox *AndOrCheckBox;
	TCheckBox *AskRepCheckBox;
	TCheckBox *CaseCheckBox;
	TCheckBox *CaseRCheckBox;
	TCheckBox *ExclTagCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *NextLineCheckBox;
	TCheckBox *OneMatchCheckBox;
	TCheckBox *RegExCheckBox;
	TCheckBox *RegExRCheckBox;
	TCheckBox *SubDirCheckBox;
	TComboBox *FilterComboBox;
	TComboBox *GrepFindComboBox;
	TComboBox *GrepMaskComboBox;
	TComboBox *InpDirComboBox;
	TComboBox *RepFindComboBox;
	TComboBox *RepStrComboBox;
	TComboBox *SubDirNCombo;
	TEditCopy *Log_EditCopy;
	TEditCopy *TV_EditCopyAction;
	TEditSelectAll *Log_EditSelectAll;
	TEditSelectAll *TV_EditSelAllAction;
	THeaderControl *L_HeaderControl;
	THeaderControl *R_HeaderControl;
	TImage *HotPosImage;
	TImage *PreviewImage;
	TImage *ViewerImage;
	TImageList *IconImgList;
	TImageList *IconImgListI;
	TImageList *IconImgListP;
	TImageList *IconImgListV;
	TLabel *GrepFindLabel;
	TLabel *PreviewSizeLabel;
	TLabel *PreviewSttLabel;
	TLabel *ProgressLabel;
	TLabel *ProgressSubLabel;
	TLabel *RepFindLabel;
	TLabel *RepStrLabel;
	TLabeledEdit *GrepFilterEdit;
	TLabeledEdit *SkipDirEdit;
	TListBox *ImgInfListBox;
	TListBox *InfListBox;
	TListBox *L_ListBox;
	TListBox *LogListBox;
	TListBox *R_ListBox;
	TListBox *ResultListBox;
	TListBox *TxtPrvListBox;
	TListBox *TxtTailListBox;
	TMainMenu *MainMenu1;
	TMenuItem *AboutItem;
	TMenuItem *AddTagItem;
	TMenuItem *AppListItem;
	TMenuItem *BackupItem;
	TMenuItem *BitmapViewItem;
	TMenuItem *CalcItem;
	TMenuItem *ChangeCodePageItem;
	TMenuItem *CharInfoItem;
	TMenuItem *ChgCodePageItem;
	TMenuItem *CmdFileListItem;
	TMenuItem *CmdIndexItem;
	TMenuItem *ColPickerItem;
	TMenuItem *ComExeItem;
	TMenuItem *CompDlgItem;
	TMenuItem *ConvertImageItem;
	TMenuItem *CopyAddItem;
	TMenuItem *CopyFielNameItem;
	TMenuItem *CopyPreImgItem;
	TMenuItem *CopyPrviewImgItem;
	TMenuItem *CP_AutoItem;
	TMenuItem *CP_EUCItem;
	TMenuItem *CP_JisItem;
	TMenuItem *CP_Latin1Item;
	TMenuItem *CP_ShiftJISItem;
	TMenuItem *CP_UTF8Item;
	TMenuItem *CP_UTF16Item;
	TMenuItem *CpAutoItem;
	TMenuItem *CpEucItem;
	TMenuItem *CpJisItem;
	TMenuItem *CpLatin1Item;
	TMenuItem *CpShiftJISItem;
	TMenuItem *CpUtf8Item;
	TMenuItem *CpUtf16Item;
	TMenuItem *CreDirsItem;
	TMenuItem *CsvRecordItem;
	TMenuItem *DefHighlightItem;
	TMenuItem *DelJpgExifItem;
	TMenuItem *DiffDirItem;
	TMenuItem *DirHistItem;
	TMenuItem *DirStackItem;
	TMenuItem *DistributionItem;
	TMenuItem *DotNyanDlgItem;
	TMenuItem *DoublePageItem;
	TMenuItem *DriveGraphItem;
	TMenuItem *DropCopyItem;
	TMenuItem *DropLinkItem;
	TMenuItem *DropMoveItem;
	TMenuItem *DuplicateItem;
	TMenuItem *EditFontSmplItem;
	TMenuItem *EditHighlightItem;
	TMenuItem *EditHistoryItem;
	TMenuItem *EditIniFileItem;
	TMenuItem *EditMenu;
	TMenuItem *EditSwatchItem;
	TMenuItem *EjectItem;
	TMenuItem *EqualSizeItem;
	TMenuItem *ExecuteItem;
	TMenuItem *ExitItem;
	TMenuItem *ExportCsvItem;
	TMenuItem *ExtractIconItem;
	TMenuItem *ExtractMp3ImgItem;
	TMenuItem *FileExtListItem;
	TMenuItem *FileListOnlyItem;
	TMenuItem *FileMenu;
	TMenuItem *FileRunItem;
	TMenuItem *FindDirItem;
	TMenuItem *FindFileItem;
	TMenuItem *FindFolderIconItem;
	TMenuItem *FindMenu;
	TMenuItem *FindTagItem;
	TMenuItem *FisTabPathItem;
	TMenuItem *FittedSizeItem;
	TMenuItem *FixedLenItem;
	TMenuItem *FtpChmodItem;
	TMenuItem *FtpConnectItem;
	TMenuItem *FtpDisconItem;
	TMenuItem *FullScreenItem;
	TMenuItem *FunctionListItem;
	TMenuItem *GrayScaleItem;
	TMenuItem *GrepClipCopyItem;
	TMenuItem *GrepConfirmItem;
	TMenuItem *GrepEmFilterItem;
	TMenuItem *GrepExceptItem;
	TMenuItem *GrepExtractItem;
	TMenuItem *GrepFileItemNoItem;
	TMenuItem *GrepItem;
	TMenuItem *GrepOmitTopItem;
	TMenuItem *GrepOptionItem;
	TMenuItem *GrepReleaseItem;
	TMenuItem *GrepSaveAsItem;
	TMenuItem *GrepSelResItem;
	TMenuItem *GrepShowItemNoItem;
	TMenuItem *GrepShowSubDirItem;
	TMenuItem *GrepTrimTopItem;
	TMenuItem *HelpContentsItem;
	TMenuItem *HelpHistoryItem;
	TMenuItem *HelpMenu;
	TMenuItem *HideSizeTimeItem;
	TMenuItem *HistogramItem;
	TMenuItem *HomeWorkListItem;
	TMenuItem *Htm2TxtItem;
	TMenuItem *HtmlToTextItem;
	TMenuItem *InfEditCopyItem;
	TMenuItem *InfEditSelectAllItem;
	TMenuItem *InfEmpItemItem;
	TMenuItem *InfHideItemItem;
	TMenuItem *InfOpenUrlItem;
	TMenuItem *InfShowItemItem;
	TMenuItem *InspectorItem;
	TMenuItem *InsSeparatorItem;
	TMenuItem *InvSelItem;
	TMenuItem *IV_ClipCopyItem;
	TMenuItem *JoinTextItem;
	TMenuItem *K1;
	TMenuItem *KeyListItem;
	TMenuItem *LibraryItem;
	TMenuItem *ListMenu;
	TMenuItem *LoadResultItem;
	TMenuItem *LoadTabGroupItem;
	TMenuItem *LoadWorkFileItem;
	TMenuItem *LockTxPreviewItem;
	TMenuItem *LoupeItem;
	TMenuItem *MakeShortcutItem;
	TMenuItem *MarkListItem;
	TMenuItem *MarkListVItem;
	TMenuItem *MaskSelItem;
	TMenuItem *NetConDlgItem;
	TMenuItem *NetDisDlgItem;
	TMenuItem *NewFileItem;
	TMenuItem *NewTextItem;
	TMenuItem *NewWorkListItem;
	TMenuItem *OK1;
	TMenuItem *OpenAsRunItem;
	TMenuItem *OpenExpItem;
	TMenuItem *OpenTrashItem;
	TMenuItem *OptionItem;
	TMenuItem *OptionMenu;
	TMenuItem *OrgOrderItem;
	TMenuItem *PageBindItem;
	TMenuItem *PathMaskItem;
	TMenuItem *Pop_g_1;
	TMenuItem *Pop_g_2;
	TMenuItem *Pop_g_3;
	TMenuItem *Pop_g_4;
	TMenuItem *PopAddTabItem;
	TMenuItem *PopClrLogItem;
	TMenuItem *PopCopyFileInfoItm;
	TMenuItem *PopCopyItem;
	TMenuItem *PopCopyValueItem;
	TMenuItem *PopCsvCalcItem;
	TMenuItem *PopCsvGraphItem;
	TMenuItem *PopDebugInfItem;
	TMenuItem *PopDelTabItem;
	TMenuItem *PopEditFontSmplItem;
	TMenuItem *PopFindItem;
	TMenuItem *PopGrepClipCopyItem;
	TMenuItem *PopGrepConfirmItem;
	TMenuItem *PopGrepEmFilterItem;
	TMenuItem *PopGrepExceptItem;
	TMenuItem *PopGrepExtractItem;
	TMenuItem *PopGrepFileItemNoItem;
	TMenuItem *PopGrepItemNoItem;
	TMenuItem *PopGrepLineCopyItem;
	TMenuItem *PopGrepOmitItem;
	TMenuItem *PopGrepOpenUrlItem;
	TMenuItem *PopGrepReleaseItem;
	TMenuItem *PopGrepSaveAsItem;
	TMenuItem *PopGrepSelResItem;
	TMenuItem *PopGrepSubDirItem;
	TMenuItem *PopGrepTrimTopItem;
	TMenuItem *PopLoadTabGroupItem;
	TMenuItem *PopPopResultItem;
	TMenuItem *PopPropertyItem;
	TMenuItem *PopPushResultItem;
	TMenuItem *PopResultItem;
	TMenuItem *PopSaveAsTabGroupItem;
	TMenuItem *PopSaveDumpItem;
	TMenuItem *PopSaveTagGroupItem;
	TMenuItem *PopSelAllItem;
	TMenuItem *PopSoloTabItem;
	TMenuItem *PopTabDlgItem;
	TMenuItem *PopTabHomeItem;
	TMenuItem *PopViewLogItem;
	TMenuItem *PopWebSearchItem;
	TMenuItem *PowerShellItem;
	TMenuItem *PoxExpCsvItem;
	TMenuItem *PrintItem;
	TMenuItem *PropertyItem;
	TMenuItem *PushResultItem;
	TMenuItem *RecentListItem;
	TMenuItem *RegDirItem;
	TMenuItem *RegExChkItem;
	TMenuItem *RegSyncDlgItem;
	TMenuItem *ReloadItem;
	TMenuItem *RepositoryListItem;
	TMenuItem *RestartItem;
	TMenuItem *ReternListItem;
	TMenuItem *SaveAsResultItem;
	TMenuItem *SaveAsTabGroupItem;
	TMenuItem *SaveAsWorkFileItem;
	TMenuItem *SaveDumpItem;
	TMenuItem *SaveWorkListItem;
	TMenuItem *SelAllFileItem;
	TMenuItem *SelAllItem;
	TMenuItem *SelDriveItem;
	TMenuItem *Sep_e_1;
	TMenuItem *Sep_e_2;
	TMenuItem *Sep_e_3;
	TMenuItem *Sep_e_4;
	TMenuItem *Sep_e_5;
	TMenuItem *Sep_e_6;
	TMenuItem *Sep_f_1;
	TMenuItem *Sep_f_2;
	TMenuItem *Sep_f_3;
	TMenuItem *Sep_f_4;
	TMenuItem *Sep_f_5;
	TMenuItem *Sep_f_6;
	TMenuItem *Sep_f_7;
	TMenuItem *Sep_g_1;
	TMenuItem *Sep_g_2;
	TMenuItem *Sep_h_1;
	TMenuItem *Sep_i_1;
	TMenuItem *Sep_i_2;
	TMenuItem *Sep_i_3;
	TMenuItem *Sep_ip_1;
	TMenuItem *Sep_l_1;
	TMenuItem *Sep_l_2;
	TMenuItem *Sep_o_1;
	TMenuItem *Sep_o_2;
	TMenuItem *Sep_o_3;
	TMenuItem *Sep_o_4;
	TMenuItem *Sep_o_5;
	TMenuItem *Sep_pt_1;
	TMenuItem *Sep_pt_2;
	TMenuItem *Sep_r_1;
	TMenuItem *Sep_r_2;
	TMenuItem *Sep_r_3;
	TMenuItem *Sep_s_1;
	TMenuItem *Sep_t_1;
	TMenuItem *Sep_t_2;
	TMenuItem *Sep_t_3;
	TMenuItem *Sep_t_4;
	TMenuItem *Sep_t_5;
	TMenuItem *Sep_tp_1;
	TMenuItem *Sep_tp_2;
	TMenuItem *Sep_v_1;
	TMenuItem *Sep_v_2;
	TMenuItem *Sep_v_3;
	TMenuItem *Sep_v_4;
	TMenuItem *Sep_v_5;
	TMenuItem *Sep_v_6;
	TMenuItem *Sep_v_7;
	TMenuItem *Sep_vp_1;
	TMenuItem *Sep_vp_2;
	TMenuItem *Sep_vp_3;
	TMenuItem *SetAliasItem;
	TMenuItem *SetColorItem;
	TMenuItem *SetExifTimeItem;
	TMenuItem *SetFolderIconItem;
	TMenuItem *SetTagItem;
	TMenuItem *SetToolBtnItem;
	TMenuItem *ShareListItem;
	TMenuItem *ShowCrItem;
	TMenuItem *ShowGridItem;
	TMenuItem *ShowIconItem;
	TMenuItem *ShowLineNoItem;
	TMenuItem *ShowLogWinItem;
	TMenuItem *ShowPreviewItem;
	TMenuItem *ShowPropertyItem;
	TMenuItem *ShowRubyItem;
	TMenuItem *ShowRulerItem;
	TMenuItem *ShowSeekBarItem;
	TMenuItem *ShowSttBarItem;
	TMenuItem *ShowTabBarItem;
	TMenuItem *ShowTabItem;
	TMenuItem *ShowToolBarItem;
	TMenuItem *SidebarItem;
	TMenuItem *SidebarPos0Item;
	TMenuItem *SidebarPos1Item;
	TMenuItem *SidebarPosItem;
	TMenuItem *SortAscItem;
	TMenuItem *SortDesItem;
	TMenuItem *SortItem;
	TMenuItem *SpecialDirListItem;
	TMenuItem *SubViewerItem;
	TMenuItem *TaskManItem;
	TMenuItem *TextEncItem;
	TMenuItem *ThumbnailExItem;
	TMenuItem *ThumbnailItem;
	TMenuItem *ThumbPos0Item;
	TMenuItem *ThumbPos1Item;
	TMenuItem *ThumbPos2Item;
	TMenuItem *ThumbPos3Item;
	TMenuItem *ThumbPosItem;
	TMenuItem *ToLowerItem;
	TMenuItem *ToolMenu;
	TMenuItem *TopIsHdrItem;
	TMenuItem *ToUpperItem;
	TMenuItem *TrayAppListItem;
	TMenuItem *TrayDuplItem;
	TMenuItem *TrayExitItem;
	TMenuItem *TrayRestartItem;
	TMenuItem *TrayTaskManItem;
	TMenuItem *TrimTagDataItem;
	TMenuItem *TV_EditCopyItem;
	TMenuItem *TV_EditSelectAllItem;
	TMenuItem *TV_FindTextItem;
	TMenuItem *TxtEditCopyItem;
	TMenuItem *TxtEditSelectAllItem;
	TMenuItem *TxtKeepIndexItem;
	TMenuItem *TxtOpenUrlItem;
	TMenuItem *TxtShowLineNoItem;
	TMenuItem *TxtShowTailItem;
	TMenuItem *UpdateFromArcItem;
	TMenuItem *UpdateItem;
	TMenuItem *UserDefListItem;
	TMenuItem *ViewCopyItem;
	TMenuItem *ViewHistoryItem;
	TMenuItem *ViewIniFileItem;
	TMenuItem *ViewMenu;
	TMenuItem *ViewSelAllItem;
	TMenuItem *WarnHighItem;
	TMenuItem *WevSearchItem;
	TMenuItem *WorkItemDownItem;
	TMenuItem *WorkItemUpItem;
	TMenuItem *WorkListHistoryItem;
	TMenuItem *ZoomInItem;
	TMenuItem *ZoomOutItem;
	TPageControl *GrepPageControl;
	TPaintBox *RelPaintBox;
	TPaintBox *RelPaintBox2;
	TPaintBox *TabBottomPaintBox;
	TPaintBox *TaskPaintBox;
	TPaintBox *TextMarginBox;
	TPaintBox *TextPaintBox;
	TPaintBox *TextRulerBox;
	TPanel *GrepFilterPanel;
	TPanel *GrepM1Panel;
	TPanel *GrepM2Panel;
	TPanel *GrepM3Panel;
	TPanel *GrepMidPanel;
	TPanel *GrepOpPanel;
	TPanel *GrepPanel;
	TPanel *GrepSttPanel;
	TPanel *GrepT1Panel;
	TPanel *GrepT2Panel;
	TPanel *GrepT11Panel;
	TPanel *GrepT12Panel;
	TPanel *HdrPanel;
	TPanel *HistDockPanel;
	TPanel *ImgMainPanel;
	TPanel *ImgScrollPanel;
	TPanel *ImgSidePanel;
	TPanel *ImgViewPanel;
	TPanel *InfListPanel;
	TPanel *InfPanel;
	TPanel *InpDirPanel;
	TPanel *L_DirPanel;
	TPanel *L_DirPanel2;
	TPanel *L_HdrPanel;
	TPanel *L_Panel;
	TPanel *L_StatPanel;
	TPanel *L_TopPanel;
	TPanel *ListPanel;
	TPanel *LockTxtPanel;
	TPanel *LogListPanel;
	TPanel *LogPanel;
	TPanel *LogSplitPanel;
	TPanel *LoupeDockPanel;
	TPanel *MainContainer;
	TPanel *MainPanel;
	TPanel *PreviewPanel;
	TPanel *ProgressPanel;
	TPanel *PrvInnerPanel;
	TPanel *R_DirPanel;
	TPanel *R_DirPanel2;
	TPanel *R_HdrPanel;
	TPanel *R_Panel;
	TPanel *R_StatPanel;
	TPanel *R_TopPanel;
	TPanel *RelPanel;
	TPanel *RelPanel2;
	TPanel *RepT1Panel;
	TPanel *RepT2Panel;
	TPanel *SeekEndPanel;
	TPanel *SeekPanel;
	TPanel *SeekSttPanel;
	TPanel *SeekTopPanel;
	TPanel *SideDockPanel;
	TPanel *SideInfPanel;
	TPanel *SideMainPanel;
	TPanel *SubPanel;
	TPanel *TabOuterPanel;
	TPanel *TabPanel;
	TPanel *TextCurColPanel;
	TPanel *TxtMainPanel;
	TPanel *TxtPrvListPanel;
	TPanel *TxtScrollPanel;
	TPanel *TxtTailListPanel;
	TPanel *TxtViewPanel;
	TPopupMenu *DrivePopupMenu;
	TPopupMenu *DropPopupMenu;
	TPopupMenu *ExPopupMenu;
	TPopupMenu *GrepPopupMenu;
	TPopupMenu *ImgPrvPopupMenu;
	TPopupMenu *InfPopupMenu;
	TPopupMenu *LogPopupMenu;
	TPopupMenu *SelItemPopupMenu;
	TPopupMenu *TabPopupMenu;
	TPopupMenu *TrayPopupMenu;
	TPopupMenu *TxtPrvPopupMenu;
	TPopupMenu *ViewPopupMenu;
	TProgressBar *WorkProgressBar;
	TRichEdit *ViewMemo;
	TScrollBar *TextScrollBar;
	TScrollBox *ImgScrollBox;
	TSpeedButton *DelTabBtn;
	TSpeedButton *HideSeekBtn;
	TSpeedButton *L_SelDrvBtn;
	TSpeedButton *L_SelDrvBtn2;
	TSpeedButton *PopTabBtn;
	TSpeedButton *R_SelDrvBtn;
	TSpeedButton *R_SelDrvBtn2;
	TSpeedButton *RefDirBtn;
	TSpeedButton *SeekLBtn;
	TSpeedButton *SeekRBtn;
	TSplitter *GrepMidSplitter;
	TSplitter *GrepSttSplitter;
	TSplitter *GrepTopSplitter;
	TSplitter *ImgInfSplitter;
	TSplitter *ImgSideSplitter;
	TSplitter *InfLogSplitter;
	TSplitter *ListSubSplitter;
	TSplitter *LRSplitter;
	TSplitter *RepTopSplitter;
	TSplitter *SideDockSplitter;
	TSplitter *TxtPrvSplitter;
	TStatusBar *ClockBar;
	TStatusBar *ClockBarI;
	TStatusBar *GrepStatusBar;
	TStatusBar *ImgInfBar;
	TStatusBar *ImgSttHeader;
	TStatusBar *StatusBar1;
	TStatusBar *TxtSttHeader;
	TStringGrid *ThumbnailGrid;
	TTabControl *TabControl1;
	TTabSheet *FindSheet;
	TTabSheet *ReplaceSheet;
	TTimer *BlinkTimer;
	TTimer *KeyHintTimer;
	TTimer *MsgHintTimer;
	TTimer *TaskSttTimer;
	TTimer *UpdLogTimer;
	TTimer *WatchDirTimer;
	TTimer *WatchTailTimer;
	TToolBar *FKeyBar;
	TToolBar *ToolBarF;
	TToolBar *ToolBarI;
	TToolBar *ToolBarI2;
	TToolBar *ToolBarV;
	TTrackBar *SeekBar;
	TTrayIcon *TrayIcon1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ApplicationEvents1Activate(TObject *Sender);
	void __fastcall ApplicationEvents1Deactivate(TObject *Sender);
	void __fastcall ApplicationEvents1Message(tagMSG &Msg, bool &Handled);
	void __fastcall ApplicationEvents1ShowHint(UnicodeString &HintStr, bool &CanShow, THintInfo &HintInfo);
	bool __fastcall ApplicationEvents1Help(WORD Command, NativeInt Data, bool &CallHelp);
	void __fastcall ApplicationEvents1Idle(TObject *Sender, bool &Done);
	void __fastcall ActionList1Execute(TBasicAction *Action, bool &Handled);
	void __fastcall ListPanelResize(TObject *Sender);
	void __fastcall L_PanelResize(TObject *Sender);
	void __fastcall R_PanelResize(TObject *Sender);
	void __fastcall LRSplitterCanResize(TObject *Sender, int &NewSize, bool &Accept);
	void __fastcall LRSplitterMoved(TObject *Sender);
	void __fastcall ListSubSplitterCanResize(TObject *Sender, int &NewSize, bool &Accept);
	void __fastcall SplitterBgPaint(TObject *Sender);
	void __fastcall InfLogSplitterMoved(TObject *Sender);
	void __fastcall ImgInfSplitterCanResize(TObject *Sender, int &NewSize, bool &Accept);
	void __fastcall ImgInfSplitterMoved(TObject *Sender);
	void __fastcall SubPanelResize(TObject *Sender);
	void __fastcall RepT1PanelResize(TObject *Sender);
	void __fastcall GrepT11PanelResize(TObject *Sender);
	void __fastcall TaskSttTimerTimer(TObject *Sender);
	void __fastcall WatchDirTimerTimer(TObject *Sender);
	void __fastcall WatchTailTimerTimer(TObject *Sender);
	void __fastcall UpdLogTimerTimer(TObject *Sender);
	void __fastcall MsgHintTimerTimer(TObject *Sender);
	void __fastcall KeyHintTimerTimer(TObject *Sender);
	void __fastcall BlinkTimerTimer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SubListBoxExit(TObject *Sender);
	void __fastcall FileListBoxEnter(TObject *Sender);
	void __fastcall FileListBoxExit(TObject *Sender);
	void __fastcall FileListDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall FileListKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FileListKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FileListBoxKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FileListBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall FileListBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FileListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FileListBoxDblClick(TObject *Sender);
	void __fastcall FileListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y, TDragState State, bool &Accept);
	void __fastcall FileListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall InfListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall InfListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall LogListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall LogListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall LogListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall LogListBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall LogListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TextPaintBoxPaint(TObject *Sender);
	void __fastcall TextPaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TextPaintBoxDblClick(TObject *Sender);
	void __fastcall RelPaintBoxPaint(TObject *Sender);
	void __fastcall ViewerImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ViewerImageMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ViewerImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ViewerImageDblClick(TObject *Sender);
	void __fastcall ViewerImageMouseLeave(TObject *Sender);
	void __fastcall ThumbnailGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall ThumbnailGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ThumbnailGridMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ThumbnailGridClick(TObject *Sender);
	void __fastcall ThumbnailGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall StatusBarDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall MainMenuClick(TObject *Sender);
	void __fastcall FileMenuClick(TObject *Sender);
	void __fastcall ExtMenuClick(TObject *Sender);
	void __fastcall CmdIndexItemClick(TObject *Sender);
	void __fastcall SelAllFileActionExecute(TObject *Sender);
	void __fastcall RegDirDlgActionExecute(TObject *Sender);
	void __fastcall CopyFileNameActionExecute(TObject *Sender);
	void __fastcall DirHistoryActionExecute(TObject *Sender);
	void __fastcall DriveListActionExecute(TObject *Sender);
	void __fastcall OpenByExpActionExecute(TObject *Sender);
	void __fastcall TaskManActionExecute(TObject *Sender);
	void __fastcall SetExifTimeActionExecute(TObject *Sender);
	void __fastcall ReloadListActionExecute(TObject *Sender);
	void __fastcall PropertyDlgActionExecute(TObject *Sender);
	void __fastcall PropertyDlgActionUpdate(TObject *Sender);
	void __fastcall BackDirHistActionExecute(TObject *Sender);
	void __fastcall BackDirHistActionUpdate(TObject *Sender);
	void __fastcall MaskSelectActionExecute(TObject *Sender);
	void __fastcall NameToUpperActionExecute(TObject *Sender);
	void __fastcall NameToLowerActionExecute(TObject *Sender);
	void __fastcall CreateShortcutActionExecute(TObject *Sender);
	void __fastcall ShowOpenAsRunActionExecute(TObject *Sender);
	void __fastcall ShowOpenAsRunActionUpdate(TObject *Sender);
	void __fastcall SelReverseActionExecute(TObject *Sender);
	void __fastcall FindFileDlgActionExecute(TObject *Sender);
	void __fastcall FindDlgActionUpdate(TObject *Sender);
	void __fastcall NewTextFileActionExecute(TObject *Sender);
	void __fastcall PackActionExecute(TObject *Sender);
	void __fastcall UnPackActionExecute(TObject *Sender);
	void __fastcall SelAllItemActionExecute(TObject *Sender);
	void __fastcall TaskManActionUpdate(TObject *Sender);
	void __fastcall ExtMenuItemClick(TObject *Sender);
	void __fastcall EjectActionExecute(TObject *Sender);
	void __fastcall CalcDirSizeActionExecute(TObject *Sender);
	void __fastcall CursorDownActionExecute(TObject *Sender);
	void __fastcall CursorUpActionExecute(TObject *Sender);
	void __fastcall CursorDownSelActionExecute(TObject *Sender);
	void __fastcall CursorUpSelActionExecute(TObject *Sender);
	void __fastcall KeyListActionExecute(TObject *Sender);
	void __fastcall ContextMenuActionExecute(TObject *Sender);
	void __fastcall ClearLogActionExecute(TObject *Sender);
	void __fastcall FileEditActionExecute(TObject *Sender);
	void __fastcall SwapLRActionExecute(TObject *Sender);
	void __fastcall PopFindItemClick(TObject *Sender);
	void __fastcall PathMaskDlgActionExecute(TObject *Sender);
	void __fastcall EqualListWidthActionExecute(TObject *Sender);
	void __fastcall ExeCommandLineActionExecute(TObject *Sender);
	void __fastcall CommandPromptActionExecute(TObject *Sender);
	void __fastcall SortDlgActionExecute(TObject *Sender);
	void __fastcall RenameDlgActionExecute(TObject *Sender);
	void __fastcall OpenByAppActionExecute(TObject *Sender);
	void __fastcall SelectActionExecute(TObject *Sender);
	void __fastcall SelectUpActionExecute(TObject *Sender);
	void __fastcall CreateDirActionExecute(TObject *Sender);
	void __fastcall TextViewerActionExecute(TObject *Sender);
	void __fastcall ImageViewerActionExecute(TObject *Sender);
	void __fastcall OpenByWinActionExecute(TObject *Sender);
	void __fastcall CompareDlgActionExecute(TObject *Sender);
	void __fastcall WorkListActionExecute(TObject *Sender);
	void __fastcall OptionDlgActionExecute(TObject *Sender);
	void __fastcall OptionDlgActionUpdate(TObject *Sender);
	void __fastcall MenuBarActionExecute(TObject *Sender);
	void __fastcall MenuBarActionUpdate(TObject *Sender);
	void __fastcall ExPopupMenuActionExecute(TObject *Sender);
	void __fastcall WinMaximizeActionExecute(TObject *Sender);
	void __fastcall WinMinimizeActionExecute(TObject *Sender);
	void __fastcall WinNormalActionExecute(TObject *Sender);
	void __fastcall WinPosActionExecute(TObject *Sender);
	void __fastcall SelSameNameActionExecute(TObject *Sender);
	void __fastcall LoadWorkListActionExecute(TObject *Sender);
	void __fastcall SaveAsWorkListActionExecute(TObject *Sender);
	void __fastcall NetConnectActionExecute(TObject *Sender);
	void __fastcall NetDisconnectActionExecute(TObject *Sender);
	void __fastcall AboutNyanFiActionExecute(TObject *Sender);
	void __fastcall ExitActionExecute(TObject *Sender);
	void __fastcall ExitActionUpdate(TObject *Sender);
	void __fastcall SetAliasActionExecute(TObject *Sender);
	void __fastcall IncSearchActionExecute(TObject *Sender);
	void __fastcall SelOnlyCurActionExecute(TObject *Sender);
	void __fastcall CurrFromOppActionExecute(TObject *Sender);
	void __fastcall CurrToOppActionExecute(TObject *Sender);
	void __fastcall ToRootActionExecute(TObject *Sender);
	void __fastcall ToParentActionExecute(TObject *Sender);
	void __fastcall ToLeftActionExecute(TObject *Sender);
	void __fastcall ToRightActionExecute(TObject *Sender);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyToActionExecute(TObject *Sender);
	void __fastcall MoveActionExecute(TObject *Sender);
	void __fastcall MoveToActionExecute(TObject *Sender);
	void __fastcall CloneActionExecute(TObject *Sender);
	void __fastcall CloneToCurrActionExecute(TObject *Sender);
	void __fastcall DeleteActionExecute(TObject *Sender);
	void __fastcall PageDownActionExecute(TObject *Sender);
	void __fastcall PageUpActionExecute(TObject *Sender);
	void __fastcall ClearMaskActionExecute(TObject *Sender);
	void __fastcall CheckUpdateActionExecute(TObject *Sender);
	void __fastcall CheckUpdateActionUpdate(TObject *Sender);
	void __fastcall ResultListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall ResultListBoxExit(TObject *Sender);
	void __fastcall ResultListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ResultListBoxClick(TObject *Sender);
	void __fastcall ResultListBoxDblClick(TObject *Sender);
	void __fastcall GrepStartActionExecute(TObject *Sender);
	void __fastcall GrepStartActionUpdate(TObject *Sender);
	void __fastcall GrepActionExecute(TObject *Sender);
	void __fastcall GrepAbortActionExecute(TObject *Sender);
	void __fastcall GrepAbortActionUpdate(TObject *Sender);
	void __fastcall GrepClipCopyActionExecute(TObject *Sender);
	void __fastcall GrepSaveAsActionExecute(TObject *Sender);
	void __fastcall GrepExtractActionExecute(TObject *Sender);
	void __fastcall GrepResultActionUpdate(TObject *Sender);
	void __fastcall GrepExceptActionExecute(TObject *Sender);
	void __fastcall GrepReleaseActionExecute(TObject *Sender);
	void __fastcall GrepReleaseActionUpdate(TObject *Sender);
	void __fastcall GrepOptionActionExecute(TObject *Sender);
	void __fastcall GrepPageControlChanging(TObject *Sender, bool &AllowChange);
	void __fastcall GrepShowSubDirActionExecute(TObject *Sender);
	void __fastcall GrepShowSubDirActionUpdate(TObject *Sender);
	void __fastcall GrepLineCopyActionExecute(TObject *Sender);
	void __fastcall GrepActionUpdate(TObject *Sender);
	void __fastcall GrepOptionActionUpdate(TObject *Sender);
	void __fastcall GrepPageControlChange(TObject *Sender);
	void __fastcall GrepFilterEditChange(TObject *Sender);
	void __fastcall GrepFilterEditEnter(TObject *Sender);
	void __fastcall GrepFilterEditExit(TObject *Sender);
	void __fastcall GrepFilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GrepFilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall GrepFindComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GrepFindComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall GrepSttSplitterMoved(TObject *Sender);
	void __fastcall GrepCanBtnClick(TObject *Sender);
	void __fastcall InsSeparatorActionExecute(TObject *Sender);
	void __fastcall EditHistoryActionExecute(TObject *Sender);
	void __fastcall ReturnListActionExecute(TObject *Sender);
	void __fastcall FileExtListActionExecute(TObject *Sender);
	void __fastcall FileListOnlyActionExecute(TObject *Sender);
	void __fastcall FileListOnlyActionUpdate(TObject *Sender);
	void __fastcall HelpContentsActionExecute(TObject *Sender);
	void __fastcall CursorTopActionExecute(TObject *Sender);
	void __fastcall CursorEndActionExecute(TObject *Sender);
	void __fastcall CursorTopSelActionExecute(TObject *Sender);
	void __fastcall CursorEndSelActionExecute(TObject *Sender);
	void __fastcall SelEmptyDirActionExecute(TObject *Sender);
	void __fastcall FindDirDlgActionExecute(TObject *Sender);
	void __fastcall CompleteDeleteActionExecute(TObject *Sender);
	void __fastcall CodePageActionExecute(TObject *Sender);
	void __fastcall ChgCodePageActionExecute(TObject *Sender);
	void __fastcall TxtViewEditActionUpdate(TObject *Sender);
	void __fastcall ClipCopyActionExecute(TObject *Sender);
	void __fastcall ClipCopyActionUpdate(TObject *Sender);
	void __fastcall FindTextActionExecute(TObject *Sender);
	void __fastcall KeyListActionUpdate(TObject *Sender);
	void __fastcall ReturnListActionUpdate(TObject *Sender);
	void __fastcall CopyFileInfoActionExecute(TObject *Sender);
	void __fastcall NextSameNameActionExecute(TObject *Sender);
	void __fastcall FittedSizeActionExecute(TObject *Sender);
	void __fastcall FittedSizeActionUpdate(TObject *Sender);
	void __fastcall EqualSizeActionExecute(TObject *Sender);
	void __fastcall EqualSizeActionUpdate(TObject *Sender);
	void __fastcall ZoomInIActionExecute(TObject *Sender);
	void __fastcall ZoomIActionUpdate(TObject *Sender);
	void __fastcall ZoomOutIActionExecute(TObject *Sender);
	void __fastcall ReplaceStartActionExecute(TObject *Sender);
	void __fastcall ReplaceStartActionUpdate(TObject *Sender);
	void __fastcall PrevFileActionExecute(TObject *Sender);
	void __fastcall NextFileActionExecute(TObject *Sender);
	void __fastcall ScrollUpIActionExecute(TObject *Sender);
	void __fastcall ScrollActionUpdate(TObject *Sender);
	void __fastcall ScrollDownIActionExecute(TObject *Sender);
	void __fastcall ScrollLeftActionExecute(TObject *Sender);
	void __fastcall ScrollRightActionExecute(TObject *Sender);
	void __fastcall ConvertImageActionExecute(TObject *Sender);
	void __fastcall ConvertTextEncActionExecute(TObject *Sender);
	void __fastcall ExtractIconActionExecute(TObject *Sender);
	void __fastcall ShowGridActionExecute(TObject *Sender);
	void __fastcall ShowGridActionUpdate(TObject *Sender);
	void __fastcall GrayScaleActionExecute(TObject *Sender);
	void __fastcall GrayScaleActionUpdate(TObject *Sender);
	void __fastcall RotateLeftActionExecute(TObject *Sender);
	void __fastcall RotateActionUpdate(TObject *Sender);
	void __fastcall RotateRightActionExecute(TObject *Sender);
	void __fastcall FlipHorzActionExecute(TObject *Sender);
	void __fastcall FlipVertActionExecute(TObject *Sender);
	void __fastcall CloseIActionExecute(TObject *Sender);
	void __fastcall ThumbnailActionExecute(TObject *Sender);
	void __fastcall ThumbnailActionUpdate(TObject *Sender);
	void __fastcall ThumbnailExActionExecute(TObject *Sender);
	void __fastcall ThumbnailExActionUpdate(TObject *Sender);
	void __fastcall IV_ThumbPosActionExecute(TObject *Sender);
	void __fastcall IV_ThumbPosActionUpdate(TObject *Sender);
	void __fastcall ThumbPosItemClick(TObject *Sender);
	void __fastcall NextPageActionExecute(TObject *Sender);
	void __fastcall PrevPageActionExecute(TObject *Sender);
	void __fastcall DeleteIActionExecute(TObject *Sender);
	void __fastcall SortDlgIActionExecute(TObject *Sender);
	void __fastcall PageUpIActionExecute(TObject *Sender);
	void __fastcall PageIxActionUpdate(TObject *Sender);
	void __fastcall PageDownIActionExecute(TObject *Sender);
	void __fastcall ChgCodePageActionUpdate(TObject *Sender);
	void __fastcall SendToWorkListActionExecute(TObject *Sender);
	void __fastcall SendToWorkListActionUpdate(TObject *Sender);
	void __fastcall WorkListIActionExecute(TObject *Sender);
	void __fastcall WorkListIActionUpdate(TObject *Sender);
	void __fastcall SaveWorkListActionExecute(TObject *Sender);
	void __fastcall NewWorkListActionExecute(TObject *Sender);
	void __fastcall WorkListHstItemClick(TObject *Sender);
	void __fastcall MenuFActionUpdate(TObject *Sender);
	void __fastcall MenuFIActionUpdate(TObject *Sender);
	void __fastcall MenuFVActionUpdate(TObject *Sender);
	void __fastcall MenuFVIActionUpdate(TObject *Sender);
	void __fastcall MenuVUpdate(TObject *Sender);
	void __fastcall MenuIActionUpdate(TObject *Sender);
	void __fastcall SaveWorkListActionUpdate(TObject *Sender);
	void __fastcall HomeWorkListActionExecute(TObject *Sender);
	void __fastcall HomeWorkListActionUpdate(TObject *Sender);
	void __fastcall ChangeDriveActionExecute(TObject *Sender);
	void __fastcall ChangeDirActionExecute(TObject *Sender);
	void __fastcall ChangeOppDirActionExecute(TObject *Sender);
	void __fastcall ChangeRegDirActionExecute(TObject *Sender);
	void __fastcall ChangeOppRegDirActionExecute(TObject *Sender);
	void __fastcall RegDirPopupActionExecute(TObject *Sender);
	void __fastcall HistogramActionExecute(TObject *Sender);
	void __fastcall HistogramActionUpdate(TObject *Sender);
	void __fastcall DelJpgExifActionExecute(TObject *Sender);
	void __fastcall BinaryEditActionExecute(TObject *Sender);
	void __fastcall JoinTextActionExecute(TObject *Sender);
	void __fastcall ToParentOnLeftActionExecute(TObject *Sender);
	void __fastcall ToParentOnRightActionExecute(TObject *Sender);
	void __fastcall NextDriveActionExecute(TObject *Sender);
	void __fastcall PrevDriveActionExecute(TObject *Sender);
	void __fastcall BorderLeftActionExecute(TObject *Sender);
	void __fastcall BorderRightActionExecute(TObject *Sender);
	void __fastcall WorkItemDownActionExecute(TObject *Sender);
	void __fastcall WorkItemActionUpdate(TObject *Sender);
	void __fastcall WorkItemUpActionExecute(TObject *Sender);
	void __fastcall WidenCurListActionExecute(TObject *Sender);
	void __fastcall OpenStandardActionExecute(TObject *Sender);
	void __fastcall GrepFltOptCheckBoxClick(TObject *Sender);
	void __fastcall ExtractMp3ImgActionExecute(TObject *Sender);
	void __fastcall PageIActionUpdate(TObject *Sender);
	void __fastcall TopFileActionExecute(TObject *Sender);
	void __fastcall EndFileActionExecute(TObject *Sender);
	void __fastcall FileRunActionExecute(TObject *Sender);
	void __fastcall ExeExtToolActionExecute(TObject *Sender);
	void __fastcall ExeExtMenuActionExecute(TObject *Sender);
	void __fastcall ViewPopupMenuPopup(TObject *Sender);
	void __fastcall TV_EditSelAllActionExecute(TObject *Sender);
	void __fastcall TV_EditCopyActionExecute(TObject *Sender);
	void __fastcall TV_EditCopyActionUpdate(TObject *Sender);
	void __fastcall TV_SortAscActionExecute(TObject *Sender);
	void __fastcall TV_SortDesActionExecute(TObject *Sender);
	void __fastcall TV_OrgOrderActionExecute(TObject *Sender);
	void __fastcall TV_SortActionUpdate(TObject *Sender);
	void __fastcall TV_TopIsHdrActionExecute(TObject *Sender);
	void __fastcall TV_TopIsHdrActionUpdate(TObject *Sender);
	void __fastcall FunctionListActionExecute(TObject *Sender);
	void __fastcall FunctionListActionUpdate(TObject *Sender);
	void __fastcall UserDefListActionExecute(TObject *Sender);
	void __fastcall ViewHistoryActionExecute(TObject *Sender);
	void __fastcall HtmlToTextActionExecute(TObject *Sender);
	void __fastcall HtmlToTextActionUpdate(TObject *Sender);
	void __fastcall CP_xxx_ActionUpdate(TObject *Sender);
	void __fastcall PageUpSelActionExecute(TObject *Sender);
	void __fastcall PageDownSelActionExecute(TObject *Sender);
	void __fastcall ShowRubyActionExecute(TObject *Sender);
	void __fastcall ShowRubyActionUpdate(TObject *Sender);
	void __fastcall ViewLogActionExecute(TObject *Sender);
	void __fastcall Log_EditCopyExecute(TObject *Sender);
	void __fastcall Log_EditCopyUpdate(TObject *Sender);
	void __fastcall Log_EditSelectAllExecute(TObject *Sender);
	void __fastcall LogActionUpdate(TObject *Sender);
	void __fastcall ShowFileInfoActionExecute(TObject *Sender);
	void __fastcall DropMenuItemClick(TObject *Sender);
	void __fastcall ToLogActionExecute(TObject *Sender);
	void __fastcall UpdateFromArcActionExecute(TObject *Sender);
	void __fastcall SelReverseAllActionExecute(TObject *Sender);
	void __fastcall FullScreenActionExecute(TObject *Sender);
	void __fastcall FullScreenActionUpdate(TObject *Sender);
	void __fastcall ToOppSameItemActionExecute(TObject *Sender);
	void __fastcall ListArchiveActionExecute(TObject *Sender);
	void __fastcall ListDurationActionExecute(TObject *Sender);
	void __fastcall ListExpFuncActionExecute(TObject *Sender);
	void __fastcall ListFileNameActionExecute(TObject *Sender);
	void __fastcall ListLogActionExecute(TObject *Sender);
	void __fastcall ListTextActionExecute(TObject *Sender);
	void __fastcall TestArchiveActionExecute(TObject *Sender);
	void __fastcall LogFileInfoActionExecute(TObject *Sender);
	void __fastcall CopyPreImgOrgActionExecute(TObject *Sender);
	void __fastcall CopyPreImgOrgActionUpdate(TObject *Sender);
	void __fastcall CopyPreImgActionExecute(TObject *Sender);
	void __fastcall ShowPreviewActionExecute(TObject *Sender);
	void __fastcall ShowIconActionExecute(TObject *Sender);
	void __fastcall ShowHideAtrActionExecute(TObject *Sender);
	void __fastcall ShowSystemAtrActionExecute(TObject *Sender);
	void __fastcall ExeCommandsActionExecute(TObject *Sender);
	void __fastcall InputCommandsActionExecute(TObject *Sender);
	void __fastcall ShowStatusBarActionExecute(TObject *Sender);
	void __fastcall ShowStatusBarActionUpdate(TObject *Sender);
	void __fastcall ShowIconActionUpdate(TObject *Sender);
	void __fastcall ShowPreviewActionUpdate(TObject *Sender);
	void __fastcall CsrDirToOppActionExecute(TObject *Sender);
	void __fastcall ToOppositeActionExecute(TObject *Sender);
	void __fastcall NameFromClipActionExecute(TObject *Sender);
	void __fastcall SelMaskActionExecute(TObject *Sender);
	void __fastcall InputDirActionExecute(TObject *Sender);
	void __fastcall PushDirActionExecute(TObject *Sender);
	void __fastcall PopDirActionExecute(TObject *Sender);
	void __fastcall DirStackActionExecute(TObject *Sender);
	void __fastcall FindFileDirDlgActionExecute(TObject *Sender);
	void __fastcall CharInfoActionUpdate(TObject *Sender);
	void __fastcall CharInfoActionExecute(TObject *Sender);
	void __fastcall TextScrollBoxDblClick(TObject *Sender);
	void __fastcall DateSelectActionExecute(TObject *Sender);
	void __fastcall SelSameExtActionExecute(TObject *Sender);
	void __fastcall ClearAllActionExecute(TObject *Sender);
	void __fastcall CancelAllTaskActionExecute(TObject *Sender);
	void __fastcall CancelAllTaskActionUpdate(TObject *Sender);
	void __fastcall MaskFindActionExecute(TObject *Sender);
	void __fastcall PowerOffActionExecute(TObject *Sender);
	void __fastcall RebootActionExecute(TObject *Sender);
	void __fastcall PasteActionExecute(TObject *Sender);
	void __fastcall PasteActionUpdate(TObject *Sender);
	void __fastcall ShowPropertyActionExecute(TObject *Sender);
	void __fastcall ShowPropertyActionUpdate(TObject *Sender);
	void __fastcall LoupeActionExecute(TObject *Sender);
	void __fastcall LoupeActionUpdate(TObject *Sender);
	void __fastcall WarnHighlightActionExecute(TObject *Sender);
	void __fastcall WarnHighlightActionUpdate(TObject *Sender);
	void __fastcall EmptyTrashActionExecute(TObject *Sender);
	void __fastcall InputPathMaskActionExecute(TObject *Sender);
	void __fastcall PushResultActionExecute(TObject *Sender);
	void __fastcall PopResultActionExecute(TObject *Sender);
	void __fastcall PopResultActionUpdate(TObject *Sender);
	void __fastcall SidebarActionExecute(TObject *Sender);
	void __fastcall SidebarActionUpdate(TObject *Sender);
	void __fastcall SidePanelDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y);
	void __fastcall SidePanelUnDock(TObject *Sender, TControl *Client, TWinControl *NewTarget, bool &Allow);
	void __fastcall ImgSidePanelDockDrop(TObject *Sender, TDragDockObject *Source, int X, int Y);
	void __fastcall IV_SidebarPosActionExecute(TObject *Sender);
	void __fastcall IV_SidebarPosActionUpdate(TObject *Sender);
	void __fastcall SidebarPosItemClick(TObject *Sender);
	void __fastcall SideDockPanelDockOver(TObject *Sender, TDragDockObject *Source,
			int X, int Y, TDragState State, bool &Accept);
	void __fastcall ImgSttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall InspectorActionExecute(TObject *Sender);
	void __fastcall InspectorActionUpdate(TObject *Sender);
	void __fastcall BitmapViewActionExecute(TObject *Sender);
	void __fastcall BitmapViewActionUpdate(TObject *Sender);
	void __fastcall SaveDumpActionExecute(TObject *Sender);
	void __fastcall SaveDumpActionUpdate(TObject *Sender);
	void __fastcall SetTopAddrActionExecute(TObject *Sender);
	void __fastcall SetTopAddrActionUpdate(TObject *Sender);
	void __fastcall TextScrollBarChange(TObject *Sender);
	void __fastcall PopSelectItemDrawItem(TObject *Sender, TCanvas *ACanvas, const TRect &ARect, bool Selected);
	void __fastcall PopSelectItemClick(TObject *Sender);
	void __fastcall SelDrvBtnClick(TObject *Sender);
	void __fastcall InpDirEditExit(TObject *Sender);
	void __fastcall InpDirEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall InpDirEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall SetWidthActionExecute(TObject *Sender);
	void __fastcall SetMarginActionExecute(TObject *Sender);
	void __fastcall ShowRulerActionExecute(TObject *Sender);
	void __fastcall ShowRulerActionUpdate(TObject *Sender);
	void __fastcall ShowLineNoActionExecute(TObject *Sender);
	void __fastcall ShowLineNoActionUpdate(TObject *Sender);
	void __fastcall CsvRecordActionExecute(TObject *Sender);
	void __fastcall CsvRecordActionUpdate(TObject *Sender);
	void __fastcall PackToCurrActionExecute(TObject *Sender);
	void __fastcall UnPackToCurrActionExecute(TObject *Sender);
	void __fastcall RestartActionExecute(TObject *Sender);
	void __fastcall RestartActionUpdate(TObject *Sender);
	void __fastcall MarkListVActionExecute(TObject *Sender);
	void __fastcall MarkListVActionUpdate(TObject *Sender);
	void __fastcall CmdFileListActionExecute(TObject *Sender);
	void __fastcall EditIniFileActionExecute(TObject *Sender);
	void __fastcall ViewIniFileActionExecute(TObject *Sender);
	void __fastcall SetColorActionExecute(TObject *Sender);
	void __fastcall SetColorActionUpdate(TObject *Sender);
	void __fastcall SetTabActionExecute(TObject *Sender);
	void __fastcall SetTvMetricActionUpdate(TObject *Sender);
	void __fastcall WebMapActionExecute(TObject *Sender);
	void __fastcall WebSearchActionExecute(TObject *Sender);
	void __fastcall WebSearchActionUpdate(TObject *Sender);
	void __fastcall SetPathMaskActionExecute(TObject *Sender);
	void __fastcall SetSttBarFmtActionExecute(TObject *Sender);
	void __fastcall SetSubSizeActionExecute(TObject *Sender);
	void __fastcall JumpToActionExecute(TObject *Sender);
	void __fastcall JumpToActionUpdate(TObject *Sender);
	void __fastcall MatchSelectActionExecute(TObject *Sender);
	void __fastcall TxtPrvListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall TxtTailListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall TxtPrvListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall Txt_EditCopyExecute(TObject *Sender);
	void __fastcall Txt_EditCopyUpdate(TObject *Sender);
	void __fastcall Txt_EditSelectAllExecute(TObject *Sender);
	void __fastcall Txt_ShowLnNoActionExecute(TObject *Sender);
	void __fastcall Txt_ShowLnNoActionUpdate(TObject *Sender);
	void __fastcall ScrollUpTextActionExecute(TObject *Sender);
	void __fastcall ScrollDownTextActionExecute(TObject *Sender);
	void __fastcall ScrollDownLogActionExecute(TObject *Sender);
	void __fastcall ScrollUpLogActionExecute(TObject *Sender);
	void __fastcall ExeMenuFileActionExecute(TObject *Sender);
	void __fastcall MenuFileItemClick(TObject *Sender);
	void __fastcall L_StatPanelClick(TObject *Sender);
	void __fastcall R_StatPanelClick(TObject *Sender);
	void __fastcall L_DirPanelClick(TObject *Sender);
	void __fastcall R_DirPanelClick(TObject *Sender);
	void __fastcall MonitorOffActionExecute(TObject *Sender);
	void __fastcall ShowToolBarActionExecute(TObject *Sender);
	void __fastcall ShowToolBarActionUpdate(TObject *Sender);
	void __fastcall ExeToolBtnActionExecute(TObject *Sender);
	void __fastcall ShowSystemAtrActionUpdate(TObject *Sender);
	void __fastcall ShowHideAtrActionUpdate(TObject *Sender);
	void __fastcall ToolBarDlgActionExecute(TObject *Sender);
	void __fastcall ToolBarDblClick(TObject *Sender);
	void __fastcall ToolBtnClick(TObject *Sender);
	void __fastcall PreviewImageDblClick(TObject *Sender);
	void __fastcall SuspendActionExecute(TObject *Sender);
	void __fastcall SuspendActionUpdate(TObject *Sender);
	void __fastcall PauseAllTaskActionExecute(TObject *Sender);
	void __fastcall PauseAllTaskActionUpdate(TObject *Sender);
	void __fastcall FileListHeaderSectionClick(THeaderControl *HeaderControl, THeaderSection *Section);
	void __fastcall CompareHashActionExecute(TObject *Sender);
	void __fastcall GetHashActionExecute(TObject *Sender);
	void __fastcall DelSelMaskActionExecute(TObject *Sender);
	void __fastcall SelWorkItemActionExecute(TObject *Sender);
	void __fastcall MarkActionExecute(TObject *Sender);
	void __fastcall MarkListActionExecute(TObject *Sender);
	void __fastcall MarkMaskActionExecute(TObject *Sender);
	void __fastcall ClearMarkActionExecute(TObject *Sender);
	void __fastcall FindMarkActionExecute(TObject *Sender);
	void __fastcall NextMarkActionExecute(TObject *Sender);
	void __fastcall PrevMarkActionExecute(TObject *Sender);
	void __fastcall SelMarkActionExecute(TObject *Sender);
	void __fastcall CopyPreImgActionUpdate(TObject *Sender);
	void __fastcall ColorPickerActionExecute(TObject *Sender);
	void __fastcall PrintActionExecute(TObject *Sender);
	void __fastcall LinkToOppActionExecute(TObject *Sender);
	void __fastcall EjectDriveActionExecute(TObject *Sender);
	void __fastcall ExeEjectDrive(UnicodeString dstr);
	void __fastcall SwapNameActionExecute(TObject *Sender);
	void __fastcall ScrollUpActionExecute(TObject *Sender);
	void __fastcall ScrollDownActionExecute(TObject *Sender);
	void __fastcall DoublePageActionExecute(TObject *Sender);
	void __fastcall DoublePageActionUpdate(TObject *Sender);
	void __fastcall PageBindActionExecute(TObject *Sender);
	void __fastcall PageBindActionUpdate(TObject *Sender);
	void __fastcall SeekBarChange(TObject *Sender);
	void __fastcall ShowSeekBarActionUpdate(TObject *Sender);
	void __fastcall ShowSeekBarActionExecute(TObject *Sender);
	void __fastcall JumpIndexActionExecute(TObject *Sender);
	void __fastcall BackupActionExecute(TObject *Sender);
	void __fastcall FL_HeaderControlDrawSection(THeaderControl *HeaderControl, THeaderSection *Section,
			const TRect &Rect, bool Pressed);
	void __fastcall ExtractChmSrcActionExecute(TObject *Sender);
	void __fastcall PopEjectItemClick(TObject *Sender);
	void __fastcall ShowByteSizeActionExecute(TObject *Sender);
	void __fastcall ShowByteSizeActionUpdate(TObject *Sender);
	void __fastcall FilterComboBoxChange(TObject *Sender);
	void __fastcall FilterComboBoxExit(TObject *Sender);
	void __fastcall FilterComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterComboBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FilterActionExecute(TObject *Sender);
	void __fastcall UseTrashActionExecute(TObject *Sender);
	void __fastcall UseTrashActionUpdate(TObject *Sender);
	void __fastcall ListNyanFiActionExecute(TObject *Sender);
	void __fastcall SyncLRActionExecute(TObject *Sender);
	void __fastcall SyncLRActionUpdate(TObject *Sender);
	void __fastcall RegSyncDlgActionExecute(TObject *Sender);
	void __fastcall CalcDirSizeAllActionExecute(TObject *Sender);
	void __fastcall ZoomInActionExecute(TObject *Sender);
	void __fastcall ZoomOutActionExecute(TObject *Sender);
	void __fastcall ZoomResetActionExecute(TObject *Sender);
	void __fastcall ForwardDirHistActionExecute(TObject *Sender);
	void __fastcall ForwardDirHistActionUpdate(TObject *Sender);
	void __fastcall AlphaBlendActionExecute(TObject *Sender);
	void __fastcall LoadBgImageActionExecute(TObject *Sender);
	void __fastcall BgImgModeActionExecute(TObject *Sender);
	void __fastcall LockTextPreviewActionExecute(TObject *Sender);
	void __fastcall LockTextPreviewActionUpdate(TObject *Sender);
	void __fastcall LockTxtPanelDblClick(TObject *Sender);
	void __fastcall DotNyanDlgActionExecute(TObject *Sender);
	void __fastcall ItemTmpDownActionExecute(TObject *Sender);
	void __fastcall ItemTmpUpActionExecute(TObject *Sender);
	void __fastcall CopyCmdNameActionExecute(TObject *Sender);
	void __fastcall AppListActionExecute(TObject *Sender);
	void __fastcall AppListActionUpdate(TObject *Sender);
	void __fastcall DistributionDlgActionExecute(TObject *Sender);
	void __fastcall UndoRenameActionExecute(TObject *Sender);
	void __fastcall SetArcTimeActionExecute(TObject *Sender);
	void __fastcall SetDirTimeActionExecute(TObject *Sender);
	void __fastcall PlayListActionExecute(TObject *Sender);
	void __fastcall ExtractGifBmpActionExecute(TObject *Sender);
	void __fastcall CreateDirsDlgActionExecute(TObject *Sender);
	void __fastcall FixedLenActionExecute(TObject *Sender);
	void __fastcall FixedLenActionUpdate(TObject *Sender);
	void __fastcall CanDlBtnClick(TObject *Sender);
	void __fastcall TabControl1DrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall TabControl1Changing(TObject *Sender, bool &AllowChange);
	void __fastcall TabControl1Change(TObject *Sender);
	void __fastcall TabControl1Enter(TObject *Sender);
	void __fastcall TabControl1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall TabControl1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TabControl1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall TabControl1MouseLeave(TObject *Sender);
	void __fastcall AddTabActionExecute(TObject *Sender);
	void __fastcall AddTabActionUpdate(TObject *Sender);
	void __fastcall DelTabActionExecute(TObject *Sender);
	void __fastcall ChgTabActionUpdate(TObject *Sender);
	void __fastcall MoveTabActionExecute(TObject *Sender);
	void __fastcall PrevTabActionExecute(TObject *Sender);
	void __fastcall NextTabActionExecute(TObject *Sender);
	void __fastcall ToTabActionExecute(TObject *Sender);
	void __fastcall ShowTabBarActionExecute(TObject *Sender);
	void __fastcall ShowTabBarActionUpdate(TObject *Sender);
	void __fastcall SoloTabActionExecute(TObject *Sender);
	void __fastcall PopupTabActionExecute(TObject *Sender);
	void __fastcall PopupTabActionUpdate(TObject *Sender);
	void __fastcall PopTabBtnClick(TObject *Sender);
	void __fastcall DelTabBtnClick(TObject *Sender);
	void __fastcall DelTabBtnMouseLeave(TObject *Sender);
	void __fastcall TabHomeActionExecute(TObject *Sender);
	void __fastcall TabDlgActionExecute(TObject *Sender);
	void __fastcall ToolBarDlgActionUpdate(TObject *Sender);
	void __fastcall EditIniFileActionUpdate(TObject *Sender);
	void __fastcall RegSyncDlgActionUpdate(TObject *Sender);
	void __fastcall DuplicateActionExecute(TObject *Sender);
	void __fastcall DuplicateActionUpdate(TObject *Sender);
	void __fastcall ExitDuplActionExecute(TObject *Sender);
	void __fastcall ExitDuplActionUpdate(TObject *Sender);
	void __fastcall NextNyanFiActionExecute(TObject *Sender);
	void __fastcall ChgNyanFiActionUpdate(TObject *Sender);
	void __fastcall PrevNyanFiActionExecute(TObject *Sender);
	void __fastcall CopyDirActionExecute(TObject *Sender);
	void __fastcall ListClipboardActionExecute(TObject *Sender);
	void __fastcall ToPrevOnLeftActionExecute(TObject *Sender);
	void __fastcall ToNextOnRightActionExecute(TObject *Sender);
	void __fastcall ViewTailActionExecute(TObject *Sender);
	void __fastcall ListTailActionExecute(TObject *Sender);
	void __fastcall WatchTailActionExecute(TObject *Sender);
	void __fastcall GrepTrimTopActionExecute(TObject *Sender);
	void __fastcall GrepTrimTopActionUpdate(TObject *Sender);
	void __fastcall TrayIcon1Click(TObject *Sender);
	void __fastcall ApplicationEvents1Minimize(TObject *Sender);
	void __fastcall TrayPopupMenuPopup(TObject *Sender);
	void __fastcall ShowNyanFiClick(TObject *Sender);
	void __fastcall TxtSttHeaderDrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall Log_EditSelectAllUpdate(TObject *Sender);
	void __fastcall LibraryActionExecute(TObject *Sender);
	void __fastcall LibraryItemClick(TObject *Sender);
	void __fastcall IdFTP1Disconnected(TObject *Sender);
	void __fastcall IdFTP1Work(TObject *ASender, TWorkMode AWorkMode, __int64 AWorkCount);
	void __fastcall IdFTP1BannerAfterLogin(TObject *ASender, const UnicodeString AMsg);
	void __fastcall IdFTP1Status(TObject *ASender, const TIdStatus AStatus, const UnicodeString AStatusText);
	void __fastcall FTPConnectActionExecute(TObject *Sender);
	void __fastcall FTPDisconnectActionExecute(TObject *Sender);
	void __fastcall FTPDisconnectActionUpdate(TObject *Sender);
	void __fastcall FTPChmodActionExecute(TObject *Sender);
	void __fastcall FTPChmodActionUpdate(TObject *Sender);
	void __fastcall CopyToClipActionExecute(TObject *Sender);
	void __fastcall CutToClipActionExecute(TObject *Sender);
	void __fastcall ShowTABActionExecute(TObject *Sender);
	void __fastcall ShowTABActionUpdate(TObject *Sender);
	void __fastcall ShowCRActionExecute(TObject *Sender);
	void __fastcall ShowCRActionUpdate(TObject *Sender);
	void __fastcall ConvertHtm2TxtActionExecute(TObject *Sender);
	void __fastcall TabBottomPaintBoxPaint(TObject *Sender);
	void __fastcall PopupMainMenuActionExecute(TObject *Sender);
	void __fastcall TaskPaintBoxPaint(TObject *Sender);
	void __fastcall TaskPaintBoxDblClick(TObject *Sender);
	void __fastcall TaskPaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FlScrPanelMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SubListBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall Txt_OpenUrlActionExecute(TObject *Sender);
	void __fastcall Txt_OpenUrlActionUpdate(TObject *Sender);
	void __fastcall XmlViewerActionExecute(TObject *Sender);
	void __fastcall SetFontSizeActionExecute(TObject *Sender);
	void __fastcall GrepOpenUrlActionExecute(TObject *Sender);
	void __fastcall GrepOpenUrlActionUpdate(TObject *Sender);
	void __fastcall TxtPrvSplitterMoved(TObject *Sender);
	void __fastcall TxtTailListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall HelpHistoryItemClick(TObject *Sender);
	void __fastcall CountLinesActionExecute(TObject *Sender);
	void __fastcall GrepConfirmActionExecute(TObject *Sender);
	void __fastcall GrepConfirmActionUpdate(TObject *Sender);
	void __fastcall GrepShowItemNoActionExecute(TObject *Sender);
	void __fastcall GrepShowItemNoActionUpdate(TObject *Sender);
	void __fastcall ThumbnailGridMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ThumbnailGridMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall GrepSelResActionExecute(TObject *Sender);
	void __fastcall GrepSelResActionUpdate(TObject *Sender);
	void __fastcall DotNyanDlgActionUpdate(TObject *Sender);
	void __fastcall ShareListActionExecute(TObject *Sender);
	void __fastcall HighlightActionExecute(TObject *Sender);
	void __fastcall HighlightActionUpdate(TObject *Sender);
	void __fastcall GrepOmitTopActionExecute(TObject *Sender);
	void __fastcall GrepOmitTopActionUpdate(TObject *Sender);
	void __fastcall GrepFileItemNoActionExecute(TObject *Sender);
	void __fastcall GrepFileItemNoActionUpdate(TObject *Sender);
	void __fastcall OpenTrashActionExecute(TObject *Sender);
	void __fastcall GrepEmFilterActionUpdate(TObject *Sender);
	void __fastcall GrepEmFilterActionExecute(TObject *Sender);
	void __fastcall HideSizeTimeActionExecute(TObject *Sender);
	void __fastcall HideSizeTimeActionUpdate(TObject *Sender);
	void __fastcall DeleteVActionExecute(TObject *Sender);
	void __fastcall ToTextActionExecute(TObject *Sender);
	void __fastcall LogListBoxEnter(TObject *Sender);
	void __fastcall Txt_KeepIndexActionExecute(TObject *Sender);
	void __fastcall Txt_KeepIndexActionUpdate(TObject *Sender);
	void __fastcall Txt_ShowTailActionExecute(TObject *Sender);
	void __fastcall Txt_ShowTailActionUpdate(TObject *Sender);
	void __fastcall MenuFIGActionUpdate(TObject *Sender);
	void __fastcall ConvertDoc2TxtActionExecute(TObject *Sender);
	void __fastcall FindTagNameActionExecute(TObject *Sender);
	void __fastcall MuteVolumeActionExecute(TObject *Sender);
	void __fastcall MuteVolumeActionUpdate(TObject *Sender);
	void __fastcall ToExViewerActionExecute(TObject *Sender);
	void __fastcall ApplicationEvents1ModalBegin(TObject *Sender);
	void __fastcall FKeyBtnClick(TObject *Sender);
	void __fastcall ShowFKeyBarActionExecute(TObject *Sender);
	void __fastcall ShowFKeyBarActionUpdate(TObject *Sender);
	void __fastcall FK_RenameActionExecute(TObject *Sender);
	void __fastcall FKeyBtnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall SubViewerActionExecute(TObject *Sender);
	void __fastcall SubViewerActionUpdate(TObject *Sender);
	void __fastcall TV_EditAddActionExecute(TObject *Sender);
	void __fastcall DebugCmdFileActionExecute(TObject *Sender);
	void __fastcall DefHighlightActionExecute(TObject *Sender);
	void __fastcall EditHighlightActionUpdate(TObject *Sender);
	void __fastcall EditHighlightActionExecute(TObject *Sender);
	void __fastcall RegExCheckBoxClick(TObject *Sender);
	void __fastcall RegExRCheckBoxClick(TObject *Sender);
	void __fastcall RegExCheckerActionExecute(TObject *Sender);
	void __fastcall DriveGraphActionExecute(TObject *Sender);
	void __fastcall CreateHardLinkActionExecute(TObject *Sender);
	void __fastcall ExportCsvActionExecute(TObject *Sender);
	void __fastcall ExportCsvActionUpdate(TObject *Sender);
	void __fastcall CalculatorActionExecute(TObject *Sender);
	void __fastcall SubDirListActionExecute(TObject *Sender);
	void __fastcall FindHardLinkActionExecute(TObject *Sender);
	void __fastcall CreateTestFileActionExecute(TObject *Sender);
	void __fastcall Log_DebugInfActionExecute(TObject *Sender);
	void __fastcall Log_DebugInfActionUpdate(TObject *Sender);
	void __fastcall TxtPrvListBoxEnter(TObject *Sender);
	void __fastcall InfListBoxEnter(TObject *Sender);
	void __fastcall ApplicationEvents1ModalEnd(TObject *Sender);
	void __fastcall SetTagActionExecute(TObject *Sender);
	void __fastcall FindTagActionExecute(TObject *Sender);
	void __fastcall AddTagActionExecute(TObject *Sender);
	void __fastcall DelTagActionExecute(TObject *Sender);
	void __fastcall TrimTagDataActionExecute(TObject *Sender);
	void __fastcall CreateSymLinkActionExecute(TObject *Sender);
	void __fastcall CreateJunctionActionExecute(TObject *Sender);
	void __fastcall DiffDirActionExecute(TObject *Sender);
	void __fastcall NewFileActionExecute(TObject *Sender);
	void __fastcall WorkItemMoveActionExecute(TObject *Sender);
	void __fastcall ItemTmpMoveActionExecute(TObject *Sender);
	void __fastcall TagSelectActionExecute(TObject *Sender);
	void __fastcall EditSwatchItemClick(TObject *Sender);
	void __fastcall PreviewImageMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall PreviewImageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall PreviewImageMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall SpecialDirListActionExecute(TObject *Sender);
	void __fastcall CsvCalcActionExecute(TObject *Sender);
	void __fastcall CsvCalcActionUpdate(TObject *Sender);
	void __fastcall CsvGraphActionExecute(TObject *Sender);
	void __fastcall CsvGraphActionUpdate(TObject *Sender);
	void __fastcall EditFontSmplItemClick(TObject *Sender);
	void __fastcall ImgPrvPopupMenuPopup(TObject *Sender);
	void __fastcall PowerShellActionExecute(TObject *Sender);
	void __fastcall RecentListActionExecute(TObject *Sender);
	void __fastcall ShowLogWinActionExecute(TObject *Sender);
	void __fastcall ShowLogWinActionUpdate(TObject *Sender);
	void __fastcall FixTabPathActionExecute(TObject *Sender);
	void __fastcall TabPopupMenuPopup(TObject *Sender);
	void __fastcall DeleteADSActionExecute(TObject *Sender);
	void __fastcall OpenADSActionExecute(TObject *Sender);
	void __fastcall FindDuplDlgActionExecute(TObject *Sender);
	void __fastcall ToOppSameHashActionExecute(TObject *Sender);
	void __fastcall SaveAsResultListActionExecute(TObject *Sender);
	void __fastcall LoadResultListActionExecute(TObject *Sender);
	void __fastcall SaveAsResultListActionUpdate(TObject *Sender);
	void __fastcall SaveAsTabGroupActionExecute(TObject *Sender);
	void __fastcall LoadTabGroupActionExecute(TObject *Sender);
	void __fastcall TabGroupActionUpdate(TObject *Sender);
	void __fastcall SaveTabGroupActionExecute(TObject *Sender);
	void __fastcall CompressDirActionExecute(TObject *Sender);
	void __fastcall ListTreeActionExecute(TObject *Sender);
	void __fastcall GrepRepComboBoxEnter(TObject *Sender);
	void __fastcall TextMarginBoxPaint(TObject *Sender);
	void __fastcall InfPopupMenuPopup(TObject *Sender);
	void __fastcall ShowInfItemClick(TObject *Sender);
	void __fastcall Inf_EditCopyExecute(TObject *Sender);
	void __fastcall Inf_EditCopyUpdate(TObject *Sender);
	void __fastcall Inf_EditSelectAllExecute(TObject *Sender);
	void __fastcall Inf_EditSelectAllUpdate(TObject *Sender);
	void __fastcall Inf_EmpItemActionExecute(TObject *Sender);
	void __fastcall Inf_EmpItemActionUpdate(TObject *Sender);
	void __fastcall Inf_HideItemActionExecute(TObject *Sender);
	void __fastcall Inf_HideItemActionUpdate(TObject *Sender);
	void __fastcall Inf_OpenUrlActionExecute(TObject *Sender);
	void __fastcall Inf_OpenUrlActionUpdate(TObject *Sender);
	void __fastcall OpenGitURLActionExecute(TObject *Sender);
	void __fastcall SelByListActionExecute(TObject *Sender);
	void __fastcall SelGitChangedActionExecute(TObject *Sender);
	void __fastcall SelGitChangedActionUpdate(TObject *Sender);
	void __fastcall SetFolderIconActionExecute(TObject *Sender);
	void __fastcall SetFolderIconActionUpdate(TObject *Sender);
	void __fastcall GitViewerActionExecute(TObject *Sender);
	void __fastcall GitViewerActionUpdate(TObject *Sender);
	void __fastcall Inf_CopyValueActionExecute(TObject *Sender);
	void __fastcall GitDiffActionExecute(TObject *Sender);
	void __fastcall RepositoryListActionUpdate(TObject *Sender);
	void __fastcall RepositoryListActionExecute(TObject *Sender);
	void __fastcall FindFolderIconActionExecute(TObject *Sender);
	void __fastcall FindFolderIconActionUpdate(TObject *Sender);
	void __fastcall SimilarSortActionExecute(TObject *Sender);

private:	// ユーザー宣言
	TIdFTP *IdFTP1;
	TIdAntiFreeze *IdAntiFreeze1;
	TIdSSLIOHandlerSocketOpenSSL *IdSSLIOHandlerSocketOpenSSL1;
	__int64 FTPMaxCount;
	bool InhFTPCheck;					//FTP接続チェックを抑止
	bool FTPhasCHMOD;					//CHMOD コマンドに対応している

	bool ReqClose;						//終了要求
	bool ReqPowerOff;					//シャットダウン要求
	bool ReqReboot;						//再起動要求
	bool ReqKeepDupl;					//二重起動した NyanFi を終了させない
	bool NotSaveINI;					//INIファイルを保存せずに終了

	bool WndSizing;
	int  ViewInfCnt;					//ファイル情報の表示処理中
	int  InhDrawImg;					//画像描画の抑止
	int  InhDirHist;					//ディレクトリ履歴追加の抑止
	int  InhUpdBgImg;					//背景画像更新の抑止
	bool HideBgImg[MAX_FILELIST];
	int  DrawOppCsr;					//反対側カーソルの描画
	int  KeepCurCsr;					//カレント側カーソル表示を維持
	bool KeepModalScr;					//モーダル表示効果を維持
	bool InhModalScr;					//モーダル表示効果を抑止
	bool ApplyDotNyan;					//.nyanfi を適用
	bool ChkHardLink;					//ReloadList でハードリンク更新

	UnicodeString StartMsg;				//起動時メッセージ
	int  StartTag;						//起動時のカレントタグ
	UnicodeString StartFile;			//起動時のカレントファイル
	UnicodeString StartCmds;			//起動時の実行コマンド
	UnicodeString InitialLog;			//起動時の追加ログ

	HANDLE hWatchDir[MAX_FILELIST];
	UnicodeString WatchPath[MAX_FILELIST];	//監視ディレクトリ

	UnicodeString DirRelStr;			//ディレクトリ関係

	bool isViewText;					//テキスト表示
	bool isRichText;					//リッチテキスト表示
	bool isXDoc2Txt;					//xdoc2txt を利用して表示
	bool fromGrep;						//GREPから開いている
	bool isViewWork;					//イメージビュアーでワークリストを表示中
	bool VListMaking;					//ViewFileList 作成中
	bool ImgMoving;						//ビュアーイメージ移動中
	bool TmpEqualSize;					//一時的な等倍表示
	bool ThumbExtended;					//サムネイルの拡張表示
	bool ThumbClicked;					//直前にサムネイルがクリックされている
	bool FinfSkipped;					//ファイル情報取得がスキップされた
	bool isLoopHint;
	TPoint LastP;						//イメージ移動中の前回のマウス位置
	TPoint ButtonPos;					//メニュー表示用ボタン位置
	bool IsEvenPage;					//見開き表示は偶数ページから
	int  InhSeekBar;					//シークバー処理の抑止

	UnicodeString ActionParam;			//コマンドアクションのパラメータ
	UnicodeString ActionDesc;			//コマンドアクションの説明
	bool ActionOk;						//コマンドアクションの実行結果
	UnicodeString ActionErrMsg;			//コマンドアクションのエラーメッセージ
	UnicodeString ActionOptStr;			//コマンドアクションのオプション
	UnicodeString ActionDstDir;

	UnicodeString FirstKey;				//2ストローク開始キー
	bool Wait2ndKey;					//2ストローク目待ち状態
	bool DblClicked;					//ダブルクリックされた
	bool R_Clicked;
	bool RClickAtCur;					//カーソル位置項目が右クリックされた

	TShiftState LastShift;

	bool CancelWork;

	int MoveIndex;
	int MoveTag;
	int MoveCnt;

	bool DragCancel;
	int  LastIndex[MAX_FILELIST];

	int  DragTabIndex;					//ドラッグ対象のタブ
	int  HotTabIndex;					//ドラッグ中のカーソル位置のタブ
	bool TabMouseCliped;				//ドラッグでマウスクリップ中

	bool fromOpenStd;					//OpenStandard から呼ばれている
	bool fromFileList;					//ファイルリストから実行された
	bool fromMenuFile;					//メニューファイルから実行された

	UnicodeString GrepPath;				//GREP 対象パス
	UnicodeString GrepKeyword;			//検索語
	UnicodeString GrepResultMsg;		//検索結果メッセージ
	UnicodeString GrepResultMsgBuf;		//  待避用バッファ
	UnicodeString GrepResultPath;		//検索結果パス
	UnicodeString GrepResultPathBuf;	//  待避用バッファ

	bool GrepCaseSenstive;				//大小文字を区別
	bool GrepFiltered;					//結果絞り込み中
	bool GrepWorkList;					//対象がワークリスト

	bool fromViewer;					//テキストビュアーから
	bool WarnBlink;						//白飛び警告の点滅フラグ

	SttProgressBar *SttPrgBar;			//Grep用プログレスバー
	UsrHintWindow *MsgHint;				//メッセージ、警告のヒント表示ウィンドウ
	UsrHintWindow *KeyHint;				//2ストローク操作のヒント表示ウィンドウ

	int TabPinWidth;
	int PopMenuIndex;
	int PrevListIdx;

	TWindowState LastWinState;
	TWindowState OrgWinState;
	TRect OrgWinRect;

	bool LastConnect;

	UnicodeString LastBatteryStt;
	int LastBatteryLife;

	TStringList *TxtPrvBuff;			//テキストプレビュー用バッファ
	TStringList *TxtTailBuff;			//テキストプレビュー用バッファ
	UnicodeString TxtPrvFile;			//テキストプレビュー中のファイル名
	bool TxtPrvShowLineNo;				//テキストプレビューで行番号表示
	bool TxtPrvKeepIndex;				//テキストプレビューでカーソル行を維持
	bool LockTxtPrv;					//テキストプレビューをロック

	UnicodeString FTPHostItem;			//ホスト設定項目
	UnicodeString TopFTPPath;			//FTP開始ディレクトリ
	UnicodeString CurFTPPath;			//FTP接続中のカレントパス
	bool FTPTryModTime;					//SetModTimeを試す
	int  FTPLastWorkCnt;
	int  FTPLastNoopCnt;

	void __fastcall ActivateMainForm();

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall WmQueryEndSession(TMessage &msg);

	void __fastcall WmSysCommand(TWMSysCommand & SysCom)
	{
		if (SysCom.CmdType==SC_RESTORE && StoreTaskTray) Show();
		TForm::Dispatch(&SysCom);
	}

	void __fastcall WmFormMoving(TMessage &msg)
	{
		if (IsPrimary && IniWinMode==1 && FixWinPos) *((TRect*)msg.LParam) = BoundsRect;
	}

	void __fastcall WmEnterSizeMove(TMessage &msg)
	{
		WndSizing = true;
	}

	void __fastcall WmDropped(TMessage &msg);
	void __fastcall WmSettingChange(TMessage &msg);
	void __fastcall WmDeviceChange(TMessage &msg);
	void __fastcall WmExitSizeMove(TMessage &msg);
	void __fastcall WmGetMinMaxInfo(TWMGetMinMaxInfo &msg);
	void __fastcall WmCopyData(TMessage &msg);
	void __fastcall WmHotKey(TMessage &msg);
	void __fastcall WmMenuChar(TMessage &msg);
	void __fastcall WmContextMnueProc(TMessage &msg);
	void __fastcall MmMciNotify(TMessage &msg);
	void __fastcall WmNyanfiAppearance(TMessage &msg);

	void __fastcall WmNyanFiFlIcon(TMessage &msg)
	{
		if (ScrMode==SCMD_FLIST) {
			FileListBox[0]->Invalidate();
			FileListBox[1]->Invalidate();
		}
	}

	void __fastcall WmNyanFiThumbnail(TMessage &msg) { if (ThumbnailGrid->Visible) ThumbnailGrid->Repaint(); }

	void __fastcall WmNyanFiClpCopied(TMessage &msg);
	void __fastcall WmActivate(TMessage &msg);

	void __fastcall ActiveFormChange(TObject *Sender);

	//タブ
	TWndMethod org_TabCtrlWindowProc;
	void __fastcall TabCtrlWindowProc(TMessage &msg);

	TWndMethod org_TabPanelWndProc;
	void __fastcall TabPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_TabPanelWndProc(msg);
	}

	int __fastcall get_TopTabIndex();
	int __fastcall get_EndTabIndex();

	//ディレクトリ関係
	TWndMethod org_RelPanelWndProc;
	void __fastcall RelPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_RelPanelWndProc(msg);
	}

	TWndMethod org_RelPanel2WndProc;
	void __fastcall RelPanel2WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_RelPanel2WndProc(msg);
	}

	//ディレクトリ情報
	void __fastcall DrawDirPanel(TPanel *pp);
	TWndMethod org_L_DirPanelWndProc;
	void __fastcall L_DirPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_L_DirPanelWndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDirPanel(L_DirPanel);
	}
	TWndMethod org_L_DirPanel2WndProc;
	void __fastcall L_DirPanel2WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_L_DirPanel2WndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDirPanel(L_DirPanel2);
	}
	TWndMethod org_R_DirPanelWndProc;
	void __fastcall R_DirPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_R_DirPanelWndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDirPanel(R_DirPanel);
	}
	TWndMethod org_R_DirPanel2WndProc;
	void __fastcall R_DirPanel2WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_R_DirPanel2WndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDirPanel(R_DirPanel2);
	}

	//ドライブ情報
	void __fastcall DrawDrivePanel(TPanel *pp);
	TWndMethod org_L_StatPanelWndProc;
	void __fastcall L_StatPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_L_StatPanelWndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDrivePanel(L_StatPanel);
	}
	TWndMethod org_R_StatPanelWndProc;
	void __fastcall R_StatPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_R_StatPanelWndProc(msg);
		if (msg.Msg==WM_PAINT) DrawDrivePanel(R_StatPanel);
	}

	TWndMethod org_TvViewPanelWndProc;
	void __fastcall TvViewPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_TvViewPanelWndProc(msg);
	}

	TWndMethod org_TvScrlPanelWndProc;
	void __fastcall TvScrlPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_TvScrlPanelWndProc(msg);
	}

	TWndMethod org_LogPanelWndProc;
	void __fastcall LogPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_LogPanelWndProc(msg);
	}

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	TWndMethod org_ClockBarWndProc;
	void __fastcall ClockBarWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(ClockBar, msg)) return;
		org_ClockBarWndProc(msg);
	}

	TWndMethod org_ClockBarIWndProc;
	void __fastcall ClockBarIWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(ClockBarI, msg)) return;
		org_ClockBarIWndProc(msg);
	}

	TWndMethod org_TxtSttHdrWndProc;
	void __fastcall TxtSttHdrWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(TxtSttHeader, msg)) return;
		org_TxtSttHdrWndProc(msg);
	}

	TWndMethod org_ImgSttHdrWndProc;
	void __fastcall ImgSttHdrWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(ImgSttHeader, msg)) return;
		org_ImgSttHdrWndProc(msg);
	}

	TWndMethod org_ImgInfBarWndProc;
	void __fastcall ImgInfBarWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_InfHdrBg(ImgInfBar, msg)) return;
		org_ImgInfBarWndProc(msg);
	}

	TWndMethod org_FileListWindowProc[MAX_FILELIST];
	void __fastcall ListWndProc(TMessage &msg, int tag);
	void __fastcall L_ListWindowProc(TMessage &msg)
	{
		ListWndProc(msg, 0);
	}
	void __fastcall R_ListWindowProc(TMessage &msg)
	{
		ListWndProc(msg, 1);
	}

	bool __fastcall UpdateBgImage(bool repaint_sw = false, bool reload_sw = false);
	void __fastcall RestoreBgImg();

	void __fastcall SetupFont();
	void __fastcall SetSubLayout(bool initial = false);
	void __fastcall SetupThumbnail(int idx = -1);
	void __fastcall ResetIndColor(int tag);
	void __fastcall SetupDesign(bool chg_layout = true, bool initial = false);
	void __fastcall SetupToolBarColor(bool act_sw = true, bool bg_only = false);
	void __fastcall SetFileListFontSize(int sz = 0, bool r_sw = false, bool x_sw = false);

	void __fastcall UpdateFileListRect();

	void __fastcall SetExtMenuItem(TMenuItem *m_item, TStringList *lst, int tag_base, int s_idx = -1);
	void __fastcall ExePopMenuList(TStringList *lst, bool is_execmd = false, TControl *cp = NULL);
	UnicodeString __fastcall get_MenuItemStr(TStringDynArray menu_lst);
	void __fastcall ExeCmdsMenuClick(TObject *Sender);

	bool __fastcall SureOtherActiv();

	void __fastcall DockSideCtrl(TControl *cp, TPanel *pp, bool is_top);

	void __fastcall SetCurStt(int tag, bool redraw = false);
	UnicodeString __fastcall GetCurPathStr(int tag = CurListTag);
	UnicodeString __fastcall GetSrcPathStr();
	file_rec * __fastcall GetFrecPtr(TListBox *lp, TStringList *lst);
	file_rec * __fastcall GetOppFrecPtr();
	UnicodeString __fastcall GetCurFileName();
	int  __fastcall SetCurFrecPtr(file_rec *fp);
	int  __fastcall GetCurIndex();
	bool __fastcall SetCurIndex(int idx);

	UnicodeString __fastcall GetModeIdStr()
	{
		return (ScrMode==SCMD_FLIST)? "F" : (ScrMode==SCMD_TVIEW)? "V" : (ScrMode==SCMD_IVIEW)? "I" : "";
	}

	bool __fastcall IsCurFList()
	{
		return (!CurStt->is_Arc && !CurStt->is_ADS && !CurStt->is_Find && !CurStt->is_Work && !CurStt->is_FTP);
	}
	bool __fastcall IsOppFList()
	{
		return (!OppStt->is_Arc && !OppStt->is_ADS && !OppStt->is_Find && !OppStt->is_Work && !OppStt->is_FTP);
	}
	bool __fastcall EqualDirLR()
	{
		return SameText(CurPath[CurListTag], CurPath[OppListTag]);
	}
	bool __fastcall IsDiffList()
	{
		return (CurStt->is_Find && OppStt->is_Find);
	}

	TListBox * __fastcall GetCurInfListBox()
	{
		return ((ScrMode==SCMD_IVIEW)? ImgInfListBox : InfListBox);
	}

	UnicodeString __fastcall GetCurInfFext(UnicodeString fnam)
	{
		return (dir_exists(fnam) || StartsStr('<', fnam)) ? UnicodeString("\\") 
														  : def_if_empty(get_extension(fnam), ".");
	}

	void __fastcall ResetIncSeaFilter(int tag, bool set_listbox = false);
	void __fastcall SaveToTmpBufList();
	void __fastcall CurToOppDiffList();
	void __fastcall ExitIncSearch();

	file_rec * __fastcall GetCurRepFrecPtr();
	file_rec * __fastcall GetFrecPtrFromViewList(int idx, UnicodeString fnam = EmptyStr);
	TControl * __fastcall GetCurControl(bool center = false);

	void __fastcall SetDirWatch(UnicodeString dnam, int tag);
	void __fastcall SetDirWatch(bool active);

	void __fastcall StoreTabStt(int idx);

	bool __fastcall IncProtectItem();
	bool __fastcall TestCurIncDir(bool only_cur = false);
	bool __fastcall TestCurIncNotDir();
	bool __fastcall TestCurIncFindVirtual();
	void __fastcall NotConvertAbort();
	void __fastcall ApplySelMask(TStringList *lst, int tag, UnicodeString dnam = EmptyStr);
	void __fastcall ApplyPathMask(TStringList *lst, int tag);
	void __fastcall UpdateList(TStringList *lst, UnicodeString dnam, int tag = CurListTag);
	void __fastcall ReloadList(int tag = -1, UnicodeString fnam = EmptyStr);
	bool __fastcall JumpToList(int tag, UnicodeString fnam);
	bool __fastcall JumpToArcR(int tag, UnicodeString fnam);
	void __fastcall SetDirRelation();
	void __fastcall SetDirCaption(int tag);
	void __fastcall SetDrivePanel(int tag, UnicodeString msg = EmptyStr);
	UnicodeString __fastcall GetDriveInfo(int tag, bool drv_upd = true, bool sel_upd = true, UnicodeString get_str = EmptyStr);
	void __fastcall SetDriveInfo(int tag = CurListTag, bool drv_upd = true, bool sel_upd = true);
	void __fastcall SetFileInf();
	void __fastcall SetDriveFileInfo(int tag, bool drv_upd = true, bool sel_upd = true);

	bool __fastcall SetTxtPreview(UnicodeString fnam, UnicodeString text, UnicodeString tail = EmptyStr,
						int idx = 0, bool force_sw = false);
	void __fastcall SetListHeader(int tag);
	void __fastcall RepaintList(int tag = -1, UnicodeString cur_fnam = EmptyStr, bool sort_sw = false);
	UnicodeString __fastcall GetCurFileStr(bool inc_dir = false);
	UnicodeString __fastcall GetOppFileStr(bool inc_dir = false);
	int  __fastcall GetCurObjList(TStringList *flst);
	void __fastcall SplitMasksFD(UnicodeString masks, TStringList *f_msk, TStringList *d_msk, bool sel_sw = false);
	int  __fastcall SelectMask(TStringList *lst, UnicodeString masks);
	int  __fastcall set_IncSeaStt(bool sel_sw = false, bool smsk_sw = false);
	void __fastcall FileListIncSearch(UnicodeString keystr);
	void __fastcall SetFlItemWidth(TStringList *lst, int tag);

	void __fastcall SttWorkMsg(UnicodeString msg, int tag);
	void __fastcall SttWorkMsg(const _TCHAR *msg, int tag);
	void __fastcall SttBarWarn(UnicodeString msg = EmptyStr);
	void __fastcall SttBarWarn(const _TCHAR *msg);
	void __fastcall SttBarWarnUstr(unsigned id);

	void __fastcall DeleteSelFiles(TStringList *lst);
	void __fastcall DeleteFileP(file_rec *fp);

	bool __fastcall UnpackCopyCore(UnicodeString anam, UnicodeString snam, UnicodeString files, UnicodeString rnam,
						UnicodeString dst_dir, TStringList *f_lst, TStringList *d_lst);

	bool __fastcall CopyAdsCore(UnicodeString src_nam, UnicodeString dst_nam, bool &ow_all, bool &sk_all);

	void __fastcall CreateLinkCore(UnicodeString lnk_type);

	void __fastcall ViewFileInf(file_rec *fp, bool force = false);
	void __fastcall ViewCurFileInf();

	bool __fastcall OpenTxtViewer(file_rec *fp, bool bin_mode = false, int code_page = 0, int lno = 0, bool force_txt = false);
	bool __fastcall SetAndOpenTxtViewer(UnicodeString fnam, int lno = 0, bool pop_dir = false);
	bool __fastcall OpenTxtViewerTail(file_rec *fp, int limit_ln, bool reverse);
	bool __fastcall OpenTxtViewerTail(UnicodeString fnam, int limit_ln, bool reverse);
	void __fastcall SetViewFileList(bool clr_thumb = true, bool draw_img = false);
	void __fastcall WaitForImgReady(bool reload = false);
	bool __fastcall OpenImgViewer(file_rec *fp, bool fitted = false, int zoom = 0);
	bool __fastcall OpenImgViewer(int idx);
	bool __fastcall ViewClipImage(bool fitted = false, int zoom = 0);
	void __fastcall SetImgInfListBox(file_rec *fp);
	void __fastcall ClearViewImage();
	void __fastcall SetViewFileIdx();
	int  __fastcall FindFileCore(bool dir_sw = false, int tag = -1);
	void __fastcall FindFileDlgExecute(bool both = false);
	int  __fastcall FindHardLinkCore(UnicodeString fnam, int tag);
	int  __fastcall FindMarkCore(int tag = -1);
	int  __fastcall FindTagCore(int tag = -1);
	int  __fastcall FindFolderIconCore(int tag);
	int  __fastcall ChangeWorkList(int tag, bool check = true);
	bool __fastcall SetWorkList(UnicodeString fnam = EmptyStr, bool load_only = false);
	void __fastcall CheckChangeWorkList(int tag);

	void __fastcall AssignFileList(TStringList *lst, int tag,
		UnicodeString last_nam = EmptyStr, int last_idx = -1, int last_top = -1);

	bool __fastcall ChangeArcFileList(UnicodeString anam, UnicodeString dnam, int tag, UnicodeString last_nam = EmptyStr);

#if !defined(_WIN64)
	bool __fastcall ChangeSpiArcList(UnicodeString anam, UnicodeString dnam, int tag, UnicodeString last_nam = EmptyStr);
#endif

	bool __fastcall ChangeArcFileListEx(UnicodeString anam, UnicodeString dnam, int tag, UnicodeString last_nam = EmptyStr);
	bool __fastcall ChangeAdsList(UnicodeString fnam, int tag);
	bool __fastcall ChangeFtpFileList(int tag = CurListTag, UnicodeString dnam = EmptyStr, UnicodeString last_nam = EmptyStr);

	void __fastcall AddDirHistory(UnicodeString dnam, int tag);
	void __fastcall CheckDirHistory(int tag = CurListTag, UnicodeString drv = EmptyStr);
	void __fastcall CaseDirHistory(TStringList *lst);
	void __fastcall MoveDirHistCore(bool is_back);

	bool __fastcall ExeCmdAction(TAction *ap, UnicodeString prm = EmptyStr);
	bool __fastcall ExeListBoxCommandV(UnicodeString cmd, HWND hWnd);

	void __fastcall ShowExPopupMenu(TPoint p);
	void __fastcall ShowExPopupMenu();

	bool __fastcall TestActionParam(const _TCHAR *prm);
	bool __fastcall TestDelActionParam(const _TCHAR *prm);
	bool __fastcall ActionOptIsMousePos()	{ return USAME_TI(ActionOptStr, "MousePos"); }
	bool __fastcall ActionOptIsButtonPos()	{ return USAME_TI(ActionOptStr, "ButtonPos"); }
	bool __fastcall SetToggleAction(bool &sw);
	UnicodeString __fastcall FormatParam(UnicodeString fmt);
	void __fastcall ClipSaveList(TStringList *lst, const _TCHAR *tit);
	void __fastcall ClipSaveList(TStringList *lst, const _TCHAR *tit, bool sw_cc, bool sw_ls, UnicodeString fnam);

	void __fastcall SetActionAbort(UnicodeString msg = EmptyStr);
	void __fastcall SetActionAbort(const _TCHAR *msg);
	void __fastcall SetActionAbort(unsigned id);

	void __fastcall ActionAbort();
	void __fastcall GlobalAbort();
	void __fastcall SetInternalException(UnicodeString cmd = EmptyStr, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandAction(UnicodeString cmd, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandV(UnicodeString cmd, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandV(const _TCHAR *cmd);
	void __fastcall DirectTagJumpCore(bool is_edit = false, UnicodeString prm = EmptyStr,
		UnicodeString dnam = EmptyStr, UnicodeString onam = EmptyStr);
	bool __fastcall ExeCommandI(UnicodeString cmd, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandL(UnicodeString cmd, UnicodeString prm = EmptyStr);
	bool __fastcall ExeCommandsCore(UnicodeString cmds, UnicodeString f_name = EmptyStr, bool debug_sw = false);
	bool __fastcall ExeAliasOrCommands(UnicodeString cmds);
	void __fastcall ExeEventCommand(UnicodeString cmds, UnicodeString opt = EmptyStr, UnicodeString fnam = EmptyStr);
	void __fastcall ExeEventCommandMP(UnicodeString cmds);
	UnicodeString __fastcall FormatExtTool(UnicodeString prm);
	bool __fastcall ExeExtMenuItem(int idx);
	bool __fastcall ExeExtToolItem(int idx);
	bool __fastcall ExeExtTool(UnicodeString cmd, UnicodeString prm, UnicodeString wdir);
	bool __fastcall ExeAlias(UnicodeString alias);
	void __fastcall CancelKeySeq();
	UnicodeString __fastcall TwoStrokeSeq(WORD &Key, TShiftState Shift);
	TPoint __fastcall CurListItemPos();
	void __fastcall SetSttBarGrepDir(UnicodeString dnam = EmptyStr);
	void __fastcall SetSttBarGrepOpt();
	void __fastcall PrepareGrep();
	UnicodeString __fastcall MakeGrepOutLine(int idx, bool rep_log = false);
	void __fastcall MakeGrepOutList(TStringList *lst, bool rep_log = false);
	void __fastcall CalcDirSizeCore(file_rec *fp, TStringList *r_lst = NULL,
		__int64 *total_sz = NULL, __int64 *total_c_sz = NULL, int *total_f_cnt = NULL, int *total_d_cnt = NULL);
	void __fastcall GrepExtractCore(bool except);
	void __fastcall LogFileInfoCore(file_rec *fp);
	bool __fastcall NameToUpLowCore(bool upper);
	void __fastcall ToOppItemCore(const _TCHAR *mode_str);

	UnicodeString __fastcall GetCopyFileNames(UnicodeString fmt, bool all_files = false,
		file_rec *cfp = NULL, TStringList *r_lst = NULL);
	UnicodeString __fastcall GetDistDir(bool is_move, bool *to_flag);

	void __fastcall MakeCloneTaskList(TStringList *src_lst, UnicodeString dst_path, UnicodeString fmt,
		bool force, TStringList *tsk_lst);
	UnicodeString __fastcall ApplyTemplate(UnicodeString tmplt, UnicodeString fnam, TStringList *txtbuf, int idx, int code_page);

	void __fastcall ListTextCore(bool is_tail);

	bool __fastcall UpdateFromArc(UnicodeString arc_name, bool is_dl = false);

	void __fastcall ShowPreviewSize(int wd, int hi);

	void __fastcall ShowInpDirPanel(bool drop_sw = false);
	void __fastcall HideInpDirPanel();
	void __fastcall ApplyInpDir();
	bool __fastcall PopupDriveMenu(int tag = CurListTag, bool is_eject = false, bool no_size = false);
	void __fastcall PopupRegDirMenu(const _TCHAR *id_str);
	void __fastcall PopupTabMenu();

	void __fastcall SetTabStr(int idx, bool add = false);
	void __fastcall UpdateTabBar(int idx = -1, bool force = false);
	void __fastcall SetCurTab(bool redraw = false);

	void __fastcall UpdateToolBtn(int scr_mode = -1);
	void __fastcall UpdateToolDriveBtn();
	
	void __fastcall InitFKeyBtn();
	void __fastcall UpdateFKeyBtn(TShiftState shift);
	void __fastcall UpdateFKeyBtn();

	bool __fastcall IsDoubleStep();

	void __fastcall BeginWorkProgress(UnicodeString tit, UnicodeString s = EmptyStr, TControl *cp = NULL, bool can_sw = false);
	void __fastcall BeginWorkProgress(const _TCHAR *tit, UnicodeString s = EmptyStr, TControl *cp = NULL, bool can_sw = false);
	void __fastcall EndWorkProgress(UnicodeString tit = EmptyStr, UnicodeString s = EmptyStr, int wait = 500);
	void __fastcall PosWorkProgress(int idx, int cnt);
	void __fastcall PosWorkProgress(__int64 idx, __int64 cnt);
	TModalResult __fastcall DownloadWorkProgress(UnicodeString url, UnicodeString fnam, TControl *cp = NULL);

	void __fastcall AssignToMenuItem(TMenuItem *m_item, TMenuItem *src_m);

	void __fastcall SetTags(UnicodeString tags, bool sw_add = false);

	void __fastcall AddInitialLog(UnicodeString msg)
	{
		InitialLog.cat_sprintf(_T("%s\r\n"), msg.c_str());
	}

	void __fastcall RecycleFTP();
	void __fastcall DisconnectFTP();
	void __fastcall set_CurFTPPath();
	void __fastcall add_FTPLogMsg(UnicodeString msg = EmptyStr);
	UnicodeString __fastcall DownloadFtpCore(file_rec *fp, UnicodeString dst_dir = EmptyStr);
	bool __fastcall UploadFtpCore(file_rec *fp);

public:		// ユーザー宣言
	//CurPath プロパティ
	UnicodeString FCurPath[MAX_FILELIST];
	UnicodeString __fastcall GetCurPath(int Index) { return FCurPath[Index]; }
	void __fastcall SetCurPath(int Index, UnicodeString Value);
	__property UnicodeString CurPath[int Index] = {read = GetCurPath, write = SetCurPath};

	//FindBusy プロパティ		検索中(ファイル名、ディレクトリ名、GREP)
	bool FFindBusy;
	void __fastcall SetFindBusy(bool Value);
	__property bool FindBusy = {read = FFindBusy, write = SetFindBusy};

	//CalcBusy プロパティ		計算中(ディレクトリ容量、ファイル再生時間)
	bool FCalcBusy;
	void __fastcall SetCalcBusy(bool Value);
	__property bool CalcBusy = {read = FCalcBusy, write = SetCalcBusy};

	//CurWorking プロパティ		カレントで処理中
	bool FCurWorking;
	void __fastcall SetCurWorking(bool Value);
	__property bool CurWorking = {read = FCurWorking, write = SetCurWorking};

	//ExeCmdsBusy プロパティ	ExeCommands 実行中
	bool FExeCmdsBusy;
	void __fastcall SetExeCmdsBusy(bool Value);
	__property bool ExeCmdsBusy = {read = FExeCmdsBusy, write = SetExeCmdsBusy};

	Graphics::TBitmap *ImgBuff;					//イメージバッファ(元サイズ)
	Graphics::TBitmap *BgBuff[MAX_FILELIST];	//背景表示バッファ

	UsrScrollPanel *FlScrPanel[MAX_FILELIST];	//独自スクロールバー(ファイルリスト)
	UsrScrollPanel *TxtPrvScrPanel;				//独自スクロールバー(テキストプレビュー)
	UsrScrollPanel *TxtTailScrPanel;			//独自スクロールバー(テキストプレビュー(末尾))
	UsrScrollPanel *InfScrPanel;				//独自スクロールバー(ファイル情報)
	UsrScrollPanel *LogScrPanel;				//独自スクロールバー(ログ)
	UsrScrollPanel *ImgInfScrPanel;				//独自スクロールバー(ファイル情報)
	UsrScrollPanel *TxtViewScrPanel;			//独自スクロールバー(テキストビュアー)
	UsrScrollPanel *ResultScrPanel;				//独自スクロールバー(Grep結果)

	int CurListWidth, CurListHeight;
	int PreviewWidth, PreviewHeight;
	int InfPanelWidth, InfPanelHeight;

	__fastcall TNyanFiForm(TComponent* Owner);

	bool __fastcall CheckUncPath(UnicodeString pnam);
	bool __fastcall CheckPath(UnicodeString dnam);

	void __fastcall SetScrMode(int scr_mode = SCMD_FLIST, int tag = -1);
	void __fastcall SetCurPathMask(UnicodeString kstr = EmptyStr);
	void __fastcall SetFullScreen(bool full_sw, bool redraw = true);
	void __fastcall RecoverFileList(int tag = CurListTag);
	void __fastcall RecoverFileList2(int tag = CurListTag);
	void __fastcall UpdateCurPath(UnicodeString dir = EmptyStr, int idx = -1, bool inh_hist = false);
	void __fastcall UpdateCurPath(UnicodeString dir, UnicodeString fnam);
	void __fastcall RefreshCurPath(UnicodeString fnam);
	void __fastcall UpdateOppPath(UnicodeString dir, int idx = -1);
	void __fastcall UpdateCurDrive(UnicodeString drv, int tag = -1);
	void __fastcall OppToParent();

	file_rec * __fastcall GetCurFrecPtr(bool only_filer = false, bool inc_up = false);

	void __fastcall ShowMessageHint(UnicodeString msg = EmptyStr, TColor col = clWindow,
						bool beep = true, bool not_hide = false, bool center = false);
	void __fastcall ShowMessageHint(const _TCHAR *msg, TColor col = clWindow,
						bool beep = true, bool not_hide = false, bool center = false);
	void __fastcall ShowMessageHint(unsigned id, TColor col = clWindow,
						bool beep = true, bool not_hide = false, bool center = false);

	void __fastcall ShowMessageHintEsc(UnicodeString msg);
	void __fastcall ShowHintAndStatus(UnicodeString msg);
	void __fastcall ShowHintAndStatus(unsigned id);

	void __fastcall SetSttBarInf(file_rec *fp = NULL, UnicodeString *stt_txt = NULL);

	bool __fastcall ViewClipText();

	void __fastcall UpdateLoupe();
	bool __fastcall DeleteICore(int idx);
	void __fastcall NextPrevFileICore(bool is_next);

	bool __fastcall DeleteVCore(UnicodeString fnam);

	bool __fastcall PopSelLibrary(UnicodeString prm, int tag = CurListTag, TControl *cp = NULL);

	void __fastcall SetPrvImgCursor(bool sw);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED, 	TMessage,			WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_ACTIVATE,		TMessage,			WmActivate)
		VCL_MESSAGE_HANDLER(WM_QUERYENDSESSION, TMessage,			WmQueryEndSession)
		VCL_MESSAGE_HANDLER(WM_SYSCOMMAND, 		TWMSysCommand, 		WmSysCommand)
		VCL_MESSAGE_HANDLER(WM_FORM_DROPPED,	TMessage,			WmDropped)
		VCL_MESSAGE_HANDLER(WM_SETTINGCHANGE,	TMessage,			WmSettingChange)
		VCL_MESSAGE_HANDLER(WM_DEVICECHANGE, 	TMessage,			WmDeviceChange)
		VCL_MESSAGE_HANDLER(WM_MOVING,			TMessage,			WmFormMoving)
		VCL_MESSAGE_HANDLER(WM_SIZING,			TMessage,			WmFormMoving)
		VCL_MESSAGE_HANDLER(WM_ENTERSIZEMOVE,	TMessage,			WmEnterSizeMove)
		VCL_MESSAGE_HANDLER(WM_EXITSIZEMOVE,	TMessage,			WmExitSizeMove)
		VCL_MESSAGE_HANDLER(WM_GETMINMAXINFO,	TWMGetMinMaxInfo,	WmGetMinMaxInfo)
		VCL_MESSAGE_HANDLER(WM_COPYDATA,		TMessage,			WmCopyData)
		VCL_MESSAGE_HANDLER(WM_HOTKEY,	 		TMessage,			WmHotKey)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,		TMessage,			WmMenuChar)
		VCL_MESSAGE_HANDLER(WM_INITMENUPOPUP,	TMessage,			WmContextMnueProc)
		VCL_MESSAGE_HANDLER(WM_DRAWITEM,		TMessage,			WmContextMnueProc)
		VCL_MESSAGE_HANDLER(WM_MEASUREITEM,		TMessage,			WmContextMnueProc)
		VCL_MESSAGE_HANDLER(MM_MCINOTIFY,		TMessage,			MmMciNotify)
		VCL_MESSAGE_HANDLER(WM_NYANFI_APPEAR,	TMessage,			WmNyanfiAppearance)
		VCL_MESSAGE_HANDLER(WM_NYANFI_FLICON,	TMessage,			WmNyanFiFlIcon)
		VCL_MESSAGE_HANDLER(WM_NYANFI_THUMBNAIL,TMessage,			WmNyanFiThumbnail)
		VCL_MESSAGE_HANDLER(WM_NYANFI_CLPCOPIED,TMessage,			WmNyanFiClpCopied)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TNyanFiForm *NyanFiForm;
//---------------------------------------------------------------------------
#endif
