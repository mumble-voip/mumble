include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)

!win32 {
  VERSION=0 #$$ Fool packaging script
}

!exists(../$$SOURCEDIR/COPYING) {
  message("The $$SOURCEDIR/ directory was not found. Please update your submodules (git submodule update --init).")
  error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
VPATH = ../$$SOURCEDIR
TARGET = rnnoise
DEFINES += HAVE_CONFIG_H

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

QMAKE_CFLAGS -= -fPIE -pie

win32 {
  DEFINES += WIN32 _WIN32
  DEFINES += DLL_EXPORT

  CONFIG -= static
  CONFIG += shared
}

win32-msvc* {
  DEFINES += _USE_MATH_DEFINES # To define M_PI
  DEFINES += USE_MALLOC # To use malloc() and free() instead of variable length arrays
}

unix {
  CONFIG += staticlib

  contains(QMAKE_CFLAGS, -ffast-math) {
    DEFINES += FLOAT_APPROX
  }
}

INCLUDEPATH *= \
../$$SOURCEDIR/include \

SOURCES *= \
src/rnn_data.c \
src/rnn.c \
src/pitch.c \
src/kiss_fft.c \
src/denoise.c \
src/celt_lpc.c

include(../../qmake/symbols.pri)
