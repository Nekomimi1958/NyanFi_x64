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
    Top = 271
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
      TabOrder = 7
    end
    object DowButton: TButton
      Left = 256
      Top = 37
      Width = 30
      Height = 26
      Action = UserModule.DownListItemAction
      TabOrder = 8
    end
    object EditButton: TButton
      Left = 79
      Top = 37
      Width = 60
      Height = 26
      Action = EditItemAction
      TabOrder = 5
    end
    object AddButton: TButton
      Left = 16
      Top = 37
      Width = 60
      Height = 26
      Action = AddItemAction
      TabOrder = 4
    end
    object DelButton: TButton
      Left = 142
      Top = 37
      Width = 60
      Height = 26
      Action = UserModule.DelListItemAction
      TabOrder = 6
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
      Width = 327
      Height = 21
      Hint = #29872#22659#22793#25968#12418#20351#29992#21487#33021
      Anchors = [akLeft, akTop, akRight]
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 2
    end
    object RefDirButton: TButton
      Left = 553
      Top = 8
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 3
      OnClick = RefDirButtonClick
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
      TabOrder = 9
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 271
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
    Height = 251
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      592
      251)
    object RegDirListBox: TListBox
      Left = 0
      Top = 0
      Width = 592
      Height = 251
      Style = lbOwnerDrawFixed
      AutoComplete = False
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      DragMode = dmAutomatic
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
      Top = 235
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
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
      OnUpdate = PropertyActionUpdate
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
      OnUpdate = UseEnvVarActionUpdate
    end
    object OpenByExpAction: TAction
      Caption = #12456#12463#12473#12503#12525#12540#12521#12391#38283#12367'(&X)'
      OnExecute = OpenByExpActionExecute
      OnUpdate = OpenByExpActionUpdate
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 128
    Top = 41
    object C1: TMenuItem
      Action = CopyPathAction
    end
    object UseEnvVarItem: TMenuItem
      Action = UseEnvVarAction
    end
    object S1: TMenuItem
      Action = SaveAsWorkAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object OpenByExpItem: TMenuItem
      Action = OpenByExpAction
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
end
