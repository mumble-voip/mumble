include(../compiler.pri)

VERSION		= 1.2.0
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD .
SOURCES		*= Mumble.pb.cc
PROTOBUF	*= ../Mumble.proto

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = protoc --cpp_out=. -I. -I.. ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.CONFIG *= no_link

win32 {
	INCLUDEPATH *= /dev/protobuf-2.1.0/vsprojects/include
	CONFIG(debug, debug|release) {
		LIBPATH *= /dev/protobuf-2.1.0/vsprojects/Debug
	} else {
		LIBPATH *= /dev/protobuf-2.1.0/vsprojects/Release
	}
	LIBS *= -llibprotobuf
}

unix {
	LIBS *= -lprotobuf
	macx {
		INCLUDEPATH *= /opt/mumble/protobuf/include/
		LIBPATH *= /opt/mumble/protobuf/lib/
	}
}

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
