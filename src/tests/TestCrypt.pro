TEMPLATE = app
CONFIG += qt thread warn_on network qtestlib debug
CONFIG -= app_bundle
QT += network
LANGUAGE = C++
TARGET = TestCrypt
HEADERS = Timer.h CryptState.h
SOURCES = TestCrypt.cpp CryptState.cpp Timer.cpp
VPATH += ..
INCLUDEPATH += .. ../murmur ../mumble
LIBS	+= -lcrypto
