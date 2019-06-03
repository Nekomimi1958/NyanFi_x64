object RegDirDlg: TRegDirDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #30331#37682#12487#12451#12524#12463#12488#12522
  ClientHeight = 341
  ClientWidth = 592
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
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object OptPanel: TPanel
    Left = 0
    Top = 252
    Width = 592
    Height = 70
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    DesignSize = (
      592
      70)
    object HideOptBtn: TSpeedButton
      Left = 574
      Top = 54
      Width = 18
      Height = 16
      Anchors = [akTop, akRight]
      Flat = True
      OnClick = ChgOptBtnClick
    end
    object UpButton: TButton
      Left = 224
      Top = 37
      Width = 30
      Height = 26
      Action = UserModule.UpListItemAction
      TabOrder = 8
    end
    object DowButton: TButton
      Left = 256
      Top = 37
      Width = 30
      Height = 26
      Action = UserModule.DownListItemAction
      TabOrder = 9
    end
    object EditButton: TButton
      Left = 79
      Top = 37
      Width = 60
      Height = 26
      Action = EditItemAction
      TabOrder = 6
    end
    object AddButton: TButton
      Left = 16
      Top = 37
      Width = 60
      Height = 26
      Action = AddItemAction
      TabOrder = 5
    end
    object DelButton: TButton
      Left = 142
      Top = 37
      Width = 60
      Height = 26
      Action = DelItemAction
      TabOrder = 7
    end
    object DescEdit: TEdit
      Left = 59
      Top = 8
      Width = 163
      Height = 21
      Hint = #21517#21069
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 1
    end
    object DirEdit: TEdit
      Left = 224
      Top = 8
      Width = 270
      Height = 21
      Hint = #29872#22659#22793#25968#12418#20351#29992#21487#33021
      Anchors = [akLeft, akTop, akRight]
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 2
    end
    object RefDirBtn: TButton
      Left = 496
      Top = 8
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 3
      OnClick = RefDirBtnClick
    end
    object KeyEdit: TEdit
      Left = 16
      Top = 8
      Width = 41
      Height = 21
      Hint = #12461#12540
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
    end
    object MoveTopCheckBox: TCheckBox
      Left = 316
      Top = 41
      Width = 171
      Height = 17
      Caption = #21628#20986#38917#30446#12434#20808#38957#12408
      TabOrder = 10
    end
    object RefSpBtn: TButton
      Left = 533
      Top = 8
      Width = 55
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #29305#27530'...'
      TabOrder = 4
      OnClick = RefSpBtnClick
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 252
    Width = 592
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 1
  end
  object RegDirHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 592
    Height = 20
    Sections = <
      item
        Alignment = taCenter
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 100
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 100
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 100
      end>
    Style = hsFlat
    OnDrawSection = RegDirHeaderDrawSection
    OnSectionResize = RegDirHeaderSectionResize
    OnResize = RegDirHeaderResize
  end
  object ListPanel: TPanel
    Left = 0
    Top = 20
    Width = 592
    Height = 232
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      592
      232)
    object RegDirListBox: TListBox
      Left = 0
      Top = 0
      Width = 592
      Height = 232
      Style = lbOwnerDrawFixed
      AutoComplete = False
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      ItemHeight = 13
      MultiSelect = True
      ParentDoubleBuffered = False
      ParentFont = False
      ParentShowHint = False
      PopupMenu = PopupMenu1
      ShowHint = True
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = RegDirListBoxClick
      OnDblClick = RegDirListBoxDblClick
      OnDrawItem = RegDirListBoxDrawItem
      OnKeyDown = RegDirListBoxKeyDown
      OnKeyPress = RegDirListBoxKeyPress
    end
    object BlankPanel: TPanel
      Left = 574
      Top = 216
      Width = 18
      Height = 16
      Anchors = [akRight, akBottom]
      AutoSize = True
      BevelOuter = bvNone
      Color = clAppWorkSpace
      ParentBackground = False
      TabOrder = 1
      Visible = False
      StyleElements = [seBorder]
      object ShowOptBtn: TSpeedButton
        Left = 0
        Top = 0
        Width = 18
        Height = 16
        Flat = True
        OnClick = ChgOptBtnClick
      end
    end
  end
  object OpeToolBar: TToolBar
    Left = 0
    Top = 322
    Width = 592
    Height = 19
    Align = alBottom
    AutoSize = True
    ButtonHeight = 19
    ButtonWidth = 59
    DrawingStyle = dsGradient
    List = True
    ShowCaptions = True
    AllowTextButtons = True
    TabOrder = 4
    object FilterBtn: TToolButton
      Left = 0
      Top = 0
      Caption = #12501#12451#12523#12479'(&F)'
      ImageIndex = 0
      Style = tbsTextButton
      OnClick = FilterBtnClick
    end
    object FilterEdit: TEdit
      Left = 63
      Top = 0
      Width = 120
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
      Left = 183
      Top = 0
      Width = 4
      Height = 19
    end
    object MigemoBtn: TToolButton
      Left = 187
      Top = 0
      Action = MigemoAction
      Style = tbsTextButton
    end
    object AndOrBtn: TToolButton
      Left = 234
      Top = 0
      Action = AndOrAction
      Style = tbsTextButton
    end
  end
  object ActionList1: TActionList
    Left = 40
    Top = 40
    object EditItemAction: TAction
      Caption = #22793#26356
      OnExecute = EditItemActionExecute
      OnUpdate = EditItemActionUpdate
    end
    object AddItemAction: TAction
      Caption = #36861#21152
      OnExecute = AddItemActionExecute
      OnUpdate = AddItemActionUpdate
    end
    object DelItemAction: TAction
      Caption = #21066#38500
      OnExecute = DelItemActionExecute
      OnUpdate = DelItemActionUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
      OnUpdate = PropertyActionUpdate
    end
    object AppInfoAction: TAction
      Caption = #12450#12503#12522#12465#12540#12471#12519#12531#24773#22577'(&A)'
      OnExecute = AppInfoActionExecute
      OnUpdate = AppInfoActionUpdate
    end
    object SaveAsWorkAction: TAction
      Caption = #19968#35239#12434#12527#12540#12463#12522#12473#12488#12392#12375#12390#20445#23384'(&S)...'
      OnExecute = SaveAsWorkActionExecute
      OnUpdate = SaveAsWorkActionUpdate
    end
    object CopyPathAction: TAction
      Caption = #12497#12473#21517#12434#12467#12500#12540'(&C)'
      OnExecute = CopyPathActionExecute
      OnUpdate = CopyPathActionUpdate
    end
    object UseEnvVarAction: TAction
      Caption = #29872#22659#22793#25968#12434#29992#12356#12390#12497#12473#21517#12434#34920#31034'(&E)'
      OnExecute = UseEnvVarActionExecute
    end
    object OpenByExpAction: TAction
      Caption = #12456#12463#12473#12503#12525#12540#12521#12391#38283#12367'(&X)'
      OnExecute = OpenByExpActionExecute
      OnUpdate = OpenByExpActionUpdate
    end
    object MigemoAction: TAction
      Caption = '&Migemo'
      OnExecute = ToggleActionExecute
    end
    object AndOrAction: TAction
      Caption = '&ADN/OR'
      OnExecute = ToggleActionExecute
    end
    object AddNyanFiAction: TAction
      Caption = 'NyanFi '#38306#36899#12398#12487#12451#12524#12463#12488#12522#12434#34920#31034'(&N)'
      OnExecute = AddNyanFiActionExecute
    end
    object ShowIconAction: TAction
      Caption = #23455#34892#12501#12449#12452#12523#12398#12450#12452#12467#12531#12434#34920#31034'(&I)'
      OnExecute = ToggleActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 128
    Top = 41
    object OpenByExpItem: TMenuItem
      Action = OpenByExpAction
    end
    object CopyPathItem: TMenuItem
      Action = CopyPathAction
    end
    object SaveAsWorkItem: TMenuItem
      Action = SaveAsWorkAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
    object AppInfoItem: TMenuItem
      Action = AppInfoAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object UseEnvVarItem: TMenuItem
      Action = UseEnvVarAction
    end
    object AddNyanFiItem: TMenuItem
      Action = AddNyanFiAction
    end
    object ShowIconItem: TMenuItem
      Action = ShowIconAction
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
end
