# Overlay app-launcher for Mac OS X

include(../app.pri)

TARGET = mumble-overlay
SOURCES = launcher.m
QMAKE_LFLAGS += -framework ApplicationServices -framework Cocoa
