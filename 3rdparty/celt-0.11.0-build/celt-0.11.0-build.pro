include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)
VERSION=$$replace(BUILDDIR,-build,)
VERSION=$$replace(VERSION,celt-,)

!exists(../$$SOURCEDIR/COPYING) {
  message("The $$SOURCEDIR/ directory was not found. Please update your submodules (git submodule update --init) or build with CONFIG+=no-bundled-celt.")
  error("Aborting configuration")
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += no_include_pwd
VPATH = ../$$SOURCEDIR/libcelt
TARGET = celt0
RC_FILE = celt-0.11.0.rc
DEFINES += HAVE_CONFIG_H
TARGET_VERSION_EXT = .$$VERSION

CONFIG(static) {
  CONFIG -= static
  CONFIG += shared
}

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

QMAKE_CFLAGS -= -fPIE -pie

win32 {
  DEFINES += WIN32 _WIN32

  win32-g++ {
    # MinGW uses the config.h for Unix-like systems.
    INCLUDEPATH += ../$$BUILDDIR
  }

  win32-msvc* {
    INCLUDEPATH += ../$$BUILDDIR/win32

    CONFIG(sse2) {
      TARGET_VERSION_EXT = .$${VERSION}.sse2
    } else {
      QMAKE_CFLAGS_RELEASE -= -arch:SSE
      QMAKE_CFLAGS_DEBUG -= -arch:SSE
    }
  }
}

unix {
  contains(QMAKE_CFLAGS, -ffast-math) {
    DEFINES += FLOAT_APPROX
  }

  INCLUDEPATH += ../$$BUILDDIR
}

SOURCES *= bands.c celt.c cwrs.c entcode.c entdec.c entenc.c header.c kiss_fft.c laplace.c mathops.c mdct.c modes.c pitch.c plc.c quant_bands.c rate.c vq.c

# TARGET is the same in both versions of CELT, thus we have to specify
# a separated folder to prevent objects collision between the two.
OBJECTS_DIR = $$OBJECTS_ROOT/celt-0.11.0-build
MOC_DIR = $$MOC_ROOT/celt-0.11.0-build
RCC_DIR = $$RCC_ROOT/celt-0.11.0-build
UI_DIR = $$UI_ROOT/celt-0.11.0-build

include(../../qmake/symbols.pri)
