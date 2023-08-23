object RegSyncDlg: TRegSyncDlg
  Left = 0
  Top = 0
  HelpContext = 74
  BorderStyle = bsDialog
  Caption = #21516#26399#12467#12500#12540#12398#35373#23450
  ClientHeight = 453
  ClientWidth = 634
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnShow = FormShow
  TextHeight = 15
  object OptPanel: TPanel
    Left = 0
    Top = 213
    Width = 634
    Height = 240
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 212
    ExplicitWidth = 630
    DesignSize = (
      634
      240)
    object DirListBox: TListBox
      Left = 40
      Top = 36
      Width = 582
      Height = 165
      Style = lbOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      ItemHeight = 13
      ParentShowHint = False
      ShowHint = True
      Sorted = True
      TabOrder = 6
      OnDrawItem = DirListBoxDrawItem
      OnKeyDown = SyncListBoxKeyDown
      ExplicitWidth = 578
    end
    object OkButton: TButton
      Left = 458
      Top = 208
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 10
      OnClick = OkButtonClick
    end
    object CanButton: TButton
      Left = 546
      Top = 208
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 11
    end
    object AddMenuBtn: TButton
      Left = 430
      Top = 6
      Width = 60
      Height = 24
      Action = AddRegAction
      Anchors = [akLeft, akBottom]
      TabOrder = 3
    end
    object DelMenuBtn: TButton
      Left = 566
      Top = 6
      Width = 60
      Height = 24
      Action = DelRegAction
      Anchors = [akLeft, akBottom]
      TabOrder = 5
    end
    object NameEdit: TLabeledEdit
      Left = 40
      Top = 7
      Width = 180
      Height = 23
      EditLabel.Width = 26
      EditLabel.Height = 23
      EditLabel.Caption = #21517#21069
      LabelPosition = lpLeft
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 0
      Text = ''
    end
    object SyncDelCheckBox: TCheckBox
      Left = 320
      Top = 9
      Width = 79
      Height = 17
      Caption = #21516#26399#21066#38500
      TabOrder = 2
    end
    object ChgMenuBtn: TButton
      Left = 498
      Top = 6
      Width = 60
      Height = 24
      Action = ChgRegAction
      Anchors = [akLeft, akBottom]
      TabOrder = 4
    end
    object DelDirBtn: TButton
      Left = 108
      Top = 209
      Width = 60
      Height = 24
      Action = DelDirAction
      Anchors = [akLeft, akBottom]
      TabOrder = 8
    end
    object AddDirBtn: TButton
      Left = 40
      Top = 209
      Width = 60
      Height = 24
      Action = AddDirAction
      Anchors = [akLeft, akBottom]
      TabOrder = 7
    end
    object SyncOwrCheckBox: TCheckBox
      Left = 248
      Top = 9
      Width = 68
      Height = 17
      Caption = #19978#26360#12365
      TabOrder = 1
    end
    object ClrDirBtn: TButton
      Left = 176
      Top = 209
      Width = 60
      Height = 24
      Action = ClrDirAction
      Anchors = [akLeft, akBottom]
      TabOrder = 9
    end
  end
  object RegListBox: TCheckListBox
    Left = 0
    Top = 0
    Width = 634
    Height = 213
    Align = alClient
    DragMode = dmAutomatic
    Style = lbOwnerDrawFixed
    TabOrder = 0
    OnClick = RegListBoxClick
    OnClickCheck = RegListBoxClickCheck
    OnDrawItem = RegListBoxDrawItem
    OnKeyDown = SyncListBoxKeyDown
    ExplicitWidth = 630
    ExplicitHeight = 212
  end
  object ActionList1: TActionList
    Left = 43
    Top = 30
    object AddRegAction: TAction
      Caption = #30331#37682
      OnExecute = AddRegActionExecute
      OnUpdate = AddRegActionUpdate
    end
    object DelRegAction: TAction
      Caption = #21066#38500
      OnExecute = DelRegActionExecute
      OnUpdate = ChgRegActionUpdate
    end
    object ChgRegAction: TAction
      Caption = #22793#26356
      OnExecute = ChgRegActionExecute
      OnUpdate = ChgRegActionUpdate
    end
    object AddDirAction: TAction
      Caption = #36861#21152'...'
      OnExecute = AddDirActionExecute
    end
    object DelDirAction: TAction
      Caption = #21066#38500
      OnExecute = DelDirActionExecute
      OnUpdate = DelDirActionUpdate
    end
    object ClrDirAction: TAction
      Caption = #12463#12522#12450
      OnExecute = ClrDirActionExecute
    end
  end
end
