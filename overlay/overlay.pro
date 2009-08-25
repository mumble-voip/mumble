include (../compiler.pri)

TEMPLATE = lib
CONFIG -= qt
CONFIG *= dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp d3d10.cpp opengl.cpp
DIST = overlay.h

DEFINES -= UNICODE

QMAKE_CXXFLAGS_RELEASE	-= -MD
QMAKE_CXXFLAGS_DEBUG	-= -MDd

QMAKE_CXXFLAGS_RELEASE	*= -MT
QMAKE_CXXFLAGS_DEBUG	*= -MTd

INCLUDEPATH *= "$$(DXSDK_DIR)Include"

LIBS *= -l"$$(DXSDK_DIR)Lib/x86/dxguid" -luuid -lole32 -luser32

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
  DEFINES	*= DEBUG
}
