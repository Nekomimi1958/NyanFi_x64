object NewFileDlg: TNewFileDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12501#12449#12452#12523#12398#26032#35215#20316#25104
  ClientHeight = 171
  ClientWidth = 474
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
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    474
    171)
  TextHeight = 15
  object Label1: TLabel
    Left = 34
    Top = 11
    Width = 60
    Height = 15
    Alignment = taRightJustify
    Caption = #12486#12531#12503#12524#12540#12488
  end
  object TplComboBox: TComboBox
    Left = 98
    Top = 8
    Width = 319
    Height = 22
    Style = csOwnerDrawFixed
    Anchors = [akLeft, akTop, akRight]
    PopupMenu = UserModule.EditPopupMenuC
    TabOrder = 0
    OnClick = TplComboBoxClick
    OnDrawItem = TplComboBoxDrawItem
    ExplicitWidth = 315
  end
  object RefTplBtn: TButton
    Left = 419
    Top = 8
    Width = 36
    Height = 22
    Anchors = [akTop, akRight]
    Caption = '...'
    TabOrder = 1
    OnClick = RefTplBtnClick
    ExplicitLeft = 415
  end
  object NewNameEdit: TLabeledEdit
    Left = 98
    Top = 37
    Width = 319
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 26
    EditLabel.Height = 23
    EditLabel.Caption = #21517#21069
    LabelPosition = lpLeft
    TabOrder = 2
    Text = ''
    ExplicitWidth = 315
  end
  object OkBtn: TButton
    Left = 279
    Top = 137
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
    ExplicitLeft = 275
  end
  object CancelBtn: TButton
    Left = 375
    Top = 137
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 5
    ExplicitLeft = 371
  end
  object NewExeCmdEdit: TLabeledEdit
    Left = 98
    Top = 94
    Width = 319
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 134
    EditLabel.Height = 15
    EditLabel.Caption = #20316#25104#24460#12395#23455#34892#12377#12427#12467#12510#12531#12489
    TabOrder = 3
    Text = ''
    ExplicitWidth = 315
  end
  object OK: TActionList
    Left = 32
    Top = 80
    object OkAction: TAction
      Caption = 'OK'
      OnUpdate = OkActionUpdate
    end
  end
end
