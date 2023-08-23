object TaskManDlg: TTaskManDlg
  Left = 0
  Top = 0
  HelpContext = 69
  BorderIcons = [biSystemMenu]
  Caption = #12479#12473#12463#12510#12493#12540#12472#12515
  ClientHeight = 241
  ClientWidth = 616
  Color = clBtnFace
  Constraints.MinWidth = 624
  DoubleBuffered = True
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
  object TaskHeader: THeaderControl
    Left = 0
    Top = 0
    Width = 616
    Height = 17
    Sections = <
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        AllowClick = False
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 65
      end
      item
        ImageIndex = -1
        Style = hsOwnerDraw
        Width = 50
      end>
    Style = hsFlat
    OnDrawSection = TaskHeaderDrawSection
    OnSectionResize = TaskHeaderSectionResize
    OnResize = TaskHeaderResize
    ExplicitWidth = 612
  end
  object OpePanel: TPanel
    Left = 0
    Top = 200
    Width = 616
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 199
    ExplicitWidth = 612
    object btnStartAction: TButton
      Left = 442
      Top = 8
      Width = 70
      Height = 26
      Action = StartAction
      TabOrder = 6
    end
    object CanAllBtn: TButton
      Left = 78
      Top = 8
      Width = 95
      Height = 26
      Action = CanTaskAllAction
      TabOrder = 2
    end
    object CanTaskBtn: TButton
      Left = 6
      Top = 8
      Width = 70
      Height = 26
      Action = CancelTaskAction
      Default = True
      TabOrder = 1
    end
    object CloseBtn: TButton
      Left = 616
      Top = 0
      Width = 0
      Height = 41
      Align = alRight
      Cancel = True
      ModalResult = 2
      TabOrder = 0
      ExplicitLeft = 612
    end
    object PauseBtn: TButton
      Left = 188
      Top = 8
      Width = 95
      Height = 26
      Action = PauseAction
      TabOrder = 3
    end
    object RestartBtn: TButton
      Left = 285
      Top = 8
      Width = 70
      Height = 26
      Action = RestartAction
      TabOrder = 4
    end
    object SuspendBtn: TButton
      Left = 370
      Top = 8
      Width = 70
      Height = 26
      Action = SuspendAction
      TabOrder = 5
    end
    object Button1: TButton
      Left = 514
      Top = 8
      Width = 95
      Height = 26
      Action = ExtStartAction
      TabOrder = 7
    end
  end
  object GridPanel: TPanel
    Left = 0
    Top = 17
    Width = 616
    Height = 183
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 612
    ExplicitHeight = 182
    DesignSize = (
      616
      183)
    object TaskGrid: TStringGrid
      Left = 0
      Top = 0
      Width = 612
      Height = 185
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clAppWorkSpace
      ColCount = 6
      DefaultDrawing = False
      FixedCols = 0
      FixedRows = 0
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = #65325#65331' '#12468#12471#12483#12463
      Font.Style = []
      Options = [goFixedVertLine, goColSizing, goRowSelect, goThumbTracking]
      ParentFont = False
      TabOrder = 0
      StyleElements = [seBorder]
      OnDrawCell = TaskGridDrawCell
      OnKeyDown = TaskGridKeyDown
      ExplicitWidth = 608
      ExplicitHeight = 184
      ColWidths = (
        64
        64
        64
        64
        64
        64)
      RowHeights = (
        24
        24
        24
        24
        24)
    end
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 250
    OnTimer = Timer1Timer
    Left = 16
    Top = 44
  end
  object ActionList1: TActionList
    Left = 80
    Top = 44
    object CancelTaskAction: TAction
      Caption = #20013#27490'(&C)'
      OnExecute = CancelTaskActionExecute
      OnUpdate = CancelTaskActionUpdate
    end
    object CanTaskAllAction: TAction
      Caption = #12377#12409#12390#20013#27490'(&A)'
      OnExecute = CanTaskAllActionExecute
      OnUpdate = CanTaskAllActionUpdate
    end
    object PauseAction: TAction
      Caption = #19968#26086#20572#27490'(&P)'
      OnExecute = PauseActionExecute
      OnUpdate = PauseActionUpdate
    end
    object RestartAction: TAction
      Caption = #20877#38283'(&R)'
      OnExecute = RestartActionExecute
      OnUpdate = RestartActionUpdate
    end
    object SuspendAction: TAction
      OnExecute = SuspendActionExecute
      OnUpdate = SuspendActionUpdate
    end
    object StartAction: TAction
      Caption = #38283#22987'(&S)'
      OnExecute = StartActionExecute
      OnUpdate = StartActionUpdate
    end
    object ExtStartAction: TAction
      Caption = #21106#36796#23455#34892'(&X)'
      OnExecute = ExtStartActionExecute
      OnUpdate = ExtStartActionUpdate
    end
  end
end
