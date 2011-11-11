include (../compiler.pri)

TEMPLATE = lib
CONFIG -= qt
CONFIG *= dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp d3d10.cpp d3d11.cpp ods.cpp opengl.cpp HardHook.cpp D11StateBlock.cpp
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
LIBS *= -l"$$(DXSDK_DIR)Lib/x86/d3dcompiler"

EFFECTS11DIR = $$(DXSDK_DIR)Samples/C++/Effects11/
EFFECTS11PATH = Inc Binary
EFFECTS11SOURCES = EffectAPI.cpp EffectLoad.cpp EffectNonRuntime.cpp EffectReflection.cpp EffectRuntime.cpp d3dx11dbg.cpp d3dxGlobal.cpp

SOURCES *= $$join(EFFECTS11SOURCES," $$EFFECTS11DIR","$$EFFECTS11DIR")
INCLUDEPATH *= $$join(EFFECTS11PATH," $$EFFECTS11DIR","$$EFFECTS11DIR")

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
  DEFINES	*= DEBUG
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

