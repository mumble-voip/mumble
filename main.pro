TEMPLATE = subdirs
CONFIG += debug_and_release

!CONFIG(no-client) {
  SUBDIRS += speexbuild plugins
  SUBDIRS += src/mumble

  win32 {
    SUBDIRS += overlay
  }

  unix:!macx {
    SUBDIRS += overlay_gl
  }
}

!CONFIG(no-server) {
  SUBDIRS += src/murmur
}

DIST=LICENSE INSTALL README README.Linux CHANGES

