TEMPLATE	=app
CONFIG  += qt thread warn_on network qdbus xml qtestlib qsql
QT += qdbus xml sql network
LANGUAGE	= C++
TARGET = TestTimer
SOURCES = TestTimer.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble

