object EditItemDlg: TEditItemDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #35373#23450#38917#30446#12398#32232#38598
  ClientHeight = 395
  ClientWidth = 354
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  TextHeight = 15
  object ItemListBox: TListBox
    Left = 0
    Top = 0
    Width = 354
    Height = 275
    Style = lbOwnerDrawFixed
    Align = alClient
    DragMode = dmAutomatic
    ItemHeight = 13
    TabOrder = 0
    OnClick = ItemListBoxClick
    OnDrawItem = ItemListBoxDrawItem
    OnKeyDown = ItemListBoxKeyDown
    ExplicitWidth = 350
    ExplicitHeight = 274
  end
  object Panel1: TPanel
    Left = 0
    Top = 275
    Width = 354
    Height = 120
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 274
    ExplicitWidth = 350
    DesignSize = (
      354
      120)
    object AddItemBtn: TButton
      Left = 9
      Top = 51
      Width = 60
      Height = 25
      Action = AddItemAction
      TabOrder = 1
    end
    object ChgItemBtn: TButton
      Left = 73
      Top = 51
      Width = 60
      Height = 25
      Action = ChgItemAction
      TabOrder = 2
    end
    object DelItemBtn: TButton
      Left = 137
      Top = 51
      Width = 60
      Height = 25
      Action = UserModule.DelListItemAction
      TabOrder = 3
    end
    object UpItemBtn: TButton
      Left = 281
      Top = 51
      Width = 30
      Height = 25
      Action = UserModule.UpListItemAction
      TabOrder = 5
    end
    object DownItemBtn: TButton
      Left = 315
      Top = 51
      Width = 30
      Height = 25
      Action = UserModule.DownListItemAction
      TabOrder = 6
    end
    object ItemEdit: TLabeledEdit
      Left = 10
      Top = 22
      Width = 335
      Height = 23
      EditLabel.Width = 44
      EditLabel.Height = 15
      EditLabel.Caption = 'ItemEdit'
      TabOrder = 0
      Text = ''
    end
    object OkButton: TButton
      Left = 89
      Top = 87
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 7
    end
    object CanButton: TButton
      Left = 185
      Top = 87
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 8
    end
    object SortItemBtn: TButton
      Left = 209
      Top = 51
      Width = 60
      Height = 25
      Action = SortItemAction
      TabOrder = 4
    end
  end
  object ActionList1: TActionList
    Left = 40
    Top = 16
    object AddItemAction: TAction
      Caption = #36861#21152
      OnExecute = AddItemActionExecute
      OnUpdate = AddItemActionUpdate
    end
    object ChgItemAction: TAction
      Caption = #22793#26356
      OnExecute = ChgItemActionExecute
      OnUpdate = ChgItemActionUpdate
    end
    object SortItemAction: TAction
      Caption = #12477#12540#12488
      OnExecute = SortItemActionExecute
      OnUpdate = SortItemActionUpdate
    end
  end
end
