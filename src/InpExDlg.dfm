object InputExDlg: TInputExDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 232
  ClientWidth = 594
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
  object InputPanel: TPanel
    Left = 0
    Top = 0
    Width = 594
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 590
    object NameLabel: TLabel
      Left = 16
      Top = 13
      Width = 26
      Height = 15
      Alignment = taRightJustify
      Caption = #21517#21069
    end
    object InputEdit: TLabeledEdit
      Left = 45
      Top = 10
      Width = 341
      Height = 23
      EditLabel.Width = 26
      EditLabel.Height = 23
      EditLabel.Caption = #21517#21069
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
      Text = ''
      OnKeyDown = InputEditKeyDown
      OnKeyPress = InputEditKeyPress
    end
    object InputComboBox: TComboBox
      Left = 45
      Top = 10
      Width = 341
      Height = 23
      PopupMenu = UserModule.EditPopupMenuC
      TabOrder = 1
      OnChange = InputComboBoxChange
    end
  end
  object NewTextPanel: TPanel
    Left = 0
    Top = 41
    Width = 594
    Height = 54
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    Visible = False
    ExplicitWidth = 590
    object CodePageLabel: TLabel
      Left = 228
      Top = 2
      Width = 128
      Height = 20
      AutoSize = False
      Caption = #25991#23383#12467#12540#12489'(&C)'
      FocusControl = CodePageComboBox
      WordWrap = True
    end
    object CodePageComboBox: TComboBox
      Left = 228
      Top = 24
      Width = 158
      Height = 23
      Style = csDropDownList
      TabOrder = 2
    end
    object EditCheckBox: TCheckBox
      Left = 15
      Top = 27
      Width = 156
      Height = 17
      Caption = #12456#12487#12451#12479#12391#38283#12367'(&E)'
      TabOrder = 1
      OnClick = OptionClick
    end
    object ClipPanel: TPanel
      Left = 15
      Top = 4
      Width = 186
      Height = 17
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 0
      object ClipCheckBox: TCheckBox
        Left = 0
        Top = 0
        Width = 186
        Height = 17
        Caption = #12463#12522#12483#12503#12508#12540#12489#12363#12425#36028#12426#20184#12369'(&P)'
        TabOrder = 0
        OnClick = OptionClick
      end
    end
  end
  object CreDirPanel: TPanel
    Left = 0
    Top = 95
    Width = 594
    Height = 52
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 590
    object CnvChCheckBox: TCheckBox
      Left = 203
      Top = 26
      Width = 180
      Height = 17
      Caption = #25991#23383#32622#25563#12434#36969#29992'(&R)'
      TabOrder = 1
      OnClick = OptionClick
    end
    object DirChgCheckBox: TCheckBox
      Left = 15
      Top = 26
      Width = 180
      Height = 17
      Caption = #20316#25104#24460#12395#12459#12524#12531#12488#22793#26356'(&C)'
      TabOrder = 0
      OnClick = OptionClick
    end
    object DirSttPanel: TPanel
      Left = 0
      Top = 0
      Width = 594
      Height = 20
      Align = alTop
      BevelOuter = bvNone
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 2
      ExplicitWidth = 590
      DesignSize = (
        594
        20)
      object NameInfLabel: TLabel
        Left = 219
        Top = 0
        Width = 160
        Height = 18
        Anchors = [akLeft, akBottom]
        AutoSize = False
        Caption = #21517#21069#12398#25991#23383#25968' = '
      end
      object PathInfLabel: TLabel
        Left = 45
        Top = 0
        Width = 170
        Height = 18
        Anchors = [akLeft, akBottom]
        AutoSize = False
        Caption = #12501#12523#12497#12473#21517#12398#25991#23383#25968' = '
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 191
    Width = 594
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
    ExplicitTop = 190
    ExplicitWidth = 590
    DesignSize = (
      594
      41)
    object CancelBtn: TButton
      Left = 507
      Top = 8
      Width = 75
      Height = 26
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 2
      ExplicitLeft = 503
    end
    object OkBtn: TButton
      Left = 416
      Top = 8
      Width = 75
      Height = 26
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 1
      ExplicitLeft = 412
    end
    object NotationPanel: TPanel
      Left = 200
      Top = 0
      Width = 200
      Height = 41
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 0
      object DecRadioBtn: TRadioButton
        Left = 99
        Top = 8
        Width = 72
        Height = 17
        Caption = '10'#36914'(&D)'
        TabOrder = 1
        OnClick = OptionClick
      end
      object HexRadioBtn: TRadioButton
        Left = 15
        Top = 8
        Width = 72
        Height = 17
        Caption = '16'#36914'(&H)'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = OptionClick
      end
    end
    object IniSttPanel: TPanel
      Left = 0
      Top = 0
      Width = 200
      Height = 41
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 3
      object SelDefCheckBox: TCheckBox
        Left = 15
        Top = 8
        Width = 180
        Height = 17
        Caption = #12487#12501#12457#12523#12488#12434#36984#25246#29366#24907#12395'(&S)'
        TabOrder = 0
        OnClick = SelDefCheckBoxClick
      end
    end
  end
  object CreTestPanel: TPanel
    Left = 0
    Top = 147
    Width = 594
    Height = 32
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    ExplicitWidth = 590
    object TestSizeEdit: TLabeledEdit
      Left = 95
      Top = 6
      Width = 120
      Height = 23
      Alignment = taRightJustify
      EditLabel.Width = 31
      EditLabel.Height = 23
      EditLabel.Caption = #12469#12452#12474
      LabelPosition = lpLeft
      TabOrder = 0
      Text = ''
    end
    object TestCntEdit: TLabeledEdit
      Left = 271
      Top = 6
      Width = 60
      Height = 23
      Alignment = taRightJustify
      EditLabel.Width = 26
      EditLabel.Height = 23
      EditLabel.Caption = #20491#25968
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 1
      Text = ''
    end
  end
end
