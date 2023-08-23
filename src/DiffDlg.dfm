object DiffDirDlg: TDiffDirDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #12487#12451#12524#12463#12488#12522#12398#27604#36611
  ClientHeight = 242
  ClientWidth = 492
  Color = clBtnFace
  Constraints.MaxHeight = 280
  Constraints.MinHeight = 280
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
    242)
  TextHeight = 15
  object Label1: TLabel
    Left = 1
    Top = 77
    Width = 71
    Height = 18
    Alignment = taRightJustify
    AutoSize = False
    Caption = #23550#35937#12510#12473#12463
  end
  object TLabel
    Left = 10
    Top = 168
    Width = 62
    Height = 18
    Alignment = taRightJustify
    AutoSize = False
    Caption = #38500#22806#12510#12473#12463
  end
  object TLabel
    Left = 1
    Top = 106
    Width = 71
    Height = 18
    Alignment = taRightJustify
    AutoSize = False
    Caption = #38500#22806#12510#12473#12463
  end
  object SubDirCheckBox: TCheckBox
    Left = 22
    Top = 144
    Width = 236
    Height = 17
    Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12418#23550#35937#12392#12377#12427'(&S)'
    TabOrder = 4
  end
  object IncMaskComboBox: TComboBox
    Left = 76
    Top = 74
    Width = 390
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 2
    ExplicitWidth = 386
  end
  object SrcDirEdit: TLabeledEdit
    Left = 76
    Top = 8
    Width = 390
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 39
    EditLabel.Height = 21
    EditLabel.Caption = #27604#36611#20803
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 0
    Text = ''
    ExplicitWidth = 386
  end
  object DstDirEdit: TLabeledEdit
    Left = 76
    Top = 37
    Width = 390
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    BorderStyle = bsNone
    EditLabel.Width = 39
    EditLabel.Height = 21
    EditLabel.Caption = #27604#36611#20808
    LabelPosition = lpLeft
    ReadOnly = True
    TabOrder = 1
    Text = ''
    ExplicitWidth = 386
  end
  object DiffExcDirComboBox: TComboBox
    Left = 76
    Top = 165
    Width = 390
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 5
    ExplicitWidth = 386
  end
  object CanButton: TButton
    Left = 386
    Top = 209
    Width = 80
    Height = 26
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 7
    ExplicitLeft = 382
    ExplicitTop = 208
  end
  object OkButton: TButton
    Left = 294
    Top = 209
    Width = 80
    Height = 26
    Action = StartAction
    Anchors = [akRight, akBottom]
    Default = True
    ModalResult = 1
    TabOrder = 6
    ExplicitLeft = 290
    ExplicitTop = 208
  end
  object ExcMaskComboBox: TComboBox
    Left = 76
    Top = 103
    Width = 390
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 20
    TabOrder = 3
    ExplicitWidth = 386
  end
  object ActionList1: TActionList
    Left = 216
    Top = 200
    object StartAction: TAction
      Caption = #38283#22987
      OnUpdate = StartActionUpdate
    end
  end
end
