include(../../compiler.pri)

TEMPLATE = app
CONFIG -= qt
CONFIG += debug_and_release
TARGET = mumble_ol_helper

win32 {
  DEFINES += WIN32 _WIN32
  RC_FILE = overlay_exe.rc
  LIBS *= -luser32 -lshlwapi -lshell32

  CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS_RELEASE -= -MD
    QMAKE_CXXFLAGS += -MT
  }
  CONFIG(debug, debug|release) {
    QMAKE_CXXFLAGS_DEBUG -= -MDd
    QMAKE_CXXFLAGS += -MTd
  }
}

SOURCES *= overlay_exe.cpp

QMAKE_POST_LINK = $$QMAKE_POST_LINK$$escape_expand(\\n\\t)$$quote(mt.exe -nologo -updateresource:$(DESTDIR_TARGET);1 -manifest ../../src/mumble/mumble.appcompat.manifest)

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
