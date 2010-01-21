TEMPLATE	=app
CONFIG  += qt thread warn_on debug console
QT += network gui
LANGUAGE	= C++
TARGET = Overlay
SOURCES = Overlay.cpp
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
