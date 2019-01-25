object CsvRecForm: TCsvRecForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  ClientHeight = 283
  ClientWidth = 310
  Color = clBtnFace
  Constraints.MinWidth = 320
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
  object OptPanel: TPanel
    Left = 0
    Top = 251
    Width = 310
    Height = 32
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      310
      32)
    object TopIsHeaderCheckBox: TCheckBox
      Left = 12
      Top = 8
      Width = 139
      Height = 17
      Caption = #20808#38957#34892#12364#38917#30446#21517
      TabOrder = 0
      OnClick = TopIsHeaderCheckBoxClick
    end
    object CalcBtn: TButton
      Left = 153
      Top = 4
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #38598#35336
      TabOrder = 1
      OnClick = CalcBtnClick
    end
    object GraphBtn: TButton
      Left = 231
      Top = 4
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #12464#12521#12501
      TabOrder = 2
      OnClick = GraphBtnClick
    end
  end
  object RecordHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 310
    Height = 20
    Sections = <
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 201
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 201
      end
      item
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 50
      end>
    Style = hsFlat
    OnDrawSection = RecordHeaderDrawSection
    OnSectionResize = RecordHeaderSectionResize
    OnResize = RecordHeaderResize
  end
  object GridPanel: TPanel
    Left = 0
    Top = 20
    Width = 310
    Height = 231
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      310
      231)
    object RecordGrid: TStringGrid
      Left = 0
      Top = 0
      Width = 310
      Height = 233
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clAppWorkSpace
      ColCount = 2
      DefaultColWidth = 100
      DefaultDrawing = False
      FixedCols = 0
      FixedRows = 0
      Options = [goFixedVertLine, goFixedHorzLine, goRangeSelect, goColSizing, goThumbTracking]
      PopupMenu = PopupMenu1
      TabOrder = 0
      StyleElements = []
      OnClick = RecordGridClick
      OnDrawCell = RecordGridDrawCell
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
    Left = 56
    Top = 136
    object CopyItem: TMenuItem
      Caption = #12467#12500#12540'(&C)'
      OnClick = CopyItemClick
    end
  end
end
