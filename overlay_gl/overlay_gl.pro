# Simple project file to build Speex with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG += release
TARGET = mumble_ol
SOURCES = overlay.cpp

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
