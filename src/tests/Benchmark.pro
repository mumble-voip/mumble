include(../mumble.pri)

TEMPLATE	=app
CONFIG *= qt thread warn_on network xml qtestlib sql debug
QT *= xml sql network
LANGUAGE	= C++
TARGET = Benchmark
SOURCES *= Benchmark.cpp Timer.cpp CryptState.cpp
HEADERS *= Timer.h
VPATH *= ..
INCLUDEPATH *= .. ../murmur ../mumble
!win32 {
	LIBS *= -lcrypto
}
