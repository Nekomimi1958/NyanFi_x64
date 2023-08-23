object ExeCmdDlg: TExeCmdDlg
  Left = 0
  Top = 0
  HelpContext = 62
  BorderIcons = [biSystemMenu]
  Caption = #12467#12510#12531#12489#12521#12452#12531#12398#23455#34892
  ClientHeight = 187
  ClientWidth = 490
  Color = clBtnFace
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
    490
    187)
  TextHeight = 15
  object CanButton: TButton
    Left = 398
    Top = 152
    Width = 80
    Height = 26
    Anchors = [akTop, akRight]
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    ModalResult = 2
    TabOrder = 5
    ExplicitLeft = 394
  end
  object OkButton: TButton
    Left = 312
    Top = 152
    Width = 80
    Height = 26
    Action = OkAction
    Anchors = [akTop, akRight]
    Default = True
    ModalResult = 1
    TabOrder = 4
    ExplicitLeft = 308
  end
  object ExeComboBox: TComboBox
    Left = 8
    Top = 8
    Width = 470
    Height = 23
    Hint = #12467#12510#12531#12489#12521#12452#12531
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 16
    TabOrder = 0
    ExplicitWidth = 466
  end
  object OptGroupBox: TGroupBox
    Left = 8
    Top = 38
    Width = 470
    Height = 76
    Anchors = [akLeft, akTop, akRight]
    Caption = #12467#12531#12477#12540#12523#12450#12503#12522#12398#20986#21147
    TabOrder = 1
    ExplicitWidth = 466
    DesignSize = (
      470
      76)
    object LogStdOutCheckBox: TCheckBox
      Left = 10
      Top = 18
      Width = 116
      Height = 17
      Caption = #12525#12464#12395#20986#21147'(&L)'
      TabOrder = 0
    end
    object CopyStdOutCheckBox: TCheckBox
      Left = 135
      Top = 18
      Width = 171
      Height = 17
      Caption = #12463#12522#12483#12503#12508#12540#12489#12395#12467#12500#12540'(&C)'
      TabOrder = 1
    end
    object SaveStdOutCheckBox: TCheckBox
      Left = 10
      Top = 47
      Width = 121
      Height = 17
      Caption = #12501#12449#12452#12523#12395#20445#23384'(&S)'
      TabOrder = 3
    end
    object RefOutBtn: TButton
      Left = 428
      Top = 45
      Width = 36
      Height = 22
      Anchors = [akTop, akRight]
      Caption = '...'
      TabOrder = 5
      ExplicitLeft = 424
    end
    object SaveNameEdit: TEdit
      Left = 135
      Top = 45
      Width = 291
      Height = 23
      Anchors = [akLeft, akTop, akRight]
      PopupMenu = UserModule.EditPopupMenuE
      TabOrder = 4
      ExplicitWidth = 287
    end
    object ListStdOutCheckBox: TCheckBox
      Left = 310
      Top = 18
      Width = 119
      Height = 17
      Caption = #19968#35239#12391#34920#31034'(&D)'
      TabOrder = 2
    end
  end
  object RunAsCheckBox: TCheckBox
    Left = 18
    Top = 127
    Width = 223
    Height = 17
    Caption = #31649#29702#32773#12392#12375#12390#23455#34892'(&A)'
    TabOrder = 2
  end
  object UacDlgCheckBox: TCheckBox
    Left = 18
    Top = 152
    Width = 223
    Height = 17
    Caption = #27177#38480#26119#26684#12480#12452#12450#12525#12464#12434#34920#31034'(&E)'
    TabOrder = 3
  end
  object ActionList1: TActionList
    Left = 265
    Top = 127
    object OkAction: TAction
      Caption = 'Ok'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
    object DelItemAction: TAction
      Caption = #12371#12398#38917#30446#12434#21066#38500'(&D)'
      OnExecute = DelItemActionExecute
      OnUpdate = DelItemActionUpdate
    end
    object RefCurFnamAction: TAction
      Caption = 'RefCurFnamAction'
    end
    object RefCurPnamAction: TAction
      Caption = 'RefCurPnamAction'
    end
  end
end
