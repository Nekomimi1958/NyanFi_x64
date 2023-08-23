object BitmapForm: TBitmapForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsSizeToolWin
  Caption = #12499#12483#12488#12510#12483#12503#12499#12517#12540
  ClientHeight = 266
  ClientWidth = 156
  Color = clBtnFace
  Constraints.MaxWidth = 164
  Constraints.MinHeight = 200
  Constraints.MinWidth = 164
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  FormStyle = fsStayOnTop
  KeyPreview = True
  Position = poDesigned
  ShowHint = True
  OnCreate = FormCreate
  OnHide = FormHide
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 265
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 155
    Top = 0
    Width = 1
    Height = 265
    Align = alRight
    Pen.Color = clSilver
    ExplicitLeft = 153
  end
  object BorderShape3: TShape
    Left = 0
    Top = 265
    Width = 156
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
    ExplicitWidth = 154
  end
  object ClientPanel: TPanel
    Left = 1
    Top = 0
    Width = 154
    Height = 265
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitWidth = 146
    ExplicitHeight = 253
    object FileMapPanel: TPanel
      Left = 0
      Top = 0
      Width = 20
      Height = 243
      Align = alLeft
      BevelOuter = bvNone
      BorderWidth = 2
      TabOrder = 0
      ExplicitHeight = 231
      object FileMapBox: TPaintBox
        Left = 2
        Top = 30
        Width = 16
        Height = 183
        Align = alClient
        PopupMenu = PopupMenu2
        OnMouseDown = FileMapBoxMouseDown
        OnMouseMove = FileMapBoxMouseMove
        OnPaint = FileMapBoxPaint
      end
      object TopButton: TSpeedButton
        Left = 2
        Top = 2
        Width = 16
        Height = 14
        Action = SetTopAdrAction
        Align = alTop
        Flat = True
        Layout = blGlyphTop
        Margin = 1
      end
      object EndButton: TSpeedButton
        Left = 2
        Top = 227
        Width = 16
        Height = 14
        Action = SetEndAdrAction
        Align = alBottom
        Flat = True
        Layout = blGlyphTop
        Margin = 1
      end
      object PreButton: TSpeedButton
        Left = 2
        Top = 16
        Width = 16
        Height = 14
        Action = SetPreAdrAction
        Align = alTop
        Flat = True
        Layout = blGlyphTop
        Margin = 1
      end
      object NxtButton: TSpeedButton
        Left = 2
        Top = 213
        Width = 16
        Height = 14
        Action = SetNextAdrAction
        Align = alBottom
        Flat = True
        Layout = blGlyphBottom
        Margin = 1
      end
    end
    object BorderPanel: TPanel
      Left = 20
      Top = 0
      Width = 134
      Height = 243
      Align = alClient
      BevelOuter = bvNone
      BorderWidth = 4
      Color = clAppWorkSpace
      ParentBackground = False
      TabOrder = 1
      ExplicitWidth = 126
      ExplicitHeight = 231
      object MapScrBox: TScrollBox
        Left = 4
        Top = 4
        Width = 126
        Height = 235
        HorzScrollBar.Visible = False
        VertScrollBar.Tracking = True
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clAppWorkSpace
        ParentColor = False
        PopupMenu = PopupMenu1
        TabOrder = 0
        ExplicitWidth = 118
        ExplicitHeight = 223
        object BitmapBox: TPaintBox
          Left = 0
          Top = 0
          Width = 128
          Height = 105
          OnMouseDown = BitmapBoxMouseDown
          OnMouseMove = BitmapBoxMouseMove
          OnPaint = BitmapBoxPaint
        end
      end
    end
    object SttPanel: TPanel
      Left = 0
      Top = 243
      Width = 154
      Height = 22
      Align = alBottom
      Alignment = taLeftJustify
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 2
      StyleElements = [seBorder]
      ExplicitTop = 231
      ExplicitWidth = 146
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 40
    Top = 120
    object W128Item: TMenuItem
      Action = W128Action
    end
    object W256Item: TMenuItem
      Action = W256Action
    end
  end
  object ActionList1: TActionList
    Left = 104
    Top = 120
    object W128Action: TAction
      Tag = 128
      Caption = #24133': 128'
      OnExecute = MapWidthActionExecute
      OnUpdate = MapWidthActionUpdate
    end
    object W256Action: TAction
      Tag = 256
      Caption = #24133': 256'
      OnExecute = MapWidthActionExecute
      OnUpdate = MapWidthActionUpdate
    end
    object SetTopAdrAction: TAction
      Caption = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#20808#38957#12395'(&T) '
      Hint = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#20808#38957#12395
      OnExecute = SetTopAdrActionExecute
      OnUpdate = SetTopAdrActionUpdate
    end
    object SetNextAdrAction: TAction
      Caption = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#24460#32154#37096#12395'(&N)'
      Hint = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#24460#32154#37096#12395
      OnExecute = SetNextAdrActionExecute
      OnUpdate = SetNextAdrActionUpdate
    end
    object SetPreAdrAction: TAction
      Caption = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#20808#34892#37096#12395'(&P)'
      Hint = #20808#38957#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#20808#34892#37096#12395
      OnExecute = SetPreAdrActionExecute
      OnUpdate = SetPreAdrActionUpdate
    end
    object SetEndAdrAction: TAction
      Caption = #26368#32066#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#26368#24460#12395'(&E)'
      Hint = #26368#32066#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#26368#24460#12395
      OnExecute = SetEndAdrActionExecute
      OnUpdate = SetEndAdrActionUpdate
    end
  end
  object PopupMenu2: TPopupMenu
    Left = 40
    Top = 180
    object SetTopAdr0Item: TMenuItem
      Action = SetTopAdrAction
    end
    object SetPreAdrItem: TMenuItem
      Action = SetPreAdrAction
    end
    object SetNextAdrItem: TMenuItem
      Action = SetNextAdrAction
    end
    object SetEndAdrItem: TMenuItem
      Action = SetEndAdrAction
      Caption = #32066#31471#12450#12489#12524#12473#12434#12501#12449#12452#12523#12398#26368#24460#12395'(&E)'
    end
  end
end
