object CmdFileListDlg: TCmdFileListDlg
  Left = 0
  Top = 0
  HelpContext = 67
  BorderIcons = [biSystemMenu]
  ClientHeight = 372
  ClientWidth = 581
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object OpPanel: TPanel
    Left = 0
    Top = 334
    Width = 581
    Height = 38
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    TabStop = True
    DesignSize = (
      581
      38)
    object OkBtn: TButton
      Left = 488
      Top = 6
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Caption = 'OK'
      Default = True
      TabOrder = 4
      OnClick = OkBtnClick
    end
    object FilterEdit: TLabeledEdit
      Left = 72
      Top = 8
      Width = 235
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 47
      EditLabel.Height = 13
      EditLabel.Caption = #12501#12451#12523#12479'(&F)'
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
      OnChange = FilterEditChange
      OnEnter = FilterEditEnter
      OnExit = FilterEditExit
      OnKeyDown = FilterEditKeyDown
      OnKeyPress = FilterEditKeyPress
    end
    object MigemoCheckBox: TCheckBox
      Left = 314
      Top = 10
      Width = 80
      Height = 17
      Anchors = [akTop, akRight]
      Caption = '&Migemo'
      TabOrder = 1
    end
    object CnfExeCheckBox: TCheckBox
      Left = 483
      Top = 10
      Width = 92
      Height = 17
      Anchors = [akTop, akRight]
      Caption = #30906#23450#21363#23455#34892
      TabOrder = 3
    end
    object PreviewCheckBox: TCheckBox
      Left = 393
      Top = 10
      Width = 86
      Height = 17
      Anchors = [akTop, akRight]
      Caption = #12503#12524#12499#12517#12540
      TabOrder = 2
      OnClick = PreviewCheckBoxClick
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 334
    Width = 581
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 2
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 581
    Height = 334
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object PreviewSplitter: TSplitter
      Left = 0
      Top = 152
      Width = 581
      Height = 4
      Cursor = crVSplit
      Align = alBottom
      AutoSnap = False
    end
    object PreviewPanel: TPanel
      Left = 0
      Top = 156
      Width = 581
      Height = 178
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
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
    end
    object GridPanel: TPanel
      Left = 0
      Top = 17
      Width = 581
      Height = 135
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      DesignSize = (
        581
        135)
      object CmdFileGrid: TStringGrid
        Left = 0
        Top = 0
        Width = 581
        Height = 135
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
  object ListPopupMenu: TPopupMenu
    Left = 79
    Top = 221
    object RefEditCopyItem: TMenuItem
      Action = EditCopyAction
    end
    object RefEditSelAllItem: TMenuItem
      Action = EditSelectAllAction
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
      ShortCut = 32837
      OnExecute = ReqEditActionExecute
      OnUpdate = ReqEditActionUpdate
    end
  end
end
