object PreSameNemeDlg: TPreSameNemeDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #21516#21517#26178#20966#29702#12398#20107#21069#25351#23450
  ClientHeight = 228
  ClientWidth = 394
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
  object PreModeRadioGroup: TRadioGroup
    Left = 8
    Top = 8
    Width = 378
    Height = 161
    Caption = #20966#29702#26041#27861
    Items.Strings = (
      #23455#34892#26178#12395#36984#25246'(&R)'
      #24375#21046#19978#26360#12365'(&O)'
      #26368#26032#12394#12425#19978#26360#12365'(&N)'
      #12473#12461#12483#12503'(&S)'
      #33258#21205#30340#12395#21517#21069#12434#22793#26356'(&U)')
    TabOrder = 0
  end
  object CanButton: TButton
    Left = 205
    Top = 194
    Width = 80
    Height = 26
    Cancel = True
    Caption = #20013#26029
    ModalResult = 2
    TabOrder = 1
  end
  object OkButton: TButton
    Left = 109
    Top = 194
    Width = 80
    Height = 26
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 2
  end
end
