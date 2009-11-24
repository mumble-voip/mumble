# Overlay app-launcher for Mac OS X

include(../../compiler.pri)
include(../common.pri)

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG *= debug_and_release

TARGET = mumble-overlay
SOURCES = launcher.m
QMAKE_LFLAGS += -framework ApplicationServices -framework Cocoa -sectcreate __TEXT __info_plist launcher.plist

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}

include(../../symbols.pri)
