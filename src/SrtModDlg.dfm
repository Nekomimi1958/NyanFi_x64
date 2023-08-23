object SortModeDlg: TSortModeDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #12477#12540#12488
  ClientHeight = 351
  ClientWidth = 506
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
  TextHeight = 15
  object MainPanel: TPanel
    Left = 0
    Top = 0
    Width = 254
    Height = 351
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitHeight = 350
    object BtnPanel: TPanel
      Left = 0
      Top = 309
      Width = 254
      Height = 42
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      ExplicitTop = 308
      object OkButton: TButton
        Left = 40
        Top = 9
        Width = 80
        Height = 26
        Action = OkAction
        Default = True
        ModalResult = 1
        TabOrder = 0
      end
      object CanButton: TButton
        Left = 133
        Top = 9
        Width = 80
        Height = 26
        Cancel = True
        Caption = #12461#12515#12531#12475#12523
        ModalResult = 2
        TabOrder = 1
      end
    end
    object DirOptPanel: TPanel
      Left = 0
      Top = 208
      Width = 254
      Height = 97
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object DirSortModeGroupBox: TGroupBox
        Left = 8
        Top = 4
        Width = 238
        Height = 50
        Caption = #12487#12451#12524#12463#12488#12522#12398#12477#12540#12488#26041#27861
        TabOrder = 0
        object DirSortModeComboBox: TComboBox
          Left = 10
          Top = 18
          Width = 218
          Height = 23
          Style = csDropDownList
          TabOrder = 0
        end
      end
      object SortBothCheckBox: TCheckBox
        Left = 18
        Top = 66
        Width = 139
        Height = 17
        Caption = #24038#21491#12392#12418#22793#26356'(&B)'
        TabOrder = 1
        OnClick = SortBothCheckBoxClick
      end
      object OptBtn: TButton
        Left = 161
        Top = 63
        Width = 75
        Height = 24
        Action = ExOptAction
        TabOrder = 2
      end
    end
    object ModePanel: TPanel
      Left = 0
      Top = 0
      Width = 254
      Height = 208
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      DesignSize = (
        254
        208)
      object SortModeRadioGroup: TRadioGroup
        Left = 8
        Top = 8
        Width = 238
        Height = 192
        Anchors = [akLeft, akTop, akRight]
        Caption = #12477#12540#12488#26041#27861
        Items.Strings = (
          #21517#21069'(&F)'
          #25313#24373#23376'(&E)'
          #26356#26032#26085#26178'(&D)'
          #12469#12452#12474'(&S)'
          #23646#24615'(&A)'
          #12394#12375'(&U)')
        TabOrder = 0
        OnClick = SortModeRadioGroupClick
        OnEnter = SortModeRadioGroupEnter
        OnExit = SortModeRadioGroupExit
      end
      object GroupBox1: TGroupBox
        Left = 118
        Top = 25
        Width = 108
        Height = 52
        TabOrder = 1
        object DscNameCheckBox: TCheckBox
          Left = 7
          Top = 31
          Width = 77
          Height = 17
          Caption = #38477#38918'(&R)'
          TabOrder = 1
          OnClick = OptCheckBoxClick
        end
        object NaturalCheckBox: TCheckBox
          Left = 7
          Top = 3
          Width = 85
          Height = 17
          Caption = #33258#28982#38918'(&N)'
          TabOrder = 0
          OnClick = OptCheckBoxClick
        end
      end
      object OldCheckBox: TCheckBox
        Left = 125
        Top = 86
        Width = 88
        Height = 17
        Caption = #21476#12356#38918'(&O)'
        TabOrder = 2
        OnClick = OptCheckBoxClick
      end
      object SmallCheckBox: TCheckBox
        Left = 125
        Top = 115
        Width = 88
        Height = 17
        Caption = #23567#12373#12356#38918'(&M)'
        TabOrder = 3
        OnClick = OptCheckBoxClick
      end
      object DscAttrCheckBox: TCheckBox
        Left = 125
        Top = 144
        Width = 88
        Height = 17
        Caption = #38477#38918'(&V)'
        TabOrder = 4
        OnClick = OptCheckBoxClick
      end
    end
  end
  object OptPanel: TPanel
    Left = 254
    Top = 0
    Width = 252
    Height = 351
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitWidth = 248
    ExplicitHeight = 350
    object AccDtoTCheckBox: TCheckBox
      Left = 8
      Top = 236
      Width = 234
      Height = 17
      Caption = #26356#26032#26085#26178#12398#12450#12463#12475#12521#12524#12540#12479#12434' T '#12395#22793#26356
      TabOrder = 2
      OnClick = AccDtoTCheckBoxClick
    end
    object SameCloseCheckBox: TCheckBox
      Left = 8
      Top = 261
      Width = 234
      Height = 17
      Caption = #29694#12514#12540#12489#12398#12461#12540#12434#25276#12375#12383#12392#12365#12418#38281#12376#12427
      TabOrder = 3
    end
    object PrimeGroupBox: TGroupBox
      Left = 8
      Top = 8
      Width = 234
      Height = 161
      Caption = #31532'1'#12477#12540#12488#26041#27861
      TabOrder = 0
      object PrimeComboBox: TComboBox
        Left = 10
        Top = 18
        Width = 213
        Height = 23
        Style = csDropDownList
        TabOrder = 0
        OnClick = PrimeComboBoxClick
        Items.Strings = (
          #21517#21069
          #25313#24373#23376
          #26356#26032#26085#26178
          #12469#12452#12474
          #23646#24615
          '')
      end
      object SubModeRadioGroup: TRadioGroup
        Left = 10
        Top = 51
        Width = 213
        Height = 102
        Caption = #31532'2'#12477#12540#12488#26041#27861
        Columns = 2
        Items.Strings = (
          #21517#21069
          #25313#24373#23376
          #26356#26032#26085#26178
          #12469#12452#12474
          #23646#24615
          #12394#12375)
        TabOrder = 1
        OnClick = SubModeRadioGroupClick
      end
    end
    object ExOptCheckBox: TCheckBox
      Left = 8
      Top = 311
      Width = 234
      Height = 17
      Caption = #25313#24373#35373#23450#12434#24120#12395#34920#31034
      TabOrder = 5
    end
    object SortLogicalCheckBox: TCheckBox
      Left = 8
      Top = 286
      Width = 234
      Height = 17
      Caption = #33258#28982#38918#28961#21177#26178#12395#35542#29702#12477#12540#12488
      TabOrder = 4
    end
    object ExtListEdit: TLabeledEdit
      Left = 8
      Top = 199
      Width = 234
      Height = 23
      EditLabel.Width = 149
      EditLabel.Height = 15
      EditLabel.Caption = #25313#24373#23376#38918#12391#20778#20808#12377#12427#25313#24373#23376
      TabOrder = 1
      Text = ''
    end
  end
  object ActionList1: TActionList
    Left = 166
    Top = 176
    object ExOptAction: TAction
      Caption = #25313#24373#35373#23450
      OnExecute = ExOptActionExecute
      OnUpdate = ExOptActionUpdate
    end
    object OkAction: TAction
      Caption = 'OK'
      OnUpdate = OkActionUpdate
    end
  end
end
