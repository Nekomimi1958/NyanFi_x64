object PrintImgDlg: TPrintImgDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #30011#20687#12398#21360#21047
  ClientHeight = 372
  ClientWidth = 634
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
  object PrviewPanel: TPanel
    Left = 294
    Top = 0
    Width = 340
    Height = 372
    Align = alRight
    BevelOuter = bvNone
    Color = clAppWorkSpace
    ParentBackground = False
    TabOrder = 1
    StyleElements = []
    ExplicitLeft = 290
    ExplicitHeight = 371
    object SizeLabel: TLabel
      Left = 0
      Top = 336
      Width = 340
      Height = 36
      Align = alBottom
      Alignment = taCenter
      AutoSize = False
      Color = clBtnFace
      ParentColor = False
    end
    object PrvImage: TImage
      Left = 10
      Top = 10
      Width = 320
      Height = 320
    end
  end
  object LeftPanel: TPanel
    Left = 0
    Top = 0
    Width = 294
    Height = 372
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 290
    ExplicitHeight = 371
    DesignSize = (
      294
      372)
    object OkButton: TButton
      Left = 209
      Top = 340
      Width = 70
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #38281#12376#12427
      ModalResult = 1
      TabOrder = 7
      ExplicitTop = 339
    end
    object PageControl1: TPageControl
      Left = 8
      Top = 123
      Width = 281
      Height = 212
      ActivePage = TabSheet1
      OwnerDraw = True
      TabOrder = 1
      TabWidth = 64
      OnDrawTab = PageControl1DrawTab
      object TabSheet1: TTabSheet
        Caption = #22522#26412
        object Panel1: TPanel
          Left = 0
          Top = 0
          Width = 273
          Height = 182
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object GrayCheckBox: TCheckBox
            Left = 14
            Top = 160
            Width = 104
            Height = 17
            Caption = #12464#12524#12540#12473#12465#12540#12523
            TabOrder = 7
            OnClick = OptionChanged
          end
          object OptRadioGroup: TRadioGroup
            Left = 4
            Top = 8
            Width = 262
            Height = 144
            Caption = #12469#12452#12474#12289#20301#32622
            Items.Strings = (
              #29992#32025#12395#21512#12431#12379#12427
              #29992#32025#12469#12452#12474#12391#12488#12522#12511#12531#12464
              #20013#22830
              #24038#19978)
            TabOrder = 0
            OnClick = OptionChanged
          end
          object OfsYEdit: TLabeledEdit
            Left = 205
            Top = 120
            Width = 40
            Height = 23
            Alignment = taRightJustify
            EditLabel.Width = 7
            EditLabel.Height = 23
            EditLabel.Caption = 'Y'
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 5
            Text = '0'
            OnClick = OptionChanged
          end
          object OfsXEdit: TLabeledEdit
            Left = 133
            Top = 120
            Width = 40
            Height = 23
            Alignment = taRightJustify
            EditLabel.Width = 57
            EditLabel.Height = 23
            EditLabel.Caption = #12458#12501#12475#12483#12488' X'
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 3
            Text = '0'
            OnClick = OptionChanged
          end
          object ScaleEdit: TLabeledEdit
            Left = 133
            Top = 90
            Width = 40
            Height = 23
            Alignment = taRightJustify
            EditLabel.Width = 42
            EditLabel.Height = 23
            EditLabel.Caption = #12473#12465#12540#12523
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 1
            Text = '100'
            OnClick = OptionChanged
          end
          object OfsXUpDown: TUpDown
            Left = 173
            Top = 120
            Width = 16
            Height = 23
            Associate = OfsXEdit
            Max = 99
            TabOrder = 4
            OnClick = OptionUpDownClick
          end
          object OfsYUpDown: TUpDown
            Left = 245
            Top = 120
            Width = 16
            Height = 23
            Associate = OfsYEdit
            Max = 99
            TabOrder = 6
            OnClick = OptionUpDownClick
          end
          object ScaleUpDown: TUpDown
            Left = 173
            Top = 90
            Width = 16
            Height = 23
            Associate = ScaleEdit
            Min = 1
            Position = 100
            TabOrder = 2
            OnClick = OptionUpDownClick
          end
        end
      end
      object TabSheet2: TTabSheet
        Caption = #25991#23383
        ImageIndex = 1
        object Panel2: TPanel
          Left = 0
          Top = 0
          Width = 273
          Height = 184
          Align = alClient
          BevelOuter = bvNone
          TabOrder = 0
          object StrFmtEdit: TLabeledEdit
            Left = 8
            Top = 103
            Width = 258
            Height = 21
            EditLabel.Width = 26
            EditLabel.Height = 15
            EditLabel.Caption = #26360#24335
            PopupMenu = UserModule.EditPopupMenuE
            TabOrder = 5
            Text = ''
            OnChange = OptionChanged
          end
          object RefFontBtn: TButton
            Left = 226
            Top = 148
            Width = 40
            Height = 24
            Caption = '...'
            TabOrder = 7
            OnClick = RefFontBtnClick
          end
          object StrFontEdit: TLabeledEdit
            Left = 8
            Top = 149
            Width = 216
            Height = 21
            TabStop = False
            BorderStyle = bsNone
            Color = clSilver
            EditLabel.Width = 37
            EditLabel.Height = 15
            EditLabel.Caption = #12501#12457#12531#12488
            ReadOnly = True
            TabOrder = 6
            Text = ''
          end
          object StrCheckBox: TCheckBox
            Left = 8
            Top = 8
            Width = 76
            Height = 17
            Caption = #26377#21177
            TabOrder = 0
            OnClick = OptionChanged
          end
          object StrMgnEdit: TLabeledEdit
            Left = 210
            Top = 8
            Width = 40
            Height = 21
            Alignment = taRightJustify
            EditLabel.Width = 42
            EditLabel.Height = 21
            EditLabel.Caption = #12510#12540#12472#12531
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 1
            Text = '0'
            OnClick = OptionChanged
          end
          object StrPosRadioGroup: TRadioGroup
            Left = 8
            Top = 36
            Width = 105
            Height = 41
            Caption = #20301#32622
            Columns = 2
            Items.Strings = (
              #19978
              #19979)
            TabOrder = 3
            OnClick = OptionChanged
          end
          object StrAlRadioGroup: TRadioGroup
            Left = 117
            Top = 36
            Width = 149
            Height = 41
            Caption = #25539#12360
            Columns = 3
            Items.Strings = (
              #24038
              #20013
              #21491)
            TabOrder = 4
            OnClick = OptionChanged
          end
          object StrMgnUpDown: TUpDown
            Left = 250
            Top = 8
            Width = 16
            Height = 21
            Associate = StrMgnEdit
            Max = 99
            TabOrder = 2
            OnClick = OptionUpDownClick
          end
        end
      end
    end
    object PrnButton: TButton
      Left = 14
      Top = 340
      Width = 70
      Height = 26
      Action = PrintAction
      Anchors = [akLeft, akBottom]
      Default = True
      TabOrder = 2
      ExplicitTop = 339
    end
    object PrnGroupBox: TGroupBox
      Left = 8
      Top = 8
      Width = 277
      Height = 105
      Caption = #12503#12522#12531#12479#12540
      TabOrder = 0
      object PrnNameLabel: TLabel
        Left = 8
        Top = 20
        Width = 3
        Height = 15
      end
      object PrnPropBtn: TButton
        Left = 12
        Top = 42
        Width = 253
        Height = 26
        Caption = #12503#12522#12531#12479#12540#12398#35373#23450'(&S)...'
        TabOrder = 0
        OnClick = PrnPropBtnClick
      end
      object CpsUpDown: TUpDown
        Left = 100
        Top = 75
        Width = 16
        Height = 23
        Associate = PrnCpsEdit
        Min = 1
        Position = 1
        TabOrder = 2
      end
      object PrnCpsEdit: TLabeledEdit
        Left = 60
        Top = 75
        Width = 40
        Height = 23
        Alignment = taRightJustify
        EditLabel.Width = 42
        EditLabel.Height = 23
        EditLabel.Caption = #37096#25968'(&C)'
        LabelPosition = lpLeft
        NumbersOnly = True
        TabOrder = 1
        Text = '1'
      end
      object OriVBtn: TRadioButton
        Left = 161
        Top = 77
        Width = 45
        Height = 17
        Caption = #32294
        TabOrder = 3
        OnClick = OptionChanged
      end
      object OriHBtn: TRadioButton
        Tag = 1
        Left = 208
        Top = 77
        Width = 45
        Height = 17
        Caption = #27178
        TabOrder = 4
        OnClick = OptionChanged
      end
    end
    object TopBtn: TButton
      Left = 92
      Top = 340
      Width = 26
      Height = 26
      Caption = '|<'
      TabOrder = 3
      OnClick = TopBtnClick
    end
    object PrevBtn: TButton
      Left = 120
      Top = 340
      Width = 26
      Height = 26
      Caption = '<'
      TabOrder = 4
      OnClick = PrevBtn_Click
    end
    object NextBtn: TButton
      Left = 148
      Top = 340
      Width = 26
      Height = 26
      Caption = '>'
      TabOrder = 5
      OnClick = NextBtnClick
    end
    object EndBtn: TButton
      Left = 176
      Top = 340
      Width = 26
      Height = 26
      Caption = '>|'
      TabOrder = 6
      OnClick = EndBtnClick
    end
  end
  object PrinterSetupDialog1: TPrinterSetupDialog
    Left = 352
    Top = 24
  end
  object ActionList1: TActionList
    Left = 352
    Top = 90
    object PrintAction: TAction
      Caption = #21360#21047'(&P)'
      OnExecute = PrintActionExecute
      OnUpdate = PrintActionUpdate
    end
  end
end
