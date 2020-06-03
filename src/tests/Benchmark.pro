include(../mumble.pri)

TEMPLATE = app
CONFIG *= qt thread warn_on network qtestlib debug
CONFIG -= app_bundle
QT *= network xml
LANGUAGE = C++
TARGET = Benchmark
SOURCES *= Benchmark.cpp Timer.cpp crypto/CryptStateOCB2.cpp
HEADERS *= Timer.h crypto/CryptState.h crypto/CryptStateOCB2.h
VPATH *= ..
INCLUDEPATH *= .. ../murmur ../mumble
!win32 {
  LIBS *= -lcrypto
}
