include(../compiler.pri)
include(../qt.pri)

VERSION		= 1.3.0
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h SSL.h Mumble.proto
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD .
VPATH		+= $$PWD
HEADERS		*= ACL.h Channel.h CryptState.h Connection.h Group.h User.h Net.h OSInfo.h Timer.h SSL.h Version.h
SOURCES 	*= ACL.cpp Group.cpp Channel.cpp Connection.cpp User.cpp Timer.cpp CryptState.cpp OSInfo.cpp Net.cpp SSL.cpp Version.cpp
PROTOBUF	*= ../Mumble.proto

pbh.output = ${QMAKE_FILE_BASE}.pb.h
pbh.depends = ${QMAKE_FILE_BASE}.pb.cc
pbh.commands = $$escape_expand(\\n)
pbh.input = PROTOBUF
pbh.CONFIG *= no_link explicit_dependencies target_predeps

pb.output = ${QMAKE_FILE_BASE}.pb.cc
pb.commands = protoc --cpp_out=. -I. -I.. ${QMAKE_FILE_NAME}
pb.input = PROTOBUF
pb.CONFIG *= no_link explicit_dependencies
pb.variable_out = SOURCES

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

	CONFIG(static) {
		PKG_CONFIG = pkg-config --static
	}

	CONFIG *= link_pkgconfig
	LIBS *= -lprotobuf

	contains(UNAME, FreeBSD) {
		LIBS *= -lcrypto
	} else {
		PKGCONFIG *= openssl
	}
}

QMAKE_EXTRA_COMPILERS *= pb pbh

CONFIG(debug, debug|release) {
  CONFIG += console
  QMAKE_LIBDIR += ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LIBDIR += ../../release
  DESTDIR	= ../../release
}
