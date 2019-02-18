//----------------------------------------------------------------------//
// NyanFi																//
//  �ėp�ꗗ�_�C�A���O													//
//----------------------------------------------------------------------//
#ifndef GenInfDlgH
#define GenInfDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.AppEvnts.hpp>
#include "usr_scrpanel.h"
#include "Global.h"

//---------------------------------------------------------------------------
class TGeneralInfoDlg : public TForm
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *CopyAction;
	TAction *DelDuplAction;
	TAction *EditFileAction;
	TAction *ErrOnlyAction;
	TAction *FileName1stAction;
	TAction *KeepIndexAction;
	TAction *OmitComPathAction;
	TAction *OrgOrderAction;
	TAction *PropertyAction;
	TAction *RestoreListAction;
	TAction *SaveAsAction;
	TAction *SelAllAction;
	TAction *ShowFileInfoAction;
	TAction *ShowLineNoAction;
	TAction *SortAscAction;
	TAction *SortDesAction;
	TAction *ViewFileAction;
	TAction *ViewListAction;
	TActionList *ActionList1;
	TCheckBox *AndOrCheckBox;
	TCheckBox *HighlightCheckBox;
	TCheckBox *MigemoCheckBox;
	TCheckBox *NotifyCheckBox;
	TCheckBox *WatchCheckBox;
	TLabeledEdit *FilterEdit;
	TListBox *GenListBox;
	TMenuItem *DelDuplItem;
	TMenuItem *EditFileItem;
	TMenuItem *ErrOnlyItem;
	TMenuItem *FileName1stItem;
	TMenuItem *FindTextItem;
	TMenuItem *FitSizePosItem;
	TMenuItem *InfEditCopyItem;
	TMenuItem *InfEditSelectAllItem;
	TMenuItem *KeepIndexItem;
	TMenuItem *OmitComPathItem;
	TMenuItem *OrgOrderItem;
	TMenuItem *R2;
	TMenuItem *RestoreListItem;
	TMenuItem *SaveAsItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TMenuItem *Sep_3;
	TMenuItem *Sep_4;
	TMenuItem *Sep_5;
	TMenuItem *ShowFileInfoItem;
	TMenuItem *ShowLineNoItem;
	TMenuItem *SortAscItem;
	TMenuItem *SortDesItem;
	TMenuItem *SortItem;
	TMenuItem *ViewFileItem;
	TMenuItem *ViewListItem;
	TPanel *FilterPanel;
	TPanel *ListPanel;
	TPanel *MianPanel;
	TPanel *OpePanel;
	TPanel *TailPanel;
	TPopupMenu *InfPopupMenu;
	TStatusBar *StatusBar1;
	TTimer *Timer1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GenListBoxData(TWinControl *Control, int Index, UnicodeString &Data);
	void __fastcall GenListBoxDataObject(TWinControl *Control, int Index, TObject *&DataObject);
	void __fastcall GenListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall GenListBoxClick(TObject *Sender);
	void __fastcall GenListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall GenListBoxKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar, TStatusPanel *Panel, const TRect &Rect);
	void __fastcall CopyActionExecute(TObject *Sender);
	void __fastcall CopyActionUpdate(TObject *Sender);
	void __fastcall SelAllActionExecute(TObject *Sender);
	void __fastcall SaveAsActionExecute(TObject *Sender);
	void __fastcall FilterEditChange(TObject *Sender);
	void __fastcall FilterEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FilterEditKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FilterEditEnter(TObject *Sender);
	void __fastcall FilterEditExit(TObject *Sender);
	void __fastcall FilterOptheckBoxClick(TObject *Sender);
	void __fastcall HighlightCheckBoxClick(TObject *Sender);
	void __fastcall ViewListActionExecute(TObject *Sender);
	void __fastcall ShowLineNoActionExecute(TObject *Sender);
	void __fastcall ViewFileActionExecute(TObject *Sender);
	void __fastcall ViewListActionUpdate(TObject *Sender);
	void __fastcall EditFileActionExecute(TObject *Sender);
	void __fastcall OpenFileActionUpdate(TObject *Sender);
	void __fastcall ErrOnlyActionExecute(TObject *Sender);
	void __fastcall ErrOnlyActionUpdate(TObject *Sender);
	void __fastcall GenListBoxDblClick(TObject *Sender);
	void __fastcall GenListBoxExit(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall WatchCheckBoxClick(TObject *Sender);
	void __fastcall FindTextItemClick(TObject *Sender);
	void __fastcall SortAscActionExecute(TObject *Sender);
	void __fastcall ListActionUpdate(TObject *Sender);
	void __fastcall SortDesActionExecute(TObject *Sender);
	void __fastcall OrgOrderActionExecute(TObject *Sender);
	void __fastcall DelDuplActionExecute(TObject *Sender);
	void __fastcall RestoreListActionExecute(TObject *Sender);
	void __fastcall ShowFileInfoActionExecute(TObject *Sender);
	void __fastcall KeepIndexActionExecute(TObject *Sender);
	void __fastcall KeepIndexActionUpdate(TObject *Sender);
	void __fastcall FileName1stActionExecute(TObject *Sender);
	void __fastcall FileName1stActionUpdate(TObject *Sender);
	void __fastcall OmitComPathActionExecute(TObject *Sender);
	void __fastcall OmitComPathActionUpdate(TObject *Sender);
	void __fastcall PropertyActionExecute(TObject *Sender);
	void __fastcall PropertyActionUpdate(TObject *Sender);
	void __fastcall InfPopupMenuPopup(TObject *Sender);

private:	// ���[�U�[�錾
	int  MaxNameWidth;
	int  TabWidth;
	UnicodeString SavePath;
	bool isFiltered;
	int  LastIndex;
	int  LastTopIndex;

	UnicodeString FirstKey;		//2�X�g���[�N�J�n�L�[
	bool Wait2ndKey;			//2�X�g���[�N�ڑ҂����

	__int64   FileSize;
	TDateTime FileTime;

	int  ListWidth;				//���X�g��
	int  SortMode;				//�\�[�g���[�h 1:����/ -1:�~��/ 0:����
	bool Found;					//��������

	void __fastcall WmFormShowed(TMessage &msg);
	void __fastcall WmMenuChar(TMessage &msg);
	void __fastcall WmNyanFiPlayList(TMessage &msg);

	TWndMethod org_GenListWndProc;
	void __fastcall GenListWndProc(TMessage &msg);

	TWndMethod org_SttBar1WndProc;
	void __fastcall SttBar1WndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND && draw_SttBarBg(StatusBar1, msg)) return;
		org_SttBar1WndProc(msg);
	}

	bool __fastcall UpdateList(bool reload = false);
	UnicodeString __fastcall GetBuffText();

	UnicodeString __fastcall TwoStrokeSeq(WORD &Key, TShiftState Shift);
	void __fastcall CancelKeySeq();

	void __fastcall SortGenList(int mode);

public:		// ���[�U�[�錾
	UsrScrollPanel *ListScrPanel;	//�V���v���X�N���[���o�[

	bool isVarList;		//�u���O=�l�v�`��
	bool isLog;			//���O
	bool isGit;			//git �̏o�� (�擪�s�� "$ git ")
	bool isFileList;	//�t�@�C�����ꗗ
	bool isPlayList;	//�v���C���X�g
	bool isDirs;		//�f�B���N�g���ꗗ
	bool isTree;		//�f�B���N�g���c���[�\��
	bool isFTP;			//FTP
	bool isTail;		//�����\��
	bool isReverse;		//�t���\��
	bool ErrOnly;		//�G���[�����̂�(���O)
	bool ToEnd;			//�Ō�Ɉړ�
	bool ToFilter;		//�t�B���^���Ƀt�H�[�J�X

	UnicodeString ComPathName;

	UnicodeString FileName;
	UnicodeString RetStr;
	int  TailLine;				//�����\���s��
	int  CodePage;				//�R�[�h�y�[�W
	bool HasBOM;				//BOM
	UnicodeString LineBreakStr;	//���s�R�[�h
	UnicodeString FindWord;		//����������
	UnicodeString RegExPtn;		//���K�\���p�^�[��
	UnicodeString HdrLnStr;		//���o���s�J�n������(/�`/ �̏ꍇ���K�\��)
	UnicodeString ErrMsg;

	TStringList *GenInfoList;
	TStringList *GenInfoBuff;
	TStringList *GenSelList;

	__fastcall TGeneralInfoDlg(TComponent* Owner);
	void __fastcall SetStatusBar(UnicodeString msg = EmptyStr);

	bool __fastcall FindText(bool down);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_FORM_SHOWED,		TMessage,	WmFormShowed)
		VCL_MESSAGE_HANDLER(WM_MENUCHAR,		TMessage,	WmMenuChar)
		VCL_MESSAGE_HANDLER(WM_NYANFI_PLAYLIST, TMessage,	WmNyanFiPlayList)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGeneralInfoDlg *GeneralInfoDlg;
//---------------------------------------------------------------------------
#endif
