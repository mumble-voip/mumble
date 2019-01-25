# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Globally use "/debug" or "/release" as build directory,
# if it's not a shadow build, otherwise use the shadowed directory.
#
# https://wiki.qt.io/QMake-top-level-srcdir-and-builddir

# Since this file is in the "qmake" directory,
# "$$PWD/.." is the top of the source tree.
#
# clean_path() is used to clean the path.
# It's not available on Qt 4.
greaterThan(QT_MAJOR_VERSION, 4) {
  TOP_SRCDIR = $$clean_path($$PWD/..)
}

# In case absolute_path() doesn't return anything.
isEmpty(TOP_SRCDIR) {
  TOP_SRCDIR = $$PWD/..
}

# shadowed() is used to retrieve the shadowed build path.
# It's not available on Qt 4.
greaterThan(QT_MAJOR_VERSION, 4) {
  TOP_BUILDDIR = $$shadowed($$TOP_SRCDIR)
}

# In case shadowed() doesn't return anything.
isEmpty(TOP_BUILDDIR) {
  TOP_BUILDDIR = $$TOP_SRCDIR
}

# If the build and source paths are equal,
# it's not a shadow build.
equals(TOP_BUILDDIR, $$TOP_SRCDIR) {
  CONFIG(debug, debug|release) {
    DESTDIR = $$TOP_BUILDDIR/debug
  }

  CONFIG(release, debug|release) {
    DESTDIR = $$TOP_BUILDDIR/release
  }
} else {
  DESTDIR = $$TOP_BUILDDIR
}

OBJECTS_ROOT = $$DESTDIR/.obj
MOC_ROOT = $$DESTDIR/.moc
RCC_ROOT = $$DESTDIR/.qrc
UI_ROOT = $$DESTDIR/.ui

OBJECTS_DIR = $$OBJECTS_ROOT/$$TARGET
MOC_DIR = $$MOC_ROOT/$$TARGET
RCC_DIR = $$RCC_ROOT/$$TARGET
UI_DIR = $$UI_ROOT/$$TARGET
