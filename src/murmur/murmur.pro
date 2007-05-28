include(../mumble.pri)

TEMPLATE	=app
CONFIG  += network console qdbus
CONFIG	-= gui
QT += network sql
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
RC_FILE = murmur.rc
FORMS = 
HEADERS = Server.h ServerDB.h DBus.h
SOURCES = murmur.cpp Server.cpp ServerDB.cpp DBus.cpp
HEADERS	+= ../ACL.h ../Group.h ../Channel.h ../Connection.h ../Player.h
SOURCES += ../ACL.cpp ../Group.cpp ../Channel.cpp ../Message.cpp ../Connection.cpp ../Player.cpp

PRECOMPILED_HEADER = murmur_pch.h
DIST = murmur.pl murmur.ini link.pl Commands.txt

win32 {
  LIBS	+= -lws2_32
}
