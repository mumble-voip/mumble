include(../../compiler.pri)
TEMPLATE	=app
CONFIG  += qt thread warn_on release qtestlib no_keywords console
LANGUAGE	= C++
TARGET = Resample
SOURCES = Resample.cpp
VPATH += ..
INCLUDEPATH	*= ../../speex/include ../../speex/libspeex ../../speexbuild
LIBS 		*= -lspeex

CONFIG(debug, debug|release) {
  LIBPATH	+= ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  LIBPATH	+= ../../release
  DESTDIR	= ../../release
}
