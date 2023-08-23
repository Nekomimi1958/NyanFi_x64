object ColorDlg: TColorDlg
  Left = 0
  Top = 0
  HelpContext = 6
  BorderStyle = bsDialog
  Caption = #12486#12461#12473#12488#12499#12517#12450#12540#12398#37197#33394
  ClientHeight = 473
  ClientWidth = 354
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
  OnDestroy = FormDestroy
  OnHelp = FormHelp
  OnShow = FormShow
  TextHeight = 15
  object ColorListBox: TListBox
    Left = 0
    Top = 0
    Width = 239
    Height = 473
    Style = lbOwnerDrawFixed
    Align = alLeft
    ItemHeight = 13
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
    OnDrawItem = ColorListBoxDrawItem
  end
  object RefColBtn: TButton
    Left = 281
    Top = 11
    Width = 64
    Height = 24
    Caption = '...'
    TabOrder = 2
    OnClick = RefColBtnClick
  end
  object InportBtn: TButton
    Left = 253
    Top = 240
    Width = 92
    Height = 26
    Caption = #12452#12531#12509#12540#12488'...'
    TabOrder = 3
    OnClick = InportBtnClick
  end
  object ExportBtn: TButton
    Left = 253
    Top = 274
    Width = 92
    Height = 26
    Caption = #12456#12463#12473#12509#12540#12488'...'
    TabOrder = 4
    OnClick = ExportBtnClick
  end
  object OkBtn: TButton
    Left = 252
    Top = 405
    Width = 92
    Height = 26
    Action = OkAction
    Default = True
    ModalResult = 1
    TabOrder = 7
  end
  object CancelBtn: TButton
    Left = 252
    Top = 439
    Width = 92
    Height = 26
    Cancel = True
    Caption = #12461#12515#12531#12475#12523
    TabOrder = 8
    OnClick = CancelBtnClick
  end
  object ApplyBtn: TButton
    Left = 252
    Top = 371
    Width = 92
    Height = 26
    Caption = #36969#29992
    TabOrder = 6
    OnClick = ApplyBtnClick
  end
  object OptApplyBtn: TButton
    Left = 252
    Top = 308
    Width = 92
    Height = 26
    Hint = #20840#20307#12398#12458#12503#12471#12519#12531#35373#23450#12395#21453#26144#12375#12414#12377#12290
    Caption = #20840#20307#12395#21453#26144
    TabOrder = 5
    OnClick = OptApplyBtnClick
  end
  object SpuitPanel: TPanel
    Left = 255
    Top = 11
    Width = 22
    Height = 22
    BevelOuter = bvLowered
    TabOrder = 1
    StyleElements = [seBorder]
    object SpuitImage: TImage
      Left = 1
      Top = 1
      Width = 20
      Height = 20
      Align = alClient
      Center = True
      Picture.Data = {
        0954506E67496D61676589504E470D0A1A0A0000000D49484452000000140000
        001408060000008D891D0D0000000774494D4507E708170A351F1885EFE80000
        00097048597300000EC300000EC301C76FA8640000000467414D410000B18F0B
        FC6105000000674944415478DAD5D4310EC0200C03C0FAE7FC9C06D1214211D8
        5009F0C4109F92053C3F0777812971259BCBEE0DDF93418F458BED05236CFAE4
        162B50D4A3C082596A0158035BCC4332C86034C86214A8604350C5BAA045C628
        50C186E03720FD423D7039E7832F4C325415354458800000000049454E44AE42
        6082}
      Stretch = True
      Transparent = True
      OnMouseDown = SpuitImageMouseDown
      OnMouseUp = SpuitImageMouseUp
    end
  end
  object Button1: TButton
    Left = 253
    Top = 43
    Width = 92
    Height = 26
    Action = DisableColAction
    TabOrder = 9
  end
  object ActionList1: TActionList
    Left = 32
    Top = 80
    object DisableColAction: TAction
      Caption = #28961#21177
      OnExecute = DisableColActionExecute
      OnUpdate = DisableColActionUpdate
    end
    object OkAction: TAction
      Caption = 'OK'
      OnExecute = OkActionExecute
      OnUpdate = OkActionUpdate
    end
  end
end
