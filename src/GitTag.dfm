object SetGitTagDlg: TSetGitTagDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 332
  ClientWidth = 474
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
  object TagPanel: TPanel
    Left = 0
    Top = 26
    Width = 474
    Height = 74
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 470
    DesignSize = (
      474
      74)
    object NameEdit: TLabeledEdit
      Left = 8
      Top = 24
      Width = 447
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 43
      EditLabel.Height = 15
      EditLabel.Caption = #21517#21069'(&N)'
      TabOrder = 0
      Text = ''
      ExplicitWidth = 443
    end
    object AnnotateCheckBox: TCheckBox
      Left = 8
      Top = 55
      Width = 97
      Height = 17
      Caption = #27880#37320#20184#12365'(&A)'
      TabOrder = 1
    end
  end
  object MsgPanel: TPanel
    Left = 0
    Top = 100
    Width = 474
    Height = 232
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitWidth = 470
    ExplicitHeight = 231
    DesignSize = (
      474
      232)
    object CancelBtn: TButton
      Left = 248
      Top = 198
      Width = 75
      Height = 26
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 0
      ExplicitLeft = 244
      ExplicitTop = 197
    end
    object OkBtn: TButton
      Left = 144
      Top = 198
      Width = 75
      Height = 26
      Action = OkAction
      Anchors = [akRight, akBottom]
      Default = True
      ModalResult = 1
      TabOrder = 1
      ExplicitLeft = 140
      ExplicitTop = 197
    end
    object AnnotateMemo: TMemo
      Left = 8
      Top = 4
      Width = 454
      Height = 183
      Anchors = [akLeft, akTop, akRight, akBottom]
      ScrollBars = ssVertical
      TabOrder = 2
      ExplicitWidth = 450
      ExplicitHeight = 182
    end
  end
  object CommitPanel: TPanel
    Left = 0
    Top = 0
    Width = 474
    Height = 26
    Align = alTop
    Alignment = taLeftJustify
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitWidth = 470
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 50
      Height = 15
      Caption = #12513#12483#12475#12540#12472
    end
  end
  object ActionList1: TActionList
    Left = 400
    Top = 152
    object OkAction: TAction
      Caption = 'Ok'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
  end
end
