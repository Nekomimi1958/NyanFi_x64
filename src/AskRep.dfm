object AskRepDlg: TAskRepDlg
  Left = 0
  Top = 0
  HelpContext = 61
  Caption = #25991#23383#21015#12398#32622#25563
  ClientHeight = 144
  ClientWidth = 528
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
  object ListPanel: TPanel
    Left = 0
    Top = 19
    Width = 528
    Height = 89
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object RepListBox: TListBox
      Left = 0
      Top = 0
      Width = 528
      Height = 89
      TabStop = False
      Style = lbOwnerDrawFixed
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Font.Charset = SHIFTJIS_CHARSET
      Font.Color = clWhite
      Font.Height = -16
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Pitch = fpFixed
      Font.Style = []
      ItemHeight = 24
      ParentFont = False
      TabOrder = 0
      StyleElements = [seBorder]
      OnDrawItem = RepListBoxDrawItem
      OnEnter = RepListBoxEnter
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 0
    Width = 528
    Height = 19
    Align = alTop
    Font.Charset = SHIFTJIS_CHARSET
    Font.Color = clBtnText
    Font.Height = -15
    Font.Name = #65325#65331' '#12468#12471#12483#12463
    Font.Pitch = fpVariable
    Font.Style = []
    Panels = <
      item
        Style = psOwnerDraw
        Width = 200
      end
      item
        Style = psOwnerDraw
        Width = 50
      end>
    UseSystemFont = False
    OnDrawPanel = StatusBar1DrawPanel
  end
  object OpPanel: TPanel
    Left = 0
    Top = 108
    Width = 528
    Height = 36
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      528
      36)
    object OKBtn: TButton
      Left = 8
      Top = 5
      Width = 90
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = #32622#25563'(&Y)'
      Default = True
      ModalResult = 6
      TabOrder = 0
    end
    object AllBtn: TButton
      Left = 114
      Top = 5
      Width = 90
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = #20840#12390#32622#25563'(&A)'
      ModalResult = 12
      TabOrder = 1
    end
    object SkipBtn: TButton
      Left = 220
      Top = 5
      Width = 90
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = #27425#26908#32034'(&N)'
      ModalResult = 7
      TabOrder = 2
    end
    object CanBtn: TButton
      Left = 430
      Top = 5
      Width = 90
      Height = 26
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = #20013#26029
      ModalResult = 3
      TabOrder = 3
    end
  end
end
