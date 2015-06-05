include(../compiler.pri)
include(../qt.pri)

VERSION		= 1.3.0
DIST		= mumble.pri Message.h PacketDataStream.h CryptState.h Timer.h Version.h OSInfo.h SSL.h
CONFIG		+= qt thread debug_and_release warn_on
DEFINES		*= MUMBLE_VERSION_STRING=$$VERSION
INCLUDEPATH	+= $$PWD . ../mumble_proto
VPATH		+= $$PWD
HEADERS		*= ACL.h Channel.h CryptState.h Connection.h Group.h User.h Net.h OSInfo.h Timer.h SSL.h Version.h
SOURCES 	*= ACL.cpp Group.cpp Channel.cpp Connection.cpp User.cpp Timer.cpp CryptState.cpp OSInfo.cpp Net.cpp SSL.cpp Version.cpp
LIBS		*= -lmumble_proto
# Note: Protobuf generates into its own directory so we can mark it as a
#       system include folder for unix. Otherwise the generated code creates
#       a lot of spurious warnings in ours.

CONFIG(packaged) {
	MUMDEFVER = $$find(DEFINES, "MUMBLE_VERSION=")
	count(MUMDEFVER, 0) {
		DEFINES	*= MUMBLE_VERSION=$$VERSION
	}
}

win32 {
	INCLUDEPATH *= "$$PROTOBUF_PATH/vsprojects/include" "$$PROTOBUF_PATH/src" protobuf
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

	QMAKE_CFLAGS *= -isystem ../mumble_proto
	QMAKE_CXXFLAGS *= -isystem ../mumble_proto

	CONFIG *= link_pkgconfig
	LIBS *= -lprotobuf

	contains(UNAME, FreeBSD) {
		LIBS *= -lcrypto
	} else {
		PKGCONFIG *= openssl
	}
}

# Make Q_DECL_OVERRIDE and Q_DECL_FINAL no-ops
# for Qt 4.
isEqual(QT_MAJOR_VERSION, 4) {
	DEFINES *= Q_DECL_OVERRIDE=
	DEFINES *= Q_DECL_FINAL=
}

CONFIG(debug, debug|release) {
  CONFIG += console
  QMAKE_LIBDIR += ../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  QMAKE_LIBDIR += ../../release
  DESTDIR	= ../../release
}
