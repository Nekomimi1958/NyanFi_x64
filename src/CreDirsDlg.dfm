object CreateDirsDlg: TCreateDirsDlg
  Left = 0
  Top = 0
  HelpContext = 79
  BorderIcons = [biSystemMenu]
  Caption = #12487#12451#12524#12463#12488#12522#19968#25324#20316#25104
  ClientHeight = 371
  ClientWidth = 410
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 420
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 351
    Width = 410
    Height = 20
    Panels = <
      item
        Style = psOwnerDraw
        Width = 100
      end
      item
        Style = psOwnerDraw
        Width = 50
      end>
    OnDrawPanel = StatusBar1DrawPanel
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 410
    Height = 351
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Panel1: TPanel
      Left = 0
      Top = 119
      Width = 410
      Height = 232
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        410
        232)
      object CreateBtn: TButton
        Left = 230
        Top = 200
        Width = 80
        Height = 26
        Action = CreateAction
        Anchors = [akRight, akBottom]
        Default = True
        ModalResult = 1
        TabOrder = 3
      end
      object CancelBtn: TButton
        Left = 322
        Top = 200
        Width = 80
        Height = 26
        Anchors = [akRight, akBottom]
        Cancel = True
        Caption = #12461#12515#12531#12475#12523
        ModalResult = 2
        TabOrder = 4
      end
      object SirealGroupBox: TGroupBox
        Left = 8
        Top = 6
        Width = 394
        Height = 63
        Caption = #36899#30058
        TabOrder = 0
        DesignSize = (
          394
          63)
        object StartEdit: TLabeledEdit
          Left = 8
          Top = 32
          Width = 100
          Height = 21
          Alignment = taRightJustify
          EditLabel.Width = 36
          EditLabel.Height = 13
          EditLabel.Caption = #38283#22987#20516
          NumbersOnly = True
          TabOrder = 0
        end
        object IncEdit: TLabeledEdit
          Left = 112
          Top = 32
          Width = 90
          Height = 21
          Alignment = taRightJustify
          EditLabel.Width = 24
          EditLabel.Height = 13
          EditLabel.Caption = #22679#20998
          NumbersOnly = True
          TabOrder = 1
        end
        object AddSerBtn: TButton
          Left = 334
          Top = 31
          Width = 50
          Height = 24
          Action = AddSerAction
          Anchors = [akTop, akRight]
          TabOrder = 4
        end
        object PreSerRadioBtn: TRadioButton
          Left = 218
          Top = 34
          Width = 54
          Height = 17
          Caption = #21069#32622
          Checked = True
          TabOrder = 2
          TabStop = True
        end
        object PstSerRadioBtn: TRadioButton
          Left = 274
          Top = 34
          Width = 54
          Height = 17
          Caption = #24460#32622
          TabOrder = 3
        end
      end
      object StrGroupBox: TGroupBox
        Left = 8
        Top = 143
        Width = 394
        Height = 50
        Caption = #25991#23383#21015
        TabOrder = 2
        DesignSize = (
          394
          50)
        object AddStrBtn: TButton
          Left = 334
          Top = 17
          Width = 50
          Height = 24
          Action = AddStrAction
          Anchors = [akTop, akRight]
          TabOrder = 3
        end
        object PstStrRadioBtn: TRadioButton
          Left = 274
          Top = 20
          Width = 54
          Height = 17
          Caption = #24460#32622
          TabOrder = 2
        end
        object PreStrRadioBtn: TRadioButton
          Left = 218
          Top = 20
          Width = 54
          Height = 17
          Caption = #21069#32622
          Checked = True
          TabOrder = 1
          TabStop = True
        end
        object StrEdit: TEdit
          Left = 8
          Top = 18
          Width = 194
          Height = 21
          TabOrder = 0
        end
      end
      object DateGroupBox: TGroupBox
        Left = 8
        Top = 71
        Width = 394
        Height = 66
        Caption = #26085#20184
        TabOrder = 1
        DesignSize = (
          394
          66)
        object Label1: TLabel
          Left = 112
          Top = 15
          Width = 36
          Height = 13
          Caption = #38283#22987#26085
        end
        object AddDateBtn: TButton
          Left = 334
          Top = 33
          Width = 50
          Height = 24
          Action = AddDateAction
          Anchors = [akTop, akRight]
          TabOrder = 4
        end
        object PstDateRadioBtn: TRadioButton
          Left = 274
          Top = 36
          Width = 54
          Height = 17
          Caption = #24460#32622
          TabOrder = 3
        end
        object PreDateRadioBtn: TRadioButton
          Left = 218
          Top = 36
          Width = 54
          Height = 17
          Caption = #21069#32622
          Checked = True
          TabOrder = 2
          TabStop = True
        end
        object DtFmtEdit: TLabeledEdit
          Left = 8
          Top = 34
          Width = 100
          Height = 21
          EditLabel.Width = 24
          EditLabel.Height = 13
          EditLabel.Caption = #26360#24335
          TabOrder = 0
        end
        object DateMaskEdit: TMaskEdit
          Left = 112
          Top = 34
          Width = 90
          Height = 21
          EditMask = '!9999/99/99;1;_'
          MaxLength = 10
          PopupMenu = UserModule.EditPopupMenuE
          TabOrder = 1
          Text = '    /  /  '
        end
      end
    end
    object ListMemo: TMemo
      Left = 0
      Top = 0
      Width = 410
      Height = 119
      Align = alClient
      Lines.Strings = (
        '')
      PopupMenu = PopupMenu1
      ScrollBars = ssVertical
      TabOrder = 0
      WordWrap = False
      OnKeyDown = ListMemoKeyDown
    end
  end
  object ActionList1: TActionList
    Left = 32
    Top = 16
    object CreateAction: TAction
      Caption = #20316#25104
      OnExecute = CreateActionExecute
      OnUpdate = CreateActionUpdate
    end
    object AddSerAction: TAction
      Caption = #20184#21152
      OnExecute = AddSerActionExecute
      OnUpdate = AddSerActionUpdate
    end
    object AddStrAction: TAction
      Caption = #20184#21152
      OnExecute = AddStrActionExecute
      OnUpdate = AddStrActionUpdate
    end
    object AddDateAction: TAction
      Caption = #20184#21152
      OnExecute = AddDateActionExecute
      OnUpdate = AddDateActionUpdate
    end
    object SaveListAction: TAction
      Caption = #12522#12473#12488#12395#21517#21069#12434#20184#12369#12390#20445#23384'(&S)...'
      OnExecute = SaveListActionExecute
      OnUpdate = SaveListActionUpdate
    end
    object LoadListAction: TAction
      Caption = #12522#12473#12488#12434#12501#12449#12452#12523#12363#12425#35501#12415#36796#12416'(&L)...'
      OnExecute = LoadListActionExecute
    end
    object UndoAction: TAction
      Caption = #12522#12473#12488#12434#20803#12395#25147#12377'(&U)'
      OnExecute = UndoActionExecute
      OnUpdate = UndoActionUpdate
    end
    object EditPaste1: TEditPaste
      Category = #32232#38598
      Caption = #36028#12426#20184#12369'(&P)'
      Hint = #36028#12426#20184#12369'|'#12463#12522#12483#12503#12508#12540#12489#12398#20869#23481#12434#25407#20837#12375#12414#12377
      ImageIndex = 2
      ShortCut = 16470
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 112
    Top = 16
    object AddEmptyItem: TMenuItem
      Caption = #31354#38917#30446#12434#36861#21152'(&A)...'
      OnClick = AddEmptyItemClick
    end
    object GetSubDirItem: TMenuItem
      Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12434#21462#24471'(&D)...'
      OnClick = GetSubDirItemClick
    end
    object Sep_p_1: TMenuItem
      Caption = '-'
    end
    object SaveListItem: TMenuItem
      Action = SaveListAction
    end
    object LoadListItem: TMenuItem
      Action = LoadListAction
    end
    object Sep_p_2: TMenuItem
      Caption = '-'
    end
    object PasteItem: TMenuItem
      Action = EditPaste1
    end
    object RepCharItem: TMenuItem
      Caption = #31105#27490#25991#23383#12398#32622#25563'(&R)'
      OnClick = RepCharItemClick
    end
    object ClearListItem: TMenuItem
      Caption = #12522#12473#12488#12434#12463#12522#12450'(&C)'
      OnClick = ClearListItemClick
    end
    object UndoItem: TMenuItem
      Action = UndoAction
    end
  end
end
