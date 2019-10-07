object ExpCsvDlg: TExpCsvDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderWidth = 4
  Caption = 'CSV/TSV'#12456#12463#12473#12509#12540#12488
  ClientHeight = 361
  ClientWidth = 464
  Color = clBtnFace
  Constraints.MinHeight = 360
  Constraints.MinWidth = 400
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object SrcListBox: TListBox
    Left = 0
    Top = 0
    Width = 193
    Height = 253
    Style = lbOwnerDrawFixed
    Align = alLeft
    ItemHeight = 13
    TabOrder = 0
    OnDblClick = SrcListBoxDblClick
    OnDrawItem = ItemListBoxDrawItem
  end
  object DstListBox: TListBox
    Left = 283
    Top = 0
    Width = 181
    Height = 253
    Style = lbOwnerDrawFixed
    Align = alClient
    DragMode = dmAutomatic
    ItemHeight = 13
    TabOrder = 2
    OnDblClick = DstListBoxDblClick
    OnDrawItem = ItemListBoxDrawItem
    OnKeyDown = DstListBoxKeyDown
  end
  object OpePanel: TPanel
    Left = 0
    Top = 253
    Width = 464
    Height = 108
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    DesignSize = (
      464
      108)
    object Label1: TLabel
      Left = 57
      Top = 51
      Width = 48
      Height = 13
      Alignment = taRightJustify
      Caption = #20986#21147#24418#24335
    end
    object CanButton: TButton
      Left = 379
      Top = 78
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 6
    end
    object OkButton: TButton
      Left = 283
      Top = 78
      Width = 80
      Height = 26
      Action = ExportAction
      Anchors = [akTop, akRight]
      Default = True
      TabOrder = 5
    end
    object OutNameEdit: TLabeledEdit
      Left = 108
      Top = 14
      Width = 313
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 86
      EditLabel.Height = 13
      EditLabel.Caption = #20986#21147#12501#12449#12452#12523#21517'(&O)'
      LabelPosition = lpLeft
      TabOrder = 0
    end
    object QuotCheckBox: TCheckBox
      Left = 249
      Top = 50
      Width = 195
      Height = 17
      Caption = #38917#30446#12434#24341#29992#31526#12391#22258#12416
      TabOrder = 4
    end
    object CsvRadioBtn: TRadioButton
      Left = 110
      Top = 50
      Width = 59
      Height = 17
      Caption = 'CSV /'
      TabOrder = 2
    end
    object TsvRadioBtn: TRadioButton
      Left = 173
      Top = 50
      Width = 63
      Height = 17
      Caption = 'TSV'
      TabOrder = 3
    end
    object RefOutNameBtn: TButton
      Left = 423
      Top = 14
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 1
      OnClick = RefOutNameBtnClick
    end
  end
  object MidPanel: TPanel
    Left = 193
    Top = 0
    Width = 90
    Height = 253
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      90
      253)
    object DelItemBtn: TButton
      Left = 35
      Top = 179
      Width = 50
      Height = 24
      Action = UserModule.DelListItemAction
      Anchors = [akRight, akBottom]
      TabOrder = 4
    end
    object DownItemBtn: TButton
      Left = 35
      Top = 149
      Width = 50
      Height = 24
      Action = UserModule.DownListItemAction
      Anchors = [akRight, akBottom]
      TabOrder = 3
    end
    object UpItemBtn: TButton
      Left = 35
      Top = 119
      Width = 50
      Height = 24
      Action = UserModule.UpListItemAction
      Anchors = [akRight, akBottom]
      TabOrder = 2
    end
    object AddBtn: TButton
      Left = 5
      Top = 12
      Width = 80
      Height = 24
      Action = AddItemAction
      Anchors = [akTop, akRight]
      TabOrder = 0
    end
    object AddAllBtn: TButton
      Left = 5
      Top = 42
      Width = 80
      Height = 24
      Action = AddAllItemAction
      Anchors = [akTop, akRight]
      TabOrder = 1
    end
    object ClrItemBtn: TButton
      Left = 35
      Top = 219
      Width = 50
      Height = 24
      Action = UserModule.ClrListItemAction
      Anchors = [akRight, akBottom]
      TabOrder = 5
    end
  end
  object ActionList1: TActionList
    Left = 24
    Top = 16
    object ExportAction: TAction
      Caption = #38283#22987
      OnExecute = ExportActionExecute
      OnUpdate = ExportActionUpdate
    end
    object AddItemAction: TAction
      Caption = #38917#30446#36861#21152'>'
      OnExecute = AddItemActionExecute
      OnUpdate = AddItemActionUpdate
    end
    object AddAllItemAction: TAction
      Caption = #20840#36861#21152'>>'
      OnExecute = AddAllItemActionExecute
    end
  end
end
