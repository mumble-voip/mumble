TEMPLATE = subdirs
CONFIG *= ordered debug_and_release

!CONFIG(no-client) {
  unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp) {
	CONFIG *= no-bundled-speex
  }
  !CONFIG(no-bundled-speex) {
    SUBDIRS *= speexbuild 
  }
  SUBDIRS *= src/mumble plugins

  win32 {
    SUBDIRS *= overlay
    !CONFIG(no-g15) {
      SUBDIRS *= g15helper
    }
  }

  unix:!macx {
    SUBDIRS *= overlay_gl
  }

  macx {
    SUBDIRS *= overlay_macx
  }
}

!CONFIG(no-server) {
  SUBDIRS *= src/murmur
}

DIST=LICENSE INSTALL README README.Linux CHANGES compiler.pri
