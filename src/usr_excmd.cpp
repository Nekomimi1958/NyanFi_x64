//----------------------------------------------------------------------//
// NyanFi																//
//  ExeCommands �p�֐�													//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <memory>
#include <System.DateUtils.hpp>
#include <System.IOUtils.hpp>
#include <Vcl.Clipbrd.hpp>
#include <RegularExpressions.hpp>
#include "usr_str.h"
#include "usr_file_ex.h"
#include "usr_wic.h"
#include "UserFunc.h"
#include "GenInfDlg.h"
#include "TxtViewer.h"
#include "MemoFrm.h"
#include "DebugFrm.h"
#include "InpExDlg.h"
#include "usr_excmd.h"

//---------------------------------------------------------------------------
// ExeCmdsList �N���X
//---------------------------------------------------------------------------
ExeCmdsList::ExeCmdsList(UnicodeString cmds)
{
	CmdRec  = NULL;
	CmdList = new TStringList();

	ListCount = 0;
	LineNo = 0;
	PC = 0;
	EndOfCmds = false;

	RepLevel = -1;
	IfCnt	 = 0;

	for (int i=0; i<MAX_REPEAT_LEVEL; i++) RepCnt[i] = RepTop[i] = 0;

	//������ǂݔ�΂�
	split_dsc(cmds);

	//�t�@�C������ǂݍ���
	bool loaded  = false;
	if (remove_top_AT(cmds)) {
		FileName = to_absolute_name(exclude_quot(cmds));
		if (!file_exists(FileName)) {
			ErrMsg = SysErrorMessage(ERROR_FILE_NOT_FOUND);
			return;
		}

		int idx = CmdFileList->IndexOf(FileName);
		//����
		if (idx!=-1) {
			CmdRec = (cmdf_rec*)CmdFileList->Objects[idx];
			if (CmdRec->noreload || WithinPastMilliSeconds(CmdRec->f_time, get_file_age(FileName), TimeTolerance)) {
				CmdList->Assign(CmdRec->cmd_list);
				loaded = true;
			}
			CmdRec->exe_count++;
		}
		//����
		else {
			CmdRec = new cmdf_rec;
			CmdRec->file_buf  = new TStringList();
			CmdRec->cmd_list  = new TStringList();
			CmdRec->f_time	  = get_file_age(FileName);
			CmdRec->exe_count = 1;
			CmdRec->noreload  = false;
			CmdFileList->AddObject(FileName, (TObject*)CmdRec);
		}

		if (!loaded) {
			CmdRec->file_buf->LoadFromFile(FileName);
			std::unique_ptr<TStringList> f_buf(new TStringList());
			f_buf->Assign(CmdRec->file_buf);
			//�s�ԍ���t��
			for (int i=0; i<f_buf->Count; i++) {
				UnicodeString lbuf = Trim(f_buf->Strings[i]);
				if (lbuf.IsEmpty() || StartsStr(';', lbuf)) continue;
				f_buf->Strings[i] = UnicodeString().sprintf(_T("��%u: %s"), i + 1, lbuf.c_str());
			}
			//�R�����g�A��s���폜/ �O����
			int i = 0;
			while (i<f_buf->Count) {
				UnicodeString lbuf = Trim(f_buf->Strings[i]);
				if (EndsText(" #noreload", lbuf)) {
					CmdRec->noreload = true;
					f_buf->Delete(i);
				}
				else if (lbuf.IsEmpty() || StartsStr(';', lbuf)) {
					f_buf->Delete(i);
				}
				else i++;
			}
			cmds = f_buf->Text;
		}
	}

	//�R�}���h���X�g���쐬
	if (!loaded) {
		cmds = ReplaceStr(cmds, "\t",   " ");
		cmds = ReplaceStr(cmds, ":\\",  "\f");	//��؂蕶���Ƃ̍���������邽�߈ꎞ�u��
		cmds = ReplaceStr(cmds, "\r\n", ":");

		UnicodeString c_buf;
		int p = 1;
		bool in_qut = false;
		while (p<=cmds.Length()) {
			WideChar c = cmds[p];
			//���p���O
			if (!in_qut) {
				if (c==':') {
					c_buf = Trim(ReplaceStr(c_buf, "\f", ":\\"));
					if (!c_buf.IsEmpty()) CmdList->Add(c_buf);
					c_buf = EmptyStr;
				}
				else {
					if (c=='\"') in_qut = true;
					c_buf += c;
				}
			}
			//���p����
			else {
				if (c=='\"') in_qut = false;
				c_buf += c;
			}
			p++;
		}
		c_buf = Trim(ReplaceStr(c_buf, "\f", ":\\"));
		if (!c_buf.IsEmpty()) CmdList->Add(c_buf);

		if (CmdRec) CmdRec->cmd_list->Assign(CmdList);
	}

	ListCount = CmdList->Count;
	if (ListCount==0) ErrMsg = "�L���ȃR�}���h������܂���";
}
//---------------------------------------------------------------------------
ExeCmdsList::~ExeCmdsList()
{
	delete CmdList;
};

//---------------------------------------------------------------------------
//�O�����A�R�}���h/�p�����[�^�Ǝ��o��
//---------------------------------------------------------------------------
bool ExeCmdsList::Preproc(UnicodeString &cmd, UnicodeString &prm)
{
	CmdStr = CmdList->Strings[PC];
	//�s�ԍ�
	if (remove_top_s(CmdStr, _T('��'))) {
		LineNo = CmdStr.ToIntDef(0);
		IncPC();
		return false;
	}
	//���x��
	if (StartsStr('#', CmdStr)) {
		if (RepLevel!=-1) UserAbort(USTR_BadStatmet);
		IncPC();
		return false;
	}

	//�R�}���h�A�p�����[�^
	cmd = get_CmdStr(CmdStr);
	prm = get_PrmStr(CmdStr);
	if (!USAME_TI(cmd, "ReplaceBuffer")) prm = exclude_quot(prm);

	if (USAME_TI(cmd, "Exit")) EndOfCmds = true;

	return !EndOfCmds;
}
//---------------------------------------------------------------------------
bool ExeCmdsList::IncPC()
{
	if (PC < ListCount-1) {
		PC++;
		return true;
	}

	EndOfCmds = true;
	return false;
}

//---------------------------------------------------------------------------
bool ExeCmdsList::proc_Repeat(UnicodeString cmd, UnicodeString prm)
{
	bool handled = true;

	try {
		int idx = idx_of_word_i(_T("Repeat|EndRepeat|Break|Continue"), cmd);
		switch (idx) {
		case 0:	//Repeat
			if (RepLevel>=(MAX_REPEAT_LEVEL - 1)) TextAbort(_T("����ȏ㑽�d���ł��܂���B"));
			RepLevel++;
			if (prm.IsEmpty()) {
				RepCnt[RepLevel] = -2;
			}
			else if (USAME_TI(prm, "YN")) {
				RepCnt[RepLevel] = -1;
			}
			else {
				if (USAME_TI(prm, "IN")) prm = inputbox_ex(_T("Repeat"), _T("�J��Ԃ���"), EmptyStr, true);
				RepCnt[RepLevel] = prm.ToIntDef(0);
				if (RepCnt[RepLevel]==0) SysErrAbort(ERROR_INVALID_PARAMETER);
			}
			RepTop[RepLevel] = ++PC;
			break;

		case 1:	//EndRepeat
			if (RepLevel==-1) UserAbort(USTR_BadStatmet);
			if (RepCnt[RepLevel]>0) {
				RepCnt[RepLevel]--;
			}
			else if (RepCnt[RepLevel]==-1 && !msgbox_Sure(_T("�J��Ԃ��܂���?"))) {
				RepCnt[RepLevel] = 0;
			}
			if (RepCnt[RepLevel]!=0)
				PC = RepTop[RepLevel];
			else {
				RepLevel--;  IncPC();
			}
			break;

		case 2: case 3:	//Break, Continue
			if (RepLevel==-1) UserAbort(USTR_BadStatmet);
			if (!search_EndRepeat()) UserAbort(USTR_BadStatmet);
			if (idx==2) {
				RepLevel--;  IncPC();
			}
			break;
		default:
			handled = false;
		}
	}
	catch (EAbort &e) {
		ErrMsg = e.Message;
	}

	return handled;
}

//---------------------------------------------------------------------------
//EndRepeat ��T��
//---------------------------------------------------------------------------
bool ExeCmdsList::search_EndRepeat()
{
	int end_pc = -1;
	int level  = 0;
	for (int i=PC; i<CmdList->Count; i++) {
		UnicodeString cmd = get_CmdStr(CmdList->Strings[i]);
		if (USAME_TI(cmd, "Repeat")) {
			level++;
		}
		else if (USAME_TI(cmd, "EndRepeat")) {
			if (level==0) {
				end_pc = i; break;
			}
			else {
				level--;
			}
		}
	}

	if (end_pc==-1) return false;

	PC = end_pc;
	return true;
}
//---------------------------------------------------------------------------
bool ExeCmdsList::proc_Goto(UnicodeString label)
{
	PC = -1;
	for (int i=0; i<CmdList->Count; i++)
		if (SameText("#" + label, CmdList->Strings[i])) { PC = i; break; }

	if (PC==-1) {
		ErrMsg.sprintf(_T("���x�� %s ��������܂���"), label.c_str());
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
bool ExeCmdsList::proc_IfCnd(bool cnd, UnicodeString prm)
{
	try {
		if (cnd) {
			//�p�����[�^����
			if (!prm.IsEmpty()) {
				if (USAME_TI(prm, "Exit")) {
					EndOfCmds = true;
				}
				else if (contained_wd_i(_T("Break|Continue"), prm)) {
					if (RepLevel==-1) UserAbort(USTR_BadStatmet);
					if (!search_EndRepeat()) UserAbort(USTR_BadStatmet);
					if (USAME_TI(prm, "Break")) {
						RepLevel--;  IncPC();
					}
				}
				else if (StartsText("Goto", prm)) {
					if (!proc_Goto(get_tkn_r(prm, '_'))) throw EAbort(ErrMsg);
				}
				else UserAbort(USTR_SyntaxError);
			}
			//�p�����[�^�Ȃ�
			else {
				IfCnt++;  IncPC();
			}
		}
		//�U
		else {
			IncPC();
			if (!EndOfCmds && prm.IsEmpty()) {
				IfCnt++;
				int if_lvl = 0;
				while (!EndOfCmds) {
					UnicodeString lbuf = CmdList->Strings[PC];
					if (StartsText("If", lbuf) && !ContainsStr(lbuf, "_")) {
						if_lvl++;
					}
					else {
						if (if_lvl==0) {
							if (contained_wd_i(_T("Else|EndIf"), lbuf)) break;
						}
						else if (USAME_TI(lbuf, "EndIf")) {
							if_lvl--;
						}
					}
					IncPC();
				}
				if (EndOfCmds) UserAbort(USTR_BadStatmet);
				IncPC();
			}
		}
		return true;
	}
	catch (EAbort &e) {
		ErrMsg = e.Message;
		return false;
	}
}
//---------------------------------------------------------------------------
bool ExeCmdsList::proc_IfElseEnd(UnicodeString cmd)
{
	if (USAME_TI(cmd, "Else")) {
		if (IfCnt==0) UserAbort(USTR_BadStatmet);
		IncPC();
		int if_lvl = 0;
		while (!EndOfCmds) {
			UnicodeString lbuf = CmdList->Strings[PC];
			if (StartsText("If", lbuf) && !ContainsStr(lbuf, "_")) {
				if_lvl++;
			}
			else if (USAME_TI(lbuf, "EndIf")) {
				if (if_lvl==0) break;
				if_lvl--;
			}
			IncPC();
		}
		if (EndOfCmds) UserAbort(USTR_BadStatmet);
		IncPC();
	}
	else if (USAME_TI(cmd, "EndIf")) {
		if (IfCnt==0) UserAbort(USTR_BadStatmet);
		IfCnt--;  IncPC();
	}
	else return false;

	return true;
}

//---------------------------------------------------------------------------
//�⏕�R�}���h���X�g
const UnicodeString XCMD_SubCmds =
	"ActivateWnd\n"
	"Add\n"
	"AppendBuffer\n"
	"CallCommands\n"
	"Debug\n"
	"Download\n"
	"Echo\n"
	"EchoOff\n"
	"EchoOn\n"
	"EchoT\n"
	"Edit\n"
	"FileExists\n"
	"FilterBuffer\n"
	"FlashWin\n"
	"Format\n"
	"FormatDI\n"
	"FormatDT\n"
	"FormatFN\n"
	"Git\n"
	"GrepSetMask\n"
	"GrepStart\n"
	"Hint\n"
	"Input\n"
	"Ins\n"
	"JumpCommands\n"
	"ListBuffer\n"
	"ListVar\n"
	"LoadBuffer\n"
	"MatchBuffer\n"
	"MatchDir\n"
	"MatchExt\n"
	"MatchName\n"
	"Mid\n"
	"MsgBox\n"
	"MsgBoxYN\n"
	"MsgBoxYNC\n"
	"MsgOff\n"
	"Nop\n"
	"PlaySound\n"
	"PopIndex\n"
	"PopTopIndex\n"
	"PopupMenu\n"
	"PopVar\n"
	"PushIndex\n"
	"PushTopIndex\n"
	"PushVar\n"
	"Random\n"
	"ReadINI\n"
	"Redraw\n"
	"ReplaceBuffer\n"
	"ReturnCommands\n"
	"Right\n"
	"SaveBuffer\n"
	"SearchDown\n"
	"SearchDownD\n"
	"SearchDownF\n"
	"SearchUp\n"
	"SearchUpD\n"
	"SearchUpF\n"
	"SendCommands\n"
	"Set\n"
	"SetClipboard\n"
	"SetCopyMode\n"
	"SetExeDir\n"
	"SetExeFile\n"
	"SetExeOpt\n"
	"SetExeParam\n"
	"SetFileTime\n"
	"SetIndex\n"
	"ShellExecute\n"
	"Sleep\n"
	"Test\n"
	"Timer\n"
	"Trim\n"
	"WaitForKey\n"
	"WaitForTask\n"
	"WaitForTask2\n"
	"Warn\n"
	"WriteINI\n";

//��`�ςݕϐ��̖��O���X�g
const UnicodeString XCMD_VarNames =
	"UserName|ExePath|TmpPath|CurPath|OppPath|FileName|NamePart|BaseName|DirName|DownloadName|FileSize|FileSizeF|"
	"FileTime|FileTimeF|Date|Time|TabNo|FileCount|DirCount|SelCount|SelFileCount|SelDirCount|VolumeLabel|ScrMode|"
	"SortMode|SelText|LineNo|LineText|CursorPosY|CursorPosX|PageSize|Battery|TaskCount|MatchedStr|CodePage|MenuIndex|"
	"ExitCode|WinWidth|WinHeight|Clipboard|Buffer|BufferLength|BufferCount|BufferIndex|BufferLine|CurFiles|SelFiles|"
	"ModalResult|ThisName";

//---------------------------------------------------------------------------
//ExeCommands �̏���
//---------------------------------------------------------------------------
bool XCMD_IsBusy;				//ExeCommands ���s�� (NyanFiForm->ExeCmdsBusy�𔽉f)
bool XCMD_Aborted;				//ExeCommands ���f�v��
bool XCMD_MsgOff;				//�m�F�Ȃ�
bool XCMD_BufChanged;			//Buffer ���ύX���ꂽ
bool XCMD_FileChanged;			//�J�����g�t�@�C�����ύX���ꂽ

TForm *LastModalForm = NULL;	//�Ō�ɊJ���ꂽ���[�_���t�H�[��
TModalResult XCMD_ModalResult;

bool XCMD_Debugging;			//�f�o�b�O���[�h
bool XCMD_Debug_Step, XCMD_Debug_Go, XCMD_Debug_List, XCMD_Debug_Exit, XCMD_Debug_Quit, XCMD_Debug_Help;

TStringList *XCMD_XList;			//�R�}���h���X�g�̃��X�g
TStringList *XCMD_WatchList;		//�Ď��ϐ����X�g
TStringList *XCMD_IdxStack;			//�C���f�b�N�X�E�X�^�b�N
TStringList *XCMD_TopIdxStack;		//�g�b�v�C���f�b�N�X�E�X�^�b�N
TStringList *XCMD_VarStack;			//�ϐ��X�^�b�N

ExeCmdsList *XCMD_xlp;
UnicodeString XCMD_cmd, XCMD_prm;

int  XCMD_last_task_id;
int  XCMD_tim_cnt;
int  XCMD_start_cnt;
TDateTime XCMD_tim_t;
bool XCMD_echo_on, XCMD_echo_lno, XCMD_view_log, XCMD_view_clip;
file_rec *XCMD_cfp;

UnicodeString XCMD_cur_f_name, XCMD_cur_r_name;
UnicodeString XCMD_cur_fext, XCMD_cur_bnam, XCMD_cur_path;
bool XCMD_is_dir, XCMD_is_file, XCMD_selected, XCMD_is_virtual, XCMD_is_img, XCMD_is_log;
bool XCMD_is_bin, XCMD_is_htm;
bool XCMD_matched, XCMD_found, XCMD_marked;
bool XCMD_is_top, XCMD_is_end, XCMD_is_sel;
bool XCMD_fromGrep;
TModalResult XCMD_box_res;

bool XCMD_chg_CodePage;			//�R�[�h�y�[�W���ύX���ꂽ(TV���[�h�ł̑΍�)

__int64   XCMD_f_size;
TDateTime XCMD_f_time;

//---------------------------------------------------------------------------
//������
//---------------------------------------------------------------------------
void XCMD_Initialize(UnicodeString &opt)
{
	XCMD_XList		 = new TStringList();
	XCMD_WatchList	 = new TStringList();
	XCMD_IdxStack	 = new TStringList();
	XCMD_TopIdxStack = new TStringList();
	XCMD_VarStack	 = new TStringList();

	//�ϐ����X�g������
	int i = 0;
	while (i<XCMD_VarList->Count) {
		if (StartsStr("..", XCMD_VarList->Strings[i])) i++; else XCMD_VarList->Delete(i);
	}

	XCMD_set_Var(_T("CodePage"),	932);
	XCMD_chg_CodePage = false;

	XCMD_set_Var(_T("UserName"),	UserName);
	XCMD_set_Var(_T("ExePath"),		ExePath);
	XCMD_set_Var(_T("TmpPath"),		TempPathA);
	XCMD_set_Var(_T("BufferIndex"),	0);
	XCMD_set_Var(_T("ModalResult"),	(int)mrNone);

	//���łɎ��s���^�X�N�̍ő�ID���擾
	XCMD_last_task_id = -1;
	for (int i=0; i<MAX_TASK_THREAD; i++) {
		TTaskThread *tp = TaskThread[i];
		if (tp && tp->ID>XCMD_last_task_id) XCMD_last_task_id = tp->ID;
	}

	XCMD_tim_cnt	= 0;
	XCMD_start_cnt	= 0;
	XCMD_tim_t		= 0.0;
	XCMD_BufChanged = true;
	XCMD_echo_on	= XCMD_echo_lno = XCMD_view_log = XCMD_view_clip = false;
	XCMD_f_size		= -1;
	XCMD_f_time		= 0;

	if (USAME_TI(opt, "EL")) {
		XCMD_echo_on = true;  XCMD_echo_lno = true;
		opt = EmptyStr;
	}
	else if (USAME_TI(opt, "ViewLog")) {
		XCMD_view_log = true;
		opt = EmptyStr;
	}
	else if (USAME_TI(opt, "ViewClip")) {
		XCMD_view_clip = true;
		opt = EmptyStr;
	}

	XCMD_cfp = NULL;
	XCMD_FileChanged = false;

	XCMD_matched  = false;
	XCMD_fromGrep = false;

	XCMD_Debugging = XCMD_Debug_Step = XCMD_Debug_Go = XCMD_Debug_List = XCMD_Debug_Exit = XCMD_Debug_Quit = XCMD_Debug_Help = false;
}

//---------------------------------------------------------------------------
//�I������
//---------------------------------------------------------------------------
void XCMD_Uninitialize()
{
	del_file_rec(XCMD_cfp);

	for (int i=0; i<XCMD_XList->Count; i++) delete (ExeCmdsList*)XCMD_XList->Objects[i];
	delete XCMD_XList;
	XCMD_XList = NULL;
	XCMD_xlp   = NULL;

	delete XCMD_WatchList;		XCMD_WatchList	 = NULL;
	delete XCMD_IdxStack;		XCMD_IdxStack	 = NULL;
	delete XCMD_TopIdxStack;	XCMD_TopIdxStack = NULL;
	delete XCMD_VarStack;		XCMD_VarStack	 = NULL;
}

//---------------------------------------------------------------------------
void XCMD_set_Var(const _TCHAR *name, UnicodeString v)
{
	XCMD_VarList->Values[name] = v;
}
//---------------------------------------------------------------------------
void XCMD_set_Var(const _TCHAR *name, int v)
{
	XCMD_VarList->Values[name] = v;
}

//---------------------------------------------------------------------------
//�R�}���h���X�g��ǉ�
//---------------------------------------------------------------------------
ExeCmdsList *XCMD_AddCmdsList(UnicodeString cmds, bool is_call)
{
	XCMD_xlp = new ExeCmdsList(cmds);

	if (is_call) {
		XCMD_XList->AddObject(XCMD_xlp->FileName, (TObject*)XCMD_xlp);
	}
	else {
		for (int i=0; i<XCMD_XList->Count; i++) delete (ExeCmdsList*)XCMD_XList->Objects[i];
		XCMD_XList->Clear();
		XCMD_XList->AddObject(XCMD_xlp->FileName, (TObject*)XCMD_xlp);
	}

	return XCMD_xlp;
}
//---------------------------------------------------------------------------
//�Ăяo�����֖߂�
//---------------------------------------------------------------------------
ExeCmdsList *XCMD_Return()
{
	if (XCMD_XList->Count>1) {
		int idx = XCMD_XList->Count - 1;
		delete (ExeCmdsList*)XCMD_XList->Objects[idx];
		XCMD_XList->Delete(idx--);
		XCMD_xlp = (ExeCmdsList*)XCMD_XList->Objects[idx];
		XCMD_xlp->IncPC();
		return XCMD_xlp;
	}

	if (XCMD_XList->Count>0) {
		XCMD_xlp = (ExeCmdsList*)XCMD_XList->Objects[0];
		XCMD_xlp->EndOfCmds = true;
		return XCMD_xlp;
	}

	return NULL;
}

//---------------------------------------------------------------------------
//�J�����g�̃t�@�C�����R�[�h��ݒ�
//---------------------------------------------------------------------------
file_rec *XCMD_set_cfp(UnicodeString fnam, UnicodeString cnam, file_rec *cfp)
{
	del_file_rec(XCMD_cfp);

	int v_idx = (ScrMode==SCMD_IVIEW)? ViewFileList->IndexOf(ViewFileName) : -1;

	if (XCMD_view_log || XCMD_view_clip) {
		XCMD_cfp = NULL;
	}
	else if (!fnam.IsEmpty()) {
		XCMD_cfp = cre_new_file_rec(fnam);
	}
	else {
		switch (ScrMode) {
		case SCMD_TVIEW:
			XCMD_cfp = cre_new_file_rec(TxtViewer->FileName);
			break;
		case SCMD_IVIEW:
			if (v_idx!=-1)
				XCMD_cfp = cre_new_file_rec((file_rec*)ViewFileList->Objects[v_idx]);
			else
				XCMD_cfp = cre_new_file_rec(ViewFileName);
			break;
		default:
			XCMD_cfp = cre_new_file_rec(cfp);
		}
	}

	if (is_selectable(XCMD_cfp)) {
		XCMD_cur_f_name = (XCMD_cfp->is_virtual && SetTmpFile(XCMD_cfp, true))? XCMD_cfp->tmp_name : XCMD_cfp->f_name;
		XCMD_cur_r_name = XCMD_cfp->r_name;
		XCMD_cur_fext	= XCMD_cfp->f_ext;
		XCMD_cur_bnam	= XCMD_cfp->b_name;
		XCMD_is_dir 	= XCMD_cfp->is_dir;
		XCMD_is_file	= !XCMD_cfp->is_dir;
		XCMD_selected	= XCMD_cfp->selected;
		XCMD_is_virtual = XCMD_cfp->is_virtual;
		XCMD_is_img 	= XCMD_is_file && is_Viewable(XCMD_cfp);
		XCMD_is_log 	= str_match(ExePath + "tasklog*.txt", XCMD_cur_f_name);
		XCMD_f_size		= XCMD_cfp->f_size;
		XCMD_f_time		= XCMD_cfp->f_time;
	}
	else {
		XCMD_cur_f_name = XCMD_cur_r_name = XCMD_cur_fext = XCMD_cur_bnam = EmptyStr;
		XCMD_is_dir = XCMD_is_file = XCMD_selected = XCMD_is_virtual = XCMD_is_img = false;
		XCMD_is_log = XCMD_view_log;
		XCMD_f_size = -1;
		XCMD_f_time = 0;
	}

	//�J�����g�̍X�V
	XCMD_cur_path = (ScrMode==SCMD_FLIST)? cnam : ExtractFilePath(XCMD_cur_f_name);
	if (!SameText(XCMD_cur_path, XCMD_VarList->Values["CurPath"])) {
		XCMD_set_Var(_T("CurPath"), XCMD_cur_path);
		XCMD_set_Var(_T("VolumeLabel"), CurStt->is_FTP? EmptyStr : get_VolumeInfo(XCMD_cur_path));
	}

	//�t�@�C�����Ȃǂ��X�V
	if (!SameText(XCMD_cur_f_name, XCMD_VarList->Values["FileName"])) {
		//��`�ςݕϐ��̍X�V
		XCMD_set_Var(_T("FileName"), XCMD_cur_f_name);
		XCMD_set_Var(_T("BaseName"), XCMD_cur_bnam);
		XCMD_set_Var(_T("DirName"),  ExtractFileDir(XCMD_cur_f_name));
		XCMD_set_Var(_T("NamePart"), ExtractFileName(XCMD_cur_f_name));
		XCMD_FileChanged = true;
	}

	//��`�ςݕϐ��̍X�V
	XCMD_set_Var(_T("FileSize"),	XCMD_f_size);
	XCMD_set_Var(_T("FileSizeF"),	get_FileSizeStr(XCMD_f_size));
	XCMD_set_Var(_T("FileTime"),	format_DateTime(XCMD_f_time));
	XCMD_set_Var(_T("FileTimeF"),	FormatDateTime(TimeStampFmt, XCMD_f_time));

	XCMD_is_bin = XCMD_is_htm = false;
	XCMD_found  = XCMD_marked = false;
	XCMD_is_top = XCMD_is_end = XCMD_is_sel = false;
	TListBox *lp  = FileListBox[CurListTag];

	switch (ScrMode) {
	case SCMD_FLIST:	//�t�@�C���[
		XCMD_is_top = (lp->ItemIndex == 0);
		XCMD_is_end = (lp->ItemIndex == lp->Count-1);
		XCMD_is_sel = (GetSelCount(GetCurList())>0);
		if (XCMD_view_log)
			XCMD_is_bin = false;
		else
			XCMD_is_bin = !XCMD_is_dir && (XCMD_is_virtual? test_NonTxtExt(XCMD_cur_fext) : !is_TextFile(XCMD_cur_f_name));
		XCMD_found	= CurStt->is_Find;
		XCMD_marked = IniFile->IsMarked(XCMD_cur_r_name);
		break;
	case SCMD_TVIEW:	//�e�L�X�g�r���A�[
		if (TxtViewer->isReady) {
			XCMD_is_top	= (TxtViewer->CurPos.y == 0);
			XCMD_is_end	= (TxtViewer->CurPos.y == TxtViewer->MaxDispLine-1);
			XCMD_is_sel	= (TxtViewer->SelStart != TxtViewer->SelEnd);
			XCMD_is_bin	= TxtViewer->isBinary;
			XCMD_is_htm	= TxtViewer->isHtm2Txt;
			XCMD_is_log = TxtViewer->isLog;
			XCMD_found	= TxtViewer->LastFound;
			XCMD_marked = TxtViewer->IsMarked();
		}
		break;
	case SCMD_IVIEW:	//�C���[�W�r���A�[
		XCMD_is_top	= (v_idx == 0);
		XCMD_is_end	= (v_idx == ViewFileList->Count-1);
		XCMD_is_sel	= (GetSelCount(GetCurList())>0);
		XCMD_is_bin = true;
		XCMD_marked	= IniFile->IsMarked(ViewFileName);
		break;
	}

	return XCMD_cfp;
}

//---------------------------------------------------------------------------
//�ϐ��̉���
//---------------------------------------------------------------------------
UnicodeString XCMD_eval_Var(UnicodeString prm)
{
	if (ContainsStr(prm, "%")) {
		for (int i=0; i<XCMD_VarList->Count; i++)
			prm = ReplaceText(prm, "%" + XCMD_VarList->Names[i] + "%", XCMD_VarList->ValueFromIndex[i]);
	}
	return prm;
}

//---------------------------------------------------------------------------
//��`�ςݕϐ��̍X�V
//---------------------------------------------------------------------------
void XCMD_upd_Var()
{
	XCMD_set_Var(_T("ThisName"),	XCMD_xlp? XCMD_xlp->FileName : EmptyStr);
	XCMD_set_Var(_T("Date"),		format_Date(Now()));
	XCMD_set_Var(_T("Time"),		FormatDateTime("hh:nn:ss", Now()));
	XCMD_set_Var(_T("TabNo"),		CurTabIndex + 1);
	XCMD_set_Var(_T("FileCount"),	CurStt->f_cnt);
	XCMD_set_Var(_T("DirCount"),	CurStt->d_cnt);
	XCMD_set_Var(_T("SelCount"),	GetSelCount(GetCurList()));
	XCMD_set_Var(_T("SelFileCount"),CurStt->sel_f_cnt);
	XCMD_set_Var(_T("SelDirCount"), CurStt->sel_d_cnt);
	XCMD_set_Var(_T("Battery"), 	Trim(get_BatteryPercentStr()));
	XCMD_set_Var(_T("SelText"), 	(ScrMode==SCMD_TVIEW && TxtViewer->isReady)? TxtViewer->get_SelText() : EmptyStr);
	XCMD_set_Var(_T("WinWidth"), 	Application->MainForm->Width);
	XCMD_set_Var(_T("WinHeight"), 	Application->MainForm->Height);
	XCMD_set_Var(_T("Clipboard"),	Clipboard()->AsText);
	XCMD_set_Var(_T("LastWatchLog"),LastWatchLog);
	XCMD_set_Var(_T("ModalResult"),	(int)XCMD_ModalResult);

	switch (ScrMode) {
	case SCMD_TVIEW:
		if (TxtViewer->isReady) {
			XCMD_set_Var(_T("LineNo"),		TxtViewer->get_CurLineNo());
			XCMD_set_Var(_T("LineText"),	TxtViewer->get_CurLine(true));
			XCMD_set_Var(_T("CursorPosY"),	TxtViewer->CurPos.Y);
			XCMD_set_Var(_T("CursorPosX"),	TxtViewer->CurPos.X);
			XCMD_set_Var(_T("PageSize"),	TxtViewer->LineCount + 1);

			if (!TxtViewer->isBinary && TxtViewer->TxtBufList->Encoding && !XCMD_chg_CodePage)
				XCMD_set_Var(_T("CodePage"), TxtViewer->TxtBufList->Encoding->CodePage);
		}
		break;
	default:
		XCMD_set_Var(_T("LineNo"),	  	0);
		XCMD_set_Var(_T("CursorPosY"),	0);
		XCMD_set_Var(_T("CursorPosX"),	0);
	}

	//��ʃ��[�h
	switch (ScrMode) {
	case SCMD_GREP:  XCMD_set_Var(_T("ScrMode"), "GR"); break;
	case SCMD_TVIEW: XCMD_set_Var(_T("ScrMode"), "TV"); break;
	case SCMD_IVIEW: XCMD_set_Var(_T("ScrMode"), "IV"); break;
	default:		 XCMD_set_Var(_T("ScrMode"), "FL");
	}

	//�\�[�g���[�h
	XCMD_set_Var(_T("SortMode"), get_word_i_idx(_T("F|E|D|S|A|U|L"), CurSortMode()));

	//�^�X�N��
	XCMD_set_Var(_T("TaskCount"), get_BusyTaskCount());

	//�t�@�C���̃��X�g
	std::unique_ptr<TStringList> flst(new TStringList());	//���ׂ�
	std::unique_ptr<TStringList> slst(new TStringList());	//�I��
	TStringList *lst = GetCurList();
	for (int i=0; i<lst->Count; i++) {
		file_rec *fp = (file_rec*)lst->Objects[i];
		if (fp->is_dummy || fp->is_up) continue;
		UnicodeString fnam = fp->f_name;
		if (fp->is_dir) fnam = IncludeTrailingPathDelimiter(fnam);
		flst->Add(fnam);
		if (fp->selected) slst->Add(fnam);
	}
	XCMD_set_Var(_T("CurFiles"), flst->Text);
	XCMD_set_Var(_T("SelFiles"), slst->Text);

	//Buffer
	if (XCMD_BufChanged) {
		std::unique_ptr<TStringList> b_buf(new TStringList());
		b_buf->Text = XCMD_GetBuffer();
		XCMD_set_Var(_T("BufferCount"),  b_buf->Count);
		XCMD_set_Var(_T("BufferLength"), b_buf->Text.Length());
		int idx = get_ListIntVal(XCMD_VarList, _T("BufferIndex"), -1);
		XCMD_set_Var(_T("BufferLine"),   (idx>=0 && idx<b_buf->Count)? b_buf->Strings[idx] : EmptyStr);
		XCMD_BufChanged = false;
	}

	if (!XCMD_prm.IsEmpty()) {
		//�ϐ��̉���
		XCMD_prm = XCMD_eval_Var(XCMD_prm);
		//���ϐ��̉���
		XCMD_prm = cv_env_str(XCMD_prm);
		if (ContainsStr(XCMD_prm, "%")) XCMD_prm = TRegEx::Replace(XCMD_prm, "%.+%", EmptyStr);
	}
}

//---------------------------------------------------------------------------
//�s�G�R�[
//---------------------------------------------------------------------------
void XCMD_EchoLn()
{
	if (XCMD_xlp && XCMD_echo_on) {
		UnicodeString msg;
		if (XCMD_echo_lno && XCMD_xlp->LineNo>0) msg.sprintf(_T("%3u: "), XCMD_xlp->LineNo);
		msg += XCMD_xlp->CmdStr;
		AddLog(msg);
	}
}

//---------------------------------------------------------------------------
//�f�o�b�O���̕\��
//---------------------------------------------------------------------------
void XCMD_ShowDebugInf(
	UnicodeString err)	//�G���[	(default = EmptyStr)
{
	if (!DebugForm || !DebugForm->Visible) return;

	if (XCMD_xlp && !XCMD_xlp->FileName.IsEmpty()) {
		cmdf_rec *cp = XCMD_xlp->CmdRec;
		if (cp) {
			//�v���r���[
			DebugForm->FileName = XCMD_xlp->FileName;
			DebugForm->SetPreview(cp->file_buf, XCMD_xlp->LineNo - 1);

			//�f�o�b�O���
			std::unique_ptr<TStringList> i_lst(new TStringList());
			i_lst->AddObject(ExtractFileName(XCMD_xlp->FileName),	(TObject*)LBFLG_STD_FINF);
			i_lst->AddObject(yen_to_delimiter(ExtractFilePath(XCMD_xlp->FileName)), (TObject*)LBFLG_STD_FINF);
			i_lst->AddObject(get_FileInfStr(XCMD_xlp->FileName),	(TObject*)LBFLG_STD_FINF);

			UnicodeString dbgstt = get_PropTitle(_T("�f�o�b�O���"));
			if (!err.IsEmpty()) {
				dbgstt.cat_sprintf(_T("�G���[�I�� [%s]"), err.c_str());
			}
			else {
				dbgstt.cat_sprintf(_T("%s"),
						!XCMD_Debugging? _T("----") :
					XCMD_xlp->EndOfCmds? _T("�I��") :
						  XCMD_Debug_Go? _T("���s��") : _T("��~��"));
			}
			i_lst->AddObject(dbgstt, (TObject*)LBFLG_DEBUG);

			//Repeat
			if (XCMD_xlp->RepLevel!=-1) {
				for (int i=0; i<=XCMD_xlp->RepLevel; i++) {
					dbgstt = get_PropTitle("Repeat" + IntToStr(i + 1));
					int r_cnt = XCMD_xlp->RepCnt[i];
					switch (r_cnt) {
					case -2: dbgstt += "����";	break;
					case -1: dbgstt += "Y/N";	break;
					default: dbgstt.cat_sprintf(_T("%3u"), r_cnt);
					}
					i_lst->Add(dbgstt);
				}
			}

			//�Ď��ϐ�
			if (XCMD_WatchList->Count>0) {
				i_lst->Add(EmptyStr);
				for (int i=0; i<XCMD_WatchList->Count; i++) {
					UnicodeString vnam = XCMD_WatchList->Strings[i];
					add_PropLine(vnam, XCMD_VarList->Values[vnam], i_lst.get());
				}
			}

			//���[�U��`�ϐ�
			if (XCMD_VarList->Count>0) {
				i_lst->Add(EmptyStr);
				XCMD_get_UsrVarList(i_lst.get());
				assign_InfListBox(DebugForm->ReferListBox, i_lst.get());
			}
		}
	}
}

//===========================================================================
//�I�ȉ��̊֐��ł́A�G���[�������� EAbort ��O�����o����܂��B
//===========================================================================

//---------------------------------------------------------------------------
//�_�����̕]��
//---------------------------------------------------------------------------
bool XCMD_EvalCnd(TStringDynArray lst)
{
	std::unique_ptr<TStringList> res_lst(new TStringList());
	for (int i=0; i<lst.Length; i++) res_lst->Add(lst[i]);

	//.and.
	for (;;) {
		int idx  = res_lst->IndexOf("and");	if (idx==-1) break;
		int idx0 = idx - 1;
		int idx1 = idx + 1;
		if (idx<0 || idx1>=res_lst->Count) UserAbort(USTR_SyntaxError);
		res_lst->Strings[idx0] = (equal_1(res_lst->Strings[idx0]) && equal_1(res_lst->Strings[idx1]))? "1" : "0";
		res_lst->Delete(idx); res_lst->Delete(idx);
	}

	//.or.
	for (;;) {
		int idx  = res_lst->IndexOf("or");	if (idx==-1) break;
		int idx0 = idx - 1;
		int idx1 = idx + 1;
		if (idx<0 || idx1>=res_lst->Count) UserAbort(USTR_SyntaxError);
		res_lst->Strings[idx0] = (equal_1(res_lst->Strings[idx0]) || equal_1(res_lst->Strings[idx1]))? "1" : "0";
		res_lst->Delete(idx);  res_lst->Delete(idx);
	}

	return equal_1(res_lst->Strings[0]);
}

//---------------------------------------------------------------------------
//If���̏���
//---------------------------------------------------------------------------
bool XCMD_Control()
{
	//If��
	if (remove_top_text(XCMD_cmd, _T("If"))) {
		TStringDynArray cnd_lst = SplitString(XCMD_cmd, ".");
		if (cnd_lst.Length%2==0) UserAbort(USTR_SyntaxError);

		for (int i=0; i<cnd_lst.Length; i++) {
			UnicodeString cnd = cnd_lst[i];
			if (USAME_TI(cnd, "and") || USAME_TI(cnd, "or")) {
				cnd_lst[i] = cnd.LowerCase();
				continue;
			}

			bool if_not = remove_top_text(cnd, _T("Not")) || remove_top_s(cnd, '!');
			bool cnd_res = false;

			switch (idx_of_word_i(
				_T("True|False|Selected|Dir|File|Arc|Sel|Top|End|Left|")					//0�`9
				_T("Right|Empty|Root|Remote|Removable|Virtual|Work|Found|Yes|No|")			//10�`19
				_T("Cancel|SelMask|PathMask|Task|Suspend|Binary|Log|HtmTxt|Marked|Image|")	//20�`29
				_T("Shift|Ctrl|Alt|Grep|Connected|Primary|Duplicated|FTP|ADS|Git|")			//30�`39
				_T("TimeOut"),																//40�`
				cnd))
			{
			case  0: cnd_res = XCMD_matched;							break;
			case  1: cnd_res = !XCMD_matched;							break;
			case  2: cnd_res = XCMD_selected;							break;
			case  3: cnd_res = XCMD_is_dir;								break;
			case  4: cnd_res = XCMD_is_file;							break;
			case  5: cnd_res = test_ArcExt(XCMD_cur_fext);				break;
			case  6: cnd_res = XCMD_is_sel;								break;
			case  7: cnd_res = XCMD_is_top;								break;
			case  8: cnd_res = XCMD_is_end;								break;
			case  9: cnd_res = (CurListTag==0);							break;
			case 10: cnd_res = (CurListTag==1);							break;
			case 11: cnd_res = (CurStt->f_cnt==0 && CurStt->d_cnt==0);	break;
			case 12: cnd_res = is_root_dir(XCMD_cur_path);				break;
			case 13: cnd_res = (get_drive_type(XCMD_cur_path)==DRIVE_REMOTE);		break;
			case 14: cnd_res = (get_drive_type(XCMD_cur_path)==DRIVE_REMOVABLE);	break;
			case 15: cnd_res = CurStt->is_Arc;							break;
			case 16: cnd_res = CurStt->is_Work;							break;
			case 17: cnd_res = XCMD_found;								break;
			case 18: cnd_res = (XCMD_box_res==mrYes);					break;
			case 19: cnd_res = (XCMD_box_res==mrNo);					break;
			case 20: cnd_res = (XCMD_box_res==mrCancel);				break;
			case 21: cnd_res = (SelMaskList[CurListTag]->Count>0);		break;
			case 22: cnd_res = !PathMask[CurListTag].IsEmpty();			break;
			case 23: cnd_res = (get_BusyTaskCount()>0);					break;
			case 24: cnd_res = RsvSuspended;							break;
			case 25: cnd_res = XCMD_is_bin;								break;
			case 26: cnd_res = XCMD_is_log;								break;
			case 27: cnd_res = XCMD_is_htm;								break;
			case 28: cnd_res = XCMD_marked;								break;
			case 29: cnd_res = XCMD_is_img;								break;
			case 30: cnd_res = is_KeyDown(VK_SHIFT);					break;
			case 31: cnd_res = is_KeyDown(VK_CONTROL);					break;
			case 32: cnd_res = is_KeyDown(VK_MENU);						break;
			case 33: cnd_res = XCMD_fromGrep;							break;
			case 34: cnd_res = InternetConnected();						break;
			case 35: cnd_res = IsPrimary;								break;
			case 36: cnd_res = IsDuplicated();							break;
			case 37: cnd_res = CurStt->is_FTP;							break;
			case 38: cnd_res = CurStt->is_ADS;							break;
			case 39: cnd_res = !get_GitTopPath(XCMD_cur_path).IsEmpty();	break;
			case 40:
				if (XCMD_tim_cnt>0)
					cnd_res = ((int)(GetTickCount()-XCMD_start_cnt) > XCMD_tim_cnt);
				else if (XCMD_tim_t>TDateTime(0))
					cnd_res = (CompareDateTime(Now(), XCMD_tim_t)==GreaterThanValue);
				break;

			default: UserAbort(USTR_SyntaxError);
			}

			//�ے�
			if (if_not) cnd_res = !cnd_res;

			cnd_lst[i].sprintf(_T("%s"), cnd_res? _T("1") : _T("0"));
		} //for

		if (!XCMD_xlp->proc_IfCnd(XCMD_EvalCnd(cnd_lst), XCMD_prm)) throw EAbort(XCMD_xlp->ErrMsg);
	}
	else if (XCMD_xlp->proc_IfElseEnd(XCMD_cmd)) {
		;
	}
	//Repeat��
	else if (XCMD_xlp->proc_Repeat(XCMD_cmd, XCMD_prm)) {
		if (!XCMD_xlp->ErrMsg.IsEmpty()) throw EAbort(XCMD_xlp->ErrMsg);
	}
	//Goto��
	else if (USAME_TI(XCMD_cmd, "Goto")) {
		if (!XCMD_xlp->proc_Goto(XCMD_prm)) throw EAbort(XCMD_xlp->ErrMsg);
	}
	else return false;

	return true;
}

//---------------------------------------------------------------------------
//������}�b�`
//---------------------------------------------------------------------------
void XCMD_match_Str(UnicodeString prm, UnicodeString s)
{
	XCMD_set_Var(_T("MatchedStr"), ptn_match_str(prm, s));
	XCMD_matched = !ListVal_is_empty(XCMD_VarList, _T("MatchedStr"));
}
//---------------------------------------------------------------------------
void XCMD_match_Buffer(UnicodeString prm)
{
	std::unique_ptr<TStringList> b_buf(new TStringList());
	b_buf->Text = XCMD_GetBuffer();

	UnicodeString rstr;
	for (int i=0; i<b_buf->Count && rstr.IsEmpty(); i++) {
		rstr = ptn_match_str(prm, b_buf->Strings[i]);
		if (!rstr.IsEmpty()) {
			XCMD_set_Var(_T("MatchedStr"),	rstr);
			XCMD_set_Var(_T("BufferIndex"),	i);
			XCMD_set_Var(_T("BufferLine"),	b_buf->Strings[i]);
		}
	}
	XCMD_matched = !rstr.IsEmpty();
}
//---------------------------------------------------------------------------
void XCMD_MatchExt(UnicodeString prm, UnicodeString fext)
{
	XCMD_matched = false;
	TStringDynArray ext_lst = split_strings_semicolon(prm);
	for (int i=0; i<ext_lst.Length && !XCMD_matched; i++) XCMD_matched = test_FileExt(fext, ext_lst[i]);
	if (XCMD_matched) XCMD_set_Var(_T("MatchedStr"), fext);
}

//---------------------------------------------------------------------------
//Set : �ϐ��̐ݒ�
//---------------------------------------------------------------------------
void XCMD_Set(UnicodeString prm)
{
	TMatch mt = TRegEx::Match(prm, "[\\+\\-\\*/%]*=");
	if (!mt.Success) UserAbort(USTR_SyntaxError);
	UnicodeString opstr = mt.Value;
	UnicodeString vnam	= get_tkn(prm, opstr);
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	UnicodeString vstr	= XCMD_VarList->Values[vnam];
	UnicodeString nstr	= exclude_quot(get_tkn_r(prm, opstr));
	UnicodeString rstr;
	//�������Z�q
	if (!USAME_TS(opstr, "=")) {
		//���̕ϐ��̏������`�F�b�N
		int t_flag = -1;
		if (is_match_regex(vstr, _T("\\d{4}/\\d{2}/\\d{2}\\s\\d{2}:\\d{2}:\\d{2}")))
			t_flag = 0;
		else if (is_match_regex(vstr, _T("\\d{4}/\\d{2}/\\d{2}")))
			t_flag = 1;
		else if (is_match_regex(vstr, _T("\\d{2}:\\d{2}:\\d{2}")))
			t_flag = 2;

		//���ԉ��Z
		if (t_flag!=-1) {
			try {
				TDateTime vt;
				if (t_flag==1)
					vt = TDate(vstr);
				else if (t_flag==2)
					vt = TTime(vstr);
				else
					vt = TDateTime(vstr);

				TDateTime nt;
				if (is_match_regex(nstr, _T("\\d{4}/\\d{2}/\\d{2}\\s\\d{2}:\\d{2}:\\d{2}")))
					nt = TDateTime(nstr);
				else if (is_match_regex(nstr, _T("\\d{4}/\\d{2}/\\d{2}")))
					nt = TDate(nstr);
				else if (is_match_regex(nstr, _T("\\d{2}:\\d{2}:\\d{2}")))
					nt = TTime(nstr);
				else
					nt = TDate(nstr.ToIntDef(0));

				//���Z
				if		(USAME_TS(opstr, "+=")) vt += nt;
				else if (USAME_TS(opstr, "-=")) vt -= nt;
				else UserAbort(USTR_SyntaxError);

				//���ʂ̐ݒ�
				if		(t_flag==0) rstr = format_DateTime(vt);
				else if (t_flag==1) rstr = format_Date(vt);
				else if (t_flag==2) rstr = FormatDateTime("hh:nn:ss", vt);
			}
			catch (EConvertError &e) {
				t_flag = -1;
			}
		}
		//���ԈȊO�̉��Z
		if (t_flag==-1) {
			try {
				//�������Z
				if (!vstr.IsEmpty() && !nstr.IsEmpty()) { 
					int v = vstr.ToInt();
					int n = nstr.ToInt();
					if (USAME_TS(opstr, "/=") && n==0) SysErrAbort(DISP_E_DIVBYZERO);

					switch (idx_of_word_i(_T("+=|-=|*=|/=|%="), opstr)) {
					case 0: v += n; break;
					case 1: v -= n; break;
					case 2: v *= n; break;
					case 3: v /= n; break;
					case 4: v %= n; break;
					}

					rstr.sprintf(_T("%0*d"), vstr.Length(), v);
				}
				else if (USAME_TS(opstr, "+=")) {
					rstr = vstr + nstr;
				}
			}
			catch (EConvertError &e) {
				if (USAME_TS(opstr, "+="))
					rstr = vstr + nstr;
				else
					UserAbort(USTR_SyntaxError);
			}
		}
	}
	//���
	else rstr = nstr;

	//�ϐ��X�V
	XCMD_VarList->Values[vnam] = rstr;
	if (USAME_TI(vnam, "CodePage")) XCMD_chg_CodePage = true;

	//Buffer�X�V
	if (contained_wd_i(_T("Buffer|BufferIndex"), vnam)) {
		XCMD_BufChanged = true;
	}
	else if (USAME_TI(vnam, "BufferLine")) {
		std::unique_ptr<TStringList> b_buf(new TStringList());
		b_buf->Text = XCMD_GetBuffer();
		int idx = get_ListIntVal(XCMD_VarList, _T("BufferIndex"), -1);
		if (idx>=0 && idx<b_buf->Count) {
			b_buf->Strings[idx] = rstr;
			XCMD_BufChanged = true;
		}
	}
}

//---------------------------------------------------------------------------
//Add : �ϐ��֍s�ǉ�
//---------------------------------------------------------------------------
void XCMD_Add(UnicodeString prm)
{
	if (!ContainsStr(prm, "<<")) UserAbort(USTR_SyntaxError);
	std::unique_ptr<TStringList> v_buf(new TStringList());
	UnicodeString vnam = get_tkn(prm, _T("<<"));
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	v_buf->Text = XCMD_VarList->Values[vnam];
	UnicodeString lbuf = exclude_quot(get_tkn_r(prm, _T("<<")));
	while (ends_tchs(_T("\r\n"), lbuf)) delete_end(lbuf);
	v_buf->Add(lbuf);
	XCMD_VarList->Values[vnam] = v_buf->Text;
	if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
}
//---------------------------------------------------------------------------
//Ins : �ϐ��̐擪�ɍs�}��
//---------------------------------------------------------------------------
void XCMD_Ins(UnicodeString prm)
{
	if (!ContainsStr(prm, "<<")) UserAbort(USTR_SyntaxError);
	std::unique_ptr<TStringList> v_buf(new TStringList());
	UnicodeString vnam = get_tkn(prm, _T("<<"));
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	v_buf->Text = XCMD_VarList->Values[vnam];
	UnicodeString lbuf = exclude_quot(get_tkn_r(prm, _T("<<")));
	while (ends_tchs(_T("\r\n"), lbuf)) delete_end(lbuf);
	v_buf->Insert(0, lbuf);
	XCMD_VarList->Values[vnam] = v_buf->Text;
	if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
}

//---------------------------------------------------------------------------
//Mid : ����������̎擾
//---------------------------------------------------------------------------
void XCMD_Mid(UnicodeString prm)
{
	TStringDynArray prmlst = SplitString(prm, ";");
	if (prmlst.Length<2) UserAbort(USTR_SyntaxError);
	UnicodeString vnam = prmlst[0];
	if (vnam.IsEmpty())  UserAbort(USTR_SyntaxError);
	UnicodeString vstr = XCMD_VarList->Values[vnam];

	int n1 = prmlst[1].ToIntDef(0);
	if (n1<=0) UserAbort(USTR_SyntaxError);
	if (n1<=vstr.Length()) {
		int n2;
		if (prmlst.Length>2 && !prmlst[2].IsEmpty()) {
			n2 = prmlst[2].ToIntDef(0);
			if (n2==0) UserAbort(USTR_SyntaxError);
		}
		else {
			n2 = vstr.Length() - n1 + 1;
		}
		vstr = vstr.SubString(n1, n2);
		XCMD_VarList->Values[vnam] = vstr;
		if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
	}
}
//---------------------------------------------------------------------------
void XCMD_Right(UnicodeString prm)
{
	TStringDynArray prmlst = SplitString(prm, ";");
	if (prmlst.Length<2) UserAbort(USTR_SyntaxError);
	UnicodeString vnam = prmlst[0];
	if (vnam.IsEmpty())  UserAbort(USTR_SyntaxError);
	UnicodeString vstr = XCMD_VarList->Values[vnam];

	int n2 = prmlst[1].ToIntDef(0);
	if (n2<=0) UserAbort(USTR_SyntaxError);
	int n1 = vstr.Length() - n2 + 1;
	if (n1>1) {
		vstr = vstr.SubString(n1, n2);
		XCMD_VarList->Values[vnam] = vstr;
		if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
	}
}
//---------------------------------------------------------------------------
void XCMD_Trim(UnicodeString prm)
{
	UnicodeString vnam = prm;
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	XCMD_VarList->Values[vnam] = Trim(XCMD_VarList->Values[vnam]);
	if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
}

//---------------------------------------------------------------------------
//Input : ���[�U��`�ϐ��̓���
//---------------------------------------------------------------------------
void XCMD_Input(UnicodeString prm)
{
	UnicodeString tit, msg, vnam;
	if (remove_top_s(prm, ':')) {
		prm = ReplaceStr(prm, ":\\", "\f");	//��؂�Ƃ̍���������邽�߈ꎞ�u��
		TStringDynArray prmlst = SplitString(prm, ":");
		if (prmlst.Length>0) vnam = prmlst[prmlst.Length - 1];
		if (prmlst.Length>1) tit  = ReplaceStr(prmlst[0], "\f", ":\\");
		if (prmlst.Length>2) msg  = ReplaceStr(prmlst[1], "\f", ":\\");
	}
	else {
		vnam = prm;
	}

	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	if (msg.IsEmpty())  msg = vnam + "=";

	UnicodeString vstr = XCMD_VarList->Values[vnam];

	if (input_query_ex(def_if_empty(tit, _T("���[�U��`�ϐ��̓���")).c_str(), msg.c_str(), &vstr)) {
		XCMD_VarList->Values[vnam] = vstr;
		XCMD_box_res = mrYes;
	}
	else {
		XCMD_box_res = mrCancel;
	}

	if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
}
//---------------------------------------------------------------------------
//Edit : ���[�U��`�ϐ��̕ҏW
//---------------------------------------------------------------------------
void XCMD_Edit(UnicodeString prm)
{
	UnicodeString tit, msg, vnam;
	if (remove_top_s(prm, ':')) {
		prm = ReplaceStr(prm, ":\\", "\f");	//��؂�Ƃ̍���������邽�߈ꎞ�u��
		TStringDynArray prmlst = SplitString(prm, ":");
		if (prmlst.Length>0) vnam = prmlst[prmlst.Length - 1];
		if (prmlst.Length>1) tit  = ReplaceStr(prmlst[0], "\f", ":\\");
	}
	else {
		vnam = prm;
	}

	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);

	if (!MemoForm) MemoForm = new TMemoForm(Application->MainForm);	//����ɓ��I�쐬
	MemoForm->Caption = def_if_empty(tit, _T("���[�U��`�ϐ��̕ҏW"));
	MemoForm->LinesBuff->Text = XCMD_VarList->Values[vnam];
	if (MemoForm->ShowModal()==mrOk) {
		XCMD_VarList->Values[vnam] = MemoForm->LinesBuff->Text;
		XCMD_box_res = mrYes;
	}
	else {
		XCMD_box_res = mrCancel;
	}

	if (USAME_TI(vnam, "Buffer")) XCMD_BufChanged = true;
}

//---------------------------------------------------------------------------
//�����_���l��ݒ�
//---------------------------------------------------------------------------
void XCMD_Random(UnicodeString prm)
{
	if (!ContainsStr(prm, "=")) UserAbort(USTR_SyntaxError);

	UnicodeString vnam = split_tkn(prm, '=');
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);

	UnicodeString fmt  = exclude_quot(prm);
	UnicodeString nstr, s0, s1;
	int cnt = 0;
	for (int i=1; i<=fmt.Length(); i++) {
		switch (cnt) {
		case 0:
			if (iswdigit(fmt[i])) {
				nstr = fmt[i];  cnt = 1;
			}
			else s0.cat_sprintf(_T("%c"), fmt[i]);
			break;
		case 1:
			if (!iswdigit(fmt[i])) {
				s1 = fmt[i];  cnt = 2;
			}
			else nstr.cat_sprintf(_T("%c"), fmt[i]);
			break;
		default:
			s1.cat_sprintf(_T("%c"), fmt[i]);
		}
	}

	bool z_sw = remove_top_s(nstr, '0');
	int n = nstr.ToIntDef(0);
	UnicodeString vstr;
	if (z_sw)
		vstr.sprintf(_T("%0*u"), nstr.Length(), Random(n + 1));
	else
		vstr.sprintf(_T("%u"), Random(n));

	XCMD_VarList->Values[vnam] = s0 + vstr + s1;
}

//---------------------------------------------------------------------------
//Test : �ϐ��̔�r
//---------------------------------------------------------------------------
bool XCMD_Test(UnicodeString prm)
{
	TMatch mt = TRegEx::Match(prm, "([=!<>]=)|[<>]");
	if (!mt.Success) UserAbort(USTR_SyntaxError);
	UnicodeString opstr = mt.Value;
	UnicodeString vnam	= get_tkn(prm, opstr);
	if (vnam.IsEmpty()) UserAbort(USTR_SyntaxError);
	UnicodeString vstr	= XCMD_VarList->Values[vnam];
	UnicodeString nstr	= exclude_quot(get_tkn_r(prm, opstr));
	int res = StrCmpLogicalW(vstr.c_str(), nstr.c_str());

	bool match = false;
	switch (idx_of_word_i(_T("==|!=|<=|>=|<|>"), opstr)) {
	case 0: match = (res==0);	break;
	case 1: match = (res!=0);	break;
	case 2: match = (res<=0);	break;
	case 3: match = (res>=0);	break;
	case 4: match = (res<0);	break;
	case 5: match = (res>0);	break;
	}
	return match;
}

//---------------------------------------------------------------------------
int XCMD_GetCodePage()
{
	return get_ListIntVal(XCMD_VarList, _T("CodePage"), 932);
}
//---------------------------------------------------------------------------
UnicodeString XCMD_GetBuffer()
{
	return XCMD_VarList->Values["Buffer"];
}
//---------------------------------------------------------------------------
void XCMD_SetBuffer(UnicodeString s)
{
	XCMD_set_Var(_T("Buffer"), s);
}

//---------------------------------------------------------------------------
//FilterBuffer : �o�b�t�@���i�荞��
//---------------------------------------------------------------------------
void XCMD_FilterBuffer(UnicodeString prm)
{
	std::unique_ptr<TStringList> b_buf(new TStringList());
	b_buf->Text = XCMD_GetBuffer();
	int i = 0;
	while (i<b_buf->Count) {
		if (ptn_match_str(prm, b_buf->Strings[i]).IsEmpty()) b_buf->Delete(i); else i++;
	}
	XCMD_SetBuffer(b_buf->Text);
	XCMD_BufChanged = true;
}
//---------------------------------------------------------------------------
//ReplaceBuffer : �o�b�t�@�̕�����u��
//---------------------------------------------------------------------------
void XCMD_ReplaceBuffer(UnicodeString prm)
{
	//�u���p�^�[�����
	UnicodeString o_str, r_str;
	int p = 1;
	bool in_qut = false;
	while (p<=prm.Length()) {
		WideChar c = prm[p];
		//���p���O
		if (!in_qut) {
			if (c=='=') {
				if (p<prm.Length()) r_str = exclude_quot(prm.SubString(p + 1, prm.Length() - p));
				break;
			}
			if (c=='\"') in_qut = true; else o_str.cat_sprintf(_T("%c"), c);
		}
		//���p����
		else if (c=='\"') in_qut = false; else o_str.cat_sprintf(_T("%c"), c);
		p++;
	}

	//�u��
	UnicodeString buf_str = XCMD_GetBuffer();
	//���K�\��
	if (is_regex_slash(o_str)) {
		try {
			o_str = exclude_top_end(o_str);
			r_str = ReplaceStr(ReplaceStr(r_str, "\\n", "\n"), "\\r", "\r");
			TRegExOptions opt;
			opt << roIgnoreCase << roMultiLine;	//�召���������A�����s���[�h
			if (contained_wd_i(_T("^|$|^$"), o_str)) {
				//��\r�A\n �̗����Ƀ}�b�`���Ă��܂����߁A�ꎞ�I�� \r\n �� \n
				buf_str = ReplaceStr(buf_str, "\r\n", "\n");
				r_str	= ReplaceStr(r_str,   "\r\n", "\n");
				buf_str = replace_regex_2(buf_str, o_str, r_str, opt);
				buf_str = ReplaceStr(buf_str, "\n", "\r\n");
			}
			else {
				buf_str = replace_regex_2(buf_str, o_str, r_str, opt);
			}
		}
		catch (...) {
			UserAbort(USTR_IllegalRegEx);
		}
	}
	//�ʏ�
	else {
		buf_str = ReplaceText(buf_str, o_str, r_str);
	}

	XCMD_SetBuffer(buf_str);
	XCMD_BufChanged = true;
}

//---------------------------------------------------------------------------
//LoadBuffer : �o�b�t�@�Ƀe�L�X�g�t�@�C����ǂݍ���
//---------------------------------------------------------------------------
void XCMD_LoadBuffer(UnicodeString prm)
{
	std::unique_ptr<TStringList> f_buf(new TStringList());
	XCMD_set_Var(_T("CodePage"), load_text_ex(prm, f_buf.get()));
	XCMD_SetBuffer(f_buf->Text);
	XCMD_set_Var(_T("BufferIndex"), 0);
	XCMD_BufChanged = true;
	if (!GlobalErrMsg.IsEmpty()) throw EAbort(GlobalErrMsg);
}

//---------------------------------------------------------------------------
//SaveBuffer : �o�b�t�@���t�@�C���ɕۑ�
//---------------------------------------------------------------------------
void XCMD_SaveBuffer(UnicodeString prm)
{
	try {
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(XCMD_GetCodePage()));
		std::unique_ptr<TStringList> f_buf(new TStringList());
		f_buf->Text = XCMD_GetBuffer();
		f_buf->SaveToFile(prm, enc.get());
	}
	catch (...) {
		UserAbort(USTR_FaildSave);
	}
}
//---------------------------------------------------------------------------
//AppendBuffer : �o�b�t�@���t�@�C���ɒǋL�ۑ�
//---------------------------------------------------------------------------
void XCMD_AppendBuffer(UnicodeString prm)
{
	try {
		std::unique_ptr<TEncoding> enc(TEncoding::GetEncoding(XCMD_GetCodePage()));
		TFile::AppendAllText(prm, XCMD_GetBuffer(), enc.get());
	}
	catch (...) {
		UserAbort(USTR_FaildSave);
	}
}

//---------------------------------------------------------------------------
//MsgBox : ���b�Z�[�W�\��
//---------------------------------------------------------------------------
int XCMD_MsgBox(UnicodeString cmd, UnicodeString prm)
{
	XCMD_box_res = mrNone;
	UnicodeString opt = cmd.SubString(7, 16);

	UnicodeString tit;
	prm = ReplaceStr(prm, ":\\",  "\f");	//��؂�Ƃ̍���������邽�߈ꎞ�u��
	if (remove_top_s(prm, ':')) tit = ReplaceStr(split_tkn(prm, ':'), "\f", ":\\");
	prm = ReplaceStr(prm, "\f", ":\\");

	UnicodeString lbuf;
	while (!prm.IsEmpty()) {
		WideChar c = split_top_wch(prm);
		if (c=='\\') {
			if		(remove_top_s(prm, 'n'))  lbuf += "\r\n";
			else if (remove_top_s(prm, '\\')) lbuf += "\\";
			else lbuf += split_top_ch(prm);
		}
		else {
			lbuf.cat_sprintf(_T("%c"), c);
		}
	}
	prm = lbuf;

	if		(opt.IsEmpty()) msgbox_OK(prm, tit);
	else if (USAME_TI(opt, "YN"))  XCMD_box_res = msgbox_Y_N(prm, tit)? mrYes : mrNo;
	else if (USAME_TI(opt, "YNC")) XCMD_box_res = msgbox_Y_N_C(prm, tit);
	else UserAbort(USTR_SyntaxError);

	return XCMD_box_res;
}

//---------------------------------------------------------------------------
//PlaySound : �T�E���h�Đ�
//---------------------------------------------------------------------------
void XCMD_PlaySound(UnicodeString prm)
{
	if (prm.IsEmpty()) {
		::mciSendString(_T("close TPLYSND"), NULL, 0, NULL);
	}
	else {
		UnicodeString fnam = to_absolute_name(prm);
		if (!file_exists(fnam)) SysErrAbort(ERROR_FILE_NOT_FOUND);
		play_sound_ex(fnam, false);
	}
}

//---------------------------------------------------------------------------
//FlashWin : �r�W���A���x��
//---------------------------------------------------------------------------
void XCMD_FlashWin(UnicodeString prm)
{
	if (!prm.IsEmpty()) {
		int cnt = split_tkn(prm, '_').ToIntDef(0);
		int tm  = prm.ToIntDef(0);
		flash_win(cnt, tm);
	}
	else {
		flash_win(FlashCntTaskfin, FlashTimeTaskfin);
	}
}

//---------------------------------------------------------------------------
//ListVar : �ϐ��ꗗ��\��
//---------------------------------------------------------------------------
void XCMD_get_UsrVarList(TStringList *lst)
{
	for (int i=0; i<XCMD_VarList->Count; i++) {
		if (!contained_wd_i(XCMD_VarNames, XCMD_VarList->Names[i]))
			add_PropLine(XCMD_VarList->Names[i], XCMD_VarList->ValueFromIndex[i], lst);
	}
}
//---------------------------------------------------------------------------
void XCMD_ListVar()
{
	std::unique_ptr<TStringList> v_buf(new TStringList());
	std::unique_ptr<TStringList> u_buf(new TStringList());
	v_buf->Assign(XCMD_VarList);

	//���[�U��`�ϐ��𒊏o
	int i = 0;
	while (i<v_buf->Count) {
		if (!contained_wd_i(XCMD_VarNames, v_buf->Names[i])) {
			u_buf->Add(v_buf->Strings[i]);
			v_buf->Delete(i);
		}
		else i++;
	}

	v_buf->Sort();
	v_buf->Insert(0, "��`�ςݕϐ�");
	if (u_buf->Count>0) {
		u_buf->Sort();
		u_buf->Insert(0, "���[�U��`�ϐ�");
		u_buf->Insert(0, EmptyStr);
	}

	set_FormTitle(GeneralInfoDlg, _T("�ϐ��ꗗ"));
	GeneralInfoDlg->isVarList = true;
	GeneralInfoDlg->GenInfoList->Assign(v_buf.get());
	GeneralInfoDlg->GenInfoList->AddStrings(u_buf.get());
	GeneralInfoDlg->ShowModal();
}

//---------------------------------------------------------------------------
//Sleep : ���ԑ҂�
//---------------------------------------------------------------------------
void XCMD_Sleep(UnicodeString prm)
{
	int ms = param_to_mSec(prm);
	if (ms<=0) UserAbort(USTR_SyntaxError);
	while (ms>0 && !XCMD_Aborted) {
		Application->ProcessMessages();
		if (ms>100) { Sleep(100); ms -= 100; } else { Sleep(ms); ms = 0; }
	}
}
//---------------------------------------------------------------------------
//Timer : �^�C�}�[
//---------------------------------------------------------------------------
void XCMD_Timer(UnicodeString prm)
{
	XCMD_tim_cnt = 0; 
	XCMD_tim_t	 = 0;

	if (!prm.IsEmpty()) {
		//����
		if (ContainsStr(prm, ":")) {
			try {
				XCMD_tim_t = TTime(prm);
			}
			catch (EConvertError &e) {
				XCMD_tim_t = 0;
				UserAbort(USTR_IllegalParam);
			}
			XCMD_tim_t += Date();
			//���łɉ߂��Ă����痂��
			if (CompareDateTime(Now(), XCMD_tim_t)==GreaterThanValue) XCMD_tim_t = IncDay(XCMD_tim_t, 1);
		}
		//����
		else { 
			int ms = param_to_mSec(prm);
			if (ms<=0) UserAbort(USTR_SyntaxError);
			XCMD_tim_cnt   = ms;
			XCMD_start_cnt = GetTickCount();
		}
	}
}

//---------------------------------------------------------------------------
//SearchDown(F/D)
//---------------------------------------------------------------------------
void XCMD_SearchDown(UnicodeString prm, TListBox *lp, TStringList *lst, bool file_only, bool dir_only)
{
	int idx = -1;
	if (ScrMode==SCMD_FLIST && lst) {
		for (int i=lp->ItemIndex+1; i<lp->Count && idx==-1; i++) {
			file_rec *fp = (file_rec*)lst->Objects[i];
			if ((file_only && fp->is_dir) || (dir_only && !fp->is_dir)) continue;
			if (!ptn_match_str(prm, ExtractFileName(lst->Strings[i])).IsEmpty()) idx = i;
		}
		if (idx!=-1) ListBoxSetIndex(lp, idx);
	}
	XCMD_matched = (idx!=-1);
}
//---------------------------------------------------------------------------
//SearchUp(F/D)
//---------------------------------------------------------------------------
void XCMD_SearchUp(UnicodeString prm, TListBox *lp, TStringList *lst, bool file_only, bool dir_only)
{
	int idx = -1;
	if (ScrMode==SCMD_FLIST) {
		for (int i=lp->ItemIndex-1; i>=0 && idx==-1; i--) {
			file_rec *fp = (file_rec*)lst->Objects[i];
			if ((file_only && fp->is_dir) || (dir_only && !fp->is_dir)) continue;
			if (!ptn_match_str(prm, ExtractFileName(lst->Strings[i])).IsEmpty()) idx = i;
		}
		if (idx!=-1) ListBoxSetIndex(lp, idx);
	}
	XCMD_matched = (idx!=-1);
}

//---------------------------------------------------------------------------
//PushIndex
//---------------------------------------------------------------------------
void XCMD_PushIndex(TListBox *lp)
{
	if (ScrMode==SCMD_FLIST) XCMD_IdxStack->Add(IntToStr(lp->ItemIndex));
}
//---------------------------------------------------------------------------
//PopIndex
//---------------------------------------------------------------------------
void XCMD_PopIndex(TListBox *lp)
{
	if (ScrMode==SCMD_FLIST && XCMD_IdxStack->Count>0) {
		int idx = XCMD_IdxStack->Strings[XCMD_IdxStack->Count - 1].ToIntDef(0);
		XCMD_IdxStack->Delete(XCMD_IdxStack->Count - 1);
		lp->ItemIndex = (idx<lp->Count)? idx : lp->Count - 1;
	}
}
//---------------------------------------------------------------------------
//PushTopIndex
//---------------------------------------------------------------------------
void XCMD_PushTopIndex(TListBox *lp)
{
	if (ScrMode==SCMD_FLIST) XCMD_TopIdxStack->Add(IntToStr(lp->TopIndex));
}
//---------------------------------------------------------------------------
//PopTopIndex
//---------------------------------------------------------------------------
void XCMD_PopTopIndex(TListBox *lp)
{
	if (ScrMode==SCMD_FLIST && XCMD_TopIdxStack->Count>0) {
		lp->TopIndex = XCMD_TopIdxStack->Strings[XCMD_TopIdxStack->Count - 1].ToIntDef(0);
		XCMD_TopIdxStack->Delete(XCMD_TopIdxStack->Count - 1);
	}
}

//---------------------------------------------------------------------------
//PushVar
//---------------------------------------------------------------------------
void XCMD_PushVar(UnicodeString prm)
{
	if (prm.IsEmpty()) UserAbort(USTR_SyntaxError);
	XCMD_VarStack->Add(XCMD_VarList->Values[prm]);
}
//---------------------------------------------------------------------------
//PopVar
//---------------------------------------------------------------------------
void XCMD_PopVar(UnicodeString prm)
{
	if (prm.IsEmpty()) UserAbort(USTR_SyntaxError);
	if (XCMD_VarStack->Count>0) {
		XCMD_VarList->Values[prm] = XCMD_VarStack->Strings[XCMD_VarStack->Count - 1];
		XCMD_VarStack->Delete(XCMD_VarStack->Count - 1);
	}
}

//---------------------------------------------------------------------------
//�^�C���X�^���v�̐ݒ�
//---------------------------------------------------------------------------
void XCMD_SetFileTime(UnicodeString prm)
{
	TDateTime dt;
	try {
		if (is_match_regex(prm, _T("\\d{4}/\\d{2}/\\d{2}\\s\\d{2}:\\d{2}:\\d{2}")))
			dt = TDateTime(prm);
		else if (is_match_regex(prm, _T("\\d{4}/\\d{2}/\\d{2}")))
			dt = TDate(prm);
		else if (is_match_regex(prm, _T("\\d{2}:\\d{2}:\\d{2}")))
			dt = TDateTime(prm.Insert(XCMD_VarList->Values["FileTime"].SubString(1, 11), 1));
		else UserAbort(USTR_IllegalParam);
	}
	catch (EConvertError &e) {
		UserAbort(USTR_IllegalParam);
	}
	if (!set_file_age(XCMD_cur_f_name, dt, ForceDel)) TextAbort(_T("�^�C���X�^���v�ݒ�Ɏ��s���܂����B"));
}

//---------------------------------------------------------------------------
//ReadINI : INI�t�@�C������Ǎ�
//---------------------------------------------------------------------------
void XCMD_ReadINI(UnicodeString prm)
{
	if (!ContainsStr(prm, "=")) UserAbort(USTR_SyntaxError);

	XCMD_VarList->Values[get_tkn(prm, '=')] = IniFile->ReadString("ExeCommands", exclude_quot(get_tkn_r(prm, '=')));
}
//---------------------------------------------------------------------------
//WriteINI : INI�t�@�C���ɏ���
//---------------------------------------------------------------------------
void XCMD_WriteINI(UnicodeString prm)
{
	if (!ContainsStr(prm, "=")) UserAbort(USTR_SyntaxError);

	IniFile->WriteString("ExeCommands", get_tkn(prm, '='), exclude_quot(get_tkn_r(prm, '=')));
}

//---------------------------------------------------------------------------
//�O���c�[���̎��s
//---------------------------------------------------------------------------
bool XCMD_ShellExe(UnicodeString cmd, UnicodeString prm, UnicodeString wdir,
	UnicodeString opt)	//H=��\��/ W=�I���҂�
{
	wdir = ExcludeTrailingPathDelimiter(wdir);
	XCMD_set_Var(_T("ExitCode"), EmptyStr);

	DWORD exit_code;
	std::unique_ptr<TStringList> o_lst(new TStringList());
	if (!Execute_ex(cmd, prm, wdir, opt, &exit_code, o_lst.get())) return false;

	if (contains_wd_i(opt, _T("W|O|L"))) {
		XCMD_set_Var(_T("ExitCode"), (int)exit_code);
		if (ContainsStr(opt, "O")) {
			if (USAME_TI(ExtractFileName(cmd), "git.exe")) {
				o_lst->Insert(0, "$ git " + prm);
			}
			if (o_lst->Count>0) {
				XCMD_SetBuffer(o_lst->Text);
				XCMD_BufChanged = true;
			}
		}
	}

	return true;
}

//---------------------------------------------------------------------------
//�w��N���X/�e�L�X�g�Ƀ}�b�`����E�B���h�E���A�N�e�B�u��
//---------------------------------------------------------------------------
bool XCMD_ActivateWnd(
	UnicodeString prm)	//[�N���X��][;�e�L�X�g|/���K�\��/]
{
	HWND hWnd = NULL;
	std::unique_ptr<TStringList> w_lst(new TStringList());
	get_GenWndList(w_lst.get());

	UnicodeString cls = get_tkn(prm, ';');
	UnicodeString txt = get_tkn_r(prm, ';');
	UnicodeString ptn = is_regex_slash(txt)? exclude_top_end(txt) : EmptyStr;
	if (cls.IsEmpty() && txt.IsEmpty() && ptn.IsEmpty()) return false;

	try {
		TRegExOptions opt; opt << roIgnoreCase;
		for (int i=0; i<w_lst->Count && !hWnd; i++) {
			UnicodeString lbuf = w_lst->Strings[i];
			if (!cls.IsEmpty() && !SameText(get_pre_tab(lbuf), cls)) continue;
			if (!ptn.IsEmpty()) {
				if (!TRegEx::IsMatch(get_post_tab(lbuf), ptn, opt)) continue;
			}
			else {
				if (!txt.IsEmpty() && !ContainsText(get_post_tab(lbuf), txt)) continue;
			}
			hWnd = (HWND)w_lst->Objects[i];
		}
	}
	catch (...) {
		return false;
	}
	if (!hWnd) return false;

	if (::IsIconic(hWnd)) ::SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
	::SetForegroundWindow(hWnd);
	return true;
}

//---------------------------------------------------------------------------
//�t�@�C�����X�g�̕`����~/�ĊJ
//---------------------------------------------------------------------------
void XCMD_Redraw(UnicodeString prm)
{
	if (ScrMode!=SCMD_FLIST) return;

	int tag = -1;
	if		(remove_top_text(prm, _T("L"))) tag = 0;
	else if (remove_top_text(prm, _T("R"))) tag = 1;

	switch (idx_of_word_i(_T("ON|OFF"), prm)) {
	case 0:
		if (tag==-1 || tag==0) set_RedrawOn(FileListPanel[0]);
		if (tag==-1 || tag==1) set_RedrawOn(FileListPanel[1]);
		break;
	case 1:
		if (tag==-1 || tag==0) set_RedrawOff(FileListPanel[0]);
		if (tag==-1 || tag==1) set_RedrawOff(FileListPanel[1]);
		break;
	default:
		UserAbort(USTR_SyntaxError);
	}
}
//---------------------------------------------------------------------------
