include(../mumble.pri)

TEMPLATE	=app
CONFIG  += network
CONFIG(static) {
	QMAKE_LFLAGS += -static
}
CONFIG	-= gui
QT += network sql xml
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
RC_FILE = murmur.rc
FORMS = 
HEADERS = Server.h ServerDB.h Register.h
SOURCES = murmur.cpp Server.cpp ServerDB.cpp Register.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp

DIST = DBus.h

PRECOMPILED_HEADER = murmur_pch.h
DIST = murmur.pl murmur.ini link.pl Commands.txt mysql.sql mysql_upgrade.sql

win32 {
  HEADERS += DBus_fake.h
  LIBS	+= -lws2_32
}

unix {
  SOURCES += DBus.cpp
  HEADERS += DBus_real.h
  CONFIG += qdbus
}
