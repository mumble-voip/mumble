TEMPLATE = app
CONFIG += qt warn_on qtestlib
CONFIG -= app_bundle
LANGUAGE = C++
TARGET = TestTimer
SOURCES = TestTimer.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble

