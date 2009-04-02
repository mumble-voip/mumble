# Mac OS X context menu plugin for OSX overlay

include(../bundle.pri)

TARGET = MumbleOverlayContextMenu
QMAKE_INFO_PLIST = contextmenu.plist

QMAKE_BUNDLE_EXTENSION = .plugin

SOURCES = contextmenu.c
HEADERS = contextmenu.h
DIST = contextmenu.plist

QMAKE_LFLAGS = -framework CoreFoundation -framework Carbon
