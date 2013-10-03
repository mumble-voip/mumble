include (../compiler.pri)

VERSION = 1.2.5
TARGET_EXT = .dll
TEMPLATE = lib
CONFIG -= qt
CONFIG *= dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp d3d10.cpp d3d11.cpp ods.cpp opengl.cpp HardHook.cpp D11StateBlock.cpp
HEADERS = lib.h ods.h HardHook.h overlay_blacklist.h D11StateBlock.h
EFFECTS = overlay.fx
DIST = overlay.h overlay.fx HardHook.h
SUBDIRS *= effects11

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= "$(DXSDK_DIR)Include"
INCLUDEPATH *= "$$EFFECTS11_PATH/inc"
INCLUDEPATH *= effects11/inc

LIBS *= -l"$$(DXSDK_DIR)Lib/x86/dxguid" -luuid -lole32 -luser32 -ladvapi32
LIBS *= "c:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/*"

CONFIG(release, debug|release) {
  DESTDIR       = ../release
  #LIBS *= -l"$$EFFECTS11_PATH/Bin/Desktop_2012/Win32/Debug/Effects11"
  #LIBS *= -l"$$EFFECTS11_PATH/Bin/Desktop_DXSDK/Win32/Debug/Effects11"
  #LIBS *= effects11/release/effects11.lib
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
  DEFINES	*= DEBUG
  #LIBS *= -l"$$EFFECTS11_PATH/Bin/Desktop_2012/Win32/Release/Effects11"
  #LIBS *= -l"$$EFFECTS11_PATH/Bin/Desktop_DXSDK/Win32/Release/Effects11"
  #LIBS *= effects11/debug/effects11.lib
  LIBS *= -leffects11/debug/effects11
}

fxc.output = ${QMAKE_FILE_BASE}.hex
fxc.commands = fxc /Tfx_4_0 /O3 /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
fxc.input = EFFECTS
fxc.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= fxc

fxc11.output = ${QMAKE_FILE_BASE}11.hex
fxc11.commands = fxc /Tfx_5_0 /O3 /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
fxc11.input = EFFECTS
fxc11.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= fxc11
