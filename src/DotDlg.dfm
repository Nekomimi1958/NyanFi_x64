object DotNyanDlg: TDotNyanDlg
  Left = 0
  Top = 0
  HelpContext = 75
  BorderStyle = bsDialog
  ClientHeight = 582
  ClientWidth = 678
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  ShowHint = True
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  DesignSize = (
    678
    582)
  TextHeight = 15
  object Label1: TLabel
    Left = 33
    Top = 223
    Width = 50
    Height = 15
    Alignment = taRightJustify
    Caption = #12497#12473#12510#12473#12463
  end
  object TLabel
    Left = 26
    Top = 252
    Width = 57
    Height = 15
    Alignment = taRightJustify
    Caption = 'GREP'#12510#12473#12463
  end
  object TLabel
    Left = 346
    Top = 177
    Width = 10
    Height = 15
    Caption = '%'
  end
  object TLabel
    Left = 451
    Top = 552
    Width = 42
    Height = 15
    Alignment = taRightJustify
    Caption = #12377#12409#12390#12434
  end
  object InheritLabel: TLabel
    Left = 495
    Top = 308
    Width = 52
    Height = 15
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
    Width = 572
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 26
    EditLabel.Height = 23
    EditLabel.Caption = #35500#26126
    LabelPosition = lpLeft
    TabOrder = 15
    Text = ''
    ExplicitWidth = 568
  end
  object IconRadioGroup: TRadioGroup
    Left = 207
    Top = 114
    Width = 368
    Height = 41
    Caption = #12450#12452#12467#12531
    Columns = 4
    Items.Strings = (
      #12381#12398#12414#12414
      #34920#31034
      #38750#34920#31034
      #12501#12457#12523#12480#12398#12415)
    TabOrder = 10
  end
  object HandledCheckBox: TCheckBox
    Left = 393
    Top = 176
    Width = 276
    Height = 17
    Caption = #12300#12459#12524#12531#12488#12398#12487#12451#12524#12463#12488#12522#22793#26356#12301#12452#12505#12531#12488#12434#25233#27490
    TabOrder = 12
  end
  object GrepMaskComboBox: TComboBox
    Left = 87
    Top = 249
    Width = 572
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 14
    ExplicitWidth = 568
  end
  object PathMaskComboBox: TComboBox
    Left = 87
    Top = 220
    Width = 572
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 13
    ExplicitWidth = 568
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
    Width = 536
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #12469#12454#12531#12489
    TabOrder = 18
    ExplicitWidth = 532
    DesignSize = (
      536
      48)
    object SoundEdit: TEdit
      Left = 10
      Top = 18
      Width = 405
      Height = 23
      TabOrder = 3
    end
    object RefSndBtn: TButton
      Tag = 2
      Left = 420
      Top = 17
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 0
      OnClick = RefSndBtnClick
      ExplicitLeft = 416
    end
    object TestSndBtn: TButton
      Tag = 2
      Left = 457
      Top = 17
      Width = 22
      Height = 22
      Hint = #12486#12473#12488#20877#29983
      Anchors = [akTop, akRight]
      Caption = '>'
      TabOrder = 1
      OnClick = TestSndBtnClick
      ExplicitLeft = 453
    end
    object InhSndBtn: TButton
      Left = 481
      Top = 17
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 2
      OnClick = InhSndBtnClick
      ExplicitLeft = 477
    end
  end
  object ListWdEdit: TLabeledEdit
    Left = 302
    Top = 174
    Width = 40
    Height = 23
    Alignment = taRightJustify
    EditLabel.Width = 88
    EditLabel.Height = 23
    EditLabel.Caption = #12501#12449#12452#12523#12522#12473#12488#12398#24133
    LabelPosition = lpLeft
    NumbersOnly = True
    TabOrder = 11
    Text = ''
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
    Width = 536
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #32972#26223#30011#20687
    TabOrder = 17
    ExplicitWidth = 532
    DesignSize = (
      536
      48)
    object BgImgEdit: TEdit
      Left = 10
      Top = 18
      Width = 405
      Height = 23
      TabOrder = 2
    end
    object RefImgBtn: TButton
      Tag = 2
      Left = 420
      Top = 17
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 0
      OnClick = RefImgBtnClick
      ExplicitLeft = 416
    end
    object InhImgBtn: TButton
      Left = 481
      Top = 17
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 1
      OnClick = InhImgBtnClick
      ExplicitLeft = 477
    end
  end
  object CmdsGroupBox: TGroupBox
    Left = 10
    Top = 490
    Width = 536
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #12467#12510#12531#12489#12501#12449#12452#12523
    TabOrder = 19
    ExplicitWidth = 532
    DesignSize = (
      536
      48)
    object RefCmdsBtn: TButton
      Tag = 2
      Left = 420
      Top = 18
      Width = 36
      Height = 22
      Hint = #21442#29031
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 1
      OnClick = RefCmdsBtnClick
      ExplicitLeft = 416
    end
    object ExeCmdsEdit: TLabeledEdit
      Left = 26
      Top = 18
      Width = 389
      Height = 23
      EditLabel.Width = 11
      EditLabel.Height = 23
      EditLabel.Caption = '@'
      LabelPosition = lpLeft
      TabOrder = 0
      Text = ''
    end
    object InhCmdBtn: TButton
      Left = 481
      Top = 18
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 2
      OnClick = InhCmdBtnClick
      ExplicitLeft = 477
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
    Width = 536
    Height = 48
    Anchors = [akLeft, akTop, akRight]
    Caption = #37197#33394
    TabOrder = 16
    ExplicitWidth = 532
    DesignSize = (
      536
      48)
    object RefColBtn: TButton
      Left = 393
      Top = 18
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 2
      OnClick = RefColBtnClick
      ExplicitLeft = 389
    end
    object DisableBtn: TButton
      Left = 431
      Top = 18
      Width = 48
      Height = 22
      Action = DisableColAction
      Anchors = [akTop, akRight]
      TabOrder = 3
      ExplicitLeft = 427
    end
    object ColorComboBox: TComboBox
      Left = 10
      Top = 18
      Width = 351
      Height = 22
      Style = csOwnerDrawFixed
      TabOrder = 0
      OnDrawItem = ColorComboBoxDrawItem
    end
    object SpuitPanel: TPanel
      Left = 369
      Top = 18
      Width = 22
      Height = 22
      Anchors = [akTop, akRight]
      BevelOuter = bvLowered
      TabOrder = 1
      StyleElements = [seBorder]
      ExplicitLeft = 365
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
    object InhColBtn: TButton
      Left = 481
      Top = 18
      Width = 48
      Height = 22
      Anchors = [akTop, akRight]
      Caption = #32153#25215
      TabOrder = 4
      OnClick = InhColBtnClick
      ExplicitLeft = 477
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
