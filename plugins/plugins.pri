include(../compiler.pri)

TEMPLATE	= lib
CONFIG		+= plugin debug_and_release warn_on
CONFIG		-= qt

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR       = ../../debug/plugins
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release/plugins
}

