VERSION		= 0.9.5
DIST		= LICENSE INSTALL README CHANGES mumble.pro murmur.pro mumble.pri mumble.ico
CONFIG		+= qt thread debug_and_release warn_on precompile_header

QMAKE_CFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare
QMAKE_CXXFLAGS	+= -Wshadow -Woverloaded-virtual -Wold-style-cast -Wconversion -Wsign-compare

INCLUDEPATH	+= ..

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBS	+= -L../../debug
  DESTDIR	= ../../debug
}

CONFIG(release, debug|release) {
  LIBS	+= -L../../release
  DESTDIR	= ../../release
}

# To use external profiling tools on the release builds, but
# we need the debug symbols.

CONFIG(symbols) {
  QMAKE_CFLAGS_RELEASE += -g 
  QMAKE_CXXFLAGS_RELEASE += -g 
  QMAKE_LFLAGS += -g
  QMAKE_LFLAGS_RELEASE -= -Wl,-s
}
  
