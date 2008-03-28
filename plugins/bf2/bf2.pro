include(../../compiler.pri)
TEMPLATE	= lib
CONFIG		+= dll release warn_on shared
CONFIG		-= qt
TARGET		= ../../bf2
SOURCES		= bf2.cpp
LIBS		+= -luser32


