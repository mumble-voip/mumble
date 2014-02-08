# Build rules for G15 helper daemon

include (../compiler.pri)

TEMPLATE = app
CONFIG -= qt
CONFIG *= debug_and_release warn_on
TARGET = mumble-g15-helper

HEADERS = g15helper.h

win32 {
  SOURCES = g15helper.c
  RC_FILE = g15helper.rc
  LIBS *= -llgLcd -ladvapi32 -lshell32 -luser32
  QMAKE_LIBDIR *= "$$G15SDK_PATH/Lib/x86"
  INCLUDEPATH *= "$$G15SDK_PATH/Src"
  DEFINES *= WIN32
}

macx {
  CONFIG -= app_bundle x86_64
  CONFIG += x86
  SOURCES = g15helper_macx.c
  LIBS *= -llgLcd
  QMAKE_LIBDIR *= $$(MUMBLE_PREFIX)/../LCDSDK/lib/
  INCLUDEPATH *= $$(MUMBLE_PREFIX)/../LCDSDK/Src/
  QMAKE_LFLAGS += -framework CoreFoundation -sectcreate __TEXT __info_plist g15helper.plist
  DEFINES *= APPLE
  DIST = g15helper.plist
}

CONFIG(release, debug|release) {
  DESTDIR = ../release
}

CONFIG(debug, debug|release) {
  DEFINES *= USE_LOGFILE
  DESTDIR = ../debug/
}

include(../symbols.pri)
