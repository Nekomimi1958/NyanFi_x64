object DotNyanDlg: TDotNyanDlg
  Left = 0
  Top = 0
  HelpContext = 75
  BorderStyle = bsDialog
  ClientHeight = 582
  ClientWidth = 678
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    678
    582)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 38
    Top = 223
    Width = 45
    Height = 13
    Alignment = taRightJustify
    Caption = #12497#12473#12510#12473#12463
  end
  object TLabel
    Left = 31
    Top = 252
    Width = 52
    Height = 13
    Alignment = taRightJustify
    Caption = 'GREP'#12510#12473#12463
  end
  object TLabel
    Left = 636
    Top = 132
    Width = 11
    Height = 13
    Caption = '%'
  end
  object TLabel
    Left = 455
    Top = 552
    Width = 38
    Height = 13
    Alignment = taRightJustify
    Caption = #12377#12409#12390#12434
  end
  object InheritLabel: TLabel
    Left = 499
    Top = 308
    Width = 48
    Height = 13
    Alignment = taRightJustify
    Caption = #32153#25215#21487#33021
  end
  object CreNyanBtn: TButton
    Left = 583
    Top = 480
    Width = 80
    Height = 26
    Action = CreNyanAction
    Default = True
    TabOrder = 22
  end
  object DelNyanBtn: TButton
    Left = 583
    Top = 514
    Width = 80
    Height = 26
    Action = DelNyanAction
    TabOrder = 23
  end
  object CancelBtn: TButton
    Left = 583
    Top = 548
    Width = 80
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    TabOrder = 24
    OnClick = CancelBtnClick
  end
  object HideRadioGroup: TRadioGroup
    Left = 207
    Top = 8
    Width = 224
    Height = 41
    Caption = #38560#12375#12501#12449#12452#12523
    Columns = 3
    Items.Strings = (
      #12381#12398#12414#12414
      #34920#31034
      #38750#34920#31034)
    TabOrder = 6
  end
  object SysRadioGroup: TRadioGroup
    Left = 207
    Top = 61
    Width = 224
    Height = 41
    Caption = #12471#12473#12486#12512#12501#12449#12452#12523
    Columns = 3
    Items.Strings = (
      #12381#12398#12414#12414
      #34920#31034
      #38750#34920#31034)
    TabOrder = 8
  end
  object SizeRadioGroup: TRadioGroup
    Left = 443
    Top = 8
    Width = 224
    Height = 41
    Caption = #12469#12452#12474#34920#31034
    Columns = 3
    Items.Strings = (
      #12381#12398#12414#12414
      #12496#12452#12488
      'T/G/MB')
    TabOrder = 7
  end
  object DescEdit: TLabeledEdit
    Left = 87
    Top = 278
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 24
    EditLabel.Height = 13
    EditLabel.Caption = #35500#26126
    LabelPosition = lpLeft
    TabOrder = 15
  end
  object IconRadioGroup: TRadioGroup
    Left = 207
    Top = 114
    Width = 224
    Height = 41
    Caption = #12450#12452#12467#12531
    Columns = 3
    Items.Strings = (
      #12381#12398#12414#12414
      #34920#31034
      #38750#34920#31034)
    TabOrder = 10
  end
  object HandledCheckBox: TCheckBox
    Left = 215
    Top = 177
    Width = 301
    Height = 17
    Caption = #12300#12459#12524#12531#12488#12398#12487#12451#12524#12463#12488#12522#22793#26356#12301#12452#12505#12531#12488#12434#25233#27490
    TabOrder = 12
  end
  object GrepMaskComboBox: TComboBox
    Left = 87
    Top = 249
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 14
  end
  object PathMaskComboBox: TComboBox
    Left = 87
    Top = 220
    Width = 580
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 13
  end
  object SortRadioGroup: TRadioGroup
    Left = 10
    Top = 8
    Width = 185
    Height = 203
    Caption = #12477#12540#12488#26041#27861
    Items.Strings = (
      #12381#12398#12414#12414
      #21517#21069
      #25313#24373#23376
      #26356#26032#26085#26178
      #12469#12452#12474
      #23646#24615
      #12394#12375)
    TabOrder = 0
  end
  object SndGroupBox: TGroupBox
    Left = 10
    Top = 434
    Width = 544
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #12469#12454#12531#12489
    TabOrder = 18
    DesignSize = (
      544
      48)
    object SoundEdit: TEdit
      Left = 10
      Top = 18
      Width = 415
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 3
    end
    object RefSndBtn: TButton
      Tag = 2
      Left = 428
      Top = 17
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 0
      OnClick = RefSndBtnClick
    end
    object TestSndBtn: TButton
      Tag = 2
      Left = 465
      Top = 17
      Width = 22
      Height = 22
      Hint = #12486#12473#12488#20877#29983
      Anchors = [akTop, akRight]
      Caption = '>'
      TabOrder = 1
      OnClick = TestSndBtnClick
    end
    object InhSndBtn: TButton
      Left = 489
      Top = 17
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 2
      OnClick = InhSndBtnClick
    end
  end
  object ListWdEdit: TLabeledEdit
    Left = 582
    Top = 129
    Width = 50
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 80
    EditLabel.Height = 13
    EditLabel.Caption = #12501#12449#12452#12523#12522#12473#12488#12398#24133
    LabelPosition = lpLeft
    NumbersOnly = True
    TabOrder = 11
  end
  object SyncRadioGroup: TRadioGroup
    Left = 443
    Top = 61
    Width = 224
    Height = 41
    Caption = #24038#21491#12487#12451#12524#12463#12488#12522#12398#21516#26399#22793#26356
    Columns = 3
    Items.Strings = (
      #12381#12398#12414#12414
      #26377#21177
      #35299#38500)
    TabOrder = 9
  end
  object ImgGroupBox: TGroupBox
    Left = 10
    Top = 378
    Width = 544
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #32972#26223#30011#20687
    TabOrder = 17
    DesignSize = (
      544
      48)
    object BgImgEdit: TEdit
      Left = 10
      Top = 18
      Width = 415
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 2
    end
    object RefImgBtn: TButton
      Tag = 2
      Left = 428
      Top = 17
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 0
      OnClick = RefImgBtnClick
    end
    object InhImgBtn: TButton
      Left = 489
      Top = 17
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 1
      OnClick = InhImgBtnClick
    end
  end
  object CmdsGroupBox: TGroupBox
    Left = 10
    Top = 490
    Width = 544
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #12467#12510#12531#12489#12501#12449#12452#12523
    TabOrder = 19
    DesignSize = (
      544
      48)
    object RefCmdsBtn: TButton
      Tag = 2
      Left = 428
      Top = 18
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 1
      OnClick = RefCmdsBtnClick
    end
    object ExeCmdsEdit: TLabeledEdit
      Left = 26
      Top = 18
      Width = 399
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 10
      EditLabel.Height = 13
      EditLabel.Caption = '@'
      LabelPosition = lpLeft
      TabOrder = 0
    end
    object InhCmdBtn: TButton
      Left = 489
      Top = 18
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 2
      OnClick = InhCmdBtnClick
    end
  end
  object HideCheckBox: TCheckBox
    Left = 583
    Top = 453
    Width = 89
    Height = 17
    Caption = #38560#12375#12501#12449#12452#12523
    TabOrder = 21
  end
  object ColGroupBox: TGroupBox
    Left = 10
    Top = 322
    Width = 544
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #37197#33394
    TabOrder = 16
    DesignSize = (
      544
      48)
    object RefColBtn: TButton
      Left = 401
      Top = 18
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 2
      OnClick = RefColBtnClick
    end
    object DisableBtn: TButton
      Left = 439
      Top = 18
      Width = 48
      Height = 22
      Action = DisableColAction
      Anchors = [akTop, akRight]
      TabOrder = 3
    end
    object ColorComboBox: TComboBox
      Left = 10
      Top = 18
      Width = 363
      Height = 22
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnDrawItem = ColorComboBoxDrawItem
    end
    object SpuitPanel: TPanel
      Left = 377
      Top = 18
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
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
          07544269746D6170F6000000424DF60000000000000076000000280000001000
          000010000000010004000000000080000000C30E0000C30E0000100000001000
          000000000000000080000080000000808000800000008000800080800000C0C0
          C000808080000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFF
          FF003333333333333333300333333333333330F033333333333330FF03333333
          3333330FF033333333333330FF033333333333330FF033333333333330FF0333
          33333333330FF033333333333330FF0033333333333300003333333333333000
          0033333333333300000333333333330000033333333333300033333333333333
          3333}
        Transparent = True
        OnMouseDown = SpuitImageMouseDown
        OnMouseUp = SpuitImageMouseUp
      end
    end
    object InhColBtn: TButton
      Left = 489
      Top = 18
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 4
      OnClick = InhColBtnClick
    end
  end
  object InheritBtn: TButton
    Left = 499
    Top = 547
    Width = 48
    Height = 25
    Action = DelNyanAction
    Caption = #32153#25215
    TabOrder = 20
    OnClick = InheritBtnClick
  end
  object OldCheckBox: TCheckBox
    Left = 107
    Top = 107
    Width = 82
    Height = 17
    Caption = #21476#12356#38918
    TabOrder = 3
  end
  object SmallCheckBox: TCheckBox
    Left = 107
    Top = 132
    Width = 82
    Height = 17
    Caption = #23567#12373#12356#38918
    TabOrder = 4
  end
  object GroupBox2: TGroupBox
    Left = 100
    Top = 50
    Width = 85
    Height = 52
    TabOrder = 2
    object DscNameCheckBox: TCheckBox
      Left = 7
      Top = 27
      Width = 77
      Height = 17
      Caption = #38477#38918
      TabOrder = 1
    end
    object NaturalCheckBox: TCheckBox
      Left = 7
      Top = 4
      Width = 77
      Height = 17
      Caption = #33258#28982#38918
      TabOrder = 0
    end
  end
  object DscAttrCheckBox: TCheckBox
    Left = 107
    Top = 157
    Width = 82
    Height = 17
    Caption = #38477#38918
    TabOrder = 5
  end
  object NoOderCheckBox: TCheckBox
    Left = 107
    Top = 28
    Width = 82
    Height = 17
    Caption = #12381#12398#12414#12414
    TabOrder = 1
    OnClick = NoOderCheckBoxClick
  end
  object ActionList1: TActionList
    Left = 603
    Top = 382
    object CreNyanAction: TAction
      Caption = #20316#25104
      OnExecute = CreNyanActionExecute
      OnUpdate = CreNyanActionUpdate
    end
    object DelNyanAction: TAction
      Caption = #21066#38500
      OnExecute = DelNyanActionExecute
      OnUpdate = DelNyanActionUpdate
    end
    object DisableColAction: TAction
      Caption = #28961#21177
      OnExecute = DisableColActionExecute
      OnUpdate = DisableColActionUpdate
    end
  end
end
