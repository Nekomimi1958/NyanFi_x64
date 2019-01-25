object SortModeDlg: TSortModeDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = #12477#12540#12488
  ClientHeight = 342
  ClientWidth = 234
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ModePanel: TPanel
    Left = 0
    Top = 0
    Width = 234
    Height = 208
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 0
    DesignSize = (
      234
      208)
    object SortModeRadioGroup: TRadioGroup
      Left = 8
      Top = 8
      Width = 218
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
      Width = 95
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
  object DirOptPanel: TPanel
    Left = 0
    Top = 208
    Width = 234
    Height = 91
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 180
    object DirSortModeGroupBox: TGroupBox
      Left = 8
      Top = 4
      Width = 218
      Height = 50
      Caption = #12487#12451#12524#12463#12488#12522#12398#12477#12540#12488#26041#27861
      TabOrder = 0
      object DirSortModeComboBox: TComboBox
        Left = 10
        Top = 18
        Width = 197
        Height = 21
        Style = csDropDownList
        TabOrder = 0
      end
    end
    object SortBothCheckBox: TCheckBox
      Left = 30
      Top = 66
      Width = 139
      Height = 17
      Caption = #24038#21491#12392#12418#22793#26356'(&B)'
      TabOrder = 1
      OnClick = SortBothCheckBoxClick
    end
  end
  object BtnPanel: TPanel
    Left = 0
    Top = 300
    Width = 234
    Height = 42
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    ExplicitTop = 275
    object OkButton: TButton
      Left = 30
      Top = 9
      Width = 80
      Height = 26
      Caption = 'OK'
      Default = True
      ModalResult = 1
      TabOrder = 0
    end
    object CanButton: TButton
      Left = 123
      Top = 9
      Width = 80
      Height = 26
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      ModalResult = 2
      TabOrder = 1
    end
  end
end
