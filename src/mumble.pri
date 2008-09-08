include(../compiler.pri)

VERSION		= 1.1.6
DIST		= mumble.pro murmur.pro mumble.pri mumble.ico Message.h mumble_plugin.h PacketDataStream.h CryptState.h Timer.h Version.h
CONFIG		+= qt thread debug_and_release warn_on

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
