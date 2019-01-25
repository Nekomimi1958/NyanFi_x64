object MaskSelectDlg: TMaskSelectDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 77
  ClientWidth = 312
  Color = clBtnFace
  Constraints.MaxHeight = 110
  Constraints.MinHeight = 110
  Constraints.MinWidth = 320
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
    312
    77)
  PixelsPerInch = 96
  TextHeight = 13
  object MaskSelComboBox: TComboBox
    Left = 8
    Top = 8
    Width = 298
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    PopupMenu = UserModule.EditPopupMenuC
    TabOrder = 0
  end
  object OkButton: TButton
    Left = 130
    Top = 43
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
  object CanButton: TButton
    Left = 226
    Top = 43
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 2
  end
end
