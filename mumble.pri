VERSION		= 0.2.2
DIST		= LICENSE INSTALL README CHANGES mumble.pro murmur.pro mumble.pri mumble.ico
CONFIG		+= qt thread debug_and_release warn_on

QMAKE_CFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
QMAKE_CXXFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBS	+= -Ldebug
}

CONFIG(release, debug|release) {
  LIBS	+= -Lrelease
}

# To use external profiling tools on the release builds, but
# we need the debug symbols.

CONFIG(profile) {
  QMAKE_CFLAGS += -g -O3
  QMAKE_CXXFLAGS += -g -O3
  QMAKE_LFLAGS += -g
  QMAKE_LFLAGS -= -Wl,-s
}
  