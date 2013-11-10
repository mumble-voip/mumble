include(../../compiler.pri)

TEMPLATE = app
CONFIG -= qt
CONFIG += debug_and_release
TARGET = mumble

win32 {
  DEFINES += WIN32 _WIN32
  RC_FILE = ../mumble/mumble.rc
  LIBS *= -luser32 -lshlwapi

  CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS_RELEASE -= -MD
    QMAKE_CXXFLAGS += -MT
  }
  CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS_DEBUG -= -MDd
    QMAKE_CXXFLAGS += -MTd
  }
}

SOURCES *= mumble_exe.cpp Overlay.cpp

!CONFIG(no-elevation) {
  CONFIG(release, debug|release) {
    QMAKE_LFLAGS *= /MANIFESTUAC:\"level=\'asInvoker\' uiAccess=\'true\'\"
  }
}

QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../mumble/mumble.appcompat.manifest)

CONFIG(debug, debug|release) {
  CONFIG += console
  DEFINES *= DEBUG
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  DEFINES *= NDEBUG
  DESTDIR	= ../../release
}

include(../../symbols.pri)
