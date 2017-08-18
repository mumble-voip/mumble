@echo off
cls
echo 1. Build for release
echo 2. Build for development

CHOICE /C 12 /M "Enter your choice:"

IF ERRORLEVEL 2 GOTO ConfigDev
IF ERRORLEVEL 1 GOTO ConfigRelease

:ConfigRelease
echo ConfigRelease begin
call git submodule init && ^
call git submodule update && ^
call qmake -recursive main.pro CONFIG+="release static no-game-plugins no-elevation no-g15 no-asio no-overlay no-server" CONFIG-=sse2 DEFINES+="NO_UPDATE_CHECK NO_CRASH_REPORT PLUTOVR_BUILD"
goto Done

:ConfigDev
call git submodule init && ^
call git submodule update && ^
call qmake -recursive main.pro CONFIG+="static no-game-plugins no-elevation no-g15 no-asio no-overlay no-server" CONFIG-=sse2 DEFINES+="DEBUG NO_UPDATE_CHECK NO_CRASH_REPORT PLUTOVR_BUILD"

:Done
echo nmake
call nmake
