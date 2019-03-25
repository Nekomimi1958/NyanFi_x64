//----------------------------------------------------------------------//
//コマンド一覧															//
//																		//
//----------------------------------------------------------------------//
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "usr_file_ex.h"
#include "usr_tag.h"
#include "usr_cmdlist.h"

//---------------------------------------------------------------------------
UnicodeString ScrModeIdStr = "FSVIL";

//---------------------------------------------------------------------------
//コマンド一覧リストを設定
//---------------------------------------------------------------------------
void set_CmdList(
	TStringList *c_list,	//[o] コマンド名=説明
	TStringList *s_list)	//[o] 識別文字:コマンド名=説明
{
	c_list->Text =
		"F:AboutNyanFi=バージョン情報\n"
		"F:AddTab=タブを追加\n"
		"F:BackDirHist=ディレクトリ履歴を戻る\n"
		"F:Backup=反対パスにバックアップ\n"
		"F:BgImgMode=背景画像の表示モード設定\n"
		"F:BorderLeft=ファイルリストの境界を左に移動\n"
		"F:BorderRight=ファイルリストの境界を右に移動\n"
		"F:CalcDirSize=ディレクトリ容量を計算\n"
		"F:CalcDirSizeAll=全ディレクトリの容量を計算\n"
		"F:ChangeDir=ディレクトリを変更\n"
		"F:ChangeDrive=ドライブの変更\n"
		"F:ChangeOppDir=反対側のディレクトリを変更\n"
		"F:ChangeOppRegDir=反対側で登録ディレクトリを開く\n"
		"F:ChangeRegDir=登録ディレクトリを開く\n"
		"F:CheckUpdate=更新の確認\n"
		"F:ClearMask=選択マスク/パスマスクを解除\n"
		"F:Clone=反対パスにクローン作成\n"
		"F:CloneToCurr=カレントにクローン作成\n"
		"F:CommandPrompt=コマンドプロンプト\n"
		"F:CompareDlg=同名ファイルの比較ダイアログ\n"
		"F:CompareHash=ハッシュ値の比較\n"
		"F:CompleteDelete=完全削除\n"
		"F:CompressDir=ディレクトリのNTFS圧縮\n"
		"F:ContextMenu=コンテキストメニューを表示\n"
		"F:ConvertDoc2Txt=バイナリ文書→テキスト変換\n"
		"F:ConvertHtm2Txt=HTML→テキスト変換\n"
		"F:ConvertImage=画像ファイルの変換\n"
		"F:ConvertTextEnc=文字コードの変換\n"
		"F:Copy=コピー\n"
		"F:CopyCmdName=コマンド名をクリップボードにコピー\n"
		"F:CopyDir=ディレクトリ構造のコピー\n"
		"F:CopyFileName=ファイル名をクリップボードにコピー\n"
		"F:CopyTo=指定ディレクトリにコピー\n"
		"F:CopyToClip=クリップボードにコピー\n"
		"F:CountLines=テキストファイルの行数をカウント\n"
		"F:CreateDir=ディレクトリの作成\n"
		"F:CreateDirsDlg=ディレクトリ一括作成ダイアログ\n"
		"F:CreateHardLink=ハードリンクの作成\n" 
		"F:CreateJunction=ジャンクションの作成\n" 
		"F:CreateShortcut=ショートカットの作成\n"
		"F:CreateSymLink=シンボリックリンクの作成\n" 
		"F:CreateTestFile=テストファイルの作成\n"
		"F:CsrDirToOpp=カーソル位置のディレクトリを反対側に開く\n"
		"F:CurrFromOpp=カレントパスを反対パスに\n"
		"F:CurrToOpp=反対パスをカレントパスに\n"
		"F:CursorEnd=カーソルを最下行に移動\n"
		"F:CursorEndSel=選択しながらカーソルを最下行に移動\n"
		"F:CursorTop=カーソルを最上行に移動\n"
		"F:CursorTopSel=選択しながらカーソルを最上行に移動\n"
		"F:CutToClip=クリップボードに切り取り\n"
		"F:DateSelect=指定した日付条件に合うファイルを選択\n"
		"F:DeleteADS=ファイルの代替データストリームを削除\n"
		"F:DelJpgExif=Exif情報の削除\n"
		"F:DelSelMask=選択マスクから項目を削除\n"
		"F:DelTab=タブを削除\n"
		"F:DiffDir=ディレクトリの比較\n"
		"F:DirHistory=ディレクトリ履歴\n"
		"F:DirStack=ディレクトリ・スタック\n"
		"F:DistributionDlg=振り分けダイアログ\n"
		"F:DotNyanDlg=.nyanfi ファイルの設定\n"
		"F:DriveList=ドライブ一覧\n"
		"F:DriveGraph=ドライブ使用率推移\n"
		"F:EditIniFile=INIファイルの編集\n"
		"F:Eject=CD/DVDドライブのトレイを開く\n"
		"F:EjectDrive=ドライブの取り外し\n"
		"F:EmptyTrash=ごみ箱を空にする\n"
		"F:EqualListWidth=左右のリスト幅を均等に\n"
		"F:ExeCommandLine=コマンドラインの実行\n"
		"F:ExeExtMenu=追加メニューの実行\n"
		"F:ExeExtTool=外部ツールの実行\n"
		"F:Exit=NyanFiの終了\n"
		"F:ExitDupl=二重起動されたNyanFiを終了\n"
		"F:ExPopupMenu=拡張ポップアップメニュー\n"
		"F:ExtractChmSrc=CHMからソースを抽出\n"
		"F:ExtractGifBmp=アニメGIFからビットマップを抽出\n"
		"F:ExtractIcon=アイコンを抽出\n"
		"F:ExtractImage=埋め込み画像を抽出\n"
		"F:ExtractMp3Img=MP3の埋め込み画像を抽出\n"
		"F:FileExtList=拡張子別一覧\n"
		"F:FileListOnly=ファイルリストのみを表示\n"
		"F:FileRun=ファイル名を指定して実行\n"
		"F:Filter=ファイルリストの絞り込み\n"
		"F:FindDuplDlg=重複ファイルの検索ダイアログ\n"
		"F:FindDirDlg=ディレクトリ名検索ダイアログ\n"
		"F:FindFileDirDlg=ファイル/ディレクトリ名検索ダイアログ\n"
		"F:FindFileDlg=ファイル名検索ダイアログ\n"
		"F:FindFolderIcon=フォルダアイコン検索\n"
		"F:FindHardLink=ハードリンクを列挙\n"
		"F:FindMark=栞マーク項目を検索\n"
		"F:FindTag=タグ検索\n"
		"F:FixTabPath=タブ変更に対してカレントパスを一時固定/解除\n"
		"F:FTPChmod=パーミッションの設定\n"
		"F:FTPConnect=FTPホストに接続\n"
		"F:FTPDisconnect=FTPホストから切断\n"
		"F:ForwardDirHist=ディレクトリ履歴を進む\n"
		"F:GetHash=ファイルのハッシュ値を取得\n"
		"F:GitDiff=カーソル位置ファイルの差分を表示\n"
		"F:GitViewer=Gitビュアー\n"
		"F:HideSizeTime=サイズと更新日時を隠す\n"
		"F:ImageViewer=イメージビュアーで開く\n"
		"F:InputDir=入力したディレクトリに変更\n"
		"F:InputPathMask=パスマスクを入力\n"
		"F:InsSeparator=ワークリストにセパレータを挿入\n"
		"F:ItemTmpDown=項目を一時的に1つ下に移動\n"
		"F:ItemTmpMove=選択項目を一時的にカーソル位置に移動\n"
		"F:ItemTmpUp=項目を一時的に1つ上に移動\n"
		"F:JoinText=テキストファイルの結合\n"
		"F:JumpTo=指定したファイル位置へ\n"
		"F:Library=ライブラリへ\n"
		"F:LinkToOpp=リンク先を反対側に開く\n"
		"F:ListArchive=アーカイブの内容一覧\n"
		"F:ListClipboard=クリップボードを一覧で表示\n"
		"F:ListDuration=ファイル再生時間の一覧\n"
		"F:ListExpFunc=エクスポート関数一覧\n"
		"F:ListFileName=ファイル名を一覧で表示\n"
		"F:ListNyanFi=NyanFi についての情報をログに表示\n"
		"F:ListText=テキストファイルを一覧で表示\n"
		"F:ListTail=テキストファイルの末尾を一覧で表示\n"
		"F:ListTree=ディレクトリ構造のツリー表示\n"
		"F:LoadResultList=結果リストをファイルから読み込む\n"
		"F:LoadTabGroup=タブグループをファイルから読み込む\n"
		"F:LockTextPreview=テキストプレビューのロック/解除\n"
		"F:LogFileInfo=ファイル情報をログに出力\n"
		"F:MarkMask=栞マーク項目だけを残して他を隠す\n"
		"F:MaskFind=指定マスクにマッチする項目を検索\n"
		"F:MatchSelect=指定文字列を含むファイルを選択\n"
		"F:MonitorOff=ディスプレイの電源を切る\n"
		"F:Move=移動\n"
		"F:MoveTab=タブの位置を移動\n"
		"F:MoveTo=指定ディレクトリへ移動\n"
		"F:MuteVolume=音量ミュート\n"
		"F:NameFromClip=ファイル名をクリップボードの内容に\n"
		"F:NameToLower=ファイル名の小文字化\n"
		"F:NameToUpper=ファイル名の大文字化\n"
		"F:NetConnect=ネットワークドライブの割り当て\n"
		"F:NetDisconnect=ネットワークドライブの切断\n"
		"F:NewFile=新規ファイルの作成\n"
		"F:NewTextFile=新規テキストファイルの作成\n"
		"F:NextDrive=次のドライブへ\n"
		"F:NextTab=次のタブへ\n"
		"F:OpenADS=代替データストリームを仮想リストとして開く\n"
		"F:OpenByExp=エクスプローラで開く\n"
		"F:OpenCtrlPanel=コントロールパネルを開く\n"
		"F:OpenGitURL=リモートリポジトリURLを開く\n"
		"F:OpenStandard=標準の Enter キー動作\n"
		"F:OpenTrash=ごみ箱を開く\n"
		"F:Pack=反対パスにアーカイブ作成\n"
		"F:PackToCurr=カレントパスにアーカイブ作成\n"
		"F:Paste=貼り付け\n"
		"F:PathMaskDlg=パスマスクダイアログ\n"
		"F:PlayList=プレイリストを作って再生\n"
		"F:PopDir=ディレクトリをポップ\n"
		"F:PopupTab=タブ選択メニューを表示\n"
		"F:PowerOff=Windowsを終了\n"
		"F:PowerShell=PowerShell を起動\n"
		"F:PrevDrive=前のドライブへ\n"
		"F:PrevTab=前のタブへ\n"
		"F:PushDir=ディレクトリをプッシュ\n"
		"F:Reboot=Windowsを再起動\n"
		"F:RecentList=最近使ったファイル一覧\n"
		"F:RegDirDlg=登録ディレクトリダイアログ\n"
		"F:RegDirPopup=登録ディレクトリ・ポップアップメニュー\n"
		"F:RegSyncDlg=同期コピーの設定\n"
		"F:ReloadList=ファイルリストを最新の情報に更新\n"
		"F:RenameDlg=名前等の変更\n"
		"F:RepositoryList=リポジトリ一覧\n"
		"F:Restart=NyanFiの再起動\n"
		"F:ReturnList=ファイルリスト表示に戻る\n"
		"F:SaveAsResultList=結果リストに名前を付けて保存\n"
		"F:SaveAsTabGroup=タブグループに名前を付けて保存\n"
		"F:SaveTabGroup=タブグループを上書き保存\n"
		"F:ScrollDownLog=ログを下にスクロール\n"
		"F:ScrollDownText=テキストプレビューを下にスクロール\n"
		"F:ScrollUpLog=ログを上にスクロール\n"
		"F:ScrollUpText=テキストプレビューを上にスクロール\n"
		"F:SelAllItem=すべての項目を選択/解除\n"
		"F:SelByList=リストによって項目を選択\n"
		"F:SelectUp=選択/解除後、カーソルを上に移動\n"
		"F:SelEmptyDir=空のディレクトリだけを選択\n"
		"F:SelGitChanged=Git作業ディレクトリで変更ファイルを選択\n"
		"F:SelMask=選択中の項目だけを残して他を隠す\n"
		"F:SelOnlyCur=カレント側だけにあるファイルを選択\n"
		"F:SelReverseAll=すべての項目の選択状態を反転\n"
		"F:SetAlias=ワークリストの項目にエイリアスを設定\n"
		"F:SetArcTime=アーカイブのタイムスタンプを最新に合わせる\n"
		"F:SetDirTime=ディレクトリのタイムスタンプを最新に合わせる\n"
		"F:SetExifTime=タイムスタンプをExif撮影日時に設定\n"
		"F:SetFolderIcon=フォルダアイコンの設定\n"
		"F:SetPathMask=パスマスクを設定\n"
		"F:SetSttBarFmt=ステータスバー書式を設定\n"
		"F:SetSubSize=サブウィンドウのサイズを設定\n"
		"F:ShareList=共有フォルダ一覧\n"
		"F:ShowByteSize=ファイルサイズをバイト単位で表示\n"
		"F:ShowHideAtr=隠しファイルを表示\n"
		"F:ShowIcon=アイコンの表示/非表示\n"
		"F:ShowLogWin=ログウィンドウーの表示/非表示\n"
		"F:ShowPreview=イメージプレビューの表示/非表示\n"
		"F:ShowProperty=ファイル情報の表示/非表示\n"
		"F:ShowSystemAtr=システムファイルを表示\n"
		"F:ShowTabBar=タブバーの表示/非表示\n"
		"F:SoloTab=他のタブをすべて削除\n"
		"F:SpecialDirList=特殊フォルダ一覧\n"
		"F:SubDirList=サブディレクトリ一覧\n"
		"F:SwapLR=左右のファイルリストを入れ替える\n"
		"F:SwapName=名前を入れ替える\n"
		"F:SyncLR=左右ディレクトリの同期変更を有効/解除\n"
		"F:TabDlg=タブの設定ダイアログ\n"
		"F:TabHome=タブのホームへ\n"
		"F:TestArchive=アーカイブの正当性を検査\n"
		"F:TextViewer=テキストビュアーで開く\n"
		"F:ToExViewer=別ウィンドウのテキストビュアーへ\n"
		"F:ToLog=ログウィンドウへ\n"
		"F:ToNextOnRight=右ファイルリストで次のNyanFiへ\n"
		"F:ToOpposite=反対側のファイルリストへ\n"
		"F:ToOppSameItem=カーソル位置と同名の反対側項目へ\n"
		"F:ToOppSameHash=カーソル位置と同ハッシュ値の反対側項目へ\n"
		"F:ToParent=親ディレクトリへ\n"
		"F:ToParentOnLeft=左ファイルリストで親ディレクトリへ\n"
		"F:ToParentOnRight=右ファイルリストで親ディレクトリへ\n"
		"F:ToPrevOnLeft=左ファイルリストで前のNyanFiへ\n"
		"F:ToRoot=ルートディレクトリへ\n"
		"F:ToTab=指定番号/キャプションのタブへ\n"
		"F:ToText=テキストプレビューへ\n"
		"F:TrimTagData=タグデータの整理\n"
		"F:UndoRename=直前の改名を元に戻す\n"
		"F:UnPack=反対パスに解凍\n"
		"F:UnPackToCurr=カレントパスに解凍\n"
		"F:UpdateFromArc=アーカイブから更新\n"
		"F:ViewIniFile=INIファイルの閲覧\n"
		"F:ViewTail=テキストファイルの末尾を閲覧\n"
		"F:WatchTail=テキストファイルの追加更新を監視\n"
		"F:WidenCurList=カレント側リストの幅を広げる\n"
		"F:WinMaximize=ウィンドウの最大化\n"
		"F:WinMinimize=ウィンドウの最小化\n"
		"F:WinNormal=ウィンドウを元のサイズに戻す\n"
		"F:WorkItemDown=ワークリストの項目を1つ下に移動\n"
		"F:WorkItemMove=選択ワークリスト項目をカーソル位置に移動\n"
		"F:WorkItemUP=ワークリストの項目を1つ上に移動\n"
		"F:XmlViewer=XMLビュアー\n"
		"FI:AddTag=項目にタグを追加\n"
		"FI:DelTag=項目のタグを削除\n"
		"FI:HomeWorkList=ホームワークリストを開く\n"
		"FI:LoadBgImage=背景画像を読み込む\n"
		"FI:LoadWorkList=ワークリストをファイルから読み込む\n"
		"FI:MaskSelect=指定マスクにマッチするファイルを選択\n"
		"FI:NewWorkList=新規ワークリストの作成\n"
		"FI:NextMark=次の栞マーク項目へ\n"
		"FI:PrevMark=前の栞マーク項目へ\n"
		"FI:NextSameName=ファイル名主部が同じ次ファイルへ\n"
		"FI:OpenByApp=独自の関連付けで開く\n"
		"FI:OpenByWin=Windowsの関連付けで開く\n"
		"FI:SaveAsWorkList=ワークリストに名前を付けて保存\n"
		"FI:SaveWorkList=ワークリストを上書き保存\n"
		"FI:SelAllFile=すべてのファイルを選択/解除\n"
		"FI:SelMark=栞マーク項目を選択\n"
		"FI:SelReverse=ファイルの選択状態を反転\n"
		"FI:SelSameExt=拡張子が同じファイルを選択\n"
		"FI:SelSameName=ファイル名主部が同じファイルを選択\n"
		"FI:SelWorkItem=ワークリストの登録項目を選択\n"
		"FI:SetTag=項目にタグを設定\n"
		"FI:SortDlg=ソートダイアログ\n"
		"FI:SubViewer=サブビュアーの表示/非表示\n"
		"FI:TagSelect=指定タグを含む項目を選択\n"
		"FI:UseTrash=削除にごみ箱を使う/使わない\n"
		"FI:WorkList=ワークリスト\n"
		"FL:CancelAllTask=すべてのタスクを中断\n"
		"FL:ClearLog=ログをクリア\n"
		"FL:PauseAllTask=すべてのタスクを一旦停止/再開\n"
		"FL:Suspend=タスク予約項目の実行を保留/解除\n"
		"FL:TaskMan=タスクマネージャ\n"
		"FL:ToLeft=左ファイルリストへ\n"
		"FL:ToRight=右ファイルリストへ\n"
		"FL:ViewLog=ログをテキストビュアーで表示\n"
		"FS:ClearAll=すべての選択状態を解除\n"
		"FSI:Select=選択/解除\n"
		"FSVIL:HelpContents=ヘルプの目次/索引を表示\n"
		"FSVIL:KeyList=キー割り当て一覧\n"
		"FSVIL:OptionDlg=オプション設定\n"
		"FV:BinaryEdit=ファイルのバイナリ編集\n"
		"FV:CursorDownSel=選択しながらカーソルを下に移動\n"
		"FV:CursorUpSel=選択しながらカーソルを上に移動\n"
		"FV:EditHighlight=構文強調表示定義ファイルの編集\n"
		"FV:EditHistory=最近編集したファイル一覧\n"
		"FV:Grep=文字列検索(GREP)\n"
		"FV:HtmlToText=テキストビュアーでHTML→テキスト変換表示\n"
		"FV:IncSearch=インクリメンタルサーチ\n"
		"FV:FindTagName=tags からタグ名検索\n"
		"FV:FixedLen=テキストビュアーでCSV/TSVを固定長表示\n"
		"FV:PageDownSel=選択しながら1ページ下に移動\n"
		"FV:PageUpSel=選択しながら1ページ上に移動\n"
		"FV:RegExChecker=正規表現チェッカー\n"
		"FV:SetColor=テキストビュアーの配色\n"
		"FV:SetFontSize=フォントサイズを変更\n"
		"FV:SetMargin=テキストビュアーの左側余白を設定\n"
		"FV:SetTab=テキストビュアーのタブストップ幅を設定\n"
		"FV:SetWidth=テキストビュアーの折り返し幅を設定\n"
		"FV:ShowCR=テキストビュアーで改行を表示/非表示\n"
		"FV:ShowLineNo=テキストビュアーの行番号を表示/非表示\n"
		"FV:ShowRuby=テキストビュアーでルビを表示/非表示\n"
		"FV:ShowRuler=テキストビュアーのルーラを表示/非表示\n"
		"FV:ShowTAB=テキストビュアーでタブを表示/非表示\n"
		"FV:ViewHistory=最近閲覧したファイル一覧\n"
		"FV:WebSearch=Webで検索\n"
		"FV:ZoomReset=ズームを解除\n"
		"FVL:CursorDown=カーソルを下に移動\n"
		"FVL:CursorUp=カーソルを上に移動\n"
		"FVI:AlphaBlend=メインウィンドウを透過表示\n"
		"FVI:AppList=アプリケーション一覧\n"
		"FVI:Calculator=電卓\n"
		"FVI:ClearMark=すべての栞マークを解除\n"
		"FVI:Close=閉じる\n"
		"FVI:CopyFileInfo=ファイル情報をクリップボードにコピー\n"
		"FVI:CmdFileList=コマンドファイル一覧\n"
		"FVI:DebugCmdFile=コマンドファイルのデバッグ実行\n"
		"FVI:Delete=削除\n"
		"FVI:Duplicate=NyanFiの二重起動\n"
		"FVI:ExeCommands=指定したコマンドを実行\n"
		"FVI:ExeMenuFile=メニューファイルの実行\n"
		"FVI:ExeToolBtn=ツールボタンの実行\n"
		"FVI:FileEdit=ファイルの編集\n"
		"FVI:InputCommands=入力したコマンドを実行\n"
		"FVI:ListFileInfo=ファイル情報をダイアログ表示\n"
		"FVI:ListLog=ログを一覧で表示\n"
		"FVI:Mark=栞マーク/解除\n"
		"FVI:MarkList=栞マーク一覧\n"
		"FVI:MenuBar=メニューバーの表示/非表示\n"
		"FVI:NextNyanFi=次のNyanFiをアクティブに\n"
		"FVI:PopupMainMenu=メインメニューをポップアップ表示\n"
		"FVI:PrevNyanFi=前のNyanFiをアクティブに\n"
		"FVI:PropertyDlg=プロパティダイアログを表示\n"
		"FVI:ScrollDown=下にスクロール\n"
		"FVI:ScrollUp=上にスクロール\n"
		"FVI:ShowFileInfo=ファイル情報を強制的に表示\n"
		"FVI:ShowFKeyBar=ファンクションキーバーの表示/非表示\n"
		"FVI:ShowStatusBar=ステータスバーの表示/非表示\n"
		"FVI:ShowToolBar=ツールバーを表示\n"
		"FVI:ToolBarDlg=ツールバーの設定\n"
		"FVI:WebMap=画像のGPS情報や指定地点の地図を開く\n"
		"FVI:WinPos=ウィンドウの四辺を設定\n"
		"FVIL:PageDown=1ページ下に移動\n"
		"FVIL:PageUp=1ページ上に移動\n"
		"FVI:ZoomIn=ズームイン\n"
		"FVI:ZoomOut=ズームアウト\n"
		"I:ColorPicker=カラーピッカー\n"
		"I:DoublePage=見開き表示\n"
		"I:EndFile=最後のファイルに移動\n"
		"I:EqualSize=等倍表示\n"
		"I:FittedSize=画面フィット表示\n"
		"I:FlipHorz=左右反転\n"
		"I:FlipVert=上下反転\n"
		"I:FullScreen=全画面表示\n"
		"I:GifViewer=GIFビュアーの表示/非表示\n"
		"I:GrayScale=グレースケール表示\n"
		"I:Histogram=ヒストグラムの表示/非表示\n"
		"I:JumpIndex=指定したインデックスに移動\n"
		"I:Loupe=ルーペの表示/非表示\n"
		"I:NextPage=サムネイルの次ページに移動\n"
		"I:PageBind=見開き表示の綴じ方向を設定\n"
		"I:PrevPage=サムネイルの前ページに移動\n"
		"I:Print=画像の印刷\n"
		"I:RotateLeft=左に90度回転\n"
		"I:RotateRight=右に90度回転\n"
		"I:ScrollLeft=左にスクロール\n"
		"I:ScrollRight=右にスクロール\n"
		"I:SendToWorkList=ワークリストに送る\n"
		"I:ShowGrid=画像分割グリッドの表示/非表示\n"
		"I:ShowSeekBar=シークバーの表示/非表示\n"
		"I:Sidebar=サイドバーの表示/非表示\n"
		"I:Thumbnail=サムネイルの表示/非表示\n"
		"I:ThumbnailEx=サムネイルの全面表示/通常表示\n"
		"I:TopFile=先頭ファイルに移動\n"
		"I:WarnHighlight=白飛び警告\n"
		"S:ClearIncKeyword=キーワードをクリア\n"
		"S:IncMatchSelect=マッチする項目をすべて選択\n"
		"S:IncSearchDown=マッチする項目を下方向検索\n"
		"S:IncSearchExit=インクリメンタルサーチから抜ける\n"
		"S:IncSearchUp=マッチする項目を上方向検索\n"
		"S:KeywordHistory=キーワード履歴を参照\n"
		"S:MigemoMode=Migomoモードの切り換え\n"
		"S:NormalMode=通常のサーチモードに戻る\n"
		"S:SelectDown=選択/解除後、下方向に検索\n"
		"V:BackViewHist=テキストビュアーの履歴を戻る\n"
		"V:BitmapView=ビットマップビューの表示/非表示\n"
		"V:BoxSelMode=箱形選択モードの開始/解除\n"
		"V:ChangeCodePage=文字コード変更\n"
		"V:ChangeViewMode=テキスト/バイナリ表示の切り換え\n"
		"V:CharInfo=文字情報の表示/非表示\n"
		"V:CsvCalc=CSV/TSV項目の集計\n"
		"V:CsvGraph=CSV/TSV項目のグラフ\n"
		"V:CsvRecord=CSV/TSVレコードの表示/非表示\n"
		"V:CursorLeft=カーソルを左に移動\n"
		"V:CursorLeftSel=選択しながらカーソルを左に移動\n"
		"V:CursorRight=カーソルを右に移動\n"
		"V:CursorRightSel=選択しながらカーソルを右に移動\n"
		"V:ExportCsv=CSV/TSVエクスポート\n"
		"V:FindDown=下方向に再検索\n"
		"V:FindLinkDown=リンク先を下方向に検索\n"
		"V:FindLinkUp=リンク先を上方向に検索\n"
		"V:FindMarkDown=マーク行を下方向に検索\n"
		"V:FindMarkUp=マーク行を上方向に検索\n"
		"V:FindSelDown=選択文字列を下方向に検索\n"
		"V:FindSelUp=選択文字列を上方向に検索\n"
		"V:FindText=文字列検索\n"
		"V:FindUp=上方向に再検索\n"
		"V:FunctionList=関数一覧\n"
		"V:HelpCurWord=カーソル位置の単語を指定ヘルプで検索\n"
		"V:Highlight=マッチ語を強調表示\n"
		"V:Inspector=インスペクタの表示/非表示\n"
		"V:ImgPreview=イメージプレビュー\n"
		"V:JumpLine=指定行番号に移動\n"
		"V:LineEnd=行末に移動\n"
		"V:LineEndSel=選択しながら行末に移動\n"
		"V:LineTop=行頭に移動\n"
		"V:LineTopSel=選択しながら行頭に移動\n"
		"V:OpenURL=URLを開く\n"
		"V:ReloadFile=ファイルの再読み込み\n"
		"V:SaveDump=ダンプリストをファイルに保存\n"
		"V:ScrollCursorDown=スクロールしながら下に移動\n"
		"V:ScrollCursorUp=スクロールしながら上に移動\n"
		"V:SearchPair=対応する括弧を検索\n"
		"V:SelCurWord=カーソル位置の単語を選択\n"
		"V:SelLine=カーソル行を選択\n"
		"V:SelLineCR=カーソル行全体を改行単位で選択\n"
		"V:SelectAll=すべて選択\n"
		"V:SelectFile=表示中ファイルを選択/解除\n"
		"V:SelectMode=選択モードの開始/解除\n"
		"V:SetTopAddr=先頭アドレスを設定\n"
		"V:SetUserDefStr=ユーザ定義文字列を設定\n"
		"V:Sort=テキスト全体を改行単位でソート\n"
		"V:SwitchSameName=ファイル名主部が同じ次のファイルに切り換え\n"
		"V:SwitchSrcHdr=ヘッダ/ソースファイルの切り換え\n"
		"V:TagJump=エディタでタグジャンプ\n"
		"V:TagJumpDirect=エディタでダイレクトタグジャンプ\n"
		"V:TagView=ビュアーでタグジャンプ\n"
		"V:TagViewDirect=ビュアーでダイレクトタグジャンプ\n"
		"V:TextEnd=最後尾に移動\n"
		"V:TextEndSel=選択しながら最後尾に移動\n"
		"V:TextTop=先頭に移動\n"
		"V:TextTopSel=選択しながら先頭に移動\n"
		"V:UserDefList=ユーザ定義文字列一覧\n"
		"V:WordLeft=前の単語に移動\n"
		"V:WordRight=次の単語に移動\n"
		"VIL:ClipCopy=クリップボードにコピー\n"
		"VI:NextFile=次のファイルを表示\n"
		"VI:PrevFile=前のファイルを表示\n"
		"VL:NextErr=次のエラー位置へ\n"
		"VL:PrevErr=前のエラー位置へ\n"
		"InitialSearch=頭文字サーチ\n";

	s_list->Clear();
	UnicodeString cmd, tmp;
	for (int i=0; i<c_list->Count; i++) {
		UnicodeString lbuf = c_list->Strings[i];
		if (!contains_s(lbuf, _T(':'))) continue;
		UnicodeString ct = split_tkn(lbuf, ':');
		c_list->Strings[i] = lbuf;
		cmd = get_tkn(lbuf, '=');
		for (int j=1; j<=ct.Length(); j++) {
			s_list->Add(tmp.sprintf(_T("%s:%s"), ct.SubString(j, 1).c_str(), cmd.c_str()));
		}
	}
	s_list->Sort();
}

//---------------------------------------------------------------------------
//コマンドのパラメータ一覧を取得
//---------------------------------------------------------------------------
void get_PrmList(
	UnicodeString cmd,		//コマンド
	int id_idx,				//"FSVIL" モードのインデックス(0ベース)
	TStringList *p_list,	//[o] パラメータ一覧
	TComboBox *cp)			//コンボボックス	(default = NULL)
{
	if (cp) {
		cp->Clear();
		cp->Style	= csDropDownList;
		cp->Text	= EmptyStr;
		cp->Enabled = false;

		if (cp && contained_wd_i(
			_T("AddTag|CalcDirSize|CalcDirSizeAll|Calculator|ChangeDir|ChangeOppDir|Clone|CloneToCurr|ContextMenu|")
			_T("ConvertHtm2Txt|Copy|CopyTo|CountLines|CreateDir|CursorDown|CursorUp|DateSelect|DebugCmdFile|")
			_T("DistributionDlg|ExeCommands|ExeMenuFile|FileEdit|FileRun|Filter|FindDown|FindFileDirDlg|FindFileDlg|")
			_T("FindTag|FindUp|FTPChmod|Grep|HelpCurWord|ImageViewer|IncSearch|JumpIndex|JumpLine|JumpTo|ListArchive|")
			_T("ListDuration|ListExpFunc|ListTail|ListText|ListTree|LoadBgImage|LoadTabGroup|LoadResultList|LoadWorkList|")
			_T("MaskFind|MaskSelect|MatchSelect|MoveTo|NewTextFile|OpenByApp|OpenByExp|OpenByWin|OpenStandard|OpenURL|")
			_T("Pack|PackToCurr|PlayList|PropertyDlg|RegExChecker|Restart|SaveAsTabGroup|ScrollCursorDown|ScrollCursorUp|")
			_T("ScrollDown|ScrollDownLog|ScrollDownText|ScrollUp|ScrollUpLog|ScrollUpText|SelByList|SetColor|SetDirTime|")
			_T("SetFontSize|SetMargin|SetPathMask|SetSttBarFmt|SetSubSize|SetTab|SetTag|SetUserDefStr|SetWidth|SortDlg|")
			_T("SubDirList|TagJumpDirect|TagSelect|TagViewDirect|TextViewer|ToTab|ViewTail|ToOppSameHash|WatchTail|")
			_T("WidenCurList|WinPos|GitDiff|GitViewer"),
			cmd))
		{
			cp->Enabled = true;
			cp->Style = USAME_TI(cmd, "ImageViewer")? csDropDownList : csDropDown;
		}
	}

	UnicodeString params;

	if (USAME_TI(cmd, "AddTab")) {
		params.USET_T("\nNX : 現タブの次に挿入\nPR : 現タブの前に挿入");
	}
	else if (USAME_TI(cmd, "AlphaBlend")) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\r\nIN : 透明度を入力\r\n");
	}
	else if (USAME_TI(cmd, "AppList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("FA : 一覧側にフォーカス\n")
			_T("FL : ランチャー側にフォーカス\n")
			_T("FI : ランチャー側にフォーカス(INC.サーチ)\n")
			_T("AO : 一覧のみ表示\n")
			_T("LO : ランチャーのみ表示\n")
			_T("LI : ランチャーのみ表示(INC.サーチ)\n"));
	}
	else if (USAME_TI(cmd, "BgImgMode")) {
		params.sprintf(_T("%s"),
			_T("OFF : 非表示\n")
			_T("1 : 2画面にわたって表示\n")
			_T("2 : それぞれに表示\n")
			_T("3 : デスクトップ背景を表示\n")
			_T("^1 : 2画面にわたって表示/非表示\n")
			_T("^2 : それぞれに表示/非表示\n")
			_T("^3 : デスクトップ背景を表示/非表示\n"));
	}
	else if (contained_wd_i(_T("CursorUp|CursorDown|ScrollCursorDown|ScrollCursorUp|ScrollDown|ScrollUp"), cmd)) {
		if (id_idx!=4) params.USET_T("\nHP : 半ページ分\nFP : 1ページ分\n");
	}
	else if (contained_wd_i(_T("CalcDirSize|CalcDirSizeAll"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("FC : ファイル数、サブディレクトリ数を表示\n")
			_T("LO : 結果をログに出力\n")
			_T("CC : 結果をクリップボードにコピー\n")
			_T("LS : 結果を一覧表示\n")
			_T("SA : 結果をサイズの小さい順にソート\n")
			_T("SD : 結果をサイズの大きい順にソート\n"));
		if (USAME_TI(cmd, "CalcDirSizeAll")) {
			params.UCAT_T("SG : グラフ表示(対カレント)\nDG : グラフ表示(対ドライブ)\n");
		}
	}
	else if (USAME_TI(cmd, "Calculator")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("CB : クリップボードを介して計算\n"));
	}
	else if (USAME_TI(cmd, "ChangeCodePage")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("932 :   Shift_JIS\n")
			_T("50220 : JIS(ISO-2022-JP)\n")
			_T("20932 : EUC-JP\n")
			_T("1252 :  Latin-1\n")
			_T("65001 : UTF-8\n")
			_T("1200 :  UTF-16\n"));
	}
	else if (contained_wd_i(_T("ChangeDrive|DriveGraph|EjectDrive"), cmd)) {
		for (int i=0; i<26; i++) params.cat_sprintf(_T("%c\n"), 'A'+i);	//英字
		if (USAME_TI(cmd, "EjectDrive")) params.UCAT_T(". : カレントドライブ\n");
		if (USAME_TI(cmd, "DriveGraph")) params = "\n" + params;
	}
	else if (USAME_TI(cmd, "CheckUpdate")) {
		params.USET_T("\nNC : 保存場所の選択、確認なし\n");
	}
	else if (USAME_TI(cmd, "ClearAll")) {
		params.USET_T("\nAL : 左右すべての選択状態を解除\n");
	}
	else if (USAME_TI(cmd, "ClearMark")) {
		params.USET_T("\nAC : すべての場所のすべてのマークを解除\n");
	}
	else if (USAME_TI(cmd, "ClipCopy")) {
		if (id_idx==2) {
			params.USET_T("\nAD : 現在の内容に追加\n");
		}
		else if (id_idx==3) {
			params.USET_T("\nVI : 表示されている状態でコピー\n");
		}
	}
	else if (USAME_TI(cmd, "Close") && id_idx==2) {
		params.USET_T("\nAL : すべての別ウィンドウを閉じる\n");
	}
	else if (USAME_TI(cmd, "CompareDlg")) {
		params.USET_T("\nNC : ダイアログを出さず、名前のみ比較\n");
	}
	else if (contained_wd_i(_T("CompareHash|GetHash|ToOppSameHash"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("MD5\n")
			_T("SHA1\n")
			_T("SHA256\n")
			_T("SHA384\n")
			_T("SHA512\n")
			_T("CRC32\n"));
		if (USAME_TI(cmd, "ToOppSameHash")) params.UCAT_T("NO : 反対側へ移動しない\n");
		if (USAME_TI(cmd, "CompareHash"))   params.UCAT_T("OS : 不一致ファイルを反対側で選択\n");
	}
	else if (USAME_TI(cmd, "CompressDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("UN : 圧縮属性を解除\n")
			_T("AL : すべての種類のファイルを圧縮\n"));
	}
	else if (USAME_TI(cmd, "ConvertDoc2Txt")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("SJ : Shift_JIS で出力(デフォルト)\n")
			_T("IJ : ISO-2022-JP で出力\n")
			_T("EJ : EUC-JP で出力\n")
			_T("U8 : UTF-8 で出力\n")
			_T("UL : UTF-16 で出力\n")
			_T("UB : UTF-16BE で出力\n"));
	}
	else if (USAME_TI(cmd, "ConvertHtm2Txt")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("HD : ヘッダ情報を挿入\n")
			_T("MD : Markdown記法に変換\n")
			_T("TX : 通常テキストに変換\n"));
	}
	else if (USAME_TI(cmd, "CopyDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("TO : コピー先を入力\n")
			_T("KT : タイムスタンプを維持\n")
			_T("CC : ディレクトリ名をクリップボードにコピー\n")
			_T("LS : ディレクトリ名を一覧表示\n"));
	}
	else if (contained_wd_i(_T("CopyFileName|ListFileName"), cmd)) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\nFN : ファイル名部分のみ\n");
	}
	else if (USAME_TI(cmd, "CreateDir")) {
		params.USET_T("\r\nIN : デフォルト名を指定して入力\r\n");
	}
	else if (contained_wd_i(_T("CsrDirToOpp|LinkToOpp|SwapLR"), cmd)) {
		params.USET_T("\nTO : 反対側へ移動\n");
	}
	else if (contained_wd_i(_T("CurrFromOpp|CurrToOpp"), cmd)) {
		params.USET_T("\nSL : 選択状態を反映\n");
		if (USAME_TI(cmd, "CurrToOpp")) params.UCAT_T("TO : 反対側へ移動\n");
	}
	else if (USAME_TI(cmd, "DeleteADS")) {
		params.USET_T("\nZI : Zone.Identifier のみ維持\n");
	}
	else if (USAME_TI(cmd, "DelJpgExif")) {
		params.USET_T("\nKT : タイムスタンプを維持\n");
	}
	else if (USAME_TI(cmd, "DiffDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("AL : マスク *.*、サブディレクトリも対象として直ちに比較実行\n"));
	}
	else if (USAME_TI(cmd, "DirHistory")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("GA : 全体履歴を表示\n")
			_T("GS : 全体履歴を表示(ソート/重複削除)\n")
			_T("FM : 全体履歴をフィルタ検索\n")
			_T("AC : カレント側の履歴をすべて削除\n")
			_T("GC : 全体履歴をすべて削除\n"));
	}
	else if (USAME_TI(cmd, "DistributionDlg")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("XC : 確認なしで直ちにコピー\n")
			_T("XM : 確認なしで直ちに移動\n")
			_T("SN : ファイルリストからマスクと振分先を設定\n"));
	}
	else if (USAME_TI(cmd, "DotNyanDlg")) {
		params.USET_T("\nRS : カレント側に .nyanfi を再適用\n");
	}
	else if (USAME_TI(cmd, "Duplicate")) {
		params.USET_T("\nRA : 管理者として二重起動\n");
	}
	else if (contained_wd_i(_T("EditHistory|ViewHistory"), cmd)) {
		params.USET_T("\nFF : フィルタ欄にフォーカス\nAC : 履歴をすべて消去\n");
	}
	else if (USAME_TI(cmd, "ExeCommandLine")) {
		params.USET_T("\nFN : カーソル位置のファイル名を入力\nLC : 前回のコマンドを初期表示\n");
	}
	else if (USAME_TI(cmd, "ExtractIcon")) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\nSI : スモールアイコンを抽出\n");
	}
	else if (USAME_TI(cmd, "FileEdit") && id_idx==0) {
		params.USET_T("\nOS : 反対側で選択中のファイルも開く\n");
	}
	else if (USAME_TI(cmd, "FileExtList")) {
		params.USET_T("\nCP : カーソル位置のディレクトリが対象\n");
	}
	else if (USAME_TI(cmd, "Filter")) {
		params.USET_T("\nCS : 大小文字を区別\nCA : 実行前に選択マスクを解除\n");
	}
	else if (contained_wd_i(_T("FindFileDlg|FindFileDirDlg"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("NM : マスク欄を非表示\n")
			_T("FK : 検索語欄にフォーカス\n")
			_T("R0 : 「サブディレクトリも検索」オフ\n")
			_T("R1 : 「サブディレクトリも検索」オン\n"));
		if (USAME_TI(cmd, "FindFileDlg")) {
			params.cat_sprintf(_T("%s"),
				_T("X0 : 「拡張検索」オフ\n")
				_T("X1 : 「拡張検索」オン\n")
				_T("A0 : 「アーカイブ内も検索」オフ\n")
				_T("A1 : 「アーカイブ内も検索」オン\n"));
		}
	}
	else if (USAME_TI(cmd, "FindHardLink")) {
		params.USET_T("\nOP : 結果リストから反対側へ反映\n");
	}
	else if (USAME_TI(cmd, "FindMark")) {
		params.USET_T("\nAL : すべてのマーク項目を検索\n");
	}
	else if (contained_wd_i(_T("FindTag|AddTag|SetTag|TagSelect"), cmd)) {
		params.USET_T("\n; : 入力ボックスでタグを指定\n");
		params += usr_TAG->TagNameList->Text;
	}
	else if (USAME_TI(cmd, "FindTagName")) {
		params.USET_T("\nEJ : テキストエディタでダイレクトタグジャンプ\n");
		if (id_idx==2) params.UCAT_T("CO : 現在のファイルのみを検索\n");
	}
	else if (contained_wd_i(_T("FindSelDown|FindSelUp"), cmd)) {
		params.USET_T("\nEM : マッチ語を強調表示\n");
	}
	else if (USAME_TI(cmd, "FixTabPath")) {
		params.USET_T("\nON : カレント側を固定\nOFF : 固定解除\n");
	}
	else if (USAME_TI(cmd, "GitDiff")) {
		params.USET_T("\nHD : HEAD から差分を表示\nXT : 外部diffツールで表示\n");
	}
	else if (USAME_TI(cmd, "GitViewer")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("N0 : コミット履歴数の制限を外す\n")
			_T("N30 : コミット履歴数を制限する\n")
			_T("CP : カーソル位置ファイルのコミット履歴を表示\n"));
	}
	else if (USAME_TI(cmd, "HelpContents")) {
		params.USET_T("\nCI : コマンドの索引\nFI : コマンドの機能別索引\nCH : 変更履歴\n");
	}
	else if (USAME_TI(cmd, "IncSearch")) {
		params.USET_T("\nMM : Migomoモード\nNM : 通常モード\n");
		if (id_idx==0) params.UCAT_T("FM : フィルタマスク・モード\nCA : 実行前に選択マスクを解除\n");
	}
	else if (USAME_TI(cmd, "InputDir")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : ダイアログを表示しないで入力\n")
			_T("ND2 : ND でドロップダウンを開いて表示\n")
			_T("SD : フォルダ参照ダイアログを表示\n"));
	}
	else if (USAME_TI(cmd, "DriveList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : ポップアップメニューで選択\n")
			_T("NS : ポップアップメニューで選択(空き容量非表示)\n"));
	}
	else if (USAME_TI(cmd, "InputCommands")) {
		params.USET_T("\nEL : エコー、コマンドファイルの行番号表示\n");
	}
	else if (contained_wd_i(_T("CmdFileList|ListClipboard|MarkList|KeyList|FunctionList|RepositoryList|UserDefList"), cmd)) {
		params.USET_T("\nFF : フィルタ欄にフォーカス\n");
	}
	else if (contained_wd_i(_T("Exit|Close"), cmd)) {
		params.USET_T("\nNS : INIファイルを保存しない\nNX : 他のNyanFiを終了させない\n");
	}
	else if (USAME_TI(cmd, "ExPopupMenu")) {
		params.USET_T("\nMN : 追加メニューのみ表示\nTL : 外部ツールのみ表示\n");
	}
	else if (USAME_TI(cmd, "Library")) {
		params.USET_T("\nSD : 選択ダイアログを表示\n* : 選択メニューを表示\n");
		UnicodeString pnam = cv_env_str("%APPDATA%\\Microsoft\\Windows\\Libraries\\");
		std::unique_ptr<TStringList> lst(new TStringList());
		get_files(pnam, _T("*.library-ms"), lst.get());
		for (int i=0; i<lst->Count; i++)
			params.cat_sprintf(_T("%s\n"), get_base_name(lst->Strings[i]).c_str());
	}
	else if (contained_wd_i(_T("CountLines|ListArchive|ListDuration|ListExpFunc|ListNyanFi"), cmd)) {
		params.USET_T("\nCC : 結果をクリップボードにコピー\nLS : 結果を一覧表示\n");
	}
	else if (USAME_TI(cmd, "ListLog")) {
		params.USET_T("\nEO : エラー箇所の絞り込み表示\n");
	}
	else if (USAME_TI(cmd, "ListTail")) {
		params.USET_T("\nTE : 最後尾に移動r\nFF : フィルタ欄にフォーカス\n");
	}
	else if (USAME_TI(cmd, "ListText")) {
		params.USET_T("\nFF : フィルタ欄にフォーカス\nEO : エラー箇所の絞り込み表示\n");
	}
	else if (USAME_TI(cmd, "Mark") && (id_idx==0 || id_idx==3)) {
		params.USET_T("\nND : カーソルを移動しない\nIM : メモを入力\n");
	}
	else if (USAME_TI(cmd, "MoveTab")) {
		params.USET_T("\nTP : 先頭に先頭\nED : 最後に移動\nPR : １つ前に移動\n");
	}
	else if (USAME_TI(cmd, "PageBind")) {
		params.USET_T("\nR : 右綴じ\nL : 左綴じ\n");
	}
	else if (USAME_TI(cmd, "Paste")) {
		params.USET_T("\nCL : 同名時にクローン化\n");
	}
	else if (contained_wd_i(_T("NextFile|PrevFile"), cmd)) {
		if (id_idx==3) params.USET_T("\nF1 : 見開き表示でも1ファイルずつ移動\n");
	}
	else if (USAME_TI(cmd, "NextNyanFi")) {
		params.USET_T("\nDN : なければ二重起動\n");
	}
	else if (USAME_TI(cmd, "PlayList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("RP : リピート再生\n")
			_T("SF : シャッフル再生\n")
			_T("SR : シャッフル・リピート再生\n")
			_T("NX : 次の曲へ\n")
			_T("PR : 前の曲へ\n")
			_T("PS : 一時停止\n")
			_T("RS : 再開\n")
			_T("FI : ファイル情報を表示\n")
			_T("LS : プレイリストを表示\n"));
	}
	else if (USAME_TI(cmd, "RecentList")) {
		params.USET_T("\nAC : 最近使ったすべての項目を削除\nBC : リンク切れ項目を整理\n");
	}
	else if (contained_wd_i(_T("RegDirDlg|PathMaskDlg"), cmd)) {
		params.USET_T("\nND : ポップアップメニューで選択\n");
		if (USAME_TI(cmd, "RegDirDlg")) params.UCAT_T("AD : 追加モード\n");
	}
	else if (contained_wd_i(_T("RegDirPopup|PushDir|PopDir"), cmd)) {
		params.USET_T("\nOP : 反対側で実行\n");
	}
	else if (USAME_TI(cmd, "ReloadList")) {
		params.USET_T("\nCO : カレントのみ更新\nHL : ハードリンクのタイムスタンプ更新\nOFF : 更新禁止\n");
	}
	else if (USAME_TI(cmd, "Restart")) {
		params.USET_T("\nNS : INIファイルを保存しない\nRA : 管理者として再起動\nDM : 管理者から降格して再起動\n");
	}
	else if (USAME_TI(cmd, "SaveAsWorkList")) {
		params.USET_T("\nFL : カレントの内容をワークリストとして保存\n");
	}
	else if (contained_wd_i(_T("ScrollUpLog|ScrollUpText|ScrollDownLog|ScrollDownText"), cmd)) {
		params.USET_T("\nHP : 半ページ分\nFP : 1ページ分\n");
		params.cat_sprintf(_T("%s\n"), contains_i(cmd, _T("Down"))? _T("ED : 最後へ") : _T("TP : 先頭へ"));
	}
	else if (USAME_TI(cmd, "SelByList")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("LR : カレント/反対側の両方で選択\n")
			_T("SM : 選択された項目だけを残す(選択マスク)\n")
			_T("CP : カーソル位置のリストファイルでカレント側を選択\n")
			_T("OP : カーソル位置のリストファイルで反対側を選択\n"));
	}
	else if (USAME_TI(cmd, "Select") && (id_idx==0 || id_idx==3)) {
		if (cp) cp->Style = csDropDown;
		params.USET_T("\n");
		if (id_idx==0) params.UCAT_T("IN : 繰り返し回数を入力\n");
		params.UCAT_T("ND : カーソルを移動しない\n");
	}
	else if (USAME_TI(cmd, "SelectFile")) {
		params.USET_T("\nNX : 次のファイルを表示\n");
	}
	else if (USAME_TI(cmd, "SelEmptyDir")) {
		params.USET_T("\nNF : ファイルが含まれていなければ選択\n");
	}
	else if (contained_wd_i(_T("MarkMask|SelMask"), cmd)) {
		params.USET_T("\nCA : マスクを解除\n");
	}
	else if (USAME_TI(cmd, "SetColor")) {
		params.USET_T("\nRS : 配色をリセット\n");
	}
	else if (USAME_TI(cmd, "SetFolderIcon")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ND : ポップアップメニューで選択\n")
			_T("RS : デフォルトアイコンに戻す\n"));
	}
	else if (USAME_TI(cmd, "SetPathMask")) {
		params.USET_T("\nEX : カーソル位置の拡張子でマスク\n");
	}
	else if (USAME_TI(cmd, "SetTopAddr")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("TP : 先頭アドレスを先頭に\n")
			_T("NX : 先頭アドレスを後続部に\n")
			_T("PR : 先頭アドレスを先行部に\n")
			_T("ED : 終端アドレスを最後に\n"));
	}
	else if (USAME_TI(cmd, "ShowFileInfo") && id_idx!=2) {
		params.USET_T("\nSD : ダイアログで表示\n");
	}
	else if (USAME_TI(cmd, "Sort")) {
		params.USET_T("\nAO : 昇順\nDO : 降順\n");
	}
	else if (USAME_TI(cmd, "SortDlg")) {
		params.sprintf(_T("%s"),
		 	_T("\n")
			_T("F : 名前順\n")
			_T("E : 拡張子順\n")
			_T("D : 更新日時順\n")
			_T("S : サイズ順\n")
			_T("A : 属性順\n")
			_T("U : なし\n")
			_T("L : 場所順(結果リスト)\n")
			_T("FE : 名前/拡張子順 トグル切り替え\n")
			_T("FD : 名前/更新日時順 トグル切り替え\n")
			_T("FS : 名前/サイズ順 トグル切り替え\n")
			_T("ED : 拡張子/更新日時順 トグル切り替え\n")
			_T("ES : 拡張子/サイズ順 トグル切り替え\n")
			_T("DS : 更新日時順/サイズ順 トグル切り替え\n")
			_T("IV : 現在のソート方法を逆順に\n")
			_T("IA : すべてのソート方法を逆順に\n")
			_T("XN : ディレクトリ - ファイルと同じ\n")
			_T("XF : ディレクトリ - 名前\n")
			_T("XD : ディレクトリ - 更新日時\n")
			_T("XS : ディレクトリ - サイズ\n")
			_T("XA : ディレクトリ - 属性\n")
			_T("XX : ディレクトリを区別しない\n")
			_T("XI : アイコン(ファイルが名前/拡張子の場合)\n")
			_T("XNX : ファイルと同じ/ディレクトリを区別しない トグル切り替え\n")
			_T("XNI : ファイルと同じ/アイコン トグル切り替え\n"));
	}
	else if (USAME_TI(cmd, "SubDirList")) {
		params.USET_T("\nND : ポップアップメニューで選択\n");
	}
	else if (USAME_TI(cmd, "SwapName")) {
		params.USET_T("\nLR : 左右で入れ替え\n");
	}
	else if (USAME_TI(cmd, "TabHome")) {
		params.USET_T("\nAL : すべてのタブに適用\nCO : カレントのみに適用\n");
	}
	else if (contained_wd_i("TagJump|TagView", cmd)) {
		params.USET_T("\nDJ : 見つからなければダイレクトタグジャンプ\n");
	}
	else if (contained_wd_i(_T("TextViewer|ImageViewer"), cmd)) {
		params.USET_T("\nCB : クリップボードの内容を表示\nNN : 次のNyanFiで表示\n");
		if (USAME_TI(cmd, "TextViewer")) params.UCAT_T("XW : 別ウィンドウで表示\n");
	}
	else if (USAME_TI(cmd, "ToOppSameItem")) {
		params.USET_T("\nNO : 反対側へ移動しない\n");
	}
	else if (contained_wd_i(_T("ToParent|ToParentOnLeft|ToParentOnRight"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("DL : ルートならドライブ/共有フォルダ一覧を表示\n")
			_T("DP : ルートならドライブ選択メニューを表示\n"));
	}
	else if (contained_wd_i(_T("UnPack|UnPackToCurr"), cmd)) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("CD : ディレクトリを作成してその中に\n")
			_T("CD2 : ルートに複数の対象があったらディレクトリ作成\n"));
	}
	else if (USAME_TI(cmd, "UpdateFromArc")) {
		params.USET_T("\nUN : 新しいアーカイブを探して更新\n");
	}
	else if (contained_wd_i("ViewIniFile|ViewLog", cmd)) {
		params.UCAT_T("\nXW : 別ウィンドウで表示\n");
	}
	else if (USAME_TI(cmd, "WatchTail")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("ST : 監視内容を表示\n")
			_T("CC : カーソル位置ファイルの監視を中止\n")
			_T("AC : すべての監視を中止\n"));
	}
	if (USAME_TI(cmd, "WebMap")) {
		params.USET_T("\nIN : 緯度,経度を入力\n");
	}
	else if (USAME_TI(cmd, "WebSearch")) {
		params.USET_T("\nCB : クリップボードの内容を検索\n");
		if (id_idx==0) params.UCAT_T("FN : カーソル位置のファイル名を検索\n");
	}
	else if (USAME_TI(cmd, "WorkList") && id_idx==0) {
		params.USET_T("\nOP : 反対側で実行\nRL : 変更内容を破棄して読み込み直す\nDI : 無効な項目を一括削除\n");
	}
	else if (contained_wd_i(_T("ZoomIn|ZoomOut"), cmd)) {
		params.USET_T("\n");
		for (int i=2; i<=12; i++) params.cat_sprintf(_T("%u\n"), i);
	}
	else if (contained_wd_i(_T("Copy|Move|Delete|CompleteDelete"), cmd)) {
		params.USET_T("\nSO : 選択項目のみ処理\n");
		if (USAME_TI(cmd, "Copy")) {
			params.cat_sprintf(_T("%s"),
				_T("OP : 反対側コピー先のカーソル位置を設定\nOP2 : コピー先のカーソル位置を逐次設定\n")
			 	_T("TO : コピー先を入力\nSD : コピー先を参照\nSS : カレントのサブディレクトリを選択\n"));
		}
		if (USAME_TI(cmd, "Move")) {
			params.cat_sprintf(_T("%s"),
				_T("OP : 反対側移動先のカーソル位置を設定\nOP2 : 移動先のカーソル位置を逐次設定\n")
			 	_T("TO : 移動先を入力\nSD : 移動先を参照\nSS : カレントのサブディレクトリを選択\n"));
		}
	}
	//トグル動作コマンド
	else if (contained_wd_i(
		_T("FileListOnly|HideSizeTime|LockTextPreview|MenuBar|MuteVolume|")
		_T("ShowByteSize|ShowFKeyBar|ShowHideAtr|ShowIcon|ShowPreview|ShowProperty|ShowStatusBar|ShowSystemAtr|ShowTabBar|")
		_T("PauseAllTask|Suspend|SyncLR|UseTrash|")
		_T("BitmapView|CharInfo|CsvRecord|Highlight|HtmlToText|Inspector|ShowCR|ShowLineNo|ShowRuby|ShowRuler|ShowTAB|FixedLen|")
		_T("DoublePage|FullScreen|GifViewer|SubViewer|GrayScale|Histogram|Loupe|ShowGrid|ShowSeekBar|Sidebar|")
		_T("Thumbnail|ThumbnailEx|WarnHighlight"),
		cmd))
	{
		params.USET_T("\nON : 表示/有効\nOFF : 非表示/無効/解除\n");
		if (USAME_TI(cmd, "SubViewer"))
			params.UCAT_T("LK : ロック/解除\nRL : 左に90度回転\nRR : 右に90度回転\nFH : 左右反転\nFV : 上下反転\n");
		if (USAME_TI(cmd, "ShowIcon"))
			params.UCAT_T("FD : 全表示/フォルダアイコンのみ表示\n");
	}
	//オプション設定
	else if (USAME_TI(cmd, "OptionDlg")) {
		params.sprintf(_T("%s"),
			_T("\n")
			_T("GN : 一般\n")
			_T("MO : マウス操作\n")
			_T("DS : デザイン\n")
			_T("TV : テキストビュアー\n")
			_T("IV : イメージビュアー\n")
			_T("ED : エディタ\n")
			_T("AC : 関連付け\n")
			_T("ST : 起動時\n")
			_T("XM : 追加メニュー\n")
			_T("XT : 外部ツール\n")
			_T("KY : キー設定\n")
			_T("NT : 通知・確認・ヒント\n")
			_T("CM : コマンド\n")
			_T("EV : イベント\n"));
	}

	p_list->Text = params;
	if (!cp && p_list->Count>0) {
		if (p_list->Strings[0].IsEmpty()) p_list->Delete(0);
	}
}

//---------------------------------------------------------------------------
//ファイル/ディレクトリ参照が必要か?
//---------------------------------------------------------------------------
bool need_RefDirFile(UnicodeString cmd)
{
	return contained_wd_i(
		_T("CalcDirSize|CalcDirSizeAll|ChangeDir|ChangeOppDir|ContextMenu|CopyTo|DebugCmdFile|DistributionDlg|")
		_T("ExeCommands|ExeMenuFile|FileEdit|FileRun|JumpTo|ListArchive|ListDuration|ListExpFunc|ListTail|")
		_T("ListText|ListTree|LoadBgImage|LoadTabGroup|LoadResultList|LoadWorkList|MoveTo|OpenByApp|OpenByExp|")
		_T("OpenByWin|OpenStandard|PropertyDlg|PlayList|TextViewer|SelByList|SetColor|SetFolderIcon|SubDirList|")
		_T("HelpCurWord|Restart"),
		cmd);
}

//---------------------------------------------------------------------------
//コマンドパラメータから説明文字列を削除
//---------------------------------------------------------------------------
UnicodeString del_CmdDesc(UnicodeString cmd)
{
	UnicodeString prm = get_PrmStr(cmd);
	cmd = get_CmdStr(cmd);
	split_dsc(prm);
	if (!prm.IsEmpty()) cmd.cat_sprintf(_T("_%s"), prm.c_str());
	return cmd;
}

//---------------------------------------------------------------------------
//カーソルキーをコマンドに変換
//---------------------------------------------------------------------------
UnicodeString get_CsrKeyCmd(UnicodeString key_str)
{
	switch (idx_of_word_i(_T("DOWN|UP|LEFT|RIGHT"), key_str)) {
	case  0: return "CursorDown";
	case  1: return "CursorUp";
	case  2: return "ToLeft";
	case  3: return "ToRight";
	default: return EmptyStr;
	}
}
//---------------------------------------------------------------------------
