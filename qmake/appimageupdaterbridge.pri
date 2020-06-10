# Copyright 2005-2020 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

include(pkgconfig.pri)

# Please note that you have to make sure that AppImageUpdaterBridge's
# pkgconfig files are available in the $PKG_CONFIG_PATH.
# Because most of the time it's not.
unix {
	APPIMAGE_UPDATER_BRIDGE_ENABLED {
		DEFINES += USE_APPIMAGE_UPDATER_BRIDGE
		CONFIG *= link_pkgconfig
		must_pkgconfig(AppImageUpdaterBridge)
		message(Will be using AppImageUpdaterBridge)
	}
} else {
	APPIMAGE_UPDATER_BRIDGE_ENABLED {
		error(You cannot use AppImageUpdaterBridge for this type of build)
	}
}
