TEMPLATE = lib
CONFIG -= qt
CONFIG += dll shared debug_and_release warn_on
CONFIG -= embed_manifest_dll
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp opengl.cpp
DIST = overlay.h

DEFINES -= UNICODE
QMAKE_CXX 		= icl
QMAKE_CXXFLAGS		+= -Qrestrict 
QMAKE_CXXFLAGS_RELEASE	= -O3 -Qip -QxK -MT
QMAKE_CXXFLAGS_DEBUG	= -Ob0 -Zi -MTd

LIBS += -ldxguid -luuid -lole32 -luser32
LIBPATH += /dev/WinSDK/Lib/i386 /dev/dxsdk/Lib/x86

INCLUDEPATH	+= /dev/WinSDK/Include /dev/dxsdk/Include /dev/Boost/include/boost-1_34_1

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../release
}
