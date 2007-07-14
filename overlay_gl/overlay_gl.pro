# Simple project file to build Speex with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
TARGET = mumble
VERSION = 1.0.0
SOURCES = overlay.c
LIBS += -lrt -ldl
QMAKE_CFLAGS += -fvisibility=hidden

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
