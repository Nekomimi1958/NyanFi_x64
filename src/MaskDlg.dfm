object PathMaskDlg: TPathMaskDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #12497#12473#12510#12473#12463
  ClientHeight = 341
  ClientWidth = 564
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
    Top = 271
    Width = 564
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 3
  end
  object OptPanel: TPanel
    Left = 0
    Top = 271
    Width = 564
    Height = 70
    Align = alBottom
    BevelOuter = bvNone
    ParentShowHint = False
    ShowHint = True
    TabOrder = 1
    DesignSize = (
      564
      70)
    object HideOptBtn: TSpeedButton
      Left = 546
      Top = 54
      Width = 18
      Height = 16
      Anchors = [akTop, akRight]
      Flat = True
      OnClick = ChgOptBtnClick
    end
    object UpButton: TButton
      Left = 232
      Top = 39
      Width = 30
      Height = 26
      Action = UserModule.UpListItemAction
      TabOrder = 6
    end
    object DowButton: TButton
      Left = 264
      Top = 39
      Width = 30
      Height = 26
      Action = UserModule.DownListItemAction
      TabOrder = 7
    end
    object EditButton: TButton
      Left = 16
      Top = 39
      Width = 60
      Height = 26
      Action = EditItemAction
      TabOrder = 3
    end
    object AddButton: TButton
      Left = 83
      Top = 39
      Width = 60
      Height = 26
      Action = AddItemAction
      TabOrder = 4
    end
    object DelButton: TButton
      Left = 150
      Top = 39
      Width = 60
      Height = 26
      Action = UserModule.DelListItemAction
      TabOrder = 5
    end
    object DescEdit: TEdit
      Left = 59
      Top = 8
      Width = 163
      Height = 21
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 1
    end
    object MaskEdit: TEdit
      Left = 224
      Top = 8
      Width = 332
      Height = 21
      Hint = '; '#12391#21306#20999#12387#12390#35079#25968#25351#23450#21487#33021#12289'!'#12399#21542#23450
      Anchors = [akLeft, akTop, akRight]
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 2
    end
    object KeyEdit: TEdit
      Left = 16
      Top = 8
      Width = 41
      Height = 21
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
    end
  end
  object PathMaskHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 564
    Height = 17
    Sections = <
      item
        Alignment = taCenter
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end>
    Style = hsFlat
    OnDrawSection = PathMaskHeaderDrawSection
    OnSectionResize = PathMaskHeaderSectionResize
    DoubleBuffered = False
    ParentDoubleBuffered = False
    StyleElements = [seFont, seBorder]
    OnResize = PathMaskHeaderResize
  end
  object ListPanel: TPanel
    Left = 0
    Top = 17
    Width = 564
    Height = 254
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      564
      254)
    object PathMaskListBox: TListBox
      Left = 0
      Top = 0
      Width = 564
      Height = 254
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
      ParentDoubleBuffered = False
      ParentFont = False
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = PathMaskListBoxClick
      OnDblClick = PathMaskListBoxDblClick
      OnDrawItem = PathMaskListBoxDrawItem
      OnKeyDown = PathMaskListBoxKeyDown
    end
    object BlankPanel: TPanel
      Left = 546
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
  end
end
