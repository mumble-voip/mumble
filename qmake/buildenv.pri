# Copyright 2005-2019 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

# This .pri detects whether Mumble is being built in a
# Mumble buildenv, and if so, sets the buildenv CONFIG.
#
# It also adds USE_BUILDENV as a preprocessor define
# such that code can detect the presence of a buildenv
# at build time.

MUMBLE_PREFIX=$$(MUMBLE_PREFIX)
!isEmpty(MUMBLE_PREFIX) {
	CONFIG += buildenv
}

CONFIG(buildenv) {
	DEFINES += USE_BUILDENV
}
