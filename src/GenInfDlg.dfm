object GeneralInfoDlg: TGeneralInfoDlg
  Left = 0
  Top = 0
  HelpContext = 73
  BorderIcons = [biSystemMenu]
  ClientHeight = 512
  ClientWidth = 470
  Color = clBtnFace
  Constraints.MinWidth = 320
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 492
    Width = 470
    Height = 20
    DoubleBuffered = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Pitch = fpVariable
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
    Width = 470
    Height = 492
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object OpePanel: TPanel
      Left = 0
      Top = 458
      Width = 470
      Height = 34
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      object FilterPanel: TPanel
        Left = 0
        Top = 0
        Width = 350
        Height = 34
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        TabStop = True
        DesignSize = (
          350
          34)
        object MigemoCheckBox: TCheckBox
          Left = 136
          Top = 8
          Width = 72
          Height = 17
          Anchors = [akTop, akRight]
          Caption = '&Migemo'
          TabOrder = 1
          OnClick = FilterOptheckBoxClick
        end
        object FilterEdit: TLabeledEdit
          Left = 66
          Top = 6
          Width = 64
          Height = 21
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 47
          EditLabel.Height = 13
          EditLabel.Caption = #12501#12451#12523#12479'(&F)'
          LabelPosition = lpLeft
          PopupMenu = UserModule.EditPopupMenuE
          TabOrder = 0
          OnChange = FilterEditChange
          OnEnter = FilterEditEnter
          OnExit = FilterEditExit
          OnKeyDown = FilterEditKeyDown
          OnKeyPress = FilterEditKeyPress
        end
        object HighlightCheckBox: TCheckBox
          Left = 288
          Top = 8
          Width = 58
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #24375#35519
          TabOrder = 3
          OnClick = HighlightCheckBoxClick
        end
        object AndOrCheckBox: TCheckBox
          Left = 210
          Top = 8
          Width = 76
          Height = 17
          Anchors = [akTop, akRight]
          Caption = '&AND/OR'
          TabOrder = 2
          OnClick = FilterOptheckBoxClick
        end
      end
      object TailPanel: TPanel
        Left = 350
        Top = 0
        Width = 120
        Height = 34
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 1
        object WatchCheckBox: TCheckBox
          Left = 0
          Top = 8
          Width = 56
          Height = 17
          Caption = #30435#35222
          TabOrder = 0
          OnClick = WatchCheckBoxClick
        end
        object NotifyCheckBox: TCheckBox
          Left = 58
          Top = 8
          Width = 56
          Height = 17
          Caption = #36890#30693
          TabOrder = 1
        end
      end
    end
    object ListPanel: TPanel
      Left = 0
      Top = 0
      Width = 470
      Height = 458
      Align = alClient
      BevelOuter = bvNone
      DoubleBuffered = False
      ParentDoubleBuffered = False
      TabOrder = 1
      DesignSize = (
        470
        458)
      object GenListBox: TListBox
        Left = 0
        Top = 0
        Width = 470
        Height = 458
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
  end
  object InfPopupMenu: TPopupMenu
    OnPopup = InfPopupMenuPopup
    Left = 48
    Top = 86
    object InfEditCopyItem: TMenuItem
      Action = CopyAction
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
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object SaveAsItem: TMenuItem
      Action = SaveAsAction
    end
    object ViewListItem: TMenuItem
      Action = ViewListAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object ViewFileItem: TMenuItem
      Action = ViewFileAction
    end
    object EditFileItem: TMenuItem
      Action = EditFileAction
    end
    object Sep_4: TMenuItem
      Caption = '-'
    end
    object ShowFileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object R2: TMenuItem
      Action = PropertyAction
    end
    object Sep_5: TMenuItem
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
    object SelAllAction: TAction
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = SelAllActionExecute
    end
    object SaveAsAction: TAction
      Caption = #19968#35239#12434#12501#12449#12452#12523#12395#20445#23384'(&S)...'
      OnExecute = SaveAsActionExecute
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
      OnUpdate = ShowLineNoActionUpdate
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
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 48
    Top = 152
  end
end
