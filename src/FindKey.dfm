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
  OldCreateOrder = False
  Position = poMainFormCenter
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 64
    Top = 16
    Width = 143
    Height = 13
    Caption = #26908#32034#12377#12427#12461#12540#12434#25276#12375#12390#12367#12384#12373#12356#12290
  end
  object CancelBtn: TSpeedButton
    Left = 95
    Top = 88
    Width = 80
    Height = 24
    Caption = #12461#12515#12531#12475#12523
    OnClick = CancelBtnClick
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
end
