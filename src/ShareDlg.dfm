object NetShareDlg: TNetShareDlg
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  ClientHeight = 322
  ClientWidth = 232
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ListPanel: TPanel
    Left = 0
    Top = 29
    Width = 232
    Height = 293
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object ShareListBox: TListBox
      Left = 0
      Top = 0
      Width = 232
      Height = 293
      Style = lbOwnerDrawFixed
      Align = alLeft
      Anchors = [akLeft, akTop, akRight, akBottom]
      BorderStyle = bsNone
      PopupMenu = PopupMenu1
      TabOrder = 0
      OnDblClick = ShareListBoxDblClick
      OnDrawItem = ShareListBoxDrawItem
      OnKeyDown = ShareListBoxKeyDown
      OnKeyPress = ShareListBoxKeyPress
    end
  end
  object HiddenCanBtn: TButton
    Left = 0
    Top = 322
    Width = 232
    Height = 0
    Align = alBottom
    Cancel = True
    ModalResult = 2
    TabOrder = 2
    TabStop = False
  end
  object PathTabControl: TTabControl
    Left = 0
    Top = 0
    Width = 232
    Height = 29
    Align = alTop
    DoubleBuffered = False
    OwnerDraw = True
    ParentDoubleBuffered = False
    Style = tsFlatButtons
    TabOrder = 0
    TabStop = False
    OnChange = PathTabControlChange
    OnDrawTab = PathTabControlDrawTab
  end
  object PopupMenu1: TPopupMenu
    OnPopup = PopupMenu1Popup
    Left = 24
    Top = 48
    object CopyUncItem: TMenuItem
      Caption = 'UNC'#12497#12473#12434#12467#12500#12540'(&C)'
      OnClick = CopyUncItemClick
    end
    object CopyUncAllItem: TMenuItem
      Caption = #12377#12409#12390#12398'UNC'#12497#12473#12434#12467#12500#12540'(&A)'
      OnClick = CopyUncAllItemClick
    end
  end
end
