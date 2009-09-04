# Overlay payload for Mac OS X.

CONFIG += x86_64

CONFIG(input-manager) {
	include(../bundle.pri)

	TARGET = MumbleOverlay
	QMAKE_INFO_PLIST = overlay.plist

	CONFIG(debug, debug|release) {
		DESTDIR = ../../debug/MumbleOverlayEnabler/
	}
	CONFIG(release, debug|release) {
	        DESTDIR = ../../release/MumbleOverlayEnabler/
	}
} else {
	include(../../compiler.pri)

	TEMPLATE = lib
	CONFIG += plugin
	CONFIG -= gui qt

	TARGET = mumbleoverlay

	QMAKE_LFLAGS_PLUGIN += -undefined dynamic_lookup -dynamic

	CONFIG(debug, debug|release) {
		DESTDIR = ../../debug/
        }
        CONFIG(release, debug|release) {
                DESTDIR = ../../release/
        }
}

SOURCES = mach_override.c overlay.m
HEADERS = mach_override.h
DIST = overlay.plist

QMAKE_LFLAGS = -framework CoreFoundation
