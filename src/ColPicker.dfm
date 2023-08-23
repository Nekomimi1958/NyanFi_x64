object ColorPicker: TColorPicker
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = #12459#12521#12540#12500#12483#12459#12540
  ClientHeight = 201
  ClientWidth = 324
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    324
    201)
  TextHeight = 15
  object ColLabel: TLabel
    Left = 8
    Top = 4
    Width = 3
    Height = 15
    Anchors = [akLeft, akTop, akRight]
  end
  object ColEdit: TEdit
    Left = 122
    Top = 84
    Width = 134
    Height = 23
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 6
  end
  object ColPanel: TPanel
    Left = 262
    Top = 4
    Width = 50
    Height = 50
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    TabOrder = 0
    StyleElements = []
    object ColImage: TImage
      Left = 1
      Top = 1
      Width = 48
      Height = 48
      Align = alClient
    end
  end
  object FmtComboBox: TComboBox
    Left = 8
    Top = 84
    Width = 109
    Height = 23
    Style = csDropDownList
    DropDownCount = 12
    TabOrder = 5
  end
  object Col2Panel: TPanel
    Left = 262
    Top = 58
    Width = 50
    Height = 50
    Anchors = [akTop, akRight]
    BevelOuter = bvLowered
    TabOrder = 3
    StyleElements = []
  end
  object Ave9pxCheckBox: TCheckBox
    Left = 8
    Top = 59
    Width = 78
    Height = 17
    Caption = '9px'#24179#22343
    TabOrder = 1
  end
  object SafeColCheckBox: TCheckBox
    Left = 150
    Top = 59
    Width = 103
    Height = 17
    Caption = #12475#12540#12501#12459#12521#12540
    TabOrder = 2
  end
  object CmpColCheckBox: TCheckBox
    Left = 89
    Top = 59
    Width = 58
    Height = 17
    Caption = #35036#33394
    TabOrder = 4
  end
  object RepeatGroupBox: TGroupBox
    Left = 8
    Top = 122
    Width = 300
    Height = 71
    Anchors = [akLeft, akTop, akRight]
    Caption = #36899#32154#21462#24471
    TabOrder = 7
    DesignSize = (
      300
      71)
    object RepSttLabel: TLabel
      Left = 16
      Top = 47
      Width = 275
      Height = 20
      Alignment = taRightJustify
      AutoSize = False
    end
    object RepCntEdit: TLabeledEdit
      Left = 48
      Top = 20
      Width = 61
      Height = 23
      Alignment = taRightJustify
      EditLabel.Width = 26
      EditLabel.Height = 23
      EditLabel.Caption = #22238#25968
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 0
      Text = ''
    end
    object RepComboBox: TComboBox
      Left = 113
      Top = 20
      Width = 126
      Height = 23
      Style = csDropDownList
      Anchors = [akTop, akRight]
      TabOrder = 1
      Items.Strings = (
        ', '#21306#20999#12426
        'TAG '#21306#20999#12426
        #31354#30333#21306#20999#12426
        #34892#36861#21152)
    end
    object ExeRepBtn: TButton
      Left = 243
      Top = 19
      Width = 50
      Height = 24
      Action = StartRepAction
      Anchors = [akTop, akRight]
      TabOrder = 2
    end
  end
  object ActionList1: TActionList
    Left = 32
    Top = 8
    object StartRepAction: TAction
      Caption = #38283#22987
      OnExecute = StartRepActionExecute
      OnUpdate = StartRepActionUpdate
    end
  end
end
