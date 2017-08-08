TEMPLATE =app
CONFIG += qt thread warn_on qtestlib debug
LANGUAGE = C++
TARGET = Hash
SOURCES = Hash.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
