# Mac OS X bundle template

include(../compiler.pri)
include(common.pri)

TEMPLATE = lib
CONFIG += plugin plugin_bundle

CONFIG -= qt
CONFIG *= debug_and_release

QMAKE_CFLAGS_RELEASE -= -Os
QMAKE_CFLAGS_RELEASE += -O0
QMAKE_CFLAGS -= -fvisibility=hidden
QMAKE_LFLAGS_PLUGIN -= -dynamiclib
QMAKE_LFLAGS_PLUGIN += -bundle -undefined dynamic_lookup -dynamic
QMAKE_LFLAGS_COMPAT_VERSION =
QMAKE_LFLAGS_VERSION =
QMAKE_LFLAGS_SONAME =
QMAKE_BUNDLE_EXTENSION = .bundle
QMAKE_BUNDLE_LOCATION = Contents/MacOS

equals(QMAKE_LINK,g++) {
  QMAKE_LINK = gcc
  QMAKE_LINK_SHLIB = gcc
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}
