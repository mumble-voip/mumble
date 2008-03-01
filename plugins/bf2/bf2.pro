TEMPLATE	= lib
CONFIG		= debug_and_release dll release warn_on shared
CONFIG		-= qt gui
TARGET		= ../bf2
SOURCES		= bf2.cpp
LIBS		+= -luser32


