object AppListDlg: TAppListDlg
  Left = 0
  Top = 0
  HelpContext = 76
  BorderIcons = [biSystemMenu]
  ClientHeight = 289
  ClientWidth = 390
  Color = clBtnFace
  DoubleBuffered = True
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
  OnMouseWheel = FormMouseWheel
  OnShow = FormShow
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 270
    Width = 390
    Height = 19
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clBtnText
    Font.Height = -13
    Font.Name = 'Meiryo UI'
    Font.Pitch = fpVariable
    Font.Style = []
    Panels = <
      item
        Style = psOwnerDraw
        Width = 600
      end>
    UseSystemFont = False
    StyleElements = [seBorder]
    OnDrawPanel = StatusBar1DrawPanel
    ExplicitTop = 269
    ExplicitWidth = 386
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 390
    Height = 270
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 386
    ExplicitHeight = 269
    object LRSplitter: TSplitter
      Left = 285
      Top = 0
      Width = 5
      Height = 270
      Align = alRight
      AutoSnap = False
      Color = clBtnFace
      ParentColor = False
    end
    object LaunchPanel: TPanel
      Left = 290
      Top = 0
      Width = 100
      Height = 270
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitLeft = 286
      ExplicitHeight = 269
      object DirPanel: TPanel
        Left = 0
        Top = 22
        Width = 100
        Height = 22
        Align = alTop
        Alignment = taLeftJustify
        PopupMenu = PopupMenu2
        TabOrder = 0
        StyleElements = [seBorder]
        OnDblClick = DirPanelDblClick
      end
      object LaunchListBox: TListBox
        Left = 0
        Top = 44
        Width = 100
        Height = 226
        Style = lbVirtualOwnerDraw
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        DoubleBuffered = True
        ItemHeight = 13
        ParentDoubleBuffered = False
        PopupMenu = PopupMenu2
        TabOrder = 1
        StyleElements = []
        OnClick = LaunchListBoxClick
        OnData = LaunchListBoxData
        OnDblClick = LaunchListBoxDblClick
        OnDrawItem = LaunchListBoxDrawItem
        OnEnter = LaunchListBoxEnter
        OnKeyDown = LaunchListBoxKeyDown
        ExplicitHeight = 225
      end
      object InpPanel: TPanel
        Left = 0
        Top = 0
        Width = 100
        Height = 22
        Align = alTop
        TabOrder = 2
        object InpPaintBox: TPaintBox
          Left = 1
          Top = 1
          Width = 98
          Height = 20
          Align = alClient
          OnPaint = InpPaintBoxPaint
        end
      end
    end
    object AppPanel: TPanel
      Left = 0
      Top = 0
      Width = 285
      Height = 270
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitWidth = 281
      ExplicitHeight = 269
      object ViewSplitter: TSplitter
        Left = 0
        Top = 160
        Width = 285
        Height = 5
        Cursor = crVSplit
        Align = alBottom
        AutoSnap = False
        Color = clBtnFace
        ParentColor = False
      end
      object ViewPanel: TPanel
        Left = 0
        Top = 165
        Width = 285
        Height = 105
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        StyleElements = []
        OnDblClick = ViewPanelDblClick
        OnMouseDown = ViewPanelMouseDown
        OnMouseUp = ViewPanelMouseUp
        ExplicitTop = 164
        ExplicitWidth = 281
      end
      object AppListPanel: TPanel
        Left = 0
        Top = 0
        Width = 285
        Height = 160
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        ExplicitWidth = 281
        ExplicitHeight = 159
        object AppListBox: TListBox
          Left = 0
          Top = 0
          Width = 273
          Height = 160
          Style = lbVirtualOwnerDraw
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          DoubleBuffered = True
          ParentDoubleBuffered = False
          PopupMenu = PopupMenu1
          TabOrder = 0
          StyleElements = []
          OnClick = AppListBoxClick
          OnData = AppListBoxData
          OnDblClick = AppListBoxDblClick
          OnDrawItem = AppListBoxDrawItem
          OnEnter = AppListBoxEnter
          OnKeyDown = AppListBoxKeyDown
          ExplicitWidth = 269
          ExplicitHeight = 159
        end
      end
    end
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 200
    Top = 96
    object MinWinItem: TMenuItem
      Action = MinimizeAction
    end
    object MaxWinItem: TMenuItem
      Action = MaximizeAction
    end
    object RstWinItem: TMenuItem
      Action = RestoreAction
    end
    object ToPrimaryItem: TMenuItem
      Action = ToPrimaryAction
    end
    object FitToFileListItem: TMenuItem
      Action = FitToFileListAction
    end
    object TopMostItem: TMenuItem
      Action = TopMostAction
    end
    object DesktopItem: TMenuItem
      Caption = #12487#12473#12463#12488#12483#12503#12434#34920#31034'(&D)'
      OnClick = DesktopItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object CloseItem: TMenuItem
      Caption = #38281#12376#12427'(&C)'
      ShortCut = 46
      OnClick = CloseItemClick
    end
    object CloseSameItem: TMenuItem
      Caption = #12414#12392#12417#12390#38281#12376#12427'(&S)'
      ShortCut = 16430
      OnClick = CloseSameItemClick
    end
    object TerminateItem: TMenuItem
      Caption = #24375#21046#32066#20102'(&K)'
      OnClick = TerminateItemClick
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object CopyNameItem: TMenuItem
      Caption = #23455#34892#12501#12449#12452#12523#21517#12434#12467#12500#12540'(&F)'
      OnClick = CopyNameItemClick
    end
    object JumpExeItem: TMenuItem
      Caption = #23455#34892#12501#12449#12452#12523#20301#32622#12395#31227#21205'(&J)'
      OnClick = JumpExeItemClick
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object AppInfoItem: TMenuItem
      Action = AppInfoAction
    end
    object ShowMonNoItem: TMenuItem
      Action = ShowMonNoAction
    end
    object ShowCmdParamItem: TMenuItem
      Action = ShowCmdParamAction
    end
    object ExcAppTextItem: TMenuItem
      Caption = #38500#22806#12486#12461#12473#12488#12434#35373#23450'(&E)'
      OnClick = ExcAppTextItemClick
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 16
    Top = 16
  end
  object PopupMenu2: TPopupMenu
    OnPopup = PopupMenu2Popup
    Left = 322
    Top = 96
    object SelLaunhDirItem: TMenuItem
      Caption = #12488#12483#12503#12487#12451#12524#12463#12488#12522#12398#35373#23450'(&T)'
      OnClick = SelLaunhDirItemClick
    end
    object JumpLaunchDirItem: TMenuItem
      Caption = #12371#12398#12487#12451#12524#12463#12488#12522#12395#31227#21205'(&J)'
      OnClick = JumpLaunchDirItemClick
    end
    object JumpLinkItem: TMenuItem
      Action = JumpLinkedAction
    end
    object Sep_l_1: TMenuItem
      Caption = '-'
    end
    object LaunchPosItem: TMenuItem
      Caption = #12521#12531#12481#12515#12540#12398#34920#31034#20301#32622'(&P)'
      object LaunchRightItem: TMenuItem
        Tag = 1
        Caption = #21491#12395#34920#31034'(&R)'
        OnClick = LaunchPosItemClick
      end
      object LaunchLeftItem: TMenuItem
        Tag = 2
        Caption = #24038#12395#34920#31034'(&L)'
        OnClick = LaunchPosItemClick
      end
    end
    object Sep_l_2: TMenuItem
      Caption = '-'
    end
    object UpdateIcoItem: TMenuItem
      Caption = #12450#12452#12467#12531#12434#26356#26032'(&I)'
      OnClick = UpdateIcoItemClick
    end
    object SortByIconItem: TMenuItem
      Action = SortByIconAction
    end
    object SortByRemItem: TMenuItem
      Action = SortByRemAction
    end
    object Sep_l_3: TMenuItem
      Caption = '-'
    end
    object PropertyItem: TMenuItem
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnClick = PropertyItemClick
    end
  end
  object ActionList1: TActionList
    Left = 322
    Top = 32
    object MinimizeAction: TAction
      Caption = #26368#23567#21270'(&N)'
      OnExecute = MinimizeActionExecute
      OnUpdate = MinimizeActionUpdate
    end
    object MaximizeAction: TAction
      Caption = #26368#22823#21270'(&X)'
      OnExecute = MaximizeActionExecute
      OnUpdate = MaximizeActionUpdate
    end
    object RestoreAction: TAction
      Caption = #20803#12398#12469#12452#12474#12395#25147#12377'(&R)'
      OnExecute = RestoreActionExecute
      OnUpdate = RestoreActionUpdate
    end
    object TopMostAction: TAction
      Caption = #24120#12395#25163#21069#12395#34920#31034'(&T)'
      OnExecute = TopMostActionExecute
      OnUpdate = TopMostActionUpdate
    end
    object ToPrimaryAction: TAction
      Caption = #12503#12521#12452#12510#12522#12514#12491#12479#12395#31227#21205'(&G)'
      OnExecute = ToPrimaryActionExecute
      OnUpdate = ToPrimaryActionUpdate
    end
    object AppInfoAction: TAction
      Caption = #12450#12503#12522#12465#12540#12471#12519#12531#24773#22577'(&A)'
      OnExecute = AppInfoActionExecute
    end
    object JumpLinkedAction: TAction
      Caption = #12522#12531#12463#20808#12398#12501#12449#12452#12523#20301#32622#12395#31227#21205'(&L)'
      OnExecute = JumpLinkedActionExecute
      OnUpdate = JumpLinkedActionUpdate
    end
    object FitToFileListAction: TAction
      Caption = #12501#12449#12452#12523#12522#12473#12488#12395#21512#12431#12379#12427'(&L)'
      OnExecute = FitToFileListActionExecute
      OnUpdate = FitToFileListActionUpdate
    end
    object ShowMonNoAction: TAction
      Caption = #12514#12491#12479#30058#21495#12434#34920#31034'(&M)'
      OnExecute = ShowMonNoActionExecute
      OnUpdate = ShowMonNoActionUpdate
    end
    object ShowCmdParamAction: TAction
      Caption = #12467#12510#12531#12489#12521#12452#12531#12497#12521#12513#12540#12479#12434#34920#31034'(&P)'
      OnExecute = ShowCmdParamActionExecute
    end
    object SortByIconAction: TAction
      Caption = #12501#12457#12523#12480#12450#12452#12467#12531#12391#12477#12540#12488'(&F)'
      OnExecute = SortByIconActionExecute
      OnUpdate = SortByIconActionUpdate
    end
    object SortByRemAction: TAction
      Caption = #12467#12513#12531#12488#12391#12477#12540#12488'(&S)'
      OnExecute = SortByRemActionExecute
      OnUpdate = SortByRemActionUpdate
    end
  end
end
