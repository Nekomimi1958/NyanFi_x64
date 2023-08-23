object CmdFileListDlg: TCmdFileListDlg
  Left = 0
  Top = 0
  HelpContext = 67
  BorderIcons = [biSystemMenu]
  ClientHeight = 372
  ClientWidth = 581
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
  object HiddenCanBtn: TButton
    Left = 0
    Top = 350
    Width = 581
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 1
    ExplicitTop = 349
    ExplicitWidth = 577
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 581
    Height = 350
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 577
    ExplicitHeight = 349
    object PreviewSplitter: TSplitter
      Left = 0
      Top = 168
      Width = 581
      Height = 4
      Cursor = crVSplit
      Align = alBottom
      AutoSnap = False
    end
    object PreviewPanel: TPanel
      Left = 0
      Top = 172
      Width = 581
      Height = 178
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      ExplicitTop = 171
      ExplicitWidth = 577
      object ReferSplitter: TSplitter
        Left = 384
        Top = 0
        Width = 4
        Height = 178
        Align = alRight
      end
      object ReferPanel: TPanel
        Left = 388
        Top = 0
        Width = 193
        Height = 178
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        ExplicitLeft = 384
        object ReferListBox: TListBox
          Left = 0
          Top = 0
          Width = 193
          Height = 178
          Style = lbOwnerDrawFixed
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ItemHeight = 13
          MultiSelect = True
          PopupMenu = ListPopupMenu
          TabOrder = 0
          StyleElements = [seBorder]
          OnDrawItem = ReferListBoxDrawItem
          OnKeyDown = PreviewListBoxKeyDown
        end
      end
      object PrevPanel: TPanel
        Left = 0
        Top = 0
        Width = 384
        Height = 178
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 1
        ExplicitWidth = 380
        object PreviewListBox: TListBox
          Left = 0
          Top = 0
          Width = 384
          Height = 178
          Style = lbOwnerDrawFixed
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          ItemHeight = 13
          MultiSelect = True
          PopupMenu = ListPopupMenu
          TabOrder = 0
          StyleElements = [seBorder]
          OnDrawItem = PreviewListBoxDrawItem
          OnKeyDown = PreviewListBoxKeyDown
          ExplicitWidth = 380
        end
      end
    end
    object CmdFileHeader: THeaderControl
      Left = 0
      Top = 0
      Width = 581
      Height = 17
      Sections = <
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 65
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 50
        end
        item
          ImageIndex = -1
          Style = hsOwnerDraw
          Width = 50
        end>
      Style = hsFlat
      OnDrawSection = CmdFileHeaderDrawSection
      OnSectionResize = CmdFileHeaderSectionResize
      OnResize = CmdFileHeaderResize
      ExplicitWidth = 577
    end
    object GridPanel: TPanel
      Left = 0
      Top = 17
      Width = 581
      Height = 151
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitWidth = 577
      ExplicitHeight = 150
      DesignSize = (
        581
        151)
      object CmdFileGrid: TStringGrid
        Left = 0
        Top = 0
        Width = 581
        Height = 151
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Color = clAppWorkSpace
        ColCount = 7
        DefaultRowHeight = 22
        DefaultDrawing = False
        DoubleBuffered = False
        FixedCols = 0
        FixedRows = 0
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = #65325#65331' '#12468#12471#12483#12463
        Font.Style = []
        Options = [goFixedVertLine, goColSizing, goRowSelect, goThumbTracking]
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 0
        StyleElements = [seBorder]
        OnClick = CmdFileGridClick
        OnDblClick = CmdFileGridDblClick
        OnDrawCell = CmdFileGridDrawCell
        OnKeyDown = CmdFileGridKeyDown
        ExplicitWidth = 577
        ExplicitHeight = 150
        ColWidths = (
          64
          64
          64
          64
          64
          64
          64)
        RowHeights = (
          22
          22
          22
          22
          22)
      end
    end
  end
  object OpePanel: TPanel
    Left = 0
    Top = 350
    Width = 581
    Height = 22
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitTop = 349
    ExplicitWidth = 577
    object OkBtn: TButton
      Left = 501
      Top = 0
      Width = 80
      Height = 22
      Align = alRight
      Caption = 'OK'
      Default = True
      TabOrder = 0
      OnClick = OkBtnClick
      ExplicitLeft = 497
    end
    object ToolPanel: TPanel
      Left = 0
      Top = 0
      Width = 501
      Height = 22
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitWidth = 497
      object OpeToolBar: TToolBar
        Left = 0
        Top = 0
        Width = 501
        Height = 21
        AutoSize = True
        ButtonHeight = 21
        ButtonWidth = 92
        DrawingStyle = dsGradient
        List = True
        ShowCaptions = True
        AllowTextButtons = True
        TabOrder = 0
        ExplicitWidth = 497
        object FilterBtn: TToolButton
          Left = 0
          Top = 0
          Caption = #12501#12451#12523#12479'(&F)'
          ImageIndex = 0
          Style = tbsTextButton
          OnClick = FilterBtnClick
        end
        object FilterEdit: TEdit
          Left = 66
          Top = 0
          Width = 120
          Height = 21
          Align = alLeft
          Constraints.MinWidth = 60
          PopupMenu = UserModule.EditPopupMenuE
          TabOrder = 0
          OnChange = FilterEditChange
          OnEnter = FilterEditEnter
          OnExit = FilterEditExit
          OnKeyDown = FilterEditKeyDown
          OnKeyPress = FilterEditKeyPress
        end
        object FilterSplitter: TSplitter
          Left = 186
          Top = 0
          Width = 4
          Height = 21
        end
        object MigemoBtn: TToolButton
          Left = 190
          Top = 0
          Action = MigemoAction
          Style = tbsTextButton
        end
        object PreviewBtn: TToolButton
          Left = 246
          Top = 0
          Action = PreviewAction
          Style = tbsTextButton
        end
        object CnfExeBtn: TToolButton
          Left = 321
          Top = 0
          Action = CnfExeAction
          Style = tbsTextButton
        end
      end
    end
  end
  object ListPopupMenu: TPopupMenu
    Left = 79
    Top = 221
    object RefEditCopyItem: TMenuItem
      Action = EditCopyAction
    end
    object RefEditSelAllItem: TMenuItem
      Action = EditSelectAllAction
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object ActionList1: TActionList
    Left = 169
    Top = 221
    object EditCopyAction: TAction
      Caption = #12467#12500#12540'(&C)'
      OnExecute = EditCopyActionExecute
      OnUpdate = EditCopyActionUpdate
    end
    object EditSelectAllAction: TAction
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = EditSelectAllActionExecute
    end
    object ReqEditAction: TAction
      OnExecute = ReqEditActionExecute
      OnUpdate = ReqEditActionUpdate
    end
    object MigemoAction: TAction
      Caption = '&Migemo'
      OnExecute = MigemoActionExecute
    end
    object PreviewAction: TAction
      Caption = #12503#12524#12499#12517#12540'(&V)'
      OnExecute = PreviewActionExecute
    end
    object CnfExeAction: TAction
      Caption = #30906#23450#21363#23455#34892'(&X)'
      OnExecute = CnfExeActionExecute
    end
  end
end
