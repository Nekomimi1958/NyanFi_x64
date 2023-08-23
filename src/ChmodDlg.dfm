object FtpChmodDlg: TFtpChmodDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12497#12540#12511#12483#12471#12519#12531#12398#35373#23450
  ClientHeight = 203
  ClientWidth = 281
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  Position = poDesigned
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object Label1: TLabel
    Left = 61
    Top = 131
    Width = 45
    Height = 15
    Alignment = taRightJustify
    Caption = '8'#36914#25968#20516
  end
  object OkButton: TButton
    Left = 50
    Top = 169
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 4
  end
  object CanButton: TButton
    Left = 151
    Top = 169
    Width = 80
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 5
  end
  object GroupBox1: TGroupBox
    Left = 12
    Top = 12
    Width = 80
    Height = 97
    Caption = #12458#12540#12490#12540
    TabOrder = 0
    object R1CheckBox: TCheckBox
      Left = 12
      Top = 20
      Width = 64
      Height = 17
      Caption = #35501#36796
      TabOrder = 0
      OnClick = PrmCheckBoxClick
    end
    object W1CheckBox: TCheckBox
      Left = 12
      Top = 45
      Width = 64
      Height = 17
      Caption = #26360#36796
      TabOrder = 1
      OnClick = PrmCheckBoxClick
    end
    object X1CheckBox: TCheckBox
      Left = 12
      Top = 70
      Width = 64
      Height = 17
      Caption = #23455#34892
      TabOrder = 2
      OnClick = PrmCheckBoxClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 100
    Top = 12
    Width = 80
    Height = 97
    Caption = #12464#12523#12540#12503
    TabOrder = 1
    object R2CheckBox: TCheckBox
      Left = 12
      Top = 20
      Width = 64
      Height = 17
      Caption = #35501#36796
      TabOrder = 0
      OnClick = PrmCheckBoxClick
    end
    object W2CheckBox: TCheckBox
      Left = 12
      Top = 45
      Width = 64
      Height = 17
      Caption = #26360#36796
      TabOrder = 1
      OnClick = PrmCheckBoxClick
    end
    object X2CheckBox: TCheckBox
      Left = 12
      Top = 70
      Width = 64
      Height = 17
      Caption = #23455#34892
      TabOrder = 2
      OnClick = PrmCheckBoxClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 188
    Top = 12
    Width = 80
    Height = 97
    Caption = #12381#12398#20182
    TabOrder = 2
    object R3CheckBox: TCheckBox
      Left = 12
      Top = 20
      Width = 64
      Height = 17
      Caption = #35501#36796
      TabOrder = 0
      OnClick = PrmCheckBoxClick
    end
    object W3CheckBox: TCheckBox
      Left = 12
      Top = 45
      Width = 64
      Height = 17
      Caption = #26360#36796
      TabOrder = 1
      OnClick = PrmCheckBoxClick
    end
    object X3CheckBox: TCheckBox
      Left = 12
      Top = 70
      Width = 64
      Height = 17
      Caption = #23455#34892
      TabOrder = 2
      OnClick = PrmCheckBoxClick
    end
  end
  object OctalEdit: TMaskEdit
    Left = 110
    Top = 128
    Width = 60
    Height = 23
    Alignment = taRightJustify
    EditMask = '!999;1;0'
    MaxLength = 3
    TabOrder = 3
    Text = '   '
    OnChange = OctalEditChange
  end
end
