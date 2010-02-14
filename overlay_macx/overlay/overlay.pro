# Overlay payload for Mac OS X.

include(../../compiler.pri)

CONFIG += x86_64 i386 ppc

TEMPLATE = lib
CONFIG += plugin
CONFIG -= gui qt

TARGET = mumbleoverlay

QMAKE_LFLAGS_PLUGIN += -undefined dynamic_lookup -dynamic
QMAKE_LFLAGS = -framework CoreFoundation

SOURCES = mach_override.c overlay.m
HEADERS = mach_override.h
DIST = overlay.plist

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}

include(../../symbols.pri)
