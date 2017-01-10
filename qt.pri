# Copyright 2005-2017 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

QT_VERSION_INT_MAJOR = $$QT_MAJOR_VERSION

greaterThan(QT_MINOR_VERSION, 99) {
	error(bad Qt minor version)
} greaterThan(QT_MINOR_VERSION, 9) {
	QT_VERSION_INT_MINOR = "$$QT_MINOR_VERSION"
} else {
	QT_VERSION_INT_MINOR = "0$$QT_MINOR_VERSION"
}

greaterThan(QT_PATCH_VERSION, 99) {
	error(bad Qt patch version)
} greaterThan(QT_PATCH_VERSION, 9) {
	QT_VERSION_INT_PATCH = "$$QT_PATCH_VERSION"
} else {
	QT_VERSION_INT_PATCH = "0$$QT_PATCH_VERSION"
}

QT_VERSION_INT = "$$QT_VERSION_INT_MAJOR$$QT_VERSION_INT_MINOR$$QT_VERSION_INT_PATCH"
