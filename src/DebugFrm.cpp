//----------------------------------------------------------------------//
// NyanFi																//
//  �f�o�b�O���														//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "UserFunc.h"
#include "UserMdl.h"
#include "usr_excmd.h"
#include "Global.h"
#include "DebugFrm.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDebugForm *DebugForm = NULL;

//---------------------------------------------------------------------------
__fastcall TDebugForm::TDebugForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormCreate(TObject *Sender)
{
	ListBuff = new TStringList();

	PrevScrPanel  = new UsrScrollPanel(PrevPanel,  PreviewListBox, USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);
	ReferScrPanel = new UsrScrollPanel(ReferPanel, ReferListBox,   USCRPNL_FLAG_P_WP | USCRPNL_FLAG_L_WP);

	ReferListBox->Tag = LBTAG_OPT_FIF1;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormShow(TObject *Sender)
{
	IniFile->LoadPosInfo(this, DialogCenter);

	SetToolWinBorder(this);

	ReferSplitter->Color = col_Splitter;

	PreviewListBox->Font->Assign(TxtPrvFont);
	PreviewListBox->Color		= col_bgView;
	PreviewListBox->Font->Color = col_fgView;
	PreviewListBox->Tag 		= LBTAG_OPT_LNNO;
	PreviewListBox->TabWidth	= 4;
	set_ListBoxItemHi(PreviewListBox);
	set_UsrScrPanel(PrevScrPanel);

	ReferListBox->Font->Assign(FileInfFont);
	ReferListBox->Color		    = col_bgInf;
	ReferListBox->Font->Color   = col_fgInf;
	set_ListBoxItemHi(ReferListBox);
	set_UsrScrPanel(ReferScrPanel);

	ReferPanel->Height = IniFile->ReadIntGen(_T("DebugReferHeight"), 200);

	setup_ToolBar(ToolBar1);

	XCMD_ShowDebugInf();
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (XCMD_Debugging && XCMD_xlp && !XCMD_xlp->EndOfCmds)
		CanClose = msgbox_Sure(_T("�f�o�b�N�𒆎~���ăR�}���h�̎��s���I�����܂���?"));
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);
	IniFile->WriteIntGen(_T("DebugReferHeight"),	ReferPanel->Height);

	ExitAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormDestroy(TObject *Sender)
{
	delete PrevScrPanel;
	delete ReferScrPanel;
	delete ListBuff;
}

//---------------------------------------------------------------------------
void __fastcall TDebugForm::UpdateToolBar()
{
	for (int i=0; i<ToolBar1->ButtonCount; i++) {
		TToolButton *bp = ToolBar1->Buttons[i];
		if (bp && bp->Action) bp->Action->Update();
	}
}

//---------------------------------------------------------------------------
//�v���r���[���e�̐ݒ�
//---------------------------------------------------------------------------
void __fastcall TDebugForm::SetPreview(TStringList *lst, int idx)
{
	if (!FileName.IsEmpty())
		Caption = UnicodeString().cat_sprintf(_T("�f�o�b�O - %s"), ExtractFileName(FileName).c_str());
	
	ListBuff->Assign(lst);
	ListBuff->Insert(0, EmptyStr);

	TListBox *lp = PreviewListBox;
	assign_FileListBox(lp, ListBuff, -1, PrevScrPanel);
	ListBoxSetIndex(lp, idx + 1);
	lp->Invalidate();

	UpdateToolBar();
}

//---------------------------------------------------------------------------
void __fastcall TDebugForm::PreviewListBoxEnter(TObject *Sender)
{
	ReferListBox->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::PreviewListBoxData(TWinControl *Control, int Index, UnicodeString &Data)
{
	Data = (Index>=0 && Index<ListBuff->Count)? ListBuff->Strings[Index] : EmptyStr;
}

//---------------------------------------------------------------------------
//�v���r���[�̕`��
//---------------------------------------------------------------------------
void __fastcall TDebugForm::PreviewListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	TListBox *lp = (TListBox*)Control;
	TCanvas  *cv = lp->Canvas;
	TRect     rc = Rect;

	//�s�ԍ�
	LineNoOut(cv, rc, Index);

	//�e�L�X�g
	cv->Brush->Color = State.Contains(odSelected)? col_selItem : col_bgView;
	cv->FillRect(rc);
	PrvTextOut(lp, Index, cv, rc,
		is_SelFgCol(State)? col_fgSelItem : col_fgView,
		4, NULL, "*.nbt");

	//�J�[�\��
	if (lp->ItemIndex==Index) {
		int lw = 1;
		int yp = Rect.Bottom - lw;
		draw_Line(cv, Rect.Left, yp, Rect.Right, yp, lw, col_Cursor, psSolid);
	}
}
//---------------------------------------------------------------------------
//�v���r���[/�Q�Ə��ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TDebugForm::ReferListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (SameText(KeyStr, KeyStr_Copy)) EditCopyAction->Execute();
}
//---------------------------------------------------------------------------
//�Q�Ə��̕`��
//---------------------------------------------------------------------------
void __fastcall TDebugForm::ReferListBoxDrawItem(TWinControl *Control, int Index,
	TRect &Rect, TOwnerDrawState State)
{
	draw_InfListBox((TListBox*)Control, Rect, Index, State);
}

//---------------------------------------------------------------------------
//�L�[����
//---------------------------------------------------------------------------
void __fastcall TDebugForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (SpecialKeyProc(this, Key, Shift, _T(HELPTOPIC_XC) _T("#Debug"))) return;

	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (KeyStr.Length()==1) {
		switch (KeyStr[1]) {
		case 'S': StepAction->Execute();	break;
		case 'G': GoAction->Execute();		break;
		case 'B': BreakAction->Execute();	break;
		case 'Q': QuitAction->Execute();	break;
		case 'X': ExitAction->Execute();	break;
		case 'V': VarListAction->Execute();	break;
		case 'E': EditAction->Execute();	break;
		case 'H': HelpAction->Execute();	break;
		}

		UpdateToolBar();
	}
}

//---------------------------------------------------------------------------
//�R�s�[
//---------------------------------------------------------------------------
void __fastcall TDebugForm::EditCopyActionExecute(TObject *Sender)
{
	ExeCmdListBox(ReferListBox, _T("ClipCopy"));
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::EditCopyActionUpdate(TObject *Sender)
{
	TListBox *lp = ReferListBox;
	((TAction*)Sender)->Enabled = lp && lp->SelCount>0;
}

//---------------------------------------------------------------------------
//�X�e�b�v���s
//---------------------------------------------------------------------------
void __fastcall TDebugForm::StepActionExecute(TObject *Sender)
{
	XCMD_Debug_Step = true;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::StepActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XCMD_IsBusy && XCMD_Debugging;
}
//---------------------------------------------------------------------------
//���s
//---------------------------------------------------------------------------
void __fastcall TDebugForm::GoActionExecute(TObject *Sender)
{
	XCMD_Debug_Go = true;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::GoActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XCMD_IsBusy && XCMD_Debugging && !XCMD_Debug_Go;
}
//---------------------------------------------------------------------------
//��~
//---------------------------------------------------------------------------
void __fastcall TDebugForm::BreakActionExecute(TObject *Sender)
{
	XCMD_Debug_Go = false;
	XCMD_ShowDebugInf();
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::BreakActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XCMD_IsBusy && XCMD_Debugging && XCMD_Debug_Go;
}

//---------------------------------------------------------------------------
//�f�o�b�O���[�h�I��
//---------------------------------------------------------------------------
void __fastcall TDebugForm::QuitActionExecute(TObject *Sender)
{
	XCMD_Debug_Quit = true;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::QuitActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XCMD_IsBusy && XCMD_Debugging;
}

//---------------------------------------------------------------------------
//�R�}���h�̎��s���I��
//---------------------------------------------------------------------------
void __fastcall TDebugForm::ExitActionExecute(TObject *Sender)
{
	XCMD_Debug_Exit = true;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::ExitActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = XCMD_IsBusy && XCMD_Debugging;
}

//---------------------------------------------------------------------------
//�ϐ��ꗗ
//---------------------------------------------------------------------------
void __fastcall TDebugForm::VarListActionExecute(TObject *Sender)
{
	if (XCMD_IsBusy && XCMD_Debugging)
		XCMD_Debug_List = true;
	else
		XCMD_ListVar();
}

//---------------------------------------------------------------------------
//�G�f�B�^�ŊJ��
//---------------------------------------------------------------------------
void __fastcall TDebugForm::EditActionExecute(TObject *Sender)
{
	if (!open_by_TextEditor(FileName, PreviewListBox->ItemIndex)) msgbox_ERR(GlobalErrMsg);
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::EditActionUpdate(TObject *Sender)
{
	((TAction*)Sender)->Enabled = !FileName.IsEmpty();
}

//---------------------------------------------------------------------------
//�w���v
//---------------------------------------------------------------------------
void __fastcall TDebugForm::HelpActionExecute(TObject *Sender)
{
	XCMD_Debug_Help = true;
}
//---------------------------------------------------------------------------

