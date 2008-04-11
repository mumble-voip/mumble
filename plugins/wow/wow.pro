include(../plugins.pri)
TEMPLATE	= lib
CONFIG		+= dll release warn_on shared
CONFIG		-= qt
TARGET		= ../../wow
SOURCES		= wow.cpp
LIBS		+= -luser32
