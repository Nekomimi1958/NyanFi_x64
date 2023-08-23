object DistributionDlg: TDistributionDlg
  Left = 0
  Top = 0
  HelpContext = 77
  BorderIcons = [biSystemMenu]
  Caption = #25391#12426#20998#12369
  ClientHeight = 371
  ClientWidth = 632
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 640
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
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object PrvSplitter: TSplitter
    Left = 0
    Top = 250
    Width = 632
    Height = 5
    Cursor = crVSplit
    Align = alTop
    AutoSnap = False
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 632
    Height = 250
    Align = alTop
    BevelOuter = bvNone
    Constraints.MinHeight = 220
    TabOrder = 0
    ExplicitWidth = 628
    object RegPanel: TPanel
      Left = 0
      Top = 88
      Width = 632
      Height = 118
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitWidth = 628
      DesignSize = (
        632
        118)
      object CheckBtn: TButton
        Left = 4
        Top = 4
        Width = 24
        Height = 24
        Anchors = [akLeft, akBottom]
        Caption = #10003
        TabOrder = 0
        OnClick = CheckBtnClick
      end
      object ChgItemBtn: TButton
        Left = 65
        Top = 88
        Width = 55
        Height = 24
        Action = ChgRegAction
        Anchors = [akLeft, akBottom]
        TabOrder = 7
      end
      object DelItemBtn: TButton
        Left = 122
        Top = 88
        Width = 55
        Height = 24
        Action = DelRegAction
        Anchors = [akLeft, akBottom]
        TabOrder = 8
      end
      object DowItemBtn: TButton
        Left = 215
        Top = 88
        Width = 30
        Height = 24
        Action = UserModule.DownListItemAction
        Anchors = [akLeft, akBottom]
        TabOrder = 10
      end
      object UpItemBtn: TButton
        Left = 183
        Top = 88
        Width = 30
        Height = 24
        Action = UserModule.UpListItemAction
        Anchors = [akLeft, akBottom]
        TabOrder = 9
      end
      object TitleEdit: TLabeledEdit
        Left = 96
        Top = 6
        Width = 530
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        BevelEdges = [beLeft, beTop, beRight]
        EditLabel.Width = 38
        EditLabel.Height = 23
        EditLabel.Caption = #12479#12452#12488#12523
        LabelPosition = lpLeft
        TabOrder = 1
        Text = ''
        ExplicitWidth = 526
      end
      object DistrDirEdit: TLabeledEdit
        Left = 96
        Top = 60
        Width = 492
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        BevelEdges = [beLeft, beTop, beRight]
        EditLabel.Width = 58
        EditLabel.Height = 23
        EditLabel.Caption = #25391#12426#20998#12369#20808
        LabelPosition = lpLeft
        TabOrder = 4
        Text = ''
        ExplicitWidth = 488
      end
      object DistrMaskEdit: TLabeledEdit
        Left = 96
        Top = 33
        Width = 492
        Height = 23
        Hint = '/ '#65374' / '#12391#22258#12416#12392#27491#35215#34920#29694#12497#12479#12540#12531#13#10'@'#12391#22987#12414#12427#22580#21512#12399#12522#12473#12488#12501#12449#12452#12523#21517
        Anchors = [akLeft, akTop, akRight]
        BevelEdges = [beLeft, beTop, beRight]
        EditLabel.Width = 75
        EditLabel.Height = 23
        EditLabel.Caption = #12510#12473#12463'/'#12497#12479#12540#12531
        LabelPosition = lpLeft
        TabOrder = 2
        Text = ''
        ExplicitWidth = 488
      end
      object RefDirButton: TButton
        Left = 590
        Top = 60
        Width = 36
        Height = 22
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 5
        OnClick = RefDirButtonClick
        ExplicitLeft = 586
      end
      object AddItemBtn: TButton
        Left = 8
        Top = 88
        Width = 55
        Height = 24
        Action = AddRegAction
        Anchors = [akLeft, akBottom]
        TabOrder = 6
      end
      object FindEdit: TLabeledEdit
        Left = 295
        Top = 89
        Width = 277
        Height = 23
        Anchors = [akLeft, akRight, akBottom]
        EditLabel.Width = 26
        EditLabel.Height = 23
        EditLabel.Caption = #26908#32034
        LabelPosition = lpLeft
        TabOrder = 11
        Text = ''
        OnChange = FindEditChange
        OnKeyDown = FindEditKeyDown
        ExplicitWidth = 273
      end
      object RefListBtn: TButton
        Left = 590
        Top = 33
        Width = 36
        Height = 22
        Hint = #12522#12473#12488#12501#12449#12452#12523#12398#21442#29031
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 3
        OnClick = RefListBtnClick
        ExplicitLeft = 586
      end
      object Button1: TButton
        Left = 574
        Top = 88
        Width = 25
        Height = 24
        Action = FindDownAction
        Anchors = [akTop, akRight]
        TabOrder = 12
        ExplicitLeft = 570
      end
      object FindUpBtn: TButton
        Left = 601
        Top = 88
        Width = 25
        Height = 24
        Action = FindUpAction
        Anchors = [akTop, akRight]
        TabOrder = 13
        ExplicitLeft = 597
      end
    end
    object ExePanel: TPanel
      Left = 0
      Top = 206
      Width = 632
      Height = 44
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      ExplicitWidth = 628
      DesignSize = (
        632
        44)
      object Label1: TLabel
        Left = 206
        Top = 15
        Width = 76
        Height = 15
        Alignment = taRightJustify
        Anchors = [akTop, akRight]
        Caption = #21516#21517#26178#12398#20966#29702
      end
      object MoveBtn: TButton
        Left = 551
        Top = 9
        Width = 75
        Height = 26
        Action = ExeMoveAction
        Anchors = [akTop, akRight]
        Default = True
        TabOrder = 3
        ExplicitLeft = 547
      end
      object CopyBtn: TButton
        Left = 468
        Top = 9
        Width = 75
        Height = 26
        Action = ExeCopyAction
        Anchors = [akTop, akRight]
        TabOrder = 2
        ExplicitLeft = 464
      end
      object SameNameComboBox: TComboBox
        Left = 286
        Top = 12
        Width = 156
        Height = 23
        Style = csDropDownList
        Anchors = [akTop, akRight]
        TabOrder = 1
        ExplicitLeft = 282
      end
      object CreDistrDirCheckBox: TCheckBox
        Left = 16
        Top = 14
        Width = 153
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #25391#12426#20998#12369#20808#12434#33258#21205#20316#25104
        TabOrder = 0
        OnClick = CreDistrDirCheckBoxClick
        ExplicitLeft = 12
      end
      object BevelPanel1: TPanel
        Left = 0
        Top = 0
        Width = 632
        Height = 2
        Align = alTop
        BevelOuter = bvNone
        TabOrder = 4
        ExplicitWidth = 628
        object Shape1: TShape
          Left = 0
          Top = 0
          Width = 632
          Height = 1
          Align = alTop
          Brush.Style = bsClear
          Pen.Color = clBtnShadow
        end
        object Shape2: TShape
          Left = 0
          Top = 1
          Width = 632
          Height = 1
          Align = alTop
          Brush.Style = bsClear
          Pen.Color = clBtnHighlight
        end
      end
    end
    object ListPanel: TPanel
      Left = 0
      Top = 0
      Width = 632
      Height = 88
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitWidth = 628
      object ListSplitter: TSplitter
        Left = 477
        Top = 0
        Width = 5
        Height = 88
        Align = alRight
      end
      object RegListBox: TCheckListBox
        Left = 0
        Top = 0
        Width = 477
        Height = 88
        Align = alClient
        BorderStyle = bsNone
        DoubleBuffered = True
        DragMode = dmAutomatic
        ItemHeight = 13
        ParentDoubleBuffered = False
        PopupMenu = PopupMenu1
        Style = lbOwnerDrawFixed
        TabOrder = 0
        OnClick = RegListBoxClick
        OnClickCheck = RegListBoxClickCheck
        OnDrawItem = RegListBoxDrawItem
        OnKeyDown = RegListBoxKeyDown
        ExplicitWidth = 473
      end
      object ListListBox: TListBox
        Left = 482
        Top = 0
        Width = 150
        Height = 88
        Style = lbOwnerDrawFixed
        Align = alRight
        BorderStyle = bsNone
        PopupMenu = PopupMenu2
        TabOrder = 1
        OnDrawItem = ListListBoxDrawItem
        OnKeyDown = ListListBoxKeyDown
        ExplicitLeft = 478
      end
    end
  end
  object PrvPanel: TPanel
    Left = 0
    Top = 255
    Width = 632
    Height = 96
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitWidth = 628
    ExplicitHeight = 95
    object PrvListBox: TListBox
      Left = 0
      Top = 19
      Width = 632
      Height = 77
      Style = lbVirtualOwnerDraw
      Align = alClient
      BorderStyle = bsNone
      ItemHeight = 13
      TabOrder = 1
      OnData = PrvListBoxData
      OnDblClick = PrvListBoxDblClick
      OnDrawItem = PrvListBoxDrawItem
      ExplicitWidth = 628
      ExplicitHeight = 76
    end
    object PrvListHeader: THeaderControl
      Tag = 1
      Left = 0
      Top = 2
      Width = 632
      Height = 17
      Sections = <
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 200
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 200
        end>
      OnDrawSection = PrvListHeaderDrawSection
      OnSectionClick = PrvListHeaderSectionClick
      OnSectionResize = PrvListHeaderSectionResize
      ExplicitWidth = 628
    end
    object BevelPanel2: TPanel
      Left = 0
      Top = 0
      Width = 632
      Height = 2
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 2
      ExplicitWidth = 628
      object Shape3: TShape
        Left = 0
        Top = 0
        Width = 632
        Height = 1
        Align = alTop
        Brush.Style = bsClear
        Pen.Color = clBtnShadow
      end
      object Shape4: TShape
        Left = 0
        Top = 1
        Width = 632
        Height = 1
        Align = alTop
        Brush.Style = bsClear
        Pen.Color = clBtnHighlight
      end
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 351
    Width = 632
    Height = 20
    Panels = <
      item
        Style = psOwnerDraw
        Width = 120
      end
      item
        Style = psOwnerDraw
        Width = 300
      end
      item
        Style = psOwnerDraw
        Width = 120
      end
      item
        Style = psOwnerDraw
        Width = 50
      end>
    OnDrawPanel = StatusBar1DrawPanel
    ExplicitTop = 350
    ExplicitWidth = 628
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 351
    Width = 632
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 3
    TabStop = False
    ExplicitTop = 350
    ExplicitWidth = 628
  end
  object ActionList1: TActionList
    Left = 40
    Top = 16
    object ExeCopyAction: TAction
      Caption = #12467#12500#12540
      OnExecute = ExeCopyActionExecute
      OnUpdate = ExeActionUpdate
    end
    object ExeMoveAction: TAction
      Caption = #31227#21205
      OnExecute = ExeMoveActionExecute
      OnUpdate = ExeActionUpdate
    end
    object AddRegAction: TAction
      Caption = #30331#37682
      OnExecute = AddRegActionExecute
      OnUpdate = AddRegActionUpdate
    end
    object ChgRegAction: TAction
      Caption = #22793#26356
      OnExecute = ChgRegActionExecute
      OnUpdate = ChgRegActionUpdate
    end
    object DelRegAction: TAction
      Caption = #21066#38500
      OnExecute = DelRegActionExecute
      OnUpdate = ChgRegActionUpdate
    end
    object FindDownAction: TAction
      Caption = #9660
      OnExecute = FindDownActionExecute
      OnUpdate = FindDownActionUpdate
    end
    object FindUpAction: TAction
      Caption = #9650
      OnExecute = FindUpActionExecute
      OnUpdate = FindUpActionUpdate
    end
    object MakeFileAction: TAction
      Caption = #25391#12426#20998#12369#30331#37682#12501#12449#12452#12523#12434#20316#25104'(&M)...'
      OnExecute = MakeFileActionExecute
    end
    object SelFileAction: TAction
      Caption = #25391#12426#20998#12369#30331#37682#12501#12449#12452#12523#12398#36984#25246'(&S)...'
      OnExecute = SelFileActionExecute
    end
    object GroupCheckAction: TAction
      Caption = #21516#19968#12479#12452#12488#12523#12434#21516#26178#12395#35373#23450'(&G)'
      OnExecute = GroupCheckActionExecute
    end
    object EditListAction: TAction
      Caption = #12522#12473#12488#12501#12449#12452#12523#12398#32232#38598'(&E)'
      OnExecute = EditListActionExecute
      OnUpdate = EditListActionUpdate
    end
    object PrvListAction: TAction
      Caption = #12522#12473#12488#12501#12449#12452#12523#12398#12503#12524#12499#12517#12540#12434#34920#31034'(&L)'
      OnExecute = PrvListActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 152
    Top = 16
    object SaveRegItem: TMenuItem
      Action = MakeFileAction
    end
    object SelFileItem: TMenuItem
      Action = SelFileAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object GroupChkItem: TMenuItem
      Action = GroupCheckAction
    end
    object PrvListItem: TMenuItem
      Action = PrvListAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object EditListItem: TMenuItem
      Action = EditListAction
    end
  end
  object PopupMenu2: TPopupMenu
    Left = 552
    Top = 16
    object PrvListItem2: TMenuItem
      Action = PrvListAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object EditListItem2: TMenuItem
      Action = EditListAction
    end
  end
end
