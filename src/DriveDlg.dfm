object SelDriveDlg: TSelDriveDlg
  Left = 0
  Top = 0
  HelpContext = 64
  BorderIcons = [biSystemMenu]
  Caption = #12489#12521#12452#12502#19968#35239
  ClientHeight = 341
  ClientWidth = 644
  Color = clBtnFace
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
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object HiddenCanBtn: TButton
    Left = 0
    Top = 309
    Width = 644
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 1
    TabStop = False
    ExplicitTop = 308
    ExplicitWidth = 640
  end
  object OptPanel: TPanel
    Left = 0
    Top = 309
    Width = 644
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 308
    ExplicitWidth = 640
    DesignSize = (
      644
      32)
    object HideOptBtn: TSpeedButton
      Left = 622
      Top = 16
      Width = 18
      Height = 16
      Anchors = [akTop, akRight]
      Flat = True
      OnClick = ChgOptBtnClick
      ExplicitLeft = 626
    end
    object OnlyAccCheckBox: TCheckBox
      Left = 186
      Top = 8
      Width = 135
      Height = 17
      Caption = #12450#12463#12475#12473#21487#33021#12398#12415#34920#31034
      TabOrder = 3
      OnClick = OptCheckBoxClick
    end
    object DrivePropBtn: TButton
      Left = 10
      Top = 4
      Width = 64
      Height = 25
      Action = PropertyAction
      Caption = #12503#12525#12497#12486#12451
      TabOrder = 0
    end
    object ShowIconCheckBox: TCheckBox
      Left = 354
      Top = 8
      Width = 107
      Height = 17
      Caption = #12450#12452#12467#12531#12434#34920#31034
      TabOrder = 4
      OnClick = OptCheckBoxClick
    end
    object LargeIconCheckBox: TCheckBox
      Left = 467
      Top = 8
      Width = 72
      Height = 17
      Caption = #12521#12540#12472
      TabOrder = 5
      OnClick = OptCheckBoxClick
    end
    object EjectDrvBtn: TButton
      Left = 107
      Top = 4
      Width = 64
      Height = 25
      Action = EjectDriveAction
      Caption = #21462#12426#22806#12375
      TabOrder = 2
    end
    object ToRootCheckBox: TCheckBox
      Left = 541
      Top = 8
      Width = 110
      Height = 17
      Caption = #12523#12540#12488#12395#31227#21205
      TabOrder = 6
      OnClick = ToRootCheckBoxClick
    end
    object EjectTrayBtn: TButton
      Left = 80
      Top = 4
      Width = 25
      Height = 25
      Action = EjectTrayAction
      Caption = #9650
      TabOrder = 1
    end
  end
  object DriveHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 644
    Height = 20
    Sections = <
      item
        Alignment = taCenter
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        Alignment = taCenter
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        Alignment = taCenter
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        Alignment = taCenter
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 61
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 50
      end>
    Style = hsFlat
    OnDrawSection = DriveHeaderDrawSection
    OnSectionResize = DriveHeaderSectionResize
    OnResize = DriveHeaderResize
    ExplicitWidth = 640
  end
  object GridPanel: TPanel
    Left = 0
    Top = 20
    Width = 644
    Height = 289
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 3
    ExplicitWidth = 640
    ExplicitHeight = 288
    DesignSize = (
      644
      289)
    object DriveGrid: TStringGrid
      Left = 0
      Top = 0
      Width = 640
      Height = 287
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clAppWorkSpace
      ColCount = 7
      DefaultColWidth = 60
      DefaultRowHeight = 22
      DefaultDrawing = False
      FixedCols = 0
      FixedRows = 0
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      Options = [goFixedVertLine, goColSizing, goRowSelect, goThumbTracking]
      ParentFont = False
      TabOrder = 0
      StyleElements = [seBorder]
      OnDblClick = DriveGridDblClick
      OnDrawCell = DriveGridDrawCell
      OnKeyDown = DriveGridKeyDown
      OnMouseUp = DriveGridMouseUp
      ExplicitWidth = 636
      ExplicitHeight = 286
      ColWidths = (
        60
        60
        60
        60
        60
        60
        60)
      RowHeights = (
        22
        22
        22
        22
        22)
    end
    object BlankPanel: TPanel
      Left = 626
      Top = 273
      Width = 14
      Height = 14
      Anchors = [akRight, akBottom]
      AutoSize = True
      BevelOuter = bvNone
      Color = clAppWorkSpace
      ParentBackground = False
      TabOrder = 1
      Visible = False
      StyleElements = [seBorder]
      ExplicitLeft = 622
      ExplicitTop = 272
      object ShowOptBtn: TSpeedButton
        Left = 0
        Top = 0
        Width = 14
        Height = 14
        Flat = True
        OnClick = ChgOptBtnClick
      end
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 72
    object EjectDriveAction: TAction
      Caption = #12489#12521#12452#12502#12398#21462#12426#22806#12375'(&X)'
      Hint = #12489#12521#12452#12502#12398#12487#12496#12452#12473#12434#21462#12426#22806#12375#12414#12377
      OnExecute = EjectDriveActionExecute
      OnUpdate = EjectDriveActionUpdate
    end
    object EjectTrayAction: TAction
      Caption = #12489#12521#12452#12502#12398#12488#12524#12452#12434#38283#12367'(&J)'
      Hint = 'CD/DVD'#12489#12521#12452#12502#12398#12488#12524#12452#12434#38283#12365#12414#12377
      OnExecute = EjectTrayActionExecute
      OnUpdate = EjectTrayActionUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 48
    Top = 144
    object EjectDrvItem: TMenuItem
      Action = EjectDriveAction
    end
    object EjectTryItem: TMenuItem
      Action = EjectTrayAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object NetConItem: TMenuItem
      Action = NyanFiForm.NetConnectAction
    end
    object NetDisItem: TMenuItem
      Action = NyanFiForm.NetDisconnectAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object OpenExpItem: TMenuItem
      Caption = #12456#12463#12473#12503#12525#12540#12521#12391#38283#12367'(&O)'
      OnClick = OpenExpItemClick
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
  end
end
