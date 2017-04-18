include(../../qmake/compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-build,-src)

!exists(../$$SOURCEDIR/README.markdown) {
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
CONFIG += x86_64 x86

!CONFIG(third-party-warnings) {
  # We ignore warnings in third party builds. We won't actually look
  # at them and they clutter out our warnings.
  CONFIG -= warn_on
  CONFIG += warn_off
}

VPATH	= ../mach-override-src
TARGET = mach-override
INCLUDEPATH = ../mach-override-src/ ../mach-override-src/libudis86

SOURCES *= \
  mach_override.c \
  libudis86/decode.c \
  libudis86/input.c \
  libudis86/itab.c \
  libudis86/syn-att.c \
  libudis86/syn-intel.c \
  libudis86/syn.c \
  libudis86/udis86.c
