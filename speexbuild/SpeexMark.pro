TEMPLATE	=app
CONFIG  += qt warn_on release console
QT -= gui
TARGET = SpeexMark
SOURCES = SpeexMark.cpp
HEADERS = Timer.h
INCLUDEPATH = ../src ../speex/include
LIBS += -lspeex
LIBPATH += ../release

win32 {
	QMAKE_CXX = icl
	QMAKE_CXXFLAGS += -Qrestrict
	QMAKE_CXXFLAGS_RELEASE += -O3 -QxK -Qip -Qipo
	QMAKE_LINK = xilink
}
