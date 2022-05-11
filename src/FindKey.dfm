object FindKeyDlg: TFindKeyDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #12461#12540#26908#32034
  ClientHeight = 122
  ClientWidth = 270
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  Position = poOwnerFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object Label1: TLabel
    Left = 64
    Top = 16
    Width = 157
    Height = 15
    Caption = #26908#32034#12377#12427#12461#12540#12434#25276#12375#12390#12367#12384#12373#12356#12290
  end
  object KeyLabel: TLabel
    Left = 8
    Top = 41
    Width = 254
    Height = 20
    Alignment = taCenter
    AutoSize = False
  end
  object DummyBtn: TButton
    Left = 0
    Top = 122
    Width = 270
    Height = 0
    Align = alBottom
    Default = True
    TabOrder = 0
    OnClick = DummyBtnClick
  end
  object CanOutPanel: TPanel
    Left = 94
    Top = 80
    Width = 82
    Height = 26
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 1
    ParentBackground = False
    TabOrder = 1
    StyleElements = [seFont, seBorder]
    object CanInPanel: TPanel
      Left = 1
      Top = 1
      Width = 80
      Height = 24
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 0
      StyleElements = [seFont, seBorder]
      object CancelBtn: TSpeedButton
        Left = 0
        Top = 0
        Width = 80
        Height = 24
        OnClick = CancelBtnClick
      end
    end
  end
end
