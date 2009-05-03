include(../compiler.pri)

!exists(../speex/COPYING) {
	message("The speex/ directory was not found. You need to do one of the following:")
	message("")
	message("Option 1: Use Speex Git:")
	message("git submodule init")
	message("git submodule update")
	message("")
	message("Option 2: Use system speex (only if it's a bleeding edge version):")
	message("qmake CONFIG+=no-bundled-speex -recursive")
	message("")
	error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib debug_and_release
CONFIG -= warn_on
CONFIG += warn_off
VPATH	= ../speex/libspeex
TARGET = speexdsp
DEFINES += NDEBUG HAVE_CONFIG_H
INCLUDEPATH = ../speex/include ../speex/libspeex

win32 {
  INCLUDEPATH += ../speex/win32
  DEFINES+=WIN32 _WINDOWS _USE_SSE _USE_MATH_DEFINES
}

unix {
	INCLUDEPATH += ../speexbuild
}

DIST = config.h speex.def speex/speex_config_types.h

DEF_FILE = speex.def
SOURCES = mdf.c resample.c preprocess.c jitter.c filterbank.c fftwrap.c smallft.c

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR	= ../debug
}

CONFIG(release, debug|release) {
  DESTDIR	= ../release
}


