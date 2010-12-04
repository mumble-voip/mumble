include(../compiler.pri)

VERSION		= 1.2.3
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h SSL.h Mumble.proto
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD .
VPATH		+= $$PWD
HEADERS		*= ACL.h Group.h Channel.h Connection.h User.h Net.h
SOURCES 	*= Mumble.pb.cc ACL.cpp Group.cpp Channel.cpp Connection.cpp User.cpp Timer.cpp CryptState.cpp OSInfo.cpp Net.cpp SSL.cpp Version.cpp
PROTOBUF	*= ../Mumble.proto

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = protoc --cpp_out=. -I. -I.. ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.CONFIG *= no_link

CONFIG(packaged) {
	MUMDEFVER = $$find(DEFINES, "MUMBLE_VERSION=")
	count(MUMDEFVER, 0) {
		DEFINES	*= MUMBLE_VERSION=$$VERSION
	}
}

win32 {
	INCLUDEPATH *= "$$PROTOBUF_PATH/vsprojects/include" "$$PROTOBUF_PATH/src"
	CONFIG(debug, debug|release) {
		QMAKE_LIBDIR *= "$$PROTOBUF_PATH/vsprojects/Debug"
	} else {
		QMAKE_LIBDIR *= "$$PROTOBUF_PATH/vsprojects/Release"
	}
	INCLUDEPATH *= "$$OPENSSL_PATH/include"
	QMAKE_LIBDIR *= "$$OPENSSL_PATH/lib"

	LIBS *= -llibprotobuf -lcrypt32 -lws2_32 -llibeay32
	LIBS *= -ldelayimp -lQwave -delayload:Qwave.DLL
}

unix {
	UNAME=$$system(uname -s)

	CONFIG *= link_pkgconfig
	LIBS *= -lprotobuf

	contains(UNAME, FreeBSD) {
		LIBS *= -lcrypto
	} else {
		PKGCONFIG *= openssl
	}
}

QMAKE_EXTRA_COMPILERS *= pb

CONFIG(debug, debug|release) {
  CONFIG += console
  QMAKE_LIBDIR += ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LIBDIR += ../../release
  DESTDIR	= ../../release
}
