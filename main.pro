TEMPLATE = subdirs
CONFIG += ordered debug_and_release

!CONFIG(no-client) {
  !CONFIG(no-bundled-speex) {
    SUBDIRS += speexbuild 
  }
  SUBDIRS += src/mumble plugins

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
