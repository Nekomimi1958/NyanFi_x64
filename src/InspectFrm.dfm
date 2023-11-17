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
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 368
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 391
    Top = 0
    Width = 1
    Height = 368
    Align = alRight
    Pen.Color = clSilver
  end
  object BorderShape3: TShape
    Left = 0
    Top = 368
    Width = 392
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
  end
  object ClientPanel: TPanel
    Left = 1
    Top = 0
    Width = 390
    Height = 368
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object GridSplitter: TSplitter
      Left = 0
      Top = 217
      Width = 390
      Height = 4
      Cursor = crVSplit
      Align = alTop
    end
    object InsStatusBar: TStatusBar
      Left = 0
      Top = 0
      Width = 390
      Height = 19
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clBtnText
      Font.Height = -13
      Font.Name = 'Meiryo UI'
      Font.Pitch = fpVariable
      Font.Style = []
      Panels = <
        item
          Style = psOwnerDraw
          Width = 200
        end>
      PopupMenu = PopupMenu1
      SizeGrip = False
      UseSystemFont = False
      StyleElements = [seFont, seBorder]
      OnDrawPanel = InsStatusBarDrawPanel
    end
    object InspectHeader: THeaderControl
      Left = 0
      Top = 19
      Width = 390
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
    object GridPanel: TPanel
      Left = 0
      Top = 39
      Width = 390
      Height = 178
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 2
      object InspectPanel: TPanel
        Left = 0
        Top = 0
        Width = 390
        Height = 178
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          390
          178)
        object InspectGrid: TStringGrid
          Left = 0
          Top = 0
          Width = 390
          Height = 172
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
        object OptToolBar: TToolBar
          Left = 0
          Top = 157
          Width = 390
          Height = 21
          Align = alBottom
          AutoSize = True
          ButtonHeight = 21
          ButtonWidth = 68
          DrawingStyle = dsGradient
          List = True
          AllowTextButtons = True
          TabOrder = 1
          object UsButton: TToolButton
            Left = 0
            Top = 0
            Action = UnsignedAction
            Style = tbsTextButton
          end
          object BeButton: TToolButton
            Left = 61
            Top = 0
            Action = BigEndianAction
            Style = tbsTextButton
          end
        end
      end
    end
    object CodePageHeader: THeaderControl
      Left = 0
      Top = 221
      Width = 390
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
      Top = 241
      Width = 390
      Height = 127
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 4
      DesignSize = (
        390
        127)
      object CodePageGrid: TStringGrid
        Tag = 1
        Left = 0
        Top = 0
        Width = 390
        Height = 127
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
  end
  object PopupMenu1: TPopupMenu
    Left = 40
    Top = 88
    object CopyItem: TMenuItem
      Caption = #12467#12500#12540'(&C)'
      OnClick = CopyItemClick
    end
  end
  object ActionList1: TActionList
    Left = 145
    Top = 87
    object UnsignedAction: TAction
      Caption = 'Unsigned'
      OnExecute = UnsignedActionExecute
    end
    object BigEndianAction: TAction
      Caption = 'Big Endian'
      OnExecute = BigEndianActionExecute
    end
  end
end
