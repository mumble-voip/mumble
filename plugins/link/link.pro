include(../../compiler.pri)
TEMPLATE	= lib
CONFIG		+= dll release warn_on shared
CONFIG		-= qt
TARGET		= ../../link
SOURCES		= link.cpp
LIBS		+= -luser32
