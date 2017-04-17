include(../../qmake/compiler.pri)

!exists(../speex-src/COPYING) | !exists(../speexdsp-src/COPYING) {
	message("The speex-src/ or speexdsp-src/ directories were not found. You need to do one of the following:")
	message("")
	message("Option 1: Use Speex Git:")
	message("git submodule init")
	message("git submodule update")
	message("")
	message("Option 2: Use system speex and speex-dsp (v 1.2 or later):")
	message("qmake CONFIG+=no-bundled-speex -recursive")
	message("")
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
CONFIG -= warn_on
CONFIG += warn_off
CONFIG += no_include_pwd

# Enable no_batch, which disables nmake's inference rules.
# We have to do this because we use files from two VPATHs
# below, and nmake is unable to figure out how to handle
# that.
CONFIG += no_batch
VPATH	= ../speex-src/libspeex ../speexdsp-src/libspeexdsp

TARGET = speex
DEFINES += NDEBUG HAVE_CONFIG_H
INCLUDEPATH = ../speex-src/include ../speex-src/libspeex ../speexdsp-src/include ../speexdsp-src/libspeexdsp

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

  SOURCES	*= mumble_speex_init.c

  CONFIG -= static
  CONFIG += shared

  DEFINES+=USE_SMALLFT

} else {
  CONFIG += staticlib
  INCLUDEPATH += ../speex-build
}

DIST = config.h speex.def speex/speex_config_types.h

DEF_FILE = speex.def

# libspeexdsp
SOURCES *= mdf.c resample.c preprocess.c jitter.c filterbank.c fftwrap.c smallft.c
# libspeex
SOURCES *= bits.c cb_search.c exc_10_16_table.c exc_10_32_table.c exc_20_32_table.c exc_5_256_table.c exc_5_64_table.c exc_8_128_table.c filters.c gain_table.c gain_table_lbr.c hexc_10_32_table.c hexc_table.c high_lsp_tables.c lpc.c lsp.c lsp_tables_nb.c ltp.c modes.c modes_wb.c nb_celp.c quant_lsp.c sb_celp.c scal.c speex.c speex_callbacks.c speex_header.c stereo.c vbr.c vq.c kiss_fft.c kiss_fftr.c vorbis_psy.c window.c



