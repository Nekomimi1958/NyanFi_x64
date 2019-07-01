object FindDuplDlg: TFindDuplDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #37325#35079#12501#12449#12452#12523#12398#26908#32034
  ClientHeight = 215
  ClientWidth = 314
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    314
    215)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 266
    Top = 34
    Width = 14
    Height = 13
    Caption = 'MB'
  end
  object Label2: TLabel
    Left = 12
    Top = 12
    Width = 150
    Height = 13
    Caption = #12495#12483#12471#12517#12539#12450#12523#12468#12522#12474#12512
  end
  object OkButton: TButton
    Left = 65
    Top = 181
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
    Top = 181
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
    Top = 96
    Width = 287
    Height = 17
    Caption = #12469#12502#12487#12451#12524#12463#12488#12522#12418#26908#32034'(&R)'
    TabOrder = 3
  end
  object ResLinkCheckBox: TCheckBox
    Left = 16
    Top = 146
    Width = 287
    Height = 17
    Caption = #32080#26524#12522#12473#12488#12363#12425#21453#23550#20596#12408#21453#26144'(&L)'
    TabOrder = 5
  end
  object OppPathCheckBox: TCheckBox
    Left = 16
    Top = 71
    Width = 287
    Height = 17
    Caption = #21453#23550#20596#12418#26908#32034'(&O)'
    TabOrder = 2
  end
  object MaxSizeEdit: TLabeledEdit
    Left = 194
    Top = 31
    Width = 70
    Height = 21
    EditLabel.Width = 53
    EditLabel.Height = 13
    EditLabel.Caption = #26368#22823#12469#12452#12474
    TabOrder = 1
  end
  object ExcSymCheckBox: TCheckBox
    Left = 16
    Top = 121
    Width = 287
    Height = 17
    Caption = #12472#12515#12531#12463#12471#12519#12531'/'#12471#12531#12508#12522#12483#12463#12522#12531#12463#12434#38500#22806'(&S)'
    TabOrder = 4
  end
  object AlgComboBox: TComboBox
    Left = 12
    Top = 31
    Width = 150
    Height = 21
    Style = csDropDownList
    TabOrder = 0
  end
end
