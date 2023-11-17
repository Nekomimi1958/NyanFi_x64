object OptionDlg: TOptionDlg
  Left = 0
  Top = 0
  HelpContext = 8
  BorderStyle = bsDialog
  Caption = #12458#12503#12471#12519#12531#35373#23450
  ClientHeight = 671
  ClientWidth = 914
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
  OnDestroy = FormDestroy
  OnHelp = FormHelp
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object BottomPanel: TPanel
    Left = 0
    Top = 630
    Width = 914
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      914
      41)
    object CanButton: TButton
      Left = 477
      Top = 8
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      TabOrder = 1
      OnClick = CanButtonClick
    end
    object FindEdit: TLabeledEdit
      Left = 691
      Top = 10
      Width = 200
      Height = 23
      EditLabel.Width = 26
      EditLabel.Height = 23
      EditLabel.Caption = #26908#32034
      LabelPosition = lpLeft
      TabOrder = 2
      Text = ''
      OnChange = FindEditChange
    end
    object OkButton: TButton
      Left = 358
      Top = 8
      Width = 80
      Height = 26
      Action = OkAction
      Anchors = [akLeft, akBottom]
      Default = True
      TabOrder = 0
    end
  end
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 914
    Height = 630
    ActivePage = GeneralSheet
    Align = alClient
    MultiLine = True
    OwnerDraw = True
    TabOrder = 0
    TabWidth = 110
    StyleElements = [seFont, seBorder]
    OnChange = PageControl1Change
    OnDrawTab = PageControl1DrawTab
    object GeneralSheet: TTabSheet
      HelpContext = 9
      Caption = #19968#33324
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object CopyGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 211
        Caption = #12467#12500#12540#21205#20316
        TabOrder = 9
        object TLabel
          Left = 187
          Top = 92
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object TLabel
          Left = 381
          Top = 92
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object NopDtctTimeEdit: TLabeledEdit
          Left = 10
          Top = 116
          Width = 50
          Height = 23
          Hint = '0 '#12391#28961#21177
          Alignment = taRightJustify
          EditLabel.Width = 193
          EditLabel.Height = 23
          EditLabel.Caption = #31186#20197#19978#28961#25805#20316#12394#12425#24453#12385#26178#38291#12434#26368#23567#12395
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 5
          Text = ''
        end
        object NormalWaitTimeEdit: TLabeledEdit
          Left = 327
          Top = 89
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 75
          EditLabel.Height = 23
          EditLabel.Caption = #36890#24120#24453#12385#26178#38291
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
        object RemoteWaitTimeEdit: TLabeledEdit
          Left = 133
          Top = 89
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 107
          EditLabel.Height = 23
          EditLabel.Caption = #12493#12483#12488#12527#12540#12463#24453#12385#26178#38291
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 3
          Text = ''
        end
        object RemoveCdRoCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 303
          Height = 17
          Caption = 'CD-ROM'#12363#12425#12398#12467#12500#12540#12391#35501#36796#23554#29992#23646#24615#12434#35299#38500
          TabOrder = 0
        end
        object AutoRenFmtEdit: TLabeledEdit
          Left = 101
          Top = 45
          Width = 322
          Height = 23
          EditLabel.Width = 78
          EditLabel.Height = 23
          EditLabel.Caption = #33258#21205#25913#21517#26360#24335
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object CopyNoBuffCheckBox: TCheckBox
          Left = 10
          Top = 158
          Width = 380
          Height = 17
          Caption = #12461#12515#12483#12471#12517#12539#12522#12477#12540#12473#12434#12496#12452#12497#12473#12375#12289#12496#12483#12501#12449#12540#12394#12375'I/O'#12434#20351#29992
          TabOrder = 6
        end
        object ShowArcCopyPrgCheckBox: TCheckBox
          Left = 10
          Top = 183
          Width = 380
          Height = 17
          Caption = #12450#12540#12459#12452#12502#12363#12425#12398#12467#12500#12540#12391#36914#25431#29366#27841#12434#34920#31034
          TabOrder = 7
        end
        object CopyTagsCheckBox: TCheckBox
          Left = 327
          Top = 20
          Width = 101
          Height = 17
          Caption = #12479#12464#12434#12467#12500#12540
          TabOrder = 1
        end
        object BevelPanel1: TPanel
          Left = 5
          Top = 77
          Width = 423
          Height = 2
          AutoSize = True
          BevelOuter = bvNone
          TabOrder = 8
          object Shape1: TShape
            Left = 0
            Top = 0
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnShadow
          end
          object Shape2: TShape
            Left = 0
            Top = 1
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnHighlight
          end
        end
        object BevelPanel2: TPanel
          Left = 5
          Top = 146
          Width = 423
          Height = 2
          AutoSize = True
          BevelOuter = bvNone
          TabOrder = 9
          object Shape3: TShape
            Left = 0
            Top = 0
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnShadow
          end
          object Shape4: TShape
            Left = 0
            Top = 1
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnHighlight
          end
        end
      end
      object DeleteGroupBox: TGroupBox
        Left = 460
        Top = 231
        Width = 434
        Height = 46
        Caption = #21066#38500'/'#19978#26360#12365#21205#20316
        TabOrder = 10
        object DelUseTrsCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 176
          Height = 17
          Caption = #21066#38500#12395#12372#12415#31665#12434#20351#12358
          TabOrder = 0
        end
        object ForceDelCheckBox: TCheckBox
          Left = 194
          Top = 20
          Width = 232
          Height = 17
          Caption = #35501#36796#23554#29992#12391#12418#24375#21046#30340#12395#19978#26360#12365#12539#21066#38500
          TabOrder = 1
        end
      end
      object DirHistGroupBox: TGroupBox
        Left = 460
        Top = 369
        Width = 434
        Height = 145
        Caption = #12487#12451#12524#12463#12488#12522#23653#27508
        TabOrder = 12
        DesignSize = (
          434
          145)
        object DelDplDirHistCheckBox: TCheckBox
          Left = 218
          Top = 45
          Width = 200
          Height = 17
          Caption = #37325#35079#12377#12427#23653#27508#12434#21066#38500#12377#12427
          TabOrder = 3
        end
        object DirHistCsrPosCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 200
          Height = 17
          Caption = #12459#12540#12477#12523#20301#32622#12434#35352#25014#12377#12427
          TabOrder = 0
        end
        object NoChkDirHistCheckBox: TCheckBox
          Left = 10
          Top = 45
          Width = 200
          Height = 17
          Caption = #23653#27508#12398#23384#22312#12481#12455#12483#12463#12434#34892#12431#12394#12356
          TabOrder = 2
        end
        object WorkToDirHistCheckBox: TCheckBox
          Left = 10
          Top = 70
          Width = 200
          Height = 17
          Caption = #12527#12540#12463#12522#12473#12488#12434#23653#27508#12395#21547#12417#12427
          TabOrder = 4
        end
        object ExtSavDirHistCheckBox: TCheckBox
          Left = 218
          Top = 70
          Width = 200
          Height = 17
          Caption = #20840#20307#23653#27508#12434#21029#12501#12449#12452#12523#12395#20445#23384
          TabOrder = 5
          OnClick = ExtSavDirHistCheckBoxClick
        end
        object NoDirHistPathEdit: TLabeledEdit
          Left = 71
          Top = 113
          Width = 352
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 164
          EditLabel.Height = 15
          EditLabel.Caption = #23653#27508#12395#20837#12428#12394#12356#12497#12473'('#37096#20998#19968#33268')'
          TabOrder = 7
          Text = ''
        end
        object DirHistSrotMdCheckBox: TCheckBox
          Left = 218
          Top = 20
          Width = 200
          Height = 17
          Caption = #12477#12540#12488#26041#27861#12434#35352#25014#12377#12427
          TabOrder = 1
        end
        object MaxDirHistEdit: TLabeledEdit
          Left = 10
          Top = 113
          Width = 45
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 39
          EditLabel.Height = 15
          EditLabel.Caption = #20445#23384#25968
          NumbersOnly = True
          TabOrder = 6
          Text = ''
        end
      end
      object DispGroupBox: TGroupBox
        Left = 10
        Top = 77
        Width = 434
        Height = 186
        Caption = #34920#31034
        TabOrder = 4
        object TLabel
          Left = 10
          Top = 130
          Width = 63
          Height = 15
          Caption = #12479#12452#12488#12523#12496#12540':'
        end
        object HideAtrCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 129
          Height = 17
          Caption = #38560#12375#12501#12449#12452#12523#12434#34920#31034
          TabOrder = 0
        end
        object SysAtrCheckBox: TCheckBox
          Left = 143
          Top = 20
          Width = 154
          Height = 17
          Caption = #12471#12473#12486#12512#12501#12449#12452#12523#12434#34920#31034
          TabOrder = 1
        end
        object DispRegNameCheckBox: TCheckBox
          Left = 10
          Top = 45
          Width = 200
          Height = 17
          Caption = #12497#12473#34920#31034#12395#30331#37682#21517#12434#20351#29992
          TabOrder = 3
        end
        object UncToNetDriveCheckBox: TCheckBox
          Left = 216
          Top = 45
          Width = 200
          Height = 17
          Caption = 'UNC'#12434#12493#12483#12488#12527#12540#12463#12489#12521#12452#12502#21517#12395
          TabOrder = 4
        end
        object PathInTitleBarCheckBox: TCheckBox
          Left = 297
          Top = 129
          Width = 105
          Height = 17
          Caption = #12459#12524#12531#12488#12497#12473
          TabOrder = 10
        end
        object IconCacheEdit: TLabeledEdit
          Left = 313
          Top = 68
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 99
          EditLabel.Height = 23
          EditLabel.Caption = #12450#12452#12467#12531#12461#12515#12483#12471#12517#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 6
          Text = ''
        end
        object UseIndIconCheckBox: TCheckBox
          Left = 10
          Top = 70
          Width = 200
          Height = 17
          Caption = #12501#12449#12452#12523#22266#26377#12398#12450#12452#12467#12531#12434#34920#31034
          TabOrder = 5
        end
        object DirDelimiterEdit: TLabeledEdit
          Left = 387
          Top = 18
          Width = 36
          Height = 23
          EditLabel.Width = 88
          EditLabel.Height = 23
          EditLabel.Caption = #12487#12451#12524#12463#12488#12522#21306#20999#12426
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object GetFaviUrlEdit: TLabeledEdit
          Left = 110
          Top = 97
          Width = 313
          Height = 23
          Hint = ' \D : '#12489#12513#12452#12531#21517
          EditLabel.Width = 86
          EditLabel.Height = 23
          EditLabel.Caption = 'favicon '#21462#24471'API'
          LabelPosition = lpLeft
          TabOrder = 7
          Text = ''
        end
        object CompInTitleBarCheckBox: TCheckBox
          Left = 83
          Top = 129
          Width = 105
          Height = 17
          Caption = #12467#12531#12500#12517#12540#12479#21517
          TabOrder = 8
        end
        object TabGrInTitleBarCheckBox: TCheckBox
          Left = 190
          Top = 129
          Width = 105
          Height = 17
          Caption = #12479#12502#12464#12523#12540#12503
          TabOrder = 9
        end
        object DlgCenterCheckBox: TCheckBox
          Left = 10
          Top = 158
          Width = 223
          Height = 17
          Caption = #12480#12452#12450#12525#12464#12434#12513#12452#12531#30011#38754#12398#20013#22830#12395#34920#31034
          TabOrder = 11
        end
        object NoRoundWinCheckBox: TCheckBox
          Left = 241
          Top = 158
          Width = 182
          Height = 17
          Caption = #35282#20024#12454#12451#12531#12489#12454#12398#28961#21177#21270
          TabOrder = 12
        end
      end
      object MigemoDirEdit: TLabeledEdit
        Left = 120
        Top = 41
        Width = 269
        Height = 23
        EditLabel.Width = 102
        EditLabel.Height = 23
        EditLabel.Caption = 'Migemo '#12487#12451#12524#12463#12488#12522
        LabelPosition = lpLeft
        TabOrder = 2
        Text = ''
      end
      object OpeGroupBox: TGroupBox
        Left = 10
        Top = 273
        Width = 434
        Height = 98
        Caption = #25805#20316
        TabOrder = 5
        object FlCsrVisiCheckBox: TCheckBox
          Left = 216
          Top = 20
          Width = 210
          Height = 17
          Caption = #12459#12540#12477#12523#12434#24120#12395#21487#35222#38936#22495#12395
          TabOrder = 1
        end
        object LoopFlCsrCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 200
          Height = 17
          Caption = #19978#19979#31471#12391#12523#12540#12503#31227#21205
          TabOrder = 0
        end
        object AutoCompCheckBox: TCheckBox
          Left = 216
          Top = 70
          Width = 169
          Height = 17
          Caption = #12467#12531#12508#12508#12483#12463#12473#12398#33258#21205#35036#23436
          TabOrder = 5
        end
        object AutoHotkeyCheckBox: TCheckBox
          Left = 10
          Top = 70
          Width = 200
          Height = 17
          Caption = #12450#12463#12475#12521#12524#12540#12479#12461#12540#33258#21205#35519#25972
          TabOrder = 4
        end
        object EscHelpCheckBox: TCheckBox
          Left = 10
          Top = 45
          Width = 200
          Height = 17
          Caption = 'ESC'#12461#12540#12391#12504#12523#12503#12434#38281#12376#12427
          TabOrder = 2
        end
        object InhAltMenuCheckBox: TCheckBox
          Left = 216
          Top = 45
          Width = 210
          Height = 17
          Caption = 'Alt'#12461#12540#12391#12513#12491#12517#12540#12395#31227#12425#12394#12356
          TabOrder = 3
        end
      end
      object RecentGroupBox: TGroupBox
        Left = 460
        Top = 523
        Width = 434
        Height = 50
        Caption = #26368#36817#20351#12387#12383#12501#12449#12452#12523
        TabOrder = 13
        object NoChkRUncCheckBox: TCheckBox
          Left = 269
          Top = 20
          Width = 157
          Height = 17
          Caption = 'UNC'#12497#12473#12434#12481#12455#12483#12463#12375#12394#12356
          TabOrder = 1
        end
        object AddToRecentCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 253
          Height = 17
          Caption = #38283#12356#12383#12501#12449#12452#12523#12434#12300#26368#36817#20351#12387#12383#38917#30446#12301#12395#36861#21152
          TabOrder = 0
        end
      end
      object RefMigemoBtn: TButton
        Left = 392
        Top = 41
        Width = 36
        Height = 22
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 3
        OnClick = RefMigemoBtnClick
      end
      object RefTmpDirBtn: TButton
        Left = 392
        Top = 12
        Width = 36
        Height = 22
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 1
        OnClick = RefTmpDirBtnClick
      end
      object TaskGroupBox: TGroupBox
        Left = 10
        Top = 523
        Width = 213
        Height = 50
        Caption = #12479#12473#12463
        TabOrder = 7
        object TLabel
          Left = 52
          Top = 21
          Width = 67
          Height = 15
          Alignment = taRightJustify
          Caption = #26368#22823#12479#12473#12463#25968
        end
        object MaxTasksComboBox: TComboBox
          Left = 125
          Top = 18
          Width = 58
          Height = 23
          Style = csDropDownList
          TabOrder = 0
        end
      end
      object TempDirEdit: TLabeledEdit
        Left = 120
        Top = 12
        Width = 269
        Height = 23
        EditLabel.Width = 80
        EditLabel.Height = 23
        EditLabel.Caption = #19968#26178#12487#12451#12524#12463#12488#12522
        LabelPosition = lpLeft
        TabOrder = 0
        Text = ''
      end
      object TimeGroupBox: TGroupBox
        Left = 231
        Top = 523
        Width = 213
        Height = 50
        Caption = #12479#12452#12512#12473#12479#12531#12503#12398#35377#23481#35492#24046
        TabOrder = 8
        object ToleranceEdit: TLabeledEdit
          Left = 106
          Top = 18
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 24
          EditLabel.Height = 23
          EditLabel.Caption = 'm'#31186
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
      end
      object WatchGroupBox: TGroupBox
        Left = 10
        Top = 381
        Width = 434
        Height = 130
        Caption = #30435#35222'/'#26356#26032
        TabOrder = 6
        DesignSize = (
          434
          130)
        object TLabel
          Left = 397
          Top = 50
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object ReloadActCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 200
          Height = 17
          Caption = #12450#12463#12486#12451#12502#26178#12395#26368#26032#12398#24773#22577#12395
          TabOrder = 0
        end
        object OpenAddDrvCheckBox: TCheckBox
          Left = 216
          Top = 20
          Width = 206
          Height = 17
          Caption = #36861#21152#12489#12521#12452#12502#12434#12459#12524#12531#12488#12395#34920#31034
          TabOrder = 1
        end
        object WatchIntervalEdit: TLabeledEdit
          Left = 345
          Top = 47
          Width = 50
          Height = 23
          Hint = '0 '#12391#30435#35222#12434#20572#27490
          Alignment = taRightJustify
          EditLabel.Width = 106
          EditLabel.Height = 23
          EditLabel.Caption = #12487#12451#12524#12463#12488#12522#30435#35222#38291#38548
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 3
          Text = ''
        end
        object NoWatchPathEdit: TLabeledEdit
          Left = 10
          Top = 96
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 174
          EditLabel.Height = 15
          EditLabel.Caption = #30435#35222#12363#12425#38500#22806#12377#12427#12497#12473'('#21069#26041#19968#33268')'
          TabOrder = 4
          Text = ''
        end
        object CheckUncCheckBox: TCheckBox
          Left = 10
          Top = 49
          Width = 143
          Height = 17
          Caption = 'UNC'#12497#12473#12434#12481#12455#12483#12463
          TabOrder = 2
        end
      end
      object WorkListGroupBox: TGroupBox
        Left = 460
        Top = 287
        Width = 434
        Height = 72
        Caption = #12527#12540#12463#12522#12473#12488
        TabOrder = 11
        object NotSortWLCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 139
          Height = 17
          Caption = #38917#30446#12434#12477#12540#12488#12375#12394#12356' '
          TabOrder = 0
        end
        object AutoDelWLCheckBox: TCheckBox
          Left = 10
          Top = 45
          Width = 139
          Height = 17
          Caption = #38917#30446#12398#33258#21205#21066#38500
          TabOrder = 2
        end
        object NoChkWUncCheckBox: TCheckBox
          Left = 153
          Top = 45
          Width = 165
          Height = 17
          Caption = 'UNC'#12497#12473#12434#12481#12455#12483#12463#12375#12394#12356
          TabOrder = 3
        end
        object RegWkCsrPosCheckBox: TCheckBox
          Left = 153
          Top = 20
          Width = 165
          Height = 17
          Caption = #12459#12540#12477#12523#20301#32622#12395#30331#37682
          TabOrder = 1
        end
      end
    end
    object General2Sheet: TTabSheet
      HelpContext = 24
      Caption = #19968#33324'2'
      ImageIndex = -1
      object KeyboardLabel: TLabel
        Left = 515
        Top = 320
        Width = 19
        Height = 15
        Caption = '(JP)'
      end
      object TLabel
        Left = 244
        Top = 551
        Width = 24
        Height = 15
        Caption = 'm'#31186
      end
      object DlgKeyGroupBox: TGroupBox
        Left = 460
        Top = 209
        Width = 434
        Height = 77
        Caption = #12461#12540#12395#12424#12427#12480#12452#12450#12525#12464#12398#31227#21205'/'#12469#12452#12474#22793#26356
        TabOrder = 8
        object DlgMoveLabel: TLabel
          Left = 170
          Top = 21
          Width = 111
          Height = 15
          Caption = '+ '#12459#12540#12477#12523#12461#12540#12391#31227#21205
        end
        object DlgSizeLabel: TLabel
          Left = 170
          Top = 48
          Width = 142
          Height = 15
          Caption = '+ '#12459#12540#12477#12523#12461#12540#12391#12469#12452#12474#22793#26356
        end
        object DlgMoveAltCheckBox: TCheckBox
          Left = 120
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Alt'
          TabOrder = 2
        end
        object DlgMoveCtrlCheckBox: TCheckBox
          Left = 65
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 1
        end
        object DlgMoveShiftCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Shift'
          TabOrder = 0
        end
        object DlgSizeShiftCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 47
          Height = 17
          Caption = 'Shift'
          TabOrder = 4
        end
        object DlgSizeCtrlCheckBox: TCheckBox
          Left = 65
          Top = 47
          Width = 47
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 5
        end
        object DlgSizeAltCheckBox: TCheckBox
          Left = 120
          Top = 47
          Width = 47
          Height = 17
          Caption = 'Alt'
          TabOrder = 6
        end
        object DlgMoveEdit: TEdit
          Left = 344
          Top = 18
          Width = 45
          Height = 23
          Alignment = taRightJustify
          NumbersOnly = True
          TabOrder = 3
        end
        object DlgSizeEdit: TEdit
          Left = 344
          Top = 45
          Width = 45
          Height = 23
          Alignment = taRightJustify
          NumbersOnly = True
          TabOrder = 7
        end
      end
      object EtcGroupBox: TGroupBox
        Left = 460
        Top = 360
        Width = 434
        Height = 211
        Caption = #12381#12398#20182
        TabOrder = 11
        object PermitCmdsCheckBox: TCheckBox
          Left = 10
          Top = 78
          Width = 253
          Height = 17
          Caption = '.nyanfi '#12391#12398#12467#12510#12531#12489#12501#12449#12452#12523#23455#34892#12434#35377#21487
          TabOrder = 3
        end
        object InheritDotCheckBox: TCheckBox
          Left = 10
          Top = 51
          Width = 253
          Height = 17
          Caption = '.nyanfi '#12364#12394#12356#22580#21512#12289#19978#20301#12363#12425#32153#25215
          TabOrder = 1
        end
        object DotPerUserCheckBox: TCheckBox
          Left = 272
          Top = 51
          Width = 155
          Height = 17
          Caption = #12518#12540#12470#21517#21029#12395#20316#25104
          TabOrder = 2
        end
        object TaskTrayCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 253
          Height = 17
          Caption = #26368#23567#21270#26178#12395#12450#12452#12467#12531#12434#36890#30693#38936#22495#12395#26684#32013
          TabOrder = 0
        end
        object FExt7zDllEdit: TLabeledEdit
          Left = 10
          Top = 128
          Width = 413
          Height = 23
          EditLabel.Width = 204
          EditLabel.Height = 15
          EditLabel.Caption = '7-zip32.dll(7z.dll'#23550#24540#29256')'#12398#23550#24540#25313#24373#23376
          TabOrder = 4
          Text = ''
        end
        object CmdGitExeEdit: TLabeledEdit
          Left = 10
          Top = 175
          Width = 374
          Height = 23
          EditLabel.Width = 131
          EditLabel.Height = 15
          EditLabel.Caption = 'git.exe (Git for Windows)'
          TabOrder = 5
          Text = ''
        end
        object RefCmdGitExeBtn: TButton
          Tag = 4
          Left = 387
          Top = 175
          Width = 36
          Height = 22
          Caption = '...'
          TabOrder = 6
          OnClick = RefCmdGitExeBtnClick
        end
      end
      object FileInfoGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 187
        Caption = #12501#12449#12452#12523#24773#22577
        TabOrder = 0
        DesignSize = (
          434
          187)
        object FExtGetInfEdit: TLabeledEdit
          Left = 10
          Top = 79
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 122
          EditLabel.Height = 15
          EditLabel.Caption = #24773#22577#12434#21462#24471#12377#12427#25313#24373#23376
          TabOrder = 1
          Text = ''
        end
        object FExtNoInfEdit: TLabeledEdit
          Left = 10
          Top = 124
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 187
          EditLabel.Height = 15
          EditLabel.Caption = #24773#22577#12434#21462#24471#12375#12394#12356#25313#24373#23376'[:'#21046#38480'MB]'
          TabOrder = 2
          Text = ''
        end
        object NoInfPathEdit: TLabeledEdit
          Left = 10
          Top = 34
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 174
          EditLabel.Height = 15
          EditLabel.Caption = #24773#22577#12434#21462#24471#12375#12394#12356#12497#12473'('#21069#26041#19968#33268')'
          TabOrder = 0
          Text = ''
        end
        object ShowAdsInfCheckBox: TCheckBox
          Left = 10
          Top = 157
          Width = 215
          Height = 17
          Caption = #20195#26367#12487#12540#12479#12473#12488#12522#12540#12512#24773#22577#12434#34920#31034
          TabOrder = 3
        end
        object ShowProcInfCheckBox: TCheckBox
          Left = 237
          Top = 157
          Width = 186
          Height = 17
          Caption = #12503#12525#12475#12473#20351#29992#24773#22577#12434#34920#31034
          TabOrder = 4
        end
      end
      object HotKeyGroupBox: TGroupBox
        Left = 460
        Top = 147
        Width = 434
        Height = 50
        Caption = 'NyanFi  '#21628#12403#20986#12375#12507#12483#12488#12461#12540
        TabOrder = 7
        object HotAltCheckBox: TCheckBox
          Left = 120
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Alt'
          TabOrder = 2
        end
        object HotCtrlCheckBox: TCheckBox
          Left = 65
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 1
        end
        object HotShiftCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 47
          Height = 17
          Caption = 'Shift'
          TabOrder = 0
        end
        object HotKeyComboBox: TComboBox
          Left = 253
          Top = 18
          Width = 70
          Height = 23
          Style = csDropDownList
          DropDownCount = 24
          ImeMode = imClose
          TabOrder = 4
        end
        object HotWinCheckBox: TCheckBox
          Left = 175
          Top = 20
          Width = 71
          Height = 17
          Caption = 'Win   +'
          TabOrder = 3
        end
      end
      object ImgPrvGroupBox: TGroupBox
        Left = 10
        Top = 209
        Width = 434
        Height = 210
        Caption = #12452#12513#12540#12472#12503#12524#12499#12517#12540
        TabOrder = 1
        DesignSize = (
          434
          210)
        object DirJumboCheckBox: TCheckBox
          Left = 10
          Top = 157
          Width = 215
          Height = 17
          Caption = #12487#12451#12524#12463#12488#12522#12399#29305#22823#12450#12452#12467#12531#12434#34920#31034
          TabOrder = 3
        end
        object NoImgPrvPathEdit: TLabeledEdit
          Left = 10
          Top = 34
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 186
          EditLabel.Height = 15
          EditLabel.Caption = #12503#12524#12499#12517#12540#12434#34892#12431#12394#12356#12497#12473'('#21069#26041#19968#33268')'
          TabOrder = 0
          Text = ''
        end
        object FExtNoImgPrvEdit: TLabeledEdit
          Left = 10
          Top = 124
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 199
          EditLabel.Height = 15
          EditLabel.Caption = #12503#12524#12499#12517#12540#12434#34892#12431#12394#12356#25313#24373#23376'[:'#21046#38480'MB]'
          TabOrder = 2
          Text = ''
        end
        object FExtImgPrvEdit: TLabeledEdit
          Left = 10
          Top = 79
          Width = 413
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 119
          EditLabel.Height = 15
          EditLabel.Caption = #12503#12524#12499#12517#12540#12434#34892#12358#25313#24373#23376
          TabOrder = 1
          Text = ''
        end
        object AniGifCheckBox: TCheckBox
          Left = 10
          Top = 182
          Width = 147
          Height = 17
          Caption = #12450#12491#12513'GIF'#12395#23550#24540
          TabOrder = 5
        end
        object InZipImgCheckBox: TCheckBox
          Left = 237
          Top = 157
          Width = 172
          Height = 17
          Caption = 'ZIP'#20869#12398#30011#20687#12434#34920#31034
          TabOrder = 4
        end
        object SetCursorCheckBox: TCheckBox
          Left = 237
          Top = 182
          Width = 172
          Height = 17
          Caption = #12510#12454#12473#12509#12452#12531#12479#12434#35373#23450
          TabOrder = 6
        end
      end
      object KeybdRadioGroup: TRadioGroup
        Left = 460
        Top = 298
        Width = 245
        Height = 50
        Caption = #12461#12540#12508#12540#12489#12398#31278#39006
        Columns = 3
        Items.Strings = (
          #33258#21205
          'JP'
          'US')
        TabOrder = 9
        OnClick = KeybdRadioGroupClick
      end
      object LogGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 127
        Caption = #12525#12464
        TabOrder = 6
        object TLabel
          Left = 128
          Top = 19
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object SaveLogCheckBox: TCheckBox
          Left = 10
          Top = 98
          Width = 97
          Height = 17
          Caption = #12525#12464#12434#20445#23384
          TabOrder = 8
        end
        object LogErrOnlyCheckBox: TCheckBox
          Left = 10
          Top = 48
          Width = 147
          Height = 17
          Caption = #12456#12521#12540#31623#25152#12398#12415#34920#31034
          TabOrder = 2
        end
        object LogErrMsgCheckBox: TCheckBox
          Left = 165
          Top = 48
          Width = 132
          Height = 17
          Caption = #12456#12521#12540#12513#12483#12475#12540#12472
          TabOrder = 3
        end
        object AppendLogCheckBox: TCheckBox
          Left = 120
          Top = 98
          Width = 138
          Height = 17
          Caption = #21516#26085#12398#12525#12464#12399#36861#35352
          TabOrder = 9
        end
        object LogIntervalEdit: TLabeledEdit
          Left = 76
          Top = 16
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #26356#26032#38291#38548
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
        object MaxLogLinesEdit: TLabeledEdit
          Left = 277
          Top = 16
          Width = 48
          Height = 23
          Hint = '0 '#12391#28961#21046#38480
          Alignment = taRightJustify
          EditLabel.Width = 78
          EditLabel.Height = 23
          EditLabel.Caption = #26368#22823#34920#31034#34892#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 1
          Text = ''
        end
        object MaxLogFilesEdit: TLabeledEdit
          Left = 344
          Top = 96
          Width = 45
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #20445#23384#19990#20195#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 10
          Text = ''
        end
        object LogFullPathCheckBox: TCheckBox
          Left = 165
          Top = 73
          Width = 132
          Height = 17
          Caption = #12501#12523#12497#12473#34920#31034
          TabOrder = 6
        end
        object LogHideSkipCheckBox: TCheckBox
          Left = 10
          Top = 73
          Width = 147
          Height = 17
          Caption = #12473#12461#12483#12503#38917#30446#38750#34920#31034
          TabOrder = 5
        end
        object LogDebugInfCheckBox: TCheckBox
          Left = 307
          Top = 48
          Width = 116
          Height = 17
          Caption = #12487#12496#12483#12464#24773#22577
          TabOrder = 4
        end
        object LogDestinationCheckBox: TCheckBox
          Left = 307
          Top = 73
          Width = 116
          Height = 17
          Caption = #20966#29702#20808#12434#34920#31034
          TabOrder = 7
        end
      end
      object PlayGroupBox: TGroupBox
        Left = 713
        Top = 298
        Width = 181
        Height = 50
        Caption = #12469#12454#12531#12489#20877#29983
        TabOrder = 10
        object TLabel
          Left = 138
          Top = 21
          Width = 13
          Height = 15
          Caption = #31186
        end
        object LimitSndEdit: TLabeledEdit
          Left = 91
          Top = 18
          Width = 45
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #20877#29983#21046#38480
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
      end
      object TxtPrvGroupBox: TGroupBox
        Left = 10
        Top = 431
        Width = 434
        Height = 77
        Caption = #12486#12461#12473#12488#12503#12524#12499#12517#12540
        TabOrder = 2
        object PrevTxtToInfCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 55
          Height = 17
          Caption = #26377#21177
          TabOrder = 0
        end
        object PrvTxtInfLnEdit: TLabeledEdit
          Left = 135
          Top = 18
          Width = 58
          Height = 23
          Hint = '0 '#12398#22580#21512#12377#12409#12390#34920#31034
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #26368#22823#34892#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 1
          Text = ''
        end
        object PrevTailCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 116
          Height = 17
          Caption = #26411#23614#20998#21106#34920#31034
          TabOrder = 2
        end
        object PrvTxtTailLnEdit: TLabeledEdit
          Left = 135
          Top = 45
          Width = 58
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #34892
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 3
          Text = ''
        end
        object PrvActTailLnEdit: TLabeledEdit
          Left = 237
          Top = 45
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 85
          EditLabel.Height = 23
          EditLabel.Caption = #34892#20197#19978#12394#12425#20998#21106
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
      end
      object ViewArcInfCheckBox: TCheckBox
        Left = 20
        Top = 522
        Width = 237
        Height = 17
        Caption = #12450#12540#12459#12452#12502#20869#12398#24773#22577#34920#31034'('#19968#26178#35299#20941')'
        TabOrder = 3
      end
      object CheckTSCheckBox: TCheckBox
        Left = 269
        Top = 522
        Width = 175
        Height = 17
        Caption = #25313#24373#23376' .ts '#12398#21028#21029#12434#34892#12358
        TabOrder = 4
      end
      object InfPrvWaitEdit: TLabeledEdit
        Left = 183
        Top = 548
        Width = 58
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 143
        EditLabel.Height = 23
        EditLabel.Caption = #24773#22577#21462#24471'/'#12503#12524#12499#12517#12540#12398#36933#24310
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 5
        Text = ''
      end
    end
    object MouseSheet: TTabSheet
      HelpContext = 22
      Caption = #12510#12454#12473#25805#20316
      ImageIndex = -1
      object MouseFlGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 511
        Caption = #12501#12449#12452#12521#12540
        TabOrder = 0
        DesignSize = (
          434
          511)
        object TLabel
          Left = 10
          Top = 22
          Width = 150
          Height = 15
          Caption = #12501#12449#12452#12523#12522#12473#12488#12391#12398#12480#12502#12523#12463#12522#12483#12463
        end
        object TLabel
          Left = 10
          Top = 72
          Width = 208
          Height = 15
          Caption = #12501#12449#12452#12523#12522#12473#12488#12408#12398#12487#12501#12457#12523#12488#12398#12489#12525#12483#12503#21205#20316
        end
        object TLabel
          Left = 20
          Top = 176
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12510#12454#12473#12507#12452#12540#12523
        end
        object TLabel
          Left = 32
          Top = 257
          Width = 59
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+Shift+'
        end
        object TLabel
          Left = 59
          Top = 230
          Width = 32
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Shift+'
        end
        object TLabel
          Left = 64
          Top = 203
          Width = 27
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+'
        end
        object TLabel
          Left = 20
          Top = 330
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12507#12452#12540#12523#12508#12479#12531
        end
        object TLabel
          Left = 32
          Top = 411
          Width = 59
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+Shift+'
        end
        object TLabel
          Left = 59
          Top = 384
          Width = 32
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Shift+'
        end
        object TLabel
          Left = 64
          Top = 357
          Width = 27
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+'
        end
        object TLabel
          Left = 48
          Top = 448
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X1'#12508#12479#12531
        end
        object TLabel
          Left = 48
          Top = 475
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X2'#12508#12479#12531
        end
        object DblClickFlComboBox: TComboBox
          Left = 10
          Top = 41
          Width = 413
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
          OnChange = LayoutChange
        end
        object DefDropComboBox: TComboBox
          Left = 10
          Top = 91
          Width = 413
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 1
          OnChange = LayoutChange
        end
        object SelBaseOnlyCheckBox: TCheckBox
          Left = 115
          Top = 131
          Width = 156
          Height = 17
          Caption = #12501#12449#12452#12523#21517#37096#20998#12391#36984#25246
          TabOrder = 3
        end
        object SelByMouseCheckBox: TCheckBox
          Left = 10
          Top = 131
          Width = 101
          Height = 17
          Caption = #12510#12454#12473#12391#36984#25246
          TabOrder = 2
        end
        object ListWheelScrLnEdit: TLabeledEdit
          Left = 383
          Top = 285
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 151
          EditLabel.Height = 23
          EditLabel.Caption = ' '#12507#12452#12540#12523#12395#12424#12427#12473#12463#12525#12540#12523#34892#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 9
          Text = ''
        end
        object WheelCmdF0ComboBox: TComboBox
          Left = 95
          Top = 173
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 5
        end
        object WheelCmdF3ComboBox: TComboBox
          Left = 95
          Top = 254
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 8
        end
        object WheelCmdF2ComboBox: TComboBox
          Left = 95
          Top = 227
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 7
        end
        object WheelCmdF1ComboBox: TComboBox
          Left = 95
          Top = 200
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 6
        end
        object WheelBtnF0ComboBox: TComboBox
          Left = 95
          Top = 327
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 10
        end
        object WheelBtnF3ComboBox: TComboBox
          Left = 95
          Top = 408
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 13
        end
        object WheelBtnF2ComboBox: TComboBox
          Left = 95
          Top = 381
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 12
        end
        object WheelBtnF1ComboBox: TComboBox
          Left = 95
          Top = 354
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 11
        end
        object X1BtnFComboBox: TComboBox
          Left = 95
          Top = 445
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 14
        end
        object X2BtnFComboBox: TComboBox
          Left = 95
          Top = 472
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 15
        end
        object SelIconCheckBox: TCheckBox
          Left = 275
          Top = 131
          Width = 147
          Height = 17
          Caption = #12450#12452#12467#12531#12391#20491#21029#36984#25246
          TabOrder = 4
        end
      end
      object MouseIvGroupBox: TGroupBox
        Left = 460
        Top = 287
        Width = 434
        Height = 234
        Caption = #12452#12513#12540#12472#12499#12517#12450#12540
        TabOrder = 2
        DesignSize = (
          434
          234)
        object TLabel
          Left = 64
          Top = 50
          Width = 27
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+'
        end
        object TLabel
          Left = 59
          Top = 77
          Width = 32
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Shift+'
        end
        object TLabel
          Left = 32
          Top = 104
          Width = 59
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+Shift+'
        end
        object TLabel
          Left = 18
          Top = 23
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12510#12454#12473#12507#12452#12540#12523
        end
        object TLabel
          Left = 20
          Top = 141
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12507#12452#12540#12523#12508#12479#12531
        end
        object TLabel
          Left = 48
          Top = 175
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X1'#12508#12479#12531
        end
        object TLabel
          Left = 48
          Top = 202
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X2'#12508#12479#12531
        end
        object WheelCmdI0ComboBox: TComboBox
          Left = 96
          Top = 20
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 0
        end
        object WheelCmdI1ComboBox: TComboBox
          Left = 96
          Top = 47
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 1
        end
        object WheelCmdI2ComboBox: TComboBox
          Left = 96
          Top = 74
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 2
        end
        object WheelCmdI3ComboBox: TComboBox
          Left = 96
          Top = 101
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 3
        end
        object WheelBtnIComboBox: TComboBox
          Left = 96
          Top = 138
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 4
        end
        object X1BtnIComboBox: TComboBox
          Left = 96
          Top = 172
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 5
        end
        object X2BtnIComboBox: TComboBox
          Left = 96
          Top = 199
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 6
        end
      end
      object MouseTvGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 267
        Caption = #12486#12461#12473#12488#12499#12517#12450#12540
        TabOrder = 1
        DesignSize = (
          434
          267)
        object TLabel
          Left = 64
          Top = 50
          Width = 27
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+'
        end
        object TLabel
          Left = 59
          Top = 77
          Width = 32
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Shift+'
        end
        object TLabel
          Left = 32
          Top = 104
          Width = 59
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'Ctrl+Shift+'
        end
        object TLabel
          Left = 20
          Top = 23
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12510#12454#12473#12507#12452#12540#12523
        end
        object TLabel
          Left = 20
          Top = 174
          Width = 72
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #12507#12452#12540#12523#12508#12479#12531
        end
        object TLabel
          Left = 48
          Top = 209
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X1'#12508#12479#12531
        end
        object TLabel
          Left = 48
          Top = 236
          Width = 43
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = 'X2'#12508#12479#12531
        end
        object ViewWheelScrLnEdit: TLabeledEdit
          Left = 384
          Top = 132
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 148
          EditLabel.Height = 23
          EditLabel.Caption = #12507#12452#12540#12523#12395#12424#12427#12473#12463#12525#12540#12523#34892#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
        object WheelCmdV0ComboBox: TComboBox
          Left = 96
          Top = 20
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 0
        end
        object WheelCmdV1ComboBox: TComboBox
          Left = 96
          Top = 47
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 1
        end
        object WheelCmdV2ComboBox: TComboBox
          Left = 96
          Top = 74
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 2
        end
        object WheelCmdV3ComboBox: TComboBox
          Left = 96
          Top = 101
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 3
        end
        object WheelBtnVComboBox: TComboBox
          Left = 96
          Top = 171
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 5
        end
        object X1BtnVComboBox: TComboBox
          Left = 96
          Top = 206
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 6
        end
        object X2BtnVComboBox: TComboBox
          Left = 96
          Top = 233
          Width = 328
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 12
          TabOrder = 7
        end
      end
    end
    object DesignSheet: TTabSheet
      HelpContext = 10
      Caption = #12487#12470#12452#12531
      ImageIndex = -1
      object TLabel
        Left = 748
        Top = 551
        Width = 52
        Height = 15
        Alignment = taRightJustify
        Caption = #12487#12470#12452#12531#12398
      end
      object TLabel
        Left = 676
        Top = 425
        Width = 72
        Height = 15
        Alignment = taRightJustify
        Caption = #12473#12463#12525#12540#12523#12496#12540
      end
      object AppDesignBtn: TButton
        Left = 804
        Top = 547
        Width = 80
        Height = 24
        Caption = #36969#29992
        TabOrder = 16
        OnClick = AppDesignBtnClick
      end
      object BgImageGroupBox: TGroupBox
        Left = 10
        Top = 367
        Width = 434
        Height = 204
        Caption = #32972#26223#30011#20687
        TabOrder = 2
        DesignSize = (
          434
          204)
        object TLabel
          Left = 359
          Top = 175
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object BgImgModeComboBox: TComboBox
          Left = 10
          Top = 18
          Width = 195
          Height = 23
          Style = csDropDownList
          TabOrder = 0
        end
        object BgImg1Edit: TLabeledEdit
          Left = 50
          Top = 49
          Width = 335
          Height = 23
          EditLabel.Width = 32
          EditLabel.Height = 23
          EditLabel.Caption = #30011#20687'1'
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object RefBgImg1Btn: TButton
          Left = 388
          Top = 49
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 3
          OnClick = RefBgImgBtnClick
        end
        object BgImgSubModeComboBox: TComboBox
          Left = 209
          Top = 18
          Width = 214
          Height = 23
          Style = csDropDownList
          TabOrder = 1
        end
        object RefBgImg2Btn: TButton
          Tag = 1
          Left = 388
          Top = 78
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 5
          OnClick = RefBgImgBtnClick
        end
        object BgImg2Edit: TLabeledEdit
          Left = 50
          Top = 78
          Width = 335
          Height = 23
          EditLabel.Width = 32
          EditLabel.Height = 23
          EditLabel.Caption = #30011#20687'2'
          LabelPosition = lpLeft
          TabOrder = 4
          Text = ''
        end
        object BgColAlphaEdit: TLabeledEdit
          Left = 99
          Top = 111
          Width = 50
          Height = 23
          Hint = '0'#65374'255'
          Alignment = taRightJustify
          EditLabel.Width = 46
          EditLabel.Height = 23
          EditLabel.Caption = #32972#26223#33394#945
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 6
          Text = ''
          OnChange = ModalAlphaEditChange
        end
        object GrayScaleCheckBox: TCheckBox
          Left = 188
          Top = 113
          Width = 169
          Height = 17
          Caption = #12464#12524#12540#12473#12465#12540#12523#21270'(&S)'
          TabOrder = 7
        end
        object HideScrCheckBox: TCheckBox
          Left = 10
          Top = 174
          Width = 203
          Height = 17
          Caption = #12473#12463#12525#12540#12523#26178#12395#30011#20687#12434#38560#12377
          TabOrder = 10
        end
        object HideTimeEdit: TLabeledEdit
          Left = 309
          Top = 172
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #24489#24112#26178#38291
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 11
          Text = ''
        end
        object TileIfCheckBox: TCheckBox
          Left = 10
          Top = 144
          Width = 289
          Height = 17
          Caption = #30701#36794#12364#25351#23450#12469#12452#12474#20197#19979#12394#12425#24120#12395#20006#12409#12390#34920#31034
          TabOrder = 8
        end
        object TileIfSzEdit: TLabeledEdit
          Left = 309
          Top = 142
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'px'
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 9
          Text = ''
        end
      end
      object DriveInfGroupBox: TGroupBox
        Left = 460
        Top = 70
        Width = 434
        Height = 106
        Caption = #12489#12521#12452#12502#24773#22577#12398#26360#24335
        TabOrder = 4
        DesignSize = (
          434
          106)
        object DrvInfFmtREdit: TLabeledEdit
          Left = 58
          Top = 18
          Width = 366
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 31
          EditLabel.Height = 23
          EditLabel.Caption = #12523#12540#12488
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object DrvInfFmtSEdit: TLabeledEdit
          Left = 58
          Top = 45
          Width = 366
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 39
          EditLabel.Height = 23
          EditLabel.Caption = #36984#25246#26178
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object DrvInfFmtNEdit: TLabeledEdit
          Left = 58
          Top = 72
          Width = 366
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 34
          EditLabel.Height = 23
          EditLabel.Caption = #12381#12398#20182
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
      end
      object FileListGroupBox: TGroupBox
        Left = 10
        Top = 70
        Width = 434
        Height = 285
        Caption = #12501#12449#12452#12523#12522#12473#12488
        TabOrder = 1
        DesignSize = (
          434
          285)
        object TLabel
          Left = 83
          Top = 21
          Width = 13
          Height = 15
          Caption = 'px'
        end
        object TLabel
          Left = 242
          Top = 21
          Width = 13
          Height = 15
          Caption = 'px'
        end
        object TLabel
          Left = 239
          Top = 82
          Width = 39
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #23567#25968#26689
        end
        object TLabel
          Left = 17
          Top = 82
          Width = 57
          Height = 15
          Alignment = taRightJustify
          Caption = #12469#12452#12474#34920#31034
        end
        object TLabel
          Left = 306
          Top = 228
          Width = 13
          Height = 15
          Caption = 'px'
        end
        object TLabel
          Left = 306
          Top = 255
          Width = 13
          Height = 15
          Caption = 'px'
        end
        object ListInterLnEdit: TLabeledEdit
          Left = 41
          Top = 18
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #34892#38291
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
        object CursorWidthEdit: TLabeledEdit
          Left = 200
          Top = 18
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 67
          EditLabel.Height = 23
          EditLabel.Caption = #12459#12540#12477#12523#32218#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 1
          Text = ''
        end
        object DirBraEdit: TLabeledEdit
          Left = 121
          Top = 49
          Width = 40
          Height = 23
          EditLabel.Width = 106
          EditLabel.Height = 23
          EditLabel.Caption = #12487#12451#12524#12463#12488#12522#25324#24359#25991#23383
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object DirKetEdit: TLabeledEdit
          Left = 182
          Top = 49
          Width = 40
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #65374
          LabelPosition = lpLeft
          TabOrder = 4
          Text = ''
        end
        object SizeDecDgtComboBox: TComboBox
          Left = 280
          Top = 79
          Width = 45
          Height = 23
          Style = csDropDownList
          TabOrder = 8
        end
        object TmStampFmtEdit: TLabeledEdit
          Left = 167
          Top = 113
          Width = 158
          Height = 23
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #26085#26178#26360#24335
          LabelPosition = lpLeft
          TabOrder = 11
          Text = ''
        end
        object ShowIconCheckBox: TCheckBox
          Left = 240
          Top = 50
          Width = 94
          Height = 17
          Caption = #12450#12452#12467#12531#34920#31034
          TabOrder = 5
        end
        object HideScrBarCheckBox: TCheckBox
          Left = 188
          Top = 150
          Width = 144
          Height = 17
          Caption = #12473#12463#12525#12540#12523#12496#12540#12434#38560#12377
          TabOrder = 14
        end
        object KeepOnResizeCheckBox: TCheckBox
          Left = 10
          Top = 175
          Width = 87
          Height = 17
          Caption = #27604#29575#12434#32173#25345
          TabOrder = 16
        end
        object KeepCurListWdCheckBox: TCheckBox
          Left = 188
          Top = 175
          Width = 144
          Height = 17
          Caption = #12459#12524#12531#12488#24133#12434#32173#25345
          TabOrder = 18
        end
        object NoSpcFExtCheckBox: TCheckBox
          Left = 188
          Top = 200
          Width = 144
          Height = 17
          Caption = #25313#24373#23376#12434#20998#38626#12375#12394#12356
          TabOrder = 21
        end
        object ListPercentEdit: TLabeledEdit
          Left = 106
          Top = 173
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 10
          EditLabel.Height = 23
          EditLabel.Caption = '%'
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 17
          Text = ''
        end
        object FixListCheckBox: TCheckBox
          Left = 338
          Top = 175
          Width = 90
          Height = 17
          Caption = #24133#12434#22266#23450
          TabOrder = 19
        end
        object ShowHeaderCheckBox: TCheckBox
          Left = 10
          Top = 150
          Width = 92
          Height = 17
          Caption = #12504#12483#12480#12434#34920#31034
          TabOrder = 13
        end
        object ShowByteSizeCheckBox: TCheckBox
          Left = 338
          Top = 81
          Width = 90
          Height = 17
          Caption = #12496#12452#12488#34920#31034
          TabOrder = 9
        end
        object CursorAlphaEdit: TLabeledEdit
          Left = 338
          Top = 18
          Width = 48
          Height = 23
          Hint = '0'#65374'255'
          Alignment = taRightJustify
          EditLabel.Width = 46
          EditLabel.Height = 23
          EditLabel.Caption = #34892#32972#26223#945
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 2
          Text = ''
          OnChange = ModalAlphaEditChange
        end
        object OmitEndOfNameCheckBox: TCheckBox
          Left = 10
          Top = 200
          Width = 174
          Height = 17
          Caption = #38263#12356#21517#21069#12399#26411#23614#12434#30465#30053
          TabOrder = 20
        end
        object SizeFmtComboBox: TComboBox
          Left = 76
          Top = 79
          Width = 146
          Height = 23
          Style = csDropDownList
          TabOrder = 7
        end
        object FindPathWdEdit: TLabeledEdit
          Left = 256
          Top = 225
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #26368#23567#21015#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 24
          Text = ''
        end
        object FindPathColCheckBox: TCheckBox
          Left = 10
          Top = 227
          Width = 193
          Height = 17
          Caption = #32080#26524#12522#12473#12488#12395#12300#22580#25152#12301#21015#12434#34920#31034
          TabOrder = 23
        end
        object FindTagsColCheckBox: TCheckBox
          Left = 10
          Top = 254
          Width = 193
          Height = 17
          Caption = #12479#12464#26908#32034#12391#12399#12300#12479#12464#12301#21015#12434#34920#31034
          TabOrder = 25
        end
        object FindTagsWdEdit: TLabeledEdit
          Left = 256
          Top = 252
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #26368#23567#21015#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 26
          Text = ''
        end
        object FindTagsSortCheckBox: TCheckBox
          Left = 338
          Top = 254
          Width = 90
          Height = 17
          Caption = #20006#12403#12434#12477#12540#12488
          TabOrder = 27
        end
        object ShowSpaceCheckBox: TCheckBox
          Left = 338
          Top = 200
          Width = 90
          Height = 17
          Caption = #31354#30333#34920#31034
          TabOrder = 22
        end
        object ShowDirTypeCheckBox: TCheckBox
          Left = 338
          Top = 50
          Width = 90
          Height = 17
          Caption = 'DIR'#31278#21029
          TabOrder = 6
        end
        object ShowTargetInfCheckBox: TCheckBox
          Left = 338
          Top = 115
          Width = 90
          Height = 17
          Caption = #23455#20307#34920#31034
          TabOrder = 12
        end
        object ShowPopDirBtnCheckBox: TCheckBox
          Left = 338
          Top = 150
          Width = 90
          Height = 17
          Caption = #9660#12508#12479#12531
          TabOrder = 15
        end
        object FExtMaxWidthEdit: TLabeledEdit
          Left = 65
          Top = 113
          Width = 36
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #25313#24373#23376#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 10
          Text = ''
        end
      end
      object FlatInfPanelCheckBox: TCheckBox
        Left = 470
        Top = 424
        Width = 166
        Height = 17
        Caption = #24773#22577#12497#12493#12523#12434#12501#12521#12483#12488#34920#31034
        TabOrder = 11
      end
      object HideTitMenuCheckBox: TCheckBox
        Left = 623
        Top = 399
        Width = 275
        Height = 17
        Caption = #26368#22823#21270#26178#12395#12479#12452#12488#12523#12496#12540#12392#12513#12491#12517#12540#12434#38560#12377
        TabOrder = 10
      end
      object KnobImgEdit: TEdit
        Left = 752
        Top = 422
        Width = 16
        Height = 23
        TabOrder = 13
        Visible = False
      end
      object LayoutGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 50
        Caption = #30011#38754#12524#12452#12450#12454#12488
        TabOrder = 0
        DesignSize = (
          434
          50)
        object LayoutComboBox: TComboBox
          Left = 10
          Top = 18
          Width = 180
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
          OnChange = LayoutChange
        end
        object DivFlistUDCheckBox: TCheckBox
          Left = 204
          Top = 20
          Width = 97
          Height = 17
          Caption = #19978#19979#12395#20998#21106
          TabOrder = 1
        end
        object DivDirInfUDCheckBox: TCheckBox
          Left = 308
          Top = 20
          Width = 120
          Height = 17
          Caption = 'Dir.'#24773#22577#12418#19978#19979
          TabOrder = 2
        end
      end
      object ModalEfctGroupBox: TGroupBox
        Left = 460
        Top = 482
        Width = 434
        Height = 50
        Caption = #12514#12540#12480#12523#34920#31034#21177#26524
        TabOrder = 15
        object ModalScrCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 57
          Height = 17
          Caption = #26377#21177
          TabOrder = 0
        end
        object ModalAlphaEdit: TLabeledEdit
          Left = 133
          Top = 18
          Width = 48
          Height = 23
          Hint = '0'#65374'255'
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #19981#36879#26126#24230
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 1
          Text = ''
          OnChange = ModalAlphaEditChange
        end
        object ModalColorBox: TColorBox
          Left = 211
          Top = 18
          Width = 172
          Height = 22
          Style = [cbStandardColors, cbCustomColor]
          DropDownCount = 18
          TabOrder = 2
          OnChange = ModalColorBoxChange
        end
      end
      object PinMarkEdit: TLabeledEdit
        Left = 518
        Top = 449
        Width = 40
        Height = 23
        EditLabel.Width = 46
        EditLabel.Height = 23
        EditLabel.Caption = #22266#23450#12500#12531
        LabelPosition = lpLeft
        TabOrder = 14
        Text = ''
      end
      object ScrBarStyleComboBox: TComboBox
        Left = 752
        Top = 422
        Width = 132
        Height = 23
        Style = csDropDownList
        TabOrder = 12
        OnClick = ScrBarStyleComboBoxClick
      end
      object ShowFilePropCheckBox: TCheckBox
        Left = 623
        Top = 376
        Width = 162
        Height = 17
        Caption = #12501#12449#12452#12523#24773#22577#12434#34920#31034
        TabOrder = 8
        OnClick = LayoutChange
      end
      object ShowImgPrvCheckBox: TCheckBox
        Left = 470
        Top = 376
        Width = 142
        Height = 17
        Caption = #12503#12524#12499#12517#12540#12434#34920#31034
        TabOrder = 7
        OnClick = LayoutChange
      end
      object ShowMainMenuCheckBox: TCheckBox
        Left = 470
        Top = 399
        Width = 142
        Height = 17
        Caption = #12513#12491#12517#12540#12496#12540#12434#34920#31034
        TabOrder = 9
      end
      object SplitterGroupBox: TGroupBox
        Left = 460
        Top = 279
        Width = 434
        Height = 82
        Caption = #20998#21106#22659#30028#32218
        TabOrder = 6
        DesignSize = (
          434
          82)
        object SpImgEdit: TLabeledEdit
          Left = 29
          Top = 20
          Width = 292
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #27491
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object RefSpImgBtn: TButton
          Tag = 2
          Left = 325
          Top = 20
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefBgImgBtnClick
        end
        object RefSpImg2Btn: TButton
          Tag = 3
          Left = 325
          Top = 50
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefBgImgBtnClick
        end
        object SpImg2Edit: TLabeledEdit
          Left = 29
          Top = 49
          Width = 292
          Height = 23
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #21103
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object SplitWidthEdit: TLabeledEdit
          Left = 365
          Top = 19
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'px'
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 2
          Text = ''
        end
        object SplitWidth2Edit: TLabeledEdit
          Left = 365
          Top = 48
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = 'px'
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 5
          Text = ''
        end
      end
      object SttBarGroupBox: TGroupBox
        Left = 460
        Top = 188
        Width = 434
        Height = 79
        Caption = #12473#12486#12540#12479#12473#12496#12540#12398#26360#24335
        TabOrder = 5
        DesignSize = (
          434
          79)
        object SttBarFmtEdit: TEdit
          Left = 10
          Top = 18
          Width = 414
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
        object ClockFmtEdit: TLabeledEdit
          Left = 79
          Top = 46
          Width = 345
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 59
          EditLabel.Height = 23
          EditLabel.Caption = #26178#35336#12497#12493#12523
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
      end
      object TabGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 50
        Caption = #12479#12502#12496#12540
        TabOrder = 3
        DesignSize = (
          434
          50)
        object TLabel
          Left = 100
          Top = 21
          Width = 13
          Height = 15
          Caption = 'px'
        end
        object TLabel
          Left = 131
          Top = 21
          Width = 39
          Height = 15
          Alignment = taRightJustify
          Caption = #12473#12479#12452#12523
        end
        object TabWidthEdit: TLabeledEdit
          Left = 58
          Top = 18
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 32
          EditLabel.Height = 23
          EditLabel.Caption = #12479#12502#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
        object TabStyleComboBox: TComboBox
          Left = 174
          Top = 18
          Width = 84
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 1
          OnChange = LayoutChange
        end
        object ShowClsTabBtnCheckBox: TCheckBox
          Left = 270
          Top = 20
          Width = 84
          Height = 17
          Caption = #21066#38500#12508#12479#12531
          TabOrder = 2
        end
        object ShowPopTabBtnCheckBox: TCheckBox
          Left = 360
          Top = 20
          Width = 68
          Height = 17
          Caption = #9660#12508#12479#12531
          TabOrder = 3
        end
      end
    end
    object FontColSheet: TTabSheet
      HelpContext = 23
      Caption = #12501#12457#12531#12488#12539#37197#33394
      ImageIndex = -1
      object TLabel
        Left = 470
        Top = 551
        Width = 37
        Height = 15
        Alignment = taRightJustify
        Caption = #37197#33394#12398
      end
      object TLabel
        Left = 719
        Top = 551
        Width = 81
        Height = 15
        Alignment = taRightJustify
        Caption = #12501#12457#12531#12488#12539#37197#33394#12398
      end
      object AllowDkMdCheckBox: TCheckBox
        Left = 470
        Top = 518
        Width = 263
        Height = 17
        Caption = 'Windows '#12480#12540#12463#12514#12540#12489#12434#36969#29992
        TabOrder = 5
      end
      object AppColorBtn: TButton
        Left = 804
        Top = 547
        Width = 80
        Height = 24
        Caption = #36969#29992
        TabOrder = 8
        OnClick = AppColorBtnClick
      end
      object ExpColBtn: TButton
        Left = 604
        Top = 547
        Width = 90
        Height = 24
        Caption = #12456#12463#12473#12509#12540#12488'...'
        TabOrder = 7
        OnClick = ExpColBtnClick
      end
      object ExtColGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 302
        Caption = #25313#24373#23376#21029#37197#33394
        TabOrder = 3
        DesignSize = (
          434
          302)
        object ExtColListBox: TListBox
          Left = 10
          Top = 18
          Width = 413
          Height = 159
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akRight]
          DragMode = dmAutomatic
          ItemHeight = 12
          TabOrder = 0
          OnClick = ExtColListBoxClick
          OnDrawItem = ExtColListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object RefExtColPanel: TPanel
          Left = 341
          Top = 185
          Width = 22
          Height = 22
          Anchors = [akTop, akRight]
          BevelOuter = bvLowered
          TabOrder = 2
          StyleElements = [seBorder]
        end
        object RefExtColBtn: TButton
          Tag = 2
          Left = 388
          Top = 185
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefExtColBtnClick
        end
        object ExtColorEdit: TLabeledEdit
          Left = 57
          Top = 185
          Width = 280
          Height = 23
          EditLabel.Width = 39
          EditLabel.Height = 23
          EditLabel.Caption = #25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object AddColBtn: TButton
          Left = 10
          Top = 215
          Width = 60
          Height = 24
          Action = AddExtColAction
          TabOrder = 5
        end
        object ChgColBtn: TButton
          Left = 74
          Top = 215
          Width = 60
          Height = 24
          Action = ChgExtColAction
          TabOrder = 6
        end
        object DelColBtn: TButton
          Left = 138
          Top = 215
          Width = 60
          Height = 24
          Action = UserModule.DelListItemAction
          TabOrder = 7
        end
        object UpColBtn: TButton
          Left = 210
          Top = 215
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          TabOrder = 8
        end
        object DowColBtn: TButton
          Left = 244
          Top = 215
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          TabOrder = 9
        end
        object PriFExtColCheckBox: TCheckBox
          Left = 200
          Top = 250
          Width = 201
          Height = 17
          Caption = #25313#24373#23376#37096#20998#12399#23646#24615#33394#12424#12426#20778#20808
          TabOrder = 12
        end
        object SpuitPanel3: TPanel
          Left = 365
          Top = 185
          Width = 22
          Height = 22
          BevelOuter = bvLowered
          TabOrder = 3
          StyleElements = [seBorder]
          object SpuitImage3: TImage
            Tag = 2
            Left = 1
            Top = 1
            Width = 20
            Height = 20
            Align = alClient
            Center = True
            Picture.Data = {
              0954506E67496D61676589504E470D0A1A0A0000000D49484452000000140000
              001408060000008D891D0D0000000774494D4507E708170A351F1885EFE80000
              00097048597300000EC300000EC301C76FA8640000000467414D410000B18F0B
              FC6105000000674944415478DAD5D4310EC0200C03C0FAE7FC9C06D1214211D8
              5009F0C4109F92053C3F0777812971259BCBEE0DDF93418F458BED05236CFAE4
              162B50D4A3C082596A0158035BCC4332C86034C86214A8604350C5BAA045C628
              50C186E03720FD423D7039E7832F4C325415354458800000000049454E44AE42
              6082}
            Stretch = True
            Transparent = True
            OnMouseDown = SpuitImageMouseDown
            OnMouseUp = SpuitImageMouseUp
          end
        end
        object ColOnlyFExtCheckBox: TCheckBox
          Left = 10
          Top = 250
          Width = 182
          Height = 17
          Caption = #25313#24373#23376#37096#20998#12398#12415#12395#36969#29992
          TabOrder = 11
        end
        object SymColToCheckBox: TCheckBox
          Left = 10
          Top = 273
          Width = 263
          Height = 17
          Caption = #12501#12449#12452#12523#21517#20027#37096#12395#12471#12531#12508#12522#12483#12463#12522#12531#12463#33394#12434#36969#29992
          TabOrder = 13
        end
        object FindXColEdit: TLabeledEdit
          Left = 341
          Top = 216
          Width = 82
          Height = 23
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #26908#32034
          LabelPosition = lpLeft
          TabOrder = 10
          Text = ''
          OnChange = FindXColEditChange
        end
      end
      object FontGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 50
        Caption = #12501#12457#12531#12488
        TabOrder = 0
        DesignSize = (
          434
          50)
        object FontComboBox: TComboBox
          Left = 10
          Top = 18
          Width = 377
          Height = 22
          Style = csOwnerDrawFixed
          DropDownCount = 20
          TabOrder = 0
          OnDrawItem = FontComboBoxDrawItem
        end
        object RefFontBtn: TButton
          Left = 390
          Top = 18
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefFontBtnClick
        end
      end
      object InpColBtn: TButton
        Left = 511
        Top = 547
        Width = 90
        Height = 24
        Caption = #12452#12531#12509#12540#12488'...'
        TabOrder = 6
        OnClick = InpColBtnClick
      end
      object OptColGroupBox: TGroupBox
        Left = 10
        Top = 72
        Width = 434
        Height = 284
        Caption = #37197#33394
        TabOrder = 1
        DesignSize = (
          434
          284)
        object OptColListBox: TListBox
          Left = 10
          Top = 18
          Width = 348
          Height = 227
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akBottom]
          TabOrder = 0
          OnDrawItem = OptColListBoxDrawItem
        end
        object RefColBtn: TButton
          Left = 388
          Top = 18
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 2
          OnClick = RefColBtnClick
        end
        object SpuitPanel: TPanel
          Left = 365
          Top = 18
          Width = 22
          Height = 22
          BevelOuter = bvLowered
          TabOrder = 1
          StyleElements = [seBorder]
          object SpuitImage: TImage
            Left = 1
            Top = 1
            Width = 20
            Height = 20
            Align = alClient
            Center = True
            Picture.Data = {
              0954506E67496D61676589504E470D0A1A0A0000000D49484452000000140000
              001408060000008D891D0D0000000774494D4507E708170A351F1885EFE80000
              00097048597300000EC300000EC301C76FA8640000000467414D410000B18F0B
              FC6105000000674944415478DAD5D4310EC0200C03C0FAE7FC9C06D1214211D8
              5009F0C4109F92053C3F0777812971259BCBEE0DDF93418F458BED05236CFAE4
              162B50D4A3C082596A0158035BCC4332C86034C86214A8604350C5BAA045C628
              50C186E03720FD423D7039E7832F4C325415354458800000000049454E44AE42
              6082}
            Stretch = True
            Transparent = True
            OnMouseDown = SpuitImageMouseDown
            OnMouseUp = SpuitImageMouseUp
          end
        end
        object DisableBtn: TButton
          Left = 364
          Top = 48
          Width = 60
          Height = 24
          Action = DisableColAction
          TabOrder = 3
        end
        object FindColEdit: TLabeledEdit
          Left = 198
          Top = 251
          Width = 160
          Height = 23
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #26908#32034
          LabelPosition = lpLeft
          TabOrder = 4
          Text = ''
          OnChange = FindColEditChange
          OnKeyDown = FindColEditKeyDown
        end
        object FindUpColBtn: TButton
          Left = 392
          Top = 250
          Width = 26
          Height = 24
          Action = FindColUpAction
          TabOrder = 6
        end
        object FindDownColBtn: TButton
          Left = 364
          Top = 250
          Width = 26
          Height = 24
          Action = FindColDownAction
          TabOrder = 5
        end
      end
      object TagColGroupBox: TGroupBox
        Left = 460
        Top = 324
        Width = 434
        Height = 183
        Caption = #12479#12464#21029#37197#33394
        TabOrder = 4
        DesignSize = (
          434
          183)
        object TagColListBox: TListBox
          Left = 10
          Top = 18
          Width = 348
          Height = 155
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akBottom]
          DoubleBuffered = False
          ParentDoubleBuffered = False
          TabOrder = 0
          OnDrawItem = TagColListBoxDrawItem
        end
        object RefTagColBtn: TButton
          Tag = 3
          Left = 388
          Top = 18
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 2
          OnClick = RefTagColBtnClick
        end
        object SpuitPanel4: TPanel
          Left = 365
          Top = 18
          Width = 22
          Height = 22
          BevelOuter = bvLowered
          TabOrder = 1
          StyleElements = [seBorder]
          object SpuitImage4: TImage
            Tag = 3
            Left = 1
            Top = 1
            Width = 20
            Height = 20
            Align = alClient
            Center = True
            Picture.Data = {
              0954506E67496D61676589504E470D0A1A0A0000000D49484452000000140000
              001408060000008D891D0D0000000774494D4507E708170A351F1885EFE80000
              00097048597300000EC300000EC301C76FA8640000000467414D410000B18F0B
              FC6105000000674944415478DAD5D4310EC0200C03C0FAE7FC9C06D1214211D8
              5009F0C4109F92053C3F0777812971259BCBEE0DDF93418F458BED05236CFAE4
              162B50D4A3C082596A0158035BCC4332C86034C86214A8604350C5BAA045C628
              50C186E03720FD423D7039E7832F4C325415354458800000000049454E44AE42
              6082}
            Stretch = True
            Transparent = True
            OnMouseDown = SpuitImageMouseDown
            OnMouseUp = SpuitImageMouseUp
          end
        end
        object DisableTagBtn: TButton
          Left = 364
          Top = 48
          Width = 60
          Height = 24
          Action = DefTagColAction
          TabOrder = 3
        end
        object RevTagColCheckBox: TCheckBox
          Left = 366
          Top = 83
          Width = 58
          Height = 17
          Caption = #21453#36578
          TabOrder = 4
          OnClick = RevTagColCheckBoxClick
        end
      end
      object TimColGroupBox: TGroupBox
        Left = 10
        Top = 368
        Width = 434
        Height = 203
        Caption = #12479#12452#12512#12473#12479#12531#12503'/'#12469#12452#12474#12398#37197#33394
        TabOrder = 2
        DesignSize = (
          434
          203)
        object TimColListBox: TListBox
          Left = 10
          Top = 18
          Width = 348
          Height = 175
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akBottom]
          TabOrder = 0
          OnDrawItem = OptColListBoxDrawItem
        end
        object RefTimColBtn: TButton
          Tag = 1
          Left = 388
          Top = 18
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 2
          OnClick = RefColBtnClick
        end
        object SpuitPanel2: TPanel
          Left = 365
          Top = 18
          Width = 22
          Height = 22
          BevelOuter = bvLowered
          TabOrder = 1
          StyleElements = [seBorder]
          object SpuitImage2: TImage
            Tag = 1
            Left = 1
            Top = 1
            Width = 20
            Height = 20
            Align = alClient
            Center = True
            Picture.Data = {
              0954506E67496D61676589504E470D0A1A0A0000000D49484452000000140000
              001408060000008D891D0D0000000774494D4507E708170A351F1885EFE80000
              00097048597300000EC300000EC301C76FA8640000000467414D410000B18F0B
              FC6105000000674944415478DAD5D4310EC0200C03C0FAE7FC9C06D1214211D8
              5009F0C4109F92053C3F0777812971259BCBEE0DDF93418F458BED05236CFAE4
              162B50D4A3C082596A0158035BCC4332C86034C86214A8604350C5BAA045C628
              50C186E03720FD423D7039E7832F4C325415354458800000000049454E44AE42
              6082}
            Stretch = True
            Transparent = True
            OnMouseDown = SpuitImageMouseDown
            OnMouseUp = SpuitImageMouseUp
          end
        end
        object DisableTimBtn: TButton
          Left = 364
          Top = 48
          Width = 60
          Height = 24
          Action = DisableTimColAction
          TabOrder = 3
        end
      end
    end
    object TxtViewerSheet: TTabSheet
      HelpContext = 11
      Caption = #12486#12461#12473#12488#12499#12517#12450#12540
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object TLabel
        Left = 90
        Top = 147
        Width = 13
        Height = 15
        Caption = 'px'
      end
      object TLabel
        Left = 651
        Top = 403
        Width = 14
        Height = 15
        Caption = 'KB'
      end
      object TLabel
        Left = 651
        Top = 436
        Width = 18
        Height = 15
        Caption = 'MB'
      end
      object TLabel
        Left = 220
        Top = 147
        Width = 13
        Height = 15
        Caption = 'px'
      end
      object AltBackSlashCheckBox: TCheckBox
        Left = 470
        Top = 340
        Width = 205
        Height = 17
        Caption = '"\" '#12434' "'#8726'"(U+2216)'#12391#34920#31034
        TabOrder = 24
      end
      object AozoraCheckBox: TCheckBox
        Left = 684
        Top = 315
        Width = 209
        Height = 17
        Caption = #38738#31354#25991#24235#24418#24335#12395#23550#24540#12377#12427
        TabOrder = 23
      end
      object BinMemMapedCheckBox: TCheckBox
        Left = 684
        Top = 435
        Width = 203
        Height = 17
        Caption = #12513#12514#12522#12510#12483#12503#12489#12501#12449#12452#12523#12392#12375#12390#38283#12367
        TabOrder = 29
      end
      object CellAlphaEdit: TLabeledEdit
        Left = 265
        Top = 279
        Width = 48
        Height = 23
        Hint = '0'#65374'255'
        Alignment = taRightJustify
        EditLabel.Width = 29
        EditLabel.Height = 23
        EditLabel.Caption = #12475#12523#945
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 15
        Text = ''
        OnChange = ModalAlphaEditChange
      end
      object ClickableUrlCheckBox: TCheckBox
        Left = 265
        Top = 365
        Width = 180
        Height = 17
        Caption = #12480#12502#12523#12463#12522#12483#12463#12391'URL'#12434#38283#12367
        TabOrder = 19
      end
      object ColorHintCheckBox: TCheckBox
        Left = 20
        Top = 365
        Width = 237
        Height = 17
        Caption = #12459#12540#12477#12523#20301#32622#25968#20516#12398#12459#12521#12540#34920#31034
        TabOrder = 18
      end
      object DecDfmStrCheckBox: TCheckBox
        Left = 684
        Top = 341
        Width = 209
        Height = 17
        Caption = '.dfm '#12501#12449#12452#12523#12398#25991#23383#21015#20516#12434#12487#12467#12540#12489
        TabOrder = 25
      end
      object EmpGroupBox: TGroupBox
        Left = 10
        Top = 409
        Width = 429
        Height = 162
        Anchors = [akLeft, akTop, akRight]
        Caption = #24375#35519#34920#31034
        TabOrder = 20
        object EmRemCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 72
          Height = 17
          Caption = #12467#12513#12531#12488
          TabOrder = 0
        end
        object EmRsvCheckBox: TCheckBox
          Left = 89
          Top = 20
          Width = 72
          Height = 17
          Caption = #20104#32004#35486
          TabOrder = 1
        end
        object EmStrCheckBox: TCheckBox
          Left = 255
          Top = 20
          Width = 76
          Height = 17
          Caption = #25991#23383#21015
          TabOrder = 3
        end
        object EmHdlnCheckBox: TCheckBox
          Left = 10
          Top = 45
          Width = 72
          Height = 17
          Caption = #35211#20986#12375
          TabOrder = 5
        end
        object EmRubyCheckBox: TCheckBox
          Left = 89
          Top = 45
          Width = 60
          Height = 17
          Caption = #12523#12499
          TabOrder = 6
        end
        object EmpBin1Edit: TLabeledEdit
          Left = 89
          Top = 75
          Width = 330
          Height = 21
          Hint = #27491#35215#34920#29694#21487
          AutoSize = False
          EditLabel.Width = 70
          EditLabel.Height = 21
          EditLabel.Caption = #12496#12452#12490#12522#24375#35519'1'
          LabelPosition = lpLeft
          TabOrder = 7
          Text = ''
        end
        object EmpBin2Edit: TLabeledEdit
          Left = 89
          Top = 102
          Width = 330
          Height = 21
          Hint = #27491#35215#34920#29694#21487
          AutoSize = False
          EditLabel.Width = 6
          EditLabel.Height = 21
          EditLabel.Caption = '2'
          LabelPosition = lpLeft
          TabOrder = 8
          Text = ''
        end
        object EmSymCheckBox: TCheckBox
          Left = 169
          Top = 20
          Width = 78
          Height = 17
          Caption = #12471#12531#12508#12523
          TabOrder = 2
        end
        object EmNumCheckBox: TCheckBox
          Left = 339
          Top = 20
          Width = 64
          Height = 17
          Caption = #25968#20516
          TabOrder = 4
        end
        object EmpBin3Edit: TLabeledEdit
          Left = 89
          Top = 129
          Width = 330
          Height = 21
          Hint = #27491#35215#34920#29694#21487
          AutoSize = False
          EditLabel.Width = 6
          EditLabel.Height = 21
          EditLabel.Caption = '3'
          LabelPosition = lpLeft
          TabOrder = 9
          Text = ''
        end
      end
      object ExtTxViewerGroupBox: TGroupBox
        Left = 460
        Top = 495
        Width = 429
        Height = 76
        Anchors = [akLeft, akTop, akRight]
        Caption = #22806#37096#12499#12517#12450#12540
        TabOrder = 30
        object ExtTxViewerEdit: TEdit
          Left = 10
          Top = 18
          Width = 378
          Height = 23
          TabOrder = 0
        end
        object RefExtTxViewerBtn: TButton
          Tag = 4
          Left = 391
          Top = 17
          Width = 36
          Height = 22
          Caption = '...'
          TabOrder = 1
          OnClick = RefEditorBtnClick
        end
        object ExtTxViewerFmtEdit: TLabeledEdit
          Left = 189
          Top = 44
          Width = 199
          Height = 23
          Hint = '$F '#12501#12449#12452#12523#21517'/  $L '#34892#30058#21495
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12521#12513#12540#12479
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
      end
      object FixedLimitEdit: TLabeledEdit
        Left = 142
        Top = 279
        Width = 60
        Height = 23
        Hint = '0 '#12391#28961#21046#38480
        Alignment = taRightJustify
        EditLabel.Width = 115
        EditLabel.Height = 23
        EditLabel.Caption = #22266#23450#38263#34920#31034#12398#21046#38480#24133
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 14
        Text = ''
      end
      object FoldGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 433
        Height = 46
        Caption = #25240#12426#36820#12375#24133
        TabOrder = 0
        object FitFoldRadioBtn: TRadioButton
          Left = 16
          Top = 18
          Width = 196
          Height = 17
          Caption = #12454#12451#12531#12489#12454#12398#24133#12395#21512#12431#12379#12427
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object SetFoldRadioBtn: TRadioButton
          Left = 219
          Top = 18
          Width = 127
          Height = 17
          Caption = #25351#23450'('#21322#35282#21336#20301')'
          TabOrder = 1
        end
        object FoldWdEdit: TEdit
          Left = 352
          Top = 16
          Width = 65
          Height = 23
          Alignment = taRightJustify
          NumbersOnly = True
          TabOrder = 2
        end
      end
      object H2TGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 429
        Height = 283
        Anchors = [akLeft, akTop, akRight]
        Caption = 'HTML'#8594#12486#12461#12473#12488#22793#25563
        TabOrder = 21
        object InsHrClsEdit: TLabeledEdit
          Left = 10
          Top = 190
          Width = 410
          Height = 23
          EditLabel.Width = 177
          EditLabel.Height = 15
          EditLabel.Caption = '<div>'#12398#21069#12395#32619#32218#12434#25407#20837#12377#12427#12463#12521#12473
          TabOrder = 3
          Text = ''
        end
        object HdrStrEdit: TLabeledEdit
          Left = 10
          Top = 36
          Width = 160
          Height = 23
          Hint = '; '#12391#21306#20999#12387#12390'6'#20491#12414#12391#25351#23450#21487#33021
          EditLabel.Width = 112
          EditLabel.Height = 15
          EditLabel.Caption = #35211#20986#12375#25991#23383'(H1'#65374'H6)'
          TabOrder = 0
          Text = ''
        end
        object InsHrSctCheckBox: TCheckBox
          Left = 10
          Top = 221
          Width = 100
          Height = 17
          Caption = '<section> /'
          TabOrder = 4
        end
        object InsHrArtCheckBox: TCheckBox
          Left = 114
          Top = 221
          Width = 100
          Height = 17
          Caption = '<article> /'
          TabOrder = 5
        end
        object InsHrNavCheckBox: TCheckBox
          Left = 218
          Top = 221
          Width = 171
          Height = 17
          Caption = '<nav>'#12398#21069#12395#32619#32218#25407#20837
          TabOrder = 6
        end
        object MarkdownCheckBox: TCheckBox
          Left = 10
          Top = 254
          Width = 169
          Height = 17
          Caption = 'Markdown'#35352#27861#12395#22793#25563
          TabOrder = 7
        end
        object DelBlkClsEdit: TLabeledEdit
          Left = 10
          Top = 88
          Width = 410
          Height = 23
          EditLabel.Width = 148
          EditLabel.Height = 15
          EditLabel.Caption = #21066#38500#12377#12427#12502#12525#12483#12463#35201#32032#12398#12463#12521#12473
          TabOrder = 1
          Text = ''
        end
        object DelBlkIdEdit: TLabeledEdit
          Left = 10
          Top = 135
          Width = 410
          Height = 23
          EditLabel.Width = 131
          EditLabel.Height = 15
          EditLabel.Caption = #21066#38500#12377#12427#12502#12525#12483#12463#35201#32032#12398'ID'
          TabOrder = 2
          Text = ''
        end
      end
      object LeftMarginEdit: TLabeledEdit
        Left = 178
        Top = 144
        Width = 40
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 52
        EditLabel.Height = 23
        EditLabel.Caption = #24038#20596#20313#30333
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 8
        Text = ''
      end
      object LimitBinCsrCheckBox: TCheckBox
        Left = 20
        Top = 340
        Width = 327
        Height = 17
        Caption = #12496#12452#12490#12522#34920#31034#12391#12459#12540#12477#12523#31227#21205#12434'16'#36914#12480#12531#12503#20869#12395#21046#38480
        TabOrder = 17
      end
      object LimitBinEdit: TLabeledEdit
        Left = 589
        Top = 433
        Width = 60
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 121
        EditLabel.Height = 23
        EditLabel.Caption = #12496#12452#12490#12522#26368#22823#35501#36796#12469#12452#12474
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 28
        Text = ''
      end
      object LimitTxtEdit: TLabeledEdit
        Left = 589
        Top = 400
        Width = 60
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 123
        EditLabel.Height = 23
        EditLabel.Caption = #12486#12461#12473#12488#26368#22823#35501#36796#12469#12452#12474
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 27
        Text = ''
      end
      object RstViewLnCheckBox: TCheckBox
        Left = 281
        Top = 91
        Width = 112
        Height = 17
        Caption = #34892#20301#32622#12434#24489#20803
        TabOrder = 5
      end
      object ScrBarFoldCheckBox: TCheckBox
        Left = 20
        Top = 116
        Width = 277
        Height = 17
        Caption = #12473#12463#12525#12540#12523#12496#12540#12434#25240#12426#36820#12375#20301#32622#12395#34920#31034
        TabOrder = 6
      end
      object ShowCrCheckBox: TCheckBox
        Left = 355
        Top = 146
        Width = 90
        Height = 17
        Caption = #25913#34892#12434#34920#31034
        TabOrder = 10
      end
      object ShowLnCsrCheckBox: TCheckBox
        Left = 141
        Top = 91
        Width = 124
        Height = 17
        Caption = #34892#12459#12540#12477#12523#12434#34920#31034
        TabOrder = 4
      end
      object ShowLnNoCheckBox: TCheckBox
        Left = 20
        Top = 91
        Width = 109
        Height = 17
        Caption = #34892#30058#21495#12434#34920#31034
        TabOrder = 3
      end
      object ShowRulerCheckBox: TCheckBox
        Left = 20
        Top = 66
        Width = 109
        Height = 17
        Caption = #12523#12540#12521#12434#34920#31034
        TabOrder = 1
      end
      object ShowTabCheckBox: TCheckBox
        Left = 257
        Top = 146
        Width = 90
        Height = 17
        Caption = #12479#12502#12434#34920#31034
        TabOrder = 9
      end
      object TabXWdEdit: TLabeledEdit
        Left = 16
        Top = 244
        Width = 54
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 56
        EditLabel.Height = 15
        EditLabel.Caption = #12479#12502#12473#12488#12483#12503
        NumbersOnly = True
        TabOrder = 12
        Text = ''
      end
      object TextInterLnEdit: TLabeledEdit
        Left = 48
        Top = 144
        Width = 40
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 26
        EditLabel.Height = 23
        EditLabel.Caption = #34892#38291
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 7
        Text = ''
      end
      object TvCsrVisiCheckBox: TCheckBox
        Left = 20
        Top = 315
        Width = 198
        Height = 17
        Caption = #12459#12540#12477#12523#12434#24120#12395#21487#35222#38936#22495#12395
        TabOrder = 16
      end
      object TxtSttIsBtmCheckBox: TCheckBox
        Left = 141
        Top = 66
        Width = 167
        Height = 17
        Caption = #24773#22577#12504#12483#12480#12434#19979#37096#12395#34920#31034
        TabOrder = 2
      end
      object UseXd2txCheckBox: TCheckBox
        Left = 470
        Top = 315
        Width = 205
        Height = 17
        Caption = 'xd2txlib.dll '#12391#12486#12461#12473#12488#34920#31034
        TabOrder = 22
      end
      object ViewTab4Edit: TLabeledEdit
        Left = 16
        Top = 197
        Width = 413
        Height = 23
        EditLabel.Width = 112
        EditLabel.Height = 15
        EditLabel.Caption = #12479#12502#12473#12488#12483#12503'4'#12398#25313#24373#23376
        TabOrder = 11
        Text = ''
      end
      object ViewTabXEdit: TLabeledEdit
        Left = 74
        Top = 244
        Width = 355
        Height = 23
        EditLabel.Width = 39
        EditLabel.Height = 15
        EditLabel.Caption = #25313#24373#23376
        TabOrder = 13
        Text = ''
      end
      object FormatJsonCheckBox: TCheckBox
        Left = 470
        Top = 365
        Width = 205
        Height = 17
        Caption = '.json '#12501#12449#12452#12523#12434#25972#24418#12375#12390#34920#31034
        TabOrder = 26
      end
    end
    object ImgViewerSheet: TTabSheet
      HelpContext = 20
      Caption = #12452#12513#12540#12472#12499#12517#12450#12540
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object TLabel
        Left = 391
        Top = 53
        Width = 10
        Height = 15
        Caption = '%'
      end
      object TLabel
        Left = 199
        Top = 83
        Width = 13
        Height = 15
        Caption = 'px'
      end
      object TLabel
        Left = 7
        Top = 19
        Width = 120
        Height = 15
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        Caption = #32302#23567#12539#25313#22823#12450#12523#12468#12522#12474#12512
        ExplicitLeft = 23
      end
      object TLabel
        Left = 199
        Top = 53
        Width = 13
        Height = 15
        Caption = 'px'
      end
      object TLabel
        Left = 199
        Top = 115
        Width = 24
        Height = 15
        Caption = 'm'#31186
      end
      object AnimateGifCheckBox: TCheckBox
        Left = 12
        Top = 277
        Width = 223
        Height = 17
        Caption = #12450#12491#12513#12540#12471#12519#12531'GIF'#12434#20877#29983#12377#12427
        TabOrder = 18
      end
      object BeepTopEndCheckBox: TCheckBox
        Left = 380
        Top = 177
        Width = 69
        Height = 17
        Caption = #35686#21578#38899
        TabOrder = 11
      end
      object FExtNoIViewEdit: TLabeledEdit
        Left = 470
        Top = 181
        Width = 414
        Height = 23
        EditLabel.Width = 144
        EditLabel.Height = 15
        EditLabel.Caption = #34920#31034#12375#12394#12356#12501#12449#12452#12523#12398#25313#24373#23376
        TabOrder = 22
        Text = ''
      end
      object FitMaxZoomEdit: TLabeledEdit
        Left = 339
        Top = 50
        Width = 50
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 85
        EditLabel.Height = 23
        EditLabel.Caption = #35377#23481#12501#12451#12483#12488#20493#29575
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 2
        Text = ''
      end
      object FrmMgnEdit: TLabeledEdit
        Left = 147
        Top = 50
        Width = 50
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 109
        EditLabel.Height = 23
        EditLabel.Caption = #12501#12451#12483#12488#34920#31034#12398#20313#30333#24133
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 1
        Text = ''
      end
      object HideCsrFullCheckBox: TCheckBox
        Left = 223
        Top = 252
        Width = 228
        Height = 17
        Caption = #20840#30011#38754#34920#31034#12391#12510#12454#12473#12509#12452#12531#12479#12434#38560#12377
        TabOrder = 17
      end
      object HideThumbCheckBox: TCheckBox
        Left = 12
        Top = 252
        Width = 203
        Height = 17
        Caption = #20840#30011#38754#34920#31034#12391#12469#12512#12493#12452#12523#12434#38560#12377
        TabOrder = 16
      end
      object HintTopEndCheckBox: TCheckBox
        Left = 207
        Top = 177
        Width = 167
        Height = 17
        Caption = #20808#38957#12539#26368#24460#12391#12498#12531#12488#34920#31034
        TabOrder = 10
      end
      object ImgGridHEdit: TLabeledEdit
        Left = 339
        Top = 80
        Width = 30
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 77
        EditLabel.Height = 23
        EditLabel.Caption = #20998#21106#12464#12522#12483#12489' '#27178
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 4
        Text = ''
      end
      object ImgGridVEdit: TLabeledEdit
        Left = 399
        Top = 80
        Width = 30
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 13
        EditLabel.Height = 23
        EditLabel.Caption = #32294
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 5
        Text = ''
      end
      object ImgSttIsBtmCheckBox: TCheckBox
        Left = 12
        Top = 202
        Width = 173
        Height = 17
        Caption = #24773#22577#12504#12483#12480#12434#19979#37096#12395#34920#31034
        TabOrder = 12
      end
      object IniModeIRadioGroup: TRadioGroup
        Left = 10
        Top = 312
        Width = 434
        Height = 46
        Caption = #38283#12356#12383#26178#12398#21021#26399#29366#24907
        Columns = 3
        Items.Strings = (
          #12501#12451#12483#12488#34920#31034
          #31561#20493#34920#31034
          #21069#22238#12398#20493#29575)
        TabOrder = 19
      end
      object IntrPgEdit: TLabeledEdit
        Left = 147
        Top = 80
        Width = 50
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 62
        EditLabel.Height = 23
        EditLabel.Caption = #35211#38283#12365#38291#38548
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 3
        Text = ''
      end
      object KeepZoomCheckBox: TCheckBox
        Left = 207
        Top = 154
        Width = 210
        Height = 17
        Caption = #12474#12540#12512#20493#29575#12434#32173#25345#12377#12427
        TabOrder = 8
      end
      object LoopCurCheckBox: TCheckBox
        Left = 12
        Top = 177
        Width = 173
        Height = 17
        Caption = #12459#12540#12477#12523#12434#12523#12540#12503#31227#21205#12377#12427
        TabOrder = 9
      end
      object MarkImgGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 433
        Height = 139
        Caption = #38281#12376#12383#26178#12395#26654#12510#12540#12463#12434#35373#23450
        TabOrder = 21
        object MarkImgCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 57
          Height = 17
          Caption = #26377#21177
          TabOrder = 0
        end
        object MarkFextEdit: TLabeledEdit
          Left = 10
          Top = 107
          Width = 202
          Height = 23
          EditLabel.Width = 86
          EditLabel.Height = 15
          EditLabel.Caption = #36969#29992#12377#12427#25313#24373#23376
          TabOrder = 2
          Text = ''
        end
        object MarkMemoEdit: TLabeledEdit
          Left = 222
          Top = 107
          Width = 202
          Height = 23
          EditLabel.Width = 19
          EditLabel.Height = 15
          EditLabel.Caption = #12513#12514
          TabOrder = 3
          Text = ''
        end
        object MarkPathEdit: TLabeledEdit
          Left = 10
          Top = 60
          Width = 414
          Height = 23
          EditLabel.Width = 161
          EditLabel.Height = 15
          EditLabel.Caption = #36969#29992#12377#12427#12487#12451#12524#12463#12488#12522'('#21069#26041#19968#33268')'
          TabOrder = 1
          Text = ''
        end
      end
      object MinShowTimeEdit: TLabeledEdit
        Left = 147
        Top = 112
        Width = 50
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 81
        EditLabel.Height = 23
        EditLabel.Caption = #26368#23567#34920#31034#12479#12452#12512
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 6
        Text = ''
      end
      object RotViewImgCheckBox: TCheckBox
        Left = 12
        Top = 152
        Width = 173
        Height = 17
        Caption = 'Exif'#24773#22577#12395#12375#12383#12364#12387#12390#22238#36578
        TabOrder = 7
      end
      object ScaleOptComboBox: TComboBox
        Left = 147
        Top = 16
        Width = 170
        Height = 23
        Style = csDropDownList
        TabOrder = 0
      end
      object SeekBindDirCheckBox: TCheckBox
        Left = 12
        Top = 227
        Width = 234
        Height = 17
        Caption = #12471#12540#12463#12496#12540#12398#22987#28857#12434#32180#12376#26041#21521#12395#21512#12431#12377
        TabOrder = 14
      end
      object SeekSwapNPCheckBox: TCheckBox
        Left = 254
        Top = 227
        Width = 197
        Height = 17
        Caption = #21491#32180#12376#12391'Next/Prev'#20837#26367
        TabOrder = 15
      end
      object ThumbGroupBox: TGroupBox
        Left = 10
        Top = 370
        Width = 434
        Height = 201
        Caption = #12469#12512#12493#12452#12523
        TabOrder = 20
        DesignSize = (
          434
          201)
        object TLabel
          Left = 107
          Top = 23
          Width = 26
          Height = 15
          Alignment = taRightJustify
          Caption = #32302#23567
        end
        object ThumbScrCheckBox: TCheckBox
          Left = 10
          Top = 51
          Width = 179
          Height = 17
          Caption = #12473#12463#12525#12540#12523#12496#12540#12434#34920#31034
          TabOrder = 3
        end
        object ThumbSizeEdit: TLabeledEdit
          Left = 48
          Top = 20
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 31
          EditLabel.Height = 23
          EditLabel.Caption = #12469#12452#12474
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
        object ThumbBdrEdit: TLabeledEdit
          Left = 373
          Top = 20
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #22659#30028#32218#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 2
          Text = ''
        end
        object ThumbFExtCheckBox: TCheckBox
          Left = 290
          Top = 75
          Width = 108
          Height = 17
          Caption = #25313#24373#23376#24375#35519
          TabOrder = 7
        end
        object ThumbExifCheckBox: TCheckBox
          Left = 104
          Top = 75
          Width = 85
          Height = 17
          Caption = 'Exif'#24773#22577
          TabOrder = 5
        end
        object ThumbNameCheckBox: TCheckBox
          Left = 10
          Top = 75
          Width = 84
          Height = 17
          Caption = #12501#12449#12452#12523#21517
          TabOrder = 4
        end
        object NotThumbIfArcCheckBox: TCheckBox
          Left = 10
          Top = 99
          Width = 223
          Height = 17
          Caption = #12450#12540#12459#12452#12502#20869#12391#12399#19968#25324#21462#24471#12375#12394#12356
          TabOrder = 8
        end
        object ThumbTagCheckBox: TCheckBox
          Left = 197
          Top = 75
          Width = 85
          Height = 17
          Caption = #12479#12464
          TabOrder = 6
        end
        object CacheThumbADSCheckBox: TCheckBox
          Left = 10
          Top = 130
          Width = 223
          Height = 17
          Caption = #20195#26367#12487#12540#12479#12473#12488#12522#12540#12512#12395#12461#12515#12483#12471#12517
          TabOrder = 9
        end
        object NoCachePathEdit: TLabeledEdit
          Left = 26
          Top = 168
          Width = 397
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 160
          EditLabel.Height = 15
          EditLabel.Caption = #12461#12515#12483#12471#12517#12375#12394#12356#12497#12473'('#21069#26041#19968#33268')'
          TabOrder = 10
          Text = ''
        end
        object ThumbOptComboBox: TComboBox
          Left = 137
          Top = 20
          Width = 170
          Height = 23
          Style = csDropDownList
          TabOrder = 1
        end
      end
      object ToolBarISideCheckBox: TCheckBox
        Left = 207
        Top = 202
        Width = 210
        Height = 17
        Caption = #12484#12540#12523#12496#12540#12434#12469#12452#12489#12496#12540#19978#12395#34920#31034
        TabOrder = 13
      end
    end
    object EditorSheet: TTabSheet
      HelpContext = 12
      Caption = #12456#12487#12451#12479
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object BinEditGroupBox: TGroupBox
        Left = 10
        Top = 186
        Width = 879
        Height = 50
        Anchors = [akLeft, akTop, akRight]
        Caption = #12496#12452#12490#12522#12456#12487#12451#12479
        TabOrder = 2
        DesignSize = (
          879
          50)
        object RefBinEdtBtn: TButton
          Tag = 2
          Left = 833
          Top = 20
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefEditorBtnClick
        end
        object BinaryEditorEdit: TEdit
          Left = 10
          Top = 20
          Width = 820
          Height = 23
          TabOrder = 0
        end
      end
      object EtcEditGroupBox: TGroupBox
        Left = 10
        Top = 248
        Width = 879
        Height = 322
        Anchors = [akLeft, akTop, akRight]
        Caption = #12381#12398#20182#12398#12456#12487#12451#12479
        TabOrder = 3
        DesignSize = (
          879
          322)
        object RefEtcEdt1Btn: TButton
          Tag = 3
          Left = 835
          Top = 257
          Width = 36
          Height = 22
          Anchors = [akRight, akBottom]
          Caption = '...'
          TabOrder = 3
          OnClick = RefEditorBtnClick
        end
        object FExtEtcEdit: TLabeledEdit
          Left = 90
          Top = 257
          Width = 120
          Height = 23
          Anchors = [akLeft, akBottom]
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #23550#35937#25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object EtcEditorEdit: TLabeledEdit
          Left = 276
          Top = 257
          Width = 557
          Height = 23
          Anchors = [akLeft, akRight, akBottom]
          EditLabel.Width = 37
          EditLabel.Height = 23
          EditLabel.Caption = #12456#12487#12451#12479
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object EtcEditorListBox: TListBox
          Left = 10
          Top = 18
          Width = 858
          Height = 231
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akRight, akBottom]
          DragMode = dmAutomatic
          ItemHeight = 12
          TabOrder = 0
          OnClick = EtcEditorListBoxClick
          OnDrawItem = EtcEditorListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object AddEdtBtn: TButton
          Left = 26
          Top = 289
          Width = 70
          Height = 24
          Action = AddEtcEditorAction
          Anchors = [akLeft, akBottom]
          TabOrder = 4
        end
        object ChgEdtBtn: TButton
          Left = 100
          Top = 289
          Width = 70
          Height = 24
          Action = ChgEtcEditorAction
          Anchors = [akLeft, akBottom]
          TabOrder = 5
        end
        object DelEdtBtn: TButton
          Left = 174
          Top = 289
          Width = 70
          Height = 24
          Action = UserModule.DelListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 6
        end
        object DowEdtBtn: TButton
          Left = 290
          Top = 289
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 8
        end
        object UpEdtBtn: TButton
          Left = 256
          Top = 289
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 7
        end
      end
      object ImgEditGroupBox: TGroupBox
        Left = 10
        Top = 98
        Width = 879
        Height = 80
        Anchors = [akLeft, akTop, akRight]
        Caption = #12452#12513#12540#12472#12456#12487#12451#12479
        TabOrder = 1
        DesignSize = (
          879
          80)
        object RefImgEdtBtn: TButton
          Tag = 1
          Left = 833
          Top = 18
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefEditorBtnClick
        end
        object FExtImgEdit: TLabeledEdit
          Left = 95
          Top = 49
          Width = 574
          Height = 23
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #23550#35937#25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object ImageEditorEdit: TEdit
          Left = 10
          Top = 18
          Width = 820
          Height = 23
          TabOrder = 0
        end
        object ImgEditSglCheckBox: TCheckBox
          Left = 683
          Top = 51
          Width = 183
          Height = 17
          Caption = #12501#12449#12452#12523#12372#12392#12395#20491#21029#36215#21205
          TabOrder = 3
        end
      end
      object TxtEditGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 879
        Height = 80
        Anchors = [akLeft, akTop, akRight]
        Caption = #12486#12461#12473#12488#12456#12487#12451#12479
        TabOrder = 0
        DesignSize = (
          879
          80)
        object TextEditorEdit: TEdit
          Left = 10
          Top = 20
          Width = 820
          Height = 23
          TabOrder = 0
        end
        object RefTxtEditBtn: TButton
          Left = 833
          Top = 20
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefEditorBtnClick
        end
        object TextEditorFrmtEdit: TLabeledEdit
          Left = 276
          Top = 49
          Width = 226
          Height = 23
          Hint = '$F '#12501#12449#12452#12523#21517'/  $L '#34892#30058#21495
          EditLabel.Width = 232
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12521#12513#12540#12479' (GREP'#12420#12499#12517#12450#12540#12391#12398#12479#12464#12472#12515#12531#12503#29992')'
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object TextEditorFrmt2Edit: TLabeledEdit
          Left = 610
          Top = 49
          Width = 220
          Height = 23
          Hint = '$F '#12501#12449#12452#12523#21517'('#35079#25968#26178#12399#12381#12398#20006#12403')'
          EditLabel.Width = 67
          EditLabel.Height = 23
          EditLabel.Caption = '('#12501#12449#12452#12521#12540#29992')'
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
      end
    end
    object KeySetSheet: TTabSheet
      HelpContext = 16
      Caption = #12461#12540#35373#23450
      ImageIndex = -1
      object KeyTabControl: TTabControl
        Left = 0
        Top = 0
        Width = 906
        Height = 450
        Align = alClient
        OwnerDraw = True
        TabOrder = 0
        TabPosition = tpBottom
        Tabs.Strings = (
          #12501#12449#12452#12521#12540
          'INC.'#12469#12540#12481
          #12486#12461#12473#12488#12499#12517#12450#12540
          #12452#12513#12540#12472#12499#12517#12450#12540
          #12525#12464#12454#12451#12531#12489#12454)
        TabIndex = 0
        TabWidth = 120
        StyleElements = [seFont, seBorder]
        OnChange = KeyTabControlChange
        OnChanging = KeyTabControlChanging
        OnDrawTab = KeyTabControlDrawTab
        object KeyListBox: TListBox
          Left = 4
          Top = 21
          Width = 898
          Height = 401
          Style = lbOwnerDrawFixed
          Align = alClient
          ItemHeight = 13
          TabOrder = 1
          OnClick = KeyListBoxClick
          OnDblClick = KeyListBoxDblClick
          OnDrawItem = KeyListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object KeyHeaderControl: THeaderControl
          Left = 4
          Top = 4
          Width = 898
          Height = 17
          Sections = <
            item
              ImageIndex = -1
              Style = hsOwnerDraw
              Width = 50
            end
            item
              ImageIndex = -1
              Style = hsOwnerDraw
              Width = 50
            end
            item
              ImageIndex = -1
              Style = hsOwnerDraw
              Width = 50
            end
            item
              ImageIndex = -1
              Style = hsOwnerDraw
              Width = 50
            end>
          OnDrawSection = KeyHeaderControlDrawSection
          OnSectionClick = KeyHeaderControlSectionClick
        end
        object KeySpcPanel: TPanel
          Left = 4
          Top = 422
          Width = 898
          Height = 4
          Align = alBottom
          BevelOuter = bvNone
          TabOrder = 2
        end
      end
      object KeySetPanel: TPanel
        Left = 0
        Top = 450
        Width = 906
        Height = 130
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 1
        DesignSize = (
          906
          130)
        object TLabel
          Left = 645
          Top = 103
          Width = 59
          Height = 15
          Alignment = taRightJustify
          Caption = #12461#12540#35373#23450#12398
        end
        object InpKeyBtn: TButton
          Left = 708
          Top = 97
          Width = 90
          Height = 24
          Caption = #12452#12531#12509#12540#12488'...'
          TabOrder = 8
          OnClick = InpKeyBtnClick
        end
        object FindUpBtn: TButton
          Left = 583
          Top = 97
          Width = 26
          Height = 24
          Action = FindKeyUpAction
          TabOrder = 7
        end
        object FindKeyEdit: TLabeledEdit
          Left = 448
          Top = 98
          Width = 105
          Height = 23
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #26908#32034
          LabelPosition = lpLeft
          TabOrder = 5
          Text = ''
          OnChange = FindKeyEditChange
          OnKeyDown = FindKeyEditKeyDown
        end
        object KeyKeyGroupBox: TGroupBox
          Left = 4
          Top = 12
          Width = 199
          Height = 109
          Caption = ' '
          TabOrder = 0
          object TLabel
            Left = 96
            Top = 22
            Width = 8
            Height = 15
            Caption = '~'
          end
          object KeyComboBox: TComboBox
            Left = 8
            Top = 18
            Width = 83
            Height = 22
            Style = csOwnerDrawFixed
            DropDownCount = 26
            ImeMode = imClose
            TabOrder = 0
            OnClick = KeySetClick
            OnDrawItem = KeyComboBoxDrawItem
          end
          object CtrlCheckBox: TCheckBox
            Left = 65
            Top = 48
            Width = 49
            Height = 17
            Caption = 'Ctrl'
            TabOrder = 3
            OnClick = KeySetClick
          end
          object ShiftCheckBox: TCheckBox
            Left = 8
            Top = 48
            Width = 51
            Height = 17
            Caption = 'Shift'
            TabOrder = 2
            OnClick = KeySetClick
          end
          object AltCheckBox: TCheckBox
            Left = 120
            Top = 48
            Width = 49
            Height = 17
            Caption = 'Alt'
            TabOrder = 4
            OnClick = KeySetClick
          end
          object Key2ComboBox: TComboBox
            Left = 109
            Top = 18
            Width = 83
            Height = 22
            Hint = '2'#12473#12488#12525#12540#12463#12461#12540
            Style = csOwnerDrawFixed
            DropDownCount = 27
            ImeMode = imClose
            TabOrder = 1
            OnClick = KeySetClick
            OnDrawItem = KeyComboBoxDrawItem
          end
          object SelCheckBox: TCheckBox
            Left = 8
            Top = 79
            Width = 111
            Height = 17
            Caption = #36984#25246#26178#12398#12415#23455#34892
            TabOrder = 5
            OnClick = KeySetClick
          end
          object FindKeyBtn: TButton
            Left = 124
            Top = 75
            Width = 68
            Height = 25
            Caption = #26908#32034'(&F)...'
            TabOrder = 6
            OnClick = FindKeyBtnClick
          end
        end
        object KeyCmdGroupBox: TGroupBox
          Left = 211
          Top = 12
          Width = 675
          Height = 79
          Anchors = [akLeft, akTop, akRight]
          Caption = #12467#12510#12531#12489
          TabOrder = 1
          DesignSize = (
            675
            79)
          object TLabel
            Left = 143
            Top = 51
            Width = 49
            Height = 15
            Alignment = taRightJustify
            Anchors = [akTop, akRight]
            Caption = #12497#12521#12513#12540#12479
            ExplicitLeft = 159
          end
          object CmdComboBox: TComboBox
            Left = 8
            Top = 18
            Width = 434
            Height = 22
            Style = csOwnerDrawFixed
            DropDownCount = 30
            TabOrder = 0
            OnChange = CmdComboBoxChange
            OnDrawItem = CmdComboBoxDrawItem
          end
          object UnRegCmdCheckBox: TCheckBox
            Left = 9
            Top = 50
            Width = 136
            Height = 17
            Caption = #26410#30331#37682#12398#12415#34920#31034
            TabOrder = 3
            OnClick = UnRegCmdCheckBoxClick
          end
          object PrmComboBox: TComboBox
            Left = 216
            Top = 47
            Width = 412
            Height = 22
            Hint = #12497#12521#12513#12540#12479
            Style = csOwnerDrawFixed
            DropDownCount = 16
            TabOrder = 4
          end
          object RefCmdPrmBtn: TButton
            Left = 631
            Top = 47
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            Enabled = False
            TabOrder = 5
            OnClick = RefCmdPrmBtnClick
          end
          object MigemoCheckBox: TCheckBox
            Left = 601
            Top = 20
            Width = 70
            Height = 17
            Anchors = [akTop, akRight]
            Caption = 'Migemo'
            TabOrder = 2
          end
          object KeyFilterEdit: TLabeledEdit
            Left = 493
            Top = 18
            Width = 102
            Height = 23
            EditLabel.Width = 36
            EditLabel.Height = 23
            EditLabel.Caption = #12501#12451#12523#12479
            LabelPosition = lpLeft
            TabOrder = 1
            Text = ''
            OnChange = KeyFilterEdit_Change
            OnKeyDown = KeyFilterEdit_KeyDown
            OnKeyPress = KeyFilterEdit_KeyPress
          end
        end
        object FindDownBtn: TButton
          Left = 555
          Top = 97
          Width = 26
          Height = 24
          Action = FindKeyDownAction
          TabOrder = 6
        end
        object AddKeyBtn: TButton
          Left = 211
          Top = 97
          Width = 60
          Height = 24
          Action = AddKeyAction
          TabOrder = 2
        end
        object ExpKeyBtn: TButton
          Left = 802
          Top = 97
          Width = 90
          Height = 24
          Caption = #12456#12463#12473#12509#12540#12488'...'
          TabOrder = 9
          OnClick = ExpKeyBtnClick
        end
        object DelKeyBtn: TButton
          Left = 339
          Top = 97
          Width = 60
          Height = 24
          Action = DelKeyAction
          TabOrder = 4
        end
        object ChgKeyBtn: TButton
          Left = 275
          Top = 97
          Width = 60
          Height = 24
          Action = ChgKeyAction
          TabOrder = 3
        end
      end
    end
    object AssoSheet: TTabSheet
      HelpContext = 13
      Caption = #38306#36899#20184#12369
      ImageIndex = -1
      object AssociateListBox: TListBox
        Left = 0
        Top = 0
        Width = 906
        Height = 510
        Style = lbOwnerDrawFixed
        Align = alClient
        DoubleBuffered = False
        DragMode = dmAutomatic
        ItemHeight = 12
        ParentDoubleBuffered = False
        TabOrder = 0
        OnClick = AssociateListBoxClick
        OnDrawItem = OptListBoxDrawItem
        OnKeyDown = OptListBoxKeyDown
        OnMouseUp = AssociateListBoxMouseUp
      end
      object AsoPanel: TPanel
        Left = 0
        Top = 510
        Width = 906
        Height = 70
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 1
        DesignSize = (
          906
          70)
        object TLabel
          Left = 436
          Top = 44
          Width = 341
          Height = 15
          Caption = #8251#12371#12371#12391#35373#23450#12375#12383#38306#36899#20184#12369#12399' OpenByApp '#12467#12510#12531#12489#12391#29992#12356#12425#12428#12414#12377#12290
        end
        object TLabel
          Left = 126
          Top = 11
          Width = 12
          Height = 15
          Caption = #10145
        end
        object AsoRefBtn: TButton
          Left = 865
          Top = 7
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 2
          OnClick = AsoRefBtnClick
        end
        object AsoDelBtn: TButton
          Left = 170
          Top = 40
          Width = 70
          Height = 24
          Action = UserModule.DelListItemAction
          TabOrder = 5
        end
        object AsoChgBtn: TButton
          Left = 96
          Top = 40
          Width = 70
          Height = 24
          Action = ChgAssoAction
          TabOrder = 4
        end
        object AsoAddBtn: TButton
          Left = 22
          Top = 40
          Width = 70
          Height = 24
          Action = AddAssoAction
          TabOrder = 3
        end
        object UpAssBtn: TButton
          Left = 256
          Top = 40
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          TabOrder = 6
        end
        object DowAssBtn: TButton
          Left = 290
          Top = 40
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          TabOrder = 7
        end
        object AsoExtEdit: TLabeledEdit
          Left = 56
          Top = 8
          Width = 60
          Height = 23
          EditLabel.Width = 39
          EditLabel.Height = 23
          EditLabel.Caption = #25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
        object AsoSortCheckBox: TCheckBox
          Left = 336
          Top = 43
          Width = 77
          Height = 17
          Caption = #12477#12540#12488
          TabOrder = 8
        end
        object AsoAppComboBox: TComboBox
          Left = 146
          Top = 8
          Width = 717
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          DropDownCount = 20
          TabOrder = 1
          OnDropDown = AsoAppComboBoxDropDown
        end
      end
    end
    object ExtMenuSheet: TTabSheet
      HelpContext = 14
      Caption = #36861#21152#12513#12491#12517#12540
      ImageIndex = -1
      object ExtMenuPanel: TPanel
        Left = 0
        Top = 430
        Width = 906
        Height = 150
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        DesignSize = (
          906
          150)
        object TLabel
          Left = 436
          Top = 124
          Width = 289
          Height = 15
          Caption = #8251#12371#12371#12391#35373#23450#12375#12383#38917#30446#12399#12300#32232#38598#12513#12491#12517#12540#12301#12395#36861#21152#12373#12428#12414#12377#12290
        end
        object TLabel
          Left = 46
          Top = 41
          Width = 38
          Height = 15
          Alignment = taRightJustify
          Caption = #12467#12510#12531#12489
        end
        object MenuAliasLabel: TLabel
          Left = 611
          Top = 11
          Width = 242
          Height = 18
          AutoSize = False
        end
        object MenuTitEdit: TLabeledEdit
          Left = 96
          Top = 8
          Width = 220
          Height = 23
          EditLabel.Width = 58
          EditLabel.Height = 23
          EditLabel.Caption = #12461#12515#12503#12471#12519#12531
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
          OnChange = MenuTitEditChange
        end
        object MenuCmdComboBox: TComboBox
          Left = 96
          Top = 35
          Width = 802
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 2
          OnChange = MenuCmdComboBoxChange
        end
        object MenuPrmEdit: TLabeledEdit
          Left = 96
          Top = 62
          Width = 673
          Height = 23
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12521#12513#12540#12479
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object RefMenuPrmBtn: TButton
          Left = 771
          Top = 61
          Width = 36
          Height = 24
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefCmdsBtnClick
        end
        object DowMenuBtn: TButton
          Left = 290
          Top = 120
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          TabOrder = 12
        end
        object UpMenuBtn: TButton
          Left = 256
          Top = 120
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          TabOrder = 11
        end
        object DelMenuBtn: TButton
          Left = 170
          Top = 120
          Width = 70
          Height = 24
          Action = UserModule.DelListItemAction
          TabOrder = 10
        end
        object ChgMenuBtn: TButton
          Left = 96
          Top = 120
          Width = 70
          Height = 24
          Action = ChgMenuAction
          TabOrder = 9
        end
        object AddMenuBtn: TButton
          Left = 22
          Top = 120
          Width = 70
          Height = 24
          Action = AddMenuAction
          TabOrder = 8
        end
        object MenuAliasEdit: TLabeledEdit
          Left = 400
          Top = 8
          Width = 205
          Height = 23
          EditLabel.Width = 47
          EditLabel.Height = 23
          EditLabel.Caption = #12456#12452#12522#12450#12473
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object ExEditBtn: TButton
          Left = 808
          Top = 61
          Width = 90
          Height = 24
          Action = EditFileAction
          Anchors = [akTop, akRight]
          TabOrder = 5
        end
        object MenuIconEdit: TLabeledEdit
          Left = 96
          Top = 89
          Width = 673
          Height = 23
          EditLabel.Width = 38
          EditLabel.Height = 23
          EditLabel.Caption = #12450#12452#12467#12531
          LabelPosition = lpLeft
          TabOrder = 6
          Text = ''
        end
        object RefMenuIconBtn: TButton
          Tag = 100
          Left = 771
          Top = 88
          Width = 36
          Height = 23
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 7
          OnClick = RefMenuIconBtnClick
        end
      end
      object ExtMenuListBox: TCheckListBox
        Left = 0
        Top = 20
        Width = 906
        Height = 410
        Align = alClient
        DoubleBuffered = False
        DragMode = dmAutomatic
        ItemHeight = 13
        ParentDoubleBuffered = False
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnClick = ExtMenuListBoxClick
        OnDrawItem = OptMenuListBoxDrawItem
        OnKeyDown = OptListBoxKeyDown
      end
      object ExtMenuHeader: THeaderControl
        Left = 0
        Top = 0
        Width = 906
        Height = 20
        Sections = <
          item
            Alignment = taCenter
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end
          item
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end
          item
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end>
        Style = hsFlat
        OnDrawSection = ExtHeaderDrawSection
        OnSectionResize = ExtMenuHeaderSectionResize
      end
    end
    object ExtToolSheet: TTabSheet
      HelpContext = 15
      Caption = #22806#37096#12484#12540#12523
      ImageIndex = -1
      object ExtToolPanel: TPanel
        Left = 0
        Top = 430
        Width = 906
        Height = 150
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 2
        DesignSize = (
          906
          150)
        object TLabel
          Left = 436
          Top = 124
          Width = 295
          Height = 15
          Caption = #8251#12371#12371#12391#35373#23450#12375#12383#38917#30446#12399#12300#12484#12540#12523#12513#12491#12517#12540#12301#12395#36861#21152#12373#12428#12414#12377#12290
        end
        object RefToolFmtBtn: TSpeedButton
          Left = 865
          Top = 60
          Width = 23
          Height = 22
          Anchors = [akTop, akRight]
          Flat = True
          OnClick = RefToolFmtBtnClick
        end
        object ToolAliasLabel: TLabel
          Left = 611
          Top = 11
          Width = 234
          Height = 18
          AutoSize = False
        end
        object ToolTitEdit: TLabeledEdit
          Left = 96
          Top = 8
          Width = 220
          Height = 23
          EditLabel.Width = 58
          EditLabel.Height = 23
          EditLabel.Caption = #12461#12515#12503#12471#12519#12531
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
          OnChange = ToolTitEditChange
        end
        object ToolExeEdit: TLabeledEdit
          Left = 96
          Top = 35
          Width = 764
          Height = 23
          EditLabel.Width = 63
          EditLabel.Height = 23
          EditLabel.Caption = #23455#34892#12501#12449#12452#12523
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object ToolPrmEdit: TLabeledEdit
          Left = 96
          Top = 62
          Width = 764
          Height = 23
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12521#12513#12540#12479
          LabelPosition = lpLeft
          TabOrder = 4
          Text = ''
        end
        object RefExeBtn: TButton
          Left = 862
          Top = 35
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 3
          OnClick = RefExeBtnClick
        end
        object DelExtBtn: TButton
          Left = 170
          Top = 120
          Width = 70
          Height = 24
          Action = UserModule.DelListItemAction
          TabOrder = 9
        end
        object ChgExtBtn: TButton
          Left = 96
          Top = 120
          Width = 70
          Height = 24
          Action = ChgToolAction
          TabOrder = 8
        end
        object AddExtBtn: TButton
          Left = 22
          Top = 120
          Width = 70
          Height = 24
          Action = AddToolAction
          TabOrder = 7
        end
        object UpExtBtn: TButton
          Left = 256
          Top = 120
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          TabOrder = 10
        end
        object DowExtBtn: TButton
          Left = 290
          Top = 120
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          TabOrder = 11
        end
        object RefDirBtn: TButton
          Left = 862
          Top = 89
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 6
          OnClick = RefDirBtnClick
        end
        object ToolDirEdit: TLabeledEdit
          Left = 96
          Top = 89
          Width = 764
          Height = 23
          EditLabel.Width = 80
          EditLabel.Height = 23
          EditLabel.Caption = #20316#26989#12487#12451#12524#12463#12488#12522
          LabelPosition = lpLeft
          TabOrder = 5
          Text = ''
        end
        object ToolAliasEdit: TLabeledEdit
          Left = 400
          Top = 8
          Width = 205
          Height = 23
          EditLabel.Width = 47
          EditLabel.Height = 23
          EditLabel.Caption = #12456#12452#12522#12450#12473
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
      end
      object ExtToolListBox: TCheckListBox
        Left = 0
        Top = 20
        Width = 906
        Height = 410
        Align = alClient
        DragMode = dmAutomatic
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnClick = ExtToolListBoxClick
        OnDrawItem = OptMenuListBoxDrawItem
        OnKeyDown = OptListBoxKeyDown
      end
      object ExtToolHeader: THeaderControl
        Left = 0
        Top = 0
        Width = 906
        Height = 20
        Sections = <
          item
            Alignment = taCenter
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end
          item
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end
          item
            AllowClick = False
            ImageIndex = -1
            Style = hsOwnerDraw
            Width = 100
          end>
        Style = hsFlat
        OnDrawSection = ExtHeaderDrawSection
        OnSectionResize = ExtToolHeaderSectionResize
      end
    end
    object StartupSheet: TTabSheet
      HelpContext = 17
      Caption = #36215#21205#26178
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object DuplGroupBox: TGroupBox
        Left = 10
        Top = 392
        Width = 434
        Height = 50
        Caption = #20108#37325#36215#21205
        TabOrder = 6
        object MulInstCheckBox: TCheckBox
          Left = 13
          Top = 20
          Width = 76
          Height = 17
          Caption = #35377#21487
          TabOrder = 0
        end
        object CloseOtherCheckBox: TCheckBox
          Left = 95
          Top = 20
          Width = 279
          Height = 17
          Caption = #32066#20102#26178#12395#12289#20108#37325#36215#21205#12375#12383'NyanFi'#12418#32066#20102#12377#12427
          TabOrder = 1
        end
      end
      object IniPathToTab1CheckBox: TCheckBox
        Left = 26
        Top = 208
        Width = 201
        Height = 17
        Caption = #21021#26399#12497#12473#12434#26368#21021#12398#12479#12502#12391#38283#12367
        TabOrder = 2
      end
      object IniTabHomeAllCheckBox: TCheckBox
        Left = 237
        Top = 208
        Width = 250
        Height = 17
        Caption = #21021#26399#12497#12473#20197#22806#12398#12479#12502#12434#12507#12540#12512#12395#25147#12377
        TabOrder = 3
      end
      object IniWinSizeGroupBox: TGroupBox
        Left = 10
        Top = 243
        Width = 868
        Height = 75
        Anchors = [akLeft, akTop, akRight]
        Caption = #12454#12451#12531#12489#12454#12469#12452#12474
        TabOrder = 4
        object IniWin0RadioBtn: TRadioButton
          Left = 15
          Top = 18
          Width = 200
          Height = 17
          Caption = #21069#22238#32066#20102#26178#12398#12469#12452#12474
          TabOrder = 0
        end
        object IniWin1RadioBtn: TRadioButton
          Left = 15
          Top = 46
          Width = 106
          Height = 17
          Caption = #25351#23450#12469#12452#12474
          TabOrder = 1
          OnClick = IniWinSizeChange
        end
        object IniWinLeftEdit: TLabeledEdit
          Left = 139
          Top = 44
          Width = 55
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #24038
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
          OnChange = IniWinSizeChange
        end
        object IniWinTopEdit: TLabeledEdit
          Left = 220
          Top = 44
          Width = 55
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #19978
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
          OnChange = IniWinSizeChange
        end
        object IniWinWdEdit: TLabeledEdit
          Left = 301
          Top = 44
          Width = 55
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
          OnChange = IniWinSizeChange
        end
        object IniWinHiEdit: TLabeledEdit
          Left = 382
          Top = 44
          Width = 55
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 13
          EditLabel.Height = 23
          EditLabel.Caption = #39640
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 5
          Text = ''
          OnChange = IniWinSizeChange
        end
        object AlRightBtn: TButton
          Left = 624
          Top = 42
          Width = 70
          Height = 24
          Caption = #21491#23492#12379
          TabOrder = 7
          OnClick = AlRightBtnClick
        end
        object MaxWdBtn: TButton
          Left = 698
          Top = 42
          Width = 70
          Height = 24
          Caption = #26368#22823#24133
          TabOrder = 8
          OnClick = MaxWdBtnClick
        end
        object MaxHiBtn: TButton
          Left = 772
          Top = 42
          Width = 70
          Height = 24
          Caption = #26368#22823#39640
          TabOrder = 9
          OnClick = MaxHiBtnClick
        end
        object FixWinCheckBox: TCheckBox
          Left = 470
          Top = 46
          Width = 144
          Height = 17
          Caption = #12469#12452#12474#12392#20301#32622#12434#22266#23450
          TabOrder = 6
        end
      end
      object IniWorkListGroupBox: TGroupBox
        Left = 10
        Top = 330
        Width = 868
        Height = 50
        Anchors = [akLeft, akTop, akRight]
        Caption = #36215#21205#26178#12395#35501#12415#36796#12416#12527#12540#12463#12522#12473#12488
        TabOrder = 5
        DesignSize = (
          868
          50)
        object IniWork0RadioBtn: TRadioButton
          Left = 15
          Top = 20
          Width = 177
          Height = 17
          Caption = #21069#22238#32066#20102#26178#12398#12527#12540#12463#12522#12473#12488
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object IniWork1RadioBtn: TRadioButton
          Left = 194
          Top = 20
          Width = 180
          Height = 17
          Caption = #25351#23450#12527#12540#12463#12522#12473#12488'('#12507#12540#12512')'
          TabOrder = 1
        end
        object HomeWorkListEdit: TEdit
          Left = 375
          Top = 18
          Width = 447
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 2
        end
        object RefHomeWorkBtn: TButton
          Left = 823
          Top = 17
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 3
          OnClick = RefHomeWorkBtnClick
        end
      end
      object LeftPathGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 186
        Caption = #21021#26399#12497#12473' - '#24038
        TabOrder = 0
        DesignSize = (
          434
          186)
        object TLabel
          Left = 17
          Top = 154
          Width = 50
          Height = 15
          Alignment = taRightJustify
          Caption = #12497#12473#12510#12473#12463
        end
        object L_IniPatMod0RadioBtn: TRadioButton
          Left = 15
          Top = 18
          Width = 148
          Height = 17
          Caption = #21069#22238#32066#20102#26178#12398#12497#12473
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object L_IniPatMod1RadioBtn: TRadioButton
          Left = 172
          Top = 18
          Width = 118
          Height = 17
          Caption = #12527#12540#12463#12522#12473#12488
          TabOrder = 1
          OnClick = L_IniPatMod1RadioBtnClick
        end
        object L_IniPatMod2RadioBtn: TRadioButton
          Left = 300
          Top = 18
          Width = 82
          Height = 17
          Caption = #25351#23450#12497#12473
          TabOrder = 2
        end
        object L_IniPatEdit: TEdit
          Left = 71
          Top = 40
          Width = 353
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 3
        end
        object L_RefIniPatBtn: TButton
          Left = 388
          Top = 16
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefIniPatBtnClick
        end
        object L_IniSortRadioGroup: TRadioGroup
          Left = 16
          Top = 77
          Width = 408
          Height = 60
          Caption = #12477#12540#12488#26041#27861
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #21069#22238#32066#20102#26178
            #21517#21069
            #25313#24373#23376
            #26356#26032#26085#20184
            #12469#12452#12474
            #23646#24615
            #12394#12375)
          TabOrder = 5
        end
        object L_IniMaskComboBox: TComboBox
          Left = 71
          Top = 151
          Width = 353
          Height = 23
          TabOrder = 6
        end
      end
      object RightPathGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 186
        Caption = #21021#26399#12497#12473' - '#21491
        TabOrder = 1
        DesignSize = (
          434
          186)
        object TLabel
          Left = 17
          Top = 154
          Width = 50
          Height = 15
          Alignment = taRightJustify
          Caption = #12497#12473#12510#12473#12463
        end
        object R_IniPatMod0RadioBtn: TRadioButton
          Left = 15
          Top = 18
          Width = 148
          Height = 17
          Caption = #21069#22238#32066#20102#26178#12398#12497#12473
          Checked = True
          TabOrder = 0
          TabStop = True
        end
        object R_IniPatMod1RadioBtn: TRadioButton
          Left = 172
          Top = 18
          Width = 118
          Height = 17
          Caption = #12527#12540#12463#12522#12473#12488
          TabOrder = 1
          OnClick = R_IniPatMod1RadioBtnClick
        end
        object R_IniPatMod2RadioBtn: TRadioButton
          Left = 300
          Top = 18
          Width = 82
          Height = 17
          Caption = #25351#23450#12497#12473
          TabOrder = 2
        end
        object R_IniPatEdit: TEdit
          Left = 71
          Top = 40
          Width = 353
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 3
        end
        object R_RefIniPatBtn: TButton
          Tag = 1
          Left = 388
          Top = 16
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 4
          OnClick = RefIniPatBtnClick
        end
        object R_IniSortRadioGroup: TRadioGroup
          Left = 16
          Top = 77
          Width = 408
          Height = 60
          Caption = #12477#12540#12488#26041#27861
          Columns = 4
          ItemIndex = 0
          Items.Strings = (
            #21069#22238#32066#20102#26178
            #21517#21069
            #25313#24373#23376
            #26356#26032#26085#20184
            #12469#12452#12474
            #23646#24615
            #12394#12375)
          TabOrder = 5
        end
        object R_IniMaskComboBox: TComboBox
          Left = 71
          Top = 151
          Width = 353
          Height = 23
          TabOrder = 6
        end
      end
      object SplashCheckBox: TCheckBox
        Left = 480
        Top = 412
        Width = 203
        Height = 17
        Caption = #12473#12503#12521#12483#12471#12517#30011#38754#12434#34920#31034
        TabOrder = 7
      end
      object VirDrvGroupBox: TGroupBox
        Left = 10
        Top = 454
        Width = 884
        Height = 119
        Caption = #20206#24819#12489#12521#12452#12502#12398#12510#12454#12531#12488
        TabOrder = 8
        DesignSize = (
          884
          119)
        object TLabel
          Left = 452
          Top = 27
          Width = 37
          Height = 15
          Alignment = taRightJustify
          Caption = #12489#12521#12452#12502
        end
        object VirDrvListBox: TListBox
          Left = 10
          Top = 18
          Width = 424
          Height = 91
          Style = lbOwnerDrawFixed
          Sorted = True
          TabOrder = 0
          OnClick = VirDrvListBoxClick
          OnDrawItem = OptListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object DelDrvBtn: TButton
          Left = 577
          Top = 85
          Width = 60
          Height = 24
          Action = DelDrvAction
          Anchors = [akLeft, akBottom]
          TabOrder = 6
        end
        object ChgDrvBtn: TButton
          Left = 513
          Top = 85
          Width = 60
          Height = 24
          Action = ChgDrvAction
          Anchors = [akLeft, akBottom]
          TabOrder = 5
        end
        object AddDrvBtn: TButton
          Left = 449
          Top = 85
          Width = 60
          Height = 24
          Action = AddDrvAction
          Anchors = [akLeft, akBottom]
          TabOrder = 4
        end
        object VirDriveEdit: TEdit
          Left = 450
          Top = 52
          Width = 388
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 2
        end
        object RefVDrvBtn: TButton
          Left = 839
          Top = 51
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 3
          OnClick = RefVDrvBtnClick
        end
        object VirDrvComboBox: TComboBox
          Left = 493
          Top = 24
          Width = 80
          Height = 23
          Style = csDropDownList
          TabOrder = 1
        end
      end
    end
    object NotifySheet: TTabSheet
      HelpContext = 18
      Caption = #36890#30693#12539#30906#35469
      ImageIndex = -1
      object TLabel
        Left = 768
        Top = 53
        Width = 24
        Height = 15
        Caption = 'm'#31186
      end
      object TLabel
        Left = 768
        Top = 21
        Width = 24
        Height = 15
        Caption = 'm'#31186
      end
      object EtcWarnGroupBox: TGroupBox
        Left = 10
        Top = 497
        Width = 434
        Height = 76
        Caption = #12381#12398#20182#12398#35686#21578
        TabOrder = 5
        object WarnPowFailCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 116
          Height = 17
          Caption = #38651#28304#12398#25509#32154#20999#12428
          TabOrder = 0
        end
        object WarnLowBatheckBox: TCheckBox
          Left = 154
          Top = 20
          Width = 117
          Height = 17
          Caption = #12496#12483#12486#12522#12540#20302#19979
          TabOrder = 1
        end
        object BatLowLimitEdit: TLabeledEdit
          Left = 274
          Top = 18
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 10
          EditLabel.Height = 23
          EditLabel.Caption = '%'
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 2
          Text = ''
        end
        object WarnDisconCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 166
          Height = 17
          Caption = #12452#12531#12479#12540#12493#12483#12488#25509#32154#20999#12428
          TabOrder = 3
        end
      end
      object KeyHintTimeEdit: TLabeledEdit
        Left = 718
        Top = 49
        Width = 48
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 52
        EditLabel.Height = 23
        EditLabel.Caption = #24453#27231#26178#38291
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 9
        Text = ''
      end
      object MsgHintTimeEdit: TLabeledEdit
        Left = 718
        Top = 18
        Width = 48
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 52
        EditLabel.Height = 23
        EditLabel.Caption = #34920#31034#26178#38291
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 7
        Text = ''
      end
      object PrtDirGroupBox: TGroupBox
        Left = 460
        Top = 190
        Width = 434
        Height = 383
        Caption = #12487#12451#12524#12463#12488#12522#12398#21066#38500#21046#38480
        TabOrder = 14
        object PrtDirListBox: TListBox
          Left = 10
          Top = 18
          Width = 414
          Height = 269
          Style = lbOwnerDrawFixed
          ItemHeight = 13
          Sorted = True
          TabOrder = 0
          OnDrawItem = PrtDirListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object AddDirBtn: TButton
          Left = 10
          Top = 293
          Width = 60
          Height = 24
          Action = AddPrtDirAction
          TabOrder = 1
        end
        object DelDirBtn: TButton
          Left = 76
          Top = 293
          Width = 60
          Height = 24
          Action = UserModule.DelListItemAction
          TabOrder = 2
        end
        object PrtDirRadioGroup: TRadioGroup
          Left = 10
          Top = 327
          Width = 220
          Height = 44
          Caption = #21046#38480#12514#12540#12489
          Columns = 2
          Items.Strings = (
            #21066#38500#31105#27490
            #30906#35469#12375#12390#21066#38500)
          TabOrder = 3
        end
        object PrtSubCheckBox: TCheckBox
          Left = 251
          Top = 329
          Width = 180
          Height = 17
          Caption = #19979#20301#12487#12451#12524#12463#12488#12522#12418#21046#38480
          TabOrder = 4
        end
        object PrtFileCheckBox: TCheckBox
          Left = 251
          Top = 352
          Width = 180
          Height = 17
          Caption = #12501#12449#12452#12523#12398#21066#38500#12418#21046#38480
          TabOrder = 5
        end
      end
      object ShowCopyHintCheckBox: TCheckBox
        Left = 475
        Top = 107
        Width = 268
        Height = 17
        Caption = #12463#12522#12483#12503#12508#12540#12489#12408#12398#12467#12500#12540#24773#22577#12434#12498#12531#12488#34920#31034
        TabOrder = 11
      end
      object ShowDuplNotifyCheckBox: TCheckBox
        Left = 475
        Top = 132
        Width = 268
        Height = 17
        Caption = #20108#37325#36215#21205#12373#12428#12383'NyanFi'#12363#12425#12398#36890#30693#12434#34920#31034
        TabOrder = 12
      end
      object ShowKeyHintCheckBox: TCheckBox
        Left = 475
        Top = 51
        Width = 183
        Height = 17
        Caption = '2'#12473#12488#12525#12540#12463#12398#12498#12531#12488#34920#31034
        TabOrder = 8
      end
      object ShowMsgHintCheckBox: TCheckBox
        Left = 475
        Top = 19
        Width = 183
        Height = 17
        Caption = #12513#12540#12475#12540#12472#12434#12498#12531#12488#34920#31034
        TabOrder = 6
      end
      object ShowNoKeyHintCheckBox: TCheckBox
        Left = 475
        Top = 82
        Width = 268
        Height = 17
        Caption = #28961#21177#12394'2'#12473#12488#12525#12540#12463#25805#20316#12395#35686#21578#12434#20986#12377' '
        TabOrder = 10
      end
      object ShowTooltipCheckBox: TCheckBox
        Left = 475
        Top = 157
        Width = 183
        Height = 17
        Caption = #12484#12540#12523#12481#12483#12503#12434#34920#31034
        TabOrder = 13
      end
      object SndFindGroupBox: TGroupBox
        Left = 10
        Top = 190
        Width = 434
        Height = 50
        Caption = #26908#32034#32066#20102#26178#12398#36890#30693#38899
        TabOrder = 2
        DesignSize = (
          434
          50)
        object SndFindFinEdit: TEdit
          Left = 10
          Top = 18
          Width = 355
          Height = 23
          TabOrder = 0
        end
        object RefSndFindFinBtn: TButton
          Tag = 2
          Left = 368
          Top = 19
          Width = 36
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefSoundBtnClick
        end
        object TestSndFindFinBtn: TButton
          Tag = 2
          Left = 405
          Top = 19
          Width = 22
          Height = 22
          Hint = #12486#12473#12488#20877#29983
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 2
          OnClick = TestSoundBtnClick
        end
      end
      object SndTaskGroupBox: TGroupBox
        Left = 10
        Top = 100
        Width = 434
        Height = 78
        Caption = #12479#12473#12463#32066#20102#26178#12398#36890#30693#38899
        TabOrder = 1
        DesignSize = (
          434
          78)
        object TLabel
          Left = 198
          Top = 49
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object SndTaskFinEdit: TEdit
          Left = 10
          Top = 18
          Width = 355
          Height = 23
          TabOrder = 0
        end
        object RefSndTaskFinBtn: TButton
          Tag = 1
          Left = 368
          Top = 19
          Width = 36
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefSoundBtnClick
        end
        object TestSndTaskFinBtn: TButton
          Tag = 1
          Left = 405
          Top = 19
          Width = 22
          Height = 22
          Hint = #12486#12473#12488#20877#29983
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 2
          OnClick = TestSoundBtnClick
        end
        object NotifyTaskTimeEdit: TLabeledEdit
          Left = 243
          Top = 46
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 122
          EditLabel.Height = 23
          EditLabel.Caption = #31186#20197#19978#12398#12479#12473#12463#12391#40180#12425#12377
          LabelPosition = lpRight
          NumbersOnly = True
          TabOrder = 5
          Text = ''
        end
        object FwCntTaskFinEdit: TLabeledEdit
          Left = 72
          Top = 46
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #28857#28357#22238#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 3
          Text = ''
        end
        object FwTmTaskFinEdit: TLabeledEdit
          Left = 156
          Top = 46
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #38291#38548
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
      end
      object SndWarnGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 78
        Caption = #35686#21578#38899
        TabOrder = 0
        DesignSize = (
          434
          78)
        object TLabel
          Left = 198
          Top = 49
          Width = 24
          Height = 15
          Caption = 'm'#31186
        end
        object SndWarnEdit: TEdit
          Left = 10
          Top = 18
          Width = 355
          Height = 23
          TabOrder = 0
        end
        object RefSndWarnBtn: TButton
          Left = 368
          Top = 19
          Width = 36
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefSoundBtnClick
        end
        object TestSndWarnBtn: TButton
          Left = 405
          Top = 19
          Width = 22
          Height = 22
          Hint = #12486#12473#12488#20877#29983
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 2
          OnClick = TestSoundBtnClick
        end
        object FwCntWarnEdit: TLabeledEdit
          Left = 72
          Top = 46
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 52
          EditLabel.Height = 23
          EditLabel.Caption = #28857#28357#22238#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 3
          Text = ''
        end
        object FwTmWarnEdit: TLabeledEdit
          Left = 156
          Top = 46
          Width = 40
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 26
          EditLabel.Height = 23
          EditLabel.Caption = #38291#38548
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
      end
      object SndWatchGroupBox: TGroupBox
        Left = 10
        Top = 252
        Width = 434
        Height = 50
        Caption = #12501#12449#12452#12523#30435#35222#12398#36890#30693#38899
        TabOrder = 3
        DesignSize = (
          434
          50)
        object SndWatchEdit: TEdit
          Left = 10
          Top = 18
          Width = 355
          Height = 23
          TabOrder = 0
        end
        object RefSndWatchBtn: TButton
          Tag = 3
          Left = 368
          Top = 19
          Width = 36
          Height = 22
          Hint = #21442#29031
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 1
          OnClick = RefSoundBtnClick
        end
        object TestSndWatchBtn: TButton
          Tag = 3
          Left = 405
          Top = 19
          Width = 22
          Height = 22
          Hint = #12486#12473#12488#20877#29983
          Anchors = [akTop, akRight]
          Caption = '>'
          TabOrder = 2
          OnClick = TestSoundBtnClick
        end
      end
      object SureMsgGroupBox: TGroupBox
        Left = 10
        Top = 318
        Width = 434
        Height = 161
        Caption = #30906#35469#12513#12483#12475#12540#12472
        TabOrder = 4
        object SureCopyCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 64
          Height = 17
          Caption = #12467#12500#12540
          TabOrder = 0
        end
        object SureMoveCheckBox: TCheckBox
          Left = 80
          Top = 20
          Width = 64
          Height = 17
          Caption = #31227#21205
          TabOrder = 1
        end
        object SureDeleteCheckBox: TCheckBox
          Left = 150
          Top = 20
          Width = 64
          Height = 17
          Caption = #21066#38500
          TabOrder = 2
        end
        object SureUnPackCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 64
          Height = 17
          Caption = #35299#20941
          TabOrder = 4
        end
        object SureCmpDelCheckBox: TCheckBox
          Left = 276
          Top = 20
          Width = 152
          Height = 17
          Caption = #23436#20840#21066#38500#12399'2'#24230#30906#35469
          TabOrder = 3
        end
        object SureExitCheckBox: TCheckBox
          Left = 150
          Top = 47
          Width = 64
          Height = 17
          Caption = #32066#20102
          TabOrder = 6
        end
        object SureOtherCheckBox: TCheckBox
          Left = 220
          Top = 47
          Width = 74
          Height = 17
          Caption = #12381#12398#20182
          TabOrder = 7
        end
        object SureExecCheckBox: TCheckBox
          Left = 80
          Top = 47
          Width = 64
          Height = 17
          Caption = #23455#34892
          TabOrder = 5
        end
        object SureCancelCheckBox: TCheckBox
          Left = 10
          Top = 101
          Width = 164
          Height = 17
          Caption = #12461#12515#12531#12475#12523#12508#12479#12531#12434#34920#31034
          TabOrder = 9
        end
        object SureDefNoCheckBox: TCheckBox
          Left = 184
          Top = 101
          Width = 186
          Height = 17
          Caption = #12300#12356#12356#12360#12301#12364#12487#12501#12457#12523#12488#12508#12479#12531
          TabOrder = 10
        end
        object SureAdjPosCheckBox: TCheckBox
          Left = 10
          Top = 128
          Width = 231
          Height = 17
          Caption = #29366#27841#12395#24540#12376#12390#34920#31034#20301#32622#12434#35519#25972
          TabOrder = 11
        end
        object SureWorkListCheckBox: TCheckBox
          Left = 10
          Top = 74
          Width = 261
          Height = 17
          Caption = #12527#12540#12463#12522#12473#12488#26356#26032#26178#12398#20877#35501#12415#36796#12415
          TabOrder = 8
        end
      end
    end
    object CommandSheet: TTabSheet
      HelpContext = 19
      Caption = #12467#12510#12531#12489
      ImageIndex = -1
      object AppListGroupBox: TGroupBox
        Left = 10
        Top = 10
        Width = 434
        Height = 80
        Caption = 'AppList'
        TabOrder = 0
        DesignSize = (
          434
          80)
        object TLabel
          Left = 10
          Top = 22
          Width = 50
          Height = 15
          Caption = #12507#12483#12488#12461#12540
        end
        object TLabel
          Left = 208
          Top = 51
          Width = 49
          Height = 15
          Alignment = taRightJustify
          Anchors = [akLeft, akTop, akRight]
          Caption = #12497#12521#12513#12540#12479
        end
        object AppAltCheckBox: TCheckBox
          Left = 211
          Top = 21
          Width = 47
          Height = 17
          Caption = 'Alt'
          TabOrder = 2
        end
        object AppCtrlCheckBox: TCheckBox
          Left = 156
          Top = 21
          Width = 47
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 1
        end
        object AppShiftCheckBox: TCheckBox
          Left = 101
          Top = 21
          Width = 47
          Height = 17
          Caption = 'Shift'
          TabOrder = 0
        end
        object AppKeyComboBox: TComboBox
          Left = 350
          Top = 19
          Width = 75
          Height = 23
          Style = csDropDownList
          DropDownCount = 24
          ImeMode = imClose
          TabOrder = 4
        end
        object AppWinCheckBox: TCheckBox
          Left = 266
          Top = 21
          Width = 71
          Height = 17
          Caption = 'Win   +'
          TabOrder = 3
        end
        object AppMinCheckBox: TCheckBox
          Left = 10
          Top = 50
          Width = 180
          Height = 17
          Caption = #20182#12450#12503#12522#20999#26367#26178#12395#26368#23567#21270
          TabOrder = 5
        end
        object AppPrmComboBox: TComboBox
          Left = 261
          Top = 48
          Width = 164
          Height = 23
          DropDownCount = 12
          TabOrder = 6
        end
      end
      object BorderLRGroupBox: TGroupBox
        Left = 10
        Top = 102
        Width = 213
        Height = 46
        Caption = 'BorderLeft'#12289'BorderRight'
        TabOrder = 1
        object BdrMovWdLabeledEdit: TLabeledEdit
          Left = 156
          Top = 16
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 69
          EditLabel.Height = 23
          EditLabel.Caption = '1'#22238#12398#31227#21205#24133
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
      end
      object CompDelGroupBox: TGroupBox
        Left = 231
        Top = 102
        Width = 213
        Height = 46
        Caption = 'CompleteDelete'
        TabOrder = 2
        object CmpDelOwCntEdit: TLabeledEdit
          Left = 156
          Top = 16
          Width = 48
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 62
          EditLabel.Height = 23
          EditLabel.Caption = #19978#26360#12365#22238#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 0
          Text = ''
        end
      end
      object ExeCmdLineGroupBox: TGroupBox
        Left = 10
        Top = 160
        Width = 434
        Height = 50
        Caption = 'ExeCommandLine'
        TabOrder = 3
        object FExtExeFileEdit: TLabeledEdit
          Left = 182
          Top = 18
          Width = 243
          Height = 23
          EditLabel.Width = 144
          EditLabel.Height = 23
          EditLabel.Caption = #23455#34892#12501#12449#12452#12523#12392#12415#12394#12377#25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object FileEditGroupBox: TGroupBox
        Left = 10
        Top = 222
        Width = 434
        Height = 48
        Caption = 'FileEdit'
        TabOrder = 4
        object OpenOnlyCurEditCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 272
          Height = 17
          Caption = #36984#25246#29366#24907#12395#12363#12363#12431#12425#12378#12459#12540#12477#12523#20301#32622#12398#12415#38283#12367
          TabOrder = 0
        end
        object DontClrSelEditCheckBox: TCheckBox
          Left = 290
          Top = 20
          Width = 137
          Height = 17
          Caption = #36984#25246#12434#35299#38500#12375#12394#12356
          TabOrder = 1
        end
      end
      object IncSearchGroupBox: TGroupBox
        Left = 10
        Top = 282
        Width = 434
        Height = 76
        Caption = 'IncSearch'
        TabOrder = 5
        object IncSeaCaseCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 132
          Height = 17
          Caption = #22823#23567#25991#23383#12434#21306#21029
          TabOrder = 0
        end
        object IncSeaLoopCheckBox: TCheckBox
          Left = 158
          Top = 20
          Width = 126
          Height = 17
          Caption = #19978#19979#31471#12391#12523#12540#12503
          TabOrder = 1
        end
        object IncSea1ExitCheckBox: TCheckBox
          Left = 290
          Top = 20
          Width = 136
          Height = 17
          Caption = #12510#12483#12481#25968'1'#12391#25244#12369#12427
          TabOrder = 2
        end
        object MigemoMinEdit: TLabeledEdit
          Left = 375
          Top = 44
          Width = 50
          Height = 23
          Alignment = taRightJustify
          EditLabel.Width = 139
          EditLabel.Height = 23
          EditLabel.Caption = 'Migemo '#26908#32034#38283#22987#25991#23383#25968
          LabelPosition = lpLeft
          NumbersOnly = True
          TabOrder = 4
          Text = ''
        end
        object IncSeaFuzzyCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 132
          Height = 17
          Caption = #12354#12356#12414#12356#26908#32034
          TabOrder = 3
        end
      end
      object IniSearchGroupBox: TGroupBox
        Left = 10
        Top = 370
        Width = 434
        Height = 67
        Caption = 'InitialSearch'
        TabOrder = 6
        object TLabel
          Left = 171
          Top = 19
          Width = 130
          Height = 15
          Caption = '+ A '#65374' Z '#12391#38957#25991#23383#12469#12540#12481
        end
        object IniSeaAltCheckBox: TCheckBox
          Left = 120
          Top = 18
          Width = 47
          Height = 17
          Caption = 'Alt'
          TabOrder = 2
        end
        object IniSeaCtrlCheckBox: TCheckBox
          Left = 65
          Top = 18
          Width = 47
          Height = 17
          Caption = 'Ctrl'
          TabOrder = 1
        end
        object IniSeaShiftCheckBox: TCheckBox
          Left = 10
          Top = 18
          Width = 47
          Height = 17
          Caption = 'Shift'
          TabOrder = 0
        end
        object IniSeaByNumCheckBox: TCheckBox
          Left = 10
          Top = 42
          Width = 167
          Height = 17
          Caption = #25968#23383#12461#12540#12391#12418#12469#12540#12481
          TabOrder = 3
        end
        object IniSeaBySignCheckBox: TCheckBox
          Left = 182
          Top = 42
          Width = 206
          Height = 17
          Caption = 'Shift+'#25968#23383#12461#12540#12398#35352#21495#12418#12469#12540#12481
          TabOrder = 4
        end
      end
      object OpenByAppGroupBox: TGroupBox
        Left = 460
        Top = 398
        Width = 434
        Height = 97
        Caption = 'OpenByApp '
        TabOrder = 11
        DesignSize = (
          434
          97)
        object TLabel
          Left = 30
          Top = 20
          Width = 140
          Height = 15
          Alignment = taRightJustify
          Anchors = [akTop, akRight]
          Caption = #38306#36899#20184#12369#12373#12428#12390#12356#12394#12356#22580#21512
        end
        object OpenByModeComboBox: TComboBox
          Left = 176
          Top = 16
          Width = 247
          Height = 23
          Style = csDropDownList
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 0
        end
        object OpenOnlyCurAppCheckBox: TCheckBox
          Left = 10
          Top = 70
          Width = 274
          Height = 17
          Caption = #36984#25246#29366#24907#12395#12363#12363#12431#12425#12378#12459#12540#12477#12523#20301#32622#12398#12415#38283#12367
          TabOrder = 2
        end
        object OpenDirByStdCheckBox: TCheckBox
          Left = 10
          Top = 47
          Width = 351
          Height = 17
          Caption = #12487#12451#12524#12463#12488#12522#12398#22580#21512#12289#27161#28310#12398' Enter '#12461#12540#21205#20316
          TabOrder = 1
        end
        object DontClrSelAppCheckBox: TCheckBox
          Left = 290
          Top = 70
          Width = 135
          Height = 17
          Caption = #36984#25246#12434#35299#38500#12375#12394#12356
          TabOrder = 3
        end
      end
      object OpenByWinGroupBox: TGroupBox
        Left = 460
        Top = 507
        Width = 433
        Height = 50
        Caption = 'OpenByWin'
        TabOrder = 12
        object OpenOnlyCurWinCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 274
          Height = 17
          Caption = #36984#25246#29366#24907#12395#12363#12363#12431#12425#12378#12459#12540#12477#12523#20301#32622#12398#12415#38283#12367
          TabOrder = 0
        end
        object DontClrSelWinCheckBox: TCheckBox
          Left = 290
          Top = 20
          Width = 135
          Height = 17
          Caption = #36984#25246#12434#35299#38500#12375#12394#12356
          TabOrder = 1
        end
      end
      object OpenStdGroupBox: TGroupBox
        Left = 460
        Top = 10
        Width = 434
        Height = 375
        Caption = 'OpenStandard'
        TabOrder = 10
        DesignSize = (
          434
          375)
        object TLabel
          Left = 11
          Top = 16
          Width = 110
          Height = 15
          Caption = #12467#12510#12531#12489#12408#12398#38306#36899#20184#12369
        end
        object TLabel
          Left = 49
          Top = 233
          Width = 38
          Height = 15
          Alignment = taRightJustify
          Anchors = [akRight, akBottom]
          Caption = #12467#12510#12531#12489
        end
        object FExtStdEdit: TLabeledEdit
          Left = 92
          Top = 201
          Width = 331
          Height = 23
          EditLabel.Width = 65
          EditLabel.Height = 23
          EditLabel.Caption = #23550#35937#25313#24373#23376
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object StdCmdComboBox: TComboBox
          Left = 92
          Top = 228
          Width = 331
          Height = 23
          Style = csDropDownList
          TabOrder = 2
          OnChange = StdCmdComboBoxChange
        end
        object StdCmdListBox: TListBox
          Left = 10
          Top = 33
          Width = 413
          Height = 156
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akRight, akBottom]
          DragMode = dmAutomatic
          TabOrder = 0
          OnClick = StdCmdListBoxClick
          OnDrawItem = StdCmdListBoxDrawItem
          OnKeyDown = OptListBoxKeyDown
        end
        object CmdDelBtn: TButton
          Left = 220
          Top = 282
          Width = 60
          Height = 24
          Action = UserModule.DelListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 7
        end
        object CmdChgBtn: TButton
          Left = 156
          Top = 282
          Width = 60
          Height = 24
          Action = ChgStdCmdAction
          Anchors = [akLeft, akBottom]
          TabOrder = 6
        end
        object CmdAddBtn: TButton
          Left = 92
          Top = 282
          Width = 60
          Height = 24
          Action = AddStdCmdAction
          Anchors = [akLeft, akBottom]
          TabOrder = 5
        end
        object DowCmdBtn: TButton
          Left = 324
          Top = 282
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 9
        end
        object UpCmdBtn: TButton
          Left = 292
          Top = 282
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 8
        end
        object DowAfterStdCheckBox: TCheckBox
          Left = 10
          Top = 348
          Width = 200
          Height = 17
          Anchors = [akLeft, akBottom]
          Caption = #23455#34892#24460#12395#12459#12540#12477#12523#12434#19979#12395#31227#21205
          TabOrder = 13
        end
        object OpnStdResCheckBox: TCheckBox
          Left = 216
          Top = 348
          Width = 200
          Height = 17
          Anchors = [akLeft, akBottom]
          Caption = #32080#26524#12522#12473#12488#12391#12418#36890#24120#21205#20316
          TabOrder = 14
        end
        object StdCmdPrmEdit: TLabeledEdit
          Left = 92
          Top = 255
          Width = 290
          Height = 23
          EditLabel.Width = 49
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12521#12513#12540#12479
          Enabled = False
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object RefStdCmdPrmBtn: TButton
          Tag = 100
          Left = 385
          Top = 255
          Width = 38
          Height = 22
          Caption = '...'
          TabOrder = 4
          OnClick = RefStdCmdPrmBtnClick
        end
        object StdTabGroupCheckBox: TCheckBox
          Left = 10
          Top = 323
          Width = 110
          Height = 17
          Anchors = [akLeft, akBottom]
          Caption = #12479#12502#12464#12523#12540#12503
          TabOrder = 10
        end
        object StdResultListCheckBox: TCheckBox
          Left = 242
          Top = 323
          Width = 100
          Height = 17
          Anchors = [akLeft, akBottom]
          Caption = #32080#26524#12522#12473#12488
          TabOrder = 12
        end
        object StdMenuFileCheckBox: TCheckBox
          Left = 126
          Top = 323
          Width = 110
          Height = 17
          Anchors = [akLeft, akBottom]
          Caption = #12513#12491#12517#12540#23450#32681
          TabOrder = 11
        end
        object BevelPanel3: TPanel
          Left = 5
          Top = 314
          Width = 423
          Height = 2
          AutoSize = True
          BevelOuter = bvNone
          TabOrder = 15
          object Shape5: TShape
            Left = 0
            Top = 0
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnShadow
          end
          object Shape6: TShape
            Left = 0
            Top = 1
            Width = 423
            Height = 1
            Brush.Style = bsClear
            Pen.Color = clBtnHighlight
          end
        end
      end
      object TaskManGroupBox: TGroupBox
        Left = 168
        Top = 449
        Width = 276
        Height = 46
        Caption = 'TaskMan'
        TabOrder = 8
        object NotShowNoTaskCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 259
          Height = 17
          Caption = #12479#12473#12463#12364#12394#12356#12392#12365#12399#12510#12493#12540#12472#12515#12434#34920#31034#12375#12394#12356
          TabOrder = 0
        end
      end
      object WebSearchGroupBox: TGroupBox
        Left = 10
        Top = 507
        Width = 434
        Height = 50
        Caption = 'WebSearch'
        TabOrder = 9
        object WebSeaUrlEdit: TLabeledEdit
          Left = 40
          Top = 18
          Width = 385
          Height = 23
          Hint = '\S : '#26908#32034#35486
          EditLabel.Width = 21
          EditLabel.Height = 23
          EditLabel.Caption = 'URL'
          LabelPosition = lpLeft
          TabOrder = 0
          Text = ''
        end
      end
      object SyncLRGroupBox: TGroupBox
        Left = 10
        Top = 449
        Width = 150
        Height = 46
        Caption = 'SyncLR'
        TabOrder = 7
        object SyncItemCheckBox: TCheckBox
          Left = 10
          Top = 20
          Width = 132
          Height = 17
          Caption = #38917#30446#20301#32622#12418#21516#26399
          TabOrder = 0
        end
      end
    end
    object EventSheet: TTabSheet
      HelpContext = 21
      Caption = #12452#12505#12531#12488
      ImageIndex = -1
      DesignSize = (
        906
        580)
      object EvEditBtn: TButton
        Left = 781
        Top = 438
        Width = 90
        Height = 24
        Action = EditFileAction
        Anchors = [akTop, akRight]
        TabOrder = 6
      end
      object EventCmdsEdit: TLabeledEdit
        Left = 12
        Top = 439
        Width = 723
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 327
        EditLabel.Height = 15
        EditLabel.Caption = #12452#12505#12531#12488#30330#29983#26178#12395#23455#34892#12377#12427#12467#12510#12531#12489' (ExeCommands '#12398#12497#12521#12513#12540#12479')'
        TabOrder = 4
        Text = ''
        OnChange = EventCmdsEditChange
      end
      object EventListBox: TListBox
        Left = 0
        Top = 0
        Width = 906
        Height = 405
        Style = lbOwnerDrawFixed
        Align = alTop
        DoubleBuffered = False
        ParentDoubleBuffered = False
        TabOrder = 0
        OnClick = EventListBoxClick
        OnDrawItem = OptListBoxDrawItem
        OnKeyDown = OptListBoxKeyDown
      end
      object FindDownEvBtn: TButton
        Left = 738
        Top = 410
        Width = 26
        Height = 24
        Action = FindEvDownAction
        TabOrder = 2
      end
      object FindEvEdit: TLabeledEdit
        Left = 576
        Top = 411
        Width = 159
        Height = 23
        EditLabel.Width = 26
        EditLabel.Height = 23
        EditLabel.Caption = #26908#32034
        LabelPosition = lpLeft
        TabOrder = 1
        Text = ''
        OnChange = FindEvEditChange
        OnKeyDown = FindEvEditKeyDown
      end
      object FindUpEvBtn: TButton
        Left = 766
        Top = 410
        Width = 26
        Height = 24
        Action = FindEvUpAction
        TabOrder = 3
      end
      object RefCmdsBtn: TButton
        Tag = 100
        Left = 738
        Top = 439
        Width = 38
        Height = 22
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 5
        OnClick = RefCmdsBtnClick
      end
      object TimerGroupBox: TGroupBox
        Left = 10
        Top = 480
        Width = 870
        Height = 91
        Anchors = [akLeft, akTop, akRight]
        Caption = #12479#12452#12510#12540#12452#12505#12531#12488
        TabOrder = 7
        DesignSize = (
          870
          91)
        object TimerCmd1Edit: TLabeledEdit
          Left = 234
          Top = 32
          Width = 491
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 38
          EditLabel.Height = 15
          EditLabel.Caption = #12467#12510#12531#12489
          TabOrder = 1
          Text = ''
        end
        object RefCmdsT1Btn: TButton
          Tag = 101
          Left = 728
          Top = 31
          Width = 38
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 2
          OnClick = RefCmdsBtnClick
        end
        object EvEditT1Btn: TButton
          Tag = 1
          Left = 771
          Top = 30
          Width = 90
          Height = 24
          Action = EditFile1Action
          Anchors = [akTop, akRight]
          TabOrder = 3
        end
        object TimerCnd1Edit: TLabeledEdit
          Left = 10
          Top = 32
          Width = 220
          Height = 23
          EditLabel.Width = 26
          EditLabel.Height = 15
          EditLabel.Caption = #26465#20214
          TabOrder = 0
          Text = ''
        end
        object EvEditT2Btn: TButton
          Tag = 2
          Left = 771
          Top = 58
          Width = 90
          Height = 24
          Action = EditFile2Action
          Anchors = [akTop, akRight]
          TabOrder = 7
        end
        object RefCmdsT2Btn: TButton
          Tag = 102
          Left = 728
          Top = 59
          Width = 38
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 6
          OnClick = RefCmdsBtnClick
        end
        object TimerCnd2Edit: TEdit
          Left = 10
          Top = 60
          Width = 220
          Height = 23
          TabOrder = 4
        end
        object TimerCmd2Edit: TEdit
          Left = 234
          Top = 60
          Width = 491
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          TabOrder = 5
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 23
    Top = 621
    object AddExtColAction: TAction
      Caption = #36861#21152
      OnExecute = AddExtColActionExecute
      OnUpdate = AddExtColActionUpdate
    end
    object ChgExtColAction: TAction
      Caption = #22793#26356
      OnExecute = ChgExtColActionExecute
      OnUpdate = ChgExtColActionUpdate
    end
    object AddEtcEditorAction: TAction
      Caption = #36861#21152
      OnExecute = AddEtcEditorActionExecute
      OnUpdate = AddEtcEditorActionUpdate
    end
    object ChgEtcEditorAction: TAction
      Caption = #22793#26356
      OnExecute = ChgEtcEditorActionExecute
      OnUpdate = ChgEtcEditorActionUpdate
    end
    object AddToolAction: TAction
      Caption = #36861#21152
      OnExecute = AddToolActionExecute
      OnUpdate = ChgToolActionUpdate
    end
    object ChgToolAction: TAction
      Caption = #22793#26356
      OnExecute = ChgToolActionExecute
      OnUpdate = ChgToolActionUpdate
    end
    object AddMenuAction: TAction
      Caption = #36861#21152
      OnExecute = AddMenuActionExecute
      OnUpdate = ChgMenuActionUpdate
    end
    object ChgMenuAction: TAction
      Caption = #22793#26356
      OnExecute = ChgMenuActionExecute
      OnUpdate = ChgMenuActionUpdate
    end
    object AddAssoAction: TAction
      Caption = #36861#21152
      OnExecute = AddAssoActionExecute
      OnUpdate = ChgAssoActionUpdate
    end
    object ChgAssoAction: TAction
      Caption = #22793#26356
      OnExecute = ChgAssoActionExecute
      OnUpdate = ChgAssoActionUpdate
    end
    object AddKeyAction: TAction
      Caption = #36861#21152
      OnExecute = AddKeyActionExecute
      OnUpdate = AddKeyActionUpdate
    end
    object ChgKeyAction: TAction
      Caption = #22793#26356
      OnExecute = ChgKeyActionExecute
      OnUpdate = ChgKeyActionUpdate
    end
    object DelKeyAction: TAction
      Caption = #21066#38500
      OnExecute = DelKeyActionExecute
      OnUpdate = DelKeyActionUpdate
    end
    object AddDrvAction: TAction
      Caption = #36861#21152
      OnExecute = AddDrvActionExecute
      OnUpdate = AddDrvActionUpdate
    end
    object ChgDrvAction: TAction
      Caption = #22793#26356
      OnExecute = ChgDrvActionExecute
      OnUpdate = ChgDrvActionUpdate
    end
    object DelDrvAction: TAction
      Caption = #21066#38500
      OnExecute = DelDrvActionExecute
      OnUpdate = DelDrvActionUpdate
    end
    object AddStdCmdAction: TAction
      Caption = #36861#21152
      OnExecute = AddStdCmdActionExecute
      OnUpdate = AddStdCmdActionUpdate
    end
    object ChgStdCmdAction: TAction
      Caption = #22793#26356
      OnExecute = ChgStdCmdActionExecute
      OnUpdate = ChgStdCmdActionUpdate
    end
    object EditFileAction: TAction
      Caption = #12501#12449#12452#12523#32232#38598
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
    object EditFile1Action: TAction
      Tag = 1
      Caption = #12501#12449#12452#12523#32232#38598
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
    object EditFile2Action: TAction
      Tag = 2
      Caption = #12501#12449#12452#12523#32232#38598
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
    object DisableColAction: TAction
      Caption = #28961#21177
      OnExecute = DisableColActionExecute
      OnUpdate = DisableColActionUpdate
    end
    object DisableTimColAction: TAction
      Caption = #28961#21177
      OnExecute = DisableTimColActionExecute
      OnUpdate = DisableTimColActionUpdate
    end
    object DefTagColAction: TAction
      Caption = #12487#12501#12457#12523#12488
      OnExecute = DefTagColActionExecute
      OnUpdate = DefTagColActionUpdate
    end
    object FindKeyDownAction: TAction
      Caption = #9660
      Hint = #19979#26041#21521#12395#26908#32034
      OnExecute = FindKeyDownActionExecute
      OnUpdate = FindKeyDownActionUpdate
    end
    object FindKeyUpAction: TAction
      Caption = #9650
      Hint = #19978#26041#21521#12395#26908#32034
      OnExecute = FindKeyUpActionExecute
      OnUpdate = FindKeyUpActionUpdate
    end
    object AddPrtDirAction: TAction
      Caption = #36861#21152
      OnExecute = AddPrtDirActionExecute
    end
    object FindEvDownAction: TAction
      Caption = #9660
      Hint = #19979#26041#21521#12395#26908#32034
      OnExecute = FindEvDownActionExecute
      OnUpdate = FindEvDownActionUpdate
    end
    object FindEvUpAction: TAction
      Caption = #9650
      Hint = #19978#26041#21521#12395#26908#32034
      OnExecute = FindEvUpActionExecute
      OnUpdate = FindEvUpActionUpdate
    end
    object FindColDownAction: TAction
      Caption = #9660
      Hint = #19979#26041#21521#12395#26908#32034
      OnExecute = FindColDownActionExecute
      OnUpdate = FindColDownActionUpdate
    end
    object FindColUpAction: TAction
      Caption = #9650
      Hint = #19978#26041#21521#12395#26908#32034
      OnExecute = FindColUpActionExecute
      OnUpdate = FindColUpActionUpdate
    end
    object OkAction: TAction
      Caption = 'OK'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
  end
  object ToolPopupMenu: TPopupMenu
    Left = 107
    Top = 621
    object Frmt_F_Item: TMenuItem
      Caption = '$F '#12459#12524#12531#12488#12391#36984#25246#20013#12398#12497#12473#20184#12501#12449#12452#12523#21517
      OnClick = FrmtItemClick
    end
    object Frmt_B_Item: TMenuItem
      Caption = '$B $F '#12363#12425#12497#12473#21517#12434#38500#12356#12383#12501#12449#12452#12523#21517#37096#20998
      OnClick = FrmtItemClick
    end
    object Frmt_N_Item: TMenuItem
      Caption = '$N $B '#12363#12425#25313#24373#23376#12434#38500#12356#12383#12501#12449#12452#12523#21517#20027#37096
      OnClick = FrmtItemClick
    end
    object Frmt_O_Item: TMenuItem
      Caption = '$O '#21453#23550#20596#12391#36984#25246#20013#12398#12497#12473#20184#12501#12449#12452#12523#21517
      OnClick = FrmtItemClick
    end
    object Frmt_P_Item: TMenuItem
      Caption = '$P '#12459#12524#12531#12488#12497#12473#21517
      OnClick = FrmtItemClick
    end
    object Frmt_Q_Item: TMenuItem
      Caption = '$Q '#21453#23550#12497#12473#21517
      OnClick = FrmtItemClick
    end
    object Frmt_R_Item: TMenuItem
      Caption = '$R '#21453#23550#20596#12391#36984#25246#20013#12394#12425'$O'#12289#12394#12369#12428#12400'$Q\$B '#20182
      OnClick = FrmtItemClick
    end
    object Frmt_X_Item: TMenuItem
      Caption = '$X NyanFi'#12398#36215#21205#12497#12473
      OnClick = FrmtItemClick
    end
    object Frmt_D_Item: TMenuItem
      Caption = '$D NyanFi'#12398#36215#21205#12489#12521#12452#12502
      OnClick = FrmtItemClick
    end
  end
  object TestPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    AutoPopup = False
    Images = IconVImgListP
    OwnerDraw = True
    OnPopup = TestPopupMenuPopup
    Left = 204
    Top = 622
  end
  object IconVImgListP: TVirtualImageList
    AutoFill = True
    Images = <>
    ImageCollection = ImgCollectionP
    Left = 252
    Top = 622
  end
  object ImgCollectionP: TImageCollection
    Images = <>
    Left = 300
    Top = 622
  end
end
