include(../plugins.pri)

TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt
TARGET		= link

win32 {
	SOURCES		= link.cpp
	LIBS		+= -luser32
}

!win32 {
	SOURCES		= link-posix.cpp
	LIBS		+= -lrt
}

