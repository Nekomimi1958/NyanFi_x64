object SetGitTagDlg: TSetGitTagDlg
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = #12467#12511#12483#12488#12395#12479#12464#12434#20184#12369#12427
  ClientHeight = 284
  ClientWidth = 474
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnShow = FormShow
  DesignSize = (
    474
    284)
  PixelsPerInch = 96
  TextHeight = 13
  object NameEdit: TLabeledEdit
    Left = 8
    Top = 24
    Width = 458
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    EditLabel.Width = 39
    EditLabel.Height = 13
    EditLabel.Caption = #21517#21069'(&N)'
    TabOrder = 0
  end
  object AnnotateMemo: TMemo
    Left = 8
    Top = 80
    Width = 458
    Height = 161
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
  end
  object AnnotateCheckBox: TCheckBox
    Left = 8
    Top = 57
    Width = 97
    Height = 17
    Caption = #27880#37320#20184#12365'(&A)'
    TabOrder = 1
  end
  object CancelBtn: TButton
    Left = 251
    Top = 251
    Width = 75
    Height = 26
    Anchors = [akRight, akBottom]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 4
  end
  object OkBtn: TButton
    Left = 147
    Top = 251
    Width = 75
    Height = 26
    Action = OkAction
    Anchors = [akRight, akBottom]
    Default = True
    ModalResult = 1
    TabOrder = 3
  end
  object ActionList1: TActionList
    Left = 384
    Top = 96
    object OkAction: TAction
      Caption = 'Ok'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
  end
end
