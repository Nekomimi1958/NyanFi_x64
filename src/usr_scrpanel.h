//----------------------------------------------------------------------//
// シンプルスクロールバー・パネル										//
//																		//
//  TListBox、TStringGrid、TScrollBar の何れかに関連付け可能			//
//----------------------------------------------------------------------//
#ifndef UsrScrollPanelH
#define UsrScrollPanelH

//---------------------------------------------------------------------------
#include <Vcl.CheckLst.hpp>
#include <Vcl.Grids.hpp>
#include "usr_scale.h"

//---------------------------------------------------------------------------
//Flag
#define USCRPNL_FLAG_FL		0x0001	//ファイルリスト
#define USCRPNL_FLAG_GL		0x0002	//一覧リスト
#define USCRPNL_FLAG_TV		0x0004	//テキストビュアー

#define USCRPNL_FLAG_P_WP	0x0020	//親パネルの WindowProc 入れ替え
#define USCRPNL_FLAG_L_WP	0x0040	//リストボックスの WindowProc 入れ替え
#define USCRPNL_FLAG_G_WP	0x0080	//グリッドの WindowProc 入れ替え
#define USCRPNL_FLAG_HS		0x0100	//水平スクロールバー付(ListBox or Grid)

//---------------------------------------------------------------------------
#define USCRPNL_SCRTYPE_V	0		//垂直スクロールバー
#define USCRPNL_SCRTYPE_H	1		//水平スクロールバー

//---------------------------------------------------------------------------
#define WM_NYANFI_USCROLL	(WM_APP + 500)	//ファイルリストのノブ処理用
#define WM_NYANFI_UPDKNOB	(WM_APP + 501)	//ファイルリストのノブ更新用

//---------------------------------------------------------------------------
class UsrScrollPanel
{
private:
	TPanel     *ScrPanelV;			//垂直スクロールパネル
	TPaintBox  *ScrPaintBoxV;		//描画領域

	TPanel     *ScrPanelH;			//水平スクロールパネル
	TPaintBox  *ScrPaintBoxH;		//描画領域

	TRect ScrKnobRectV;				//ノブの長方形
	TRect ScrKnobRectH;
	int   ScrKnobMaxY;				//最大ノブ位置
	int   ScrKnobMaxX;

	bool  ScrCatchKnob;				//ノブをつかんでいる
	int	  ScrCatchTag;
	int   ScrCatchYp;
	int   ScrCatchXp;
	int   ScrPage;

	int   UsKnobWidth;				//スケーリング100%時のノブ幅
	int   LastPPI;

	TWndMethod org_ParentPanelWndProc;
	void __fastcall ParentPanelWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_ParentPanelWndProc(msg);
		if (msg.Msg==WM_SIZE) UpdateKnob();
	}

	TWndMethod org_AssoListWndProc;
	void __fastcall AssoListWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();
		org_AssoListWndProc(msg);
	}

	TWndMethod org_AssoChkListWndProc;
	void __fastcall AssoChkListWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();
		org_AssoChkListWndProc(msg);
	}

	TWndMethod org_AssoGridWndProc;
	void __fastcall AssoGridWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_PAINT && !ScrCatchKnob) Repaint();
		org_AssoGridWndProc(msg);
	}

	TWndMethod org_ScrPanelVWndProc;
	void __fastcall ScrPanelVWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_ScrPanelVWndProc(msg);
		if (org_ParentPanelWndProc==NULL && msg.Msg==WM_SIZE) UpdateKnob();
	}

	TWndMethod org_ScrPanelHWndProc;
	void __fastcall ScrPanelHWndProc(TMessage &msg)
	{
		if (msg.Msg==WM_ERASEBKGND) { msg.Result = 1; return; }
		org_ScrPanelHWndProc(msg);
		if (org_ParentPanelWndProc==NULL && msg.Msg==WM_SIZE) UpdateKnob();
	}

	void __fastcall ScrPaintBoxPaint(TObject *Sender);
	int  __fastcall GetKnobPosH();

	int  __fastcall get_GridTotalWidth();
	void __fastcall set_GridLefCol(int pos);
	int  __fastcall get_GridTotalHeight();
	void __fastcall set_GridTopRow(int pos);

	void __fastcall ScrPaintBoxMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall ScrPaintBoxMouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall ScrPaintBoxMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);

public:
	TPanel			*ParentPanel;		//親パネル
	TListBox		*AssoListBox;		//関連リストボックス
	TCheckListBox	*AssoChkListBox;	//関連チェックリストボックス
	TStringGrid		*AssoStrGrid;		//関連グリッド
	TScrollBar		*AssoScrollBar;		//関連スクロールバー

	int Flag;							//設定フラグ

	int FKnobWidth;						//ノブ幅
	void __fastcall SetKnobWidth(int Value)
	{
		FKnobWidth  = Value;
		UsKnobWidth = UnscaledInt(Value, ParentPanel);
		LastPPI     = ParentPanel->CurrentPPI;
	}
	__property int KnobWidth = {read = FKnobWidth,  write = SetKnobWidth};

	Graphics::TBitmap *KnobImgBuffV;	//垂直ノブ画像
	Graphics::TBitmap *KnobImgBuffH;	//水平ノブ画像

	TColor Color;						//背景色
	TColor KnobColor;					//ノブ色
	TColor KnobBdrColor;				//ノブ輪郭色
	TColor KnobActColor;				//ドラッグ時のノブ色
	TColor HitLineColor;				//ヒット位置色
	TColor SelLineColor;				//選択位置色
	int    PosLineAlpha;				//位置表示アルファ

	bool ListCsrVisible;				//リストボックスのカーソルを常に可視領域に

	TMouseEvent OnRButtonUp;			//右クリックイベント

	bool FVisible;
	void __fastcall SetVisible(bool Value)
	{
		FVisible = Value;
		if (!FVisible) {
			ScrPanelV->Visible = VisibleV = false;
			if (ScrPanelH) ScrPanelH->Visible = VisibleH = false;
		}
	}
	__property bool Visible = {read = FVisible,  write = SetVisible};

	bool VisibleV, VisibleH;			//スクロールバーの表示状態

	TStringList *HitLines;				//検索結果位置リスト
	TStringList *SelLines;				//選択項目位置リスト

	UsrScrollPanel(TPanel *pp, TListBox *lp,      int flag);
	UsrScrollPanel(TPanel *pp, TCheckListBox *lp, int flag);
	UsrScrollPanel(TPanel *pp, TStringGrid *gp,   int flag);
	UsrScrollPanel(TPanel *pp, TScrollBar *sp,    int flag);
	~UsrScrollPanel();

	void __fastcall InitializePanel();
	void __fastcall UpdateKnob();
	void __fastcall Repaint();

	void __fastcall SetRBCornerPanel(TPanel *pp);

	bool __fastcall KeyWordChanged(UnicodeString kwd, int max_cnt, bool case_sw = false, int code_page = 0);
	void __fastcall AddHitLine(int n);
};
//---------------------------------------------------------------------------
#endif
