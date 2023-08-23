object TabSetDlg: TTabSetDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12479#12502#12398#35373#23450
  ClientHeight = 343
  ClientWidth = 506
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  Position = poMainFormCenter
  ShowHint = True
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    506
    343)
  TextHeight = 15
  object CanButton: TButton
    Left = 264
    Top = 307
    Width = 80
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 4
  end
  object OkButton: TButton
    Left = 162
    Top = 307
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 3
    OnClick = OkButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 6
    Width = 486
    Height = 81
    Anchors = [akLeft, akTop, akRight]
    Caption = #34920#31034
    TabOrder = 0
    ExplicitWidth = 482
    DesignSize = (
      486
      81)
    object IcoImage: TImage
      Left = 12
      Top = 47
      Width = 22
      Height = 22
    end
    object RefIconBtn: TButton
      Tag = 100
      Left = 443
      Top = 46
      Width = 36
      Height = 23
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 2
      OnClick = RefIconBtnClick
      ExplicitLeft = 439
    end
    object IconEdit: TLabeledEdit
      Left = 82
      Top = 47
      Width = 359
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 38
      EditLabel.Height = 23
      EditLabel.Caption = #12450#12452#12467#12531
      LabelPosition = lpLeft
      TabOrder = 1
      Text = ''
      OnChange = IconEditChange
      ExplicitWidth = 355
    end
    object CaptionEdit: TLabeledEdit
      Left = 82
      Top = 18
      Width = 359
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 58
      EditLabel.Height = 23
      EditLabel.Caption = #12461#12515#12503#12471#12519#12531
      LabelPosition = lpLeft
      TabOrder = 0
      Text = ''
      ExplicitWidth = 355
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 95
    Width = 486
    Height = 110
    Anchors = [akLeft, akTop, akRight]
    Caption = #12507#12540#12512
    TabOrder = 1
    ExplicitWidth = 482
    DesignSize = (
      486
      110)
    object RefHome1Btn: TButton
      Left = 443
      Top = 18
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 1
      OnClick = RefHomeBtnClick
      ExplicitLeft = 439
    end
    object HomeDir1Edit: TEdit
      Left = 12
      Top = 18
      Width = 429
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 425
    end
    object HomeDir2Edit: TEdit
      Tag = 1
      Left = 12
      Top = 47
      Width = 429
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
      ExplicitWidth = 425
    end
    object RefHome2Btn: TButton
      Tag = 1
      Left = 443
      Top = 47
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 3
      OnClick = RefHomeBtnClick
      ExplicitLeft = 439
    end
    object SetCurDirBtn: TButton
      Left = 229
      Top = 77
      Width = 250
      Height = 25
      Anchors = [akTop, akRight]
      Caption = #29694#22312#12398#12487#12451#12524#12463#12488#12522'/'#12527#12540#12463#12522#12473#12488#12434#35373#23450
      TabOrder = 4
      OnClick = SetCurDirBtnClick
      ExplicitLeft = 225
    end
  end
  object GroupBox3: TGroupBox
    Left = 8
    Top = 213
    Width = 490
    Height = 81
    Caption = #12527#12540#12463#12522#12473#12488
    TabOrder = 2
    DesignSize = (
      490
      81)
    object Work0RadioBtn: TRadioButton
      Left = 12
      Top = 23
      Width = 80
      Height = 17
      Caption = #12381#12398#12414#12414
      TabOrder = 0
      OnClick = WorkRadioBtnClick
    end
    object Work1RadioBtn: TRadioButton
      Left = 96
      Top = 23
      Width = 80
      Height = 17
      Caption = #12507#12540#12512
      TabOrder = 1
      OnClick = WorkRadioBtnClick
    end
    object Work2RadioBtn: TRadioButton
      Left = 180
      Top = 23
      Width = 80
      Height = 17
      Caption = #25351#23450
      TabOrder = 2
      OnClick = WorkRadioBtnClick
    end
    object WorkListEdit: TEdit
      Left = 12
      Top = 48
      Width = 433
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
    end
    object RefWorkBtn: TButton
      Left = 447
      Top = 48
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 4
      OnClick = RefWorkBtnClick
    end
  end
end
