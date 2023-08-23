object SplashForm: TSplashForm
  Left = 277
  Top = 257
  AlphaBlend = True
  AlphaBlendValue = 205
  BorderIcons = []
  BorderStyle = bsNone
  ClientHeight = 240
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poScreenCenter
  OnClose = FormClose
  OnDestroy = FormDestroy
  OnPaint = FormPaint
  TextHeight = 15
  object MsgLabel: TLabel
    Left = 10
    Top = 181
    Width = 300
    Height = 24
    Alignment = taCenter
    AutoSize = False
  end
  object TitleLabel: TLabel
    Left = 10
    Top = 33
    Width = 300
    Height = 42
    Alignment = taCenter
    AutoSize = False
    Caption = 'NyanFi'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -29
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
  end
  object VerLabel: TLabel
    Left = 10
    Top = 75
    Width = 300
    Height = 24
    Alignment = taCenter
    AutoSize = False
  end
end
