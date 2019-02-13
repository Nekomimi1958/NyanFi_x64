object ToolBtnDlg: TToolBtnDlg
  Left = 0
  Top = 0
  HelpContext = 68
  BorderStyle = bsDialog
  Caption = #12484#12540#12523#12496#12540#12398#35373#23450
  ClientHeight = 611
  ClientWidth = 694
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poMainFormCenter
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnHelp = FormHelp
  OnResize = FormResize
  OnShow = FormShow
  DesignSize = (
    694
    611)
  PixelsPerInch = 96
  TextHeight = 13
  object IcoImage: TImage
    Left = 8
    Top = 518
    Width = 22
    Height = 22
    Anchors = [akLeft, akBottom]
    Center = True
  end
  object CmdLabel: TLabel
    Left = 185
    Top = 500
    Width = 6
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = '  '
  end
  object CaptionEdit: TLabeledEdit
    Left = 34
    Top = 518
    Width = 148
    Height = 21
    Hint = '- '#12399#12475#12497#12524#12540#12479
    Anchors = [akLeft, akBottom]
    EditLabel.Width = 110
    EditLabel.Height = 13
    EditLabel.Caption = #12461#12515#12503#12471#12519#12531';'#12484#12540#12523#12498#12531#12488
    TabOrder = 1
  end
  object RefCmdsBtn: TButton
    Tag = 100
    Left = 649
    Top = 518
    Width = 36
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = '...'
    TabOrder = 3
    OnClick = RefCmdsBtnClick
  end
  object CanButton: TButton
    Left = 567
    Top = 577
    Width = 80
    Height = 26
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 14
  end
  object OkButton: TButton
    Left = 479
    Top = 577
    Width = 80
    Height = 26
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 13
    OnClick = OkButtonClick
  end
  object EditBtn: TButton
    Left = 370
    Top = 577
    Width = 95
    Height = 24
    Action = EditFileAction
    Anchors = [akLeft, akBottom]
    TabOrder = 12
  end
  object BtnListBox: TListBox
    Left = 0
    Top = 0
    Width = 694
    Height = 494
    Style = lbOwnerDrawFixed
    Align = alTop
    Anchors = [akLeft, akTop, akRight, akBottom]
    DragMode = dmAutomatic
    ExtendedSelect = False
    TabOrder = 0
    OnClick = BtnListBoxClick
    OnDrawItem = BtnListBoxDrawItem
    OnKeyDown = BtnListBoxKeyDown
  end
  object DowItemBtn: TButton
    Left = 286
    Top = 577
    Width = 30
    Height = 24
    Action = UserModule.DownListItemAction
    Anchors = [akLeft, akBottom]
    TabOrder = 11
  end
  object UpItemBtn: TButton
    Left = 252
    Top = 577
    Width = 30
    Height = 24
    Action = UserModule.UpListItemAction
    Anchors = [akLeft, akBottom]
    TabOrder = 10
  end
  object DelItemBtn: TButton
    Left = 185
    Top = 577
    Width = 55
    Height = 24
    Action = UserModule.DelListItemAction
    Anchors = [akLeft, akBottom]
    TabOrder = 9
  end
  object ChgItemBtn: TButton
    Left = 126
    Top = 577
    Width = 55
    Height = 24
    Action = ChgBtnAction
    Anchors = [akLeft, akBottom]
    TabOrder = 8
  end
  object AddItemBtn: TButton
    Left = 8
    Top = 577
    Width = 55
    Height = 24
    Action = AddBtnAction
    Anchors = [akLeft, akBottom]
    TabOrder = 6
  end
  object RefIconBtn: TButton
    Tag = 100
    Left = 649
    Top = 546
    Width = 36
    Height = 22
    Anchors = [akRight, akBottom]
    Caption = '...'
    TabOrder = 5
    OnClick = RefIconBtnClick
  end
  object IconEdit: TLabeledEdit
    Left = 185
    Top = 546
    Width = 462
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    EditLabel.Width = 35
    EditLabel.Height = 13
    EditLabel.Caption = #12450#12452#12467#12531
    LabelPosition = lpLeft
    TabOrder = 4
    OnChange = IconEditChange
  end
  object BtnCmdsComboBox: TComboBox
    Left = 185
    Top = 518
    Width = 462
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    DropDownCount = 20
    TabOrder = 2
    OnChange = BtnCmdsComboBoxChange
  end
  object InsItemBtn: TButton
    Left = 67
    Top = 577
    Width = 55
    Height = 24
    Action = InsBtnAction
    Anchors = [akLeft, akBottom]
    TabOrder = 7
  end
  object ActionList1: TActionList
    Left = 50
    Top = 33
    object AddBtnAction: TAction
      Caption = #36861#21152
      OnExecute = AddBtnActionExecute
      OnUpdate = AddBtnActionUpdate
    end
    object InsBtnAction: TAction
      Caption = #25407#20837
      OnExecute = InsBtnActionExecute
      OnUpdate = ChgBtnActionUpdate
    end
    object ChgBtnAction: TAction
      Caption = #22793#26356
      OnExecute = ChgBtnActionExecute
      OnUpdate = ChgBtnActionUpdate
    end
    object EditFileAction: TAction
      Caption = #12501#12449#12452#12523#32232#38598
      OnExecute = EditFileActionExecute
      OnUpdate = EditFileActionUpdate
    end
  end
end
