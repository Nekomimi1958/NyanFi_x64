object PackArcDlg: TPackArcDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12450#12540#12459#12452#12502#12398#20316#25104
  ClientHeight = 395
  ClientWidth = 444
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    444
    395)
  PixelsPerInch = 96
  TextHeight = 13
  object FextLabel: TLabel
    Left = 354
    Top = 11
    Width = 17
    Height = 13
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
    Width = 319
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 73
    EditLabel.Height = 13
    EditLabel.Caption = #12450#12540#12459#12452#12502#21517'(&A)'
    LabelPosition = lpLeft
    PopupMenu = UserModule.EditPopupMenuE
    TabOrder = 0
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
    Width = 428
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
  end
  object OptionGroupBox: TGroupBox
    Left = 8
    Top = 134
    Width = 428
    Height = 143
    Anchors = [akLeft, akTop, akRight]
    Caption = #12458#12503#12471#12519#12531'(&O)'
    TabOrder = 4
    DesignSize = (
      428
      143)
    object Label1: TLabel
      Left = 46
      Top = 21
      Width = 48
      Height = 13
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      Caption = #22311#32302#24418#24335
    end
    object PasswordEdit: TLabeledEdit
      Left = 98
      Top = 109
      Width = 319
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = #12497#12473#12527#12540#12489
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 3
      StyleElements = [seBorder]
    end
    object ParamComboBox: TComboBox
      Left = 98
      Top = 18
      Width = 319
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
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
      Width = 319
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 58
      EditLabel.Height = 13
      EditLabel.Caption = #36861#21152#12473#12452#12483#12481
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 1
      StyleElements = [seBorder]
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
    Width = 428
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
