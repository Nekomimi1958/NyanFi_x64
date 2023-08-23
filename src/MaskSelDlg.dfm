object MaskSelectDlg: TMaskSelectDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 72
  ClientWidth = 312
  Color = clBtnFace
  Constraints.MaxHeight = 110
  Constraints.MinHeight = 110
  Constraints.MinWidth = 320
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
    312
    72)
  TextHeight = 15
  object MaskSelComboBox: TComboBox
    Left = 8
    Top = 8
    Width = 294
    Height = 23
    Anchors = [akLeft, akTop, akRight]
    PopupMenu = UserModule.EditPopupMenuC
    TabOrder = 0
    ExplicitWidth = 290
  end
  object OkButton: TButton
    Left = 126
    Top = 43
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
    ExplicitLeft = 122
  end
  object CanButton: TButton
    Left = 222
    Top = 43
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 2
    ExplicitLeft = 218
  end
end
