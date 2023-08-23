object Calculator: TCalculator
  Left = 0
  Top = 0
  HelpContext = 87
  BorderIcons = [biSystemMenu]
  Caption = #38651#21331
  ClientHeight = 69
  ClientWidth = 392
  Color = clBtnFace
  Constraints.MinWidth = 320
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  StyleElements = []
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object LineEdit: TEdit
    Left = 0
    Top = 0
    Width = 392
    Height = 23
    Align = alTop
    PopupMenu = UserModule.EditPopupMenuE
    TabOrder = 0
    OnKeyDown = LineEditKeyDown
    OnKeyPress = LineEditKeyPress
  end
  object HistComboBox: TComboBox
    Left = 0
    Top = 23
    Width = 392
    Height = 23
    Align = alTop
    DropDownCount = 12
    TabOrder = 1
    OnKeyDown = HistComboBoxKeyDown
    OnKeyPress = LineEditKeyPress
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 48
    Width = 392
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 2
    TabStop = False
    ExplicitTop = 50
  end
  object ToolBar1: TToolBar
    Left = 0
    Top = 48
    Width = 392
    Height = 21
    Align = alBottom
    AutoSize = True
    ButtonHeight = 21
    ButtonWidth = 50
    DrawingStyle = dsGradient
    List = True
    AllowTextButtons = True
    TabOrder = 3
    StyleElements = []
    ExplicitTop = 50
    object DegBtn: TToolButton
      Left = 0
      Top = 0
      Action = AngleAction
      Style = tbsTextButton
    end
    object SpBtn1: TToolButton
      Left = 33
      Top = 0
      Width = 12
      ImageIndex = 0
      Style = tbsSeparator
    end
    object RefDefBtn: TToolButton
      Left = 45
      Top = 0
      Hint = #23450#25968#12434#21442#29031
      Caption = '&DEFINE'
      ImageIndex = 0
      Style = tbsTextButton
      OnClick = RefDefBtnClick
    end
    object NowBtn: TToolButton
      Left = 99
      Top = 0
      Hint = #29694#22312#26178#21051
      Caption = 'N&OW'
      ImageIndex = 1
      Style = tbsTextButton
      OnClick = NowBtnClick
    end
    object SpBtn2: TToolButton
      Left = 144
      Top = 0
      Width = 16
      Caption = 'SpBtn2'
      ImageIndex = 1
      Style = tbsSeparator
    end
    object ToDecBtn: TToolButton
      Left = 160
      Top = 0
      Action = ToHexDecAction
      Style = tbsTextButton
    end
    object NotBtn: TToolButton
      Left = 193
      Top = 0
      Action = NotAction
      Style = tbsTextButton
    end
    object SpBtn3: TToolButton
      Left = 227
      Top = 0
      Width = 16
      Caption = 'SpBtn3'
      ImageIndex = 3
      Style = tbsSeparator
    end
    object AcBtn: TToolButton
      Left = 243
      Top = 0
      Hint = #20837#21147#27396#12434#12463#12522#12450
      Caption = '&AC'
      ImageIndex = 0
      Style = tbsTextButton
      OnClick = AcBtnClick
    end
  end
  object ActionList1: TActionList
    Left = 240
    object ToHexDecAction: TAction
      Caption = 'H&EX'
      Hint = '16'#36914#25968#12395#22793#25563
      OnExecute = ToHexDecActionExecute
      OnUpdate = ToHexDecActionUpdate
    end
    object NotAction: TAction
      Caption = '&NOT'
      OnExecute = NotActionExecute
      OnUpdate = NotActionUpdate
    end
    object AngleAction: TAction
      Caption = 'DEG'
      OnExecute = AngleActionExecute
      OnUpdate = AngleActionUpdate
    end
    object EditDefAction: TAction
      Caption = #23450#32681#12501#12449#12452#12523#12398#32232#38598'(&E)'
      OnExecute = EditDefActionExecute
    end
  end
  object ConstPopupMenu: TPopupMenu
    Left = 320
  end
end
