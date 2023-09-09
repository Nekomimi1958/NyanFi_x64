object GitViewer: TGitViewer
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 448
  ClientWidth = 710
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
  object CommitSplitter: TSplitter
    Left = 185
    Top = 0
    Width = 4
    Height = 448
  end
  object R_Panel: TPanel
    Left = 189
    Top = 0
    Width = 521
    Height = 448
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object DiffSplitter: TSplitter
      Left = 0
      Top = 251
      Width = 521
      Height = 4
      Cursor = crVSplit
      Align = alBottom
    end
    object RL_Panel: TPanel
      Left = 0
      Top = 255
      Width = 521
      Height = 193
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      object DiffPanel: TPanel
        Left = 0
        Top = 0
        Width = 521
        Height = 172
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        OnResize = DiffPanelResize
        object DiffListBox: TListBox
          Left = 0
          Top = 0
          Width = 521
          Height = 172
          Style = lbOwnerDrawFixed
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BorderStyle = bsNone
          Color = clBlack
          PopupMenu = DiffPopupMenu
          TabOrder = 0
          StyleElements = [seBorder]
          OnClick = DiffListBoxClick
          OnDrawItem = DiffListBoxDrawItem
          OnEnter = DiffListBoxEnter
          OnKeyDown = DiffListBoxKeyDown
          OnKeyPress = GitListBoxKeyPress
          OnMouseDown = GitListBoxMouseDown
        end
      end
      object DiffBar: TToolBar
        Left = 0
        Top = 172
        Width = 521
        Height = 21
        Align = alBottom
        AutoSize = True
        ButtonHeight = 21
        ButtonWidth = 65
        Caption = 'DiffBar'
        DrawingStyle = dsGradient
        List = True
        AllowTextButtons = True
        TabOrder = 1
        object AddBtn: TToolButton
          Left = 0
          Top = 0
          Action = AddAction
          Style = tbsTextButton
        end
        object ResetBtn: TToolButton
          Left = 53
          Top = 0
          Action = ResetAction
          Style = tbsTextButton
        end
        object DiffSpcBtn1: TToolButton
          Left = 105
          Top = 0
          Width = 8
          Caption = 'DiffSpcBtn1'
          ImageIndex = 0
          Style = tbsSeparator
        end
        object ToolButton1: TToolButton
          Left = 113
          Top = 0
          Action = CommitAction
          Style = tbsTextButton
        end
        object DiffSpcBtn2: TToolButton
          Left = 177
          Top = 0
          Width = 20
          Caption = 'DiffSpcBtn2'
          ImageIndex = 0
          Style = tbsSeparator
        end
        object EditBtn: TToolButton
          Left = 197
          Top = 0
          Hint = #12486#12461#12473#12488#12456#12487#12451#12479#12391#38283#12367
          Action = EditFileAction
          Caption = #32232#38598'(&E)'
          Style = tbsTextButton
        end
        object DetailBtn: TToolButton
          Left = 248
          Top = 0
          Hint = #24046#20998#35443#32048#12434#34920#31034
          Action = DiffDetailAction
          Caption = #35443#32048'(&I)'
          Style = tbsTextButton
        end
        object DiffSpcBtn3: TToolButton
          Left = 296
          Top = 0
          Width = 16
          Caption = 'DiffSpcBtn3'
          ImageIndex = 0
          Style = tbsSeparator
        end
        object DeffToolBtn: TToolButton
          Left = 312
          Top = 0
          Hint = 'diff'#12484#12540#12523#12434#36215#21205
          Action = DiffToolAction
          Caption = '&Diff tool'
          Style = tbsTextButton
        end
        object BlameBtn: TToolButton
          Left = 366
          Top = 0
          Action = BlameAction
          Caption = '&Blame'
          Style = tbsTextButton
        end
      end
    end
    object RU_Panel: TPanel
      Left = 0
      Top = 0
      Width = 521
      Height = 251
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object CommitPanel: TPanel
        Left = 0
        Top = 0
        Width = 521
        Height = 230
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        OnResize = CommitPanelResize
        object CommitListBox: TListBox
          Left = 0
          Top = 0
          Width = 521
          Height = 230
          Style = lbOwnerDrawFixed
          Align = alLeft
          Anchors = [akLeft, akTop, akRight, akBottom]
          BevelOuter = bvNone
          BorderStyle = bsNone
          Color = clBlack
          DoubleBuffered = False
          ParentDoubleBuffered = False
          PopupMenu = CmPopupMenu
          TabOrder = 0
          TabWidth = 40
          StyleElements = [seBorder]
          OnClick = CommitListBoxClick
          OnDrawItem = CommitListBoxDrawItem
          OnKeyDown = CommitListBoxKeyDown
          OnKeyPress = GitListBoxKeyPress
          OnMouseDown = GitListBoxMouseDown
        end
      end
      object FindBar: TToolBar
        Left = 0
        Top = 230
        Width = 521
        Height = 21
        Align = alBottom
        AutoSize = True
        ButtonHeight = 21
        ButtonWidth = 55
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
          Left = 56
          Top = 0
          Width = 101
          Height = 21
          Align = alLeft
          Constraints.MinWidth = 60
          TabOrder = 0
          OnChange = FindCommitEditChange
          OnEnter = FindCommitEditEnter
          OnExit = FindCommitEditExit
          OnKeyDown = FindCommitEditKeyDown
          OnKeyPress = FindCommitEditKeyPress
        end
        object FindSplitter: TSplitter
          Left = 157
          Top = 0
          Width = 4
          Height = 21
        end
        object FindUpBtn: TToolButton
          Left = 161
          Top = 0
          Action = FindUpAction
          Style = tbsTextButton
        end
        object FindDownBtn: TToolButton
          Left = 182
          Top = 0
          Action = FindDownAction
          Style = tbsTextButton
        end
        object FindSpcBtn: TToolButton
          Left = 203
          Top = 0
          Width = 40
          Caption = 'FindSpcBtn'
          ImageIndex = 1
          Style = tbsSeparator
        end
        object UpdateBtn: TToolButton
          Left = 243
          Top = 0
          Action = UpdateLogAction
          Style = tbsTextButton
        end
        object ToolButton2: TToolButton
          Left = 296
          Top = 0
          Action = StashAction
          Style = tbsTextButton
        end
        object FindSpcBtn2: TToolButton
          Left = 347
          Top = 0
          Width = 16
          Caption = 'FindSpcBtn2'
          ImageIndex = 0
          Style = tbsSeparator
        end
        object ConsoleBtn: TToolButton
          Left = 363
          Top = 0
          Action = ConsoleAction
          Style = tbsTextButton
        end
        object GuiBtn: TToolButton
          Left = 417
          Top = 0
          Action = GuiAction
          Style = tbsTextButton
        end
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
      ItemHeight = 15
      PopupMenu = BrPopupMenu
      TabOrder = 0
      StyleElements = [seBorder]
      OnClick = BranchListBoxClick
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
    object BrSetTagItem: TMenuItem
      Action = SetTagAction
    end
    object BrDelTagItem: TMenuItem
      Action = DelTagAction
    end
    object Sep_b_1: TMenuItem
      Caption = '-'
    end
    object ShowRemoteBrItem: TMenuItem
      Action = ShowRBranchAction
    end
    object ShowTagItem: TMenuItem
      Action = ShowTagAction
    end
  end
  object ActionList1: TActionList
    Left = 64
    Top = 16
    object ChckoutAction: TAction
      Caption = #12481#12455#12483#12463#12450#12454#12488'(&C)'
      OnExecute = CheckoutActionExecute
      OnUpdate = CheckoutActionUpdate
    end
    object CreBranchAction: TAction
      Caption = #12502#12521#12531#12481#12434#20316#25104'(&B)'
      OnExecute = CreBranchActionExecute
      OnUpdate = CreBranchActionUpdate
    end
    object DelBranchAction: TAction
      Caption = #12502#12521#12531#12481#12398#21066#38500'(&D)'
      OnExecute = DelBranchActionExecute
      OnUpdate = DelBranchActionUpdate
    end
    object RenBranchAction: TAction
      Caption = #12502#12521#12531#12481#21517#12398#22793#26356'(&R)'
      OnExecute = RenBranchActionExecute
      OnUpdate = RenBranchActionUpdate
    end
    object MergeAction: TAction
      Caption = #12510#12540#12472'(&M)'
      OnExecute = MergeActionExecute
      OnUpdate = MergeActionUpdate
    end
    object CherryPickAction: TAction
      Caption = #12371#12398#12467#12511#12483#12488#12434#12481#12455#12522#12540#12500#12483#12463'(&C)'
      OnExecute = CherryPickActionExecute
      OnUpdate = CherryPickActionUpdate
    end
    object SetTagAction: TAction
      Caption = #12479#12464#12434#20184#12369#12427'(&T)'
      OnExecute = SetTagActionExecute
      OnUpdate = SetTagActionUpdate
    end
    object DelTagAction: TAction
      Caption = #12479#12464#12398#21066#38500'(&D)'
      OnExecute = DelTagActionExecute
      OnUpdate = DelTagActionUpdate
    end
    object StashAction: TAction
      Caption = #36864#36991'(&S)'
      OnExecute = StashActionExecute
      OnUpdate = StashActionUpdate
    end
    object StashPopAction: TAction
      Caption = #24489#24112'/'#21066#38500' (stash pop)'
      OnExecute = StashPopActionExecute
      OnUpdate = StashPopActionUpdate
    end
    object StashApplyAction: TAction
      Caption = #24489#24112' (stash apply)'
      OnExecute = StashApplyActionExecute
      OnUpdate = StashPopActionUpdate
    end
    object StashDropAction: TAction
      Caption = #21066#38500' (stash drop)'
      OnExecute = StashDropActionExecute
      OnUpdate = StashPopActionUpdate
    end
    object CommitInfoAction: TAction
      Caption = #12467#12511#12483#12488#24773#22577#12434#34920#31034'(&I)'
      OnExecute = CommitInfoActionExecute
      OnUpdate = CommitInfoActionUpdate
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
    object ArchiveAction: TAction
      Caption = 'ZIP'#12450#12540#12459#12452#12502#12434#20316#25104'(&P)...'
      OnExecute = ArchiveActionExecute
      OnUpdate = ArchiveActionUpdate
    end
    object OpenTmpArcAction: TAction
      Caption = #19968#26178#12450#12540#12459#12452#12502#12392#12375#12390#38283#12367'(&O)'
      OnExecute = OpenTmpArcActionExecute
      OnUpdate = ArchiveActionUpdate
    end
    object LogThisCommitAction: TAction
      Caption = #12371#12398#12467#12511#12483#12488#12414#12391#12398#23653#27508#12434#34920#31034'(&H)'
      OnExecute = LogThisCommitActionExecute
      OnUpdate = LogThisCommitActionUpdate
    end
    object ShowRBranchAction: TAction
      Caption = #12522#12514#12540#12488#12502#12521#12531#12481#12434#34920#31034'(&R)'
      OnExecute = ShowRBranchActionExecute
    end
    object ShowTagAction: TAction
      Caption = #12479#12464#12434#34920#31034'(&T)'
      OnExecute = ShowTagActionExecute
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
    object EditFileAction: TAction
      Caption = #12486#12461#12473#12488#12456#12487#12451#12479#12391#38283#12367'(&E)'
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
    object CopyFileHashAction: TAction
      Caption = #12371#12398#12501#12449#12452#12523#12398#12495#12483#12471#12517#12434#12467#12500#12540
      OnExecute = CopyFileHashActionExecute
      OnUpdate = CopyFileInfActionUpdate
    end
    object CopyPathAction: TAction
      Caption = #12371#12398#12501#12449#12452#12523#12398#12497#12473#12434#12467#12500#12540
      OnExecute = CopyPathActionExecute
      OnUpdate = CopyFileInfActionUpdate
    end
    object LogThisFileAction: TAction
      Caption = #12371#12398#12501#12449#12452#12523#12398#12467#12511#12483#12488#23653#27508#12434#34920#31034'(&F)'
      OnExecute = LogThisFileActionExecute
      OnUpdate = DiffActionUpdate
    end
    object DiffDetailAction: TAction
      Caption = #12371#12398#12501#12449#12452#12523#12398#24046#20998#35443#32048#12434#34920#31034'(&I)...'
      OnExecute = DiffDetailActionExecute
      OnUpdate = DiffDetailActionUpdate
    end
    object ViewFileAction: TAction
      Caption = #12371#12398#12501#12449#12452#12523#12398#20869#23481#12434#34920#31034'(&V)...'
      OnExecute = ViewFileActionExecute
      OnUpdate = ViewFileActionUpdate
    end
    object UpdateLogAction: TAction
      Caption = #26356#26032'(&U)'
      OnExecute = UpdateLogActionExecute
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
    object FindDownAction: TAction
      Caption = #9660
      Hint = #19979#26041#21521#12395#26908#32034
      OnExecute = FindDownActionExecute
      OnUpdate = FindDownActionUpdate
    end
    object FindUpAction: TAction
      Caption = #9650
      Hint = #19978#26041#21521#12395#26908#32034
      OnExecute = FindUpActionExecute
      OnUpdate = FindUpActionUpdate
    end
    object AddAction: TAction
      Caption = #36861#21152'(&A)'
      Hint = #12452#12531#12487#12483#12463#12473#12395#36861#21152
      OnExecute = AddActionExecute
      OnUpdate = AddActionUpdate
    end
    object AddAAction: TAction
      Caption = #12377#12409#12390#12398#26032#35215#20316#25104#12539#22793#26356#12539#21066#38500#12434#36861#21152' (add -&A)'
      OnExecute = AddAActionExecute
      OnUpdate = AddAllActionUpdate
    end
    object AddUAction: TAction
      Caption = #12377#12409#12390#12398#22793#26356#12539#21066#38500#12434#36861#21152' (add -&u)'
      OnExecute = AddUActionExecute
      OnUpdate = AddAllActionUpdate
    end
    object ResetAction: TAction
      Caption = #21462#28040'(&R)'
      Hint = #12473#12486#12540#12472#12398#21462#12426#28040#12375
      OnExecute = ResetActionExecute
      OnUpdate = ResetActionUpdate
    end
    object ResetAllAction: TAction
      Caption = #12377#12409#12390#12398#12473#12486#12540#12472#12531#12464#12434#21462#12426#28040#12375
      OnExecute = ResetAllActionExecute
      OnUpdate = ResetAllActionUpdate
    end
    object CommitAction: TAction
      Caption = #12467#12511#12483#12488'(&M)'
      OnExecute = CommitActionExecute
      OnUpdate = CommitActionUpdate
    end
  end
  object CmPopupMenu: TPopupMenu
    OnPopup = GitPopupMenuPopup
    Left = 289
    Top = 120
    object SetTagItem: TMenuItem
      Action = SetTagAction
    end
    object DelTagItem: TMenuItem
      Action = DelTagAction
    end
    object CherryPickItem: TMenuItem
      Action = CherryPickAction
    end
    object Sep_c_1: TMenuItem
      Caption = '-'
    end
    object StashPopItem: TMenuItem
      Action = StashPopAction
    end
    object StashApplyItem: TMenuItem
      Action = StashApplyAction
    end
    object StashDropItem: TMenuItem
      Action = StashDropAction
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
    object CopyBranchNameItem: TMenuItem
      Action = CopyBranchNameAction
    end
    object CopyCommitIDItem: TMenuItem
      Action = CopyCommitIDAction
    end
    object CommitInfoItem: TMenuItem
      Action = CommitInfoAction
    end
    object LogThisCommitItem: TMenuItem
      Action = LogThisCommitAction
    end
    object Sep_c_4: TMenuItem
      Caption = '-'
    end
    object AddAItem: TMenuItem
      Action = AddAAction
    end
    object AddUItem: TMenuItem
      Action = AddUAction
    end
    object ResetAllItem: TMenuItem
      Action = ResetAllAction
    end
    object Sep_c_5: TMenuItem
      Caption = '-'
    end
    object ShowBranchesItem: TMenuItem
      Action = ShowBranchesAction
    end
    object ShowRemoteItem: TMenuItem
      Action = ShowRemoteAction
    end
    object ShowAuthorItem: TMenuItem
      Action = ShowAuthorAction
    end
    object FitSizePosItem: TMenuItem
      Action = UserModule.SizePosToFlieListAction
    end
  end
  object DiffPopupMenu: TPopupMenu
    OnPopup = GitPopupMenuPopup
    Left = 289
    Top = 335
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
    object CopyFileHashItem: TMenuItem
      Action = CopyFileHashAction
    end
    object CopyPathItem: TMenuItem
      Action = CopyPathAction
    end
    object Sep_d_2: TMenuItem
      Caption = '-'
    end
    object LogThisFileItem: TMenuItem
      Action = LogThisFileAction
    end
    object DiffDetailItem: TMenuItem
      Action = DiffDetailAction
    end
    object ViewFileItem: TMenuItem
      Action = ViewFileAction
    end
    object Sep_d_3: TMenuItem
      Caption = '-'
    end
    object AddA2Item: TMenuItem
      Action = AddAAction
    end
    object AddU2Item: TMenuItem
      Action = AddUAction
    end
    object ResetAll2Item: TMenuItem
      Action = ResetAllAction
    end
  end
  object ComOptPopupMenu: TPopupMenu
    Left = 602
    Top = 268
    object SelConsoleItem: TMenuItem
      Caption = 'Console '#12398#36984#25246'(&S)...'
      OnClick = SelConsoleItemClick
    end
    object SelGuiItem: TMenuItem
      Caption = 'GUI'#12484#12540#12523#12398#36984#25246'(&G)...'
      OnClick = SelGuiItemClick
    end
  end
  object WaitTimer: TTimer
    Enabled = False
    OnTimer = WaitTimerTimer
    Left = 461
    Top = 119
  end
end
