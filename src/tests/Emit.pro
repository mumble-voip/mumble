TEMPLATE	=app
CONFIG  += qt thread warn_on release no_keywords
LANGUAGE	= C++
TARGET = Emit
HEADERS = Timer.h
SOURCES = Emit.cpp Timer.cpp
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
LIBS += -lboost_signals
QMAKE_CXXFLAGS *= -O3
