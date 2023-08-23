//----------------------------------------------------------------------//
// NyanFi																//
//  FTP接続ダイアログ													//
//----------------------------------------------------------------------//
#ifndef FtpDlgH
#define FtpDlgH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ActnList.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Mask.hpp>

//---------------------------------------------------------------------------
class TFtpConnectDlg : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *AddHostAction;
	TAction *ChgHostAction;
	TAction *ConnectAction;	TButton *DelMenuBtn;
	TActionList *ActionList1;
	TButton *AddMenuBtn;
	TButton *CancelBtn;
	TButton *ChgMenuBtn;
	TButton *ConnectBtn;
	TButton *DowButton;
	TButton *RefDirBtn;
	TButton *RefSndConnectBtn;
	TButton *RefSndDisconBtn;
	TButton *RefSndTransferBtn;
	TButton *TestSndConnectBtn;
	TButton *TestSndDisconBtn;
	TButton *TestSndTransferBtn;
	TButton *UpButton;
	TCheckBox *AnonyCheckBox;
	TCheckBox *DlKeepTimeCheckBox;
	TCheckBox *LogResCheckBox;
	TCheckBox *PasvCheckBox;
	TCheckBox *SyncLRCheckBox;
	TCheckBox *UpKeepTimeCheckBox;
	TCheckBox *UpToLowerCheckBox;
	TComboBox *SSLComboBox;
	TGroupBox *AutoDisconGroupBox;
	TGroupBox *RemoteGroupBox;
	TGroupBox *SndFindGroupBox;
	TLabel *Label1;
	TLabel *Label2;
	TLabeledEdit *DisconTimeEdit;
	TLabeledEdit *HostAddrEdit;
	TLabeledEdit *HostDirEdit;
	TLabeledEdit *HostNameEdit;
	TLabeledEdit *LocalDirEdit;
	TLabeledEdit *PassWdEdit;
	TLabeledEdit *SndConnectEdit;
	TLabeledEdit *SndDisconEdit;
	TLabeledEdit *SndTransferEdit;
	TLabeledEdit *TextFExtEdit;
	TLabeledEdit *UserIdEdit;
	TListBox *HostListBox;
	TPageControl *PageControl1;
	TPanel *HostPanel;
	TPanel *OptPanel;
	TRadioButton *RSide0Btn;
	TRadioButton *RSide1Btn;
	TRadioButton *RSide2Btn;
	TTabSheet *HostSheet;
	TTabSheet *OptionSheet;
	TCheckBox *LastDirCheckBox;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PageControl1DrawTab(TCustomTabControl *Control, int TabIndex, const TRect &Rect, bool Active);
	void __fastcall HostListBoxDrawItem(TWinControl *Control, int Index, TRect &Rect, TOwnerDrawState State);
	void __fastcall HostListBoxClick(TObject *Sender);
	void __fastcall HostListBoxKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall AnonyCheckBoxClick(TObject *Sender);
	void __fastcall AddHostActionExecute(TObject *Sender);
	void __fastcall ChgHostActionExecute(TObject *Sender);
	void __fastcall HostItemActionUpdate(TObject *Sender);
	void __fastcall RefDirBtnClick(TObject *Sender);
	void __fastcall ConnectActionExecute(TObject *Sender);
	void __fastcall ConnectActionUpdate(TObject *Sender);
	void __fastcall HostListBoxDblClick(TObject *Sender);
	void __fastcall RefSoundBtnClick(TObject *Sender);
	void __fastcall TestSoundBtnClick(TObject *Sender);

private:	// ユーザー宣言
	TStringList *HostList;

	UnicodeString __fastcall MakeHostItem();

public:		// ユーザー宣言
	UnicodeString HostItem;

	__fastcall TFtpConnectDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFtpConnectDlg *FtpConnectDlg;
//---------------------------------------------------------------------------
#endif
