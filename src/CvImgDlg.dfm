object CvImageDlg: TCvImageDlg
  Left = 0
  Top = 0
  HelpContext = 59
  BorderStyle = bsDialog
  Caption = #30011#20687#12501#12449#12452#12523#12398#22793#25563
  ClientHeight = 552
  ClientWidth = 394
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
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 394
    Height = 289
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 390
    DesignSize = (
      394
      289)
    object CvFmtRadioGroup: TRadioGroup
      Left = 8
      Top = 8
      Width = 374
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
      ExplicitWidth = 370
    end
    object SubOptGroupBox: TGroupBox
      Left = 8
      Top = 52
      Width = 374
      Height = 47
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      ExplicitWidth = 370
      DesignSize = (
        374
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
        Width = 116
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        Max = 100
        Frequency = 10
        Position = 50
        TabOrder = 0
        OnChange = ImgQTrackBarChange
        ExplicitWidth = 112
      end
      object CmpModeComboBox: TComboBox
        Left = 101
        Top = 13
        Width = 133
        Height = 23
        Style = csDropDownList
        TabOrder = 1
      end
      object YCrCbComboBox: TComboBox
        Left = 274
        Top = 13
        Width = 90
        Height = 23
        Style = csDropDownList
        TabOrder = 2
      end
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
    object ScaleGroupBox: TGroupBox
      Left = 8
      Top = 143
      Width = 374
      Height = 139
      Anchors = [akLeft, akTop, akRight]
      Caption = #32302#23567#12539#25313#22823
      TabOrder = 3
      ExplicitWidth = 370
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
        Height = 23
        Style = csDropDownList
        DropDownCount = 10
        TabOrder = 0
        OnChange = ScaleModeComboBoxChange
      end
      object ScalePrm1Edit: TLabeledEdit
        Left = 101
        Top = 49
        Width = 75
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 55
        EditLabel.Height = 23
        EditLabel.Caption = #12497#12521#12513#12540#12479'1'
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 1
        Text = ''
      end
      object ScalePrm2Edit: TLabeledEdit
        Left = 271
        Top = 49
        Width = 75
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 55
        EditLabel.Height = 23
        EditLabel.Caption = #12497#12521#12513#12540#12479'2'
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 2
        Text = ''
      end
      object ScaleOptComboBox: TComboBox
        Left = 101
        Top = 109
        Width = 263
        Height = 23
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
  end
  object SubPanel: TPanel
    Left = 0
    Top = 289
    Width = 394
    Height = 122
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitWidth = 390
    DesignSize = (
      394
      122)
    object NotUsePrvCheckBox: TCheckBox
      Left = 24
      Top = 8
      Width = 348
      Height = 17
      Caption = #12503#12524#12499#12517#12540#12487#12540#12479#12434#21033#29992#12375#12394#12356'(WIC)'
      TabOrder = 0
    end
    object KeepTimeCheckBox: TCheckBox
      Left = 24
      Top = 99
      Width = 348
      Height = 17
      Caption = #12479#12452#12512#12473#12479#12531#12503#12434#32173#25345#12377#12427
      TabOrder = 1
    end
    object RenGroupBox: TGroupBox
      Left = 8
      Top = 41
      Width = 374
      Height = 50
      Anchors = [akLeft, akTop, akRight]
      Caption = #12501#12449#12452#12523#21517#12398#22793#26356
      TabOrder = 2
      ExplicitWidth = 370
      object ChgNameComboBox: TComboBox
        Left = 12
        Top = 20
        Width = 257
        Height = 23
        Style = csDropDownList
        TabOrder = 0
      end
      object ChgNameEdit: TEdit
        Left = 275
        Top = 20
        Width = 89
        Height = 23
        PopupMenu = UserModule.EditPopupMenuE
        TabOrder = 1
      end
    end
  end
  object BtnPanel: TPanel
    Left = 0
    Top = 510
    Width = 394
    Height = 42
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    ExplicitTop = 509
    ExplicitWidth = 390
    object CanButton: TButton
      Left = 210
      Top = 8
      Width = 80
      Height = 26
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object OkButton: TButton
      Left = 105
      Top = 8
      Width = 80
      Height = 26
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
  end
  object NamePanel: TPanel
    Left = 0
    Top = 411
    Width = 394
    Height = 90
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 390
    object NameGroupBox: TGroupBox
      Left = 8
      Top = 8
      Width = 378
      Height = 75
      Caption = #12501#12449#12452#12523#21517
      TabOrder = 0
      object FextLabel: TLabel
        Left = 336
        Top = 23
        Width = 28
        Height = 15
        Caption = '.bmp'
      end
      object SttLabel: TLabel
        Left = 12
        Top = 50
        Width = 70
        Height = 18
        Alignment = taRightJustify
        AutoSize = False
        Caption = #21516#21517#12354#12426
      end
      object ClipNameComboBox: TComboBox
        Left = 12
        Top = 20
        Width = 320
        Height = 23
        TabOrder = 0
        OnChange = ClipNameComboBoxChange
      end
      object ClipOWBtn: TRadioButton
        Left = 98
        Top = 49
        Width = 70
        Height = 17
        Caption = #19978#26360#12365
        TabOrder = 1
      end
      object ClipAutoBtn: TRadioButton
        Left = 172
        Top = 49
        Width = 114
        Height = 17
        Caption = ' '#33258#21205#25913#21517
        TabOrder = 2
      end
    end
  end
end
