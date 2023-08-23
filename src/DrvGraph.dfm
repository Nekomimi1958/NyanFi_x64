object DriveGraph: TDriveGraph
  Left = 0
  Top = 0
  HelpContext = 86
  BorderIcons = [biSystemMenu]
  Caption = #12489#12521#12452#12502#20351#29992#29575#25512#31227
  ClientHeight = 289
  ClientWidth = 390
  Color = clBtnFace
  Constraints.MinWidth = 400
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
  OnMouseWheel = FormMouseWheel
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object HiddenCanBtn: TButton
    Left = 0
    Top = 270
    Width = 390
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 0
    TabStop = False
    ExplicitTop = 269
    ExplicitWidth = 386
  end
  object GraphScrollBox: TScrollBox
    Left = 0
    Top = 21
    Width = 390
    Height = 249
    HorzScrollBar.Visible = False
    VertScrollBar.Tracking = True
    Align = alClient
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    TabOrder = 2
    ExplicitTop = 19
    ExplicitWidth = 386
    ExplicitHeight = 250
    object PaintBox1: TPaintBox
      Left = 0
      Top = 0
      Width = 390
      Height = 249
      Align = alClient
      OnMouseDown = PaintBox1MouseDown
      OnPaint = PaintBox1Paint
      ExplicitWidth = 392
      ExplicitHeight = 251
    end
    object HiddenEdit: TEdit
      Left = 0
      Top = 0
      Width = 0
      Height = 23
      PopupMenu = PopupMenu1
      ReadOnly = True
      TabOrder = 0
      OnEnter = HiddenEditEnterExit
      OnExit = HiddenEditEnterExit
      OnKeyDown = HiddenEditKeyDown
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 270
    Width = 390
    Height = 19
    Panels = <
      item
        Style = psOwnerDraw
        Width = 600
      end>
    StyleElements = [seBorder]
    OnDrawPanel = StatusBar1DrawPanel
    ExplicitTop = 269
    ExplicitWidth = 386
  end
  object OptToolBar: TToolBar
    Left = 0
    Top = 0
    Width = 390
    Height = 21
    AutoSize = True
    ButtonHeight = 21
    ButtonWidth = 61
    Caption = 'OptToolBar'
    DrawingStyle = dsGradient
    List = True
    AllowTextButtons = True
    TabOrder = 1
    ExplicitWidth = 386
    ExplicitHeight = 60
    object DriveComboBox: TComboBox
      Left = 0
      Top = 0
      Width = 120
      Height = 23
      Align = alLeft
      Style = csDropDownList
      TabOrder = 0
      OnChange = DriveComboBoxChange
      OnKeyDown = OptComboBoxKeyDown
    end
    object OptSplitter: TSplitter
      Left = 120
      Top = 0
      Width = 4
      Height = 21
    end
    object SizeBtn: TToolButton
      Left = 124
      Top = 0
      Caption = #26834#24133
      ImageIndex = 1
      Style = tbsTextButton
      OnClick = SizeBtnClick
    end
    object SizeComboBox: TComboBox
      Left = 166
      Top = 0
      Width = 50
      Height = 23
      Align = alRight
      Style = csDropDownList
      DropDownCount = 16
      TabOrder = 1
      OnChange = SizeComboBoxChange
      OnKeyDown = OptComboBoxKeyDown
    end
    object ToolButton3: TToolButton
      Left = 216
      Top = 0
      Width = 18
      ImageIndex = 2
      Style = tbsSeparator
    end
    object ToolButton4: TToolButton
      Left = 234
      Top = 0
      Action = OldOdrAction
      Style = tbsTextButton
    end
    object ToolButton5: TToolButton
      Left = 282
      Top = 0
      Action = MinMaxAction
      Caption = '&Min/Max'
      Style = tbsTextButton
    end
    object ToolButton1: TToolButton
      Left = 342
      Top = 0
      Action = CursorAction
      Caption = '&Cursor'
      Style = tbsTextButton
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 36
    object OldOdrAction: TAction
      Caption = #21476#12356#38918
      OnExecute = ToggleActionExecute
    end
    object MinMaxAction: TAction
      Caption = 'Min/Max'
      Hint = #26368#23569'/'#26368#22823#20351#29992#29575#12521#12452#12531#12434#34920#31034
      OnExecute = ToggleActionExecute
    end
    object CursorAction: TAction
      Caption = 'Cursor'
      Hint = #12459#12540#12477#12523#20301#32622#12398#20351#29992#29575#12521#12452#12531#12434#34920#31034
      OnExecute = ToggleActionExecute
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 128
    Top = 35
  end
end
