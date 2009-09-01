# Overlay payload for Mac OS X.

include(../bundle.pri)

TARGET = MumbleOverlay
QMAKE_INFO_PLIST = overlay.plist

SOURCES = mach_override.c overlay.m
HEADERS = mach_override.h
DIST = overlay.plist

QMAKE_LFLAGS = -framework CoreFoundation -framework Carbon

CONFIG(debug, debug|release) {
        DESTDIR       = ../../debug/MumbleOverlayEnabler/
}

CONFIG(release, debug|release) {
        DESTDIR       = ../../release/MumbleOverlayEnabler/
}

