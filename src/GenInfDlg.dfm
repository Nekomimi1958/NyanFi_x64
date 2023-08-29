object GeneralInfoDlg: TGeneralInfoDlg
  Left = 0
  Top = 0
  HelpContext = 73
  BorderIcons = [biSystemMenu]
  ClientHeight = 512
  ClientWidth = 530
  Color = clBtnFace
  Constraints.MinWidth = 320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 492
    Width = 530
    Height = 20
    DoubleBuffered = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Panels = <
      item
        Style = psOwnerDraw
        Width = 240
      end
      item
        Style = psOwnerDraw
        Width = 100
      end
      item
        Style = psOwnerDraw
        Width = 80
      end
      item
        Style = psOwnerDraw
        Width = 50
      end>
    ParentDoubleBuffered = False
    UseSystemFont = False
    StyleElements = [seBorder]
    OnDrawPanel = StatusBar1DrawPanel
  end
  object MianPanel: TPanel
    Left = 0
    Top = 0
    Width = 530
    Height = 492
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object ListPanel: TPanel
      Left = 0
      Top = 0
      Width = 530
      Height = 450
      Align = alClient
      BevelOuter = bvNone
      DoubleBuffered = False
      ParentDoubleBuffered = False
      TabOrder = 0
      DesignSize = (
        530
        450)
      object GenListBox: TListBox
        Left = 0
        Top = 0
        Width = 526
        Height = 450
        Style = lbVirtualOwnerDraw
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Constraints.MinHeight = 32
        Constraints.MinWidth = 32
        DoubleBuffered = False
        ItemHeight = 13
        MultiSelect = True
        ParentDoubleBuffered = False
        PopupMenu = InfPopupMenu
        TabOrder = 0
        StyleElements = [seBorder]
        OnClick = GenListBoxClick
        OnData = GenListBoxData
        OnDataObject = GenListBoxDataObject
        OnDblClick = GenListBoxDblClick
        OnDrawItem = GenListBoxDrawItem
        OnExit = GenListBoxExit
        OnKeyDown = GenListBoxKeyDown
        OnKeyPress = GenListBoxKeyPress
      end
    end
    object OpeToolBar: TToolBar
      Left = 0
      Top = 450
      Width = 530
      Height = 42
      Align = alBottom
      AutoSize = True
      ButtonHeight = 21
      ButtonWidth = 62
      DrawingStyle = dsGradient
      List = True
      ShowCaptions = True
      AllowTextButtons = True
      TabOrder = 1
      object FilterBtn: TToolButton
        Left = 0
        Top = 0
        Caption = #12501#12451#12523#12479'(&F)'
        ImageIndex = 0
        Style = tbsTextButton
        OnClick = FilterBtnClick
      end
      object FilterEdit: TEdit
        Left = 66
        Top = 0
        Width = 120
        Height = 21
        Align = alLeft
        Constraints.MinWidth = 60
        PopupMenu = UserModule.EditPopupMenuE
        TabOrder = 0
        OnChange = FilterEditChange
        OnClick = FilterEditChange
        OnEnter = FilterEditEnter
        OnExit = FilterEditExit
        OnKeyDown = FilterEditKeyDown
        OnKeyPress = FilterEditKeyPress
      end
      object FilterSplitter: TSplitter
        Left = 186
        Top = 0
        Width = 4
        Height = 21
      end
      object MigemoBtn: TToolButton
        Left = 190
        Top = 0
        Action = MigemoAction
        Style = tbsTextButton
      end
      object AndOrBtn: TToolButton
        Left = 246
        Top = 0
        Action = AndOrAction
        Wrap = True
        Style = tbsTextButton
      end
      object HighlightBtn: TToolButton
        Left = 0
        Top = 21
        Action = HighlightAction
        Style = tbsTextButton
      end
      object ToolButton5: TToolButton
        Left = 54
        Top = 21
        Width = 8
        ImageIndex = 4
        Style = tbsSeparator
      end
      object WatchBtn: TToolButton
        Left = 62
        Top = 21
        Action = WatchAction
        Style = tbsTextButton
      end
      object NotifyBtn: TToolButton
        Left = 118
        Top = 21
        Action = NotifyAction
        Style = tbsTextButton
      end
      object PlayPrevBtn: TToolButton
        Left = 172
        Top = 21
        Action = PlayPrevAction
        Style = tbsTextButton
      end
      object PlayPauseBtn: TToolButton
        Left = 199
        Top = 21
        Action = PlayPauseAction
        Style = tbsTextButton
      end
      object PlayNextBtn: TToolButton
        Left = 226
        Top = 21
        Action = PlayNextAction
        Style = tbsTextButton
      end
    end
  end
  object InfPopupMenu: TPopupMenu
    OnPopup = InfPopupMenuPopup
    Left = 48
    Top = 86
    object InfEditCopyItem: TMenuItem
      Action = CopyAction
    end
    object InfEditCopyValItem: TMenuItem
      Action = CopyValAction
    end
    object InfEditSelectAllItem: TMenuItem
      Action = SelAllAction
    end
    object FindTextItem: TMenuItem
      Caption = #26908#32034'(&F)...'
      OnClick = FindTextItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object ClrCmdHistoryItem: TMenuItem
      Action = ClrCmdHistoryAction
    end
    object CopyCmdItem: TMenuItem
      Action = CopyCmdAction
    end
    object SaveAsNbtItem: TMenuItem
      Action = SaveAsNbtAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object SortItem: TMenuItem
      Caption = #19968#35239#12434#12477#12540#12488'(&O)'
      object SortAscItem: TMenuItem
        Action = SortAscAction
      end
      object SortDesItem: TMenuItem
        Action = SortDesAction
      end
      object OrgOrderItem: TMenuItem
        Action = OrgOrderAction
      end
    end
    object DelDuplItem: TMenuItem
      Action = DelDuplAction
    end
    object RestoreListItem: TMenuItem
      Action = RestoreListAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object SaveAsItem: TMenuItem
      Action = SaveAsAction
    end
    object ViewListItem: TMenuItem
      Action = ViewListAction
    end
    object Sep_4: TMenuItem
      Caption = '-'
    end
    object ViewFileItem: TMenuItem
      Action = ViewFileAction
    end
    object EditFileItem: TMenuItem
      Action = EditFileAction
    end
    object Sep_5: TMenuItem
      Caption = '-'
    end
    object ShowFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
    object Sep_6: TMenuItem
      Caption = '-'
    end
    object ShowLineNoItem: TMenuItem
      Action = ShowLineNoAction
    end
    object FileName1stItem: TMenuItem
      Action = FileName1stAction
    end
    object OmitComPathItem: TMenuItem
      Action = OmitComPathAction
    end
    object KeepIndexItem: TMenuItem
      Action = KeepIndexAction
    end
    object ErrOnlyItem: TMenuItem
      Action = ErrOnlyAction
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 13
    object CopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object CopyValAction: TAction
      Caption = #12459#12540#12477#12523#34892#12398#20516#12434#12467#12500#12540'(&X)'
      OnExecute = CopyValActionExecute
      OnUpdate = CopyValActionUpdate
    end
    object SelAllAction: TAction
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = SelAllActionExecute
    end
    object SaveAsAction: TAction
      Caption = #19968#35239#12434#12501#12449#12452#12523#12395#20445#23384'(&S)...'
      OnExecute = SaveAsActionExecute
    end
    object ClrCmdHistoryAction: TAction
      Caption = #12467#12510#12531#12489#23653#27508#12434#12463#12522#12450
      OnExecute = ClrCmdHistoryActionExecute
      OnUpdate = ClrCmdHistoryActionUpdate
    end
    object CopyCmdAction: TAction
      Caption = #12467#12510#12531#12489#12434#12467#12500#12540'(&B)'
      OnExecute = CopyCmdActionExecute
      OnUpdate = CopyCmdActionUpdate
    end
    object SaveAsNbtAction: TAction
      Caption = #36984#25246#34892#12434#12467#12510#12531#12489#12501#12449#12452#12523#12392#12375#12390#20445#23384'...'
      OnExecute = SaveAsNbtActionExecute
      OnUpdate = CopyCmdActionUpdate
    end
    object ViewListAction: TAction
      Caption = #19968#35239#12434#12486#12461#12473#12488#12499#12517#12450#12540#12391#38283#12367'(&V)'
      OnExecute = ViewListActionExecute
      OnUpdate = ViewListActionUpdate
    end
    object ViewFileAction: TAction
      Caption = #12501#12449#12452#12523#12434#12486#12461#12473#12488#12499#12517#12450#12540#12391#38283#12367'(&V)'
      OnExecute = ViewFileActionExecute
      OnUpdate = OpenFileActionUpdate
    end
    object EditFileAction: TAction
      Caption = #12501#12449#12452#12523#12434#12486#12461#12473#12488#12456#12487#12451#12479#12391#38283#12367'(&E)'
      ShortCut = 32837
      OnExecute = EditFileActionExecute
      OnUpdate = OpenFileActionUpdate
    end
    object ShowLineNoAction: TAction
      Caption = #34892#30058#21495#12434#34920#31034'(&N)'
      OnExecute = ShowLineNoActionExecute
    end
    object FileName1stAction: TAction
      Caption = #12501#12449#12452#12523#21517#12434#20808#12395#34920#31034'(&F)'
      OnExecute = FileName1stActionExecute
      OnUpdate = FileName1stActionUpdate
    end
    object OmitComPathAction: TAction
      Caption = #12497#12473#12398#20849#36890#37096#20998#12434#30465#30053'(&P)'
      OnExecute = OmitComPathActionExecute
      OnUpdate = OmitComPathActionUpdate
    end
    object KeepIndexAction: TAction
      Caption = #12459#12540#12477#12523#20301#32622#12434#32173#25345'(&K)'
      OnExecute = KeepIndexActionExecute
      OnUpdate = KeepIndexActionUpdate
    end
    object ErrOnlyAction: TAction
      Caption = #12456#12521#12540#31623#25152#12398#32094#12426#36796#12415'(&X)'
      OnExecute = ErrOnlyActionExecute
      OnUpdate = ErrOnlyActionUpdate
    end
    object SortAscAction: TAction
      Caption = #26119#38918' (&A)'
      OnExecute = SortAscActionExecute
      OnUpdate = ListActionUpdate
    end
    object SortDesAction: TAction
      Caption = #38477#38918'(&D)'
      OnExecute = SortDesActionExecute
      OnUpdate = ListActionUpdate
    end
    object OrgOrderAction: TAction
      Caption = #35299#38500'(&R)'
      OnExecute = OrgOrderActionExecute
      OnUpdate = ListActionUpdate
    end
    object DelDuplAction: TAction
      Caption = #19968#35239#12398#37325#35079#34892#12434#38500#22806'(&D)'
      OnExecute = DelDuplActionExecute
      OnUpdate = ListActionUpdate
    end
    object RestoreListAction: TAction
      Caption = #19968#35239#12398#20869#23481#12434#20803#12395#25147#12377
      OnExecute = RestoreListActionExecute
      OnUpdate = ListActionUpdate
    end
    object ShowFileInfoAction: TAction
      Caption = #12501#12449#12452#12523#24773#22577'(&I)'
      OnExecute = ShowFileInfoActionExecute
      OnUpdate = PropertyActionUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
      OnUpdate = PropertyActionUpdate
    end
    object MigemoAction: TAction
      Caption = '&Migemo'
      OnExecute = ToggleActionExecute
    end
    object AndOrAction: TAction
      Caption = '&ADN/OR'
      OnExecute = ToggleActionExecute
    end
    object HighlightAction: TAction
      Caption = #24375#35519'(&H)'
      OnExecute = ToggleActionExecute
    end
    object WatchAction: TAction
      Caption = #30435#35222'(&W)'
      OnExecute = ToggleActionExecute
      OnUpdate = TailActionUpdate
    end
    object NotifyAction: TAction
      Caption = #36890#30693'(&N)'
      OnExecute = ToggleActionExecute
      OnUpdate = TailActionUpdate
    end
    object PlayNextAction: TAction
      Caption = '>>'
      Hint = #27425#12398#26354#12434#20877#29983
      OnExecute = PlayNextActionExecute
      OnUpdate = PlayActionUpdate
    end
    object PlayPrevAction: TAction
      Caption = '<<'
      Hint = #21069#12398#26354#12434#20877#29983
      OnExecute = PlayPrevActionExecute
      OnUpdate = PlayActionUpdate
    end
    object PlayPauseAction: TAction
      Caption = #9655'||'
      OnExecute = PlayPauseActionExecute
      OnUpdate = PlayPauseActionUpdate
    end
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 48
    Top = 152
  end
end
