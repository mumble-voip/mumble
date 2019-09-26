include(../../qmake/compiler.pri)

!exists(../speexdsp-src/COPYING) {
  message("The speexdsp-src/ directories was not found. You need to do one of the following:")
  message("")
  message("Option 1: Use Speex Git:")
  message("git submodule init")
  message("git submodule update")
  message("")
  message("Option 2: Use system speex-dsp (v1.2 or later):")
  message("qmake CONFIG+=no-bundled-speex -recursive")
  message("")
  error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG -= warn_on
CONFIG += warn_off
CONFIG += no_include_pwd

VPATH = ../speexdsp-src/libspeexdsp

TARGET = speex
DEFINES += NDEBUG HAVE_CONFIG_H
INCLUDEPATH = ../speexdsp-src/include ../speexdsp-src/libspeexdsp

win32 {
  INCLUDEPATH += ../speex-build/win32
  DEFINES+=WIN32 _WINDOWS _USE_SSE _USE_MATH_DEFINES

  win32-g++ {
    # MinGW uses the config.h for Unix-like systems.
    INCLUDEPATH += ../speex-build
  }

  win32-msvc* {
    INCLUDEPATH += ../speex-build/win32

    CONFIG(sse2) {
      TARGET = speex.sse2
      DEFINES += _USE_SSE2
    } else {
      QMAKE_CFLAGS_RELEASE -= -arch:SSE
      QMAKE_CFLAGS_DEBUG -= -arch:SSE
    }
  }

  SOURCES *= mumble_speex_init.c

  CONFIG -= static
  CONFIG += shared

  DEFINES+=USE_SMALLFT

} else {
  CONFIG += staticlib
  INCLUDEPATH += ../speex-build
}

DEF_FILE = speex.def

# libspeexdsp
SOURCES *= mdf.c resample.c preprocess.c jitter.c filterbank.c fftwrap.c smallft.c
