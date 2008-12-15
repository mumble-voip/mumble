# Build rules for G15 helper daemon

include (../compiler.pri)

TEMPLATE = app
CONFIG -= qt
CONFIG *= debug_and_release warn_on
TARGET = mumble-g15-helper

SOURCES = g15helper.c
HEADERS = g15helper.h
RC_FILE = g15helper.rc
LIBS *= -llgLcd -ladvapi32 -lshell32 -luser32
LIBPATH *= /dev/G15SDK/SDK/Libs/x86
INCLUDEPATH *= /dev/G15SDK/SDK/Src

CONFIG(release, debug|release) {
  DESTDIR = ../release
}

CONFIG(debug, debug|release) {
  DEFINES += USE_LOGFILE
  DESTDIR = ../release
}
