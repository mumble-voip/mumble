include(../plugins.pri)
TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt
TARGET		= link

win32 {
	SOURCES		= link.cpp
	LIBS		+= -luser32
}

unix {
	SOURCES		= link-posix.cpp

        !macx {
		LIBS          += -lrt
	}
}

