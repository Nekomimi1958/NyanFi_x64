object SetGitTagDlg: TSetGitTagDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  ClientHeight = 332
  ClientWidth = 474
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object TagPanel: TPanel
    Left = 0
    Top = 26
    Width = 474
    Height = 74
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      474
      74)
    object NameEdit: TLabeledEdit
      Left = 8
      Top = 24
      Width = 451
      Height = 21
      Anchors = [akLeft, akTop, akRight]
      EditLabel.Width = 39
      EditLabel.Height = 13
      EditLabel.Caption = #21517#21069'(&N)'
      TabOrder = 0
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
    DesignSize = (
      474
      232)
    object CancelBtn: TButton
      Left = 252
      Top = 198
      Width = 75
      Height = 26
      Anchors = [akRight, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 0
    end
    object OkBtn: TButton
      Left = 148
      Top = 198
      Width = 75
      Height = 26
      Action = OkAction
      Anchors = [akRight, akBottom]
      Default = True
      ModalResult = 1
      TabOrder = 1
    end
    object AnnotateMemo: TMemo
      Left = 8
      Top = 4
      Width = 458
      Height = 183
      Anchors = [akLeft, akTop, akRight, akBottom]
      TabOrder = 2
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
    object Label1: TLabel
      Left = 8
      Top = 8
      Width = 45
      Height = 13
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
