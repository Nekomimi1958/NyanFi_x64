object GitViewer: TGitViewer
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 448
  ClientWidth = 710
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
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
      Height = 291
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      OnResize = CommitPanelResize
      object CommitListBox: TListBox
        Left = 0
        Top = 0
        Width = 521
        Height = 291
        Style = lbOwnerDrawFixed
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBlack
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'MeiryoKe_Gothic'
        Font.Style = []
        ParentFont = False
        PopupMenu = CmPopupMenu
        TabOrder = 0
        TabWidth = 40
        OnClick = CommitListBoxClick
        OnDrawItem = CommitListBoxDrawItem
        OnKeyDown = CommitListBoxKeyDown
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
      TabOrder = 1
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
        OnMouseDown = GitListBoxMouseDown
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
        OnClick = RestItemClick
      end
      object MixedResetItem: TMenuItem
        Tag = 1
        Caption = '&Mixed - '#12467#12511#12483#12488#12392#12452#12531#12487#12483#12463#12473#12434#21066#38500
        OnClick = RestItemClick
      end
      object HardResetItem: TMenuItem
        Tag = 2
        Caption = '&Hard - '#12377#12409#12390#22793#26356#12434#21066#38500
        OnClick = RestItemClick
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
      Caption = #20182#12398#12502#12521#12531#12481#12418#34920#31034
      OnExecute = ShowBranchesActionExecute
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
      OnUpdate = DiffToolActionUpdate
    end
    object ArchiveAction: TAction
      Caption = 'ZIP'#12450#12540#12459#12452#12502#12434#20316#25104'(&P)...'
      OnExecute = ArchiveActionExecute
      OnUpdate = ArchiveActionUpdate
    end
    object AppFextColorAction: TAction
      Caption = #25313#24373#23376#21029#37197#33394#12434#36969#29992'(&X)'
      OnExecute = AppFextColorActionExecute
    end
  end
  object CmPopupMenu: TPopupMenu
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
    object Sep_c_3: TMenuItem
      Caption = '-'
    end
    object ShowBranchesItem: TMenuItem
      Action = ShowBranchesAction
    end
  end
  object DiffPopupMenu: TPopupMenu
    Left = 293
    Top = 311
    object DiffToolItem: TMenuItem
      Action = DiffToolAction
    end
    object Sep_d_1: TMenuItem
      Caption = '-'
    end
    object AppFextColItem: TMenuItem
      Action = AppFextColorAction
    end
  end
end
