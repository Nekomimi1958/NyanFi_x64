//----------------------------------------------------------------------//
// NyanFi																//
//  画像ビュー処理スレッド												//
//----------------------------------------------------------------------//
#ifndef ImgViewThreadH
#define ImgViewThreadH

//---------------------------------------------------------------------------
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
class TImgViewThread : public TThread
{
private:
	UnicodeString Img_f_name;
	__int64		  Img_f_size;
	TDateTime	  Img_f_time;
	UnicodeString Img_rot_str;	//メタファイルの回転・反転処理履歴
	bool ChgPrvCur;				//プレビュー上のマウスポインタを変更

	UnicodeString MsgStr;
	bool MsgIsErr;

	TStringList *RequestList;	//処理キュー

	TMultiReadExclusiveWriteSynchronizer *TaskRWLock;

	//スレッドセーフを考慮したプロパティ
	bool FTaskBusy;
	bool __fastcall GetTaskBusy()
	{
		TaskRWLock->BeginRead();
		bool v = FTaskBusy;
		TaskRWLock->EndRead();
		return v;
	}
	void __fastcall SetTaskBusy(bool Value)
	{
		TaskRWLock->BeginWrite();
		FTaskBusy = Value;
		TaskRWLock->EndWrite();
	}
	__property bool TaskBusy = {read = GetTaskBusy,  write = SetTaskBusy};

	void __fastcall ClearImage();
	void __fastcall DrawImage();
	void __fastcall DrawViewBuff();
	void __fastcall DrawMessage();
	void __fastcall Execute();

public:
	UnicodeString FileName;		//要求画像ファイル名
	UnicodeString FileName2;

	bool ReqRedraw;			//再描画を要求
	bool ReqReload;			//リロードを要求
	bool ReqClear;			//画面クリアを要求
	bool ReqVideo;			//動画(サムネイル表示を要求)
	bool ReqWait;			//MinShowTime による待ち要求
	bool UseViewBuff;		//メイン側で設定されたバッファ内容を使用
	int  Rotation;			//回転/反転を要求 (1,3,4,5)
	int  ExifOri;			//Exif の方向情報
	int  ZoomRatio;			//ズーム倍率(%)
	double ZoomRatioF;

	bool Fitted;			//フィット表示
	bool GrayScaled;		//グレースケール表示
	bool ShowGrid;			//グリッドを表示

	bool ImgIsWmf;

	Graphics::TBitmap *ImgBuff;		//画像バッファ(元サイズ)
	Graphics::TBitmap *ViewBuff;	//表示バッファ
	Graphics::TBitmap *ChgBuff;
	TMetafile *MetaBuff;			//メタファイル用バッファ

	__fastcall TImgViewThread(bool CreateSuspended);
	bool __fastcall IsReady();
	bool __fastcall IsWaiting();
	void __fastcall AddRequest(const _TCHAR *cmd, UnicodeString prm1 = EmptyStr, UnicodeString prm2 = EmptyStr);
	void __fastcall AddRequest(const _TCHAR *cmd, int prm);
};
//---------------------------------------------------------------------------
#endif
