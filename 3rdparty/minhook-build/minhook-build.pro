CONFIG += force-x86_64-toolchain
include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)

!exists(../$$SOURCEDIR/README.md) {
  message("The $$SOURCEDIR/ directory was not found. Please update your submodules (git submodule update --init).")
  error("Aborting configuration")
}

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR	= ../../release
}

TEMPLATE = lib
CONFIG -= qt
CONFIG += debug_and_release
CONFIG += staticlib
VPATH	= ../$$SOURCEDIR
TARGET = minhook
INCLUDEPATH *= ../$$SOURCEDIR/src ../$$SOURCEDIR/src/hde ../$$SOURCEDIR/include
DEFINES += WIN32 _WINDOWS _USRDLL MINHOOK_EXPORTS

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

win32-msvc* {
  QMAKE_CFLAGS_RELEASE -= -MD
  QMAKE_CFLAGS_DEBUG -= -MDd

  QMAKE_CXXFLAGS_RELEASE *= -MT
  QMAKE_CXXFLAGS_DEBUG *= -MTd
}

SOURCES *= \
  src/hde/hde64.c \
  src/hde/hde32.c \
  src/buffer.c \
  src/hook.c \
  src/trampoline.c
