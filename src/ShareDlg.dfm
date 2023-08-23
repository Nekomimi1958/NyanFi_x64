object NetShareDlg: TNetShareDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 322
  ClientWidth = 232
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
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object ListPanel: TPanel
    Left = 0
    Top = 29
    Width = 232
    Height = 293
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitWidth = 228
    ExplicitHeight = 292
    object ShareListBox: TListBox
      Left = 0
      Top = 0
      Width = 228
      Height = 293
      Style = lbOwnerDrawFixed
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      PopupMenu = PopupMenu1
      TabOrder = 0
      OnDblClick = ShareListBoxDblClick
      OnDrawItem = ShareListBoxDrawItem
      OnKeyDown = ShareListBoxKeyDown
      OnKeyPress = ShareListBoxKeyPress
      ExplicitWidth = 224
      ExplicitHeight = 292
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 322
    Width = 232
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 2
    TabStop = False
    ExplicitTop = 321
    ExplicitWidth = 228
  end
  object PathTabControl: TTabControl
    Left = 0
    Top = 0
    Width = 232
    Height = 29
    Align = alTop
    DoubleBuffered = False
    OwnerDraw = True
    ParentDoubleBuffered = False
    Style = tsFlatButtons
    TabOrder = 0
    TabStop = False
    OnChange = PathTabControlChange
    OnDrawTab = PathTabControlDrawTab
    ExplicitWidth = 228
  end
  object PopupMenu1: TPopupMenu
    Left = 24
    Top = 48
    object CopyUncItem: TMenuItem
      Action = CopyUncAction
    end
    object CopyUncAllItem: TMenuItem
      Action = CopyUncAllAction
    end
    object CopyPathItem: TMenuItem
      Action = CopyPathAction
    end
    object CopyPathAllItem: TMenuItem
      Action = CopyPathAllAction
    end
  end
  object ActionList1: TActionList
    Left = 104
    Top = 45
    object CopyUncAction: TAction
      Caption = 'UNC'#12497#12473#12434#12467#12500#12540'(&C)'
      OnExecute = CopyUncActionExecute
      OnUpdate = CopyUncActionUpdate
    end
    object CopyUncAllAction: TAction
      Caption = #12377#12409#12390#12398'UNC'#12497#12473#12434#12467#12500#12540'(&A)'
      OnExecute = CopyUncAllActionExecute
      OnUpdate = CopyUncAllActionUpdate
    end
    object CopyPathAction: TAction
      Caption = #12487#12451#12524#12463#12488#12522#12539#12497#12473#12434#12467#12500#12540'(&C)'
      OnExecute = CopyPathActionExecute
      OnUpdate = CopyPathActionUpdate
    end
    object CopyPathAllAction: TAction
      Caption = #12377#12409#12390#12398#12487#12451#12524#12463#12488#12522#12539#12497#12473#12434#12467#12500#12540'(&A)'
      OnExecute = CopyPathAllActionExecute
      OnUpdate = CopyPathAllActionUpdate
    end
  end
end
