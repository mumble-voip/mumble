include(../plugins.pri)

DEFINES		+= "NULL_DESC=\"L\\\"Half Life 2 Death Match (Retracted, now using link)\\\"\""
TARGET		= hl2dm
SOURCES		= ../null_plugin.cpp
LIBS		+= -luser32


