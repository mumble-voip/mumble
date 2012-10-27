include(../mumble.pri)

TEMPLATE = app
CONFIG *= qt thread warn_on network qtestlib debug
CONFIG -= app_bundle
QT *= network xml
LANGUAGE = C++
TARGET = Benchmark
SOURCES *= Benchmark.cpp Timer.cpp CryptState.cpp
HEADERS *= Timer.h CryptState.h
VPATH *= ..
INCLUDEPATH *= .. ../murmur ../mumble
!win32 {
	LIBS *= -lcrypto
}
