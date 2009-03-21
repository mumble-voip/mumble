# Overlay app-launcher for Mac OS X

include(../app.pri)

TARGET = mumble-overlay
SOURCES = mumble-overlay.c
QMAKE_LFLAGS += -framework ApplicationServices
