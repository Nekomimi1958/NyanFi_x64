object BackupDlg: TBackupDlg
  Left = 0
  Top = 0
  HelpContext = 71
  BorderIcons = [biSystemMenu]
  Caption = #12496#12483#12463#12450#12483#12503
  ClientHeight = 362
  ClientWidth = 492
  Color = clBtnFace
  Constraints.MaxHeight = 400
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
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
  DesignSize = (
    492
    362)
  TextHeight = 15
  object Label1: TLabel
    Left = 13
    Top = 77
    Width = 71
    Height = 18
    Alignment = taRightJustify
    AutoSize = False
    Caption = #23550#35937#12510#12473#12463
  end
  object TLabel
    Left = 13
    Top = 106
    Width = 71
    Height = 18
    Alignment = taRightJustify
    AutoSize = False
    Caption = #38500#22806#12510#12473#12463
  end
  object SyncLabel: TLabel
    Left = 331
    Top = 235
    Width = 141
    Height = 15
    AutoSize = False
    Caption = '(  :   )'
  end
  object SubDirCheckBox: TCheckBox
    Left = 16
    Top = 138
    Width = 236
    Height = 17
    Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12418#23550#35937#12392#12377#12427
    TabOrder = 4
  end
  object MirrorCheckBox: TCheckBox
    Left = 16
    Top = 235
    Width = 151
    Height = 17
    Caption = #12511#12521#12540#12522#12531#12464#12434#34892#12358
    TabOrder = 7
  end
  object BakIncMaskComboBox: TComboBox
    Left = 88
    Top = 74
    Width = 376
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 2
    ExplicitWidth = 372
  end
  object BakSkipDirEdit: TLabeledEdit
    Left = 88
    Top = 163
    Width = 376
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    EditLabel.Width = 55
    EditLabel.Height = 22
    EditLabel.Caption = #38500#22806#12510#12473#12463
    LabelPosition = lpLeft
    TabOrder = 5
    Text = ''
    ExplicitWidth = 372
  end
  object SrcDirEdit: TLabeledEdit
    Left = 88
    Top = 8
    Width = 376
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 69
    EditLabel.Height = 21
    EditLabel.Caption = #12496#12483#12463#12450#12483#12503#20803
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 0
    Text = ''
    ExplicitWidth = 372
  end
  object DstDirEdit: TLabeledEdit
    Left = 88
    Top = 37
    Width = 376
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 69
    EditLabel.Height = 21
    EditLabel.Caption = #12496#12483#12463#12450#12483#12503#20808
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 1
    Text = ''
    ExplicitWidth = 372
  end
  object BakExcMaskComboBox: TComboBox
    Left = 88
    Top = 103
    Width = 376
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 3
    ExplicitWidth = 372
  end
  object SyncCheckBox: TCheckBox
    Left = 171
    Top = 235
    Width = 158
    Height = 17
    Caption = #21516#26399#12496#12483#12463#12450#12483#12503#26377#21177
    TabOrder = 8
  end
  object BakDateCondEdit: TLabeledEdit
    Left = 88
    Top = 197
    Width = 150
    Height = 22
    AutoSize = False
    EditLabel.Width = 52
    EditLabel.Height = 22
    EditLabel.Caption = #26085#20184#26465#20214
    LabelPosition = lpLeft
    TabOrder = 6
    Text = ''
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 262
    Width = 492
    Height = 100
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 9
    ExplicitTop = 261
    ExplicitWidth = 488
    DesignSize = (
      492
      100)
    object TLabel
      Left = 26
      Top = 22
      Width = 26
      Height = 15
      Alignment = taRightJustify
      Caption = #35373#23450
    end
    object OkButton: TButton
      Left = 292
      Top = 66
      Width = 80
      Height = 26
      Anchors = [akRight, akBottom]
      Caption = #38283#22987
      Default = True
      TabOrder = 4
      OnClick = OkButtonClick
      ExplicitLeft = 288
    end
    object CanButton: TButton
      Left = 384
      Top = 66
      Width = 80
      Height = 26
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 5
      ExplicitLeft = 380
    end
    object SureCheckBox: TCheckBox
      Left = 183
      Top = 70
      Width = 97
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = #38283#22987#30906#35469
      TabOrder = 3
      ExplicitLeft = 179
    end
    object SetupComboBox: TComboBox
      Left = 58
      Top = 19
      Width = 180
      Height = 23
      PopupMenu = SetupPopupMenu
      TabOrder = 0
      OnClick = SetupComboBoxClick
    end
    object SaveCmdBtn: TButton
      Left = 304
      Top = 18
      Width = 172
      Height = 24
      Action = MakeNbtAction
      TabOrder = 2
    end
    object SaveFmtBtn: TButton
      Left = 240
      Top = 18
      Width = 50
      Height = 24
      Action = SaveSetupAction
      TabOrder = 1
    end
    object BevelPanel1: TPanel
      Left = 8
      Top = 4
      Width = 464
      Height = 2
      Anchors = [akLeft, akTop, akRight]
      BevelOuter = bvNone
      TabOrder = 6
      ExplicitWidth = 460
      object Shape1: TShape
        Left = 0
        Top = 0
        Width = 464
        Height = 1
        Align = alTop
        Brush.Style = bsClear
        Pen.Color = clBtnShadow
        ExplicitWidth = 476
      end
      object Shape2: TShape
        Left = 0
        Top = 1
        Width = 464
        Height = 1
        Align = alTop
        Brush.Style = bsClear
        Pen.Color = clBtnHighlight
        ExplicitWidth = 476
      end
    end
  end
  object ActionList1: TActionList
    Left = 424
    Top = 194
    object SaveSetupAction: TAction
      Caption = #20445#23384
      OnExecute = SaveSetupActionExecute
      OnUpdate = SaveSetupActionUpdate
    end
    object MakeNbtAction: TAction
      Caption = #12467#12510#12531#12489#12501#12449#12452#12523#12392#12375#12390#20445#23384'...'
      OnExecute = MakeNbtActionExecute
      OnUpdate = MakeNbtActionUpdate
    end
    object DelSetupAction: TAction
      Caption = #12371#12398#38917#30446#12434#21066#38500'(&D)'
      OnExecute = DelSetupActionExecute
      OnUpdate = DelSetupActionUpdate
    end
  end
  object SetupPopupMenu: TPopupMenu
    Left = 80
    Top = 310
    object DelSetupItem: TMenuItem
      Action = DelSetupAction
    end
  end
end
