object DebugForm: TDebugForm
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = #12487#12496#12483#12464
  ClientHeight = 441
  ClientWidth = 384
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 440
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 383
    Top = 0
    Width = 1
    Height = 440
    Align = alRight
    Pen.Color = clSilver
  end
  object BorderShape3: TShape
    Left = 0
    Top = 440
    Width = 384
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
  end
  object ClientPanel: TPanel
    Left = 1
    Top = 0
    Width = 382
    Height = 440
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 374
    ExplicitHeight = 428
    object DebugPanel: TPanel
      Left = 0
      Top = 21
      Width = 382
      Height = 419
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitTop = 19
      ExplicitWidth = 374
      ExplicitHeight = 409
      object ReferSplitter: TSplitter
        Left = 0
        Top = 222
        Width = 382
        Height = 4
        Cursor = crVSplit
        Align = alBottom
        ExplicitTop = 224
      end
      object ReferPanel: TPanel
        Left = 0
        Top = 226
        Width = 382
        Height = 193
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        ExplicitTop = 216
        ExplicitWidth = 374
        object ReferListBox: TListBox
          Left = 0
          Top = 0
          Width = 374
          Height = 193
          Style = lbOwnerDrawFixed
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
          StyleElements = [seBorder]
          OnDrawItem = ReferListBoxDrawItem
          OnKeyDown = ReferListBoxKeyDown
          ExplicitWidth = 366
        end
      end
      object PrevPanel: TPanel
        Left = 0
        Top = 0
        Width = 382
        Height = 222
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        ExplicitWidth = 374
        ExplicitHeight = 212
        object PreviewListBox: TListBox
          Left = 0
          Top = 0
          Width = 374
          Height = 222
          Style = lbVirtualOwnerDraw
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
          StyleElements = [seBorder]
          OnData = PreviewListBoxData
          OnDrawItem = PreviewListBoxDrawItem
          OnEnter = PreviewListBoxEnter
          ExplicitWidth = 366
          ExplicitHeight = 212
        end
      end
    end
    object OpPanel: TPanel
      Left = 0
      Top = 0
      Width = 382
      Height = 21
      Align = alTop
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitWidth = 374
      ExplicitHeight = 19
      object ToolBar1: TToolBar
        Left = 0
        Top = 0
        Width = 382
        Height = 21
        AutoSize = True
        ButtonHeight = 21
        ButtonWidth = 21
        Caption = 'ToolBar1'
        DrawingStyle = dsGradient
        List = True
        ShowCaptions = True
        TabOrder = 0
        StyleElements = []
        ExplicitWidth = 374
        object ToolButton1: TToolButton
          Left = 0
          Top = 0
          Action = StepAction
        end
        object ToolButton2: TToolButton
          Left = 21
          Top = 0
          Action = GoAction
        end
        object ToolButton3: TToolButton
          Left = 42
          Top = 0
          Action = BreakAction
        end
        object SepBtn1: TToolButton
          Left = 63
          Top = 0
          Width = 12
          Caption = 'SepBtn1'
          ImageIndex = 7
          Style = tbsSeparator
        end
        object ToolButton4: TToolButton
          Left = 75
          Top = 0
          Action = QuitAction
        end
        object ToolButton5: TToolButton
          Left = 96
          Top = 0
          Action = ExitAction
        end
        object SepBtn2: TToolButton
          Left = 117
          Top = 0
          Width = 12
          Caption = 'SepBtn2'
          ImageIndex = 0
          Style = tbsSeparator
        end
        object ToolButton6: TToolButton
          Left = 129
          Top = 0
          Action = VarListAction
        end
        object ToolButton7: TToolButton
          Left = 150
          Top = 0
          Action = EditAction
        end
        object SepBtn3: TToolButton
          Left = 171
          Top = 0
          Width = 12
          Caption = 'SepBtn3'
          ImageIndex = 6
          Style = tbsSeparator
        end
        object ToolButton8: TToolButton
          Left = 183
          Top = 0
          Action = HelpAction
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 24
    Top = 35
    object EditCopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = EditCopyActionExecute
      OnUpdate = EditCopyActionUpdate
    end
    object StepAction: TAction
      Caption = 'S'
      Hint = #12473#12486#12483#12503#23455#34892
      OnExecute = StepActionExecute
      OnUpdate = StepActionUpdate
    end
    object GoAction: TAction
      Caption = 'G'
      Hint = #23455#34892
      OnExecute = GoActionExecute
      OnUpdate = GoActionUpdate
    end
    object BreakAction: TAction
      Caption = 'B'
      Hint = #20572#27490
      OnExecute = BreakActionExecute
      OnUpdate = BreakActionUpdate
    end
    object VarListAction: TAction
      Caption = 'V'
      Hint = #22793#25968#19968#35239
      OnExecute = VarListActionExecute
    end
    object QuitAction: TAction
      Caption = 'Q'
      Hint = #12487#12496#12483#12464#12514#12540#12489#32066#20102
      OnExecute = QuitActionExecute
      OnUpdate = QuitActionUpdate
    end
    object ExitAction: TAction
      Caption = 'X'
      Hint = #12467#12510#12531#12489#12398#23455#34892#12434#32066#20102
      OnExecute = ExitActionExecute
      OnUpdate = ExitActionUpdate
    end
    object EditAction: TAction
      Caption = 'E'
      Hint = #12456#12487#12451#12479#12391#38283#12367
      OnExecute = EditActionExecute
      OnUpdate = EditActionUpdate
    end
    object HelpAction: TAction
      Caption = 'H'
      Hint = #12504#12523#12503#12434#34920#31034
      OnExecute = HelpActionExecute
    end
  end
end
