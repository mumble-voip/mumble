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
  INCLUDEPATH += ../speexbuild/win32
  DEFINES+=WIN32 _WINDOWS _USE_SSE _USE_MATH_DEFINES
  !CONFIG(intelcpp) {
    DEFINES+=USE_SMALLFT
  }
} else {
  INCLUDEPATH += ../speexbuild
}

macx {
	DEFINES += OS_SUPPORT_CUSTOM
}

DIST = config.h speex.def speex/speex_config_types.h

DEF_FILE = speex.def
# libspeexdsp
SOURCES = mdf.c resample.c preprocess.c jitter.c filterbank.c fftwrap.c smallft.c
# libspeex
SOURCES *= bits.c cb_search.c exc_10_16_table.c exc_10_32_table.c exc_20_32_table.c exc_5_256_table.c exc_5_64_table.c exc_8_128_table.c filters.c gain_table.c gain_table_lbr.c hexc_10_32_table.c hexc_table.c high_lsp_tables.c lpc.c lsp.c lsp_tables_nb.c ltp.c modes.c modes_wb.c nb_celp.c quant_lsp.c sb_celp.c scal.c speex.c speex_callbacks.c speex_header.c stereo.c vbr.c vq.c kiss_fft.c kiss_fftr.c vorbis_psy.c window.c

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR	= ../debug
}

CONFIG(release, debug|release) {
  DESTDIR	= ../release
}


