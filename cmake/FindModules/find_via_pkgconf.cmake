# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

find_package(PkgConfig)
include(FindPackageHandleStandardArgs)

macro(mumble_find_via_pkgconf DEP_NAME TARGET_NAME PKGCONF_SPEC)
	if (PkgConfig_FOUND AND NOT TARGET "${TARGET_NAME}")
		pkg_search_module(Mumble_${DEP_NAME} IMPORTED_TARGET "${PKGCONF_SPEC}")

		if (Mumble_${DEP_NAME}_FOUND)
			add_library(${TARGET_NAME} ALIAS PkgConfig::Mumble_${DEP_NAME})
			set(${DEP_NAME}_VERSION "${Mumble_${DEP_NAME}_VERSION}")
		endif()
	endif()

	find_package_handle_standard_args(${DEP_NAME}
		REQUIRED_VARS PkgConfig_FOUND Mumble_${DEP_NAME}_FOUND
		VERSION_VAR ${DEP_NAME}_VERSION
		HANDLE_VERSION_RANGE
	)
endmacro()
