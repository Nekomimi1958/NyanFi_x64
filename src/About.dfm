object AboutBox: TAboutBox
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = #12496#12540#12472#12519#12531#24773#22577
  ClientHeight = 222
  ClientWidth = 354
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poMainFormCenter
  OnCreate = FormCreate
  OnShow = FormShow
  TextHeight = 15
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 338
    Height = 161
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 0
    object ProductName: TLabel
      Left = 79
      Top = 20
      Width = 180
      Height = 37
      Alignment = taCenter
      AutoSize = False
      Caption = 'NyanFi'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -29
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      IsControl = True
    end
    object Version: TLabel
      Left = 79
      Top = 62
      Width = 180
      Height = 26
      Alignment = taCenter
      AutoSize = False
      Caption = 'Version'
      IsControl = True
    end
    object Copyright: TLabel
      Left = 9
      Top = 96
      Width = 320
      Height = 26
      Alignment = taCenter
      AutoSize = False
      Caption = 'Copyright'
      IsControl = True
    end
    object SupportURL: TLabel
      Left = 9
      Top = 126
      Width = 320
      Height = 26
      Cursor = crHandPoint
      Alignment = taCenter
      AutoSize = False
      Caption = 'http://'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clNavy
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      ParentFont = False
      OnClick = SupportURLClick
      IsControl = True
    end
    object VirtualImage1: TVirtualImage
      Left = 16
      Top = 16
      Width = 64
      Height = 64
      ImageCollection = ImageCollection1
      ImageWidth = 0
      ImageHeight = 0
      ImageIndex = -1
    end
  end
  object OKButton: TButton
    Left = 139
    Top = 188
    Width = 75
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object CancelBtn: TButton
    Left = 0
    Top = 222
    Width = 354
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 3
  end
  object ListNyanBtn: TButton
    Left = 240
    Top = 188
    Width = 106
    Height = 26
    Caption = #35443#32048#19968#35239'(&L)'
    TabOrder = 2
    OnClick = ListNyanBtnClick
  end
  object ImageCollection1: TImageCollection
    Images = <>
    Left = 56
    Top = 96
  end
end
