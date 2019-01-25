object GifViewer: TGifViewer
  Left = 0
  Top = 0
  BorderStyle = bsSizeToolWin
  ClientHeight = 248
  ClientWidth = 310
  Color = clBtnFace
  DoubleBuffered = True
  ParentFont = True
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PopupMenu = PopupMenu1
  Position = poDesigned
  OnClick = FormClick
  OnCreate = FormCreate
  OnHide = FormHide
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ImgPanel: TPanel
    Left = 0
    Top = 0
    Width = 310
    Height = 248
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    StyleElements = []
    object Image1: TImage
      Left = 0
      Top = 0
      Width = 310
      Height = 248
      Align = alClient
      Center = True
      PopupMenu = PopupMenu1
      Proportional = True
      OnClick = Image1Click
    end
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 40
    Top = 24
    object BgColItem: TMenuItem
      Caption = #32972#26223#33394'(&C)'
      OnClick = BgColItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object RotateLItem: TMenuItem
      Action = RotateLeftAction
    end
    object RotateRItem: TMenuItem
      Action = RotateRightAction
    end
    object FlipHItem: TMenuItem
      Tag = 4
      Action = FlipHorzAction
    end
    object FlipVItem: TMenuItem
      Tag = 5
      Action = FlipVertAction
    end
    object Sep_2: TMenuItem
      Caption = '-'
    end
    object LockItem: TMenuItem
      Caption = #34920#31034#12434#12525#12483#12463'(&L)'
      OnClick = LockItemClick
    end
  end
  object ActionList1: TActionList
    Left = 112
    Top = 24
    object RotateLeftAction: TAction
      Tag = 3
      Caption = #24038#12395'90'#24230#22238#36578'(&L)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object RotateRightAction: TAction
      Tag = 1
      Caption = #21491#12395'90'#24230#22238#36578'(&R)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FlipHorzAction: TAction
      Tag = 4
      Caption = #24038#21491#21453#36578'(&H)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FlipVertAction: TAction
      Tag = 5
      Caption = #19978#19979#21453#36578'(&V)'
      OnExecute = RotateActionExecute
      OnUpdate = RotateActionUpdate
    end
  end
end
