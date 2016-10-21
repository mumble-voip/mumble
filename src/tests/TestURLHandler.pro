include(../../compiler.pri)

QT += testlib

TEMPLATE = app
TARGET = testurlhandler
CONFIG += warn_on

VPATH += .
VPATH += ..
VPATH += ../mumble
INCLUDEPATH += ..
INCLUDEPATH += ../mumble

HEADERS += URLHandler.h \
	Channel.h
SOURCES += TestURLHandler.cpp \
	URLHandler.cpp \
	Channel.cpp
