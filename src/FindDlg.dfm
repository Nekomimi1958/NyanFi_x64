object FindFileDlg: TFindFileDlg
  Left = 0
  Top = 0
  HelpContext = 52
  BorderStyle = bsDialog
  Caption = #12501#12449#12452#12523#21517#26908#32034
  ClientHeight = 1371
  ClientWidth = 768
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
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object Bevel2: TBevel
    Left = 402
    Top = 0
    Width = 4
    Height = 1371
    Align = alLeft
    Shape = bsSpacer
  end
  object BasicPanel: TPanel
    Left = 0
    Top = 0
    Width = 400
    Height = 1371
    Align = alLeft
    BevelOuter = bvNone
    BorderWidth = 2
    ParentBackground = False
    TabOrder = 0
    ExplicitHeight = 1370
    object AttrPanel: TPanel
      Left = 2
      Top = 309
      Width = 396
      Height = 103
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 4
      StyleElements = []
      DesignSize = (
        396
        103)
      object AttrGroupBox: TGroupBox
        Left = 6
        Top = 6
        Width = 384
        Height = 89
        Anchors = [akLeft, akTop, akRight]
        Caption = #23646#24615'(&A)'
        TabOrder = 0
        DesignSize = (
          384
          89)
        object AttrRadioGroup: TRadioGroup
          Left = 12
          Top = 43
          Width = 359
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #21547#12416
            #21547#12414#12394#12356)
          TabOrder = 5
          OnClick = CondChangeUpdate
        end
        object AttrRCheckBox: TCheckBox
          Left = 12
          Top = 19
          Width = 79
          Height = 17
          Caption = #35501#21462#23554#29992
          TabOrder = 0
          OnClick = AttrCheckBoxClick
        end
        object AttrHCheckBox: TCheckBox
          Left = 93
          Top = 19
          Width = 55
          Height = 17
          Caption = #38560#12375
          TabOrder = 1
          OnClick = AttrCheckBoxClick
        end
        object AttrSCheckBox: TCheckBox
          Left = 150
          Top = 19
          Width = 79
          Height = 17
          Caption = #12471#12473#12486#12512
          TabOrder = 2
          OnClick = AttrCheckBoxClick
        end
        object AttrACheckBox: TCheckBox
          Left = 231
          Top = 19
          Width = 89
          Height = 17
          Caption = #12450#12540#12459#12452#12502
          TabOrder = 3
          OnClick = AttrCheckBoxClick
        end
        object AttrCCheckBox: TCheckBox
          Left = 322
          Top = 19
          Width = 55
          Height = 17
          Caption = #22311#32302
          TabOrder = 4
          OnClick = AttrCheckBoxClick
        end
      end
    end
    object ContPanel: TPanel
      Left = 2
      Top = 412
      Width = 396
      Height = 78
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 5
      StyleElements = []
      object ContRadioGroup: TRadioGroup
        Left = 4
        Top = 6
        Width = 386
        Height = 38
        Caption = #20869#23481'(&C)'
        Columns = 4
        Items.Strings = (
          #28961#35222
          #31354
          #31354#12391#12394#12356)
        TabOrder = 0
        OnClick = CondChangeUpdate
      end
      object DirWarnCheckBox: TCheckBox
        Left = 12
        Top = 52
        Width = 235
        Height = 17
        Caption = #22909#12414#12375#12367#12394#12356#12487#12451#12524#12463#12488#12522#21517
        TabOrder = 1
        OnClick = CondChangeUpdate
      end
    end
    object DatePanel: TPanel
      Left = 2
      Top = 97
      Width = 396
      Height = 106
      Align = alTop
      BevelOuter = bvNone
      ParentBackground = False
      ShowCaption = False
      TabOrder = 2
      StyleElements = []
      DesignSize = (
        396
        106)
      object DateGroupBox: TGroupBox
        Left = 6
        Top = 6
        Width = 384
        Height = 96
        Anchors = [akLeft, akTop, akRight]
        Caption = #26356#26032#26085#20184'(&D)'
        TabOrder = 0
        DesignSize = (
          384
          96)
        object DateMaskEdit: TMaskEdit
          Left = 12
          Top = 20
          Width = 87
          Height = 23
          EditMask = '!9999/99/99;1;_'
          MaxLength = 10
          TabOrder = 0
          Text = '    /  /  '
        end
        object DateRadioGroup: TRadioGroup
          Left = 12
          Top = 50
          Width = 359
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #19968#33268
            #20197#21069
            #20197#38477)
          TabOrder = 6
          OnClick = CondChangeUpdate
        end
        object DateBtn5: TButton
          Left = 285
          Top = 18
          Width = 44
          Height = 24
          Caption = '-1Y'
          TabOrder = 5
          OnClick = DateBtnClick
        end
        object DateBtn1: TButton
          Left = 109
          Top = 18
          Width = 44
          Height = 24
          Caption = #26412#26085
          TabOrder = 1
          OnClick = DateBtnClick
        end
        object DateBtn2: TButton
          Left = 153
          Top = 18
          Width = 44
          Height = 24
          Caption = '-1D'
          TabOrder = 2
          OnClick = DateBtnClick
        end
        object DateBtn3: TButton
          Left = 197
          Top = 18
          Width = 44
          Height = 24
          Caption = '-7D'
          TabOrder = 3
          OnClick = DateBtnClick
        end
        object DateBtn4: TButton
          Left = 241
          Top = 18
          Width = 44
          Height = 24
          Caption = '-1M'
          TabOrder = 4
          OnClick = DateBtnClick
        end
      end
    end
    object KeywordPanel: TPanel
      Left = 2
      Top = 37
      Width = 396
      Height = 60
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        396
        60)
      object KwdLabel: TLabel
        Left = 9
        Top = 7
        Width = 54
        Height = 15
        Alignment = taRightJustify
        Caption = #26908#32034#35486'(&K)'
        FocusControl = KeywordComboBox
      end
      object KeywordComboBox: TComboBox
        Left = 67
        Top = 4
        Width = 323
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
      end
      object RegExCheckBox: TCheckBox
        Left = 298
        Top = 33
        Width = 90
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #27491#35215#34920#29694
        TabOrder = 3
        OnClick = RegExCheckBoxClick
      end
      object AndCheckBox: TCheckBox
        Left = 115
        Top = 33
        Width = 88
        Height = 17
        Anchors = [akTop, akRight]
        Caption = 'AND'#26908#32034
        TabOrder = 1
      end
      object CaseCheckBox: TCheckBox
        Left = 206
        Top = 33
        Width = 90
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #22823#23567#21306#21029
        TabOrder = 2
      end
    end
    object MaskPanel: TPanel
      Left = 2
      Top = 2
      Width = 396
      Height = 35
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 0
      DesignSize = (
        396
        35)
      object TLabel
        Left = 15
        Top = 11
        Width = 48
        Height = 15
        Alignment = taRightJustify
        Caption = #12510#12473#12463'(&M)'
        FocusControl = MaskComboBox
      end
      object MaskComboBox: TComboBox
        Left = 67
        Top = 8
        Width = 323
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = MaskComboBoxChange
      end
    end
    object OkPanel: TPanel
      Left = 2
      Top = 490
      Width = 396
      Height = 130
      Align = alTop
      BevelOuter = bvNone
      ParentBackground = False
      ShowCaption = False
      TabOrder = 6
      DesignSize = (
        396
        130)
      object OkButton: TButton
        Left = 105
        Top = 98
        Width = 80
        Height = 26
        Action = FindOkAction
        Anchors = [akLeft, akBottom]
        Default = True
        ModalResult = 1
        TabOrder = 5
      end
      object SubDirCheckBox: TCheckBox
        Left = 12
        Top = 12
        Width = 184
        Height = 17
        Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12418#26908#32034'(&R)'
        TabOrder = 0
      end
      object CanButton: TButton
        Left = 210
        Top = 98
        Width = 80
        Height = 26
        Anchors = [akLeft, akBottom]
        Cancel = True
        Caption = #12461#12515#12531#12475#12523
        ModalResult = 2
        TabOrder = 6
      end
      object ExtraCheckBox: TCheckBox
        Left = 210
        Top = 35
        Width = 119
        Height = 17
        Caption = #25313#24373#26908#32034'(&X) '
        TabOrder = 2
        OnClick = ExtraCheckBoxClick
      end
      object ResLinkCheckBox: TCheckBox
        Left = 12
        Top = 61
        Width = 200
        Height = 17
        Caption = #32080#26524#12522#12473#12488#12363#12425#21453#23550#20596#12408#21453#26144
        TabOrder = 3
      end
      object DirLinkCheckBox: TCheckBox
        Left = 210
        Top = 61
        Width = 143
        Height = 17
        Caption = #12487#12451#12524#12463#12488#12522#21517#12434#21453#26144
        TabOrder = 4
      end
      object ArcCheckBox: TCheckBox
        Left = 210
        Top = 12
        Width = 184
        Height = 17
        Caption = #12450#12540#12459#12452#12502#20869#12418#26908#32034'(&V)'
        TabOrder = 1
        OnClick = ArcCheckBoxClick
      end
    end
    object SizePanel: TPanel
      Left = 2
      Top = 203
      Width = 396
      Height = 106
      Align = alTop
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 3
      StyleElements = []
      DesignSize = (
        396
        106)
      object SizeGroupBox: TGroupBox
        Left = 6
        Top = 6
        Width = 384
        Height = 96
        Anchors = [akLeft, akTop, akRight]
        Caption = #12469#12452#12474'(&S)'
        TabOrder = 0
        DesignSize = (
          384
          96)
        object SizeRadioGroup: TRadioGroup
          Left = 12
          Top = 50
          Width = 359
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #20197#19978)
          TabOrder = 5
          OnClick = CondChangeUpdate
        end
        object SizeEdit: TEdit
          Left = 12
          Top = 20
          Width = 87
          Height = 23
          Alignment = taRightJustify
          NumbersOnly = True
          TabOrder = 0
        end
        object BRadioButton: TRadioButton
          Left = 111
          Top = 22
          Width = 41
          Height = 17
          Caption = 'B'
          TabOrder = 1
        end
        object KBRadioButton: TRadioButton
          Left = 158
          Top = 22
          Width = 41
          Height = 17
          Caption = 'KB'
          TabOrder = 2
        end
        object MBRadioButton: TRadioButton
          Left = 207
          Top = 22
          Width = 41
          Height = 17
          Caption = 'MB'
          TabOrder = 3
        end
        object GBRadioButton: TRadioButton
          Left = 256
          Top = 22
          Width = 41
          Height = 17
          Caption = 'GB'
          TabOrder = 4
        end
      end
    end
  end
  object ExtPanel: TPanel
    Left = 406
    Top = 0
    Width = 360
    Height = 1371
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitHeight = 1370
    object PanelSpacer: TBevel
      Left = 0
      Top = 1132
      Width = 360
      Height = 6
      Align = alTop
      Shape = bsSpacer
    end
    object TimePanel: TPanel
      Left = 0
      Top = 0
      Width = 360
      Height = 68
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 0
      StyleElements = []
      object TimeGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 56
        Caption = #38263#12373'(&T)'
        TabOrder = 0
        DesignSize = (
          344
          56)
        object TimeMaskEdit: TMaskEdit
          Left = 12
          Top = 23
          Width = 73
          Height = 23
          Alignment = taRightJustify
          EditMask = '!99:99:99;1;_'
          MaxLength = 8
          TabOrder = 0
          Text = '  :  :  '
        end
        object TimeRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
      end
    end
    object FrmtPanel: TPanel
      Left = 0
      Top = 316
      Width = 360
      Height = 154
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 4
      StyleElements = []
      object FrmtGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 142
        Caption = #12469#12454#12531#12489#24418#24335'(&F)'
        TabOrder = 0
        DesignSize = (
          344
          142)
        object SmplRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #12469#12531#12503#12522#12531#12464#21608#27874#25968
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object SmplEdit: TLabeledEdit
          Left = 12
          Top = 25
          Width = 59
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 14
          EditLabel.Height = 23
          EditLabel.Caption = 'Hz'
          LabelPosition = lpRight
          TabOrder = 0
          Text = ''
        end
        object BitRadioGroup: TRadioGroup
          Left = 12
          Top = 54
          Width = 319
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #37327#23376#21270#12499#12483#12488#25968
          Columns = 5
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            '8'
            '16'
            '24'
            '32')
          TabOrder = 2
          OnClick = CondChangeUpdate
        end
        object ChnRadioGroup: TRadioGroup
          Left = 12
          Top = 96
          Width = 319
          Height = 38
          Caption = #12481#12515#12531#12493#12523#25968
          Columns = 6
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            '1'
            '2'
            '4'
            '6'
            '8')
          TabOrder = 3
          OnClick = CondChangeUpdate
        end
      end
    end
    object RatePanel: TPanel
      Left = 0
      Top = 68
      Width = 360
      Height = 68
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 1
      StyleElements = []
      object RateGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 56
        Caption = #12499#12483#12488#12524#12540#12488'(&B)'
        TabOrder = 0
        DesignSize = (
          344
          56)
        object RateRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object RateEdit: TLabeledEdit
          Left = 12
          Top = 23
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 25
          EditLabel.Height = 23
          EditLabel.Caption = 'kbps'
          LabelPosition = lpRight
          TabOrder = 0
          Text = ''
        end
      end
    end
    object FpsPanel: TPanel
      Left = 0
      Top = 136
      Width = 360
      Height = 70
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 2
      StyleElements = []
      object FpsGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 56
        Caption = #12501#12524#12540#12512#29575'(&P)'
        TabOrder = 0
        DesignSize = (
          344
          56)
        object FpsRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object FpsEdit: TLabeledEdit
          Left = 12
          Top = 23
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 16
          EditLabel.Height = 23
          EditLabel.Caption = 'fps'
          LabelPosition = lpRight
          TabOrder = 0
          Text = ''
        end
      end
    end
    object FrSzPanel: TPanel
      Left = 0
      Top = 206
      Width = 360
      Height = 110
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 3
      StyleElements = []
      object FrSzGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 99
        Caption = #12501#12524#12540#12512#12469#12452#12474'(&W)'
        TabOrder = 0
        DesignSize = (
          344
          99)
        object FrWdRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object FrWdEdit: TLabeledEdit
          Left = 29
          Top = 23
          Width = 58
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #24133
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object FrHiRadioGroup: TRadioGroup
          Left = 100
          Top = 54
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 3
          OnClick = CondChangeUpdate
        end
        object FrHiEdit: TLabeledEdit
          Left = 29
          Top = 65
          Width = 58
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #39640
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
      end
    end
    object ImgSzPanel: TPanel
      Left = 0
      Top = 470
      Width = 360
      Height = 138
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 5
      StyleElements = []
      object ImgSzGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 126
        Caption = #30011#20687#12469#12452#12474'(&W) '
        TabOrder = 0
        DesignSize = (
          344
          126)
        object ImgWdRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object ImgWdEdit: TLabeledEdit
          Left = 29
          Top = 23
          Width = 58
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #24133
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object ImgHiRadioGroup: TRadioGroup
          Left = 100
          Top = 54
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 3
          OnClick = CondChangeUpdate
        end
        object ImgHiEdit: TLabeledEdit
          Left = 29
          Top = 65
          Width = 58
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #39640
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object ImgMaxCheckBox: TCheckBox
          Left = 29
          Top = 98
          Width = 140
          Height = 17
          Caption = #38263#36794'/'#30701#36794#12391#27604#36611
          TabOrder = 4
        end
      end
    end
    object PropPanel: TPanel
      Left = 0
      Top = 784
      Width = 360
      Height = 86
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 7
      StyleElements = []
      object PropGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 74
        Caption = ' '
        TabOrder = 0
        DesignSize = (
          344
          74)
        object PrpRegExCheckBox: TCheckBox
          Left = 220
          Top = 48
          Width = 84
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #27491#35215#34920#29694
          TabOrder = 3
        end
        object PrpAndCheckBox: TCheckBox
          Left = 34
          Top = 48
          Width = 80
          Height = 17
          Anchors = [akTop, akRight]
          Caption = 'AND'#26908#32034
          TabOrder = 1
        end
        object PrpCaseCheckBox: TCheckBox
          Left = 125
          Top = 48
          Width = 88
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #22823#23567#21306#21029
          TabOrder = 2
        end
        object PrpKwdEdit: TLabeledEdit
          Left = 82
          Top = 18
          Width = 249
          Height = 23
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #26908#32034#25991#23383#21015
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
          OnChange = CondChangeUpdate
        end
      end
    end
    object IconPanel: TPanel
      Left = 0
      Top = 956
      Width = 360
      Height = 68
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 9
      StyleElements = []
      object IconGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 56
        Caption = #12450#12452#12467#12531#25968'(&N)'
        TabOrder = 0
        DesignSize = (
          344
          56)
        object IconRadioGroup: TRadioGroup
          Left = 100
          Top = 12
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object IconEdit: TLabeledEdit
          Left = 12
          Top = 23
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #20491
          LabelPosition = lpRight
          TabOrder = 0
          Text = ''
        end
      end
    end
    object ExifPanel: TPanel
      Left = 0
      Top = 608
      Width = 360
      Height = 176
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 6
      StyleElements = []
      object ExifGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 166
        Caption = 'Exif'#24773#22577'(&E)'
        TabOrder = 0
        DesignSize = (
          344
          166)
        object ExifRegExCheckBox: TCheckBox
          Left = 220
          Top = 50
          Width = 84
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #27491#35215#34920#29694
          TabOrder = 3
        end
        object ExifAndCheckBox: TCheckBox
          Left = 34
          Top = 50
          Width = 80
          Height = 17
          Anchors = [akTop, akRight]
          Caption = 'AND'#26908#32034
          TabOrder = 1
        end
        object ExifCaseCheckBox: TCheckBox
          Left = 125
          Top = 50
          Width = 88
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #22823#23567#21306#21029
          TabOrder = 2
        end
        object ExifKwdEdit: TLabeledEdit
          Left = 82
          Top = 20
          Width = 249
          Height = 23
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #26908#32034#25991#23383#21015
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
          OnChange = CondChangeUpdate
        end
        object GpsGroupBox: TGroupBox
          Left = 12
          Top = 81
          Width = 319
          Height = 76
          Caption = 'GPS'#24773#22577'(&G)'
          TabOrder = 4
          object TLabel
            Left = 11
            Top = 21
            Width = 55
            Height = 15
            Alignment = taRightJustify
            Caption = #32239#24230','#32076#24230
          end
          object TLabel
            Left = 154
            Top = 50
            Width = 11
            Height = 15
            Caption = 'm'
          end
          object KmLabel: TLabel
            Left = 194
            Top = 50
            Width = 17
            Height = 15
            Caption = 'km'
          end
          object LatLngComboBox: TComboBox
            Left = 70
            Top = 18
            Width = 235
            Height = 23
            TabOrder = 0
            OnChange = CondChangeUpdate
          end
          object GpsRangeEdit: TLabeledEdit
            Left = 70
            Top = 47
            Width = 80
            Height = 23
            Alignment = taRightJustify
            EditLabel.Width = 26
            EditLabel.Height = 23
            EditLabel.Caption = #31684#22258
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 1
            Text = ''
            OnChange = CondChangeUpdate
          end
        end
      end
    end
    object CodePanel: TPanel
      Left = 0
      Top = 1024
      Width = 360
      Height = 108
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 10
      StyleElements = []
      object CodeGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 95
        Caption = #12467#12540#12489#12506#12540#12472'/'#25913#34892#12467#12540#12489'(&C)'
        TabOrder = 0
        object LineBrkComboBox: TComboBox
          Left = 237
          Top = 23
          Width = 94
          Height = 23
          Style = csDropDownList
          TabOrder = 1
          OnChange = CondChangeUpdate
        end
        object CodePageComboBox: TComboBox
          Left = 12
          Top = 23
          Width = 217
          Height = 23
          Style = csDropDownList
          DropDownCount = 20
          TabOrder = 0
          OnChange = CondChangeUpdate
        end
        object BomRadioGroup: TRadioGroup
          Left = 12
          Top = 50
          Width = 319
          Height = 38
          Caption = 'BOM'
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20184#12365
            #28961#12375)
          TabOrder = 2
          OnClick = CondChangeUpdate
        end
      end
    end
    object TextPanel: TPanel
      Left = 0
      Top = 870
      Width = 360
      Height = 86
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 8
      StyleElements = []
      object TextGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 74
        Caption = #12486#12461#12473#12488'(&T)'
        TabOrder = 0
        DesignSize = (
          344
          74)
        object Label1: TLabel
          Left = 13
          Top = 21
          Width = 65
          Height = 15
          Alignment = taRightJustify
          Caption = #26908#32034#25991#23383#21015
          FocusControl = KeywordComboBox
        end
        object TxtRegExCheckBox: TCheckBox
          Left = 220
          Top = 48
          Width = 84
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #27491#35215#34920#29694
          TabOrder = 3
          OnClick = TxtRegExCheckBoxClick
        end
        object TxtAndCheckBox: TCheckBox
          Left = 34
          Top = 48
          Width = 80
          Height = 17
          Anchors = [akTop, akRight]
          Caption = 'AND'#26908#32034
          TabOrder = 1
        end
        object TxtCaseCheckBox: TCheckBox
          Left = 125
          Top = 48
          Width = 88
          Height = 17
          Anchors = [akTop, akRight]
          Caption = #22823#23567#21306#21029
          TabOrder = 2
        end
        object TxtKwdComboBox: TComboBox
          Left = 82
          Top = 18
          Width = 250
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
          OnChange = CondChangeUpdate
        end
      end
    end
    object LCntPanel: TPanel
      Left = 0
      Top = 1200
      Width = 360
      Height = 72
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 12
      StyleElements = []
      object LCntGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 60
        Caption = #12495#12540#12489#12522#12531#12463#12398#12522#12531#12463#12459#12454#12531#12488#25968'(&L)'
        TabOrder = 0
        DesignSize = (
          344
          60)
        object LCntRadioGroup: TRadioGroup
          Left = 100
          Top = 16
          Width = 231
          Height = 38
          Anchors = [akLeft, akTop, akRight]
          Caption = #26465#20214
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #28961#35222
            #20197#19979
            #19968#33268
            #20197#19978)
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
        object LCntEdit: TLabeledEdit
          Left = 12
          Top = 27
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 3
          EditLabel.Height = 23
          EditLabel.Caption = ' '
          LabelPosition = lpRight
          TabOrder = 0
          Text = ''
        end
      end
    end
    object TagsPanel: TPanel
      Left = 0
      Top = 1138
      Width = 360
      Height = 62
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 11
      StyleElements = []
      object TagGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 50
        Caption = #12479#12464'(&G)'
        TabOrder = 0
        DesignSize = (
          344
          50)
        object TagsComboBox: TComboBox
          Left = 12
          Top = 20
          Width = 320
          Height = 23
          Hint = #35079#25968#25351#23450#21487#33021'( ; '#21306#20999#12426#12391'AND'#12289#65372#21306#20999#12426#12391'OR)'
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 0
          OnChange = CondChangeUpdate
        end
      end
    end
    object OtherPanel: TPanel
      Left = 0
      Top = 1272
      Width = 360
      Height = 82
      Align = alTop
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 13
      StyleElements = []
      DesignSize = (
        360
        82)
      object OtherGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 344
        Height = 72
        Anchors = [akLeft, akTop, akBottom]
        Caption = #12381#12398#20182'(&O)'
        TabOrder = 0
        object NameWarnCheckBox: TCheckBox
          Left = 12
          Top = 45
          Width = 198
          Height = 17
          Caption = #22909#12414#12375#12367#12394#12356#12501#12449#12452#12523#21517
          TabOrder = 2
          OnClick = CondChangeUpdate
        end
        object HasAdsCheckBox: TCheckBox
          Left = 12
          Top = 20
          Width = 179
          Height = 17
          Caption = #20195#26367#12487#12540#12479#12473#12488#12522#12540#12512#12434#25345#12388
          TabOrder = 0
          OnClick = CondChangeUpdate
        end
        object UseProcCheckBox: TCheckBox
          Left = 195
          Top = 20
          Width = 142
          Height = 17
          Caption = #12503#12525#12475#12473#12364#20351#29992#20013
          TabOrder = 1
          OnClick = CondChangeUpdate
        end
      end
    end
  end
  object BevelPanel: TPanel
    Left = 400
    Top = 0
    Width = 2
    Height = 1371
    Align = alLeft
    AutoSize = True
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitHeight = 1370
    object Shape1: TShape
      Left = 0
      Top = 0
      Width = 1
      Height = 1371
      Align = alLeft
      Brush.Style = bsClear
      Pen.Color = clBtnShadow
    end
    object Shape2: TShape
      Left = 1
      Top = 0
      Width = 1
      Height = 1371
      Align = alLeft
      Brush.Style = bsClear
      Pen.Color = clBtnHighlight
    end
  end
  object ActionList1: TActionList
    Left = 304
    Top = 625
    object FindOkAction: TAction
      Caption = 'OK'
      OnExecute = FindOkActionExecute
      OnUpdate = FindOkActionUpdate
    end
  end
end
