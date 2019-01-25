object FindDuplDlg: TFindDuplDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #37325#35079#12501#12449#12452#12523#12398#26908#32034
  ClientHeight = 251
  ClientWidth = 314
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    314
    251)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 279
    Top = 34
    Width = 14
    Height = 13
    Caption = 'MB'
  end
  object AlgRadioGroup: TRadioGroup
    Left = 8
    Top = 8
    Width = 193
    Height = 80
    Anchors = [akLeft, akTop, akRight]
    Caption = #12495#12483#12471#12517#12539#12450#12523#12468#12522#12474#12512'(&A)'
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'MD5'
      'SHA-1'
      'SHA-256'
      'SHA-384'
      'SHA-512'
      'CRC32')
    TabOrder = 0
  end
  object OkButton: TButton
    Left = 65
    Top = 217
    Width = 80
    Height = 26
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 6
  end
  object CanButton: TButton
    Left = 170
    Top = 217
    Width = 80
    Height = 26
    Anchors = [akLeft, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 7
  end
  object SubDirCheckBox: TCheckBox
    Left = 16
    Top = 130
    Width = 287
    Height = 17
    Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12418#26908#32034'(&R)'
    TabOrder = 3
  end
  object ResLinkCheckBox: TCheckBox
    Left = 16
    Top = 180
    Width = 287
    Height = 17
    Caption = #32080#26524#12522#12473#12488#12363#12425#21453#23550#20596#12408#21453#26144'(&L)'
    TabOrder = 5
  end
  object OppPathCheckBox: TCheckBox
    Left = 16
    Top = 105
    Width = 287
    Height = 17
    Caption = #21453#23550#20596#12418#26908#32034'(&O)'
    TabOrder = 2
  end
  object MaxSizeEdit: TLabeledEdit
    Left = 222
    Top = 31
    Width = 55
    Height = 21
    Alignment = taRightJustify
    EditLabel.Width = 53
    EditLabel.Height = 13
    EditLabel.Caption = #26368#22823#12469#12452#12474
    TabOrder = 1
  end
  object ExcSymCheckBox: TCheckBox
    Left = 16
    Top = 155
    Width = 287
    Height = 17
    Caption = #12472#12515#12531#12463#12471#12519#12531'/'#12471#12531#12508#12522#12483#12463#12522#12531#12463#12434#38500#22806'(&S)'
    TabOrder = 4
  end
end
