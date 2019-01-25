object NyanFiForm: TNyanFiForm
  Left = 0
  Top = 0
  HelpContext = 1
  Caption = 'NyanFi'
  ClientHeight = 521
  ClientWidth = 724
  Color = clBtnFace
  ParentFont = True
  KeyPreview = True
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poDesigned
  ShowHint = True
  OnActivate = FormActivate
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyDown = FormKeyDown
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ProgressPanel: TPanel
    Left = 196
    Top = 226
    Width = 320
    Height = 140
    TabOrder = 2
    Visible = False
    StyleElements = [seBorder]
    DesignSize = (
      320
      140)
    object ProgressLabel: TLabel
      Left = 10
      Top = 14
      Width = 300
      Height = 20
      Alignment = taCenter
      AutoSize = False
      StyleElements = []
    end
    object ProgressSubLabel: TLabel
      Left = 10
      Top = 42
      Width = 300
      Height = 20
      Alignment = taCenter
      AutoSize = False
      StyleElements = []
    end
    object WorkProgressBar: TProgressBar
      Left = 40
      Top = 70
      Width = 240
      Height = 17
      Smooth = True
      TabOrder = 0
    end
    object CanDlBtn: TButton
      Left = 120
      Top = 101
      Width = 80
      Height = 26
      Anchors = [akLeft, akBottom]
      Cancel = True
      Caption = #12461#12515#12531#12475#12523
      TabOrder = 1
      Visible = False
      OnClick = CanDlBtnClick
    end
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 501
    Width = 724
    Height = 20
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Pitch = fpVariable
    Font.Style = []
    Panels = <
      item
        Style = psOwnerDraw
        Width = 600
      end
      item
        Style = psOwnerDraw
        Width = 0
      end>
    UseSystemFont = False
    StyleElements = [seBorder]
    OnDrawPanel = StatusBarDrawPanel
  end
  object InpDirPanel: TPanel
    Left = 32
    Top = 52
    Width = 219
    Height = 21
    BevelOuter = bvNone
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Visible = False
    object RefDirBtn: TSpeedButton
      Left = 196
      Top = 0
      Width = 23
      Height = 21
      Align = alRight
      Caption = '...'
      OnClick = RefDirBtnClick
    end
    object InpDirComboBox: TComboBox
      Left = 0
      Top = 0
      Width = 196
      Height = 21
      Align = alClient
      DropDownCount = 20
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      TabStop = False
      OnExit = InpDirEditExit
      OnKeyDown = InpDirEditKeyDown
      OnKeyPress = InpDirEditKeyPress
    end
  end
  object MainContainer: TPanel
    Left = 0
    Top = 0
    Width = 724
    Height = 501
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object GrepPanel: TPanel
      Left = 0
      Top = 0
      Width = 724
      Height = 479
      HelpContext = 56
      Align = alClient
      BevelOuter = bvNone
      ParentColor = True
      ShowCaption = False
      TabOrder = 0
      Visible = False
      object ResultListBox: TListBox
        Left = 0
        Top = 130
        Width = 724
        Height = 349
        Style = lbOwnerDrawFixed
        Align = alLeft
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        DoubleBuffered = False
        MultiSelect = True
        ParentDoubleBuffered = False
        PopupMenu = GrepPopupMenu
        TabOrder = 1
        StyleElements = [seBorder]
        OnClick = ResultListBoxClick
        OnDblClick = ResultListBoxDblClick
        OnDrawItem = ResultListBoxDrawItem
        OnExit = ResultListBoxExit
        OnKeyDown = ResultListBoxKeyDown
      end
      object GrepOpPanel: TPanel
        Left = 0
        Top = 0
        Width = 724
        Height = 130
        Align = alTop
        AutoSize = True
        TabOrder = 0
        object GrepCanBtn: TButton
          Left = 1
          Top = 129
          Width = 722
          Height = 0
          Align = alBottom
          Cancel = True
          TabOrder = 3
          OnClick = GrepCanBtnClick
        end
        object GrepSttPanel: TPanel
          Left = 1
          Top = 99
          Width = 722
          Height = 30
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 2
          object GrepSttSplitter: TSplitter
            Left = 468
            Top = 0
            Width = 4
            Height = 30
            Align = alRight
            Beveled = True
            OnMoved = GrepSttSplitterMoved
          end
          object GrepStatusBar: TStatusBar
            Left = 0
            Top = 0
            Width = 468
            Height = 30
            Align = alClient
            Panels = <
              item
                Width = 240
              end
              item
                Width = 150
              end
              item
                Width = 50
              end>
            ParentFont = True
            UseSystemFont = False
          end
          object GrepFilterPanel: TPanel
            Left = 472
            Top = 0
            Width = 250
            Height = 30
            Align = alRight
            BevelOuter = bvLowered
            TabOrder = 1
            DesignSize = (
              250
              30)
            object GrepFilterEdit: TLabeledEdit
              Left = 62
              Top = 5
              Width = 28
              Height = 22
              Anchors = [akLeft, akTop, akRight]
              AutoSize = False
              EditLabel.Width = 33
              EditLabel.Height = 13
              EditLabel.Caption = #12501#12451#12523#12479
              LabelPosition = lpLeft
              TabOrder = 0
              OnChange = GrepFilterEditChange
              OnEnter = GrepFilterEditEnter
              OnExit = GrepFilterEditExit
              OnKeyDown = GrepFilterEditKeyDown
              OnKeyPress = GrepFilterEditKeyPress
            end
            object MigemoCheckBox: TCheckBox
              Left = 96
              Top = 7
              Width = 72
              Height = 17
              Anchors = [akTop, akRight]
              Caption = '&Migemo'
              TabOrder = 1
              OnClick = GrepFltOptCheckBoxClick
            end
            object AndOrCheckBox: TCheckBox
              Left = 170
              Top = 7
              Width = 80
              Height = 17
              Anchors = [akTop, akRight]
              Caption = '&AND/OR'
              TabOrder = 2
              OnClick = GrepFltOptCheckBoxClick
            end
          end
        end
        object GrepMidPanel: TPanel
          Left = 1
          Top = 65
          Width = 722
          Height = 34
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 1
          object GrepMidSplitter: TSplitter
            Left = 384
            Top = 0
            Width = 4
            Height = 34
            AutoSnap = False
            Beveled = True
          end
          object GrepM1Panel: TPanel
            Left = 0
            Top = 0
            Width = 384
            Height = 34
            Align = alLeft
            BevelOuter = bvNone
            TabOrder = 0
            DesignSize = (
              384
              34)
            object TLabel
              Left = 294
              Top = 9
              Width = 20
              Height = 13
              Anchors = [akTop, akRight]
              Caption = #28145#12373
            end
            object TLabel
              Left = 11
              Top = 8
              Width = 26
              Height = 13
              Caption = #12510#12473#12463
            end
            object SubDirCheckBox: TCheckBox
              Left = 185
              Top = 7
              Width = 103
              Height = 17
              Anchors = [akTop, akRight]
              Caption = #12469#12502#12487#12451#12524#12463#12488#12522
              TabOrder = 1
            end
            object SubDirNCombo: TComboBox
              Left = 321
              Top = 5
              Width = 55
              Height = 21
              Style = csDropDownList
              Anchors = [akTop, akRight]
              TabOrder = 2
            end
            object GrepMaskComboBox: TComboBox
              Left = 48
              Top = 5
              Width = 123
              Height = 21
              Anchors = [akLeft, akTop, akRight]
              DropDownCount = 20
              TabOrder = 0
            end
          end
          object GrepM2Panel: TPanel
            Left = 388
            Top = 0
            Width = 251
            Height = 34
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            DesignSize = (
              251
              34)
            object SkipDirEdit: TLabeledEdit
              Left = 118
              Top = 5
              Width = 123
              Height = 22
              Anchors = [akLeft, akTop, akRight]
              AutoSize = False
              EditLabel.Width = 73
              EditLabel.Height = 13
              EditLabel.Caption = #38500#22806#12487#12451#12524#12463#12488#12522
              LabelPosition = lpLeft
              TabOrder = 0
            end
          end
          object GrepM3Panel: TPanel
            Left = 639
            Top = 0
            Width = 83
            Height = 34
            Align = alRight
            BevelOuter = bvNone
            TabOrder = 2
            DesignSize = (
              83
              34)
            object GrepExOptBtn: TButton
              Left = 5
              Top = 4
              Width = 72
              Height = 24
              Action = GrepOptionAction
              Anchors = [akTop, akRight]
              TabOrder = 0
            end
          end
        end
        object GrepPageControl: TPageControl
          Left = 1
          Top = 1
          Width = 722
          Height = 64
          ActivePage = FindSheet
          Align = alTop
          TabOrder = 0
          TabWidth = 100
          OnChange = GrepPageControlChange
          OnChanging = GrepPageControlChanging
          object FindSheet: TTabSheet
            Caption = #26908#32034'(&S)'
            object GrepTopSplitter: TSplitter
              Left = 702
              Top = 0
              Width = 4
              Height = 36
              Align = alRight
              AutoSnap = False
              Beveled = True
              MinSize = 8
            end
            object GrepT1Panel: TPanel
              Left = 0
              Top = 0
              Width = 702
              Height = 36
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 0
              object GrepT11Panel: TPanel
                Left = 0
                Top = 0
                Width = 150
                Height = 36
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 0
                OnResize = GrepT11PanelResize
                DesignSize = (
                  150
                  36)
                object GrepFindLabel: TLabel
                  Left = 16
                  Top = 11
                  Width = 60
                  Height = 13
                  Alignment = taRightJustify
                  Caption = #26908#32034#25991#23383#21015
                end
                object GrepFindComboBox: TComboBox
                  Left = 80
                  Top = 7
                  Width = 57
                  Height = 21
                  Anchors = [akLeft, akTop, akRight]
                  DropDownCount = 24
                  TabOrder = 0
                  OnEnter = GrepRepComboBoxEnter
                  OnKeyDown = GrepFindComboBoxKeyDown
                  OnKeyPress = GrepFindComboBoxKeyPress
                end
              end
              object GrepT12Panel: TPanel
                Left = 150
                Top = 0
                Width = 552
                Height = 36
                Align = alRight
                BevelOuter = bvNone
                TabOrder = 1
                DesignSize = (
                  552
                  36)
                object StartBtn: TButton
                  Left = 445
                  Top = 5
                  Width = 50
                  Height = 26
                  Action = GrepStartAction
                  Anchors = [akTop, akRight]
                  Default = True
                  TabOrder = 6
                end
                object AbortBtn: TButton
                  Left = 497
                  Top = 5
                  Width = 50
                  Height = 26
                  Action = GrepAbortAction
                  Anchors = [akTop, akRight]
                  TabOrder = 7
                end
                object AndCheckBox: TCheckBox
                  Left = 84
                  Top = 7
                  Width = 52
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = 'AND'
                  TabOrder = 1
                end
                object CaseCheckBox: TCheckBox
                  Left = 138
                  Top = 7
                  Width = 80
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = #22823#23567#21306#21029
                  TabOrder = 2
                end
                object NextLineCheckBox: TCheckBox
                  Left = 364
                  Top = 7
                  Width = 80
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = #27425#34892#34920#31034
                  TabOrder = 5
                end
                object OneMatchCheckBox: TCheckBox
                  Left = 220
                  Top = 7
                  Width = 64
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = '1'#12510#12483#12481
                  TabOrder = 3
                end
                object RegExCheckBox: TCheckBox
                  Left = 2
                  Top = 7
                  Width = 80
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = #27491#35215#34920#29694
                  TabOrder = 0
                  OnClick = RegExCheckBoxClick
                end
                object ExclTagCheckBox: TCheckBox
                  Left = 286
                  Top = 7
                  Width = 76
                  Height = 21
                  Anchors = [akTop, akRight]
                  Caption = #12479#12464#38500#22806
                  TabOrder = 4
                end
              end
            end
            object GrepT2Panel: TPanel
              Left = 706
              Top = 0
              Width = 8
              Height = 36
              Align = alRight
              BevelOuter = bvNone
              Constraints.MinWidth = 4
              TabOrder = 1
            end
          end
          object ReplaceSheet: TTabSheet
            HelpContext = 61
            Caption = #32622#25563'(&R)'
            ImageIndex = 1
            object RepTopSplitter: TSplitter
              Left = 702
              Top = 0
              Width = 4
              Height = 36
              Align = alRight
              AutoSnap = False
              Beveled = True
              MinSize = 8
              ExplicitLeft = 710
            end
            object RepT1Panel: TPanel
              Left = 0
              Top = 0
              Width = 702
              Height = 36
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 0
              OnResize = RepT1PanelResize
              DesignSize = (
                702
                36)
              object RepStrLabel: TLabel
                Left = 209
                Top = 11
                Width = 74
                Height = 13
                Alignment = taRightJustify
                Caption = #8594' '#32622#25563#25991#23383#21015
              end
              object RepFindLabel: TLabel
                Left = 16
                Top = 11
                Width = 60
                Height = 13
                Alignment = taRightJustify
                Caption = #26908#32034#25991#23383#21015
              end
              object AskRepCheckBox: TCheckBox
                Left = 515
                Top = 9
                Width = 80
                Height = 17
                Anchors = [akTop, akRight]
                Caption = #30906#35469#12354#12426
                TabOrder = 4
              end
              object StartRBtn: TButton
                Left = 595
                Top = 5
                Width = 50
                Height = 26
                Action = ReplaceStartAction
                Anchors = [akTop, akRight]
                Default = True
                TabOrder = 5
              end
              object AbortRBtn: TButton
                Left = 647
                Top = 5
                Width = 50
                Height = 26
                Action = GrepAbortAction
                Anchors = [akTop, akRight]
                TabOrder = 6
              end
              object RegExRCheckBox: TCheckBox
                Left = 351
                Top = 7
                Width = 80
                Height = 21
                Anchors = [akTop, akRight]
                Caption = #27491#35215#34920#29694
                TabOrder = 2
                OnClick = RegExRCheckBoxClick
              end
              object CaseRCheckBox: TCheckBox
                Left = 433
                Top = 9
                Width = 80
                Height = 17
                Anchors = [akTop, akRight]
                Caption = #22823#23567#21306#21029
                TabOrder = 3
              end
              object RepFindComboBox: TComboBox
                Left = 80
                Top = 7
                Width = 0
                Height = 21
                Anchors = [akLeft, akTop, akRight]
                DropDownCount = 24
                TabOrder = 0
                OnEnter = GrepRepComboBoxEnter
                OnKeyDown = GrepFindComboBoxKeyDown
                OnKeyPress = GrepFindComboBoxKeyPress
              end
              object RepStrComboBox: TComboBox
                Left = 290
                Top = 7
                Width = 0
                Height = 21
                Anchors = [akLeft, akTop, akRight]
                DropDownCount = 24
                TabOrder = 1
                OnKeyDown = GrepFindComboBoxKeyDown
                OnKeyPress = GrepFindComboBoxKeyPress
              end
            end
            object RepT2Panel: TPanel
              Left = 706
              Top = 0
              Width = 8
              Height = 36
              Align = alRight
              BevelOuter = bvNone
              Constraints.MinWidth = 4
              TabOrder = 1
            end
          end
        end
      end
    end
    object ImgViewPanel: TPanel
      Left = 0
      Top = 0
      Width = 724
      Height = 479
      HelpContext = 7
      Align = alClient
      BevelOuter = bvNone
      Color = clBlack
      DoubleBuffered = True
      ParentBackground = False
      ParentDoubleBuffered = False
      ShowCaption = False
      TabOrder = 2
      Visible = False
      StyleElements = [seBorder]
      object ToolBarI: TToolBar
        Left = 0
        Top = 0
        Width = 724
        Height = 19
        AutoSize = True
        ButtonWidth = 70
        DrawingStyle = dsGradient
        Images = IconImgListI
        List = True
        ShowCaptions = True
        AllowTextButtons = True
        TabOrder = 0
        StyleElements = []
        OnDblClick = ToolBarDblClick
      end
      object SeekPanel: TPanel
        Left = 0
        Top = 445
        Width = 724
        Height = 34
        Align = alBottom
        BevelOuter = bvLowered
        ParentBackground = False
        TabOrder = 2
        object SeekSttPanel: TPanel
          Left = 1
          Top = 1
          Width = 80
          Height = 32
          Align = alLeft
          Alignment = taRightJustify
          BevelOuter = bvNone
          BorderWidth = 4
          TabOrder = 0
        end
        object SeekBar: TTrackBar
          Left = 109
          Top = 1
          Width = 556
          Height = 32
          Align = alClient
          ShowSelRange = False
          TabOrder = 2
          TabStop = False
          TickMarks = tmTopLeft
          OnChange = SeekBarChange
        end
        object SeekTopPanel: TPanel
          Left = 81
          Top = 1
          Width = 28
          Height = 32
          Align = alLeft
          BevelOuter = bvNone
          TabOrder = 1
          object SeekLBtn: TSpeedButton
            Left = 2
            Top = 6
            Width = 24
            Height = 22
            Action = TopFileAction
            Flat = True
          end
        end
        object SeekEndPanel: TPanel
          Left = 665
          Top = 1
          Width = 58
          Height = 32
          Align = alRight
          BevelOuter = bvNone
          TabOrder = 3
          DesignSize = (
            58
            32)
          object SeekRBtn: TSpeedButton
            Left = 2
            Top = 6
            Width = 23
            Height = 22
            Action = EndFileAction
            Flat = True
          end
          object HideSeekBtn: TSpeedButton
            Left = 30
            Top = 6
            Width = 23
            Height = 22
            Action = ShowSeekBarAction
            Anchors = [akTop, akRight]
            Caption = #215
            Flat = True
          end
        end
      end
      object ImgMainPanel: TPanel
        Left = 0
        Top = 19
        Width = 724
        Height = 426
        Align = alClient
        BevelOuter = bvNone
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 1
        object ImgSideSplitter: TSplitter
          Left = 185
          Top = 22
          Width = 4
          Height = 404
          Color = clBtnFace
          ParentColor = False
          OnPaint = SplitterBgPaint
        end
        object ImgSttHeader: TStatusBar
          Left = 0
          Top = 0
          Width = 724
          Height = 22
          Align = alTop
          DoubleBuffered = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBtnText
          Font.Height = -12
          Font.Name = 'MS UI Gothic'
          Font.Pitch = fpVariable
          Font.Style = []
          Panels = <
            item
              Style = psOwnerDraw
              Width = 200
            end
            item
              Style = psOwnerDraw
              Width = 200
            end
            item
              Style = psOwnerDraw
              Width = 100
            end
            item
              Style = psOwnerDraw
              Width = 100
            end
            item
              Style = psOwnerDraw
              Width = 50
            end>
          ParentDoubleBuffered = False
          UseSystemFont = False
          StyleElements = []
          OnDrawPanel = ImgSttHeaderDrawPanel
        end
        object ImgScrollPanel: TPanel
          Left = 189
          Top = 22
          Width = 394
          Height = 404
          Align = alClient
          BevelOuter = bvNone
          Color = clBlack
          ParentBackground = False
          TabOrder = 2
          StyleElements = []
          object ImgScrollBox: TScrollBox
            Left = 0
            Top = 0
            Width = 394
            Height = 404
            HorzScrollBar.Smooth = True
            HorzScrollBar.Tracking = True
            VertScrollBar.Smooth = True
            VertScrollBar.Tracking = True
            Align = alClient
            BorderStyle = bsNone
            TabOrder = 0
            StyleElements = []
            object ViewerImage: TImage
              Left = 0
              Top = 0
              Width = 394
              Height = 404
              Align = alClient
              Center = True
              Proportional = True
              OnDblClick = ViewerImageDblClick
              OnMouseDown = ViewerImageMouseDown
              OnMouseLeave = ViewerImageMouseLeave
              OnMouseMove = ViewerImageMouseMove
              OnMouseUp = ViewerImageMouseUp
            end
          end
        end
        object ImgSidePanel: TPanel
          Left = 0
          Top = 22
          Width = 185
          Height = 404
          Align = alLeft
          BevelOuter = bvNone
          DockSite = True
          ParentBackground = False
          TabOrder = 1
          OnDockDrop = ImgSidePanelDockDrop
          object ToolBarI2: TToolBar
            Left = 0
            Top = 0
            Width = 185
            Height = 22
            AutoSize = True
            ButtonWidth = 85
            DrawingStyle = dsGradient
            Images = IconImgListI
            List = True
            ShowCaptions = True
            AllowTextButtons = True
            TabOrder = 0
            StyleElements = []
            OnDblClick = ToolBarDblClick
          end
          object SideMainPanel: TPanel
            Left = 0
            Top = 22
            Width = 185
            Height = 382
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            object SideDockSplitter: TSplitter
              Tag = 1
              Left = 0
              Top = 2
              Width = 185
              Height = 4
              Cursor = crVSplit
              Align = alTop
              Color = clBtnFace
              ParentColor = False
              OnPaint = SplitterBgPaint
            end
            object ClockBarI: TStatusBar
              Left = 0
              Top = 363
              Width = 185
              Height = 19
              Font.Charset = DEFAULT_CHARSET
              Font.Color = clBtnText
              Font.Height = -13
              Font.Name = 'Meiryo UI'
              Font.Pitch = fpVariable
              Font.Style = []
              Panels = <
                item
                  Bevel = pbNone
                  Style = psOwnerDraw
                  Width = 200
                end>
              SizeGrip = False
              UseSystemFont = False
              StyleElements = []
              OnDrawPanel = StatusBarDrawPanel
            end
            object SideDockPanel: TPanel
              Left = 0
              Top = 0
              Width = 185
              Height = 2
              Align = alTop
              BevelOuter = bvNone
              Color = clBlack
              ParentBackground = False
              TabOrder = 0
              StyleElements = []
              object LoupeDockPanel: TPanel
                Left = 0
                Top = 0
                Width = 185
                Height = 1
                Align = alClient
                BevelOuter = bvNone
                Color = clBlack
                Constraints.MinHeight = 1
                DoubleBuffered = False
                ParentBackground = False
                ParentDoubleBuffered = False
                TabOrder = 0
                StyleElements = []
                OnDockDrop = SidePanelDockDrop
                OnDockOver = SideDockPanelDockOver
                OnUnDock = SidePanelUnDock
              end
              object HistDockPanel: TPanel
                Left = 0
                Top = 1
                Width = 185
                Height = 1
                Align = alBottom
                BevelOuter = bvNone
                Color = clBlack
                Constraints.MinHeight = 1
                DoubleBuffered = False
                ParentBackground = False
                ParentDoubleBuffered = False
                TabOrder = 1
                StyleElements = []
                OnDockDrop = SidePanelDockDrop
                OnDockOver = SideDockPanelDockOver
                OnUnDock = SidePanelUnDock
              end
            end
            object SideInfPanel: TPanel
              Left = 0
              Top = 6
              Width = 185
              Height = 357
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 1
              object ImgInfListBox: TListBox
                Left = 0
                Top = 22
                Width = 185
                Height = 335
                Style = lbOwnerDrawFixed
                Align = alLeft
                Anchors = [akLeft, akTop, akRight, akBottom]
                BorderStyle = bsNone
                DoubleBuffered = False
                ItemHeight = 13
                MultiSelect = True
                ParentDoubleBuffered = False
                PopupMenu = InfPopupMenu
                TabOrder = 1
                StyleElements = []
                OnDrawItem = InfListBoxDrawItem
              end
              object ImgInfBar: TStatusBar
                Left = 0
                Top = 0
                Width = 185
                Height = 22
                Align = alTop
                DoubleBuffered = True
                Font.Charset = DEFAULT_CHARSET
                Font.Color = clBtnText
                Font.Height = -13
                Font.Name = 'Meiryo UI'
                Font.Pitch = fpVariable
                Font.Style = []
                Panels = <
                  item
                    Style = psOwnerDraw
                    Width = 50
                  end
                  item
                    Style = psOwnerDraw
                    Width = 50
                  end>
                ParentDoubleBuffered = False
                UseSystemFont = False
                OnDrawPanel = ImgSttHeaderDrawPanel
              end
            end
          end
        end
        object ThumbnailGrid: TStringGrid
          Left = 583
          Top = 22
          Width = 141
          Height = 404
          Align = alRight
          BevelInner = bvNone
          BevelKind = bkFlat
          BorderStyle = bsNone
          Color = clBlack
          ColCount = 1
          DefaultColWidth = 120
          DefaultRowHeight = 120
          DefaultDrawing = False
          FixedCols = 0
          RowCount = 1
          FixedRows = 0
          GridLineWidth = 3
          Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
          ScrollBars = ssVertical
          TabOrder = 3
          StyleElements = [seBorder]
          OnClick = ThumbnailGridClick
          OnDrawCell = ThumbnailGridDrawCell
          OnKeyDown = ThumbnailGridKeyDown
          OnMouseDown = ThumbnailGridMouseDown
          OnMouseMove = ThumbnailGridMouseMove
          OnMouseUp = ThumbnailGridMouseUp
          OnSelectCell = ThumbnailGridSelectCell
          ColWidths = (
            120)
          RowHeights = (
            120)
        end
      end
    end
    object FKeyBar: TToolBar
      Left = 0
      Top = 479
      Width = 724
      Height = 22
      Align = alBottom
      AutoSize = True
      DoubleBuffered = True
      DrawingStyle = dsGradient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      List = True
      ParentDoubleBuffered = False
      ParentFont = False
      ShowCaptions = True
      TabOrder = 4
      StyleElements = []
    end
    object TxtViewPanel: TPanel
      Left = 0
      Top = 0
      Width = 724
      Height = 479
      HelpContext = 6
      Align = alClient
      BevelOuter = bvNone
      DoubleBuffered = True
      ParentDoubleBuffered = False
      TabOrder = 3
      Visible = False
      StyleElements = [seBorder]
      object ViewMemo: TRichEdit
        Left = 0
        Top = 19
        Width = 724
        Height = 460
        TabStop = False
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Font.Charset = SHIFTJIS_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ReadOnly = True
        ShowHint = False
        TabOrder = 1
        Visible = False
        WantReturns = False
        Zoom = 100
      end
      object ToolBarV: TToolBar
        Left = 0
        Top = 0
        Width = 724
        Height = 19
        AutoSize = True
        ButtonWidth = 70
        DrawingStyle = dsGradient
        Images = IconImgListV
        List = True
        ShowCaptions = True
        AllowTextButtons = True
        TabOrder = 0
        Visible = False
        StyleElements = []
        OnDblClick = ToolBarDblClick
      end
      object TxtMainPanel: TPanel
        Left = 0
        Top = 19
        Width = 724
        Height = 460
        Align = alClient
        BevelOuter = bvNone
        TabOrder = 2
        object TxtSttHeader: TStatusBar
          Left = 0
          Top = 0
          Width = 724
          Height = 19
          Align = alTop
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBtnText
          Font.Height = -12
          Font.Name = 'MS UI Gothic'
          Font.Pitch = fpVariable
          Font.Style = []
          Panels = <
            item
              Style = psOwnerDraw
              Width = 50
            end
            item
              Style = psOwnerDraw
              Width = 50
            end
            item
              Style = psOwnerDraw
              Width = 50
            end
            item
              Style = psOwnerDraw
              Width = 50
            end
            item
              Style = psOwnerDraw
              Width = 50
            end>
          UseSystemFont = False
          StyleElements = []
          OnDrawPanel = TxtSttHeaderDrawPanel
        end
        object TxtScrollPanel: TPanel
          Left = 0
          Top = 19
          Width = 724
          Height = 441
          Align = alClient
          BevelOuter = bvNone
          DoubleBuffered = True
          ParentDoubleBuffered = False
          TabOrder = 1
          StyleElements = [seBorder]
          object TextPaintBox: TPaintBox
            Left = 0
            Top = 16
            Width = 705
            Height = 425
            Align = alClient
            Color = clBtnFace
            Font.Charset = ANSI_CHARSET
            Font.Color = clWindowText
            Font.Height = -15
            Font.Name = #65325#65331' '#65328#12468#12471#12483#12463
            Font.Style = []
            ParentColor = False
            ParentFont = False
            PopupMenu = ViewPopupMenu
            OnDblClick = TextPaintBoxDblClick
            OnMouseDown = TextPaintBoxMouseDown
            OnMouseMove = TextPaintBoxMouseMove
            OnMouseUp = TextPaintBoxMouseUp
            OnPaint = TextPaintBoxPaint
            ExplicitWidth = 707
          end
          object TextRulerBox: TPaintBox
            Left = 0
            Top = 0
            Width = 724
            Height = 16
            Align = alTop
          end
          object TextMarginBox: TPaintBox
            Left = 722
            Top = 16
            Width = 2
            Height = 425
            Align = alRight
            OnPaint = TextMarginBoxPaint
            ExplicitLeft = 684
          end
          object TextScrollBar: TScrollBar
            Left = 705
            Top = 16
            Width = 17
            Height = 425
            Align = alRight
            Kind = sbVertical
            Min = 1
            PageSize = 0
            Position = 1
            TabOrder = 0
            TabStop = False
            OnChange = TextScrollBarChange
          end
          object TextCurColPanel: TPanel
            Left = 32
            Top = 221
            Width = 60
            Height = 24
            TabOrder = 1
            Visible = False
            StyleElements = [seBorder]
          end
        end
      end
    end
    object MainPanel: TPanel
      Left = 0
      Top = 0
      Width = 724
      Height = 479
      Align = alClient
      BevelOuter = bvNone
      ShowCaption = False
      TabOrder = 1
      StyleElements = [seBorder]
      object ListSubSplitter: TSplitter
        Left = 0
        Top = 362
        Width = 724
        Height = 4
        Cursor = crVSplit
        Align = alBottom
        AutoSnap = False
        Color = clBtnFace
        MinSize = 60
        ParentColor = False
        OnCanResize = ListSubSplitterCanResize
        OnPaint = SplitterBgPaint
      end
      object ListPanel: TPanel
        Left = 0
        Top = 22
        Width = 724
        Height = 340
        Align = alClient
        BevelOuter = bvNone
        Constraints.MinHeight = 100
        Constraints.MinWidth = 100
        DoubleBuffered = True
        ParentDoubleBuffered = False
        TabOrder = 1
        OnResize = ListPanelResize
        object LRSplitter: TSplitter
          Left = 342
          Top = 53
          Width = 4
          Height = 287
          AutoSnap = False
          Color = clBtnFace
          ParentColor = False
          OnCanResize = LRSplitterCanResize
          OnMoved = LRSplitterMoved
          OnPaint = SplitterBgPaint
        end
        object L_Panel: TPanel
          Left = 0
          Top = 53
          Width = 342
          Height = 287
          Align = alLeft
          BevelOuter = bvNone
          Constraints.MinWidth = 30
          TabOrder = 2
          OnResize = L_PanelResize
          object L_StatPanel: TPanel
            Left = 0
            Top = 269
            Width = 342
            Height = 18
            Align = alBottom
            Alignment = taRightJustify
            BevelEdges = [beTop]
            BevelKind = bkFlat
            BevelOuter = bvLowered
            TabOrder = 3
            StyleElements = [seBorder]
            OnClick = L_StatPanelClick
            object CaretPaintBox: TPaintBox
              Left = 256
              Top = 0
              Width = 4
              Height = 16
              Visible = False
              OnPaint = CaretPaintBoxPaint
            end
            object FilterComboBox: TComboBox
              Left = 74
              Top = 0
              Width = 150
              Height = 21
              TabOrder = 0
              Visible = False
              OnChange = FilterComboBoxChange
              OnExit = FilterComboBoxExit
              OnKeyDown = FilterComboBoxKeyDown
              OnKeyPress = FilterComboBoxKeyPress
            end
          end
          object L_ListBox: TListBox
            Left = 0
            Top = 38
            Width = 342
            Height = 231
            HelpContext = 4
            Style = lbVirtualOwnerDraw
            AutoComplete = False
            Align = alLeft
            Anchors = [akLeft, akTop, akRight, akBottom]
            BevelInner = bvLowered
            BorderStyle = bsNone
            Constraints.MinHeight = 16
            DoubleBuffered = False
            ItemHeight = 20
            MultiSelect = True
            ParentDoubleBuffered = False
            ParentShowHint = False
            ShowHint = False
            TabOrder = 1
            StyleElements = [seBorder]
            OnDblClick = FileListBoxDblClick
            OnDragDrop = FileListBoxDragDrop
            OnDragOver = FileListBoxDragOver
            OnDrawItem = FileListDrawItem
            OnEnter = FileListBoxEnter
            OnExit = FileListBoxExit
            OnKeyDown = FileListKeyDown
            OnKeyPress = FileListKeyPress
            OnKeyUp = FileListBoxKeyUp
            OnMouseDown = FileListBoxMouseDown
            OnMouseMove = FileListBoxMouseMove
            OnMouseUp = FileListBoxMouseUp
          end
          object L_HeaderControl: THeaderControl
            Left = 0
            Top = 20
            Width = 342
            Height = 18
            Sections = <
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end>
            OnDrawSection = FL_HeaderControlDrawSection
            OnSectionClick = FileListHeaderSectionClick
            NoSizing = True
          end
          object L_HdrPanel: TPanel
            Left = 0
            Top = 0
            Width = 342
            Height = 20
            Align = alTop
            Alignment = taLeftJustify
            TabOrder = 0
            Visible = False
            StyleElements = [seBorder]
            object L_SelDrvBtn2: TSpeedButton
              Left = 1
              Top = 1
              Width = 16
              Height = 18
              Align = alLeft
              Flat = True
              Layout = blGlyphTop
              OnClick = SelDrvBtnClick
            end
            object L_DirPanel2: TPanel
              Left = 17
              Top = 1
              Width = 276
              Height = 18
              Align = alClient
              Alignment = taLeftJustify
              BevelOuter = bvNone
              ParentBackground = False
              ParentColor = True
              TabOrder = 0
              OnClick = L_DirPanelClick
            end
            object RelPanel2: TPanel
              Left = 293
              Top = 1
              Width = 48
              Height = 18
              Align = alRight
              BevelOuter = bvLowered
              TabOrder = 1
              StyleElements = [seBorder]
              object RelPaintBox2: TPaintBox
                Left = 1
                Top = 1
                Width = 46
                Height = 16
                Align = alClient
                OnPaint = RelPaintBoxPaint
              end
            end
          end
        end
        object R_Panel: TPanel
          Left = 346
          Top = 53
          Width = 378
          Height = 287
          Align = alClient
          BevelOuter = bvNone
          Constraints.MinWidth = 30
          TabOrder = 3
          OnResize = R_PanelResize
          object R_ListBox: TListBox
            Tag = 1
            Left = 0
            Top = 38
            Width = 378
            Height = 231
            HelpContext = 4
            Style = lbVirtualOwnerDraw
            AutoComplete = False
            Align = alLeft
            Anchors = [akLeft, akTop, akRight, akBottom]
            BevelInner = bvLowered
            BorderStyle = bsNone
            Constraints.MinHeight = 16
            DoubleBuffered = False
            ItemHeight = 20
            MultiSelect = True
            ParentDoubleBuffered = False
            ParentShowHint = False
            ShowHint = False
            TabOrder = 1
            StyleElements = [seBorder]
            OnDblClick = FileListBoxDblClick
            OnDragDrop = FileListBoxDragDrop
            OnDragOver = FileListBoxDragOver
            OnDrawItem = FileListDrawItem
            OnEnter = FileListBoxEnter
            OnExit = FileListBoxExit
            OnKeyDown = FileListKeyDown
            OnKeyPress = FileListKeyPress
            OnKeyUp = FileListBoxKeyUp
            OnMouseDown = FileListBoxMouseDown
            OnMouseMove = FileListBoxMouseMove
            OnMouseUp = FileListBoxMouseUp
          end
          object R_StatPanel: TPanel
            Tag = 1
            Left = 0
            Top = 269
            Width = 378
            Height = 18
            Align = alBottom
            Alignment = taRightJustify
            BevelEdges = [beTop]
            BevelKind = bkFlat
            BevelOuter = bvLowered
            TabOrder = 3
            StyleElements = [seBorder]
            OnClick = R_StatPanelClick
          end
          object R_HeaderControl: THeaderControl
            Tag = 1
            Left = 0
            Top = 20
            Width = 378
            Height = 18
            Sections = <
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                FixedWidth = True
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end
              item
                ImageIndex = -1
                Style = hsOwnerDraw
                Width = 50
              end>
            OnDrawSection = FL_HeaderControlDrawSection
            OnSectionClick = FileListHeaderSectionClick
            NoSizing = True
          end
          object R_HdrPanel: TPanel
            Left = 0
            Top = 0
            Width = 378
            Height = 20
            Align = alTop
            Alignment = taLeftJustify
            TabOrder = 0
            Visible = False
            StyleElements = [seBorder]
            object R_SelDrvBtn2: TSpeedButton
              Tag = 1
              Left = 1
              Top = 1
              Width = 16
              Height = 18
              Align = alLeft
              Flat = True
              Layout = blGlyphTop
              OnClick = SelDrvBtnClick
            end
            object R_DirPanel2: TPanel
              Tag = 1
              Left = 17
              Top = 1
              Width = 360
              Height = 18
              Align = alClient
              Alignment = taLeftJustify
              BevelOuter = bvNone
              ParentBackground = False
              ParentColor = True
              TabOrder = 0
              OnClick = R_DirPanelClick
            end
          end
        end
        object TabPanel: TPanel
          Left = 0
          Top = 0
          Width = 724
          Height = 31
          Align = alTop
          BevelOuter = bvNone
          TabOrder = 0
          StyleElements = [seBorder]
          object TabBottomPaintBox: TPaintBox
            Left = 0
            Top = 30
            Width = 724
            Height = 1
            Align = alBottom
            OnPaint = TabBottomPaintBoxPaint
          end
          object TabOuterPanel: TPanel
            Left = 0
            Top = 0
            Width = 724
            Height = 30
            Align = alClient
            BevelOuter = bvNone
            ParentColor = True
            TabOrder = 0
            StyleElements = []
            DesignSize = (
              724
              30)
            object PopTabBtn: TSpeedButton
              Left = 0
              Top = 4
              Width = 16
              Height = 25
              Flat = True
              Layout = blGlyphTop
              OnClick = PopTabBtnClick
            end
            object TabControl1: TTabControl
              Left = 16
              Top = 4
              Width = 709
              Height = 27
              Anchors = [akLeft, akTop, akRight]
              DoubleBuffered = True
              OwnerDraw = True
              ParentDoubleBuffered = False
              TabOrder = 0
              TabStop = False
              StyleElements = []
              OnChange = TabControl1Change
              OnChanging = TabControl1Changing
              OnDrawTab = TabControl1DrawTab
              OnEnter = TabControl1Enter
              OnMouseDown = TabControl1MouseDown
              OnMouseLeave = TabControl1MouseLeave
              OnMouseMove = TabControl1MouseMove
              OnMouseUp = TabControl1MouseUp
              object DelTabBtn: TSpeedButton
                Left = 675
                Top = 6
                Width = 14
                Height = 14
                Flat = True
                Glyph.Data = {
                  C6040000424DC60400000000000036040000280000000C0000000C0000000100
                  08000000000090000000C30E0000C30E000000010000000000001C24AC00232A
                  A900252DBF002B33BF002C34BD002D35BD002C34BF002E35BE002E36BE00262E
                  C000262EC100252EC200262FC9002730C0002F37C900212BD800232DD800252F
                  D9002630D9002731D9002831D9002932D9002933D9002832DA002A34D9002A34
                  DA002B34DA002B35DA002D36D9002C36DA002D36DA002F38D8002E38DA002F38
                  DA002F39DA002F39DC00313ADA00313ADB00323BDB00333CDB00353EDB003039
                  DD00323BDC00313ADE00323CDD00333CDD00353FDC00363FDC00343EDE00353E
                  DE00363FDE003740DD003641DF003840DA003841DC003A43DC003841DE003842
                  DE003943DE003B44DC003F47DD003F48DD00414ADD005058E000545CE100565E
                  E100585FE2005B63E2005D64E2007077E7007177E700747BE7007980E7007B81
                  EB007C82EB007F85E9007E84EC007F85EC007E84ED0000C0C0008187E9008086
                  EC00848AE900888DEA00898FEA008C91EB008E93EB00969BED009B9FEE00999E
                  F1009EA3EE00A0A5EE00A0A4F200A8ADF000B9BDF300C2C6F500C9CCF600CACC
                  F600D3D5F700D6D8F800D8DAF800F3F4FD00F4F5FD00F5F5FD00F7F7FD00F7F7
                  FE00F9FAFE00FBFBFE00FFFFFF00000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  0000000000000000000000000000000000000000000000000000000000000000
                  00000000000000000000000000000000000000000000000000004F0E03040707
                  07080506014F461F2A292D383833232B35004E17475F3C223614565E340A4D19
                  446A62370F536C5D2C094D271A426661546C5B1A320D4D2636163F696C5A102F
                  390D4D262E14526B68603B203A0D4D261D556C573F65643D2D0D4C12486C5818
                  154067633002511E41502128361B434B310B5C3E1C11252626261A13240C4F59
                  4C494A4A4A4A4A4A454F}
                Visible = False
                OnClick = DelTabBtnClick
                OnMouseLeave = DelTabBtnMouseLeave
              end
            end
          end
        end
        object HdrPanel: TPanel
          Left = 0
          Top = 31
          Width = 724
          Height = 22
          Align = alTop
          BevelEdges = [beTop, beBottom]
          TabOrder = 1
          StyleElements = [seBorder]
          object L_TopPanel: TPanel
            Left = 1
            Top = 1
            Width = 320
            Height = 20
            Align = alLeft
            Alignment = taLeftJustify
            BevelOuter = bvNone
            TabOrder = 0
            StyleElements = [seBorder]
            object L_SelDrvBtn: TSpeedButton
              Left = 0
              Top = 0
              Width = 16
              Height = 20
              Align = alLeft
              Flat = True
              Layout = blGlyphTop
              OnClick = SelDrvBtnClick
            end
            object L_DirPanel: TPanel
              Left = 16
              Top = 0
              Width = 304
              Height = 20
              Align = alClient
              Alignment = taLeftJustify
              BevelOuter = bvNone
              ParentBackground = False
              ParentColor = True
              TabOrder = 0
              StyleElements = [seBorder]
              OnClick = L_DirPanelClick
            end
          end
          object R_TopPanel: TPanel
            Tag = 1
            Left = 369
            Top = 1
            Width = 354
            Height = 20
            Align = alClient
            Alignment = taLeftJustify
            BevelOuter = bvNone
            TabOrder = 2
            StyleElements = [seBorder]
            object R_SelDrvBtn: TSpeedButton
              Tag = 1
              Left = 0
              Top = 0
              Width = 16
              Height = 20
              Align = alLeft
              Flat = True
              Layout = blGlyphTop
              OnClick = SelDrvBtnClick
            end
            object R_DirPanel: TPanel
              Tag = 1
              Left = 16
              Top = 0
              Width = 338
              Height = 20
              Align = alClient
              Alignment = taLeftJustify
              BevelOuter = bvNone
              ParentColor = True
              TabOrder = 0
              StyleElements = [seBorder]
              OnClick = R_DirPanelClick
            end
          end
          object RelPanel: TPanel
            Left = 321
            Top = 1
            Width = 48
            Height = 20
            Align = alLeft
            BevelOuter = bvLowered
            TabOrder = 1
            StyleElements = [seBorder]
            object RelPaintBox: TPaintBox
              Left = 1
              Top = 1
              Width = 46
              Height = 18
              Align = alClient
              OnPaint = RelPaintBoxPaint
            end
          end
        end
      end
      object SubPanel: TPanel
        Left = 0
        Top = 366
        Width = 724
        Height = 113
        Align = alBottom
        BevelOuter = bvNone
        Constraints.MinHeight = 32
        Constraints.MinWidth = 32
        TabOrder = 2
        StyleElements = []
        OnResize = SubPanelResize
        object InfLogSplitter: TSplitter
          Tag = 1
          Left = 341
          Top = 0
          Width = 4
          Height = 113
          AutoSnap = False
          MinSize = 50
          OnMoved = InfLogSplitterMoved
          OnPaint = SplitterBgPaint
        end
        object InfPanel: TPanel
          Left = 0
          Top = 0
          Width = 341
          Height = 113
          Align = alLeft
          BevelOuter = bvNone
          ParentBackground = False
          TabOrder = 0
          StyleElements = [seBorder]
          object ImgInfSplitter: TSplitter
            Tag = 1
            Left = 136
            Top = 0
            Width = 4
            Height = 113
            OnCanResize = ImgInfSplitterCanResize
            OnMoved = ImgInfSplitterMoved
            OnPaint = SplitterBgPaint
          end
          object PreviewPanel: TPanel
            Left = 0
            Top = 0
            Width = 136
            Height = 113
            Align = alLeft
            BevelEdges = []
            BevelOuter = bvNone
            ParentBackground = False
            TabOrder = 0
            StyleElements = []
            object LockTxtPanel: TPanel
              Left = 0
              Top = 89
              Width = 136
              Height = 24
              Align = alBottom
              Alignment = taLeftJustify
              BevelEdges = [beTop]
              BevelKind = bkSoft
              BevelOuter = bvNone
              PopupMenu = TxtPrvPopupMenu
              TabOrder = 1
              Visible = False
              StyleElements = [seBorder]
              OnDblClick = LockTxtPanelDblClick
            end
            object PrvInnerPanel: TPanel
              Left = 0
              Top = 0
              Width = 136
              Height = 89
              Align = alClient
              BevelOuter = bvNone
              TabOrder = 0
              StyleElements = []
              DesignSize = (
                136
                89)
              object PreviewImage: TImage
                Left = 0
                Top = 0
                Width = 136
                Height = 53
                Align = alClient
                PopupMenu = ImgPrvPopupMenu
                OnDblClick = PreviewImageDblClick
                OnMouseDown = PreviewImageMouseDown
                OnMouseMove = PreviewImageMouseMove
                OnMouseUp = PreviewImageMouseUp
              end
              object PreviewSizeLabel: TLabel
                Left = 20
                Top = 20
                Width = 50
                Height = 13
                Caption = '100 '#215' 100'
                Transparent = True
                Visible = False
              end
              object PreviewSttLabel: TLabel
                Left = 4
                Top = 2
                Width = 3
                Height = 13
                Anchors = []
                Transparent = True
                Visible = False
              end
              object TxtPrvSplitter: TSplitter
                Tag = 1
                Left = 0
                Top = 53
                Width = 136
                Height = 4
                Cursor = crVSplit
                Align = alBottom
                AutoSnap = False
                Visible = False
                OnMoved = TxtPrvSplitterMoved
                OnPaint = SplitterBgPaint
              end
              object HotPosImage: TImage
                Left = 0
                Top = 0
                Width = 64
                Height = 64
                Transparent = True
                Visible = False
              end
              object TxtPrvListPanel: TPanel
                Left = 0
                Top = 0
                Width = 136
                Height = 53
                Align = alClient
                BevelOuter = bvNone
                TabOrder = 0
                Visible = False
                object TxtPrvListBox: TListBox
                  Left = 0
                  Top = 0
                  Width = 134
                  Height = 53
                  HelpContext = 82
                  Style = lbVirtualOwnerDraw
                  Align = alLeft
                  Anchors = [akLeft, akTop, akRight, akBottom]
                  BorderStyle = bsNone
                  DoubleBuffered = False
                  ItemHeight = 13
                  MultiSelect = True
                  ParentDoubleBuffered = False
                  PopupMenu = TxtPrvPopupMenu
                  TabOrder = 0
                  StyleElements = [seBorder]
                  OnData = TxtPrvListBoxData
                  OnDrawItem = TxtPrvListBoxDrawItem
                  OnEnter = TxtPrvListBoxEnter
                  OnExit = SubListBoxExit
                  OnKeyDown = InfListBoxKeyDown
                  OnMouseDown = SubListBoxMouseDown
                end
              end
              object TxtTailListPanel: TPanel
                Left = 0
                Top = 57
                Width = 136
                Height = 32
                Align = alBottom
                BevelOuter = bvNone
                TabOrder = 1
                Visible = False
                object TxtTailListBox: TListBox
                  Left = 0
                  Top = 0
                  Width = 134
                  Height = 32
                  HelpContext = 82
                  Style = lbVirtualOwnerDraw
                  Align = alLeft
                  Anchors = [akLeft, akTop, akRight, akBottom]
                  BorderStyle = bsNone
                  DoubleBuffered = False
                  ItemHeight = 13
                  MultiSelect = True
                  ParentDoubleBuffered = False
                  PopupMenu = TxtPrvPopupMenu
                  TabOrder = 0
                  StyleElements = [seBorder]
                  OnData = TxtTailListBoxData
                  OnDrawItem = TxtTailListBoxDrawItem
                  OnEnter = TxtPrvListBoxEnter
                  OnExit = SubListBoxExit
                  OnKeyDown = InfListBoxKeyDown
                  OnMouseDown = SubListBoxMouseDown
                end
              end
            end
          end
          object InfListPanel: TPanel
            Left = 140
            Top = 0
            Width = 201
            Height = 113
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            object InfListBox: TListBox
              Left = 0
              Top = 0
              Width = 201
              Height = 113
              HelpContext = 4
              Style = lbOwnerDrawFixed
              Align = alLeft
              Anchors = [akLeft, akTop, akRight, akBottom]
              BorderStyle = bsNone
              Constraints.MinHeight = 32
              Constraints.MinWidth = 32
              DoubleBuffered = False
              ItemHeight = 13
              MultiSelect = True
              ParentDoubleBuffered = False
              PopupMenu = InfPopupMenu
              TabOrder = 0
              StyleElements = [seBorder]
              OnDrawItem = InfListBoxDrawItem
              OnEnter = InfListBoxEnter
              OnExit = SubListBoxExit
              OnKeyDown = InfListBoxKeyDown
              OnMouseDown = SubListBoxMouseDown
            end
          end
        end
        object LogPanel: TPanel
          Left = 345
          Top = 0
          Width = 379
          Height = 113
          Align = alClient
          BevelOuter = bvNone
          DoubleBuffered = True
          ParentDoubleBuffered = False
          TabOrder = 1
          object TaskPaintBox: TPaintBox
            Left = 0
            Top = 0
            Width = 60
            Height = 93
            Align = alLeft
            OnDblClick = TaskPaintBoxDblClick
            OnMouseUp = TaskPaintBoxMouseUp
            OnPaint = TaskPaintBoxPaint
          end
          object ClockBar: TStatusBar
            Left = 0
            Top = 93
            Width = 379
            Height = 20
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -11
            Font.Name = 'Tahoma'
            Font.Pitch = fpVariable
            Font.Style = []
            Panels = <
              item
                Bevel = pbNone
                Style = psOwnerDraw
                Width = 200
              end>
            UseSystemFont = False
            StyleElements = []
            OnDrawPanel = StatusBarDrawPanel
          end
          object LogSplitPanel: TPanel
            Left = 60
            Top = 0
            Width = 1
            Height = 93
            Align = alLeft
            BevelOuter = bvNone
            Color = clGrayText
            ParentBackground = False
            TabOrder = 0
            StyleElements = []
          end
          object LogListPanel: TPanel
            Left = 61
            Top = 0
            Width = 318
            Height = 93
            Align = alClient
            BevelOuter = bvNone
            TabOrder = 1
            object LogListBox: TListBox
              Left = 0
              Top = 0
              Width = 318
              Height = 93
              HelpContext = 4
              Style = lbVirtualOwnerDraw
              Align = alLeft
              Anchors = [akLeft, akTop, akRight, akBottom]
              BorderStyle = bsNone
              DoubleBuffered = False
              MultiSelect = True
              ParentDoubleBuffered = False
              PopupMenu = LogPopupMenu
              TabOrder = 0
              StyleElements = [seBorder]
              OnData = LogListBoxData
              OnDrawItem = LogListBoxDrawItem
              OnEnter = LogListBoxEnter
              OnExit = SubListBoxExit
              OnKeyDown = LogListBoxKeyDown
              OnKeyPress = LogListBoxKeyPress
              OnMouseDown = SubListBoxMouseDown
              OnMouseMove = LogListBoxMouseMove
            end
          end
        end
      end
      object ToolBar1: TToolBar
        Left = 0
        Top = 0
        Width = 724
        Height = 22
        AutoSize = True
        ButtonWidth = 43
        DoubleBuffered = True
        DrawingStyle = dsGradient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Images = IconImgList
        List = True
        ParentDoubleBuffered = False
        ParentFont = False
        ShowCaptions = True
        AllowTextButtons = True
        TabOrder = 0
        Transparent = True
        StyleElements = []
        OnDblClick = ToolBarDblClick
      end
    end
  end
  object MainMenu1: TMainMenu
    AutoHotkeys = maManual
    Left = 32
    Top = 136
    object FileMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #12501#12449#12452#12523'(&F)'
      OnClick = FileMenuClick
      object NewWorkListItem: TMenuItem
        Action = NewWorkListAction
      end
      object LoadWorkFileItem: TMenuItem
        Action = LoadWorkListAction
      end
      object SaveWorkListItem: TMenuItem
        Action = SaveWorkListAction
      end
      object SaveAsWorkFileItem: TMenuItem
        Action = SaveAsWorkListAction
      end
      object WorkListHistoryItem: TMenuItem
        Caption = #26368#36817#38283#12356#12383#12527#12540#12463#12522#12473#12488'(&H)'
      end
      object HomeWorkListItem: TMenuItem
        Action = HomeWorkListAction
      end
      object Sep_f_1: TMenuItem
        Caption = '-'
      end
      object SaveAsTabGroupItem: TMenuItem
        Action = SaveAsTabGroupAction
      end
      object LoadTabGroupItem: TMenuItem
        Action = LoadTabGroupAction
      end
      object Sep_f_2: TMenuItem
        Caption = '-'
      end
      object SaveAsResultItem: TMenuItem
        Action = SaveAsResultListAction
      end
      object LoadResultItem: TMenuItem
        Action = LoadResultListAction
      end
      object Sep_f_3: TMenuItem
        Caption = '-'
      end
      object LibraryItem: TMenuItem
        Caption = #12521#12452#12502#12521#12522
        OnClick = LibraryItemClick
      end
      object OpenTrashItem: TMenuItem
        Action = OpenTrashAction
      end
      object GrepSaveAsItem: TMenuItem
        Action = GrepSaveAsAction
      end
      object ExportCsvItem: TMenuItem
        Action = ExportCsvAction
      end
      object SaveDumpItem: TMenuItem
        Action = SaveDumpAction
      end
      object Sep_f_4: TMenuItem
        Caption = '-'
      end
      object PropertyItem: TMenuItem
        Action = PropertyDlgAction
      end
      object OpenAsRunItem: TMenuItem
        Action = ShowOpenAsRunAction
      end
      object EjectItem: TMenuItem
        Action = EjectAction
      end
      object Sep_f_5: TMenuItem
        Caption = '-'
      end
      object NewTextItem: TMenuItem
        Action = NewTextFileAction
      end
      object NewFileItem: TMenuItem
        Action = NewFileAction
      end
      object PrintItem: TMenuItem
        Action = PrintAction
      end
      object Sep_f_6: TMenuItem
        Caption = '-'
      end
      object CreDirsItem: TMenuItem
        Action = CreateDirsDlgAction
      end
      object BackupItem: TMenuItem
        Action = BackupAction
      end
      object DistributionItem: TMenuItem
        Action = DistributionDlgAction
      end
      object Sep_f_7: TMenuItem
        Caption = '-'
      end
      object DuplicateItem: TMenuItem
        Action = DuplicateAction
      end
      object RestartItem: TMenuItem
        Action = RestartAction
      end
      object ExitItem: TMenuItem
        Action = ExitAction
      end
    end
    object EditMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #32232#38598'(&E)'
      SubMenuImages = IconImgListP
      OnClick = ExtMenuClick
      object MaskSelItem: TMenuItem
        Action = MaskSelectAction
      end
      object SelAllFileItem: TMenuItem
        Action = SelAllFileAction
      end
      object SelAllItem: TMenuItem
        Action = SelAllItemAction
      end
      object InvSelItem: TMenuItem
        Action = SelReverseAction
      end
      object Sep_e_1: TMenuItem
        Caption = '-'
      end
      object CopyFielNameItem: TMenuItem
        Action = CopyFileNameAction
        Caption = #12497#12473#21517#12434#12467#12500#12540'(&C)'
      end
      object MakeShortcutItem: TMenuItem
        Action = CreateShortcutAction
      end
      object TV_EditCopyItem: TMenuItem
        Action = TV_EditCopyAction
      end
      object TV_EditSelectAllItem: TMenuItem
        Action = TV_EditSelAllAction
      end
      object IV_ClipCopyItem: TMenuItem
        Action = ClipCopyAction
      end
      object GrepClipCopyItem: TMenuItem
        Action = GrepClipCopyAction
      end
      object Sep_e_2: TMenuItem
        Caption = '-'
      end
      object SetAliasItem: TMenuItem
        Action = SetAliasAction
      end
      object InsSeparatorItem: TMenuItem
        Action = InsSeparatorAction
      end
      object WorkItemUpItem: TMenuItem
        Action = WorkItemUpAction
      end
      object WorkItemDownItem: TMenuItem
        Action = WorkItemDownAction
      end
      object Sep_e_3: TMenuItem
        Caption = '-'
      end
      object SetTagItem: TMenuItem
        Action = SetTagAction
      end
      object AddTagItem: TMenuItem
        Action = AddTagAction
      end
      object TrimTagDataItem: TMenuItem
        Action = TrimTagDataAction
      end
      object Sep_e_4: TMenuItem
        Caption = '-'
      end
      object PushResultItem: TMenuItem
        Action = PushResultAction
      end
      object PopResultItem: TMenuItem
        Action = PopResultAction
      end
      object Sep_e_5: TMenuItem
        Caption = '-'
      end
      object GrepExtractItem: TMenuItem
        Action = GrepExtractAction
      end
      object GrepExceptItem: TMenuItem
        Action = GrepExceptAction
      end
      object GrepReleaseItem: TMenuItem
        Action = GrepReleaseAction
      end
      object GrepConfirmItem: TMenuItem
        Action = GrepConfirmAction
      end
      object Sep_e_6: TMenuItem
        Caption = '-'
      end
      object GrepSelResItem: TMenuItem
        Action = GrepSelResAction
      end
    end
    object FindMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #26908#32034'(&S)'
      OnClick = MainMenuClick
      object FindFileItem: TMenuItem
        Action = FindFileDlgAction
      end
      object FindDirItem: TMenuItem
        Action = FindDirDlgAction
      end
      object FindTagItem: TMenuItem
        Action = FindTagAction
      end
      object OK1: TMenuItem
        Action = FindDuplDlgAction
      end
      object TV_FindTextItem: TMenuItem
        Action = FindTextAction
      end
      object GrepItem: TMenuItem
        Action = GrepAction
      end
      object WevSearchItem: TMenuItem
        Action = WebSearchAction
      end
      object Sep_s_1: TMenuItem
        Caption = '-'
      end
      object CompDlgItem: TMenuItem
        Action = CompareDlgAction
      end
      object DiffDirItem: TMenuItem
        Action = DiffDirAction
      end
    end
    object ViewMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #34920#31034'(&V)'
      OnClick = MainMenuClick
      object ReternListItem: TMenuItem
        Action = ReturnListAction
      end
      object ReloadItem: TMenuItem
        Action = ReloadListAction
      end
      object Sep_v_1: TMenuItem
        Caption = '-'
      end
      object ShowIconItem: TMenuItem
        Action = ShowIconAction
      end
      object ShowPreviewItem: TMenuItem
        Action = ShowPreviewAction
      end
      object ShowPropertyItem: TMenuItem
        Action = ShowPropertyAction
      end
      object ShowLogWinItem: TMenuItem
        Action = ShowLogWinAction
      end
      object ShowToolBarItem: TMenuItem
        Action = ShowToolBarAction
      end
      object ShowTabBarItem: TMenuItem
        Action = ShowTabBarAction
      end
      object K1: TMenuItem
        Action = ShowFKeyBarAction
      end
      object ShowSttBarItem: TMenuItem
        Action = ShowStatusBarAction
      end
      object ShowRulerItem: TMenuItem
        Action = ShowRulerAction
      end
      object ShowLineNoItem: TMenuItem
        Action = ShowLineNoAction
      end
      object ShowSeekBarItem: TMenuItem
        Action = ShowSeekBarAction
      end
      object Sep_v_2: TMenuItem
        Caption = '-'
      end
      object FileListOnlyItem: TMenuItem
        Action = FileListOnlyAction
      end
      object HideSizeTimeItem: TMenuItem
        Action = HideSizeTimeAction
      end
      object ChangeCodePageItem: TMenuItem
        Action = ChgCodePageAction
        object CpShiftJISItem: TMenuItem
          Action = CP_ShiftJisAction
        end
        object CpJisItem: TMenuItem
          Action = CP_JisAction
        end
        object CpEucItem: TMenuItem
          Action = CP_EucAction
        end
        object CpLatin1Item: TMenuItem
          Action = CP_Latin1Action
        end
        object CpUtf8Item: TMenuItem
          Action = CP_UTF8Action
        end
        object CpUtf16Item: TMenuItem
          Action = CP_UTF16Action
        end
        object CpAutoItem: TMenuItem
          Action = CP_AutoAction
        end
      end
      object HtmlToTextItem: TMenuItem
        Action = HtmlToTextAction
      end
      object FixedLenItem: TMenuItem
        Action = FixedLenAction
      end
      object ShowRubyItem: TMenuItem
        Action = ShowRubyAction
      end
      object ShowTabItem: TMenuItem
        Action = ShowTABAction
      end
      object ShowCrItem: TMenuItem
        Action = ShowCRAction
      end
      object Sep_v_3: TMenuItem
        Caption = '-'
      end
      object CharInfoItem: TMenuItem
        Action = CharInfoAction
      end
      object CsvRecordItem: TMenuItem
        Action = CsvRecordAction
      end
      object InspectorItem: TMenuItem
        Action = InspectorAction
      end
      object BitmapViewItem: TMenuItem
        Action = BitmapViewAction
      end
      object GrepShowItemNoItem: TMenuItem
        Action = GrepShowItemNoAction
      end
      object GrepFileItemNoItem: TMenuItem
        Action = GrepFileItemNoAction
      end
      object GrepShowSubDirItem: TMenuItem
        Action = GrepShowSubDirAction
      end
      object GrepTrimTopItem: TMenuItem
        Action = GrepTrimTopAction
      end
      object GrepOmitTopItem: TMenuItem
        Action = GrepOmitTopAction
      end
      object GrepEmFilterItem: TMenuItem
        Action = GrepEmFilterAction
      end
      object Sep_v_4: TMenuItem
        Caption = '-'
      end
      object EqualSizeItem: TMenuItem
        Action = EqualSizeAction
      end
      object FittedSizeItem: TMenuItem
        Action = FittedSizeAction
      end
      object ZoomInItem: TMenuItem
        Action = ZoomInIAction
      end
      object ZoomOutItem: TMenuItem
        Action = ZoomOutIAction
      end
      object FullScreenItem: TMenuItem
        Action = FullScreenAction
      end
      object DoublePageItem: TMenuItem
        Action = DoublePageAction
      end
      object PageBindItem: TMenuItem
        Action = PageBindAction
      end
      object Sep_v_5: TMenuItem
        Caption = '-'
      end
      object GrayScaleItem: TMenuItem
        Action = GrayScaleAction
      end
      object ShowGridItem: TMenuItem
        Action = ShowGridAction
        Caption = #30011#20687#20998#21106#12464#12522#12483#12489#12434#34920#31034'(&L)'
      end
      object WarnHighItem: TMenuItem
        Action = WarnHighlightAction
      end
      object Sep_v_6: TMenuItem
        Caption = '-'
      end
      object ThumbnailItem: TMenuItem
        Action = ThumbnailAction
      end
      object ThumbnailExItem: TMenuItem
        Action = ThumbnailExAction
      end
      object ThumbPosItem: TMenuItem
        Action = IV_ThumbPosAction
        object ThumbPos0Item: TMenuItem
          Caption = #19978
          OnClick = ThumbPosItemClick
        end
        object ThumbPos1Item: TMenuItem
          Tag = 1
          Caption = #19979
          OnClick = ThumbPosItemClick
        end
        object ThumbPos2Item: TMenuItem
          Tag = 2
          Caption = #24038
          OnClick = ThumbPosItemClick
        end
        object ThumbPos3Item: TMenuItem
          Tag = 3
          Caption = #21491
          OnClick = ThumbPosItemClick
        end
      end
      object Sep_v_7: TMenuItem
        Caption = '-'
      end
      object SidebarItem: TMenuItem
        Action = SidebarAction
        Caption = #12469#12452#12489#12496#12540#12434#34920#31034'(&B)'
      end
      object SidebarPosItem: TMenuItem
        Action = IV_SidebarPosAction
        object SidebarPos0Item: TMenuItem
          Caption = #24038
          OnClick = SidebarPosItemClick
        end
        object SidebarPos1Item: TMenuItem
          Tag = 1
          Caption = #21491
          OnClick = SidebarPosItemClick
        end
      end
      object LoupeItem: TMenuItem
        Action = LoupeAction
      end
      object HistogramItem: TMenuItem
        Action = HistogramAction
      end
      object SubViewerItem: TMenuItem
        Action = SubViewerAction
      end
      object ColPickerItem: TMenuItem
        Action = ColorPickerAction
      end
    end
    object ListMenu: TMenuItem
      Caption = #19968#35239'(&L)'
      object SelDriveItem: TMenuItem
        Action = DriveListAction
      end
      object PathMaskItem: TMenuItem
        Action = PathMaskDlgAction
      end
      object RegDirItem: TMenuItem
        Action = RegDirDlgAction
      end
      object N1: TMenuItem
        Action = SpecialDirListAction
      end
      object DirHistItem: TMenuItem
        Action = DirHistoryAction
      end
      object DirStackItem: TMenuItem
        Action = DirStackAction
      end
      object EditHistoryItem: TMenuItem
        Action = EditHistoryAction
      end
      object ViewHistoryItem: TMenuItem
        Action = ViewHistoryAction
      end
      object RecentListItem: TMenuItem
        Action = RecentListAction
      end
      object MarkListItem: TMenuItem
        Action = MarkListAction
      end
      object ShareListItem: TMenuItem
        Action = ShareListAction
      end
      object Sep_l_1: TMenuItem
        Caption = '-'
      end
      object KeyListItem: TMenuItem
        Action = KeyListAction
      end
      object CmdFileListItem: TMenuItem
        Action = CmdFileListAction
        Caption = #12467#12510#12531#12489#12501#12449#12452#12523#19968#35239'(&C)...'
      end
      object AppListItem: TMenuItem
        Action = AppListAction
      end
      object FunctionListItem: TMenuItem
        Action = FunctionListAction
      end
      object UserDefListItem: TMenuItem
        Action = UserDefListAction
      end
      object MarkListVItem: TMenuItem
        Action = MarkListVAction
      end
      object TaskManItem: TMenuItem
        Action = TaskManAction
      end
      object Sep_l_2: TMenuItem
        Caption = '-'
      end
      object FileExtListItem: TMenuItem
        Action = FileExtListAction
      end
      object DriveGraphItem: TMenuItem
        Action = DriveGraphAction
      end
    end
    object ToolMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #12484#12540#12523'(&T)'
      SubMenuImages = IconImgListP
      OnClick = ExtMenuClick
      object OpenExpItem: TMenuItem
        Action = OpenByExpAction
      end
      object Sep_t_1: TMenuItem
        Caption = '-'
      end
      object PowerShellItem: TMenuItem
        Action = PowerShellAction
      end
      object ComExeItem: TMenuItem
        Action = CommandPromptAction
      end
      object ExecuteItem: TMenuItem
        Action = ExeCommandLineAction
      end
      object FileRunItem: TMenuItem
        Action = FileRunAction
      end
      object Sep_t_2: TMenuItem
        Caption = '-'
      end
      object NetConDlgItem: TMenuItem
        Action = NetConnectAction
      end
      object NetDisDlgItem: TMenuItem
        Action = NetDisconnectAction
      end
      object Sep_t_3: TMenuItem
        Caption = '-'
      end
      object FtpConnectItem: TMenuItem
        Action = FTPConnectAction
      end
      object FtpDisconItem: TMenuItem
        Action = FTPDisconnectAction
      end
      object FtpChmodItem: TMenuItem
        Action = FTPChmodAction
      end
      object Sep_t_4: TMenuItem
        Caption = '-'
      end
      object ToUpperItem: TMenuItem
        Action = NameToUpperAction
      end
      object ToLowerItem: TMenuItem
        Action = NameToLowerAction
      end
      object SetExifTimeItem: TMenuItem
        Action = SetExifTimeAction
      end
      object DelJpgExifItem: TMenuItem
        Action = DelJpgExifAction
      end
      object ConvertImageItem: TMenuItem
        Action = ConvertImageAction
      end
      object ExtractMp3ImgItem: TMenuItem
        Action = ExtractMp3ImgAction
      end
      object ExtractIconItem: TMenuItem
        Action = ExtractIconAction
      end
      object JoinTextItem: TMenuItem
        Action = JoinTextAction
      end
      object TextEncItem: TMenuItem
        Action = ConvertTextEncAction
      end
      object Htm2TxtItem: TMenuItem
        Action = ConvertHtm2TxtAction
      end
      object Sep_t_5: TMenuItem
        Caption = '-'
      end
      object CalcItem: TMenuItem
        Action = CalculatorAction
      end
      object RegExChkItem: TMenuItem
        Action = RegExCheckerAction
      end
    end
    object OptionMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #35373#23450'(&O)'
      OnClick = MainMenuClick
      object OptionItem: TMenuItem
        Action = OptionDlgAction
      end
      object SetToolBtnItem: TMenuItem
        Action = ToolBarDlgAction
      end
      object GrepOptionItem: TMenuItem
        Action = GrepOptionAction
        Caption = 'Grep'#12398#25313#24373#35373#23450'(&G)...'
      end
      object RegSyncDlgItem: TMenuItem
        Action = RegSyncDlgAction
      end
      object SetColorItem: TMenuItem
        Action = SetColorAction
      end
      object Sep_o_1: TMenuItem
        Caption = '-'
      end
      object ViewIniFileItem: TMenuItem
        Action = ViewIniFileAction
      end
      object EditIniFileItem: TMenuItem
        Action = EditIniFileAction
      end
      object Sep_o_2: TMenuItem
        Caption = '-'
      end
      object DotNyanDlgItem: TMenuItem
        Action = DotNyanDlgAction
      end
      object Sep_o_3: TMenuItem
        Caption = '-'
      end
      object EditHighlightItem: TMenuItem
        Action = EditHighlightAction
      end
      object DefHighlightItem: TMenuItem
        Action = DefHighlightAction
      end
      object Sep_o_4: TMenuItem
        Caption = '-'
      end
      object EditSwatchItem: TMenuItem
        Caption = #33394#35211#26412#23450#32681#12501#12449#12452#12523#12398#32232#38598
        OnClick = EditSwatchItemClick
      end
      object EditFontSmplItem: TMenuItem
        Caption = #12501#12457#12531#12488#12469#12531#12503#12523#23450#32681#12501#12449#12452#12523#12398#32232#38598
        OnClick = EditFontSmplItemClick
      end
    end
    object HelpMenu: TMenuItem
      AutoHotkeys = maAutomatic
      Caption = #12504#12523#12503'(&H)'
      OnClick = MainMenuClick
      object HelpContentsItem: TMenuItem
        Action = HelpContentsAction
      end
      object CmdIndexItem: TMenuItem
        Caption = #12467#12510#12531#12489#12398#32034#24341'(&I)'
        HelpContext = 100
        OnClick = CmdIndexItemClick
      end
      object HelpHistoryItem: TMenuItem
        Caption = #22793#26356#23653#27508'(&H)'
        OnClick = HelpHistoryItemClick
      end
      object AboutItem: TMenuItem
        Action = AboutNyanFiAction
      end
      object Sep_h_1: TMenuItem
        Caption = '-'
      end
      object UpdateItem: TMenuItem
        Action = CheckUpdateAction
      end
      object UpdateFromArcItem: TMenuItem
        Action = UpdateFromArcAction
      end
    end
  end
  object ActionList1: TActionList
    OnExecute = ActionList1Execute
    Left = 32
    Top = 88
    object ClipCopyAction: TAction
      Category = 'CommandI'
      Caption = #30011#20687#12434#12463#12522#12483#12503#12508#12540#12489#12395#12467#12500#12540'(&C)'
      OnExecute = ClipCopyActionExecute
      OnUpdate = ClipCopyActionUpdate
    end
    object CloseIAction: TAction
      Category = 'CommandI'
      OnExecute = CloseIActionExecute
    end
    object ColorPickerAction: TAction
      Category = 'CommandI'
      Caption = #12459#12521#12540#12500#12483#12459#12540'(&C)...'
      OnExecute = ColorPickerActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object DefHighlightAction: TAction
      Category = 'Menu'
      Caption = #12487#12501#12457#12523#12488#12398#27083#25991#24375#35519#34920#31034#23450#32681#12434#12467#12500#12540
      HelpContext = 84
      OnExecute = DefHighlightActionExecute
      OnUpdate = MenuFVActionUpdate
    end
    object TV_EditCopyAction: TEditCopy
      Category = 'Menu'
      Caption = #12467#12500#12540'(&C)'
      OnExecute = TV_EditCopyActionExecute
      OnUpdate = TV_EditCopyActionUpdate
    end
    object TV_EditAddAction: TAction
      Category = 'Menu'
      Caption = #36861#21152#12467#12500#12540
      OnExecute = TV_EditAddActionExecute
      OnUpdate = TV_EditCopyActionUpdate
    end
    object TV_EditSelAllAction: TEditSelectAll
      Category = 'Menu'
      Caption = #12377#12409#12390#36984#25246'(&A)'
      OnExecute = TV_EditSelAllActionExecute
      OnUpdate = TxtViewEditActionUpdate
    end
    object TV_SortAscAction: TAction
      Category = 'Menu'
      Caption = #26119#38918'(&A)'
      OnExecute = TV_SortAscActionExecute
      OnUpdate = TV_SortActionUpdate
    end
    object TV_SortDesAction: TAction
      Category = 'Menu'
      Caption = #38477#38918'(&D)'
      OnExecute = TV_SortDesActionExecute
      OnUpdate = TV_SortActionUpdate
    end
    object TV_OrgOrderAction: TAction
      Category = 'Menu'
      Caption = #35299#38500'(&R)'
      OnExecute = TV_OrgOrderActionExecute
      OnUpdate = TV_SortActionUpdate
    end
    object DeleteIAction: TAction
      Category = 'CommandI'
      OnExecute = DeleteIActionExecute
    end
    object DoublePageAction: TAction
      Category = 'CommandI'
      Caption = #35211#38283#12365#34920#31034'(&D)'
      OnExecute = DoublePageActionExecute
      OnUpdate = DoublePageActionUpdate
    end
    object EndFileAction: TAction
      Category = 'CommandI'
      OnExecute = EndFileActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object EqualSizeAction: TAction
      Category = 'CommandI'
      Caption = #31561#20493#34920#31034'(&E)'
      OnExecute = EqualSizeActionExecute
      OnUpdate = EqualSizeActionUpdate
    end
    object FittedSizeAction: TAction
      Category = 'CommandI'
      Caption = #30011#38754#12501#12451#12483#12488#34920#31034'(&F)'
      OnExecute = FittedSizeActionExecute
      OnUpdate = FittedSizeActionUpdate
    end
    object FlipHorzAction: TAction
      Category = 'CommandI'
      OnExecute = FlipHorzActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FlipVertAction: TAction
      Category = 'CommandI'
      OnExecute = FlipVertActionExecute
      OnUpdate = RotateActionUpdate
    end
    object FullScreenAction: TAction
      Category = 'CommandI'
      Caption = #20840#30011#38754#34920#31034'(&U)'
      OnExecute = FullScreenActionExecute
      OnUpdate = FullScreenActionUpdate
    end
    object GifViewerAction: TAction
      Category = 'CommandI'
      Caption = 'GIF'#12499#12517#12450#12540
      OnExecute = GifViewerActionExecute
      OnUpdate = SubViewerActionUpdate
    end
    object GrayScaleAction: TAction
      Category = 'CommandI'
      Caption = #12464#12524#12540#12473#12465#12540#12523#34920#31034'(&G)'
      OnExecute = GrayScaleActionExecute
      OnUpdate = GrayScaleActionUpdate
    end
    object HistogramAction: TAction
      Category = 'CommandI'
      Caption = #12498#12473#12488#12464#12521#12512'(&H)'
      OnExecute = HistogramActionExecute
      OnUpdate = HistogramActionUpdate
    end
    object JumpIndexAction: TAction
      Category = 'CommandI'
      OnExecute = JumpIndexActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object LoupeAction: TAction
      Category = 'CommandI'
      Caption = #12523#12540#12506'(&M)'
      OnExecute = LoupeActionExecute
      OnUpdate = LoupeActionUpdate
    end
    object NextFileAction: TAction
      Category = 'CommandI'
      OnExecute = NextFileActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object NextPageAction: TAction
      Category = 'CommandI'
      OnExecute = NextPageActionExecute
      OnUpdate = PageIActionUpdate
    end
    object PageBindAction: TAction
      Category = 'CommandI'
      OnExecute = PageBindActionExecute
      OnUpdate = PageBindActionUpdate
    end
    object PageUpIAction: TAction
      Category = 'CommandI'
      OnExecute = PageUpIActionExecute
      OnUpdate = PageIxActionUpdate
    end
    object PageDownIAction: TAction
      Category = 'CommandI'
      OnExecute = PageDownIActionExecute
      OnUpdate = PageIxActionUpdate
    end
    object PrevFileAction: TAction
      Category = 'CommandI'
      OnExecute = PrevFileActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object PrevPageAction: TAction
      Category = 'CommandI'
      OnExecute = PrevPageActionExecute
      OnUpdate = PageIActionUpdate
    end
    object PrintAction: TAction
      Category = 'CommandI'
      Caption = #21360#21047'(&P)...'
      OnExecute = PrintActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object RotateLeftAction: TAction
      Category = 'CommandI'
      OnExecute = RotateLeftActionExecute
      OnUpdate = RotateActionUpdate
    end
    object RotateRightAction: TAction
      Category = 'CommandI'
      OnExecute = RotateRightActionExecute
      OnUpdate = RotateActionUpdate
    end
    object ScrollDownIAction: TAction
      Category = 'CommandI'
      OnExecute = ScrollDownIActionExecute
      OnUpdate = ScrollActionUpdate
    end
    object ScrollLeftAction: TAction
      Category = 'CommandI'
      OnExecute = ScrollLeftActionExecute
      OnUpdate = ScrollActionUpdate
    end
    object ScrollRightAction: TAction
      Category = 'CommandI'
      OnExecute = ScrollRightActionExecute
      OnUpdate = ScrollActionUpdate
    end
    object AboutNyanFiAction: TAction
      Category = 'Command'
      Caption = #12496#12540#12472#12519#12531#24773#22577'(&A)'
      OnExecute = AboutNyanFiActionExecute
    end
    object ScrollUpIAction: TAction
      Category = 'CommandI'
      OnExecute = ScrollUpIActionExecute
      OnUpdate = ScrollActionUpdate
    end
    object SendToWorkListAction: TAction
      Category = 'CommandI'
      OnExecute = SendToWorkListActionExecute
      OnUpdate = SendToWorkListActionUpdate
    end
    object ShowSeekBarAction: TAction
      Category = 'CommandI'
      Caption = #12471#12540#12463#12496#12540#12434#34920#31034
      OnExecute = ShowSeekBarActionExecute
      OnUpdate = ShowSeekBarActionUpdate
    end
    object ShowGridAction: TAction
      Category = 'CommandI'
      Caption = #30011#20687#20998#21106#12464#12522#12483#12489#12398#34920#31034'(&L)'
      OnExecute = ShowGridActionExecute
      OnUpdate = ShowGridActionUpdate
    end
    object SidebarAction: TAction
      Category = 'CommandI'
      Caption = #12469#12452#12489#12496#12540#12398#34920#31034'(&B)'
      OnExecute = SidebarActionExecute
      OnUpdate = SidebarActionUpdate
    end
    object SortDlgIAction: TAction
      Category = 'CommandI'
      OnExecute = SortDlgIActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object ThumbnailAction: TAction
      Category = 'CommandI'
      Caption = #12469#12512#12493#12452#12523#12398#34920#31034'(&N)'
      OnExecute = ThumbnailActionExecute
      OnUpdate = ThumbnailActionUpdate
    end
    object ThumbnailExAction: TAction
      Category = 'CommandI'
      Caption = #12469#12512#12493#12452#12523#12398#20840#38754#34920#31034'(&X)'
      OnExecute = ThumbnailExActionExecute
      OnUpdate = ThumbnailExActionUpdate
    end
    object TopFileAction: TAction
      Category = 'CommandI'
      OnExecute = TopFileActionExecute
      OnUpdate = MenuIActionUpdate
    end
    object WarnHighlightAction: TAction
      Category = 'CommandI'
      Caption = #30333#39131#12403#35686#21578'(&W)'
      OnExecute = WarnHighlightActionExecute
      OnUpdate = WarnHighlightActionUpdate
    end
    object WorkListIAction: TAction
      Category = 'CommandI'
      OnExecute = WorkListIActionExecute
      OnUpdate = WorkListIActionUpdate
    end
    object ZoomInIAction: TAction
      Category = 'CommandI'
      Caption = #12474#12540#12512#12452#12531'(&I)'
      OnExecute = ZoomInIActionExecute
      OnUpdate = ZoomIActionUpdate
    end
    object AddTabAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12434#36861#21152'(&A)'
      OnExecute = AddTabActionExecute
      OnUpdate = AddTabActionUpdate
    end
    object AddTagAction: TAction
      Category = 'Command'
      Caption = #12479#12464#12398#36861#21152'...'
      OnExecute = AddTagActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object AlphaBlendAction: TAction
      Category = 'Command'
      OnExecute = AlphaBlendActionExecute
    end
    object AppListAction: TAction
      Category = 'Command'
      Caption = #12450#12503#12522#12465#12540#12471#12519#12531#19968#35239'(&A)...'
      HelpContext = 76
      OnExecute = AppListActionExecute
      OnUpdate = AppListActionUpdate
    end
    object BackDirHistAction: TAction
      Category = 'Command'
      OnExecute = BackDirHistActionExecute
      OnUpdate = BackDirHistActionUpdate
    end
    object BackupAction: TAction
      Category = 'Command'
      Caption = #12496#12483#12463#12450#12483#12503'...'
      HelpContext = 71
      OnExecute = BackupActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object BgImgModeAction: TAction
      Category = 'Command'
      OnExecute = BgImgModeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object BinaryEditAction: TAction
      Category = 'Command'
      OnExecute = BinaryEditActionExecute
      OnUpdate = MenuFVActionUpdate
    end
    object BorderLeftAction: TAction
      Category = 'Command'
      OnExecute = BorderLeftActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object GrepStartAction: TAction
      Category = 'Grep'
      Caption = #38283#22987
      OnExecute = GrepStartActionExecute
      OnUpdate = GrepStartActionUpdate
    end
    object ZoomOutIAction: TAction
      Category = 'CommandI'
      Caption = #12474#12540#12512#12450#12454#12488'(&O)'
      OnExecute = ZoomOutIActionExecute
      OnUpdate = ZoomIActionUpdate
    end
    object GrepAbortAction: TAction
      Category = 'Grep'
      Caption = #20013#26029
      OnExecute = GrepAbortActionExecute
      OnUpdate = GrepAbortActionUpdate
    end
    object BorderRightAction: TAction
      Category = 'Command'
      OnExecute = BorderRightActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CalcDirSizeAction: TAction
      Category = 'Command'
      OnExecute = CalcDirSizeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CalcDirSizeAllAction: TAction
      Category = 'Command'
      OnExecute = CalcDirSizeAllActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CalculatorAction: TAction
      Category = 'Command'
      Caption = #38651#21331
      HelpContext = 87
      OnExecute = CalculatorActionExecute
    end
    object CancelAllTaskAction: TAction
      Category = 'Command'
      OnExecute = CancelAllTaskActionExecute
      OnUpdate = CancelAllTaskActionUpdate
    end
    object ChangeDirAction: TAction
      Category = 'Command'
      OnExecute = ChangeDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ChangeDriveAction: TAction
      Category = 'Command'
      OnExecute = ChangeDriveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ChangeOppDirAction: TAction
      Category = 'Command'
      OnExecute = ChangeOppDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ChangeOppRegDirAction: TAction
      Category = 'Command'
      OnExecute = ChangeOppRegDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ChangeRegDirAction: TAction
      Category = 'Command'
      OnExecute = ChangeRegDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CheckUpdateAction: TAction
      Category = 'Command'
      Caption = #26356#26032#12398#30906#35469'(&U)'
      OnExecute = CheckUpdateActionExecute
      OnUpdate = CheckUpdateActionUpdate
    end
    object ClearAllAction: TAction
      Category = 'Command'
      OnExecute = ClearAllActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ClearLogAction: TAction
      Category = 'Command'
      Caption = #12525#12464#12434#12463#12522#12450'(&L)'
      OnExecute = ClearLogActionExecute
      OnUpdate = LogActionUpdate
    end
    object ClearMarkAction: TAction
      Category = 'Command'
      OnExecute = ClearMarkActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object ClearMaskAction: TAction
      Category = 'Command'
      OnExecute = ClearMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CloneAction: TAction
      Category = 'Command'
      OnExecute = CloneActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CloneToCurrAction: TAction
      Category = 'Command'
      OnExecute = CloneToCurrActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CmdFileListAction: TAction
      Category = 'Command'
      Caption = #12467#12510#12531#12489#12501#12449#12452#12523#19968#35239
      HelpContext = 67
      OnExecute = CmdFileListActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object CommandPromptAction: TAction
      Category = 'Command'
      Caption = #12467#12510#12531#12489#12503#12525#12531#12503#12488'(&C)'
      OnExecute = CommandPromptActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CompareDlgAction: TAction
      Category = 'Command'
      Caption = #21516#21517#12501#12449#12452#12523#12398#27604#36611'...'
      OnExecute = CompareDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CompareHashAction: TAction
      Category = 'Command'
      OnExecute = CompareHashActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CompleteDeleteAction: TAction
      Category = 'Command'
      OnExecute = CompleteDeleteActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CompressDirAction: TAction
      Category = 'Command'
      OnExecute = CompressDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ConvertDoc2TxtAction: TAction
      Category = 'Command'
      OnExecute = ConvertDoc2TxtActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ContextMenuAction: TAction
      Category = 'Command'
      OnExecute = ContextMenuActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ConvertHtm2TxtAction: TAction
      Category = 'Command'
      Caption = 'HTML'#8594#12486#12461#12473#12488#22793#25563
      OnExecute = ConvertHtm2TxtActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ConvertImageAction: TAction
      Category = 'Command'
      Caption = #30011#20687#12501#12449#12452#12523#12398#22793#25563'...'
      HelpContext = 59
      OnExecute = ConvertImageActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ConvertTextEncAction: TAction
      Category = 'Command'
      Caption = #25991#23383#12467#12540#12489#12398#22793#25563'...'
      OnExecute = ConvertTextEncActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyAction: TAction
      Category = 'Command'
      OnExecute = CopyActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyCmdNameAction: TAction
      Category = 'Command'
      OnExecute = CopyCmdNameActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyDirAction: TAction
      Category = 'Command'
      OnExecute = CopyDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyFileInfoAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#24773#22577#12434#12467#12500#12540
      OnExecute = CopyFileInfoActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object CopyFileNameAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#21517#12434#12467#12500#12540'(&C)'
      OnExecute = CopyFileNameActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyToAction: TAction
      Category = 'Command'
      OnExecute = CopyToActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CopyToClipAction: TAction
      Category = 'Command'
      OnExecute = CopyToClipActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CountLinesAction: TAction
      Category = 'Command'
      OnExecute = CountLinesActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateDirAction: TAction
      Category = 'Command'
      OnExecute = CreateDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateDirsDlgAction: TAction
      Category = 'Command'
      Caption = #12487#12451#12524#12463#12488#12522#19968#25324#20316#25104'...'
      HelpContext = 79
      OnExecute = CreateDirsDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateHardLinkAction: TAction
      Category = 'Command'
      OnExecute = CreateHardLinkActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateJunctionAction: TAction
      Category = 'Command'
      OnExecute = CreateJunctionActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateShortcutAction: TAction
      Category = 'Command'
      Caption = #12471#12519#12540#12488#12459#12483#12488#12398#20316#25104'(&S)'
      OnExecute = CreateShortcutActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateSymLinkAction: TAction
      Category = 'Command'
      OnExecute = CreateSymLinkActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CreateTestFileAction: TAction
      Category = 'Command'
      OnExecute = CreateTestFileActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CsrDirToOppAction: TAction
      Category = 'Command'
      OnExecute = CsrDirToOppActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CursorDownAction: TAction
      Category = 'Command'
      OnExecute = CursorDownActionExecute
    end
    object CursorDownSelAction: TAction
      Category = 'Command'
      OnExecute = CursorDownSelActionExecute
    end
    object CursorEndAction: TAction
      Category = 'Command'
      OnExecute = CursorEndActionExecute
    end
    object CursorEndSelAction: TAction
      Category = 'Command'
      OnExecute = CursorEndSelActionExecute
    end
    object CursorTopAction: TAction
      Category = 'Command'
      OnExecute = CursorTopActionExecute
    end
    object CursorTopSelAction: TAction
      Category = 'Command'
      OnExecute = CursorTopSelActionExecute
    end
    object CursorUpAction: TAction
      Category = 'Command'
      OnExecute = CursorUpActionExecute
    end
    object CursorUpSelAction: TAction
      Category = 'Command'
      OnExecute = CursorUpSelActionExecute
    end
    object CurrToOppAction: TAction
      Category = 'Command'
      OnExecute = CurrToOppActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CurrFromOppAction: TAction
      Category = 'Command'
      OnExecute = CurrFromOppActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object CutToClipAction: TAction
      Category = 'Command'
      OnExecute = CutToClipActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DateSelectAction: TAction
      Category = 'Command'
      OnExecute = DateSelectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DebugCmdFileAction: TAction
      Category = 'Command'
      OnExecute = DebugCmdFileActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object DeleteAction: TAction
      Category = 'Command'
      OnExecute = DeleteActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DeleteADSAction: TAction
      Category = 'Command'
      OnExecute = DeleteADSActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DelTagAction: TAction
      Category = 'Command'
      Caption = #12479#12464#12398#21066#38500
      OnExecute = DelTagActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object DelJpgExifAction: TAction
      Category = 'Command'
      Caption = 'Exif'#24773#22577#12398#21066#38500
      OnExecute = DelJpgExifActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DelSelMaskAction: TAction
      Category = 'Command'
      OnExecute = DelSelMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DelTabAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12434#21066#38500'(&D)'
      OnExecute = DelTabActionExecute
      OnUpdate = ChgTabActionUpdate
    end
    object DiffDirAction: TAction
      Category = 'Command'
      Caption = #12487#12451#12524#12463#12488#12522#12398#27604#36611'...'
      OnExecute = DiffDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DirHistoryAction: TAction
      Category = 'Command'
      Caption = #12487#12451#12524#12463#12488#12522#23653#27508'(&H)...'
      HelpContext = 57
      OnExecute = DirHistoryActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DirStackAction: TAction
      Category = 'Command'
      Caption = #12487#12451#12524#12463#12488#12522#12539#12473#12479#12483#12463'(&Q)...'
      OnExecute = DirStackActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DistributionDlgAction: TAction
      Category = 'Command'
      Caption = #25391#12426#20998#12369'...'
      HelpContext = 77
      OnExecute = DistributionDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DotNyanDlgAction: TAction
      Category = 'Command'
      Caption = '.nyanfi '#12501#12449#12452#12523#12398#35373#23450'(&N)'
      HelpContext = 75
      OnExecute = DotNyanDlgActionExecute
      OnUpdate = DotNyanDlgActionUpdate
    end
    object DriveGraphAction: TAction
      Category = 'Command'
      Caption = #12489#12521#12452#12502#20351#29992#29575#25512#31227
      HelpContext = 86
      OnExecute = DriveGraphActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DriveListAction: TAction
      Category = 'Command'
      Caption = #12489#12521#12452#12502#19968#35239'(&D)...'
      HelpContext = 64
      OnExecute = DriveListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object DuplicateAction: TAction
      Category = 'Command'
      Caption = #20108#37325#36215#21205'(&D)'
      OnExecute = DuplicateActionExecute
      OnUpdate = DuplicateActionUpdate
    end
    object EditHighlightAction: TAction
      Category = 'Command'
      Caption = #27083#25991#24375#35519#34920#31034#23450#32681#12501#12449#12452#12523#12398#32232#38598
      HelpContext = 84
      OnExecute = EditHighlightActionExecute
      OnUpdate = EditHighlightActionUpdate
    end
    object EditHistoryAction: TAction
      Category = 'Command'
      Caption = #26368#36817#32232#38598#12375#12383#12501#12449#12452#12523#19968#35239'(&L)...'
      HelpContext = 58
      OnExecute = EditHistoryActionExecute
      OnUpdate = MenuFVActionUpdate
    end
    object EditIniFileAction: TAction
      Category = 'Command'
      Caption = 'INI'#12501#12449#12452#12523#12398#32232#38598'(&E)'
      OnExecute = EditIniFileActionExecute
      OnUpdate = EditIniFileActionUpdate
    end
    object EjectAction: TAction
      Category = 'Command'
      Caption = 'CD/DVD'#12489#12521#12452#12502' '#12398#12488#12524#12452#12434#38283#12367'(&T)'
      OnExecute = EjectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object EjectDriveAction: TAction
      Category = 'Command'
      OnExecute = EjectDriveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object EmptyTrashAction: TAction
      Category = 'Command'
      OnExecute = EmptyTrashActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object EqualListWidthAction: TAction
      Category = 'Command'
      OnExecute = EqualListWidthActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExeCommandLineAction: TAction
      Category = 'Command'
      Caption = #12467#12510#12531#12489#12521#12452#12531#12398#23455#34892'(&X)...'
      HelpContext = 62
      OnExecute = ExeCommandLineActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExeCommandsAction: TAction
      Category = 'Command'
      OnExecute = ExeCommandsActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object ExeExtMenuAction: TAction
      Category = 'Command'
      OnExecute = ExeExtMenuActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExeExtToolAction: TAction
      Category = 'Command'
      OnExecute = ExeExtToolActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExeMenuFileAction: TAction
      Category = 'Command'
      OnExecute = ExeMenuFileActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object ExeToolBtnAction: TAction
      Category = 'Command'
      OnExecute = ExeToolBtnActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object ExitAction: TAction
      Category = 'Command'
      Caption = #32066#20102'(&X)'
      OnExecute = ExitActionExecute
      OnUpdate = ExitActionUpdate
    end
    object ExitDuplAction: TAction
      Category = 'Command'
      OnExecute = ExitDuplActionExecute
      OnUpdate = ExitDuplActionUpdate
    end
    object ExPopupMenuAction: TAction
      Category = 'Command'
      OnExecute = ExPopupMenuActionExecute
    end
    object ExtractChmSrcAction: TAction
      Category = 'Command'
      OnExecute = ExtractChmSrcActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExtractGifBmpAction: TAction
      Category = 'Command'
      OnExecute = ExtractGifBmpActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExtractIconAction: TAction
      Category = 'Command'
      Caption = #12450#12452#12467#12531#12434#25277#20986
      OnExecute = ExtractIconActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ExtractMp3ImgAction: TAction
      Category = 'Command'
      Caption = 'MP3'#12363#12425#22475#12417#36796#12415#30011#20687#12434#25277#20986
      OnExecute = ExtractMp3ImgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FileEditAction: TAction
      Category = 'Command'
      OnExecute = FileEditActionExecute
    end
    object FileExtListAction: TAction
      Category = 'Command'
      Caption = #25313#24373#23376#21029#19968#35239'(&X)...'
      HelpContext = 72
      OnExecute = FileExtListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FileListOnlyAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#12522#12473#12488#12398#12415#12434#34920#31034
      OnExecute = FileListOnlyActionExecute
      OnUpdate = FileListOnlyActionUpdate
    end
    object FileRunAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#21517#12434#25351#23450#12375#12390#23455#34892'(&F)...'
      OnExecute = FileRunActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FilterAction: TAction
      Category = 'Command'
      OnExecute = FilterActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FindDirDlgAction: TAction
      Category = 'Command'
      Caption = #12487#12451#12524#12463#12488#12522#21517#26908#32034'(&D)...'
      HelpContext = 53
      OnExecute = FindDirDlgActionExecute
      OnUpdate = FindDlgActionUpdate
    end
    object FindDuplDlgAction: TAction
      Category = 'Command'
      Caption = #37325#35079#12501#12449#12452#12523#12398#26908#32034'...'
      OnExecute = FindDuplDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FindFileDlgAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#21517#26908#32034'(&F)...'
      HelpContext = 52
      OnExecute = FindFileDlgActionExecute
      OnUpdate = FindDlgActionUpdate
    end
    object FindFileDirDlgAction: TAction
      Category = 'Command'
      OnExecute = FindFileDirDlgActionExecute
      OnUpdate = FindDlgActionUpdate
    end
    object FindMarkAction: TAction
      Category = 'Command'
      OnExecute = FindMarkActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FindTagAction: TAction
      Category = 'Command'
      Caption = #12479#12464#26908#32034'(&T)...'
      OnExecute = FindTagActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FindTagNameAction: TAction
      Category = 'Command'
      OnExecute = FindTagNameActionExecute
      OnUpdate = MenuFVActionUpdate
    end
    object FixedLenAction: TAction
      Category = 'Command'
      Caption = 'CSV/TSV'#12434#22266#23450#38263#34920#31034
      OnExecute = FixedLenActionExecute
      OnUpdate = FixedLenActionUpdate
    end
    object FixTabPathAction: TAction
      Category = 'Command'
      OnExecute = FixTabPathActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ForwardDirHistAction: TAction
      Category = 'Command'
      OnExecute = ForwardDirHistActionExecute
      OnUpdate = ForwardDirHistActionUpdate
    end
    object FTPChmodAction: TAction
      Category = 'Command'
      Caption = #12497#12540#12511#12483#12471#12519#12531#12398#35373#23450'...'
      OnExecute = FTPChmodActionExecute
      OnUpdate = FTPChmodActionUpdate
    end
    object FTPConnectAction: TAction
      Category = 'Command'
      Caption = 'FTP'#25509#32154'...'
      HelpContext = 80
      OnExecute = FTPConnectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FTPDisconnectAction: TAction
      Category = 'Command'
      Caption = 'FTP'#20999#26029
      OnExecute = FTPDisconnectActionExecute
      OnUpdate = FTPDisconnectActionUpdate
    end
    object GetHashAction: TAction
      Category = 'Command'
      OnExecute = GetHashActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object GrepAction: TAction
      Category = 'Command'
      Caption = #25991#23383#21015#26908#32034'(&Grep)...'
      HelpContext = 56
      OnExecute = GrepActionExecute
      OnUpdate = GrepActionUpdate
    end
    object HelpContentsAction: TAction
      Category = 'Command'
      Caption = #12504#12523#12503#12398#30446#27425'(&C)'
      OnExecute = HelpContentsActionExecute
    end
    object HideSizeTimeAction: TAction
      Category = 'Command'
      Caption = #12469#12452#12474#12392#26356#26032#26085#26178#12434#38560#12377
      OnExecute = HideSizeTimeActionExecute
      OnUpdate = HideSizeTimeActionUpdate
    end
    object HomeWorkListAction: TAction
      Category = 'Command'
      Caption = #12507#12540#12512#12527#12540#12463#12522#12473#12488#12434#38283#12367'(&M)'
      OnExecute = HomeWorkListActionExecute
      OnUpdate = HomeWorkListActionUpdate
    end
    object HtmlToTextAction: TAction
      Category = 'Command'
      Caption = 'HTML'#8594#12486#12461#12473#12488#22793#25563#34920#31034'(&H)'
      OnExecute = HtmlToTextActionExecute
      OnUpdate = HtmlToTextActionUpdate
    end
    object ImageViewerAction: TAction
      Category = 'Command'
      HelpContext = 7
      OnExecute = ImageViewerActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object IncSearchAction: TAction
      Category = 'Command'
      HelpContext = 50
      OnExecute = IncSearchActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object InputCommandsAction: TAction
      Category = 'Command'
      OnExecute = InputCommandsActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object InputDirAction: TAction
      Category = 'Command'
      OnExecute = InputDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object InputPathMaskAction: TAction
      Category = 'Command'
      OnExecute = InputPathMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object InsSeparatorAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12395#12475#12497#12524#12540#12479#12434#25407#20837
      OnExecute = InsSeparatorActionExecute
      OnUpdate = WorkItemActionUpdate
    end
    object ItemTmpDownAction: TAction
      Category = 'Command'
      Caption = 'ItemTmpDownAction'
      OnExecute = ItemTmpDownActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ItemTmpMoveAction: TAction
      Category = 'Command'
      OnExecute = ItemTmpMoveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ItemTmpUpAction: TAction
      Category = 'Command'
      Caption = 'ItemTmpUpAction'
      OnExecute = ItemTmpUpActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object JoinTextAction: TAction
      Category = 'Command'
      Caption = #12486#12461#12473#12488#12501#12449#12452#12523#12398#32080#21512'...'
      HelpContext = 78
      OnExecute = JoinTextActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object JumpToAction: TAction
      Category = 'Command'
      OnExecute = JumpToActionExecute
      OnUpdate = JumpToActionUpdate
    end
    object KeyListAction: TAction
      Category = 'Command'
      Caption = #12461#12540#21106#12426#24403#12390#19968#35239'(&K)...'
      HelpContext = 55
      OnExecute = KeyListActionExecute
      OnUpdate = KeyListActionUpdate
    end
    object LibraryAction: TAction
      Category = 'Command'
      Caption = #12521#12452#12502#12521#12522
      OnExecute = LibraryActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object LinkToOppAction: TAction
      Category = 'Command'
      OnExecute = LinkToOppActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListArchiveAction: TAction
      Category = 'Command'
      OnExecute = ListArchiveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListClipboardAction: TAction
      Category = 'Command'
      OnExecute = ListClipboardActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListDurationAction: TAction
      Category = 'Command'
      OnExecute = ListDurationActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListExpFuncAction: TAction
      Category = 'Command'
      OnExecute = ListExpFuncActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListFileNameAction: TAction
      Category = 'Command'
      OnExecute = ListFileNameActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object FindHardLinkAction: TAction
      Category = 'Command'
      OnExecute = FindHardLinkActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListLogAction: TAction
      Category = 'Command'
      OnExecute = ListLogActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object ListNyanFiAction: TAction
      Category = 'Command'
      OnExecute = ListNyanFiActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListTailAction: TAction
      Category = 'Command'
      OnExecute = ListTailActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListTextAction: TAction
      Category = 'Command'
      OnExecute = ListTextActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ListTreeAction: TAction
      Category = 'Command'
      OnExecute = ListTreeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object LoadBgImageAction: TAction
      Category = 'Command'
      OnExecute = LoadBgImageActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object LoadResultListAction: TAction
      Category = 'Command'
      Caption = #32080#26524#12522#12473#12488#12434#12501#12449#12452#12523#12363#12425#35501#12415#36796#12416'...'
      OnExecute = LoadResultListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object LoadTabGroupAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12464#12523#12540#12503#12434#12501#12449#12452#12523#12363#12425#35501#12415#36796#12416'...'
      OnExecute = LoadTabGroupActionExecute
      OnUpdate = TabGroupActionUpdate
    end
    object LoadWorkListAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12434#12501#12449#12452#12523#12363#12425#35501#12415#36796#12416'(&L)...'
      OnExecute = LoadWorkListActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object LockTextPreviewAction: TAction
      Category = 'Command'
      Caption = #12486#12461#12473#12488#12503#12524#12499#12517#12540#12434#12525#12483#12463'(&L)'
      OnExecute = LockTextPreviewActionExecute
      OnUpdate = LockTextPreviewActionUpdate
    end
    object LogFileInfoAction: TAction
      Category = 'Command'
      OnExecute = LogFileInfoActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MarkAction: TAction
      Category = 'Command'
      OnExecute = MarkActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object MarkListAction: TAction
      Category = 'Command'
      Caption = #26654#12510#12540#12463#19968#35239'(&B)...'
      HelpContext = 70
      OnExecute = MarkListActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object MarkMaskAction: TAction
      Category = 'Command'
      OnExecute = MarkMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MaskFindAction: TAction
      Category = 'Command'
      OnExecute = MaskFindActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MaskSelectAction: TAction
      Category = 'Command'
      Caption = #12510#12473#12463#36984#25246'(&M)...'
      OnExecute = MaskSelectActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object MatchSelectAction: TAction
      Category = 'Command'
      OnExecute = MatchSelectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MenuBarAction: TAction
      Category = 'Command'
      Caption = #12513#12491#12517#12540#12496#12540#12434#34920#31034'(&B)'
      OnExecute = MenuBarActionExecute
      OnUpdate = MenuBarActionUpdate
    end
    object MonitorOffAction: TAction
      Category = 'Command'
      OnExecute = MonitorOffActionExecute
    end
    object MoveAction: TAction
      Category = 'Command'
      OnExecute = MoveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MoveTabAction: TAction
      Category = 'Command'
      OnExecute = MoveTabActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MoveToAction: TAction
      Category = 'Command'
      OnExecute = MoveToActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object MuteVolumeAction: TAction
      Category = 'Command'
      OnExecute = MuteVolumeActionExecute
      OnUpdate = MuteVolumeActionUpdate
    end
    object NameFromClipAction: TAction
      Category = 'Command'
      OnExecute = NameFromClipActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NameToLowerAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#21517#12398#23567#25991#23383#21270
      OnExecute = NameToLowerActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NameToUpperAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#21517#12398#22823#25991#23383#21270
      OnExecute = NameToUpperActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NetConnectAction: TAction
      Category = 'Command'
      Caption = #12493#12483#12488#12527#12540#12463#12489#12521#12452#12502#12398#21106#12426#24403#12390'(&N)...'
      OnExecute = NetConnectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NetDisconnectAction: TAction
      Category = 'Command'
      Caption = #12493#12483#12488#12527#12540#12463#12489#12521#12452#12502#12398#20999#26029'(&D)...'
      OnExecute = NetDisconnectActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NewFileAction: TAction
      Category = 'Command'
      Caption = #26032#35215#12501#12449#12452#12523'...'
      OnExecute = NewFileActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NewTextFileAction: TAction
      Category = 'Command'
      Caption = #26032#35215#12486#12461#12473#12488#12501#12449#12452#12523'(&E)...'
      OnExecute = NewTextFileActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NewWorkListAction: TAction
      Category = 'Command'
      Caption = #26032#35215#12527#12540#12463#12522#12473#12488'(&N)'
      OnExecute = NewWorkListActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object NextDriveAction: TAction
      Category = 'Command'
      OnExecute = NextDriveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object NextMarkAction: TAction
      Category = 'Command'
      OnExecute = NextMarkActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object NextNyanFiAction: TAction
      Category = 'Command'
      OnExecute = NextNyanFiActionExecute
      OnUpdate = ChgNyanFiActionUpdate
    end
    object NextSameNameAction: TAction
      Category = 'Command'
      OnExecute = NextSameNameActionExecute
    end
    object NextTabAction: TAction
      Category = 'Command'
      OnExecute = NextTabActionExecute
      OnUpdate = ChgTabActionUpdate
    end
    object OpenADSAction: TAction
      Category = 'Command'
      OnExecute = OpenADSActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object OpenByAppAction: TAction
      Category = 'Command'
      OnExecute = OpenByAppActionExecute
      OnUpdate = MenuFIGActionUpdate
    end
    object OpenByExpAction: TAction
      Category = 'Command'
      Caption = #12456#12463#12473#12503#12525#12540#12521#12391#38283#12367'(&O)'
      OnExecute = OpenByExpActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object OpenByWinAction: TAction
      Category = 'Command'
      OnExecute = OpenByWinActionExecute
      OnUpdate = MenuFIGActionUpdate
    end
    object OpenStandardAction: TAction
      Category = 'Command'
      OnExecute = OpenStandardActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object OpenTrashAction: TAction
      Category = 'Command'
      Caption = #12372#12415#31665#12434#38283#12367
      OnExecute = OpenTrashActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object OptionDlgAction: TAction
      Category = 'Command'
      Caption = #12458#12503#12471#12519#12531#35373#23450'(&O)...'
      HelpContext = 8
      OnExecute = OptionDlgActionExecute
      OnUpdate = OptionDlgActionUpdate
    end
    object PackAction: TAction
      Category = 'Command'
      OnExecute = PackActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PackToCurrAction: TAction
      Category = 'Command'
      OnExecute = PackToCurrActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PageDownAction: TAction
      Category = 'Command'
      OnExecute = PageDownActionExecute
    end
    object PageDownSelAction: TAction
      Category = 'Command'
      OnExecute = PageDownSelActionExecute
    end
    object PageUpAction: TAction
      Category = 'Command'
      OnExecute = PageUpActionExecute
    end
    object PageUpSelAction: TAction
      Category = 'Command'
      OnExecute = PageUpSelActionExecute
    end
    object PasteAction: TAction
      Category = 'Command'
      OnExecute = PasteActionExecute
      OnUpdate = PasteActionUpdate
    end
    object PathMaskDlgAction: TAction
      Category = 'Command'
      Caption = #12497#12473#12510#12473#12463'(&M)...'
      OnExecute = PathMaskDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PauseAllTaskAction: TAction
      Category = 'Command'
      OnExecute = PauseAllTaskActionExecute
      OnUpdate = PauseAllTaskActionUpdate
    end
    object PlayListAction: TAction
      Category = 'Command'
      OnExecute = PlayListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PopDirAction: TAction
      Category = 'Command'
      OnExecute = PopDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PopupMainMenuAction: TAction
      Category = 'Command'
      OnExecute = PopupMainMenuActionExecute
    end
    object PopupTabAction: TAction
      Category = 'Command'
      OnExecute = PopupTabActionExecute
      OnUpdate = PopupTabActionUpdate
    end
    object PowerOffAction: TAction
      Category = 'Command'
      OnExecute = PowerOffActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PowerShellAction: TAction
      Category = 'Command'
      Caption = 'PowerShell'
      OnExecute = PowerShellActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PrevDriveAction: TAction
      Category = 'Command'
      OnExecute = PrevDriveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object PrevMarkAction: TAction
      Category = 'Command'
      OnExecute = PrevMarkActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object PrevNyanFiAction: TAction
      Category = 'Command'
      Caption = 'PrevNyanFiAction'
      OnExecute = PrevNyanFiActionExecute
      OnUpdate = ChgNyanFiActionUpdate
    end
    object PrevTabAction: TAction
      Category = 'Command'
      OnExecute = PrevTabActionExecute
      OnUpdate = ChgTabActionUpdate
    end
    object PropertyDlgAction: TAction
      Category = 'Command'
      Caption = #12503#12525#12497#12486#12451'(&R)'
      OnExecute = PropertyDlgActionExecute
      OnUpdate = PropertyDlgActionUpdate
    end
    object PushDirAction: TAction
      Category = 'Command'
      OnExecute = PushDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RebootAction: TAction
      Category = 'Command'
      OnExecute = RebootActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RecentListAction: TAction
      Category = 'Command'
      Caption = #26368#36817#20351#12387#12383#12501#12449#12452#12523#19968#35239'...'
      OnExecute = RecentListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RegDirDlgAction: TAction
      Category = 'Command'
      Caption = #30331#37682#12487#12451#12524#12463#12488#12522'(&R)...'
      HelpContext = 54
      OnExecute = RegDirDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RegDirPopupAction: TAction
      Category = 'Command'
      OnExecute = RegDirPopupActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RegExCheckerAction: TAction
      Category = 'Command'
      Caption = #27491#35215#34920#29694#12481#12455#12483#12459#12540
      HelpContext = 85
      OnExecute = RegExCheckerActionExecute
    end
    object RegSyncDlgAction: TAction
      Category = 'Command'
      Caption = #21516#26399#12467#12500#12540#12398#35373#23450'(&Y)...'
      HelpContext = 74
      OnExecute = RegSyncDlgActionExecute
      OnUpdate = RegSyncDlgActionUpdate
    end
    object ReloadListAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#12522#12473#12488#12434#26368#26032#12398#24773#22577#12395'(&N)'
      OnExecute = ReloadListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RenameDlgAction: TAction
      Category = 'Command'
      HelpContext = 51
      OnExecute = RenameDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object RestartAction: TAction
      Category = 'Command'
      Caption = #20877#36215#21205'(&B)'
      OnExecute = RestartActionExecute
      OnUpdate = RestartActionUpdate
    end
    object ReturnListAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#12522#12473#12488#34920#31034#12395#25147#12427'(&R)'
      OnExecute = ReturnListActionExecute
      OnUpdate = ReturnListActionUpdate
    end
    object SaveAsResultListAction: TAction
      Category = 'Command'
      Caption = #32080#26524#12522#12473#12488#12395#21517#21069#12434#20184#12369#12390#20445#23384'...'
      OnExecute = SaveAsResultListActionExecute
      OnUpdate = SaveAsResultListActionUpdate
    end
    object SaveAsTabGroupAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12464#12523#12540#12503#12395#21517#21069#12434#20184#12369#12390#20445#23384'...'
      OnExecute = SaveAsTabGroupActionExecute
      OnUpdate = TabGroupActionUpdate
    end
    object SaveAsWorkListAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12395#21517#21069#12434#20184#12369#12390#20445#23384'(&A)...'
      OnExecute = SaveAsWorkListActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SaveTabGroupAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12464#12523#12540#12503#12434#19978#26360#12365#20445#23384
      OnExecute = SaveTabGroupActionExecute
      OnUpdate = TabGroupActionUpdate
    end
    object SaveWorkListAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12434#19978#26360#12365#20445#23384'(&S)'
      OnExecute = SaveWorkListActionExecute
      OnUpdate = SaveWorkListActionUpdate
    end
    object ScrollDownAction: TAction
      Category = 'Command'
      OnExecute = ScrollDownActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ScrollDownLogAction: TAction
      Category = 'Command'
      OnExecute = ScrollDownLogActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ScrollDownTextAction: TAction
      Category = 'Command'
      OnExecute = ScrollDownTextActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ScrollUpAction: TAction
      Category = 'Command'
      OnExecute = ScrollUpActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ScrollUpLogAction: TAction
      Category = 'Command'
      OnExecute = ScrollUpLogActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ScrollUpTextAction: TAction
      Category = 'Command'
      OnExecute = ScrollUpTextActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelAllFileAction: TAction
      Category = 'Command'
      Caption = #12377#12409#12390#12398#12501#12449#12452#12523#12434#36984#25246'(&A)'
      OnExecute = SelAllFileActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelAllItemAction: TAction
      Category = 'Command'
      Caption = #12377#12409#12390#12398#12501#12449#12452#12523#12392#12487#12451#12524#12463#12488#12522#12434#36984#25246
      OnExecute = SelAllItemActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelectAction: TAction
      Category = 'Command'
      OnExecute = SelectActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelectUpAction: TAction
      Category = 'Command'
      OnExecute = SelectUpActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelEmptyDirAction: TAction
      Category = 'Command'
      OnExecute = SelEmptyDirActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelMarkAction: TAction
      Category = 'Command'
      OnExecute = SelMarkActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelMaskAction: TAction
      Category = 'Command'
      OnExecute = SelMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelOnlyCurAction: TAction
      Category = 'Command'
      OnExecute = SelOnlyCurActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelReverseAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#12398#36984#25246#29366#24907#12434#21453#36578'(&R)'
      OnExecute = SelReverseActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelReverseAllAction: TAction
      Category = 'Command'
      Caption = #12377#12409#12390#12398#38917#30446#12398#36984#25246#29366#24907#12434#21453#36578
      OnExecute = SelReverseAllActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SelSameExtAction: TAction
      Category = 'Command'
      OnExecute = SelSameExtActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelSameNameAction: TAction
      Category = 'Command'
      OnExecute = SelSameNameActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SelWorkItemAction: TAction
      Category = 'Command'
      OnExecute = SelWorkItemActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SetAliasAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12398#38917#30446#12395#12456#12452#12522#12450#12473#12434#35373#23450
      OnExecute = SetAliasActionExecute
      OnUpdate = WorkItemActionUpdate
    end
    object SetArcTimeAction: TAction
      Category = 'Command'
      OnExecute = SetArcTimeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetColorAction: TAction
      Category = 'Command'
      Caption = #12486#12461#12473#12488#12499#12517#12450#12540#12398#37197#33394'(&C)...'
      OnExecute = SetColorActionExecute
      OnUpdate = SetColorActionUpdate
    end
    object SetDirTimeAction: TAction
      Category = 'Command'
      OnExecute = SetDirTimeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetExifTimeAction: TAction
      Category = 'Command'
      Caption = #12479#12452#12512#12473#12479#12531#12503#12434' Exif '#25774#24433#26085#26178#12395
      OnExecute = SetExifTimeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetFontSizeAction: TAction
      Category = 'Command'
      OnExecute = SetFontSizeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetMarginAction: TAction
      Category = 'Command'
      OnExecute = SetMarginActionExecute
      OnUpdate = SetTvMetricActionUpdate
    end
    object SetPathMaskAction: TAction
      Category = 'Command'
      OnExecute = SetPathMaskActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetSttBarFmtAction: TAction
      Category = 'Command'
      OnExecute = SetSttBarFmtActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetSubSizeAction: TAction
      Category = 'Command'
      OnExecute = SetSubSizeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SetTagAction: TAction
      Category = 'Command'
      Caption = #12479#12464#12398#35373#23450'...'
      OnExecute = SetTagActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SetTabAction: TAction
      Category = 'Command'
      OnExecute = SetTabActionExecute
      OnUpdate = SetTvMetricActionUpdate
    end
    object SetWidthAction: TAction
      Category = 'Command'
      Caption = #25240#12426#36820#12375#24133
      OnExecute = SetWidthActionExecute
      OnUpdate = SetTvMetricActionUpdate
    end
    object ShareListAction: TAction
      Category = 'Command'
      Caption = #20849#26377#12501#12457#12523#12480#19968#35239'(&S)...'
      OnExecute = ShareListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ShowByteSizeAction: TAction
      Category = 'Command'
      OnExecute = ShowByteSizeActionExecute
      OnUpdate = ShowByteSizeActionUpdate
    end
    object ShowCRAction: TAction
      Category = 'Command'
      Caption = #25913#34892#12434#34920#31034
      OnExecute = ShowCRActionExecute
      OnUpdate = ShowCRActionUpdate
    end
    object ShowFileInfoAction: TAction
      Category = 'Command'
      OnExecute = ShowFileInfoActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object ShowFKeyBarAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12531#12463#12471#12519#12531#12461#12540#12496#12540#12398#34920#31034
      OnExecute = ShowFKeyBarActionExecute
      OnUpdate = ShowFKeyBarActionUpdate
    end
    object ShowHideAtrAction: TAction
      Category = 'Command'
      OnExecute = ShowHideAtrActionExecute
      OnUpdate = ShowHideAtrActionUpdate
    end
    object ShowIconAction: TAction
      Category = 'Command'
      Caption = #12450#12452#12467#12531#12434#34920#31034'(&I)'
      OnExecute = ShowIconActionExecute
      OnUpdate = ShowIconActionUpdate
    end
    object ShowLineNoAction: TAction
      Category = 'Command'
      Caption = #34892#30058#21495#12434#34920#31034'(&N)'
      OnExecute = ShowLineNoActionExecute
      OnUpdate = ShowLineNoActionUpdate
    end
    object ShowLogWinAction: TAction
      Category = 'Command'
      Caption = #12525#12464#12454#12451#12531#12489#12454#12434#34920#31034'(&L)'
      OnExecute = ShowLogWinActionExecute
      OnUpdate = ShowLogWinActionUpdate
    end
    object ShowPreviewAction: TAction
      Category = 'Command'
      Caption = #12503#12524#12499#12517#12540#12434#34920#31034'(&P)'
      OnExecute = ShowPreviewActionExecute
      OnUpdate = ShowPreviewActionUpdate
    end
    object ShowPropertyAction: TAction
      Category = 'Command'
      Caption = #12501#12449#12452#12523#24773#22577#12434#34920#31034'(&F)'
      OnExecute = ShowPropertyActionExecute
      OnUpdate = ShowPropertyActionUpdate
    end
    object ShowStatusBarAction: TAction
      Category = 'Command'
      Caption = #12473#12486#12540#12479#12473#12496#12540#12434#34920#31034'(&S)'
      OnExecute = ShowStatusBarActionExecute
      OnUpdate = ShowStatusBarActionUpdate
    end
    object ShowSystemAtrAction: TAction
      Category = 'Command'
      OnExecute = ShowSystemAtrActionExecute
      OnUpdate = ShowSystemAtrActionUpdate
    end
    object ShowRubyAction: TAction
      Category = 'Command'
      Caption = #12523#12499#12434#34920#31034'(&B)'
      OnExecute = ShowRubyActionExecute
      OnUpdate = ShowRubyActionUpdate
    end
    object ShowRulerAction: TAction
      Category = 'Command'
      Caption = #12523#12540#12521#12434#34920#31034'(&X)'
      OnExecute = ShowRulerActionExecute
      OnUpdate = ShowRulerActionUpdate
    end
    object ShowTABAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12434#34920#31034
      OnExecute = ShowTABActionExecute
      OnUpdate = ShowTABActionUpdate
    end
    object ShowTabBarAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12496#12540#12434#34920#31034
      OnExecute = ShowTabBarActionExecute
      OnUpdate = ShowTabBarActionUpdate
    end
    object ShowToolBarAction: TAction
      Category = 'Command'
      Caption = #12484#12540#12523#12496#12540#12434#34920#31034
      OnExecute = ShowToolBarActionExecute
      OnUpdate = ShowToolBarActionUpdate
    end
    object SoloTabAction: TAction
      Category = 'Command'
      Caption = #20182#12398#12479#12502#12434#12377#12409#12390#21066#38500'(&S)'
      OnExecute = SoloTabActionExecute
      OnUpdate = ChgTabActionUpdate
    end
    object SortDlgAction: TAction
      Category = 'Command'
      OnExecute = SortDlgActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object SpecialDirListAction: TAction
      Category = 'Command'
      Caption = #29305#27530#12501#12457#12523#12480#19968#35239'...'
      OnExecute = SpecialDirListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SubDirListAction: TAction
      Category = 'Command'
      OnExecute = SubDirListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SubViewerAction: TAction
      Category = 'Command'
      Caption = #12469#12502#12499#12517#12450#12540
      OnExecute = SubViewerActionExecute
      OnUpdate = SubViewerActionUpdate
    end
    object SuspendAction: TAction
      Category = 'Command'
      OnExecute = SuspendActionExecute
      OnUpdate = SuspendActionUpdate
    end
    object SwapLRAction: TAction
      Category = 'Command'
      OnExecute = SwapLRActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SwapNameAction: TAction
      Category = 'Command'
      OnExecute = SwapNameActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object SyncLRAction: TAction
      Category = 'Command'
      OnExecute = SyncLRActionExecute
      OnUpdate = SyncLRActionUpdate
    end
    object TabDlgAction: TAction
      Category = 'Command'
      Caption = #12479#12502#12398#35373#23450'(&T)...'
      OnExecute = TabDlgActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object TabHomeAction: TAction
      Category = 'Command'
      Caption = #12507#12540#12512#12487#12451#12524#12463#12488#12522'(&H)'
      OnExecute = TabHomeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object TagSelectAction: TAction
      Category = 'Command'
      OnExecute = TagSelectActionExecute
      OnUpdate = MenuFIActionUpdate
    end
    object TaskManAction: TAction
      Category = 'Command'
      Caption = #12479#12473#12463#12510#12493#12540#12472#12515'(&T)...'
      HelpContext = 69
      OnExecute = TaskManActionExecute
      OnUpdate = TaskManActionUpdate
    end
    object TestArchiveAction: TAction
      Category = 'Command'
      OnExecute = TestArchiveActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object TextViewerAction: TAction
      Category = 'Command'
      OnExecute = TextViewerActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToExViewerAction: TAction
      Category = 'Command'
      OnExecute = ToExViewerActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToLeftAction: TAction
      Category = 'Command'
      OnExecute = ToLeftActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToLogAction: TAction
      Category = 'Command'
      OnExecute = ToLogActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToNextOnRightAction: TAction
      Category = 'Command'
      OnExecute = ToNextOnRightActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToolBarDlgAction: TAction
      Category = 'Command'
      Caption = #12484#12540#12523#12496#12540#12398#35373#23450'(&T)...'
      HelpContext = 68
      OnExecute = ToolBarDlgActionExecute
      OnUpdate = ToolBarDlgActionUpdate
    end
    object ToOppositeAction: TAction
      Category = 'Command'
      OnExecute = ToOppositeActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToOppSameHashAction: TAction
      Category = 'Command'
      OnExecute = ToOppSameHashActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToOppSameItemAction: TAction
      Category = 'Command'
      OnExecute = ToOppSameItemActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToParentAction: TAction
      Category = 'Command'
      OnExecute = ToParentActionExecute
    end
    object ToParentOnLeftAction: TAction
      Category = 'Command'
      OnExecute = ToParentOnLeftActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToParentOnRightAction: TAction
      Category = 'Command'
      OnExecute = ToParentOnRightActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToPrevOnLeftAction: TAction
      Category = 'Command'
      OnExecute = ToPrevOnLeftActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToRightAction: TAction
      Category = 'Command'
      OnExecute = ToRightActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToRootAction: TAction
      Category = 'Command'
      OnExecute = ToRootActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ToTabAction: TAction
      Category = 'Command'
      OnExecute = ToTabActionExecute
      OnUpdate = ChgTabActionUpdate
    end
    object ToTextAction: TAction
      Category = 'Command'
      OnExecute = ToTextActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object TrimTagDataAction: TAction
      Category = 'Command'
      Caption = #12479#12464#12487#12540#12479#12398#25972#29702
      OnExecute = TrimTagDataActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object UndoRenameAction: TAction
      Category = 'Command'
      OnExecute = UndoRenameActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object UnPackAction: TAction
      Category = 'Command'
      OnExecute = UnPackActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object TV_TopIsHdrAction: TAction
      Category = 'Menu'
      Caption = #20808#38957#34892#12364#38917#30446#21517'(&H)'
      OnExecute = TV_TopIsHdrActionExecute
      OnUpdate = TV_TopIsHdrActionUpdate
    end
    object ShowOpenAsRunAction: TAction
      Category = 'Menu'
      Caption = #12501#12449#12452#12523#12434#38283#12367#12503#12525#12464#12521#12512#12398#36984#25246'(&O)...'
      OnExecute = ShowOpenAsRunActionExecute
      OnUpdate = ShowOpenAsRunActionUpdate
    end
    object IV_ThumbPosAction: TAction
      Category = 'Menu'
      Caption = #12469#12512#12493#12452#12523#12398#20301#32622'(&P)'
      OnExecute = IV_ThumbPosActionExecute
      OnUpdate = IV_ThumbPosActionUpdate
    end
    object IV_SidebarPosAction: TAction
      Category = 'Menu'
      Caption = #12469#12452#12489#12496#12540#12398#20301#32622
      OnExecute = IV_SidebarPosActionExecute
      OnUpdate = IV_SidebarPosActionUpdate
    end
    object UnPackToCurrAction: TAction
      Category = 'Command'
      OnExecute = UnPackToCurrActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object UpdateFromArcAction: TAction
      Category = 'Command'
      Caption = #12450#12540#12459#12452#12502#12363#12425#26356#26032'(&Z)...'
      OnExecute = UpdateFromArcActionExecute
      OnUpdate = CheckUpdateActionUpdate
    end
    object UseTrashAction: TAction
      Category = 'Command'
      OnExecute = UseTrashActionExecute
      OnUpdate = UseTrashActionUpdate
    end
    object ViewHistoryAction: TAction
      Category = 'Command'
      Caption = #26368#36817#38322#35239#12375#12383#12501#12449#12452#12523#19968#35239'(&V)...'
      HelpContext = 58
      OnExecute = ViewHistoryActionExecute
      OnUpdate = MenuFVActionUpdate
    end
    object ViewIniFileAction: TAction
      Category = 'Command'
      Caption = 'INI'#12501#12449#12452#12523#12398#38322#35239'(&V)'
      OnExecute = ViewIniFileActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ViewLogAction: TAction
      Category = 'Command'
      Caption = #12525#12464#12434#38322#35239'(&V)'
      OnExecute = ViewLogActionExecute
      OnUpdate = LogActionUpdate
    end
    object ViewTailAction: TAction
      Category = 'Command'
      OnExecute = ViewTailActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object WatchTailAction: TAction
      Category = 'Command'
      OnExecute = WatchTailActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object WebMapAction: TAction
      Category = 'Command'
      OnExecute = WebMapActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object WebSearchAction: TAction
      Category = 'Command'
      Caption = 'Web '#12391#26908#32034'(&S)'
      OnExecute = WebSearchActionExecute
      OnUpdate = WebSearchActionUpdate
    end
    object WinMaximizeAction: TAction
      Category = 'Command'
      OnExecute = WinMaximizeActionExecute
    end
    object WinMinimizeAction: TAction
      Category = 'Command'
      OnExecute = WinMinimizeActionExecute
    end
    object WinNormalAction: TAction
      Category = 'Command'
      OnExecute = WinNormalActionExecute
    end
    object GrepClipCopyAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12434#12463#12522#12483#12503#12508#12540#12489#12395#12467#12500#12540'(&C)'
      OnExecute = GrepClipCopyActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object GrepLineCopyAction: TAction
      Category = 'Grep'
      Caption = #12459#12540#12477#12523#34892#12434#12463#12522#12483#12503#12508#12540#12489#12395#12467#12500#12540'(&L)'
      OnExecute = GrepLineCopyActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object CP_ShiftJisAction: TAction
      Tag = 932
      Category = 'Menu'
      Caption = '&Shift JIS'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_JisAction: TAction
      Tag = 50220
      Category = 'Menu'
      Caption = '&JIS (ISO-2022-JP)'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_EucAction: TAction
      Tag = 20932
      Category = 'Menu'
      Caption = '&EUC-JP'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_Latin1Action: TAction
      Tag = 1252
      Category = 'Menu'
      Caption = 'Latin-1'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_UTF8Action: TAction
      Tag = 65001
      Category = 'Menu'
      Caption = '&UTF-8'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_UTF16Action: TAction
      Tag = 1200
      Category = 'Menu'
      Caption = 'U&TF16'
      OnExecute = CodePageActionExecute
      OnUpdate = CP_xxx_ActionUpdate
    end
    object CP_AutoAction: TAction
      Category = 'Menu'
      Caption = #33258#21205#21028#23450'(&A)'
      OnExecute = CodePageActionExecute
    end
    object ChgCodePageAction: TAction
      Category = 'Menu'
      Caption = #25991#23383#12467#12540#12489#22793#26356'(&C)'
      OnExecute = ChgCodePageActionExecute
      OnUpdate = ChgCodePageActionUpdate
    end
    object GrepSaveAsAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12395#21517#21069#12434#20184#12369#12390#20445#23384'(&A)...'
      OnExecute = GrepSaveAsActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object GrepExtractAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12398#32094#12426#36796#12415' : '#25277#20986'(&E)...'
      OnExecute = GrepExtractActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object GrepExceptAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12398#32094#12426#36796#12415' : '#38500#22806'(&X)...'
      OnExecute = GrepExceptActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object GrepReleaseAction: TAction
      Category = 'Grep'
      Caption = #32094#12426#36796#12415#12398#35299#38500'(&R)'
      OnExecute = GrepReleaseActionExecute
      OnUpdate = GrepReleaseActionUpdate
    end
    object GrepConfirmAction: TAction
      Category = 'Grep'
      Caption = #32094#12426#36796#12415#12434#30906#23450'(&M)'
      OnExecute = GrepConfirmActionExecute
      OnUpdate = GrepConfirmActionUpdate
    end
    object GrepOpenUrlAction: TAction
      Category = 'Grep'
      Caption = 'URL'#12434#38283#12367'(&W)'
      OnExecute = GrepOpenUrlActionExecute
      OnUpdate = GrepOpenUrlActionUpdate
    end
    object ReplaceStartAction: TAction
      Category = 'Grep'
      Caption = #38283#22987
      OnExecute = ReplaceStartActionExecute
      OnUpdate = ReplaceStartActionUpdate
    end
    object GrepOptionAction: TAction
      Category = 'Grep'
      Caption = #25313#24373#35373#23450
      OnExecute = GrepOptionActionExecute
      OnUpdate = GrepOptionActionUpdate
    end
    object GrepSelResAction: TAction
      Category = 'Grep'
      Caption = #12498#12483#12488#12375#12383#12459#12524#12531#12488#12398#12501#12449#12452#12523#12434#36984#25246
      OnExecute = GrepSelResActionExecute
      OnUpdate = GrepSelResActionUpdate
    end
    object GrepShowItemNoAction: TAction
      Category = 'Grep'
      Caption = #32080#26524#12522#12473#12488#12395#38917#30446#30058#21495#12434#34920#31034
      OnExecute = GrepShowItemNoActionExecute
      OnUpdate = GrepShowItemNoActionUpdate
    end
    object GrepFileItemNoAction: TAction
      Category = 'Grep'
      Caption = #38917#30446#30058#21495#12434#12501#12449#12452#12523#21336#20301#12391#34920#31034
      OnExecute = GrepFileItemNoActionExecute
      OnUpdate = GrepFileItemNoActionUpdate
    end
    object GrepShowSubDirAction: TAction
      Category = 'Grep'
      Caption = #32080#26524#12522#12473#12488#12395#12469#12502#12487#12451#12524#12463#12488#12522#21517#12434#34920#31034
      OnExecute = GrepShowSubDirActionExecute
      OnUpdate = GrepShowSubDirActionUpdate
    end
    object WorkItemDownAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12398#38917#30446#12434'1'#12388#19979#12395#31227#21205
      OnExecute = WorkItemDownActionExecute
      OnUpdate = WorkItemActionUpdate
    end
    object WorkItemMoveAction: TAction
      Category = 'Command'
      OnExecute = WorkItemMoveActionExecute
      OnUpdate = WorkItemActionUpdate
    end
    object WorkItemUpAction: TAction
      Category = 'Command'
      Caption = #12527#12540#12463#12522#12473#12488#12398#38917#30446#12434'1'#12388#19978#12395#31227#21205
      OnExecute = WorkItemUpActionExecute
      OnUpdate = WorkItemActionUpdate
    end
    object WorkListAction: TAction
      Category = 'Command'
      HelpContext = 60
      OnExecute = WorkListActionExecute
    end
    object WidenCurListAction: TAction
      Category = 'Command'
      OnExecute = WidenCurListActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object BitmapViewAction: TAction
      Category = 'CommandV'
      Caption = #12499#12483#12488#12510#12483#12503#12499#12517#12540'(&B)'
      OnExecute = BitmapViewActionExecute
      OnUpdate = BitmapViewActionUpdate
    end
    object Inf_EditCopy: TAction
      Category = 'Menu'
      Caption = #12467#12500#12540'(&C)'
      ShortCut = 16451
      OnExecute = Inf_EditCopyExecute
      OnUpdate = Inf_EditCopyUpdate
    end
    object Inf_EditSelectAll: TAction
      Category = 'Menu'
      Caption = #12377#12409#12390#36984#25246'(&A)'
      ShortCut = 16449
      OnExecute = Inf_EditSelectAllExecute
      OnUpdate = Inf_EditSelectAllUpdate
    end
    object Inf_OpenUrlAction: TAction
      Category = 'Menu'
      Caption = 'URL'#12434#38283#12367'(&W)'
      ShortCut = 13
      OnExecute = Inf_OpenUrlActionExecute
      OnUpdate = Inf_OpenUrlActionUpdate
    end
    object Inf_EmpItemAction: TAction
      Category = 'Menu'
      Caption = #12371#12398#38917#30446#20869#23481#12434#24375#35519#34920#31034'(&E)'
      OnExecute = Inf_EmpItemActionExecute
      OnUpdate = Inf_EmpItemActionUpdate
    end
    object Txt_EditCopy: TAction
      Category = 'Menu'
      Caption = #12467#12500#12540'(&C)'
      ShortCut = 16451
      OnExecute = Txt_EditCopyExecute
      OnUpdate = Txt_EditCopyUpdate
    end
    object Txt_EditSelectAll: TAction
      Category = 'Menu'
      Caption = #12377#12409#12390#36984#25246'(&A)'
      ShortCut = 16449
      OnExecute = Txt_EditSelectAllExecute
    end
    object Txt_OpenUrlAction: TAction
      Category = 'Menu'
      Caption = 'URL'#12434#38283#12367'(&W)'
      OnExecute = Txt_OpenUrlActionExecute
      OnUpdate = Txt_OpenUrlActionUpdate
    end
    object Txt_ShowLnNoAction: TAction
      Category = 'Menu'
      Caption = #34892#30058#21495#12434#34920#31034'(&N)'
      OnExecute = Txt_ShowLnNoActionExecute
      OnUpdate = Txt_ShowLnNoActionUpdate
    end
    object Txt_ShowTailAction: TAction
      Category = 'Menu'
      Caption = #26411#23614#20998#21106#34920#31034'(&T)'
      OnExecute = Txt_ShowTailActionExecute
      OnUpdate = Txt_ShowTailActionUpdate
    end
    object Txt_KeepIndexAction: TAction
      Category = 'Menu'
      Caption = #12459#12540#12477#12523#20301#32622#12434#32173#25345'(&K)'
      OnExecute = Txt_KeepIndexActionExecute
      OnUpdate = Txt_KeepIndexActionUpdate
    end
    object Log_EditCopy: TEditCopy
      Category = 'Menu'
      Caption = #12467#12500#12540'(&C)'
      ImageIndex = 1
      ShortCut = 16451
      OnExecute = Log_EditCopyExecute
      OnUpdate = Log_EditCopyUpdate
    end
    object Log_EditSelectAll: TEditSelectAll
      Category = 'Menu'
      Caption = #12377#12409#12390#36984#25246'(&A)'
      ShortCut = 16449
      OnExecute = Log_EditSelectAllExecute
      OnUpdate = Log_EditSelectAllUpdate
    end
    object Log_DebugInfAction: TAction
      Category = 'Menu'
      Caption = #12487#12496#12483#12464#24773#22577#12434#34920#31034
      OnExecute = Log_DebugInfActionExecute
      OnUpdate = Log_DebugInfActionUpdate
    end
    object CopyPreImgOrgAction: TAction
      Category = 'Menu'
      Caption = #30011#20687#12434#20803#12469#12452#12474#12391#12467#12500#12540'(&O)'
      OnExecute = CopyPreImgOrgActionExecute
      OnUpdate = CopyPreImgOrgActionUpdate
    end
    object CopyPreImgAction: TAction
      Category = 'Menu'
      Caption = #30011#20687#12434#34920#31034#12469#12452#12474#12391#12467#12500#12540'(&C)'
      OnExecute = CopyPreImgActionExecute
      OnUpdate = CopyPreImgActionUpdate
    end
    object CharInfoAction: TAction
      Category = 'CommandV'
      Caption = #25991#23383#24773#22577'(&I)'
      OnExecute = CharInfoActionExecute
      OnUpdate = CharInfoActionUpdate
    end
    object CsvCalcAction: TAction
      Category = 'CommandV'
      Caption = 'CSV/TSV'#38917#30446#12398#38598#35336'(&T)...'
      OnExecute = CsvCalcActionExecute
      OnUpdate = CsvCalcActionUpdate
    end
    object CsvGraphAction: TAction
      Category = 'CommandV'
      Caption = 'CSV/TSV'#38917#30446#12398#12464#12521#12501'(&G)...'
      OnExecute = CsvGraphActionExecute
      OnUpdate = CsvGraphActionUpdate
    end
    object CsvRecordAction: TAction
      Category = 'CommandV'
      Caption = 'CSV/TSV'#12524#12467#12540#12489'(&D)'
      OnExecute = CsvRecordActionExecute
      OnUpdate = CsvRecordActionUpdate
    end
    object DeleteVAction: TAction
      Category = 'CommandV'
      OnExecute = DeleteVActionExecute
      OnUpdate = MenuVUpdate
    end
    object FindTextAction: TAction
      Category = 'CommandV'
      Caption = #26908#32034'(&F)...'
      OnExecute = FindTextActionExecute
      OnUpdate = MenuVUpdate
    end
    object FunctionListAction: TAction
      Category = 'CommandV'
      Caption = #38306#25968#19968#35239'(&F)...'
      OnExecute = FunctionListActionExecute
      OnUpdate = FunctionListActionUpdate
    end
    object HighlightAction: TAction
      Category = 'CommandV'
      OnExecute = HighlightActionExecute
      OnUpdate = HighlightActionUpdate
    end
    object InspectorAction: TAction
      Category = 'CommandV'
      Caption = #12452#12531#12473#12506#12463#12479#12540'(&I)'
      OnExecute = InspectorActionExecute
      OnUpdate = InspectorActionUpdate
    end
    object MarkListVAction: TAction
      Category = 'CommandV'
      Caption = #12510#12540#12463#34892#19968#35239'(&M)...'
      OnExecute = MarkListVActionExecute
      OnUpdate = MarkListVActionUpdate
    end
    object GrepTrimTopAction: TAction
      Category = 'Grep'
      Caption = #34892#38957#12398#12479#12502#12420#31354#30333#12434#38500#22806
      OnExecute = GrepTrimTopActionExecute
      OnUpdate = GrepTrimTopActionUpdate
    end
    object SaveDumpAction: TAction
      Category = 'CommandV'
      Caption = #12480#12531#12503#12522#12473#12488#12434#20445#23384'(&D)...'
      OnExecute = SaveDumpActionExecute
      OnUpdate = SaveDumpActionUpdate
    end
    object SetTopAddrAction: TAction
      Category = 'CommandV'
      Caption = #20808#38957#12450#12489#12524#12473#12434#35373#23450'(&T)'
      OnExecute = SetTopAddrActionExecute
      OnUpdate = SetTopAddrActionUpdate
    end
    object WinPosAction: TAction
      Category = 'Command'
      OnExecute = WinPosActionExecute
      OnUpdate = MenuFVIActionUpdate
    end
    object XmlViewerAction: TAction
      Category = 'Command'
      Caption = 'XML'#12499#12517#12450#12540'(&X)...'
      OnExecute = XmlViewerActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ZoomInAction: TAction
      Category = 'Command'
      OnExecute = ZoomInActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ZoomOutAction: TAction
      Category = 'Command'
      OnExecute = ZoomOutActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object ZoomResetAction: TAction
      Category = 'Command'
      OnExecute = ZoomResetActionExecute
      OnUpdate = MenuFActionUpdate
    end
    object UserDefListAction: TAction
      Category = 'CommandV'
      Caption = #12518#12540#12470#23450#32681#25991#23383#21015#19968#35239'(&U)...'
      OnExecute = UserDefListActionExecute
      OnUpdate = FunctionListActionUpdate
    end
    object ExportCsvAction: TAction
      Category = 'CommandV'
      Caption = 'CSV/TSV'#12456#12463#12473#12509#12540#12488'(&X)...'
      OnExecute = ExportCsvActionExecute
      OnUpdate = ExportCsvActionUpdate
    end
    object GrepOmitTopAction: TAction
      Category = 'Grep'
      Caption = #12510#12483#12481#35486#12364#35211#12360#12394#12356#12392#12365#21069#37096#20998#12434#30465#30053
      OnExecute = GrepOmitTopActionExecute
      OnUpdate = GrepOmitTopActionUpdate
    end
    object GrepEmFilterAction: TAction
      Category = 'Grep'
      Caption = #12501#12451#12523#12479#12398#35486#12434#24375#35519#34920#31034
      OnExecute = GrepEmFilterActionExecute
      OnUpdate = GrepEmFilterActionUpdate
    end
    object PushResultAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12434#36864#36991'(&U)'
      OnExecute = PushResultActionExecute
      OnUpdate = GrepResultActionUpdate
    end
    object PopResultAction: TAction
      Category = 'Grep'
      Caption = #26908#32034#32080#26524#12398#24489#24112'(&O)'
      OnExecute = PopResultActionExecute
      OnUpdate = PopResultActionUpdate
    end
    object FK_RenameAction: TAction
      Category = 'Menu'
      Caption = #34920#31034#21517#12398#22793#26356'(&R)...'
      OnExecute = FK_RenameActionExecute
    end
  end
  object TaskSttTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = TaskSttTimerTimer
    Left = 376
    Top = 135
  end
  object ViewPopupMenu: TPopupMenu
    OnPopup = ViewPopupMenuPopup
    Left = 136
    Top = 246
    object ViewCopyItem: TMenuItem
      Action = TV_EditCopyAction
    end
    object CopyAddItem: TMenuItem
      Action = TV_EditAddAction
    end
    object ViewSelAllItem: TMenuItem
      Action = TV_EditSelAllAction
    end
    object PopFindItem: TMenuItem
      Action = FindTextAction
    end
    object PopWebSearchItem: TMenuItem
      Action = WebSearchAction
    end
    object Sep_vp_1: TMenuItem
      Caption = '-'
    end
    object ChgCodePageItem: TMenuItem
      Action = ChgCodePageAction
      object CP_ShiftJISItem: TMenuItem
        Action = CP_ShiftJisAction
      end
      object CP_JisItem: TMenuItem
        Action = CP_JisAction
      end
      object CP_EUCItem: TMenuItem
        Action = CP_EucAction
      end
      object CP_Latin1Item: TMenuItem
        Action = CP_Latin1Action
      end
      object CP_UTF8Item: TMenuItem
        Action = CP_UTF8Action
      end
      object CP_UTF16Item: TMenuItem
        Action = CP_UTF16Action
      end
      object CP_AutoItem: TMenuItem
        Action = CP_AutoAction
      end
    end
    object Sep_vp_2: TMenuItem
      Caption = '-'
    end
    object SortItem: TMenuItem
      Caption = #12477#12540#12488'(&O)'
      object SortAscItem: TMenuItem
        Action = TV_SortAscAction
      end
      object SortDesItem: TMenuItem
        Action = TV_SortDesAction
      end
      object OrgOrderItem: TMenuItem
        Action = TV_OrgOrderAction
      end
    end
    object TopIsHdrItem: TMenuItem
      Action = TV_TopIsHdrAction
    end
    object PopCsvCalcItem: TMenuItem
      Action = CsvCalcAction
    end
    object PopCsvGraphItem: TMenuItem
      Action = CsvGraphAction
    end
    object Sep_vp_3: TMenuItem
      Caption = '-'
    end
    object PoxExpCsvItem: TMenuItem
      Action = ExportCsvAction
    end
    object PopSaveDumpItem: TMenuItem
      Action = SaveDumpAction
    end
  end
  object ApplicationEvents1: TApplicationEvents
    OnActivate = ApplicationEvents1Activate
    OnDeactivate = ApplicationEvents1Deactivate
    OnIdle = ApplicationEvents1Idle
    OnHelp = ApplicationEvents1Help
    OnMessage = ApplicationEvents1Message
    OnMinimize = ApplicationEvents1Minimize
    OnModalBegin = ApplicationEvents1ModalBegin
    OnModalEnd = ApplicationEvents1ModalEnd
    OnShowHint = ApplicationEvents1ShowHint
    Left = 140
    Top = 88
  end
  object LogPopupMenu: TPopupMenu
    Left = 448
    Top = 406
    object PopCopyItem: TMenuItem
      Action = Log_EditCopy
    end
    object PopSelAllItem: TMenuItem
      Action = Log_EditSelectAll
    end
    object Sep_g_1: TMenuItem
      Caption = '-'
    end
    object PopClrLogItem: TMenuItem
      Action = ClearLogAction
    end
    object PopViewLogItem: TMenuItem
      Action = ViewLogAction
    end
    object Sep_g_2: TMenuItem
      Caption = '-'
    end
    object PopDebugInfItem: TMenuItem
      Action = Log_DebugInfAction
    end
  end
  object UpdLogTimer: TTimer
    Enabled = False
    Interval = 200
    OnTimer = UpdLogTimerTimer
    Left = 452
    Top = 135
  end
  object ExPopupMenu: TPopupMenu
    AutoPopup = False
    Images = IconImgListP
    Left = 139
    Top = 137
  end
  object WatchDirTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = WatchDirTimerTimer
    Left = 528
    Top = 135
  end
  object MsgHintTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = MsgHintTimerTimer
    Left = 372
    Top = 199
  end
  object KeyHintTimer: TTimer
    Enabled = False
    OnTimer = KeyHintTimerTimer
    Left = 448
    Top = 199
  end
  object DropPopupMenu: TPopupMenu
    AutoHotkeys = maManual
    Left = 248
    Top = 138
    object DropCopyItem: TMenuItem
      Caption = #12467#12500#12540'(&C)'
      OnClick = DropMenuItemClick
    end
    object DropMoveItem: TMenuItem
      Tag = 1
      Caption = #31227#21205'(&M)'
      OnClick = DropMenuItemClick
    end
    object DropLinkItem: TMenuItem
      Tag = 2
      Caption = #12471#12519#12540#12488#12459#12483#12488#12434#20316#25104'(&S)'
      OnClick = DropMenuItemClick
    end
  end
  object ImgPrvPopupMenu: TPopupMenu
    OnPopup = ImgPrvPopupMenuPopup
    Left = 31
    Top = 300
    object CopyPrviewImgItem: TMenuItem
      Action = CopyPreImgOrgAction
    end
    object CopyPreImgItem: TMenuItem
      Action = CopyPreImgAction
    end
    object Sep_ip_1: TMenuItem
      Caption = '-'
    end
    object PopEditFontSmplItem: TMenuItem
      Caption = #12501#12457#12531#12488#12469#12531#12503#12523#23450#32681#12501#12449#12452#12523#12398#32232#38598
      OnClick = EditFontSmplItemClick
    end
  end
  object InfPopupMenu: TPopupMenu
    Left = 176
    Top = 406
    object PopCopyFileInfoItm: TMenuItem
      Action = CopyFileInfoAction
    end
    object Sep_i_1: TMenuItem
      Caption = '-'
    end
    object InfEditCopyItem: TMenuItem
      Action = Inf_EditCopy
    end
    object InfEditSelectAllItem: TMenuItem
      Action = Inf_EditSelectAll
    end
    object Sep_i_2: TMenuItem
      Caption = '-'
    end
    object InfOpenUrlItem: TMenuItem
      Action = Inf_OpenUrlAction
    end
    object InfEmpItemAction: TMenuItem
      Action = Inf_EmpItemAction
    end
    object Sep_i_3: TMenuItem
      Caption = '-'
    end
    object PopPropertyItem: TMenuItem
      Action = PropertyDlgAction
    end
  end
  object BlinkTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = BlinkTimerTimer
    Left = 528
    Top = 197
  end
  object DrivePopupMenu: TPopupMenu
    OwnerDraw = True
    Left = 34
    Top = 190
  end
  object SelItemPopupMenu: TPopupMenu
    OwnerDraw = True
    Left = 136
    Top = 192
  end
  object TxtPrvPopupMenu: TPopupMenu
    Left = 135
    Top = 300
    object TxtEditCopyItem: TMenuItem
      Action = Txt_EditCopy
    end
    object TxtEditSelectAllItem: TMenuItem
      Action = Txt_EditSelectAll
    end
    object Sep_tp_2: TMenuItem
      Caption = '-'
    end
    object TxtOpenUrlItem: TMenuItem
      Action = Txt_OpenUrlAction
      ShortCut = 13
    end
    object Sep_tp_1: TMenuItem
      Caption = '-'
    end
    object TxtShowLineNoItem: TMenuItem
      Action = Txt_ShowLnNoAction
    end
    object TxtKeepIndexItem: TMenuItem
      Action = Txt_KeepIndexAction
    end
    object TxtShowTailItem: TMenuItem
      Action = Txt_ShowTailAction
    end
    object LockTxPreviewItem: TMenuItem
      Action = LockTextPreviewAction
    end
  end
  object IconImgList: TImageList
    ColorDepth = cd32Bit
    Left = 376
    Top = 267
  end
  object IconImgListV: TImageList
    ColorDepth = cd32Bit
    Left = 448
    Top = 267
  end
  object IconImgListI: TImageList
    ColorDepth = cd32Bit
    Left = 520
    Top = 267
  end
  object IconImgListP: TImageList
    ColorDepth = cd32Bit
    Left = 592
    Top = 266
  end
  object GrepPopupMenu: TPopupMenu
    AutoLineReduction = maManual
    Left = 32
    Top = 248
    object PopGrepClipCopyItem: TMenuItem
      Action = GrepClipCopyAction
    end
    object PopGrepLineCopyItem: TMenuItem
      Action = GrepLineCopyAction
    end
    object PopGrepSaveAsItem: TMenuItem
      Action = GrepSaveAsAction
    end
    object Pop_g_1: TMenuItem
      Caption = '-'
    end
    object PopPushResultItem: TMenuItem
      Action = PushResultAction
    end
    object PopPopResultItem: TMenuItem
      Action = PopResultAction
    end
    object Pop_g_2: TMenuItem
      Caption = '-'
    end
    object PopGrepExtractItem: TMenuItem
      Action = GrepExtractAction
    end
    object PopGrepExceptItem: TMenuItem
      Action = GrepExceptAction
    end
    object PopGrepReleaseItem: TMenuItem
      Action = GrepReleaseAction
    end
    object PopGrepConfirmItem: TMenuItem
      Action = GrepConfirmAction
    end
    object Pop_g_3: TMenuItem
      Caption = '-'
    end
    object PopGrepSelResItem: TMenuItem
      Action = GrepSelResAction
    end
    object PopGrepOpenUrlItem: TMenuItem
      Action = GrepOpenUrlAction
    end
    object Pop_g_4: TMenuItem
      Caption = '-'
    end
    object PopGrepItemNoItem: TMenuItem
      Action = GrepShowItemNoAction
    end
    object PopGrepFileItemNoItem: TMenuItem
      Action = GrepFileItemNoAction
    end
    object PopGrepSubDirItem: TMenuItem
      Action = GrepShowSubDirAction
    end
    object PopGrepTrimTopItem: TMenuItem
      Action = GrepTrimTopAction
    end
    object PopGrepOmitItem: TMenuItem
      Action = GrepOmitTopAction
    end
    object PopGrepEmFilterItem: TMenuItem
      Action = GrepEmFilterAction
    end
  end
  object TabPopupMenu: TPopupMenu
    OnPopup = TabPopupMenuPopup
    Left = 248
    Top = 193
    object PopAddTabItem: TMenuItem
      Action = AddTabAction
    end
    object PopDelTabItem: TMenuItem
      Action = DelTabAction
    end
    object PopSoloTabItem: TMenuItem
      Action = SoloTabAction
    end
    object FisTabPathItem: TMenuItem
      Action = FixTabPathAction
      Caption = #12459#12524#12531#12488#12497#12473#12434#19968#26178#22266#23450'/'#35299#38500'(&F)'
    end
    object Sep_pt_2: TMenuItem
      Caption = '-'
    end
    object PopSaveAsTabGroupItem: TMenuItem
      Action = SaveAsTabGroupAction
    end
    object PopSaveTagGroupItem: TMenuItem
      Action = SaveTabGroupAction
    end
    object PopLoadTabGroupItem: TMenuItem
      Action = LoadTabGroupAction
    end
    object Sep_pt_1: TMenuItem
      Caption = '-'
    end
    object PopTabDlgItem: TMenuItem
      Action = TabDlgAction
    end
    object PopTabHomeItem: TMenuItem
      Action = TabHomeAction
    end
  end
  object WatchTailTimer: TTimer
    Enabled = False
    OnTimer = WatchTailTimerTimer
    Left = 603
    Top = 135
  end
  object TrayIcon1: TTrayIcon
    Icon.Data = {
      0000010001001010100001000400280100001600000028000000100000002000
      0000010004000000000000000000000000000000000000000000000000000000
      000000008000008000000080800080000000800080008080000080808000C0C0
      C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF000111
      1111111111101444444444444441144444EEEE44444114444EEEEEE444411444
      4EEEEEE4444114444EEEEEE4444114E44EEEEEE44E411EEE44EEEE44EEE11EEE
      44444444EEE114E4444444444E4114444E4444E444411444EEE44EEE44411444
      EEE44EEE444114444E4444E44441144444444444444101111111111111108001
      F3040000F3040000F3040000F3040000F3040000F3040000F3040000F3040000
      F3040000F3040000F3040000F3040000F3040000F3040000F3048001F304}
    PopupMenu = TrayPopupMenu
    OnClick = TrayIcon1Click
    Left = 562
    Top = 403
  end
  object TrayPopupMenu: TPopupMenu
    OnPopup = TrayPopupMenuPopup
    Left = 642
    Top = 403
    object Sep_r_1: TMenuItem
      Caption = '-'
    end
    object TrayDuplItem: TMenuItem
      Action = DuplicateAction
    end
    object TrayRestartItem: TMenuItem
      Action = RestartAction
    end
    object Sep_r_2: TMenuItem
      Caption = '-'
    end
    object TrayAppListItem: TMenuItem
      Action = AppListAction
    end
    object TrayTaskManItem: TMenuItem
      Action = TaskManAction
    end
    object Sep_r_3: TMenuItem
      Caption = '-'
    end
    object TrayExitItem: TMenuItem
      Action = ExitAction
    end
  end
end
