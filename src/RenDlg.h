//----------------------------------------------------------------------//
// NyanFi																//
//  名前の変更															//
//----------------------------------------------------------------------//
#ifndef RenDlgH
#define RenDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.CheckLst.hpp>
#include "usr_spbar.h"
#include "Global.h"

//---------------------------------------------------------------------------
class TRenameDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddAssRenAction;
	TAction *AddCnvChAction;
	TAction *ChgAssRenAction;
	TAction *ChgCnvChAction;
	TAction *DelAssRenAction;
	TAction *DelCnvChAction;
	TAction *DelItemAction;
	TAction *EditListAction;
	TAction *Inp1stNameAction;
	TAction *LatestTimeAction;
	TAction *RenOkAction;
	TAction *SaveSerFmtAction;
	TActionList *ActionList1;
	TButton *AddAssRenBtn;
	TButton *AddKeyBtn;
	TButton *CanButton;
	TButton *ChgAssRenBtn;
	TButton *ChgKeyBtn;
	TButton *DelAssRenBtn;
	TButton *DelKeyBtn;
	TButton *DowColBtn;
	TButton *EditListBtn;
	TButton *Fmt_A_Btn;
	TButton *Fmt_E_Btn;
	TButton *Fmt_L_Btn;
	TButton *Fmt_R_Btn;
	TButton *Fmt_S_Btn;
	TButton *Fmt_TS_Btn;
	TButton *Fmt_XT_Btn;
	TButton *Id3Btn1;
	TButton *Id3Btn2;
	TButton *Id3Btn3;
	TButton *Id3Btn4;
	TButton *Id3Btn5;
	TButton *OkButton;
	TButton *PreviewBtn;
	TButton *RefListBtn;
	TButton *SaveFmtBtn;
	TButton *Time00Btn;
	TButton *Time12Btn;
	TButton *TimeLatestBtn;
	TButton *TimeNowBtn;
	TButton *TimeX0Btn;
	TButton *UpColBtn;
	TCheckBox *ArcCheckBox;
	TCheckBox *AutoPrvCheckBox;
	TCheckBox *Case2CheckBox;
	TCheckBox *CaseCheckBox;
	TCheckBox *CmpCheckBox;
	TCheckBox *CnvCharCheckBox;
	TCheckBox *CnvKanaCheckBox;
	TCheckBox *EndMatchCheckBox;
	TCheckBox *HiddenCheckBox;
	TCheckBox *KeepBsExtCheckBox;
	TCheckBox *KeepCsrCheckBox;
	TCheckBox *NoRenLogCheckBox;
	TCheckBox *OnlyBase2CheckBox;
	TCheckBox *OnlyBaseCheckBox;
	TCheckBox *PrtChgCheckBox;
	TCheckBox *ReadOnlyCheckBox;
	TCheckBox *RegExCheckBox;
	TCheckBox *SysCheckBox;
	TCheckListBox *AssRenListBox;
	TComboBox *IniStt2ComboBox;
	TComboBox *IniSttComboBox;
	TComboBox *Mp3FmtComboBox;
	TComboBox *RenListComboBox;
	TComboBox *RepStrComboBox;
	TComboBox *SerFmtComboBox;
	TComboBox *SrcStrComboBox;
	TEdit *CnvChREdit;
	TEdit *CnvChSEdit;
	TEdit *ExtEdit;
	TEdit *IncNoEdit;
	TEdit *PostNameEdit;
	TEdit *PreNameEdit;
	TEdit *RenameEdit;
	TEdit *SerNoEdit;
	TGroupBox *AtrGroupBox;
	TGroupBox *CnvCharGroupBox;
	TGroupBox *IniSttGroupBox;
	TGroupBox *TimeGroupBox;
	TLabel *CnvChLabel;
	TLabel *DotLabel;
	TLabel *ListErrLabel;
	TLabel *mp3fextLabel;
	TLabel *NameInfMLabel;
	TLabel *PathInfMLabel;
	TLabel *SttInfLabel;
	TLabeledEdit *AssExtEdit;
	TLabeledEdit *IniSttWdEdit;
	TListBox *CnvCharListBox;
	TMaskEdit *TimeMaskEdit;
	TMenuItem *DelItemItem;
	TPageControl *NamePageControl;
	TPageControl *PageControl1;
	TPanel *ArcPanel;
	TPanel *CmpPanel;
	TPanel *CnvCharPanelL;
	TPanel *CnvCharPanelR;
	TPanel *CommonPanel;
	TPanel *HiddenPanel;
	TPanel *ListTopPanel;
	TPanel *MainPanel;
	TPanel *Mp3TopPanel;
	TPanel *NameComPanel;
	TPanel *NameTopPanel;
	TPanel *Opt1BtmPanel;
	TPanel *Opt1MainPanel;
	TPanel *Opt2MainPanel;
	TPanel *Opt2SubPanel;
	TPanel *ReadOnlyPanel;
	TPanel *ReplaceTopPanel;
	TPanel *SerEditPanel;
	TPanel *SerialTopPanel;
	TPanel *SerLeftPanel;
	TPanel *SerRightPanel;
	TPanel *SysPanel;
	TPopupMenu *FmtPopupMenu;
	TRadioGroup *FbaseRadioGroup;
	TRadioGroup *FextRadioGroup;
	TSplitter *SerSplitter;
	TStatusBar *StatusBar1;
	TStringGrid *PreviewGrid;
	TTabSheet *AssRenSheet;
	TTabSheet *Mp3Sheet;
	TTabSheet *NameSheet;
	TTabSheet *OptionSheet;
	TTabSheet *OtherSheet;
	TTabSheet *RenListSheet;
	TTabSheet *ReplaceSheet;
	TTabSheet *SerialSheet;
	TTimer *Timer1;
	TLabeledEdit *PrvWaitEdit;
	TLabel *Label1;
	TPanel *PreviewPanel;
	TPanel *SglOpPanel;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Opt2MainPanelResize(TObject *Sender);
	void __fastcall RenPageControlDrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall NamePageControlChange(TObject *Sender);
	void __fastcall RenameEditExit(TObject *Sender);
	void __fastcall SrcStrComboBoxEnter(TObject *Sender);
	void __fastcall SrcStrComboBoxExit(TObject *Sender);
	void __fastcall RenameEditChange(TObject *Sender);
	void __fastcall RenameEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RenameEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EtcNameEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RegExCheckBoxClick(TObject *Sender);
	void __fastcall ReplaceComboChange(TObject *Sender);
	void __fastcall Mp3FmtComboBoxChange(TObject *Sender);
	void __fastcall RefFmtBtnClick(TObject *Sender);
	void __fastcall FModRadioGroupClick(TObject *Sender);
	void __fastcall AttrCheckBoxClick(TObject *Sender);
	void __fastcall TimeMaskEditChange(TObject *Sender);
	void __fastcall TimeNowBtnClick(TObject *Sender);
	void __fastcall Time00BtnClick(TObject *Sender);
	void __fastcall Time12BtnClick(TObject *Sender);
	void __fastcall TimeX0BtnClick(TObject *Sender);
	void __fastcall LatestTimeActionExecute(TObject *Sender);
	void __fastcall LatestTimeActionUpdate(TObject *Sender);
	void __fastcall Fmt_L_BtnClick(TObject *Sender);
	void __fastcall Fmt_S_Btn2Click(TObject *Sender);
	void __fastcall Fmt_R_BtnClick(TObject *Sender);
	void __fastcall Fmt_A_BtnClick(TObject *Sender);
	void __fastcall Fmt_E_BtnClick(TObject *Sender);
	void __fastcall Fmt_TS_BtnClick(TObject *Sender);
	void __fastcall Fmt_XT_BtnClick(TObject *Sender);
	void __fastcall SaveSerFmtActionExecute(TObject *Sender);
	void __fastcall SaveSerFmtActionUpdate(TObject *Sender);
	void __fastcall SerFmtComboBoxClick(TObject *Sender);
	void __fastcall DelItemActionExecute(TObject *Sender);
	void __fastcall DelItemActionUpdate(TObject *Sender);
	void __fastcall PreviewGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall PreviewGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CnvCharListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall CnvCharListBoxClick(TObject *Sender);
	void __fastcall CnvChSEditChange(TObject *Sender);
	void __fastcall AddCnvChActionExecute(TObject *Sender);
	void __fastcall AddCnvChActionUpdate(TObject *Sender);
	void __fastcall ChgCnvChActionExecute(TObject *Sender);
	void __fastcall ChgCnvChActionUpdate(TObject *Sender);
	void __fastcall DelCnvChActionExecute(TObject *Sender);
	void __fastcall DelCnvChActionUpdate(TObject *Sender);
	void __fastcall AssRenListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall AssRenListBoxClick(TObject *Sender);
	void __fastcall AssRenListBoxClickCheck(TObject *Sender);
	void __fastcall AssRenListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AddAssRenActionExecute(TObject *Sender);
	void __fastcall AddAssRenActionUpdate(TObject *Sender);
	void __fastcall ChgAssRenActionExecute(TObject *Sender);
	void __fastcall ChgAssRenActionUpdate(TObject *Sender);
	void __fastcall DelAssRenActionExecute(TObject *Sender);
	void __fastcall DelAssRenActionUpdate(TObject *Sender);
	void __fastcall Inp1stNameActionExecute(TObject *Sender);
	void __fastcall Inp1stNameActionUpdate(TObject *Sender);
	void __fastcall IniSttComboBoxClick(TObject *Sender);
	void __fastcall PreviewBtnClick(TObject *Sender);
	void __fastcall AutoPrvCheckBoxClick(TObject *Sender);
	void __fastcall RenOkActionExecute(TObject *Sender);
	void __fastcall RenOkActionUpdate(TObject *Sender);
	void __fastcall RenListComboBoxChange(TObject *Sender);
	void __fastcall RefListBtnClick(TObject *Sender);
	void __fastcall EditListActionExecute(TObject *Sender);
	void __fastcall EditListActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	bool DlgInitialized;
	bool Previewing;
	bool Previewed;
	int  ChangeCount;
	bool ExistErr;
	bool KeyHandled;

	UnicodeString cmnFext;		//共通拡張子
	bool isMp3;					//すべての対象がMP3
	bool isFlac;				//すべての対象がFLAC

	TEdit	  *LastEdit;
	TComboBox *LastComboBox;
	int LastSelStart;
	int LastSelLength;

	TStringList *SerFormatList;	//連番改名書式設定リスト

	SttProgressBar *SttPrgBar;	//プログレスバー

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && IsDarkMode && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}


	void __fastcall WmFormShowed(TMessage &msg);

	void __fastcall WmDpiChanged(TMessage &msg)
	{
		TForm::Dispatch(&msg);
		RefreshDarkMode(this);
	}

	void __fastcall WmMenuChar(TMessage &msg)
	{
		if (msg.WParamHi==MF_POPUP) TForm::Dispatch(&msg); else msg.Result = (MNC_CLOSE << 16);
	}

	void __fastcall UpdateNewNameList();
	void __fastcall UpdatePreview();
	UnicodeString __fastcall ConvCharType(UnicodeString s, int idx);
	void __fastcall ChangeTime(TDateTime dt);

	void __fastcall set_FmtEdit(const _TCHAR *s);
	void __fastcall set_FmtEdit(UnicodeString s);

	bool __fastcall LoadListFile();

	bool __fastcall IsNameSheet()	 { return (NamePageControl->ActivePage==NameSheet); }
	bool __fastcall IsSerialSheet()	 { return (NamePageControl->ActivePage==SerialSheet); }
	bool __fastcall IsRenListSheet() { return (NamePageControl->ActivePage==RenListSheet); }
	bool __fastcall IsReplaceSheet() { return (NamePageControl->ActivePage==ReplaceSheet); }
	bool __fastcall IsMp3Sheet()	 { return (NamePageControl->ActivePage==Mp3Sheet); }
	bool __fastcall IsOptionSheet()	 { return (NamePageControl->ActivePage==OptionSheet); }

public:		// ユーザー宣言
	TStringList *CurNameList;	//カレントの全ファイルリスト
	TStringList *ItemList;		//対象リスト
	TStringList *NewNameList;	//新しい名前のリスト

	UnicodeString OppPath;		//反対パス(カレント側改名による反映のチェック用)

	TStringList *RepRenList;	//改名リスト
	UnicodeString RenListFile;	//改名リストファイル名

	bool EditedList;			//リスト編集による改名
	bool IsMulti;				//複数の対象がある
	bool NameChanged;			//連番改名で名前変更
	bool FExtChanged;			//連番改名で拡張子変更
	bool AttrChanged;			//属性変更
	bool TimeChanged;			//タイムスタンプ変更
	bool KeepCsr;				//単独改名後にカーソルを移動しない

	__fastcall TRenameDlg(TComponent* Owner);

	UnicodeString __fastcall MakeAssRenItem(int idx = -1);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,	TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_DPICHANGED,	TMessage,	WmDpiChanged)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,	TMessage,	WmMenuChar)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TRenameDlg *RenameDlg;
//---------------------------------------------------------------------------
#endif
