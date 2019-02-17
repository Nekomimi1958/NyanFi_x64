object DriveGraph: TDriveGraph
  Left = 0
  Top = 0
  HelpContext = 86
  BorderIcons = [biSystemMenu]
  Caption = #12489#12521#12452#12502#20351#29992#29575#25512#31227
  ClientHeight = 289
  ClientWidth = 392
  Color = clBtnFace
  Constraints.MinWidth = 360
  DoubleBuffered = True
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnMouseWheel = FormMouseWheel
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 392
    Height = 25
    Align = alTop
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 2
    TabOrder = 0
    object DriveComboBox: TComboBox
      Left = 2
      Top = 2
      Width = 118
      Height = 21
      Align = alClient
      Style = csDropDownList
      TabOrder = 0
      OnChange = DriveComboBoxChange
    end
    object SizeComboBox: TComboBox
      Left = 180
      Top = 2
      Width = 50
      Height = 21
      Align = alRight
      Style = csDropDownList
      TabOrder = 1
      OnChange = SizeComboBoxChange
    end
    object Panel3: TPanel
      Left = 230
      Top = 2
      Width = 160
      Height = 21
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 2
      object OldOdrCheckBox: TCheckBox
        Left = 6
        Top = 2
        Width = 72
        Height = 17
        Caption = #21476#12356#38918
        TabOrder = 0
        OnClick = OptionChanged
      end
      object MinMaxCheckBox: TCheckBox
        Left = 80
        Top = 2
        Width = 80
        Height = 17
        Caption = 'Min/Max'
        TabOrder = 1
        OnClick = OptionChanged
      end
    end
    object Panel2: TPanel
      Left = 120
      Top = 2
      Width = 60
      Height = 21
      Align = alRight
      Alignment = taRightJustify
      BevelOuter = bvNone
      Caption = #26834#24133' '
      TabOrder = 3
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 270
    Width = 392
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 1
    TabStop = False
  end
  object GraphScrollBox: TScrollBox
    Left = 0
    Top = 25
    Width = 392
    Height = 245
    HorzScrollBar.Visible = False
    VertScrollBar.Tracking = True
    Align = alClient
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    TabOrder = 2
    object PaintBox1: TPaintBox
      Left = 0
      Top = 0
      Width = 392
      Height = 245
      Align = alClient
      OnMouseDown = PaintBox1MouseDown
      OnPaint = PaintBox1Paint
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 270
    Width = 392
    Height = 19
    Panels = <>
    SimplePanel = True
  end
end
