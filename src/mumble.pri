include(../compiler.pri)

VERSION		= 1.1.8
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD
SOURCES	*= Mumble.pb.cc
PROTOBUF	*= ../Mumble.proto
LIBS		*= -lprotobuf

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = protoc --cpp_out=. -I. -I.. ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.config *= no_link target_predeps


QMAKE_EXTRA_COMPILERS *= pb

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBPATH	+= ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  LIBPATH	+= ../../release
  DESTDIR	= ../../release
}
