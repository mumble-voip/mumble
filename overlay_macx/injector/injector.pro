# Overlay injector for Mac OS X

include(../app.pri)

TARGET = mumble-overlay-injector
SOURCES = mach_inject.c mumble-overlay-injector.c
HEADERS = mach_inject.h ../stub/stub.h
QMAKE_LFLAGS += -framework CoreFoundation -framework Carbon -framework ApplicationServices -sectcreate __TEXT __info_plist injector.plist
