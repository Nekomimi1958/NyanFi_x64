object GitViewer: TGitViewer
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 448
  ClientWidth = 710
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
  object CommitSplitter: TSplitter
    Left = 185
    Top = 0
    Width = 4
    Height = 448
  end
  object RightPanel: TPanel
    Left = 189
    Top = 0
    Width = 521
    Height = 448
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object DiffSplitter: TSplitter
      Left = 0
      Top = 291
      Width = 521
      Height = 4
      Cursor = crVSplit
      Align = alBottom
    end
    object CommitPanel: TPanel
      Left = 0
      Top = 0
      Width = 521
      Height = 272
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      OnResize = CommitPanelResize
      object CommitListBox: TListBox
        Left = 0
        Top = 0
        Width = 521
        Height = 272
        Style = lbOwnerDrawFixed
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBlack
        DoubleBuffered = False
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MeiryoKe_Gothic'
        Font.Style = []
        ParentDoubleBuffered = False
        ParentFont = False
        PopupMenu = CmPopupMenu
        TabOrder = 0
        TabWidth = 40
        OnClick = CommitListBoxClick
        OnDrawItem = CommitListBoxDrawItem
        OnKeyDown = CommitListBoxKeyDown
        OnKeyPress = GitListBoxKeyPress
        OnMouseDown = GitListBoxMouseDown
      end
    end
    object DiffPanel: TPanel
      Left = 0
      Top = 295
      Width = 521
      Height = 153
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      OnResize = DiffPanelResize
      object DiffListBox: TListBox
        Left = 0
        Top = 0
        Width = 521
        Height = 153
        Style = lbOwnerDrawFixed
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Color = clBlack
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MeiryoKe_Gothic'
        Font.Style = []
        ParentFont = False
        PopupMenu = DiffPopupMenu
        TabOrder = 0
        OnDrawItem = DiffListBoxDrawItem
        OnKeyDown = DiffListBoxKeyDown
        OnKeyPress = GitListBoxKeyPress
        OnMouseDown = GitListBoxMouseDown
      end
    end
    object FindBar: TToolBar
      Left = 0
      Top = 272
      Width = 521
      Height = 19
      Align = alBottom
      AutoSize = True
      ButtonHeight = 19
      ButtonWidth = 51
      DrawingStyle = dsGradient
      List = True
      PopupMenu = ComOptPopupMenu
      ShowCaptions = True
      AllowTextButtons = True
      TabOrder = 1
      object FindBtn: TToolButton
        Left = 0
        Top = 0
        Caption = #26908#32034'(&F)'
        ImageIndex = 0
        Style = tbsTextButton
        OnClick = FindBtnClick
      end
      object FindCommitEdit: TEdit
        Left = 54
        Top = 0
        Width = 101
        Height = 19
        Align = alLeft
        Constraints.MinWidth = 60
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 0
        OnChange = FindCommitEditChange
        OnEnter = FindCommitEditEnter
        OnExit = FindCommitEditExit
        OnKeyDown = FindCommitEditKeyDown
        OnKeyPress = FindCommitEditKeyPress
      end
      object FindSplitter: TSplitter
        Left = 155
        Top = 0
        Width = 4
        Height = 19
      end
      object ToolButton2: TToolButton
        Left = 159
        Top = 0
        Action = FindUpAction
        Style = tbsTextButton
      end
      object ToolButton3: TToolButton
        Left = 181
        Top = 0
        Action = FindDownAction
        Style = tbsTextButton
      end
      object ToolButton4: TToolButton
        Left = 203
        Top = 0
        Width = 40
        Caption = 'ToolButton4'
        ImageIndex = 1
        Style = tbsSeparator
      end
      object ToolButton1: TToolButton
        Left = 243
        Top = 0
        Caption = #26356#26032'(&U)'
        ImageIndex = 0
        Style = tbsTextButton
        OnClick = UpdateBtnClick
      end
      object ToolButton5: TToolButton
        Left = 298
        Top = 0
        Action = ConsoleAction
        Style = tbsTextButton
      end
      object ToolButton6: TToolButton
        Left = 347
        Top = 0
        Action = GuiAction
        Style = tbsTextButton
      end
    end
  end
  object HiddenCanBtn: TButton
    Left = 710
    Top = 0
    Width = 0
    Height = 448
    Align = alRight
    Cancel = True
    ModalResult = 2
    TabOrder = 1
    TabStop = False
  end
  object BranchPanel: TPanel
    Left = 0
    Top = 0
    Width = 185
    Height = 448
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 2
    OnResize = BranchPanelResize
    object BranchListBox: TListBox
      Left = 0
      Top = 0
      Width = 183
      Height = 448
      Style = lbOwnerDrawFixed
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      Color = clBlack
      Font.Charset = ANSI_CHARSET
      Font.Color = clWhite
      Font.Height = -12
      Font.Name = 'Meiryo UI'
      Font.Style = []
      ItemHeight = 15
      ParentFont = False
      PopupMenu = BrPopupMenu
      TabOrder = 0
      OnDblClick = BranchListBoxDblClick
      OnDrawItem = BranchListBoxDrawItem
      OnKeyDown = BranchListBoxKeyDown
      OnKeyPress = GitListBoxKeyPress
      OnMouseDown = GitListBoxMouseDown
    end
  end
  object BrPopupMenu: TPopupMenu
    OnPopup = BrPopupMenuPopup
    Left = 64
    Top = 120
    object CheckoutItem: TMenuItem
      Action = ChckoutAction
    end
    object CreBranchItem: TMenuItem
      Action = CreBranchAction
    end
    object DelBranchItem: TMenuItem
      Action = DelBranchAction
    end
    object RenBranchItem: TMenuItem
      Action = RenBranchAction
    end
    object MergeItem: TMenuItem
      Action = MergeAction
    end
    object SetTagItem: TMenuItem
      Action = SetTagAction
    end
    object ResetItem: TMenuItem
      Caption = #12467#12511#12483#12488#12434#19968#12388#25147#12377
      object SoftResetItem: TMenuItem
        Caption = '&Soft - '#12467#12511#12483#12488#12434#21066#38500
        OnClick = ResetItemClick
      end
      object MixedResetItem: TMenuItem
        Tag = 1
        Caption = '&Mixed - '#12467#12511#12483#12488#12392#12452#12531#12487#12483#12463#12473#12434#21066#38500
        OnClick = ResetItemClick
      end
      object HardResetItem: TMenuItem
        Tag = 2
        Caption = '&Hard - '#12377#12409#12390#22793#26356#12434#21066#38500
        OnClick = ResetItemClick
      end
    end
  end
  object ActionList1: TActionList
    Left = 64
    Top = 32
    object ChckoutAction: TAction
      Caption = #12481#12455#12483#12463#12450#12454#12488'(&C)'
      OnExecute = ChckoutActionExecute
      OnUpdate = InactBranchActionUpdate
    end
    object CreBranchAction: TAction
      Caption = #12502#12521#12531#12481#12434#20316#25104
      OnExecute = CreBranchActionExecute
      OnUpdate = ActTagActionUpdate
    end
    object DelBranchAction: TAction
      Caption = #12502#12521#12531#12481#12398#21066#38500
      OnExecute = DelBranchActionExecute
      OnUpdate = InactBranchActionUpdate
    end
    object RenBranchAction: TAction
      Caption = #12502#12521#12531#12481#21517#12398#22793#26356'(&R)'
      OnExecute = RenBranchActionExecute
      OnUpdate = ActTagActionUpdate
    end
    object MergeAction: TAction
      Caption = #12510#12540#12472'(&M)'
      OnExecute = MergeActionExecute
      OnUpdate = InactBranchActionUpdate
    end
    object SetTagAction: TAction
      Caption = #12479#12464#12434#20184#12369#12427'(&T)'
      OnExecute = SetTagActionExecute
      OnUpdate = ActTagActionUpdate
    end
    object DelTagAction: TAction
      Caption = #12479#12464#12398#21066#38500'(&D)'
      OnExecute = DelTagActionExecute
      OnUpdate = DelTagActionUpdate
    end
    object ShowBranchesAction: TAction
      Caption = #20182#12398#12502#12521#12531#12481#12418#34920#31034'(&B)'
      OnExecute = ShowBranchesActionExecute
    end
    object ShowRemoteAction: TAction
      Caption = #12522#12514#12540#12488#21442#29031#12434#34920#31034'(&R)'
      OnExecute = ShowRemoteActionExecute
    end
    object ShowAuthorAction: TAction
      Caption = 'Author '#12398#21517#21069#12434#34920#31034'(&A)'
      OnExecute = ShowAuthorActionExecute
    end
    object CopyCommitIDAction: TAction
      Caption = #12467#12511#12483#12488'ID'#12434#12467#12500#12540
      OnExecute = CopyCommitIDActionExecute
      OnUpdate = CopyCommitIDActionUpdate
    end
    object CopyBranchNameAction: TAction
      Caption = #12502#12521#12531#12481#21517#12434#12467#12500#12540
      OnExecute = CopyBranchNameActionExecute
      OnUpdate = CopyBranchNameActionUpdate
    end
    object DiffToolAction: TAction
      Caption = 'diff'#12484#12540#12523#12434#36215#21205'(&T)'
      OnExecute = DiffToolActionExecute
      OnUpdate = DiffActionUpdate
    end
    object BlameAction: TAction
      Caption = 'git gui blame (&B)'
      OnExecute = BlameActionExecute
      OnUpdate = DiffActionUpdate
    end
    object ArchiveAction: TAction
      Caption = 'ZIP'#12450#12540#12459#12452#12502#12434#20316#25104'(&P)...'
      OnExecute = ArchiveActionExecute
      OnUpdate = ArchiveActionUpdate
    end
    object OpenTmpArcAction: TAction
      Caption = #19968#26178#12450#12540#12459#12452#12502#12392#12375#12390#38283#12367'(&T)'
      OnExecute = OpenTmpArcActionExecute
      OnUpdate = ArchiveActionUpdate
    end
    object AppFextColorAction: TAction
      Caption = #25313#24373#23376#21029#37197#33394#12434#36969#29992'(&X)'
      OnExecute = AppFextColorActionExecute
    end
    object FindUpAction: TAction
      Caption = #9650
      Hint = #19978#26041#21521#12395#26908#32034
      OnExecute = FindUpActionExecute
      OnUpdate = FindUpActionUpdate
    end
    object FindDownAction: TAction
      Caption = #9660
      Hint = #19979#26041#21521#12395#26908#32034
      OnExecute = FindDownActionExecute
      OnUpdate = FindDownActionUpdate
    end
    object GuiAction: TAction
      Caption = '&GUI'
      OnExecute = GuiActionExecute
      OnUpdate = GuiActionUpdate
    end
    object ConsoleAction: TAction
      Caption = '&Console'
      OnExecute = ConsoleActionExecute
      OnUpdate = ConsoleActionUpdate
    end
    object EditFileAction: TAction
      Caption = #12486#12461#12473#12488#12456#12487#12451#12479#12391#38283#12367'(&E)'
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
  end
  object CmPopupMenu: TPopupMenu
    OnPopup = GitPopupMenuPopup
    Left = 297
    Top = 88
    object N2: TMenuItem
      Action = DelTagAction
    end
    object Sep_c_1: TMenuItem
      Caption = '-'
    end
    object CopyBranchNameItem: TMenuItem
      Action = CopyBranchNameAction
    end
    object CopyCommitIDItem: TMenuItem
      Action = CopyCommitIDAction
    end
    object Sep_c_2: TMenuItem
      Caption = '-'
    end
    object ArchiveItem: TMenuItem
      Action = ArchiveAction
    end
    object OpenTmpArcItem: TMenuItem
      Action = OpenTmpArcAction
    end
    object Sep_c_3: TMenuItem
      Caption = '-'
    end
    object ShowBranchesItem: TMenuItem
      Action = ShowBranchesAction
    end
    object ShowRemoteItem: TMenuItem
      Action = ShowRemoteAction
    end
    object ShowAuthorAction1: TMenuItem
      Action = ShowAuthorAction
    end
    object Sep_c_4: TMenuItem
      Caption = '-'
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object DiffPopupMenu: TPopupMenu
    OnPopup = GitPopupMenuPopup
    Left = 293
    Top = 311
    object DiffToolItem: TMenuItem
      Action = DiffToolAction
    end
    object BlameItem: TMenuItem
      Action = BlameAction
    end
    object EditFileItem: TMenuItem
      Action = EditFileAction
    end
    object Sep_d_1: TMenuItem
      Caption = '-'
    end
    object AppFextColItem: TMenuItem
      Action = AppFextColorAction
    end
  end
  object ComOptPopupMenu: TPopupMenu
    Left = 450
    Top = 316
    object SelConsoleItem: TMenuItem
      Caption = 'Console '#12398#36984#25246'(&S)...'
      OnClick = SelConsoleItemClick
    end
    object SelGuiItem: TMenuItem
      Caption = 'GUI'#12484#12540#12523#12398#36984#25246'(&G)...'
      OnClick = SelGuiItemClick
    end
  end
end
