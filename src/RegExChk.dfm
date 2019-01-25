object RegExChecker: TRegExChecker
  Left = 0
  Top = 0
  HelpContext = 85
  BorderIcons = [biSystemMenu]
  Caption = #27491#35215#34920#29694#12481#12455#12483#12459#12540
  ClientHeight = 400
  ClientWidth = 625
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 380
    Top = 0
    Width = 5
    Height = 400
    AutoSnap = False
    MinSize = 60
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 380
    Height = 400
    Align = alLeft
    BevelOuter = bvNone
    BorderWidth = 4
    Constraints.MinWidth = 380
    TabOrder = 0
    object Splitter2: TSplitter
      Left = 4
      Top = 248
      Width = 372
      Height = 5
      Cursor = crVSplit
      Align = alTop
      AutoSnap = False
      MinSize = 60
    end
    object OpePanel: TPanel
      Left = 4
      Top = 4
      Width = 372
      Height = 244
      Align = alTop
      BevelOuter = bvNone
      Constraints.MinHeight = 220
      TabOrder = 0
      DesignSize = (
        372
        244)
      object TLabel
        Left = 39
        Top = 7
        Width = 37
        Height = 13
        Alignment = taRightJustify
        Caption = #12497#12479#12540#12531
      end
      object TLabel
        Left = 4
        Top = 134
        Width = 48
        Height = 13
        Caption = #26908#32034#23550#35937
      end
      object Bevel1: TBevel
        Left = 0
        Top = 122
        Width = 372
        Height = 4
        Anchors = [akLeft, akTop, akRight]
        Shape = bsBottomLine
      end
      object CaseCheckBox: TCheckBox
        Left = 80
        Top = 64
        Width = 135
        Height = 17
        Caption = #22823#23567#25991#23383#12434#21306#21029
        TabOrder = 4
      end
      object CCopyBtn: TButton
        Tag = 1
        Left = 90
        Top = 92
        Width = 114
        Height = 24
        Action = CopyCAction
        Anchors = [akTop, akRight]
        Caption = #12467#12500#12540'(C'#25991#23383#21015')'
        TabOrder = 5
      end
      object MatchBtn: TButton
        Left = 292
        Top = 3
        Width = 80
        Height = 24
        Action = TestAction
        Anchors = [akTop, akRight]
        Default = True
        TabOrder = 1
      end
      object PtnComboBox: TComboBox
        Left = 80
        Top = 4
        Width = 208
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        DropDownCount = 20
        TabOrder = 0
        OnEnter = PtnComboBoxEnter
      end
      object ReplaceBtn: TButton
        Left = 292
        Top = 34
        Width = 80
        Height = 24
        Action = ReplaceAction
        Anchors = [akTop, akRight]
        TabOrder = 3
      end
      object ReplaceEdit: TLabeledEdit
        Left = 80
        Top = 35
        Width = 208
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 60
        EditLabel.Height = 13
        EditLabel.Caption = #32622#25563#25991#23383#21015
        LabelPosition = lpLeft
        PopupMenu = UserModule.EditPopupMenuE
        TabOrder = 2
      end
      object CopyBtn: TButton
        Left = 208
        Top = 92
        Width = 80
        Height = 24
        Action = CopyAction
        Anchors = [akTop, akRight]
        TabOrder = 6
      end
      object PasteBtn: TButton
        Left = 292
        Top = 92
        Width = 80
        Height = 24
        Action = PasteAction
        Anchors = [akTop, akRight]
        ModalResult = 1
        TabOrder = 7
      end
      object ObjMemo: TMemo
        Left = 0
        Top = 156
        Width = 372
        Height = 88
        Align = alBottom
        Anchors = [akLeft, akTop, akRight, akBottom]
        Lines.Strings = (
          '')
        ScrollBars = ssVertical
        TabOrder = 10
      end
      object FileEdit: TLabeledEdit
        Left = 120
        Top = 129
        Width = 128
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 34
        EditLabel.Height = 13
        EditLabel.Caption = #12501#12449#12452#12523
        LabelPosition = lpLeft
        TabOrder = 8
      end
      object RefFileBtn: TButton
        Left = 252
        Top = 128
        Width = 36
        Height = 22
        Anchors = [akTop, akRight]
        Caption = '...'
        TabOrder = 9
        OnClick = RefFileBtnClick
      end
      object UpdtCheckBox: TCheckBox
        Left = 294
        Top = 131
        Width = 78
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #22793#26356#21453#26144
        TabOrder = 11
      end
    end
    object ResPanel: TPanel
      Left = 4
      Top = 253
      Width = 372
      Height = 143
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object ResultLabel: TLabel
        Left = 4
        Top = 4
        Width = 24
        Height = 13
        Caption = #32080#26524
        StyleElements = [seClient, seBorder]
      end
      object ResListBox: TListBox
        Left = 0
        Top = 23
        Width = 372
        Height = 120
        Style = lbOwnerDrawFixed
        Align = alBottom
        Anchors = [akLeft, akTop, akRight, akBottom]
        ItemHeight = 13
        TabOrder = 0
        TabWidth = 40
        OnDrawItem = ResListBoxDrawItem
      end
    end
  end
  object ReferPanel: TPanel
    Left = 385
    Top = 0
    Width = 240
    Height = 400
    Align = alClient
    BevelOuter = bvLowered
    BorderWidth = 4
    TabOrder = 1
    object ReferListBox: TListBox
      Left = 5
      Top = 5
      Width = 230
      Height = 390
      Align = alClient
      BorderStyle = bsNone
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MeiryoKe_Gothic'
      Font.Style = []
      ItemHeight = 15
      ParentFont = False
      TabOrder = 0
      TabWidth = 36
      OnDblClick = ReferListBoxDblClick
    end
    object HiddenCanBtn: TButton
      Left = 5
      Top = 395
      Width = 230
      Height = 0
      Align = alBottom
      Cancel = True
      ModalResult = 2
      TabOrder = 1
      TabStop = False
    end
  end
  object ActionList1: TActionList
    Left = 440
    Top = 16
    object TestAction: TAction
      Caption = #12486#12473#12488
      OnExecute = TestActionExecute
      OnUpdate = TestActionUpdate
    end
    object ReplaceAction: TAction
      Caption = #32622#25563
      OnExecute = ReplaceActionExecute
      OnUpdate = ReplaceActionUpdate
    end
    object PasteAction: TAction
      Caption = #36028#12426#20184#12369'(&P)'
      OnExecute = PasteActionExecute
      OnUpdate = PasteActionUpdate
    end
    object CopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object CopyCAction: TAction
      Tag = 1
      Caption = #12467#12500#12540'(C'#29992#25991#23383#21015')'
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
  end
end
