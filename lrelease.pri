# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This .pri file finds the correct lrelease binary name on the system.

isEmpty(QMAKE_LRELEASE) {
	QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
