include(../../qmake/compiler.pri)
TEMPLATE = app
CONFIG += qt thread warn_on release qtestlib no_keywords console
CONFIG -= app_bundle
LANGUAGE = C++
TARGET = Resample
SOURCES = Resample.cpp Timer.cpp
HEADERS = Timer.h
VPATH += ..
INCLUDEPATH	*= .. ../../3rdparty/speex-src/include ../../3rdparty/speex-src/libspeex ../../3rdparty/speex-build
LIBS 		*= -lspeex

win32 {
  CONFIG(intelcpp) {
    LIBS	*= -l"\Program Files (x86)\Intel/Compiler\11.0\075\cpp\ipp\ia32\lib\ippsemerged"
    LIBS	*= -l"\Program Files (x86)\Intel/Compiler\11.0\075\cpp\ipp\ia32\lib\ippsmerged"
    LIBS	*= -l"\Program Files (x86)\Intel/Compiler\11.0\075\cpp\ipp\ia32\lib\ippsremerged"
    LIBS	*= -l"\Program Files (x86)\Intel/Compiler\11.0\075\cpp\ipp\ia32\lib\ippsrmerged"
    LIBS	*= -l"\Program Files (x86)\Intel/Compiler\11.0\075\cpp\ipp\ia32\lib\ippcorel"
  }
}

CONFIG(debug, debug|release) {
  QMAKE_LIBDIR = ../../debug $$QMAKE_LIBDIR
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LIBDIR = ../../release $$QMAKE_LIBDIR
  DESTDIR	= ../../release
}
