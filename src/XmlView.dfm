object XmlViewer: TXmlViewer
  Left = 0
  Top = 0
  HelpContext = 81
  Caption = 'XML'#12499#12517#12450#12540
  ClientHeight = 371
  ClientWidth = 592
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
  object StatusBar1: TStatusBar
    Left = 0
    Top = 351
    Width = 592
    Height = 20
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Pitch = fpVariable
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
    object XmlTreeView: TTreeView
      Left = 0
      Top = 0
      Width = 592
      Height = 307
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      DoubleBuffered = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
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
      OnChange = XmlTreeViewChange
      OnCustomDrawItem = XmlTreeViewCustomDrawItem
      OnHint = XmlTreeViewHint
      OnKeyDown = XmlTreeViewKeyDown
      OnKeyPress = XmlTreeViewKeyPress
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
      object Label1: TLabel
        Left = 87
        Top = 15
        Width = 63
        Height = 13
        Caption = #33258#21205#38283#38281'(&A)'
      end
      object Label2: TLabel
        Left = 426
        Top = 15
        Width = 12
        Height = 13
        Anchors = [akTop, akRight]
        Caption = #21517
      end
      object Label3: TLabel
        Left = 469
        Top = 15
        Width = 12
        Height = 13
        Anchors = [akTop, akRight]
        Caption = #23646
      end
      object Label4: TLabel
        Left = 512
        Top = 15
        Width = 12
        Height = 13
        Anchors = [akTop, akRight]
        Caption = #20516
      end
      object FindEdit: TLabeledEdit
        Left = 234
        Top = 12
        Width = 165
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        EditLabel.Width = 38
        EditLabel.Height = 13
        EditLabel.Caption = #26908#32034'(&F)'
        LabelPosition = lpLeft
        TabOrder = 3
        OnKeyDown = FindEditKeyDown
        OnKeyPress = FindEditKeyPress
      end
      object AutoCheckBox: TCheckBox
        Left = 70
        Top = 14
        Width = 107
        Height = 17
        Action = AutoAction
        TabOrder = 2
      end
      object NamCheckBox: TCheckBox
        Left = 409
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        TabOrder = 4
      end
      object ValCheckBox: TCheckBox
        Left = 495
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        TabOrder = 6
      end
      object AtrCheckBox: TCheckBox
        Left = 452
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
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
        Left = 533
        Top = 10
        Width = 25
        Height = 25
        Action = FindDownAction
        Anchors = [akTop, akRight]
        TabOrder = 7
      end
      object FindUpBtn: TButton
        Left = 560
        Top = 10
        Width = 25
        Height = 25
        Action = FindUpAction
        Anchors = [akTop, akRight]
        TabOrder = 8
      end
    end
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 64
    Top = 144
    object ExpandItem: TMenuItem
      Caption = #12377#12409#12390#23637#38283'(&E)'
      OnClick = ExpandItemClick
    end
    object CollapseItem: TMenuItem
      Caption = #12377#12409#12390#30067#12416'(&A)'
      OnClick = CollapseItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object CopyItem: TMenuItem
      Action = CopyAction
    end
    object CopyXmlItem: TMenuItem
      Action = CopyXmlAction
    end
    object XPath1: TMenuItem
      Action = CopyXPathAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object OpenUrlItem: TMenuItem
      Action = OpenUrlAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object ViewModeItem: TMenuItem
      Caption = #34920#31034#12514#12540#12489'(&V)'
      object ViewMode0Item: TMenuItem
        Caption = #12494#12540#12489#21517'(&N)'
        OnClick = ViewModeItemClick
      end
      object ViewMode1Item: TMenuItem
        Tag = 1
        Caption = #12494#12540#12489#21517' ['#23646#24615'](&A)'
        OnClick = ViewModeItemClick
      end
      object ViewMode2Item: TMenuItem
        Tag = 2
        Caption = 'XML(&X)'
        OnClick = ViewModeItemClick
      end
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 64
    Top = 88
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
    object CopyXmlAction: TAction
      Caption = 'XML'#12477#12540#12473#12434#12467#12500#12540'(&S)'
      OnExecute = CopyXmlActionExecute
      OnUpdate = CopyXmlActionUpdate
    end
    object CopyXPathAction: TAction
      Caption = 'XPath'#12434#12467#12500#12540'(&P)'
      OnExecute = CopyXPathActionExecute
      OnUpdate = CopyXPathActionUpdate
    end
    object OpenUrlAction: TAction
      Caption = 'URL'#12434#38283#12367'(&W)'
      OnExecute = OpenUrlActionExecute
      OnUpdate = OpenUrlActionUpdate
    end
    object AutoAction: TAction
      OnExecute = AutoActionExecute
    end
  end
  object XMLDocument1: TXMLDocument
    Left = 64
    Top = 24
  end
end
