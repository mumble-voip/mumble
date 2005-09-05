VERSION		= 0.2.1
DIST		= LICENSE INSTALL README mumble.pro murmur.pro mumble.pri mumble.ico
CONFIG		+= qt thread debug_and_release

CONFIG(debug, debug|release) {
  CONFIG += console
  LIBS	+= -Ldebug
}

CONFIG(release, debug|release) {
  LIBS	+= -Lrelease
}
