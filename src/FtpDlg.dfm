object FtpConnectDlg: TFtpConnectDlg
  Left = 0
  Top = 0
  HelpContext = 80
  BorderStyle = bsDialog
  Caption = 'FTP'#25509#32154
  ClientHeight = 397
  ClientWidth = 612
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
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  TextHeight = 15
  object PageControl1: TPageControl
    Left = 0
    Top = 0
    Width = 612
    Height = 397
    ActivePage = HostSheet
    Align = alClient
    OwnerDraw = True
    TabOrder = 0
    TabWidth = 100
    OnDrawTab = PageControl1DrawTab
    object HostSheet: TTabSheet
      Caption = #12507#12473#12488
      object HostPanel: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 367
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        StyleElements = []
        DesignSize = (
          604
          367)
        object Label2: TLabel
          Left = 395
          Top = 141
          Width = 57
          Height = 15
          Alignment = taRightJustify
          Caption = 'SSL'#26263#21495#21270
        end
        object AddMenuBtn: TButton
          Left = 9
          Top = 338
          Width = 60
          Height = 24
          Action = AddHostAction
          Anchors = [akLeft, akBottom]
          TabOrder = 13
        end
        object AnonyCheckBox: TCheckBox
          Left = 505
          Top = 77
          Width = 89
          Height = 17
          Caption = 'anonymous'
          TabOrder = 4
          OnClick = AnonyCheckBoxClick
        end
        object CancelBtn: TButton
          Left = 514
          Top = 336
          Width = 80
          Height = 26
          Anchors = [akLeft, akBottom]
          Cancel = True
          Caption = #12461#12515#12531#12475#12523
          ModalResult = 2
          TabOrder = 19
        end
        object ChgMenuBtn: TButton
          Left = 73
          Top = 338
          Width = 60
          Height = 24
          Action = ChgHostAction
          Anchors = [akLeft, akBottom]
          TabOrder = 14
        end
        object ConnectBtn: TButton
          Left = 418
          Top = 336
          Width = 80
          Height = 26
          Action = ConnectAction
          Anchors = [akLeft, akBottom]
          Default = True
          ModalResult = 1
          TabOrder = 18
        end
        object DelMenuBtn: TButton
          Left = 137
          Top = 338
          Width = 60
          Height = 24
          Action = UserModule.DelListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 15
        end
        object DowButton: TButton
          Left = 235
          Top = 338
          Width = 30
          Height = 24
          Action = UserModule.DownListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 17
        end
        object HostAddrEdit: TLabeledEdit
          Left = 264
          Top = 45
          Width = 334
          Height = 23
          Hint = ': '#12391#21306#20999#12387#12390#27161#28310#20197#22806#12398#12509#12540#12488#30058#21495#12434#25351#23450#21487#33021
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 39
          EditLabel.Height = 23
          EditLabel.Caption = #12450#12489#12524#12473
          LabelPosition = lpLeft
          TabOrder = 2
          Text = ''
        end
        object HostDirEdit: TLabeledEdit
          Left = 214
          Top = 190
          Width = 384
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 110
          EditLabel.Height = 15
          EditLabel.Caption = #12507#12473#12488#38283#22987#12487#12451#12524#12463#12488#12522
          TabOrder = 8
          Text = ''
        end
        object UserIdEdit: TLabeledEdit
          Left = 264
          Top = 74
          Width = 228
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 44
          EditLabel.Height = 23
          EditLabel.Caption = #12518#12540#12470'ID'
          LabelPosition = lpLeft
          TabOrder = 3
          Text = ''
        end
        object LocalDirEdit: TLabeledEdit
          Left = 214
          Top = 237
          Width = 346
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 120
          EditLabel.Height = 15
          EditLabel.Caption = #12525#12540#12459#12523#38283#22987#12487#12451#12524#12463#12488#12522
          TabOrder = 9
          Text = ''
        end
        object HostListBox: TListBox
          Left = 8
          Top = 8
          Width = 190
          Height = 317
          Style = lbOwnerDrawFixed
          Anchors = [akLeft, akTop, akBottom]
          DragMode = dmAutomatic
          ItemHeight = 13
          ParentShowHint = False
          ShowHint = False
          TabOrder = 0
          OnClick = HostListBoxClick
          OnDblClick = HostListBoxDblClick
          OnDrawItem = HostListBoxDrawItem
          OnKeyDown = HostListBoxKeyDown
        end
        object RefDirBtn: TButton
          Tag = 100
          Left = 562
          Top = 238
          Width = 36
          Height = 22
          Anchors = [akTop, akRight]
          Caption = '...'
          TabOrder = 10
          OnClick = RefDirBtnClick
        end
        object PassWdEdit: TLabeledEdit
          Left = 264
          Top = 107
          Width = 228
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 51
          EditLabel.Height = 23
          EditLabel.Caption = #12497#12473#12527#12540#12489
          LabelPosition = lpLeft
          PasswordChar = '*'
          TabOrder = 5
          Text = ''
        end
        object SyncLRCheckBox: TCheckBox
          Left = 214
          Top = 274
          Width = 182
          Height = 17
          Caption = #24038#21491#12487#12451#12524#12463#12488#12522#12398#21516#26399#22793#26356
          TabOrder = 11
        end
        object UpButton: TButton
          Left = 203
          Top = 338
          Width = 30
          Height = 24
          Action = UserModule.UpListItemAction
          Anchors = [akLeft, akBottom]
          TabOrder = 16
        end
        object SSLComboBox: TComboBox
          Left = 456
          Top = 138
          Width = 142
          Height = 23
          Style = csDropDownList
          TabOrder = 7
        end
        object HostNameEdit: TLabeledEdit
          Left = 264
          Top = 12
          Width = 334
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 43
          EditLabel.Height = 23
          EditLabel.Caption = #12507#12473#12488#21517
          LabelPosition = lpLeft
          TabOrder = 1
          Text = ''
        end
        object PasvCheckBox: TCheckBox
          Left = 264
          Top = 140
          Width = 97
          Height = 17
          Caption = 'PASV'#12514#12540#12489
          Checked = True
          State = cbChecked
          TabOrder = 6
        end
        object LastDirCheckBox: TCheckBox
          Left = 402
          Top = 274
          Width = 198
          Height = 17
          Caption = #20999#26029#26178#12398#12487#12451#12524#12463#12488#12522#12391#27425#22238#38283#22987
          TabOrder = 12
        end
      end
    end
    object OptionSheet: TTabSheet
      Caption = #12458#12503#12471#12519#12531
      ImageIndex = 1
      object OptPanel: TPanel
        Left = 0
        Top = 0
        Width = 604
        Height = 367
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 0
        StyleElements = []
        DesignSize = (
          604
          367)
        object SndFindGroupBox: TGroupBox
          Left = 12
          Top = 203
          Width = 575
          Height = 108
          Caption = #36890#30693#38899
          TabOrder = 7
          DesignSize = (
            575
            108)
          object RefSndConnectBtn: TButton
            Left = 509
            Top = 18
            Width = 36
            Height = 22
            Hint = #21442#29031
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 1
            OnClick = RefSoundBtnClick
          end
          object TestSndConnectBtn: TButton
            Left = 546
            Top = 18
            Width = 22
            Height = 22
            Hint = #12486#12473#12488#20877#29983
            Anchors = [akTop, akRight]
            Caption = '>'
            TabOrder = 2
            OnClick = TestSoundBtnClick
          end
          object SndConnectEdit: TLabeledEdit
            Left = 72
            Top = 18
            Width = 425
            Height = 23
            EditLabel.Width = 56
            EditLabel.Height = 23
            EditLabel.Caption = #12507#12473#12488#25509#32154
            LabelPosition = lpLeft
            TabOrder = 0
            Text = ''
          end
          object SndDisconEdit: TLabeledEdit
            Left = 72
            Top = 47
            Width = 425
            Height = 23
            EditLabel.Width = 56
            EditLabel.Height = 23
            EditLabel.Caption = #12507#12473#12488#20999#26029
            LabelPosition = lpLeft
            TabOrder = 3
            Text = ''
          end
          object TestSndDisconBtn: TButton
            Tag = 1
            Left = 546
            Top = 47
            Width = 22
            Height = 22
            Hint = #12486#12473#12488#20877#29983
            Anchors = [akTop, akRight]
            Caption = '>'
            TabOrder = 5
            OnClick = TestSoundBtnClick
          end
          object RefSndDisconBtn: TButton
            Tag = 1
            Left = 509
            Top = 47
            Width = 36
            Height = 22
            Hint = #21442#29031
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 4
            OnClick = RefSoundBtnClick
          end
          object SndTransferEdit: TLabeledEdit
            Left = 72
            Top = 76
            Width = 425
            Height = 23
            EditLabel.Width = 52
            EditLabel.Height = 23
            EditLabel.Caption = #36578#36865#23436#20102
            LabelPosition = lpLeft
            TabOrder = 6
            Text = ''
          end
          object TestSndTransferBtn: TButton
            Tag = 2
            Left = 546
            Top = 76
            Width = 22
            Height = 22
            Hint = #12486#12473#12488#20877#29983
            Anchors = [akTop, akRight]
            Caption = '>'
            TabOrder = 8
            OnClick = TestSoundBtnClick
          end
          object RefSndTransferBtn: TButton
            Tag = 2
            Left = 509
            Top = 76
            Width = 36
            Height = 22
            Hint = #21442#29031
            Anchors = [akTop, akRight]
            Caption = '...'
            TabOrder = 7
            OnClick = RefSoundBtnClick
          end
        end
        object TextFExtEdit: TLabeledEdit
          Left = 20
          Top = 98
          Width = 567
          Height = 23
          Anchors = [akLeft, akTop, akRight]
          EditLabel.Width = 167
          EditLabel.Height = 15
          EditLabel.Caption = #12486#12461#12473#12488#12514#12540#12489#12391#36578#36865#12377#12427#25313#24373#23376
          TabOrder = 2
          Text = ''
        end
        object UpKeepTimeCheckBox: TCheckBox
          Left = 302
          Top = 137
          Width = 280
          Height = 17
          Caption = #12450#12483#12503#12525#12540#12489#12501#12449#12452#12523#12398#12479#12452#12512#12473#12479#12531#12503#12434#32173#25345
          TabOrder = 4
        end
        object DlKeepTimeCheckBox: TCheckBox
          Left = 20
          Top = 137
          Width = 280
          Height = 17
          Caption = #12480#12454#12531#12525#12540#12489#12501#12449#12452#12523#12398#12479#12452#12512#12473#12479#12531#12503#12434#32173#25345
          TabOrder = 3
        end
        object AutoDisconGroupBox: TGroupBox
          Left = 274
          Top = 10
          Width = 221
          Height = 52
          Caption = #12450#12463#12486#12451#12499#12486#12451#12364#12394#12356#22580#21512#12398#33258#21205#20999#26029
          TabOrder = 1
          object Label1: TLabel
            Left = 152
            Top = 23
            Width = 13
            Height = 15
            Caption = #31186
          end
          object DisconTimeEdit: TLabeledEdit
            Left = 88
            Top = 20
            Width = 60
            Height = 23
            Alignment = taRightJustify
            EditLabel.Width = 58
            EditLabel.Height = 23
            EditLabel.Caption = #12479#12452#12512#12450#12454#12488
            LabelPosition = lpLeft
            NumbersOnly = True
            TabOrder = 0
            Text = ''
          end
        end
        object LogResCheckBox: TCheckBox
          Left = 302
          Top = 160
          Width = 280
          Height = 17
          Caption = #12507#12473#12488#12363#12425#12398#24540#31572#12513#12483#12475#12540#12472#12434#12525#12464#12395#34920#31034
          TabOrder = 6
        end
        object UpToLowerCheckBox: TCheckBox
          Left = 20
          Top = 160
          Width = 280
          Height = 17
          Caption = #12501#12449#12452#12523#21517#12434#23567#25991#23383#21270#12375#12390#12450#12483#12503#12525#12540#12489#12377#12427
          TabOrder = 5
        end
        object RemoteGroupBox: TGroupBox
          Left = 12
          Top = 10
          Width = 250
          Height = 52
          Caption = #12522#12514#12540#12488#20596
          TabOrder = 0
          object RSide0Btn: TRadioButton
            Left = 8
            Top = 22
            Width = 65
            Height = 17
            Caption = #12459#12524#12531#12488
            TabOrder = 0
          end
          object RSide1Btn: TRadioButton
            Left = 87
            Top = 22
            Width = 60
            Height = 17
            Caption = #24038
            TabOrder = 1
          end
          object RSide2Btn: TRadioButton
            Left = 168
            Top = 22
            Width = 60
            Height = 17
            Caption = #21491
            TabOrder = 2
          end
        end
      end
    end
  end
  object ActionList1: TActionList
    Left = 536
    Top = 40
    object AddHostAction: TAction
      Caption = #36861#21152
      OnExecute = AddHostActionExecute
      OnUpdate = HostItemActionUpdate
    end
    object ChgHostAction: TAction
      Caption = #22793#26356
      OnExecute = ChgHostActionExecute
      OnUpdate = HostItemActionUpdate
    end
    object ConnectAction: TAction
      Caption = #25509#32154
      OnExecute = ConnectActionExecute
      OnUpdate = ConnectActionUpdate
    end
  end
end
