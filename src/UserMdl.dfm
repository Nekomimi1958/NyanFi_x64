object UserModule: TUserModule
  OnCreate = DataModuleCreate
  OnDestroy = DataModuleDestroy
  Height = 220
  Width = 342
  object OpenDlg: TOpenDialog
    Options = [ofHideReadOnly, ofNoDereferenceLinks, ofEnableSizing]
    Left = 16
    Top = 8
  end
  object SaveDlg: TSaveDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 152
    Top = 8
  end
  object SaveTxtDlg: TSaveTextFileDialog
    Options = [ofOverwritePrompt, ofHideReadOnly, ofEnableSizing]
    Left = 216
    Top = 8
  end
  object ColorDlg: TColorDialog
    Left = 16
    Top = 80
  end
  object SpuitTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = SpuitTimerTimer
    Left = 144
    Top = 80
  end
  object CommonActList: TActionList
    Left = 32
    Top = 152
    object UpListItemAction: TAction
      Category = 'ListBox'
      Caption = #8593
      OnExecute = UpListItemActionExecute
      OnUpdate = UpListItemActionUpdate
    end
    object DownListItemAction: TAction
      Category = 'ListBox'
      Caption = #8595
      OnExecute = DownListItemActionExecute
      OnUpdate = DownListItemActionUpdate
    end
    object RefFileNameAction: TAction
      Category = 'Other'
      Caption = #20803#12501#12449#12452#12523#21517#12434#25407#20837'(&F)'
      OnExecute = RefFileNameActionExecute
      OnUpdate = RefFileNameActionUpdate
    end
    object DelListItemAction: TAction
      Category = 'ListBox'
      Caption = #21066#38500
      OnExecute = DelListItemActionExecute
      OnUpdate = DelListItemActionUpdate
    end
    object CutComboAction: TAction
      Category = 'ComboBox'
      Caption = #20999#12426#21462#12426'(&T)'
      ShortCut = 16472
      OnExecute = CutComboActionExecute
      OnUpdate = EditComboActionUpdate
    end
    object CopyComboAction: TAction
      Category = 'ComboBox'
      Caption = #12467#12500#12540'(&C)'
      ShortCut = 16451
      OnExecute = CopyComboActionExecute
      OnUpdate = EditComboActionUpdate
    end
    object PasteComboAction: TAction
      Category = 'ComboBox'
      Caption = #36028#12426#20184#12369'(&P)'
      ShortCut = 16470
      OnExecute = PasteComboActionExecute
      OnUpdate = PasteComboActionUpdate
    end
    object SelAllComboAction: TAction
      Category = 'ComboBox'
      Caption = #12377#12409#12390#36984#25246'(&A)'
      ShortCut = 16449
      OnExecute = SelAllComboActionExecute
      OnUpdate = SelAllComboActionUpdate
    end
    object DelHistComboAction: TAction
      Category = 'ComboBox'
      Caption = #12371#12398#38917#30446#12434#23653#27508#12363#12425#21066#38500'(&D)'
      OnExecute = DelHistComboActionExecute
      OnUpdate = DelHistComboActionUpdate
    end
    object ClrHistComboAction: TAction
      Category = 'ComboBox'
      Caption = #23653#27508#12434#12377#12409#12390#21066#38500'(&H)'
      OnExecute = ClrHistComboActionExecute
      OnUpdate = ClrHistComboActionUpdate
    end
    object RenInsNameAction: TAction
      Category = 'ComboBox'
      Caption = '1'#30058#30446#12398#21517#21069#12434#20837#21147'(&I)'
      ShortCut = 16457
    end
    object RefCurPNameAction: TAction
      Category = 'Other'
      Caption = #12459#12540#12477#12523#20301#32622#12398#12497#12473#20184#12365#12501#12449#12452#12523#21517'(&P)'
      OnExecute = RefCurPNameActionExecute
      OnUpdate = RefCurPNameActionUpdate
    end
    object EditCut1: TEditCut
      Category = #32232#38598
      Caption = #20999#12426#21462#12426'(&T)'
      Hint = #20999#12426#21462#12426'|'#36984#25246#37096#20998#12434#20999#12426#21462#12387#12390#12463#12522#12483#12503#12508#12540#12489#12395#20837#12428#12414#12377
      ImageIndex = 0
      ShortCut = 16472
    end
    object EditCopy1: TEditCopy
      Category = #32232#38598
      Caption = #12467#12500#12540'(&C)'
      Hint = #12467#12500#12540'|'#36984#25246#37096#20998#12434#12467#12500#12540#12375#12390#12463#12522#12483#12503#12508#12540#12489#12395#20837#12428#12414#12377
      ImageIndex = 1
      ShortCut = 16451
    end
    object EditPaste1: TEditPaste
      Category = #32232#38598
      Caption = #36028#12426#20184#12369'(&P)'
      Hint = #36028#12426#20184#12369'|'#12463#12522#12483#12503#12508#12540#12489#12398#20869#23481#12434#25407#20837#12375#12414#12377
      ImageIndex = 2
      ShortCut = 16470
      OnExecute = EditPaste1Execute
    end
    object EditSelectAll1: TEditSelectAll
      Category = #32232#38598
      Caption = #12377#12409#12390#36984#25246'(&A)'
      Hint = #12377#12409#12390#36984#25246'|'#12489#12461#12517#12513#12531#12488#20840#20307#12434#36984#25246#12375#12414#12377
      ShortCut = 16449
    end
    object EditUndo1: TEditUndo
      Category = #32232#38598
      Caption = #20803#12395#25147#12377'(&U)'
      Hint = #20803#12395#25147#12377'|'#30452#21069#12398#25805#20316#12434#20803#12395#25147#12375#12414#12377
      ImageIndex = 3
      ShortCut = 16474
    end
    object EditDelete1: TEditDelete
      Category = #32232#38598
      Caption = #21066#38500'(&D)'
      Hint = #21066#38500'|'#36984#25246#37096#20998#12434#28040#21435#12375#12414#12377
      ImageIndex = 5
    end
    object RefCurFNameAction: TAction
      Category = 'Other'
      Caption = #12459#12540#12477#12523#20301#32622#12398#12501#12449#12452#12523#21517'(&F)'
      OnExecute = RefCurFNameActionExecute
      OnUpdate = RefCurFNameActionUpdate
    end
    object RefCurPathAction: TAction
      Category = 'Other'
      Caption = #12459#12524#12531#12488#12497#12473
      OnExecute = RefCurPathActionExecute
      OnUpdate = RefCurPathActionUpdate
    end
    object RefOppPathAction: TAction
      Category = 'Other'
      Caption = #21453#23550#12497#12473'(&O)'
      OnExecute = RefOppPathActionExecute
      OnUpdate = RefOppPathActionUpdate
    end
    object RefCurDirAction: TAction
      Category = 'Other'
      Caption = #12459#12524#12531#12488#12398#12497#12473#28961#12375#12487#12451#12524#12463#12488#12522#21517
      OnExecute = RefCurDirActionExecute
      OnUpdate = RefCurDirActionUpdate
    end
    object RefCmdNameAction: TAction
      Category = 'Other'
      Caption = #12467#12510#12531#12489#21517#12434#21442#29031'(&R)'
      OnExecute = RefCmdNameActionExecute
      OnUpdate = RefCmdNameActionUpdate
    end
    object RegExCheckerAction: TAction
      Category = 'Other'
      Caption = #27491#35215#34920#29694#12481#12455#12483#12459#12540'(&K)'
      HelpContext = 85
      OnExecute = RegExCheckerActionExecute
      OnUpdate = RegExCheckerActionUpdate
    end
    object ClrListItemAction: TAction
      Category = 'ListBox'
      Caption = #12463#12522#12450
      OnExecute = ClrListItemActionExecute
      OnUpdate = ClrListItemActionUpdate
    end
    object CalculatorAction: TAction
      Category = 'Other'
      Caption = #38651#21331
      OnExecute = CalculatorActionExecute
      OnUpdate = CalculatorActionUpdate
    end
    object SizePosToFlieListAction: TAction
      Category = 'Other'
      Caption = #12469#12452#12474#12392#20301#32622#12434#12501#12449#12452#12523#12522#12473#12488#12395#21512#12431#12379#12427
      OnExecute = SizePosToFlieListActionExecute
    end
    object WebSeaComboAction: TAction
      Category = 'ComboBox'
      OnExecute = WebSeaComboActionExecute
      OnUpdate = WebSeaComboActionUpdate
    end
    object WebSeaEditAction: TAction
      Category = 'Other'
      OnExecute = WebSeaEditActionExecute
      OnUpdate = WebSeaEditActionUpdate
    end
  end
  object EditPopupMenuC: TPopupMenu
    OnPopup = EditPopupMenuCPopup
    Left = 139
    Top = 150
    object GrepCutItem: TMenuItem
      Action = CutComboAction
    end
    object GrepCopyItem: TMenuItem
      Action = CopyComboAction
    end
    object GrepPasteItem: TMenuItem
      Action = PasteComboAction
    end
    object SelAllItem: TMenuItem
      Action = SelAllComboAction
    end
    object Sep_g_1: TMenuItem
      Caption = '-'
    end
    object RenInputNameItem: TMenuItem
      Action = RenInsNameAction
      Visible = False
    end
    object RefCurFnamItem: TMenuItem
      Action = RefCurFNameAction
    end
    object RefCurPnamItem: TMenuItem
      Action = RefCurPNameAction
    end
    object RefCurDirItem: TMenuItem
      Action = RefCurDirAction
    end
    object Sep_g_2: TMenuItem
      Caption = '-'
    end
    object CmdParamItem: TMenuItem
      Caption = #12497#12521#12513#12540#12479'(&R)'
    end
    object RegExpItem: TMenuItem
      Caption = #27491#35215#34920#29694'(&X)'
    end
    object RegExChkItem: TMenuItem
      Action = RegExCheckerAction
    end
    object EditItemItem: TMenuItem
      Caption = #35373#23450#38917#30446#12398#32232#38598'(&E)...'
      OnClick = EditItemItemClick
    end
    object CalculatorItem: TMenuItem
      Action = CalculatorAction
    end
    object WebSeaComboItem: TMenuItem
      Action = WebSeaComboAction
    end
    object Sep_g_3: TMenuItem
      Caption = '-'
    end
    object DelHistItem: TMenuItem
      Action = DelHistComboAction
    end
    object ClrHistItem: TMenuItem
      Action = ClrHistComboAction
    end
    object Sep_g_4: TMenuItem
      Caption = '-'
    end
    object RefCmdItem: TMenuItem
      Action = RefCmdNameAction
    end
  end
  object OpenImgDlg: TOpenPictureDialog
    Left = 80
    Top = 8
  end
  object EditPopupMenuE: TPopupMenu
    OnPopup = EditPopupMenuEPopup
    Left = 248
    Top = 152
    object PopUndoItem: TMenuItem
      Action = EditUndo1
    end
    object Sep_e_1: TMenuItem
      Caption = '-'
    end
    object PopCutItem: TMenuItem
      Action = EditCut1
    end
    object PopCopyItem: TMenuItem
      Action = EditCopy1
    end
    object PopPasteItem: TMenuItem
      Action = EditPaste1
    end
    object PopDelItem: TMenuItem
      Action = EditDelete1
    end
    object Sep_e_2: TMenuItem
      Caption = '-'
    end
    object PopAllSelItem: TMenuItem
      Action = EditSelectAll1
    end
    object Sep_e_3: TMenuItem
      Caption = '-'
    end
    object RefFileNameItem: TMenuItem
      Action = RefFileNameAction
    end
    object RefCurDirEItem: TMenuItem
      Action = RefCurDirAction
    end
    object PopRefCmdItem: TMenuItem
      Action = RefCmdNameAction
    end
    object PopRefSndIdItem: TMenuItem
      AutoHotkeys = maManual
      Caption = #12469#12454#12531#12489#35672#21029#23376#12434#21442#29031'(&S)'
    end
    object PopRefFmtItem: TMenuItem
      Caption = #26360#24335#25991#23383#21015#12434#21442#29031'(&R)'
      object PopDFmt01Item: TMenuItem
        Caption = '\L(n)  '#12501#12449#12452#12523#21517#20027#37096#12398#24038#12363#12425'n'#25991#23383
        OnClick = PopDFmtItemClick
      end
      object PopDFmt02Item: TMenuItem
        Caption = '\S(m,n)  '#12501#12449#12452#12523#21517#20027#37096#12398'm'#30058#30446#12363#12425'n'#25991#23383
        OnClick = PopDFmtItemClick
      end
      object PopDFmt03Item: TMenuItem
        Caption = '\R(n)  '#12501#12449#12452#12523#21517#20027#37096#12398#21491#12363#12425'n'#25991#23383
        OnClick = PopDFmtItemClick
      end
      object PopDFmt04Item: TMenuItem
        Caption = '\E  '#25313#24373#23376
        OnClick = PopDFmtItemClick
      end
      object PopDFmt05Item: TMenuItem
        Caption = '\TS(yyyymmdd)  '#12479#12452#12512#12473#12479#12531#12503
        OnClick = PopDFmtItemClick
      end
      object PopDFmt06Item: TMenuItem
        Caption = '\XT(yyyymmdd)  Exif'#25774#24433#26085#26178
        OnClick = PopDFmtItemClick
      end
      object PopDFmt07Item: TMenuItem
        Caption = '\1  1'#30058#30446#12398#27491#35215#34920#29694#12469#12502#12497#12479#12540#12531
        OnClick = PopDFmtItemClick
      end
      object PopDFmt08Item: TMenuItem
        Caption = '\2  2'#30058#30446#12398#27491#35215#34920#29694#12469#12502#12497#12479#12540#12531
        OnClick = PopDFmtItemClick
      end
      object PopDFmt09Item: TMenuItem
        Caption = '\\  \ '#12381#12398#12418#12398
        OnClick = PopDFmtItemClick
      end
    end
    object PopRefDlmtItem: TMenuItem
      Caption = #12487#12451#12524#12463#12488#12522#21306#20999#12426#12398#21442#29031
      object PopDlmt1Item: TMenuItem
        Caption = '/ (U+002F)'
        OnClick = PopDlmtItemClick
      end
      object PopDlmt2Item: TMenuItem
        Caption = '\ (U+005C)'
        OnClick = PopDlmtItemClick
      end
      object PopDlmt3Item: TMenuItem
        Caption = #8726' (U+2216)'
        OnClick = PopDlmtItemClick
      end
      object PopDlmt4Item: TMenuItem
        Caption = #9586' (U+2572)'
        OnClick = PopDlmtItemClick
      end
      object PopDlmt5Item: TMenuItem
        Caption = #10741' (U+29F5)'
        OnClick = PopDlmtItemClick
      end
      object PopDlmt6Item: TMenuItem
        Caption = #65340' (U+FF3C)'
        OnClick = PopDlmtItemClick
      end
    end
    object RegExpEItem: TMenuItem
      Caption = #27491#35215#34920#29694'(&X)'
    end
    object RegExChkEItem: TMenuItem
      Action = RegExCheckerAction
    end
    object PopEditItemItem: TMenuItem
      Caption = #35373#23450#38917#30446#12398#32232#38598'(&E)...'
      OnClick = PopEditItemItemClick
    end
    object Sep_e_4: TMenuItem
      Caption = '-'
    end
    object CalculatorEItem: TMenuItem
      Action = CalculatorAction
    end
    object WebSeaEditItem: TMenuItem
      Action = WebSeaEditAction
    end
    object Sep_e_5: TMenuItem
      Caption = '-'
    end
    object PopCurFnamItem: TMenuItem
      Action = RefCurFNameAction
    end
    object PopCurPathItem: TMenuItem
      Action = RefCurPathAction
    end
    object PopOppPathItem: TMenuItem
      Action = RefOppPathAction
    end
  end
  object ScrollTimer: TTimer
    Enabled = False
    OnTimer = ScrollTimerTimer
    Left = 216
    Top = 80
  end
  object BlinkTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = BlinkTimerTimer
    Left = 288
    Top = 80
  end
  object FontDlg: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Left = 80
    Top = 80
  end
end
