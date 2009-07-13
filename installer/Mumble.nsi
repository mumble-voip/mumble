; Mumble Installer Script for NSIS

XPStyle on
SetCompressor /SOLID lzma

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "LogicLib.nsh"
  !include "Library.nsh"
  !include "WinVer.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Mumble"
  OutFile "Mumble.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Mumble"

  ;Get installation folder from registry if available
  InstallDirRegKey HKLM "Software\Mumble" ""

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER
  
  InstType "$(INSTALL_FULL)"
  InstType "$(INSTALL_CLIENT)"
  InstType "$(INSTALL_SERVER)"

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

  ;Remember the installer language
  !define MUI_LANGDLL_REGISTRY_ROOT "HKLM" 
  !define MUI_LANGDLL_REGISTRY_KEY "Software\Mumble" 
  !define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Mumble"
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !define MUI_FINISHPAGE_SHOWREADME
  !define MUI_FINISHPAGE_SHOWREADME_TEXT $(MUMBLE_CREATE_SHORTCUT)
  !define MUI_FINISHPAGE_SHOWREADME_FUNCTION Desktop_Shortcut
  
  !define MUI_FINISHPAGE_RUN $INSTDIR\mumble.exe
  !define MUI_FINISHPAGE_RUN_TEXT $(MUMBLE_START)
  !define MUI_FINISHPAGE_RUN_FUNCTION Run_Mumble

  !define MUI_FINISHPAGE_LINK $(MUMBLE_DONATE)
  !define MUI_FINISHPAGE_LINK_LOCATION "https://sourceforge.net/project/project_donations.php?group_id=147372"

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "gpl.txt"
Page custom PageReinstall PageLeaveReinstall
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER
  !insertmacro MUI_PAGE_INSTFILES

  !define MUI_PAGE_CUSTOMFUNCTION_SHOW Finish_Show
  !insertmacro MUI_PAGE_FINISH

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_COMPONENTS
  !insertmacro MUI_UNPAGE_INSTFILES
  !insertmacro MUI_UNPAGE_FINISH


;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Spanish"
  
  
  !include "MumbleEnglish.nsh"
  !include "MumbleFrench.nsh"
  !include "MumbleGerman.nsh"
  !include "MumbleJapanese.nsh"
  !include "MumbleNorwegian.nsh"
  !include "MumblePolish.nsh"
  !include "MumbleSpanish.nsh"
  
  !insertmacro MUI_RESERVEFILE_LANGDLL
  
  ReserveFile "${NSISDIR}\Plugins\FindProcUnicode.dll"
  ReserveFile "${NSISDIR}\Plugins\CPUFeatures.dll"

;--------------------------------
;Installer Sections

Section "" SectionCommon
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  FindProcUnicode::FindProc "dbus-daemon.exe"

  IntCmp 0 $R0 NoDBusRunning
  MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON1 $(MUMBLE_TERMINATE_DBUS) IDNO NoDBusRunning
  FindProcUnicode::KillProc "dbus-daemon.exe"
 NoDBusRunning:

  File /oname=license.txt "gpl.txt"
  File /oname=Readme.txt "..\README"
  File /oname=Changes.txt "..\CHANGES"
  File "speex.txt"
  File "qt.txt"

  File "\dev\Qt4.5.2\lib\QtCore4.dll"
  File "\dev\Qt4.5.2\lib\QtGui4.dll"
  File "\dev\Qt4.5.2\lib\QtNetwork4.dll"
  File "\dev\Qt4.5.2\lib\QtSql4.dll"
  File "\dev\Qt4.5.2\lib\QtOpenGL4.dll"
  File "\dev\Qt4.5.2\lib\QtXml4.dll"
  File "\dev\Qt4.5.2\lib\QtDBus4.dll"
  File "\dev\Qt4.5.2\lib\QtSvg4.dll"

  File "\dev\OpenSSL\bin\libeay32.dll"
  File "\dev\OpenSSL\bin\ssleay32.dll"

  File "\dev\MySQL\lib\opt\libmysql.dll"
  
  File "\dev\libsndfile\libsndfile-1.dll"

  File "\dev\dbus\bin\dbus-1.dll"
  File "\dev\dbus\bin\libxml2.dll"
  File "\dev\dbus\bin\iconv.dll"
  File "\dev\dbus\bin\zlib1.dll"
  File "\dev\dbus\bin\dbus-daemon.exe"
  File "\dev\dbus\etc\session.conf"

  File "\Program Files (x86)\Intel\Compiler\11.0\075\cpp\Bin\ia32\libmmd.dll"

  File "Microsoft.VC90.CRT.manifest"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcm90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcp90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcr90.dll"

  SetOutPath "$INSTDIR\iconengines"
  File /x q*d4.dll "\dev\Qt4.5.2\plugins\iconengines\q*4.dll"
  
  SetOutPath "$INSTDIR\imageformats"
  File /x q*d4.dll "\dev\Qt4.5.2\plugins\imageformats\q*4.dll"

  SetOutPath "$INSTDIR\codecs"
  File /x q*d4.dll "\dev\Qt4.5.2\plugins\codecs\q*4.dll"

  ;Store installation folder
  WriteRegStr HKLM "Software\Mumble" "" $INSTDIR
  WriteRegStr HKLM "Software\Mumble\Mumble" "InstPath" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"

  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "DisplayName" "Mumble and Murmur"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "UninstallString" "$INSTDIR\Uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "InstallLocation" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "Publisher" "Mumble"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "InstallSource" "http://prdownloads.sourceforge.net/mumble/Mumble-1.2.0.exe?download"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "URLInfoAbout" "http://mumble.sourceforge.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "DisplayVersion" "1.2.0"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "NoRepair" 1

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application

    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_README_LNK).lnk" "$INSTDIR\Readme.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_LICENSE_LNK).lnk" "$INSTDIR\license.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_QT_LNK).lnk" "$INSTDIR\qt.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_UNINSTALL_LNK).lnk" "$INSTDIR\Uninstall.exe"

  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section "!$(MUMBLE_SEC_MUMBLE)" SectionMumble
  SectionIn 1 2
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\mumble.exe"
  File "..\release\mumble-g15-helper.exe"
  !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "..\release\mumble_ol.dll" "$INSTDIR\mumble_ol.dll" "$INSTDIR"

  SetOutPath "$INSTDIR\plugins"
  File /oname=aoc.dll "..\release\plugins\aoc.dll"
  File /oname=bf2.dll "..\release\plugins\bf2.dll"
  File /oname=bf2142.dll "..\release\plugins\bf2142.dll"
  File /oname=cod2.dll "..\release\plugins\cod2.dll"
  File /oname=cod4.dll "..\release\plugins\cod4.dll"
  File /oname=cod5.dll "..\release\plugins\cod5.dll"
  File /oname=css.dll "..\release\plugins\css.dll"
  File /oname=dods.dll "..\release\plugins\dods.dll"
  File /oname=dys.dll "..\release\plugins\dys.dll"
  File /oname=gmod.dll "..\release\plugins\gmod.dll"
  File /oname=hl2dm.dll "..\release\plugins\hl2dm.dll"
  File /oname=insurgency.dll "..\release\plugins\insurgency.dll"
  File /oname=link.dll "..\release\plugins\link.dll"
  File /oname=l4d.dll "..\release\plugins\l4d.dll"
  File /oname=tf2.dll "..\release\plugins\tf2.dll"
  File /oname=wolfet.dll "..\release\plugins\wolfet.dll"  
  ;File /oname=wow.dll "..\release\plugins\wow.dll"

  File "Microsoft.VC90.CRT.manifest"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcm90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcp90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcr90.dll"

  SetOutPath "$INSTDIR"

  ;Register URL Handler
  WriteRegStr HKCR "mumble" "" "URL:Mumble"
  WriteRegStr HKCR "mumble" "URL Protocol" ""
  WriteRegStr HKCR "mumble\DefaultIcon" "" "$INSTDIR\mumble.exe"
  WriteRegStr HKCR "mumble\shell\open\command" "" "$\"$INSTDIR\mumble.exe$\" $\"%1$\""

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Mumble.lnk" "$INSTDIR\mumble.exe"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_SPEEX_LNK).lnk" "$INSTDIR\speex.txt"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section /o "$(MUMBLE_SEC_MUMBLE11X)" SectionMumble11X
  SectionIn 1 2
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\mumble11x.exe"

  SetOutPath "$INSTDIR\plugins11x"
  File /oname=aoc.dll "..\release\plugins11x\aoc.dll"
  File /oname=bf2.dll "..\release\plugins11x\bf2.dll"
  File /oname=cod2.dll "..\release\plugins11x\cod2.dll"
  File /oname=cod4.dll "..\release\plugins11x\cod4.dll"
  File /oname=cod5.dll "..\release\plugins11x\cod5.dll"
  File /oname=css.dll "..\release\plugins11x\css.dll"
  File /oname=dods.dll "..\release\plugins11x\dods.dll"
  File /oname=dys.dll "..\release\plugins11x\dys.dll"
  File /oname=gmod.dll "..\release\plugins11x\gmod.dll"
  File /oname=hl2dm.dll "..\release\plugins11x\hl2dm.dll"
  File /oname=insurgency.dll "..\release\plugins11x\insurgency.dll"
  File /oname=link.dll "..\release\plugins11x\link.dll"
  File /oname=l4d.dll "..\release\plugins11x\l4d.dll"
  File /oname=tf2.dll "..\release\plugins11x\tf2.dll"
  File /oname=wolfet.dll "..\release\plugins11x\wolfet.dll"  

  File "Microsoft.VC90.CRT.manifest"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcm90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcp90.dll"
  File "\Program Files (x86)\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\msvcr90.dll"

  SetOutPath "$INSTDIR"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\$(MUMBLE_COMPAT_LNK).lnk" "$INSTDIR\mumble11x.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section /o "$(MUMBLE_SEC_MURMUR)" SectionMurmur
  SectionIn 1 3
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\murmur.exe"

  SetOverwrite off
  File /oname=murmur.ini "..\scripts\murmur.ini.win32"
  SetOverwrite on
  File "..\src\murmur\Murmur.ice"

  File "\dev\dbus\bin\dbus-send.exe"

  File "\dev\Ice\bin\ice33.dll"
  File "\dev\Ice\bin\iceutil33.dll"
  File "\dev\Ice\bin\bzip2.dll"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Murmur.lnk" "$INSTDIR\murmur.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section /o "$(MUMBLE_SEC_DEBUG)" SectionDebug
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\mumble.pdb"
  File "..\release\murmur.pdb"
  File "..\release\mumble-g15-helper.pdb"
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionMumble} $(DESC_SectionMumble)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionMurmur} $(DESC_SectionMurmur)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionDebug} $(DESC_SectionDebug)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "un.$(MUMBLE_UNSEC_BASE)" SectionUninstBase
  SectionIn RO
  SetShellVarContext all

  FindProcUnicode::KillProc "$INSTDIR\mumble.exe"
  FindProcUnicode::KillProc "$INSTDIR\mumble11x.exe"
  FindProcUnicode::KillProc "$INSTDIR\murmur.exe"
  FindProcUnicode::KillProc "$INSTDIR\mumble-g15-helper.exe"
  FindProcUnicode::KillProc "$INSTDIR\dbus-daemon.exe"

  Delete "$INSTDIR\mumble.exe"
  Delete "$INSTDIR\mumble11x.exe"
  Delete "$INSTDIR\murmur.exe"
  Delete "$INSTDIR\mumble-g15-helper.exe"
  Delete "$INSTDIR\mumble.pdb"
  Delete "$INSTDIR\murmur.pdb"
  Delete "$INSTDIR\mumble-g15-helper.pdb"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\mumble_ol.dll"
  Delete "$INSTDIR\qos.reg"
  Delete "$INSTDIR\murmur.ini"

  Delete "$INSTDIR\plugins\aoc.dll"
  Delete "$INSTDIR\plugins\bf2.dll"
  Delete "$INSTDIR\plugins\bf2142.dll"
  Delete "$INSTDIR\plugins\cod2.dll"
  Delete "$INSTDIR\plugins\cod4.dll"
  Delete "$INSTDIR\plugins\cod5.dll"
  Delete "$INSTDIR\plugins\css.dll"
  Delete "$INSTDIR\plugins\dods.dll"
  Delete "$INSTDIR\plugins\dys.dll"
  Delete "$INSTDIR\plugins\gmod.dll"
  Delete "$INSTDIR\plugins\hl2dm.dll"
  Delete "$INSTDIR\plugins\insurgency.dll"
  Delete "$INSTDIR\plugins\link.dll"
  Delete "$INSTDIR\plugins\l4d.dll"
  Delete "$INSTDIR\plugins\tf2.dll"
  Delete "$INSTDIR\plugins\wolfet.dll"  
  ;Delete "$INSTDIR\plugins\wow.dll"

  Delete "$INSTDIR\plugins11x\aoc.dll"
  Delete "$INSTDIR\plugins11x\bf2.dll"
  Delete "$INSTDIR\plugins11x\bf2142.dll"
  Delete "$INSTDIR\plugins11x\cod2.dll"
  Delete "$INSTDIR\plugins11x\cod4.dll"
  Delete "$INSTDIR\plugins11x\cod5.dll"
  Delete "$INSTDIR\plugins11x\css.dll"
  Delete "$INSTDIR\plugins11x\dods.dll"
  Delete "$INSTDIR\plugins11x\dys.dll"
  Delete "$INSTDIR\plugins11x\gmod.dll"
  Delete "$INSTDIR\plugins11x\hl2dm.dll"
  Delete "$INSTDIR\plugins11x\insurgency.dll"
  Delete "$INSTDIR\plugins11x\link.dll"
  Delete "$INSTDIR\plugins11x\l4d.dll"
  Delete "$INSTDIR\plugins11x\tf2.dll"
  Delete "$INSTDIR\plugins11x\wolfet.dll"  
  ;Delete "$INSTDIR\plugins11x\wow.dll"
  
  Delete "$INSTDIR\Murmur.ice"

  Delete "$INSTDIR\plugins\Microsoft.VC90.CRT.manifest"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\plugins\msvcm90.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\plugins\msvcp90.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\plugins\msvcr90.dll"

  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\QtDBus4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  
  RMDir /r "$INSTDIR\iconengines"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\codecs"

  Delete "$INSTDIR\libmySQL.dll"

  Delete "$INSTDIR\dbus-1.dll"
  Delete "$INSTDIR\libxml2.dll"
  Delete "$INSTDIR\iconv.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\dbus-daemon.exe"
  Delete "$INSTDIR\dbus-send.exe"
  Delete "$INSTDIR\session.conf"

  Delete "$INSTDIR\libsndfile-1.dll"

  Delete "$INSTDIR\libmmd.dll"

  Delete "$INSTDIR\ice33.dll"
  Delete "$INSTDIR\iceutil33.dll"
  Delete "$INSTDIR\bzip2.dll"

  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"

  Delete "$INSTDIR\Microsoft.VC90.CRT.manifest"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\msvcm90.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\msvcp90.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\msvcr90.dll"

  Delete "$INSTDIR\Changes.txt"
  Delete "$INSTDIR\qt.txt"
  Delete "$INSTDIR\speex.txt"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR\etc"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR\plugins11x"
  RMDir "$INSTDIR"

  Delete "$DESKTOP\Mumble.lnk"
  Delete "$DESKTOP\Murmur.lnk"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_UNINSTALL_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Mumble.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Murmur.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_README_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_LICENSE_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_QT_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_SPEEX_LNK).lnk"

  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"

  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."

    IfErrors startMenuDeleteLoopDone

    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

  DeleteRegKey HKCR "mumble"

  DeleteRegKey HKLM "Software\Mumble\Mumble\InstPath"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble"
  DeleteRegKey /ifempty HKLM "Software\Mumble"
SectionEnd

Section /o "un.$(MUMBLE_UNSEC_ALL)" SectionUninstAll
  SetShellVarContext all

  RMDir /r /REBOOTOK "$INSTDIR"

  DeleteRegKey HKLM "Software\Mumble"

  StrCpy $0 0
  registryDeleteLoop:
    EnumRegKey $1 HKU "" $0
    StrCmp $1 "" registryDeleteLoopDone
    IntOp $0 $0 + 1
    StrCpy $2 "$1\Software\Mumble"
    DeleteRegKey HKU $2
    
    StrCpy $2 "$1\Software\Microsoft\Windows\CurrentVersion\Explorer\Shell Folders"
    ReadRegStr $3 HKU $2 "AppData"
    StrCmp $3 "" registryDeleteLoop
    StrCpy $4 "$3\Mumble"
    RMDir /r /REBOOTOK $4
    
    goto registryDeleteLoop
  registryDeleteLoopDone:
SectionEnd

!insertmacro MUI_UNFUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionUninstBase} $(DESC_SectionUninstBase)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionUninstAll} $(DESC_SectionUninstAll)
!insertmacro MUI_UNFUNCTION_DESCRIPTION_END

Function PageReinstall
  ReadRegStr $R0 HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" "UninstallString"

  ${If} $R0 == ""
    Abort
  ${EndIf}

  !insertmacro MUI_HEADER_TEXT $(MUMBLE_ALREADY_INSTALLED_HEAD) $(MUMBLE_ALREADY_INSTALLED_SUBTEXT)

  nsDialogs::Create 1018
  Pop $R4

  ${NSD_CreateLabel} 0 0 100% 24u $(MUMBLE_ALREADY_INSTALLED)
  Pop $R1

  ${NSD_CreateRadioButton} 30u 50u -30u 8u $(MUMBLE_UNINSTALL)
  Pop $R2

  ${NSD_CreateRadioButton} 30u 70u -30u 8u $(MUMBLE_NO_UNINSTALL)
  Pop $R3

  SendMessage $R2 ${BM_SETCHECK} ${BST_CHECKED} 0

  ${NSD_SetFocus} $R2

  nsDialogs::Show
FunctionEnd

Function PageLeaveReinstall

  ${NSD_GetState} $R2 $R1
  StrCmp $R1 "1" reinst_uninstall reinst_done

reinst_uninstall:
  HideWindow
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR'
  IfErrors no_remove_uninstaller
  IfFileExists "$INSTDIR\mumble.exe" no_remove_uninstaller
  Delete $R0
  RMDir $INSTDIR
  ; Just in case they already messed it up once.
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR\etc"
no_remove_uninstaller:
  BringToFront
reinst_done:

FunctionEnd

Function .onInit
  MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 "This is an experimental release of the 1.2 series of Mumble and Murmur, and is not compatible with clients and servers from the 1.1.x series or other 1.2 snapshots. Do you still wish to install?" IDYES +2
  Abort
  Push $R0
  CPUFeatures::hasSSE
  Pop $0
  ${IfNot} $0 == "1"
    MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 $(MUMBLE_NO_SSE) IDYES +2
    Abort
  ${EndIf}
  ${IfNot} ${AtLeastWinXP}
    MessageBox MB_YESNO|MB_ICONQUESTION|MB_DEFBUTTON2 $(MUMBLE_NO_XP) IDYES +2
    Abort
  ${EndIf}
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Function Desktop_Shortcut
  ${If} ${SectionIsSelected} ${SectionMumble}
    SetOutPath "$INSTDIR"
    CreateShortCut "$DESKTOP\Mumble.lnk" "$INSTDIR\mumble.exe"
  ${EndIf}
FunctionEnd

Function Run_Mumble
  ${If} ${SectionIsSelected} ${SectionMumble}
    Exec "$INSTDIR\mumble.exe"
  ${EndIf}
FunctionEnd

Function Finish_Show
  ${IfNot} ${SectionIsSelected} ${SectionMumble}
	ShowWindow $mui.Finishpage.Run ${SW_HIDE}
	ShowWindow $mui.Finishpage.ShowReadme ${SW_HIDE}
  ${EndIf}
FunctionEnd
