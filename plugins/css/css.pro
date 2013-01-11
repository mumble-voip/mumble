include(../plugins.pri)

DEFINES		+= "NULL_DESC=\"L\\\"CounterStrike : Source (Retracted, now using link)\\\"\""
TARGET		= css
SOURCES		= ../null_plugin.cpp
LIBS		+= -luser32
