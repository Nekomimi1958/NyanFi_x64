object GraphForm: TGraphForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = #12464#12521#12501
  ClientHeight = 289
  ClientWidth = 426
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  KeyPreview = True
  StyleElements = [seFont, seBorder]
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 426
    Height = 289
    Align = alClient
    PopupMenu = PopupMenu1
    OnMouseDown = PaintBox1MouseDown
    OnPaint = PaintBox1Paint
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 289
    Width = 426
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 0
    ExplicitTop = 288
    ExplicitWidth = 422
  end
  object PopupMenu1: TPopupMenu
    Left = 64
    Top = 48
    object CopyImgItem: TMenuItem
      Caption = #30011#20687#12392#12375#12390#12467#12500#12540'(&C)'
      OnClick = CopyImgItemClick
    end
    object Sep_1: TMenuItem
      Caption = '-'
    end
    object ShowMaxItem: TMenuItem
      Action = ShowMaxAction
    end
    object ShowAveItem: TMenuItem
      Action = ShowAveAction
    end
    object ShowMinItem: TMenuItem
      Action = ShowMinAction
    end
  end
  object ActionList1: TActionList
    Left = 144
    Top = 48
    object ShowAveAction: TAction
      Caption = #24179#22343#20516'(&A)'
      OnExecute = ShowInfActionExecute
    end
    object ShowMaxAction: TAction
      Caption = #26368#22823#20516'(&H)'
      OnExecute = ShowInfActionExecute
    end
    object ShowMinAction: TAction
      Caption = #26368#23567#20516'(&L)'
      OnExecute = ShowInfActionExecute
    end
  end
end
