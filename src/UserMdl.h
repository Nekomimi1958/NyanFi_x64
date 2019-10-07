//----------------------------------------------------------------------//
// �ėp���p���W���[��													//
//																		//
//----------------------------------------------------------------------//
#ifndef UserMdlH
#define UserMdlH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <System.ImageList.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.StdActns.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ImgList.hpp>
#include "usr_scrpanel.h"

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̃I�v�V�����^�O
#define LBTAG_OPT_FOCS	0x10000000		//�����A���X�g�{�b�N�X�Ƀt�H�[�J�X
#define LBTAG_OPT_INHI	0x20000000		//�ύX�֎~

//�R���{�{�b�N�X�̃I�v�V�����^�O
#define CBTAG_RGEX_V	0x01000000		//���K�\���Q�Ƃ�\��
#define CBTAG_RGEX_E	0x02000000		//���K�\���Q�Ƃ�L��
#define CBTAG_HISTORY	0x04000000		//����������
#define CBTAG_NO_CALC	0x10000000		//�d���\�����Ȃ�

//�G�f�B�b�g�{�b�N�X�̃I�v�V�����^�O
#define EDTAG_RGEX_V	0x01000000		//���K�\���Q�Ƃ�\��
#define EDTAG_RGEX_E	0x02000000		//���K�\���Q�Ƃ�L��
#define EDTAG_DST_FMT	0x04000000		//�U����̏���������Q��
#define EDTAG_NO_CALC	0x10000000		//�d���\�����Ȃ�

//�t�@�C���_�C�A���O�p�t�B���^
#define F_FILTER_EXE	_T("���s�t�@�C�� (*.exe)|*.EXE")
#define F_FILTER_EXE2	_T("���s�t�@�C�� (*.exe)|*.EXE|�V���[�g�J�b�g (*.lnk)|*.LNK")
#define F_FILTER_NBT	_T("�R�}���h�t�@�C��(*.nbt)|*.nbt")
#define F_FILTER_INI	_T("�ݒ�t�@�C�� (*.ini)|*.INI")
#define F_FILTER_TXT	_T("�e�L�X�g�t�@�C�� (*.txt)|*.txt")
#define F_FILTER_CSV	_T("CSV�t�@�C�� (*.csv)|*.csv")
#define F_FILTER_TSV	_T("TSV�t�@�C�� (*.txt,*.csv,*.tsv)|*.txt;*.csv;*.tsv")
#define F_FILTER_NWL	_T("���[�N���X�g (*.nwl)|*.nwl")
#define F_FILTER_LST	_T("�e�L�X�g�t�@�C�� (*.txt)|*.TXT|���[�N���X�g (*.nwl)|*.nwl")
#define F_FILTER_ICO	_T("(*.ico,*.exe,*.dll,*.lnk)|*.ICO;*.EXE;*.DLL;*.LNK")
#define F_FILTER_WAV	_T("WAV�t�@�C��|*.WAV|���ׂẴt�@�C��(*.*)|*.*")
#define F_FILTER_ALL	_T("���ׂẴt�@�C��(*.*)|*.*")

//�R�[�h�y�[�W
struct code_page_inf {
	unsigned int  page;
	UnicodeString name;
};

#define MAX_SAVE_CODEPAGES 7
extern code_page_inf SaveCodePages[MAX_SAVE_CODEPAGES];

//---------------------------------------------------------------------------
typedef BOOL (WINAPI *FUNC_PickIconDlg)(HWND, LPWSTR, DWORD, LPDWORD);

//---------------------------------------------------------------------------
class TUserModule : public TDataModule
{
__published:	// IDE �ŊǗ������R���|�[�l���g
	TAction *CalculatorAction;
	TAction *ClrHistComboAction;
	TAction *ClrListItemAction;
	TAction *CopyComboAction;
	TAction *CutComboAction;
	TAction *DelHistComboAction;
	TAction *DelListItemAction;
	TAction *DownListItemAction;
	TAction *PasteComboAction;
	TAction *RefCmdNameAction;
	TAction *RefCurFNameAction;
	TAction *RefCurPathAction;
	TAction *RefCurPNameAction;
	TAction *RefFileNameAction;
	TAction *RefOppPathAction;
	TAction *RegExCheckerAction;
	TAction *RenInsNameAction;
	TAction *SelAllComboAction;
	TAction *SizePosToFlieListAction;
	TAction *UpListItemAction;
	TAction *WebSeaComboAction;
	TAction *WebSeaEditAction;
	TActionList *CommonActList;
	TColorDialog *ColorDlg;
	TEditCopy *EditCopy1;
	TEditCut *EditCut1;
	TEditDelete *EditDelete1;
	TEditPaste *EditPaste1;
	TEditSelectAll *EditSelectAll1;
	TEditUndo *EditUndo1;
	TFontDialog *FontDlg;
	TMenuItem *CalculatorEItem;
	TMenuItem *CalculatorItem;
	TMenuItem *ClrHistItem;
	TMenuItem *CmdParamItem;
	TMenuItem *DelHistItem;
	TMenuItem *EditItemItem;
	TMenuItem *GrepCopyItem;
	TMenuItem *GrepCutItem;
	TMenuItem *GrepPasteItem;
	TMenuItem *PopAllSelItem;
	TMenuItem *PopCopyItem;
	TMenuItem *PopCurFnamItem;
	TMenuItem *PopCurPathItem;
	TMenuItem *PopCutItem;
	TMenuItem *PopDelItem;
	TMenuItem *PopDFmt01Item;
	TMenuItem *PopDFmt02Item;
	TMenuItem *PopDFmt03Item;
	TMenuItem *PopDFmt04Item;
	TMenuItem *PopDFmt05Item;
	TMenuItem *PopDFmt06Item;
	TMenuItem *PopDFmt07Item;
	TMenuItem *PopDFmt08Item;
	TMenuItem *PopDFmt09Item;
	TMenuItem *PopDlmt1Item;
	TMenuItem *PopDlmt2Item;
	TMenuItem *PopDlmt3Item;
	TMenuItem *PopDlmt4Item;
	TMenuItem *PopDlmt5Item;
	TMenuItem *PopDlmt6Item;
	TMenuItem *PopEditItemItem;
	TMenuItem *PopOppPathItem;
	TMenuItem *PopPasteItem;
	TMenuItem *PopRefCmdItem;
	TMenuItem *PopRefDlmtItem;
	TMenuItem *PopRefFmtItem;
	TMenuItem *PopUndoItem;
	TMenuItem *RefCmdItem;
	TMenuItem *RefCurFnamItem;
	TMenuItem *RefCurPnamItem;
	TMenuItem *RefFileNameItem;
	TMenuItem *RegExChkEItem;
	TMenuItem *RegExChkItem;
	TMenuItem *RegExpEItem;
	TMenuItem *RegExpItem;
	TMenuItem *RenInputNameItem;
	TMenuItem *SelAllItem;
	TMenuItem *Sep_e_1;
	TMenuItem *Sep_e_2;
	TMenuItem *Sep_e_3;
	TMenuItem *Sep_e_4;
	TMenuItem *Sep_e_5;
	TMenuItem *Sep_g_1;
	TMenuItem *Sep_g_2;
	TMenuItem *Sep_g_3;
	TMenuItem *WebSeaComboItem;
	TMenuItem *WebSeaEditItem;
	TOpenDialog *OpenDlg;
	TOpenPictureDialog *OpenImgDlg;
	TPopupMenu *EditPopupMenuC;
	TPopupMenu *EditPopupMenuE;
	TSaveDialog *SaveDlg;
	TSaveTextFileDialog *SaveTxtDlg;
	TTimer *BlinkTimer;
	TTimer *ScrollTimer;
	TTimer *SpuitTimer;

	void __fastcall DataModuleCreate(TObject *Sender);
	void __fastcall UpListItemActionExecute(TObject *Sender);
	void __fastcall UpListItemActionUpdate(TObject *Sender);
	void __fastcall DownListItemActionExecute(TObject *Sender);
	void __fastcall DownListItemActionUpdate(TObject *Sender);
	void __fastcall DelListItemActionExecute(TObject *Sender);
	void __fastcall DelListItemActionUpdate(TObject *Sender);
	void __fastcall ClrListItemActionExecute(TObject *Sender);
	void __fastcall ClrListItemActionUpdate(TObject *Sender);
	void __fastcall SpuitTimerTimer(TObject *Sender);

	void __fastcall CutComboActionExecute(TObject *Sender);
	void __fastcall EditComboActionUpdate(TObject *Sender);
	void __fastcall CopyComboActionExecute(TObject *Sender);
	void __fastcall PasteComboActionExecute(TObject *Sender);
	void __fastcall PasteComboActionUpdate(TObject *Sender);

	void __fastcall FileDlgClose(TObject *Sender);
	void __fastcall DelHistComboActionExecute(TObject *Sender);
	void __fastcall DelHistComboActionUpdate(TObject *Sender);
	void __fastcall ClrHistComboActionExecute(TObject *Sender);
	void __fastcall ClrHistComboActionUpdate(TObject *Sender);
	void __fastcall EditPopupMenuCPopup(TObject *Sender);
	void __fastcall RefRegExItemClick(TObject *Sender);
	void __fastcall SelAllComboActionExecute(TObject *Sender);
	void __fastcall SelAllComboActionUpdate(TObject *Sender);
	void __fastcall RefCmdNameActionExecute(TObject *Sender);
	void __fastcall RefCmdNameActionUpdate(TObject *Sender);
	void __fastcall RefCurPathActionExecute(TObject *Sender);
	void __fastcall RefCurPathActionUpdate(TObject *Sender);
	void __fastcall RefOppPathActionExecute(TObject *Sender);
	void __fastcall RefOppPathActionUpdate(TObject *Sender);
	void __fastcall RefCurFNameActionExecute(TObject *Sender);
	void __fastcall RefCurFNameActionUpdate(TObject *Sender);
	void __fastcall RefCurPNameActionExecute(TObject *Sender);
	void __fastcall RefCurPNameActionUpdate(TObject *Sender);
	void __fastcall EditPopupMenuEPopup(TObject *Sender);
	void __fastcall EditPaste1Execute(TObject *Sender);
	void __fastcall PopDFmtItemClick(TObject *Sender);
	void __fastcall ScrollTimerTimer(TObject *Sender);
	void __fastcall RegExCheckerActionExecute(TObject *Sender);
	void __fastcall RegExCheckerActionUpdate(TObject *Sender);
	void __fastcall DataModuleDestroy(TObject *Sender);
	void __fastcall CalculatorActionExecute(TObject *Sender);
	void __fastcall CalculatorActionUpdate(TObject *Sender);
	void __fastcall SizePosToFlieListActionExecute(TObject *Sender);
	void __fastcall CmdParamItemClick(TObject *Sender);
	void __fastcall PopDlmtItemClick(TObject *Sender);
	void __fastcall PopEditItemItemClick(TObject *Sender);
	void __fastcall EditItemItemClick(TObject *Sender);
	void __fastcall BlinkTimerTimer(TObject *Sender);
	void __fastcall WebSeaComboActionExecute(TObject *Sender);
	void __fastcall WebSeaComboActionUpdate(TObject *Sender);
	void __fastcall WebSeaEditActionExecute(TObject *Sender);
	void __fastcall WebSeaEditActionUpdate(TObject *Sender);
	void __fastcall RefFileNameActionExecute(TObject *Sender);
	void __fastcall RefFileNameActionUpdate(TObject *Sender);

private:	// ���[�U�[�錾
	TCustomListBox *ActiveListBox;		//����Ώۃ��X�g�{�b�N�X
	UsrScrollPanel *ActiveScrPanel;		//�V���v���X�N���[���o�[
	int PrevListIdx;

	TComboBox *LastComboBox;
	int LastComboBoxSS;
	int LastComboBoxSL;

	TPanel *SpuitPanel;
	TImage *SpuitImage;
	TColor SpuitDefColor;
	TColor SpuitPanelColor;

	TComponent *BlinkObj;

	void __fastcall ListBoxStartDrag(TObject *Sender, TDragObject *&DragObject);
	void __fastcall ListBoxDragOver(TObject *Sender, TObject *Source, int X, int Y,
			TDragState State, bool &Accept);
	void __fastcall ListBoxDragDrop(TObject *Sender, TObject *Source, int X, int Y);
	void __fastcall ListBoxEndDrag(TObject *Sender, TObject *Target, int X, int Y);
	void __fastcall ComboBoxContextPopup(TObject *Sender, const TPoint &MousePos, bool &Handled);

	TComboBox   * __fastcall GetActiveComboBox();
	TCustomEdit * __fastcall GetActiveCustomEdit();

	void __fastcall SelectIconDlg(UnicodeString &fnam);

public:		// ���[�U�[�錾
	TCursor crSpuitTool;
	TCursor crHandGrabR;

	UnicodeString ExePath;		//NyanFi���s�p�X
	UnicodeString CurFileName;	//�J�[�\���ʒu�̃t�@�C����
	UnicodeString CurPathName;	//���݂̃J�����g�p�X��
	UnicodeString OppPathName;	//���݂̔��΃p�X��
	UnicodeString RefFileName;	//���j���[�Q�Ɨp�t�@�C����

	UnicodeString IconFilePath;	//�A�C�R���Q�ƃp�X

	TStringList *RefRegExList;	//���K�\���̎Q�ƃ��X�g
	TStringList *CmdParamList;	//�R�}���h�p�����[�^�̎Q�ƃ��X�g

	TRect FileListRect;			//�t�@�C�����X�g�����̃X�N���[�����WRect

	int ScrMode;				//��ʃ��[�h

	__fastcall TUserModule(TComponent* Owner);

	void __fastcall InitializeListBox(TCustomListBox *lp, UsrScrollPanel *sp = NULL);
	void __fastcall UninitializeListBox();

	bool __fastcall ListBoxOpeItem(UnicodeString keystr);

	void __fastcall BeginSpuit(TPanel *pp, TImage *ip, TColor col_def);
	TColor __fastcall EndSpuit();
	bool __fastcall SpuitEnabled();

	void __fastcall SetUsrPopupMenu(TForm *fp);
	bool __fastcall ShowPopupMenu();

	void __fastcall SetOpenImgFilter(UnicodeString fnam);
	bool __fastcall SelectDirEx(const _TCHAR *tit, UnicodeString &dnam, bool to_rel = false);

	UnicodeString __fastcall get_DlgDefExt(UnicodeString filter);
	void __fastcall PrepareOpenDlg(const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam = NULL, UnicodeString inidir = EmptyStr);
	UnicodeString __fastcall OpenDlgExecute();
	bool __fastcall OpenDlgToEdit(TWinControl *ep, bool to_rel = false);
	bool __fastcall OpenDlgToStr(UnicodeString &s, bool to_rel = false);
	bool __fastcall OpenImgDlgToEdit(TWinControl *ep, bool to_rel = false);
	bool __fastcall FontDlgToFont(TFont *f);
	UnicodeString __fastcall OpenDlgIconIndex(bool to_rel = false);

	void __fastcall PrepareSaveDlg(const _TCHAR *tit, const _TCHAR *filter, const _TCHAR *fnam = NULL, UnicodeString inidir = EmptyStr);
	UnicodeString __fastcall SaveDlgExecute();

	void __fastcall SaveLastComboBox();
	void __fastcall RestoreLastComboBox();

	void __fastcall SetBlinkTimer(TComponent *cp);
	void __fastcall RepaintBlink(bool rst_sw = true);
};
//---------------------------------------------------------------------------
extern PACKAGE TUserModule *UserModule;
//---------------------------------------------------------------------------
#endif
