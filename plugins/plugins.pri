include(../compiler.pri)

CONFIG(debug, debug|release) {
  CONFIG += console
  DESTDIR       = ../../debug/plugins
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release/plugins
}

