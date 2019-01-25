object FileExtensionDlg: TFileExtensionDlg
  Left = 0
  Top = 0
  HelpContext = 72
  BorderIcons = [biSystemMenu]
  ClientHeight = 448
  ClientWidth = 590
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
  object ListSplitter: TSplitter
    Left = 0
    Top = 299
    Width = 590
    Height = 4
    Cursor = crVSplit
    Align = alBottom
    StyleElements = [seFont, seBorder]
    OnMoved = ListSplitterMoved
  end
  object InfoPanel: TPanel
    Left = 0
    Top = 0
    Width = 590
    Height = 299
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object FextInfHeader: THeaderControl
      Left = 0
      Top = 0
      Width = 590
      Height = 17
      Sections = <
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 80
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 100
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 200
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 100
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 50
        end>
      OnDrawSection = FextHeaderDrawSection
      OnSectionClick = FextInfHeaderSectionClick
    end
    object FextInfBar: TStatusBar
      Left = 0
      Top = 280
      Width = 590
      Height = 19
      DoubleBuffered = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Pitch = fpVariable
      Font.Style = []
      Panels = <
        item
          Bevel = pbNone
          Style = psOwnerDraw
          Width = 50
        end
        item
          Bevel = pbNone
          Style = psOwnerDraw
          Width = 100
        end
        item
          Bevel = pbNone
          Style = psOwnerDraw
          Width = 100
        end
        item
          Bevel = pbNone
          Style = psOwnerDraw
          Width = 50
        end
        item
          Bevel = pbNone
          Width = 50
        end>
      ParentDoubleBuffered = False
      SizeGrip = False
      UseSystemFont = False
      StyleElements = []
      OnDrawPanel = FextInfBarDrawPanel
    end
    object ListPanel: TPanel
      Left = 0
      Top = 17
      Width = 590
      Height = 263
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 3
      object InfoListBox: TListBox
        Left = 0
        Top = 0
        Width = 590
        Height = 263
        Style = lbVirtualOwnerDraw
        AutoComplete = False
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        MultiSelect = True
        ParentShowHint = False
        PopupMenu = PopupMenu1
        ShowHint = False
        TabOrder = 0
        StyleElements = [seBorder]
        OnClick = InfoListBoxClick
        OnDrawItem = InfoListBoxDrawItem
        OnKeyDown = InfoListBoxKeyDown
        OnKeyPress = InfoListBoxKeyPress
      end
    end
    object HintPanel: TPanel
      Left = 115
      Top = 137
      Width = 240
      Height = 60
      BevelOuter = bvNone
      TabOrder = 1
      Visible = False
      StyleElements = [seBorder]
      object HintLabel: TLabel
        Left = 114
        Top = 23
        Width = 12
        Height = 13
        Alignment = taCenter
        Caption = '    '
      end
    end
  end
  object FilePanel: TPanel
    Left = 0
    Top = 303
    Width = 590
    Height = 145
    Align = alBottom
    BevelOuter = bvNone
    Constraints.MinHeight = 50
    TabOrder = 1
    object FileInfBar: TStatusBar
      Left = 0
      Top = 125
      Width = 590
      Height = 20
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Pitch = fpVariable
      Font.Style = []
      Panels = <
        item
          Bevel = pbNone
          Width = 600
        end>
      UseSystemFont = False
      StyleElements = []
    end
    object FileListBox: TListBox
      Tag = 1
      Left = 0
      Top = 17
      Width = 590
      Height = 108
      Style = lbVirtualOwnerDraw
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      ItemHeight = 13
      MultiSelect = True
      ParentShowHint = False
      PopupMenu = PopupMenu1
      ShowHint = False
      TabOrder = 1
      StyleElements = [seBorder]
      OnClick = FileListBoxClick
      OnData = FileListBoxData
      OnDrawItem = FileListBoxDrawItem
      OnEnter = FileListBoxEnter
      OnExit = FileListBoxExit
      OnKeyDown = FileListBoxKeyDown
    end
    object HiddenCanBtn: TButton
      Left = 0
      Top = 145
      Width = 590
      Height = 0
      Align = alBottom
      Cancel = True
      ModalResult = 2
      TabOrder = 3
      TabStop = False
    end
    object FileListHeader: THeaderControl
      Tag = 1
      Left = 0
      Top = 0
      Width = 590
      Height = 17
      Sections = <
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 200
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 200
        end>
      OnDrawSection = FextHeaderDrawSection
      OnSectionClick = FileListHeaderSectionClick
      OnSectionResize = FileListHeaderSectionResize
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 72
    Top = 48
    object FextCopyItem: TMenuItem
      Action = FextClipCopyAction
    end
    object FextLogOutItem: TMenuItem
      Action = FextLogOutAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object FextSaveItem: TMenuItem
      Action = FextSaveAction
    end
    object SaveCsvItem: TMenuItem
      Action = FextCsvAction
    end
    object SaveTsvItem: TMenuItem
      Action = FextTsvAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object MaskFindItem: TMenuItem
      Action = FextMaskFindAction
      Caption = #25313#24373#23376#12391#12510#12473#12463#26908#32034'(&M)'
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object SortExtItem: TMenuItem
      Action = SortFextAction
    end
    object SortCntItem: TMenuItem
      Tag = 1
      Action = SortCntAction
    end
    object SortSizeItem: TMenuItem
      Tag = 2
      Action = SortSizeAction
    end
    object SortNameItem: TMenuItem
      Action = SortNameAction
    end
    object SortDirItem: TMenuItem
      Action = SortDirAction
    end
    object Sep_4: TMenuItem
      Caption = '-'
    end
    object FileInfoItem: TMenuItem
      Action = ShowFileInfoAction
    end
    object RropertyItem: TMenuItem
      Action = PropertyAction
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 144
    Top = 48
    object FextClipCopyAction: TAction
      Caption = #19968#35239#12434#12463#12522#12483#12503#12508#12540#12489#12395#12467#12500#12540'(&C)'
      OnExecute = FextClipCopyActionExecute
      OnUpdate = FextOutActionUpdate
    end
    object FextLogOutAction: TAction
      Caption = #19968#35239#12434#12525#12464#12395#20986#21147'(&L)'
      OnExecute = FextLogOutActionExecute
      OnUpdate = FextOutActionUpdate
    end
    object FextSaveAction: TAction
      Caption = #19968#35239#12434#12501#12449#12452#12523#12395#20445#23384'(&A)...'
      OnExecute = FextSaveActionExecute
      OnUpdate = FextOutActionUpdate
    end
    object FextCsvAction: TAction
      Tag = 1
      Caption = #19968#35239#12434'CSV'#24418#24335#12391#20445#23384'...'
      OnExecute = FextSaveActionExecute
      OnUpdate = FextCsvActionUpdate
    end
    object FextTsvAction: TAction
      Tag = 2
      Caption = #19968#35239#12434'TSV'#24418#24335#12391#20445#23384'...'
      OnExecute = FextSaveActionExecute
      OnUpdate = FextCsvActionUpdate
    end
    object SortFextAction: TAction
      Caption = #12477#12540#12488': '#25313#24373#23376'(&E)'
      OnExecute = SortFextActionExecute
      OnUpdate = SortFextActionUpdate
    end
    object SortCntAction: TAction
      Tag = 1
      Caption = #12477#12540#12488': '#12501#12449#12452#12523#25968'(&F)'
      OnExecute = SortFextActionExecute
      OnUpdate = SortFextActionUpdate
    end
    object SortSizeAction: TAction
      Tag = 2
      Caption = #12477#12540#12488': '#12469#12452#12474'(&S)'
      OnExecute = SortFextActionExecute
      OnUpdate = SortFextActionUpdate
    end
    object SortNameAction: TAction
      Caption = #12477#12540#12488': '#12501#12449#12452#12523#21517'(&F)'
      OnExecute = SortFileActionExecute
      OnUpdate = SortFileActionUpdate
    end
    object SortDirAction: TAction
      Tag = 1
      Caption = #12477#12540#12488': '#22580#25152'(&D)'
      OnExecute = SortFileActionExecute
      OnUpdate = SortFileActionUpdate
    end
    object FextMaskFindAction: TAction
      Caption = #12501#12449#12452#12521#12540#12391#25313#24373#23376#12434#26908#32034'(&M)'
      OnExecute = FextMaskFindActionExecute
      OnUpdate = FextOutActionUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
      OnUpdate = PropertyActionUpdate
    end
    object ShowFileInfoAction: TAction
      Caption = #12501#12449#12452#12523#24773#22577'(&I)'
      OnExecute = ShowFileInfoActionExecute
      OnUpdate = PropertyActionUpdate
    end
  end
end
