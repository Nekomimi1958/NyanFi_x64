//----------------------------------------------------------------------//
// NyanFi																//
//  サブビュアー(旧GIFビュアー)											//
//----------------------------------------------------------------------//
#ifndef GifViewH
#define GifViewH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <System.Actions.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ActnList.hpp>

//---------------------------------------------------------------------------
class TGifViewer : public TForm
{
__published:	// IDE で管理されるコンポーネント
	TAction *FlipHorzAction;
	TAction *FlipVertAction;
	TAction *RotateLeftAction;
	TAction *RotateRightAction;
	TActionList *ActionList1;
	TImage *Image1;
	TMenuItem *BgColItem;
	TMenuItem *FlipHItem;
	TMenuItem *FlipVItem;
	TMenuItem *LockItem;
	TMenuItem *RotateLItem;
	TMenuItem *RotateRItem;
	TMenuItem *Sep_1;
	TMenuItem *Sep_2;
	TPanel *ImgPanel;
	TPopupMenu *PopupMenu1;

	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Image1Click(TObject *Sender);
	void __fastcall FormClick(TObject *Sender);
	void __fastcall BgColItemClick(TObject *Sender);
	void __fastcall PopupMenu1Popup(TObject *Sender);
	void __fastcall LockItemClick(TObject *Sender);
	void __fastcall RotateActionExecute(TObject *Sender);
	void __fastcall RotateActionUpdate(TObject *Sender);

private:	// ユーザー宣言
	bool hasBitmap;
	UnicodeString TitleInf, ZoomInf;

	void __fastcall WmSysCommand(TWMSysCommand & SysCom);
	void __fastcall WmExitSizeMove(TMessage &msg);

	TWndMethod org_ImgPanelWndProc;
	void __fastcall ImgPanelWndProc(TMessage &msg);

	double __fastcall GetZoomRatio();

public:		// ユーザー宣言
	UnicodeString FileName;

	bool FImgLocked;
	void __fastcall SetImgLocked(bool Value);
	__property bool ImgLocked = {read = FImgLocked,  write = SetImgLocked};

	void __fastcall DrawImage(UnicodeString fnam = EmptyStr);
	void __fastcall RotateImage(int rn);
	void __fastcall LockImage();

	__fastcall TGifViewer(TComponent* Owner);

	BEGIN_MESSAGE_MAP
		VCL_MESSAGE_HANDLER(WM_SYSCOMMAND,		TWMSysCommand,	WmSysCommand)
		VCL_MESSAGE_HANDLER(WM_EXITSIZEMOVE,	TMessage,		WmExitSizeMove)
	END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TGifViewer *GifViewer;
//---------------------------------------------------------------------------
#endif
