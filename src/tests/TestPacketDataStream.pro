TEMPLATE	=app
CONFIG  += qt thread warn_on network qtestlib
QT += network
LANGUAGE	= C++
TARGET = TestPacketDataStream
SOURCES = TestPacketDataStream.cpp
INCLUDEPATH += .. ../murmur ../mumble

