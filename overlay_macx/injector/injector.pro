# Overlay injector for Mac OS X

include(../../compiler.pri)

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG *= debug_and_release

TARGET = mumble-overlay-injector
SOURCES = mach_inject.c mumble-overlay-injector.c
HEADERS = mach_inject.h ../stub/stub.h
QMAKE_LFLAGS += -framework CoreFoundation -framework Carbon -framework ApplicationServices

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}
