# Simple project file to build Speex with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG *= debug_and_release
TARGET = mumble
VERSION = 1.1.5
SOURCES = overlay.c
LIBS *= -lrt -ldl
QMAKE_CFLAGS *= -fvisibility=hidden
QMAKE_LFLAGS -= -Wl,--no-undefined
equals(QMAKE_LINK,g++) {
  message(Overriding linker)
  QMAKE_LINK = gcc
  QMAKE_LINK_SHLIB = gcc
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
