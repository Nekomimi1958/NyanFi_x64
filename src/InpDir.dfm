object InpDirDlg: TInpDirDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 84
  ClientWidth = 342
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
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object TopPanel: TPanel
    Left = 0
    Top = 0
    Width = 342
    Height = 36
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 338
    object RefBtnPanel: TPanel
      Left = 298
      Top = 0
      Width = 44
      Height = 36
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitLeft = 294
      object RefDirBtn: TButton
        Left = 0
        Top = 6
        Width = 36
        Height = 22
        Caption = '...'
        TabOrder = 0
        OnClick = RefDirBtnClick
      end
    end
    object InpPanel: TPanel
      Left = 0
      Top = 0
      Width = 298
      Height = 36
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitWidth = 294
      DesignSize = (
        298
        36)
      object SubComboBox: TComboBox
        Tag = 1
        Left = 4
        Top = 6
        Width = 290
        Height = 23
        AutoComplete = False
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        DropDownCount = 16
        TabOrder = 1
        TabStop = False
        OnClick = SubComboBoxClick
        OnCloseUp = SubComboBoxCloseUp
        ExplicitWidth = 286
      end
      object InpDirComboBox: TComboBox
        Left = 4
        Top = 6
        Width = 290
        Height = 23
        Anchors = [akLeft, akTop, akRight]
        DropDownCount = 20
        TabOrder = 0
        OnChange = InpDirComboBoxChange
        OnKeyDown = InpDirComboBoxKeyDown
        OnKeyPress = InpDirComboBoxKeyPress
        OnSelect = InpDirComboBoxSelect
        ExplicitWidth = 286
      end
    end
  end
  object BottomPanel: TPanel
    Left = 0
    Top = 39
    Width = 342
    Height = 45
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 38
    ExplicitWidth = 338
    DesignSize = (
      342
      45)
    object CanButton: TButton
      Left = 250
      Top = 10
      Width = 80
      Height = 26
      Anchors = [akTop, akRight]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 2
      ExplicitLeft = 246
    end
    object OkButton: TButton
      Left = 156
      Top = 10
      Width = 80
      Height = 26
      Action = OkAction
      Anchors = [akTop, akRight]
      Default = True
      ModalResult = 1
      TabOrder = 1
      ExplicitLeft = 152
    end
    object CmpByKeyPanel: TPanel
      Left = 12
      Top = 13
      Width = 85
      Height = 17
      AutoSize = True
      BevelOuter = bvNone
      TabOrder = 0
      object CmpByKeyCheckBox: TCheckBox
        Left = 0
        Top = 0
        Width = 85
        Height = 17
        Hint = #8594#12461#12540#12391#12501#12523#12497#12473#35036#23436#12522#12473#12488#12434#12489#12525#12483#12503#12480#12454#12531
        Caption = #8594#12391#35036#23436
        TabOrder = 0
      end
    end
  end
  object ActionList1: TActionList
    Left = 96
    Top = 32
    object OkAction: TAction
      Caption = 'OK'
      OnUpdate = OkActionUpdate
    end
  end
end
