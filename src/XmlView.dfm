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
        Width = 600
      end>
    UseSystemFont = False
    StyleElements = []
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
      object FindUpBtn: TSpeedButton
        Left = 563
        Top = 11
        Width = 23
        Height = 22
        Action = FindUpAction
        Anchors = [akTop, akRight]
      end
      object FindDownBtn: TSpeedButton
        Left = 538
        Top = 11
        Width = 23
        Height = 22
        Action = FindDownAction
        Anchors = [akTop, akRight]
      end
      object SpeedButton1: TSpeedButton
        Left = 35
        Top = 11
        Width = 23
        Height = 22
        Action = CollapseAction
      end
      object SpeedButton2: TSpeedButton
        Left = 10
        Top = 11
        Width = 23
        Height = 22
        Action = ExpandAction
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
        TabOrder = 1
        OnKeyDown = FindEditKeyDown
        OnKeyPress = FindEditKeyPress
      end
      object AutoCheckBox: TCheckBox
        Left = 68
        Top = 14
        Width = 107
        Height = 17
        Caption = #33258#21205#38283#38281'(&A)'
        TabOrder = 0
        OnClick = AutoCheckBoxClick
      end
      object NamCheckBox: TCheckBox
        Left = 409
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #21517
        TabOrder = 2
      end
      object ValCheckBox: TCheckBox
        Left = 495
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #20516
        TabOrder = 4
      end
      object AtrCheckBox: TCheckBox
        Left = 452
        Top = 14
        Width = 41
        Height = 17
        Anchors = [akTop, akRight]
        Caption = #23646
        TabOrder = 3
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
      OnExecute = FindDownActionExecute
      OnUpdate = FindActionUpdate
    end
    object FindUpAction: TAction
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
  end
  object XMLDocument1: TXMLDocument
    Left = 64
    Top = 24
  end
end
