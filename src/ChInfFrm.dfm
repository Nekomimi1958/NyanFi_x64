object CharInfoForm: TCharInfoForm
  Left = 0
  Top = 0
  HelpContext = 6
  BorderIcons = [biSystemMenu]
  BorderStyle = bsToolWindow
  Caption = #25991#23383#24773#22577
  ClientHeight = 318
  ClientWidth = 274
  Color = clBtnFace
  DoubleBuffered = True
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poDesigned
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BorderShape1: TShape
    Left = 0
    Top = 0
    Width = 1
    Height = 317
    Align = alLeft
    Pen.Color = clSilver
  end
  object BorderShape2: TShape
    Left = 273
    Top = 0
    Width = 1
    Height = 317
    Align = alRight
    Pen.Color = clSilver
  end
  object BorderShape3: TShape
    Left = 0
    Top = 317
    Width = 274
    Height = 1
    Align = alBottom
    Pen.Color = clSilver
  end
  object ClientPanel: TPanel
    Left = 1
    Top = 0
    Width = 272
    Height = 317
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 0
      Top = 160
      Width = 272
      Height = 4
      Cursor = crVSplit
      Align = alTop
      AutoSnap = False
      MinSize = 44
      OnMoved = Splitter1Moved
    end
    object InfoPanel: TPanel
      Left = 0
      Top = 164
      Width = 272
      Height = 153
      Align = alClient
      AutoSize = True
      BevelOuter = bvNone
      Enabled = False
      PopupMenu = PopupMenu1
      TabOrder = 0
      object InfoListBox: TListBox
        Left = 0
        Top = 0
        Width = 272
        Height = 153
        TabStop = False
        Style = lbOwnerDrawFixed
        AutoComplete = False
        Align = alClient
        BevelInner = bvNone
        BorderStyle = bsNone
        DoubleBuffered = True
        ItemHeight = 13
        ParentDoubleBuffered = False
        PopupMenu = PopupMenu1
        TabOrder = 0
        StyleElements = [seBorder]
        OnDrawItem = InfoListBoxDrawItem
        ExplicitHeight = 170
      end
    end
    object SamplePanel: TPanel
      Left = 0
      Top = 0
      Width = 272
      Height = 160
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 1
      object CharPanel: TPanel
        Left = 0
        Top = 0
        Width = 272
        Height = 138
        Align = alClient
        BevelOuter = bvNone
        Color = clWindow
        Font.Charset = ANSI_CHARSET
        Font.Color = clWindowText
        Font.Height = -96
        Font.Name = #65325#65331' '#26126#26397
        Font.Style = []
        ParentBackground = False
        ParentFont = False
        PopupMenu = PopupMenu1
        TabOrder = 0
        StyleElements = []
      end
      object FontNamePanel: TPanel
        Left = 0
        Top = 138
        Width = 272
        Height = 22
        Align = alBottom
        BevelOuter = bvNone
        PopupMenu = PopupMenu1
        TabOrder = 1
        StyleElements = []
      end
    end
  end
  object PopupMenu1: TPopupMenu
    Left = 112
    Top = 40
    object CopyItem: TMenuItem
      Caption = #24773#22577#12434#12467#12500#12540'(&C)'
      OnClick = CopyItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object ChgFontItem: TMenuItem
      Caption = #12501#12457#12531#12488#12398#36984#25246'(&F)...'
      OnClick = ChgFontItemClick
    end
    object SetBgColItem: TMenuItem
      Caption = #25313#22823#34920#31034#12398#32972#26223#33394'(&B)...'
      OnClick = SetColItemClick
    end
    object SetFgColItem: TMenuItem
      Tag = 1
      Caption = #25313#22823#34920#31034#12398#25991#23383#33394'(&T)...'
      OnClick = SetColItemClick
    end
  end
end
