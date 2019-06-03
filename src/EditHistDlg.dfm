object EditHistoryDlg: TEditHistoryDlg
  Left = 0
  Top = 0
  HelpContext = 58
  BorderIcons = [biSystemMenu]
  ClientHeight = 341
  ClientWidth = 630
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object HiddenCanBtn: TButton
    Left = 0
    Top = 303
    Width = 630
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 0
  end
  object EditHistHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 630
    Height = 17
    Sections = <>
    Style = hsFlat
    OnDrawSection = EditHistHeaderDrawSection
    OnSectionClick = EditHistHeaderSectionClick
    OnSectionResize = EditHistHeaderSectionResize
    OnResize = EditHistHeaderResize
  end
  object GridPanel: TPanel
    Left = 0
    Top = 17
    Width = 630
    Height = 286
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      630
      286)
    object EditHistGrid: TStringGrid
      Left = 0
      Top = 0
      Width = 630
      Height = 303
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clAppWorkSpace
      ColCount = 4
      DefaultDrawing = False
      FixedCols = 0
      FixedRows = 0
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      Options = [goFixedVertLine, goColSizing, goRowSelect, goThumbTracking]
      ParentFont = False
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = EditHistGridClick
      OnDblClick = EditHistGridDblClick
      OnDrawCell = EditHistGridDrawCell
      OnKeyDown = EditHistGridKeyDown
      ColWidths = (
        64
        64
        64
        64)
      RowHeights = (
        24
        24
        24
        24
        24)
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 322
    Width = 630
    Height = 19
    Panels = <
      item
        Style = psOwnerDraw
        Width = 50
      end>
    StyleElements = [seBorder]
    OnDrawPanel = StatusBar1DrawPanel
  end
  object OpeToolBar: TToolBar
    Left = 0
    Top = 303
    Width = 630
    Height = 19
    Align = alBottom
    AutoSize = True
    ButtonHeight = 19
    ButtonWidth = 85
    Caption = 'OpeToolBar'
    DrawingStyle = dsGradient
    List = True
    ShowCaptions = True
    AllowTextButtons = True
    TabOrder = 3
    object FilterBtn: TToolButton
      Left = 0
      Top = 0
      Caption = #12501#12451#12523#12479'(&F)'
      ImageIndex = 3
      Style = tbsTextButton
      OnClick = FilterBtnClick
    end
    object FilterEdit: TEdit
      Left = 63
      Top = 0
      Width = 121
      Height = 19
      Align = alLeft
      Constraints.MinWidth = 60
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
      OnChange = FilterEditChange
      OnEnter = FilterEditEnter
      OnExit = FilterEditExit
      OnKeyDown = FilterEditKeyDown
      OnKeyPress = FilterEditKeyPress
    end
    object FilterSplitter: TSplitter
      Left = 184
      Top = 0
      Width = 4
      Height = 19
    end
    object MigemoBtn: TToolButton
      Left = 188
      Top = 0
      Action = MigemoAction
      Style = tbsTextButton
    end
    object OpenBtn: TToolButton
      Left = 235
      Top = 0
      Action = OpenAction
      Style = tbsTextButton
    end
    object ToolButton5: TToolButton
      Left = 298
      Top = 0
      Width = 20
      Caption = 'ToolButton5'
      ImageIndex = 0
      Style = tbsSeparator
    end
    object MemoBtn: TToolButton
      Left = 318
      Top = 0
      Action = MemoAction
      Style = tbsTextButton
    end
    object OptMode0Btn: TToolButton
      Left = 362
      Top = 0
      Action = OptMode0Action
      Grouped = True
      Style = tbsTextButton
    end
    object OptMode1Btn: TToolButton
      Left = 441
      Top = 0
      Action = OptMode1Action
      Grouped = True
      Style = tbsTextButton
    end
    object OptMode2Btn: TToolButton
      Left = 525
      Top = 0
      Action = OptMode2Action
      Grouped = True
      Style = tbsTextButton
    end
  end
  object MarkPopupMenu: TPopupMenu
    Left = 144
    Top = 64
    object MemoItem: TMenuItem
      Caption = #12513#12514#12434#20837#21147'(&M)'
    end
    object ClrMemoItem: TMenuItem
      Caption = #12513#12514#12434#21066#38500'(&C)'
      OnClick = ClrMemoItemClick
    end
    object Sep_m_1: TMenuItem
      Caption = '-'
    end
    object DelMarkItem: TMenuItem
      Caption = #12510#12540#12463#35299#38500'(&D)'
      OnClick = DelMarkItemClick
    end
    object ClrAllMarkItem: TMenuItem
      Caption = #12377#12409#12390#12398#12510#12540#12463#12434#35299#38500'(&A)'
      OnClick = ClrAllMarkItemClick
    end
    object Sep_m_2: TMenuItem
      Caption = '-'
    end
    object mFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object mPropertyItem: TMenuItem
      Action = ShowPropertyAction
    end
    object Sep_m_3: TMenuItem
      Caption = '-'
    end
    object mStatusBarItem: TMenuItem
      Action = ShowStatusBarAction
    end
    object mFitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object HistPopupMenu: TPopupMenu
    Left = 48
    Top = 64
    object ClearAllItem: TMenuItem
      Caption = #23653#27508#12434#12377#12409#12390#21066#38500'(&A)'
      OnClick = ClearAllItemClick
    end
    object NoHistItem: TMenuItem
      Caption = #23653#27508#12395#20837#12428#12394#12356#12497#12473#12398#35373#23450'(&N)...'
      OnClick = NoHistItemClick
    end
    object Sep_h_1: TMenuItem
      Caption = '-'
    end
    object hFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object hPropertyItem: TMenuItem
      Action = ShowPropertyAction
    end
    object Sep_h_2: TMenuItem
      Caption = '-'
    end
    object hStatusBarItem: TMenuItem
      Action = ShowStatusBarAction
    end
    object hFitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object RecentPopupMenu: TPopupMenu
    Left = 248
    Top = 65
    object CleAllRecentItem: TMenuItem
      Action = ClrAllRecentAction
    end
    object ClearBrokenItem: TMenuItem
      Action = ClrBrkRecentAction
    end
    object Sep_r_1: TMenuItem
      Caption = '-'
    end
    object ShowUsedTimeItem: TMenuItem
      Action = ShowUsedTimeAction
    end
    object rFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object rPropertyItem: TMenuItem
      Action = ShowPropertyAction
    end
    object Sep_r_2: TMenuItem
      Caption = '-'
    end
    object rStatusBarItem: TMenuItem
      Action = ShowStatusBarAction
    end
    object rFitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 496
    Top = 65
    object ClrAllRecentAction: TAction
      Caption = #38917#30446#12434#12377#12409#12390#21066#38500'(&A)'
      OnExecute = ClrAllRecentActionExecute
      OnUpdate = ClrAllRecentActionUpdate
    end
    object ShowUsedTimeAction: TAction
      Caption = #20351#29992#26085#26178#12434#34920#31034'(&U)'
      OnExecute = ShowUsedTimeActionExecute
    end
    object ShowPropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = ShowPropertyActionExecute
      OnUpdate = ShowFileInfoActionUpdate
    end
    object ShowFileInfoAction: TAction
      Caption = #12501#12449#12452#12523#24773#22577'(&I)'
      OnExecute = ShowFileInfoActionExecute
      OnUpdate = ShowFileInfoActionUpdate
    end
    object ClrBrkRecentAction: TAction
      Caption = #12522#12531#12463#20999#12428#38917#30446#12434#25972#29702'(&B)'
      OnExecute = ClrBrkRecentActionExecute
    end
    object ShowStatusBarAction: TAction
      Caption = #12473#12486#12540#12479#12473#12496#12540'(&S)'
      OnExecute = ShowStatusBarActionExecute
    end
    object MigemoAction: TAction
      Caption = '&Migemo'
      OnExecute = ToggleActionExecute
    end
    object OpenAction: TAction
      Caption = #30906#23450#12391#38283#12367
      OnExecute = ToggleActionExecute
    end
    object MemoAction: TAction
      Caption = #12513#12514'(&O)'
      OnExecute = MemoActionExecute
    end
    object OptMode0Action: TAction
      Caption = #12377#12409#12390#34920#31034'(&A)'
      GroupIndex = 1
      OnExecute = OptModeActionExecute
    end
    object OptMode1Action: TAction
      Tag = 1
      Caption = #12459#12524#12531#12488#20197#19979'(&B)'
      GroupIndex = 1
      OnExecute = OptModeActionExecute
    end
    object OptMode2Action: TAction
      Tag = 2
      Caption = #12459#12524#12531#12488#12398#12415'(&C)'
      GroupIndex = 1
      OnExecute = OptModeActionExecute
    end
    object UpdateGitInfAction: TAction
      Caption = #26368#26032#12398#24773#22577#12395#26356#26032
      OnExecute = UpdateGitInfActionExecute
      OnUpdate = UpdateGitInfActionUpdate
    end
    object GitViewerAction: TAction
      Caption = 'Git'#12499#12517#12450#12540#12434#38283#12367'(&V)'
      OnExecute = GitViewerActionExecute
      OnUpdate = ShowFileInfoActionUpdate
    end
  end
  object RepoPopupMenu: TPopupMenu
    Left = 360
    Top = 65
    object UpdGitInfItem: TMenuItem
      Action = UpdateGitInfAction
    end
    object Sep_g_1: TMenuItem
      Caption = '-'
    end
    object GitViewerItem: TMenuItem
      Action = GitViewerAction
    end
    object gFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object gPropertyItem: TMenuItem
      Action = ShowPropertyAction
    end
    object Sep_g_2: TMenuItem
      Caption = '-'
    end
    object gStatusBarItem: TMenuItem
      Action = ShowStatusBarAction
    end
    object gFitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
end
