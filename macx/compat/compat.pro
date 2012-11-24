# Tiny stub program that we splice together into our
# main app bundle for architectures that we don't support.

include(../../compiler.pri)
include(../common.pri)

TEMPLATE = app
CONFIG -= qt app_bundle x86_64
CONFIG += debug_and_release x86
CONFIG(universal) {
	CONFIG += ppc
}

TARGET = Mumble.compat
OBJECTIVE_SOURCES = compat.m
QMAKE_LFLAGS += -framework ApplicationServices -framework Cocoa -framework AppKit

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}

include(../../symbols.pri)
