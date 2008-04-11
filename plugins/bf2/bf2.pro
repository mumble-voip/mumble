include(../plugins.pri)
TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt
TARGET		= bf2
SOURCES		= bf2.cpp
LIBS		+= -luser32


