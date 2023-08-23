object LoupeForm: TLoupeForm
  Left = 0
  Top = 0
  HelpContext = 7
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #12523#12540#12506
  ClientHeight = 186
  ClientWidth = 196
  Color = clBtnFace
  Constraints.MinHeight = 48
  Constraints.MinWidth = 32
  UseDockManager = True
  DoubleBuffered = True
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  Position = poDesigned
  OnEndDock = FormEndDock
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 185
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 195
    Top = 0
    Width = 1
    Height = 185
    Align = alRight
    Pen.Color = clSilver
  end
  object BorderShape3: TShape
    Left = 0
    Top = 185
    Width = 196
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
  end
  object ClientPanel: TPanel
    Left = 1
    Top = 0
    Width = 194
    Height = 185
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 186
    ExplicitHeight = 173
    object ImgPanel: TPanel
      Left = 0
      Top = 0
      Width = 194
      Height = 164
      Align = alClient
      BevelOuter = bvNone
      Color = clBlack
      ParentBackground = False
      TabOrder = 0
      ExplicitWidth = 186
      ExplicitHeight = 154
      object LoupeImage: TImage
        Left = 0
        Top = 0
        Width = 194
        Height = 164
        Align = alClient
        Center = True
        PopupMenu = PopupMenu1
        OnMouseMove = LoupeImageMouseMove
        ExplicitHeight = 166
      end
    end
    object OptPanel: TPanel
      Left = 0
      Top = 164
      Width = 194
      Height = 21
      Align = alBottom
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 1
      OnMouseMove = LoupeImageMouseMove
      ExplicitTop = 154
      ExplicitWidth = 186
      ExplicitHeight = 19
      object MagToolBar: TToolBar
        Left = 0
        Top = 0
        Width = 194
        Height = 21
        Align = alBottom
        AutoSize = True
        ButtonHeight = 21
        ButtonWidth = 26
        DrawingStyle = dsGradient
        List = True
        PopupMenu = PopupMenu1
        ShowCaptions = True
        TabOrder = 0
        StyleElements = []
        ExplicitWidth = 186
        object ToolButton1: TToolButton
          Tag = 1
          Left = 0
          Top = 0
          Caption = #215'1'
          Grouped = True
          ImageIndex = 0
          Style = tbsCheck
          OnClick = ToolBtnClick
        end
        object ToolButton2: TToolButton
          Tag = 2
          Left = 26
          Top = 0
          Caption = #215'2'
          Grouped = True
          ImageIndex = 1
          Style = tbsCheck
          OnClick = ToolBtnClick
        end
        object ToolButton3: TToolButton
          Tag = 4
          Left = 52
          Top = 0
          Caption = #215'4'
          Grouped = True
          ImageIndex = 2
          Style = tbsCheck
          OnClick = ToolBtnClick
        end
        object ToolButton4: TToolButton
          Tag = 8
          Left = 78
          Top = 0
          Caption = #215'8'
          Grouped = True
          ImageIndex = 3
          Style = tbsCheck
          OnClick = ToolBtnClick
        end
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 112
    Top = 32
    object Mag1Item: TMenuItem
      Action = Mag1Action
    end
    object Mag2Item: TMenuItem
      Action = Mag2Action
    end
    object Mag4Item: TMenuItem
      Action = Mag4Action
    end
    object Mag8Item: TMenuItem
      Action = Mag8Action
    end
    object Sep_p_1: TMenuItem
      Caption = '-'
    end
    object ShowBtnItem: TMenuItem
      Action = ShowBtnAction
    end
  end
  object ActionList1: TActionList
    Left = 32
    Top = 32
    object Mag1Action: TAction
      Caption = #215'1'
      OnExecute = MagActionExecute
      OnUpdate = MagActionUpdate
    end
    object Mag2Action: TAction
      Tag = 1
      Caption = #215'2'
      OnExecute = MagActionExecute
      OnUpdate = MagActionUpdate
    end
    object Mag4Action: TAction
      Tag = 2
      Caption = #215'4'
      OnExecute = MagActionExecute
      OnUpdate = MagActionUpdate
    end
    object Mag8Action: TAction
      Tag = 3
      Caption = #215'8'
      OnExecute = MagActionExecute
      OnUpdate = MagActionUpdate
    end
    object ShowBtnAction: TAction
      Caption = #20493#29575#12508#12479#12531#12398#34920#31034'(&B)'
      OnExecute = ShowBtnActionExecute
      OnUpdate = ShowBtnActionUpdate
    end
  end
end
