include(../plugins.pri)

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
