VERSION		= 0.2.1
DIST		= LICENSE INSTALL mumble.pro murmur.pro mumble.pri mumble.ico
CONFIG		+= qt thread debug_and_release

debug {
  CONFIG += console
  LIBS	+= -Ldebug
}

release {
  LIBS	+= -Lrelease
}
