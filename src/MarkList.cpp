//----------------------------------------------------------------------//
// �R���g���[���̌������}�[�N											//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <System.StrUtils.hpp>
#include "MarkList.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	�}�[�N���X�g (TList ����p��)
//---------------------------------------------------------------------------
__fastcall MarkList::MarkList(Classes::TComponent* AOwner): TList()
{
	MarkOwner = (TForm*)AOwner;

	MarkColor = clYellow;
}
//---------------------------------------------------------------------------
__fastcall MarkList::~MarkList()
{
	ClearAll();
}

//---------------------------------------------------------------------------
//�}�[�N�����ׂăN���A
//---------------------------------------------------------------------------
void __fastcall MarkList::ClearAll()
{
	for (int i=0; i<Count; i++) delete Items[i];
	Clear();
}

//---------------------------------------------------------------------------
//�R���g���[���̌���
//�L���v�V�����Ɍ����ꂪ�܂܂��R���g���[�����}�[�N
//---------------------------------------------------------------------------
int __fastcall MarkList::SearchControl(TWinControl *ctrl, UnicodeString wd)
{
	int cnt = 0;
	wd = wd.UpperCase();

	for (int i=0; i<ctrl->ControlCount; i++) {
		TControl *cp = ctrl->Controls[i];
		if (cp->ClassNameIs("TShape")) continue;

		UnicodeString capstr;
		if 		(cp->ClassNameIs("TLabel"))			capstr = ((TLabel*)cp)->Caption;
		else if (cp->ClassNameIs("TLabeledEdit"))	capstr = ((TLabeledEdit*)cp)->EditLabel->Caption;
		else if (cp->ClassNameIs("TGroupBox"))		capstr = ((TGroupBox*)cp)->Caption;
		else if (cp->ClassNameIs("TCheckBox"))		capstr = ((TCheckBox*)cp)->Caption;
		else if (cp->ClassNameIs("TButton"))		capstr = ((TButton*)cp)->Caption;

		if (ContainsText(capstr, wd)) {
			TShape *sp = new TShape(MarkOwner);
			sp->Parent = NULL;
			sp->Tag = (int)ctrl;
			TRect rc = cp->BoundsRect;
			InflateRect(rc, 2, 2);
			sp->BoundsRect	 = rc;
			sp->Pen->Color	 = MarkColor;
			sp->Pen->Width	 = 2;
			sp->Brush->Style = bsClear;
			sp->Visible 	 = false;
			Add(sp);
			cnt++;
		}

		TWinControl *wp = dynamic_cast<TWinControl *>(cp);	if (!wp) continue;
		if (wp->ControlCount>0) cnt += SearchControl(wp, wd);
	}
	return cnt;
}

//---------------------------------------------------------------------------
//�����}�[�N��\��
//---------------------------------------------------------------------------
void __fastcall MarkList::ShowMark()
{
	for (int i=0; i<Count; i++) {
		TShape *sp	= Items[i];
		sp->Parent	= (TWinControl*)(sp->Tag);
		sp->Visible = true;
	}
}
//---------------------------------------------------------------------------
