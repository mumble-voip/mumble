@echo off
cls
echo 1. Setup to use Speex git submodule
echo 2. Configure for release
echo 3. Configure for development

CHOICE /C 123 /M "Enter your choice:"

IF ERRORLEVEL 3 GOTO ConfigDev
IF ERRORLEVEL 2 GOTO ConfigRelease
IF ERRORLEVEL 1 GOTO SetupSpeex

:SetupSpeex
git submodule init
git submodule update
goto Done

:ConfigRelease
qmake -recursive main.pro CONFIG+="release static no-elevation no-g15 no-asio" CONFIG-=sse2 DEFINES+="NO_UPDATE_CHECK NO_CRASH_REPORT PLUTOVR_BUILD"
goto Done

:ConfigDev
qmake -recursive main.pro CONFIG+="static no-elevation no-g15 no-asio" CONFIG-=sse2 DEFINES+="DEBUG NO_UPDATE_CHECK NO_CRASH_REPORT PLUTOVR_BUILD"

:Done
