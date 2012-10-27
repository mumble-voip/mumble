include(../../compiler.pri)

TEMPLATE = app
CONFIG += qt thread warn_on debug console
CONFIG -= app_bundle
QT += network gui
LANGUAGE = C++
TARGET = OverlayTest
HEADERS = ../mumble/SharedMemory.h
SOURCES = OverlayTest.cpp ../mumble/SharedMemory.cpp
win32 {
	SOURCES += ../mumble/SharedMemory_win.cpp
} else {
	SOURCES += ../mumble/SharedMemory_unix.cpp
}

VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
