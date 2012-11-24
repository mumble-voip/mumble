TEMPLATE = app
CONFIG += qt thread warn_on network qtestlib
CONFIG -= app_bundle
QT += network
LANGUAGE = C++
TARGET = TestPacketDataStream
SOURCES = TestPacketDataStream.cpp
INCLUDEPATH += .. ../murmur ../mumble

