//----------------------------------------------------------------------//
// NyanFi																//
//  �d��																//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include <System.StrUtils.hpp>
#include <RegularExpressions.hpp>
#include <Vcl.Clipbrd.hpp>
#include "UserFunc.h"
#include "Global.h"
#include "CalcDlg.h"
#include "UserMdl.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCalculator *Calculator = NULL;

bool Calculating = false;	//�v�Z��

//---------------------------------------------------------------------------
//���[�U�[�ɂ��Z�p�G���[�n���h��
//  �v�Z���̓G���[��}�~
//---------------------------------------------------------------------------
int _matherrl(struct _exceptionl *e)
{
	return Calculating? 1 : 0;
}

//---------------------------------------------------------------------------
//TCalculator �N���X
//---------------------------------------------------------------------------
__fastcall TCalculator::TCalculator(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::FormCreate(TObject *Sender)
{
	UserModule->SetUsrPopupMenu(this);

	ConstList = new TStringList();
	FuncList  = new TStringList();
	DefConstCount = 0;

	LineEdit->Tag	  = EDTAG_NO_CALC;
	HistComboBox->Tag = CBTAG_HISTORY | CBTAG_NO_CALC;

	AngleMode = 0;
	KommaFlag = false;
	isError   = false;
	ErrMsg	  = EmptyStr;

	OutDigit  = 18;

	InitialLine = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::FormShow(TObject *Sender)
{
	set_ComboBox_AutoComp(this);

	CloseIME(Handle);

	IniFile->LoadPosInfo(this, DialogCenter);

	LineEdit->Font->Assign(CalcFont);
	HistComboBox->Font->Assign(CalcFont);
	ToolBar1->Font->Assign(ToolBarFont);

	int hi = LineEdit->Height + HistComboBox->Height + ToolBar1->Height;
	Constraints->MinHeight = hi + Height - ClientHeight;
	Constraints->MaxHeight = hi + Height - ClientHeight;

	setup_ToolBar(ToolBar1);

	IniFile->LoadComboBoxItems(HistComboBox, _T("CalculatorHistory"));

	AngleMode = IniFile->ReadIntGen(_T("CalculatorAngMode"), 0);

	InitUsrDef();

	SetLineEdit(InitialLine);

	KommaFlag = false;
	isError   = false;
	ErrMsg	  = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::FormClose(TObject *Sender, TCloseAction &Action)
{
	IniFile->SavePosInfo(this);

	IniFile->WriteIntGen(_T("CalculatorAngMode"),	AngleMode);
	IniFile->SaveComboBoxItems(HistComboBox, _T("CalculatorHistory"));

	InitialLine = EmptyStr;
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::FormDestroy(TObject *Sender)
{
	delete ConstList;
	delete FuncList;
}

//---------------------------------------------------------------------------
//���[�U��`�̏�����
//---------------------------------------------------------------------------
void __fastcall TCalculator::InitUsrDef()
{
	//�֐�
	std::unique_ptr<UsrIniFile> def_file(new UsrIniFile(ExePath + CALC_INI_FILE));
	def_file->ReadSection("Function", FuncList);

	//�萔
	ConstList->Text = 
		"pi=3.14159265358979323846	�~����\n"
		"e=2.71828182845904523536	���R�ΐ��̒�\n"
		"c=299792458	�^�󒆂̌����x [m*sec^-1]\n"
		"g=9.80665	�d�͉����x [m/sec^2]\n"
		"h=6.6260755E-34	�v�����N�萔 [Js]\n"
		"ec=1.60217733E-19	�f�d�� [C]\n"
		"u0=1.25663706E-6	�^��̓����� [H/m]\n"
		"e0=8.85418782E-12	�^��̗U�d�� [F/m]\n"
		"k=1.380658E-23	�{���c�}���萔 [J/K]\n"
		"NA=6.0221367E+23	�A�{�K�h���萔[1/mol]\n"
		"Now=hh:nn:ss	���ݎ���\n";
	DefConstCount = ConstList->Count;

	std::unique_ptr<TStringList> c_lst(new TStringList());
	def_file->ReadSection("Constant", c_lst.get());
	ConstList->AddStrings(c_lst.get());

	HideDefConst = def_file->ReadBool("Option", "HideDefConst");
}

//---------------------------------------------------------------------------
//������ hh:nn[:ss] �`����?
//---------------------------------------------------------------------------
bool __fastcall TCalculator::IsTimeStr(UnicodeString s)
{
	return is_match_regex_i(s, _T("^(\\+|-)?(\\d+(:[0-5][0-9]){1,2}|Now)$"));
}
//---------------------------------------------------------------------------
//������16�i����?
//---------------------------------------------------------------------------
bool __fastcall TCalculator::IsHexStr(UnicodeString s)
{
	return is_match_regex_i(s, _T("^0x[0-9a-f]+$"));
}
//---------------------------------------------------------------------------
//������16�i/10�i������?
//---------------------------------------------------------------------------
bool __fastcall TCalculator::IsHexOrInt(UnicodeString s)
{
	return is_match_regex_i(s, _T("^((0x[0-9a-f]+)|((\\+|-)?[0-9][0-9,]*))$"));
}

//---------------------------------------------------------------------------
//long double ��������?
//---------------------------------------------------------------------------
bool __fastcall TCalculator::DoubleIsInt(long double v, long double &ip)
{
	return (modfl(v, &ip)==0 && ip>=_I64_MIN && ip<=_I64_MAX);
}
//---------------------------------------------------------------------------
bool __fastcall TCalculator::DoubleIsInt(long double v)
{
	long double ip;
	return (modfl(v, &ip)==0 && ip>=_I64_MIN && ip<=_I64_MAX);
}

//---------------------------------------------------------------------------
//long double �l�𕶎���ɕϊ�
//---------------------------------------------------------------------------
UnicodeString __fastcall TCalculator::LongDoubleToStr(
	long double v,	//�l
	bool is_x,		//16�i�\�L	(default = false)
	bool is_t,		//����		(default = false)
	bool is_ans)	//���ʗp��2�����炷	(default = false)
{
	UnicodeString ret_str;

	long double ip;
	if (DoubleIsInt(v, ip)) {
		if (KommaFlag) {
			ret_str = FormatFloat(",0", ip);
		}
		else if (is_x) {
			ret_str.sprintf(_T("0x%llx"), (__int64)ip);
		}
		else if (is_t) {
			int sig = (ip<0)? -1 : 1;
			int ss = fabsl(ip);
			int hh = ss/3600;  ss -= hh*3600;
			int mm = ss/60;	   ss -= mm*60;
			hh *= sig;
			ret_str.sprintf(_T("%d:%02u:%02u"), hh,mm,ss);
		}
		else {
			ret_str.sprintf(_T("%lld"), (__int64)ip);
		}
	}
	else {
		ret_str.sprintf(_T("%.*LG"), OutDigit - (is_ans? 2 : 0), v);
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//���l������̕]��
//�I�G���[���ɃT�C�����g��O���o
//---------------------------------------------------------------------------
long double __fastcall TCalculator::EvalNumStr(UnicodeString s)
{
	long double v = 0;

	try {
		//�萔������
		UnicodeString c = ConstList->Values[s];
		if (!c.IsEmpty()) {
			s = get_pre_tab(c);
			if (USAME_TI(s, "hh:nn:ss")) s = FormatDateTime("hh:nn:ss", Now());
		}
		s = ReplaceStr(s, ",", "");

		//16�i��
		if (IsHexStr(s)) {
			v = (long double)StrToInt64(s);
		}
		//����(ms)
		else if (IsTimeStr(s)) {
			int hh = split_tkn(s, ':').ToInt();
			int mm = split_tkn(s, ':').ToInt();
			int ss = s.IsEmpty()? 0 : s.ToInt();
			v = hh*3600 + mm * 60 + ss;
		}
		//���̑�
		else {
			bool is_fct = remove_end_s(s, '!');
			wchar_t *topptr = s.c_str();
			wchar_t *endptr;
#if defined(_WIN64)
			v = wcstold(topptr, &endptr);
#else
			v = _wcstold(topptr, &endptr);
#endif
			if ((topptr += s.Length()) != endptr) Abort();
			if (is_IllegalVal(v)) Abort();

			//�K��
			if (is_fct) {
				long double ip;
				if (!DoubleIsInt(v, ip)) Abort();
				int n = (int)ip;
				if (n<0) Abort();
				if (n==0) {
					v = 1;
				}
				else {
					v = n;
					for (int i=1; i<n-1; i++) {
						v *= (n - i);
						if (IsInfinite(v)) Abort();
					}
				}
			}
		}
	}
	catch (...) {
		ErrMsg = "�ُ�l�܂��̓I�[�o�[�t���[";
		Abort();
	}

	return v;
}

//---------------------------------------------------------------------------
//�֐��̕]��
//�I�G���[���ɃT�C�����g��O���o
//---------------------------------------------------------------------------
long double __fastcall TCalculator::EvalFunc(UnicodeString s)
{
	UnicodeString buf = Trim(s);

	long double sig = remove_top_s(buf, '-')? -1.0 : 1.0;
	remove_top_s(buf, '+');

	long double ans = 0;

	try {
		int p =buf.Pos(' ');
		if (p>0) {
			UnicodeString func = get_tkn(buf, ' ');
			long double   regA = EvalNumStr(Trim(get_tkn_r(buf, ' ')));

			bool handled = true;
			//�O�p�֐�
			if (contained_wd_i(_T("SIN|COS|TAN"), func)) {
				regA = (AngleMode==0)? regA/180*M_PI : (AngleMode==2)? regA /200*M_PI : regA;
				if		(USAME_TI(func, "SIN")) ans = sinl(regA);
				else if (USAME_TI(func, "COS")) ans = cosl(regA);
				else if (USAME_TI(func, "TAN")) ans = tanl(regA);
				else handled = false;
			}
			else if (contained_wd_i(_T("ASIN|ACOS|ATAN"), func)) {
				if		(USAME_TI(func, "ASIN")) ans = asinl(regA);
				else if (USAME_TI(func, "ACOS")) ans = acosl(regA);
				else if (USAME_TI(func, "ATAN")) ans = atanl(regA);
				else handled = false;
				ans = (AngleMode==0)? ans/M_PI*180 : (AngleMode==2)? ans/M_PI*100 : ans;
			}
			//�o�Ȑ��֐�
			else if (USAME_TI(func ,"SINH")) ans = sinhl(regA);
			else if (USAME_TI(func ,"COSH")) ans = coshl(regA);
			else if (USAME_TI(func ,"TANH")) ans = tanhl(regA);
			//�ΐ�
			else if (USAME_TI(func, "LN"))  ans = logl(regA);
			else if (USAME_TI(func, "LOG")) ans = log10l(regA);
			//���̑�
			else if (USAME_TI(func, "ABS"))   ans = fabsl(regA);
			else if (USAME_TI(func, "CEIL"))  ans = Ceil(regA);
			else if (USAME_TI(func, "FLOOR")) ans = Floor(regA);
			else handled = false;

			if (!handled) {
				ErrMsg = "�s���Ȋ֐�";  Abort();
			}
			else if (is_IllegalVal(ans)) {
				ErrMsg = "�ُ�l�܂��̓I�[�o�[�t���[";  Abort();
			}
		}
		else {
			//�K��̎��O�`�F�b�N
			if (EndsStr('!', s) && sig==-1) {
				ErrMsg = "��`��G���[";  Abort();
			}
			ans = EvalNumStr(buf);
		}
	}
	catch (...) {
		isError = true;
		Abort();
	}

	return sig * ans;
}

//---------------------------------------------------------------------------
//�w��ʒu�̉��Z���ڂ�]��
//�I�G���[���ɃT�C�����g��O���o
//---------------------------------------------------------------------------
void __fastcall TCalculator::EvalOpeItem(
	TStringList *o_lst,		//���Z�q���X�g
	TStringList *m_lst,		//���������X�g
	int p)					//���Z�ʒu
{
	if (p>=o_lst->Count) Abort();
	if (o_lst->Strings[p].IsEmpty()) Abort();

	int q = -1;
	for (int i=p-1; i>=0; i--) {
		if (!o_lst->Strings[i].IsEmpty()) {
			q = i; break;
		}
	}

	if (q==-1) Abort();
	if (p>=m_lst->Count || q>=m_lst->Count) Abort();

	long double  regA = EvalFunc(m_lst->Strings[q]);
	long double  regB = EvalFunc(m_lst->Strings[p]);
	UnicodeString ope = o_lst->Strings[p];
	long double   ans = 0;

	bool is_tA = IsTimeStr(m_lst->Strings[q]);
	bool is_tB = IsTimeStr(m_lst->Strings[p]);
	bool is_t  = false;

	try {
		if (ope=="+") {
			ans  = regA + regB;
			is_t = is_tA || is_tB;
		}
		else if (ope=="-") {
			ans = regA - regB;
			is_t = is_tA || is_tB;
		}
		else if (ope=="*") {
			ans = regA * regB;
			is_t = (is_tA && !is_tB) || (!is_tA && is_tB);
		}
		else if (ope=="/") {
			if (regB==0) {
				ErrMsg = "0�ɂ�銄��Z";  Abort();
			}
			ans = regA / regB;
			is_t = is_tA && !is_tB;
		}
		else if (ope=="%") {
			ans = fmodl(regA, regB);
		}
		else if (ope=="^") {
			if ((regA<0 && !DoubleIsInt(regB)) || (regA==0 && regB<=0)) {
				ErrMsg = "��`��G���[";  Abort();
			}
			ans = powl(regA, regB);
		}
		else if (ope=="&" || ope=="|" || ope=="�O" || ope=="�f" || ope=="�k") {
			long double ipA, ipB;
			if (!DoubleIsInt(regA, ipA) || !DoubleIsInt(regB, ipB)) {
				ErrMsg = "�l�������łȂ����I�[�o�[�t���[";  Abort();
			}
			int iA = (int)ipA;
			int iB = (int)ipB;
			//�ő����/�ŏ����{��
			if (ope=="�f" || ope=="�k") {
				if (iA==0 || iB==0) {
					ErrMsg = "��`��G���[";  Abort();
				}
				//���[�N���b�h�̌ݏ��@
				int a = iA; int b = iB;
				if (a<b) std::swap(a, b);
				for (;;) {
					int r = a%b;
					if (r==0) break;
					a = b; b = r;
				}
				ans = b;	//GCD

				if (ope=="�k") ans = (iA * iB)/ans;	//LCM
			}
			//�r�b�g���Z
			else {
				ans = (ope=="&")? (iA & iB) : (ope=="�O")? (iA ^ iB) : (iA | iB);
			}
		}

		if (is_IllegalVal(ans)) {
			ErrMsg = "�ُ�l�܂��̓I�[�o�[�t���[";  Abort();
		}

		//�b�����͎l�̌ܓ�
		if (is_t) ans = (ans>=0.0)? Floor(ans + 0.5) : -Floor(-ans + 0.5);
	}
	catch (...) {
		Abort();
	}

	m_lst->Strings[q] = LongDoubleToStr(ans,
							IsHexStr(m_lst->Strings[q]) && IsHexStr(m_lst->Strings[p]), is_t);
	m_lst->Delete(p);
	o_lst->Delete(p);
}

//---------------------------------------------------------------------------
//�������̕]��
//---------------------------------------------------------------------------
UnicodeString __fastcall TCalculator::EvalExpr(UnicodeString s)
{
	//�������Z���ڂɕ���
	std::unique_ptr<TStringList> ope_lst(new TStringList());
	std::unique_ptr<TStringList> mem_lst(new TStringList());
	ope_lst->Add("+");	//����̓_�~�[�Ŏg���Ȃ�

	UnicodeString lbuf = Trim(s);
	UnicodeString mbuf;
	int flag = 0;
	for (int i=1; i<=lbuf.Length(); i++) {
		WideChar c = lbuf[i];

		if (flag==0) {
			if (c!=' ') {
				mbuf = c;
				flag = 1;
			}
		}
		else if (flag==1) {
			if (UnicodeString("+-").Pos(c)) {
				UnicodeString tmp = Trim(mbuf);
				if (StartsStr("+", tmp) || StartsStr("-", tmp)) tmp = Trim(tmp.Delete(1, 1));

				if (contained_wd_i(_T("SIN|COS|TAN|ASIN|ACOS|ATAN|SINH|COSH|TANH|LOG|LN|ABS|CEIL|FLOOR"), tmp)) {
					flag = 2;
				}
				else {
					int n = tmp.Length();
					if (n>1 && EndsText("E", tmp) && iswdigit(tmp[n - 1])) flag = 2;
				}

				if (flag==2) {
					mbuf += c;
					flag = 1;
				}
				else {
					mem_lst->Add(Trim(mbuf)); ope_lst->Add(c); mbuf = EmptyStr;
					flag = 0;
				}
			}
			else if (UnicodeString("*/%^&�O|�f�k").Pos(c)) {
				mem_lst->Add(Trim(mbuf)); ope_lst->Add(c); mbuf = EmptyStr;
				flag = 0;
			}
			else {
				mbuf += c;
			}
		}
	}

	if (!mbuf.IsEmpty()) mem_lst->Add(Trim(mbuf));

	//�v�Z
	UnicodeString ret_str;
	try {
		if (mem_lst->Count<ope_lst->Count) Abort();

		for (;;) {
			//����
			if (mem_lst->Count==1) {
				UnicodeString mem = mem_lst->Strings[0];
				if (IsHexStr(mem) || IsTimeStr(mem))
					ret_str = mem;
				else
					ret_str = LongDoubleToStr(EvalFunc(mem), false, false, true);
				break;
			}
			//��������D�揇�ʂɏ]���Čv�Z
			else {
				// ^
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("^", ope_lst->Strings[i]))  EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// * / %
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("*/%", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// + -
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("*+-", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// &
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("&", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// xor
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("�O", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// |
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("|", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
				// gcd, lcm
				for (int i=1; i<ope_lst->Count; i++)
					if (ContainsStr("�f�k", ope_lst->Strings[i])) EvalOpeItem(ope_lst.get(), mem_lst.get(), i);
			}
		}
	}
	catch (...) {
		isError = true;
		ret_str = EmptyStr;
	}

	return ret_str;
}

//---------------------------------------------------------------------------
//�v�Z�s�̕]��
//---------------------------------------------------------------------------
UnicodeString __fastcall TCalculator::EvalLine(UnicodeString s)
{
	UnicodeString lbuf = Trim(s);

	TRegExOptions opt;  opt<<roIgnoreCase;
	lbuf = TRegEx::Replace(lbuf, "\\b([a-z]\\w*)\\(", "\\1 (", opt);

	while (!isError) {
		//��ԓ����̊���
		TMatch mt = TRegEx::Match(lbuf, "\\([^()]+\\)");
		if (mt.Success) {
			int idx = mt.Index;
			int len = mt.Length;
			UnicodeString ebuf = mt.Value;
			ebuf = EvalLine(ebuf.SubString(2, len - 2));
			UnicodeString rbuf;
			if (idx>1) {
				rbuf = lbuf.SubString(1, idx - 1);
				lbuf.Delete(1, idx - 1);
			}
			lbuf = Trim(rbuf + ebuf + lbuf.Delete(1, len));
		}
		//���ʖ���
		else {
			if (lbuf.Pos('(') || lbuf.Pos(')')) {
				ErrMsg  = "�s���Ȋ���";
				isError = true;
			}
			else {
				lbuf = EvalExpr(lbuf);
			}
			break;
		}
	}

	if (isError) lbuf = EmptyStr;

	return lbuf;
}

//---------------------------------------------------------------------------
//�����v�Z
//---------------------------------------------------------------------------
void __fastcall TCalculator::CalcLine(
	UnicodeString s,	//�v�Z��
	bool use_cb)		//�N���b�v�{�[�h����Ď��s
{
	s = SetLineStr(s);

	UnicodeString exline = s;
	exline = Trim(get_tkn(exline, '='));

	InitUsrDef();

	//���[�U��`�֐��̓W�J
	try {
		for (int i=0; i<FuncList->Count; i++) {
			UnicodeString fnc = FuncList->Names[i];
			UnicodeString def = get_pre_tab(FuncList->ValueFromIndex[i]);

			TStringDynArray p_lst = SplitString(get_in_paren(fnc), ",");
			if (p_lst.Length==0) TextAbort(_T("�֐��̈���������`"));

			fnc = get_tkn(fnc, '(') + "(";
			if (pos_i(fnc, def)) TextAbort(_T("�s���Ȋ֐���`"));

			for (;;) {
				int p0 = pos_i(fnc, exline);  if (p0==0) break;

				//�֐������𒊏o
				int lvl = 1;
				int p1 = p0 + fnc.Length();
				UnicodeString fstr = fnc;
				UnicodeString pstr;
				while (p1<=exline.Length()) {
					WideChar c = exline[p1];
					fstr.cat_sprintf(_T("%c"), c);
					if (c=='(') {
						lvl++;
					}
					else if (c==')') {
						lvl--;
						if (lvl==0) break;
					}
					pstr.cat_sprintf(_T("%c"), c);
					p1++;
				}
				if (lvl!=0) TextAbort(_T("�s���Ȋ���"));

				//�֐�����������������𒊏o
				std::unique_ptr<TStringList> q_lst(new TStringList());
				lvl = 0;
				UnicodeString pbuf;
				for (int j=1; j<=pstr.Length(); j++) {
					WideChar c = pstr[j];
					if (c==',' && lvl==0) {
						q_lst->Add(pbuf);
						pbuf = EmptyStr;
					}
					else {
						if (c=='(') lvl++; else if (c==')') lvl--;
						pbuf.cat_sprintf(_T("%c"), c);
					}
				}
				if (!pbuf.IsEmpty()) q_lst->Add(pbuf);

				if (p_lst.Length!=q_lst->Count) TextAbort(_T("�������s��v"));

				//�֐���`�Ɉ���������ݒ�
				UnicodeString dbuf = def;
				for (int j=0; j<q_lst->Count; j++) {
					dbuf = ReplaceText(dbuf, p_lst[j], "(" + q_lst->Strings[j] + ")");
				}

				//�֐������̒u��
				exline = ReplaceText(exline, fstr, "(" + dbuf + ")");
			}
		}
	}
	catch (EAbort &e) {
		ShowError(e.Message.c_str());
		return;
	}

	//���Z�q��1�����ɓ����u��
	exline = ReplaceText(exline, " AND ", "&");
	exline = ReplaceText(exline, " XOR ", "�O");
	exline = ReplaceText(exline, " OR ", "|");
	exline = ReplaceText(exline, " GCD ", "�f");
	exline = ReplaceText(exline, " LCM ", "�k");

	//�J���}�̏���
	KommaFlag = (exline.Pos(',')>0);
	exline = ReplaceStr(exline, ",", "");

	UnicodeString answer;

	//���������_��O���ꎞ�I�Ƀ}�X�N
	TFPUExceptionMask orgMask = GetExceptionMask();
	TFPUExceptionMask tmpMask;
	tmpMask<<exInvalidOp<<exDenormalized<<exZeroDivide<<exOverflow<<exUnderflow<<exPrecision;
	SetExceptionMask(tmpMask);

	try {
		Calculating = true;
		isError = false;
		try {
			if (is_match_regex(exline, _T("[^\\-^+*/%&�O|�f�k:()!,.\\w\\s]"))) {
				ErrMsg = "�s���ȕ���";
				Abort();
			}

			//���ݎ���
			if (USAME_TI(exline, "Now"))
				answer = FormatDateTime("hh:nn:ss", Now());
			//�P�Ƃ̐��� (16�i/10�i�ϊ�)
			else if (IsHexOrInt(exline))
				answer = LongDoubleToStr(EvalNumStr(exline), !IsHexStr(exline));
			//�P�Ƃ̎��� (�b�ɕϊ�)
			else if (IsTimeStr(s))
				answer = LongDoubleToStr(EvalNumStr(exline));
			//��
			else
				answer = EvalLine(exline);
		}
		catch (...) {
			isError = true;
		}
	}
	__finally {
		Calculating = false;
		SetExceptionMask(orgMask);
	}

	//���ʕ\��
	if (!isError) {
		if (use_cb) {
			OutputLine = answer;
		}
		else {
			add_ComboBox_history(HistComboBox,
				Trim(get_tkn(s, '=')).cat_sprintf(_T(" = %s"), answer.c_str()));
			SetLineEdit(answer);
		}
	}
	//�G���[�\��
	else {
		if (use_cb) {
			if (ErrMsg.IsEmpty()) ErrMsg = "���͓��e�̌��";
			OutputLine.sprintf(_T("ERR: %s [%s]"), ErrMsg.c_str(), s.c_str());
		}
		else {
			ShowError();
		}
	}
}

//---------------------------------------------------------------------------
//���͍s��ݒ�(�����s�Ȃ� + �ŘA��)
//---------------------------------------------------------------------------
UnicodeString __fastcall TCalculator::SetLineStr(UnicodeString s)
{
	//�����s�̏ꍇ + �ŘA��
	std::unique_ptr<TStringList> lst(new TStringList());
	lst->Text = s;
	if (lst->Count>1) {
		s = EmptyStr;
		for (int i=0; i<lst->Count; i++) {
			UnicodeString lbuf = exclude_quot(trim_ex(lst->Strings[i]));
			if (lbuf.IsEmpty()) continue;
			if (s.IsEmpty())
				s = lbuf;
			else
				s.cat_sprintf(_T("+%s"), Trim(lbuf).c_str());
		}
	}
	else {
		s = exclude_quot(trim_ex(s));
	}
	return s;
}

//---------------------------------------------------------------------------
//���͗��̐ݒ�
//---------------------------------------------------------------------------
void __fastcall TCalculator::SetLineEdit(UnicodeString s)
{
	LineEdit->SetFocus();
	LineEdit->Text = SetLineStr(s);
	if (!s.IsEmpty()) LineEdit->SelStart = s.Length();
}

//---------------------------------------------------------------------------
//�G���[��\��
//---------------------------------------------------------------------------
void __fastcall TCalculator::ShowError(_TCHAR *msg)
{
	UnicodeString s = msg;
	if (s.IsEmpty()) s = ErrMsg;
	if (s.IsEmpty()) s = "���͓��e�̌��";

	if (LineEdit->Focused()) {
		UnicodeString lbuf = LineEdit->Text;
		LineEdit->Color = col_bgWarn;
		LineEdit->Text	= s;
		LineEdit->Repaint();
		Sleep(1000);
		LineEdit->Color = scl_Window;
		LineEdit->Text	= lbuf;
		LineEdit->SelStart = lbuf.Length();
	}
	else if (HistComboBox->Focused()) {
		UnicodeString lbuf = HistComboBox->Text;
		HistComboBox->Color = col_bgWarn;
		HistComboBox->Text	= s;
		HistComboBox->Repaint();
		Sleep(1000);
		HistComboBox->Color = scl_Window;
		HistComboBox->Text	= lbuf;
		HistComboBox->SelStart = lbuf.Length();
	}

	ErrMsg = EmptyStr;
}

//---------------------------------------------------------------------------
//�v�Z���ɕ������ǉ�
//---------------------------------------------------------------------------
void __fastcall TCalculator::AppendToLine(UnicodeString s)
{
	int p = s.Pos('(');
	if (p>0) p = s.Length() - p;

	if (HistComboBox->Focused()) {
		s = HistComboBox->Text + s;
		HistComboBox->Text	   = s;
		HistComboBox->SelStart = s.Length() - p;
	}
	else {
		s = LineEdit->Text + s;
		LineEdit->Text	   = s;
		LineEdit->SelStart = s.Length() - p;
	}
}

//---------------------------------------------------------------------------
//���͗��ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TCalculator::LineEditKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);
	if (contained_wd_i(KeysStr_CsrDown, KeyStr)) KeyStr = "DOWN";

	bool handled = true;
	if		(equal_ENTER(KeyStr))		CalcLine(Trim(LineEdit->Text));
	else if (USAME_TI(KeyStr, "DOWN"))	HistComboBox->SetFocus();
	else handled = false;

	if (handled) Key = 0;
}

//---------------------------------------------------------------------------
//���𗓂ł̃L�[����
//---------------------------------------------------------------------------
void __fastcall TCalculator::HistComboBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	UnicodeString KeyStr = get_KeyStr(Key, Shift);

	bool handled = true;
	if (equal_ENTER(KeyStr)) {
		CalcLine(Trim(get_tkn(HistComboBox->Text, '=')));
	}
	else if (contained_wd_i(KeysStr_CsrUp,   KeyStr) && !HistComboBox->DroppedDown) {
		LineEdit->SetFocus();
	}
	else if (contained_wd_i(KeysStr_CsrDown, KeyStr) && !HistComboBox->DroppedDown) {
		HistComboBox->DroppedDown = true;
	}
	else if (USAME_TI(KeyStr, "BKSP")) {
		UnicodeString lbuf = HistComboBox->Text;
		if (lbuf.Pos('=')) {
			lbuf = Trim(get_tkn(lbuf, '=')) + " ";
			HistComboBox->Text = lbuf;
			HistComboBox->SelStart = lbuf.Length();
		}
	}
	else handled = false;

	if (handled) Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::LineEditKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key==VK_RETURN) Key = 0;
}

//---------------------------------------------------------------------------
void __fastcall TCalculator::AngleActionExecute(TObject *Sender)
{
	AngleMode = (AngleMode + 1)%3;
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::AngleActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Caption = (AngleMode==1)? "RAD" : (AngleMode==2)? "GRAD" : "DEG";
}

//---------------------------------------------------------------------------
//DEFINE
//---------------------------------------------------------------------------
void __fastcall TCalculator::RefDefBtnClick(TObject *Sender)
{
	while (ConstPopupMenu->Items->Count>0) ConstPopupMenu->Items->Delete(0);

	//�萔
	TMenuItem *mp;
	for (int i=HideDefConst? DefConstCount : 0; i<ConstList->Count; i++) {
		mp = new TMenuItem(ConstPopupMenu);
		mp->Caption = ConstList->Strings[i];
		mp->OnClick = RefDefItemClick;
		ConstPopupMenu->Items->Add(mp);
	}

	//�֐�
	if (FuncList->Count>0) {
		mp = new TMenuItem(ConstPopupMenu);
		mp->Caption = "-";
		ConstPopupMenu->Items->Add(mp);
		for (int i=0; i<FuncList->Count; i++) {
			mp = new TMenuItem(ConstPopupMenu);
			mp->Caption = FuncList->Strings[i];
			mp->OnClick = RefDefItemClick;
			ConstPopupMenu->Items->Add(mp);
		}
	}

	//�ҏW
	mp = new TMenuItem(ConstPopupMenu);
	mp->Caption = "-";
	ConstPopupMenu->Items->Add(mp);
	mp = new TMenuItem(ConstPopupMenu);
	mp->Action = EditDefAction;
	ConstPopupMenu->Items->Add(mp);

	Mouse->CursorPos = RefDefBtn->ClientToScreen(Point(8, 8));
	TPoint p = Mouse->CursorPos;
	ConstPopupMenu->Popup(p.x, p.y);
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::RefDefItemClick(TObject *Sender)
{
	AppendToLine(ReplaceStr(get_tkn(((TMenuItem*)Sender)->Caption, '='), "&", ""));
}

//---------------------------------------------------------------------------
//��`�t�@�C���̕ҏW
//---------------------------------------------------------------------------
void __fastcall TCalculator::EditDefActionExecute(TObject *Sender)
{
	UnicodeString fnam = ExePath + CALC_INI_FILE;
	std::unique_ptr<UsrIniFile> def_file(new UsrIniFile(fnam));
	if (def_file->SectionList->Count==0) {
		//�V�K�쐬
		std::unique_ptr<TStringList> dummy(new TStringList());
		def_file->WriteString("Constant", "Tax",    "0.08\t�����");
		def_file->WriteString("Function", "cot(X)", "1/tan(X)\t�]��");
		def_file->WriteBool("Option", "HideDefConst", false);
		def_file->UpdateFile(true);
	}
	open_by_TextEditor(fnam);
}

//---------------------------------------------------------------------------
//NOW
//---------------------------------------------------------------------------
void __fastcall TCalculator::NowBtnClick(TObject *Sender)
{
	AppendToLine(FormatDateTime("hh:nn:ss", Now()));
}

//---------------------------------------------------------------------------
//16�i/10�i�ϊ�
//---------------------------------------------------------------------------
void __fastcall TCalculator::ToHexDecActionExecute(TObject *Sender)
{
	try {
		bool is_h = IsHexStr(LineEdit->Text);
		SetLineEdit(LongDoubleToStr(EvalNumStr(LineEdit->Text), !is_h));
	}
	catch (...) {
		ShowError();
	}
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::ToHexDecActionUpdate(TObject *Sender)
{
	UnicodeString s = LineEdit->Text;
	((TAction *)Sender)->Caption = IsHexStr(s)? "D&EC" : "H&EX";
	((TAction *)Sender)->Enabled = IsHexOrInt(s);
}

//---------------------------------------------------------------------------
//NOT
//---------------------------------------------------------------------------
void __fastcall TCalculator::NotActionExecute(TObject *Sender)
{
	try {
		__int64 v = StrToInt64(LineEdit->Text);
		v = ~v;
		SetLineEdit(LongDoubleToStr(v, IsHexStr(LineEdit->Text)));
	}
	catch (...) {
		ShowError();
	}
}
//---------------------------------------------------------------------------
void __fastcall TCalculator::NotActionUpdate(TObject *Sender)
{
	((TAction *)Sender)->Enabled = IsHexOrInt(LineEdit->Text);
}

//---------------------------------------------------------------------------
//AC
//---------------------------------------------------------------------------
void __fastcall TCalculator::AcBtnClick(TObject *Sender)
{
	if (HistComboBox->Focused())
		HistComboBox->Text = EmptyStr;
	else
		LineEdit->Text	   = EmptyStr;
}

//---------------------------------------------------------------------------
void __fastcall TCalculator::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SpecialKeyProc(this, Key, Shift);
}
//---------------------------------------------------------------------------

