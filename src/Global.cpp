//----------------------------------------------------------------------//
// NyanFi																//
//  �O���[�o��															//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <algorithm>
#include <memory>
#include <utilcls.h>
#include <shlwapi.h>
#include <winioctl.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <VersionHelpers.h>
#include <System.DateUtils.hpp>
#include <System.Character.hpp>
#include <System.IOUtils.hpp>
#include <System.Zip.hpp>
#include <RegularExpressions.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.Imaging.GIFImg.hpp>
#include <Vcl.Clipbrd.hpp>
#include "usr_wic.h"
#include "usr_arc.h"
#include "usr_mmfile.h"
#include "usr_exif.h"
#include "usr_id3.h"
#include "usr_xd2tx.h"
#include "usr_highlight.h"
#include "usr_swatch.h"
#include "check_thread.h"
#include "UserFunc.h"
#include "UserMdl.h"
#include "InpExDlg.h"
#include "InpDir.h"
#include "Global.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
UnicodeString RstBatName;	//�ċN���p�o�b�`�t�@�C����

//---------------------------------------------------------------------------
//HTML�w���v
//�IVcl.HTMLHelpViewer �͗p�����AHtmlHelp API(���I�����N)�ŏ���
HINSTANCE	  hHHctrl	  = NULL;	//hhctrl.ocx
FUNC_HtmlHelp lpfHtmlHelp = NULL;
DWORD		  Cookie	  = NULL;
bool 		  CancelHelp  = false;

//---------------------------------------------------------------------------
//����JAPI
HINSTANCE hGdi32 = NULL;
FUNC_GetFontResourceInfo lpGetFontResourceInfo = NULL;

//---------------------------------------------------------------------------
TCursor crTmpPrev = (TCursor)10;	//�J�[�\���̃v���r���[�p

//---------------------------------------------------------------------------
UnicodeString KeyStr_SELECT   = "SELECT+";
UnicodeString KeyStr_Copy	  = "Ctrl+C";
UnicodeString KeyStr_Cut	  = "Ctrl+X";
UnicodeString KeyStr_Migemo   = "Ctrl+M";
UnicodeString KeyStr_Filter   = "Ctrl+F";
UnicodeString KeysStr_ToList  = "Ctrl+L";
UnicodeString KeysStr_CsrDown = "DOWN|Ctrl+X";
UnicodeString KeysStr_CsrUp   = "UP|Ctrl+E";
UnicodeString KeysStr_PgDown  = "PGDN|Ctrl+C";
UnicodeString KeysStr_PgUp	  = "PGUP|Ctrl+R";
UnicodeString KeysStr_Popup   = "APP|Shift+F10";

//---------------------------------------------------------------------------
UnicodeString SortIdStr = "FEDSAU";

//---------------------------------------------------------------------------
HWND  MainHandle = NULL;		//���C���E�B���h�E�̃n���h��
DWORD ProcessId  = 0;			//�v���Z�XID
bool  IsAdmin	 = false;		//�Ǘ��Ҍ���
int   StartedCount;				//���s�J�n�J�E���g(m�b)
int   NyanFiIdNo = 0;			//���d NyanFi ����ID
bool  IsPrimary  = true;		//�ŏ��ɋN�����ꂽ
win_dat Win2Data;				//��d�N���I�����̉�ʏ��

int ScrMode  = SCMD_FLIST;	//��ʃ��[�h

double ScrScale = 1.0;			//��ʃX�P�[�����O
int    SIcoSize = 16;			//�X���[���A�C�R���̕\���T�C�Y
int    Scaled1  = 1;
int    Scaled2  = 2;
int    Scaled3  = 3;
int    Scaled4  = 4;
int    Scaled8  = 8;

bool  IsMuted	= false;		//���ʃ~���[�g

bool  GitExists = false;		//Git ���C���X�g�[������Ă���

TRichEdit *TempRichEdit = NULL;

TTaskThread *TaskThread[MAX_TASK_THREAD];	//�^�X�N�����X���b�h
int  MaxTasks;								//�ő�^�X�N��
bool RsvSuspended = false;					//�\��ۗ̕�
TaskConfigList *TaskReserveList;			//�^�X�N�\�񃊃X�g
int  LastOpCount;							//�Ō�ɑ��삵�����̃J�E���g�l

TImgViewThread   *ImgViewThread   = NULL;	//�摜�r���[�����X���b�h
TThumbnailThread *ThumbnailThread = NULL;	//�T���l�C���擾�X���b�h
TGetIconThread   *GetIconThread   = NULL;	//�A�C�R���擾�X���b�h

UserArcUnit *usr_ARC;			//�A�[�J�C�u�������j�b�g
UnicodeString FExt7zDll;		//7z.dll �őΉ�����g���q

MigemoUnit  *usr_Migemo;		//Migemo�������j�b�g
UnicodeString MigemoPath;
bool LastMigemoMode;
bool LastMigemoModeF;

SpiUnit *SPI = NULL;			//Susie�v���O�C��
UnicodeString SpiDir;			//Susie�v���O�C���̃f�B���N�g��
bool UseSpiFirst;				//Susie�v���O�C����D��I�Ɏg��

int  WicScaleOpt;				//WIC �̏k���E�g��A���S���Y��
UnicodeString WicFextStr;		//WIC ���Ή����Ă���g���q

bool gCopyAll;
bool gCopyCancel;

int  gCopyMode	= CPYMD_OW;		//���E���ʃf�B���N�g���̏ꍇ�̃R�s�[���[�h
int  gCopyMode2 = CPYMD_OW;		//���E������f�B���N�g���̏ꍇ�̃R�s�[���[�h
int  xCopyMode = -1;			//ExeCommand ���ŗp���鋭�����[�h

UnicodeString gCopyFmt = "\\N_\\SN(1)";

int  OptionPageIndex = 0;		//�I�v�V�����ݒ�_�C�A���O�̃y�[�W�C���f�b�N�X

bool SyncLR;					//���E�̃f�B���N�g���ύX�𓯊�

UnicodeString GlobalErrMsg;

bool Initialized	= false;	//����������������
bool UnInitializing = false;	//�I���������J�n���ꂽ
bool Closing		= false;	//�I��������

int  InhReload = 0;				//Reload �̗}�~
bool DisReload = false;			//Reload �̖�����

bool InhUpdate = false;			//UpdateList �}�~

bool InhCmdHistory = false;		//�R�}���h�����̗}�~

UnicodeString FindPath;
bool FindAborted;				//�������f�v��
bool FindDiff;					//DiffDir ���s��
int  FindTag;
int  FindCount;					//�}�b�`��

bool CalcAborted;				//�v�Z���f�v��
int  CalcTag;

bool MultiInstance;				//��d�N��������
bool CloseOthers;				//����NyanFi���I��
bool StoreTaskTray;				//�ŏ������Ƀ^�X�N�g���C�Ɋi�[
bool ShowDirType;				//DIR��ʂ�\��
bool ShowSpace;					//�󔒂�\��
bool UseIndIcon;				//�t�@�C���ŗL�̃A�C�R�����g�p
bool ShowHideAtr;				//�B���t�@�C����\��
bool ShowSystemAtr;				//�V�X�e���t�@�C����\��
bool DispRegName;				//�p�X�̕\���ɓo�^����p����
bool UncToNetDrive;				//UNC���l�b�g���[�N�h���C�u����
bool PathInTitleBar;			//�^�C�g���o�[�ɃJ�����g�p�X��\��
bool OmitEndOfName;				//�������O�͖������ȗ�
bool ShowAdsInf;				//��փf�[�^�X�g���[������\��
bool ShowUseProcInf;			//�v���Z�X�g�p����\��
bool ShowDirJumboIco;			//�f�B���N�g���͓���A�C�R����\��
bool ShowInZipImg;				//ZIP���̉摜��\��
bool PreviewAniGif;				//�A�j���[�V����GIF�̃v���r���[
bool SetPrvCursor;				//�v���r���[���Ń}�E�X�J�[�\����ݒ�
bool ForceDel;					//�Ǎ���p�t�@�C���������I�ɏ㏑���E�폜
bool RemoveCdReadOnly;			//CD-ROM����̃R�s�[���ɓǍ���p����������
bool CopyTags;					//�^�O���R�s�[
bool CopyNoBuffering;			//�o�b�t�@�[�Ȃ�I/O���g�p���ăR�s�[
bool ShowArcCopyProg;
bool DelUseTrash;				//�t�@�C���̍폜�ɃS�~�����g��
bool EditNewText;				//�V�K�e�L�X�g�쐬��ɃG�f�B�^�ŊJ��
bool ViewArcInf;				//�A�[�J�C�u���̃t�@�C������\��
bool ReloadOnActive;			//�A�N�e�B�u���ɍŐV�̏���
bool OpenAddedDrive;			//�h���C�u���ǉ����ꂽ��J�����g�ɕ\��
bool CheckUnc;					//UNC�p�X���`�F�b�N
bool ShowMsgHint;				//�x���Ȃǂ��q���g�\��
bool ShowKeyHint;				//2�X�g���[�N����̃q���g�\��
bool ShowNoKeyHint;				//������2�X�g���[�N����̌x�����o��
bool ShowCopyHint;				//�N���b�v�{�[�h�ւ̃R�s�[�����q���g�\��
bool ShowDuplNotify;			//��d�N�����ꂽNyanFi����̒ʒm��\��
bool ShowTooltip;				//�c�[���`�b�v��\��
bool LogErrOnly;				//�^�X�N�ŃG���[�ӏ��݂̂����O�\��
bool LogErrMsg;					//���O�ɃG���[���b�Z�[�W���o��
bool LogDebugInf;				//���O�Ƀf�o�b�O�����o��
bool LogHideSkip;				//���O�ɃX�L�b�v���ڂ��o�͂��Ȃ�
bool LogFullPath;				//���O�̏������e���t���p�X�ŕ\��
bool SaveLog;					//�I�����Ƀ��O��ۑ�
bool AppendLog;					//�����̃��O�͒ǋL�ۑ�
bool NotSortWorkList;			//���[�N���X�g���ڂ��\�[�g���Ȃ�
bool RegWorkCsrPos;				//�J�[�\���ʒu�ɓo�^
bool AutoDelWorkList;			//���݂��Ȃ����ڂ������폜
bool NoCheckWorkUnc;			//���[�N���X�g��UNC���`�F�b�N���Ȃ�
bool AddToRecent;				//�ŋߎg�������ڂɒǉ�
bool NoCheckRecentUnc;			//UNC�p�X���`�F�b�N���Ȃ�
bool DirHistCsrPos;				//�f�B���N�g�������ŃJ�[�\���ʒu���L��
bool DelDplDirHist;				//�d������f�B���N�g���������폜
bool WorkToDirHist;				//���[�N���X�g�𗚗��Ɋ܂߂�
bool NoCheckDirHist;			//�f�B���N�g�������̑��݃`�F�b�N���s��Ȃ�
bool ExtSaveDirHist;			//�S�̗�����ʃt�@�C���ɕۑ�
bool LoopFilerCursor;			//�t�@�C���[�ŃJ�[�\�������[�v�ړ�
bool FlCursorVisible;			//�t�@�C���[�ŃJ�[�\������ɉ��̈��
bool AutoCompComboBox;			//�R���{�{�b�N�X���͂������⊮
bool RestoreComboBox;			//�A�N�e�B�u���ɃR���{�{�b�N�X�̏�Ԃ𕜌�
bool DialogCenter;				//�_�C�A���O�����C����ʂ̒�����
bool MenuAutoHotkey;			//���j���[�̃A�N�Z�����[�^�L�[����������
bool EscapeHelp;				//ESC�L�[�Ńw���v�����
bool InhbitAltMenu;				//ALT�L�[�Ń��j���[�Ƀt�H�[�J�X���Ȃ�
bool SelectByMouse;				//�}�E�X�Ńt�@�C���[�̍��ڂ�I��
bool SelectBaseOnly;			//�t�@�C�����啔�ł̂ݑI��
bool SelectIconSngl;			//�A�C�R�������ŌʂɑI��
bool TimColEnabled;				//�^�C���X�^���v�̔z�F��L��
bool PriorFExtCol;				//�g���q�����͑����F���D��
bool ColorOnlyFExt;				//�g���q�ʔz�F�͊g���q�����݂̂ɓK�p
bool SymColorToName;			//�t�@�C�����啔�ɃV���{���b�N�����N�F��K�p
bool RevTagCololr;				//�^�O�F�̔��]�\��
bool ShowMainMenu;				//���C�����j���[��\��
bool ShowImgPreview;			//�C���[�W�v���r���[��\��
bool ShowProperty;				//�t�@�C������\��
bool ShowLogWin;				//���O�E�B���h�E��\��
bool ShowFileListOnly;			//�t�@�C�����X�g�݂̂�\��
bool ShowFileListOnly2;
bool ShowSttBar;				//�X�e�[�^�X�o�[��\��
bool ShowToolBar;				//�c�[���o�[��\��
bool ShowToolBarV;
bool ShowToolBarI;
bool ToolBarISide;
bool ShowTabBar;				//�^�u�o�[��\��
bool ShowFKeyBar;				//�t�@���N�V�����L�[�o�[��\��
bool ShowClsTabBtn;				//�^�u�̕���{�^����\��
bool ShowPopTabBtn;				//�^�u�́��{�^����\��
bool ShowPopDirBtn;				//�f�B���N�g�����́��{�^����\��
bool HideScrBar;				//�c�X�N���[���o�[���B��
bool ShowByteSize;				//�o�C�g�P�ʕ\��
bool ShowTargetInf;				//���̂̏��ŕ\��
bool ShowHeader;				//�w�b�_��\��
bool FixListWidth;				//���X�g�����Œ�
bool KeepOnResize;				//�E�B���h�E�T�C�Y�ύX���ɔ䗦���ێ�
bool KeepCurListWidth;			//�J�����g���̃��X�g�����ێ�
bool NoSpaceFExt;				//�g���q�𕪗����Ȃ�
bool HideTitleMenu;				//�ő剻���Ƀ^�C�g���o�[�ƃ��j���[���B��
bool FlatInfPanel;				//���p�l�����t���b�g��
bool ShowLineNo;				//�s�ԍ���\��
bool ShowLineCursor;			//�s�J�[�\����\��
bool ShowTAB;					//�^�u��\��
bool ShowCR;					//���s��\��
bool ShowTextRuler;				//���[����\��
bool ScrBarToFoldPos;			//�X�N���[���o�[��܂�Ԃ��ʒu�ɕ\��
bool TxtSttIsBottom;			//���w�b�_�������ɕ\��(TV)
bool ChkAozora;					//�󕶌Ɍ`�����l������
bool UseXd2tx;					//xd2txlib.dll �Ńe�L�X�g�\��
bool ClickableUrl;				//�_�u���N���b�N��URL���J��
bool RestoreViewLine;			//�r���A�[�ŃJ�[�\���̍s�ʒu�𕜌�
bool TvCursorVisible;			//�r���A�[�ŃJ�[�\������ɉ��̈��
bool LimitBinCsr;				//�o�C�i���\���ŃJ�[�\���ړ��𐧌�
bool TxtColorHint;				//�J�[�\���ʒu���l�̃J���[���q���g�\��
bool AltBackSlash;				//\ �� �_(U+2216)�ŕ\��
bool DecodeDfmStr;				//.dfm �t�@�C���̕�������f�R�[�h
bool BinMemMaped;				//�o�C�i���ł̓������}�b�v�h�t�@�C���Ƃ��ĊJ��
bool EmpComment;				//�R�����g�������\��
bool EmpStrings;				//������������\��
bool EmpReserved;				//�\���������\��
bool EmpSymbol;					//�V���{���������\��
bool EmpNumeric;				//���l�������\��
bool EmpHeadline;				//���o���������\��
bool EmpRuby;					//���r�������\��
bool RotViewImg;				//Exif���ɂ��������ĉ摜����]
bool KeepZoomRatio;				//�ʂ̉摜�Ɉڂ������ɃY�[���{�����ێ�����
bool ShowThumbName;				//�T���l�C���Ƀt�@�C������\��
bool ShowThumbExif;				//�T���l�C���ɊȈ�Exif����\��
bool ShowThumbTags;				//�T���l�C���Ƀ^�O��\��
bool ShowThumbFExt;				//�T���l�C���Ɋg���q�������\��
bool NotThumbIfArc;				//�A�[�J�C�u���͈ꊇ�擾���Ȃ�
bool LoopViewCursor;			//�C���[�W�r���A�[�ŃJ�[�\�������[�v�ړ�
bool HintTopEndImg;				//�擪�E�Ō�Ńq���g�\��
bool BeepTopEndImg;				//�x����
bool ImgSttIsBottom;			//���w�b�_�������ɕ\��(IV)
bool SeekBindDir;				//�V�[�N�o�[�̎n�_��Ԃ������ɍ��킹��
bool SeekSwapNxtPrv;			//�E�Ԃ���Next/PrevFile����
bool HideCsrInFull;				//�S��ʕ\���Ń}�E�X�J�[�\�����B��
bool HideThumbInFull;			//�S��ʕ\���ŃT���l�C�����B��
bool AnimateGif;				//�C���[�W�r���A�[�ŃA�j���[�V����GIF��\��
bool ShowThumbScroll;			//�T���l�C���̃X�N���[���o�[��\��
bool ShowHistogram;				//�q�X�g�O������\��
bool ShowLoupe;					//���[�y��\��
bool ShowSubViewer;				//GIF�r���A�[��\��
bool ShowSeekBar;				//�V�[�N�o�[��\��
bool WarnHighlight;				//����ьx��
bool DoublePage;				//���J���\��
bool RightBind;					//�E�Ԃ�

bool PermitDotCmds;				//.nyanfi �ŃR�}���h���s������
bool InheritDotNyan;			//��ʃf�B���N�g������ .nyanfi ���p��
bool DotNyanPerUser;			//���[�U���ʂ� .nyanfi ���쐬

int  ScrBarStyle;				//�X�N���[���o�[�E�X�^�C��	0:�W��/ 1:�V���v��/ 2:3/4��/ 3:1/2��/ 4:�摜
int  IconMode;					//�A�C�R���̕\�����[�h		0:��\��/ 1:�\��/ 2:�f�B���N�g���̂ݕ\��

bool ModalScreen;				//���[�_���\������
int  ModalScrAlpha;				//�X�N���[���̓����x

int  TlWinBorderWidth;			//�c�[���E�C���h�E�̋��E����

int  InitialModeI;				//�C���[�W�r���A�[���J�������̏������
int  LastZoomRatio;				//�O��̃Y�[��

bool MarkImgClose;				//�C���[�W�r���A�[����鎞�ɞx�}�[�N��ݒ�
UnicodeString MarkImgPath;		//�}�[�N����p�X
UnicodeString MarkImgFExt;		//�}�[�N����g���q
UnicodeString MarkImgMemo;

UnicodeString DlgMoveShift;		//�_�C�A���O�ړ��̃V�t�g�L�[
UnicodeString DlgSizeShift;		//�_�C�A���O�̃T�C�Y�ύX�̃V�t�g�L�[
int  DlgMovePrm;
int  DlgSizePrm;

UnicodeString WheelCmdF[4];		//�z�C�[���R�}���h
UnicodeString WheelCmdV[4];
UnicodeString WheelCmdI[4];

UnicodeString WheelBtnCmdF[4];	//�z�C�[���{�^�������������̃R�}���h
UnicodeString WheelBtnCmdV;
UnicodeString WheelBtnCmdI;

UnicodeString X1BtnCmdF;		//X1�{�^�������������̃R�}���h
UnicodeString X2BtnCmdF;		//X2�{�^�������������̃R�}���h
UnicodeString X1BtnCmdV;
UnicodeString X2BtnCmdV;
UnicodeString X1BtnCmdI;
UnicodeString X2BtnCmdI;

UnicodeString EmpBinPtn1;		//�o�C�i�������p�^�[��1
UnicodeString EmpBinPtn2;		//�o�C�i�������p�^�[��2
UnicodeString EmpBinPtn3;		//�o�C�i�������p�^�[��3

UnicodeString HtmInsHrCls;		//�u���b�N�̑O�Ɍr����}������N���X
UnicodeString HtmHdrStr;		//���o������
bool HtmInsHrSct;				//<section>�̑O�Ɍr���}��
bool HtmInsHrArt;				//<article>�̑O�Ɍr���}��
bool HtmInsHrNav;				//<nav>�̑O�Ɍr���}��
bool ToMarkdown;				//Markdown�L�@�ɕϊ�

UnicodeString HtmDelBlkCls;		//�폜����u���b�N�� class
UnicodeString HtmDelBlkId;		//�폜����u���b�N�� id

UnicodeString NoWatchPath;		//�f�B���N�g���Ď����珜�O����p�X

UnicodeString FExtGetInf;		//�t�@�C�������擾����g���q
UnicodeString FExtNoInf;		//�t�@�C�������擾���Ȃ��g���q
UnicodeString NoInfPath;		//�t�@�C�������擾���Ȃ��p�X
UnicodeString EmpInfItems;		//�����\������t�@�C����񍀖�(|��؂�)
TStringList  *HideInfItems;		//�B���t�@�C����񍀖�(�g���q=|��؂�`���̃��X�g)

UnicodeString FExtImgPrv;		//�C���[�W�v���r���[���s���g���q
UnicodeString FExtNoImgPrv;		//�C���[�W�v���r���[���s��Ȃ��g���q
UnicodeString NoImgPrvPath;		//�C���[�W�v���r���[���s��Ȃ��p�X���X�g

UnicodeString FExtNoIView;		//�C���[�W�r���A�[�ŉ{�����Ȃ��g���q

UnicodeString DrvInfFmtR;		//�h���C�u���̏��� : ���[�g
UnicodeString DrvInfFmtS;		//�h���C�u���̏��� : �I����
UnicodeString DrvInfFmtN;		//�h���C�u���̏��� : ���̑�

UnicodeString SttBarFmt;		//�X�e�[�^�X�o�[�̏���
UnicodeString SttClockFmt;		//���v�̏���

UnicodeString TabPinMark;		//�^�u�Œ�s���}�[�N
UnicodeString HEAD_Mark;		//git HEAD �}�[�N
UnicodeString PLAY_Mark;		//�Đ����}�[�N

int  MaxLogFiles;				//���O�t�@�C���̕ۑ����㐔

int  ProtectDirMode;			//�f�B���N�g���̍폜�������[�h
bool ProtectSubDir;				//���ʃf�B���N�g��������
bool ProtectFile;				//�t�@�C��������

int  BorderMoveWidth;			//BorderLeft�ABorderRight: �P��̈ړ���
UnicodeString AutoRenFmt;		//Copy�AMove: �������̎�����������

int  RemoteWaitTime;			//�R�[���o�b�N���̃E�F�C�g�^�C��
int  NormalWaitTime;
int  NopDtctTime;				//�����삾�Ƃ݂Ȃ�����
int  TimeTolerance;				//�^�C���X�^���v�̋��e�덷

bool AppListChgMin;				//AppList:    ���A�v���ɐ؂�ւ����Ƃ��ɍŏ���
bool CreDirChg;					//CreateDir:  �f�B���N�g���쐬��ɃJ�����g�ύX
bool CreDirCnvChar;				//CreateDir:  �����u����K�p
UnicodeString FExtExeFile;		//ExeCommandLine: ���s�t�@�C���Ƃ݂Ȃ��g���q
bool OpenOnlyCurEdit;			//FileEdit:  �I����Ԃɂ�����炸�J�[�\���ʒu�݂̂��J��
bool DontClrSelEdit;			//FileEdit:  �I�����������Ȃ�
int  OpenByMode;				//OpenByApp:  �֘A�t������Ă��Ȃ��ꍇ�̓��샂�[�h
bool OpenDirByStd;				//OpenByApp:  �f�B���N�g���ł͕W�� Enter �L�[����
int  DblClickFlMode;			//�t�@�C�����X�g�ł̃_�u���N���b�N
bool OpenOnlyCurApp;			//OpenByApp:  �I����Ԃɂ�����炸�J�[�\���ʒu�݂̂��J��
bool DontClrSelApp;				//OpenByApp:  �I�����������Ȃ�
bool OpenOnlyCurWin;			//OpenByWin:  �I����Ԃɂ�����炸�J�[�\���ʒu�݂̂��J��
bool DontClrSelWin;				//OpenByWin:  �I�����������Ȃ�
bool OpenStdTabGroup;			//OpenStandard: �^�u�O���[�v���J��
bool OpenStdMenuFile;			//OpenStandard: ���j���[�t�@�C�����J��
bool OpenStdResultList;			//OpenStandard: ���ʃ��X�g���J��
bool DownAfterOpenStd;			//OpenStandard: ���s��ɃJ�[�\�������Ɉړ�
bool OpenStdOnResList;			//OpenStandard: ���ʃ��X�g�ł��ʏ퓮��
UnicodeString IniSeaShift;		//IniSearch:  �������T�[�`�̃V�t�g�L�[
bool IniSeaByNum;				//  �����L�[�ł��������T�[�`
bool IniSeaBySign;				//  Shift+�����L�[�̋L�����T�[�`
bool IncSeaCaseSens;			//IncSearch:  �召���������
bool IncSeaLoop;				//IncSearch:  �㉺�[�Ń��[�v
bool IncSeaMatch1Exit;			//IncSearch:  �}�b�`��1�Ŕ�����
int  IncSeaMigemoMin;			//IncSearch:  migemo �̌����J�n������
bool NotShowNoTask;				//TaskMan:    �^�X�N�����s���Ă��Ȃ��Ƃ��̓}�l�[�W����\�����Ȃ�
UnicodeString WebSeaUrl;		//WebSearch:  �����G���W��URL

bool FindPathColumn;			//���ʃ��X�g�Ɂu�ꏊ�v���\��
int  FindPathWidth;				//�u�ꏊ�v�̍ŏ���
bool FindTagsColumn;			//���ʃ��X�g�Ɂu�^�O�v���\��
int  FindTagsWidth;				//�u�^�O�v�̍ŏ���

bool GrepShowItemNo;			//GREP���ʂɍ��ڔԍ���\��
bool GrepFileItemNo;			//���ڔԍ����t�@�C���P�ʂŕ\��
bool GrepShowSubDir;			//GREP���ʂɃT�u�f�B���N�g������\��
bool GrepTrimTop;				//GREP���ʂōs���̃^�u��󔒂��폜
bool GrepOmitTop;				//GREP���ʂŃ}�b�`�ꂪ�����Ȃ��Ƃ��O�������ȗ�
bool GrepEmFilter;				//GREP���ʂŃt�B���^�̌�������\��
bool GrepAdjNextLn;				//GREP���ʂŎ��s�\�������̖��x������
int  GrepOutMode;				//GREP �o�͐�	0:����/ 1:�t�@�C��/ 2:�N���b�v�{�[�h
UnicodeString GrepFileName;		//�o�̓t�@�C����
UnicodeString GrepAppName;		//�N���A�v����
UnicodeString GrepAppParam;		//�A�v���̋N���p�����[�^
UnicodeString GrepAppDir;		//�A�v���̍�ƃf�B���N�g��
bool GrepAppend;				//�����t�@�C���ɒǉ�

UnicodeString GrepFileFmt;		//�t�@�C�����̏���
UnicodeString GrepInsStrW;		//�}�b�`��O�̑}��������
UnicodeString GrepInsStrW2;		//�}�b�`���̑}��������
bool GrepTrimLeft;				//�s���̃^�u��󔒂��폜
bool GrepReplaceTab;			//�^�u����1�����ɒu��
bool GrepReplaceCr;				//���s�𕶎���ɒu��
UnicodeString GrepRepCrStr;		//���s�̒u��������

bool BackupReplace;				//�u���o�b�N�A�b�v�쐬
UnicodeString FExtRepBackup;	//�u���o�b�N�A�b�v�̊g���q
UnicodeString RepBackupDir;		//�u���o�b�N�A�b�v��
UnicodeString ReplaceLogName;	//�u�����O�t�@�C����

bool SaveReplaceLog;			//�u�����O��ۑ�
bool ReplaceAppend;				//�����t�@�C���ɒǉ�
bool OpenReplaceLog;			//���O���e�L�X�g�G�f�B�^�ŊJ��

UnicodeString UserName;			//���[�U��
UnicodeString TempPath;			//�ꎞ�f�B���N�g��
UnicodeString TempPathA;		//�ꎞ�f�B���N�g��(���)
UnicodeString TempPathFTP;		//FTP�p�ꎞ�f�B���N�g��(���)

UnicodeString DownloadPath;		//�_�E�����[�h�p
UnicodeString LibraryPath;		//���C�u����
UnicodeString WorkListPath;		//���[�N���X�g�Q�ƃp�X
UnicodeString ResultListPath;	//���ʃ��X�g�Q�ƃp�X
UnicodeString RefParamPath;		//�p�����[�^�Q�ƃp�X
UnicodeString CmdFilePath;		//�R�}���h�t�@�C���Q�ƃp�X

int VersionNo;
UnicodeString VersionStr;		//�o�[�W�����\��

UnicodeString DirBraStr;		//�f�B���N�g�����ʕ���
UnicodeString DirKetStr;
UnicodeString TimeStampFmt;		//�^�C���X�^���v�̕\������

UnicodeString CallHotKey;		//�Ăяo���z�b�g�L�[
UnicodeString AppListHotKey;	//�A�v���ꗗ�z�b�g�L�[
UnicodeString AppListHotPrm;	//�A�v���ꗗ�p�����[�^

UnicodeString SaveTxtPath;		//�e�L�X�g�ۑ��p�X
int SaveEncIndex;				//�G���R�[�f�B���O�̃C���f�b�N�X

UnicodeString NoDirHistPath;	//�f�B���N�g�������ɓ���Ȃ��p�X
UnicodeString NoEditHistPath;	//�ҏW�����ɓ���Ȃ��p�X
UnicodeString NoViewHistPath;	//�{�������ɓ���Ȃ��p�X

UnicodeString DirDelimiter;		//�f�B���N�g����؂�̕\������

//---------------------------------------------------------------------------
UnicodeString FTPTextModeFExt;	//�e�L�X�g���[�h�œ]������g���q
bool FTPDlKeepTime;				//�_�E�����[�h�t�@�C���̃^�C���X�^���v���ێ�
bool FTPUpKeepTime;				//�A�b�v���[�h�t�@�C���̃^�C���X�^���v���ێ�
bool FTPUpToLower;				//�t�@�C�����������������ăA�b�v���[�h����
bool FTPLogResponse;			//�������b�Z�[�W�����O�ɕ\��
int  FTPRemoteSide;				//�����[�g�� 0:�J�����g/ 1:��/ 2:�E
int  FTPDisconTimeout;			//FTP��������̏ꍇ�ؒf���鎞��
UnicodeString FTPSndConnect;	//�ڑ����̒ʒm��
UnicodeString FTPSndDiscon;		//�ؒf���̒ʒm��
UnicodeString FTPSndTransfer;	//�]���������̒ʒm��

//---------------------------------------------------------------------------
int CurTabIndex = 0;			//�^�u�C���f�b�N�X(0�`

UnicodeString CurPathName;
int CurListTag = 0;				//�J�����g���^�O  0:��/ 1:�E
int OppListTag = 1;				//���Α��^�O
int LastCurTag = 0;				//�O��I�����̃J�����g
int WorkingTag = 0;				//��Ƒ��̃^�O(CurWorking = true ��)

TListBox *FileListBox[MAX_FILELIST];	//�t�@�C�����X�g�{�b�N�X
TPanel	 *FileListPanel[MAX_FILELIST];

UnicodeString ViewFileName;		//�\�����̃t�@�C����
UnicodeString ViewFileName2;
bool ViewFromArc = false;		//�A�[�J�C�u����\��

//---------------------------------------------------------------------------
TStringList *GeneralList;				//TStringList �̃��X�g(�ꊇ�j���p)

TStringList *FileList[MAX_FILELIST];	//�t�@�C�����X�g
TStringList *ArcFileList[MAX_FILELIST];	//���z�f�B���N�g���̃t�@�C�����X�g
TStringList *AdsFileList[MAX_FILELIST];	//��փf�[�^�X�g���[���̃t�@�C�����X�g
TStringList *ResultList[MAX_FILELIST];	//�������ʃ��X�g
TStringList *SelMaskList[MAX_FILELIST];	//�I���}�X�N���X�g
TStringList *TmpBufList[MAX_FILELIST];	//�ꎞ�ޔ����X�g

TStringList *TabList;					//�^�u���X�g
TStringList *TabBuff;					//�Œ蕜�A�p�o�b�t�@
int FlTabWidth;							//�^�u�̕�
int FlTabStyle;							//�^�u�̃X�^�C��
UnicodeString TabGroupName;				//�^�u�O���[�v�E�t�@�C����

TStringList *DriveInfoList;				//�h���C�u���
TStringList *DriveLogList;				//�h���C�u�e�ʃ��O
UnicodeString DriveLogName;				//�h���C�u�e�ʃ��O�̃t�@�C����

TStringList *WatchTailList;				//�����Ď����X�g
UnicodeString LastWatchLog;				//���O�̊Ď����O

TStringList *InvalidUncList;			//������UNC���X�g

TStringList *GitCfgUrlList;				//.Git\config - URL �Ή����X�g (�t�@�C����=URL \t yyyy/mm/dd hh:nn:ss)
TStringList *GitInfList;				//Git���̃L���b�V��

//�t�@�C���ŗL�A�C�R���̃L���b�V�� (�K�� IconRWLock �ŕی삷�邱��)
TStringList *CachedIcoList;
TMultiReadExclusiveWriteSynchronizer *IconRWLock;
int IconCache;							//�A�C�R���L���V����

//�t�H���_�A�C�R��
UsrIniFile  *FolderIconFile;			//�t�H���_�A�C�R���ݒ�t�@�C��
TStringList *FolderIconList;			//�t�H���_�A�C�R�����X�g
TMultiReadExclusiveWriteSynchronizer *FldIcoRWLock;
UnicodeString DefFldIcoName;			//�f�t�H���g�̃t�H���_�A�C�R��
HICON hLinkIcon = NULL;					//�����N�}�[�N(Shell32.dll,29)

TStringList *GeneralIconList;			//�t�@�C�����X�g�\���p�̈�ʃA�C�R��
TStringList *MenuBtnIcoList;

TStringList *UsrIcoList;				//���j���[�A�c�[���{�^���p�A�C�R��
TStringList *DrvIcoList;				//�h���C�u�̃A�C�R�����X�g

TStringList *BakSetupList;				//�o�b�N�A�b�v�ݒ胊�X�g
TStringList *SyncDirList;				//�����R�s�[�惊�X�g
TStringList *AssRenList;				//�֘A�t�@�C�����������ݒ胊�X�g
TStringList *DistrDefList;				//�U�蕪�����`���X�g

TStringList *GrepPathList;				//GREP �Ώۃp�X�̃��X�g
TStringList *GrepFileList; 				//GREP �Ώۃt�@�C�����X�g
TStringList *GrepResultList;			//GREP �̌��ʃ��X�g
TStringList *GrepResultBuff;			//GREP ���ʂ̑ޔ��o�b�t�@

TStringList *ViewFileList;				//�C���[�W�r���A�[�ł̃t�@�C�������X�g
bool isViewIcon   = false;				//�C���[�W�r���A�[�ŃA�C�R����\����
bool isViewAGif   = false;				//�C���[�W�r���A�[�ŃA�j���[�V����GIF��\����
bool isViewClip   = false;				//�C���[�W�r���A�[�ŃN���b�v�{�[�h��\����
bool isViewFTP	  = false;				//�C���[�W�r���A�[��FTP�̃t�@�C����\����
bool rqThumbnail  = false;				//�N���b�v�{�[�h�{����̃T���l�C�����A�v��
bool nrmThumbnail = false;				//�ʏ��ʎ��̃T���l�C���\��

TStringList *LaunchList;				//�����`���[���X�g(�A�v���P�[�V�����ꗗ)

TStringList *CmdFileList;				//�R�}���h�t�@�C���̃��X�g
TStringList *XCMD_VarList = NULL;		//�ϐ����X�g

UnicodeString PathMask[MAX_FILELIST];	//�p�X�}�X�N
UnicodeString DriveInf[MAX_FILELIST];	//�h���C�u���

flist_stt ListStt[MAX_FILELIST];
flist_stt *CurStt;
flist_stt *OppStt;

TStringList *WorkList;					//���[�N���X�g
UnicodeString WorkListName;				//���[�N���X�g�̃t�@�C����
TDateTime WorkListTime;					//���[�N���X�g�̃^�C���X�^���v
bool WorkListChanged;					//���[�N���X�g�̓��e���ύX���ꂽ
bool WorkListFiltered;					//���[�N���X�g�Ƀt�B���^���K�p����Ă���
bool WorkListHasSep;					//���[�N���X�g�ɃZ�p���[�^������
bool rqWorkListDirInf;					//���[�N���X�g�̃f�B���N�g�����X�V��v��

TStringList  *PlayList;					//�v���C���X�g
UnicodeString PlayListFile;				//�v���C���X�g�̃t�@�C����
UnicodeString PlayFile;					//�Đ����t�@�C����
int  PlayStbIdx;
bool PlayRepeat;						//���s�[�g�Đ�
bool PlayShuffle;						//�V���b�t���Đ�
bool ListShuffled;						//�V���b�t�����ꂽ

TStringList *PopMenuList;				//ExeMenuFile �R�}���h�p���j���[����
TStringList *ToolBtnList;				//�c�[���{�^����`���X�g
TStringList *ToolBtnListV;
TStringList *ToolBtnListI;

//---------------------------------------------------------------------------
//�N�����I�v�V����
int  IniPathMode[MAX_FILELIST];			//�p�X���[�h
UnicodeString InitialPath[MAX_FILELIST];
UnicodeString InitialMask[MAX_FILELIST];
int  IniSortMode[MAX_FILELIST];
int  IniWorkMode;						//���[�N���X�g 0:�O��/ 1:�w��
UnicodeString HomeWorkList;
int  IniWinMode;						//�T�C�Y���[�h 0:�O��/ 1:�w��
int  IniWinLeft, IniWinTop, IniWinWidth, IniWinHeight;
bool FixWinPos;							//�E�B���h�E�T�C�Y���Œ�
bool IniPathToTab1;						//�����p�X���^�u1�ɐݒ�
bool IniTabHomeAll;						//�����p�X�ȊO�̃^�u���z�[���ɖ߂�
bool ShowSplash;						//�X�v���b�V����\��

//�t�@�C�����X�g�̃\�[�g���[�h
int  SortMode[MAX_FILELIST];			//0:���O/ 1:�g���q/ 2:�X�V����/ 3:�T�C�Y/ 4:����/ 5:�Ȃ�
int  DirSortMode[MAX_FILELIST];			//0:�t�@�C���Ɠ���/ 1:���O/ 2:�X�V����/ 3:�T�C�Y/ 4:����/
										//	5:�f�B���N�g������ʂ��Ȃ�/ 6:�t�H���_�A�C�R��

//�t�@�C�����X�g�̃\�[�g��
bool FlOdrNatural[MAX_FILELIST];		//���R��
bool FlOdrDscName[MAX_FILELIST];		//�~��
bool FlOdrSmall[MAX_FILELIST];			//��������
bool FlOdrOld[MAX_FILELIST];			//�Â���
bool FlOdrDscAttr[MAX_FILELIST];		//�����~��
bool FlOdrDscPath[MAX_FILELIST];		//�ꏊ�~��

bool SortBoth;					//���E�Ƃ��ύX

//��r�֐��̃\�[�g��
bool NaturalOrder = true;		//���R��
bool DscNameOrder = false;		//���O�~��
bool SmallOrder   = false;		//��������
bool OldOrder	  = false;		//�Â���
bool DscAttrOrder = false;		//�����~��
bool DscPathOrder = false;		//�ꏊ�~��

TStringList *OptionList;		//�I�v�V����
TStringList *KeyFuncList;		//�L�[���蓖��
TStringList *FKeyLabelList;		//�t�@���N�V�����L�[��
TStringList *HotKeyList;		//�z�b�g�L�[
TStringList *DirStack;			//�f�B���N�g���X�^�b�N
TStringList *PathMaskList;		//�p�X�}�X�N���X�g
TStringList *RegDirList;		//�o�^�f�B���N�g��
TStringList *ProtectDirList;	//�폜�����f�B���N�g��
TStringList *VirDriveList;		//���z�h���C�u���X�g
TStringList *ColorList;			//�z�F
TStringList *ExtColList;		//�g���q�ʔz�F
TStringList *AssociateList;		//�֘A�t��
TStringList *OpenStdCmdList;	//OpenStandard �̃R�}���h�֘A�t��
TStringList *EtcEditorList;		//���̑��̃G�f�B�^���X�g
TStringList *ExtToolList;		//�O���c�[��
TStringList *ExtMenuList;		//�ǉ����j���[
TStringList *CommandList;		//�R�}���h���X�g
TStringList *CmdSetList;		//�R�}���h�ꗗ���X�g
TStringList *TaskCmdList;		//�^�X�N�R�}���h�����X�g
TStringList *DriveList;			//�ω��Ď��p�h���C�u���X�g
TStringList *NetDriveList;		//���ݗL���ȃl�b�g�h���C�u���X�g
TStringList *AllDirHistory;		//�S�̂̃f�B���N�g������
TStringList *TextViewHistory;	//�e�L�X�g�{������
TStringList *TextEditHistory;	//�e�L�X�g�ҏW����
TStringList *WorkListHistory;	//���[�N���X�g����
TStringList *InputDirHistory;	//�f�B���N�g�����̗͂���
TStringList *InputCmdsHistory;	//�R�}���h���̗͂���
TStringList *InputCmdsHistoryV;
TStringList *InputCmdsHistoryI;
TStringList *IncSeaHistory;		//�C���N�������^���T�[�`�̃L�[���[�h����
TStringList *FilterHistory;		//�t�B���^�̃L�[���[�h����
TStringList *WebSeaHistory;		//Web������̗���
TStringList *LatLngHistory;		//�ܓx�o�x�����̗���
TStringList *HeadlineList;		//���o�����X�g (�g���q=���K�\���p�^�[��)
TStringList *ZoomRatioList;		//�Y�[���{�����X�g
TStringList *CnvCharList;		//�֎~�����^���[�U��`�����ϊ����X�g
TStringList *RenCmdFileList;	//���������R�}���h�t�@�C���̃��X�g
TStringList *RenArcFileList;	//���������A�[�J�C�u�t�@�C���̃��X�g
TStringList *RedrawList;		//�`��}�~�p���X�g
TStringList *CmdRequestList;	//�R�}���h�v�����X�g
TStringList *CommandHistory;	//�R�}���h����

//�^�X�N����̃��O�����o�b�t�@ (�K�� LogRWLock �ŕی삷�邱��)
UnicodeString LogBufStr;
TMultiReadExclusiveWriteSynchronizer *LogRWLock;

TListBox     *LogWndListBox;	//���O�E�B���h�E�p���X�g�{�b�N�X
TStringList  *LogBufList;		//���O���X�g�{�b�N�X(���z)�p�o�b�t�@
UsrScrollPanel *LogWndScrPanel;	//�V���v���X�N���[���o�[

int  LayoutMode;		//��ʃ��C�A�E�g���[�h
bool DivFileListUD;		//�t�@�C�����X�g���㉺�ɕ���
bool DivDirInfUD;		//�f�B���N�g�������㉺�ɕ���

//�t�H���g
TFont *ListFont;		//�t�@�C�����X�g��
TFont *FileInfFont;		//�t�@�C�����
TFont *TxtPrvFont;		//�e�L�X�g�v���r���[
TFont *LogFont;			//���O
TFont *ViewerFont;		//�e�L�X�g�r���A�[
TFont *GrepResFont;		//GREP���ʃ��X�g
TFont *DirInfFont;		//�f�B���N�g�����
TFont *DrvInfFont;		//�h���C�u���
TFont *LstHdrFont;		//�ꗗ�̃w�b�_
TFont *ViewHdrFont;		//�r���A�[���w�b�_
TFont *GenListFont;		//�ꗗ�_�C�A���O
TFont *HintFont;		//�q���g
TFont *SttBarFont;		//�X�e�[�^�X�o�[
TFont *CalcFont;		//�d��
TFont *DialogFont;		//�_�C�A���O
TFont *ToolBarFont;		//�c�[���o�[
TFont *TabBarFont;		//�^�u�o�[
TFont *CharInfFont;		//�������
TStringList *FontList;

//�J���[
TColor col_bgList;		//�ꗗ�̔w�i�F
TColor col_bgList2;		//�ꗗ�̔w�i�F(����)
TColor col_fgList;		//�ꗗ�̕����F
TColor col_Splitter;	//�������E��
TColor col_bgArc;		//���z�f�B���N�g���̔w�i�F
TColor col_bgFind;		//�������ʃ��X�g�̔w�i�F
TColor col_bgWork;		//���[�N���X�g�̔w�i�F
TColor col_bgFTP;		//FTP�����[�g���̔w�i�F
TColor col_bgADS;		//��փf�[�^�X�g���[���ꗗ�̔w�i�F
TColor col_selItem;		//�I�����ڂ̔w�i�F
TColor col_oppItem;		//���΃p�X�I�����ڂ̔w�i�F
TColor col_fgSelItem;	//�I�����ڂ̕����F
TColor col_bgMark;		//�x�}�[�N���ڂ̔w�i�F
TColor col_matchItem;	//INC.�T�[�`�̃}�b�`���ڔw�i�F
TColor col_Differ;		//���ʃ��X�g�̑���ӏ��w�i�F
TColor col_DifferN;		//���ʃ��X�g�̒��ډӏ��w�i�F
TColor col_Cursor;		//���C���J�[�\���̐F
TColor col_bgScrBar;	//�V���v���X�N���[���o�[�̔w�i�F
TColor col_bgScrKnob;	//�V���v���X�N���[���m�u�̐F
TColor col_frScrKnob;	//�V���v���X�N���[���m�u�̗֊s�F
TColor col_bgActKnob;	//�h���b�O���̃V���v���X�N���[���m�u�F
TColor col_lnScrHit;	//�V���v���X�N���[���o�[�̃q�b�g�s�F
TColor col_lnScrSel;	//�V���v���X�N���[���o�[�̑I���s�F
TColor col_Folder;		//�f�B���N�g���̕����F
TColor col_SymLink;		//�V���{���b�N�����N/�W�����N�V�����̕����F
TColor col_Protect;		//�폜�����f�B���N�g���̕����F
TColor col_ReadOnly;	//�ǂݍ��ݐ�p�����̕����F
TColor col_Compress;	//���k�����̕����F
TColor col_Hidden;		//�B�������̕����F
TColor col_System;		//�V�X�e�������̕����F
TColor col_fgSpace;		//�󔒋L���̕����F
TColor col_fgTagNam;	//�f�t�H���g�̃^�O�F
TColor col_InvItem;		//�����ȍ��ڂ̕����F
TColor col_bgTabBar;	//�^�u�o�[�̔w�i�F
TColor col_bgActTab;	//�A�N�e�B�u�ȃ^�u�̔w�i�F
TColor col_bgInAcTab;	//��A�N�e�B�u�ȃ^�u�̔w�i�F
TColor col_frmTab;		//�^�u�̗֊s�F
TColor col_fgTab;		//�^�u�̕����F
TColor col_bgListHdr;	//���X�g�w�b�_�̔w�i�F
TColor col_fgListHdr;	//���X�g�w�b�_�̕����F
TColor col_bgDirInf;	//�f�B���N�g�����̔w�i�F
TColor col_fgDirInf;	//�f�B���N�g�����̕����F
TColor col_bgDirRel;	//�f�B���N�g���֌W�̔w�i�F
TColor col_fgDirRel;	//�f�B���N�g���֌W�̕����F
TColor col_bgDrvInf;	//�h���C�u���̔w�i�F
TColor col_fgDrvInf;	//�h���C�u���̕����F
TColor col_bgInf;		//�t�@�C�����̔w�i�F
TColor col_fgInf;		//�t�@�C�����̕����F
TColor col_fgInfNam;	//�t�@�C�����̍��ږ������F
TColor col_fgInfEmp;	//�t�@�C�����̋��������F
TColor col_bgTxtPrv;	//�e�L�X�g�v���r���[�̔w�i�F
TColor col_fgTxtPrv;	//�e�L�X�g�v���r���[�̕����F
TColor col_bgLog;		//���O�̔w�i�F
TColor col_fgLog;		//���O�̕����F
TColor col_bgTask;		//�^�X�N��ԕ\���̔w�i�F
TColor col_fgPrgBar;	//�^�X�N�i���o�[�F
TColor col_bgPrgBar;	//�^�X�N�i���w�i�F
TColor col_Error;		//�G���[/���ӂ̕����F
TColor col_bgOptTab;	//�A�N�e�B�u�Ȑݒ�^�u�̔w�i�F
TColor col_fgOptTab;	//�A�N�e�B�u�Ȑݒ�^�u�̕����F
TColor col_TlBorder;	//�c�[���E�C���h�E�̋��E��

TColor col_bgView;		//�e�L�X�g�r���A�[�̔w�i�F
TColor col_fgView;		//�e�L�X�g�r���A�[�̕����F
TColor col_Margin;		//�e�L�X�g�r���A�[�̗]����
TColor col_bgRuler;		//���[���̔w�i�F
TColor col_fgRuler;		//���[���̖ڐ��F
TColor col_bgLineNo;	//�s�ԍ��w�i�F
TColor col_LineNo;		//�s�ԍ������F
TColor col_Mark;		//�s�}�[�N
TColor col_bdrLine;		//�s�ԍ��̋��E��
TColor col_bdrFold;		//�܂�Ԃ����E��
TColor col_bdrFixed;	//�Œ蒷�\���̏c�r��
TColor col_Comment;		//�R�����g�̕����F
TColor col_Strings;		//������̕����F
TColor col_Reserved;	//�\���̕����F
TColor col_Symbol;		//�V���{���̕����F
TColor col_Numeric;		//���l�̕����F
TColor col_fgEmpBin1;	//�o�C�i�����������F1
TColor col_fgEmpBin2;	//�o�C�i�����������F2
TColor col_fgEmpBin3;	//�o�C�i�����������F3
TColor col_Headline;	//���o���̕����F
TColor col_URL;			//URL�̕����F
TColor col_LocalLink;	//���[�J�t�@�C���ւ̃����N
TColor col_fgEmp;		//���������F
TColor col_bgEmp;		//�����w�i�F
TColor col_Ruby;		//���r
TColor col_TAB;			//�^�u�\���F
TColor col_CR;			//���s�\���F
TColor col_HR;			//�r���̐F
TColor col_Ctrl;		//�R���g���[���R�[�h

TColor col_bgImage;		//�摜�̔w�i�F
TColor col_bgDblPg;		//���J���\���̗]���F
TColor col_bgWMF;		//���^�t�@�C���̔w�i�F
TColor col_bdrThumb;	//�T���l�C���̋��E��
TColor col_ThumbExif;	//�T���l�C����Eixf���
TColor col_ImgGrid;		//�摜�����O���b�h
TColor col_OptFind;		//�I�v�V�����ݒ�̌�������
TColor col_bgHint;		//�q���g�\���̔w�i�F
TColor col_fgHint;		//�q���g�\���̕����F
TColor col_bgWarn;		//�x���\���̔w�i�F
TColor col_Invalid;		//�����ȍ��ڂ̔w�i�F
TColor col_Illegal;		//�s���ȓ��͍��ڂ̔w�i�F
TColor col_Tim1H;		//1���Ԉȓ��̃^�C���X�^���v�F
TColor col_Tim3H;		//3���Ԉȓ��́`
TColor col_Tim1D;		//1���ȓ��́`
TColor col_Tim3D;		//3���ȓ��́`
TColor col_Tim7D;		//7���ȓ��́`
TColor col_Tim1M;		//1�����ȓ��́`
TColor col_Tim3M;		//3�����ȓ��́`
TColor col_Tim6M;		//6�����ȓ��́`
TColor col_Tim1Y;		//1�N�ȓ��́`
TColor col_Tim3Y;		//3�N�ȓ��́`
TColor col_TimOld;		//������O�́`
TColor col_Size4G;		//4GB�ȏ�
TColor col_Size1G;		//1GB�ȏ�
TColor col_Size1M;		//1MB�ȏ�
TColor col_Size1K;		//1KB�ȏ�
TColor col_SizeLT;		//1KB����
TColor col_Size0;		//0

TColor col_GrBack;		//�O���t�̔w�i�F
TColor col_GrLine;		//�O���t�̃��C��
TColor col_GrGrid;		//�O���t�̃O���b�h
TColor col_GrText;		//�O���t�̕����F

TColor col_GitHEAD;		//Git�r���A�[ : �w�b�h
TColor col_GitMark;		//  �O���t�}�[�N
TColor col_GitBra;		//  �u�����`
TColor col_GitBraR;		//  �����[�g�u�����`
TColor col_GitTag;		//  �^�O
TColor col_GitHash;		//  �n�b�V��
TColor col_GitIns;		//  - �s
TColor col_GitDel;		//  + �s

TColor col_bgTlBar1;	//�c�[���o�[�̃O���f�[�V�����J�n�F
TColor col_bgTlBar2;	//�c�[���o�[�̃O���f�[�V�����I���F
TColor col_fgTlBar;		//�c�[���o�[�̕����F
TColor col_htTlBar;		//�c�[���o�[�z�b�g�g���b�L���O�F

TColor col_bgSttBar;	//�X�e�[�^�X�o�[�̔w�i�F
TColor col_fgSttBar;	//�X�e�[�^�X�o�[�̕����F
TColor col_bgInfHdr;	//���w�b�_�̔w�i�F
TColor col_fgInfHdr;	//���w�b�_�̕����F

TColor col_bgChInf;		//�������T���v���̔w�i�F
TColor col_fgChInf;		//�������T���v���̕����F
TColor col_bgEdBox;		//Edit �{�b�N�X�̔w�i�F
TColor col_fgEdBox;		//Edit �{�b�N�X�̕����F

TColor col_ModalScr;	//���[�_���\�����ʐF

const TColor col_Teal = clTeal;
const TColor col_None = Graphics::clNone;

//�w�i�摜
Graphics::TBitmap *BgImgBuff[MAX_BGIMAGE];
UnicodeString BgImgName[MAX_BGIMAGE];

int  BgImgMode;			//���[�h
int  BgImgSubMode;
int  BgColAlpha;		//�w�i�F�A���t�@
bool BgImgGray;			//�O���[�X�P�[����
bool BgImgHideScr;		//�X�N���[�����ɉB��
int  BgHideTime;		//���A����
bool BgImgTileIf;		//�w��T�C�Y�ȉ��Ȃ���ׂĕ\��
int  BgTileSize;

bool AlphaForm;			//���C����ʂ𓧖���
int  AlphaValue;		//���C����ʃA���t�@

//�t�H���g�̃T���v���\��
UnicodeString FontSampleTxt = "0123456789(!?)+-\r\nABCDEFGabcdefg\r\n�����������A�C�E�G�I\r\n�t�ďH�~�Ԓ��������L";
UnicodeString FontSampleSym = "!\"#$%&'()*+,-./\r\n0123456789:;<=>\r\n?@ABCDEFGHIJKLM\r\nNOPQRSTUVWXYZ[\\\r\n";

int    FontSampleSize	  = 20;
TColor FontSampleFgCol	  = col_None;	//�����F
TColor FontSampleBgCol	  = col_None;	//�w�i�F
TColor FontSampleGridCol  = col_None;	//����F
bool   FontSampleShowGrid = false;		//�����\��

//�O���c�[��
UnicodeString TextEditor;		//�e�L�X�g�G�f�B�^
UnicodeString TextEditorFrmt;	//�p�����[�^(Grep�p)

UnicodeString ExtTxViewer;		//�O���e�L�X�g�r���A�[
UnicodeString ExtTxViewerFrmt;

UnicodeString ImageEditor;		//�C���[�W�G�f�B�^
UnicodeString FExtImgEidt;		//�C���[�W�G�f�B�^�̑Ώۊg���q
bool ImageEditSgl;				//�C���[�W�G�f�B�^���t�@�C�����ɌʋN��

UnicodeString FExtViewTab4;		//�^�u4�̊g���q
UnicodeString FExtViewTabX;		//�C�Ӄ^�u���̊g���q
int  ViewTabWidthX;				//�C�Ӄ^�u��

UnicodeString BinaryEditor;		//�o�C�i���G�f�B�^

UnicodeString CmdGitExe;		//git.exe
UnicodeString GitBashExe;		//git-bash.exe
UnicodeString GitGuiExe;		//git-gui.exe

//�T�E���h
UnicodeString SoundTaskFin;		//�^�X�N�I�����̒ʒm��
UnicodeString SoundFindFin;		//�����I�����̒ʒm��
UnicodeString SoundWarning;		//�x����
UnicodeString SoundWatch;		//�t�@�C���Ď�

int  FlashCntWarning;			//�x���̓_�ŉ�
int  FlashTimeWarning;			//�x���̓_�ŊԊu
int  FlashCntTaskfin;			//�^�X�N�I�����̓_�ŉ�
int  FlashTimeTaskfin;			//�^�X�N�I�����̓_�ŊԊu

bool WarnPowerFail;				//�d���ؒf�x��
bool WarnDisconnect;			//�C���^�[�l�b�g�ڑ��؂�
bool WarnLowBattery;			//�o�b�e���[�ቺ
int  BatLowerLimit;

int  MaxLogLines;				//���O�̍ő�s��
int  MsgHintTime;				//�q���g�̕\������
int  KeyHintTime;				//2�X�g���[�N�q���g�̑ҋ@����
int  NotifyTaskTime;			//�`�b�ȏォ�������^�X�N�̏I�����ɖ炷
int  CmpDelOwCnt;				//���S�폜�̏㏑����
int  ViewTxtInterLn;			//�r���A�[�̍s��
int  ViewLeftMargin;			//�r���A�[�̍��]��
int  ViewFoldWidth;				//�܂�Ԃ���(���p�P��)
bool ViewFoldFitWin;			//�܂�Ԃ����E�B���h�E���ɍ��킹��
int  ViewFixedLimit;			//�Œ蒷�\���̍ő啝
int  ViewTxtLimitSize;			//�e�L�X�g�̍ő�Ǎ��T�C�Y
int  ViewBinLimitSize;			//�o�C�i���̍ő�Ǎ�(or �}�b�v)�T�C�Y
int  ListWheelScrLn;			//�z�C�[���ɂ��X�N���[���s��(���X�g)
int  ViewWheelScrLn;			//�z�C�[���ɂ��X�N���[���s��(�r���A�[)
int  PlaySndLimitTime;			//�T�E���h�Đ��^�C������
bool ShowTextPreview;			//�e�L�X�g�v���r���[��\��
int  PrvTxtInfLn;				//�e�L�X�g�v���r���[�̍s��
bool ShowTailPreview;			//�����v���r���[��\��
int  PrvTxtTailLn;				//�����v���r���[�s��
int  PrvActTailLn;				//�����v���r���[��L���ɂ���s��
int  WatchInterval;				//�f�B���N�g���Ď��Ԋu
int  LogInterval;				//���O�̍X�V�Ԋu
int  ListInterLn;				//�t�@�C�����X�g�̍s��
int  CursorWidth;				//�J�[�\�����̕�
int  CursorAlpha;				//�J�[�\���s�w�i�A���t�@
int  CellAlpha;					//�Z���̔w�i�A���t�@
int  SplitterWidth;				//���E���̕�
int  SplitterWidth2;
int  SizeFormatMode;
int  SizeDecDigits;				//�T�C�Y�\���ɂ����鏬���_�ȉ��̌���
int  ListPercent;				//�t�@�C�����X�g���̔䗦
int  ImgFrameMargin;			//�t�B�b�g�\�����̗]����
int  ImgFitMaxZoom;				//�t�B�b�g�\�����̋��e�{��
int  ImgGridHorzN;				//�O���b�h�̉�������
int  ImgGridVertN;				//�O���b�h�̏c������
int  MinShowTime;				//�ŏ��\���^�C��
int  ThumbnailSize;				//�T���l�C���̃T�C�Y
int  ThumbBdrWidth;				//�T���l�C���̋��E����
int  ThumbnailPos;				//�T���l�C���̕\���ʒu
int  ImgDblMargin;				//���J���Ԋu

bool ShowImgSidebar;			//�T�C�h�o�[��\��
bool ImgSidebarIsLeft;			//�T�C�h�o�[�͍�

bool HasCallHotkey;				//�Ăяo���z�b�g�L�[�����邩�H

bool HideSizeTime;				//�T�C�Y�ƍX�V�������B��

//---------------------------------------------------------------------------
//�C�x���g�������Ɏ��s����R�}���h
UnicodeString OnAppStart;
UnicodeString OnAppClose;
UnicodeString OnMaximized;
UnicodeString OnMinimized;
UnicodeString OnRestored;
UnicodeString OnCurChange;
UnicodeString OnTabChange;
UnicodeString OnNewDrive;
UnicodeString OnArcOpend;
UnicodeString OnArcClosed;
UnicodeString OnFindOpend;
UnicodeString OnGrepOpen;
UnicodeString OnGrepClosed;
UnicodeString OnWatchNotify;
UnicodeString OnFTPConnect;
UnicodeString OnFTPDiscon;
UnicodeString OnFlCursor;
UnicodeString OnFlExDClick;
UnicodeString OnFlRClick;
UnicodeString OnDragStart;
UnicodeString OnDragEnd;
UnicodeString OnFlTbRClick;
UnicodeString OnTabDClick;
UnicodeString OnTabRClick;
UnicodeString OnDirDClick;
UnicodeString OnDirRClick;
UnicodeString OnRelDClick;
UnicodeString OnRelRClick;
UnicodeString OnHdrRClick;
UnicodeString OnFScrRClick;
UnicodeString OnDrvDClick;
UnicodeString OnDrvRClick;
UnicodeString OnSplDClick;
UnicodeString OnSplRClick;
UnicodeString OnImgDClick;
UnicodeString OnTxtDClick;
UnicodeString OnFlIDClick;
UnicodeString OnTskDClick;
UnicodeString OnTskRClick;
UnicodeString OnLogDClick;
UnicodeString OnSttDClick;
UnicodeString OnSttRClick;
UnicodeString OnTimDClick;
UnicodeString OnTimRClick;
UnicodeString OnTvOpen;
UnicodeString OnTvOpened;
UnicodeString OnTvClose;
UnicodeString OnTvClosed;
UnicodeString OnTvTbRClick;
UnicodeString OnTvHRClick;
UnicodeString OnIvOpened;
UnicodeString OnIvClosed;
UnicodeString OnFullScr;
UnicodeString OnNormScr;
UnicodeString OnIvTbRClick;
UnicodeString OnIvImgDClick;
UnicodeString OnIvImgRClick;
UnicodeString OnIvMgnDClick;
UnicodeString OnThmDClick;
UnicodeString OnThmRClick;
UnicodeString OnIvIDClick;
UnicodeString OnIvSbRClick;
UnicodeString OnDragStartI;
UnicodeString OnDragEndI;
UnicodeString OnClipText;
UnicodeString OnUnlocked;

const event_rec EventCmdList[MAX_EVENT_CMD] = {
	{&OnAppStart,		_T("OnAppStart"),	_T("NyanFi �N����")},
	{&OnAppClose,		_T("OnAppClose"),	_T("NyanFi �I����")},
	{&OnMaximized,		_T("OnMaximized"),	_T("�E�B���h�E���ő剻���ꂽ")},
	{&OnMinimized,		_T("OnMinimized"),	_T("�E�B���h�E���ŏ������ꂽ")},
	{&OnRestored,		_T("OnRestored"),	_T("�E�B���h�E�����̃T�C�Y�ɖ߂���")},
	{&OnCurChange,		_T("OnCurChange"),	_T("|�J�����g�̃f�B���N�g���ύX(FL)")},
	{&OnTabChange,		_T("OnTabChange"),	_T("�ʂ̃^�u�Ɉڂ���(FL)")},
	{&OnNewDrive,		_T("OnNewDrive"),	_T("�ǉ��h���C�u���J���ꂽ(FL)")},
	{&OnArcOpend,		_T("OnArcOpend"),	_T("���z�f�B���N�g�����J��������(FL)")},
	{&OnArcClosed,		_T("OnArcClosed"),	_T("���z�f�B���N�g�����������(FL)")},
	{&OnFindOpend,		_T("OnFindOpend"),	_T("�������ʃ��X�g���\�����ꂽ����(FL)")},
	{&OnFlCursor,		_T("OnFlCursor"),	_T("�t�@�C�����X�g�ŃJ�[�\���ړ�����(FL)")},
	{&OnFlExDClick,		_T("OnFlExDClick"),	_T("�t�@�C�����X�g�Ńt�@�C�����啔�ȊO���_�u���N���b�N(FL)")},
	{&OnFlRClick,		_T("OnFlRClick"),	_T("�t�@�C�����X�g�ŉE�N���b�N(FL)")},
	{&OnDragStart,		_T("OnDragStart"),	_T("�t�@�C�����X�g����̃h���b�O�J�n��(FL)")},
	{&OnDragEnd,		_T("OnDragEnd"),	_T("�t�@�C�����X�g����̃h���b�O������(FL)")},
	{&OnGrepOpen,		_T("OnGrepOpen"),	_T("�����񌟍�(GREP)���J�����O(FL,TV)")},
	{&OnGrepClosed,		_T("OnGrepClosed"),	_T("�����񌟍�(GREP)���������(FL)")},
	{&OnWatchNotify,	_T("OnWatchNotify"),_T("WatchTail�R�}���h�ŒǋL���o")},
	{&OnFTPConnect,		_T("OnFTPConnect"),	_T("|FTP�z�X�g�ɐڑ�����(FL)")},
	{&OnFTPDiscon,		_T("OnFTPDiscon"),	_T("FTP�z�X�g����ؒf����(FL)")},
	{&OnFlTbRClick,		_T("OnFlTbRClick"),	_T("|�t�@�C���[�̃c�[���o�[���E�N���b�N(FL)")},
	{&OnTabDClick,		_T("OnTabDClick"),	_T("�^�u�o�[�̋󂫃X�y�[�X���_�u���N���b�N(FL)")},
	{&OnTabRClick,		_T("OnTabRClick"),	_T("�^�u�o�[���E�N���b�N(FL)")},
	{&OnDirDClick,		_T("OnDirDClick"),	_T("�f�B���N�g�������_�u���N���b�N(FL)")},
	{&OnDirRClick,		_T("OnDirRClick"),	_T("�f�B���N�g�������E�N���b�N(FL)")},
	{&OnRelDClick,		_T("OnRelDClick"),	_T("�f�B���N�g���֌W���_�u���N���b�N(FL)")},
	{&OnRelRClick,		_T("OnRelRClick"),	_T("�f�B���N�g���֌W���E�N���b�N(FL)")},
	{&OnHdrRClick,		_T("OnHdrRClick"),	_T("�t�@�C�����X�g�̃w�b�_���E�N���b�N(FL)")},
	{&OnFScrRClick,		_T("OnFScrRClick"),	_T("�t�@�C�����X�g�̃V���v���X�N���[���o�[���E�N���b�N(FL)")},
	{&OnDrvDClick,		_T("OnDrvDClick"),	_T("�h���C�u�����_�u���N���b�N(FL)")},
	{&OnDrvRClick,		_T("OnDrvRClick"),	_T("�h���C�u�����E�N���b�N(FL)")},
	{&OnSplDClick,		_T("OnSplDClick"),	_T("���E�������E�����_�u���N���b�N(FL)")},
	{&OnSplRClick,		_T("OnSplRClick"),	_T("���E�������E�����E�N���b�N(FL)")},
	{&OnImgDClick,		_T("OnImgDClick"),	_T("�C���[�W�v���r���[���_�u���N���b�N(FL)")},
	{&OnTxtDClick,		_T("OnTxtDClick"),	_T("�e�L�X�g�v���r���[���_�u���N���b�N(FL)")},
	{&OnFlIDClick,		_T("OnFlIDClick"),	_T("�t�@�C�������_�u���N���b�N(FL)")},
	{&OnTskDClick,		_T("OnTskDClick"),	_T("�^�X�N�\�����_�u���N���b�N")},
	{&OnTskRClick,		_T("OnTskRClick"),	_T("�^�X�N�\�����E�N���b�N")},
	{&OnLogDClick,		_T("OnLogDClick"),	_T("���O�E�B���h�E���_�u���N���b�N")},
	{&OnSttDClick,		_T("OnSttDClick"),	_T("�X�e�[�^�X�o�[���_�u���N���b�N")},
	{&OnSttRClick,		_T("OnSttRClick"),	_T("�X�e�[�^�X�o�[���E�N���b�N")},
	{&OnTimDClick,		_T("OnTimDClick"),	_T("���v�p�l�����_�u���N���b�N")},
	{&OnTimRClick,		_T("OnTimRClick"),	_T("���v�p�l�����E�N���b�N")},
	{&OnTvOpen,			_T("OnTvOpen"),		_T("|�e�L�X�g�r���A�[���J�����O(FL)")},
	{&OnTvOpened,		_T("OnTvOpened"),	_T("�e�L�X�g�r���A�[���J��������(TV)")},
	{&OnTvClose,		_T("OnTvClose"),	_T("�e�L�X�g�r���A�[����钼�O(TV)")},
	{&OnTvClosed,		_T("OnTvClosed"),	_T("�e�L�X�g�r���A�[���������(FL)")},
	{&OnTvTbRClick,		_T("OnTvTbRClick"),	_T("�e�L�X�g�r���A�[�̃c�[���o�[���E�N���b�N(TV)")},
	{&OnTvHRClick,		_T("OnTvHRClick"),	_T("�e�L�X�g�r���A�[�ŏ��w�b�_���E�N���b�N(TV)")},
	{&OnIvOpened,		_T("OnIvOpened"),	_T("|�C���[�W�r���A�[���J��������(IV)")},
	{&OnIvClosed,		_T("OnIvClosed"),	_T("�C���[�W�r���A�[���������(FL)")},
	{&OnFullScr,		_T("OnFullScr"),	_T("�C���[�W�r���A�[�őS��ʕ\���ɂ���(IV)")},
	{&OnNormScr,		_T("OnNormScr"),	_T("�C���[�W�r���A�[�Œʏ�\���ɖ߂���(IV)")},
	{&OnIvTbRClick,		_T("OnIvTbRClick"),	_T("�C���[�W�r���A�[�̃c�[���o�[���E�N���b�N(IV)")},
	{&OnIvImgDClick,	_T("OnIvImgDClick"),_T("�C���[�W�r���A�[�ŉ摜�������_�u���N���b�N(IV)")},
	{&OnIvImgRClick,	_T("OnIvImgRClick"),_T("�C���[�W�r���A�[�ŉ摜�\�������E�N���b�N(IV)")},
	{&OnIvMgnDClick,	_T("OnIvMgnDClick"),_T("�C���[�W�r���A�[�ŗ]���������_�u���N���b�N(IV)")},
	{&OnThmDClick,		_T("OnThmDClick"),	_T("�C���[�W�r���A�[�ŃT���l�C�����_�u���N���b�N(IV)")},
	{&OnThmRClick,		_T("OnThmRClick"),	_T("�C���[�W�r���A�[�ŃT���l�C�����E�N���b�N(IV)")},
	{&OnIvIDClick,		_T("OnIvIDClick"),	_T("�C���[�W�r���A�[�Ńt�@�C�������_�u���N���b�N(IV)")},
	{&OnIvSbRClick,		_T("OnIvSbRClick"),	_T("�C���[�W�r���A�[�ŃV�[�N�o�[���E�N���b�N(IV)")},
	{&OnDragStartI,		_T("OnDragStartI"),	_T("�T���l�C������̃h���b�O�J�n��(IV)")},
	{&OnDragEndI,		_T("OnDragEndI"),	_T("�T���l�C������̃h���b�O������(IV)")},
	{&OnClipText,		_T("OnClipText"),	_T("|�N���b�v�{�[�h�Ƀe�L�X�g���R�s�[")},
	{&OnUnlocked,		_T("OnUnlocked"),	_T("�L�[�{�[�h/�}�E�X�̃��b�N���������ꂽ(FL)")}
};

bool Timer_Enabled[MAX_TIMER_EVENT], Timer_NopAct[MAX_TIMER_EVENT];
int  Timer_StartCnt[MAX_TIMER_EVENT], Timer_TimeCnt[MAX_TIMER_EVENT], Timer_NopCnt[MAX_TIMER_EVENT];
int  Timer_RepeatN[MAX_TIMER_EVENT], Timer_RepeatCnt[MAX_TIMER_EVENT];
TStringList  *Timer_AlarmList[MAX_TIMER_EVENT];
UnicodeString Timer_Condition[MAX_TIMER_EVENT];
UnicodeString OnTimerEvent[MAX_TIMER_EVENT];

//---------------------------------------------------------------------------
//TStringList ���쐬���AGeneralList �ɓo�^(�I�����Ɉꊇ�j��)
//---------------------------------------------------------------------------
TStringList *CreStringList(int obj_type)
{
	TStringList *lp = new TStringList();
	GeneralList->AddObject(IntToStr(obj_type), (TObject*)lp);
	return lp;
}

//---------------------------------------------------------------------------
//������
//---------------------------------------------------------------------------
void InitializeGlobal()
{
	Randomize();

	UserModule->ExePath = ExePath;

	LibraryPath = cv_env_str("%APPDATA%\\Microsoft\\Windows\\Libraries\\");

	//�o�[�W����
	unsigned mj, mi, bl;
	VersionNo = GetProductVersion(Application->ExeName, mj, mi, bl)? mj*100 + mi*10 + bl : 0;
	VersionStr.sprintf(_T("V%.2f"), VersionNo/100.0);

	ScrScale = Screen->PixelsPerInch / 96.0;	//�X�P�[�����O��ݒ�
	SIcoSize = ScaledInt(16);
	Scaled1  = ScaledInt(1);
	Scaled2  = ScaledInt(2);
	Scaled3  = ScaledInt(3);
	Scaled4  = ScaledInt(4);
	Scaled8  = ScaledInt(8);

	//���[�U��
	_TCHAR szName[256];
	DWORD size = 256;
	if (::GetUserName(szName, &size)) UserName = szName;

	//�Ǘ��Ҍ����������Ă��邩?
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	PSID admin_group;
	BOOL res = ::AllocateAndInitializeSid(
					&NtAuthority, 2,
					SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
					&admin_group);
	if (res) {
		if (!CheckTokenMembership(NULL, admin_group, &res)) res = FALSE;
		::FreeSid(admin_group);
	}
	IsAdmin = res;

	WIC_get_ex_list(NULL, &WicFextStr);

	mute_Volume("GET");	//�~���[�g��Ԃ��擾

	//�p�~�Z�N�V�����A�L�[�̍폜�A�C��
	IniFile->DeleteKey( SCT_General, "MemoFormBgCol");		//v13.54
	IniFile->DeleteKey( SCT_General, "MemoFormfgCol");
	IniFile->DeleteKey( SCT_Option,  "SelColDrvInf");						//v13.53
	IniFile->ReplaceKey(SCT_General, "ShowGifViewer",	"ShowSubViewer");	//v13.52
	IniFile->ReplaceKey(SCT_General, "GifViewerLeft",	"SubViewerLeft");
	IniFile->ReplaceKey(SCT_General, "GifViewerTop",	"SubViewerTop");
	IniFile->ReplaceKey(SCT_General, "GifViewerWidth",	"SubViewerWidth");
	IniFile->ReplaceKey(SCT_General, "GifViewerHeight",	"SubViewerHeight");
	IniFile->ReplaceKey(SCT_General, "GifViewBgCol",	"SubViewerBgCol");

	IniFile->DeleteKey(SCT_General, "GitViewFExtColor");	//v13.41
	IniFile->DeleteKey(SCT_General, "MarkListShowOpt");		//v13.24
	IniFile->DeleteKey(SCT_General, "EditHistShowOpt");		//v13.24

	if (IniFile->KeyExists(SCT_Option,	"ShowIcon")) {		//v13.17
		IniFile->WriteBool(SCT_Option,	"IconMode", IniFile->ReadBool(SCT_Option, "ShowIcon")? 1 : 0);
		IniFile->DeleteKey(SCT_Option,	"ShowIcon");
	}


	CurStt = &ListStt[CurListTag];
	OppStt = &ListStt[OppListTag];

	for (int i=0; i<MAX_BGIMAGE; i++) BgImgBuff[i] = new Graphics::TBitmap();

	GeneralList = new TStringList();

	for (int i=0; i<MAX_FILELIST; i++) {
		FileList[i] 	 = CreStringList(GENLST_FILELIST);
		ArcFileList[i]	 = CreStringList(GENLST_FILELIST);
		AdsFileList[i]	 = CreStringList(GENLST_FILELIST);
		ResultList[i]	 = CreStringList(GENLST_FILELIST);
		TmpBufList[i]	 = CreStringList(GENLST_FILELIST);
		SelMaskList[i]	 = CreStringList();

		IniPathMode[i]	 = 0;
		SortMode[i] 	 = 1;
		DirSortMode[i] 	 = 0;

		flist_stt *sp	 = &ListStt[i];
		sp->clu_size	 = 0;
		sp->drive_Used	 = 0;
		sp->drive_Free	 = 0;
		sp->drive_Total  = 0;
		sp->cur_total	 = -1;
		sp->occ_total	 = -1;
		sp->cmp_total	 = -1;
		sp->f_cnt		 = 0;
		sp->d_cnt		 = 0;
		sp->sel_f_cnt	 = 0;
		sp->sel_d_cnt	 = 0;
		sp->sel_size	 = 0;

		sp->show_f_d_cnt = false;
		sp->dir_graph	 = false;
		sp->dsk_graph	 = false;
		sp->sub_counted	 = false;

		sp->lwd_half	 = 8;
		sp->lwd_fext_max = 32;
		sp->lwd_fext	 = 32;
		sp->lwd_size	 = 100;
		sp->lwd_time	 = 100;
		sp->lwd_path	 = 0;

		sp->lxp_fext	 = 0;
		sp->lxp_size	 = 0;
		sp->lxp_time	 = 0;

		sp->color_bgDirInf = col_None;
		sp->color_fgDirInf = col_None;
		sp->color_bgDrvInf = col_None;
		sp->color_fgDrvInf = col_None;

		sp->is_TabFixed	 = false;

		sp->is_Work 	 = false;
		sp->is_FTP		 = false;
		sp->is_Arc		 = false;
		sp->arc_RetList  = CreStringList();
		sp->arc_TmpList  = CreStringList();

		sp->find_SubList = CreStringList();
		sp->find_ResLink = false;
		sp->find_DirLink = false;
		sp->find_PathSort= false;
		sp->is_IncSea	 = false;
		sp->is_Migemo	 = false;
		sp->is_Filter	 = false;
		sp->filter_sens  = false;
		clear_FindStt(sp);
	}

	for (int i=0; i<MAX_TIMER_EVENT; i++) {
		Timer_Enabled[i]   = false;
		Timer_AlarmList[i] = CreStringList();
	}

	ViewFileList	  = CreStringList(GENLST_FILELIST);
	WorkList		  = CreStringList(GENLST_FILELIST);
	LaunchList		  = CreStringList(GENLST_FILELIST);

	GeneralIconList	  = CreStringList(GENLST_ICON);
	CachedIcoList	  = CreStringList(GENLST_ICON);
	MenuBtnIcoList	  = CreStringList(GENLST_ICON);
	UsrIcoList		  = CreStringList(GENLST_ICON);
	DrvIcoList		  = CreStringList(GENLST_ICON);

	FontList		  = CreStringList(GENLST_FONT);
	CmdFileList 	  = CreStringList(GENLST_CMDSLIST);
	TabList 		  = CreStringList(GENLST_TABLIST);
	TabBuff			  = CreStringList();
	DriveInfoList	  = CreStringList(GENLST_DRIVE);
	DriveLogList	  = CreStringList();
	WatchTailList	  = CreStringList();
	InvalidUncList	  = CreStringList();
	PlayList		  = CreStringList();
	XCMD_VarList	  = CreStringList();
	BakSetupList	  = CreStringList();
	SyncDirList 	  = CreStringList();
	AssRenList		  = CreStringList();
	DistrDefList	  = CreStringList();
	GrepPathList	  = CreStringList();
	GrepFileList	  = CreStringList();
	GrepResultList	  = CreStringList();
	GrepResultBuff	  = CreStringList();
	KeyFuncList 	  = CreStringList();
	FKeyLabelList	  = CreStringList();
	HotKeyList		  = CreStringList();
	DirStack		  = CreStringList();
	RegDirList		  = CreStringList();
	ProtectDirList	  = CreStringList();
	VirDriveList	  = CreStringList();
	PathMaskList	  = CreStringList();
	ColorList		  = CreStringList();
	ExtColList		  = CreStringList();
	AssociateList	  = CreStringList();
	OpenStdCmdList	  = CreStringList();
	EtcEditorList	  = CreStringList();
	ExtToolList 	  = CreStringList();
	ExtMenuList 	  = CreStringList();
	DriveList		  = CreStringList();
	NetDriveList	  = CreStringList();
	AllDirHistory	  = CreStringList();
	TextViewHistory   = CreStringList();
	TextEditHistory   = CreStringList();
	WorkListHistory   = CreStringList();
	InputDirHistory   = CreStringList();
	InputCmdsHistory  = CreStringList();
	InputCmdsHistoryV = CreStringList();
	InputCmdsHistoryI = CreStringList();
	IncSeaHistory	  = CreStringList();
	FilterHistory	  = CreStringList();
	WebSeaHistory	  = CreStringList();
	LatLngHistory	  = CreStringList();
	HeadlineList	  = CreStringList();
	RenCmdFileList	  = CreStringList();
	RenArcFileList	  = CreStringList();
	RedrawList		  = CreStringList();
	CmdRequestList	  = CreStringList();
	CommandHistory	  = CreStringList();
	PopMenuList 	  = CreStringList();
	ToolBtnList 	  = CreStringList();
	ToolBtnListV	  = CreStringList();
	ToolBtnListI	  = CreStringList();
	CnvCharList 	  = CreStringList();
	LogBufList		  = CreStringList();
	HideInfItems	  = CreStringList();

	GitCfgUrlList	  = CreStringList();
	GitInfList		  = CreStringList();

	//�t�H���_�A�C�R����`
	FolderIconFile = new UsrIniFile(ExePath + FICO_INI_FILE);
	FolderIconList = CreStringList();
	FolderIconFile->ReadSection("FolderIcon", FolderIconList);
	DefFldIcoName  = FolderIconFile->ReadString(SCT_Option, "DefFldIcoName");

	//�����N�}�[�N�̎擾
	_TCHAR sdir[MAX_PATH] = {};
	if (::GetSystemDirectory(sdir, MAX_PATH)>0) {
		HICON icons[1];
		UnicodeString fnam = IncludeTrailingPathDelimiter(sdir) + "Shell32.dll";
		if (::ExtractIconEx(fnam.c_str(), 29, NULL, icons, 1)==1) hLinkIcon = icons[0];
	}

	//�R�}���h���X�g���쐬
	CommandList = CreStringList();
	CmdSetList	= CreStringList();
	set_CmdList(CommandList, CmdSetList);

	//�Y�[���{�����X�g
	ZoomRatioList = CreStringList();
	ZoomRatioList->Text = "10\n25\n50\n75\n100\n150\n200\n300\n400\n";

	//�^�X�N�R�}���h�����X�g���쐬
	TaskCmdList = CreStringList();
	TaskCmdList->Text =
		"CPY=�R�s�[\n"
		"MOV=�ړ�\n"
		"DEL=�폜\n"
		"CPYDIR=�f�B���N�g���̃R�s�[\n"
		"CMPDEL=���S�폜\n"
		"CVIMG=�摜�ϊ�\n"
		"DLEXIF=Exif�폜\n"
		"EXTCHM=���o\n"
		"BACKUP=�o�b�N�A�b�v\n"
		"DTIME=�^�C���ݒ�\n"
		"DCOMP=���k�ݒ�\n";

	//�^�O
	usr_TAG = new TagManager(ExePath + TAGDATA_FILE);

	//----------------------------------------------
	//�I�v�V�����̃L�[�ƕϐ��Ƃ̑Ή���o�^���ēǍ�
	//----------------------------------------------
	OptionList = CreStringList();

	struct {
		const _TCHAR  *def;	//�L�[��=�f�t�H���g
		TObject *vp;		//�ϐ��ւ̃|�C���^
	} opt_def_list[] = {
		//[Option]
		//String (�f�t�H���g�l�����p���ň͂�Ŏw��)
		{_T("TextEditor=\"\""),						(TObject*)&TextEditor},
		{_T("TextEditorFrmt=\"$F\""),				(TObject*)&TextEditorFrmt},
		{_T("ExtTxViewer=\"\""),					(TObject*)&ExtTxViewer},
		{_T("ExtTxViewerFrmt=\"$F\""),				(TObject*)&ExtTxViewerFrmt},
		{_T("ImageEditor=\"\""),					(TObject*)&ImageEditor},
		{_T("FExtImgEidt=\".jpg.gif.png.bmp\""),	(TObject*)&FExtImgEidt},
		{_T("BinaryEditor=\"\""),					(TObject*)&BinaryEditor},
		{_T("CmdGitExer=\"\""),						(TObject*)&CmdGitExe},
		{_T("GitBashExe=\"\""),						(TObject*)&GitBashExe},
		{_T("GitGuiExe=\"\""),						(TObject*)&GitGuiExe},
		{_T("FExtViewTab4=\".cpp.cxx.c.h\""),		(TObject*)&FExtViewTab4},
		{_T("FExtViewTabX=\"\""),					(TObject*)&FExtViewTabX},
		{_T("SoundTaskFin=\"\""),					(TObject*)&SoundTaskFin},
		{_T("SoundFindFin=\"\""),					(TObject*)&SoundFindFin},
		{_T("SoundWarning=\"\""),	 				(TObject*)&SoundWarning},
		{_T("SoundWatch=\"\""),	 					(TObject*)&SoundWatch},
		{_T("EmpBinPtn1=\"\""),						(TObject*)&EmpBinPtn1},
		{_T("EmpBinPtn2=\"\""),						(TObject*)&EmpBinPtn2},
		{_T("EmpBinPtn3=\"\""),						(TObject*)&EmpBinPtn3},
		{_T("HtmInsHrCls=\"post\""),				(TObject*)&HtmInsHrCls},
		{_T("HtmHdrStr=\"��;��;��;��;��;��\""),		(TObject*)&HtmHdrStr},
		{_T("HtmDelBlkCls=\"\""),					(TObject*)&HtmDelBlkCls},
		{_T("HtmDelBlkId=\"\""),					(TObject*)&HtmDelBlkId},
		{_T("IniSeaShift=\"F:Ctrl+\""),				(TObject*)&IniSeaShift},
		{_T("AutoRenFmt=\"\\N_\\SN(1)\""),			(TObject*)&AutoRenFmt},
		{_T("FExtExeFile=\".exe.bat.cmd\""),		(TObject*)&FExtExeFile},
		{_T("CallHotKey=\"\""),						(TObject*)&CallHotKey},
		{_T("AppListHotKey=\"\""),					(TObject*)&AppListHotKey},
		{_T("AppListHotPrm=\"\""),					(TObject*)&AppListHotPrm},
		{_T("NoWatchPath=\"\""),					(TObject*)&NoWatchPath},
		{_T("FExtGetInf=\"\""),						(TObject*)&FExtGetInf},
		{_T("FExtNoInf=\"\""),						(TObject*)&FExtNoInf},
		{_T("NoInfPath=\"\""),						(TObject*)&NoInfPath},
		{_T("EmpInfItems=\"\""),					(TObject*)&EmpInfItems},
		{_T("FExtImgPrv=\"\""),						(TObject*)&FExtImgPrv},
		{_T("FExtNoImgPrv=\"\""),					(TObject*)&FExtNoImgPrv},
		{_T("NoImgPrvPath=\"\""),					(TObject*)&NoImgPrvPath},
		{_T("FExtNoIView=\"\""),					(TObject*)&FExtNoIView},
		{_T("MarkImgPath=\"\""),					(TObject*)&MarkImgPath},
		{_T("MarkImgFExt=\".jpg\""),				(TObject*)&MarkImgFExt},
		{_T("MarkImgMemo=\"������\""),				(TObject*)&MarkImgMemo},

#if defined(_WIN64)
		{_T("FExt7zDll=\".lzh.cab.iso.arj.chm.msi.wim\""),
													(TObject*)&FExt7zDll},
#else
		{_T("FExt7zDll=\".arj.chm.msi.wim\""),		(TObject*)&FExt7zDll},
#endif

		{_T("SttBarFmt=\"$F\""),					(TObject*)&SttBarFmt},
		{_T("SttClockFmt=\"\""),					(TObject*)&SttClockFmt},
		{_T("DrvInfFmtR=\"$VN    $US Use    $FS Free($FR) \""),
													(TObject*)&DrvInfFmtR},
		{_T("DrvInfFmtS=\"$AD $SD Directories    $AF $SF Files Selected    $SZ \""),
													(TObject*)&DrvInfFmtS},
		{_T("DrvInfFmtN=\"$VN    $US Use    $FS Free($FR) \""),
													(TObject*)&DrvInfFmtN},
		{_T("WebSeaUrl=\"https://www.google.co.jp/search?q=\\S&ie=UTF-8\""),
													(TObject*)&WebSeaUrl},
		{_T("DlgMoveShift=\"Alt+\""),				(TObject*)&DlgMoveShift},
		{_T("DlgSizeShift=\"\""),					(TObject*)&DlgSizeShift},
		{_T("WheelCmdF0=\"ScrollUp/ScrollDown\""),	(TObject*)&WheelCmdF[0]},
		{_T("WheelCmdF1=\"ZoomIn/ZoomOut\""),		(TObject*)&WheelCmdF[1]},
		{_T("WheelCmdF2=\"\""),						(TObject*)&WheelCmdF[2]},
		{_T("WheelCmdF3=\"\""),						(TObject*)&WheelCmdF[3]},
		{_T("WheelCmdV0=\"ScrollUp/ScrollDown\""),	(TObject*)&WheelCmdV[0]},
		{_T("WheelCmdV1=\"ZoomIn/ZoomOut\""),		(TObject*)&WheelCmdV[1]},
		{_T("WheelCmdV2=\"PageUp/PageDown\""),		(TObject*)&WheelCmdV[2]},
		{_T("WheelCmdV3=\"\""),						(TObject*)&WheelCmdV[3]},
		{_T("WheelCmdI0=\"PrevFile/NextFile\""),	(TObject*)&WheelCmdI[0]},
		{_T("WheelCmdI1=\"ZoomIn/ZoomOut\""),		(TObject*)&WheelCmdI[1]},
		{_T("WheelCmdI2=\"ScrollUp/ScrollDown\""),	(TObject*)&WheelCmdI[2]},
		{_T("WheelCmdI3=\"ScrollLeft/ScrollRight\""),	(TObject*)&WheelCmdI[3]},
		{_T("WheelBtnCmdF=\"ZoomReset\""),			(TObject*)&WheelBtnCmdF[0]},
		{_T("WheelBtnCmdF1=\"\""),					(TObject*)&WheelBtnCmdF[1]},
		{_T("WheelBtnCmdF2=\"\""),					(TObject*)&WheelBtnCmdF[2]},
		{_T("WheelBtnCmdF3=\"\""),					(TObject*)&WheelBtnCmdF[3]},
		{_T("WheelBtnCmdV=\"ZoomReset\""),			(TObject*)&WheelBtnCmdV},
		{_T("WheelBtnCmdI=\"FittedSize\""),			(TObject*)&WheelBtnCmdI},
		{_T("X1BtnCmdF=\"BackDirHist\""),			(TObject*)&X1BtnCmdF},
		{_T("X2BtnCmdF=\"\""),						(TObject*)&X2BtnCmdF},
		{_T("X1BtnCmdV=\"\""),						(TObject*)&X1BtnCmdV},
		{_T("X2BtnCmdV=\"\""),						(TObject*)&X2BtnCmdV},
		{_T("X1BtnCmdI=\"\""),						(TObject*)&X1BtnCmdI},
		{_T("X2BtnCmdI=\"\""),						(TObject*)&X2BtnCmdI},
		{_T("MigemoPath=\"\""),						(TObject*)&MigemoPath},
		{_T("SpiDir=\"\""),							(TObject*)&SpiDir},
		{_T("BgImgName=\"\""),						(TObject*)&BgImgName[0]},
		{_T("BgImgName2=\"\""),						(TObject*)&BgImgName[1]},
		{_T("SpImgName=\"\""),						(TObject*)&BgImgName[2]},
		{_T("SpImgName2=\"\""),						(TObject*)&BgImgName[3]},
		{_T("KnobImgName=\"\""),					(TObject*)&BgImgName[4]},
		{_T("NoDirHistPath=\"\""),					(TObject*)&NoDirHistPath},
		{_T("NoEditHistPath=\"\""),					(TObject*)&NoEditHistPath},
		{_T("NoViewHistPath=\"\""),					(TObject*)&NoViewHistPath},
		{_T("DirDelimiter=\"/\""),					(TObject*)&DirDelimiter},
		{_T("FTPTextModeFExt=\".txt.htm.html.shtm.shtml.css.cgi.log.pl.sh.rb.js.c.cpp.h\""),
													(TObject*)&FTPTextModeFExt},
		{_T("FTPSndConnect=\"\""),					(TObject*)&FTPSndConnect},
		{_T("FTPSndDiscon=\"\""),					(TObject*)&FTPSndDiscon},
		{_T("FTPSndTransfer=\"\""),					(TObject*)&FTPSndTransfer},

		{_T("TabPinMark=\"\""),						(TObject*)&TabPinMark},		//�B���ݒ�
		{_T("HEAD_Mark=\"\""),						(TObject*)&HEAD_Mark},		//�B���ݒ�
		{_T("PLAY_Mark=\"\""),						(TObject*)&PLAY_Mark},		//�B���ݒ�

		// default = ExePath
		{_T("DownloadPath=\"%ExePath%\""),			(TObject*)&DownloadPath},
		{_T("WorkListPath=\"%ExePath%\""),			(TObject*)&WorkListPath},
		{_T("ResultListPath=\"%ExePath%\""),		(TObject*)&ResultListPath},
		{_T("RefParamPath=\"%ExePath%\""),			(TObject*)&RefParamPath},
		{_T("IconFilePath=\"%ExePath%\""),			(TObject*)&UserModule->IconFilePath},
		{_T("SaveTxtPath=\"%ExePath%\""),			(TObject*)&SaveTxtPath},
		{_T("CmdFilePath=\"%ExePath%\""),			(TObject*)&CmdFilePath},

		//Integer  (�f�t�H���g�l�����p���ň͂܂��Ɏw��)
		{_T("MaxTasks=4"),					(TObject*)&MaxTasks},
		{_T("FlashCntWarning=0"),			(TObject*)&FlashCntWarning},
		{_T("FlashTimeWarning=0"),			(TObject*)&FlashTimeWarning},
		{_T("FlashCntTaskfin=0"),			(TObject*)&FlashCntTaskfin},
		{_T("FlashTimeTaskfin=0"),			(TObject*)&FlashTimeTaskfin},
		{_T("MaxLogLines=1000"),			(TObject*)&MaxLogLines},
		{_T("MsgHintTime=1000"),			(TObject*)&MsgHintTime},
		{_T("KeyHintTime=1000"),			(TObject*)&KeyHintTime},
		{_T("NotifyTaskTime=10"),			(TObject*)&NotifyTaskTime},
		{_T("ViewTxtInterLn=4"),			(TObject*)&ViewTxtInterLn},
		{_T("ViewLeftMargin=10"),			(TObject*)&ViewLeftMargin},
		{_T("ViewFoldWidth=80"),	 		(TObject*)&ViewFoldWidth},
		{_T("ViewFixedLimit=0"),			(TObject*)&ViewFixedLimit},
		{_T("ViewTabWidthX=2"),				(TObject*)&ViewTabWidthX},
		{_T("ViewTxtLimitSize=262144"),		(TObject*)&ViewTxtLimitSize},
		{_T("ViewWheelScrLn=3"),			(TObject*)&ViewWheelScrLn},
		{_T("ListWheelScrLn=2"),			(TObject*)&ListWheelScrLn},
		{_T("PlaySndLimitTime=3"),			(TObject*)&PlaySndLimitTime},
		{_T("CmpDelOwCnt=3"),				(TObject*)&CmpDelOwCnt},
		{_T("PrvTxtInfLn=1000"),			(TObject*)&PrvTxtInfLn},
		{_T("PrvTxtTailLn=100"),			(TObject*)&PrvTxtTailLn},
		{_T("PrvActTailLn=0"),				(TObject*)&PrvActTailLn},
		{_T("WatchInterval=1000"),			(TObject*)&WatchInterval},
		{_T("LogInterval=250"),				(TObject*)&LogInterval},
		{_T("InitialModeI=0"),				(TObject*)&InitialModeI},
		{_T("LastZoomRatio=100"),			(TObject*)&LastZoomRatio},
		{_T("RemoteWaitTime=200"),			(TObject*)&RemoteWaitTime},
		{_T("NormalWaitTime=2"),			(TObject*)&NormalWaitTime},
		{_T("NopDtctTime=10"),				(TObject*)&NopDtctTime},
		{_T("TimeTolerance=2000"),			(TObject*)&TimeTolerance},
		{_T("DlgMovePrm=16"),				(TObject*)&DlgMovePrm},
		{_T("DlgSizePrm=16"),				(TObject*)&DlgSizePrm},
		{_T("BorderMoveWidth=50"),			(TObject*)&BorderMoveWidth},
		{_T("OpenByMode=0"),				(TObject*)&OpenByMode},
		{_T("DblClickFlMode=0"),			(TObject*)&DblClickFlMode},
		{_T("IncSeaMigemoMin=1"),			(TObject*)&IncSeaMigemoMin},
		{_T("DefDropMode=0"),				(TObject*)&DefDropMode},
		{_T("ListInterLn=4"),				(TObject*)&ListInterLn},
		{_T("CursorWidth=2"),				(TObject*)&CursorWidth},
		{_T("CursorAlpha=0"),				(TObject*)&CursorAlpha},
		{_T("CellAlpha=64"),				(TObject*)&CellAlpha},
		{_T("SplitterWidth=4"),				(TObject*)&SplitterWidth},
		{_T("SplitterWidth2=4"),			(TObject*)&SplitterWidth2},
		{_T("SizeFormatMode=0"),			(TObject*)&SizeFormatMode},
		{_T("SizeDecDigits=1"),				(TObject*)&SizeDecDigits},
		{_T("ListPercent=50"),				(TObject*)&ListPercent},
		{_T("ImgFrameMargin=0"),			(TObject*)&ImgFrameMargin},
		{_T("ImgFitMaxZoom=100"),			(TObject*)&ImgFitMaxZoom},
		{_T("ImgGridHorzN=3"),				(TObject*)&ImgGridHorzN},
		{_T("ImgGridVertN=3"),				(TObject*)&ImgGridVertN},
		{_T("MinShowTime=0"),				(TObject*)&MinShowTime},
		{_T("ThumbnailSize=120"),			(TObject*)&ThumbnailSize},
		{_T("ThumbBdrWidth=3"),				(TObject*)&ThumbBdrWidth},
		{_T("ImgDblMargin=0"),				(TObject*)&ImgDblMargin},
		{_T("WicScaleOpt=0"),				(TObject*)&WicScaleOpt},
		{_T("MaxLogFiles=5"),				(TObject*)&MaxLogFiles},
		{_T("BatLowerLimit=50"),			(TObject*)&BatLowerLimit},
		{_T("ScrBarStyle=0"),				(TObject*)&ScrBarStyle},
		{_T("IconMode=0"),					(TObject*)&IconMode},
		{_T("ModalScrAlpha=64"),			(TObject*)&ModalScrAlpha},
		{_T("TlWinBorderWidth=1"),			(TObject*)&TlWinBorderWidth},
		{_T("BgImgMode=0"),					(TObject*)&BgImgMode},
		{_T("BgImgSubMode=0"),				(TObject*)&BgImgSubMode},
		{_T("BgColAlpha=0"),				(TObject*)&BgColAlpha},
		{_T("BgHideTime=1000"),				(TObject*)&BgHideTime},
		{_T("BgTileSize=100"),				(TObject*)&BgTileSize},
		{_T("AlphaValue=255"),				(TObject*)&AlphaValue},
		{_T("FlTabWidth=100"),				(TObject*)&FlTabWidth},
		{_T("FlTabStyle=0"),				(TObject*)&FlTabStyle},
		{_T("IconCache=500"),				(TObject*)&IconCache},
		{_T("FTPRemoteSide=0"),				(TObject*)&FTPRemoteSide},
		{_T("FTPDisconTimeout=300"),		(TObject*)&FTPDisconTimeout},
		{_T("ProtectDirMode=0"),			(TObject*)&ProtectDirMode},
		{_T("FindPathWidth=150"),			(TObject*)&FindPathWidth},
		{_T("FindTagsWidth=150"),			(TObject*)&FindTagsWidth},
		{_T("KeyboardMode=0"),				(TObject*)&KeyboardMode},

		//Bool (�f�t�H���g�l�� true �܂��� false �Ŏw��)
		{_T("UseSpiFirst=false"),			(TObject*)&UseSpiFirst},
		{_T("LastMigemoMode=false"),		(TObject*)&LastMigemoMode},
		{_T("LastMigemoModeF=false"),		(TObject*)&LastMigemoModeF},
		{_T("DivFileListUD=false"),			(TObject*)&DivFileListUD},
		{_T("DivDirInfUD=true"),			(TObject*)&DivDirInfUD},
		{_T("ViewFoldFitWin=true"),			(TObject*)&ViewFoldFitWin},
		{_T("TvCursorVisible=false"),		(TObject*)&TvCursorVisible},
		{_T("LimitBinCsr=true"),			(TObject*)&LimitBinCsr},
		{_T("TxtColorHint=true"),			(TObject*)&TxtColorHint},
		{_T("AltBackSlash=false"),			(TObject*)&AltBackSlash},
		{_T("DecodeDfmStr=false"),			(TObject*)&DecodeDfmStr},
		{_T("BinMemMaped=false"),			(TObject*)&BinMemMaped},
		{_T("MultiInstance=false"),			(TObject*)&MultiInstance},
		{_T("CloseOthers=false"),			(TObject*)&CloseOthers},
		{_T("StoreTaskTray=false"),			(TObject*)&StoreTaskTray},
		{_T("ShowDirType=false"),	 		(TObject*)&ShowDirType},
		{_T("ShowSpace=false"),		 		(TObject*)&ShowSpace},
		{_T("UseIndIcon=true"),		 		(TObject*)&UseIndIcon},
		{_T("ShowHideAtr=false"),	 		(TObject*)&ShowHideAtr},
		{_T("ShowSystemAtr=false"),			(TObject*)&ShowSystemAtr},
		{_T("DispRegName=false"),			(TObject*)&DispRegName},
		{_T("UncToNetDrive=false"),			(TObject*)&UncToNetDrive},
		{_T("PathInTitleBar=false"),		(TObject*)&PathInTitleBar},
		{_T("OmitEndOfName=false"),			(TObject*)&OmitEndOfName},
		{_T("ShowAdsInf=false"),			(TObject*)&ShowAdsInf},
		{_T("ShowUseProcInf=false"),		(TObject*)&ShowUseProcInf},
		{_T("ShowDirJumboIco=true"),		(TObject*)&ShowDirJumboIco},
		{_T("ShowInZipImg=false"),			(TObject*)&ShowInZipImg},
		{_T("PreviewAniGif=false"),			(TObject*)&PreviewAniGif},
		{_T("SetPrvCursor=true"),			(TObject*)&SetPrvCursor},
		{_T("ForceDel=false"),				(TObject*)&ForceDel},
		{_T("RemoveCdReadOnly=false"),		(TObject*)&RemoveCdReadOnly},
		{_T("CopyTags=false"),				(TObject*)&CopyTags},
		{_T("CopyNoBuffering=false"),		(TObject*)&CopyNoBuffering},
		{_T("ShowArcCopyProg=false"),		(TObject*)&ShowArcCopyProg},
		{_T("DelUseTrash=false"),			(TObject*)&DelUseTrash},
		{_T("EditNewText=false"),			(TObject*)&EditNewText},
		{_T("ViewArcInf=true"),				(TObject*)&ViewArcInf},
		{_T("PreviewTxtToInf=true"),		(TObject*)&ShowTextPreview},
		{_T("ShowTailPreview=false"),		(TObject*)&ShowTailPreview},
		{_T("ReloadOnActive=true"),			(TObject*)&ReloadOnActive},
		{_T("OpenAddedDrive=false"),		(TObject*)&OpenAddedDrive},
		{_T("CheckUnc=true"),				(TObject*)&CheckUnc},
		{_T("NotShowNoTask=false"),			(TObject*)&NotShowNoTask},
		{_T("ShowMsgHint=false"),	 		(TObject*)&ShowMsgHint},
		{_T("ShowKeyHint=false"),			(TObject*)&ShowKeyHint},
		{_T("ShowNoKeyHint=true"),			(TObject*)&ShowNoKeyHint},
		{_T("ShowCopyHint=true"),			(TObject*)&ShowCopyHint},
		{_T("ShowDuplNotify=true"),			(TObject*)&ShowDuplNotify},
		{_T("ShowTooltip=true"),			(TObject*)&ShowTooltip},
		{_T("LogErrOnly=false"),			(TObject*)&LogErrOnly},
		{_T("LogErrMsg=true"),				(TObject*)&LogErrMsg},
		{_T("LogDebugInf=false"),			(TObject*)&LogDebugInf},
		{_T("LogHideSkip=false"),			(TObject*)&LogHideSkip},
		{_T("LogFullPath=false"),			(TObject*)&LogFullPath},
		{_T("SaveLog=true"),				(TObject*)&SaveLog},
		{_T("AppendLog=false"),				(TObject*)&AppendLog},
		{_T("NotSortWorkList=false"),	 	(TObject*)&NotSortWorkList},
		{_T("RegWorkCsrPos=false"),		 	(TObject*)&RegWorkCsrPos},
		{_T("AutoDelWorkList=true"),	 	(TObject*)&AutoDelWorkList},
		{_T("NoCheckWorkUnc=false"),	 	(TObject*)&NoCheckWorkUnc},
		{_T("NoCheckRecentUnc=false"),		(TObject*)&NoCheckRecentUnc},
		{_T("NoCheckDirHist=false"),		(TObject*)&NoCheckDirHist},
		{_T("ExtSaveDirHist=false"),		(TObject*)&ExtSaveDirHist},
		{_T("DirHistCsrPos=true"),	 		(TObject*)&DirHistCsrPos},
		{_T("DelDplDirHist=false"),			(TObject*)&DelDplDirHist},
		{_T("WorkToDirHist=false"),			(TObject*)&WorkToDirHist},
		{_T("LoopFilerCursor=false"),		(TObject*)&LoopFilerCursor},
		{_T("FlCursorVisible=false"),		(TObject*)&FlCursorVisible},
		{_T("AutoCompComboBox=true"),		(TObject*)&AutoCompComboBox},
		{_T("RestoreComboBox=false"),		(TObject*)&RestoreComboBox},
		{_T("DialogCenter=false"),			(TObject*)&DialogCenter},
		{_T("SelectByMouse=false"),			(TObject*)&SelectByMouse},
		{_T("SelectBaseOnly=false"),		(TObject*)&SelectBaseOnly},
		{_T("SelectIconSngl=false"),		(TObject*)&SelectIconSngl},
		{_T("ShowLineNo=true"),				(TObject*)&ShowLineNo},
		{_T("ShowLineCursor=true"),			(TObject*)&ShowLineCursor},
		{_T("ShowTAB=true"),				(TObject*)&ShowTAB},
		{_T("ShowCR=true"),					(TObject*)&ShowCR},
		{_T("ShowTextRuler=true"),			(TObject*)&ShowTextRuler},
		{_T("ScrBarToFoldPos=true"),		(TObject*)&ScrBarToFoldPos},
		{_T("TxtSttIsBottom=false"),		(TObject*)&TxtSttIsBottom},
		{_T("ChkAozora=false"),				(TObject*)&ChkAozora},
		{_T("UseXd2tx=true"),				(TObject*)&UseXd2tx},
		{_T("ClickableUrl=true"),			(TObject*)&ClickableUrl},
		{_T("RestoreViewLine=true"),		(TObject*)&RestoreViewLine},
		{_T("EmpComment=true"),				(TObject*)&EmpComment},
		{_T("EmpStrings=true"),				(TObject*)&EmpStrings},
		{_T("EmpReserved=true"),			(TObject*)&EmpReserved},
		{_T("EmpSymbol=true"),				(TObject*)&EmpSymbol},
		{_T("EmpNumeric=true"),				(TObject*)&EmpNumeric},
		{_T("EmpHeadline=true"),			(TObject*)&EmpHeadline},
		{_T("EmpRuby=true"),				(TObject*)&EmpRuby},
		{_T("ToMarkdown=false"),			(TObject*)&ToMarkdown},
		{_T("HtmInsHrSct=false"),			(TObject*)&HtmInsHrSct},
		{_T("HtmInsHrArt=false"),			(TObject*)&HtmInsHrArt},
		{_T("HtmInsHrNav=false"),			(TObject*)&HtmInsHrNav},
		{_T("RotViewImg=true"),				(TObject*)&RotViewImg},
		{_T("KeepZoomRatio=false"),			(TObject*)&KeepZoomRatio},
		{_T("ShowThumbName=false"),			(TObject*)&ShowThumbName},
		{_T("ShowThumbExif=false"),			(TObject*)&ShowThumbExif},
		{_T("ShowThumbTags=false"),			(TObject*)&ShowThumbTags},
		{_T("ShowThumbFExt=false"),			(TObject*)&ShowThumbFExt},
		{_T("NotThumbIfArc=true"),			(TObject*)&NotThumbIfArc},
		{_T("LoopViewCursor=false"),		(TObject*)&LoopViewCursor},
		{_T("HintTopEndImg=false"),			(TObject*)&HintTopEndImg},
		{_T("BeepTopEndImg=false"),			(TObject*)&BeepTopEndImg},
		{_T("ImgSttIsBottom=false"),		(TObject*)&ImgSttIsBottom},
		{_T("SeekBindDir=false"),			(TObject*)&SeekBindDir},
		{_T("SeekSwapNxtPrv=false"),		(TObject*)&SeekSwapNxtPrv},
		{_T("HideCsrInFull=true"),			(TObject*)&HideCsrInFull},
		{_T("HideThumbInFull=false"),		(TObject*)&HideThumbInFull},
		{_T("AnimateGif=false"),			(TObject*)&AnimateGif},
		{_T("ShowThumbScroll=true"),		(TObject*)&ShowThumbScroll},
		{_T("MarkImgClose=false"),			(TObject*)&MarkImgClose},
		{_T("TimColEnabled=false"),			(TObject*)&TimColEnabled},
		{_T("PriorFExtCol=false"),			(TObject*)&PriorFExtCol},
		{_T("ColorOnlyFExt=false"),			(TObject*)&ColorOnlyFExt},
		{_T("SymColorToName=false"),		(TObject*)&SymColorToName},
		{_T("RevTagCololr=false"),			(TObject*)&RevTagCololr},
		{_T("ShowMainMenu=true"),			(TObject*)&ShowMainMenu},
		{_T("ShowImgPreview=true"),			(TObject*)&ShowImgPreview},
		{_T("ShowProperty=true"),			(TObject*)&ShowProperty},
		{_T("ShowLogWin=true"),				(TObject*)&ShowLogWin},
		{_T("ShowFileListOnly=false"),		(TObject*)&ShowFileListOnly},
		{_T("ShowFileListOnly2=false"),		(TObject*)&ShowFileListOnly2},
		{_T("ShowSttBar=true"),				(TObject*)&ShowSttBar},
		{_T("ShowToolBar=false"),			(TObject*)&ShowToolBar},
		{_T("ShowToolBarV=false"),			(TObject*)&ShowToolBarV},
		{_T("ShowToolBarI=false"),			(TObject*)&ShowToolBarI},
		{_T("ToolBarISide=false"),			(TObject*)&ToolBarISide},
		{_T("ShowTabBar=false"),			(TObject*)&ShowTabBar},
		{_T("ShowFKeyBar=false"),			(TObject*)&ShowFKeyBar},
		{_T("ShowClsTabBtn=true"),			(TObject*)&ShowClsTabBtn},
		{_T("ShowPopTabBtn=true"),			(TObject*)&ShowPopTabBtn},
		{_T("ShowPopDirBtn=true"),			(TObject*)&ShowPopDirBtn},
		{_T("HideScrBar=false"),			(TObject*)&HideScrBar},
		{_T("ShowByteSize=false"),			(TObject*)&ShowByteSize},
		{_T("ShowTargetInf=false"),			(TObject*)&ShowTargetInf},
		{_T("ShowHeader=false"),			(TObject*)&ShowHeader},
		{_T("FixListWidth=false"),			(TObject*)&FixListWidth},
		{_T("KeepOnResize=false"),			(TObject*)&KeepOnResize},
		{_T("KeepCurListWidth=false"),		(TObject*)&KeepCurListWidth},
		{_T("NoSpaceFExt=false"),			(TObject*)&NoSpaceFExt},
		{_T("HideTitleMenu=false"),			(TObject*)&HideTitleMenu},
		{_T("FlatInfPanel=false"),			(TObject*)&FlatInfPanel},
		{_T("MenuAutoHotkey=false"),		(TObject*)&MenuAutoHotkey},
		{_T("EscapeHelp=false"),			(TObject*)&EscapeHelp},
		{_T("InhbitAltMenu=false"),			(TObject*)&InhbitAltMenu},
		{_T("SureCopy=false"),				(TObject*)&SureCopy},
		{_T("SureMove=false"),				(TObject*)&SureMove},
		{_T("SureDelete=true"),				(TObject*)&SureDelete},
		{_T("SureUnPack=false"),			(TObject*)&SureUnPack},
		{_T("SureExec=false"),				(TObject*)&SureExec},
		{_T("SureOther=true"),				(TObject*)&SureOther},
		{_T("SureExit=false"),				(TObject*)&SureExit},
		{_T("SureCmpDel=true"),				(TObject*)&SureCmpDel},
		{_T("SureWorkList=true"),			(TObject*)&SureWorkList},
		{_T("SureCancel=false"),			(TObject*)&SureCancel},
		{_T("SureDefNo=false"),				(TObject*)&SureDefNo},
		{_T("SureAdjPos=false"),			(TObject*)&SureAdjPos},
		{_T("WarnPowerFail=false"),			(TObject*)&WarnPowerFail},
		{_T("WarnLowBattery=false"),		(TObject*)&WarnLowBattery},
		{_T("WarnDisconnect=false"),		(TObject*)&WarnDisconnect},
		{_T("AppListChgMin=false"),			(TObject*)&AppListChgMin},
		{_T("CreDirChg=true"),				(TObject*)&CreDirChg},
		{_T("CreDirCnvChar=false"),			(TObject*)&CreDirCnvChar},
		{_T("OpenOnlyCurEdit=false"),		(TObject*)&OpenOnlyCurEdit},
		{_T("DontClrSelEdit=false"),		(TObject*)&DontClrSelEdit},
		{_T("OpenDirByStd=false"),			(TObject*)&OpenDirByStd},
		{_T("OpenOnlyCurApp=false"),		(TObject*)&OpenOnlyCurApp},
		{_T("DontClrSelApp=false"),			(TObject*)&DontClrSelApp},
		{_T("OpenOnlyCurWin=false"),		(TObject*)&OpenOnlyCurWin},
		{_T("DontClrSelWin=false"),			(TObject*)&DontClrSelWin},
		{_T("IniSeaByNum=false"),	 		(TObject*)&IniSeaByNum},
		{_T("IniSeaBySign=false"),			(TObject*)&IniSeaBySign},
		{_T("IncSeaCaseSens=false"),		(TObject*)&IncSeaCaseSens},
		{_T("IncSeaLoop=true"),				(TObject*)&IncSeaLoop},
		{_T("IncSeaMatch1Exit=false"),		(TObject*)&IncSeaMatch1Exit},
		{_T("OpenStdTabGroup=false"),		(TObject*)&OpenStdTabGroup},
		{_T("OpenStdMenuFile=false"),		(TObject*)&OpenStdMenuFile},
		{_T("OpenStdResultList=false"),		(TObject*)&OpenStdResultList},
		{_T("DownAfterOpenStd=false"),		(TObject*)&DownAfterOpenStd},
		{_T("OpenStdOnResList=false"),		(TObject*)&OpenStdOnResList},
		{_T("SyncLR=false"),				(TObject*)&SyncLR},
		{_T("ModalScreen=false"),			(TObject*)&ModalScreen},
		{_T("BgImgGray=false"),				(TObject*)&BgImgGray},
		{_T("BgImgHideScr=false"),			(TObject*)&BgImgHideScr},
		{_T("BgImgTileIf=false"),			(TObject*)&BgImgTileIf},
		{_T("AlphaForm=false"),				(TObject*)&AlphaForm},
		{_T("ProtectSubDir=true"),			(TObject*)&ProtectSubDir},
		{_T("ProtectFile=false"),			(TObject*)&ProtectFile},
		{_T("ImageEditSgl=false"),			(TObject*)&ImageEditSgl},
		{_T("FTPDlKeepTime=false"),			(TObject*)&FTPDlKeepTime},
		{_T("FTPUpKeepTime=false"),			(TObject*)&FTPUpKeepTime},
		{_T("FTPUpToLower=false"),			(TObject*)&FTPUpToLower},
		{_T("FTPLogResponse=false"),		(TObject*)&FTPLogResponse},
		{_T("FindPathColumn=false"),		(TObject*)&FindPathColumn},
		{_T("FindTagsColumn=false"),		(TObject*)&FindTagsColumn},
		{_T("FindTagsSort=false"),			(TObject*)&usr_TAG->SortTags},

		//[General] (prefix = U:)
		{_T("U:LastCurTag=0"),				(TObject*)&LastCurTag},
		{_T("U:TabGroupName=\"\""),			(TObject*)&TabGroupName},
		{_T("U:SortMode1=1"),				(TObject*)&SortMode[0]},
		{_T("U:SortMode2=1"),				(TObject*)&SortMode[1]},
		{_T("U:DirSortMode1=0"),			(TObject*)&DirSortMode[0]},
		{_T("U:DirSortMode2=0"),			(TObject*)&DirSortMode[1]},
		{_T("U:IniPathMode1=0"),			(TObject*)&IniPathMode[0]},
		{_T("U:IniPathMode2=0"),			(TObject*)&IniPathMode[1]},
		{_T("U:IniSortMode1=0"),			(TObject*)&IniSortMode[0]},
		{_T("U:IniSortMode2=0"),			(TObject*)&IniSortMode[1]},
		{_T("U:InitialPath1=\"\""),			(TObject*)&InitialPath[0]},
		{_T("U:InitialPath2=\"\""),			(TObject*)&InitialPath[1]},
		{_T("U:InitialMask1=\"\""),			(TObject*)&InitialMask[0]},
		{_T("U:InitialMask2=\"\""),			(TObject*)&InitialMask[1]},
		{_T("U:IniWorkMode=0"),				(TObject*)&IniWorkMode},
		{_T("U:HomeWorkList=\"WORKLIST.nwl\""),	(TObject*)&HomeWorkList},
		{_T("U:IniWinMode=0"),				(TObject*)&IniWinMode},
		{_T("U:FixWinPos=false"),			(TObject*)&FixWinPos},
		{_T("U:IniPathToTab1=true"),		(TObject*)&IniPathToTab1},
		{_T("U:IniTabHomeAll=false"),		(TObject*)&IniTabHomeAll},
		{_T("U:ShowSplash=true"),			(TObject*)&ShowSplash},
		{_T("U:IniWinWidth=800"),			(TObject*)&IniWinWidth},
		{_T("U:IniWinHeight=600"),			(TObject*)&IniWinHeight},
		{_T("U:IniWinLeft=50"),				(TObject*)&IniWinLeft},
		{_T("U:IniWinTop=50"),				(TObject*)&IniWinTop},
		{_T("U:LayoutMode=0"),				(TObject*)&LayoutMode},
		{_T("U:SaveEncIndex=0"),			(TObject*)&SaveEncIndex},
		{_T("U:CopyAll=false"),				(TObject*)&gCopyAll},
		{_T("U:CopyMode=0"),				(TObject*)&gCopyMode},
		{_T("U:CopyMode2=0"),				(TObject*)&gCopyMode2},

		{_T("U:FlOdrNatural1=true"),		(TObject*)&FlOdrNatural[0]},
		{_T("U:FlOdrNatural2=true"),		(TObject*)&FlOdrNatural[1]},
		{_T("U:FlOdrDscName1=false"),		(TObject*)&FlOdrDscName[0]},
		{_T("U:FlOdrDscName2=false"),		(TObject*)&FlOdrDscName[1]},
		{_T("U:FlOdrSmall1=false"),			(TObject*)&FlOdrSmall[0]},
		{_T("U:FlOdrSmall2=false"),			(TObject*)&FlOdrSmall[1]},
		{_T("U:FlOdrOld1=false"),			(TObject*)&FlOdrOld[0]},
		{_T("U:FlOdrOld2=false"),			(TObject*)&FlOdrOld[1]},
		{_T("U:FlOdrDscAttr1=false"),		(TObject*)&FlOdrDscAttr[0]},
		{_T("U:FlOdrDscAttr2=false"),		(TObject*)&FlOdrDscAttr[1]},
		{_T("U:FlOdrDscPath1=false"),		(TObject*)&FlOdrDscPath[0]},
		{_T("U:FlOdrDscPath2=false"),		(TObject*)&FlOdrDscPath[1]},

		{_T("U:SortBoth=false"),			(TObject*)&SortBoth},
		{_T("U:ShowImgSidebar=false"),		(TObject*)&ShowImgSidebar},
		{_T("U:ImgSidebarIsLeft=true"),		(TObject*)&ImgSidebarIsLeft},
		{_T("U:ThumbnailPos=1"),			(TObject*)&ThumbnailPos},
		{_T("U:ShowHistogram=false"),		(TObject*)&ShowHistogram},
		{_T("U:ShowLoupe=false"),			(TObject*)&ShowLoupe},
		{_T("U:ShowSubViewer=false"),		(TObject*)&ShowSubViewer},
		{_T("U:ShowSeekBar=false"),			(TObject*)&ShowSeekBar},
		{_T("U:WarnHighlight=false"),		(TObject*)&WarnHighlight},
		{_T("U:DoublePage=false"),			(TObject*)&DoublePage},
		{_T("U:RightBind=true"),			(TObject*)&RightBind},
		{_T("U:PermitDotCmds=false"),		(TObject*)&PermitDotCmds},
		{_T("U:InheritDotNyan=false"),		(TObject*)&InheritDotNyan},
		{_T("U:DotNyanPerUser=false"),		(TObject*)&DotNyanPerUser},
		{_T("U:AddToRecent=false"),			(TObject*)&AddToRecent},

		//[Grep] (prefix = G:)
		{_T("G:OutMode=0"),					(TObject*)&GrepOutMode},
		{_T("G:OutFileName=\"\""),			(TObject*)&GrepFileName},
		{_T("G:OutAppName=\"\""),			(TObject*)&GrepAppName},
		{_T("G:OutAppParam=\"\""),			(TObject*)&GrepAppParam},
		{_T("G:OutAppDir=\"\""),			(TObject*)&GrepAppDir},
		{_T("G:GrepAppend=false"),			(TObject*)&GrepAppend},
		{_T("G:FileFmt=\"$F $L:\""),		(TObject*)&GrepFileFmt},
		{_T("G:InsStrW=\"\""),				(TObject*)&GrepInsStrW},
		{_T("G:InsStrW2=\"\""),				(TObject*)&GrepInsStrW2},
		{_T("G:TremLeft=true"),				(TObject*)&GrepTrimLeft},
		{_T("G:ReplaceTab=true"),			(TObject*)&GrepReplaceTab},
		{_T("G:ReplaceCr=true"),			(TObject*)&GrepReplaceCr},
		{_T("G:RepCrStr=\" �^ \""),			(TObject*)&GrepRepCrStr},
		{_T("G:BackupReplace=true"),		(TObject*)&BackupReplace},
		{_T("G:FExtRepBackup=\".bak\""),	(TObject*)&FExtRepBackup},
		{_T("G:RepBackupDir=\"\""),			(TObject*)&RepBackupDir},
		{_T("G:ReplaceLogName=\"\""),		(TObject*)&ReplaceLogName},
		{_T("G:SaveReplaceLog=false"),		(TObject*)&SaveReplaceLog},
		{_T("G:ReplaceAppend=false"),		(TObject*)&ReplaceAppend},
		{_T("G:OpenReplaceLog=false"),		(TObject*)&OpenReplaceLog},
		{_T("G:ShowItemNo=false"),			(TObject*)&GrepShowItemNo},
		{_T("G:FileItemNo=false"),			(TObject*)&GrepFileItemNo},
		{_T("G:ShowSubDir=true"),			(TObject*)&GrepShowSubDir},
		{_T("G:TrimTop=false"),				(TObject*)&GrepTrimTop},
		{_T("G:OmitTop=true"),				(TObject*)&GrepOmitTop},
		{_T("G:EmFilter=false"),			(TObject*)&GrepEmFilter},
		{_T("G:AdjNextLn=true"),			(TObject*)&GrepAdjNextLn},

		//�Z�N�V���� (prefix = S:)
		{_T("S:KeyFuncList="),				(TObject*)KeyFuncList},
		{_T("S:FKeyLabelList="),			(TObject*)FKeyLabelList},
		{_T("S:Associate="),				(TObject*)AssociateList},
		{_T("S:OpenStdCmd="),	 			(TObject*)OpenStdCmdList},
		{_T("S:EtcEditor="),				(TObject*)EtcEditorList},
		{_T("S:HeadlineList="),				(TObject*)HeadlineList},
		{_T("S:CnvCharList="),				(TObject*)CnvCharList},
		{_T("S:Color="),					(TObject*)ColorList},
		{_T("S:BakSetupList="),				(TObject*)BakSetupList},
		{_T("S:CustomColors="),				(TObject*)UserModule->ColorDlg->CustomColors},
		{_T("S:TagColList="),				(TObject*)usr_TAG->TagColList},
		{_T("S:HideInfItems="),				(TObject*)HideInfItems},
		{_T("S:GitInfList="),				(TObject*)GitInfList},
		{_T("S:VirDriveList="),				(TObject*)VirDriveList},

		//���X�g	(prefix = L:)	�ő區�ڐ�,���p�����O��
		{_T("L:DirStack=30,false"),			(TObject*)DirStack},
		{_T("L:TabList=30,false"),			(TObject*)TabList},
		{_T("L:PathMaskList=30,false"),		(TObject*)PathMaskList},
		{_T("L:RegDirList=50,false"),		(TObject*)RegDirList},
		{_T("L:ProtectDirList=0,true"),		(TObject*)ProtectDirList},
		{_T("L:SyncDirList=50,false"),		(TObject*)SyncDirList},
		{_T("L:AssRenList=30,false"),		(TObject*)AssRenList},
		{_T("L:DistrDefList=200,false"),	(TObject*)DistrDefList},
		{_T("L:ExtColList=0,true"),			(TObject*)ExtColList},
		{_T("L:ExtMenuList=100,false"),		(TObject*)ExtMenuList},
		{_T("L:ExtToolList=100,false"),		(TObject*)ExtToolList},
		{_T("L:ToolBtnList=0,false"),		(TObject*)ToolBtnList},
		{_T("L:ToolBtnListV=0,false"),		(TObject*)ToolBtnListV},
		{_T("L:ToolBtnListI=0,false"),		(TObject*)ToolBtnListI},
		{_T("L:TextViewHistory=50,true"),	(TObject*)TextViewHistory},
		{_T("L:TextEditHistory=50,true"),	(TObject*)TextEditHistory},
		{_T("L:WorkListHistory=50,true"),	(TObject*)WorkListHistory},
		{_T("L:InpDirHistory=50,true"),		(TObject*)InputDirHistory},
		{_T("L:InpCmdsHistory=30,true"),	(TObject*)InputCmdsHistory},
		{_T("L:InpCmdsHistoryV=20,true"),	(TObject*)InputCmdsHistoryV},
		{_T("L:InpCmdsHistoryI=20,true"),	(TObject*)InputCmdsHistoryI},
		{_T("L:IncSeaHistory=50,true"),		(TObject*)IncSeaHistory},
		{_T("L:FilterHistory=50,true"),		(TObject*)FilterHistory},
		{_T("L:WebSeaHistory=30,true"),		(TObject*)WebSeaHistory},
		{_T("L:LatLngHistory=100,true"),	(TObject*)LatLngHistory}
	};

	int cnt = sizeof(opt_def_list)/sizeof(opt_def_list[0]);
	for (int i=0; i<cnt; i++) OptionList->AddObject(opt_def_list[i].def, (TObject*)opt_def_list[i].vp);
	//�ǂݍ���
	LoadOptions();

	//git.exe �̃`�F�b�N
	std::unique_ptr<TStringList> plst(new TStringList());
	TStringDynArray elst = split_strings_semicolon(GetEnvironmentVariable("PATH"));
	for (int i=0; i<elst.Length; i++) {
		if (ContainsText(elst[i], "\\Git\\")) plst->Add(elst[i]);
	}

#if defined(_WIN64)
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES%")) + "Git");
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES%")) + "Git\\bin");
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES%")) + "Git\\cmd");
#else
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMW6432%")) + "Git");
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMW6432%")) + "Git\\bin");
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMW6432%")) + "Git\\cmd");
#endif

	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES(X86)%")) + "Git\\bin");
	plst->Add(IncludeTrailingPathDelimiter(cv_env_var("%PROGRAMFILES(X86)%")) + "Git\\cmd");
	if (CmdGitExe.IsEmpty()) {
		for (int i=0; i<plst->Count; i++) {
			UnicodeString xnam = IncludeTrailingPathDelimiter(plst->Strings[i]) + "git.exe";
			if (file_exists(xnam)) { CmdGitExe = xnam;  break; }
		}
	}

	if (GitBashExe.IsEmpty()) {
		for (int i=0; i<plst->Count; i++) {
			UnicodeString xnam = IncludeTrailingPathDelimiter(plst->Strings[i]) + "git-bash.exe";
			if (file_exists(xnam)) { GitBashExe = xnam;  break; }
		}
	}
	if (GitGuiExe.IsEmpty()) {
		for (int i=0; i<plst->Count; i++) {
			UnicodeString xnam = IncludeTrailingPathDelimiter(plst->Strings[i]) + "git-gui.exe";
			if (file_exists(xnam)) { GitGuiExe = xnam;  break; }
		}
	}
	GitExists = file_exists(CmdGitExe);

	//�c�[���`�b�v�̕\��
	Application->ShowHint = ShowTooltip;

	//�f�t�H���g�̃L�[�ݒ�
	if (KeyFuncList->Count==0) {
		KeyFuncList->Text =
			"F:A=SelAllFile\n"
			"F:B=BackDirHist\n"
			"F:C=Copy\n"
			"F:D=Delete\n"
			"F:E=FileEdit\n"
			"F:F=IncSearch\n"
			"F:G=ImageViewer\n"
			"F:H=DirHistory\n"
			"F:I=CalcDirSize\n"
			"F:J=RegDirDlg\n"
			"F:K=CreateDir\n"
			"F:L=DriveList\n"
			"F:M=Move\n"
			"F:O=CurrFromOpp\n"
			"F:P=Pack\n"
			"F:Q=Exit\n"
			"F:R=RenameDlg\n"
			"F:S=SortDlg\n"
			"F:U=UnPack\n"
			"F:V=TextViewer\n"
			"F:W=WorkList\n"
			"F:X=ContextMenu\n"
			"F:ESC=TaskMan\n"
			"F:F3=FindFileDlg\n"
			"F:F5=ReloadList\n"
			"F:LEFT=ToLeft\n"
			"F:RIGHT=ToRight\n"
			"F:SPACE=Select\n"
			"F:Ctrl+Enter=OpenByApp\n"
			"F:Shift+O=CurrToOpp\n"
			"F:Shift+DOWN=CursorDownSel\n"
			"F:Shift+UP=CursorUpSel\n"
			"F:Shift+ENTER=OpenByWin\n"
			"V:B=ChangeViewMode\n"
			"V:F=FindText\n"
			"V:E=FileEdit\n"
			"V:Q=Close\n"
			"I:C=ClipCopy\n"
			"I:D=Delete\n"
			"I:E=FileEdit\n"
			"I:S=SortDlg\n"
			"I:SPACE=Select\n"
			"I:Q=Close\n"
			"L:ESC=TaskMan\n"
			"S:ENTER=IncSearchExit\n";
	}

	//�f�t�H���g�̃p�X�}�X�N
	if (PathMaskList->Count==0) {
		PathMaskList->Text =
			"A,\"�}�X�N������\",\"*\"\n"
			"X,\"���s�t�@�C��\",\"*.exe;*.com;*.bat;*.cmd\"\n";
	}

	//�֎~�����̃f�t�H���g
	if (CnvCharList->Count<9) {
		CnvCharList->Text =
			"\\=��\n"
			"/=�^\n"
			":=�F\n"
			"*=��\n"
			"?=�H\n"
			"\"=�h\n"
			"<=��\n"
			">=��\n"
			"|=�b\n";
	}

	//----------------------------------
	//���̑��̏�����
	//----------------------------------
	usr_SH = new UserShell(MainHandle);
	usr_SH->PropNameWidth = FPRP_NAM_WD;
	usr_SH->ScrScale = ScrScale;

	usr_ARC = new UserArcUnit(MainHandle);
	usr_ARC->FExt7zDll = FExt7zDll;
	usr_ARC->fpAddDebugLog = AddDebugLog;

	SPI = new SpiUnit(to_absolute_name(SpiDir));

	usr_Migemo = new MigemoUnit(to_absolute_name(MigemoPath));
	usr_Migemo->MinLength = IncSeaMigemoMin;

	TaskReserveList = new TaskConfigList();
	ClearNopStt();

	//�h���C�u�e�ʃ��O�̏�����
	DriveLogName = ExePath + DRVLOG_FILE;
	if (file_exists(DriveLogName) && load_text_ex(DriveLogName, DriveLogList)!=0)
		update_DriveLog(false);
	else
		DriveLogName = EmptyStr;

	//----------------------------------
	//HtmlHelp �̏�����
	//----------------------------------
	Application->HelpFile = ChangeFileExt(Application->ExeName, ".chm");
	hHHctrl = ::LoadLibrary(_T("hhctrl.ocx"));
	if (hHHctrl) {
		lpfHtmlHelp = (FUNC_HtmlHelp)::GetProcAddress(hHHctrl, "HtmlHelpW");
		if (lpfHtmlHelp) lpfHtmlHelp(NULL, NULL, HH_INITIALIZE, (DWORD)&Cookie);
	}

	//����JAPI
	HINSTANCE hGdi32 = ::LoadLibrary(_T("gdi32.dll"));
	if (hGdi32) {
		lpGetFontResourceInfo = (FUNC_GetFontResourceInfo)::GetProcAddress(hGdi32, "GetFontResourceInfoW");
	}

	WorkListChanged = WorkListFiltered = rqWorkListDirInf = false;
}

//---------------------------------------------------------------------------
//��Еt��
//---------------------------------------------------------------------------
void EndGlobal()
{
	::mciSendString(_T("close all"), NULL, 0, NULL);

	for (int i=0; i<GeneralList->Count; i++) {
		TStringList *lst = (TStringList*)GeneralList->Objects[i];

		switch (GeneralList->Strings[i].ToIntDef(0)) {
		case GENLST_FILELIST:
			clear_FileList(lst);
			break;
		case GENLST_DRIVE:
			for (int j=0; j<lst->Count; j++) {
				drive_info *dp = (drive_info*)lst->Objects[j];
				delete dp->small_ico;
				delete dp->large_ico;
				delete dp;
			}
			break;
		case GENLST_CMDSLIST:
			for (int j=0; j<lst->Count; j++) {
				cmdf_rec *cp = (cmdf_rec*)lst->Objects[j];
				delete cp->file_buf;
				delete cp->cmd_list;
				delete cp;
			}
			break;
		case GENLST_TABLIST:
			for (int j=0; j<lst->Count; j++) del_tab_info((tab_info*)lst->Objects[j]);
			break;
		case GENLST_ICON:
			for (int j=0; j<lst->Count; j++) delete (TIcon*)lst->Objects[j];
			break;
		case GENLST_FONT:
			for (int j=0; j<lst->Count; j++) delete (TFont*)lst->Objects[j];
			break;
		}

		delete lst;
	}
	delete GeneralList;

	delete FolderIconFile;
	if (hLinkIcon) ::DestroyIcon(hLinkIcon);

	for (int i=0; i<MAX_BGIMAGE; i++) delete BgImgBuff[i];

	delete usr_Migemo;
	delete SPI;
	delete usr_ARC;
	delete usr_TAG;
	delete usr_SH;

	delete TaskReserveList;

	if (hHHctrl) {
		if (lpfHtmlHelp) {
			lpfHtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0) ;
			lpfHtmlHelp(NULL, NULL, HH_UNINITIALIZE, (DWORD)Cookie);
		}
		::FreeLibrary(hHHctrl);
	}

	if (hGdi32) ::FreeLibrary(hGdi32);
}

//---------------------------------------------------------------------------
//�^�C�}�[�ݒ�̍X�V
//---------------------------------------------------------------------------
void SetupTimer()
{
	for (int i=0; i<MAX_TIMER_EVENT; i++) {
		Timer_Enabled[i] = Timer_NopAct[i] = false;
		if (Timer_Condition[i].IsEmpty()) continue;
		TStringDynArray prm_lst = get_csv_array(Timer_Condition[i], MAX_TIMER_ALARM);
		if (prm_lst.Length==0) continue;
		//����
		if (ContainsStr(prm_lst[0], ":")) {
			Timer_TimeCnt[i] = Timer_NopCnt[i] = 0;
			Timer_AlarmList[i]->Clear();
			for (int j=0; j<prm_lst.Length; j++) {
				if (prm_lst[j].IsEmpty()) continue;
				try {
					if (StartsStr("??:", prm_lst[j])) {
						UnicodeString mm = get_tkn_r(prm_lst[j], _T("??:"));
						for (int h=0; h<24; h++) {
							TDateTime dt = TTime(UnicodeString().sprintf(_T("%02u:%s"), h, mm.c_str())) + Date();
							if (CompareDateTime(Now(), dt)==GreaterThanValue) dt = IncDay(dt, 1);
							Timer_AlarmList[i]->Add(format_DateTime(dt));
						}
					}
					else {
						TDateTime dt = TTime(prm_lst[j]) + Date();
						if (CompareDateTime(Now(), dt)==GreaterThanValue) dt = IncDay(dt, 1);
						Timer_AlarmList[i]->Add(format_DateTime(dt));
					}
				}
				catch (EConvertError &e) {
					;
				}
			}

			if (Timer_AlarmList[i]->Count>0) {
				Timer_AlarmList[i]->Sort();
				Timer_Enabled[i] = true;
			}
		}
		//�Ԋu�A������
		else {
			UnicodeString prm = prm_lst[0];
			if (remove_top_s(prm, '!')) {
				Timer_TimeCnt[i] = 0;
				Timer_NopCnt[i]  = param_to_mSec(prm);
			}
			else {
				Timer_TimeCnt[i] = param_to_mSec(prm);
				Timer_NopCnt[i]  = 0;
			}

			if (Timer_TimeCnt[i]>0 || Timer_NopCnt[i]>0) {
				Timer_RepeatCnt[i] = Timer_RepeatN[i] = (prm_lst.Length>1)? prm_lst[1].ToIntDef(0) : 0;
				Timer_Enabled[i]   = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
//�������Ԃ̉���
//---------------------------------------------------------------------------
void ClearNopStt()
{
	LastOpCount = GetTickCount();
	for (int i=0; i<MAX_TIMER_EVENT; i++) Timer_NopAct[i] = false;
}

//---------------------------------------------------------------------------
//�I�v�V�����ݒ�̓ǂݍ���
//---------------------------------------------------------------------------
void LoadOptions()
{
	UnicodeString sct, tmp;

	//�o�^�I�v�V�����̓ǂݍ���
	TStringList *lp = OptionList;
	for (int i=0; i<lp->Count; i++) {
		UnicodeString key  = lp->Names[i];
		UnicodeString vbuf = lp->ValueFromIndex[i];
		//Section
		if (remove_top_text(key, _T("S:"))) {
			IniFile->ReadSection(key, (TStringList*)lp->Objects[i]);
		}
		//List
		else if (remove_top_text(key, _T("L:"))) {
			int  n = split_tkn(vbuf, ',').ToIntDef(20);
			bool q = USAME_TI(vbuf, "true");
			IniFile->LoadListItems(key, (TStringList*)lp->Objects[i], n, q);
		}
		else {
			sct = remove_top_text(key, _T("U:"))? SCT_General :
				  remove_top_text(key, _T("G:"))? UnicodeString("Grep") : SCT_Option;
			if (is_quot(vbuf)) {
				//UnicodeString
				vbuf = exclude_quot(vbuf);
				if (USAME_TI(vbuf, "%ExePath%"))
					*((UnicodeString*)lp->Objects[i]) = to_path_name(IniFile->ReadString(sct, key, ExePath));
				else
					*((UnicodeString*)lp->Objects[i]) = IniFile->ReadString(sct, key, vbuf);
			}
			else {
				//bool
				if (contained_wd_i(_T("true|false"), vbuf))
					*((bool*)lp->Objects[i]) = IniFile->ReadBool(sct, key, USAME_TI(vbuf, "true"));
				//int
				else
					*((int*)lp->Objects[i])  = IniFile->ReadInteger(sct, key, vbuf.ToIntDef(0));
			}
		}
	}

	//�}�[�N�̃f�t�H���g�ݒ�
	if (TabPinMark.IsEmpty()) TabPinMark = u"\U0001F4CD";
	if (HEAD_Mark.IsEmpty())  HEAD_Mark  = _T("\u25b6");
	if (PLAY_Mark.IsEmpty())  PLAY_Mark  = _T("\u25b6");

	//�^�u���X�g�̏�����
	for (int i=0; i<TabList->Count; i++) {
		tab_info *tp = new tab_info;
		for (int j=0; j<MAX_FILELIST; j++) {
			tp->sel_list[j]   = new TStringList();
			tp->dir_hist[j]   = new TStringList();
			tp->dir_hist_p[j] = 0;
			IniFile->LoadListItems(sct.sprintf(_T("DirHistory%02u_%u"), i + 1, j), tp->dir_hist[j], 30, false);
		}
		TabList->Objects[i] = (TObject*)tp;
	}

	//�S�̃f�B���N�g������
	if (ExtSaveDirHist) {
		AllDirHistory->Clear();
		UnicodeString fnam = ExePath + DIR_HIST_FILE;
		if (FileExists(fnam)) {
			try {
				std::unique_ptr<TStringList> fbuf(new TStringList());
				fbuf->LoadFromFile(fnam);
				int tag = 0;
				for (int i=0; i<fbuf->Count; i++) {
					UnicodeString lbuf = fbuf->Strings[i];
					//�Z�N�V����
					if (StartsStr('[', lbuf)) {
						tag = SameText(lbuf, "[AllDirHistory]")? 1 : 0;
					}
					//�L�[ (���ڔԍ��͖���)
					else if (tag==1 && StartsText("Item", lbuf)) {
						lbuf = get_tkn_r(lbuf, "=");
						if (!lbuf.IsEmpty()) AllDirHistory->Add(lbuf);
					}
				}
			}
			catch (...) {
				;
			}
		}
	}
	else {
		IniFile->LoadListItems("AllDirHistory", AllDirHistory, 100, false);
	}

	//���̑��̃I�v�V�����̓ǂݍ���
	sct = SCT_Option;
	TempPath  = to_path_name(IniFile->ReadString(sct, "TempPath", "Temp\\"));
	remove_top_s(TempPath, '\\');
	SetTempPathA(TempPath);

	if (MaxTasks<1 || MaxTasks>4) MaxTasks = 4;
	if (CmpDelOwCnt<=0) CmpDelOwCnt = 3;

	DirBraStr = IniFile->ReadString(sct, "DirBraStr", !IniFile->KeyExists(sct, "DirBraStr")? "[" : "");
	DirKetStr = IniFile->ReadString(sct, "DirKetStr", !IniFile->KeyExists(sct, "DirKetStr")? "]" : "");

	TimeStampFmt = IniFile->ReadString(sct, "TimeStampFmt",
		!IniFile->KeyExists(sct, "TimeStampFmt")? "yy/mm/dd hh:nn" : "");

	ViewBinLimitSize = IniFile->ReadInteger(sct, "ViewBinLimitSize", FAILED_BUF_SIZE);
	if (ViewBinLimitSize>MAX_MEMMAP_SIZE) ViewBinLimitSize = MAX_MEMMAP_SIZE;

	set_col_from_ColorList();

	//�C�x���g
	for (int i=0; i<MAX_EVENT_CMD; i++)
		*(EventCmdList[i].sp) = IniFile->ReadString("Event", EventCmdList[i].key);

	//�^�C�}�[�C�x���g
	sct = "TimerEvent";
	for (int i=0; i<MAX_TIMER_EVENT; i++) {
		UnicodeString key;
		Timer_Condition[i] = IniFile->ReadString(sct, key.sprintf(_T("Condition%u"), i + 1));
		OnTimerEvent[i]    = IniFile->ReadString(sct, key.sprintf(_T("OnTimer%u"), i + 1));
		Timer_TimeCnt[i]   = Timer_NopCnt[i] = Timer_RepeatCnt[i] = Timer_RepeatN[i] = 0;
	}
	SetupTimer();
}

//---------------------------------------------------------------------------
//�I�v�V�����ݒ�̕ۑ�
//---------------------------------------------------------------------------
void SaveOptions()
{
	//�f�B���N�g�������̐i�ޕ������폜
	for (int i=0; i<TabList->Count; i++) {
		for (int j=0; j<MAX_FILELIST; j++) {
			TStringList *h_lst = get_DirHistory(i, j);
			if (h_lst) {
				int *h_ptr = get_DirHistPtr(i, j);
				if (h_ptr) for (int k=0; k<*h_ptr && h_lst->Count>0; k++) h_lst->Delete(0);
			}
		}
	}

	UnicodeString sct;
	//�o�^�I�v�V����
	TStringList *lp = OptionList;
	for (int i=0; i<lp->Count; i++) {
		UnicodeString key  = lp->Names[i];
		UnicodeString vbuf = lp->ValueFromIndex[i];
		//Section
		if (remove_top_text(key, _T("S:"))) {
			IniFile->AssignSection(key, (TStringList*)lp->Objects[i]);
		}
		//List
		else if (remove_top_text(key, _T("L:"))) {
			IniFile->SaveListItems(key, (TStringList*)lp->Objects[i], get_tkn(vbuf, ',').ToIntDef(20));
		}
		else {
			sct = remove_top_text(key, _T("U:"))? SCT_General :
				  remove_top_text(key, _T("G:"))? UnicodeString("Grep") : SCT_Option;
			if (is_quot(vbuf)) {
				//UnicodeString
				IniFile->WriteString(sct, key,	*((UnicodeString*)lp->Objects[i]));
			}
			else {
				//bool
				if (contained_wd_i(_T("true|false"), vbuf))
					IniFile->WriteBool(sct, key,	*((bool*)lp->Objects[i]));
				//int
				else
					IniFile->WriteInteger(sct, key,	*((int*)lp->Objects[i]));
			}
		}
	}

	//�^�u���X�g
	for (int i=0; i<MAX_TABLIST; i++) {
		tab_info *tp = get_TabInfo(i);
		for (int j=0; j<MAX_FILELIST; j++) {
			sct.sprintf(_T("DirHistory%02u_%u"), i + 1, j);
			if (tp)
				IniFile->SaveListItems(sct, tp->dir_hist[j], 30);
			else
				IniFile->EraseSection(sct);
		}
	}

	//�S�̃f�B���N�g������
	if (ExtSaveDirHist) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		UnicodeString s;
		fbuf->Add("[AllDirHistory]");
		for (int i=0; i<AllDirHistory->Count; i++) {
			fbuf->Add(s.sprintf(_T("Item%u=%s"), i + 1, AllDirHistory->Strings[i].c_str()));
		}
		//�ۑ�
		UnicodeString fnam = ExePath + DIR_HIST_FILE;
		UnicodeString msg  = make_LogHdr(_T("SAVE"), fnam);
		if (saveto_TextUTF8(fnam, fbuf.get())) {
			IniFile->EraseSection("AllDirHistory");
		}
		else {
			UnicodeString errmsg = LoadUsrMsg(USTR_FaildSave, _T("�����t�@�C��"));
			msgbox_ERR(errmsg);
			set_LogErrMsg(msg, errmsg, fnam);
		}
		AddLog(msg);
	}
	else {
		IniFile->SaveListItems("AllDirHistory", AllDirHistory, 100);
	}

	//���̑��̃I�v�V����
	sct = SCT_Option;
	IniFile->WriteString( sct, "TempPath",		to_relative_name(TempPathA));
	IniFile->WriteString( sct, "DirBraStr",		DirBraStr);
	IniFile->WriteString( sct, "DirKetStr",		DirKetStr);
	IniFile->WriteString( sct, "TimeStampFmt",	TimeStampFmt);
	IniFile->WriteInteger(sct, "ViewBinLimitSize",	ViewBinLimitSize);

	//�C�x���g
	for (int i=0; i<MAX_EVENT_CMD; i++)
		IniFile->WriteString("Event", EventCmdList[i].key,	*(EventCmdList[i].sp));

	sct = "TimerEvent";
	for (int i=0; i<MAX_TIMER_EVENT; i++) {
		UnicodeString key;
		IniFile->WriteString(sct, key.sprintf(_T("Condition%u"), i + 1),	Timer_Condition[i]);
		IniFile->WriteString(sct, key.sprintf(_T("OnTimer%u"), i + 1),		OnTimerEvent[i]);
	}

	//�t�H���_�A�C�R��
	FldIcoRWLock->BeginWrite();
	FolderIconList->Sort();
	FolderIconFile->AssignSection("FolderIcon", FolderIconList);
	FldIcoRWLock->EndWrite();

	FolderIconFile->WriteString(SCT_Option, "DefFldIcoName",	DefFldIcoName);
}

//---------------------------------------------------------------------------
//INI�t�@�C�����X�V
//---------------------------------------------------------------------------
void UpdateIniFile(UsrIniFile *ini_file)
{
	if (ini_file->Modified) {
		UnicodeString msg = make_LogHdr(_T("SAVE"), ini_file->FileName);
		if (!ini_file->UpdateFile()) {
			UnicodeString errmsg = LoadUsrMsg(USTR_FaildSave, ExtractFileName(ini_file->FileName));
			msgbox_ERR(errmsg);
			set_LogErrMsg(msg, errmsg, ini_file->FileName);
		}
		AddLog(msg);
	}
}

//---------------------------------------------------------------------------
//�^�O�ɐݒ肳��Ă���ϐ��|�C���^��p���ăR���g���[���ɒl��ݒ�
//---------------------------------------------------------------------------
void BringOptionByTag(TForm *fp)
{
	for (int i=0; i<fp->ComponentCount; i++) {
		TComponent *cp = fp->Components[i];  if (cp->Tag==0) continue;
		if (class_is_CheckBox(cp)) {
			((TCheckBox*)cp)->Checked = *(bool*)cp->Tag;
		}
		else if (class_is_Edit(cp)) {
			if (((TEdit*)cp)->NumbersOnly)
				((TEdit*)cp)->Text = *(int*)cp->Tag;
			else
				((TEdit*)cp)->Text = *(UnicodeString*)cp->Tag;
		}
		else if (class_is_LabeledEdit(cp)) {
			if (((TLabeledEdit*)cp)->NumbersOnly)
				((TLabeledEdit*)cp)->Text = *(int*)cp->Tag;
			else
				((TLabeledEdit*)cp)->Text = *(UnicodeString*)cp->Tag;
		}
		else if (class_is_RadioGroup(cp)) {
			((TRadioGroup*)cp)->ItemIndex = *(int*)cp->Tag;
		}
		else if (class_is_ComboBox(cp)) {
			if (((TComboBox*)cp)->Style==csDropDown)
				((TComboBox*)cp)->Text = *(UnicodeString*)cp->Tag;
			else
				((TComboBox*)cp)->ItemIndex = *(int*)cp->Tag;
		}
	}
}

//---------------------------------------------------------------------------
//int �^�I�v�V�����̃f�t�H���g�l���擾
//---------------------------------------------------------------------------
int GetOptionIntDef(int tag)
{
	int def = 0;
	int idx = OptionList->IndexOfObject((TObject*)(NativeInt)tag);
	if (idx!=-1) {
		UnicodeString vbuf = OptionList->ValueFromIndex[idx];
		if (!is_quot(vbuf)) def = vbuf.ToIntDef(0);
	}
	return def;
}
//---------------------------------------------------------------------------
//�^�O�ɐݒ肳��Ă���ϐ��|�C���^��p���ăR���g���[���̕ύX�𔽉f
//---------------------------------------------------------------------------
void ApplyOptionByTag(TComponent *cp)
{
	if (cp->Tag==0) return;
	if (class_is_CheckBox(cp)) {
		*(bool*)cp->Tag = ((TCheckBox*)cp)->Checked;
	}
	else if (class_is_Edit(cp)) {
		if (((TEdit*)cp)->NumbersOnly)
			*(int*)cp->Tag = ((TEdit*)cp)->Text.ToIntDef(GetOptionIntDef(cp->Tag));
		else
			*(UnicodeString*)cp->Tag = ((TEdit*)cp)->Text;
	}
	else if (class_is_LabeledEdit(cp)) {
		if (((TLabeledEdit*)cp)->NumbersOnly)
			*(int*)cp->Tag = ((TLabeledEdit*)cp)->Text.ToIntDef(GetOptionIntDef(cp->Tag));
		else
			*(UnicodeString*)cp->Tag = ((TLabeledEdit*)cp)->Text;
	}
	else if (class_is_RadioGroup(cp)) {
		*(int*)cp->Tag = ((TRadioGroup*)cp)->ItemIndex;
	}
	else if (class_is_ComboBox(cp)) {
		if (((TComboBox*)cp)->Style==csDropDown)
			*(UnicodeString*)cp->Tag = ((TComboBox*)cp)->Text;
		else
			*(int*)cp->Tag = ((TComboBox*)cp)->ItemIndex;
	}
}
//---------------------------------------------------------------------------
void ApplyOptionByTag(TForm *fp)
{
	for (int i=0; i<fp->ComponentCount; i++) ApplyOptionByTag(fp->Components[i]);
}
//---------------------------------------------------------------------------
void ApplyOptionByTag(TTabSheet *sp)
{
	for (int i=0; i<sp->ControlCount; i++) {
		TControl *cp = sp->Controls[i];
		if (cp->ClassNameIs("TGroupBox")) {
			TGroupBox *gp = (TGroupBox *)cp;
			for (int j=0; j<gp->ControlCount; j++) {
				ApplyOptionByTag((TComponent *)gp->Controls[j]);
			}
		}
		else {
			ApplyOptionByTag((TComponent *)cp);
		}
	}
}

//---------------------------------------------------------------------------
//�c�[���E�C���h�E�̋��E����ݒ�
//---------------------------------------------------------------------------
void SetToolWinBorder(TForm *fp, bool sw)
{
	sw &= (col_TlBorder!=col_None && TlWinBorderWidth>0);
	for (int i=0; i<fp->ControlCount; i++) {
		TControl *cp = fp->Controls[i];
		if (cp->ClassNameIs("TShape") && StartsStr("Border", cp->Name)) {
			TShape *sp = (TShape *)cp;
			if (sw) {
				sp->Brush->Color = col_TlBorder;
				sp->Pen->Color	 = col_TlBorder;
				if (sp->Align==alLeft || sp->Align==alRight)
					sp->Width = TlWinBorderWidth;
				else
					sp->Height = TlWinBorderWidth;
			}
			sp->Visible = sw;
		}
	}
}

//---------------------------------------------------------------------------
//�ꗗ�p�O���b�h�̏�����
//---------------------------------------------------------------------------
void InitializeListGrid(TStringGrid *gp, TFont *fnt)
{
	gp->Color = col_bgList;
	gp->Font->Assign(fnt? fnt : ListFont);
	gp->DefaultRowHeight = get_FontHeight(gp->Font, ListInterLn);
}
//---------------------------------------------------------------------------
//�ꗗ�p�w�b�_�̏�����
//---------------------------------------------------------------------------
void InitializeListHeader(THeaderControl *hp, const _TCHAR *hdr, TFont *fnt)
{
	hp->DoubleBuffered = true;
	hp->Font->Assign(fnt? fnt : LstHdrFont);
	hp->Height = get_FontHeight(hp->Font, 6);

	UnicodeString s = hdr;
	for (int i=0; i<hp->Sections->Count && !s.IsEmpty(); i++)
		hp->Sections->Items[i]->Text = split_tkn(s, '|');
}

//---------------------------------------------------------------------------
//�t�H�[�����̃R���{�{�b�N�X�ɂ��Ď����⊮�̐ݒ��K�p
//---------------------------------------------------------------------------
void set_ComboBox_AutoComp(TForm *frm)
{
	for (int i=0; i<frm->ComponentCount; i++) {
		TComponent *cp = frm->Components[i];
		if (class_is_ComboBox(cp)) {
			((TComboBox*)cp)->AutoComplete = AutoCompComboBox;
			Sleep(0);
		}
	}
}

//---------------------------------------------------------------------------
//Megemo �I�v�V������ݒ�
//---------------------------------------------------------------------------
void set_MigemoCheckBox(TCheckBox *cp, const _TCHAR *key,
	UnicodeString sct)	//�Z�N�V������	(default = EmptyStr : General)
{
	cp->Enabled = usr_Migemo->DictReady;
	cp->Checked = cp->Enabled && (sct.IsEmpty()? IniFile->ReadBoolGen(key) : IniFile->ReadBool(sct, key));
}
//---------------------------------------------------------------------------
void set_MigemoAction(TAction *ap, const _TCHAR *key,
	UnicodeString sct)	//�Z�N�V������	(default = EmptyStr : General)
{
	ap->Enabled = usr_Migemo->DictReady;
	ap->Checked = ap->Enabled && (sct.IsEmpty()? IniFile->ReadBoolGen(key) : IniFile->ReadBool(sct, key));
}

//---------------------------------------------------------------------------
//�R���{�{�b�N�X�̌����������ւ�
//---------------------------------------------------------------------------
void change_ComboBoxHistory(TComboBox *cp,
	const _TCHAR *nrm_sct,	//�ʏ�̗����Z�N�V����
	const _TCHAR *reg_sct,	//���K�\���̗����Z�N�V����
	bool reg_sw)			//true = ���K�\��
{
	UnicodeString s = cp->Text;
	IniFile->SaveComboBoxItems(cp, !reg_sw? reg_sct : nrm_sct);
	IniFile->LoadComboBoxItems(cp,  reg_sw? reg_sct : nrm_sct);
	cp->Text = s;
}

//---------------------------------------------------------------------------
//���X�g�̍i�荞��
//  Objects ���e���ݒ� (0 �Ȃ� ���X�g�̃C���f�b�N�X��ݒ�)
//  �召�����͋�ʂ��Ȃ�
//---------------------------------------------------------------------------
void filter_List(
	TStringList *i_lst, //�Ώۃ��X�g
	TStringList *o_lst, //���ʃ��X�g
	UnicodeString kwd,	//������
	bool migemo_sw,		//Migemo���g�p
	bool and_or_sw,		//AND(' ') / OR('|') ����	(default = false)
	bool csv_sw,		//CSV�̍ŏ��̍���			(default = false)
	bool tsv_sw,		//TSV�̍ŏ��̍���			(default = false)
	bool tree_sw)		//�c���[�̕\������			(default = false)
{
	o_lst->Clear();

	//AND/OR����
	if (and_or_sw) {
		//�p�^�[�����X�g���쐬
		std::unique_ptr<TStringList> ptn_lst(new TStringList());
		TStringDynArray or_lst = SplitString(Trim(kwd), "|");
		for (int i=0; i<or_lst.Length; i++) {
			UnicodeString lbuf = Trim(or_lst[i]);
			if (!lbuf.IsEmpty()) {
				TStringDynArray and_lst = SplitString(lbuf, " ");
				TStringList *sp = new TStringList();
				for (int j=0; j<and_lst.Length; j++) {
					UnicodeString  ptn = Trim(and_lst[j]);
					if (migemo_sw) ptn = usr_Migemo->GetRegExPtn(true, ptn);
					if (!ptn.IsEmpty()) sp->Add(ptn);
				}
				ptn_lst->AddObject(lbuf, (TObject*)sp);
			}
		}

		//����
		if (ptn_lst->Count>0) {
			TRegExOptions opt; opt << roIgnoreCase;
			for (int i=0; i<i_lst->Count; i++) {
				UnicodeString lbuf = i_lst->Strings[i];

				bool ok = false;
				if (tree_sw && i==0) {
					ok = true;
				}
				else {
					UnicodeString sbuf = csv_sw? get_csv_item(lbuf, 0) : (tree_sw || tsv_sw)? get_pre_tab(lbuf) : lbuf;
					for (int j=0; j<ptn_lst->Count && !ok; j++) {
						TStringList *sp = (TStringList *)ptn_lst->Objects[j];
						if (sp->Count>0) {
							bool and_ok = true;
							for (int k=0; k<sp->Count && and_ok; k++) {
								UnicodeString ptn = sp->Strings[k];
								and_ok = migemo_sw? TRegEx::IsMatch(sbuf, ptn, opt) : ContainsText(sbuf, ptn);
							}
							ok = and_ok;
						}
					}
				}

				if (ok) {
					int idx = (int)i_lst->Objects[i];
					if (idx==0) idx = i;
					o_lst->AddObject(lbuf, (TObject*)(NativeInt)idx);
				}
			}

			for (int i=0; i<ptn_lst->Count; i++) delete (TStringList*)ptn_lst->Objects[i];
		}
	}
	//�P������
	else {
		UnicodeString ptn = migemo_sw? usr_Migemo->GetRegExPtn(true, kwd) : kwd;
		if (!ptn.IsEmpty()) {
			TRegExOptions opt; opt << roIgnoreCase;
			for (int i=0; i<i_lst->Count; i++) {
				UnicodeString lbuf = i_lst->Strings[i];
				UnicodeString sbuf = csv_sw? get_csv_item(lbuf, 0) : lbuf;
				if (migemo_sw? TRegEx::IsMatch(sbuf, ptn, opt) : ContainsText(sbuf, ptn)) {
					int idx = (int)i_lst->Objects[i];
					if (idx==0) idx = i;
					o_lst->AddObject(lbuf, (TObject*)(NativeInt)idx);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//�t�@�C���ꗗ�\�[�g�p��r�֐�
//---------------------------------------------------------------------------
int __fastcall CompDirName(file_rec *fp0, file_rec *fp1)
{
	return SameText(fp0->b_name, fp1->b_name) ? CompTextN(fp0->p_name, fp1->p_name)
											  : CompTextN(fp0->b_name, fp1->b_name);
}
//---------------------------------------------------------------------------
int __fastcall CompDirTime(file_rec *fp0, file_rec *fp1)
{
	if (fp0->f_time==fp1->f_time) return CompDirName(fp0, fp1);
	return !OldOrder? ((fp0->f_time<fp1->f_time)? 1 : -1) : ((fp0->f_time>fp1->f_time)? 1 : -1);
}
//---------------------------------------------------------------------------
int __fastcall CompDirSize(file_rec *fp0, file_rec *fp1)
{
	if (fp0->f_size==fp1->f_size) return CompDirName(fp0, fp1);
	return !SmallOrder? ((fp1->f_size > fp0->f_size)? 1 : -1) : ((fp1->f_size < fp0->f_size)? 1 : -1);
}
//---------------------------------------------------------------------------
int __fastcall CompDirAttr(file_rec *fp0, file_rec *fp1)
{
	if (fp0->f_attr==fp1->f_attr) return CompDirName(fp0, fp1);
	return !DscAttrOrder? (fp1->f_attr - fp0->f_attr) : (fp0->f_attr - fp1->f_attr);
}

//---------------------------------------------------------------------------
int __fastcall CompDirIcon(file_rec *fp0, file_rec *fp1)
{
	UnicodeString inam0 = get_FolderIconName(fp0->f_name);
	UnicodeString inam1 = get_FolderIconName(fp1->f_name);

	if (SameText(inam0, inam1)) return CompDirName(fp0, fp1);
	if (inam0.IsEmpty()) return 1;
	if (inam1.IsEmpty()) return -1;
	return CompTextN(inam0, inam1);
}

//---------------------------------------------------------------------------
//���O
int __fastcall SortComp_Name(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 1;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 2:  return CompDirTime(fp0, fp1);
			case 3:  return CompDirSize(fp0, fp1);
			case 4:  return CompDirAttr(fp0, fp1);
			case 6:  return CompDirIcon(fp0, fp1);
			default: return CompDirName(fp0, fp1);
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return  1;
	}

	//�t�@�C�����m
	UnicodeString nam0 = !fp0->alias.IsEmpty()? fp0->alias : fp0->b_name;
	UnicodeString nam1 = !fp1->alias.IsEmpty()? fp1->alias : fp1->b_name;
	int res = SameText(nam0, nam1)? CompTextN(fp0->f_ext, fp1->f_ext) : CompTextN(nam0, nam1);
	return (res==0)? CompTextN(fp0->p_name, fp1->p_name) : res;
}
//---------------------------------------------------------------------------
//�g���q
int __fastcall SortComp_Ext(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 1;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 2:  return CompDirTime(fp0, fp1);
			case 3:  return CompDirSize(fp0, fp1);
			case 4:  return CompDirAttr(fp0, fp1);
			case 6:  return CompDirIcon(fp0, fp1);
			default: return CompDirName(fp0, fp1);
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return  1;
	}

	if (SameText(fp0->f_ext, fp1->f_ext)) return SortComp_Name(List, Index1, Index2);

	return CompTextN(fp0->f_ext, fp1->f_ext);
}
//---------------------------------------------------------------------------
//�^�C���X�^���v
int __fastcall SortComp_Time(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 2;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 1:  return CompDirName(fp0, fp1);
			case 3:  return CompDirSize(fp0, fp1);
			case 4:  return CompDirAttr(fp0, fp1);
			default: return CompDirTime(fp0, fp1);
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return  1;
	}

	if (fp0->f_time==fp1->f_time) return SortComp_Ext(List, Index1, Index2);

	return !OldOrder? ((fp0->f_time<fp1->f_time)? 1 : -1) : ((fp0->f_time>fp1->f_time)? 1 : -1);
}
//---------------------------------------------------------------------------
//�T�C�Y
int __fastcall SortComp_Size(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 1;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 1:  return CompDirName(fp0, fp1);
			case 2:  return CompDirTime(fp0, fp1);
			case 4:  return CompDirAttr(fp0, fp1);
			default: return CompDirSize(fp0, fp1);
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return  1;
	}

	if (fp0->f_size==fp1->f_size) return SortComp_Ext(List, Index1, Index2);

	return !SmallOrder? ((fp1->f_size>fp0->f_size)? 1 : -1) : ((fp1->f_size<fp0->f_size)? 1 : -1);
}
//---------------------------------------------------------------------------
//����
int __fastcall SortComp_Attr(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 1;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 1:  return CompDirName(fp0, fp1);
			case 2:  return CompDirTime(fp0, fp1);
			case 3:  return CompDirSize(fp0, fp1);
			default: return CompDirAttr(fp0, fp1);
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return 1;
	}

	if (fp0->f_attr==fp1->f_attr)	return SortComp_Ext(List, Index1, Index2);

	return !DscAttrOrder? (fp1->f_attr - fp0->f_attr) : (fp0->f_attr - fp1->f_attr);
}
//---------------------------------------------------------------------------
//�f�B���N�g���݂̂̃��X�g�̃\�[�g
int __fastcall SortComp_DirOnly(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	switch ((fp0->tag!=-1)? DirSortMode[fp0->tag] : 1) {
	case  1: return CompDirName(fp0, fp1);
	case  2: return CompDirTime(fp0, fp1);
	case  3: return CompDirSize(fp0, fp1);
	case  4: return CompDirAttr(fp0, fp1);
	case  6: return CompDirIcon(fp0, fp1);
	default: return (Index1 - Index2);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g����(�o�^��)
int __fastcall SortComp_PathName(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	UnicodeString pnam0 = fp0->p_name;
	UnicodeString pnam1 = fp1->p_name;

	if (DispRegName) {
		pnam0 = get_RegDirName(pnam0);
		pnam1 = get_RegDirName(pnam1);
	}

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	int d_mode = (fp0->tag!=-1)? DirSortMode[fp0->tag] : 1;
	if (d_mode!=5) {
		if (fp0->is_dir && fp1->is_dir) {
			switch (d_mode) {
			case 2:  return CompDirTime(fp0, fp1);
			case 3:  return CompDirSize(fp0, fp1);
			case 4:  return CompDirAttr(fp0, fp1);
			default:
				return (SameText(pnam0, pnam1) ? CompTextN2(fp0->b_name, fp1->b_name)
											   : CompTextN2(pnam0, pnam1));
			}
		}

		if (fp0->is_dir) return -1;
		if (fp1->is_dir) return 1;
	}

	//�t�@�C�����m
	if (SameText(pnam0, pnam1)) {
		int res = SameText(fp0->b_name, fp1->b_name) ? CompTextN2(fp0->f_ext, fp1->f_ext)
													 : CompTextN2(fp0->b_name, fp1->b_name);
		return (res==0)? CompTextN2(fp0->p_name, fp1->p_name) : res;
	}

	return CompTextN2(pnam0, pnam1);
}
//---------------------------------------------------------------------------
//�������e	(fp->memo �� \t �O)
int __fastcall SortComp_Memo(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	UnicodeString memo0 = get_pre_tab(fp0->memo);
	UnicodeString memo1 = get_pre_tab(fp1->memo);

	if (memo0.IsEmpty() && !memo1.IsEmpty()) return  1;
	if (!memo0.IsEmpty() && memo1.IsEmpty()) return -1;

	if ((memo0.IsEmpty() && memo1.IsEmpty()) || SameText(memo0, memo1)) {
		int res = SameText(fp0->b_name, fp1->b_name) ? CompTextN(fp0->f_ext, fp1->f_ext)
													 : CompTextN(fp0->b_name, fp1->b_name);
		return (res==0)? CompTextN(fp0->p_name, fp1->p_name) : res;
	}

	return CompTextN(memo0, memo1);
}
//---------------------------------------------------------------------------
//Git���	(fp->memo �� \t ��)
int __fastcall SortComp_GitStt(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	UnicodeString memo0 = get_post_tab(fp0->memo);
	UnicodeString memo1 = get_post_tab(fp1->memo);

	if (memo0.IsEmpty() && !memo1.IsEmpty()) return  1;
	if (!memo0.IsEmpty() && memo1.IsEmpty()) return -1;

	if ((memo0.IsEmpty() && memo1.IsEmpty()) || SameText(memo0, memo1)) {
		int res = SameText(fp0->b_name, fp1->b_name) ? CompTextN(fp0->f_ext, fp1->f_ext)
													 : CompTextN(fp0->b_name, fp1->b_name);
		return (res==0)? CompTextN(fp0->p_name, fp1->p_name) : res;
	}

	return CompTextN(memo0, memo1);
}
//---------------------------------------------------------------------------
//�}�[�N�ݒ����
int __fastcall SortComp_MarkTime(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	TDateTime dt0 = fp0->f_time;
	TDateTime dt1 = fp1->f_time;

	UnicodeString tstr0 = get_post_tab(fp0->memo);
	if (!tstr0.IsEmpty()) {
		try {
			dt0 = VarToDateTime(tstr0);
		}
		catch (...) {
			;
		}
	}

	UnicodeString tstr1 = get_post_tab(fp1->memo);
	if (!tstr1.IsEmpty()) {
		try {
			dt1 = VarToDateTime(tstr1);
		}
		catch (...) {
			;
		}
	}

	if (dt0==dt1) return SortComp_Ext(List, Index1, Index2);
	if ((int)dt0==0 || (int)dt1==0) return ((int)dt0==0)? 1 : -1;

	return !OldOrder? ((dt0<dt1)? 1 : -1) : ((dt0>dt1)? 1 : -1);
}
//---------------------------------------------------------------------------
//�ҏW����
int __fastcall SortComp_Distance(TStringList *List, int Index1, int Index2)
{
	file_rec *fp0 = (file_rec*)List->Objects[Index1];
	file_rec *fp1 = (file_rec*)List->Objects[Index2];
	if (!fp0 || !fp1) return 0;

	if (fp0->is_up) return -1;
	if (fp1->is_up) return  1;

	if (fp0->distance==fp1->distance) return SortComp_Ext(List, Index1, Index2);

	return (fp0->distance - fp1->distance);
}

//---------------------------------------------------------------------------
//�f�B���N�g����(�c���[�p)
int __fastcall Comp_PathTree(TStringList *List, int Index1, int Index2)
{
	TStringDynArray plst1 = split_path(List->Strings[Index1]);
	TStringDynArray plst2 = split_path(List->Strings[Index2]);

	for (int i=0; i<plst1.Length && i<plst2.Length; i++) {
		if (SameText(plst1[i], plst2[i])) continue;
		return StrCmpLogicalW(plst1[i].c_str(), plst2[i].c_str());
	}

	return (plst1.Length - plst2.Length);
}

//---------------------------------------------------------------------------
//�L�[�ꗗ�\�[�g�p��r�֐�
//---------------------------------------------------------------------------
//�L�[
//---------------------------------------------------------------------------
int __fastcall KeyComp_Key(TStringList *List, int Index1, int Index2)
{
	UnicodeString k0 = List->Names[Index1];
	UnicodeString k1 = List->Names[Index2];

	//�J�e�S���[
	UnicodeString c0, c1;
	if (ContainsStr(k0, ":")) c0 = split_tkn(k0, ':');
	if (ContainsStr(k1, ":")) c1 = split_tkn(k1, ':');
	if (!SameText(c0, c1)) return ScrModeIdStr.Pos(c0) - ScrModeIdStr.Pos(c1);

	if (k0.IsEmpty() && k1.IsEmpty())
		return CompareText(List->ValueFromIndex[Index1], List->ValueFromIndex[Index2]);
	if (k0.IsEmpty() && !k1.IsEmpty())	return 1;
	if (!k0.IsEmpty() && k1.IsEmpty())	return -1;
	if ( ContainsStr(k0, "~") && !ContainsStr(k1, "~"))	return 1;
	if (!ContainsStr(k0, "~") &&  ContainsStr(k1, "~"))	return -1;

	//�V�t�g
	UnicodeString s0, s1;
	if (remove_text(k0, KeyStr_SELECT))	s0 += "S0+";
	if (remove_text(k0, KeyStr_Shift))	s0 += "S1+";
	if (remove_text(k0, KeyStr_Ctrl))	s0 += "S2+";
	if (remove_text(k0, KeyStr_Alt))	s0 += "S3+";
	if (remove_text(k1, KeyStr_SELECT))	s1 += "S0+";
	if (remove_text(k1, KeyStr_Shift))	s1 += "S1+";
	if (remove_text(k1, KeyStr_Ctrl))	s1 += "S2+";
	if (remove_text(k1, KeyStr_Alt))	s1 += "S3+";
	if (!SameText(s0, s1)) return CompareStr(s0, s1);

	//2�X�g���[�N
	if (ContainsStr(k0, "~") && ContainsStr(k1, "~")) {
		UnicodeString f0 = split_tkn(k0, '~');
		UnicodeString f1 = split_tkn(k1, '~');
		if (!SameText(f0, f1)) return CompareText(f0, f1);
	}

	//�L�[
	if (k0.Length()==1 && k1.Length()==1) return CompareText(k0, k1);

	if (k0.Length()>1 && k1.Length()>1) {
		if (k0[1]=='F' && k1[1]!='F') return -1;
		if (k0[1]!='F' && k1[1]=='F') return 1;
		return StrCmpLogicalW(k0.c_str(), k1.c_str());
	}

	return k0.Length() - k1.Length();
}
//---------------------------------------------------------------------------
//�R�}���h
//---------------------------------------------------------------------------
int __fastcall KeyComp_Cmd(TStringList *List, int Index1, int Index2)
{
	UnicodeString cmd0 = List->ValueFromIndex[Index1];
	UnicodeString cmd1 = List->ValueFromIndex[Index2];
	return !SameText(cmd0, cmd1)? CompareText(cmd0, cmd1) : KeyComp_Key(List, Index1, Index2);
}
//---------------------------------------------------------------------------
//����
//---------------------------------------------------------------------------
int __fastcall KeyComp_Dsc(TStringList *List, int Index1, int Index2)
{
	UnicodeString dsc0 = get_CmdDesc(List->ValueFromIndex[Index1], true);
	UnicodeString dsc1 = get_CmdDesc(List->ValueFromIndex[Index2], true);
	return !SameText(dsc0, dsc1)? CompareText(dsc0, dsc1) : KeyComp_Key(List, Index1, Index2);
}

//---------------------------------------------------------------------------
UnicodeString yen_to_delimiter(UnicodeString s)
{
	return ReplaceStr(s, "\\", DirDelimiter);
}
//---------------------------------------------------------------------------
UnicodeString alt_yen_to(UnicodeString s)
{
	return AltBackSlash? ReplaceStr(s, "\\", _T("\u2216")) : s;
}

//---------------------------------------------------------------------------
//�w�蕝�Ɏ��܂�悤�Ƀp�X���𒲐�
//---------------------------------------------------------------------------
UnicodeString get_MiniPathName(
	UnicodeString pnam,		//�p�X��
	int max_w,				//������
	TFont *font,
	bool rep_delimiter)		//�f�B���N�g����؂��u��	(default = false)
{
	HWND hWnd = Application->ActiveFormHandle;
	HDC hDc = ::GetDC(hWnd);
	if (hDc) {
		std::unique_ptr<TCanvas> cv(new TCanvas());
		cv->Handle = hDc;
		cv->Font->Assign(font);
		bool is_irreg = IsIrregularFont(cv->Font);
		bool ends_dlmt = EndsStr("\\", pnam);

		//�f�B���N�g����؂�̈Ⴂ�ɂ�鐧�����̕␳
		if (rep_delimiter) {
			int w = get_TextWidth(cv.get(), yen_to_delimiter(pnam), is_irreg) - get_TextWidth(cv.get(), pnam, is_irreg);
			if (w>0) max_w -= w;
		}

		while (get_TextWidth(cv.get(), pnam, is_irreg)>max_w) {
			TStringDynArray plst = split_path(pnam);
			bool changed = false;
			for (int i=0; i<plst.Length; i++) {
				UnicodeString dnam = plst[i];
				if (i==0 && (StartsStr("\\\\", dnam) || StartsStr('<', dnam))) continue;
				int dlen = dnam.Length();
				if (!EndsStr("�c", dnam) && dlen>4) {
					plst[i] = dnam.SubString(1, (dlen>=32)? dlen/2 : dlen - 3) + "�c";
					changed = true;
					break;
				}
				if (EndsStr("�c", dnam) && dlen>2) {
					plst[i].Delete(dlen - 1, 1);
					changed = true;
					break;
				}
				if (i>0 && i<plst.Length-1 && EndsStr("�c", dnam) && dlen==2) {
					plst[i] = "�c";
					changed = true;
					break;
				}
			}
			if (!changed) break;

			pnam = EmptyStr;
			for (int i=0; i<plst.Length; i++) pnam.cat_sprintf(_T("%s\\"), plst[i].c_str());
			if (!ends_dlmt) pnam = ExcludeTrailingPathDelimiter(pnam);
		}
		::ReleaseDC(hWnd, hDc);

		if (rep_delimiter) pnam = yen_to_delimiter(pnam);
	}
	return pnam;
}

//---------------------------------------------------------------------------
//.nyanfi �����擾
//---------------------------------------------------------------------------
UnicodeString get_dotNaynfi(UnicodeString dnam)
{
	UnicodeString fnam;
	if (!dnam.IsEmpty()) fnam = IncludeTrailingPathDelimiter(dnam);
	fnam += ".nyanfi";
	if (DotNyanPerUser && !UserName.IsEmpty()) fnam.cat_sprintf(_T("_%s"), UserName.c_str());
	return fnam;
}
//---------------------------------------------------------------------------
UnicodeString get_dotNaynfi(UnicodeString dnam,
	bool inherit,	//��ʂ���p��
	bool force)		//�J�����g�𖳎����ċ����I�Ɍp��
{
	if (force) dnam = IncludeTrailingPathDelimiter(get_parent_path(dnam));

	UnicodeString fnam = get_dotNaynfi(dnam);
	while (!file_exists(fnam) && inherit) {
		if (is_root_dir(dnam)) break;
		dnam = IncludeTrailingPathDelimiter(get_parent_path(dnam));
		fnam = get_dotNaynfi(dnam);
	}
	return fnam;
}

//---------------------------------------------------------------------------
//Web�����\����������擾
//---------------------------------------------------------------------------
UnicodeString get_WebSeaCaption(
	UnicodeString kwd,	//������				(default = EmptyStr)
	bool with_ak)		//�A�N�Z�����[�^��t��	(default = true)
{
	UnicodeString ret_str;
	if (!kwd.IsEmpty()) {
		kwd = Trim(get_first_line(kwd));
		kwd = ReplaceStr(kwd, "&", "&&");
		if (kwd.Length()>20) kwd = kwd.SubString(1, 20) + "�c";
		if (!kwd.IsEmpty()) ret_str.sprintf(_T("�u%s�v�� "), kwd.c_str());
	}

	UnicodeString url = get_tkn_m(WebSeaUrl, _T("//"), _T("/"));
	remove_top_text(url, _T("www."));
	if (url.IsEmpty()) url = "Web";
	ret_str.cat_sprintf(_T("%s �Ō���"), url.c_str());
	if (with_ak) ret_str += "(&S)";

	return ret_str;
}

//---------------------------------------------------------------------------
//�ŐV�̃^�C���X�^���v���H
//---------------------------------------------------------------------------
bool is_NewerTime(TDateTime scr_t, TDateTime dst_t)
{
	return (!WithinPastMilliSeconds(scr_t, dst_t, TimeTolerance) && scr_t>dst_t);
}

//---------------------------------------------------------------------------
//UNC�p�X�̑��݃`�F�b�N
//  ESC �Œ��f�\(�X���b�h�Ń`�F�b�N)
//�߂�l: true	= ���݂��� or ��UNC�p�X
//		  false = ���݂��Ȃ� or ������ UNC�p�X
//---------------------------------------------------------------------------
bool check_if_unc(UnicodeString pnam)
{
	UnicodeString tmp = pnam;
	if (!remove_top_s(tmp, "\\\\")) return true;

	TStringDynArray plst = SplitString(tmp, "\\");
	if (plst.Length<3) return false;

	TCheckPathThread *tp = new TCheckPathThread(true);
	tp->PathName = pnam;
	tp->Start();
	while (!tp->PathName.IsEmpty()) {
		if (is_KeyPress_ESC()) {
			ClearKeyBuff(true);
			break;
		}
		Sleep(100);
	}

	bool ok = tp->isOk;
	tp->Terminate();
	return ok;
}

//---------------------------------------------------------------------------
//�����񂩂�R�}���h�t�@�C�������擾
// �擪�� @ �͍폜���A��΃p�X��
// .nbt �łȂ���� EmptyStr ��Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_cmdfile(UnicodeString s)
{
	if (!remove_top_AT(s) && !test_NbtExt(get_extension(s))) return EmptyStr; 
	return to_absolute_name(s);
}

//---------------------------------------------------------------------------
//ExeCommands �̃p�����[�^����t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_cmds_prm_file(UnicodeString prm)
{
	if (remove_top_AT(prm))	return to_absolute_name(prm);
	if (remove_top_text(prm, _T("ExeMenuFile_")))
							return to_absolute_name(exclude_quot(prm));
	return EmptyStr;
}

//---------------------------------------------------------------------------
//�K�v�ȏꍇ���X�|���X�t�@�C�����쐬
//  �߂�l: ���X�|���X�t�@�C����
//			�s�v�̏ꍇ�� EmptyStr�A�G���[�̏ꍇ�� RESPONSE_ERR
//---------------------------------------------------------------------------
UnicodeString make_ResponseFile(TStringList *lst,
	int arc_t,				//�A�[�J�C�u�^�C�v
	UnicodeString *files,	//[o] �t�@�C�������X�g (default = NULL)
	bool excl)				//true: ���X�|���X�t�@�C��/���X�g�̈���݂̂��쐬
							// (default = false: ���X�|���X�t�@�C��/���X�g�ɕ��U)
{
	std::unique_ptr<TStringList> r_lst(new TStringList());
	std::unique_ptr<TStringList> f_lst(new TStringList());

	//�����A�[�J�C�o�ȊO
	if (arc_t==0) {
#if defined(_WIN64)
		return RESPONSE_ERR;
#else
		for (int i=0; i<lst->Count; i++) r_lst->Add(add_quot_if_spc(lst->Strings[i]));
#endif
	}
	//�����A�[�J�C�o
	else {
		if (arc_t==UARCTYP_CAB && lst->Count>250) {
			for (int i=0; i<lst->Count; i++) r_lst->Add(add_quot_if_spc(lst->Strings[i]));
		}
		else {
			for (int i=0; i<lst->Count; i++) {
				UnicodeString fnam = lst->Strings[i];
				if ((arc_t!=UARCTYP_RAR && starts_AT(fnam)) || (arc_t==UARCTYP_CAB && StartsStr('-', fnam))) {
					r_lst->Add(add_quot_if_spc(fnam));
				}
				else {
					//��unrarXX.dll �̃o�O? �΍�
					if (starts_AT(fnam)) fnam = "?" + exclude_top(fnam);
					f_lst->Add(add_quot_if_spc(fnam));
				}
			}
		}

		//���X�|���X�t�@�C���̂�
		if ((excl || !files) && r_lst->Count>0 && f_lst->Count>0) {
			r_lst->Clear();
			f_lst->Clear();
			for (int i=0; i<lst->Count; i++) r_lst->Add(add_quot_if_spc(lst->Strings[i]));
		}
		//���X�g
		if (files && f_lst->Count>0) {
			for (int i=0; i<f_lst->Count; i++) {
				if (i>0) *files += " ";
				*files += f_lst->Strings[i];
			}
		}
	}

	UnicodeString res_file;
	if (r_lst->Count>0) {
		res_file.sprintf(_T("%s" RESPONSE_FILE), TempPathA.c_str());
		//�t�@�C���ۑ�
		try {
			if (usr_ARC->IsUnicode(arc_t))
				r_lst->SaveToFile(res_file, TEncoding::UTF8);
			else
				r_lst->SaveToFile(res_file);
		}
		catch (...) {
			res_file = RESPONSE_ERR;
		}
	}

	return res_file;
}
//---------------------------------------------------------------------------
UnicodeString make_ResponseFile(UnicodeString fnam, int arc_t)
{
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Add(fnam);
	return make_ResponseFile(lst.get(), arc_t);
}

//---------------------------------------------------------------------------
//�N���[����/�����������̉��������������W�J
//---------------------------------------------------------------------------
UnicodeString format_CloneName(
	UnicodeString fmt,		//����
	UnicodeString fnam,		//�t�@�C����
	UnicodeString dst_dir,	//�쐬/�R�s�[��f�B���N�g��
	bool is_dir,			//�Ώۂ̓f�B���N�g���[��?				(default = false)
	TDateTime *ts,			//�R�s�[���̃^�C���X�^���v				(default = NULL : fnam ����擾)
	TStringList *lst)		//���݃`�F�b�N�p�R�s�[��t�@�C�����X�g	(default = NULL : fnam �Ń`�F�b�N)
{
	if (fmt.IsEmpty()) fmt = FMT_AUTO_REN;

	if (!dst_dir.IsEmpty()) dst_dir = IncludeTrailingPathDelimiter(dst_dir);
	fnam = ExcludeTrailingPathDelimiter(fnam);
	UnicodeString bnam = !is_dir? get_base_name(fnam) : ExtractFileName(fnam);
	UnicodeString fext = !is_dir? get_extension(fnam) : EmptyStr;

	UnicodeString ret_str, last_str;
	int sn = 0;
	for (int i=0; ; i++) {
		ret_str = dst_dir;
		UnicodeString fmt_str = (i==0)? get_tkn(fmt, _T("\\-")) : ReplaceStr(fmt, "\\-", "");

		UnicodeString tmp = fmt_str;
		while (!tmp.IsEmpty()) {
			WideChar c = split_top_wch(tmp);
			if (c=='\\') {
				//�A��
				if (StartsStr("SN(", tmp)) {
					UnicodeString nstr = split_in_paren(tmp);
					if (nstr.IsEmpty()) nstr = "1";
					ret_str.cat_sprintf(_T("%0*u"), nstr.Length(), nstr.ToIntDef(0) + sn);
				}
				//����/�^�C���X�^���v
				else if (StartsStr("DT(", tmp) || StartsStr("TS(", tmp)) {
					TDateTime dt = StartsStr("TS(", tmp)? (ts? *ts : get_file_age(fnam)) : Now();
					ret_str += FormatDateTime(split_in_paren(tmp), dt);
				}
				else {
					c = split_top_wch(tmp);
					//�t�@�C�����啔�܂��̓f�B���N�g����
					if (c=='N') ret_str += bnam;
				}
			}
			else ret_str.cat_sprintf(_T("%c"), c);
		}

		ret_str += fext;
		if (lst) {
			if (!ExistsInList(ret_str, lst)) break;
		}
		else {
			if (!file_exists(ret_str)) break;
		}

		//�d���ɂ��Ď��s
		if (ContainsStr(fmt_str, "\\SN("))
			sn++;
		else if (SameText(last_str, ret_str))
			fmt += SFX_AUTOREN;

		last_str = ret_str;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�����������W�J
//  \L(n)    �t�@�C�����̍�����n����
//  \S(m,n)  �t�@�C������m�Ԗڂ���n����
//  \R(n)    �t�@�C�����̉E����n����
//  \A       �t�@�C�����S��
//  \E       �g���q
//  \TS(�c)  �^�C���X�^���v
//  \XT(�c)  Exit�B�e����
//  \\       \ ���̂���
//---------------------------------------------------------------------------
UnicodeString format_FileName(UnicodeString fmt, UnicodeString fnam)
{
	UnicodeString ret_str;
	UnicodeString bnam = get_base_name(fnam);
	UnicodeString fext = get_extension(fnam);	remove_top_s(fext, '.');

	int i = 1;
	while (i<=fmt.Length()) {
		WideChar c = fmt[i];
		if (c=='\\') {
			UnicodeString s = fmt.SubString(i, 512);
			if (s.Pos(')')) s = get_tkn(s, ')') + ")";

			if (StartsStr("\\L(", s)) {
				int n = get_in_paren(s).ToIntDef(0);
				if (n>0) ret_str += bnam.SubString(1, n);
			}
			else if (StartsStr("\\S(", s)) {
				int m = get_tkn_m(s, '(', ',').ToIntDef(0);
				int n = get_tkn_m(s, ',', ')').ToIntDef(0);
				if (n>0 && m>0) ret_str += bnam.SubString(m, n);
			}
			else if (StartsStr("\\R(", s)) {
				int n = get_in_paren(s).ToIntDef(0);
				if (n>0) ret_str += bnam.SubString(bnam.Length() - n + 1, n);
			}
			else if (StartsStr("\\TS(", s)) {
				ret_str += FormatDateTime(get_in_paren(s), get_file_age(fnam));
			}
			else if (StartsStr("\\XT(", s)) {
				ret_str += EXIF_GetExifTimeStr(fnam, get_in_paren(s));
			}
			else {
				s = fmt.SubString(i, 2);
				if		(StartsStr("\\A", s)) ret_str += bnam;
				else if (StartsStr("\\E", s)) ret_str += fext;
				else						   ret_str += s.SubString(2, 1);
			}
			i += s.Length();
		}
		else {
			ret_str += c;
			i++;
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�Ή�����\�[�X�^�w�b�_�t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_SrcHdrName(UnicodeString fnam)
{
	UnicodeString fext = get_extension(fnam);
	if		(test_FileExt(fext, FEXT_C_SRC)) fext = FEXT_C_HDR;
	else if (test_FileExt(fext, FEXT_C_HDR)) fext = FEXT_C_SRC;
	else return EmptyStr;

	TStringDynArray x_lst = SplitString(fext, ".");
	bool found = false;
	for (int i=0; i<x_lst.Length && !found; i++) {
		if (x_lst[i].IsEmpty()) continue;
		fnam  = ChangeFileExt(fnam, "." + x_lst[i]);
		found = FileExists(fnam);
	}

	return found? fnam : EmptyStr;
}

//---------------------------------------------------------------------------
//�t�@�C�����啔���������̃t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_NextSameName(
	UnicodeString fnam,		//�t�@�C����
	bool only_text)			//true = �e�L�X�g�̂� (default = false)
{
	std::unique_ptr<TStringList> l_lst(new TStringList());
	get_files(ExtractFilePath(fnam), (get_base_name(fnam) + ".*").c_str(), l_lst.get());
	if (l_lst->Count<2) return EmptyStr;

	int idx = l_lst->IndexOf(fnam);
	if (idx==-1) return EmptyStr;

	int idx0 = -1, idx1 = -1;
	for (int i=0; i<l_lst->Count && idx1==-1; i++) {
		if (i<=idx && idx0!=-1) continue;
		UnicodeString nnam = l_lst->Strings[i];
		if (dir_exists(nnam)) continue;
		if (only_text && !is_TextFile(nnam)) continue;
		if (i<=idx) idx0 = i; else idx1 = i;
	}
	idx = (idx1!=-1)? idx1 : idx0;

	return (idx!=-1)? l_lst->Strings[idx] : EmptyStr;
}
//---------------------------------------------------------------------------
UnicodeString get_NextSameName(
	TStringList *lst,
	int idx,
	bool only_text)			//true = �e�L�X�g�̂� (default = false)
{
	if (idx<0 || idx>=lst->Count) return EmptyStr;
	UnicodeString bnam = get_base_name(lst->Strings[idx]);

	int idx0 = -1, idx1 = -1;
	for (int i=0; i<lst->Count && idx1==-1; i++) {
		if (i<=idx && idx0!=-1) continue;
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_dir || fp->is_dummy) continue;
		if (!SameText(fp->b_name, bnam)) continue;
		if (only_text && !is_TextFile(fp->f_name)) continue;
		if (i<=idx) idx0 = i; else idx1 = i;
	}
	idx = (idx1!=-1)? idx1 : idx0;

	return (idx!=-1)? lst->Strings[idx] : EmptyStr;
}

//---------------------------------------------------------------------------
//���C�u�������X�g�̎擾
//---------------------------------------------------------------------------
void get_LibraryList(
	UnicodeString fnam,		//���C�u�����t�@�C����(������ \ �Ȃ烉�C�u�����p�X)
	TStringList *lst,		//[o] ���C�u�������X�g
	bool get_info)			//�t�@�C�����Ƃ��Ď擾 (default = false)
{
	try {
		std::unique_ptr<TStringList> l_lst(new TStringList());
		if (ends_PathDlmtr(fnam)) {
			get_files(fnam, _T("*.library-ms"), l_lst.get());
		}
		else {
			if (!file_exists(fnam)) Abort();
			l_lst->Text = fnam;
		}

		int l_cnt = 0;
		for (int l_i=0; l_i<l_lst->Count; l_i++) {
			std::unique_ptr<TStringList> f_buf(new TStringList());
			load_text_ex(l_lst->Strings[l_i], f_buf.get());
			int tag = 0;
			for (int i=0; i<f_buf->Count; i++) {
				UnicodeString lbuf = Trim(f_buf->Strings[i]);
				switch (tag) {
				case 0:
					if (ContainsText(lbuf, "<searchConnectorDescriptionList>")) tag = 1;
					break;
				case 1:
					if (ContainsText(lbuf, "<searchConnectorDescription")) tag = 2;
					break;
				case 2:
					if (ContainsText(lbuf, "<url>") && ContainsText(lbuf, "</url>")) {
						UnicodeString url = get_tkn_m(lbuf, _T("<url>"), _T("</url>"));
						if (StartsText("knownfolder:", url)) {
							url = usr_SH->KnownGuidStrToPath(get_tkn_r(url, ':'));
						}
						if (!url.IsEmpty()) {
							if (l_cnt==0 && get_info) lst->Add(EmptyStr);	//�Z�p���[�^
							l_cnt++;
							if (get_info)
								add_PropLine(UnicodeString("�ꏊ").cat_sprintf(_T("%u"), l_cnt), url, lst);
							else
								lst->Add(url);
						}
						tag = 1;
					}
					break;
				}
			}
		}
		if (l_cnt>0 && get_info) lst->Add(EmptyStr);	//�Z�p���[�^
	}
	catch (...) {
		;
	}
}

//---------------------------------------------------------------------------
//�J�����g�����C�u�����o�^�f�B���N�g���̃��[�g�Ȃ烉�C�u�����t�@�C����Ԃ�
//---------------------------------------------------------------------------
UnicodeString get_LibFile_if_root()
{
	if (CurStt->LibraryInfo.IsEmpty()) return EmptyStr;

	UnicodeString dnam = CurStt->LibraryInfo;
	UnicodeString lnam = split_pre_tab(dnam);
	if (SameText(IncludeTrailingPathDelimiter(dnam), CurPathName)) {
		CurStt->LibraryInfo = CurStt->LibSubPath = EmptyStr;
		return lnam;
	}
	else {
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//�����Ώۃ��X�g���擾
//  �߂�l: �I�v�V����������(O,D)
//---------------------------------------------------------------------------
UnicodeString get_SyncDstList(
	UnicodeString dnam,		//�f�B���N�g����
	TStringList *lst,
	bool del_sw, 			//�����폜			(default = false)
	UnicodeString ex_dnam)	//���O�f�B���N�g��	(default = EmptyStr)
{
	dnam = IncludeTrailingPathDelimiter(dnam);

	lst->Clear();
	lst->Add(dnam);

	UnicodeString opt;
	for (int i=0; i<SyncDirList->Count; i++) {
		//"�^�C�g��","�L��:1/����:0","�I�v�V����","dir1","dir2",...
		TStringDynArray syn_lst = get_csv_array(SyncDirList->Strings[i], 50);	//***
		if (syn_lst.Length<5)		continue;	//�s��
		if (equal_0(syn_lst[1]))	continue;	//����
		if (del_sw && !ContainsText(syn_lst[2], "D")) continue;
		//�����Ώۂ����邩?
		for (int j=3; j<syn_lst.Length; j++) syn_lst[j] = IncludeTrailingPathDelimiter(syn_lst[j]);
		bool flag = false;
		UnicodeString snam;	//�T�u�f�B���N�g������
		for (int j=3; j<syn_lst.Length; j++) {
			if (StartsText(syn_lst[j], dnam)) {
				snam = dnam;
				snam.Delete(1, syn_lst[j].Length());
				flag = true;
				break;
			}
		}
		if (!flag) continue;

		//�����Ώۂ���
		opt = syn_lst[2];
		for (int j=3; j<syn_lst.Length; j++) {
			UnicodeString pnam = syn_lst[j] + snam;
			if (!SameText(dnam, pnam) && !SameText(ex_dnam, pnam) && dir_exists(pnam)) lst->Add(pnam);
		}
		break;
	}

	return opt;
}

//---------------------------------------------------------------------------
//�������ׂ��f�B���N�g�������邩?
//---------------------------------------------------------------------------
bool has_SyncDir(
	UnicodeString dnam,
	bool del_sw)			//�����폜	(default = false)
{
	std::unique_ptr<TStringList> d_lst(new TStringList());
	get_SyncDstList(dnam, d_lst.get(), del_sw);
	return (d_lst->Count>1);
}

//---------------------------------------------------------------------------
//�f�B���N�g�����݂��ɓ����֌W�ɂ��邩?
//  �߂�l: S | [1][2]  S=����, 1=dnam1���o�^, 2=dnam2���o�^
//---------------------------------------------------------------------------
UnicodeString is_SyncDir(UnicodeString dnam1, UnicodeString dnam2)
{
	UnicodeString ret_str;
	dnam1 = IncludeTrailingPathDelimiter(dnam1);
	dnam2 = IncludeTrailingPathDelimiter(dnam2);

	for (int i=0; i<SyncDirList->Count; i++) {
		//"�^�C�g��","�L��:1/����:0","�I�v�V����","dir1","dir2",...
		TStringDynArray syn_lst = get_csv_array(SyncDirList->Strings[i], 50);
		if (syn_lst.Length<5 || equal_0(syn_lst[1])) continue;

		for (int j=3; j<syn_lst.Length; j++) syn_lst[j] = IncludeTrailingPathDelimiter(syn_lst[j]);
		bool flag1 = false, flag2 = false;
		UnicodeString snam1, snam2;	//�T�u�f�B���N�g������
		for (int j=3; j<syn_lst.Length; j++) {
			UnicodeString inam = syn_lst[j];
			if (StartsText(inam, dnam1)) {
				snam1 = dnam1;  snam1.Delete(1, inam.Length());  flag1 = true;
			}
			if (StartsText(inam, dnam2)) {
				snam2 = dnam2;  snam2.Delete(1, inam.Length());  flag2 = true;
			}
		}

		if (flag1 || flag2) {
			if (flag1 && flag2 && !SameText(dnam1, dnam2) && SameText(snam1, snam2)) {
				ret_str = "S";  break;
			}
			if (flag1) ret_str += "1";
			if (flag2) ret_str += "2";
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�ꎞ�f�B���N�g���̐�΃p�X��ݒ�
// ��d�N�����l��
//---------------------------------------------------------------------------
void SetTempPathA(UnicodeString dnam)
{
	dnam = ExcludeTrailingPathDelimiter(to_absolute_name(dnam));
	if (!IsPrimary) dnam.cat_sprintf(_T("_%d\\"), NyanFiIdNo);
	TempPathA = IncludeTrailingPathDelimiter(dnam);
	TempPathFTP.sprintf(_T("%s" TMP_FTP_D), TempPathA.c_str());
}

//---------------------------------------------------------------------------
//�ꎞ�f�B���N�g�����N���A
//---------------------------------------------------------------------------
void ClearTempDir(UnicodeString dnam)
{
	if (!is_EmptyDir(TempPathA + dnam)) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		get_files(TempPathA + dnam, _T("*.*"), fbuf.get(), true);
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString fnam = fbuf->Strings[i];
			if (set_FileWritable(fnam)) {
				DeleteFile(fnam);
				del_CachedIcon(fnam);
			}
		}
	}
}
//---------------------------------------------------------------------------
//�A�[�J�C�u�p�̈ꎞ�f�B���N�g�����폜
//---------------------------------------------------------------------------
void ClearTempArc(
	UnicodeString dnam)		//�f�B���N�g���� (default = EmptyStr : ���ׂč폜)
{
	UnicodeString sea_str = dnam.IsEmpty()? (TempPathA + TMP_ARC_P) : ExcludeTrailingPathDelimiter(dnam);
	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			if ((sr.Attr & faDirectory)==0) continue;
			UnicodeString snam = TempPathA + sr.Name;
			std::unique_ptr<TStringList> fbuf(new TStringList());
			get_files(snam, _T("*.*"), fbuf.get(), true);
			for (int i=0; i<fbuf->Count; i++) {
				UnicodeString fnam = fbuf->Strings[i];
				if (set_FileWritable(fnam)) {
					DeleteFile(fnam);
					del_CachedIcon(fnam);
				}
			}
			delete_Dirs(snam);
		} while(FindNext(sr)==0);
		FindClose(sr);
	}
}
//---------------------------------------------------------------------------
void ClearTempArcList(int tag)
{
	flist_stt *lst_stt = &ListStt[tag];
	int i = 0;
	while (i < lst_stt->arc_TmpList->Count) {
		UnicodeString dnam = lst_stt->arc_TmpList->Strings[i];
		ClearTempArc(dnam);
		if (!dir_exists(dnam)) lst_stt->arc_TmpList->Delete(i); else i++;
	}
}
//---------------------------------------------------------------------------
//�A�[�J�C�u�p�̈ꎞ�f�B���N�g�����X�V
//  tag �w��̏ꍇ�Aarc_TmpList �ɓo�^
//---------------------------------------------------------------------------
UnicodeString UpdateTempArcList(int tag)
{
	//�����̈ꎞ�f�B���N�g�����폜
	if (tag!=-1) ClearTempArcList(tag);

	//�V�K�ꎞ�f�B���N�g����ǉ�
	int n = 0;
	while (file_exists(TempPathA + UnicodeString().sprintf(_T(TMP_ARC_F), n))) n++;
	UnicodeString tmp_path = TempPathA + UnicodeString().sprintf(_T(TMP_ARC_F), n) + "\\";
	if (create_ForceDirs(tmp_path)) {
		if (tag==0 || tag==1) {
			flist_stt *lst_stt = &ListStt[tag];
			lst_stt->arc_TmpList->Insert(0, tmp_path);
		}
		return tmp_path;
	}
	else {
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//�A�[�J�C�u���t�@�C���̈ꎞ�𓀏���
//  fp->tmp_name �Ɉꎞ�t�@�C������ݒ�
//---------------------------------------------------------------------------
bool SetTmpFile(
	file_rec *fp,
	bool not_unpk,		//�𓀂����Ɉꎞ�t�@�C������ݒ肷�邾�� (default = false)
	bool show_prg)		//�i���󋵂�\�� (default = false)
{
	if (!fp || fp->is_dir || !fp->is_virtual || fp->arc_name.IsEmpty()) return false;

	int tag = fp->tag;
	if (tag==-1) tag = CurListTag;
	flist_stt *lst_stt = &ListStt[tag];

	try {
		UnicodeString tmp_path;
		bool is_v_item = contains_Slash(fp->f_name);
		if (!file_exists(fp->tmp_name) || is_v_item) {
			//�A�[�J�C�u������
			if (is_v_item) {
				if (!not_unpk) UpdateTempArcList(tag);
				tmp_path = lst_stt->arc_TmpList->Strings[0];
			}
			//���d�A�[�J�C�u
			else if (test_ArcExt2(fp->f_ext)) {
				tmp_path = UpdateTempArcList();
				if (tmp_path.IsEmpty()) Abort();
			}
			//���̑�
			else {
				tmp_path = lst_stt->arc_TmpList->Strings[0];
			}

			UnicodeString tmp_name;
			tmp_name.sprintf(_T("%s%s%s"), tmp_path.c_str(), fp->b_name.c_str(), fp->f_ext.c_str());

			if (!not_unpk && !file_exists(tmp_name)) {
				fp->tmp_name = EmptyStr;
				if (!is_X64() && SPI->TestFExt(get_extension(fp->arc_name), true)) {
					if (!SPI->UnPack(fp->arc_name, fp->f_name, tmp_path)) Abort();
				}
				else {
					UnicodeString fnam = fp->f_name;
					if (contains_Slash(fnam)) fnam = get_tkn_r(fnam, '/');
					UnicodeString res_file = make_ResponseFile(fnam, usr_ARC->GetArcType(fp->arc_name, true));
					if (res_file==RESPONSE_ERR) Abort();
					if (!res_file.IsEmpty()) fnam = "@" + res_file;
					bool ok = usr_ARC->UnPack(fp->arc_name, tmp_path, add_quot_if_spc(fnam), false, !show_prg);
					delete_FileIf(res_file);
					if (!ok) {
						UnicodeString msg = make_LogHdr(_T("UNPACK"), fnam);
						msg[1] = 'E';
						set_LogErrMsg(msg, LoadUsrMsg(USTR_FaildTmpUnpack));
						msg += usr_ARC->ErrMsg;
						AddLog(msg);
						Abort();
					}
				}
			}
			fp->tmp_name = tmp_name;
		}
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//ZIP �A�[�J�C�u���̓K���ȉ摜�������ĉ�
//---------------------------------------------------------------------------
UnicodeString ExtractInZipImg(
	UnicodeString arc_file,		//�A�[�J�C�u�t�@�C����
	UnicodeString img_fext)		//�Ή��摜�g���q
{
	if (!test_FileExt(get_extension(arc_file), FEXT_ZIPIMG)) return EmptyStr;

	UnicodeString i_fnam;
	std::unique_ptr<TZipFile> zp(new TZipFile());

	try {
		zp->Open(arc_file, zmRead);

		UnicodeString i_cover, i_first;
		for (int i=0; i<zp->FileCount; i++) {
			UnicodeString fnam = zp->FileName[i];
			UnicodeString fext = get_extension(fnam);
			if (test_FileExt(fext, img_fext)) {
				if (i_first.IsEmpty()) i_first = fnam;
				if (i_cover.IsEmpty()
					&& (ContainsText(fnam, "cover") || ContainsText(fnam, "title") || ContainsText(fnam, "page")))
						i_cover = fnam;
			}
		}

		UnicodeString znam = !i_cover.IsEmpty()? i_cover : i_first;
		if (!znam.IsEmpty()) {
			i_fnam = TempPathA + ExtractFileName(ReplaceStr(znam, "/", "\\"));
			zp->Extract(znam, TempPathA, false);
		}
	}
	catch (...) {
		if (file_exists(i_fnam)) DeleteFile(i_fnam);
		i_fnam = EmptyStr;
	}

	zp->Close();

	return i_fnam;
}

//---------------------------------------------------------------------------
//���k�Ώۃ��X�g�ɍ��ڂ�ǉ�
//---------------------------------------------------------------------------
void add_PackItem(file_rec *fp, int arc_t, UnicodeString src_dir, TStringList *lst)
{
	UnicodeString fnam = fp->n_name;
	if (fp->is_dir) {
		if (arc_t==UARCTYP_CAB) {
			//cabXX.dll �� -r �̑�֍�
			std::unique_ptr<TStringList> fbuf(new TStringList());
			get_files(fp->f_name, _T("*.*"), fbuf.get(), true);
			for (int i=0; i<fbuf->Count; i++) {
				UnicodeString lbuf = fbuf->Strings[i];
				lst->Add(lbuf.Delete(1, src_dir.Length()));
			}
		}
		else {
			//�J�����g�̋�f�B���N�g���̓}�X�N��t���Ȃ�
			if (!is_EmptyDir(fp->f_name)) fnam += "\\*";
			lst->Add(fnam);
		}
	}
	else lst->Add(fnam);
}

//---------------------------------------------------------------------------
//������UNC�p�X��?
//---------------------------------------------------------------------------
bool is_InvalidUnc(UnicodeString dnam,
	bool del_sw)	//�����������ڂ��폜 (default = false);
{
	int idx = -1;
	if (StartsStr("\\\\", dnam)) {
		idx = InvalidUncList->IndexOf(dnam);
		if (idx!=-1 && del_sw) InvalidUncList->Delete(idx);
	}

	return (idx != -1);
}

//---------------------------------------------------------------------------
//�p�X�̑��݂��`�F�b�N���A������Η��p�\�ȃp�X���擾
//---------------------------------------------------------------------------
UnicodeString CheckAvailablePath(UnicodeString dnam, int tag)
{
	if (!StartsStr("\\\\", dnam)  && is_root_dir(dnam) && is_drive_accessible(dnam)) return dnam;

	//�f�B���N�g���̑��݃`�F�b�N(�Ȃ���ΐe������)
	UnicodeString drvnam = ExtractFileDrive(dnam);
	if (!dnam.IsEmpty()) {
		if (is_InvalidUnc(dnam)) dnam = EmptyStr;

		if (StartsStr("\\\\", dnam) || is_drive_accessible(dnam)) {
			bool found = true;
			while (!dir_exists(dnam)) {
				dnam = get_parent_path(dnam);
				if (!contains_PathDlmtr(dnam) && !dir_exists(dnam)) {
					found = false;  break;
				}
			}
			if (found) return dnam;
		}

		//���݂��Ȃ��h���C�u�̗����E�X�^�b�N���ڂ��폜
		for (int i=0; i<MAX_FILELIST+1; i++) {
			TStringList *h_lst = (i<MAX_FILELIST)? get_DirHistory(CurTabIndex, i) : DirStack;
			if (h_lst) {
				int idx = 0;
				while (idx<h_lst->Count)
					if (StartsText(drvnam, get_csv_item(h_lst->Strings[idx], 0))) h_lst->Delete(idx); else idx++;
			}
		}
	}

	//��������T��
	TStringList *h_lst = get_DirHistory(CurTabIndex, tag);
	if (h_lst && h_lst->Count>0) {
		dnam = get_csv_item(h_lst->Strings[0], 0);
	}
	//���݂���h���C�u����T��
	else {
		dnam = EmptyStr;
		DWORD d_bit  = ::GetLogicalDrives();	//���p�\�ȃh���C�u���r�b�g�}�X�N�`���Ŏ擾
		DWORD d_flag = 0x00000001;
		for (int d_n = _T('A'); d_n <= _T('Z'); d_n++,d_flag<<=1 ){
			if ((d_bit & d_flag)==0) continue;
			drvnam.sprintf(_T("%c:\\"), d_n);
			if (::GetDriveType(drvnam.c_str())==DRIVE_FIXED) {
				dnam = drvnam;  break;
			}
		}
	}
	return dnam;
}

//---------------------------------------------------------------------------
//���݂̃c�[���{�^����`���X�g���擾
//---------------------------------------------------------------------------
TStringList *GetCurBtnList(int scr_mode)
{
	return (scr_mode==SCMD_FLIST)? ToolBtnList :
		   (scr_mode==SCMD_TVIEW)? ToolBtnListV :
		   (scr_mode==SCMD_IVIEW)? ToolBtnListI : NULL;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g���擾
//---------------------------------------------------------------------------
TStringList* GetFileList(int tag)
{
	if (tag==-1) return NULL;

	flist_stt *cur_stt = &ListStt[tag];
	return cur_stt->is_Arc?  ArcFileList[tag] :
		   cur_stt->is_ADS?  AdsFileList[tag] :
		   cur_stt->is_Find? ResultList[tag] :
		   cur_stt->is_Work? WorkList: FileList[tag];
}
//---------------------------------------------------------------------------
//�J�����g�̃��X�g���擾
//---------------------------------------------------------------------------
TStringList* GetCurList(
	bool only_filer)	//�t�@�C���[�݂̂�Ώ� (default = false)
{
	return ((!only_filer && ScrMode==SCMD_IVIEW)? ViewFileList : GetFileList(CurListTag));
}
//---------------------------------------------------------------------------
//���΃p�X�̃��X�g���擾
//---------------------------------------------------------------------------
TStringList* GetOppList()
{
	return GetFileList(OppListTag);
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�̔w�i�F���擾
//---------------------------------------------------------------------------
TColor get_FlBgColor(flist_stt *lst_stt, int idx)
{
	return lst_stt->is_Find? 	col_bgFind :
		   (lst_stt->is_Arc)?	col_bgArc :
		   lst_stt->is_Work?	col_bgWork :
		   lst_stt->is_FTP?		col_bgFTP :
		   lst_stt->is_ADS?		col_bgADS :
		   is_AltLnBgCol(idx)?	col_bgList2 : col_bgList;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�̃\�[�g
//---------------------------------------------------------------------------
void SortList(TStringList *lst, int tag)
{
	flist_stt *lst_stt = &ListStt[tag];
	if (lst_stt->is_Work && (NotSortWorkList || WorkListHasSep)) return;

	NaturalOrder = FlOdrNatural[tag];
	DscNameOrder = FlOdrDscName[tag];
	SmallOrder   = FlOdrSmall[tag];
	OldOrder	 = FlOdrOld[tag];
	DscAttrOrder = FlOdrDscAttr[tag];
	DscPathOrder = FlOdrDscPath[tag];

	if (lst_stt->is_Find && lst_stt->find_PathSort) {
		lst->CustomSort(SortComp_PathName);		//�ꏊ
	}
	else {
		switch (SortMode[tag]) {
		case 1:  lst->CustomSort(SortComp_Ext);		break;
		case 2:  lst->CustomSort(SortComp_Time);	break;
		case 3:  lst->CustomSort(SortComp_Size);	break;
		case 4:  lst->CustomSort(SortComp_Attr);	break;
		case 5:	//�Ȃ�
			if (DirSortMode[tag]!=5 && lst->Count>0) {
				//�f�B���N�g������ɏW�߂�
				int i0 = ((file_rec*)lst->Objects[0])->is_up? 1 : 0;	//�W�߂�擪�ʒu
				int i1 = i0;											//���̈ړ���
				for (int i=i0; i<lst->Count; i++) {
					file_rec *fp = (file_rec*)lst->Objects[i];
					if (fp->is_dir) {
						if (i1<i) lst->Move(i, i1);
						i1++;
					}
				}
				int d_cnt = i1 - i0;	//�f�B���N�g����
				if (DirSortMode[tag]>0 && d_cnt>1) {
					//�f�B���N�g�������݂̂��\�[�g
					std::unique_ptr<TStringList> d_lst(new TStringList());
					for (int i=0,j=i0; i<d_cnt; i++,j++) {
						d_lst->AddObject(lst->Strings[j], lst->Objects[j]);
					}
					d_lst->CustomSort(SortComp_DirOnly);
					for (int i=0,j=i0; i<d_lst->Count; i++,j++) {
						lst->Strings[j] = d_lst->Strings[i];
						lst->Objects[j] = d_lst->Objects[i];
					}
				}
			}
			break;
		default: lst->CustomSort(SortComp_Name);
		}
	}
}

//---------------------------------------------------------------------------
//�t�@�C���X�g�ō��ڃT�[�`
//---------------------------------------------------------------------------
int IndexOfFileList(
	UnicodeString fnam,	//�t�@�C����
	int tag,			//�^�O				(default = CurListTag)
	int top_idx)		//�擪�C���f�b�N�X	(default = -1)
{
	if (fnam.IsEmpty()) return -1;

	TStringList *lst = GetFileList(tag);
	int idx = lst->IndexOf(fnam);

	if (idx!=-1) {
		TListBox *lp = FileListBox[tag];
		if (top_idx!=-1) lp->TopIndex = top_idx;
		lp->ItemIndex = idx;
	}

	return idx;
}

//---------------------------------------------------------------------------
file_rec* ExistsInList(UnicodeString fnam, TStringList *lst)
{
	file_rec *ret_fp = NULL;
	for (int i=0; i<lst->Count && !ret_fp; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (SameText(fp->n_name, fnam)) ret_fp = fp;
	}
	return ret_fp;
}

//---------------------------------------------------------------------------
//�I�𒆃t�@�C���̃��X�g��������擾
//�I���s������ EAbort ��O
//---------------------------------------------------------------------------
UnicodeString GetSelFileStr(
	TStringList *lst,
	bool use_tmp,			//�A�[�J�C�u���̃t�@�C�����ꎞ��	(default = false)
	bool inc_dir,			//�f�B���N�g�����܂߂�				(default = false)
	TStringList *s_lst)		//[o] �I�����X�g					(default = NULL)
{
	UnicodeString f_str;
	if (s_lst) s_lst->Clear();

	for (int i=0; i<lst->Count; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (!fp->selected) continue;
		if (!inc_dir && fp->is_dir) continue;

		UnicodeString fnam;
		if (use_tmp && fp->is_virtual) {
			if (!SetTmpFile(fp)) UserAbort(USTR_FaildTmpUnpack);
			fnam = fp->tmp_name;
		}
		else {
			fnam = fp->f_name;
		}

		if (!f_str.IsEmpty()) f_str += " ";
		f_str += add_quot_if_spc(fnam);

		if (s_lst) s_lst->Add(add_quot_if_spc(fnam));
	}
	return f_str;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�̃N���A
//---------------------------------------------------------------------------
void clear_FileList(TStringList *lst)
{
	for (int i=0; i<lst->Count; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp) {
			delete fp->inf_list;
			delete fp;
		}
	}
	lst->Clear();
}
//---------------------------------------------------------------------------
//�t�@�C�����X�g�̍��ڍ폜
//---------------------------------------------------------------------------
void del_FileListItem(TStringList *lst, int idx)
{
	if (idx>=0 && idx<lst->Count) {
		file_rec *fp = (file_rec*)lst->Objects[idx];
		if (fp) {
			delete fp->inf_list;
			delete fp;
		}
		lst->Delete(idx);
	}
}

//---------------------------------------------------------------------------
//�I��L��
//---------------------------------------------------------------------------
bool ListSelected(TStringList *lst)
{
	bool res = false;
	if (lst) for (int i=0; i<lst->Count && !res; i++) res = ((file_rec*)lst->Objects[i])->selected;
	return res;
}
//---------------------------------------------------------------------------
//�I�𒆂̍��ڐ�
//---------------------------------------------------------------------------
int GetSelCount(TStringList *lst)
{
	int cnt = 0;
	if (lst) {
		for (int i=0; i<lst->Count; i++) if (((file_rec*)lst->Objects[i])->selected) cnt++;
	}
	return cnt;
}

//---------------------------------------------------------------------------
//�I�𒆍��ږ����X�g���擾
//---------------------------------------------------------------------------
void GetSelList(
	TStringList *lst,
	TStringList *sel_lst,
	bool with_path,		//�p�X�t�Ŏ擾		(default = true)
	bool with_obj)		//Objects ���擾	(default = false)
{
	if (lst && sel_lst) {
		sel_lst->Clear();
		for (int i=0; i<lst->Count; i++) {
			file_rec *fp = (file_rec*)lst->Objects[i];
			if (fp->selected) {
				if (with_obj)
					sel_lst->AddObject(with_path? fp->f_name : fp->n_name, (TObject*)fp);
				else
					sel_lst->Add(with_path? fp->f_name : fp->n_name);
			}
		}
	}
}

//---------------------------------------------------------------------------
//�I����Ԃ�����
//---------------------------------------------------------------------------
void ClrSelect(TStringList *lst)
{
	if (!lst) lst = GetCurList();
	for (int i=0; i<lst->Count; i++) ((file_rec*)lst->Objects[i])->selected = false;
}

//---------------------------------------------------------------------------
//�I�𒆂̍��ڐ�
//---------------------------------------------------------------------------
int GetMatchCount(TStringList *lst)
{
	int cnt = 0;
	if (lst) {
		for (int i=0; i<lst->Count; i++) if (((file_rec*)lst->Objects[i])->matched) cnt++;
	}
	return cnt;
}

//---------------------------------------------------------------------------
//file_rec �𐶐����ď�����
//  copy_fp ���w�肷��ƁA���̓��e���R�s�[
//---------------------------------------------------------------------------
file_rec* cre_new_file_rec(file_rec *copy_fp)
{
	file_rec *fp = new file_rec;
	fp->inf_list = new TStringList();
	fp->matched  = false;
	fp->failed	 = false;

	if (copy_fp) {
		fp->f_name	   = copy_fp->f_name;
		fp->p_name	   = copy_fp->p_name;
		fp->n_name	   = copy_fp->n_name;
		fp->b_name	   = copy_fp->b_name;
		fp->f_ext	   = copy_fp->f_ext;
		fp->alias	   = copy_fp->alias;
		fp->arc_name   = copy_fp->arc_name;
		fp->tmp_name   = copy_fp->tmp_name;
		fp->r_name	   = copy_fp->r_name;
		fp->l_name	   = copy_fp->l_name;
		fp->memo	   = copy_fp->memo;
		fp->tags	   = copy_fp->tags;

		fp->tag 	   = copy_fp->tag;
		fp->is_up	   = copy_fp->is_up;
		fp->is_dir	   = copy_fp->is_dir;
		fp->is_sym	   = copy_fp->is_sym;
		fp->is_jct	   = copy_fp->is_jct;
		fp->is_virtual = copy_fp->is_virtual;
		fp->is_ftp	   = copy_fp->is_ftp;
		fp->is_ads	   = copy_fp->is_ads;
		fp->is_dummy   = copy_fp->is_dummy;
		fp->selected   = copy_fp->selected;
		fp->f_size	   = copy_fp->f_size;
		fp->f_attr	   = copy_fp->f_attr;
		fp->attr_str   = copy_fp->attr_str;
		fp->f_time	   = copy_fp->f_time;
		fp->o_size	   = copy_fp->o_size;
		fp->c_size	   = copy_fp->c_size;
		fp->f_count    = copy_fp->f_count;
		fp->d_count    = copy_fp->d_count;
		fp->img_ori    = copy_fp->img_ori;
		fp->distance   = copy_fp->distance;
		fp->is_video   = copy_fp->is_video;
		fp->inf_list->Assign(copy_fp->inf_list);
		fp->prv_text   = copy_fp->prv_text;
		fp->tail_text  = copy_fp->tail_text;
	}
	else {
		fp->tag 	   = -1;
		fp->is_up	   = false;
		fp->is_dir	   = false;
		fp->is_sym	   = false;
		fp->is_jct	   = false;
		fp->is_virtual = false;
		fp->is_ftp	   = false;
		fp->is_ads	   = false;
		fp->is_dummy   = false;
		fp->selected   = false;
		fp->f_size	   = -1;
		fp->f_attr	   = 0;
		fp->f_time	   = 0;
		fp->o_size	   = -1;
		fp->c_size	   = -1;
		fp->f_count    = -1;
		fp->d_count    = -1;
		fp->img_ori    = 0;
		fp->distance   = 0;
		fp->is_video   = false;
	}

	return fp;
}
//---------------------------------------------------------------------------
//�t�@�C�������� file_rec �𐶐�
//---------------------------------------------------------------------------
file_rec* cre_new_file_rec(
	UnicodeString fnam,		//�t�@�C����(������ \ �̏ꍇ�̓f�B���N�g����)
	int  tag,				//�^�O							(default = -1)
	bool force)				//���݂��Ȃ��ꍇ�������I�ɐݒ�	(default = false)
{
	file_rec *fp = cre_new_file_rec();
	fp->tag = tag;

	if (fnam.IsEmpty()) {
		fp->is_dummy = true;
	}
	else if (is_separator(fnam)) {
		fp->alias	 = "-";
		fp->is_dummy = true;
	}
	else if (USAME_TS(fnam, "..")) {
		fp->is_up  = true;
		fp->is_dir = true;
		fp->f_name = "..";
		fp->n_name = "..";
		fp->b_name = "..";
	}
	else {
		bool is_dir = ends_PathDlmtr(fnam);
		if (is_dir) fnam = ExcludeTrailingPathDelimiter(fnam);
		fp->f_attr	= file_GetAttr(fnam);
		bool exist  = (fp->f_attr!=faInvalid);
		if (exist || force) {
			fp->is_dummy = !exist && force;
			fp->f_name = fp->r_name = fnam;
			fp->p_name = ExtractFilePath(fnam);
			fp->n_name = ExtractFileName(fnam);

			int p = fnam.Length() - fp->n_name.Length();
			fp->is_ads = (p>2 && fnam[p]==':');

			if (is_dir) {
				fp->is_dir = true;
				fp->b_name = ExtractFileName(fnam);
			}
			else {
				fp->b_name = get_base_name(fnam);
				fp->f_ext  = get_extension(fnam);
			}

			if (exist) {
				fp->f_time	 = get_file_age(fnam);
				fp->is_dir	 = (fp->f_attr & faDirectory);
				fp->is_sym	 = (fp->f_attr & faSymLink);
				if (fp->is_sym) fp->l_name = get_ReparsePointTarget(fnam, fp->is_jct);
				fp->attr_str = get_file_attr_str(fp->f_attr);
				if (!fp->is_dir) fp->f_size = get_file_size(fnam);
			}
		}
	}

	return fp;
}
//---------------------------------------------------------------------------
file_rec* cre_new_up_rec(int tag)
{
	file_rec *fp = cre_new_file_rec();
	fp->tag 	 = tag;
	fp->is_up	 = true;
	fp->is_dir	 = true;
	fp->f_name	 = "..";
	fp->n_name	 = "..";
	fp->b_name	 = "..";
	fp->attr_str = get_file_attr_str(fp->f_attr);
	return fp;
}

//---------------------------------------------------------------------------
void del_file_rec(file_rec *fp)
{
	if (fp) {
		delete fp->inf_list;
		delete fp;
	}
}

//---------------------------------------------------------------------------
void inv_file_rec(file_rec *fp)
{
	if (fp) {
		fp->f_size	 = -1;
		fp->f_attr	 = faInvalid;
		fp->f_time	 = 0;
		fp->attr_str = get_file_attr_str(fp->f_attr);
	}
}

//---------------------------------------------------------------------------
// FileList�̓��e�����ւ���
//---------------------------------------------------------------------------
void swap_FileList(TStringList *lst1, TStringList *lst2, bool swap_tag)
{
	std::unique_ptr<TStringList> sbuf(new TStringList());
	sbuf->Assign(lst1); lst1->Assign(lst2); lst2->Assign(sbuf.get());

	//�^�O�̓���ւ�
	if (swap_tag) {
		for (int i=0; i<lst1->Count; i++) {
			file_rec *fp = (file_rec*)lst1->Objects[i];
			fp->tag = (fp->tag==0)? 1 : 0;
		}
		for (int i=0; i<lst2->Count; i++) {
			file_rec *fp = (file_rec*)lst2->Objects[i];
			fp->tag = (fp->tag==0)? 1 : 0;
		}
	}
}

//---------------------------------------------------------------------------
//�^�u���X�g�̏���
//---------------------------------------------------------------------------
tab_info* cre_tab_info()
{
	tab_info *tp = new tab_info;
	for (int i=0; i<MAX_FILELIST; i++) {
		tp->sel_list[i]   = new TStringList();
		tp->dir_hist[i]   = new TStringList();
		tp->dir_hist_p[i] = 0;
	}
	return tp;
}
//---------------------------------------------------------------------------
int add_TabList(UnicodeString item)
{
	return TabList->AddObject(item, (TObject*)cre_tab_info());
}
//---------------------------------------------------------------------------
void insert_TabList(int idx, UnicodeString item)
{
	TabList->InsertObject(idx, item, (TObject*)cre_tab_info());
}

//---------------------------------------------------------------------------
void del_tab_info(tab_info *tp)
{
	if (tp) {
		for (int i=0; i<MAX_FILELIST; i++) {
			delete tp->sel_list[i];
			delete tp->dir_hist[i];
		}
		delete tp;
	}
}

//---------------------------------------------------------------------------
tab_info *get_TabInfo(int tab_idx)
{
	if (tab_idx==-1) tab_idx = CurTabIndex;
	if (tab_idx>=0 && tab_idx<TabList->Count) return (tab_info*)TabList->Objects[tab_idx];
	return NULL;
}

//---------------------------------------------------------------------------
TStringList *get_DirHistory(int tab_idx, int tag)
{
	tab_info *tp = get_TabInfo(tab_idx);
	return (tp? tp->dir_hist[tag] : NULL);
}
//---------------------------------------------------------------------------
int *get_DirHistPtr(int tab_idx, int tag)
{
	tab_info *tp = get_TabInfo(tab_idx);
	return (tp? &tp->dir_hist_p[tag] : NULL);
}
//---------------------------------------------------------------------------
void clear_DirHistory(int tab_idx, int tag)
{
	TStringList *h_lst = get_DirHistory(tab_idx, tag);
	if (h_lst) h_lst->Clear();
}

//---------------------------------------------------------------------------
UnicodeString get_TabWorkList(int tab_idx)
{
	if (tab_idx>=0 && tab_idx<TabList->Count) {
		TStringDynArray itm_buf = get_csv_array(TabList->Strings[tab_idx], TABLIST_CSVITMCNT, true);
		int w_mod = itm_buf[6].ToIntDef(0);
		return to_absolute_name((w_mod==1)? HomeWorkList : (w_mod==2)? itm_buf[7] : EmptyStr);
	}
	else {
		return EmptyStr;
	}
}

//---------------------------------------------------------------------------
//�^�u�O���[�v�̕ۑ�
//---------------------------------------------------------------------------
bool save_TagGroup(UnicodeString fnam)
{
	if (fnam.IsEmpty()) return false;

	std::unique_ptr<UsrIniFile> tab_file(new UsrIniFile(fnam));
	UnicodeString sct = "General";
	tab_file->WriteInteger(sct, "CurTabIndex",	CurTabIndex);

	sct = "TabList";
	tab_file->SaveListItems(sct, TabList, 30);

	//�f�B���N�g�������̐i�ޕ������폜
	for (int i=0; i<TabList->Count; i++) {
		for (int j=0; j<MAX_FILELIST; j++) {
			TStringList *h_lst = get_DirHistory(i, j);
			if (h_lst) {
				int *h_ptr = get_DirHistPtr(i, j);
				if (h_ptr) for (int k=0; k<*h_ptr && h_lst->Count>0; k++) h_lst->Delete(0);
			}
		}
	}
	//�f�B���N�g��������ۑ�
	for (int i=0; i<MAX_TABLIST; i++) {
		tab_info *tp = get_TabInfo(i);
		for (int j=0; j<MAX_FILELIST; j++) {
			sct.sprintf(_T("DirHistory%02u_%u"), i + 1, j);
			if (tp)
				tab_file->SaveListItems(sct, tp->dir_hist[j], 30);
			else
				tab_file->EraseSection(sct);
		}
	}

	return tab_file->UpdateFile(true);
}

//---------------------------------------------------------------------------
//�����ݒ�̃N���A
//---------------------------------------------------------------------------
void clear_FindStt(flist_stt *lst_stt)
{
	lst_stt->is_Find	  = false;
	lst_stt->find_Loaded  = false;

	lst_stt->find_Dir	  = false;
	lst_stt->find_Both	  = false;
	lst_stt->find_SubDir  = false;
	lst_stt->find_Arc	  = false;
	lst_stt->find_MARK	  = false;
	lst_stt->find_TAG	  = false;
	lst_stt->find_TAG_all = false;
	lst_stt->find_DUPL	  = false;
	lst_stt->find_HLINK   = false;
	lst_stt->find_DICON   = false;

	lst_stt->find_RegEx   = false;
	lst_stt->find_And	  = false;
	lst_stt->find_Case	  = false;

	lst_stt->find_Warn	  = false;
	lst_stt->find_hasAds  = false;
	lst_stt->find_useProc = false;

/*	���ʃ��X�g�̏�Ԉێ��̂��߈ȉ��͏��������Ȃ�
	lst_stt->find_ResLink = false;
	lst_stt->find_DirLink = false;
	lst_stt->find_PathSort= false;
*/

	lst_stt->find_Path	  = EmptyStr;
	lst_stt->find_DirList = EmptyStr;
	lst_stt->find_Name	  = EmptyStr;
	lst_stt->find_Mask	  = EmptyStr;
	lst_stt->find_Keywd   = EmptyStr;
	lst_stt->find_Tags	  = EmptyStr;
	lst_stt->find_Icons   = EmptyStr;

	lst_stt->find_DT_mode = 0;
	lst_stt->find_SZ_mode = 0;
	lst_stt->find_AT_mode = 0;
	lst_stt->find_TM_mode = 0;

	lst_stt->find_codepage = -1;
	lst_stt->find_BOM_mode = 0;
	lst_stt->find_LineBrk  = EmptyStr;

	lst_stt->find_RT_mode  = 0;
	lst_stt->find_FS_mode  = 0;
	lst_stt->find_FW_mode  = 0;
	lst_stt->find_FH_mode  = 0;

	lst_stt->find_IW_mode  = 0;
	lst_stt->find_IH_mode  = 0;

	lst_stt->find_SR_mode  = 0;
	lst_stt->find_BT_mode  = 0;
	lst_stt->find_CH_mode  = 0;

	lst_stt->find_IC_mode  = 0;

	lst_stt->find_HL_mode  = 0;

	lst_stt->find_PrpKwd   = EmptyStr;
	lst_stt->find_TxtKwd   = EmptyStr;
	lst_stt->find_ExifKwd  = EmptyStr;
	lst_stt->find_LatLng   = EmptyStr;
}

//---------------------------------------------------------------------------
//�����Ώۂ��S�̂�?
//---------------------------------------------------------------------------
bool is_FindAll(flist_stt *lst_stt)
{
	if (!lst_stt) return false;

	return (lst_stt->is_Find
			 && (lst_stt->find_DICON || lst_stt->find_TAG || (lst_stt->find_MARK && lst_stt->find_SubList->Count==0)));
}

//---------------------------------------------------------------------------
//���������̃`�F�b�N
//---------------------------------------------------------------------------
bool check_int(int v0, int v1, int mode)
{
	return ((mode==1)? (v0<=v1) : (mode==2)? (v0==v1) : (mode==3)? (v0>=v1) : true);
}
//---------------------------------------------------------------------------
bool check_prop(UnicodeString fnam, UnicodeString prop, int v, int mode)
{
	if (mode==0) return true;

	UnicodeString vstr = usr_SH->get_PropStr(fnam, prop);
	return !vstr.IsEmpty()? check_int(extract_int(vstr), v, mode) : false;
}

//---------------------------------------------------------------------------
//�W���t�@�C������
//�I�s���Ȑ��K�\���p�^�[����p���Ȃ�����(�v���O�`�F�b�N)
//---------------------------------------------------------------------------
bool check_file_std(
	UnicodeString fnam,		//�p�X���t�@�C����
	TDateTime f_tm, __int64 f_sz, int f_atr, flist_stt *lst_stt)
{
	//�L�[���[�h
	if (!lst_stt->find_Keywd.IsEmpty()) {
		UnicodeString kwd = lst_stt->find_Keywd;
		bool is_regex = lst_stt->find_RegEx;
		//�_�u���N�H�[�e�[�V�����ň͂܂�Ă�����󔒂��܂ތ�Ƃ��Đ��K�\����
		if (is_quot(kwd)) {
			kwd = TRegEx::Escape(exclude_quot(kwd));
			if (ContainsStr(kwd, " ")) kwd = ReplaceStr(kwd, " ", "\\s");
			is_regex = true;
		}
		//����
		if (is_regex) {
			TRegExOptions opt;  if (!lst_stt->find_Case) opt << roIgnoreCase;
			if (!TRegEx::IsMatch(fnam, kwd, opt)) return false;
		}
		else {
			if (!find_mlt(kwd, fnam, lst_stt->find_And, false, lst_stt->find_Case)) return false;
		}
	}
	//�^�C���X�^���v
	if (lst_stt->find_DT_mode>0) {
		if (!lst_stt->find_DT_str.IsEmpty() && lst_stt->find_DT_mode==1) {
			if (!str_match(lst_stt->find_DT_str, format_Date(f_tm))) return false;
		}
		else {
			TValueRelationship res = CompareDate(f_tm, lst_stt->find_DT_value);
			switch (lst_stt->find_DT_mode) {
			case 1: if (res!=EqualsValue)							return false; break;
			case 2: if (res!=EqualsValue && res!=LessThanValue)		return false; break;
			case 3: if (res!=EqualsValue && res!=GreaterThanValue)	return false; break;
			}
		}
	}
	//�T�C�Y
	if (!(f_atr & faDirectory) && lst_stt->find_SZ_mode>0) {
		switch (lst_stt->find_SZ_mode) {
		case 1: if (!(f_sz <= lst_stt->find_SZ_value)) return false; break;
		case 2: if (!(f_sz >= lst_stt->find_SZ_value)) return false; break;
		}
	}
	//����
	if (lst_stt->find_AT_mode>0) {
		switch (lst_stt->find_AT_mode) {
		case 1: if (!(f_atr & lst_stt->find_AT_value)) return false; break;
		case 2: if ((f_atr & lst_stt->find_AT_value))  return false; break;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//�g���t�@�C������
//�I�s���Ȑ��K�\���p�^�[����p���Ȃ�����(�v���O�`�F�b�N)
//---------------------------------------------------------------------------
bool check_file_ex(UnicodeString fnam, flist_stt *lst_stt)
{
	UnicodeString fext = get_extension(fnam);

	if (lst_stt->find_TM_mode>0) {
		try {
			TTime tm = get_duration(fnam)/86400000.0;	//ms/(24*60*60*1000)
			TValueRelationship res = CompareTime(tm, lst_stt->find_TM_value);
			switch (lst_stt->find_TM_mode) {
			case 1: if (res!=EqualsValue && res!=LessThanValue)		Abort(); break;
			case 2: if (res!=EqualsValue)							Abort(); break;
			case 3: if (res!=EqualsValue && res!=GreaterThanValue)	Abort(); break;
			}
		}
		catch (...) {
			return false;
		}
	}

	//�^�O
	if (!lst_stt->find_Tags.IsEmpty()) {
		if (USAME_TS(lst_stt->find_Tags, "*")) {
			if (!usr_TAG->HasTag(fnam)) return false;
		}
		else {
			bool and_sw = (lst_stt->find_Tags.Pos('|')==0);
			UnicodeString tags = ReplaceStr(lst_stt->find_Tags, "|", ";");
			if (!usr_TAG->Match(fnam, tags, and_sw)) return false;
		}
	}

	//�R�[�h�y�[�W/���s�R�[�h
	if (lst_stt->find_codepage!=-1 || !lst_stt->find_LineBrk.IsEmpty()) {
		//�R�[�h�y�[�W
		UnicodeString linebrk;
		bool has_bom;
		int code_page = get_FileCodePage(fnam, &linebrk, &has_bom);	if (code_page==0) return false;
		if (lst_stt->find_codepage!=-1 && code_page!=lst_stt->find_codepage) return false;
		//���s�R�[�h
		if (!lst_stt->find_LineBrk.IsEmpty()) {
			if (USAME_TS(lst_stt->find_LineBrk, "����")) {
				std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				ms->CopyFrom(ms.get(), 0);
				linebrk = get_StreamLineBreak(fs.get(), code_page, true);
			}
			if (!SameStr(linebrk, lst_stt->find_LineBrk)) return false;
		}
		//BOM
		if (lst_stt->find_BOM_mode>0) {
			if (lst_stt->find_BOM_mode==1 && !has_bom) return false;
			if (lst_stt->find_BOM_mode==2 &&  has_bom) return false;
		}
	}

	if (!check_prop(fnam, "�r�b�g ���[�g",	lst_stt->find_RT_value, lst_stt->find_RT_mode)) return false;
	if (!check_prop(fnam, "�t���[����",		lst_stt->find_FS_value, lst_stt->find_FS_mode)) return false;
	if (!check_prop(fnam, "�t���[����",		lst_stt->find_FW_value, lst_stt->find_FW_mode)) return false;
	if (!check_prop(fnam, "�t���[����",		lst_stt->find_FH_value, lst_stt->find_FH_mode)) return false;

	//�T���v�����O���g��
	if (lst_stt->find_SR_mode>0 || lst_stt->find_BT_mode>0 || lst_stt->find_CH_mode>0) {
		try {
			std::unique_ptr<TStringList> lst(new TStringList());
			if (test_FlacExt(fext)) get_FlacInf(fnam, lst.get()); else get_WavInf(fnam, lst.get());
			if (!ContainsStr(lst->Text, " �`��: ")) Abort();
			UnicodeString frmt = get_tkn_m(lst->Text, _T("�`��: "), _T("\r\n"));
			//�T���v�����O
			int sp = split_tkn(frmt, _T("Hz ")).ToInt();
			if (!check_int(sp, lst_stt->find_SR_value, lst_stt->find_SR_mode)) Abort();
			//�r�b�g
			int bt = split_tkn(frmt, _T("bit ")).ToInt();
			switch (lst_stt->find_BT_mode) {
			case 1: if (bt!=8)	Abort(); break;
			case 2: if (bt!=16)	Abort(); break;
			case 3: if (bt!=24)	Abort(); break;
			case 4: if (bt!=32)	Abort(); break;
			}
			//�`�����l��
			switch (lst_stt->find_CH_mode) {
			case 1: if (!USAME_TI(frmt, "���m"))		Abort(); break;
			case 2: if (!USAME_TI(frmt, "�X�e���I"))	Abort(); break;
			case 3: if (!USAME_TI(frmt, "4ch"))			Abort(); break;
			case 4: if (!USAME_TI(frmt, "6ch"))			Abort(); break;
			case 5: if (!USAME_TI(frmt, "8ch"))			Abort(); break;
			}
		}
		catch (...) {
			return false;
		}
	}

	//�摜�T�C�Y
	if (lst_stt->find_IW_mode>0 || lst_stt->find_IH_mode>0) {
		unsigned int i_wd, i_hi;
		if (!get_img_size(fnam, &i_wd, &i_hi)) return false;
		if (lst_stt->find_IWH_max && i_hi>i_wd) std::swap(i_wd, i_hi);
		if (!check_int(i_wd, lst_stt->find_IW_value, lst_stt->find_IW_mode)) return false;
		if (!check_int(i_hi, lst_stt->find_IH_value, lst_stt->find_IH_mode)) return false;
	}

	//�A�C�R����
	if (lst_stt->find_IC_mode>0) {
		int ixn = -1;
		if (test_IcoExt(fext) || test_CurExt(fext))
			get_IconInf(fnam, NULL, &ixn);
		else
			ixn = (int)::ExtractIcon(HInstance, fnam.c_str(), -1);
		if (ixn!=-1 && !check_int(ixn, lst_stt->find_IC_value, lst_stt->find_IC_mode)) return false;
	}

	//�����N�J�E���g��
	if (lst_stt->find_HL_mode>0) {
		int hln = get_HardLinkCount(fnam);
		if (lst_stt->find_HL_mode==1 && hln>lst_stt->find_HL_value)		return false;
	 	if (lst_stt->find_HL_mode==2 && hln!=lst_stt->find_HL_value)	return false;
	 	if (lst_stt->find_HL_mode==3 && hln<lst_stt->find_HL_value)		return false;
	}

	//��փf�[�^�X�g���[��������
	if (lst_stt->find_hasAds) {
		if (get_ADS_count(fnam)==0) return false;
	}

	//�ʃv���Z�X���g�p��
	bool use_Proc = false;
	if (lst_stt->find_useProc) {
		if (get_ProcessingInf(fnam)==0) return false;
		use_Proc = true;
	}

	//�t�@�C�����x��
	if (lst_stt->find_Warn) {
		if (get_filename_warn(fnam)==0) return false;
	}

	//�t�@�C����񕶎��񌟍�
	if (!lst_stt->find_PrpKwd.IsEmpty()) {
		std::unique_ptr<TStringList> lst(new TStringList());	//�t�@�C�����p
		TStringList *i_lst = lst.get();
		add_PropLine(_T("���"), usr_SH->get_FileTypeStr(fnam), i_lst);

		if (use_Proc) 						get_ProcessingInf(fnam, i_lst);

		if (test_FileExt(fext, _T(".pdf"))) get_PdfVer(fnam, i_lst);

		if		(test_AppInfExt(fext))		get_AppInf(fnam,  i_lst);
		else if (test_HtmlExt(fext))		get_HtmlInf(fnam, i_lst);
		else if (test_ExifExt(fext))		get_ExifInf(fnam, i_lst);
		else if (test_FontExt(fext))		get_FontInf(fnam, i_lst);
		else if (test_Mp3Ext(fext))			ID3_GetInf(fnam,  i_lst);
		else if (test_FlacExt(fext))		get_FlacInf(fnam, i_lst);
		else if (USAME_TI(fext, ".opus"))	get_OpusInf(fnam, i_lst);
		else if (test_PngExt(fext))			get_PngInf(fnam,  i_lst);
		else if (test_GifExt(fext))			get_GifInf(fnam,  i_lst);
		else if (test_PspExt(fext))			get_PspInf(fnam,  i_lst);
		else if (USAME_TI(fext, ".webp"))	get_WebpInf(fnam, i_lst);
		else if (test_IcoExt(fext) || test_CurExt(fext))
											get_IconInf(fnam, i_lst);
		else if (test_AniExt(fext))			get_AniInf(fnam, i_lst);
		else if (test_LnkExt(fext))			usr_SH->get_LnkInf(fnam,  i_lst);
		else if (xd2tx_PropExt(fext))		xd2tx_GetInfo(fnam, i_lst);
		else 								usr_SH->get_PropInf(fnam, i_lst);

		if (lst_stt->find_PrpRegEx) {
			TRegExOptions opt;  if (!lst_stt->find_PrpCase) opt << roIgnoreCase;
			if (!TRegEx::IsMatch(i_lst->Text, lst_stt->find_PrpKwd, opt)) return false;
		}
		else {
			if (!find_mlt(lst_stt->find_PrpKwd, i_lst->Text, lst_stt->find_PrpAnd, false, lst_stt->find_PrpCase)) return false;
		}
	}

	//�e�L�X�g
	if (!lst_stt->find_TxtKwd.IsEmpty()) {
		std::unique_ptr<TStringList> f_buf(new TStringList());
		if (xd2tx_TestExt(get_extension(fnam))) {
			if (!xd2tx_Extract(fnam, f_buf.get())) return false;
		}
		else {
			if (load_text_ex(fnam, f_buf.get())==0) return false;
		}
		if (lst_stt->find_TxtRegEx) {
			TRegExOptions opt;
			if (!lst_stt->find_TxtCase) opt << roIgnoreCase;
			opt<<roMultiLine;
			if (!TRegEx::IsMatch(f_buf->Text, lst_stt->find_TxtKwd, opt)) return false;
		}
		else {
			if (!find_mlt(lst_stt->find_TxtKwd, f_buf->Text, lst_stt->find_TxtAnd, false, lst_stt->find_TxtCase)) return false;
		}
	}

	//Exif��񕶎��񌟍�
	if (!lst_stt->find_ExifKwd.IsEmpty()) {
		std::unique_ptr<TStringList> i_lst(new TStringList());
		if (!get_ExifInf(fnam, i_lst.get())) return false;
		if (lst_stt->find_ExifRegEx) {
			TRegExOptions opt;  if (!lst_stt->find_ExifCase) opt << roIgnoreCase;
			if (!TRegEx::IsMatch(i_lst->Text, lst_stt->find_ExifKwd, opt)) return false;
		}
		else {
			if (!find_mlt(lst_stt->find_ExifKwd, i_lst->Text,
				lst_stt->find_ExifAnd, false, lst_stt->find_ExifCase))
					return false;
		}
	}

	//GPS�����͈�
	if (!lst_stt->find_LatLng.IsEmpty()) {
		double lat, lng;
		if (!get_GpsInf(fnam, &lat, &lng)) return false;
		return (GetDistance(lst_stt->find_GpsLat, lst_stt->find_GpsLng, lat, lng) <= lst_stt->find_GpsRange);
	}

	return true;
}

//---------------------------------------------------------------------------
//�t�H���g�����擾
//�߂�l: �t�H���g��(�擪)
//---------------------------------------------------------------------------
UnicodeString get_FontInf(
	UnicodeString fnam,	//�t�H���g�t�@�C����
	TStringList *lst)	//[o] ��񃊃X�g	(default = NULL)
{
	UnicodeString fext = get_extension(fnam);
	if (!test_FontExt(fext)) return EmptyStr;

	std::unique_ptr<TStringList> tmp_lst(new TStringList());

	//�C���X�g�[���ς݃t�H���g
	bool ok = false;
	if (SameText(ExtractFileDir(fnam), usr_SH->KnownGuidToPath(FOLDERID_Fonts))) {
		DWORD dwSize = 0;
		if (lpGetFontResourceInfo && lpGetFontResourceInfo(fnam.c_str(), &dwSize, NULL, 2)) {
			int n = dwSize/sizeof(LOGFONT);
			if (n>0) {
				std::unique_ptr<LOGFONT[]> lfs(new LOGFONT[n]);
				if (lpGetFontResourceInfo(fnam.c_str(), &dwSize, lfs.get(), 2)) {
					//�t�H���g��
					UnicodeString lbuf;
					for (int i=0; i<n; i++) {
						if (i>0) lbuf += "; ";
						lbuf.cat_sprintf(_T("%s"), lfs[i].lfFaceName);
					}
					add_PropLine(_T("�t�H���g��"), lbuf, tmp_lst.get());
					ok = true;
				}
			}
		}
	}

	//�擾���s�܂��̓C���X�g�[������Ă��Ȃ��ꍇ�̓v���p�e�B�擾�����݂�
	if (!ok) {
		usr_SH->get_PropInf(fnam, tmp_lst.get());
		tmp_lst->Text = ReplaceStr(tmp_lst->Text, "�^�C�g��:", "�t�H���g��:");
	}

	//�ŏ��̃t�H���g�����擾
	UnicodeString font_name;
	for (int i=0; i<tmp_lst->Count; i++) {
		UnicodeString lbuf = tmp_lst->Strings[i];
		if (StartsText("�t�H���g��:", Trim(lbuf))) {
			font_name = Trim(get_tkn_m(lbuf, ':', ';'));
			break;
		}
	}

	if (lst) lst->AddStrings(tmp_lst.get());

	if (!font_name.IsEmpty() && lst) {
		bool tmp_flag = (Screen->Fonts->IndexOf(font_name)==-1)?
						(::AddFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL)>0) : false;

		std::unique_ptr<TFont> tmp_font(new TFont());
		tmp_font->Name	 = font_name;
		tmp_font->Height = 100;	//***

		HDC hDc = ::GetDC(MainHandle);
		if (hDc) {
			std::unique_ptr<TCanvas> cv(new TCanvas());
			cv->Handle = hDc;
			cv->Font->Assign(tmp_font.get());
			TEXTMETRIC tm;
			if (::GetTextMetrics(cv->Handle, &tm)) {
				lst->Add(EmptyStr);
				add_PropLine(_T("�����Z�b�g"),		get_NameOfCharSet(tm.tmCharSet), lst);
				add_PropLine(_T("����")	,			get_NameOfWeight(tm.tmWeight), lst);
				add_PropLine(_T("����"),			tm.tmHeight, lst);
				add_PropLine(_T("�A�Z���g"),		tm.tmAscent, lst);
				add_PropLine(_T("�f�B�Z���g"),		tm.tmDescent, lst);
				add_PropLine(_T("�������f�B���O"),	tm.tmInternalLeading, lst);
				add_PropLine(_T("�O�����f�B���O"),	tm.tmExternalLeading, lst);
				add_PropLine(_T("���ϕ�"),			tm.tmAveCharWidth, lst);
				add_PropLine(_T("�ő啝"),			tm.tmMaxCharWidth, lst);
			}
			::ReleaseDC(MainHandle, hDc);
		}

		if (tmp_flag) ::RemoveFontResourceEx(fnam.c_str(), FR_PRIVATE, NULL);
	}

	return font_name;
}

//---------------------------------------------------------------------------
//�h���C�u���X�g���擾
//  �߂�l : �V���ɒǉ����ꂽ�h���C�u���
//---------------------------------------------------------------------------
drive_info *get_DriveInfoList()
{
	drive_info *new_drive = NULL;

	DWORD d_bit  = ::GetLogicalDrives();	//���p�\�ȃh���C�u���r�b�g�}�X�N�`���Ŏ擾
	DWORD d_flag = 0x00000001;
	for (int d_n = _T('A'); d_n <= _T('Z'); d_n++,d_flag<<=1) {
		UnicodeString dstr;
		dstr.sprintf(_T("%c:\\"), d_n);
		int idx = DriveInfoList->IndexOf(dstr);
		drive_info *dp;
		if ((d_bit & d_flag)==0) {
			//���݂��Ȃ��Ȃ����h���C�u�����폜
			if (idx!=-1) {
				dp = (drive_info *)DriveInfoList->Objects[idx];
				delete dp->small_ico;
				delete dp->large_ico;
				delete dp;
				DriveInfoList->Delete(idx);
			}
			continue;
		}

		if (idx==-1) {
			dp = new drive_info;
			dp->small_ico = new TIcon();
			dp->large_ico = new TIcon();
			DriveInfoList->AddObject(dstr, (TObject*)dp);
			new_drive = dp;
		}
		else {
			dp = (drive_info *)DriveInfoList->Objects[idx];
			dp->small_ico->Handle = NULL;
			dp->large_ico->Handle = NULL;
		}

		dp->drive_str  = dstr;
		dp->accessible = is_drive_accessible(dstr);		//�A�N�Z�X�\
		dp->ejectable  = EjectDrive(dstr, false);		//���O���\
		dp->is_virtual = false;

		//���
		dp->drv_type = ::GetDriveType(dstr.c_str());
		switch (dp->drv_type) {
		case DRIVE_REMOVABLE:	dp->type_str = "�����[�o�u���E���f�B�A";	break;
		case DRIVE_FIXED:		dp->type_str = "�n�[�h�f�B�X�N";			break;
		case DRIVE_REMOTE:		dp->type_str = "�l�b�g���[�N�E�h���C�u";	break;
		case DRIVE_CDROM:		dp->type_str = "CD-ROM�h���C�u";			break;
		case DRIVE_RAMDISK:		dp->type_str = "RAM�f�B�X�N";  dp->is_RAM = true;	break;
		default: 				dp->type_str = EmptyStr;
		}

		//�{�����[�����A�t�@�C���V�X�e��
		if (dp->accessible)
			dp->volume = get_VolumeInfo(dstr, &dp->f_system);
		else
			dp->volume = dp->f_system = EmptyStr;

		dp->is_NTFS = USAME_TI(dp->f_system, "NTFS");

		dp->bus_type = EmptyStr;
		dp->is_SSD	 = dp->is_RAM = false;
		if (dp->drv_type!=DRIVE_REMOTE) {
			//�ڑ�I/F���擾
			UnicodeString drv;
			drv.sprintf(_T("\\\\.\\%s"), ExtractFileDrive(dstr).c_str());
			HANDLE hDrive = ::CreateFile(drv.c_str(), 0,
				FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
			if (hDrive!=INVALID_HANDLE_VALUE) {
				bool rq_chk_ram = false;
				DWORD dwLen = 4096;
				std::unique_ptr<BYTE[]> pcbData(new BYTE[dwLen]);
				::ZeroMemory(pcbData.get(), dwLen);

				STORAGE_PROPERTY_QUERY sQuery;
				sQuery.PropertyId = StorageDeviceProperty;
				sQuery.QueryType  = PropertyStandardQuery;
				sQuery.AdditionalParameters[0] = NULL;
				DWORD dwRet;
				if (::DeviceIoControl(hDrive, IOCTL_STORAGE_QUERY_PROPERTY,
						&sQuery, sizeof(sQuery), pcbData.get(), dwLen, &dwRet, NULL))
				{
					STORAGE_DEVICE_DESCRIPTOR *pDescriptor = (STORAGE_DEVICE_DESCRIPTOR*)pcbData.get();
					int ix = -1;
					switch(pDescriptor->BusType) {
					case BusTypeScsi:	ix = 0;		break;
					case BusTypeAtapi:	ix = 1;		break;
					case BusTypeAta:	ix = 2;		break;
					case BusType1394:	ix = 3;		break;
					case BusTypeSsa:	ix = 4;		break;
					case BusTypeFibre:	ix = 5;		break;
					case BusTypeUsb:	ix = 6;		break;
					case BusTypeRAID:	ix = 7;		break;
					case BusTypeiScsi:	ix = 8;		break;
					case BusTypeSas:	ix = 9;		break;
					case BusTypeSata:	ix = 10;	break;
					case BusTypeSd:		ix = 11;	break;
					case BusTypeMmc:	ix = 12;	break;
					case BusTypeSpaces:	ix = 13;	break;
					default:
						;
					}
					dp->bus_type = get_word_i_idx(_T("SCSI|ATAPI|ATA|IEEE1394|SSA|Fibre|USB|RAID|iSCSI|SAS|SATA|SD|MMC|SPACE"), ix);

					//SSD�̔��� (Windows 7 �ȍ~)
					if (dp->accessible && IsWindows7OrGreater()) {
						::ZeroMemory(pcbData.get(), dwLen);
						sQuery.PropertyId = StorageDeviceSeekPenaltyProperty;
						sQuery.QueryType  = PropertyStandardQuery;
						if (::DeviceIoControl(hDrive, IOCTL_STORAGE_QUERY_PROPERTY,
								&sQuery, sizeof(sQuery),
								pcbData.get(), sizeof(DEVICE_SEEK_PENALTY_DESCRIPTOR), &dwRet, NULL))
						{
							DEVICE_SEEK_PENALTY_DESCRIPTOR *pDescriptor = (DEVICE_SEEK_PENALTY_DESCRIPTOR*)pcbData.get();
							dp->is_SSD = !pDescriptor->IncursSeekPenalty && !USAME_TI(dp->bus_type, "SD");
							if (dp->is_SSD) dp->type_str = "�\���b�h�X�e�[�g";
						}
						else rq_chk_ram = true;
					}
				}
				else rq_chk_ram = true;

				//RAM�f�B�X�N�̐���
				if (rq_chk_ram && dp->accessible && dp->drv_type==DRIVE_FIXED && dp->bus_type.IsEmpty()) {
					MEMORYSTATUSEX stt_ex;
					stt_ex.dwLength = sizeof(stt_ex);
					if (::GlobalMemoryStatusEx(&stt_ex)) {
						__int64 sTotal = DiskSize((char)dstr[1] - 'A' + 1);
						//�T�C�Y�������������g�p�e�ʂ�菬����
						if (sTotal<=(__int64)(stt_ex.ullTotalPhys - stt_ex.ullAvailPhys)) {
							dp->is_RAM	 = true;
							dp->type_str = "RAM�f�B�X�N";
						}
					}
				}

				::CloseHandle(hDrive);
			}
		}

		//UNC(�����[�g)
		dp->unc = EmptyStr;
		if (dp->drv_type==DRIVE_REMOTE) {
			DWORD InfoSize = 1000;
			std::unique_ptr<BYTE[]> inf(new BYTE[InfoSize]);
			LPUNIVERSAL_NAME_INFO pInf = (LPUNIVERSAL_NAME_INFO)inf.get();
			if (WNetGetUniversalName(dstr.c_str(), UNIVERSAL_NAME_INFO_LEVEL, pInf, &InfoSize)==NO_ERROR)
				dp->unc = pInf->lpUniversalName;
		}

		//�A�C�R��
		UnicodeString inam = dp->accessible?
								def_if_empty(get_autorun_ico(dstr + "autorun.inf"), dstr) : dstr;
		SHFILEINFO sif;
		if (::SHGetFileInfo(inam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON))
			dp->small_ico->Handle = sif.hIcon;
		if (::SHGetFileInfo(inam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_LARGEICON))
			dp->large_ico->Handle = sif.hIcon;
	}

	DriveInfoList->Sort();

	//���z�h���C�u�̃`�F�b�N
	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (Execute_cmdln("cmd /c subst", ExePath, "HO", NULL, o_lst.get())) {
		for (int i=0; i<o_lst->Count; i++) {
			UnicodeString lbuf = o_lst->Strings[i];
			UnicodeString dstr = split_tkn(lbuf, ": => ");
			if (!dstr.IsEmpty()) {
				int idx = DriveInfoList->IndexOf(dstr);
				if (idx!=-1) {
					drive_info *dp = (drive_info *)DriveInfoList->Objects[idx];
					dp->is_virtual = true;
					dp->mnt_dir    = ExcludeTrailingPathDelimiter(lbuf);
				}
			}
		}
	}

	//�\���p���x��
	for (int i=0; i<DriveInfoList->Count; i++) {
		drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
		dp->label =
			dp->is_virtual? (is_root_dir(dp->mnt_dir)? dp->mnt_dir : "[" + ExtractFileName(dp->mnt_dir)) + "]" :
			(dp->drv_type==DRIVE_REMOTE)? yen_to_delimiter(dp->unc) : dp->volume;
	}

	return new_drive;
}
//---------------------------------------------------------------------------
//�w��h���C�u�̏����擾
//---------------------------------------------------------------------------
drive_info *get_DriveInfo(
	UnicodeString dstr)		//�擪���h���C�u�����̕�����
{
	drive_info *dp = NULL;
	if (!dstr.IsEmpty() && isalpha(dstr[1])) {
		UnicodeString drv_str;
		drv_str.sprintf(_T("%c:\\"), dstr[1]);
		int idx = DriveInfoList->IndexOf(drv_str.UpperCase());
		if (idx!=-1) dp = (drive_info *)DriveInfoList->Objects[idx];
	}
	return dp;
}
//---------------------------------------------------------------------------
//�{�����[�������擾
//�߂�l: �{�����[����
//---------------------------------------------------------------------------
UnicodeString get_VolumeInfo(
	UnicodeString dnam,		//�h���C�u��(�t�@�C��������)
	UnicodeString *fsys)	//[o] �t�@�C���V�X�e�� (default = NULL)
{
	UnicodeString ret_str;
	_TCHAR vol_nam[MAX_PATH];
	_TCHAR fil_sys[MAX_PATH];
	DWORD VolSerialNo, MaxCompLen, Flags;
	if (::GetVolumeInformation(get_drive_str(dnam).c_str(),
		vol_nam, sizeof(vol_nam), &VolSerialNo, &MaxCompLen, &Flags, fil_sys, sizeof(fil_sys)))
	{
		ret_str = vol_nam;
		if (fsys) *fsys = fil_sys;
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//VirDriveList �̍��ڂ����z�h���C�u�Ƃ��ă}�E���g
//�߂�l: ���O���b�Z�[�W
//---------------------------------------------------------------------------
UnicodeString mount_VirDriveList(int idx)
{
	UnicodeString res_msg;

	if (idx>=0 && idx<VirDriveList->Count) {
		UnicodeString drv  = VirDriveList->Names[idx];
		UnicodeString dnam = VirDriveList->ValueFromIndex[idx];
		if (StartsStr("\\\\", dnam)) dnam.Insert("?\\UNC\\", 3);

		drive_info *dp = get_DriveInfo(drv);
		if (!dp) {
			drv += ":";
			res_msg = make_LogHdr(_T("MOUNT")).cat_sprintf(_T("%s => %s"), drv.c_str(), dnam.c_str());
			if (!dir_exists(dnam) || !::DefineDosDevice(0, drv.c_str(), dnam.c_str())) res_msg[1] = 'E';
		}
	}

	return res_msg;
}
//---------------------------------------------------------------------------
//���z�h���C�u�Ƀ}�E���g����Ă���f�B���N�g��
//---------------------------------------------------------------------------
UnicodeString get_VirMountDir(UnicodeString drv)
{
	drive_info *dp = get_DriveInfo(drv);
	return (dp && dp->is_virtual)? dp->mnt_dir : EmptyStr;
}

//---------------------------------------------------------------------------
//���z�h���C�u���܂ރp�X�������̖��O�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString cv_VirToOrgName(UnicodeString fnam)
{
	UnicodeString vdir = get_VirMountDir(fnam);
	if (vdir.IsEmpty()) return fnam;
	return IncludeTrailingPathDelimiter(vdir) + get_tkn_r(fnam, ":\\");
}

//---------------------------------------------------------------------------
//���z�h���C�u���l���������s�p�X����
//---------------------------------------------------------------------------
bool is_ExePath(UnicodeString pnam)
{
	if (SameText(pnam, ExePath)) return true;
	return SameText(IncludeTrailingPathDelimiter(cv_VirToOrgName(pnam)), cv_VirToOrgName(ExePath));
}

//---------------------------------------------------------------------------
//�h���C�u�e�ʃ��O�̓��e���擾
//---------------------------------------------------------------------------
void get_DriveLogList(
	UnicodeString dstr,	//�Ώۃh���C�u
	TStringList *lst,
	double &r_min,		//�ŏ��g�p��
	double &r_max)		//�ő�g�p��
{
	update_DriveLog(DriveLogName.IsEmpty());

	//�w��h���C�u�̂ݒ��o
	lst->Clear();
	r_min = r_max = -1;
	if (DriveLogList->Count>0) {
		dstr = dstr.SubString(1, 1);
		for (int i=1; i<DriveLogList->Count; i++) {
			UnicodeString lbuf = DriveLogList->Strings[i];
			if (SameText(get_csv_item(lbuf, 1), dstr)) {
				lst->Add(lbuf);
				__int64 used_sz = StrToInt64Def(get_csv_item(lbuf, 2), -1);
				__int64 free_sz = StrToInt64Def(get_csv_item(lbuf, 3), -1);
				if (used_sz>=0 && free_sz>=0) {
					double r = 1.0 * used_sz / (used_sz + free_sz);;
					r_min = (r_min<0)? r : std::min(r_min, r);
					r_max = (r_max<0)? r : std::max(r_max, r);
				}
			}
		}
		lst->Sort();
	}
}

//---------------------------------------------------------------------------
//�h���C�u�e�ʃ��O�̍X�V
//---------------------------------------------------------------------------
void update_DriveLog(bool save)
{
	if (DriveLogList->Count==0) DriveLogList->Add("Date,Drive,Used,Free");

	for (int i=0; i<DriveInfoList->Count; i++) {
		drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
		if (!dp->accessible || dp->drv_type==DRIVE_CDROM) continue;

		__int64 used_sz = 0, free_sz = 0;
		ULARGE_INTEGER FreeAvailable, TotalBytes, FreeBytes;
		if (::GetDiskFreeSpaceEx(dp->drive_str.c_str(), &FreeAvailable, &TotalBytes, &FreeBytes)) {
			__int64 total_sz = TotalBytes.QuadPart;
			free_sz = FreeBytes.QuadPart;
			used_sz = total_sz - free_sz;
		}

		//�{���̃f�[�^��T��
		int idx = -1;
		UnicodeString dstr = dp->drive_str.SubString(1, 1);
		for (int j=DriveLogList->Count-1; j>0; j--) {
			UnicodeString ibuf = DriveLogList->Strings[j];
			TDateTime dt;
			if (str_to_DateTime(get_csv_item(ibuf, 0), &dt)) {
				TValueRelationship res = CompareDate(dt, Now());
				if (res==LessThanValue) continue;
				if (res==EqualsValue && SameText(get_csv_item(ibuf, 1), dstr)) {
					idx = j; break;
				}
			}
		}

		UnicodeString lbuf = FormatDateTime("yyyy/mm/dd", Now());
		lbuf.cat_sprintf(_T(",%s,%llu,%llu"), dp->drive_str.SubString(1, 1).c_str(), used_sz, free_sz);
		if (idx!=-1) 
			DriveLogList->Strings[idx] = lbuf;
		else
			DriveLogList->Add(lbuf);
	}

	if (save) {
		DriveLogName = ExePath + DRVLOG_FILE;
		if (!saveto_TextFile(DriveLogName, DriveLogList)) DriveLogName = EmptyStr;
	}
}

//---------------------------------------------------------------------------
//�ėp���X�g�̍��ڍ���ݒ� (�s�� = 1/3)
//---------------------------------------------------------------------------
void set_ListBoxItemHi(
	TListBox *lp,
	TFont *font,	//�t�H���g			(default = NULL);
	bool with_ico)	//�A�C�R����\��	(default = false)
{
	if (font) {
		lp->Font->Assign(font);
		lp->Canvas->Font->Assign(font);
	}

	lp->ItemHeight = std::max(get_FontHeight(lp->Font, abs(lp->Font->Height) / 3.0 + 1), 
								with_ico? (int)(20 * ScrScale) : 0);
}
//---------------------------------------------------------------------------
void set_ListBoxItemHi(
	TCheckListBox *lp,
	TFont *font,	//�t�H���g			(default = NULL);
	bool with_ico)	//�A�C�R����\��	(default = false)
{
	if (font) {
		lp->Font->Assign(font);
		lp->Canvas->Font->Assign(font);
	}

	lp->ItemHeight = std::max(get_FontHeight(lp->Font, abs(lp->Font->Height) / 3.0 + 1),
								with_ico? (int)(20 * ScrScale) : 0);
}

//---------------------------------------------------------------------------
//�W�����X�g�̐ݒ� (�s�� = ListInterLn)
//---------------------------------------------------------------------------
void set_StdListBox(
	TListBox *lp,
	int tag,		//	(default = 0)
	TFont *font,	//	(default = NULL)
	bool with_ico)	//�A�C�R����\��	(default = false)
{
	if (tag!=0) lp->Tag = tag;
	lp->Color = col_bgList;
	if (!font) font = ListFont;
	lp->Font->Assign(font);
	lp->Canvas->Font->Assign(font);
	lp->ItemHeight = std::max(get_FontHeight(lp->Font, ListInterLn), with_ico? (int)(20 * ScrScale) : 0);
}
//---------------------------------------------------------------------------
void set_StdListBox(
	TCheckListBox *lp,
	int tag,		//	(default = 0)
	TFont *font,	//	(default = NULL)
	bool with_ico)	//�A�C�R����\��	(default = false)
{
	if (tag!=0) lp->Tag = tag;
	lp->Color = col_bgList;
	if (!font) font = ListFont;
	lp->Font->Assign(font);
	lp->Canvas->Font->Assign(font);
	lp->ItemHeight = std::max(get_FontHeight(lp->Font, ListInterLn), with_ico? (int)(20 * ScrScale) : 0);
}

//---------------------------------------------------------------------------
//�c�[���o�[�̐ݒ�
//---------------------------------------------------------------------------
void setup_ToolBar(TToolBar *tb)
{
	tb->Font->Assign(ToolBarFont);
	tb->Font->Color 	   = col_fgTlBar;
	tb->GradientStartColor = col_bgTlBar1;
	tb->GradientEndColor   = col_bgTlBar2;
	tb->HotTrackColor	   = col_htTlBar;

	for (int i=0; i<tb->ControlCount; i++) {
		TControl *cp = tb->Controls[i];
		if (class_is_Edit(cp)) {
			((TEdit*)cp)->Font->Assign(DialogFont);
			((TEdit*)cp)->Text = EmptyStr;
		}
		else if (class_is_ComboBox(cp)) {
			((TComboBox*)cp)->Font->Assign(DialogFont);
		}
		else if (cp->ClassNameIs("TSplitter")) {
			((TSplitter*)cp)->Color = Mix2Colors(col_bgTlBar1, col_bgTlBar2);
		}
	}
}

//---------------------------------------------------------------------------
//�V���v���X�N���[���o�[�̐ݒ�
//---------------------------------------------------------------------------
void set_UsrScrPanel(UsrScrollPanel *sp)
{
	int std_wd = ::GetSystemMetrics(SM_CXVSCROLL);
	int knob_wd = std_wd;

	sp->KnobImgBuffV = NULL;
	sp->KnobImgBuffH = NULL;

	switch (ScrBarStyle) {
	case 2:	//3/4��
		knob_wd *= 0.75;	break;
	case 3:	//1/2��
		knob_wd /= 2;		break;
	case 4:	//�摜
		if (!BgImgBuff[BGIMGID_KNOB_V]->Empty) {
			knob_wd = std::min(BgImgBuff[BGIMGID_KNOB_V]->Width + 2, knob_wd);
			sp->KnobImgBuffV = BgImgBuff[BGIMGID_KNOB_V];
			if ((sp->Flag & USCRPNL_FLAG_HS) && !BgImgBuff[BGIMGID_KNOB_H]->Empty)
				sp->KnobImgBuffH = BgImgBuff[BGIMGID_KNOB_H];
		}
		break;
	}

	sp->KnobWidth = knob_wd;
	sp->UpdateKnob();

	sp->Color		 = col_bgScrBar;
	sp->KnobColor	 = col_bgScrKnob;
	sp->KnobBdrColor = col_frScrKnob;
	sp->KnobActColor = col_bgActKnob;
	sp->HitLineColor = col_lnScrHit;
	sp->SelLineColor = col_lnScrSel;

	bool is_simple = (ScrBarStyle>0);
	bool is_flist  = (sp->Flag & USCRPNL_FLAG_FL);
	sp->Visible    = is_flist? (!HideScrBar && is_simple) : is_simple;

	sp->ListCsrVisible = (sp->Flag & (USCRPNL_FLAG_TV|USCRPNL_FLAG_GL))? TvCursorVisible : FlCursorVisible;

	if (sp->ParentPanel) {
		//���X�g�{�b�N�X�̃T�C�Y����
		if (sp->AssoListBox) {
			//��
			int wd = sp->ParentPanel->ClientWidth;
			if ((is_flist && HideScrBar) || is_simple) wd += (std_wd + 2);
			sp->AssoListBox->Width = wd;
			//����
			if (sp->Flag & USCRPNL_FLAG_HS) {
				int hi = sp->ParentPanel->ClientHeight;
				if (is_simple) hi += (std_wd + 2);
				sp->AssoListBox->Height = hi;
			}
		}
		//�`�F�b�N���X�g�{�b�N�X�̃T�C�Y����
		else if (sp->AssoChkListBox) {
			//��
			int wd = sp->ParentPanel->ClientWidth;
			if (is_simple) wd += (std_wd + 2);
			sp->AssoChkListBox->Width  = wd;
		}
		//�O���b�h�̃T�C�Y����
		else if (sp->AssoStrGrid) {
			//��
			int wd = sp->ParentPanel->ClientWidth;
			if (is_simple) wd += (std_wd + 2);
			sp->AssoStrGrid->Width = wd;
			//����
			if (sp->Flag & USCRPNL_FLAG_HS) {
				int hi = sp->ParentPanel->ClientHeight;
				if (is_simple) hi += (std_wd + 2);
				sp->AssoStrGrid->Height = hi;
			}
		}
	}
}

//---------------------------------------------------------------------------
//�g���q�̕��E�ʒu�𒲐� (�������Ɏg�p)
//---------------------------------------------------------------------------
void set_FextWidth(file_rec *fp, int tag)
{
	TCanvas *cv = FileListBox[tag]->Canvas;
	flist_stt *lst_stt = &ListStt[tag];

	int x_base = ScaledInt((IconMode==1 || (IconMode==2 && fp->is_dir))? 20 : 8);
	int w_mx_f = std::min(lst_stt->lxp_size - x_base - lst_stt->lwd_half * 9, lst_stt->lwd_fext_max);

	lst_stt->lwd_fext = std::max(lst_stt->lwd_fext, get_TextWidth(cv, fp->f_ext, IsIrregularFont(cv->Font)));
	lst_stt->lwd_fext = std::min(lst_stt->lwd_fext, w_mx_f);
	lst_stt->lxp_fext = lst_stt->lxp_size - lst_stt->lwd_fext - lst_stt->lwd_half;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�����X�g�{�b�N�X�ɐݒ�
// (���z���X�g�{�b�N�X�Ή�)
//---------------------------------------------------------------------------
void assign_FileListBox(
	TListBox *lp,
	TStringList *lst,
	int idx,				//�C���f�b�N�X (default = -1)
	UsrScrollPanel *sp)		//�V���v���X�N���[���o�[ (default = NULL)
{
	if (lp->Style==lbVirtualOwnerDraw)
		lp->Count = lst->Count;
	else
		lp->Items->Assign(lst);

	if (idx!=-1) ListBoxSetIndex(lp, idx);

	if (sp) sp->UpdateKnob();
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�{�b�N�X�X�V
//---------------------------------------------------------------------------
void update_FileListBox(
	TStrings *lst, int tag,
	int idx)	//�C���f�b�N�X (default = -1 : ItemIndex)
{
	TListBox *lp = FileListBox[tag];
	if (idx==-1) idx = lp->ItemIndex;
	lp->Count = lst->Count;
	lp->ItemIndex = std::min(idx, lp->Count - 1);
	lp->Perform(WM_NYANFI_UPDKNOB, 0, (NativeInt)0);
}
//---------------------------------------------------------------------------
//�ŏ�250ms�Ԋu�Ńt�@�C�����X�g�{�b�N�X�X�V
//---------------------------------------------------------------------------
void update_FileListBoxT(TStrings *lst, int tag)
{
	static unsigned int start_cnt = 0;

	if ((GetTickCount() - start_cnt)>250) {
		start_cnt = GetTickCount();
		update_FileListBox(lst, tag, -1);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̑S�t�@�C�����擾
//  Objects �� �T�C�Y����t��
//  ESC�L�[�Œ��f�\
//---------------------------------------------------------------------------
bool get_Files_objSize(
	UnicodeString pnam,		//�f�B���N�g����
	TStrings *lst,			//���ʊi�[���X�g
	bool exc_sym)			//�W�����N�V����/�V���{���b�N�����N�����O
{
	if (!dir_exists(pnam)) return true;
	if (is_KeyPress_ESC()) return false;

	pnam = IncludeTrailingPathDelimiter(pnam);

	//�T�u�f�B���N�g��������
	TSearchRec sr;
	bool ok = true;
	if (FindFirst(cv_ex_filename(pnam + "*"), faAnyFile, sr)==0) {
		do {
			if ((sr.Attr & faDirectory)==0) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			if (exc_sym        && (sr.Attr & faSymLink)) continue;
			if (ContainsStr("..", sr.Name)) continue;
			ok = get_Files_objSize(pnam + sr.Name, lst, exc_sym);
		} while(ok && FindNext(sr)==0);
		FindClose(sr);
	}

	//�t�@�C��������
	if (ok && FindFirst(cv_ex_filename(pnam + "*.*"), faAnyFile, sr)==0) {
		do {
			if (sr.Attr & faDirectory) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			if (exc_sym        && (sr.Attr & faSymLink)) continue;
			__int64 *sz = new __int64;
			*sz = sr.Size;
			if (lst->AddObject(pnam + sr.Name, (TObject*)sz)%ESC_INTERVAL==0) {
				ok = !is_KeyPress_ESC();
			}
		} while(ok && FindNext(sr)==0);
		FindClose(sr);
	}

	return ok;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g����t�@�C�������X�g���擾
//---------------------------------------------------------------------------
bool get_NameList_objSize(
	TStringList *lst,
	TStringList *o_lst,
	bool sub_sw,			//�T�u�f�B���N�g��������
	bool exc_sym)			//�W�����N�V����/�V���{���b�N�����N�����O
{
	for (int i=0; i<lst->Count; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_up || fp->is_dummy || fp->f_attr==faInvalid) continue;
		if (exc_sym && fp->is_sym) continue;

		if (fp->is_dir) {
			if (sub_sw && !get_Files_objSize(fp->f_name, o_lst, exc_sym)) return false;
		}
		else {
			__int64 *sz = new __int64;
			*sz = fp->f_size;
			o_lst->AddObject(fp->f_name, (TObject*)sz);
			if (i%ESC_INTERVAL==0 && is_KeyPress_ESC()) return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//�t�@�C���������X�g�̎擾
//---------------------------------------------------------------------------
void get_FindListF(UnicodeString pnam, flist_stt *lst_stt, TStrings *lst, int tag)
{
	if (FindAborted) return;
	if (pnam.IsEmpty()) return;
	if (!dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);
	FindPath  = pnam;
	FindCount = (lst->Count>1)? lst->Count - 1 : 0;

	if (lst_stt->find_Mask.IsEmpty()) lst_stt->find_Mask = "*.*";

	//�T�u�f�B���N�g��������
	UnicodeString sea_str;
	TSearchRec sr;
	if (lst_stt->find_SubDir) {
		sea_str = pnam + "*";
		if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);
		if (FindFirst(sea_str, faAnyFile, sr)==0) {
			do {
				if ((sr.Attr & faDirectory)==0) continue;
				if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
				if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
				if (ContainsStr("..", sr.Name)) continue;
				get_FindListF(pnam + sr.Name, lst_stt, lst, tag);
				Application->ProcessMessages();
			} while(FindNext(sr)==0 && !FindAborted);
			FindClose(sr);
		}
	}
	if (FindAborted) return;

	//�t�@�C��������
	sea_str = lst_stt->find_Arc? (pnam + "*") : (pnam + lst_stt->find_Mask);

	if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);

	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			Application->ProcessMessages();

			bool is_dir = (sr.Attr & faDirectory);
			if (!lst_stt->find_Both && is_dir) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			UnicodeString fnam = sr.Name;  if (ContainsStr("..", fnam)) continue;
			UnicodeString fext = !is_dir? get_extension(fnam) : EmptyStr;

			//�A�[�J�C�u������
			if (lst_stt->find_Arc && test_ArcExt(fext)) {
				UnicodeString anam = pnam + fnam;
				FindPath = anam;
				Application->ProcessMessages();

				if (usr_ARC->OpenArc(anam)) {
					int add_cnt = 0;
					arc_find_inf inf;
					UnicodeString arc_mask = USAME_TS(lst_stt->find_Mask, "*.*")? UnicodeString("*") : lst_stt->find_Mask;
					if (usr_ARC->FindFirstEx(EmptyStr, &inf, true)) {
						do {
							if (!lst_stt->find_Both && inf.is_dir) continue;
							if (!ShowHideAtr   && (inf.f_attr & faHidden))  continue;
							if (!ShowSystemAtr && (inf.f_attr & faSysFile)) continue;

							if (inf.is_dir) inf.f_name = ExcludeTrailingPathDelimiter(inf.f_name);
							if (!str_match(arc_mask, ExtractFileName(inf.f_name))) continue;
							if (!check_file_std(ExtractFileName(inf.f_name), inf.f_time, inf.f_size, inf.f_attr, lst_stt)) continue;

							//��������
							file_rec *fp   = cre_new_file_rec();
							fp->is_virtual = true;
							fp->p_name.sprintf(_T("%s/%s"), anam.c_str(), ExtractFilePath(inf.f_name).c_str());
							fp->f_name.sprintf(_T("%s/%s"), anam.c_str(), inf.f_name.c_str());
							fp->n_name	   = ExtractFileName(inf.f_name);
							fp->arc_name   = anam;
							fp->r_name	   = fp->f_name;
							fp->tag 	   = tag;
							fp->is_dir	   = inf.is_dir;
							fp->f_time	   = inf.f_time;
							fp->f_attr	   = inf.f_attr;
							fp->attr_str   = get_file_attr_str(fp->f_attr);

							if (fp->is_dir) {
								fp->b_name = ExtractFileName(inf.f_name);
							}
							else {
								fp->b_name = get_base_name(inf.f_name);
								fp->f_ext  = get_extension(inf.f_name);
								fp->f_size = inf.f_size;
								//�g���q�̕��E�ʒu����
								set_FextWidth(fp, tag);
							}

							lst->AddObject(fp->f_name, (TObject*)fp);
							FindCount = (lst->Count>1)? lst->Count - 1 : 0;
							add_cnt++;
						} while (usr_ARC->FindNextEx(&inf, true));
					}
					usr_ARC->CloseArc();

					//���X�g�{�b�N�X�X�V(���z)
					if (add_cnt>0) update_FileListBoxT(lst, tag);
				}
			}
			else {
				FindPath  = pnam;
			}

			//��FindFirst �ł̌�}�b�`�����O
			//  ��: �V�X�e������ *.asp �� *.aspx �Ƀ}�b�` (��)
			if ((!is_dir || !USAME_TS(lst_stt->find_Mask, "*.*")) && !str_match(lst_stt->find_Mask, fnam)) continue;

			//�W������
			TDateTime f_tm = sr.TimeStamp;
			__int64   f_sz = sr.Size;
			bool is_jct;
			UnicodeString lnk_name = (sr.Attr & faSymLink)? get_ReparsePointTarget(pnam + fnam, is_jct) : EmptyStr;
			if (ShowTargetInf && !lnk_name.IsEmpty()) {
				f_tm = get_file_age(lnk_name);
				if (!is_dir) f_sz = get_file_size(lnk_name);
			}
			if (!check_file_std(fnam, f_tm, f_sz, sr.Attr, lst_stt)) continue;

			//�g���t�@�C������
			if (!is_dir && !check_file_ex(pnam + fnam, lst_stt)) continue;

			//��������
			file_rec *fp = cre_new_file_rec();
			fp->tag		 = tag;
			fp->is_dir	 = is_dir;
			fp->is_sym	 = (sr.Attr & faSymLink);
			fp->is_jct	 = is_jct;
			fp->p_name	 = pnam;
			fp->f_name	 = fp->r_name = pnam + fnam;
			fp->n_name	 = fnam;
			fp->l_name	 = lnk_name;
			fp->f_time	 = f_tm;
			fp->f_attr	 = sr.Attr;
			fp->attr_str = get_file_attr_str(fp->f_attr);
			if (fp->is_dir) {
				fp->b_name = fp->n_name;
			}
			else {
				fp->b_name = get_base_name(fp->f_name);
				fp->f_ext  = fext;
				fp->f_size = f_sz;
				//�g���q�̕��E�ʒu����
				set_FextWidth(fp, tag);
			}

			lst->AddObject(fp->f_name, (TObject*)fp);
			FindCount = (lst->Count>1)? lst->Count - 1 : 0;
			update_FileListBoxT(lst, tag);
		} while(FindNext(sr)==0 && !FindAborted);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//�f�B���N�g���������X�g�̎擾
//---------------------------------------------------------------------------
void get_FindListD(UnicodeString pnam, flist_stt *lst_stt, TStrings *lst, int tag)
{
	if (FindAborted) return;
	if (pnam.IsEmpty() || !dir_exists(pnam)) return;

	pnam = IncludeTrailingPathDelimiter(pnam);
	FindPath  = pnam;
	FindCount = (lst->Count>1)? lst->Count - 1 : 0;
	if (lst_stt->find_Mask.IsEmpty()) lst_stt->find_Mask = "*";

	UnicodeString sea_str = pnam + "*";
	if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);

	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			if ((sr.Attr & faDirectory)==0) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			UnicodeString dnam = sr.Name;
			if (ContainsStr("..", dnam)) continue;

			if (str_match(lst_stt->find_Mask, dnam)) {
				bool match = true;
				//�^�C���X�^���v
				TDateTime f_tm = sr.TimeStamp;
				bool is_jct;
				UnicodeString lnk_name = (sr.Attr & faSymLink)? get_ReparsePointTarget(pnam + dnam, is_jct) : EmptyStr;
				if (ShowTargetInf && !lnk_name.IsEmpty()) f_tm = get_file_age(lnk_name);

				if (lst_stt->find_DT_mode>0) {
					if (!lst_stt->find_DT_str.IsEmpty() && lst_stt->find_DT_mode==1) {
					    match = str_match(lst_stt->find_DT_str, format_Date(f_tm));
					}
					else {
						TValueRelationship res = CompareDate(f_tm, lst_stt->find_DT_value);
						switch (lst_stt->find_DT_mode) {
						case 1: match = (res==EqualsValue); break;
						case 2: match = (res==EqualsValue || res==LessThanValue);    break;
						case 3: match = (res==EqualsValue || res==GreaterThanValue); break;
						}
					}
				}
				//���e
				if (match && lst_stt->find_CT_mode>0) {
					bool is_empty = is_EmptyDir(pnam + dnam);
					switch (lst_stt->find_CT_mode) {
					case 1: match = is_empty;  break;
					case 2: match = !is_empty; break;
					}
				}
				//����
				if (match && lst_stt->find_AT_mode>0) {
					switch (lst_stt->find_AT_mode) {
					case 1: match = (sr.Attr & lst_stt->find_AT_value); break;
					case 2: match = (sr.Attr & lst_stt->find_AT_value); break;
					}
				}

				//�f�B���N�g�����x��
				if (match && lst_stt->find_Warn) {
					match = (get_filename_warn(pnam + dnam, NULL, true)>0);
				}

				//��������
				if (match) {
					file_rec *fp = cre_new_file_rec();
					fp->tag 	 = tag;
					fp->is_dir	 = true;
					fp->is_sym	 = (sr.Attr & faSymLink);
					fp->p_name	 = pnam;
					fp->f_name	 = fp->r_name = pnam + dnam;
					fp->n_name	 = dnam;
					fp->b_name	 = dnam;
					fp->l_name	 = lnk_name;
					fp->f_time	 = f_tm;
					fp->f_attr	 = sr.Attr;
					fp->attr_str = get_file_attr_str(fp->f_attr);
					lst->AddObject(fp->f_name, (TObject*)fp);
					FindCount = (lst->Count>1)? lst->Count - 1 : 0;
					//���X�g�{�b�N�X�X�V(���z)
					update_FileListBoxT(lst, tag);
				}
			}

			get_FindListD(pnam + dnam, lst_stt, lst, tag);
			Application->ProcessMessages();
		} while(FindNext(sr)==0 && !FindAborted);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//���ׂẴT�u�f�B���N�g�����擾
//---------------------------------------------------------------------------
void get_SubDirs(
	UnicodeString pnam,		//��f�B���N�g��
	TStrings *lst,			//[o] �f�B���N�g�������X�g
	TStatusBar *stt_bar,	//�o�ߕ\���p�X�e�[�^�X�o�[	(default = NULL)
	int  stt_idx,			//�\���p�l���̃C���f�b�N�X	(default = 0)
	bool proc_msg)			//���b�Z�[�W�L���[������	(default = false)
{
	if (!dir_exists(pnam)) return;
	pnam = IncludeTrailingPathDelimiter(pnam);
	if (stt_bar && stt_idx>=0 && stt_idx<stt_bar->Panels->Count) {
		stt_bar->Panels->Items[stt_idx]->Text = pnam;
		stt_bar->Repaint();
	}

	//�T�u�f�B���N�g��������
	UnicodeString sea_str;
	TSearchRec sr;
	sea_str = cv_ex_filename(pnam + "*");
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			if (proc_msg) Application->ProcessMessages();
			if ((sr.Attr & faDirectory)==0) continue;
			if (!ShowHideAtr   && (sr.Attr & faHidden))  continue;
			if (!ShowSystemAtr && (sr.Attr & faSysFile)) continue;
			if (ContainsStr("..", sr.Name)) continue;
			get_SubDirs(pnam + sr.Name, lst, stt_bar, stt_idx, proc_msg);
			lst->Add(pnam + sr.Name);
		} while(FindNext(sr)==0);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
//�w��t�@�C���ƃn�[�h�����N����Ă���t�@�C���̃��X�g���擾
//---------------------------------------------------------------------------
int get_HardLinkList(UnicodeString fnam, TStringList *o_lst)
{
	UnicodeString drvstr = ExtractFileDrive(fnam);
	UnicodeString prm;
	prm.sprintf(_T("hardlink list \"%s\""), fnam.c_str());
	DWORD exit_code;
	if (Execute_ex("fsutil", prm, ExtractFileDir(fnam), "OH", &exit_code, o_lst) && exit_code==0) {
		for (int i=0; i<o_lst->Count; i++) o_lst->Strings[i] = drvstr + o_lst->Strings[i];
	}
	return o_lst->Count;
}

//---------------------------------------------------------------------------
//�f�B���N�g���e�ʂ̌v�Z
//  �߂�l: �f�B���N�g���e��
//---------------------------------------------------------------------------
__int64 get_DirSize(
	UnicodeString dnam,
	int *f_cnt,			//�t�@�C�����ւ̃|�C���^
	int *d_cnt,			//�f�B���N�g�����ւ̃|�C���^
	__int64 *o_size,	//��L�T�C�Y�ւ̃|�C���^
	__int64 *c_size)	//���k�T�C�Y�ւ̃|�C���^
{
	if (CalcAborted) return -1;

	__int64 d_size = 0;
	int     clu_sz = get_ClusterSize(dnam);	//�N���X�^�T�C�Y
	__int64 clu_n  = 0;						//�N���X�^��

	dnam = IncludeTrailingPathDelimiter(dnam);
	UnicodeString sea_str = dnam + "*";
	if (sea_str.Length()>=MAX_PATH) sea_str.Insert("\\\\?\\", 1);

	TSearchRec sr;
	if (FindFirst(sea_str, faAnyFile, sr)==0) {
		do {
			//�T�u�f�B���N�g��������
			if (sr.Attr & faDirectory) {
				if (!ContainsStr("..", sr.Name)) {
					d_size += get_DirSize(dnam + sr.Name, f_cnt, d_cnt, o_size, c_size);
					++(*d_cnt);
				}
			}
			//�t�@�C��
			else {
				//�T�C�Y
				d_size += sr.Size;
				//���k
				if (c_size) {
					if (sr.Attr & faCompressed) {
						__int64 csz = get_comp_size(dnam + sr.Name);
						*c_size += (csz>=0)? csz : sr.Size;
					}
					else {
						*c_size += sr.Size;
					}
				}

				if (clu_sz>0) {
					clu_n += (sr.Size/clu_sz);
					if (sr.Size%clu_sz>0) clu_n++;
				}
				++(*f_cnt);
			}
			Application->ProcessMessages();
		} while (FindNext(sr)==0 && !CalcAborted);
		FindClose(sr);
	}

	if (CalcAborted) return -1;

	if (clu_sz>0) *o_size += (clu_sz * clu_n);
	return d_size;
}
//---------------------------------------------------------------------------
//���z�f�B���N�g���e�ʂ̌v�Z
//  �߂�l: �f�B���N�g���e��
//---------------------------------------------------------------------------
__int64 get_ArcDirSize(
	UnicodeString anam,		//�A�[�J�C�u�̃t�@�C����
	UnicodeString dnam,		//�Ώۃf�B���N�g����
	int *f_cnt,				//�t�@�C�����ւ̃|�C���^
	int *d_cnt)				//�f�B���N�g�����ւ̃|�C���^
{
	if (CalcAborted) return -1;

	__int64 d_size = 0;

	if (usr_ARC->OpenArc(anam)) {
		dnam = IncludeTrailingPathDelimiter(dnam);
		arc_find_inf inf;
		if (usr_ARC->FindFirstEx(EmptyStr, &inf, true)) {
			std::unique_ptr<TStringList> d_lst(new TStringList());
			do {
				UnicodeString fnam = inf.f_name;
				if (!StartsText(dnam, fnam)) continue;
				fnam.Delete(1, dnam.Length());
				if (!fnam.IsEmpty()) {
					if (!inf.is_dir) {
						d_size += inf.f_size;
						++(*f_cnt);
						UnicodeString snam = ExtractFileDir(fnam);
						if (!snam.IsEmpty() && d_lst->IndexOf(snam)==-1) d_lst->Add(snam);
					}
					//�f�B���N�g��
					else {
						if (d_lst->IndexOf(fnam)==-1) d_lst->Add(fnam);
					}
				}
				Application->ProcessMessages();
			} while (usr_ARC->FindNextEx(&inf, true) && !CalcAborted);
			(*d_cnt) += d_lst->Count;
		}
		usr_ARC->CloseArc();
	}

	if (CalcAborted) return -1;
	return d_size;
}

//---------------------------------------------------------------------------
//�L���b�V������w��t�@�C���̃A�C�R�����폜
//---------------------------------------------------------------------------
void del_CachedIcon(UnicodeString fnam)
{
	if (!test_FileExt(get_extension(fnam), FEXT_INDIVICO)) return;

	IconRWLock->BeginWrite();
		int idx = CachedIcoList->IndexOf(fnam);
		if (idx!=-1) {
			delete (TIcon*)CachedIcoList->Objects[idx];
			CachedIcoList->Delete(idx);
		}
	IconRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//�t�@�C���̃X���[���A�C�R�����擾
//---------------------------------------------------------------------------
HICON get_file_icon(
	UnicodeString fnam)		//�t�@�C����[,�C���f�b�N�X]
{
	HICON hIcon = NULL;
	int ico_idx = get_tkn_r(fnam, ",").ToIntDef(-1);

	if (ico_idx!=-1) {
		fnam = get_tkn(fnam, ",");
		HICON icons[1];
		if (::ExtractIconEx(fnam.c_str(), ico_idx, NULL, icons, 1)==1) {
			hIcon = icons[0];
		}
	}
	else {
		SHFILEINFO sif;
		if (::SHGetFileInfo(fnam.c_str(), 0, &sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_SMALLICON))
			hIcon = sif.hIcon;
		else
			hIcon = get_fext_icon(get_extension(fnam));
	}

	return hIcon;
}

//---------------------------------------------------------------------------
//�g���q�ˑ��X���[���A�C�R�����擾 (�L���b�V���𗘗p)
//---------------------------------------------------------------------------
HICON get_fext_icon(
	UnicodeString fext)		//�g���q .xxx	(default = EmptyStr: �t�H���_)
{
	HICON hIcon = NULL;

	if (fext.IsEmpty()) fext = "$DIR$";
	int idx = GeneralIconList->IndexOf(fext);

	if (idx!=-1) {
		hIcon = ((TIcon*)GeneralIconList->Objects[idx])->Handle;
	}
	else {
		SHFILEINFO sif;
		if (::SHGetFileInfo(fext.c_str(), (StartsStr('.', fext)? faArchive : faDirectory),
			&sif, sizeof(SHFILEINFO), SHGFI_ICON|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES))
		{
			hIcon = sif.hIcon;
			TIcon *icon  = new TIcon();
			icon->Handle = hIcon;
			GeneralIconList->AddObject(fext, (TObject*)icon);
		}
	}

	return hIcon;
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R�����擾
//---------------------------------------------------------------------------
HICON get_folder_icon(UnicodeString dnam)
{
	HICON hIcon = NULL;

	UnicodeString fnam = to_absolute_name(get_actual_name(DefFldIcoName));

	FldIcoRWLock->BeginWrite();
	int idx = FolderIconList->IndexOfName(ExcludeTrailingPathDelimiter(dnam));
	if (idx!=-1) fnam = to_absolute_name(get_actual_name(FolderIconList->ValueFromIndex[idx]));
	FldIcoRWLock->EndWrite();

	if (file_exists_ico(fnam)) {
		IconRWLock->BeginWrite();
		{
			int idx = CachedIcoList->IndexOf(fnam);
			if (idx!=-1) {
				TIcon *icon = (TIcon*)CachedIcoList->Objects[idx];
				if (icon) hIcon = icon->Handle;
			}
			else {
				CachedIcoList->Add(fnam);	//�X���b�h�Ɏ擾��v��
			}
		}
		IconRWLock->EndWrite();
	}
	else {
		set_FolderIcon(dnam);	//����
	}

	if (!hIcon) hIcon = get_fext_icon();

	return hIcon;
}

//---------------------------------------------------------------------------
//�X���[���A�C�R����`�� (�X���b�h�Ŏ擾�ACachedIcoList �g�p)
//  �W�����N�V����/�V���{���b�N�����N�ɂ��Ή�
//---------------------------------------------------------------------------
bool draw_SmallIcon(
	file_rec *fp,
	TCanvas *cv, int x, int y,
	bool force_cache)	//�����I�ɃL���V��	(default = false)
{
	if (!is_selectable(fp)) return false;

	HICON hIcon  = NULL;
	bool handled = false;

	//�f�B���N�g��
	if (fp->is_dir) {
		hIcon = get_folder_icon(fp->f_name);
	}
	//�t�@�C��
	else {
		UnicodeString fext = LowerCase(fp->f_ext);
		//���t�@�C���ˑ�
		if ((UseIndIcon || force_cache) && (test_FileExt(fext, FEXT_INDIVICO) || fp->is_sym)) {
			UnicodeString fnam = (fp->is_virtual || fp->is_ftp)? fp->tmp_name : fp->f_name;
			if (!fnam.IsEmpty()) {
				if (fp->is_dir) fnam = IncludeTrailingPathDelimiter(fnam);

				IconRWLock->BeginWrite();
				{
					int idx = CachedIcoList->IndexOf(fnam);
					if (idx!=-1) {
						if (CachedIcoList->Objects[idx]) {
							hIcon = ((TIcon*)CachedIcoList->Objects[idx])->Handle;
							::DrawIconEx(cv->Handle, x, y, hIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);
							handled = true;
						}
					}
					else {
						CachedIcoList->Add(fnam);	//�X���b�h�Ɏ擾��v��
					}
				}
				IconRWLock->EndWrite();
			}
			else {
				hIcon = get_fext_icon(fext);
			}
		}
		//�g���q�ˑ�
		else {
			if (fext.IsEmpty()) fext = ".nyanfi";
			hIcon = get_fext_icon(fext);
		}
	}

	if (handled) return true;
	if (!hIcon)  return false;

	//�`��
	::DrawIconEx(cv->Handle, x, y, hIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);

	//�f�B���N�g���ɖ��}�[�N���I�[�o�[���C�\��
	if (fp->is_dir && fp->is_sym && hLinkIcon) {
		::DrawIconEx(cv->Handle, x, y, hLinkIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);
	}

	return true;
}
//---------------------------------------------------------------------------
//�X���[���A�C�R����`�� (�X���b�h�Ŏ擾�ACachedIcoList �g�p)
//  ���t�@�C���ˑ���p
//---------------------------------------------------------------------------
bool draw_SmallIconF(
	UnicodeString fnam,
	TCanvas *cv, int x, int y)
{
	if (fnam.IsEmpty() || (starts_Dollar(fnam) && !contains_PathDlmtr(fnam))) return false;

	HICON hIcon  = NULL;
	bool handled = false;
	IconRWLock->BeginWrite();
	{
		int idx = CachedIcoList->IndexOf(fnam);
		if (idx!=-1) {
			if (CachedIcoList->Objects[idx]) {
				hIcon = ((TIcon*)CachedIcoList->Objects[idx])->Handle;
				::DrawIconEx(cv->Handle, x, y, hIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);
				handled = true;
			}
		}
		else {
			CachedIcoList->Add(fnam);	//�X���b�h�Ɏ擾��v��
		}
	}
	IconRWLock->EndWrite();

	if (handled) return true;
	if (!hIcon)  return false;

	::DrawIconEx(cv->Handle, x, y, hIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);
	return true;
}

//---------------------------------------------------------------------------
//�w��t�@�C���̃X���[���A�C�R����`��
//  MenuBtnIcoList ���g�p
//---------------------------------------------------------------------------
bool draw_SmallIcon2(
	UnicodeString fnam,
	TCanvas *cv, int x, int y)
{
	HICON hIcon;

	//�C���f�b�N�X�w��
	int idx = get_tkn_r(fnam, ",").ToIntDef(-1);
	if (idx!=-1) {
		hIcon = usr_SH->get_SmallIcon(fnam);
	}
	else {
		UnicodeString fext = LowerCase(get_extension(fnam));
		if (fnam.Pos('*')==1) fnam = EmptyStr;

		//�f�B���N�g��
		if (ends_PathDlmtr(fnam)) {
			hIcon = get_folder_icon(fnam);
		}
		//���t�@�C���ˑ�
		else if (test_FileExt(fext, FEXT_INDIVICO)) {
			if (!fnam.IsEmpty()) {
				int idx = MenuBtnIcoList->IndexOf(fnam);
				if (idx!=-1) {
					hIcon = (MenuBtnIcoList->Objects[idx])?
						((TIcon*)MenuBtnIcoList->Objects[idx])->Handle : usr_SH->get_SmallIcon(fnam);
				}
				else {
					hIcon = usr_SH->get_SmallIcon(fnam);
					if (hIcon) {
						TIcon *icon  = new TIcon();
						icon->Handle = hIcon;
						MenuBtnIcoList->AddObject(fnam, (TObject*)icon);
					}
				}
			}
			else {
				hIcon = get_fext_icon(fext);
			}
		}
		//�g���q�ˑ�
		else {
			hIcon = get_fext_icon(fext);
		}
	}

	if (!hIcon)  return false;

	//�`��
	::DrawIconEx(cv->Handle, x, y, hIcon, SIcoSize, SIcoSize, 0, NULL, DI_NORMAL);
	return true;
}

//---------------------------------------------------------------------------
//�\���p�X���[���A�C�R�����擾���ăC���[�W���X�g�ɒǉ�
//  UsrIcoList �ɓo�^���čė��p
//---------------------------------------------------------------------------
int add_IconImage(
	UnicodeString fnam,		//�t�@�C����[,�C���f�b�N�X]  (���p�X�ɑΉ�)
	TImageList *lst)
{
	if (fnam.IsEmpty()) return -1;

	TIcon *icon = NULL;
	fnam = to_absolute_name(get_actual_name(fnam));
	if (file_exists_ico(fnam)) {
		int usr_idx = UsrIcoList->IndexOf(fnam);
		if (usr_idx==-1) {
			HICON hIcon = get_file_icon(fnam);
			if (hIcon) {
				icon = new TIcon();
				icon->Handle = hIcon;
				UsrIcoList->AddObject(fnam, (TObject*)icon);
			}
		}
		else {
			icon = (TIcon*)UsrIcoList->Objects[usr_idx];
		}
	}

	return (icon? lst->AddIcon(icon) : -1);
}

//---------------------------------------------------------------------------
//�R�}���h����t�@�C�������擾
//  �t�@�C�����Ƀp�X���Ȃ��ꍇ�A���p�X���猟��
//---------------------------------------------------------------------------
UnicodeString get_file_from_cmd(UnicodeString s)
{
	//�擪�R�}���h���擾
	s = ReplaceStr(s, ":\\", "\f");

	UnicodeString cmd;
	int p = 1;
	bool in_qut = false;
	while (p<=s.Length()) {
		WideChar c = s[p];
		//���p���O
		if (!in_qut) {
			if (c==':') break;
			if (c=='\"') in_qut = true;
			cmd.cat_sprintf(_T("%c"), c);
		}
		//���p����
		else {
			if (c=='\"') in_qut = false;
			cmd.cat_sprintf(_T("%c"), c);
		}
		p++;
	}

	cmd = Trim(ReplaceStr(cmd, "\f", ":\\"));

	//�t�@�C�������擾
	if (USAME_TI(cmd, "PowerShell")) cmd = "FileRun_powershell";
	UnicodeString fnam;
	if (remove_top_text(cmd, _T("OpenByWin_")) || remove_top_text(cmd, _T("SetExeFile_"))
		 || remove_top_text(cmd, _T("FileRun_")) || remove_top_text(cmd, _T("FileEdit_")))
	{
		fnam = get_actual_name(exclude_quot(cmd));
	}
	else if (remove_top_text(cmd, _T("SetFolderIcon_"))) {
		fnam = to_absolute_name(get_actual_name(exclude_quot(cmd)));
	}
	else if (USAME_TI(cmd, "CommandPrompt")) fnam = "%ComSpec%";
	else if (USAME_TI(cmd, "OpenByExp"))	 fnam = "%windir%\\explorer.exe";
	else fnam = cmd;

	return fnam;
}

//---------------------------------------------------------------------------
//���s�t�@�C�������擾���ăR���g���[���ɐݒ� (.lnk ����̒��o�ɑΉ�)
//---------------------------------------------------------------------------
void SetExtNameToCtrl(UnicodeString fnam, TWinControl *cp, 
	bool nbt_sw)	//.nbt �ɑΉ� (default = false)
{
	UnicodeString fext = get_extension(fnam);
	if (test_LnkExt(fext)) {
		UnicodeString xnam, prm, fld;
		int shw;
		usr_SH->get_LnkInf(fnam, NULL, &xnam, &prm, &fld, &shw);
		if (test_ExeExt(get_extension(xnam))) {
			fnam = xnam;
		}
		else {
			msgbox_WARN("�����N�悪 .exe �t�@�C���ł͂���܂���");
			return;
		}
	}

	if (nbt_sw && test_FileExt(fext, _T(".nbt")))
		fnam = "@" + to_relative_name(fnam);

	//�R���g���[���ɐݒ�
	if (cp->InheritsFrom(__classid(TCustomEdit)))
		((TCustomEdit*)cp)->Text = fnam;
	else if (class_is_ComboBox(cp))
		((TComboBox*)cp)->Text	 = fnam;
}

//---------------------------------------------------------------------------
//���j���[�t�@�C����ǂݍ���
//---------------------------------------------------------------------------
bool load_MenuFile(UnicodeString fnam, TStringList *lst)
{
	fnam = to_absolute_name(cv_env_str(fnam));

	if (load_text_ex(fnam, lst)!=0) {
		int i = 0;
		while (i<lst->Count) {
			UnicodeString lbuf = Trim(lst->Strings[i]);
			if (lbuf.IsEmpty() || StartsStr(';', lbuf)) {
				i++;
				continue;
			}

			TStringDynArray m_buf = split_strings_tab(lbuf);
			//�����s�R�}���h
			if (m_buf.Length>1 && StartsStr('{', m_buf[1])) {
				UnicodeString cmd_str = get_tkn_r(m_buf[1], '{');
				UnicodeString inam;
				int ix = i;
				i++;
				while (i<lst->Count) {
					UnicodeString lbuf = Trim(lst->Strings[i]);
					lst->Delete(i);
					if (lbuf.IsEmpty() || StartsStr(';', lbuf)) continue;
					if (StartsStr('}', lbuf)) {
						//�A�C�R���t�@�C��
						inam = get_post_tab(lbuf);
						inam = to_absolute_name(get_actual_name(inam));
						inam = file_exists_ico(inam)? inam : EmptyStr;
						break;
					}
					else {
						if (!cmd_str.IsEmpty()) cmd_str += ":";
						cmd_str += lbuf;
					}
				}

				lbuf.sprintf(_T("%s\t%s"), m_buf[0].c_str(), cmd_str.c_str());
				if (!inam.IsEmpty()) lbuf.cat_sprintf(_T("\t%s"), inam.c_str());
				lst->Strings[ix] = lbuf;
			}
			//��s�R�}���h
			else {
				//EditMenuFile �R�}���h
				if (m_buf.Length>1 && USAME_TI(m_buf[1], "EditMenuFile")) m_buf[1].sprintf(_T("FileEdit_\"%s\""), fnam.c_str());

				//�A�C�R���t�@�C��
				UnicodeString inam = (m_buf.Length==3)? m_buf[2] : (m_buf.Length==2)? get_file_from_cmd(m_buf[1]) : EmptyStr;
				if (!inam.IsEmpty()) {
					inam = to_absolute_name(get_actual_name(inam));
					inam = file_exists_ico(inam)? inam : EmptyStr;
					lbuf.sprintf(_T("%s\t%s"), m_buf[0].c_str(), m_buf[1].c_str());
					if (!inam.IsEmpty()) lbuf.cat_sprintf(_T("\t%s"), inam.c_str());
				}
				else {
					lbuf = m_buf[0];
					if (m_buf.Length==2) lbuf.cat_sprintf(_T("\t%s"), m_buf[1].c_str());
				}
				lst->Strings[i] = lbuf;
				i++;
			}
		}
	}

	return (lst->Count>0);
}

//---------------------------------------------------------------------------
//���[�N���X�g���t�@�C������ǂݍ���
//�@�t�@�C�����͎��s�p�X����̑��Ύw��ł���
//---------------------------------------------------------------------------
bool load_WorkList(UnicodeString wnam)
{
	wnam = to_absolute_name(wnam);
	if (!file_exists(wnam)) return false;

	UnicodeString msg = make_LogHdr(_T("LOAD"), wnam);

	try {
		clear_FileList(WorkList);
		std::unique_ptr<TStringList> f_lst(new TStringList());
		f_lst->LoadFromFile(wnam);
		WorkListName	 = wnam;
		WorkListTime	 = get_file_age(wnam);
		WorkListChanged  = WorkListFiltered = false;
		rqWorkListDirInf = true;
		for (int i=0; i<f_lst->Count; i++) {
			UnicodeString lbuf = f_lst->Strings[i];
			if (lbuf.IsEmpty() || StartsStr(';', lbuf)) continue;

			UnicodeString fnam = split_pre_tab(lbuf);
			UnicodeString anam = lbuf;
			if (fnam.IsEmpty() && anam.IsEmpty()) continue;

			file_rec *fp = cre_new_file_rec();
			if (fnam.IsEmpty() && is_separator(anam)) {
				//�Z�p���[�^
				fp->alias	 = anam;
				fp->is_dummy = true;
				WorkList->AddObject(fnam, (TObject*)fp);
			}
			else {
				bool is_dir = ends_PathDlmtr(fnam);
				if (is_dir) fnam = ExcludeTrailingPathDelimiter(fnam);

				fp->f_name = fp->r_name = fnam;
				fp->p_name = ExtractFilePath(fnam);
				fp->n_name = ExtractFileName(fnam);
				fp->alias  = anam;

				if (is_dir) {
					fp->is_dir = true;
					fp->b_name = ExtractFileName(fnam);
				}
				else {
					fp->b_name = get_base_name(fnam);
					fp->f_ext  = get_extension(fnam);
				}

				if (!NoCheckWorkUnc || !StartsStr("\\\\", fp->p_name)) {
					bool ok = !is_InvalidUnc(fp->p_name) && (is_dir? dir_exists(fnam) : file_exists(fnam));
					if (ok) {
						fp->f_time	 = get_file_age(fnam);
						fp->f_attr	 = file_GetAttr(fnam);
						fp->is_sym	 = (fp->f_attr & faSymLink);
						if (fp->is_sym) fp->l_name = get_ReparsePointTarget(fnam, fp->is_jct);
						fp->attr_str = get_file_attr_str(fp->f_attr);
						if (!is_dir) fp->f_size = get_file_size(fnam);
					}
					else {
						if (AutoDelWorkList) {
							del_file_rec(fp);
							fp = NULL;
						}
						else {
							inv_file_rec(fp);
						}
					}
				}

				if (fp) WorkList->AddObject(fnam, (TObject*)fp);
			}
		}

		AddLog(msg);
		return true;
	}
	catch (...) {
		clear_FileList(WorkList);

		msg[1] = 'E';
		AddLog(msg);
		return false;
	}
}

//---------------------------------------------------------------------------
//���[�N���X�g��ۑ�
//---------------------------------------------------------------------------
bool save_WorkList(UnicodeString wnam, TStringList *lst)
{
	if (wnam.IsEmpty()) return false;

	if (!lst) lst = WorkList;
	std::unique_ptr<TStringList> fbuf(new TStringList());
	for (int i=0; i<lst->Count; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_up || (fp->is_dummy && fp->alias.IsEmpty())) continue;
		UnicodeString lbuf = (fp->is_dir)? IncludeTrailingPathDelimiter(fp->f_name) : fp->f_name;
		lbuf.cat_sprintf(_T("\t%s"), fp->alias.c_str());
		fbuf->Add(lbuf);
	}

	UnicodeString msg = make_LogHdr(_T("SAVE"), WorkListName);
	bool res = saveto_TextUTF8(wnam, fbuf.get());
	if (res) {
		WorkListChanged  = false;
		rqWorkListDirInf = true;
	}
	else {
		msg[1] = 'E';
	}
	AddLog(msg);

	return res;
}

//---------------------------------------------------------------------------
//�t�H���g�T���v����`��ǂݍ���
//---------------------------------------------------------------------------
bool load_FontSample(UnicodeString fnam)
{
	bool ok = false;
	if (file_exists(fnam)) {
		UnicodeString msg = make_LogHdr(_T("LOAD"), fnam);
		std::unique_ptr<UsrIniFile> smpl_file(new UsrIniFile(fnam));
		UnicodeString sct = "FontSample";
		if (smpl_file->RectionExists(sct)) {
			FontSampleSize	   = smpl_file->ReadInteger(sct, "Size", 20);
			FontSampleBgCol    = xRRGGBB_to_col(smpl_file->ReadString(sct, "BgColor",	"000000"));
			FontSampleFgCol    = xRRGGBB_to_col(smpl_file->ReadString(sct, "FgColor",	"FFFFFF"));
			FontSampleGridCol  = xRRGGBB_to_col(smpl_file->ReadString(sct, "GridColor",	"FF0000"));
			FontSampleShowGrid = smpl_file->ReadBool(sct, "ShowGrid");
			//Text�`
			UnicodeString sbuf = EmptyStr;
			for (int i=0;;i++) {
				UnicodeString lbuf = smpl_file->ReadString(sct, UnicodeString().sprintf(_T("Text%u"), i + 1));
				if (lbuf.IsEmpty()) break;
				sbuf.cat_sprintf(_T("%s\r\n"), lbuf.c_str());
			}
			if (!sbuf.IsEmpty()) FontSampleTxt = sbuf;
			//Sample�`
			sbuf = EmptyStr;
			for (int i=0;;i++) {
				UnicodeString lbuf = smpl_file->ReadString(sct, UnicodeString().sprintf(_T("Symbol%u"), i + 1));
				if (lbuf.IsEmpty()) break;
				sbuf.cat_sprintf(_T("%s\r\n"), lbuf.c_str());
			}
			if (!sbuf.IsEmpty()) FontSampleSym = sbuf;
			ok = true;
		}
		else {
			msg[1] = 'E';
		}
		AddLog(msg);
	}
	return ok;
}
//---------------------------------------------------------------------------
//�t�H���g�T���v����`��ۑ�
//---------------------------------------------------------------------------
bool save_FontSample(UnicodeString fnam)
{
	UnicodeString tmp;
	std::unique_ptr<TStringList> fbuf(new TStringList());
	fbuf->Add("[FontSample]");
	fbuf->Add(tmp.sprintf(_T("Size=%u"),		FontSampleSize));
	fbuf->Add(tmp.sprintf(_T("BgColor=%s"),
		col_to_xRRGGBB((FontSampleBgCol==col_None)? col_bgImage : FontSampleBgCol).c_str()));
	fbuf->Add(tmp.sprintf(_T("FgColor=%s"),
		col_to_xRRGGBB((FontSampleFgCol==col_None)? SelectWorB(col_bgImage) : FontSampleFgCol).c_str()));

	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = FontSampleTxt;
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (!lst->Strings[i].IsEmpty()) fbuf->Add(tmp.sprintf(_T("Text%u=%s"), i + 1, lst->Strings[i].c_str()));
	}
	lst->Text = FontSampleSym;
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lbuf = lst->Strings[i];
		if (!lst->Strings[i].IsEmpty()) fbuf->Add(tmp.sprintf(_T("Symbol%u=%s"), i + 1, lst->Strings[i].c_str()));
	}

	fbuf->Add(tmp.sprintf(_T("ShowGrid=%u"),	FontSampleShowGrid? 1 : 0));
	fbuf->Add(tmp.sprintf(_T("GridColor=%s"),
		col_to_xRRGGBB((FontSampleGridCol==col_None)? clMaroon : FontSampleGridCol).c_str()));

	return saveto_TextUTF8(fnam, fbuf.get());
}

//---------------------------------------------------------------------------
//�w��g���q��L�[���[�h�Ƀ}�b�`���鎟�̃t�@�C��������
//---------------------------------------------------------------------------
int find_NextFile(
	TStringList *lst,
	int idx,
	UnicodeString fext,		//�g���q���X�g						(default = EmptyStr)
	UnicodeString keywd,	//�L�[���[�h(���K�\��)				(default = EmptyStr)
	bool skip_dir,			//�f�B���N�g�����X�L�b�v			(default = true)
	bool circular,			//������Ȃ�������擪����Č���	(default = true)
	bool case_sns,			//�召���������					(default = false)
	bool regex,				//���K�\��							(default = true)
	bool with_tag)			//�^�O���Ώ�						(default = false)
{
	try {
		TRegExOptions opt; if (!case_sns) opt << roIgnoreCase;
		int idx0 = -1, idx1 = -1;
		for (int i=0; i<lst->Count && idx1==-1; i++) {
			if (i<=idx && idx0!=-1) continue;
			file_rec *fp = (file_rec*)lst->Objects[i];
			if ((skip_dir && fp->is_dir) || fp->is_dummy) continue;

			bool match;
			if (!keywd.IsEmpty()) {
				UnicodeString lbuf = (!fp->alias.IsEmpty())? (fp->alias + fp->f_ext) : fp->n_name;
				if (with_tag) lbuf.cat_sprintf(_T("\t%s"), fp->tags.c_str());
				match = regex? TRegEx::IsMatch(lbuf, keywd, opt) : contains_word_and_or(lbuf, keywd, case_sns);
			}
			else {
				match = (fext.IsEmpty() || test_FileExt(fp->f_ext, fext));
			}

			if (match) {
				if (i<=idx) idx0 = i; else idx1 = i;
			}
		}
		return (idx1!=-1)? idx1 : (circular? idx0 : -1);
	}
	catch (...) {
		return -1;
	}
}
//---------------------------------------------------------------------------
//�w��g���q��L�[���[�h�Ƀ}�b�`����O�̃t�@�C��������
//---------------------------------------------------------------------------
int find_PrevFile(
	TStringList *lst,
	int idx,
	UnicodeString fext,		//�g���q���X�g
	UnicodeString keywd,	//�L�[���[�h(���K�\��)
	bool skip_dir,			//�f�B���N�g�����X�L�b�v
	bool circular,			//������Ȃ�������Ōォ��Č���
	bool case_sns,			//�召���������
	bool regex,				//���K�\��
	bool with_tag)			//�^�O���Ώ�
{
	try {
		TRegExOptions opt; if (!case_sns) opt << roIgnoreCase;
		int idx0 = -1, idx1 = -1;
		for (int i=lst->Count-1; i>=0 && idx1==-1; i--) {
			if (i>=idx && idx0!=-1) continue;
			file_rec *fp = (file_rec*)lst->Objects[i];
			if ((skip_dir && fp->is_dir) || fp->is_dummy) continue;

			bool match;
			if (!keywd.IsEmpty()) {
				UnicodeString lbuf = (!fp->alias.IsEmpty())? (fp->alias + fp->f_ext) : fp->n_name;
				if (with_tag) lbuf.cat_sprintf(_T("\t%s"), fp->tags.c_str());
				match = regex? TRegEx::IsMatch(lbuf, keywd, opt) : contains_word_and_or(lbuf, keywd, case_sns);
			}
			else {
				match = (fext.IsEmpty() || test_FileExt(fp->f_ext, fext));
			}

			if (match) {
				if (i>=idx) idx0 = i; else idx1 = i;
			}
		}
		return (idx1!=-1)? idx1 : (circular? idx0 : -1);
	}
	catch (...) {
		return -1;
	}
}

//---------------------------------------------------------------------------
int to_NextFile(TStringList *lst, int idx)
{
	int idx0 = -1, idx1 = -1;
	for (int i=0; i<lst->Count && idx1==-1; i++) {
		if (i<=idx && idx0!=-1) continue;
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_dir || fp->is_dummy || fp->f_attr==faInvalid) continue;
		if (i<=idx) idx0 = i; else idx1 = i;
	}

	return (idx1!=-1)? idx1 : (LoopFilerCursor? idx0 : -1);
}
//---------------------------------------------------------------------------
int to_PrevFile(TStringList *lst, int idx)
{
	int idx0 = -1, idx1 = -1;
	for (int i=lst->Count-1; i>=0 && idx1==-1; i--) {
		if (i>=idx && idx0!=-1) continue;
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_dir || fp->is_dummy || fp->f_attr==faInvalid) continue;
		if (i>=idx) idx0 = i; else idx1 = i;
	}

	return (idx1!=-1)? idx1 : (LoopFilerCursor? idx0 : -1);
}

//---------------------------------------------------------------------------
int to_NextSelItem(TStringList *lst, int idx)
{
	int idx0 = -1, idx1 = -1;
	for (int i=0; i<lst->Count && idx1==-1; i++) {
		if (i<=idx && idx0!=-1) continue;
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (!fp->selected) continue;
		if (i<=idx) idx0 = i; else idx1 = i;
	}

	return (idx1!=-1)? idx1 : (LoopFilerCursor? idx0 : -1);
}
//---------------------------------------------------------------------------
int to_PrevSelItem(TStringList *lst, int idx)
{
	int idx0 = -1, idx1 = -1;
	for (int i=lst->Count-1; i>=0 && idx1==-1; i--) {
		if (i>=idx && idx0!=-1) continue;
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (!fp->selected) continue;
		if (i>=idx) idx0 = i; else idx1 = i;
	}

	return (idx1!=-1)? idx1 : (LoopFilerCursor? idx0 : -1);
}

//---------------------------------------------------------------------------
//�֘A�t������Ă���A�v���̃��X�g���擾
//---------------------------------------------------------------------------
TStringDynArray get_AssociatedApps(UnicodeString fext)
{
	TStringDynArray ret_array;
	for (int i=0; i<AssociateList->Count; i++) {
		if (test_FileExt(fext, AssociateList->Names[i]))
			add_dyn_array(ret_array, exclude_quot(AssociateList->ValueFromIndex[i]));
	}
	return ret_array;
}

//---------------------------------------------------------------------------
//�֘A�t�����X�g���烁�j���[�p���X�g���쐬
//---------------------------------------------------------------------------
void make_AssoMenuList(TStringDynArray app_lst, TStringList *lst)
{
	for (int i=0; i<app_lst.Length; i++) {
		UnicodeString anam = app_lst[i];
		UnicodeString lbuf;
		//�R�}���h
		if (StartsText("ExeCommands_", anam)) {
			lbuf = get_CmdDesc(anam, true);
			anam = del_CmdDesc(anam);
			remove_top_text(anam, "ExeCommands_");
			lbuf.cat_sprintf(_T("\t\t%s"), get_file_from_cmd(anam).c_str());
		}
		//�R�}���h�t�@�C��
		else if (remove_top_AT(anam)) {
			lbuf = get_base_name(anam);
		}
		//�G�C���A�X
		else if (remove_top_Dollar(anam)) {
			UnicodeString inam;	//�A�C�R��
			//�O���c�[��
			TStringDynArray itm_buf = record_of_csv_list(ExtToolList, anam, 4, EXTTOOL_CSVITMCNT);
			if (itm_buf.Length==EXTTOOL_CSVITMCNT) {
				lbuf = itm_buf[0];
				inam = itm_buf[1];
			}
			//�ǉ����j���[
			else {
				itm_buf = record_of_csv_list(ExtMenuList, anam, 3, EXTMENU_CSVITMCNT);
				if (itm_buf.Length==EXTMENU_CSVITMCNT) {
					lbuf = itm_buf[0];
					inam = itm_buf[5];
				}
			}
			if (StartsStr('>', lbuf)) continue;
			if (lbuf.IsEmpty())
				lbuf.sprintf(_T("[%s] �s���ȃG�C���A�X"), anam.c_str());
			else if (!inam.IsEmpty())
				lbuf.cat_sprintf(_T("\t\t%s"), get_actual_path(inam).c_str());
		}
		//�A�v���P�[�V����
		else {
			UnicodeString dsc_str = split_dsc(anam);
			lbuf = def_if_empty(dsc_str, get_base_name(anam));
			lbuf.cat_sprintf(_T("\t\t%s"), get_file_from_cmd(anam).c_str());
		}
		lst->Add(lbuf);
	}
}

//---------------------------------------------------------------------------
//�󗓂Ȃ疳���F��
//---------------------------------------------------------------------------
void InvColIfEmpty(TLabeledEdit *ep)
{
	bool flag = ep->NumbersOnly? (ep->Text.ToIntDef(0)==0) : ep->Text.IsEmpty();
	ep->Color = flag? col_Invalid : scl_Window;
}
//---------------------------------------------------------------------------
void InvColIfEmpty(TEdit *ep)
{
	bool flag = ep->NumbersOnly? (ep->Text.ToIntDef(0)==0) : ep->Text.IsEmpty();
	ep->Color = flag? col_Invalid : scl_Window;
}

//---------------------------------------------------------------------------
//�t�@�C�����̐F���擾
//---------------------------------------------------------------------------
TColor get_FileColor(file_rec *fp, TColor col_x)
{
	if (fp->f_attr==faInvalid)		return col_Error;
	if (fp->f_attr & faSysFile) 	return col_System;
	if (fp->f_attr & faHidden)		return col_Hidden;
	if (fp->f_attr & faReadOnly)	return col_ReadOnly;
	if (col_Compress!=col_None && (fp->f_attr & faCompressed))
									return col_Compress;
	if (fp->is_dir) {
		if (fp->is_sym) 			return col_SymLink;
		if (col_Protect!=col_None && is_ProtectDir(fp->is_up? fp->p_name : fp->f_name))
									return col_Protect;
									return col_Folder;
	}

	return (ColorOnlyFExt? col_fgList : col_x);
}

//---------------------------------------------------------------------------
//�g���q�ʃJ���[���擾
//---------------------------------------------------------------------------
TColor get_ExtColor(
	UnicodeString fext,	//�g���q
	TColor col)			//�f�t�H���g�����F	(default = col_fgList)
{
	if (fext.Pos("<DIR>")) return col_Folder;

	TColor col_f = col_fgList;
	if (USAME_TS(fext, ".")) fext = EmptyStr;
	if (!fext.IsEmpty()) {
		for (int i=0; i<ExtColList->Count; i++) {
			UnicodeString ibuf = ExtColList->Strings[i];
			if (test_FileExt(fext, get_tkn_r(ibuf, ','))) {
				col_f = (TColor)get_tkn(ibuf, ',').ToIntDef(col_fgList);  break;
			}
		}
	}
	//�g���q�Ȃ�(�h�b�g�t�@�C�����܂�)
	else {
		for (int i=0; i<ExtColList->Count; i++) {
			UnicodeString ibuf = ExtColList->Strings[i];
			if (USAME_TS(get_tkn_r(ibuf, ','), ".")) {
				col_f = (TColor)get_tkn(ibuf, ',').ToIntDef(col_fgList);  break;
			}
		}
	}

	return col_f;
}

//---------------------------------------------------------------------------
//�^�C���X�^���v�F���擾
//---------------------------------------------------------------------------
TColor get_TimeColor(TDateTime dt, TColor col_def)
{
	TDateTime dt0 = Now();
	double dd  = dt0 - dt;

	TColor col_t =
			(dd <= 1.0/24) ? col_Tim1H :	//1���Ԉȓ�
			(dd <= 3.0/24) ? col_Tim3H : 	//3���Ԉȓ�
			(dd <= 1.0)	   ? col_Tim1D :	//1���ȓ�
			(dd <= 3.0)	   ? col_Tim3D :	//3���ȓ�
			(dd <= 7.0)	   ? col_Tim7D :	//7���ȓ�
			(dd <= (double)(dt0 - IncMonth(dt0, -1))) ? col_Tim1M :	//1�����ȓ�
			(dd <= (double)(dt0 - IncMonth(dt0, -3))) ? col_Tim3M :	//3�����ȓ�
			(dd <= (double)(dt0 - IncMonth(dt0, -6))) ? col_Tim6M :	//6�����ȓ�
			(dd <= (double)(dt0 - IncYear( dt0, -1))) ? col_Tim1Y :	//1�N�ȓ�
			(dd <= (double)(dt0 - IncYear( dt0, -3))) ? col_Tim3Y :	//3�N�ȓ�
						     						    col_TimOld;

	if (col_t==col_None) col_t = col_def;

	return col_t;
}
//---------------------------------------------------------------------------
//�T�C�Y�F���擾
//---------------------------------------------------------------------------
TColor get_SizeColor(__int64 size, TColor col_def)
{
	TColor col_s =
		           (size==-1) ? col_def    :
		            (size==0) ? col_Size0  :
		(size>=4294967296ull) ? col_Size4G :
		(size>=1073741824ull) ? col_Size1G :
		(size>=1073741824ull) ? col_Size1G :
			(size>=1048576ul) ? col_Size1M :
			     (size>=1024) ? col_Size1K :
								col_SizeLT;

	if (col_s==col_None) col_s = col_def;

	return col_s;
}

//---------------------------------------------------------------------------
//���O�̕\���F���擾
//---------------------------------------------------------------------------
TColor get_LogColor(UnicodeString s)
{
	bool has_tm = (s.Pos(':')==5);
	bool is_err = StartsStr("         �G���[: ", s)
					|| (has_tm && s.Pos("�I��  ") && (s.Pos("  NG:") || s.Pos("  ERR:")))
					|| is_match_regex(s, _T("^.>([ECW]|     [45]\\d{2})\\s"));

	return (					 			  is_err? col_Error :
		 (has_tm && contains_wd_i(s, _T("�J�n|>>")))? col_Headline :
							 StartsText("$ git ", s)? col_Headline :
									(s.Pos('!')==10)? AdjustColor(col_fgLog, ADJCOL_FGLIST)
													: col_fgLog
		);
}

//---------------------------------------------------------------------------
//�f�B���N�g�����܂��̓��[�N���X�g�����擾
//---------------------------------------------------------------------------
UnicodeString get_DirNwlName(UnicodeString pnam)
{
	if (!ends_PathDlmtr(pnam) && test_NwlExt(get_extension(pnam)))
		return ChangeFileExt(ExtractFileName(pnam), EmptyStr);
	else
		return (is_root_dir(pnam)? pnam : get_dir_name(pnam));
}

//---------------------------------------------------------------------------
//�t�@�C�����\����������擾
//---------------------------------------------------------------------------
UnicodeString get_DispName(file_rec *fp)
{
	UnicodeString ret_str;

	if (fp) {
		if (fp->is_dir) {
			ret_str.sprintf(_T("[%s]"), get_dir_name(fp->f_name).c_str());
		}
		else if (fp->is_dummy && is_separator(fp->alias)) {
			ret_str = "< �Z�p���[�^ >";
		}
		else {
			if (!fp->alias.IsEmpty())
				ret_str.sprintf(_T("%s%s (%s)"), fp->alias.c_str(), fp->f_ext.c_str(), fp->n_name.c_str());
			else
				ret_str = fp->n_name;
		}
	}
	return ret_str;
}
//---------------------------------------------------------------------------
//�o�^���ɒu��
//---------------------------------------------------------------------------
UnicodeString get_RegDirName(UnicodeString pnam)
{
	pnam = IncludeTrailingPathDelimiter(pnam);
	if (SameText(pnam, LibraryPath)) return "���C�u����\\";

	int dlen = 0;
	UnicodeString rnam;
	for (int i=0; i<RegDirList->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(RegDirList->Strings[i], 3, true);
		if (is_separator(itm_buf[1])) continue;
		UnicodeString dnam = get_actual_path(itm_buf[2]);
		if (StartsText(dnam, pnam) && dnam.Length()>dlen) {
			dlen = dnam.Length();
			rnam = itm_buf[1];
		}
	}

	UnicodeString ret_nam;
	if (dlen>0) {
		ret_nam.sprintf(_T("<%s>\\"), rnam.c_str());
		ret_nam += pnam.Delete(1, dlen);
	}
	else {
		ret_nam = pnam;
	}

	return ret_nam;
}
//---------------------------------------------------------------------------
//UNC�p�X���l�b�g���[�N�h���C�u���p�X�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString UNC_to_NetDriveName(UnicodeString pnam)
{
	if (StartsStr("\\\\", pnam)) {
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
			if (dp->unc.IsEmpty()) continue;
			if (StartsText(dp->unc, pnam)) {
				pnam = ReplaceText(pnam, dp->unc, dp->drive_str);
				break;
			}
		}
	}
	return pnam;
}
//---------------------------------------------------------------------------
//�l�b�g���[�N�h���C�u���p�X��UNC�p�X�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString NetDriveName_to_UNC(UnicodeString pnam)
{
	if (!StartsStr("\\\\", pnam)) {
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
			if (dp->unc.IsEmpty()) continue;
			if (StartsText(dp->drive_str, pnam)) {
				pnam = ReplaceText(pnam, dp->drive_str, dp->unc);
				break;
			}
		}
	}
	return pnam;
}

//---------------------------------------------------------------------------
//���z�h���C�u���p�X�����ۂ̃p�X�ɕϊ�
//---------------------------------------------------------------------------
UnicodeString VirtualDrive_to_Actual(UnicodeString pnam)
{
	if (!StartsStr("\\\\", pnam)) {
		for (int i=0; i<DriveInfoList->Count; i++) {
			drive_info *dp = (drive_info *)DriveInfoList->Objects[i];
			if (dp->is_virtual && StartsText(dp->drive_str, pnam)) {
				pnam = ReplaceText(pnam, dp->drive_str, IncludeTrailingPathDelimiter(dp->mnt_dir));
				break;
			}
		}
	}
	return pnam;
}

//---------------------------------------------------------------------------
// *._sf �t�@�C������p�X���擾
//---------------------------------------------------------------------------
UnicodeString get_PathFrom_SF(file_rec *fp)
{
	UnicodeString dnam;
	if (fp->is_virtual && !fp->is_ftp) {
		if (SPI->TestFExt(fp->f_ext, true)) {
			std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
			if (SPI->UnPack(fp->arc_name, fp->f_name, ms.get())) {
				std::unique_ptr<TStringList> o_lst(new TStringList());
				int code_page = get_MemoryCodePage(ms.get());
				if (code_page>0) {
					std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
					ms->Seek(0, soFromBeginning);
					o_lst->LoadFromStream(ms.get(), enc.get());
					if (o_lst->Count>0) dnam = o_lst->Strings[0];
				}
			}
		}
	}
	else {
		dnam = get_top_line(fp->f_name);
	}

	dnam = get_tkn_r(dnam, _T("_SF:"));

	return dnam;
}

//---------------------------------------------------------------------------
//�o�^�f�B���N�g���̃p�X���ڂ��擾
//---------------------------------------------------------------------------
UnicodeString get_RegDirItem(int idx)
{
	UnicodeString dnam;
	if (idx>=0 && idx<RegDirList->Count) {
		TStringDynArray itm_buf = get_csv_array(RegDirList->Strings[idx], 3, true);
		if (!itm_buf[2].IsEmpty())
			dnam = exclede_delimiter_if_root(get_actual_path(itm_buf[2]));
	}
	return dnam;
}
//---------------------------------------------------------------------------
UnicodeString get_RegDirItem(WideChar key)
{
	UnicodeString dnam;
	TStringDynArray itm_buf = record_of_csv_list(RegDirList, key, 0, 3);
	if (itm_buf.Length==3 && !itm_buf[2].IsEmpty())
		dnam = exclede_delimiter_if_root(get_actual_path(itm_buf[2]));
	return dnam;
}

//---------------------------------------------------------------------------
//�o�^�f�B���N�g���̎w�荀�ڂ��Z�p���[�^���l�����Đ擪�ֈړ�
//---------------------------------------------------------------------------
void move_top_RegDirItem(int idx)
{
	if (idx>0 && idx<RegDirList->Count) {
		int top = 0;
		for (int i=idx; i>0; i--) {
			TStringDynArray itm_buf = get_csv_array(RegDirList->Strings[i], 3, true);
			if (is_separator(itm_buf[1])) {
				top = i + 1;
				break;
			}
		}
		RegDirList->Move(idx, top);
	}
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�̏㉺���E���擾
//---------------------------------------------------------------------------
UnicodeString get_LRUD_str(
	int  tag,		//�^�O	(default = CurListTag)
	bool both)		//����	(default = false)
{
	return both ? (DivFileListUD? "�㉺" : "���E")
				: (DivFileListUD? ((tag==0)? "��" : "��") : ((tag==0)? "��" : "�E"));
}

//---------------------------------------------------------------------------
//�폜�������ꂽ�f�B���N�g����?
//---------------------------------------------------------------------------
bool is_ProtectDir(UnicodeString dnam)
{
	if (dnam.IsEmpty()) return false;

	dnam = IncludeTrailingPathDelimiter(dnam);
	bool ret = false;
	for (int i=0; i<ProtectDirList->Count && !ret; i++) {
		UnicodeString prt_nam = IncludeTrailingPathDelimiter(ProtectDirList->Strings[i]);
		ret = StartsText(dnam, prt_nam) || (ProtectSubDir && StartsText(prt_nam, dnam));
	}

	return ret;
}

//---------------------------------------------------------------------------
//�t�@�C����񃊃X�g���擾
//---------------------------------------------------------------------------
void GetFileInfList(
	file_rec *fp,
	bool force)			//�����I�Ɏ擾 (default = false)
{
	if (!fp) return;

	flist_stt *lst_stt = (fp->tag!=-1)? &ListStt[fp->tag] : NULL;
	bool is_ads = lst_stt && lst_stt->is_ADS;
	bool is_all = is_FindAll(lst_stt);

	TStringList *i_list = fp->inf_list;
	i_list->Clear();
	fp->prv_text  = EmptyStr;
	fp->tail_text = EmptyStr;

	bool is_inv_unc = is_InvalidUnc(fp->p_name);

	//�t�@�C����
	UnicodeString lbuf;
	if (fp->is_up) {
		if (lst_stt) {
			if (lst_stt->is_Find) {
				lbuf.sprintf(_T("<���ʃ��X�g%s>"),
					lst_stt->find_DICON? _T(" - �t�H���_�A�C�R��") :
					lst_stt->find_HLINK? _T(" - �n�[�h�����N") :
					lst_stt->find_MARK?  _T(" - �}�[�N") :
					lst_stt->find_TAG?   _T(" - �^�O") :
					lst_stt->find_DUPL?  _T(" - �d��") : null_TCHAR);
			}
			else {
				lbuf = "<�J�����g>";
			}
		}
	}
	else {
		lbuf = fp->b_name + fp->f_ext;
		if (!fp->alias.IsEmpty()) lbuf.cat_sprintf(_T("  <%s>"), fp->alias.c_str());
		//�Ď���?
		if (!fp->is_dir && !fp->is_virtual && !fp->is_ftp) {
			for (int i=0; i<WatchTailList->Count; i++) {
				if (SameText(get_pre_tab(WatchTailList->Strings[i]), fp->f_name)) {
					lbuf += "  (�Ď���)";
					break;
				}
			}
		}
	}
	i_list->AddObject(lbuf, (TObject*)(LBFLG_STD_FINF|LBFLG_FILE_FIF));

	//�p�X��
	lbuf = fp->p_name;
	if (is_root_dir(lbuf)) {
		//���z�h���C�u���`�F�b�N
		UnicodeString s = get_VirMountDir(lbuf);
		if (!s.IsEmpty()) lbuf.cat_sprintf(_T(" => %s"), s.c_str());
	}

	if (fp->is_up && lst_stt && lst_stt->is_Find) {
		if (is_all) {
			lbuf = "<�S��>";
		}
		else if (lst_stt->find_MARK) {
			if (lst_stt->find_SubList->Count==0) lbuf = "*";
		}
		else {
			if (!lst_stt->find_DirList.IsEmpty()) lbuf = ExtractFileName(lst_stt->find_DirList) + "/";
			lbuf += lst_stt->find_Mask;
			if (!lst_stt->find_Keywd.IsEmpty()) lbuf.cat_sprintf(_T(" : %s"), lst_stt->find_Keywd.c_str());
		}
	}
	else if (fp->is_virtual && !contains_Slash(fp->f_name)) {
		lbuf = fp->arc_name + "\\" + lbuf;
	}
	else if (fp->is_ftp) {
		lbuf = yen_to_slash(lbuf);
	}
	else if (is_ads) {
		lbuf += ":";
	}
	i_list->AddObject(lbuf, (TObject*)(LBFLG_STD_FINF|LBFLG_PATH_FIF));

	//�����A�T�C�Y�A�^�C���X�^���v
	if (!fp->is_virtual) {
		if (is_inv_unc || is_root_unc(fp->f_name)) {
			if (is_inv_unc) {
				fp->f_attr = faInvalid;
				if (fp->tag!=-1) FileListBox[fp->tag]->Invalidate();
			}
		}
		else if (fp->f_time==(TDateTime)0) {
			fp->f_time = get_file_age(fp->f_name);
			fp->f_attr = file_GetAttr(fp->f_name);
		}
	}
	i_list->AddObject(get_FileInfStr(fp), (TObject*)LBFLG_STD_FINF);

	//�n�b�V��
	if (!fp->hash.IsEmpty()) {
		int n = fp->hash.Length();
		UnicodeString id_str =
			(n==32)? "MD5" : (n==40)? "SHA1" : (n==64)? "SHA256" :
			(n==96)? "SHA384" : (n==128)? "SHA512" : (n==8)? "CRC32" : "";
		if (!id_str.IsEmpty()) add_PropLine(id_str, fp->hash, i_list);
	}

	//�^�O
	if (!fp->is_virtual && !fp->is_ftp) {
		if (fp->tags.IsEmpty()) fp->tags = usr_TAG->GetTags(fp->f_name);
		if (!fp->tags.IsEmpty()) add_PropLine(_T("�^�O"), fp->tags, i_list, LBFLG_TAGS_FIF);
	}

	//�x��
	if (fp->f_attr==faInvalid || is_inv_unc)
		add_WarnLine("���݂��܂���", i_list);
	else
		get_filename_warn(fp->f_name, i_list, fp->is_dir);

	//�f�B���N�g�������
	if (is_inv_unc) return;

	if (fp->is_dir) {
		//FTP�����[�g
		if (fp->is_ftp) {
			add_PropLine(_T("���"), (fp->is_sym? "�V���{���b�N�����N" : "�f�B���N�g��"), i_list);
			//�t�@�C�����A�f�B���N�g����
			if (fp->is_up && lst_stt) {
				if (lst_stt->f_cnt!=-1)
					add_PropLine(_T("�t�@�C����"), get_size_str_B(lst_stt->f_cnt, 0), i_list);
				if (lst_stt->d_cnt!=-1)
					add_PropLine(_T("�f�B���N�g����"), get_size_str_B(lst_stt->d_cnt, 0), i_list);
			}
		}
		//���̑�
		else {
			bool is_git_top = false;
			UnicodeString rpnam = IncludeTrailingPathDelimiter(!fp->is_up? fp->f_name : fp->p_name);

			//���
			if (is_ads) {
				add_PropLine(_T("���"), "��փf�[�^�X�g���[��", i_list);
			}
			else if (fp->is_sym) {
				add_PropLine(_T("���"), fp->is_jct? "�W�����N�V����" : "�V���{���b�N�����N", i_list);
			}
			else if (!is_all) {
				UnicodeString tmp = "�f�B���N�g��";
				if (!fp->is_virtual) {
					is_git_top = dir_exists(rpnam + "\\.git");
					if (is_git_top) tmp += " (���|�W�g��)";
					if (is_ProtectDir(rpnam)) tmp += " (�폜����)";
				}
				add_PropLine(_T("���"), tmp, i_list);
			}

			//�����N��
			if (!fp->l_name.IsEmpty()) {
				add_PropLine(_T("�����N��"), fp->l_name, i_list, LBFLG_PATH_FIF);
				if (file_exists(fp->l_name)) {
					i_list->Add(EmptyStr);
					i_list->AddObject(get_FileInfStr(fp->l_name), (TObject*)LBFLG_STD_FINF);
				}
				else if (fp->is_sym) {
					add_WarnLine("���݂��Ȃ��I�u�W�F�N�g�ւ̃����N", i_list);
				}
			}

			//�t�@�C���A�f�B���N�g����
			if (lst_stt) {
				int f_cnt  = fp->is_up? lst_stt->f_cnt : fp->f_count;
				int d_cnt  = fp->is_up? lst_stt->d_cnt : fp->d_count;
				__int64 f_size	 = fp->is_up? lst_stt->cur_total : fp->f_size;
				__int64 o_size	 = fp->is_up? lst_stt->occ_total : fp->o_size;
				__int64 c_size	 = fp->is_up? lst_stt->cmp_total : fp->c_size;
				__int64 drv_size = lst_stt->drive_Total;
				if (f_cnt!=-1 && !is_all) {
					lbuf = make_PropLine(is_ads? _T("�X�g���[����") : _T("�t�@�C����"), get_size_str_B(f_cnt, 0));
					if (fp->is_up && lst_stt->sub_counted)
						lbuf.cat_sprintf(_T(" / %s"), get_size_str_B(lst_stt->f_cnt_total, 0).c_str());
					i_list->Add(lbuf);
				}
				if (!is_ads && d_cnt!=-1) {
					lbuf = make_PropLine(_T("�f�B���N�g����"), get_size_str_B(d_cnt, 0));
					if (fp->is_up && lst_stt->sub_counted)
						lbuf.cat_sprintf(_T(" / %s"), get_size_str_B(lst_stt->d_cnt_total, 0).c_str());
					i_list->Add(lbuf);
				}

				//�T�C�Y
				if (f_size>0) {
					i_list->Add(get_PropTitle(_T("���v�T�C�Y")).cat_sprintf(_T("%s (%s)"),
						get_size_str_G(f_size, 10, SizeDecDigits, 1).c_str(), get_size_str_B(f_size, 0).c_str()));
					if (!fp->is_virtual && o_size>0) {
						i_list->Add(get_PropTitle(_T("��L�T�C�Y")).cat_sprintf(_T("%s (%s)"),
							get_size_str_G(o_size, 10, SizeDecDigits, 1).c_str(), get_size_str_B(o_size, 0).c_str()));
						__int64 g_size = o_size - f_size;
						i_list->Add(get_PropTitle(_T("�N���X�^gap")).cat_sprintf(_T("%s (%s) %4.1f%%"),
							get_size_str_G(g_size, 10, SizeDecDigits, 1).c_str(), get_size_str_B(g_size, 0).c_str(),
							100.0 * g_size/o_size));
						if (drv_size>0) {
							i_list->Add(
								get_PropTitle(_T("�h���C�u��L��")).cat_sprintf(_T("%6.2f%%"), 100.0 * o_size/drv_size));
						}
						//���k
						if (c_size>=0 && c_size<f_size) {
							i_list->Add(EmptyStr);
							i_list->Add(get_PropTitle(_T("���k�T�C�Y")).cat_sprintf(_T("%s (%s)"),
								get_size_str_G(c_size, 10, SizeDecDigits, 1).c_str(), get_size_str_B(c_size, 0).c_str()));
							i_list->Add(get_PropTitle(_T("���k��")).cat_sprintf(_T("%5.1f %"), 100.0 * c_size/f_size));
							if (fp->is_up && drv_size>0) {
								i_list->Add(
									get_PropTitle(_T("�΃h���C�u�䗦")).cat_sprintf(_T("%6.2f%%"), 100.0 * c_size/drv_size));
							}
						}
					}
				}
			}

			if (!fp->is_virtual && !is_all) {
				UnicodeString cfg_nam = get_dotNaynfi(fp->is_up? fp->p_name : fp->f_name);
				//����
				if (file_exists(cfg_nam)) {
					std::unique_ptr<TStringList> cfg_lst(new TStringList());
					load_text_ex(cfg_nam, cfg_lst.get());
					add_PropLine_if(_T("����"), cfg_lst->Values["Description"], i_list);
				}

				//������
				std::unique_ptr<TStringList> syn_lst(new TStringList());
				get_SyncDstList(fp->is_up? fp->p_name : fp->f_name, syn_lst.get());
				if (syn_lst->Count>1) {
					i_list->Add(EmptyStr);
					for (int i=1; i<syn_lst->Count; i++) {
						add_PropLine(UnicodeString().sprintf(_T("������%u"), i),
							syn_lst->Strings[i], i_list, LBFLG_PATH_FIF);
					}
				}

				//Git���
				if (GitExists && is_git_top) {
					i_list->Add(EmptyStr);
					if (!test_word_i("Remote URL", HideInfItems->Values["\\"]))
						add_PropLine_if(_T("Remote URL"), get_GitUrl(fp), i_list);

					get_GitInf(rpnam, i_list, force, is_NoInfPath(fp, NoInfPath));
				}
			}

			//�s�p�ȃZ�p���[�^���폜
			if (i_list->Count>0 && i_list->Strings[i_list->Count - 1].IsEmpty()) i_list->Delete(i_list->Count - 1);
		}

		//�g���q�ʃt�@�C����
		if (fp->is_up && fp->tag!=-1) add_FExtInfList(GetFileList(fp->tag), i_list);

		//����
		UnicodeString memo = IniFile->GetMarkMemo(fp->r_name);
		if (!memo.IsEmpty()) {
			i_list->Add(EmptyStr);	//�Z�p���[�^
			add_PropLine(_T("����"), memo, i_list);
		}

		//��փf�[�^�X�g���[��
		if (!is_inv_unc && ShowAdsInf && !fp->is_virtual) {
			std::unique_ptr<TStringList> ads_lst(new TStringList());
			get_ADS_Inf(fp->f_name, ads_lst.get());
			if (ads_lst->Count>0) {
				i_list->Add(EmptyStr);
				i_list->AddStrings(ads_lst.get());
			}
		}
	}
	//�t�@�C�����k���
	else if ((fp->f_size>0) && fp->f_attr!=faInvalid && (fp->f_attr & faCompressed)) {
		fp->c_size = get_comp_size(fp->f_name);
		if (fp->c_size>=0) {
			i_list->Add(get_PropTitle(_T("���k�T�C�Y")).cat_sprintf(_T("%s (%s)"),
							get_size_str_G(fp->c_size, 8, SizeDecDigits).c_str(),
							get_size_str_B(fp->c_size, 5).c_str()));
			i_list->Add(get_PropTitle(_T("���k��")).cat_sprintf(_T("%5.1f %"), 100.0 * fp->c_size/fp->f_size));
		}
	}

	if (fp->is_dir) return;

	fp->is_video = test_FileExt(fp->f_ext, FEXT_VIDEO);

	UnicodeString msg;
	if (!force) {
		//�t�@�C�������擾���Ȃ��p�X�̃`�F�b�N
		if (is_NoInfPath(fp, NoInfPath)) return;
		//�t�@�C�������擾����g���q�̃`�F�b�N
		if (!FExtGetInf.IsEmpty() && !test_FileExt(fp->f_ext, FExtGetInf)) return;
		//�t�@�C�������擾���Ȃ��g���q�̃`�F�b�N
		if (test_FileExtSize(fp->f_ext, FExtNoInf, fp->f_size)) return;
		//��������?
		if (!ShowUseProcInf && is_Processing(fp, &msg)) {
			add_WarnLine(msg, i_list);
			return;
		}
	}

	if (ShowUseProcInf && !fp->is_dir && !fp->is_virtual && !fp->is_ftp) {
		std::unique_ptr<TStringList> p_lst(new TStringList());
		int use_cnt = get_ProcessingInf(fp->f_name, p_lst.get());
		if (use_cnt>0) {
			i_list->AddObject(
					make_PropLine(_T("�x��"), msg.sprintf(_T("�ȉ���%u�̃v���Z�X���g�p���ł��B"), use_cnt)),
					(TObject*)LBFLG_ERR_FIF);
			i_list->AddStrings(p_lst.get());
			i_list->Add(EmptyStr);
		}
	}

	//--------------------------
	//�t�@�C�������擾
	//--------------------------
	std::unique_ptr<TStringList> i_buf(new TStringList());
	//._SF
	if (test_FileExt(fp->f_ext, _T("._sf"))) {
		//���
		add_PropLine(_T("���"), "_SF �t�@�C��", i_list);
		//�p�X
		add_PropLine_if(_T("�p�X"), get_PathFrom_SF(fp), i_list);

#if defined(_WIN64)
		i_list->Add(EmptyStr);
#else
		return;
#endif
	}

	if (get_FileInfList(fp, i_buf.get())) i_list->AddStrings(i_buf.get());
}

//---------------------------------------------------------------------------
//�w�肵���t�@�C�����̒l��������擾
//---------------------------------------------------------------------------
UnicodeString get_FileInfValue(
	file_rec *fp,
	UnicodeString tit,		//�^�C�g��
	UnicodeString dlmt)		// dlmt: �l�̋�؂蕶�� (default = EmptyStr)
{
	UnicodeString ret_str;

	for (int i=3; i<fp->inf_list->Count; i++) {
		UnicodeString lbuf = fp->inf_list->Strings[i];
		if (StartsText(tit + ":", Trim(lbuf))) {
			ret_str = get_tkn_r(lbuf, ':');
			if (!dlmt.IsEmpty()) ret_str = get_tkn(ret_str, ';');
			ret_str = Trim(ret_str);
			break;
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�t�@�C���������X�g�{�b�N�X�Ɋ��蓖��
//  �ő區�ږ����� Tag �̉���2�o�C�g�ɐݒ�
//---------------------------------------------------------------------------
void assign_InfListBox(
	TListBox *lp, 
	TStringList *i_lst,		//�t�@�C����񃊃X�g
	UsrScrollPanel *sp)		//�V���v���X�N���[���o�[ (default = NULL)
{
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(FileInfFont);

	bool is_irreg = IsIrregularFont(cv->Font);
	int w_max = 0;
	for (int i=((lp->Tag & LBTAG_OPT_FIF1)? 3: 0); i<i_lst->Count; i++) {
		UnicodeString lbuf = i_lst->Strings[i];
		if (lbuf.Pos(": ")>1) w_max = std::max(w_max, get_TextWidth(cv, get_tkn(lbuf, _T(": ")), is_irreg));
	}
	lp->Tag &= 0x7fff0000;
	lp->Tag |= w_max;

	lp->Items->Assign(i_lst);
	if (sp) sp->UpdateKnob();
}

//---------------------------------------------------------------------------
//�t�@�C�����̕`��
//---------------------------------------------------------------------------
void draw_InfListBox(TListBox *lp, TRect &Rect, int Index, TOwnerDrawState State)
{
	TCanvas *cv = lp->Canvas;
	cv->Font->Assign(FileInfFont);
	bool is_irreg = IsIrregularFont(cv->Font);

	cv->Brush->Color = (State.Contains(odSelected) && lp->Focused())? col_selItem : col_bgInf;
	cv->FillRect(Rect);

	int xp = Rect.Left + Scaled2;
	int yp = Rect.Top  + Scaled1;
	UnicodeString lbuf = lp->Items->Strings[Index];
	bool use_fgsel = lp->Focused() && is_SelFgCol(State);

	//�Z�p���[�^
	if (lbuf.IsEmpty()) {
		draw_separateLine(cv, Rect, 1); return;
	}

	cv->Font->Color = use_fgsel? col_fgSelItem : col_fgInf;

	//��{���/�����N���{���
	int flag = (int)lp->Items->Objects[Index];
	if (flag & LBFLG_STD_FINF) {
		if		(flag & LBFLG_FILE_FIF)	Emphasis_RLO_info(lbuf, cv, xp, yp);
		else if (flag & LBFLG_PATH_FIF)	PathNameOut(lbuf, cv, xp, yp);
		else							cv->TextOut(xp, yp, lbuf);
		return;
	}

	//�ő區�ږ������擾
	int w_max = LOWORD(lp->Tag);

	//���ږ�
	UnicodeString namstr = split_tkn(lbuf, _T(": "));
	xp = Rect.Left + 2 + w_max - get_TextWidth(cv, namstr, is_irreg);
	UnicodeString iname = Trim(namstr);
	namstr += ": ";
	if (!iname.IsEmpty()) {
		cv->Font->Color = use_fgsel? col_fgSelItem :
			(flag & LBFLG_FEXT_FIF)? get_ExtColor(iname) :
			 (flag & LBFLG_ERR_FIF)? col_Error : col_fgInfNam;
		cv->TextOut(xp, yp, namstr);
	}
	xp += get_TextWidth(cv, namstr, is_irreg);

	//���ڒl
	cv->Font->Color = use_fgsel? col_fgSelItem :
					  test_word_i(iname, EmpInfItems)? col_fgInfEmp :
					  (flag & LBFLG_GIT_HASH)? col_GitHash : col_fgInf;

	if		(flag & LBFLG_PATH_FIF)	PathNameOut(lbuf, cv, xp, yp);
	else if (flag & LBFLG_FILE_FIF)	Emphasis_RLO_info(lbuf, cv, xp, yp);
	else if (flag & LBFLG_TAGS_FIF)	usr_TAG->DrawTags(lbuf, cv, xp, yp, RevTagCololr? col_bgInf : col_None);
	else if (flag & LBFLG_FEXT_FIF) {
		xp = xp + (lbuf.Length() * cv->TextWidth("0")) - cv->TextWidth(lbuf);
		cv->TextOut(xp, yp, lbuf);
	}
	else if (flag & LBFLG_TIME_FIF) {
		if (!use_fgsel) {
			try {
				cv->Font->Color = get_TimeColor(TDateTime(lbuf), col_fgInf);
			}
			catch (EConvertError &e) {
				;
			}
		}
		cv->TextOut(xp, yp, lbuf);
	}
	else if (flag & LBFLG_CRCD_FIF) {
		if (!use_fgsel && USAME_TS(lbuf, "����")) cv->Font->Color = col_Error;
		cv->TextOut(xp, yp, lbuf);
	}
	else if (flag & LBFLG_DEBUG) {
		if (!use_fgsel && ContainsStr(lbuf, "�I��")) cv->Font->Color = col_Error;
		EmphasisTextOut(lbuf, EmptyStr, cv, xp, yp);
	}
	else if (flag & LBFLG_GIT_TAG) {
		if (remove_top_s(lbuf, "tags/")) {
			int p = 0;
			for (int i=1; i<=lbuf.Length() && !p; i++) if (lbuf.IsDelimiter("~^", i)) p = i;
			UnicodeString tag = (p>1)? lbuf.SubString(1, p - 1) : lbuf;
			draw_GitTag(cv, xp, yp, tag, Scaled4);
			if (p>1) out_TextEx(cv, xp, yp, lbuf.Delete(1, p - 1));
		}
		else {
			draw_GitTag(cv, xp, yp, lbuf);
		}
	}
	else {
		EmphasisTextOut(lbuf, EmptyStr, cv, xp, yp);
	}
}

//---------------------------------------------------------------------------
//�J���[�ݒ胊�X�g�̕`��
//---------------------------------------------------------------------------
void draw_ColorListBox(TListBox *lp, TRect &Rect, int Index, TOwnerDrawState State, TStringList *col_lst)
{
	TCanvas  *cv = lp->Canvas;
	int yp = Rect.Top + get_TopMargin(cv);

	UnicodeString col_nam = lp->Items->Names[Index];
	UnicodeString vbuf	  = lp->Items->ValueFromIndex[Index];
	bool brk = remove_top_s(vbuf, '|');

	SetHighlight(cv, State.Contains(odSelected));
	cv->FillRect(Rect);
	cv->TextOut(Rect.Left + 34, yp, vbuf);

	//�J���[
	TRect rc = Rect;  rc.Right = rc.Left + 30;
	cv->Brush->Color = (TColor)col_lst->Values[col_nam].ToIntDef(clBlack);
	if (cv->Brush->Color!=col_None)
		cv->FillRect(rc);
	else {
		cv->Brush->Color = scl_BtnFace;
		cv->FillRect(rc);
		out_Text(cv, rc.Left + 2, yp, _T("����"), scl_WindowText);
	}

	//���E��
	cv->Brush->Color = scl_Window;
	if (brk) draw_separateLine(cv, Rect);
}

//---------------------------------------------------------------------------
//���͗��̕`��(�^���L�����b�g�t��)
//---------------------------------------------------------------------------
void draw_InputPaintBox(TPaintBox *pp, UnicodeString s)
{
	TCanvas *cv = pp->Canvas;
	cv->Font->Assign(pp->Font);
	cv->Brush->Color = col_bgList;
	cv->FillRect(pp->ClientRect);

	int xp = Scaled2;
	int yp = get_TopMargin(cv);
	cv->Font->Color = col_fgList;
	cv->TextOut(xp, yp, s);

	xp += get_TextWidth(cv, s, IsIrregularFont(cv->Font));

	//�L�����b�g
	draw_Line(cv, xp, 2, xp, pp->ClientHeight-2, ScaledInt(1), col_Cursor);
}

//---------------------------------------------------------------------------
//�t�@�C���T�C�Y�𕶎����
//---------------------------------------------------------------------------
UnicodeString get_FileSizeStr(__int64 sz,
	bool trim)	// (default = false)
{
	UnicodeString ret_str = ShowByteSize ? get_size_str_B(sz, 14)
										 : get_size_str_G(sz, ((SizeFormatMode>0)? 7 : 10), SizeDecDigits, SizeFormatMode);
	if (trim) ret_str = Trim(ret_str);
	return ret_str;
}

//---------------------------------------------------------------------------
//�t�@�C����񕶎�����擾
//---------------------------------------------------------------------------
UnicodeString get_FileInfStr(file_rec *fp,
	bool with_atr)	//������t��	(default = true)
{
	if (!fp) return EmptyStr;

	UnicodeString ret_str = with_atr? fp->attr_str : EmptyStr;

	if (fp->f_attr!=faInvalid) {
		if (!ret_str.IsEmpty()) ret_str += " ";
		ret_str += get_FileSizeStr(fp->f_size);
		if (!ShowByteSize && fp->f_size!=-1 && EndsStr('B', ret_str))
			ret_str.cat_sprintf(_T(" (%s)"), get_size_str_B(fp->f_size, 0).c_str());

		ret_str += ("  " + format_DateTime(fp->f_time));
	}

	return ret_str;
}
//---------------------------------------------------------------------------
UnicodeString get_FileInfStr(UnicodeString fnam, bool with_atr)
{
	file_rec *fp = cre_new_file_rec(fnam);
	UnicodeString ret_str = get_FileInfStr(fp, with_atr);
	del_file_rec(fp);
	return ret_str;
}

//---------------------------------------------------------------------------
void get_FileNamePathInf(UnicodeString fnam, TStringList *lst, bool get_app)
{
	lst->Add(ExtractFileName(fnam));
	lst->Add(ExtractFilePath(fnam));

	if (file_exists(fnam)) {
		lst->Add(get_FileInfStr(fnam));
		if (get_app) get_AppInf(fnam, lst, false);
	}
	else {
		add_PropLine(_T("�G���["), LoadUsrMsg(USTR_NotFound), lst, LBFLG_ERR_FIF);
	}
}

//---------------------------------------------------------------------------
//�t�@�C����񃊃X�g���擾
//---------------------------------------------------------------------------
bool get_FileInfList(
	file_rec *fp,
	TStringList *lst)	//[o] ��񃊃X�g
{
	if (!fp) return false;

	try {
		lst->Clear();

		UnicodeString fnam = (fp->is_virtual || fp->is_ftp)? fp->tmp_name : fp->f_name;
		UnicodeString fext = fp->f_ext;
		if (!file_exists(fnam)) Abort();

		//----------------------
		//���ڂ̎��
		//----------------------
		UnicodeString tnam = (!fp->is_dir && fp->is_sym)? UnicodeString("�V���{���b�N�����N")
														: usr_SH->get_FileTypeStr(fnam);
		//NyanFi �ŗL�̃t�@�C��
		UnicodeString typ_str = get_IniTypeStr(fp);
		UnicodeString pnam	  = ExtractFilePath(fnam);
		if		(!typ_str.IsEmpty())	tnam.cat_sprintf(_T(" [%s]"), typ_str.c_str());
		else if (SameText(fnam, to_absolute_name(ReplaceLogName)))
										tnam += " [������u�����O]";
		else if (SameText(fnam, to_absolute_name(GrepFileName)))
										tnam += " [GREP���O]";
		else if (is_MenuFile(fp))		tnam += " [���j���[��`]";
		else if (is_ResultList(fp))		tnam += " [���ʃ��X�g]";
		else if (is_ExePath(pnam)) {
			UnicodeString nnam = ExtractFileName(fnam);
			if		(SameText(nnam, TAGDATA_FILE))	tnam += " [�^�O�f�[�^]";
			else if (SameText(nnam, WEBMAP_TPLT))	tnam += " [�}�b�v�\���e���v���[�g]";
			else if (SameText(nnam, DRVLOG_FILE))	tnam += " [�h���C�u�e�ʃ��O]";
			else if (SameText(nnam, RENLOG_FILE))	tnam += " [�������O]";
			else if (SameText(nnam, "NyanFi.exe"))	tnam += " [���s��]";
			else if (is_match_regex_i(nnam, _T("^tasklog\\d?(~\\d)?\\.txt"))) tnam += " [�^�X�N���O]";
		}

		add_PropLine(_T("���"), tnam, lst, LBFLG_TYPE_FIF);

		//�n�[�h�����N��
		int lnk_cnt = get_HardLinkCount(fnam);
		if (lnk_cnt>1) add_PropLine(_T("�n�[�h�����N��"), lnk_cnt, lst);

		//Git URL
		if (GitExists && !test_word_i("Remote URL", HideInfItems->Values["\\"]))
			add_PropLine_if(_T("Remote URL"), get_GitUrl(fp), lst);

		bool fp_created = false;
		file_rec *org_fp = fp;

		//�V���[�g�J�b�g
		if (test_LnkExt(fext)) {
			UnicodeString lnk_name;
			usr_SH->get_LnkInf(fnam, lst, &lnk_name);
			//�����N�悪�t�@�C���Ȃ� file_rec ������� fp ����ւ�
			if (!lnk_name.IsEmpty() && file_exists(lnk_name)) {
				fp->l_name = lnk_name;
				if (dir_exists(lnk_name)) lnk_name = IncludeTrailingPathDelimiter(lnk_name);

				fp = cre_new_file_rec(lnk_name);
				fp_created = true;

				lst->Add(EmptyStr);	//�Z�p���[�^
				//��{���
				lst->AddObject(get_FileInfStr(fp), (TObject*)LBFLG_STD_FINF);
				fnam = fp->f_name;
				fext = fp->f_ext;
			}
		}

		//------------------------------------------
		//�t�@�C���̃V���{���b�N�����N�̃����N��
		//------------------------------------------
		if (!fp->is_dir && fp->is_sym && !fp->l_name.IsEmpty()) {
			add_PropLine(_T("�����N��"), fp->l_name, lst, LBFLG_PATH_FIF);
			add_PropLine(_T("�����N��̎��"), usr_SH->get_FileTypeStr(fp->l_name), lst);
			lst->Add(EmptyStr);	//�Z�p���[�^
			//�����N���{���
			if (file_exists(fp->l_name)) lst->AddObject(get_FileInfStr(fp->l_name), (TObject*)LBFLG_STD_FINF);
		}

		//------------------------------------------
		//�֘A�Â�����Ă���v���O����
		//------------------------------------------
		if (!test_FileExt(fext, FEXT_EXECUTE FEXT_APPINFO _T(".nbt.nwl"))) {
			_TCHAR pszFile[MAX_PATH*2];
			DWORD dwOut = MAX_PATH*2;
			if (::AssocQueryString(ASSOCF_NOTRUNCATE, ASSOCSTR_EXECUTABLE,
				fext.c_str(), _T("open"), pszFile, &dwOut)==S_OK)
			{
				lst->AddObject(UnicodeString().sprintf(_T("%s%s  %s"),
					get_PropTitle(_T("�v���O����")).c_str(),
					ExtractFileName(pszFile).c_str(), ExtractFileDir(pszFile).c_str()),
					(TObject*)LBFLG_PATH_FIF);
			}
		}

		//------------------------------------------
		//�t�@�C�����
		//------------------------------------------
		std::unique_ptr<TStringList> ref_lst(new TStringList());	//�R�}���h�t�@�C���̎Q�Ə��

		int lst_cnt = lst->Count;
		//�A�v���P�[�V�������
		if (test_AppInfExt(fext)) {
			get_AppInf(fnam, lst);
		}
		//�A�C�R��/�J�[�\��
		else if (test_IcoExt(fext) || test_CurExt(fext)) {
			get_IconInf(fnam, lst);
		}
		else if (test_AniExt(fext)) {
			get_AniInf(fnam,  lst);
		}
		//WebP
		else if (USAME_TI(fext, ".webp")) {
			get_WebpInf(fnam, lst);
		}
		//PSP
		else if (test_PspExt(fext)) {
			get_PspInf(fnam,  lst);
		}
		//�\���\�ȉ摜
		else if (is_ViewableFext(fext)) {
			if (test_MetaExt(fext)) {
				get_MetafileInf(fnam, lst);
			}
			else {
				bool x_flag = false;
				if (test_ExifExt(fext))			 	x_flag = get_ExifInf(fnam, lst, &fp->img_ori);
				if (!x_flag && test_PngExt(fext))	x_flag = get_PngInf(fnam, lst);
				if (!x_flag && test_GifExt(fext))	x_flag = get_GifInf(fnam, lst);
				if (!x_flag) {
					unsigned int i_wd, i_hi;
					if (!ContainsStr(lst->Text, "�摜�T�C�Y: ") && get_img_size(fnam, &i_wd, &i_hi))
						lst->Add(get_img_size_str(i_wd, i_hi));
					usr_SH->get_PropInf(fnam, lst);
				}
				if (test_JpgExt(fext)) get_JpgExInf(fnam, lst);
			}
		}
		//Exif
		else if (test_ExifExt(fext)) {
			get_ExifInf(fnam, lst, &fp->img_ori);
		}
		//WAV
		else if (USAME_TI(fext, ".wav")) {
			get_WavInf(fnam, lst);
			UnicodeString tmp = "�r�b�g ���[�g";
			if (!ContainsStr(lst->Text, "����: ")) tmp.Insert("����,", 1);
			usr_SH->get_PropInf(fnam, lst, tmp);
		}
		//MP3
		else if (test_Mp3Ext(fext)) {
			usr_SH->get_PropInf(fnam, lst, "����,�r�b�g ���[�g");
			ID3_GetInf(fnam, lst);
		}
		//FLAC
		else if (test_FlacExt(fext)) {
			get_FlacInf(fnam, lst);
		}
		//Opus
		else if (USAME_TI(fext, ".opus")) {
			usr_SH->get_PropInf(fnam, lst, "����");
			get_OpusInf(fnam, lst);
		}
		//CDA
		else if (USAME_TI(fext, ".cda")) {
			get_CdaInf(fnam, lst);
		}
		else if (USAME_TI(fext, ".url")) {
			std::unique_ptr<UsrIniFile> url_file(new UsrIniFile(fnam));
			add_PropLine(_T("URL"), url_file->ReadString("InternetShortcut", "URL"), lst);
			add_PropLine_if(_T("IconFile"), url_file->ReadString("InternetShortcut", "IconFile"), lst);
		}
		//PDF�o�[�W����
		else if (test_FileExt(fext, _T(".pdf"))) {
			get_PdfVer(fnam, lst);
		}
		//XML
		else if (test_FileExt(fext, FEXT_XML)) {
			get_xml_inf(fnam, lst);
			if (test_LibExt(fext)) get_LibraryList(fnam, lst, true);
		}
		//HTML����
		else if (test_HtmlExt(fext)) {
			get_HtmlInf(fnam, lst);
		}
		//�X�^�C���V�[�g
		else if (USAME_TI(fext, ".css")) {
			UnicodeString lbuf = Trim(get_top_line(fnam));
			if (StartsText("@charset", lbuf))
				add_PropLine(_T("charset"), exclude_quot(Trim(get_tkn(lbuf.Delete(1, 8), ';'))), lst);
		}
		//C++Builder �v���W�F�N�g�A�\�[�X
		else if (test_FileExt(fext, _T(".cbproj.dproj.cpp.pas.dfm.fmx.h"))) {
			get_BorlandInf(fnam, lst);
		}
		//NyanFi �R�}���h�t�@�C��
		else if (test_NbtExt(fext)) {
			UnicodeString lbuf = Trim(get_top_line(fnam));
			if (remove_top_s(lbuf, ';')) add_PropLine(_T("����"), lbuf, lst);
			get_ref_CmdFile(fnam, ref_lst.get());
		}
		//Nyanfi ���[�N���X�g
		else if (test_NwlExt(fext)) {
			;
		}
		//�t�H���g
		else if (test_FontExt(fext)) {
			get_FontInf(fnam, lst);
		}
		//xdoc2txt
		else if (xd2tx_PropExt(fext)) {
			xd2tx_GetInfo(fnam, lst);
		}
		//tags
		else if (USAME_TI(fp->n_name, "tags")) {
			get_TagsInf(fnam, lst);
		}
		//���̑�
		else {
			//����
			if (USAME_TS(usr_SH->get_PropInf(fnam, lst), "�r�f�I")) {
				fp->is_video = true;
			}
			//���s�\�t�@�C��(�`�F�b�N)
			else {
				get_AppInf(fnam, lst);
			}
		}

		//�A�[�J�C�u���
		if (is_AvailableArc(fnam)) {
			if (lst->Count>lst_cnt) lst->Add(EmptyStr);

			add_PropLine_if(_T("�`��"), usr_ARC->GetSubTypeStr(fnam), lst);

			int fcnt;
			__int64 org_size;
			float ratio;
			bool is_enc;
			std::unique_ptr<TStringList> r_lst(new TStringList());
			if (usr_ARC->GetArcInfo(fnam, &fcnt, &org_size, &ratio, &is_enc, r_lst.get())) {
				add_PropLine(_T("�t�@�C����"), get_size_str_B(fcnt, 0), lst);
				if (org_size>0) lst->Add(get_PropTitle(_T("���T�C�Y")).cat_sprintf(_T("%s (%s)"),
									get_size_str_G(org_size, 8, SizeDecDigits).c_str(),
									get_size_str_B(org_size, 5).c_str()));
				if (ratio>0)	lst->Add(get_PropTitle(_T("���k��")).cat_sprintf(_T("%5.1f %"), ratio));
				if (is_enc)		lst->Add(make_PropLine(_T("�p�X���[�h�ی�"), "����"));
				UnicodeString mnam = IniFile->MarkedInArc(fnam);
				if (!mnam.IsEmpty()) {
					lst->Add(make_PropLine(_T("�}�[�N����"), yen_to_delimiter(get_tkn_r(mnam, "/"))));
				}
				//�g���q�ʃt�@�C����
				add_FExtInfList(r_lst.get(), lst);
			}
			else {
				add_WarnLine("�s���܂��͑Ή����Ă��Ȃ��`���ł�", lst);
				InhReload++;	//�G���[�̌J��Ԃ���h������
			}
		}

		//��փf�[�^�X�g���[��
		std::unique_ptr<TStringList> ads_lst(new TStringList());
		if (ShowAdsInf && !fp->is_virtual && !fp->is_ads) get_ADS_Inf(fnam, ads_lst.get());

		//����
		UnicodeString memo = IniFile->GetMarkMemo(fp->r_name);
		if (!memo.IsEmpty()) {
			if (lst->Count>0 && !lst->Strings[lst->Count - 1].IsEmpty()) lst->Add(EmptyStr);	//�Z�p���[�^
			add_PropLine(_T("����"), memo, lst);
		}

		//----------------------
		//�e�L�X�g�v���r���[
		//----------------------
		fp->prv_text  = EmptyStr;
		fp->tail_text = EmptyStr;
		int code_page = 0;
		UnicodeString line_brk;
		bool has_bom;
		bool is_text = is_TextFile(fnam, &code_page, &line_brk, &has_bom);

		if (ShowTextPreview) {
			//���b�`�e�L�X�g
			if (test_FileExt(fext, _T(".rtf.wri"))) {
				if (TempRichEdit) {
					TempRichEdit->Lines->LoadFromFile(fnam);
					fp->prv_text = TempRichEdit->Lines->Text;
					//�������e��ݒ�
					if (ShowTailPreview && PrvTxtTailLn>0) {
						int tp = std::max(TempRichEdit->Lines->Count - PrvTxtTailLn, 0);
						for (int i=tp; i<TempRichEdit->Lines->Count; i++)
							fp->tail_text += TempRichEdit->Lines->Strings[i] + "\r\n";
					}
				}
			}
			//�ʏ�e�L�X�g
			else if (is_text) {
				std::unique_ptr<TStringList> f_buf(new TStringList());
				int lmit_size = (PrvTxtInfLn>0)? std::max(PrvTxtInfLn*1024, TXT_DETECT_SIZE) : 0;		//***
				double ave_lsz = 0;
				code_page = load_text_ex(fnam, f_buf.get(), 0, lmit_size, false, NULL, NULL, &ave_lsz);
				int ln_cnt = f_buf->Count;

				//�G���[
				if (!GlobalErrMsg.IsEmpty()) {
					add_list_errmsg(lst, GlobalErrMsg);
				}
				else {
					//XML
					if (ln_cnt>0) {
						UnicodeString lbuf = f_buf->Strings[0];
						if (ContainsText(lbuf, "<?xml ")) {
							if (!ContainsText(lst->Text, "XML version:")) get_xml_inf(lbuf, lst);
						}
					}

					//�R�[�h�y�[�W
					if (code_page>0) {
						UnicodeString cp_str = make_PropLine(_T("�R�[�h�y�[�W"), code_page);
						UnicodeString cnam	 = get_NameOfCodePage(code_page, true, has_bom);
						if (!cnam.IsEmpty()) cp_str.cat_sprintf(_T(" / %s"), cnam.c_str());
						lst->Add(cp_str);
					}

					//���s�R�[�h
					add_PropLine_if(_T("���s�R�[�h"), line_brk, lst, LBFLG_CRCD_FIF);

					//�s��
					bool over = (PrvTxtInfLn>0 && ln_cnt>PrvTxtInfLn);
					if (over) {
						if (ave_lsz>0 && PrvTxtInfLn>=1000) {
							int n = fp->f_size / ave_lsz;
							n -= n % (int)pow(10, floor(log10(n/10.0)));
							add_PropLine(_T("����s��"), "��" + get_size_str_B(n, 0), lst);
						}
					}
					else {
						add_PropLine(_T("�s��"),   get_size_str_B(ln_cnt, 0), lst);
						add_PropLine(_T("������"), get_size_str_B(f_buf->Text.Length(), 0), lst);
					}

					//CSV/TSV���ڐ�
					if (test_FileExt(fext, FEXT_CSV) && ln_cnt>0) {
						UnicodeString lbuf = f_buf->Strings[0];
						TStringDynArray hdr_buf = ContainsStr(lbuf, "\t")? split_strings_tab(lbuf) : get_csv_array(lbuf, MAX_CSV_ITEM);
						if (hdr_buf.Length>0) add_PropLine(_T("���ڐ�"), hdr_buf.Length, lst);
					}
					//���[�N���X�g�g���q�ʍ��ڐ�
					else if (test_NwlExt(fext) && ln_cnt>0) {
						std::unique_ptr<TStringList> f_lst(new TStringList());
						for (int i=0; i<f_buf->Count; i++) {
							UnicodeString lbuf = get_pre_tab(f_buf->Strings[i]);
							if (!lbuf.IsEmpty() && !StartsStr(';', lbuf)) f_lst->Add(lbuf);
						}
						add_FExtInfList(f_lst.get(), lst);
					}

					//�v���r���[���e��ݒ�
					if (over) {
						for (int i=0; i<PrvTxtInfLn; i++) fp->prv_text += f_buf->Strings[i] + "\r\n";
						fp->prv_text += "\r\n";	//�ő�s��+1�ɂ��Ē��ߔ���
					}
					else {
						fp->prv_text = f_buf->Text;
					}

					//�������e��ݒ�
					if (ShowTailPreview && PrvTxtTailLn>0) {
						if (over) {
							if (load_text_tail(fnam, f_buf.get(), code_page, PrvTxtTailLn)!=0) fp->tail_text = f_buf->Text;
						}
						else {
							int tp = std::max(ln_cnt - PrvTxtTailLn, 0);
							for (int i=tp; i<ln_cnt; i++) fp->tail_text += f_buf->Strings[i] + "\r\n";
						}
					}
				}
			}
		}
		else if (is_text) {
			//�R�[�h�y�[�W
			if (code_page>0) {
				UnicodeString cp_str = make_PropLine(_T("�R�[�h�y�[�W"), code_page);
				UnicodeString cnam = get_NameOfCodePage(code_page, true, has_bom);
				if (!cnam.IsEmpty()) cp_str.cat_sprintf(_T(" / %s"), cnam.c_str());
				lst->Add(cp_str);
			}
			//���s�R�[�h
			add_PropLine_if(_T("���s�R�[�h"), line_brk, lst, LBFLG_CRCD_FIF);
		}

		//�R�}���h�t�@�C���̎Q�Ə��
		if (ref_lst->Count>0) {
			lst->Add(EmptyStr);
			lst->AddStrings(ref_lst.get());
		}

		//��փf�[�^�X�g���[�����𖖔��ɒǉ�
		if (ads_lst->Count>0) {
			lst->Add(EmptyStr);
			lst->AddStrings(ads_lst.get());
		}

		//�����N����𔽉f
		if (fp_created) {
			org_fp->prv_text  = fp->prv_text;
			org_fp->tail_text = fp->tail_text;
			org_fp->img_ori   = fp->img_ori;
			org_fp->is_video  = fp->is_video;
			del_file_rec(fp);
		}

		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//���ڂ͑I���\���H
//---------------------------------------------------------------------------
bool is_selectable(file_rec *fp)
{
	if (!fp)						return false;
	if (fp->is_dummy && is_separator(fp->alias))	return true;	//�Z�p���[�^
	if (fp->is_dummy || fp->is_up)	return false;
	if (fp->b_name.IsEmpty())		return false;
	return true;
}
//---------------------------------------------------------------------------
//�I���\�Ȃ�I��
//---------------------------------------------------------------------------
bool set_select(file_rec *fp, bool sw)
{
	if (is_selectable(fp)) fp->selected = sw;
	return fp? fp->selected : false;
}
//---------------------------------------------------------------------------
bool set_select_r(file_rec *fp)
{
	if (is_selectable(fp)) fp->selected = !fp->selected;
	return fp? fp->selected : false;
}

//---------------------------------------------------------------------------
//�t�@�C������������?
//---------------------------------------------------------------------------
bool is_Processing(
	file_rec *fp,
	UnicodeString *err_msg)	//[0] �G���[���b�Z�[�W (default = NULL)
{
	bool flag = false;
	if (err_msg) *err_msg = EmptyStr;

	if (!fp->is_dir && (fp->f_attr & faReadOnly)==0) {
		UnicodeString fnam = (fp->is_virtual || fp->is_ftp)? fp->tmp_name : fp->f_name;
		flag = is_Processing(fnam, fp->is_video, err_msg);
	}
	return flag;
}
//---------------------------------------------------------------------------
bool is_Processing(
	UnicodeString fnam,
	bool is_video,			//����(�������`�F�b�N)	(default = false)
	UnicodeString *err_msg)	//[o] �G���[���b�Z�[�W	(default = NULL)
{
	bool flag = false;
	if (err_msg) *err_msg = EmptyStr;

	if (!fnam.IsEmpty()) {
		DWORD acc_mod = GENERIC_READ;
		if (is_video) acc_mod |= GENERIC_WRITE;
		HANDLE hFile = ::CreateFile(cv_ex_filename(fnam).c_str(),
			acc_mod, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile==INVALID_HANDLE_VALUE) {
			flag = true;
			if (err_msg) *err_msg = SysErrorMessage(GetLastError());
		}
		else ::CloseHandle(hFile);
	}
	return flag;
}

//---------------------------------------------------------------------------
//�擾���}�~����Ă���p�X���H
//---------------------------------------------------------------------------
bool is_NoInfPath(
	file_rec *fp,
	UnicodeString no_path)	//�p�X���X�g (���ϐ��A%ExePath% �Ή�)
{
	TStringDynArray path_lst = split_strings_semicolon(no_path);
	for (int i=0; i<path_lst.Length; i++) {
		UnicodeString pnam = path_lst[i];
		if (remove_top_Dollar(pnam)) {
			if (pnam.IsEmpty()) continue;
			pnam = pnam.UpperCase();
			UnicodeString dstr = get_drive_str(fp->p_name);
			if (StartsStr("\\\\", pnam)) continue;
			int idx = DriveInfoList->IndexOf(dstr);
			if (idx==-1) continue;
			drive_info *dp = (drive_info *)DriveInfoList->Objects[idx];
			WideChar c0 = pnam[1];
			if (c0=='H' && dp->drv_type==DRIVE_FIXED && !dp->is_SSD && !dp->is_RAM) return true;
			if (c0=='C' && dp->drv_type==DRIVE_CDROM)		return true;
			if (c0=='N' && dp->drv_type==DRIVE_REMOTE)		return true;
			if (c0=='R' && dp->drv_type==DRIVE_REMOVABLE)	return true;
			if (c0=='U' && USAME_TI(dp->bus_type, "USB")) {
				if (pnam.Length()>1) {
					WideChar c1 = pnam[2];
					if (c0=='H' && dp->drv_type==DRIVE_FIXED && !dp->is_SSD) return true;
					if (c1=='C' && dp->drv_type==DRIVE_CDROM)		return true;
					if (c1=='R' && dp->drv_type==DRIVE_REMOVABLE)	return true;
				}
				else return true;
			}
		}
		else {
			pnam = IncludeTrailingPathDelimiter(pnam);
			if (pnam.IsEmpty()) continue;
			if (StartsText(cv_env_str(pnam), fp->p_name)) return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//�X�g���[���̉��s�R�[�h���擾
//---------------------------------------------------------------------------
UnicodeString get_StreamLineBreak(
	TStream *ms,
	int  code_page,
	bool strict)	//���S�`�F�b�N	(default = false);
{
	if (code_page<=0) return EmptyStr;

	bool is_BE = (code_page==1201);
	int  ch_sz = (code_page==1200 || code_page==1201)? 2 : 1;

	int r_cnt  = 0;	//CR
	int n_cnt  = 0;	//LF
	int rn_cnt = 0;	//CR/LF
	bool first = true;
	BYTE cbuf[2]; cbuf[1] = 0;

	ms->Seek(0, soFromBeginning);
	while (ms->Position < ms->Size) {
		if (ms->Read(cbuf, ch_sz)!=ch_sz) break;
		unsigned short c = is_BE? ((cbuf[0] << 8) | cbuf[1]) : ((cbuf[1] << 8) | cbuf[0]);
		bool is_CR = (c==0x000d);
		bool is_LF = (c==0x000a);
		if (is_CR || is_LF) {
			if (first) {
				if		(is_CR) {r_cnt++; first = false;}
				else if (is_LF) n_cnt++;
			}
			else {
				if		(is_CR) r_cnt++;
				else if (is_LF) {rn_cnt++; r_cnt--;}
				first =  true;
			}
			if (strict) {
				//���S�`�F�b�N(�g��������)�̏ꍇ�͒��f���󂯕t����
				Application->ProcessMessages();
	 			if (FindAborted) return EmptyStr;
			}
			else {
				if (first && (r_cnt + n_cnt + rn_cnt)>CRLF_DETECT_LINE) break;
			}
		}
	}

	if (!first) r_cnt--;
	if (rn_cnt>0 && r_cnt==0 && n_cnt==0)	return "CR/LF";
	if (rn_cnt==0 && r_cnt>0 && n_cnt==0)	return "CR";
	if (rn_cnt==0 && r_cnt==0 && n_cnt>0)	return "LF";
	if ((rn_cnt + r_cnt + n_cnt)>0)			return "����";
	if (strict)								return "����";
											return EmptyStr;
}

//---------------------------------------------------------------------------
//�t�@�C���̃R�[�h�y�[�W���擾
//  �߂�l: �R�[�h�y�[�W (0: �s��)
//---------------------------------------------------------------------------
int get_FileCodePage(
	UnicodeString fnam,
	UnicodeString *line_brk,	//[o] ���s		(default = NULL)
	bool *has_bom)				//[o] BOM�L��	(default = NULL)
{
	if (!file_exists(fnam)) return 0;

	int code_page = 0;

	//XML
	UnicodeString enc;
	UnicodeString fext = get_extension(fnam);
	if (test_FileExt(fext, FEXT_XML _T(".xhtml"))) {
		UnicodeString lbuf = get_top_line(fnam);
		if (ContainsText(lbuf, "<?xml ")) {
			int p = pos_i(_T("encoding="), lbuf);
			if (p>0) {
				lbuf = lbuf.SubString(p, lbuf.Length() - p + 1);
				lbuf = get_tkn(get_tkn_m(lbuf, '=', ' '), '?');
				enc  = Trim(exclude_quot(lbuf));
			}
		}
	}
	//HTML
	else if (test_HtmlExt(fext)) {
		UnicodeString lbuf = get_html_header(fnam);
		int p = pos_i(_T("charset="), lbuf);
		if (p>0) {
			lbuf = lbuf.SubString(p, lbuf.Length() - p + 1);
			lbuf = get_tkn(get_tkn_m(lbuf, '=', '>'), '/');
			enc  = Trim(ReplaceStr(lbuf, "\"", ""));
		}
	}

	if (!enc.IsEmpty()) code_page = get_CodePageOfName(enc);

	//���̑�
	if (code_page==0 || line_brk || has_bom) {
		try {
			std::unique_ptr<TFileStream>   fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
			std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
			ms->CopyFrom(fs.get(), std::min<__int64>(fs->Size, TXT_DETECT_SIZE));
			int cpag = get_MemoryCodePage(ms.get(), has_bom);
			if (code_page==0) code_page = cpag;
			//���s�R�[�h
			if (line_brk) *line_brk = get_StreamLineBreak(ms.get(), cpag);
		}
		catch (...) {
			;
		}
	}
	return code_page;
}

//---------------------------------------------------------------------------
//�t�@�C���̓e�L�X�g��?
//---------------------------------------------------------------------------
bool is_TextFile(
	UnicodeString fnam,
	int *code_page,				//[o] �R�[�h�y�[�W	(default = NULL)
	UnicodeString *line_brk,	//[o] ���s			(default = NULL)
	bool *has_bom)				//[o] BOM�L��		(default = NULL)
{
	if (!file_exists(fnam)) return false;

	//���炩�Ƀe�L�X�g�ł͂Ȃ��g���q�͂͂˂�
	if (test_NonTxtExt(get_extension(fnam))) return false;

	//�R�[�h�y�[�W�𒲂ׂ�(�擾�ł��Ȃ��������e�L�X�g�Ƃ݂Ȃ�)
	int cp = get_FileCodePage(fnam, line_brk, has_bom);
	if (code_page) *code_page = cp;
	return (cp>0);
}

//---------------------------------------------------------------------------
//�����񃊃X�g�̍s���A�R�����g�A�󔒍s���J�E���g
//  �߂�l: ���s��
//---------------------------------------------------------------------------
int CountListLines(
	TStringList *lst,		//�����񃊃X�g
	UnicodeString fnam,		//�t�@�C����
	int *rem_cnt,			//[o] �R�����g
	int *blk_cnt)			//[o] �󔒍s
{
	*blk_cnt = 0;

	std::unique_ptr<TStringList> ln_lst(new TStringList());	//�P�s�R�����g
	std::unique_ptr<TStringList> bg_lst(new TStringList());	//�u���b�N�J�n
	std::unique_ptr<TStringList> ed_lst(new TStringList());	//�u���b�N�I��
	//�R�����g����
	if (UserHighlight->GetCommentList(fnam, ln_lst.get(), bg_lst.get(), ed_lst.get())) {
		*rem_cnt = 0;
		bool in_mlt = false;
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			UnicodeString tbuf = Trim(lbuf);
			bool is_blnk = tbuf.IsEmpty();

			if (!in_mlt) {
				bool is_rem = false;
				for (int j=0; j<ln_lst->Count && !is_rem; j++)
					is_rem = StartsText(ln_lst->Strings[j], tbuf);

				if (!is_rem) {
					for (int j=0; j<bg_lst->Count; j++) {
						in_mlt = StartsStr(bg_lst->Strings[j], tbuf);
						if (in_mlt) {
							tbuf.Delete(1, bg_lst->Strings[j].Length());
							break;
						}
					}
				}

				if (is_rem || in_mlt)
					(*rem_cnt)++;
				else if (is_blnk)
					(*blk_cnt)++;
			}
			else if (in_mlt) {
				(*rem_cnt)++;
			}

			if (in_mlt) {
				for (int j=0; j<ed_lst->Count && in_mlt; j++) {
					if (ContainsStr(tbuf, ed_lst->Strings[j])) in_mlt = false;
				}
			}
		}
	}
	//�R�����g����
	else {
		*rem_cnt = -1;
		for (int i=0; i<lst->Count; i++) {
			if (Trim(lst->Strings[i]).IsEmpty()) (*blk_cnt)++;
		}
	}

	return lst->Count;
}

//---------------------------------------------------------------------------
//INI�t�@�C���̃^�C�v��������擾
//---------------------------------------------------------------------------
UnicodeString get_IniTypeStr(file_rec *fp)
{
	if (!fp || fp->is_dummy || fp->is_ftp || fp->is_virtual || !test_FileExt(fp->f_ext, ".ini")) return EmptyStr;

	UnicodeString fnam = cv_VirToOrgName(fp->f_name);
	if (SameText(fnam, IniFile->FileName))	return "�g�p���̐ݒ�";
	if (SameText(fnam, TabGroupName))		return "�g�p���̃^�u�O���[�v";
	if (SameText(fnam, to_absolute_name(IniFile->ReadStrGen(_T("DistrDlgFileName"), DISTR_FILE))))
											return "�U�蕪���o�^";

	if (is_ExePath(ExtractFilePath(fnam))) {
		UnicodeString nnam = ExtractFileName(fnam);
		if (SameText(nnam, HILT_INI_FILE))	return "�\�������\����`";
		if (SameText(nnam, DIR_HIST_FILE))	return "�S�̃f�B���N�g������";
		if (SameText(nnam, CALC_INI_FILE))	return "�d��̒�`";
		if (SameText(nnam, SWATCH_FILE))	return "�F���{��`";
		if (SameText(nnam, FONTSMPL_FILE))	return "�t�H���g�T���v����`";
	}

	std::unique_ptr<UsrIniFile> tmp_file(new UsrIniFile(fnam));
	if (tmp_file->KeyExists("TabList", "Item1")
		&& !tmp_file->RectionExists("KeyFuncList") && !tmp_file->RectionExists("Color"))
			return "�^�u�O���[�v";

	if (tmp_file->RectionExists("Color")
		&& !tmp_file->RectionExists("General") && !tmp_file->RectionExists("KeyFuncList"))
			return "�z�F�ݒ�";

	if (tmp_file->RectionExists("KeyFuncList")
		&& !tmp_file->RectionExists("General") && !tmp_file->RectionExists("Color"))
			return "�L�[�ݒ�";

	return EmptyStr;
}

//---------------------------------------------------------------------------
//���j���[�t�@�C�����H
//---------------------------------------------------------------------------
bool is_MenuFile(file_rec *fp)
{
	return (fp && !fp->is_dummy && !fp->is_ftp && !fp->is_virtual
			&& test_FileExt(fp->f_ext, ".txt")
			&& USAME_TI(get_top_line(fp->f_name), ";[MenuFile]"));
}
//---------------------------------------------------------------------------
//���ʃ��X�g�t�@�C�����H
//---------------------------------------------------------------------------
bool is_ResultList(file_rec *fp)
{
	return (fp && !fp->is_dummy && !fp->is_ftp && !fp->is_virtual
			&& test_FileExt(fp->f_ext, ".txt")
			&& USAME_TI(get_top_line(fp->f_name), ";[ResultList]"));
}

//---------------------------------------------------------------------------
//�r�f�I/�I�[�f�B�I�E�t�@�C�����H
//---------------------------------------------------------------------------
bool is_AudioVideo(UnicodeString fnam)
{
	if (test_FileExt(ExtractFileExt(fnam), FEXT_DURATION)) return true;
	if (contained_wd_i(_T("�r�f�I|�I�[�f�B�I"), usr_SH->get_PropInf(fnam))) return true;
	return false;
}

//---------------------------------------------------------------------------
//�r���A�[�ŕ\���\���H (�A�C�R���͏���)
//---------------------------------------------------------------------------
bool is_ViewableFext(UnicodeString fext)
{
	if (test_FileExt(fext, FEXT_ICONVIEW)) return false;
	if (test_FileExt(fext, FEXT_META + get_img_fext())) return true;
	return false;
}
//---------------------------------------------------------------------------
bool is_Viewable(file_rec *fp)
{
	if (fp->is_dir || fp->is_dummy)	return false;
	if (fp->f_name.IsEmpty())		return false;
	return is_ViewableFext(fp->f_ext);
}

//---------------------------------------------------------------------------
//�A�C�R�����o�\��?
//---------------------------------------------------------------------------
bool is_ExtractIcon(file_rec *fp)
{
	return test_FileExt(fp->f_ext, FEXT_ICONVIEW);
}

//---------------------------------------------------------------------------
//�����A�[�J�C�o�̑Ή��g���q��?
//---------------------------------------------------------------------------
bool test_ArcExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_ARCHIVE) || (usr_ARC->Use7zDll && test_FileExt(fext, FExt7zDll));
}
//---------------------------------------------------------------------------
//�Ή��A�[�J�C�u�̊g���q��?
//---------------------------------------------------------------------------
bool test_ArcExt2(UnicodeString fext)
{
	return (test_ArcExt(fext) || SPI->TestFExt(fext, true));
}
//---------------------------------------------------------------------------
//���p�\�ȃA�[�J�C�u��?
//---------------------------------------------------------------------------
bool is_AvailableArc(UnicodeString fnam,
	bool ex_sw)		//7z.dll�Ή��ł̂��̂��܂�	(default = false)
{
	return (usr_ARC->IsAvailable(fnam, ex_sw) || SPI->TestFExt(get_extension(fnam), true));
}

//---------------------------------------------------------------------------
//�摜�̊g���q���擾
//---------------------------------------------------------------------------
UnicodeString get_img_fext()
{
	return FEXT_WICSTD + WicFextStr + SPI->GetImgFExt();
}

//---------------------------------------------------------------------------
//�e�L�X�g�t�@�C����? (�g���q or �t�@�C�����Ŕ���)
//---------------------------------------------------------------------------
bool test_TxtFile(UnicodeString fnam)
{
	UnicodeString nnam = ExtractFileName(fnam);
	UnicodeString fext = get_extension(fnam);

	if (test_TxtExt(fext)) return true;

	if (fext.IsEmpty()) {
		if (USAME_TI(nnam, "tags") || StartsText(".nyanfi", nnam)) return true;
	}

	return false;
}
//---------------------------------------------------------------------------
//��e�L�X�g�̊g���q��?
//---------------------------------------------------------------------------
bool test_NonTxtExt(UnicodeString fext)
{
	return test_FileExt(fext, FEXT_NONTEXT + WicFextStr);
}

//---------------------------------------------------------------------------
//�g���q�ʃt�@�C��������ǉ�
//  \ �ŏI��閼�O�� " <DIR>"
//---------------------------------------------------------------------------
void add_FExtInfList(
	TStringList *f_lst,	//[i] �t�@�C�������X�g (file_rec �t�ɂ��Ή�)
	TStringList *lst)	//[o] ��񃊃X�g
{
	std::unique_ptr<TStringList> x_lst(new TStringList());
	if (f_lst->Count>0) {
		for (int i=0; i<f_lst->Count; i++) {
			file_rec *fp = (file_rec*)f_lst->Objects[i];
			if (fp && (fp->is_dir || fp->is_dummy)) continue;

			UnicodeString lbuf = f_lst->Strings[i];
			UnicodeString xnam = ends_PathDlmtr(lbuf)? UnicodeString(" <DIR>")
													 : def_if_empty(get_extension(lbuf).LowerCase(), _T("."));
			int idx = x_lst->IndexOf(xnam);
			if (idx!=-1)
				x_lst->Objects[idx] = (TObject*)((NativeInt)x_lst->Objects[idx] + 1);
			else
				x_lst->AddObject(xnam, (TObject*)(NativeInt)1);
		}

		//�ǉ�
		if (x_lst->Count>0) {
			x_lst->Sort();
			lst->Add(EmptyStr);

			int max_cnt = 0;
			for (int i=0; i<x_lst->Count; i++) max_cnt = std::max(max_cnt, (int)x_lst->Objects[i]);
			int w = std::max(UnicodeString(max_cnt).Length(), 3);

			for (int i=0; i<x_lst->Count; i++) {
				lst->AddObject(
					get_PropTitle(x_lst->Strings[i]).cat_sprintf(_T("%*u"), w, (int)x_lst->Objects[i]),
					(TObject*)LBFLG_FEXT_FIF);
			}
		}
	}
}

//---------------------------------------------------------------------------
//�t�@�C������摜�T�C�Y���擾
//---------------------------------------------------------------------------
bool get_img_size(UnicodeString fnam, unsigned int *wd, unsigned int *hi)
{
	UnicodeString fext = get_extension(fnam);

	if (test_PspExt(fext) && get_PspInf(fnam, NULL, wd, hi)) return true;

	bool size_ok = WIC_get_img_size(fnam, wd, hi) || SPI->GetImgSize(fnam, wd, hi);

	if (!size_ok && test_ExifExt(fext)) {
		std::unique_ptr<TStringList> i_lst(new TStringList());	//�t�@�C�����p
		size_ok = get_ExifInf(fnam, i_lst.get(), NULL, wd, hi);
	}

	if (!size_ok && test_MetaExt(get_extension(fnam))) {
		try {
			std::unique_ptr<TMetafile> mf(new TMetafile());
			mf->LoadFromFile(fnam);
			*wd = mf->Width;
			*hi = mf->Height;
			size_ok = true;
		}
		catch (...) {
			;
		}
	}
	return size_ok;
}

//---------------------------------------------------------------------------
//�摜�t�@�C����ǂݍ���
//�߂�l ���s = 0/  ���� = LOADED_BY_STD�ALOADED_BY_WIC�ALOADED_BY_SPI
//---------------------------------------------------------------------------
int load_ImageFile(
	UnicodeString fnam,
	Graphics::TBitmap *o_bmp,
	int wic_type,		//				(default = WICIMG_PREVIEW)
	TColor trans_bg)	//����G�̔w�i�F	(default = clNone)
{
	int res = 0;
	try {
		o_bmp->Canvas->Lock();
		try {
			if (fnam.IsEmpty()) Abort();
			UnicodeString fext = get_extension(fnam);
			//PNG/GIF�͓Ǝ��ɏ���
			if (test_PngExt(fext) || test_GifExt(fext)) {
				std::unique_ptr<TWICImage>wic_img(new TWICImage());
				wic_img->LoadFromFile(fnam);
				wic_img->Transparent = true;
				o_bmp->SetSize(wic_img->Width, wic_img->Height);
				TRect rc = Rect(0, 0, o_bmp->Width, o_bmp->Height);
				if (trans_bg!=col_None) {
					o_bmp->Canvas->Brush->Color = trans_bg;
					o_bmp->Canvas->FillRect(rc);
				}
				o_bmp->Canvas->Draw(0, 0, wic_img.get());
				res = LOADED_BY_STD;
			}
			else {
#if defined(_WIN64)
				if (WIC_load_image(fnam, o_bmp, wic_type))
					res = test_FileExt(fext, FEXT_WICSTD)? LOADED_BY_STD : LOADED_BY_WIC;
#else
				//SPI�D��Ȃ�SPI��
				if (UseSpiFirst && SPI->LoadImage(fnam, o_bmp))
					res = LOADED_BY_SPI;
				//WIC��
				else if (WIC_load_image(fnam, o_bmp, wic_type))
					res = test_FileExt(fext, FEXT_WICSTD)? LOADED_BY_STD : LOADED_BY_WIC;
				//SPI�D��łȂ��Ȃ�SPI��
				else if (!UseSpiFirst && SPI->LoadImage(fnam, o_bmp))
					res = LOADED_BY_SPI;
#endif
			}
		}
		catch (...) {
			res = 0;
		}
	}
	__finally {
		o_bmp->Canvas->Unlock();
	}
	return res;
}

//---------------------------------------------------------------------------
//ViewFileList �ŗL���ȍ��ڐ����擾
//---------------------------------------------------------------------------
int get_ViewCount()
{
	int cnt = 0;
	for (int i=0; i<ViewFileList->Count; i++)
		if (!((file_rec*)ViewFileList->Objects[i])->failed) cnt++;
	return cnt;
}

//---------------------------------------------------------------------------
//�t�@�C���̍폜
//---------------------------------------------------------------------------
bool delete_File(
	UnicodeString fnam,
	bool use_trash)		//���ݔ����g�p	(default = false)
{
	bool res;
	//���ݔ��ɓ����
	if (use_trash) {
		SHFILEOPSTRUCT lpFileOp;
		std::unique_ptr<_TCHAR[]> lpszFil(new _TCHAR[fnam.Length() + 4]);
		_TCHAR *p = lpszFil.get();
		_tcscpy(p, fnam.c_str());
		p += (_tcslen(p) + 1);  *p ='\0';

		lpFileOp.hwnd				   = MainHandle;
		lpFileOp.wFunc				   = FO_DELETE;
		lpFileOp.pFrom				   = lpszFil.get();
		lpFileOp.pTo				   = NULL;
		lpFileOp.fFlags 			   = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_SILENT;
		lpFileOp.fAnyOperationsAborted = true;
		lpFileOp.hNameMappings		   = NULL;
		lpFileOp.lpszProgressTitle	   = NULL;
		res = (::SHFileOperation(&lpFileOp) == 0);
	}
	//�폜
	else {
		res = ::DeleteFile(cv_ex_filename(fnam).c_str());
	}

	//�^�O�̍폜
	if (res) usr_TAG->DelItem(fnam);

	return res;
}
//---------------------------------------------------------------------------
void delete_FileIf(UnicodeString fnam)
{
	if (file_exists(fnam)) delete_File(fnam);
}

//---------------------------------------------------------------------------
//�t�@�C���̈ړ�(�^�O/�t�H���_�A�C�R�����l��)
//---------------------------------------------------------------------------
bool move_FileT(UnicodeString old_nam, UnicodeString new_nam)
{
	bool res = ::MoveFileEx(cv_ex_filename(old_nam).c_str(), cv_ex_filename(new_nam).c_str(),
				MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED | MOVEFILE_WRITE_THROUGH);

	if (res) {
		if (usr_TAG) usr_TAG->Rename(old_nam, new_nam);
		move_FolderIcon(old_nam, new_nam);
	}

	return res;
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R���̈ړ�
//---------------------------------------------------------------------------
void move_FolderIcon(UnicodeString old_dnam, UnicodeString new_dnam)
{
	old_dnam = ExcludeTrailingPathDelimiter(old_dnam);
	new_dnam = ExcludeTrailingPathDelimiter(new_dnam);

	FldIcoRWLock->BeginWrite();
	int idx = FolderIconList->IndexOfName(old_dnam);
	if (idx!=-1) {
		FolderIconList->Strings[idx] = new_dnam + "=" + FolderIconList->ValueFromIndex[idx];
	}
	FldIcoRWLock->EndWrite();
}
//---------------------------------------------------------------------------
//�t�H���_�A�C�R���̃R�s�[
//---------------------------------------------------------------------------
void copy_FolderIcon(UnicodeString src_dnam, UnicodeString dst_dnam)
{
	src_dnam = ExcludeTrailingPathDelimiter(src_dnam);
	dst_dnam = ExcludeTrailingPathDelimiter(dst_dnam);

	FldIcoRWLock->BeginWrite();
	int idx = FolderIconList->IndexOfName(src_dnam);
	if (idx!=-1) {
		FolderIconList->Add(dst_dnam + "=" + FolderIconList->ValueFromIndex[idx]);
	}
	FldIcoRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R���t�@�C���̎擾
//---------------------------------------------------------------------------
UnicodeString get_FolderIconName(UnicodeString dnam)
{
	UnicodeString inam;
	FldIcoRWLock->BeginWrite();
	int idx = FolderIconList->IndexOfName(ExcludeTrailingPathDelimiter(dnam));
	if (idx!=-1) {
		inam = to_absolute_name(FolderIconList->ValueFromIndex[idx]);
	}
	FldIcoRWLock->EndWrite();
	return inam;
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R���̐ݒ�
//---------------------------------------------------------------------------
void set_FolderIcon(
	UnicodeString dnam,		//�f�B���N�g����
	UnicodeString inam)		//�A�C�R���t�@�C����	(default = EmptyStr : ����)
{
	dnam = ExcludeTrailingPathDelimiter(dnam);

	//����
	if (inam.IsEmpty()) {
		FldIcoRWLock->BeginWrite();
		int idx = FolderIconList->IndexOfName(dnam);
		if (idx!=-1) FolderIconList->Delete(idx);
		FldIcoRWLock->EndWrite();
	}
	//�ݒ�
	else {
		UnicodeString lbuf = dnam + "=" + to_relative_name(inam);
		FldIcoRWLock->BeginWrite();
		int idx = FolderIconList->IndexOfName(dnam);
		if (idx==-1)
			FolderIconList->Add(lbuf);
		else
			FolderIconList->Strings[idx] = lbuf;
		FldIcoRWLock->EndWrite();
	}
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R���ꗗ���擾
//---------------------------------------------------------------------------
void get_FolderIconList(TStringList *lst)
{
	lst->Clear();
	FldIcoRWLock->BeginWrite();
	for (int i=0; i<FolderIconList->Count; i++) {
		UnicodeString inam = to_absolute_name(FolderIconList->ValueFromIndex[i]);
		int idx = lst->IndexOf(inam);
		if (idx!=-1)
			lst->Objects[idx] = (TObject*)((NativeInt)lst->Objects[idx] + 1);
		else
			lst->AddObject(inam, (TObject*)(NativeInt)1);
	}
	FldIcoRWLock->EndWrite();
	lst->Sort();
}

//---------------------------------------------------------------------------
//�t�H���_�A�C�R���̉���
//---------------------------------------------------------------------------
void rename_FolderIcon(UnicodeString old_nam, UnicodeString new_nam)
{
	if (!test_FileExt(get_extension(old_nam), FEXT_INDIVICO)) return;

	std::unique_ptr<TStringList> lst(new TStringList());
	get_FolderIconList(lst.get());
	if (lst->IndexOf(ExcludeTrailingPathDelimiter(old_nam))==-1) return;

	new_nam = to_relative_name(new_nam);
	FldIcoRWLock->BeginWrite();
	for (int i=0; i<FolderIconList->Count; i++) {
		UnicodeString inam = to_absolute_name(FolderIconList->ValueFromIndex[i]);
		if (SameText(inam, old_nam)) {
			FolderIconList->Strings[i] = get_tkn(FolderIconList->Strings[i], '=') + "=" + new_nam;
		}
	}
	FldIcoRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//���݂��Ȃ��t�H���_�A�C�R���ݒ�𐮗�
//---------------------------------------------------------------------------
void chk_FolderIcon()
{
	FldIcoRWLock->BeginWrite();
	int i = 0;
	while (i<FolderIconList->Count) {
		UnicodeString dnam = FolderIconList->Names[i];
		if (!StartsStr("\\\\", dnam) && !is_dir_accessible(dnam))
			FolderIconList->Delete(i);
		else
			i++;
	}
	FolderIconList->Sort();
	FldIcoRWLock->EndWrite();
}

//---------------------------------------------------------------------------
//�L�[�Ɋ��蓖�Ă��Ă���R�}���h���擾
//  APP/Shift+F10 �� ContextMenu �ɕϊ�
//---------------------------------------------------------------------------
UnicodeString Key_to_CmdF(UnicodeString key)
{
	UnicodeString cmd = KeyFuncList->Values["F:" + key];
	if (cmd.IsEmpty() && contained_wd_i(KeysStr_Popup, key)) cmd = "ContextMenu";
	return cmd;
}
//---------------------------------------------------------------------------
UnicodeString Key_to_CmdV(UnicodeString key)
{
	return KeyFuncList->Values["V:" + key];
}

//---------------------------------------------------------------------------
bool is_ToLeftOpe(UnicodeString keystr, UnicodeString cmdstr)
{
	return (equal_LEFT(keystr) || contained_wd_i(_T("ToLeft|ToParentOnLeft"), cmdstr));
}
//---------------------------------------------------------------------------
bool is_ToRightOpe(UnicodeString keystr, UnicodeString cmdstr)
{
	return (equal_RIGHT(keystr) || contained_wd_i(_T("ToRight|ToParentOnRight"), cmdstr));
}

//---------------------------------------------------------------------------
//�R�}���h�ɑ΂��������������擾
//---------------------------------------------------------------------------
UnicodeString get_CmdDesc(
	UnicodeString cmd,
	bool only_inf,			//�����̂ݎ擾	(default = false)
	TStrings *menu_list,	//�ǉ����j���[	(default = NULL)
	TStrings *tool_list,	//�O���c�[��	(default = NULL)
	bool is_TV)				//TV�R�}���h	(�x�}�[�N/�s�}�[�N�̋�ʗp	default = false)
{
	if (!menu_list) menu_list = ExtMenuList;
	if (!tool_list) tool_list = ExtToolList;

	UnicodeString prm, modch;
	if (!starts_Dollar(cmd)) {
		prm = get_PrmStr(cmd);
		cmd = get_CmdStr(cmd);
	}

	//����
	UnicodeString dsc = split_dsc(prm);
	if (dsc.IsEmpty()) {
		if (contained_wd_i(_T("ExeExtMenu|ExeExtTool"), cmd)) {
			//�Y�����郁�j���[���ڂ��擾
			TStrings *lst= USAME_TI(cmd, "ExeExtMenu")? menu_list : tool_list;
			for (int i=0; i<lst->Count && dsc.IsEmpty(); i++) {
				UnicodeString ibuf = get_csv_item(lst->Strings[i], 0);
				if (!ContainsStr(ibuf, "&")) continue;
				UnicodeString ak = get_tkn_r(ibuf, '&').SubString(1, 1);
				if (!SameText(ak, prm)) continue;
				dsc = ReplaceStr(ibuf, "&", "");
				break;
			}
		}
		//�G�C���A�X
		else if (starts_Dollar(cmd)) {
			UnicodeString alias = exclude_top(cmd);
			//�ǉ����j���[
			for (int i=0; i<menu_list->Count && dsc.IsEmpty(); i++) {
				TStringDynArray itm_buf = get_csv_array(menu_list->Strings[i], EXTMENU_CSVITMCNT, true);
				if (!SameText(alias, itm_buf[3])) continue;
				dsc = ReplaceStr(itm_buf[0], "&", "");
				break;
			}
			//�O���c�[��
			for (int i=0; i<tool_list->Count && dsc.IsEmpty(); i++) {
				TStringDynArray itm_buf = get_csv_array(tool_list->Strings[i], EXTTOOL_CSVITMCNT, true);
				if (!SameText(alias, itm_buf[4])) continue;
				dsc = ReplaceStr(itm_buf[0], "&", "");
				break;
			}
		}
	}

	if (dsc.IsEmpty()) {
		dsc = CommandList->Values[cmd];
		if (is_TV) {
			dsc = ReplaceStr(dsc, "�x�}�[�N�ꗗ" , "�}�[�N�s�ꗗ");
			dsc = ReplaceStr(dsc, "�x�}�[�N" , "�s�}�[�N");
		}
	}

	if (only_inf) {
		return dsc;
	}
	else {
		if (!prm.IsEmpty()) cmd.cat_sprintf(_T("_%s"), prm.c_str());
		return UnicodeString().sprintf((cmd.Length()<20)? _T("%-20s%s") : _T("%s  %s"), cmd.c_str(), dsc.c_str());
	}
}

//---------------------------------------------------------------------------
//�֎~�����^���[�U��`�����̕ϊ�
//---------------------------------------------------------------------------
UnicodeString ApplyCnvCharList(UnicodeString s)
{
	if (!s.IsEmpty()) {
		for (int i=0; i<CnvCharList->Count; i++) {
			UnicodeString sch = CnvCharList->Names[i];	if (sch.IsEmpty()) continue;
			UnicodeString rch = CnvCharList->ValueFromIndex[i];
			if (StartsStr("\\x{", sch) && EndsStr("}", sch) && chk_RegExPtn(sch))
				s = TRegEx::Replace(s, sch, rch);
			else
				s = ReplaceStr(s, sch, rch);
		}
	}
	return s;
}

//---------------------------------------------------------------------------
TColor read_ColorList(UnicodeString key, TColor def, TStringList *lst)
{
	TColor col = def;
	if (!lst) lst = ColorList;
	UnicodeString vstr = lst->Values[key];
	if (!vstr.IsEmpty())
		col = (TColor)vstr.ToIntDef(def);
	else
		lst->Add(key.cat_sprintf(_T("=%d"), (int)def));
	return col;
}
//---------------------------------------------------------------------------
TColor read_ColorList(const _TCHAR *key, TColor def, TStringList *lst)
{
	return read_ColorList(UnicodeString(key), def, lst);
}

//---------------------------------------------------------------------------
//ColorList ����X�̔z�F���擾
//---------------------------------------------------------------------------
void set_col_from_ColorList()
{
	struct {
		TColor *vp;			//�ϐ��ւ̃|�C���^
		const _TCHAR *key;	//�L�[��=�f�t�H���g
		TColor def;
	} col_def_list[] = {
		{&col_bgList,	_T("bgList"),		clBlack},
		{&col_bgList2,	_T("bgList2"),		col_None},
		{&col_fgList,	_T("fgList"),		clWhite},
		{&col_Splitter,	_T("Splitter"),		clBtnFace},
		{&col_bgArc,	_T("bgArc"),		clNavy},
		{&col_bgFind,	_T("bgFind"),		clNavy},
		{&col_bgWork,	_T("bgWork"),		clNavy},
		{&col_bgFTP,	_T("bgFTP"),		clNavy},
		{&col_bgADS,	_T("bgADS"),		clNavy},
		{&col_selItem,	_T("selItem"),		clBlue},
		{&col_oppItem,	_T("oppItem"),		clNavy},
		{&col_fgSelItem,_T("fgSelItem"),	col_None},
		{&col_bgMark,	_T("bgMark"),		clWebIndigo},
		{&col_matchItem,_T("matchItem"),	clNavy},
		{&col_Differ,	_T("Differ"),		clWebSaddleBrown},
		{&col_DifferN,	_T("DifferN"),		clWebFirebrick},
		{&col_Cursor,	_T("Cursor"),		clFuchsia},
		{&col_bgScrBar,	_T("bgScrBar"),		clAppWorkSpace},
		{&col_bgScrKnob,_T("bgScrKnob"),	clBtnFace},
		{&col_frScrKnob,_T("frScrKnob"),	clBtnShadow},
		{&col_bgActKnob,_T("bgActKnob"),	col_None},
		{&col_lnScrHit,	_T("lnScrHit"),		clYellow},
		{&col_lnScrSel,	_T("lnScrSel"),		col_None},
		{&col_Folder,	_T("Folder"),		clYellow},
		{&col_SymLink,	_T("SymLink"),		clYellow},
		{&col_Protect,	_T("Protect"),		col_None},
		{&col_ReadOnly,	_T("ReadOnly"),		clDkGray},
		{&col_Compress,	_T("Compress"),		col_None},
		{&col_Hidden,	_T("Hidden"),		clTeal},
		{&col_System,	_T("System"),		clPurple},
		{&col_fgSpace,	_T("fgSpace"),		clMaroon},
		{&col_fgTagNam,	_T("fgTagNam"),		clLime},
		{&col_InvItem,	_T("InvItem"),		clGray},
		{&col_bgTabBar,	_T("bgTabBar"),		clBtnFace},
		{&col_bgActTab,	_T("bgActTab"),		clWindow},
		{&col_bgInAcTab,_T("bgInAcTab"),	clBtnFace},
		{&col_frmTab,	_T("frmTab"),		clBlack},
		{&col_fgTab,	_T("fgTab"),		clBtnText},
		{&col_bgListHdr,_T("bgListHdr"),	clBtnFace},
		{&col_fgListHdr,_T("fgListHdr"),	clBtnText},
		{&col_bgDirInf,	_T("bgDirInf"),		clBtnFace},
		{&col_fgDirInf,	_T("fgDirInf"),		clBtnText},
		{&col_bgDirRel,	_T("bgDirRel"),		clBtnFace},
		{&col_fgDirRel,	_T("fgDirRel"),		clBtnText},
		{&col_bgDrvInf,	_T("bgDrvInf"),		clBtnFace},
		{&col_fgDrvInf,	_T("fgDrvInf"),		clBtnText},
		{&col_bgInf,	_T("bgInf"),		clBlack},
		{&col_fgInf,	_T("fgInf"),		clWhite},
		{&col_fgInfNam,	_T("fgInfNam"),		clWhite},
		{&col_fgInfEmp,	_T("fgInfEmp"),		clYellow},
		{&col_bgTxtPrv,	_T("bgTxtPrv"),		clBlack},
		{&col_fgTxtPrv,	_T("fgTxtPrv"),		clWhite},
		{&col_bgLog,	_T("bgLog"),		clBlack},
		{&col_fgLog,	_T("fgLog"),		clWhite},
		{&col_bgTask,	_T("bgTask"),		clBlack},
		{&col_fgPrgBar,	_T("fgPrgBar"),		clLime},
		{&col_bgPrgBar,	_T("bgPrgBar"),		clGray},
		{&col_Error,	_T("Error"),		clRed},
		{&col_bgOptTab,	_T("bgOptTab"),		clHighlight},
		{&col_fgOptTab,	_T("fgOptTab"),		clHighlightText},
		{&col_TlBorder,	_T("TlBorder"),		col_None},
		{&col_bgView,	_T("bgView"),		clNavy},
		{&col_fgView,	_T("fgView"),		clWhite},
		{&col_Margin,	_T("Margin"),		clNavy},
		{&col_bgLineNo,	_T("bgLineNo"),		clNavy},
		{&col_LineNo,	_T("LineNo"),		clTeal},
		{&col_Mark,		_T("Mark"),			clFuchsia},
		{&col_bgRuler,	_T("bgRuler"),		clWindow},
		{&col_fgRuler,	_T("fgRuler"),		clWindowText},
		{&col_bdrLine,	_T("bdrLine"),		clDkGray},
		{&col_bdrFold,	_T("bdrFold"),		clBlue},
		{&col_bdrFixed,	_T("bdrFixed"),		col_None},
		{&col_Comment,	_T("Comment"),		clLime},
		{&col_Strings,	_T("Strings"),		clAqua},
		{&col_Reserved,	_T("Reserved"),		clSkyBlue},
		{&col_Symbol,	_T("Symbol"),		clLtGray},
		{&col_Numeric,	_T("Numeric"),		clFuchsia},
		{&col_fgEmpBin1,_T("fgEmpBin1"),	clYellow},
		{&col_fgEmpBin2,_T("fgEmpBin2"),	clBlue},
		{&col_fgEmpBin3,_T("fgEmpBin3"),	clRed},
		{&col_Headline,	_T("Headline"),		clYellow},
		{&col_URL,		_T("URL"),			clWebLimeGreen},
		{&col_LocalLink,_T("LocalLink"),	clWebDeepPink},
		{&col_bgImage,	_T("bgImage"),		clBlack},
		{&col_bgDblPg,	_T("bgDblPg"),		clWindow},
		{&col_bgWMF,	_T("bgWMF"),		clWindow},
		{&col_fgEmp,	_T("fgEmp"),		clYellow},
		{&col_bgEmp,	_T("bgEmp"),		clNavy},
		{&col_Ruby,		_T("Ruby"),			clGray},
		{&col_TAB,		_T("TAB"),			clBlue},
		{&col_CR,		_T("CR"),			clPurple},
		{&col_HR,		_T("HR"),			clDkGray},
		{&col_Ctrl,		_T("Ctrl"),			clFuchsia},
		{&col_bdrThumb,	_T("bdrThumb"),		clBtnFace},
		{&col_ThumbExif,_T("ThumbExif"),	clWhite},
		{&col_ImgGrid,	_T("ImgGrid"),		clGray},
		{&col_bgHint,	_T("bgHint"),		Application->HintColor},
		{&col_fgHint,	_T("fgHint"),		clBlack},
		{&col_bgWarn,	_T("bgWarn"),		clRed},
		{&col_OptFind,	_T("OptFind"),		clRed},
		{&col_Invalid,	_T("Invalid"),		clLtGray},
		{&col_Illegal,	_T("Illegal"),		clWebPink},
		{&col_Tim1H,	_T("Tim1H"),		col_None},
		{&col_Tim3H,	_T("Tim3H"),		col_None},
		{&col_Tim1D,	_T("Tim1D"),		col_None},
		{&col_Tim3D,	_T("Tim3D"),		col_None},
		{&col_Tim7D,	_T("Tim7D"),		col_None},
		{&col_Tim1M,	_T("Tim1M"),		col_None},
		{&col_Tim3M,	_T("Tim3M"),		col_None},
		{&col_Tim6M,	_T("Tim6M"),		col_None},
		{&col_Tim1Y,	_T("Tim1Y"),		col_None},
		{&col_Tim3Y,	_T("Tim3Y"),		col_None},
		{&col_TimOld,	_T("TimOld"),		col_None},
		{&col_Size4G,	_T("Size4G"),		col_None},
		{&col_Size1G,	_T("Size1G"),		col_None},
		{&col_Size1M,	_T("Size1M"),		col_None},
		{&col_Size1K,	_T("Size1K"),		col_None},
		{&col_SizeLT,	_T("SizeLT"),		col_None},
		{&col_Size0,	_T("Size0"),		col_None},
		{&col_GrBack,	_T("GrBack"),		clBlack},
		{&col_GrLine,	_T("GrLine"),		clLime},
		{&col_GrGrid,	_T("GrGrid"),		clGray},
		{&col_GrText,	_T("GrText"),		clLtGray},
		{&col_GitHEAD,	_T("GitHEAD"),		clFuchsia},
		{&col_GitMark,	_T("GitMark"),		clAqua},
		{&col_GitBra,	_T("GitBra"),		clGreen},
		{&col_GitBraR,	_T("GitBraR"),		clOlive},
		{&col_GitTag,	_T("GitTag"),		clYellow},
		{&col_GitHash,	_T("GitHash"),		clGray},
		{&col_GitIns,	_T("GitIns"),		clGreen},
		{&col_GitDel,	_T("GitDel"),		clRed},
		{&col_bgTlBar1,	_T("bgTlBar1"),		clWhite},
		{&col_bgTlBar2,	_T("bgTlBar2"),		clWebGainsboro},
		{&col_fgTlBar,	_T("fgTlBar"),		clBlack},
		{&col_htTlBar,	_T("htTlBar"),		TColor(0x00efd3c6)},
		{&col_bgSttBar,	_T("bgSttBar"),		scl_BtnFace},
		{&col_fgSttBar,	_T("fgSttBar"),		scl_BtnText},
		{&col_bgInfHdr,	_T("bgInfHdr"),		scl_BtnFace},
		{&col_fgInfHdr,	_T("fgInfHdr"),		scl_BtnText},
		{&col_bgChInf,	_T("bgChInf"),		scl_Window},
		{&col_fgChInf,	_T("fgChInf"),		scl_BtnText},
		{&col_bgEdBox,	_T("bgEdBox"),		col_None},
		{&col_fgEdBox,	_T("fgEdBox"),		col_None},
		{&col_ModalScr,	_T("ModalScr"),		clBlack}
	};

	int cnt = sizeof(col_def_list)/sizeof(col_def_list[0]);
	for (int i=0; i<cnt; i++)
		*col_def_list[i].vp = read_ColorList(col_def_list[i].key, col_def_list[i].def);

	//�f�t�H���g�̃^�O�F
	usr_TAG->SetColor(EmptyStr, col_fgTagNam);
}

//---------------------------------------------------------------------------
//�ҏW������ǉ�
//---------------------------------------------------------------------------
void add_TextEditHistory(
	UnicodeString fnam)		//�t�@�C����(���p����)
{
	fnam = exclude_quot(fnam);
	if (StartsText(TempPathA, fnam)) return;
	if (match_path_list(fnam, NoEditHistPath)) return;

	int i=0;
	while (i < TextEditHistory->Count)
		if (SameText(TextEditHistory->Strings[i], fnam)) TextEditHistory->Delete(i); else i++;
	TextEditHistory->Insert(0, fnam);
}

//---------------------------------------------------------------------------
//�Ăяo���z�b�g�L�[�̓o�^
//---------------------------------------------------------------------------
bool register_HotKey(int id, UnicodeString kstr)
{
	GlobalErrMsg = EmptyStr;

	try {
		int idx = HotKeyList->IndexOfName(IntToStr(id));
		if (idx!=-1) {
			if (!::UnregisterHotKey(MainHandle, id)) Abort();
			HotKeyList->Delete(idx);
		}

		if (!kstr.IsEmpty()) {
			bool windows_key   = remove_text(kstr, _T("Win+"));
			TShortCut shortcut = TextToShortCut(kstr);
			Word vkcode;
			TShiftState ss;
			ShortCutToKey(shortcut, vkcode, ss);

			UINT modflag = 0;
			if (ss.Contains(ssShift)) modflag |= MOD_SHIFT;
			if (ss.Contains(ssAlt))   modflag |= MOD_ALT;
			if (ss.Contains(ssCtrl))  modflag |= MOD_CONTROL;
			if (windows_key)		  modflag |= MOD_WIN;
			if (modflag!=0 && vkcode!=0) {
				if (!::RegisterHotKey(MainHandle, id, modflag, vkcode)) Abort();
				HotKeyList->Add(UnicodeString().sprintf(_T("%u=%s"), id, kstr.c_str()));
			}
		}

		return true;
	}
	catch (EAbort &e) {
		GlobalErrMsg = SysErrorMessage(GetLastError());
		switch (id) {
		case ID_CALL_HOTKEY: GlobalErrMsg += "\r\n�uNyanFi �Ăяo���v�̐ݒ���m�F���Ă��������B";	break;
		case ID_APP_HOTKEY:  GlobalErrMsg += "\r\n�uAppList �R�}���h�v�̐ݒ���m�F���Ă��������B";	break;
		}
		return false;
	}
}

//---------------------------------------------------------------------------
//�t�H�[���̃^�C�g����ݒ�
//---------------------------------------------------------------------------
void set_FormTitle(TForm *frm, const _TCHAR *s)
{
	frm->Caption = s;
}

//---------------------------------------------------------------------------
//�x����
//---------------------------------------------------------------------------
void beep_Warn(UnicodeString fnam)
{
	if (Initialized && !UnInitializing) {
		if (fnam.IsEmpty()) fnam = SoundWarning;
		bool fw = (FlashCntWarning>0 && FlashTimeWarning>0);
		if (!play_sound(fnam) && !fw) ::MessageBeep(MB_ICONEXCLAMATION);
		if (fw) flash_win(FlashCntWarning, FlashTimeWarning);
	}
}

//---------------------------------------------------------------------------
//�^�C�g���o�[�̓_��
//---------------------------------------------------------------------------
void flash_win(
	int cnt,	//�_�ŉ� default = 3
	int tm)		//�_�ŊԊu default = 250
{
	if (cnt<=0) cnt = 3;
	if (tm<=0)  tm  = 250;

	FLASHWINFO fwi;
	fwi.cbSize    = sizeof(FLASHWINFO);
	fwi.hwnd      = Screen->ActiveForm->Handle;
	fwi.dwFlags   = FLASHW_ALL;
	fwi.uCount    = cnt;
	fwi.dwTimeout = tm;
	::FlashWindowEx(&fwi);
}

//---------------------------------------------------------------------------
//�^�C�g���o�[�̕���{�^����L��/������
//---------------------------------------------------------------------------
void set_CloseButton(bool enabled)
{
	HMENU hMenu = ::GetSystemMenu(MainHandle, false);
	if (hMenu) {
		int mc = ::GetMenuItemCount(hMenu);
		if (mc>0) {
			int mi = ::GetMenuItemID(hMenu, mc - 1);
			::EnableMenuItem(hMenu, mi, enabled? MF_ENABLED : MF_GRAYED);
		}
	}
}

//---------------------------------------------------------------------------
//���ʃ~���[�g
//---------------------------------------------------------------------------
bool mute_Volume(
	UnicodeString prm)	// ON/OFF/GET(�擾�̂�) ��̏ꍇ�̓g�O��
{
	try {
		TComInterface<IMMDeviceEnumerator> devEnumerator;
		TComInterface<IMMDevice> device;
		TComInterface<IAudioEndpointVolume> endp_vol;

		if (FAILED(CoCreateInstance(
			__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&devEnumerator)))) Abort();

		if (FAILED(devEnumerator->GetDefaultAudioEndpoint(
			EDataFlow::eRender, ERole::eMultimedia, &device))) Abort();

		if (FAILED(device->Activate(
			__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (void**)&endp_vol))) Abort();

		BOOL mute = FALSE;
		if (FAILED(endp_vol->GetMute(&mute))) Abort() ;
		IsMuted = mute;

		if (!USAME_TI(prm, "GET")) {
			IsMuted = USAME_TI(prm, "ON")? true : USAME_TI(prm, "OFF")? false : !IsMuted;
			mute = IsMuted;
			if (FAILED(endp_vol->SetMute(mute, NULL))) Abort();
		}
		return true;
	}
	catch (EAbort &e) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�T�E���h�Đ�
// �t�@�C�����͎��s�p�X����̑��Ύw��ł���
// �t�@�C�������݂��Ȃ��Ƃ��͉����炳�Ȃ�
//---------------------------------------------------------------------------
bool play_sound(UnicodeString fnam)
{
	fnam = to_absolute_name(fnam);
	if (!file_exists(fnam)) return false;

	::sndPlaySound(fnam.c_str(), SND_ASYNC);
	return true;
}

//---------------------------------------------------------------------------
//MCI�ɂ��T�E���h�Đ�(WAV�AWMA�AMP3�AMID)
//�I���C������ MM_MCINOTIFY ������
//---------------------------------------------------------------------------
bool play_sound_ex(
	UnicodeString fnam,		//�t�@�C����(���s�p�X����̑��Ύw��ł���)
	bool limit)				//�b���������� (default = true)
{
	::mciSendString(_T("close TPLYSND"), NULL, 0, NULL);

	fnam = to_absolute_name(fnam);
	if (!file_exists(fnam)) return false;
	if (limit && PlaySndLimitTime==0) return true;

	bool res = false;

	cursor_HourGlass();
	UnicodeString cmd;
	cmd.sprintf(_T("open \"%s\""), fnam.c_str());
	if (test_Mp3Ext(get_extension(fnam))) cmd += " type MPEGVideo";
	cmd += " alias TPLYSND";
	if (::mciSendString(cmd.c_str(), NULL, 0, NULL)==0) {
		if (limit) {
			::mciSendString(_T("set TPLYSND time format milliseconds"), NULL, 0, NULL);
			_TCHAR rbuf[256];
			if (::mciSendString(_T("status TPLYSND length"), rbuf, 255, NULL)==0) {
				int t = std::min(UnicodeString(rbuf).ToIntDef(0), PlaySndLimitTime*1000);
				if (t>0) {
					cmd.sprintf(_T("play TPLYSND from 0 to %u notify"), t);
					res = (::mciSendString(cmd.c_str(), NULL, 0, MainHandle)==0);
				}
			}
		}
		else {
			res = (::mciSendString(_T("play TPLYSND notify"), NULL, 0, MainHandle)==0);
		}

		if (!res) ::mciSendString(_T("close TPLYSND"), NULL, 0, NULL);
	}
	cursor_Default();
	return res;
}

//---------------------------------------------------------------------------
//�v���C���X�g�ɒǉ�
//---------------------------------------------------------------------------
bool add_PlayFile(UnicodeString fnam)
{
	if (!test_MciSndExt(get_extension(fnam))) return false;
	PlayList->Add(fnam);
	return true;
}
//---------------------------------------------------------------------------
bool add_PlayList(UnicodeString lnam)
{
	PlayListFile = EmptyStr;

	lnam = to_absolute_name(lnam);
	int last_cnt = PlayList->Count;

	//�f�B���N�g��
	if (dir_exists(lnam)) {
		std::unique_ptr<TStringList> fbuf(new TStringList());
		get_files(lnam, _T("*.*"), fbuf.get(), true);
		for (int i=0; i<fbuf->Count; i++) add_PlayFile(fbuf->Strings[i]);
	}
	//�t�@�C��
	else if (file_exists(lnam)) {
		//�T�E���h�t�@�C��
		if (add_PlayFile(lnam))
			;
		//���X�g�t�@�C��
		else if (is_TextFile(lnam)) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			if (load_text_ex(lnam, fbuf.get())!=0) {
				PlayListFile = lnam;
				for (int i=0; i<fbuf->Count; i++) {
					UnicodeString inam = get_pre_tab(fbuf->Strings[i]);
					//�f�B���N�g��
					if (dir_exists(inam)) {
						std::unique_ptr<TStringList> fbuf(new TStringList());
						get_files(inam, _T("*.*"), fbuf.get(), true);
						for (int i=0; i<fbuf->Count; i++) add_PlayFile(fbuf->Strings[i]);
					}
					//�t�@�C��
					else if (file_exists(inam)) add_PlayFile(inam);
				}
			}
		}
	}

	return (PlayList->Count>last_cnt);
}

//---------------------------------------------------------------------------
//�v���C���X�g���Đ�
//---------------------------------------------------------------------------
bool play_PlayList(
	bool prev,			//�O�̋Ȃ�			(default = false)
	bool inh_shfl)		//�V���b�t���}�~	(default = false)
{
	PlayFile  = EmptyStr;
	ListShuffled = false;
	::mciSendString(_T("close PLYLIST"), NULL, 0, NULL);
	if (PlayList->Count==0) return false;

	if (prev) {
		if (PlayStbIdx>1) PlayStbIdx -= 2; else PlayStbIdx = 0;
	}
	else {
		//�V���b�t��
		if (!inh_shfl && PlayShuffle && PlayList->Count>1 && PlayStbIdx==0) {
			for (int i=0; i<PlayList->Count; i++) {
				int j = Random(PlayList->Count);
				UnicodeString s = PlayList->Strings[i];
				PlayList->Strings[i] = PlayList->Strings[j];
				PlayList->Strings[j] = s;
			}
			ListShuffled = true;
		}
	}

	bool res = false;
	int ng_cnt = 0;
	while (PlayStbIdx<PlayList->Count && ng_cnt<PlayList->Count && !res) {
		UnicodeString fnam = PlayList->Strings[PlayStbIdx];
		if (file_exists(fnam)) {
			UnicodeString cmd;
			cmd.sprintf(_T("open \"%s\""), fnam.c_str());
			if (test_Mp3Ext(get_extension(fnam))) cmd += " type MPEGVideo";
			cmd += " alias PLYLIST";
			if (::mciSendString(cmd.c_str(), NULL, 0, NULL)==0) {
				res = (::mciSendString(_T("play PLYLIST notify"), NULL, 0, MainHandle)==0);
				if (res) {
					PlayFile = fnam;
					AddLog(make_LogHdr(_T("PLAY"), fnam));
				}
				else  {
					::mciSendString(_T("close PLYLIST"), NULL, 0, NULL);
				}
			}
		}
		if (!res) ng_cnt++;
		PlayStbIdx++;
		if (PlayRepeat && PlayStbIdx==PlayList->Count) PlayStbIdx = 0;
	}
	return res;
}

//---------------------------------------------------------------------------
//�^�u���l�����ĕ����񕝂��擾
//---------------------------------------------------------------------------
int get_TabTextWidth(
	UnicodeString s, TCanvas *cv,
	int tab_wd)		//�^�u�X�g�b�v��
{
	int wd = 0;
	TEXTMETRIC tm;
	::GetTextMetrics(cv->Handle , &tm);
	int w_tab = tab_wd * tm.tmAveCharWidth;

	bool is_irreg = IsIrregularFont(cv->Font);

	if (tab_wd>0) {
		while (!s.IsEmpty()) {
			int p = s.Pos('\t');
			if (p>0) {
				if (p>1) wd += get_TextWidth(cv, s.SubString(1, p - 1), is_irreg);
				wd += (w_tab - (wd % w_tab));
				s.Delete(1, p);
			}
			else {
				wd += get_TextWidth(cv, s, is_irreg);
				break;
			}
		}
	}
	else {
		wd = get_TextWidth(cv, s, is_irreg);
	}

	return wd;
}

//---------------------------------------------------------------------------
//������̕`��
//---------------------------------------------------------------------------
void out_Text(TCanvas *cv, int x, int y, const _TCHAR *s)
{
	cv->TextOut(x, y, s);
}
//---------------------------------------------------------------------------
void out_Text(TCanvas *cv, int x, int y, const _TCHAR *s, TColor fg)
{
	cv->Font->Color = fg;
	cv->TextOut(x, y, s);
}
//---------------------------------------------------------------------------
void out_TextEx(
	TCanvas *cv, int &x, int y,
	UnicodeString s,
	TColor fg, TColor bg,	//	(default = clNone)
	int mgn,				//	(default = 0)
	bool is_irreg)			//	(default = false)
{
	if (s.IsEmpty()) return;

	TColor org_fg = cv->Font->Color;
	TColor org_bg = cv->Brush->Color;

	if (fg!=col_None) cv->Font->Color  = fg;
	if (bg!=col_None) cv->Brush->Color = bg;

	cv->TextOut(x, y, s);

	x += (get_TextWidth(cv, s, is_irreg) + mgn);
	cv->Font->Color  = org_fg;
	cv->Brush->Color = org_bg;
}

//---------------------------------------------------------------------------
//������ɑ΂���}�b�`��̃��X�g���擾
//---------------------------------------------------------------------------
int get_MatchWordList(
	UnicodeString lbuf,	//�Ώە�����
	UnicodeString kwd,	//������
	bool migemo_sw,		//Migemo���g�p
	bool regex_sw,		//kwd �͐��K�\��
	bool and_or_sw,		//AND(' ') / OR('|') ����
	bool case_sw,		//�召���������
	TStringList *lst)	//[o] �}�b�`�ꃊ�X�g
{
	lst->Clear();

	TRegExOptions opt;
	if (!case_sw) opt << roIgnoreCase;

	//AND/OR����
	if (and_or_sw) {
		TStringDynArray or_lst = SplitString(Trim(kwd), "|");
		for (int i=0; i<or_lst.Length; i++) {
			std::unique_ptr<TStringList> tmp_lst(new TStringList());
			TStringDynArray and_lst = SplitString(Trim(or_lst[i]), " ");
			bool and_ok = true;
			for (int j=0; j<and_lst.Length && and_ok; j++) {
				UnicodeString s   = Trim(and_lst[j]);
				UnicodeString ptn = regex_sw? s : migemo_sw? usr_Migemo->GetRegExPtn(migemo_sw, s) : TRegEx::Escape(s);
				if (ptn.IsEmpty()) continue;
				TMatchCollection mts = TRegEx::Matches(lbuf, ptn, opt);
				and_ok = (mts.Count>0);
				if (and_ok) for (int j=0; j<mts.Count; j++) tmp_lst->Add(mts.Item[j].Value);
			}

			if (and_ok) {
				lst->AddStrings(tmp_lst.get());
				break;
			}
		}
	}
	//�P������
	else if (!kwd.IsEmpty()) {
		UnicodeString ptn = regex_sw? kwd : migemo_sw? usr_Migemo->GetRegExPtn(migemo_sw, kwd) : TRegEx::Escape(kwd);
		if (!ptn.IsEmpty()) {
			TMatchCollection mts = TRegEx::Matches(lbuf, ptn, opt);
			for (int i=0; i<mts.Count; i++) lst->Add(mts.Item[i].Value);
		}
	}

	return lst->Count;
}

//---------------------------------------------------------------------------
//��̋����\�� (URL�����L��)
//---------------------------------------------------------------------------
void EmphasisTextOut(
	UnicodeString s,		//�\��������
	TStringList *kw_lst,	//������̃��X�g
	TCanvas *cv,
	int &x,				//[i/o] �\��X�ʒu
	int y,				//[i]	�\��Y�ʒu
	bool case_sns,		//�召���������   (default = false)
	bool only_top,		//�擪�̌ꂾ������ (default = false)
	TColor fg,			//���������F (default = col_fgEmp)
	TColor bg)			//�����w�i�F (default = col_bgEmp)
{
	if (s.IsEmpty()) return;

	int s_len = s.Length();

	std::unique_ptr<TColor[]> FgCol(new TColor[s_len + 1]);
	std::unique_ptr<TColor[]> BgCol(new TColor[s_len + 1]);

	for (int i=0; i<=s_len; i++) {	//0 �� ���ݐF
		FgCol[i] = cv->Font->Color;
		BgCol[i] = cv->Brush->Color;
	}

	//URL����
	if (s.Pos("://")) {	//URL �������ԒZ�k�̂��߂̑O�`�F�b�N
		TMatch mt = TRegEx::Match(s, URL_MATCH_PTN);
		if (mt.Success) for (int j=0,p=mt.Index; j<mt.Length; j++,p++) FgCol[p] = col_URL;
	}

	//�L�[���[�h����
	if (kw_lst) {
		int ofs = 1;
		for (;;) {
			int p1 = 0;	//����������̖����ʒu + 1 �̍Ō��
			for (int i=0; i<kw_lst->Count; i++) {
				UnicodeString kwd = kw_lst->Strings[i];
				int p = case_sns ? PosEx(kwd, s, ofs) : PosEx(kwd.UpperCase(), s.UpperCase(), ofs);
				if (p>0) {
					int len = kwd.Length();
					for (int j=0,k=p; j<len; j++,k++) {
						FgCol[k] = fg;  BgCol[k] = bg;
					}
					p1 = std::max(p + len, p1);
				}
			}
			if (p1==0 || only_top) break;
			ofs = p1;
		}
	}

	//�R���g���[���R�[�h
	for (int i=1; i<=s_len; i++) {
		if (iscntrl(s[i]) && (s[i]!='\t')) {
			s[i] = (WideChar)((int)s[i] + 0x40);
			FgCol[i] = col_Ctrl;
		}
	}

	//������`��
	cv->Font->Color  = FgCol[1];
	cv->Brush->Color = BgCol[1];
	UnicodeString sbuf;
	for (int j=1; j<=s_len; j++) {
		if (FgCol[j]==cv->Font->Color && BgCol[j]==cv->Brush->Color) {
			sbuf.cat_sprintf(_T("%c"), s[j]);
		}
		else {
			TabCrTextOut(sbuf, cv, x, y, cv->Font->Color);
			sbuf = s[j];
			cv->Font->Color  = FgCol[j];
			cv->Brush->Color = BgCol[j];
		}
	}
	if (!sbuf.IsEmpty()) TabCrTextOut(sbuf, cv, x, y, cv->Font->Color);

	cv->Font->Color  = FgCol[0];
	cv->Brush->Color = BgCol[0];
}

//---------------------------------------------------------------------------
void EmphasisTextOut(
	UnicodeString s,
	UnicodeString kwd,	//������
	TCanvas *cv,
	int &x,				//[i/o] �\��X�ʒu
	int y,				//[i]	�\��Y�ʒu
	bool case_sns,		//�召���������   (default = false)
	bool only_top,		//�擪�̌ꂾ������ (default = false)
	TColor fg,			//���������F (default = col_fgEmp)
	TColor bg)			//�����w�i�F (default = col_bgEmp)
{
	std::unique_ptr<TStringList> kwd_lst(new TStringList());
	kwd_lst->Text = kwd;
	EmphasisTextOut(s, kwd_lst.get(), cv, x, y, case_sns, only_top, fg, bg);
}

//---------------------------------------------------------------------------
//�^�u�L���̕`�� (U+21C0 ����)
//---------------------------------------------------------------------------
void draw_TAB(TCanvas *cv, int x, int y, int w, int h)
{
	cv->TextOut(x, y, " ");
	cv->Pen->Style = psSolid;
	cv->Pen->Width = Scaled1;
	cv->Pen->Color = col_TAB;
	int w1 = w - 1;
	int wa = w1/2;
	int yc = y + h/2;
	cv->MoveTo(x, yc);  cv->LineTo(x + w1, yc);
	cv->MoveTo(x + w1 - wa - 1, yc - wa);	cv->LineTo(x + w1 - 1, yc);
}

//---------------------------------------------------------------------------
//���s�L���̕`�� (U+21B5 ����)
//---------------------------------------------------------------------------
void draw_CR(TCanvas *cv, int x, int y, int w, int h)
{
	cv->TextOut(x, y, " ");
	cv->Pen->Style = psSolid;
	cv->Pen->Width = Scaled1;
	cv->Pen->Color = col_CR;
	int w1 = w - 2;
	int wa = w1/2;
	int yc = y + (h * 5 / 7);
	cv->MoveTo(x + w1, y + h/3);	cv->LineTo(x + w1, yc);	cv->LineTo(x - 1, yc);
	cv->MoveTo(x + wa, yc - wa);	cv->LineTo(x, yc);
	cv->MoveTo(x + wa, yc + wa);	cv->LineTo(x, yc);
}

//---------------------------------------------------------------------------
//�^�u����s���܂ޕ�����̕`��
//---------------------------------------------------------------------------
void TabCrTextOut(
	UnicodeString s,	//�\��������
	TCanvas *cv,
	int &x,				//[i/o] �\��X�ʒu
	int y,				//[i]	�\��Y�ʒu
	TColor fg,			//�����F
	int max_x)			//�����ʒu (default = 0 : ����)
{
	if (max_x>0 && x>=max_x) return;

	s = ReplaceStr(s, "\r", "");
	int slen  = s.Length();
	int yh = y + cv->TextHeight(s);
	bool is_irreg = IsIrregularFont(cv->Font);

	UnicodeString sbuf;

	TMatchCollection mts = TRegEx::Matches(s, "[\t\n]");
	int p0 = 1;
	for (int i=0; i<mts.Count && p0<=slen; i++) {
		//������
		int p1 = mts.Item[i].Index;
		if (p1>p0) {
			sbuf = s.SubString(p0, p1 - p0);
			cv->Font->Color = fg;
			int w = get_TextWidth(cv, sbuf, is_irreg);
			cv->TextRect(Rect(x, y, x + w, yh), x, y, alt_yen_to(sbuf));
			x += w;
		}
		//�^�u/���s
		int w = get_TextWidth(cv, "W", is_irreg);
		if (USAME_TS(s.SubString(p1, 1), "\t"))
			draw_TAB(cv, x, y, w, cv->TextHeight("Q"));
		else
			draw_CR(cv, x, y, w, cv->TextHeight("Q"));
		x += w;
		if (max_x>0 && x>=max_x) break;
		p0 = p1 + 1;
	}

	//������
	if (p0<=slen && (max_x==0 || x<max_x)) {
		sbuf = s.SubString(p0, slen - p0 + 1);
		cv->Font->Color = fg;
		int w = get_TextWidth(cv, sbuf, is_irreg);
		cv->TextRect(Rect(x, y, x + w, yh), x, y, alt_yen_to(sbuf));
		x += w;
	}
}

//---------------------------------------------------------------------------
//�r���A�^�u���܂ލs�̕`��
//---------------------------------------------------------------------------
void RuledLnTextOut(
	UnicodeString s,		//�\��������
	TCanvas *cv,
	TRect &rc,				//�\���ʒu (rc.Left �X�V)
	TColor fg,				//�����F
	int tab_wd,				//�^�u��			(default = 8)
	TStringList *kw_lst)	//������̃��X�g	(default = NULL)
{
	bool is_irreg = IsIrregularFont(cv->Font);

	int slen = s.Length();
	int xp	 = rc.Left + 2;
	int yp	 = rc.Top + 1;
	UnicodeString sbuf;

	//�^�u�����v�Z
	TEXTMETRIC tm;
	::GetTextMetrics(cv->Handle , &tm);
	int w_tab = tab_wd * tm.tmAveCharWidth;

	TMatchCollection mts = TRegEx::Matches(s, "[\t�\����������������������]");
	int p0 = 1;
	for (int i=0; i<mts.Count && p0<=slen; i++) {
		//������
		int p1 = mts.Item[i].Index;
		if (p1>p0) {
			sbuf = s.SubString(p0, p1 - p0);
			cv->Font->Color = fg;
			EmphasisTextOut(sbuf, kw_lst, cv, xp, yp);
		}
		//�r��
		sbuf = s.SubString(p1, 1);
		cv->Pen->Style = psSolid;
		cv->Pen->Width = Scaled1;
		cv->Pen->Color = col_HR;
		int w  = get_TextWidth(cv, sbuf, is_irreg);
		int xc = xp + w/2;
		int yc = rc.Top + (rc.Bottom - rc.Top) / 2;
		if (starts_tchs(_T("������"), sbuf)) {
			cv->MoveTo(xc, rc.Top);  cv->LineTo(xc, rc.Bottom);	//�c��
			if (starts_tchs(_T("����"), sbuf)) {
				cv->MoveTo(xc, yc); cv->LineTo(USAME_TS(sbuf, "��")? xc + w : xp - 1, yc);
			}
		}
		else if (starts_tchs(_T("�\��������"), sbuf)) {
			cv->MoveTo(xp, yc);  cv->LineTo(xp + w, yc);	//����
			if (USAME_TS(sbuf, "��")) {
				cv->MoveTo(xc, rc.Top);  cv->LineTo(xc, rc.Bottom);
			}
			else if (starts_tchs(_T("����"), sbuf)) {
				cv->MoveTo(xc, yc); cv->LineTo(xc, USAME_TS(sbuf, "��")? rc.Bottom : rc.Top - 1);
			}
		}
		else if (starts_tchs(_T("����"), sbuf)) {
			cv->MoveTo(xc, rc.Bottom); cv->LineTo(xc, yc);
			cv->LineTo(USAME_TS(sbuf, "��")? xp + w : xp - 1, yc);
		}
		else if (starts_tchs(_T("����"), sbuf)) {
			cv->MoveTo(xc, rc.Top);  cv->LineTo(xc, yc);
			cv->LineTo(USAME_TS(sbuf, "��")? xp + w : xp - 1, yc);
		}
		//�^�u
		else if (USAME_TS(sbuf, "\t")) {
			int dt = (xp - (rc.Left + 2)) % w_tab;
			w = get_TextWidth(cv, "W", is_irreg);
			xp += (w_tab - dt - w);
			if (ShowTAB)
				draw_TAB(cv, xp, yp, w, cv->TextHeight("Q"));
			else
				cv->TextOut(xp, yp, " ");
		}

		xp += w;
		if (xp>=rc.Right) break;
		p0 = p1 + 1;
	}

	//������
	if (p0<=slen && xp<rc.Right) {
		sbuf = s.SubString(p0, slen - p0 + 1);
		cv->Font->Color = fg;
		EmphasisTextOut(sbuf, kw_lst, cv, xp, yp);
	}

	rc.Left = std::min(xp, (int)rc.Right);
}

//---------------------------------------------------------------------------
//�e�L�X�g�v���r���[��`��
//---------------------------------------------------------------------------
void PrvTextOut(
	TListBox *lp,			//���X�g�{�b�N�X
	int idx,				//�C���f�b�N�X
	TCanvas *cv, TRect &rc,
	TColor fg,				//�����F
	int tab_wd,				//�^�u��
	TStringList *kw_lst,	//������̃��X�g			(default = NULL)
	UnicodeString fnam,		//�R�����g����p�t�@�C����	(default = EmptyStr)
	bool en_mlt)			//�����s�R�����g�ɑΉ�		(default = true)
{
	UnicodeString s = lp->Items->Strings[idx];
	if (s.Length()>1024) s = s.SubString(1, 1024) + "�c";	//*** ����������

	//�R�����g�̔���
	int p = 0;
	if (EmpComment && fg!=col_fgSelItem && !s.IsEmpty()) {
		UnicodeString s0 = lp->Items->Strings[0];
		if ((USAME_TI(s0, ";[MenuFile]") || USAME_TI(s0, ";[ResultList]")) && s[1]==';')
			p = 1;
		else
			p = UserHighlight->GetCommentPos(fnam, lp, idx, en_mlt);
	}

	//�R�����g�L��
	if (p>0 && p<=s.Length()) {
		if (p>1) RuledLnTextOut(s.SubString(1, p - 1), cv, rc, fg, tab_wd, kw_lst);
		RuledLnTextOut(s.SubString(p, s.Length() - p + 1), cv, rc, col_Comment, tab_wd, kw_lst);
	}
	//�R�����g����
	else {
		if (test_FileExt(get_extension(fnam), _T(".dfm"))) {
			UnicodeString lbuf = s;
			if (lbuf.Pos('=')) {
				UnicodeString nbuf = split_tkn(lbuf, '=') + "= ";
				lbuf = TrimLeft(lbuf);
				if (starts_tchs(_T("\'#"), lbuf)) {
					s = nbuf + decode_TxtVal(lbuf, true);
				}
			}
			else {
				lbuf = TrimLeft(lbuf);
				if (starts_tchs(_T("\'#"), lbuf)) {
					UnicodeString end_s = remove_end_s(lbuf, ')')? ")" : "";
					s = StringOfChar(_T(' '), s.Length() - TrimLeft(s).Length())
							+ decode_TxtVal(lbuf, true) + end_s;
				}
			}
		}

		RuledLnTextOut(s, cv, rc, fg, tab_wd, kw_lst);
	}
}

//---------------------------------------------------------------------------
//�s�ԍ���`��
//---------------------------------------------------------------------------
void LineNoOut(TCanvas *cv, TRect &rc, UnicodeString l_str)
{
	//�w�i
	int w_ln = get_CharWidth(cv, 6, Scaled4);
	TRect l_rc = rc;
	cv->Brush->Color = col_bgLineNo;
	l_rc.Right = l_rc.Left + w_ln;
	cv->FillRect(l_rc);

	//�ԍ�
	if (!l_str.IsEmpty()) {
		cv->Font->Color = col_LineNo;
		cv->TextOut(
			l_rc.Left + w_ln - get_TextWidth(cv, l_str, IsIrregularFont(cv->Font)) - get_CharWidth(cv, 1)/2,
			l_rc.Top + 1,
			l_str);
	}

	//���E��
	cv->Pen->Style = psSolid;
	cv->Pen->Width = Scaled1;
	cv->Pen->Color = col_bdrLine;
	cv->MoveTo(l_rc.Right - 1, l_rc.Top);
	cv->LineTo(l_rc.Right - 1, l_rc.Bottom);
	rc.Left = l_rc.Right - 1;
}
//---------------------------------------------------------------------------
void LineNoOut(TCanvas *cv, TRect &rc, int l_no)
{
	UnicodeString l_str;
	if (l_no>0) l_str.sprintf(_T("%u"), l_no);
	LineNoOut(cv, rc, l_str);
}

//---------------------------------------------------------------------------
//�p�X����`��
//  \ �� ��؂蕶���A���x����
//---------------------------------------------------------------------------
void PathNameOut(
	UnicodeString s,		//�\��������
	TStringList *kw_lst,	//������̃��X�g (�s�v�Ȃ�NULL)
	TCanvas *cv,
	int &x, int y,			//�\���ʒu (x �͍X�V)
	bool mgn_sw)			//true = / �̑O��Ƀ}�[�W�������� (default = true)
{
	if (s.IsEmpty()) return;

	s = ReplaceStr(s, DirDelimiter, "\\");	//��؂肪�ϊ�����Ă������U \ �ɖ߂�

	int s_len = s.Length();
	std::unique_ptr<TColor[]> FgCol(new TColor[s_len + 1]);
	std::unique_ptr<TColor[]> BgCol(new TColor[s_len + 1]);

	for (int i=0; i<=s_len; i++) {	//0 �� ���ݐF
		FgCol[i] = cv->Font->Color;
		BgCol[i] = cv->Brush->Color;
	}

	//�L�[���[�h����
	if (kw_lst) {
		int ofs = 1;
		for (;;) {
			int p1 = 0;	//����������̖����ʒu + 1 �̍Ō��
			for (int i=0; i<kw_lst->Count; i++) {
				UnicodeString kwd = kw_lst->Strings[i];
				int p = PosEx(kwd.UpperCase(), s.UpperCase(), ofs);
				if (p>0) {
					int len = kwd.Length();
					for (int j=0,k=p; j<len; j++,k++) {
						FgCol[k] = col_fgEmp;  BgCol[k] = col_bgEmp;
					}
					p1 = std::max(p + len, p1);
				}
			}
			if (p1==0) break;
			ofs = p1;
		}
	}

	//������`��
	bool is_irreg = IsIrregularFont(cv->Font);
	TColor fg_sep = AdjustColor(cv->Font->Color, ADJCOL_FGLIST);
	int yh = y + cv->TextHeight("Q");
	int s_wd = get_TextWidth(cv, DirDelimiter, is_irreg);
	int s_mg = (s_wd>get_TextWidth(cv, "/", is_irreg))? 0 : s_wd/4;
	cv->Font->Color  = FgCol[1];
	cv->Brush->Color = BgCol[1];
	UnicodeString sbuf;
	for (int i=1; i<=s_len; i++) {
		WideChar c = s[i];
		if (FgCol[i]!=cv->Font->Color || BgCol[i]!=cv->Brush->Color || EndsStr('\\', sbuf)) {
			if (!sbuf.IsEmpty()) {
				bool flag = remove_end_s(sbuf, '\\');
				int w = get_TextWidth(cv, sbuf, is_irreg);
				if (w>0) {
					cv->TextRect(Rect(x, y, x + w, yh), x, y, sbuf);
					x += w;
				}
				if (flag) {
					if (cv->Font->Color!=col_fgEmp) cv->Font->Color = fg_sep;
					if (mgn_sw && i>3) x += s_mg;
					cv->TextRect(Rect(x, y, x + s_wd, yh), x, y, DirDelimiter);
					x += s_wd;
					if (mgn_sw && i>3) x += s_mg;
				}
			}
			sbuf = c;
			cv->Font->Color  = FgCol[i];
			cv->Brush->Color = BgCol[i];
		}
		else {
			sbuf.cat_sprintf(_T("%c"), c);
		}
	}

	if (!sbuf.IsEmpty()) {
		bool flag = remove_end_s(sbuf, '\\');
		int w = get_TextWidth(cv, sbuf, is_irreg);
		if (w>0) {
			cv->TextRect(Rect(x, y, x + w, yh), x, y, sbuf);
			x += w;
		}
		if (flag) {
			if (cv->Font->Color!=col_fgEmp) cv->Font->Color = fg_sep;
			if (mgn_sw) x += s_mg;
			cv->TextRect(Rect(x, y, x + s_wd, yh), x, y, DirDelimiter);
			x += s_wd;
			if (mgn_sw) x += s_mg;
		}
	}

	cv->Font->Color  = FgCol[0];
	cv->Brush->Color = BgCol[0];
}
//---------------------------------------------------------------------------
void PathNameOut(
	UnicodeString s,
	TCanvas *cv,
	int &x, int y,		//�\���ʒu (x �͍X�V)
	int max_w)			//������ (default = 0 : ������)
{
	if (s.IsEmpty()) return;

	if (max_w>0) s = get_MiniPathName(s, max_w, cv->Font);
	int s_len = s.Length();

	bool is_irreg = IsIrregularFont(cv->Font);
	TColor fg_org = cv->Font->Color;
	TColor fg_sep = AdjustColor(fg_org, ADJCOL_FGLIST);

	int yh = y + cv->TextHeight("Q");
	int s_wd = get_TextWidth(cv, DirDelimiter, is_irreg);
	UnicodeString sbuf;
	for (int i=1; i<=s_len; i++) {
		WideChar c = s[i];
		if (EndsStr('\\', sbuf)) {
			if (!sbuf.IsEmpty()) {
				bool flag = remove_end_s(sbuf, '\\');
				int w = get_TextWidth(cv, sbuf, is_irreg);
				if (w>0) {
					cv->Font->Color = fg_org;
					cv->TextRect(Rect(x, y, x + w, yh), x, y, sbuf);
					x += w;
				}
				if (flag) {
					cv->Font->Color = fg_sep;
					cv->TextRect(Rect(x, y, x + s_wd, yh), x, y, DirDelimiter);
					x += s_wd;
				}
			}
			sbuf = c;
		}
		else {
			sbuf.cat_sprintf(_T("%c"), c);
		}
	}

	if (!sbuf.IsEmpty()) {
		bool flag = remove_end_s(sbuf, '\\');
		int w = get_TextWidth(cv, sbuf, is_irreg);
		if (w>0) {
			cv->Font->Color = fg_org;
			cv->TextRect(Rect(x, y, x + w, yh), x, y, sbuf);
			x += w;
		}
		if (flag) {
			cv->Font->Color = fg_sep;
			cv->TextRect(Rect(x, y, x + s_wd, yh), x, y, DirDelimiter);
			x += s_wd;
		}
	}
}

//---------------------------------------------------------------------------
//RLO�����̌x�����܂ޕ�����̕`��
// "|" : RLO �x���̑�֕���
//---------------------------------------------------------------------------
void RloPipeTextOut(
	UnicodeString s,	//�\��������
	TCanvas *cv,
	int &x, int y,		//�\���ʒu (x �͍X�V)
	TColor fg)			//�����F
{
	if (s.IsEmpty()) return;

	bool is_irreg = IsIrregularFont(cv->Font);

	if (s.Pos('|')) {
		TStringDynArray slst = SplitString(s, "|");
		for (int i=0; i<slst.Length; i++) {
			cv->Font->Color = fg;
			cv->TextOut(x, y, slst[i]);
			x += get_TextWidth(cv, slst[i], is_irreg);
			if (i<slst.Length-1) {
				cv->Font->Color = col_Error;
				cv->TextOut(x, y, "|");
				x += get_TextWidth(cv, "|", is_irreg);
			}
		}
	}
	else {
		cv->Font->Color = fg;
		cv->TextOut(x, y, s);
		x += get_TextWidth(cv, s, is_irreg);
	}
}

//---------------------------------------------------------------------------
//�󔒋L�����܂ޕ�����̕`��
// �f�t�H���g�ł� ">"=���p��)�A"<"=�S�p�� "|"= RLO �ƌ��Ȃ��ĕ`��
// force_nrm=true �ł́A" "�A"�@" ���������������F�ŋ����`��
//---------------------------------------------------------------------------
void SpaceTextOut(
	UnicodeString s,	//�\��������
	TCanvas *cv,
	int &x, int y,		//�\���ʒu (x �͍X�V)
	TColor fg,			//�����F
	bool force_nrm)		//" " �� "�@" �������`��	(default = false)
{
	if (s.IsEmpty()) return;

	UnicodeString hs_ch = force_nrm? " " : ">";
	UnicodeString zs_ch = force_nrm? "�@" : "<";
	if ((ShowSpace || force_nrm) && (s.Pos(hs_ch) || s.Pos(zs_ch))) {
		bool is_irreg = IsIrregularFont(cv->Font);
		int w1 = Scaled1;
		int w2 = Scaled2;
		int w3 = Scaled3;
		int hs_wd = get_TextWidth(cv, hs_ch, is_irreg);
		int zs_wd = std::min(abs(cv->Font->Height), hs_wd * 2);
		cv->Pen->Width = w1;
		cv->Pen->Style = psSolid;
		cv->Pen->Color = force_nrm? AdjustColor(fg, ADJCOL_FGLIST) : col_fgSpace;
		for (;;) {
			int p_z = s.Pos(zs_ch);
			int p_h = s.Pos(hs_ch);
			int p = (p_h>0 && p_z>0)? std::min(p_h, p_z) : (p_h>0)? p_h : p_z;
			if (p==0) break;

			UnicodeString tmp = s.SubString(1, p - 1);
			bool is_zs = SameStr(s.SubString(p, 1), zs_ch);
			s.Delete(1, p);
			//����
			if (force_nrm) out_TextEx(cv, x, y, tmp, fg); else RloPipeTextOut(tmp, cv, x, y, fg);

			//�S�p��
			if (is_zs) {
				int y0 = y + w3;
				int y1 = y + abs(cv->Font->Height);
				cv->MoveTo(x + w3, y0);	cv->LineTo(x + zs_wd - w3, y0);
				cv->MoveTo(x + w3, y1);	cv->LineTo(x + zs_wd - w3, y1);
				cv->MoveTo(x + w1, y0 + w2);			cv->LineTo(x + w1, y1 - w1);
				cv->MoveTo(x + zs_wd - w2, y0 + w2);	cv->LineTo(x + zs_wd - w2, y1 - w1);
				x += zs_wd;
			}
			//���p��
			else {
				int y0 = y + abs(cv->Font->Height)*2/3;
				int y1 = y + abs(cv->Font->Height);
				cv->MoveTo(x + w1, y0);
				cv->LineTo(x + w1, y1);
				cv->LineTo(x + hs_wd - w2, y1);
				cv->LineTo(x + hs_wd - w2, y0 - w1);
				x += hs_wd;
			}
		}

		if (force_nrm) out_TextEx(cv, x, y, s, fg); else RloPipeTextOut(s, cv, x, y, fg);
	}
	else {
		if (force_nrm) out_TextEx(cv, x, y, s, fg); else RloPipeTextOut(s, cv, x, y, fg);
	}
}

//---------------------------------------------------------------------------
//�t�@�C�������� RLO ���x�������\��
//---------------------------------------------------------------------------
void Emphasis_RLO_info(
	UnicodeString fnam,	//�t�@�C����
	TCanvas *cv,
	int xp, int yp)		//�擪�ʒu
{
	//RLO ����
	if (fnam.Pos(L"\u202e")) {
		EmphasisTextOut(warn_filename_RLO(fnam), "<RLO>", cv, xp, yp, false, false, col_Error, cv->Brush->Color);

		//���ۂ̕\����
		bool is_irreg	= IsIrregularFont(cv->Font);
		cv->Font->Color = AdjustColor(cv->Font->Color, ADJCOL_FGLIST);
		cv->TextOut(xp, yp, "  (");
		xp += get_TextWidth(cv, "  (", is_irreg);
		cv->TextOut(xp, yp, fnam);
		xp += get_TextWidth(cv, fnam, is_irreg);
		cv->TextOut(xp, yp, ")");
	}
	//�ʏ�
	else cv->TextOut(xp, yp, fnam);
}

//---------------------------------------------------------------------------
//�t�@�C������`��
//---------------------------------------------------------------------------
void FileNameOut(
	TCanvas *cv, TRect &rc,
	UnicodeString fnam,		//�t�@�C����
	bool use_fgsel,			//�I��F���g�p
	bool to_slash,			//\ ����؂蕶���ɒu��	(default = false)
	TStringList *kw_lst)	//������̃��X�g		(default = NULL)
{
	UnicodeString dnam = ExtractFilePath(fnam);
	//�f�B���N�g����
	if (!dnam.IsEmpty()) {
		TColor fg = use_fgsel? col_fgSelItem : col_Folder;
		fnam = ExtractFileName(fnam);
		if (to_slash) {
			int xp = rc.Left + 2;
			int yp = rc.Top + 1;
			cv->Font->Color = fg;
			PathNameOut(dnam, kw_lst, cv, xp, yp);
			rc.Left = xp;
		}
		else {
			RuledLnTextOut(dnam, cv, rc, fg, 8, kw_lst);
		}
	}

	//�t�@�C����
	RuledLnTextOut(fnam, cv, rc, use_fgsel? col_fgSelItem : get_ExtColor(get_extension(fnam)), 8, kw_lst);
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̃p�l����`��
//---------------------------------------------------------------------------
void draw_SttBarPanel(TStatusBar *sp, TStatusPanel *pp, TRect rc)
{
	TCanvas *cv = sp->Canvas;
	cv->Font->Assign(sp->Font);
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(rc);

	cv->Font->Color = col_fgSttBar;
	cv->TextOut(rc.Left + Scaled4, rc.Top, pp->Text);
}

//---------------------------------------------------------------------------
//�X�e�[�^�X�o�[�̔w�i��`��
//---------------------------------------------------------------------------
bool draw_SttBarBg(TStatusBar *sp, TMessage &msg)
{
	if (col_bgSttBar==scl_BtnFace) return false;

	std::unique_ptr<TCanvas> cv(new TCanvas());
	cv->Handle = (HDC)msg.WParam;
	cv->Brush->Color = col_bgSttBar;
	cv->FillRect(sp->ClientRect);
	//��[���E
	cv->Pen->Color = SelectWorB(col_bgSttBar, 0.25);
	cv->Pen->Width = Scaled1;
	cv->Pen->Style = psSolid;
	cv->MoveTo(0, 0);
	cv->LineTo(sp->ClientWidth, 0);

	msg.Result = 1;
	return true;
}

//---------------------------------------------------------------------------
//���w�b�_�̔w�i��`��
//---------------------------------------------------------------------------
bool draw_InfHdrBg(TStatusBar *sp, TMessage &msg)
{
	if (col_bgInfHdr==scl_BtnFace) return false;

	std::unique_ptr<TCanvas> cv(new TCanvas());
	cv->Handle = (HDC)msg.WParam;
	cv->Brush->Color = col_bgInfHdr;
	cv->FillRect(sp->ClientRect);

	msg.Result = 1;
	return true;
}

//---------------------------------------------------------------------------
//�w�b�_�[�Z�N�V�����̕`��(�\�[�g�����}�[�N�t)
//---------------------------------------------------------------------------
void draw_SortHeader(
	THeaderControl *hp, THeaderSection *sp,
	TRect rc,
	int mk_mode, 		//0:����/ 1:��/ -1:��
	bool use_syscol)	//�V�X�e���F���g�p		(default = false : �ݒ�F)
{
	TCanvas *cv = hp->Canvas;
	cv->Font->Assign(hp->Font);
	int xp = rc.Left + 4;
	int yp = rc.Top + get_TopMargin(cv) + 1;

	cv->Brush->Color = use_syscol? scl_BtnFace : col_bgListHdr;
	cv->Font->Color  = use_syscol? scl_BtnText : col_fgListHdr;

	//�w�i
	cv->FillRect(rc);
	//�e�L�X�g
	if (rc.Width()>8) cv->TextOut(xp, yp, sp->Text);	//$$$

	//��؂��
	if (sp->Index < hp->Sections->Count-1) {
		cv->Pen->Style = psSolid;
		cv->Pen->Width = Scaled1;
		cv->Pen->Color = SelectWorB(cv->Brush->Color, 0.25);
		cv->MoveTo(rc.Right - 2, rc.Top);  cv->LineTo(rc.Right - 2, rc.Bottom);
	}

	//�}�[�N
	if (mk_mode!=0) {
		xp += cv->TextWidth(sp->Text) + 4;
		draw_SortMark(cv, xp, yp + 1, (mk_mode==1), cv->Font->Color);
	}
}

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�̃��C���J�[�\����`��
//---------------------------------------------------------------------------
void draw_ListCursor(TListBox *lp, TRect &Rect, int Index, TOwnerDrawState State)
{
	bool is_focused = State.Contains(odFocused);
	if (Index==lp->ItemIndex && is_focused) {
		int lw = std::max(CursorWidth, 1);
		int yp = Rect.Bottom - lw;
		draw_Line(lp->Canvas, Rect.Left, yp, Rect.Right, yp, lw, col_Cursor, psSolid);
	}
	if (is_focused) lp->Canvas->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------
void draw_ListCursor2(TListBox *lp, TRect &Rect, int Index, TOwnerDrawState State)
{
	//�J�[�\��
	bool is_focused = State.Contains(odFocused);
	if (Index==lp->ItemIndex && (is_focused || !lp->Focused())) {
		int lw = is_focused? std::max(CursorWidth, 1) : 1;
		int yp = Rect.Bottom - lw;
		draw_Line(lp->Canvas, Rect.Left, yp, Rect.Right, yp, lw, col_Cursor, is_focused? psSolid : psDot);
	}
	if (is_focused) lp->Canvas->DrawFocusRect(Rect);
}
//---------------------------------------------------------------------------
//�O���b�h�̃��C���J�[�\����`��
//---------------------------------------------------------------------------
void draw_GridCursor(TStringGrid *gp, TRect &Rect, int ARow, TGridDrawState State)
{
	if (gp->Row == ARow) {
		int lw = gp->Focused()? std::max(CursorWidth, 1) : 1;
		int yp = Rect.Bottom - lw;
		draw_Line(gp->Canvas, Rect.Left, yp, Rect.Right + 1, yp, lw, col_Cursor, gp->Focused()? psSolid : psDot);
	}
}

//---------------------------------------------------------------------------
//�摜�����O���b�h��`��
//---------------------------------------------------------------------------
void draw_ImgGrid(TCanvas *cv, Graphics::TBitmap *bmp)
{
	//�����O���b�h
	cv->Pen->Style = psSolid;
	cv->Pen->Width = Scaled1;
	cv->Pen->Color = col_ImgGrid;
	//��
	for (int i=1; i<ImgGridHorzN; i++) {
		int x = bmp->Width/ImgGridHorzN * i;
		cv->MoveTo(x, 0);  cv->LineTo(x, bmp->Height);
	}
	//�c
	for (int i=1; i<ImgGridVertN; i++) {
		int y = bmp->Height/ImgGridVertN * i;
		cv->MoveTo(0, y);  cv->LineTo(bmp->Width, y);
	}
	//�O�`��
	cv->Brush->Color = col_ImgGrid;
	cv->FrameRect(Rect(0, 0, bmp->Width, bmp->Height));
}

//---------------------------------------------------------------------------
//�o�[�O���t�̕`��
//  0�`50% �΁��� 100%
//---------------------------------------------------------------------------
void draw_BarGraph(TCanvas *cv, TRect rc, double r)
{
	TColor      org_col = cv->Brush->Color;
	TBrushStyle org_stl = cv->Brush->Style;

	//�w�i
	cv->Brush->Style = bsSolid;
	cv->Brush->Color = col_bgPrgBar;
	cv->FillRect(rc);

	//�o�[
	rc.Right = rc.Left + rc.Width() * r;
	cv->Brush->Color =
		(r<=0.5)? clLime : TColor(RGB(std::min((int)((r - 0.5)*1020), 255), std::min((int)((1.0 - r)*1020), 255), 0));
	cv->FillRect(rc);

	cv->Brush->Color = org_col;
	cv->Brush->Style = org_stl;
}

//---------------------------------------------------------------------------
//�i���o�[�̕`��
//---------------------------------------------------------------------------
void draw_ProgressBar(TCanvas *cv, TRect rc, double r)
{
	TRect rc_f = rc;
	TRect rc_b = rc;
	int p_pos  = (int)(rc_b.Width() * r);
	rc_f.Right = rc_f.Left + p_pos;
	rc_b.Left  = rc_f.Right;

	cv->Brush->Color = col_fgPrgBar;
	cv->FillRect(rc_f);
	cv->Brush->Color = col_bgPrgBar;
	cv->FillRect(rc_b);
}

//---------------------------------------------------------------------------
//�����^�u�̕`��
//���N���V�b�N�ȊO�̃e�[�}�ŉ����^�u���������`�悳��Ȃ��s��̑΍�
//---------------------------------------------------------------------------
void draw_BottomTab(TCustomTabControl *Control, int idx, const TRect rc, bool active)
{
	TTabControl *tp = (TTabControl*)Control;
	TCanvas *cv = tp->Canvas;
	//�w�i
	cv->Brush->Color = active? col_bgOptTab : scl_BtnFace;
	cv->FillRect(rc);
	//����
	UnicodeString titstr = tp->Tabs->Strings[idx];
	cv->Font->Color = active? col_fgOptTab : scl_BtnText;
	cv->Font->Style = active? (cv->Font->Style << fsBold) : (cv->Font->Style >> fsBold);
	cv->TextOut(
		rc.Left + (rc.Width() - cv->TextWidth(titstr))/2,
		rc.Bottom - cv->TextHeight(titstr) - 4,
		titstr);
}

//---------------------------------------------------------------------------
//�e�L�X�g���t�@�C���ɕۑ�
//---------------------------------------------------------------------------
bool saveto_TextFile(UnicodeString fnam, TStrings *lst, TEncoding *enc)
{
	try {
		if (enc)
			lst->SaveToFile(fnam, enc);
		else
			lst->SaveToFile(fnam);
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
bool saveto_TextFile(UnicodeString fnam, TStrings *lst, int enc_idx)
{
	int code_page = 932;
	if (enc_idx>=0 && enc_idx<MAX_SAVE_CODEPAGES) {
		code_page = SaveCodePages[enc_idx].page;
		if (SameText(SaveCodePages[enc_idx].name, "UTF-8N")) lst->WriteBOM = false;
	}

	std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
	return saveto_TextFile(fnam, lst, enc.get());
}

//---------------------------------------------------------------------------
bool saveto_TextUTF8(UnicodeString fnam, TStrings *lst)
{
	return saveto_TextFile(fnam, lst, TEncoding::UTF8);
}

//---------------------------------------------------------------------------
//�����R�[�h�𔻒�(�w��)���ăe�L�X�g�t�@�C����ǂݍ���
//  �߂�l: �R�[�h�y�[�W
//  �����L��̏ꍇ�A�ŏI�s�� TXLIMIT_MARK
//---------------------------------------------------------------------------
int load_text_ex(
	UnicodeString fnam,			//�t�@�C����
	TStringList *f_buf,			//�擾�o�b�t�@
	int code_page,				//�R�[�h�y�[�W		(default = 0 : �擾)
	int limit_size,				//�ő�Ǎ��T�C�Y	(default = 0 : ������)
	bool force_txt,				//NULL�������󔒂ɒu�����ċ����Ǎ� �T�C�Y�������̂ݗL�� (default = false)
	UnicodeString *line_brk,	//[o] ���s�R�[�h	(default = NULL);
	bool *has_bom,				//[o] BOM�L��		(default = NULL)
	double *ave_lsz)			//[o] ���ύs�T�C�Y	(default = NULL)
{
	GlobalErrMsg = EmptyStr;
	f_buf->Clear();

	try {
		//�R�[�h�y�[�W���擾
		if (code_page==0) code_page = get_FileCodePage(fnam, NULL, has_bom);

		//�������X�g���[���ɃT�C�Y�������ēǂݍ���
		if (limit_size>0) {
			std::unique_ptr<TFileStream>   fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
			std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
			ms->CopyFrom(fs.get(), std::min<__int64>(fs->Size, limit_size));

			//UTF8�̏ꍇ�A�����̓r���Ő؂�Ă���Ɠǂݍ��݂Ɏ��s���邽�߁A�[���o�C�g��؂�l�߂�
			if (code_page==65001 && ms->Size<fs->Size) {
				BYTE mbuf[8];
				ms->Seek(-8, soFromEnd);
				if (ms->Read(mbuf, 8)==8) {
					int n = 0;
					for (int i=7; i>=0; i--,n++) {
						if ((mbuf[i] & 0x80)==0x00) break;	//1�o�C�g����
						if ((mbuf[i] & 0xc0)==0xc0) {		//�擪�o�C�g
							n++; break;
						}
					}
					if (n>0) ms->Size = ms->Size - n;
				}
			}

			//�����\���̂��߂� NULL�������󔒂ɒu��
			if (force_txt) {
				ms->Seek(0, soFromBeginning);
				//UTF16
				if (code_page==1200 || code_page==1201) {
					WORD d;
					while (ms->Position < ms->Size) {
						if (ms->Read(&d, 2) < 2) break;
						if (d==0) {
							d = (code_page==1200)? 0x0020 : 0x2000;
							ms->Seek((__int64)-2, soCurrent);
							ms->Write(&d, 2);
						}
					}
				}
				//���̑�
				else {
					BYTE d;
					while (ms->Position < ms->Size) {
						if (ms->Read(&d, 1) < 1) break;
						if (d==0) {
							d = 0x20;
							ms->Seek((__int64)-1, soCurrent);
							ms->Write(&d, 1);
						}
					}
				}
			}

			//���s�R�[�h
			if (line_brk) *line_brk = force_txt? EmptyStr : get_StreamLineBreak(ms.get(), code_page);

			//�e�L�X�g�Ƃ��ēǂݍ���
			ms->Seek(0, soFromBeginning);
			if (code_page>0) {
				std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
				f_buf->LoadFromStream(ms.get(), enc.get());
			}
			else {
				f_buf->LoadFromStream(ms.get());
			}

			if (ms->Size<fs->Size) {
				if (f_buf->Count>0) {
					f_buf->Delete(f_buf->Count - 1);
					f_buf->Add(TXLIMIT_MARK);	//�s�������}�[�N
				}
			}

			if (ave_lsz && f_buf->Count>0) *ave_lsz = 1.0 * ms->Size / f_buf->Count;
		}
		//���ׂēǂݍ���
		else {
			//���s�R�[�h
			if (!force_txt && line_brk) {
				std::unique_ptr<TFileStream>   fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
				std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
				ms->CopyFrom(fs.get(), std::min<__int64>(fs->Size, TXT_DETECT_SIZE));
				*line_brk = get_StreamLineBreak(ms.get(), code_page);
			}

			//�ǂݍ���
			if (code_page>0) {
				std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
				f_buf->LoadFromFile(fnam, enc.get());
			}
			else {
				f_buf->LoadFromFile(fnam);
			}
		}
	}
	catch (...) {
		GlobalErrMsg = SysErrorMessage(GetLastError());
		f_buf->Clear();
		code_page = 0;
	}

	return code_page;
}

//---------------------------------------------------------------------------
//�e�L�X�g�t�@�C���̖������w��s�����ǂݍ���
//  �߂�l: �R�[�h�y�[�W	0 = �G���[
//---------------------------------------------------------------------------
int load_text_tail(
	UnicodeString fnam,
	TStringList *f_buf,
	int code_page,				//�R�[�h�y�[�W		(default = 0 : �擾)
	int limit_ln,				//�ő�Ǎ��s��		(default = 100)
	bool reverse,				//�t���\��			(default = false);
	UnicodeString *line_brk)	//[o] ���s�R�[�h	(default = NULL);
{
	GlobalErrMsg = EmptyStr;
	f_buf->Clear();
	if (limit_ln<=0) return 0;

	if (code_page==0) code_page = get_FileCodePage(fnam, line_brk);
	if (code_page==0) return 0;

	try {
		bool is_BE	= (code_page==1201);
		int  ch_sz	= (code_page==1200 || code_page==1201)? 2 : 1;
		int  ch_sz2 = ch_sz*2;
		BYTE cbuf[2]; cbuf[1] = 0;

		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		fs->Seek(-ch_sz, soFromEnd);
		int lcnt = 0;
		for (;;) {
			if (fs->Read(cbuf, ch_sz)!=ch_sz) break;
			unsigned short c = is_BE? ((cbuf[0] << 8) | cbuf[1]) : ((cbuf[1] << 8) | cbuf[0]);
			if (c==0x000d) {		//CR
				if (++lcnt>limit_ln) break;
				if (fs->Seek(-ch_sz2, soFromCurrent)==0) break;
			}
			else if (c==0x000a) {	//LF
				if (++lcnt>limit_ln) break;
				if (fs->Seek(-ch_sz2, soFromCurrent)==0) break;
				if (fs->Read(cbuf, ch_sz)!=ch_sz) break;
				unsigned short c = is_BE? ((cbuf[0] << 8) | cbuf[1]) : ((cbuf[1] << 8) | cbuf[0]);
				if (fs->Seek(-((c==0x000d)? ch_sz2 : ch_sz), soFromCurrent)==0) break;
			}
			else {
				if (lcnt==0 && c!=0x001a) lcnt++;
				if (fs->Seek(-ch_sz*2, soFromCurrent)==0) break;
			}
		}

		//�e�L�X�g�Ƃ��ēǂݍ���
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
		f_buf->LoadFromStream(fs.get(), enc.get());

		//�t��
		if (reverse) {
			std::unique_ptr<TStringList> tbuf(new TStringList());
			for (int i=f_buf->Count-1; i>=0; i--) tbuf->Add(f_buf->Strings[i]);
			f_buf->Assign(tbuf.get());
		}
	}
	catch (...) {
		GlobalErrMsg = SysErrorMessage(GetLastError());
		f_buf->Clear();
		code_page = 0;
	}

	return code_page;
}
//---------------------------------------------------------------------------
//�e�L�X�g�t�@�C���̖����̒ǉ����e���`�F�b�N
//---------------------------------------------------------------------------
bool check_text_tail(UnicodeString fnam, int &code_page, int size, UnicodeString kwd, TStringList *lst)
{
	if (code_page==0) code_page = get_FileCodePage(fnam);
	if (code_page==0) return false;

	try {
		bool is_BE = (code_page==1201);
		int  ch_sz = (code_page==1200 || code_page==1201)? 2 : 1;
		BYTE cbuf[2]; cbuf[1] = 0;

		std::unique_ptr<TFileStream> fs(new TFileStream(fnam, fmOpenRead | fmShareDenyNone));
		if (size<fs->Size) {
			fs->Seek(-(size + ch_sz), soFromEnd);
			for (;;) {
				if (fs->Read(cbuf, ch_sz)!=ch_sz) break;
				unsigned short c = is_BE? ((cbuf[0] << 8) | cbuf[1]) : ((cbuf[1] << 8) | cbuf[0]);
				if (c==0x000d || c==0x000a) break;
				if (fs->Seek(-ch_sz*2, soFromCurrent)==0) break;
			}
		}

		//�e�L�X�g�Ƃ��ēǂݍ���
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(code_page));
		lst->LoadFromStream(fs.get(), enc.get());
		//�L�[���[�h�ōi�荞��
		if (!kwd.IsEmpty()) {
			int i=0;
			//���K�\��
			if (is_regex_slash(kwd)) {
				kwd = exclude_top_end(kwd);
				TRegExOptions opt; opt << roIgnoreCase;
				while (i < lst->Count) if (TRegEx::IsMatch(lst->Strings[i], kwd, opt)) i++; else lst->Delete(i);
			}
			//�ʏ�
			else {
				while (i < lst->Count) if (ContainsText(lst->Strings[i], kwd)) i++; else lst->Delete(i);
			}
		}
		return (lst->Count>0);
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//XML�̐錾�����擾����
//---------------------------------------------------------------------------
void get_xml_inf(
	UnicodeString fnam,		//�t�@�C���� or "<?xml " ���܂ލs���e
	TStringList *lst)		//�擾�o�b�t�@(���O=�l �̌`���Ŋi�[);
{
	UnicodeString lbuf = fnam;
	if (!ContainsText(lbuf, "<?xml ")) lbuf = get_top_line(fnam);
	if (ContainsText(lbuf, "<?xml ") && ContainsText(lbuf, "?>")) {
		lbuf = Trim(get_tkn_m(lbuf, _T("<?xml "), _T("?>")));
		while (!lbuf.IsEmpty()) {
			UnicodeString vstr = exclude_quot(get_tkn_m(lbuf, '=', ' '));
			if (vstr.IsEmpty()) break;
			UnicodeString nstr = ReplaceStr(get_tkn(lbuf, '='), "version", "XML version");
			lst->Add(UnicodeString().sprintf(_T("%*s: %s"), FPRP_NAM_WD, nstr.c_str(), vstr.c_str()));
			lbuf = get_tkn_r(lbuf, ' ');
		}
	}
}

//---------------------------------------------------------------------------
//autorun.inf ����A�C�R���t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_autorun_ico(UnicodeString fnam)
{
	if (!file_exists(fnam)) return EmptyStr;

	UnicodeString ico_nam;
	std::unique_ptr<TStringList> fbuf(new TStringList());
	if (load_text_ex(fnam, fbuf.get())!=0) {
		bool s_flag = false;
		for (int i=0; i<fbuf->Count; i++) {
			UnicodeString lbuf = Trim(fbuf->Strings[i]);
			if (USAME_TI(lbuf, "[autorun]")) {
				s_flag = true;
			}
			else if (s_flag && StartsText("icon=", lbuf)) {
				ico_nam = ExtractFilePath(fnam) + get_tkn_r(lbuf, '=');  break;
			}
		}
		if (!file_exists(ico_nam)) ico_nam = EmptyStr;
	}
	return ico_nam;
}

//---------------------------------------------------------------------------
//�e�L�X�g�G�f�B�^�ŊJ��
//---------------------------------------------------------------------------
bool open_by_TextEditor(UnicodeString fnam, int lno)
{
	try {
		GlobalErrMsg = EmptyStr;
		if (!file_exists(fnam))   SysErrAbort(ERROR_FILE_NOT_FOUND);
		UnicodeString editor = get_actual_path(TextEditor);
		if (!file_exists(editor)) UserAbort(USTR_AppNotFound);

		//�N���p�����[�^�̍\��
		UnicodeString p_fnam = add_quot_if_spc(fnam);
		UnicodeString prmstr;
		if (lno>0) {
			if (!TextEditorFrmt.IsEmpty()) {
				prmstr = TextEditorFrmt;
				prmstr = ReplaceStr(prmstr, "$F", p_fnam);
				prmstr = ReplaceStr(prmstr, "$L", IntToStr(lno));
			}
			else {
				prmstr = p_fnam;
			}
		}
		else {
			prmstr = p_fnam;
		}

		if (!Execute_ex(editor, prmstr)) UserAbort(USTR_FaildExec);
		if (AddToRecent) AddToRecentFile(fnam);

		//�ҏW�������X�V
		if (!ContainsText(fnam, TempPathA)) add_TextEditHistory(fnam);

		return true;
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		return false;
	}
}

//---------------------------------------------------------------------------
//�O���e�L�X�g�r���A�[�ŊJ��
//---------------------------------------------------------------------------
bool open_by_ExtTextViewer(UnicodeString fnam, int lno)
{
	try {
		GlobalErrMsg = EmptyStr;
		if (!file_exists(fnam)) SysErrAbort(ERROR_FILE_NOT_FOUND);
		UnicodeString viewer = get_actual_path(ExtTxViewer);
		if (!file_exists(viewer)) UserAbort(USTR_AppNotFound);

		//�N���p�����[�^�̍\��
		UnicodeString p_fnam = add_quot_if_spc(fnam);
		UnicodeString prmstr;
		if (lno>0) {
			if (!ExtTxViewerFrmt.IsEmpty()) {
				prmstr = ExtTxViewerFrmt;
				prmstr = ReplaceStr(prmstr, "$F", p_fnam);
				prmstr = ReplaceStr(prmstr, "$L", IntToStr(lno));
			}
			else {
				prmstr = p_fnam;
			}
		}
		else {
			prmstr = p_fnam;
		}

		if (!Execute_ex(viewer, prmstr)) UserAbort(USTR_FaildExec);

		return true;
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		return false;
	}
}

//---------------------------------------------------------------------------
//��������t�@�C�����ƍs�ԍ��ɕ���
//---------------------------------------------------------------------------
bool divide_FileName_LineNo(
	UnicodeString &fnam,	//[i]   �t�@�C�������܂ޕ�����(ctags ����)
							//[o]   �p�X�t���t�@�C����
	int &lno,				//[i/o] �s�ԍ�
	UnicodeString rnam,		//[i]   ��f�t�H���g�� (default = EmptyStr/ ExePath)
	int pos)				//[i]   �擾�J�n�ʒu	 (default = 0)
{
	GlobalErrMsg = EmptyStr;

	rnam = ExtractFilePath(rnam);
	if (rnam.IsEmpty()) rnam = ExePath;

	do {
		fnam = Trim(fnam);
		if (fnam.IsEmpty()) break;

		//html �����N
		TRegExOptions opt; opt << roIgnoreCase;
		TMatchCollection mts = TRegEx::Matches(fnam, "\\bhref=\".*?\"", opt);
		bool found = false;
		for (int i=0; i<mts.Count && !found; i++) {
			int p1 = mts.Item[i].Index + mts.Item[i].Length;
			if (mts.Count==1 || pos<p1) {
				fnam = exclude_quot(Trim(get_tkn_r(mts.Item[i].Value, '=')));
				fnam = get_tkn(fnam, '#');
				remove_top_text(fnam, _T("file:///"));
				fnam = slash_to_yen(fnam);
				lno = 1; found = true;
			}
		}
		if (found) break;

		//�}�[�N�_�E��
		mts = TRegEx::Matches(fnam, "\\[.*\\]\\([^*?<>]+\\.\\w+(#\\w+)?\\)", opt);
		found = false;
		for (int i=0; i<mts.Count && !found; i++) {
			int p1 = mts.Item[i].Index + mts.Item[i].Length;
			if (mts.Count==1 || pos<p1) {
				fnam = get_in_paren(mts.Item[i].Value);
				fnam = get_tkn(fnam, '#');
				fnam = slash_to_yen(fnam);
				fnam = to_absolute_name(fnam, rnam);
				lno = 1; found = true;
			}
		}
		if (found) break;

		//���[�J���t�@�C��
		mts = TRegEx::Matches(fnam, LOCAL_FILE_PTN);
		for (int i=0; i<mts.Count && !found; i++) {
			int p1 = mts.Item[i].Index + mts.Item[i].Length;
			if (mts.Count==1 || pos<p1) {
				fnam = exclude_quot(Trim(get_tkn_r(mts.Item[i].Value, _T("file:///"))));
				fnam = get_tkn(fnam, '#');
				fnam = slash_to_yen(fnam);
				remove_end_s(fnam, ')');
				lno = 1; found = true;
			}
		}
		if (found) break;

		//c �C���N���[�h
		if (ContainsStr(fnam, "#include")) {
			fnam = exclude_quot(Trim(get_tkn_r(fnam, _T("#include"))));
			if (ContainsStr(fnam, "<")) fnam = get_tkn_m(fnam, '<', '>');
			lno = 1;	break;
		}

		//ctags �t�H�[�}�b�g
		TMatch mt = TRegEx::Match(fnam, "^.+\\t.+\\.\\w+\\t(\\d+|/.+/)", opt);
		if (mt.Success) {
			UnicodeString lbuf = get_post_tab(get_tkn(fnam, ";\""));
			UnicodeString nptn = get_post_tab(lbuf);
			fnam = get_pre_tab(lbuf);
			//�p�^�[��
			if (nptn.Length()>=2
				&& ((StartsStr('/', nptn) && EndsStr('/', nptn)) || (StartsStr('?', nptn) && EndsStr('?', nptn))))
			{
				lno = 1;
				nptn = exclude_top_end(nptn);
				nptn = ReplaceStr(nptn, "\\/", "/");
				bool p_top = remove_top_s(nptn, '^');
				bool p_end = remove_end_s(nptn, '$');
				UnicodeString nkwd = nptn;
				nptn = TRegEx::Escape(nptn);
				if (p_top) nptn.Insert("^", 1);
				if (p_end) nptn += "$";
				if (chk_RegExPtn(nptn)) {
					fnam = to_absolute_name(fnam, rnam);
					if (file_exists(fnam)) {
						std::unique_ptr<TStringList> fbuf(new TStringList());
						if (load_text_ex(fnam, fbuf.get())!=0) {
							TRegExOptions opt;
							for (int i=0; i<fbuf->Count; i++) {
								UnicodeString lbuf = fbuf->Strings[i];
								if (lbuf.Pos(nkwd)==0) continue;
								if (TRegEx::IsMatch(fbuf->Strings[i], nptn, opt)) {
									lno = i+1;	break;
								}
							}
						}
					}
				}
			}
			//�s�ԍ�
			else {
				lno = nptn.ToIntDef(1);
			}
			break;
		}

		//���
		UnicodeString lstr;
		if (remove_top_s(fnam, '\"')) {
			lstr = Trim(get_tkn_r(fnam, '\"'));
			fnam = get_tkn(fnam, '\"');
		}
		else {
			int p;
			int p0 = fnam.Pos(' ');
			int p1 = fnam.Pos('(');
			if (p0==0) p = p1; else if (p1==0) p = p0; else p = std::min(p0, p1);
			if (p>0) {
				lstr = fnam.SubString(p + 1, fnam.Length() - p);
				fnam = fnam.SubString(1, p - 1);
			}
		}

		//�s�ԍ����擾
		int p;
		int p0 = lstr.Pos(':');
		int p1 = lstr.Pos(')');
		if (p0==0) p = p1; else if (p1==0) p = p0; else p = std::min(p0, p1);
		if (p>0) {
			lstr = lstr.SubString(1, p - 1);
			if (ContainsStr(lstr, "(")) lstr = get_tkn_r(lstr, '(');
			lstr = Trim(lstr);
		}
		lno = lstr.ToIntDef(1);
	} while (0);

	fnam = to_absolute_name(fnam, rnam);

	if (fnam.IsEmpty())
		GlobalErrMsg = "�W�����v�悪�擾�ł��܂���";
	else if (!file_exists(fnam))
		GlobalErrMsg = SysErrorMessage(ERROR_FILE_NOT_FOUND);

	return GlobalErrMsg.IsEmpty();
}

//---------------------------------------------------------------------------
//tags �t�@�C�����擾
//---------------------------------------------------------------------------
UnicodeString get_tags_file(UnicodeString rnam)
{
	if (rnam.IsEmpty()) return EmptyStr;

	rnam = ExtractFilePath(rnam);
	UnicodeString tags = IncludeTrailingPathDelimiter(rnam) + "tags";
	while (!file_exists(tags)) {
		if (is_root_dir(rnam)) break;
		rnam = IncludeTrailingPathDelimiter(get_parent_path(rnam));
		tags = rnam + "tags";
	}

	if (!file_exists(tags)) tags = EmptyStr;

	return tags;
}

//---------------------------------------------------------------------------
//�^�O������^�O�����擾
//	�^�O�� \t �t�@�C���� \t �����p�^�[ ��/�s�ԍ�;"�` �`���̃��X�g���擾
//---------------------------------------------------------------------------
int get_FileNameByTag(
	UnicodeString tnam,		//[i]   �^�O��
	UnicodeString &rnam,	//[i/o] ��f�t�H���g��
	TStringList *lst,		//[o]   ���ʃ��X�g
	UnicodeString onam)		//�Ώۃt�@�C��	(default = EmptyStr)
{
	GlobalErrMsg = EmptyStr;

	try {
		if (tnam.IsEmpty()) UserAbort(USTR_NoParameter);
		tnam += "\t";

		UnicodeString tags = get_tags_file(rnam);
		if (tags.IsEmpty()) throw EAbort(LoadUsrMsg(USTR_NotFound, _T("tags �t�@�C��")));
		rnam = ExtractFilePath(tags);

		std::unique_ptr<TStringList> f_buf(new TStringList());
		if (load_text_ex(tags, f_buf.get())==0) UserAbort(USTR_FaildLoad);

		lst->Clear();
		for (int i=0; i<f_buf->Count; i++) {
			UnicodeString lbuf = f_buf->Strings[i];
			if (!StartsText(tnam, lbuf)) continue;
			if (!onam.IsEmpty() && !SameText(onam, to_absolute_name(get_pre_tab(get_post_tab(lbuf)), rnam))) continue;
			lst->Add(lbuf);
		}
		if (lst->Count==0) UserAbort(USTR_NotFound);

		return lst->Count;
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		return 0;
	}
}


//---------------------------------------------------------------------------
//HtmConv ���f�t�H���g�ŏ�����
//---------------------------------------------------------------------------
void ini_HtmConv_def(HtmConv *htmcnv, UnicodeString fnam)
{
	htmcnv->FileName	 = fnam;
	htmcnv->ToMarkdown	 = ToMarkdown;
	htmcnv->HeaderStr	 = HtmHdrStr;
	htmcnv->InsHrClass	 = HtmInsHrCls;
	htmcnv->InsHrSection = HtmInsHrSct;
	htmcnv->InsHrArticle = HtmInsHrArt;
	htmcnv->InsHrNav	 = HtmInsHrNav;
	htmcnv->DelBlkCls	 = HtmDelBlkCls;
	htmcnv->DelBlkId	 = HtmDelBlkId;
}

//---------------------------------------------------------------------------
// �w��t�@�C�������s/�J��
//---------------------------------------------------------------------------
bool Execute_ex(
	UnicodeString cmd,		//���s�t�@�C��/ �t�H���_�� or CLSId or shell �R�}���h(������\)
	UnicodeString prm,		//�p�����[�^				(default = EmptyStr)
	UnicodeString wdir,		//��ƃf�B���N�g��			(default = EmptyStr)
	UnicodeString opt,		//�I�v�V���� H=��\��/ W=�I���҂�/ O=�R���\�[���o�͂��擾/ L=���O�ɏo��
							//			 A=�Ǘ��҂Ƃ��Ď��s (W,O ����)
							//							(default = EmptyStr)
	DWORD *exit_code,		//  exit_code: �I���R�[�h	(default = NULL)
	TStringList *o_lst)		//  o_lst: �R���\�[���o��	(default = NULL)
{
	if (cmd.IsEmpty()) return false;

	try {
		cmd = exclude_quot(cmd);
		if (!wdir.IsEmpty() && is_root_dir(wdir)) wdir = IncludeTrailingPathDelimiter(wdir);
		UnicodeString wdir_str = "��ƃf�B���N�g��: " + wdir;

		if (ends_PathDlmtr(cmd)) {
			if (StartsStr("::{", cmd) || StartsText("shell:", cmd)) //CLSID or shell �R�}���h
				cmd = ExcludeTrailingPathDelimiter(cmd);
			AddDebugLog("Explorer", cmd, EmptyStr);
			if (::ShellExecute(NULL, _T("explore"), cmd.c_str(), NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32)
				Abort();
		}
		else if (ContainsStr(opt, "A")) {
			AddDebugLog("RunAs", cmd + " " + prm, wdir_str);
			if (::ShellExecute(NULL, _T("runas"), cmd.c_str(), prm.c_str(), wdir.c_str(),
				ContainsStr(opt, "H")? SW_HIDE : SW_SHOWNORMAL) <= (HINSTANCE)32)
					Abort();
		}
		else if (!ContainsStr(opt, "W") && !ContainsStr(opt, "O") && !ContainsStr(opt, "L")) {
			AddDebugLog("Execute", cmd + " " + prm, wdir_str);
			bool ok = false;
			if (test_LnkExt(get_extension(cmd))) {
				UnicodeString lnam, prm, fld;
				int shw;
				usr_SH->get_LnkInf(cmd, NULL, &lnam, &prm, &fld, &shw);
				if (!lnam.IsEmpty()) {
					ok = (::ShellExecute(NULL, _T("open"), lnam.c_str(), prm.c_str(), fld.c_str(), shw) > (HINSTANCE)32);
				}
			}
			if (!ok) {
				if (::ShellExecute(NULL, _T("open"), cmd.c_str(), prm.c_str(), wdir.c_str(),
					ContainsStr(opt, "H")? SW_HIDE : SW_SHOWNORMAL) <= (HINSTANCE)32)
						Abort();
			}
		}
		else {
			UnicodeString cmdln = add_quot_if_spc(cmd);
			if (!prm.IsEmpty()) cmdln.cat_sprintf(_T(" %s"), prm.c_str());
			AddDebugLog("Execute", cmdln, wdir_str);
			if (!Execute_cmdln(cmdln, wdir, opt, exit_code, o_lst)) Abort();
		}
		return true;
	}
	catch (...) {
		return false;
	}
}
//---------------------------------------------------------------------------
bool Execute_cmdln(UnicodeString cmdln, UnicodeString wdir,
	UnicodeString opt, DWORD *exit_code,
	TStringList   *o_lst,	//�o�̓��X�g	(default = NULL)
	TMemoryStream *o_ms)	//�o�̓C���[�W	(default = NULL)
{
	GlobalErrMsg  = EmptyStr;
	if (cmdln.IsEmpty()) return false;

	try {
		HANDLE hRead  = NULL;
		HANDLE hWrite = NULL;
		//���O�Ȃ��p�C�v���쐬
		if (contains_wd_i(opt, _T("O|L"))) {
			SECURITY_ATTRIBUTES	sa;
			sa.nLength				= sizeof(sa);
			sa.lpSecurityDescriptor = 0;
			sa.bInheritHandle		= TRUE;
			if (!::CreatePipe(&hRead, &hWrite, &sa, 0)) UserAbort(USTR_FaildProc);
		}

		STARTUPINFO si;
		::ZeroMemory(&si, sizeof(si));
		si.cb		   = sizeof(si);
		si.dwFlags	   = STARTF_USESHOWWINDOW;
		si.wShowWindow = ContainsStr(opt, "H")? SW_HIDE : SW_SHOWNORMAL;
		if (hRead && hWrite) {
			si.dwFlags |= STARTF_USESTDHANDLES;
			si.hStdOutput = hWrite;
			si.hStdError  = hWrite;
		}

		PROCESS_INFORMATION pi;
		if (::CreateProcess(NULL, cmdln.c_str(), NULL, NULL, TRUE, 0, NULL, wdir.c_str(), &si, &pi)) {
			//�I���҂�
			if (contains_wd_i(opt, _T("W|O|L"))) {
				bool exited = false;

				//�R���\�[���o�͂̎�荞��
				if (::WaitForInputIdle(pi.hProcess, 0)==0xffffffff && hRead && hWrite) {
					bool l_sw = ContainsStr(opt, "L");
					if (l_sw) {
						AddLogCr();
						//�R�}���h���C��
						UnicodeString lbuf = cmdln;
						UnicodeString nnam;
						if (remove_top_s(lbuf, '\"')) {
							nnam = ExtractFileName(get_tkn(lbuf, '\"'));
							lbuf = Trim(get_tkn_r(lbuf, '\"'));
						}
						else {
							nnam = ExtractFileName(get_tkn(lbuf, ' '));
							lbuf = Trim(get_tkn_r(lbuf, ' '));
						}
						if (!lbuf.IsEmpty()) lbuf.Insert(' ', 1);
						lbuf.Insert(StartsText("git.exe", nnam)? ("$ " + get_base_name(nnam)) : nnam, 1);
						AddLog(lbuf, false, true);
					}

					std::unique_ptr<TMemoryStream> ms(new TMemoryStream());
					TMemoryStream *mp = o_ms? o_ms : ms.get();
					UnicodeString log_buf;
					DWORD len;
					while (::WaitForSingleObject(pi.hProcess, WAIT_INTERVAL)==WAIT_TIMEOUT) {
						if (::PeekNamedPipe(hRead, NULL, 0, NULL, &len, NULL) && len>0) {
							std::unique_ptr<char[]> buf(new char[len + 4]);
							if (::ReadFile(hRead, buf.get(), len, &len, NULL)) {
								mp->Write(buf.get(), len);
								//���O�ɏo��
								if (l_sw) {
									std::unique_ptr<TMemoryStream> mbuf(new TMemoryStream());
									mbuf->Write(buf.get(), len);
									log_buf += get_MemoryStrins(mbuf.get());
									int p = pos_r(_T("\r\n"), log_buf);
									if (p>0) {
										AddLog(log_buf.SubString(1, p + 1), false, true);
										log_buf.Delete(1, p + 1);
									}
								}
							}
						}
						Application->ProcessMessages();
					}
					if (::PeekNamedPipe(hRead, NULL, 0, NULL, &len, NULL) && len>0) {
						std::unique_ptr<char[]> buf(new char[len + 4]);
						if (::ReadFile(hRead, buf.get(), len, &len, NULL)) {
							mp->Write(buf.get(), len);
							//���O�ɏo��
							if (l_sw) {
								std::unique_ptr<TMemoryStream> mbuf(new TMemoryStream());
								mbuf->Write(buf.get(), len);
								log_buf += get_MemoryStrins(mbuf.get());
								AddLog(log_buf, false, true);
							}
						}
						Application->ProcessMessages();
					}

					//�o�͓��e��ݒ�
					if (mp->Size>0 && o_lst) o_lst->Text = get_MemoryStrins(mp);
					exited = true;
				}
				//�I���҂�
				else if (ContainsStr(opt, "W")) {
					while (::WaitForSingleObject(pi.hProcess, 50)==WAIT_TIMEOUT) Application->ProcessMessages();;
					exited = true;
				}

				//�I����Ԃ��擾
				if (exited && exit_code) {
					if (!::GetExitCodeProcess(pi.hProcess, exit_code)) SysErrAbort(GetLastError());
				}
			}
			::CloseHandle(pi.hThread);
			::CloseHandle(pi.hProcess);
			if (hRead)  ::CloseHandle(hRead);
			if (hWrite) ::CloseHandle(hWrite);
		}
		//�N�����s
		else {
			if (hRead)  ::CloseHandle(hRead);
			if (hWrite) ::CloseHandle(hWrite);
			SysErrAbort(GetLastError());
		}
		return true;
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		return false;
	}
}

//---------------------------------------------------------------------------
//git.exe �����s
//---------------------------------------------------------------------------
bool GitShellExe(UnicodeString prm, UnicodeString wdir, TStringList *o_lst, DWORD *exit_cd)
{
	if (!GitExists) return false;

	wdir = ExcludeTrailingPathDelimiter(wdir);
	UnicodeString cmdln = add_quot_if_spc(CmdGitExe);
	if (!prm.IsEmpty()) cmdln.cat_sprintf(_T(" %s"), prm.c_str());

	DWORD exit_code = 0;
	bool res = Execute_cmdln(cmdln, wdir, "HWO", &exit_code, o_lst);
	if (exit_cd) *exit_cd = exit_code;

	return res;
}
//---------------------------------------------------------------------------
bool GitShellExe(UnicodeString prm, UnicodeString wdir, TMemoryStream *o_ms, DWORD *exit_cd)
{
	if (!GitExists) return false;

	wdir = ExcludeTrailingPathDelimiter(wdir);
	UnicodeString cmdln = add_quot_if_spc(CmdGitExe);
	if (!prm.IsEmpty()) cmdln.cat_sprintf(_T(" %s"), prm.c_str());

	DWORD exit_code = 0;
	bool res = Execute_cmdln(cmdln, wdir, "HWO", &exit_code, NULL, o_ms);
	if (exit_cd) *exit_cd = exit_code;

	return res;
}
//---------------------------------------------------------------------------
bool GitShellExe(UnicodeString prm, UnicodeString wdir)
{
	if (!GitExists) return false;

	UnicodeString cmdln = add_quot_if_spc(CmdGitExe);
	if (!prm.IsEmpty()) cmdln.cat_sprintf(_T(" %s"), prm.c_str());

	DWORD exit_code = 0;
	return (Execute_cmdln(cmdln, ExcludeTrailingPathDelimiter(wdir), "H", &exit_code) && exit_code==0);
}

//---------------------------------------------------------------------------
//���s/�ҋ@���̃^�X�N�����擾
//---------------------------------------------------------------------------
int get_BusyTaskCount()
{
	int busy_cnt = 0;
	//���s���^�X�N
	for (int i=0; i<MAX_TASK_THREAD; i++) {
		TTaskThread *tp = TaskThread[i];
		if (tp && !tp->TaskCancel) busy_cnt++;
	}
	//�\��
	busy_cnt += TaskReserveList->Count;
	return busy_cnt;
}

//---------------------------------------------------------------------------
//�������s���܂ލő�^�X�N�����擾
//---------------------------------------------------------------------------
int get_MaxTaskCount()
{
	int max_cnt = 0;
	for (int i=0; i<MAX_TASK_THREAD; i++) {
		TTaskThread *tp = TaskThread[i];
		if (tp && !tp->TaskCancel) max_cnt++;
	}

	return std::max(max_cnt, MaxTasks);
}

//---------------------------------------------------------------------------
//�󂫃^�X�N�����邩?
//---------------------------------------------------------------------------
bool has_EmptyTask(
	bool ex_sw)	//�ő�^�X�N���𒴂��Ď擾
{
	bool res = false;
	int maxn = ex_sw? std::min((MaxTasks + 2), MAX_TASK_THREAD): MaxTasks;
	for (int i=0; i<maxn && !res ; i++) if (!TaskThread[i]) res = true;
	return res;
}

//---------------------------------------------------------------------------
//�^�X�N�X���b�h���擾
//�󂫂��ȂȂ�������ۗ����̏ꍇ�͗\��
//---------------------------------------------------------------------------
TTaskThread * CreTaskThread(
	TaskConfig **cp,
	bool reserve,		//�\�񂷂�					(default = true)
	bool force,			//�ۗ����ł������擾		(default = false)
	bool ext_sw)		//�ő�^�X�N���𒴂��Ď擾	(default = false)
{
	TTaskThread *tp = NULL;
	if (force || !RsvSuspended) {
		int maxn = ext_sw? std::min(MaxTasks + 2, MAX_TASK_THREAD) : MaxTasks;
		for (int i=0; i<maxn; i++) {
			if (!TaskThread[i]) {
				tp = new TTaskThread(true);
				tp->Tag = i;
				TaskThread[i]  = tp;
				tp->Start();
				break;
			}
		}
	}

	*cp = tp? tp->Config : reserve? new TaskConfig() : NULL;
	return tp;
}

//---------------------------------------------------------------------------
//�^�X�N�̊J�n�܂��͗\��
//---------------------------------------------------------------------------
void ActivateTask(TTaskThread *tp, TaskConfig *cp)
{
	UnicodeString msg;
	if (tp) {
		tp->TaskStart();
		if (cp) StartLog(msg.sprintf(_T("%s�J�n  %s"), cp->CmdStr.c_str(), cp->InfStr.c_str()), tp->Tag);
	}
	else if (cp) {
		TaskReserveList->Add(cp);
		StartLog(msg.sprintf(_T("%s�\��  %s"), cp->CmdStr.c_str(), cp->InfStr.c_str()));
	}
}

//---------------------------------------------------------------------------
//�\��ۗ����ڂ̎��s
//---------------------------------------------------------------------------
void StartReserve()
{
	while (TaskReserveList->Count>0 && has_EmptyTask()) {
		TaskConfig  *cp = NULL;
		TTaskThread *tp = CreTaskThread(&cp, false);
		if (!tp || !cp) break;
		TaskConfig *rp = TaskReserveList->Items[0];
		cp->Assign(rp);
		delete rp;
		TaskReserveList->Delete(0);
		ActivateTask(tp, cp);
	}
}
//---------------------------------------------------------------------------
bool StartReserve(
	int idx,
	bool ext_sw)	//�ő�^�X�N���𒴂��ĊJ�n	(default = false)
{
	bool ok = false;
	if (idx>=0 && idx<TaskReserveList->Count && has_EmptyTask(ext_sw)) {
		TaskConfig  *cp = NULL;
		TTaskThread *tp = CreTaskThread(&cp, false, true, ext_sw);
		if (tp && cp) {
			TaskConfig *rp = TaskReserveList->Items[idx];
			cp->Assign(rp);
			delete rp;
			TaskReserveList->Delete(idx);
			ActivateTask(tp, cp);
			ok = true;
		}
	}
	return ok;
}

//---------------------------------------------------------------------------
//�^�X�N�ɒᑬ���s��v��
//---------------------------------------------------------------------------
void RequestSlowTask()
{
	for (int i=0; i<MAX_TASK_THREAD; i++) {
		TTaskThread *tp = TaskThread[i];
		if (tp) tp->ReqTaskSlow = true;
	}
}

//---------------------------------------------------------------------------
//���O�p�ɍŐV�̃G���[���b�Z�[�W���擾
//---------------------------------------------------------------------------
UnicodeString get_LogErrMsg(
	UnicodeString msg,	//���b�Z�[�W		(default = EmptyStr)
	bool add_cr,		//���s��}��		(default = true)
	UnicodeString fnam)	//�Ώۃt�@�C����	(default = EmptyStr)
{
	UnicodeString ret_str;

	if (LogErrMsg) {
		if (!msg.IsEmpty()) {
			ret_str = msg;
		}
		else {
			int msg_id = GetLastError();
			if (msg_id!=NO_ERROR) {
				ret_str = SysErrorMessage(msg_id);
				//�g�p���Ă���v���Z�X�̎��s�t�@�C������t��
				if ((msg_id==ERROR_SHARING_VIOLATION || msg_id==ERROR_LOCK_VIOLATION) && !fnam.IsEmpty()) {
					std::unique_ptr<TStringList> lst(new TStringList());
					get_ProcessingInf(fnam, lst.get(), true);
					if (lst->Count>0) {
						for (int i=0; i<lst->Count; i++)
							ret_str.cat_sprintf(_T("\r\n    >%s"), lst->Strings[i].c_str());
					}
				}
			}
			remove_top_text(ret_str, _T("%1 ��"));
		}

		if (add_cr && !ret_str.IsEmpty() && !StartsStr("\r\n", ret_str))
			ret_str.Insert("\r\n    ", 1);
	}

	return ret_str;
}
//---------------------------------------------------------------------------
void set_LogErrMsg(
	UnicodeString &msg,	//���b�Z�[�W
	UnicodeString s,	//�ǉ����b�Z�[�W	(default = EmptyStr)
	UnicodeString fnam)	//�Ώۃt�@�C����	(default = EmptyStr)
{
	if (StartsText("Abort", s)) s = EmptyStr;
	if (!msg.IsEmpty()) msg[1] = ContainsStr(s, "���f���܂���")? 'C' : 'E';
	msg += get_LogErrMsg(s, true, fnam);
}

//---------------------------------------------------------------------------
//�O���b�h�⃊�X�g�ɕ������`�悷��ۂ̏㕔�}�[�W�����擾
//---------------------------------------------------------------------------
int get_TopMargin(TCanvas *cv)
{
	return has_Leading(cv)? 0 : Scaled2;
}
//---------------------------------------------------------------------------
int get_TopMargin2(TCanvas *cv)
{
	return has_Leading(cv)? Scaled1 : std::max(ListInterLn/2, Scaled2);
}

//---------------------------------------------------------------------------
//���v�p��������擾
//---------------------------------------------------------------------------
UnicodeString GetClockStr()
{
	UnicodeString ret_str;
	if (!SttClockFmt.IsEmpty()) {
		ret_str = SttClockFmt;
		//�o�b�e���[
		UnicodeString lbuf, s, tmp;
		ret_str = ReplaceStr(ret_str, "$BP", tmp.sprintf(_T("\"%s%\""), get_BatteryPercentStr().c_str()));
		ret_str = ReplaceStr(ret_str, "$BT", tmp.sprintf(_T("\"%s\""),  get_BatteryTimeStr().c_str()));
		//�d�����
		if (ContainsStr(ret_str, "$PS(")) {
			lbuf = split_tkn(ret_str, _T("$PS(")) + "\"";
			s    = split_tkn(ret_str, ')');
			SYSTEM_POWER_STATUS ps;
			::GetSystemPowerStatus(&ps);
			lbuf += (((ps.ACLineStatus==1)? get_tkn(s, ':') : get_tkn_r(s, ':')) + "\"");
			ret_str.Insert(lbuf, 1);
		}
		//�ڑ����
		if (ContainsStr(ret_str, "$NS(")) {
			lbuf = split_tkn(ret_str, _T("$NS(")) + "\"";
			s    = split_tkn(ret_str, ')');
			lbuf += ((InternetConnected()? get_tkn(s, ':') : get_tkn_r(s, ':')) + "\"");
			ret_str.Insert(lbuf, 1);
		}

		//���t�J�E���g�_�E��
		while (ContainsStr(ret_str, "$CD(")) {
			lbuf = split_tkn(ret_str, _T("$CD("));
			s    = split_tkn(ret_str, ')');

			try {
				TDateTime dt;
				UnicodeString dstr = Trim(split_tkn(s, ';'));
				//yyyy/mm/dd
				if (is_match_regex(dstr + ";", _T("^\\d{4}/\\d{1,2}/\\d{1,2};"))) {
					if (!str_to_DateTime(dstr, &dt)) Abort();
				}
				//�ȗ�����
				else {
					bool to_sw	 = remove_top_s(dstr, '+');
					bool from_sw = remove_top_s(dstr, '-');
					//�N/�����ȗ�����Ă���(���̂�)
					if (is_match_regex(dstr + ";", _T("^\\d{1,2};"))) {
						unsigned short y = YearOf(Now());
						unsigned short m = YearOf(Now());
						unsigned short d = dstr.ToIntDef(0);	if (d==0) Abort();
						dt = set_NormDay(y, m, d);

						int d_m = (to_sw && dt<Today())? 1 : (from_sw && dt>Today())? -1 : 0;
						if (d_m!=0) {
							dt = IncMonth(dt, d_m);
							dt = set_NormDay(YearOf(dt), MonthOf(dt), d);
						}
					}
					//�N���ȗ�����Ă���
					else if (is_match_regex(dstr + ";", _T("^\\d{1,2}/\\d{1,2};"))) {
						unsigned short y = YearOf(Now());
						unsigned short m = get_tkn(dstr, '/').ToIntDef(0);
						unsigned short d = get_tkn_r(dstr, '/').ToIntDef(0);
						if (m==0 || d==0) Abort();
						dt = set_NormDay(y, m, d);

						int d_y = (to_sw && dt<Today())? 1 : (from_sw && dt>Today())? -1 : 0;
						if (d_y!=0) {
							dt = IncYear(dt, d_y);
							dt = set_NormDay(YearOf(dt), MonthOf(dt), d);
						}
					}
					else Abort();
				}

				if (s.IsEmpty()) s = format_Date(dt);
				int n = dt - Today();
				if (n==0) {
					s += "����";
				}
				else {
					if (n>0)
						s.cat_sprintf(_T("�܂�%s��"), get_size_str_B(n, 0).c_str());
					else
						s.cat_sprintf(_T("����%s��"), get_size_str_B(-n, 0).c_str());
				}
			}
			catch (...) {
				s = EmptyStr;
			}

			if (!s.IsEmpty()) lbuf.cat_sprintf(_T("\"%s\""), s.c_str());
			ret_str.Insert(lbuf, 1);
		}

		//���� (���p�����̕�����͕ϊ����ꂸ�A���p�����O���Ă��̂܂ܕ\��)
		ret_str = format_DateTimeEx(ret_str, Now());	//$EN �ŉp���
	}
	return ret_str;
}

//---------------------------------------------------------------------------
// �R�}���h�t�@�C���̉������I�v�V�����ݒ�ɔ��f
//---------------------------------------------------------------------------
void RenameOptCmdFile()
{
	while (RenCmdFileList->Count > 0) {
		UnicodeString lbuf	  = RenCmdFileList->Strings[0];
		UnicodeString org_nam = split_pre_tab(lbuf);
		UnicodeString new_nam = lbuf;
		UnicodeString new_rel = to_relative_name(lbuf);
		if (!org_nam.IsEmpty() && !new_rel.IsEmpty()) {
			//�C�x���g
			for (int i=0; i<MAX_EVENT_CMD; i++) {
				if (SameText(org_nam, to_absolute_name(get_tkn_r(*(EventCmdList[i].sp), '@'))))
					*(EventCmdList[i].sp) = "@" + new_rel;
			}
			//���X�g
			int idx = CmdFileList->IndexOf(org_nam);
			if (idx!=-1) CmdFileList->Strings[idx] = new_nam;

			//�ǉ����j���[
			for (int i=0; i<ExtMenuList->Count; i++) {
				TStringDynArray itm_buf = get_csv_array(ExtMenuList->Strings[i], EXTMENU_CSVITMCNT, true);
				if (is_separator(itm_buf[0]) || !USAME_TI(itm_buf[1], "ExeCommands")) continue;
				UnicodeString prm = itm_buf[2];
				if (!remove_top_AT(prm)) continue;
				if (is_same_file(org_nam, prm)) {
					itm_buf[2] = "@" + new_rel;
					ExtMenuList->Strings[i] = make_csv_rec_str(itm_buf);
				}
			}
			//�L�[�ݒ�
			for (int i=0; i<KeyFuncList->Count; i++) {
				UnicodeString vbuf = KeyFuncList->ValueFromIndex[i];
				if (!StartsText("ExeCommands_", vbuf)) continue;
				UnicodeString vstr = split_tkn(vbuf, '_') + "_";
				UnicodeString dsc  = split_dsc(vbuf);
				if (!dsc.IsEmpty()) vstr.cat_sprintf(_T(":%s:"), dsc.c_str());
				if (!remove_top_AT(vbuf)) continue;
				if (is_same_file(org_nam, vbuf)) KeyFuncList->ValueFromIndex[i] = vstr.cat_sprintf(_T("@%s"), new_rel.c_str());
			}
			//�֘A�t��(OpenByApp)
			for (int i=0; i<AssociateList->Count; i++) {
				UnicodeString vbuf = exclude_quot(AssociateList->ValueFromIndex[i]);
				if (!remove_top_AT(vbuf)) continue;
				if (is_same_file(org_nam, vbuf)) AssociateList->ValueFromIndex[i] = "@" + new_rel;
			}
			//�֘A�t��(OpenStandard)
			for (int i=0; i<OpenStdCmdList->Count; i++) {
				UnicodeString vbuf = OpenStdCmdList->ValueFromIndex[i];
				if (!remove_top_text(vbuf, _T("ExeCommands_@"))) continue;
				if (is_same_file(org_nam, vbuf)) OpenStdCmdList->ValueFromIndex[i] = "ExeCommands_@" + new_rel;
			}
		}
		RenCmdFileList->Delete(0);
	}
}

//---------------------------------------------------------------------------
//�R�}���h�t�@�C���̎Q�Ə����擾
//---------------------------------------------------------------------------
UnicodeString get_ref_CmdFile(
	UnicodeString fnam,
	TStringList *lst)	//[o] ��񃊃X�g	(default = NULL)
{
	UnicodeString ref_str = "���Q�ƌ���";

	//�L�[�ݒ�
	int K_cnt = 0;
	for (int i=0; i<KeyFuncList->Count; i++) {
		UnicodeString vbuf = KeyFuncList->ValueFromIndex[i];
		if (!remove_top_text(vbuf, _T("ExeCommands_")))	continue;
		UnicodeString dsc = split_dsc(vbuf);
		if (!remove_top_AT(vbuf)) 						continue;
		if (!is_same_file(fnam, exclude_quot(vbuf)))	continue;
		K_cnt++;
		if (lst) {
			add_PropLine(ref_str, "�L�[", lst);
			UnicodeString tmp = make_PropLine(_T("�ݒ�"), KeyFuncList->Names[i]);
			if (!dsc.IsEmpty()) tmp.cat_sprintf(_T(" : %s"), dsc.c_str());
			lst->Add(tmp);
		}
	}

	//�ǉ����j���[
	int M_cnt = 0;
	for (int i=0; i<ExtMenuList->Count; i++) {
		TStringDynArray itm_buf = get_csv_array(ExtMenuList->Strings[i], EXTMENU_CSVITMCNT, true);
		if (is_separator(itm_buf[0]) || !USAME_TI(itm_buf[1], "ExeCommands")) continue;
		UnicodeString prm = itm_buf[2];
		if (!remove_top_AT(prm)) 					continue;
		if (!is_same_file(fnam, exclude_quot(prm))) continue;
		M_cnt++;
		if (lst) {
			lst->Add(make_PropLine(ref_str, "�ǉ����j���[") + (equal_0(itm_buf[4])? " (��\��)" : ""));
			add_PropLine(_T("�^�C�g��"), itm_buf[0], lst);
			UnicodeString alias = itm_buf[3];
			if (!alias.IsEmpty()) {
				add_PropLine(_T("�G�C���A�X"), alias, lst);
				add_PropLine_if(_T("�L�["), get_Alias_KeyStr(alias), lst);
			}
		}
	}
	//�c�[���{�^��
	int B_cnt = 0;
	for (int ti=0; ti<3; ti++) {
		TStringList *tblst = (ti==1)? ToolBtnListV : (ti==2)? ToolBtnListI : ToolBtnList;
		for (int i=0; i<tblst->Count; i++) {
			UnicodeString vbuf = get_csv_item(tblst->Strings[i], 1);
			if (!remove_top_AT(vbuf))		continue;
			if (!is_same_file(fnam, vbuf))	continue;
			B_cnt++;
			if (lst) lst->Add(make_PropLine(ref_str, "�c�[���{�^��(") + ((ti==1)? "TV)" : (ti==2)? "IV)" : "FL)"));
		}
	}

	//�֘A�t��(OpenByApp)
	int A_cnt = 0;
	for (int i=0; i<AssociateList->Count; i++) {
		UnicodeString vbuf = AssociateList->ValueFromIndex[i];
		vbuf = exclude_quot(vbuf);
		if (!remove_top_AT(vbuf))		continue;
		if (!is_same_file(fnam, vbuf))	continue;
		A_cnt++;
		if (lst) add_PropLine(ref_str, "�֘A�t��(OpenByApp): " + AssociateList->Names[i], lst);
	}

	//�֘A�t��(OpenStandard)
	int S_cnt = 0;
	for (int i=0; i<OpenStdCmdList->Count; i++) {
		UnicodeString vbuf = OpenStdCmdList->ValueFromIndex[i];
		if (!remove_top_text(vbuf, _T("ExeCommands_@"))) continue;
		if (!is_same_file(fnam, vbuf))	continue;
		S_cnt++;
		if (lst) add_PropLine(ref_str, "�֘A�t��(OpenStandard): " + OpenStdCmdList->Names[i], lst);
	}
	//�C�x���g
	int E_cnt = 0;
	for (int i=0; i<MAX_EVENT_CMD; i++) {
		if (!is_same_file(fnam, get_tkn_r(*(EventCmdList[i].sp), '@'))) continue;
		E_cnt++;
		if (lst) {
			UnicodeString tmp = EventCmdList[i].dsc;
			remove_top_s(tmp, '|');
			add_PropLine(ref_str, "�C�x���g: " + tmp, lst);
		}
	}
	for (int i=0; i<MAX_TIMER_EVENT; i++) {
		UnicodeString vbuf = OnTimerEvent[i];
		if (!remove_top_AT(vbuf))		continue;
		if (!is_same_file(fnam, vbuf))	continue;
		E_cnt++;
		if (lst) {
			add_PropLine(ref_str, "�^�C�}�[�C�x���g", lst);
			add_PropLine(_T("���s����"), Timer_Condition[i], lst);
		}
	}

	UnicodeString ret_str;
	if (K_cnt>0) ret_str += "K";
	if (M_cnt>0) ret_str += "M";
	if (B_cnt>0) ret_str += "B";
	if (A_cnt>0) ret_str += "A";
	if (S_cnt>0) ret_str += "S";
	if (E_cnt>0) ret_str += "E";

	//���s��
	if (lst) {
		int idx = CmdFileList->IndexOf(fnam);
		if (idx!=-1) {
			cmdf_rec *cp = (cmdf_rec*)CmdFileList->Objects[idx];
			add_PropLine(_T("���s��"), cp->exe_count, lst);
		}
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�G�C���A�X���Q�Ƃ��Ă���L�[���擾(�J���}��؂蕶����)
//---------------------------------------------------------------------------
UnicodeString get_Alias_KeyStr(UnicodeString alias, TStringList *k_lst)
{
	UnicodeString ret_str;
	if (!k_lst) k_lst = KeyFuncList;
	for (int j=0; j<k_lst->Count; j++) {
		UnicodeString vbuf = k_lst->ValueFromIndex[j];
		if (!remove_top_Dollar(vbuf)) continue;
		if (SameText(alias, vbuf)) {
			if (!ret_str.IsEmpty()) ret_str += ", ";
			ret_str += k_lst->Names[j];
		}
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//��������p�����O�����ăp�����[�^�𒊏o
//---------------------------------------------------------------------------
UnicodeString extract_ExeParam(UnicodeString prm, UnicodeString *dsc)
{
	//�������擾/�폜
	UnicodeString dsc_str = split_dsc(prm);
	if (dsc) *dsc = dsc_str;

	//���p�����O��
	return exclude_quot(prm);
}

//---------------------------------------------------------------------------
//�}�X�N�ݒ�p��������쐬
//---------------------------------------------------------------------------
UnicodeString make_PathMask(UnicodeString mask, UnicodeString def_desc)
{
	UnicodeString ret_str;
	UnicodeString desc = split_dsc(mask);
	if (desc.IsEmpty()) desc = def_desc;

	if (!mask.IsEmpty()) {
		if (desc.IsEmpty()) desc = mask;
		ret_str.sprintf(_T(",%s,%s"), make_csv_str(desc).c_str(), make_csv_str(mask).c_str());
	}
	return ret_str;
}

//---------------------------------------------------------------------------
//���O�̃w�b�_���쐬
//---------------------------------------------------------------------------
UnicodeString make_LogHdr(
	UnicodeString cmd,		//�R�}���h���ʕ�����
	UnicodeString fnam,		//���O
	bool is_dir,			//fnam �� �f�B���N�g���� (default = false)
	int wd)					//�\���� (default = 0)
{
	bool full  = (LogFullPath && !USAME_TI(cmd, "LOAD")) || USAME_TI(cmd, "EDIT");

	bool slash = StartsStr('/', fnam);
	if (slash) fnam = slash_to_yen(fnam);

	UnicodeString lnam = is_dir ? ("[" +(full? ExcludeTrailingPathDelimiter(fnam) : get_dir_name(fnam)) + "]")
								: (full? warn_pathname_RLO(fnam) : warn_filename_RLO(fnam));

	if (slash) lnam = yen_to_slash(lnam);

	UnicodeString msg; msg.sprintf(_T("  %6s "), cmd.c_str());
	if (wd>0) msg.cat_sprintf(_T("%-*s"), wd, lnam.c_str()); else msg += lnam;
	return msg;
}
//---------------------------------------------------------------------------
UnicodeString make_LogHdr(const _TCHAR *cmd, UnicodeString fnam, bool is_dir, int wd)
{
	return make_LogHdr(UnicodeString(cmd), fnam, is_dir, wd);
}
//---------------------------------------------------------------------------
UnicodeString make_LogHdr(const _TCHAR *cmd, file_rec *fp)
{
	UnicodeString fnam = fp->f_name;
	if (fp->is_ftp) fnam = yen_to_slash(fnam);
	return make_LogHdr(UnicodeString(cmd), fnam);
}

//---------------------------------------------------------------------------
UnicodeString make_CreateLog(UnicodeString dnam)
{
	return make_LogHdr(_T("CREATE")).cat_sprintf(_T("[%s]"), dnam.c_str());
}
//---------------------------------------------------------------------------
UnicodeString make_RenameLog(UnicodeString o_nam, UnicodeString n_nam)
{
	return make_LogHdr(_T("RENAME"), o_nam).cat_sprintf(_T(" ---> %s"), ExtractFileName(n_nam).c_str());
}

//---------------------------------------------------------------------------
//���O�ɉ�������t��
//---------------------------------------------------------------------------
void set_RenameLog(UnicodeString &msg, UnicodeString fnam)
{
	if (msg.IsEmpty()) return;

	msg[1] = 'R';
	msg.cat_sprintf(_T(" ---> %s"), ExtractFileName(fnam).c_str());
}

//---------------------------------------------------------------------------
//���O���X�g�{�b�N�X�̍X�V
//---------------------------------------------------------------------------
void UpdateLogListBox()
{
	if (MaxLogLines>0) while (LogBufList->Count>MaxLogLines) LogBufList->Delete(0);

	TListBox *lp  = LogWndListBox;
	lp->Count	  = LogBufList->Count;
	lp->ItemIndex = lp->Count - 1;
	LogWndScrPanel->UpdateKnob();
}

//---------------------------------------------------------------------------
//���O�ɊJ�n����\��
//---------------------------------------------------------------------------
void StartLog(
	UnicodeString msg,	//���[�Z�[�W (\t �� " ---> " �ɒu��)
	int task_no)		//�^�X�N�ԍ� (default = -1)
{
	if (msg.IsEmpty()) {
		LogBufList->Add(EmptyStr);
	}
	else {
		//�O����s�łȂ���΁A��s��}��
		if (LogBufList->Count>0 && !LogBufList->Strings[LogBufList->Count - 1].IsEmpty()) LogBufList->Add(EmptyStr);
		//�J�n�\��
		UnicodeString s;
		if (task_no>=0) s.sprintf(_T("%u>"), task_no + 1); else s = ">>";
		s += FormatDateTime("hh:nn:ss ", Now()) + ReplaceStr(msg, "\t", " ---> ");
		LogBufList->Add(s);
	}

	UpdateLogListBox();
}
//---------------------------------------------------------------------------
void StartLog(const _TCHAR *msg, int task_no)
{
	StartLog(UnicodeString(msg), task_no);
}

//---------------------------------------------------------------------------
//���O�ɏI������\��
//---------------------------------------------------------------------------
void EndLog(UnicodeString msg, UnicodeString inf)
{
	if (contained_wd_i(_T("���k|��"), msg)) NotifyPrimNyan(msg + "���I�����܂���");

	AddLog(msg.cat_sprintf(_T("�I��%s"), inf.c_str()), true);
}
//---------------------------------------------------------------------------
void EndLog(const _TCHAR *msg, UnicodeString inf)
{
	EndLog(UnicodeString(msg), inf);
}
//---------------------------------------------------------------------------
void EndLog(int id, UnicodeString inf)
{
	EndLog(LoadUsrMsg(id), inf);
}

//---------------------------------------------------------------------------
//���s�G���[��\��
//---------------------------------------------------------------------------
void ExeErrLog(UnicodeString fnam, UnicodeString msg)
{
	UnicodeString lbuf = make_LogHdr(_T("EXEC"), fnam); lbuf[1] = 'E';
	AddLog(lbuf + get_LogErrMsg(msg));
}

//---------------------------------------------------------------------------
//���O���o��
//---------------------------------------------------------------------------
void AddLog(
	UnicodeString msg,	//���b�Z�[�W
	bool with_t,		//�^�C���\��   (default = false)
	bool raw)			//���̂܂܏o�� (default = false)
{
	if (!Initialized) return;

	UnicodeString s;
	if (!msg.IsEmpty()) {
		if (!raw)   s = " >";
		if (with_t) s += FormatDateTime("hh:nn:ss ", Now());
		s += msg;
	}
	else {
		s = "\r\n";
	}

	std::unique_ptr<TStringList> log_buf(new TStringList());
	log_buf->Text = s;
	LogBufList->AddStrings(log_buf.get());
	UpdateLogListBox();
}
//---------------------------------------------------------------------------
void AddLog(const _TCHAR *msg, bool with_t)
{
	AddLog(UnicodeString(msg), with_t);
}
//---------------------------------------------------------------------------
void AddLogStrings(TStringList *lst)
{
	LogBufList->AddStrings(lst);
	UpdateLogListBox();
}
//---------------------------------------------------------------------------
void AddLogCr()
{
	TListBox *lp = LogWndListBox;
	if (lp->Count>0 && !lp->Items->Strings[lp->Count - 1].IsEmpty()) {
		LogBufList->Add(EmptyStr);
		UpdateLogListBox();
	}
}
//---------------------------------------------------------------------------
void AddErr_Highlight()
{
	UnicodeString msg = make_LogHdr(_T("LOAD"), ExtractFileName(UserHighlight->FileName));
	if (UserHighlight->ErrorList->Count>0) {
		AddLogCr();
		msg[1] = 'E';
		AddLog(msg);
		for (int i=0; i<UserHighlight->ErrorList->Count; i++)
			AddLog(UserHighlight->ErrorList->Strings[i]);
		UserHighlight->ErrorList->Clear();
	}
	else {
		AddLog(msg);
	}
}
//---------------------------------------------------------------------------
void AddDebugLog(
	UnicodeString msg,		//���b�Z�[�W
	UnicodeString info,		//�s��� (1�s)
	UnicodeString info2)	//�s��� (�����s)
{
	if (!LogDebugInf) return;

	if (!msg.IsEmpty() || !info.IsEmpty()) {
		msg = "       ! " + msg;
		if (!info.IsEmpty()) msg.cat_sprintf(_T(" %s"), info.c_str());
		AddLog(msg);
	}

	if (!info2.IsEmpty()) {
		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Text = info2;
		for (int i=0; i<lst->Count; i++) AddLog("       ! " + lst->Strings[i]);
	}
}

//---------------------------------------------------------------------------
//LogBufStr �̓��e����ʂɏo��
//---------------------------------------------------------------------------
void FlushLog()
{
	LogRWLock->BeginRead();
		UnicodeString msg = LogBufStr;
	LogRWLock->EndRead();

	if (!msg.IsEmpty()) {
		LogRWLock->BeginWrite();
			LogBufStr.Delete(1, msg.Length());
		LogRWLock->EndWrite();

		std::unique_ptr<TStringList> log_buf(new TStringList());
		log_buf->Text = msg;
		AddLogStrings(log_buf.get());
	}
}

//---------------------------------------------------------------------------
//�`��̈ꎞ��~
// ���d�ݒ�ɑΉ�
//---------------------------------------------------------------------------
void set_RedrawOff(TWinControl *cp)
{
	int idx = RedrawList->IndexOfObject((TObject*)cp);
	if (idx==-1) {
		RedrawList->AddObject("1", (TObject*)cp);
		cp->Perform(WM_SETREDRAW, 0, (NativeInt)0);
	}
	else {
		int n = RedrawList->Strings[idx].ToIntDef(0);
		RedrawList->Strings[idx] = IntToStr(n + 1);
	}
}

//---------------------------------------------------------------------------
//�`���~�̉���
// ���ׂĉ������ꂽ�Ƃ��ɕ`��ĊJ
//---------------------------------------------------------------------------
void set_RedrawOn(TWinControl *cp)
{
	bool flag = false;
	int idx = RedrawList->IndexOfObject((TObject*)cp);
	if (idx!=-1) {
		//1�i�K����
		int n = RedrawList->Strings[idx].ToIntDef(0);
		if (n>0) n--;
		if (n==0) {
			//���ׂĉ������ꂽ
			RedrawList->Delete(idx);
			flag = true;
		}
		else {
			RedrawList->Strings[idx] = IntToStr(n);
		}
	}
	else {
		flag = true;
	}

	//�`��ĊJ
	if (flag) {
		cp->Perform(WM_SETREDRAW, 1, (NativeInt)0);
		::InvalidateRect(cp->Handle, NULL, TRUE);
	}
}

//---------------------------------------------------------------------------
//�t�H�[���ɑ΂������ȃL�[����
//  �߂�l: true = �������ꂽ
//---------------------------------------------------------------------------
bool __fastcall SpecialKeyProc(
	TForm *frm,
	WORD &Key,
	TShiftState Shift,
	const _TCHAR *topic)	//�w���v�g�s�b�N (default = NULL)
{
	UnicodeString KeyStr   = get_KeyStr(Key);		if (KeyStr.IsEmpty()) return false;
	UnicodeString ShiftStr = get_ShiftStr(Shift);
	KeyStr = ShiftStr + KeyStr;

	//Shift+F10 �ɂ��|�b�v�A�b�v���j���[�\��
	//���R���{�{�b�N�X�Ńf�t�H���g�̃��j���[���o�Ă��܂����ۂɑΉ�
	//  OnMessage �ŏE���Ȃ��悤�Ȃ̂ł����ŏ���
	if (USAME_TI(KeyStr, "Shift+F10")) {
		if (UserModule->ShowPopupMenu()) Key = 0;
		return true;
	}

	//�w���v
	if (equal_F1(KeyStr) && topic) {
		HtmlHelpTopic(topic);
		Key = 0;
		return true;
	}

	//�L�[�ɂ��_�C�A���O�̈ړ�/�T�C�Y�ύX
	if (ShiftStr.IsEmpty()) return false;

	bool handled = true;
	//�ړ�
	if (SameText(ShiftStr, DlgMoveShift)) {
		TRect dsk_rc = Screen->DesktopRect;
		switch (Key) {
		case VK_UP:	   frm->Top  = std::max<int>(frm->Top  - DlgMovePrm, dsk_rc.Top - frm->Height + 16);	break;
		case VK_DOWN:  frm->Top  = std::min<int>(frm->Top  + DlgMovePrm, dsk_rc.Bottom - 16);				break;
		case VK_LEFT:  frm->Left = std::max<int>(frm->Left - DlgMovePrm, dsk_rc.Left - frm->Width + 16);	break;
		case VK_RIGHT: frm->Left = std::min<int>(frm->Left + DlgMovePrm, dsk_rc.Right - 16);				break;
		default:	   handled = false;
		}
	}
	//�T�C�Y�ύX
	else if (SameText(ShiftStr, DlgSizeShift) && (frm->BorderStyle==bsSizeable || frm->BorderStyle==bsSizeToolWin)) {
		switch (Key) {
		case VK_UP:    frm->Height = frm->Height - DlgSizePrm;	break;
		case VK_DOWN:  frm->Height = frm->Height + DlgSizePrm;	break;
		case VK_LEFT:  frm->Width  = frm->Width  - DlgSizePrm;	break;
		case VK_RIGHT: frm->Width  = frm->Width  + DlgSizePrm;	break;
		default:	   handled = false;
		}
	}
	//����
	else if (USAME_TI(KeyStr, "Alt+F4")) {
		frm->Close();
	}
	else {
		handled = false;
	}

	if (handled) {
		//�R���{�{�b�N�X���h���b�v�_�E�����Ă��������
		for (int i=0; i<frm->ComponentCount; i++) {
			TComponent *cp = frm->Components[i];
			if (class_is_ComboBox(cp)) {
				if (((TComboBox*)cp)->DroppedDown)
					((TComboBox*)cp)->DroppedDown = false;
			}
		}

		Key = 0;
	}

	return handled;
}
//---------------------------------------------------------------------------
bool __fastcall SpecialKeyProc(
	TForm *frm,
	WORD &Key, TShiftState Shift,
	int hlp_cnt)	//�w���v�R���e�L�X�g
{
	UnicodeString topic;
	if (hlp_cnt>0) topic.sprintf(_T("hid00%03u.htm"), hlp_cnt);

	return SpecialKeyProc(frm, Key, Shift, topic.c_str());
}

//---------------------------------------------------------------------------
//����ȕҏW����
//  �J�[�\���ʒu�̑啶����/��������
//---------------------------------------------------------------------------
bool __fastcall SpecialEditProc(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	bool handled = false;

	if (Sender->InheritsFrom(__classid(TCustomEdit))) {
		TCustomEdit *ep = (TCustomEdit *)Sender;
		UnicodeString s = ep->Text;
		if (ep->SelLength==0 && ep->SelStart<s.Length()) {
			int p = ep->SelStart + 1;
			handled = true;
			if		(USAME_TI(KeyStr, "Ctrl+U")) s[p] = toupper(s[p]);	//�啶����
			else if (USAME_TI(KeyStr, "Ctrl+L")) s[p] = tolower(s[p]);	//��������
			else handled = false;

			if (handled) {
				ep->Text	 = s;
				ep->SelStart = p;
			}
		}
	}

	if (handled) Key = 0;
	return handled;
}

//---------------------------------------------------------------------------
//�f�B���N�g�����̓{�b�N�X
//---------------------------------------------------------------------------
UnicodeString inputbox_dir(const _TCHAR *tit, const _TCHAR *cmd)
{
	InpDirDlg->Caption	  = tit;
	InpDirDlg->CommandStr = cmd;
	return ((InpDirDlg->ShowModal()==mrOk)? InpDirDlg->InpDirComboBox->Text : EmptyStr);
}

//---------------------------------------------------------------------------
//2�X�g���[�N�����1�X�g���[�N�ڂ�?
//---------------------------------------------------------------------------
bool is_FirstKey(UnicodeString id, UnicodeString keystr)
{
	if (!EndsStr(':', id)) id += ":";
	UnicodeString key_n = id + keystr;
	bool found = false;
	for (int i=0; i<KeyFuncList->Count && !found; i++) {
		UnicodeString kbuf = KeyFuncList->Strings[i];
		if (!ContainsStr(kbuf, "~"))  continue;
		found = SameText(key_n, get_tkn(kbuf, '~'));
	}
	return found;
}

//---------------------------------------------------------------------------
//�������T�[�`�L�[��?
//  keystr �ɐ��K�\���p�^�[����Ԃ�
//---------------------------------------------------------------------------
bool is_IniSeaKey(UnicodeString &keystr)
{
	if (keystr.IsEmpty()) return false;
	UnicodeString k; k.sprintf(_T("F:%s"), keystr.c_str());
	if (!IniSeaShift.IsEmpty() && !remove_top_text(k, IniSeaShift)) return false;
	if (k.Length()!=1) return false;

	if (_istalpha(k[1]) || USAME_TS(k, "�_") || ((IniSeaByNum || IniSeaBySign) && _istdigit(k[1]))) {
		k = ReplaceStr(k, "�_", "_");
		if (IniSeaBySign) {
			//Shift+�����L�[�̋L�����T�[�`
			std::unique_ptr<TStringList> r_lst(new TStringList());
			r_lst->Text = is_JpKeybd()?
				"1\t[1!]\n3\t[3#]\n4\t[4\\$]\n5\t[5%]\n6\t[6&]\n7\t[7']\n8\t[8\\(]\n9\t[9\\)]\n" :				//JP
				"0\t[0\\)]\n1\t[1!]\n2\t[2@]\n3\t[3#]\n4\t[4\\$]\n5\t[5%]\n6\t[6\\^]\n7\t[7&]\n9\t[9\\(]\n";	//US
			k = replace_str_by_list(k, r_lst.get());
		}
		keystr.sprintf(_T("^%s"), k.c_str());
		return chk_RegExPtn(keystr);
	}
	else {
		return false;
	}
}

//---------------------------------------------------------------------------
//�C���N�������^���T�[�`�̃L�[���[�h���X�V
//  ��ʃ��[�h�ɂ���Đ���������؂�ւ��AJP/US�L�[�{�[�h�Ή�
//---------------------------------------------------------------------------
bool update_IncSeaWord(
	UnicodeString &kwd,		//[i/o] �L�[���[�h
	UnicodeString keystr,	//[i]   ���̓L�[
	bool fl_sw)				//[i]   �t�@�C�����Ɏg���Ȃ��������l�� (default = false)
{
	bool is_char = false;
	bool is_cap  = false;

	remove_text(keystr, _T("10Key_"));
	keystr = ReplaceStr(keystr, "��", "=");

	if (remove_top_text(keystr, KeyStr_Shift)) {
		if (keystr.Length()==1) {
			//Shift+�p��
			if (_istalpha(keystr[1])) {
				is_char = true; is_cap = true;
			}
			//Shift+����
			else if (_istdigit(keystr[1])) {
				std::unique_ptr<TStringList> r_lst(new TStringList());
				//�t�@�C����
				if (fl_sw) {
					r_lst->Text = is_JpKeybd()?
						"0\t\n1\t!\n2\t�h\n3\t#\n4\t$\n5\t%\n6\t&\n7\t'\n8\t(\n9\t)\n" : //JP
						"0\t)\n1\t!\n2\t@\n3\t#\n4\t$\n5\t%\n6\t^\n7\t&\n8\t\n9\t(\n";	 //US
				}
				//�e�L�X�g
				else {
					r_lst->Text = is_JpKeybd()?
						"0\t\n1\t!\n2\t\"\n3\t#\n4\t$\n5\t%\n6\t&\n7\t'\n8\t(\n9\t)\n" : //JP
						"0\t)\n1\t!\n2\t@\n3\t#\n4\t$\n5\t%\n6\t^\n7\t&\n8\t*\n9\t(\n";	 //US
				}
				keystr	= replace_str_by_list(keystr, r_lst.get());
				is_char = !keystr.IsEmpty();
			}
			//Shift+�L��
			else {
				std::unique_ptr<TStringList> r_lst(new TStringList());
				//�t�@�C����
				if (fl_sw) {
					r_lst->Text = is_JpKeybd()?
						"-\t=\n^\t~\n\\\t\n@\t`\n[\t{\n]\t}\n;\t+\n�_\t_\n,\t\n.\t\n/\t\n" :	//JP
						"`\t~\n-\t_\n=\t+\n[\t{\n]\t}\n;\t\n'\t\n\\\t\n,\t\n.\t\n/\t\n";		//US
				}
				//�e�L�X�g
				else {
					r_lst->Text = is_JpKeybd()?
						"-\t=\n^\t~\n\\\t|\n@\t`\n[\t{\n]\t}\n;\t+\n:\t*\n�_\t_\n,\t<\n.\t>\n/\t?\n" :	//JP
						"`\t~\n-\t_\n=\t+\n[\t{\n]\t}\n;\t:\n'\t\"\n\\\t|\n,\t<\n.\t>\n/\t?\n";	//US
				}
				keystr	= replace_str_by_list(keystr, r_lst.get());
				is_char = !keystr.IsEmpty();
			}
		}
	}
	else if (keystr.Length()==1) {
		//�p����
		if (_istalnum(keystr[1])) {
			is_char = true;
		}
		//�L��
		else {
			//�t�@�C����
			if (fl_sw)
				is_char = ContainsStr((is_JpKeybd()? "-^@[];,." : "-=[];,.`'"), keystr);
			//�e�L�X�g
			else
				is_char = ContainsStr((is_JpKeybd()? "-^\\@[];:,./" : "-=[];,.`'\\/"), keystr);
		}
	}
	else if (USAME_TI(keystr, "SPACE")) {
		keystr	= " ";
		is_char = true;
	}

	//�ꕶ�����
	if (contained_wd_i(_T("BKSP|Ctrl+H"), keystr)) {
		if (!kwd.IsEmpty()) delete_end(kwd);
		return true;
	}
	//�ꕶ���ǉ�
	if (is_char) {
		keystr = is_cap? keystr.UpperCase() : keystr.LowerCase();
		kwd += keystr;
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
//�P�Ƃ̒ʏ�R�}���h���H
//---------------------------------------------------------------------------
bool is_OneNrmCmd(
	UnicodeString cmds,
	bool no_prm)			//�p�����[�^���� (default = false)
{
	if (cmds.IsEmpty())									return false;
	if (starts_AT(cmds))	 							return false;
	if (pos_r_q_colon(cmds)!=0)							return false;
	if (CommandList->IndexOfName(get_CmdStr(cmds))==-1)	return false;
	if (no_prm && !get_PrmStr(cmds).IsEmpty())	 		return false;
														return true;
}

//---------------------------------------------------------------------------
//�t�B���^������̈ꗗ�̃J�[�\���ړ�
//---------------------------------------------------------------------------
bool MovListBoxFromFilter(TListBox *lp, UnicodeString key_str)
{
	if		(contained_wd_i(KeysStr_CsrDown, key_str)) ListBoxCursorDown(lp);
	else if (contained_wd_i(KeysStr_CsrUp,   key_str)) ListBoxCursorUp(lp);
	else if (contained_wd_i(KeysStr_PgDown,  key_str)) ListBoxPageDown(lp);
	else if (contained_wd_i(KeysStr_PgUp,    key_str)) ListBoxPageUp(lp);
	else 											   return false;

	return true;
}
//---------------------------------------------------------------------------
bool MovGridFromFilter(TStringGrid *gp, UnicodeString key_str)
{
	if		(contained_wd_i(KeysStr_CsrDown, key_str)) GridCursorDown(gp);
	else if (contained_wd_i(KeysStr_CsrUp,   key_str)) GridCursorUp(gp);
	else if (contained_wd_i(KeysStr_PgDown,  key_str)) GridPageDown(gp);
	else if (contained_wd_i(KeysStr_PgUp,    key_str)) GridPageUp(gp);
	else											   return false;

	return true;
}

//---------------------------------------------------------------------------
//���X�g�{�b�N�X�ł̃R�}���h����
//---------------------------------------------------------------------------
bool ExeCmdListBox(TListBox *lp, UnicodeString cmd, UnicodeString prm)
{
	if (!lp || cmd.IsEmpty()) return false;

	if (prm.IsEmpty()) {
		prm = exclude_quot(get_PrmStr(cmd));
		cmd = get_CmdStr(cmd);
	}
	prm = extract_ExeParam(prm);

	int idx = lp->ItemIndex;
	int pn	= lp->ClientHeight/lp->ItemHeight;
	int mov_n;

	if (prm.IsEmpty() && USAME_TI(cmd, "Scroll")) mov_n = ViewWheelScrLn;
	else if (USAME_TI(prm, "HP")) mov_n = pn/2;
	else if (USAME_TI(prm, "FP")) mov_n = pn;
	else mov_n = prm.ToIntDef(1);

	TFont *l_font = (lp->Tag & LBTAG_GEN_LIST)? GenListFont :
					(lp->Tag & (LBTAG_OPT_FIF1 | LBTAG_OPT_FIF2))? FileInfFont : ListFont;

	//�ړ�
	int cmd_id = idx_of_word_i(
		_T("CursorDown|CursorUp|CursorTop|TextTop|CursorEnd|TextEnd|")				//0�`5
		_T("PageDown|PageUp|ScrollDown|ScrollUp|ScrollCursorDown|ScrollCursorUp"),	//6�`11
		cmd);

	if (cmd_id!=-1) {
		switch (cmd_id) {
		case 0:
			if ((lp->Tag & LBTAG_OPT_LOOP) && idx==lp->Count-1) ListBoxTop(lp); else ListBoxCursorDown(lp, mov_n);
			break;
		case 1:
			if ((lp->Tag & LBTAG_OPT_LOOP) && idx==0) ListBoxEnd(lp); else ListBoxCursorUp(lp, mov_n);
			break;
		case 2: case 3:	ListBoxTop(lp);						break;
		case 4: case 5:	ListBoxEnd(lp);						break;
		case 6:			ListBoxPageDown(lp);				break;
		case 7:			ListBoxPageUp(lp);					break;
		case 8:			ListBoxScrollDown(lp, mov_n);		break;
		case 9:			ListBoxScrollUp(lp, mov_n);			break;
		case 10:		ListBoxScrollDown(lp, mov_n, true);	break;
		case 11:		ListBoxScrollUp(lp, mov_n, true);	break;
		}
	}
	//�I��
	else if (lp->MultiSelect && USAME_TI(cmd, "Select")) {
		if (idx!=-1) {
			lp->Selected[idx] = !lp->Selected[idx];
			ListBoxCursorDown(lp);
		}
	}
	//���ׂđI��
	else if (lp->MultiSelect && (StartsText("SelAll",cmd) || USAME_TI(cmd, "SelectAll"))) {
		ListBoxSelectAll(lp, lp->SelCount==0);
	}
	//�R�s�[
	else if (USAME_TI(cmd, "ClipCopy")) {
		//�I���s�R�s�[
		if (lp->MultiSelect && lp->SelCount>0) {
			cursor_HourGlass();
			std::unique_ptr<TStringList> cb_buf(new TStringList());
			for (int i=0; i<lp->Count; i++) {
				if (lp->Selected[i]) {
					UnicodeString lbuf = lp->Items->Strings[i];
					if (lp->Tag & LBTAG_TAB_FNAM) lbuf = get_pre_tab(lbuf);
					cb_buf->Add(lbuf);
				}
			}
			lp->Perform(WM_SETREDRAW, 0, (NativeInt)0);
			lp->ClearSelection();
			lp->Perform(WM_SETREDRAW, 1, (NativeInt)0);
			::InvalidateRect(lp->Handle, NULL, TRUE);
			if (USAME_TI(prm, "AD")) cb_buf->Text.Insert(Clipboard()->AsText, 1);	//�ǉ�
			cursor_Default();
			copy_to_Clipboard(cb_buf->Text);
		}
		//1�s�R�s�[
		else if (lp->Tag & LBTAG_OPT_LCPY) {
			if (idx!=-1) {
				UnicodeString lbuf = lp->Items->Strings[idx];
				if (lp->Tag & LBTAG_TAB_FNAM) lbuf = get_pre_tab(lbuf);
				copy_to_Clipboard(lbuf);
			}
		}
	}
	//URL���J��
	else if (USAME_TI(cmd, "OpenURL")) {
		if (idx!=-1) Execute_ex(extract_URL(lp->Items->Strings[idx]));
	}
	//�Y�[��
	else if ((lp->Tag & LBTAG_OPT_ZOOM) && contained_wd_i(_T("ZoomIn|ZoomOut"), cmd)) {
		int d_sz = std::min(prm.ToIntDef(1), 12);
		lp->Font->Size = USAME_TI(cmd, "ZoomIn") ? std::min(lp->Font->Size + d_sz, MAX_FNTZOOM_SZ)
											 	 : std::max(lp->Font->Size - d_sz, MIN_FNTZOOM_SZ);
		set_ListBoxItemHi(lp);
	}
	else if ((lp->Tag & LBTAG_OPT_ZOOM) && USAME_TI(cmd, "ZoomReset")) {
		lp->Font->Assign(l_font);
		set_ListBoxItemHi(lp);
	}
	//�t�H���g�T�C�Y�ύX
	else if ((lp->Tag & LBTAG_OPT_ZOOM) && USAME_TI(cmd, "SetFontSize")) {
		bool x_sw = remove_top_s(prm, '^');
		if (!prm.IsEmpty()) {
			int f_sz = std::max(std::min(prm.ToIntDef(l_font->Size), MAX_FNTZOOM_SZ), MIN_FNTZOOM_SZ);
			lp->Font->Size = (x_sw && lp->Font->Size==f_sz)? l_font->Size : f_sz;
			set_ListBoxItemHi(lp);
		}
	}
	//�E�N���b�N���j���[
	else if (StartsText("ContextMenu", cmd) && lp->PopupMenu) {
		show_PopupMenu(lp);
	}
	else return false;

	if (!lp->Focused()) lp->Invalidate();

	return true;
}
//---------------------------------------------------------------------------
bool ExeCmdListBox(TListBox *lp, const _TCHAR *cmd, UnicodeString prm)
{
	return ExeCmdListBox(lp, UnicodeString(cmd), prm);
}

//---------------------------------------------------------------------------
//�N���b�v�{�[�h�ɃR�s�[
//�R�s�[���̃q���g�\�����s��
//---------------------------------------------------------------------------
void copy_to_Clipboard(UnicodeString s)
{
	if (s.IsEmpty()) return;

	Clipboard()->AsText = s;
	::SendMessage(MainHandle, WM_NYANFI_CLPCOPIED, get_line_count(s) | 0x80000000, str_len_unicode(s));
	//WParam | 0x80000000 �Ńe�L�X�g�ł��邱�Ƃ�ʒm
}
//---------------------------------------------------------------------------
void copy_to_Clipboard(TPicture *pic)
{
	Clipboard()->Assign(pic);
	::SendMessage(MainHandle, WM_NYANFI_CLPCOPIED, pic->Width, pic->Height);
}
//---------------------------------------------------------------------------
void copy_to_Clipboard(TMetafile *mf)
{
	if (mf->Empty) return;

	Clipboard()->Assign(mf);
	::SendMessage(MainHandle, WM_NYANFI_CLPCOPIED, mf->Width, mf->Height);
}
//---------------------------------------------------------------------------
void copy_to_Clipboard(Graphics::TBitmap *bmp)
{
	if (bmp->Empty) return;

	Clipboard()->Assign(bmp);
	::SendMessage(MainHandle, WM_NYANFI_CLPCOPIED, bmp->Width, bmp->Height);
}

//---------------------------------------------------------------------------
//�ŏ��ɋN�����ꂽ NyanFi �̃n���h�����擾
//---------------------------------------------------------------------------
HWND get_PrimNyanWnd()
{
	std::unique_ptr<TStringList> lst(new TStringList());
	HWND hWnd = NULL;
	get_NyanFiList(lst.get());
	for (int i=0; i<lst->Count; i++) {
		if (!equal_1(get_csv_item(lst->Strings[i], 0))) continue;
		hWnd = (HWND)str_to_NativeInt(get_csv_item(lst->Strings[i], 1));
	}
	return hWnd;
}

//---------------------------------------------------------------------------
// �E�B�C�h�E�� NyanFi �̃��C����ʂ�?
//---------------------------------------------------------------------------
bool IsNyanfiWnd(HWND hWnd)
{
	return USAME_TS(get_WndClassName(hWnd), "TNyanFiForm");
}
//---------------------------------------------------------------------------
// �E�B�C�h�E�͓�d�N�����ꂽ NyanFi �̃��C����ʂ�?
//---------------------------------------------------------------------------
bool IsNyanfi2Wnd(HWND hWnd)
{
	return (USAME_TS(get_WndClassName(hWnd), "TNyanFiForm")
			&& is_match_regex(get_WndText(hWnd), _T("NyanFi-\\d+$")));
}

//---------------------------------------------------------------------------
//�ŏ��ɋN�����ꂽ NyanFi �֕�����𑗏o
//---------------------------------------------------------------------------
bool NotifyPrimNyan(UnicodeString msg)
{
	if (IsPrimary || !ShowDuplNotify) return false;

	HWND hPrimeWnd = get_PrimNyanWnd();
	if (!hPrimeWnd) return false;

	UnicodeString lbuf;
	lbuf.sprintf(_T("NyanFi-%u\r\n�@%s�@"), NyanFiIdNo, msg.c_str());

	COPYDATASTRUCT cd;
	cd.dwData = CPYDTID_DPL_MSG;
	cd.cbData = sizeof(_TCHAR) * (lbuf.Length() + 1);
	cd.lpData = lbuf.c_str();
	::SendMessage(hPrimeWnd, WM_COPYDATA, 0, (LPARAM)&cd);
	return true;
}

//---------------------------------------------------------------------------
//���[�_���\���̃E�B���h�E���擾
//---------------------------------------------------------------------------
BOOL CALLBACK EnumModalWndProc(HWND hWnd, LPARAM hTop)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid) return TRUE;

	if (::IsWindowVisible(hWnd) && USAME_TS(get_WndClassName(hWnd), "#32770")) {
		*((HWND*)hTop) = hWnd;
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
HWND get_ModalWnd()
{
	HWND top_wnd = NULL;

	//�\�����̃��[�_���t�H�[����T��
	for (int i=0; i<Screen->FormCount; i++) {
		TForm *fp = Screen->Forms[i];
		if (!fp->Visible || fp==Application->MainForm || !fp->Floating || !fp->Enabled || fp->FormStyle==fsStayOnTop) continue;
		if (fp->ClassNameIs("TExTxtViewer")) continue;
		top_wnd = fp->Handle;
		break;
	}

	//�Ȃ���Ε\�����̃��b�Z�[�W�{�b�N�X��T��
	if (!top_wnd && !::IsWindowEnabled(MainHandle)) ::EnumWindows(EnumModalWndProc , (LPARAM)&top_wnd);

	return top_wnd;
}

//---------------------------------------------------------------------------
//���� NyanFi ���I��
//---------------------------------------------------------------------------
BOOL CALLBACK CloseNyanWndProc(HWND hWnd, LPARAM cnt)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid && IsNyanfi2Wnd(hWnd)) {
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
		*((int*)cnt) += 1;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CALLBACK CountNyanWndProc(HWND hWnd, LPARAM cnt)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid && IsNyanfi2Wnd(hWnd)) *((int*)cnt) += 1;
	return TRUE;
}
//---------------------------------------------------------------------------
bool CloseOtherNyanFi()
{
	int cnt = 0;
	::EnumWindows(CloseNyanWndProc, (LPARAM)&cnt);
	if (cnt==0) return true;

	for (int i=0; i<20; i++) {	//�^�C���A�E�g10�b
		Application->ProcessMessages();
		cnt = 0;
		::EnumWindows(CountNyanWndProc, (LPARAM)&cnt);
		if (cnt==0) break;
		Sleep(500);
	}

	return (cnt==0);
}

//---------------------------------------------------------------------------
//�\�����̃E�B���h�E�ꗗ���擾
// �N���X�� \t �e�L�X�g/ Object = �n���h��
//---------------------------------------------------------------------------
BOOL CALLBACK EnumGenWndProc(HWND hWnd, LPARAM lst)
{
	if (::IsWindowVisible(hWnd)) {
		((TStringList*)lst)->AddObject(get_WndClassName(hWnd) + "\t" + get_WndText(hWnd), (TObject *)hWnd);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
int get_GenWndList(TStringList *lst)
{
	::EnumWindows(EnumGenWndProc, (LPARAM)lst);
	return lst->Count;
}

//---------------------------------------------------------------------------
//NyanFi �ꗗ���擾
// �A��(1�`),�n���h��,�^�C�g��
//---------------------------------------------------------------------------
BOOL CALLBACK EnumNyanWndProc(HWND hWnd, LPARAM lst)
{
	if (IsNyanfiWnd(hWnd)) {
		UnicodeString tbuf = get_WndText(hWnd);
		int p = pos_r(_T(" - "), tbuf);
		UnicodeString lbuf = (p>0)? tbuf.SubString(p + 3, 16) : tbuf;
		if (lbuf.Pos('-')) lbuf = get_tkn_r(lbuf, '-'); else lbuf = "1";

#if defined(_WIN64)
		lbuf.cat_sprintf(_T(",%llu,%s"), hWnd, tbuf.c_str());
#else
		lbuf.cat_sprintf(_T(",%u,%s"), hWnd, tbuf.c_str());
#endif

		((TStringList*)lst)->Add(lbuf);
	}
	return TRUE;
}
//---------------------------------------------------------------------------
int get_NyanFiList(TStringList *lst)
{
	lst->Clear();
	::EnumWindows(EnumNyanWndProc, (LPARAM)lst);
	lst->Sort();
	return lst->Count;
}

//---------------------------------------------------------------------------
//�V�K�N�����̃^�C�g�����擾
//---------------------------------------------------------------------------
UnicodeString get_NewTitle(bool primary)
{
	UnicodeString tnam;

	if (!primary) {
		std::unique_ptr<TStringList> lst(new TStringList());
		if (get_NyanFiList(lst.get())>0) {
			int n = get_tkn(lst->Strings[lst->Count - 1], ',').ToIntDef(0);
			if (n>0) {
				NyanFiIdNo = n + 1;
				tnam.sprintf(_T("NyanFi-%u"), NyanFiIdNo);
			}
		}
	}

	if (tnam.IsEmpty()) {
		NyanFiIdNo = 1;
		tnam = "NyanFi";
	}

	return tnam;
}

//---------------------------------------------------------------------------
//�O���NyanFi�̃n���h�����擾
//---------------------------------------------------------------------------
HWND get_NyanFiWnd(
	int rel_idx)	//1=��/ -1=�O
{
	HWND hWnd = NULL;

	std::unique_ptr<TStringList> lst(new TStringList());
	if (get_NyanFiList(lst.get())<2) return hWnd;

	int idx = -1;
	for (int i=0; i<lst->Count && idx==-1; i++)
		if ((HWND)str_to_NativeInt(get_csv_item(lst->Strings[i], 1))==MainHandle) idx = i;

	if (idx!=-1) {
		idx += rel_idx;
		while (idx<0) idx += lst->Count;
		idx %= lst->Count;
		hWnd = (HWND)str_to_NativeInt(get_csv_item(lst->Strings[idx], 1));
	}

	return hWnd;
}
//---------------------------------------------------------------------------
//NyanFi���A�N�e�B�u��
//  �߂�l: false=�����̃C���X�^���X���Ȃ�
//---------------------------------------------------------------------------
bool activate_NyanFi(
	int idx,	//rel? ����(1=��/ -1=�O) : ���
	bool rel)	//���Ύw��	(default = true);
{
	HWND hWnd = NULL;
	if (rel) {
		hWnd = get_NyanFiWnd(idx);	if (!hWnd) return false;
	}
	else {
		std::unique_ptr<TStringList> lst(new TStringList());
		get_NyanFiList(lst.get());
		UnicodeString n_str = idx;
		for (int i=0; i<lst->Count && !hWnd; i++) {
			if (SameStr(get_csv_item(lst->Strings[i], 0), n_str))
				hWnd = (HWND)str_to_NativeInt(get_csv_item(lst->Strings[i], 1));
		}
	}
	if (!hWnd) return false;

	if (::IsIconic(hWnd)) ::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	::SetForegroundWindow(hWnd);
	return true;
}

//---------------------------------------------------------------------------
//������NyanFi������
//---------------------------------------------------------------------------
bool IsDuplicated()
{
	std::unique_ptr<TStringList> lst(new TStringList());
	return (get_NyanFiList(lst.get())>1);
}

//---------------------------------------------------------------------------
//�ʃE�B���h�E�̃e�L�X�g�r���A�[�̃��X�g���擾
//---------------------------------------------------------------------------
int get_ExViewerList(TStringList *lst)
{
	for (int i=0; i<Screen->FormCount; i++) {
		TForm *fp = Screen->Forms[i];
		if (!fp->Visible) continue;
		if (fp->ClassNameIs("TExTxtViewer")) lst->AddObject(fp->Name, (TObject*)fp);
	}

	lst->Sort();
	return lst->Count;
}

//---------------------------------------------------------------------------
//�ʃE�B���h�E�̃e�L�X�g�r���A�[������΃t�H�[�J�X���ڂ�
//---------------------------------------------------------------------------
bool focus_ExViewer()
{
	std::unique_ptr<TStringList> lst(new TStringList());
	bool ok = (get_ExViewerList(lst.get()) > 0);
	if (ok) ((TForm *)lst->Objects[0])->SetFocus();
	return ok;
}
//---------------------------------------------------------------------------
//�ʃE�B���h�E�̂��ׂẴe�L�X�g�r���A�[�����
//---------------------------------------------------------------------------
void close_all_ExViewer(
	TForm *frm)		//���O����t�H�[��
{
	for (int i=0; i<Screen->FormCount; i++) {
		TForm *fp = Screen->Forms[i];
		if (fp->ClassNameIs("TExTxtViewer") && fp!=frm) fp->Close();
	}
}

//---------------------------------------------------------------------------
//�w���v�E�B���h�E���擾
//---------------------------------------------------------------------------
BOOL CALLBACK EnumHelpWndProc(HWND hWnd, LPARAM hHelp)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid) return TRUE;
	if (!USAME_TI(get_WndClassName(hWnd), "HH Parent")) return TRUE;
	*((HWND*)hHelp) = hWnd;
	return FALSE;
}
//---------------------------------------------------------------------------
HWND get_HelpWnd()
{
	HWND hlp_wnd = NULL;
	::EnumWindows(EnumHelpWndProc, (LPARAM)&hlp_wnd);
	return hlp_wnd;
}

//---------------------------------------------------------------------------
//���j���[���擾
//---------------------------------------------------------------------------
BOOL CALLBACK EnumMenuWndProc(HWND hWnd, LPARAM hMenu)
{
	DWORD pid;
	::GetWindowThreadProcessId(hWnd, &pid);
	if (ProcessId!=pid) return TRUE;

	if (!USAME_TS(get_WndClassName(hWnd), "#32768")) return TRUE;
	*((HWND*)hMenu) = hWnd;
	return FALSE;
}
//---------------------------------------------------------------------------
HWND get_MenuWnd()
{
	HWND menu_wnd = NULL;
	::EnumWindows(EnumMenuWndProc, (LPARAM)&menu_wnd);
	return menu_wnd;
}

//---------------------------------------------------------------------------
//�w���v����
//---------------------------------------------------------------------------
void HtmlHelpTopic(
	const _TCHAR *topic)	// topic: �t�@�C����[#ID]
{
	if (!lpfHtmlHelp) return;

	UnicodeString pnam;
	pnam.sprintf(_T("%s::/%s"),
		Application->HelpFile.c_str(), (topic && _tcslen(topic))? topic : _T(HELPTOPIC_TOP));

	lpfHtmlHelp(Screen->ActiveForm->Handle, pnam.c_str(), HH_DISPLAY_TOPIC, NULL);
}
//---------------------------------------------------------------------------
void HtmlHelpContext(int idx)
{
	if (!lpfHtmlHelp) return;
	lpfHtmlHelp(Screen->ActiveForm->Handle, Application->HelpFile.c_str(), HH_HELP_CONTEXT, idx);
}
//---------------------------------------------------------------------------
void HtmlHelpKeyword(
	UnicodeString fnam,		//�w���v�t�@�C����
	UnicodeString kwd)		//�L�[���[�h
{
	if (lpfHtmlHelp) {
		HH_AKLINK ak;
		::ZeroMemory(&ak, sizeof(HH_AKLINK));
		ak.cbStruct 	= sizeof(HH_AKLINK);
		ak.pszKeywords	= kwd.c_str();
		ak.fIndexOnFail = TRUE;
		if (lpfHtmlHelp) lpfHtmlHelp(Screen->ActiveForm->Handle, fnam.c_str(), HH_KEYWORD_LOOKUP, (DWORD)&ak);
	}
}
//---------------------------------------------------------------------------
void HtmlHelpClose()
{
	if (lpfHtmlHelp && get_HelpWnd()) lpfHtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0);
}

//---------------------------------------------------------------------------
//2�n�_�Ԃ̋������擾 (Hubeny�̌����𗘗p)
//---------------------------------------------------------------------------
double GetDistance(double lat1, double lng1, double lat2, double lng2)
{
	double lat1_r	= lat1 * M_PI / 180;
	double lng1_r	= lng1 * M_PI / 180;
	double lat2_r	= lat2 * M_PI / 180;
	double lng2_r	= lng2 * M_PI / 180;
	double ave_r	= (lat1_r + lat2_r) / 2;
	double meridian = 6334834.0 / sqrt(pow(1.0 - 0.006674*sin(ave_r)*sin(ave_r), 3));
	double prime_v	= 6377397.0 / sqrt(1.0 - 0.006674*sin(ave_r)*sin(ave_r));
	double x = meridian * (lat1_r - lat2_r);
	double y = prime_v * cos(ave_r) * (lng1_r - lng2_r);
	return sqrt(x*x + y*y);
}

//---------------------------------------------------------------------------
//�ܓx�o�x���w�肵�� Google Maps ���J��
//---------------------------------------------------------------------------
bool OpenGoogleMaps(
	double lat,		//lat: �ܓx
	double lng,		//lng: �o�x
	UnicodeString fnam)
{
	try {
		GlobalErrMsg  = EmptyStr;

		UnicodeString lbuf;
		//�e���v���[�g
		UnicodeString tnam = ExePath + WEBMAP_TPLT;
		if (file_exists(tnam)) {
			std::unique_ptr<TStringList> fbuf(new TStringList());
			if (load_text_ex(tnam, fbuf.get())==0) UserAbort(USTR_FaildProc);
			lbuf = fbuf->Text;
		}
		//�f�t�H���g
		else {
			lbuf =
				"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\r\n"
				"<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n"
				"<head>\r\n"
				"<meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\"/>\r\n"
				"<title>$Title$</title>"
				"<script type=\"text/javascript\" src=\"http://maps.google.com/maps/api/js?sensor=false\"></script>\r\n"
				"<script type=\"text/javascript\">\r\n"
				"google.maps.event.addDomListener(window, 'load', function() {\r\n"
				"var latlng = new google.maps.LatLng($Latitude$, $Longitude$);\r\n"
				"var options = {zoom:16, center:latlng, mapTypeId:google.maps.MapTypeId.ROADMAP};\r\n"
				"var map = new google.maps.Map(document.getElementById(\"gmap\"), options);\r\n"
				"var marker = new google.maps.Marker({position:latlng, map:map});\r\n"
				"});\r\n"
				"</script>\r\n"
				"<style TYPE=\"text/css\">\r\n<!--\r\nhtml,body {height:100%;}\r\n-->\r\n</style>\r\n"
				"</head>\r\n"
				"<body>\r\n"
				"<div id=\"gmap\" style=\"width:100%; height:90%;\"></div>\r\n"
				"$FileRef$"
				"</body></html>\r\n";

			lbuf = ReplaceStr(lbuf, "$Title$",	 !fnam.IsEmpty()? "$FileName$�̒n�}" : "�w��n�_�̒n�}");
			lbuf = ReplaceStr(lbuf, "$FileRef$", !fnam.IsEmpty()? "<p><a href=\"file:///$PathName$\">$PathName$</a></p>\r\n" : "");
		}

		lbuf = ReplaceStr(lbuf, "$Latitude$",  UnicodeString().sprintf(_T("%.8f") ,lat));
		lbuf = ReplaceStr(lbuf, "$Longitude$", UnicodeString().sprintf(_T("%.8f") ,lng));
		lbuf = ReplaceStr(lbuf, "$FileName$",  ExtractFileName(fnam));
		lbuf = ReplaceStr(lbuf, "$PathName$",  fnam);

		std::unique_ptr<TStringList> lst(new TStringList());
		lst->Text = lbuf;

		UnicodeString hnam;  hnam.sprintf(_T("%s" WEBMAP_FILE), TempPathA.c_str());
		if (!saveto_TextUTF8(hnam, lst.get())) throw EAbort(LoadUsrMsg(USTR_FaildSave, _T("�n�}�\���p�ꎞ�t�@�C��")));

		Execute_ex("file:///" + hnam);
		return true;
	}
	catch (EAbort &e) {
		GlobalErrMsg = e.Message;
		return false;
	}
}

//---------------------------------------------------------------------------
//�Í����̂��߂̃r�b�g�p�^�[�����쐬
//  Windows���[�U�� + �V�X�e���h���C�u�̃V���A���i���o�[���擾
//  �������ƂɃr�b�g���]�����A128�ɂȂ�܂ŌJ��Ԃ��Ȃ�
//  �擪���珇�ԂɁA����ȍ~�̒l�̍��v�̉��ʃo�C�g�������Ă���
//---------------------------------------------------------------------------
#define MAX_XORBIT_SIZE 128
//---------------------------------------------------------------------------
void get_cipher_ptn(BYTE *bp, int size)
{
	UnicodeString keystr = UserName;
	if (keystr.IsEmpty()) keystr = "UnknownUser";

	UnicodeString sysdrv = GetEnvironmentVariable("SYSTEMDRIVE");
	if (!sysdrv.IsEmpty()) {
		UnicodeString dstr = IncludeTrailingPathDelimiter(sysdrv).UpperCase();
		DWORD VolSerialNo, MaxCompLen, Flags;
		if (::GetVolumeInformation(dstr.c_str(), NULL, 0, &VolSerialNo, &MaxCompLen, &Flags, NULL, 0))
			keystr += IntToHex((int)VolSerialNo, 8);
	}

	int n = keystr.Length();
	for (int i=0; i<size; i++) bp[i] = (unsigned char)(~keystr[(i%n) + 1] & 0x00ff);

	for (int i=0; i<size-1; i++) {
		int sum = 0;
		for (int j=i+1; j<MAX_XORBIT_SIZE; j++) sum += bp[j];
		bp[i] = (unsigned char)(sum & 0x00ff);
	}
}

//---------------------------------------------------------------------------
//�Í���
//  1. ��������t���ɂЂ�����Ԃ�
//  2. �������ƂɃr�b�g�p�^�[����XOR
//  3. 2��16�i���̕�����ɂ��Č���
//---------------------------------------------------------------------------
UnicodeString cipher(UnicodeString orgstr)
{
	if (orgstr.IsEmpty()) return EmptyStr;

	std::unique_ptr<BYTE[]> ptn(new BYTE[MAX_XORBIT_SIZE]);
	get_cipher_ptn(ptn.get(), MAX_XORBIT_SIZE);

	UnicodeString retstr;
	int n = orgstr.Length();
	for (int i=0; i<n; i++)
		retstr.cat_sprintf(_T("%02x"), (unsigned char)(orgstr[n - i] ^ ptn[i%MAX_XORBIT_SIZE]));

	return retstr;
}

//---------------------------------------------------------------------------
//������
//  1.������̓�����2�����������16�i���Ƃ��Đ��l��
//  2.�r�b�g�p�^�[����XOR
//  3.�����ɒ����ċt���Ɍ���
//---------------------------------------------------------------------------
UnicodeString uncipher(UnicodeString cipstr)
{
	if (cipstr.IsEmpty()) return EmptyStr;

	std::unique_ptr<BYTE[]> ptn(new BYTE[MAX_XORBIT_SIZE]);
	get_cipher_ptn(ptn.get(), MAX_XORBIT_SIZE);

	UnicodeString retstr = EmptyStr;
	int n = cipstr.Length();
	for (int i=0; i<n/2; i++) {
		UnicodeString dgtstr = cipstr.SubString(i*2 + 1, 2);
		UnicodeString tmpstr;
		tmpstr.sprintf(_T("%c"), (unsigned char)(StrToIntDef("0x" + dgtstr, 0x3f) ^ ptn[i%MAX_XORBIT_SIZE]));
		retstr = tmpstr + retstr;
	}

	return retstr;
}

//---------------------------------------------------------------------------
//�ŋߎg�������ڂɒǉ�
//---------------------------------------------------------------------------
void AddToRecentFile(
	UnicodeString fnam)	//�t�@�C����(���p����)
{
	if (!AddToRecent) return;

	fnam = exclude_quot(fnam);
	if (!ContainsText(fnam, TempPathA) && !is_ADS_name(fnam))
		::SHAddToRecentDocs(SHARD_PATHW, fnam.c_str());
}
//---------------------------------------------------------------------------
void AddToRecentFile(TStringList *lst)
{
	if (!AddToRecent) return;

	for (int i=0; i<lst->Count; i++) {
		UnicodeString fnam = exclude_quot(lst->Strings[i]);
		if (!ContainsText(fnam, TempPathA) && !is_ADS_name(fnam))
			::SHAddToRecentDocs(SHARD_PATHW, fnam.c_str());
	}
}

//---------------------------------------------------------------------------
//�g���q�ɑ΂���^�u�����擾
//---------------------------------------------------------------------------
int get_ViewTabWidth(UnicodeString fext)
{
	return test_FileExt(fext, FExtViewTab4)? 4 :
			(ViewTabWidthX>0 && test_FileExt(fext, FExtViewTabX))? ViewTabWidthX : 8;
}

//---------------------------------------------------------------------------
//�c���[���X�g��(��)�쐬
//�Ilst->Objects �𗘗p
//---------------------------------------------------------------------------
void MakeTreeList(
	TStringList *lst,
	UnicodeString pnam,
	bool chk_sw)			//�������`�F�b�N	(default = false)
{
	if (lst->Count==0) return;

	pnam = IncludeTrailingPathDelimiter(pnam);
	//�c���[���X�g�Ȃ��U�T�u�f�B���N�g���E���X�g�ɖ߂�
	if (lst->Strings[0].Pos('\t')) {
		for (int i=0; i<lst->Count; i++) lst->Strings[i] = get_post_tab(lst->Strings[i]);
		if (SameText(IncludeTrailingPathDelimiter(lst->Strings[0]), pnam)) lst->Delete(0);
	}
	lst->CustomSort(Comp_PathTree);

	//�����Ă���K�w��₤
	if (chk_sw) {
		int i = 0;
		while (i<lst->Count) {
			TStringDynArray plst0 = split_path(ReplaceStr(get_tkn_r(lst->Strings[i], pnam), "/", "\\"));
			TStringDynArray plst1 = split_path((i>0)? ReplaceStr(get_tkn_r(lst->Strings[i - 1], pnam), "/", "\\") : EmptyStr);

			int n = 0;
			for (int j=0; j<plst0.Length && j<plst1.Length; j++) {
				if (SameText(plst0[j], plst1[j])) n++; else break;
			}

			int lvl = plst0.Length;
			int dn = lvl - n - 1;	//�����Ă���K�w��
			if (dn>0) {
				UnicodeString dnam = get_parent_path(lst->Strings[i]);
				for (int j=0; j<dn; j++) {
					lst->Insert(i, dnam);
					dnam = get_parent_path(dnam);
				}
				i += dn;
			}
			i++;
		}
	}

	//�\�����ڂ̏���
	for (int i=0; i<lst->Count; i++) {
		TStringDynArray plst = split_path(get_tkn_r(ReplaceStr(lst->Strings[i], "/", "\\"), pnam));
		int n = plst.Length;
		if (n>=1) {
			UnicodeString lbuf = lst->Strings[i];
			if (!EndsStr('/', lbuf)) lbuf = IncludeTrailingPathDelimiter(lbuf);
			lst->Strings[i] = StringOfChar(_T('?'), n) + plst[n - 1] + "\t" + lbuf;
			lst->Objects[i] = (TObject *)(NativeInt)n;
		}
	}

	//�r���̐ݒ�
	for (int i=0; i<lst->Count; i++) {
		UnicodeString lnam = lst->Strings[i];
		int n = (int)lst->Objects[i];
		if (n>=1) {
			int idx = i;
			for (int j=i+1; j<lst->Count; j++) {
				int nj = (int)lst->Objects[j];
				if (nj<=n) {
					if (nj==n) idx = j;
					break;
				}
			}

			if (idx>i) {
				lnam[n] = _T('��');
				if ((idx - i)>1) {
					for (int j=i+1; j<idx; j++) {
						UnicodeString ibuf = lst->Strings[j];
						ibuf[n] = _T('��');
						lst->Strings[j] = ibuf;
					}
				}
			}
			else {
				lnam[n] = _T('��');
			}
		}

		lst->Strings[i] = lnam;
	}

	for (int i=0; i<lst->Count; i++) {
		UnicodeString lnam = lst->Strings[i];
		int n = (int)lst->Objects[i];
		UnicodeString s0 = lnam.SubString(1, n);
		UnicodeString s1 = lnam.SubString(n + 1, lnam.Length() - n);
		s0 = ReplaceStr(s0, "��", "����");
		s0 = ReplaceStr(s0, "��", "����");
		s0 = ReplaceStr(s0, "��", "��  ");
		s0 = ReplaceStr(s0, "?",  "    ");
		lst->Strings[i] = s0 + s1;
	}

	lst->Insert(0, ExcludeTrailingPathDelimiter(pnam) + "\t" + pnam);
}

//---------------------------------------------------------------------------
//�c���[�`���̃��X�g�̊e�s���Ƀp�X��t��
//  �߂�l: false = �c���[�`���ł͂Ȃ�
//---------------------------------------------------------------------------
bool AddPathToTreeList(TStringList *lst)
{
	try {
		if (lst->Count==0) Abort();

		UnicodeString pnam = lst->Strings[0];
		if (!dir_exists(pnam)) Abort();

		std::unique_ptr<TStringList> o_buf(new TStringList());
		std::unique_ptr<TStringList> d_lst(new TStringList());
		o_buf->Add(lst->Strings[0] + "\t" + IncludeTrailingPathDelimiter(pnam));
		d_lst->Add(ExcludeTrailingPathDelimiter(pnam));

		UnicodeString arc_name;
		for (int i=1; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			if (lbuf.IsEmpty()) continue;

			UnicodeString lstr;
			for (int j=1; j<lbuf.Length(); j++) {
				if (!lbuf.IsDelimiter("�������� ", j)) break;
				lstr.cat_sprintf(_T("%c"), lbuf[j]);
			}
			if (lstr.IsEmpty()) Abort();

			UnicodeString dnam = get_tkn_r(lbuf, lstr);
			if (dnam.IsEmpty()) Abort();

			lstr = ReplaceStr(lstr, "��  ", "��");
			lstr = ReplaceStr(lstr, "    ", "�@");
			lstr = ReplaceStr(lstr, "��", EmptyStr);

			int lvl = lstr.Length();
			if (lvl>=d_lst->Count) while (lvl>=d_lst->Count) d_lst->Add(EmptyStr);
			d_lst->Strings[lvl] = dnam;

			UnicodeString sbuf;
			for (int j = 0; j<=lvl; j++) {
				UnicodeString s = d_lst->Strings[j];
				if (s.IsEmpty()) Abort();
				sbuf.cat_sprintf(_T("%s\\"), s.c_str());
			}

			if (!dir_exists(sbuf) && file_exists(ExcludeTrailingPathDelimiter(sbuf))) {
				arc_name = ExcludeTrailingPathDelimiter(sbuf);
				sbuf = arc_name + "/";
			}
			else if (!arc_name.IsEmpty() && StartsText(arc_name, sbuf)) {
				int p = (arc_name.Length()<sbuf.Length())? arc_name.Length() + 1 : 0;
				if (p>0 && sbuf[p]=='\\') sbuf[p] = '/';
			}

			lbuf += ("\t" + sbuf);
			o_buf->Add(lbuf);
		}

		lst->Assign(o_buf.get());
		return true;
	}
	catch (...) {
		return false;
	}
}

//---------------------------------------------------------------------------
//�R�}���h�����̒ǉ�
//---------------------------------------------------------------------------
void AddCmdHistory(UnicodeString cmd, UnicodeString prm, UnicodeString id)
{
	if (SameText(cmd, "CmdHistory") || (InhCmdHistory && !SameStr(id, "-"))) return;

	UnicodeString lbuf = FormatDateTime("hh:nn:ss.zzz", Now());
	lbuf.cat_sprintf(_T(" %s %s %s"), id.c_str(), (CurListTag==1)? _T("_R") : _T("L_"), cmd.c_str());
	if (!prm.IsEmpty()) lbuf.cat_sprintf(_T("_%s"), prm.c_str());

	if (!SameStr(id, "-")) {
		lbuf.cat_sprintf(_T("\t%s"), CurPathName.c_str());
		if (CurStt->is_Arc) lbuf += CurStt->arc_DspPath;
	}

	CommandHistory->Add(lbuf);
	while (CommandHistory->Count>MAX_CMD_HISTORY) CommandHistory->Delete(0);
}

//---------------------------------------------------------------------------
//���|�W�g���̃g�b�v�p�X���擾
//---------------------------------------------------------------------------
UnicodeString get_GitTopPath(UnicodeString dnam)
{
	UnicodeString gnam = IncludeTrailingPathDelimiter(dnam) + ".git";
	while (!file_exists(gnam)) {
		if (is_root_dir(dnam)) break;
		dnam = IncludeTrailingPathDelimiter(get_parent_path(dnam));
		gnam = dnam + ".git";
	}

	return file_exists(gnam)? ExtractFilePath(gnam) : EmptyStr;
}

//---------------------------------------------------------------------------
//.git\config �t�@�C�����擾
//---------------------------------------------------------------------------
UnicodeString get_GitConfig(UnicodeString dnam)
{
	UnicodeString cfg_nam;
	dnam = get_GitTopPath(dnam);
	if (!dnam.IsEmpty()) {
		cfg_nam = dnam + ".git\\config";
		if (!file_exists(cfg_nam)) cfg_nam = EmptyStr;
	}
	return cfg_nam;
}
//---------------------------------------------------------------------------
//�����[�g���|�W�g����URL���擾
//---------------------------------------------------------------------------
UnicodeString get_GitUrl(file_rec *fp)
{
	if (!fp || fp->is_virtual || fp->is_ftp) return EmptyStr;

	UnicodeString url;
	UnicodeString cfg_nam = get_GitConfig((fp->is_dir && !fp->is_up)? fp->f_name : fp->p_name);
	if (!cfg_nam.IsEmpty()) {
		int idx = GitCfgUrlList->IndexOfName(cfg_nam);
		if (idx!=-1) {
			//�L���b�V����񂠂�
			UnicodeString lbuf = GitCfgUrlList->ValueFromIndex[idx];
			try {
				if (!WithinPastMilliSeconds(get_file_age(cfg_nam), TDateTime(get_post_tab(lbuf)), TimeTolerance)) Abort();
				url = get_pre_tab(lbuf);
			}
			catch (...) {
				GitCfgUrlList->Delete(idx);
				idx = -1;
			}
		}

		if (url.IsEmpty()) {
			std::unique_ptr<TStringList> cfg_lst(new TStringList());
			load_text_ex(cfg_nam, cfg_lst.get());
			int cnt = 0;
			for (int i=0; i<cfg_lst->Count; i++) {
				UnicodeString lbuf = Trim(cfg_lst->Strings[i]);
				if (cnt==0 && USAME_TI(lbuf, "[remote \"origin\"]")) {
					cnt = 1;
				}
				else if (cnt==1) {
					if (StartsStr("[", lbuf)) break;
					UnicodeString key = Trim(split_tkn(lbuf, "="));
					if (USAME_TI(key, "url")) {
						url = Trim(lbuf);
						if (url.Pos('@')) {
							url = TRegEx::Replace(url, 
									"(https?://)(\\w+@)([\\w/:%#$&?()~.=+-]+)", "\\1\\3");
						}
						break;
					}
				}
			}
		}

		if (!url.IsEmpty()) {
			if (idx==-1) {
				//URL�Ή������L���b�V��
				GitCfgUrlList->Add(UnicodeString().sprintf(_T("%s=%s\t%s"), cfg_nam.c_str(), url.c_str(),
									FormatDateTime("yyyy/mm/dd hh:nn:ss", get_file_age(cfg_nam)).c_str()));
			}

			UnicodeString snam = IncludeTrailingPathDelimiter(fp->is_up? fp->p_name : fp->f_name);
			UnicodeString gnam = IncludeTrailingPathDelimiter(get_tkn(cfg_nam, "\\.git\\config"));
			if (StartsText(gnam, snam)) {
				snam.Delete(1, gnam.Length());
				snam = yen_to_slash(ExcludeTrailingPathDelimiter(snam));
				if (!snam.IsEmpty()) {
					remove_end_text(url, ".git");	//Gitlab
					//Bitbucket
					if (ContainsStr(url, "/bitbucket.org/")) {
						url.cat_sprintf(_T("/src/master/%s"), snam.c_str());
					}
					//Github
					else {
						if (fp->is_dir || snam.Pos('/'))
							url.cat_sprintf(_T("/tree/master/%s"), snam.c_str());
						else
							url.cat_sprintf(_T("/blob/master/%s"), snam.c_str());
					}
				}
				else {
					//Bitbucket
					if (ContainsStr(url, "/bitbucket.org/")) {
						remove_end_text(url, ".git");
						url += "/src/master/";
					}
				}
			}
		}
	}

	return url;
}

//---------------------------------------------------------------------------
//Git �X�e�[�^�X���X�g���擾
//  �t���O���� [TAB] �t�@�C���p�X
//---------------------------------------------------------------------------
int get_GitStatusList(
	UnicodeString pnam,
	TStringList *lst,
	bool full_sw)		//�t���p�X�Ŏ擾	(default = false)
{
	try {
		if (!GitExists) Abort();

		pnam = get_GitTopPath(IncludeTrailingPathDelimiter(pnam));
		if (pnam.IsEmpty()) Abort();

		std::unique_ptr<TStringList> o_buf(new TStringList());
		DWORD exit_code = 0;
		if (!GitShellExe("status --porcelain", pnam, o_buf.get(), &exit_code) || exit_code!=0) Abort();

		lst->Clear();
		for (int i=0; i<o_buf->Count; i++) {
			UnicodeString lbuf = o_buf->Strings[i];
			UnicodeString flag = lbuf.SubString(1, 3);
			if (flag.Length()<3 || flag[3]!=' ') continue;
			lbuf.Delete(1, 3);
			if (full_sw) lbuf = to_absolute_name(slash_to_yen(lbuf), pnam);
			lst->Add(flag + lbuf);
		}
		return lst->Count;

	}
	catch (...) {
		return -1;
	}
}

//---------------------------------------------------------------------------
//���|�W�g�����ŕύX�̂������t�@�C�����X�g���擾
//---------------------------------------------------------------------------
int get_GitChangedList(UnicodeString pnam, TStringList *lst)
{
	try {
		if (!GitExists) Abort();

		pnam = IncludeTrailingPathDelimiter(pnam);
		UnicodeString tnam = get_GitTopPath(pnam);
		if (tnam.IsEmpty()) Abort();

		lst->Clear();
		std::unique_ptr<TStringList> o_buf(new TStringList());
		if (get_GitStatusList(pnam, o_buf.get(), true)>0) {
			for (int i=0; i<o_buf->Count; i++) {
				UnicodeString lbuf = o_buf->Strings[i];
				UnicodeString flag = lbuf.SubString(1, 3);
				if (flag.Length()<3 || flag[3]!=' ') continue;
				lbuf.Delete(1, 3);
				if (flag[1]!='?' && flag[2]!='?') lst->Add(lbuf);
			}
		}
		return lst->Count;
	}
	catch (...) {
		return -1;
	}
}

//---------------------------------------------------------------------------
//Git�X�e�[�^�X��������擾
//  �߂�l: �S�̂̏�� [TAB] ���[�L���O�c���[ [TAB] �C���f�b�N�X
//---------------------------------------------------------------------------
UnicodeString get_GitStatusStr(
	TStringList *lst,
	bool *staged)		//[o] �X�e�[�W���O����Ă���
{
	UnicodeString stt_wk, stt_ix, stt_all;
	if (staged) *staged = false;

	if (lst->Count>=0) {
		int flag[8][2] = {};
		UnicodeString flag_str = "MADRCU?";
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = lst->Strings[i];
			int p0 = flag_str.Pos(lbuf[1]);
			int p1 = flag_str.Pos(lbuf[2]);
			if (p0>0) flag[p0][0]++;
			if (p1>0) flag[p1][1]++;
			if (staged && (p0>=1 && p0<=6)) *staged = true;
		}

		for (int i=1; i<8; i++) {
			if (flag[i][1]>0) stt_wk.cat_sprintf(_T(" %c:%u"), flag_str[i], flag[i][1]);
			if (flag[i][0]>0) stt_ix.cat_sprintf(_T(" %c:%u"), flag_str[i], flag[i][0]);
			if (flag[i][0]>0 || flag[i][1]>0) {
				stt_all.cat_sprintf(_T(" %c:%u/%u"), flag_str[i], flag[i][0], flag[i][1]);
			}
		}

		stt_all = Trim(replace_regex(stt_all, _T("\\b0\\b"), _T("_")));
		stt_all = def_if_empty(stt_all, "Clean");
		stt_wk	= def_if_empty(Trim(stt_wk), "Clean");
		stt_ix	= def_if_empty(Trim(stt_ix), "Nothing to commit");
	}

	return (stt_all + "\t" + stt_wk + "\t" + stt_ix );
}

//---------------------------------------------------------------------------
//�s����Git�O���t�������擾
//---------------------------------------------------------------------------
UnicodeString get_GitGraphStr(UnicodeString lbuf)
{
	int p = 0;
	for (int i=1; i<=lbuf.Length(); i++) {
		if (!lbuf.IsDelimiter("*|\\/_ ", i)) break;
		p = i;
	}

	return (p>0)? lbuf.SubString(1, p) : EmptyStr;
}
//---------------------------------------------------------------------------
//�s����Git�O���t�����𕪗�
//---------------------------------------------------------------------------
UnicodeString split_GitGraphStr(UnicodeString &lbuf)
{
	int p = 0;
	for (int i=1; i<=lbuf.Length(); i++) {
		if (!lbuf.IsDelimiter("*|\\/_ ", i)) break;
		p = i;
	}

	UnicodeString ret_str;
	if (p>0 && lbuf[p]==' ') {
		ret_str = lbuf.SubString(1, p);
		lbuf.Delete(1, p);
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//Git�O���t�̕`��
//---------------------------------------------------------------------------
void draw_GitGraph(
	UnicodeString s,
	UnicodeString s1,	//�O�s
	UnicodeString s2,	//���s
	TCanvas *cv,
	TRect &rc,			//�\���ʒu (rc.Left �X�V)
	bool is_head,		//HEAD (default = false)
	bool is_wip)		//WIP  (default = false)
{
	if (s.IsEmpty()) return;

	int xp = rc.Left + 2;
	int w  = rc.Height()/3;
	int w2 = w/2;
	int r  = rc.Height()/5;

	int y0 = rc.Top;
	int y1 = rc.Bottom;
	int hi = rc.Height();
	int yc = y0 + hi/2;

	int slen = s.Length();
	if (s1.Length()<(slen + 1)) s1 += StringOfChar(_T(' '), slen - s1.Length() + 1);
	if (s2.Length()<(slen + 1)) s2 += StringOfChar(_T(' '), slen - s2.Length() + 1);

	cv->Pen->Style	 = psSolid;
	cv->Pen->Width	 = Scaled1;

	TColor org_bg = cv->Brush->Color;
	cv->Brush->Color = is_head? col_GitHEAD : is_wip? org_bg : col_GitMark;
	cv->Brush->Style = bsSolid;

	for (int i=1; i<=slen; i++) {
		int x1 = xp;
		int x2 = x1 + w;
		int xc = x1 + w2;
		WideChar c1l = (i>1)? s1[i - 1] : ' ';
		WideChar c1r = s1[i + 1];
		WideChar c2l = (i>1)? s2[i - 1] : ' ';
		WideChar c2r = s2[i + 1];
		cv->Pen->Color = col_HR;

		switch (s[i]) {
		case '*':
			if ((s1[i]!='-' && s1[i]!=' ') || c1l=='\\' || c1r=='/') {
				cv->MoveTo(xc, y0); cv->LineTo(xc, yc);
			}
			if ((s1[i]!='-' && s2[i]!=' ') || c2l=='/' || c2r=='\\') {
				cv->MoveTo(xc, yc); cv->LineTo(xc, y1);
			}
			cv->Pen->Color = is_head? col_GitHEAD : col_GitMark;
			cv->Ellipse(xc - r, yc - r, xc + r, yc + r);
			break;
		case '|':
			cv->MoveTo(xc, y0); cv->LineTo(xc, y1);
			break;
		case '/':
			if (c1r=='|' || c1r=='*') x2 += w2;
			if (c2l=='|' || c2l=='*') x1 -= w2;
			cv->MoveTo(x2, y0); cv->LineTo(x1 - 1, y1 + 1);
			break;
		case '\\':
			if (c1l=='|' || c1l=='*') x1 -= w2;
			if (c2r=='|' || c2r=='*') x2 += w2;
			cv->MoveTo(x1, y0); cv->LineTo(x2, y1);
			break;
		case '_':
			cv->MoveTo(x1 - w2, y1 - 1); cv->LineTo(x2 + w2, y1 - 1);
			break;
		}

		xp += w;
		if (xp>=rc.Right) break;
	}

	rc.Left = std::min(xp, (int)rc.Right);
	cv->Brush->Color = org_bg;
}
//---------------------------------------------------------------------------
//Git�^�O�̕`��
//---------------------------------------------------------------------------
void draw_GitTag(
	TCanvas *cv, int &x, int y,
	UnicodeString tag,
	int mgn)			//  (default = 0)
{
	TColor org_bg = cv->Brush->Color;
	int mk_h = cv->TextHeight("Q");
	int mk_w = mk_h/4;
	x += 2;
	TPoint shape[3]  = {Point(x, y + mk_h/2), Point(x + mk_w, y), Point(x + mk_w, y + mk_h - 1)};
	cv->Pen->Style	 = psSolid;
	cv->Pen->Color	 = col_GitTag;
	cv->Brush->Color = col_GitTag;
	cv->Polygon(shape, 2);
	x += (mk_w + 1);
	cv->Brush->Color = org_bg;
	out_TextEx(cv, x, y, tag, org_bg, col_GitTag, mgn);
}

//---------------------------------------------------------------------------
//Git���|�W�g�����(Git-Commit/Status)���擾
//---------------------------------------------------------------------------
void get_GitInf(
	UnicodeString dnam,
	TStringList *lst,	//[o] ��񃊃X�g
	bool upd_sw,		//�ŐV�̏��ɍX�V	(default = false)
	bool ext_sw)		//�������̂ݗ��p	(default = false)
{
	//.git/index ���ω����Ă��Ȃ�������񂪂���Η��p
	dnam = IncludeTrailingPathDelimiter(dnam);
	UnicodeString xnam = dnam + ".git\\index";
	TDateTime xdt = file_exists(xnam)? get_file_age(xnam) : 
		   file_exists(dnam + ".git")? get_file_age(dnam + ".git")
									 : get_file_age(dnam);

	int idx = GitInfList->IndexOfName(dnam);
	if (idx!=-1) {
		try {
			if (upd_sw) Abort();
			if (lst) {
				TStringDynArray ibuf = get_csv_array(GitInfList->ValueFromIndex[idx], 3);
				if (ibuf.Length==0) Abort();
				if (!WithinPastMilliSeconds(xdt, VarToDateTime(ibuf[0]), TimeTolerance)) Abort();
				//��������ݒ�
				for (int i=1; i<ibuf.Length; i++) {
					UnicodeString inam = get_tkn(ibuf[i], ": ");
					if (!inam.IsEmpty()) add_PropLine(inam, get_tkn_r(ibuf[i], ": "), lst);
				}
				return;
			}
		}
		catch (...) {
			GitInfList->Delete(idx);
		}
	}

	if (!upd_sw && ext_sw) return;

	TStringDynArray ibuf;
	add_dyn_array(ibuf, FormatDateTime("yyyy/mm/dd hh:nn:ss", xdt));

	std::unique_ptr<TStringList> o_buf(new TStringList());
	DWORD exit_code;

	//�R�~�b�g���
	UnicodeString hide_items = HideInfItems->Values["\\"];
	if (!test_word_i("Git-Commit", hide_items)
		&& GitShellExe("log -1 --pretty=format:\"%d\t%s\"", dnam, o_buf.get(), &exit_code)
			&& exit_code==0 && o_buf->Count>0)
	{
		UnicodeString lbuf = o_buf->Strings[0];
		UnicodeString cmt_s;
		TStringDynArray b_buf = SplitString(get_in_paren(get_pre_tab(lbuf)), ",");
		for (int i=0; i<b_buf.Length; i++) {
			UnicodeString s = Trim(b_buf[i]);
			if (s.Pos("/")==0) {
				if (!cmt_s.IsEmpty()) cmt_s += ", ";
				cmt_s += s;
			}
		}
		if (!cmt_s.IsEmpty()) {
			cmt_s = ReplaceStr(cmt_s, "HEAD -> ", HEAD_Mark);
			cmt_s = "(" + cmt_s + ") ";
		}
		cmt_s += get_post_tab(lbuf);
		if (lst) add_PropLine_if(_T("Git-Commit"), cmt_s, lst);
		add_dyn_array(ibuf, "Git-Commit: " + cmt_s);
	}

	//���[�L���O�E�c���[�̏��
	o_buf->Clear();
	if (!test_word_i("Git-Status", hide_items) && get_GitStatusList(dnam, o_buf.get())>=0) {
		UnicodeString stt_s = get_pre_tab(get_GitStatusStr(o_buf.get()));
		if (lst) add_PropLine_if(_T("Git-Status"), stt_s, lst);
		add_dyn_array(ibuf, "Git-Status: " + stt_s);
	}

	//�L���b�V���ɕۑ�
	if (ibuf.Length>1) GitInfList->Add(dnam + "=" + make_csv_rec_str(ibuf));
}

//---------------------------------------------------------------------------
//�����p�̃t�@�C�������擾
//---------------------------------------------------------------------------
UnicodeString get_GitDiffFiles(UnicodeString s, UnicodeString &fnam2)
{
	UnicodeString s1, fnam1;
	if (s.Pos('{')) {
		if (remove_top_s(s, '{')) {
			s1	  = split_tkn(s, '}');
			fnam1 = get_tkn(s1, " => ") + s;
			fnam2 = get_tkn_r(s1, " => ");
			if (!fnam2.IsEmpty()) fnam2 += s;
		}
		else {
			s1	  = split_tkn(s, '{');
			s	  = get_tkn(s, '}');
			fnam1 = s1 + get_tkn(s, " => ");
			fnam2 = get_tkn_r(s, " => ");
			if (!fnam2.IsEmpty()) fnam2 = s1 + fnam2;
		}
	}
	else {
		if (s.Pos(" => ")) {
			fnam1 = get_tkn(s, " => ");
			fnam2 = get_tkn_r(s, " => ");
		}
		else {
			fnam1 = s;
		}
	}

	if (fnam2.IsEmpty()) fnam2 = fnam1;
	return fnam1;
}
//---------------------------------------------------------------------------
UnicodeString get_GitDiffFile2(UnicodeString s)
{
	UnicodeString fnam2;
	UnicodeString fnam1 = get_GitDiffFiles(s, fnam2);
	return !fnam2.IsEmpty()? fnam2 : fnam1;
}
//---------------------------------------------------------------------------
