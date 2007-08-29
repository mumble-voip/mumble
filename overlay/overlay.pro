TEMPLATE = lib
CONFIG -= qt
CONFIG += dll shared debug_and_release warn_on precompile_header
VERSION =
TARGET = mumble_ol
SOURCES = lib.cpp d3d9.cpp dsound.cpp openal.cpp

LIBS += -ldxguid -luuid -lole32

INCLUDEPATH	+= /dev/dxsdk/Include "/Program Files/OpenAL 1.1 SDK/Include"

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
