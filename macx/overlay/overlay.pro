# Overlay payload for Mac OS X.

include(../../compiler.pri)

CONFIG += x86_64 x86 debug_and_release plugin
CONFIG(universal) {
	CONFIG += ppc
}

TEMPLATE = lib
CONFIG -= gui qt

CONFIG(static) {
	CONFIG -= static
}

TARGET = mumbleoverlay

QMAKE_LFLAGS_PLUGIN += -undefined dynamic_lookup -dynamic
QMAKE_LFLAGS = -framework CoreFoundation

INCLUDEPATH *= ../../3rdparty/mach-override-src

LIBS *= -lmach-override

OBJECTIVE_SOURCES = overlay.m
HEADERS = avail.h
DIST = overlay.plist avail.pl

CONFIG(debug, debug|release) {
  QMAKE_LIBDIR *= ../../debug
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LIBDIR *= ../../release
  DESTDIR       = ../../release
}

include(../../symbols.pri)
