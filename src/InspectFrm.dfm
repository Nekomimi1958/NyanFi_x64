object InspectForm: TInspectForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #12452#12531#12473#12506#12463#12479
  ClientHeight = 369
  ClientWidth = 392
  Color = clBtnFace
  DoubleBuffered = True
  ParentFont = True
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GridSplitter: TSplitter
    Left = 0
    Top = 219
    Width = 392
    Height = 4
    Cursor = crVSplit
    Align = alTop
  end
  object InsStatusBar: TStatusBar
    Left = 0
    Top = 0
    Width = 392
    Height = 19
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -13
    Font.Name = 'Meiryo UI'
    Font.Style = []
    Panels = <
      item
        Width = 200
      end>
    PopupMenu = PopupMenu1
    SizeGrip = False
    UseSystemFont = False
  end
  object GridPanel: TPanel
    Left = 0
    Top = 19
    Width = 392
    Height = 200
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object OptPanel: TPanel
      Left = 0
      Top = 169
      Width = 392
      Height = 31
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      object UnsigCheckBox: TCheckBox
        Left = 10
        Top = 8
        Width = 90
        Height = 17
        Caption = 'unsigned'
        TabOrder = 0
        OnClick = OptCheckBoxClick
      end
      object BigCheckBox: TCheckBox
        Left = 104
        Top = 8
        Width = 90
        Height = 17
        Caption = 'Big Endian'
        TabOrder = 1
        OnClick = OptCheckBoxClick
      end
    end
    object InspectHeader: THeaderControl
      Left = 0
      Top = 0
      Width = 392
      Height = 20
      Sections = <
        item
          AllowClick = False
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 101
        end
        item
          AllowClick = False
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 101
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 50
        end>
      Style = hsFlat
      OnDrawSection = InspectHeaderDrawSection
      OnSectionResize = InspectHeaderSectionResize
      OnResize = InspectHeaderResize
    end
    object InspectPanel: TPanel
      Left = 0
      Top = 20
      Width = 392
      Height = 149
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 2
      DesignSize = (
        392
        149)
      object InspectGrid: TStringGrid
        Left = 0
        Top = 0
        Width = 392
        Height = 149
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Color = clAppWorkSpace
        ColCount = 2
        DefaultColWidth = 100
        DefaultDrawing = False
        FixedCols = 0
        FixedRows = 0
        Options = [goFixedVertLine, goVertLine, goColSizing, goRowSelect]
        PopupMenu = PopupMenu1
        TabOrder = 0
        StyleElements = [seBorder]
        OnDrawCell = InspectGridDrawCell
        ColWidths = (
          100
          100)
        RowHeights = (
          24
          24
          24
          24
          24)
      end
    end
  end
  object CodePageHeader: THeaderControl
    Left = 0
    Top = 223
    Width = 392
    Height = 20
    Sections = <
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 101
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 101
      end
      item
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 50
      end>
    Style = hsFlat
    OnDrawSection = CodePageHeaderDrawSection
    OnSectionResize = CodePageHeaderSectionResize
    OnResize = CodePageHeaderResize
  end
  object CodePanel: TPanel
    Left = 0
    Top = 243
    Width = 392
    Height = 126
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      392
      126)
    object CodePageGrid: TStringGrid
      Tag = 1
      Left = 0
      Top = 0
      Width = 392
      Height = 126
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clAppWorkSpace
      ColCount = 2
      DefaultColWidth = 100
      DefaultDrawing = False
      FixedCols = 0
      FixedRows = 0
      Options = [goFixedVertLine, goVertLine, goColSizing, goRowSelect, goThumbTracking]
      PopupMenu = PopupMenu1
      TabOrder = 0
      StyleElements = [seBorder]
      OnDrawCell = InspectGridDrawCell
      ColWidths = (
        100
        100)
      RowHeights = (
        24
        24
        24
        24
        24)
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 40
    Top = 88
    object CopyItem: TMenuItem
      Caption = #12467#12500#12540'(&C)'
      OnClick = CopyItemClick
    end
  end
end
