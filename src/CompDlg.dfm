object FileCompDlg: TFileCompDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #21516#21517#12501#12449#12452#12523#12398#27604#36611
  ClientHeight = 344
  ClientWidth = 434
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
  DesignSize = (
    434
    344)
  TextHeight = 15
  object CanButton: TButton
    Left = 229
    Top = 310
    Width = 80
    Height = 26
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 10
    ExplicitTop = 309
  end
  object OkButton: TButton
    Left = 124
    Top = 310
    Width = 80
    Height = 26
    Action = OkAction
    Anchors = [akLeft, akBottom]
    Default = True
    ModalResult = 1
    TabOrder = 9
    ExplicitTop = 309
  end
  object CmpDirCheckBox: TCheckBox
    Left = 12
    Top = 246
    Width = 154
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = #12487#12451#12524#12463#12488#12522#12418#27604#36611'(&D)'
    TabOrder = 4
    ExplicitTop = 245
  end
  object TimePanel: TPanel
    Left = 0
    Top = 0
    Width = 434
    Height = 55
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    StyleElements = []
    ExplicitWidth = 430
    DesignSize = (
      434
      55)
    object TimeRadioGroup: TRadioGroup
      Left = 8
      Top = 6
      Width = 406
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = #12479#12452#12512#12473#12479#12531#12503'(&T)'
      Columns = 5
      ItemIndex = 0
      Items.Strings = (
        #28961#35222
        #19981#19968#33268
        #19968#33268
        #26032#12375#12356
        #21476#12356)
      TabOrder = 0
      ExplicitWidth = 402
    end
  end
  object SizePanel: TPanel
    Left = 0
    Top = 55
    Width = 434
    Height = 55
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    StyleElements = []
    ExplicitWidth = 430
    DesignSize = (
      434
      55)
    object SizeRadioGroup: TRadioGroup
      Left = 9
      Top = 6
      Width = 405
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = #12469#12452#12474'(&S)'
      Columns = 5
      ItemIndex = 0
      Items.Strings = (
        #28961#35222
        #19981#19968#33268
        #19968#33268
        #22823#12365#12356
        #23567#12373#12356)
      TabOrder = 0
      ExplicitWidth = 401
    end
  end
  object HashPanel: TPanel
    Left = 0
    Top = 110
    Width = 434
    Height = 67
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    StyleElements = []
    ExplicitWidth = 430
    DesignSize = (
      434
      67)
    object HashRadioGroup: TRadioGroup
      Left = 8
      Top = 6
      Width = 406
      Height = 52
      Anchors = [akLeft, akTop, akRight]
      Caption = #12495#12483#12471#12517'(&H)'
      Columns = 5
      ItemIndex = 0
      Items.Strings = (
        #28961#35222
        #19981#19968#33268
        #19968#33268)
      TabOrder = 0
      OnClick = OptRadioGroupClick
      ExplicitWidth = 402
    end
    object AlgComboBox: TComboBox
      Left = 263
      Top = 25
      Width = 150
      Height = 23
      Style = csDropDownList
      TabOrder = 1
    end
  end
  object SelOppCheckBox: TCheckBox
    Left = 12
    Top = 271
    Width = 130
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = #21453#23550#20596#12418#36984#25246'(&O)'
    TabOrder = 6
    ExplicitTop = 270
  end
  object ReverseCheckBox: TCheckBox
    Left = 144
    Top = 271
    Width = 117
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = #32080#26524#12434#21453#36578'(&R)'
    TabOrder = 7
    ExplicitTop = 270
  end
  object SelMaskCheckBox: TCheckBox
    Left = 263
    Top = 271
    Width = 163
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = #36984#25246#38917#30446#12384#12369#12434#27531#12377'(&M)'
    TabOrder = 8
    ExplicitTop = 270
  end
  object CmpArcCheckBox: TCheckBox
    Left = 172
    Top = 246
    Width = 223
    Height = 17
    Anchors = [akLeft, akBottom]
    Caption = #12487#12451#12524#12463#12488#12522#12392#12450#12540#12459#12452#12502#12434#27604#36611'(&P)'
    TabOrder = 5
    ExplicitTop = 245
  end
  object IdPanel: TPanel
    Left = 0
    Top = 177
    Width = 434
    Height = 55
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 3
    StyleElements = []
    ExplicitWidth = 430
    DesignSize = (
      434
      55)
    object IdRadioGroup: TRadioGroup
      Left = 8
      Top = 6
      Width = 406
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = #12508#12522#12517#12540#12512#12471#12522#12450#12523#30058#21495'/'#12501#12449#12452#12523'ID(&I)'
      Columns = 5
      ItemIndex = 0
      Items.Strings = (
        #28961#35222
        #19981#19968#33268
        #19968#33268)
      TabOrder = 0
      OnClick = OptRadioGroupClick
      ExplicitWidth = 402
    end
  end
  object ActionList1: TActionList
    Left = 361
    Top = 294
    object OkAction: TAction
      Caption = 'OK'
      OnUpdate = OkActionUpdate
    end
  end
end
