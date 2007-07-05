# Simple project file to build Speex with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
TARGET = mumble_ol
SOURCES = overlay.cpp
LIBS += -lrt

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
