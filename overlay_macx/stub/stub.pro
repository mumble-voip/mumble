# Stub bundle for OSX overlay

include(../bundle.pri)

TARGET = Stub
QMAKE_INFO_PLIST = stub.plist

SOURCES = stub.c
HEADERS = stub.h
DIST = stub.plist

QMAKE_LFLAGS = -framework CoreFoundation -framework Carbon
