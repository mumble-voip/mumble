TEMPLATE	=app
CONFIG  += qt thread warn_on network qdbus xml qtestlib qsql
QT += qdbus xml sql network
LANGUAGE	= C++
TARGET = TestPacketDataStream
SOURCES = TestPacketDataStream.cpp
INCLUDEPATH += .. ../murmur ../mumble

