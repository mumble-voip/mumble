include(../plugins.pri)
TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt
TARGET		= wow
SOURCES		= wow.cpp
LIBS		+= -luser32
