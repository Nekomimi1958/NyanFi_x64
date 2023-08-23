object HistForm: THistForm
  Left = 0
  Top = 0
  HelpContext = 7
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = #12498#12473#12488#12464#12521#12512
  ClientHeight = 131
  ClientWidth = 257
  Color = clBtnFace
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poDesigned
  OnHide = FormHide
  OnShow = FormShow
  TextHeight = 15
  object HistPanel: TPanel
    Left = 0
    Top = 0
    Width = 257
    Height = 131
    Align = alClient
    BevelOuter = bvNone
    Color = clBlack
    DoubleBuffered = False
    ParentBackground = False
    ParentDoubleBuffered = False
    TabOrder = 0
    ExplicitWidth = 249
    ExplicitHeight = 119
    object HistImage: TImage
      Left = 0
      Top = 0
      Width = 257
      Height = 131
      Align = alClient
    end
  end
end
