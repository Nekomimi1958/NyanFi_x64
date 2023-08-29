object ExTxtViewer: TExTxtViewer
  Left = 0
  Top = 0
  HelpContext = 6
  ClientHeight = 561
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  ShowHint = True
  OnActivate = FormActivate
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnDeactivate = FormDeactivate
  OnKeyDown = FormKeyDown
  OnMouseWheel = FormMouseWheel
  OnResize = FormResize
  OnShow = FormShow
  TextHeight = 15
  object TxtViewPanel: TPanel
    Left = 0
    Top = 0
    Width = 624
    Height = 561
    HelpContext = 6
    Align = alClient
    BevelOuter = bvNone
    DoubleBuffered = True
    ParentBackground = False
    ParentDoubleBuffered = False
    TabOrder = 0
    StyleElements = [seBorder]
    object ViewMemo: TRichEdit
      Left = 0
      Top = 0
      Width = 624
      Height = 561
      TabStop = False
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      ParentShowHint = False
      ReadOnly = True
      ShowHint = False
      TabOrder = 1
      Visible = False
      WantReturns = False
    end
    object TxtMainPanel: TPanel
      Left = 0
      Top = 0
      Width = 624
      Height = 561
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object TxtSttHeader: TStatusBar
        Left = 0
        Top = 0
        Width = 624
        Height = 19
        Align = alTop
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -12
        Font.Name = 'Segoe UI'
        Font.Style = []
        Panels = <
          item
            Style = psOwnerDraw
            Width = 50
          end
          item
            Style = psOwnerDraw
            Width = 50
          end
          item
            Style = psOwnerDraw
            Width = 50
          end
          item
            Style = psOwnerDraw
            Width = 50
          end
          item
            Style = psOwnerDraw
            Width = 50
          end>
        UseSystemFont = False
        StyleElements = [seBorder]
        OnDrawPanel = TxtSttHeaderDrawPanel
      end
      object TxtScrollPanel: TPanel
        Left = 0
        Top = 19
        Width = 624
        Height = 542
        Align = alClient
        BevelOuter = bvNone
        DoubleBuffered = True
        ParentBackground = False
        ParentDoubleBuffered = False
        TabOrder = 1
        StyleElements = [seBorder]
        object TextPaintBox: TPaintBox
          Left = 0
          Top = 16
          Width = 607
          Height = 526
          Align = alClient
          Color = clBtnFace
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -12
          Font.Name = 'Segoe UI'
          Font.Style = []
          ParentColor = False
          ParentFont = False
          PopupMenu = ViewPopupMenu
          OnDblClick = TextPaintBoxDblClick
          OnMouseDown = TextPaintBoxMouseDown
          OnMouseMove = TextPaintBoxMouseMove
          OnMouseUp = TextPaintBoxMouseUp
          OnPaint = TextPaintBoxPaint
        end
        object TextRulerBox: TPaintBox
          Left = 0
          Top = 0
          Width = 624
          Height = 16
          Align = alTop
        end
        object TextScrollBar: TScrollBar
          Left = 607
          Top = 16
          Width = 17
          Height = 526
          Align = alRight
          Kind = sbVertical
          Min = 1
          PageSize = 0
          Position = 1
          TabOrder = 0
          TabStop = False
          OnChange = TextScrollBarChange
        end
        object TextCurColPanel: TPanel
          Left = 32
          Top = 221
          Width = 60
          Height = 24
          TabOrder = 1
          Visible = False
          StyleElements = [seBorder]
        end
      end
    end
  end
  object ViewPopupMenu: TPopupMenu
    Left = 56
    Top = 126
    object PopCopyItem: TMenuItem
      Action = CopyAction
    end
    object PopAddCopyItem: TMenuItem
      Action = AddCopyAction
    end
    object PopSelAllItem: TMenuItem
      Action = SelectAllAction
    end
    object PopFindItem: TMenuItem
      Action = FindTextAction
    end
    object PopWebSearchItem: TMenuItem
      Action = WebSearchAction
      OnMeasureItem = PopWebSearchItemMeasureItem
    end
    object Sep_vp_1: TMenuItem
      Caption = '-'
    end
    object PopCodePageItem: TMenuItem
      Action = CodePageAction
      object CP_ShiftJISItem: TMenuItem
        Action = CP_ShiftJisAction
      end
      object CP_JisItem: TMenuItem
        Action = CP_JisAction
      end
      object CP_EUCItem: TMenuItem
        Action = CP_EucAction
      end
      object CP_Latin1Item: TMenuItem
        Action = CP_Latin1Action
      end
      object CP_UTF8Item: TMenuItem
        Action = CP_UTF8Action
      end
      object CP_UTF16Item: TMenuItem
        Action = CP_UTF16Action
      end
      object CP_AutoItem: TMenuItem
        Action = CP_AutoAction
      end
    end
    object Sep_vp_2: TMenuItem
      Caption = '-'
    end
    object PopSortItem: TMenuItem
      Caption = #12477#12540#12488'(&O)'
      object SortAscItem: TMenuItem
        Action = SortAscAction
      end
      object SortDesItem: TMenuItem
        Action = SortDesAction
      end
      object OrgOrderItem: TMenuItem
        Action = OrgOrderAction
      end
    end
    object PopTopIsHdrItem: TMenuItem
      Action = TopIsHdrAction
    end
    object Sep_vp_3: TMenuItem
      Caption = '-'
    end
    object PopExpCsvItem: TMenuItem
      Action = ExportCsvAction
    end
    object PopSaveDumpItem: TMenuItem
      Action = SaveDumpAction
    end
  end
  object ActionList1: TActionList
    Left = 56
    Top = 59
    object CopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = CopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object AddCopyAction: TAction
      Caption = #36861#21152#12467#12500#12540
      OnExecute = AddCopyActionExecute
      OnUpdate = CopyActionUpdate
    end
    object SelectAllAction: TAction
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = SelectAllActionExecute
    end
    object FindTextAction: TAction
      Caption = #26908#32034'(&F)...'
      OnExecute = FindTextActionExecute
    end
    object WebSearchAction: TAction
      Caption = 'Web '#12391#26908#32034'(&S)'
      OnExecute = WebSearchActionExecute
      OnUpdate = WebSearchActionUpdate
    end
    object SortAscAction: TAction
      Caption = #26119#38918'(&A)'
      OnExecute = SortAscActionExecute
      OnUpdate = SortActionUpdate
    end
    object SortDesAction: TAction
      Caption = #38477#38918'(&D)'
      OnExecute = SortDesActionExecute
    end
    object OrgOrderAction: TAction
      Caption = #35299#38500'(&R)'
      OnExecute = OrgOrderActionExecute
    end
    object TopIsHdrAction: TAction
      Caption = #20808#38957#34892#12364#38917#30446#21517'(&H)'
    end
    object CodePageAction: TAction
      Caption = #25991#23383#12467#12540#12489#22793#26356'(&C)'
      OnExecute = PopCodePageActionExecute
      OnUpdate = CodePageActionUpdate
    end
    object CP_ShiftJisAction: TAction
      Tag = 932
      Caption = '&Shift JIS'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_JisAction: TAction
      Tag = 50220
      Caption = '&JIS (ISO-2022-JP)'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_EucAction: TAction
      Tag = 20932
      Caption = '&EUC-JP'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_Latin1Action: TAction
      Tag = 1252
      Caption = 'Latin-1'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_UTF8Action: TAction
      Tag = 65001
      Caption = '&UTF-8'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_UTF16Action: TAction
      Caption = 'U&TF16'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_AutoAction: TAction
      Caption = #33258#21205#21028#23450'(&A)'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object SaveDumpAction: TAction
      Caption = #12480#12531#12503#12522#12473#12488#12434#20445#23384'(&D)...'
      OnExecute = SaveDumpActionExecute
      OnUpdate = SaveDumpActionUpdate
    end
    object ExportCsvAction: TAction
      Caption = 'CSV/TSV'#12456#12463#12473#12509#12540#12488'(&X)...'
      OnExecute = ExportCsvActionExecute
      OnUpdate = ExportCsvActionUpdate
    end
  end
end
