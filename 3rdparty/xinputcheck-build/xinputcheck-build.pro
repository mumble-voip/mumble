include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-lib-build,-src)

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG -= warn_on
CONFIG += warn_off
CONFIG += static
VPATH = ../xinputcheck-src
TARGET = xinputcheck
INCLUDEPATH = ../$$SOURCEDIR

SOURCES *= xinputcheck.cpp

include(../../qmake/symbols.pri)
