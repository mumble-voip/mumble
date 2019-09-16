# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include (../qmake/compiler.pri)

VERSION = 1.4.0
TARGET_EXT = .dll
TEMPLATE = lib
CONFIG -= qt
CONFIG *= dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
RC_FILE = mumble_ol.rc
SOURCES = ancestor.cpp lib.cpp olsettings.cpp excludecheck.cpp d3d9.cpp dxgi.cpp d3d10.cpp d3d11.cpp ods.cpp opengl.cpp HardHook.cpp D11StateBlock.cpp
HEADERS = ancestor.h lib.h olsettings.h excludecheck.h ods.h HardHook.h overlay_blacklist.h D11StateBlock.h ../3rdparty/GL/glext.h
EFFECTS = overlay.fx
DX11_PIXEL_SHADERS = overlay11.ps
DX11_VERTEX_SHADERS = overlay11.vs

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE -= -MD
QMAKE_CXXFLAGS_DEBUG -= -MDd

QMAKE_CXXFLAGS_RELEASE *= -MT
QMAKE_CXXFLAGS_DEBUG *= -MTd

LIBS *= -ldxguid -luuid -lole32 -luser32 -ladvapi32
LIBS *= -ld3d9 -ld3d10 -ld3d11 -ld3dcompiler -ldxgi

CONFIG(force-x86_64-toolchain) {
  DEFINES += USE_MINHOOK
  INCLUDEPATH *= ../3rdparty/minhook-src/include
  LIBS *= -lminhook
}

CONFIG(debug, debug|release) {
  DEFINES *= DEBUG
}

QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR

# Override fxc binary for the x86 build.
CONFIG(force-x86-toolchain) {
  exists($$DXSDK_DIR) {
    FXC = "\"$$(DXSDK_DIR)\\Utilities\\bin\\x86\\fxc.exe\""
  } else {
    FXC = fxc.exe
  }
} else {
  FXC = fxc.exe
}

fxc.output = ${QMAKE_FILE_BASE}.hex
fxc.commands = $${FXC} /Tfx_4_0 /O3 /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
fxc.input = EFFECTS
fxc.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= fxc

vs11.output = ${QMAKE_FILE_BASE}.ps.h
vs11.commands = $${FXC} /Tvs_5_0 /O3 /Vng_vertex_shader /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
vs11.input = DX11_VERTEX_SHADERS
vs11.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= vs11

ps11.output = ${QMAKE_FILE_BASE}.vs.h
ps11.commands = $${FXC} /Tps_5_0 /O3 /Vng_pixel_shader /Fh${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
ps11.input = DX11_PIXEL_SHADERS
ps11.CONFIG *= no_link target_predeps
QMAKE_EXTRA_COMPILERS *= ps11
