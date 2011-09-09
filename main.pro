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
    SUBDIRS *= celt-0.7.0-build celt-0.11.0-build
  }

  SUBDIRS *= src/mumble

  !CONFIG(no-plugins) {
    SUBDIRS *= plugins
  }

  win32 {
    SUBDIRS *= overlay
  }

  unix:!macx:!CONFIG(no-overlay) {
    SUBDIRS *= overlay_gl
  }

  macx {
    MUMBLE_PREFIX = $$(MUMBLE_PREFIX)
    isEmpty(MUMBLE_PREFIX) {
      error("Missing $MUMBLE_PREFIX environment variable");
    }
    SUBDIRS *= macx
    !exists($$(MUMBLE_PREFIX)/../lglcd) {
      CONFIG *= no-g15
    }
    !CONFIG(no-g15) {
      SUBDIRS *= g15helper
    }
  }
}

!CONFIG(no-server) {
  SUBDIRS *= src/murmur
}

!CONFIG(no-mutter) {
  !CONFIG(no-ice) {
    SUBDIRS *= src/mutter
  }
}

DIST=LICENSE INSTALL README README.Linux CHANGES

include(scripts/scripts.pro)
