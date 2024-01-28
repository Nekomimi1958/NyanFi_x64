object JsonViewer: TJsonViewer
  Left = 0
  Top = 0
  HelpContext = 92
  Caption = 'JSON'#12499#12517#12540#12450
  ClientHeight = 371
  ClientWidth = 592
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
  TextHeight = 15
  object StatusBar1: TStatusBar
    Left = 0
    Top = 351
    Width = 592
    Height = 20
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Panels = <
      item
        Bevel = pbNone
        Style = psOwnerDraw
        Width = 600
      end>
    UseSystemFont = False
    StyleElements = [seBorder]
    OnDrawPanel = StatusBar1DrawPanel
  end
  object Button1: TButton
    Left = 0
    Top = 351
    Width = 592
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 1
    TabStop = False
  end
  object ViewPanel: TPanel
    Left = 0
    Top = 0
    Width = 592
    Height = 351
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object JsonTreeView: TTreeView
      Left = 0
      Top = 0
      Width = 588
      Height = 307
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      DoubleBuffered = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Segoe UI'
      Font.Style = []
      Indent = 19
      ParentDoubleBuffered = False
      ParentFont = False
      ParentShowHint = False
      PopupMenu = PopupMenu1
      ReadOnly = True
      RowSelect = True
      ShowHint = False
      TabOrder = 0
      StyleElements = []
      OnChange = JsonTreeViewChange
      OnCustomDrawItem = JsonTreeViewCustomDrawItem
      OnDeletion = JsonTreeViewDeletion
      OnHint = JsonTreeViewHint
      OnKeyDown = JsonTreeViewKeyDown
      OnKeyPress = JsonTreeViewKeyPress
    end
    object OpePanel: TPanel
      Left = 0
      Top = 307
      Width = 592
      Height = 44
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        592
        44)
      object FindEdit: TLabeledEdit
        Left = 234
        Top = 12
        Width = 195
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 40
        EditLabel.Height = 23
        EditLabel.Caption = #26908#32034'(&F)'
        LabelPosition = lpLeft
        TabOrder = 3
        Text = ''
        OnKeyDown = FindEditKeyDown
        OnKeyPress = FindEditKeyPress
      end
      object AutoCheckBox: TCheckBox
        Left = 70
        Top = 14
        Width = 107
        Height = 17
        Action = AutoAction
        Caption = #33258#21205#38283#38281'(&A)'
        TabOrder = 2
      end
      object NamCheckBox: TCheckBox
        Left = 439
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #21517
        TabOrder = 4
      end
      object ValCheckBox: TCheckBox
        Left = 484
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #20516
        TabOrder = 5
      end
      object ExpBtn: TButton
        Left = 10
        Top = 10
        Width = 25
        Height = 25
        Action = ExpandAction
        TabOrder = 0
      end
      object ColBtn: TButton
        Left = 37
        Top = 10
        Width = 25
        Height = 25
        Action = CollapseAction
        TabOrder = 1
      end
      object FindDownBtn: TButton
        Left = 529
        Top = 10
        Width = 25
        Height = 25
        Action = FindDownAction
        Anchors = [akTop, akRight]
        TabOrder = 6
      end
      object FindUpBtn: TButton
        Left = 556
        Top = 10
        Width = 25
        Height = 25
        Action = FindUpAction
        Anchors = [akTop, akRight]
        TabOrder = 7
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 64
    Top = 144
    object ExpandItem: TMenuItem
      Caption = #12377#12409#12390#23637#38283'(&A)'
      OnClick = ExpandItemClick
    end
    object CollapseItem: TMenuItem
      Caption = #12377#12409#12390#30067#12416'(&X)'
      OnClick = CollapseItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object CopyItem: TMenuItem
      Action = CopyAction
    end
    object CopyPathItem: TMenuItem
      Action = CopyJsonPathAction
    end
    object CopyJsonItem: TMenuItem
      Action = CopyJsonAction
    end
    object CopyFormatItem: TMenuItem
      Action = CopyFormatAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object OpenUrlItem: TMenuItem
      Action = OpenUrlAction
    end
    object E1: TMenuItem
      Action = EditErrAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 64
    Top = 56
    object FindDownAction: TAction
      Caption = #9660
      OnExecute = FindDownActionExecute
      OnUpdate = FindActionUpdate
    end
    object FindUpAction: TAction
      Caption = #9650
      OnExecute = FindUpActionExecute
      OnUpdate = FindActionUpdate
    end
    object ExpandAction: TAction
      Caption = '+'
      Hint = #36984#25246#20013#12494#12540#12489#12398#23376#12434#12377#12409#12390#23637#38283
      OnExecute = ExpandActionExecute
    end
    object CollapseAction: TAction
      Caption = '-'
      Hint = #36984#25246#20013#12494#12540#12489#12398#23376#12434#12377#12409#12390#30067#12416
      OnExecute = CollapseActionExecute
    end
    object CopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      ShortCut = 16451
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object CopyJsonAction: TAction
      Caption = 'JSON'#25991#23383#21015#12434#12467#12500#12540'(&S)'
      OnExecute = CopyJsonActionExecute
      OnUpdate = CopyJsonActionUpdate
    end
    object CopyFormatAction: TAction
      Caption = #20840#20307#12434#25972#24418#12375#12390#12467#12500#12540'(&F)'
      OnExecute = CopyFormatActionExecute
      OnUpdate = CopyFormatActionUpdate
    end
    object CopyJsonPathAction: TAction
      Caption = #12497#12473#24335#12434#12467#12500#12540'(&P)'
      OnExecute = CopyJsonPathActionExecute
      OnUpdate = CopyJsonPathActionUpdate
    end
    object OpenUrlAction: TAction
      Caption = 'URL'#12434#38283#12367'(&W)'
      OnExecute = OpenUrlActionExecute
      OnUpdate = OpenUrlActionUpdate
    end
    object AutoAction: TAction
      OnExecute = AutoActionExecute
    end
    object EditErrAction: TAction
      Caption = #12456#12521#12540#34892#12434#12456#12487#12451#12479#12391#38283#12367'(&O)'
      OnExecute = EditErrActionExecute
      OnUpdate = EditErrActionUpdate
    end
  end
end
