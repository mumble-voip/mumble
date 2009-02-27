# Overlay bundle for OSX overlay

include(../bundle.pri)

TARGET = Overlay
QMAKE_INFO_PLIST = overlay.plist

SOURCES = mach_override.c overlay.m
HEADERS = mach_override.h
DIST = overlay.plist

QMAKE_LFLAGS = -framework CoreFoundation -framework Carbon
