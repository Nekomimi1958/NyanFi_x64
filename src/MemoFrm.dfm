object MemoForm: TMemoForm
  Left = 0
  Top = 0
  ClientHeight = 367
  ClientWidth = 590
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
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    590
    367)
  TextHeight = 15
  object OptPanel: TPanel
    Left = 0
    Top = 326
    Width = 590
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      590
      41)
    object CancelBtn: TButton
      Left = 485
      Top = 8
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
    object OkBtn: TButton
      Left = 389
      Top = 8
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object BtnPanel: TPanel
      Left = 572
      Top = 0
      Width = 18
      Height = 41
      Align = alRight
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 2
      DesignSize = (
        18
        41)
      object HideOptBtn: TSpeedButton
        Left = 0
        Top = 22
        Width = 18
        Height = 16
        Anchors = [akLeft, akBottom]
        Flat = True
        OnClick = ChgOptBtnClick
      end
    end
  end
  object BlankPanel: TPanel
    Left = 572
    Top = 353
    Width = 14
    Height = 14
    Anchors = [akRight, akBottom]
    AutoSize = True
    BevelOuter = bvNone
    Color = clAppWorkSpace
    ParentBackground = False
    TabOrder = 2
    Visible = False
    StyleElements = [seBorder]
    object ShowOptBtn: TSpeedButton
      Left = 0
      Top = 0
      Width = 14
      Height = 14
      Flat = True
      OnClick = ChgOptBtnClick
    end
  end
  object MemoBox: TRichEdit
    Left = 0
    Top = 0
    Width = 590
    Height = 326
    Align = alClient
    BorderStyle = bsNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    PlainText = True
    PopupMenu = PopupMenu1
    ScrollBars = ssVertical
    TabOrder = 0
    WantTabs = True
    OnChange = MemoBoxChange
  end
  object PopupMenu1: TPopupMenu
    Left = 48
    Top = 80
    object UndoItem: TMenuItem
      Action = UndoAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object CutItem: TMenuItem
      Action = UserModule.EditCut1
    end
    object CopyItem: TMenuItem
      Action = UserModule.EditCopy1
    end
    object PasteItem: TMenuItem
      Action = PasteAction
    end
    object DelItem: TMenuItem
      Action = UserModule.EditDelete1
    end
    object DelLineItem: TMenuItem
      Action = DelLineAction
    end
    object SelAllItem: TMenuItem
      Action = UserModule.EditSelectAll1
    end
    object Sep3: TMenuItem
      Caption = '-'
    end
    object WebSearchItem: TMenuItem
      Action = WebSearchAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object ColorItem: TMenuItem
      Caption = #37197#33394#12398#35373#23450'(&O)'
      object ColBgItem: TMenuItem
        Caption = #32972#26223#33394#12434#22793#26356'(&B)...'
        OnClick = ColBgItemClick
      end
      object ColFgItem: TMenuItem
        Caption = #25991#23383#33394#12434#22793#26356'(&F)...'
        OnClick = ColFgItemClick
      end
      object ColDefItem: TMenuItem
        Caption = #12487#12501#12457#12523#12488#12395#25147#12377'(&D)'
        OnClick = ColDefItemClick
      end
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 48
    Top = 24
    object PasteAction: TAction
      Caption = #36028#12426#20184#12369'(&P)'
      Hint = #36028#12426#20184#12369'|'#12463#12522#12483#12503#12508#12540#12489#12398#20869#23481#12434#25407#20837#12375#12414#12377
      ShortCut = 16470
      OnExecute = PasteActionExecute
      OnUpdate = PasteActionUpdate
    end
    object WebSearchAction: TAction
      Caption = 'Web '#12391#26908#32034'(&S)'
      OnExecute = WebSearchActionExecute
      OnUpdate = WebSearchActionUpdate
    end
    object UndoAction: TAction
      Caption = #20803#12395#25147#12377'(&Z)'
      ShortCut = 16474
      OnExecute = UndoActionExecute
      OnUpdate = UndoActionUpdate
    end
    object DelLineAction: TAction
      Caption = #12459#12540#12477#12523#34892#12434#21066#38500'(&Y)'
      ShortCut = 16473
      OnExecute = DelLineActionExecute
      OnUpdate = DelLineActionUpdate
    end
  end
end
