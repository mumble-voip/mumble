# Simple project file to build Speex with same compilers
# as Qt used.

TEMPLATE = lib
CONFIG -= qt
CONFIG += staticlib debug_and_release
VPATH	= ../speex/libspeex
TARGET = speex
DEFINES += NDEBUG HAVE_CONFIG_H
INCLUDEPATH = ../speex/include ../speex/libspeex

win32 {
	DEFINES+=WIN32 _WINDOWS _USE_SSE VAR_ARRAYS
	INCLUDEPATH += ../speex/win32
}

unix {
	INCLUDEPATH += ../speexbuild
}

macx {
  CONFIG += x86 ppc
}

DIST = config.h speex.def speex/speex_config_types.h

DEF_FILE = speex.def
SOURCES = bits.c cb_search.c exc_10_16_table.c exc_10_32_table.c exc_20_32_table.c exc_5_256_table.c exc_5_64_table.c exc_8_128_table.c filterbank.c filters.c fftwrap.c gain_table.c gain_table_lbr.c hexc_10_32_table.c hexc_table.c high_lsp_tables.c jitter.c lpc.c lsp.c lsp_tables_nb.c ltp.c mdf.c modes.c modes_wb.c nb_celp.c preprocess.c quant_lsp.c sb_celp.c smallft.c speex.c speex_callbacks.c speex_header.c stereo.c vbr.c vq.c kiss_fft.c kiss_fftr.c vorbis_psy.c window.c resample.c

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR	= ../debug
}

CONFIG(release, debug|release) {
  DESTDIR	= ../release
}

CONFIG(symbols) {
  QMAKE_CFLAGS_RELEASE += -g 
  QMAKE_CXXFLAGS_RELEASE += -g
  QMAKE_LFLAGS += -g
  QMAKE_LFLAGS -= -Wl,-s
  QMAKE_LFLAGS_RELEASE -= -Wl,-s
}

CONFIG(optgen) {
  QMAKE_CFLAGS += -O3 -ffast-math -ftree-vectorize -fprofile-generate
}

CONFIG(optimize) {
  !exists(release/speex.gcda) {
     error(You need to run SpeexMark first)
  }
  QMAKE_CFLAGS += -O3 -ffast-math -ftree-vectorize -fprofile-use
}
