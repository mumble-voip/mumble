include(../bundle.pri)

TARGET = MumbleOverlayEnabler
QMAKE_INFO_PLIST = MumbleOverlayEnabler.plist

QMAKE_BUNDLE_EXTENSION = .bundle

SOURCES = MumbleOverlayEnabler.m
HEADERS = MumbleOverlayEnabler.h
DIST = MumbleOverlayEnabler.plist

QMAKE_LFLAGS = -framework Cocoa
