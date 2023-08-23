object DirHistoryDlg: TDirHistoryDlg
  Left = 0
  Top = 0
  HelpContext = 57
  BorderIcons = [biSystemMenu]
  Caption = #12487#12451#12524#12463#12488#12522#23653#27508
  ClientHeight = 387
  ClientWidth = 415
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
    Top = 0
    Width = 415
    Height = 387
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 411
    ExplicitHeight = 386
    object DirHistListBox: TListBox
      Left = 0
      Top = 22
      Width = 411
      Height = 365
      Style = lbVirtualOwnerDraw
      AutoComplete = False
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      DoubleBuffered = False
      ItemHeight = 13
      MultiSelect = True
      ParentDoubleBuffered = False
      PopupMenu = HistPopupMenu
      TabOrder = 1
      StyleElements = [seBorder]
      OnData = DirHistListBoxData
      OnDblClick = DirHistListBoxDblClick
      OnDrawItem = DirHistListBoxDrawItem
      OnKeyDown = DirHistListBoxKeyDown
      OnKeyPress = DirHistListBoxKeyPress
      ExplicitWidth = 407
      ExplicitHeight = 364
    end
    object InpPanel: TPanel
      Left = 0
      Top = 0
      Width = 415
      Height = 22
      Align = alTop
      Alignment = taLeftJustify
      BevelOuter = bvLowered
      TabOrder = 0
      StyleElements = [seBorder]
      ExplicitWidth = 411
      object InpPaintBox: TPaintBox
        Left = 1
        Top = 1
        Width = 413
        Height = 20
        Align = alClient
        OnPaint = InpPaintBoxPaint
      end
    end
  end
  object HintPanel: TPanel
    Left = 91
    Top = 164
    Width = 240
    Height = 60
    BevelOuter = bvNone
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 1
    Visible = False
    StyleElements = [seBorder]
    object HintLabel1: TLabel
      Left = 7
      Top = 9
      Width = 225
      Height = 20
      Alignment = taCenter
      AutoSize = False
      Caption = '    '
    end
    object HintLabel2: TLabel
      Left = 7
      Top = 31
      Width = 225
      Height = 20
      Alignment = taCenter
      AutoSize = False
      Caption = '    '
    end
  end
  object HistPopupMenu: TPopupMenu
    Left = 40
    Top = 104
    object AddDirsItem: TMenuItem
      Action = AddDirsAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object ClearAllItem: TMenuItem
      Action = ClearAllAction
    end
    object ClearFltItem: TMenuItem
      Action = ClearFltAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object CopyAllItem: TMenuItem
      Action = CopyAllAction
    end
    object PropertyItem: TMenuItem
      Action = PropertyAction
    end
    object Sep_3: TMenuItem
      Caption = '-'
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 40
    Top = 32
    object ClearAllAction: TAction
      OnExecute = ClearAllActionExecute
      OnUpdate = ClearAllActionUpdate
    end
    object ClearFltAction: TAction
      Caption = #32094#12426#36796#12415#34920#31034#20013#12398#23653#27508#12434#21066#38500'(&F)'
      OnExecute = ClearFltActionExecute
      OnUpdate = ClearFltActionUpdate
    end
    object AddDirsAction: TAction
      Caption = #25351#23450#12487#12451#12524#12463#12488#12522#20197#19979#12434#20840#20307#23653#27508#12395#36861#21152'(&S)...'
      OnExecute = AddDirsActionExecute
      OnUpdate = AddDirsActionUpdate
    end
    object CopyAllAction: TAction
      Caption = #19968#35239#12434#12467#12500#12540'(&C)'
      OnExecute = CopyAllActionExecute
      OnUpdate = CopyAllActionUpdate
    end
    object PropertyAction: TAction
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyActionExecute
      OnUpdate = PropertyActionUpdate
    end
  end
end
