include(mumble.pri)

TEMPLATE	=app
CONFIG  += network console 
CONFIG	-= gui
QT += network sql
QT -= gui
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
RC_FILE = mumble.rc
FORMS = 
HEADERS = ACL.h Group.h Channel.h Server.h Connection.h Player.h ServerDB.h
SOURCES = ACL.cpp Group.cpp Channel.cpp murmur.cpp  Message.cpp Connection.cpp Server.cpp Player.cpp ServerDB.cpp
DIST = murmur.pl murmur.ini Commands.txt
