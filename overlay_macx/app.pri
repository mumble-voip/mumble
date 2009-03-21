# Template for OSX overlay CLI tools.

include(../compiler.pri)

TEMPLATE = app
CONFIG -= qt app_bundle
CONFIG *= debug_and_release

OVERLAY_PATH = /Library/MumbleOverlay/
VERSION = 1.1.8

DEFINES *= MUMBLE_OVERLAY_MACX_PATH=\\\"$$OVERLAY_PATH\\\" VERSION=\\\"$$VERSION\\\"

CONFIG(debug, debug|release) {
  DESTDIR       = ../../debug
}

CONFIG(release, debug|release) {
  DESTDIR       = ../../release
}
