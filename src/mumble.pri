VERSION		= 1.1.0
DIST		= mumble.pro murmur.pro mumble.pri mumble.ico Message.h mumble_plugin.h PacketDataStream.h CryptState.h Timer.h
CONFIG		+= qt thread debug_and_release warn_on
!nopch {
  CONFIG += precompile_header
}

QMAKE_CFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
QMAKE_CXXFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare

INCLUDEPATH	+= ..

CONFIG(debug, debug|release) {
  CONFIG += console
  QMAKE_LFLAGS	+= -L../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LFLAGS	+= -L../../release
  DESTDIR	= ../../release
}

# To use external profiling tools on the release builds, but
# we need the debug symbols.

CONFIG(symbols) {
  QMAKE_CFLAGS_RELEASE += -g 
  QMAKE_CXXFLAGS_RELEASE += -g 
  QMAKE_LFLAGS += -g
  QMAKE_LFLAGS_RELEASE -= -Wl,-s
}
  
