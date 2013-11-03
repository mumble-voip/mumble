include(../plugins.pri)

DEFINES		+= "NULL_DESC=\"L\\\"Team Fortress 2 (Retracted, now using link)\\\"\""
TARGET		= tf2
SOURCES		= ../null_plugin.cpp
LIBS		+= -luser32


