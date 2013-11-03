include(../plugins.pri)

DEFINES		+= "NULL_DESC=\"L\\\"Day of Defeat : Source (Retracted, now using link)\\\"\""
TARGET		= dods
SOURCES		= ../null_plugin.cpp
LIBS		+= -luser32
