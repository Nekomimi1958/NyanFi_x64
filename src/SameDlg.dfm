object SameNameDlg: TSameNameDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 400
  ClientWidth = 394
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    394
    400)
  PixelsPerInch = 96
  TextHeight = 13
  object OkButton: TButton
    Left = 109
    Top = 368
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
  object CanButton: TButton
    Left = 205
    Top = 368
    Width = 80
    Height = 26
    Cancel = True
    Caption = #20013#26029
    ModalResult = 2
    TabOrder = 3
  end
  object ModeGroupBox: TGroupBox
    Left = 8
    Top = 8
    Width = 378
    Height = 204
    Anchors = [akLeft, akTop, akRight]
    Caption = #20966#29702#26041#27861
    TabOrder = 0
    DesignSize = (
      378
      204)
    object Mode0Btn: TRadioButton
      Left = 11
      Top = 21
      Width = 169
      Height = 17
      Caption = #24375#21046#19978#26360#12365'(&O)'
      TabOrder = 0
      OnClick = ModeBtnClick
    end
    object Mode1Btn: TRadioButton
      Left = 11
      Top = 45
      Width = 169
      Height = 17
      Caption = #26368#26032#12394#12425#19978#26360#12365'(&N)'
      TabOrder = 1
      OnClick = ModeBtnClick
    end
    object Mode2Btn: TRadioButton
      Left = 11
      Top = 70
      Width = 169
      Height = 17
      Caption = #12473#12461#12483#12503'(&S)'
      TabOrder = 2
      OnClick = ModeBtnClick
    end
    object Mode4Btn: TRadioButton
      Left = 11
      Top = 119
      Width = 169
      Height = 17
      Caption = #21517#21069#12434#22793#26356'(&R)'
      TabOrder = 4
      OnClick = ModeBtnClick
    end
    object RenameEdit: TEdit
      Left = 30
      Top = 142
      Width = 337
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 5
      OnKeyDown = RenameEditKeyDown
      OnKeyPress = RenameEditKeyPress
    end
    object Mode3Btn: TRadioButton
      Left = 11
      Top = 94
      Width = 169
      Height = 17
      Caption = #33258#21205#30340#12395#21517#21069#12434#22793#26356'(&U)'
      TabOrder = 3
      OnClick = ModeBtnClick
    end
    object AllCheckBox: TCheckBox
      Left = 11
      Top = 176
      Width = 169
      Height = 17
      Anchors = [akLeft, akBottom]
      Caption = #12377#12409#12390#12395#36969#29992'(&A)'
      TabOrder = 6
      OnClick = AllCheckBoxClick
    end
  end
  object InfoPanel: TPanel
    Left = 8
    Top = 218
    Width = 378
    Height = 143
    Anchors = [akLeft, akTop, akRight]
    BevelOuter = bvNone
    BorderWidth = 2
    Enabled = False
    TabOrder = 1
    object InfoListBox: TListBox
      Left = 2
      Top = 2
      Width = 374
      Height = 139
      Style = lbOwnerDrawFixed
      Align = alClient
      BorderStyle = bsNone
      TabOrder = 0
      OnDrawItem = InfoListBoxDrawItem
    end
  end
end
