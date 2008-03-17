TEMPLATE = subdirs
CONFIG += debug_and_release

SUBDIRS += speexbuild plugins
SUBDIRS += src/mumble src/murmur

win32 {
  SUBDIRS += overlay
}

unix:!macx {
  SUBDIRS += overlay_gl
}

DIST=LICENSE INSTALL README README.Linux CHANGES

