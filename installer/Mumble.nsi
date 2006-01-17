; Mumble Installer Script for NSIS

SetCompressor /SOLID lzma
; !packhdr "$%TEMP%\exehead.tmp" '"..\..\vc\upx125w\upx.exe" "-9" "$%TEMP%\exehead.tmp"'

;--------------------------------
;Include Modern UI

  !include "MUI.nsh"

;--------------------------------
;General

  ;Name and file
  Name "Mumble"
  OutFile "Mumble.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Mumble"
  
  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\Mumble" ""

;--------------------------------
;Variables

  Var MUI_TEMP
  Var STARTMENU_FOLDER

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "..\LICENSE"
  !insertmacro MUI_PAGE_LICENSE "qt.txt"
  !insertmacro MUI_PAGE_LICENSE "speex.txt"
;  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY

  ;Start Menu Folder Page Configuration
  !define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
  !define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Mumble" 
  !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
  !insertmacro MUI_PAGE_STARTMENU Application $STARTMENU_FOLDER

  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "Mumble & Murmur" SecMumble

  SetOutPath "$INSTDIR"
  
  File /oname=license.txt "..\LICENSE"
  File /oname=Readme.txt "..\README"
  File /oname=Changes.txt "..\CHANGES"
  File "speex.txt"
  File "qt.txt"
  File "..\release\mumble.exe"
  File "..\release\murmur.exe"
  File "..\overlay\mumble_ol.dll"
  SetOverwrite off
  File "..\murmur.ini"
  SetOverwrite on
  File "\qt\4.1.0os\lib\QtCore4.dll"
  File "\qt\4.1.0os\lib\QtGui4.dll"
  File "\qt\4.1.0os\lib\QtNetwork4.dll"
  File "\qt\4.1.0os\lib\QtSql4.dll"
  File "\qt\4.1.0os\lib\QtOpenGL4.dll"
  File "..\release\speex.dll"
  File "\mingw\bin\mingwm10.dll"
  File "\Program Files\FMOD SoundSystem\FMOD Programmers API\api\fmodex.dll"

  SetOutPath "$INSTDIR\plugins"
  File /oname=bf2.dll "..\plugins\bf2.dll"
  File /oname=wow.dll "..\plugins\wow.dll"

  SetOutPath "$INSTDIR"

  ;Store installation folder
  WriteRegStr HKCU "Software\Mumble" "" $INSTDIR
  WriteRegStr HKCU "Software\Mumble\Mumble" "InstPath" $INSTDIR
  
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
                 "InstallSource" "http://prdownloads.sourceforge.net/mumble/Mumble-0.9.1.exe?download"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "URLInfoAbout" "http://mumble.sourceforge.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "DisplayVersion" "0.9.1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble" \
                 "NoRepair" 1

  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$STARTMENU_FOLDER"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Mumble.lnk" "$INSTDIR\mumble.exe"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Murmur.lnk" "$INSTDIR\murmur.exe"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Mumble Readme.lnk" "$INSTDIR\Readme.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Mumble License.lnk" "$INSTDIR\license.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\QT License.lnk" "$INSTDIR\qt.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Speex License.lnk" "$INSTDIR\speex.txt"
    CreateShortCut "$SMPROGRAMS\$STARTMENU_FOLDER\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

SectionEnd

;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...

  Delete "$INSTDIR\mumble.exe"
  Delete "$INSTDIR\murmur.exe"
  Delete "$INSTDIR\mumble_ol.dll"
  Delete "$INSTDIR\murmur.ini"
  Delete "$INSTDIR\plugins\bf2.dll"
  Delete "$INSTDIR\plugins\wow.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\speex.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\fmodex.dll"
  Delete "$INSTDIR\Changes.txt"
  Delete "$INSTDIR\qt.txt"
  Delete "$INSTDIR\speex.txt"
  Delete "$INSTDIR\Readme.txt"
  Delete "$INSTDIR\license.txt"
  Delete "$INSTDIR\Uninstall.exe"

  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR"

  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_TEMP
    
  Delete "$SMPROGRAMS\$MUI_TEMP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Mumble.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Murmur.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Mumble Readme.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Mumble License.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\QT License.lnk"
  Delete "$SMPROGRAMS\$MUI_TEMP\Speex License.lnk"
  
  ;Delete empty start menu parent diretories
  StrCpy $MUI_TEMP "$SMPROGRAMS\$MUI_TEMP"
 
  startMenuDeleteLoop:
	ClearErrors
    RMDir $MUI_TEMP
    GetFullPathName $MUI_TEMP "$MUI_TEMP\.."
    
    IfErrors startMenuDeleteLoopDone
  
    StrCmp $MUI_TEMP $SMPROGRAMS startMenuDeleteLoopDone startMenuDeleteLoop
  startMenuDeleteLoopDone:

  DeleteRegKey /ifempty HKCU "Software\Mumble"
  DeleteRegKey HKCU "Software\Mumble\Mumble\InstPath"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Mumble"
SectionEnd
