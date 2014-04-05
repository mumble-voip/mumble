# Simple project file to build overlay with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG *= debug_and_release
TARGET = mumble$(TARGET_ADD)
VERSION = 1.3.0
SOURCES = overlay.c
LIBS *= -lrt -ldl
QMAKE_CFLAGS *= -fvisibility=hidden $(CFLAGS_ADD)
QMAKE_LFLAGS -= -Wl,--no-undefined
QMAKE_LFLAGS *= $(LFLAGS_ADD)
equals(QMAKE_LINK,g++) {
  message(Overriding linker)
  QMAKE_LINK = gcc
  QMAKE_LINK_SHLIB = gcc
}

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug$(DESTDIR_ADD)
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release$(DESTDIR_ADD)
}
