TEMPLATE = subdirs
CONFIG *= ordered debug_and_release

!CONFIG(no-client) {
  unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp) {
	CONFIG *= no-bundled-speex
  }
  unix:!CONFIG(bundled-celt):system(pkg-config --atleast-version=0.7.0 celt) {
	CONFIG *= no-bundled-celt
  }
  !CONFIG(no-bundled-speex) {
    SUBDIRS *= speexbuild
  }
  !CONFIG(no-bundled-celt) {
    SUBDIRS *= celt-0.7.0-build
  }
  !CONFIG(no-11x) {
    SUBDIRS *= src/mumble11x
  }
  SUBDIRS *= src/mumble plugins

  win32 {
    SUBDIRS *= overlay
    !CONFIG(no-g15) {
      SUBDIRS *= g15helper
    }
  }

  unix:!macx:!CONFIG(no-overlay) {
    SUBDIRS *= overlay_gl
  }

  macx {
    SUBDIRS *= overlay_macx
    !CONFIG(no-g15) {
      SUBDIRS *= g15helper
    }
  }
}

!CONFIG(no-server) {
  SUBDIRS *= src/murmur
}

DIST=LICENSE INSTALL README README.Linux CHANGES

include(scripts/scripts.pro)
