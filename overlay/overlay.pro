include (../compiler.pri)

TEMPLATE = lib
CONFIG -= qt
CONFIG *= dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp d3d10.cpp d3d11.cpp ods.cpp opengl.cpp HardHook.cpp
HEADERS = lib.h ods.h HardHook.h overlay_blacklist.h
EFFECTS = overlay.fx
DIST = overlay.h overlay.fx HardHook.h

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= "$$(DXSDK_DIR)Include"

LIBS *= -l"$$(DXSDK_DIR)Lib/x86/dxguid" -luuid -lole32 -luser32 -ladvapi32

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
  DEFINES	*= DEBUG
}

CONFIG(release, debug|release) {
  #QMAKE_LIBDIR *= $$(DXSDK_DIR)/Samples/C++/Effects11/Release
  QMAKE_LIBDIR *= Effects11/Release
  LIBS *= -lEffects11 -l"$$(DXSDK_DIR)Lib/x86/d3dcompiler"
}

CONFIG(debug, debug|release) {
  #QMAKE_LIBDIR *= $$(DXSDK_DIR)/Samples/C++/Effects11/Debug
  QMAKE_LIBDIR *= Effects11/Debug
  LIBS *= -lEffects11 -l"$$(DXSDK_DIR)Lib/x86/d3dcompiler"
}

fxc.output = ${QMAKE_FILE_BASE}.hex
fxc.commands = fxc /Tfx_4_0 /O3 /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
fxc.input = EFFECTS
fxc.CONFIG *= no_link target_predeps

fxc11.output = ${QMAKE_FILE_BASE}11.hex
fxc11.commands = fxc /Tfx_5_0 /Emain11 /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
fxc11.input = EFFECTS
fxc11.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= fxc fxc11

