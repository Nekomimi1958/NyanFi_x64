object DebugForm: TDebugForm
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  Caption = #12487#12496#12483#12464
  ClientHeight = 441
  ClientWidth = 384
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object DebugPanel: TPanel
    Left = 0
    Top = 19
    Width = 384
    Height = 422
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object ReferSplitter: TSplitter
      Left = 0
      Top = 225
      Width = 384
      Height = 4
      Cursor = crVSplit
      Align = alBottom
    end
    object ReferPanel: TPanel
      Left = 0
      Top = 229
      Width = 384
      Height = 193
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 0
      object ReferListBox: TListBox
        Left = 0
        Top = 0
        Width = 384
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
      end
    end
    object PrevPanel: TPanel
      Left = 0
      Top = 0
      Width = 384
      Height = 225
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object PreviewListBox: TListBox
        Left = 0
        Top = 0
        Width = 384
        Height = 225
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
      end
    end
  end
  object OpPanel: TPanel
    Left = 0
    Top = 0
    Width = 384
    Height = 19
    Align = alTop
    AutoSize = True
    BevelOuter = bvNone
    TabOrder = 1
    object ToolBar1: TToolBar
      Left = 0
      Top = 0
      Width = 384
      Height = 19
      AutoSize = True
      ButtonHeight = 19
      ButtonWidth = 20
      Caption = 'ToolBar1'
      DrawingStyle = dsGradient
      List = True
      ShowCaptions = True
      TabOrder = 0
      StyleElements = []
      object ToolButton1: TToolButton
        Left = 0
        Top = 0
        Action = StepAction
      end
      object ToolButton2: TToolButton
        Left = 20
        Top = 0
        Action = GoAction
      end
      object ToolButton3: TToolButton
        Left = 40
        Top = 0
        Action = BreakAction
      end
      object SepBtn1: TToolButton
        Left = 60
        Top = 0
        Width = 12
        Caption = 'SepBtn1'
        ImageIndex = 7
        Style = tbsSeparator
      end
      object ToolButton4: TToolButton
        Left = 72
        Top = 0
        Action = QuitAction
      end
      object ToolButton5: TToolButton
        Left = 92
        Top = 0
        Action = ExitAction
      end
      object SepBtn2: TToolButton
        Left = 112
        Top = 0
        Width = 12
        Caption = 'SepBtn2'
        ImageIndex = 0
        Style = tbsSeparator
      end
      object ToolButton6: TToolButton
        Left = 124
        Top = 0
        Action = VarListAction
      end
      object ToolButton7: TToolButton
        Left = 144
        Top = 0
        Action = EditAction
      end
      object SepBtn3: TToolButton
        Left = 164
        Top = 0
        Width = 12
        Caption = 'SepBtn3'
        ImageIndex = 6
        Style = tbsSeparator
      end
      object ToolButton8: TToolButton
        Left = 176
        Top = 0
        Action = HelpAction
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
