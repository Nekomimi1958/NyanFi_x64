object PackArcDlg: TPackArcDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12450#12540#12459#12452#12502#12398#20316#25104
  ClientHeight = 395
  ClientWidth = 444
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    444
    395)
  TextHeight = 15
  object FextLabel: TLabel
    Left = 404
    Top = 11
    Width = 18
    Height = 15
    Caption = '.zip'
  end
  object OkButton: TButton
    Left = 134
    Top = 359
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 7
  end
  object ArcNameEdit: TLabeledEdit
    Left = 106
    Top = 8
    Width = 290
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 78
    EditLabel.Height = 23
    EditLabel.Caption = #12450#12540#12459#12452#12502#21517'(&A)'
    LabelPosition = lpLeft
    PopupMenu = UserModule.EditPopupMenuE
    TabOrder = 0
    Text = ''
    ExplicitWidth = 286
  end
  object CanButton: TButton
    Left = 230
    Top = 359
    Width = 80
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 8
  end
  object FormatRadioGroup: TRadioGroup
    Left = 8
    Top = 74
    Width = 424
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #12501#12457#12540#12510#12483#12488'(&F)'
    Columns = 5
    ItemIndex = 0
    Items.Strings = (
      'ZIP'
      '7z'
      'LHA'
      'CAB'
      'TAR')
    TabOrder = 3
    OnClick = FormatRadioGroupClick
    ExplicitWidth = 420
  end
  object OptionGroupBox: TGroupBox
    Left = 8
    Top = 134
    Width = 424
    Height = 143
    Anchors = [akLeft, akTop, akRight]
    Caption = #12458#12503#12471#12519#12531'(&O)'
    TabOrder = 4
    ExplicitWidth = 420
    DesignSize = (
      424
      143)
    object Label1: TLabel
      Left = 38
      Top = 21
      Width = 52
      Height = 15
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #22311#32302#24418#24335
      ExplicitLeft = 42
    end
    object PasswordEdit: TLabeledEdit
      Left = 98
      Top = 109
      Width = 315
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 66
      EditLabel.Height = 23
      EditLabel.Caption = #12497#12473#12527#12540#12489'(&P)'
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 3
      Text = ''
      StyleElements = [seBorder]
      ExplicitWidth = 311
    end
    object ParamComboBox: TComboBox
      Left = 98
      Top = 18
      Width = 315
      Height = 23
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 311
    end
    object SfxCheckBox: TCheckBox
      Left = 98
      Top = 80
      Width = 113
      Height = 17
      Caption = #33258#24049#35299#20941
      TabOrder = 2
      OnClick = SfxCheckBoxClick
    end
    object ExSwEdit: TLabeledEdit
      Left = 98
      Top = 47
      Width = 315
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 63
      EditLabel.Height = 23
      EditLabel.Caption = #36861#21152#12473#12452#12483#12481
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 1
      Text = ''
      StyleElements = [seBorder]
      ExplicitWidth = 311
    end
  end
  object PerDirCheckBox: TCheckBox
    Left = 21
    Top = 41
    Width = 200
    Height = 17
    Caption = #12487#12451#12524#12463#12488#12522#12372#12392#12395#20491#21029#20316#25104'(&I)'
    TabOrder = 1
    OnClick = PerDirCheckBoxClick
  end
  object IncDirCheckBox: TCheckBox
    Left = 225
    Top = 41
    Width = 200
    Height = 17
    Caption = #23550#35937#12487#12451#12524#12463#12488#12522#12418#21547#12417#12427'(&D)'
    TabOrder = 2
    OnClick = PerDirCheckBoxClick
  end
  object SameRadioGroup: TRadioGroup
    Left = 8
    Top = 289
    Width = 424
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #21516#21517#12450#12540#12459#12452#12502#12364#12354#12427#22580#21512#12398#20966#29702
    Columns = 3
    ItemIndex = 0
    Items.Strings = (
      #26082#23384#20869#23481#12395#36861#21152
      #21066#38500#12375#12390#26032#35215#20316#25104)
    TabOrder = 5
    OnClick = FormatRadioGroupClick
    ExplicitWidth = 420
  end
  object SureSameCheckBox: TCheckBox
    Left = 346
    Top = 309
    Width = 77
    Height = 17
    Caption = #30906#35469
    TabOrder = 6
  end
end
