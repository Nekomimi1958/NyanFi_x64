object FuncListDlg: TFuncListDlg
  Left = 0
  Top = 0
  HelpContext = 66
  BorderIcons = [biSystemMenu]
  ClientHeight = 341
  ClientWidth = 484
  Color = clBtnFace
  Constraints.MinWidth = 400
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
  OnShow = FormShow
  TextHeight = 15
  object UserDefPanel: TPanel
    Left = 0
    Top = 271
    Width = 484
    Height = 35
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 270
    ExplicitWidth = 480
    DesignSize = (
      484
      35)
    object UsrStrLabel: TLabel
      Left = 22
      Top = 13
      Width = 39
      Height = 15
      Alignment = taRightJustify
      Caption = #25991#23383#21015
    end
    object UserDefComboBox: TComboBox
      Left = 66
      Top = 10
      Width = 124
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnClick = UserDefComboBoxClick
      OnEnter = UserDefComboBoxEnter
      ExplicitWidth = 120
    end
    object UpdateBtn: TButton
      Left = 394
      Top = 9
      Width = 75
      Height = 24
      Action = UpdUserDefAction
      Anchors = [akTop, akRight]
      TabOrder = 3
      ExplicitLeft = 390
    end
    object RegExCheckBox: TCheckBox
      Left = 196
      Top = 12
      Width = 100
      Height = 17
      Action = RegExAction
      Anchors = [akTop, akRight]
      Caption = #27491#35215#34920#29694'(&R)'
      TabOrder = 1
      ExplicitLeft = 192
    end
    object RegHeaderBtn: TButton
      Left = 300
      Top = 9
      Width = 90
      Height = 24
      Action = RegHeaderAction
      Anchors = [akTop, akRight]
      TabOrder = 2
      ExplicitLeft = 296
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 306
    Width = 484
    Height = 35
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 305
    ExplicitWidth = 480
    object Opt1Panel: TPanel
      Left = 332
      Top = 0
      Width = 76
      Height = 35
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      TabStop = True
      ExplicitLeft = 328
      object NameOnlyCheckBox: TCheckBox
        Left = 2
        Top = 9
        Width = 71
        Height = 17
        Action = NameOnlyAction
        Caption = #21517#21069'(&N)'
        TabOrder = 0
      end
    end
    object FilterPanel: TPanel
      Left = 0
      Top = 0
      Width = 332
      Height = 35
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      TabStop = True
      ExplicitWidth = 328
      DesignSize = (
        332
        35)
      object MigemoCheckBox: TCheckBox
        Left = 251
        Top = 9
        Width = 80
        Height = 17
        Action = MigemoAction
        Anchors = [akTop, akRight]
        Caption = '&Migemo'
        TabOrder = 1
        ExplicitLeft = 247
      end
      object FilterEdit: TLabeledEdit
        Left = 66
        Top = 8
        Width = 177
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 50
        EditLabel.Height = 23
        EditLabel.Caption = #12501#12451#12523#12479'(&F)'
        LabelPosition = lpLeft
        PopupMenu = UserModule.EditPopupMenuE
        TabOrder = 0
        Text = ''
        OnChange = FilterEditChange
        OnEnter = FilterEditEnter
        OnExit = FilterEditExit
        OnKeyDown = FilterEditKeyDown
        OnKeyPress = FilterEditKeyPress
        ExplicitWidth = 173
      end
    end
    object Opt2Panel: TPanel
      Left = 408
      Top = 0
      Width = 76
      Height = 35
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 2
      TabStop = True
      ExplicitLeft = 404
      object LinkCheckBox: TCheckBox
        Left = 2
        Top = 9
        Width = 70
        Height = 17
        Action = LinkAction
        Caption = #36899#21205'(&L)'
        TabOrder = 0
      end
    end
  end
  object ListPanel: TPanel
    Left = 0
    Top = 0
    Width = 484
    Height = 271
    Align = alClient
    BevelOuter = bvNone
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 2
    ExplicitWidth = 480
    ExplicitHeight = 270
    object FuncListBox: TListBox
      Left = 0
      Top = 0
      Width = 476
      Height = 271
      Style = lbOwnerDrawFixed
      AutoComplete = False
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      MultiSelect = True
      ParentDoubleBuffered = False
      ParentFont = False
      PopupMenu = PopupMenu1
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = FuncListBoxClick
      OnDblClick = FuncListBoxDblClick
      OnDrawItem = FuncListBoxDrawItem
      OnKeyDown = FuncListBoxKeyDown
      OnKeyPress = FuncListBoxKeyPress
      ExplicitWidth = 472
      ExplicitHeight = 270
    end
  end
  object ActionList1: TActionList
    Left = 72
    Top = 16
    object UpdUserDefAction: TAction
      Caption = #26356#26032'(&U)'
      OnExecute = UpdUserDefActionExecute
      OnUpdate = UpdUserDefActionUpdate
    end
    object ReqEditAction: TAction
      OnExecute = ReqEditActionExecute
    end
    object CloseListAction: TAction
      Caption = 'CloseListAction'
      OnExecute = CloseListActionExecute
    end
    object CopyListAction: TAction
      Caption = #19968#35239#12434#12467#12500#12540'(&C)'
      OnExecute = CopyListActionExecute
      OnUpdate = ListActionUpdate
    end
    object SaveListAction: TAction
      Caption = #19968#35239#12434#12501#12449#12452#12523#12395#20445#23384'(&S)'
      OnExecute = SaveListActionExecute
      OnUpdate = ListActionUpdate
    end
    object RegHeaderAction: TAction
      Caption = #35211#20986#12375#30331#37682
      OnExecute = RegHeaderActionExecute
      OnUpdate = RegHeaderActionUpdate
    end
    object RegExAction: TAction
      OnExecute = RegExActionExecute
    end
    object MigemoAction: TAction
      OnExecute = MigemoActionExecute
    end
    object NameOnlyAction: TAction
      OnExecute = NameOnlyActionExecute
    end
    object LinkAction: TAction
      OnExecute = LinkActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 72
    Top = 76
    object CopyListItem: TMenuItem
      Action = CopyListAction
    end
    object SaveListItem: TMenuItem
      Action = SaveListAction
    end
  end
end
