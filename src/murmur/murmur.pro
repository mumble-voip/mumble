include(../mumble.pri)

TEMPLATE	=app
CONFIG  += network qdbus
CONFIG(static) {
	QMAKE_LFLAGS += -static
}
CONFIG	-= gui
QT += network sql xml
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
FORMS =
HEADERS = Server.h Meta.h DBus.h
SOURCES = murmur.cpp Server.cpp ServerDB.cpp Register.cpp Cert.cpp Messages.cpp Meta.cpp DBus.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp ../Timer.cpp ../CryptState.cpp

DIST = DBus.h ServerDB.h murmur.ico
PRECOMPILED_HEADER = murmur_pch.h
DIST += murmur.pl murmur.ini murmur.ini.system murmur.init murmur.conf murmur.logrotate link.pl dbusauth.pl weblist.pl murmur-user-wrapper

win32 {
  RC_FILE = murmur.rc
  CONFIG += gui
  QT += gui
  LIBS	+= -lws2_32
  RESOURCES	+= murmur.qrc
  SOURCES += Tray.cpp
  HEADERS += Tray.h
  INCLUDEPATH += /dev/OpenSSL/include
  LIBPATH += /dev/OpenSSL/lib
  LIBS += -llibeay32 -luser32
}

unix {
  CONFIG += link_pkgconfig
  PKGCONFIG += openssl
  HEADERS += UnixMurmur.h
  SOURCES += UnixMurmur.cpp
  TARGET = murmurd
}
