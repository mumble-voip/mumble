include(../compiler.pri)

!exists(../celt/COPYING) {
	message("The celt/ directory was not found. You need to do one of the following:")
	message("")
	message("Option 1: Use CELT Git:")
	message("git submodule init");
	message("git submodule update");
	message("")
	message("Option 2: Use system celt (only if it's a bleeding edge version):")
	message("qmake CONFIG+=no-bundled-celt -recursive")
	message("")
	error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib debug_and_release
CONFIG -= warn_on
CONFIG += warn_off
VPATH	= ../celt/libcelt
TARGET = celt
DEFINES += NDEBUG HAVE_CONFIG_H

win32 {
  DEFINES+=WIN32 _WINDOWS _USE_SSE _USE_MATH_DEFINES
}

unix {
	INCLUDEPATH += ../celtbuild
}

DIST = config.h 

SOURCES *= bands.c celt.c cwrs.c ecintrin.h entcode.c entdec.c entenc.c header.c kfft_single.c kiss_fft.c kiss_fftr.c laplace.c mdct.c modes.c pitch.c psy.c quant_bands.c rangedec.c rangeenc.c rate.c vq.c

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR	= ../debug
}

CONFIG(release, debug|release) {
  DESTDIR	= ../release
}


