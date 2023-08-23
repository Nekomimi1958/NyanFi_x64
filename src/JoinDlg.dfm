object JoinTextDlg: TJoinTextDlg
  Left = 0
  Top = 0
  HelpContext = 78
  BorderIcons = [biSystemMenu]
  Caption = #12486#12461#12473#12488#12501#12449#12452#12523#12398#32080#21512
  ClientHeight = 391
  ClientWidth = 370
  Color = clBtnFace
  Constraints.MinHeight = 420
  Constraints.MinWidth = 380
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
    370
    391)
  TextHeight = 15
  object SrcFileListBox: TListBox
    Left = 8
    Top = 8
    Width = 291
    Height = 210
    Style = lbOwnerDrawFixed
    Anchors = [akLeft, akTop, akRight, akBottom]
    DragMode = dmAutomatic
    ItemHeight = 13
    TabOrder = 0
    OnDrawItem = SrcFileListBoxDrawItem
    OnKeyDown = SrcFileListBoxKeyDown
    ExplicitWidth = 287
    ExplicitHeight = 209
  end
  object UpItemBtn: TButton
    Left = 307
    Top = 122
    Width = 50
    Height = 24
    Action = UserModule.UpListItemAction
    Anchors = [akRight, akBottom]
    TabOrder = 1
    ExplicitLeft = 303
    ExplicitTop = 121
  end
  object DownItemBtn: TButton
    Left = 307
    Top = 153
    Width = 50
    Height = 24
    Action = UserModule.DownListItemAction
    Anchors = [akRight, akBottom]
    TabOrder = 2
    ExplicitLeft = 303
    ExplicitTop = 152
  end
  object DelItemBtn: TButton
    Left = 307
    Top = 194
    Width = 50
    Height = 24
    Action = UserModule.DelListItemAction
    Anchors = [akRight, akBottom]
    TabOrder = 3
    ExplicitLeft = 303
    ExplicitTop = 193
  end
  object OpPanel: TPanel
    Left = 0
    Top = 229
    Width = 370
    Height = 162
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 4
    ExplicitTop = 228
    ExplicitWidth = 366
    DesignSize = (
      370
      162)
    object Label1: TLabel
      Left = 29
      Top = 42
      Width = 86
      Height = 18
      Alignment = taRightJustify
      AutoSize = False
      Caption = #20986#21147#12467#12540#12489'(&C)'
      FocusControl = OutCodeComboBox
      WordWrap = True
    end
    object Label2: TLabel
      Left = 230
      Top = 41
      Width = 53
      Height = 18
      Alignment = taRightJustify
      Anchors = [akTop, akRight]
      AutoSize = False
      Caption = #25913#34892'(&R)'
      FocusControl = OutLnBrkComboBox
      WordWrap = True
      ExplicitLeft = 234
    end
    object OutNameEdit: TLabeledEdit
      Left = 118
      Top = 7
      Width = 239
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 93
      EditLabel.Height = 23
      EditLabel.Caption = #20986#21147#12501#12449#12452#12523#21517'(&O)'
      LabelPosition = lpLeft
      TabOrder = 0
      Text = ''
      ExplicitWidth = 235
    end
    object TemplateEdit: TLabeledEdit
      Left = 118
      Top = 93
      Width = 149
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 74
      EditLabel.Height = 23
      EditLabel.Caption = #12486#12531#12503#12524#12540#12488'(&T)'
      LabelPosition = lpLeft
      TabOrder = 4
      Text = ''
      ExplicitWidth = 145
    end
    object OutCodeComboBox: TComboBox
      Left = 118
      Top = 38
      Width = 107
      Height = 23
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 1
      ExplicitWidth = 103
    end
    object OutLnBrkComboBox: TComboBox
      Left = 285
      Top = 38
      Width = 72
      Height = 23
      Style = csDropDownList
      Anchors = [akTop, akRight]
      TabOrder = 2
      ExplicitLeft = 281
    end
    object EditTmpltBtn: TButton
      Tag = 1
      Left = 307
      Top = 92
      Width = 50
      Height = 24
      Action = EditTmpltAction
      Anchors = [akTop, akRight]
      TabOrder = 6
      ExplicitLeft = 303
    end
    object RefTpltBtn: TButton
      Left = 269
      Top = 92
      Width = 36
      Height = 24
      HelpContext = 78
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 5
      OnClick = RefTpltBtnClick
      ExplicitLeft = 265
    end
    object CanButton: TButton
      Left = 277
      Top = 129
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 8
      ExplicitLeft = 273
    end
    object OkButton: TButton
      Left = 181
      Top = 129
      Width = 80
      Height = 26
      Action = JoinAction
      Anchors = [akTop, akRight]
      Default = True
      ModalResult = 1
      TabOrder = 7
      ExplicitLeft = 177
    end
    object BomCheckBox: TCheckBox
      Left = 285
      Top = 65
      Width = 79
      Height = 17
      Anchors = [akTop, akRight]
      Caption = 'BOM'#20184#12365
      TabOrder = 3
      ExplicitLeft = 281
    end
  end
  object ActionList1: TActionList
    Left = 323
    Top = 16
    object JoinAction: TAction
      Caption = #38283#22987
      OnUpdate = JoinActionUpdate
    end
    object EditTmpltAction: TAction
      Caption = #32232#38598
      OnExecute = EditTmpltActionExecute
      OnUpdate = EditTmpltActionUpdate
    end
  end
end
