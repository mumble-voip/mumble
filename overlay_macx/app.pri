# Template for OSX overlay CLI tools.

include(../compiler.pri)
include(common.pri)

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG *= debug_and_release

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}
