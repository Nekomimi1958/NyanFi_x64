object BackupDlg: TBackupDlg
  Left = 0
  Top = 0
  HelpContext = 71
  BorderIcons = [biSystemMenu]
  Caption = #12496#12483#12463#12450#12483#12503
  ClientHeight = 368
  ClientWidth = 492
  Color = clBtnFace
  Constraints.MaxHeight = 400
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
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
    492
    368)
  PixelsPerInch = 96
  TextHeight = 13
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
    Width = 388
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 2
  end
  object BakSkipDirEdit: TLabeledEdit
    Left = 88
    Top = 163
    Width = 388
    Height = 22
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    EditLabel.Width = 50
    EditLabel.Height = 13
    EditLabel.Caption = #38500#22806#12510#12473#12463
    LabelPosition = lpLeft
    TabOrder = 5
  end
  object SrcDirEdit: TLabeledEdit
    Left = 88
    Top = 8
    Width = 388
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 62
    EditLabel.Height = 13
    EditLabel.Caption = #12496#12483#12463#12450#12483#12503#20803
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 0
  end
  object DstDirEdit: TLabeledEdit
    Left = 88
    Top = 37
    Width = 388
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 62
    EditLabel.Height = 13
    EditLabel.Caption = #12496#12483#12463#12450#12483#12503#20808
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 1
  end
  object BakExcMaskComboBox: TComboBox
    Left = 88
    Top = 103
    Width = 388
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 3
  end
  object SyncCheckBox: TCheckBox
    Left = 170
    Top = 235
    Width = 176
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
    EditLabel.Width = 48
    EditLabel.Height = 13
    EditLabel.Caption = #26085#20184#26465#20214
    LabelPosition = lpLeft
    TabOrder = 6
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 268
    Width = 492
    Height = 100
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 9
    DesignSize = (
      492
      100)
    object SaveFmtBtn: TSpeedButton
      Tag = 2
      Left = 240
      Top = 17
      Width = 50
      Height = 24
      Action = SaveSetupAction
    end
    object TLabel
      Left = 28
      Top = 22
      Width = 24
      Height = 13
      Alignment = taRightJustify
      Caption = #35373#23450
    end
    object Bevel1: TBevel
      Left = 8
      Top = 4
      Width = 468
      Height = 4
      Anchors = [akLeft, akTop, akRight]
      Shape = bsBottomLine
    end
    object OkButton: TButton
      Left = 304
      Top = 66
      Width = 80
      Height = 26
      Anchors = [akRight, akBottom]
      Caption = #38283#22987
      Default = True
      TabOrder = 3
      OnClick = OkButtonClick
    end
    object CanButton: TButton
      Left = 396
      Top = 66
      Width = 80
      Height = 26
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 4
    end
    object SureCheckBox: TCheckBox
      Left = 195
      Top = 70
      Width = 97
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = #38283#22987#30906#35469
      TabOrder = 2
    end
    object SetupComboBox: TComboBox
      Left = 58
      Top = 19
      Width = 180
      Height = 21
      TabOrder = 0
      OnClick = SetupComboBoxClick
    end
    object Button1: TButton
      Left = 304
      Top = 17
      Width = 172
      Height = 24
      Action = MakeNbtAction
      TabOrder = 1
    end
  end
  object ActionList1: TActionList
    Left = 376
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
  end
end
