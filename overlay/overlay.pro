TEMPLATE = lib
CONFIG -= qt
CONFIG += dll shared debug_and_release warn_on precompile_header
VERSION =
TARGET = mumble_ol
SOURCES = lib.cpp

QMAKE_CFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
QMAKE_CXXFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare

CONFIG(debug, debug|release) {
  DESTDIR       = ../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../release
}
