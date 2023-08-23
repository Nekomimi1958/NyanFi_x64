object InpCmdsDlg: TInpCmdsDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #12467#12510#12531#12489#12398#20837#21147
  ClientHeight = 77
  ClientWidth = 432
  Color = clBtnFace
  Constraints.MaxHeight = 140
  Constraints.MinHeight = 70
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
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object CancelBtn: TButton
    Left = 432
    Top = 36
    Width = 0
    Height = 5
    Align = alRight
    Cancel = True
    ModalResult = 2
    TabOrder = 2
    ExplicitLeft = 428
    ExplicitHeight = 4
  end
  object ModePanel: TPanel
    Left = 0
    Top = 41
    Width = 432
    Height = 36
    Align = alBottom
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 6
    TabOrder = 1
    ExplicitTop = 40
    ExplicitWidth = 428
    object ModeTabControl: TTabControl
      Left = 6
      Top = 6
      Width = 420
      Height = 24
      Align = alBottom
      OwnerDraw = True
      Style = tsButtons
      TabOrder = 0
      Tabs.Strings = (
        #12288#12501#12449#12452#12521#12540#12288
        #12486#12461#12473#12488#12499#12517#12450#12540
        #12452#12513#12540#12472#12499#12517#12450#12540
        #12525#12464#12454#12451#12531#12489#12454)
      TabIndex = 0
      OnChange = ModeTabControlChange
      OnDrawTab = ModeTabControlDrawTab
      ExplicitWidth = 416
    end
  end
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 432
    Height = 36
    Align = alTop
    BevelOuter = bvNone
    BorderWidth = 6
    TabOrder = 0
    ExplicitWidth = 428
    DesignSize = (
      432
      36)
    object SubComboBox: TComboBox
      Tag = 1
      Left = 8
      Top = 7
      Width = 326
      Height = 21
      AutoComplete = False
      Style = csOwnerDrawFixed
      Anchors = [akLeft, akTop, akRight]
      DropDownCount = 16
      ItemHeight = 15
      TabOrder = 1
      TabStop = False
      OnClick = SubComboBoxClick
      OnCloseUp = SubComboBoxCloseUp
      OnDrawItem = SubComboBoxDrawItem
      ExplicitWidth = 322
    end
    object MigemoCheckBox: TCheckBox
      Left = 348
      Top = 9
      Width = 76
      Height = 17
      Anchors = [akTop, akRight]
      Caption = '&Migemo'
      TabOrder = 2
      OnClick = MigemoCheckBoxClick
      ExplicitLeft = 344
    end
    object CmdsComboBox: TComboBox
      Left = 8
      Top = 7
      Width = 326
      Height = 23
      AutoComplete = False
      Anchors = [akLeft, akTop, akRight]
      DropDownCount = 16
      PopupMenu = UserModule.EditPopupMenuC
      TabOrder = 0
      OnChange = CmdsComboBoxChange
      OnKeyDown = CmdsComboBoxKeyDown
      OnKeyPress = CmdsComboBoxKeyPress
      OnSelect = CmdsComboBoxSelect
      ExplicitWidth = 322
    end
  end
end
