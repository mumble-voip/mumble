TEMPLATE	=app
CONFIG  += qt thread warn_on debug
QT -= gui
LANGUAGE	= C++
TARGET = SpeexMark
SOURCES = SpeexMark.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble ../../speex/include ../../speexbuild
LIBS += -lcrypto -lspeex
QMAKE_LFLAGS += -L../../debug

