# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(qmake/compiler.pri)

TEMPLATE = subdirs
CONFIG *= ordered debug_and_release

SUBDIRS *= src/mumble_proto

!CONFIG(no-client) {
  unix:!CONFIG(bundled-speex):system(pkg-config --atleast-version=1.2 speexdsp):system(pkg-config --atleast-version=1.2 speex) {
    CONFIG *= no-bundled-speex
  }
  !CONFIG(no-bundled-speex) {
    SUBDIRS *= 3rdparty/speex-build
  }

  CONFIG(sbcelt) {
    SUBDIRS *= 3rdparty/celt-0.7.0-build 3rdparty/sbcelt-lib-build 3rdparty/sbcelt-helper-build
  } else {
    unix:!CONFIG(bundled-celt):system(pkg-config --atleast-version=0.7.0 celt) {
      CONFIG *= no-bundled-celt
    }
    !CONFIG(no-bundled-celt) {
      SUBDIRS *= 3rdparty/celt-0.7.0-build 3rdparty/celt-0.11.0-build
    }
  }

  !CONFIG(no-opus) {
    CONFIG *= opus
  }

  CONFIG(opus):!CONFIG(no-bundled-opus) {
    SUBDIRS *= 3rdparty/opus-build
  }

  win32 {
    SUBDIRS *= 3rdparty/xinputcheck-build
  }

  SUBDIRS *= src/mumble

  win32:CONFIG(static) {
    SUBDIRS *= src/mumble_exe
  }

  !CONFIG(no-plugins) {
    SUBDIRS *= plugins
  }

  win32:!CONFIG(no-overlay) {
    SUBDIRS *= 3rdparty/minhook-build
    SUBDIRS *= overlay
    SUBDIRS *= overlay/overlay_exe
    SUBDIRS *= overlay_winx64
    SUBDIRS *= overlay_winx64/overlay_exe_winx64
  }

  win32:!CONFIG(no-g15) {
    SUBDIRS *= g15helper
  }

  contains(UNAME, OpenBSD) {
    CONFIG *= no-overlay
  }

  unix:!macx:!CONFIG(no-overlay) {
    SUBDIRS *= overlay_gl
  }

  macx {
    !CONFIG(buildenv) {
      error("Not inside a Mumble buildenv ($MUMBLE_PREFIX environment variable is missing)");
    }
    SUBDIRS *= 3rdparty/mach-override-build
    SUBDIRS *= overlay_gl
    SUBDIRS *= macx
    !exists($$(MUMBLE_PREFIX)/../LCDSDK) {
      CONFIG *= no-g15
    }
    !CONFIG(no-g15) {
      SUBDIRS *= g15helper
    }
  }
}

!CONFIG(no-server) {
  !CONFIG(no-ice) {
    SUBDIRS *= src/murmur/murmur_ice
  }
  CONFIG(grpc) {
    !system(pkg-config --atleast-version=3 protobuf) {
      error(grpc requires protobuf>=3)
    }
    SUBDIRS *= src/murmur_grpcwrapper_protoc_plugin
    SUBDIRS *= src/murmur/murmur_grpc
  }
  SUBDIRS *= src/murmur
}

CONFIG(tests) {
  SUBDIRS *= src/tests
}

DIST=LICENSE INSTALL README README.Linux CHANGES

include(scripts/scripts.pro)
