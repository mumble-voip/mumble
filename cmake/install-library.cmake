# Copyright The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# Mumble source tree or at <https://www.mumble.info/LICENSE>.

function(install_library lib component)
	get_target_property(lib_type ${lib} TYPE)

	if(NOT lib_type STREQUAL "STATIC_LIBRARY")
		# only install non-static libraries
		if(WIN32)
			install(TARGETS ${lib} RUNTIME DESTINATION "${MUMBLE_INSTALL_LIBDIR}" COMPONENT "${component}")
		else()
			install(TARGETS ${lib} LIBRARY DESTINATION "${MUMBLE_INSTALL_LIBDIR}" COMPONENT "${component}")
		endif()
	endif()
endfunction()
