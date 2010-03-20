; Mumble Installer Script for NSIS

XPStyle on
SetCompressor /SOLID lzma
SetPluginUnload alwaysoff
RequestExecutionLevel user

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include "LogicLib.nsh"
  !include "Library.nsh"
  !include "WinVer.nsh"
  !include "FileFunc.nsh"

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
  !insertmacro MUI_LANGUAGE "Czech"
  !insertmacro MUI_LANGUAGE "Danish"
  !insertmacro MUI_LANGUAGE "Dutch"
  !insertmacro MUI_LANGUAGE "French"
  !insertmacro MUI_LANGUAGE "German"
  !insertmacro MUI_LANGUAGE "Italian"
  !insertmacro MUI_LANGUAGE "Japanese"
  !insertmacro MUI_LANGUAGE "Norwegian"
  !insertmacro MUI_LANGUAGE "Polish"
  !insertmacro MUI_LANGUAGE "Spanish"
  !insertmacro MUI_LANGUAGE "SimpChinese"
  !insertmacro MUI_LANGUAGE "TradChinese"
  
  
  !include "MumbleEnglish.nsh"
  !include "MumbleCzech.nsh"
  !include "MumbleDanish.nsh"
  !include "MumbleDutch.nsh"
  !include "MumbleFrench.nsh"
  !include "MumbleGerman.nsh"
  !include "MumbleItalian.nsh"
  !include "MumbleJapanese.nsh"
  !include "MumbleNorwegian.nsh"
  !include "MumblePolish.nsh"
  !include "MumbleSimpChinese.nsh"
  !include "MumbleTradChinese.nsh"
  !include "MumbleSpanish.nsh"
  
  !insertmacro MUI_RESERVEFILE_LANGDLL
  
  ReserveFile "${NSISDIR}\Plugins\FindProcUnicode.dll"
  ReserveFile "${NSISDIR}\Plugins\CPUFeatures.dll"
  ReserveFile "${NSISDIR}\Plugins\UAC.dll"

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

  File "..\scripts\qt.conf"
  File "\dev\Qt4.6\lib\QtCore4.dll"
  File "\dev\Qt4.6\lib\QtGui4.dll"
  File "\dev\Qt4.6\lib\QtNetwork4.dll"
  File "\dev\Qt4.6\lib\QtSql4.dll"
  File "\dev\Qt4.6\lib\QtOpenGL4.dll"
  File "\dev\Qt4.6\lib\QtXml4.dll"
  File "\dev\Qt4.6\lib\QtDBus4.dll"
  File "\dev\Qt4.6\lib\QtSvg4.dll"

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

  File "\dev\protobuf-2.3.0\vsprojects\Release\libprotobuf.dll"

  File "\Program Files (x86)\Intel\Compiler\11.1\054\Bin\ia32\libmmd.dll"
  File "\Program Files (x86)\Debugging Tools for Windows (x86)\dbghelp.dll"

  SetOutPath "$INSTDIR\QtPlugins\iconengines"
  File /x q*d4.dll "\dev\Qt4.6\plugins\iconengines\q*4.dll"
  
  SetOutPath "$INSTDIR\QtPlugins\imageformats"
  File /x q*d4.dll "\dev\Qt4.6\plugins\imageformats\q*4.dll"

  SetOutPath "$INSTDIR\QtPlugins\codecs"
  File /x q*d4.dll "\dev\Qt4.6\plugins\codecs\q*4.dll"

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
                 "InstallSource" "http://prdownloads.sourceforge.net/mumble/Mumble-1.2.3.exe?download"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "URLInfoAbout" "http://mumble.sourceforge.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "DisplayVersion" "1.2.3"
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

# Based on http://kobyk.wordpress.com/2008/04/12/deploying-the-visual-c-libraries-with-an-nsis-installer/

  InitPluginsDir
  SetOutPath $PLUGINSDIR
  File "$%WINDIR%\winsxs\Manifests\x86_policy.9.0.microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_f47e1bd6f6571810.cat"
  File "$%WINDIR%\winsxs\Manifests\x86_policy.9.0.microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_f47e1bd6f6571810.manifest"
  File "$%WINDIR%\winsxs\Manifests\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2.cat"
  File "$%WINDIR%\winsxs\Manifests\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2.manifest"
  File "$%WINDIR%\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2\msvcm90.dll"
  File "$%WINDIR%\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2\msvcp90.dll"
  File "$%WINDIR%\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2\msvcr90.dll"
  DetailPrint "Installing CRT assembly..."
  System::Call "sxs::CreateAssemblyCache(*i .r0, i 0) i.r1"
  StrCmp $1 0 0 fail
# Fill a FUSION_INSTALL_REFERENCE.
# fir.cbSize = sizeof(FUSION_INSTALL_REFERENCE) == 32
# fir.dwFlags = 0
# fir.guidScheme = FUSION_REFCOUNT_UNINSTALL_SUBKEY_GUID
# fir.szIdentifier = "nsissxs"
# fir.szNonCanonicalData = 0
  System::Call "*(i 32, i 0, i 2364391957, i 1217113163, i 178634899, i 3090139977, w 'mumble', w '') i.s"
  Pop $2
# IAssemblyCache::InstallAssembly(0, manifestPath, fir)
  System::Call "$0->7(i 0, w '$PLUGINSDIR\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_5090ab56bcba71c2.manifest', i r2) i.r1"
  StrCmp $1 0 0 fail2

  System::Call "$0->7(i 0, w '$PLUGINSDIR\x86_policy.9.0.microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.4148_none_f47e1bd6f6571810.manifest', i r2) i.r1"
  System::Free $2
  StrCmp $1 0 0 fail3

  System::Call "$0->2()"
Goto end
fail:
  DetailPrint "CreateAssemblyCache failed."
  DetailPrint $1
  Goto end
fail2:
  DetailPrint "InstallAssembly failed for CRT."
  DetailPrint $1
  Goto end
fail3:
  DetailPrint "InstallAssembly failed for Policy."
  DetailPrint $1
  Goto end
end:
SectionEnd

Section "!$(MUMBLE_SEC_MUMBLE)" SectionMumble
  SectionIn 1 2
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\mumble.exe"
  File "..\release\mumble-g15-helper.exe"
  !insertmacro InstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "..\release\mumble_ol.dll" "$INSTDIR\mumble_ol.dll" "$INSTDIR"
  File "..\release\celt0.*.dll"
  File "..\release\speex*.dll"

  SetOutPath "$INSTDIR\plugins"
  File /oname=aoc.dll "..\release\plugins\aoc.dll"
  File /oname=aoc.dll "..\release\plugins\arma2.dll"
  File /oname=bf1942.dll "..\release\plugins\bf1942.dll"
  File /oname=bf2.dll "..\release\plugins\bf2.dll"
  File /oname=bf2142.dll "..\release\plugins\bf2142.dll"
  File /oname=bf2.dll "..\release\plugins\bfbc2.dll"
  File /oname=bfheroes.dll "..\release\plugins\bfheroes.dll"
  File /oname=cod2.dll "..\release\plugins\cod2.dll"
  File /oname=cod4.dll "..\release\plugins\cod4.dll"
  File /oname=cod5.dll "..\release\plugins\cod5.dll"
  File /oname=codmw2.dll "..\release\plugins\codmw2.dll"
  File /oname=codmw2so.dll "..\release\plugins\codmw2so.dll"
  File /oname=css.dll "..\release\plugins\css.dll"
  File /oname=dods.dll "..\release\plugins\dods.dll"
  File /oname=dys.dll "..\release\plugins\dys.dll"
  File /oname=etqw.dll "..\release\plugins\etqw.dll"
  File /oname=gmod.dll "..\release\plugins\gmod.dll"
  File /oname=hl2dm.dll "..\release\plugins\hl2dm.dll"
  File /oname=insurgency.dll "..\release\plugins\insurgency.dll"
  File /oname=l4d.dll "..\release\plugins\l4d.dll"
  File /oname=l4d2.dll "..\release\plugins\l4d2.dll"
  File /oname=link.dll "..\release\plugins\link.dll"
  File /oname=lotro.dll "..\release\plugins\lotro.dll"
  File /oname=manual.dll "..\release\plugins\manual.dll"
  File /oname=tf2.dll "..\release\plugins\tf2.dll"
  File /oname=ut2004.dll "..\release\plugins\ut2004.dll"
  File /oname=ut3.dll "..\release\plugins\ut3.dll"
  File /oname=wolfet.dll "..\release\plugins\wolfet.dll"  
  File /oname=wow.dll "..\release\plugins\wow.dll"

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

Section "$(MUMBLE_SEC_MUMBLE11X)" SectionMumble11X
  SectionIn 1 2
  SetShellVarContext all

  SetOutPath "$INSTDIR"

  File "..\release\mumble11x.exe"
  File "..\release\speex.dll"

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

  File "\dev\Ice\bin\ice34.dll"
  File "\dev\Ice\bin\iceutil34.dll"
  File "\dev\Ice\bin\bzip2.dll"

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    ;Create shortcuts
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Murmur.lnk" "$INSTDIR\murmur.exe"
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

Section /o "$(MUMBLE_SEC_BONJOUR)" SectionBonjour
  SetShellVarContext all
  InitPluginsDir
  SetOutPath $PLUGINSDIR
  NSISdl::download http://support.apple.com/downloads/DL755/en_US/BonjourSetup.exe "$PLUGINSDIR\BonjourSetup.exe"
  Pop $R0
  StrCmp $R0 "success" 0 end
  ExecWait '"$PLUGINSDIR\BonjourSetup.exe" /passive /norestart' 
end:
SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionMumble} $(DESC_SectionMumble)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionMumble11X} $(DESC_SectionMumble11X)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionMurmur} $(DESC_SectionMurmur)
  !insertmacro MUI_DESCRIPTION_TEXT ${SectionBonjour} $(DESC_SectionBonjour)
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

  DetailPrint "Removing CRT assembly..."
  System::Call "sxs::CreateAssemblyCache(*i .r0, i 0) i.r1"
  StrCmp $1 0 0 fail
  System::Call "*(i 32, i 0, i 2364391957, i 1217113163, i 178634899, i 3090139977, w 'mumble', w '') i.s"
  Pop $2

  System::Call "$0->3(i 0, w 'policy.9.0.Microsoft.VC90.CRT,version=$\"9.0.30729.4148$\",type=$\"win32-policy$\",processorArchitecture=$\"x86$\",publicKeyToken=$\"1fc8b3b9a1e18e3b$\"', i r2, *i . r3) i.r1"
  StrCmp $1 0 0 fail2
  DetailPrint "Disposition returned from policy removal is $3"

  System::Call "$0->3(i 0, w 'Microsoft.VC90.CRT,version=$\"9.0.30729.4148$\",type=$\"win32$\",processorArchitecture=$\"x86$\",publicKeyToken=$\"1fc8b3b9a1e18e3b$\"', i r2, *i . r3) i.r1"
  StrCmp $1 0 0 fail2
  DetailPrint "Disposition returned from CRT removal is $3"

  System::Call "$0->2()"
  Goto end
fail:
  DetailPrint "CreateAssemblyCache failed."
  DetailPrint $1
  Goto end
fail2:
  DetailPrint "UninstallAssembly failed."
  DetailPrint $1
  Goto end
end:

  Delete "$INSTDIR\mumble.exe"
  Delete "$INSTDIR\mumble11x.exe"
  Delete "$INSTDIR\murmur.exe"
  Delete "$INSTDIR\mumble-g15-helper.exe"
  Delete "$INSTDIR\celt0.*.dll"
  Delete "$INSTDIR\speex*.dll"
  Delete "$INSTDIR\mumble_ol_old.dll"
  Rename "$INSTDIR\mumble_ol.dll" "$INSTDIR\mumble_ol_old.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\mumble_ol_old.dll"
  !insertmacro UnInstallLib DLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\mumble_ol.dll"
  Delete "$INSTDIR\qos.reg"

  ${un.Getparameters} $R1
  ${un.GetOptions} $R1 '/UPGRADE' $R0
  IfErrors 0 +2
  Delete "$INSTDIR\murmur.ini"

  Delete "$INSTDIR\Murmur.ice"

  Delete "$INSTDIR\qt.conf"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\QtDBus4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  
  RMDir /r "$INSTDIR\QtPlugins"

  Delete "$INSTDIR\libmySQL.dll"

  Delete "$INSTDIR\dbus-1.dll"
  Delete "$INSTDIR\libxml2.dll"
  Delete "$INSTDIR\iconv.dll"
  Delete "$INSTDIR\zlib1.dll"
  Delete "$INSTDIR\dbus-daemon.exe"
  Delete "$INSTDIR\dbus-send.exe"
  Delete "$INSTDIR\session.conf"

  Delete "$INSTDIR\libsndfile-1.dll"

  Delete "$INSTDIR\libprotobuf.dll"
  Delete "$INSTDIR\libmmd.dll"
  Delete "$INSTDIR\dbghelp.dll"

  Delete "$INSTDIR\ice34.dll"
  Delete "$INSTDIR\iceutil34.dll"
  Delete "$INSTDIR\bzip2.dll"

  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\ssleay32.dll"

  Delete "$INSTDIR\Changes.txt"
  Delete "$INSTDIR\qt.txt"
  Delete "$INSTDIR\speex.txt"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR\bin"
  RMDir "$INSTDIR\etc"
  RMDir /r "$INSTDIR\plugins"
  RMDir "$INSTDIR"

  Delete "$DESKTOP\Mumble.lnk"
  Delete "$DESKTOP\$(MUMBLE_COMPAT_LNK).lnk"
  Delete "$DESKTOP\Murmur.lnk"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP

  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_UNINSTALL_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Mumble.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Murmur.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_README_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_LICENSE_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_QT_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_SPEEX_LNK).lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\$(MUMBLE_COMPAT_LNK).lnk"

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
  ExecWait '$R0 /UPGRADE _?=$INSTDIR'
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
UAC_Elevate:
    UAC::RunElevated 
    StrCmp 1223 $0 UAC_ElevationAborted ; UAC dialog aborted by user?
    StrCmp 0 $0 0 UAC_Err ; Error?
    StrCmp 1 $1 0 UAC_Success ;Are we the real deal or just the wrapper?
    Quit
 
UAC_Err:
    Abort
 
UAC_ElevationAborted:
    # elevation was aborted, run as normal?
    Abort
 
UAC_Success:
    StrCmp 1 $3 +4 ;Admin?
    StrCmp 3 $1 0 UAC_ElevationAborted ;Try again?
    goto UAC_Elevate 

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

  System::Call kernel32::LoadLibrary(t"dnssd.dll")i.r0
  ${If} $0 != 0
    System::Call kernel32::FindProcAddress(i r0, t 'DNSServiceBrowse')i.r1
    ${If} $1 != 0
      SectionSetText ${SectionBonjour} ""
      !insertmacro SetSectionFlag ${SectionBonjour} ${SF_RO}
    ${EndIf}
    System::Call 'kernel32::FreeLibrary(i r0)'
  ${EndIf}
FunctionEnd

Function .OnInstFailed
    UAC::Unload
FunctionEnd
 
Function .OnInstSuccess
    UAC::Unload
FunctionEnd

Function un.onInit
  !insertmacro MUI_UNGETLANGUAGE
FunctionEnd

Function Desktop_Shortcut
  ${If} ${SectionIsSelected} ${SectionMumble}
    SetOutPath "$INSTDIR"
    CreateShortCut "$DESKTOP\Mumble.lnk" "$INSTDIR\mumble.exe"
  ${EndIf}
  ${If} ${SectionIsSelected} ${SectionMumble11X}
    SetOutPath "$INSTDIR"
    CreateShortCut "$DESKTOP\$(MUMBLE_COMPAT_LNK).lnk" "$INSTDIR\mumble11x.exe"
  ${EndIf}
FunctionEnd

Function Run_Mumble
  ${If} ${SectionIsSelected} ${SectionMumble}
    UAC::ShellExec "open" SW_SHOWNORMAL "$INSTDIR\mumble.exe" "" "$INSTDIR"
  ${EndIf}
FunctionEnd

Function Finish_Show
  ${IfNot} ${SectionIsSelected} ${SectionMumble}
	ShowWindow $mui.Finishpage.Run ${SW_HIDE}
	ShowWindow $mui.Finishpage.ShowReadme ${SW_HIDE}
  ${EndIf}
FunctionEnd
