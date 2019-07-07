object GrepExOptDlg: TGrepExOptDlg
  Left = 0
  Top = 0
  HelpContext = 56
  BorderStyle = bsDialog
  Caption = #25313#24373#35373#23450
  ClientHeight = 365
  ClientWidth = 514
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnHelp = FormHelp
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 514
    Height = 319
    ActivePage = OutModeSheet
    Align = alClient
    OwnerDraw = True
    TabOrder = 0
    TabWidth = 100
    OnDrawTab = PageControl1DrawTab
    object OutModeSheet: TTabSheet
      Caption = #20986#21147#26041#27861
      ImageIndex = -1
      object GrepOpt1Panel: TPanel
        Left = 0
        Top = 0
        Width = 506
        Height = 291
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          506
          291)
        object AppGroupBox: TGroupBox
          Left = 8
          Top = 158
          Width = 488
          Height = 110
          Anchors = [akLeft, akTop, akRight]
          Caption = #36215#21205#12450#12503#12522#12465#12540#12471#12519#12531
          TabOrder = 2
          DesignSize = (
            488
            110)
          object RefAppBtn: TButton
            Left = 444
            Top = 19
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 2
            OnClick = RefAppBtnClick
          end
          object AppParamEdit: TLabeledEdit
            Left = 101
            Top = 48
            Width = 339
            Height = 21
            Hint = '$F :'#20986#21147#12501#12449#12452#12523#21517'/  $L :'#36861#21152#34892#30058#21495
            Anchors = [akLeft, akTop, akRight]
            EditLabel.Width = 44
            EditLabel.Height = 13
            EditLabel.Caption = #12497#12521#12513#12540#12479
            LabelPosition = lpLeft
            TabOrder = 3
          end
          object AppDirEdit: TLabeledEdit
            Left = 101
            Top = 76
            Width = 339
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            EditLabel.Width = 73
            EditLabel.Height = 13
            EditLabel.Caption = #20316#26989#12487#12451#12524#12463#12488#12522
            LabelPosition = lpLeft
            TabOrder = 4
          end
          object RefDirBtn: TButton
            Left = 444
            Top = 75
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 5
            OnClick = RefDirBtnClick
          end
          object AppEnabledCheckBox: TCheckBox
            Left = 12
            Top = 22
            Width = 87
            Height = 17
            Caption = #23455#34892#12501#12449#12452#12523
            TabOrder = 0
          end
          object AppNameEdit: TEdit
            Left = 101
            Top = 20
            Width = 339
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            TabOrder = 1
          end
        end
        object FileGroupBox: TGroupBox
          Left = 8
          Top = 65
          Width = 488
          Height = 77
          Anchors = [akLeft, akTop, akRight]
          Caption = #20986#21147#12501#12449#12452#12523
          TabOrder = 1
          DesignSize = (
            488
            77)
          object OutFileEdit: TLabeledEdit
            Left = 101
            Top = 20
            Width = 339
            Height = 21
            Anchors = [akLeft, akTop, akRight]
            EditLabel.Width = 46
            EditLabel.Height = 13
            EditLabel.Caption = #12501#12449#12452#12523#21517
            LabelPosition = lpLeft
            TabOrder = 0
          end
          object RefOutBtn: TButton
            Left = 444
            Top = 19
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 1
            OnClick = RefOutBtnClick
          end
          object AppendCheckBox: TCheckBox
            Left = 101
            Top = 52
            Width = 304
            Height = 17
            Caption = #26082#23384#12501#12449#12452#12523#12364#12354#12428#12400#36861#21152
            TabOrder = 2
          end
        end
        object OutModeGroup: TGroupBox
          Left = 8
          Top = 8
          Width = 488
          Height = 45
          Caption = #20986#21147#20808
          TabOrder = 0
          object OutMode0Btn: TRadioButton
            Left = 8
            Top = 18
            Width = 80
            Height = 17
            Caption = #28961#12375
            TabOrder = 0
          end
          object OutMode1Btn: TRadioButton
            Left = 117
            Top = 18
            Width = 80
            Height = 17
            Caption = #12501#12449#12452#12523
            TabOrder = 1
          end
          object OutMode2Btn: TRadioButton
            Left = 227
            Top = 18
            Width = 110
            Height = 17
            Caption = #12463#12522#12483#12503#12508#12540#12489
            TabOrder = 2
          end
        end
      end
    end
    object ReplaceSheet: TTabSheet
      Caption = #32622#25563
      ImageIndex = -1
      object GrepOpt2Panel: TPanel
        Left = 0
        Top = 0
        Width = 506
        Height = 291
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        DesignSize = (
          506
          291)
        object BakGroupBox: TGroupBox
          Left = 8
          Top = 8
          Width = 488
          Height = 101
          Anchors = [akLeft, akTop, akRight]
          Caption = #20803#12501#12449#12452#12523#12398#12496#12483#12463#12450#12483#12503
          TabOrder = 0
          DesignSize = (
            488
            101)
          object BakRepCheckBox: TCheckBox
            Left = 21
            Top = 24
            Width = 172
            Height = 17
            Caption = #12496#12483#12463#12450#12483#12503#12434#20316#25104#12377#12427
            TabOrder = 0
          end
          object FExtBakEdit: TLabeledEdit
            Left = 341
            Top = 37
            Width = 100
            Height = 21
            EditLabel.Width = 36
            EditLabel.Height = 13
            EditLabel.Caption = #25313#24373#23376
            LabelPosition = lpLeft
            TabOrder = 1
          end
          object RepBakDirEdit: TLabeledEdit
            Left = 64
            Top = 65
            Width = 377
            Height = 21
            EditLabel.Width = 36
            EditLabel.Height = 13
            EditLabel.Caption = #20445#23384#20808
            LabelPosition = lpLeft
            TabOrder = 2
          end
          object RefBakBtn: TButton
            Left = 445
            Top = 64
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 3
            OnClick = RefBakBtnClick
          end
        end
        object LogGroupBox: TGroupBox
          Left = 8
          Top = 122
          Width = 488
          Height = 145
          Anchors = [akLeft, akTop, akRight]
          Caption = #32080#26524#12525#12464
          TabOrder = 1
          DesignSize = (
            488
            145)
          object LogFileEdit: TLabeledEdit
            Left = 97
            Top = 44
            Width = 344
            Height = 21
            EditLabel.Width = 46
            EditLabel.Height = 13
            EditLabel.Caption = #12501#12449#12452#12523#21517
            LabelPosition = lpLeft
            TabOrder = 1
          end
          object RefLogBtn: TButton
            Left = 445
            Top = 44
            Width = 36
            Height = 22
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 2
            OnClick = RefLogBtnClick
          end
          object AppendLogCheckBox: TCheckBox
            Left = 97
            Top = 76
            Width = 304
            Height = 17
            Caption = #26082#23384#12501#12449#12452#12523#12364#12354#12428#12400#36861#21152
            TabOrder = 3
          end
          object SaveLogCheckBox: TCheckBox
            Left = 16
            Top = 20
            Width = 289
            Height = 17
            Caption = #32622#25563#32080#26524#12434#12501#12449#12452#12523#12395#20445#23384#12377#12427
            TabOrder = 0
          end
          object OpenLogCheckBox: TCheckBox
            Left = 16
            Top = 110
            Width = 289
            Height = 17
            Caption = #20445#23384#12375#12383#12501#12449#12452#12523#12434#12486#12461#12473#12488#12456#12487#12451#12479#12391#38283#12367
            TabOrder = 4
          end
        end
      end
    end
    object OutFormSheet: TTabSheet
      Caption = #20986#21147#24418#24335
      ImageIndex = -1
      object GrepOpt3Panel: TPanel
        Left = 0
        Top = 0
        Width = 506
        Height = 291
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        object Label1: TLabel
          Left = 172
          Top = 33
          Width = 321
          Height = 13
          Alignment = taRightJustify
          Caption = '$F '#12501#12449#12452#12523#21517'/  $B '#12497#12473#28961#12375#12501#12449#12452#12523#21517'/  $L '#34892#30058#21495'/  \t '#12479#12502'/  \n '#25913#34892
        end
        object Label2: TLabel
          Left = 16
          Top = 158
          Width = 62
          Height = 13
          Caption = #12469#12531#12503#12523#34920#31034
        end
        object InsStrWEdit: TLabeledEdit
          Left = 180
          Top = 59
          Width = 120
          Height = 21
          EditLabel.Width = 131
          EditLabel.Height = 13
          EditLabel.Caption = #12510#12483#12481#35486#21069#24460#12398#25407#20837#25991#23383#21015
          LabelPosition = lpLeft
          TabOrder = 1
          OnChange = SampleChange
        end
        object InsStrW2Edit: TLabeledEdit
          Left = 339
          Top = 59
          Width = 120
          Height = 21
          EditLabel.Width = 20
          EditLabel.Height = 13
          EditLabel.Caption = #65374#12288
          LabelPosition = lpLeft
          TabOrder = 2
          OnChange = SampleChange
        end
        object FileFmtEdit: TLabeledEdit
          Left = 112
          Top = 8
          Width = 381
          Height = 21
          EditLabel.Width = 92
          EditLabel.Height = 13
          EditLabel.Caption = #12501#12449#12452#12523#24773#22577#12398#26360#24335
          LabelPosition = lpLeft
          TabOrder = 0
          OnChange = SampleChange
        end
        object SampleMemo: TMemo
          Left = 16
          Top = 176
          Width = 477
          Height = 105
          TabStop = False
          BevelInner = bvNone
          BevelOuter = bvNone
          Lines.Strings = (
            '')
          ReadOnly = True
          TabOrder = 7
          WantReturns = False
        end
        object TrimLeftCheckBox: TCheckBox
          Left = 16
          Top = 95
          Width = 180
          Height = 17
          Caption = #34892#38957#12398#12479#12502#12420#31354#30333#12434#21066#38500
          TabOrder = 3
          OnClick = SampleChange
        end
        object RepTabCheckBox: TCheckBox
          Left = 200
          Top = 95
          Width = 179
          Height = 17
          Caption = #12479#12502#12434#31354#30333'1'#25991#23383#12395#32622#25563
          TabOrder = 4
          OnClick = SampleChange
        end
        object RepCrCheckBox: TCheckBox
          Left = 16
          Top = 126
          Width = 147
          Height = 17
          Caption = #25913#34892#12434#25991#23383#21015#12395#32622#25563
          TabOrder = 5
          OnClick = SampleChange
        end
        object RepCrEdit: TEdit
          Left = 180
          Top = 123
          Width = 120
          Height = 21
          TabOrder = 6
          OnChange = SampleChange
        end
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 319
    Width = 514
    Height = 46
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object CanButton: TButton
      Left = 269
      Top = 10
      Width = 80
      Height = 26
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object OkButton: TButton
      Left = 164
      Top = 10
      Width = 80
      Height = 26
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
      OnClick = OkButtonClick
    end
  end
end
