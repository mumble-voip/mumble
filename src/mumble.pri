VERSION		= 1.1.3
DIST		= mumble.pro murmur.pro mumble.pri mumble.ico Message.h mumble_plugin.h PacketDataStream.h CryptState.h Timer.h
CONFIG		+= qt thread debug_and_release warn_on
!nopch {
  CONFIG += precompile_header
}

!win32 {
  QMAKE_CFLAGS		+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
  QMAKE_CXXFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
  !macx {
    QMAKE_LFLAGS		+= -Wl,--as-needed
  }
}
win32 {
  QMAKE_CXX = icl
  QMAKE_CFLAGS += -Qstd=c99 -Qrestrict
  QMAKE_CXXFLAGS += -Qstd=c++0x -Qrestrict
  QMAKE_CXXFLAGS_RELEASE += -O3 -QxK -Qip -Qipo
  QMAKE_CXXFLAGS_DEBUG += -O2 -QxK -Ob0
  QMAKE_LINK = xilink
}

INCLUDEPATH	+= $$PWD

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBPATH	+= ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  LIBPATH	+= ../../release
  DESTDIR	= ../../release
}

# To use external profiling tools on the release builds, but
# we need the debug symbols.

CONFIG(symbols) {
  QMAKE_CFLAGS_RELEASE += -GR -Zi -Oy-
  QMAKE_CXXFLAGS_RELEASE += -GR -Zi -Oy-
  QMAKE_LFLAGS += -fixed:no -debug
}
  
