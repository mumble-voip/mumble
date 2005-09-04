include(mumble.pri)

TEMPLATE	=app
CONFIG  += network console
CONFIG	-= gui
QT           += network
TARGET = murmur
DBFILE  = murmur.db
LANGUAGE	= C++
RC_FILE = mumble.rc
FORMS = 
HEADERS = Server.h Connection.h Player.h
SOURCES = murmur.cpp  Message.cpp Connection.cpp Server.cpp Player.cpp
