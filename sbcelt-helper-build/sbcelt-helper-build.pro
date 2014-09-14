include (../compiler.pri)

BUILDDIR=$$basename(PWD)
SOURCEDIR=$$replace(BUILDDIR,-helper-build,-src)
CELTDIR=../celt-0.7.0-src/libcelt

!exists($$CELTDIR/../COPYING) {
        message("The $$CELTDIR/ directory was not found. Please update your submodules (git submodule update --init).")
        error("Aborting configuration")
}

!exists(../$$SOURCEDIR/LICENSE) {
        message("The $$SOURCEDIR/ directory was not found. You need to do the following:")
        message("")
        message("Use SBCELT Git:")
        message("git submodule init")
        message("git submodule update")
        message("")
        error("Aborting configuration")
}

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG *= debug_and_release
CONFIG -= warn-on
VPATH   = ../$$SOURCEDIR/helper
TARGET = sbcelt-helper
DEFINES += HAVE_CONFIG_H
INCLUDEPATH += ../$$SOURCEDIR/helper ../$$SOURCEDIR/lib ../$$SOURCEDIR $$CELTDIR
LIBS += -lpthread

SOURCES = \
        $$CELTDIR/bands.c \
        $$CELTDIR/celt.c \
        $$CELTDIR/cwrs.c \
        $$CELTDIR/entcode.c \
        $$CELTDIR/entdec.c \
        $$CELTDIR/entenc.c \
        $$CELTDIR/header.c \
        $$CELTDIR/kiss_fft.c \
        $$CELTDIR/kiss_fftr.c \
        $$CELTDIR/laplace.c \
        $$CELTDIR/mdct.c \
        $$CELTDIR/modes.c \
        $$CELTDIR/pitch.c \
        $$CELTDIR/psy.c \
        $$CELTDIR/quant_bands.c \
        $$CELTDIR/rangedec.c \
        $$CELTDIR/rangeenc.c \
        $$CELTDIR/rate.c \
        $$CELTDIR/vq.c \
	sbcelt-helper.c \
	alloc.c

unix:!macx {
	UNAME=$$system(uname -s)
	contains(UNAME, Linux) {
		SOURCES *= ../lib/futex-linux.c seccomp-sandbox.c sbcelt-sandbox-linux.c pdeath-linux.c
		LIBS += -lrt
	}
	contains(UNAME, FreeBSD) {
		SOURCES *= ../lib/futex-freebsd.c sbcelt-sandbox-freebsd.c pdeath-kqueue.c
	}
}

macx {
	SOURCES *= ../lib/futex-stub.c sbcelt-sandbox-darwin.c pdeath-kqueue.c
}

CONFIG(release, debug|release) {
  DESTDIR = ../release
}

CONFIG(debug, debug|release) {
  DEFINES *= USE_LOGFILE
  DESTDIR = ../debug/
}

include(../symbols.pri)
