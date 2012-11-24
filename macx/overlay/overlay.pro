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

SOURCES = mach_override.c
OBJECTIVE_SOURCES = overlay.m
HEADERS = mach_override.h avail.h
DIST = overlay.plist avail.pl

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}

include(../../symbols.pri)
