include(../bundle.pri)

TARGET = MumbleOverlayEnabler
QMAKE_INFO_PLIST = MumbleOverlayEnabler.plist

SOURCES = MumbleOverlayEnabler.m
HEADERS = MumbleOverlayEnabler.h
DIST = MumbleOverlayEnabler.plist

QMAKE_LFLAGS = -framework Cocoa

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug/MumbleOverlayEnabler/
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release/MumbleOverlayEnabler/
}

EXTRA_FILES *= Info

extra.output = $${DESTDIR}${QMAKE_FILE_NAME}
extra.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
extra.input = EXTRA_FILES
extra.CONFIG *= no_link target_predeps

QMAKE_EXTRA_COMPILERS *= extra
