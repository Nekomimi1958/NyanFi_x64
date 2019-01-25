object CvImageDlg: TCvImageDlg
  Left = 0
  Top = 0
  HelpContext = 59
  BorderStyle = bsDialog
  Caption = #30011#20687#12501#12449#12452#12523#12398#22793#25563
  ClientHeight = 450
  ClientWidth = 394
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    394
    450)
  PixelsPerInch = 96
  TextHeight = 13
  object CanButton: TButton
    Left = 210
    Top = 417
    Width = 80
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 8
  end
  object OkButton: TButton
    Left = 105
    Top = 417
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 7
  end
  object ScaleGroupBox: TGroupBox
    Left = 8
    Top = 143
    Width = 378
    Height = 139
    Anchors = [akLeft, akTop, akRight]
    Caption = #32302#23567#12539#25313#22823
    TabOrder = 3
    object TLabel
      Left = 14
      Top = 112
      Width = 83
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = #12450#12523#12468#12522#12474#12512
    end
    object TLabel
      Left = 14
      Top = 82
      Width = 83
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = #20313#30333#33394
    end
    object ScaleModeComboBox: TComboBox
      Left = 12
      Top = 20
      Width = 352
      Height = 21
      Style = csDropDownList
      DropDownCount = 10
      TabOrder = 0
      OnChange = ScaleModeComboBoxChange
    end
    object ScalePrm1Edit: TLabeledEdit
      Left = 101
      Top = 49
      Width = 75
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 50
      EditLabel.Height = 13
      EditLabel.Caption = #12497#12521#12513#12540#12479'1'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentFont = False
      TabOrder = 1
    end
    object ScalePrm2Edit: TLabeledEdit
      Left = 271
      Top = 49
      Width = 75
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 50
      EditLabel.Height = 13
      EditLabel.Caption = #12497#12521#12513#12540#12479'2'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      LabelPosition = lpLeft
      NumbersOnly = True
      ParentFont = False
      TabOrder = 2
    end
    object ScaleOptComboBox: TComboBox
      Left = 101
      Top = 109
      Width = 263
      Height = 21
      Style = csDropDownList
      TabOrder = 5
    end
    object RefMgnColBtn: TButton
      Left = 178
      Top = 77
      Width = 36
      Height = 22
      Caption = '...'
      TabOrder = 4
      OnClick = RefMgnColBtnClick
    end
    object MgnColPanel: TPanel
      Left = 101
      Top = 77
      Width = 75
      Height = 22
      BevelOuter = bvLowered
      TabOrder = 3
      StyleElements = [seBorder]
    end
  end
  object NotUsePrvCheckBox: TCheckBox
    Left = 24
    Top = 293
    Width = 348
    Height = 17
    Caption = #12503#12524#12499#12517#12540#12487#12540#12479#12434#21033#29992#12375#12394#12356'(WIC)'
    TabOrder = 4
  end
  object GrayScaleCheckBox: TCheckBox
    Left = 24
    Top = 109
    Width = 196
    Height = 17
    Caption = #12464#12524#12540#12473#12465#12540#12523#21270'(&S)'
    TabOrder = 2
    OnClick = GrayScaleCheckBoxClick
  end
  object NameGroupBox: TGroupBox
    Left = 8
    Top = 324
    Width = 378
    Height = 50
    Anchors = [akLeft, akTop, akRight]
    Caption = #12501#12449#12452#12523#21517#12398#22793#26356
    TabOrder = 5
    object ChgNameComboBox: TComboBox
      Left = 12
      Top = 20
      Width = 257
      Height = 21
      Style = csDropDownList
      TabOrder = 0
    end
    object ChgNameEdit: TEdit
      Left = 275
      Top = 20
      Width = 89
      Height = 21
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      ParentFont = False
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 1
    end
  end
  object SubOptGroupBox: TGroupBox
    Left = 8
    Top = 51
    Width = 378
    Height = 47
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    DesignSize = (
      378
      47)
    object ImgQLabel: TLabel
      Left = 16
      Top = 16
      Width = 76
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = #21697#36074' 100'
    end
    object CmpModeLabel: TLabel
      Left = 16
      Top = 16
      Width = 81
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = #22311#32302#12514#12540#12489
    end
    object YCrCbLabel: TLabel
      Left = 227
      Top = 16
      Width = 41
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = 'YCrCb'
    end
    object ImgQTrackBar: TTrackBar
      Left = 94
      Top = 12
      Width = 120
      Height = 32
      Anchors = [akLeft, akTop, akRight]
      Max = 100
      Frequency = 10
      Position = 50
      TabOrder = 0
      OnChange = ImgQTrackBarChange
    end
    object CmpModeComboBox: TComboBox
      Left = 101
      Top = 13
      Width = 133
      Height = 21
      Style = csDropDownList
      TabOrder = 1
    end
    object YCrCbComboBox: TComboBox
      Left = 274
      Top = 13
      Width = 90
      Height = 21
      Style = csDropDownList
      TabOrder = 2
    end
  end
  object CvFmtRadioGroup: TRadioGroup
    Left = 8
    Top = 8
    Width = 378
    Height = 41
    Anchors = [akLeft, akTop, akRight]
    Caption = #22793#25563#24418#24335
    Columns = 6
    Items.Strings = (
      '&BMP'
      '&JPG'
      '&PNG'
      '&GIF'
      '&TIF'
      '&HDP')
    TabOrder = 0
    OnClick = CvFmtRadioGroupClick
  end
  object KeepTimeCheckBox: TCheckBox
    Left = 24
    Top = 386
    Width = 348
    Height = 17
    Caption = #12479#12452#12512#12473#12479#12531#12503#12434#32173#25345#12377#12427
    TabOrder = 6
  end
end
