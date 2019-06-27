object ColorPicker: TColorPicker
  Left = 0
  Top = 0
  BorderStyle = bsToolWindow
  Caption = #12459#12521#12540#12500#12483#12459#12540
  ClientHeight = 201
  ClientWidth = 314
  Color = clBtnFace
  ParentFont = True
  FormStyle = fsStayOnTop
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  DesignSize = (
    314
    201)
  PixelsPerInch = 96
  TextHeight = 13
  object ColLabel: TLabel
    Left = 8
    Top = 4
    Width = 3
    Height = 13
    Anchors = [akLeft, akTop, akRight]
  end
  object Label1: TLabel
    Left = 25
    Top = 61
    Width = 42
    Height = 13
    Caption = '9px'#24179#22343
  end
  object Label2: TLabel
    Left = 106
    Top = 61
    Width = 24
    Height = 13
    Caption = #35036#33394
  end
  object Label3: TLabel
    Left = 167
    Top = 61
    Width = 55
    Height = 13
    Caption = #12475#12540#12501#12459#12521#12540
  end
  object ColEdit: TEdit
    Left = 119
    Top = 84
    Width = 136
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 6
  end
  object ColPanel: TPanel
    Left = 260
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
    Width = 110
    Height = 21
    Style = csDropDownList
    DropDownCount = 12
    TabOrder = 5
  end
  object Col2Panel: TPanel
    Left = 260
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
    TabOrder = 1
  end
  object SafeColCheckBox: TCheckBox
    Left = 150
    Top = 59
    Width = 103
    Height = 17
    TabOrder = 2
  end
  object CmpColCheckBox: TCheckBox
    Left = 89
    Top = 59
    Width = 58
    Height = 17
    TabOrder = 4
  end
  object RepeatGroupBox: TGroupBox
    Left = 8
    Top = 122
    Width = 298
    Height = 71
    TabOrder = 7
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
      Width = 45
      Height = 21
      Alignment = taRightJustify
      EditLabel.Width = 24
      EditLabel.Height = 13
      EditLabel.Caption = #22238#25968
      LabelPosition = lpLeft
      NumbersOnly = True
      TabOrder = 0
    end
    object RepComboBox: TComboBox
      Left = 99
      Top = 20
      Width = 136
      Height = 21
      Style = csDropDownList
      TabOrder = 1
      Items.Strings = (
        ', '#21306#20999#12426
        'TAG '#21306#20999#12426
        #31354#30333#21306#20999#12426
        #34892#36861#21152)
    end
    object ExeRepBtn: TButton
      Left = 241
      Top = 19
      Width = 50
      Height = 24
      Action = StartRepAction
      TabOrder = 2
    end
  end
  object Panel1: TPanel
    Left = 16
    Top = 115
    Width = 52
    Height = 17
    AutoSize = True
    BevelOuter = bvNone
    BorderWidth = 2
    TabOrder = 8
    object Label4: TLabel
      Left = 2
      Top = 2
      Width = 48
      Height = 13
      Caption = #36899#32154#21462#24471
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
