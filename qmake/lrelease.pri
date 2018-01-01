# Copyright 2005-2018 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This .pri file finds the correct lrelease binary name on the system.

isEmpty(QMAKE_LRELEASE) {
	equals(QMAKE_HOST.os, "Windows")|equals(QMAKE_VERSION_INT_MAJOR, 4):win32 {
		MY_QT_INSTALL_BINS = $$[QT_INSTALL_BINS]
		WINDOWS_QT_INSTALL_BINS = $$replace(MY_QT_INSTALL_BINS, $$quote(/), $$quote(\\))
		QMAKE_LRELEASE = $$WINDOWS_QT_INSTALL_BINS\\lrelease.exe
	} else {
		QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
	}
}
