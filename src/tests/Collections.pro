TEMPLATE = app
CONFIG  += qt thread warn_on release
CONFIG -= app_bundle
LANGUAGE = C++
TARGET = Collections
SOURCES = Collections.cpp Timer.cpp
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
