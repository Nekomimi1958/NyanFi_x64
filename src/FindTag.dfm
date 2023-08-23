object FindTagForm: TFindTagForm
  Left = 0
  Top = 0
  HelpContext = 83
  BorderIcons = [biSystemMenu]
  ClientHeight = 281
  ClientWidth = 264
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object ListSplitter: TSplitter
    Left = 0
    Top = 177
    Width = 264
    Height = 4
    Cursor = crVSplit
    Align = alBottom
  end
  object ListPanel: TPanel
    Left = 0
    Top = 0
    Width = 264
    Height = 177
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 260
    ExplicitHeight = 176
    object InpPanel: TPanel
      Left = 0
      Top = 0
      Width = 264
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvLowered
      TabOrder = 0
      StyleElements = [seBorder]
      ExplicitWidth = 260
      object InpPaintBox: TPaintBox
        Left = 1
        Top = 1
        Width = 262
        Height = 20
        Align = alClient
        OnPaint = InpPaintBoxPaint
      end
    end
    object TagsListBox: TListBox
      Left = 0
      Top = 22
      Width = 260
      Height = 155
      Style = lbVirtualOwnerDraw
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      ItemHeight = 13
      MultiSelect = True
      ParentDoubleBuffered = False
      PopupMenu = PopupMenu1
      TabOrder = 1
      StyleElements = []
      OnClick = TagsListBoxClick
      OnData = TagsListBoxData
      OnDataObject = TagsListBoxDataObject
      OnDblClick = TagsListBoxDblClick
      OnDrawItem = TagsListBoxDrawItem
      OnKeyDown = TagsListBoxKeyDown
      OnKeyPress = TagsListBoxKeyPress
      ExplicitWidth = 256
      ExplicitHeight = 154
    end
  end
  object InfoPanel: TPanel
    Left = 0
    Top = 181
    Width = 264
    Height = 100
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 180
    ExplicitWidth = 260
    object InfoListBox: TListBox
      Left = 0
      Top = 0
      Width = 260
      Height = 100
      Style = lbOwnerDrawFixed
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      ItemHeight = 13
      MultiSelect = True
      ParentDoubleBuffered = False
      PopupMenu = PopupMenu1
      TabOrder = 0
      StyleElements = []
      OnDblClick = InfoListBoxDblClick
      OnDrawItem = InfoListBoxDrawItem
      OnKeyDown = InfoListBoxKeyDown
      ExplicitWidth = 256
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 48
    Top = 112
    object TagViewItem: TMenuItem
      Action = TagViewAction
    end
    object TagJumpItem: TMenuItem
      Action = TagJumpAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object CopyItem: TMenuItem
      Action = CopyAction
    end
    object SelAllItem: TMenuItem
      Action = SelAllAction
      ShortCut = 16449
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 48
    object TagViewAction: TAction
      Caption = #12499#12517#12450#12540#12391#12480#12452#12524#12463#12488#12479#12464#12472#12515#12531#12503'(&V)'
      OnExecute = TagViewActionExecute
      OnUpdate = PopupItemUpdate
    end
    object TagJumpAction: TAction
      Caption = #12456#12487#12451#12479#12391#12480#12452#12524#12463#12488#12479#12464#12472#12515#12531#12503'(&E)'
      OnExecute = TagJumpActionExecute
      OnUpdate = PopupItemUpdate
    end
    object CopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object SelAllAction: TAction
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = SelAllActionExecute
      OnUpdate = CopyActionUpdate
    end
  end
end
