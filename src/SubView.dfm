object SubViewer: TSubViewer
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  ClientHeight = 248
  ClientWidth = 310
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  PopupMenu = PopupMenu1
  Position = poDesigned
  OnClick = FormClick
  OnCreate = FormCreate
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 247
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 309
    Top = 0
    Width = 1
    Height = 247
    Align = alRight
    Pen.Color = clSilver
  end
  object BorderShape3: TShape
    Left = 0
    Top = 247
    Width = 310
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
  end
  object ImgPanel: TPanel
    Left = 1
    Top = 0
    Width = 308
    Height = 247
    Align = alClient
    BevelOuter = bvNone
    ParentBackground = False
    TabOrder = 0
    StyleElements = []
    ExplicitWidth = 300
    ExplicitHeight = 235
    object Image1: TImage
      Left = 0
      Top = 0
      Width = 308
      Height = 247
      Align = alClient
      Center = True
      PopupMenu = PopupMenu1
      Proportional = True
      OnClick = Image1Click
    end
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 40
    Top = 24
    object RotateLItem: TMenuItem
      Action = RotateLeftAction
    end
    object RotateRItem: TMenuItem
      Action = RotateRightAction
    end
    object FlipHItem: TMenuItem
      Tag = 4
      Action = FlipHorzAction
    end
    object FlipVItem: TMenuItem
      Tag = 5
      Action = FlipVertAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object LockItem: TMenuItem
      Caption = #34920#31034#12434#12525#12483#12463'(&L)'
      OnClick = LockItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object BgColItem: TMenuItem
      Caption = #32972#26223#33394'(&C)'
      OnClick = BgColItemClick
    end
  end
  object ActionList1: TActionList
    Left = 112
    Top = 24
    object RotateLeftAction: TAction
      Tag = 3
      Caption = #24038#12395'90'#24230#22238#36578'(&L)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object RotateRightAction: TAction
      Tag = 1
      Caption = #21491#12395'90'#24230#22238#36578'(&R)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FlipHorzAction: TAction
      Tag = 4
      Caption = #24038#21491#21453#36578'(&H)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FlipVertAction: TAction
      Tag = 5
      Caption = #19978#19979#21453#36578'(&V)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
  end
end
