# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# Allow cross-building by making a call to return the pkg-config
# that the user supplied to the build.
# pkgConfigExecutable() is part of Qt5.
# If building with Qt4, pkg-config is called instead
isEqual(QT_MAJOR_VERSION, 5) {
	PKG_CONFIG = $$pkgConfigExecutable()
} else {
	PKG_CONFIG = "pkg-config"
}

# must_pkgconfig(pkg)
#
# This function checks if the passed-in package
# name is available on the system. If it is,
# it is added to the PKGCONFIG variable.
# If not, exit qmake with a fatal error stating
# that the package could not be found.
#
# Example usage:
#
#     must_pkgconfig(openssl)
#
defineTest(must_pkgconfig) {
	pkg = $$1
	system($$PKG_CONFIG --exists $$pkg) {
		PKGCONFIG *= $$pkg
		export(PKGCONFIG)
	} else {
		error(pkg-config could not find package $$pkg)
	}
}
